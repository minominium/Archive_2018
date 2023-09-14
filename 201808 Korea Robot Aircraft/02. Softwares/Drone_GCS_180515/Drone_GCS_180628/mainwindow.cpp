#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "math.h"
#include <cstdio>
#include <cstdlib>
#include <vector>
#include "spline.h"
#include <conio.h>

#define POINT_CNT   4

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
    std::cout.setf(std::ios::fixed);
    std::cout.precision(6);

    m_sp_lat =  37.170600;
    m_sp_lng = 128.472100;

    m_wp_lat =  37.169500;
    m_wp_lng = 128.472500;

    m_dp_lat =  37.170500;
    m_dp_lng = 128.472400;

    m_ep_lat =  37.170600;
    m_ep_lng = 128.472100;

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
//    std::vector<double> X(POINT_CNT), Y(POINT_CNT), N(3);
    std::vector<double> X(4), Y(4), N(3);
    double XX[4] = {0,};

    X[0] = m_sp_lat;
    X[1] = m_wp_lat;
    X[2] = m_dp_lat;
//    X[3] = m_ep_lat;

    Y[0] = m_sp_lng;
    Y[1] = m_wp_lng;
    Y[2] = m_dp_lng;
//    Y[3] = m_ep_lng;

    N[0] = m_np_lat;
    N[1] = m_np_lng;
    N[2] = m_np_rad;

    double r_lat = (N[2]+10)/88804;


    double slope[10] = {0,};
    slope[0] = (Y[1]-Y[0])/(X[1]-X[0]);
    slope[1] = (Y[1]-Y[2])/(X[1]-X[2]);

    std::cout << "slope = "<<slope[0]<<std::endl;
    std::cout << "slope = "<<slope[1]<<std::endl;

    double root_a = (1/(slope[0]*slope[0]))+1;
    double root_b = (-1)*2.0 *N[0] * root_a;
    double root_c = (N[0]*N[0]) + ((N[0]*N[0])/(slope[0]*slope[0])) - pow(r_lat,2);

    double root_a2 = (1/(slope[1]*slope[1]))+1;
    double root_b2 = (-1)*2.0 *N[0] * root_a2;
    double root_c2 = (N[0]*N[0]) + ((N[0]*N[0])/(slope[1]*slope[1])) - pow(r_lat,2);


    double point_x = root_formula(root_a, root_b, root_c, 1);
    double point_y = (-1/slope[0])*(point_x-N[0])+N[1];

    double point_x2 = root_formula(root_a2, root_b2, root_c2, 0);
    double point_y2 = (-1/slope[1])*(point_x2-N[0])+N[1];


    std::cout << "(x1,y1) = "<<point_x<<", "<<point_y<<std::endl;                                        //left, right circle point
    std::cout << "(x2,y2) = "<<point_x2<<", "<<point_y2<<std::endl;


    tk::spline s;
    s.set_points(X,Y);

    for(int i=0;i<4;i++)
    {
        XX[i] = X[0]+(((X[1] - X[0]) /5) * (i+1));
    }


    double h[10],a,b,c,d,sum,u[10]={0},x[10],F[10],f[10],p,m[10][10]={0},temp;

    x[0] = m_sp_lat;
    x[1] = point_x;
    x[2] = m_wp_lat;
    x[3] = point_x2;
    x[4] = m_dp_lat;

    f[0] = m_sp_lng;
    f[1] = point_y;
    f[2] = m_wp_lng;
    f[3] = point_y2;
    f[4] = m_dp_lng;

    for(int i=POINT_CNT-1;i>0;i--)
    {
//        F[i]=(f[i]-f[i-1])/(x[i]-x[i-1]);
//        h[i-1]=x[i]-x[i-1];

        F[i]=(f[i-1]-f[i])/(x[i-1]-x[i]);
        h[i-1]=x[i-1]-x[i];

//        if(x[i] >= x[i-1]){
//            F[i]=(f[i]-f[i-1])/(x[i]-x[i-1]);
//            h[i-1]=x[i]-x[i-1];
//        }
//        else{
//            F[i]=(f[i-1]-f[i])/(x[i-1]-x[i]);
//            h[i-1]=x[i-1]-x[i];
//        }
//        if(x[i] >= x[i-1]){
//            F[i]=(f[i]-f[i-1])/(x[i]-x[i-1]);
//            h[i-1]=x[i]-x[i-1];
//        }
//        else{
//            F[i]=(f[i-1]-f[i])/(x[i-1]-x[i]);
//            h[i-1]=x[i-1]-x[i];
//        }


    }

    //*********** formation of h, s , f matrix **************//
    for(int i=1;i<POINT_CNT-1;i++)
    {
        m[i][i]=2*(h[i-1]+h[i]);
        if(i!=1)
        {
            m[i][i-1]=h[i-1];
            m[i-1][i]=h[i-1];
        }
        m[i][POINT_CNT-1]=6*(F[i+1]-F[i]);
//        m[i][4]=6*(F[i]-F[i+1]);
    }

    //***********  forward elimination **************//

    for(int i=1;i<POINT_CNT-2;i++)
    {
        temp=(m[i+1][i]/m[i][i]);
        for(int j=1;j<=POINT_CNT-1;j++)
            m[i+1][j]-=temp*m[i][j];
    }

    //*********** back ward substitution *********//
    for(int i=POINT_CNT-2;i>0;i--)
    {
        sum=0;
        for(int j=i;j<=POINT_CNT-2;j++)
            sum+=m[i][j]*u[j];
        u[i]=(m[i][POINT_CNT-1]-sum)/m[i][i];
    }

