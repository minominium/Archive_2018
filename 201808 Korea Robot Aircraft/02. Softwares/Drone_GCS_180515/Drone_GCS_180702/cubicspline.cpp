#include "cubicspline.h"
#include "iostream"

double waypointData_Lat[9] = {0,};
double waypointData_Lng[9] = {0,};

CubicSpline::CubicSpline(QObject *parent) :
    QObject(parent)
{

}

void CubicSpline::runSpline(Point sp, Point wp, Point dp, Point ep, Point nf)
{
    double h[10]={0},a=0,b=0,c=0,d=0,sum=0,p=0,temp=0,u[10]={0},x[10]={0},F[10]={0},f[10]={0},m[10][10]={{0}};
    std::vector<double> X(4), Y(4), N(3);

    X[0] = sp.lat;
    X[1] = wp.lat;

    Y[0] = sp.lng;
    Y[1] = wp.lng;

    N[0] = nf.lat;
    N[1] = nf.lng;
    N[2] = nf.rad;

    int dist_gain = 0;
    double r_lat = (N[2] + dist_gain)/88804;
    double slope[10] = {0,};

    //************  Up - SP>WP  ************//
    if(X[0] == X[1])    slope[0] = 0;
    else                slope[0] = (Y[1]-Y[0])/(X[1]-X[0]);

    dist_gain = 0;                                                      //distance error from circle to dot
    r_lat = (N[2] + dist_gain)/88804;
    double root_upCenter_a = (1/(slope[0]*slope[0]))+1;
    double root_upCenter_b = (-1)*2.0 *N[0] * root_upCenter_a;
    double root_upCenter_c = (N[0]*N[0]) + ((N[0]*N[0])/(slope[0]*slope[0])) - pow(r_lat,2);

    double point_upCenter_x = root_formula(root_upCenter_a, root_upCenter_b, root_upCenter_c, 0);
    double point_upCenter_y = (-1/slope[0])*(point_upCenter_x-N[0])+N[1];

    //************  Up Right - SP(0)>point_upCenter_x(2)  ************//
    X[2] = point_upCenter_x;
    Y[2] = point_upCenter_y;

    if(X[0] == X[2])    slope[1] = 0;
    else                slope[1] = (Y[2]-Y[0])/(X[2]-X[0]);

    dist_gain = 0;                                                      //distance error from circle to dot
    r_lat = (N[2] + dist_gain)/88804;
    double root_upRight_a = (1/(slope[1]*slope[1]))+1;
    double root_upRight_b = (-1)*2.0 *N[0] * root_upRight_a;
    double root_upRight_c = (N[0]*N[0]) + ((N[0]*N[0])/(slope[1]*slope[1])) - pow(r_lat,2);

    double point_upRight_x = root_formula(root_upRight_a, root_upRight_b, root_upRight_c, 0);
    double point_upRight_y = (-1/slope[1])*(point_upRight_x-N[0])+N[1];

    //************  Up Left - point_upCenter_x(2)>WP(1)  ************//
    if(X[1] == X[2])    slope[2] = 0;
    else                slope[2] = (Y[2]-Y[1])/(X[2]-X[1]);

    dist_gain = 3;                                                      //distance error from circle to dot
    r_lat = (N[2] + dist_gain)/88804;
    double root_upLeft_a = (1/(slope[2]*slope[2]))+1;
    double root_upLeft_b = (-1)*2.0 *N[0] * root_upLeft_a;
    double root_upLeft_c = (N[0]*N[0]) + ((N[0]*N[0])/(slope[2]*slope[2])) - pow(r_lat,2);

    double point_upLeft_x = root_formula(root_upLeft_a, root_upLeft_b, root_upLeft_c, 0);
    double point_upLeft_y = (-1/slope[2])*(point_upLeft_x-N[0])+N[1];

    //************  Down - WP(1)>DP(3)  ************//
    X[3] = dp.lat;
    Y[3] = dp.lng;

    if(X[1] == X[3])    slope[3] = 0;
    else                slope[3] = (Y[3]-Y[1])/(X[3]-X[1]);

    dist_gain = 0;                                                      //distance error from circle to dot
    r_lat = (N[2] + dist_gain)/88804;
    double point_downCenter_x = X[1] - r_lat;
    double point_downCenter_y = (Y[3] + Y[1]) * 0.5;

    //************  Down Left - WP(1)>point_downCenter_x(4)  ************//
    X[4] = point_downCenter_x;
    Y[4] = point_downCenter_y;

    if(X[1] == X[4])    slope[4] = 0;
    else                slope[4] = (Y[4]-Y[1])/(X[4]-X[1]);

    dist_gain = 3;                                                      //distance error from circle to dot
    r_lat = (N[2] + dist_gain)/88804;
    double root_downLeft_a = (1/(slope[4]*slope[4]))+1;
    double root_downLeft_b = (-1)*2.0 *N[0] * root_downLeft_a;
    double root_downLeft_c = (N[0]*N[0]) + ((N[0]*N[0])/(slope[4]*slope[4])) - pow(r_lat,2);

    double point_downLeft_x = root_formula(root_downLeft_a, root_downLeft_b, root_downLeft_c, 1);
    double point_downLeft_y = (-1/slope[4])*(point_downLeft_x-N[0])+N[1];

    //************  Down Right - point_downCenter_x(4)>DP(3)  ************//
    if(X[3] == X[4])    slope[5] = 0;
    else                slope[5] = (Y[4]-Y[3])/(X[4]-X[3]);

    dist_gain = 3;                                                      //distance error from circle to dot
    r_lat = (N[2] + dist_gain)/88804;
    double root_downRight_a = (1/(slope[5]*slope[5]))+1;
    double root_downRight_b = (-1)*2.0 *N[0] * root_downRight_a;
    double root_downRight_c = (N[0]*N[0]) + ((N[0]*N[0])/(slope[5]*slope[5])) - pow(r_lat,2);

    double point_downRight_x = root_formula(root_downRight_a, root_downRight_b, root_downRight_c, 1);
    double point_downRight_y = (-1/slope[5])*(point_downRight_x-N[0])+N[1];

    //************  Display Function  ************//
    sigDrawSpline(QString("addMarker(%1, %2)"). arg(QString::number(sp.lat,'f',6)). arg(QString::number(sp.lng,'f',6)));
    waypointData_Lat[0] = sp.lat;
    waypointData_Lng[0] = sp.lng;
    sigDrawSpline(QString("addMarker(%1, %2)"). arg(QString::number(point_upRight_x,'f',6)). arg(QString::number(point_upRight_y,'f',6)));
    waypointData_Lat[1] = point_upRight_x;
    waypointData_Lng[1] = point_upRight_y;
    sigDrawSpline(QString("addMarker(%1, %2)"). arg(QString::number(point_upCenter_x,'f',6)). arg(QString::number(point_upCenter_y,'f',6)));
    waypointData_Lat[2] = point_upCenter_x;
    waypointData_Lng[2] = point_upCenter_y;
    sigDrawSpline(QString("addMarker(%1, %2)"). arg(QString::number(point_upLeft_x,'f',6)). arg(QString::number(point_upLeft_y,'f',6)));
    waypointData_Lat[3] = point_upLeft_x;
    waypointData_Lng[3] = point_upLeft_y;
    sigDrawSpline(QString("addMarker(%1, %2)"). arg(QString::number(wp.lat,'f',6)). arg(QString::number(wp.lng,'f',6)));
    waypointData_Lat[4] = wp.lat;
    waypointData_Lng[4] = wp.lng;

    sigDrawSpline(QString("addMarker(%1, %2)"). arg(QString::number(point_downLeft_x,'f',6)). arg(QString::number(point_downLeft_y,'f',6)));
    waypointData_Lat[5] = point_downLeft_x;
    waypointData_Lng[5] = point_downLeft_y;
    sigDrawSpline(QString("addMarker(%1, %2)"). arg(QString::number(point_downCenter_x,'f',6)). arg(QString::number(point_downCenter_y,'f',6)));
    waypointData_Lat[6] = point_downCenter_x;
    waypointData_Lng[6] = point_downCenter_y;
    sigDrawSpline(QString("addMarker(%1, %2)"). arg(QString::number(point_downRight_x,'f',6)). arg(QString::number(point_downRight_y,'f',6)));
    waypointData_Lat[7] = point_downRight_x;
    waypointData_Lng[7] = point_downRight_y;

    sigDrawSpline(QString("addMarker(%1, %2)"). arg(QString::number(dp.lat,'f',6)). arg(QString::number(dp.lng,'f',6)));
    waypointData_Lat[8] = dp.lat;
    waypointData_Lng[8] = dp.lng;
    sigDrawSpline(QString("addMarker(%1, %2)"). arg(QString::number(sp.lat,'f',6)). arg(QString::number(sp.lng,'f',6)));
}

