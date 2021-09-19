#include "object.h"

Object::Object(const Object& b)
{
    qDebug() << "Copy";
    Load(b.path);
    pos = b.pos;
    angle = b.angle;
    size = b.size;
    rotateAngle = b.rotateAngle;
}

Object &Object::operator=(const Object & b)
{
    if (this == &b) {
        return *this;
    }
    pos = b.pos;
    angle = b.angle;
    size = b.size;
    rotateAngle = b.rotateAngle;
    path = b.path;
    return *this;
}

const QVector<QVector3D>& Object::data() const
{
    return loader.getVertexList();
}

const QVector<unsigned int> &Object::dataTrinagles() const
{
    return loader.getTriangleList();
}

void Object::Load(const QString& pathToObj)
{
    //
    path = pathToObj;
    //load vertex and faces
    loader.load(pathToObj);
    //
    interShader = new Shader();
    interShader->init(":/shaders/shaders/1.vs",":/shaders/shaders/2.fs");
    outLineShader = new Shader();
    outLineShader->init(":/shaders/shaders/1.vs",":/shaders/shaders/3.fs");
    //
    vao.create();
    vbo.create();
    index = new QOpenGLBuffer(QOpenGLBuffer::Type::IndexBuffer);
    index->create();
    vao.bind();
    //Setup our vertex buffer object.  
    vbo.bind();
    vbo.allocate(loader.getVertexList().constData(), sizeof(QVector3D) * loader.getVertexList().count());
    //Store the vertex attribute bindings for the program.
    if (!loader.getTriangleList().empty()) {
        index->bind();
        index->allocate(loader.getTriangleList().constData(), loader.getTriangleList().count() * sizeof(GLuint));
    }
    QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
    f->glEnableVertexAttribArray(0);
    //f->glEnableVertexAttribArray(1); if use light and normal
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);//3 * sizeof(GLfloat)
    //f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),reinterpret_cast<void*>(3 * sizeof(GLfloat))); -- normal
    vao.release();
    size = QVector3D(1, 1, 1);
}

void Object::move(const QVector3D& distance)
{
    pos += distance;
}

void Object::rotate(float angleR,const QVector3D& trns)
{
    angle = trns;
    rotateAngle = angleR;
}

void Object::scale(const QVector3D& newSize)
{
    size = newSize;
}

void Object::setPos(const QVector2D &newPos)
{
    pos = QVector3D(newPos,pos.z());
}

const QMatrix4x4& Object::getModelTransform()
{
    return modelTransform;
}

QVector<QVector3D> Object::getAffine() const
{
    QVector<QVector3D> affine;
    affine.push_back(pos);
    affine.push_back(angle);
    affine.push_back(size);
    affine.push_back(QVector3D(rotateAngle,0.f,0.f));
    return affine;
}

void Object::drawIt()
{
    if (loader.getTriangleList().empty()) {
        glDrawArrays(GL_TRIANGLES, 0, loader.getVertexList().count());
    }
    else {// Draw using indices
        glDrawElements(GL_TRIANGLES, loader.getTriangleList().count(), GL_UNSIGNED_INT, nullptr);
    }
}

void Object::update(Shader *shader, const QMatrix4x4& world, const QMatrix4x4& proj, const QMatrix4x4& cam)
{
    transform();
    shader->bind();
    shader->setValue("proj", proj);
    shader->setValue("mv", cam * world * modelTransform);//modelTransform
    QMatrix3x3 normalMatrix = world.normalMatrix(); //for light world
    shader->setValue("norm", normalMatrix);
    //shader.setValue("pick", QVector3D(0, 0, 1));
    shader->release();
}

void Object::transform()
{
    modelTransform.setToIdentity();
    modelTransform.translate(pos);
    modelTransform.rotate(rotateAngle, angle);
    modelTransform.scale(size);
}

void Object::draw(const QMatrix4x4& world, const QMatrix4x4& proj, const QMatrix4x4& cam, bool outLine)
{
    if(outLine && isClicked){
        {//draw outLine
            glDisable(GL_DEPTH_TEST);
            scale(size + QVector3D(0.02f,0.02f,0.02f));
            update(outLineShader,world, proj, cam);
            QOpenGLVertexArrayObject::Binder vaoBinder(&vao);
            outLineShader->bind();
            outLineShader->setValue("pick", QVector3D(1, 0, 0));
            drawIt();
            outLineShader->release();
            scale(size - QVector3D(0.02f,0.02f,0.02f));
        }
        {// draw object
            glEnable(GL_DEPTH_TEST);
            glDepthMask(GL_TRUE);
            update(outLineShader,world, proj, cam);
            QOpenGLVertexArrayObject::Binder vaoBinder(&vao);
            outLineShader->bind();
            outLineShader->setValue("def", QVector3D(1, 1, 1));
            drawIt();
            outLineShader->release();
        }
    }else{
        update(interShader,world, proj, cam);
        QOpenGLVertexArrayObject::Binder vaoBinder(&vao);
        interShader->bind();
        drawIt();
        interShader->release();
    }
}

void Object::click()
{
    isClicked = true;
    interShader->bind();
    interShader->setValue("pick", QVector3D(0, 0, 0));
    interShader->release();
}

void Object::unclick()
{
    isClicked = false;
    interShader->bind();
    interShader->setValue("def", QVector3D(1, 1, 1));
    interShader->release();
}
