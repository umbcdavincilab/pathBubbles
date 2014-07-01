#include "ControlBubble.h"
#include "openglscene.h"

ControlBubble::ControlBubble(int x, int y, OpenGLScene *scene)
: ItemBase( CONTROL, x, y )
, _lastSize( CONTROL )
, _lastPos( x, y )
{
	this->_scene = scene;
	//initUI();
	return;
}

ControlBubble::~ControlBubble()
{
}

void ControlBubble::initUI()
{
	QWidget* widget = TDHelper::createDialog( tr("") );
	widget->resize( CONTROL, CONTROL );
	//
	this->_myControl = this->_scene->addWidget( widget );
	return;
}

void ControlBubble::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
	if( onBoarder( event->pos() ) )
	{
		ItemBase::mousePressEvent( event );
	}
	else
	{
		event->setAccepted( false );
	}
	return;
}

void ControlBubble::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	if( onBoarder( event->pos() ) )
	{
		ItemBase::mouseReleaseEvent( event );
	}
	else
	{
		event->setAccepted( false );
	}
	//
	return;
}

void ControlBubble::mouseMoveEvent( QGraphicsSceneMouseEvent *event )
{
	ItemBase::mouseMoveEvent( event );
	return;
}

void ControlBubble::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	//if( this->_lastSize == this->Size() )
	//{
	this->_myControl->resize( this->Width(), this->Height() );
		//this->_lastSize = this->Size();
	//}
	//if( this->_lastPos == this->pos() )
	//{
	this->_myControl->setPos( this->pos().x() - this->Width()/2, this->pos().y() - this->Height()/2 );
		//this->_lastPos = this->pos();
	//}
	
	ItemBase::paint( painter, option, widget );
}

void ControlBubble::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
	ItemBase::hoverMoveEvent( event);
	return;
}

void ControlBubble::myHide()
{
	this->_myControl->hide();
	this->setVisible( false );
}
void ControlBubble::myShow()
{
	this->_myControl->show();
	this->setVisible( true );
}