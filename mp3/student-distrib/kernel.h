#ifndef KERNEL_H
#define KERNEL_H

// Massive list of includes
#include "multiboot.h"
#include "x86_desc.h"
#include "lib.h"
#include "i8259.h"
#include "debug.h"
#include "idt.h"
#include "rtc.h"
#include "paging.h"
#include "terminal.h"
#include "keyboard.h"
#include "directory.h"
#include "kernel_tests.h"
#include "file.h"
#include "sys_calls.h"
#include "serial.h"
#include "sound.h"
#include "timer.h"
#include "mouse.h"
#include "scheduler.h"

// Macros
#define CHECK_FLAG(flags,bit)   ((flags) & (1 << (bit))) //Check if the bit BIT in FLAGS is set

// Function definitions
void entry (unsigned long magic, unsigned long addr);
MI();

#endif
