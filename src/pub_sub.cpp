#include "ros/ros.h"
#include "std_msgs/String.h"
#include "projectbergamascodigiusto/floatStamped.h"
#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>


class pub_sub{

	std_msgs::String messagio;
	std_msgs::String messagio2;

	private:
	ros::NodeHandle n; 

	/*ros::Subscriber sub;
	ros::Subscriber sub2;*/
	message_filters::Subscriber<projectbergamascodigiusto::floatStamped> subLeft(n, "speedL_stamped", 1);
	message_filters::Subscriber<projectbergamascodigiusto::floatStamped> subRight(n, "speedR_stamped", 1);
	message_filters::Subscriber<projectbergamascodigiusto::floatStamped> subSteer(n, "steer_stamped", 1);
	message_filters::TimeSynchronizer<projectbergamascodigiusto::floatStamped, projectbergamascodigiusto::floatStamped, projectbergamascodigiusto::floatStamped> sync(subLeft, subRight, subSteer, 10);
	
	//idea: if function. if(odom=diff){sync.reg ... with callback1} else ...callback2
	//callback1 compute differential drive, callback2 ackermann
	sync.registerCallback(boost::bind(&callback, _1, _2, _3));

	ros::Publisher pub; 
	ros::Timer timer1;
		
		
	public:
	pub_sub(){
		
		sub = n.subscribe("/chatter", 1, &pub_sub::callback, this);
		sub2 = n.subscribe("/chatter2", 1, &pub_sub::callback2, this);
		pub = n.advertise<std_msgs::String>("/rechatter", 1);
		timer1 = n.createTimer(ros::Duration(1), &pub_sub::callback1, this);
		
	}



	//Here compute the odometry.
	void callback(const std_msgs::String::ConstPtr& msg){
		messagio=*msg;
	}

};




//Classic main of pub_sub in the same node

int main(int argc, char **argv){

 	ros::init(argc, argv, "subscribe_and_publish");
 	pub_sub my_pub_sub;
 	ros::spin();
 	return 0;

}