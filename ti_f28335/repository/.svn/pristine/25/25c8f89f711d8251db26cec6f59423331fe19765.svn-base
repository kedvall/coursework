#include <coecsl.h>
#include <28335_pwm.h>

//#define DAN_PWM_CARRIER_HZ	80000L
//#define DAN_PWM_CARRIER_HZ	40000L  // The limit of the A3953 chip is 70Khz
#define DAN_PWM_CARRIER_HZ	20000L  // The limit of the A3953 chip is 70Khz

#define DAN_PWM_TPS			0x0
#define DAN_PWM_TCLK		(HISPCLK_HZ >> (DAN_PWM_TPS)) //HISPCLK is at 150 MHz
#define DAN_PWM_PR			(DAN_PWM_TCLK / DAN_PWM_CARRIER_HZ)
#define DAN_PWM_DUTY50		(DAN_PWM_PR >> 1)

int PWM3setupasRCservo = 0;
int PWM4setupasRCservo = 0;
int PWM5setupasRCservo = 0;
int dummy2 = 0;


void init_PWMandDIR(enum epwm ep)
{
	switch (ep) {


	case EPWM1:

		EALLOW;
		GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;
		GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 0;
		GpioCtrlRegs.GPADIR.bit.GPIO1 = 1;
		GpioDataRegs.GPACLEAR.bit.GPIO1 = 1;
		EDIS;
		dummy2 = DAN_PWM_PR;
		EPwm1Regs.TBPRD = DAN_PWM_PR;  // Set to Period for 20Khz carrier
		EPwm1Regs.TBPHS.half.TBPHS = 0;  // not using Phase
		EPwm1Regs.TBCTL.bit.FREE_SOFT = 2; // don't stop on emulation suspend
		EPwm1Regs.TBCTL.bit.CLKDIV = 0; // Base clk set to SYSCLK
		EPwm1Regs.TBCTL.bit.HSPCLKDIV = 0; // Base clk set to SYSCLK
		EPwm1Regs.TBCTL.bit.SYNCOSEL = 3; //  SYNCO disabled
		EPwm1Regs.TBCTL.bit.PRDLD = 0; // Use Shadow
		EPwm1Regs.TBCTL.bit.PHSEN = 0; // Phase disabled
		EPwm1Regs.TBCTL.bit.CTRMODE = 0; // Count up

		EPwm1Regs.CMPA.half.CMPA = 0;  // Start off zero percent duty cycle
		EPwm1Regs.CMPCTL.bit.LOADAMODE = 1; // Load on CTR=PRD
		EPwm1Regs.AQCTLA.bit.CAU = 1; // Clear pin when CTR=CMPA
		EPwm1Regs.AQCTLA.bit.PRD = 2; // Set pin when CTR=PRD

		break;

	case EPWM2:

		EALLOW;
		GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1;
		GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 0;
		GpioCtrlRegs.GPADIR.bit.GPIO3 = 1;
		GpioDataRegs.GPACLEAR.bit.GPIO3 = 1;
		EDIS;

		EPwm2Regs.TBPRD = DAN_PWM_PR;  // Set to Period for 20Khz carrier
		EPwm2Regs.TBPHS.half.TBPHS = 0;  // not using Phase
		EPwm2Regs.TBCTL.bit.FREE_SOFT = 2; // don't stop on emulation suspend
		EPwm2Regs.TBCTL.bit.CLKDIV = 0; // Base clk set to SYSCLK
		EPwm2Regs.TBCTL.bit.HSPCLKDIV = 0; // Base clk set to SYSCLK
		EPwm2Regs.TBCTL.bit.SYNCOSEL = 3; //  SYNCO disabled
		EPwm2Regs.TBCTL.bit.PRDLD = 0; // Use Shadow
		EPwm2Regs.TBCTL.bit.PHSEN = 0; // Phase disabled
		EPwm2Regs.TBCTL.bit.CTRMODE = 0; // Count up

		EPwm2Regs.CMPA.half.CMPA = 0;  // Start off with a square wave
		EPwm2Regs.CMPCTL.bit.LOADAMODE = 1; // Load on CTR=PRD
		EPwm2Regs.AQCTLA.bit.CAU = 1; // Clear pin when CTR=CMPA
		EPwm2Regs.AQCTLA.bit.PRD = 2; // Set pin when CTR=PRD

		break;

	case EPWM3:

		EALLOW;
		GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 1;
		GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 0;
		GpioCtrlRegs.GPADIR.bit.GPIO5 = 1;
		GpioDataRegs.GPACLEAR.bit.GPIO5 = 1;
		EDIS;

		EPwm3Regs.TBPRD = DAN_PWM_PR;  // Set to Period for 20Khz carrier
		EPwm3Regs.TBPHS.half.TBPHS = 0;  // not using Phase
		EPwm3Regs.TBCTL.bit.FREE_SOFT = 2; // don't stop on emulation suspend
		EPwm3Regs.TBCTL.bit.CLKDIV = 0; // Base clk set to SYSCLK
		EPwm3Regs.TBCTL.bit.HSPCLKDIV = 0; // Base clk set to SYSCLK
		EPwm3Regs.TBCTL.bit.SYNCOSEL = 3; //  SYNCO disabled
		EPwm3Regs.TBCTL.bit.PRDLD = 0; // Use Shadow
		EPwm3Regs.TBCTL.bit.PHSEN = 0; // Phase disabled
		EPwm3Regs.TBCTL.bit.CTRMODE = 0; // Count up

		EPwm3Regs.CMPA.half.CMPA = 0;  // Start off with a square wave
		EPwm3Regs.CMPCTL.bit.LOADAMODE = 1; // Load on CTR=PRD
		EPwm3Regs.AQCTLA.bit.CAU = 1; // Clear pin when CTR=CMPA
		EPwm3Regs.AQCTLA.bit.PRD = 2; // Set pin when CTR=PRD

		PWM3setupasRCservo = 0;
		break;

	case EPWM4:

		EALLOW;
		GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 1;
		GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 0;
		GpioCtrlRegs.GPADIR.bit.GPIO7 = 1;
		GpioDataRegs.GPACLEAR.bit.GPIO7 = 1;
		EDIS;

		EPwm4Regs.TBPRD = DAN_PWM_PR;  // Set to Period for 20Khz carrier
		EPwm4Regs.TBPHS.half.TBPHS = 0;  // not using Phase
		EPwm4Regs.TBCTL.bit.FREE_SOFT = 2; // don't stop on emulation suspend
		EPwm4Regs.TBCTL.bit.CLKDIV = 0; // Base clk set to SYSCLK
		EPwm4Regs.TBCTL.bit.HSPCLKDIV = 0; // Base clk set to SYSCLK
		EPwm4Regs.TBCTL.bit.SYNCOSEL = 3; //  SYNCO disabled
		EPwm4Regs.TBCTL.bit.PRDLD = 0; // Use Shadow
		EPwm4Regs.TBCTL.bit.PHSEN = 0; // Phase disabled
		EPwm4Regs.TBCTL.bit.CTRMODE = 0; // Count up

		EPwm4Regs.CMPA.half.CMPA = 0;  // Start off with a square wave
		EPwm4Regs.CMPCTL.bit.LOADAMODE = 1; // Load on CTR=PRD
		EPwm4Regs.AQCTLA.bit.CAU = 1; // Clear pin when CTR=CMPA
		EPwm4Regs.AQCTLA.bit.PRD = 2; // Set pin when CTR=PRD

		PWM4setupasRCservo = 0;
		break;

	case EPWM5:

		EALLOW;
		GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 1;
		GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 0;
		GpioCtrlRegs.GPADIR.bit.GPIO19 = 1;
		GpioDataRegs.GPACLEAR.bit.GPIO19 = 1;
		EDIS;

		EPwm5Regs.TBPRD = DAN_PWM_PR;  // Set to Period for 20Khz carrier
		EPwm5Regs.TBPHS.half.TBPHS = 0;  // not using Phase
		EPwm5Regs.TBCTL.bit.FREE_SOFT = 2; // don't stop on emulation suspend
		EPwm5Regs.TBCTL.bit.CLKDIV = 0; // Base clk set to SYSCLK
		EPwm5Regs.TBCTL.bit.HSPCLKDIV = 0; // Base clk set to SYSCLK
		EPwm5Regs.TBCTL.bit.SYNCOSEL = 3; //  SYNCO disabled
		EPwm5Regs.TBCTL.bit.PRDLD = 0; // Use Shadow
		EPwm5Regs.TBCTL.bit.PHSEN = 0; // Phase disabled
		EPwm5Regs.TBCTL.bit.CTRMODE = 0; // Count up

		EPwm5Regs.CMPA.half.CMPA = 0;  // Start off with a square wave
		EPwm5Regs.CMPCTL.bit.LOADAMODE = 1; // Load on CTR=PRD
		EPwm5Regs.AQCTLA.bit.CAU = 1; // Clear pin when CTR=CMPA
		EPwm5Regs.AQCTLA.bit.PRD = 2; // Set pin when CTR=PRD

		PWM5setupasRCservo = 0;
		break;

	case EPWM6:

//		EALLOW;
//		GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 1;
//		EDIS;
//
//		EPwm6Regs.TBPRD = DAN_PWM_PR;  // Set to Period for 20Khz carrier
//		EPwm6Regs.TBPHS.half.TBPHS = 0;  // not using Phase
//		EPwm6Regs.TBCTL.bit.FREE_SOFT = 2; // don't stop on emulation suspend
//		EPwm6Regs.TBCTL.bit.CLKDIV = 0; // Base clk set to SYSCLK
//		EPwm6Regs.TBCTL.bit.HSPCLKDIV = 0; // Base clk set to SYSCLK
//		EPwm6Regs.TBCTL.bit.SYNCOSEL = 3; //  SYNCO disabled
//		EPwm6Regs.TBCTL.bit.PRDLD = 0; // Use Shadow
//		EPwm6Regs.TBCTL.bit.PHSEN = 0; // Phase disabled
//		EPwm6Regs.TBCTL.bit.CTRMODE = 0; // Count up
//
//		EPwm6Regs.CMPA.half.CMPA = DAN_PWM_DUTY50;  // Start off with a square wave
//		EPwm6Regs.CMPCTL.bit.LOADAMODE = 1; // Load on CTR=PRD
//		EPwm6Regs.AQCTLA.bit.CAU = 1; // Clear pin when CTR=CMPA
//		EPwm6Regs.AQCTLA.bit.PRD = 2; // Set pin when CTR=PRD

		break;

	}

}


