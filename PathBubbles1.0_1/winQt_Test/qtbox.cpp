#include "Pathbubble1.h"



const qreal ROTATE_SPEED_X = 30.0 / 1000.0;
const qreal ROTATE_SPEED_Y = 20.0 / 1000.0;
const qreal ROTATE_SPEED_Z = 40.0 / 1000.0;

//============================================================================//
//                                  ItemBase                                  //
//============================================================================//

ItemBase::ItemBase(int size, int x, int y) 
	: m_size_width(size)
	, m_size_height( size )
, m_isResizing( false )
, m_isMoving(false)
, m_rightButton(true)
, m_isInnerMoving(false)
, _Base_ONMOVE( false )
, _Base_ONCONTROL( false )
, _TYPE( -1 )
, _initSize( size )
, _initPos( x, y )
, _highLight( false )
, _REMOVECONTROL( false )
, _CANBESHIFTED( true )
, CONTROLLOCK( true )
, LASTOPENSTATE( false )
, UNGROUP( false )
, UNGROUPBYMENU(false)
, DELEGATE( false )
, AFTERUNGROUP(false)
, boarderLeft( true )
, boarderRight( true )
, boarderUp( true )
, boarderDown( true )
, _penisOn(false)
, _grabisOn(false)
, _queryisOn(false)
, _upisOn(false)
, _downisOn(false)
, groupSelected(false)
, m_pathwayID(-1)
, m_codeBubbleID(-1)
, _parentPathBubbleID(-1)
, _labelInitiated(false)
, _colorBoarderAlpha(255)
, _evenisOn(false)
, fixedSize(true)
, resizeFinished(true)
, resizeMode(2)
, referH(18)
, deleted(false)
, pressOnBoarder(false)
, pressOnUngroup(false)
, initWidth(250)
,_ScaleBackup(1.0)
, _ScaleBackup_S(1.0)
{
	//initializeGL();
	_scene=NULL;
	_color.a=-1;
	setFlag(QGraphicsItem::ItemIsMovable, true);
	setFlag(QGraphicsItem::ItemIsSelectable, true);
	setFlag(QGraphicsItem::ItemIsFocusable, true);	
	setAcceptHoverEvents(true);
	mySetPos( QPointF(x, y) );
	//this->_colorBoarder = TDHelper::getBoarderColorByType(_TYPE);	
	m_startTime = QTime::currentTime();
	
	QDesktopWidget *mydesk = QApplication::desktop () ;
    screenWidth=mydesk->screenGeometry().width(); screenHeight=mydesk->screenGeometry().height();
    //width=mydesk->widthMM();
    //return QSize( width,height);
	//_oriPos = QRectF(x,y,size,size);
	//_scene->_pathBubbles[p1]->resetOriRect();
	_label = NULL;
}


ItemBase::~ItemBase()
{
	if( this->needRemoveControl() )
	{
		this->_myControl->hide();
		//delete this->_myControl;
	}
}

void ItemBase::finishInit()
{
	return;
}

void ItemBase::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{

}

void ItemBase::resetOriRect()
{
	_oriRect=QRectF(sceneBoundingRect().x(), sceneBoundingRect().y(), sceneBoundingRect().width(), sceneBoundingRect().height()); //never change
	_curScale=1.0; //never change	
}

void ItemBase::scaleUpBasedonOriRect() // resize navi
{
	_curScale=_curScale*1.10;
	QRectF rect=QRectF(_oriRect.x()*_curScale, _oriRect.y()*_curScale, _oriRect.width()*_curScale, _oriRect.height()*_curScale); //never change
	mySetPos( QPointF(rect.center().x(), rect.center().y()));
	resizeItem( rect.width(), rect.height() );
	emit itemResized(this, _oriRect); //catch this later
}

void ItemBase::scaleDownBasedonOriRect() //resize navi
{
	_curScale=_curScale/1.10;
	QRectF rect=QRectF(_oriRect.x()*_curScale, _oriRect.y()*_curScale, _oriRect.width()*_curScale, _oriRect.height()*_curScale); //never change
	mySetPos( QPointF(rect.center().x(), rect.center().y()));
	resizeItem( rect.width(), rect.height() );
	emit itemResized(this, _oriRect); //catch this later
}

QRectF ItemBase::boundingRect() const
{
	return QRectF(-m_size_width / 2 - 20, -m_size_height / 2 - 20, m_size_width + 40, m_size_height + 40);
}

QRect ItemBase::realRect()
{
	return QRect( -m_size_width / 2, -m_size_height / 2, m_size_width, m_size_height );
}

QRectF ItemBase::realRectF()
{
	return QRectF( -m_size_width / 2, -m_size_height / 2, m_size_width, m_size_height );
}

QRegion ItemBase::roundRect(QRect &rect, int radius)
{
	QRegion reg;

	reg += rect.adjusted( radius, 0, -radius, 0 );
	reg += rect.adjusted( 0, radius, 0, -radius );

 	QRect cc(rect.topLeft(), QSize(2*radius, 2*radius));
 	QRegion corner( cc, QRegion::Ellipse );
 	reg += corner;
 	corner.translate( rect.width()-2*radius, 0 );
 	reg += corner;
 	corner.translate( 0, rect.height()-2*radius);
 	reg += corner;
 	corner.translate( -rect.width()+2*radius, 0 );
 	reg += corner;

	return reg;
}

bool ItemBase::onHalo(  QPointF pos )
{
	if( pos.x() > m_size_width/2 || pos.x() < -m_size_width/2 )
	{
		return true;
	}
	if( pos.y() > m_size_height/2 || pos.y() < -m_size_height/2 )
	{
		return true;
	}
	else
	{
		return false;
	}
}

void ItemBase::mySetPos(QPointF pos)
{
	QPointF offset = pos - this->pos();
	this->_initPos = this->pos();
	QPointF Point=this->pos();
	  
	this->setPos( pos );
	this->_targetPos = pos;
	this->newPos = pos;
	
	if(_scene!=NULL && ( this->_TYPE==PATHBUBBLE1 || this->_TYPE==SUBPATHBUBBLE1))
	for( int i = 0; i <_scene->_nodeNote.size(); i++ )
	{
		int pid=_scene->_nodeNote[i]->_pid;
		if( !_scene->_nodeNote[i]->deleted )
		{
			if(pid==this->m_pathwayID)
			{
				//QPointF offset=_scene->_nodeNote[i]->scenePos() - this->_initPos;
				_scene->_nodeNote[i]->mySetPos(_scene->_nodeNote[i]->pos() +offset );				
			}
		}
	}	
	return;
}

void ItemBase::movePosFromNavigationBar(QPointF pos)
{
	QPointF offset=pos - this->pos();
	this->_initPos = this->pos();
	QPointF Point=this->pos();
	  
	this->setPos( pos );
	this->_targetPos = pos;
	this->newPos = pos;
	
	if(_scene!=NULL && ( this->_TYPE==PATHBUBBLE1 || this->_TYPE==SUBPATHBUBBLE1))
	for( int i = 0; i <_scene->_nodeNote.size(); i++ )
	{
		int pid=_scene->_nodeNote[i]->_pid;
		if( !_scene->_nodeNote[i]->deleted )
		{
			if(pid==this->m_pathwayID)
			{
				//QPointF offset=_scene->_nodeNote[i]->scenePos() - this->_initPos;
				_scene->_nodeNote[i]->mySetPos(_scene->_nodeNote[i]->pos() +offset);				
			}
		}
	}	
	_scene->getGroup(this)->moveOtherMember(this, offset);
	return;
}
void ItemBase::setTargetPos( QPointF pos, QPointF stepSize, bool synchronizedSpeed)
{	
    _moveStepSize = stepSize;
	_synchronizedSpeed = synchronizedSpeed;
	_targetPos = pos;

	if(_scene!=NULL && ( this->_TYPE==PATHBUBBLE1 || this->_TYPE==SUBPATHBUBBLE1))
	for( int i = 0; i <_scene->_nodeNote.size(); i++ )
	{
		int pid=_scene->_nodeNote[i]->_pid;
		if( !_scene->_nodeNote[i]->deleted )
		{
			if(pid==this->m_pathwayID)
			{
				_scene->_nodeNote[i]->setTargetPos(pos + _scene->_nodeNote[i]->scenePos()-this->pos(), stepSize, synchronizedSpeed);
			}
		}
	}
}

int ItemBase::getCalloutNote(QPointF pos)
{  
	float w, h;
	
	QRegion reg;	
	if(_scene==NULL)
		return -1;
	for(int i=0; i<_scene->_nodeNote.size(); i++)
	{
		CalloutNote *note=_scene->_nodeNote[i];
		if(note!=NULL && note->isVisible() && !note->deleted)
		{
			QRectF bRect=note->sceneBoundingRect();
			if(note->sceneBoundingRect().contains(pos))			
			{
				note->setZValue(this->zValue()+1);
				return i;
			}
		}
	}	
	return -1;
}

QRegion ItemBase::getClipRegionforCalloutNote()
{   //plus fileselector
	//PathBubble1* tmp = dynamic_cast<PathBubble1 *> (this);
	float w, h;
	QRegion reg;		
	reg += realRect();	
	if(_scene==NULL)
		return reg;

	for(int i=0; i<_scene->_nodeNote.size(); i++)
	{
		CalloutNote *note=_scene->_nodeNote[i];
		if(note!=NULL && note->isVisible() && !note->deleted)
		{
			bool flag = TDHelper::overlaid(this->sceneBoundingRect(), note->sceneBoundingRect());
			if(flag)
			{
				QPointF dis=note->scenePos()-this->scenePos();
				QRectF noteRect = note->realRect();
				
				noteRect.moveCenter(noteRect.center()+dis);
				w=noteRect.width(); h=noteRect.height();
				w=w+4, h=h+4; //6/2= border width
				//reg -= QRect(noteRect.x(), noteRect.y(), noteRect.width(), noteRect.height());				
				//reg -= roundRect( QRect(noteRect.center().x() - w/2, noteRect.center().y() - h/2, w, h), 20);		
				reg -= QRect(noteRect.center().x() - w/2, noteRect.center().y() - h/2, w, h);		
			    
				note->setZValue(this->zValue()+1);
				
			}
		}
	}	
	if(_scene->_fileSelection!=NULL)
	{
		QPointF dis=_scene->_fileSelection->getDiaPositon()-this->scenePos();
		QRectF fileRect=_scene->_fileSelection->getDiaRect();
		fileRect.moveCenter(fileRect.center()+dis);
		w=fileRect.width(), h=fileRect.height();
		reg -= QRect(fileRect.center().x() - w/2, fileRect.center().y() - h/2, w, h);
	}	
	return reg;
}

QRegion ItemBase::getCalloutNoteRectforNote()
{   //scene
	//PathBubble1* tmp = dynamic_cast<PathBubble1 *> (this);	
	float w, h;
	QRegion reg;		
	if(_scene==NULL || _TYPE!=CALLOUT)
		return reg;
	for(int i=0; i<_scene->_nodeNote.size(); i++)
	{
		CalloutNote *note=_scene->_nodeNote[i];
		if(note!=NULL && note->isVisible() && !note->deleted)
		{
			QPointF dis=note->scenePos();
			QRectF noteRect = note->realRect();
			noteRect.moveCenter(noteRect.center()+dis);
			w=noteRect.width(); h=noteRect.height();
			w=w+6, h=h+6; 					
			reg = reg + QRect(noteRect.center().x() - w/2, noteRect.center().y() - h/2, w, h);								
		}		
	}
	return reg;
}


QRegion ItemBase::getCalloutNoteRect()
{
	//PathBubble1* tmp = dynamic_cast<PathBubble1 *> (this);	
	float w, h;
	QRegion reg;		
	if(_scene==NULL)
		return reg;
	for(int i=0; i<_scene->_nodeNote.size(); i++)
	{
		CalloutNote *note=_scene->_nodeNote[i];
		if(note!=NULL && note->isVisible() && !note->deleted)
		{
			bool flag = TDHelper::overlaid(this->sceneBoundingRect(), note->sceneBoundingRect());
			if(flag)
			{
				QPointF dis=note->scenePos()-this->scenePos();
				QRectF noteRect = note->realRect();
				noteRect.moveCenter(noteRect.center()+dis);
				w=noteRect.width(); h=noteRect.height();
				w=w+6, h=h+6; 					
				reg = reg + QRect(noteRect.center().x() - w/2, noteRect.center().y() - h/2, w, h);					
			}
		}		
	}
	return reg;
}

void ItemBase::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{	
	painter->save();
	painter->setRenderHint(QPainter::Antialiasing, true);
	painter->setPen( Qt::NoPen );

	
	painter->setBrush(QColor(_colorInner.a,this->_colorInner.b,this->_colorInner.c,UNGROUPBYMENU?255:228));
	 //painter->setBrush(QColor(255, 255, 255, 255));

	if(this->_TYPE!=PATHBUBBLE1 && this->_TYPE!=SUBPATHBUBBLE1 && this->_TYPE!=EXPRESSIONBUBBLE && this->_TYPE!=TREERING)//||this->_TYPE==TEXTBUBBLE2||this->_TYPE==REACTIONBUBBLE3||this->_TYPE==REACTIONBUBBLE3)
		painter->setBrush(Qt::NoBrush);

	QRegion reg=getClipRegionforCalloutNote();

	if(!reg.isEmpty())
		painter->setClipRegion(reg);
		
	if(this->_TYPE==REACTIONBUBBLE3)
	{		
        float cx,cy,h2,w2,h,w;
		cx=this->realRect().center().x(); 
		cy=this->realRect().center().y();
		h=this->realRect().height()/2.0;
		w=this->realRect().width()/2.0;
		h2=h-8; w2=w-8; 
		 
		QVector<QPoint> points;	         
		points.append(QPoint(cx-w2, cy-h));
		points.append(QPoint(cx-w, cy-h2));
		points.append(QPoint(cx-w, cy+h2));
		points.append(QPoint(cx-w2, cy+h));
		points.append(QPoint(cx+w2, cy+h));
		points.append(QPoint(cx+w, cy+h2));
		points.append(QPoint(cx+w, cy-h2));
		points.append(QPoint(cx+w2, cy-h));
		QPolygon whole(points);
		painter->drawPolygon(whole);				
	}   
	else 
	{
		//painter->drawRect( realRect());
		painter->drawRect( QRect(realRect().x()+1, realRect().y()+1, realRect().width()-2, realRect().height()-2));
	}
	
	if (option->state & QStyle::State_Selected) 
	{
		painter->setPen( QColor( _colorBoarder.a, _colorBoarder.b, _colorBoarder.c, 200 ) );
		drawControlHint( painter );
		painter->drawRoundedRect( realRect(), 20, 20 );
		painter->setBrush( QColor( _colorBoarder.a, _colorBoarder.b, _colorBoarder.c, 100 ) );
		_color=_colorBoarder;
		_colorBoarderAlpha=100;
		painter->setPen( Qt::NoPen );
		drawHalo( painter );
	}
	else
	{
		if( this->_highLight) 
		{
			//painter->setBrush( QColor( _hiColor.a, _hiColor.b, _hiColor.c, 100 ) );
			painter->setBrush( QColor( _colorBoarder.a, _colorBoarder.b, _colorBoarder.c, 100 ) );
			_colorBoarderAlpha=100;
			_color=_colorBoarder;
			drawHalo( painter );
		}		
		else
		{
			painter->setBrush( QColor( _colorBoarder.a, _colorBoarder.b, _colorBoarder.c, 220 ) );		
			_colorBoarderAlpha=220;
			_color=_colorBoarder;
			drawHalo( painter );
		}
		// draw a halo
	}
	
	if( this->_TYPE==SUBPATHBUBBLE1 || this->_TYPE==PATHBUBBLE1 )//||this->_TYPE==TEXTBUBBLE2||this->_TYPE==REACTIONBUBBLE3||this->_TYPE==REACTIONBUBBLE3)
	{
		vector<int> item;
		PathBubble1* tmp = dynamic_cast<PathBubble1 *> (this);
		//tmp->updateLabel();
		item.push_back('W'); item.push_back(m_pathwayID);
		 bool sharedS=false;
			 for(int i=0; i<sharedPathway.size();i++)
			{
				set<int> record=sharedPathway[i];
				if(record.find(m_pathwayID)!=record.end())
				{
					sharedS=true;
				}   
			}
			bool sharedD=false;
			for(int i=0; i<differedPathway.size();i++)
			{
				set<int> record=differedPathway[i];
				if(record.find(m_pathwayID)!=record.end())
				{
					sharedD=true;
				}   
			}
			 bool sharedE=false;
			for(int i=0; i<expressedPathway.size();i++)
			{
				set<int> record=expressedPathway[i];
				if(record.find(m_pathwayID)!=record.end())
				{
					sharedE=true;
				}   
			}
	        if( this->_pathwayHighlight)
		    {
			    QPen pen(QColor( 100, 100, 100, 200));
		        pen.setWidthF(4);	
				painter->setPen(pen); //else if(sharedL>=0)
			    
	        }
			if(highlighted.find(item)!=highlighted.end())	//pathway			
		    {
			    QPen pen(MIDIUM_HIGHLIGHTCOLOR);
		        pen.setWidthF(4);	
				painter->setPen(pen);					
	         }
			 if(sharedS || sharedD || sharedE)
			 {
				QPen pen;
				if(sharedS || highlighted.find(item)!=highlighted.end() )
					pen.setColor(assignColorToSharedItems(m_pathwayID, 'W', m_pathwayID, tmp->_scene->getGroup(this)->sharedSets));  					
				else if (sharedD)			        
					pen.setColor(assignColorToDifferedItems(m_pathwayID, 'W', m_pathwayID));
				else if (sharedE)			        
					pen.setColor(assignColorToExpressedItems(m_pathwayID, 'W', m_pathwayID, tmp->_scene->getGroup(this)->expressedSets,_itemColored,QColor(230,234,172,255)));
				
				pen.setWidthF(6);
				painter->setPen(pen);	
				painter->setBrush(Qt::NoBrush);
			 }
			 else painter->setBrush(Qt::NoBrush);
			 QRectF pRect=QRectF(0,0, realRect().width()-8, realRect().height()-8);		 
			 pRect.moveCenter(realRect().center());
			 painter->drawRoundedRect( pRect, 20, 20 );				 	 
	}
	//painter->setRenderHint(QPainter::Antialiasing, false);
	//extend the boundary a bit

	/*painter->setBrush(Qt::NoBrush);
	painter->setPen( QColor( getColor().a, getColor().b, getColor().c, getAlpha() ) );
	QRectF pRect=QRectF(0,0, realRect().width()-1, realRect().height()-1);		 
	pRect.moveCenter(realRect().center());
	painter->drawRoundedRect( pRect, 16, 16 );*/

	ControlAutoHide();
	painter->restore();
}

QRegion ItemBase::AllRegionSmallHalo()
{
	QRect bounding = this->realRect();
	QRegion reg;	
	
	QRect left( bounding.topLeft() - QPoint( 2, 0 ), bounding.bottomLeft() );
	reg += left;
	
	QRect right( bounding.topRight() , bounding.bottomRight() + QPoint( 2, 0 ) );
	reg += right;
	
	QRect down( bounding.bottomLeft() , bounding.bottomRight() + QPoint( 0, 2 ));
	reg += down;
	
	QRect top( bounding.topLeft()-QPoint( 0, 2 ), bounding.topRight() );
	reg += top;
	
	reg += this->realRect();
	return reg;
}


QRegion ItemBase::AllRegion()
{
	QRect bounding = this->realRect();
	QRegion reg;	
	
	int hwidth= HALOWIDTH;	
	if(this->_TYPE==REACTIONBUBBLE3)
	{
		float cx,cy,h2,w2,h,w;
		cx=bounding.center().x(); 
		cy=bounding.center().y();
		h=bounding.height()/2.0+5;
		w=bounding.width()/2.0+5;
		h2=h-8; w2=w-8; 
		 
		QVector<QPoint> points;	         
		points.append(QPoint(cx-w2, cy-h));
		points.append(QPoint(cx-w, cy-h2));
		points.append(QPoint(cx-w, cy+h2));
		points.append(QPoint(cx-w2, cy+h));
		points.append(QPoint(cx+w2, cy+h));
		points.append(QPoint(cx+w, cy+h2));
		points.append(QPoint(cx+w, cy-h2));
		points.append(QPoint(cx+w2, cy-h));
		QPolygon whole(points);		
	    reg += QRegion(whole, Qt::OddEvenFill);		

		return reg;
	}
	if(this->_TYPE==CALLOUT)
	{
		bounding = QRect(bounding.x()-2, bounding.y()-2, bounding.width()+4, bounding.height()+4);
	    reg += bounding; 
		return reg;
	}
	if(this->_TYPE==TEXTBUBBLE2)
	{
		hwidth=10;		
	}	

	if ( boarderLeft )
	{
		QRect left( bounding.topLeft() - QPoint( hwidth, 2 ), bounding.bottomLeft() + QPoint( hwidth, 2 ) );		
		reg += QRegion(left, QRegion::Ellipse);
	}
	else
	{
		QRect left( bounding.topLeft() - QPoint( 2, 0 ), bounding.bottomLeft() );		
		reg += left;
	}
	
	if ( boarderRight )
	{
		QRect right( bounding.topRight() - QPoint( hwidth, 2 ), bounding.bottomRight() + QPoint( hwidth, 2 ) );		
		reg += QRegion(right, QRegion::Ellipse);
	}
	else
	{
		QRect right( bounding.topRight() , bounding.bottomRight() + QPoint( 2, 0 ) );		
		reg += right;
	}
	////
	if ( boarderDown )
	{
		QRect down( bounding.bottomLeft() - QPoint( 2, hwidth ), bounding.bottomRight() + QPoint( 2, hwidth ) );		
		reg += QRegion(down, QRegion::Ellipse);;
	}
	else
	{
		QRect down( bounding.bottomLeft() , bounding.bottomRight() + QPoint( 0, 2 ));		
		reg += down;
	}
	//////
	if ( boarderUp )
	{
		QRect top( bounding.topLeft() - QPoint( 2, hwidth ), bounding.topRight() + QPoint( 2, hwidth ) );
		//painter->drawChord( top, 0*16, 180*16 );
		reg += QRegion(top, QRegion::Ellipse);;
	}
	else
	{
		QRect top( bounding.topLeft()-QPoint( 0, 2 ), bounding.topRight() );
		//painter->drawRect( top );
		reg += top;
	}

	reg += this->realRect();	
	return reg;
}

void ItemBase::drawHalo( QPainter* painter )
{
 	QRegion reg = AllRegionSmallHalo() - roundRect( realRect(), 12 );
 	QPainterPath path;
 	path.addRegion( reg );
 	painter->fillPath( path, painter->brush() );
 	return;
}


void ItemBase::addToGroup( ItemBase* target, bool updatePos)
{
	return;
}

void ItemBase::drawControlHint( QPainter *painter )
{
	if( _Base_ONMOVE )
	{
		QRadialGradient radialGradL(QPointF( -m_size_width/2 , -m_size_height/2 ), 20);
		radialGradL.setColorAt(0, QColor(0,0,80,200) );
		radialGradL.setColorAt(1, QColor(0,0,0,0) );
		QBrush curBrush = painter->brush();
		QPen curPen = painter->pen();
		//QLinearGradient linearGrad( QPointF( m_size/2 , m_size/2), QPointF( m_size/2 + 30, m_size/2 + 30) );
		painter->setBrush( QBrush( radialGradL ) );
		painter->setPen( QColor( 0,0,0,0) );
		painter->drawPie( QRectF( -m_size_width/2 - 19, -m_size_height/2 - 19, 40, 40 ), 16*270, 16*90 );
		painter->setBrush( curBrush );
		painter->setPen( curPen );
	}
	//			control
	if( _Base_ONCONTROL )
	{
		QRadialGradient radialGradR(QPointF( m_size_width/2 , -m_size_height/2 ), 20);
		radialGradR.setColorAt(0, QColor(0,0,80,200) );
		radialGradR.setColorAt(1, QColor(0,0,0,0) );
		QBrush curBrush = painter->brush();
		QPen curPen = painter->pen();
		painter->setBrush( QBrush( radialGradR ) );
		painter->setPen( QColor( 0,0,0,0) );
		painter->drawPie( QRectF( m_size_width/2 - 19, -m_size_height/2 - 19, 40, 40 ), 16*180, 16*90 );
		painter->setBrush( curBrush );
		painter->setPen( curPen );
	}

	//			resize
	painter->drawLine(m_size_width / 2 - 9, m_size_height / 2, m_size_width / 2, m_size_height / 2 - 9);
	painter->drawLine(m_size_width / 2 - 6, m_size_height / 2, m_size_width / 2, m_size_height / 2 - 6);
	painter->drawLine(m_size_width / 2 - 3, m_size_height / 2, m_size_width / 2, m_size_height / 2 - 3);
	//
	painter->drawLine( -m_size_width / 2 + 10, -m_size_height / 2, -m_size_width / 2 + 10, -m_size_height / 2 + 10);
	painter->drawLine( -m_size_width / 2 + 10, -m_size_height / 2 + 10, -m_size_width / 2, -m_size_height / 2 + 10);
	painter->drawLine( -m_size_width / 2 + 5, -m_size_height / 2, -m_size_width / 2 + 5, -m_size_height / 2 + 10);
	painter->drawLine( -m_size_width / 2, -m_size_height / 2 + 5, -m_size_width / 2 + 10, -m_size_height / 2 + 5);
	//
	painter->drawLine( m_size_width / 2 - 10, -m_size_height / 2, m_size_width / 2 - 10, -m_size_height / 2 + 10 );
	painter->drawLine( m_size_width / 2 - 10, -m_size_height / 2 + 10, m_size_width / 2, -m_size_height / 2 + 10 );
	painter->drawLine( m_size_width / 2 - 10, -m_size_height / 2 + 5, m_size_width / 2, -m_size_height / 2 + 5);
	painter->drawLine( m_size_width / 2 - 5, -m_size_height / 2, m_size_width / 2, -m_size_height / 2 + 5);
	painter->drawLine( m_size_width / 2 - 5, -m_size_height / 2 + 10, m_size_width / 2, -m_size_height / 2 + 5);

	return;
}

bool ItemBase::onMove( QPointF pos )
{
	return this->onHalo( pos );
	//int dis = int( sqrt( pow( -m_size/2 - pos.x(), 2) + pow( -m_size/2 - pos.y(), 2) ) );
	//if( dis < 20 )
	//{
	//	return true;
	//}
	////if( pos.x() >= -m_size / 2 && pos.x() <= -m_size / 2 + 10 )
	////{
	////	if( pos.y() >= -m_size / 2 && pos.y() <= -m_size / 2 + 10)
	////	{
	////		return true;
	////	}
	////}
	//return false;
}

bool ItemBase::onBoarder( QPointF pos )
{
	int x = pos.x();
	int y = pos.y();
	int xhalf = this->Width()/2;
	int yhalf = this->Height()/2;
	if( (x > xhalf - HALOWIDTH) || (x < -xhalf + HALOWIDTH) )
	{
		return true;
	}
	if( (y > yhalf - HALOWIDTH) || (y < -yhalf + HALOWIDTH) )
	{
		return true;
	}
	return false;
}

bool ItemBase::onControl(QPointF pos)
{
	/*
	int dis = int( sqrt( pow( m_size_width/2 - pos.x(), 2) + pow( -m_size_height/2 - pos.y(), 2) ) );
	if( dis < 20 )
	{
		return true;
	}*/

	//if( pos.x() >= m_size / 2 - 10 && pos.x() <= m_size / 2 )
	//{
	//	if( pos.y() >= -m_size / 2 && pos.y() <= -m_size / 2 + 10)
	//	{
	//		return true;
	//	}
	//}
	return false;
}

bool ItemBase::onUngroup( QPointF pos )
{
	int dis = int( sqrt( pow( -m_size_width/2 - pos.x(), 2) + pow( -m_size_height/2 - pos.y(), 2) ) );
	if( dis < 20 )
	{
		return true;
	}
	return false;
}

void ItemBase::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
	return;

	if(insideBubble(event->pos()))
	{
	   UNGROUP=false;
	   AFTERUNGROUP=false;
	}
	if(m_isMoving)
	{
	   m_isMoving=false;
	   return;
	}

	if (!isSelected() && scene()) 
	{
		scene()->clearSelection();
		setSelected(true);
	}
	QMenu menu;
	//QAction *queryAction = menu.addAction("Query");	
	//QAction *grabAction = menu.addAction("Grab");	
	//QAction *penAction = menu.addAction("Pen");
	//QAction *resetAction = menu.addAction("Reset");
	//QString ss("QMenu::item {  background-color: rgba(100,100,100,255); border: 3px darkgray; color: white }"); // Use background-color instead of background
	QString ss("QMenu::item {  background-color: black; border: 3px solid black; color: white }");
        menu.setStyleSheet(ss);
        menu.setMinimumWidth(70);
	

	QAction *delAction = menu.addAction("Delete");	
	QAction *selectedAction = menu.exec(event->screenPos());
	
	//_penisOn=false;	_grabisOn=false; _queryisOn=false;
	if (selectedAction == delAction)
	{
			QMessageBox msgBox;
			msgBox.setWindowTitle("Delete");
			msgBox.setText("Delete this bubble?");
			msgBox.setStandardButtons(QMessageBox::Yes);
			msgBox.addButton (QMessageBox::No);
			if(msgBox.exec() == QMessageBox::Yes){
			_penisOn=false;	
			_grabisOn=false;		   
			_queryisOn=false;	
			logRemove();
			deleteSelectedItems(scene());
		}
		else {
			// do something else
		}		
	}

}

void ItemBase::ControlAutoHide()
{
	if( !_REMOVECONTROL )
	{
		return;
	}
	int X = this->pos().x();
	int Y = this->pos().y();
	int W = this->scene()->width();
	int H = this->scene()->height();
	if( X < 0 || Y < 0 || X > W || Y > H )
	{
		if( CONTROLLOCK )
		{
			LASTOPENSTATE = this->_myControl->isVisible();
			this->_myControl->hide();
			CONTROLLOCK = false;
		}
	}
	else
	{
		if( LASTOPENSTATE && this->needRemoveControl() )// && this->_TYPE == WEB )
		{
			this->_myControl->show();
		}
		CONTROLLOCK = true;
	}
	return;
}

// high risk
void ItemBase::deleteSelectedItems(QGraphicsScene *_scene)
{
	if (!_scene)
		return;

	QList<QGraphicsItem *> selected;
	selected = _scene->selectedItems();

	//keqin added for speciall case that can not delete pathbubble
	//if( _TYPE==PATHBUBBLE1 || _TYPE==SUBPATHBUBBLE1 )
	//{
	     hide();
		 properRemove();

		 //emit itemPosUpdated( this );	

    //delete all of this
	 //this->_manager->clearAll();
	
		 //m_pathwayID=-1;
		 delete this;	
	//}
	//else
    /*foreach (QGraphicsItem *item, selected) 
	{
		ItemBase *itemBase = dynamic_cast<ItemBase *>(item);
		if (itemBase)
		{
			itemBase->hide();
			itemBase->properRemove();
			delete itemBase;
		}
	}*/

    
}

void ItemBase::deleteSelectedItems()
{
	deleteSelectedItems(scene());
}

void ItemBase::growSelectedItems(QGraphicsScene *scene)
{
	if (!scene)
		return;

	QList<QGraphicsItem *> selected;
	selected = scene->selectedItems();

	foreach (QGraphicsItem *item, selected) {
		ItemBase *itemBase = dynamic_cast<ItemBase *>(item);
		if (itemBase) {
			itemBase->prepareGeometryChange();
			itemBase->m_size_width *= 2;
			itemBase->m_size_height *= 2;
			if (itemBase->m_size_width > MAX_ITEM_SIZE)
				itemBase->m_size_width = MAX_ITEM_SIZE;
			if (itemBase->m_size_height > MAX_ITEM_SIZE)
				itemBase->m_size_height = MAX_ITEM_SIZE;
			/*if (itemBase->m_size_width > screenWidth )
				itemBase->m_size_width = screenWidth;
			if (itemBase->m_size_height > screenHeight)
				itemBase->m_size_height = screenHeight;*/
		}
	}
}

void ItemBase::shrinkSelectedItems(QGraphicsScene *scene)
{
	if (!scene)
		return;

	QList<QGraphicsItem *> selected;
	selected = scene->selectedItems();

	foreach (QGraphicsItem *item, selected)
	{
		ItemBase *itemBase = dynamic_cast<ItemBase *>(item);
		if (itemBase)
		{
			itemBase->prepareGeometryChange();
			itemBase->m_size_width /= 2;
			itemBase->m_size_height /= 2;
			if (itemBase->m_size_width < MIN_ITEM_SIZE)
				itemBase->m_size_width = MIN_ITEM_SIZE;
			if (itemBase->m_size_height < MIN_ITEM_SIZE)
				itemBase->m_size_height = MIN_ITEM_SIZE;

		}
	}
}

void ItemBase::Pan(QPointF &x0y0, QPointF &x1y1, QPointF dxy0, QPointF dxy1, float &_scale, QPointF &dCenter)
{
	//QPointF x0y0_t, x1y1_t;	
	x0y0 = x0y0 + dxy0;			    
	x1y1 = x1y1 + dxy1;		
	QPointF center = (x1y1 + x0y0) / 2.0;	
	dCenter.setX(center.x()/this->Width());
	dCenter.setY(center.y()/this->Height());	
	//_scale = _scale_t;				
}

