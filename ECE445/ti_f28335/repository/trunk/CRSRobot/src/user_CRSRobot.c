#include <tistdtypes.h>
#include <coecsl.h>
#include "user_includes.h"
#include "28335_spi.h"
#include "28335_inits.h"
#include "math.h"

void matlab_serialRX(serial_t *s, char data);
void simulink_serialRX(serial_t *s, char data);

void updateData(void);
void sendData(void);

extern int SPIenc_state;
extern long SPIenc1_reading;
extern long SPIenc2_reading;
extern long SPIenc3_reading;
extern long SPIenc4_reading;
extern long SPIenc5_reading;
extern float offset_Enc2_rad;
extern float offset_Enc3_rad;

unsigned long Main_timeint = 0;

float Main_Enc1_rad = 0;
float Main_Enc2_rad = 0;
float Main_Enc3_rad = 0;
float Main_Enc4_rad = 0;
float Main_Enc5_rad = 0;
float testvar = 0;
float testvar2 = 0;
eqep_t Main_enc1;
eqep_t Main_enc2;
float Main_value_enc1 = 0;
float Main_value_enc2 = 0;

float Main_v1 = 0;
float Main_v1old1 = 0;
float Main_v1old2 = 0;
float Main_angle1_ref = 0.0;
float Main_u1 = 0;
float Main_Enc1_rad_old = 0;
float Main_e1 = 0, Main_e1_thresh = 0.1;

float Main_v2 = 0;
float Main_v2old1 = 0;
float Main_v2old2 = 0;
float Main_angle2_ref = 0.0;
float Main_u2 = 0;
float Main_Enc2_rad_old = 0;
float Main_e2 = 0, Main_e2_thresh = 0.1;

float Main_v3 = 0;
float Main_v3old1 = 0;
float Main_v3old2 = 0;
float Main_angle3_ref = 0.0;
float Main_u3 = 0;
float Main_Enc3_rad_old = 0;
float Main_e3 = 0, Main_e3_thresh = 0.1;

float Main_v4 = 0;
float Main_v4old1 = 0;
float Main_v4old2 = 0;
float Main_angle4_ref = 0.0;
float Main_u4 = 0;
float Main_Enc4_rad_old = 0;
float Main_e4 = 0, Main_e4_thresh = 0.1;

float Main_v5 = 0;
float Main_v5old1 = 0;
float Main_v5old2 = 0;
float Main_angle5_ref = 0.0;
float Main_u5 = 0;
float Main_Enc5_rad_old = 0;
float Main_e5 = 0, Main_e5_thresh = 0.1;

float Main_Kp1 = 30;
float Main_Kp2 = 30;
float Main_Kp3 = 30;
float Main_Kp4 = 55;
float Main_Kp5 = 55;

float Main_Kd1 = 1;
float Main_Kd2 = 1;
float Main_Kd3 = 1;
float Main_Kd4 = 1;
float Main_Kd5 = 1;

float Main_Kp1_long = 15;
float Main_Kp2_long = 15;
float Main_Kp3_long = 15;
float Main_Kp4_long = 55;
float Main_Kp5_long = 55;
float Main_Kp1_short = 25;
float Main_Kp2_short = 25;
float Main_Kp3_short = 25;
float Main_Kp4_short = 55;
float Main_Kp5_short = 55;


#define NUM_SEND_QUEUES 120
#define MAX_SEND_LENGTH 1600
#define MAX_VAR_NUM 10

#pragma DATA_SECTION(matlabLock, ".my_vars")
float matlabLock = 0;



float matlabLockshadow = 0;

// UART 1 GLOBAL VARIABLES
int	UARTsensordatatimeouterror = 0;	// Initialize timeout error count
int	UARTtransmissionerror = 0;	        // Initialize transmission error count

int UARTbeginnewdata = 0;
extern SEM_Obj SEM_UARTMessageReady;
int UARTdatacollect = 0;
char UARTMessageArray[101];
int UARTreceivelength = 0;

char Main_sendingarray = 0;  // Flag to Stop terminal prints when using matlab commands
						//  Only way to clear this flag

union mem_add {
	float f;
	long i;
	char c[2];
}memloc;

union ptrmem_add {
	float* f;
	long* i;
	char c[2];
}ptrmemloc;

long* Main_address[MAX_VAR_NUM];
float Main_value[MAX_VAR_NUM];
char Main_SendArray[128];
char Main_SendArray2[128];
float Main_tempf=0;