void init_PWM(enum epwm ep)
{
	switch (ep) {


	case EPWM1:
		
		EALLOW;
		GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;
		EDIS;

		EPwm1Regs.TBPRD = DAN_PWM_PR;  // Set to Period for 20Khz carrier
		EPwm1Regs.TBPHS.half.TBPHS = 0;  // not using Phase
		EPwm1Regs.TBCTL.bit.FREE_SOFT = 2; // don't stop on emulation suspend
		EPwm1Regs.TBCTL.bit.CLKDIV = 0; // Base clk set to SYSCLK
		EPwm1Regs.TBCTL.bit.HSPCLKDIV = 0; // Base clk set to SYSCLK
		EPwm1Regs.TBCTL.bit.SYNCOSEL = 3; //  SYNCO disabled
		EPwm1Regs.TBCTL.bit.PRDLD = 0; // Use Shadow
		EPwm1Regs.TBCTL.bit.PHSEN = 0; // Phase disabled
		EPwm1Regs.TBCTL.bit.CTRMODE = 0; // Count up

		EPwm1Regs.CMPA.half.CMPA = DAN_PWM_DUTY50;  // Start off with a square wave
		EPwm1Regs.CMPCTL.bit.LOADAMODE = 1; // Load on CTR=PRD
		EPwm1Regs.AQCTLA.bit.CAU = 1; // Clear pin when CTR=CMPA 
		EPwm1Regs.AQCTLA.bit.PRD = 2; // Set pin when CTR=PRD

		break;

	case EPWM2:
		
		EALLOW;
		GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1;
		EDIS;

		EPwm2Regs.TBPRD = DAN_PWM_PR;  // Set to Period for 20Khz carrier
		EPwm2Regs.TBPHS.half.TBPHS = 0;  // not using Phase
		EPwm2Regs.TBCTL.bit.FREE_SOFT = 2; // don't stop on emulation suspend
		EPwm2Regs.TBCTL.bit.CLKDIV = 0; // Base clk set to SYSCLK
		EPwm2Regs.TBCTL.bit.HSPCLKDIV = 0; // Base clk set to SYSCLK
		EPwm2Regs.TBCTL.bit.SYNCOSEL = 3; //  SYNCO disabled
		EPwm2Regs.TBCTL.bit.PRDLD = 0; // Use Shadow
		EPwm2Regs.TBCTL.bit.PHSEN = 0; // Phase disabled
		EPwm2Regs.TBCTL.bit.CTRMODE = 0; // Count up

		EPwm2Regs.CMPA.half.CMPA = DAN_PWM_DUTY50;  // Start off with a square wave
		EPwm2Regs.CMPCTL.bit.LOADAMODE = 1; // Load on CTR=PRD
		EPwm2Regs.AQCTLA.bit.CAU = 1; // Clear pin when CTR=CMPA 
		EPwm2Regs.AQCTLA.bit.PRD = 2; // Set pin when CTR=PRD

		break;

	case EPWM3:
		
		EALLOW;
		GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 1;
		EDIS;

		EPwm3Regs.TBPRD = DAN_PWM_PR;  // Set to Period for 20Khz carrier
		EPwm3Regs.TBPHS.half.TBPHS = 0;  // not using Phase
		EPwm3Regs.TBCTL.bit.FREE_SOFT = 2; // don't stop on emulation suspend
		EPwm3Regs.TBCTL.bit.CLKDIV = 0; // Base clk set to SYSCLK
		EPwm3Regs.TBCTL.bit.HSPCLKDIV = 0; // Base clk set to SYSCLK
		EPwm3Regs.TBCTL.bit.SYNCOSEL = 3; //  SYNCO disabled
		EPwm3Regs.TBCTL.bit.PRDLD = 0; // Use Shadow
		EPwm3Regs.TBCTL.bit.PHSEN = 0; // Phase disabled
		EPwm3Regs.TBCTL.bit.CTRMODE = 0; // Count up

		EPwm3Regs.CMPA.half.CMPA = DAN_PWM_DUTY50;  // Start off with a square wave
		EPwm3Regs.CMPCTL.bit.LOADAMODE = 1; // Load on CTR=PRD
		EPwm3Regs.AQCTLA.bit.CAU = 1; // Clear pin when CTR=CMPA 
		EPwm3Regs.AQCTLA.bit.PRD = 2; // Set pin when CTR=PRD

		PWM3setupasRCservo = 0;
		break;

	case EPWM4:
		
		EALLOW;
		GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 1;
		EDIS;

		EPwm4Regs.TBPRD = DAN_PWM_PR;  // Set to Period for 20Khz carrier
		EPwm4Regs.TBPHS.half.TBPHS = 0;  // not using Phase
		EPwm4Regs.TBCTL.bit.FREE_SOFT = 2; // don't stop on emulation suspend
		EPwm4Regs.TBCTL.bit.CLKDIV = 0; // Base clk set to SYSCLK
		EPwm4Regs.TBCTL.bit.HSPCLKDIV = 0; // Base clk set to SYSCLK
		EPwm4Regs.TBCTL.bit.SYNCOSEL = 3; //  SYNCO disabled
		EPwm4Regs.TBCTL.bit.PRDLD = 0; // Use Shadow
		EPwm4Regs.TBCTL.bit.PHSEN = 0; // Phase disabled
		EPwm4Regs.TBCTL.bit.CTRMODE = 0; // Count up

		EPwm4Regs.CMPA.half.CMPA = DAN_PWM_DUTY50;  // Start off with a square wave
		EPwm4Regs.CMPCTL.bit.LOADAMODE = 1; // Load on CTR=PRD
		EPwm4Regs.AQCTLA.bit.CAU = 1; // Clear pin when CTR=CMPA 
		EPwm4Regs.AQCTLA.bit.PRD = 2; // Set pin when CTR=PRD

		PWM4setupasRCservo = 0;
		break;

	case EPWM5:
		
		EALLOW;
		GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 1;
		EDIS;

		EPwm5Regs.TBPRD = DAN_PWM_PR;  // Set to Period for 20Khz carrier
		EPwm5Regs.TBPHS.half.TBPHS = 0;  // not using Phase
		EPwm5Regs.TBCTL.bit.FREE_SOFT = 2; // don't stop on emulation suspend
		EPwm5Regs.TBCTL.bit.CLKDIV = 0; // Base clk set to SYSCLK
		EPwm5Regs.TBCTL.bit.HSPCLKDIV = 0; // Base clk set to SYSCLK
		EPwm5Regs.TBCTL.bit.SYNCOSEL = 3; //  SYNCO disabled
		EPwm5Regs.TBCTL.bit.PRDLD = 0; // Use Shadow
		EPwm5Regs.TBCTL.bit.PHSEN = 0; // Phase disabled
		EPwm5Regs.TBCTL.bit.CTRMODE = 0; // Count up

		EPwm5Regs.CMPA.half.CMPA = DAN_PWM_DUTY50;  // Start off with a square wave
		EPwm5Regs.CMPCTL.bit.LOADAMODE = 1; // Load on CTR=PRD
		EPwm5Regs.AQCTLA.bit.CAU = 1; // Clear pin when CTR=CMPA 
		EPwm5Regs.AQCTLA.bit.PRD = 2; // Set pin when CTR=PRD

		PWM5setupasRCservo = 0;

		break;

	case EPWM6:
		
		EALLOW;
		GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 1;
		EDIS;

		EPwm6Regs.TBPRD = DAN_PWM_PR;  // Set to Period for 20Khz carrier
		EPwm6Regs.TBPHS.half.TBPHS = 0;  // not using Phase
		EPwm6Regs.TBCTL.bit.FREE_SOFT = 2; // don't stop on emulation suspend
		EPwm6Regs.TBCTL.bit.CLKDIV = 0; // Base clk set to SYSCLK
		EPwm6Regs.TBCTL.bit.HSPCLKDIV = 0; // Base clk set to SYSCLK
		EPwm6Regs.TBCTL.bit.SYNCOSEL = 3; //  SYNCO disabled
		EPwm6Regs.TBCTL.bit.PRDLD = 0; // Use Shadow
		EPwm6Regs.TBCTL.bit.PHSEN = 0; // Phase disabled
		EPwm6Regs.TBCTL.bit.CTRMODE = 0; // Count up

		EPwm6Regs.CMPA.half.CMPA = DAN_PWM_DUTY50;  // Start off with a square wave
		EPwm6Regs.CMPCTL.bit.LOADAMODE = 1; // Load on CTR=PRD
		EPwm6Regs.AQCTLA.bit.CAU = 1; // Clear pin when CTR=CMPA 
		EPwm6Regs.AQCTLA.bit.PRD = 2; // Set pin when CTR=PRD

		break;

	}
		
}