//    for(p = x[0]; x[0] >= x[1]; p -= 0.0001)
//    {

//        p = (x[0]+x[1]) *0.5;
        p = 0;
/*        for(int i=0;i<4;i++){
            if((x[i]<=p)&&(p<=x[i+1])){
                a=(u[i+1]-u[i])/(6*h[i]);
                b=u[i]/2;
                c=(f[i+1]-f[i])/h[i]-(2*h[i]*u[i]+u[i+1]*h[i])/6;
                d=f[i];
                sum=a*pow((p-x[i]),3)+b*pow((p-x[i]),2)+c*(p-x[i])+d;
            }
        }*/

        ui_webView->page()->mainFrame()->evaluateJavaScript(QString("addMarker(%1, %2)"). arg(QString::number(m_sp_lat,'f',6)). arg(QString::number(m_sp_lng,'f',6)));
/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
        p = x[1] + (x[0]-x[1]) *0.8;
        for(int i=0;i<1;i++){
            a=(u[i+1]-u[i])/(6*h[i]);
            b=u[i]/2;
            c=(f[i]-f[i+1])/h[i]-(2*h[i]*u[i]+u[i+1]*h[i])/6;
            d=f[i];
//            sum=a*pow((x[i]-p),3)+b*pow((x[i]-p),2)+c*(x[i]-p)+d;
            sum=a*pow((p-x[i]),3)+b*pow((p-x[i]),2)+c*(p-x[i])+d;
            ui_webView->page()->mainFrame()->evaluateJavaScript(QString("addMarker(%1, %2)"). arg(QString::number(p,'f',6)). arg(QString::number(sum,'f',6)));
            std::cout<<"abc = "<<a<<","<<b<<","<<c<<","<<d<<std::endl;
            std::cout<<"x[i], x[i+1] = "<<x[0]<<", "<<x[1]<<", |x[0]-p| = "<<x[0]-p<<std::endl;
            std::cout<<"(p,sum) = "<<p<<", "<<sum<<std::endl;
        }
        p = x[1] + (x[0]-x[1]) *0.5;
        for(int i=0;i<1;i++){
            a=(u[i+1]-u[i])/(6*h[i]);
            b=u[i]/2;
            c=(f[i]-f[i+1])/h[i]-(2*h[i]*u[i]+u[i+1]*h[i])/6;
            d=f[i];
            sum=a*pow((p-x[i]),3)+b*pow((p-x[i]),2)+c*(p-x[i])+d;
            ui_webView->page()->mainFrame()->evaluateJavaScript(QString("addMarker(%1, %2)"). arg(QString::number(p,'f',6)). arg(QString::number(sum,'f',6)));
            std::cout<<"abc = "<<a<<","<<b<<","<<c<<","<<d<<std::endl;
            std::cout<<"x[i], x[i+1] = "<<x[0]<<", "<<x[1]<<", |x[0]-p| = "<<x[0]-p<<std::endl;
            std::cout<<"(p,sum) = "<<p<<", "<<sum<<std::endl;
        }
        p = x[1] + (x[0]-x[1]) *0.2;
        for(int i=0;i<1;i++){
            a=(u[i+1]-u[i])/(6*h[i]);
            b=u[i]/2;
            c=(f[i]-f[i+1])/h[i]-(2*h[i]*u[i]+u[i+1]*h[i])/6;
            d=f[i];
            sum=a*pow((p-x[i]),3)+b*pow((p-x[i]),2)+c*(p-x[i])+d;
            ui_webView->page()->mainFrame()->evaluateJavaScript(QString("addMarker(%1, %2)"). arg(QString::number(p,'f',6)). arg(QString::number(sum,'f',6)));
            std::cout<<"abc = "<<a<<","<<b<<","<<c<<","<<d<<std::endl;
            std::cout<<"x[i], x[i+1] = "<<x[0]<<", "<<x[1]<<", |x[0]-p| = "<<x[0]-p<<std::endl;
            std::cout<<"(p,sum) = "<<p<<", "<<sum<<std::endl;
        }
