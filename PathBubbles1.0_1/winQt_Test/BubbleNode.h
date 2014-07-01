#ifndef BUBBLENODE_H
#define BUBBLENODE_H

#include "BubbleInstance.h"
#include "3DHelper.h"
#include <QList>
#include <math.h>
#include <fstream>
#include <QFont>

#define MARGIN 5

struct Infor
{
	QString Name;
	QString Label;
	int Type;// 1 individue 2 run 3 view
	int Control; // 3 root 2 inner 1 leaf
};

struct POS
{
	QPointF pos;
	bool ACCEPT;
};

class BubbleNode
{
public:
	BubbleNode( Infor infor, BubbleNode* parent = 0 );
	void paint( QPainter* painter, QRectF boundingRect, QRectF max );
	void addChild( BubbleNode* child )
	{
		this->_child.append( child );
	}
	bool satisfyNode( BubbleInstance* instance );
	void printNode( std::ofstream* file, int space );
	void highLight( POS &pos );
	void enableDrag();
	void disableDrag();
	void drawInitViewPos( QPointF pos );
	void getHighLight( QString &name );
	bool textCanBeDrawn( QRectF boundingRect );

private:
	void initBoundingRect( QRectF boundingRect );
	bool formalizeRect( QRectF &bounding, QRectF &max );
	void drawCurrentRect( QPainter* painter, QRectF boundingRect, QRectF max, bool font );
	void drawChildrenRect( QPainter* painter, QRectF boundingRect, QRectF max );
	int findMinSqur( int size );
	Infor _infor;
	QList<BubbleNode*> _child;
	QList<QRectF> _bounding;
	BubbleNode* _parent;
	QRectF _currentRect;
	QRectF _transRect;
	bool _HIGHLIGHT;
	bool _DRAG;
	bool _DRAGREADY;
	//
	QFont _font;
	QFontMetricsF _metric;
};

#endif