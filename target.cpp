#include "target.h"

ShootError Target::load(const std::string& filename) {
  original_name_ = filename;
  original_ = cv::imread(filename, cv::ImreadModes::IMREAD_COLOR);
  if (original_.empty()) {
    std::cerr << "No image data!" << std::endl;
    return ShootError::NotFound;
  } else {
    width_ = original_.cols;
    height_ = original_.rows;
    printf("Image size: x: %zu, y: %zu\n", width_, height_);
  }
  cv::cvtColor(original_, grey_, CV_BGR2GRAY);
  return ShootError::NoError;
}

ShootError Target::set(cv::Mat& frame) {
  original_ = frame;
  width_ = original_.cols;
  height_ = original_.rows;
  cv::cvtColor(original_, grey_, CV_BGR2GRAY);
  return ShootError::NoError;
}

void Target::set_description(std::vector<uint16_t>&& target_description) {
  target_description_ = std::move(target_description);
}

ShootError Target::find_center() {

  // Check if we should start from black "10"
  // Assume so
  bool blackCenter = grey_.at<uchar>(height_ / 2, width_ / 2) < kBWLimit;

  if (blackCenter) {
    int xr{}, xl{}, yu{}, yd{};
    for (size_t c{0}; c < width_ / 2; ++c) {

      // Print center search cross

      //if (!xr) cv::circle(original_, cv::Point(width_ / 2 + c, height_ / 2), 1, cv::Scalar(255, 0, 0));
      //if (!xl) cv::circle(original_, cv::Point(width_ / 2 - c, height_ / 2), 1, cv::Scalar(255, 0, 0));
      //if (!yd) cv::circle(original_, cv::Point(width_ / 2, height_ / 2 + c), 1, cv::Scalar(255, 0, 0));
      //if (!yu) cv::circle(original_, cv::Point(width_ / 2, height_ / 2 - c), 1, cv::Scalar(255, 0, 0));

      // Actual search

      if (grey_.at<uchar>(height_ / 2, width_ / 2 + c) > kBWLimit && !xr) {
        xr = width_ / 2 + c;
      }
      if (grey_.at<uchar>(height_ / 2, width_ / 2 - c) > kBWLimit && !xl) {
        xl = width_ / 2 - c;
      }
      if (grey_.at<uchar>(height_ / 2 + c, width_ / 2) > kBWLimit && !yd) {
        yd = height_ / 2 + c;
      }
      if (grey_.at<uchar>(height_ / 2 - c, width_ / 2) > kBWLimit && !yu) {
        yu = height_ / 2 - c;
      }
    }
    center_.x = (xl + xr) / 2;
    center_.y = (yu + yd) / 2;
  }

  // Center marker

  cv::circle(original_, cv::Point(center_), 1, cv::Scalar(255, 255, 0), 3);
  // cv::circle(original_, cv::Point(width_ / 2, height_ / 2), 1, cv::Scalar(0, 0, 255), 3);

  return ShootError::NoError;
}

ShootError Target::find_points() {
  bool is_white{false};
  size_t target_index{0};

  for (size_t r{0}; r < height_ / 2; ++r) {
    bool found{false};
    uchar value = grey_.at<uchar>(width_ / 2 + r, height_ / 2 + r);

    if (is_white && value < kBlackThreshold) {
      is_white = false;
      if (target_description_[target_index] != ColorChange::WhiteBlack) {
        //printf("Recognition error\n");
        return ShootError::UnexpectedImage;
      }
      found = true;
    }

    if (!is_white && value > kWhiteThreshold) {
      is_white = true;
      if (target_description_[target_index] != ColorChange::BlackWhite) {
        //printf("Recognition error\n");
        return ShootError::UnexpectedImage;
      }
      found = true;
    }

    if (found) {
      cv::circle(original_, cv::Point(width_ / 2 + r, height_ / 2 + r), 1, cv::Scalar(255, 0, 0), 2);
    }

    if (found && target_description_[++target_index] < ColorChange::Start) {
      //auto tv = c - width_ / 2;
      auto radius = sqrt(r * r + r * r);
      target_points_[target_description_[target_index]] = static_cast<size_t>(radius);
      if (target_description_[++target_index] == ColorChange::Finish) {
        printf("Done!!!\n");
        break;
      }
    }
  }
  return ShootError::NoError;
}

void Target::print_points() {
  /*
  for (const auto& [point, radius] : target_points_) {
    std::cout << point << "\t=>\t" << radius << std::endl;
  }
  */
}

void Target::add_markers() {
  cv::circle(original_, cv::Point(width_ / 2, height_ / 2), 1, cv::Scalar(0, 0, 255), 3);
  for (const auto& t : target_points_) {
    auto point = t.first;
    auto radius = t.second;
    cv::circle(original_, cv::Point(width_ / 2, height_ / 2), radius, cv::Scalar(0, 255, 0), 1);
    auto font = cv::HersheyFonts::FONT_HERSHEY_SIMPLEX;
    int base_line;
    cv::Size text_size = cv::getTextSize(std::to_string(point), font, 1, 3, &base_line);
    
    cv::putText(
        original_, std::to_string(point),
        cv::Point(width_ / 2 + radius - text_size.width, height_ / 2 + text_size.height / 2), font,
        1, cv::Scalar(0, 255, 0), 3);
        
  }
  //cv::imwrite("marker_" + original_name_, original_);
}