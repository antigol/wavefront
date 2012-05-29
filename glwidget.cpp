#include "glwidget.h"
#include "objparser.h"
#include <QDebug>
#include <cmath>

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(parent), _a(0.0), _b(0.0), _z(-20.0)
{
    ObjParser parser(&_scene);
    //    parser.parse("../obj/cube.obj");
//    parser.parse("../obj/lamp.obj");
//    parser.parse("../obj/icosahedron.obj");
//    parser.parse("../obj/untitled.obj");
//    parser.parse("../obj/monkey.obj");
//    parser.parse("../wavefront/suzanne.obj");
    parser.parse("../wavefront/stem.obj");
//    parser.parse("../obj/torus.obj");

    _oldTime = 0.0;
    _time.start();
    _pendulum.setParameters(1.0, 1.0, 1.0, 1.0, 0.0, 80.0);
    _pendulum.reset();


    startTimer(0);
}

void GLWidget::initializeGL()
{
    _scene.initializeGL(context());
    _scene.setLight(QVector3D(0, 1, 1).normalized());

    _linepath.initializeGL(context());
    _linepath.setColor(Qt::red, Qt::white, 0.5);
}

void GLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    QMatrix4x4 p;
    p.perspective(50.0, qreal(w)/qreal(h?h:1), 0.1, 1000.0);
    _scene.setProjection(p);
    _linepath.setProjection(p);
}

void GLWidget::paintGL()
{
    glEnable(GL_DEPTH_TEST);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 m;
    m.translate(0, 0, _z);
    m.rotate(_b, 1, 0, 0);
    m.rotate(_a, 0, 1, 0);
    _scene.setView(m);
    _linepath.setView(m);

    _linepath.drawGL();

    m.setToIdentity();
    m.scale(_pendulum.l1(), 1.0, 1.0);
    m.rotate(_pendulum.a1(), 0.0, 0.0, 1.0);
    _scene.setModel(m);
    _scene.setColor(QColor(Qt::red).darker(), QColor(Qt::yellow).lighter(), Qt::white, 10.0);
    _scene.drawGL("stem1");

    m.translate(10.0, 0.0, 0.0);
    m.scale(_pendulum.l2() / _pendulum.l1(), 1.0, 1.0);
    m.rotate(_pendulum.a2() - _pendulum.a1(), 0.0, 0.0, 1.0);
    m.translate(0.0, 0.0, -1.5);
    _scene.setModel(m);
    _scene.setColor(QColor(Qt::blue).darker(), QColor(Qt::cyan).lighter(), Qt::white, 10.0);
    _scene.drawGL("stem2");

    m.translate(10.0, 0.0, 0.0);
    QVector3D p = m.map(QVector3D(0.0, 0.0, 0.0));
    _linepath.addPoint(p);
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
        _a += d.x() * 0.5;
        _b += d.y() * 0.5;
        _b = _b < -90.0 ? -90.0 : _b > 90.0 ? 90.0 : _b;
    }
    updateGL();
}

#include <QWheelEvent>
void GLWidget::wheelEvent(QWheelEvent *e)
{
    _z *= pow(0.999, e->delta());
    updateGL();
}

void GLWidget::timerEvent(QTimerEvent *)
{
    double time = double(_time.elapsed()) / 1000.0;
    double dt = time - _oldTime;
    _oldTime = time;

    if (dt != 0.0) {
        _a += dt * 5.0;
        if (_a > 360.0) _a -= 360.0;
        _pendulum.move(dt);
        updateGL();
    }
}
