/* 28335_EQEP.C:  F28335 DSP EQEP peripherals interface.
 *
 *
 * QEP (Quadrature Encoder Pulse) interface:
 *    1) initialize the QEP circuit with init_QEP()
 *    2) call QEP_read() to read the current encoder position in radians
 *
 * WRITTEN BY: Paul Miller <pamiller@uiuc.edu>
 * UPDATED: 01/26/2005
 */

#include <coecsl.h>
#include <28335_eQep.h>

/* init_EQEP(): initialize quadrature encoder pulse QEP
 *
 * parameters:
 *    eqep   = address pointer to QEP structure
 *    eq    = EQEP 1 or 2
 *    cpr   = encoder's rated counts per revolution
 *    init_rad_coord = initial radian coordinate for 0 encoder count
 *
 */
void init_EQEP(eqep_t *eqep, enum eQep_e eq, int32 cpr, int pol, float init_rad_coord)
{

	eqep->eq = eq;
	eqep->cpr = 4*cpr;
	eqep->init_rad_coord = init_rad_coord;
	eqep->res = TWOPI/(eqep->cpr);
	eqep->pol = pol;

	// select event manager
	switch (eq) {

	case EQEP1:

		// setup QEP12 pins for input
		EALLOW;
			GpioCtrlRegs.GPAMUX2.bit.GPIO20 = 1;
			GpioCtrlRegs.GPAMUX2.bit.GPIO21 = 1;
			GpioCtrlRegs.GPACTRL.bit.QUALPRD2 = 0;  // use SYSCLK/1
			GpioCtrlRegs.GPAQSEL2.bit.GPIO20 = 2;  // Qual every 6 samples
			GpioCtrlRegs.GPAQSEL2.bit.GPIO21 = 2;

		EDIS;
	
		EQep1Regs.QEPCTL.bit.QPEN = 0;  // make sure qep reset
		//EQep1Regs.QEPCTL.bit.SWI = 1;   // Initialize Position counter to zero
		EQep1Regs.QDECCTL.bit.QSRC = 0;  // Quadrature count mode
		EQep1Regs.QPOSCTL.all = 0x0;   // Disable eQep Position Compare
		EQep1Regs.QCAPCTL.all = 0x0;  // Disable eQep Capture
		EQep1Regs.QEINT.all = 0x0;  // Disable all eQep interrupts
		EQep1Regs.QPOSMAX = 0xFFFFFFFF;
		EQep1Regs.QEPCTL.bit.FREE_SOFT = 2;  // EQep uneffected by emulation suspend
		EQep1Regs.QEPCTL.bit.QPEN = 1; // Enable EQep

		break;

	case EQEP2:

		// setup QEP34 pins for input
		EALLOW;
			GpioCtrlRegs.GPAMUX2.bit.GPIO24 = 2;
			GpioCtrlRegs.GPAMUX2.bit.GPIO25 = 2;
			GpioCtrlRegs.GPACTRL.bit.QUALPRD3 = 0; // use SYSCLK/1
			GpioCtrlRegs.GPAQSEL2.bit.GPIO24 = 2;  // Qual every 6 samples
			GpioCtrlRegs.GPAQSEL2.bit.GPIO25 = 2;

		EDIS;
	
		EQep2Regs.QEPCTL.bit.QPEN = 0;  // make sure qep reset
		//EQep2Regs.QEPCTL.bit.SWI = 1;   // Initialize Position counter to zero
		EQep2Regs.QDECCTL.bit.QSRC = 0;  // Quadrature count mode
		EQep2Regs.QPOSCTL.all = 0x0;   // Disable eQep Position Compare
		EQep2Regs.QCAPCTL.all = 0x0;  // Disable eQep Capture
		EQep2Regs.QEINT.all = 0x0;  // Disable all eQep interrupts
		EQep2Regs.QPOSMAX = 0xFFFFFFFF;
		EQep2Regs.QEPCTL.bit.FREE_SOFT = 2;  // EQep uneffected by emulation suspend
		EQep2Regs.QEPCTL.bit.QPEN = 1; // Enable EQep

		break;

	}
}

/* EQEP_read(): read QEP encoder's position in radians
 *
 * parameters:
 *    eqep   = address pointer to an initialized QEP structure
 *
 * return: encoder position in radians
 */
float EQEP_read(eqep_t *eqep)
{
	int32 raw = 0;

	// read raw value
	switch (eqep->eq) {
	case EQEP1:
		raw = EQep1Regs.QPOSCNT;
		break;
	case EQEP2:
		raw = EQep2Regs.QPOSCNT;
		break;
	}

	if (raw >= QEP_MAXVALUE/2) raw -= QEP_MAXVALUE;

	return ((eqep->pol)*(eqep->res)*raw + eqep->init_rad_coord);
}


