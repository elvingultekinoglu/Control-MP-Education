#include <ros/ros.h> 
#include <mavros_msgs/State.h> 
#include <nav_msgs/Odometry.h> 

mavros_msgs::State state; 
nav_msgs::Odometry odom; 

void state_cb(const mavros_msgs::State::ConstPtr &msg){
    state=*msg; 
}

void odom_cb(const nav_msgs::Odometry::ConstPtr &msg){
    odom=*msg; 
}

int main(int argc, char**argv){
    ros::init(argc, argv, "odometry_node"); 
    ros::NodeHandle nh;

    ros::Subscriber state_sub=nh.subscribe<mavros_msgs::State>("/mavros/state", 10, state_cb);
    ros::Subscriber odom_sub=nh.subscribe<nav_msgs::Odometry>("/mavros/local_position/odom", 10, odom_cb); 

    ros::Rate looprate(20); 

    while(ros::ok()){
        ROS_INFO("x:%f, y:%f, z:%f, mode:%s" , odom.pose.pose.position.x, odom.pose.pose.position.y, odom.pose.pose.position.z, state.mode.c_str()); 
        ros::spinOnce(); 
        looprate.sleep(); 
    }
}