#include "BarBase.h"

BarBase::BarBase(QSizeF _size, int x, int y)
: QObject()
, _size( _size )
{
	setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);
    setAcceptHoverEvents(true);
	this->setPos( x, y );
	_lastPos = this->pos();
	return;
}

void BarBase::updateSize( QSizeF size )
{
	this->resizeItem( size );
}

QString BarBase::headofLogMsg()
{
	QTime time = QTime::currentTime();
	QString timestr = time.toString( "h:m:s ap" );
	QString logtext;
	logtext.append( timestr );
	logtext.append( "     " );

	return logtext;
}

QString BarBase::posString()
{
	QString str, tmp;
	str = "(";
	tmp.setNum( this->pos().x() );
	str.append( tmp );
	str.append( ", ");
	tmp.setNum( this->pos().y() );
	str.append( tmp );
	str.append( ")" );
	return str;
}

void BarBase::logInit()
{
	QString logtext;

	logtext.append( headofLogMsg() );
	logtext.append( "Initiation at " );
	logtext.append( posString() );
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void BarBase::logMoveTo()
{
	QString logtext;

	logtext.append( headofLogMsg() );
	logtext.append( "Move to " );
	logtext.append( posString() );
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void BarBase::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	painter->save();
	painter->setPen( Qt::NoPen );
	painter->setBrush( QBrush( QColor( 10, 10, 10, 100) ) );
	painter->drawRect( this->shadow() );
	painter->setBrush( QBrush( QColor( 200, 200, 200, 255) ) );
	painter->drawRect( this->boundingRect() );
	painter->restore();
	return;
}

QRectF BarBase::boundingRect() const
{
	return QRectF( QPointF( 0, 0 ), _size );
}

void BarBase::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
	QGraphicsItem::contextMenuEvent( event );
	return;
}

void BarBase::mouseMoveEvent( QGraphicsSceneMouseEvent *event )
{
	QPointF offset = event->pos() - event->lastPos();
	this->setPos(this->pos().x()+offset.x(), this->pos().y()+offset.y());
	return;
}

void BarBase::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
	QGraphicsItem::hoverMoveEvent( event );
	return;
}

void BarBase::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsItem::mousePressEvent( event );
	_lastPos = this->pos();
	return;
}

void BarBase::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsItem::mouseReleaseEvent( event );
	if ( this->pos().x() != _lastPos.x() || this->pos().y() != _lastPos.y() )
	{
		logMoveTo();
	}
	return;
}

void BarBase::keyPressEvent(QKeyEvent *event)
{
	QGraphicsItem::keyPressEvent( event );
	return;
}

void BarBase::wheelEvent(QGraphicsSceneWheelEvent *event)
{
	QGraphicsItem::wheelEvent( event );
	return;
}

void BarBase::resizeItem(QSizeF size)
{
	this->_size = size;
	return;
}

QSizeF BarBase::getSize()
{
	return this->_size;
}

QRectF BarBase::shadow()
{
	QRectF shadow = this->boundingRect();
	//shadow.setWidth( shadow.width() + 2 );
	//shadow.setHeight( shadow.height() + 2 );
	shadow.translate( 3, 3 );
	return shadow;
}