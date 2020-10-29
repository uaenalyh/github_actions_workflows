/*
 * Copyright (C) 2018 Intel Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <types.h>
#include <errno.h>
#include <bits.h>
#include <irq.h>
#include <io.h>
#include <console.h>
#include <per_cpu.h>
#include <vmx.h>
#include <cpuid.h>
#include <ptdev.h>
#include <vm.h>
#include <logmsg.h>
#include <version.h>
#include "vuart.h"
#include "shell_priv.h"
#include "lib.h"
#include "config_debug.h"
#include "pgtable.h"
#include "idt.h"

#define TEMP_STR_SIZE		60U
#define MAX_STR_SIZE		256U
#define SHELL_PROMPT_STR	"ACRN:\\>"

#define SHELL_LOG_BUF_SIZE		(PAGE_SIZE * MAX_PCPU_NUM / 2U)
static char shell_log_buf[SHELL_LOG_BUF_SIZE];

static uint64_t save_exception_entry;

/* Input Line Other - Switch to the "other" input line (there are only two
 * input lines total).
 */
#define SHELL_INPUT_LINE_OTHER(v)	(((v) + 1U) & 0x1U)

static int32_t shell_cmd_help(__unused int32_t argc, __unused char **argv);
static int32_t shell_version(__unused int32_t argc, __unused char **argv);
static int32_t shell_list_vm(__unused int32_t argc, __unused char **argv);
static int32_t shell_list_vcpu(__unused int32_t argc, __unused char **argv);
static int32_t shell_vcpu_dumpreg(int32_t argc, char **argv);
static int32_t shell_dumpmem(int32_t argc, char **argv);
static int32_t shell_to_vm_console(int32_t argc, char **argv);
static int32_t shell_show_ptdev_info(__unused int32_t argc, __unused char **argv);
static int32_t shell_loglevel(int32_t argc, char **argv);
static int32_t shell_cpuid(int32_t argc, char **argv);
static int32_t shell_trigger_reboot(int32_t argc, char **argv);
static int32_t shell_rdmsr(int32_t argc, char **argv);
static int32_t shell_wrmsr(int32_t argc, char **argv);
static int shell_start_test(int argc, char **argv);
static int shell_stop_test(__unused int argc, __unused char **argv);
static int shell_inject_mc(__unused int argc, __unused char **argv);

static struct shell_cmd shell_cmds[] = {
	{
		.str		= SHELL_CMD_HELP,
		.cmd_param	= SHELL_CMD_HELP_PARAM,
		.help_str	= SHELL_CMD_HELP_HELP,
		.fcn		= shell_cmd_help,
	},
	{
		.str		= SHELL_CMD_VERSION,
		.cmd_param	= SHELL_CMD_VERSION_PARAM,
		.help_str	= SHELL_CMD_VERSION_HELP,
		.fcn		= shell_version,
	},
	{
		.str		= SHELL_CMD_VM_LIST,
		.cmd_param	= SHELL_CMD_VM_LIST_PARAM,
		.help_str	= SHELL_CMD_VM_LIST_HELP,
		.fcn		= shell_list_vm,
	},
	{
		.str		= SHELL_CMD_VCPU_LIST,
		.cmd_param	= SHELL_CMD_VCPU_LIST_PARAM,
		.help_str	= SHELL_CMD_VCPU_LIST_HELP,
		.fcn		= shell_list_vcpu,
	},
	{
		.str		= SHELL_CMD_VCPU_DUMPREG,
		.cmd_param	= SHELL_CMD_VCPU_DUMPREG_PARAM,
		.help_str	= SHELL_CMD_VCPU_DUMPREG_HELP,
		.fcn		= shell_vcpu_dumpreg,
	},
	{
		.str		= SHELL_CMD_DUMPMEM,
		.cmd_param	= SHELL_CMD_DUMPMEM_PARAM,
		.help_str	= SHELL_CMD_DUMPMEM_HELP,
		.fcn		= shell_dumpmem,
	},
	{
		.str		= SHELL_CMD_VM_CONSOLE,
		.cmd_param	= SHELL_CMD_VM_CONSOLE_PARAM,
		.help_str	= SHELL_CMD_VM_CONSOLE_HELP,
		.fcn		= shell_to_vm_console,
	},
	{
		.str		= SHELL_CMD_PTDEV,
		.cmd_param	= SHELL_CMD_PTDEV_PARAM,
		.help_str	= SHELL_CMD_PTDEV_HELP,
		.fcn		= shell_show_ptdev_info,
	},
	{
		.str		= SHELL_CMD_LOG_LVL,
		.cmd_param	= SHELL_CMD_LOG_LVL_PARAM,
		.help_str	= SHELL_CMD_LOG_LVL_HELP,
		.fcn		= shell_loglevel,
	},
	{
		.str		= SHELL_CMD_CPUID,
		.cmd_param	= SHELL_CMD_CPUID_PARAM,
		.help_str	= SHELL_CMD_CPUID_HELP,
		.fcn		= shell_cpuid,
	},
	{
		.str		= SHELL_CMD_REBOOT,
		.cmd_param	= SHELL_CMD_REBOOT_PARAM,
		.help_str	= SHELL_CMD_REBOOT_HELP,
		.fcn		= shell_trigger_reboot,
	},
	{
		.str		= SHELL_CMD_RDMSR,
		.cmd_param	= SHELL_CMD_RDMSR_PARAM,
		.help_str	= SHELL_CMD_RDMSR_HELP,
		.fcn		= shell_rdmsr,
	},
	{
		.str		= SHELL_CMD_WRMSR,
		.cmd_param	= SHELL_CMD_WRMSR_PARAM,
		.help_str	= SHELL_CMD_WRMSR_HELP,
		.fcn		= shell_wrmsr,
	},
	{
		.str		= SHELL_CMD_START_TEST,
		.cmd_param	= SHELL_CMD_START_TEST_PARAM,
		.help_str	= SHELL_CMD_START_TEST_HELP,
		.fcn		= shell_start_test,
	},
	{
		.str		= SHELL_CMD_STOP_TEST,
		.cmd_param	= SHELL_CMD_STOP_TEST_PARAM,
		.help_str	= SHELL_CMD_STOP_TEST_HELP,
		.fcn		= shell_stop_test,
	},
	{
		.str		= SHELL_CMD_INJECT_MC,
		.cmd_param	= SHELL_CMD_INJECT_MC_PARAM,
		.help_str	= SHELL_CMD_INJECT_MC_HELP,
		.fcn		= shell_inject_mc,
	},
};

