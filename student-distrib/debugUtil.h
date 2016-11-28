#ifndef DEBUGUTIL_H
#define DEBUGUTIL_H

#include "types.h"

void test();
int32_t test_syscall_open();
void test_syscall_read(int32_t fd);
void test_syscall_write(int32_t fd);
void test_syscall_close(int32_t fd);
void test_user_prog_page();
void test_syscall_linkage();
void test_kernel_page();

#endif 
