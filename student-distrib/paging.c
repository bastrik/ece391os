#include "types.h"
#include "x86_desc.h"
#include "lib.h"
#include "paging.h"

	// uint32_t page_directory[1024] __attribute__((aligned (4096)));
	// uint32_t page_table[1024] __attribute__((aligned (4096)));
	
#define KERNEL_LOAD_ADDRESS 0x400000
#define VIDEO_MEM_ADDRE 0xB8

/* 
 * paging_init
 * DESCRIPTION: Initialize paging, set up page directory and page table
 * INPUT: None
 * OUTPUT: None
 * RETURN: None
 * SIDE EFFECT: None
 */
void paging_init(){


	int i,j;
	for (j = 0; j < MAX_PROGRAMS; j++)
	{
		for (i = 0; i < TABLE_SIZE; i++)
		{

			video_page_table[j].page_table_array[i].present =1;
			video_page_table[j].page_table_array[i].rw =0;
			video_page_table[j].page_table_array[i].user_supervisor =0;
			video_page_table[j].page_table_array[i].write_through =0;
			video_page_table[j].page_table_array[i].cache_disabled =0;
			video_page_table[j].page_table_array[i].accessed =0;
			video_page_table[j].page_table_array[i].dirty =0;
			video_page_table[j].page_table_array[i].page_table_attribute_index =0;
			video_page_table[j].page_table_array[i].global =0;
			video_page_table[j].page_table_array[i].avail =0;
			video_page_table[j].page_table_array[i].page_addr =i;

			process_page_directory[j].page_directory_array[i].present =0;
			process_page_directory[j].page_directory_array[i].rw =0;
			process_page_directory[j].page_directory_array[i].user_supervisor =0;
			process_page_directory[j].page_directory_array[i].write_through =0;
			process_page_directory[j].page_directory_array[i].cache_disabled =0;
			process_page_directory[j].page_directory_array[i].accessed =0;
			process_page_directory[j].page_directory_array[i].reserved =0;
			process_page_directory[j].page_directory_array[i].page_size =0;
			process_page_directory[j].page_directory_array[i].global =0;
			process_page_directory[j].page_directory_array[i].avail =0;
			process_page_directory[j].page_directory_array[i].page_addr =i*TEN_BIT_OFFSET;	
		}
	}

	for (j = 0; j < MAX_PROGRAMS; j++)
	{
		// set 4kb paging
		process_page_directory[j].page_directory_array[0].present =1;
		process_page_directory[j].page_directory_array[0].rw =1;
		process_page_directory[j].page_directory_array[0].user_supervisor =1;
		process_page_directory[j].page_directory_array[0].write_through =0;
		process_page_directory[j].page_directory_array[0].cache_disabled =0;
		process_page_directory[j].page_directory_array[0].accessed =0;
		process_page_directory[j].page_directory_array[0].reserved =0;
		process_page_directory[j].page_directory_array[0].page_size =0;
		process_page_directory[j].page_directory_array[0].global =0;
		process_page_directory[j].page_directory_array[0].avail =0;
		process_page_directory[j].page_directory_array[0].page_addr =((uint32_t) (&video_page_table[0]) >> BIT_SHIFT);	
		
		// set 4mb paging
		process_page_directory[j].page_directory_array[1].present =1;
		process_page_directory[j].page_directory_array[1].rw =1;
		process_page_directory[j].page_directory_array[1].user_supervisor =1;
		process_page_directory[j].page_directory_array[1].write_through =0;
		process_page_directory[j].page_directory_array[1].cache_disabled =0;
		process_page_directory[j].page_directory_array[1].accessed =0;
		process_page_directory[j].page_directory_array[1].reserved =0;
		process_page_directory[j].page_directory_array[1].page_size =1;
		process_page_directory[j].page_directory_array[1].global =1;
		process_page_directory[j].page_directory_array[1].avail =0;
		process_page_directory[j].page_directory_array[1].page_addr =(uint32_t)(KERNEL_LOAD_ADDRESS >> BIT_SHIFT);	
	}

	/* Initialize the process page tables */
	// for (j = 0; j < 7; j++)
	// {
	// 	for (i = 0; i < TABLE_SIZE; i++)
	// 	{
	// 		process_page_table[j].page_table_array[i].present =1;
	// 		process_page_table[j].page_table_array[i].rw =0;
	// 		process_page_table[j].page_table_array[i].user_supervisor =0;
	// 		process_page_table[j].page_table_array[i].write_through =0;
	// 		process_page_table[j].page_table_array[i].cache_disabled =0;
	// 		process_page_table[j].page_table_array[i].accessed =0;
	// 		process_page_table[j].page_table_array[i].dirty =0;
	// 		process_page_table[j].page_table_array[i].page_table_attribute_index =0;
	// 		process_page_table[j].page_table_array[i].global =0;
	// 		process_page_table[j].page_table_array[i].avail =0;
	// 		process_page_table[j].page_table_array[i].page_addr =i;
	// 	}
	// }

	/* set the first page table to be empty */
	for (j = 0; j < MAX_PROGRAMS; j++)
	{
		video_page_table[j].page_table_array[0].present = 0;
	}

	/* set the video memory */
	for (j = 0; j < MAX_PROGRAMS; j++)
	{
		video_page_table[j].page_table_array[VIDEO_MEM_ADDRE].present =1;
		video_page_table[j].page_table_array[VIDEO_MEM_ADDRE].rw =1;
		video_page_table[j].page_table_array[VIDEO_MEM_ADDRE].user_supervisor =1;
		video_page_table[j].page_table_array[VIDEO_MEM_ADDRE].write_through =0;
		video_page_table[j].page_table_array[VIDEO_MEM_ADDRE].cache_disabled =0;
		video_page_table[j].page_table_array[VIDEO_MEM_ADDRE].accessed =0;
		video_page_table[j].page_table_array[VIDEO_MEM_ADDRE].dirty =0;
		video_page_table[j].page_table_array[VIDEO_MEM_ADDRE].page_table_attribute_index =0;
		video_page_table[j].page_table_array[VIDEO_MEM_ADDRE].global =0;
		video_page_table[j].page_table_array[VIDEO_MEM_ADDRE].avail =0;
		video_page_table[j].page_table_array[VIDEO_MEM_ADDRE].page_addr =VIDEO_MEM_ADDRE;		
	}

	// video_page_table[0].page_table_array[VIDEO_MEM_ADDRE].rw = 0;

	// /* allocate the fisrt page table into the directory table */
	// 	page_directory[0].present = 1;
	// 	page_directory[0].rw = 1;
	// 	page_directory[0].user_supervisor = 1;
	// 	page_directory[0].page_addr = ((uint32_t) (&video_page_table[0]) >> BIT_SHIFT);
	
	// 	page_directory[1].present = 1;
	// 	page_directory[1].rw = 1;
	// 	page_directory[1].user_supervisor = 0;
	// 	page_directory[1].write_through = 0;
	// 	page_directory[1].cache_disabled = 0;
	// 	page_directory[1].accessed = 0;
	// 	page_directory[1].reserved = 0;
	// 	page_directory[1].page_size = 1;
	// 	page_directory[1].global = 1;
	// 	page_directory[1].avail = 0;
	// 	page_directory[1].page_addr = TEN_BIT_OFFSET;
	
	/* enable paging */
	uint32_t dir_ptr = (uint32_t)&process_page_directory;
	enable_4MBpaging(dir_ptr);
}

