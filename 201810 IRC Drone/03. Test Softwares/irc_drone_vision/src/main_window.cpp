/**
 * @file /src/main_window.cpp
 *
 * @brief Implementation for the qt gui.
 *
 * @date February 2011
 **/
/*****************************************************************************
** Includes
*****************************************************************************/

#include <ros/ros.h>
#include <QtGui>
#include <QMessageBox>
#include <iostream>
#include <math.h>
#include <geometry_msgs/PoseStamped.h>
#include "../include/irc_drone_vision/main_window.hpp"
#include "../include/irc_drone_vision/robitvision.hpp"
#include "../include/irc_drone_vision/robit_master_vision.h"
#include "../include/irc_drone_vision/robit_ransac.h"
#include "../include/irc_drone_vision/robit_ransac_circle.h"

#define     TAB_RED          0
#define     TAB_GREEN        1
#define     TAB_BLUE         2

#define     DP           2      //The inversion ratio of resolution
#define     MIN_DIST     3      //Minimum distance between detected centers
#define     PARAM1       150    //Upper threshold for the internal Canny edge detector
#define     PARAM2       80    //Threshold for center detection
#define     MIN_RADIUS   0      //Minimum radius to be detected. If unknown, put zero as default
#define     MAX_RADIUS   0      //Maximum radius to be detected. If unknown, put zero as default


ColorInfoHsv RED,
             GREEN,
             BLUE;

int ball_erode=0,ball_dilate=0;
int red_dilate=0,green_dilate=0,blue_dilate=0;
int red_erode=0,green_erode=0,blue_erode=0;
int isSaved=0;
float slope[10] = {0, };

/*****************************************************************************
** Namespaces
*****************************************************************************/

using namespace std;
using namespace cv;

