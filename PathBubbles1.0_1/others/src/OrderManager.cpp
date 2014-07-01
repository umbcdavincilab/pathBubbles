#include "OrderManager.h"

OrderManager::OrderManager()
{
	return;
}

void OrderManager::begin()
{
	list.clear();
	return;
}

void OrderManager::end()
{
	row = list.length();
	orderItems();
	return;
}

void OrderManager::addItem(QPointF pos)
{
	list.append(pos);
	return;
}

QPoint OrderManager::getNewPos(int index)
{
	//QPoint newpos((index % num_Wid) * Width + Width/2 ,(index - (index % num_Wid))/num_Wid*Width + Width/2);
	QPoint newpos( list[index].x(), list[index].y());
	return newpos;
}

void OrderManager::orderItems()
{
	//			initialize the position array
	QList<QPointF> available;
	for( int index = 0; index < list.length(); index++)
	{
		QPointF ava((index % num_Wid) * Width + Width/2 ,(index - (index % num_Wid))/num_Wid*Width + Width/2);
		available.append( ava );
	}
	int left = list.length();
	while( left != 0)
	{
		QPointF cur_Point = list[left - 1];
		int cur_cost = 999999;
		int index;
		for( int i = 0; i < left; i++)
		{
			if( cost( cur_Point, available[i] ) < cur_cost)
			{
				cur_cost = cost( cur_Point, available[i] );
				index = i;
			}
		}
		list.replace( left - 1, available[ index ]);
		available.removeAt( index );
		left -= 1;
	}
	return;
}

int OrderManager::num()
{
	return list.length();
}

int OrderManager::cost(QPointF cur, QPointF ava)
{
	return (int)sqrt( pow( cur.x() - ava.x(), 2) + pow( cur.y() - ava.y(), 2));
}