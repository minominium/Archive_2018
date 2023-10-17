#include "../include/irc_drone_vision/robitvision.hpp"

inline double SQR(double num)
{
    return (num*num);
}

void RV::rangeFilter(const Mat& input,const Scalar& min,const Scalar& max,Mat& output)
{
    register const unsigned int IMGSIZE=input.size().area();
    if(output.empty()==true)output=Mat::zeros(input.size(),CV_8UC1);

    register const unsigned char A=min[0],B=min[1],C=min[2]
                                ,D=max[0],E=max[1],F=max[2];
    register const unsigned char *data=input.data;
    register unsigned char *outputdata=output.data;
    register unsigned int index=0;

    for(register unsigned int i=0; i<IMGSIZE; i++)
    {
        index=i*3;
        if((data[index]>=A)&&(data[index]<=D))
            if((data[++index]>=B)&&(data[index]<=E))
                if((data[++index]>=C)&&(data[index]<=F))
                {
                    outputdata[i]=255;
                    continue;
                }
        outputdata[i]=0;
    }
}

void RV::rgb2hsv(const Mat& data_rgb,Mat& data_hsv)
{
    if(data_hsv.empty()==true)data_hsv=Mat::zeros(data_rgb.size(),CV_8UC3);

    unsigned char* data=       (unsigned char*)data_rgb.data;
    unsigned char* outputdata= (unsigned char*)data_hsv.data;

    int IMGSIZE=data_rgb.size().area(),
        h=0,s=0,v=0,
         bgrbg[5]={0,}   ,
         theta[3]={120,60,0}   ,
            delta=0;
    do
    {
        bgrbg[0]=*data++;
        bgrbg[1]=*data++;
        bgrbg[2]=*data++;
        bgrbg[3]=bgrbg[0];
        bgrbg[4]=bgrbg[1];

        v=(bgrbg[0]>bgrbg[1])?0:1;
        v=(bgrbg[v]>bgrbg[2])?v:2;
        delta= bgrbg[v]-((bgrbg[v+1]<bgrbg[v+2])?bgrbg[v+1]:bgrbg[v+2]);

        h=s=0;
        if(delta)
        {
            h=(((bgrbg[v+2]-bgrbg[v+1])*30.0)/delta)+theta[v];
            h += (-180)*(h>>31);
            s =((delta*255)/bgrbg[v]);
        }
        *outputdata++= h;
        *outputdata++= s;
        *outputdata++= bgrbg[v];
    }
    while(--IMGSIZE);
}

void RV::DrawLine(Mat& img, const Point& PtA, const Point& PtB , const Scalar &color ,const int LineThresHold)
{
    unsigned char ucColor=color[0];
    unsigned char* imgData=img.data;

    const int dX = PtB.x-PtA.x,
              nX = abs(dX),
              dY = PtB.y-PtA.y,
              nY = abs(dY);

    const int sign_x = dX > 0? 1 : -1,
              sign_y = dY > 0? 1 : -1;


    int x=PtA.x,
        y=PtA.y;

    vector <int> idxXY;

    for (int ix = 0, iy = 0; (ix < nX || iy < nY);)
    {
        if ((0.5+ix) / nX == (0.5+iy) / nY)
        {
            // next step is diagonal
            x += sign_x;
            y += sign_y;
            ix++;
            iy++;
        }
        else if ((0.5+ix) / nX < (0.5+iy) / nY)
        {
            // next step is horizontal
            x += sign_x;
            ix++;
        }
        else
        {
            // next step is vertical
            y += sign_y;
            iy++;
        }

        if(y>0&&y<img.rows)
        if(x>0&&x<img.cols)
        if(imgData[y*img.cols+x]!=255)
        {
            idxXY.push_back(y*img.cols+x);
        }
    }

    if(idxXY.size()<LineThresHold)
    {
        for(size_t i=0;i<idxXY.size();i++)
        {
            imgData[idxXY[i]]=ucColor;
        }
    }
}


void RobitKalmanFilter::measurementUpdate(){
    K = (P + Q) / (P + Q + R);
    P = ((R * (P + Q)) / (R + P + Q));
}

const double RobitKalmanFilter::update(double measurement)
{
   measurementUpdate();
   X = X + ((measurement - X) * K);
   return X;
}


