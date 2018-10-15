#ifndef OPENCVHANDLER_H
#define OPENCVHANDLER_H

#include <opencv2/opencv.hpp>

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
  cv::Mat get_frame();
  cv::Mat get_sample_frame();

 private:
  std::unique_ptr<cv::VideoCapture> input_video_;
  cv::Mat input_video_frame_;
};

#endif