double CubicSpline::root_formula(double a, double b, double c, bool dir)
{
    double temp = (b*b) - (4*a*c);
    if(temp > 0){
        if(dir)     return (-b - sqrt(pow(b,2)-4*a*c)) / (2*a);
        else        return (-b + sqrt(pow(b,2)-4*a*c)) / (2*a);
    }
    return 1;
}


//#include "cubicspline.h"
//#include "iostream"

//double waypointData_Lat[21] = {0,};
//double waypointData_Lng[21] = {0,};

//CubicSpline::CubicSpline(QObject *parent) :
//    QObject(parent)
//{

//}

//void CubicSpline::runSpline(Point sp, Point wp, Point dp, Point ep, Point nf)
//{
//    double h[10]={0},a=0,b=0,c=0,d=0,sum=0,p=0,temp=0,u[10]={0},x[10]={0},F[10]={0},f[10]={0},m[10][10]={{0}};
//    std::vector<double> X(4), Y(4), N(3);

//    X[0] = sp.lat;
//    X[1] = wp.lat;
//    X[2] = dp.lat;
////    X[3] = m_ep_lat;

//    Y[0] = sp.lng;
//    Y[1] = wp.lng;
//    Y[2] = dp.lng;
////    Y[3] = m_ep_lng;

//    N[0] = nf.lat;
//    N[1] = nf.lng;
//    N[2] = nf.rad;