int Main_i = 0;
int Main_j = 0;
int Main_memcount = 0;

void lab(float theta1motor,float theta2motor,float theta3motor,float *tau1,float *tau2,float *tau3, int error);
float Main_labtau1 = 0;
float Main_labtau2 = 0;
float Main_labtau3 = 0;
int Main_safetymode = 0;

float Main_thetaDH1 = 0;
float Main_thetaDH2 = 0;
float Main_thetaDH3 = 0;
// forward kinematics
float Main_cosq1 = 0;
float Main_sinq1 = 0;
float Main_cosq2 = 0;
float Main_sinq2 = 0;
float Main_cosq3 = 0;
float Main_sinq3 = 0;
float Main_xk = 0;
float Main_yk = 0;
float Main_zk = 0;

float Main_u1Integral = 0;
float Main_u2Integral = 0;
float Main_u3Integral = 0;

float Main_IntegralSum = 0;

float Main_u1_old = 0;
float Main_u2_old = 0;
float Main_u3_old = 0;


#define DHTHETA1_MAX PI*0.6667
#define DHTHETA1_MIN -PI*0.6667
#define DHTHETA2_MAX 0.15
#define DHTHETA2_MIN -2.3
#define DHTHETA3_MAX 2.3
#define DHTHETA3_MIN 0.15
#define Z_MIN 4.0


char SIMU_databyte1 = 0;
int SIMU_Var1_fromSIMU_16bit = 0;
int SIMU_Var2_fromSIMU_16bit = 0;
int SIMU_Var3_fromSIMU_16bit = 0;
int SIMU_Var4_fromSIMU_16bit = 0;
int SIMU_Var5_fromSIMU_16bit = 0;
int SIMU_Var6_fromSIMU_16bit = 0;
int SIMU_Var7_fromSIMU_16bit = 0;
char SIMU_TXrawbytes[16];
long SIMU_Var1_toSIMU_32bit = 0;
long SIMU_Var2_toSIMU_32bit = 0;
long SIMU_Var3_toSIMU_32bit = 0;
long SIMU_Var4_toSIMU_32bit = 0;
int SIMU_beginnewdata = 0;
int SIMU_datacollect = 0;
int SIMU_Tranaction_Type = 0;
int SIMU_checkfirstcommandbyte = 0;

extern float Simulink_PlotVar1;
extern float Simulink_PlotVar2;
extern float Simulink_PlotVar3;
extern float Simulink_PlotVar4;

int shutoffEnc5error  = 0;


