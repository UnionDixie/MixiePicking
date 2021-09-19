//
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
	const QVector<QVector3D>& getVertexList() const;
	const QVector<unsigned int>& getTriangleList() const;
private:
	void add(const QString& row);
private:
	QVector<QVector3D> vertexList;
	QVector<unsigned int> triangleList;
};

