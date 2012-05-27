#ifndef OBJSCENE_H
#define OBJSCENE_H

#include <QString>
#include <QList>
#include <QVector3D>
#include <QtOpenGL/QGLShaderProgram>
#include <QtOpenGL/QGLBuffer>
#include "objmesh.h"

class ObjScene : public QObject
{
    Q_OBJECT

public:
    ObjScene(QObject *parent = 0);

    void addTriangleWithNormals(QVector3D v1, QVector3D vn1, QVector3D v2, QVector3D vn2, QVector3D v3, QVector3D vn3);
    void addQuadWithNormals(QVector3D v1, QVector3D vn1, QVector3D v2, QVector3D vn2, QVector3D v3, QVector3D vn3, QVector3D v4, QVector3D vn4);
    void addPolygonWithNormals(QList<QVector3D> v, QList<QVector3D> vn);

    // counterclockwise orientation
    void addTriangle(QVector3D v1, QVector3D v2, QVector3D v3);
    void addQuad(QVector3D v1, QVector3D v2, QVector3D v3, QVector3D v4);
    void addPolygon(QList<QVector3D> v);

    void clear();
    void initializeGL(const QGLContext *context);
    void drawGL();

    void setProjection(const QMatrix4x4 &p);
    void setModelView(const QMatrix4x4 &m);

    void setColor(const QColor &ambiant, const QColor &diffuse, const QColor &specular, GLfloat specularpower);
    void setLight(const QVector3D &lightPosition);

private:
    QList<ObjMesh> _elements;
    QGLShaderProgram *_program;
    QGLBuffer _vbo;

    int vertexLocation;
    int normalLocation;
    int projectionLocation;
    int modelviewLocation;

    int ambiantLocation;
    int diffuseLocation;
    int specularLocation;
    int specularpowerLocation;
    int lightLocation;
};

#endif // OBJSCENE_H