void ItemBase::reScale(QPointF &x0y0, QPointF &x1y1, QPointF dxy0, QPointF dxy1, float &_scale, QPointF &dCenter, float rate)
{	
	float width = initWidth;
	QPointF p0, p1;
	float _scale_t=_scale;
	int i=1;
	bool flag=true;
	while(flag)
	{		
		flag=false;
		{
			QPointF x0y0_t, x1y1_t;					    
			x0y0_t = x0y0 + dxy0*i;			    
			x1y1_t = x1y1 + dxy1*i;	
			if(rate > 1)
			{
				_scale_t = fabs(x1y1_t.x()-x0y0_t.x())/width ;//this->Width();
				if(_scale_t < _scale)
				{
					flag = true;						
				}
			}
			else if(rate<1)
			{
				_scale_t = fabs(x1y1_t.x()-x0y0_t.x())/width ;//this->Width();
				if(_scale_t > _scale)
				{
					flag = true;						
				}
			}
			if(rate!=1)
			{
			     float t_scale=fabs(x1y1.x()-x0y0.x())/width;
				 if( t_scale <= 0.12*_ScaleBackup || t_scale >= 3.8*_ScaleBackup ) //check setDrawSmallmolecule in openglscene.cpp since it also use the scale range here
				 {
				     width = fabs(x1y1.x()-x0y0.x())/_scale;
				    _scale_t = fabs(x1y1_t.x()-x0y0_t.x())/width;
				 }
			}
			if(!flag)
			if( _scale_t > 0.12*_ScaleBackup && _scale_t < 3.8*_ScaleBackup )
			{
				x0y0 = x0y0_t;   x1y1 = x1y1_t;																
				QPointF center = (x1y1 + x0y0) / 2.0;	
				dCenter.setX(center.x()/this->Width());
				dCenter.setY(center.y()/this->Height());	
				_scale = _scale_t;				
			}				
		}
		if(i++>5)
			break;
	}	
}



void ItemBase::wheelReScale(QPointF &x0y0, QPointF &x1y1, QPointF dxy0, QPointF dxy1, float &_scale, QPointF &dCenter, float rate)
{
	//float width = initWidth;
	//QPointF p0, p1;
	float _scale_t=_scale;
	//int i=1;
	//bool flag=true;
	
	//dxy0 = (x0y0-dxy0)*rate+dxy0-x0y0;
	//dxy1 = (x1y1-dxy1)*rate+dxy1-x1y1;
	//while(flag)
	//{		
		//flag=false;
		//p0 = x0y0 + dxy0*i;			    
		//p1 = x1y1 + dxy1*i;					
		//if( fabs(p1.x()-p0.x())/width  > 0.005 || (p1.x()-p0.x())/width  > _scale )
		{
			//QPointF x0y0_t, x1y1_t;					    
			//x0y0_t = x0y0 + dxy0*i;			    
			//x1y1_t = x1y1 + dxy1*i;		
			/*_scale_t = fabs(x1y1_t.x()-x0y0_t.x())/width ;//this->Width();
			if(rate > 1)
			{
				if(_scale_t < _scale)
				{
					flag = true;						
				}
			}
			else if(rate<1)
			{
				if(_scale_t > _scale)
				{
					flag = true;						
				}
			}
			if(rate!=1)
			{
			     float t_scale=fabs(x1y1.x()-x0y0.x())/width;
				 if( t_scale <= 0.33 || t_scale >= 2.4 )
				 {
				     width = fabs(x1y1.x()-x0y0.x())/_scale;
				    _scale_t = fabs(x1y1_t.x()-x0y0_t.x())/width;
				 }
			}
			*/
		
			if(rate > 1)
			{
				_scale_t=_scale_t+0.1;
				if(_scale < 3.8*_ScaleBackup)
				   _scale=_scale_t;
			}
			else if(rate<1)
			{
				_scale_t=_scale_t-0.1;
				if(_scale > 0.12*_ScaleBackup)
				   _scale=_scale_t;
			}			
		}		
}

void ItemBase::mouseMoveEvent( QGraphicsSceneMouseEvent *event )
{
	if (m_isResizing)
	{
		if( (resizeFinished && getType()!=TEXTBUBBLE2) || getType()==CALLOUT)
		{
			resizeFinished=false;

			int dx = int(2.0 * event->pos().x());
			int dy = int(2.0 * event->pos().y());
			int size_x,sizey;

			if( (dx>=0 && dy>=0) || ( dx<=0 && dy<=0) )
			{
				setCursor(Qt::SizeFDiagCursor);
			}
			else 
			{
				setCursor(Qt::SizeBDiagCursor);
			}
			//resizeItem( dy );
			dx=abs(dx);
			dy=abs(dy);
	    
			if( resizeMode ==1 )
			{
				x0y0.setX(x0y0.x() - (dx-this->Width())/2.0);			
				x0y0.setY(x0y0.y() - (dy-this->Height())/2.0);	
	    
				x1y1.setX(x1y1.x() + (dx-this->Width())/2.0);			
				x1y1.setY(x1y1.y() + (dy-this->Height())/2.0);	

				
			}
			
			int index=whichCorner(event->pos());
			//if(dx > MIN_ITEM_SIZE && dy > MIN_ITEM_SIZE)
			if(_TYPE!=CALLOUT || index!=4)
			{	
				//QPointF center=this->sceneBoundingRect().center();
				//QRectF oriRect = QRectF((float)center.x()-(float)this->realRect().width()/2, (float)center.y()-(float)this->realRect().height()/2, (float)this->realRect().width(), (float)this->realRect().height());
				QRectF oriRect = QRectF(pos().x()-this->Width()/2.0, pos().y()-this->Height()/2.0, (float)this->Width(), (float)this->Height());	
			
				if(resizeMode==1)
					resizeItem( dx, dy );
				else if(resizeMode==2||resizeMode==3) 
				{
					 resizeItem_1(event->pos(), event->lastPos(), index);
					/* if( this->getType()==SUBPATHBUBBLE1 || this->getType()==PATHBUBBLE1)
					 {	
						PathBubble1* pbubble = dynamic_cast<PathBubble1 *> (this);										
						//pbubble->getGraphToBePaint();
					    pbubble->recordItemGrid();
					 }*/
				}
				//
				//change index				
				if(index==1)
					index=0;
				else if(index==2)
					index=6;
				else if(index==3)
					index=4;
				else if(index==4)
					index=2;



				emit itemResized(this, oriRect, index);				
				//_manager->bubbleResized();
			}		
		}
	}
	else if(!m_rightButton)
	{
		//QGraphicsItem::mouseMoveEvent(event);
		m_isMoving= true;
		if(UNGROUP)
		{
			emit unGroup( this );
	        logUnGroup();
			UNGROUP=false;
			AFTERUNGROUP=true;	
			this->setCursor( Qt::SizeAllCursor );
		}
		//QPointF newPos = this->pos()+ event->pos() - event->lastPos();
		//if(newPos.y() + this->realRect().height()/2 >55 && newPos.y() - this->realRect().height()/2 < screenHeight-80 && newPos.x() + this->realRect().width()/2 > 2 )
		{
			this->mySetPos( this->pos()+ event->pos() - event->lastPos() ); 
			emit itemPosMoved( this, event->pos() - event->lastPos() );
		}
	}
	UNGROUPBYMENU = false;
}

void ItemBase::resizeItem_1( QPointF Pos, QPointF lastPos, int cornerID)
{ //corner move
	
	QPointF  dis = Pos- lastPos;
	prepareGeometryChange();
	float Px,Py,disx,disy;
	float sizew,sizeh;
	
	disx=dis.x();  disy=dis.y();
	if(cornerID==1)
	{
		disx=-disx;
		disy=-disy;
	}
	else if(cornerID==2)
    {
		disx=-disx;			
	}
	else if(cornerID==4)
    {
		disy=-disy;			
	}

	//if(dis.x()<=1 && dis.y()<=1)
	sizew =  this->realRect().width() + disx;
	sizeh =  this->realRect().height() + disy;

	/*
	x1y1.setX(x1y1.x() + disx);			
    x1y1.setY(x1y1.y() + disy);		

	x0y0.setX(x0y0.x() + disx);			
    x0y0.setY(x0y0.y() + disy);	*/
		
	if ( sizew < MIN_ITEM_SIZE)
	{
		sizew = MIN_ITEM_SIZE;
		disx = sizew - this->realRect().width(); 		
	}
	if (sizew > screenWidth)
	{
		sizew = screenWidth;
		disx = sizew - this->realRect().width(); 
	}
	if ( sizeh < MIN_ITEM_SIZE)
	{
	   sizeh = MIN_ITEM_SIZE;
	   disy = sizeh - this->realRect().height();
	}
	if (sizeh > screenHeight)
	{
		sizeh = screenHeight;
		disy = sizeh - this->realRect().height();
	}
	
	//adjust it back
	if(cornerID==1)
	{
		disx=-disx;
		disy=-disy;
	}
	else if(cornerID==2)
    {
		disx=-disx;			
	}
	else if(cornerID==4)
    {
		disy=-disy;			
	}
	
	dis=QPointF(disx,disy);
	Pos = dis - lastPos;
	
	float dx = 2.0 * Pos.x();
	float dy = 2.0 * Pos.y();
		
	dx=abs(dx);
	dy=abs(dy);

	Px = disx/2.0; 
	Py = disy/2.0;

	QPointF p0,p1;
	
		
	QPointF center=QPointF(QPointF(Px,Py) + this->pos());
	
	
		   
	QPointF tcenter=(x1y1+x0y0)/2.0;		
    dCenter.setX(tcenter.x()/sizew);
	dCenter.setY(tcenter.y()/sizeh);
	//tcenter=-tcenter;
	x1y1=tcenter+QPointF(sizew/2.0,sizew/2.0);
	x0y0=tcenter+QPointF(-sizew/2.0,-sizew/2.0);


	//initWidth = fabs(x1y1.x()-x0y0.x());
	
	this->mySetPos(center);//use this->Pos() to get the center of the current  bubble instead of boundingrect.center!
	this->m_size_width=sizew;
	this->m_size_height=sizeh;	
	
	_initSize = this->m_size_width;
	_initSize2 = this->m_size_height;

	
	if (_label)
	{
		this->_label->updateLabel( this->m_size_width, this->m_size_height );		
	}
	return;
}

void ItemBase::drawLinkArrow( QPainter *painter, QPointF start, QPointF end)
{// L shape connector

    /*start=QPointF(x0,y0);
	end = bubble->pos();
	end.setX( end.x() - bubble->Width()/2 + dis.x());
	end.setY( end.y() + dis.y());
	end -= this->pos();	*/

	QPointF tmpmid;

	float width = end.x() - start.x();

	tmpmid.setX( start.x() + width/2  );
	tmpmid.setY( start.y() );

	if ( fabs(end.y()-start.y()) > 20)
	{
		painter->drawLine( start, tmpmid - QPointF( 10.0, 0) );
		if ( end.y() > start.y() )
		{
			painter->drawArc( tmpmid.x()-20.0, tmpmid.y(), 20.0, 20.0, 0, 90*16 );
			painter->drawLine( tmpmid+QPointF( 0, 10.0), QPointF(tmpmid.x(), end.y()-10.0));
			tmpmid.setY( end.y() );
			painter->drawArc( tmpmid.x(), tmpmid.y()-20.0, 20.0, 20.0, 180*16, 90*16 );
		}
		else
		{
			painter->drawArc(  tmpmid.x()-20.0,  tmpmid.y()-20.0, 20.0, 20.0, 270*16, 90*16 );
			painter->drawLine( tmpmid-QPointF( 0, 10.0), QPointF(tmpmid.x(), end.y()+10.0));
			tmpmid.setY( end.y() );
			painter->drawArc(  tmpmid.x(), tmpmid.y(), 20.0, 20.0, 90*16, 90*16 );
		}
		painter->drawLine( tmpmid + QPointF( 10.0, 0), end );
	}
	else
	{
		painter->drawLine( start, tmpmid);
		painter->drawLine( tmpmid, QPointF(tmpmid.x(), end.y()));
		tmpmid.setY( end.y() );
		painter->drawLine( tmpmid, end );
	}		
	//arrow
	if (end.x() > start.x())
	{
		painter->drawLine( end, QPointF(end.x()-10, end.y()-4) );
		painter->drawLine( end, QPointF(end.x()-10, end.y()+4) );
	}
	else
	{
 		painter->drawLine( end, QPointF(end.x()+10, end.y()-4) );
 		painter->drawLine( end, QPointF(end.x()+10, end.y()+4) );
	}
}

void ItemBase::drawLinkArrow_2( QPainter *painter, QPointF start, QPointF end)
{//simple arrow connector
    /*start=QPointF(x0,y0);
	end = bubble->pos();
	end.setX( end.x() - bubble->Width()/2 + dis.x());
	end.setY( end.y() + dis.y());
	end -= this->pos();	*/
	painter->save(); 
	painter->drawLine( start, end );
	
    QLineF line(start, end);
	float size1=10;
    double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0)
		angle = M_PI*2 - angle;

	QPointF sourceArrowP1 = start + QPointF(sin(angle - M_PI / 2.3) * size1,
													  cos(angle - M_PI / 2.3) * size1);                                                  
	QPointF sourceArrowP2 = start + QPointF(sin(angle - M_PI + M_PI / 2.3) * size1,
													cos(angle - M_PI + M_PI / 2.3) * size1);   
	QPointF dis = (sourceArrowP1 + sourceArrowP2)/2;
	dis=start-dis;
	sourceArrowP1 = sourceArrowP1 + dis;
	sourceArrowP2 = sourceArrowP2 + dis;
	
	painter->drawLine( sourceArrowP1, end);
	painter->drawLine( sourceArrowP2, end);

	//painter->setRenderHint(QPainter::Antialiasing, false);
	painter->restore(); 
	

}

bool ItemBase::outsideBubble( QPointF pos )
{
	int x = pos.x();
	int y = pos.y();
	int half = this->Width()/2;
	int half2 = this->Height()/2;
	if( (x > half ) || ( x < -half ) )
	{
		return true;
	}
	if( (y > half2 ) || ( y < -half2 ) )
	{
		return true;
	}
	return false;
}


bool ItemBase::insideBubble( QPointF pos )
{
	if(onBoarder(pos))
		return false;

	if(outsideBubble(pos))
	   return false;
	return true;
}

bool ItemBase::onUnGroupArea( QPointF pos )
{
	//left middle
    int x = pos.x();
	int y = pos.y();
	int half = this->Width()/2;
    int half2 = this->Height()/2;	
	int bottom= -half2*0.25, top = half2*0.25, right = -half , left= -half - HALOWIDTH;
	if(top-bottom<MIN_ITEM_SIZE)
	{	
	    bottom = -MIN_ITEM_SIZE/4, top = MIN_ITEM_SIZE/4;
	}
	
    if( x>left && x<right && y>bottom && y<top )
	{
		 return true;
	}
	return false;
}


bool ItemBase::onTagArea( QPointF pos )
{//on boundary area instead
	int x = pos.x();
	int y = pos.y();
	int half = this->Width()/2;
	int half2 = this->Height()/2;
	if( (x > half - (BOARDERSIZE + half*0.05+2) && x < half - (BOARDERSIZE - 12) ) || (x < -half + (BOARDERSIZE + half*0.05+2) && x > -half + (BOARDERSIZE - 12) ) )
	{
		 return true;
	}
	if( (y > half2 - (BOARDERSIZE + half2*0.05+2) && y < half2 - (BOARDERSIZE - 12) ) || (y < -half2 + (BOARDERSIZE + half2*0.05+2) && y > -half2 + (BOARDERSIZE - 12) ) )
	{
	 	 return true;
	}
	return false;
}


void ItemBase::initializeGL()
{
	
}

void ItemBase::resizeItem( int size1, int size2 )
{//enlarge based on the center
	prepareGeometryChange();
	this->m_size_width = size1;
	this->m_size_height = size2;
	//
	if ( m_size_width < MIN_ITEM_SIZE)
	{
		m_size_width = MIN_ITEM_SIZE;
	}
	if (m_size_width > screenWidth)
	{
		m_size_width = screenWidth;
	}
	if ( m_size_height < MIN_ITEM_SIZE)
	{
		m_size_height = MIN_ITEM_SIZE;
	}
	if (m_size_height > screenHeight)
	{
		m_size_height = screenHeight;
	}

	_initSize = this->m_size_width;
	_initSize2 = this->m_size_height;

	if (_label)
	{
		//this->_label->updateLabel( this->m_size_width, this->m_size_height );
	}
	return;
}

void ItemBase::initControlsAnimation( QPointF pos )
{
	if( onMove( pos ) )
	{
		_Base_ONMOVE = true;
	}
	else
	{
		_Base_ONMOVE = false;
	}

	if( onControl( pos ) )
	{
		_Base_ONCONTROL = true;
	}
	else
	{
		_Base_ONCONTROL = false;
	}
}

void ItemBase::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
   if(_LABEL && _label)
   {
	   _label->mouseOvered=false;
	   _label->mouseClicked=0;
   }

   int noteID=getCalloutNote(event->scenePos());
	if(noteID>=0)
		return;

	//initControlsAnimation( event->pos() );

	if(getType()==TEXTBUBBLE2)
		return;

	int resizeflag = isInResizeArea(event->pos());

	if (m_isResizing || ( resizeflag && isSelected()))
	{
		if(resizeflag==1 || resizeflag==3)
		{
		    setCursor(Qt::SizeFDiagCursor);
		}
		else
		{
		    setCursor(Qt::SizeBDiagCursor);
		}
	}
	else if(onUnGroupArea(event->pos()) )
	{
		QList<ItemBase *> mList=_scene->getGroupMembers(this);	 
	    if(mList.size()>1)
			this->setCursor( Qt::SplitHCursor );
	}	
	else
	{
		setCursor(Qt::ArrowCursor);
	}
	QGraphicsItem::hoverMoveEvent(event);
}

void ItemBase::unGroupbyMenu()
{
	//UNGROUP = true;	
	UNGROUPBYMENU = true;	
	emit unGroup( this );
	logUnGroup();
	//AFTERUNGROUP=false;
	//_highLight = true;
	mySetPos( scenePos() + QPointF(20,-20) );		
	//UNGROUP=false;
	//AFTERUNGROUP=true;		
}

void ItemBase::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	static qreal z = 0.0;
	int noteID=getCalloutNote(event->scenePos());
	if(noteID>=0)
		return;

	if(_scene!=NULL && this->getType()==CALLOUT)
	{
		_scene->setTopItem(this);			
	}	
	setZValue(z += 1.0);
		
	if (event->button() == Qt::RightButton) 
	   m_rightButton=true;
	else m_rightButton=false;

	//if( // this->onUngroup( event->pos()) ||
	//(this->onBoarder( event->pos())  && event->button() == Qt::RightButton ) )
	if( onUnGroupArea (event->pos())  && event->button() == Qt::LeftButton )
	{
		//if(highlighted.empty())
		UNGROUP = true;				
		return;
	}
	if (event->button() == Qt::LeftButton && isInResizeArea(event->pos())) 
	{
		m_isResizing = true;
	}
	else
	{
		QGraphicsItem::mousePressEvent(event);
	}	
}

QSizeF ItemBase::getItemSize( int type, int id, QString qstr)
{
	float width=graphReferenceSize, height=graphReferenceSize;
	float h,w;
	string s1;
	std::ostringstream oss;
	QSizeF size;

	switch(type)
	{
	     /*case 'C': w=0.022;  break;
		 case 'E': w=0.03; break;
		 case 'P': w=0.019;  break;
		 case 'S': w=0.011; break;		
		 case 'D': w=0.011; break;	 
		 case 'N': w=0.022;  break;
		 case 'R': size.setWidth(6/width); size.setHeight(6/height); break;	 */
		 case 'C': w=0.015;  break;
		 case 'E': w=0.015; break;
		 case 'P': w=0.015;  break;
		 case 'S': w=0.015; break;		
		 case 'D': w=0.015; break;	 
		 case 'N': w=0.015;  break;
		 case 'R': size.setWidth(6/width); size.setHeight(6/height); break;	 
	}

	if(type!='R')
	{
		w=w*width;
		QTextDocument doc;
		doc.setHtml(qstr);
		QFont f("Arial",6);
		doc.setDefaultFont(f);
		doc.setTextWidth( w );
		size = doc.size();
		size.setWidth(size.width());
		size.setHeight(size.height());
	}	
	return size;
}


QSizeF ItemBase::getItemSize_2(int type, int id, QString qstr)
{
	float width=graphReferenceSize, height=graphReferenceSize;
	float h,w;
	string s1;
	std::ostringstream oss;
	QSizeF size;

	switch(type)
	{
	     /*case 'C': w=0.022*2;  break;
		 case 'E': w=0.03*2;   break;
		 case 'P': w=0.019*2;  break;
		 case 'S': w=0.011*2;  break;		
		 case 'D': w=0.011*2;  break;	
		 case 'N': w=0.022*2;  break;
		 case 'R': size.setWidth(6/width); size.setHeight(6/height); break;	 */
		 case 'C': w=0.015;  break;
		 case 'E': w=0.015; break;
		 case 'P': w=0.015;  break;
		 case 'S': w=0.015; break;		
		 case 'D': w=0.015; break;	 
		 case 'N': w=0.015;  break;
		 case 'R': size.setWidth(6/width); size.setHeight(6/height); break;	 
	}

	//if(type!='R')
	{
		w=w*width;
		QTextDocument doc;
		doc.setHtml(qstr);
		QFont f("Arial",8);
		doc.setDefaultFont(f);
		//doc.setTextWidth( w );
		size = doc.size();
		size.setWidth(size.width());
		size.setHeight(size.height());
	}	
	return size;
}


QRectF ItemBase::paintPreComputedCompartment( QPainter *painter,float boundaryWidth, int fontSize, vector<QColor> insideColor, QColor boundaryColor, QColor fontColor, QRectF fontRect, QRectF nodeRect, vector<QRectF> compartmentPos, vector<vector<QString>> _compartmentName, int id, vector<set<vector<int>>> sharedSets, QPointF dCenter, float scale, bool flag,  QColor c)
{		
   //int width = this->realRect().width()*scale;
   //int height = this->realRect().height()*scale;

   int width = graphReferenceSize * scale;
   int  height = graphReferenceSize * scale; 
   QPen pen;

   painter->setBrush(Qt::NoBrush);
   
   pen.setColor( boundaryColor);  

   pen.setWidthF(boundaryWidth);

   int rw=nodeRect.width(), rh=nodeRect.height();
   rw=rw<100?rw*0.15:15, rh=rh<100?rh*0.15:15;
   painter->setPen(pen);   
   
   QFont f("Arial",fontSize);
   painter->setFont(f);	   
   
   if(scale<1)
   {
	   QFontMetrics fontMetrics(f);
	   fontRect = fontMetrics.boundingRect(QRect(nodeRect.x()+3, nodeRect.y()+6, nodeRect.width()-6, nodeRect.height()-6), Qt::AlignBottom | Qt::AlignCenter, _compartmentName[id][0]); 	
	   /*if(fontRect.width()+6 > nodeRect.width())
	   {
		   float w=fontRect.width()+6, h=nodeRect.height();
		   nodeRect = QRectF(nodeRect.center().x()-w/2,nodeRect.center().y()-h/2,w,h);
	   }*/
   }
   painter->drawRoundedRect( nodeRect, rw, rh);
   
   pen.setColor(fontColor);         
   painter->setPen(pen);
   painter->drawText(QRect(nodeRect.x()+3, nodeRect.y()+6, nodeRect.width()-6, nodeRect.height()-6), Qt::AlignBottom | Qt::AlignCenter, _compartmentName[id][0]);//_complexName[id]);   

   return nodeRect;   
}

QRectF ItemBase::paintPreComputedCompartment_H( QPainter *painter,float boundaryWidth, int fontSize, vector<QColor> insideColor, QColor boundaryColor, QColor fontColor, QRectF fontRect, QRectF nodeRect, vector<QRectF> compartmentPos, vector<vector<QString>> _compartmentName, int id, vector<set<vector<int>>> sharedSets, QPointF dCenter, float scale, bool flag,  QColor c)
{		
   //int width = this->realRect().width()*scale;
   //int height = this->realRect().height()*scale;

   int width = graphReferenceSize * scale;
   int  height = graphReferenceSize * scale; 
   float exWidth=HBWIDTH*scale-2;

   painter->setBrush(Qt::NoBrush);
   QPen pen(HIGHLIGHTCOLOR);              
   pen.setWidthF(exWidth+2);

   int rw=nodeRect.width(), rh=nodeRect.height();
   rw=rw<100?rw*0.15:15, rh=rh<100?rh*0.15:15;
   painter->setPen(pen);   
   
   QFont f("Arial",fontSize);
   painter->setFont(f);	   
   
   //if(boundaryColor == HIGHLIGHTCOLOR ) //draw it a background bubble set mode
   {
	   QRectF rect=QRectF(nodeRect.x()-exWidth,nodeRect.y()-exWidth,nodeRect.width()+exWidth*2,nodeRect.height()+exWidth*2);
       painter->drawRoundedRect( rect, rw, rh);
   }
   return nodeRect;   
}



QRectF ItemBase::paintCompartment( QPainter *painter, vector<QRectF> compartmentPos, vector<vector<QString>> _compartmentName, int id, vector<set<vector<int>>> sharedSets, vector<set<vector<int>>> expressedSets, bool fixedSize, QPointF dCenter, float scale, bool flag,  QColor c)
{
	//pathwaySelected=-1;
	int sharedS=-1;  
   for(int i=0; i<sharedCompartment.size();i++)
   {
       set<int> record=sharedCompartment[i];
	   if(record.find(id)!=record.end())
	   {
		  sharedS=i;
	   }   
   } 
   int sharedL=-1;  
   for(int i=0; i<linkedCompartment.size();i++)
   {
       set<int> record=linkedCompartment[i];
	   if(record.find(id)!=record.end())
	   {
		  sharedL=i;
	   }   
   } 
   int sharedD=-1;  
   for(int i=0; i<differedCompartment.size();i++)
   {
       set<int> record=differedCompartment[i];
	   if(record.find(id)!=record.end())
	   {
		  sharedD=i;
	   }   
   } 
    int sharedE=-1;  
   for(int i=0; i<expressedCompartment.size();i++)
   {
       set<int> record=expressedCompartment[i];
	   if(record.find(id)!=record.end())
	   {
		  sharedE=i;
	   }   
   } 

   int width = this->realRect().width()*scale;
   int height = this->realRect().height()*scale;

   if(fixedSize)
   {
       width = graphReferenceSize * scale;
	   height = graphReferenceSize * scale; 
   }

   int h=compartmentPos[id].height()*height, w=compartmentPos[id].width()*width;	
   int cx=compartmentPos[id].x()*width-width/2, cy=compartmentPos[id].y()*height-height/2;
   int dcx,dcy;
  
   /*if(fixedSize)
   {
      dcx=dCenter.x()*600; dcy=dCenter.y()*600;
   }
   else*/
	   dcx=dCenter.x()*this->realRect().width(); dcy=dCenter.y()*this->realRect().height();

   QRect rect = QRect(cx+ dcx, cy+ dcy, w, h);
	
   vector<int> item; 
   item.push_back('M'); item.push_back(id);
   QPen pen(QColor(0,0,0,255));    
   if(((!hoveredItem.empty()&&hoveredItem[0]==m_pathwayID && hoveredItem[1]==item[0] && hoveredItem[2]==item[1]))||foundlist.find(item)!=foundlist.end()||highlighted.find(item)!=highlighted.end())
   {
		pen.setWidthF(4);
   }
   else 
   {	
		pen.setWidthF(2);
   }
   painter->setPen(pen);	

   if(highlighted.find(item)!=highlighted.end())
	{
			pen.setColor(QColor(255,100,100,255));
		    //QPen pen(c);
		    pen.setWidthF(4);
	}
	QColor color;
   if(sharedS<0 && sharedL<0 && sharedD<0)
       painter->setBrush(Qt::NoBrush);
   else 
   {	painter->setBrush(Qt::NoBrush);
        pen.setWidthF(4);
	    vector<int> item;
	    item.push_back('M');  item.push_back(id);
		
		if(sharedS>=0)		
			color=assignColorToSharedItems(m_pathwayID, 'M', id, sharedSets);
		else if(sharedD>=0)
			color=assignColorToDifferedItems(m_pathwayID, 'M', id);
		else if(sharedL>=0)
		{			
			PathBubble1* pbubble = dynamic_cast<PathBubble1 *> (this);
			float level=pbubble->findLinkedLevel(m_pathwayID, 'M', id);
			float totalLevel=pbubble->getTotalLinkLevel();			
			color=assignColorToLinkedItems(level, totalLevel); 
		} 
		else if(sharedE>=0)
			color=assignColorToSharedItems(m_pathwayID, 'M', id, expressedSets);
		else if(highlighted.find(item)!=highlighted.end())
			color=QColor(255,100,100,255); 
		else 
			color=QColor(0,0,0,255);

		pen.setColor(color);
	}

   //painter->setPen(pen);   
   painter->setPen(QColor(255,100,100,255));   
   painter->drawRoundedRect( rect,  8.0*graphReferenceSize/width, 8.0*graphReferenceSize/height );

   pen.setColor(QColor(color.red()*0.0, color.green()*0.0, color.blue()*0.0, 255));
   painter->setPen(pen);

   int fontsize;

   /*
   if(fixedSize)
   {
		fontsize=12*scale; if(fontsize<9) fontsize=9; else if(fontsize>27) fontsize=27; 
   }
   else
   {   
		fontsize=6*(this->realRect().width()+this->realRect().height())/graphReferenceSize.0*scale; if(fontsize<9) fontsize=9; else if(fontsize>27) fontsize=27; 
   }
   QRectF rect2;

 
   QFont f1("Arial",fontsize);		          
   QFontMetrics fontMetrics(f1); 
   QRectF rect3 = fontMetrics.boundingRect(0, 0, 0, 0, Qt::TextExpandTabs|Qt::TextWrapAnywhere, "A"); 		
   do
   {
		fontsize=fontsize-1;	
		QFont f("Arial",fontsize);		        
		
		//int width = rect.width();
		QFontMetrics fontMetrics(f);
		rect2 = fontMetrics.boundingRect(0, 0, this->realRect().width(), rect3.height(), Qt::TextExpandTabs|Qt::TextWrapAnywhere, _compartmentName[id][0]); 		
   }while(rect2.width()>rect.width() && fontsize>8);
  
   if(rect2.height()>rect.height())
		rect2=rect;	
  */
   //getNodeFont(fontSize, fontRect, rect, nodeName, type, width, height, fixedSize, scale);	
   QRectF fontRect;
   getCompartmentFont(fontsize, fontRect, rect, _compartmentName[id][0], fixedSize, scale);

   QFont f("Arial",fontsize);
   painter->setFont(f);	
   
   // painter->drawText(QRect(fontRect.center().x()-rect2.width()/2, fontRect.y()+3, rect2.width(), rect2.height()), Qt::AlignBottom | Qt::AlignLeft, _compartmentName[id][0]);//_complexName[id]);   
   painter->drawText(QRect(fontRect.x()+3, fontRect.y()+3, fontRect.width()-6, fontRect.height()-6), Qt::AlignBottom | Qt::AlignLeft, _compartmentName[id][0]);//_complexName[id]);   
   return fontRect;   
}

QRectF ItemBase::transRect(QRectF rect, float scale, QPointF dCenter, int width, int height)
{
		width = width*scale;
		height = height*scale;
		float h=rect.height(), w=rect.width();	
		float cx=rect.x(), cy=rect.y();

		h=h*height, w=w*width;	
		cx=cx*width-width/2, cy=cy*height-height/2;
	
		return QRectF(cx+ dCenter.x(), cy+ dCenter.y(), w, h);
}


QColor ItemBase::assignColorToSharedItems(int pid, int type, int id, vector<set<vector<int>>> sharedSets)
{
	vector<int> item(3,0);
	QColor color;
	item[0]=pid, item[1]=type, item[2]=id;
	int cid=-1;
	
	color=HIGHLIGHTCOLOR;//QColor( 255, 255, 67, 255); //light yellow
	return color;
	/*cid=(cid+24)%28;	
	if(cid>=0)
	{
		switch(cid)
		{
			case 0:	 color=QColor( 255, 255, 179, 255); break; ////brewer light qunatitive color 
			case 1:	 color=QColor( 141, 211, 199, 255); break;
			case 2:	 color=QColor( 190, 186, 218, 255); break;
			case 3:	 color=QColor( 251, 128, 114, 255); break;
			case 4:	 color=QColor( 128, 177, 211, 255); break;
			case 5:	 color=QColor( 253, 180, 98, 255); break;
			case 6:	 color=QColor( 179, 222, 105, 255); break;
			case 7:	 color=QColor( 252, 205, 229, 255); break;
			case 8:	 color=QColor( 217, 217, 217, 255); break;
			case 10: color=QColor( 188, 128, 189, 255); break;
			case 11:    color=QColor( 166, 206, 227, 255); break; ////brewer dark qunatitivecolor 
			case 12:	color=QColor( 31, 120, 180, 255); break;
			case 13:	color=QColor( 51, 160, 44, 255); break;
			case 14:	color=QColor( 251, 154, 153, 255); break;
			case 15:	color=QColor( 227, 26, 28, 255); break;
			case 16:	color=QColor( 253, 191, 111, 255); break;
			case 17:	color=QColor( 255, 127, 0, 255); break;
			case 18:	color=QColor( 202, 178, 214, 255); break;
			case 20:    color=QColor( 106, 61, 154, 255); break; 			
			case 21:    color=QColor( 255, 120, 255, 255); break; //R, G, B
			case 22:    color=QColor( 255, 120, 120, 255); break;
			case 23:    color=QColor( 120, 255, 255, 255); break;
			case 24:    color=QColor( 255, 255, 120, 255); break;
			case 25:    color=QColor( 120, 255, 120, 255); break; 
			case 26:    color=QColor( 120, 120, 255, 255); break;
			case 27:    color=QColor( 255, 148, 28, 255); break;
			default:    color=QColor( 255, 255, 255, 255); break;
		}		
		return color;	
	}
	else return ( QColor( 255, 255, 255, 255));*/
}

QColor ItemBase::assignColorToLinkedItems(float level, float totalLevel, int type)
{//type for node or edge, for edge, the color is darker
	float r, g, b;	
	float l;
	//TDHelper::HLS_TO_RGB(60,level,240/255.0,r,g,b);	
	if(totalLevel<=8)
	{
	   totalLevel=8;
	}
	
	if(type==0)
		TDHelper::HLS_TO_RGB(110-50*(totalLevel-level )/totalLevel, 0.55, 0.9, r, g, b);//
	else TDHelper::HLS_TO_RGB(110-50*(totalLevel-level )/totalLevel, 0.45, 0.9, r, g, b);//
  
	return QColor( r, g, b, 255);
}

