#include "glwidget.h"
#include "objparser.h"
#include <QDebug>
#include <QTimerEvent>
#include <cmath>

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(parent), _a(-140.0), _b(20.0), _z(-55.0),
    _pendulum(10000)
{
    ObjParser parser(&_scene);
    parser.parse(":/files/stem.obj");

    _oldTime = 0.0;
    _pendulum.setParameters(2.0, 1.0, 1.0, 1.0, 75.0, 80.0);
    _pendulum.reset();
}

GLWidget::~GLWidget()
{
    deleteTexture(_texture);
}

void GLWidget::initializeGL()
{
    _scene.initializeGL(context());
    _scene.setLight(QVector3D(1.0, 0.5, -1.0).normalized());

    _linepath.initializeGL(context());
    _linepath.setColor(QColor(0, 0, 0, 50), QColor(255, 255, 255, 50));

    _skybox.initialiseGL(context());
    _texture = bindTexture(QImage(":/files/skybox2.jpg"));

    _time.start();
    _timerGL = startTimer(20);
    _timerDP = startTimer(0);
}

void GLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    QMatrix4x4 p;
    p.perspective(50.0, qreal(w)/qreal(h?h:1), 0.1, 1000.0);
    _scene.setProjection(p);
    _linepath.setProjection(p);
    _skybox.setProjection(p);
}

void GLWidget::paintGL()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE); // cull face

    glEnable(GL_TEXTURE_2D); // texture

    glEnable(GL_BLEND); // alpha
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 m;
    m.translate(0, 0, _z);
    m.rotate(_b, 1, 0, 0);
    m.rotate(_a, 0, 1, 0);
    _scene.setView(m);
    _linepath.setView(m);
    _skybox.setView(m);

    _skybox.drawGL(_texture);

    _linepath.drawGL();

    m.setToIdentity();
    m.translate(0.0, 0.0, 2.5);

    _scene.setModel(m);
    _scene.setColor(QColor(Qt::gray).darker(800), Qt::gray, Qt::white, 50.0);
    _scene.drawGL("basis");

    m.translate(0.0, 0.0, -5.0);
    m.scale(-1.0, 1.0, -1.0);
    _scene.setModel(m);
    _scene.drawGL("basis");

    m.translate(0.0, 0.0, -5.0);
    m.scale(-1.0, 1.0, -1.0);


    m.rotate(_pendulum.a1(), 0.0, 0.0, 1.0);
    m.scale(_pendulum.l1(), 1.0, 1.0);

    m.translate(0.0, 0.0, -1.0);
    _scene.setModel(m);
    _scene.setColor(QColor(Qt::red).darker(400), QColor(Qt::yellow).lighter(), Qt::white, 10.0);
    _scene.drawGL("stem1");

    m.translate(0.0, 0.0, -3.0);
    m.scale(1.0, -1.0, -1.0);
    _scene.setModel(m);
    _scene.drawGL("stem1");
    m.translate(0.0, 0.0, -3.0);
    m.scale(1.0, -1.0, -1.0);


    m.translate(10.0, 0.0, 0.0);
    m.scale(1.0/_pendulum.l1(), 1.0, 1.0);


    m.rotate(_pendulum.a2() - _pendulum.a1(), 0.0, 0.0, 1.0);
    m.scale(_pendulum.l2(), 1.0, 1.0);
    m.translate(0.0, 0.0, -1.5);
    _scene.setModel(m);
    _scene.setColor(QColor(Qt::blue).darker(400), QColor(Qt::cyan).lighter(), Qt::white, 10.0);
    _scene.drawGL("stem2");

    m.translate(10.0, 0.0, 0.0);
    QVector3D p = m.map(QVector3D(0.0, 0.0, 0.0));
    _linepath.addPoint(p, _pendulum.kinetic() / 100.0);
    qDebug() << "kinetic:" << _pendulum.kinetic() << "potential:" << _pendulum.potential() << "total:" << _pendulum.kinetic() + _pendulum.potential();
}

#include <QMouseEvent>
void GLWidget::mousePressEvent(QMouseEvent *e)
{
    _oldmouseposition = e->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *e)
{
    QPointF d = e->pos() - _oldmouseposition;
    _oldmouseposition = e->pos();

    if (e->buttons() & Qt::LeftButton) {
        _a += d.x() * 0.5;
        _b += d.y() * 0.5;
        _b = _b < -90.0 ? -90.0 : _b > 90.0 ? 90.0 : _b;
    }
}

#include <QWheelEvent>
void GLWidget::wheelEvent(QWheelEvent *e)
{
    _z *= pow(0.999, e->delta());
}

void GLWidget::timerEvent(QTimerEvent *e)
{
    if (e->timerId() == _timerGL) {
        updateGL();
    } else if (e->timerId() == _timerDP) {
        double time = 1.0 * double(_time.elapsed()) / 1000.0;
        double dt = time - _oldTime;
        if (dt == 0.0)
            return;
        _oldTime = time;

//        QTime tt;
//        tt.start();
        _pendulum.move2(dt);
//        qDebug() << tt.elapsed();
    }
}

#include <QKeyEvent>
void GLWidget::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_Space:
        _linepath.clear();
        _pendulum.reset();
        break;
    }
}
