#ifndef _RTC_H
#define _RTC_H

#include "types.h"
#include "x86_desc.h"
#include "lib.h"
#include "i8259.h"

#define IRQ8 0x08
#define IRQ2 0x02
//#define RATE_LIMIT 0x0F
#define RTC_INDEX_PORT 0x70
#define RTC_RW_PORT 0x71
#define RTC_IDT_VEC 0x28

#define	RTC_STATUS_REGISTER_A 0x8A
#define RTC_STATUS_REGISTER_B 0x8B
#define RTC_STATUS_REGISTER_C 0x0C

#define HZ2_RATE 0x0F
#define HZ4_RATE 0x0E
#define HZ8_RATE 0x0D
#define HZ16_RATE 0x0C
#define HZ32_RATE 0x0B
#define HZ64_RATE 0x0A
#define HZ128_RATE 0x09
#define HZ256_RATE 0x08
#define HZ512_RATE 0x07
#define HZ1024_RATE 0x06
#define MAX_FREQ 1024

extern void rtc_init();
extern void rtc_handler();

extern uint32_t rtc_open();
extern uint32_t rtc_close();
extern uint32_t rtc_read();
extern uint32_t rtc_write(int32_t fd, const uint32_t* buf, int32_t nbyte);
extern void test_rtc(int freq, int flag);


#endif /* _RTC_H */