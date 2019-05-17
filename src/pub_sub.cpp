#include "ros/ros.h"
#include "std_msgs/String.h"
#include "projectbergamascodigiusto/floatStamped.h"
#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <message_filters/sync_policies/exact_time.h>
#include <message_filters/sync_policies/approximate_time.h>


class pub_sub{

	private:
	ros::NodeHandle n; 
			
	public:
	pub_sub(){

		message_filters::Subscriber<projectbergamascodigiusto::floatStamped> subLeft(n, "speedL_stamped", 1);
		message_filters::Subscriber<projectbergamascodigiusto::floatStamped> subRight(n, "speedR_stamped", 1);
		message_filters::Subscriber<projectbergamascodigiusto::floatStamped> subSteer(n, "steer_stamped", 1);
		typedef message_filters::sync_policies::ApproximateTime<projectbergamascodigiusto::floatStamped, projectbergamascodigiusto::floatStamped, projectbergamascodigiusto::floatStamped> MySyncPolicy;

  		message_filters::Synchronizer<MySyncPolicy> sync(MySyncPolicy(10), subLeft, subRight,subSteer);
	    sync.registerCallback(boost::bind(callback, _1, _2));

		//idea: if function. if(odom=diff){sync.reg ... with callback1} else ...callback2
		//callback1 compute differential drive, callback2 ackermann
		//sync.registerCallback(boost::bind(&pub_sub::callback, _1, _2, _3));
		
	}



	//Here compute the odometry.
	void callback(const projectbergamascodigiusto::floatStamped& left, const projectbergamascodigiusto::floatStamped& right, const projectbergamascodigiusto::floatStamped& steer){

		ROS_INFO ("Received a messages: (%f,%f,%f)", left.data  , right.data, steer.data);

	}

};




//Classic main of pub_sub in the same node

int main(int argc, char **argv){

 	ros::init(argc, argv, "subscribe_and_publish");
	 
 	pub_sub my_pub_sub;
 	ros::spin();
 	return 0;

}