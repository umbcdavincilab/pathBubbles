
#include "BarChartBubble.h"
#include "openglscene.h"

BarChartBubble::BarChartBubble( int size, int x, int y, OpenGLScene* Scene, StatisticManager* manager, QString name ) 
: PlotBase(size, x, y, Scene, manager)
{
	batName = name;
	this->_TYPE = BARCHART;
}

void BarChartBubble::logOpen()
{
	ItemBase::logOpen();
	emit logInfor("\n");
}

void BarChartBubble::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
	PlotBase::paint( painter, option, widget );

	if ( xParaName == "X" || yParaName == "Y" )
	{	
		QRect bounding;
		QString typeName;
		typeName = tr("Bar Chart");
		painter->setPen( QColor(0,0,0) );
		bounding = painter->fontMetrics().boundingRect( typeName );
		painter->drawText(  QPointF(this->Width()/2-3, -this->Height()/2+10) + QPointF(-bounding.width(), bounding.height()/2), typeName );
	}
	if ( !BADDATA )
	{
		this->plotXY( painter );
		if ( _RAWDATAVIEW )
		{
			//rawDataView->paintDataView( painter );
		}
	}
}

void BarChartBubble::plotXY( QPainter *painter )
{
	painter->save();

	painter->setPen( Qt::NoPen );
	painter->setBrush( QColor(220, 73, 217, 100) );
	for ( int i = 0; i < dataPoint.size(); i ++ )
	{
		painter->drawRect( QRectF( QPointF(dataPoint[i].x()-5, dataPoint[i].y()), QPointF(dataPoint[i].x()+5, this->Height()/2-30) ));
	}

	painter->restore();
}

void BarChartBubble::saveAsXmlNode( QDomDocument &doc, QDomElement &node )
{
	QDomText text;

	QDomElement type = doc.createElement( tr("Type") );
	text = doc.createTextNode( tr("Bar Chart") );
	type.appendChild( text );
	node.appendChild( type );

	QDomElement fileName = doc.createElement( tr("BatName") );
	text = doc.createTextNode( batName );
	fileName.appendChild( text );
	node.appendChild( fileName );

	PlotBase::saveAsXmlNode( doc, node );
}

void BarChartBubble::recoverFromXmlNode( QDomElement node )
{
	QDomNodeList paraNode;
	paraNode = node.elementsByTagName( tr("BatName") );
	batName = paraNode.at(0).toElement().text();

	PlotBase::recoverFromXmlNode( node );
}

void BarChartBubble::hoverMoveEvent( QGraphicsSceneHoverEvent *event )
{
	this->MouseOverBars( event->pos() );
}

void BarChartBubble::MouseOverBars( QPointF pos )
{
	_RAWDATAVIEW = false;
	for ( int i = 0; i < dataPoint.size(); i ++ )
	{
		QRectF rect = QRectF( QPointF(dataPoint[i].x()-5, dataPoint[i].y()), QPointF(dataPoint[i].x()+5, this->Height()/2-30) );
		if ( rect.contains( pos ) )
		{
			//rawDataView->setBatName( nameList[i] );
			//rawDataView->setPos( pos );

			_RAWDATAVIEW = true;
		}
	}
}