#include "mainwindow.h"

#include "OpenglWrapper/window.h"
#include <QMenuBar>
#include <QMenu>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QMenuBar *menuBar = new QMenuBar;
    QMenu *menuWindow = menuBar->addMenu("File");
    QAction *addNew = new QAction(menuWindow);
    addNew->setText(tr("Open"));
    auto windowPtr = new Window(this);
    connect(addNew, &QAction::triggered,windowPtr,&Window::openNewFile);
    menuWindow->addAction(addNew);
    setMenuBar(menuBar);

    if (!centralWidget())
        setCentralWidget(windowPtr);
}

void MainWindow::closeWindow()
{
    close();
}
