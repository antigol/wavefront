#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QtOpenGL/QGLWidget>
#include <QMatrix4x4>
#include "objscene.h"

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
    
    QPointF _oldmouseposition;
    float a, b;
    float u, v;
    float z;

    ObjScene _scene;
};

#endif // GLWIDGET_H
