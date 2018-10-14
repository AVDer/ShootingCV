#include <QApplication>

#include "mainscreen.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainScreen w;
    w.show();

    return app.exec();
}