/* The initial log level*/
uint16_t console_loglevel = CONFIG_CONSOLE_LOGLEVEL_DEFAULT;

static struct shell hv_shell;
static struct shell *p_shell = &hv_shell;

static int32_t string_to_argv(char *argv_str, void *p_argv_mem,
		__unused uint32_t argv_mem_size,
		uint32_t *p_argc, char ***p_argv)
{
	uint32_t argc;
	char **argv;
	char *p_ch;

	/* Setup initial argument values. */
	argc = 0U;
	argv = NULL;

	/* Ensure there are arguments to be processed. */
	if (argv_str == NULL) {
		*p_argc = argc;
		*p_argv = argv;
		return -EINVAL;
	}

	/* Process the argument string (there is at least one element). */
	argv = (char **)p_argv_mem;
	p_ch = argv_str;

	/* Remove all spaces at the beginning of cmd*/
	while (*p_ch == ' ') {
		p_ch++;
	}

	while (*p_ch != 0) {
		/* Add argument (string) pointer to the vector. */
		argv[argc] = p_ch;

		/* Move past the vector entry argument string (in the
		 * argument string).
		 */
		while ((*p_ch != ' ') && (*p_ch != ',') && (*p_ch != 0)) {
			p_ch++;
		}

		/* Count the argument just processed. */
		argc++;

		/* Check for the end of the argument string. */
		if (*p_ch != 0) {
			/* Terminate the vector entry argument string
			 * and move to the next.
			 */
			*p_ch = 0;
			/* Remove all space in middile of cmdline */
			p_ch++;
			while (*p_ch == ' ') {
				p_ch++;
			}
		}
	}

	/* Update return parameters */
	*p_argc = argc;
	*p_argv = argv;

	return 0;
}

static struct shell_cmd *shell_find_cmd(const char *cmd_str)
{
	uint32_t i;
	struct shell_cmd *p_cmd = NULL;

	for (i = 0U; i < p_shell->cmd_count; i++) {
		p_cmd = &p_shell->cmds[i];
		if (strcmp(p_cmd->str, cmd_str) == 0) {
			return p_cmd;
		}
	}
	return NULL;
}

static char shell_getc(void)
{
	return console_getc();
}

static void shell_puts(const char *string_ptr)
{
	/* Output the string */
	(void)console_write(string_ptr, strnlen_s(string_ptr,
				SHELL_STRING_MAX_LEN));
}

static uint16_t sanitize_vmid(uint16_t vmid)
{
	uint16_t sanitized_vmid = vmid;
	char temp_str[TEMP_STR_SIZE];

	if (vmid >= CONFIG_MAX_VM_NUM) {
		snprintf(temp_str, MAX_STR_SIZE,
			"VM ID given exceeds the MAX_VM_NUM(%u), using 0 instead\r\n",
			CONFIG_MAX_VM_NUM);
		shell_puts(temp_str);
		sanitized_vmid = 0U;
	}

	return sanitized_vmid;
}

