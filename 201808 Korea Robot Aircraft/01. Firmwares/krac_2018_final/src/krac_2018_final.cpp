////////////////////////////////////////////
// Includes
////////////////////////////////////////////
#include <ros/ros.h>
#include <krac_2018_final.h>


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
//    gpsData.pose.position.x = gpsmsg.pose.position.x;
//    gpsData.pose.position.y = gpsmsg.pose.position.y;
//    gpsData.pose.position.z = gpsmsg.pose.position.z;

    gpsData.latitude = gpsmsg.latitude;
    gpsData.longitude = gpsmsg.longitude;
    gpsData.altitude = gpsmsg.altitude - 222.0;
}

void serialSend(const ros::TimerEvent&)
{
    serial_Data = mSerial.readline();
    if(!serial_Data.empty())
    {
        ROS_INFO_STREAM(serial_Data);
        if(serial_Data == "It's test line")
        {
            ROS_INFO_STREAM("Serial Connection Detected");
        }
        else if(serial_Data.at(0) == '8' && serial_Data.at(1) == '4')
        {
            std::stringstream serial_Stream(serial_Data);
            std::string token;

            int i = 0;

            while(std::getline(serial_Stream, token, ','))
            {
                //ROS_INFO_STREAM(token);
                std::istringstream token_Iss(token);
                double token_Double;

                token_Iss >> token_Double;

                if(token.at(0) == '3')
                {
                    waypoint_Lat[i] = token_Double;
                }
                else if(token.at(0) == '1')
                {
                    waypoint_Lng[i] = token_Double;
                    i++;
                }
            }
            for(int k = 0; k < WAYPOINTS; k++)
            {
                mWaypoint[k].frame = mavros_msgs::Waypoint::FRAME_GLOBAL_REL_ALT;
                mWaypoint[k].autocontinue = true;
                mWaypoint[k].is_current = false;
                mWaypoint[k].command = mavros_msgs::CommandCode::NAV_WAYPOINT;
                mWaypoint[k].param1 = 0;
                mWaypoint[k].param2 = 0;
                mWaypoint[k].param3 = 0;
                mWaypoint[k].param4 = 0;
                if(k == 0)
                {
                    mWaypoint[0].command = mavros_msgs::CommandCode::NAV_TAKEOFF;
                    mWaypoint[k].x_lat = waypoint_Lat[k];
                    mWaypoint[k].y_long = waypoint_Lng[k];
                    mWaypoint[k].z_alt = 30.0;
                }
                else if(k == 1)
                {
                    mWaypoint[k].command = mavros_msgs::CommandCode::DO_CHANGE_SPEED;
                    mWaypoint[k].frame = mavros_msgs::Waypoint::FRAME_MISSION;
                    mWaypoint[k].param2 = 3.0;
                    mWaypoint[k].x_lat = waypoint_Lat[k - 1];
                    mWaypoint[k].y_long = waypoint_Lng[k - 1];
                    mWaypoint[k].z_alt = 30.0;
                }
                else if(k > 1 && k < 6)
                {
                    mWaypoint[k].x_lat = waypoint_Lat[k - 1];
                    mWaypoint[k].y_long = waypoint_Lng[k - 1];
                    mWaypoint[k].z_alt = 30.0;
                }
                else if(k == 6)
                {
                    mWaypoint[k].x_lat = waypoint_Lat[k - 2];
                    mWaypoint[k].y_long = waypoint_Lng[k - 2];
                    mWaypoint[k].z_alt = 30.0;
                    mWaypoint[k].param1 = 5.0;
                }
                else if(k > 6 && k < WAYPOINTS - 4)
                {
                    mWaypoint[k].x_lat = waypoint_Lat[k - 2];
                    mWaypoint[k].y_long = waypoint_Lng[k - 2];
                    mWaypoint[k].z_alt = 30.0;
                }
                else if(k == WAYPOINTS - 4)
                {
                    mWaypoint[k].command = mavros_msgs::CommandCode::NAV_LAND;
                    mWaypoint[k].x_lat = waypoint_Lat[k - 3];
                    mWaypoint[k].y_long = waypoint_Lng[k - 3];
                    mWaypoint[k].z_alt = 0.0;
                }
                else if(k == WAYPOINTS - 3)
                {
                    mWaypoint[k].command = mavros_msgs::CommandCode::NAV_TAKEOFF;
                    mWaypoint[k].x_lat = waypoint_Lat[k - 4];
                    mWaypoint[k].y_long = waypoint_Lng[k - 4];
                    mWaypoint[k].z_alt = 28.0;
                }
                else if(k == WAYPOINTS - 2)
                {
                    mWaypoint[k].x_lat = waypoint_Lat[0];
                    mWaypoint[k].y_long = waypoint_Lng[0];
                    mWaypoint[k].z_alt = 28.0;
                }
                else if(k == WAYPOINTS - 1)
                {
                    mWaypoint[k].x_lat = waypoint_Lat[0];
                    mWaypoint[k].y_long = waypoint_Lng[0];
                    mWaypoint[k].command = mavros_msgs::CommandCode::NAV_LAND;
                    mWaypoint[k].z_alt = 0.0;
                }
            }
            mWaypoint[0].is_current = true;

            for(int k = 0; k < WAYPOINTS; k++)
            {
                service.request.waypoints.push_back(mWaypoint[k]);
            }
            if(client.call(service))
            {
                ROS_INFO("Success %d", service.response.success);
            }
            else
            {
                ROS_INFO("Failed");
            }

            serialFlag = 1;
        }
        else if(serial_Data == "700")                   // Arm Signal Received
        {
            srv_setArm.request.value = true;
            if(thirdClient.call(srv_setArm))
            {
                ROS_INFO("Arming Success %d", srv_setArm.response.result);
            }
            else
            {
                ROS_INFO("Arming Failed");
            }
        }
        else if(serial_Data == "500")                   // Disarm Signal Received
        {
            srv_setArm.request.value = false;
            if(thirdClient.call(srv_setArm))
            {
                ROS_INFO("Disarming Success %d", srv_setArm.response.result);
            }
            else
            {
                ROS_INFO("Disarming Failed");
            }
        }
        else if(serial_Data == "600")                   // Mission Mode Signal Received
        {
            srv_setMode.request.custom_mode = "AUTO.MISSION";
            if(secondClient.call(srv_setMode))
            {
                ROS_INFO("Mode Change Success %d", srv_setMode.response.mode_sent);
            }
            else
            {
                ROS_INFO("Mode Change Failed");
            }
        }
        else if(serial_Data == "400")                   // Stabilized Mode Signal Received
        {
            srv_setMode.request.custom_mode = "STABILIZED";
            if(secondClient.call(srv_setMode))
            {
                ROS_INFO("Mode Change Success %d", srv_setMode.response.mode_sent);
            }
            else
            {
                ROS_INFO("Mode Change Failed");
            }
        }
    }
    else
    {
        if(serialFlag != 0)
        {
            std::ostringstream buff[5];
//            float gps_Latitude = gpsData.pose.position.x * POSITIONSIZING / 100;
//            float gps_Longitude = gpsData.pose.position.y * POSITIONSIZING * POSITIONSIZING / 1000;
//            float gps_Altitude = gpsData.pose.position.z;

            float gps_Latitude = gpsData.latitude;
            float gps_Longitude = gpsData.longitude;
            float gps_Altitude = gpsData.altitude - 222.0;

            buff[0] << mode;
            buff[1] << waypointNum;
            buff[2] << gps_Latitude;
            buff[3] << gps_Longitude;
            buff[4] << gps_Altitude;

            mSerial.write("225,"                        // Aircraft GPS Position Transmit
                        + buff[0].str() + ","
                        + buff[1].str() + ","
                        + buff[2].str() + ","
                        + buff[3].str() + ","
                        + buff[4].str() + ",");


            mSerial.flush();
        }

        begin = ros::Time::now();
    }
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

std::vector<std::string> split(std::string str, std::string token)
{
    std::vector<std::string> result;
    while(str.size())
    {
        int index = str.find(token);
        if(index != std::string::npos)
        {
            result.push_back(str.substr(0,index));
            str = str.substr(index + token.size());
            if(str.size() == 0)
            {
                result.push_back(str);
            }
        }
        else
        {
            result.push_back(str);
            str = "";
        }
    }
    return result;
}


////////////////////////////////////////////
// main Function
////////////////////////////////////////////
int main(int argc, char **argv)
{
    ros::init(argc, argv, "krac_node_final_pub");
    ros::NodeHandle nh;

    mTimer = nh.createTimer(ros::Duration(0.5), serialSend);

    ros::Subscriber gpsLog = nh.subscribe("/mavros/global_position/global", 100, gpsLogging);
    //ros::Subscriber gpsLog = nh.subscribe("/mavros/local_position/pose", 100, gpsLogging);
    ros::Subscriber stateSub = nh.subscribe("/mavros/state", 100, stateCheck);
    ros::Subscriber waypoints = nh.subscribe("/mavros/mission/waypoints",100, waypointNumberCheck);
    client = nh.serviceClient<mavros_msgs::WaypointPush>("/mavros/mission/push");
    secondClient = nh.serviceClient<mavros_msgs::SetMode>("/mavros/set_mode");
    thirdClient = nh.serviceClient<mavros_msgs::CommandBool>("/mavros/cmd/arming");

    ros::Rate loop_rate(10);

    try
    {
        mSerial.setPort("/dev/ttyUSB1");
        mSerial.setBaudrate(57600);
        serial::Timeout mTimeout = serial::Timeout::simpleTimeout(200);
        mSerial.setTimeout(mTimeout);
        mSerial.open();
    }
    catch(serial::IOException& e)
    {
        ROS_ERROR_STREAM("Unable to open the port");
    }

    if(mSerial.isOpen())
    {
        ROS_INFO_STREAM("Serial Port Initialized");
    }
    else
    {
        return -1;
    }

    while(ros::ok())
    {
        ros::spinOnce();
        loop_rate.sleep();
    }

    mSerial.close();
    return 0;
}