void init_PWM_AS_RCSERVO(enum epwm ep)
{
	switch (ep) {


	case EPWM1:
		
		EALLOW;
		GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;
		EDIS;

		EPwm1Regs.TBPRD = 46875;  // Set to Period 50Hz  .02/((1/150000000)*64)
		EPwm1Regs.TBPHS.half.TBPHS = 0;  // not using Phase
		EPwm1Regs.TBCTL.bit.FREE_SOFT = 2; // don't stop on emulation suspend
		EPwm1Regs.TBCTL.bit.CLKDIV = 6; // Base clk set to SYSCLK/64
		EPwm1Regs.TBCTL.bit.HSPCLKDIV = 0; // Base clk remains at SYSCLK/64
		EPwm1Regs.TBCTL.bit.SYNCOSEL = 3; //  SYNCO disabled
		EPwm1Regs.TBCTL.bit.PRDLD = 0; // Use Shadow
		EPwm1Regs.TBCTL.bit.PHSEN = 0; // Phase disabled
		EPwm1Regs.TBCTL.bit.CTRMODE = 0; // Count up

		EPwm1Regs.CMPA.half.CMPA = 1875;  // Start off with 0.8 ms pulse .0008/((1/150000000)*64)
		EPwm1Regs.CMPCTL.bit.LOADAMODE = 1; // Load on CTR=PRD
		EPwm1Regs.AQCTLA.bit.CAU = 1; // Clear pin when CTR=CMPA 
		EPwm1Regs.AQCTLA.bit.PRD = 2; // Set pin when CTR=PRD

		break;

	case EPWM2:

				
		EALLOW;
		GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1;
		EDIS;

		EPwm2Regs.TBPRD = 46875;  // Set to Period 50Hz  .02/((1/150000000)*64)
		EPwm2Regs.TBPHS.half.TBPHS = 0;  // not using Phase
		EPwm2Regs.TBCTL.bit.FREE_SOFT = 2; // don't stop on emulation suspend
		EPwm2Regs.TBCTL.bit.CLKDIV = 6; // Base clk set to SYSCLK/64
		EPwm2Regs.TBCTL.bit.HSPCLKDIV = 0; // Base clk remains at SYSCLK/64
		EPwm2Regs.TBCTL.bit.SYNCOSEL = 3; //  SYNCO disabled
		EPwm2Regs.TBCTL.bit.PRDLD = 0; // Use Shadow
		EPwm2Regs.TBCTL.bit.PHSEN = 0; // Phase disabled
		EPwm2Regs.TBCTL.bit.CTRMODE = 0; // Count up

		EPwm2Regs.CMPA.half.CMPA = 1875;  // Start off with 0.8 ms pulse .0008/((1/150000000)*64)
		EPwm2Regs.CMPCTL.bit.LOADAMODE = 1; // Load on CTR=PRD
		EPwm2Regs.AQCTLA.bit.CAU = 1; // Clear pin when CTR=CMPA 
		EPwm2Regs.AQCTLA.bit.PRD = 2; // Set pin when CTR=PRD

		break;

	case EPWM3B:  // Sets up both PWM3A and PWM3B
		EALLOW;
		GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 1;
		EDIS;

		EPwm3Regs.CMPB = DAN_PWM_DUTY50;
		EPwm3Regs.AQCTLB.bit.CBU = 1;// Clear pin when CTR=CMPB
		EPwm3Regs.AQCTLB.bit.PRD = 2; // Set pin when CTR=PRD
		// No break here so both EPWM3A and EPWM3B are initialized
	case EPWM3:
		
		EALLOW;
		GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 1;
		EDIS;

		EPwm3Regs.TBPRD = 46875;  // Set to Period 50Hz  .02/((1/150000000)*64)
		EPwm3Regs.TBPHS.half.TBPHS = 0;  // not using Phase
		EPwm3Regs.TBCTL.bit.FREE_SOFT = 2; // don't stop on emulation suspend
		EPwm3Regs.TBCTL.bit.CLKDIV = 6; // Base clk set to SYSCLK/64
		EPwm3Regs.TBCTL.bit.HSPCLKDIV = 0; // Base clk remains at SYSCLK/64
		EPwm3Regs.TBCTL.bit.SYNCOSEL = 3; //  SYNCO disabled
		EPwm3Regs.TBCTL.bit.PRDLD = 0; // Use Shadow
		EPwm3Regs.TBCTL.bit.PHSEN = 0; // Phase disabled
		EPwm3Regs.TBCTL.bit.CTRMODE = 0; // Count up

		EPwm3Regs.CMPA.half.CMPA = 1875;  // Start off with 0.8 ms pulse .0008/((1/150000000)*64)
		EPwm3Regs.CMPCTL.bit.LOADAMODE = 1; // Load on CTR=PRD
		EPwm3Regs.AQCTLA.bit.CAU = 1; // Clear pin when CTR=CMPA 
		EPwm3Regs.AQCTLA.bit.PRD = 2; // Set pin when CTR=PRD

		PWM3setupasRCservo = 1;
		break;

	case EPWM4B:  // Sets up both PWM3A and PWM3B
		EALLOW;
		GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 1;
		EDIS;

		EPwm4Regs.CMPB = DAN_PWM_DUTY50;
		EPwm4Regs.AQCTLB.bit.CBU = 1;// Clear pin when CTR=CMPB
		EPwm4Regs.AQCTLB.bit.PRD = 2; // Set pin when CTR=PRD
		// No break here so both EPWM4A and EPWM4B are initialized

	case EPWM4:
		
		EALLOW;
		GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 1;
		EDIS;

		EPwm4Regs.TBPRD = 46875;  // Set to Period 50Hz  .02/((1/150000000)*64)
		EPwm4Regs.TBPHS.half.TBPHS = 0;  // not using Phase
		EPwm4Regs.TBCTL.bit.FREE_SOFT = 2; // don't stop on emulation suspend
		EPwm4Regs.TBCTL.bit.CLKDIV = 6; // Base clk set to SYSCLK/64
		EPwm4Regs.TBCTL.bit.HSPCLKDIV = 0; // Base clk remains at SYSCLK/64
		EPwm4Regs.TBCTL.bit.SYNCOSEL = 3; //  SYNCO disabled
		EPwm4Regs.TBCTL.bit.PRDLD = 0; // Use Shadow
		EPwm4Regs.TBCTL.bit.PHSEN = 0; // Phase disabled
		EPwm4Regs.TBCTL.bit.CTRMODE = 0; // Count up

		EPwm4Regs.CMPA.half.CMPA = 1875;  // Start off with 0.8 ms pulse .0008/((1/150000000)*64)
		EPwm4Regs.CMPCTL.bit.LOADAMODE = 1; // Load on CTR=PRD
		EPwm4Regs.AQCTLA.bit.CAU = 1; // Clear pin when CTR=CMPA 
		EPwm4Regs.AQCTLA.bit.PRD = 2; // Set pin when CTR=PRD

		PWM4setupasRCservo = 1;

		break;

	case EPWM5:
		
		EALLOW;
		GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 1;
		EDIS;

		EPwm5Regs.TBPRD = 46875;  // Set to Period 50Hz  .02/((1/150000000)*64)
		EPwm5Regs.TBPHS.half.TBPHS = 0;  // not using Phase
		EPwm5Regs.TBCTL.bit.FREE_SOFT = 2; // don't stop on emulation suspend
		EPwm5Regs.TBCTL.bit.CLKDIV = 6; // Base clk set to SYSCLK/64
		EPwm5Regs.TBCTL.bit.HSPCLKDIV = 0; // Base clk remains at SYSCLK/64
		EPwm5Regs.TBCTL.bit.SYNCOSEL = 3; //  SYNCO disabled
		EPwm5Regs.TBCTL.bit.PRDLD = 0; // Use Shadow
		EPwm5Regs.TBCTL.bit.PHSEN = 0; // Phase disabled
		EPwm5Regs.TBCTL.bit.CTRMODE = 0; // Count up

		EPwm5Regs.CMPA.half.CMPA = 1875;  // Start off with 0.8 ms pulse .0008/((1/150000000)*64)
		EPwm5Regs.CMPCTL.bit.LOADAMODE = 1; // Load on CTR=PRD
		EPwm5Regs.AQCTLA.bit.CAU = 1; // Clear pin when CTR=CMPA 
		EPwm5Regs.AQCTLA.bit.PRD = 2; // Set pin when CTR=PRD

		PWM5setupasRCservo = 1;

		break;

	case EPWM6:
		
		EALLOW;
		GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 1;
		EDIS;

		EPwm6Regs.TBPRD = 46875;  // Set to Period 50Hz  .02/((1/150000000)*64)
		EPwm6Regs.TBPHS.half.TBPHS = 0;  // not using Phase
		EPwm6Regs.TBCTL.bit.FREE_SOFT = 2; // don't stop on emulation suspend
		EPwm6Regs.TBCTL.bit.CLKDIV = 6; // Base clk set to SYSCLK/64
		EPwm6Regs.TBCTL.bit.HSPCLKDIV = 0; // Base clk remains at SYSCLK/64
		EPwm6Regs.TBCTL.bit.SYNCOSEL = 3; //  SYNCO disabled
		EPwm6Regs.TBCTL.bit.PRDLD = 0; // Use Shadow
		EPwm6Regs.TBCTL.bit.PHSEN = 0; // Phase disabled
		EPwm6Regs.TBCTL.bit.CTRMODE = 0; // Count up

		EPwm6Regs.CMPA.half.CMPA = 1875;  // Start off with 0.8 ms pulse .0008/((1/150000000)*64)
		EPwm6Regs.CMPCTL.bit.LOADAMODE = 1; // Load on CTR=PRD
		EPwm6Regs.AQCTLA.bit.CAU = 1; // Clear pin when CTR=CMPA 
		EPwm6Regs.AQCTLA.bit.PRD = 2; // Set pin when CTR=PRD

		break;

	}
		
}

