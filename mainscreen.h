#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QQuickItem>
#include <QQuickView>

#include <opencvhandler.h>
#include <targetimageprovider.h>

class MainScreen : public QQuickView {
  Q_OBJECT
 public:
  MainScreen();

 private:
  QObject* frameImage;
  std::unique_ptr<OpenCVHandler> input_device;
  std::unique_ptr<TargetImageProvider> target_provider;

 public slots:
  void someSlot();
};

#endif  // MAINWINDOW_H
