#ifndef OPENCVHANDLER_H
#define OPENCVHANDLER_H

#include <opencv2/opencv.hpp>

class OpenCVHandler
{
public:
    enum hue_color { green = 60, blue = 120, yellow = 30, red = 180 };
    const uchar kBlackThreshold{50};
    const uchar kWhiteThreshold{200};
    const double kFPS{30};

    OpenCVHandler();
    ~OpenCVHandler();

    void initialize();

private:
    std::unique_ptr<cv::VideoCapture> input_video;
};

#endif // OPENCVHANDLER_H
