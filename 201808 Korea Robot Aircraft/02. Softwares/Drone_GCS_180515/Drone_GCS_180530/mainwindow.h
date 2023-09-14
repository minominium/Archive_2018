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
#include "spline.h"
#include <string>
#include <stdlib.h>


#define UNIX2GPS 315964800
#define POINT_CNT  4
#define SPLINE_CNT  40

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

    double m_sp_lat;
    double m_sp_lng;
    double m_wp_lat;
    double m_wp_lng;
    double m_dp_lat;
    double m_dp_lng;
    double m_ep_lat;
    double m_ep_lng;
    double m_np_lat;
    double m_np_lng;
    double m_np_rad;

    void uiInit();

private slots:
    void spline();
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
};

#endif // MAINWINDOW_H
