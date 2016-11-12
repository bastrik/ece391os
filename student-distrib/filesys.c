/* filesys.c 					  *
 * Includes File System Utilities */
#include "filesys.h"
#include "lib.h"
/* File system local variables */
int is_Init = 0;
boot_block_t* boot_block;
inode_t* inode_start; 
uint32_t data_block_addr;
fotp_t file_fotp;
fotp_t dir_fotp;
// File descriptor declared here
file_descriptor_t filedesc[8];
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
 	inode_start = (inode_t*) (boot_block + 1);
	/* assign the start address of the first data block */
 	data_block_addr = ((uint32_t) inode_start) + (boot_block -> sys_stat).num_inode * BLOCK_SIZE;
 	// for checkpoint 2 we want to init our file descriptor
 	filedesc[0].flags = 1;	// stdin
 	filedesc[1].flags = 1;	// stdout
 	file_fotp.open = (uint32_t)&(file_open);
 	//file_fotp.close = (uint32_t)&(file_close);
 	file_fotp.read = (uint32_t)&(file_read);
 	file_fotp.write = (uint32_t)&(file_write);
 	dir_fotp.open = (uint32_t)&(dir_open);
 	dir_fotp.close = (uint32_t)&(dir_close);
 	dir_fotp.read = (uint32_t)&(dir_read);
 	dir_fotp.write = (uint32_t)&(dir_write);
 	is_Init = 1;
 	//printf("fs init\n");
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
	for (i = 0; i < (boot_block -> sys_stat).num_dentry; ++i)
	{
		if (!strncmp((int8_t*) (boot_block -> dentry)[i].f_name, (int8_t*) fname, FILE_NAME_MAX_L))
		{
			strncpy((int8_t*)dentry -> f_name, (int8_t*)(boot_block -> dentry)[i].f_name, FILE_NAME_MAX_L);
			dentry -> f_type = (boot_block -> dentry)[i].f_type;
			dentry -> inode = (boot_block -> dentry)[i].inode;
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
 	strncpy((int8_t*)dentry -> f_name, (int8_t*)(boot_block -> dentry)[index].f_name, FILE_NAME_MAX_L);
	dentry -> f_type = (boot_block -> dentry)[index].f_type;
	dentry -> inode = (boot_block -> dentry)[index].inode;
 	
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
	if (cur_inode_db_index > inode_start[inode].length / BLOCK_SIZE) return -1; 	/* If the starting data block is out of bound reutrn -1 */
	cur_db_index = inode_start[inode].data_block_index[cur_inode_db_index];
	cur_db_addr = data_block_addr + cur_db_index * BLOCK_SIZE;
	start_offset = offset % BLOCK_SIZE;
	start_addr = cur_db_addr + start_offset;								// offset in the first data block
	while(remain_length > 0)
	{
		// if we can finish copying in this data block
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
			if (cur_inode_db_index > inode_start[inode].length / BLOCK_SIZE) return 0; 	/* If the next data block is does not exist in inode, stop*/
			cur_db_index = inode_start[inode].data_block_index[cur_inode_db_index];
			start_addr = data_block_addr + cur_db_index * BLOCK_SIZE;
		}
	}
	return 0; 	// should not reach here
}
//TODO: fncs
int32_t file_open (const uint8_t* filename)
{
	return 0; 	// do nothing becuase nothing means everything
}
int32_t file_read (int32_t fd, void* buf, int32_t nbytes)
{
	uint32_t offset = filedesc[fd].file_pos;
	int ret = read_data(filedesc[fd].inode, offset, buf, nbytes);
	if (ret == 0 || -1)
		ret = 0;
	filedesc[fd].file_pos = offset + ret;	// WILL cause issues when buffer length < nbytes
	return ret;
} 
// MP3.3 File load -------------------------------------------------------
/* 
 *	file_load(const int8_t * filename, uint32_t address)
 *  DESCRIPTION: Loads an executable file on given memory address
 *  INPUTS: 
 *			filename -- name of the executable file to load on memory 	
 *			address --  address in memory to load the file on
 *  OUTPUTS: 
 *  RETURN VALUE: 0 on success, -1 on failure 
 *  SIDE EFFECTS: a file is loaded on the memory space
 */