QString ItemBase::getPathID(QString pathName)
{
		pathName = pathName.replace("_"," ");
			
		QString tname;
		const char * ename;
		//string sname;
		//QString lastStoredfileName;
		//struct expressedPathwayInfo infor;

		//pathName = pathName.replace(",","");
		QString fileName = "data/Reactome_Pathway_Data/pathwayTable/"+ pathName + ".path";
		QString num;
		QFile inputFile(fileName);
		if(inputFile.open(QIODevice::ReadOnly))
		{
			QTextStream in(&inputFile);
			QString line = in.readLine();  
			line = line.split('\n')[0];
			num=line;			  
		}	
		else
			return "";//continue;
					
		// e. g. "428359") //Binding of RNA by Insulin-like Growth Factor-2 mRNA Binding Proteins (IGF2BPs/IMPs/VICKZs)

		if(num =="")
		{
			pathName = pathName.replace(",","");
			QFile inputFile(fileName);
			if(inputFile.open(QIODevice::ReadOnly))
			{
				QTextStream in(&inputFile);
				QString line = in.readLine();  
				line = line.split('\n')[0];
				num=line;			  
			}	
			else 
				return "";
		}
		inputFile.close();
		return num;
}


void ItemBase::getLinkPoint(QPointF start, QPointF &end, QPointF dis, QRectF startRect, QRectF endRect)
{
	end = endRect.center();
	vector<QPointF> ends;
	ends.resize(4);
	float min=1000000;
	int mini=-1;
	for(int i=0; i<4; i++) //compare four nodes of endRect and output the one closest to start
	{
		float distance;
		if(i==0)
		{
			
			ends[i].setX( end.x() - endRect.width()/2 + dis.x());
			ends[i].setY( end.y() + dis.y());
			ends[i] -= this->pos();
		}
		else if(i==1)
		{
			
			ends[i].setX( end.x() + dis.x() );
			ends[i].setY( end.y() - endRect.height()/2 + dis.y() );
			ends[i] -= this->pos();
		}
		else if(i==2)
		{
			
			ends[i].setX( end.x() + endRect.width()/2 + dis.x() );
			ends[i].setY( end.y() + dis.y());
			ends[i] -= this->pos();
		}
		else if(i==3)
		{
			
			ends[i].setX( end.x() + dis.x() );
			ends[i].setY( end.y() + endRect.height()/2 + dis.y());
			ends[i] -= this->pos();
		}
		distance = pow(start.x() - ends[i].x(), 2) + pow(start.y() - ends[i].y(), 2);
		if(distance<min)
		{
		  min=distance, mini=i;
		}
	}
	if(mini<0)
		mini=0;  // a bug
	end = ends[mini];
}

QColor ItemBase::assignColorToExpressedItems(int pid, int type, int id, vector<set<vector<int>>> expressedSets, set<vector<int>> _itemColored, QColor oColor)
{
	vector<int> item(3,0);
	QColor color;
	item[0]=pid, item[1]=type, item[2]=id;

	bool flag=0;
	vector<int> temp;	 temp.push_back(m_pathwayID);  temp.push_back(type);    temp.push_back(id);   temp.push_back(GO_UP);
	if(_itemColored.empty())
	{
	    color = oColor; 
	    return color;	
	}
	if(_itemColored.find(temp)!=_itemColored.end())
	{
		color=UPEXPRESSIONCOLOR;			
		flag=1;
	}
	else
	{
		temp[3]=_DOWN;
		if(_itemColored.find(temp)!=_itemColored.end())
		{
			color=DOWNEXPRESSIONCOLOR;		 
			flag=1;
		}
		else 
		{
			temp[3]=_EVEN;
			if(_itemColored.find(temp)!=_itemColored.end())
		    {
				flag=1;
			}				
			color=oColor;//QColor(0,0,0,255);				
		}
	}	
    //color = QColor( 255, 255, 67, 255); 
	return color;	
}

QColor ItemBase::assignColorToDifferedItems(int pid, int type, int id)
{
	
	QColor color;
	/*
	vector<int> item(3,0);
	item[0]=pid, item[1]=type, item[2]=id;
	int cid=id;
	
	//cid=(cid)%5;	//light 		
	/*switch(cid)
	{
		//case 0:	 color=QColor( 239, 237, 245, 255);  break; ////brewer light qunatitive color 
		case 0:	 color=QColor( 218, 218, 235, 255);  break;
		case 1:	 color=QColor( 188, 189, 220, 255);  break;
		case 2:	 color=QColor( 158, 154, 200, 255);  break;
		case 3:	 color=QColor( 128, 125, 186, 255);  break;
		case 4:	 color=QColor( 106, 81, 163, 255);   break;			
		case 5:	 color=QColor( 84, 39, 143, 255);   break;			
		case 6:	 color=QColor( 63, 0, 125, 255);   break;			
		default: color=QColor( 255, 255, 255, 255);  break;
	}*/
	color=QColor( 255, 255, 67, 255); //light purple
	return color;
	/*cid=(cid+24)%28;	
	if(cid>=0)
	{
		switch(cid)
		{
			case 0:	 color=QColor( 255, 255, 179, 255); break; ////brewer light qunatitive color 
			case 1:	 color=QColor( 141, 211, 199, 255); break;
			case 2:	 color=QColor( 190, 186, 218, 255); break;
			case 3:	 color=QColor( 251, 128, 114, 255); break;
			case 4:	 color=QColor( 128, 177, 211, 255); break;
			case 5:	 color=QColor( 253, 180, 98, 255); break;
			case 6:	 color=QColor( 179, 222, 105, 255); break;
			case 7:	 color=QColor( 252, 205, 229, 255); break;
			case 8:	 color=QColor( 217, 217, 217, 255); break;
			case 10: color=QColor( 188, 128, 189, 255); break;
			case 11:    color=QColor( 166, 206, 227, 255); break; ////brewer dark qunatitivecolor 
			case 12:	color=QColor( 31, 120, 180, 255); break;
			case 13:	color=QColor( 51, 160, 44, 255); break;
			case 14:	color=QColor( 251, 154, 153, 255); break;
			case 15:	color=QColor( 227, 26, 28, 255); break;
			case 16:	color=QColor( 253, 191, 111, 255); break;
			case 17:	color=QColor( 255, 127, 0, 255); break;
			case 18:	color=QColor( 202, 178, 214, 255); break;
			case 20:    color=QColor( 106, 61, 154, 255); break; 			
			case 21:    color=QColor( 255, 120, 255, 255); break; //R, G, B
			case 22:    color=QColor( 255, 120, 120, 255); break;
			case 23:    color=QColor( 120, 255, 255, 255); break;
			case 24:    color=QColor( 255, 255, 120, 255); break;
			case 25:    color=QColor( 120, 255, 120, 255); break; 
			case 26:    color=QColor( 120, 120, 255, 255); break;
			case 27:    color=QColor( 255, 148, 28, 255); break;
			default:    color=QColor( 255, 255, 255, 255); break;
		}		
		return color;	
	}
	else return ( QColor( 255, 255, 255, 255));*/
}


QColor ItemBase::ColorPalette(int i)
{
	QColor color;
	int cid=i%28;	
	
		switch(cid)
		{
			case 0:	 color=QColor( 255, 255, 179, 255); break; ////brewer light qunatitive color 
			case 1:	 color=QColor( 141, 211, 199, 255); break;
			case 2:	 color=QColor( 190, 186, 218, 255); break;
			case 3:	 color=QColor( 251, 128, 114, 255); break;
			case 4:	 color=QColor( 128, 177, 211, 255); break;
			case 5:	 color=QColor( 253, 180, 98, 255); break;
			case 6:	 color=QColor( 179, 222, 105, 255); break;
			case 7:	 color=QColor( 252, 205, 229, 255); break;
			case 8:	 color=QColor( 217, 217, 217, 255); break;
			case 10: color=QColor( 188, 128, 189, 255); break;
			case 11:    color=QColor( 166, 206, 227, 255); break; ////brewer dark qunatitivecolor 
			case 12:	color=QColor( 31, 120, 180, 255); break;
			case 13:	color=QColor( 51, 160, 44, 255); break;
			case 14:	color=QColor( 251, 154, 153, 255); break;
			case 15:	color=QColor( 227, 26, 28, 255); break;
			case 16:	color=QColor( 253, 191, 111, 255); break;
			case 17:	color=QColor( 255, 127, 0, 255); break;
			case 18:	color=QColor( 202, 178, 214, 255); break;
			case 20:    color=QColor( 106, 61, 154, 255); break; 			
			case 21:    color=QColor( 255, 120, 255, 255); break; //R, G, B
			case 22:    color=QColor( 255, 120, 120, 255); break;
			case 23:    color=QColor( 120, 255, 255, 255); break;
			case 24:    color=QColor( 255, 255, 120, 255); break;
			case 25:    color=QColor( 120, 255, 120, 255); break; 
			case 26:    color=QColor( 120, 120, 255, 255); break;
			case 27:    color=QColor( 255, 148, 28, 255); break;
			default:    color=QColor( 255, 255, 255, 255); break;
		}
		return color;	
}


/*QPointF ItemBase::getRelativeDistance(QPointF dCenter1, float scale1, QPointF dCenter2, float scale2)
{   //the change before and after move or scale
    	
	int dcx1=dCenter1.x()*this->realRect().width(), dcy1=dCenter1.y()*this->realRect().height();
	int dcx2=dCenter2.x()*this->realRect().width(), dcy2=dCenter2.y()*this->realRect().height();			

	return QPointF(dcx2-dcx1, dcy2-dcy1);
}*/

QPointF ItemBase::getNewNodePoint(QPointF point, QPointF dCenter1, float scale1, QPointF dCenter2, float scale2)
{//reverse?
	float width1, height1, width2, height2;
	float x=point.x(), y=point.y();
	if(fixedSize)
	{
		width1=graphReferenceSize*scale1, height1=graphReferenceSize*scale1;
		width2=graphReferenceSize*scale2, height2=graphReferenceSize*scale2;
	}
	else
	{
		width1=this->realRect().width()*scale1;		height1=this->realRect().height()*scale1;
		width2=this->realRect().width()*scale2;		height2=this->realRect().height()*scale2;
	}	
	float dcx1=dCenter1.x()*this->realRect().width(), dcy1=dCenter1.y()*this->realRect().height();    
	float x1 = (( x - dcx1 ) + width1/2) /width1, y1 = (( y - dcy1 ) + height1/2) /height1;
    
	float cx2=x1*width2-width2/2, cy2=y1*height2-height2/2;	
	float dcx2=dCenter2.x()*this->realRect().width(), dcy2=dCenter2.y()*this->realRect().height();
	return QPointF(cx2+dcx2, cy2+dcy2);
}

QRectF ItemBase::getNewNodeRect(QRectF nodePos, int type, QPointF dCenter, float scale, QString reactionType)
{   //the change before and after move or scale
	//do not allow the rect smaller than ..
	int width, height;
	if(fixedSize)
	{
		width=graphReferenceSize*scale, height=graphReferenceSize*scale;
	}
	else
	{
		width=this->realRect().width()*scale;
		height=this->realRect().height()*scale;
	}
	float h=nodePos.height(), w=nodePos.width();	
    float cx=nodePos.x(), cy=nodePos.y();
	
	h=h*height, w=w*width;	
    cx=cx*width-width/2, cy=cy*height-height/2;
	
	if(h<0.04*height/3.0)
    {
        h=0.04*height/3.0; 
    }
	int dcx=dCenter.x()*this->realRect().width(), dcy=dCenter.y()*this->realRect().height();

	switch(type)
	{
	    case 'C':   
			h=10*scale;
		    if(h<6)  h=6;
		          w=h*2.0;
		    break;
		case 'R':
		    if(h<4) h=4;
		       w=h;	  
		    break;
		case 'L':
		    if(w>h*3.5)  w=h*3.5;
			else if(w<h*1.3)
				w=h*1.3;
		    if(h<16) {  
		       w=w/h*16;
			   h=16;					   
		    }
		    break;
		default:
			if(type!='M' && type!='L')
			{
				if(w<h*1.5)
			   	    w=h*1.5;
			}
			if(type=='M')
			{
			   if(w<80)  w=80;	if(h<16)  h=16;
			}
			else
			{
			   if(h<14)  {
			          w=w/h*14;
			          h=14;	}
			}
		         break; 
	}		
    QRect rect = QRect(cx+ dcx, cy+ dcy, w, h);
	if(type=='R')
	{
		if(reactionType=="D"||reactionType=="B") // dissociation and binding // will be drawn as elipse, so enlarge it a bit incase it looks smaller than rect
		{  
			float w=rect.width()*1.2,h=rect.height()*1.2;
			rect=QRect(rect.center().x()-w/2, rect.center().y()-h/2, w, h);
		}				
	}
    return rect;
}


void ItemBase::drawAngleText(QPainter *painter, int x, int y, QString qstr, float angle)
{
	//static const double Pi = 3.14159265358979323846264338327950288419717; 
	painter->save();
	painter->translate(x, y);
	painter->rotate(angle);

	painter->drawText(0, 0, qstr);

	painter->restore();
}

void ItemBase::drawAngleRect(QPainter *painter, QRectF rect, float angle)
{
	//static const double Pi = 3.14159265358979323846264338327950288419717; 
	painter->save();
	
	painter->translate(rect.x(), rect.y());
	painter->rotate(angle);

	QRectF rect2=QRect(0,0,rect.width(), rect.height());
	painter->drawRect(rect2);

	painter->restore();
}

QRectF ItemBase::getNodeRect(QRectF nodePos, QString nodeName, int type, bool fixedSize,  QPointF dCenter, float scale)
{	
	int w2,h2;	
	int width, height;
	if(fixedSize)
	{
		width=graphReferenceSize*scale, height=graphReferenceSize*scale;
	}
	else
	{
		width=this->realRect().width()*scale;
		height=this->realRect().height()*scale;
	}
	float h=nodePos.height(), w=nodePos.width();	
    float cx=nodePos.x(), cy=nodePos.y();
	
	h=h*height, w=w*width;	   
	
	if(h<0.04*graphReferenceSize/3.0)
    {
        h=0.04*graphReferenceSize/3.0; 
    }
	int dcx=dCenter.x()*this->realRect().width(), dcy=dCenter.y()*this->realRect().height();
	/*if(type=='C')
	{
	   h=10*scale;
	   cx=(cx+w/2.0-h*1.25); w=h*2.5;
	}
	else if(type=='R')
	{
	   cx=(cx+w/2.0-h*0.5);  w=h;	  
	}*/
	switch(type)
	{
	    case 'C':   
			h=10*scale;
		    if(h<6)  h=6;		 
	        
			w=h*2.5;
		    break;
		case 'R':
		    if(h<4) h=4;
		    //cx=(cx+w/2.0-h*0.5);  
			w=h;	    
		    break;
		case 'L':
		    if(w>h*3.5)  w=h*3.5;
			else if(w<h*1.3)
				w=h*1.3;
		    if(h<16) {  
		       w=w/h*16;
			   h=16;					   
		    }
		    break;
		default:
			if(type!='M')
			{
				if(w<h*1.5)
			   	    w=h*1.5;
			}			
			
			if(type=='M')
			{
			   if(w<80)  w=80;	if(h<16)  h=16;
			}
			else
			{
			   if(h<14)  {
			          w=w/h*14;
			          h=14;	}
			}
		         break; 
	}	
	cx=cx*width-width/2, cy=cy*height-height/2;
    QRect rect = QRect(cx+ dcx, cy+ dcy, w, h);
    return rect;
}

QRectF ItemBase::getrecttoRect(QRectF nodeRect, int type, bool fixedSize,  QPointF dCenter, float scale)
{
    float h=nodeRect.height(), w=nodeRect.width();	
	int width, height;
	if(fixedSize)
	{
		width=graphReferenceSize*scale, height=graphReferenceSize*scale;
	}
	else
	{
		width=this->realRect().width()*scale;
		height=this->realRect().height()*scale;
	}

	h=h*height, w=w*width;		
	if(h<0.04*graphReferenceSize/3.0)
    {
        h=0.04*graphReferenceSize/3.0; 
	}
    /*if(type=='C')
	{
	   w=h*2.5;
	}
	else if(type=='R')
	{
	   w=h;	  
	}*/
	switch(type)
	{
	    case 'C':   
			h=10*scale;
		    if(h<6)  h=6;		 
	        //cx=(cx+w/2.0-h*1.25);
			w=h*2.5;
		    break;
		case 'R':
		    if(h<4) h=4;
		    //cx=(cx+w/2.0-h*0.5);  
			w=h;	    
		    break;
		case 'L':
		    if(w>h*3.5)  w=h*3.5;
			else if(w<h*1.3)
				w=h*1.3;
		    if(h<16) {  
		       w=w/h*16;
			   h=16;					   
		    }
		    break;
		default: 
			if(type!='M')
			{
				if(w<h*1.5)
			   	    w=h*1.5;
			}
			if(type=='M')
			{
			   if(w<80)  w=80;	if(h<16)  h=16;
			}
			else
			{
			   if(h<14)  {
			          w=w/h*14;
			          h=14;	}
			}
		
		         break; 
	}	
	
	float cx=nodeRect.x(), cy=nodeRect.y();	
    cx=cx*width-width/2, cy=cy*height-height/2;	
	int dcx=dCenter.x()*this->realRect().width(), dcy=dCenter.y()*this->realRect().height();
    return QRectF(cx+ dcx, cy+ dcy, w, h);
}



QPointF ItemBase::moveDis( QPointF move, float scale)
{	
	int width, height;
	if(fixedSize)
	{
		width=graphReferenceSize*scale, height=graphReferenceSize*scale;
	}
	else
	{
		width=this->realRect().width()*scale;
		height=this->realRect().height()*scale;
	}
	float cx=move.x(), cy=move.y();
	cx=cx/width, cy=cy/height;
    return QPointF(cx,cy);
}


QRectF ItemBase::paintPreComputedNode( QPainter *painter,  float boundaryWidth, int fontSize, vector<QColor> insideColor, QColor boundaryColor, QColor fontColor, QRectF fontRect, QRectF nodeRect, QRectF nodePos, QString nodeName, int type, int id, vector<set<vector<int>>> sharedSets, QPointF dCenter, float scale, QString reactionType, float L, float B, float W, float H,  bool flag,  QColor c)
{	
    PathBubble1* pbubble = dynamic_cast<PathBubble1 *> (this);
	int w2,h2;
	//bool oflag=false; //make if the shared node are the origin
	int sharedP,sharedE,sharedS,sharedD,sharedR,sharedC;
	int width=graphReferenceSize*scale, height=graphReferenceSize*scale;
	
	float h=nodePos.height(), w=nodePos.width();	
    float cx=nodePos.x(), cy=nodePos.y();
	float edgescale;
		vector<int> item; 
	item.push_back(type); item.push_back(id);

	QRectF rect = nodeRect;
	h=nodeRect.height();
	w=nodeRect.width();

	int dcx=dCenter.x()*this->realRect().width(), dcy=dCenter.y()*this->realRect().height();
	
    QPen pen(boundaryColor); //?
	pen.setWidthF(boundaryWidth);
	painter->setPen(pen);	
	
    h=h/2, w=w/2;  

	cx=nodeRect.center().x();
	cy=nodeRect.center().y();
	//h2=0;
	if(type=='E')
	    edgescale=h*0.4;
	else
		edgescale=0;

	h2=h-(h<edgescale?h:edgescale); 
	w2=w-(w<edgescale?w:edgescale);  

	if(type=='C')
		type=type;

    const QPointF points[8] = 
	{
		QPointF(cx-w2, cy-h),
		QPointF(cx-w, cy-h2),		
		QPointF(cx-w, cy+h2),
		QPointF(cx-w2, cy+h),
		QPointF(cx+w2, cy+h),
		QPointF(cx+w, cy+h2),
		QPointF(cx+w, cy-h2),
		QPointF(cx+w2, cy-h)
	}; 

	const QPointF point6[6] = 
	{
		QPointF(cx-w/1.5, cy-h),
		QPointF(cx-w, cy),		
		QPointF(cx-w/1.5, cy+h),
		QPointF(cx+w/1.5, cy+h),
		QPointF(cx+w, cy),	
		QPointF(cx+w/1.5, cy-h),
	}; 
	
	painter->setBrush(insideColor[0]);  				
	switch(type)
	{
		case 'P':
	            if(insideColor.size()>1 )//&& insideColor[0]!=QColor(255,255,204, 0)) // 180, 255, 180, 0 //insideColor[0]=QColor(255,255,204, 0): orthology
				{
					//insideColor	
								
					float dx=5;	//the colored ribbon width	
					int ladd=0, radd=0;
					bool uflag=false, dflag=false, rflag=false, cflag=false, oflag=false, hflag=false;
					
					for(int i=1;i<insideColor.size();i++)
					{
					    if( insideColor[i]==UPEXPRESSIONCOLOR)//QColor(230,68,113,255));//"Up-expressed";QColor(231,78,120,255);	
						   uflag=true;
						else if( insideColor[i]==DOWNEXPRESSIONCOLOR )//QColor(81,81,255,255));//"Down-expressed";
						   dflag=true;	
						else if( insideColor[i]==RATELIMITCOLOR)//QColor(255, 180, 180,255)); //rate-limited
							rflag=true;
						else if(insideColor[i]==CROSSTALKCOLOR ) //QColor(180, 180, 255, 255)); str= "Cross-talking";	
							cflag=true;
						else if(insideColor[i]==ORTHOLOGYCOLOR ) //QColor(180, 180, 255, 255)); str= "Cross-talking";	
							oflag=true;
						else if(insideColor[i]==HIGHLIGHTCOLOR ) //QColor(180, 180, 255, 255)); str= "highlight";	
							hflag=true;
					}
				    if(uflag || dflag)
						ladd=1;
					if(rflag)
						radd++;
					if(cflag)
						radd++;
					if(oflag)
						radd++;


					QRectF rect1=QRectF(rect.x()-dx*ladd, rect.y(), rect.width()+dx*(ladd+radd), rect.height());

					
					
					painter->setPen(pen);	
					painter->setBrush(PROTEINCOLOR);  		
					painter->drawRoundedRect( rect1, 6, 6 );  		
					
					float x = rect1.x()+rect1.width()-dx;
					for(int i=1;i<insideColor.size();i++)
					{	
						if( insideColor[i]==RATELIMITCOLOR || insideColor[i]==CROSSTALKCOLOR || insideColor[i]==ORTHOLOGYCOLOR)//QColor(255, 180, 180,255)); //rate-limited
						{	
							QPainterPath path;						
							QRect cRect = QRect(x, rect1.y(), dx, rect1.height());						 
							QRect rRect = QRect(rect1.x(), rect1.y(), rect1.width(), rect1.height());
							QRegion cRegion = cRect;
							QRegion rRegion = roundRect(rRect, 6);

							path.addRegion(cRegion.intersect(rRegion));
							painter->fillPath( path, QBrush( insideColor[i] ) );
						
							x=x-dx;
						}
					}
					for(int i=1;i<insideColor.size();i++)
					{	
						if(  insideColor[i]==DOWNEXPRESSIONCOLOR || insideColor[i]==UPEXPRESSIONCOLOR)//QColor(255, 180, 180,255)); //rate-limited
						{	
							QPainterPath path;						
							QRect cRect = QRect(rect1.x(), rect1.y(), dx, rect1.height());						 
							QRect rRect = QRect(rect1.x(), rect1.y(), rect1.width(), rect1.height());
							QRegion cRegion = cRect;
							QRegion rRegion = roundRect(rRect, 6);

							path.addRegion(cRegion.intersect(rRegion));
							painter->fillPath( path, QBrush( insideColor[i] ) );
						}
					}
					painter->setBrush(Qt::NoBrush);
					painter->drawRoundedRect( rect1, 6, 6 );  		
				}
				else 
				{
					painter->setBrush(PROTEINCOLOR);  	
					painter->drawRoundedRect( rect, 6, 6 );   				
				}
		        break; 
		case 'C':	
			    painter->setBrush(COMPLEXCOLOR);  		
				painter->drawPolygon(points, 8);
				break;
		case 'S':  
		   	    painter->setBrush(SMALLMOLECULECOLOR);  		
				painter->drawEllipse( rect);    			  
				break;
		case 'D':   
		   	    painter->setBrush(DNACOLOR);  		
				painter->drawEllipse( rect );  		   
		         break; 
		case 'R':
			    painter->setBrush(REACTIONCOLOR);  	
			   if(reactionType=="D") // dissociation
			   {
                   drawDissociation(painter, rect);	    
			   }
			   // binding
			   else if(reactionType=="B")
			   {
				   painter->drawEllipse( rect);
			   }
			   else if(reactionType=="T")
			   {   
			       painter->drawRect( rect );
			   }  		      		   
			   break;
		case 'L':
			  painter->setBrush(SETCOLOR);  	
			  painter->drawPolygon(point6, 6);			
			  break;
		case 'E':  
			  painter->setBrush(PHYSICALENTITYCOLOR);  	
			  painter->drawPolygon(points, 8); break;		   
		      break;
	}
   painter->setPen(fontColor);  
   
   if(!flag) 
    { 
		
		if(type!='R' && type!='C')
		{
			QFont f("Arial",fontSize);	
			painter->setFont(f);
			QString testName=nodeName;
			if(testName.size()>18)
			{
				 for(int i=0;i<2;i++)
			     {
					int tindex0 = testName.lastIndexOf(" ");	
					if(tindex0 <0)
					   tindex0 = testName.lastIndexOf("/");	
					if(tindex0>9)				
					   testName.remove(tindex0,testName.size());
					if(testName.size()<18)
						break;
				 }
				 if(testName.size()>18)
					 testName.remove(18,testName.size());
			}
			nodeName=testName;
			painter->drawText(fontRect, Qt::AlignCenter|Qt::TextExpandTabs|Qt::TextWrapAnywhere, nodeName);//_complexName[id]);				
		}				
   }   
   return rect;
}

QRectF ItemBase::paintPreComputedNode_H( QPainter *painter,  float boundaryWidth, int fontSize, vector<QColor> insideColor, QColor boundaryColor, QColor fontColor, QRectF fontRect, QRectF nodeRect, QRectF nodePos, QString nodeName, int type, int id, vector<set<vector<int>>> sharedSets, QPointF dCenter, float scale, QString reactionType, float L, float B, float W, float H,  bool flag,  QColor c)
{	
    PathBubble1* pbubble = dynamic_cast<PathBubble1 *> (this);
	int w2,h2;
	//bool oflag=false; //make if the shared node are the origin
	int sharedP,sharedE,sharedS,sharedD,sharedR,sharedC;
	int width=graphReferenceSize*scale, height=graphReferenceSize*scale;
	
	float h=nodePos.height(), w=nodePos.width();	
    float cx=nodePos.x(), cy=nodePos.y();
	float edgescale;
	vector<int> item; 
	item.push_back(type); item.push_back(id);

	float exWidth=(HBWIDTH-2)*scale;
	QRectF rect = QRect(nodeRect.x()-exWidth, nodeRect.y()-exWidth, nodeRect.width()+exWidth*2, nodeRect.height()+exWidth*2);
	nodeRect=rect;
	h=nodeRect.height();
	w=nodeRect.width();

	int dcx=dCenter.x()*this->realRect().width(), dcy=dCenter.y()*this->realRect().height();
	
    QPen pen(boundaryColor); //?
	pen.setWidthF(boundaryWidth);
	painter->setPen(pen);	
	
    h=h/2, w=w/2;  

	cx=nodeRect.center().x();
	cy=nodeRect.center().y();
	//h2=0;
	if(type=='E')
	    edgescale=h*0.4;
	else
		edgescale=0;

	h2=h-(h<edgescale?h:edgescale); 
	w2=w-(w<edgescale?w:edgescale);  

	if(type=='C')
		type=type;

    const QPointF points[8] = 
	{
		QPointF(cx-w2, cy-h),
		QPointF(cx-w, cy-h2),		
		QPointF(cx-w, cy+h2),
		QPointF(cx-w2, cy+h),
		QPointF(cx+w2, cy+h),
		QPointF(cx+w, cy+h2),
		QPointF(cx+w, cy-h2),
		QPointF(cx+w2, cy-h)
	}; 

	const QPointF point6[6] = 
	{
		QPointF(cx-w/1.5, cy-h),
		QPointF(cx-w, cy),		
		QPointF(cx-w/1.5, cy+h),
		QPointF(cx+w/1.5, cy+h),
		QPointF(cx+w, cy),	
		QPointF(cx+w/1.5, cy-h),
	}; 
	
	painter->setPen(Qt::NoPen);
	painter->setBrush(PRIME_PATH_COLOR);  				
	switch(type)
	{
		case 'P':
	            if(insideColor.size()>1 )//&& insideColor[0]!=QColor(255,255,204, 0)) // 180, 255, 180, 0 //insideColor[0]=QColor(255,255,204, 0): orthology
				{
					//insideColor									
					float dx=4;		
					int ladd=0, radd=0;
					bool uflag=false, dflag=false, rflag=false, cflag=false, oflag=false, hflag=false;
					
					for(int i=1;i<insideColor.size();i++)
					{
					    if( insideColor[i]==UPEXPRESSIONCOLOR)//QColor(230,68,113,255));//"Up-expressed";QColor(231,78,120,255);	
						   uflag=true;
						else if( insideColor[i]==DOWNEXPRESSIONCOLOR )//QColor(81,81,255,255));//"Down-expressed";
						   dflag=true;	
						else if( insideColor[i]==RATELIMITCOLOR)//QColor(255, 180, 180,255)); //rate-limited
							rflag=true;
						else if(insideColor[i]==CROSSTALKCOLOR ) //QColor(180, 180, 255, 255)); str= "Cross-talking";	
							cflag=true;
						else if(insideColor[i]==ORTHOLOGYCOLOR ) //QColor(180, 180, 255, 255)); str= "Cross-talking";	
							oflag=true;
						else if(insideColor[i]==HIGHLIGHTCOLOR ) //QColor(180, 180, 255, 255)); str= "highlight";	
							hflag=true;
					}
				    if(uflag || dflag)
						ladd=1;
					if(rflag)
						radd++;
					if(cflag)
						radd++;
					if(oflag)
						radd++;

					QRectF rect1=QRectF(rect.x()-dx*ladd, rect.y(), rect.width()+dx*(ladd+radd), rect.height());					
						
					//painter->setBrush(HIGHLIGHTCOLOR); 
					painter->drawRoundedRect( rect1, 6, 6 ); 
				}
				else
				{   //painter->setPen(pen);	
					//painter->setBrush(HIGHLIGHTCOLOR);  		
					painter->drawRoundedRect( rect, 6, 6 );  		
				}
		        break; 
		case 'C':			
				painter->drawPolygon(points, 8);
				break;
		case 'S':  
		   	
				painter->drawEllipse( rect);    			  
				break;
		case 'D':   
		   	
				painter->drawEllipse( rect );  		   
		         break; 
		case 'R':
			   if(reactionType=="D") // dissociation
			   {
                   drawDissociation(painter, rect);	    
			   }
			   // binding
			   else if(reactionType=="B")
			   {
				   painter->drawEllipse( rect);
			   }
			   else if(reactionType=="T")
			   {   
			       painter->drawRect( rect );
			   }  		      		   
			   break;
		case 'L':			 
			  painter->drawPolygon(point6, 6);			
			  break;
		case 'E':  
			  painter->drawPolygon(points, 8); break;		   
		      break;
	}
   return rect;
}

