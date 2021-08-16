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
public:
	const QVector<QVector3D>& data() const;
	void Load(const QString& pathToObj);
	void draw(const QMatrix4x4& world, const QMatrix4x4& proj, const QMatrix4x4& cam);
	void click();
	void unclick();
	void move(const QVector3D& distance);
	void rotate(float angle, const QVector3D& newAngle);
	void scale(const QVector3D& newSize);
public:
	QString path;
	QMatrix4x4 modelTransform;
private:
	void update(const QMatrix4x4& world, const QMatrix4x4& proj, const QMatrix4x4& cam);
	void transform();
private:
	Loader loader;
	Shader shader;
	QOpenGLVertexArrayObject vao;
	QOpenGLBuffer vbo;
	QOpenGLBuffer* index = nullptr;
	QVector3D pos, angle, size;
	float rotateAngle;
};

