#include "window.h"

#include <QSlider>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QApplication>
#include <QFileDialog>
#include <QSpinBox>

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
    listOfObjects->setMaximumWidth(150);

    //https://doc.qt.io/qt-5/qtwidgets-widgets-spinboxes-example.html
    QSpinBox *zoomSpinBox = new QSpinBox;
    zoomSpinBox->setRange(0, 1000);
    zoomSpinBox->setSingleStep(10);
    zoomSpinBox->setSuffix("%");
    zoomSpinBox->setSpecialValueText(tr("Automatic"));
    zoomSpinBox->setValue(100);
    zoomSpinBox->setMaximumWidth(50);
    QSpinBox *zoomSpinBox2 = new QSpinBox;
    zoomSpinBox2->setRange(0, 1000);
    zoomSpinBox2->setSingleStep(10);
    zoomSpinBox2->setSuffix("%");
    zoomSpinBox2->setSpecialValueText(tr("Automatic"));
    zoomSpinBox2->setValue(105);
    zoomSpinBox2->setMaximumWidth(50);

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
    connect(glWidget, &GLWidget::setActiveItemList, this, &Window::setActiveItem);
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
    QVBoxLayout *mainLayout2 = new QVBoxLayout;
    mainLayout2->addWidget(zoomSpinBox);
    mainLayout2->addWidget(zoomSpinBox2);
    container->addLayout(mainLayout2);
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

void Window::setActiveItem(const QString &name)
{
    if(name.isEmpty())
        listOfObjects->clearSelection();
    else{
        auto items = listOfObjects->findItems(name,Qt::MatchExactly);
        for(const auto& it : items){
            it->setSelected(true);
        }
    }
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