void simulink_serialRX(serial_t *s, char data) {
//	if (savenumbytes < 400) {  // Just for Debug
//		savebytes[savenumbytes] = data;
//		savenumbytes++;
//	}
	if (!SIMU_beginnewdata) {// Only true if have not yet begun a message
		if (SIMU_checkfirstcommandbyte == 1) {
			if (0xFF == (unsigned char)data) {// Check for start 2 bytes command = 32767 becuase assuming command will stay between -10000 and 10000
				SIMU_checkfirstcommandbyte = 0;
			}
		} else {
			SIMU_checkfirstcommandbyte = 1;
			if (0x7F == (unsigned char)data) {// Check for start char

				SIMU_datacollect = 0;		// amount of data collected in message set to 0
				SIMU_beginnewdata = 1;		// flag to indicate we are collecting a message

				SIMU_Tranaction_Type = 2;

				// For Simulink data collection just send most current value
				// Simulink Sample rate needs to be at best 500HZ but 200Hz or slower probably better
				SIMU_Var1_toSIMU_32bit = 10000*Simulink_PlotVar1;
				SIMU_Var2_toSIMU_32bit = 10000*Simulink_PlotVar2;

				SIMU_Var3_toSIMU_32bit = 10000*Simulink_PlotVar3;
				SIMU_Var4_toSIMU_32bit = 10000*Simulink_PlotVar4;

				SIMU_TXrawbytes[3] = (char)((SIMU_Var1_toSIMU_32bit >> 24) & 0xFF);
				SIMU_TXrawbytes[2] = (char)((SIMU_Var1_toSIMU_32bit >> 16) & 0xFF);
				SIMU_TXrawbytes[1] = (char)((SIMU_Var1_toSIMU_32bit >> 8) & 0xFF);
				SIMU_TXrawbytes[0] = (char)((SIMU_Var1_toSIMU_32bit) & 0xFF);

				SIMU_TXrawbytes[7] = (char)((SIMU_Var2_toSIMU_32bit >> 24) & 0xFF);
				SIMU_TXrawbytes[6] = (char)((SIMU_Var2_toSIMU_32bit >> 16) & 0xFF);
				SIMU_TXrawbytes[5] = (char)((SIMU_Var2_toSIMU_32bit >> 8) & 0xFF);
				SIMU_TXrawbytes[4] = (char)((SIMU_Var2_toSIMU_32bit) & 0xFF);

				SIMU_TXrawbytes[11] = (char)((SIMU_Var3_toSIMU_32bit >> 24) & 0xFF);
				SIMU_TXrawbytes[10] = (char)((SIMU_Var3_toSIMU_32bit >> 16) & 0xFF);
				SIMU_TXrawbytes[9] = (char)((SIMU_Var3_toSIMU_32bit >> 8) & 0xFF);
				SIMU_TXrawbytes[8] = (char)((SIMU_Var3_toSIMU_32bit) & 0xFF);

				SIMU_TXrawbytes[15] = (char)((SIMU_Var4_toSIMU_32bit >> 24) & 0xFF);
				SIMU_TXrawbytes[14] = (char)((SIMU_Var4_toSIMU_32bit >> 16) & 0xFF);
				SIMU_TXrawbytes[13] = (char)((SIMU_Var4_toSIMU_32bit >> 8) & 0xFF);
				SIMU_TXrawbytes[12] = (char)((SIMU_Var4_toSIMU_32bit) & 0xFF);


				serial_send(&SerialB,SIMU_TXrawbytes,16);

			}
		}
	} else {	// Filling data
		if (SIMU_Tranaction_Type == 2) {
			if (SIMU_datacollect == 0){
				SIMU_databyte1 = data;
				SIMU_datacollect++;
			}else if (SIMU_datacollect == 1){

				SIMU_Var1_fromSIMU_16bit = ((int)data)<<8 | SIMU_databyte1;

				SIMU_datacollect++;
			} else if (SIMU_datacollect == 2){
				SIMU_databyte1 = data;
				SIMU_datacollect++;
			}else if (SIMU_datacollect == 3){

				SIMU_Var2_fromSIMU_16bit = ((int)data)<<8 | SIMU_databyte1;

				SIMU_datacollect++;
			} else if (SIMU_datacollect == 4){
				SIMU_databyte1 = data;
				SIMU_datacollect++;
			}else if (SIMU_datacollect == 5){

				SIMU_Var3_fromSIMU_16bit = ((int)data)<<8 | SIMU_databyte1;
				SIMU_datacollect++;
			} else if (SIMU_datacollect == 6){
				SIMU_databyte1 = data;
				SIMU_datacollect++;
			}else if (SIMU_datacollect == 7){

				SIMU_Var4_fromSIMU_16bit = ((int)data)<<8 | SIMU_databyte1;
				SIMU_datacollect++;

			} else if (SIMU_datacollect == 8){
				SIMU_databyte1 = data;
				SIMU_datacollect++;
			}else if (SIMU_datacollect == 9){

				SIMU_Var5_fromSIMU_16bit = ((int)data)<<8 | SIMU_databyte1;
				SIMU_datacollect++;
			} else if (SIMU_datacollect == 10) {
				SIMU_databyte1 = data;
				SIMU_datacollect++;
			} else if (SIMU_datacollect == 11) {
				SIMU_Var6_fromSIMU_16bit = ((int)data)<<8 | SIMU_databyte1;
				SIMU_datacollect++;
			} else if (SIMU_datacollect == 12) {
				SIMU_databyte1 = data;
				SIMU_datacollect++;
			}else if (SIMU_datacollect == 13) {
				SIMU_Var7_fromSIMU_16bit = ((int)data)<<8 | SIMU_databyte1;

				SIMU_beginnewdata = 0;	// Reset the flag
				SIMU_datacollect = 0;	// Reset the number of chars collected
				SIMU_Tranaction_Type = 0;
			}

		}
	}
}



void EchoSerialData(int memcount,char *buffer) {


	char sendmsg[256];
	int i;

	sendmsg[0] = 0x2A; // *
	sendmsg[2] = '0'; // 0
	for (i=3;i<(memcount*8+3);i++) {
			sendmsg[i] = buffer[i-3];
	}
	sendmsg[1] = i;
	serial_send(&SerialC, sendmsg, i);


}

