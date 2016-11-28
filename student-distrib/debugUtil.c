#include "debugUtil.h"
#include "system_call.h"
#include "filesys.h"
#include "types.h"
#include "lib.h"

/* Sihao: stand alone debug file for system call testing without pcb working */
void test()
{
	clear();
	printf("\n");
	int32_t fd;
	fd = test_syscall_open();
	test_syscall_read(fd);
	test_syscall_write(fd);
	test_syscall_close(fd);
	test_user_prog_page();
	test_syscall_linkage();
}

int32_t test_syscall_open()
{
	int32_t fd;
	uint8_t buf[] = "frame1.txt\0";
	uint8_t* ptr = buf;
	printf("Testing open()...");
	if (fs_ready())
	{
		fd = open(ptr);
		if (fd > 1 && fd < 8)
			printf("Passed!\n");
		else
			printf("Failed!\n");
	}
	else 
	{
		printf("fs not ready\n");
	}
	return fd;
}


void test_syscall_read(int32_t fd)
{
	uint8_t buf[256];
	printf("Testing read() (see the fish for success)...\n");
	if (fs_ready())
	{
		read(fd, (void*)buf, 256);
		printf("%s\n", buf);
	}
	else 
	{
		printf("fs not ready\n");
	}
}

void test_syscall_write(int32_t fd)
{
	char buf[] = "Terminal Write works!\0";
	printf("Testing write()...");
	write(fd, (void*)buf, 256);
	printf("%s\n", buf);
}

void test_syscall_close(int32_t fd)
{
	//file_descriptor_t* filedesc = curr_pcb -> file_array;
	uint32_t prev_flags = filedesc[fd].flags;
	printf("Testing close()...");
	close(fd);
	if (filedesc[fd].flags == prev_flags) 
		printf("Failed!\n");
	else
		printf("Passed!\n");
}

void test_user_prog_page()
{
	printf("Testing User Program Page (Fail if page fault)...");
	uint32_t* dummy = (uint32_t*) 0x8000000;
	printf("uint32_t at %x is %d\n", dummy, *dummy);
}

void test_syscall_linkage()
{
	file_descriptor_t* filedesc = curr_pcb -> file_array;
	printf("Testing syscall_handler()...");
	uint8_t buf[] = "frame1.txt\0";
	uint8_t* ptr = buf; 
	asm volatile ("	movl 	$5, %%eax\n \
					movl 	%0, %%ebx\n \
				"	:
					: "irm"(ptr));	
	syscall_handler();
	if (filedesc[2].flags) 
		printf("Passed!\n");
	else 
		printf("Failed!\n");
}

void test_kernel_page()
{
	uint32_t* random_stuff = 0x7ffc54;
	printf("%d\n", *random_stuff);
}