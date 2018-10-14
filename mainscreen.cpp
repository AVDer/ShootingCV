#include "mainscreen.h"

MainScreen::MainScreen()
{
    setSource(QUrl(QStringLiteral("qrc:/main.qml")));

    QObject *qmlRoot = rootObject();

    QObject * frameImage = qmlRoot->findChild<QObject*>("frameImage");
    if (frameImage) {
        frameImage->setProperty("source", "qrc:/target.png");
    }
}