static void shell_handle_special_char(char ch)
{
	switch (ch) {
	/* Escape character */
	case 0x1b:
		/* Consume the next 2 characters */
		(void) shell_getc();
		(void) shell_getc();
		break;
	default:
		/*
		 * Only the Escape character is treated as special character.
		 * All the other characters have been handled properly in
		 * shell_input_line, so they will not be handled in this API.
		 * Gracefully return if prior case clauses have not been met.
		 */
		break;
	}
}

static bool shell_input_line(void)
{
	bool done = false;
	char ch;

	ch = shell_getc();

	/* Check character */
	switch (ch) {
	/* Backspace */
	case '\b':
		/* Ensure length is not 0 */
		if (p_shell->input_line_len > 0U) {
			/* Reduce the length of the string by one */
			p_shell->input_line_len--;

			/* Null terminate the last character to erase it */
			p_shell->input_line[p_shell->input_line_active]
					[p_shell->input_line_len] = 0;

			/* Echo backspace */
			shell_puts("\b");

			/* Send a space + backspace sequence to delete
			 * character
			 */
			shell_puts(" \b");
		}
		break;

	/* Carriage-return */
	case '\r':
		/* Echo carriage return / line feed */
		shell_puts("\r\n");

		/* Set flag showing line input done */
		done = true;

		/* Reset command length for next command processing */
		p_shell->input_line_len = 0U;
		break;

	/* Line feed */
	case '\n':
		/* Do nothing */
		break;

	/* All other characters */
	default:
		/* Ensure data doesn't exceed full terminal width */
		if (p_shell->input_line_len < SHELL_CMD_MAX_LEN) {
			/* See if a "standard" prINTable ASCII character received */
			if ((ch >= 32) && (ch <= 126)) {
				/* Add character to string */
				p_shell->input_line[p_shell->input_line_active]
						[p_shell->input_line_len] = ch;
				/* Echo back the input */
				shell_puts(&p_shell->input_line
						[p_shell->input_line_active]
						[p_shell->input_line_len]);

				/* Move to next character in string */
				p_shell->input_line_len++;
			} else {
				/* call special character handler */
				shell_handle_special_char(ch);
			}
		} else {
			/* Echo carriage return / line feed */
			shell_puts("\r\n");

			/* Set flag showing line input done */
			done = true;

			/* Reset command length for next command processing */
			p_shell->input_line_len = 0U;

		}
		break;
	}


	return done;
}

static int32_t shell_process_cmd(const char *p_input_line)
{
	int32_t status = -EINVAL;
	struct shell_cmd *p_cmd;
	char cmd_argv_str[SHELL_CMD_MAX_LEN + 1U];
	int32_t cmd_argv_mem[sizeof(char *) * ((SHELL_CMD_MAX_LEN + 1U) >> 1U)];
	int32_t cmd_argc;
	char **cmd_argv;

	/* Copy the input line INTo an argument string to become part of the
	 * argument vector.
	 */
	(void)strncpy_s(&cmd_argv_str[0], SHELL_CMD_MAX_LEN + 1U, p_input_line, SHELL_CMD_MAX_LEN);
	cmd_argv_str[SHELL_CMD_MAX_LEN] = 0;

	/* Build the argv vector from the string. The first argument in the
	 * resulting vector will be the command string itself.
	 */

	/* NOTE: This process is destructive to the argument string! */

	(void) string_to_argv(&cmd_argv_str[0],
			(void *) &cmd_argv_mem[0],
			sizeof(cmd_argv_mem), (void *)&cmd_argc, &cmd_argv);

	/* Determine if there is a command to process. */
	if (cmd_argc != 0) {
		/* See if command is in cmds supported */
		p_cmd = shell_find_cmd(cmd_argv[0]);
		if (p_cmd == NULL) {
			shell_puts("\r\nError: Invalid command.\r\n");
			return -EINVAL;
		}

		status = p_cmd->fcn(cmd_argc, &cmd_argv[0]);
		if (status == -EINVAL) {
			shell_puts("\r\nError: Invalid parameters.\r\n");
		} else if (status != 0) {
			shell_puts("\r\nCommand launch failed.\r\n");
		} else {
			/* No other state currently, do nothing */
		}
	}

	return status;
}

static int32_t shell_process(void)
{
	int32_t status;
	char *p_input_line;

	/* Check for the repeat command character in active input line.
	 */
	if (p_shell->input_line[p_shell->input_line_active][0] == '.') {
		/* Repeat the last command (using inactive input line).
		 */
		p_input_line =
			&p_shell->input_line[SHELL_INPUT_LINE_OTHER
				(p_shell->input_line_active)][0];
	} else {
		/* Process current command (using active input line). */
		p_input_line =
			&p_shell->input_line[p_shell->input_line_active][0];

		/* Switch active input line. */
		p_shell->input_line_active =
			SHELL_INPUT_LINE_OTHER(p_shell->input_line_active);
	}

	/* Process command */
	status = shell_process_cmd(p_input_line);

	/* Now that the command is processed, zero fill the input buffer */
	(void)memset((void *) p_shell->input_line[p_shell->input_line_active],
			0, SHELL_CMD_MAX_LEN + 1U);

	/* Process command and return result to caller */
	return status;
}