RobitRansacLine::RobitRansacLine(const Mat& img)
{
    m_ImgData=(unsigned char *)img.data;
    m_width  =img.cols;
    m_height =img.rows;

    m_pos.clear();

    m_noSamples=true;
    m_degree=0.0;
}

void RobitRansacLine::getSamples()
{
    const unsigned int threshold_nSamples=(m_width+m_height)/4;
    Point Pt_temp;

    for(int i=0,idxY=0; i<m_height;i++,idxY+=m_width)
    {
        for(int j=0; j<m_width;j++)
        {
            if(m_ImgData[idxY+j]!=0)
            {
                Pt_temp.x = j;
                Pt_temp.y = i;

                m_pos.push_back(Pt_temp);
            }
        }
    }

    if(m_pos.size()>=threshold_nSamples)m_noSamples=false;
}

void RobitRansacLine::compute_model_parameter(const vector<Point>& vInlierPoint, sLine &model)
{
// PCA ¹æ½ÄÀ¸·Î Á÷¼± ¸ðµ¨ÀÇ ÆÄ¶ó¸ÞÅÍ¸¦ ¿¹ÃøÇÑ´Ù.
    double sx  = 0, sy  = 0;
    double sxx = 0, syy = 0;
    double sxy = 0;

    for(size_t i = 0; i<vInlierPoint.size();++i)
    {
        const double x = (double)vInlierPoint.at(i).x;
        const double y = (double)vInlierPoint.at(i).y;

        sx  += x;
        sy  += y;
        sxx += x*x;
        sxy += x*y;
        syy += y*y;
    }
    const double sw= vInlierPoint.size();

//variance;
    const double vxx = (sxx - sx*sx/sw)/sw;
    const double vxy = (sxy - sx*sy/sw)/sw;
    const double vyy = (syy - sy*sy/sw)/sw;

//principal axis
    const double theta = atan2(2.0*vxy, vxx - vyy)/2.0;

    model.mx = cos(theta);
    model.my = sin(theta);

//center of mass(xc, yc)
    model.sx = sx/sw;
    model.sy = sy/sw;
}

double RobitRansacLine::compute_distance(sLine &line, Point &x)const
{
    return fabs((double)((double)(x.x - line.sx)*line.my - ((double)x.y - line.sy)*line.mx))/sqrt(line.mx*line.mx + line.my*line.my);
}

void RobitRansacLine::runRansac()
{
    getSamples();
    if(m_noSamples==true) return;
    srand(time(NULL));

    unsigned int nSamples=0;

    if(nSamples>30)nSamples=30;
    else nSamples=(m_pos.size()*3)/10;

    const double distance_threshold=1.0;
    double max_cost=0.0;

    sLine estimated_model;

    for(int i=0;i<nSamples;i++)
    {
        const int  n=rand()%m_pos.size();
        const int  m=rand()%m_pos.size();

        if(n==m)continue;

        vector<Point> SamplePt;
        SamplePt.push_back(m_pos[n]);
        SamplePt.push_back(m_pos[m]);

        compute_model_parameter(SamplePt,estimated_model);

        double cost = 0.;

        const double sqrtmmyy=sqrt(estimated_model.mx*estimated_model.mx + estimated_model.my*estimated_model.my);
        const double mysx=estimated_model.sx*estimated_model.my;
        const double mxsy=estimated_model.sy*estimated_model.mx;
        const double sub_ms=mxsy-mysx;
        for(size_t i=0; i<m_pos.size(); i++)
        {
            const double distance = fabs((((double)m_pos[i].x*estimated_model.my)-((double)m_pos[i].y*estimated_model.mx))+sub_ms)/sqrtmmyy;
            if (distance < distance_threshold)
            {
                cost += 1.0;
            }
        }

        if(cost>max_cost)
        {
            m_line_A= m_pos[n];
            m_line_B= m_pos[m];
            max_cost=cost;
        }
    }

    if(m_line_A.x<m_line_B.x)
    {
        Point tempP=m_line_A;
        m_line_A=m_line_B;
        m_line_B=tempP;
    }

    m_degree=-(atan2(m_line_A.y-m_line_B.y,m_line_A.x-m_line_B.x)*180.0)/CV_PI;
    if(m_degree<0.0)m_degree+=180.0;
}



