/**
 * @file /include/irc_drone_vision/main_window.hpp
 *
 * @brief Qt based gui for irc_drone_vision.
 *
 * @date November 2010
 **/
#ifndef irc_drone_vision_MAIN_WINDOW_H
#define irc_drone_vision_MAIN_WINDOW_H

/*****************************************************************************
** Includes
*****************************************************************************/

#include <QtGui/QMainWindow>
#include "ui_main_window.h"
#include "qnode.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/video.hpp"

/*****************************************************************************
** Namespace
*****************************************************************************/

using namespace std;
using namespace cv;

namespace irc_drone_vision {

/*****************************************************************************
** Interface [MainWindow]
*****************************************************************************/
/**
 * @brief Qt central, all operations relating to the view part here.
 */
class MainWindow : public QMainWindow {
Q_OBJECT

public:
	MainWindow(int argc, char** argv, QWidget *parent = 0);
        ~MainWindow();

public Q_SLOTS:
        void set_start_tab(int index);
        void on_horizontalSlider_dilate_valueChanged(int value);
        void on_horizontalSlider_erode_valueChanged(int value);
        void on_horizontalSlider_vmax_valueChanged(int value);
        void on_horizontalSlider_vmin_valueChanged(int value);
        void on_horizontalSlider_smax_valueChanged(int value);
        void on_horizontalSlider_smin_valueChanged(int value);
        void on_horizontalSlider_hmin_valueChanged(int value);
        void on_horizontalSlider_hmax_valueChanged(int value);
        void on_tabWidget_currentChanged(int index);
        void on_pushButton_save_clicked();

        void updateImg();

private:
	Ui::MainWindowDesign ui;
	QNode qnode;
        QTimer* mTimer;
};

}  // namespace irc_drone_vision

#endif // irc_drone_vision_MAIN_WINDOW_H
