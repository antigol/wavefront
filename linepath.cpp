#include "linepath.h"
#define PROGRAM_VERTEX_ATTRIBUTE 0
#define MAXCOUNT 1024

LinePath::LinePath()
{
}

void LinePath::addPoint(const QVector3D &p, float color)
{
    _vertices << QVector4D(p, color);

    if (_vertices.size() >= MAXCOUNT) {
        QGLBuffer vbo;
        vbo.create();
        vbo.bind();
        vbo.allocate(_vertices.constData(), MAXCOUNT * sizeof (QVector4D));
        vbo.release();

        _buffers << vbo;
        _vertices.clear();
    }
}

void LinePath::clear()
{
    _vertices.clear();
    _buffers.clear();
}

void LinePath::initializeGL(const QGLContext *context)
{
    _program = new QGLShaderProgram(context, this);
    _program->addShaderFromSourceFile(QGLShader::Vertex, ":/files/linepath.vert");
    _program->addShaderFromSourceFile(QGLShader::Fragment, ":/files/linepath.frag");
    _program->bindAttributeLocation("vertex", PROGRAM_VERTEX_ATTRIBUTE);
    if (!_program->link())
        qDebug() << _program->log();

    _modelLocation = _program->uniformLocation("matrixm");
    _viewLocation = _program->uniformLocation("matrixv");
    _projectionLocation = _program->uniformLocation("matrixp");
    _color1Location = _program->uniformLocation("color1");
    _color2Location = _program->uniformLocation("color2");

    setProjection(QMatrix4x4());
    setView(QMatrix4x4());
    setModel(QMatrix4x4());
}

void LinePath::drawGL()
{
    _program->bind();
    _program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);

    for (int i = 0; i < _buffers.size(); ++i) {
        _buffers[i].bind();
        _program->setAttributeBuffer(PROGRAM_VERTEX_ATTRIBUTE, GL_FLOAT, 0, 4);
        glDrawArrays(GL_LINE_STRIP, 0, MAXCOUNT);
        _buffers[i].release();
    }

    _program->setAttributeArray(PROGRAM_VERTEX_ATTRIBUTE, _vertices.constData());
    glDrawArrays(GL_LINE_STRIP, 0, _vertices.size());

    _program->disableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
    _program->release();
}

void LinePath::setModel(const QMatrix4x4 &m)
{
    _program->bind();
    _program->setUniformValue(_modelLocation, m);
    _program->release();
}

void LinePath::setView(const QMatrix4x4 &v)
{
    _program->bind();
    _program->setUniformValue(_viewLocation, v);
    _program->release();
}

void LinePath::setProjection(const QMatrix4x4 &p)
{
    _program->bind();
    _program->setUniformValue(_projectionLocation, p);
    _program->release();
}

void LinePath::setColor(const QColor &color1, const QColor &color2)
{
    _program->bind();
    _program->setUniformValue(_color1Location, color1);
    _program->setUniformValue(_color2Location, color2);
    _program->release();
}
