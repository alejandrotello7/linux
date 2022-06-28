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
#define ARG1 %rsi
#define ARG2 %rdx

.text

SYM_INNER_LABEL(fct_example_functions_start, SYM_L_GLOBAL)

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
/* 
 * New fastcall function 
 */ 
SYM_CODE_START(new_function)
UNWIND_HINT_EMPTY 
FASTCALL_SETUP_STACK 
pushq %rbp 
pushq %rdi 
movq %rsp, %rbp
movq ARG1, %rdi 
movq ARG2, %rsi
.byte 0x48;
.byte 0x83;
.byte 0xec;
.byte 0x10;          	
.byte 0x89;
.byte 0x7d;
.byte 0xfc;  
.byte 0x8b;
.byte 0x45;
.byte 0xfc;             	
.byte 0x0f;
.byte 0xaf; 
.byte 0xc0;             
.byte 0x89;
.byte 0x45;
.byte 0xfc;              	
.byte 0x8b;
.byte 0x45;
.byte 0xfc;             	
.byte 0x0f;
.byte 0xaf;
.byte 0xc0;  
.byte 0x89;
.byte 0x45;
.byte 0xfc;
.byte 0x8b;
.byte 0x45;
.byte 0xfc;             	
.byte 0x89;
.byte 0xc6;                	
.byte 0x48;
.byte 0x8d;
.byte 0x3d;
.byte 0x00;
.byte 0x00;
.byte 0x00;
.byte 0x00;
.byte 0xb8;
.byte 0x00;
.byte 0x00;
.byte 0x00;
.byte 0x00;       	
.byte 0xe8;
.byte 0x00;
.byte 0x00;
.byte 0x00;
.byte 0x00;	
.byte 0x8b;
.byte 0x45;
.byte 0xfc;             	
popq %rdi 
popq %rbp 
movq %rdi, %rsp 
sysretq
SYM_CODE_END(new_function)

SYM_CODE_START(fct_example_template)
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
SYM_CODE_END(fct_example_template)

SYM_INNER_LABEL(fct_example_functions_end, SYM_L_GLOBAL)
