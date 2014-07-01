
#include "plotBase.h"
#include "openglscene.h"

PlotBase::PlotBase( int size, int x, int y, OpenGLScene* Scene, StatisticManager* manager) : ItemBase( size, x, y )
, xParaName("X")
, yParaName("Y")
, BADDATA(false)
{	
	scene = Scene;
	this->_TYPE = DATAPLOT;
	this->bIndex = scene->getBubbleIndex();
	xDataType = ZEROD;
	yDataType = ZEROD;
	staManager = manager;
	rawDataView = new BatDataView( staManager );
	_LABEL = false;
	_RAWDATAVIEW = false;
}

PlotBase::~PlotBase()
{
	xVaule.clear();
	yVaule.clear();
	xAxisData.clear();
	yAxisData.clear();
	dataPoint.clear();
}

void PlotBase::updatePlotData()
{
	if ( xVaule.size() <= 0 || yVaule.size() <= 0 )
	{
		return;
	}
	calInterval();
	genDataPoint();
}

void PlotBase::setXYValue( QList<double> xlist, QList<double> ylist )
{
	xVaule = xlist;
	yVaule = ylist;
	dataNum = xVaule.size();

	updatePlotData();
}

void PlotBase::setXValue( QList<double> xlist )
{
	xVaule = xlist;
	if ( xVaule.size() > 0 && yVaule.size() > 0 )
	{
		updatePlotData();
	}
}

void PlotBase::setYValue( QList<double> ylist )
{
	yVaule = ylist;
	if ( xVaule.size() > 0 && yVaule.size() > 0 )
	{
		updatePlotData();
	}
}

void PlotBase::setXYName( QString xName, QString yName )
{
	xParaName = xName;
	yParaName = yName;
}

void PlotBase::setXName( QString xName )
{
	xParaName = xName;
}

void PlotBase::setYName( QString yName )
{
	yParaName = yName;
}

void PlotBase::logDragOnX( QString para )
{
	QString logtext;
	
	logtext.append( headofLogMsg() );
	logtext.append( "Drag ");
	logtext.append( para );
	logtext.append(" to X");
	logtext.append( tailofLogMsg() );	

	emit logInfor( logtext );
}

