#include "../include/irc_drone_vision/robit_ransac_circle.h"

JYJ_RansacCircle::JYJ_RansacCircle(const double   &min_radius,
                                   const double   &max_radius,
                                   const double   &similarity,
                                   const double   &success_rate,
                                   const double   &inlier_rate,
                                   const double   &distance_threshold,
                                   const double   &length_threshold)
{
  if(!m_Image.empty())             m_Image.release();

  m_height                         = 0;
  m_width                          = 0;
  m_center.x                       = 0;
  m_center.y                       = 0;
  m_radius                         = 0.0;

  m_min_radius                     = min_radius;
  m_max_radius                     = max_radius;
  m_similarity                     = similarity;
  m_inlier_rate                    = inlier_rate;
  m_success_rate                   = success_rate;
  m_length_threshold               = length_threshold;
  m_distance_threshold             = distance_threshold;
}

JYJ_RansacCircle::JYJ_RansacCircle(const cv::Mat &Img,
                                   const double   &min_radius,
                                   const double   &max_radius,
                                   const double   &similarity,
                                   const double   &success_rate,
                                   const double   &inlier_rate,
                                   const double   &distance_threshold,
                                   const double   &length_threshold)
{
  if(!m_edge_point.empty())        m_edge_point.clear();

  m_Img_data                       = (unsigned char*)Img.data;
  m_Image                          = Img.clone();
  m_height                         = Img.rows;
  m_width                          = Img.cols;
  m_center.x                       = 0;
  m_center.y                       = 0;
  m_radius                         = 0.0;

  m_min_radius                     = min_radius;
  m_max_radius                     = max_radius;
  m_similarity                     = similarity;
  m_inlier_rate                    = inlier_rate;
  m_success_rate                   = success_rate;
  m_length_threshold               = length_threshold;
  m_distance_threshold             = distance_threshold;
}

void JYJ_RansacCircle::runRansac()
{
  if(m_Image.channels() != 1)
  {
    std::cout << "Image ypte is not collect" << std::endl;
    return;
  }

  _getEdgePoint();
  unsigned int edge_point_size      = m_edge_point.size();
  //std::cout << edge_point_size << std::endl<<std::endl;
  //std::cout <<"max_radius = "<<m_max_radius<<endl;
  if(edge_point_size < 3) return;
  srand(time(NULL));

  cv::Point           center_Pt;
  double              radius        = 0.0;
  double              max_cost      = 0.0;
  unsigned int        max_iteration = _getMaxIteration(edge_point_size);


  if(edge_point_size > 1800) edge_point_size = 1800;
  for(int i = 0 ; i < edge_point_size ; i++)
  {
    vector<cv::Point> sample_Pt;

    if(_getSamples(sample_Pt, edge_point_size) == false) continue;

    if(_getCircle(sample_Pt[0], sample_Pt[1], sample_Pt[2], center_Pt, radius) == false) continue;

    const double similariry = _getSimilarity(center_Pt, radius);

    if(similariry < m_similarity) continue;

    if(similariry > max_cost)
    {
      max_cost = similariry;
      m_center = center_Pt;
      m_radius = radius;
    }
  }
  m_real_similarity = _getSimilarity(m_center, m_radius);
  //std::cout <<"radius = " <<m_radius<<endl;
}

void JYJ_RansacCircle::_getEdgePoint()
{
  for(int nRow = 0, nowY = 0 ; nRow < m_height ; nRow ++, nowY += m_width)
   for(int nCol = 0 ; nCol < m_width ; nCol ++)
   {
     if(*(m_Img_data + nowY + nCol) != 0)
       m_edge_point.push_back(cv::Point(nCol, nRow));
   }
}

unsigned int JYJ_RansacCircle::_getMaxIteration(const unsigned int& edge_point_size)
{
  return (unsigned int)((log(1.0 - m_success_rate)/
                         log(1.0 - pow(m_inlier_rate, 3))) *
                         edge_point_size) * 0.02;
}

bool JYJ_RansacCircle::_getSamples(vector<cv::Point>& sample_point, const unsigned int& edge_point_size)
{
  const unsigned int  n = rand() % edge_point_size;
  const unsigned int  m = rand() % edge_point_size;
  const unsigned int  k = rand() % edge_point_size;

  if(n == m)            return false;
  if(n == k)            return false;
  if(k == m)            return false;

  const double distn = norm(m_edge_point[n] - m_edge_point[m]);
  const double distm = norm(m_edge_point[m] - m_edge_point[k]);
  const double distk = norm(m_edge_point[k] - m_edge_point[n]);

  if(distn < m_length_threshold ||
     distm < m_length_threshold ||
     distk < m_length_threshold)  return false;

  sample_point.push_back(m_edge_point[n]);
  sample_point.push_back(m_edge_point[m]);
  sample_point.push_back(m_edge_point[k]);

  return true;
}

bool JYJ_RansacCircle::_getCircle(cv::Point& p1,cv::Point& p2,cv::Point& p3, cv::Point& center, double& radius)const
{
  const double A = p1.x - p2.x;
  const double B = p1.y - p2.y;
  const double C = p3.x - p2.x;
  const double D = p3.y - p2.y;

  const double G = 2.0 * ( (A * D )- (B * C));
  if (G == 0.) return false;

  const double E = A * ((double)(p2.x + p1.x)) + B * ((double)(p2.y + p1.y));
  const double F = C * ((double)(p2.x + p3.x)) + D * ((double)(p2.y + p3.y));

  center.x = (D * E - B * F) / G;
  center.y = (A * F - C * E) / G;

  radius = sqrt((double)((center.x - p1.x) * (center.x - p1.x) +
                         (center.y - p1.y) * (center.y - p1.y)));

  if(radius > m_max_radius || radius < m_min_radius) return false;
  return true;
}

const double JYJ_RansacCircle::_getSimilarity(cv::Point &temp_center, const double &radius)
{
  int cost = 0;

  for(int i = 0 ; i < m_edge_point.size() ; i ++)
  {
    const double distance = norm(m_edge_point[i] - temp_center);
    if(fabs(distance - radius) < m_distance_threshold) cost += 1;
  }

  return ((double)cost)/(6 * radius);
}

JYJ_RansacCircle::~JYJ_RansacCircle()
{
  m_Image.release();
  m_edge_point.clear();
}
