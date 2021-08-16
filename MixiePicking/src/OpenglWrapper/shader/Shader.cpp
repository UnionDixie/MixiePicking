#include "Shader.h"

void Shader::init()
{
    isDestroy = false;

    m_program = new QOpenGLShaderProgram;

    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "data/shaders/1.vs");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "data/shaders/2.fs");
    m_program->bindAttributeLocation("vertex", 0);
    m_program->bindAttributeLocation("normal", 1);
    m_program->link();

    bind();
    m_projMatrixLoc = m_program->uniformLocation("projMatrix");
    m_mvMatrixLoc = m_program->uniformLocation("mvMatrix");
    m_normalMatrixLoc = m_program->uniformLocation("normalMatrix");
    m_lightPosLoc = m_program->uniformLocation("lightPos");
    m_outLineLoc = m_program->uniformLocation("fastOutLine");

    // Light position is fixed.
    m_program->setUniformValue(m_lightPosLoc, QVector3D(0, 0, 70));
    m_program->setUniformValue(m_outLineLoc, QVector3D(0, 1, 0));

    release();
}

void Shader::bind()
{
    m_program->bind();
}

void Shader::release()
{
    m_program->release();
}

void Shader::destroy()
{
    isDestroy = true;
    delete m_program;
    m_program = nullptr;
}

