#include <tistdtypes.h>
#include <coecsl.h>
#include "user_includes.h"
#include "math.h"

// These two offsets are only used in the main file user_CRSRobot.c  You just need to create them here and find the correct offset and then these offset will adjust the encoder readings
float offset_Enc2_rad = -0.43;
float offset_Enc3_rad = 0.22;


// Your global variables.

long mycount = 0;

#pragma DATA_SECTION(whattoprint, ".my_vars")
float whattoprint = 0.0;

#pragma DATA_SECTION(theta1array, ".my_arrs")
float theta1array[100];
#pragma DATA_SECTION(theta2array, ".my_arrs")
float theta2array[100];
#pragma DATA_SECTION(check, ".my_vars")
float check = 0.0;

long arrayindex = 0;

float printtheta1motor = 0;
float printtheta2motor = 0;
float printtheta3motor = 0;

float printx;
float printy;
float printz;

float theta1calc_dh;
float theta2calc_dh;
float theta3calc_dh;
float D;

float theta1calc_m;
float theta2calc_m;
float theta3calc_m;

// Assign these float to the values you would like to plot in Simulink
float Simulink_PlotVar1 = 0;
float Simulink_PlotVar2 = 0;
float Simulink_PlotVar3 = 0;
float Simulink_PlotVar4 = 0;

int counter=0;
float theta1_prev = 0;
float theta2_prev = 0;
float theta3_prev = 0;
float theta1_dot_prev[2];
float theta2_dot_prev[2];
float theta3_dot_prev[2];
float theta1_dot = 0;
float theta2_dot = 0;
float theta3_dot = 0;
float theta1_dot_avg = 0;
float theta2_dot_avg = 0;
float theta3_dot_avg = 0;
float theta1_int = 0;
float theta2_int = 0;
float theta3_int = 0;
float theta1_threshold = 0;
float theta2_threshold = 0;
float theta3_threshold = 0;
float tau1_prev = 0;
float tau2_prev = 0;
float tau3_prev = 0;

int order = 2;
int i;
int cnt = 0;
int edit1 = 0;
int edit2 = 0;
int edit3 = 0;
int controller = 1;
float t1=0;
float t2=0;
float t3=0;

float kp1 = 40;
float kp2 = 40;
float kp3 = 50;
float kd1 = 4;
float kd2 = 3;
float kd3 = 3;
float ki1 = 0;
float ki2 = 0;
float ki3 = 0;

float error1=0;
float error2=0;
float error3=0;

float theta1desired = 0;
float theta2desired = 0;
float theta3desired = 0;

void printing(void);
void forwardKinematics(float theta1motor,float theta2motor,float theta3motor);
void inverseKinematics(float x, float y, float z);

