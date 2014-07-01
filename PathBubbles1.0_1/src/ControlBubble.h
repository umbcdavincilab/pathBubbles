#ifndef CONTROLBUBBLE_H
#define CONTROLBUBBLE_H

#include "qtbox.h"
#include "3DHelper.h"

class OpenGLScene;
#define CONTROL 200

class ControlBubble : public ItemBase
{
	Q_OBJECT
public:
	ControlBubble( int x, int y, OpenGLScene* scene );
	~ControlBubble();
	virtual void initUI();
	void myHide();
	void myShow();
protected:
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	virtual void mousePressEvent( QGraphicsSceneMouseEvent *event );
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseMoveEvent( QGraphicsSceneMouseEvent *event );
	virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
	OpenGLScene* _scene;
	int _lastSize;
	QPointF _lastPos;
};

#endif