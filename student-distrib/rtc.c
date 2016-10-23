#include "types.h"
#include "x86_desc.h"
#include "lib.h"
#include "i8259.h"
#include "rtc.h"

#define IRQ8 0x08
#define IRQ2 0x02


/* #define RTC_INDEX_PORT 0x70
#define RTC_RW_PORT 0x71

#define	RTC_STATUS_REGISTER_A 0x8A
#define RTC_STATUS_REGISTER_B 0x8B
#define RTC_STATUS_REGISTER_C 0x8C */

uint32_t rate = HZ2_RATE;
volatile uint8_t rtc_interrupt_flag;


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
	// disable_irq(IRQ8);
	/* To turn on the periodic interrupt */
	outb(RTC_STATUS_REGISTER_B, RTC_INDEX_PORT); // select register B, and disable NMI
	char prev=inb(RTC_RW_PORT); // read the current value form register B
	outb(RTC_STATUS_REGISTER_B, RTC_INDEX_PORT); // set the index again
	outb(prev | ORED_MASK, RTC_RW_PORT); //write the previous value ORed with 0x40. This turns on bit 6 of register B

	/* To change the rate */
	rate &= HZ2_RATE; // rate must be above 2 and not over 15
	outb(RTC_STATUS_REGISTER_A, RTC_INDEX_PORT); // set index to register A, disable NMI
	char prevA=inb(RTC_RW_PORT); // get initial value of register A
	outb(RTC_STATUS_REGISTER_A, RTC_INDEX_PORT); // reset index to A
	outb((prevA & UPPER_FOURBIT_MASK) | rate, RTC_RW_PORT); //write only our rate to A. Note, rate is the bottom 4 bits.

	/* enable interrupts */
	enable_irq(IRQ2);
	enable_irq(IRQ8);

}

/*
 * RTC_open
 * DESCRIPTION: rtc_open function
 * Input: None
 * Ouput: None
 * RETURN: None
 * Side_Effect: none
 */
int32_t
rtc_open(const uint8_t* filename){

	rate = HZ2_RATE;
	rtc_interrupt_flag = 0;

	outb(RTC_STATUS_REGISTER_A, RTC_INDEX_PORT); // set index to register A, disable NMI
	char prevA=inb(RTC_RW_PORT); // get initial value of register A
	outb(RTC_STATUS_REGISTER_A, RTC_INDEX_PORT); // reset index to A
	outb((prevA & UPPER_FOURBIT_MASK) | rate, RTC_RW_PORT); //write only our rate to A. Note, rate is the bottom 4 bits.

	return 0;
	
}

/*
 * RTC_close
 * DESCRIPTION: rtc_close function
 * Input: None
 * Ouput: None
 * RETURN: None
 * Side_Effect: none
 */
int32_t
rtc_close(int32_t fd)
{
	return 0;
}

/*
 * RTC_read
 * DESCRIPTION: rtc_read function, which would set the rtc_interrupt_flag and wait till the rtc_handler clear the flag
 * Input: None
 * Ouput: None
 * RETURN: None
 * Side_Effect: none
 */
int32_t
rtc_read(int32_t fd, uint8_t* buf, int32_t nbytes)
{
	sti();
	// set the rtc flag to 1
	rtc_interrupt_flag = 1;

	/* wait untill the rtc handler clear the flag */
	while ((rtc_interrupt_flag==1));

	return 0;
}

/*
 * RTC_write
 * DESCRIPTION: rtc_write function, which would set the rtc_interrupt_flag and wait till the rtc_handler clear the flag
 * Input: fd - not used in this function
 * 		  buf - buffer pointer
 * 		  nbyte - number of inputbyte
 * Ouput: None
 * RETURN: None
 * Side_Effect: none
 */
int32_t rtc_write(int32_t fd, const uint8_t* buf, int32_t nbyte)
{

	/* check NULL for buf */
	if (buf == NULL)
		return -1;

	/* input must be 4 bytes */
	if (nbyte != 4)
		return -1;

	/* get the target frequency from the buffer */
	uint32_t frequency;
	frequency = *buf;

	/* max frequency is 1024 hz */
	if (frequency > MAX_FREQ) {
		return -1;
	}

	/* set the rate at different frequency */
	switch (frequency) {
		// 2Hz
		case 2:
			rate = HZ2_RATE;
			break;
		// 4Hz
		case 4:
			rate = HZ4_RATE;
			break;
		// 8Hz
		case 8:
			rate = HZ8_RATE;
			break;
		// 16Hz
		case 16:
			rate = HZ16_RATE;
			break;
		// 32 Hz
		case 32:
			rate = HZ32_RATE;
			break;
		// 64 Hz
		case 64:
			rate = HZ64_RATE;
			break;
		// 128 Hz
		case 128:
			rate = HZ128_RATE;
			break;
		// 256 Hz
		case 256:
			rate = HZ256_RATE;
			break;
		// 512 Hz
		case 512:
			rate = HZ512_RATE;
			break;
		// 1024 Hz
		case 1024:
			rate = HZ1024_RATE;
			break;
		default:
			return -1;
	}

	cli();
	outb(RTC_STATUS_REGISTER_A, RTC_INDEX_PORT); // set index to register A, disable NMI
	char prevA=inb(RTC_RW_PORT); // get initial value of register A
	outb(RTC_STATUS_REGISTER_A, RTC_INDEX_PORT); // reset index to A
	outb((prevA & UPPER_FOURBIT_MASK) | rate, RTC_RW_PORT); //write only our rate to A. Note, rate is the bottom 4 bits.
	sti();

	/* return number of bytes */
	return nbyte;

}

	// TEST RTC driver
	// int j;
	// for (i = 2; i<=1024; i*=2) {
		// rtc_write(1, (uint32_t *) &i, 4);
		// printf("current frequency is %d\n",i);

		// for (j=0; j<10; j++){
			// printf("a ");
			// rtc_read();
		// }
	// }


/*
 * RTC_handler
 * Description: Real-time clock handler goes into IDT
 * INPUT: None
 * OUTPUT: None
 * RETURN: None
 * SIDE EFFECT: None
 */

void rtc_handler() 
{
	// disable_irq(IRQ8);
	cli();
	/* interrupts and register C */
	outb(RTC_STATUS_REGISTER_C, RTC_INDEX_PORT);
	inb(RTC_RW_PORT);

	 //test_interrupts();

	 /* clear the flag */
	rtc_interrupt_flag = 0;

	// send_eoi(2);
	send_eoi(IRQ8);
	sti();
	// enable_irq(IRQ8);
}
