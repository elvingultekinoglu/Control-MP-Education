#include <ros/ros.h>
#include <std_msgs/String.h> 

int main(int argc, char **argv){

    ros::init(argc, argv, "ilk_deneme_publisher");
    ros::NodeHandle nh; 

    ros::Rate looprate(5); 

    ros::Publisher pub=nh.advertise<std_msgs::String>("/ilk_topic", 10);

    while(ros::ok()){

        std_msgs::String str;

        str.data = "Data yollandi!"; 

        pub.publish(str); 

        ros::spinOnce();
        looprate.sleep(); 
    }
}