#include <QtGui>
#include <QtOpenGL>
#include <fstream>
#include <math.h>

#include "glBar.h"

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

GLBAR::GLBAR(QWidget *parent)
: QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
}

GLBAR::~GLBAR()
{
}

QSize GLBAR::minimumSizeHint() const
{
	return QSize(10, 10);
}

QSize GLBAR::sizeHint() const
//! [3] //! [4]
{
	return QSize(10, 10);
}

void GLBAR::initializeGL()
{
	qglClearColor(QColor(0,0,0));
	//glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glShadeModel(GL_SMOOTH);
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	glEnable(GL_MULTISAMPLE);
	static GLfloat lightPosition[4] = { 0.5, 5.0, 7.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
}

void GLBAR::paintGL()
{
	myColor3 color;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	//std::ofstream output("Lab_Color");
	for( int i = 0; i <= 100; i++)
	{
		Lab3D templab = cut->get_Position(i/100.0);
		lab->get_Color(templab, color);
		glBegin(GL_POLYGON);
		//glColor3f(1.0, 0.0, 0.0);
		//output<<"<Lab_Color R = "<<color.a<<" G = "<<color.b<<" B = "<<color.c<<" Index = "<<i<<" </Lab_Color>"<<std::endl;
		glColor3f((float)color.a/255.0, (float)color.b/255.0, (float)color.c/255.0);
		//glColor3f(0.0, 1.0, 0.0);
		glBegin(GL_TRIANGLES);
			glVertex2i( i, 0);
			glVertex2i( i + 1, 30);
			glVertex2i( i, 30);
		glEnd();
		glBegin(GL_TRIANGLES);
			glVertex2i( i, 0);
			glVertex2i( i + 1, 0);
			glVertex2i( i + 1, 30);
		glEnd();
	}
	//output.close();
	//glBegin(GL_POLYGON);
	//	glVertex2i(0, 0);
	//	glVertex2i(1, 0);
	//	glVertex2i(1, 1);
	//	glVertex2i(0, 0);
	//	glVertex2i(1, 1);
	//	glVertex2i(1, 0);
	//glEnd();
	//glTranslatef(-OffX, -OffY, 0.0);
	//updateGL();
}

void GLBAR::resizeGL(int width, int height)
{
	//int side = qMin(width, height);
	//glViewport((width - side) / 2, (height - side) / 2, side, side);
	//glViewport(width / 2, height / 2, width, height);
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(-0.5, +0.5, -0.5, +0.5, 4.0, 15.0);
	gluOrtho2D(0, 100, 0, 30);
	//gluOrtho2D(-width/2, width/2, -height/2, height/2);
	//gluOrtho2D(-width / 2, width / 2,  height / 2,  height / 2);
	glMatrixMode(GL_MODELVIEW);
}