#ifndef TESTOBJECT_H
#define TESTOBJECT_H

#include <qopengl.h>
#include <QList>
#include <QVector3D>

class TestObject
{
public:
    TestObject();
    const GLfloat *constData() const { return m_data.constData(); }
    int count() const { return m_count; }
    int vertexCount() const { return m_count / 6; }
    QList<QVector3D>& getVertex();
private:
    void quad(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3, GLfloat x4, GLfloat y4);
    void extrude(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
    void add(const QVector3D &v, const QVector3D &n);

    QList<GLfloat> m_data;
    QList<QVector3D> data2;
    int m_count = 0;
};

#endif // TESTOBJECT_H