//    double r_lat = (N[2]+5)/88804;


//    double slope[10] = {0,};
//    slope[0] = (Y[1]-Y[0])/(X[1]-X[0]);
//    slope[1] = (Y[1]-Y[2])/(X[1]-X[2]);
//    if(slope[1]>0) slope[1] = slope[1] * (-1);

//    double root_a = (1/(slope[0]*slope[0]))+1;
//    double root_b = (-1)*2.0 *N[0] * root_a;
//    double root_c = (N[0]*N[0]) + ((N[0]*N[0])/(slope[0]*slope[0])) - pow(r_lat,2);

//    double root_a2 = (1/(slope[1]*slope[1]))+1;
//    double root_b2 = (-1)*2.0 *N[0] * root_a2;
//    double root_c2 = (N[0]*N[0]) + ((N[0]*N[0])/(slope[1]*slope[1])) - pow(r_lat,2);


//    double point_x = root_formula(root_a, root_b, root_c, 1);
//    double point_y = (-1/slope[0])*(point_x-N[0])+N[1];

//    double point_x2 = root_formula(root_a2, root_b2, root_c2, 0);
//    double point_y2 = (-1/slope[1])*(point_x2-N[0])+N[1];

//    x[0] = wp.lat;
//    x[1] = point_x;
//    x[2] = sp.lat;

//    f[0] = wp.lng;
//    f[1] = point_y;
//    f[2] = sp.lng;

//    for(int i = i_point_cnt-1; i > 0; i--){
//        F[i] = (f[i] - f[i-1]) / (x[i] - x[i-1]);
//        h[i-1] = x[i] - x[i-1];
//    }
//    //*********** formation of h, s , f matrix **************//
//    for(int i = 1; i < i_point_cnt-1; i++){
//        m[i][i] = 2 * (h[i-1] + h[i]);
//        if(i != 1){
//            m[i][i-1] = h[i-1];
//            m[i-1][i] = h[i-1];
//        }
//        m[i][i_point_cnt-1] = 6 * (F[i+1] - F[i]);
//    }
//    //***********  forward elimination **************//
//    for(int i = 1; i < i_point_cnt-2; i++){
//        temp = (m[i+1][i] / m[i][i]);
//        for(int j = 1; j <= i_point_cnt-1; j++)     m[i+1][j] -= temp * m[i][j];
//    }
//    //*********** back ward substitution *********//
//    for(int i = i_point_cnt-2; i > 0; i--){
//        sum = 0;
//        for(int j = i; j <= i_point_cnt-2; j++)     sum += m[i][j] * u[j];
//        u[i] = (m[i][i_point_cnt-1] - sum) / m[i][i];
//    }

//    /*//START Point///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/


//    sigDrawSpline(QString("addMarker(%1, %2)"). arg(QString::number(wp.lat,'f',6)). arg(QString::number(wp.lng,'f',6)));

