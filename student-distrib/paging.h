#ifndef _PAGING_H
#define _PAGING_H

#include "types.h"

extern void paging_init();
void enable_4MBpaging(uint32_t address);
void map_4mb_page(uint32_t virt_addr, uint32_t phys_addr, uint32_t pid);
void swap_tables(int32_t pid);
extern uint32_t get_current_pid();

#define TABLE_SIZE 1024
#define BIT_SHIFT 12
#define TEN_BIT_OFFSET 1024

#define MB128	134217728
#define MB4		4194304
#define MAX_PROGRAMS 7

/* page_directory_entry_descripter (4kb) */
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

typedef	struct process_page_directory_desc{
	page_directory_entry_desc page_directory_array[1024] __attribute__((aligned(4096)));
}process_page_directory_desc;

typedef	struct process_page_table_desc{
	page_table_entry_desc page_table_array[1024] __attribute__((aligned(4096)));
}process_page_table_desc;

process_page_directory_desc process_page_directory[7];
process_page_table_desc process_page_table[7];
process_page_table_desc video_page_table[7];

	

page_directory_entry_desc page_directory[1024] __attribute__((aligned(4096)));
page_table_entry_desc page_table[1024] __attribute__((aligned(4096)));
// page_directory_entry_desc_4Mb page_directory_4Mb[1024] __attribute__((aligned(4096)));


// uint32_t page_directory_entry[1024] __attribute__((aligned (4096)));
// uint32_t page_table_entry[1024] __attribute__((aligned (4096)));

#endif /* _PAGING_H */