QRectF ItemBase::paintNode( QPainter *painter, QRectF nodePos, QString nodeName, int type, int id, vector<set<vector<int>>> sharedSets, vector<set<vector<int>>> expressedSets, bool fixedSize, QPointF dCenter, float scale, QString reactionType, float L, float B, float W, float H,  bool flag,  QColor c)
{	
	PathBubble1* pbubble = dynamic_cast<PathBubble1 *> (this);
	int w2,h2;
	//bool oflag=false; //make if the shared node are the origin
	int sharedP,sharedE,sharedS,sharedD,sharedR,sharedC;
	int width, height;
	if(fixedSize)
	{
		width=graphReferenceSize*scale, height=graphReferenceSize*scale;
	}
	else
	{
		width=this->realRect().width()*scale;
		height=this->realRect().height()*scale;
	}
	float h=nodePos.height(), w=nodePos.width();	
    float cx=nodePos.x(), cy=nodePos.y();
	float edgescale;
	cx=(cx-L)/W; cy=(cy-B)/H; 
    w=w/W, h=h/H;
	

	h=h*height, w=w*width;	
    cx=cx*width-width/2, cy=cy*height-height/2;
	
	if(h<0.04*graphReferenceSize/3.0)
    {
        h=0.04*graphReferenceSize/3.0; 
    }

	//h=h+4;
	int dcx=dCenter.x()*this->realRect().width(), dcy=dCenter.y()*this->realRect().height();

	if(type=='C')
	{
	   cx=(cx+w/2.0-h*1.25); w=h*2.5;
	}
	else if(type=='R')
	{
	   cx=(cx+w/2.0-h*0.5);  w=h;	  
	}

   QRect rect = QRect(cx+ dcx, cy+ dcy, w, h);
	//QRect rect = QRect(cx+ dCenter.x(), cy+ dCenter.y(), w, h);

	vector<int> item; 
	item.push_back(type); item.push_back(id);
	
	
	if(flag || (!hoveredItem.empty()&&hoveredItem[0]==m_pathwayID && hoveredItem[1]==item[0] && hoveredItem[2]==item[1]) )
	{
		QPen pen(c);
		pen.setWidthF(4);
		painter->setPen(pen);			
		//oflag=true;
	}
	else if(highlighted.find(item)!=highlighted.end())
	{
	    c=QColor(255,100,100,255);
		QPen pen(c);
		pen.setWidthF(4);
		painter->setPen(pen);	
	}
	else if(searched.find(item)!=searched.end())
	{
		c=QColor(80,80,255,255);
		QPen pen(c);
		pen.setWidthF(4);
		painter->setPen(pen);	
	}
    else 
	{
		QPen pen(QColor(102,102,102,255));
		pen.setWidthF(1);
		painter->setPen(pen);	
	}

	  //for complex	
      h=h/2, w=w/2;   

	  cx= cx + w + dcx, cy= cy + h + dcy;      
      h=h+2;
	  h2=0;
	  edgescale=h2*0.5;
	  h2=h-(h<edgescale?h:edgescale); 
	  w2=w-(w<edgescale?w:edgescale);  


       const QPointF points[8] = 
       {
		QPointF(cx-w2, cy-h),
		QPointF(cx-w, cy-h2),		
		QPointF(cx-w, cy+h2),
		QPointF(cx-w2, cy+h),
		QPointF(cx+w2, cy+h),
		QPointF(cx+w, cy+h2),
		QPointF(cx+w, cy-h2),
		QPointF(cx+w2, cy-h)
      }; 
	
	   //if(rect.width()>20)
		 //  return rect;
	   
    if(flag)
	{
		 painter->setBrush(Qt::NoBrush);
		 switch(type)
		 {
			 case 'P':
					painter->drawRoundedRect( rect, edgescale*0.75, edgescale*0.75); break;
			 case 'C':
					painter->drawPolygon(points, 8); break;
			 case 'S':
					painter->drawEllipse( rect );	break;
			 case 'D':
					painter->drawRect( rect ); break;
			 case 'E':
					painter->drawRoundedRect( rect, edgescale*0.75, edgescale*0.75); break;
			 case 'R':
					 if(reactionType=="D") // dissociation
					 {
						painter->drawEllipse( rect);
	   				 }
					 else if(reactionType=="B") //binding
					 {
						painter->drawEllipse( rect);
					 }
					 else if(reactionType=="T") //transition
					 {
						painter->drawRect( rect );
					}            
				   break;
		 }
   }

  sharedP = sharedC = sharedS = sharedD = sharedR = sharedE = -1;
  vector< set<int> > Protein, Complex, SmallMolecule, Dna, Reaction, PhysicalEntity, ANode;//sharedCompartment;   
  for(int j=0; j<4; j++)
  {
   
	   switch(j)
	   {
			 case 0: Protein = sharedProtein, Complex = sharedComplex, SmallMolecule = sharedSmallMolecule, Dna = sharedDna, Reaction = sharedReaction, PhysicalEntity = sharedPhysicalEntity, ANode = sharedANode;  break;
			 case 1: Protein = differedProtein, Complex = differedComplex, SmallMolecule = differedSmallMolecule, Dna = differedDna, Reaction = differedReaction, PhysicalEntity = differedPhysicalEntity, ANode = differedANode;  break;
			 case 2: Protein = linkedProtein, Complex = linkedComplex, SmallMolecule = linkedSmallMolecule, Dna = linkedDna, Reaction = linkedReaction, PhysicalEntity = linkedPhysicalEntity, ANode = linkedANode;  break;
			 case 3: Protein = expressedProtein, Complex = expressedComplex, SmallMolecule = expressedSmallMolecule, Dna = expressedDna, Reaction = expressedReaction, PhysicalEntity = expressedPhysicalEntity, ANode = expressedANode; break;
	   }	
	   switch(type)
	   {
		   case 'P':
		   if(sharedP==-1 )
		   {
				for(int i=0; i<Protein.size();i++)
				{
					set<int> record=Protein[i];
					if(record.find(id)!=record.end())
					{
						sharedP=i;
					}   
				}
				if(sharedP!=-1)
				{
					if(j==0)//||highlighted.find(item)!=highlighted.end())
						painter->setBrush(assignColorToSharedItems(m_pathwayID, type, id, sharedSets));
					else if(j==1)
						painter->setBrush(assignColorToDifferedItems(m_pathwayID, type, id)); //(QColor(255,255,100,255)); //painter->setBrush(QColor(190,186,218,255));  
					else if(j==2)
					{
						float level=pbubble->findLinkedLevel(m_pathwayID, type, id);
						float totalLevel=pbubble->getTotalLinkLevel();
						//painter->setBrush(QColor(255, 255,179,255)); 
						//painter->setBrush(QColor(255.0, 255.0, 255.0*level, totalLevel,255)); //painter->setBrush(QColor(251,128,114,255));  						
						painter->setBrush(assignColorToLinkedItems(level, totalLevel));
					}
					else if(j==3)
						painter->setBrush(assignColorToExpressedItems(m_pathwayID, type, id, expressedSets,_itemColored, QColor(204,235,197,255)));			
				}
				else
					painter->setBrush(QColor(204,235,197,255));  
				painter->drawRoundedRect( rect, 6, 6 );   
			} 
		   break; 
		   case 'C':
		   if(sharedC==-1 )
		   {
				for(int i=0; i<Complex.size();i++)
				{
					set<int> record=Complex[i];
					if(record.find(id)!=record.end())
					{
						sharedC=i;
					}   
				} 
				if(sharedC!=-1)
				{	
					if(j==0)//||highlighted.find(item)!=highlighted.end())
						painter->setBrush(assignColorToSharedItems(m_pathwayID, type, id, sharedSets));
					else if(j==1)
						painter->setBrush(assignColorToDifferedItems(m_pathwayID, type, id)); //(QColor(255,255,100,255));  //(255,255,179,255) painter->setBrush(QColor(190,186,218,255));  
					else if(j==2)
					{
						//painter->setBrush(QColor(255,255,179,255));  //painter->setBrush(QColor(251,128,114,255)); 
						float level=pbubble->findLinkedLevel(m_pathwayID, type, id);
						float totalLevel=pbubble->getTotalLinkLevel();
						//painter->setBrush(QColor(255.0, 255.0, 255.0*level, totalLevel,255)); //painter->setBrush(QColor(251,128,114,255));  
						painter->setBrush(assignColorToLinkedItems(level, totalLevel));
					}
					else if(j==3)
						painter->setBrush(assignColorToExpressedItems(m_pathwayID, type, id, expressedSets,_itemColored,QColor(222,203,228,255)));  //QColor(255,255,179,255)
				}
				else
					painter->setBrush(QColor(222,203,228,255));    
				painter->drawPolygon(points, 8);
		   }
		   break;
		case 'S':  
		   if(sharedS==-1 )
		   {
				for(int i=0; i<SmallMolecule.size();i++)
				{
					set<int> record=SmallMolecule[i];
					if(record.find(id)!=record.end())
					{
						sharedS=i;
					}   
				}
				if(sharedS!=-1)
				{	
					if(j==0)//||highlighted.find(item)!=highlighted.end())
						painter->setBrush(QColor(255,255,100,255));  //QColor(255,255,179,255)
					else if(j==1)
						painter->setBrush(QColor(255,255,100,255));  //painter->setBrush(QColor(190,186,218,255));  
					else if(j==2)
						painter->setBrush(QColor(255,255,100,255));  //painter->setBrush(QColor(251,128,114,255)); 
					else if(j==3)
						painter->setBrush(QColor(255,255,100,255)); 
				}	 
				else
					  painter->setBrush(QColor(199,253,205,255));  
				float th=rect.height(),tw=rect.width();
				float tth=nodePos.height(),ttw=nodePos.width();
				painter->drawEllipse( rect);    			  
		  }
		  break;
		case 'D':   
		   if(sharedD==-1 )
		   {
				for(int i=0; i<Dna.size();i++)
				{
					set<int> record=Dna[i];
					if(record.find(id)!=record.end())
					{
						sharedD=i;
					}   
				} 
				if(sharedD!=-1)
				{	
					if(j==0)//||highlighted.find(item)!=highlighted.end())
						painter->setBrush(QColor(255,255,100,255));  
					else if(j==1)
						painter->setBrush(QColor(255,255,100,255));  //painter->setBrush(QColor(190,186,218,255));  
					else if(j==2)
						painter->setBrush(QColor(255,255,100,255));  //painter->setBrush(QColor(251,128,114,255)); 
					else if(j==3)
						painter->setBrush(QColor(255,255,100,255)); 
				}	
				else
					painter->setBrush(QColor(199,253,205,255));  
				painter->drawRect( rect );  
		   }
		   break; 
		case 'R':   
		   if(sharedR==-1 )
		   {
			   for(int i=0; i<Reaction.size();i++)
			   {
		  		  set<int> record=Reaction[i];
				  if(record.find(id)!=record.end())
				   		   sharedR=i;   
			   }   
			   if(reactionType=="D") // dissociation
			   {
					 if(sharedR==-1)
						 painter->setBrush(QColor(255,255,255,255));	   
					 else 
					 {
						 if(j==0)//||highlighted.find(item)!=highlighted.end())
							painter->setBrush(QColor(255,255,100,255));  
						else if(j==1)
							painter->setBrush(QColor(255,255,100,255));  //painter->setBrush(QColor(190,186,218,255));  
						else if(j==2)
							painter->setBrush(QColor(255,255,100,255));  //painter->setBrush(QColor(251,128,114,255)); 
						else if(j==3)
							painter->setBrush(QColor(255,255,100,255)); 				 
					 }
					 painter->drawEllipse( rect);	    
			   }
			   // binding
			   else if(reactionType=="B")
			   {
				 if(sharedR==-1)
					 painter->setBrush(QColor(0,0,0,255));		
				 else
				 {
					if(j==0)//||highlighted.find(item)!=highlighted.end())
						painter->setBrush(QColor(255*0.7, 255*0.7, 100*0.7, 255));  
					else if(j==1)
						painter->setBrush(QColor(255*0.7, 255*0.7, 100*0.7, 255)); //painter->setBrush(QColor(190*0.7, 186*0.7, 218*0.7, 255));  
					else if(j==2)
						painter->setBrush(QColor(255*0.7, 255*0.7, 100*0.7, 255)); //painter->setBrush(QColor(251*0.7, 128*0.7, 114*0.7, 255)); 				 
					else if(j==3)
						painter->setBrush(QColor(255*0.7, 255*0.7, 100*0.7, 255)); 	
				 }			 
				 painter->drawEllipse( rect);
			   }
			   else if(reactionType=="T")
			   {   if(sharedR==-1)
				        painter->setBrush(QColor(255,255,255,255));					 
				   else 
				   {
					 if(j==0)//||highlighted.find(item)!=highlighted.end())
						painter->setBrush(QColor(255,255,100,255));  
					else if(j==1)
						painter->setBrush(QColor(255,255,100,255));  //painter->setBrush(QColor(190,186,218,255));  
					else if(j==2)
						painter->setBrush(QColor(255,255,100,255));  //painter->setBrush(QColor(251,128,114,255)); 				 
					else if(j==3)
						painter->setBrush(QColor(255,255,100,255));  //painter->setBrush(QColor(255,255,100,255)); 	
				  }	   			 			 			  
				  painter->drawRect( rect );
			   }  		      
		   }
		   break;
		case 'E':    
		   if(sharedE==-1 )
		   {
			  for(int i=0; i<PhysicalEntity.size();i++)
			  {
				 set<int> record=PhysicalEntity[i];
				 if(record.find(id)!=record.end())
				 {
					 sharedE=i;
				 }   
			  } 
			  if(sharedE!=-1)
			  {	
					if(j==0)//||highlighted.find(item)!=highlighted.end())
						painter->setBrush(QColor(255,255,100,255));  
					else if(j==1)
						painter->setBrush(QColor(255,255,100,255));  //painter->setBrush(QColor(190,186,218,255));  
					else if(j==2)
						painter->setBrush(QColor(255,255,100,255));  //painter->setBrush(QColor(251,128,114,255)); 
					else if(j==3)
						painter->setBrush(QColor(255,255,100,255)); 
				}	
			  else
				 painter->setBrush(QColor(199,253,205,255));  
			  painter->drawRoundedRect( rect, 6, 6 ); 
		   }
		   break;
		   }
   }
   painter->setPen(QColor(0,0,0,255));  
   int fontsize=(width+height)/8;//9*(width+height)/600.0; if(fontsize<2) fontsize=2; else if(fontsize>28) fontsize=28; 
   //if(fontsize>28) fontsize=28; 

   w=w*width;
   
   if(!flag) 
    { 
		//doc.drawContents( painter, rect.translated( -rect.topLeft() ) );
		bool flag=0;
		vector<int> temp;	 temp.push_back(m_pathwayID);   temp.push_back(type);    temp.push_back(id);   temp.push_back(GO_UP);
		if(_itemColored.find(temp)!=_itemColored.end())
		{
			painter->setPen(QColor(228,26,28));	
			flag=1;
		}
		else
		{
			temp[2]=_DOWN;
		    if(_itemColored.find(temp)!=_itemColored.end())
		    {
				painter->setPen(QColor(77,175,74));
				flag=1;
			}
			else 
			{
			    temp[2]=_EVEN;
				if(_itemColored.find(temp)!=_itemColored.end())
		        {
					painter->setPen(QColor(0,0,0,255));			
					flag=1;
				}
				else painter->setPen(QColor(0,0,0,255));
			}
		}	
	 	
		QRect rect2;
		if(type!='R' && type!='C')
		{
			QRectF fontRect;

			getNodeFont(fontsize, fontRect, rect, nodeName, type, width, height, fixedSize, scale);
		    
			QFont f("Arial",fontsize);	
			painter->setFont(f);
			painter->drawText(fontRect, Qt::TextExpandTabs|Qt::TextWrapAnywhere, nodeName);//_complexName[id]);				
		}				
   }   
   return rect;
}

QRectF ItemBase::getCompartmentToBePaint(float &boundaryWidth, int &fontSize, vector<QColor> &insideColor, QColor &boundaryColor, QColor &fontColor, QRectF &fontRect,vector<QRectF> compartmentPos, QString _compartmentName, int id, vector<set<vector<int>>> sharedSets, vector<set<vector<int>>> expressedSets, QPointF dCenter, float scale, bool flag,  QColor c)
{ 
   insideColor.resize(1);
   boundaryColor = QColor(0,0,0,255);
   //pathwaySelected=-1;
   int sharedS=-1;  
   for(int i=0; i<sharedCompartment.size();i++)
   {
       set<int> record=sharedCompartment[i];
	   if(record.find(id)!=record.end())
	   {
		  sharedS=i;
		  //boundaryColor=QColor(255, 255, 67, 255);
	   }   
   } 
   int sharedL=-1;  
   for(int i=0; i<linkedCompartment.size();i++)
   {
       set<int> record=linkedCompartment[i];
	   if(record.find(id)!=record.end())
	   {
		  sharedL=i;
		  //boundaryColor=QColor(255, 255, 67, 255);
	   }   
   } 

   int sharedD=-1;  
   for(int i=0; i<differedCompartment.size();i++)
   {
       set<int> record=differedCompartment[i];
	   if(record.find(id)!=record.end())
	   {
		  sharedD=i;
		 // boundaryColor=QColor(255, 255, 67, 255);
	   }   
   } 

   int sharedE=-1;  
   for(int i=0; i<expressedCompartment.size();i++)
   {
       set<int> record=expressedCompartment[i];
	   if(record.find(id)!=record.end())
	   {
		  sharedE=i;
		  //boundaryColor=QColor(255, 255, 67, 255);
	   }   
   } 
   int level=-1;
   int width = graphReferenceSize * scale,   height = graphReferenceSize * scale;    

      
   QRectF rect = getNewNodeRect(compartmentPos[id], 'M', dCenter, scale);
   boundaryWidth=2;

   vector<int> item; 
   item.push_back('M'); item.push_back(id);   
  
   if(((!hoveredItem.empty()&&hoveredItem[0]==m_pathwayID && hoveredItem[1]==item[0] && hoveredItem[2]==item[1]))||foundlist.find(item)!=foundlist.end()||highlighted.find(item)!=highlighted.end())
   {
	   //pen.setWidthF(4);
	   boundaryColor = QColor(96,96,96,255);
	   boundaryWidth=4;
   }
   else 
   {	
	   //pen.setWidthF(2);
	   boundaryWidth=2;
   }
   //painter->setPen(pen);	
    

   //***
   insideColor[0]=QColor(0,0,0,255);
   
   QColor color;
   if(sharedS<0 && sharedL<0 && sharedD<0 && sharedE<0)
   {
	    //****
	    insideColor[0]=QColor(0,0,0,255);		
	    //painter->setBrush(Qt::NoBrush);
   }
   else 
   {	
	    //painter->setBrush(Qt::NoBrush);
	    //****
	    //insideColor[0]=QColor(0,0,0,255);
		boundaryWidth=2;
        //pen.setWidthF(4);

	    vector<int> item;
	    item.push_back('M');  item.push_back(id);
		
		if(sharedS>=0)		
			color=HIGHLIGHTCOLOR; //assignColorToSharedItems(m_pathwayID, 'M', id, sharedSets);
		else if(sharedD>=0)
			color=HIGHLIGHTCOLOR; //assignColorToDifferedItems(m_pathwayID, 'M', id);
		else if(sharedL>=0)
		{
			PathBubble1* pbubble = dynamic_cast<PathBubble1 *> (this);
			level=pbubble->findLinkedLevel(m_pathwayID, 'M', id);
			float totalLevel=pbubble->getTotalLinkLevel();			
			color=HIGHLIGHTCOLOR; //assignColorToLinkedItems(level, totalLevel); 
		}
		else if(sharedE>=0)
			color=assignColorToExpressedItems(m_pathwayID, 'M', id, expressedSets,_itemColored, QColor(0,0,0,255)); 		
		else 
			color=QColor(0,0,0,255);

		//pen.setColor(color);
		///***
		insideColor[0]=color;
	}
   if(flag || (!hoveredItem.empty() && hoveredItem[0]==m_pathwayID && hoveredItem[1]==item[0] && hoveredItem[2]==item[1]) )
	{
		boundaryWidth=3; 		
	}
   else if(highlighted.find(item)!=highlighted.end())
   {		
		c= MIDIUM_HIGHLIGHTCOLOR;
		boundaryColor=c;
		boundaryWidth=3;		
	}
	else if(searched.find(item)!=searched.end())
	{
		c= MIDIUM_HIGHLIGHTCOLOR;	
		boundaryWidth=3;
		boundaryColor=c;
	}
    else if(level==0)
	{
		c= MIDIUM_HIGHLIGHTCOLOR;
		boundaryColor=c;
		boundaryWidth=3;		
	}
	else
	{
		boundaryWidth=2;	
		//boundaryColor=c;
	}

   fontColor=QColor(color.red()*0.0, color.green()*0.0, color.blue()*0.0, 255);
   getCompartmentFont(fontSize, fontRect, rect, _compartmentName, fixedSize, scale);
   //boundaryColor=QColor(255, 255, 67, 255);
   return rect;
   
}


void ItemBase::getCompartmentFont(int &fontSize, QRectF &fontRect, QRectF compartmentRect, QString compartmentName, bool fixedSize, float scale)
{
  
   if(fixedSize)
   {
		fontSize=14*scale; 
		if(fontSize<10) fontSize=10; else if(fontSize>24) 
			fontSize=24;
   }
   else
   {   
		fontSize=6*(this->realRect().width()+this->realRect().height())/graphReferenceSize*scale; //if(fontSize<9) fontSize=9; else if(fontSize>18) fontSize=18; 
   }
 
   QFont f1("Arial",fontSize);		          
   QFontMetrics fontMetrics(f1); 
   QRectF rect3 = fontMetrics.boundingRect(0, 0, 0, 0, Qt::TextExpandTabs|Qt::TextWrapAnywhere, "A"); 		
   do
   {
		fontSize=fontSize-1;	
		QFont f("Arial",fontSize);		        	
		QFontMetrics fontMetrics(f);
		fontRect = fontMetrics.boundingRect(0, 0, this->realRect().width(), rect3.height(), Qt::TextExpandTabs|Qt::TextWrapAnywhere, compartmentName); 		
   }while((fontRect.width()*1.1)>compartmentRect.width() && fontSize>6);
  
   if(fontRect.height()>compartmentRect.height())
		fontRect=compartmentRect;	   
   //fontRect=compartmentRect;	
}

QRectF ItemBase::getCompartmentFont(QString compartmentName, bool fixedSize, float scale)
{
   int fontSize;
   if(fixedSize)
   {
		fontSize=10*scale;  
   }  
   QRectF fontRect;
   QFont f1("Arial",fontSize);		          
   QFontMetrics fontMetrics(f1); 
   QRectF rect3 = fontMetrics.boundingRect(compartmentName); 		
   rect3.setWidth(rect3.width()*1.1);
   
   return rect3;   
}

QRectF ItemBase::getNodetoBePaint(float &boundaryWidth, int &fontSize, vector<QColor> &insideColor, QColor &boundaryColor, QColor &fontColor, QRectF &fontRect, QRectF nodePos, QString nodeName, int type, int id, vector<set<vector<int>>> sharedSets, vector<set<vector<int>>> expressedSets, QPointF dCenter, float scale, QString reactionType, float L, float B, float W, float H,  bool flag,  QColor c)
{	
	//QPainter *painter, 
	PathBubble1* pbubble = dynamic_cast<PathBubble1 *> (this);
	insideColor.resize(1);
	//bool oflag=false; //make if the shared node are the origin
	int sharedP,sharedE,sharedS,sharedD,sharedR,sharedC,sharedA;
	int width=graphReferenceSize*scale, height=graphReferenceSize*scale;
	
	QRectF rect = getNewNodeRect(nodePos, type, dCenter, scale, reactionType);	

	vector<int> item; 
	item.push_back(type); item.push_back(id);
	
     int w2,h2;
	float edgescale;
    float h=rect.height()/2, w=rect.width()/2;   
	float cx= rect.center().x(), cy= rect.center().y();      
	h=h;
	h2=0;
	edgescale=h2*0.5;
	h2=h-(h<edgescale?h:edgescale); 
	w2=w-(w<edgescale?w:edgescale);  

    const QPointF points[8] = 
    {
		QPointF(cx-w2, cy-h),
		QPointF(cx-w, cy-h2),		
		QPointF(cx-w, cy+h2),
		QPointF(cx-w2, cy+h),
		QPointF(cx+w2, cy+h),
		QPointF(cx+w, cy+h2),
		QPointF(cx+w, cy-h2),
		QPointF(cx+w2, cy-h)
    }; 
	float level=-1;
 
  //bool shared, differed, expressed, linked;
  //shared = differed = expressed = linked=false;
  sharedP = sharedC = sharedS = sharedD = sharedR = sharedE = sharedA=-1;
  vector< set<int> > Protein, Complex, SmallMolecule, Dna, Reaction, PhysicalEntity, ANode;//, sharedCompartment;   
  if(!linkedProtein.empty()&& !linkedProtein[0].empty())
	  linkedProtein=linkedProtein;
  for(int j=0; j<4; j++)
  {   
	   switch(j)
	   {
			 case 0: Protein = sharedProtein, Complex = sharedComplex, SmallMolecule = sharedSmallMolecule, Dna = sharedDna, Reaction = sharedReaction, PhysicalEntity = sharedPhysicalEntity, ANode = sharedANode; break;
			 case 1: Protein = differedProtein, Complex = differedComplex, SmallMolecule = differedSmallMolecule, Dna = differedDna, Reaction = differedReaction, PhysicalEntity = differedPhysicalEntity, ANode = differedANode;  break;
			 case 2: Protein = linkedProtein, Complex = linkedComplex, SmallMolecule = linkedSmallMolecule, Dna = linkedDna, Reaction = linkedReaction, PhysicalEntity = linkedPhysicalEntity,  ANode = linkedANode; break;
			 case 3: Protein = expressedProtein, Complex = expressedComplex, SmallMolecule = expressedSmallMolecule, Dna = expressedDna, Reaction = expressedReaction, PhysicalEntity = expressedPhysicalEntity, ANode = expressedANode;  break;
			 //case 4: Protein = expressedProtein_edit, Complex = expressedComplex, SmallMolecule = expressedSmallMolecule, Dna = expressedDna, Reaction = expressedReaction, PhysicalEntity = expressedPhysicalEntity, ANode = expressedANode;  break;	 expressedDna_Edit
	   }	
	   switch(type)
	   {
		   case 'P':
		   if(sharedP==-1 )
		   {
			    for(int i=0; i<Protein.size();i++)
				{
					set<int> record=Protein[i];
					if(record.find(id)!=record.end())
					{
						sharedP=i;
					}   
				}
				if(sharedP!=-1)
				{
					insideColor[0]=QColor(204,235,197,255);
					if(j==0)//||highlighted.find(item)!=highlighted.end())
					{
                        //insideColor.push_back(HIGHLIGHTCOLOR); //assignColorToSharedItems(m_pathwayID, type, id, sharedSets));						
						insideColor[0]=HIGHLIGHTCOLOR;
					}
					else if(j==1)
					{
						//insideColor.push_back(assignColorToDifferedItems(m_pathwayID, type, id));
						insideColor[0]=HIGHLIGHTCOLOR; //=assignColorToDifferedItems(m_pathwayID, type, id);
					}
					else if(j==2)
					{
						level=pbubble->findLinkedLevel(m_pathwayID, type, id);					
						//float totalLevel=pbubble->getTotalLinkLevel();
						insideColor[0]=HIGHLIGHTCOLOR; //assignColorToLinkedItems(level, totalLevel);
						//insideColor.push_back(assignColorToLinkedItems(level, totalLevel));						
					}
					else if(j==3)
					{
						insideColor.push_back(assignColorToExpressedItems(m_pathwayID, type, id, expressedSets,_itemColored,PROTEINCOLOR));						
						//insideColor[0]=assignColorToExpressedItems(m_pathwayID, type, id, expressedSets,_itemColored,PROTEINCOLOR);
					}	
				}
				else
				{
					if( expressedProtein_Edit.find(id)!=expressedProtein_Edit.end())
					{
						vector<int> temp;	 temp.push_back(m_pathwayID);  temp.push_back(type);    temp.push_back(id);   temp.push_back(_EVEN);
						if(_itemColored.find(temp)==_itemColored.end())
						{	
							insideColor[0]=PROTEINCOLOR;
						    insideColor.resize(1);
					        insideColor.push_back(assignColorToExpressedItems(m_pathwayID, type, id, expressedSets,_itemColored,PROTEINCOLOR));			
						}
						else
							insideColor[0]=PROTEINCOLOR;//QColor(204,235,197,255);					
					}
					else insideColor[0]=PROTEINCOLOR;//QColor(204,235,197,255);			
				}
				//painter->drawRoundedRect( rect, 6, 6 );   
			} 
		   break; 
		   case 'C':
		   if(sharedC==-1 )
		   {
				for(int i=0; i<Complex.size();i++)
				{
					set<int> record=Complex[i];
					if(record.find(id)!=record.end())
					{
						sharedC=i;
					}   
				} 
				if(sharedC!=-1)
				{	
					if(j==0)//||highlighted.find(item)!=highlighted.end())
					{
						insideColor[0]=HIGHLIGHTCOLOR;//assignColorToSharedItems(m_pathwayID, type, id, sharedSets);	
						
					}
					else if(j==1)
					{
						insideColor[0]=HIGHLIGHTCOLOR;//assignColorToDifferedItems(m_pathwayID, type, id);							
					}
					else if(j==2)
					{
						level=pbubble->findLinkedLevel(m_pathwayID, type, id);
						//float totalLevel=pbubble->getTotalLinkLevel();						
						//insideColor[0]=QColor(255.0, 255.0, 255.0*level, totalLevel,255);		
						insideColor[0]=HIGHLIGHTCOLOR;
						//insideColor[0]=QColor(assignColorToLinkedItems(level, totalLevel));						
					}
					else if(j==3)
					{
						insideColor[0]=assignColorToExpressedItems(m_pathwayID, type, id, expressedSets,_itemColored,COMPLEXCOLOR);						
					}					
				}
				else
				{
					if( expressedComplex_Edit.find(id)!= expressedComplex_Edit.end())
					{
						 insideColor[0]=assignColorToExpressedItems(m_pathwayID, type, id, expressedSets,_itemColored,COMPLEXCOLOR);					
						 //insideColor.resize(1);						 
					     //insideColor.push_back(assignColorToExpressedItems(m_pathwayID, type, id, expressedSets,_itemColored,COMPLEXCOLOR));			
					}
					else
						insideColor[0]=COMPLEXCOLOR;					
				}				
		   }
		   break;
		case 'S':  
		   if(sharedS==-1 )
		   {
				for(int i=0; i<SmallMolecule.size();i++)
				{
					set<int> record=SmallMolecule[i];
					if(record.find(id)!=record.end())
					{
						sharedS=i;
					}   
				}
				if(sharedS!=-1)
				{	
					if(j==0)//||highlighted.find(item)!=highlighted.end())
					{
						//insideColor[0]=QColor(255,255,100,255);
						insideColor[0]=HIGHLIGHTCOLOR;//assignColorToSharedItems(m_pathwayID, type, id, sharedSets);							
					}						
					else if(j==1)
					{
						//insideColor[0]=QColor(255,255,100,255);
						insideColor[0]=HIGHLIGHTCOLOR;//assignColorToDifferedItems(m_pathwayID, type, id);							
					}					    
					else if(j==2)
					{
						level=pbubble->findLinkedLevel(m_pathwayID, type, id);
						//float totalLevel=pbubble->getTotalLinkLevel();						
						
						insideColor[0] = HIGHLIGHTCOLOR;//QColor(assignColorToLinkedItems(level, totalLevel));
					}					    
					else if(j==3)
					{
                       insideColor[0]=assignColorToExpressedItems(m_pathwayID, type, id, expressedSets,_itemColored,SMALLMOLECULECOLOR );
					}					   
				}	 
				else
				{					
					/*if( expressedSmallMolecule_Edit.find(id)!= expressedSmallMolecule_Edit.end())
					{
						
					    insideColor.push_back(assignColorToExpressedItems(m_pathwayID, type, id, expressedSets,_itemColored,SMALLMOLECULECOLOR ));			
					}
					else*/
					insideColor[0]=SMALLMOLECULECOLOR ;
				}
				//float th=rect.height(),tw=rect.width();
				//float tth=nodePos.height(),ttw=nodePos.width();
				
		  }
		  break;
		case 'D':   
		   if(sharedD==-1 )
		   {
				for(int i=0; i<Dna.size();i++)
				{
					set<int> record=Dna[i];
					if(record.find(id)!=record.end())
					{
						sharedD=i;
					}   
				} 
				if(sharedD!=-1)
				{	
					if(j==0)//||highlighted.find(item)!=highlighted.end())
					{
						insideColor[0]=HIGHLIGHTCOLOR;//QColor(255,255,100,255);  
						
					}
					else if(j==1)
					{
						
						insideColor[0]=HIGHLIGHTCOLOR;//QColor(255,255,100,255);  //painter->setBrush(QColor(190,186,218,255));  
						
					}
					else if(j==2)
					{
						level=pbubble->findLinkedLevel(m_pathwayID, type, id);
						insideColor[0]=QColor(255,255,100,255);  //painter->setBrush(QColor(251,128,114,255)); 
						
					}
					else if(j==3)
					{
						insideColor[0]=QColor(255,255,100,255); 
						
					}
				}	
				else
				{									
					/*if( expressedDna_Edit.find(id)!= expressedDna_Edit.end())
					{
						
					    insideColor.push_back(assignColorToExpressedItems(m_pathwayID, type, id, expressedSets,_itemColored,DNACOLOR));			
					}
					else*/
						insideColor[0]=DNACOLOR;			  				
				}
		   }
		   break; 
		case 'R':   
		   if(sharedR==-1 )
		   {
			   for(int i=0; i<Reaction.size();i++)
			   {
		  		  set<int> record=Reaction[i];
				  if(record.find(id)!=record.end())
				   		sharedR=i;   
			   }   
			   if(reactionType=="D") // dissociation
			   {
					 if(sharedR==-1)
						 insideColor[0]=QColor(255,255,255,255);	   
					 else 
					 {
						if(j==0)//||highlighted.find(item)!=highlighted.end())
						{
							//insideColor[0]=QColor(255,255,100,255);
							insideColor[0]=HIGHLIGHTCOLOR;//assignColorToSharedItems(m_pathwayID, type, id, sharedSets);	
							
						}						
						else if(j==1)
						{
							insideColor[0]=HIGHLIGHTCOLOR;//assignColorToDifferedItems(m_pathwayID, type, id);								
						}					    
						else if(j==2)
						{
							level=pbubble->findLinkedLevel(m_pathwayID, type, id);
							insideColor[0]=HIGHLIGHTCOLOR;//=QColor(assignColorToLinkedItems(level, totalLevel));		
						}					    
						else if(j==3)
						{
							insideColor[0]=assignColorToExpressedItems(m_pathwayID, type, id, expressedSets,_itemColored, QColor(255,255,255,255));
						}	
					 }					
			   }
			   // binding
			   else if(reactionType=="B")
			   {
				 if(sharedR==-1)
					 insideColor[0]=QColor(255,255,255,255);		
				 else
				 {
						if(j==0)//||highlighted.find(item)!=highlighted.end())
						{
							insideColor[0]=HIGHLIGHTCOLOR;//assignColorToSharedItems(m_pathwayID, type, id, sharedSets);								
						}						
						else if(j==1)
						{
							insideColor[0]=HIGHLIGHTCOLOR;//assignColorToDifferedItems(m_pathwayID, type, id);								
						}					    
						else if(j==2)
						{
							level=pbubble->findLinkedLevel(m_pathwayID, type, id);
							insideColor[0] = HIGHLIGHTCOLOR;//QColor(assignColorToLinkedItems(level, totalLevel));						
						}					    
						else if(j==3)
						{
							insideColor[0]=assignColorToExpressedItems(m_pathwayID, type, id, expressedSets,_itemColored, QColor(255,255,255,255));
						}	
				 }		
				
			   }
			   else if(reactionType=="T")
			   {   if(sharedR==-1)
				        insideColor[0]=QColor(255,255,255,255);					 
				   else 
				   {
						if(j==0)//||highlighted.find(item)!=highlighted.end())
						{
							//insideColor[0]=QColor(255,255,100,255);
							insideColor[0]=HIGHLIGHTCOLOR; //assignColorToSharedItems(m_pathwayID, type, id, sharedSets);	
							
						}						
						else if(j==1)
						{
							//insideColor[0]=QColor(255,255,100,255);
							insideColor[0] = HIGHLIGHTCOLOR;//assignColorToDifferedItems(m_pathwayID, type, id);	
							
						}					    
						else if(j==2)
						{
							level=pbubble->findLinkedLevel(m_pathwayID, type, id);
							//float totalLevel=pbubble->getTotalLinkLevel();													
							insideColor[0]=HIGHLIGHTCOLOR;//=QColor(assignColorToLinkedItems(level, totalLevel));	
						}					    
						else if(j==3)
						{
							insideColor[0]=assignColorToExpressedItems(m_pathwayID, type, id, expressedSets,_itemColored,QColor(255,255,255,255));
							
						}	
				  }	 
			   
			   }  
			   
		   }
		   break;
		case 'E':    
		   if(sharedE==-1 )
		   {
			  for(int i=0; i<PhysicalEntity.size();i++)
			  {
				 set<int> record=PhysicalEntity[i];
				 if(record.find(id)!=record.end())
				 {
					 sharedE=i;
				 }   
			  } 
			  if(sharedE!=-1)
			  {	
					if(j==0)//||highlighted.find(item)!=highlighted.end())
					{
						//insideColor[0]=QColor(255,255,100,255);
						insideColor[0]=HIGHLIGHTCOLOR;//assignColorToSharedItems(m_pathwayID, type, id, sharedSets);	
						
					}						
					else if(j==1)
					{
						//insideColor[0]=QColor(255,255,100,255);
						insideColor[0]=HIGHLIGHTCOLOR;//assignColorToDifferedItems(m_pathwayID, type, id);	
						
					}					    
					else if(j==2)
					{
						level=pbubble->findLinkedLevel(m_pathwayID, type, id);
						//float totalLevel=pbubble->getTotalLinkLevel();												
						insideColor[0]=HIGHLIGHTCOLOR;//=QColor(assignColorToLinkedItems(level, totalLevel));												
					}					    
					else if(j==3)
					{
						insideColor[0]=assignColorToExpressedItems(m_pathwayID, type, id, expressedSets, _itemColored,PHYSICALENTITYCOLOR );
						
					}	
			  }	
			  else
			  {
					
									
					if( expressedPhysicalEntity_Edit.find(id)!= expressedPhysicalEntity_Edit.end())
					{
						 //insideColor[0]=PHYSICALENTITYCOLOR ;  
						 //insideColor.resize(1);
					     //insideColor.push_back(assignColorToExpressedItems(m_pathwayID, type, id, expressedSets,_itemColored,PHYSICALENTITYCOLOR ));			
						insideColor[0]= assignColorToExpressedItems(m_pathwayID, type, id, expressedSets,_itemColored,PHYSICALENTITYCOLOR );			
					}
					else
						insideColor[0]=PHYSICALENTITYCOLOR;  
			  }
			  
			 
		   }
		   break;		   
		case 'L':    
		   if(sharedA==-1 )
		   {
			  for(int i=0; i<ANode.size();i++)
			  {
				 set<int> record=ANode[i];
				 if(record.find(id)!=record.end())
				 {
					 sharedA=i;
				 }   
			  } 
			  if(sharedA!=-1)
			  {	
					if(j==0)//||highlighted.find(item)!=highlighted.end())
					{
						//insideColor[0]=QColor(255,255,100,255);
						insideColor[0]=HIGHLIGHTCOLOR;//assignColorToSharedItems(m_pathwayID, type, id, sharedSets);	
						
					}						
					else if(j==1)
					{
						//insideColor[0]=QColor(255,255,100,255);
						insideColor[0]=HIGHLIGHTCOLOR;//assignColorToDifferedItems(m_pathwayID, type, id);	
						
					}					    
					else if(j==2)
					{
						level=pbubble->findLinkedLevel(m_pathwayID, type, id);
						//float totalLevel=pbubble->getTotalLinkLevel();												
						insideColor[0]=HIGHLIGHTCOLOR; //=QColor(assignColorToLinkedItems(level, totalLevel));						
						
					}					    
					else if(j==3)
					{
						insideColor[0]=assignColorToExpressedItems(m_pathwayID, type, id, expressedSets,_itemColored, SETCOLOR);
						
					}	
				}	
			  else
			  {
				    /*if( expressedDna_Edit.find(id)!= expressedDna_Edit.end())
					{
					   insideColor.push_back(assignColorToExpressedItems(m_pathwayID, type, id, expressedSets,_itemColored, SETCOLOR));			
					}
					else */ insideColor[0]=SETCOLOR;  
			  }
		   }
		   break;
		   }
   }
   if(insideColor.size()==1)
   {
	    if(drawRateLimit && type=='P' && rateLimit.find(id)!=rateLimit.end())
		{
			insideColor.push_back(RATELIMITCOLOR);
		}
		if(drawCrossTalk && type=='P' && crossTalk.find(id)!=crossTalk.end())
		{
			insideColor.push_back(CROSSTALKCOLOR);	
		}	
		/*if(drawOrthology && type=='P' && orthology.find(id)==orthology.end())
		{
			//insideColor.push_back(QColor(180, 255, 180, 255));	//
			if(insideColor.size()>=1)
				insideColor[0]=QColor(255,255,204, 0);
		}*/
		if(drawOrthology && type=='P' && orthology.find(id)!=orthology.end())
		{
			//insideColor.push_back(QColor(180, 255, 180, 255));	//
			//if(insideColor.size()>=1)
			//	insideColor[0]=QColor(255,255,204, 0);
			insideColor.push_back(ORTHOLOGYCOLOR);	
		}
   }
    ////***
    fontColor=QColor(0,0,0,255);  
  	////***
		
	boundaryColor=c;
	boundaryWidth=1;
	if(flag || (!hoveredItem.empty() && hoveredItem[0]==m_pathwayID && hoveredItem[1]==item[0] && hoveredItem[2]==item[1]) )
	{
		boundaryWidth=3; 		
	}	
	else if(highlighted.find(item)!=highlighted.end())
	{
		//if(level==0)
		{
			c= MIDIUM_HIGHLIGHTCOLOR;
			boundaryColor=c;
		}
		boundaryWidth=3;		
	}
	else if(searched.find(item)!=searched.end())
	{
		c= MIDIUM_HIGHLIGHTCOLOR;	
		boundaryWidth=3;
		boundaryColor=c;
	}
    else if(level==0)
	{
			c= MIDIUM_HIGHLIGHTCOLOR;
			boundaryColor=c;
			boundaryWidth=3;		
	}
	else
	{
		boundaryWidth=1;	
		boundaryColor=c;
	}

   w=w*width;   
   if(!flag) 
    { 
		/*bool flag=0;
		vector<int> temp;	 temp.push_back(m_pathwayID);  temp.push_back(type);    temp.push_back(id);   temp.push_back(GO_UP);
		if(_itemColored.find(temp)!=_itemColored.end())
		{
			fontColor=QColor(231,78,120);			
			flag=1;
		}
		else
		{
			temp[3]=_DOWN;
		    if(_itemColored.find(temp)!=_itemColored.end())
		    {
				fontColor=QColor(64,64,255);				
				flag=1;
			}
			else 
			{
			    temp[3]=_EVEN;
				if(_itemColored.find(temp)!=_itemColored.end())
		        {
					//painter->setPen(QColor(0,0,0,255));			
					flag=1;
				}				
				fontColor=QColor(0,0,0,255);				
			}
		}	*/		
		fontColor=QColor(0,0,0,255); //keqin: do not color font according to up and down
		getNodeFont(fontSize, fontRect, rect, nodeName, type, width, height, fixedSize, scale);				
   }   
   return rect;
}