void init_dualPWM(enum epwm ep)
{
	switch (ep) {


	case EPWM1:
		EALLOW;
		GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 0;
		GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 0;
		GpioDataRegs.GPACLEAR.bit.GPIO0 = 0;
		GpioDataRegs.GPACLEAR.bit.GPIO1 = 0;
		GpioCtrlRegs.GPADIR.bit.GPIO0 = 1;
		GpioCtrlRegs.GPADIR.bit.GPIO1 = 1;
		EDIS;
		

		EPwm1Regs.TBPRD = DAN_PWM_PR;  // Set to Period for 20Khz carrier
		EPwm1Regs.TBPHS.half.TBPHS = 0;  // not using Phase
		EPwm1Regs.TBCTL.bit.FREE_SOFT = 2; // don't stop on emulation suspend
		EPwm1Regs.TBCTL.bit.CLKDIV = 0; // Base clk set to SYSCLK
		EPwm1Regs.TBCTL.bit.HSPCLKDIV = 0; // Base clk set to SYSCLK
		EPwm1Regs.TBCTL.bit.SYNCOSEL = 3; //  SYNCO disabled
		EPwm1Regs.TBCTL.bit.PRDLD = 0; // Use Shadow
		EPwm1Regs.TBCTL.bit.PHSEN = 0; // Phase disabled
		EPwm1Regs.TBCTL.bit.CTRMODE = 0; // Count up

		EPwm1Regs.CMPA.half.CMPA = 0;  // Both signals zero
		EPwm1Regs.CMPB = 0;
		EPwm1Regs.CMPCTL.bit.LOADAMODE = 1; // Load on CTR=PRD
		EPwm1Regs.CMPCTL.bit.LOADBMODE = 1; // Load on CTR=PRD
		EPwm1Regs.AQCTLA.bit.CAU = 1; // Clear pin when CTR=CMPA 
		EPwm1Regs.AQCTLA.bit.PRD = 2; // Set pin when CTR=PRD
		EPwm1Regs.AQCTLB.bit.CBU = 1; // Clear EPWM1B pin when CTR=CMPB
		EPwm1Regs.AQCTLB.bit.PRD = 2; // Set EPWM1B pin when CTR=PRD

		EALLOW;
		GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;
		GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 1;
		EDIS;

		break;

/*	case EPWM2:
		
		EALLOW;
		GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1;
		EDIS;

		EPwm2Regs.TBPRD = PWM_PR;  // Set to Period for 20Khz carrier
		EPwm2Regs.TBPHS.half.TBPHS = 0;  // not using Phase
		EPwm2Regs.TBCTL.bit.FREE_SOFT = 2; // don't stop on emulation suspend
		EPwm2Regs.TBCTL.bit.CLKDIV = 0; // Base clk set to SYSCLK
		EPwm2Regs.TBCTL.bit.HSPCLKDIV = 0; // Base clk set to SYSCLK
		EPwm2Regs.TBCTL.bit.SYNCOSEL = 3; //  SYNCO disabled
		EPwm2Regs.TBCTL.bit.PRDLD = 0; // Use Shadow
		EPwm2Regs.TBCTL.bit.PHSEN = 0; // Phase disabled
		EPwm2Regs.TBCTL.bit.CTRMODE = 0; // Count up

		EPwm2Regs.CMPA.half.CMPA = PWM_DUTY50;  // Start off with a square wave
		EPwm2Regs.CMPCTL.bit.LOADAMODE = 1; // Load on CTR=PRD
		EPwm2Regs.AQCTLA.bit.CAU = 1; // Clear pin when CTR=CMPA 
		EPwm2Regs.AQCTLA.bit.PRD = 2; // Set pin when CTR=PRD

		break;

	case EPWM3:
		
		EALLOW;
		GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 1;
		EDIS;

		EPwm3Regs.TBPRD = PWM_PR;  // Set to Period for 20Khz carrier
		EPwm3Regs.TBPHS.half.TBPHS = 0;  // not using Phase
		EPwm3Regs.TBCTL.bit.FREE_SOFT = 2; // don't stop on emulation suspend
		EPwm3Regs.TBCTL.bit.CLKDIV = 0; // Base clk set to SYSCLK
		EPwm3Regs.TBCTL.bit.HSPCLKDIV = 0; // Base clk set to SYSCLK
		EPwm3Regs.TBCTL.bit.SYNCOSEL = 3; //  SYNCO disabled
		EPwm3Regs.TBCTL.bit.PRDLD = 0; // Use Shadow
		EPwm3Regs.TBCTL.bit.PHSEN = 0; // Phase disabled
		EPwm3Regs.TBCTL.bit.CTRMODE = 0; // Count up

		EPwm3Regs.CMPA.half.CMPA = PWM_DUTY50;  // Start off with a square wave
		EPwm3Regs.CMPCTL.bit.LOADAMODE = 1; // Load on CTR=PRD
		EPwm3Regs.AQCTLA.bit.CAU = 1; // Clear pin when CTR=CMPA 
		EPwm3Regs.AQCTLA.bit.PRD = 2; // Set pin when CTR=PRD

		break;

	case EPWM4:
		
		EALLOW;
		GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 1;
		EDIS;

		EPwm4Regs.TBPRD = PWM_PR;  // Set to Period for 20Khz carrier
		EPwm4Regs.TBPHS.half.TBPHS = 0;  // not using Phase
		EPwm4Regs.TBCTL.bit.FREE_SOFT = 2; // don't stop on emulation suspend
		EPwm4Regs.TBCTL.bit.CLKDIV = 0; // Base clk set to SYSCLK
		EPwm4Regs.TBCTL.bit.HSPCLKDIV = 0; // Base clk set to SYSCLK
		EPwm4Regs.TBCTL.bit.SYNCOSEL = 3; //  SYNCO disabled
		EPwm4Regs.TBCTL.bit.PRDLD = 0; // Use Shadow
		EPwm4Regs.TBCTL.bit.PHSEN = 0; // Phase disabled
		EPwm4Regs.TBCTL.bit.CTRMODE = 0; // Count up

		EPwm4Regs.CMPA.half.CMPA = PWM_DUTY50;  // Start off with a square wave
		EPwm4Regs.CMPCTL.bit.LOADAMODE = 1; // Load on CTR=PRD
		EPwm4Regs.AQCTLA.bit.CAU = 1; // Clear pin when CTR=CMPA 
		EPwm4Regs.AQCTLA.bit.PRD = 2; // Set pin when CTR=PRD

		break;

	case EPWM5:
		
		EALLOW;
		GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 1;
		EDIS;

		EPwm5Regs.TBPRD = PWM_PR;  // Set to Period for 20Khz carrier
		EPwm5Regs.TBPHS.half.TBPHS = 0;  // not using Phase
		EPwm5Regs.TBCTL.bit.FREE_SOFT = 2; // don't stop on emulation suspend
		EPwm5Regs.TBCTL.bit.CLKDIV = 0; // Base clk set to SYSCLK
		EPwm5Regs.TBCTL.bit.HSPCLKDIV = 0; // Base clk set to SYSCLK
		EPwm5Regs.TBCTL.bit.SYNCOSEL = 3; //  SYNCO disabled
		EPwm5Regs.TBCTL.bit.PRDLD = 0; // Use Shadow
		EPwm5Regs.TBCTL.bit.PHSEN = 0; // Phase disabled
		EPwm5Regs.TBCTL.bit.CTRMODE = 0; // Count up

		EPwm5Regs.CMPA.half.CMPA = PWM_DUTY50;  // Start off with a square wave
		EPwm5Regs.CMPCTL.bit.LOADAMODE = 1; // Load on CTR=PRD
		EPwm5Regs.AQCTLA.bit.CAU = 1; // Clear pin when CTR=CMPA 
		EPwm5Regs.AQCTLA.bit.PRD = 2; // Set pin when CTR=PRD

		break;

	case EPWM6:
		
		EALLOW;
		GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 1;
		EDIS;

		EPwm6Regs.TBPRD = PWM_PR;  // Set to Period for 20Khz carrier
		EPwm6Regs.TBPHS.half.TBPHS = 0;  // not using Phase
		EPwm6Regs.TBCTL.bit.FREE_SOFT = 2; // don't stop on emulation suspend
		EPwm6Regs.TBCTL.bit.CLKDIV = 0; // Base clk set to SYSCLK
		EPwm6Regs.TBCTL.bit.HSPCLKDIV = 0; // Base clk set to SYSCLK
		EPwm6Regs.TBCTL.bit.SYNCOSEL = 3; //  SYNCO disabled
		EPwm6Regs.TBCTL.bit.PRDLD = 0; // Use Shadow
		EPwm6Regs.TBCTL.bit.PHSEN = 0; // Phase disabled
		EPwm6Regs.TBCTL.bit.CTRMODE = 0; // Count up

		EPwm6Regs.CMPA.half.CMPA = PWM_DUTY50;  // Start off with a square wave
		EPwm6Regs.CMPCTL.bit.LOADAMODE = 1; // Load on CTR=PRD
		EPwm6Regs.AQCTLA.bit.CAU = 1; // Clear pin when CTR=CMPA 
		EPwm6Regs.AQCTLA.bit.PRD = 2; // Set pin when CTR=PRD

		break;
*/
	}
		
}


