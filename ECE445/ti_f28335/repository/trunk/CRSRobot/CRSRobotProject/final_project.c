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
float tau1p = 0;
float tau2p = 0;
float tau3p = 0;

int counter=0;
float theta1_prev = 0;
float theta2_prev = 0;
float theta3_prev = 0;
float theta1_dot_prev[2] = {0,0};
float theta2_dot_prev[2] = {0,0};
float theta3_dot_prev[2] = {0,0};
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

int i;
long cnt = 0;
int controller = 0;
float t1=0;
float t2=0;
float t3=0;

float q = 0;
float qd= 0;
float qdd= 0;

float error1=0;
float error2=0;
float error3=0;
float error1_prev=0;
float error2_prev=0;
float error3_prev=0;

float theta1desired = 0;
float theta2desired = 0;
float theta3desired = 0;
float x = 0;
float y = 0;
float z = 0;

float q1 =0;
float q2 = 0;
float q3 = 0;

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
float lambda = 1.0;

float D2 = 0;
float D3 = 0;
float C2 = 0;
float C3 = 0;
float G2 = 0;
float G3 = 0;

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
float J11 = 0;
float J12 = 0;
float J13 = 0;
float J21 = 0;
float J22 = 0;
float J23 = 0;
float J31 = 0;
float J32 = 0;
float J33 = 0;

float velocity = 0;
int total_time = 0;
int direction = 0;
int started = 0;
int start_cnt = 0;
float xd = 10;
float yd = -10;
float zd = 10;
float deltax=0;
float deltay=0;
float deltaz=0;
float delta = 0;
float x_dot = 0;
float y_dot = 0;
float z_dot = 0;
float zd_dot = 0;
float yd_dot = 0;
float xd_dot = 0;
float startx = 0;
float starty = 0;
float startz = 0;
float endx = 0;
float endy = 0;
float endz = 0;
// Kp gains
float Kpxn = 0.5;
float Kpyn = 0.5;
float Kpzn = 0.5;
// Kd gains
float Kdxn = 0.025;
float Kdyn = 0.025;
float Kdzn = 0.025;
// To filter x_dot, y_dot, z_dot
float x_prev = 0;
float y_prev = 0;
float z_prev = 0;
float x_dot_prev[2];
float y_dot_prev[2];
float z_dot_prev[2];
// Kp*Rn_w*error + Kd*Rn_w*error_dot
float inside_mult1 = 0;
float inside_mult2 = 0;
float inside_mult3 = 0;
int robot_state = -1;
float xbound = 0;
float ybound = 0;
float zbound = 0;
float total_timex = 0;
float total_timey = 0;
float total_timez = 0;
float zzz = 8.45;
float noegg = 39;
float zz_speed = 20;
int egg_cnt = 0;
int competition_time = 0;
int competition_counter = 0;
int time_recorded = 0;

void printing(void);
void forwardKinematics(float theta1motor,float theta2motor,float theta3motor);
void inverseKinematics(float x, float y, float z);
void setK(float xp, float yp, float zp, float xd, float yd, float zd);
void linetrajectory(float startx, float starty, float startz, float endx, float endy, float endz, float velocity );
void setEndpoint(float ex, float ey, float ez);
void setAngles(float t1,float t2,float t3);

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


