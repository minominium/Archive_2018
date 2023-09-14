#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <stdio.h>

#include <QMainWindow>
#include <QTimer>

#include <opencv2/core/core.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QTimer *mTimer;

    cv::VideoCapture cap;
    cv::Mat image, image_HSV;

private slots:
    void on_pushButton_clicked();
    void timerUpdate();

private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
