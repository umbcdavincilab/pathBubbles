#include <QTextEdit>
#include "openglscene.h"
#include "NoteBlock.h"

NoteBlock::NoteBlock( int size, int x, int y, OpenGLScene* Scene ):ItemBase(size, x, y)
{

	this->_TYPE = NOTE;
	initUI( Scene );
	_scene = Scene;
	this->bIndex = _scene->getBubbleIndex();
	this->_myControl->setPos( x - size/2, y - size/2 );

	this->_colorBoarder = TDHelper::getBoarderColorByType(_TYPE);
	this->_colorInner = TDHelper::getInnerColorByType(_TYPE);

	_REMOVECONTROL = true;
}

void NoteBlock::logOpen()
{
	ItemBase::logOpen();

	emit logInfor("\n");
}

void NoteBlock::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
	if(insideBubble(event->pos()))
	{
	   UNGROUP=false;
	   AFTERUNGROUP=false;
	}
	if(AFTERUNGROUP || m_isMoving)
	{
		m_isMoving=false;
	   return;
	}
	if (!isSelected()) 
	{
		_scene->clearSelection();
		setSelected(true);
	}
	vector<int> node;
	_scene->dropDownMenu0(event, this, node);
	return;
	
}

void NoteBlock::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	/*ItemBase::mouseDoubleClickEvent(event);	
	AFTERUNGROUP = false;	
	UNGROUP = false;

	_scene->clearSelected(this);
	_scene->clearExpressed(this);	
	_scene->clearColored(this);
	_scene->clearUndoRedoRecord(this);	
	_scene->hoveredItem.clear();

	_scene->removeToolsinGroup(this, true);*/
	return;
}

void NoteBlock::initUI( OpenGLScene* Scene )
{
	this->m_note = new QTextEdit();
	m_note->setStyleSheet( "background-color: rgb(215, 200, 145)");
	m_note->setMask( this->roundRect( QRect(0, 0, m_size_width, m_size_height), 20 ) );
	this->_myControl = Scene->addWidget( m_note );
}

void NoteBlock::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
	this->_myControl->resize( this->Width(), this->Height() );
	this->_myControl->setPos( this->pos().x() - this->Width()/2, this->pos().y() - this->Height()/2 );	
	
	QRegion reg=getClipRegionforCalloutNote();
	painter->setClipRegion(reg);
	
	ItemBase::paint( painter, option, widget );
}

void NoteBlock::mouseMoveEvent( QGraphicsSceneMouseEvent *event )
{
	ItemBase::mouseMoveEvent( event );
	if(m_isResizing)
		resizeItem( this->Width(), this->Height() );
	return;
}

void NoteBlock::resizeItem( int size1, int size2 )
{
	ItemBase::resizeItem( size1, size2 );
	this->_myControl->resize( size1, size2 );
	m_note->setMask( roundRect(QRect(0, 0, m_size_width, m_size_height), 20 ) );

}

void NoteBlock::mousePressEvent( QGraphicsSceneMouseEvent *event )
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

void NoteBlock::mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
{
	if( onBoarder( event->pos() ) )
	{
		ItemBase::mouseReleaseEvent( event );
	}
	else
	{
		event->setAccepted( false );
	}
	return;
}

void NoteBlock::saveAsXmlNode( QDomDocument &doc, QDomElement &node )
{
	QDomText text;
	QString str, tmpStr;

	QDomElement type = doc.createElement( tr("Type") );
	text = doc.createTextNode( tr("Note") );
	type.appendChild( text );
	node.appendChild( type );

	ItemBase::saveAsXmlNode( doc, node );

	QDomElement content = doc.createElement( tr("Content") );
	str = m_note->toPlainText();
	text = doc.createTextNode( str );
	content.appendChild( text );
	node.appendChild( content );
}

void NoteBlock::recoverFromXmlNode( QDomElement node )
{
	QDomNodeList paraNode;
	QString str;

	paraNode = node.elementsByTagName( tr("Content") );
	str = paraNode.at(0).toElement().text();

	m_note->setText( str );
}