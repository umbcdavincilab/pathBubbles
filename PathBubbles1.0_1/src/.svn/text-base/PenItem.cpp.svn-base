#include "PenItem.h"

PenItem::PenItem()
: _curState( false )
{
	return;
}

PenItem::~PenItem()
{

}

QRectF PenItem::boundingRect()
{
	return this->_bounding;
}

QPoint PenItem::initPos()
{
	return this->_pos;
}

void PenItem::paint( QPainter *painter )
{
	painter->save();
	//******************************************************
	painter->setPen( Qt::NoPen );
	if( this->_curState )
	{
		painter->setBrush( QBrush( QColor( 166, 216, 84, 100 ) ) );
	}
	else
	{
		painter->setBrush( QBrush( QColor( 166, 216, 84, 20 ) ) );
	}
	painter->drawRect( this->_bounding );
	//******************************************************
	painter->restore();
	return;
}

QString PenItem::getText()
{
	return "";
}

void PenItem::setText( QString text )
{
	return;
}

void PenItem::setPos(QPoint pos)
{
	QPointF offset = QPointF( pos.x(), pos.y() ) - this->_bounding.topLeft();
	this->_bounding.translate( offset );
	this->_pos = pos;
	return;
}

QPoint PenItem::getPos()
{
	return this->_pos;
}

bool PenItem::mouseOver( QPointF pos )
{
	this->_curState = _bounding.contains( pos );
	return _curState;
}

int PenItem::getType()
{
	return this->_TYPE;
}

QString PenItem::toQString()
{
	return QObject::tr( "Undefined toString()" );
}

void PenItem::click()
{
	return;
}

void PenItem::release()
{
	return;
}