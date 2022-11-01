#include <ros/ros.h>
#include <test_pkg/robot_srv.h>

bool server_fonksiyon(test_pkg::robot_srv::Request &req, test_pkg::robot_srv::Response &res){

    res.toplam=req.a+req.b; 
    ROS_INFO("Toplam: %ld", res.toplam); 
    return true; 
}

int main(int argc, char **argv){

    ros::init(argc, argv, "ilk_custom_serv");
    ros::NodeHandle nh; 

    ros::ServiceServer server=nh.advertiseService("/ilk_servis", server_fonksiyon); 

    ros::spin(); 
}