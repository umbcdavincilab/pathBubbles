#include "CamberMenu.h"

CamberMenu::CamberMenu( int index, PenCamber* camber )
: _index( index )
, _curFont( "times", 12 )
, _metrics( _curFont )
{
	this->_camber = camber;
	_indexInString.setNum( index );
	_indexInString = " " + _indexInString;
	//
	QRectF size = this->_metrics.boundingRect( _indexInString );
	this->_bounding.setWidth( size.width() + 4 );
	_bounding.setHeight( size.height() );
	//
	return;
}

CamberMenu::~CamberMenu()
{
	delete _camber;
	return;
}

void CamberMenu::paint(QPainter *painter)
{
	PenItem::paint( painter );
	this->_camber->paint( painter );
	painter->save();
	//**********************************************
	painter->setPen( QColor( 0, 0, 0) );
	painter->setFont( _curFont );
	painter->drawText( _bounding.bottomLeft() - QPoint( 0, this->_metrics.descent() ), _indexInString );
	//**********************************************
	painter->restore();
	return;
}

void CamberMenu::setPos(QPoint pos)
{
	PenItem::setPos( pos );
	return;
}

int CamberMenu::getIndex()
{
	return this->_index;
}

void CamberMenu::setFrame(int index)
{
	this->_camber->setFrame( index );
	return;
}

void CamberMenu::switchProjection()
{
	this->_camber->switchProjection();
	return;
}

bool CamberMenu::mouseOver( QPoint pos )
{
	return PenItem::mouseOver( pos );
}

void CamberMenu::switchFrame()
{
	this->_camber->switchFrame();
	return;
}

void CamberMenu::switchMarker()
{
	this->_camber->switchName();
	return;
}