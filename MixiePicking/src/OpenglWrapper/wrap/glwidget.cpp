#include "glwidget.h"

#include <QMouseEvent>
#include <QOpenGLShaderProgram>
#include <QCoreApplication>
#include <cmath>

GlWidget::GlWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{

}

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

void GlWidget::cleanup()
{
    if (simpleShader.isDestroy)
        return;
    makeCurrent();
    m_logoVbo.destroy();
    simpleShader.destroy();
    doneCurrent();
}



void GlWidget::initializeGL()
{
    initializeOpenGLFunctions();

    qDebug() << (char*)(glGetString(GL_VERSION)) << "!\n";

    glClearColor(0, 0, 0, 1);

    simpleShader.init();

    m_vao.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);

    //Setup our vertex buffer object.
    m_logoVbo.create();
    m_logoVbo.bind();
    m_logoVbo.allocate(testObj.constData(), testObj.count() * sizeof(GLfloat));

    //Store the vertex attribute bindings for the program.
    setupVertexAttribs();

    // Our camera never changes in this example.
    m_camera.setToIdentity();
    m_camera.translate(0, 0, -7.0f);//!

    //for (size_t i = 1; i < 3; i++)
    //{
    //    Object tmp;
    //    tmp.Load(QString("data/obj/%1.txt").arg(i));
    //    //objects.push_back(tmp);            
    //}
    
    Object tmp;
    tmp.Load(QString("data/obj/%1.txt").arg(4));
    objects.push_back(tmp);


}

void GlWidget::setupVertexAttribs()
{
    m_logoVbo.bind();
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glEnableVertexAttribArray(0);
    f->glEnableVertexAttribArray(1);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                             nullptr);
    f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                             reinterpret_cast<void *>(3 * sizeof(GLfloat)));
    m_logoVbo.release();
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

    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);

    simpleShader.bind();

    simpleShader.setValue("proj", m_proj);
    simpleShader.setValue("mv", m_camera * m_world);
    QMatrix3x3 normalMatrix = m_world.normalMatrix(); //for light
    simpleShader.setValue("norm", normalMatrix);

    //glDrawArrays(GL_TRIANGLES, 0, testObj.vertexCount());

    simpleShader.release();

    for (size_t i = 0; i < objects.size(); i++)
    {
        objects[i].draw(m_world, m_proj, m_camera);
    }

}

void GlWidget::resizeGL(int w, int h)
{
    m_proj.setToIdentity();
    m_proj.perspective(60.0f, GLfloat(w) / h, 0.1f, 100.0f);
}

void GlWidget::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "Pick\n";

    //picker.getEvent(testObj.getVertex(), event, m_proj, m_world, m_camera);


    picker.checkScence(objects, event, m_proj, m_world, m_camera);

    simpleShader.bind();
    if (picker.isPick)
        simpleShader.setValue("pick", QVector3D(1, 0, 0));
    else
        simpleShader.setValue("def", QVector3D(0, 1, 0));
    simpleShader.release();

    m_lastPos = event->pos();
}

void GlWidget::mouseMoveEvent(QMouseEvent *event)
{
   int dx = event->pos().x() - m_lastPos.x();
   int dy = event->pos().y() - m_lastPos.y();

   if (event->buttons() & Qt::LeftButton) {
       setXRotation(m_xRot + 8 * dy);
       setYRotation(m_yRot + 8 * dx);
   } else if (event->buttons() & Qt::RightButton) {
       setXRotation(m_xRot + 8 * dy);
       setZRotation(m_zRot + 8 * dx);
   }
   m_lastPos = event->pos();
}
