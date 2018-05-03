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

	xgrip = xWgrip + 0.150;
	ygrip = yWgrip - 0.150;
	zgrip = zWgrip - 0.014;

    float yaw_WgripRad = yaw_WgripDegree*(PI/180);

    // ---------------------------------------------------------
    // Equation 1:
    // Input: theta_y (yaw desired)
    //        Xgrip, Ygrip, Zgrip (desired position of gripper)
    // Output: xcen, ycen, zcen
    xcen = xgrip - a6*cos(yaw_WgripRad);
    ycen = ygrip - a6*sin(yaw_WgripRad);
    zcen = zgrip;
    // ---------------------------------------------------------

    theta5 = - PI/2.0 ; // By definition

    // ---------------------------------------------------------
    // Equation 2:
    float A = sqrt(xcen*xcen + ycen*ycen);

    float theta_A = atan2(ycen, xcen);
    float theta_B = asin((d2+d3+d4)/A);

    theta1 = theta_A - theta_B;

    // cout << "Theta A: " << theta_A << " theta B: " << theta_B << endl;
    // ---------------------------------------------------------

    // ---------------------------------------------------------
    // Equation 3:
    theta6 = PI/2.0 + theta1 - yaw_WgripRad;
    // ---------------------------------------------------------

    // ---------------------------------------------------------
    // Equation 4:
    // Input: theta_1, xcen, ycen, zcen
    // Output: Y3end, X3end, Z3end
    y3end =  ycen - (d2+d3+d4)*cos(theta1) - d5*sin(theta1);
    x3end =  xcen - d5*cos(theta1) + (d2+d3+d4)*sin(theta1);
    z3end = zcen + d6;
    // ---------------------------------------------------------

    // ---------------------------------------------------------
    // Equation 5:
    // Input: X3end, Y3end, Z3end
    // Output: theta_2, theta_3, theta_4
    float C = sqrt(x3end*x3end + y3end*y3end + (z3end-d1)*(z3end-d1));
    // float theta_2_1 = asin((a2*a2 + C*C - .213*.213)/(2*.244*C));
    // float theta_2_2 = atan2(z3end-.152, sqrt(x3end*x3end + y3end*y3end));
    // theta2 = -theta_2_1 -theta_2_2;
    // theta3 = PI - asin((C*sin(theta_2_1))/.213);
    // theta4 = -PI/2.0 - theta3 - theta2;
    theta_a = asin((z3end-d1)/C);
    theta_b = acos((a2*a2 + C*C - a3*a3)/2.0*a2*C);
    theta_c = acos((a2*a2 + a3*a3 - C*C)/2.0*a2*a3);
    theta2 = -theta_a - theta_b;
    theta3 = PI - theta_c;
    theta4 = theta_a + theta_b + theta_c - 3.0*PI/2.0;

    // ---------------------------------------------------------

	// View values
	// use cout
	cout<<"theta1: "<< theta1<<endl;
	cout<<"theta2: "<< theta2<<endl;
	cout<<"theta3: "<< theta3<<endl;
	cout<<"theta4: "<< theta4<<endl;
	cout<<"theta5: "<< theta5<<endl;
	cout<<"theta6: "<< theta6<<endl;

    cout<<"huskies are awesome!"<<endl;

	// check that your values are good BEFORE sending commands to UR3
	//lab_fk calculates the forward kinematics and convert it to std::vector<double>
	return lab_fk((float)theta1,(float)theta2,(float)theta3,(float)theta4,(float)theta5,(float)theta6);
}
