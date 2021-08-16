#include "Object.h"

Object::Object(const Object& b)
{
    Load(b.path);
}

const QVector<QVector3D>& Object::data() const
{
    return loader.getVertexList();
}

void Object::Load(const QString& pathToObj)
{
    //
    path = pathToObj;
    //load vertex and faces
    loader.load(pathToObj);
    //
    shader.init();
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
    //f->glEnableVertexAttribArray(1);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);//3 * sizeof(GLfloat)
    //f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),reinterpret_cast<void*>(3 * sizeof(GLfloat))); -- normal
    vao.release();
}

void Object::move(const QVector3D& distance)
{
    pos += distance;
}

void Object::rotate(float angleR,const QVector3D& trns)
{
    modelTransform.setToIdentity();
    angle = trns;
    rotateAngle = angleR;
}

void Object::scale(const QVector3D& newSize)
{
    size = newSize;
}

void Object::update(const QMatrix4x4& world, const QMatrix4x4& proj, const QMatrix4x4& cam)
{
    transform();
    shader.bind();
    shader.setValue("proj", proj);
    shader.setValue("mv", cam * world * modelTransform);//modelTransform
    QMatrix3x3 normalMatrix = world.normalMatrix(); //for light world
    shader.setValue("norm", normalMatrix);
    //shader.setValue("pick", QVector3D(0, 0, 1));
    shader.release();
}

void Object::transform()
{
    modelTransform.setToIdentity();
    modelTransform.translate(pos);
    modelTransform.rotate(rotateAngle, angle);
    //modelTransform.scale(0.5, 0.5, 0.5);
}

void Object::draw(const QMatrix4x4& world, const QMatrix4x4& proj, const QMatrix4x4& cam)
{
    update(world, proj, cam);
    QOpenGLVertexArrayObject::Binder vaoBinder(&vao);
    shader.bind();    
    if (loader.getTriangleList().empty()) {
        glDrawArrays(GL_TRIANGLES, 0, loader.getVertexList().count());
    }
    else {
        // Draw cube geometry using indices
        glDrawElements(GL_TRIANGLES, loader.getTriangleList().count(), GL_UNSIGNED_INT, nullptr);
    }
    shader.release();
}

void Object::click()
{
    shader.bind();
    shader.setValue("pick", QVector3D(1, 0, 0));
    shader.release();
}

void Object::unclick()
{
    shader.bind();
    shader.setValue("def", QVector3D(0, 1, 0));
    shader.release();
}
