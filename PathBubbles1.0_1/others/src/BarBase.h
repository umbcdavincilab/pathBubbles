#ifndef BARBASE_H
#define BARBASE_H

#include <QtGui>
#include "3DHelper.h"

class BarBase : public QObject, public QGraphicsItem
{
	Q_OBJECT
	Q_INTERFACES( QGraphicsItem )
public:
	BarBase( QSizeF, int x, int y);
	virtual void resizeItem( QSizeF size );
	QSizeF getSize();
	virtual QRectF boundingRect() const;
	QRectF shadow();
	virtual void updateSize( QSizeF size );
	virtual void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	
	QString posString();
	QString headofLogMsg();
	virtual QString tailofLogMsg() = 0;
	virtual void logInit();
	virtual void logMoveTo();

signals:
	void logInfor( QString str );

protected:
	virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void wheelEvent(QGraphicsSceneWheelEvent *event);

	QSizeF _size;
	QPointF _lastPos;
};
#endif 
