
#include "ScatterBubble.h"
#include "openglscene.h"

ScatterBubble::ScatterBubble( int size, int x, int y, OpenGLScene* Scene, StatisticManager* manager, QString name ) 
: PlotBase(size, x, y, Scene, manager)
{
	batName = name;
	this->_TYPE = SCATTER;
	_RAWDATAVIEW = false;
	QString timestr = this->scene->getCurTime().toString( "h:m:s ap" );
	QString logtext;
	logtext.append( timestr );
	logtext.append( "     " );
	logtext.append( "Open ScatterPlot bubble at " );
	logtext.append( this->posString() );
	logtext.append( "\n" );
	this->scene->addLogText( logtext );
}

void ScatterBubble::logOpen()
{
	ItemBase::logOpen();
	emit logInfor("\n");
}

void ScatterBubble::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
	PlotBase::paint( painter, option, widget );

	if ( xParaName == "X" || yParaName == "Y" )
	{
		QRect bounding;
		QString typeName;
		typeName = tr("Scatter Plot");
		bounding = painter->fontMetrics().boundingRect( typeName );
		painter->drawText(  QPointF(Width()/2-3, -Height()/2+10) + QPointF(-bounding.width(), bounding.height()/2), typeName );
	}
	if ( !BADDATA )
	{
		this->plotXY( painter );
		if (_RAWDATAVIEW)
		{
			//rawDataView->paintDataView( painter );
		}
	}
}

void ScatterBubble::plotXY( QPainter *painter )
{
	QPen pen;

	painter->save();

	pen.setBrush( QColor( 47, 102, 247, 100 ) );
	pen.setWidth( 2.0 );

	painter->setPen( pen );

	for ( int i = 0; i < dataPoint.size(); i ++ )
	{
		painter->drawEllipse( dataPoint[i], 2, 2);
	}

	painter->restore();
}

void ScatterBubble::saveAsXmlNode( QDomDocument &doc, QDomElement &node )
{
	QDomText text;

	QDomElement type = doc.createElement( tr("Type") );
	text = doc.createTextNode( tr("Scatter Plot") );
	type.appendChild( text );
	node.appendChild( type );

	QDomElement fileName = doc.createElement( tr("BatName") );
	text = doc.createTextNode( batName );
	fileName.appendChild( text );
	node.appendChild( fileName );

	PlotBase::saveAsXmlNode( doc, node );
}

void ScatterBubble::recoverFromXmlNode( QDomElement node )
{
	QDomNodeList paraNode;
	paraNode = node.elementsByTagName( tr("BatName") );
	batName = paraNode.at(0).toElement().text();

	PlotBase::recoverFromXmlNode( node );
}

void ScatterBubble::hoverMoveEvent( QGraphicsSceneHoverEvent *event )
{
	if ( nameRect.contains( event->pos() ) )
	{
		_RAWDATAVIEW = true;
		//rawDataView->setBatName( nameList[0] );
		//rawDataView->setPos( event->pos() );
	}
	else
	{
		_RAWDATAVIEW = false;
	}
}