void PlotBase::logDragOnY( QString para )
{
	QString logtext;
	
	logtext.append( headofLogMsg() );
	logtext.append( "Drag ");
	logtext.append( para );
	logtext.append(" to Y");
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void PlotBase::logBadData()
{
	QString logtext;
	
	logtext.append( headofLogMsg() );
	logtext.append( "Cannot plot because of bad data");
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void PlotBase::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
//	updateLabel();
	ItemBase::paint( painter, option, widget );
	PlotBase::plotXY( painter );
	if (BADDATA)
	{
		displayBadData( painter );
	}
}

void PlotBase::plotXY( QPainter* painter )
{
	painter->save();

	drawXAxis( painter );
	drawYAxis( painter );

	displayName( painter );
	
	painter->restore();
}

void PlotBase::drawXAxis( QPainter* painter )
{
	QPointF xstart, xend, tmp;
	QString valStr;
	QRect bounding;
	QPen pen;

	xstart.setX( -m_size_width/2+40 );
	xstart.setY( m_size_height/2-30 );

	xend.setX( m_size_width/2-20 );
	xend.setY( m_size_height/2-30 );

	painter->save();

	pen.setColor( Qt::black );
	pen.setWidth( 2 );
	painter->setPen( pen );
	painter->setFont( QFont( "times", 11 ) );
	painter->drawLine( xstart, xend );
	tmp = xend + QPointF(-5, 5);
	painter->drawLine( tmp, xend );
	tmp = xend + QPointF(-5, -5);
	painter->drawLine( tmp, xend );
	
	bounding = painter->fontMetrics().boundingRect( xParaName );
	painter->drawText( xend + QPointF( 10, -5 ) + QPointF(-bounding.width()/2, bounding.height()), xParaName );

	if (xVaule.size() > 0)
	{
		tmp = xstart;

		painter->setFont( QFont( "times", 10 ) );
		pen.setWidth( 1 );
		painter->setPen( pen );

		for (int i = 0; i < xAxisData.size(); i ++)
		{
			valStr.setNum( xAxisData[i], 'g', 2 );

			painter->drawLine( tmp, tmp - QPointF(0, 3));
			bounding = painter->fontMetrics().boundingRect( valStr );
			painter->drawText( tmp + QPointF(-bounding.width()/2, bounding.height()), valStr );

			tmp += QPointF(xIntervalDis, 0);
		}
	}

	painter->restore();
}

void PlotBase::drawYAxis( QPainter* painter )
{
	QPointF ystart, yend, tmp;
	QString valStr;
	QRect bounding;
	QPen pen;

	ystart.setX( -m_size_width/2+40 );
	ystart.setY( m_size_height/2-30 );

	yend.setX( -m_size_width/2+40 );
	yend.setY( -m_size_height/2+30 );

	painter->save();

	pen.setColor( Qt::black );
	pen.setWidth( 2 );
	painter->setPen( pen );
	painter->setFont( QFont( "times", 11 ) );
	painter->drawLine( ystart, yend );
	tmp = yend + QPointF(-5, 5);
	painter->drawLine( tmp, yend );
	tmp = yend + QPointF(5, 5);
	painter->drawLine( tmp, yend );

	bounding = painter->fontMetrics().boundingRect( yParaName );
	painter->drawText( yend + QPointF( 30, -15 ) + QPointF(-bounding.width()-3, bounding.height()/2), yParaName);

	if (yVaule.size() > 0)
	{
		tmp = ystart;

		painter->setFont( QFont( "times", 10 ) );
		pen.setWidth( 1 );
		painter->setPen( pen );

		for (int i = 0; i < yAxisData.size(); i ++)
		{
			valStr.setNum( yAxisData[i], 'g', 2 );

			painter->drawLine( tmp + QPointF(3, 0), tmp );
			bounding = painter->fontMetrics().boundingRect( valStr );

			painter->drawText( tmp + QPointF(-bounding.width()-3, bounding.height()/2), valStr );

			tmp -= QPointF( 0, yIntervalDis );
		}
	}
	
	painter->restore();
}

void PlotBase::displayName( QPainter* painter )
{
	if (nameList.size() == 0)
	{
		return;
	}
	QRect bounding;
	QString name;

	painter->save();

	painter->setFont( QFont( "times", 9 ) );
	painter->setPen( QColor(Qt::black) );

	if (nameList.size() == 1)
	{
		name = nameList[0];
	}
	else
	{
		int tindex = nameList[0].lastIndexOf("_");
		name = nameList[0].left( tindex );
	}
	int index = name.indexOf( "_" );
	name.remove( 0, index+1 );
	bounding = painter->fontMetrics().boundingRect( name );
	painter->drawText( this->Width()/2-bounding.width()-3, -this->Height()/2+bounding.height()+14, name );
	
	nameRect = QRect( this->Width()/2-bounding.width()-3, -this->Height()/2+14, bounding.width(), bounding.height() );

	painter->restore();
}

void PlotBase::displayBadData( QPainter* painter )
{
	QRect bounding;
	QString name = "BAD DATA";

	painter->save();

	painter->setFont( QFont( "times", 12 ) );
	painter->setPen( QColor(Qt::darkRed) );
	
	bounding = painter->fontMetrics().boundingRect( name );
	painter->drawText( -bounding.width()/2, -bounding.height()/2, name );

	painter->restore();
}

double PlotBase::getMinValue( QList<double> value )
{
	double minVal = value[0];
	for (int i = 1; i < value.size(); i ++)
	{
		if (value[i] < minVal)
		{
			minVal = value[i];
		}
	}
	return minVal;
}

double PlotBase::getMaxValue( QList<double> value )
{
	double maxVal = value[0];
	for (int i = 1; i < value.size(); i ++)
	{
		if ( value[i] > maxVal )
		{
			maxVal = value[i];
		}
	}
	return maxVal;
}

void PlotBase::genDataPoint()
{
	int i;
	double tx, ty;

	dataPoint.clear();
	dataNum = yVaule.size();

	for (i = 0; i < dataNum; i ++)
	{
		tx = ( xVaule[i] - xmin ) / xIntervalVal * xIntervalDis - m_size_width/2 + 40;
		ty = -( yVaule[i] - ymin ) / yIntervalVal * yIntervalDis + m_size_height/2 - 30;

		dataPoint.append( QPointF(tx, ty) );
	}
}

void PlotBase::calInterval()
{
	xmin = getMinValue( xVaule );
	xmax = getMaxValue( xVaule );
	ymin = getMinValue( yVaule );
	ymax = getMaxValue( yVaule );

	xIntervalDis = (m_size_width - 80) / (INTERNALNUM+2);
	yIntervalDis = (m_size_height - 80) / (INTERNALNUM+2);
	xIntervalVal = (xmax - xmin) / INTERNALNUM;
	yIntervalVal = (ymax - ymin) / INTERNALNUM;

	xAxisData.clear();
	yAxisData.clear();

	for (int i = -1; i <= INTERNALNUM+1; i ++)
	{
		xAxisData.append(xmin + i*xIntervalVal);
		yAxisData.append(ymin + i*yIntervalVal);
	}

	xmin -= xIntervalVal;
	xmax += xIntervalVal;
	ymin -= yIntervalVal;
	ymax += yIntervalVal;
}

void PlotBase::resizeItem( int size1, int size2 )
{
	ItemBase::resizeItem( size1, size2 );
	updatePlotData();
	return;
} 

void PlotBase::wheelEvent( QGraphicsSceneWheelEvent *event )
{
	ItemBase::wheelEvent( event );
	updatePlotData();
	return ;
}

QRectF PlotBase::XAxisBoundingRect()
{
	return QRectF( QPointF(-m_size_width/2+40, m_size_height/2-50), QPointF(m_size_width/2-20, m_size_height/2-10) );	
}

QRectF PlotBase::YAxisBoundingRect()
{
	return QRectF( QPointF(-m_size_width/2+20, -m_size_height/2+30), QPointF(-m_size_width/2+60, m_size_height/2-30) );
}

QRectF PlotBase::plotAreaBoundingRect()
{
	return QRectF( QPointF(-m_size_width/2+60, -m_size_height/2+30), QPointF(m_size_width/2-20, m_size_height/2-50) );
}

void PlotBase::changeAttriFromFile( QPointF pos, QList<QString> names )
{
	if (names.size() == 0)	return;
	
	nameList = names;

	QString paraName, str;
	int index;

	if ( names.size() == 1 )
	{
		paraName = "nFrames";
		str = names[0];
	}
	else
	{
		paraName = "Species";
		index = names[0].lastIndexOf("_");
		str = names[0].left( index );
	}
	index = str.indexOf("_");
	str.remove( 0, index+1 );

	if ( this->XAxisBoundingRect().contains( pos ) )
	{
		xParaName = paraName;
		xDataType = ZEROD;

		logDragOnX( paraName+" "+str );
	}

	updateXYParameter();
}

void PlotBase::changeAttriFromStat( QPointF pos, int index )
{
	QString paraName;
	int dataType;

	paraName = staManager->getNameByIndex( index );

	if ( index < staManager->get1DNameList().size() )
	{
		dataType = ONED;
	}
	else if ( index - staManager->get1DNameList().size() < staManager->get1DTNameList().size() )
	{
		int index = paraName.indexOf( ")" );
		if ( index > -1 )
		{
			paraName.remove(0, index+1);
		}
		dataType = ONEDT;
	}
	else if ( index - staManager->get1DNameList().size() - staManager->get1DTNameList().size() < staManager->get1DFromMatNameList().size() )
	{
		dataType = ONEDMAT;
	}
	else if ( index - staManager->get1DNameList().size() - staManager->get1DTNameList().size() - staManager->get1DFromMatNameList().size() < staManager->get1DTFromMatNameList().size() )
	{
		int index = paraName.indexOf( ")" );
		if ( index > -1 )
		{
			paraName.remove(0, index+1);
		}
		dataType = ONEDTMAT;
	}

	if ( this->YAxisBoundingRect().contains( pos ) )
	{
		yParaName = paraName;
		yDataType = dataType;

		logDragOnY( yParaName );
	}
	else if ( this->XAxisBoundingRect().contains( pos ) )
	{
		xParaName = paraName;
		xDataType = dataType;

		logDragOnX( xParaName );
	}

	//staManager = manager;
	updateXYParameter();
}

void PlotBase::updateXYParameter()
{
	xVaule.clear();
	yVaule.clear();
	int i, index;
	float xdata, ydata, xframes;
	QString str, filePath;

	if ( xParaName == "X" && yDataType == ONEDTMAT)
	{
		yVaule = staManager->getParaValueFromMat( yParaName );
		dataNum = yVaule.size();
		for (i = 0; i < dataNum;  i ++)
		{
			xVaule.append( i );
		}
		updatePlotData();
		return;
	}
	if ( xDataType == ONEDTMAT && yDataType == ONEDTMAT )
	{
		xVaule = staManager->getParaValueFromMat( xParaName );
		yVaule = staManager->getParaValueFromMat( yParaName );
		if (xVaule.size() < yVaule.size())
		{
			dataNum = xVaule.size();
		}
		else
		{
			dataNum = yVaule.size();
		}
		updatePlotData();
		return;
	}

	if ( xParaName == "X" || yParaName == "Y" || nameList.size() == 0 )
		return;

	BADDATA = false;
	if ( xParaName == "nFrames" && yDataType == ONEDT )
	{
		str = yParaName;

		//load camber data
		index = str.indexOf( "camber" );
		if (index != -1)
		{
			filePath = getCamberFilePath( str );
			yVaule = loadCamberData( filePath );
		}

		//load speed data
		index = str.indexOf( "speed" );
		if (index != -1)
		{
			filePath = getSpeedFilePath( str );
			yVaule = loadSpeedData( filePath );
		}

		dataNum = yVaule.size();
		for (i = 0; i < dataNum;  i ++)
		{
			xVaule.append( i );
		}
	}
	else if ( xParaName == "Species" && yDataType == ONED )
	{
		for ( i = 0; i < nameList.size(); i ++ )
		{
			xVaule.append( i+1 );
		}
		
		yVaule.clear();
		for ( i = 0; i < nameList.size(); i ++ )
		{
			ydata = staManager->getValueAt( nameList[i], yParaName );
			yVaule.append( ydata );
		}
	}
	else if ( xDataType == ONED && yDataType == ONED )
	{
		if ( nameList.size() > 0 )
		{
			for (int i = 0; i < nameList.size(); i ++)
			{
				xdata = staManager->getValueAt( nameList[i], xParaName );
				ydata = staManager->getValueAt( nameList[i], yParaName );
				
				xVaule.append( xdata );
				yVaule.append( ydata );
			}
		}
	}
	else if ( xDataType == ONEDT && yDataType == ONEDT )
	{
		str = xParaName;
		//load camber data
		index = str.indexOf( "camber" );
		if (index != -1)
		{
			filePath = getCamberFilePath( str );
			xVaule = loadCamberData( filePath );	
		}
		//load speed data
		index = str.indexOf( "speed" );
		if (index != -1)
		{
			filePath = getSpeedFilePath( str );
			xVaule = loadSpeedData( filePath );
		}

		str = yParaName;
		//load camber data
		index = str.indexOf( "camber" );
		if (index != -1)
		{
			filePath = getCamberFilePath( str );
			yVaule = loadCamberData( filePath );	
		}
		//load speed data
		index = str.indexOf( "speed" );
		if (index != -1)
		{
			filePath = getSpeedFilePath( str );
			yVaule = loadSpeedData( filePath );
		}

		if (xVaule.size() < yVaule.size())
		{
			dataNum = xVaule.size();
		}
		else
		{
			dataNum = yVaule.size();
		}
	}
	else
	{
		BADDATA = true;
		logBadData();
		return ;
	}
	updatePlotData();
}

QList<double> PlotBase::loadCamberData( QString str )
{
	std::ifstream cinput( str.toStdString().c_str() );
	std::string temp;
	QList<double> data;
	float tmpData;
	int index, num;
	//			load camber
	for(int i = 0; i < 5; i++)
	{
		cinput>>temp;
	}
	while(!cinput.eof())
	{
		cinput>>index;
		cinput>>temp;
		cinput>>tmpData;
		data.append( tmpData );
		cinput>>temp;
		cinput>>temp;
	}

	return data;
}

QList<double> PlotBase::loadSpeedData( QString str )
{
	std::ifstream sinput( str.toStdString().c_str() );
	std::string temp;
	float tmpData;
	QList<double> data;
	int index;
	for( int i = 0; i < 2; i++ )
	{
		sinput>>temp;
	}
	while(!sinput.eof())
	{
		sinput>>index;
		sinput>>tmpData;
		data.append( tmpData );
	}

	return data;
}

QString PlotBase::getCamberFilePath( QString str )
{
	QString filePath;

	filePath = "data/Bat/" + nameList[0];
	int index = str.indexOf("_");
	filePath += str.right(str.size()-index);
	filePath += ".";
	filePath += str.left(index);

	return filePath;
}

QString PlotBase::getSpeedFilePath( QString str )
{
	QString filePath;

	filePath = "data/PC/" + nameList[0];
	int index = str.indexOf("_");
	filePath += str.right(str.size()-index);
	filePath += ".";
	filePath += str.left(index);

	return filePath;
}

void PlotBase::saveAsXmlNode( QDomDocument &doc, QDomElement &node )
{
	QDomText text;
	QString str;

	QDomElement xpara = doc.createElement( tr("xParaName") );
	text = doc.createTextNode( this->xParaName );
	xpara.appendChild( text );
	node.appendChild( xpara );

	QDomElement ypara = doc.createElement( tr("yParaName") );
	text = doc.createTextNode( this->yParaName );
	ypara.appendChild( text );
	node.appendChild( ypara );

	QDomElement xtype = doc.createElement( tr("xDataType") );
	str.setNum( xDataType );
	text = doc.createTextNode( str );
	xtype.appendChild( text );
	node.appendChild( xtype );

	QDomElement ytype = doc.createElement( tr("yDataType") );
	str.setNum( yDataType );
	text = doc.createTextNode( str );
	ytype.appendChild( text );
	node.appendChild( ytype );

	QDomElement names = doc.createElement( tr("NameList") );
	str = "(";
	for (int i = 0; i < nameList.size(); i ++)
	{
		if(i > 0)	str += ", ";
		str += nameList[i];
	}
	str += ")";
	text = doc.createTextNode( str );
	names.appendChild( text );
	node.appendChild( names );

	ItemBase::saveAsXmlNode( doc, node );
}

void PlotBase::recoverFromXmlNode( QDomElement node )
{
	QDomNodeList paraNode;

	ItemBase::recoverFromXmlNode( node );

	paraNode = node.elementsByTagName( tr("xParaName") );
	xParaName = paraNode.at(0).toElement().text();

	paraNode = node.elementsByTagName( tr("yParaName") );
	yParaName = paraNode.at(0).toElement().text();

	paraNode = node.elementsByTagName( tr("xDataType") );
	xDataType = paraNode.at(0).toElement().text().toInt();

	paraNode = node.elementsByTagName( tr("yDataType") );
	yDataType = paraNode.at(0).toElement().text().toInt();

	paraNode = node.elementsByTagName( tr("NameList") );
	QStringList str = paraNode.at(0).toElement().text().split(",");
	nameList.clear();
	QString tmpStr;
	for (int i = 0; i < str.size(); i ++)
	{
		tmpStr = str.at(i);
		if ( tmpStr[0] == '(' )
		{
			tmpStr.remove( 0, 1 );
		}
		if ( tmpStr[tmpStr.size()-1] == ')' )
		{
			tmpStr.chop( 1 );
		}
		tmpStr = tmpStr.simplified();
		nameList.append( tmpStr );
	}
	updateXYParameter();
}

void PlotBase::updateLabel()
{
	if (_LABEL == false)
	{
		_label = new ItemLabel( this, scene, m_size_width, m_size_height );
		_label->setLabelColor(_colorBoarder);
		this->scene->addItem(_label);
		_LABEL = true;

		emit updateLabelState( this );

		return;
	}
	_label->updateLabel( m_size_width, m_size_height );

}

QRectF PlotBase::getNameRect()
{
	return nameRect;
}