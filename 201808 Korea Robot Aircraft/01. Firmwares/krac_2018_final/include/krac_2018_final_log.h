#ifndef KRAC_2018_FINAL_LOG
#define KRAC_2018_FINAL_LOG

////////////////////////////////////////////
// Includes
////////////////////////////////////////////
#include <ros/ros.h>
#include <std_msgs/String.h>
#include <stdio.h>
#include <iomanip>
#include <fstream>
#include <serial/serial.h>
#include <std_msgs/Empty.h>
#include <sstream>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <limits>

#include "geometry_msgs/PoseStamped.h"
#include "geometry_msgs/Point.h"
#include "geometry_msgs/Vector3Stamped.h"

#include "sensor_msgs/NavSatFix.h"

#include "mavros_msgs/WaypointList.h"
#include "mavros_msgs/WaypointPush.h"
#include "mavros_msgs/WaypointPull.h"
#include "mavros_msgs/WaypointSetCurrent.h"
#include "mavros_msgs/CommandCode.h"
#include "mavros_msgs/SetMode.h"
#include "mavros_msgs/State.h"
#include "mavros_msgs/CommandBool.h"


////////////////////////////////////////////
// Defines
////////////////////////////////////////////
#define WAYPOINTS 15
#define UNIX2GPS 315964800
#define POSITIONSIZING 100000000000000000


////////////////////////////////////////////
// Plain Variables
////////////////////////////////////////////
std::ofstream file;
int mode = 0;
int modeCounter = 0;
int waypointNum = 225;

std::string modeStable = "STABILIZED";
std::string modeMission = "MISSION";


////////////////////////////////////////////
// ROS Variables
////////////////////////////////////////////
ros::Time begin;
mavros_msgs::Waypoint mWaypoint[WAYPOINTS];


////////////////////////////////////////////
// User Functions
////////////////////////////////////////////
void gpsLogging(const sensor_msgs::NavSatFix& gpsmsg);
//void gpsLogging(const geometry_msgs::PoseStamped& gpsmsg);
void stateCheck(const mavros_msgs::State& flightMode);
void waypointNumberCheck(const mavros_msgs::WaypointList &waypoints);


#endif // KRAC_2018_FINAL_LOG
