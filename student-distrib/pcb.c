/* pcb.c					  *
 * includes operations on the pcb*/

#include "pcb.h"
#include "filesys.h"
#include "keyboard.h"


/*
 *   pcb_init()
 *   DESCRIPTION: Initialize the two pcbs for kernel and user level in the kernel page 
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: Write two PCB struct data into kernel page 
 */
void pcb_init()
{
	pcb_t* kernel_pcb = (pcb_t*) KERNEL_PCB_START_ADDR;
	pcb_t* user_lvl_pcb = (pcb_t*) USER_PCB_START_ADDR;

	/* Initialize stdin for both pcb*/
	kernel_pcb->fd[0].fotp = (uint32_t) (&terminal_fotp); // TODO: Not sure about this? 
	kernel_pcb->fd[0].inode = 0;
	kernel_pcb->fd[0].file_pos = 0;
	kernel_pcb->fd[0].flags = 1;			   // Mark this fd as present

	user_lvl_pcb->fd[0].fotp = (uint32_t) (&terminal_fotp); // TODO 
	user_lvl_pcb->fd[0].inode = 0;
	user_lvl_pcb->fd[0].file_pos = 0;
	user_lvl_pcb->fd[0].flags = 1;			   // Mark this fd as present

	/* Initialize stdout for both pcb*/
	kernel_pcb->fd[1].fotp = (uint32_t) (&terminal_fotp); // TODO: Not sure about this? 
	kernel_pcb->fd[1].inode = 0;
	kernel_pcb->fd[1].file_pos = 0;
	kernel_pcb->fd[1].flags = 1;			   // Mark this fd as present

	user_lvl_pcb->fd[1].fotp = (uint32_t) (&terminal_fotp); // TODO 
	user_lvl_pcb->fd[1].inode = 0;
	user_lvl_pcb->fd[1].file_pos = 0;
	user_lvl_pcb->fd[1].flags = 1;			   // Mark this fd as present

}
