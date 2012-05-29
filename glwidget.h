#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QtOpenGL/QGLWidget>
#include <QMatrix4x4>
#include <QTime>
#include "objscene.h"
#include "linepath.h"
#include "doublependulum.h"

class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = 0);
    
private:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);
    void timerEvent(QTimerEvent *);
    void keyPressEvent(QKeyEvent *e);
    
    QPointF _oldmouseposition;
    float _a, _b;
    float _z;

    ObjScene _scene;
    LinePath _linepath;

    QTime _time;
    double _oldTime;

    DoublePendulum _pendulum;
};

#endif // GLWIDGET_H
