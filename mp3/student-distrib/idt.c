#include "idt.h"

/*
 * intIdt
 *   DESCRIPTION: initialize the IDT
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: A IDT is created
 */
void initIdt()
{
  /* Load the IDT*/
  lidt(idt_desc_ptr);

  int i; // iterator for the loops below

  for(i = 0; i < NUM_VEC; i++)
  {
    idt[i].seg_selector = KERNEL_CS;

    /* I think we are only using interrupt gates so let's set the reserved bits
       to 0D110 */
    idt[i].reserved3 = 0;
    idt[i].reserved2 = 1;
    idt[i].reserved1 = 1;
    idt[i].reserved0 = 0;

    /* Making a 32 bit operating system, so I think this is always 1. */
    idt[i].size = 1;

    /* Of course the segment is present - clarify this with a TA anyway*/
    idt[i].present = 1;

    idt[i].dpl = (i == 0x80) ? 3 : 0;
  }

  /* Setting IDT offsets for Intel specific interrupts*/
  SET_IDT_ENTRY(idt[0], divideByZeroException_wrap);
  SET_IDT_ENTRY(idt[1], debugException_wrap);
  SET_IDT_ENTRY(idt[2], NMIInterrupt_wrap);
  SET_IDT_ENTRY(idt[3], breakpointException_wrap);
  SET_IDT_ENTRY(idt[4], overflowException_wrap);
  SET_IDT_ENTRY(idt[5], boundRangeExceededException_wrap);
  SET_IDT_ENTRY(idt[6], invalidOpcodeException_wrap);
  SET_IDT_ENTRY(idt[7], deviceNotAvailableException_wrap);
  SET_IDT_ENTRY(idt[8], doubleFaultException_wrap);
  SET_IDT_ENTRY(idt[9], coprocessorSegmentOverrun_wrap);
  SET_IDT_ENTRY(idt[10], invalidTSSException_wrap);
  SET_IDT_ENTRY(idt[11], segmentNotPresent_wrap);
  SET_IDT_ENTRY(idt[12], stackFaultException_wrap);
  SET_IDT_ENTRY(idt[13], generalProtectionException_wrap);
  SET_IDT_ENTRY(idt[14], pageFaultException_wrap);
  SET_IDT_ENTRY(idt[16], fpuFloatingPointError_wrap);
  SET_IDT_ENTRY(idt[17], alignmentCheckException_wrap);
  SET_IDT_ENTRY(idt[18], machineCheckException_wrap);
  SET_IDT_ENTRY(idt[19], SIMDFloatingPointException_wrap);

  /* Set IDT offset for the PIT*/
  SET_IDT_ENTRY(idt[0x20], timer_wrapper);

  /* Set IDT offset for the keyboard */
  SET_IDT_ENTRY(idt[0x21], keyboardHandler_wrapper);

  /* Set IDT offset for COM1 and COM2 communication */
  SET_IDT_ENTRY(idt[0x24], COM1_handler_wrapper);
  SET_IDT_ENTRY(idt[0x23], COM2_handler_wrapper);

  /* set IDT offset for the rtc*/
  SET_IDT_ENTRY(idt[0x28], rtcHandler_wrapper);
  
  /* set IDT offset for the mouse*/
  SET_IDT_ENTRY(idt[0x2C], mouse_handler_wrapper);

  /* IDT offset for system call interrupts */
  SET_IDT_ENTRY(idt[0x80], sys_call_dispatch);
}

/*
 *  Function to print out Exception Error screen
 *   DESCRIPTION: this prints out the contents of the registers as well as the exception number and error code
 *   INPUTS: a system_status struct
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: eax is filled with contents of CR2 register during a page fault
 */
static void print_error_page(system_status stat)
{
    printf("edi: %x\nesi: %x\nebp: %x\nesp: %x\nedx: %x\necx: %x\nebx: %x\neax: %x\nds: %x\nes: %x\nException #: %x\nError code: %x\n",
        stat.edi, stat.esi, stat.ebp, stat.esp, stat.edx, stat.ecx,
        stat.ebx, stat.eax, stat.ds, stat.es, stat.exception_num,
        stat.error_code);
}

