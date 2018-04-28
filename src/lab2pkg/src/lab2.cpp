/*
KE-KT-KG presents:

          ECE470
_           _       ___
| |         | |     |__ \
| |     __ _| |__      ) |
| |    / _` | '_ \    / /
| |___| (_| | |_) |  / /_
|______\__,_|_.__/  |____|

*/

//Error https://pastebin.com/a2bfkaKE

#include "lab2pkg/lab2.h"
// #include "lab2_2.h"
#define PI 3.14159265359
#define SPIN_RATE 20  /* Hz */

// 3-d position lookup array to move robot to different positions
double position_lookup[4][4][6] = {
    {{169.94*PI/180,-85.70*PI/180,117.82*PI/180,-122.04*PI/180,-89.41*PI/180,78.84*PI/180}},    //Home (same as peg2_ready in TP)

    {{138.73*PI/180,-56.19*PI/180,124.45*PI/180,-157.16*PI/180,-89.55*PI/180,47.50*PI/180},     //peg1_0
     {138.73*PI/180,-64.64*PI/180,123.51*PI/180,-148.37*PI/180,-89.58*PI/180,47.52*PI/180},     //peg1_1
     {138.73*PI/180,-71.32*PI/180,121.50*PI/180,-139.67*PI/180,-89.58*PI/180,47.54*PI/180},     //peg1_2
     {138.73*PI/180,-82.27*PI/180,113.71*PI/180,-120.97*PI/180,-89.59*PI/180,47.60*PI/180}},    //peg1_3

    {{169.99*PI/180,-57.37*PI/180,128.94*PI/180,-161.49*PI/180,-89.367*PI/180,78.72*PI/180},     //peg2_0
     {169.97*PI/180,-66.12*PI/180,128.12*PI/180,-151.90*PI/180,-89.39*PI/180,78.75*PI/180},     //peg2_1
     {169.96*PI/180,-74.30*PI/180,125.77*PI/180,-141.37*PI/180,-89.41*PI/180,78.78*PI/180},     //peg2_2
     {169.94*PI/180,-85.70*PI/180,117.81*PI/180,-122.04*PI/180,-89.41*PI/180,78.84*PI/180},},   //peg2_3

    {{199.91*PI/180,-47.98*PI/180,106.78*PI/180,-149.13*PI/180,-89.40*PI/180,108.68*PI/180},  //peg3_0
     {199.89*PI/180,-55.18*PI/180,106.07*PI/180,-141.22*PI/180,-89.42*PI/180,108.71*PI/180},  //peg3_1
     {199.89*PI/180,-60.80*PI/180,104.27*PI/180,-133.80*PI/180,-89.43*PI/180,108.73*PI/180},  //peg3_2
     {199.87*PI/180,-69.58*PI/180,97.35*PI/180,-118.11*PI/180,-89.43*PI/180,108.78*PI/180}}  //peg3_3
};



// Global bool variables that are assigned in the callback associated when subscribed
// to the "ur3/position" topic
bool isReady=1;
bool pending=0;

// Global bool variables that are assigned in the callback associated when subscribed to
// the ur_msgs/IOStates
bool suctionStatus;

// Global int variables (which are better than enums) that determine start, temp, and end pegs
int start;
int temp;
int end;

// Functions are:

/* move_block
    description: this will pickup a block and drop it at another positions. fails if no block exists
    input: ros::Publisher pub_command -initialized publisher to publish to
           ros::Rate loop_rate        -initialized rate to publish ur3/commands
           ros::ServiceClient srv_SetIO  - initialized service to toggle suction_state
           ur_msgs::SetIO srv            - more service stuff
           int origin_peg
           int origin_height
           int destination_peg
           int destination_height
    output: 0 if successful, 1 if unsuccessful (block doesn't exist) or input invalid */
int move_block(ros::Publisher pub_command, ros::Rate loop_rate,
               ros::ServiceClient srv_SetIO, ur_msgs::SetIO srv,
               int origin_peg, int origin_height,
               int destination_peg, int destination_height);


/* move_to_point
    description: this will move the robot to specified point
    input: ros::Publisher pub_command -initialized publisher to publish to
           ros::Rate loop_rate        -initialized rate to publish ur3/commands
           double dest[]              -array containing the destination arrays
           float duration             -how long our move should take
    output: 0 if successful, 1 if unsuccessful or input invalid */
int move_to_point(ros::Publisher pub_command , ros::Rate loop_rate, double dest[], float duration);

/* set_suction
    description: this will turn the robot's suction on or off
    input: ros::ServiceClient srv_SetIO  - initialized service to toggle suction_state
           ur_msgs::SetIO srv            - more service stuff
           bool suction_state            - bool with value 0 (turn suction off) or 1 (turn suction on)
    output: 0 if successful, 1 if unsuccessful or input invalid */
int set_suction(ros::ServiceClient srv_SetIO, ur_msgs::SetIO srv, bool suction_state);

