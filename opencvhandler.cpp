#include "opencvhandler.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <chrono>
#include <cstdio>
#include <iostream>
#include <thread>

#include "target.h"

OpenCVHandler::OpenCVHandler() : input_video(new cv::VideoCapture(0))
{
    if (!input_video->isOpened()) {
      printf("Can't open video\n");
    }
}

OpenCVHandler::~OpenCVHandler()
{
    input_video.release();
}

void OpenCVHandler::initialize()
{
    Target target;
    target.set_description(
        {ColorChange::BlackWhite, 10, ColorChange::WhiteBlack, 9, ColorChange::BlackWhite, 8,
         ColorChange::WhiteBlack, 7,  ColorChange::BlackWhite, 6, ColorChange::WhiteBlack, 5,
         ColorChange::BlackWhite, 4,  ColorChange::WhiteBlack, 3, ColorChange::BlackWhite, 2,
         //ColorChange::WhiteBlack, 1,
         ColorChange::Finish});


    cv::Mat input_video_frame;
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

cv::Mat OpenCVHandler::get_frame()
{
    cv::Mat input_video_frame;
    (*input_video) >> input_video_frame;
    return input_video_frame;
}