/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
        ui_webView->page()->mainFrame()->evaluateJavaScript(QString("addMarker(%1, %2)"). arg(QString::number(point_x ,'f',6)). arg(QString::number(point_y ,'f',6)));
        std::cout<<"/////////////////////////////////////////"<<std::endl;
/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
        p = x[2] + (x[1]-x[2]) *0.8;
        for(int i=1;i<2;i++){
            a=(u[i+1]-u[i])/(6*h[i]);
//            a=(u[i]-u[i+1])/(6*h[i]);
            b=u[i]/2;
//            c=(f[i+1]-f[i])/h[i]-(2*h[i]*u[i]+u[i+1]*h[i])/6;
//            c=(2*h[i]*u[i]+u[i+1]*h[i])/6 - (f[i]-f[i+1])/h[i];
//            c=((f[i]-f[i+1])/h[i]-(2*h[i]*u[i]+u[i+1]*h[i])/6)*(-1);
            c=(f[i]-f[i+1])/h[i]-(2*h[i]*u[i]+u[i+1]*h[i])/6;
            d=f[i];
            sum=a*pow((p-x[i]),3)+b*pow((p-x[i]),2)+c*(p-x[i])+d;
//            sum=a*pow((x[i]-p),3)+b*pow((x[i]-p),2)+c*(x[i]-p)+d;
            ui_webView->page()->mainFrame()->evaluateJavaScript(QString("addMarker(%1, %2)"). arg(QString::number(p,'f',6)). arg(QString::number(sum,'f',6)));
            std::cout<<"abc = "<<a<<","<<b<<","<<c<<","<<d<<std::endl;
            std::cout<<"x[i], x[i+1] = "<<x[1]<<", "<<x[2]<<", |x[0]-p| = "<<x[1]-p<<std::endl;
            std::cout<<"(p,sum) = "<<p<<", "<<sum<<std::endl;
        }
        p = x[2] + (x[1]-x[2]) *0.5;
        for(int i=1;i<2;i++){
            a=(u[i+1]-u[i])/(6*h[i]);
            b=u[i]/2;
            c=(f[i]-f[i+1])/h[i]-(2*h[i]*u[i]+u[i+1]*h[i])/6;
            d=f[i];
            sum=a*pow((p-x[i]),3)+b*pow((p-x[i]),2)+c*(p-x[i])+d;
            ui_webView->page()->mainFrame()->evaluateJavaScript(QString("addMarker(%1, %2)"). arg(QString::number(p,'f',6)). arg(QString::number(sum,'f',6)));
            std::cout<<"abc = "<<a<<","<<b<<","<<c<<","<<d<<std::endl;
            std::cout<<"x[i], x[i+1] = "<<x[1]<<", "<<x[2]<<", |x[0]-p| = "<<x[1]-p<<std::endl;
            std::cout<<"(p,sum) = "<<p<<", "<<sum<<std::endl;
        }
        p = x[2] + (x[1]-x[2]) *0.2;
        for(int i=1;i<2;i++){
            a=(u[i+1]-u[i])/(6*h[i]);
            b=u[i]/2;
            c=(f[i]-f[i+1])/h[i]-(2*h[i]*u[i]+u[i+1]*h[i])/6;
            d=f[i];
            sum=a*pow((p-x[i]),3)+b*pow((p-x[i]),2)+c*(p-x[i])+d;
            ui_webView->page()->mainFrame()->evaluateJavaScript(QString("addMarker(%1, %2)"). arg(QString::number(p,'f',6)). arg(QString::number(sum,'f',6)));
            std::cout<<"abc = "<<a<<","<<b<<","<<c<<","<<d<<std::endl;
            std::cout<<"x[i], x[i+1] = "<<x[1]<<", "<<x[2]<<", |x[0]-p| = "<<x[1]-p<<std::endl;
            std::cout<<"(p,sum) = "<<p<<", "<<sum<<std::endl;
        }