/* check_suction
    description: this will check if the robot's suction is attached to an object or not
    input: none
    output: 0 if there is nothing attached to suction, 1 if there is */
int check_suction();

// functions for callbacks
void position_callback(const ece470_ur3_driver::positions::ConstPtr& msg);
void suction_callback(const ur_msgs::IOStates::ConstPtr& msg);


//arrays defining Waypointts
double arr11[]={138.71*PI/180,-82.27*PI/180,113.72*PI/180,-120.96*PI/180,-89.59*PI/180,47.61*PI/180};

// array to define final velocity of point to point moves.  For now slow down to zero once
// each point is reached
double arrv[]={0,0,0,0,0,0};

//Var to track spins
int spincount = 0;


/* Main function
   Initializes necessary stuff for ros,
   Asks user for input of two pegs, then solves 3-block Hanoi Puzzle
*/
int main(int argc, char **argv)
{
    //initialization & variable definition
    ros::init(argc, argv, "lab2node");  //initialzation of ros required for each Node.
    ros::NodeHandle nh;             //handler for this node.

    //initialized publisher ur3/command, buffer size of 10.
    ros::Publisher pub_command=nh.advertise<ece470_ur3_driver::command>("ur3/command",10);
    // initialize subscriber to ur3/position and call function position_callback each time data is published
    ros::Subscriber sub_position=nh.subscribe("ur3/position",1,position_callback);
    // initialize subscriber to ur_driver/io_states
    ros::Subscriber sub_io=nh.subscribe("ur_driver/io_states",1,suction_callback);

    // setup the services for suction
    ros::ServiceClient srv_SetIO = nh.serviceClient<ur_msgs::SetIO>("ur_driver/set_io");
    ur_msgs::SetIO srv;

    while(!ros::ok()){};    //check if ros is ready for operation

    // initialize the loop_rate
    ros::Rate loop_rate(SPIN_RATE);

    std::string start_peg;
    while (true) {
        std::cout << "Enter start peg: ";
        std::getline(std::cin, start_peg);
        std::cout << "You entered " << start_peg << "\n";
        if (start_peg == "1") {
            start = 1;
            break;
        } else if (start_peg == "2") {
            start = 2;
            break;
        } else if (start_peg == "3") {
            start = 3;
            break;
        } else
            cout << "Please just enter 1 2 or 3\n\n";
    }

    std::string end_peg;
    while (true) {
        std::cout << "Enter end peg: ";
        std::getline(std::cin, end_peg);
        std::cout << "You entered " << end_peg << "\n";
        if (end_peg == "1") {
            end = 1;
            break;
        } else if (end_peg == "2") {
            end = 2;
            break;
        } else if (end_peg == "3") {
            end = 3;
            break;
        } else
            cout << "Please just enter 1 2 or 3\n\n";
    }

    if (start == 1 && end == 2)
        temp = 3;
    else if (start == 1 && end == 3)
        temp = 2;
    else if (start == 2 && end == 1)
        temp = 3;
    else if (start == 2 && end == 3)
        temp = 1;
    else if (start == 3 && end == 1)
        temp = 2;
    else if (start == 3 && end == 2)
        temp = 1;


    ROS_INFO("Starting moves");

    //Move to starting point
    move_to_point(pub_command, loop_rate, position_lookup[0][0], 3);

    //Solve Puzzle
    if (move_block(pub_command, loop_rate, srv_SetIO, srv, start, 2, end, 0))    // MOVE 1
        return 1;
    if (move_block(pub_command, loop_rate, srv_SetIO, srv, start, 1, temp, 0))   // MOVE 2
        return 2;
    if (move_block(pub_command, loop_rate, srv_SetIO, srv, end, 0, temp, 1))    // MOVE 3
        return 3;
    if (move_block(pub_command, loop_rate, srv_SetIO, srv, start, 0, end, 0))    // MOVE 4
        return 4;
    if (move_block(pub_command, loop_rate, srv_SetIO, srv, temp, 1, start, 0))    // MOVE 5
        return 5;
    if (move_block(pub_command, loop_rate, srv_SetIO, srv, temp, 0, end, 1))    // MOVE 6
        return 6;
    if (move_block(pub_command, loop_rate, srv_SetIO, srv, start, 0, end, 2))    // MOVE 7
        return 7;

    ROS_INFO("\n\nTa-Daaa!\n\n");

    return 0;
}


