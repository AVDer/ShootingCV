#include "target_image_provider.h"

TargetImageProvider::TargetImageProvider() : QQuickImageProvider(QQuickImageProvider::Image) {}

void TargetImageProvider::setOpenCVInterface(std::shared_ptr<OpenCVInterface> interface) {
  interface_ = interface;
}

void TargetImageProvider::newOpenCVFrame(cv::Mat frame) {
  image_ = QImage(static_cast<const unsigned char *>(frame.data), frame.cols, frame.rows,
                  QImage::Format_RGB888)
               .rgbSwapped();
}

QImage TargetImageProvider::requestImage(const QString &id, QSize *size,
                                         const QSize &requestedSize) {
  if (id == "sample") {
    auto frame = interface_->get_sample_frame();
    image_ = QImage(static_cast<const unsigned char *>(frame.data), frame.cols, frame.rows,
                    QImage::Format_RGB888)
                 .rgbSwapped();
    if (size) *size = image_.size();
    return image_.scaled(requestedSize.width() > 0 ? requestedSize.width() : image_.width(),
                         requestedSize.height() > 0 ? requestedSize.height() : image_.height(),
                         Qt::AspectRatioMode::KeepAspectRatio);
  }
  else if (id == "target") {
      auto frame = interface_->get_frame();
      image_ = QImage(static_cast<const unsigned char *>(frame.data), frame.cols, frame.rows,
                      QImage::Format_RGB888)
                   .rgbSwapped();
      if (size) *size = image_.size();
      return image_.scaled(requestedSize.width() > 0 ? requestedSize.width() : image_.width(),
                           requestedSize.height() > 0 ? requestedSize.height() : image_.height(),
                           Qt::AspectRatioMode::KeepAspectRatio);
    }
  return QImage();
}

std::shared_ptr<OpenCVInterface> TargetImageProvider::interface_;
QImage TargetImageProvider::image_;
