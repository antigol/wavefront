#include "objparser.h"
#include <QFile>
#include <QDebug>
#include <QStringList>

ObjParser::ObjParser(ObjScene *scene) :
    _scene(scene)
{
}

void ObjParser::parse(const QString &filename)
{
    // --- CLEAR ---
    _scene->clear();
    _vertices.clear();
    _normals.clear();

    // --- FILE ---
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Cannot read file " << filename;
        return;
    }

    QTextStream input(&file);


    QString keyWord;

    while (!input.atEnd()) {
        input >> keyWord;

        if (keyWord.startsWith('#')) {
            // en s'en fiche
            input.readLine(); // skip line
        }

        else if (keyWord == "mtllib") {
            // en s'en fiche
            input.readLine(); // skip line
        }

        else if (keyWord == "o") {
            QString str;
            input >> str;
            _scene->setCurrentObject(str);
        }

        else if (keyWord == "g") {
            QString str;
            input >> str;
            _scene->setCurrentGroup(str);
        }

        else if (keyWord == "v") {
            parseV(input);
        }

        else if (keyWord == "vt") {
            // en s'en fiche
            input.readLine(); // skip line
        }

        else if (keyWord == "vn") {
            parseVN(input);
        }

        else if (keyWord == "usemtl") {
            // en s'en fiche
            input.readLine(); // skip line
        }

        else if (keyWord == "f") {
            parseFace(input);
        }
    }
}

QTextStream &operator>>(QTextStream &input, QVector3D &v)
{
    float xp, yp, zp;
    input >> xp >> yp >> zp;
    v.setX(xp);
    v.setY(yp);
    v.setZ(zp);
    return input;
}

void ObjParser::parseV(QTextStream &input)
{
    QVector3D v;
    input >> v;
    _vertices.append(v);
}

void ObjParser::parseVN(QTextStream &input)
{
    QVector3D vn;
    input >> vn;
    _normals.append(vn);
}

void ObjParser::parseFace(QTextStream &input)
{
    QString line = input.readLine();
    QStringList split = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);

    QList<QVector3D> v;
    QList<QVector3D> vn;
    for (int i = 0; i < split.size(); ++i) {

        QStringList summit = split[i].split('/');
        for (int i = 0; i < summit.size(); ++i) {
            if (i == 0) {
                int k = summit[i].toInt() - 1;
                v << _vertices[k];
            }
            if (i == 2) {
                int k = summit[i].toInt() - 1;
                vn << _normals[k];
            }
        }
    }

    if (v.size() == 3) {
        if (vn.size() == 3)
            _scene->addTriangleWithNormals(v[0], vn[0], v[1], vn[1], v[2], vn[2]);
        else
            _scene->addTriangle(v[0], v[1], v[2]);
    }

    if (v.size() == 4) {
        if (vn.size() == 4)
            _scene->addQuadWithNormals(v[0], vn[0], v[1], vn[1], v[2], vn[2], v[3], vn[3]);
        else
            _scene->addQuad(v[0], v[1], v[2], v[3]);
    }

    if (v.size() > 4) {
        if (vn.size() == v.size())
            _scene->addPolygonWithNormals(v, vn);
        else
            _scene->addPolygon(v);
    }
}