/* 
 * Enable 4Mb paging
 * DESCRIPTION: Enable 4Mb paging function, load CR3 is included
 * INPUT: Address (page_directory address)
 * OUTPUT: none
 * RETURN: none
 * SIDE EFFECT: Load cr3 and enable PSE
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

void map_to_4mb(uint32_t virt_addr, uint32_t phys_addr, uint32_t pid)
{
	uint32_t pd_idx;
	pd_idx = (uint32_t) (virt_addr /MB4); // should be 32 for program at 128Mb

	process_page_directory[pid].page_directory_array[pd_idx].present =1;
	process_page_directory[pid].page_directory_array[pd_idx].rw =1;
	process_page_directory[pid].page_directory_array[pd_idx].user_supervisor =1;
	process_page_directory[pid].page_directory_array[pd_idx].write_through =0;
	process_page_directory[pid].page_directory_array[pd_idx].cache_disabled =0;
	process_page_directory[pid].page_directory_array[pd_idx].accessed =0;
	process_page_directory[pid].page_directory_array[pd_idx].reserved =0;
	process_page_directory[pid].page_directory_array[pd_idx].page_size =1;
	process_page_directory[pid].page_directory_array[pd_idx].global =1;
	process_page_directory[pid].page_directory_array[pd_idx].avail =0;
	process_page_directory[pid].page_directory_array[pd_idx].page_addr =(uint32_t)(phys_addr >> BIT_SHIFT);		
}


void set_table(int32_t pid)
{
	uint32_t page_dir_ptr;
	page_dir_ptr = (uint32_t)&process_page_directory[pid];

	// point cr3 to page directory
	asm volatile("					\n\
			movl	%0, %%cr3		\n\
			"
			:
			: "r" (page_dir_ptr)
			); 
}

uint32_t get_current_pid()
{
	uint32_t i, cr3;
	asm volatile ("movl %%CR3, %0": "=b"(cr3));
		for (i = 0; i < MAX_PROGRAMS; i++)
		{
			if (cr3 == (uint32_t)&process_page_directory[i])
				printf("%d\n", i);
				return i;
		}

	return -1;
}

// 	for (int i = 0; i < 7; i++)
// 	{
// 		for (int j = 0; j < TABLE_SIZE; j++)		
// 		{
// 			/* initialize the page_directory */			
// 			page_directory[i][j].present = 0;
// 			page_directory[i][j].rw = 0;
// 			page_directory[i][j].user_supervisor = 0;
// 			page_directory[i][j].write_through = 0;
// 			page_directory[i][j].cache_disabled = 0;
// 			page_directory[i][j].accessed = 0;
// 			page_directory[i][j].reserved = 0;
// 			page_directory[i][j].page_size = 0;
// 			page_directory[i][j].global = 1;
// 			page_directory[i][j].avail = 0;
// 			page_directory[i][j].page_addr = j*TEN_BIT_OFFSET; // 4Mb is 0x400000 = 2^22

