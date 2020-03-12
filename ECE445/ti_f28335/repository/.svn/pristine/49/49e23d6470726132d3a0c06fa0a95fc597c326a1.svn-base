
#include <tistdtypes.h>
#include <coecsl.h>
#include <DSP2833x_Examples.h>
#include "28335_spi.h"
#include "user_includes.h"

long SPIbyte1,SPIbyte2,SPIbyte3,SPIbyte4,SPIbyte5;
long SPIenc1_reading = 0;
long SPIenc2_reading = 0;
long SPIenc3_reading = 0;
long SPIenc4_reading = 0;
long SPIenc5_reading = 0;
int SPIenc_state = 0;
int	SPIenc_state_errors = 0;
float pwm1 = 0;
float pwm2 = 0;
float pwm3 = 0;
float pwm4 = 0;
float pwm5 = 0;
float dec1 = 0;
float dec2 = 0;
float dec3 = 0;
float dec4 = 0;
float dec5 = 0;

short dummy = 0;


unsigned int dac1data = 0;
unsigned int dac2data = 0;

// SpiaRegs.SPIFFRX.bit.RXFFIL is the max # of bytes allowed to sit in FIFO.
// Since we have to write an command to LS7366 at first and all four LS7366 are
//  set to 4-byte counting mode, when reading from them SpiaRegs.SPIFFRX.bit.RXFFIL
//  should be set to 5.