/* PWM_out(): update PWM's duty cycle -10.0<u<10.0
 *
 * parameters:
 *     pwm = PWM to update
 *     u   = duty cycle (-10 < u < 10)
 */
void PWM_out(enum epwm ep, float u)
{
	unsigned int val;

	if (u > 10.0F) {
		u = 10.0F;
	} else if (u < -10.0F) {
		u = -10.0F;
	}
	val = DAN_PWM_PR*u/20.0F + DAN_PWM_DUTY50;

	switch (ep) {
	case EPWM1:		EPwm1Regs.CMPA.half.CMPA = val;	break;
	case EPWM2:		EPwm2Regs.CMPA.half.CMPA = val;	break;
	case EPWM3:
		if (PWM3setupasRCservo == 0) {
			EPwm3Regs.CMPA.half.CMPA = val;
		}
		break;
	case EPWM4:
		if (PWM4setupasRCservo == 0) {
			EPwm4Regs.CMPA.half.CMPA = val;
		}
		break;
	case EPWM5:
		if (PWM5setupasRCservo == 0) {
			EPwm5Regs.CMPA.half.CMPA = val;
		}
		break;
	case EPWM6:		EPwm6Regs.CMPA.half.CMPA = val;	break;
	}
}

/* dualPWM_out(): update PWM's duty cycle -10.0<u<10.0
 *
 * parameters:
 *     pwm = PWM to update
 *     u   = duty cycle (-10 < u < 10)
 */