// 			/* initialize the page_table */
// 			page_table[i][j].present = 0;
// 			page_table[i][j].rw = 0;
// 			page_table[i][j].user_supervisor = 0;
// 			page_table[i][j].write_through = 0;
// 			page_table[i][j].cache_disabled = 0;
// 			page_table[i][j].accessed = 0;
// 			page_table[i][j].reserved = 0;
// 			page_table[i][j].page_size = 0;
// 			page_table[i][j].global = 1;
// 			page_table[i][j].avail = 0;
// 			page_table[i][j].page_addr = j; // 4Mb is 0x400000 = 2^22

// 			page_directory[i][0].present = 1;
// 			page_directory[i][0].rw = 1;
// 			page_directory[i][0].user_supervisor = 1;
// 			page_directory[i][0].page_addr = ((uint32_t) (&page_table) >> BIT_SHIFT);
		
// 		}
// 	}

// 	/* allocate the fisrt page table into the directory table */
// 		page_directory[0].present = 1;
// 		page_directory[0].rw = 1;
// 		page_directory[0].user_supervisor = 1;
// 		page_directory[0].page_addr = ((uint32_t) (&page_table) >> BIT_SHIFT);
		
// 		/* set up the kernel page */
// 		page_directory[1].present = 1;
// 		page_directory[1].rw = 1;
// 		page_directory[1].user_supervisor = 1;
// 		page_directory[1].page_size = 1;
// 		page_directory[1].global = 1;
// 		page_directory[1].page_addr = (uint32_t)(KERNEL_LOAD_ADDRESS >> BIT_SHIFT); 
	


// /*		uint32_t present : 1;
// 		uint32_t rw : 1;
// 		uint32_t user_supervisor : 1;
// 		uint32_t write_through	: 1;
// 		uint32_t cache_disabled : 1;
// 		uint32_t accessed : 1;
// 		uint32_t reserved : 1; // set to 0
// 		uint32_t page_size : 1;
// 		uint32_t global : 1; // ignored
// 		uint32_t avail : 3;
// 		uint32_t page_addr : 20;*/



// 	// for (i=0; i<TABLE_SIZE; i++)
// 	// {
// 	// 	/* initialize the page_directory */
// 	// 	page_directory[i].present = 0;
// 	// 	page_directory[i].rw =1;
// 	// 	page_directory[i].page_addr = i*TEN_BIT_OFFSET; // 4Mb is 0x400000 = 2^22

// 	// 	/* initialize the page_table */
// 	// 	page_table[i].present = 0;
// 	// 	page_table[i].rw = 1;
// 	// 	page_table[i].user_supervisor = 1;
// 	// 	page_table[i].page_addr = i;
// 	// }
	
// 	/* set the first page to be empty */
// 	page_table[0].present = 0;
// 	page_table[0].rw = 0;
// 	page_table[0].user_supervisor = 0;
	
// 	/* set for video memory */
// 	page_table[VIDEO_MEM_ADDRE].present = 1;
// 	//page_table[0xB8].rw = 1;
// 	//page_table[0].user_supervisor = 0;

