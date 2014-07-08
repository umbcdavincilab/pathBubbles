#include "webBlock.h"
#include "openglscene.h"

WebBlock::WebBlock(int size, int x, int y, OpenGLScene *scene, int type, QString target, int epid, int etype, int eid) : ItemBase( WEBSIZE, x, y )
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
	
	if(_target!="" &&  this->_TYPE == WEB)
	{
		m_eGiftID= _scene->_eGiftBubbles.size();
		_scene->_eGiftBubbles.push_back(this);
		_pid = epid, _type = etype, _id = eid;
	}
	else
	{
		m_eGiftID=-1;
	    _pid=_type=_id=-1; 
	}
}

WebBlock::~WebBlock()
{
	_scene->_eGiftBubbles[m_eGiftID]=NULL;
	m_eGiftID=-1;
}

void WebBlock::drawConnections(QPainter *painter, QPointF dis)
{
	if(_target=="" ||  this->_TYPE != WEB || _pid < 0)
	{
		return; //not a egift query bubbble
	}
	QPen pen;
	//myColor3 color;
	
	vector<int> node;  
	vector<vector<int>> nodes;
	QRectF rect;

	pen.setWidth(2);
	painter->save();

	pen.setColor( BUBBLELINKCOLOR );
	painter->setPen( pen );
	//painter->setBrush( BUBBLELINKCOLOR );
	if(_scene->_pathBubbles[_pid])
	{
	    QPointF end, start;
		switch(_type)
		{
			case 'C': rect = _scene->_pathBubbles[_pid]->complexRect[_id];       break;
			case 'D': rect = _scene->_pathBubbles[_pid]->DnaRect[_id];            break;
			case 'E': rect = _scene->_pathBubbles[_pid]->physicalEntityRect[_id]; break; 
			case 'L': rect = _scene->_pathBubbles[_pid]->ANodeRect[_id];          break; 
			case 'P': rect = _scene->_pathBubbles[_pid]->proteinRect[_id];        break;
			case 'S': rect = _scene->_pathBubbles[_pid]->smallMoleculeRect[_id];  break;			
			case 'R': rect = _scene->_pathBubbles[_pid]->reactionRect[_id];       break;  
		}
		
		rect.moveCenter(rect.center()+_scene->_pathBubbles[_pid]->sceneBoundingRect().center());
		start = rect.center();
		
		float x1=start.x(), y1=start.y(), x2=sceneBoundingRect().center().x(), y2=sceneBoundingRect().center().y();//+sceneBoundingRect().center().y()
        int flag=CSLineClip(x1, y1, x2, y2, rect.left(), rect.right(), rect.top(), rect.bottom());
		if(flag==1)
			start=QPointF(x1,y1);
		else if(flag==2)
			start=QPointF(x2,y2);		
		else if(flag==0) //no intersection
		{			
		}	
		else 
			start=QPointF(x2,y2);
				
		getLinkPoint(start, end, dis, _scene->_pathBubbles[_pid]->sceneBoundingRect(), sceneBoundingRect());	
	    drawABubbleConnection(painter, start, _scene->_pathBubbles[_pid], end, this, dis);	
        
		pen.setWidth(2);
		pen.setColor(MEDIUM_HIGHLIGHTCOLOR);
		painter->setPen( pen );
		painter->setBrush( Qt::NoBrush);
		painter->drawRoundedRect( rect, 6, 6 );  				
		
		//getLinkPoint(start1, end1, dis, sceneBoundingRect(), bubble->sceneBoundingRect());
		//drawABubbleConnection(painter, start2, this, end2, bubble, dis);
	}
	//_pid = epid, _type = etype, _id = eid;	
	//for (int i = 0; i <_connections.size(); i ++)
	/*{
		struct treeRingConnector node=_connections[i];
		int layer,idx; //start
		QPointF start1, end1, start2, end2, tmpmid;
		ItemBase *bubble=node.childBubble;
		
		
		layer=node.layer; idx=node.idx;
		if(layer<0)
			continue;
		
		QRectF sRect = realRect();
		sRect.moveCenter(sceneBoundingRect().center());
		QPolygonF iRect = sRect;
		QPolygon fRect=iRect.toPolygon();
		if(bubble->getType()==TREERING)
		{
			vector<QPolygon> segPoints;
			vector<int> cornerNode_2; //record the other corner point
			getStartSegsNodes(node.itemSelected, segPoints,  cornerNode_2, this->pos());
			
			for(int j=0; j<segPoints.size(); j++)
			{
				pen.setColor( MEDIUM_HIGHLIGHTCOLOR);
				painter->setPen( pen );

				int cSize = segPoints[j].size();

				painter->drawPolygon(segPoints[j].intersected(fRect));

				pen.setColor( BUBBLELINKCOLOR );
				painter->setPen( pen );

				float x, y, x1, y1, x2, y2;
				//if(cornerNode_2[1]>=segPoints[j].size())
				//	j=j;
				x1=segPoints[j][cornerNode_2[j*2]].x(); //(_treeRing->_ring[layer]._pos[idx][0].x + _treeRing->_ring[layer]._pos[idx][1].x + _treeRing->_ring[layer]._pos[idx][2].x + _treeRing->_ring[layer]._pos[idx][3].x)/4;
				y1=segPoints[j][cornerNode_2[j*2]].y(); //(_treeRing->_ring[layer]._pos[idx][0].y + _treeRing->_ring[layer]._pos[idx][1].y + _treeRing->_ring[layer]._pos[idx][2].y + _treeRing->_ring[layer]._pos[idx][3].y)/4;
		
				x2=segPoints[j][cornerNode_2[j*2+1]].x(); //(_treeRing->_ring[layer]._pos[idx][0].x + _treeRing->_ring[layer]._pos[idx][1].x + _treeRing->_ring[layer]._pos[idx][2].x + _treeRing->_ring[layer]._pos[idx][3].x)/4;
				y2=segPoints[j][cornerNode_2[j*2+1]].y(); //(_treeRing->_ring[layer]._pos[idx][0].y + _treeRing->_ring[layer]._pos[idx][1].y + _treeRing->_ring[layer]._pos[idx][2].y + _treeRing->_ring[layer]._pos[idx][3].y)/4;
		
				start1=QPointF(x1, y1);
				start2=QPointF(x2, y2);
				//start = start + this->pos();		
				getLinkPoint(start1, end1, dis, sceneBoundingRect(), bubble->sceneBoundingRect());
				getLinkPoint(start2, end2, dis, sceneBoundingRect(), bubble->sceneBoundingRect());
				QPointF D1=start1-end1, D2=start2-end2;
			
				if(D1.x()*D1.x() + D1.y()*D1.y() <= D2.x()*D2.x() + D2.y()*D2.y())
					drawABubbleConnection(painter, start1, this, end1, bubble, dis);
				else
					drawABubbleConnection(painter, start2, this, end2, bubble, dis);
			}
		}
	    else if(bubble->getType()==SUBPATHBUBBLE1 || bubble->getType()== PATHBUBBLE1)
		{
			int cornerID1, cornerID2; 
			//vector<int> item = *node.itemSelected.begin();
			QPolygon segPoint = _treeRing->getSegPoint(node.layer, node.layer, node.idx, node.idx, this->pos().x(), this->pos().y(), cornerID1, cornerID2);	
			
			
			pen.setColor( MEDIUM_HIGHLIGHTCOLOR);
			painter->setPen( pen );
			
			painter->drawPolygon(segPoint.intersected(fRect));

			pen.setColor( BUBBLELINKCOLOR );
			painter->setPen( pen );

	        float x, y, x1, y1, x2, y2;
			x1=segPoint[cornerID1].x();//(_treeRing->_ring[layer]._pos[idx][0].x + _treeRing->_ring[layer]._pos[idx][1].x + _treeRing->_ring[layer]._pos[idx][2].x + _treeRing->_ring[layer]._pos[idx][3].x)/4;
			y1=segPoint[cornerID1].y();//(_treeRing->_ring[layer]._pos[idx][0].y + _treeRing->_ring[layer]._pos[idx][1].y + _treeRing->_ring[layer]._pos[idx][2].y + _treeRing->_ring[layer]._pos[idx][3].y)/4;
		
			x2=segPoint[cornerID2].x();//(_treeRing->_ring[layer]._pos[idx][0].x + _treeRing->_ring[layer]._pos[idx][1].x + _treeRing->_ring[layer]._pos[idx][2].x + _treeRing->_ring[layer]._pos[idx][3].x)/4;
			y2=segPoint[cornerID2].y();//(_treeRing->_ring[layer]._pos[idx][0].y + _treeRing->_ring[layer]._pos[idx][1].y + _treeRing->_ring[layer]._pos[idx][2].y + _treeRing->_ring[layer]._pos[idx][3].y)/4;
				
			start1=QPointF(x1, y1);
			start2=QPointF(x2, y2);

			getLinkPoint(start1, end1, dis, sceneBoundingRect(), bubble->sceneBoundingRect());
			getLinkPoint(start2, end2, dis, sceneBoundingRect(), bubble->sceneBoundingRect());
			QPointF D1=start1-end1, D2=start2-end2;
			if(D1.x()*D1.x() + D1.y()*D1.y() <= D2.x()*D2.x() + D2.y()*D2.y())
				drawABubbleConnection(painter, start1, this, end1, bubble, dis);
			else
				drawABubbleConnection(painter, start2, this, end2, bubble, dis);
		}
	}	*/
	painter->restore();
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