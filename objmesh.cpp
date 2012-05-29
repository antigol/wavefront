#include "objmesh.h"

ObjMesh::ObjMesh(GLenum mode, const QString &objectName, const QString &groupName) :
    _mode(mode), _objectName(objectName), _groupName(groupName)
{
}

GLenum ObjMesh::mode() const
{
    return _mode;
}

void ObjMesh::addVertex(const QVector3D &v, const QVector3D &vn)
{
    _vertices << v << vn;
}
