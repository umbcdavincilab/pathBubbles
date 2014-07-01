#include "PenCamber.h"

PenCamber::PenCamber(Camber *camber, int marker, int frame, QPointF pos )
: _marker( marker )
, _frame( frame )
, _offset( 0, 0 )
, _currentName( 0 )
, _currentProjection( -1 )
, ATTACHFRAME( true )
{
	this->_pos = pos;
	_camber = camber;
	initLines();
	switchName( marker );
	switchProjection();
	return;
}

void PenCamber::paint(QPainter *painter)
{
	PenItem::paint( painter );
	painter->save();
	//***************************************************
	drawName( painter );
	painter->setPen( QPen( QBrush( QColor( 0, 0, 255 )), 2 ));
	painter->drawLines( this->_curve );
	//***************************************************
	painter->restore();
	return;
}

void PenCamber::drawName( QPainter* painter )
{
	painter->drawText( this->boundingRect().topLeft(), _QName + _QProjection + frameInString );
	return;
}

void PenCamber::setFrame( int frame )
{
	if( !ATTACHFRAME )
	{
		return;
	}
	this->_frame = frame;
	frameInString.setNum( _frame );
	frameInString = " Frame:" + frameInString;

	initLines();
	return;
}

void PenCamber::setPos( QPoint pos )
{
	QPointF offset = QPointF( pos.x(), pos.y() ) - this->_bounding.topLeft();
	this->_offset = _offset + offset;
	for( int i = 0; i < this->_curve.size(); i++ )
	{
		this->_curve[i].translate( offset );
	}
	PenItem::setPos( pos );
	return;
}

void PenCamber::initLines()
{
	this->_curve.clear();
	//*******************************************
	int size = this->_camber->getNumOfPointAtFrame( _currentName, this->_frame);
	double *point_data;
	point_data = new double[size*3];
	this->_camber->getPointsAtFrame( _frame, _currentName, point_data );
	QPointF firstPos( -point_data[_currentProjection%3]*200, -point_data[(_currentProjection+1)%3]*200 );
	//**********************************************************************
	this->_bounding = QRectF( _pos, _pos );
	for( int i = 0; i < size-1; i++ )
	{
		QPointF left(  -point_data[i*3+_currentProjection%3]*200, -point_data[i*3+(_currentProjection+1)%3]*200 );
		left = left - firstPos + _pos;
		QPointF right( -point_data[(i+1)*3+_currentProjection%3]*200, -point_data[(i+1)*3+(_currentProjection+1)%3]*200 );
		right = right - firstPos + _pos;
		updateBoundingRect( left );
		updateBoundingRect( right );
		this->_curve.append( QLineF( left, right ) );
	}
	return;
}

void PenCamber::switchName( int i )
{
	if( i == -1 )
	{
		this->_currentName = ( _currentName + 1 ) % 3;
	}
	else
	{
		this->_currentName = i;
	}
	//*******************************************************
	switch( _currentName )
	{
	case 0:
		this->_QName = tr("Elbow ");
		break;
	case 1:
		this->_QName = tr("Wrist ");
		break;
	default:
		this->_QName = tr("MCP_3 ");
	}
	initLines();
	return;
}

void PenCamber::switchProjection()
{
 	_currentProjection = ( _currentProjection + 1 ) % 3;
	switch( _currentProjection )
	{
	case 0:
		this->_QProjection = tr("Projection on XY");
		break;
	case 1:
		this->_QProjection = tr("Projection on YZ");
		break;
	default:
		this->_QProjection = tr("Projection on XZ");
	}
	initLines();
	return;
}

void PenCamber::switchFrame()
{
	if( ATTACHFRAME )
	{
		ATTACHFRAME = false;
	}
	else
	{
		ATTACHFRAME = true;
	}
	return;
}

void PenCamber::updateBoundingRect( QPointF pos )
{
	if( this->_bounding.x() == 0 && this->_bounding.y() == 0 && this->_bounding.width() == 0 && this->_bounding.height() == 0 )
	{
		this->_bounding.setX( pos.x() );
		this->_bounding.setY( pos.y() );
		this->_bounding.setWidth( 0 );
		this->_bounding.setHeight( 0 );
	}
	if( this->_bounding.contains( pos ) )
	{
		return;
	}
	if( _bounding.left() > pos.x() )
	{
		_bounding.setLeft( pos.x() );
	}
	if( _bounding.right() < pos.x() )
	{
		_bounding.setRight( pos.x() );
	}
	if( _bounding.top() > pos.y() )
	{
		_bounding.setTop( pos.y() );
	}
	if( _bounding.bottom() < pos.y() )
	{
		_bounding.setBottom( pos.y() );
	}
	return;
}