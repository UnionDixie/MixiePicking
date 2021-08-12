#include "testobject.h"

#include <qmath.h>

TestObject::TestObject()
{
    //m_data.resize(2500 * 6);
    //m_data.resize(3 * 6);

    const GLfloat x1 = +0.06f;
    const GLfloat y1 = -0.14f;
    const GLfloat x2 = +0.14f;
    const GLfloat y2 = -0.06f;
    const GLfloat x3 = +0.08f;
    const GLfloat y3 = +0.00f;
    const GLfloat x4 = +0.30f;
    const GLfloat y4 = +0.22f;

    quad(x1, y1, x2, y2, y2, x2, y1, x1);
    quad(x3, y3, x4, y4, y4, x4, y3, x3);

    extrude(x1, y1, x2, y2);
    extrude(x2, y2, y2, x2);
    extrude(y2, x2, y1, x1);
    extrude(y1, x1, x1, y1);
    extrude(x3, y3, x4, y4);
    extrude(x4, y4, y4, x4);
    extrude(y4, x4, y3, x3);

    const int NumSectors = 100;

    for (int i = 0; i < NumSectors; ++i) {
        GLfloat angle = (i * 2 * M_PI) / NumSectors;
        GLfloat angleSin = qSin(angle);
        GLfloat angleCos = qCos(angle);
        const GLfloat x5 = 0.30f * angleSin;
        const GLfloat y5 = 0.30f * angleCos;
        const GLfloat x6 = 0.20f * angleSin;
        const GLfloat y6 = 0.20f * angleCos;

        angle = ((i + 1) * 2 * M_PI) / NumSectors;
        angleSin = qSin(angle);
        angleCos = qCos(angle);
        const GLfloat x7 = 0.20f * angleSin;
        const GLfloat y7 = 0.20f * angleCos;
        const GLfloat x8 = 0.30f * angleSin;
        const GLfloat y8 = 0.30f * angleCos;

        quad(x5, y5, x6, y6, x7, y7, x8, y8);

        extrude(x6, y6, x7, y7);
        extrude(x8, y8, x5, y5);
    }
}

QVector<QVector3D> &TestObject::getVertex()
{
   return vertex;
}

void TestObject::add(const QVector3D &v, const QVector3D &n)
{
    vertex.push_back(v);
    m_data.push_back(v.x());
    m_data.push_back(v.y());
    m_data.push_back(v.z());
    m_data.push_back(n.x());
    m_data.push_back(n.y());
    m_data.push_back(n.z());
    m_count += 6;
}

void TestObject::quad(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3, GLfloat x4, GLfloat y4)
{
    QVector3D n = QVector3D::normal(QVector3D(x4 - x1, y4 - y1, 0.0f), QVector3D(x2 - x1, y2 - y1, 0.0f));

    add(QVector3D(x1, y1, -0.05f), n);
    add(QVector3D(x4, y4, -0.05f), n);
    add(QVector3D(x2, y2, -0.05f), n);

    add(QVector3D(x3, y3, -0.05f), n);
    add(QVector3D(x2, y2, -0.05f), n);
    add(QVector3D(x4, y4, -0.05f), n);

    n = QVector3D::normal(QVector3D(x1 - x4, y1 - y4, 0.0f), QVector3D(x2 - x4, y2 - y4, 0.0f));

    add(QVector3D(x4, y4, 0.05f), n);
    add(QVector3D(x1, y1, 0.05f), n);
    add(QVector3D(x2, y2, 0.05f), n);

    add(QVector3D(x2, y2, 0.05f), n);
    add(QVector3D(x3, y3, 0.05f), n);
    add(QVector3D(x4, y4, 0.05f), n);
}

void TestObject::extrude(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2)
{
    QVector3D n = QVector3D::normal(QVector3D(0.0f, 0.0f, -0.1f), QVector3D(x2 - x1, y2 - y1, 0.0f));

    add(QVector3D(x1, y1, +0.05f), n);
    add(QVector3D(x1, y1, -0.05f), n);
    add(QVector3D(x2, y2, +0.05f), n);

    add(QVector3D(x2, y2, -0.05f), n);
    add(QVector3D(x2, y2, +0.05f), n);
    add(QVector3D(x1, y1, -0.05f), n);
}
