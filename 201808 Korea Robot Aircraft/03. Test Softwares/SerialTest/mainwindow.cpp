#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    uiInit();

    connect(this, SIGNAL(isSerialOpened(QString, int)), &serial, SLOT(serialOpen(QString, int)));
    connect(this, SIGNAL(isSerialClosed()), &serial, SLOT(serialClose()));
}

void MainWindow::uiInit()
{
    addPort();
    addBaudrate();
}

void MainWindow::addPort()
{
    QList<QSerialPortInfo> port_list= serial.getPort();
    QSerialPortInfo temp_port;

    foreach(temp_port, port_list)
    {
        ui->comboBox_set_port->addItem(temp_port.portName());
    }
}

void MainWindow::addBaudrate()
{
    ui->comboBox_set_baudrate->addItem(QString::number(QSerialPort::Baud1200));
    ui->comboBox_set_baudrate->addItem(QString::number(QSerialPort::Baud2400));
    ui->comboBox_set_baudrate->addItem(QString::number(QSerialPort::Baud4800));
    ui->comboBox_set_baudrate->addItem(QString::number(QSerialPort::Baud9600));
    ui->comboBox_set_baudrate->addItem(QString::number(QSerialPort::Baud19200));
    ui->comboBox_set_baudrate->addItem(QString::number(QSerialPort::Baud38400));
    ui->comboBox_set_baudrate->addItem(QString::number(QSerialPort::Baud57600));
    ui->comboBox_set_baudrate->addItem(QString::number(QSerialPort::Baud115200));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_connect_clicked()
{
    if(isConnected == false)
    {
        ui->comboBox_set_port->setDisabled(true);
        ui->comboBox_set_baudrate->setDisabled(true);
        ui->pushButton_connect->setText("disconnect");
        isConnected = true;

        emit isSerialOpened(ui->comboBox_set_port->currentText(), ui->comboBox_set_baudrate->currentText().toInt());
    }
    else
    {
        ui->comboBox_set_port->setDisabled(false);
        ui->comboBox_set_baudrate->setDisabled(false);
        ui->pushButton_connect->setText("connect");
        isConnected = false;

        emit isSerialClosed();
    }
}
