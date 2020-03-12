.text

.globl keyboardHandler_wrapper
.globl rtcHandler_wrapper
.globl COM1_handler_wrapper
.globl COM2_handler_wrapper
.globl timer_wrapper
.globl mouse_handler_wrapper

.globl divideByZeroException, debugException, NMIInterrupt, breakpointException, overflowException, \
        boundRangeExceededException, invalidOpcodeException, deviceNotAvailableException, doubleFaultException, \
        coprocessorSegmentOverrun, invalidTSSException, segmentNotPresent, stackFaultException, generalProtectionException, \
        pageFaultException, fpuFloatingPointError, alignmentCheckException, machineCheckException, \
        SIMDFloatingPointException

.globl divideByZeroException_wrap, debugException_wrap, NMIInterrupt_wrap, breakpointException_wrap, overflowException_wrap, \
        boundRangeExceededException_wrap, invalidOpcodeException_wrap, deviceNotAvailableException_wrap, doubleFaultException_wrap, \
        coprocessorSegmentOverrun_wrap, invalidTSSException_wrap, segmentNotPresent_wrap, stackFaultException_wrap, generalProtectionException_wrap, \
        pageFaultException_wrap, fpuFloatingPointError_wrap, alignmentCheckException_wrap, machineCheckException_wrap, \
        SIMDFloatingPointException_wrap

.globl exitException
.globl halt_wrap

divideByZeroException_wrap:
    pushl $0
    pushl $0
    pushl %es
    pushl %ds
    pushal
    jmp common_interrupt

debugException_wrap:
    pushl $0
    pushl $1
    pushl %es
    pushl %ds
    pushal
    jmp common_interrupt

NMIInterrupt_wrap:
    pushl $0
    pushl $2
    pushl %es
    pushl %ds
    pushal
    jmp common_interrupt

breakpointException_wrap:
    pushl $0
    pushl $3
    pushl %es
    pushl %ds
    pushal
    jmp common_interrupt

overflowException_wrap:
    pushl $0
    pushl $4
    pushl %es
    pushl %ds
    pushal
    jmp common_interrupt

boundRangeExceededException_wrap:
    pushl $0
    pushl $5
    pushl %es
    pushl %ds
    pushal
    jmp common_interrupt

invalidOpcodeException_wrap:
    pushl $0
    pushl $6
    pushl %es
    pushl %ds
    pushal
    jmp common_interrupt

deviceNotAvailableException_wrap:
    pushl $0
    pushl $7
    pushl %es
    pushl %ds
    pushal
    jmp common_interrupt

doubleFaultException_wrap:
    /* dummy need not be pushed */
    pushl $8
    pushl %es
    pushl %ds
    pushal
    jmp common_interrupt

coprocessorSegmentOverrun_wrap:
    pushl $0
    pushl $9
    pushl %es
    pushl %ds
    pushal
    jmp common_interrupt

invalidTSSException_wrap:
    /* dummy need not be pushed */
    pushl $10
    pushl %es
    pushl %ds
    pushal
    jmp common_interrupt

segmentNotPresent_wrap:
    /* dummy need not be pushed*/
    pushl $11
    pushl %es
    pushl %ds
    pushal
    jmp common_interrupt

stackFaultException_wrap:
    /* dummy need not be pushed*/
    pushl $12
    pushl %es
    pushl %ds
    pushal
    jmp common_interrupt

generalProtectionException_wrap:
    /* dummy need not be pushed*/
    pushl $13
    pushl %es
    pushl %ds
    pushal
    jmp common_interrupt

pageFaultException_wrap:
    /* dummy need not be pushed*/
    pushl $14
    pushl %es
    pushl %ds
    movl %cr2, %eax
    pushal
    jmp common_interrupt

fpuFloatingPointError_wrap:
    /* dummy need not be pushed*/
    pushl $16
    pushl %es
    pushl %ds
    pushal
    jmp common_interrupt

alignmentCheckException_wrap:
    /* dummy need not be pushed*/
    pushl $17
    pushl %es
    pushl %ds
    pushal
    jmp common_interrupt

machineCheckException_wrap:
    pushl $0
    pushl $18
    pushl %es
    pushl %ds
    pushal
    jmp common_interrupt

SIMDFloatingPointException_wrap:
    pushl $0
    pushl $19
    pushl %es
    pushl %ds
    pushal
    jmp common_interrupt

timer_wrapper:
    pushal
/*    pushfl        IT WAS NOT SARAH AND KEVINS FAULT IT WAS SOMEBODY ELSES*/
    call timer_handler
    popal
    sti
    iret

mouse_handler_wrapper:
    pushal
    call mouse_handler
    popal
    sti
    iret

rtcHandler_wrapper:
    pushal
    call rtc_handler
    popal
    sti
    iret

 keyboardHandler_wrapper:
    pushal
    call keyboardHandler
    popal
    sti
    iret

COM1_handler_wrapper:
    pushal
    call COM1_handler
    popal
    sti
    iret

COM2_handler_wrapper:
    pushal
    call COM2_handler
    popal
    sti
    iret

common_interrupt:
    movl 40(%esp), %eax /* To fetch the exception number after pushing all registers, one must move down 40 bytes to get it */
    call *interrupt_c_list(, %eax, 4)
    call exitException
    jmp halt_wrap

/* List of all the C-counterpart code for the exception handlers */
interrupt_c_list:
    .long divideByZeroException, debugException, NMIInterrupt, breakpointException, overflowException, \
    boundRangeExceededException, invalidOpcodeException, deviceNotAvailableException, doubleFaultException, \
    coprocessorSegmentOverrun, invalidTSSException, segmentNotPresent, stackFaultException, generalProtectionException, \
    pageFaultException, 0, fpuFloatingPointError, alignmentCheckException, machineCheckException, \
    SIMDFloatingPointException
