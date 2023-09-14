#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <qserialport.h>
#include <qserialportinfo.h>
#include <stdio.h>
#include <chrono>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QSerialPort *mSerial;

    QByteArray sendData = "asdf";

private slots:
    void on_pushButtonRescan_clicked();
    void on_pushButtonConnect_clicked();
    void on_pushButtonSend_clicked();
    void serial_send();
    void serial_received();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
