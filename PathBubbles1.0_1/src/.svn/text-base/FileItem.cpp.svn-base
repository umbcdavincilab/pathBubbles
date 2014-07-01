#include "FileItem.h"

FileItem::FileItem( QString name, int type, OpenManager* open )
: PenItem()
, PREOPEN( false )
, SIMUOPEN( false )
, _font( "times", 12 )
, _metrics( _font )
{
	this->_bounding = QRectF( 0, 0, 40, 60);
	ViewStep = this->boundingRect().height() / 8;
	this->_name = name;
	this->_open = open;
	this->TYPE = type;
	//
	int length = this->_metrics.charWidth( name, 0 );
	return;
}

void FileItem::paint( QPainter *painter )
{
	//*********************************************
	painter->save();
	//*********************************************
	drawPreOpen( painter );
	drawName( painter );
	drawView( painter );
	//*********************************************
	painter->restore();
	//*********************************************
	PenItem::paint( painter );
	return;
}


void FileItem::drawName( QPainter* painter )
{
	painter->save();
	//painter->drawText( this->_fontBounding, this->_name );
	painter->drawText( this->boundingRect().bottomLeft(), this->_name );
	painter->restore();
	return;
}

bool FileItem::contains(QPointF pos)
{
	return this->_bounding.contains( pos );
}

void FileItem::setPos(QPoint pos)
{
	QPointF offset = pos - this->boundingRect().bottomLeft();
	PenItem::setPos( pos );
	this->_preRect = this->boundingRect();
	return;
}

QString FileItem::toQString()
{
	return this->_name;
}

void FileItem::enablePreOpen()
{
	this->PREOPEN = true;
}

void FileItem::disablePreOpen( QPointF pos, bool open )
{
	if( !open )
	{
		this->_open->openDataInViewAt( this->_name, removeOpened( _view ), pos );
	}
	this->PREOPEN = false;
	this->SIMUOPEN = false;
	this->_preRect = this->boundingRect();
}

QList<int> FileItem::removeOpened( QList<int> views )
{
	QList<int> result;
	for( int i = 0; i < views.size(); i++ )
	{
		if( !_openedView.contains( views[i] ) )
		{
			_openedView.append( views[i] );
			result.append( views[i] );
		}
	}
	return result;
}

QList<int> FileItem::removeOpenedForPreOpen( QList<int> views )
{
	QList<int> result;
	for( int i = 0; i < views.size(); i++ )
	{
		if( !_openedView.contains( views[i] ) )
		{
			result.append( views[i] );
		}
	}
	return result;
}

void FileItem::drawPreOpen(QPointF pos, bool flag )
{
	PREOPEN = !flag;
	SIMUOPEN = flag;
	if( !flag )
	{
		_preRect.translate( pos - _preRect.topLeft() );
	}
	else
	{
		int totalNum = this->_open->queryPossibleOpen( this->_name );
		int views = removeOpenedForPreOpen( _view ).size();
		int row, col;
		if ( views < totalNum )
		{
			row = views;
			col = totalNum;
		}
		else
		{
			row = totalNum;
			col = views;
		}
		this->_rectList = this->_open->queryPreOpen( pos, row, col );
	}
}

void FileItem::drawPreOpen( QPainter *painter )
{
	painter->save();
	if( PREOPEN )
	{
		//***************************************************
		painter->setPen( Qt::NoPen );
		painter->setBrush( QBrush( QColor( 200, 0, 0, 40 ) ) );
		painter->drawRect( _preRect );
		//***************************************************
	}
	else if( SIMUOPEN )
	{
		painter->setPen( Qt::NoPen );
		painter->setBrush( QBrush( QColor( 0, 0, 200, 40 ) ) );
		painter->drawRects( this->_rectList );
	}
	painter->restore();
	return;
}

void FileItem::addView( int index, QColor color )
{
	if( !this->_view.contains( index ) )
	{
		QRectF rect = this->boundingRect();
		this->_view.append( index );
		this->_color.append( color );
		_viewRects.append( QRectF( rect.left(), ViewStep * _viewRects.size() + rect.top(), rect.width(), ViewStep ) );
	}
}

void FileItem::drawView(QPainter *painter)
{
	painter->save();
	//******************************************
	for( int i = 0; i < this->_viewRects.size(); i++ )
	{
		//painter->drawText( _viewRects[i], "12345" );
		painter->setPen( Qt::NoPen );
		painter->setBrush( QBrush( _color[i] ) );
		painter->drawRect( _viewRects[i] );
	}
	//******************************************
	painter->restore();
	return;
}