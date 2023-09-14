////////////////////////////////////////////
// Includes
////////////////////////////////////////////
#include <ros/ros.h>
#include <krac_2018_final_log.h>


////////////////////////////////////////////
// Defines
////////////////////////////////////////////


////////////////////////////////////////////
// Usings
////////////////////////////////////////////


////////////////////////////////////////////
// Plain Variables
////////////////////////////////////////////


////////////////////////////////////////////
// ROS Variables
////////////////////////////////////////////


////////////////////////////////////////////
// User Functions
////////////////////////////////////////////
void gpsLogging(const sensor_msgs::NavSatFix& gpsmsg)
//void gpsLogging(const geometry_msgs::PoseStamped& gpsmsg)
{
    /*file << std::fixed << std::setprecision(6)
         << std::setw(11) << mode
         << std::setw(15) << waypointNum
         << std::setw(15) << begin.sec - UNIX2GPS
         << std::setw(15) << gpsmsg.pose.position.x * POSITIONSIZING
         << std::setw(15) << gpsmsg.pose.position.y * POSITIONSIZING
         << std::setprecision(1)
         << std::setw(15) << gpsmsg.pose.position.z
         << std::endl;*/

    file << std::fixed << std::setprecision(6)
         << std::setw(11) << mode
         << std::setw(15) << waypointNum
         << std::setw(15) << begin.sec - UNIX2GPS
         << std::setw(15) << gpsmsg.latitude
         << std::setw(15) << gpsmsg.longitude
         << std::setprecision(1)
         << std::setw(15) << gpsmsg.altitude - 222.0
         << std::endl;
}

void stateCheck(const mavros_msgs::State& flightMode)
{
    if((flightMode.mode == "STABILIZED") || (flightMode.mode == "MANUAL"))
    {
        mode = 0;
    }
    else if((flightMode.mode == "AUTO.MISSION") || (flightMode.mode == "AUTO.LOITER"))
    {
        mode = 1;
        if(modeCounter == 0)
        {
            file << "Auto flight mode engaged" << std::endl;
            modeCounter = 1;
        }
    }
    else
    {
        mode = 2;
    }
}

void waypointNumberCheck(const mavros_msgs::WaypointList &waypoints)
{
    for(int i = 0; i < WAYPOINTS; i++)
    {
        if(waypoints.waypoints[i].is_current == true)
        {
            waypointNum = i;
        }
    }
}


////////////////////////////////////////////
// main Function
////////////////////////////////////////////
int main(int argc, char **argv)
{
    ros::init(argc, argv, "krac_node_final_log_pub");
    ros::NodeHandle mNodeHandle;

    file.open("//home//odroid//Desktop//GPS_Log_KwangWoonUniv.asc");
    file << std::setw(11) << "Flight Mode"
         << std::setw(15) << "Waypoint Num."
         << std::setw(15) << "GPS Second"
         << std::setw(15) << "Latitude"
         << std::setw(15) << "Longitude"
         << std::setw(15) << "Altitude"
         << std::endl;

    begin = ros::Time::now();

    ros::Subscriber gpsLog = mNodeHandle.subscribe("/mavros/global_position/global", 100, gpsLogging);
    //ros::Subscriber gpsLog = mNodeHandle.subscribe("/mavros/local_position/pose", 100, gpsLogging);
    ros::Subscriber stateSub = mNodeHandle.subscribe("/mavros/state", 100, stateCheck);
    ros::Subscriber waypoints = mNodeHandle.subscribe("/mavros/mission/waypoints",100, waypointNumberCheck);

    ros::Rate loop_rate(10);

    ROS_INFO_STREAM("GPS Logging Started");

    while(ros::ok())
    {
        ros::spinOnce();
        loop_rate.sleep();
    }

    file << "Flight logging process end" << std::endl;
    file.close();
    return 0;
}
