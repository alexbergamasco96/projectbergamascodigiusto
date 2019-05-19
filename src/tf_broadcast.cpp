#include "ros/ros.h"
#include <nav_msgs/Odometry.h>
#include <tf/transform_broadcaster.h>
#include <geometry_msgs/Pose.h>

class tf_sub_pub{

    public:
        tf_sub_pub(){
            sub = n.subscribe("/robot/pose", 1000, &tf_sub_pub::callback, this);
        }
    
    void callback(const nav_msgs::OdometryConstPtr& msg){
        tf::Transform transform;
        transform.setOrigin( tf::Vector3(msg->pose.pose.position.x, msg->pose.pose.position.y, 0));
        tf::Quaternion q;
        q.setRPY(0,0,0);//msg->pose.pose.orientation);
        //è da fixare il puntatore sopra
        transform.setRotation(q);
        br.sendTransform( tf::StampedTransform(transform, ros::Time::now(), "world", "robot"));     
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