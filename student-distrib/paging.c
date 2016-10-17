#include "types.h"
#include "x86_desc.h"
#include "lib.h"
#include "paging.h"

/* 
 * paging_init
 * DESCRIPTION: Initialize paging.
 * INPUT: NONE
 * OUTPUT: NONE
 * RETURN: NONE
 * SIDE EFFECT: Enable interrupt. Page directory and page table set.
 */
void paging_init(){

	int i;
	
	/* Initialize the page_directory and the  page_table */
	for (i=0; i<TABLE_SIZE; i++)
	{
		/* initialize the page_table */
		page_table[i].present = 0;
		page_table[i].rw = 1;
		page_table[i].user_supervisor = 1;
		page_table[i].page_addr = i;

		/* initialize the page_directory */
		page_directory[i].present = 0;
		page_directory[i].rw =1;
		page_directory[i].page_addr = i*TABLE_SIZE; 


	}
	

	/* set up the kernel page */
	page_directory[1].present = 1;
	page_directory[1].rw = 1;
	page_directory[1].user_supervisor = 1;
	page_directory[1].page_size = 1;
	page_directory[1].global = 1;
	page_directory[1].page_addr = (uint32_t)(KERNEL_ADDRESS >> BIT_SHIFT); 

	/* set the first page to be empty */
	page_table[0].present = 0;
	page_table[0].rw = 0;
	page_table[0].user_supervisor = 0;
	
	/* set for video memory */
	page_table[VIDEO_MEM_ADDRE].present = 1;

	/* allocate the fisrt page table into the directory table */
	page_directory[0].present = 1;
	page_directory[0].rw = 1;
	page_directory[0].user_supervisor = 1;
	page_directory[0].page_addr = ((uint32_t) (&page_table) >> BIT_SHIFT);


	/* enable paging */
	uint32_t dir_ptr = (uint32_t)&page_directory;
	enable_4MBpaging(dir_ptr);
	
}

/* 
 * Enable 4Mb paging
 * DESCRIPTION: Enable 4Mb paging function.
 * INPUT: Address (page_directory address)
 * OUTPUT: NONE
 * RETURN: NONE
 * SIDE EFFECT: NONE
 */
void enable_4MBpaging(uint32_t address)
{
	asm volatile("movl %%eax,%%cr3 \n\
				movl %%cr4,%%eax  \n\
				orl  $0x00000010,%%eax \n\
				movl %%eax,%%cr4       \n   \
				movl %%cr0,%%eax      \n   \
				orl  $0x80000000,%%eax  \n  \
				movl %%eax ,%%cr0 "
	:  
	: "a"(address)
	: "memory", "cc" );
}


