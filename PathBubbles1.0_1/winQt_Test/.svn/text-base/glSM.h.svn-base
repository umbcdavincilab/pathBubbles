#ifndef GLSM_H
#define GLSM_H

#include <QGLWidget>
#include "SM.h"

#define DOWN 1
#define UP 0

class GLSM :public QGLWidget
{
	Q_OBJECT
public:
	GLSM(QWidget *parent = 0);
	~GLSM();

	QSize minimumSizeHint() const;
	QSize sizeHint() const;
	void setSM(SM* input){ sm = input; };
	void updateScene(){ updateGL(); }

protected:
	void initializeGL();
	void paintGL();
	void resizeGL(int width, int height);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent * event );

private:
	SM *sm;
	int OffX, OffY;
	QPoint lastPos;
	int DRAG;
};

#endif