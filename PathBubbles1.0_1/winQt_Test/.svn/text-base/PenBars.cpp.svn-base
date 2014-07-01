#include "PenBars.h"

PenBars::PenBars() 
: _max( -999 )
, _min( 999 )
, _font( "times", 6 )
{
	//this->_font = QFont( "times", 8 );
	//this->_metrics = QFontMetrics( this->_font );
	return;
}

void PenBars::acceptData(QList<QString> name, QList<float> data, QList<QColor> color, QString para )
{
	this->_name = name;
	this->_data = data;
	for( int i = 0; i < this->_data.size(); i++ )
	{
		QString dataS;
		dataS.setNum( _data[i] );
		if( dataS.size() > 5 )
		{
			dataS.resize( 5 );
		}
		this->_dataString.append( dataS );
		if( _max < _data[i] )
		{
			_max = _data[i];
		}
		if( _min > _data[i] )
		{
			_min = _data[i];
		}
	}
	this->_color = color;
	//***********************************************
	this->_para = para;
	//***********************************************
	updateBoundingRect();
	return;
}

void PenBars::paint( QPainter* painter )
{
	painter->save();
	//******************************************
	drawFonts( painter );
	drawBars( painter );
	//******************************************
	painter->restore();
	return;
}
void PenBars::drawBars( QPainter* painter )
{
	painter->save();
	//******************************************
	QPointF initPos = this->_bounding.bottomLeft() + QPointF( _barOffsetX, -_fontHeight );
	for( int i = 0; i < this->_data.size(); i++ )
	{
		//QRectF rect( initPos, QSize( BARSIZE, -BARHEIGHT * ( _data[i] + _min )/ ( _max - _min ) ) );
		QRectF rect( initPos, QSize( BARSIZE, -BARHEIGHT * ( _data[i] + _max )/ ( _max * 2 ) ) );
		//
		painter->setFont( this->_font );
		painter->setPen( QColor( 0,0,0,255) );
		painter->drawText( rect.bottomLeft(), _dataString[i] );
		//
		painter->setPen( Qt::NoPen );
		painter->setBrush( QBrush( this->_color[i] ) );
		painter->drawRect( rect );
		initPos += QPointF( BARSIZE, 0 );
	}
	//******************************************
	painter->restore();
	return;
}

void PenBars::drawFonts( QPainter* painter )
{
	painter->save();
	//******************************************
	QPointF initPos = this->_bounding.bottomLeft() + QPointF( BARMARGIN, 0);
	painter->drawText( initPos, _para );
	//for( int i = 0; i < this->_name.size(); i++ )
	//{
	//	painter->setPen( this->_color[i] );
	//	painter->drawText( initPos, _name[i] );
	//	//
	//	initPos += QPointF( 0, _fontHeight );
	//}
	//******************************************
	painter->restore();
	return;
}

void PenBars::setPos(QPoint pos)
{
	PenItem::setPos( pos );
	return;
}

void PenBars::updateBoundingRect()
{
	QFontMetrics _metrics( QFont( "times", 8 ) );
	int width, height;
	QRectF bounding = _metrics.boundingRect( _para );
	_fontHeight = bounding.height();
	if( bounding.width() > _name.size() * 10 + BARMARGIN*2 )
	{
		_bounding.setWidth( bounding.width() + BARMARGIN*2 );
	}
	else
	{
		_bounding.setWidth( _name.size() * BARSIZE + BARMARGIN*2 );
	}
	_barOffsetX = ( _bounding.width() - _name.size() * BARSIZE ) / 2;
	//_bounding.setWidth( name.size() * BARSIZE );
	//************************************************************************
	//height = bounding.height() * (_name.size()+1) + BARHEIGHT;
	height = bounding.height() + BARHEIGHT;
	_bounding.setHeight( height );
	return;
}