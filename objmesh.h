#ifndef OBJMESH_H
#define OBJMESH_H

// définit une unité de dessin 3D
// un ObjMesh est draw en un appel de glDrawElements

// Il contient :
// o   une couleur
// o   un type de formes (GL_TRIANGLES, ...)

#include <QVector3D>
#include <QVector>
#include <QtOpenGL/qgl.h>

class ObjScene;

class ObjMesh
{
public:
    ObjMesh(GLenum mode, const QString &objectName = QString(), const QString &groupName = QString());
    GLenum mode() const;
    void addVertex(const QVector3D &v, const QVector3D &vn);

private:
    GLenum _mode;

    QVector<QVector3D> _vertices;

    QString _objectName;
    QString _groupName;

    friend class ObjScene;
};

#endif // OBJMESH_H
