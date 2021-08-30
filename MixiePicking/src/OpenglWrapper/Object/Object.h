#pragma once

#include <QString>
#include <QMatrix4x4>
#include <QVector3D>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>

#include "Loader/Loader.h"
#include "../shader/Shader.h"

class Object
{
public:
	Object() = default;
	Object(const Object& b);
    Object &operator=(const Object&);
	~Object() = default;
public:
	//u get vertexes
	const QVector<QVector3D>& data() const;
	//u get faces
    const QVector<unsigned int>& dataTrinagles() const;
	void Load(const QString& pathToObj);
    void draw(const QMatrix4x4& world, const QMatrix4x4& proj, const QMatrix4x4& cam, bool outLine = false);
	void click();
	void unclick();
	void move(const QVector3D& distance);
	void rotate(float angle, const QVector3D& newAngle);
	void scale(const QVector3D& newSize);
    void setPos(const QVector2D& newPos);
	const QMatrix4x4& getModelTransform();
    QVector<QVector3D> getAffine() const;
public:
	QString path;
private:
	QMatrix4x4 modelTransform;
private:
    void drawIt();
    void update(Shader* shader,const QMatrix4x4& world, const QMatrix4x4& proj, const QMatrix4x4& cam);
	void transform();
private:
    bool isClicked = false;
	Loader loader;
    Shader* interShader;
    Shader* outLineShader;
	QOpenGLVertexArrayObject vao;
	QOpenGLBuffer vbo;
	QOpenGLBuffer* index = nullptr;
    QVector3D pos, angle, size;
    float rotateAngle = 0;
};

