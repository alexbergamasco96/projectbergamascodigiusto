#include "ros/ros.h"
#include <nav_msgs/Odometry.h>
#include <tf/transform_broadcaster.h>
#include <geometry_msgs/Pose.h>

class tf_sub_pub{

    public:
        tf_sub_pub(){
            sub = n.subscribe("odom", 1000, &tf_sub_pub::callback, this);
        }
    
    void callback(const nav_msgs::Odometry& odom){
        
       //first, we'll publish the transform over tf
       geometry_msgs::TransformStamped odom_trans;
       odom_trans.header.stamp = ros::Time::now();
       odom_trans.header.frame_id = "world";
       odom_trans.child_frame_id = "robot";
   
       odom_trans.transform.translation.x = odom.pose.pose.position.x;
       odom_trans.transform.translation.y = odom.pose.pose.position.y;
       odom_trans.transform.translation.z = 0.0;
       odom_trans.transform.rotation = odom.pose.pose.orientation;
   
       //send the transform
       br.sendTransform(odom_trans);

    }

    private:
    ros::NodeHandle n;
    tf::TransformBroadcaster br;
    ros::Subscriber sub;
};

//Main
int main(int argc, char **argv){

    ros::init(argc, argv, "tf_broad");
    tf_sub_pub my_tf_sub_bub;
    ros::spin();
    return 0;

}