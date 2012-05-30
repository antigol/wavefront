#include "linepath.h"
#define MAXCOUNT 1024

LinePath::LinePath()
{
}

void LinePath::addPoint(const QVector3D &p)
{
    _vertices << p;

    if (_vertices.size() >= MAXCOUNT) {
        QGLBuffer vbo;
        vbo.create();
        vbo.bind();
        vbo.allocate(_vertices.constData(), MAXCOUNT * sizeof (QVector3D));
        vbo.release();

        _buffers << vbo;
        _vertices.clear();
    }

    qDebug() << _vertices.size() << _buffers.size();
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
    if (!_program->link())
        qDebug() << _program->log();

    _vertexLocation = _program->attributeLocation("vertex");
    _modelLocation = _program->uniformLocation("matrixm");
    _viewLocation = _program->uniformLocation("matrixv");
    _projectionLocation = _program->uniformLocation("matrixp");
    _color1Location = _program->uniformLocation("color1");
    _color2Location = _program->uniformLocation("color2");
    _stepLocation = _program->uniformLocation("step");

    setProjection(QMatrix4x4());
    setView(QMatrix4x4());
    setModel(QMatrix4x4());
}

void LinePath::drawGL()
{
    _program->bind();
    _program->enableAttributeArray(_vertexLocation);

    for (int i = 0; i < _buffers.size(); ++i) {
        _buffers[i].bind();
        _program->setAttributeBuffer(_vertexLocation, GL_FLOAT, 0, 3);
        glDrawArrays(GL_LINE_STRIP, 0, MAXCOUNT);
        _buffers[i].release();
    }

    _program->setAttributeArray(_vertexLocation, _vertices.constData());
    glDrawArrays(GL_LINE_STRIP, 0, _vertices.size());

    _program->disableAttributeArray(_vertexLocation);
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

void LinePath::setColor(const QColor &color1, const QColor &color2, GLfloat step)
{
    _program->bind();
    _program->setUniformValue(_color1Location, color1);
    _program->setUniformValue(_color2Location, color2);
    _program->setUniformValue(_stepLocation, step);
    _program->release();
}
