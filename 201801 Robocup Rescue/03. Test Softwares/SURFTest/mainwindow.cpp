#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;

String file1 = "C:\\Users\\Minos\\Desktop\\QrTest5.png";
VideoCapture cap;
Mat img_scene;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    cap.open(0);
    connect(mTimer, SIGNAL(timeout()), this, SLOT(SURFCompare()));
    mTimer->start(0);

    ui->setupUi(this);  
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_buttonClose_clicked()
{
    this->close();
}

void MainWindow::SURFCompare()
{
    cap.read(img_scene);
    Mat img_object = imread(file1, IMREAD_COLOR);

    if(!img_object.data || !img_scene.data)
    {
        cout << "Error Reading Images!!" << endl;
        return;
    }

    double minHessian = 400.0;

    Ptr<SurfFeatureDetector> detector = SurfFeatureDetector::create(minHessian);

    vector<KeyPoint> keypoints_object, keypoints_scene;

    detector->detect(img_object, keypoints_object);
    detector->detect(img_scene, keypoints_scene);

    Ptr<SurfDescriptorExtractor> extractor = SurfDescriptorExtractor::create();

    Mat descriptors_object, descriptors_scene;

    extractor->compute(img_object, keypoints_object, descriptors_object);
    extractor->compute(img_scene, keypoints_scene, descriptors_scene);

    FlannBasedMatcher matcher;
    vector<DMatch> matches;
    matcher.match(descriptors_object, descriptors_scene, matches);

    double max_dist = 0, min_dist = 100;

    for(int i = 0; i < descriptors_object.rows; i++)
    {
        double dist = matches[i].distance;
        if(dist < min_dist)
            min_dist = dist;
        if(dist > max_dist)
            max_dist = dist;
    }

    cout << "-- Max Dist : " << max_dist << endl;
    cout << "-- Min Dist : " << min_dist << endl;

    vector<DMatch> good_matches;

    for(int i = 0; i < descriptors_object.rows; i++)
    {
        if(matches[i].distance < 3 * min_dist)
            good_matches.push_back(matches[i]);
    }

    Mat img_matches;

    drawMatches(img_object, keypoints_object, img_scene,
                keypoints_scene, good_matches, img_matches,
                Scalar::all(-1), Scalar::all(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

    vector<Point2f> obj;
    vector<Point2f> scn;

    for(int i = 0; i < good_matches.size(); i++)
    {
        obj.push_back(keypoints_object[good_matches[i].queryIdx].pt);
        scn.push_back(keypoints_scene[good_matches[i].trainIdx].pt);
    }

    Mat H = findHomography(obj, scn, RANSAC);

    vector<Point2f> obj_corners(4);
    obj_corners[0] = cvPoint(0, 0);
    obj_corners[1] = cvPoint(img_object.cols, 0);
    obj_corners[2] = cvPoint(img_object.cols, img_object.rows);
    obj_corners[3] = cvPoint(0, img_object.rows);

    vector<Point2f> scene_corners(4);

    perspectiveTransform(obj_corners, scene_corners, H);

    line(img_matches, scene_corners[0] + Point2f(img_object.cols, 0), scene_corners[0] + Point2f(img_object.cols, 0), Scalar(0, 255, 0), 4);
    line(img_matches, scene_corners[1] + Point2f(img_object.cols, 0), scene_corners[1] + Point2f(img_object.cols, 0), Scalar(0, 255, 0), 4);
    line(img_matches, scene_corners[2] + Point2f(img_object.cols, 0), scene_corners[2] + Point2f(img_object.cols, 0), Scalar(0, 255, 0), 4);
    line(img_matches, scene_corners[3] + Point2f(img_object.cols, 0), scene_corners[3] + Point2f(img_object.cols, 0), Scalar(0, 255, 0), 4);

    imshow("Good Matches & Object Detection", img_matches);

    waitKey(0);
}
