#include "window.h"

#include "wrap/glwidget.h"
#include "../mainwindow.h"
#include "../Picking/picker.h"

#include <QSlider>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QApplication>

Window::Window(MainWindow *mw) : mainWindow(mw)
{
    glWidget = new GlWidget;

    xSlider = createSlider();
    ySlider = createSlider();
    zSlider = createSlider();

    sSlider = createSlider();

    moveXSlider = createSlider();
    moveYSlider = createSlider();

    connect(xSlider, &QSlider::valueChanged, glWidget, &GlWidget::setXRotation);
    connect(glWidget, &GlWidget::xRotationChanged, xSlider, &QSlider::setValue);
    connect(ySlider, &QSlider::valueChanged, glWidget, &GlWidget::setYRotation);
    connect(glWidget, &GlWidget::yRotationChanged, ySlider, &QSlider::setValue);
    connect(zSlider, &QSlider::valueChanged, glWidget, &GlWidget::setZRotation);
    connect(glWidget, &GlWidget::zRotationChanged, zSlider, &QSlider::setValue);

    connect(sSlider, &QSlider::valueChanged, glWidget, &GlWidget::setScale);
    connect(glWidget, &GlWidget::scaleChanged, sSlider, &QSlider::setValue);
    //
    connect(moveXSlider, &QSlider::valueChanged, glWidget, &GlWidget::setMoveX);
    connect(glWidget, &GlWidget::moveXChanged, moveXSlider, &QSlider::setValue);
    //
    connect(moveYSlider, &QSlider::valueChanged, glWidget, &GlWidget::setMoveY);
    connect(glWidget, &GlWidget::moveYChanged, moveYSlider, &QSlider::setValue);
    //
    connect(this, &Window::closeWin, mw, &MainWindow::closeWindow);
    //
    connect(this,&Window::openFile,glWidget,&GlWidget::openFile);
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

    QWidget *w = new QWidget;
    w->setLayout(container);
    mainLayout->addWidget(w);

    setLayout(mainLayout);
    //default postinon for sliders
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
        emit closeWin();//close();
    else
        QWidget::keyPressEvent(event);
}

#include <QFileDialog>

void Window::openNewFile()
{
    const QString fileName = QFileDialog::getOpenFileName(this, ("Open File"),
                                                          nullptr, ("Object file (*.txt *.obj)"));
    qDebug() << "Open " << fileName;
    emit openFile(fileName);
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
