/* filesys.c 					  *
 * Includes File System Utilities */
#include "filesys.h"

/* File system local variables */

boot_block_t* boot_block;

inode_t* inode_start; 

uint32_t data_block_addr;

fotp_t file_fotp;
fotp_t dir_fotp;

/* 
 *	fs_init()
 *  DESCRIPTION: initialize the file system
 *  INPUTS: 
 *			fname 	- 	the name of the file to read
 *			dentry 	-  	A pointer to the dentry struct
 *  OUTPUTS: 
 *  RETURN VALUE: None
 *  SIDE EFFECTS: changed content of the passed in dentry struct
 */
void fs_init(uint32_t start_addr)
{
	/* assign the start address of the boot block*/
 	boot_block = (boot_block_t*) start_addr;

 	/* assign the start address of the first block inode */
 	inode_start = (inode_t*) (start_addr + BLOCK_SIZE);

	/* assign the start address of the first data block */
 	data_block_addr = (uint32_t) inode_start + (boot_block -> sys_stat).num_inode * BLOCK_SIZE;

 	// for checkpoint 2 we want to init our file descriptor
 	fd[0].flags = 1;	// stdin
 	fd[1].flags = 1;	// stdout

 	file_fotp.open = (uint32_t)&(file_open);
 	file_fotp.close = (uint32_t)&(file_close);
 	file_fotp.read = (uint32_t)&(file_read);
 	file_fotp.write = (uint32_t)&(file_write);

 	dir_fotp.open = (uint32_t)&(dir_open);
 	dir_fotp.close = (uint32_t)&(dir_close);
 	dir_fotp.read = (uint32_t)&(dir_read);
 	dir_fotp.write = (uint32_t)&(dir_write);
}



/* 
 *	read_dentry_by_name()
 *  DESCRIPTION: Read the content of the dentry by its file name
 *  INPUTS: 
 *			fname 	- 	the name of the file to read
 *			dentry 	-  	A pointer to the dentry struct
 *  OUTPUTS: 
 *  RETURN VALUE: 0 on success, -1 on failure 
 *  SIDE EFFECTS: changed content of the passed in dentry struct
 */
int32_t read_dentry_by_name (const uint8_t* fname, dentry_t* dentry)
{
	int i;
	int32_t found_file = -1;
	dentry_t* dentries = (dentry_t*) boot_block + SYS_STAT_SIZE;
	for (i = 0; i < (boot_block -> sys_stat).num_dentry; ++i)
	{
		if (strncmp((int8_t*) dentries[i].f_name, (int8_t*) fname, FILE_NAME_MAX_L))
		{
			strcpy((int8_t*)dentry -> f_name, (int8_t*)fname);
			dentry -> f_type = dentries[i].f_type;
			dentry -> inode = dentries[i].inode;
			found_file = 0;
			break;
		}
	}
	return found_file;
}

 /* 
*	read_dentry_by_index()
*  DESCRIPTION: Read the content of the dentry by its index in the boot block dentry list 
*  INPUTS: 
*			index	- 	index of the file block
*			dentry 	-  	A pointer to the dentry struct
*  OUTPUTS: *  RETURN VALUE: 0 on success, -1 on failure 
*  SIDE EFFECTS: changed content of the passed in dentry struct
*/
 int32_t read_dentry_by_index (uint32_t index, dentry_t* dentry)
 {
 	if (index >= (boot_block -> sys_stat).num_dentry) return -1; /* If the index is out of bound, return -1*/
 	if (index < 0) return -1; /* If the index is less than 0, return -1*/
 	dentry_t* dentries = boot_block + SYS_STAT_SIZE;

 	strcpy((int8_t*)dentry -> f_name, (int8_t*)dentries[index].f_name);
	dentry -> f_type = dentries[index].f_type;
	dentry -> inode = dentries[index].inode;
 	
 	return 0;
 }

/* 
*	read_data()
*  DESCRIPTION: read data from our file system
*  INPUTS: 
*			inode	- 	index of the source inode
*			offset 	-  	index offset to the block in the inode struct
*			buf 	- 	Destination pointer
*			length 	- 	Length of data block to read
*  OUTPUTS: 
*  RETURN VALUE: 0 on success, -1 on failure 
*  SIDE EFFECTS:  
*/
int32_t read_data (uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length){
	uint32_t cur_inode_db_index;		// address of the current data block
	uint32_t cur_db_index;
	uint32_t cur_db_addr;
	uint32_t start_offset, start_addr;
	uint32_t remain_length = length;
	if (length == 0) return 0;
	
	if (inode >= (boot_block -> sys_stat).num_inode) return -1;
	
	cur_inode_db_index = offset / BLOCK_SIZE;

	if (cur_inode_db_index >= inode_start[inode].legnth) return -1; 	/* If the starting data block is out of bound reutrn -1 */

	cur_db_index = inode_start[inode].data_block_index[cur_inode_db_index];
	cur_db_addr = data_block_addr + cur_db_index * BLOCK_SIZE;
	start_offset = offset % BLOCK_SIZE;
	start_addr = cur_db_addr + start_offset;								// offset in the first data block

	while(remain_length > 0)
	{
		if (start_offset + remain_length < BLOCK_SIZE)
		{
			memcpy((void*) (buf + length - remain_length), (void *) start_addr, remain_length);
			return length;
		}
		else 
		{
			memcpy((void*) (buf + length - remain_length), (void *) start_addr, BLOCK_SIZE - start_offset);
			remain_length -= (BLOCK_SIZE - start_offset);
			start_offset = 0;
			cur_inode_db_index++;
			if (cur_inode_db_index >= inode_start[inode].length) return 0; 	/* If the next data block is does not exist in inode, stop*/
			cur_db_index = inode_start[inode].data_block_index[cur_inode_db_index];
			start_addr = data_block_addr + cur_db_index * BLOCK_SIZE;
		}
	}


	return -1; 	// should not reach here
}


//TODO: fncs

int32_t file_open (const uint8_t* filename)
{
	return 0; 	// do nothing becuase nothing means everything
}
int32_t file_read (int32_t fd, void* buf, int32_t nbytes)
{
	uint32_t offset = fd[fd].file_pos;
	int ret = read_data(fd[fd].inode, offset, buf, nbytes);
	if (ret == 0 || -1)
		ret = 0;
	fd[fd].file_pos = offset + ret;	// WILL cause issues when buffer length < nbytes
	return ret;
} 
int32_t file_write (int32_t fd, void* buf, int32_t nbytes)
{
	return 0; // STUB
}

int32_t dir_open
{
//stub
} 
int32_t dir_close
{
//stub
} 
int32_t dir_read
{
//TODO
} 
int32_t dir_write
{
//stub
}



//TODO: test case