//    }



//    ui_webView->page()->mainFrame()->evaluateJavaScript(QString("addMarker(%1, %2)"). arg(QString::number(m_sp_lat,'f',6)). arg(QString::number(m_sp_lng,'f',6)));
//    ui_webView->page()->mainFrame()->evaluateJavaScript(QString("addMarker(%1, %2)"). arg(QString::number(point_x ,'f',6)). arg(QString::number(point_y ,'f',6)));
//    ui_webView->page()->mainFrame()->evaluateJavaScript(QString("addMarker(%1, %2)"). arg(QString::number(p,'f',6)). arg(QString::number(sum,'f',6)));
    ui_webView->page()->mainFrame()->evaluateJavaScript(QString("addMarker(%1, %2)"). arg(QString::number(m_wp_lat,'f',6)). arg(QString::number(m_wp_lng,'f',6)));
    ui_webView->page()->mainFrame()->evaluateJavaScript(QString("addMarker(%1, %2)"). arg(QString::number(point_x2,'f',6)). arg(QString::number(point_y2,'f',6)));
    ui_webView->page()->mainFrame()->evaluateJavaScript(QString("addMarker(%1, %2)"). arg(QString::number(m_dp_lat,'f',6)). arg(QString::number(m_dp_lng,'f',6)));


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

   // ui_webView->page()->mainFrame()->evaluateJavaScript(QString("addMarker(%1, %2)").   arg(n_spLat).arg(n_spLng));
    std::cout<<"TEST 1"<<std::endl;
    spline();
//    ui_webView->page()->mainFrame()->evaluateJavaScript(QString("addMarker(%1, %2)").   arg(n_wpLat).arg(n_wpLng));
//    ui_webView->page()->mainFrame()->evaluateJavaScript(QString("addMarker(%1, %2)").   arg(n_dpLat).arg(n_dpLng));

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
//    std::string str = std::to_string(X[0]);
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

}
double MainWindow::root_formula(double a, double b, double c, bool dir)
{
    double temp = (b*b) - (4*a*c);

    if(temp <= 0)    return 1;
    if(temp > 0){
        if(dir)     return (-b - sqrt(pow(b,2)-4*a*c)) / (2*a);
        else        return (-b + sqrt(pow(b,2)-4*a*c)) / (2*a);
    }
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
