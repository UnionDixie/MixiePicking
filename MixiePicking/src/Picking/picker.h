#ifndef PICKER_H
#define PICKER_H

#include <QMouseEvent>
#include <qopengl.h>
#include <QVector>
#include <QVector3D>
#include <QMatrix4x4>
#include <functional>

class Object;

class Picker
{
public:
    using ptrPickObj = Object*;
    Picker() = default;
    ~Picker() = default;
public:
    void checkScence(std::vector<Object>& data, QMouseEvent* event, QMatrix4x4& proj, QMatrix4x4& world, QMatrix4x4& cam);
    ptrPickObj pickObj = nullptr;
private:
    bool isPick;
private:
    QVector3D getOrgDirRay(QMouseEvent *event, QMatrix4x4 &proj, QMatrix4x4 &world, QMatrix4x4 &cam,QVector3D& orgn);
    void rayCheker(std::vector<Object>& data, QMouseEvent *event, QMatrix4x4 &proj, QMatrix4x4 &world, QMatrix4x4 &cam,
                   std::function<bool(QVector3D origin, QVector3D dir, QVector3D v0, QVector3D v1, QVector3D v2, float* intersection)>);
    static bool rayIntersectsTriangle(QVector3D origin, QVector3D dir, QVector3D v0, QVector3D v1, QVector3D v2, float* intersection);
};

#endif // PICKER_H
