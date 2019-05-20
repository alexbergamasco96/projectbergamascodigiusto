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
        /*tf::Transform transform;
        transform.setOrigin( tf::Vector3(msg->pose.pose.position.x, msg->pose.pose.position.y, 0));
        tf::Quaternion q;
        q.setRPY(0,0,0);//msg->pose.pose.orientation);
        //è da fixare il puntatore sopra
        transform.setRotation(q);
        br.sendTransform( tf::StampedTransform(transform, ros::Time::now(), "world", "robot")); 
        */
   
       //first, we'll publish the transform over tf
       geometry_msgs::TransformStamped odom_trans;
       odom_trans.header.stamp = ros::Time::now();
       odom_trans.header.frame_id = "odom";
       odom_trans.child_frame_id = "base_link";
   
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


int main(int argc, char **argv){

    ros::init(argc, argv, "tf_broad");
    tf_sub_pub my_tf_sub_bub;
    ros::spin();
    return 0;

}