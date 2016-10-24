/*
 * idt.c 
 * 
 * Functions to handle interrupt descriptor table
 *
 */
#include "idt.h"
#include "x86_desc.h"
#include "lib.h"
#include "rtc.h"
#include "keyboard.h"


void de_handler(){
	printf("Exception: Divide Zero Error");
	while(1);
}

void db_handler(){
	printf("Exception: Debug");
	while(1);
}

void nmi_handler(){
	printf("Exception: Nonmaskable Interrupt");
	while(1);
}

void bp_handler(){
	printf("Exception: Breakpoint");
	while(1);
}

void of_handler(){
	printf("Exception: Overflow");
	while(1);
}

void br_handler(){
	printf("Exception: Bound range exceeded");
	while(1);
}

void ud_handler(){
	printf("Exception: Invalid Opcode");
	while(1);
}

void nm_handler(){
	printf("Exception: Device Not Available");
	while(1);
}

void df_handler(){
	printf("Exception: Double Fault");
	while(1);
}

void cs_handler(){
	printf("Exception: Coprocessor Segment Overrun");
	while(1);
}

void ts_handler(){
	printf("Exception: Invalid TSS Exception");
	while(1);
}

void np_handler(){
	printf("Exception: Segment Not Present");
	while(1);
}

void ss_handler(){
	printf("Exception: Stack Fault");
	while(1);
}

void gp_handler(){
	printf("Exception: General Protection");
	while(1);
}

void pf_handler(){
	printf("Exception: Page-Fault");
	while(1);
}

void mf_handler(){
	printf("Exception: x87 FPU Floating-Point Error");
	while(1);
}

void ac_handler(){
	printf("Exception: Alignment Check Error");
	while(1);
}

void mc_handler(){
	printf("Exception: Machine-Check Error");
	while(1);
}

void xf_handler(){
	printf("Exception: SIMD Floating-Point Exception Error");
	while(1);
}

void invalid_interrupt(){
 	cli();
 	printf("Error: Undefined Interrupt");
 	sti();
}

/* 
 *	init_idt()
 *  DESCRIPTION: initialize the content of interrupt descriptor table
 *  INPUTS: none
 *  OUTPUTS: none
 *  RETURN VALUE: none
 *  SIDE EFFECTS: Initialize the IDT entry
 */


void init_idt(){

 	/* loop index*/
 	int i;		

 	/* set each idt entry */
	for (i=0; i<NUM_VEC; i++)
	{
			idt[i].seg_selector = KERNEL_CS;
			idt[i].present = 1;
			idt[i].dpl = 0;
			idt[i].reserved0 = 0;
			idt[i].size = 1;
			idt[i].reserved1 = 1;
			idt[i].reserved2 = 1;
			idt[i].reserved3 = (i == SYS_CALL_INT | i <= 0x20) ? 1 : 0;
			idt[i].reserved4= 0x00;
			
			SET_IDT_ENTRY(idt[i], (uint32_t)&invalid_interrupt);
	}

	/* Set the handler function for the x86 reserved interrupts *
 	 * (According to Intel x86 instrution manual)				*
 	 * See int_handler.c for definitions for these handlers 	*/
 	SET_IDT_ENTRY(idt[0], (uint32_t)&de_handler);
 	SET_IDT_ENTRY(idt[1], (uint32_t)&db_handler);
 	SET_IDT_ENTRY(idt[2], (uint32_t)&nmi_handler);
 	SET_IDT_ENTRY(idt[3], (uint32_t)&bp_handler);
 	SET_IDT_ENTRY(idt[4], (uint32_t)&of_handler);
 	SET_IDT_ENTRY(idt[5], (uint32_t)&br_handler);
 	SET_IDT_ENTRY(idt[6], (uint32_t)&ud_handler);
 	SET_IDT_ENTRY(idt[7], (uint32_t)&nm_handler);
 	SET_IDT_ENTRY(idt[8], (uint32_t)&df_handler);
 	SET_IDT_ENTRY(idt[9], (uint32_t)&cs_handler);
 	SET_IDT_ENTRY(idt[10], (uint32_t)&ts_handler);
 	SET_IDT_ENTRY(idt[11], (uint32_t)&np_handler);
 	SET_IDT_ENTRY(idt[12], (uint32_t)&ss_handler);
 	SET_IDT_ENTRY(idt[13], (uint32_t)&gp_handler);
 	SET_IDT_ENTRY(idt[14], (uint32_t)&pf_handler);
 	SET_IDT_ENTRY(idt[16], (uint32_t)&mf_handler);
 	SET_IDT_ENTRY(idt[17], (uint32_t)&ac_handler);
 	SET_IDT_ENTRY(idt[18], (uint32_t)&mc_handler);
 	SET_IDT_ENTRY(idt[19], (uint32_t)&xf_handler);		

 	/* Set interrupt for RTC */
 	SET_IDT_ENTRY(idt[RTC_IDT_VEC], (uint32_t)&rtc_handler);

 	/* Set interrupt for Keyboard */
 	SET_IDT_ENTRY(idt[0x21], (uint32_t)&keyboard_handler);


 }

