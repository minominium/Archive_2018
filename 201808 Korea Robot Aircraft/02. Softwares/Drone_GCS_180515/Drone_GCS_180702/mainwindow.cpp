#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "math.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),

    // YeongWol GPS coordination
    m_sp_lat( 37.171208),
    m_sp_lng(128.471903),
    m_wp_lat( 37.171039),
    m_wp_lng(128.471128),
    m_dp_lat( 37.171039),
    m_dp_lng(128.471961),
    m_ep_lat( 37.171208),
    m_ep_lng(128.471903),
    m_np_lat( 37.171039),
    m_np_lng(128.471544),
    m_np_rad( 30.0),

    // KW Univ. GPS coordination
    /*m_sp_lat( 37.620200),
    m_sp_lng(127.059090),
    m_wp_lat( 37.619875),
    m_wp_lng(127.059241),
    m_dp_lat( 37.620140),
    m_dp_lng(127.059428),
    m_ep_lat( 37.620250),
    m_ep_lng(127.059137),
    m_np_lat( 37.620030),
    m_np_lng(127.059290),
    m_np_rad( 5.0),*/
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    uiInit();

    data_Log_FM = "0";
    data_Log_WP = "0";
    data_Log_Lat = "0";
    data_Log_Long = "0";
    data_Log_Alt = "0";

    winLogFile.open("C:\\Users\\hinmo\\Documents\\QtWS\\Drone_GCS_180515\\GPS_Log_KwangWoonUniv.asc");
    winLogFile << std::setw(11) << "Flight Mode"
               << std::setw(15) << "Waypoint Num."
               << std::setw(15) << "GPS Second"
               << std::setw(15) << "Latitude"
               << std::setw(15) << "Longitude"
               << std::setw(15) << "Altitude"
               << std::endl;

    mSerial = new QSerialPort(this);

    // Webview Settings
    ui_webView = ui->ui_webView;
    ui_webView->load(QUrl("file:///C:/_QT_Project/_map_html/mapAPI.html"));
    ui_webView->setGeometry(20,20,1000,500);
    connect(this, SIGNAL(spline(Point,Point,Point,Point,Point)), &m_Spline, SLOT(runSpline(Point,Point,Point,Point,Point)));
    connect(&m_Spline, SIGNAL(sigDrawSpline(QString)), this, SLOT(drawSpline(QString)));

    connect(ui->ui_button_GPSdisplay, SIGNAL(clicked(bool)), this, SLOT(addMarker()));
    connect(ui->ui_button_GPSreset, SIGNAL(clicked(bool)), this, SLOT(resetMarker()));
    connect(ui->ui_check_ep, SIGNAL(stateChanged(int)), this, SLOT(on_ui_check_ep_stateChanged(int)));

    // Serial Settings
    connect(ui->ui_button_scan, SIGNAL(clicked()), this, SLOT(on_ui_button_scan_clicked()));
    connect(ui->ui_button_connect, SIGNAL(clicked()), this, SLOT(on_ui_button_connect_clicked()));
    connect(ui->ui_button_disconnect, SIGNAL(clicked()), this, SLOT(on_ui_button_disconnect_clicked()));
    connect(mSerial, SIGNAL(readyRead()), this, SLOT(serial_received()));
    on_ui_button_scan_clicked();
    ui->ui_comboBox_baudrate->addItem("57600");
    ui->ui_comboBox_baudrate->addItem("9600");
    ui->ui_comboBox_baudrate->addItem("19200");
    ui->ui_comboBox_baudrate->addItem("38400");
    ui->ui_comboBox_baudrate->addItem("57600");
    ui->ui_comboBox_baudrate->addItem("115200");

    ui->ui_button_disconnect->setEnabled(false);
}
void MainWindow::uiInit()
{
    setWindowFlags(Qt::CustomizeWindowHint);
    QWidget::showMaximized();

    std::cout.setf(std::ios::fixed);
    std::cout.precision(6);

    ui->ui_line_sp_lat->setText(QString::number(m_sp_lat,'f',6));
    ui->ui_line_sp_lng->setText(QString::number(m_sp_lng,'f',6));
    ui->ui_line_wp_lat->setText(QString::number(m_wp_lat,'f',6));
    ui->ui_line_wp_lng->setText(QString::number(m_wp_lng,'f',6));
    ui->ui_line_dp_lat->setText(QString::number(m_dp_lat,'f',6));
    ui->ui_line_dp_lng->setText(QString::number(m_dp_lng,'f',6));
    ui->ui_line_ep_lat->setText(QString::number(m_ep_lat,'f',6));
    ui->ui_line_ep_lng->setText(QString::number(m_ep_lng,'f',6));
    ui->ui_line_np_lat->setText(QString::number(m_np_lat,'f',6));
    ui->ui_line_np_lng->setText(QString::number(m_np_lng,'f',6));
    ui->ui_line_np_rad->setText(QString::number(m_np_rad));
}

