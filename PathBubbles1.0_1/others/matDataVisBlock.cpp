
#include "matDataVisBlock.h"
#include "openglscene.h"

MatDataVisBlock::MatDataVisBlock( int size, int x, int y, OpenGLScene* scene) : PlotBase(size, x, y, scene)
, codeLine("")
{
	setFlag(QGraphicsItem::ItemIsMovable, false);
}

void MatDataVisBlock::setCodeLine( QString str )
{
	codeLine = str;
}

void MatDataVisBlock::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
	QPen pen;
	int lineWidth = BOARDERSIZE;

	painter->save();
	painter->setRenderHint(QPainter::Antialiasing, true);

	if (option->state & QStyle::State_Selected) 
	{
		pen.setColor( QColor( _colorBoarder.a, _colorBoarder.b, _colorBoarder.c, 50 ) );
		painter->setPen( pen );
		ItemBase::drawControlHint( painter );
		painter->drawRect(realRect());
		pen.setWidth( lineWidth );
		painter->setPen( pen );
		drawBoarder( painter );
	}
	else
	{
		if( this->_highLight )
		{
			pen.setColor( QColor( _hiColor.a, _hiColor.b, _hiColor.c, 100 ) );	
			pen.setWidth( lineWidth );
			painter->setPen( pen );
			drawBoarder( painter );
		}
		else
		{
			pen.setColor( QColor( _colorBoarder.a, _colorBoarder.b, _colorBoarder.c, 100 ) );
			pen.setWidth( lineWidth );
			painter->setPen( pen );
			drawBoarder( painter );
		}
	}
	painter->setRenderHint(QPainter::Antialiasing, false);
	ItemBase::ControlAutoHide();

	painter->restore();
	this->plotXY( painter );
}

void MatDataVisBlock::plotXY( QPainter* painter )
{
	QRect rect;
	PlotBase::plotXY( painter );
	painter->save();
 	
	painter->setPen( Qt::darkRed );
	painter->setFont( QFont( "times",9 ) );
	rect = painter->fontMetrics().boundingRect( codeLine );
	painter->drawText( QPointF(Width()/2, -Height()/2+10) + QPointF(-rect.width(), rect.height()/2), codeLine );

 	for (int i = 0; i < dataNum; i ++)
 	{
		painter->setPen( Qt::darkRed );
		painter->drawLine( dataPoint[i]+QPointF(-1,-1), dataPoint[i]+QPointF(1,1));
		painter->drawLine( dataPoint[i]+QPointF(-1,1), dataPoint[i]+QPointF(1,-1));
 		if (i > 0)
 		{
			painter->setPen( Qt::darkGreen );
 			painter->drawLine( dataPoint[i-1], dataPoint[i] );
 		}
 	}

	painter->restore();
}

void MatDataVisBlock::drawBoarder( QPainter* painter )
{
	painter->save();
	painter->drawRoundedRect(this->realRect(), 20, 20);
	painter->restore();
}

bool MatDataVisBlock::onBoarder( QPointF pos )
{
	int x = pos.x();
	int y = pos.y();
	int half = this->Width()/2;
	int half2 = this->Height()/2;
	if( (x > half - BOARDERSIZE) || (x < -half + BOARDERSIZE) )
	{
		return true;
	}
	if( (y > half2 - BOARDERSIZE) || (y < -half2 + BOARDERSIZE) )
	{
		return true;
	}
	return false;
}

void MatDataVisBlock::saveAsXmlNode( QDomDocument &doc, QDomElement &node )
{
	PlotBase::saveAsXmlNode( doc, node );

	QDomElement cline = doc.createElement( tr("CodeLine") );
	QDomText text = doc.createTextNode( this->codeLine );
	cline.appendChild( text );
	node.appendChild( cline );
}