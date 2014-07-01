#include "qtbox.h"

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
, DELEGATE( false )
, boaderLeft( true )
, boarderRight( true )
, boarderUp( true )
, boarderDown( true )
{
	setFlag(QGraphicsItem::ItemIsMovable, true);
	setFlag(QGraphicsItem::ItemIsSelectable, true);
	setFlag(QGraphicsItem::ItemIsFocusable, true);
	setAcceptHoverEvents(true);
	mySetPos( QPointF(x, y) );
	this->_colorBoarder = TDHelper::getBoarderColor();
	m_startTime = QTime::currentTime();
	
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

QRectF ItemBase::boundingRect() const
{
	return QRectF(-m_size_width / 2 - 20, -m_size_height / 2 - 20, m_size_width + 40, m_size_height + 40);
}

QRect ItemBase::realRect()
{
	return QRect( -m_size_width / 2, -m_size_height / 2, m_size_width, m_size_height );
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
	this->_initPos = this->pos();
	this->setPos( pos );
	this->_targetPos = pos;
	this->newPos = pos;
	return;
}

void ItemBase::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	painter->save();

	painter->setRenderHint(QPainter::Antialiasing, true);
	
	//painter->setCompositionMode( QPainter::CompositionMode_SourceOut );
	if (option->state & QStyle::State_Selected) 
	{
		painter->setPen( QColor( _colorBoarder.a, _colorBoarder.b, _colorBoarder.c, 200 ) );
		drawControlHint( painter );
		painter->drawRoundedRect( realRect(), 20, 20 );
		painter->setBrush( QColor( _colorBoarder.a, _colorBoarder.b, _colorBoarder.c, 30 ) );
		painter->setPen( Qt::NoPen );
		drawHalo( painter );
	}
	else
	{
		painter->setPen( Qt::NoPen );
		if( this->_highLight )
		{
			//painter->setPen( QPen( QBrush( QColor( _hiColor.a, _hiColor.b, _hiColor.c, 30 ) ), 100 ));
			painter->setBrush( QColor( _hiColor.a, _hiColor.b, _hiColor.c, 100 ) );
			//painter->drawRect(boundingRect());
			drawHalo( painter );
		}
		else
		{
			painter->setBrush( QColor( _colorBoarder.a, _colorBoarder.b, _colorBoarder.c, 100 ) );
			drawHalo( painter );
		}
		//			draw a halo
	}
	painter->setRenderHint(QPainter::Antialiasing, false);
	ControlAutoHide();

	painter->restore();
}

