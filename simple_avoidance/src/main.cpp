

#include <ros/ros.h>
#include <ros/console.h>
#include <sensor_msgs/LaserScan.h>

#include <geometry_msgs/Twist.h>

#include<stdio.h>

ros::Publisher cmdVelPub;

void laserCallback(const sensor_msgs::LaserScan::ConstPtr& msg){
	ROS_INFO("Got %i laser scans", msg->ranges.size());

	bool obstacle = false;
	float angle = msg->angle_min;
	for(int i=0; i<msg->ranges.size(); i++){
		float range = msg->ranges[i];
		if(range >= msg->range_min && range <= msg->range_max){
			//ROS_INFO("index=%i\t	angle=%f	\trange=%f", i, angle, range);
			if(range < 1.0){
				obstacle = true;
				break;
			}
		}

		angle += msg->angle_increment;
	}
	
	geometry_msgs::Twist cmd;
	if(!obstacle){
		ROS_INFO("SAFE");

		cmd.linear.x = 0.5;
		cmd.angular.z = 0;
	}
	else{
		ROS_INFO("TURN");

		cmd.linear.x = 0;
		cmd.angular.z = 1.0;
	}

	cmdVelPub.publish(cmd);
}


int main(int argc, char **argv){

	ROS_INFO("Hello World");

	ros::init(argc, argv, "obstacle_avoid");

	ros::NodeHandle nh;

	ros::Subscriber laserScanSub = nh.subscribe("scan", 1, laserCallback);

	cmdVelPub = nh.advertise<geometry_msgs::Twist>("cmd_vel", 2);
	ros::spin();

	return 0;
}