void init_SPI(void){
	/*****************************************************/
	/*********  SPI  *************************************/
	EALLOW;
	// Set EALLOW bit
	// Disable CPU write access, allowing CPU to write freely to protected registers

		GpioCtrlRegs.GPAMUX1.bit.GPIO9 = 0;
		GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 0;
		GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 0;
		GpioCtrlRegs.GPAMUX2.bit.GPIO22 = 0;
		GpioCtrlRegs.GPAMUX2.bit.GPIO23 = 0;
		// Configure the GPIO9, 10, 11, 22 pins as: General purpose I/O
		GpioCtrlRegs.GPADIR.bit.GPIO6 = 1;
		// Used for debug

		GpioCtrlRegs.GPADIR.bit.GPIO9 = 1;
		GpioCtrlRegs.GPADIR.bit.GPIO10 = 1;
		GpioCtrlRegs.GPADIR.bit.GPIO11 = 1;
		GpioCtrlRegs.GPADIR.bit.GPIO22 = 1;
		GpioCtrlRegs.GPADIR.bit.GPIO23 = 1;
		// Configures the GPIO09, 10, 11, 22 pins as output

		GpioDataRegs.GPACLEAR.bit.GPIO6 = 1;
		// Don't care

		GpioDataRegs.GPASET.bit.GPIO9 = 1;
		GpioDataRegs.GPASET.bit.GPIO10 = 1;
		GpioDataRegs.GPASET.bit.GPIO11 = 1;
		GpioDataRegs.GPASET.bit.GPIO22 = 1;
		GpioDataRegs.GPASET.bit.GPIO23 = 1;
		// Drive GPIO9, 10, 11, 22 high, hold LS7366's in high impedance mode


		//6/6/2016 Adding GPIO85 as CS for slave MC
		GpioCtrlRegs.GPCMUX2.bit.GPIO85 = 0;  // use GPIO85 for CS
		GpioCtrlRegs.GPCDIR.bit.GPIO85 = 1;
		GpioDataRegs.GPCSET.bit.GPIO85 = 1;

	EDIS;
	// Disable CPU write access

	// This part set GPIO 10, 11, 22 as GPIO output and drive them high to hold LS7366's in high impedance mode
	// Transition from low to high disables serial data transmission

// ---------------------------------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------------------------------------------

	InitSpiaGpio();
	// Some function in headfiles?

//	EALLOW;
	// SS for DAC7564
//		GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 0;  // use GPIO19 for SS
//		GpioDataRegs.GPASET.bit.GPIO19 = 1;  // enabled
//		GpioCtrlRegs.GPADIR.bit.GPIO19 = 1;  // GPIO19 as output
//	EDIS;
	// Don't care


	SpiaRegs.SPICCR.bit.SPISWRESET = 0;  // Put SPI in reset, clear SPISTS(SPI status registe).5-7
	// SPISTS.5 is TX BUF FULL Flag;
	// SPISTS.6 is SPI INT Flag;
	// SPISTS.7 is RECEIVER OVERRUN Flag;

	SpiaRegs.SPICCR.bit.CLKPOLARITY = 0;
	SpiaRegs.SPICTL.bit.CLK_PHASE = 1; // Set SCLK for LS7366

	SpiaRegs.SPICCR.bit.SPICHAR = 7;   // set to transmitt 8 bits (Table 2-2 of SPI Guide)

	SpiaRegs.SPICTL.bit.MASTER_SLAVE = 1; // SPI configured as a master
	SpiaRegs.SPICTL.bit.TALK = 1; // Enables transmission for the 4-pin option, ensure to enable the receiver's ~SPISET input pin

	SpiaRegs.SPICTL.bit.SPIINTENA = 0; // Disables the SPI’s ability to generate a transmit/receive interrupt before necessary configurations are made

	SpiaRegs.SPISTS.all=0x00E0; // set SPISTS.5-7
	//? Why reset then set
	// No reason, Dan forgot to change

	// This part is for setting transition: CLK, Data size, Master, Mode
// ---------------------------------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------------------------------------------

	SpiaRegs.SPIBRR = 127;   // divide by 40 3.75 Mhz, 1.4.3 of SPI Guide

	SpiaRegs.SPIFFTX.bit.SPIRST = 1; // SPI FIFO resumes transmit or receive, 2.1.9 of SPI Guide
	SpiaRegs.SPIFFTX.bit.SPIFFENA = 1; // SPI FIFO enhancements enabled (don't know what this means)
	SpiaRegs.SPIFFTX.bit.TXFIFO = 0; // Write 0 to reset the FIFO pointer to zero, and hold in reset.
	SpiaRegs.SPIFFTX.bit.TXFFINTCLR = 1; // Clear TXFFINT flag in bit 7 (TXFFINT)

	SpiaRegs.SPIFFRX.bit.RXFIFORESET = 0; // Reset the FIFO pointer to zero, and hold in reset
	SpiaRegs.SPIFFRX.bit.RXFFOVFCLR = 1; // Clear RXFFOVF flag (indicates if RX FIFO overflows)
	SpiaRegs.SPIFFRX.bit.RXFFINTCLR = 1; // Clear RXFFINT flag (indicates if FXFIFO int. has occurred)
	SpiaRegs.SPIFFRX.bit.RXFFIL = 5; // RX FIFO generates an int. when RXFFST (FIFO status bits) are greater than or equal to 00101
	SpiaRegs.SPIFFRX.bit.RXFFIENA = 0; // RX FIFO int. based on RXFFIL match disabled

	SpiaRegs.SPIFFCT.all=0x00; // zero delay between every transfer from FIFO from FIFO transmit buffer to transmit shift register; 2.1.9 SPI Guide

	SpiaRegs.SPIPRI.bit.FREE = 1; // Free run, continue SPI operation regardless of suspend or when the suspend occurred
	SpiaRegs.SPIPRI.bit.SOFT = 0; // 2.1.10 SPI Guide
	// This part is for initializing TX&RX FIFO: resume FIFO, then Clear TX/RX FIFO and flag, set transfer delay and free run mode

// ---------------------------------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------------------------------------------

	SpiaRegs.SPICCR.bit.SPISWRESET = 1;  // Pull the SPI out of reset； SPI is ready to transmit or receive the next character

	SpiaRegs.SPIFFTX.bit.TXFIFO=1; // Enable TX FIFO operation; 2.1.9 SPI Guide
	SpiaRegs.SPIFFRX.bit.RXFIFORESET=1; // Re-enable RX FIFO operation;

	SpiaRegs.SPIFFRX.bit.RXFFIL = 1; // Receive FIFO int. level bits; same as above; 2.1.10 SPI Guide
	GpioDataRegs.GPACLEAR.bit.GPIO9 = 1;
	GpioDataRegs.GPACLEAR.bit.GPIO10 = 1;
	GpioDataRegs.GPACLEAR.bit.GPIO11 = 1;
	GpioDataRegs.GPACLEAR.bit.GPIO22 = 1;
	GpioDataRegs.GPACLEAR.bit.GPIO23 = 1;	// Drive these pins low, enable serial data transfer
	SpiaRegs.SPITXBUF = ((unsigned)0x20)<<8;  // CLR COUNT all five chips
	// Transfer 0010 0000 --> 00 100 000 to LS7366: CLR register CNTR
	while (SpiaRegs.SPIFFRX.bit.RXFFST != 1) {} // Wait until RX FIFO has one word
	DELAY_US(1000);
	GpioDataRegs.GPASET.bit.GPIO9 = 1;
	GpioDataRegs.GPASET.bit.GPIO10 = 1;
	GpioDataRegs.GPASET.bit.GPIO11 = 1;
	GpioDataRegs.GPASET.bit.GPIO22 = 1;
	GpioDataRegs.GPASET.bit.GPIO23 = 1;	// Pull pins high to disable serial data transfer
	SPIbyte1 = SpiaRegs.SPIRXBUF;  // Read from SPI RX BUFF (I guess it would be reading nonsense)

	SpiaRegs.SPIFFRX.bit.RXFFIL = 2; // Receive FIFO int. level bits set to 2;
	GpioDataRegs.GPACLEAR.bit.GPIO9 = 1;
	GpioDataRegs.GPACLEAR.bit.GPIO10 = 1;
	GpioDataRegs.GPACLEAR.bit.GPIO11 = 1;
	GpioDataRegs.GPACLEAR.bit.GPIO22 = 1;
	GpioDataRegs.GPACLEAR.bit.GPIO23 = 1; // Drive these pins low to start transition

	SpiaRegs.SPITXBUF = ((unsigned)0x88)<<8;  // WR to MDR0
	SpiaRegs.SPITXBUF = ((unsigned)0x83)<<8;
	// Transfer 1000 1000 --> 10 001 000: WR (0x83?) to MDR0 register
	//? MDR0: 1 0 00 00 11 --> four cnt per quad cycle, free-running cnt mode (always running), disable index (?), negative index input (?), filter clk div factor = 2 (?)
	while (SpiaRegs.SPIFFRX.bit.RXFFST != 2) {} // Wait until RX FIFO has two words, then manually read from BUFF (since cannot generate int.)
	DELAY_US(1000);
	GpioDataRegs.GPASET.bit.GPIO9 = 1;
	GpioDataRegs.GPASET.bit.GPIO10 = 1;
	GpioDataRegs.GPASET.bit.GPIO11 = 1;
	GpioDataRegs.GPASET.bit.GPIO22 = 1;
	GpioDataRegs.GPASET.bit.GPIO23 = 1;	// Pull pins high to disable serial data transfer
	SPIbyte1 = SpiaRegs.SPIRXBUF;
	SPIbyte2 = SpiaRegs.SPIRXBUF; // Read from SPI RX BUFF (I guess it would be reading nonsense)


	SpiaRegs.SPIFFRX.bit.RXFFIL = 2;
	GpioDataRegs.GPACLEAR.bit.GPIO9 = 1;
	GpioDataRegs.GPACLEAR.bit.GPIO10 = 1;
	GpioDataRegs.GPACLEAR.bit.GPIO11 = 1;
	GpioDataRegs.GPACLEAR.bit.GPIO22 = 1;
	GpioDataRegs.GPACLEAR.bit.GPIO23 = 1;	// Drive these pins low to start transition
	SpiaRegs.SPITXBUF = ((unsigned)0x90)<<8;  // WR MDR1
	SpiaRegs.SPITXBUF = 0x00<<8;
	// Transfer 1001 0000 --> 10 010 000: WR (0?) to MDR1
	// 0000 0 00: NOP*4, Not used, enable cnting, 4-byte counter mode (size of OTR?)
	while (SpiaRegs.SPIFFRX.bit.RXFFST != 2) {}
	DELAY_US(1000);
	GpioDataRegs.GPASET.bit.GPIO9 = 1;
	GpioDataRegs.GPASET.bit.GPIO10 = 1;
	GpioDataRegs.GPASET.bit.GPIO11 = 1;
	GpioDataRegs.GPASET.bit.GPIO22 = 1;
	GpioDataRegs.GPASET.bit.GPIO23 = 1;	// Pull pins high to disable serial data transfer
	SPIbyte1 = SpiaRegs.SPIRXBUF;
	SPIbyte2 = SpiaRegs.SPIRXBUF; // Read from SPI RX BUFF (I guess it would be reading nonsense)


	SpiaRegs.SPIFFRX.bit.RXFFIL = 1; // Receive FIFO int. level bits; same as above; 2.1.10 SPI Guide
	GpioDataRegs.GPACLEAR.bit.GPIO9 = 1;
	GpioDataRegs.GPACLEAR.bit.GPIO10 = 1;
	GpioDataRegs.GPACLEAR.bit.GPIO11 = 1;
	GpioDataRegs.GPACLEAR.bit.GPIO22 = 1;
	GpioDataRegs.GPACLEAR.bit.GPIO23 = 1;	// Drive these pins low, enable serial data transfer
	SpiaRegs.SPITXBUF = ((unsigned)0x20)<<8;  // CLR COUNT all five chips
	// Transfer 0010 0000 --> 00 100 000 to LS7366: CLR register CNTR
	while (SpiaRegs.SPIFFRX.bit.RXFFST != 1) {} // Wait until RX FIFO has one word
	GpioDataRegs.GPASET.bit.GPIO9 = 1;
	GpioDataRegs.GPASET.bit.GPIO10 = 1;
	GpioDataRegs.GPASET.bit.GPIO11 = 1;
	GpioDataRegs.GPASET.bit.GPIO22 = 1;
	GpioDataRegs.GPASET.bit.GPIO23 = 1;	// Pull pins high to disable serial data transfer
	SPIbyte1 = SpiaRegs.SPIRXBUF;  // Read from SPI RX BUFF (I guess it would be reading nonsense)
	// Setting counter of LS7366


	SpiaRegs.SPICTL.bit.SPIINTENA = 1; // Enable the SPI to generate a transmit/receive interrupt
	SpiaRegs.SPIFFRX.bit.RXFFOVFCLR = 1; // Clear RXFFOVF flag (indicates if RX FIFO overflows)
	SpiaRegs.SPIFFRX.bit.RXFFINTCLR = 1; // Clear RXFFINT flag (indicates if FXFIFO int. has occurred)
	SpiaRegs.SPIFFRX.bit.RXFFIENA = 1; // RX FIFO int. based on RXFFIL match Enabled

/*********  SPI  *************************************/
/*****************************************************/

	// SPI
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP6;   // Acknowledge interrupt to PIE
	PieCtrlRegs.PIEIER6.bit.INTx1 = 1;  //Enable PIE 6.1 interrupt Page 126 of Sys_Ctrl_and_Int

}

