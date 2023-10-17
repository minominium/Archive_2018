#ifndef ROBITVISION_H
#define ROBITVISION_H

#include <opencv2/opencv.hpp>

#include <fstream>
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

const double  _PI= 3.1415926535897932385;
const double _2PI= 6.283185307179586477;

const unsigned char ErrorVal=255;

typedef struct ColorInfoHsv{
     bool isChecked;

     int hmin;
     int smin;
     int vmin;

     int hmax;
     int smax;
     int vmax;

     ColorInfoHsv()
     {
          hmin=0;
          smin=0;
          vmin=0;

          hmax=0;
          smax=0;
          vmax=0;
     }
}ColorInfoHsv;


typedef struct RobocupInfo{
     int inital_goalpostHegiht;
     int inital_goalpostDist;
     int inital_ballArea;
     int MapX;
     int MapY;
}RobocupInfo;

/*////////////////////////////////////////////////////////////////////////////////////////////////////////*/

namespace RV
{
     void rangeFilter(const Mat& input,const Scalar& min,const Scalar& max,Mat& output);
     void rgb2hsv(const Mat& data_rgb, Mat &data_hsv);
     void DrawLine(Mat& img, const Point& PtA, const Point& PtB , const Scalar &color,const int LineThresHold);
}

class RobitKalmanFilter {

private:
         double Q ;
         double R ;
         double X, P, K;

         void measurementUpdate();

public:
        RobitKalmanFilter(double initValue) {
            X = initValue;
            Q = 0.0008;
            R = 0.001;
            X = 0.0;
            P = 1.0;
            K = 0.0;
        }
        ~RobitKalmanFilter()
        {
        }
         const double update(double measurement);
};

class RobitRansacLine {


protected:

    struct sLine{
        double mx;
        double my;
        double sx;
        double sy;
    };

    unsigned char* m_ImgData;
    int m_width;
    int m_height;

    double m_degree;

    Point m_line_A,m_line_B;
    vector<Point> m_pos;

    bool m_noSamples;

    void getSamples();
    void compute_model_parameter(const vector<Point>& vInlierPoint, sLine &model);
    double compute_distance(sLine &line, Point &x)const;

public:
    RobitRansacLine(const Mat& img);

    void runRansac();
    double getDegree()const{return m_degree;}

    Point getPointLineA()const{return m_line_A;}
    Point getPointLineB()const{return m_line_B;}
};

class RobitDetectGoalPost :RobitRansacLine
{
private:
    bool m_isGoal;
    unsigned int  m_minLength;
    vector< Point > m_A;
    vector< Point > m_B;

public:
    RobitDetectGoalPost(const Mat& img);
    ~RobitDetectGoalPost(){}

    void getGoalPost();
    void setMinLength(unsigned int uLength){m_minLength = uLength;}
    bool isGoal()const{return m_isGoal;}
};

class RobitRansacCircle {

private:
      unsigned char* m_ImgData;
      int m_width;
      int m_height;

      Point m_center;
      vector<Point> m_pos;

      double m_minRadius;
      double m_maxRadius;

      double m_cost;
      double m_radius;

      bool m_noSamples;

      void getSamples();
      bool getCircle(Point& p1,Point& p2,Point& p3, Point& center, double& radius)const;
      bool getLeastSquareCircle(vector<Point> PtVec, Point &center, double &radius) const;

      double verifyCircle(const Point& center, double radius)const;
public:

        RobitRansacCircle(const Mat& img);
        void runRansac();

        void setMinRadius(const double dist){m_minRadius=dist;}
        void setMaxRadius(const double dist){m_maxRadius=dist;}

        Point getPointCenter()const{return m_center;}
        double getCost()const{return m_cost;}
        double getRadius()const{return m_radius;}
};

class RobitImageRotate {

private:
    Mat m_img_rotate;
    unsigned char* m_ImgData;

    int m_width;
    int m_height;

    void interPolation();
public:
    RobitImageRotate(const Mat& img);
    void rotate(const double theta);
    Mat& getRTimg(){return m_img_rotate;}
};



#endif // ROBITVISION_H
