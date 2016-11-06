/* system_call.h 					  *
 * includes 10 system calls */

#include "system_call.h"
#include "filesys.h"

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
	if (fd < 0 || fd > 7)
		return -1; 	// out of bounds

	((fotp_t*)filedesc[fd].fotp)->read(fd, buf, nbytes);
	return 0;
}

int32_t write (int32_t fd, const void* buf, int32_t nbytes)
{
	return 0;
} // nothing to do right now

int32_t open (const uint8_t* filename)
{
	int i;
	dentry_t* dentry;
	int fd_index = -1;

	if (read_dentry_by_name(filename, dentry) == -1)	
		return -1;	// if file doesn't exist

	for (i = 2; i < FD_LENGTH; i++)
	{
		if (filedesc[i].flags != 1)
		{
			fd_index = i;
			filedesc[fd_index].flags = 1;	// use this loc
			filedesc[fd_index].inode = dentry->inode;
			filedesc[fd_index].file_pos = 0;
			switch (dentry->f_type)
			{
				case F_TYPE_RTC:
					filedesc[fd_index].fotp = 0;//TODO
					break;
				case F_TYPE_DIRECTORY:
					filedesc[fd_index].fotp = 0;//TODO
					break;
				case F_TYPE_REGULAR:
					filedesc[fd_index].fotp = 0;//TODO
					break;
				default:
					break;
			}
			

		}
	}



	return fd_index;
}

int32_t close (int32_t fd)
{
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

