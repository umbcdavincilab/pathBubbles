#include "webBlock.h"
#include "openglscene.h"

WebBlock::WebBlock(int size, int x, int y, OpenGLScene *scene, int type, QString target) : ItemBase( WEBSIZE, x, y )
{//type 1: web type 2: search, type 3: settings
	if(type==1)
	   this->_TYPE = WEB;
	else if(type==2)
		this->_TYPE = SEARCHFIND;
	else if(type==3)
		this->_TYPE = SETTINGS;	
	else if(type==4)
		this->_TYPE = SEARCHSETTINGS;
	else if(type==5)
		this->_TYPE = SEARCHSETTINGS2;

	this->_REMOVECONTROL = true;
	this->_scene = scene;
	_target = target;	
	this->bIndex = scene->getBubbleIndex();
     _scene=scene;
	_label = NULL;	
	initUI(type);
	this->_colorBoarder = TDHelper::getBoarderColorByType(_TYPE);
	this->_colorInner = TDHelper::getInnerColorByType(_TYPE);
	//_dType=type;
	resizeItem( size, size/1.618 ); ///size/2	
}


void WebBlock::logOpen()
{
	ItemBase::logOpen();

	emit logInfor( "\n" );
}

void WebBlock::initUI(int type)
{
	// init panel
	
	this->_web = new WebQuery(_scene, _target);	   
	this->_web->resize( WEBSIZE - 20, WEBSIZE - 20 );
	if(type==1)
		this->_myControl = this->_scene->addWidget( _web );
	
	if(type==2)
	{
		_target = "all";
		this->_search = new SearchFind(_scene, _target, type);	    
		this->_search->_name = _target;
		this->_search->setWebItem(this);	    			    
		this->_myControl = this->_scene->addWidget( _search );
	}
	else if(type==3)
	{
		_target = "all";
		this->_settings = new SearchFind(_scene, _target, type);	    
		this->_settings->_name = _target;
		this->_settings->setWebItem(this);	       
	    this->_myControl = this->_scene->addWidget( _settings );
	}
	else if(type==4)
	{
		_target = "all";
		this->_settings = new SearchFind(_scene, _target, type);	    
		this->_settings->_name = _target;
		this->_settings->setWebItem(this);	       
	    this->_myControl = this->_scene->addWidget( _settings );
	}
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
		//if(_dType==2)
        //   this->_search->_path->setCursorPosition(1);		
		//else if(_dType==3)
		//   this->_search->_path->setCursorPosition(1);		
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
	if(m_isResizing)
		resizeItem( this->Width(), this->Height() );
	return;
}


void WebBlock::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	ItemBase::mouseDoubleClickEvent(event);	
	/*AFTERUNGROUP = false;	
	UNGROUP = false;

	_scene->clearSelected(this);
	_scene->clearExpressed(this);	
	_scene->clearColored(this);
	_scene->clearUndoRedoRecord(this);	
	_scene->hoveredItem.clear();

	_scene->removeToolsinGroup(this, true);*/
	return;
}


void WebBlock::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
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

void WebBlock::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	this->_myControl->resize( this->Width(), this->Height() );
	this->_myControl->setPos( this->pos().x() - this->Width()/2, this->pos().y() - this->Height()/2 );
	QRegion reg=getClipRegionforCalloutNote();
	painter->setClipRegion(reg);
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

void WebBlock::deleteItems()
{
	if(this->_TYPE == SEARCHFIND)
	{
		//this->_search->~SearchFind( );
		this->_web->deleteItems();		
		this->_search->deleteItems();
		delete [] this->_search;
		delete [] this->_web;
	}
	if(this->_TYPE == SETTINGS)
	{
		//this->_settings->~SearchFind( );
		this->_web->deleteItems();
		this->_settings->deleteItems();
		delete [] this->_settings;
		delete [] this->_web;
		
	}	
	if(this->_TYPE == WEB)
	{
		this->_web->deleteItems();
		delete [] this->_web;
	}
    //delete [] this->_myControl;
    //delete widget;	
}