void shell_kick(void)
{
	static bool is_cmd_cmplt = true;

	/* At any given instance, UART may be owned by the HV
	 * OR by the guest that has enabled the vUart.
	 * Show HV shell prompt ONLY when HV owns the
	 * serial port.
	 */
	/* Prompt the user for a selection. */
	if (is_cmd_cmplt) {
		shell_puts(SHELL_PROMPT_STR);
	}

	/* Get user's input */
	is_cmd_cmplt = shell_input_line();

	/* If user has pressed the ENTER then process
	 * the command
	 */
	if (is_cmd_cmplt) {
		/* Process current input line. */
		(void)shell_process();
	}
}


void shell_init(void)
{
	p_shell->cmds = shell_cmds;
	p_shell->cmd_count = ARRAY_SIZE(shell_cmds);

	/* Zero fill the input buffer */
	(void)memset((void *)p_shell->input_line[p_shell->input_line_active], 0U,
			SHELL_CMD_MAX_LEN + 1U);
}

#define SHELL_ROWS	10
#define MAX_INDENT_LEN	16U
static int32_t shell_cmd_help(__unused int32_t argc, __unused char **argv)
{
	uint16_t spaces;
	struct shell_cmd *p_cmd = NULL;
	char space_buf[MAX_INDENT_LEN + 1];

	/* Print title */
	shell_puts("\r\nRegistered Commands:\r\n\r\n");

	pr_dbg("shell: Number of registered commands = %u in %s\n",
		p_shell->cmd_count, __func__);

	(void)memset(space_buf, ' ', sizeof(space_buf));
	/* Proceed based on the number of registered commands. */
	if (p_shell->cmd_count == 0U) {
		/* No registered commands */
		shell_puts("NONE\r\n");
	} else {
		int32_t i = 0;
		uint32_t j;

		for (j = 0U; j < p_shell->cmd_count; j++) {
			p_cmd = &p_shell->cmds[j];

			/* Check if we've filled the screen with info */
			/* i + 1 used to avoid 0%SHELL_ROWS=0 */
			if (((i + 1) % SHELL_ROWS) == 0) {
				/* Pause before we continue on to the next
				 * page.
				 */

				/* Print message to the user. */
				shell_puts("<*** Hit any key to continue ***>");

				/* Wait for a character from user (NOT USED) */
				(void)shell_getc();

				/* Print a new line after the key is hit. */
				shell_puts("\r\n");
			}

			i++;

			/* Output the command string */
			shell_puts("  ");
			shell_puts(p_cmd->str);

			/* Calculate spaces needed for alignment */
			spaces = MAX_INDENT_LEN - strnlen_s(p_cmd->str, MAX_INDENT_LEN - 1);

			space_buf[spaces] = '\0';
			shell_puts(space_buf);
			space_buf[spaces] = ' ';

			/* Display parameter info if applicable. */
			if (p_cmd->cmd_param != NULL) {
				shell_puts(p_cmd->cmd_param);
			}

			/* Display help text if available. */
			if (p_cmd->help_str != NULL) {
				shell_puts(" - ");
				shell_puts(p_cmd->help_str);
			}
			shell_puts("\r\n");
		}
	}

	shell_puts("\r\n");

	return 0;
}

static int32_t shell_version(__unused int32_t argc, __unused char **argv)
{
	char temp_str[MAX_STR_SIZE];

	snprintf(temp_str, MAX_STR_SIZE, "HV version %s-%s-%s %s (daily tag: %s) build by %s\r\n",
			HV_FULL_VERSION, HV_BUILD_TIME, HV_BUILD_VERSION, HV_BUILD_TYPE, HV_DAILY_TAG, HV_BUILD_USER);
	shell_puts(temp_str);

	(void)memset((void *)temp_str, 0, MAX_STR_SIZE);
	snprintf(temp_str, MAX_STR_SIZE, "API version %u.%u\r\n", HV_API_MAJOR_VERSION, HV_API_MINOR_VERSION);
	shell_puts(temp_str);

	return 0;
}

