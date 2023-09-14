#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "serial.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
signals:
    void isSerialOpened(QString, int);
    void isSerialClosed();


public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_connect_clicked();

private:
    Ui::MainWindow *ui;

    bool isConnected = false;



    void uiInit();
    void addPort();
    void addBaudrate();
    Serial serial;
};

#endif // MAINWINDOW_H
