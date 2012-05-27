#ifndef OBJPARSER_H
#define OBJPARSER_H

#include <QTextStream>
#include <QList>
#include <QVector3D>
#include "objscene.h"

class ObjParser
{
public:
    ObjParser(ObjScene *scene);

    void parse(const QString &filename);

private:
    void parseV(QTextStream &input);
    void parseVN(QTextStream &input);
    void parseFace(QTextStream &input);

    ObjScene *_scene;
    QList<QVector3D> _vertices;
    QList<QVector3D> _normals;
};

#endif // OBJPARSER_H
