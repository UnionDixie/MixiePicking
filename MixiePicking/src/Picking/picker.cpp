#include "picker.h"
#include "../MixiePicking/src/OpenglWrapper/Object/Object.h"

void Picker::checkScence(std::vector<Object>& data, QMouseEvent* event, QMatrix4x4& proj, QMatrix4x4& world, QMatrix4x4& cam)
{
    if (event->buttons() & Qt::LeftButton) {
        rayCheker(data, event, proj, world, cam, rayIntersectsTriangle);
    }
}

QVector3D Picker::getOrgDirRay(QMouseEvent *event, QMatrix4x4 &proj, QMatrix4x4 &world, QMatrix4x4 &cam, QVector3D &orgn)
{
    const int w = 400, h = 400;
    double mx = event->pos().x(), my = event->pos().y();
    QVector3D worldNear = QVector3D(float(mx), float(h - my), 0.0f).unproject(cam * world, proj, QRect(0,0,w,h));
    // Mouse world pos on far plane
    QVector3D worldFar = QVector3D(float(mx), float(h - my), 1.0f).unproject(cam * world, proj, QRect(0,0,w,h));
    QVector3D rayDir = (worldFar - worldNear).normalized();
    orgn = worldNear;
    return rayDir;
}

void Picker::rayCheker(std::vector<Object> &data, QMouseEvent *event, QMatrix4x4 &proj, QMatrix4x4 &world, QMatrix4x4 &cam,
                       std::function<bool (QVector3D, QVector3D, QVector3D, QVector3D, QVector3D, float *)> rayCast)
{
    
    isPick = false;
    for (auto& it : data) {
        const auto& vertex = it.data();
        //
        auto transfom = world * it.modelTransform;
        QVector3D worldNear;
        QVector3D rayDir = getOrgDirRay(event, proj, transfom, cam, worldNear);

        for (size_t i = 0; i < vertex.size() - 3; i += 3)
        {
            QVector3D f1 = QVector3D(vertex[i]);//a
            QVector3D f2 = QVector3D(vertex[i + 1]);//b
            QVector3D f3 = QVector3D(vertex[i + 2]);//c

            float currIntersectionPos;
            if (rayCast(worldNear, rayDir, f1, f2, f3, &currIntersectionPos))
            {
                qDebug() << "\tIntersection " << "Obj:" << it.path << ": Triangle "
                         << i << " intersected at ray pos " << currIntersectionPos;
                isPick = true;
                pickObj = &it;
            }
        }
    }
    if (isPick)
        pickObj->click();
    if (!isPick) {
        if (pickObj != nullptr)
            pickObj->unclick();
        pickObj = nullptr;
    }
}

// Moller–Trumbore intersection algorithm
bool Picker::rayIntersectsTriangle(QVector3D origin, QVector3D dir, QVector3D v0, QVector3D v1, QVector3D v2, float* intersection)
{
    // Triangle edges
    QVector3D e1(v1 - v0);
    QVector3D e2(v2 - v0);

    const float epsilon = 0.000001f;

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