void ItemBase::getNodeFont(int &fontSize, QRectF &fontRect, QRectF nodeRect, QString nodeName, int type, int width, int height, bool fixedSize, float scale, bool showOthers)
{
	    fontSize=(width+height)/8;
		if(fontSize<10) fontSize=10;		
		else if(fontSize>24) 
			fontSize=24;
		        		
		if(showOthers || (type!='R' && type!='C'))
		{
			QString testName=nodeName;
			fontRect=nodeRect;
			 if(nodeName.size()<8)
			 {    
                if(type=='P')
				{
					testName="protein1";
				}
				else  if(type=='L')
				{
				    if(nodeName.size()<8)
				        testName="A_ABC...";
				}
				else 
				{
				    if(nodeName.size()<5)
				        testName="Dna12";
				}
			 }
			 else if(testName.size()>18)
			 {
				 for(int i=0;i<2;i++)
			     {
					int tindex0 = testName.lastIndexOf(" ");	
					if(tindex0 <0)
					   tindex0 = testName.lastIndexOf("/");	
					if(tindex0>9)				
					   testName.remove(tindex0,testName.size());
					if(testName.size()<18)
						break;
				 }
				 if(testName.size()>18)
					 testName.remove(10,testName.size());
			}

			 do
			 {
				fontSize=fontSize-1;				
				QFont f("Arial",fontSize);					
				QFontMetrics fontMetrics(f);
				fontRect = fontMetrics.boundingRect(0, 0, nodeRect.width(), 0, Qt::TextExpandTabs|Qt::TextWrapAnywhere, testName); 
			}while(fontRect.height()>nodeRect.height() && fontSize>4);
			
			 if(fontRect.height()>nodeRect.height())
				fontRect=nodeRect;	

		/*	 if(fontSize>24) 
			fontSize=24;

		if(fontSize>23) 
			fontSize=23;

		if(fontSize>22) 
			fontSize=22;*/
			 fontRect = QRect(nodeRect.center().x()-fontRect.width()/2, nodeRect.center().y()-fontRect.height()/2, fontRect.width(), fontRect.height());
		}
}

void ItemBase::paintHoveredText( QPainter *painter, QRectF nodePos, QString nodeName, int type, int id, QPointF dCenter, float scale, float L, float B, float W, float H,  bool flag,  QColor c)
{	
	//&& ( item[0]=='R' || item[0]=='C' || maxFontSize<8)
	if(type=='M')
		return;
	int width, height;
	if(fixedSize)
	{
		width=graphReferenceSize*scale, height=graphReferenceSize*scale;
	}
	else
	{
		width=this->realRect().width()*scale;
		height=this->realRect().height()*scale;
	}
	//get a protein for reference
	PathBubble1* pbubble = dynamic_cast<PathBubble1 *> (this);				
	QRectF rect;
	if(type=='C' || type=='R')
	{
		rect=getNodeRect(nodePos, nodeName, type, fixedSize,  dCenter, scale);	
		type='P'; 				
		rect=QRectF(rect.center().x()-rect.width()/2.0,rect.center().y()-referH/2.0,rect.width(), referH);
	}
	else 
	{
	   rect=getNodeRect(nodePos, nodeName, type, fixedSize,  dCenter, scale);	
	   if(type=='L')
	   {
		   if(rect.height()>40)
			   rect.setHeight(40);
	      referH=rect.height();
	   }
	}
		
	int fontSize1, fontSize2, fontSize;
	QRectF fontRect1,fontRect2, fontRect,rect1;
	rect1.setWidth(1000);	

	getNodeFont(fontSize1, fontRect1, rect1, nodeName, type, width, height, fixedSize, scale);
	getNodeFont(fontSize2, fontRect2, rect1, "protein123", 'P', width, height, fixedSize, scale, true);		
	fontSize=fontSize2;
	if(fontSize<fontSize1)
		fontSize=fontSize1;
	if(fontSize<10)
		fontSize=10;
	if(fontSize>18)
		fontSize=18;
	
	painter->setBrush(QColor(255,255,255,255));
	if(nodeName.size()!=0)
	{
		QFont f1("Arial",fontSize);	
		painter->setFont(f1);	
		QFontMetrics fontMetrics(f1);
		fontRect = fontMetrics.boundingRect(nodeName); 	
		
		int fontRectHeight=fontRect.height();

		if(fontRectHeight < referH*0.8)
			fontRectHeight=referH*0.8;
		rect1=QRectF(rect.center().x(), rect.center().y(), fontRect.width()+2, fontRectHeight);			
		
		painter->setPen(Qt::NoPen);		
		painter->drawRect(rect1);
		
		painter->setPen(QColor(0,0,0,255));		
		painter->drawText( rect1, Qt::AlignCenter, nodeName);//_complexName[id]);		  	    			
	}
}

/*
void ItemBase::paintHoveredText( QPainter *painter, QRectF nodePos, QString nodeName, int type, int id, QPointF dCenter, float scale, float L, float B, float W, float H,  bool flag,  QColor c)
{	
	
	QRectF rect=getNodeRect(nodePos, nodeName, type, id, fixedSize,  dCenter, scale);
	//and rect
	//QRectF nodePos, QString nodeName
	int w2,h2;
	//bool oflag=false; //make if the shared node are the origin
	int sharedP,sharedE,sharedS,sharedD,sharedR,sharedC;
	int width=this->realRect().width()*scale;
	int height=this->realRect().height()*scale;

	if(fixedSize)
	{
		width=500*scale;
	    height=500*scale;
	}


	float h=nodePos.height(), w=nodePos.width();	
    float cx=nodePos.x(), cy=nodePos.y();
	float edgescale;
	cx=(cx-L)/W; cy=(cy-B)/H; 
    w=w/W, h=h/H;

	h=h*height, w=w*width;	
    cx=cx*width-width/2, cy=cy*height-height/2;
	
	
	if(h<0.04*500.0/3.0)
    {
        h=0.04*500.0/3.0; 
    }


	int fontsize=4*(this->realRect().width()+this->realRect().height())/500.0; //if(fontsize<6) fontsize=6; else if(fontsize>18) fontsize=18; 
	
	int dcx=dCenter.x()*this->realRect().width(), dcy=dCenter.y()*this->realRect().height();
	//int dcx=dCenter.x()*width, dcy=dCenter.y()*height;

    QRect rect = QRect(cx+ dcx, cy+ dcy, w, h);
	 QRect rectBackup = rect;
 	 	fontsize=fontsize+1;	
		QRect rect2;
	    do
        {
			 fontsize=fontsize-1;	
			 //QFont f("Arial",fontsize);		
		     //doc.setDefaultFont(f);
		     //doc.setTextWidth( rect.width() );
		     //size = doc.size();	
		     QFont f("Arial",fontsize);		        
		     painter->setFont(f);
		     int width = rect.width();
		     QFontMetrics fontMetrics(f);
             rect2 = fontMetrics.boundingRect(0, 0, rect.width(), 0, Qt::TextExpandTabs|Qt::TextWrapAnywhere, nodeName); 		
	    }while(rect2.height()>rect.height() && fontsize>2);

	    //h=h+4;
	    //QRect rect = QRect(cx+ dCenter.x(), cy+ dCenter.y(), w, h);
		//if(fontsize<8)
        {
		   //if(hoveredItem[0]==m_pathwayID && hoveredItem[1]==item[0] && hoveredItem[2]==item[1])
		   //if(hoveredItem.find(item)!=highlighted.end())//||foundlist.find(item)!=foundlist.end())
	       {
			   int fontsize=8;
			   QFont f1("Arial",fontsize);	
			   painter->setFont(f1);	
			   //QSizeF size=getItemSize_2(type, id, nodeName);
			   QSizeF size=getFontSize( nodeName, fontsize);
			   //rect.moveCenter(QPoint(rect.center().x()-(size.width()*scale/2.0-rect.width()/2.0),rect.center().y()-(size.height()*scale/2.0-rect.height()/2.0) ));
			   
			   rect.moveCenter(QPoint(rect.center().x()+rect.width()/2.0,rect.center().y()+rect.height()/2.0));
			   rect.setWidth(size.width()*scale);
			   rect.setHeight(size.height()*scale);
			   
			   QFontMetrics fontMetrics(f1);	
			   QRect rect2 = fontMetrics.boundingRect( nodeName);			   

			  
			   
			   if(rect2.width()<rectBackup.width())
				   rect2.setWidth(rectBackup.width());

			   rect2.setWidth(rect2.width()*1.1);
			   rect2.setHeight(rect2.height()*1.1);

			   
			   
			   //rect.moveCenter(QPoint(rect.center().x()+rect.width()/2.0, rect.center().y()+rect.height()/2.0));
			   //rect.setWidth(size.width()*scale);
			   //rect.setHeight(size.height()*scale);
			   rect2.moveCenter(rect.center());

			   
			   painter->setBrush(QColor(255,255,255,255));
			   if(nodeName.size()!=0)
			   {
				   painter->setPen(Qt::NoPen);
				   painter->drawRect(rect2);    
			   }
			   painter->setPen(QColor(0,0,0,255));
			   //painter->drawText(QRect(rect.x()+3, rect.y()+3, rect.width()-6, rect.height()-6), Qt::AlignCenter|Qt::TextWrapAnywhere, nodeName);//_complexName[id]);		  

			   painter->drawText(QRect(rect2.x(), rect2.y(), rect2.width(), rect2.height()), Qt::AlignCenter|Qt::TextWrapAnywhere, nodeName);//_complexName[id]);		  
	       }
		}	   
 
}
*/

void ItemBase::paintProtein( QPainter *painter, vector<QRectF> proteinPos, vector<vector<QString>> _proteinName, int id, QPointF dCenter, float scale, bool flag,  QColor c)
{
	//pathwaySelected=-1;
	int width=this->realRect().width()*scale;
	int height=this->realRect().height()*scale;
	int h=proteinPos[id].height()*height, w=proteinPos[id].width()*width;	
    int cx=proteinPos[id].x()*width-width/2, cy=proteinPos[id].y()*height-height/2;

	if(h<0.04*graphReferenceSize/3.0)
   {
      w=0.04*graphReferenceSize/3.0*w/h; h=0.04*graphReferenceSize/3.0; 
   }

	//h=h+4;

	QRect rect = QRect(cx+ dCenter.x(), cy+ dCenter.y(), w, h);
	
	vector<int> item; 
	item.push_back('P'); item.push_back(id);
	if(flag || highlighted.find(item)!=highlighted.end()||foundlist.find(item)!=foundlist.end())
	{
		if(!flag)
		    c=QColor(158,158,188,255);
		QPen pen(c);
		pen.setWidthF(4);
		painter->setPen(pen);	
		/*QSizeF size=getItemSize('P', id, _proteinName[id][0]);
		rect.setWidth(size.width()*scale+6);
		rect.setHeight(size.height()*scale+6);
		rect.setX(rect.x()-3);
		rect.setY(rect.y()-3);*/
	}
    else 
	{
		QPen pen(QColor(102,102,102,255));
		pen.setWidthF(1);
		painter->setPen(pen);	
	}		
	   
   if(flag)
	{
		painter->setBrush(Qt::NoBrush);
		 painter->drawRoundedRect( rect, 6, 6 );
		//return;
   }
   int shared=-1;
   for(int i=0; i<sharedProtein.size();i++)
   {
       set<int> record=sharedProtein[i];
	   if(record.find(id)!=record.end())
	   {
		  shared=i;
	   }   
   }
   if(shared!=-1)
      painter->setBrush(QColor(255,255,255,255));  
   else
	   painter->setBrush(QColor(199,253,205,255));  

   painter->drawRoundedRect( rect, 6, 6 );
       
   painter->setPen(QColor(0,0,0,255));

   /*QTextDocument doc;  
    QString html= "<body> <p align=center>";
	html += _proteinName[id][0];
	html.append("</body>");		
	doc.setHtml(html);*/

   int fontsize=6*width/graphReferenceSize; if(fontsize<6) fontsize=6; else if(fontsize>10) fontsize=10; 
   QFont f("Arial",fontsize);

   /*doc.setDefaultFont(f);
   doc.setTextWidth( rect.width() );
   painter->save();
   painter->translate( rect.topLeft() );
   doc.setDefaultStyleSheet("a {text-decoration: none;text-align:center;}");*/  
   
   painter->save();
   if(!flag) 
    { 
		//doc.drawContents( painter, rect.translated( -rect.topLeft() ) );
		bool flag=0;
		vector<int> temp;	 temp.push_back(m_pathwayID);  temp.push_back('P');    temp.push_back(id);   temp.push_back(GO_UP);
		if(_itemColored.find(temp)!=_itemColored.end())
		{
			painter->setPen(QColor(255,0,0));	
			flag=1;
		}
		else
		{
			temp[2]=_DOWN;
		    if(_itemColored.find(temp)!=_itemColored.end())
		    {
				painter->setPen(QColor(0,255,0));
				flag=1;
			}
			else 
			{
			    temp[2]=_EVEN;
				if(_itemColored.find(temp)!=_itemColored.end())
		        {
					painter->setPen(QColor(0,0,0,255));			
					flag=1;
				}
				else painter->setPen(QColor(0,0,0,255));
			}
		}		
	
		painter->setFont(f);	
				
		painter->drawText(QRect(rect.x()+3, rect.y()+3, rect.width()-6, rect.height()-6), Qt::TextWrapAnywhere, _proteinName[id][0]);//_complexName[id]);
		if(highlighted.find(item)!=highlighted.end())//||foundlist.find(item)!=foundlist.end())
	    {
		   QSizeF size=getItemSize_2('P', id, _proteinName[id][0]);
		   rect.moveCenter(QPoint(rect.center().x()-(size.width()*scale/2.0-rect.width()/2.0),rect.center().y()-(size.height()*scale/2.0-rect.height()/2.0) ));
		   rect.setWidth(size.width()*scale);
		   rect.setHeight(size.height()*scale);
		   painter->setPen(Qt::NoPen);
		   painter->setBrush(QColor(255,255,255,255));
		   painter->drawRect(rect);       
		   painter->setPen(QColor(0,0,0,255));
		   if(_proteinName[id][0]=="XPO1")
			   id=id;
		   painter->drawText(QRect(rect.x()+3, rect.y()+3, rect.width()-6, rect.height()-6), Qt::TextWrapAnywhere, _proteinName[id][0]);//_complexName[id]);
		  
	    }
		
		//doc.setDefaultTextOption(Qt:WordWrap);
		//painter->drawText(rect.translated( -rect.topLeft() ), _proteinName[id][0]);
   }
   painter->restore();
}


QRectF ItemBase::paintProtein( QPainter *painter, vector<QRectF> proteinPos, vector<vector<QString>> _proteinName, int id , float L, float B, float W, float H, QPointF dCenter, bool flag, QColor c)
{
	//pathwaySelected=-1;
	float width=this->realRect().width();
	float height=this->realRect().height();
	float h=proteinPos[id].height(), w=proteinPos[id].width();	
    float cx=proteinPos[id].x(), cy=proteinPos[id].y();
	
	cx=(cx-L)/W; cy=(cy-B)/H; 
   w=w/W, h=h/H;

	h=h*height, w=w*width;	

	if(h<0.04*graphReferenceSize/3.0)
   {
      w=0.04*graphReferenceSize/3.0*w/h; h=0.04*graphReferenceSize/3.0; 
   }

    cx=cx*width-width/2, cy=cy*height-height/2;
	//h=h+4;

	QRect rect = QRect(cx + dCenter.x(), cy + dCenter.y(), w, h);
	
	vector<int> item; 
	item.push_back('P'); item.push_back(id);
	if(flag || highlighted.find(item)!=highlighted.end())
	{
		if(!flag)
		    c=QColor(158,158,188,255);
		QPen pen(c);
		pen.setWidthF(4);
		painter->setPen(pen);	
	}
    else 
	{
		QPen pen(QColor(102,102,102,255));
		pen.setWidthF(1);
		painter->setPen(pen);	
	}	
		
	if(flag)
    {
		 painter->setBrush(Qt::NoBrush);
		  painter->drawRoundedRect( rect, 6, 6 );		 
	}

   painter->setBrush(QColor(199,253,205,255));  
   painter->drawRoundedRect( rect, 6, 6 );
   
   painter->setPen(QColor(0,0,0,255));

  /* QTextDocument doc;
  
    QString html= "<body> <p align=center>";
	html += _proteinName[id][0];
	html.append("</body>");	
	
	doc.setHtml(html);
	*/

   //QFont f("Arial",4*width/500.0/W);
   int fontsize=6*width/graphReferenceSize/W; if(fontsize<6) fontsize=6; else if(fontsize>10) fontsize=10; 
   QFont f("Arial",fontsize);

   /*doc.setDefaultFont(f);
   doc.setTextWidth( rect.width() );
   painter->save();
   painter->translate( rect.topLeft() );
   doc.setDefaultStyleSheet("a {text-decoration: none;text-align:center;}");*/

   painter->save();
   if(!flag)
   {
      painter->setFont(f);	 	  
      painter->drawText(QRect(rect.x()+3, rect.y()+3,rect.width()-6, rect.height()-6), Qt::AlignCenter|Qt::TextWrapAnywhere, _proteinName[id][0]);//_complexName[id]);
   }
   //doc.drawContents( painter, rect.translated( -rect.topLeft() ) );
   painter->restore();
   return rect;
}



QSizeF ItemBase::getFontSize(QString qstr, int fontsize)
{
	QSizeF size;
	QTextDocument doc;
		doc.setHtml(qstr);
		QFont f("Arial", fontsize);
		doc.setDefaultFont(f);		
		size = doc.size();		
		
	return size;
}


QColor ItemBase::getEdgeColorByPathway(int id)
{
	//color brewer quanlitative darl color
	QColor color;
	switch(id)
    {
		case 1:	color=QColor(27,158,119,250); break;
		case 2: color=QColor(217,95,2,250); break;
		case 3: color=QColor(117,112,179,250); break;
		case 4: color=QColor(231,41,138,250); break;
		case 5: color=QColor(102,166,30,250); break;
		case 6: color=QColor(230,171,2,250); break;
		case 7: color=QColor(166,118,29,250); break;
		case 8: color=QColor(102,102,102,250); break;
	}
	return color;
}

void ItemBase::drawDissociation(QPainter *painter, QRectF rect)
{
	QVector<QPoint> points;	         
	/*points.append(QPoint(rect.x()-1, rect.y()));
	points.append(QPoint(rect.x()+rect.width()+1, rect.y()));
	points.append(QPoint(rect.center().x(), rect.y()+rect.height()));*/

	float w=rect.width(), h=rect.height();
	float cx=rect.center().x(), cy=rect.center().y();
	
	points.append(QPoint(cx-w*0.6,cy+h*0.6));
	points.append(QPoint(cx+w*0.6,cy+h*0.6));
	points.append(QPoint(cx,cy-h*0.6));
	
	/*float w=rect.width(), h=rect.height();
	float cx=rect.center().x(), cy=rect.center().y();
	points.append(QPoint(cx-w*0.65,cy));
	points.append(QPoint(cx,cy-h*0.65));
	points.append(QPoint(cx+w*0.65,cy));
	points.append(QPoint(cx,cy+h*0.65));*/

	QPolygon whole(points);
	painter->drawPolygon(whole);
}

void ItemBase::drawArrow_2(QPainter *painter, QPointF start, QPointF end, float width, float height, QColor color2)
{//single edge
	painter->save(); 
    float size=(1+4*(width+height)/graphReferenceSize)*0.5;	
	painter->setPen(QPen(color2, size, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine(start,end);
	painter->restore(); 
}

void ItemBase::drawATrapezoid(QPainter *painter, QPointF start, QPointF end, float size1, float size2, QColor color)
{
	if(size1<1)	size1=1;
	if(size2<1)	size2=1;

	painter->setPen(color);
	painter->setBrush(color);

	static const double Pi = 3.14159265358979323846264338327950288419717; 
    static double TwoPi = 2.0 * Pi;

    QLineF line(start, end);
    double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0)
		angle = TwoPi - angle;

	QPointF sourceArrowP1 = start + QPointF(sin(angle - Pi / 2.5) * size1,
													  cos(angle - Pi / 2.5) * size1);                                                  
	QPointF sourceArrowP2 = start + QPointF(sin(angle - Pi + Pi / 2.5) * size1,
													cos(angle - Pi + Pi / 2.5) * size1);   
	QPointF dis = (sourceArrowP1 + sourceArrowP2)/2;
	dis=start-dis;
	sourceArrowP1 = sourceArrowP1 + dis;
	sourceArrowP2 = sourceArrowP2 + dis;
				
	QPointF destArrowP1 = end + QPointF(sin(angle - Pi / 2.6) * size2, cos(angle - Pi / 2.6) * size2);
	QPointF destArrowP2 = end + QPointF(sin(angle - Pi + Pi / 2.6) * size2, cos(angle - Pi + Pi / 2.6) * size2);

	dis = (destArrowP1 + destArrowP2)/2;
	dis=end-dis;
	destArrowP1 = destArrowP1 + dis;
	destArrowP2 = destArrowP2 + dis;
	
	painter->drawPolygon(QPolygonF() << destArrowP1 << sourceArrowP1 << sourceArrowP2 << destArrowP2);
    //painter->restore(); 

}

void ItemBase::drawATrapezoid_GL(QPainter *painter, QPointF start, QPointF end, float size1, float size2)
{
	painter->save(); 
	static const double Pi = 3.14159265358979323846264338327950288419717; 
    static double TwoPi = 2.0 * Pi;

	if(size1<1)	size1=1;
	if(size2<1)	size2=1;

    QLineF line(start, end);
    double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0)
		angle = TwoPi - angle;

	QPointF sourceArrowP1 = start + QPointF(sin(angle - Pi / 2.5) * size1,
													  cos(angle - Pi / 2.5) * size1);                                                  
	QPointF sourceArrowP2 = start + QPointF(sin(angle - Pi + Pi / 2.5) * size1,
													cos(angle - Pi + Pi / 2.5) * size1);   
	QPointF dis = (sourceArrowP1 + sourceArrowP2)/2;
	dis=start-dis;
	sourceArrowP1 = sourceArrowP1 + dis;
	sourceArrowP2 = sourceArrowP2 + dis;
				
	QPointF destArrowP1 = end + QPointF(sin(angle - Pi / 2.6) * size2, cos(angle - Pi / 2.6) * size2);
	QPointF destArrowP2 = end + QPointF(sin(angle - Pi + Pi / 2.6) * size2, cos(angle - Pi + Pi / 2.6) * size2);

	dis = (destArrowP1 + destArrowP2)/2;
	dis=end-dis;
	destArrowP1 = destArrowP1 + dis;
	destArrowP2 = destArrowP2 + dis;
	
	glVertex2f(destArrowP1.x(),destArrowP1.y());
	glVertex2f(sourceArrowP1.x(),sourceArrowP1.y());
	glVertex2f(sourceArrowP2.x(),sourceArrowP2.y());
	glVertex2f(destArrowP2.x(),destArrowP2.y());
	//painter->drawPolygon(QPolygonF() << destArrowP1 << sourceArrowP1 << sourceArrowP2 << destArrowP2);

    painter->restore(); 

}

/*void ItemBase::drawATrapezoid(QPainter *painter, QPointF start, QPointF end, float size1, float size2, QColor color)
{
	//painter->save(); 
	painter->setPen(color);
	painter->setBrush(color);

	static const double Pi = 3.14159265358979323846264338327950288419717; 
    static double TwoPi = 2.0 * Pi;

    QLineF line(start, end);
    double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0)
		angle = TwoPi - angle;

	QPointF sourceArrowP1 = start + QPointF(sin(angle - Pi / 2.5) * size1,
													  cos(angle - Pi / 2.5) * size1);                                                  
	QPointF sourceArrowP2 = start + QPointF(sin(angle - Pi + Pi / 2.5) * size1,
													cos(angle - Pi + Pi / 2.5) * size1);   
	QPointF dis = (sourceArrowP1 + sourceArrowP2)/2;
	dis=start-dis;
	sourceArrowP1 = sourceArrowP1 + dis;
	sourceArrowP2 = sourceArrowP2 + dis;
				
	QPointF destArrowP1 = end + QPointF(sin(angle - Pi / 2.6) * size2, cos(angle - Pi / 2.6) * size2);
	QPointF destArrowP2 = end + QPointF(sin(angle - Pi + Pi / 2.6) * size2, cos(angle - Pi + Pi / 2.6) * size2);

	dis = (destArrowP1 + destArrowP2)/2;
	dis=end-dis;
	destArrowP1 = destArrowP1 + dis;
	destArrowP2 = destArrowP2 + dis;
	
	painter->drawPolygon(QPolygonF() << destArrowP1 << sourceArrowP1 << sourceArrowP2 << destArrowP2);
    painter->restore(); 
}　*/

QVector<QPointF> ItemBase::getArrow_4(QPointF start, QPointF end, float width, float height, float _scale)
{//doted line
	QPointF temp;
	QVector<QPointF>  points;
	temp = start; start =end; end =temp;
	
	QLineF line(start, end);	

	float ds = line.length(); //sqrt(line.dx()*line.dx() + line.dy()*line.dy());  
	
    //if (qFuzzyCompare(line.length(), qreal(0.)))
   //     return;

	float size=1+4*(width+height)/graphReferenceSize*_scale;	
	//qreal arrowSize(size);

	//static const double Pi = 3.14159265358979323846264338327950288419717; 
    //static double TwoPi = 2.0 * Pi;
	
	float itv;
	float dotSize=size*2.6;
    int count=ds/dotSize;
	if(count%2!=1)
		count=count-1;
	
	if(count<3)
		count=3;
	
	count=count*2;
	itv=ds/count;
	
	//bool flag=true;
	//if(size/ds<0.2)
	 //  flag=false;

	float l1,l2, as1, as2, scale;
	QPointF source, dest;
	for(int i=0; i<count; i=i+2)
	{
		l1=i*itv;
		scale=l1/ds;
		as1=size*scale;
		source= end + (start-end)*scale;

		l2=(i+1)*itv;
		scale=l2/ds;
		as2=size*scale;
		dest= end + (start - end)*scale;
		//drawATrapezoid(painter, source, dest, as1, as2, color);
		getATrapezoid(source, dest, as1, as2, points);
		//drawATrapezoid(painter, source, dest, as1, as2, color);
		/*if(flag)
		//if(as2!=0 && as1/as2>1.5 || as1/as2<0.67)
		{
			//dest= end + (start - end)*scale;
		    drawATrapezoid(painter, source, dest, as1, as2, color);
		}
		else
		{ 
			//painter->setPen(QPen(color, (as1+as2)/2)); //; Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
			//painter->drawLine(source, dest);
		}*/		
	}	
	//painter->restore(); 
	return points;
}

