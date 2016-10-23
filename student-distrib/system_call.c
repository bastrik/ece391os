/* system_call.h 					  *
 * includes 10 system calls */

#include "system_call.h"

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

	((fotp_t*)fd[fd].fotp)->read(fd, buf, nbytes);
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

	if (read_dentry_by_name(filename, dentry) == -1)	// if file doesn't exist
		return -1;

	for (i = 2; i < FD_LENGTH; i++)
	{
		if (fd[i].flags != 1)
		{
			fd_index = i;
			fd[fd_index].flags = 1;	// use this loc
			fd[fd_index].inode = dentry->inode;
			fd[fd_index].file_pos = 0;
			switch (dentry->f_type)
			{
				case F_TYPE_RTC:
					fd[fd_index].fotp = //TODO
					break;
				case F_TYPE_DIRECTORY:
					fd[fd_index].fotp = //TODO
					break;
				case F_TYPE_REGULAR:
					fd[fd_index].fotp = //TODO
					break;
				case default:
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

	fd[fd].flags = 0;
	fd[fd_index].inode = 0;
	fd[fd_index].file_pos = 0;
	fd[fd_index].fotp = NULL;

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

