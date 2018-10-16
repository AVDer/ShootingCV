#include "opencv_interface.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <chrono>
#include <cstdio>
#include <iostream>
#include <thread>

OpenCVInterface::OpenCVInterface()
    : input_video_(new cv::VideoCapture(0)), target_(new Target()) {
  if (!input_video_->isOpened()) {
    printf("Can't open video\n");
  }
}

OpenCVInterface::~OpenCVInterface() { input_video_->release(); }

void OpenCVInterface::initialize() {
  target_->set_description(
      {ColorChange::BlackWhite, 10, ColorChange::WhiteBlack, 9,
       ColorChange::BlackWhite, 8, ColorChange::WhiteBlack, 7,
       ColorChange::BlackWhite, 6, ColorChange::WhiteBlack, 5,
       ColorChange::BlackWhite, 4, ColorChange::WhiteBlack, 3,
       ColorChange::BlackWhite, 2,
       // ColorChange::WhiteBlack, 1,
       ColorChange::Finish});
}

cv::Mat OpenCVInterface::get_original_frame() {
  processFrame();
  return target_->original_frame();
}

cv::Mat OpenCVInterface::get_marked_frame() {
  processFrame();
  return target_->marked_frame();
}

cv::Mat OpenCVInterface::get_grey_frame() {
  processFrame();
  return target_->grey_frame();
}

cv::Mat OpenCVInterface::get_sample_frame() {
  cv::Mat sample_frame = cv::Mat(800, 600, CV_8UC3);
  cv::randu(sample_frame, cv::Scalar::all(0), cv::Scalar::all(255));
  return sample_frame;
}

void OpenCVInterface::processFrame() {
  (*input_video_) >> input_video_frame_;
  if (target_->set(input_video_frame_) != ShootError::NoError) {
    printf("Image setting error\n");
  }
  if (target_->find_center() != ShootError::NoError) {
    printf("Center find error\n");
  }
  if (target_->find_points() != ShootError::NoError) {
    printf("Points find error\n");
  }
  target_->add_markers();
}
