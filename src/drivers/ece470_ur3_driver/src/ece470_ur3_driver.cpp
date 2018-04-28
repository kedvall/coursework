/* ****************************************
*	ece470_ur3_driver:
		This is the driver to talk to ur_modern_driver via 
		actionlib::SimpleActionClient. The message type
		for the action client is 
		control_msgs::FollowJointTrajectoryAction.
		
		SimpleActionClient have some requirements for the
		messages it receives:
		1. The joint_names for the trajectores must be 
			specified and matches the joint names 
			in the joint_states.
		2. The target position and velocity must be 
			specified.
		3. The first target position/velocity must match
			the current position/velocity from the
			joint_states.
		4. Every target points must have a specific
			time_from_start and the first point matching
			the current position/velocity must have
			the duration set to zero.
		The message can have as many target points as one
		wants and the SimpleActionClient will execute the
		all the trajectory points.
		
		However, I do not recommend sending any 
		messages to the client before the current goal is
		reached.
******************************************************/

/*		WHATS UNDER CONSTRUCTION:*************************
			The ur3/position publisher does not publish while
			the ctrl_sub_callback is being executed. This is
			due to the function "client->waitForResult", the
			function will halt the entire program and wait 
			till the time specified expires or the goal result
			is returned.
			
			So student is encouraged to use the /joint_states
			instead.
			
			I was trying to write a publisher to publish the
			current state of ctrl_sub_callback, so student 
			can tell if its busy or not. 

************************************************************/

/**   About trajectory_msgs/JointTrajectory*******************
	names of the wrists, They are std::vector.
	if you type:
	rosmsg show trajectory_msgs/JointTrajectory
	This will show up in your command prompt:
	std_msgs/Header header
	  uint32 seq
	  time stamp
	  string frame_id
	string[] joint_names
	trajectory_msgs/JointTrajectoryPoint[] points
	  float64[] positions
	  float64[] velocities
	  float64[] accelerations
	  float64[] effort
	  duration time_from_start
	Let me explain how to intepret this output, first, this whole printout can be
	seen as a layout of a class, each is a variable. Let's assume we created a
	variable called trajectories using type trajectory_msgs::JointTrajectory.
	Then we have trajectories.header, under class type std_msgs/Header.
	and inside trajectories.header, we have trajectories.header.seq, 
	trajectories.header.stamp and trajectories.header.frame_id, with its respective
	variable type uint32, time and string.

	we also have trajectories.joint_names, but this time, in the command prompt we can 
	see its of type string[]. The square bracket indicates this variable is a vector 
	of strings, as in string[0], string[1] etc. and is under the type of 
	std::vector<string>.

	The rest is in similar manner.


	youbot@ros04:~/catkin_jifei$ rosmsg show control_msgs/FollowJointTrajectoryActionGoal 
	std_msgs/Header header
	  uint32 seq
	  time stamp
	  string frame_id
	actionlib_msgs/GoalID goal_id
	  time stamp
	  string id
	control_msgs/FollowJointTrajectoryGoal goal
	  trajectory_msgs/JointTrajectory trajectory
	    std_msgs/Header header
	      uint32 seq
	      time stamp
	      string frame_id
	    string[] joint_names
	    trajectory_msgs/JointTrajectoryPoint[] points
	      float64[] positions
	      float64[] velocities
	      float64[] accelerations
	      float64[] effort
	      duration time_from_start
	  control_msgs/JointTolerance[] path_tolerance
	    string name
	    float64 position
	    float64 velocity
	    float64 acceleration
	  control_msgs/JointTolerance[] goal_tolerance
	    string name
	    float64 position
	    float64 velocity
	    float64 acceleration
	  duration goal_time_tolerance

***************************************************/

		//see ece470_ur3_driver.h in include folder.
#include "ece470_ur3_driver/ece470_ur3_driver.h" 
#include <actionlib/client/simple_client_goal_state.h>
#include "ros/ros.h"
#include <string>
#include <iostream>
#include <cstdio>
#include <unistd.h>

/*	ur3_comm class function: CONSTRUCTOR *************************************
		the constructor initializes publishers, subscribers,
		the joint_names for the trajectories message 
		and the velocity vector for each trajectory points.
******************************************************************************/

ur3_comm::ur3_comm(){
	//initialize all subscribers and publishers
	ctrl_sub=nh.subscribe("ur3/command",100,&ur3_comm::ctrl_sub_callback,this); 
	state_sub=nh.subscribe("joint_states",100,&ur3_comm::state_sub_callback,this);
	pos_pub=nh.advertise<ece470_ur3_driver::positions>("ur3/position",10);
	// initialze SimpleActionClient, the "client" is typedef'd in header file
	client= new TrajClient(nh,"follow_joint_trajectory",true);
	//wait for the action server to go online. Need to have the robot ON.
	while(!client->waitForServer(ros::Duration(2.0))){
		ROS_INFO("Waiting for the joint_trajectory_action server");
	}
	ROS_INFO("connected to server");
	//Catch one joint_states message to initialize the message.
	joint_states= *ros::topic::waitForMessage<sensor_msgs::JointState>("joint_states");
	ROS_INFO("initializing joint names");
	trajectories.joint_names.push_back("shoulder_pan_joint");
	trajectories.joint_names.push_back("shoulder_lift_joint");
	trajectories.joint_names.push_back("elbow_joint");
	trajectories.joint_names.push_back("wrist_1_joint");
	trajectories.joint_names.push_back("wrist_2_joint");
	trajectories.joint_names.push_back("wrist_3_joint");
	ROS_INFO("initialization complete");
	//Initialze the velocity vector to all zeros. Since we only specify the
	//start point and end point velocity, hence zeros.
	double arr4[]={0,0,0,0,0,0};
	v.insert(v.end(), &arr4[0], &arr4[6]); //velocity
	isRdy=true;
	pending=false;
}

