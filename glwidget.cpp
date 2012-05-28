#include "glwidget.h"
#include "objparser.h"
#include <QDebug>

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(parent)
{
    ObjParser parser(&_scene);
    //    parser.parse("../obj/cube.obj");
//    parser.parse("../obj/lamp.obj");
//    parser.parse("../obj/icosahedron.obj");
//    parser.parse("../obj/untitled.obj");
//    parser.parse("../obj/monkey.obj");
    parser.parse("../wavefront/cessna2.obj");
//    parser.parse("../obj/torus.obj");
}

void GLWidget::initializeGL()
{
    _scene.initializeGL(context());
    _scene.setColor(QColor(Qt::red).darker(), QColor(Qt::yellow).lighter(), Qt::white, 10.0);
//    _scene.setColor(Qt::black, Qt::black, Qt::white, 10.0);
    _scene.setLight(QVector3D(0, 1, 1).normalized());
}

void GLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    QMatrix4x4 p;
    p.perspective(70.0, qreal(w)/qreal(h?h:1), 0.1, 1000.0);
    _scene.setProjection(p);
}

void GLWidget::paintGL()
{
    glEnable(GL_DEPTH_TEST);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 m;
    m.translate(0, 0, z);

    _scene.setView(m);

    m.setToIdentity();
    m.rotate(b, 1, 0, 0);
    m.rotate(a, 0, 1, 0);

    m.translate(-u, -v, 0);

    _scene.setModel(m);

    _scene.drawGL();
}

#include <QMouseEvent>
void GLWidget::mousePressEvent(QMouseEvent *e)
{
    _oldmouseposition = e->posF();
}

void GLWidget::mouseMoveEvent(QMouseEvent *e)
{
    QPointF d = e->posF() - _oldmouseposition;
    _oldmouseposition = e->posF();

    if (e->buttons() & Qt::LeftButton) {
        a += d.x() * 0.5;
        b += d.y() * 0.5;
        b = b < -90.0 ? -90.0 : b > 90.0 ? 90.0 : b;
    }
    if (e->buttons() & Qt::RightButton) {
        u += d.x();
        v += d.y();
    }
    updateGL();
}

#include <QWheelEvent>
void GLWidget::wheelEvent(QWheelEvent *e)
{
    z += float(e->delta()) * 0.001;
    updateGL();
}
