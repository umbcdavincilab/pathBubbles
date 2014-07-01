#ifndef CONTROLBLOCK_H
#define CONTROLBLOCK_H

#include "qtbox.h"
#include "InstanceManager.h"
#include "BubbleInstance.h"
#include "BubbleNode.h"
#include "InstanceInitializer.h"

class OpenGLScene;

#define CONTROLSIZE 300
#define CONTROLMARGIN 35
#define CORESSIZE 20

class ControlBlock : public ItemBase
{
	Q_OBJECT
public:
	ControlBlock( int x, int y, OpenGLScene* scene );
protected:
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	void mousePressEvent( QGraphicsSceneMouseEvent *event );
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent( QGraphicsSceneMouseEvent *event );
	void hoverMoveEvent( QGraphicsSceneHoverEvent *event);
	void wheelEvent( QGraphicsSceneWheelEvent * event );
private:
	void drawOrder( QPainter* painter );
	bool onSliderBar( QPointF pos );
	void zoomIn();
	void zoomOut();
	void zoomByFloat();
	void updateUpAndDown( int index );
	void paintSideBar( QPainter* painter );
	float _heightPercent;
	QRectF getPaintArea();
	QRectF getBoundingArea();
	bool onUpAndDown( QPointF pos );
	bool UPANDDOWN;
	bool onTranslate( QPointF pos );
	bool TRANSLATE;
	void initManager();
	OpenGLScene* _scene;
	bool _onBoarder;
	InstanceManager* _manager;
	InstanceInitializer* _init;
	QPointF _initDragPos;
	int _zoomFactor;
	QImage _cross;
	QImage _up;
	QImage _down;
	//
	QRectF _currentCross;
	QRectF _currentUp;
	QRectF _currentDown;
	//
	QPoint _offset;
};
#endif