#ifndef TARGETIMAGEPROVIDER_H
#define TARGETIMAGEPROVIDER_H

#include <QQuickImageProvider>

#include <opencv2/opencv.hpp>

class TargetImageProvider : public QQuickImageProvider {
 public:
  TargetImageProvider();

  void newOpenCVFrame(cv::Mat frame);

  QImage requestImage(const QString& /*id*/, QSize* /*size*/,
                      const QSize& /*requestedSize*/) override;

 private:
  static QImage image_;
};

#endif  // TARGETIMAGEPROVIDER_H
