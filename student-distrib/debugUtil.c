#include "debugUtil.h"
#include "system_call.h"
#include "filesys.h"
#include "lib.h"


void test()
{
	clear();
	printf("\n");
	int fd;
	fd = test_syscall_open();
	test_syscall_read(fd);
	test_syscall_write(fd);
	test_syscall_close(fd);
	test_user_prog_page();
}

int test_syscall_open()
{
	int fd;
	printf("Testing open()...\n");
	if (fs_ready())
	{
		fd = open("frame1.txt\0");
		if (fd > 1 && fd < 8)
			printf("Test for open() passed\n");
		else
			printf("Test for open() failed\n");
	}
	else 
	{
		printf("fs not ready\n");
	}
	return fd;
}


void test_syscall_read(int fd)
{
	uint8_t buf[256];
	printf("Testing read()...\n");
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

void test_syscall_write(int fd)
{
	char buf[] = "Terminal Write works!\0";
	printf("Testing write()...\n");
	write(fd, (void*)buf, 256);
	printf("%s\n", buf);
}

void test_syscall_close(int fd)
{
	uint32_t prev_flags = filedesc[fd].flags;
	printf("Testing close()...\n");
	close(fd);
	if (filedesc[fd].flags == prev_flags) 
		printf("Test for close() failed\n");
	else
		printf("Test for close() passed\n");
}

void test_user_prog_page()
{
	printf("Testing User Program Page (Fail if page fault)...\n");
	uint32_t* dummy = (uint32_t*) 0x8000000;
	printf("User Program Page works, uint32_t at %x is %d\n", dummy, *dummy);
}