static int32_t shell_list_vm(__unused int32_t argc, __unused char **argv)
{
	char temp_str[MAX_STR_SIZE];
	struct acrn_vm *vm;
	struct acrn_vm_config *vm_config;
	uint16_t vm_id;
	char state[32];

	shell_puts("\r\nVM_UUID                          VM_ID VM_NAME                          VM_STATE"
		   "\r\n================================ ===== ================================ ========\r\n");

	for (vm_id = 0U; vm_id < CONFIG_MAX_VM_NUM; vm_id++) {
		vm = get_vm_from_vmid(vm_id);
		switch (vm->state) {
		case VM_CREATED:
			(void)strncpy_s(state, 32U, "Created", 32U);
			break;
		case VM_STARTED:
			(void)strncpy_s(state, 32U, "Started", 32U);
			break;
		case VM_PAUSED:
			(void)strncpy_s(state, 32U, "Paused", 32U);
			break;
		case VM_POWERED_OFF:
			(void)strncpy_s(state, 32U, "Off", 32U);
			break;
		default:
			(void)strncpy_s(state, 32U, "Unknown", 32U);
			break;
		}
		vm_config = get_vm_config(vm_id);
		if (vm->state != VM_POWERED_OFF) {
			int8_t i;

			for (i = 0; i < 16; i++) {
				snprintf(temp_str + 2 * i, 3U, "%02x", vm->uuid[i]);
			}
			snprintf(temp_str + 32, MAX_STR_SIZE - 32U, "   %-3d %-32s %-8s\r\n",
				vm_id, vm_config->name, state);

			/* Output information for this task */
			shell_puts(temp_str);
		}
	}

	return 0;
}

static int32_t shell_list_vcpu(__unused int32_t argc, __unused char **argv)
{
	char temp_str[MAX_STR_SIZE];
	struct acrn_vm *vm;
	struct acrn_vcpu *vcpu;
	char state[32];
	uint16_t i;
	uint16_t idx;

	shell_puts("\r\nVM ID    PCPU ID    VCPU ID    VCPU ROLE    VCPU STATE"
		"\r\n=====    =======    =======    =========    ==========\r\n");

	for (idx = 0U; idx < CONFIG_MAX_VM_NUM; idx++) {
		vm = get_vm_from_vmid(idx);
		if (vm->state == VM_POWERED_OFF) {
			continue;
		}
		foreach_vcpu(i, vm, vcpu) {
			switch (vcpu->state) {
			case VCPU_INIT:
				(void)strncpy_s(state, 32U, "Init", 32U);
				break;
			case VCPU_PAUSED:
				(void)strncpy_s(state, 32U, "Paused", 32U);
				break;
			case VCPU_RUNNING:
				(void)strncpy_s(state, 32U, "Running", 32U);
				break;
			case VCPU_ZOMBIE:
				(void)strncpy_s(state, 32U, "Zombie", 32U);
				break;
			default:
				(void)strncpy_s(state, 32U, "Unknown", 32U);
			}
			/* Create output string consisting of VM name
			 * and VM id
			 */
			snprintf(temp_str, MAX_STR_SIZE,
					"  %-9d %-10d %-7hu %-12s %-16s\r\n",
					vm->vm_id,
					pcpuid_from_vcpu(vcpu),
					vcpu->vcpu_id,
					is_vcpu_bsp(vcpu) ?
					"PRIMARY" : "SECONDARY",
					state);
			/* Output information for this task */
			shell_puts(temp_str);
		}
	}

	return 0;
}

#define DUMPREG_SP_SIZE	32
/* the input 'data' must != NULL and indicate a vcpu structure pointer */
static void vcpu_dumpreg(void *data)
{
	struct vcpu_dump *dump = data;
	struct acrn_vcpu *vcpu = dump->vcpu;
	char *str = dump->str;
	size_t len, size = dump->str_max;

	len = snprintf(str, size,
		"=  VM ID %d ==== CPU ID %hu========================\r\n"
		"=  RIP=0x%016llx  RSP=0x%016llx RFLAGS=0x%016llx\r\n"
		"=  CR0=0x%016llx  CR2=0x%016llx\r\n"
		"=  CR3=0x%016llx  CR4=0x%016llx\r\n"
		"=  RAX=0x%016llx  RBX=0x%016llx RCX=0x%016llx\r\n"
		"=  RDX=0x%016llx  RDI=0x%016llx RSI=0x%016llx\r\n"
		"=  RBP=0x%016llx  R8=0x%016llx R9=0x%016llx\r\n"
		"=  R10=0x%016llx  R11=0x%016llx R12=0x%016llx\r\n"
		"=  R13=0x%016llx  R14=0x%016llx  R15=0x%016llx\r\n",
		vcpu->vm->vm_id, vcpu->vcpu_id,
		vcpu_get_rip(vcpu),
		vcpu_get_gpreg(vcpu, CPU_REG_RSP),
		vcpu_get_rflags(vcpu),
		vcpu_get_cr0(vcpu), vcpu_get_cr2(vcpu),
		exec_vmread(VMX_GUEST_CR3), vcpu_get_cr4(vcpu),
		vcpu_get_gpreg(vcpu, CPU_REG_RAX),
		vcpu_get_gpreg(vcpu, CPU_REG_RBX),
		vcpu_get_gpreg(vcpu, CPU_REG_RCX),
		vcpu_get_gpreg(vcpu, CPU_REG_RDX),
		vcpu_get_gpreg(vcpu, CPU_REG_RDI),
		vcpu_get_gpreg(vcpu, CPU_REG_RSI),
		vcpu_get_gpreg(vcpu, CPU_REG_RBP),
		vcpu_get_gpreg(vcpu, CPU_REG_R8),
		vcpu_get_gpreg(vcpu, CPU_REG_R9),
		vcpu_get_gpreg(vcpu, CPU_REG_R10),
		vcpu_get_gpreg(vcpu, CPU_REG_R11),
		vcpu_get_gpreg(vcpu, CPU_REG_R12),
		vcpu_get_gpreg(vcpu, CPU_REG_R13),
		vcpu_get_gpreg(vcpu, CPU_REG_R14),
		vcpu_get_gpreg(vcpu, CPU_REG_R15));
	if (len >= size) {
		goto overflow;
	}
	size -= len;
	str += len;

	/* copy_from_gva fail */
	len = snprintf(str, size, "Cannot handle user gva yet!\r\n");
	if (len >= size) {
		goto overflow;
	}
	size -= len;
	str += len;

	return;

overflow:
	printf("buffer size could not be enough! please check!\n");
}

