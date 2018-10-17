#ifndef OPENCVHANDLER_H
#define OPENCVHANDLER_H

#include <opencv2/opencv.hpp>

#include "target.h"

class OpenCVInterface {
 public:
  enum hue_color { green = 60, blue = 120, yellow = 30, red = 180 };
  static const uchar kBlackThreshold{50};
  static const uchar kWhiteThreshold{200};
  static const int kFPS{30};
  static constexpr int kUpdatePeriod{1000 / kFPS};

  OpenCVInterface();
  ~OpenCVInterface();

  void initialize();
  cv::Mat get_original_frame();
  cv::Mat get_marked_frame();
  cv::Mat get_grey_frame();
  cv::Mat get_model_frame();
  cv::Mat get_sample_frame();

 private:
  bool is_initialized_{false};
  std::unique_ptr<cv::VideoCapture> input_video_;
  std::unique_ptr<Target> target_;
  cv::Mat input_video_frame_;

  ShootError processFrame();
};

#endif
