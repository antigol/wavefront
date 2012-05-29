#include "linepath.h"
#define MAXSIZE int(1024 * sizeof (QVector3D))

LinePath::LinePath()
{
}

void LinePath::addPoint(const QVector3D &p)
{
    _vertices.append(p);
}

void LinePath::clear()
{
    _vertices.clear();
}

void LinePath::initializeGL(const QGLContext *context)
{
    _program = new QGLShaderProgram(context, this);
    _program->addShaderFromSourceFile(QGLShader::Vertex, ":/files/linepath.vert");
    _program->addShaderFromSourceFile(QGLShader::Fragment, ":/files/linepath.frag");
    if (!_program->link())
        qDebug() << _program->log();

    vertexLocation = _program->attributeLocation("vertex");
    modelLocation = _program->uniformLocation("matrixm");
    viewLocation = _program->uniformLocation("matrixv");
    projectionLocation = _program->uniformLocation("matrixp");
    color1Location = _program->uniformLocation("color1");
    color2Location = _program->uniformLocation("color2");
    stepLocation = _program->uniformLocation("step");

    setProjection(QMatrix4x4());
    setView(QMatrix4x4());
    setModel(QMatrix4x4());
}

void LinePath::drawGL()
{
    _program->bind();
    _program->enableAttributeArray(vertexLocation);

    ///!!  TODO : faire du vbo !
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < _vertices.size(); ++i) {
        glVertex3f(_vertices[i].x(), _vertices[i].y(), _vertices[i].z());
    }
    glEnd();

    _program->disableAttributeArray(vertexLocation);
    _program->release();
}

void LinePath::setModel(const QMatrix4x4 &m)
{
    _program->bind();
    _program->setUniformValue(modelLocation, m);
    _program->release();
}

void LinePath::setView(const QMatrix4x4 &v)
{
    _program->bind();
    _program->setUniformValue(viewLocation, v);
    _program->release();
}

void LinePath::setProjection(const QMatrix4x4 &p)
{
    _program->bind();
    _program->setUniformValue(projectionLocation, p);
    _program->release();
}

void LinePath::setColor(const QColor &color1, const QColor &color2, GLfloat step)
{
    _program->bind();
    _program->setUniformValue(color1Location, color1);
    _program->setUniformValue(color2Location, color2);
    _program->setUniformValue(stepLocation, step);
    _program->release();
}