void dualPWM_out(enum epwm ep, float u)
{
	unsigned int val;

	if (u > 10.0F) {
		u = 10.0F;
	} else if (u < -10.0F) {
		u = -10.0F;
	}
	val = DAN_PWM_PR*fabs(u)/10.0F;

	switch (ep) {
	case EPWM1:	
		if (u >= 0.0) { 
			EPwm1Regs.CMPA.half.CMPA = val;
			EPwm1Regs.CMPB = 0;
		} else {
			EPwm1Regs.CMPA.half.CMPA = 0;
			EPwm1Regs.CMPB = val;
		}
	break;

/*	case EPWM2:		EPwm2Regs.CMPA.half.CMPA = val;	break;
	case EPWM3:		EPwm3Regs.CMPA.half.CMPA = val;	break;
	case EPWM4:		EPwm4Regs.CMPA.half.CMPA = val;	break;
	case EPWM5:		EPwm5Regs.CMPA.half.CMPA = val;	break;
	case EPWM6:		EPwm6Regs.CMPA.half.CMPA = val;	break;
*/
	}
}


/* RCSERVO_out(): update PWM's duty cycle 2.0% < u < 15.0%
 *
 * parameters:
 *     pwm = PWM to update
 *     u   = duty cycle (2.0% < u < 15.0%)
 */
