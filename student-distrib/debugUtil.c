#include "debugUtil.h"
#include "system_call.h"
#include "filesys.h"
#include "lib.h"


void test()
{
	clear();
	test_syscall_open();
}

void test_syscall_open()
{
	printf("Testing open()...\n");
	if (fs_ready())
	{
		open("frame1.txt\0");
		if (filedesc[2].flags)
			printf("Test for open() passed\n");
		else
			printf("Test for open() failed\n");
	}
	else 
	{
		printf("fs not ready\n");
	}
		
}


/* WIP: not finished yet */
void test_syscall_read()
{
	clear();
	printf("Testing open...\n");
	if (fs_ready())
	{
		printf("Opening...\n");
		open("frame1.txt");
		if (filedesc[2].flags)
			printf("Test for Open passed\n");
		else
			printf("Test for Open failed\n");
	}
	else 
	{
		printf("fs not ready\n");
	}
		
}
