#include <ros/ros.h>
#include <mavros_msgs/CommandBool.h>
#include <nav_msgs/Odometry.h>
#include <mavros_msgs/State.h>
#include <mavros_msgs/SetMode.h>
#include <geometry_msgs/PoseStamped.h> 
#include <mavros_msgs/ParamSet.h>
#include <drone_pkg/takeoff_srv.h>
#include <drone_pkg/takeoff_serv.h>

mavros_msgs::CommandBool com;
nav_msgs::Odometry odom;
mavros_msgs::State state; 
mavros_msgs::SetMode mode;
geometry_msgs::PoseStamped pose;
mavros_msgs::ParamSet param; 
drone_pkg::takeoff_serv servis; 

void state_cb(const mavros_msgs::State::ConstPtr &msg){
    state=*msg; 
}

void odom_cb(const nav_msgs::Odometry::ConstPtr &msg){
    odom=*msg; 
}

ros::ServiceClient com_client;
ros::Subscriber odom_sub;
ros::Subscriber state_sub;
ros::ServiceClient mode_client;
ros::Publisher pose_pub;
ros::ServiceClient param_client; 
bool takeoff; 

bool takeoff_func(drone_pkg::takeoff_serv::Request &req, drone_pkg::takeoff_serv::Response &res){

    if(req.is_takeoff==true){

        com.request.value=true;
        com_client.call(com); 

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

bool location_func(drone_pkg::takeoff_serv::Request &req, drone_pkg::takeoff_serv::Response &res){
    pose=req.poseStamped;
    return true; 
}

int main(int argc, char**argv){
    ros::init(argc, argv, "takeoff_alternative_node"); 
    ros::NodeHandle nh; 

    com_client=nh.serviceClient<mavros_msgs::CommandBool>("/mavros/cmd/arming"); 
    odom_sub=nh.subscribe<nav_msgs::Odometry>("/mavros/global_position/local",10,odom_cb);
    state_sub=nh.subscribe<mavros_msgs::State>("/mavros/state",10,state_cb); 
    mode_client=nh.serviceClient<mavros_msgs::SetMode>("/mavros/set_mode");
    pose_pub=nh.advertise<geometry_msgs::PoseStamped>("/mavros/setpoint_position/local",10); 
    param_client=nh.serviceClient<mavros_msgs::ParamSet>("/mavros/param/set"); 
    ros::ServiceServer takeoff_server=nh.advertiseService("/takeoff",takeoff_func); 
    ros::ServiceServer location_server=nh.advertiseService("/location",location_func); 

    ros::Rate looprate(20); 

    while(ros::ok() && !state.connected){
        ros::spinOnce(); 
        looprate.sleep(); 
    }

    while(ros::ok()){

        ROS_INFO("%d", state.connected); 

        while(!takeoff && ros::ok()){
            ros::spinOnce(); 
            looprate.sleep(); 
        }

        for (int i=100; ros::ok() && i>0; i--){
            pose_pub.publish(pose);
            ros::spinOnce();
            looprate.sleep(); 
        }

        
        mode.request.custom_mode="OFFBOARD";

        if(mode_client.call(mode)==true){
            ROS_INFO("Called setmode");

            if(mode.response.mode_sent==true){
                ROS_INFO("Offbard is enabled"); 
            }
        }
        

        while(ros::ok()){
            ROS_INFO("x:%f, y:%f, z:%f, mode:%s, value:%f", odom.pose.pose.position.x, odom.pose.pose.position.y, odom.pose.pose.position.z, state.mode.c_str(), pose.pose.position.z);

            if(takeoff==false){
                break;
            }

            pose_pub.publish(pose);
            looprate.sleep();
            ros::spinOnce(); 
        }


    }
}