//    /*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
//    /*      Waypoint WP to PxPy     */
//    /*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
//        for(int j = 4; j > 0; j--){
//            for(int i=0;i<1;i++){
//                p = x[i+1] + ((x[i]-x[i+1]) * 0.2 * j);
//                a=(u[i+1]-u[i])/(6*h[i]);
//                b=u[i]/2;
//                c=(f[i+1]-f[i])/h[i]-(2*h[i]*u[i]+u[i+1]*h[i])/6;
//                d=f[i];
//                sum=a*pow((p-x[i]),3)+b*pow((p-x[i]),2)+c*(p-x[i])+d;
////                sigDrawSpline(QString("addMarker(%1, %2)"). arg(QString::number(p,'f',6)). arg(QString::number(sum,'f',6)));
//                std::cout<<"abc = "<<a<<","<<b<<","<<c<<","<<d<<std::endl;
//                std::cout<<"x[i], x[i+1] = "<<x[1]<<", "<<x[2]<<", |x[0]-p| = "<<x[1]-p<<std::endl;
//                std::cout<<"(p,sum) = "<<p<<", "<<sum<<std::endl;

//                waypointData_Lat[j + 5] = p;
//                waypointData_Lng[j + 5] = sum;
//            }
//        }
//    /*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
////       sigDrawSpline(QString("addMarker(%1, %2)"). arg(QString::number(point_x ,'f',6)). arg(QString::number(point_y ,'f',6)));
//       waypointData_Lat[5] = point_x;
//       waypointData_Lng[5] = point_y;
//        std::cout<<"/////////////////////////////////////////"<<std::endl;

//    /*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
//    /*      Waypoint PxPy to SP     */
//    /*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/

//        for(int j = 4; j > 0; j--){
//            for(int i=1;i<2;i++){
//                p = x[i+1] + ((x[i]-x[i+1]) * 0.2 * j);
//                a=(u[i+1]-u[i])/(3*h[i]);
//                b=u[i]/2;
//                c=(f[i+1]-f[i])/h[i]-(1.2*h[i]*u[i]+u[i+1]*h[i])/6;
//                d=f[i];
//                sum=a*pow((p-x[i]),3)+b*pow((p-x[i]),2)+c*(p-x[i])+d;
// //               sigDrawSpline(QString("addMarker(%1, %2)"). arg(QString::number(p,'f',6)). arg(QString::number(sum,'f',6)));
//                std::cout<<"abc = "<<a<<","<<b<<","<<c<<","<<d<<std::endl;
//                std::cout<<"x[i], x[i+1] = "<<x[1]<<", "<<x[2]<<", |x[0]-p| = "<<x[1]-p<<std::endl;
//                std::cout<<"(p,sum) = "<<p<<", "<<sum<<std::endl;

//                waypointData_Lat[j] = p;
//                waypointData_Lng[j] = sum;
//            }
//        }

//    /*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
//        sigDrawSpline(QString("addMarker(%1, %2)"). arg(QString::number(sp.lat,'f',6)). arg(QString::number(sp.lng,'f',6)));
//        std::cout<<"/////////////////////////////////////////"<<std::endl;
//    /*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/


//        x[0] = wp.lat;
//        x[1] = point_x2;
//        x[2] = dp.lat;

//        f[0] = wp.lng;
//        f[1] = point_y2;
//        f[2] = dp.lng;

////        slope[2] = (x[1] - x[2])/(f[1] - f[2]);
////        slope[3] = (x[0] - x[1])/(f[0] - f[1]);
////        if(slope[2]>slope[3])
////        {
////            std::cout<<"********************"<<std::endl;
////            std::cout<<"********************"<<std::endl;
////            std::cout<<"********************"<<std::endl;
////            f[1] = f[2];
////            f[2] = {0};
////            i_point_cnt = 2;
////        }
////        else i_point_cnt = 3;

//        for(int i = i_point_cnt-1; i > 0; i--){
//            F[i] = (f[i-1] - f[i]) / (x[i-1]-x[i]);
//            h[i-1] = x[i-1]-x[i];
//        }
//        //*********** formation of h, s , f matrix **************//
//        for(int i = 1; i < i_point_cnt-1; i++){
//            m[i][i] = 2 * (h[i-1] + h[i]);
//            if(i != 1){
//                m[i][i-1] = h[i-1];
//                m[i-1][i] = h[i-1];
//            }
//            m[i][i_point_cnt-1] = 6 * (F[i+1] - F[i]);
//        }
//        //***********  forward elimination **************//
//        for(int i = 1; i < i_point_cnt-2; i++){
//            temp = (m[i+1][i] / m[i][i]);
//            for(int j = 1; j <= i_point_cnt-1; j++)     m[i+1][j] -= temp * m[i][j];
//        }

//        //*********** back ward substitution *********//
//        for(int i = i_point_cnt-2; i > 0; i--){
//            sum = 0;
//            for(int j = i; j <= i_point_cnt-2; j++)     sum += m[i][j] * u[j];
//            u[i] = (m[i][i_point_cnt-1] - sum) / m[i][i];
//        }