static int32_t shell_vcpu_dumpreg(int32_t argc, char **argv)
{
	int32_t status = 0;
	uint16_t vm_id;
	uint16_t vcpu_id;
	struct acrn_vm *vm;
	struct acrn_vcpu *vcpu;
	uint64_t mask = 0UL;
	struct vcpu_dump dump;

	/* User input invalidation */
	if (argc != 3) {
		shell_puts("Please enter cmd with <vm_id, vcpu_id>\r\n");
		status = -EINVAL;
		goto out;
	}

	status = strtol_deci(argv[1]);
	if (status < 0) {
		goto out;
	}
	vm_id = sanitize_vmid((uint16_t)status);
	vcpu_id = (uint16_t)strtol_deci(argv[2]);

	vm = get_vm_from_vmid(vm_id);
	if (vm->state == VM_POWERED_OFF) {
		shell_puts("No vm found in the input <vm_id, vcpu_id>\r\n");
		status = -EINVAL;
		goto out;
	}

	if (vcpu_id >= vm->hw.created_vcpus) {
		shell_puts("vcpu id is out of range\r\n");
		status = -EINVAL;
		goto out;
	}

	vcpu = vcpu_from_vid(vm, vcpu_id);
	if (vcpu->state == VCPU_OFFLINE) {
		shell_puts("vcpu is offline\r\n");
		status = -EINVAL;
		goto out;
	}

	dump.vcpu = vcpu;
	dump.str = shell_log_buf;
	dump.str_max = SHELL_LOG_BUF_SIZE;
	if (pcpuid_from_vcpu(vcpu) == get_pcpu_id()) {
		vcpu_dumpreg(&dump);
	} else {
		bitmap_set_nolock(pcpuid_from_vcpu(vcpu), &mask);
		/* smp_call_function(mask, vcpu_dumpreg, &dump); */
	}
	shell_puts(shell_log_buf);
	status = 0;

out:
	return status;
}

#define MAX_MEMDUMP_LEN		(32U * 8U)
static int32_t shell_dumpmem(int32_t argc, char **argv)
{
	uint64_t addr;
	uint64_t *ptr;
	uint32_t i, length;
	char temp_str[MAX_STR_SIZE];

	/* User input invalidation */
	if ((argc != 2) && (argc != 3)) {
		return -EINVAL;
	}

	addr = strtoul_hex(argv[1]);
	length = (uint32_t)strtol_deci(argv[2]);
	if (length > MAX_MEMDUMP_LEN) {
		shell_puts("over max length, round back\r\n");
		length = MAX_MEMDUMP_LEN;
	}

	snprintf(temp_str, MAX_STR_SIZE,
		"Dump physical memory addr: 0x%016llx, length %d:\r\n",
		addr, length);
	shell_puts(temp_str);

	ptr = (uint64_t *)addr;
	for (i = 0U; i < (length >> 5U); i++) {
		snprintf(temp_str, MAX_STR_SIZE,
			"=  0x%016llx  0x%016llx  0x%016llx  0x%016llx\r\n",
			*(ptr + (i * 4U)), *(ptr + ((i * 4U) + 1U)),
			*(ptr + ((i * 4U) + 2U)), *(ptr + ((i * 4U) + 3U)));
		shell_puts(temp_str);
	}

	if ((length & 0x1fU) != 0U) {
		snprintf(temp_str, MAX_STR_SIZE,
			"=  0x%016llx  0x%016llx  0x%016llx 0x%016llx\r\n",
			*(ptr + (i * 4U)), *(ptr + ((i * 4U) + 1U)),
			*(ptr + ((i * 4U) + 2U)), *(ptr + ((i * 4U) + 3U)));
		shell_puts(temp_str);
	}

	return 0;
}

