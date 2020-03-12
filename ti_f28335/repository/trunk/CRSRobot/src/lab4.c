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
int simval = 0;

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
float theta1_threshold = 0.05;
float theta2_threshold = 0.05;
float theta3_threshold = 0.05;
float tau1_prev = 0;
float tau2_prev = 0;
float tau3_prev = 0;

int order = 2;
int i;
int cnt = 0;
int controller = 0;
float t1=0;
float t2=0;
float t3=0;

float kp1 = 175;
float kp2 = 115;
float kp3 = 80.0;
float kd1 = 4;
float kd2 = 3;
float kd3 = 6.0;
float ki1 = 0;//20.0;
float ki2 = 0;//10.0;
float ki3 = 0;//150.0;

float j1 = 0.0167;
float j2 = 0.03;
float j3 = 0.0128;

float q = 0;
float qd= 0;
float qdd= 0;
float a0;
float a1;
float a2;
float a3;

float error1=0;
float error2=0;
float error3=0;
float error1_prev=0;
float error2_prev=0;
float error3_prev=0;

float theta1desired = 0;
float theta2desired = 0;
float theta3desired = 0;
int check_control = 1;
float q_print = 0;
float qd_print = 0;
float qdd_print = 0;
float q1p = 0;
float q2p = 0;
float q3p = 0;
float x = 0;
float y = 0;
float z = 0;

float q1 =0;
float q2=0;
float q3=0;

float t = 0;

float Vpos1 = 0.27;
float Vmid1 = 4.25;
float Vneg1 = 0.21;
float Cpos1 = 0.4;
float Cmid1 = 0.0;
float Cneg1 = -0.42;

float Vpos2 = 0.25;
float Cpos2 = 0.45;
float Vmid2 = 3.6;
float Vneg2 = 0.287;
float Cneg2 = -0.47;

float Vpos3 = 0.27;
float Cpos3 = 0.3637;
float Vmid3 = 3.6;
float Vneg3 = 0.26;
float Cneg3 = -0.2948;

float qd1_inv = 0;
float qd1_dot_inv = 0;
float qd1_ddot_inv = 0;
float qd2_inv = 0;
float qd2_dot_inv = 0;
float qd2_ddot_inv = 0;
float qd3_inv = 0;
float qd3_dot_inv = 0;
float qd3_ddot_inv = 0;
float mystep = 0.25;

float errord1 = 0;
float errord2 = 0;
float errord3 = 0;
float a2 = 0;
float a3 = 0;
float Kp2_inv = 1700;
float Kp3_inv = 3000;
float Kd2_inv = 130;
float Kd3_inv = 245;
float cost2t3 = 0;
float cost3 = 0;
float sint2t3 = 0;
float sint2 = 0;
float g = 9.81;
float theta3_dot_dh = 0;

float D2 = 0;
float D3 = 0;
float C2 = 0;
float C3 = 0;
float G2 = 0;
float G3 = 0;

float p1 = 0.0466;
float p2 = 0.0388;
float p3 = 0.0284;
float p4 = 0.1405;
float p5 = 0.1298;
float torque1=-1;

float cosq1 = 0;
float sinq1 = 0;
float cosq2 = 0;
float sinq2 = 0;
float cosq3 = 0;
float sinq3 = 0;
float JT_11 = 0;
float JT_12 = 0;
float JT_13 = 0;
float JT_21 = 0;
float JT_22 = 0;
float JT_23 = 0;
float JT_31 = 0;
float JT_32 = 0;
float JT_33 = 0;
float cosz = 0;
float sinz = 0;
float cosx = 0;
float sinx = 0;
float cosy = 0;
float siny = 0;
float thetaz = 0;
float thetax = 0;
float thetay = 0;
float R11 = 0;
float R12 = 0;
float R13 = 0;
float R21 = 0;
float R22 = 0;
float R23 = 0;
float R31 = 0;
float R32 = 0;
float R33 = 0;
float RT11 = 0;
float RT12 = 0;
float RT13 = 0;
float RT21 = 0;
float RT22 = 0;
float RT23 = 0;
float RT31 = 0;
float RT32 = 0;
float RT33 = 0;


