/* system_call.h 					  *
 * includes 10 system calls */

#include "system_call.h"


#define BUFFERSIZE 128 // size of command buffer
#define HEADERSIZE 40	 // size of ELF file header

/* entry point to the program code */
#define ENTRY_0	24
#define ENTRY_1	25
#define ENTRY_2	26
#define ENTRY_3	27

// Maximum number of processes
#define NUM_PROC 6

#define PAGE_SIZE	0x400000 	// 4MB page size

#define MB132 0x8400000
#define MB8 	0x800000
#define KB8 	0x2000

/* Virtual address to mentioned in the MP doc */
#define PROG_IMG_ADDR		0x8048000

void* syscall_func[10] = {&halt, &execute, &read, &write, &open,
	&close, &getargs, &vidmap, &set_handler, &sigreturn};

uint8_t elf_magic[4] = {0x7f, 0x45, 0x4c, 0x46};
uint8_t task_status[NUM_PROC] = {0,0,0,0,0,0};

/* ebp and esps for each process */
static int32_t save_ebp[NUM_PROC];
static int32_t save_esp[NUM_PROC];

/*
 *	halt (uint8_t status)
 *  DESCRIPTION: Terminates a process,
 *								returning the specified value to its parent process
 *  INPUTS:
 *			status --
 *  OUTPUTS: None
 *  RETURN VALUE: 0 - Success, -1 - Fail
 *  SIDE EFFECTS: None
 */
int32_t halt (uint8_t status)
{
	int i;

	/* close all file descriptors */
	for (i = 0; i < 8; i++) {
		close(i);
	}

	/* Restore ebp and esb */
	// TODO: restore ebp and esp using asm code

	/* If current process is shell, restart the shell */
	// TODO: Check if the program we are halting is shell
	// check it by seeing if current pcb's parent is NULL

	// TODO: set TSS esp

	// TODO: swap page tables

	// TODO: task_status["current pcb's pid"] = 0;

	// TODO: set current pcb as current pcb's parent pcb

	/* jump back to execute asm code */
	asm volatile("jmp back_to_execute");

	return 0;
}

/*
 *	execute (const uint8_t* command)
 *  DESCRIPTION: Attempts to load and execute a new program, handing off the
 *								processor to the new program until it terminates.
 *  INPUTS:
 *			command -- the name of the program's file name to execute
 *  OUTPUTS: None
 *  RETURN VALUE: -1 - Fail
 *								256 - Program dies by an exception
 *								0-255 - Program executes halt system call
 *  SIDE EFFECTS: None
 */
int32_t execute (const uint8_t* command)
{
	int i;
	uint8_t cmd[BUFFERSIZE]; // local buffer to store the command (WITHOUT arguments)

	/* Check if we can run another process */
	int32_t new_pid = get_usable_pid();
	if (new_pid == -1) {
		printf("Maximum number of processes is already running\n");
		return 0;
	}
	task_status[new_pid] = 1;

	/* Save the command (for now, don't care about the arguments)*/
	for (i = 0; i < BUFFERSIZE; i++) {
		if ((cmd[i] == ' ') || (cmd[i] == '\0')) {
			cmd[i] = '\0';
			break;
		}
		cmd[i] = command[i];
	}

	/* Read the program file's dentry with the command */
	dentry_t dentry;
	if (-1 == read_dentry_by_name ((uint8_t *) cmd, &dentry))
		return -1;

	/* Read the file header (first 40 bytes) from the dentry */
	uint8_t file_header[HEADERSIZE];
	read_data(dentry.inode, 0, file_header, HEADERSIZE);

	/* Check the elf magic numbers in the file header */
	for (i = 0; i < 4; i++) {
		if (file_header[i] != elf_magic[i])
			return -1;
	}

	/* Make entry point address by bit shifting byte 24-27 of file header */
	uint32_t entry_point = file_header[ENTRY_0] | (file_header[ENTRY_1] << 8)
	 | (file_header[ENTRY_2] << 16) | (file_header[ENTRY_3] << 24);

	 // NOTE: NEED PCB ************************************************************
 	/* Initialize PCB for this process */
 	// TODO: initialize pcb here
 	// TODO: create pcb pointer for this process

	/* Update PCB address */
	// TODO: declare pcb and pcb pointer here
	// TODO: set the pcb pointer to right place

	/* Flush TLB for prev process */
	uint32_t page_addr;
	asm volatile("movl %%cr3, %0" : "=r" (page_addr));
	asm volatile("movl %0, %%cr3" : : "r" (page_addr));

	/* Set paging for the program file to execute */
	uint32_t program_addr = MB8 + new_pid * PAGE_SIZE;
	// TODO: swap page tables
	// TODO: allocate 4mb page for program

	// NOTE: NEED PCB ************************************************************
	/* Save esp and ebp */
	// TODO: save esp and ebp using asm code

	/* Load the program on the address specified by the MP document */
	file_load(cmd, PROG_IMG_ADDR);

	// NOTE: NEED PCB ************************************************************
	/* Set TSS esp0 */
	// TODO: set tss esp0

	/* Set up the stack for program execution, then IRET */
	asm volatile(" 			\n\
		movw	%0, %%ax 		\n\
		movw	%%ax, %%ds 	\n\
		pushl	%0 					\n\
		pushl	%1 					\n\
		pushf 						\n\
		popl 	%%eax				\n\
		orl 	$0x200, %%eax   \n\
		pushl 	%%eax			\n\
		pushl 	%2 				\n\
		pushl 	%3 				\n\
		iret							\n\
		back_to_execute:	\n\
		"
		:
		: "g" (USER_DS),  "g" (MB132 - 4), "g" (USER_CS), "g" (entry_point)
		: "eax", "memory"
	);

	return 0;
}

