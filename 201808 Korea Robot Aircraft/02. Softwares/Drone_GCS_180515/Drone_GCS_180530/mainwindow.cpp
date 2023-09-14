#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "math.h"
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
    ui_webView = ui->ui_webView; //ui��?WebView ?��
    ui_webView->load(QUrl("file:///C:/_QT_Project/_map_html/naver_map_2.html"));
    ui_webView->setGeometry(20,20,1000,500); //WebView?ġ? ?�� ?��

    connect(ui->ui_button_GPSdisplay, SIGNAL(clicked(bool)), this, SLOT(addMarker()));
    connect(ui->ui_button_GPSreset, SIGNAL(clicked(bool)), this, SLOT(resetMarker()));

    // Serial Settings
    connect(ui->ui_button_scan, SIGNAL(clicked()), this, SLOT(on_ui_button_scan_clicked()));
    connect(ui->ui_button_connect, SIGNAL(clicked()), this, SLOT(on_ui_button_connect_clicked()));
    connect(ui->ui_button_disconnect, SIGNAL(clicked()), this, SLOT(on_ui_button_disconnect_clicked()));
    connect(mSerial, SIGNAL(readyRead()), this, SLOT(serial_received()));
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
//    m_sp_lat =  37.170601;
//    m_sp_lng = 128.472102;

//    m_wp_lat =  37.170003;
//    m_wp_lng = 128.471850;

//    m_dp_lat =  37.170500;
//    m_dp_lng = 128.472400;

//    m_ep_lat =  37.167400;
//    m_ep_lng = 128.469000;



    m_sp_lat =  37.170600;
    m_sp_lng = 128.472100;

    m_wp_lat =  37.169500;
    m_wp_lng = 128.472500;

    m_dp_lat =  37.170500;
    m_dp_lng = 128.472400;

    m_ep_lat =  37.170600;
    m_ep_lng = 128.472100;

//    m_sp_lat =  37.170600;
//    m_sp_lng = 128.472000;

//    m_wp_lat =  37.171000;
//    m_wp_lng = 128.472600;

//    m_dp_lat =  37.170800;
//    m_dp_lng = 128.472400;