void RCSERVO_out(enum epwm ep, float u)
{
	unsigned int val;

	if (u > 15.0) {
		u = 15.0;
	} else if (u < 2.0) {
		u = 2.0;
	}
	// Period 50Hz  .02/((1/150000000)*64) = 46875
	val = 46875*(u*0.01);

	switch (ep) {
	case EPWM1:		EPwm1Regs.CMPA.half.CMPA = val;	break;
	case EPWM2:		EPwm2Regs.CMPA.half.CMPA = val;	break;
	case EPWM3:
		if (PWM3setupasRCservo == 1) {
			EPwm3Regs.CMPA.half.CMPA = val;
		}
		break;
	case EPWM4:
		if (PWM4setupasRCservo == 1) {
			EPwm4Regs.CMPA.half.CMPA = val;
		}
		break;
	case EPWM5:
		if (PWM5setupasRCservo == 1) {
			EPwm5Regs.CMPA.half.CMPA = val;
		}
		break;
	case EPWM6:		EPwm6Regs.CMPA.half.CMPA = val;	break;
	case EPWM3B:	EPwm3Regs.CMPB = val; break;
	case EPWM4B:	EPwm4Regs.CMPB = val; break;

	}
}


/* PWM_out(): update PWM's duty cycle -10.0<u<10.0
 *
 * parameters:
 *     pwm = PWM to update
 *     u   = duty cycle (-10 < u < 10)
 */
