#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <chrono>
#include <cstdio>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <thread>

#include "target.h"

enum hue_color { green = 60, blue = 120, yellow = 30, red = 180 };

const uchar kBlackThreshold{50};
const uchar kWhiteThreshold{200};
const char* const kInputWindow = "Inout video";

using namespace std::chrono_literals;

int main(int /*argc*/, char** /*argv*/) {
  cv::VideoCapture input_video(0);
  if (!input_video.isOpened()) {
    printf("Can't open video\n");
    return -1;
  }

  double fps{30};

  cv::namedWindow(kInputWindow, cv::WINDOW_NORMAL);
  cv::Mat input_video_frame;

  Target target;
  target.set_description(
      {ColorChange::BlackWhite, 10, ColorChange::WhiteBlack, 9, ColorChange::BlackWhite, 8,
       ColorChange::WhiteBlack, 7,  ColorChange::BlackWhite, 6, ColorChange::WhiteBlack, 5,
       ColorChange::BlackWhite, 4,  ColorChange::WhiteBlack, 3, ColorChange::BlackWhite, 2,
       ColorChange::WhiteBlack, 1,  ColorChange::Finish});

  while (true) {
    input_video >> input_video_frame;
    target.set(input_video_frame);
    if (target.find_center() != ShootError::NoError) {
      printf("Center find error\n");
    }
    //target.find_points();
    //target.add_markers();

    cv::imshow(kInputWindow, input_video_frame);
    //cv::imshow(kInputWindow, target.grey_frame());
    if (cv::waitKey(1000 / fps) >= 0) break;

    // cv::imwrite("result.png", original_image);
  }
  input_video.release();

  return 0;
}