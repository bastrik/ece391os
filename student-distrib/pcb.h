/* pcb.h 					  *
 * includes definition of pcb struct */

#ifndef PCB_H
#define PCB_H

#include "filesys.h"

#define KERNEL_PCB_START_ADDR 	0x400000				// TODO:Very not sure about this!!!!
#define USER_PCB_START_ADDR 	0x402000
#define PCB_BITMASK 			0x402000


// 8kB pcb block (2096 * 4B)
typedef struct
{
	file_descriptor_t fd[8]; 		// 16B * 8 = 128B = 32 * 4B
	uint32_t reserved[2064];		// TODO: figure out what else to put in the block
} pcb_t;

/* Initialize pcb */
void pcb_init();

/* Get a pointer to the current pcb */
pcb_t* get_current_pcb();

#endif /* PCB_H */
