#include "mainwindow.h"

#include <QMenuBar>
#include <QMenu>
#include <QShortcut>

#include "OpenglWrapper/window.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //Create GUI
    QMenuBar *menuBar = new QMenuBar;
    QMenu *menuWindow = menuBar->addMenu("File");
    QAction *loadNewFile = new QAction(menuWindow);
    loadNewFile->setText(tr("Open"));
    menuWindow->addAction(loadNewFile);
    setMenuBar(menuBar);
    //
    auto* window = new Window(this);
    //Signal/Slots connecting
    connect(loadNewFile, &QAction::triggered, window, &Window::openNewFile);
    QShortcut* openFileShortcut = new QShortcut(QKeySequence("Ctrl+O"), this);
    connect(openFileShortcut, &QShortcut::activated, window, &Window::openNewFile);
    //send context to window)
    if (!centralWidget())
        setCentralWidget(window);
}

void MainWindow::closeWindow()
{
    close();
}
