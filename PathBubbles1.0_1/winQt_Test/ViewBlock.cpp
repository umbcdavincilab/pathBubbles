#include "ViewBlock.h"

ViewBlock::ViewBlock(int x, int y, OpenGLScene *scene)
: ItemBase( VIEWSIZE, x, y )
, _ONDRAD( false )
, _currentIndex( -1 )
{
	this->_scene = scene;
	_marginFromTop = 20;
	// hard add some data
	this->_view.append( tr("3D") );
	this->_view.append( tr("PC1") );
	this->_view.append( tr("PC2") );
	this->_view.append( tr("PC3") );
	this->_view.append( tr("Video1") );
	this->_view.append( tr("Video2") );
	this->_view.append( tr("Video3") );
	//
	this->_gridWidth = VIEWSIZE / 3;
	this->_gridHeight = (VIEWSIZE - 2*_marginFromTop )/3;
	this->_sizeOff = VIEWSIZE / 2;
	//
	initGrid();
	return;
}

void ViewBlock::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	painter->save();
	//*****************************************************************
	drawRects( painter );
	if( _ONDRAD )
	{
		painter->setBrush( QColor( 0, 0, 255, 100 ) );
		painter->drawRect( _dragRect );
	}
	//*****************************************************************
	painter->restore();
	ItemBase::paint( painter, option, widget );
	return;
}

void ViewBlock::drawRects(QPainter *painter)
{
	painter->drawRects( _rects );
	for( int i = 0; i < this->_rects.size(); i++ )
	{
		painter->drawText( this->_rects[i].bottomLeft(), this->_view[i] );
	}
	return;
}

void ViewBlock::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if( onDrag( event->pos() ) > -1 )
	{
		_currentIndex = onDrag( event->pos() );
		_dragRect = _rects[_currentIndex];
		this->_ONDRAD = true;
	}
	ItemBase::mousePressEvent( event );
	return;
}

void ViewBlock::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if( _ONDRAD )
	{
		_dragRect.translate( event->pos() - event->lastPos() );
		return;
	}
	ItemBase::mouseMoveEvent( event );
	return;
}

void ViewBlock::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	if( this->_ONDRAD )
	{
		_currentIndex = -1;
		this->_ONDRAD = false;
	}
	ItemBase::mouseReleaseEvent( event );
	return;
}

void ViewBlock::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
	ItemBase::hoverMoveEvent( event );
	return;
}

void ViewBlock::initGrid()
{
	for( int i = 0; i < this->_view.size(); i++ )
	{
		this->_rects.append( QRectF( i%3*_gridWidth-_sizeOff, i/3*_gridHeight+_marginFromTop-_sizeOff, _gridWidth, _gridHeight ) );
	}
	return;
}

int ViewBlock::onDrag(QPointF pos)
{
	for( int i = 0; i < this->_rects.size(); i++ )
	{
		if( _rects[i].contains( pos ) )
		{
			return i;
		}
	}
	return -1;
}