int32_t read (int32_t fd, void* buf, int32_t nbytes)
{
	// TODO: Complete line after pcb
	//file_descriptor_t* filedesc =
	
	if (fd < 0 || fd > 7)
		return -1; 	// out of bounds

	((fotp_t*)filedesc[fd].fotp)->read(fd, buf, nbytes);
	return 0;
}

int32_t write (int32_t fd, const void* buf, int32_t nbytes)
{
	// TODO: Complete line after pcb
	//file_descriptor_t* filedesc =

	if (fd < 0 || fd > 7)
		return -1; 	// out of bounds

	((fotp_t*)filedesc[fd].fotp)->write(fd, (void*) buf, nbytes);
	return 0;
} 

int32_t open (const uint8_t* filename)
{
	// TODO: Complete line after pcb
	//file_descriptor_t* filedesc = 
	int i;
	dentry_t dentry;
	int fd_index = -1;

	if (read_dentry_by_name(filename, &dentry) == -1)
		return -1;	// if file doesn't exist

	for (i = 2; i < FD_LENGTH; i++)
	{
		if (filedesc[i].flags != 1)
		{
			fd_index = i;
			filedesc[fd_index].flags = 1;	// use this loc
			filedesc[fd_index].inode = dentry.inode;
			filedesc[fd_index].file_pos = 0;
			switch (dentry.f_type)
			{
				case F_TYPE_RTC:
					filedesc[fd_index].fotp = 0;//TODO
					break;
				case F_TYPE_DIRECTORY:
					filedesc[fd_index].fotp = 0;//TODO
					break;
				case F_TYPE_REGULAR:
					filedesc[fd_index].fotp = (uint32_t) &file_fotp;
					break;
				default:
					break;
			}
			
		break;	// break out of the loop if fd assigned 
		}
	}

	return fd_index;
}

int32_t close (int32_t fd)
{
	// TODO: Complete line after pcb
	//file_descriptor_t* filedesc =

	if (fd < 2 || fd > 7)
		return -1; 	// out of bounds

	filedesc[fd].flags = 0;
	filedesc[fd].inode = 0;
	filedesc[fd].file_pos = 0;
	filedesc[fd].fotp = NULL;

	return 0;
}

int32_t getargs (uint8_t* buf, int32_t nbytes)
{
	return 0; // stub
}

int32_t vidmap (uint8_t** screen_start)
{
	return 0; // stub
}

int32_t set_handler (int32_t signum, void* handler_address)
{
	return 0; // stub
}

int32_t sigreturn (void)
{
	return 0; // stub
}

/*  NOTE: WIP, 
 * 
 *	syscall_handler()
 *  DESCRIPTION: Hander for system call (INT 0x80), See the below link for using %P calling function
 * 		http://stackoverflow.com/questions/3467180/direct-call-using-gccs-inline-assembly
 *  INPUTS: none
 *  OUTPUTS: none
 *  RETURN VALUE: return value of the specific system call
 *  SIDE EFFECTS: none
 */

int32_t syscall_handler()
{
	uint32_t syscall_num, result; 
	asm volatile ("	movl 	%%eax, %0" : "=r"(syscall_num));
	if (syscall_num > 0 && syscall_num <= 10)
	{
		void* func = syscall_func[syscall_num - 1];
		asm volatile ("	pushl	%%edx\n 	\
						pushl 	%%ecx\n 	\
						pushl	%%ebx\n  	\
						call 	*%P1	\n 	\
						movl 	%%eax, %0\n  \
						" 	:"=r"(result)
							: "irm"(func));
	}
	else 
	{
		result = -1;
	}
	return result;
}
/*
 * get_usable_pid()
 * DESCRIPTION: Find and returns an usable pid number
 * Input: none
 * Ouput: None
 * RETURN: currently usable pid. -1 if none available
 * Side_Effect: none
 */
int8_t get_usable_pid() {
	int i;
	for (i = 0; i < NUM_PROC; i++) {
		if (task_status[i] == 0) {
			return i;
		}
	}
	return -1;
}