RobitRansacCircle::RobitRansacCircle(const Mat& img)
{
    m_ImgData=(unsigned char *)img.data;
    m_width  =img.cols;
    m_height =img.rows;

    m_pos.clear();

    m_noSamples=true;

    m_center.x=0;
    m_center.y=0;

    m_radius=0.0;
}


void RobitRansacCircle::getSamples()
{
    Point Pt_temp;
    for(int i=0,idxY=0; i<m_height;i++,idxY+=m_width)
    {
        for(int j=0; j<m_width;j++)
        {
            if(m_ImgData[idxY+j]!=0)
            {
                Pt_temp.x = j;
                Pt_temp.y = i;

                m_pos.push_back(Pt_temp);
            }
        }
    }
    if(m_pos.size()>m_minRadius*CV_PI)m_noSamples=false;
}

bool RobitRansacCircle::getLeastSquareCircle(vector<Point> PtVec, Point &center, double &radius) const
{
    double sx = 0.0,  sy = 0.0;
    double sx2 = 0.0, sy2 = 0.0, sxy = 0.0;
    double sx3 = 0.0, sy3 = 0.0, sx2y = 0.0, sxy2 = 0.0;

    /* compute summations */
    for (int k=0; k<PtVec.size(); k++) {
       double x=PtVec[k].x;
       double y=PtVec[k].y;
       double xx=x*x;
       double yy=y*y;
       sx = sx + x;
       sy = sy + y;
       sx2 = sx2 + xx;
       sy2 = sy2 + yy;
       sxy = sxy + x * y;
       sx3 = sx3 + x * xx;
       sy3 = sy3 + y * yy;
       sx2y = sx2y + xx * y;
       sxy2 = sxy2 + yy * x;
    }
    /* compute a's,b's,c's */
    double a1 = 2.0 * (sx * sx - sx2 * (double)PtVec.size());
    double a2 = 2.0 * (sx * sy - sxy * (double)PtVec.size());
    double b1 = a2;
    double b2 = 2.0 * (sy * sy - sy2 * (double)PtVec.size());

    double c1 = sx2 * sx - sx3 * (double)PtVec.size() + sx * sy2 - sxy2 * (double)PtVec.size();
    double c2 = sx2 * sy - sy3 * (double)PtVec.size() + sy * sy2 - sx2y * (double)PtVec.size();

    double det= a1*b2 - a2*b1;

    if (fabs(det)<1.e-10) {                /*collinearÇÑ °æ¿ìÀÓ;*/
       return false;
    }

    double cx = (c1 * b2 - c2 * b1) / det;
    double cy = (a1 * c2 - a2 * c1) / det;

    /* compute radius squared */
    double radsq= (sx2 - 2.0 * sx * cx + cx * cx * (double)PtVec.size() + sy2 - 2.0 * sy * cy + cy * cy * (double)PtVec.size()) / (double)PtVec.size();

    radius =sqrt(radsq);
    if(radius>m_maxRadius||radius<m_minRadius) return false;

    /* integer value center */

    center.x = int(cx + 0.5) ;
    center.y = int(cy + 0.5) ;

    if(center.x<0 || center.y<0)return false;


    return true;
}

bool RobitRansacCircle::getCircle(Point& p1,Point& p2,Point& p3, Point& center, double& radius)const
{
    const double A = p1.x - p2.x; //R :P3  P: P2  Q: P1
    const double B = p1.y - p2.y;
    const double C = p3.x - p2.x;
    const double D = p3.y - p2.y;
    const double E = A * ((double)(p2.x + p1.x)) + B * ((double)(p2.y + p1.y));
    const double F = C * ((double)(p2.x + p3.x)) + D * ((double)(p2.y + p3.y));

    const double G = 2.0 * ( (A * D )- (B * C));

    if (G != 0.) {
        center.x = (((D * E) - (B * F))) / G;
        center.y = (((A * F) - (C * E))) / G;

        radius = sqrt((double)((center.x-p1.x)*(center.x-p1.x) + (center.y-p1.y)*(center.y-p1.y)));
    }

    if(radius>m_maxRadius||radius<m_minRadius) return false;
    return true;
}

