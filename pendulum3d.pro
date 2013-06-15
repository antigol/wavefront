#-------------------------------------------------
#
# Project created by QtCreator 2012-05-24T07:27:08
#
#-------------------------------------------------

QT       += core gui opengl widgets

TARGET = pendulum3d
TEMPLATE = app


SOURCES += main.cpp\
    objparser.cpp \
    objmesh.cpp \
    objscene.cpp \
    glwidget.cpp \
    doublependulum.cpp \
    linepath.cpp \
    skybox.cpp

HEADERS  += \
    objparser.h \
    objmesh.h \
    objscene.h \
    glwidget.h \
    doublependulum.h \
    linepath.h \
    skybox.h

OTHER_FILES += \
    objshader.frag \
    objshader.vert \
    linepath.vert \
    linepath.frag \
    skybox.vert \
    skybox.frag

RESOURCES += \
    data.qrc

QMAKE_CXX += -std=c++11
