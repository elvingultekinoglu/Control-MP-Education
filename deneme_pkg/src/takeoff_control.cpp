#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/PoseStamped.h>
#include <mavros_msgs/State.h>
#include <mavros_msgs/CommandBool.h>
#include <mavros_msgs/ParamSet.h>
#include <mavros_msgs/SetMode.h>
#include <deneme_pkg/takeoff_srv.h>

nav_msgs::Odometry odom; 
geometry_msgs::PoseStamped pose;
mavros_msgs::State state;
mavros_msgs::CommandBool arming; 
mavros_msgs::ParamSet param;
mavros_msgs::SetMode mode;
deneme_pkg::takeoff_srv servis; 

void odom_cb(const nav_msgs::Odometry::ConstPtr &msg){
    odom=*msg;
}

void state_cb(const mavros_msgs::State::ConstPtr &msg){
    state=*msg; 
}

ros::Subscriber odom_sub;
ros::Publisher pose_pub;
ros::Subscriber state_sub;
ros::ServiceClient arming_client;
ros::ServiceClient param_client;
ros::ServiceClient mode_client; 

bool takeoff; 

bool takeoff_function(deneme_pkg::takeoff_srv::Request &req, deneme_pkg::takeoff_srv::Response &res){

    if(req.is_takeoff==true){

        arming.request.value=true;
        arming_client.call(arming);

        param.request.param_id="MIS_TAKEOFF_ALT";
        param.request.value.real=req.poseStamped.pose.position.z;
        pose=req.poseStamped; 

        param_client.call(param); 

        mode.request.custom_mode="AUTO.TAKEOFF"; 
        mode_client.call(mode); 
        takeoff=true; 
    } else {
        takeoff=false; 
    }

    return true; 
}

bool location_function(deneme_pkg::takeoff_srv::Request &req, deneme_pkg::takeoff_srv::Response &res){
    pose=req.poseStamped; 
    return true; 
}

int main(int argc, char **argv){
    ros::init(argc, argv, "takeoff_control_code");
    ros::NodeHandle nh;

    odom_sub=nh.subscribe<nav_msgs::Odometry>("/mavros/global_position/local",10,odom_cb);
    pose_pub=nh.advertise<geometry_msgs::PoseStamped>("/mavros/setpoint_position/local",10);
    state_sub=nh.subscribe<mavros_msgs::State>("/mavros/state",10,state_cb);
    arming_client=nh.serviceClient<mavros_msgs::CommandBool>("/mavros/cmd/arming");
    param_client=nh.serviceClient<mavros_msgs::ParamSet>("/mavros/param/set");
    mode_client=nh.serviceClient<mavros_msgs::SetMode>("/mavros/set_mode");
    ros::ServiceServer takeoff_server=nh.advertiseService("/takeoff_service",takeoff_function);
    ros::ServiceServer location_server=nh.advertiseService("/location_service",location_function); 

    ros::Rate looprate(10);

    while(ros::ok() && !state.connected){
        ros::spinOnce();
        looprate.sleep(); 
    }

    while(ros::ok()){

        ROS_INFO("%d",state.connected);

        while(!takeoff && ros::ok()){
            ros::spinOnce();
            looprate.sleep();
        }

        for (int i=100; i>0; i--){
            pose_pub.publish(pose);
            ros::spinOnce();
            looprate.sleep();
        }

        mode.request.custom_mode="OFFBOARD";

        if(mode_client.call(mode)==true){
            ROS_INFO("Called setmode!"); 

            if(mode.response.mode_sent==true){
                ROS_INFO("Offboard is enabled!"); 
            }
        }

        while(ros::ok()){
            ROS_INFO("x:%f, y:%f, z:%f", odom.pose.pose.position.x, odom.pose.pose.position.y, odom.pose.pose.position.z);
            if(takeoff==false){
                break; 
            }
            pose_pub.publish(pose); 
            ros::spinOnce();
            looprate.sleep(); 
        }
    }
}