#include "PenSlider.h"

PenSlider::PenSlider( QPointF pos, QList<QList<ItemBase*> > bats, QList<QList<float> > rates )
: _curFont( "times", 8 )
, _metrics( _curFont )
, _ONDRAG( false )
, _ONSLIDER( false ) 
, MaxRate( -9999 )
, MinRate( 9999 )
{
	_TYPE = 0;
	this->_iniPos = pos;
	this->_bats = bats;
	this->_rates = rates;
	_background = QRectF( 20, 5, 160, 20 );
	_bounding = QRectF( 0, 0, 200, 30 );
	_curSlider = QRectF( 23, 9, 30, 14 );
	hideAllItems();
	return;
}

PenSlider::~PenSlider()
{
	for( int i = 0; i < this->_bats.size(); i++ )
	{
		for( int j = 0; j < this->_bats[i].size(); j++ )
		{
			delete this->_bats[i][j];
		}
	}
	_bats.clear();
	return;
}

void PenSlider::initScale()
{
	for( int i = 0; i < _bats.size(); i++ )
	{
		for( int j = 0; j < _bats[i].size(); j++ )
		{
			float curRate = (_rates[i][j] - MinRate ) / ( MaxRate - MinRate );
			float x = _background.left() + _background.width() * curRate;// + this->_background.left();
			this->_scale.append( QLine( QPoint( x, _bounding.bottom() ),QPoint( x, _bounding.bottom() - 5 ) ) );
			QString scaleString;
			scaleString.setNum( _rates[i][j] );
			scaleString.resize( 5 );
			this->_scaleString.append( scaleString );
			//
		}
	}
	return;
}

void PenSlider::hideAllItems()
{
	for( int i = 0; i < _bats.size(); i++ )
	{
		for( int j = 0; j < _bats[i].size(); j++ )
		{
			//_bats[i][j]->setPos( _iniPos + QPointF( 0, j*200 ) );
			_bats[i][j]->hide();
			if( MaxRate < _rates[i][j] )
			{
				MaxRate = _rates[i][j];
			}
			if( MinRate > _rates[i][j] )
			{
				MinRate = _rates[i][j];
			}
		}
	}
	initScale();
}

void PenSlider::posUpdate( ItemBase* init )
{
	QPointF offset = init->pos() - this->_iniPos;
	this->_iniPos = init->pos() + QPointF( init->Width()/2 + 100, 0 );
	for( int i = 0; i < _bats.size(); i++ )
	{
		for( int j = 0; j < _bats[i].size(); j++ )
		{
			//_bats[i][j]->setPos( _iniPos + QPointF( 0, j*200 ) );
			_bats[i][j]->setPos( _bats[i][j]->pos() + offset );
		}
	}
	this->_iniPos = init->pos();
}

void PenSlider::showItemAbove( float rate )
{
	rate += 0.05;
	float curRate = 0;
	float batRate = 0;
	for( int i = 0; i < _bats.size(); i++ )
	{
		for( int j = 0; j < _bats[i].size(); j++ )
		{
			curRate = (MaxRate - MinRate)*rate + MinRate;
			batRate = _rates[i][j];
			if( curRate > batRate )
			{
				_bats[i][j]->show();
				/*QPointF pos = this->_bats[i][j]->pos();*/
			}
			else
			{
				_bats[i][j]->hide();
			}
		}
	}
	return;
}

void PenSlider::paint(QPainter *painter)
{
	painter->save();
	//**************************************************
	//**************************************************
	painter->setPen( Qt::NoPen );
	painter->setBrush( QColor( 90, 80, 80, 100 ) );
	painter->drawRect( this->_background );

	painter->setPen( QPen( QBrush( QColor( 50, 50, 50, 100 ) ), 3 ) );
	painter->drawLine( this->_background.topLeft(), this->_background.topRight() );
	painter->drawLine( this->_background.topLeft(), this->_background.bottomLeft() );

	painter->setPen( QPen( QBrush( QColor( 220, 220, 220, 100 ) ), 3 ) );
	painter->drawLine( this->_background.bottomLeft(), this->_background.bottomRight() );
	painter->drawLine( this->_background.bottomRight(), this->_background.topRight() );

	if( _ONSLIDER )
	{
		painter->setPen( QPen( QBrush( QColor( 200, 200, 0, 100 ) ), 3 ) );
	}
	else
	{
		painter->setPen( Qt::NoPen );
	}
	painter->setBrush( QBrush( QColor( 180, 230, 200, 100 ) ) );
	painter->drawRect( this->_curSlider );

	painter->setPen( QPen( QBrush( QColor( 50, 50, 50, 100 ) ), 2 ) );
	painter->drawLine( this->_curSlider.bottomLeft(), this->_curSlider.bottomRight() );
	painter->drawLine( this->_curSlider.bottomRight(), this->_curSlider.topRight() );

	painter->setPen( QPen( QBrush( QColor( 220, 220, 220, 100 ) ), 2 ) );
	painter->drawLine( this->_curSlider.topLeft(), this->_curSlider.topRight() );
	painter->drawLine( this->_curSlider.topLeft(), this->_curSlider.bottomLeft() );
	

	//**************************************************
	painter->restore();
	if( this->_scale.size() == 0 )
	{
		return;
	}
	painter->drawLines( _scale );

	QRectF bounding = painter->fontMetrics().boundingRect( _scaleString[0] );
	painter->drawText( this->_scale.first().p1() + QPointF( - bounding.width() / 2, 10.0) , _scaleString[0] );
	bounding = painter->fontMetrics().boundingRect( _scaleString[_scale.size()-1] );
	painter->drawText( this->_scale.last().p1() + QPointF( - bounding.width() / 2, 10.0), _scaleString[_scale.size()-1] );
	PenItem::paint( painter );
	return;
}

QString PenSlider::toQString()
{
	return QObject::tr( "Slider" );
}

bool PenSlider::mouseOver( QPointF pos )
{
	this->_ONSLIDER = this->_curSlider.contains( pos );
	//		drag
	if( _ONDRAG )
	{
		QPointF offset = pos - this->_lastPos;
		if( _curSlider.left() + offset.x() > this->_background.left() && ( _curSlider.right() + offset.x() ) < this->_background.right() )
		{
			updateRate();
			this->_curSlider.translate( offset.x(), 0 );
		}
	}
	//
	_lastPos = pos;
	//PenItem::mouseOver( pos );
	return true;
}

void PenSlider::setPos(QPoint pos)
{
	QPointF offset = QPointF( pos.x(), pos.y() ) - this->_bounding.topLeft();
	this->_curSlider.translate( offset );
	this->_background.translate( offset );
	for( int i = 0; i < this->_scale.size(); i++ )
	{
		this->_scale[i].translate( offset );
	}
	PenItem::setPos( pos );
	return;
}

void PenSlider::click()
{
	_ONDRAG = true;
	return;
}

void PenSlider::release()
{
	_ONDRAG = false;
	return;
}

void PenSlider::updateRate()
{
	float curRate = ( this->_curSlider.left() - this->_background.left() ) / ( this->_background.width() - this->_curSlider.width() );
	showItemAbove( curRate );
}