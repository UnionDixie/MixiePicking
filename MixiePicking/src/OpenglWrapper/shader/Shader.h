#pragma once

#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QMatrix4x4>

class Shader
{
public:
	Shader() = default;
	~Shader() = default;
    void init(const QString& vertex,const QString& fragment);
	void bind();
	void release();
	template<typename T>
	void setValue(QString id,const T& value);
	void destroy();
public:
	bool isDestroy;
private:
	QOpenGLShaderProgram* m_program = nullptr;
	int m_projMatrixLoc,
		m_mvMatrixLoc,
		m_normalMatrixLoc,
		m_lightPosLoc,
		m_outLineLoc;
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
    else if (id == "def")
		m_program->setUniformValue(m_outLineLoc, value);
}