double RobitRansacCircle::verifyCircle(const Point& center, const double radius)const
{
    const double threshold_dist= 1.0;                       ////!!
    const double thresholdOutLier_dist= radius+(threshold_dist*2.0);

    double inlier = 0;

    for(size_t i=0; i<m_pos.size()*3/4; i++)
    {
        const double distance=norm(m_pos[i]-center);
        if (fabs(distance-radius) <= threshold_dist)inlier += 1.0;
    }

    const double cost= ((double)inlier)/((2.0*CV_PI)*radius);
    return cost;
}

void RobitRansacCircle::runRansac()
{
    getSamples();
    if(m_noSamples==true) return;
    srand(time(NULL));

    const int nSamples=(int)((double)m_pos.size()*0.5);
    const double minCirclePercentage = 0.25;                    ////0.25
    const double min_point_separation = 5.0;
    double max_cPerc=0.0;

    Point centerP;


    for(int i=0;i<nSamples;i++)
    {
        const int  n=rand()%m_pos.size();
        const int  m=rand()%m_pos.size();
        const int  k=rand()%m_pos.size();
        const int  q=rand()%m_pos.size();
        const int  r=rand()%m_pos.size();
        const int  s=rand()%m_pos.size();
        const int  t=rand()%m_pos.size();
        const int  v=rand()%m_pos.size();
    //    const int  z=rand()%m_pos.size();
     //   const int  e=rand()%m_pos.size();

        if(n==m)continue;
        if(n==k)continue;
        if(k==m)continue;

        if(m_pos[n].x==m_pos[m].x)continue;
        if(m_pos[n].y==m_pos[m].y)continue;

        if(m_pos[n].x==m_pos[k].x)continue;
        if(m_pos[n].y==m_pos[k].y)continue;

        if(m_pos[m].x==m_pos[k].x)continue;
        if(m_pos[m].y==m_pos[k].y)continue;

        const double distm=norm(m_pos[n]-m_pos[m]);
        const double distn=norm(m_pos[m]-m_pos[k]);
        const double distk=norm(m_pos[k]-m_pos[n]);


        if(distm < min_point_separation ||
           distn < min_point_separation ||
           distk < min_point_separation)continue;

        vector< Point > PtVec;

        PtVec.push_back(m_pos[n]);
        PtVec.push_back(m_pos[m]);
        PtVec.push_back(m_pos[k]);
        PtVec.push_back(m_pos[q]);
        PtVec.push_back(m_pos[r]);
        PtVec.push_back(m_pos[s]);
        PtVec.push_back(m_pos[t]);
        PtVec.push_back(m_pos[v]);
     //   PtVec.push_back(m_pos[z]);
     //   PtVec.push_back(m_pos[e]);

        double radius=0.0;

        /*if(getCircle(m_pos[n],m_pos[m],m_pos[k],centerP,radius)==false)continue;

        cout<<"centerP.x = "<<centerP.x<<endl;
        cout<<"centerP.y = "<<centerP.y<<endl;
        cout<<"radius = "<<radius<<endl;*/

        if(getLeastSquareCircle(PtVec,centerP,radius)==false)continue;

        /*cout<<"GLScenterP.x = "<<centerP.x<<endl;
        cout<<"GLScenterP.y = "<<centerP.y<<endl;
        cout<<"GLSradius = "<<radius<<endl;*/

        const double cPerc = verifyCircle(centerP,radius);
//        std::cout << "cPerc = " << cPerc << std::endl;
        if(cPerc >= minCirclePercentage)
        {
            if(radius>max_cPerc)
            {
                max_cPerc=radius;
                m_center=centerP;
                m_radius=radius;
            }
        }
    }

    m_cost=max_cPerc;
}


RobitImageRotate::RobitImageRotate(const Mat& img)
{
    m_img_rotate =Mat::zeros(img.size(),CV_8UC1);
    m_ImgData  =(unsigned char *)img.data;
    m_width    =img.cols;
    m_height   =img.rows;
}

