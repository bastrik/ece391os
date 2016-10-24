/* filesys.h 					  *
 * Includes File System Utilities */
#ifndef FILESYS_H
#define FILESYS_H

#include "types.h"
#include "lib.h"

#define F_TYPE_RTC 			0
#define F_TYPE_DIRECTORY 	1
#define F_TYPE_REGULAR 		2

/* Size of Blocks */
#define BLOCK_SIZE 			4096
#define SYS_STAT_SIZE 		512
#define DENTRY_SIZE 		512

#define FILE_NAME_MAX_L		32



/*
 * file descriptor
 */
typedef struct {
	uint32_t fotp;			// file operations table pointer
	uint32_t inode; 		// inode pointer
	uint32_t file_pos; 		// file position
	uint32_t flags; 			// flags
} file_descriptor_t;

/*
 * File operations table pointer (move at a later date??)
 */
typedef struct {
	int32_t (*open)(uint8_t*);			// file operations table pointer
	int32_t (*close)(int32_t);			// inode pointer
	int32_t (*read)(int32_t, void*, int32_t);	// file position
	int32_t (*write)(int32_t, void*, int32_t);	// STUB
} fotp_t;

/* System Statistics struct 	*
 *								*/
typedef struct sys_stat
{
	uint32_t num_dentry;
	uint32_t num_inode;
	uint32_t num_data_block;
	uint8_t reserved[52];
} sys_stat_t;

/* The index node struct 		*
 *								*/
typedef struct inode
{
	uint32_t length;
	uint32_t data_block_index[1023];
} inode_t;

/* The 64B directory entry struct 	*
 *									*/
typedef struct dentry
{
	uint8_t f_name[32];
	uint32_t f_type;
	uint32_t inode;
	uint8_t reserved[24];
} dentry_t;

/* Boot Block struct 				*
 *									*/
typedef struct boot_block
{
	sys_stat_t sys_stat;
	dentry_t dentry[63];
} boot_block_t;

/* initialize the file system */
void fs_init(uint32_t start_addr);

/*Read the content of the dentry by its file name*/
int32_t read_dentry_by_name (const uint8_t* fname, dentry_t* dentry);

/*Read the content of the dentry by its index in the boot block dentry list */
int32_t read_dentry_by_index (uint32_t index, dentry_t* dentry);

/* read data from our file system */
int32_t read_data (uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length);

/* Function pointers for File descriptor */
/* For Directory */
int32_t dir_open ();
int32_t dir_close ();
dentry_t dir_read ();
int32_t dir_write();
/* For File */
int32_t file_open ();
int32_t file_read ();
int32_t file_write ();

void list_files();

extern file_descriptor_t filedesc[8]; 	// checkpoint 2, declared in filesys.c

#endif /* FILESYS_H */
