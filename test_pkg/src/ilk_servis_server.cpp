#include <ros/ros.h> 
#include <turtlesim/TeleportAbsolute.h> 

bool server_fonksiyon(turtlesim::TeleportAbsolute::Request &req, turtlesim::TeleportAbsolute::Response &res){

    float a,b; 
    a= req.x;
    b= req.y; 

    ROS_INFO("x: %f, y:%f", a, b); 

    return true; 

}


int main(int argc, char **argv){

    ros::init(argc, argv, "ilk_servis_server_denemesi"); 
    ros::NodeHandle nh; 

    ros::ServiceServer server=nh.advertiseService("/turtle1/teleport_absolute", server_fonksiyon); 

    ros::spin(); 

    
}