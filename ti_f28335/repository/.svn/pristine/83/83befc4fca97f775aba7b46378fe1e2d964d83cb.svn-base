
#include <tistdtypes.h>
#include <coecsl.h>
#include "28335_inits.h"

int McBSPb_read1 = 0;
int McBSPb_read2 = 0;

void pre_init(void) {

	EALLOW;

		GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 0;
		GpioDataRegs.GPASET.bit.GPIO1 = 1; // initially disabled
//		GpioDataRegs.GPACLEAR.bit.GPIO1 = 1; // initially disabled
		GpioCtrlRegs.GPADIR.bit.GPIO1 = 1;

		GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 0;
		GpioDataRegs.GPASET.bit.GPIO3 = 1;  // initially disabled
//		GpioDataRegs.GPACLEAR.bit.GPIO3 = 1;  // initially disabled
		GpioCtrlRegs.GPADIR.bit.GPIO3 = 1;

	EDIS;

	InitPieCtrl();
	// Enable the PIE
	PieCtrlRegs.PIECTRL.bit.ENPIE = 1;

	// Enables PIE to drive a pulse into the CPU
	PieCtrlRegs.PIEACK.all = 0xFFFF;

	EALLOW;
	SysCtrlRegs.PCLKCR0.bit.I2CAENCLK = 1;   // I2C
	SysCtrlRegs.PCLKCR0.bit.SCIAENCLK = 1;   // SCI-A
	SysCtrlRegs.PCLKCR0.bit.SCIBENCLK = 1;   // SCI-B
	SysCtrlRegs.PCLKCR0.bit.SCICENCLK = 1;   // SCI-C
	SysCtrlRegs.PCLKCR0.bit.SPIAENCLK = 1;   // SPI-A
	SysCtrlRegs.PCLKCR0.bit.MCBSPAENCLK = 1; // McBSP-A
	SysCtrlRegs.PCLKCR0.bit.MCBSPBENCLK = 1; // McBSP-B
	SysCtrlRegs.PCLKCR0.bit.ECANAENCLK=0;    // eCAN-A
	SysCtrlRegs.PCLKCR0.bit.ECANBENCLK=0;    // eCAN-B

	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;   // Disable TBCLK within the ePWM
	SysCtrlRegs.PCLKCR1.bit.EPWM1ENCLK = 1;  // ePWM1
	SysCtrlRegs.PCLKCR1.bit.EPWM2ENCLK = 1;  // ePWM2
	SysCtrlRegs.PCLKCR1.bit.EPWM3ENCLK = 1;  // ePWM3
	SysCtrlRegs.PCLKCR1.bit.EPWM4ENCLK = 1;  // ePWM4
	SysCtrlRegs.PCLKCR1.bit.EPWM5ENCLK = 1;  // ePWM5
	SysCtrlRegs.PCLKCR1.bit.EPWM6ENCLK = 1;  // ePWM6
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;   // Enable TBCLK within the ePWM

	SysCtrlRegs.PCLKCR1.bit.ECAP3ENCLK = 1;  // eCAP3
	SysCtrlRegs.PCLKCR1.bit.ECAP4ENCLK = 1;  // eCAP4
	SysCtrlRegs.PCLKCR1.bit.ECAP5ENCLK = 1;  // eCAP5
	SysCtrlRegs.PCLKCR1.bit.ECAP6ENCLK = 1;  // eCAP6
	SysCtrlRegs.PCLKCR1.bit.ECAP1ENCLK = 1;  // eCAP1
	SysCtrlRegs.PCLKCR1.bit.ECAP2ENCLK = 1;  // eCAP2
	SysCtrlRegs.PCLKCR1.bit.EQEP1ENCLK = 1;  // eQEP1
	SysCtrlRegs.PCLKCR1.bit.EQEP2ENCLK = 1;  // eQEP2

	SysCtrlRegs.PCLKCR3.bit.CPUTIMER0ENCLK = 1; // CPU Timer 0
	SysCtrlRegs.PCLKCR3.bit.CPUTIMER1ENCLK = 1; // CPU Timer 1
	SysCtrlRegs.PCLKCR3.bit.CPUTIMER2ENCLK = 1; // CPU Timer 2

	SysCtrlRegs.PCLKCR3.bit.DMAENCLK = 1;       // DMA Clock
	SysCtrlRegs.PCLKCR3.bit.XINTFENCLK = 1;     // XTIMCLK
	SysCtrlRegs.PCLKCR3.bit.GPIOINENCLK = 1;    // GPIO input clock
	EDIS;
}

void post_init(void) {

	// read to clear Rrdy
//	McBSPb_read2 = McbspbRegs.DRR2.all;
//	McBSPb_read1 = McbspbRegs.DRR1.all;

	IFR &= ~M_INT6;  // Make sure no ints pending.
	IER |= M_INT6; // Enable CPU Interrupt E for SPI and later for McBSP


//	EALLOW;
//		DmaRegs.CH2.CONTROL.bit.RUN = 1;           // Start DMA Receive from McBSP-B
//	EDIS;
}
