#include "target_image_provider.h"

TargetImageProvider::TargetImageProvider()
    : QQuickImageProvider(QQuickImageProvider::Image) {}

void TargetImageProvider::setOpenCVInterface(
    std::shared_ptr<OpenCVInterface> interface) {
  interface_ = interface;
}

void TargetImageProvider::newOpenCVFrame(cv::Mat frame) {
  image_ = QImage(static_cast<const unsigned char *>(frame.data), frame.cols,
                  frame.rows, QImage::Format_RGB888)
               .rgbSwapped();
}

QImage TargetImageProvider::requestImage(const QString &id, QSize *size,
                                         const QSize &requestedSize) {
  cv::Mat frame;
  QImage::Format format = QImage::Format_RGB888;
  if (id == "sample") {
    frame = interface_->get_sample_frame();
  } else if (id == "original") {
    frame = interface_->get_original_frame();
  } else if (id == "marked") {
    frame = interface_->get_marked_frame();
  } else if (id == "grey") {
    frame = interface_->get_grey_frame();
    format = QImage::Format_Grayscale8;
  } else if (id == "model") {
    frame = interface_->get_model_frame();
  } else {
    return QImage();
  }
  image_ = QImage(static_cast<const unsigned char *>(frame.data), frame.cols,
                  frame.rows, format)
               .rgbSwapped();
  if (size) *size = image_.size();
  return image_.scaled(
      requestedSize.width() > 0 ? requestedSize.width() : image_.width(),
      requestedSize.height() > 0 ? requestedSize.height() : image_.height(),
      Qt::AspectRatioMode::KeepAspectRatio);
}

std::shared_ptr<OpenCVInterface> TargetImageProvider::interface_;
QImage TargetImageProvider::image_;
