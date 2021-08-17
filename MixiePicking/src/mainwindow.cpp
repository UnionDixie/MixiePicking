#include "mainwindow.h"

#include "OpenglWrapper/window.h"
#include <QMenuBar>
#include <QMenu>
#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QMenuBar *menuBar = new QMenuBar;
    QMenu *menuWindow = menuBar->addMenu("File");
    QAction *addNew = new QAction(menuWindow);
    addNew->setText(tr("Open"));
    connect(addNew, &QAction::triggered,this,&MainWindow::test);
    menuWindow->addAction(addNew);
    setMenuBar(menuBar);

    if (!centralWidget())
        setCentralWidget(new Window(this));
}

void MainWindow::test()
{
    qDebug() << "Open";
    const QString fileName = QFileDialog::getOpenFileName(this);
    //if (!fileName.isEmpty())
        //openFile(fileName);
}

void MainWindow::closeWindow()
{
    close();
}
