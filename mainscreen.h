#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QQuickView>
#include <QQuickItem>

#include <opencvhandler.h>

class MainScreen : public QQuickView
{
    Q_OBJECT
public:
    MainScreen();

private:
    QObject * frameImage;
    std::unique_ptr<OpenCVHandler> input_device;

public slots:
    void someSlot();
};

#endif // MAINWINDOW_H
