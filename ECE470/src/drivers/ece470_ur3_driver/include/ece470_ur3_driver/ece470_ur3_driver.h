#ifndef ECE470_UR3_DRIVER_H
#define ECE470_UR3_DRIVER_H
#include "ros/ros.h"
#include <stdlib.h>

// defined msg by ece470_ur3_driver
#include <ece470_ur3_driver/command.h>
#include <ece470_ur3_driver/positions.h>

//header file for joint_state topic
#include <sensor_msgs/JointState.h>
#include <trajectory_msgs/JointTrajectory.h>


// message header files for the simple action client
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/simple_client_goal_state.h>

#include <control_msgs/JointTrajectoryControllerState.h>
#include <control_msgs/FollowJointTrajectoryAction.h>
#include <control_msgs/FollowJointTrajectoryActionGoal.h>

//using and typedef
typedef actionlib::SimpleActionClient<control_msgs::FollowJointTrajectoryAction> TrajClient;
typedef actionlib::SimpleClientGoalState ClientState;
using std::cout;
using std::endl;
using std::cin;
using std::string;

//class ur3_comm definition
class ur3_comm{
		/** publishers & subscribers
		*/
		ros::NodeHandle	nh;				// ros nh
		ros::Subscriber	ctrl_sub;		//control coming from the student
		ros::Subscriber state_sub;		//robot state from the driver
		ros::Publisher 	pos_pub;		//student will use this to figure out the current state of the robot.
		ros::Timer 	ur3_timer;
		ece470_ur3_driver::positions	pos_msg; //actual message for the position publishing to the student.
		
		/** ROS msgs
		*/
		trajectory_msgs::JointTrajectory trajectories;			// for the actual driver
		control_msgs::FollowJointTrajectoryGoal desired_goal;	
		sensor_msgs::JointState joint_states;					// for the state_sub
		/** Action Client
		*/
		TrajClient* client;				// The client for robot control (actual driver)
		/** Other Variables
		*/
		std::vector<double> v;
		bool isRdy;
		bool pending;
		//actionlib::SimpleClientGoalState execution_state(SimpleClientGoalState::LOST);
		/** callback functions
		 */
		void ctrl_sub_callback(const ece470_ur3_driver::command::ConstPtr& msg);
		void state_sub_callback(const sensor_msgs::JointState::ConstPtr& msg);
	
	public:
		ur3_comm();
		~ur3_comm();
};
#endif
