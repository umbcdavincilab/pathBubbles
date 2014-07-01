#include "mySlider.h"

mySlider::mySlider( QWidget* parent, float begin, float end )
: QSlider(Qt::Horizontal)
, _start( begin )
, _end( end )
{
	return;
}

void mySlider::paintEvent( QPaintEvent *event )
{
	QSlider::paintEvent( event );
	//****************************************************
	QRectF bounding = event->rect();
	QPainter painter( this );
	int width = bounding.width();
	int height = bounding.height()/1.5;
	QRectF highLight( QPointF( width* this->_start, height ), QPointF( width* this->_end, bounding.bottom()) );
	if( min.size() != 0 )
	{
		painter.drawText( highLight.bottomLeft(), min );
		painter.drawText( highLight.bottomRight(), max );
	}
	//*****************************************************
	painter.setBrush( QColor( 20, 20, 250, 100) );
	painter.setPen( Qt::NoPen );
	painter.drawRect( highLight );
	//*****************************************************
	//*****************************************************
	return;
}

void mySlider::setHighlight(float begin, float end, int total, int stFrame, int edFrame )
{
	_start = begin;
	_end = end;
	min.setNum( stFrame );
	max.setNum( edFrame );
	return;
}