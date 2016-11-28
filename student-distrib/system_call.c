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
	for (i = 0; i < FD_LENGTH; i++) {
		close(i);
	}

	/* Restore ebp and esb */
	asm volatile("			\n\
			movl %0, %%ebp 	\n\
			"
			:
			: "r" (save_ebp[curr_pcb->pid])
			: "memory");

	asm volatile("			\n\
			movl %0, %%esp 	\n\
			"
			:
			: "r" (save_esp[curr_pcb->pid])
			: "memory");

	pcb_t* curr_parent_ptr;
	curr_parent_ptr = (pcb_t*) curr_pcb->parent_ptr;

	// if current shell is the first program. restart the shell
	if (curr_parent_ptr == NULL)
	{
		task_status[curr_pcb->pid] = 0;
		curr_pcb = NULL;

		uint8_t cmd[33] = "shell";
		execute(cmd);
		return 0;
	}

	(&tss)->esp0 = (uint32_t)curr_parent_ptr + KB8 - 4;
	set_table(curr_parent_ptr->pid);

	// update current pcb and pcb track
	task_status[curr_pcb->pid] = 0;
	curr_pcb = curr_parent_ptr;

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
	int i, j;
	uint8_t cmd[BUFFERSIZE]; // local buffer to store the command
	uint8_t arg[BUFFERSIZE]; // local buffer to store the argument
	int32_t new_pid;
	pcb_t  new_pcb; 
	pcb_t* pcb_pointer; 
	init_pcb(&new_pcb);

	/* Save the command (for now, don't care about the arguments)*/
	for (i = 0; i < BUFFERSIZE; i++) {
		if ((command[i] == ' ') || (command[i] == '\0')) {
			cmd[i] = '\0';
			break;
		}
		cmd[i] = command[i];
		new_pcb.cmd[i] = command[i];
	}
	// skip spaces
	i++;
	for (; i < BUFFERSIZE; i++)
	{
		if (cmd[i] != ' ')
		{
			break;
		}
		i++;
	}

	// parse arguemnts
	for (j = i; j < (BUFFERSIZE - i); j++) {
		new_pcb.args[j - i] = command[j];
		arg[j-i] = command[j];

		if (command[j] == '\0') 
			break;
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

	 new_pid = get_usable_pid();

	/* Check if we can run another process */
	if (new_pid == -1) {
		printf("Maximum number of processes is already running\n");
		return 0;
	}
	pcb_pointer = (pcb_t*)(MB8 - KB8 - (new_pid * KB8));
	memcpy((void*) pcb_pointer, &new_pcb, sizeof(pcb_t));
	task_status[new_pid] = 1;
	 // NOTE: NEED PCB ************************************************************
 	/* Initialize PCB for this process */
 	// TODO: initialize pcb here
 	// TODO: create pcb pointer for this process

	/* Update PCB address */
	// TODO: declare pcb and pcb pointer here
	// TODO: set the pcb pointer to right place

	// TODO: swap page tables
	// TODO: allocate 4mb page for program
	pcb_pointer->pid = new_pid;
	strcpy((int8_t*) pcb_pointer->cmd, (int8_t*) cmd);

	/* Flush TLB for prev process */
	uint32_t page_addr;

	asm volatile("movl %%cr3, %0" : "=r" (page_addr));
	asm volatile("movl %0, %%cr3" : : "r" (page_addr));

	/* Set paging for the program file to execute */
	set_table(new_pid);
	uint32_t program_addr = MB8 + new_pid * PAGE_SIZE;
	map_to_4mb(MB128, program_addr, new_pid);

	pcb_pointer->parent_ptr = (uint32_t) curr_pcb;
	curr_pcb = pcb_pointer;
	// NOTE: NEED PCB ************************************************************
	/* Save esp and ebp */
	// TODO: save esp and ebp using asm code

	// save esp and ebp
	asm volatile("movl %%esp, %0"
		:"=a"(save_esp[curr_pcb->pid])
		:
		:"cc"
	);

	asm volatile("movl %%ebp, %0"
		:"=a"(save_ebp[curr_pcb->pid])
		:
		:"cc"
	);

	// set kernel esp
	(&tss)->esp0 = (uint32_t)curr_pcb + KB8 - 4;

	/* Load the program on the address specified by the MP document */
	file_load(cmd, PROG_IMG_ADDR);

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
	/* Sihao: declare a filedesc pointer just so it would make life easier when merging with the PCB code*/
	//file_descriptor_t* filedesc = curr_pcb -> file_array;
	
	if (fd < 0 || fd > 7)
		return -1; 	// out of bounds

	(filedesc[fd].fotp)->read(fd, (void*) buf, nbytes);
	return 0;
}

int32_t write (int32_t fd, const void* buf, int32_t nbytes)
{
	/* Sihao: declare a filedesc pointer just so it would make life easier when merging with the PCB code*/
	//file_descriptor_t* filedesc = curr_pcb -> file_array;

	if (fd < 0 || fd > 7)
		return -1; 	// out of bounds

	(filedesc[fd].fotp)->write(fd, (void*) buf, nbytes);
	return 0;
} 

int32_t open (const uint8_t* filename)
{
	/* Sihao: declare a filedesc pointer just so it would make life easier when merging with the PCB code*/
	//file_descriptor_t* filedesc = curr_pcb -> file_array;
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
					filedesc[fd_index].fotp = &rtc_fotp;
					break;
				case F_TYPE_DIRECTORY:
					filedesc[fd_index].fotp = &dir_fotp;
					break;
				case F_TYPE_REGULAR:
					filedesc[fd_index].fotp = &file_fotp;
					break;
				default:
					break;
			}
		printf("%d\n", fd_index); // debug
		break;	// break out of the loop if fd assigned 
		}
	}
	return fd_index;
}

int32_t close (int32_t fd)
{
	/* Sihao: declare a filedesc pointer just so it would make life easier when merging with the PCB code*/
	//file_descriptor_t* filedesc = curr_pcb -> file_array;

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
	/* If the handler address is 0 (NULL), do nothing */
	if (handler_address == NULL) return -1;

	/* Otherwise set the idt entry */
	SET_IDT_ENTRY(idt[signum], (uint32_t)&handler_address);

	/* Success */
	return 0;
}

int32_t sigreturn (void)
{
	return 0; // stub
}

/*   NOTE: WIP, Only tested with open()
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
						popl	%%ebx\n		\
						popl	%%ecx\n		\
						popl	%%edx\n		\
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

/*
 * init_pcb
 * DESCRIPTION: Initialize the pcb [stdin/stdout/clear space of file_array]
 * Input: PCB pointer
 * Ouput: None
 * RETURN: None
 * Side_Effect: None
 */
void init_pcb(pcb_t* pcb) {

	int i;

	// initialize stdin
	pcb->file_array[0].fotp = (fotp_t*) &terminal_fotp;
	pcb->file_array[0].inode = 0;
	pcb->file_array[0].file_pos = 0;
	pcb->file_array[0].flags = 1;

	// initialize stdout
	pcb->file_array[1].fotp = (fotp_t*) &terminal_fotp;
	pcb->file_array[1].inode = 0;
	pcb->file_array[1].file_pos = 0;
	pcb->file_array[1].flags = 1;
	
	// initialize the file array
	for(i = 2; i < FD_LENGTH; i++) {
		pcb->file_array[i].fotp = 0;
		pcb->file_array[i].inode = 0;
		pcb->file_array[i].file_pos = 0;
		pcb->file_array[i].flags = 0;
	}

	pcb->parent_ptr = NULL;
}

