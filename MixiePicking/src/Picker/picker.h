#ifndef PICKER_H
#define PICKER_H

#include <QVector3D>
#include <QMatrix4x4>
#include <chrono>
#include <vector>

class Object;

class Picker
{
public:
    Picker() = default;
    ~Picker() = default;
public:
    std::vector<Object*> checkClick(std::vector<Object>& data, int x, int y,
                                    QMatrix4x4& proj, QMatrix4x4& world, QMatrix4x4& cam);
    std::vector<Object*> checkArea(int x1, int y1, int x2, int y2,
                                   std::vector<Object>& data, QMatrix4x4& proj, QMatrix4x4& world, QMatrix4x4& cam);
public:
    int width, height;
private:
    QVector3D getOrgDirRay(int x, int y, 
                           QMatrix4x4& proj, QMatrix4x4& world, QMatrix4x4& cam, QVector3D& orgn);
    bool rayIntersectsTriangle(QVector3D origin, QVector3D dir,
                               QVector3D v0, QVector3D v1, QVector3D v2, float* intersection);
};

#endif // PICKER_H
