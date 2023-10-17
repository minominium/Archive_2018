#ifndef ROBIT_MASTER_VISION_H
#define ROBIT_MASTER_VISION_H


#include <opencv2/opencv.hpp>
#include <iostream>
#include <time.h>

using namespace std;

class RobitLabeling {

// Constructor
public:
  RobitLabeling();
  RobitLabeling(const cv::Mat& Img,
               const unsigned int& nThreshold,
               const unsigned int& nNeighbor = 4);

// Destructor
public:
  ~RobitLabeling();

// Public variable
public:
  cv::Mat                   m_Image;

// Public Function
public:
  void                      doLabeling();
  unsigned int              getNumOfBlobs();
  vector<cv::Rect>          getRecBlobs();
  void                      sortingRecBlobs();
  vector<cv::Rect>          m_recBlobs;
  unsigned int              m_nBlobs;

// Private variable
private:
  enum                      {LEFT, RIGHT, UP, DOWN};
  enum                      {W, NW, N, NE, E, SE, S, SW};
  unsigned int              m_nThreshold;
  unsigned int              m_nNeighbor;
  vector<unsigned int>      m_vecArea;

  bool                      *m_isVisited;
  cv::Point                 *m_visited_pt;

  int                       m_height;
  int                       m_width;
  unsigned int              m_area;

// Private function
private:
  void                      _labeling_four_neighbor();
  void                      _labeling_eight_neighbor();
  void                      _quick_sort_rec_blobs(int left, int right);

  void                      __dynamicAllocation();
  void                      __freeAllocation();
  const unsigned int        __check_four_neighbor (cv::Point &start_pt, cv::Point &end_pt);
  const unsigned int        __check_eight_neighbor(cv::Point &start_pt, cv::Point &end_pt);

  template <typename T>
  inline void               __swap(T &a, T &b)
  {
    T temp = a;
    a = b;
    b = temp;
  }

};









#endif // ROBIT_MASTER_VISION_H
