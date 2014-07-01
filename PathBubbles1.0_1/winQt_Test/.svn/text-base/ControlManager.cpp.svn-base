#include "ControlManager.h"

ControlManager::ControlManager()
{
	_index = 0;
	return;
}
void ControlManager::addControl(QGraphicsItem *item, QGraphicsProxyWidget *floatingControlProxy)
{
	_item[ _index ] = item;
	_floatingControlProxy[ _index ] = floatingControlProxy;
	_index += 1;
}

void ControlManager::showControl(QGraphicsItem *item)
{
	for( int i = 0; i < _index; i++ )
	{
		if( item == _item[ i ] )
		{
			_floatingControlProxy[ i ]->show();
		}
	}
	return;
}

void ControlManager::hideControl(QGraphicsItem *item)
{
	for( int i = 0; i < _index; i++ )
	{
		if( item == _item[ i ] )
		{
			_floatingControlProxy[ i ]->hide();
		}
	}
	return;
}