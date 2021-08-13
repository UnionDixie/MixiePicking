#pragma once

#include <QString>
#include <QVector>
#include <QVector3D>

class Loader
{
public:
	Loader() = default;
	~Loader() = default;
	void load(const QString& path);
	const QVector<QVector3D>& getVertexList();
	const QVector<unsigned int>& getTriangleList();
private:
	void add(const QString& row);
private:
	QVector<QVector3D> vertexList;
	QVector<unsigned int> trianleList;
};