//    m_ep_lat =  37.170600;
//    m_ep_lng = 128.472000;

    m_np_lat =  37.170000;
    m_np_lng = 128.472250;
    m_np_rad =         30;

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

    if(ui->ui_check_ep->isChecked())
    {
        ui->ui_line_ep_lat->setText(ui->ui_line_sp_lat->text());
        ui->ui_line_ep_lng->setText(ui->ui_line_sp_lng->text());
    }



}
void MainWindow::spline()
{
//    for(int i = 0; i < SPLINE_CNT-1; i++)
//    for(int i = 0; i < 3; i++)
//    {
//        ui_webView->page()->mainFrame()->evaluateJavaScript(QString("spline(%1, %2)").arg(X[i]).arg(Y[i]));
//    }
//    ui_webView->page()->mainFrame()->evaluateJavaScript(QString("spline(%1, %2)").arg(QString::number(X[0])).arg(QString::number(Y[0])));

    double d_XX[10] = {0,};
    int i_sp_lat = ui->ui_line_sp_lat->text().remove(".").toInt();
    int i_wp_lat = ui->ui_line_wp_lat->text().remove(".").toInt();

    std::cout.setf(std::ios::fixed);
    std::cout.precision(6);

    d_XX[0] = (i_wp_lat + (((i_sp_lat - i_wp_lat) / 5) * 4)) / 1000000.000000;
    d_XX[1] = (i_wp_lat + (((i_sp_lat - i_wp_lat) / 5) * 3)) / 1000000.000000;
    d_XX[2] = (i_wp_lat + (((i_sp_lat - i_wp_lat) / 5) * 2)) / 1000000.000000;
    d_XX[3] = (i_wp_lat + (((i_sp_lat - i_wp_lat) / 5) * 1)) / 1000000.000000;

    std::cout << "i_sp= " << i_sp_lat << " i_wp= " << i_wp_lat << std::endl;
    std::cout << "d_XX0= " << d_XX[0] << " d_XX1= " << d_XX[1] << " d_XX2= " << d_XX[2] << std::endl;

    std::vector<double> X(POINT_CNT), Y(POINT_CNT), N(3), YY(POINT_CNT);

    X[0] = m_sp_lat;
    X[1] = m_wp_lat;
    X[2] = m_dp_lat;
    X[3] = m_ep_lat;

    Y[0] = m_sp_lng;
    Y[1] = m_wp_lng;
    Y[2] = m_dp_lng;
    Y[3] = m_ep_lng;

    N[0] = m_np_lat;
    N[1] = m_np_lng;
    N[2] = m_np_rad;

    tk::spline s;
    s.set_points(X,Y);    // currently it is required that X is already sorted

    std::cout << "s0= " << s(d_XX[0]) << " s1= " << s(d_XX[1]) << " s2= " << s(d_XX[2]) << std::endl;
    std::cout << "s0= " << s.deriv(1,d_XX[0]) << " s1= " << s.deriv(2,d_XX[1]) << " s2= " << s.deriv(3,d_XX[2]) << std::endl;

    ui_webView->page()->mainFrame()->evaluateJavaScript(QString("addMarker(%1, %2)").arg(QString::number(d_XX[0],'6')).arg(QString::number(s(d_XX[0]),'6')));
    ui_webView->page()->mainFrame()->evaluateJavaScript(QString("addMarker(%1, %2)").arg(QString::number(d_XX[1],'6')).arg(QString::number(s(d_XX[1]),'6')));
    ui_webView->page()->mainFrame()->evaluateJavaScript(QString("addMarker(%1, %2)").arg(QString::number(d_XX[2],'6')).arg(QString::number(s(d_XX[2]),'6')));
    ui_webView->page()->mainFrame()->evaluateJavaScript(QString("addMarker(%1, %2)").arg(QString::number(d_XX[3],'6')).arg(QString::number(s(d_XX[3]),'6')));

/*//    XX[0] = X[0] + (((X[1]-X[0]) / 4) * 1);
//    XX[1] = X[0] + (((X[1]-X[0]) / 4) * 2);
//    XX[2] = X[0] + (((X[1]-X[0]) / 4) * 3);

//    ui_webView->page()->mainFrame()->evaluateJavaScript(QString("addMarker(%1, %2)").arg(QString::number(XX[0],'f',6)).arg(QString::number(s(XX[0]),'f',6)));
//    ui_webView->page()->mainFrame()->evaluateJavaScript(QString("addMarker(%1, %2)").arg(QString::number(XX[1],'f',6)).arg(QString::number(s(XX[1]),'f',6)));
//    ui_webView->page()->mainFrame()->evaluateJavaScript(QString("addMarker(%1, %2)").arg(QString::number(XX[2],'f',6)).arg(QString::number(s(XX[1]),'f',6)));


//    ui_webView->page()->mainFrame()->evaluateJavaScript(QString("addMarker(%1, %2)").arg(QString::number(X[0],'f',6)).arg(QString::number(s(X[0]),'f',6)));
//    ui_webView->page()->mainFrame()->evaluateJavaScript(QString("addMarker(%1, %2)").arg(QString::number(X[1],'f',6)).arg(QString::number(s(X[1]),'f',6)));
//    ui_webView->page()->mainFrame()->evaluateJavaScript(QString("addMarker(%1, %2)").arg(QString::number(X[2],'f',6)).arg(QString::number(s(X[2]),'f',6)));
//    ui_webView->page()->mainFrame()->evaluateJavaScript(QString("addMarker(%1, %2)").arg(QString::number(X[3],'f',6)).arg(QString::number(s(X[3]),'f',6)));

    for(int i = 0 ,ii = 10; i < ii-1; i++)
    {
        XX[i] = X[0] + (((X[1]-X[0]) * (i+1)) / ii);
        //ui_webView->page()->mainFrame()->evaluateJavaScript(QString("addMarker(%1, %2)").arg(QString::number(XX[i],'f',6)).arg(QString::number(s(XX[i]),'f',6)));
        ui_webView->page()->mainFrame()->evaluateJavaScript(QString("addMarker(%1, %2)").arg(QString::number(XX[i],'f',6)).arg(QString::number(s(X[0]),'f',6)));

        std::cout << " xx= " << XX[i] << " s(xx)= " << s(XX[i]) << std::endl;



        std::cout << " x1= " << X[1] << " x0= " << X[0]
                << " b= " << (X[1]-X[0]) * double(i+1)
                << " c= " << ((X[1]-X[0]) * double(i+1)) / double(ii)
                << " d= " << X[0] + ((X[1]-X[0]) * double(i+1)) / double(ii)
                << " e= " << X[0] + ((X[1]-X[0]) * (i+1)) / ii
                << std::endl;

    }*/


    std::cout << "X[0]= "  << X[0] << "Y[0]= " <<   Y[0] << std::endl;
    std::cout << "X[1]= "  << X[1] << "Y[1]= " <<   Y[1] << std::endl;
    /*    std::cout << "XX[0]= " << XX[0] << "YY[0]= " << s(XX[0]) << std::endl;
    std::cout << "XX[0]= " << XX[1] << "YY[0]= " << s(XX[1]) << std::endl;
    std::cout << "XX[0]= " << XX[2] << "YY[0]= " << s(XX[2]) << std::endl;*/
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
    spline();
    ui_webView->page()->mainFrame()->evaluateJavaScript(QString("addMarker(%1, %2)").   arg(n_wpLat).arg(n_wpLng));
    ui_webView->page()->mainFrame()->evaluateJavaScript(QString("addMarker(%1, %2)").   arg(n_dpLat).arg(n_dpLng));
    ui_webView->page()->mainFrame()->evaluateJavaScript(QString("addMarker(%1, %2)").   arg(n_epLat).arg(n_epLng));
    ui_webView->page()->mainFrame()->evaluateJavaScript(QString("noflyMarker(%1, %2, %3)"). arg(n_npLat).arg(n_npLng).arg(n_npRad));



//    uiInit();




    /*
    X[0] = ui->ui_line_sp_lat->text();
    X[1] = ui->ui_line_wp_lat->text();
    X[2] = ui->ui_line_dp_lat->text();
    X[3] = ui->ui_line_ep_lat->text();

    Y[0] = ui->ui_line_sp_lng->text();
    Y[1] = ui->ui_line_wp_lng->text();
    Y[2] = ui->ui_line_dp_lng->text();
    Y[3] = ui->ui_line_ep_lng->text();

    N[0] = ui->ui_line_np_lat->text();
    N[1] = ui->ui_line_np_lng->text();
    N[2] = ui->ui_line_np_rad->text();
*/
/*//    std::string str = std::to_string(X[0]);
//    qDebug() << str;


//    std::string A = "123.123456";
//    double b;
//
//    std::cout << "Y[0] = " << Y[0] << std::endl;
//    qDebug() << X[0] << atof(Y);

//    double d = std::strtod(Y[0]);
//    std::cout << "Y[0] = " << b << std::endl;

//    double x = (X[1]+X[2]) * 0.5;



//    for(int i = 0; i < POINT_CNT-1; i++){
//        ui_webView->page()->mainFrame()->evaluateJavaScript(QString("addMarker(%1, %2)").   arg(QString::number(X[i])).arg(QString::number(Y[i])));
//        qDebug()<<X[i]<<Y[i];
//    }
//    ui_webView->page()->mainFrame()->evaluateJavaScript(QString("noflyMarker(%1, %2, %3)"). arg(QString::number(N[0])).arg(QString::number(N[1])).arg(QString::number(N[2])));
*/
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

    if(data_Recv.length() >= 28 && data_Recv.length() <= 31)
    {
        QList<QByteArray> data_Recv_Splited = data_Recv.split(',');

        ui->ui_textBrowser->append(data_Recv_Splited[0]);
        ui->ui_textBrowserWP->append(data_Recv_Splited[1]);
        ui->ui_textBrowserLat->append(data_Recv_Splited[2]);
        ui->ui_textBrowserLong->append(data_Recv_Splited[3]);
        ui->ui_textBrowserAlt->append(data_Recv_Splited[4]);

        data_Log_FM = data_Recv_Splited[0].toStdString();
        data_Log_WP = data_Recv_Splited[1].toStdString();
        data_Log_Lat = data_Recv_Splited[2].toStdString();
        data_Log_Long = data_Recv_Splited[3].toStdString();
        data_Log_Alt = data_Recv_Splited[4].toStdString();
    }

    winLogFile << std::fixed << std::setprecision(6)
               << std::setw(11) << data_Log_FM
               << std::setw(15) << data_Log_WP
               << std::setw(15) << unixEpochToInt - UNIX2GPS
               << std::setw(15) << data_Log_Lat
               << std::setw(15) << data_Log_Long
               << std::setprecision(1)
               << std::setw(15) << data_Log_Alt
               << std::endl;

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
//////////// ���� § �ڵ� 2

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
/////////// �������� ���� § �ڵ�

//    QByteArray data_Recv;
//    data_Recv = mSerial->readAll();

//    QList<QByteArray> RX_Buffer = data_Recv.split(',');


//    for(int i = 0 ; i < 5 ; i ++)
//    {
//        ui->ui_textBrowser->append(RX_Buffer[i + 2]);
//    }

//    data_Recv.clear();
//    RX_Buffer.clear();
/////////// ������ �ڵ�*/
}

