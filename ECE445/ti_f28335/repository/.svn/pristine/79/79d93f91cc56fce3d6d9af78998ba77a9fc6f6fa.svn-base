
#include <coecsl.h>

#ifndef USEJUST_INTERNAL_MEM
// load/run variables declared in command file
extern Uint16 ramfuncs_loadstart;
extern Uint16 ramfuncs_loadend;
extern Uint16 ramfuncs_runstart;


#ifdef DSP28_BIOS
extern Uint16 trcdata_loadstart;
extern Uint16 trcdata_loadend;
extern Uint16 trcdata_runstart;

extern Uint16 hwi_vec_loadstart;
extern Uint16 hwi_vec_loadend;
extern Uint16 hwi_vec_runstart;
#endif
#endif

void MemCopy(Uint16 *SourceAddr, Uint16* SourceEndAddr, Uint16* DestAddr)
{
    while(SourceAddr < SourceEndAddr)
    { 
       *DestAddr++ = *SourceAddr++;
    }
    return;
}

/* init_COECSL(): initialize system defaults --
     DSP/BIOS: Called from "User Init Function" (Global Settings)
     non-BIOS: Called from main() */
void init_COECSL(void)
{
	Uint32 delay;

	// disable interrupts
	DINT;
	IER = 0x0000;
	IFR = 0x0000;
	
#ifndef USEJUST_INTERNAL_MEM
	// load code from flash memory into ram
	MemCopy(&ramfuncs_loadstart,&ramfuncs_loadend,&ramfuncs_runstart);
//	MemCopy(&extfuncs_loadstart,&extfuncs_loadend,&extfuncs_runstart);
#ifdef DSP28_BIOS
	MemCopy(&trcdata_loadstart,&trcdata_loadend,&trcdata_runstart);
	EALLOW;
	MemCopy(&hwi_vec_loadstart,&hwi_vec_loadend,&hwi_vec_runstart);
	EDIS;
#endif
#endif
	// initialize system (clocks, etc.)
	InitSysCtrl();
	InitPll(PLL,2);
	
	// Initialize flash memory for 3 wait states.  Programmers Note:  We modified TI's default function to 3 wait states
	InitFlash();  

	// power-up ADCs
	AdcRegs.ADCTRL3.bit.ADCBGRFDN = 0x3;
	for (delay = 1000000L; delay; delay--) nop();
	AdcRegs.ADCTRL3.bit.ADCPWDN = 1;
	for (delay = 1000L; delay; delay--) nop();

	// reconfigure clocks
	EALLOW;
	SysCtrlRegs.HISPCP.all = 0x0000;
	SysCtrlRegs.LOSPCP.all = 0x0000;
	EDIS;


	// initialize PIE vector tables (default interrupt functions)
#ifndef DSP28_BIOS
	InitPieCtrl();
	InitPieVectTable();
#endif
}

