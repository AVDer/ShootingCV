#include "mainscreen.h"

MainScreen::MainScreen(): input_device(new OpenCVHandler)
{
    setSource(QUrl(QStringLiteral("qrc:/main.qml")));

    QObject *qmlRoot = rootObject();

    frameImage = qmlRoot->findChild<QObject*>("frameImage");
    if (frameImage) {
        connect(frameImage, SIGNAL(imageClick()), this, SLOT(someSlot()));
    }
}

void MainScreen::someSlot()
{
    auto image = input_device->get_frame();
    QImage qImage = QImage((const unsigned char*)(image.data), image.cols,image.rows, QImage::Format_RGB888).rgbSwapped();
    qImage.save("test.png");
    frameImage->setProperty("width", image.cols);
    frameImage->setProperty("height", image.rows);
    frameImage->setProperty("source", "file:test.png");
}
