#include "mainscreen.h"

#include "targetimageprovider.h"

MainScreen::MainScreen()
    : input_device(new OpenCVHandler), target_provider(new TargetImageProvider()) {
  setSource(QUrl(QStringLiteral("qrc:/main.qml")));

  QObject *qmlRoot = rootObject();
  QQmlEngine *engine = this->engine();
  engine->addImageProvider(QLatin1String("target"), new TargetImageProvider);

  frameImage = qmlRoot->findChild<QObject *>("frameImage");
  if (frameImage) {
    connect(frameImage, SIGNAL(imageClick()), this, SLOT(someSlot()));
  }
}

void MainScreen::someSlot() {
  // auto image = input_device->get_frame();
  auto image = input_device->get_sample_frame();
  frameImage->setProperty("width", image.cols);
  frameImage->setProperty("height", image.rows);
  target_provider->newOpenCVFrame(image);
  frameImage->setProperty("source", "");
  frameImage->setProperty("source", "image://target/target");
}
