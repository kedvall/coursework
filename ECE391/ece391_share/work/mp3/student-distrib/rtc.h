#ifndef RTC_H
#define RTC_H

#include "types.h"
#include "global_includes.h"
#include "scheduler.h"
#include "i8259.h"
#include "lib.h"

// ports that define where the RTC stuff is
#define RTC_INDEX   0x70    // also used to disable/enable NMI!
#define RTC_DATA    0x71

#define REG_A    0x0A
#define REG_B    0x0B
#define REG_C    0x0C

#define MAX_FREQ    1024

#define SIX_BIT    0x40

// function declarations -----
// initializing our rtc
void rtc_init();
void rtc_handler();
int32_t rtc_open (const uint8_t* filename);
int32_t rtc_read (int32_t fd, void* buf, int32_t nbytes);
int32_t rtc_write (int32_t fd, const void* buf, int32_t nbytes);
int32_t rtc_close (int32_t fd);
void rtc_wait (int seconds, int freq);

extern int rtc_testing;

// calculating the interrupt rate based off divider value
//Shayna is GREAT (especially at massages :D)
//void change_int_rate(uint8_t rate);

#endif /* RTC_H */