QRegion ItemBase::AllRegion()
{
	QRect bounding = this->realRect();
	QRegion reg;
	//
	if ( boaderLeft )
	{
		QRect left( bounding.topLeft() - QPoint( HALOWIDTH, 5 ), bounding.bottomLeft() + QPoint( HALOWIDTH, 5 ) );
		//painter->drawChord( left, 90*16, 180*16 );
		reg += QRegion(left, QRegion::Ellipse);
	}
	else
	{
		QRect left( bounding.topLeft() - QPoint( 2, 0 ), bounding.bottomLeft() );
		//painter->drawRect( left );
		reg += left;
	}
	//
	if ( boarderRight )
	{
		QRect right( bounding.topRight() - QPoint( HALOWIDTH, 5 ), bounding.bottomRight() + QPoint( HALOWIDTH, 5 ) );
		//painter->drawChord( right, 270*16, 180*16 );
		reg += QRegion(right, QRegion::Ellipse);
	}
	else
	{
		QRect right( bounding.topRight() , bounding.bottomRight() + QPoint( 2, 0 ) );
		//painter->drawRect( right );
		reg += right;
	}
	////
	if ( boarderDown )
	{
		QRect down( bounding.bottomLeft() - QPoint( 5, HALOWIDTH ), bounding.bottomRight() + QPoint( 5, HALOWIDTH ) );
		//painter->drawChord( down, 180*16, 180*16 );
		reg += QRegion(down, QRegion::Ellipse);;
	}
	else
	{
		QRect down( bounding.bottomLeft() , bounding.bottomRight() + QPoint( 0, 2 ));
		//painter->drawRect( down );
		reg += down;
	}
	//////
	if ( boarderUp )
	{
		QRect top( bounding.topLeft() - QPoint( 5, HALOWIDTH ), bounding.topRight() + QPoint( 5, HALOWIDTH ) );
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
 	QRegion reg = AllRegion() - roundRect( realRect(), 20 );
 	QPainterPath path;
 	path.addRegion( reg );
 	painter->fillPath( path, painter->brush() );
 	return;
}


void ItemBase::addToGroup( ItemBase* target )
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
	int dis = int( sqrt( pow( m_size_width/2 - pos.x(), 2) + pow( -m_size_height/2 - pos.y(), 2) ) );
	if( dis < 20 )
	{
		return true;
	}
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
	if (!isSelected() && scene()) 
	{
		scene()->clearSelection();
		setSelected(true);
	}

	QMenu menu;
	QAction *delAction = menu.addAction("Delete");

	QAction *selectedAction = menu.exec(event->screenPos());

	if (selectedAction == delAction)
	{
		logRemove();

		deleteSelectedItems(scene());
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

//			high risk
void ItemBase::deleteSelectedItems(QGraphicsScene *scene)
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
			//if( itemBase->getType() != PC && itemBase->getType() != PLOT )
			//{
			//	delete itemBase->_myControl;
			//}
			itemBase->hide();
			//if( itemBase->needRemoveControl() )
			//{
			//	itemBase->removeControl();
			//}
			itemBase->properRemove();
			delete itemBase;
		}
	}
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

void ItemBase::mouseMoveEvent( QGraphicsSceneMouseEvent *event )
{
	if (m_isResizing)
	{
		int dx = int(2.0 * event->pos().x());
		int dy = int(2.0 * event->pos().y());
		//resizeItem( dy );
		resizeItem( dx, dy );
	}
	else
	{
		//QGraphicsItem::mouseMoveEvent(event);
		this->mySetPos( this->pos()+ event->pos() - event->lastPos() );
		emit itemPosMoved( this, event->pos() - event->lastPos() );
	}
}

