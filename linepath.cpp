#include "linepath.h"
#define MAXSIZE int(1024 * sizeof (QVector3D))

LinePath::LinePath() :
    _size(0)
{
}

void LinePath::addPoint(const QVector3D &p)
{
    if (_size < MAXSIZE && !_vbolist.isEmpty()) {
        _vbolist.last().write(_size, &p, sizeof (QVector3D));
        _last = p;
        _size += sizeof (QVector3D);
    } else {
        _size = 0;

        _vbolist << QGLBuffer(QGLBuffer::VertexBuffer);
        QGLBuffer &vbo = _vbolist.last();
        vbo.setUsagePattern(QGLBuffer::DynamicDraw);
        Q_ASSERT(vbo.create());
        vbo.allocate(MAXSIZE);

        if (_vbolist.size() > 1) {
            vbo.write(_size, &_last, sizeof (QVector3D));
            _size += sizeof (QVector3D);
        }

        vbo.write(_size, &p, sizeof (QVector3D));
        _size += sizeof (QVector3D);
    }

    qDebug() << _vbolist.size() << _size;
}

void LinePath::clear()
{
    _vbolist.clear();
    _size = 0;
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

    for (int i = 0; i < _vbolist.size(); ++i) {
        Q_ASSERT(_vbolist[i].bind());

        _program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3);
        glDrawArrays(GL_LINE_STRIP, 0, (i+1 == _vbolist.size() ? _size : MAXSIZE) / sizeof (QVector3D));

        _vbolist[i].release();
    }

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
