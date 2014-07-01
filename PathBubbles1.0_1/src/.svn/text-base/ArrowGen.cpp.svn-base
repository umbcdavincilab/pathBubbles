
#include "ArrowGen.h"

ArrowGen::ArrowGen(QSizeF size, OpenGLScene* scene)
: BarBase(size, 10, 200),
  _ONMOVE(false)
{
	setFlag( QGraphicsItem::ItemIsMovable, true);
	setFlag( QGraphicsItem::ItemIsSelectable, true);
	setFlag( QGraphicsItem::ItemIsFocusable, true);

	this->_scene = scene;
	_hoverRect = NONE;
	_currentData = NONE;
	_sphereInfor = NONE;
	_cylinderInfor = NONE;

	initGraphicsItem();
	initDataArea();
	initGItemArea();
}

QString ArrowGen::tailofLogMsg()
{
	QString logtext;

	logtext.append( " (Data mapping Bubble)\n" );

	return logtext;
}

void ArrowGen::logDragTo( QString s1, QString s2 )
{
	QString logtext;
	
	logtext.append( headofLogMsg() );
	logtext.append( "Drag ");
	logtext.append( s1 );
	logtext.append( " to ");
	logtext.append( s2 );
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void ArrowGen::logDragToScene( QString type, QPointF pos )
{
	QString logtext, str, tmp;

	logtext.append( headofLogMsg() );
	logtext.append( "Drag ");
	logtext.append( type );
	logtext.append( " to ");
	
	str = "(";
	tmp.setNum( pos.x() );
	str.append( tmp );
	str.append( ", ");
	tmp.setNum( pos.y() );
	str.append( tmp );
	str.append( ")" );
	logtext.append( str );


	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void ArrowGen::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	//BarBase::paint( painter, option, widget );
	painter->save();

	drawDataArea( painter );
	drawGraphicsArea( painter );
	drawDragRect( painter );
	//painter->setPen( QColor( 0, 0, 0 ) );
	//painter->drawRoundRect( this->boundingRect() );
	painter->restore();
}

void ArrowGen::drawDataArea( QPainter* painter )
{
	painter->save();

	painter->setPen( Qt::NoPen );
	painter->setBrush( QBrush(QColor(20, 20, 20, 180) ) );
	painter->drawRect( this->getDataArea_Shadow() );

	painter->setBrush( QBrush(QColor( 122, 122, 161, 255 ) ) );
	painter->drawRect( this->getDataArea() );

	painter->setPen( Qt::NoPen );
	if (_hoverRect == SPEED)
	{
		painter->setBrush( QBrush(QColor( 30, 30, 30, 130) ) );
		painter->drawRect( this->_areaData_Shadow[0]);
		//
		painter->setBrush( QBrush(QColor( 200, 40, 40, 180 ) ) );
		painter->drawRect( this->_areaData[0]);
	}
	else
	{
		painter->setBrush( QBrush(QColor(74, 113, 98, 180) ) );
		painter->drawRect( this->_areaData[0]);
	}

	painter->setFont( QFont( "times", 12 ) );
	painter->setPen( QColor(255, 255, 255, 200) );
	painter->drawText( this->_areaData[0], Qt::AlignCenter, tr( "Speed" ) );

	painter->setPen( Qt::NoPen );
	if (_hoverRect == MASS)
	{
		painter->setBrush( QBrush(QColor( 30, 30, 30, 130) ) );
		painter->drawRect( this->_areaData_Shadow[1]);
		//
		painter->setBrush( QBrush(QColor( 200, 40, 40, 180 ) ) );
		painter->drawRect( this->_areaData[1]);
	}
	else
	{
		painter->setBrush( QBrush(QColor(74, 113, 98, 180) ) );
		painter->drawRect( this->_areaData[1]);
	}

	painter->setFont( QFont( "times", 12 ) );
	painter->setPen( QColor(255, 255, 255, 200) );
	painter->drawText( this->_areaData[1], Qt::AlignCenter, tr( "Mass" ) );

	painter->restore();
}

void ArrowGen::drawGraphicsArea( QPainter* painter )
{
	QString str;

	painter->save();

	//
	painter->setPen( Qt::NoPen );
	painter->setBrush( QColor( 20, 20, 20, 180 ) );
	painter->drawRect( this->getGraphicsArea_Shadow() );
	//painter->drawRect( this->getArrowArea_Shadow() );
	//

	painter->setPen( Qt::NoPen );
	painter->setBrush( QBrush(QColor( 153, 153, 163, 250) ) );
	painter->drawRect( this->getGraphicsArea() );
	//painter->setPen( Qt::NoPen );
	//painter->setBrush( QBrush(QColor( 160, 160, 160, 200) ) );
	//painter->drawRect( this->getArrowArea() );

	painter->setPen( Qt::NoPen );
	painter->setBrush( QBrush(QColor(140, 120, 100, 130) ) );
	painter->drawImage(this->_areaGItem[0], this->GItem[0], this->_rectsGItem[0]);

	if (_sphereInfor == SPEED)
	{	
		str = tr("speed");
	}
	else if (_sphereInfor == MASS)
	{
		str = tr("mass");
	}
	else
	{
		str = tr("none");
	}
	painter->setFont( QFont( "times", 12 ) );
	painter->setPen( QColor( 0, 0, 0, 255 ) );
	painter->drawText( this->_areaGItem[0], Qt::AlignCenter, str );
	
	painter->setPen( Qt::NoPen );
	painter->setBrush( QBrush(QColor(100, 120, 140, 130) ) );
	painter->drawImage(this->_areaGItem[1], this->GItem[1], this->_rectsGItem[1]);

	if (_cylinderInfor == SPEED)
	{	
		str = tr("speed");
	}
	else if (_cylinderInfor == MASS)
	{
		str = tr("mass");
	}
	else
	{
		str = tr("none");
	}
	painter->setFont( QFont( "times", 12 ) );
	painter->setPen( QColor( 0, 0, 0, 255 ) );
	painter->drawText( this->_areaGItem[1], Qt::AlignCenter, str );

	painter->restore();
}

void ArrowGen::drawDragRect( QPainter* painter )
{
	if (_currentData == NONE)
	{
		return;
	}

	painter->save();

	painter->setPen(Qt::NoPen);
	painter->setBrush( QBrush(QColor(50, 60, 80, 200) ) );
	painter->drawRect( _dragRect );

	painter->restore();
}

QRectF ArrowGen::getDataArea()
{
	return QRectF( QPointF( 0, 0 ), QSize( this->boundingRect().width()/2, this->boundingRect().height() ) );
}

QRectF ArrowGen::getDataArea_Shadow()
{
	return QRectF( QPointF( 0, 0 ), QSize( this->boundingRect().width()/2 + 2, this->boundingRect().height() + 2 ) );
}

QRectF ArrowGen::getGraphicsArea()
{
	return QRectF( QPointF( this->boundingRect().width()/2, 0 ), QSize( this->boundingRect().width()/2, this->boundingRect().height() ) );
}

QRectF ArrowGen::getGraphicsArea_Shadow()
{
	return QRectF( QPointF( this->boundingRect().width()/2, 0 ), QSize( this->boundingRect().width()/2 + 2, this->boundingRect().height() + 2 ) );
}

QRectF ArrowGen::getArrowArea()
{
	return QRectF( QPointF( this->boundingRect().width()/2+5, 5 ), QSize( this->boundingRect().width()/2-10, this->boundingRect().height()-10 ) );
}

QRectF ArrowGen::getArrowArea_Shadow()
{
	return QRectF( QPointF( this->boundingRect().width()/2+5, 5 ), QSize( this->boundingRect().width()/2-10 + 2, this->boundingRect().height()-10 + 2) );
}

bool ArrowGen::inDataArea( QPointF pos )
{
	return this->getDataArea().contains( pos );
}

bool ArrowGen::inGraphicsArea( QPointF pos )
{
	return this->getGraphicsArea().contains( pos );
}

bool ArrowGen::inSpeedArea( QPointF pos )
{
	return this->_areaData[0].contains( pos );
}

bool ArrowGen::inMassArea( QPointF pos )
{
	return this->_areaData[1].contains( pos );
}

bool ArrowGen::inSphereArea( QPointF pos )
{
	return this->_areaGItem[0].contains( pos );
}

bool ArrowGen::inCylinderArea( QPointF pos )
{
	return this->_areaGItem[1].contains( pos );
}

bool ArrowGen::inArrowArea( QPointF pos )
{
	return this->getArrowArea().contains( pos );
}

void ArrowGen::initGraphicsItem()
{
	this->GItem[1] = QImage( tr("icon/cylinder.png") );
	this->GItem[0] = QImage( tr("icon/sphere.png") );

	this->_rectsGItem.append( QRectF( QPointF(0, 0), this->GItem[0].size() ) );
	this->_rectsGItem.append( QRectF( QPointF(0, 0), this->GItem[1].size() ) );
	//
	//this->_rectsGItem.append( QRectF( QPointF(0, 0), QSize( 16, 16 ) ) );
	//this->_rectsGItem.append( QRectF( QPointF(0, 0), QSize( 16, 16 ) ) );
}

void ArrowGen::initDataArea()
{
	this->_areaData.append( QRectF( QPointF(20, 20), QSizeF( 60, 25) ) );
	this->_areaData.append( QRectF( QPointF(20, 55), QSizeF( 60, 25) ) );
	//
	this->_areaData_Shadow.append( QRectF( QPointF(20, 20), QSizeF( 62, 27) ) );
	this->_areaData_Shadow.append( QRectF( QPointF(20, 55), QSizeF( 62, 27) ) );
}

void ArrowGen::initGItemArea()
{
	this->_areaGItem.append( QRectF( QPointF(this->boundingRect().width()/2+30, 10), QSizeF( 40, 40) ) );
	this->_areaGItem.append( QRectF( QPointF(this->boundingRect().width()/2+30, 55), QSizeF( 40, 40) ) );
}

void ArrowGen::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QPointF tmpPos;
	tmpPos = event->pos();

	if (inDataArea( tmpPos ) )
	{
		if ( inSpeedArea( tmpPos ) )
		{
			_currentData = SPEED;
			_dragRect = QRectF( QPointF(0, 0), QSizeF(30, 20) );
		}
		else if ( inMassArea( tmpPos ) )
		{
			_currentData = MASS;
			_dragRect = QRectF( QPointF(0, 0), QSizeF(30, 20) );
		}
		else
		{
			_currentData = NONE;
			_ONMOVE = true;
			BarBase::mousePressEvent( event );
		}
	}
	else if ( inGraphicsArea( tmpPos ) )
	{
		if (inArrowArea( tmpPos ))
		{
			if ( inSphereArea( tmpPos ) )
			{
				_currentData = SPHERE;
				_dragRect = QRectF( QPointF(0, 0), QSizeF(20, 20) );
			}
			else if (inCylinderArea( tmpPos ) )
			{
				_currentData = CYLINDER;
				_dragRect = QRectF( QPointF(0, 0), QSizeF(40, 20) );
			}
			else
			{
				_currentData = ARROW;
				_dragRect = QRectF( QPointF(0, 0), QSizeF(40, 40) );
			}
		}
		else
		{
			_currentData = NONE;
			_ONMOVE = true;
			BarBase::mousePressEvent( event );
		}
	}
}

