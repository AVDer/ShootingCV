#include "target.h"

ShootError Target::load(const std::string& filename) {
  original_name_ = filename;
  marked_ = cv::imread(filename, cv::ImreadModes::IMREAD_COLOR);
  if (marked_.empty()) {
    std::cerr << "No image data!" << std::endl;
    return ShootError::NotFound;
  } else {
    width_ = marked_.cols;
    height_ = marked_.rows;
    printf("Image size: x: %d, y: %d\n", width_, height_);
  }
  cv::cvtColor(marked_, grey_, CV_BGR2GRAY);
  return ShootError::NoError;
}

ShootError Target::set(cv::Mat& frame) {
  cv::resize(frame, frame, cv::Size(0, 1080));
  original_ = frame.clone();
  marked_ = frame;
  width_ = marked_.cols;
  height_ = marked_.rows;
  cv::cvtColor(marked_, grey_, CV_BGR2GRAY);
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
    for (int c{0}; c < width_ / 2; ++c) {
      // Print center search cross

      // if (!xr) cv::circle(original_, cv::Point(width_ / 2 + c, height_ / 2),
      // 1, cv::Scalar(255,
      // 0, 0)); if (!xl) cv::circle(original_, cv::Point(width_ / 2 - c,
      // height_ / 2), 1,
      // cv::Scalar(255, 0, 0)); if (!yd) cv::circle(original_, cv::Point(width_
      // / 2, height_ / 2 +
      // c), 1, cv::Scalar(255, 0, 0)); if (!yu) cv::circle(original_,
      // cv::Point(width_ / 2, height_
      // / 2 - c), 1, cv::Scalar(255, 0, 0));

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

    if (!xr || !xl || !yd || !yu) {
      return ShootError::UnexpectedImage;
    }

    center_.x = (xl + xr) / 2;
    center_.y = (yu + yd) / 2;
  }

  // Center marker

  cv::circle(marked_, cv::Point(center_), 1, cv::Scalar(255, 255, 0), 3);
  // cv::circle(original_, cv::Point(width_ / 2, height_ / 2), 1, cv::Scalar(0,
  // 0, 255), 3);

  return ShootError::NoError;
}

ShootError Target::find_points() {
  bool is_white{false};
  size_t target_index{0};

  for (int r{0}; r < height_ / 2; ++r) {
    bool found{false};
    uchar value = grey_.at<uchar>(center_.y + r, center_.x + r);

    if (is_white && value < kBlackThreshold) {
      is_white = false;
      if (target_description_[target_index] != ColorChange::WhiteBlack) {
        return ShootError::UnexpectedImage;
      }
      found = true;
    }

    if (!is_white && value > kWhiteThreshold) {
      is_white = true;
      if (target_description_[target_index] != ColorChange::BlackWhite) {
        return ShootError::UnexpectedImage;
      }
      found = true;
    }

    if (found) {
      cv::circle(marked_, cv::Point(center_.x + r, center_.y + r), 1,
                 cv::Scalar(255, 0, 0), 2);
    }

    if (found && target_description_[++target_index] < ColorChange::Start) {
      auto radius = sqrt(r * r + r * r);
      target_points_[target_description_[target_index]] =
          static_cast<int>(radius);
      if (target_description_[++target_index] == ColorChange::Finish) {
        break;
      }
    }
  }
  return ShootError::NoError;
}

void Target::print_points() {
  // for (const auto& [point, radius] : target_points_) {
  //  std::cout << point << "\t=>\t" << radius << std::endl;
  //}
}

void Target::add_markers() {
  for (const auto& t : target_points_) {
    auto point = t.first;
    auto radius = t.second;
    cv::circle(marked_, cv::Point(center_), radius, cv::Scalar(0, 255, 0), 1);
    auto font = cv::HersheyFonts::FONT_HERSHEY_SIMPLEX;
    int base_line;
    cv::Size text_size =
        cv::getTextSize(std::to_string(point), font, 1, 1, &base_line);

    cv::putText(marked_, std::to_string(point),
                cv::Point(center_.x + radius - text_size.width,
                          center_.y + text_size.height / 2),
                font, 1, cv::Scalar(0, 255, 0), 3);
  }
}

void Target::create_model() {
  model_ = original_.clone();
  auto font = cv::HersheyFonts::FONT_HERSHEY_SIMPLEX;
  for (uint16_t i = 0; i <= 10; ++i) {
    cv::circle(model_, center_, target_points_[i], kPointsColors[i].first, -1);
    cv::circle(model_, center_, target_points_[i], kPointsColors[i].second, 1);
    int base_line;
    cv::Size text_size =
        cv::getTextSize(std::to_string(i), font, 1, 1, &base_line);

    cv::putText(model_, std::to_string(i),
                cv::Point(center_.x + target_points_[i] - text_size.width,
                          center_.y + text_size.height / 2),
                font, 1, kPointsColors[i].second, 3);
  }
}
