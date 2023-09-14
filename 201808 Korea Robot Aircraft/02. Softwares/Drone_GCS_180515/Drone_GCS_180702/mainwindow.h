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
#include <string>
#include <stdlib.h>
#include "cubicspline.h"

#define UNIX2GPS 315964800
#define POINT_CNT  3
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
    uint unixEpochToUInt;
    std::ofstream winLogFile;
    QByteArray unixTime;

    std::string data_Log_FM;
    std::string data_Log_WP;
    std::string data_Log_Lat;
    std::string data_Log_Long;
    std::string data_Log_Alt;

    double n_spLat;
    double n_spLng;

    double n_wpLat;
    double n_wpLng;

    double n_dpLat;
    double n_dpLng;

    double n_epLat;
    double n_epLng;

    double n_npLat;
    double n_npLng;
    double n_npRad;


    struct Point{
        double lat;
        double lng;
        double rad;
        double slope;
    };

    Point sp;                //start_point
    Point wp;                //way_point
    Point dp;                //delivery_point
    Point ep;                //end_point
    Point nf;                //no_fly zone
    Point L_contact;         //
    Point R_contact;
    double r_lat;
    double h[10]={0,} ,a,b,c,d,sum,u[10]={0, }, x[10]={0, }, F[10]={0, }, f[10]={0, }, p,m[10][10]={{0, }}, temp;

private:
    Ui::MainWindow *ui;

    QWebView *ui_webView;
    CubicSpline m_Spline;
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
    void droneTracking(double lat, double lng);
private slots:
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
    void drawSpline(QString GPS_data);

    void on_ui_check_ep_stateChanged(int arg1);
    void on_ui_button_takeoff_clicked();
    void on_ui_button_landing_clicked();

    void on_ui_button_auto_clicked();

    void on_ui_button_manual_clicked();

signals:
    void spline(Point sp, Point wp, Point dp, Point ep, Point nf);
};

#endif // MAINWINDOW_H