void ArrowGen::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if (_ONMOVE)
	{
		if (event->scenePos().y() > 50)
		{
			BarBase::mouseMoveEvent( event );
		} 
	}
	else if (_currentData != NONE)
	{
		this->_dragRect.moveTo( event->pos() );
	}
}

void ArrowGen::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	QPointF tmpPos = event->pos();

	if (_currentData == SPEED)
	{
		if ( inCylinderArea( tmpPos ) )
		{
			_cylinderInfor = SPEED;
			logDragTo( "Speed", "Cylinder" );
		}
		else if ( inSphereArea( tmpPos) )
		{
			_sphereInfor = SPEED;
			logDragTo( "Speed", "Sphere" );
		}
	}
	else if (_currentData == MASS)
	{
		if ( inCylinderArea( tmpPos ) )
		{
			_cylinderInfor = MASS;
			logDragTo( "Mass", "Cylinder" );
		}
		else if ( inSphereArea( tmpPos) )
		{
			_sphereInfor = MASS;
			logDragTo( "Mass", "Sphere" );
		}
	}
	else if (_currentData == SPHERE || _currentData == CYLINDER || _currentData == ARROW)
	{
		emit( setWingShapePara( this, event->scenePos() ) );
		if (_currentData == SPHERE)
		{
			logDragToScene( "Sphere", event->scenePos() );
		}
		else if (_currentData == CYLINDER )
		{
			logDragToScene( "Cylinder", event->scenePos() );
		}
		else if ( _currentData == ARROW )
		{
			logDragToScene( "Arrow", event->scenePos() );
		}
	}
	
	_currentData = NONE;
	_ONMOVE = false;
	BarBase::mouseReleaseEvent( event );
}

void ArrowGen::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
	if ( inSpeedArea( event->pos() ) )
	{
		_hoverRect = SPEED;
	}
	else if ( inMassArea( event->pos() ) )
	{
		_hoverRect = MASS;
	}
	else	_hoverRect = NONE;
}

void ArrowGen::contextMenuEvent( QGraphicsSceneContextMenuEvent *event )
{
	QMenu menu;
	QAction *hideAction = menu.addAction("Hide");

	QAction *selectedAction = menu.exec( event->screenPos() );

	if (selectedAction == hideAction)
	{
		this->hide();
	}
}