#ifndef LINEPATH_H
#define LINEPATH_H

#include <QVector3D>
#include <QList>
#include <QtOpenGL/QGLBuffer>

class LinePath
{
public:
    LinePath();

    void addPoint(const QVector3D &p);
    void clear();

    void initializeGL();
    void drawGL();

public:
    QList<QGLBuffer> _vbolist;
    int _size;
    QVector3D _last;
};

#endif // LINEPATH_H