void matlab_serialSWI(void) {
	// Case '0' : Sending data in endian format (big-endian address, big-endian value)
	if ('0' == UARTMessageArray[0]){
		for (Main_i = 0;Main_i<Main_memcount;Main_i++){
			ptrmemloc.i=Main_address[Main_i];
			Main_SendArray[0+8*Main_i]=((ptrmemloc.c[1]>>8)&0xFF);
			Main_SendArray[1+8*Main_i]=ptrmemloc.c[1]&0xFF;
			Main_SendArray[2+8*Main_i]=((ptrmemloc.c[0]>>8)&0xFF);
			Main_SendArray[3+8*Main_i]=ptrmemloc.c[0]&0xFF;
			memloc.f=Main_value[Main_i];
			Main_SendArray[4+8*Main_i]=((memloc.c[1]>>8)&0xFF);
			Main_SendArray[5+8*Main_i]=memloc.c[1]&0xFF;
			Main_SendArray[6+8*Main_i]=((memloc.c[0]>>8)&0xFF);
			Main_SendArray[7+8*Main_i]=memloc.c[0]&0xFF;
		}
		EchoSerialData(Main_memcount,Main_SendArray);	// Append header information to send data and transmit
		// Case '1' : Writing float value to memory address (big-endian received address / value)
	}else if ('1' == UARTMessageArray[0]){
		for (Main_i = 0; Main_i < (UARTreceivelength - 2)/8;Main_i++){

			ptrmemloc.c[1] = ((UARTMessageArray[1+8*Main_i]&0xFF)<<8);
			ptrmemloc.c[1] |= (UARTMessageArray[2+8*Main_i]&0xFF);
			ptrmemloc.c[0] = ((UARTMessageArray[3+8*Main_i]&0xFF)<<8);
			ptrmemloc.c[0] |= (UARTMessageArray[4+8*Main_i]&0xFF);

			memloc.c[1] = ((UARTMessageArray[5+8*Main_i]&0xFF)<<8);
			memloc.c[1] |= (UARTMessageArray[6+8*Main_i]&0xFF);
			memloc.c[0] = ((UARTMessageArray[7+8*Main_i]&0xFF)<<8);
			memloc.c[0] |= (UARTMessageArray[8+8*Main_i]&0xFF);

			*ptrmemloc.i = memloc.i;


		}

		matlabLockshadow = matlabLock;
		// Case '2' : Sending array data in following format [char 1,char2,char3,...]
		// [*,3+input length of array,3 (code for array receiving in Matlab),...
		// 		array(0) chars in little-endian, ... , array(memcount) chars in little-endian]
	}else if ('2' == UARTMessageArray[0]){

		Main_sendingarray = 1;
		matlabLock = 1.0;
		matlabLockshadow = matlabLock;
		memloc.c[1] = NULL;
		memloc.c[0] = ((UARTMessageArray[5]&0xFF)<<8);
		memloc.c[0] |= (UARTMessageArray[6]&0xFF);
		Main_memcount = memloc.i;
		ptrmemloc.c[1] = ((UARTMessageArray[1]&0xFF)<<8);
		ptrmemloc.c[1] |= (UARTMessageArray[2]&0xFF);
		ptrmemloc.c[0] = ((UARTMessageArray[3]&0xFF)<<8);
		ptrmemloc.c[0] |= (UARTMessageArray[4]&0xFF);
		Main_SendArray[0]='*';
		Main_SendArray[1]=3+Main_memcount;
		Main_SendArray[2]='3';

		serial_send(&SerialC, Main_SendArray, 3);

		for (Main_i = 0; Main_i < Main_memcount;Main_i++){
			Main_tempf = *ptrmemloc.f;
			memloc.f = Main_tempf;
			Main_SendArray2[0+Main_j*4] =  (memloc.c[0]&0xFF);
			Main_SendArray2[1+Main_j*4] =  ((memloc.c[0]>>8)&0xFF);
			Main_SendArray2[2+Main_j*4] =  (memloc.c[1]&0xFF);
			Main_SendArray2[3+Main_j*4] =  ((memloc.c[1]>>8)&0xFF);
			memloc.c[1] = ptrmemloc.c[1];
			memloc.c[0] = ptrmemloc.c[0];
			memloc.i+=2;  // was plus 4
			ptrmemloc.c[1]=memloc.c[1];
			ptrmemloc.c[0]=memloc.c[0];
			Main_j++;
			if (32 == Main_j){
				memcpy(Main_SendArray,Main_SendArray2,128);
				serial_send(&SerialC, Main_SendArray, 128);
				Main_j = 0;
			}
		}
		if (Main_j != 0){
			serial_send(&SerialC, Main_SendArray2, (Main_memcount%32)*4);
			Main_j = 0;
		}
		Main_sendingarray = 0;
		// Case '3' : Write float value to memory address (big-endian received address,
		//		little-endian received value)
	}else if ('3' == UARTMessageArray[0]){
		for (Main_i = 0; Main_i < (UARTreceivelength - 2)/8;Main_i++){

			ptrmemloc.c[1] = ((UARTMessageArray[1+8*Main_i]&0xFF)<<8);
			ptrmemloc.c[1] |= (UARTMessageArray[2+8*Main_i]&0xFF);
			ptrmemloc.c[0] = ((UARTMessageArray[3+8*Main_i]&0xFF)<<8);
			ptrmemloc.c[0] |= (UARTMessageArray[4+8*Main_i]&0xFF);

			memloc.c[1] = ((UARTMessageArray[8+8*Main_i]&0xFF)<<8);
			memloc.c[1] |= (UARTMessageArray[7+8*Main_i]&0xFF);
			memloc.c[0] = ((UARTMessageArray[6+8*Main_i]&0xFF)<<8);
			memloc.c[0] |= (UARTMessageArray[5+8*Main_i]&0xFF);

			*ptrmemloc.i = memloc.i;

		}

		matlabLockshadow = matlabLock;
	}
}

