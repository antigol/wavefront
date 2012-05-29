#-------------------------------------------------
#
# Project created by QtCreator 2012-05-24T07:27:08
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = wavefront
TEMPLATE = app


SOURCES += main.cpp\
    objparser.cpp \
    objmesh.cpp \
    objscene.cpp \
    glwidget.cpp \
    doublependulum.cpp \
    linepath.cpp

HEADERS  += \
    objparser.h \
    objmesh.h \
    objscene.h \
    glwidget.h \
    doublependulum.h \
    linepath.h

OTHER_FILES += \
    objshader.frag \
    objshader.vert \
    linepath.vert \
    linepath.frag

RESOURCES += \
    data.qrc
