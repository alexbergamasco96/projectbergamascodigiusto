#include "ros/ros.h"
#include "std_msgs/String.h"
#include "projectbergamascodigiusto/floatStamped.h"
#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <message_filters/sync_policies/exact_time.h>
#include <message_filters/sync_policies/approximate_time.h>

void callback(const projectbergamascodigiusto::floatStamped left, const projectbergamascodigiusto::floatStamped right, const projectbergamascodigiusto::floatStamped steer){

		ROS_INFO ("Received a messages: (%f,%f,%f)", left.data  , right.data, steer.data);

	}


int main(int argc, char** argv)
{
  ros::init(argc, argv, "subscriber_bag");

  ros::NodeHandle n;

  message_filters::Subscriber<geometry_msgs::Vector3Stamped> sub1(n, "topic1", 1);
  message_filters::Subscriber<geometry_msgs::Vector3Stamped> sub2(n, "topic2", 1);
  
  //typedef message_filters::sync_policies::ExactTime<geometry_msgs::Vector3Stamped, geometry_msgs::Vector3Stamped> MySyncPolicy;
  
  //message filters with policy
  typedef message_filters::sync_policies::ApproximateTime<geometry_msgs::Vector3Stamped, geometry_msgs::Vector3Stamped> MySyncPolicy;
  
  
  message_filters::Synchronizer<MySyncPolicy> sync(MySyncPolicy(10), sub1, sub2);
  sync.registerCallback(boost::bind(&callback, _1, _2));

  ros::spin();

  return 0;
}