void SPI_RXint(void) {
	// SPI receive ISR
	// When the receive interrupt occurs the SPI has also completed transmitting

	GpioDataRegs.GPASET.bit.GPIO9 = 1;
	GpioDataRegs.GPASET.bit.GPIO10 = 1;
	GpioDataRegs.GPASET.bit.GPIO11 = 1;
	GpioDataRegs.GPASET.bit.GPIO22 = 1;
	GpioDataRegs.GPASET.bit.GPIO23 = 1;	// Pull pins high to disable serial data transfer
	GpioDataRegs.GPCSET.bit.GPIO85 = 1;

//	GpioDataRegs.GPASET.bit.GPIO19 = 1;

	switch (SPIenc_state) {
		case 1:
			SPIbyte1 = SpiaRegs.SPIRXBUF;
			// Reading in garbage from sending out one command
			SpiaRegs.SPIFFRX.bit.RXFFIL = 5;
			SPIenc_state = 2;
			// Enter case 2 after this is over (another SPI RX ISR will be triggered since RX data is involved here)
			GpioDataRegs.GPACLEAR.bit.GPIO9 = 1;
			// Pull GPIO0 low start data transfer from U7
			SpiaRegs.SPITXBUF = ((unsigned)0x68)<<8;
			//  0110 1000 --> 01 101 000: Read OTR, get reading of U7 (4 bytes)
			SpiaRegs.SPITXBUF = 0;
			SpiaRegs.SPITXBUF = 0;
			SpiaRegs.SPITXBUF = 0;
			SpiaRegs.SPITXBUF = 0;
			// Write to U7 to meet SPI specification

			break;
		case 2:
			SPIbyte1 = SpiaRegs.SPIRXBUF;
			SPIbyte2 = SpiaRegs.SPIRXBUF & 0xFF;
			SPIbyte3 = SpiaRegs.SPIRXBUF & 0xFF;
			SPIbyte4 = SpiaRegs.SPIRXBUF & 0xFF;
			SPIbyte5 = SpiaRegs.SPIRXBUF & 0xFF;
			// Read from RX FIFO, MSB first; First is garbage generated by sending command
			SPIenc1_reading = (SPIbyte2<<24) | (SPIbyte3<<16) | (SPIbyte4<<8) | SPIbyte5;

			SpiaRegs.SPIFFRX.bit.RXFFIL = 5;
			SPIenc_state = 3;
			GpioDataRegs.GPACLEAR.bit.GPIO10 = 1;
			SpiaRegs.SPITXBUF = ((unsigned)0x68)<<8;
			SpiaRegs.SPITXBUF = 0;
			SpiaRegs.SPITXBUF = 0;
			SpiaRegs.SPITXBUF = 0;
			SpiaRegs.SPITXBUF = 0;

			break;
		case 3:
			SPIbyte1 = SpiaRegs.SPIRXBUF;
			SPIbyte2 = SpiaRegs.SPIRXBUF & 0xFF;
			SPIbyte3 = SpiaRegs.SPIRXBUF & 0xFF;
			SPIbyte4 = SpiaRegs.SPIRXBUF & 0xFF;
			SPIbyte5 = SpiaRegs.SPIRXBUF & 0xFF;
			SPIenc2_reading = (SPIbyte2<<24) | (SPIbyte3<<16) | (SPIbyte4<<8) | SPIbyte5;

			SpiaRegs.SPIFFRX.bit.RXFFIL = 5;
			SPIenc_state = 4;
			GpioDataRegs.GPACLEAR.bit.GPIO11 = 1;
			SpiaRegs.SPITXBUF = ((unsigned)0x68)<<8;
			SpiaRegs.SPITXBUF = 0;
			SpiaRegs.SPITXBUF = 0;
			SpiaRegs.SPITXBUF = 0;
			SpiaRegs.SPITXBUF = 0;

			break;
		case 4:
			SPIbyte1 = SpiaRegs.SPIRXBUF;
			SPIbyte2 = SpiaRegs.SPIRXBUF & 0xFF;
			SPIbyte3 = SpiaRegs.SPIRXBUF & 0xFF;
			SPIbyte4 = SpiaRegs.SPIRXBUF & 0xFF;
			SPIbyte5 = SpiaRegs.SPIRXBUF & 0xFF;
			SPIenc3_reading = (SPIbyte2<<24) | (SPIbyte3<<16) | (SPIbyte4<<8) | SPIbyte5;

			SpiaRegs.SPIFFRX.bit.RXFFIL = 5;
			SPIenc_state = 8;
			GpioDataRegs.GPACLEAR.bit.GPIO22 = 1;
			SpiaRegs.SPITXBUF = ((unsigned)0x68)<<8;
			SpiaRegs.SPITXBUF = 0;
			SpiaRegs.SPITXBUF = 0;
			SpiaRegs.SPITXBUF = 0;
			SpiaRegs.SPITXBUF = 0;

			break;
		case 5:
			SPIbyte1 = SpiaRegs.SPIRXBUF;
			SPIbyte2 = SpiaRegs.SPIRXBUF & 0xFF;
			SPIbyte3 = SpiaRegs.SPIRXBUF & 0xFF;
			SPIbyte4 = SpiaRegs.SPIRXBUF & 0xFF;
			SPIbyte5 = SpiaRegs.SPIRXBUF & 0xFF;
			SPIenc5_reading = (SPIbyte2<<24) | (SPIbyte3<<16) | (SPIbyte4<<8) | SPIbyte5;
			SPIenc_state = 1000;   // large state number so that if error interrupt happends default case will be called
			SWI_post(&SWI_control);
			// Call control function we used a lot, indicating that data transmission is over
			// Note that case 6 won't be triggered since the value of SPIenc_state is not changed in this case

			break;
		case 6:
			SPIbyte1 = SpiaRegs.SPIRXBUF;
			SPIbyte1 = SpiaRegs.SPIRXBUF;
			SPIbyte1 = SpiaRegs.SPIRXBUF;
			SPIenc_state = 7;

			// Output to DAC Ch2
			SpiaRegs.SPIFFRX.bit.RXFFIL = 3;
			SPIenc_state = 7;
//			GpioDataRegs.GPACLEAR.bit.GPIO19 = 1;
			SpiaRegs.SPIFFRX.bit.RXFFIL = 3;
			SpiaRegs.SPITXBUF = ((unsigned)0x12)<<8;
			SpiaRegs.SPITXBUF = (int)(dac2data << 4);
			SpiaRegs.SPITXBUF = ((int)(dac2data))<<12;

			break;
		case 7:
			SPIbyte1 = SpiaRegs.SPIRXBUF;
			SPIbyte1 = SpiaRegs.SPIRXBUF;
			SPIbyte1 = SpiaRegs.SPIRXBUF;


			SpiaRegs.SPICCR.bit.CLKPOLARITY = 0;  // set for LS7366
			SpiaRegs.SPICTL.bit.CLK_PHASE = 1;

			SPIenc_state = 0;

			// Debug to see how long 4 SPI enc read takes
//			GpioDataRegs.GPACLEAR.bit.GPIO6 = 1;

			break;

		case 8:
			SPIbyte1 = SpiaRegs.SPIRXBUF;
			SPIbyte2 = SpiaRegs.SPIRXBUF & 0xFF;
			SPIbyte3 = SpiaRegs.SPIRXBUF & 0xFF;
			SPIbyte4 = SpiaRegs.SPIRXBUF & 0xFF;
			SPIbyte5 = SpiaRegs.SPIRXBUF & 0xFF;
			SPIenc4_reading = (SPIbyte2<<24) | (SPIbyte3<<16) | (SPIbyte4<<8) | SPIbyte5;

			SpiaRegs.SPIFFRX.bit.RXFFIL = 5;
			SPIenc_state = 5;
			GpioDataRegs.GPACLEAR.bit.GPIO23 = 1;
			SpiaRegs.SPITXBUF = ((unsigned)0x68)<<8;
			SpiaRegs.SPITXBUF = 0;
			SpiaRegs.SPITXBUF = 0;
			SpiaRegs.SPITXBUF = 0;
			SpiaRegs.SPITXBUF = 0;

			break;
		case 9:
			SPIbyte1 = SpiaRegs.SPIRXBUF;
			SPIbyte1 = SpiaRegs.SPIRXBUF;
			SPIbyte1 = SpiaRegs.SPIRXBUF;
			SPIbyte1 = SpiaRegs.SPIRXBUF;

			if(((SPIbyte1 & 0xFF) == 0xE3)){ //Check to see if Piccolo is reporting SPI error. If 0xE3 is sent from the Picollo it is reporting an SPI error.

				SpiaRegs.SPIFFRX.bit.RXFFIL = 4;
				SPIenc_state = 10;
				DELAY_US(20);
				GpioDataRegs.GPCCLEAR.bit.GPIO85 = 1;
				SpiaRegs.SPITXBUF = 0xDC00; //pwmnum				//the pwmnum,msb,lsb values are sent to the Picollo who will be checking for them.
				SpiaRegs.SPITXBUF = 0x0B00; //msb					//The piccolo will eventually resync with the Delfino and stop sending the SPI error message.
				SpiaRegs.SPITXBUF = 0xEF00; //lsb
				SpiaRegs.SPITXBUF = 0x0000; //dec value dummy

			}
			else{

				SpiaRegs.SPIFFRX.bit.RXFFIL = 4;					//The lines below are used for normal SPI operation.
				SPIenc_state = 10;
				DELAY_US(20);			//Added a delay to make sure the chip select line has enough time to change state. Without this line the messages don't send properly.
				GpioDataRegs.GPCCLEAR.bit.GPIO85 = 1;
				SpiaRegs.SPITXBUF = 0x0200; //pwmnum
				SpiaRegs.SPITXBUF = (short)(pwm2); //msb
				SpiaRegs.SPITXBUF = ((short)(pwm2)<<8); //lsb
				SpiaRegs.SPITXBUF = ((short)(dec2)<<8); //lsb of dec2

			}

			break;
		case 10:
			SPIbyte1 = SpiaRegs.SPIRXBUF;
			SPIbyte1 = SpiaRegs.SPIRXBUF;
			SPIbyte1 = SpiaRegs.SPIRXBUF;
			SPIbyte1 = SpiaRegs.SPIRXBUF;

			if(((SPIbyte1 & 0xFF) == 0xE3)){

				SpiaRegs.SPIFFRX.bit.RXFFIL = 4;
				SPIenc_state = 11;
				DELAY_US(20);
				GpioDataRegs.GPCCLEAR.bit.GPIO85 = 1;
				SpiaRegs.SPITXBUF = 0xDC00; //pwmnum
				SpiaRegs.SPITXBUF = 0x0B00; //msb
				SpiaRegs.SPITXBUF = 0xEF00; //lsb
				SpiaRegs.SPITXBUF = 0x0000; //dec

			}
			else{

				SpiaRegs.SPIFFRX.bit.RXFFIL = 4;
				SPIenc_state = 11;
				DELAY_US(20);
				GpioDataRegs.GPCCLEAR.bit.GPIO85 = 1;
				SpiaRegs.SPITXBUF = 0x0300; //pwmnum
				SpiaRegs.SPITXBUF = (short)(pwm3); //msb
				SpiaRegs.SPITXBUF = ((short)(pwm3)<<8); //lsb
				SpiaRegs.SPITXBUF = ((short)(dec3)<<8); //lsb of dec3

			}


			break;
		case 11:
			SPIbyte1 = SpiaRegs.SPIRXBUF;
			SPIbyte1 = SpiaRegs.SPIRXBUF;
			SPIbyte1 = SpiaRegs.SPIRXBUF;
			SPIbyte1 = SpiaRegs.SPIRXBUF;

			if(((SPIbyte1 & 0xFF) == 0xE3)){

				SpiaRegs.SPIFFRX.bit.RXFFIL = 4;
				SPIenc_state = 12;
				DELAY_US(20);
				GpioDataRegs.GPCCLEAR.bit.GPIO85 = 1;
				SpiaRegs.SPITXBUF = 0xDC00; //pwmnum
				SpiaRegs.SPITXBUF = 0x0B00; //msb
				SpiaRegs.SPITXBUF = 0xEF00; //lsb
				SpiaRegs.SPITXBUF = 0x0000; //lsb

			}
			else{

				SpiaRegs.SPIFFRX.bit.RXFFIL = 4;
				SPIenc_state = 12;
				DELAY_US(20);
				GpioDataRegs.GPCCLEAR.bit.GPIO85 = 1;
				SpiaRegs.SPITXBUF = 0x0400; //pwmnum
				SpiaRegs.SPITXBUF = (short)(pwm4); //msb
				SpiaRegs.SPITXBUF = ((short)(pwm4)<<8); //lsb
				SpiaRegs.SPITXBUF = ((short)(dec4)<<8); //lsb of dec4


			}

			break;
		case 12:
			SPIbyte1 = SpiaRegs.SPIRXBUF;
			SPIbyte1 = SpiaRegs.SPIRXBUF;
			SPIbyte1 = SpiaRegs.SPIRXBUF;
			SPIbyte1 = SpiaRegs.SPIRXBUF;

			if(((SPIbyte1 & 0xFF) == 0xE3)){

				SpiaRegs.SPIFFRX.bit.RXFFIL = 4;
				SPIenc_state = 13;
				DELAY_US(20);
				GpioDataRegs.GPCCLEAR.bit.GPIO85 = 1;
				SpiaRegs.SPITXBUF = 0xDC00; //pwmnum
				SpiaRegs.SPITXBUF = 0x0B00; //msb
				SpiaRegs.SPITXBUF = 0xEF00; //lsb
				SpiaRegs.SPITXBUF = 0x0000; //lsb

			}
			else{

				SpiaRegs.SPIFFRX.bit.RXFFIL = 4;
				SPIenc_state = 13;
				DELAY_US(20);
				GpioDataRegs.GPCCLEAR.bit.GPIO85 = 1;
				SpiaRegs.SPITXBUF = 0x0500; //pwmnum
				SpiaRegs.SPITXBUF = (short)(pwm5); //msb
				SpiaRegs.SPITXBUF = ((short)(pwm5)<<8); //lsb
				SpiaRegs.SPITXBUF = ((short)(dec5)<<8); //lsb of dec5

			}

			break;
		case 13:
			SPIbyte1 = SpiaRegs.SPIRXBUF;
			SPIbyte1 = SpiaRegs.SPIRXBUF;
			SPIbyte1 = SpiaRegs.SPIRXBUF;
			SPIbyte1 = SpiaRegs.SPIRXBUF;
			SPIenc_state = 0;



			break;
		default:
			SPIbyte1 = SpiaRegs.SPIRXBUF;
			SPIbyte2 = SpiaRegs.SPIRXBUF;  // these reads are not needed except to add some delay
			SPIbyte3 = SpiaRegs.SPIRXBUF;
			SPIbyte4 = SpiaRegs.SPIRXBUF;
			SPIbyte5 = SpiaRegs.SPIRXBUF;
			SPIenc_state_errors++;
			break;
	}

	SpiaRegs.SPIFFRX.bit.RXFFOVFCLR=1;  // Clear Overflow flag
	SpiaRegs.SPIFFRX.bit.RXFFINTCLR=1; 	// Clear Interrupt flag
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP6;   // Acknowledge interrupt to PIE




}

