#include "serial.h"

Serial::Serial(QObject *parent) :
    QObject(parent)
{
    m_serial = new QSerialPort(this);

    init();
    connect(m_serial, SIGNAL(readyRead()), this, SLOT(readData()));

    QTimer *qtimer = new QTimer(this);
    connect(qtimer, SIGNAL(timeout()), this, SLOT(print()));
    qtimer->start(100);
}

QList<QSerialPortInfo> Serial::getPort()
{
    QList<QSerialPortInfo> m_ports = QSerialPortInfo::availablePorts();

    return m_ports;
}

void Serial::init()
{
    m_serial->setDataBits(QSerialPort::Data8);
    m_serial->setParity(QSerialPort::NoParity);
    m_serial->setStopBits(QSerialPort::OneStop);
    m_serial->setFlowControl(QSerialPort::NoFlowControl);
}


void Serial::serialOpen(QString portName, int baudrate)
{
    m_serial->setPortName(portName);
    m_serial->setBaudRate(baudrate);
    m_serial->open(QIODevice::ReadWrite);
    std::cout << "open" << std::endl;
    std::cout << m_serial->portName().toStdString() << std::endl;
    std::cout << m_serial->baudRate() << std::endl;
}

void Serial::serialClose()
{
    m_serial->close();
    std::cout << "close" << std::endl;
}

void Serial::readData()
{
    QByteArray buffer;
    buffer.resize(m_serial->bytesAvailable());

}

void Serial::print()
{
    std::cout << "isOpen = " << m_serial->isOpen() << std::endl;

}

Serial::~Serial()
{

}