void matlab_serialRX(serial_t *s, char data) {
	if (!UARTbeginnewdata) {// Only TRUE if have not yet begun a message
		if (42 == (unsigned char)data) {// Check for start char
			UARTdatacollect = 0;		// amount of data collected in message set to 0
			UARTbeginnewdata = 1;		// flag to indicate we are collecting a message
			Main_memcount = 0;
			Main_i = 0;
		}
	} else {	// Filling data
		if (0 == UARTdatacollect){
			UARTreceivelength = ((int)data)-1; // set receive length to value of char after start char
			UARTdatacollect++;
		}else if (UARTdatacollect < UARTreceivelength){
			UARTMessageArray[UARTdatacollect-1] = (char) data;
			// If sending out float value(s), save input memory locations and values at those addresses
			if (('0' == UARTMessageArray[0]) &&  (UARTdatacollect > 1)){

				if (Main_i == 0) {
					ptrmemloc.c[1] = ((UARTMessageArray[UARTdatacollect-1] & 0xFF) << 8);
				}
				if (Main_i == 1) {
					ptrmemloc.c[1] |= (UARTMessageArray[UARTdatacollect-1] & 0xFF);
				}
				if (Main_i == 2) {
					ptrmemloc.c[0] = ((UARTMessageArray[UARTdatacollect-1] & 0xFF) << 8);
				}
				if (3 == Main_i){
					ptrmemloc.c[0] |= (UARTMessageArray[UARTdatacollect-1] & 0xFF);

					Main_address[Main_memcount]=ptrmemloc.i;
					Main_value[Main_memcount]=*ptrmemloc.f;

					Main_i = 0;
					Main_memcount++;
				}else{
					Main_i++;
				}
			}
			UARTdatacollect++;
		}
		if (UARTdatacollect == UARTreceivelength){  // If input receive length is reached
			UARTbeginnewdata = 0;	// Reset the flag
			UARTdatacollect = 0;	// Reset the number of chars collected
			SWI_post(&SWI_serialC);

		}
	}

}


