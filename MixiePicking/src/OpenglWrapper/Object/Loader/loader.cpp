#include "loader.h"

#include <QFile>

void Loader::load(const QString& path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        add(line);
    }
}

const QVector<QVector3D>& Loader::getVertexList() const
{
	return vertexList;
}

const QVector<unsigned int>& Loader::getTriangleList() const
{
	return triangleList;
}

void Loader::add(const QString& row)
{
    auto strToVec3 = [](const QString& str) {
        auto values = str.split(" ");//v 1.0 2 3
        auto x = values[1].toDouble(), y = values[2].toDouble(), z = values[3].toDouble();
        return QVector3D(x, y, z);
    };

    if (row.front() == 'v')
        vertexList.push_back(strToVec3(row));
    else if (row.front() == 'f') {
        auto values = row.split(" ");
        unsigned int f1 = values[1].toDouble(), f2 = values[2].toDouble(), f3 = values[3].toDouble();
        triangleList.push_back(f1 - 1);//in .obj file faces start with 1 but 
        triangleList.push_back(f2 - 1);//in openGL start with 0
        triangleList.push_back(f3 - 1);
    }
}
