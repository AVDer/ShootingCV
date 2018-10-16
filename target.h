#pragma once

#include <cstdio>
#include <iostream>
#include <map>
#include <opencv2/opencv.hpp>

#include "global.h"

enum ColorChange : uint16_t { Start = 100, BlackWhite, WhiteBlack, Finish };

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

  cv::Mat original_frame() const { return original_; }
  cv::Mat grey_frame() const { return grey_; }
  cv::Mat marked_frame() const { return marked_; }

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