void main(void)
{

	//  Init and zero encoders
	init_EQEP(&Main_enc1, EQEP1, 3000, 1, 0.0);
	init_EQEP(&Main_enc2, EQEP2, 3000, -1, 0.0);
	EQep1Regs.QPOSCNT = 0;
	EQep2Regs.QPOSCNT = 0;


	// Initialize PWMs
	init_PWMandDIR(EPWM1);
	init_PWMandDIR(EPWM2);
	init_PWMandDIR(EPWM3);
	init_PWMandDIR(EPWM4);
	init_PWMandDIR(EPWM5);


	// System initializations
	pre_init();

	// initialize serial port A to 115200 baud
	init_serial(&SerialA,115200,NULL);
	// initialize serial port B to 57600 baud
	init_serial(&SerialB,115200,simulink_serialRX);
	// initialize serial port C to 19200 baud
	init_serial(&SerialC,115200,matlab_serialRX);  //Nine Pin Connector on Board

	EALLOW;  // set up LED GPIOs
		GpioCtrlRegs.GPAMUX2.bit.GPIO30 = 0;
		GpioDataRegs.GPACLEAR.bit.GPIO30 = 1;
		GpioCtrlRegs.GPADIR.bit.GPIO30 = 1;
		GpioCtrlRegs.GPAMUX2.bit.GPIO31 = 0;
		GpioDataRegs.GPACLEAR.bit.GPIO31 = 1;
		GpioCtrlRegs.GPADIR.bit.GPIO31 = 1;
		GpioCtrlRegs.GPBMUX1.bit.GPIO34 = 0;
		GpioDataRegs.GPBSET.bit.GPIO34 = 1;
		GpioCtrlRegs.GPBDIR.bit.GPIO34 = 1;

		GpioCtrlRegs.GPBMUX2.bit.GPIO60 = 0;
		GpioDataRegs.GPBSET.bit.GPIO60 = 1;
		GpioCtrlRegs.GPBDIR.bit.GPIO60 = 1;

		GpioCtrlRegs.GPCMUX2.bit.GPIO85 = 0;
		GpioDataRegs.GPCSET.bit.GPIO85 = 1;
		GpioCtrlRegs.GPCDIR.bit.GPIO85 = 1;

		GpioCtrlRegs.GPBMUX2.bit.GPIO59 = 0;
		//GpioDataRegs.GPBCLEAR.bit.GPIO59 = 1;
		GpioDataRegs.GPBSET.bit.GPIO59 = 1;
		GpioCtrlRegs.GPBDIR.bit.GPIO59 = 1;

		// set up GPIO3 for amp enable or disable
		GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 0;
		GpioDataRegs.GPACLEAR.bit.GPIO3 = 1;
		GpioCtrlRegs.GPADIR.bit.GPIO3 = 1;

	EDIS;

	init_SPI();
	
	// Add your inits here
	// Initialize PWMs
	init_PWMandDIR(EPWM1);
	init_PWMandDIR(EPWM2);
	init_PWMandDIR(EPWM3);
	init_PWMandDIR(EPWM4);
	init_PWMandDIR(EPWM5);

	// Finalize inits and start DMA/McBSP
	post_init();

}

void start_dataCollection(void) {

	// Start SPI
	start_SPI();

}


void releaseBrk(void){
		GpioDataRegs.GPBSET.bit.GPIO59 = 1;
}

