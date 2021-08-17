#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setApplicationName("MixiePicking");

    MainWindow mainWindow;
    mainWindow.show();

    return a.exec();
}
