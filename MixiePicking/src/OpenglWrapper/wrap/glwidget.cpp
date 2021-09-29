#include "glwidget.h"

#include <QMouseEvent>
#include <QCoreApplication>

#include "../object/object.h"

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
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

void GLWidget::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != xRot) {
        xRot = angle;
        emit xRotationChanged(angle);
        update();
    }
}

void GLWidget::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != yRot) {
        yRot = angle;
        emit yRotationChanged(angle);
        update();
    }
}

void GLWidget::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != zRot) {
        zRot = angle;
        emit zRotationChanged(angle);
        update();
    }
}

void GLWidget::setScale(int resize)
{
   qNormalizeAngle(resize);
   double size = (double)(resize * 2)/(360.f * 16.f);

   for (const auto& it : pickObjects) {
       if (it != nullptr) {
           it->scale(QVector3D(size, size, size));
       }
   }

   emit scaleChanged(resize);
   update();
}

double normMove(int move) {
    double norm = (double)(move) / 16.f;
    double res;
    if (norm <= 180) {
        res = -(1 - norm / 180);
    }
    else {
        res = norm / 360;
    }
    return res;
}

void GLWidget::setMoveX(int moveX)
{
    qNormalizeAngle(moveX);
    double res = normMove(moveX);
    for (const auto& it : pickObjects) {
        if (it != nullptr) {
            it->move(QVector3D(res / 10.f, 0, 0));
        }
    }
    emit moveXChanged(180 * 16);
    update();
}

void GLWidget::setMoveY(int moveY)
{
    qNormalizeAngle(moveY);
    double res = normMove(moveY);
    for (const auto& it : pickObjects) {
        if (it != nullptr) {
            it->move(QVector3D(0, res / 10.f, 0));
        }
    }
    emit moveYChanged(180 * 16);
    update();
}

void GLWidget::openFile(const QString &fileName)
{
    makeCurrent();
    qDebug() << "Open " << fileName;
    auto type =  fileName.split(".");
    if(type[1] == "scene"){
        objects = scene.loadScene(fileName);
        for(const auto& it : objects){
            emit addItemToList(it.clampName);
        }
    }else{
        Object tmp;
        tmp.Load(fileName);
        objects.push_back(tmp);
        emit addItemToList(tmp.clampName);
    }
}

void GLWidget::listItemClicked(QListWidgetItem *item)
{
    qDebug() << "Clicked " << item;
    for (auto& it : objects) {
        if (item->text() == it.clampName) {
            it.click();
            pickObjects.push_back(&it);
        }
    }
    update();
}

void GLWidget::saveScene()
{
    scene.saveScene(objects,"test.scene");
}

void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    qDebug() << (char*)(glGetString(GL_VERSION)) << "!\n";

    glClearColor(0, 0, 0, 1);

    camera.setToIdentity();
    camera.translate(0, 0, -7.0f);
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    world.setToIdentity();
    world.rotate(180.0f - (xRot / 16.0f), 1, 0, 0);
    world.rotate(yRot / 16.0f, 0, 1, 0);
    world.rotate(zRot / 16.0f, 0, 0, 1);

    qDebug() << "Draw " << objects.size();
    for (auto& it : objects) {
        it.draw(world, proj, camera, 1);//0 for inter or 1 for simple red outline
    }
}

void GLWidget::resizeGL(int w, int h)
{
    width = w;
    height = h;
    proj.setToIdentity();
    proj.perspective(60.0f, GLfloat(w) / h, 0.1f, 100.0f);
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "Pick\n";
    picker.width = width;
    picker.height = height;

    for (auto& it : objects) {
        it.unclick();
    }
    pickObjects = picker.checkClick(objects, event->pos().x(),event->pos().y(), proj, world, camera);
    for (const auto& it : pickObjects) {
        if (it != nullptr) {
            it->click();
        }
    }
    lastPos = event->pos();
    update();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
   int dx = event->pos().x() - lastPos.x();
   int dy = event->pos().y() - lastPos.y();

   if (!pickObjects.empty()) {
       for (const auto& it : pickObjects) {
           if (it != nullptr) {
               if (event->buttons() & Qt::LeftButton) {
                   setXRotation(xRot + 8 * dy);
                   setYRotation(yRot + 8 * dx);
               }
               else if (event->buttons() & Qt::RightButton) {
                   setXRotation(xRot + 8 * dy);
                   setZRotation(zRot + 8 * dx);
               }
           }
       }
   }else{
      for (auto& it : objects) {
          it.unclick();
      }
   }
   lastPos = event->pos();
   update();
}

void GLWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        x1 = event->pos().x();
        y1 = event->pos().y();
        qDebug() << "Movel:" ;
    }else if (event->buttons() & Qt::RightButton) {
        x2 = event->pos().x();
        y2 = event->pos().y();
        qDebug() << "Mover:" ;
        pickObjects = picker.checkArea(x1,y1,x2,y2,objects,proj, world, camera);
        for (auto& it : objects) {
            it.unclick();
        }
        qDebug() << pickObjects.size();
        for (const auto& it : pickObjects) {
            if (it != nullptr) {
                it->click();
            }
        }
    }
    update();
}
