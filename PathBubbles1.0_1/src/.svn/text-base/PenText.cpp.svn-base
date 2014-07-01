#include "PenText.h"

PenText::PenText( bool background )
: _curFont( "times", 12 )
, _metrics( _curFont )
, _backGround( background )
{
	this->_TYPE = 2;
	return;
}

void PenText::appendText(QString text)
{
	this->_text.append( text );
	updateBoundingRect();
	return;
}

void PenText::updateBoundingRect()
{
	QRectF size = this->_metrics.boundingRect( _text );
	_bounding.setWidth( size.width() + 8 );
	_bounding.setHeight( size.height() );
	return;
}

void PenText::paint(QPainter *painter)
{
	painter->save();
	//**************************************************
	painter->setPen( QColor( 0, 0, 0) );
	painter->setFont( _curFont );
	painter->drawText( _bounding.bottomLeft() - QPoint( 0, this->_metrics.descent() ), _text );
	if( _backGround )
	{
		//painter->setPen( Qt::NoPen );
		//painter->setBrush( QBrush( QColor( 0, 0, 80, 100 ) ) );
		//painter->drawRect( QRectF(this->_bounding.topLeft()+QPointF(0,-10), this->_bounding.size() ) );
		//QPointF points[3] = 
		//{
		//	QPointF( _bounding.bottomLeft().x(), _bounding.bottomLeft().y() - 10 ),
		//	QPointF( _bounding.bottomLeft().x() + 5, _bounding.bottomLeft().y() ),
		//	QPointF( _bounding.bottomLeft().x() + 10, _bounding.bottomLeft().y() - 10 ),
		//};
		//painter->drawConvexPolygon(points, 3);
	}
	//**************************************************
	painter->restore();
	PenItem::paint( painter );
	return;
}

void PenText::setText( QString text )
{
	this->_text = text;
}

QString PenText::getText()
{
	return this->_text;
}

void PenText::setPos(QPoint pos)
{
	PenItem::setPos( pos );
}

QPoint PenText::getPos()
{
	QPoint pos = PenItem::getPos();
	return pos;
}

bool PenText::removeText()
{
	if( this->_text.length() == 0 )
	{
		return false;
	}
	else
	{
		this->_text.remove( this->_text.length() - 1, 1 );
		updateBoundingRect();
		return true;
	}
}

QString PenText::toQString()
{
	return this->getText().simplified();
}