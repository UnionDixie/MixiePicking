#include "window.h"

#include <QSlider>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QApplication>
#include <QFileDialog>

#include "wrap/glwidget.h"
#include "../mainwindow.h"

Window::Window(MainWindow *mw) : mainWindow(mw)
{
    //create stuff
    glWidget = new GLWidget;
    xSlider = createSlider();
    ySlider = createSlider();
    zSlider = createSlider();
    sSlider = createSlider();//scale slider
    moveXSlider = createSlider();
    moveYSlider = createSlider();
    listOfObjects = new QListWidget(this);
    //connectring action of sliders
    connect(xSlider, &QSlider::valueChanged, glWidget, &GLWidget::setXRotation);
    connect(glWidget, &GLWidget::xRotationChanged, xSlider, &QSlider::setValue);
    connect(ySlider, &QSlider::valueChanged, glWidget, &GLWidget::setYRotation);
    connect(glWidget, &GLWidget::yRotationChanged, ySlider, &QSlider::setValue);
    connect(zSlider, &QSlider::valueChanged, glWidget, &GLWidget::setZRotation);
    connect(glWidget, &GLWidget::zRotationChanged, zSlider, &QSlider::setValue);
    connect(sSlider, &QSlider::valueChanged, glWidget, &GLWidget::setScale);
    connect(glWidget, &GLWidget::scaleChanged, sSlider, &QSlider::setValue);
    connect(moveXSlider, &QSlider::valueChanged, glWidget, &GLWidget::setMoveX);
    connect(glWidget, &GLWidget::moveXChanged, moveXSlider, &QSlider::setValue);
    connect(moveYSlider, &QSlider::valueChanged, glWidget, &GLWidget::setMoveY);
    connect(glWidget, &GLWidget::moveYChanged, moveYSlider, &QSlider::setValue);
    //system event
    connect(this, &Window::closeWin, mw, &MainWindow::closeWindow);
    connect(this,&Window::openFile,glWidget,&GLWidget::openFile);
    connect(listOfObjects, &QListWidget::itemClicked, glWidget, &GLWidget::listItemClicked);
    connect(glWidget, &GLWidget::addItemToList, this, &Window::addItemOnList);
    connect(this,&Window::signalSaveScene,glWidget,&GLWidget::saveScene);
    //
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *container = new QHBoxLayout;
    container->addWidget(glWidget);
    container->addWidget(xSlider);
    container->addWidget(ySlider);
    container->addWidget(zSlider);
    container->addWidget(sSlider);
    container->addWidget(moveXSlider);
    container->addWidget(moveYSlider);
    container->addWidget(listOfObjects);
    //
    QWidget *middleLayout = new QWidget;
    middleLayout->setLayout(container);
    mainLayout->addWidget(middleLayout);
    setLayout(mainLayout);
    //set default value for sliders
    xSlider->setValue(15 * 16);
    ySlider->setValue(345 * 16);
    zSlider->setValue(0 * 16);
    sSlider->setValue(180 * 16);
    moveXSlider->setValue(180 * 16);
    moveYSlider->setValue(180 * 16);
}

void Window::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
        emit closeWin();
    else
        QWidget::keyPressEvent(event);
}

void Window::openNewFile()
{
    const QString pathToFile = QFileDialog::getOpenFileName(this, ("Open File"),
                                                          nullptr, ("Object file (*.txt *.obj *.scene)"));
    if(!pathToFile.isEmpty())
        emit openFile(pathToFile);
}

void Window::saveScene()
{
    emit signalSaveScene();
}

void Window::addItemOnList(const QString& newItem)
{
    listOfObjects->addItem(newItem);
}

QSlider *Window::createSlider()
{
    QSlider *slider = new QSlider(Qt::Vertical);
    slider->setRange(0, 360 * 16);
    slider->setSingleStep(16);
    slider->setPageStep(15 * 16);
    slider->setTickInterval(15 * 16);
    slider->setTickPosition(QSlider::TicksRight);
    return slider;
}
