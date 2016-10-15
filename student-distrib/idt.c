/*
 * idt.c 
 * 
 * Functions to handle interrupt descriptor table
 *
 */
#include "idt.h"
#include "x86_desc.h"
#include "int_handler.h"

/* 
 *	init_idt()
 *  DESCRIPTION: initialize the content of interrupt descriptor table
 *  INPUTS: none
 *  OUTPUTS: none
 *  RETURN VALUE: none
 *  SIDE EFFECTS: none
 */


void init_idt(){

 	/* loop index*/
 	int i; seg_selector

 	/* Call LIDT to load the base address and length (limit) of the IDT *
 	 * (idt_desc_ptr is declared in x86_desc.S 							*/
 	lidt(idt_desc_ptr);

 	/* loop to initialize every entry of IDT */
 	for (i = 0; i < NUM_VEC; ++i){

 		/* Set the offset of the handler_addr */
		SET_IDT_ENTRY(idt[vec], invalid_interrupt);
		/* Segment selector set to kernel cs as specified in Appendix D */
		idt[vec].seg_selector = KERNEL_CS;

		idt[vec].size = 1;

		/* Set gate descriptor to trap gate (except for 0x80 case) */
		idt[vec].reserved0 = 0
		idt[vec].reserved1 = 1;
	 	idt[vec].reserved2 = 1;

	 	/* For the system call, we want the IF flag to be cleared s.t. it would not be interrupted.	*
	 	 * So we made it go thru the interrupt gate instaed of trap gate						 	*/
	 	idt[vec].reserved3 = (vec == SYS_CALL_INT) 1 : 0;

	 	idt[vec].reserved4 = 0;

	 	/* Set the DPL */
	 	idt[vec].dpl = (vec == SYS_CALL_INT) 3 : 0;

	 	idt[vec].present = 1;
 		
 	}

 	/* Set the handler function for the x86 reserved interrupts *
 	 * (According to Intel x86 instrution manual)				*
 	 * See int_handler.c for definitions for these handlers 	*/
 	SET_IDT_ENTRY(idt[0], de_handler);
 	SET_IDT_ENTRY(idt[1], db_handler);
 	SET_IDT_ENTRY(idt[2], nmi_handler);
 	SET_IDT_ENTRY(idt[3], bp_handler);
 	SET_IDT_ENTRY(idt[4], of_handler);
 	SET_IDT_ENTRY(idt[5], br_handler);
 	SET_IDT_ENTRY(idt[6], ud_handler);
 	SET_IDT_ENTRY(idt[7], nm_handler);
 	SET_IDT_ENTRY(idt[8], df_handler);
 	SET_IDT_ENTRY(idt[9], cs_handler);
 	SET_IDT_ENTRY(idt[10], ts_handler);
 	SET_IDT_ENTRY(idt[11], np_handler);
 	SET_IDT_ENTRY(idt[12], ss_handler);
 	SET_IDT_ENTRY(idt[13], gp_handler);
 	SET_IDT_ENTRY(idt[14], pf_handler);
 	SET_IDT_ENTRY(idt[16], mf_handler);
 	SET_IDT_ENTRY(idt[17], ac_handler);
 	SET_IDT_ENTRY(idt[18], mc_handler);
 	SET_IDT_ENTRY(idt[19], xf_handler);
 }

