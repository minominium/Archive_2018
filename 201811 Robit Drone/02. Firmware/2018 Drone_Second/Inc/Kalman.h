#ifndef _Kalman_h
#define _Kalman_h

typedef struct
{
    double angle, bias, rate;
    double Q_Angle, Q_Gyro, R_Measure;
    double P[2][2], K[2];
} Kalman;

#endif