void printing(void);
void forwardKinematics(float theta1motor,float theta2motor,float theta3motor);
void inverseKinematics(float x, float y, float z);



typedef struct steptraj_s {
    long double b[6];
    long double a[6];
    long double xk[6];
    long double yk[6];
    float qd_old;
    float qddot_old;
    int size;
} steptraj_t;

steptraj_t trajectory = {7.0489768460490736e-10L,3.5244884230245368e-09L,7.0489768460490736e-09L,7.0489768460490736e-09L,3.5244884230245368e-09L,7.0489768460490736e-10L,
                        1.0000000000000000e+00L,-4.8522167487684733e+00L,9.4176029508117161e+00L,-9.1392501542359987e+00L,4.4345622669568758e+00L,-8.6069829220739369e-01L,
                        0,0,0,0,0,0,
                        0,0,0,0,0,0,
                        0,
                        0,
                        6};

// this function must be called every 1ms.
void implement_discrete_tf(steptraj_t *traj, float step, float *qd, float *qd_dot, float *qd_ddot) {
    int i = 0;

    traj->xk[0] = step;
    traj->yk[0] = traj->b[0]*traj->xk[0];
    for (i = 1;i<traj->size;i++) {
        traj->yk[0] = traj->yk[0] + traj->b[i]*traj->xk[i] - traj->a[i]*traj->yk[i];
    }

    for (i = (traj->size-1);i>0;i--) {
        traj->xk[i] = traj->xk[i-1];
        traj->yk[i] = traj->yk[i-1];
    }

    *qd = traj->yk[0];
    *qd_dot = (*qd - traj->qd_old)*1000;  //0.001 sample period
    *qd_ddot = (*qd_dot - traj->qddot_old)*1000;

    traj->qd_old = *qd;
    traj->qddot_old = *qd_dot;
}

// to call this function create a variable that steps to the new positions you want to go to, pass this var to step
// pass a reference to your qd variable your qd_dot variable and your qd_double_dot variable
// for example
//  implement_discrete_tf(&trajectory, mystep, &qd, &dot, &ddot);