MainWindow::~MainWindow()
{
    mSerial->close();
    winLogFile.close();
    delete ui;
}

void MainWindow::addMarker()
{

    QString n_spLat = ui->ui_line_sp_lat->text();
    QString n_spLng = ui->ui_line_sp_lng->text();

    QString n_wpLat = ui->ui_line_wp_lat->text();
    QString n_wpLng = ui->ui_line_wp_lng->text();

    QString n_dpLat = ui->ui_line_dp_lat->text();
    QString n_dpLng = ui->ui_line_dp_lng->text();

    QString n_epLat = ui->ui_line_ep_lat->text();
    QString n_epLng = ui->ui_line_ep_lng->text();

    QString n_npLat = ui->ui_line_np_lat->text();
    QString n_npLng = ui->ui_line_np_lng->text();
    QString n_npRad = ui->ui_line_np_rad->text();

    sp.lat = n_spLat.toDouble();
    sp.lng = n_spLng.toDouble();

    wp.lat = n_wpLat.toDouble();
    wp.lng = n_wpLng.toDouble();

    dp.lat = n_dpLat.toDouble();
    dp.lng = n_dpLng.toDouble();

    ep.lat = n_epLat.toDouble();
    ep.lng = n_epLng.toDouble();

    nf.lat = n_npLat.toDouble();
    nf.lng = n_npLng.toDouble();
    nf.rad = n_npRad.toDouble();

    spline(sp, wp, dp, ep, nf);

    ui_webView->page()->mainFrame()->evaluateJavaScript(QString("noflyMarker(%1, %2, %3)"). arg(n_npLat).arg(n_npLng).arg(n_npRad));

    std::stringstream data_Lat(data_Log_Lat), data_Lng(data_Log_Long);
    double ss2data_Lat, ss2data_Lng;

    data_Lat >> ss2data_Lat;
    data_Lng >> ss2data_Lng;

    //droneTracking(ss2data_Lat, ss2data_Lng);
    // Waypoint Information Transmit
    QByteArray waypointInfo_String;
    waypointInfo_String.append("84,780,");
    for(int count = 0; count < 9; count++)
    {
        waypointInfo_String.append(QString::number(waypointData_Lat[count], 'f', 6) + ",");
        waypointInfo_String.append(QString::number(waypointData_Lng[count], 'f', 6) + ",");
    }
    ui->ui_textBrowserAlt_3->setText(waypointInfo_String);

    mSerial->write(waypointInfo_String);
}

void MainWindow::resetMarker()
{
    QString n_npLat = ui->ui_line_np_lat->text();
    QString n_npLng = ui->ui_line_np_lng->text();

    ui_webView->page()->mainFrame()->evaluateJavaScript(QString("resetMarker(%1, %2)"). arg(n_npLat).arg(n_npLng));
}

void MainWindow::droneTracking(double lat, double lng)
{
    ui_webView->page()->mainFrame()->evaluateJavaScript(QString("droneTracking(%1, %2)"). arg(QString::number(lat,'f',6)). arg(QString::number(lng,'f',6)));
}

void MainWindow::on_ui_check_ep_toggled(bool checked)
{
    if(checked){
//        qDebug("checked");
        ui->ui_line_ep_lat->setDisabled(1);
        ui->ui_line_ep_lng->setDisabled(1);
    }
    else{
//        qDebug("Not checked");
        ui->ui_line_ep_lat->setDisabled(0);
        ui->ui_line_ep_lng->setDisabled(0);
    }
}


void MainWindow::on_ui_check_emerge_1_clicked(bool checked)
{
    if(checked){
//        qDebug("checked");
        ui->ui_button_auto->setDisabled(0);
        ui->ui_button_manual->setDisabled(0);
    }
    else{
//        qDebug("Not checked");
        ui->ui_button_auto->setDisabled(1);
        ui->ui_button_manual->setDisabled(1);
    }
}

void MainWindow::on_ui_check_emerge_2_clicked(bool checked)
{
    if(checked){
//        qDebug("checked");
        ui->ui_button_takeoff->setDisabled(0);
        ui->ui_button_landing->setDisabled(0);
    }
    else{
//        qDebug("Not checked");
        ui->ui_button_takeoff->setDisabled(1);
        ui->ui_button_landing->setDisabled(1);
    }
}

void MainWindow::on_ui_group_toque_clicked(bool checked)
{
    if(checked){
//        qDebug("checked");
        ui->ui_radio_on->setDisabled(0);
        ui->ui_radio_off->setDisabled(0);
    }
    else{
//        qDebug("Not checked");
        ui->ui_radio_on->setDisabled(1);
        ui->ui_radio_off->setDisabled(1);
    }
}

