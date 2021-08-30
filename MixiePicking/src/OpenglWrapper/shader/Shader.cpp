#include "Shader.h"

Shader::~Shader()
{
    destroy();
}

void Shader::init(const QString &vertex, const QString &fragment)
{
    program = new QOpenGLShaderProgram;

    program->addShaderFromSourceFile(QOpenGLShader::Vertex, vertex);
    program->addShaderFromSourceFile(QOpenGLShader::Fragment, fragment);
    program->bindAttributeLocation("vertex", 0);
    program->bindAttributeLocation("normal", 1);
    program->link();

    bind();
    projMatrixLoc = program->uniformLocation("projMatrix");
    mvMatrixLoc = program->uniformLocation("mvMatrix");
    normalMatrixLoc = program->uniformLocation("normalMatrix");
    lightPosLoc = program->uniformLocation("lightPos");
    outLineLoc = program->uniformLocation("fastOutLine");

    // Light position is fixed.
    program->setUniformValue(lightPosLoc, QVector3D(0, 0, 70));
    program->setUniformValue(outLineLoc, QVector3D(1, 1, 1));

    release();
}

void Shader::bind()
{
    program->bind();
}

void Shader::release()
{
    program->release();
}

void Shader::destroy()
{
    delete program;
    program = nullptr;
}

