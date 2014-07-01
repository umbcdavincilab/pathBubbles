#ifndef GLBAR_H
#define GLBAR_H

#include <QGLWidget>
#include "LabCut.h"
#include "LabObject.h"

class GLBAR :public QGLWidget
{
	Q_OBJECT
public:
	GLBAR(QWidget *parent = 0);
	virtual ~GLBAR(); //J.C.

	QSize minimumSizeHint() const;
	QSize sizeHint() const;
	void updateScene(){ updateGL(); }
	void setColor(LabCut* icut, LabObject* ilab){ cut = icut; lab = ilab; }
protected:
	void initializeGL();
	void paintGL();
	void resizeGL(int width, int height);

private:
	LabCut* cut;
	LabObject* lab;
};

#endif