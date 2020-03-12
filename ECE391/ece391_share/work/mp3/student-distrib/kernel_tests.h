#ifndef KERNEL_TESTS_C
#define KERNEL_TESTS_C

// Includes
#include "lib.h"
#include "rtc.h"
#include "types.h"
#include "global_includes.h"
#include "scheduler.h"
#include "directory.h"
#include "serial.h"
#include "mouse.h"

// Functions
int idt_test();
int paging_test();
int print_error();
int rtc_test();
int mouse_test();
int directory_read_test();

int run_ktests();
void single_term();

#endif
