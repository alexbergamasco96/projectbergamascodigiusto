

/***************************************************************************************************************/
Team Members:
	-Federico Di Giusto, 	10693473
	-Alex Bergamasco, 		10521973

/***************************************************************************************************************/
Files inside the folder /projectbergamascodigiusto:

	-/cfg/parameters.cfg 
		File python for the options of the dynamic reconfigure.
		Through it, we set x,y,theta(angle) initial (as int), and the type of odometry (Differential or Ackerman, through an enumeration)
	-/msg/computedOdom.msg
		Custom message published in the topic "/computed_odometry"
	-/srv/OdometryComputation.srv 
		Request and response structure of the service used for the computation of the odometry.
	-/src/
		/pub_sub.cpp
			Central features of the project implemented in a Class.
			It computes the message filters for the topic from the bag, with the Approximate?????/TODOTODO
			It computes Server of the Dynamic Reconfigure and its callback
			It performs the Client of the service, that computes the odometry, and it manages the response.
			It publishes as nav_msgs::Odometry in topic /odom the response of the service(for the tf_broadcast)
			It publishes as projectbergamascodigiusto::computedOdom in topic /computed_odometry
		/odometry_server.cpp
			Server for the computation of the odometry.
		/tf_broadcast.cpp
			Subscriber for the topic "/odom" (it passes the nav_msgs).
			Set the transormation and send it.
	-/launch/launcher.launch
			launch file

/***************************************************************************************************************/
Dynamic reconfigure paramters:

	"x_initial": initial x, integer, default 0
	"y_initial": initial y, integer, default 0
	"theta_initial": theta_initial (in degree), integer, default 0
	"odometry_type": Differential or Ackerman, integer, default Differential


/***************************************************************************************************************/
Structure of the tf tree:

	The TF TREE is simply composed by the World (the fixed frame) and the robot (represented by a single point).
	We have created a node (tf_broadcast) that sends the transformation from the father -world- to the child -robot-.

/***************************************************************************************************************/
Structure of the custom message:

	We have created two different custom messages:
		- floatStamped (Header, float64) : useful to extract data from the bag.
		- computedOdom (string, nav_msgs::Odometry) : final message to send the type of odometry (Differential or Ackerman) and the odometry.

/***************************************************************************************************************/
Description of how to start/use the nodes:
	rosrun projectbergamascodigiusto pub_sub
	rosrun projectbergamascodigiusto odometry_server
	rosrun projectbergamascodigiusto tf_broadcast
	rosrun rqt_reconfigure rqt_reconfigure 	//set subscribe_and_publish

Otherwise,
 roslaunch projectbergamascodigiusto/launch/launcher.launch
 	It will launch all the nodes specified before (also rqt_reconfigure).

/***************************************************************************************************************/
-info you think are important/interesting

	The odometry is computed by a service. It is not necessary but it seems a good idea in order to make the project more modular.
	We have also created a separate node in which we send the transformation.
	
	We are aware that we could have done everything inside a node but from a design point of view it seemed clearer in this way.

	To compute the robot odometry we used Runge-Kutta integration but there is also the implementation of Eulero integration and this is commented in the code.

TODO