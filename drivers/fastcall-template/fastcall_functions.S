// SPDX-License-Identifier: GPL-2.0
/*
 * fastcall_functions.S - fastcall function examples
 *
 * This file contains the actual fastcall functions in assembly.
 * They must be between fct_functions_start and fct_functions_end.
 * Additionally, they have to be position independent.
 */

#include <linux/linkage.h>
#include <asm/unwind_hints.h>
#include <asm/smap.h>
#include <asm/fastcall_module.h>

.text

SYM_INNER_LABEL(fct_functions_start, SYM_L_GLOBAL)

/*
 * fct_template - demonstrate the possibilites of fastcall functions
 *
 * Registers in:
 * rax  address of the fastcall table entry of this function
 * rsi  arg1
 * rdx  arg2
 * r10  arg3
 * r8   arg4
 * r9   arg5
 * r11  flags register
 * rcx  return address
 *
 * Return out:
 * rax  return address
 * r11  flags register
 * rcx  return address
 *
 * It should be safe to clobber all caller-saved
 * registers according to the calling convention.
 */
SYM_CODE_START(fct_template)
  // This prevents complains during compilation.
	UNWIND_HINT_EMPTY

  // Setup a per-CPU stack and store old %rsp to %rdx.
  FASTCALL_SETUP_STACK reg=%rdx scratch_reg=%rdi
  // Push the old stack pointer to the new stack.
  pushq %rdx

  // Push the first argument.
  pushq %rsi

  // Increment it.
  incq (%rsp)

  // ASM_STAC ... ASM_CLAC are required when accessing shared memory.
  ASM_STAC
  // Load the first static argument (the shared memory).
  movq 8(%rax), %rdi
  // Load the value at this location.
  movq (%rdi), %rdi
  // Add it to our stack value.
  addq %rdi, (%rsp)
  ASM_CLAC

  // Load the second static argument (the private memory).
  movq 16(%rax), %rdi
  // Use the private memory region and store the result in %rax.
  popq %rdx
  movq %rdx, (%rdi)
  incq (%rdi)
  movq (%rdi), %rax

  // Restore user %rsp.
  movq (%rsp), %rsp

  /*
   * We always have to return to user mode.
   * The return value is in %rax.
   */
  sysretq
SYM_CODE_END(fct_template)

/*
* FCE
*
*/

 SYM_CODE_START(fct_summer)
	UNWIND_HINT_EMPTY

	push %rbp 
	mov %rsp,%rbp
	sub $0x20, %rsp
	mov %edi, -0x14(%rbp)
	mov %esi, -0x18(%rbp)
	mov -0x18(%rbp), %edx
	mov -0x14(%rbp), %eax
	mov %eax, %esi
	mov $0x1bb, %edi
	mov $0x0, %eax
	jmpq *0x2fe2(%rip)
	pushq $0x0
	pushq 0x2fe2(%rip)
	jmpq *0x2fe4(%rip)
	nopl 0x0(%rax)
	mov %eax, -0x4(%rbp)
	mov -0x4(%rbp),%eax
	leaveq
	sysretq
	SYM_CODE_END(fce_summer)

SYM_INNER_LABEL(fct_functions_end, SYM_L_GLOBAL)
