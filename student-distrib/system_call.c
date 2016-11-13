/* system_call.h 					  *
 * includes 10 system calls */

#include "system_call.h"
#include "filesys.h"

void* syscall_func[10] = {&halt, &execute, &read, &write, &open,
	&close, &getargs, &vidmap, &set_handler, &sigreturn};

int32_t halt (uint8_t status)
{
	return 0; // stub
}

int32_t execute (const uint8_t* command)
{
	return 0; // stub
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

	((fotp_t*)filedesc[fd].fotp)->write(fd, buf, nbytes);
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

/*  NOTE: Work In Progress
 *	syscall_handler()
 *  DESCRIPTION: Hander for system call (INT 0x80)
 *  INPUTS: none
 *  OUTPUTS: none
 *  RETURN VALUE: return value of the specific system call
 *  SIDE EFFECTS: none
 */

/*int32_t syscall_handler()
{
	uint32_t syscall_num; 
	asm volatile ("	movl 	%%eax, %0" : "=r"(syscall_num));
	syscall_func[syscall_num]();
}
*/