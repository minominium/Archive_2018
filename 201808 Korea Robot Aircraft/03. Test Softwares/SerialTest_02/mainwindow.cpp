#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "iostream"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mSerial = new QSerialPort(this);
    connect(ui->pushButtonRescan, SIGNAL(clicked()), this, SLOT(on_pushButtonRescan_clicked()));
    connect(ui->pushButtonConnect, SIGNAL(clicked()), this, SLOT(on_pushButtonConnect_clicked()));
    connect(ui->pushButtonDisconnect, SIGNAL(clicked()), this, SLOT(on_pushButtonDisconnect_clicked()));
    connect(mSerial, SIGNAL(readyRead()), this, SLOT(serial_received()));
    on_pushButtonRescan_clicked();
    ui->pushButtonDisconnect->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::widget_changed()
{

}

void MainWindow::send_test()
{
    mSerial->write("It's test line");
}

void MainWindow::serial_received()
{
    QByteArray data_Recv;
    data_Recv = mSerial->readAll();

    QList<QByteArray> RX_Buffer = data_Recv.split(',');

    QByteArray realData[5];


    if(RX_Buffer[0].toInt() == 255 && RX_Buffer[1].toInt() == 170 && RX_Buffer.size() > 6)
    {
        ui->textBrowserRecv->clear();
        for(int i = 0 ; i < 5 ; i ++)
        {
            realData[i] = RX_Buffer[i+2];
            ui->textBrowserRecv->append(realData[i]);
        }
    }
    data_Recv.clear();
    RX_Buffer.clear();
}

void MainWindow::on_pushButtonRescan_clicked()
{
    ui->comboBoxPort->clear();
    foreach (const QSerialPortInfo& mSerialPortInfo, QSerialPortInfo::availablePorts())
    {
        ui->comboBoxPort->addItem(mSerialPortInfo.portName());
    }
}

void MainWindow::on_pushButtonConnect_clicked()
{
    if(!mSerial->isOpen())
    {
        mSerial->setPortName(ui->comboBoxPort->currentText());
        mSerial->setBaudRate(QSerialPort::Baud57600);
        mSerial->setDataBits(QSerialPort::Data8);
        mSerial->setParity(QSerialPort::NoParity);
        mSerial->setStopBits(QSerialPort::OneStop);
        mSerial->setFlowControl(QSerialPort::NoFlowControl);

        mSerial->open(QIODevice::ReadWrite);
        send_test();

        ui->pushButtonConnect->setEnabled(false);
        ui->pushButtonDisconnect->setEnabled(true);
    }
}

void MainWindow::on_pushButtonDisconnect_clicked()
{
    if(mSerial->isOpen())
    {
        mSerial->close();

        ui->pushButtonConnect->setEnabled(true);
        ui->pushButtonDisconnect->setEnabled(false);
    }
}

void MainWindow::on_pushButtonSend_clicked()
{
    QByteArray data_Send;
    data_Send = ui->lineEditSend->text().toUtf8();
    mSerial->write(data_Send.data());
}


