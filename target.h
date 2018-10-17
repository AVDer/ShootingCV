#pragma once

#include <array>
#include <cstdio>
#include <iostream>
#include <map>
#include <opencv2/opencv.hpp>

#include "global.h"

enum ColorChange : uint16_t { Start = 100, BlackWhite, WhiteBlack, Finish };

const std::array<cv::Scalar, 11> kPointsColors = {
    cv::Scalar(255, 255, 255),  // 0
    cv::Scalar(255, 255, 255),  // 1
    cv::Scalar(255, 255, 255),  // 2
    cv::Scalar(255, 255, 255),  // 3
    cv::Scalar(255, 255, 255),  // 4
    cv::Scalar(255, 255, 255),  // 5
    cv::Scalar(255, 255, 255),  // 6
    cv::Scalar(255, 255, 255),  // 7
    cv::Scalar(0, 255, 255),    // 8
    cv::Scalar(0, 128, 255),    // 9
    cv::Scalar(0, 0, 255)       // 10
};

class Target {
 public:
  Target() = default;

  [[nodiscard]] ShootError load(const std::string& filename);
  void set_description(std::vector<uint16_t>&& target_description);
  [[nodiscard]] ShootError set(cv::Mat& frame);
  [[nodiscard]] ShootError find_points();
  [[nodiscard]] ShootError find_center();
  void print_points();
  void add_markers();
  void create_model();

  cv::Mat original_frame() const { return original_; }
  cv::Mat grey_frame() const { return grey_; }
  cv::Mat marked_frame() const { return marked_; }
  cv::Mat model_frame() const { return model_; }

 private:
  static const uchar kBlackThreshold{128};
  static const uchar kWhiteThreshold{128};
  static const uchar kBWLimit{128};

  std::string original_name_;
  cv::Mat original_;
  cv::Mat grey_;
  cv::Mat marked_;
  cv::Mat model_;
  cv::Point center_;
  int width_;
  int height_;
  std::vector<uint16_t> target_description_;
  std::map<uint16_t, int> target_points_;
};
