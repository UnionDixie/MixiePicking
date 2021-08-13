#pragma once

#include <QString>
#include <QMatrix4x4>
#include <QVector3D>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>

#include "Loader/Loader.h"
#include "../MixiePicking/src/OpenglWrapper/shader/Shader.h"

class Object
{
public:
	Object() = default;

	Object(const Object& b);

	Object& operator=(const Object&) = default;

	~Object() = default;

	const QVector<QVector3D>& data();

	void Load(const QString& pathToObj);

	void move(const QVector3D& distance);
	void rotate(const QVector3D& newAngle);
	void scale(const QVector3D& newScale);

	void draw(const QMatrix4x4& world, const QMatrix4x4& proj, const QMatrix4x4& cam);

	QString path;
private:
	void update(const QMatrix4x4& world, const QMatrix4x4& proj, const QMatrix4x4& cam);
	void transform();

private:
	Loader loader;
	Shader shader;
	
	QOpenGLVertexArrayObject vao;
	QOpenGLBuffer vbo;
	QOpenGLBuffer* index;

	QVector3D pos, angle, sacle;
	QMatrix4x4 modelTransform;
};

