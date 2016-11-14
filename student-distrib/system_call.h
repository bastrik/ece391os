/* system_call.h 					  *
 * includes 10 system calls */
#ifndef SYSTEM_CALL_H
#define SYSTEM_CALL_H

#include "types.h"
#include "paging.h"
#include "filesys.h"
#include "rtc.h"
#include "keyboard.h"
#include "x86_desc.h"
#include "lib.h"
#include "i8259.h"

#define FD_LENGTH 8
#define BUFFERSIZE 128 // size of command buffer

#define SYS_HALT    1
#define SYS_EXECUTE 2
#define SYS_READ    3
#define SYS_WRITE   4
#define SYS_OPEN    5
#define SYS_CLOSE   6
#define SYS_GETARGS 7
#define SYS_VIDMAP  8
#define SYS_SET_HANDLER  9
#define SYS_SIGRETURN  10

/* The PCB struct */
typedef struct PCB{
	int32_t pid;
	
	file_descriptor_t file_array[FD_LENGTH];
	uint8_t cmd[BUFFERSIZE];
	uint8_t args[BUFFERSIZE]; 

	int32_t parent_ptr;
	int32_t esp;
	int32_t ebp;

}pcb_t;

pcb_t* curr_pcb;

int32_t halt (uint8_t status);

int32_t execute (const uint8_t* command);

int32_t read (int32_t fd, void* buf, int32_t nbytes);

int32_t write (int32_t fd, const void* buf, int32_t nbytes);

int32_t open (const uint8_t* filename);

int32_t close (int32_t fd);

int32_t getargs (uint8_t* buf, int32_t nbytes);

int32_t vidmap (uint8_t** screen_start);

int32_t set_handler (int32_t signum, void* handler_address);

int32_t sigreturn (void);

int32_t syscall_handler();
/* helper function */
int8_t get_usable_pid();


void init_pcb(pcb_t* pcb);

#endif /* SYSTEM_CALL_H */
