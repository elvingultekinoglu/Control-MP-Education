#include <ros/ros.h>
#include <std_msgs/String.h> 

void callback_fonksiyon(const std_msgs::String &msg){
    ROS_INFO("Mesaj: %s", msg.data.c_str());
}

int main(int argc, char **argv){

    ros::init(argc, argv, "ilk_deneme_subscriber");
    ros::NodeHandle nh;

    ros::Subscriber sub=nh.subscribe("/ilk_topic", 1000, callback_fonksiyon); 

    ros::spin(); 
}