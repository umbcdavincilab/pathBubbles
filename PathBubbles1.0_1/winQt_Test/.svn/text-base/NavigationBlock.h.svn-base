#ifndef NAVIGATIONBLOCK_H
#define NAVIGATIONBLOCK_H

#include "BarBase.h"
#include "qtbox.h"

#define SCALE 20

struct BookMark
{
	QString name;
	QRectF rect;
	QColor color;
};

class NavigationBlock : public BarBase
{
	Q_OBJECT
public:
	NavigationBlock( QSizeF size );
	void updateSize( QSizeF size );
	void shiftViewportByOffset( QPoint off );
	float getOffset();
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	void saveAsXmlNode( QDomDocument &doc, QDomElement &node );
	void recoverFromXmlNode( QDomElement node );

	QString tailofLogMsg();
	void logSaveCurView( int index );
	void logDeleteBookMark( int index );
	void logZoomIn();
	void logZoomOut();
	void logMoveViewTo( int x, int y );

signals:
	void shiftViewport( QPoint );
	void updateCoverage( float );
protected:
	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	void keyPressEvent(QKeyEvent *event);
	void wheelEvent(QGraphicsSceneWheelEvent *event);
private:
	int onBookmark( QPointF pos );
	void saveCurrentViewport();
	void handleKeyboard( QKeyEvent *event );
	void deleteViewport( int index );
	void drawBookmarks( QPainter* painter );
	void drawAllBubbles( QPainter* painter );
	void addBubbleRects();
	int hitBubbleRect( QPointF pos );
	QColor getQColorByIndex( int index );
	QList<ItemBase*> _bubbles;
	QList<QRectF> _bubbleRects;
	QPointF _lastPos;
	ItemBase* SELECTED;
	QList< BookMark > _marks;
	bool _ONVIEWPORT;
	int _ONITEM;
	int _currentMarks;
	int _HIGHLIGHTBUBBLE;
	float _rectOffset;
	QRect _rect;
};

#endif