// This function is called every 1 ms
void lab(float theta1motor,float theta2motor,float theta3motor,float *tau1,float *tau2,float *tau3, int error) {




                            // Rotation zxy and its Transpose
                            cosz = cos(thetaz);
                            sinz = sin(thetaz);
                            cosx = cos(thetax);
                            sinx = sin(thetax);
                            cosy = cos(thetay);
                            siny = sin(thetay);
                            RT11 = R11 = cosz*cosy-sinz*sinx*siny;
                            RT21 = R12 = -sinz*cosx;
                            RT31 = R13 = cosz*siny+sinz*sinx*cosy;
                            RT12 = R21 = sinz*cosy+cosz*sinx*siny;
                            RT22 = R22 = cosz*cosx;
                            RT32 = R23 = sinz*siny-cosz*sinx*cosy;
                            RT13 = R31 = -cosx*siny;
                            RT23 = R32 = sinx;
                            RT33 = R33 = cosx*cosy;


                            // Jacobian Transpose
                            cosq1 = cos(theta1motor);
                            sinq1 = sin(theta1motor);
                            cosq2 = cos(theta2motor);
                            sinq2 = sin(theta2motor);
                            cosq3 = cos(theta3motor);
                            sinq3 = sin(theta3motor);
                            JT_11 = -10*sinq1*(cosq3 + sinq2);
                            JT_12 = 10*cosq1*(cosq3 + sinq2);
                            JT_13 = 0;
                            JT_21 = 10*cosq1*(cosq2 - sinq3);
                            JT_22 = 10*sinq1*(cosq2 - sinq3);
                            JT_23 = -10*(cosq3 + sinq2);
                            JT_31 = -10*cosq1*sinq3;
                            JT_32 = -10*sinq1*sinq3;
                            JT_33 = -10*cosq3;

                            // x y and z
                            x = 10*cosq1*(cosq3+sinq2);
                            y = 10*sinq1*(cosq3+sinq2);
                            z = 10*(1+cosq2-sinq3);



                        // friction compensation for joint 1
                          if(theta1_dot_avg > 0.1){
                              *tau1 = *tau1 + Vpos1*theta1_dot_avg + Cpos1;
                          }
                          else if(fabs(theta1_dot_avg)<=0.1){
                              *tau1 = *tau1 + Vmid1*theta1_dot_avg + Cmid1;
                          }
                          else{
                              //theta1_dot_avg < -0.1
                              *tau1 = *tau1 + Vneg1*theta1_dot_avg + Cneg1 ;
                          }

                          // friction compensation for joint 2
                          if(theta2_dot_avg > 0.05){
                              *tau2 = *tau2 + Vpos2*theta2_dot_avg + Cpos2;
                          }
                          else if(fabs(theta2_dot_avg)<=0.05){
                              *tau2 = *tau2 + Vmid2*theta2_dot_avg;
                          }
                          else{
                              //theta2_dot_avg < -0.05
                              *tau2 = *tau2 + Vneg2*theta1_dot_avg + Cneg2;
                          }


                          // friction compensation for joint 3
                          if(theta3_dot_avg > 0.05){
                              *tau3 = *tau3 + Vpos3*theta3_dot_avg + Cpos3;
                          }
                          else if(fabs(theta3_dot_avg)<=0.05){
                              *tau3 = *tau3 + Vmid3*theta3_dot_avg;
                          }
                          else{
                              //theta3_dot_avg < -0.05
                              *tau3 = *tau3 + Vneg3*theta3_dot_avg + Cneg3;
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
if(controller == 3){
    if(cnt<1000*PI) {
        theta1_int = 0;
        theta2_int = 0;
        theta3_int = 0;
    }
    else{
        theta1_int = 0;
        theta2_int = 0;
        theta3_int = 0;
    }
    if(cnt>2000*PI){
         cnt=0;
    }
    cnt++;
}
else if(controller ==4 || controller ==2){
    if(cnt<4000) {
         mystep = 0.75;
         theta1desired = 0.0;
         theta2desired = 0.0;
         theta3desired = 0.0;
         theta1_int = 0;
         theta2_int = 0;
         theta3_int = 0;
          }
     else{
         mystep = 0.25;
         theta1desired = t1;
         theta2desired = t2;
         theta3desired = t3;
         theta1_int = 0;
         theta2_int = 0;
         theta3_int = 0;
     }
     if(cnt>8000){
          cnt=0;
     }
     cnt++;
}

else {
    if(cnt<1000) {
         mystep = 0.75;
         theta1desired = 0.0;
         theta2desired = 0.0;
         theta3desired = 0.0;
         theta1_int = 0;
         theta2_int = 0;
         theta3_int = 0;
     }
     else{
         mystep = 0.25;
         theta1desired = t1;
         theta2desired = t2;
         theta3desired = t3;
         theta1_int = 0;
         theta2_int = 0;
         theta3_int = 0;
     }
     if(cnt>2000){
          cnt=0;
     }
     cnt++;
}
    if ((mycount%500)==0) {
        if (whattoprint > 0.5) {
            serial_printf(&SerialA, "I love robotics\n\r");
        } else {
            printtheta1motor = theta1motor;
            printtheta2motor = theta2motor;
            printtheta3motor = theta3motor;

            forwardKinematics(theta1motor,theta2motor,theta3motor);

            //inverseKinematics(printx,printy,printz);


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

    Simulink_PlotVar4 = q;
  if(simval == 0){
    Simulink_PlotVar1 = theta1motor;
    Simulink_PlotVar2 = theta2motor;
    Simulink_PlotVar3 = theta3motor;

  }
  else{
    Simulink_PlotVar1 = error1;
    Simulink_PlotVar2 = error2;
    Simulink_PlotVar3 = error3;
  }

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

