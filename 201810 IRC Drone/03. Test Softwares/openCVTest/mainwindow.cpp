#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    cap.open(0);
    if(!cap.isOpened())
    {
        std::cout << "Cannot open the camera" << std::endl;
        return;
    }

    mTimer = new QTimer(this);
    connect(mTimer, SIGNAL(timeout()), this, SLOT(timerUpdate()));
    mTimer->start(100);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{

}

void MainWindow::timerUpdate()
{
    cv::namedWindow("Control", CV_WINDOW_AUTOSIZE);

    int iLowerH = 0;
    int iUpperH = 179;

    int iLowerS = 0;
    int iUpperS = 255;

    int iLowerV = 0;
    int iUpperV = 255;

    cvCreateTrackbar("LowerH", "Control", &iLowerH, 179);
    cvCreateTrackbar("UpperH", "Control", &iUpperH, 179);

    cvCreateTrackbar("LowerS", "Control", &iLowerS, 255);
    cvCreateTrackbar("UpperS", "Control", &iUpperS, 255);

    cvCreateTrackbar("LowerV", "Control", &iLowerV, 255);
    cvCreateTrackbar("UpperV", "Control", &iUpperV, 255);

    cap.read(image);

    cv::cvtColor(image, image_HSV, CV_BGR2HSV);
    cv::imshow("HSV Image", image_HSV);

    cv::Mat image_Thresholded;
    cv::inRange(image_HSV, cv::Scalar(iLowerH, iLowerS, iLowerV), cv::Scalar(iUpperH, iUpperS, iUpperV), image_Thresholded);

    cv::erode(image_Thresholded, image_Thresholded, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
    cv::dilate(image_Thresholded, image_Thresholded, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));

    cv::dilate(image_Thresholded, image_Thresholded, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
    cv::erode(image_Thresholded, image_Thresholded, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));

    cv::imshow("Thresholded Image", image_Thresholded);
    cv::imshow("Original", image);
}
