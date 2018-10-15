#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QQuickItem>
#include <QQuickView>

#include <opencv_interface.h>
#include <target_image_provider.h>

class MainScreen : public QQuickView {
  Q_OBJECT
 public:
  MainScreen();

 private:
  QObject* frameImage_;
  std::shared_ptr<OpenCVInterface> input_device_;
  std::unique_ptr<TargetImageProvider> target_provider_;

 public slots:
  void someSlot();
};

#endif  // MAINWINDOW_H