static int32_t shell_to_vm_console(int32_t argc, char **argv)
{
	char temp_str[TEMP_STR_SIZE];
	uint16_t vm_id = 0U;

	struct acrn_vm *vm;
	struct acrn_vuart *vu;

	if (argc == 2U) {
		vm_id = sanitize_vmid(strtol_deci(argv[1]));
	}

	/* Get the virtual device node */
	vm = get_vm_from_vmid(vm_id);
	if (vm->state == VM_POWERED_OFF) {
		shell_puts("VM is not valid \n");
		return -EINVAL;
	}
	vu = vm_console_vuart(vm);
	if (!vu->active) {
		shell_puts("vuart console is not active \n");
		return 0;
	}
	console_vmid = vm_id;
	/* Output that switching to SOS shell */
	snprintf(temp_str, TEMP_STR_SIZE, "\r\n----- Entering VM %d Shell -----\r\n", vm_id);

	shell_puts(temp_str);

	return 0;
}

#define MSI_DATA_TRGRMODE_LEVEL		0x1U	/* Trigger Mode: Level */
#define INVALID_INTERRUPT_PIN	0xffffffffU

static int32_t shell_show_ptdev_info(__unused int32_t argc, __unused char **argv)
{
	return 0;
}

static int32_t shell_loglevel(int32_t argc, char **argv)
{
	char str[MAX_STR_SIZE] = {0};

	switch (argc) {
	case 2:
		console_loglevel = (uint16_t)strtol_deci(argv[1]);
		break;
	case 1:
		snprintf(str, MAX_STR_SIZE, "console_loglevel: %u\r\n",
			console_loglevel);
		shell_puts(str);
		break;
	default:
		return -EINVAL;
	}

	return 0;
}

static int32_t shell_cpuid(int32_t argc, char **argv)
{
	char str[MAX_STR_SIZE] = {0};
	uint32_t leaf, subleaf = 0;
	uint32_t eax, ebx, ecx, edx;

	if (argc == 2) {
		leaf = (uint32_t)strtoul_hex(argv[1]);
	} else if (argc == 3) {
		leaf = (uint32_t)strtoul_hex(argv[1]);
		subleaf = (uint32_t)strtoul_hex(argv[2]);
	} else {
		shell_puts("Please enter correct cmd with "
			"cpuid <leaf> [subleaf]\r\n");
		return -EINVAL;
	}

	cpuid_subleaf(leaf, subleaf, &eax, &ebx, &ecx, &edx);
	snprintf(str, MAX_STR_SIZE,
		"cpuid leaf: 0x%x, subleaf: 0x%x, 0x%x:0x%x:0x%x:0x%x\r\n",
		leaf, subleaf, eax, ebx, ecx, edx);

	shell_puts(str);

	return 0;
}

static int32_t shell_trigger_reboot(int32_t argc, char **argv)
{
	(void)argc;
	(void)argv;

	pio_write8(0x6U, 0xcf9U);

	return 0;
}

static int32_t shell_rdmsr(int32_t argc, char **argv)
{
	int32_t ret = 0;
	uint32_t msr_index = 0;
	uint64_t val = 0;
	char str[MAX_STR_SIZE] = {0};

	switch (argc) {
	case 2:
		/* rdmsr <MSR_INDEX> */
		msr_index = (uint32_t)strtoul_hex(argv[1]);
		break;
	default:
		ret = -EINVAL;
	}

	if (ret == 0) {
		val = msr_read(msr_index);
		snprintf(str, MAX_STR_SIZE, "rdmsr(0x%x):0x%llx\n", msr_index, val);
		shell_puts(str);
	}

	return ret;
}

static int32_t shell_wrmsr(int32_t argc, char **argv)
{
	int32_t ret = 0;
	uint32_t msr_index = 0;
	uint64_t val = 0;

	switch (argc) {
	case 3:
		/* wrmsr <MSR_INDEX> <VALUE>*/
		msr_index = (uint32_t)strtoul_hex(argv[1]);
		val = strtoul_hex(argv[2]);
		break;
	default:
		ret = -EINVAL;
	}

	if (ret == 0) {
		msr_write(msr_index, val);
	}

	return ret;
}

