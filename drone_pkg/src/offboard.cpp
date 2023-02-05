#include <ros/ros.h>
#include <mavros_msgs/SetMode.h>
#include <geometry_msgs/PoseStamped.h>
#include <nav_msgs/Odometry.h> 
#include <mavros_msgs/State.h>
#include <mavros_msgs/CommandBool.h> 
#include <thread>
#include <iostream> 

mavros_msgs::SetMode mode; 
nav_msgs::Odometry odom; 
mavros_msgs::State state; 
geometry_msgs::PoseStamped pose; 
mavros_msgs::CommandBool arm; 

void odom_cb(const nav_msgs::Odometry::ConstPtr &msg){
    odom=*msg; 
}

void state_cb(const mavros_msgs::State::ConstPtr &msg){
    state=*msg; 
}

ros::Publisher pose_pub; 

void setpoint(int x){
    ros::Rate looprate(10); 

    while(ros::ok()){
        ROS_INFO("x:%f, y:%f, z:%f, mode:%s", odom.pose.pose.position.x, odom.pose.pose.position.y, odom.pose.pose.position.z, state.mode.c_str());
        pose_pub.publish(pose); 
        ros::spinOnce();
        looprate.sleep(); 
    }
}

int main(int argc, char **argv){
    ros::init(argc,argv, "offboard_node"); 
    ros::NodeHandle nh; 

    ros::Rate looprate(10); 

    ros::Subscriber odom_sub=nh.subscribe<nav_msgs::Odometry>("/mavros/global_position/local", 10, odom_cb);
    ros::Subscriber state_sub=nh.subscribe<mavros_msgs::State>("/mavros/state", 10, state_cb);
    ros::ServiceClient arming_client=nh.serviceClient<mavros_msgs::CommandBool>("/mavros/cmd/arming"); 
    pose_pub=nh.advertise<geometry_msgs::PoseStamped>("/mavros/setpoint_position/local", 1000);
    ros::ServiceClient mode_client=nh.serviceClient<mavros_msgs::SetMode>("/mavros/set_mode"); 

    while(ros::ok()&& !state.connected){
        ros::spinOnce(); 
        looprate.sleep(); 
    }

    std::thread thr_setpoint(setpoint,3);

    pose.pose.position.x=0;
    pose.pose.position.y=0;
    pose.pose.position.z=0; 

    for(int i=100; ros::ok()&& i>0; --i){
        pose_pub.publish(pose);
        ros::spinOnce();
        looprate.sleep(); 
    }

    mode.request.custom_mode="OFFBOARD"; 

    if(mode_client.call(mode)==true){
        if(mode.response.mode_sent==true){
            ROS_INFO("Offboard enabled"); 
        }

        arm.request.value=true; 

        if(arming_client.call(arm)==true){
            if(arm.response.result==0){
                ROS_INFO("Arming is done!"); 
            }
        }
    

        pose.pose.position.x=0.0;
        pose.pose.position.y=0.0;
        pose.pose.position.z=5.0; 

        thr_setpoint.join(); 

    } 
}