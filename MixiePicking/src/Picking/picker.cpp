#include "picker.h"

Picker::Picker()
{

}

void Picker::getEvent(QList<QVector3D> &data, QMouseEvent *event, QMatrix4x4 &proj, QMatrix4x4 &world, QMatrix4x4 &cam)
{
    if (event->buttons() & Qt::LeftButton) {
      vMatrix.clear();
      vMatrix.push_back(proj);
      vMatrix.push_back(cam);
      vMatrix.push_back(world);

      const int w = 400, h = 400;
      double mx = event->position().x(), my = event->position().y();

      QVector3D worldNear = QVector3D(float(mx), float(h - my), 0.0f).unproject(cam * world, proj, QRect(0,0,400,400));
      // Mouse world pos on far plane
      QVector3D worldFar = QVector3D(float(mx), float(h - my), 1.0f).unproject(cam * world, proj, QRect(0,0,400,400));
      QVector3D rayDir = (worldFar - worldNear).normalized();

      for(int i = 0;i <= data.size() - 3;i+=3){
         QVector3D f1 = QVector3D(data[i]);//a
         QVector3D f2 = QVector3D(data[i+1]);//b
         QVector3D f3 = QVector3D(data[i+2]);//c

         float currIntersectionPos;
         if (rayIntersectsTriangle(worldNear, rayDir, f1, f2, f3, &currIntersectionPos))
         {
             qDebug() << "\tIntersection " << "numIntersections" << ": Triangle "
                      << i << " intersected at ray pos " << currIntersectionPos;
         }
      }
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
    //float i;
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
