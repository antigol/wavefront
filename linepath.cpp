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
        vbo.create();
        vbo.allocate(MAXSIZE);

        if (!_vbolist.isEmpty()) {
            vbo.write(_size, &_last, sizeof (QVector3D));
            _size += sizeof (QVector3D);
        }

        vbo.write(_size, &p, sizeof (QVector3D));
        _size += sizeof (QVector3D);
    }
}

void LinePath::clear()
{
    _vbolist.clear();
    _size = 0;
}

void LinePath::initializeGL()
{
}

void LinePath::drawGL()
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    for (int i = 0; i < _vbolist.size(); ++i) {
        _vbolist[i].bind();
        glDrawArrays(GL_LINE_STRIP, 0, (i+1 == _vbolist.size() ? _size : MAXSIZE) / sizeof (QVector3D));
        _vbolist[i].release();
    }
    glDisableClientState(GL_VERTEX_ARRAY);
}
