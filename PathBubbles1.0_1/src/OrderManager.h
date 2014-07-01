#ifndef ORDERMANAGER_H
#define ORDERMANAGER_H

#include <QList>
#include <QGraphicsItem>
#include <QPointF>
#include <math.h>

class OrderManager
{
public:
	OrderManager();
	void begin();
	void end();
	int num();
	void set_num_Wid( int num )
	{
		num_Wid = num; 
	}
	void setWid( int width )
	{ 
		Width = width;
	}
	void addItem( QPointF pos);
	QPoint getNewPos( int index );
private:
	void orderItems();
	int cost( QPointF cur, QPointF ava);
	int row;
	int Width;
	int num_Wid;
	QList<QPointF> list;
};

#endif