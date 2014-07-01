#ifndef QTCOLORBAR_H
#define QTCOLORBAR_H

#include <qtbox.h>
#include <GL/glut.h>
#include "LabObject.h"
#include "LabCut.h"
#include "Lab3D.h"

class QtColorBar : public ItemBase
{
public:
	QtColorBar(int size, int x, int y, LabObject *Lab, LabCut *Cut);
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
private:
	LabObject *_lab;
	LabCut *_cut;
};


#endif