#ifndef SERIAL_H
#define SERIAL_H
#include <iostream>
#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>

class Serial : public QObject
{
    Q_OBJECT

private:
    QSerialPort *m_serial;
    bool connectCheck = false;
    int connectCount = 0;
public:
    explicit Serial(QObject *parent = 0);
    ~Serial();

    void setBaudrate(int baudrate);
    QList<QSerialPortInfo> getPort();
    void init();

private slots:
    void serialOpen(QString portName, int baudrate);

    void serialClose();

    void readData();

    void print();
};

#endif // SERIAL_H
