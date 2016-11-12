/* pcb.h 					  *
 * includes definition of pcb struct */

#ifndef PCB_H
#define PCB_H

#include "filesys.h"


// 8kB pcb block (2096 * 4B)
typedef struct
{
	file_descriptor_t fd[8]; 		// 16B * 8 = 128B = 32 * 4B
	uint32_t reserved[2064];		// TODO: figure out what else to put in the block
} pcb_t


#endif /* PCB_H */
