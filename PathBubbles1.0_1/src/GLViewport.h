#ifndef GLVIEWPORT_H
#define GLVIEWPORT_H

#include <QGLWidget>

class GLViewport : public QGLWidget
{
public:
	GLViewport() : QGLWidget( QGLFormat(QGL::DoubleBuffer ) )
	//GLViewport() : QGLWidget( QGLFormat::sampleBuffers() )
	{
		return;
	}
	void initializeGL()
	{
		glClearColor( 1, 0, 0, 1 );
		//glFrustum( -this->width()/2, this->width()/2, -this->height()/2, this->height()/2, -1000, 1000);
		return;
	}
	void resizeGL( int w, int h)
	{
		glViewport( 0, 0, (GLint)w, (GLint)h );
		glFrustum( -w/2, w/2, -h/2, h/2, -1000, 1000);
		//GLfloat m[16];
		//glGetFloatv (GL_PROJECTION_MATRIX, m);   
	}
};
#endif