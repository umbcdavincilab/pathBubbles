#include "NavigationBlock.h"


NavigationBlock::NavigationBlock( QSizeF size )
: BarBase( size, 0, 0 )
, _rect( 0, 0, 0, 0 )
, _ONVIEWPORT( false )
, _ONITEM( -1 )
, _HIGHLIGHTBUBBLE( -1 )
, _currentMarks( -1 )
, _rectOffset( 1.0 )
, itemMoved(false)
{
	return;
}

QString NavigationBlock::tailofLogMsg()
{
	QString logtext;

	logtext.append( " (Navigation Bubble)\n" );

	return logtext;
}

void NavigationBlock::logSaveCurView( int index )
{
	QString logtext, str;

	str.setNum( index );
	logtext.append( headofLogMsg() );
	logtext.append( "Save the Current View as BookMark " );
	logtext.append( str );
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void NavigationBlock::logDeleteBookMark( int index )
{
	QString logtext, str;

	str.setNum( index );
	logtext.append( headofLogMsg() );
	logtext.append( "Delete the " );
	logtext.append( str );
	logtext.append( "th bookmark ");
	logtext.append( this->_marks[index].name );
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void NavigationBlock::logZoomIn()
{
	QString logtext;

	logtext.append( headofLogMsg() );
	logtext.append( "Zoom in the Scene" );
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void NavigationBlock::logZoomOut()
{
	QString logtext;

	logtext.append( headofLogMsg() );
	logtext.append( "Zoom out the Scene" );
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void NavigationBlock::logMoveViewTo( int x, int y )
{
	QString logtext, str, tmp;
	
	logtext.append( headofLogMsg() );
	logtext.append( "Move the current view to " );

	str = "( ";
	tmp.setNum( x );
	str.append( tmp );
	str.append( ", " );
	tmp.setNum( y );
	str.append( tmp );
	str.append( " )");

	logtext.append( str );
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void NavigationBlock::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	painter->save();

	int width = this->_rect.width();
	int height = this->_rect.height();
	QLinearGradient linearGrad(QPointF( width/2, 0 ), QPointF( width/2, height));
	linearGrad.setColorAt(1, QColor( 10, 10, 10, 100) );
	linearGrad.setColorAt(0, QColor( 40, 40, 40, 200 ) );
	painter->setBrush( QBrush( linearGrad ) );
	painter->drawRect( this->boundingRect() );
	
	painter->setBrush( Qt::NoBrush );
	painter->setPen( QPen( QBrush( QColor( 240, 240, 240, 200) ), 2 ) );
	painter->drawRoundedRect( this->_rect, 10, 10 );
	//*********************************************************
	this->drawAllBubbles( painter );
	//*********************************************************
	this->drawBookmarks( painter ); 
	//*********************************************************
	painter->restore();
	//*********************************************************
	
	return;
}

void NavigationBlock::drawBookmarks( QPainter* painter )
{
	painter->save();
	//********************************************************
	painter->setBrush( Qt::NoBrush );
	for( int i = 0; i < this->_marks.size(); i++ )
	{
		if( _currentMarks == i )
		{
			painter->setPen( QPen( QBrush( QColor( 255, 255, 0, 100) ), 4 ) );
		}
		else
		{
			painter->setPen( QPen( QBrush( this->_marks[i].color ), 4 ) );
		}
		painter->drawRect( this->_marks[i].rect );
	}
	//********************************************************
	painter->setPen( QColor( 255,255,255,255) );
	for( int i = 0; i < this->_marks.size(); i++ )
	{
		painter->drawText( this->_marks[i].rect.bottomLeft(), this->_marks[i].name );
	}
	//********************************************************
	painter->restore();
	return;
}

void NavigationBlock::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
	QMenu menu;
	if( this->_rect.contains( QPoint( event->pos().x(), event->pos().y() ) ) )//	on current viewport
	{
		QAction *saveAction = menu.addAction("Name the current view");
		QAction *selectedAction = menu.exec( event->screenPos() );
		if( saveAction == selectedAction )
		{
			saveCurrentViewport();
		}
	}
	else if( onBookmark( event->pos() ) > -1 )//	on a book marker
	{
		QAction *deleteAction = menu.addAction("Delete");
		QAction *selectedAction = menu.exec( event->screenPos() );
		if( selectedAction == deleteAction )
		{
			deleteViewport( onBookmark( event->pos() ) );
		}
	}
	else//	on current viewport
	{
		//QAction *addAction = menu.addAction("Add bookmark");
		//QAction *addAction = menu.exec( contextMenuEvent->screenPos() );
		//if( selectedAction == addAction )
		//{
		//	saveCurrentViewport();
		//}
	}
	return;
}

void NavigationBlock::saveCurrentViewport()
{
	BookMark mark;
	mark.rect = this->_rect;
	mark.color = TDHelper::getBoarderColor( 60 );
	this->_marks.append( mark );
	int index = this->_marks.size();

	logSaveCurView( index );

	return;
}

int NavigationBlock::onBookmark( QPointF pos )
{
	for( int i = 0; i < this->_marks.size(); i++ )
	{
		if( this->_marks[i].rect.contains( pos ) )
		{
			return i;
		}
	}
	return -1;
}
void NavigationBlock::deleteViewport( int index )
{
	logDeleteBookMark( index );
	this->_marks.removeAt( index );

	return;
}

void NavigationBlock::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
	_lastPos = event->pos();
	if( this->_rect.contains( QPoint( _lastPos.x(), _lastPos.y() ) ) )
	{
		_ONVIEWPORT = true;
	}
	//_ONVIEWPORT = true;
	_ONITEM = hitBubbleRect( _lastPos );
	BarBase::mousePressEvent( event );
	return;
}

int NavigationBlock::hitBubbleRect(QPointF pos)
{
	for( int i = 0; i < this->_bubbleRects.size(); i++ )
	{
		if( this->_bubbleRects[i].contains( pos ) )
		{
			return i;
		}
	}
	return -1;
}

void NavigationBlock::shiftViewportByOffset(QPoint offset )
{
	int offX = offset.x();
	int offY = offset.y();
	if( _rect.x() + offX < 0 )
	{
		offX = 0;
	}
	else if( _rect.x() + _rect.width() + offX > this->boundingRect().width() )
	{
		offX = 0;
	}
	if( _rect.y() + offY < 0 )
	{
		offY = 0;
	}
	else if( _rect.y() + _rect.height() + offY > this->boundingRect().height() )
	{
		offY = 0;
	}
	_rect.translate( offX, offY );
	emit shiftViewport( QPoint( offX * SCALE, offY * SCALE ) );
	return;
}

void NavigationBlock::mouseMoveEvent( QGraphicsSceneMouseEvent *event )
{
	if( !this->boundingRect().contains( QPoint( _lastPos.x(), _lastPos.y() ) ) )
	{
	    return;
	}
	if( _ONITEM > -1 && _ONVIEWPORT )
	{
		QPoint offset = QPoint( event->pos().x() - _lastPos.x(), event->pos().y() - _lastPos.y() );
		int offX = offset.x();
		int offY = offset.y();

		QRectF rect=QRectF(_bubbleRects[_ONITEM].x() + offX, _bubbleRects[_ONITEM].y() + offY, _bubbleRects[_ONITEM].width(), _bubbleRects[_ONITEM].height());

		if(this->boundingRect().contains(rect))
		{
			_lastPos = event->pos();
			if( _bubbleRects[_ONITEM].x() + offX < 0 )
			{
				offX = 0;
			}
			else if( _bubbleRects[_ONITEM].x() + _bubbleRects[_ONITEM].width() + offX > this->scene()->width() )
			{
				offX = 0;
			}
			if( _bubbleRects[_ONITEM].y() + offY < 0 )
			{
				offY = 0;
			}
			else if( _bubbleRects[_ONITEM].y() + _bubbleRects[_ONITEM].height() + offY > this->scene()->height() )
			{
				offY = 0;
			}
			//_bubbles[_ONITEM]->mySetPos( QPointF(_bubbles[_ONITEM]->pos().x() + offX * SCALE, _bubbles[_ONITEM]->pos().y() + offY * SCALE) );	//only move one
			_bubbles[_ONITEM]->movePosFromNavigationBar( QPointF(_bubbles[_ONITEM]->pos().x() + offX * SCALE, _bubbles[_ONITEM]->pos().y() + offY * SCALE) );//also move other bubble in the group			
			//emit _bubbles[_ONITEM]->itemPosMoved( this, event->pos() - event->lastPos() );
			itemMoved=true;
			_lastPos = event->pos();
		}
	}
	else if( _ONVIEWPORT )
	{
		QPoint offset = QPoint( event->pos().x() - _lastPos.x(), event->pos().y() - _lastPos.y() );
		shiftViewportByOffset( offset );
		_lastPos = event->pos();
	} 
	return;
}

void NavigationBlock::mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
{
	_ONVIEWPORT = false;
	int size=_bubbles.size();
	if(_manager!=NULL && itemMoved && _bubbles.size()>_ONITEM && _ONITEM>=0)
	{
		_manager->itemPosUpdated( _bubbles[_ONITEM]);
		itemMoved = false;
	}
	_ONITEM = -1;
	/*BarBase::mouseReleaseEvent( event );
	logMoveViewTo( _rect.x(), _rect.y() );*/
	return;
}

void NavigationBlock::hoverMoveEvent( QGraphicsSceneHoverEvent *event )
{
	if( this->onBookmark( event->pos() ) > -1 )
	{
		_currentMarks = onBookmark( event->pos() );
	}
	else
	{
		_currentMarks = -1;
	}
	if( hitBubbleRect( event->pos() ) > -1 )
	{
		_HIGHLIGHTBUBBLE = hitBubbleRect( event->pos() );
	}
	else
	{
		_HIGHLIGHTBUBBLE = -1;
	}
	BarBase::hoverMoveEvent( event );
	return;
}

void NavigationBlock::keyPressEvent( QKeyEvent *event )
{
	if( _currentMarks > -1 )
	{
		this->handleKeyboard( event );
	}
	BarBase::keyPressEvent( event );
	return;
}

void NavigationBlock::wheelEvent( QGraphicsSceneWheelEvent *event )
{
	/*if( this->_rect.contains( QPoint( event->pos().x(), event->pos().y() ) ) )
	{
		if( event->delta() > 0 )
		{
			this->_rectOffset += 0.05;
			//emit updateCoverage( 1.05 );			
			emit updateCoverage( 1 );
			logZoomIn();
			//emit itemResized( this);
		}
		else
		{
			this->_rectOffset -= 0.05;
			//emit updateCoverage( 0.95 );
			emit updateCoverage( -1 );
			logZoomOut();
			//emit itemResized( this);
		}
		
	}
	BarBase::wheelEvent( event );*/
	return;
}

void NavigationBlock::updateSize( QSizeF size )
{
	this->_rect.setWidth( this->boundingRect().height()* ((float) this->scene()->width()/ this->scene()->height()));
	//this->_rect.setHeight( this->scene()->height()/20 );
	this->_rect.setHeight( this->boundingRect().height() ); 
	BarBase::updateSize( size );
}

void NavigationBlock::drawAllBubbles(QPainter *painter)
{
	painter->save();
	//**********************************************
	addBubbleRects();
	for( int i = 0; i < this->_bubbles.size(); i++ )
	{
		if( _HIGHLIGHTBUBBLE != i )
		{
			painter->setPen( Qt::NoPen );
		}
		else
		{
			painter->setPen( QPen( QBrush( QColor( 255, 255, 0, 100 ) ), 4 ) );
		}
		myColor3 color = this->_bubbles[i]->getColor();
		painter->setBrush( QColor( color.a, color.b, color.c, 250 ) );
		if(this->_bubbles[i]->getType()!=CALLOUT)		
		   painter->drawRect( this->_bubbleRects[i] );
	}

	for( int i = 0; i < this->_bubbles.size(); i++ )
	{
		if( _HIGHLIGHTBUBBLE != i )
		{
			painter->setPen( Qt::NoPen );
		}
		else
		{
			painter->setPen( QPen( QBrush( QColor( 255, 255, 0, 100 ) ), 4 ) );
		}
		myColor3 color = this->_bubbles[i]->getColor();
		painter->setBrush( QColor( color.a, color.b, color.c, 250 ) );		
		if(this->_bubbles[i]->getType()==CALLOUT)
		 painter->drawRect( this->_bubbleRects[i] );
	}

	//**********************************************
	painter->restore();
	return;
}

void NavigationBlock::addBubbleRects()
{
	this->_bubbles.clear();
	this->_bubbleRects.clear();
	QPoint offset;
	foreach( QGraphicsItem* base, this->scene()->items() )
	{
		ItemBase *item = dynamic_cast< ItemBase* >( base );
		if( !item || !item->isVisible() )
		{
			continue;
		}
		if( item->getType() == COLOR || item->getType() == -1)
		{
			continue;
		}
		int itemSize = item->Width() / SCALE * _rectOffset;
		int itemSize2 = item->Height() / SCALE * _rectOffset;
		//int X = item->getInitPos().x() * _rectOffset / SCALE + _rect.x();
		//int Y = item->getInitPos().y() * _rectOffset / SCALE + _rect.y();
		int X = item->pos().x() / SCALE + _rect.x();
		int Y = item->pos().y() / SCALE + _rect.y();
		//int X = item->pos().x() / SCALE * _rectOffset + _rect.x();
		//int Y = item->pos().y() / SCALE * _rectOffset + _rect.y();
		this->_bubbles.append( item );
		this->_bubbleRects.append( QRectF( X - itemSize/2, Y - itemSize2/2, itemSize, itemSize2 ) );
	}
	return;
}

QColor NavigationBlock::getQColorByIndex(int index)
{
	switch( index )
	{
	case BAT:
		return QColor( 0, 0, 200, 150);
	case PC:
		return QColor( 0, 200, 0, 150);
	case COMPLOT:
		return QColor( 200, 0, 0, 150);
	case MATLAB:
		return QColor( 0, 200, 200, 150);
	case WEB:
		return QColor( 200, 0, 200, 150);
	case WAKE:
		return QColor( 200, 200, 0, 150);
	case 0:
		return QColor( 20, 20, 100, 150);
	case PEN:
		return QColor( 20, 100, 20, 150);
	case VIDEO:
		return QColor( 100, 100, 20, 150 );
	case NOTE:
		return QColor( 100, 20, 100, 150 );
	default:
		return QColor( 20, 20, 20, 150);
	}
}

void NavigationBlock::handleKeyboard(QKeyEvent *event)
{
	if( event->key() == 16777219 )
	{
		this->_marks[_currentMarks].name.remove( _marks[_currentMarks].name.length() - 1, 1 );
	}
	else
	{
		this->_marks[_currentMarks].name.append( event->text() );
	}
	return;
}

void NavigationBlock::saveAsXmlNode( QDomDocument &doc, QDomElement &node )
{
	QString str, tmpStr;
	QDomText text;

	QDomElement curViewRect = doc.createElement( tr("CurrentViewRect") );
	str = "(";
	tmpStr.setNum( _rect.x() );
	str += tmpStr;
	str += ", ";
	tmpStr.setNum( _rect.y() );
	str += tmpStr;
	str += ", ";
	tmpStr.setNum( _rect.width() );
	str += tmpStr;
	str += ", ";
	tmpStr.setNum( _rect.height() );
	str += tmpStr;
	str += ")";
	text = doc.createTextNode( str );
	curViewRect.appendChild( text );
	node.appendChild( curViewRect );

	QDomElement bookmarks = doc.createElement( tr("AllBookMark") );
	for ( int i = 0; i < this->_marks.size(); i ++ )
	{
		QDomElement eachMark = doc.createElement( tr("BookMark") );
		
		QDomAttr markID = doc.createAttribute( tr("MarkID") );
		str.setNum(i+1);
		markID.setValue( str );
		eachMark.setAttributeNode( markID );

		QDomElement markName = doc.createElement( tr("Name") );
		text = doc.createTextNode( _marks[i].name );
		markName.appendChild( text );
		eachMark.appendChild( markName );

		QDomElement markRect = doc.createElement( tr("Rect") );
		str = "(";
		tmpStr.setNum( _marks[i].rect.x() );
		str += tmpStr;
		str += ", ";
		tmpStr.setNum( _marks[i].rect.y() );
		str += tmpStr;
		str += ", ";
		tmpStr.setNum( _marks[i].rect.width() );
		str += tmpStr;
		str += ", ";
		tmpStr.setNum( _marks[i].rect.height() );
		str += tmpStr;
		str += ")";
		text = doc.createTextNode( str );
		markRect.appendChild( text );
		eachMark.appendChild( markRect );

		QDomElement markColor = doc.createElement( tr("Color") );
		str = "(";
		tmpStr.setNum( _marks[i].color.red() );
		str += tmpStr;
		str += ", ";
		tmpStr.setNum( _marks[i].color.green() );
		str += tmpStr;
		str += ", ";
		tmpStr.setNum( _marks[i].color.blue() );
		str += tmpStr;
		str += ", ";
		tmpStr.setNum( _marks[i].color.alpha() );
		str += tmpStr;
		str += ")";
		text = doc.createTextNode( str );
		markColor.appendChild( text );
		eachMark.appendChild( markColor );

		bookmarks.appendChild( eachMark );
	}
	node.appendChild( bookmarks );
}

void NavigationBlock::recoverFromXmlNode( QDomElement node )
{
	QDomNodeList paraNode;
	QStringList tmpStr;
	QString str;
	int rectx, recty, rectwidth, rectheight;

	paraNode = node.elementsByTagName( tr("CurrentViewRect") );
	tmpStr = paraNode.at(0).toElement().text().split(",");
	str = tmpStr.at(0);
	str.remove( 0, 1 );
	str = str.simplified();
	rectx = str.toInt();

	str = tmpStr.at(1);
	str = str.simplified();
	recty = str.toInt();

	str = tmpStr.at(2);
	str = str.simplified();
	rectwidth = str.toInt();

	str = tmpStr.at(3);
	str.chop(1);
	str = str.simplified();
	rectheight = str.toInt();

	_rect.setRect( rectx, recty, rectwidth, rectheight );
}