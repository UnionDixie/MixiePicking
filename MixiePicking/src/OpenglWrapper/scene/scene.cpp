#include "scene.h"

#include <QFile>
#include <QTextStream>

void Scene::saveScene(const std::vector<Object> &data, const QString &filename)
{
   QFile file(filename);
   if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
          qDebug() << "ErrorOut";

   QTextStream out(&file);

   for(auto& it : data){
       auto transfoms =  it.getAffine();
       out <<"Object\n" << it.path << "\n";
       for(const auto& affine : transfoms){
           float x = affine.x(), y = affine.y(), z = affine.z();
           out << x << " " << y << " " << z << "\n";
       }
   }
}

std::vector<Object> &Scene::loadScene(const QString &filename)
{
   QFile file(filename);
   if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
           qDebug() << "ErrorIn";

   while (!file.atEnd()) {
       QByteArray line = file.readLine();
       QString row(line);

       if(row == "Object\n"){//5
           line = file.readLine();
           QString path(line);
           line = file.readLine();
           QString pos(line);
           line = file.readLine();
           QString angle(line);
           line = file.readLine();
           QString size(line);
           line = file.readLine();
           QString rotAngle(line);

           auto strToVec3 = [](const QString& str) {
               auto values = str.split(" ");//1.0 2 3
               auto x = values[0].toDouble(), y = values[1].toDouble(), z = values[2].toDouble();
               return QVector3D(x, y, z);
           };

           Object tmp;
           path.remove(path.size() - 1,1);
           tmp.Load(path);
           tmp.move(strToVec3(pos));
           tmp.scale(strToVec3(size));
           tmp.rotate(strToVec3(rotAngle).x(),strToVec3(angle));

           objects.push_back(tmp);
       }
   }
    return objects;
}