namespace irc_drone_vision {

extern ros::Publisher vision_pub;
extern bool isRecv;

using namespace Qt;

/*****************************************************************************
** Implementation [MainWindow]
*****************************************************************************/

MainWindow::MainWindow(int argc, char** argv, QWidget *parent)
	: QMainWindow(parent)
	, qnode(argc,argv)
{
	ui.setupUi(this); // Calling this incidentally connects all ui's triggers to on_...() callbacks in this class.

    QObject::connect(&qnode, SIGNAL(rosShutdown()), this, SLOT(close()));

    qnode.init();
    if(qnode.img_qnode != NULL) qnode.img_qnode = NULL;

    ros::init(argc, argv, "irc_drone_vision");

    ifstream fin_Colorinfo("/home/odroid/catkin_ws/src/irc_drone_vision/colortxt/colorinfo_rgb.txt");
    if(fin_Colorinfo.is_open())
    {

        fin_Colorinfo
                            >>(RED.hmin)
                            >>(RED.hmax)
                            >>(RED.smin)
                            >>(RED.smax)
                            >>(RED.vmin)
                            >>(RED.vmax)
                            >>(GREEN.hmin)
                            >>(GREEN.hmax)
                            >>(GREEN.smin)
                            >>(GREEN.smax)
                            >>(GREEN.vmin)
                            >>(GREEN.vmax)
                            >>(BLUE.hmin)
                            >>(BLUE.hmax)
                            >>(BLUE.smin)
                            >>(BLUE.smax)
                            >>(BLUE.vmin)
                            >>(BLUE.vmax)
                            >>red_erode
                            >>red_dilate
                            >>green_erode
                            >>green_dilate
                            >>blue_erode
                            >>blue_dilate;
    }
    fin_Colorinfo.close();

    set_start_tab(TAB_RED);

    QObject::connect(&qnode, SIGNAL(recvImg()), this, SLOT(updateImg()));
}

MainWindow::~MainWindow() {}

void MainWindow::updateImg()
{
    Mat img_org(*qnode.img_qnode);
    Mat img_rgb,
        img_hsv,
        img_red_bin,
        img_green_bin,
        img_blue_bin;

    if(!qnode.img_qnode -> empty() && !img_org.empty() && isRecv)
    {
        GaussianBlur(img_org, img_rgb, cv::Size(5,5), 2.0);

        const int IMGSIZE=(img_rgb.cols*img_rgb.rows);
        cv::cvtColor(img_rgb,img_hsv,CV_BGR2HSV);

        cv::inRange(img_hsv,
                        Scalar(RED.hmin,RED.smin,RED.vmin),
                        Scalar(RED.hmax,RED.smax,RED.vmax),
                        img_red_bin);

        erode(img_red_bin,
              img_red_bin,
              getStructuringElement(MORPH_RECT, Size(red_erode+1, red_erode+1)));

        dilate(img_red_bin,
               img_red_bin,
               getStructuringElement(MORPH_RECT, Size(red_dilate+1, red_dilate+1)));

        cv::inRange(img_hsv,
                        Scalar(GREEN.hmin,GREEN.smin,GREEN.vmin),
                        Scalar(GREEN.hmax,GREEN.smax,GREEN.vmax),
                        img_green_bin);

        erode(img_green_bin,
              img_green_bin,
              getStructuringElement(MORPH_RECT, Size(green_erode+1, green_erode+1)));

        dilate(img_green_bin,
               img_green_bin,
               getStructuringElement(MORPH_RECT, Size(green_dilate+1, green_dilate+1)));


        cv::inRange(img_hsv,
                        Scalar(BLUE.hmin,BLUE.smin,BLUE.vmin),
                        Scalar(BLUE.hmax,BLUE.smax,BLUE.vmax),
                        img_blue_bin);

        erode(img_blue_bin,
              img_blue_bin,
              getStructuringElement(MORPH_RECT, Size(blue_erode+1, blue_erode+1)));

        dilate(img_blue_bin,
               img_blue_bin,
               getStructuringElement(MORPH_RECT, Size(blue_dilate+1, blue_dilate+1)));


        vector<Vec3f> circles;

        Vec3i rc = 0;
        Vec3i gc = 0;
        Vec3i bc = 0;

        Mat img_red_canny;

        cv::Canny(img_red_bin, img_red_canny, 0, 100);

        cv::HoughCircles(img_red_canny,circles,HOUGH_GRADIENT,DP,MIN_DIST,PARAM1,PARAM2,MIN_RADIUS,MAX_RADIUS);

        for( size_t i = 0; i < circles.size(); i++ )
        {
            rc = circles[i];
            cv::circle( img_org, Point(rc[0], rc[1]), 2, Scalar(0,255,0), 3, LINE_AA);
        }


        Mat img_green_canny;

        cv::Canny(img_green_bin, img_green_canny, 0, 100);

        cv::HoughCircles(img_green_canny,circles,HOUGH_GRADIENT,DP,MIN_DIST,PARAM1,PARAM2,MIN_RADIUS,MAX_RADIUS);

        for( size_t i = 0; i < circles.size(); i++ )
        {
            gc = circles[i];
            cv::circle( img_org, Point(gc[0], gc[1]), 2, Scalar(0,255,0), 3, LINE_AA);
        }



        Mat img_blue_canny;

        cv::Canny(img_blue_bin, img_blue_canny, 0, 100);

        cv::HoughCircles(img_blue_canny,circles,HOUGH_GRADIENT,DP,MIN_DIST,PARAM1,PARAM2,MIN_RADIUS,MAX_RADIUS);

        for( size_t i = 0; i < circles.size(); i++ )
        {
            bc = circles[i];
            cv::circle( img_org, Point(bc[0], bc[1]), 2, Scalar(0,255,0), 3, LINE_AA);
        }

        if(rc[0]!=0 && rc[1]!=0 && gc[0]!=0 && gc[1]!=0 &&rc(0)-gc[0] != 0)
        {
            cv::line(img_org,Point(rc[0],rc[1]),Point(gc[0], gc[1]),Scalar(255,255,255), 1, LINE_AA);
            slope[0] = (float)(gc[1]-rc[1])/(float)(rc(0)-gc[0]);
            slope[0] = (int)(slope[0]*10);

            if(200<slope[0] || slope[0]<-200)
            {
                slope[0] = 9876;
            }
        }

        if(gc[0]!=0 && gc[1]!=0 && bc[0]!=0 && bc[1]!=0 && gc(0)-bc[0] != 0)
        {
            cv::line(img_org,Point(gc[0],gc[1]),Point(bc[0], bc[1]),Scalar(255,255,255), 1, LINE_AA);
            slope[1] = (float)(bc[1]-gc[1])/(float)(gc(0)-bc[0]);
            slope[1] = (int)(slope[1]*10);

            if(200<slope[1] || slope[1]<-200)
            {
                slope[1] = 9876;
            }
        }

        std::cout<<"---------------------"<<std::endl;
        std::cout<<"RED ( "<<rc[0]<<" , "<<rc[1]<<" )"<<"R = "<<rc[2]<<std::endl;
        std::cout<<"GREEN ( "<<gc[0]<<" , "<<gc[1]<<" )"<<"R = "<<gc[2]<<std::endl;
        std::cout<<"BLUE ( "<<bc[0]<<" , "<<bc[1]<<" )"<<"R = "<<bc[2]<<std::endl;
        std::cout<<"SLOPE1 = "<<slope[0]<<std::endl;
        std::cout<<"SLOPE2 = "<<slope[1]<<std::endl;
        std::cout<<"---------------------"<<std::endl;




        QImage image_org((const unsigned char*)(img_org.data), img_org.cols, img_org.rows, QImage::Format_RGB888);
        ui.label_org->setPixmap(QPixmap::fromImage(image_org.rgbSwapped()));

        QImage image_red((const unsigned char*)(img_red_bin.data), img_red_bin.cols, img_red_bin.rows,img_red_bin.step, QImage::Format_Indexed8);
        ui.label_red->setPixmap(QPixmap::fromImage(image_red.rgbSwapped()));

        QImage image_green((const unsigned char*)(img_green_bin.data), img_green_bin.cols, img_green_bin.rows,img_green_bin.step, QImage::Format_Indexed8);
        ui.label_green->setPixmap(QPixmap::fromImage(image_green.rgbSwapped()));

        QImage image_blue((const unsigned char*)(img_blue_bin.data), img_blue_bin.cols, img_blue_bin.rows,img_blue_bin.step, QImage::Format_Indexed8);
        ui.label_blue->setPixmap(QPixmap::fromImage(image_blue.rgbSwapped()));

        delete qnode.img_qnode; if(qnode.img_qnode!=NULL)qnode.img_qnode=NULL;
        isRecv = false;
    }
}

void MainWindow::set_start_tab(int index)
{
    ui.tabWidget->setCurrentIndex(index);

    if (index == TAB_RED)
    {
        ui.horizontalSlider_hmin->setValue(RED.hmin);
        ui.horizontalSlider_hmax->setValue(RED.hmax);
        ui.horizontalSlider_smin->setValue(RED.smin);
        ui.horizontalSlider_smax->setValue(RED.smax);
        ui.horizontalSlider_vmin->setValue(RED.vmin);
        ui.horizontalSlider_vmax->setValue(RED.vmax);
        ui.horizontalSlider_erode->setValue(ball_erode);
        ui.horizontalSlider_dilate->setValue(ball_dilate);

        on_horizontalSlider_hmin_valueChanged(RED.hmin);
        on_horizontalSlider_hmax_valueChanged(RED.hmax);
        on_horizontalSlider_smin_valueChanged(RED.smin);
        on_horizontalSlider_smax_valueChanged(RED.smax);
        on_horizontalSlider_vmin_valueChanged(RED.vmin);
        on_horizontalSlider_vmax_valueChanged(RED.vmax);
        on_horizontalSlider_erode_valueChanged(ball_erode);
        on_horizontalSlider_dilate_valueChanged(ball_dilate);

    }

    else if (index == TAB_GREEN)
    {
        ui.horizontalSlider_hmin->setValue(GREEN.hmin);
        ui.horizontalSlider_hmax->setValue(GREEN.hmax);
        ui.horizontalSlider_smin->setValue(GREEN.smin);
        ui.horizontalSlider_smax->setValue(GREEN.smax);
        ui.horizontalSlider_vmin->setValue(GREEN.vmin);
        ui.horizontalSlider_vmax->setValue(GREEN.vmax);
        ui.horizontalSlider_erode->setValue(ball_erode);
        ui.horizontalSlider_dilate->setValue(ball_dilate);

        on_horizontalSlider_hmin_valueChanged(GREEN.hmin);
        on_horizontalSlider_hmax_valueChanged(GREEN.hmax);
        on_horizontalSlider_smin_valueChanged(GREEN.smin);
        on_horizontalSlider_smax_valueChanged(GREEN.smax);
        on_horizontalSlider_vmin_valueChanged(GREEN.vmin);
        on_horizontalSlider_vmax_valueChanged(GREEN.vmax);
        on_horizontalSlider_erode_valueChanged(ball_erode);
        on_horizontalSlider_dilate_valueChanged(ball_dilate);

    }

    else if (index == TAB_BLUE)
    {
        ui.horizontalSlider_hmin->setValue(BLUE.hmin);
        ui.horizontalSlider_hmax->setValue(BLUE.hmax);
        ui.horizontalSlider_smin->setValue(BLUE.smin);
        ui.horizontalSlider_smax->setValue(BLUE.smax);
        ui.horizontalSlider_vmin->setValue(BLUE.vmin);
        ui.horizontalSlider_vmax->setValue(BLUE.vmax);
        ui.horizontalSlider_erode->setValue(ball_erode);
        ui.horizontalSlider_dilate->setValue(ball_dilate);

        on_horizontalSlider_hmin_valueChanged(BLUE.hmin);
        on_horizontalSlider_hmax_valueChanged(BLUE.hmax);
        on_horizontalSlider_smin_valueChanged(BLUE.smin);
        on_horizontalSlider_smax_valueChanged(BLUE.smax);
        on_horizontalSlider_vmin_valueChanged(BLUE.vmin);
        on_horizontalSlider_vmax_valueChanged(BLUE.vmax);
        on_horizontalSlider_erode_valueChanged(ball_erode);
        on_horizontalSlider_dilate_valueChanged(ball_dilate);

    }
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    if (index == TAB_RED)
    {
        ui.horizontalSlider_hmin->setValue(RED.hmin);
        ui.horizontalSlider_hmax->setValue(RED.hmax);
        ui.horizontalSlider_smin->setValue(RED.smin);
        ui.horizontalSlider_smax->setValue(RED.smax);
        ui.horizontalSlider_vmin->setValue(RED.vmin);
        ui.horizontalSlider_vmax->setValue(RED.vmax);
        ui.horizontalSlider_erode->setValue(ball_erode);
        ui.horizontalSlider_dilate->setValue(ball_dilate);

        on_horizontalSlider_hmin_valueChanged(RED.hmin);
        on_horizontalSlider_hmax_valueChanged(RED.hmax);
        on_horizontalSlider_smin_valueChanged(RED.smin);
        on_horizontalSlider_smax_valueChanged(RED.smax);
        on_horizontalSlider_vmin_valueChanged(RED.vmin);
        on_horizontalSlider_vmax_valueChanged(RED.vmax);
        on_horizontalSlider_erode_valueChanged(ball_erode);
        on_horizontalSlider_dilate_valueChanged(ball_dilate);

    }

    else if (index == TAB_GREEN)
    {
        ui.horizontalSlider_hmin->setValue(GREEN.hmin);
        ui.horizontalSlider_hmax->setValue(GREEN.hmax);
        ui.horizontalSlider_smin->setValue(GREEN.smin);
        ui.horizontalSlider_smax->setValue(GREEN.smax);
        ui.horizontalSlider_vmin->setValue(GREEN.vmin);
        ui.horizontalSlider_vmax->setValue(GREEN.vmax);
        ui.horizontalSlider_erode->setValue(ball_erode);
        ui.horizontalSlider_dilate->setValue(ball_dilate);

        on_horizontalSlider_hmin_valueChanged(GREEN.hmin);
        on_horizontalSlider_hmax_valueChanged(GREEN.hmax);
        on_horizontalSlider_smin_valueChanged(GREEN.smin);
        on_horizontalSlider_smax_valueChanged(GREEN.smax);
        on_horizontalSlider_vmin_valueChanged(GREEN.vmin);
        on_horizontalSlider_vmax_valueChanged(GREEN.vmax);
        on_horizontalSlider_erode_valueChanged(ball_erode);
        on_horizontalSlider_dilate_valueChanged(ball_dilate);

    }

    else if (index == TAB_BLUE)
    {
        ui.horizontalSlider_hmin->setValue(BLUE.hmin);
        ui.horizontalSlider_hmax->setValue(BLUE.hmax);
        ui.horizontalSlider_smin->setValue(BLUE.smin);
        ui.horizontalSlider_smax->setValue(BLUE.smax);
        ui.horizontalSlider_vmin->setValue(BLUE.vmin);
        ui.horizontalSlider_vmax->setValue(BLUE.vmax);
        ui.horizontalSlider_erode->setValue(ball_erode);
        ui.horizontalSlider_dilate->setValue(ball_dilate);

        on_horizontalSlider_hmin_valueChanged(BLUE.hmin);
        on_horizontalSlider_hmax_valueChanged(BLUE.hmax);
        on_horizontalSlider_smin_valueChanged(BLUE.smin);
        on_horizontalSlider_smax_valueChanged(BLUE.smax);
        on_horizontalSlider_vmin_valueChanged(BLUE.vmin);
        on_horizontalSlider_vmax_valueChanged(BLUE.vmax);
        on_horizontalSlider_erode_valueChanged(ball_erode);
        on_horizontalSlider_dilate_valueChanged(ball_dilate);

    }

}
void MainWindow::on_horizontalSlider_dilate_valueChanged(int value)
{
    if(ui.tabWidget->currentIndex() == TAB_RED)
    {
        red_dilate = value;

        ui.label_dilate->setText(QString::number(red_dilate));
    }
    else if(ui.tabWidget->currentIndex() == TAB_GREEN)
    {
        green_dilate = value;

        ui.label_dilate->setText(QString::number(green_dilate));
    }
    else if(ui.tabWidget->currentIndex() == TAB_BLUE)
    {
        blue_dilate = value;

        ui.label_dilate->setText(QString::number(blue_dilate));
    }

}

void MainWindow::on_horizontalSlider_erode_valueChanged(int value)
{
    if(ui.tabWidget->currentIndex() == TAB_RED)
    {
        red_erode = value;

        ui.label_erode->setText(QString::number(red_erode));
    }
    else if(ui.tabWidget->currentIndex() == TAB_GREEN)
    {
        green_erode = value;

        ui.label_erode->setText(QString::number(green_erode));
    }
    else if(ui.tabWidget->currentIndex() == TAB_BLUE)
    {
        blue_erode = value;

        ui.label_erode->setText(QString::number(blue_erode));
    }

}
void MainWindow::on_horizontalSlider_vmax_valueChanged(int value)
{
    if(ui.tabWidget->currentIndex() == TAB_RED)
    {
        RED.vmax = value;

        ui.label_vmax->setText(QString::number(RED.vmax));
    }
    else if(ui.tabWidget->currentIndex() == TAB_GREEN)
    {
        GREEN.vmax = value;

        ui.label_vmax->setText(QString::number(GREEN.vmax));
    }
    else if(ui.tabWidget->currentIndex() == TAB_BLUE)
    {
        BLUE.vmax = value;

        ui.label_vmax->setText(QString::number(BLUE.vmax));
    }

}
void MainWindow::on_horizontalSlider_vmin_valueChanged(int value)
{
    if(ui.tabWidget->currentIndex() == TAB_RED)
    {
        RED.vmin = value;

        ui.label_vmin->setText(QString::number(RED.vmin));
    }
    else if(ui.tabWidget->currentIndex() == TAB_GREEN)
    {
        GREEN.vmin = value;

        ui.label_vmin->setText(QString::number(GREEN.vmin));
    }
    else if(ui.tabWidget->currentIndex() == TAB_BLUE)
    {
        BLUE.vmin = value;

        ui.label_vmin->setText(QString::number(BLUE.vmin));
    }

}
void MainWindow::on_horizontalSlider_smax_valueChanged(int value)
{
    if(ui.tabWidget->currentIndex() == TAB_RED)
    {
        RED.smax = value;

        ui.label_smax->setText(QString::number(RED.smax));
    }
    else if(ui.tabWidget->currentIndex() == TAB_GREEN)
    {
        GREEN.smax = value;

        ui.label_smax->setText(QString::number(GREEN.smax));
    }
    else if(ui.tabWidget->currentIndex() == TAB_BLUE)
    {
        BLUE.smax = value;

        ui.label_smax->setText(QString::number(BLUE.smax));
    }

}
void MainWindow::on_horizontalSlider_smin_valueChanged(int value)
{
    if(ui.tabWidget->currentIndex() == TAB_RED)
    {
        RED.smin = value;

        ui.label_smin->setText(QString::number(RED.smin));
    }
    else if(ui.tabWidget->currentIndex() == TAB_GREEN)
    {
        GREEN.smin = value;

        ui.label_smin->setText(QString::number(GREEN.smin));
    }
    else if(ui.tabWidget->currentIndex() == TAB_BLUE)
    {
        BLUE.smin = value;

        ui.label_smin->setText(QString::number(BLUE.smin));
    }

}
void MainWindow::on_horizontalSlider_hmin_valueChanged(int value)
{
    if(ui.tabWidget->currentIndex() == TAB_RED)
    {
        RED.hmin = value;

        ui.label_hmin->setText(QString::number(RED.hmin));
    }
    else if(ui.tabWidget->currentIndex() == TAB_GREEN)
    {
        GREEN.hmin = value;

        ui.label_hmin->setText(QString::number(GREEN.hmin));
    }
    else if(ui.tabWidget->currentIndex() == TAB_BLUE)
    {
        BLUE.hmin = value;

        ui.label_hmin->setText(QString::number(BLUE.hmin));
    }

}
void MainWindow::on_horizontalSlider_hmax_valueChanged(int value)
{
    if(ui.tabWidget->currentIndex() == TAB_RED)
    {
        RED.hmax = value;

        ui.label_hmax->setText(QString::number(RED.hmax));
    }
    else if(ui.tabWidget->currentIndex() == TAB_GREEN)
    {
        GREEN.hmax = value;

        ui.label_hmax->setText(QString::number(GREEN.hmax));
    }
    else if(ui.tabWidget->currentIndex() == TAB_BLUE)
    {
        BLUE.hmax = value;

        ui.label_hmax->setText(QString::number(BLUE.hmax));
    }

}

void MainWindow::on_pushButton_save_clicked()
{
    cout << "saved " << endl;
    ofstream fout_Colorinfo("/home/odroid/catkin_ws/src/irc_drone_vision/colortxt/colorinfo_rgb.txt");
    if(fout_Colorinfo.is_open())
    {

        fout_Colorinfo
                            <<(RED.hmin)<<endl
                            <<(RED.hmax)<<endl
                            <<(RED.smin)<<endl
                            <<(RED.smax)<<endl
                            <<(RED.vmin)<<endl
                            <<(RED.vmax)<<endl
                            <<(GREEN.hmin)<<endl
                            <<(GREEN.hmax)<<endl
                            <<(GREEN.smin)<<endl
                            <<(GREEN.smax)<<endl
                            <<(GREEN.vmin)<<endl
                            <<(GREEN.vmax)<<endl
                            <<(BLUE.hmin)<<endl
                            <<(BLUE.hmax)<<endl
                            <<(BLUE.smin)<<endl
                            <<(BLUE.smax)<<endl
                            <<(BLUE.vmin)<<endl
                            <<(BLUE.vmax)<<endl
                            <<red_erode<<endl
                            <<red_dilate<<endl
                            <<green_erode<<endl
                            <<green_dilate<<endl
                            <<blue_erode<<endl
                            <<blue_dilate<<endl;
    }
    fout_Colorinfo.close();
    isSaved=false;
}

/*****************************************************************************
** Implementation [Slots]
*****************************************************************************/

}  // namespace irc_drone_vision

