#include "ros/ros.h"
#include "std_msgs/String.h"
#include "projectbergamascodigiusto/floatStamped.h"
#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <message_filters/sync_policies/exact_time.h>
#include <message_filters/sync_policies/approximate_time.h>

void callback(const projectbergamascodigiusto::floatStampedConstPtr& left, const projectbergamascodigiusto::floatStampedConstPtr& right){

		ROS_INFO ("Received a messages: (%f, %f)", left->data  , right->data);

	}


int main(int argc, char** argv)
{
  ros::init(argc, argv, "subscriber_bag");

  ros::NodeHandle n;

  message_filters::Subscriber<projectbergamascodigiusto::floatStamped> sub1(n, "steerL_stamped", 1);
  message_filters::Subscriber<projectbergamascodigiusto::floatStamped> sub2(n, "steerR_stamped", 1);
  
  //typedef message_filters::sync_policies::ExactTime<geometry_msgs::Vector3Stamped, geometry_msgs::Vector3Stamped> MySyncPolicy;
  
  //message filters with policy
  typedef message_filters::sync_policies::ApproximateTime<projectbergamascodigiusto::floatStamped, projectbergamascodigiusto::floatStamped> MySyncPolicy;
  
  
  message_filters::Synchronizer<MySyncPolicy> sync(MySyncPolicy(10), sub1, sub2);
  sync.registerCallback(boost::bind(&callback, _1, _2));

  ros::spin();

  return 0;
}