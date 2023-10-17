/**
 * @file /include/irc_drone_vision/qnode.hpp
 *
 * @brief Communications central!
 *
 * @date February 2011
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef irc_drone_vision_QNODE_HPP_
#define irc_drone_vision_QNODE_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/

// To workaround boost/qt4 problems that won't be bugfixed. Refer to
//    https://bugreports.qt.io/browse/QTBUG-22829
#ifndef Q_MOC_RUN
#include <ros/ros.h>
#endif
#include <string>
#include <QThread>
#include <QStringListModel>
#include <QMutex>
#include <image_transport/image_transport.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/opencv.hpp>
#include <cv_bridge/cv_bridge.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <omp.h>


/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace irc_drone_vision {

/*****************************************************************************
** Class
*****************************************************************************/

class QNode : public QThread {
    Q_OBJECT
public:
	QNode(int argc, char** argv );
	virtual ~QNode();
	bool init();
        void run();

        cv::Mat *img_qnode;

Q_SIGNALS:
    void recvImg();
    void rosShutdown();

private:
	int init_argc;
	char** init_argv;
        image_transport::Subscriber image_sub;

        void imageCallback(const sensor_msgs::ImageConstPtr& msg_img);
};

}  // namespace irc_drone_vision

#endif /* irc_drone_vision_QNODE_HPP_ */
