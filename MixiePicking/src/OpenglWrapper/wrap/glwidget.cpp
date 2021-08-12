#include "glwidget.h"

#include <QMouseEvent>
#include <QOpenGLShaderProgram>
#include <QCoreApplication>
#include <math.h>


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
    if (m_program == nullptr)
        return;
    makeCurrent();
    m_logoVbo.destroy();
    delete m_program;
    m_program = nullptr;
    doneCurrent();
}

static const char *vertexShaderSourceCore =
    "#version 150\n"
    "in vec4 vertex;\n"
    "in vec3 normal;\n"
    "out vec3 vert;\n"
    "out vec3 vertNormal;\n"
    "uniform mat4 projMatrix;\n"
    "uniform mat4 mvMatrix;\n"
    "uniform mat3 normalMatrix;\n"
    "void main() {\n"
    "   vert = vertex.xyz;\n"
    "   vertNormal = normalMatrix * normal;\n"
    "   gl_Position = projMatrix * mvMatrix * vertex;\n"
    "}\n";

static const char *fragmentShaderSourceCore =
    "#version 150\n"
    "in highp vec3 vert;\n"
    "in highp vec3 vertNormal;\n"
    "out highp vec4 fragColor;\n"
    "uniform highp vec3 lightPos;\n"
    "uniform highp vec3 fastOutLine;\n"
    "void main() {\n"
    "   highp vec3 L = normalize(lightPos - vert);\n"
    "   highp float NL = max(dot(normalize(vertNormal), L), 0.0);\n"
    "   highp vec3 color = vec3(0.39, 1.0, 0.0);\n"
    "   highp vec3 col = clamp(color * 0.2 + color * 0.8 * NL, 0.0, 1.0);\n"
    "   fragColor = vec4(col + fastOutLine, 1.0);\n"
    "}\n";

void GlWidget::initializeGL()
{
    initializeOpenGLFunctions();

    qDebug() << (char*)(glGetString(GL_VERSION)) << "!\n";


    glClearColor(0, 0, 0, 1);

    m_program = new QOpenGLShaderProgram;
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSourceCore);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSourceCore );
    m_program->bindAttributeLocation("vertex", 0);
    m_program->bindAttributeLocation("normal", 1);
    m_program->link();

    m_program->bind();
    m_projMatrixLoc = m_program->uniformLocation("projMatrix");
    m_mvMatrixLoc = m_program->uniformLocation("mvMatrix");
    m_normalMatrixLoc = m_program->uniformLocation("normalMatrix");
    m_lightPosLoc = m_program->uniformLocation("lightPos");
    m_outLineLoc = m_program->uniformLocation("fastOutLine");

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
    m_camera.translate(0, 0, -1.0f);//!

    // Light position is fixed.
    m_program->setUniformValue(m_lightPosLoc, QVector3D(0, 0, 70));

    m_program->setUniformValue(m_outLineLoc, QVector3D(0, 1, 0));

    m_program->release();
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
    m_program->bind();
    m_program->setUniformValue(m_projMatrixLoc, m_proj);
    m_program->setUniformValue(m_mvMatrixLoc, m_camera * m_world);
    QMatrix3x3 normalMatrix = m_world.normalMatrix(); //for light
    m_program->setUniformValue(m_normalMatrixLoc, normalMatrix);

    glDrawArrays(GL_TRIANGLES, 0, testObj.vertexCount());

    m_program->release();

}

void GlWidget::resizeGL(int w, int h)
{
    m_proj.setToIdentity();
    m_proj.perspective(45.0f, GLfloat(w) / h, 0.1f, 100.0f);//45.0f, GLfloat(w) / h, 0.01f, 100.0f
}

void GlWidget::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "Pick\n";
    picker.getEvent(testObj.getVertex(),event,m_proj,m_world,m_camera);
    m_program->bind();
    if(picker.isPick){
      m_program->setUniformValue(m_outLineLoc, QVector3D(1, 0, 0));
    }else{
       m_program->setUniformValue(m_outLineLoc, QVector3D(0, 1, 0));
    }
    m_program->release();

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