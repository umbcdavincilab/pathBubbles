#include "qtcolorbar.h"

QtColorBar::QtColorBar(int size, int x, int y, LabObject *Lab, LabCut *Cut) : ItemBase( size, x, y )
{
	setFlag(QGraphicsItem::ItemIsMovable, false);
    setFlag(QGraphicsItem::ItemIsSelectable, false);
    setFlag(QGraphicsItem::ItemIsFocusable, false);
	_TYPE = COLOR;
	_lab = Lab;
	_cut = Cut;
	_CANBESHIFTED = false;
	return;
}

void QtColorBar::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	int heightOff = this->scene()->height() - 420;
	myColor3 color;
	for( int i = 0; i<100; i++)
	{
		Lab3D point = this->_cut->get_Position( float((99-i)/100.0) );
		this->_lab->get_Color( point, color );
		painter->setBrush( QColor( color.a, color.b, color.c, 255) );
		painter->setPen( QColor( color.a, color.b, color.c, 255) );
		painter->drawRect( 10, i*2+10 + heightOff, 30, 2 );
	}
	painter->setPen( QColor( 255, 255, 255, 255) );
	painter->drawText( 45, 22 + heightOff, tr("100%") );
	painter->drawText( 45, 210 + heightOff, tr(" 0%") );
	return;
}