void MainWindow::on_ui_button_connect_clicked()
{
    int baudrate = ui->ui_comboBox_baudrate->currentText().toInt();

    if(!mSerial->isOpen())
    {
        mSerial->setPortName(ui->ui_comboBox_port->currentText());
        mSerial->setBaudRate(baudrate);
        mSerial->setDataBits(QSerialPort::Data8);
        mSerial->setParity(QSerialPort::NoParity);
        mSerial->setStopBits(QSerialPort::OneStop);
        mSerial->setFlowControl(QSerialPort::NoFlowControl);

        mSerial->open(QIODevice::ReadWrite);
        send_test();

        ui->ui_button_connect->setEnabled(false);
        ui->ui_button_disconnect->setEnabled(true);
    }
}

void MainWindow::on_ui_button_disconnect_clicked()
{
    if(mSerial->isOpen())
    {
        mSerial->close();

        ui->ui_button_connect->setEnabled(true);
        ui->ui_button_disconnect->setEnabled(false);
        ui->ui_textBrowser->clear();
        ui->ui_textBrowserWP->clear();
        ui->ui_textBrowserLat->clear();
        ui->ui_textBrowserLong->clear();
        ui->ui_textBrowserAlt->clear();
    }
}

void MainWindow::on_ui_button_scan_clicked()
{
    ui->ui_comboBox_port->clear();
    foreach(const QSerialPortInfo& mSerialPortInfo, QSerialPortInfo::availablePorts())
    {
        ui->ui_comboBox_port->addItem(mSerialPortInfo.portName());
    }
}

void MainWindow::send_test()
{
    mSerial->write("It's test line");
}

void MainWindow::serial_received()
{
    // Get Unix timestamp to convert to GPS timestamp
    unixEpoch = QDateTime::currentDateTime();
    unixEpochToUInt = unixEpoch.toTime_t();

    QByteArray data_Recv;
    data_Recv = mSerial->readAll();

    if(data_Recv.length() >= 27 && data_Recv.length() <= 45 )
    {
        QList<QByteArray> data_Recv_Splited = data_Recv.split(',');

        if(data_Recv_Splited[0].toInt() == 225)
        {
            ui->ui_textBrowser->append(data_Recv_Splited[1]);
            ui->ui_textBrowserWP->append(data_Recv_Splited[2]);
            ui->ui_textBrowserLat->append(data_Recv_Splited[3]);
            ui->ui_textBrowserLong->append(data_Recv_Splited[4]);
            ui->ui_textBrowserAlt->append(data_Recv_Splited[5]);

            data_Log_FM = data_Recv_Splited[1].toStdString();
            data_Log_WP = data_Recv_Splited[2].toStdString();
            data_Log_Lat = data_Recv_Splited[3].toStdString();
            data_Log_Long = data_Recv_Splited[4].toStdString();
            data_Log_Alt = data_Recv_Splited[5].toStdString();
        }
    }

    winLogFile << std::fixed << std::setprecision(6)
               << std::setw(11) << data_Log_FM
               << std::setw(15) << data_Log_WP
               << std::setw(15) << unixEpochToUInt - UNIX2GPS
               << std::setw(15) << data_Log_Lat
               << std::setw(15) << data_Log_Long
               << std::setprecision(1)
               << std::setw(15) << data_Log_Alt
               << std::endl;
}

void MainWindow::drawSpline(QString GPS_data)
{
    ui->ui_webView->page()->mainFrame()->evaluateJavaScript(GPS_data);
}

void MainWindow::on_ui_check_ep_stateChanged(int arg1)
{
    if(!arg1)
    {
        ui->ui_line_ep_lat->setText(ui->ui_line_sp_lat->text());
        ui->ui_line_ep_lng->setText(ui->ui_line_sp_lng->text());
    }
}

void MainWindow::on_ui_button_takeoff_clicked()
{
    QByteArray takeOff_Command_String;
    takeOff_Command_String.append("700");
    mSerial->write(takeOff_Command_String);
}

void MainWindow::on_ui_button_landing_clicked()
{
    QByteArray land_Command_String;
    land_Command_String.append("500");
    mSerial->write(land_Command_String);
}

void MainWindow::on_ui_button_auto_clicked()
{
    QByteArray mission_Command_String;
    mission_Command_String.append("600");
    mSerial->write(mission_Command_String);
}

void MainWindow::on_ui_button_manual_clicked()
{
    QByteArray stabilized_Command_String;
    stabilized_Command_String.append("400");
    mSerial->write(stabilized_Command_String);
}
