#include "../include/irc_drone_vision/robit_ransac.h"

JYJ_RansacLine::JYJ_RansacLine(const double  &success_rate,
                               const double  &inlier_rate,
                               const double &distance_threshold,
                               const double &length_threshold)
{
  if(!m_Image.empty())         m_Image.release();

  m_height                     = 0;
  m_width                      = 0;
  m_inlier_rate                = inlier_rate;
  m_success_rate               = success_rate;
  m_length_threshold           = length_threshold;
  m_distance_threshold         = distance_threshold;
}

JYJ_RansacLine::JYJ_RansacLine(const cv::Mat &Img,
                               const double  &success_rate,
                               const double  &inlier_rate,
                               const double &distance_threshold,
                               const double &length_threshold)
{
  m_img_data                   = (unsigned char*)Img.data;

  m_height                     = Img.rows;
  m_width                      = Img.cols;
  m_inlier_rate                = inlier_rate;
  m_success_rate               = success_rate;
  m_length_threshold           = length_threshold;
  m_distance_threshold         = distance_threshold;
}

void JYJ_RansacLine::runRansac()
{
  if(m_Image.channels() != 1)
  {
    std::cout << "Image type is not collect" << std::endl;
    return;
  }

  _getEdgePoint();

  unsigned int edge_point_size = m_edge_point.size();
  if(edge_point_size < 2)    return;
  srand(time(NULL));

  double max_cost = 0.0;
  unsigned int max_iteration = _getMaxIteration(edge_point_size);

  for(int i = 0 ; i < max_iteration ; i++)
  {
    vector<cv::Point> sample_point;

    if(_getSamples(sample_point, edge_point_size) == false) continue;

    int cost = _getLineCost(sample_point, edge_point_size);

    if(max_cost < cost)
    {
      max_cost = cost;
      m_line_start = sample_point[0];
      m_line_end   = sample_point[1];
    }
  }

  if(m_line_start.x < m_line_end.x)
  {
    cv::Point temp_point = m_line_start;
    m_line_start = m_line_end;
    m_line_end = temp_point;
  }

  m_degree=-(atan2(m_line_start.y-m_line_end.y,m_line_start.x-m_line_end.x)*180.0)/CV_PI;
  if(m_degree<0.0)m_degree+=180.0;
}

void JYJ_RansacLine::_getEdgePoint()
{
  for(int nRow = 0, nowY = 0 ; nRow < m_height ; nRow ++, nowY += m_width)
   for(int nCol = 0 ; nCol < m_width ; nCol ++)
   {
     if(*(m_img_data + nowY + nCol) != 0)
       m_edge_point.push_back(cv::Point(nCol, nRow));
   }
}

bool JYJ_RansacLine::_getSamples(vector<cv::Point> &sample_point, const unsigned int& edge_point_size)
{
  const int n = rand() % edge_point_size;
  const int m = rand() % edge_point_size;

  if (n == m ) return false;
  if (norm(m_edge_point[n]-m_edge_point[m]) < m_length_threshold) return false;

  sample_point.push_back(m_edge_point[n]);
  sample_point.push_back(m_edge_point[m]);

  if(sample_point[0].x == sample_point[1].x) return false;
  if(sample_point[0].y == sample_point[1].y) return false;
  return true;
}

const double JYJ_RansacLine::_getLineCost(vector<cv::Point> &sample_point, const unsigned int& edge_point_size)
{
  const double gradient = double(sample_point[1].y - sample_point[0].y)/
                    double(sample_point[1].x - sample_point[0].x);
  const double yValue = sample_point[0].y - (gradient * sample_point[0].x);
  const double sqrt_gradient = sqrt((gradient * gradient) + 1);

  int cost = 0;
  for(int i = 0 ; i < edge_point_size ; i ++)
  {
    const double distance = abs((m_edge_point[i].x * gradient) - m_edge_point[i].y + yValue) / sqrt_gradient;

    if(distance < m_distance_threshold) cost ++;
  }

  return cost;
}

unsigned int JYJ_RansacLine::_getMaxIteration(const unsigned int& edge_point_size)
{
  return (unsigned int)((log(1.0 - m_success_rate)/
                         log(1.0 - pow(m_inlier_rate, 2))) *
                         edge_point_size) / 1000;
}

JYJ_RansacLine::~JYJ_RansacLine()
{
  m_Image.release();
}
