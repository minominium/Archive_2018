#ifndef CUBICSPLINE_H
#define CUBICSPLINE_H

#include <QObject>
#include <math.h>

//#define POINT_CNT  3

extern double waypointData_Lat[9];
extern double waypointData_Lng[9];

class CubicSpline : public QObject
{
    Q_OBJECT
public:
    explicit CubicSpline(QObject *parent = 0);

    struct Point{
        double lat;
        double lng;
        double rad;
        double slope;
    };
    int i_point_cnt = 3;

private:
    double root_formula(double a, double b, double c, bool dir);

signals:
    void sigDrawSpline(QString GPS_data);

public slots:
    void runSpline(Point sp, Point wp, Point dp, Point ep, Point nf);
};

#endif // CUBICSPLINE_H