/*	ur3_comm class function: DESTRUCTOR **************************************
		delete the client, release the memory.

******************************************************************************/
ur3_comm::~ur3_comm(){
	delete client;
}

/*	ur3_comm class function: ctrl_sub_callback *******************************
		Callback function for the subscriber that subscribe to "ur3/command"
		topic. This function translates the msg sent to "ur3/command" by the 
		student to msg that can be used by the Action client.
******************************************************************************/
void ur3_comm::ctrl_sub_callback(const ece470_ur3_driver::command::ConstPtr& msg)
{
	// check the duration set by the student is within range 1-5,
	// "0" is the default value if the value is not set by the
	// student.
	float duration;
	if(msg->duration ==0)
		duration=3;
	else
	{	
		duration=msg->duration;
		if (duration < 1)
		{
			ROS_INFO("Duration setting too low, reset to 1.");
			duration =1;
		}
		if (duration >5)
		{
			ROS_INFO("Duration setting too high, reset to 5.");
			duration =5;
		}
	}
	ROS_INFO("SETTING isRdy to FALSE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
	isRdy=false;		// This is variable under CONSTRUCTION
	pending=1;  // Dan Block moved here
	
	// check the student sent a vector of size 6.
	if(msg->destination.size() !=6){
		ROS_INFO("WARNNING: In ctrl_sub_callback- received command size is not 6");
		return;
	}
	// recieve one message from "joint_states" to get the latest position/velocity
	// values of the robot. This value is then used to set the first goal point.
	joint_states= *ros::topic::waitForMessage<sensor_msgs::JointState>("joint_states");
	int num=6;
	trajectories.header.stamp=ros::Time::now(); //set the header time stamp to current time.
	trajectories.points.resize(2);				// resize the point to 2 for start point and end point.
	trajectories.points[0].positions=joint_states.position;		// set first point to current state.
	trajectories.points[1].positions=msg->destination;			// set the second point to destination.
	trajectories.points[0].velocities= joint_states.velocity;	
	trajectories.points[1].velocities= v;
	trajectories.points[0].time_from_start=ros::Duration(0);	// set 1st time_from_start to 0
	trajectories.points[1].time_from_start=ros::Duration(duration);	// 2nd time to desired time from start.
 	desired_goal.trajectory=trajectories;						// put the trajectory into the client msg.
 	desired_goal.trajectory.header.stamp=ros::Time::now()+ros::Duration(0);	//set the client msg header stamp.
	ROS_INFO("Sending Goal..");
 	client->sendGoal(desired_goal);								//send the trajectory
 //	pending=1;
	ROS_INFO("Waiting for goal to finish..");
	bool goal_result=client->waitForResult(ros::Duration(duration+1));	// wait for the robot to reach the goal

	actionlib::SimpleClientGoalState clientstate = client->getState();

	ROS_INFO("State = %s",clientstate.toString().c_str());
	ROS_INFO("State = %d",(int)clientstate.state_);
	ROS_INFO_STREAM(clientstate.toString());
	if (goal_result==0){
		ROS_INFO("Goal did not finish in time, cancel ALL goal..");		//did not reach the goal.
		client->cancelAllGoals();										//cancel the current goal.
	}
	else{
		ROS_INFO("Goal reached..");
	}
	pending=0;
	ROS_INFO("Ready to receive new command..");
	isRdy=true;
}

/*	ur3_comm class function: state_sub_callback *******************************
		subscriber callback for joint_states topic.
		This is used for CONSTRUCTION purpose.
		Was trying to achieve publishing the state via ur3/position
		however, this function will not run during the execution of ctrl_sub_pub
		
		UPDATE: using multi-threading may solve the problem. (testing)
******************************************************************************/
void ur3_comm::state_sub_callback(const sensor_msgs::JointState::ConstPtr& msg)
{
	joint_states=*msg;	
	pos_msg.position=joint_states.position;
	pos_msg.velocity=joint_states.velocity;
	pos_msg.grip=1;
	pos_msg.isReady=isRdy;
	pos_msg.pending=pending;
//	ROS_INFO("Dan isRdy = %d, pending = %d",(int)isRdy,(int)pending);
//	pos_msg.state=(client->getState()).toString(); 	//Possible states are: PENDING, ACTIVE, RECALLED, REJECTED, PREEMPTED, ABORTED, SUCCEEDED, LOST
													//The goal state return LOST if this SimpleActionClient isn't tracking anything.
													//google: "actionlib::SimpleActionClient< ActionSpec > Class Template Reference" for detail.
	pos_pub.publish(pos_msg);
}


int main(int argc, char **argv)
{
	ros::init(argc, argv, "ece470_ur3_driver");
	ur3_comm ur3_start;
//	ros::spin();
	ros::MultiThreadedSpinner spinner(2);		//Multi-Threaded spinner that uses two threads
	spinner.spin();								// This enables state_sub_callback to publish isRdy variable
	return 0;									// while ctrl_sub_callback is being processed.
}