void PWMandDIR_out(enum epwm ep, float u)
{
	unsigned int val;

	if (u > 10.0F) {
		u = 10.0F;
	} else if (u < -10.0F) {
		u = -10.0F;
	}

	val = DAN_PWM_PR*fabsf(u)/10.0F;

	switch (ep) {
	case EPWM1:
		EPwm1Regs.CMPA.half.CMPA = val;
		if (u >= 0) {
			GpioDataRegs.GPASET.bit.GPIO1 = 1;
		} else {
			GpioDataRegs.GPACLEAR.bit.GPIO1 = 1;
		}
		break;
	case EPWM2:
		EPwm2Regs.CMPA.half.CMPA = val;
		if (u >= 0) {
			GpioDataRegs.GPASET.bit.GPIO3 = 1;
		} else {
			GpioDataRegs.GPACLEAR.bit.GPIO3 = 1;
		}
		break;
	case EPWM3:
		if (PWM3setupasRCservo == 0) {
			EPwm3Regs.CMPA.half.CMPA = val;
			if (u >= 0) {
				GpioDataRegs.GPASET.bit.GPIO5 = 1;
			} else {
				GpioDataRegs.GPACLEAR.bit.GPIO5 = 1;
			}
		}
		break;
	case EPWM4:
		if (PWM4setupasRCservo == 0) {
			EPwm4Regs.CMPA.half.CMPA = val;
			if (u >= 0) {
				GpioDataRegs.GPASET.bit.GPIO7 = 1;
			} else {
				GpioDataRegs.GPACLEAR.bit.GPIO7 = 1;
			}
		}
		break;
	case EPWM5:
		if (PWM5setupasRCservo == 0) {
			EPwm5Regs.CMPA.half.CMPA = val;
			if (u >= 0) {
				GpioDataRegs.GPASET.bit.GPIO19 = 1;
			} else {
				GpioDataRegs.GPACLEAR.bit.GPIO19 = 1;
			}
		}
		break;
	case EPWM6:
		//EPwm6Regs.CMPA.half.CMPA = val;
		break;
	}
}
