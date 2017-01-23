#include "mainwindow.h"
#include <QApplication>
#include <QPainter>
#include <QBitmap>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    br::Context::initialize(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
