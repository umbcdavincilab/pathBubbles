#include "PenDrawing.h"  //绘制直线曲线
#include <QMessageBox>

PenDrawing::PenDrawing( ScriptEngin* engin )
: _FORMAL( false )
, _MOUSEOVER( false ) 
, _currentFont( QObject::tr("times"), 12 )
, _fontMetrics( _currentFont )
{
	//_codeContents.append( QString() );
	this->_engin = engin;
	this->_TYPE = 1;
	_fontHeight = _fontMetrics.height();
	return;
}

PenDrawing::PenDrawing()
: _FORMAL( false )
, _MOUSEOVER( false ) 
, _currentFont( QObject::tr("times"), 12 )
, _fontMetrics( _currentFont )
{
	//_codeContents.append( QString() );
	this->_TYPE = 1;
	_fontHeight = _fontMetrics.height();
	return;
}

void PenDrawing::setEngine( ScriptEngin* engin )
{
	this->_engin = engin;
	return;
}
//PenDrawing::~PenDrawing()
//{
//	return;
//}

void PenDrawing::addLine( QPointF pos1, QPointF pos2 )
{
	updateBoundingRect( pos1 );
	updateBoundingRect( pos2 );
	this->_lines.append( QLineF( pos1, pos2 ) );
	return;
}

void PenDrawing::paint( QPainter *painter )
{
	//drawBackground( painter );
	////************************************************************************
	drawCurve( painter );//绘制曲线
	////************************************************************************
	drawAnalysis( painter );
	////************************************************************************
	//drawCode( painter );
	////************************************************************************
	//drawRun( painter );
	PenItem::paint( painter );
	return;
}

void PenDrawing::drawCurve( QPainter* painter )//绘制曲线
{
	painter->save();
	//*************************************************************************
	painter->setPen( QPen( QBrush( QColor( 0, 0, 255, 255) ), 2 ) );//宽度为2，蓝色
	painter->drawLines( this->_lines );//绘制QVector<QLineF> _lines; 所存放直线
	//*************************************************************************
	painter->restore();
	return;
}

void PenDrawing::setPos(QPoint pos)
{
	QPointF offset = QPointF( pos.x(), pos.y() ) - this->_bounding.topLeft();
	for( int i = 0; i < this->_lines.size(); i++ )
	{
		this->_lines[i].translate( offset.x(), offset.y() );
	}
	_link.translate( offset.x(), offset.y() );
	for( int i = 0; i < 10; i++ )
	{
		_controlPos[i] += QPointF( offset.x(), offset.y() );
		_intersection[i] += QPointF( offset.x(), offset.y() );
		_normalVector[i].translate( QPointF( offset.x(), offset.y() ) );
	}
	//_intersection[0] += QPointF( offset.x(), offset.y() );
	//_intersection[1] += QPointF( offset.x(), offset.y() );
	PenItem::setPos( pos );
	return;
}

void PenDrawing::drawAnalysis( QPainter* painter )
{
	painter->save();
	if( this->_FORMAL )
	{
		painter->setPen( QPen( QBrush( QColor( 255, 0, 0, 255) ), 2 ) );//红色
		painter->drawLine( _link );
		//painter->drawLine( _normalVector[0] );
		//painter->drawLine( _normalVector[1] );
		for( int i = 0; i < 10; i++ )
		{
			painter->drawEllipse( _controlPos[i], 2, 2 );
			painter->drawEllipse( _intersection[i], 2, 2 );
			//painter->drawLine( _normalVector[i] );
		}
		//painter->drawEllipse( _intersection[0], 2, 2 );
		//painter->drawEllipse( _intersection[1], 2, 2 );
	}
	painter->restore();
	return;
}

void PenDrawing::initDescriptor()
{
	this->_dis.value1 = 0.15;
	this->_dis.value2 = 0.15;
	this->_engin->setDiscreptor( this->_dis );
	return;
}

void PenDrawing::initDescriptor( float camber )
{
	this->_dis.value1 = camber;
	this->_dis.value2 = camber;
	this->_engin->setDiscreptor( this->_dis );
}

void PenDrawing::formalizeDrawing()
{
	if( this->_lines.size() < 3)
	{
		return;
	}
	//QPointF start = this->_lines[0].p1();
	//QPointF end = this->_lines[ this->_lines.size() - 1 ].p2();
	//_link.setP1( start );
	//_link.setP2( end );
	////int dis = abs( start.x() - end.x() );
	//for( int i = 0; i < 10; i++ )
	//{
	//	_controlPos[i] = this->_link.pointAt( 0.1*i );
	//}

	//for( int i = 0; i < 10; i++ )
	//{
	//	_link.setP1( start );_link.setP2( end );
	//	QLineF nor = this->_link.normalVector();
	//	QPointF offset = _controlPos[i] - _controlPos[0];
	//	nor.translate( offset );
	//	int Num = 0;
	//	int localCounter = 0;
	//	while( Num != 1 )
	//	{
	//		if( localCounter > 10 )
	//		{
	//			break;
	//			//return;
	//		}
	//		localCounter += 1;
	//		for( int j = 0; j < this->_lines.size(); j++ )
	//		{
	//			int type = this->_lines[j].intersect( nor, &_intersection[i] );
	//			if( type == 1 )
	//			{
	//				Num += 1;
	//				break;
	//			}
	//		}
	//		nor.setP2( nor.p2() + ( nor.p2() - nor.p1() ) );
	//	}
	//	_normalVector[i] = nor;
	//}
	//_FORMAL = true;
	initDescriptor();
	//setHighlight();
	return;
}

void PenDrawing::setHighlight()
{
	return;
}

void PenDrawing::updateBoundingRect(QPointF pos)
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

void PenDrawing::finalize()
{
	formalizeDrawing();
	return;
}

void PenDrawing::finalize( float camber )
{
	initDescriptor( camber );
	return;
}
//bool PenDrawing::mouseOver(QPointF pos)
//{
//	_MOUSEOVER = this->contains( pos );
//	_MOUSEONRUN = this->_rectRun.contains( pos );
//	return _MOUSEOVER;
//}

void PenDrawing::runScript()
{
	//QString result = this->_engin->runScript( _codeContents );
	return;
}

QString PenDrawing::toQString()
{
	return QObject::tr( "Discreptor" );
}

void PenDrawing::getHitPos(double *pointer)
{
	for( int i = 0; i < 10; i++ )
	{
		pointer[i*3  ] = this->_intersection[i].x();
		pointer[i*3+1] = 0;
		pointer[i*3+2] = this->_intersection[i].y();
	}
	return;
}