#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    uiInit();

    data_Log_FM = "0";
    data_Log_WP = "0";
    data_Log_Lat = "0";
    data_Log_Long = "0";
    data_Log_Alt = "0";

    winLogFile.open("C:\\Users\\Minos\\Desktop\\GPS_Log_KwangWoonUniv.asc");
    winLogFile << std::setw(11) << "Flight Mode"
               << std::setw(15) << "Waypoint Num."
               << std::setw(15) << "GPS Second"
               << std::setw(15) << "Latitude"
               << std::setw(15) << "Longitude"
               << std::setw(15) << "Altitude"
               << std::endl;

    mSerial = new QSerialPort(this);

    // Webview Settings
    ui_webView = ui->ui_webView; //ui로?WebView ?정
    ui_webView->load(QUrl("file:///C:/_QT_Project/_map_html/naver_map_2.html"));
    ui_webView->setGeometry(20,20,1000,500); //WebView?치? ?기 ?정

    connect(ui->ui_button_GPSdisplay, SIGNAL(clicked(bool)), this, SLOT(addMarker()));
    connect(ui->ui_button_GPSreset, SIGNAL(clicked(bool)), this, SLOT(resetMarker()));

    // Serial Settings
    connect(ui->ui_button_scan, SIGNAL(clicked()), this, SLOT(on_ui_button_scan_clicked()));
    connect(ui->ui_button_connect, SIGNAL(clicked()), this, SLOT(on_ui_button_connect_clicked()));
    connect(ui->ui_button_disconnect, SIGNAL(clicked()), this, SLOT(on_ui_button_disconnect_clicked()));
    //connect(ui->ui_dial_gripper, SIGNAL(sliderReleased())), this, SLOT(on_ui_dial_gripper_sliderReleased());
    connect(mSerial, SIGNAL(readyRead()), this, SLOT(serial_received()));
    //connect(this, SIGNAL(sigSendData()), this, SLOT(slotSendData()));
    on_ui_button_scan_clicked();
    ui->ui_comboBox_baudrate->addItem("9600");
    ui->ui_comboBox_baudrate->addItem("19200");
    ui->ui_comboBox_baudrate->addItem("38400");
    ui->ui_comboBox_baudrate->addItem("57600");
    ui->ui_comboBox_baudrate->addItem("115200");

    ui->ui_button_disconnect->setEnabled(false);
}
void MainWindow::uiInit()
{
    if(ui->ui_check_ep->isChecked())
    {
        qDebug("checked");
        ui->ui_line_ep_lat->setText(ui->ui_line_sp_lat->text());
        ui->ui_line_ep_lng->setText(ui->ui_line_sp_lng->text());
    }
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

    ui_webView->page()->mainFrame()->evaluateJavaScript(QString("addMarker(%1, %2)").   arg(n_spLat).arg(n_spLng));
    ui_webView->page()->mainFrame()->evaluateJavaScript(QString("addMarker(%1, %2)").   arg(n_wpLat).arg(n_wpLng));
    ui_webView->page()->mainFrame()->evaluateJavaScript(QString("addMarker(%1, %2)").   arg(n_dpLat).arg(n_dpLng));
    ui_webView->page()->mainFrame()->evaluateJavaScript(QString("addMarker(%1, %2)").   arg(n_epLat).arg(n_epLng));
    ui_webView->page()->mainFrame()->evaluateJavaScript(QString("noflyMarker(%1, %2, %3)"). arg(n_npLat).arg(n_npLng).arg(n_npRad));

}

void MainWindow::resetMarker()
{
    QString n_npLat = ui->ui_line_np_lat->text();
    QString n_npLng = ui->ui_line_np_lng->text();

    ui_webView->page()->mainFrame()->evaluateJavaScript(QString("resetMarker(%1, %2)"). arg(n_npLat).arg(n_npLng));
}

void MainWindow::on_ui_check_ep_toggled(bool checked)
{
    if(checked){
        qDebug("checked");
        ui->ui_line_ep_lat->setDisabled(1);
        ui->ui_line_ep_lng->setDisabled(1);
    }
    else{
        qDebug("Not checked");
        ui->ui_line_ep_lat->setDisabled(0);
        ui->ui_line_ep_lng->setDisabled(0);
    }
}


void MainWindow::on_ui_check_emerge_1_clicked(bool checked)
{
    if(checked){
        qDebug("checked");
        ui->ui_button_auto->setDisabled(0);
        ui->ui_button_manual->setDisabled(0);
    }
    else{
        qDebug("Not checked");
        ui->ui_button_auto->setDisabled(1);
        ui->ui_button_manual->setDisabled(1);
    }
}

