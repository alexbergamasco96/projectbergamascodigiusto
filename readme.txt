ID, name, surname of all team members
-small description of the files inside the archive
-name of the parameter to change odometry source/set/reset position
-structure of the tf tree
-structure of the custom message
-description of how to start/use the nodes
-info you think are important/interesting

//////////////////////////////////////////////////
Team Members:
	-Federico Di Giusto, 10693473
	-Alex Bergamasco,


Files inside the folder /projectbergamascodigiusto:

	-/cfg/parameters.cfg 
		File python for the options of the dynamic reconfigure.
		Through it, we set x,y initial (as int), and the type of odometry (Differential or Ackerman, through an enumeration)
	-/msg/computedOdom.msg
		Custom message published as topic /computed_odometry
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
			Subscriber for the topic /odom (it passes the nav_msgs).
			Set the transormation and send it.
	-/launch/launcher.launch
			TODOTODO

name of the parameter to change odometry source/set/reset position
-structure of the tf tree
-structure of the custom message
-description of how to start/use the nodes
-info you think are important/interesting


	

