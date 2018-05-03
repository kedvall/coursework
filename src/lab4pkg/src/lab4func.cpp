#include "lab4pkg/lab4.h"

float get_theta6(float theta1, float yaw_WgripRad);
float get_theta1(float xcen, float ycen);

/**
 * function that calculates an elbow up Inverse Kinematic solution for the UR3
 */
std::vector<double> lab_invk(float xWgrip, float yWgrip, float zWgrip, float yaw_WgripDegree)
{

    double xcen,ycen,zcen,theta6,theta5,theta4,theta3,theta2,theta1,x3end,y3end,z3end; 
    double xgrip,ygrip,zgrip;
    double a1,a2,a3,a4,a5,a6;
    double d1,d2,d3,d4,d5,d6;
    double c1,beta1,beta2,beta3,l4,l5;

    a1 = 0;
    d1 = 0.152;
    a2 = 0.244;
    d2 = 0.120;
    a3 = 0.213;
    d3 = -0.093;
    a4 = 0;
    d4 = 0.083;
    a5 = 0;
    d5 = 0.083;
    a6 = 0.0535;
    d6 = (0.082+0.056);
    c1 = (d2+d3+d4);

    xgrip = xWgrip + 0.149;
    ygrip = yWgrip - 0.149;
    zgrip = zWgrip - 0.0193;

    xcen = xgrip - a6*cos(yaw_WgripDegree*(PI/180.0));
    ycen = ygrip - a6*sin(yaw_WgripDegree*(PI/180.0));
    zcen = zgrip; 

    
    theta1 = atan2(ycen,xcen) - asin(c1/sqrt( xcen*xcen + ycen*ycen )); 
    theta6 = PI/2.0 + theta1 - yaw_WgripDegree*(PI/180.0); 
 
    x3end = xcen + c1*sin(theta1) - d5*cos(theta1);
    y3end = ycen - d5*sin(theta1) - c1*cos(theta1);
    z3end = zcen + d6;

    l5 = sqrt(x3end*x3end + y3end*y3end);
    l4 = sqrt(l5*l5 + (z3end-d1)*(z3end-d1));
    beta1 = asin((z3end-d1)/l4);

    // --------- PROBLEM HERE ----------PROBLEM HERE---------------// 
    beta2 = acos((a2*a2 + l4*l4 - a3*a3) / (2.0*a2*l4));
    // --------- PROBLEM HERE ----------PROBLEM HERE---------------// 
    
    beta3 = acos((a2*a2 + a3*a3 - l4*l4) / (2.0*a2*a3));
    
    theta2= -(beta1 + beta2); 
    theta3= PI - beta3;
    theta4= beta1 + beta2 + beta3 - 3.0*PI/2.0;
    theta5=-PI/2.0;



    // View values
    //use cout
    cout<<"quat: "<< (a2*a2 + l4*l4 - a3*a3) / (2*a2*l4)<<endl;
    cout<<"theta1: "<< theta1<<endl;
    cout<<"l4: "<< l4<<endl;
    cout<<"l5: "<< l5<<endl;
    cout<<"beta1: "<< beta1<<endl;
    cout<<"beta2: "<< beta2<<endl;
    cout<<"theta2: "<< theta2<<endl;
    cout<<"theta3: "<< theta3<<endl;
    cout<<"theta4: "<< theta4<<endl;
    cout<<"theta5: "<< theta5<<endl;
    cout<<"theta6: "<< theta6<<endl;

    // check that your values are good BEFORE sending commands to UR3
    //lab_fk calculates the forward kinematics and convert it to std::vector<double>
    return lab_fk((float)theta1,(float)theta2,(float)theta3,(float)theta4,(float)theta5,(float)theta6);
}
