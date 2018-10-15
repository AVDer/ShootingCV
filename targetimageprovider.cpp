#include "targetimageprovider.h"

TargetImageProvider::TargetImageProvider() : QQuickImageProvider(QQuickImageProvider::Image) {}

void TargetImageProvider::newOpenCVFrame(cv::Mat frame) {
  image_ = QImage(static_cast<const unsigned char *>(frame.data), frame.cols, frame.rows,
                  QImage::Format_RGB888)
               .rgbSwapped();
  // image_.save("test.png");
}

QImage TargetImageProvider::requestImage(const QString &id, QSize *size,
                                         const QSize &requestedSize) {
  if (id == "target") {
    if (size) *size = image_.size();
    return image_.scaled(requestedSize.width() > 0 ? requestedSize.width() : image_.width(),
                         requestedSize.height() > 0 ? requestedSize.height() : image_.height(),
                         Qt::AspectRatioMode::KeepAspectRatio);
  }
  return QImage();
}

QImage TargetImageProvider::image_;
