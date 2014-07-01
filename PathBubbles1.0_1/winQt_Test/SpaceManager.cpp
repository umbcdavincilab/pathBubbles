#include "openglscene.h"
#include "qtbox.h"
#include "group.h"

SpaceManager::SpaceManager( OpenGLScene* scene )
{
	this->_scene = scene;
	return;
}

void SpaceManager::needToOpen( int number )
{
	return;
}



QPoint SpaceManager::getPosition()
{
	int Width = this->_scene->width();
	int Height = this->_scene->height();
	//			get all items in view
	QList<ItemBase*> list; 
	QList<QGraphicsItem*> allItem = this->_scene->items();
	for( int i = 0; i < allItem.size(); i++ )
	{
		ItemBase *itemBase = dynamic_cast<ItemBase*>( allItem[i] );
		if( itemBase )
		{
			if( itemBase->pos().x() > 0 && itemBase->pos().x() < Width )
			{
				if( itemBase->pos().y() > 0 && itemBase->pos().y() < Height )
				{
					list.append( itemBase );
				}
			}
		}
	}
	int curX = 150;
	int curY = 250;
	do
	{
		curX = 150;
		do
		{
			bool state = true;
			for( int i = 0; i < list.size(); i++ )
			{
				QRectF bounding = list[i]->mapRectToScene( list[i]->boundingRect() );
				if( bounding.contains( curX, curY ) )
				{
					state = false;
					break;
				}
			}
			if( state )
			{
				return QPoint( curX, curY );
			}
			else
			{
				curX += STEP;
			}
		}while( curX < Width );
		curY += STEP;
	}while( curY < Height );
	return QPoint( -1, -1 );
}

QVector<QRectF> SpaceManager::queryPreOpen( QPointF pos, int num)
{
	int Width = this->_scene->width() + pos.x();
	int Height = this->_scene->height() + pos.y();
	int curX = pos.x();
	int curY = pos.y();
	QVector<QRectF> result;
	for( int i = 0; i < num; i++ )
	{
		if( curX + STEP > Width )
		{
			curY += STEP;
			curX = pos.x();
		}
		result.append( QRectF( curX, curY, STEP, STEP ) );
		curX += STEP;
	}
	return result;
}

QVector<QRectF> SpaceManager::queryPreOpen( QPointF pos, int row, int col )
{
	int Width = this->_scene->width() + pos.x();
	int Height = this->_scene->height() + pos.y();
	int curX = pos.x();
	int curY = pos.y();
	QVector<QRectF> result;
	for( int i = 0; i < row; i++ )
	{
		for ( int j = 0; j < col; j ++ )
		{
			result.append( QRectF( curX, curY, STEP, STEP ) );
			curX += STEP;
		}
		curX = pos.x();
		curY += STEP;
	}
	return result;
}