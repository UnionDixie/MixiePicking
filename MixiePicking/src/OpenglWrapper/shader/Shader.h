#pragma once

#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QMatrix4x4>

class Shader
{
public:
	Shader() = default;
	~Shader() = default;
	void init();
	void bind();
	template<typename T>
	void setValue(QString id,const T& value);
	void release();
	void destroy();
public:
	bool isDestroy;
private:
	QOpenGLShaderProgram* m_program = nullptr;

	int m_projMatrixLoc = 0;
	int m_mvMatrixLoc = 0;
	int m_normalMatrixLoc = 0;
	int m_lightPosLoc = 0;
	int m_outLineLoc = 0;

};

//added .inl
template<typename T>
inline void Shader::setValue(QString id, const T& value)
{
	if (id == "proj")
		m_program->setUniformValue(m_projMatrixLoc, value);
	else if (id == "mv")
		m_program->setUniformValue(m_mvMatrixLoc, value);
	else if (id == "norm")
		m_program->setUniformValue(m_normalMatrixLoc, value);
	else if (id == "pick")
		m_program->setUniformValue(m_outLineLoc, value);
	else 
		m_program->setUniformValue(m_outLineLoc, value);
}
