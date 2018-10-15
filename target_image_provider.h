#ifndef TARGETIMAGEPROVIDER_H
#define TARGETIMAGEPROVIDER_H

#include <QQuickImageProvider>

#include <memory>
#include <opencv2/opencv.hpp>

#include <opencv_interface.h>

class TargetImageProvider : public QQuickImageProvider {
 public:
  TargetImageProvider();

  void setOpenCVInterface(std::shared_ptr<OpenCVInterface> interface);

  void newOpenCVFrame(cv::Mat frame);

  QImage requestImage(const QString& /*id*/, QSize* /*size*/,
                      const QSize& /*requestedSize*/) override;

 private:
  static std::shared_ptr<OpenCVInterface> interface_;
  static QImage image_;
};

#endif  // TARGETIMAGEPROVIDER_H