//    /*//START Point///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
//        sigDrawSpline(QString("addMarker(%1, %2)"). arg(QString::number(sp.lat,'f',6)). arg(QString::number(sp.lng,'f',6)));
//    /*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/

//    /*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
//        sigDrawSpline(QString("addMarker(%1, %2)"). arg(QString::number(dp.lat ,'f',6)). arg(QString::number(dp.lng ,'f',6)));
//        std::cout<<"/////////////////////////////////////////"<<std::endl;
//    /*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
//    /*      Waypoint DP to Px2Py2     */
//    /*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/

//        for(int j = 1; j < 5; j++){
//            for(int i=1;i<2;i++){
//                p = x[i+1] + ((x[i]-x[i+1]) * 0.2 * j);
//                a=(u[i+1]-u[i])/(6*h[i]);
//                b=u[i]/2;
//                c=(f[i+1]-f[i])/h[i]-(2*h[i]*u[i]+u[i+1]*h[i])/6;
//                d=f[i];
//                if(slope[1]<=0)
//                    sum=-a*pow((p-x[i]),3)+b*pow((p-x[i]),2)-c*(p-x[i])+d;
//                else
//                    sum=-a*pow((p-x[i]),3)+b*pow((p-x[i]),2)+c*(p-x[i])+d;
////                sigDrawSpline(QString("addMarker(%1, %2)"). arg(QString::number(p,'f',6)). arg(QString::number(sum,'f',6)));
//                std::cout<<"abc = "<<a<<","<<b<<","<<c<<","<<d<<std::endl;
//                std::cout<<"x[i], x[i+1] = "<<x[1]<<", "<<x[2]<<", |x[0]-p| = "<<x[1]-p<<std::endl;
//                std::cout<<"(p,sum) = "<<p<<", "<<sum<<std::endl;

//                waypointData_Lat[20 - j] = p;
//                waypointData_Lng[20 - j] = sum;
//            }
//        }

//    /*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
////        sigDrawSpline(QString("addMarker(%1, %2)"). arg(QString::number(point_x2,'f',6)). arg(QString::number(point_y2,'f',6)));
//        std::cout<<"/////////////////////////////////////////"<<std::endl;
//        waypointData_Lat[15] = point_x2;
//        waypointData_Lng[15] = point_y2;
//    /*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
//    /*      Waypoint Px2Py2 to WP     */
//    /*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/

//        for(int j = 1; j < 5; j++){
//            for(int i=0;i<1;i++){
//                p = x[i+1] + ((x[i]-x[i+1]) * 0.2 * j);
//                a=(u[i+1]-u[i])/(6*h[i]);
//                b=u[i]/2;
//                c=(f[i+1]-f[i])/h[i]-(2*h[i]*u[i]+u[i+1]*h[i])/6;
//                d=f[i];
//                if(slope[1]<0)
//                    sum=a*pow((x[i]-p),3)+b*pow((x[i]-p),2)+c*(x[i]-p)+d;
//                else
//                    sum=-a*pow((x[i]-p),3)+b*pow((x[i]-p),2)+c*(x[i]-p)+d;
// //               sigDrawSpline(QString("addMarker(%1, %2)"). arg(QString::number(p,'f',6)). arg(QString::number(sum,'f',6)));
//                std::cout<<"abc = "<<a<<","<<b<<","<<c<<","<<d<<std::endl;
//                std::cout<<"x[i], x[i+1] = "<<x[1]<<", "<<x[2]<<", |x[0]-p| = "<<x[1]-p<<std::endl;
//                std::cout<<"(p,sum) = "<<p<<", "<<sum<<std::endl;

//                waypointData_Lat[15 - j] = p;
//                waypointData_Lng[15 - j] = sum;
//            }
//        }

//    /*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
//        sigDrawSpline(QString("addMarker(%1, %2)"). arg(QString::number(wp.lat,'f',6)). arg(QString::number(wp.lng,'f',6)));
//        std::cout<<"/////////////////////////////////////////"<<std::endl;
//    /*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
//}

//double CubicSpline::root_formula(double a, double b, double c, bool dir)
//{
//    double temp = (b*b) - (4*a*c);
//    if(temp > 0){
//        if(dir)     return (-b - sqrt(pow(b,2)-4*a*c)) / (2*a);
//        else        return (-b + sqrt(pow(b,2)-4*a*c)) / (2*a);
//    }
//    return 1;
//}





