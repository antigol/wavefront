#include "objscene.h"
#define PROGRAM_VERTEX_ATTRIBUTE 0
#define PROGRAM_NORMAL_ATTRIBUTE 1

ObjScene::ObjScene(QObject *parent) :
    QObject(parent),
    _vbo(QGLBuffer::VertexBuffer)
{
}

void ObjScene::addTriangleWithNormals(QVector3D v1, QVector3D vn1, QVector3D v2, QVector3D vn2, QVector3D v3, QVector3D vn3)
{
    if (_elements.isEmpty()
            || _elements.last().mode() != GL_TRIANGLES
            || _elements.last()._objectName != _currentObject
            || _elements.last()._groupName != _currentGroup)
        _elements.append(ObjMesh(GL_TRIANGLES, _currentObject, _currentGroup));

    ObjMesh &mesh = _elements.last();
    mesh.addVertex(v1, vn1);
    mesh.addVertex(v2, vn2);
    mesh.addVertex(v3, vn3);
}

void ObjScene::addQuadWithNormals(QVector3D v1, QVector3D vn1, QVector3D v2, QVector3D vn2, QVector3D v3, QVector3D vn3, QVector3D v4, QVector3D vn4)
{
    if (_elements.isEmpty()
            || _elements.last().mode() != GL_QUADS
            || _elements.last()._objectName != _currentObject
            || _elements.last()._groupName != _currentGroup)
        _elements.append(ObjMesh(GL_QUADS, _currentObject, _currentGroup));

    ObjMesh &mesh = _elements.last();
    mesh.addVertex(v1, vn1);
    mesh.addVertex(v2, vn2);
    mesh.addVertex(v3, vn3);
    mesh.addVertex(v4, vn4);
}

void ObjScene::addPolygonWithNormals(QList<QVector3D> v, QList<QVector3D> vn)
{
    _elements.append(ObjMesh(GL_POLYGON, _currentObject, _currentGroup));
    ObjMesh &mesh = _elements.last();
    for (int i = 0; i < v.size(); ++i) {
        mesh.addVertex(v[i], vn[i]);
    }
}

void ObjScene::addTriangle(QVector3D v1, QVector3D v2, QVector3D v3)
{
    //     v1
    //
    // v2      v3

    QVector3D vn = QVector3D::normal(v1, v2, v3);
    addTriangleWithNormals(v1, vn, v2, vn, v3, vn);
}

void ObjScene::addQuad(QVector3D v1, QVector3D v2, QVector3D v3, QVector3D v4)
{
    // v1    v4
    //
    // v2    v3
    QVector3D vn = QVector3D::normal(v1, v2, v3);
    addQuadWithNormals(v1, vn, v2, vn, v3, vn, v4, vn);
}

void ObjScene::addPolygon(QList<QVector3D> v)
{
    QVector3D vn = QVector3D::normal(v[0], v[1], v[2]);

    _elements.append(ObjMesh(GL_POLYGON, _currentObject, _currentGroup));
    ObjMesh &mesh = _elements.last();
    for (int i = 0; i < v.size(); ++i) {
        mesh.addVertex(v[i], vn);
    }
}

void ObjScene::setCurrentObject(const QString &name)
{
    _currentObject = name;
}

void ObjScene::setCurrentGroup(const QString &name)
{
    _currentGroup = name;
}

void ObjScene::clear()
{
    _elements.clear();
    _currentObject.clear();
    _currentGroup.clear();
}

