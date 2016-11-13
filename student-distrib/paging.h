#ifndef _PAGING_H
#define _PAGING_H

#include "types.h"

extern void paging_init();
void enable_4MBpaging(uint32_t address);

#define KERNEL_ADDRESS 0x400000
#define VIDEO_MEM_ADDRE 0xB8
#define USER_PROGRAM_ADDRESS 		0x800000
#define USER_PROGRAM_ENTRY			0x20
#define TABLE_SIZE 1024
#define BIT_SHIFT 12

//FROM OSDEV
//S, or 'Page Size' stores the page size for that specific entry. If the bit is set, then pages are 4 MiB in size. Otherwise, they are 4 KiB. Please note that 4-MiB pages require PSE to be enabled.
//A, or 'Accessed' is used to discover whether a page has been read or written to. If it has, then the bit is set, otherwise, it is not. Note that, this bit will not be cleared by the CPU, so that burden falls on the OS (if it needs this bit at all).
//D, is the 'Cache Disable' bit. If the bit is set, the page will not be cached. Otherwise, it will be.
//W, the controls 'Write-Through' abilities of the page. If the bit is set, write-through caching is enabled. If not, then write-back is enabled instead.
//U, the 'User/Supervisor' bit, controls access to the page based on privilege level. If the bit is set, then the page may be accessed by all; if the bit is not set, however, only the supervisor can access it. For a page directory entry, the user bit controls access to all the pages referenced by the page directory entry. Therefore if you wish to make a page a user page, you must set the user bit in the relevant page directory entry as well as the page table entry.
//R, the 'Read/Write' permissions flag. If the bit is set, the page is read/write. Otherwise when it is not set, the page is read-only. The WP bit in CR0 determines if this is only applied to userland, always giving the kernel write access (the default) or both userland and the kernel (see Intel Manuals 3A 2-20).
//P, or 'Present'. If the bit is set, the page is actually in physical memory at the moment. For example, when a page is swapped out, it is not in physical memory and therefore not 'Present'. If a page is called, but not present, a page fault will occur, and the OS should handle it. (See below.)

/* page_directory_entry_descripter (4kb) */
//reference : :  http://wiki.osdev.org/File:Page_dir.png//
typedef union page_directory_entry_desc{
	uint32_t val;
	struct{
		uint32_t present : 1;
		uint32_t rw : 1;
		uint32_t user_supervisor : 1;
		uint32_t write_through	: 1;
		uint32_t cache_disabled : 1;
		uint32_t accessed : 1;
		uint32_t reserved : 1; // set to 0
		uint32_t page_size : 1;
		uint32_t global : 1; // ignored
		uint32_t avail : 3;
		uint32_t page_addr : 20;
	}__attribute__((packed));
}page_directory_entry_desc;

/* page_table_entry_descripter */
typedef union page_table_entry_desc{
	uint32_t val;
	struct{
		uint32_t present : 1;
		uint32_t rw : 1;
		uint32_t user_supervisor : 1;
		uint32_t write_through	: 1;
		uint32_t cache_disabled : 1;
		uint32_t accessed : 1;
		uint32_t dirty : 1;
		uint32_t page_table_attribute_index : 1;
		uint32_t global : 1;
		uint32_t avail : 3;
		uint32_t page_addr : 20;
	}__attribute__((packed));
}page_table_entry_desc;

/* page_directory_entry_descripter (4Mb) */
typedef union page_directory_entry_desc_4Mb{
		uint32_t val;
	struct{
		uint32_t present : 1;
		uint32_t rw : 1;
		uint32_t user_supervisor : 1;
		uint32_t write_through	: 1;
		uint32_t cache_disabled : 1;
		uint32_t accessed : 1;
		uint32_t dirty : 1;
		uint32_t page_table_attribute_index : 1;
		uint32_t global : 1;
		uint32_t avail : 3;
		uint32_t page_addr : 20;
	}__attribute__((packed));
}page_directory_entry_desc_4Mb;
	

page_directory_entry_desc page_directory[1024] __attribute__((aligned(4096)));
page_table_entry_desc page_table[1024] __attribute__((aligned(4096)));

#endif /* _PAGING_H */

