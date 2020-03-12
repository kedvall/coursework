
#ifndef __COECSL_H__
#define __COECSL_H__



/* DSP/BIOS includes */
#ifdef DSP28_BIOS
#include <std.h>
#include <mem.h>
#include <sem.h>
#include <que.h>
#include <log.h>
#include <sys.h>
#include <tsk.h>
#include <hwi.h>
#include <swi.h>
#include <rtdx.h>
#include <clk.h>

/* non-DSP/BIOS includes */
#else
#define TRUE		1
#define FALSE		0
#endif


/* standard ANSI C includes */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <limits.h>

/* device includes */
#include <DSP2833x_Device.h>
#include <DSP2833x_GlobalPrototypes.h>

/* general constants */
#define PI			3.1415926535897932384626433832795
#define TWOPI		6.283185307179586476925286766559
#define HALFPI		1.5707963267948966192313216916398
#define GRAV		9.81


/* COECSL types and macros */
typedef int bool;

#define PLL					0xA   
#ifndef F28335_CONTROL_CARD
	#define OSCCLK_KHZ			30000L
#else
    #ifdef F28335_CONTROL_CARD30
        #define OSCCLK_KHZ          30000L
    #else
	    #define OSCCLK_KHZ			20000L
    #endif
#endif



#if PLL
#define SYSCLKOUT_KHZ		(OSCCLK_KHZ*PLL/2)
#else
#define SYSCLKOUT_KHZ		(OSCCLK_KHZ/2)
#endif

/* high speed clock (input to event managers, adc) */
#define HISPCLK_KHZ			(SYSCLKOUT_KHZ/((SysCtrlRegs.HISPCP.bit.HSPCLK == 0) ? 1 : (SysCtrlRegs.HISPCP.bit.HSPCLK*2)))
#define HISPCLK_HZ			(HISPCLK_KHZ*1000L)

/* low speed clock (input to serial ports, etc.) */
#define LSPCLK_KHZ			(SYSCLKOUT_KHZ/((SysCtrlRegs.LOSPCP.bit.LSPCLK == 0) ? 1 : (SysCtrlRegs.LOSPCP.bit.LSPCLK*2)))
#define LSPCLK_HZ			(LSPCLK_KHZ*1000L)

/* NOTE!  You can think of these clock frequencies as "ticks per second" (HZ) or
   "ticks per millisecond (KHZ). */


/* COECSL function prototypes */
void init_COECSL(void);
#define nop() asm(" nop")


/* general structures/enumerations */
enum eQep_e {
	EQEP1,
	EQEP2
};
enum gptimer_e {
	GPTIMER1,
	GPTIMER2,
	GPTIMER3,
	GPTIMER4
};
enum epwm {
	EPWM1,
	EPWM2,
	EPWM3,
	EPWM4,
	EPWM5,
	EPWM6,
	EPWM3B,
	EPWM4B
};

/* COECSL includes */
#include <queue.h>
#include <buffer.h>
#include <smallprintf.h>

#include <io.h>

#ifndef __283355_SERIAL_H__
	#include <28335_serial.h>
#endif


#include <28335_pwm.h>
#include <28335_eQep.h>

#endif /* __COECSL_H__ */

