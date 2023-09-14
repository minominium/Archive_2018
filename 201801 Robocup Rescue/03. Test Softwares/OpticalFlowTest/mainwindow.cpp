#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace cv;
using namespace std;

Mat frame, flow;
Mat img, original;
VideoCapture cap(0);
UMat flowUmat, prevgray;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    connect(mTimer, SIGNAL(timeout()), this, SLOT(processFrameUpdateGUI()));
    mTimer->start(0);

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::processFrameUpdateGUI()
{
    if(!cap.grab())
    {
        cout << "Open Camera Failed" << endl;
        return;
    }
    else
    {
        cap.retrieve(img, CV_CAP_OPENNI_BGR_IMAGE);

        img.copyTo(original);

        cvtColor(img, img, COLOR_BGR2GRAY);

        if(!prevgray.empty())
        {
            calcOpticalFlowFarneback(prevgray, img, flowUmat, 0.4, 1, 12, 2, 8, 1.2, 0);

            flowUmat.copyTo(flow);

            for(int y = 0; y < original.rows; y += 5)
            {
                for(int x = 0; x < original.cols; x += 5)
                {
                    const Point2f flowatxy = flow.at<Point2f>(y, x) * 10;

                    line(original, Point(x, y), Point(cvRound(x + flowatxy.x), cvRound(y + flowatxy.y)), Scalar(255, 0, 0));
                    circle(original, Point(x, y), 1, Scalar(0, 0, 0), -1);
                }
            }

            namedWindow("Prew", WINDOW_AUTOSIZE);
            imshow("Prew", original);

            img.copyTo((prevgray));
        }
        else
        {
            img.copyTo(prevgray);
        }
    }
}