// This function is called every 1 ms
void lab(float theta1motor,float theta2motor,float theta3motor,float *tau1,float *tau2,float *tau3, int error) {

    // Calculate theta_dot
    theta1_dot = (theta1motor - theta1_prev)/0.001;
    theta2_dot = (theta2motor - theta2_prev)/0.001;
    theta3_dot = (theta3motor - theta3_prev)/0.001;

    // Filter theta_dot
    theta1_dot_avg = (theta1_dot_prev[0] + theta1_dot_prev[1] + theta1_dot)/3.0;
    theta2_dot_avg = (theta2_dot_prev[0] + theta2_dot_prev[1] + theta2_dot)/3.0;
    theta3_dot_avg = (theta3_dot_prev[0] + theta3_dot_prev[1] + theta3_dot)/3.0;


    theta1_dot_prev[1] = theta1_dot_prev[0];
    theta2_dot_prev[1] = theta2_dot_prev[0];
    theta3_dot_prev[1] = theta3_dot_prev[0];

    theta1_dot_prev[0] = theta1_dot_avg;
    theta2_dot_prev[0] = theta2_dot_avg;
    theta3_dot_prev[0] = theta3_dot_avg;

    theta1_prev = theta1motor;
    theta2_prev = theta2motor;
    theta3_prev = theta3motor;

    // Jacobian Transpose
    cosq1 = cos(theta1motor);
    sinq1 = sin(theta1motor);
    cosq2 = cos(theta2motor);
    sinq2 = sin(theta2motor);
    cosq3 = cos(theta3motor);
    sinq3 = sin(theta3motor);
    JT_11 = J11= -10*sinq1*(cosq3 + sinq2);
    JT_12 = J21 = 10*cosq1*(cosq3 + sinq2);
    JT_13 = J31 = 0;
    JT_21 = J12 = 10*cosq1*(cosq2 - sinq3);
    JT_22 = J22 = 10*sinq1*(cosq2 - sinq3);
    JT_23 = J32 = -10*(cosq3 + sinq2);
    JT_31 = J13 = -10*cosq1*sinq3;
    JT_32 = J23 = -10*sinq1*sinq3;
    JT_33 = J33 = -10*cosq3;

    // x y and z Forward Kinematics
    x = 10*cosq1*(cosq3+sinq2);
    y = 10*sinq1*(cosq3+sinq2);
    z = 10*(1+cosq2-sinq3);

    // Calculate position_dot
    x_dot = (x - x_prev)/0.001;
    y_dot = (y - y_prev)/0.001;
    z_dot = (z - z_prev)/0.001;

    // Filter position
    x_dot = (x_dot_prev[0] + x_dot_prev[1] + x_dot)/3.0;
    y_dot = (y_dot_prev[0] + y_dot_prev[1] + y_dot)/3.0;
    z_dot = (z_dot_prev[0] + z_dot_prev[1] + z_dot)/3.0;

    x_dot_prev[1] = x_dot_prev[0];
    y_dot_prev[1] = y_dot_prev[0];
    z_dot_prev[1] = z_dot_prev[0];

    x_dot_prev[0] = x_dot;
    y_dot_prev[0] = y_dot;
    z_dot_prev[0] = z_dot;

    x_prev = x;
    y_prev = y;
    z_prev = z;


    ////// Different Controllers for lab 4 //////
    /*  CONTROLLER == 0: Does nothing
     *          To make testing easier, controller was initialized to 0 so that when the robot was turned
     *          on it would not move
     *  CONTROLLER == 1: Task Space PD control with simple impedance control
     *          The user will
     *  */



    if(controller == 0){
        // NOTHING
        //// so robot doesn't move as soon as it's turned on
        *tau1 = 0;
        *tau2 = 0;
        *tau3 = 0;
    }
    else if(controller == 1){
        competition_counter++;
        switch(robot_state)
        {
        case -1:
            startx = 10*cosq1*(cosq3+sinq2);
            starty = 10*sinq1*(cosq3+sinq2);
            startz = 10*(1+cosq2-sinq3);
            robot_state++;
            break;
        case 0: // start position
            setEndpoint(9,0,15);
            velocity = noegg;
            setK(0.5, 0.5, 0.5, 0.025, 0.025, 0.025);
            setAngles(0,0,0);
            break;
        case 1: // start to above hole
            setEndpoint(1.4,13.9,10);
            velocity = noegg;
            setK(1, 1, 1.5, 0.025, 0.025, 0.025);
            setAngles(0,0,0);
            break;
        case 2: // above hole to in hole
            setEndpoint(1.36,13.9,4.5);
            velocity = noegg;
            setK(0.05, 0.05, 1.5, 0.025, 0.025, 0.025);
            setAngles(0,0,0);
            break;
        case 3: // in hole to above hole
            setEndpoint(1.36,13.9,10);
            velocity = noegg;
            setK(1, 1, 1, 0.025, 0.025, 0.025);
            setAngles(0,0,0);
            break;
        case 4: // above hole to around obstacle
            setEndpoint(12,2.5,12);
            velocity = noegg*1.5;
            setK(0.5, 0.5, 0.5, 0.025, 0.025, 0.025);
            setAngles(0,0,0);
            break;
        case 5: // around obstacle to start zigzag
            setEndpoint(15,4,11);
            velocity = zz_speed;
            setK(1, 1, 0.5, 0.025, 0.025, 0.025);
            setAngles(0,0,0);
            break;
        case 6: // around obstacle to start zigzag
            setEndpoint(15,4,zzz);
            velocity = zz_speed;
            setK(0.5, 0.5, 0.5, 0.025, 0.025, 0.025);
            setAngles(0,0,0);
            break;
        case 7: // start zz to corner 1
            setEndpoint(16,2,zzz);
            velocity = zz_speed;
            setK(1, 0.1, 0.5, 0.025, 0.025, 0.025);
            setAngles(0,0,-54);
            break;
        case 8: // corner 1 to corner 2
            setEndpoint(15.3,1.4,zzz);
            velocity = zz_speed;
            setK(1, 0.1, 0.5, 0.025, 0.025, 0.025);
            setAngles(0,0,46);
            break;
        case 9: // corner 2 to corner 3
            setEndpoint(12.75,1.75,zzz);
            velocity = zz_speed;
            setK(1, 0.1, 0.5, 0.025, 0.025, 0.025);
            setAngles(0,0,-15);
            break;
        case 10: // corner 3 to corner 4
            setEndpoint(12.89,1.15,zzz);
            velocity = zz_speed;
            setK(1, 0.1, 0.5, 0.025, 0.025, 0.025);
            setAngles(0,0,46);
            break;
        case 11: // corner 4 to corner 5
            setEndpoint(14.8,-1.34,zzz);
            velocity = noegg;
            setK(1, 0.1, 0.5, 0.025, 0.025, 0.025);
            setAngles(0,0,-54);
            break;
        case 12: // corner 5 to past zz
            setEndpoint(15.72,-3,zzz);
            velocity = noegg;
            setK(1, 0.1, 0.5, 0.025, 0.025, 0.025);
            setAngles(0,0,0);
            break;
        case 13: // past zz to above past zz
            setEndpoint(15.72,-3,14.5);
            velocity = noegg;
            setK(0.7, 0.5, 0.5, 0.025, 0.025, 0.025);
            setAngles(0,0,0);
            break;
        case 14: // above past zz to above egg
            setEndpoint(14.6,-5.4,15);
            velocity = noegg;
            setK(0.7, 0.7, 0.5, 0.025, 0.025, 0.025);
            setAngles(0,0,0);
            break;
        case 15: // above egg to pushing egg
            setEndpoint(14.6,-5.4,13.65);
            velocity = 7;
            setK(0.7, 0.7, 0.05, 0.025, 0.025, 0.025);
            setAngles(0,0,0);
            break;
        case 16: // wait at egg
            if(egg_cnt==900){
                egg_cnt = 0;
                robot_state++;
            }
            egg_cnt++;
            break;
        case 17:
            break;
        case 18: // pushing egg to above egg
            setEndpoint(14.6,-5.4,15);
            velocity = noegg;
            setK(0.7, 0.7, 0.5, 0.025, 0.025, 0.025);
            setAngles(0,0,0);
            break;
        case 19: // above egg to start
            setEndpoint(9,0,15);
            velocity = noegg*3;
            setK(0.5, 0.5, 0.5, 0.025, 0.025, 0.025);
            setAngles(0,0,0);
            break;

        }
        if(robot_state != 16)
        {
            linetrajectory(startx, starty,startz,endx,endy,endz,velocity);
        }


        cosz = cos(thetaz);
        sinz = sin(thetaz);
        cosx = cos(thetax);
        sinx = sin(thetax);
        cosy = cos(thetay);
        siny = sin(thetay);
        RT11  = cosz*cosy-sinz*sinx*siny;
        RT21  = -sinz*cosx;
        RT31  = cosz*siny+sinz*sinx*cosy;
        RT12  = sinz*cosy+cosz*sinx*siny;
        RT22  = cosz*cosx;
        RT32  = sinz*siny-cosz*sinx*cosy;
        RT13  = -cosx*siny;
        RT23  = sinx;
        RT33  = cosx*cosy;
        R11 = RT11;
        R12 = RT21;
        R13 = RT31;
        R21 = RT12;
        R22 = RT22;
        R23 = RT32;
        R31 = RT13;
        R32 = RT23;
        R33 = RT33;

        inside_mult1 = Kpxn*RT11*(xd-x) + Kpxn*RT12*(yd-y) + RT13*Kpxn*(zd-z) + Kdxn*RT11*(xd_dot-x_dot) + Kdxn*RT12*(yd_dot-y_dot) + RT13*Kdxn*(zd_dot-z_dot);
        inside_mult2 = Kpyn*RT21*(xd-x) + Kpyn*RT22*(yd-y) + RT23*Kpyn*(zd-z) + Kdyn*RT21*(xd_dot-x_dot) + Kdyn*RT22*(yd_dot-y_dot) + RT23*Kdyn*(zd_dot-z_dot);
        inside_mult3 = Kpzn*RT31*(xd-x) + Kpzn*RT32*(yd-y) + RT33*Kpzn*(zd-z) + Kdzn*RT31*(xd_dot-x_dot) + Kdzn*RT32*(yd_dot-y_dot) + RT33*Kdzn*(zd_dot-z_dot);

        *tau1 = tau1p = (JT_11*R11 + JT_12*R21 + JT_13*R31)*inside_mult1 + (JT_11*R12 + JT_12*R22 + JT_13*R32)*inside_mult2 + (JT_11*R13 + JT_12*R23 + JT_13*R33)*inside_mult3;
        *tau2 = tau2p = (JT_21*R11 + JT_22*R21 + JT_23*R31)*inside_mult1 + (JT_21*R12 + JT_22*R22 + JT_23*R32)*inside_mult2 + (JT_21*R13 + JT_22*R23 + JT_23*R33)*inside_mult3;
        *tau3 = tau3p = (JT_31*R11 + JT_32*R21 + JT_33*R31)*inside_mult1 + (JT_31*R12 + JT_32*R22 + JT_33*R32)*inside_mult2 + (JT_31*R13 + JT_32*R23 + JT_33*R33)*inside_mult3;

    }

    // friction compensation for joint 1
    if(theta1_dot_avg > 0.1){
        *tau1 = *tau1 + lambda*(Vpos1*theta1_dot_avg + Cpos1);
    }
    else if(fabs(theta1_dot_avg)<=0.1){
        *tau1 = *tau1 + lambda*(Vmid1*theta1_dot_avg + Cmid1);
    }
    else{
        //theta1_dot_avg < -0.1
        *tau1 = *tau1 + lambda*(Vneg1*theta1_dot_avg + Cneg1);
    }

    // friction compensation for joint 2
    if(theta2_dot_avg > 0.05){
        *tau2 = *tau2 + lambda*(Vpos2*theta2_dot_avg + Cpos2);
    }
    else if(fabs(theta2_dot_avg)<=0.05){
        *tau2 = *tau2 + lambda*(Vmid2*theta2_dot_avg);
    }
    else{
        //theta2_dot_avg < -0.05
        *tau2 = *tau2 + lambda*(Vneg2*theta1_dot_avg + Cneg2);
    }


    // friction compensation for joint 3
    if(theta3_dot_avg > 0.05){
        *tau3 = *tau3 + lambda*(Vpos3*theta3_dot_avg + Cpos3);
    }
    else if(fabs(theta3_dot_avg)<=0.05){
        *tau3 = *tau3 + lambda*(Vmid3*theta3_dot_avg);
    }
    else{
        //theta3_dot_avg < -0.05
        *tau3 = *tau3 + lambda*(Vneg3*theta3_dot_avg + Cneg3);
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

    cnt++;

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
        Simulink_PlotVar1 = tau1p;
        Simulink_PlotVar2 = tau2p;
        Simulink_PlotVar3 = tau3p;

    }
    else{
        Simulink_PlotVar1 = inside_mult1;
        Simulink_PlotVar2 = inside_mult2;
        Simulink_PlotVar3 = inside_mult3;
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

void setK(float xp, float yp, float zp, float xd, float yd, float zd)
{
    Kpxn = xp;
    Kpyn = yp;
    Kpzn = zp;
    Kdxn = xd;
    Kdyn = yd;
    Kdzn = zd;
}

void setAngles(float t1,float t2,float t3)
{
    thetax = t1*PI/180;
    thetay = t2*PI/180;
    thetaz = t3*PI/180;
}

void setEndpoint(float ex, float ey, float ez)
{
    endx = ex;
    endy = ey;
    endz = ez;
}

void updateStart()
{
    startx = endx;
    starty = endy;
    startz = endz;
}

void linetrajectory(float startx, float starty, float startz, float endx, float endy, float endz, float velocity ){
    if(!started){
        start_cnt = cnt = 0;
        started = 1;
    }
    xbound = endx-startx;
    ybound = endy-starty;
    zbound = endz-startz;

    delta = sqrt(xbound*xbound + ybound*ybound + zbound*zbound);
    total_time = 1000.0*(fabs(delta)/velocity);

    xd = xbound*((float)(cnt-start_cnt)/total_time) + startx;
    yd = ybound*((float)(cnt-start_cnt)/total_time) + starty;
    zd = zbound*((float)(cnt-start_cnt)/total_time ) + startz;

    x = 10*cosq1*(cosq3+sinq2);
    y = 10*sinq1*(cosq3+sinq2);
    z = 10*(1+cosq2-sinq3);
    if(cnt-start_cnt > total_time)
    {
        cnt = 0;
        start_cnt = cnt;
        updateStart();
        if(robot_state < 19){
            robot_state++;
        }
        else if(robot_state == 19 && !time_recorded){
            competition_time = competition_counter;
            time_recorded = 1;
        }
    }
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
    if(competition_time > 0){
        serial_printf(&SerialA, "Total Time: %.3f\n\r",(float)competition_time/1000.0);
    }
    //serial_printf(&SerialA, "%.2f %.2f %.2f  \n\r %.2f %.2f %.2f  \n\r  %.2f %.2f %.2f \n\r \n\r",printx,printy, printz,printtheta1motor*180/PI,printtheta2motor*180/PI,printtheta3motor*180/PI, theta1calc_m*180/PI,theta2calc_m*180/PI,theta3calc_m*180/PI);
    //serial_printf(&SerialA, "%.2f %.2f %.2f  \n\r",printtheta1motor*180/PI,printtheta2motor*180/PI,printtheta3motor*180/PI);
    //serial_printf(&SerialA, "hi\n\r");
}

