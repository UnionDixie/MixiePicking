#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QListWidgetItem>

#include "../MixiePicking/src/Picking/picker.h"
#include "../MixiePicking/src/obj/testobject.h"
#include "../MixiePicking/src/OpenglWrapper/shader/Shader.h"
#include "../MixiePicking/src/OpenglWrapper/Object/Object.h"

class GlWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    GlWidget(QWidget *parent = nullptr);
    ~GlWidget();

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

public slots:
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);
    void setScale(int resize);
    void setMoveX(int moveX);
    void setMoveY(int moveY);
    void cleanup();
    void openFile(const QString& fileName);
    void listItemClicked(QListWidgetItem* item);
signals:
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);
    void scaleChanged(int resize);
    void moveXChanged(int moveX);
    void moveYChanged(int moveY);
    void addItemToList(const QString& name);
protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
private:
    QMap<QString,std::vector<Object>::iterator> scenceTree;
    int width, height;
    Picker picker;
    int m_xRot = 0;
    int m_yRot = 0;
    int m_zRot = 0;
    QPoint m_lastPos;
    std::vector<Object> objects;
    std::vector<Object> userObjects;
    QMatrix4x4 m_proj;
    QMatrix4x4 m_camera;
    QMatrix4x4 m_world;
};

#endif // GLWIDGET_H
