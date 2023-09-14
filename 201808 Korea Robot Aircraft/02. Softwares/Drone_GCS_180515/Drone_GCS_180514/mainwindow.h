#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWebKitWidgets>
#include <QDebug>
#include <QtSerialPort/qserialport.h>
#include <QtSerialPort/qserialportinfo.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <QDateTime>
#include <fstream>
#include <iomanip>

#define UNIX2GPS 315964800

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QSerialPort* mSerial;
    QDateTime unixEpoch;
    int unixEpochToInt;
    std::ofstream winLogFile;
    QByteArray unixTime;

    std::string data_Log_FM;
    std::string data_Log_WP;
    std::string data_Log_Lat;
    std::string data_Log_Long;
    std::string data_Log_Alt;

private:
    Ui::MainWindow *ui;

    QWebView *ui_webView;

private slots:
    void uiInit();
    void addMarker();
    void resetMarker();
    void on_ui_check_ep_toggled(bool checked);
    void on_ui_check_emerge_1_clicked(bool checked);
    void on_ui_check_emerge_2_clicked(bool checked);
    void on_ui_group_toque_clicked(bool checked);
    void on_ui_button_connect_clicked();
    void on_ui_button_disconnect_clicked();
    void on_ui_button_scan_clicked();

    void send_test();
    void serial_received();
    //void slotSendData();

    void on_ui_button_waypoint_clicked();

    void on_ui_dial_gripper_sliderReleased();

signals:
    //void sigSendData();
};

#endif // MAINWINDOW_H
