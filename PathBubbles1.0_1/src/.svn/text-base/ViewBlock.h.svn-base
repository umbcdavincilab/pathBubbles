#ifndef VIEWBLOCK_H
#define VIEWBLOCK_H

#include "3DHelper.h"
#include "qtbox.h"
#include <QVector>

#define VIEWSIZE 200

class OpenGLScene;

class ViewBlock : public ItemBase
{
public:
	ViewBlock( int x, int y, OpenGLScene* scene );
protected:
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	void mousePressEvent( QGraphicsSceneMouseEvent *event );
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent( QGraphicsSceneMouseEvent *event );
	void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
private:
	void drawRects( QPainter* painter );
	void initGrid();
	int onDrag( QPointF pos );
	QList<QString> _view;
	QVector<QRectF> _rects;
	OpenGLScene* _scene;
	QRectF _dragRect;
	int _marginFromTop;
	int _gridWidth;
	int _gridHeight;
	int _sizeOff;
	int _currentIndex;
	bool _ONDRAD;
};

#endif