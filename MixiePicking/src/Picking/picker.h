#ifndef PICKER_H
#define PICKER_H

#include <QMouseEvent>
#include <qopengl.h>
#include <QList>
#include <QVector3D>
#include <QMatrix4x4>

class Picker
{
public:
    Picker();
    void getEvent(QList<QVector3D>& data,QMouseEvent *event,QMatrix4x4& proj,QMatrix4x4& world,QMatrix4x4& cam);
private:
    bool rayIntersectsTriangle(QVector3D origin, QVector3D dir, QVector3D v0, QVector3D v1, QVector3D v2, float* intersection);

private:
    QVector<QMatrix4x4> vMatrix;
    QVector4D eyeCoords;
};

#endif // PICKER_H