void MainWindow::on_ui_check_emerge_2_clicked(bool checked)
{
    if(checked){
        qDebug("checked");
        ui->ui_button_takeoff->setDisabled(0);
        ui->ui_button_landing->setDisabled(0);
    }
    else{
        qDebug("Not checked");
        ui->ui_button_takeoff->setDisabled(1);
        ui->ui_button_landing->setDisabled(1);
    }
}

void MainWindow::on_ui_group_toque_clicked(bool checked)
{
    if(checked){
        qDebug("checked");
        ui->ui_radio_on->setDisabled(0);
        ui->ui_radio_off->setDisabled(0);
    }
    else{
        qDebug("Not checked");
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
    unixEpochToInt = unixEpoch.toTime_t();

    QByteArray data_Recv;
    data_Recv = mSerial->readAll();

//    if(data_Recv.length() >= 27 && data_Recv.length() <= 41)
//    {
        QList<QByteArray> data_Recv_Splited = data_Recv.split(',');

//        if(data_Recv_Splited[0] == "225")
//        {
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
//        }
//    }

    winLogFile << std::fixed << std::setprecision(6)
               << std::setw(11) << data_Log_FM
               << std::setw(15) << data_Log_WP
               << std::setw(15) << unixEpochToInt - UNIX2GPS
               << std::setw(15) << data_Log_Lat
               << std::setw(15) << data_Log_Long
               << std::setprecision(1)
               << std::setw(15) << data_Log_Alt
               << std::endl;

    //emit sigSendData();
/*//    if(data_Recv.length() > 5)
//    {
//        if(data_Recv.at(0) == '8'
//        && data_Recv.at(1) == '4'
//        && (data_Recv.at(data_Recv.length() - 3) == '2')
//        && (data_Recv.at(data_Recv.length() - 2) == '2')
//        && (data_Recv.at(data_Recv.length() - 1) == '5'))
//        {

//            ui->ui_textBrowser->append(data_Recv);
//        }


//    }

//    data_Recv.clear();
//////////// 내가 짠 코드 2

//    QByteArray gripperData;
//    gripperData.setNum(ui->ui_dial_gripper->value());

//    QByteArray data_Recv;
//    data_Recv = mSerial->readAll();

//    if(data_Recv.length() > 10)
//    {
//        if((data_Recv.at(0) == '8')
//        && (data_Recv.at(1) == '4')                             // Start Frame Check
//        && (data_Recv.at(data_Recv.length() - 3) == '2')
//        && (data_Recv.at(data_Recv.length() - 2) == '2')
//        && (data_Recv.at(data_Recv.length() - 1) == '5'))       // End Frame Check
//        {
//            ui->ui_textBrowser->append(data_Recv);
//        }
//    }

//    data_Recv.clear();
//    mSerial->write(gripperData);
/////////// 여기까지 내가 짠 코드

//    QByteArray data_Recv;
//    data_Recv = mSerial->readAll();

//    QList<QByteArray> RX_Buffer = data_Recv.split(',');


//    for(int i = 0 ; i < 5 ; i ++)
//    {
//        ui->ui_textBrowser->append(RX_Buffer[i + 2]);
//    }

//    data_Recv.clear();
//    RX_Buffer.clear();
/////////// 영준형 코드*/
}

/*void MainWindow::slotSendData()
{
    QByteArray gripperValue;
    gripperValue.append(QString::number(ui->ui_dial_gripper->value()));
    mSerial->write(gripperValue,
                   gripperValue.size());
    std::cout << gripperValue.size() << std::endl;
    std::cout << "value = " << ui->ui_dial_gripper->value() << std::endl;
    mSerial->clear();
    gripperValue.clear();
}*/


void MainWindow::on_ui_button_waypoint_clicked()
{
    QByteArray waypoint_Data_String;

    waypoint_Data_String.append("84,");
    waypoint_Data_String.append(ui->ui_line_sp_lat->text().remove("."));
    waypoint_Data_String.append(",");
    waypoint_Data_String.append(ui->ui_line_sp_lng->text().remove("."));

    mSerial->write(waypoint_Data_String);
}

void MainWindow::on_ui_dial_gripper_sliderReleased()
{
    QByteArray gripper_Data_String;
    QString qstring_Gripper = QString::number(ui->ui_dial_gripper->value());

    gripper_Data_String.append("85,");
    gripper_Data_String.append(qstring_Gripper);

    mSerial->write(gripper_Data_String);
}
