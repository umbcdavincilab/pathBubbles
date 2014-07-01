#ifndef QTWAKE_H
#define QTWAKE_H

#include "qtbox.h"
#include "Bat.h"
#include "qtbat.h"
#include <GL/freeglut.h>

class OpenGLScene;

class QtWake : public ItemBase
{
	Q_OBJECT

public:
	QtWake( int size, int x, int y, OpenGLScene* Scene, Bat* bat, QtBat* pare, int begin, int end );
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	int getBegin()
	{
		return _begin;
	}
	int getEnd()
	{
		return _end;
	}
	void setBeginPosToLink( QPointF input )
	{
		beginPosToLink = input;
	}
	void setEndPosToLink( QPointF input )
	{
		endPosToLink = input;
	}
	void logOpen();
public slots:
	void sliderChanged( int );
protected:
	void mousePressEvent( QGraphicsSceneMouseEvent *event );
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent( QGraphicsSceneMouseEvent *event );
private:
	//			functions
	QString n1, n2, n3, n4, n5;
	void initInfor();
	void displayName( QPainter* painter );
	void initFloatingControl();
	void displayLinksToQtBat( QPainter* painter );
	void updateControlPos();
	QLCDNumber* lcdNumber;
	QSlider* slider;
	bool _ONMOVE;
	bool _ONRESIZE;
	bool _ONCONTROL;
	//			properties
	Bat *_bat;
	float _margin;
	QtBat* _pare;
	OpenGLScene* _scene;
	int _begin, _end;
	int lastSize;
	QPointF controlLastPos;
	QPointF beginPosToLink, endPosToLink;
};

#endif