int32_t file_load(const int8_t * filename, uint32_t address) 
{
	dentry_t dentry;
	
	/* check if the given filename */
	if(filename == NULL)
	{
		return -1;
	}
	
	/* save the file's dentry data */
	if(-1 == read_dentry_by_name((uint8_t *)filename, &dentry))
	{
		return -1;
	}

	/* load the file on the given address */
	if(read_data(dentry.inode, 0, (uint8_t *)address, 
	    							inode_start[dentry.inode].length))
	{
		return -1;
	}

	/* Success */
	return 0;
}

int32_t file_write (int32_t fd, void* buf, int32_t nbytes)
{
	return 0; // STUB
}
int32_t dir_open ()
{
	return 0;
} 
int32_t dir_close ()
{
	return 0;//stub
} 
dentry_t dir_read (uint32_t index)
{
	dentry_t ret;
	if (index < 0 || index > boot_block->sys_stat.num_dentry)
		return ret;
	ret = boot_block->dentry[index];
	return ret;
} 
int32_t dir_write ()
{
	return 0;//stub
}
//TODO: test case
// 1. list files (ctrl + 1)
void list_files()
{
	dentry_t dentry;
	uint8_t file_name[32];
	uint32_t file_type;
	uint32_t file_size;
	inode_t* inode_ptr;
	int i;
	if (is_Init == 0)
	{
		printf ("Init failed\n");
		return;
	}
	for (i = 0; i < (boot_block->sys_stat).num_dentry; ++i)
	{
		dentry = boot_block->dentry[i];
		strncpy((int8_t*)file_name, (int8_t*)dentry.f_name, FILE_NAME_MAX_L);
		file_name[FILE_NAME_MAX_L - 1] = '\0';
		file_type = dentry.f_type;
		if (file_type == 2)
		{			
			inode_ptr = dentry.inode + inode_start;
			file_size =  inode_ptr -> length;
		} else
			file_size = 0;
		printf ( "Filename: %s, File type: %d, File size: %d\n", file_name, file_type, file_size);
	}
}
// 2. read file by name (ctrl + 2)
void read_file_name (const uint8_t* filename)
{
	int i;
	dentry_t dentry;
	inode_t* inode_ptr;
	uint8_t file_name[FILE_NAME_MAX_L];
	if (read_dentry_by_name(filename, &dentry) == -1)
	{
		printf("File Not Found!");
		return;
	}
	if (dentry.f_type == 2)
	{
		inode_ptr = dentry.inode + inode_start;
		uint32_t size_lim = (BUF_SIZE > inode_ptr -> length) ? inode_ptr -> length : BUF_SIZE;
		uint8_t buf[size_lim];
		if (read_data(dentry.inode, inode_ptr -> length - size_lim, (void*) buf, size_lim) == -1)
		{
			printf ("read error");
			return;
		}
		for (i = 0; i < size_lim; ++i)
		printf("%c", buf[i]);
	}
	
	strncpy((int8_t*)file_name, (int8_t*)dentry.f_name, FILE_NAME_MAX_L);
	file_name[FILE_NAME_MAX_L - 1] = '\0';
	printf("\nFile: %s\n", file_name);
}
// 3. read file by index (ctrl + 3)
void read_file_index (uint32_t* index)
{
	int i;
	dentry_t dentry;
	inode_t* inode_ptr;
	uint8_t file_name[FILE_NAME_MAX_L];
	/* If index is out of bound, reset it to 0 */
	if (*index > (boot_block->sys_stat).num_dentry) *index = 0;
	read_dentry_by_index(*index, &dentry);
	if (dentry.f_type == 2)
	{
		inode_ptr = dentry.inode + inode_start;
		uint32_t size_lim = (BUF_SIZE > inode_ptr -> length) ? inode_ptr -> length : BUF_SIZE;
		uint8_t buf[size_lim];
		if (read_data(dentry.inode, inode_ptr -> length - size_lim, (void*) buf, size_lim) == -1)
		{
			printf ("read error");
			return;
		}
		for (i = 0; i < size_lim; ++i) printf("%c", buf[i]);
	}
	
	strncpy((int8_t*)file_name, (int8_t*)dentry.f_name, FILE_NAME_MAX_L);
	file_name[FILE_NAME_MAX_L - 1] = '\0';
	printf("\nFile: %s\n", file_name);
	
}