void control(void) {

	updateData();


// ---------- PD Control for Motor #1 ----------
	Main_e1 = Main_angle1_ref - Main_Enc1_rad;	// Compute Error
	Main_v1 = (Main_Enc1_rad - Main_Enc1_rad_old)/.001;
	Main_v1 = (Main_v1 +Main_v1old1 + Main_v1old2)/3.0;

	if(Main_e1 > Main_e1_thresh){
		Main_Kp1 = Main_Kp1_long;
	} else if(Main_e1 < -Main_e1_thresh) {
		Main_Kp1 = Main_Kp1_long;
	} else {
		Main_Kp1 = Main_Kp1_short;
	}

	Main_v1old2 = Main_v1old1;
	Main_v1old1 = Main_v1;
	Main_Enc1_rad_old = Main_Enc1_rad;

// ---------- PD Control for Motor #2 ----------
	Main_e2 = Main_angle2_ref - Main_Enc2_rad;	// Compute Error
	Main_v2 = (Main_Enc2_rad - Main_Enc2_rad_old)/.001;
	Main_v2 = (Main_v2 +Main_v2old2 + Main_v2old2)/3.0; // Averaging filter

	if(Main_e2 > Main_e2_thresh){
		Main_Kp2 = Main_Kp2_long;
	} else if(Main_e2 < -Main_e2_thresh) {
		Main_Kp2 = Main_Kp2_long;
	} else {
		Main_Kp2 = Main_Kp2_short;
	}

	Main_v2old2 = Main_v2old1;
	Main_v2old1 = Main_v2;
	Main_Enc2_rad_old = Main_Enc2_rad;

// ---------- PD Control for Motor #3 ----------
	Main_e3 = Main_angle3_ref - Main_Enc3_rad;	// Compute Error
	Main_v3 = (Main_Enc3_rad - Main_Enc3_rad_old)/.001;
	Main_v3 = (Main_v3 + Main_v3old1 + Main_v3old2)/3.0;

	if(Main_e3 > Main_e3_thresh){
		Main_Kp3 = Main_Kp3_long;
	} else if(Main_e3 < -Main_e3_thresh) {
		Main_Kp3 = Main_Kp3_long;
	} else {
		Main_Kp3 = Main_Kp3_short;
	}
	Main_v3old2 = Main_v3old1;
	Main_v3old1 = Main_v3;
	Main_Enc3_rad_old = Main_Enc3_rad;


// ---------- PD Control for Motor #4 ----------
	Main_e4 = Main_angle4_ref - Main_Enc4_rad;	// Compute Error
	Main_v4 = (Main_Enc4_rad - Main_Enc4_rad_old)/.001;
	Main_v4 = (Main_v4 + Main_v4old1 + Main_v4old2)/3.0;

	if(Main_e4 > Main_e4_thresh){
		Main_Kp4 = Main_Kp4_long;
	} else if(Main_e4 < -Main_e4_thresh) {
		Main_Kp4 = Main_Kp4_long;
	} else {
		Main_Kp4 = Main_Kp4_short;
	}

	Main_v4old2 = Main_v4old1;
	Main_v4old1 = Main_v4;
	Main_Enc4_rad_old = Main_Enc4_rad;


// ---------- PD Control for Motor #5 ----------
	Main_e5 = Main_angle5_ref - Main_Enc5_rad;	// Compute Error
	Main_v5 = (Main_Enc5_rad - Main_Enc5_rad_old)/.001;
	Main_v5 = (Main_v5 + Main_v5old1 + Main_v5old2)/3.0;

	if(Main_e5 > Main_e5_thresh){
		Main_Kp5 = Main_Kp5_long;
	} else if(Main_e5 < -Main_e5_thresh) {
		Main_Kp5 = Main_Kp5_long;
	} else {
		Main_Kp5 = Main_Kp5_short;
	}

	Main_v5old2 = Main_v5old1;
	Main_v5old1 = Main_v5;
	Main_Enc5_rad_old = Main_Enc5_rad;

	Main_u1 = Main_Kp1*(Main_angle1_ref - Main_Enc1_rad) - Main_Kd1*Main_v1;
	Main_u2 = Main_Kp2*(Main_angle2_ref - Main_Enc2_rad) - Main_Kd2*Main_v2;
	Main_u3 = Main_Kp3*(Main_angle3_ref - Main_Enc3_rad) - Main_Kd3*Main_v3;
	Main_u4 = Main_Kp4*(Main_angle4_ref - Main_Enc4_rad) - Main_Kd4*Main_v4;
	Main_u5 = Main_Kp5*(Main_angle5_ref - Main_Enc5_rad) - Main_Kd5*Main_v5;



	if(Main_timeint == 4000){
		releaseBrk();
	}
	if (Main_timeint < 4100){
		Main_u1 = 0;
		Main_u2 = 0;
		Main_u3 = 0;
		Main_u4 = 0;
		Main_u5 = 0;
	} else {

		if ( fabs(Main_angle5_ref - Main_Enc5_rad) > 0.11 ) {
			shutoffEnc5error  = 1;
		}

		lab(Main_Enc1_rad,Main_Enc2_rad,Main_Enc3_rad,&Main_labtau1,&Main_labtau2,&Main_labtau3, Main_safetymode);

		Main_thetaDH1 = Main_Enc1_rad;
		Main_thetaDH2 = Main_Enc2_rad - PI*0.5;
		Main_thetaDH3 = Main_Enc3_rad - Main_Enc2_rad + PI*0.5;

		// forward kinematics
		//Main_cosq1 = cos(Main_Enc1_rad);
		//Main_sinq1 = sin(Main_Enc1_rad);
		Main_cosq2 = cos(Main_Enc2_rad);
		//Main_sinq2 = sin(Main_Enc2_rad);
		//Main_cosq3 = cos(Main_Enc3_rad);
		Main_sinq3 = sin(Main_Enc3_rad);
		//Main_xk = 10*Main_cosq1*(Main_cosq3 + Main_sinq2);
		//Main_yk = 10*Main_sinq1*(Main_cosq3 + Main_sinq2);
		Main_zk = 10*(1+ Main_cosq2 - Main_sinq3);

		if ( (Main_thetaDH1 > DHTHETA1_MAX) ||
			 (Main_thetaDH1 < DHTHETA1_MIN) ||
			 (Main_thetaDH2 > DHTHETA2_MAX) ||
			 (Main_thetaDH2 < DHTHETA2_MIN) ||
			 (Main_thetaDH3 > DHTHETA3_MAX) ||
			 (Main_thetaDH3 < DHTHETA3_MIN) ||
			 (Main_zk < Z_MIN) ) {
			Main_safetymode = 1;
		} else {
			Main_safetymode = 0;
			Main_angle1_ref = Main_Enc1_rad;
			Main_angle2_ref = Main_Enc2_rad;
			Main_angle3_ref = Main_Enc3_rad;
			Main_angle4_ref = 0;
			Main_angle5_ref = 0;

			// Angles are in range so use lab control effort
			Main_u1 = Main_labtau1;
			Main_u2 = Main_labtau2;
			Main_u3 = Main_labtau3;

		}
		if (Main_safetymode == 1) {
			if (fabs(Main_angle1_ref-Main_Enc1_rad) > 0.15) {
				Main_angle1_ref = Main_Enc1_rad;
			}
			if (fabs(Main_angle2_ref-Main_Enc2_rad) > 0.15) {
				Main_angle2_ref = Main_Enc2_rad;
			}
			if (fabs(Main_angle3_ref-Main_Enc3_rad) > 0.15) {
				Main_angle3_ref = Main_Enc3_rad;
			}
		}

	}

	if (Main_u1 > 5) Main_u1 = 5;
	if (Main_u1 < -5) Main_u1 = -5;
	if (Main_u2 > 5) Main_u2 = 5;
	if (Main_u2 < -5) Main_u2 = -5;
	if (Main_u3 > 5) Main_u3 = 5;
	if (Main_u3 < -5) Main_u3 = -5;
	if (Main_u4 > 5) Main_u4 = 5;
	if (Main_u4 < -5) Main_u4 = -5;
	if (Main_u5 > 5) Main_u5 = 5;
	if (Main_u5 < -5) Main_u5 = -5;

	Main_u1Integral += fabs((Main_u1 + Main_u1_old)*0.0005);
	Main_u2Integral += fabs((Main_u2 + Main_u2_old)*0.0005);
	Main_u3Integral += fabs((Main_u3 + Main_u3_old)*0.0005);

	Main_IntegralSum = Main_u1Integral + Main_u2Integral + Main_u3Integral;

	Main_u1_old = Main_u1;
	Main_u2_old = Main_u2;
	Main_u3_old = Main_u3;

	if (shutoffEnc5error == 0) {
		// 6/3/2016 PWM to be sent on SPI to 2nd MC.
		PWMandDIR_out(EPWM1,Main_u1);
		PWMandDIR_out(EPWM2,-Main_u2);
		PWMandDIR_out(EPWM3,Main_u3);
		PWMandDIR_out(EPWM4,Main_u4);
		PWMandDIR_out(EPWM5,-Main_u5);

		sendPWM(Main_u1,-Main_u2,Main_u3,Main_u4,-Main_u5);
	} else {
		PWMandDIR_out(EPWM1,0);
		PWMandDIR_out(EPWM2,0);
		PWMandDIR_out(EPWM3,0);
		PWMandDIR_out(EPWM4,0);
		PWMandDIR_out(EPWM5,0);

		sendPWM(0,0,0,0,0);

	}

	Main_timeint++;

}

void updateData(void) {

	Main_value_enc1 = EQEP_read(&Main_enc1);
	Main_value_enc2 = EQEP_read(&Main_enc2);

	Main_Enc1_rad = ((float) (SPIenc1_reading)*(PI/(72000.0*2)));
	Main_Enc2_rad = ((float) (SPIenc2_reading)*(-PI/(72000.0*2))) + offset_Enc2_rad;
	Main_Enc3_rad = ((float) (SPIenc3_reading)*(PI/(72000.0*2))) + offset_Enc3_rad;
	Main_Enc4_rad = ((float) (SPIenc4_reading)*(PI/(8000.0*2)));
	Main_Enc5_rad = ((float) (SPIenc5_reading)*(-PI/(8000.0*2)));


}




