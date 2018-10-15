#include "opencv_interface.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <chrono>
#include <cstdio>
#include <iostream>
#include <thread>

#include "target.h"

OpenCVInterface::OpenCVInterface() : input_video_(new cv::VideoCapture(0)) {
  if (!input_video_->isOpened()) {
    printf("Can't open video\n");
  }
}

OpenCVInterface::~OpenCVInterface() { input_video_->release(); }

void OpenCVInterface::initialize() {
  Target target;
  target.set_description(
      {ColorChange::BlackWhite, 10, ColorChange::WhiteBlack, 9, ColorChange::BlackWhite, 8,
       ColorChange::WhiteBlack, 7, ColorChange::BlackWhite, 6, ColorChange::WhiteBlack, 5,
       ColorChange::BlackWhite, 4, ColorChange::WhiteBlack, 3, ColorChange::BlackWhite, 2,
       // ColorChange::WhiteBlack, 1,
       ColorChange::Finish});

  /*
  while (true) {
    input_video >> input_video_frame;
    target.set(input_video_frame);
    if (target.find_center() != ShootError::NoError) {
      printf("Center find error\n");
      continue;
    }
    if (target.find_points() != ShootError::NoError) {
      printf("Points find error\n");
      continue;
    }
    target.add_markers();

    cv::imshow(kInputWindow, input_video_frame);
    //cv::imshow(kInputWindow, target.grey_frame());
    if (cv::waitKey(1000 / fps) >= 0) break;

    // cv::imwrite("result.png", original_image);
  }
  */
}

cv::Mat OpenCVInterface::get_frame() {
  (*input_video_) >> input_video_frame_;
  return input_video_frame_;
}

cv::Mat OpenCVInterface::get_sample_frame() {
  cv::Mat sample_frame = cv::Mat(800, 600, CV_8UC3);
  cv::randu(sample_frame, cv::Scalar::all(0), cv::Scalar::all(255));
  return sample_frame;
}
