#include <QtGui>
#include <QtOpenGL>

#include <math.h>

#include "glSM.h"

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

GLSM::GLSM(QWidget *parent)
: QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
	OffX = OffY = 0;
	DRAG = 0;
}

GLSM::~GLSM()
{
}

QSize GLSM::minimumSizeHint() const
{
	//return QSize(150, 150);
	return QSize(250, 250);
}

QSize GLSM::sizeHint() const
//! [3] //! [4]
{
	//return QSize(150, 150);
	return QSize(250, 250);
}

void GLSM::initializeGL()
{
	qglClearColor(QColor( 0,0,0 ));
	//glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glShadeModel(GL_SMOOTH);
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	glEnable(GL_MULTISAMPLE);
	static GLfloat lightPosition[4] = { 0.5, 5.0, 7.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
}

void GLSM::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(OffX, OffY, 0.0);
	//glBegin(GL_POLYGON);
	//	glVertex2i(0, 0);
	//	glVertex2i(1, 0);
	//	glVertex2i(1, 1);
	//	glVertex2i(0, 0);
	//	glVertex2i(1, 1);
	//	glVertex2i(1, 0);
	//glEnd();
	//sm->renderScene( );
	//glTranslatef(-OffX, -OffY, 0.0);
	//updateGL();
}

void GLSM::resizeGL(int width, int height)
{
	//int side = qMin(width, height);
	//glViewport((width - side) / 2, (height - side) / 2, side, side);
	//glViewport(width / 2, height / 2, width, height);
	glViewport( 0, 0, width, height );

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(-0.5, +0.5, -0.5, +0.5, 4.0, 15.0);
	//gluOrtho2D(-200, 200, -150, 150);
	gluOrtho2D( 0, width, 0, height);
	//gluOrtho2D(-width/2, width/2, -height/2, height/2);
	//gluOrtho2D(-width / 2, width / 2,  height / 2,  height / 2);
	glMatrixMode(GL_MODELVIEW);
}

void GLSM::mousePressEvent(QMouseEvent *event)
{
	int result = sm->mousePressEvent( event->pos().x() - OffX, event->pos().y() - OffY );
	if( result != -1 )
	{
		DRAG = 1;
		return;
	}
	lastPos = event->pos();
	updateGL();
}

void GLSM::mouseMoveEvent(QMouseEvent *event)
{
	if ( DRAG == 1)
	{
		return;
	}
	OffX += (-lastPos.x() + event->pos().x());
	OffY += lastPos.y() - event->pos().y();
	lastPos = event->pos();
	updateGL();
}

void GLSM::mouseReleaseEvent(QMouseEvent * event )
{
	int result = sm->mouseReleaseEvent( event->pos().x() - OffX, event->pos().y() - OffY );
	DRAG = 0;
	updateGL();
}