#include "mainwindow.h"

#include "OpenglWrapper/window.h"
#include <QMenuBar>
#include <QMenu>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QMenuBar *menuBar = new QMenuBar;
    QMenu *menuWindow = menuBar->addMenu(tr("File"));
    QAction *addNew = new QAction(menuWindow);
    addNew->setText(tr("Open"));
    menuWindow->addAction(addNew);
    setMenuBar(menuBar);

    if (!centralWidget())
        setCentralWidget(new Window(this));

}

MainWindow::~MainWindow()
{
}

