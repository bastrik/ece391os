# asm_link.S - Set up x86 segment descriptors, descriptor tables 
# vim:ts=4 noexpandtab

#define ASM     1
#include "x86_desc.h"

kernel_ds:
	.long 0x0018

.text

.globl keyboard_handler_asm, rtc_handler_asm

keyboard_handler_asm:
	pushl	%eax
	pushl	%ebx
	pushl	%ecx
	pushl	%edx

	pushl	%esp
	pushl	%ebp

	pushl	%esi
	pushl	%edi

    pushw 	%ds
    pushw 	%es
    pushw 	%fs
    pushw 	%gs

	call keyboard_handler
	
	popw 	%gs
    popw 	%fs
    popw 	%es
    popw 	%ds
    
	popl	%edi
	popl	%esi

	popl	%ebp
	popl	%esp

	popl	%edx
	popl	%ecx
	popl	%ebx
	popl	%eax

	iret

rtc_handler_asm:
	pushal
	call rtc_handler
	popal
	iret
fail:
	movl $-1, %eax
	iret
