#include "mainwindow.h"
#include "ui_mainwindow.h"

// QZXing Header file include
#include "QZXing/src/QZXing.h"

QString result = "";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QImage imageToDecode("C:\\Users\\Minos\\Desktop\\QrTest4.png");                              // 회전 여부는 상관 없는듯. 90, 180, 270도에서 실험 완료
//    ui->label_2->setPixmap(QPixmap::fromImage(imageToDecode));
    QZXing decoder;                                                                             // 주변 물체도 필터링 하는듯
    decoder.setDecoder( QZXing::DecoderFormat_QR_CODE | QZXing::DecoderFormat_EAN_13 );
    result = decoder.decodeImage(imageToDecode);
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{

    ui->label->setText(result);
}
