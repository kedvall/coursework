#ifndef _IDT_INIT_H
#define _IDT_INIT_H

#include "x86_desc.h"
#include "lib.h"
#include "i8259.h"
#include "timer.h"

extern void initIdt();

/* Intel specified interrupts */
extern void divideByZeroException_wrap();
extern void debugException_wrap();
extern void NMIInterrupt_wrap();
extern void breakpointException_wrap();
extern void overflowException_wrap();
extern void boundRangeExceededException_wrap();
extern void invalidOpcodeException_wrap();
extern void deviceNotAvailableException_wrap();
extern void doubleFaultException_wrap();
extern void coprocessorSegmentOverrun_wrap();
extern void invalidTSSException_wrap();
extern void segmentNotPresent_wrap();
extern void stackFaultException_wrap();
extern void generalProtectionException_wrap();
extern void pageFaultException_wrap();
extern void fpuFloatingPointError_wrap();
extern void alignmentCheckException_wrap();
extern void machineCheckException_wrap();
extern void SIMDFloatingPointException_wrap();
extern void fpuFloatingPointError_wrap();

extern void keyboardHandler_wrapper();
extern void rtcHandler_wrapper();
extern void COM1_handler_wrapper();
extern void COM2_handler_wrapper();
extern void sys_call_dispatch();
extern void timer_wrapper();
extern void mouse_handler_wrapper();

typedef struct
{
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp;
    uint32_t edx;
    uint32_t ecx;
    uint32_t ebx;
    uint32_t eax;
    uint32_t ds;
    uint32_t es;
    uint32_t exception_num;
    uint32_t error_code;
} system_status;

#endif
