#include "objmesh.h"

ObjMesh::ObjMesh(GLenum mode) :
    _mode(mode)
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
