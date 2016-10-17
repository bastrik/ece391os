#include "types.h"
#include "x86_desc.h"
#include "lib.h"
#include "i8259.h"
#include "rtc.h"


int rate = 0x0F;

/* 
 * RTC_init
 * DESCRIPTION: Initialize the real time clock, frequency should be 2Hz 
 * Input: None
 * Ouput: None
 * RETURN: None
 * Side_Effect: disable all the interrupts and then enable IRQ
 */
void
rtc_init() {
	
	// reference: wiki.osdev.org/RTC

	/* To turn on the periodic interrupt */
	outb(RTC_STATUS_REGISTER_B, RTC_INDEX_PORT); // select register B, and disable NMI
	char prev=inb(RTC_RW_PORT); // read the current value form register B
	outb(RTC_STATUS_REGISTER_B, RTC_INDEX_PORT); // set the index again
	outb(prev | 0x40, RTC_RW_PORT); //write the previous value ORed with 0x40. This turns on bit 6 of register B

	/* To change the rate */
	rate &= 0x0F; // rate must be above 2 and not over 15
	outb(RTC_STATUS_REGISTER_A, RTC_INDEX_PORT); // set index to register A, disable NMI
	char prevA=inb(RTC_RW_PORT); // get initial value of register A
	outb(RTC_STATUS_REGISTER_A, RTC_INDEX_PORT); // reset index to A
	outb((prevA & 0xF0) | rate, RTC_RW_PORT); //write only our rate to A. Rate is the bottom 4 bits.

	/* enable able_irq */
	//winterrupts(IRQ2);
	enable_irq(IRQ8);
	
}

/* 
 * Keyboard_handler
 * Description: Real-time clock handler goes into IDT
 * INPUT: None
 * OUTPUT: None
 * RETURN: None
 * SIDE EFFECT: None
 */

void rtc_handler() {
	cli();
	/* interrupts and register C */
	outb(RTC_STATUS_REGISTER_C, RTC_INDEX_PORT);
	inb(RTC_RW_PORT);
	
	 //test_interrupts();
	
	send_eoi(IRQ8);
	sti();
}

