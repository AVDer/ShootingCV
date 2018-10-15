#include "main_screen.h"

#include "target_image_provider.h"

MainScreen::MainScreen()
    : input_device_(new OpenCVInterface), target_provider_(new TargetImageProvider()) {
  target_provider_->setOpenCVInterface(input_device_);

  setSource(QUrl(QStringLiteral("qrc:/main.qml")));

  QObject *qmlRoot = rootObject();
  QQmlEngine *engine = this->engine();
  engine->addImageProvider(QLatin1String("target"), new TargetImageProvider);

  frameImage_ = qmlRoot->findChild<QObject *>("frameImage");
  if (frameImage_) {
    connect(frameImage_, SIGNAL(imageClick()), this, SLOT(someSlot()));
  }
}

void MainScreen::someSlot() {
  frameImage_->setProperty("source", "");
  frameImage_->setProperty("source", "image://target/sample");
}
