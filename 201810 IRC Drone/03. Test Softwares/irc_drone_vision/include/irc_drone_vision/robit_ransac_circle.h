#ifndef ROBIT_RANSAC_CIRCLE_H
#define ROBIT_RANSAC_CIRCLE_H

#include <opencv2/opencv.hpp>
#include <iostream>
#include <time.h>
#include <vector>

using namespace std;
class JYJ_RansacCircle
{
public:
  JYJ_RansacCircle(const double   &min_radius = 10,
                   const double   &max_radius = 100,
                   const double   &similarity = 0.6,
                   const double   &success_rate = 0.99,
                   const double   &inlier_rate = 0.5,
                   const double   &distance_threshold = 2.0,
                   const double   &length_threshold = 1.0);

  JYJ_RansacCircle(const cv::Mat  &Img,
                   const double   &min_radius = 10,
                   const double   &max_radius = 100,
                   const double   &similarity = 0.6,
                   const double   &success_rate = 0.99,
                   const double   &inlier_rate = 0.5,
                   const double   &distance_threshold = 2.0,
                   const double   &length_threshold = 1.0);

public:
  ~JYJ_RansacCircle();

public:
  unsigned char*          m_Img_data;

public:
  void                    runRansac();

  void                    setMinRadius          (const double& min_radius){m_min_radius = min_radius;}
  void                    setMaxRadius          (const double& max_radius){m_max_radius = max_radius;}
  void                    setSimilarity         (const double& similarity){m_similarity = similarity;}
  void                    setInlierRate         (const double& inlier_rate){m_inlier_rate = inlier_rate;}
  void                    setSuccessRate        (const double& success_rate){m_success_rate = success_rate;}
  void                    setLengthThreshold    (const double& length_threshold){m_length_threshold = length_threshold;}
  void                    setDistanceThreshold  (const double& distance_threshold){m_distance_threshold = distance_threshold;}

  double                  getRadius             ()const{return m_radius;}
  cv::Point               getCenterPoint        ()const{return m_center;}

  double                  m_real_similarity;
  vector<cv::Point>       m_edge_point;
    double                  m_max_radius;
private:
  cv::Mat                 m_Image;
  int                     m_width;
  int                     m_height;
  double                  m_radius;
  cv::Point               m_center;

  double                  m_min_radius;

  double                  m_similarity;
  double                  m_inlier_rate;
  double                  m_success_rate;
  double                  m_length_threshold;
  double                  m_distance_threshold;

private:
  void                    _getEdgePoint();
  unsigned int            _getMaxIteration      (const unsigned int &edge_point_size);
  const double            _getSimilarity        (cv::Point &temp_center, const double& radius);
  bool                    _getSamples           (vector<cv::Point>& sample_point, const unsigned int &edge_point_size);
  bool                    _getCircle            (cv::Point &p1, cv::Point &p2, cv::Point &p3, cv::Point &center, double &radius) const;
};

#endif // ROBIT_RANSAC_CIRCLE_H
