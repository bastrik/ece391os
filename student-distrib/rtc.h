#ifndef _RTC_H
#define _RTC_H

#include "types.h"
#include "x86_desc.h"
#include "lib.h"
#include "i8259.h"

#define IRQ8 0x08
#define IRQ2 0x02
#define RATE_LIMIT 0x0F
#define RTC_INDEX_PORT 0x70
#define RTC_RW_PORT 0x71

#define	RTC_STATUS_REGISTER_A 0x8A
#define RTC_STATUS_REGISTER_B 0x8B
#define RTC_STATUS_REGISTER_C 0x0C

extern void rtc_init();
extern void rtc_handler();

#endif /* _RTC_H */

