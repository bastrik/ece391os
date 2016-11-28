#include "types.h"
#include "x86_desc.h"
#include "lib.h"
#include "i8259.h"
#include "idt.h"
#include "rtc.h"
#include "asm_link.h"
#include "system_call.h"

#define KEYBOARD_ENTRY 0x21
#define RTC_ENTRY 0x28

void set_up_idt_handler();

/*
 DESCRIPTION: Initialize the IDT 
 Input: none
 Output none
 Side_effect: none
 */
void
init_idt() {
	
	// set up the interrupt handler
	set_up_idt_handler();
	
	// load idt
	// lidt(idt_desc_ptr);
}

/* 
 * DESCRIPTION: Set_up_idt_handler
 * Input: None
 * OUTPUT: None
 * RETURN: None
 * SIDE EFFECT: get exceptions, interrupts, and system call
 */
void set_up_idt_handler() {

	int i;
	
	SET_IDT_ENTRY(idt[0], (uint32_t)&division_by_zero);
	SET_IDT_ENTRY(idt[1], (uint32_t)&debugger);
	SET_IDT_ENTRY(idt[2], (uint32_t)&non_maskable_interrupt);
	SET_IDT_ENTRY(idt[3], (uint32_t)&breakpoint);
	SET_IDT_ENTRY(idt[4], (uint32_t)&overflow);
	SET_IDT_ENTRY(idt[5], (uint32_t)&bounds);
	SET_IDT_ENTRY(idt[6], (uint32_t)&invalid_opcode);
	SET_IDT_ENTRY(idt[7], (uint32_t)&coprocessor_not_available);
	SET_IDT_ENTRY(idt[8], (uint32_t)&double_fault);
	SET_IDT_ENTRY(idt[9], (uint32_t)&coprocessor_segment_overrun);
	SET_IDT_ENTRY(idt[10], (uint32_t)&invalid_task_state_segment);
	SET_IDT_ENTRY(idt[11], (uint32_t)&segment_not_present);
	SET_IDT_ENTRY(idt[12], (uint32_t)&stack_fault);
	SET_IDT_ENTRY(idt[13], (uint32_t)&general_protection_fault);
	SET_IDT_ENTRY(idt[14], (uint32_t)&page_fault);
	SET_IDT_ENTRY(idt[16], (uint32_t)&x87_FPU_error);
	SET_IDT_ENTRY(idt[17], (uint32_t)&alignment_check);
	SET_IDT_ENTRY(idt[18], (uint32_t)&machine_check);
	SET_IDT_ENTRY(idt[19], (uint32_t)&SIMD_floating_point_exception);
	
	/* set each idt entry */
	for (i=0; i<32; i++)
	{
		idt[i].seg_selector = KERNEL_CS;
		// idt[i].offset_31_16 = 0;
		idt[i].present = 1;
		idt[i].dpl = 0;
		idt[i].reserved0 = 0;
		idt[i].size = 1;
		idt[i].reserved1 = 1;
		idt[i].reserved2 = 1;
		idt[i].reserved3 = 1;
		idt[i].reserved4= 0x00;
		// idt[i].offset_15_00 = 0;
		
	}
	
	for (i=32; i<NUM_VEC; i++)
	{

			idt[i].seg_selector = KERNEL_CS;
			idt[i].present = 1;
			idt[i].dpl = 0;
			idt[i].reserved0 = 0;
			idt[i].size = 1;
			idt[i].reserved1 = 1;
			idt[i].reserved2 = 1;
			idt[i].reserved3 = 0;
			idt[i].reserved4= 0x00;
			
			if (i != 0x80)
			{	
				SET_IDT_ENTRY(idt[i], (uint32_t)&ignore_int);
			}
			else
			{
				idt[i].dpl = 3;
				SET_IDT_ENTRY(idt[i], &systemcall_handler_asm);
			}
	}
	/* set the IDT entry of the keyboard_handler */
	SET_IDT_ENTRY(idt[KEYBOARD_ENTRY], (uint32_t)&keyboard_handler_asm);
	/* set the IDT entry of the rtc_handler */
	SET_IDT_ENTRY(idt[RTC_ENTRY], (uint32_t)&rtc_handler_asm);
}

/* 
 * Below are the exceptions 
 */

void division_by_zero() {
	printf("division by zero exception\n");
	cli();
	while(1);
}

void debugger() {
	printf("debug exception\n");
	cli();
	while(1);
}

void non_maskable_interrupt() {
	printf("non maskable interrupt exception\n");
	cli();
	while(1);
}

void breakpoint() {
	printf("breakpoint exception\n");
	cli();
	while(1);
}

void overflow() {
	printf("overflow exception\n");
	cli();
	while(1);
}

void bounds() {
	printf("bound range exceeded exception\n");
	cli();
	while(1);
}

void invalid_opcode() {
	printf("invalid opcode exception\n");
	cli();
	while(1);
}

void coprocessor_not_available() {
	printf("Device not available exception\n");
	cli();
	while(1);
}

void double_fault() {
	printf("double fault exception\n");
	cli();
	while(1);
}

void coprocessor_segment_overrun() {
	printf("coprocessor_segment_overrun exception\n");
	cli();
	while(1);
}

void invalid_task_state_segment() {
	printf("invalid_task_state_segment exception\n");
	cli();
	while(1);
}

void segment_not_present() {
	printf("segment_not_present exception\n");
	cli();
	while(1);
}

void stack_fault() {
	printf("stack fault exception\n");
	cli();
	while(1);
}

void general_protection_fault() {
	printf("general_protection_fault exception\n");
	cli();
	while(1);
}

void page_fault() {
	printf("page fault exception\n");
	cli();
	while(1);
}

void x87_FPU_error() {
	printf("x87_FPU_error exception\n");
	cli();
	while(1);
}

void alignment_check() {
	printf("alignment_check exception\n");
	cli();
	while(1);
}

void machine_check() {
	printf("machine_check exception\n");
	cli();
	while(1);
}

void SIMD_floating_point_exception() {
	printf("SIMD_floating_point exception\n");
	cli();
	while(1);
}

void ignore_int() {
	printf("ignore int exception\n");
	cli();
	while(1);
}

