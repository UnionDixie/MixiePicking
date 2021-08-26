#include "Loader.h"

#include <QFile>
#include <qopengl.h>

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
	return trianleList;
}

void Loader::add(const QString& row)
{
    auto strToV3 = [](const QString& str) {
        auto values = str.split(" ");
        auto x = values[1].toDouble(), y = values[2].toDouble(), z = values[3].toDouble();
        return QVector3D(x, y, z);
    };

    if (row.front() == 'v')
        vertexList.push_back(strToV3(row));
    else if (row.front() == 'f') {
        auto values = row.split(" ");
        unsigned int f1 = values[1].toDouble(), f2 = values[2].toDouble(), f3 = values[3].toDouble();
        trianleList.push_back(f1 - 1);
        trianleList.push_back(f2 - 1);
        trianleList.push_back(f3 - 1);
    }
}