int move_block(ros::Publisher pub_command, ros::Rate loop_rate,
               ros::ServiceClient srv_SetIO, ur_msgs::SetIO srv,
               int origin_peg, int origin_height,
               int destination_peg, int destination_height)
{

    ROS_INFO("Moving to point %d, %d",destination_peg,destination_height);

   // Move robot to origin position
   move_to_point(pub_command, loop_rate, position_lookup[origin_peg][3], 0.5);
   move_to_point(pub_command, loop_rate, position_lookup[origin_peg][origin_height], 0.1);

   // Pickup block and move up
   // ROS_INFO("Suction on");
   set_suction(srv_SetIO, srv, 1);
   move_to_point(pub_command, loop_rate, position_lookup[origin_peg][3], 0.1);
   // std::cout << "suctionstatus: " << suctionStatus << "/n";
   if (!suctionStatus)     // if we don't have a block, fail
   {
       ROS_INFO("\n\nI'm afraid I can't do that, Dave\n\n");
       set_suction(srv_SetIO, srv, 0);
       return 1;
   }

   // Move robot to destination position
   move_to_point(pub_command, loop_rate, position_lookup[destination_peg][3], 0.5);
   move_to_point(pub_command, loop_rate, position_lookup[destination_peg][destination_height], 0.1);

   // drop move_block
   // ROS_INFO("Suction off");
   set_suction(srv_SetIO, srv, 0);

   // Raise robot arm above end position to prepare for next movement
   move_to_point(pub_command, loop_rate, position_lookup[destination_peg][3], 0.1);

   return 0;
}

/* move_to_point
    description: this will move the robot to specified point
    input: ros::Publisher pub_command -initialized publisher to publish to
           ros::Rate loop_rate        -initialized rate to publish ur3/commands
           double dest[]              -array containing the destination arrays
           float duration             -how long our move should take
    output: 0 if successful, 1 if unsuccessful or input invalid */
int move_to_point(ros::Publisher pub_command , ros::Rate loop_rate, double dest[], float duration)
{
    // array to define final velocity of point to point moves.  For now slow down to zero once dest reached
    double zero[]={0,0,0,0,0,0};

    //vector to be used to publish commands to UR3 ROS Driver (ece470_ur3_driver)
    std::vector<double> dest_vector (dest,dest+48 / sizeof(double));
    std::vector<double> dest_velocity (zero,zero+48 / sizeof(double));
    // transform(dest_vector.begin(), dest_vector.end(), dest_vector.begin(), PI/180);


    // initialize driver message to fill in and publish
    ece470_ur3_driver::command driver_msg;

    driver_msg.destination = dest_vector;
    driver_msg.duration = duration;

    // publish command, but note that is possible that subscriber will not receive this message.
    pub_command.publish(driver_msg);

    // retry if the subscriber doesn't get the initial command
    spincount = 0;
    while (isReady) { // Waiting for isReady to be false meaning that the driver has the new command
        ros::spinOnce();  // Allow other ROS functionallity to run
        loop_rate.sleep(); // Sleep and wake up at 1/20 second (1/SPIN_RATE) interval
        if (spincount > SPIN_RATE) {  // if isReady does not get set within 1 second re-publish
            pub_command.publish(driver_msg);
            ROS_INFO("Just Published again driver_msg");
            spincount = 0;
        }
        spincount++;  // keep track of loop count
    }

    ROS_INFO("waiting for rdy");  // Now wait for robot arm to reach the commanded waypoint.
    while(!isReady)
    {
        ros::spinOnce();
        loop_rate.sleep();
    }

    // If the robot arm is finished, return successful
    return 0;
}


/* set_suction
    description: this will turn the robot's suction on or off
    input: integer with value either 0 (to turn suction off) or 1 (to turn suction on)
    output: 0 if successful, 1 if unsuccessful or input invalid */
int set_suction(ros::ServiceClient srv_SetIO, ur_msgs::SetIO srv, bool suction_state)
{
    if (suction_state) {
        srv.request.fun = 1;  //Set function active
        srv.request.pin = 0;  //Digital Output 0
        srv.request.state = 1.0; //Set DO0 on
        srv_SetIO.call(srv); //Call to IO

    } else {
        srv.request.fun = 1;
        srv.request.pin = 0; // Digital Output 0
        srv.request.state = 0.0; //Set DO0 off
        srv_SetIO.call(srv); //Call to IO
    }
}

// Whenever ur3/position publishes info this callback function is run.
void position_callback(const ece470_ur3_driver::positions::ConstPtr& msg)
{
    isReady=msg->isReady; // When isReady is True the robot arm has made it to its desired position
                          // and is ready to be told to go to another point if desired.
    pending=msg->pending; // pending is the opposite of isReady, pending is true until a new position is reached
//  ROS_INFO("Debug isRdy = %d, pending = %d",(int)isReady,(int)pending);
}

// whenever ur_driver/io_states publishes something, this callback function is run
// "rostopic info /ur3/position" to find the message type
// "rosmsg show ur_msgs/IOStates" to find out what's in the msg
void suction_callback(const ur_msgs::IOStates::ConstPtr& msg)
{
    float suction_analog = msg->analog_in_states[0].state;
    if (suction_analog > 2.0)
        suctionStatus = true;
    else
        suctionStatus = false;
//  std::cout << suction_analog << "/n";
}