long errortoosoon = 0;

void start_SPI(void) {
	// Called every 1 millisecond inside the ADC’s ISR


	if (SPIenc_state != 0) {
		errortoosoon++;
	} else {
		SpiaRegs.SPIFFRX.bit.RXFFIL = 1;
		GpioDataRegs.GPACLEAR.bit.GPIO9 = 1;
		GpioDataRegs.GPACLEAR.bit.GPIO10 = 1;
		GpioDataRegs.GPACLEAR.bit.GPIO11 = 1;
		GpioDataRegs.GPACLEAR.bit.GPIO22 = 1;
		GpioDataRegs.GPACLEAR.bit.GPIO23 = 1;	// Drive pins low to start serial data transfer
		SPIenc_state = 1;
		SpiaRegs.SPITXBUF = ((unsigned)0xE8)<<8; // Latch All ENCs
		// Transfer 1110 1000 --> 11 101 000: transfer CNTR to OTR in "parallel"
		// OTR is for storing value of CNTR, then it can be read without interfering with the cnting process
		// After this function, SPI_RXint will be called
	}
}

void writeDAC7564(float dac1,float dac2) {

	int rawdac1,rawdac2;

	SpiaRegs.SPICCR.bit.CLKPOLARITY = 0;  // set for DAC7564
	SpiaRegs.SPICTL.bit.CLK_PHASE = 0;

	rawdac1 = (int) (dac1 * 1638 + 0.5); // The 0.5 is used to round to the nearest integer
 	rawdac2 = (int) (dac2 * 1638 + 0.5); // The 0.5 is used to round to the nearest integer


   	if (rawdac1 < 0) rawdac1 = 0;
   	if (rawdac1 > 4095) rawdac1 = 4095;
   	if (rawdac2 < 0) rawdac2 = 0;
   	if (rawdac2 > 4095) rawdac2 = 4095;

	dac1data = rawdac1;
	dac2data = rawdac2;

	// Output to DAC Ch1
	SpiaRegs.SPIFFRX.bit.RXFFIL = 3;
	SPIenc_state = 6;
//	GpioDataRegs.GPACLEAR.bit.GPIO19 = 1;
	SpiaRegs.SPIFFRX.bit.RXFFIL = 3;
	SpiaRegs.SPITXBUF = ((unsigned)0x10)<<8;
	SpiaRegs.SPITXBUF = (int)(dac1data << 4);
	SpiaRegs.SPITXBUF = ((int)(dac1data))<<12;

}