QVector<QPointF> ItemBase::getArrow_4(QPointF start, float h, QPointF end, float width, float height, float _scale)
{//doted line
	QVector<QPointF> QaudPoints;
	QLineF line(start, end);
	qreal arrowSize(h);
	float scale=0.5;
	QPointF start1;
	start1= (end-start)*scale + start;
	
	static const double Pi = 3.14159265358979323846264338327950288419717; 
    static double TwoPi = 2.0 * Pi;

    double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0)
		angle = TwoPi - angle;

    QPointF sourceArrowP1 = start1 + QPointF(sin(angle - Pi / 2.5) * arrowSize,
                                                  cos(angle - Pi / 2.5) * arrowSize);                                                  
    QPointF sourceArrowP2 = start1 + QPointF(sin(angle - Pi + Pi / 2.5) * arrowSize,
                                                  cos(angle - Pi + Pi / 2.5) * arrowSize);  

	QPointF dis1 = (sourceArrowP1 + sourceArrowP2)/2;	
    dis1=start1-dis1;
	sourceArrowP1 = sourceArrowP1 + dis1;
	sourceArrowP2 = sourceArrowP2 + dis1;

	float size;//=(1+4*(width+height)/graphReferenceSize)*0.5*3.1;	
	
	vector<QPointF> points = getCurveLine(start, sourceArrowP1, end);

	//get curve length
	float ds=0;
	for(vector<QPointF>::iterator it = points.begin(); it != points.end(); it++)
	{
		vector<QPointF>::iterator it1=it;
		   it1++;
		if(it1==points.end())
	       break;
		QPointF p1=*it, p2=*it1;
		p1=p2-p1;
		ds+=sqrt(pow(p1.x(),2) + pow(p1.y(),2));
	}
	
	size=1+4*(width+height)/graphReferenceSize;	
	//qreal arrowSize2(size);

    float itv;
	float dotSize=size*2.6;
    int count=ds/dotSize;
	if(count%2!=1)
		count=count-1;

	count=count*2;
	itv=ds/count*_scale;

	
	float l1,l2, as1, as2, Dx=0;
	QPointF source, dest;	
	vector<QPointF>::iterator sIt=points.begin(); 
	for(int i=0; i<count; i=i+2)
	{
		l1=i*itv;
		scale=l1/ds;
		as1=size*scale;
		sIt=points.begin(); 
		source=Interpolation(points, sIt, l1);		

		l2=(i+1)*itv;
		scale=l2/ds;
		as2=size*scale;

		sIt=points.begin(); 
		dest=Interpolation(points, sIt, l2);
		getATrapezoid(source, dest, as1, as2, QaudPoints);
		
	}	
	return QaudPoints;	
}




QVector<QPointF> ItemBase::getArrow_4_H(QPointF start, float h, QPointF end, float width, float height, float _scale)
{//doted line
	QVector<QPointF> QaudPoints;
	QLineF line(start, end);
	qreal arrowSize(h);
	float scale=0.5;
	QPointF start1;
	start1= (end-start)*scale + start;
	
	static const double Pi = 3.14159265358979323846264338327950288419717; 
    static double TwoPi = 2.0 * Pi;

    double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0)
		angle = TwoPi - angle;

    QPointF sourceArrowP1 = start1 + QPointF(sin(angle - Pi / 2.5) * arrowSize,
                                                  cos(angle - Pi / 2.5) * arrowSize);                                                  
    QPointF sourceArrowP2 = start1 + QPointF(sin(angle - Pi + Pi / 2.5) * arrowSize,
                                                  cos(angle - Pi + Pi / 2.5) * arrowSize);  

	QPointF dis1 = (sourceArrowP1 + sourceArrowP2)/2;	
    dis1=start1-dis1;
	sourceArrowP1 = sourceArrowP1 + dis1;
	sourceArrowP2 = sourceArrowP2 + dis1;

	float size;//=(1+4*(width+height)/graphReferenceSize)*0.5*3.1;	
	
	vector<QPointF> points = getCurveLine(start, sourceArrowP1, end);

	//get curve length
	float ds=0;
	for(vector<QPointF>::iterator it = points.begin(); it != points.end(); it++)
	{
		vector<QPointF>::iterator it1=it;
		   it1++;
		if(it1==points.end())
	       break;
		QPointF p1=*it, p2=*it1;
		p1=p2-p1;
		ds+=sqrt(pow(p1.x(),2) + pow(p1.y(),2));
	}
		
    
	size=1+4*(width+height)/graphReferenceSize;	
	//qreal arrowSize2(size);

    float itv;
	float dotSize=size*2.6;
    int count=ds/dotSize;
	if(count%2!=1)
		count=count-1;

	count=count*2;
	itv=ds/count;

	//bool flag=true;
	//if(size/ds<0.2)
	//   flag=false;
	float exWidth=HBWIDTH*_scale*2;
	float l1,l2, as1, as2, Dx=0;
	QPointF source, dest;	
	vector<QPointF>::iterator sIt=points.begin(); 
	for(int i=0; i<count; i=i+2)
	{
		l1=i*itv;
		scale=l1/ds;
		as1=size*scale + exWidth;
		sIt=points.begin(); 
		source=Interpolation(points, sIt, l1);		

		l2=(i+1)*itv;
		scale=l2/ds;
		as2=size*scale + exWidth;

		sIt=points.begin(); 
		dest=Interpolation(points, sIt, l2);
		getATrapezoid(source, dest, as1, as2, QaudPoints);

		
	}	
	return QaudPoints;	
}


void ItemBase::getATrapezoid(QPointF start, QPointF end, float size1, float size2, QVector<QPointF> &points)
{
	//painter->save(); 
	if(size1<1)	size1=1;
	if(size2<1)	size2=1;

	static const double Pi = 3.14159265358979323846264338327950288419717; 
    static double TwoPi = 2.0 * Pi;

    QLineF line(start, end);
    double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0)
		angle = TwoPi - angle;

	QPointF sourceArrowP1 = start + QPointF(sin(angle - Pi / 2.5) * size1,
													  cos(angle - Pi / 2.5) * size1);                                                  
	QPointF sourceArrowP2 = start + QPointF(sin(angle - Pi + Pi / 2.5) * size1,
													cos(angle - Pi + Pi / 2.5) * size1);   
	QPointF dis = (sourceArrowP1 + sourceArrowP2)/2;
	dis=start-dis;
	sourceArrowP1 = sourceArrowP1 + dis;
	sourceArrowP2 = sourceArrowP2 + dis;
				
	QPointF destArrowP1 = end + QPointF(sin(angle - Pi / 2.6) * size2, cos(angle - Pi / 2.6) * size2);
	QPointF destArrowP2 = end + QPointF(sin(angle - Pi + Pi / 2.6) * size2, cos(angle - Pi + Pi / 2.6) * size2);

	dis = (destArrowP1 + destArrowP2)/2;
	dis=end-dis;
	destArrowP1 = destArrowP1 + dis;
	destArrowP2 = destArrowP2 + dis;
	
	points.push_back(destArrowP1);
	points.push_back(sourceArrowP1);
	points.push_back(sourceArrowP2);
	points.push_back(destArrowP2);
	//painter->drawPolygon(QPolygonF() << destArrowP1 << sourceArrowP1 << sourceArrowP2 << destArrowP2);
    //painter->restore(); 
}

void ItemBase::drawArrow_4(QPainter *painter, QPointF start, QPointF end, float width, float height, QColor color)
{//doted line
	QPointF temp;
	temp = start; start =end; end =temp;
	
	QLineF line(start, end);	

	float ds = line.length(); //sqrt(line.dx()*line.dx() + line.dy()*line.dy());  
	
    painter->save(); 

	float size=1+4*(width+height)/graphReferenceSize;	
	
	float itv;
	float dotSize=size*2.6;
    int count=ds/dotSize;
	if(count%2!=1)
		count=count-1;
	
	if(count<3)
		count=3;
	
	count=count*2;
	itv=ds/count;
	
	bool flag=true;
	if(size/ds<0.2)
	   flag=false;

	float l1,l2, as1, as2, scale;
	QPointF source, dest;
	for(int i=0; i<count; i=i+2)
	{
		l1=i*itv;
		scale=l1/ds;
		as1=size*scale;
		source= end + (start-end)*scale;

		l2=(i+1)*itv;
		scale=l2/ds;
		as2=size*scale;
		dest= end + (start - end)*scale;
		//drawATrapezoid(painter, source, dest, as1, as2, color);
		if(flag)
		//if(as2!=0 && as1/as2>1.5 || as1/as2<0.67)
		{
			//dest= end + (start - end)*scale;
		    drawATrapezoid(painter, source, dest, as1, as2, color);
		}
		else
		{ 
			painter->setPen(QPen(color, (as1+as2)/2)); //; Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
			painter->drawLine(source, dest);
		}		
	}	
	painter->restore(); 	
}

void ItemBase::drawArrow_4_GL(QPainter *painter, QPointF start, QPointF end, float width, float height)
{//doted line
	painter->save(); 
	QPointF temp;
	temp = start; start =end; end =temp;	
	QLineF line(start, end);	
	float ds = line.length(); //sqrt(line.dx()*line.dx() + line.dy()*line.dy());  	
	float size=1+4*(width+height)/graphReferenceSize;	
	//qreal arrowSize(size);
	float itv;
	float dotSize=size*2.6;
    int count=ds/dotSize;
	if(count%2!=1)
		count=count-1;
	
	if(count<3)
		count=3;
	
	count=count*2;
	itv=ds/count;

	float l1,l2, as1, as2, scale;
	QPointF source, dest;	
	for(int i=0; i<count; i=i+2)
	{
		l1=i*itv;
		scale=l1/ds;
		as1=size*scale;
		source= end + (start-end)*scale;

		l2=(i+1)*itv;
		scale=l2/ds;
		as2=size*scale;
		dest= end + (start - end)*scale;
		drawATrapezoid_GL(painter, source, dest, as1, as2);		
	}
	painter->restore(); 
}



void ItemBase::drawArrow_4(QPainter *painter, QPointF start, float h, QPointF end, float width, float height, QColor color)
{//doted line

	painter->save(); 
	QLineF line(start, end);
    qreal arrowSize(h);

	float scale=0.5;
	QPointF start1;
	start1= (end-start)*scale + start;
	
	static const double Pi = 3.14159265358979323846264338327950288419717; 
    static double TwoPi = 2.0 * Pi;

    double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0)
		angle = TwoPi - angle;

    QPointF sourceArrowP1 = start1 + QPointF(sin(angle - Pi / 2.5) * arrowSize,
                                                  cos(angle - Pi / 2.5) * arrowSize);                                                  
    QPointF sourceArrowP2 = start1 + QPointF(sin(angle - Pi + Pi / 2.5) * arrowSize,
                                                  cos(angle - Pi + Pi / 2.5) * arrowSize);  

	QPointF dis1 = (sourceArrowP1 + sourceArrowP2)/2;	
    dis1=start1-dis1;
	sourceArrowP1 = sourceArrowP1 + dis1;
	sourceArrowP2 = sourceArrowP2 + dis1;

	float size;//=(1+4*(width+height)/graphReferenceSize)*0.5*3.1;		
	vector<QPointF> points = getCurveLine(start, sourceArrowP1, end);

	//get curve length
	float ds=0;
	for(vector<QPointF>::iterator it = points.begin(); it != points.end(); it++)
	{
		vector<QPointF>::iterator it1=it;
		   it1++;
		if(it1==points.end())
	       break;
		QPointF p1=*it, p2=*it1;
		p1=p2-p1;
		ds+=sqrt(pow(p1.x(),2) + pow(p1.y(),2));
	}
	
	size=1+4*(width+height)/graphReferenceSize;	
	
    float itv;
	float dotSize=size*2.6;
    int count=ds/dotSize;
	if(count%2!=1)
		count=count-1;

	count=count*2;
	itv=ds/count;

	bool flag=true;
	if(size/ds<0.2)
	   flag=false;
	
	float l1,l2, as1, as2;
	QPointF source, dest;	
	vector<QPointF>::iterator sIt=points.begin(); 
	for(int i=0; i<count; i=i+2)
	{
		l1=i*itv;
		scale=l1/ds;
		as1=size*scale;
		sIt=points.begin();
		source=Interpolation(points, sIt, l1);		

		l2=(i+1)*itv;
		scale=l2/ds;
		as2=size*scale;

		sIt=points.begin();
		
		dest=Interpolation(points, sIt, l2);
		drawATrapezoid(painter, source, dest, as1, as2, color);
	}	
	painter->restore(); 
}

QPointF ItemBase::Interpolation(vector<QPointF> &points, vector<QPointF>::iterator &start, float x)
{
	float d1,d2,d;
	d1=d2=0;
	for(vector<QPointF>::iterator it = start; it != points.end(); it++)
	{
		d2=d1;
		vector<QPointF>::iterator it1=it;
		it1++;

		if(it1==points.end())
	       break;

		QPointF p1=*it, p2=*it1, p3;
		p3=p2-p1;
		d1+=sqrt(pow(p3.x(),2) + pow(p3.y(),2));				
		if(d1>=x)
		{ 
			start=it;			
			d=(x-d2)/(d1-d2);
		    return(QPointF( (1-d)*p1.x() + d*p2.x(), (1-d)*p1.y() + d*p2.y() ));		   
		}
		
	}
	return( *points.rbegin() );	
}



QPointF ItemBase::Interpolation(vector<QPointF> &points, vector<QPointF>::iterator &start, float x, float &Dx)
{
	float d1,d2,d;
	d1=d2=Dx;
	for(vector<QPointF>::iterator it = start; it != points.end(); it++)
	{
		d2=d1;
		vector<QPointF>::iterator it1=it;
		it1++;

		if(it1==points.end())
	       break;

		QPointF p1=*it, p2=*it1, p3;
		p3=p2-p1;
		d1+=sqrt(pow(p3.x(),2) + pow(p3.y(),2));				
		if(d1>=x)
		{ 
			start=it;
			Dx=d1;
			d=(x-d2)/(d1-d2);
		    return(QPointF( (1-d)*p1.x() + d*p2.x(), (1-d)*p1.y() + d*p2.y() ));		   
		}
		
	}
	return( *points.rbegin() );	
}


void ItemBase::drawArrow_3(QPainter *painter, QPointF start, QPointF end, float width, float height, QColor color)
{//Polyline brocken 
	
	//painter->setBrush(color);
	float scale=0.5;
	QPointF start1;
	start1= (end-start)*scale + start;
	
	painter->save (); 
	painter->setRenderHint(QPainter::Antialiasing, true);
    QLineF line(start, end);

	float ds = sqrt(line.dx()*line.dx() + line.dy()*line.dy());  
	QPointF Dis = QPointF(6*line.dx()/ds, 6*line.dy()/ds);
	//start=start-Dis;

  

	painter->save (); 

	float size=(1+4*(width+height)/graphReferenceSize)*(1-scale)*15;	
	qreal arrowSize(size);

	static const double Pi = 3.14159265358979323846264338327950288419717; 
    static double TwoPi = 2.0 * Pi;

    //painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    //painter->drawLine(line);

	double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0)
		angle = TwoPi - angle;

    QPointF sourceArrowP1 = start1 + QPointF(sin(angle - Pi / 2.5) * arrowSize,
                                                  cos(angle - Pi / 2.5) * arrowSize);                                                  
    QPointF sourceArrowP2 = start1 + QPointF(sin(angle - Pi + Pi / 2.5) * arrowSize,
                                                  cos(angle - Pi + Pi / 2.5) * arrowSize);   

    
	QPointF dis1 = (sourceArrowP1 + sourceArrowP2)/2;
	
    dis1=start1-dis1;
	sourceArrowP1 = sourceArrowP1 + dis1;
	sourceArrowP2 = sourceArrowP2 + dis1;

	
	size=(1+4*(width+height)/graphReferenceSize)*0.5;	
	
	painter->setPen(QPen(color, size, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

	painter->drawLine(start,sourceArrowP1);
	painter->drawLine(sourceArrowP1,end);

	
	painter->restore(); 
}

void ItemBase::drawArrow_5(QPainter *painter, QPointF start, QPointF end,  QRect noteRect, float width, float height, QColor color)
{//for note-node connector, tail of callout bubble
    QLineF line(start, end);

	float ds = sqrt(line.dx()*line.dx() + line.dy()*line.dy());  
	QPointF Dis = QPointF(6*line.dx()/ds, 6*line.dy()/ds);
	//start=start-Dis;

	painter->setPen(color);
	painter->setBrush(color);
	painter->save (); 
	painter->setRenderHint(QPainter::Antialiasing, true);

	float size=1+4*(width+height)/graphReferenceSize;	
	qreal arrowSize(size*3);

	static const double Pi = 3.14159265358979323846264338327950288419717; 
    static double TwoPi = 2.0 * Pi;

    double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0)
		angle = TwoPi - angle;

    QPointF sourceArrowP1 = start + QPointF(sin(angle - Pi / 2.5) * arrowSize,
                                                  cos(angle - Pi / 2.5) * arrowSize);                                                  
    QPointF sourceArrowP2 = start + QPointF(sin(angle - Pi + Pi / 2.5) * arrowSize,
                                                  cos(angle - Pi + Pi / 2.5) * arrowSize);   
    
	QPointF dis = (sourceArrowP1 + sourceArrowP2)/2;
    dis=start-dis;
	sourceArrowP1 = sourceArrowP1 + dis;
	sourceArrowP2 = sourceArrowP2 + dis;

	QPointF endArrowP1, endArrowP2, endArrow; 

	arrowSize = 2;	
	endArrow = QPoint(line.p2().x(), line.p2().y());
	endArrowP1 = endArrow + QPointF(sin(angle - Pi / 2.5) * arrowSize,
                                                  cos(angle - Pi / 2.5) * arrowSize);  
	endArrowP2 = endArrow +  QPointF(sin(angle - Pi + Pi / 2.5) * arrowSize,
                                                  cos(angle - Pi + Pi / 2.5) * arrowSize); 
	//painter->setBrush(color);
	QVector<QPoint> points;	         
	points.append(QPoint(endArrowP2.x(), endArrowP2.y()));
	points.append(QPoint(endArrowP1.x(), endArrowP1.y()));
	points.append(QPoint(sourceArrowP1.x(), sourceArrowP1.y()));
	points.append(QPoint(sourceArrowP2.x(), sourceArrowP2.y()));
	QRegion arrow(points);
	
	//arrow = arrow -	noteRect;
	QRegion reg=getCalloutNoteRectforNote();	
	arrow = arrow -	reg;

	QPainterPath path;
 	path.addRegion( arrow); 	
 	painter->fillPath( path, QBrush( color ) );

	painter->restore(); 	
}

void ItemBase::drawNoteMark(QPainter *painter, QPointF start, QPointF end,  QRect noteRect, QPointF markPos, QRectF &markRect, float width, float height, QColor color)
{//for note-node connector, tail of callout bubble
	painter->setPen(color);
	painter->setBrush(color);
	painter->save (); 
	//painter->setRenderHint(QPainter::Antialiasing, true);
    QLineF line(start, end);

	float ds = sqrt(line.dx()*line.dx() + line.dy()*line.dy());  
	QPointF Dis = QPointF(6*line.dx()/ds, 6*line.dy()/ds);
	//start=start-Dis;

 

	float size=1+4*(width+height)/graphReferenceSize;	
	qreal arrowSize(size*1.5);


	//mark to tell there is a note
	start = markPos - QPointF(arrowSize,0);
	end = markPos;

	QRegion reg=getCalloutNoteRectforNote();	
	drawArrow(painter, start, end, width, height, color, reg);
	float w,h;
	w=fabs(start.x()-end.x());
	h=arrowSize*2;
    markRect=QRectF(start.x()-2,start.y()-h/2-2,w+4,h+4);	
	painter->restore(); 
	
}

void ItemBase::drawCross(QPainter *painter, QRectF cross, QColor color)
{//single arrow edge
	QPen pen;
	pen.setColor(color);
	pen.setWidthF(2);
	painter->setPen(pen);
	//painter->setBrush(color);
	painter->save (); 
	painter->setRenderHint(QPainter::Antialiasing, true);
    
    painter->drawLine(cross.x()+2,cross.y()+2,cross.x()+cross.width()-2, cross.y()+cross.height()-2);
	painter->drawLine(cross.x()+2,cross.y()+cross.height()-2, cross.x()+cross.width()-2, cross.y()+2);

	painter->setBrush(Qt::NoBrush);
	painter->drawRect(cross);
	//painter->drawLine(sourceArrowP1,line.p2());
	//painter->setRenderHint(QPainter::Antialiasing, false);
	painter->restore(); 
}

void ItemBase::drawMinus(QPainter *painter, QRectF minus, QColor color)
{//single arrow edge
	QPen pen;
	pen.setColor(color);
	pen.setWidthF(2);
	painter->setPen(pen);
	//painter->setBrush(color);
	painter->save (); 
	painter->setRenderHint(QPainter::Antialiasing, true);
    
    painter->drawLine(minus.x()+2, minus.y()+ minus.height()/2+1, minus.x()+minus.width()-2, minus.y()+minus.height()/2+1);	

	painter->setBrush(Qt::NoBrush);
	painter->drawRect(minus);	
	painter->restore(); 	
}

void ItemBase::drawMaximize(QPainter *painter, QRectF maximize, QColor color)
{//single arrow edge
	QPen pen;
	pen.setColor(color);
	pen.setWidthF(2);
	painter->setPen(pen);
	//painter->setBrush(color);
	painter->save (); 
	painter->setRenderHint(QPainter::Antialiasing, true);

	int w=maximize.width()*0.8, h=maximize.height()*0.8;
	QRectF newRect= QRectF(maximize.center().x()-w/2, maximize.center().y()-h/2, w, h);
    
    //painter->drawLine(maximize.x()+2, maximize.y()+ maximize.height()/2+1, maximize.x()+maximize.width()-2, maximize.y()+maximize.height()/2+1);	
	painter->drawRect(maximize);	

	painter->setBrush(Qt::NoBrush);
	painter->drawRect(maximize);	
	painter->restore(); 	
}


void ItemBase::drawCurvedArrow_GL(QPainter *painter, set<int> code, float h, QPointF start, QPointF end, float width, float height, float _scale)
{   
	//switch all the start and and // keqin
	QVector<QPointF> points;
	if(code.size()==0)
		return;

	painter->save(); 
	if(code.size()==1)
	{	
		 switch(*code.begin())
		 {
			case 0:  points = getArrow(start, end, width, height, _scale); glColor4f(0.196, 0.196, 0.196, 1);  
				     painter->setPen(QColor(50, 50, 50, 255));	
	                 break; //, QColor(50, 50, 50, 255));	
			case 1:  points = getArrow(end, start, width, height, _scale); glColor4f(0.196, 0.196, 0.196, 1);
				     painter->setPen(QColor(50, 50, 50, 255));	
				     break; //, QColor(50, 50, 50, 255));	break;
			case 2:  points = getArrow(start, end, width, height, _scale); glColor4f(0, 0.825, 0.196, 1); 
				     painter->setPen(QColor(0, 210, 50, 255));	
					 break;//,  QColor(0, 210, 50, 255));	break;	
			case 3:  points = getArrow(end, start, width, height, _scale); glColor4f(0, 0.825, 0.196, 1); 
				     painter->setPen(QColor(0, 210, 50, 255));
					 break;//,  QColor(0, 210, 50, 255));	break;			 
			case 4:  points = getArrow(start, end, width, height, _scale); glColor4f(1, 0.5, 0, 1);  
				     painter->setPen(QColor(255, 128, 0, 255));  
					 break;//,  QColor(255, 128, 0, 255));	break;
			case 5:  points = getArrow(end, start, width, height, _scale); glColor4f(1, 0.5, 0, 1);  
				     painter->setPen(QColor(255, 128, 0, 255));
					 break;//,  QColor(255, 128, 0, 255));	break;
		}	
		painter->drawLine(0,1,0,1);
		glBegin(GL_QUADS); 
		glVertex2f(points[0].x(),points[0].y());
		glVertex2f(points[1].x(),points[1].y());
		glVertex2f(points[2].x(),points[2].y());
		glVertex2f(points[3].x(),points[3].y());
		glEnd();
	}
	else 
	{	
		int size = code.size();

		int count=0;
		size=size-1;
		h=h*15;
		float v=h/size/4;
		float h2=v*size;
		float c=-h2/2;
		if(size==0)
			c=0, v=0;

		int count1=0;
		for(set<int>::iterator it=code.begin(); it!=code.end(); it++)
		{ 
			
			 switch(*it)
			 {
				case 0:  if(size==0) 
							 points=getArrow(start, end, width, height,_scale);//, QColor(50, 50, 50, 255) );
						 else
							 points=getArrow(start, c, end, width, height,_scale);//, QColor(50, 50, 50, 255) );	
						 c=c+v; 
						 painter->setPen(QColor(50, 50, 50, 255));
						 glColor4f(0.196, 0.196, 0.196, 1); 
					     break;
				case 1:  if(size==0) 
					         points=getArrow(end, start, width, height,_scale);//, QColor(50, 50, 50, 255) );
						 else
							 points=getArrow(end, -c, start, width, height,_scale);//, QColor(50, 50, 50, 255) );	
					     
					     painter->setPen(QColor(50, 50, 50, 255));     
					     glColor4f(0.196, 0.196, 0.196, 1); 
						 c=c+v; 
					     break;
				case 2:  if(size==0) 
							 points=getArrow_4(start, end, width, height,_scale);//, QColor(0, 210, 50, 255));	
						 else
							 points=getArrow_4(start, c, end, width, height,_scale);//, QColor(0, 210, 50, 255));	
					     c=c+v;
						 painter->setPen(QColor(0, 210, 50, 255));
						 glColor4f(0, 0.825, 0.196, 1); 
					     break;	
				case 3:  if(size==0) 
							 points=getArrow_4(end, start, width, height,_scale);//, QColor(0, 210, 50, 255));	 					     
						 else 
							 points=getArrow_4(end, -c, start, width, height,_scale);//, QColor(0, 210, 50, 255));
					     c=c+v;
						 glColor4f(0, 0.825, 0.196, 1); 
						 painter->setPen(QColor(0, 210, 50, 255));
					     break;	 
				case 4:  if(size==0) 
							 points=getArrow_4(start, end, width, height,_scale);//, QColor(255, 128, 0, 255) );	
						 else 
							 points=getArrow_4(start, c, end, width, height,_scale);//, QColor(255, 128, 0, 255) );	
					     c=c+v; 
						 glColor4f(1, 0.5, 0, 1);
						 painter->setPen(QColor(255, 128, 0, 255));
						 break;
				case 5:  if(size==0) 
							 points=getArrow_4(end, start, width, height,_scale);//, QColor(255, 128, 0, 255) );	 
						 else 
							 points=getArrow_4(end, -c, start, width, height,_scale);//, QColor(255, 128, 0, 255) );	 
					     glColor4f(1, 0.5, 0, 1);
						 painter->setPen(QColor(255, 128, 0, 255));
					     c=c+v; break;
			}
			painter->drawLine(0,1,0,1);
			glBegin(GL_QUADS); 
			for(int i=0;i<points.size();i++)
			     glVertex2f(points[i].x(),points[i].y());			
			glEnd();
		}
	}	
	painter->restore(); 
}


void ItemBase::drawCurvedArrow_GL_H(QPainter *painter, set<int> code, float h, QPointF start, QPointF end, float width, float height, float _scale, QColor color)
{   
	//switch all the start and and // keqin
	
	
	QVector<QPointF> points;
	if(code.size()==0)
	{
		
		return;
	}

	painter->save(); 

	//QColor color=HIGHLIGHTCOLOR;
	painter->setPen(color);	
	glColor4f(color.redF()/255.0, color.greenF()/255.0, color.blueF()/255.0, color.alphaF()/255.0);		
	painter->drawLine(0,1,0,1);

	if(code.size()==1)
	{	
		 switch(*code.begin())
		 {
			case 0:  points = getArrow_H(start, end, width, height, _scale); 
				    
	                 break; //, QColor(50, 50, 50, 255));	
			case 1:  points = getArrow_H(end, start, width, height, _scale); 
				    
				     break; //, QColor(50, 50, 50, 255));	break;
			case 2:  points = getArrow_H(start, end, width, height, _scale); 
				    
					 break;//,  QColor(0, 210, 50, 255));	break;	
			case 3:  points = getArrow_H(end, start, width, height, _scale); 
				    
				     
					 break;//,  QColor(0, 210, 50, 255));	break;			 
			case 4:  points = getArrow_H(start, end, width, height, _scale); 
					 break;//,  QColor(255, 128, 0, 255));	break;
			case 5:  points = getArrow_H(end, start, width, height, _scale); 
					 break;//,  QColor(255, 128, 0, 255));	break;
		}	
		
		glBegin(GL_QUADS); 
		glVertex2f(points[0].x(),points[0].y());
		glVertex2f(points[1].x(),points[1].y());
		glVertex2f(points[2].x(),points[2].y());
		glVertex2f(points[3].x(),points[3].y());
		glEnd();
	}
	else 
	{	
		int size = code.size();
		int count=0;
		size=size-1;
		h=h*15;
		float v=h/size/4;
		float h2=v*size;
		float c=-h2/2;
		if(size==0)
			c=0, v=0;

		int count1=0;
		for(set<int>::iterator it=code.begin(); it!=code.end(); it++)
		{ 
			 switch(*it)
			 {
				case 0:  if(size==0) 
							 points=getArrow_H(start, end, width, height,_scale);
						 else
							 points=getArrow_H(start, c, end, width, height,_scale);
						 c=c+v; 
					     break;
				case 1:  if(size==0) 
					         points=getArrow_H(end, start, width, height,_scale);
						 else
							 points=getArrow_H(end, -c, start, width, height,_scale);
						 c=c+v; 
					     break;
				case 2:  if(size==0) 
							 points=getArrow_H(start, end, width, height,_scale);
						 else
							points=getArrow_H(start, -c, end, width, height,_scale);
					     c=c+v;
						 
					     break;	
				case 3:  if(size==0) 
							 points=getArrow_H(end, start, width, height,_scale);	 					     
						 else 
							 points=getArrow_H(end, c, start, width, height,_scale);
					     c=c+v;
					     break;	 
				case 4:  if(size==0) 
							 points=getArrow_H(start, end, width, height,_scale);	
						 else 
							 points=getArrow_H(start, -c, end, width, height,_scale);	
					     c=c+v; 
						 break;
				case 5:  if(size==0) 
							 points=getArrow_H(end, start, width, height,_scale);	 
						 else 
							 points=getArrow_H(end, c, start, width, height,_scale);	
					     c=c+v; 
						 break;
			}
			
			glBegin(GL_QUADS); 
			for(int i=0;i<points.size();i++)
			     glVertex2f(points[i].x(),points[i].y());			
			glEnd();
		}		
	}	
	painter->restore(); 
}

void ItemBase::drawCurvedArrow_H(QPainter *painter, set<int> code, float h, QPointF start, QPointF end, float width, float height, float _scale, QColor color)
{   
	//switch all the start and and // keqin
	
	QVector<QPointF> points;
	if(code.size()==0)
	{
		return;
	}

	painter->save(); 
	//QColor color=HIGHLIGHTCOLOR;
	painter->setPen(Qt::NoPen);	
	painter->setBrush(color);		

	if(code.size()==1)
	{	
		 switch(*code.begin())
		 {
			case 0:  points = getArrow_H(start, end, width, height, _scale); 				    
	                 break; //, QColor(50, 50, 50, 255));	
			case 1:  points = getArrow_H(end, start, width, height, _scale);
				    
				     break; //, QColor(50, 50, 50, 255));	break;
			case 2:  points = getArrow_H(start, end, width, height, _scale);				    
					 break;//,  QColor(0, 210, 50, 255));	break;	
			case 3:  points = getArrow_H(end, start, width, height, _scale); 				     
					 break;//,  QColor(0, 210, 50, 255));	break;			 
			case 4:  points = getArrow_H(start, end, width, height, _scale); 				     
					 break;//,  QColor(255, 128, 0, 255));	break;
			case 5:  points = getArrow_H(end, start, width, height, _scale);  				     
					 break;//,  QColor(255, 128, 0, 255));	break;
		}	
		
		/*glBegin(GL_QUADS); 
		glVertex2f(points[0].x(),points[0].y());
		glVertex2f(points[1].x(),points[1].y());
		glVertex2f(points[2].x(),points[2].y());
		glVertex2f(points[3].x(),points[3].y());
		glEnd();*/
		painter->drawPolygon(QPolygonF() << points[0] << points[1] << points[2] << points[3]);
	}
	else 
	{	
		int size = code.size();

		int count=0;
		size=size-1;
		h=h*15;
		float v=h/size/4;
		float h2=v*size;
		float c=-h2/2;
		if(size==0)
			c=0, v=0;

		int count1=0;
		for(set<int>::iterator it=code.begin(); it!=code.end(); it++)
		{ 
			
			 switch(*it)
			 {
				case 0:  if(size==0) 
							 points=getArrow_H(start, end, width, height,_scale);//, QColor(50, 50, 50, 255) );
						 else
							 points=getArrow_H(start, c, end, width, height,_scale);//, QColor(50, 50, 50, 255) );	
						 c=c+v; 
						 
					     break;
				case 1:  if(size==0) 
					         points=getArrow_H(end, start, width, height,_scale);//, QColor(50, 50, 50, 255) );
						 else
							 points=getArrow_H(end, -c, start, width, height,_scale);//, QColor(50, 50, 50, 255) );	
						 c=c+v; 
					     break;
				case 2:  if(size==0) 
							 points=getArrow_H(start, end, width, height,_scale);//, QColor(0, 210, 50, 255));	
						 else
							 points=getArrow_H(start, c, end, width, height,_scale);//, QColor(0, 210, 50, 255));	
					     c=c+v;
						 
						 glColor4f(0, 0.825, 0.196, 1); 
					     break;	
				case 3:  if(size==0) 
							 points=getArrow_H(end, start, width, height,_scale);//, QColor(0, 210, 50, 255));	 					     
						 else 
							 points=getArrow_H(end, -c, start, width, height,_scale);//, QColor(0, 210, 50, 255));
					     c=c+v;
						
					     break;	 
				case 4:  if(size==0) 
							 points=getArrow_H(start, end, width, height,_scale);//, QColor(255, 128, 0, 255) );	
						 else 
							 points=getArrow_H(start, c, end, width, height,_scale);//, QColor(255, 128, 0, 255) );	
					     c=c+v; 
						 
						 break;
				case 5:  if(size==0) 
							 points=getArrow_H(end, start, width, height,_scale);//, QColor(255, 128, 0, 255) );	 
						 else 
							 points=getArrow_H(end, -c, start, width, height,_scale);//, QColor(255, 128, 0, 255) );	 
					     
					     c=c+v; break;
			}
			
			for(int i=0;i<points.size();i=i+4)
			{	
				painter->drawPolygon(QPolygonF() << points[i] << points[i+1] << points[i+2] << points[i+3]);
				//glVertex2f(points[i].x(),points[i].y());			
			}
			
		}		
	}	
	painter->restore(); 
}