// This function is called every 1 ms
void lab(float theta1motor,float theta2motor,float theta3motor,float *tau1,float *tau2,float *tau3, int error) {


    theta1_dot = (theta1motor - theta1_prev)/0.001;
    theta2_dot = (theta2motor - theta2_prev)/0.001;
    theta3_dot = (theta3motor - theta3_prev)/0.001;

    theta1_dot_avg = (theta1_dot_prev[0] + theta1_dot_prev[1] + theta1_dot)/3.0;
    theta2_dot_avg = (theta2_dot_prev[0] + theta2_dot_prev[1] + theta2_dot)/3.0;
    theta3_dot_avg = (theta3_dot_prev[0] + theta3_dot_prev[1] + theta3_dot)/3.0;

    for(i=order;i>0;i--)
    {
        theta1_dot_prev[i] = theta1_dot_prev[i-1];
        theta2_dot_prev[i] = theta2_dot_prev[i-1];
        theta3_dot_prev[i] = theta3_dot_prev[i-1];
    }
    theta1_dot_prev[0] = theta1_dot;
    theta2_dot_prev[0] = theta2_dot;
    theta3_dot_prev[0] = theta3_dot;

    theta1_prev = theta1motor;
    theta2_prev = theta2motor;
    theta3_prev = theta3motor;

	//Motor torque limitation(Max: 5 Min: -5)


    error1 = theta1desired - theta1motor;
    error2 = theta2desired - theta2motor;
    error3 = theta3desired - theta3motor;

    if( fabs(theta1motor - theta1desired)< theta1_threshold ){
        theta1_int +=error1;

    }
    if( fabs(theta2motor - theta2desired)< theta2_threshold )
        theta2_int +=error2;
    if( fabs(theta3motor - theta3desired)< theta3_threshold )
        theta3_int +=error3;

    //PD Control
    if(controller == 0){
        *tau1 = kp1*error1 - kd1*theta1_dot;
        *tau2 = kp2*error2 - kd2*theta2_dot;
        *tau3 = kp3*error3 - kd3*theta3_dot;

    }
    //PID Control
    if(controller == 1){
        *tau1 = kp1*error1 - kd1*theta1_dot + ki1*theta1_int;
        if(*tau1 >5 ) *tau1 = 5;
        if(*tau1 <-5) *tau1 = -5;
        *tau2 = kp2*error2 - kd2*theta2_dot + ki2*theta2_int;
        if(*tau2 >5 ) *tau2 = 5;
        if(*tau2 <-5) *tau2 = -5;
        *tau3 = kp3*error3 - kd3*theta3_dot + ki3*theta3_int;
        if(*tau3 >5 ) *tau3 = 5;
        if(*tau3 <-5) *tau3 = -5;
        tau1_prev = *tau1;
        tau2_prev = *tau2;
        tau3_prev = *tau3;
    }


	// save past states
	if ((mycount%50)==0) {

		theta1array[arrayindex] = theta1motor;
		theta2array[arrayindex] = theta2motor;

		if (arrayindex >= 100) {
			arrayindex = 0;
		} else {
			arrayindex++;
		}

	}

	if(cnt<1000) {
	    theta1desired = 0.0;
	    theta2desired = 0.0;
	    theta3desired = 0.0;
	}
	else{
	    theta1desired = t1;
	    theta2desired = t2;
	    theta3desired = t3;
	}
	if(cnt>2000){
	     cnt=0;
	}
	cnt++;

	if ((mycount%500)==0) {
		if (whattoprint > 0.5) {
			serial_printf(&SerialA, "I love robotics\n\r");
		} else {
			printtheta1motor = theta1motor;
			printtheta2motor = theta2motor;
			printtheta3motor = theta3motor;

			forwardKinematics(theta1motor,theta2motor,theta3motor);

			inverseKinematics(printx,printy,printz);


			SWI_post(&SWI_printf); //Using a SWI to fix SPI issue from sending too many floats.
		}
		if (theta2motor > 0){
		    check = 1;
		}
		else{
		    check = 0;
		}
		if(counter>2000){
		    GpioDataRegs.GPBDAT.bit.GPIO60 = 1;
		}
		else if(counter>1500){
		    GpioDataRegs.GPBDAT.bit.GPIO60 = 0;
		}
		else if(counter>1000){
		    GpioDataRegs.GPBDAT.bit.GPIO60 = 1;
		}
		else if(counter>500){
		    GpioDataRegs.GPBDAT.bit.GPIO60 = 0;
		}
		GpioDataRegs.GPBTOGGLE.bit.GPIO34 = 1; // Blink LED on Control Card
		//GpioDataRegs.GPBTOGGLE.bit.GPIO60 = 1; // Blink LED on Emergency Stop Box
	}


	Simulink_PlotVar1 = theta1motor;
	Simulink_PlotVar2 = theta2motor;
	Simulink_PlotVar3 = theta3motor;
	Simulink_PlotVar4 = 0;

	mycount++;
	if(counter<5000)
	{
	    counter++;
	}
	else
	{
	    //serial_printf(&SerialA, "%.2f %.2f %.2f tau: %.2f error: %.2f dest: %0.2f \n\r",printtheta1motor*180/PI,printtheta2motor*180/PI,printtheta3motor*180/PI,*tau1,error1, dest);
	    counter = 0;
	}
	 //serial_printf(&SerialA, "%d\n\r",counter);

}

void forwardKinematics(float theta1motor,float theta2motor,float theta3motor)
{
    printx = 10*cos(theta1motor)*(cos(theta3motor)+sin(theta2motor));
    printy = 10*sin(theta1motor)*(cos(theta3motor)+sin(theta2motor));
    printz = 10*(1 + cos(theta2motor)-sin(theta3motor));
}

void inverseKinematics(float x, float y, float z){

        theta1calc_dh = atan2(y,x);
        D = (x*x + y*y + (z-10)*(z-10) - 200)/200;
        theta3calc_dh = -atan2(sqrt(1-D*D),D);
        theta2calc_dh = -atan2(z-10,sqrt(x*x + y*y)) + atan2(10*sin(theta3calc_dh),10 + 10*cos(theta3calc_dh));

        theta1calc_m = theta1calc_dh;
        theta2calc_m = theta2calc_dh + PI/2;
        theta3calc_m = -theta3calc_dh + theta2calc_m - PI/2;
}

void printing(void){
    serial_printf(&SerialA, "%.2f %.2f %.2f  \n\r %.2f %.2f %.2f  \n\r  %.2f %.2f %.2f \n\r \n\r",printx,printy, printz,printtheta1motor*180/PI,printtheta2motor*180/PI,printtheta3motor*180/PI, theta1calc_m*180/PI,theta2calc_m*180/PI,theta3calc_m*180/PI);
	//serial_printf(&SerialA, "%.2f %.2f %.2f  \n\r",printtheta1motor*180/PI,printtheta2motor*180/PI,printtheta3motor*180/PI);
    //serial_printf(&SerialA, "hi\n\r");
}

