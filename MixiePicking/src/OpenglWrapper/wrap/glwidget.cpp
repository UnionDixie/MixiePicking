#include "glwidget.h"

#include <QMouseEvent>
#include <QOpenGLShaderProgram>
#include <QCoreApplication>
#include <cmath>

GlWidget::GlWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{ }

GlWidget::~GlWidget()
{
    cleanup();
}

QSize GlWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GlWidget::sizeHint() const
{
    return QSize(400, 400);
}

static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;
}

void GlWidget::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != m_xRot) {
        m_xRot = angle;
        emit xRotationChanged(angle);
        update();
    }
}

void GlWidget::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != m_yRot) {
        m_yRot = angle;
        emit yRotationChanged(angle);
        update();
    }
}

void GlWidget::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != m_zRot) {
        m_zRot = angle;
        emit zRotationChanged(angle);
        update();
    }
}

void GlWidget::cleanup() {}

void GlWidget::initializeGL()
{
    initializeOpenGLFunctions();

    qDebug() << (char*)(glGetString(GL_VERSION)) << "!\n";

    glClearColor(0, 0, 0, 1);

    // Our camera never changes in this example.
    m_camera.setToIdentity();
    m_camera.translate(0, 0, -7.0f);//!

    Object tmp;
    tmp.Load(QString(":/image/data/obj/4.txt"));
    objects.push_back(tmp);
    objects.push_back(tmp);
    objects.back().move(QVector3D(-0.5,-4,0.5));
}

void GlWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    m_world.setToIdentity();
    m_world.rotate(180.0f - (m_xRot / 16.0f), 1, 0, 0);
    m_world.rotate(m_yRot / 16.0f, 0, 1, 0);
    m_world.rotate(m_zRot / 16.0f, 0, 0, 1);

    for (auto& it : objects) {
        it.draw(m_world, m_proj, m_camera);
    }
    qDebug() << "Draw";
}

void GlWidget::resizeGL(int w, int h)
{
    width = w;
    height = h;
    m_proj.setToIdentity();
    m_proj.perspective(60.0f, GLfloat(w) / h, 0.1f, 100.0f);
}

void GlWidget::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "Pick\n";
    picker.w = width;
    picker.h = height;
    picker.checkScence(objects, event, m_proj, m_world, m_camera);
    m_lastPos = event->pos();
}

void GlWidget::mouseMoveEvent(QMouseEvent *event)
{
   int dx = event->pos().x() - m_lastPos.x();
   int dy = event->pos().y() - m_lastPos.y();
   if (picker.pickObj != nullptr) {
       if (event->buttons() & Qt::LeftButton) {
           setXRotation(m_xRot + 8 * dy);
           setYRotation(m_yRot + 8 * dx);
       }
       else if (event->buttons() & Qt::RightButton) {
           setXRotation(m_xRot + 8 * dy);
           setZRotation(m_zRot + 8 * dx);
       }
       else {

       }
   }
   
   m_lastPos = event->pos();
}
