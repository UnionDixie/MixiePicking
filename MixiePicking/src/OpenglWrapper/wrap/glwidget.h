#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QListWidgetItem>

#include "../../picker/picker.h"
#include "../scene/scene.h"

class Object;

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    GLWidget(QWidget *parent = nullptr);
    ~GLWidget() = default;
    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;
public slots:
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);
    void setScale(int resize);
    void setMoveX(int moveX);
    void setMoveY(int moveY);
    void openFile(const QString& fileName);
    void listItemClicked(QListWidgetItem* item);
    void saveScene();
signals:
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);
    void scaleChanged(int resize);
    void moveXChanged(int moveX);
    void moveYChanged(int moveY);
    void addItemToList(const QString& name);
    void setActiveItemList(const QString& name);
protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent * event) override;
    void wheelEvent(QWheelEvent * event) override;
private:
    int x1, y1, x2, y2;
private:
    Scene scene;
    int width, height;
    Picker picker;
    int xRot = 0;
    int yRot = 0;
    int zRot = 0;
    QPoint lastPos;
    std::vector<Object> objects;
    std::vector<Object*> pickObjects;
    QMatrix4x4 proj;
    QMatrix4x4 camera;
    QMatrix4x4 world;
};

#endif // GLWIDGET_H