// 6/3/2016
void sendPWM(float u1, float u2, float u3, float u4, float u5) {



	if (u1 > 10.0F) {
			u1 = 10.0F;
		} else if (u1 < -10.0F) {
			u1 = -10.0F;
		}
	if (u2 > 10.0F) {
			u2 = 10.0F;
		} else if (u2 < -10.0F) {
			u2 = -10.0F;
		}
	if (u3 > 10.0F) {
			u3 = 10.0F;
		} else if (u3 < -10.0F) {
			u3 = -10.0F;
		}
	if (u4 > 10.0F) {
			u4 = 10.0F;
		} else if (u4 < -10.0F) {
			u4 = -10.0F;
		}
	if (u5 > 10.0F) {
			u5 = 10.0F;
		} else if (u5 < -10.0F) {
			u5 = -10.0F;
		}

	// 3000 is 28027's frequency
	pwm1 = 3000.0*(u1/10.0F);
	pwm2 = 3000.0*(u2/10.0F);
	pwm3 = 3000.0*(u3/10.0F);
	pwm4 = 3000.0*(u4/10.0F);
	pwm5 = 3000.0*(u5/10.0F);

	dec1 = fabs(pwm1) - fabs((short)pwm1);
	dec2 = fabs(pwm2) - fabs((short)pwm2);
	dec3 = fabs(pwm3) - fabs((short)pwm3);
	dec4 = fabs(pwm4) - fabs((short)pwm4);
	dec5 = fabs(pwm5) - fabs((short)pwm5);

	//pwm4 and pwm5 do not have high resolution capabilities. The dec4,dec5 lines above and below are
	//just calculating the decimal value to be consistent with the rest of the pwm signals
	//When sent over SPI they will be assigned to the correct PWM signal, but not put into any pwm
	//register because the high resolution doesn't exist for pwm4 and pwm5

	dec1 = (dec1*111)+.5;
	dec2 = (dec2*111)+.5;
	dec3 = (dec3*111)+.5;
	dec4 = (dec4*111)+.5;
	dec5 = (dec5*111)+.5;

	SpiaRegs.SPIFFRX.bit.RXFFIL = 4;
	SPIenc_state = 9;
	GpioDataRegs.GPCCLEAR.bit.GPIO85 = 1;
	SpiaRegs.SPITXBUF = 0x0100; //pwmnum
	SpiaRegs.SPITXBUF = (short)pwm1; //MSB
	SpiaRegs.SPITXBUF = ((short)pwm1)<<8; //LSB
	SpiaRegs.SPITXBUF = ((short)dec1)<<8; //LSB of Dec1 which is the actual value

}
