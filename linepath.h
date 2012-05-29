#ifndef LINEPATH_H
#define LINEPATH_H

#include <QObject>
#include <QVector3D>
#include <QList>
#include <QtOpenGL/QGLBuffer>
#include <QtOpenGL/QGLShaderProgram>

class LinePath : public QObject
{
public:
    LinePath();

    void addPoint(const QVector3D &p);
    void clear();

    void initializeGL(const QGLContext *context);
    void drawGL();

    void setModel(const QMatrix4x4 &m);
    void setView(const QMatrix4x4 &v);
    void setProjection(const QMatrix4x4 &p);
    void setColor(const QColor &color1, const QColor &color2, GLfloat step);

public:
    QList<QGLBuffer> _vbolist;
    int _size;
    QVector3D _last;

    QGLShaderProgram *_program;

    int vertexLocation;
    int modelLocation;
    int viewLocation;
    int projectionLocation;
    int color1Location;
    int color2Location;
    int stepLocation;
};

#endif // LINEPATH_H