void RobitImageRotate::rotate(const double theta)
{
    double imgMatrix[][3]={{0,},
                           {0,},
                           {0,}};

    const double Radian = (theta * 3.141592) / 180.0f;

    imgMatrix[0][0] = cos(Radian); imgMatrix[0][1] = -sin(Radian); imgMatrix[0][2] = 0;
    imgMatrix[1][0] = sin(Radian); imgMatrix[1][1] = cos(Radian);  imgMatrix[1][2] = 0;
    imgMatrix[2][0] = 0;           imgMatrix[2][1] = 0;            imgMatrix[2][2] = 1;

    for(int i=0; i<m_height;i++)
    {
        int idxY=m_width*i;

        for(int j=0; j<m_width;j++)
        {
            int x=(imgMatrix[0][0]*(j-m_width/2)+imgMatrix[0][1]*(i-m_height/2))+(imgMatrix[0][2]*1);
            x+=m_width/2;// col/2
            if(x>=m_width||x<0)continue;


            int y=((imgMatrix[1][0]*(j-m_width/2))+imgMatrix[1][1]*(i-m_height/2))+(imgMatrix[1][2]*1);
            y+=m_height/2;// row/2
            if(y>=m_height||y<0)continue;

            m_img_rotate.data[m_width*y+x]=m_ImgData[idxY+j];
        }
    }
    interPolation();
}

void RobitImageRotate::interPolation()
{
    int LpxVal = 0;
    int RpxVal = 0;

    for(int i=0,idxY=0; i<m_height; i++,idxY+=m_width)
    {
        for(int j=0; j<m_width; j++)
        {
            if(j == 0)
            {
                RpxVal = m_img_rotate.data[idxY+j+1];
                LpxVal = RpxVal;
            }
            else if(j == m_width- 1)
            {
                LpxVal = m_img_rotate.data[idxY+j-1];
                RpxVal = LpxVal;
            }
            else
            {
                LpxVal= m_img_rotate.data[idxY+(j-1)];
                RpxVal= m_img_rotate.data[idxY+(j+1)];
            }

            if(m_img_rotate.data[idxY+j]==0
               &&LpxVal != 0
               &&RpxVal != 0)
            {
                m_img_rotate.data[idxY+j]=255;
            }
        }
    }

}

RobitDetectGoalPost::RobitDetectGoalPost(const Mat& img)
    :RobitRansacLine(img)
{
    m_isGoal   = false;
}

void RobitDetectGoalPost::getGoalPost()
{
    getSamples();
    if(m_noSamples==true) return;
    srand(time(NULL));

    unsigned int nSamples=(m_pos.size()*4)/10;


    const double distance_threshold=1.0;


    sLine estimated_model;

    Point lineA,lineB;


    double degree_past=-1.0;
    int nCntBar = 0;

    for(int i=0;i<nSamples;i++)
    {
        if(m_pos.size()<=2)break;

        const int  n=rand()%m_pos.size();
        const int  m=rand()%m_pos.size();

        if(n==m)continue;
        if(norm(m_pos[n]-m_pos[m])<m_minLength)continue;

        vector<Point> SamplePt;
        SamplePt.push_back(m_pos[n]);
        SamplePt.push_back(m_pos[m]);

        compute_model_parameter(SamplePt,estimated_model);

        double cost = 0.;

        const double sqrtmmyy=sqrt(estimated_model.mx*estimated_model.mx + estimated_model.my*estimated_model.my);
        if(sqrtmmyy==0.0)continue;

        const double mysx=estimated_model.sx*estimated_model.my;
        const double mxsy=estimated_model.sy*estimated_model.mx;
        const double sub_ms=mxsy-mysx;

        vector<Point> PtLineVec;

        for(size_t i=0; i<m_pos.size(); i++)
        {

            const double distance = fabs((((double)m_pos[i].x*estimated_model.my)-((double)m_pos[i].y*estimated_model.mx))+sub_ms)/sqrtmmyy;
            if (distance < distance_threshold)
            {
                cost += 1.0;
            }
            else
            {
                PtLineVec.push_back(m_pos[i]);
            }
        }

        const double threshDist= norm(m_pos[n]-m_pos[m])/3.0;
        if(cost<threshDist)continue;

        lineA = m_pos[n];
        lineB = m_pos[m];

        if(lineA.x<lineB.x)
        {
            Point tempP=lineA;
            lineA=lineB;
            lineB=tempP;
        }

        double degree=-(atan2(lineA.y-lineB.y,lineA.x-lineB.x)*180.0)/CV_PI;
        if(degree<0.0)degree+=180.0;

        if(fabs(degree-90.0)<=20)
        {
            nCntBar++;
            if(nCntBar>=2)
            {
                m_isGoal=true;
                break;
            }
        }

        degree_past=degree;

        m_pos.clear();
        m_pos = PtLineVec;
    }
}
