#include "picker.h"

#include <QMap>

#include "../openglwrapper/object/object.h"

std::vector<Object*> Picker::checkClick(std::vector<Object>& data, int x, int y, QMatrix4x4& proj, QMatrix4x4& world, QMatrix4x4& cam)
{
    std::vector<Object*> pickObjects;
    for (auto& it : data) {
        bool intersect = false;
        const auto& vertex = it.data();
        const auto& index = it.dataTrinagles();
        //
        auto transfom = world * it.getModelTransform();
        QVector3D worldNear;
        QVector3D rayDir = getOrgDirRay(x, y, proj, transfom, cam, worldNear);
        //check triangles
        for (size_t i = 0; i <= vertex.size() - 3; i += 3)
        {
            QVector3D a(vertex[i]);
            QVector3D b(vertex[i + 1]);
            QVector3D c(vertex[i + 2]);

            float currIntersectionPos;
            if (rayIntersectsTriangle(worldNear, rayDir, a, b, c, &currIntersectionPos))
            {
                qDebug() << "\tIntersection " << "Obj:" << it.path << ": Triangle "
                         << i << " intersected at ray pos " << currIntersectionPos; 
                pickObjects.push_back(&it);
                intersect = true;
                break;
            }
        }
        if(intersect)
            continue;
        for (size_t i = 0; i <= index.size() - 3; i += 3)
        {
            QVector3D a(vertex[index[i]]);
            QVector3D b(vertex[index[i + 1]]);
            QVector3D c(vertex[index[i + 2]]);

            float currIntersectionPos;
            if (rayIntersectsTriangle(worldNear, rayDir, a, b, c, &currIntersectionPos))
            {
                qDebug() << "\tIntersectionIndx " << "Obj:" << it.path << ": Triangle "
                         << i << " intersected at ray pos " << currIntersectionPos;
                pickObjects.push_back(&it);
                break;
            }
        }
    }
    return pickObjects;
}

std::vector<Object*> Picker::checkArea(int x1, int y1, int x2, int y2, std::vector<Object> &data, QMatrix4x4 &proj, QMatrix4x4 &world, QMatrix4x4 &cam)
{ 
    auto startTime = std::chrono::steady_clock::now();
    qDebug() << x1 << " " << y1 << " " <<  x2 << " " << y2;
    std::vector<Object*> pickObjects;
    QMap<Object*,bool> isPickObj;
    for (size_t i = x1; i <= x2; i++)
    {
        for (size_t j = y1; j <= y2; j++)
        {
            auto tmp = checkClick(data, i, j, proj, world, cam);
            for(const auto& it : tmp){
                if(!isPickObj[it]){
                    isPickObj[it] = true;
                    pickObjects.push_back(it);
                }
            }
        }
    }
    auto endTime = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = endTime - startTime;
    qDebug() << "elapsed time: " << elapsed_seconds.count() << "s\n";
    qDebug() << "Pick area " << pickObjects.size();
    return pickObjects;
}

QVector3D Picker::getOrgDirRay(int x, int y, QMatrix4x4& proj, QMatrix4x4& world, QMatrix4x4& cam, QVector3D& orgn)
{
    QVector3D worldNear =
              QVector3D(float(x), float(height - y), 0.0f).unproject(cam * world, proj, QRect(0, 0, width, height));
    QVector3D worldFar =
              QVector3D(float(x), float(height - y), 1.0f).unproject(cam * world, proj, QRect(0, 0, width, height));
    QVector3D rayDir = (worldFar - worldNear).normalized();
    orgn = worldNear;
    return rayDir;
}

// Moller–Trumbore intersection algorithm
bool Picker::rayIntersectsTriangle(QVector3D origin, QVector3D dir, QVector3D v0, QVector3D v1, QVector3D v2, float* intersection)
{
    // Triangle edges
    QVector3D e1(v1 - v0);
    QVector3D e2(v2 - v0);

    constexpr float epsilon = 0.000001f;

    QVector3D P, Q;
    double t;

    // Calculate determinant
    P = QVector3D::crossProduct(dir,e2);
    double det = QVector3D::dotProduct(e1, P);
    // If determinant is (close to) zero, the ray lies in the plane of the triangle or parallel it's plane
    if ((det > -epsilon) && (det < epsilon))
        return 0;

    double invDet = 1.0f / det;

    // Distance from first vertex to ray origin
    QVector3D T = origin - v0;

    // Calculate u parameter
    double u = QVector3D::dotProduct(T, P) * invDet;
    // Intersection point lies outside of the triangle
    if ((u < 0.0f) || (u > 1.0f))
        return 0;

    //Prepare to test v parameter
    Q = QVector3D::crossProduct(T ,e1);

    // Calculate v parameter
    double v = QVector3D::dotProduct(dir, Q) * invDet;
    // Intersection point lies outside of the triangle
    if (v < 0.f || u + v  > 1.f) return 0;

    // Calculate t
    t = QVector3D::dotProduct(e2, Q) * invDet;

    if (t > epsilon)
    {
        // Triangle interesected
        if (intersection)
        {
            *intersection = t;
        }
        return true;
    }

    // No intersection
    return false;
}