void ItemBase::resizeItem( int size1, int size2 )
{
	prepareGeometryChange();
	this->m_size_width = size1;
	this->m_size_height = size2;
	//
	if ( m_size_width < MIN_ITEM_SIZE)
	{
		m_size_width = MIN_ITEM_SIZE;
	}
	if (m_size_width > MAX_ITEM_SIZE)
	{
		m_size_width = MAX_ITEM_SIZE;
	}
	if ( m_size_height < MIN_ITEM_SIZE)
	{
		m_size_height = MIN_ITEM_SIZE;
	}
	if (m_size_height > MAX_ITEM_SIZE)
	{
		m_size_height = MAX_ITEM_SIZE;
	}

	_initSize = this->m_size_width;
	_initSize2 = this->m_size_height;

	if (_label)
	{
		this->_label->updateLabel( this->m_size_width, this->m_size_height );
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
	initControlsAnimation( event->pos() );
	if (m_isResizing || (isInResizeArea(event->pos()) && isSelected()))
	{
		setCursor(Qt::SizeFDiagCursor);
	}
	else if( onControl(event->pos()) && isSelected() ) 
	{
		this->setCursor( Qt::WhatsThisCursor );
	}
	else if((onMove(event->pos()) && isSelected()))
	{
		this->setCursor( Qt::SizeAllCursor );
	}
	else
	{
		setCursor(Qt::ArrowCursor);
	}
	QGraphicsItem::hoverMoveEvent(event);
}

void ItemBase::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	static qreal z = 0.0;
	setZValue(z += 1.0);
	if( this->onUngroup( event->pos())  
		|| (this->onBoarder( event->pos()) && event->button() == Qt::RightButton ) )
	{
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

void ItemBase::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	if (event->button() == Qt::LeftButton && m_isResizing) 
	{
		m_isResizing = false;
		logResizeTo();
	}
	else if( UNGROUP )
	{
		UNGROUP = false;
		emit unGroup( this );
		logUnGroup();
	}
	else if( newPos.x() != _initPos.x() || newPos.y() != _initPos.y() )
	{
		emit itemPosUpdated( this );
		logMoveTo();
	}
}

void ItemBase::keyPressEvent(QKeyEvent *event)
{
	switch (event->key()) {
	case Qt::Key_Delete:
		deleteSelectedItems(scene());
		break;
	case Qt::Key_Plus:
		growSelectedItems(scene());
		break;
	case Qt::Key_Minus:
		shrinkSelectedItems(scene());
		break;
	default:
		QGraphicsItem::keyPressEvent(event);
		break;
	}
}

void ItemBase::wheelEvent(QGraphicsSceneWheelEvent *event)
{
	prepareGeometryChange();

	float rate = exp(-event->delta() / 600.0);

	resizeItem( m_size_width*rate, m_size_height*rate );
	
	logResizeTo();
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
{
	int half_cur = this->half( width1 );
	int half_tar = this->half( width2 );
	int half_cur2 = this->half( height1 );
	int half_tar2 = this->half( height2 );
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
	if(cur.y()-half_cur2 > tar.y()+half_tar)
	{
		pointY += 1;
	}
	if(cur.y()+half_cur2 < tar.y()-half_tar)
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

bool ItemBase::isInResizeArea(const QPointF &pos)
{
	bool result  = pos.x()<this->m_size_width/2 && pos.x()> (this->m_size_width/2-9);
	bool result1 = pos.y()<this->m_size_height/2 && pos.y()> (this->m_size_height/2-9);
	return ( result && result1 );
}

bool ItemBase::advance()
{
	QPointF itempos = this->pos();

	if (newPos == pos() || this->_TYPE == -1 )
	{
		return false;
	}
	setPos(newPos);
	return true;
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

void ItemBase::adjustPosLineUp()
{
	QLineF line(pos(), _targetPos);

	qreal xvel, yvel;
	
	xvel = yvel = 0.0;

	qreal dx = line.dx();
	qreal dy = line.dy();

	double l = 2.0 * (dx * dx + dy * dy);

	if (l > 0) 
	{
		xvel += (dx * 1000.0) / l;
		yvel += (dy * 1000.0) / l;
	}

	if ( qAbs(xvel) < 0.1 && qAbs(yvel) < 0.1 )
	{
		xvel = yvel = 0.0;
	}
	else if ( xvel > dx || yvel > dy )
	{
		xvel = dx;
		yvel = dy;
	}

	newPos = pos() + QPointF( xvel, yvel );
	return ;
}

void ItemBase::tighten(QList<ItemBase*> groupMember)
{
	if (this->_TYPE == -1)
	{
		return;
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
	if (tightenNFlag == true)	return;

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
		double l = 2.0 * (dx * dx + dy * dy);
		if (l > 0) 
		{
			//xvel += dx * 0.2f;
			//yvel += dy * 0.2f;
			xvel += (dx * 500.0) / l;
			yvel += (dy * 500.0) / l;
		}
	}

	if (qAbs(xvel) < 0.1 && qAbs(yvel) < 0.1)
	{
		xvel = yvel = 0.0;
	}

	newPos = pos() + QPointF(xvel, yvel);
	return;
}

void ItemBase::disperse(QList<ItemBase*> groupMember)
{
	if (this->_TYPE == -1)
	{
		return;
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

		if (!this->collision(node))
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
			xvel += (dx * 500.0) / l;
			yvel += (dy * 500.0) / l;
		}
	}

	if (qAbs(xvel) < 0.1 && qAbs(yvel) < 0.1)
		xvel = yvel = 0;

	newPos = pos() + QPointF(xvel, yvel);
	return;
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
			xvel += (dx * 300.0) / l;
			yvel += (dy * 300.0) / l;
		}
	}

	if (qAbs(xvel) < 0.1 && qAbs(yvel) < 0.1)
		xvel = yvel = 0;

	newPos = pos() + QPointF(xvel, yvel);
	return;
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
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
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


//
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