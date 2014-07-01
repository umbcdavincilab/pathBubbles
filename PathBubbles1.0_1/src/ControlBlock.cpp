#include "ControlBlock.h"
#include "openglscene.h"

ControlBlock::ControlBlock( int x, int y, OpenGLScene* scene )
: ItemBase( CONTROLSIZE, x, y )
, _onBoarder( false )
, _zoomFactor( 0 )
, _cross( QString( tr("icon/cross.png") ) )
, _heightPercent( 0.0 )
, _up( QString( tr("icon/up.png") ) )
, _down( QString( tr("icon/down.png") ) )
, TRANSLATE( false )
, UPANDDOWN( false ) 
, _offset( 0, 0 )
{
	this->_TYPE = CON;
	//
	initManager();
	return;
}

void ControlBlock::initManager()
{
	this->_manager = new InstanceManager();
	//*************************************************
	//			add more and more BubbleInstances
	//*************************************************
	this->_init = new InstanceInitializer();
	this->_init->initInstance( this->_manager );
	//*************************************************
	this->_manager->initTree();
}

void ControlBlock::drawOrder( QPainter* painter )
{
	painter->save();
	painter->setPen( Qt::NoPen );
	painter->setBrush( QBrush( QColor( 0, 0, 200, 200 ) ) );
	painter->restore();
}

void ControlBlock::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{ 
	//this->_manager->getRoot()->paint( painter, this->boundingRect() );
	this->_manager->renderRoot( painter, getPaintArea(), getBoundingArea() );
	this->paintSideBar( painter );
	drawOrder( painter );
	//this->_manager->renderRoot( painter, QRectF( -200, -200, 400, 400 ) );
	ItemBase::paint( painter, option, widget );
	return;
}

void ControlBlock::paintSideBar( QPainter* painter )
{
	int curPos = ( this->Height() - 5*CORESSIZE ) * this->_heightPercent;
	QRectF upRect( QPoint( -this->Width()/2 + 10, curPos - this->Height()/2 ), QSize( CORESSIZE, CORESSIZE ) );
	QRectF crossRect( QPoint( -this->Width()/2 + 10, curPos - this->Height()/2 + CORESSIZE ), QSize( CORESSIZE, CORESSIZE ) );
	QRectF downRect( QPoint( -this->Width()/2 + 10, curPos - this->Height()/2 + CORESSIZE*2 ), QSize( CORESSIZE, CORESSIZE ) );
	_currentCross = crossRect;
	_currentUp = upRect;
	_currentDown = downRect;
	painter->drawImage( upRect, this->_up, QRectF( QPointF( 0, 0 ), this->_up.size() ) );
	painter->drawImage( crossRect, this->_cross, QRectF( QPointF( 0, 0 ), this->_cross.size() ) );
	painter->drawImage( downRect, this->_down, QRectF( QPointF( 0, 0 ), this->_down.size() ) );
	return;
}

void ControlBlock::wheelEvent( QGraphicsSceneWheelEvent * event )
{
	if( event->delta() > 0 )
	{
		zoomIn();
	}
	else
	{
		zoomOut();
	}
	return;
}

void ControlBlock::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if( onUpAndDown( event->pos() ) )
	{
		UPANDDOWN = true;
	}
	else if( onTranslate( event->pos() ) )
	{
		TRANSLATE = true;
	}
	else if( this->onBoarder( event->pos() ) )
	{
		this->_onBoarder = true;
	}
	else
	{
		this->_manager->getRoot()->enableDrag();
	}
	_initDragPos = event->pos();
	ItemBase::mousePressEvent( event );
	return;
}

void ControlBlock::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if( this->_onBoarder )
	{
		ItemBase::mouseMoveEvent( event );
	}
	else if( UPANDDOWN == true )
	{
		updateUpAndDown( event->pos().y() - event->lastPos().y() );
	}
	else if( TRANSLATE == true )
	{
		_offset.setX( _offset.x() + event->pos().x() - event->lastPos().x() );
		_offset.setY( _offset.y() + event->pos().y() - event->lastPos().y() );
	}
	else
	{
		this->_manager->getRoot()->drawInitViewPos( event->pos() - _initDragPos );
	}
}

void ControlBlock::updateUpAndDown( int index )
{
	this->_heightPercent += float(index)/ float(this->Height());
	if( this->_heightPercent > 1.0 || this->_heightPercent < 0.0 )
	{
		this->_heightPercent -= float(index)/ float(this->Height());
	}
}

void ControlBlock::mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
{
	this->_onBoarder = false;
	UPANDDOWN = false;
	TRANSLATE = false;
	this->_manager->getRoot()->disableDrag();
	ItemBase::mouseReleaseEvent( event );
}

void ControlBlock::hoverMoveEvent( QGraphicsSceneHoverEvent *event)
{
	//			code for highlighting
	if( !this->onSliderBar( event->pos() ) )
	{
		this->_manager->highLightInstance( event->pos() );
	}
	//
	ItemBase::hoverMoveEvent( event );
}

bool ControlBlock::onSliderBar( QPointF pos )
{
	return !getBoundingArea().contains( pos );
}

QRectF ControlBlock::getPaintArea()
{
	int ScaleFactor = this->Height() * _heightPercent * 2;
	QRectF bounding = this->boundingRect();
	bounding.setLeft( bounding.left() - ScaleFactor );
	bounding.setRight( bounding.right() + ScaleFactor );
	bounding.setTop( bounding.top() - ScaleFactor );
	bounding.setBottom( bounding.bottom() + ScaleFactor );
	bounding.setX( bounding.x() + CONTROLMARGIN );
	bounding.translate( this->_offset.x(), this->_offset.y() );
	return bounding;
}

QRectF ControlBlock::getBoundingArea()
{
	QRectF bounding = this->boundingRect();
	bounding.setX( bounding.x() + CONTROLMARGIN );
	return bounding;
}

void ControlBlock::zoomByFloat()
{
	this->_zoomFactor = this->Height() * _heightPercent * 2;
	return;
}

void ControlBlock::zoomIn()
{
	this->_zoomFactor += 10;
	return;
}

void ControlBlock::zoomOut()
{
	this->_zoomFactor -= 10;
	return;
}

bool ControlBlock::onTranslate(QPointF pos)
{
	return this->_currentCross.contains( pos );
}

bool ControlBlock::onUpAndDown(QPointF pos)
{
	if( _currentUp.contains( pos ) || _currentDown.contains( pos ) )
	{
		return true;
	}
	else
	{
		return false;
	}
}