/*
 *  Intel-specific Exception Handlers from 0 till 19
 *   DESCRIPTION: each exception clears the screen, prints out the exception and
 *                stalls the system with a while loop
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: prints out to video memory and stalls the system
 */

/* Index 0 */
void divideByZeroException(system_status stat)
{
    clear();
    printf("You just divided by zero. Don't do that again. Ever.\n\n");
    print_error_page(stat);
    return;

}

/* Index 1 */
void debugException(system_status stat)
{
  clear();
  printf("A debug exception has occured. omg murg, what even is this?!\n\n");
  print_error_page(stat);
  return;
}

/* Index 2 */
void NMIInterrupt(system_status stat)
{
  clear();
  printf("NMI interrupt occured\n\n");
  print_error_page(stat);
  return;
}

/* Index 3 */
void breakpointException(system_status stat)
{
  clear();
  printf("A breakpoint exception occured\n\n");
  print_error_page(stat);
  return;
}

/* Index 4 */
void overflowException(system_status stat)
{
  clear();
  printf("An overflow exception occured\n\n");
  print_error_page(stat);
  return;
}

/* Index 5 */
void boundRangeExceededException(system_status stat)
{
  clear();
  printf("Bound Range Exceeded occured\n\n");
  print_error_page(stat);
  return;
}

/* Index 6 */
void invalidOpcodeException(system_status stat)
{
  clear();
  printf("An invalid opcode exception occured\n\n");
  print_error_page(stat);
  return;
}

/* Index 7 */
void deviceNotAvailableException(system_status stat)
{
  clear();
  printf("A device not available exception occured\n\n");
  print_error_page(stat);
  return;
}

/* Index 8 */
void doubleFaultException(system_status stat)
{
  clear();
  printf("A double fault occured :O\n\n");
  print_error_page(stat);
  return;
}

/* Index 9 */
void coprocessorSegmentOverrun(system_status stat)
{
  clear();
  printf("A coprocessorSegmentOverrun exception has occured\n\n");
  print_error_page(stat);
  return;
}

/* Index 10 */
void invalidTSSException(system_status stat)
{
  clear();
  printf("An invalid TSS Exception occured\n\n");
  print_error_page(stat);
  return;
}

/* Index 11 */
void segmentNotPresent(system_status stat)
{
  clear();
  printf("A segment not present exception occured\n\n");
  print_error_page(stat);
  return;
}

/* Index 12 */
void stackFaultException(system_status stat)
{
  clear();
  printf("A stack fault exception occured\n\n");
  print_error_page(stat);
  return;
}

/* Index 13 */
void generalProtectionException(system_status stat)
{
  clear();
  printf("A general protection exception occured\n\n");
  print_error_page(stat);
  return;
}

/* Index 14 */
void pageFaultException(system_status stat)
{
  clear();
  printf("A page fault exception has occured\n\n");

  print_error_page(stat);
  return;
}

/* Index 16 */
void fpuFloatingPointError(system_status stat)
{
  clear();
  printf("An FPU floating point error has occured\n\n");
  print_error_page(stat);
  return;
}

/* Index 17 */
void alignmentCheckException(system_status stat)
{
  clear();
  printf("An alignment check exception has occured\n\n");
  print_error_page(stat);
  return;
}

/* Index 18 */
void machineCheckException(system_status stat)
{
  clear();
  printf("A machine check exception has occured\n\n");
  print_error_page(stat);
  return;
}

/* Index 19 */
void SIMDFloatingPointException(system_status stat)
{
  clear();
  printf("A SIMD floating point error has occured\n\n");
  print_error_page(stat);
  return;
}

/* exitException
*   DESCRIPTION: called when we get an exception, waits for shift-ctrl to be pressed before returning
*   INPUTS: none
*   OUTPUTS: none
*   RETURN VALUE: none
*   SIDE EFFECTS: waits
*/
void exitException()
{
    printf("\nPress Ctrl-Shift to continue.\n");
    sti();
    while(!(mod_keys[2] && mod_keys[1]));
    cli();
    clear();
    printf("Ruthlessly murduring the problematic program\n");
    return;
}