void ItemBase::drawCurvedArrow(QPainter *painter, set<int> code, float h, QPointF start, QPointF end, float width, float height)
{   	
	//switch all the start and and // keqin	
	if(code.size()==0)
		return;

	if(code.size()==1)
	{	
		 switch(*code.begin())
		 {
			case 0:  drawArrow(painter, start, end, width, height, QColor(50, 50, 50, 255));	break;
			case 1:  drawArrow(painter, end, start, width, height, QColor(50, 50, 50, 255));	break;
			case 2:  drawArrow_4(painter, start, end, width, height,  QColor(0, 210, 50, 255));	break;	
			case 3:  drawArrow_4(painter, end, start, width, height,  QColor(0, 210, 50, 255));	break;			 
			case 4:  drawArrow_4(painter, start, end, width, height,  QColor(255, 128, 0, 255));	break;
			case 5:  drawArrow_4(painter, end, start, width, height,  QColor(255, 128, 0, 255));	break;
		}	
	}
	else 
	{	
		int size = code.size();

		int count=0;
		size=size-1;
		h=h*15;
		float v=h/size/4;
		float h2=v*size;
		float c=-h2/2;
		if(size==0)
			c=0, v=0;

		int count1=0;
		for(set<int>::iterator it=code.begin(); it!=code.end(); it++)
		{ 
			 //it++;
			 /*if(count1>2)
				break;
			 count1++;*/
			 switch(*it)
			 {
				case 0:  if(size==0) 
							 drawArrow(painter, start, end, width, height, QColor(50, 50, 50, 255) );
						 else
						   drawArrow(painter, start, c, end, width, height, QColor(50, 50, 50, 255) );	
						 c=c+v; 
					     break;
				case 1:  if(size==0) 
					         drawArrow(painter, end, start, width, height, QColor(50, 50, 50, 255) );
						 else
						 	 drawArrow(painter, end, -c, start, width, height, QColor(50, 50, 50, 255) );	
						 c=c+v; 
					     break;
				case 2:  if(size==0) 
							 drawArrow_4(painter, start, end, width, height, QColor(0, 210, 50, 255));	
						 else
							 drawArrow_4(painter, start, c, end, width, height, QColor(0, 210, 50, 255));	
					     c=c+v;
					     break;	
				case 3:  if(size==0) 
							 drawArrow_4(painter, end, start, width, height, QColor(0, 210, 50, 255));	 					     
						 else 
							 drawArrow_4(painter, end, -c, start, width, height, QColor(0, 210, 50, 255));
					     c=c+v;
					     break;	 
				case 4:  if(size==0) 
							 drawArrow_4(painter, start, end, width, height, QColor(255, 128, 0, 255) );	
						 else 
							 drawArrow_4(painter, start, c, end, width, height, QColor(255, 128, 0, 255) );	
					     c=c+v; 
						 break;
				case 5:  if(size==0) 
							 drawArrow_4(painter, end, start, width, height, QColor(255, 128, 0, 255) );	 
						 else 
							 drawArrow_4(painter, end, -c, start, width, height, QColor(255, 128, 0, 255) );	 
					     c=c+v; break;
			}
			//break;		
		}
		
	}	
}

void ItemBase::drawArrow(QPainter *painter, QPointF start, float h, QPointF end, float width, float height, QColor color, QRegion clipRegion)
{//single curved arrow
	QPointF temp;
	temp = start; start =end; end =temp;

	
	//painter->setBrush(color);
	float scale=0.5;
	QPointF start1;
	start1= (end-start)*scale + start;
	

    QLineF line(start, end);

	float ds = sqrt(line.dx()*line.dx() + line.dy()*line.dy());  
	QPointF Dis = QPointF(6*line.dx()/ds, 6*line.dy()/ds);
	//start=start-Dis;

    if(qFuzzyCompare(line.length(), qreal(0.)))
        return;

	
	painter->save (); 
	painter->setRenderHint(QPainter::Antialiasing, true);

	float size=h;//(1+4*(width+height)/graphReferenceSize)*(1-scale)*15;	
	qreal arrowSize(size);

	static const double Pi = 3.14159265358979323846264338327950288419717; 
    static double TwoPi = 2.0 * Pi;

    //painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    //painter->drawLine(line);

	double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0)
		angle = TwoPi - angle;

    QPointF sourceArrowP1 = start1 + QPointF(sin(angle - Pi / 2.5) * arrowSize,
                                                  cos(angle - Pi / 2.5) * arrowSize);                                                  
    QPointF sourceArrowP2 = start1 + QPointF(sin(angle - Pi + Pi / 2.5) * arrowSize,
                                                  cos(angle - Pi + Pi / 2.5) * arrowSize);  

	QPointF dis1 = (sourceArrowP1 + sourceArrowP2)/2;	
    dis1=start1-dis1;
	sourceArrowP1 = sourceArrowP1 + dis1;
	sourceArrowP2 = sourceArrowP2 + dis1;

	size=(1+4*(width+height)/graphReferenceSize)*0.5*3.1;	
	painter->setPen(QPen(color, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

	vector<QPointF> points = getCurveLine(start, sourceArrowP1, end);


	//float dotSize=size*2.6;
    /*int count=ds/dotSize;
	if(count%2!=1)
		count=count-1;
	if(count<3)
		count=3;
	
	count=count*2;*/
	
	float itv=size/(points.size()-1);
	float as1, as2;
	int count=points.size()-1;
	for(vector<QPointF>::iterator it = points.begin(); it != points.end(); it++)
	{
		as1 = count*itv, as2 = (count-1)*itv;
	    QPointF start = *it;		
		vector<QPointF>::iterator it1=it;
		it1++;
		if(it1==points.end())
			break;
		QPointF end = *it1;
		drawATrapezoid(painter, start, end, as1, as2, color);		
		count--;		
		//painter->drawLine(start, end);
		//break;
	}
	//painter->drawLine(start,sourceArrowP1);
	//painter->drawLine(sourceArrowP1,end);
	painter->restore(); 
}

vector<QPointF> ItemBase::getCurveLine(QPointF start, QPointF mid, QPointF end)
{
	set<vector<float>> points;
	vector<QPointF> output;
	vector<float> S,M,E;
	S.push_back(0); S.push_back(start.x()); S.push_back(start.y());
	M.push_back(0.5); M.push_back(mid.x()); M.push_back(mid.y());
	E.push_back(1.0); E.push_back(end.x()); E.push_back(end.y());
	points.insert(S);
	points.insert(M);
	points.insert(E);
	getMids(points);
	for(set<vector<float>>::iterator it=points.begin(); it!=points.end(); it++)
	{
	    vector<float> temp = *it;
		QPointF P=QPointF(temp[1],temp[2]);
		output.push_back(P);
	}
	return output;
	//void ItemBase::getCurveLine(QPointF start, PointF mid, QPointF end)
}

void ItemBase::getMids(set<vector<float>> &points)
{//assume start, mid, and end are already stored in list
     set<vector<float>> points1;
	 for(int i=0; i<2; i++)
	 {
		 points1=points;
		 vector<vector<float> > midNode;
		 vector<float> S=*points1.begin(), E=*points1.rbegin();
		 for(set<vector<float>>::iterator it=points1.begin(); it!=points1.end(); it++,it++)
		 {
			 set<vector<float>>::iterator it1=it;
			 it1++;			 
			 if(it1==points1.end())
				 break;
			 vector<float> temp, start=*it, end=*it1;
			 temp.push_back((start[0]+end[0])/2.0);
			 temp.push_back((start[1]+end[1])/2.0);
			 temp.push_back((start[2]+end[2])/2.0);
			 points1.insert(temp);
			 if(end!=E)
			     midNode.push_back(end);
			 //getMids(it, it1, it2, points)	 
		 }
		 for(vector<vector<float>>::iterator it=midNode.begin(); it!=midNode.end(); it++)
		 {
			 vector<float> temp=*it;	
			 points1.erase(temp);					 
			 //getMids(it, it1, it2, points)	 
		 }
		 points=points1;
	 }
}

void ItemBase::drawArrow_GL(QPainter *painter, QPointF start, QPointF end, float width, float height)
{//single arrow edge
	painter->save(); 
	QPointF temp;
	temp = start; start =end; end =temp;
	QLineF line(start, end);

	float ds = sqrt(line.dx()*line.dx() + line.dy()*line.dy());  
	QPointF Dis = QPointF(6*line.dx()/ds, 6*line.dy()/ds);
	
	float size=1+4*(width+height)/graphReferenceSize;	
	qreal arrowSize(size*1.5);
	qreal arrowSize2(1);

	static const double Pi = 3.14159265358979323846264338327950288419717; 
    static double TwoPi = 2.0 * Pi;

    double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0)
		angle = TwoPi - angle;

    QPointF sourceArrowP1 = start + QPointF(sin(angle - Pi / 2.5) * arrowSize,
                                                  cos(angle - Pi / 2.5) * arrowSize);                                                  
    QPointF sourceArrowP2 = start + QPointF(sin(angle - Pi + Pi / 2.5) * arrowSize,
                                                  cos(angle - Pi + Pi / 2.5) * arrowSize);   
   	
	QPointF dis = (sourceArrowP1 + sourceArrowP2)/2;
    dis=start-dis;
	sourceArrowP1 = sourceArrowP1 + dis;
	sourceArrowP2 = sourceArrowP2 + dis;
	
	//glBegin(GL_POLYGON);    
	/*glVertex2f(line.p2().x(),line.p2().y());
	glVertex2f(sourceArrowP1.x(), sourceArrowP1.y());
	glVertex2f(sourceArrowP2.x(), sourceArrowP2.y());
	glVertex2f(line.p2().x(),line.p2().y());*/
	
	QPointF destArrowP1 = end + QPointF(sin(angle - Pi / 2.6) * arrowSize2, cos(angle - Pi / 2.6) * arrowSize2);
	QPointF destArrowP2 = end + QPointF(sin(angle - Pi + Pi / 2.6) * arrowSize2, cos(angle - Pi + Pi / 2.6) * arrowSize2);

	dis = (destArrowP1 + destArrowP2)/2;
	dis=end-dis;
	destArrowP1 = destArrowP1 + dis;
	destArrowP2 = destArrowP2 + dis;
	
	//glBegin(GL_POLYGON);    
	glVertex2f(destArrowP1.x(), destArrowP1.y());
	glVertex2f(sourceArrowP1.x(), sourceArrowP1.y());
	glVertex2f(sourceArrowP2.x(), sourceArrowP2.y());
	glVertex2f(destArrowP2.x(), destArrowP2.y());
		
	painter->restore(); 	
}

void ItemBase::drawArrow_GL_H(QPainter *painter, QPointF start, QPointF end, float width, float height, float _scale)
{//highlight Background single arrow edge
	painter->save(); 
	QPointF temp;
	temp = start; start =end; end =temp;
	QLineF line(start, end);

	float ds = sqrt(line.dx()*line.dx() + line.dy()*line.dy());  
	QPointF Dis = QPointF(6*line.dx()/ds, 6*line.dy()/ds);
	//start=start-Dis;
	  
	//painter->setRenderHint(QPainter::Antialiasing, true);
    float exWidth = HBWIDTH*_scale*2;
	float size= (1+4*(width+height)/graphReferenceSize)*1.5 + exWidth;	
	float sd = exWidth;//size - (1+4*(width+height)/graphReferenceSize)*1.5;
	qreal arrowSize(size);
	qreal arrowSize2(sd);

	static const double Pi = 3.14159265358979323846264338327950288419717; 
    static double TwoPi = 2.0 * Pi;

    //painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    //painter->drawLine(line);

	double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0)
		angle = TwoPi - angle;

    QPointF sourceArrowP1 = start + QPointF(sin(angle - Pi / 2.5) * arrowSize,
                                                  cos(angle - Pi / 2.5) * arrowSize);                                                  
    QPointF sourceArrowP2 = start + QPointF(sin(angle - Pi + Pi / 2.5) * arrowSize,
                                                  cos(angle - Pi + Pi / 2.5) * arrowSize);   
    //QPointF destArrowP1 = end + QPointF(sin(angle - Pi / 2.6) * arrowSize, cos(angle - Pi / 2.6) * arrowSize);
    //QPointF destArrowP2 = end + QPointF(sin(angle - Pi + Pi / 2.6) * arrowSize, cos(angle - Pi + Pi / 2.6) * arrowSize);
	
	QPointF dis = (sourceArrowP1 + sourceArrowP2)/2;
    dis=start-dis;
	sourceArrowP1 = sourceArrowP1 + dis;
	sourceArrowP2 = sourceArrowP2 + dis;



	QPointF destArrowP1 = end + QPointF(sin(angle - Pi / 2.6) * arrowSize2, cos(angle - Pi / 2.6) * arrowSize2);
	QPointF destArrowP2 = end + QPointF(sin(angle - Pi + Pi / 2.6) * arrowSize2, cos(angle - Pi + Pi / 2.6) * arrowSize2);

	dis = (destArrowP1 + destArrowP2)/2;
	dis=end-dis;
	destArrowP1 = destArrowP1 + dis;
	destArrowP2 = destArrowP2 + dis;

	//painter->drawPolygon(QPolygonF() << destArrowP1 << sourceArrowP1 << sourceArrowP2 << destArrowP2);

	/*QVector<QPointF> points;	         
	points.append(QPoint(line.p2().x(),line.p2().y()));
	points.append(QPoint(sourceArrowP1.x(), sourceArrowP1.y()));
	points.append(QPoint(sourceArrowP2.x(), sourceArrowP2.y()));*/
	//QRegion arrow(points);
	
	//glBegin(GL_POLYGON);    
	glVertex2f(destArrowP1.x(), destArrowP1.y());
	glVertex2f(sourceArrowP1.x(), sourceArrowP1.y());
	glVertex2f(sourceArrowP2.x(), sourceArrowP2.y());
	glVertex2f(destArrowP2.x(), destArrowP2.y());
	//glEnd();

	painter->restore(); 
	
}

void ItemBase::drawArrow_H(QPainter *painter, QPointF start, QPointF end, float width, float height, float _scale,  QColor color, QRegion clipRegion)
{//highlight Background single arrow edge
	painter->save (); 
	
	QPointF temp;
	temp = start; start =end; end =temp;
	QLineF line(start, end);

	float ds = sqrt(line.dx()*line.dx() + line.dy()*line.dy());  
	QPointF Dis = QPointF(6*line.dx()/ds, 6*line.dy()/ds);
	//start=start-Dis;
	  
	//painter->setRenderHint(QPainter::Antialiasing, true);
	painter->setPen(color);
	painter->setBrush(color);

    float exWidth=HBWIDTH*_scale*2;
	float size= (1+4*(width+height)/graphReferenceSize)*1.5 + exWidth;	
	float sd = exWidth; //size - (1+4*(width+height)/graphReferenceSize)*1.5;
	qreal arrowSize(size);
	qreal arrowSize2(sd);

	static const double Pi = 3.14159265358979323846264338327950288419717; 
    static double TwoPi = 2.0 * Pi;

    //painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    //painter->drawLine(line);

	double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0)
		angle = TwoPi - angle;

    QPointF sourceArrowP1 = start + QPointF(sin(angle - Pi / 2.5) * arrowSize,
                                                  cos(angle - Pi / 2.5) * arrowSize);                                                  
    QPointF sourceArrowP2 = start + QPointF(sin(angle - Pi + Pi / 2.5) * arrowSize,
                                                  cos(angle - Pi + Pi / 2.5) * arrowSize);   
    //QPointF destArrowP1 = end + QPointF(sin(angle - Pi / 2.6) * arrowSize, cos(angle - Pi / 2.6) * arrowSize);
    //QPointF destArrowP2 = end + QPointF(sin(angle - Pi + Pi / 2.6) * arrowSize, cos(angle - Pi + Pi / 2.6) * arrowSize);
	
	QPointF dis = (sourceArrowP1 + sourceArrowP2)/2;
    dis=start-dis;
	sourceArrowP1 = sourceArrowP1 + dis;
	sourceArrowP2 = sourceArrowP2 + dis;

	QPointF destArrowP1 = end + QPointF(sin(angle - Pi / 2.6) * arrowSize2, cos(angle - Pi / 2.6) * arrowSize2);
	QPointF destArrowP2 = end + QPointF(sin(angle - Pi + Pi / 2.6) * arrowSize2, cos(angle - Pi + Pi / 2.6) * arrowSize2);

	dis = (destArrowP1 + destArrowP2)/2;
	dis=end-dis;
	destArrowP1 = destArrowP1 + dis;
	destArrowP2 = destArrowP2 + dis;

	painter->drawPolygon(QPolygonF() << destArrowP1 << sourceArrowP1 << sourceArrowP2 << destArrowP2);
	
	QVector<QPoint> points;	         
	points.append(QPoint(destArrowP1.x(), destArrowP1.y()));
	points.append(QPoint(sourceArrowP1.x(), sourceArrowP1.y()));
	points.append(QPoint(sourceArrowP2.x(), sourceArrowP2.y()));
	points.append(QPoint(destArrowP2.x(), destArrowP2.y()));
	QRegion arrow(points);
	
	arrow = arrow -	clipRegion;

	QPainterPath path;
 	path.addRegion( arrow); 	
	painter->drawLine(start, end);
 	painter->fillPath( path, QBrush( color ) );

	painter->restore(); 	
	/*QVector<QPointF> points;	         
	points.append(QPoint(line.p2().x(),line.p2().y()));
	points.append(QPoint(sourceArrowP1.x(), sourceArrowP1.y()));
	points.append(QPoint(sourceArrowP2.x(), sourceArrowP2.y()));*/
	//QRegion arrow(points);
	
	//glBegin(GL_POLYGON);    
	/*glVertex2f(destArrowP1.x(), destArrowP1.y());
	glVertex2f(sourceArrowP1.x(), sourceArrowP1.y());
	glVertex2f(sourceArrowP2.x(), sourceArrowP2.y());
	glVertex2f(destArrowP2.x(), destArrowP2.y());*/
	//glEnd();
	
}

void ItemBase::drawArrow(QPainter *painter, QPointF start, QPointF end, float width, float height, QColor color, QRegion clipRegion)
{//single arrow edge
	 painter->save (); 
	 QPointF temp;
	temp = start; start =end; end =temp;
	QLineF line(start, end);

	float ds = sqrt(line.dx()*line.dx() + line.dy()*line.dy());  
	QPointF Dis = QPointF(6*line.dx()/ds, 6*line.dy()/ds);
	//start=start-Dis;

   
	painter->setPen(color);
	painter->setBrush(color);
	
	//painter->setRenderHint(QPainter::Antialiasing, true);
    

	float size=1+4*(width+height)/graphReferenceSize;	
	qreal arrowSize(size*1.5);

	static const double Pi = 3.14159265358979323846264338327950288419717; 
    static double TwoPi = 2.0 * Pi;

    double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0)
		angle = TwoPi - angle;

    QPointF sourceArrowP1 = start + QPointF(sin(angle - Pi / 2.5) * arrowSize,
                                                  cos(angle - Pi / 2.5) * arrowSize);                                                  
    QPointF sourceArrowP2 = start + QPointF(sin(angle - Pi + Pi / 2.5) * arrowSize,
                                                  cos(angle - Pi + Pi / 2.5) * arrowSize);   
   	
	QPointF dis = (sourceArrowP1 + sourceArrowP2)/2;
    dis=start-dis;
	sourceArrowP1 = sourceArrowP1 + dis;
	sourceArrowP2 = sourceArrowP2 + dis;
	

	QVector<QPoint> points;	         
	points.append(QPoint(line.p2().x(),line.p2().y()));
	points.append(QPoint(sourceArrowP1.x(), sourceArrowP1.y()));
	points.append(QPoint(sourceArrowP2.x(), sourceArrowP2.y()));
	QRegion arrow(points);
	
	arrow = arrow -	clipRegion;

	QPainterPath path;
 	path.addRegion( arrow); 	
	painter->drawLine(start, end);
 	painter->fillPath( path, QBrush( color ) );
	
	painter->restore(); 	
}

QVector<QPointF> ItemBase::getArrow(QPointF start, float h, QPointF end, float width, float height, float _scale)
{//single curved arrow
	QVector<QPointF> QuadPoints;
	QPointF temp;
	temp = start; start =end; end =temp;
		
	//painter->setBrush(color);
	float scale=0.5;
	QPointF start1;
	start1= (end-start)*scale + start;
	

    QLineF line(start, end);

	float ds = sqrt(line.dx()*line.dx() + line.dy()*line.dy());  
	QPointF Dis = QPointF(6*line.dx()/ds, 6*line.dy()/ds);
	//start=start-Dis;


	float size=h;//(1+4*(width+height)/graphReferenceSize)*(1-scale)*15;	
	qreal arrowSize(size);

	static const double Pi = 3.14159265358979323846264338327950288419717; 
    static double TwoPi = 2.0 * Pi;

	double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0)
		angle = TwoPi - angle;

    QPointF sourceArrowP1 = start1 + QPointF(sin(angle - Pi / 2.5) * arrowSize,
                                                  cos(angle - Pi / 2.5) * arrowSize);                                                  
    QPointF sourceArrowP2 = start1 + QPointF(sin(angle - Pi + Pi / 2.5) * arrowSize,
                                                  cos(angle - Pi + Pi / 2.5) * arrowSize);  

	QPointF dis1 = (sourceArrowP1 + sourceArrowP2)/2;	
    dis1=start1-dis1;
	sourceArrowP1 = sourceArrowP1 + dis1;
	sourceArrowP2 = sourceArrowP2 + dis1;

	size=(1+4*(width+height)/graphReferenceSize)*0.5*3.1;	
	
	vector<QPointF> points = getCurveLine(start, sourceArrowP1, end);

	
	float itv=size/(points.size()-1)*_scale;
	float as1, as2;
	int count=points.size()-1;
	for(vector<QPointF>::iterator it = points.begin(); it != points.end(); it++)
	{
		as1 = count*itv, as2 = (count-1)*itv;
	    QPointF start = *it;		
		vector<QPointF>::iterator it1=it;
		it1++;
		if(it1==points.end())
			break;
		QPointF end = *it1;
		getATrapezoid(start, end, as1, as2, QuadPoints);		
		count--;		
		//painter->drawLine(start, end);
		//break;
	}

	return QuadPoints;
}

QVector<QPointF> ItemBase::getArrow_H(QPointF start, float h, QPointF end, float width, float height, float _scale)
{//single curved arrow

	QVector<QPointF> QuadPoints;
	QPointF temp;
	temp = start; start =end; end =temp;
		
	//painter->setBrush(color);
	float scale=0.5;
	QPointF start1;
	start1= (end-start)*scale + start;
	

    QLineF line(start, end);

	float ds = sqrt(line.dx()*line.dx() + line.dy()*line.dy());  
	QPointF Dis = QPointF(6*line.dx()/ds, 6*line.dy()/ds);
	//start=start-Dis;


	float size=h;//(1+4*(width+height)/graphReferenceSize)*(1-scale)*15;	
	qreal arrowSize(size);

	static const double Pi = 3.14159265358979323846264338327950288419717; 
    static double TwoPi = 2.0 * Pi;

	double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0)
		angle = TwoPi - angle;

    QPointF sourceArrowP1 = start1 + QPointF(sin(angle - Pi / 2.5) * arrowSize,
                                                  cos(angle - Pi / 2.5) * arrowSize);                                                  
    QPointF sourceArrowP2 = start1 + QPointF(sin(angle - Pi + Pi / 2.5) * arrowSize,
                                                  cos(angle - Pi + Pi / 2.5) * arrowSize);  

	QPointF dis1 = (sourceArrowP1 + sourceArrowP2)/2;	
    dis1=start1-dis1;
	sourceArrowP1 = sourceArrowP1 + dis1;
	sourceArrowP2 = sourceArrowP2 + dis1;

	size=(1+4*(width+height)/graphReferenceSize)*0.5*3.1;	
	
	vector<QPointF> points = getCurveLine(start, sourceArrowP1, end);

	float exWidth=HBWIDTH*_scale*2;
	float itv=size/(points.size()-1)*_scale;
	float as1, as2;
	int count=points.size()-1;
	for(vector<QPointF>::iterator it = points.begin(); it != points.end(); it++)
	{
		as1 = count*itv, as2 = (count-1)*itv;
	    QPointF start = *it;		
		vector<QPointF>::iterator it1=it;
		it1++;
		if(it1==points.end())
			break;
		QPointF end = *it1;
		getATrapezoid(start, end, as1+exWidth, as2+exWidth, QuadPoints);		
		count--;		
		//painter->drawLine(start, end);
		//break;
	}
	return QuadPoints;
}

QVector<QPointF> ItemBase::getArrow(QPointF start, QPointF end, float width, float height, float _scale)
{//single arrow edge
	QPointF temp;
	temp = start; start =end; end =temp;
	QLineF line(start, end);

	float ds = sqrt(line.dx()*line.dx() + line.dy()*line.dy());  
	QPointF Dis = QPointF(6*line.dx()/ds, 6*line.dy()/ds);
	
	float size=1+4*(width+height)/graphReferenceSize*_scale;	
	qreal arrowSize(size*1.5);

	static const double Pi = 3.14159265358979323846264338327950288419717; 
    static double TwoPi = 2.0 * Pi;

    //painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    //painter->drawLine(line);

	double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0)
		angle = TwoPi - angle;

    QPointF sourceArrowP1 = start + QPointF(sin(angle - Pi / 2.5) * arrowSize,
                                                  cos(angle - Pi / 2.5) * arrowSize);                                                  
    QPointF sourceArrowP2 = start + QPointF(sin(angle - Pi + Pi / 2.5) * arrowSize,
                                                  cos(angle - Pi + Pi / 2.5) * arrowSize);   
   
	QPointF dis = (sourceArrowP1 + sourceArrowP2)/2;
    dis=start-dis;
	sourceArrowP1 = sourceArrowP1 + dis;
	sourceArrowP2 = sourceArrowP2 + dis;
	
	QVector<QPointF> points;	         
	points.append(QPoint(line.p2().x(),line.p2().y()));
	points.append(QPoint(sourceArrowP1.x(), sourceArrowP1.y()));
	points.append(QPoint(sourceArrowP2.x(), sourceArrowP2.y()));
	points.append(QPoint(line.p2().x(),line.p2().y()));
	//QRegion arrow(points);
	
	//arrow = arrow -	clipRegion;

	//QPainterPath path;
 	//path.addRegion( arrow); 	

	return points;
 	//painter->fillPath( path, QBrush( color ) );
	//painter->restore(); 	
}

QVector<QPointF> ItemBase::getArrow_H(QPointF start, QPointF end, float width, float height, float _scale)
{//single arrow edge
	QPointF temp;
	temp = start; start =end; end =temp;
	QLineF line(start, end);

	float ds = sqrt(line.dx()*line.dx() + line.dy()*line.dy());  
	QPointF Dis = QPointF(6*line.dx()/ds, 6*line.dy()/ds);
	
	float exWidth=HBWIDTH*_scale*2;
	float size=(1+4*(width+height)/graphReferenceSize)*1.5 + exWidth;	
	float sd = exWidth;//size - (1+4*(width+height)/graphReferenceSize)*1.5;
	qreal arrowSize(size);
	qreal arrowSize2(sd);

	static const double Pi = 3.14159265358979323846264338327950288419717; 
    static double TwoPi = 2.0 * Pi;

    double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0)
		angle = TwoPi - angle;

    QPointF sourceArrowP1 = start + QPointF(sin(angle - Pi / 2.5) * arrowSize,
                                                  cos(angle - Pi / 2.5) * arrowSize);                                                  
    QPointF sourceArrowP2 = start + QPointF(sin(angle - Pi + Pi / 2.5) * arrowSize,
                                                  cos(angle - Pi + Pi / 2.5) * arrowSize);   
   
	QPointF dis = (sourceArrowP1 + sourceArrowP2)/2;
    dis=start-dis;
	sourceArrowP1 = sourceArrowP1 + dis;
	sourceArrowP2 = sourceArrowP2 + dis;


    QPointF endArrowP1 = start + QPointF(sin(angle - Pi / 2.5) * arrowSize2,
                                                  cos(angle - Pi / 2.5) * arrowSize2);                                                  
    QPointF endArrowP2 = start + QPointF(sin(angle - Pi + Pi / 2.5) * arrowSize2,
                                                  cos(angle - Pi + Pi / 2.5) * arrowSize2);   
   
	dis = (endArrowP1 + endArrowP2)/2;
    dis=start-dis;
	endArrowP1 = endArrowP1 + dis;
	endArrowP2 = endArrowP2 + dis;

	
	QVector<QPointF> points;	         
	points.append(QPoint(endArrowP2.x(), endArrowP2.y()));
	points.append(QPoint(endArrowP1.x(), endArrowP1.y()));
	points.append(QPoint(sourceArrowP1.x(), sourceArrowP1.y()));
	points.append(QPoint(sourceArrowP2.x(), sourceArrowP2.y()));

	return points;
}


void ItemBase::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	if (event->button() == Qt::LeftButton && m_isResizing) 
	{	
		//m_isMoving = false;
		logResizeTo();
		m_isMoving=false;
		m_isResizing=false;
	//	emit unGroup( this );
	//	logUnGroup();		
	}
	else if( UNGROUP )
	{
        UNGROUP = false;	    
		AFTERUNGROUP = false;	
	    //	emit unGroup( this );
	    //	logUnGroup();
	}
	else if( newPos.x() != _initPos.x() || newPos.y() != _initPos.y() )
	{
		//QPointF p=sceneBoundingRect().center();		
		_initPos = newPos;
		emit itemPosUpdated( this );		
		logMoveTo();
	}

	if(!m_rightButton)
	   m_isMoving=false;

	m_rightButton = false;	
	//AFTERUNGROUP = false;	
}

void ItemBase::keyReleaseEvent(QKeyEvent *event)
{


}


void ItemBase::keyPressEvent(QKeyEvent *event)
{
	switch (event->key()) {
	case Qt::Key_Control:
		break;
	//case Qt::Key_Delete:
	//	deleteSelectedItems(scene());
	//	break;
	//case Qt::Key_Plus:
	//	break;
	//case Qt::Key_Minus:
	//	shrinkSelectedItems(scene());
	//	break;
	default:
		QGraphicsItem::keyPressEvent(event);
		break;
	}
}


void ItemBase::wheelEvent(QGraphicsSceneWheelEvent *event)
{
	prepareGeometryChange();
	if(getType()==TEXTBUBBLE2 || getType()==CALLOUT	|| getType()==NOTE)
		return;
    if(!onBoarder( event->pos() ))
	{
		float rate = exp(-event->delta() / graphReferenceSize);
		if( (m_size_width*rate > MIN_ITEM_SIZE && m_size_width*rate > MIN_ITEM_SIZE) || rate >1.0 )
		{
			resizeItem( m_size_width*rate, m_size_height*rate );
			logResizeTo();
		}
	}	
}


