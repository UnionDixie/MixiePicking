#pragma once

#include <QOpenGLShaderProgram>

class Shader
{
public:
	Shader() = default;
	~Shader();
public:
    void init(const QString& vertex,const QString& fragment);
	void bind();
	void release();
	template<typename T>
	void setValue(const QString& id,const T& value);
	void destroy();
private:
	QOpenGLShaderProgram* program = nullptr;
	int projMatrixLoc,
		mvMatrixLoc,
		normalMatrixLoc,
		lightPosLoc,
		outLineLoc;
};


//added .inl
template<typename T>
inline void Shader::setValue(const QString& id, const T& value)
{
	if (id == "proj")
		program->setUniformValue(projMatrixLoc, value);
	else if (id == "mv")
		program->setUniformValue(mvMatrixLoc, value);
	else if (id == "norm")
		program->setUniformValue(normalMatrixLoc, value);
	else if (id == "pick")
		program->setUniformValue(outLineLoc, value);
    else if (id == "def")
		program->setUniformValue(outLineLoc, value);
}