void ObjScene::initializeGL(const QGLContext *context)
{
    _program = new QGLShaderProgram(context, this);
    _program->addShaderFromSourceFile(QGLShader::Vertex, ":/files/objshader.vert");
    _program->addShaderFromSourceFile(QGLShader::Fragment, ":/files/objshader.frag");
    _program->bindAttributeLocation("vertex", PROGRAM_VERTEX_ATTRIBUTE);
    _program->bindAttributeLocation("normal", PROGRAM_NORMAL_ATTRIBUTE);
    if (!_program->link())
        qDebug() << _program->log();

    modelLocation = _program->uniformLocation("matrixm");
    viewLocation = _program->uniformLocation("matrixv");
    projectionLocation = _program->uniformLocation("matrixp");
    ambiantLocation = _program->uniformLocation("ambiant");
    diffuseLocation = _program->uniformLocation("diffuse");
    specularLocation = _program->uniformLocation("specular");
    hardnessLocation = _program->uniformLocation("hardness");
    lightLocation = _program->uniformLocation("light");

    setProjection(QMatrix4x4());
    setView(QMatrix4x4());
    setModel(QMatrix4x4());

    // tout mettre dans le VBO
    _vbo.create();
    _vbo.bind();
    int size = 0;
    for (int i = 0; i < _elements.size(); ++i) {
        size += _elements[i]._vertices.size();
    }
    qDebug() << float(size * sizeof (QVector3D)) / 1024 << "kilobyte";
    _vbo.allocate(size * sizeof (QVector3D));
    int offset = 0;
    for (int i = 0; i < _elements.size(); ++i) {
        _vbo.write(offset, _elements[i]._vertices.constData(), _elements[i]._vertices.size() * sizeof (QVector3D));
        offset += _elements[i]._vertices.size() * sizeof (QVector3D);
    }
    _vbo.release();
}

void ObjScene::drawGL(const QString &object, const QString &group)
{
    _program->bind();
    _program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
    _program->enableAttributeArray(PROGRAM_NORMAL_ATTRIBUTE);

    _vbo.bind();
    _program->setAttributeBuffer(PROGRAM_VERTEX_ATTRIBUTE, GL_FLOAT, 0, 3, sizeof (QVector3D) * 2);
    _program->setAttributeBuffer(PROGRAM_NORMAL_ATTRIBUTE, GL_FLOAT, sizeof (QVector3D), 3, sizeof (QVector3D) * 2);

    int first = 0;
    //    qDebug() << "start drawing";
    for (int i = 0; i < _elements.size(); ++i) {
        if (object.isEmpty() || _elements[i]._objectName.startsWith(object+"_")) {
            if (group.isEmpty() || _elements[i]._groupName == group) {
//                qDebug() << "darw #" << i+1 << " mode(" << (_elements[i].mode() == GL_TRIANGLES ? "GL_TRIANGLES" : _elements[i].mode() == GL_QUADS ? "GL_QUADS" : "GL_POLYGON")  << ")" << first << _elements[i].count();
                glDrawArrays(_elements[i].mode(), first, _elements[i].count());
            }
        }
        first += _elements[i].count();
    }

    _vbo.release();

    _program->disableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
    _program->disableAttributeArray(PROGRAM_NORMAL_ATTRIBUTE);
    _program->release();
}

void ObjScene::setModel(const QMatrix4x4 &m)
{
    _program->bind();
    _program->setUniformValue(modelLocation, m);
    _program->release();
}

void ObjScene::setView(const QMatrix4x4 &v)
{
    _program->bind();
    _program->setUniformValue(viewLocation, v);
    _program->release();
}

void ObjScene::setProjection(const QMatrix4x4 &p)
{
    _program->bind();
    _program->setUniformValue(projectionLocation, p);
    _program->release();
}

void ObjScene::setColor(const QColor &ambiant, const QColor &diffuse, const QColor &specular, GLfloat hardness)
{
    _program->bind();
    _program->setUniformValue(ambiantLocation, ambiant);
    _program->setUniformValue(diffuseLocation, diffuse);
    _program->setUniformValue(specularLocation, specular);
    _program->setUniformValue(hardnessLocation, hardness);
    _program->release();
}

void ObjScene::setLight(const QVector3D &lightPosition)
{
    _program->bind();
    _program->setUniformValue(lightLocation, lightPosition);
    _program->release();
}