bool ItemBase::inBox(QPointF cur, int width1, int height1, QPointF tar, int width2, int height2 )
{
	int half_cur_x = this->half( width1 );
	int half_tar_x = this->half( width2 );
	int half_cur_y = this->half( height1 );
	int half_tar_y = this->half( height2 );
	int pointX = 0;
	int pointY = 0;
	//			X boundary
	if(between(cur.x()-half_cur_x, tar.x()-half_tar_x, tar.x()+half_tar_x))
	{
		pointX += 1;
	}
	if(between(cur.x()+half_cur_x, tar.x()-half_tar_x, tar.x()+half_tar_x))
	{
		pointX += 1;
	}
	//			Y boundary
	if(between(cur.y()+half_cur_y, tar.y()-half_tar_y, tar.y()+half_tar_y))
	{
		pointY += 1;
	}
	if(between(cur.y()-half_cur_y, tar.y()-half_tar_y, tar.y()+half_tar_y))
	{
		pointY += 1;
	}

	//			determine
	if( pointX > 0 && pointY > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool ItemBase::inBox(QPointF cur, int cur_size, QPointF tar, int tar_size)
{
	int half_cur = this->half( cur_size );
	int half_tar = this->half( tar_size );
	int pointX = 0;
	int pointY = 0;
	//			X boundary
	if(between(cur.x()-half_cur, tar.x()-half_tar, tar.x()+half_tar))
	{
		pointX += 1;
	}
	if(between(cur.x()+half_cur, tar.x()-half_tar, tar.x()+half_tar))
	{
		pointX += 1;
	}
	//			Y boundary
	if(between(cur.y()+half_cur, tar.y()-half_tar, tar.y()+half_tar))
	{
		pointY += 1;
	}
	if(between(cur.y()-half_cur, tar.y()-half_tar, tar.y()+half_tar))
	{
		pointY += 1;
	}
	//			determine
	if( pointX > 0 && pointY > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool ItemBase::outBox(QPointF cur, int width1, int height1, QPointF tar, int width2, int height2 )
{//cur tar are center point of the rects
	int half_cur = this->half( width1 ) *1.0 + 5;
	int half_tar = this->half( width2 ) *1.0 + 5;
	int half_cur2 = this->half( height1 ) *1.0 + 5;
	int half_tar2 = this->half( height2 ) *1.0 + 5;

	/*int half_cur = this->half( width1 ) *1.0;
	int half_tar = this->half( width2 ) *1.0;
	int half_cur2 = this->half( height1 ) *1.0;
	int half_tar2 = this->half( height2 ) *1.0;*/

	int pointX = 0;
	int pointY = 0;

	if(cur.x()-half_cur >= tar.x()+half_tar)
	{
		pointX += 1;
	}
	if(cur.x()+half_cur <= tar.x()-half_tar)
	{
		pointX += 1;
	}
	//			Y boundary
	if(cur.y()-half_cur2 >= tar.y()+half_tar2)
	{
		pointY += 1;
	}
	if(cur.y()+half_cur2 <= tar.y()-half_tar2)
	{
		pointY += 1;
	}

	//			determine
	if( pointX == 0 && pointY == 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool ItemBase::outBox(QPointF cur, int cur_size, QPointF tar, int tar_size)
{
	int half_cur = this->half( cur_size );
	int half_tar = this->half( tar_size );
	int pointX = 0;
	int pointY = 0;

	if(cur.x()-half_cur > tar.x()+half_tar)
	{
		pointX += 1;
	}
	if(cur.x()+half_cur < tar.x()-half_tar)
	{
		pointX += 1;
	}
	//			Y boundary
	if(cur.y()-half_cur > tar.y()+half_tar)
	{
		pointY += 1;
	}
	if(cur.y()+half_cur < tar.y()-half_tar)
	{
		pointY += 1;
	}

	//			determine
	if( pointX == 0 && pointY == 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

int ItemBase::isInResizeArea(const QPointF &pos)
{
	bool result0  = pos.x()>-this->m_size_width/2-2 && pos.x()< (-this->m_size_width/2+11);
	bool result1 = pos.y() >-this->m_size_height/2-2 && pos.y()< (-this->m_size_height/2+11);
	
	bool result2  = pos.x()> - this->m_size_width/2-2 && pos.x()< (- this->m_size_width/2+11);
	bool result3 = pos.y()< this->m_size_height/2+2 && pos.y()> (this->m_size_height/2-11);
	
	bool result4  = pos.x()<this->m_size_width/2 +2  && pos.x()> (this->m_size_width/2-11);
	bool result5 = pos.y()<this->m_size_height/2 +2 && pos.y()> (this->m_size_height/2-11);
	
	bool result6  = pos.x()<this->m_size_width/2 +2 && pos.x()> (this->m_size_width/2-11);
	bool result7 = pos.y()> - this->m_size_height/2 -2 && pos.y()< (-this->m_size_height/2+11);

	if(result0 && result1) return 1;
	if(result2 && result3) return 2;
	if(result4 && result5) return 3;
	if(result6 && result7 && _TYPE!=CALLOUT) return 4;
	return (0);
}

int ItemBase::whichCorner(const QPointF &pos)
{
	bool result0  = pos.x() < 0;
	bool result1 =  pos.y() < 0;
	
	bool result2  = pos.x() < 0;
	bool result3 =  pos.y()> 0;
	
	bool result4  = pos.x()> 0;
	bool result5 =  pos.y()> 0;
	
	bool result6  =  pos.x()> 0;
	bool result7 = pos.y()< 0;

	if(result0 && result1) return 1;
	if(result2 && result3) return 2;
	if(result4 && result5) return 3;
	if(result6 && result7) return 4;
	return (0);
}

bool ItemBase::advance()
{
	QPointF itempos = this->pos();

	if (newPos == pos() || this->_TYPE == -1 ) //
	{
		return false;
	}
	setPos(newPos);
	return true;
}

bool ItemBase::collision(ItemBase *node, QPointF pos)
{
	int tar_width = node->Width();
	int tar_height = node->Height();
	int cur_width = this->Width();
	int cur_height = this->Height();
	QPointF tar = node->pos();
	QPointF cur = pos;

	int ntype = node->getType();

	if( ntype == -1 || ntype == COLOR || this->_TYPE == COLOR )
	{
		return false;
	}
	//if( ((cur.x() < (tar.x()+(int)((float)tar_size/2.0 + 0.5)) )&&(cur.x() > tar.x() - (int)((float)tar_size/2.0 + 0.5)))&&((cur.y() < tar.y() + (int)((float)tar_size/2.0 + 0.5))&&(cur.y()>tar.y()-(int)((float)tar_size/2.0 + 0.5))) )
	//if( inBox(tar, tar_size, cur, cur_size) )
	if( inBox( tar, tar_width, tar_height, cur, cur_width, cur_height ) ) 
	{
		return true;
	}
	//if( ((tar.x()<cur.x() + (int)((float)cur_size/2.0 + 0.5))&&(tar.x() > cur.x() - (int)((float)cur_size/2.0 + 0.5)))&&((tar.y() < cur.y() + (int)((float)cur_size/2.0 + 0.5))&&(tar.y()>cur.y()-(int)((float)cur_size/2.0 + 0.5))) )
	//if( inBox(cur, cur_size, tar, tar_size) )
	if( inBox( cur, cur_width, cur_height, tar, tar_width, tar_height ) )
	{
		return true;
	}
	return false;
}


void ItemBase::encode(float x,float y,int &code,float XL,float XR,float YB,float YT)
{  int c=0;
   if(x<XL)c=c|1;
   else if(x>XR)c=c|2;
   if(y<YB)c=c|4;
   else if(y>YT)c=c|8;
   code=c;
}

int ItemBase::CSLineClip(float &X1,float &Y1,float &X2,float &Y2,float XL,float XR,float YB,float YT)
//return 0:  no intersection
//return 1:  1 intersection in *X1, *Y1
//return 2:  1 intersection in *X2, *Y2
//else:  2 intersections
{
   float x1,y1,x2,y2,x,y;
   int code1,code2,code;
   x1=X1; y1=Y1; x2=X2; y2=Y2;
   int flag=0;
   //code1=0;
   encode(x1,y1,code1,XL,XR,YB,YT);
   encode(x2,y2,code2,XL,XR,YB,YT);
   while(code1||code2)
   {
	   if(code1&code2) return(0);
       code=code1;
       if(code1==0)code=code2;
       if(1&code)
	   {x=XL;
       y=y1+(y2-y1)*(XL-x1)/(x2-x1);
	   }
       else if(2&code)
	   {x=XR;
       y=y1+(y2-y1)*(XR-x1)/(x2-x1);
	   }
       else if(4&code)
	   {y=YB;
        x=x1+(x2-x1)*(YB-y1)/(y2-y1);
	   }
       else if(8&code)
	   {y=YT;
        x=x1+(x2-x1)*(YT-y1)/(y2-y1);
	   }
       if(code==code1)
	   { X1=x1=x; Y1=y1=y; encode(x,y,code1,XL,XR,YB,YT); flag = flag==0?1 : 3; }
       else  
	   { X2=x2=x; Y2=y2=y; encode(x,y,code2,XL,XR,YB,YT); flag = flag==0?2 : 3; }
   }
   return(flag);     
}


bool ItemBase::collision(ItemBase *node)
{
	int tar_width = node->Width();
	int tar_height = node->Height();
	int cur_width = this->Width();
	int cur_height = this->Height();
	QPointF tar = node->pos();
	QPointF cur = this->pos();

	int ntype = node->getType();

	if( ntype == -1 || ntype == COLOR || this->_TYPE == COLOR )
	{
		return false;
	}
	//if( ((cur.x() < (tar.x()+(int)((float)tar_size/2.0 + 0.5)) )&&(cur.x() > tar.x() - (int)((float)tar_size/2.0 + 0.5)))&&((cur.y() < tar.y() + (int)((float)tar_size/2.0 + 0.5))&&(cur.y()>tar.y()-(int)((float)tar_size/2.0 + 0.5))) )
	//if( inBox(tar, tar_size, cur, cur_size) )
	if( inBox( tar, tar_width, tar_height, cur, cur_width, cur_height ) ) 
	{
		return true;
	}
	//if( ((tar.x()<cur.x() + (int)((float)cur_size/2.0 + 0.5))&&(tar.x() > cur.x() - (int)((float)cur_size/2.0 + 0.5)))&&((tar.y() < cur.y() + (int)((float)cur_size/2.0 + 0.5))&&(tar.y()>cur.y()-(int)((float)cur_size/2.0 + 0.5))) )
	//if( inBox(cur, cur_size, tar, tar_size) )
	if( inBox( cur, cur_width, cur_height, tar, tar_width, tar_height ) )
	{
		return true;
	}
	return false;
}

bool ItemBase::needtoTighten(ItemBase* node)
{
	int tar_size = node->Width();
	int cur_size = this->Width();
	int tar_size2 = node->Height();
	int cur_size2 = this->Height();
	QPointF tar = node->pos();
	QPointF cur = this->pos();

	int ntype = node->getType();

	if( ntype == -1 || ntype == COLOR || this->_TYPE == COLOR )
	{
		return false;
	}

	//if (outBox(tar, tar_size, cur, cur_size))
	if( outBox( tar, tar_size, tar_size2, cur, cur_size, cur_size2 ) )
	{
		return true;
	}

	return false;
}

bool ItemBase::adjustPosLineUp(QList<ItemBase*> groupMember)
{
	/*newPos = _targetPos;	
	this->setPos( _targetPos );
	return false;*/

	QLineF line(pos(), _targetPos);
	bool flag=false;
	qreal xvel, yvel;
	
	xvel = yvel = 0.0;

	qreal dx = line.dx();
	qreal dy = line.dy();

	double l = 2.0 * sqrt(dx * dx + dy * dy);
	float scale = 1;
	
	float stepSizeX=(dx * 10.0) / l * scale;
	float stepSizeY=(dy * 10.0) / l * scale;

	
	if(_synchronizedSpeed)
	   stepSizeX=_moveStepSize.x();
	   stepSizeY=_moveStepSize.y();

	if (l > 0) 
	{
		xvel += stepSizeX;
		yvel += stepSizeY;
	}

	if ( qAbs(xvel) < 0.1 && qAbs(yvel) < 0.1 )
	{
		xvel = yvel = 0.0; flag=true;
	}
	else if ( xvel > dx || yvel > dy )
	{
		xvel = dx;
		yvel = dy;
		flag=true;
		newPos = pos() + QPointF( xvel, yvel );
	}
	else 
	{  //test do it need to be dispersed in next step
	 
	   //next step
		flag = false;
		newPos = pos() + QPointF( xvel, yvel );	
		this->setPos( newPos );
		for (int i = 0; i < groupMember.size(); i ++)
		{
			ItemBase* node = groupMember[i];
			if (!node || node == this)
			{
				continue;
			}
			if (!this->needtoTighten(node))
			{
				flag = false;
				break;
			}
			else flag=true;
		}
	}
	return flag;
	/*QLineF line(pos(), _targetPos);
	bool flag=false;
	qreal xvel, yvel;
	
	xvel = yvel = 0.0;

	qreal dx = line.dx();
	qreal dy = line.dy();

	double l = 2.0 * sqrt(dx * dx + dy * dy);
	float scale = 1;
	
	float stepSizeX=(dx * 60.0) / l * scale;
	float stepSizeY=(dy * 60.0) / l * scale;

	
	if(_synchronizedSpeed)
	{
		stepSizeX=_moveStepSize.x();
	    stepSizeY=_moveStepSize.y();
	}

	if (l > 0) 
	{
		xvel += stepSizeX;
		yvel += stepSizeY;
	}

	if ( qAbs(xvel) < 0.1 && qAbs(yvel) < 0.1 )
	{
		xvel = yvel = 0.0; 
		this->setPos( _targetPos );
		flag=false;
	}
	else if ( qAbs(xvel) > qAbs(dx) || qAbs(yvel) > qAbs(dy) )
	{
		xvel = dx;
		yvel = dy;
		flag=true;
		newPos = pos() + QPointF( xvel, yvel );
	}
	else 
	{  //test do it need to be dispersed in next step
	 
	   //next step
		flag = false;
		newPos = pos() + QPointF( xvel, yvel );	
		this->setPos( newPos );
		for (int i = 0; i < groupMember.size(); i ++)
		{
			ItemBase* node = groupMember[i];
			if (!node || node == this)
			{
				continue;
			}
			if (!this->needtoTighten(node))
			{
				this->setPos( _targetPos );
				flag = false;
				break;
			}
			else flag=true;
		}
	}
	return flag;*/
}

bool ItemBase::tighten(QList<ItemBase*> groupMember)
{
	if (this->_TYPE == -1)
	{
		return false;
	}

	qreal xvel = 0;
	qreal yvel = 0;
	ItemBase* node;

	
	bool tightenNFlag = false;

	for (int i = 0; i < groupMember.size(); i ++)
	{
		node = groupMember[i];
		if (!node || node == this)
		{
			continue;
		}

		if (!this->needtoTighten(node))
		{
			tightenNFlag = true;
			break;
		}
	}
	if (tightenNFlag == true)
	{
        if(!isbridgeBroken(groupMember))
			return false;
		else
		{
		    tightenNFlag=tightenNFlag;
		}
	}

	for(int i = 0; i < groupMember.size(); i ++)
	{
		node = groupMember[i];
		if (!node || node == this)
		{
			continue;
		}
		QLineF line(QPointF(0, 0), mapFromItem(node, 0, 0));
		qreal dx = line.dx();
		qreal dy = line.dy();
		double l = 2.0 * sqrt(dx * dx + dy * dy);
		if (l > 0) 
		{
			xvel += (dx * 10.0) / l * 2;
			yvel += (dy * 10.0) / l * 2;
		}
	}

	if (qAbs(xvel) < 0.1 && qAbs(yvel) < 0.1)
	{
		xvel = yvel = 0.0;
		return true;
	}

	bool flag=false;
	newPos = pos() + QPointF(xvel, yvel);	
	this->setPos( newPos );
	//next step		
	for (int i = 0; i < groupMember.size(); i ++)
	{
			node = groupMember[i];
			if (!node || node == this)
			{
				continue;
			}
			if (!this->needtoTighten(node))
			{
				flag = true;				
				break;
			}						
	}

	/*int tar_size = node->Width();
	int cur_size = this->Width();
	int tar_size2 = node->Height();
	int cur_size2 = this->Height();
	QPointF tar = node->pos();
	QPointF cur = this->pos();

	int ntype = node->getType();

	if( ntype == -1 || ntype == COLOR || this->_TYPE == COLOR )
	{
		return false;
	}

	//if (outBox(tar, tar_size, cur, cur_size))
	if( outBox( tar, tar_size, tar_size2, cur, cur_size, cur_size2 ) )*/
	
	return flag;
}

bool ItemBase::isbridgeBroken(QList<ItemBase*> groupMember)
{	
	ItemBase* node;
	set<ItemBase *> unionSet;
	unionSet.insert(this);	
	int size;
	do
	{
		size=unionSet.size();
		for (int i = 0; i < groupMember.size(); i ++)
		{
			node=groupMember[i];
		
			if(unionSet.find(node)==unionSet.end())
			{
				for(set<ItemBase *>::iterator it=unionSet.begin(); it!=unionSet.end(); it++ )
				{
					int tar_size = node->Width();
					int cur_size = (*it)->Width();
					int tar_size2 = node->Height();
					int cur_size2 = (*it)->Height();
					QPointF tar = node->pos();
					QPointF cur = (*it)->pos();
					//int ntype = (*it)->getType();
					//if( ntype != -1 && ntype != COLOR && node->_TYPE != COLOR )
					{
					
					   if( !outBox( tar, tar_size, tar_size2, cur, cur_size, cur_size2 ) )
					   {
							unionSet.insert(node);	
							break;
					   }
					}
				}
			}		
		}
	}
	while(unionSet.size()>size);
	if(unionSet.size()!=groupMember.size())
		return true;
	else return false;
}



bool ItemBase::disperse(QList<ItemBase*> groupMember)
{
	if (this->_TYPE == -1)
	{
		return false;
	}

	qreal xvel = 0;
	qreal yvel = 0;
	ItemBase* node;

	bool tightenNFlag = false;
	bool flag=false;
	for (int i = 0; i < groupMember.size(); i ++)
	{
		node = groupMember[i];
		if (!node || node == this)
		{
			continue;
		}

		if (!this->collision(node))
		{
			continue;			
		}

		flag=true;

		QLineF line(mapFromItem(node, 0, 0), QPointF(0, 0));
		qreal dx = line.dx();
		qreal dy = line.dy();
		double l = 2.0 * sqrt(dx * dx + dy * dy);
		if (l > 0) 
		{
			//xvel += (dx * 500.0) / l ; //keqin comment out //decide bubble moving speed
			//yvel += (dy * 500.0) / l ; //keqin comment out 
			xvel += (dx * 10.0) / l ;
			yvel += (dy * 10.0) / l ;			
		}
	}
	
	    flag = false;	
		newPos = pos() + QPointF(xvel, yvel);	
		this->setPos( newPos );
		//next step		
		for (int i = 0; i < groupMember.size(); i ++)
		{
			node = groupMember[i];
			if (!node || node == this)
			{
				continue;
			}
			if (!this->needtoTighten(node))
			{
				flag = false;				
				break;
			}			
			else flag=true;
		}
	
	return flag;
}

void ItemBase::calculateForces()
{
	if( this->_TYPE == -1 )
	{
		return;
	}
	qreal xvel = 0;
	qreal yvel = 0;
	QList< QGraphicsItem * > list = this->scene()->items();
	foreach (QGraphicsItem *item, this->scene()->items()) 
	{
		ItemBase* node = dynamic_cast<ItemBase *>(item);
		if (!node || node == this)
		{
			continue;
		}
		// the distance between an item and this.
		if( !this->collision( node ))
		{
			continue;
		}
		QLineF line(mapFromItem(node, 0, 0), QPointF(0, 0));
		qreal dx = line.dx();
		qreal dy = line.dy();
		double l = 2.0 * (dx * dx + dy * dy);
		if (l > 0) 
		{
			//xvel += dx * 0.2f;
			//yvel += dy * 0.2f;
			xvel += (dx * 300.0) / l * 5;
			yvel += (dy * 300.0) / l * 5;
		}
	}

	if (qAbs(xvel) < 0.1 && qAbs(yvel) < 0.1)
		xvel = yvel = 0;

	newPos = pos() + QPointF(xvel, yvel);
	return;
}

QPointF ItemBase::dockItemDragged(QPointF Pos)
{
	if( this->_TYPE == -1 )
	{
		return QPointF(-1000,-1000);
	}
	qreal xvel = 0;
	qreal yvel = 0;
	QList< QGraphicsItem * > list = this->scene()->items();
	foreach (QGraphicsItem *item, this->scene()->items()) 
	{
		ItemBase* node = dynamic_cast<ItemBase *>(item);
		if (!node || node == this)
		{
			continue;
		}
		// the distance between an item and this.
		if( !this->collision( node, Pos ))
		{
			continue;
		}
		QLineF line(mapFromItem(node, 0, 0), QPointF(0, 0));
		qreal dx = line.dx();
		qreal dy = line.dy();
		double l = 2.0 * (dx * dx + dy * dy);
		if (l > 0) 
		{
			//xvel += dx * 0.2f;
			//yvel += dy * 0.2f;
			xvel += (dx * 300.0) / l * 5;
			yvel += (dy * 300.0) / l * 5;
		}
	}

	if (qAbs(xvel) < 0.1 && qAbs(yvel) < 0.1)
		xvel = yvel = 0;

	return (pos() + QPointF(xvel, yvel));	
}
//============================================================================//
//                                    QtBox                                   //
//============================================================================//

QtBox::QtBox(int size, int x, int y) : ItemBase(size, x, y), m_texture(0)
{
	for (int i = 0; i < 8; ++i) {
		m_vertices[i].setX(i & 1 ? 0.5f : -0.5f);
		m_vertices[i].setY(i & 2 ? 0.5f : -0.5f);
		m_vertices[i].setZ(i & 4 ? 0.5f : -0.5f);
	}
	for (int i = 0; i < 4; ++i) {
		m_texCoords[i].setX(i & 1 ? 1.0f : 0.0f);
		m_texCoords[i].setY(i & 2 ? 1.0f : 0.0f);
	}
	m_normals[0] = QVector3D(-1.0f, 0.0f, 0.0f);
	m_normals[1] = QVector3D(1.0f, 0.0f, 0.0f);
	m_normals[2] = QVector3D(0.0f, -1.0f, 0.0f);
	m_normals[3] = QVector3D(0.0f, 1.0f, 0.0f);
	m_normals[4] = QVector3D(0.0f, 0.0f, -1.0f);
	m_normals[5] = QVector3D(0.0f, 0.0f, 1.0f);
}

QtBox::~QtBox()
{
	if (m_texture)
		delete m_texture;
}

//ItemBase *QtBox::createNew(int size, int x, int y)
//{
//    return new QtBox(size, x, y);
//}

void QtBox::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	QRectF rect = boundingRect().translated(pos());
	float width = float(painter->device()->width());
	float height = float(painter->device()->height());

	float left = 2.0f * float(rect.left()) / width - 1.0f;
	float right = 2.0f * float(rect.right()) / width - 1.0f;
	float top = 1.0f - 2.0f * float(rect.top()) / height;
	float bottom = 1.0f - 2.0f * float(rect.bottom()) / height;
	float moveToRectMatrix[] = {
		0.5f * (right - left), 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f * (bottom - top), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f * (right + left), 0.5f * (bottom + top), 0.0f, 1.0f
	};

	painter->beginNativePainting();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadMatrixf(moveToRectMatrix);
	gluPerspective(60.0, 1.0, 0.01, 10.0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	//glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	//glEnable(GL_LIGHTING);//keqin
	//glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE);

	if(m_texture == 0)
		m_texture = new GLTexture2D(":/res/boxes/qt-logo.jpg", 64, 64);
	m_texture->bind();
	glEnable(GL_TEXTURE_2D);

	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	float lightColour[] = {1.0f, 1.0f, 1.0f, 1.0f};
	float lightDir[] = {0.0f, 0.0f, 1.0f, 0.0f};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColour);
	glLightfv(GL_LIGHT0, GL_POSITION, lightDir);
	glEnable(GL_LIGHT0);

	glTranslatef(0.0f, 0.0f, -1.5f);
	glRotatef(ROTATE_SPEED_X * m_startTime.msecsTo(QTime::currentTime()), 1.0f, 0.0f, 0.0f);
	glRotatef(ROTATE_SPEED_Y * m_startTime.msecsTo(QTime::currentTime()), 0.0f, 1.0f, 0.0f);
	glRotatef(ROTATE_SPEED_Z * m_startTime.msecsTo(QTime::currentTime()), 0.0f, 0.0f, 1.0f);
	int dt = m_startTime.msecsTo(QTime::currentTime());
	if (dt < 500)
		glScalef(dt / 500.0f, dt / 500.0f, dt / 500.0f);

	for (int dir = 0; dir < 3; ++dir) {
		glColor4f(1.0f, 1.0f, 1.0f, 1.0);

		glBegin(GL_TRIANGLE_STRIP);
		glNormal3fv(reinterpret_cast<float *>(&m_normals[2 * dir + 0]));
		for (int i = 0; i < 2; ++i) {
			for (int j = 0; j < 2; ++j) {
				glTexCoord2fv(reinterpret_cast<float *>(&m_texCoords[(j << 1) | i]));
				glVertex3fv(reinterpret_cast<float *>(&m_vertices[(i << ((dir + 2) % 3)) | (j << ((dir + 1) % 3))]));
			}
		}
		glEnd();

		glBegin(GL_TRIANGLE_STRIP);
		glNormal3fv(reinterpret_cast<float *>(&m_normals[2 * dir + 1]));
		for (int i = 0; i < 2; ++i) {
			for (int j = 0; j < 2; ++j) {
				glTexCoord2fv(reinterpret_cast<float *>(&m_texCoords[(j << 1) | i]));
				glVertex3fv(reinterpret_cast<float *>(&m_vertices[(1 << dir) | (i << ((dir + 1) % 3)) | (j << ((dir + 2) % 3))]));
			}
		}
		glEnd();
	}
	m_texture->unbind();

	//glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHT0);
	glDisable(GL_NORMALIZE);

	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	painter->endNativePainting();
	
	ItemBase::paint(painter, option, widget);
}

//============================================================================//
//                                 CircleItem                                 //
//============================================================================//

CircleItem::CircleItem(int size, int x, int y) : ItemBase(size, x, y)
{
	m_color = QColor::fromHsv(rand() % 360, 255, 255);
}

void CircleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	int dt = m_startTime.msecsTo(QTime::currentTime());

	int m_size = (m_size_width<m_size_height) ? m_size_width : m_size_height;

	qreal r0 = 0.5 * m_size * (1.0 - exp(-0.001 * ((dt + 3800) % 4000)));
	qreal r1 = 0.5 * m_size * (1.0 - exp(-0.001 * ((dt + 0) % 4000)));
	qreal r2 = 0.5 * m_size * (1.0 - exp(-0.001 * ((dt + 1800) % 4000)));
	qreal r3 = 0.5 * m_size * (1.0 - exp(-0.001 * ((dt + 2000) % 4000)));

	if (r0 > r1)
		r0 = 0.0;
	if (r2 > r3)
		r2 = 0.0;

	QPainterPath path;
	path.moveTo(r1, 0.0);
	path.arcTo(-r1, -r1, 2 * r1, 2 * r1, 0.0, 360.0);
	path.lineTo(r0, 0.0);
	path.arcTo(-r0, -r0, 2 * r0, 2 * r0, 0.0, -360.0);
	path.closeSubpath();
	path.moveTo(r3, 0.0);
	path.arcTo(-r3, -r3, 2 * r3, 2 * r3, 0.0, 360.0);
	path.lineTo(r0, 0.0);
	path.arcTo(-r2, -r2, 2 * r2, 2 * r2, 0.0, -360.0);
	path.closeSubpath();
	painter->setRenderHint(QPainter::Antialiasing, true);
	painter->setBrush(QBrush(m_color));
	painter->setPen(Qt::NoPen);
	painter->drawPath(path);
	painter->setBrush(Qt::NoBrush);
	painter->setPen(Qt::SolidLine);
	painter->setRenderHint(QPainter::Antialiasing, false);

	ItemBase::paint(painter, option, widget);
}



//ItemBase *CircleItem::createNew(int size, int x, int y)
//{
//    return new CircleItem(size, x, y);
//}

//============================================================================//
//                                 SquareItem                                 //
//============================================================================//

SquareItem::SquareItem(int size, int x, int y) : ItemBase(size, x, y)
{
	m_image = QPixmap(":/res/boxes/square.jpg");
}

void SquareItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	int dt = m_startTime.msecsTo(QTime::currentTime());
	QTransform oldTransform = painter->worldTransform();
	int dtMod = dt % 2000;
	qreal amp = 0.002 * (dtMod < 1000 ? dtMod : 2000 - dtMod) - 1.0;

	qreal scale = 0.6 + 0.2 * amp * amp;
	painter->setWorldTransform(QTransform().rotate(15.0 * amp).scale(scale, scale), true);

	painter->drawPixmap(-m_size_width / 2, -m_size_height / 2, m_size_width, m_size_height, m_image);

	painter->setWorldTransform(oldTransform, false);
	ItemBase::paint(painter, option, widget);
}
//
//ItemBase *SquareItem::createNew(int size, int x, int y)
//{
//    return new SquareItem(size, x, y);
//}


void ItemBase::saveAsXmlNode( QDomDocument &doc, QDomElement &node )
{
 	QDomElement pos = doc.createElement( tr("Position") );
 	QDomElement size = doc.createElement( tr("Size") );
	QDomElement boarderColor = doc.createElement( tr("BoarderColor") );
	QDomElement highColor = doc.createElement( tr("HighlightColor") );
 	
 	QDomText text;
 	QString str, tmpstr;
 
 	str = "(";
 	tmpstr.setNum( this->pos().x() );
 	str += tmpstr;
 	str += ", ";
 	tmpstr.setNum( this->pos().y() );
 	str += tmpstr;
 	str += ")";
 	text = doc.createTextNode( str );
 	pos.appendChild( text );
	node.appendChild( pos);

	str = "(";
	tmpstr.setNum( m_size_width );
	str += tmpstr;
	str += ",";
	tmpstr.setNum( m_size_height );
	str += tmpstr;
	str += ")";
 	text = doc.createTextNode( str );
 	size.appendChild( text );
 	node.appendChild( size );

	str = "(";
	tmpstr.setNum( this->_colorBoarder.a );
	str += tmpstr;
	str += ", ";
	tmpstr.setNum( this->_colorBoarder.b );
	str += tmpstr;
	str += ", ";
	tmpstr.setNum( this->_colorBoarder.c );
	str += tmpstr;
	str += ")";
	text = doc.createTextNode( str );
	boarderColor.appendChild( text );
	node.appendChild( boarderColor );

	str = "(";
	tmpstr.setNum( this->_hiColor.a );
	str += tmpstr;
	str += ", ";
	tmpstr.setNum( this->_hiColor.b );
	str += tmpstr;
	str += ", ";
	tmpstr.setNum( this->_hiColor.c );
	str += tmpstr;
	str += ")";
	text = doc.createTextNode( str );
	highColor.appendChild( text );
	node.appendChild( highColor );
}

void ItemBase::recoverFromXmlNode( QDomElement node )
{
	QDomNodeList paraNode;
	QStringList tmpStr;
	QString str;
	int a, b, c;

	paraNode = node.elementsByTagName( tr("BoarderColor") );
	tmpStr = paraNode.at(0).toElement().text().split(",");
	str = tmpStr.at(0);
	str.remove(0, 1);
	str = str.simplified();
	a = str.toInt();
	str = tmpStr.at(1);
	str = str.simplified();
	b = str.toInt();
	str = tmpStr.at(2);
	str = str.simplified();
	c = str.toInt();

	_colorBoarder.a = a;
	_colorBoarder.b = b;
	_colorBoarder.c = c;

	paraNode = node.elementsByTagName( tr("HighlightColor") );
	tmpStr = paraNode.at(0).toElement().text().split(",");
	str = tmpStr.at(0);
	str.remove(0, 1);
	str = str.simplified();
	a = str.toInt();
	str = tmpStr.at(1);
	str = str.simplified();
	b = str.toInt();
	str = tmpStr.at(2);
	str = str.simplified();
	c = str.toInt();

	_hiColor.a = a;
	_hiColor.b = b;
	_hiColor.c = c;
}

QString ItemBase::BubbleIdentifier()
{
	QString tmp;
	tmp.setNum( bIndex );
	tmp.append( "[" );

	switch ( _TYPE )
	{
	case BAT:
		tmp.append( "Geometry" );
		break;
	case PC:
		tmp.append( "Parallel coordinate");
		break;
	case COMPLOT:
		tmp.append( "Compare Plot" );
		break;
	case MATLAB:
		tmp.append( "Matlab" );
		break;
	case WEB:
		tmp.append( "Web" );
		break;
	case WAKE:
		tmp.append( "Trailing edge" );
		break;
	case VTK:
		tmp.append( "VTK" );
		break;
	case VIDEO:
		tmp.append( "Video" );
		break;
	case IMAGE:
		tmp.append( "Image" );
		break;
	case PEN:
		tmp.append( "Pen" );
		break;
	case VOLUM:
		tmp.append( "Volum" );
		break;
	case CON:
		tmp.append( "Control" );
		break;
	case NOTE:
		tmp.append( "Note" );
		break;
	case DATAPLOT:
		tmp.append( "MatData Plot" );
		break;
	case BARCHART:
		tmp.append( "Bar Chart" );
		break;
	case LINECHART:
		tmp.append( "Line Chart" );
		break;
	case SCATTER:
		tmp.append( "Scatter Plot" );
		break;
	}
	tmp.append( "]" );

	return tmp;
}

QString ItemBase::headofLogMsg()
{
	QTime time = QTime::currentTime();
	QString timestr = time.toString( "h:m:s ap" );
	QString logtext;
	logtext.append( timestr );
	logtext.append( "     " );

	return logtext;
}

QString ItemBase::tailofLogMsg()
{
	QString logtext;

	logtext.append(" (the bubble ");
	logtext.append( BubbleIdentifier() );
	logtext.append(")");
	logtext.append( "\n" );

	return logtext;
}

void ItemBase::logOpen()
{
	QString logtext;
	logtext.append( headofLogMsg() );
	logtext.append( "Open Bubble " );
	logtext.append( BubbleIdentifier() );
	logtext.append( " at ");
	logtext.append( posString() );

	emit logInfor( logtext );
}

void ItemBase::logRemove()
{
	QString logtext;
	
	logtext.append( headofLogMsg() );
	logtext.append( "Remove at ");
	logtext.append( posString() );
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void ItemBase::logMoveTo()
{
	QString logtext;
	logtext.append( headofLogMsg() );
	logtext.append( "Move to ");
	logtext.append( posString() );
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void ItemBase::logResizeTo()
{
	QString logtext;
	logtext.append( headofLogMsg() );
	logtext.append( "Resize to ");
	logtext.append( sizeString() );
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void ItemBase::logUnGroup()
{
	QString logtext;
	logtext.append( headofLogMsg() );
	logtext.append( "Ungroup from the group" );
	logtext.append( tailofLogMsg() );
	emit logInfor( logtext );
}

void ItemBase::readexpression(const char *name, vector<QString> &quantityName, vector<vector<QString>> &geneInfo, vector<vector<float>> &quantity, vector<vector<float>> &minmax)
{//return maximum step
	char ch[100];
	FILE *fp = fopen(name,"r"); 	
	char e;		
	int pid, sid, pid2=-1, sid2=-1;
	int count=0;
	vector<float> min,max;
	
	//set<int> empty;
	//_1pathway.push_back(empty);

	for(int i=0; i< 2; i++)
	{
	    min.push_back(100000), max.push_back(-100000);
	}

    // read head
	vector<string> buffer;
	      string phrase; 
		  e='x';
		  do
		  {  e=fgetc(fp);			 
			  if(e=='\t'||e=='\n')
			 {
				 buffer.push_back(phrase);
				 phrase.clear();
			 }
			 else if(e>-1) phrase.append(1,e);
		  }while(e!='\n'&&e>-1); 

		 //if(e<0||(e=='\n'&&buffer[0].empty()))
		//	  break;

		  //process phrase
		  //check pid2 
		 quantityName.push_back(buffer[2].c_str());
		 quantityName.push_back(buffer[3].c_str());
		  	 
   
	do
	{
		  //fscanf(fp,"Pathway%d Pathway Step%d\n",&pid, &sid);	
		  vector<string> buffer;
	      string phrase; 
		  e='x';
		  do
		  {  e=fgetc(fp);			 
			  if(e=='\t'||e=='\n')
			 {
				 buffer.push_back(phrase);
				 phrase.clear();
			 }
			 else if(e>-1) phrase.append(1,e);
		  }while(e!='\n'&&e>-1); 

		  if(e<0||(e=='\n'&&buffer[0].empty()))
			  break;


          vector<float> temp;
		  temp.push_back(atof(buffer[2].c_str()));
		  temp.push_back(atof(buffer[3].c_str()));
		  quantity.push_back(temp);

		  for(int i=0; i< 2; i++)
		  {
			  if(max[i]<temp[i]) max[i]=temp[i];
		      if(min[i]>temp[i]) min[i]=temp[i];
		  }
	  

		  vector<QString> stemp;
		  stemp.push_back(buffer[0].c_str());
		  stemp.push_back(buffer[1].c_str());
		  geneInfo.push_back(stemp);
		
	 }while(true);

	 for(int i=0; i< 2; i++)
	 {
		vector<float> temp;
        temp.push_back(min[i]); temp.push_back(max[i]);
	    minmax.push_back(temp);
	 }
	 
	 fclose(fp);	 
}
