#include "webBlock.h"
#include "openglscene.h"

WebBlock::WebBlock(int x, int y, OpenGLScene *scene, QString target ) : ItemBase( WEBSIZE, x, y )
{
	this->_REMOVECONTROL = true;
	this->_scene = scene;
	_target = target;
	this->_TYPE = WEB;
	this->bIndex = scene->getBubbleIndex();

	_label = NULL;

	initUI();

	return;
}

void WebBlock::logOpen()
{
	ItemBase::logOpen();

	emit logInfor( "\n" );
}

void WebBlock::initUI()
{
	//			init panel
	this->_web = new WebQuery( _target );
	this->_web->resize( WEBSIZE - 20, WEBSIZE - 20 );
	//
	this->_myControl = this->_scene->addWidget( _web );
	return;
}

void WebBlock::mousePressEvent( QGraphicsSceneMouseEvent *event )
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

void WebBlock::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
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

void WebBlock::mouseMoveEvent( QGraphicsSceneMouseEvent *event )
{
	ItemBase::mouseMoveEvent( event );
	return;
}

void WebBlock::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	this->_myControl->resize( this->Width(), this->Height() );
	this->_myControl->setPos( this->pos().x() - this->Width()/2, this->pos().y() - this->Height()/2 );
	ItemBase::paint( painter, option, widget );
}

void WebBlock::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
	ItemBase::hoverMoveEvent( event);
	return;
}

void WebBlock::saveAsXmlNode( QDomDocument &doc, QDomElement &node )
{
	QDomText text;

	QDomElement type = doc.createElement( tr("Type") );
	text = doc.createTextNode( tr("Web") );
	type.appendChild( text );
	node.appendChild( type );

	ItemBase::saveAsXmlNode( doc, node );

	QDomElement path = doc.createElement( tr("Path") );
	text = doc.createTextNode( this->_web->getWebSite() );
	path.appendChild( text );
	node.appendChild( path );
}

void WebBlock::recoverFromXmlNode( QDomElement node )
{
	ItemBase::recoverFromXmlNode( node );
}

void WebBlock::resizeItem( int size1, int size2 )
{
	ItemBase::resizeItem( size1, size2 );
	this->_myControl->resize( size1, size2 );
	_web->setMask( roundRect( QRect(0, 0, m_size_width, m_size_height), 20) );
}