#include "mainwindow.h"
#include <QSurfaceFormat>

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSurfaceFormat fmt;
    fmt.setDepthBufferSize(24);
    fmt.setSamples(4);
    fmt.setVersion(3, 2);
    fmt.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(fmt);
    MainWindow w;
    w.show();
    return a.exec();
}
