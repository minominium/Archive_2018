#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mSerial = new QSerialPort(this);

    connect(ui->pushButtonRescan, SIGNAL(clicked()), this, SLOT(on_pushButtonRescan_clicked()));
    connect(ui->pushButtonConnect, SIGNAL(clicked()), this, SLOT(on_pushButtonConnect_clicked()));
    connect(ui->pushButtonSend, SIGNAL(clicked()), this, SLOT(on_pushButtonSend_clicked()));
    connect(mSerial, SIGNAL(readyRead()), this, SLOT(serial_received()));

    on_pushButtonRescan_clicked();
}

MainWindow::~MainWindow()
{
    mSerial->close();
    delete ui;
}

void MainWindow::on_pushButtonRescan_clicked()
{
    ui->comboBoxPort->clear();
    foreach (const QSerialPortInfo &mSerialPortInfo, QSerialPortInfo::availablePorts())
    {
        ui->comboBoxPort->addItem(mSerialPortInfo.portName());
    }
}

void MainWindow::on_pushButtonConnect_clicked()
{  
    mSerial->setPortName(ui->comboBoxPort->currentText());
    mSerial->setBaudRate(QSerialPort::Baud115200);
    mSerial->setDataBits(QSerialPort::Data8);
    mSerial->setParity(QSerialPort::NoParity);
    mSerial->setStopBits(QSerialPort::OneStop);
    mSerial->setFlowControl(QSerialPort::NoFlowControl);

    mSerial->open(QIODevice::ReadWrite);

    mSerial->write(sendData);

    ui->pushButtonConnect->setEnabled(false);
}

void MainWindow::on_pushButtonSend_clicked()
{
    sendData = ui->plainTextEditSend->;
    mSerial->write(sendData);
}

void MainWindow::serial_send()
{

}

void MainWindow::serial_received()
{
    ui->textBrowserRecv->setText(mSerial->readAll());
}