static int shell_start_test(int argc, char **argv)
{
	struct acrn_vm *vm = get_vm_from_vmid(0U);

	if (vm->state == VM_POWERED_OFF) {
		struct acrn_vm_config *vm_config = get_vm_config(0U);

		char *buf = vm_config->os_config.bootargs;
		size_t sz = MAX_BOOTARGS_SIZE;
		int i;

		buf[0] = '\0';
		for (i = 1; i < argc; i++) {
			size_t len = strnlen_s(argv[i], sz);
			char ending = (i < argc - 1) ?  ' ' : '\0';

			memcpy_s(buf, sz, argv[i], len);
			buf[len] = ending;

			sz -= len + 1;
			buf += len + 1;
		}

		(void)prepare_vm(0U, vm_config);
	} else {
		shell_puts("Unit test VM already exists.\r\n");
	}

	return 0;
}

static int shell_stop_test(__unused int argc, __unused char **argv)
{
	struct acrn_vm *vm = get_vm_from_vmid(0U);

	if (vm->state != VM_POWERED_OFF) {
		(void)shutdown_vm(vm);
	} else {
		shell_puts("Unit test VM does not exist.\r\n");
	}

	return 0;
}

void set_idt_entry_offset(int vec, uint64_t addr, uint64_t idt_base, bool is_save)
{
	union idt_64_descriptor *idt_desc;

	idt_desc = (union idt_64_descriptor *)idt_base;

	if (is_save) {
		save_exception_entry = idt_desc[vec].fields.low32.bits.offset_15_0
			| (idt_desc[vec].fields.high32.bits.offset_31_16 << 16U)
			| (idt_desc[vec].fields.offset_63_32 << 32U);
	}

	idt_desc[vec].fields.offset_63_32 = addr >> 32U;
	idt_desc[vec].fields.high32.bits.offset_31_16 = addr >> 16U;
	idt_desc[vec].fields.low32.bits.offset_15_0 = addr & 0xffffUL;

	printf("entry=0x%lx save_entry=0x%lx address=0x%lx\n", addr, save_exception_entry, &idt_desc[vec]);
}

void reset_idt_entry_offset(int vec, uint64_t idt_base)
{
	if (save_exception_entry != 0UL) {
		set_idt_entry_offset(vec, save_exception_entry, idt_base, false);
	}
}

void shell_dispatch_exception(struct intr_excp_ctx *ctx)
{
	pr_fatal("find exception vector=%ld error_code=%lx rip=%lx cs=%lx\n",
			ctx->vector, ctx->error_code, ctx->rip, ctx->cs);
}

asm (".pushsection .text\n\t"
	"__handle_exception:\n\t"
	"push %r15; push %r14; push %r13; push %r12\n\t"
	"push %r11; push %r10; push %r9; push %r8\n\t"
	"push %rdi; push %rsi; push %rbp; push %rsp;\n\t"
	"push %rbx; push %rdx; push %rcx; push %rax\n\t"
	/* Put current stack pointer into 1st param register (rdi) */
	"movq %rsp, %rdi\n\t"
	"call	shell_dispatch_exception\n\t"
	"popq %rax; popq %rcx; popq %rdx; popq %rbx\n\t"
	"popq %rsp; popq %rbp; popq %rsi; popq %rdi\n\t"
	"popq %r8;  popq %r9;  popq %r10; popq %r11\n\t"
	"popq %r12; popq %r13; popq %r14; popq %r15\n\t"
	/* Skip vector and error code*/
	"add     $16, %rsp\n\t"
	"iretq\n\t"
	".popsection"
);

/* push pseudo error code */
#define EX(NAME, N) extern char NAME##_fault;	\
	asm (".pushsection .text\n\t"		\
		#NAME"_fault:\n\t"		\
		"pushq  $0x0\n\t"		\
		"pushq $"#N"\n\t"		\
		"jmp __handle_exception\n\t"	\
		".popsection")

EX(mc, 18);

#define TRIG_MC_MAGIC_ADDR_START  0xde000000UL
#define TRIG_MC_MAGIC_ADDR_END    0xde066000UL
static int shell_inject_mc(__unused int argc, __unused char **argv)
{
	uint64_t hpa;
	uint64_t *hva;
	uint64_t idt_base;
	struct host_idt_descriptor *idtd;

	idt_base = sidt();
	set_idt_entry_offset(IDT_MC, (uint64_t)&mc_fault, idt_base, true);

	stac();
	for (hpa = TRIG_MC_MAGIC_ADDR_START; hpa < TRIG_MC_MAGIC_ADDR_END; hpa += 0x1000UL) {
		hva = hpa2hva(hpa);
		pr_info("hva=0x%lx\n", hva);
		*(uint64_t *)hva = 0x1122334455667788UL;
	}
	clac();

	reset_idt_entry_offset(IDT_MC, idt_base);
	return 0;
}
