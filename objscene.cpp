#include "objscene.h"

ObjScene::ObjScene(QObject *parent) :
    QObject(parent),
    _vbo(QGLBuffer::VertexBuffer)
{
}

void ObjScene::addTriangleWithNormals(QVector3D v1, QVector3D vn1, QVector3D v2, QVector3D vn2, QVector3D v3, QVector3D vn3)
{
    if (_elements.isEmpty() || _elements.last().mode() != GL_TRIANGLES)
        _elements.append(ObjMesh(GL_TRIANGLES));

    ObjMesh &mesh = _elements.last();
    mesh.addVertex(v1, vn1);
    mesh.addVertex(v2, vn2);
    mesh.addVertex(v3, vn3);
}

void ObjScene::addQuadWithNormals(QVector3D v1, QVector3D vn1, QVector3D v2, QVector3D vn2, QVector3D v3, QVector3D vn3, QVector3D v4, QVector3D vn4)
{
    if (_elements.isEmpty() || _elements.last().mode() != GL_QUADS)
        _elements.append(ObjMesh(GL_QUADS));

    ObjMesh &mesh = _elements.last();
    mesh.addVertex(v1, vn1);
    mesh.addVertex(v2, vn2);
    mesh.addVertex(v3, vn3);
    mesh.addVertex(v4, vn4);
}

void ObjScene::addPolygonWithNormals(QList<QVector3D> v, QList<QVector3D> vn)
{
    _elements.append(ObjMesh(GL_POLYGON));
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

    _elements.append(ObjMesh(GL_POLYGON));
    ObjMesh &mesh = _elements.last();
    for (int i = 0; i < v.size(); ++i) {
        mesh.addVertex(v[i], vn);
    }
}

void ObjScene::clear()
{
    _elements.clear();
}

void ObjScene::initializeGL(const QGLContext *context)
{
    _program = new QGLShaderProgram(context, this);
    _program->addShaderFromSourceFile(QGLShader::Vertex, ":/glsl/shader.vert");
    _program->addShaderFromSourceFile(QGLShader::Fragment, ":/glsl/shader.frag");
    if (!_program->link())
        qDebug() << _program->log();

    vertexLocation = _program->attributeLocation("vertex");
    normalLocation = _program->attributeLocation("normal");
    projectionLocation = _program->uniformLocation("matrixp");
    modelviewLocation = _program->uniformLocation("matrixmv");
    ambiantLocation = _program->uniformLocation("ambiant");
    diffuseLocation = _program->uniformLocation("diffuse");
    specularLocation = _program->uniformLocation("specular");
    specularpowerLocation = _program->uniformLocation("specularpower");
    lightLocation = _program->uniformLocation("light");

    // tout mettre dans le VBO
    _vbo.create();
    _vbo.bind();
    int size = 0;
    for (int i = 0; i < _elements.size(); ++i) {
        size += _elements[i]._vertices.size();
    }
    _vbo.allocate(size * sizeof (QVector3D));
    int offset = 0;
    for (int i = 0; i < _elements.size(); ++i) {
        _vbo.write(offset, _elements[i]._vertices.constData(), _elements[i]._vertices.size() * sizeof (QVector3D));
        offset += _elements[i]._vertices.size() * sizeof (QVector3D);
    }
    _vbo.release();
}

void ObjScene::drawGL()
{
    _program->bind();
    _vbo.bind();

    _program->enableAttributeArray(vertexLocation);
    _program->enableAttributeArray(normalLocation);
    _program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3, sizeof (QVector3D) * 2);
    _program->setAttributeBuffer(normalLocation, GL_FLOAT, sizeof (QVector3D), 3, sizeof (QVector3D) * 2);

    int first = 0;
    qDebug() << "start drawing";
    for (int i = 0; i < _elements.size(); ++i) {
        qDebug() << "darw " << _elements[i].mode() << first << _elements[i]._vertices.size() / 2;
        glDrawArrays(_elements[i].mode(), first, _elements[i]._vertices.size() / 2);
        first += _elements[i]._vertices.size() / 2;
    }

    _program->disableAttributeArray(vertexLocation);
    _program->disableAttributeArray(normalLocation);

    _vbo.release();
    _program->release();
}

void ObjScene::setProjection(const QMatrix4x4 &p)
{
    _program->bind();
    _program->setUniformValue(projectionLocation, p);
    _program->release();
}

void ObjScene::setModelView(const QMatrix4x4 &m)
{
    _program->bind();
    _program->setUniformValue(modelviewLocation, m);
    _program->release();
}

void ObjScene::setColor(const QColor &ambiant, const QColor &diffuse, const QColor &specular, GLfloat specularpower)
{
    _program->bind();
    _program->setUniformValue(ambiantLocation, ambiant);
    _program->setUniformValue(diffuseLocation, diffuse);
    _program->setUniformValue(specularLocation, specular);
    _program->setUniformValue(specularpowerLocation, specularpower);
    _program->release();
}

void ObjScene::setLight(const QVector3D &lightPosition)
{
    _program->bind();
    _program->setUniformValue(lightLocation, lightPosition);
    _program->release();
}
