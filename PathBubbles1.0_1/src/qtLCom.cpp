#include "qtLCom.h"
#include "openglscene.h"

QtLCom::QtLCom( QtBat *left, QtBat *right, OpenGLScene* Scene ) : ItemBase( 200, (left->pos().x() + right->pos().x())/2, (left->pos().y() + right->pos().y())/2 )
, _colorLeft( 255, 255, 0 )
, _colorRight( 0, 255, 255 )
, _stepHeight( 0 )
, _fontForNumber( "times", 8 )
, _fontForName( "times", 10 )
, INDEXOFFSET( 0 )
, _curParaName( "Camber_" )
{
	_scene = Scene;
	setFlag( QGraphicsItem::ItemIsMovable, false );
	//setFlag( QGraphicsItem::ItemIsSelectable, false );
	this->_TYPE = COMPLOT;
	this->bIndex = _scene->getBubbleIndex();
	this->_left = left;
	this->_right = right;
	this->_nameForLeft = left->getBat()->getname();
	this->_nameForRight = right->getBat()->getname();
	//*****************************************************
	this->_backGround[0] = QColor( 100, 30, 30, 30 );
	this->_backGround[1] = QColor( 30, 100, 30, 30 );
	this->_backGround[2] = QColor( 30, 30, 100, 30 );
	//*****************************************************
	marker[0] = QString("M1");
	marker[1] = QString("M2");
	marker[2] = QString("M3");
	//*****************************************************
	connect( left, SIGNAL( itemRemoved() ), this, SLOT( needToBeRemoved() ));
	connect( right, SIGNAL( itemRemoved() ), this, SLOT( needToBeRemoved() ));
	//*****************************************************
	QFontMetrics fm( this->_fontForName );
	this->textHeight = fm.height()*2;
	QString name = tr( _nameForLeft.c_str() );
	int index = name.indexOf( "_" );
	name.remove( 0, index+1 );
	this->textWidth = fm.width( name );
	fm = QFontMetrics( _fontForNumber );
	numHeight = fm.height();

	loadData();
	//initUI();
}

QtLCom::~QtLCom()
{
	for( int i = 0; i < 3; i++ )
	{
		delete this->_dataForLeft[i];
		delete this->_dataForRight[i];
	}
	delete this->_dataForLeft;
	delete this->_dataForRight;
	//
	for( int i = 0 ; i < this->_bars.size(); i++ )
	{
		delete this->_bars[i];
	}
	this->_bars.clear();
	return;
}

void QtLCom::logOpen()
{
	ItemBase::logOpen();
	QString logtext;
	logtext.append( " between bat with name " );
	logtext.append( _left->getBatName() );
	logtext.append( " and bat with name ");
	logtext.append( _right->getBatName() );
	logtext.append( "\n" );
	
	emit logInfor( logtext );
}

void QtLCom::logAddToComAttr( QString str )
{
	QString logtext;
	
	logtext.append( headofLogMsg() );
	logtext.append( "Add " );
	logtext.append( str );
	logtext.append( " to the compare metric" );
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void QtLCom::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	QString name;
	int index;

	updatePos();
	//*********************************************
	if( this->_bars.size() > 0 )
	{
		drawBars( painter );
	}
	else
	{
		paintCurve( painter );
	}
	//*********************************************
	//			print name
	painter->setFont( _fontForName );
	painter->setPen( this->_colorLeft );
	name = tr( this->_nameForLeft.c_str() );
	index = name.indexOf( "_" );
	name.remove( 0, index+1 );
	painter->drawText( -textWidth/2, -this->Height()/2+textHeight/2, name );
	painter->setPen( this->_colorRight );
	name = tr( this->_nameForRight.c_str() );
	index = name.indexOf( "_" );
	name.remove( 0, index+1 );
	painter->drawText( -textWidth/2, -this->Height()/2 + textHeight, name );
	//*********************************************
	ItemBase::paint( painter, option, widget);
}

void QtLCom::paintCurve( QPainter* painter )
{
	//painter->drawText( this->boundingRect().bottomLeft(), _curParaName );
	int height = int(float(this->Height())/2.0+0.5);
	_stepHeight = float( this->Height() - textHeight )/3.0 - 10;
	for( int i = 0; i < 3; i++ )
	{
		this->paintCurveFrom( painter, height, i );
		height -= (_stepHeight + 10);
	}
	return;
}

void QtLCom::drawBars( QPainter* painter )
{
	for( int i = 0; i < this->_bars.size(); i++ )
	{
		this->_bars[i]->paint( painter );
	}
}

void QtLCom::keyPressEvent( QKeyEvent* event )
{
	//if( this->currentIndex != -1 )
	//{
	//	this->_drawing[currentIndex]->addCode( event->key(), event->text() );
	//}
	int key = event->key();
	switch( key )
	{
	case 16777219:	// backspace
		handleBackspace();
		break;
	default:
		break;
	}
	return;
}

void QtLCom::acceptStatisticData( StatisticManager *manager, int index )
{
	if( index == 0 )
	{
		if( _bars.size() != 0 )
		{
			removeAllBars();
		}
		swithPlot();
		return ;
	}
	float right, left;
	int index_Left = manager->checkIndexByName( this->_left->getBat()->getname() );
	if( index_Left != -1 )
	{
		left = manager->getValueAt( index_Left, index );
	}
	int index_Right = manager->checkIndexByName( this->_right->getBat()->getname() );
	if( index_Right != -1 )
	{
		right = manager->getValueAt( index_Right, index );
	}
	//
	if( left == right )
	{
		QMessageBox msgBox;
		msgBox.setText("Bad parameter!");
		msgBox.exec();
		return ;
	}
	QList<QString> name;
	name.append( tr( this->_left->getBat()->getname().c_str() ) );
	name.append( tr( this->_right->getBat()->getname().c_str() ) );
	//
	QList<float> data;
	data.append( left );
	data.append( right );
	//
	QList<QColor> color;
	color.append(_colorLeft);
	color.append(_colorRight);
	//
	PenBars* bar = new PenBars();
	bar->acceptData( name, data, color, manager->get1DNameList()[index] );
	//
	this->_bars.append( bar );
	//
	reOrderItems();

	logAddToComAttr( manager->getNameByIndex(index) );

	return ;
}

void QtLCom::updatePos()
{
	this->mySetPos( QPointF(( _left->pos().x() + _right->pos().x())/2, ( _left->pos().y() + _right->pos().y())/2) );
}

void QtLCom::mouseMoveEvent( QGraphicsSceneEvent *event )
{
	event->setAccepted( true );
	return;
}

void QtLCom::loadData()
{
	this->_dataForLeft = new SMObject*[6];
	this->_dataForRight = new SMObject*[6];
	QString perfix( TDHelper::getDataPrefix().c_str() );
	QString batPerfix = perfix;
	batPerfix.append( tr( "Bat/") );
	QString pcPerfix = perfix;
	pcPerfix.append( tr( "PC/") );
	for( int i = 0; i < 3; i++ )
	{
		QString curNum;
		curNum.setNum( i+1 );
		QString curPathLeft, curPathRight;
		curPathLeft = curPathRight = batPerfix;
		//**************************************************************
		curPathLeft.append( tr( this->_nameForLeft.c_str() ) );
		curPathLeft.append( tr("_m") );
		curPathLeft.append( curNum );
		curPathLeft.append( tr(".camber") );
		//**************************************************************
		curPathRight.append( tr( this->_nameForRight.c_str() ) );
		curPathRight.append( tr("_m") );
		curPathRight.append( curNum );
		curPathRight.append( tr(".camber") );
		//**************************************************************
		this->_dataForLeft[i] = new SMCamberObject( curPathLeft.toStdString().c_str() );
		this->_dataForRight[i] = new SMCamberObject( curPathRight.toStdString().c_str() );
		//		load three markers speed data
		QString curPathLeft2, curPathRight2;
		curPathLeft2 = curPathRight2 = pcPerfix;
		//************************************************************************
		curPathLeft2.append( tr( this->_nameForLeft.c_str() ) );
		curPathLeft2.append( tr("_m") );
		curPathLeft2.append( curNum );
		curPathLeft2.append( tr(".speed") );
		//************************************************************************
		curPathRight2.append( tr( this->_nameForRight.c_str() ) );
		curPathRight2.append( tr("_m") );
		curPathRight2.append( curNum );
		curPathRight2.append( tr(".speed") );
		//************************************************************************
		this->_dataForLeft[i+3] = new SMSpeedObject( curPathLeft2.toStdString().c_str() );
		this->_dataForRight[i+3] = new SMSpeedObject( curPathRight2.toStdString().c_str() );
	}
	//			formalize the points
	//std::string camberPath = pathName + ".camber";
	//size_t found = pathName.find( "dhl" );
	//if( found != std::string::npos)
	//{
	//	pathName.erase( found, 3);
	//}
	//std::string speedPath = pathName + ".speed";

	//_data = new SMObject*[3];
	//_data[0] = new SMCamberObject( camberPath.c_str() );
	//_data[1] = new SMFrameObject( _data[0]->total_Number() );
	//_data[2] = new SMSpeedObject( speedPath.c_str() );
}
//
//void QtLCom::initUI()
//{
//	this->_REMOVECONTROL = true;
//	QWidget* widget = TDHelper::createDialog( tr( "" ) );
//	delete widget->layout();
//	QGridLayout* result = new QGridLayout();
//	//
//	radioButCamber = new QRadioButton("Color by Individual", widget );
//	radioButCamber->setChecked( true );
//	connect( radioButCamber, SIGNAL( clicked() ), this, SLOT( swithPlot() ) );
//	radioButSpeed = new QRadioButton("Color by Global", widget);
//	connect( radioButSpeed, SIGNAL( clicked() ), this, SLOT( swithPlot() ) );
//	result->addWidget( radioButCamber, 0, 0 );
//	result->addWidget( radioButSpeed, 1, 0 );
//	//*************************************************************************
//	widget->setLayout( result );
//	_myControl = this->scene()->addWidget( widget );
//	return;
//}

void QtLCom::swithPlot()
{
	if( INDEXOFFSET == 0 )
	{
		INDEXOFFSET = 3;
		_curParaName = "Speed_";
	}
	else
	{
		INDEXOFFSET = 0;
		_curParaName = "Camber_";
	}
	return;
}

void QtLCom::paintCurveFrom( QPainter* painter, int height, int index )
{
	painter->save();
	//****************************************************************************
	index += INDEXOFFSET;
	float maxAll = TDHelper::Max( this->_dataForLeft[index]->getMAX(),  this->_dataForRight[index]->getMAX() );
	for( int i = 0; i < 50; i++ )
	{
		_LineForLeft[index][i].setX ( this->Width() * float(i)/50.0 - this->Width()/2.0 );
		float percentLeft = this->_dataForLeft[index]->getRealValueAt(i) / maxAll;
		_LineForLeft[index][i].setY ( height - percentLeft * _stepHeight);
		//*************************************************************************************
		_LineForRight[index][i].setX( this->Width() * float(i)/50.0 - this->Width()/2.0 );
		float percentRight = this->_dataForRight[index]->getRealValueAt(i) / maxAll;
		_LineForRight[index][i].setY( height - percentRight * _stepHeight );
	}
	painter->setPen( QPen( QBrush( this->_colorLeft ), 1 ) );
	painter->drawPolyline( _LineForLeft[index], 50 );
	painter->setPen( QPen( QBrush( this->_colorRight ), 1 ) );
	painter->drawPolyline( _LineForRight[index], 50 );
	//		draw background
	painter->setBrush( this->_backGround[index%3] );
	painter->setPen( QPen( QBrush( QColor(0,0,0,0) ), 1 ) );
	painter->drawRect( QRectF( -this->Width()/2, height-_stepHeight, this->Width(), _stepHeight ));
	//		draw text
	painter->setFont( this->_fontForNumber );
	painter->setPen( QColor( 0, 0, 0 ) );
	painter->drawText( 0, height - _stepHeight, _curParaName + marker[index-INDEXOFFSET] );
	//		draw scale
	QString maxString;
	maxString.setNum( maxAll );
	painter->drawText( -this->Width()/2, height - _stepHeight, maxString );
	painter->drawText( -this->Width()/2, height, tr("0") );
	//*******************************************************************************
	painter->restore();
	return;
}

void QtLCom::needToBeRemoved()
{
	this->scene()->removeItem( this );
}

void QtLCom::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
	if (!isSelected() && scene()) {
        scene()->clearSelection();
        setSelected(true);
    }

    QMenu menu;
    QAction *delAction = menu.addAction("Delete");

    QAction *selectedAction = menu.exec(event->screenPos());

    if ( selectedAction == delAction )
	{
		logRemove();
		removeCompare();
	}
	return;
}


void QtLCom::resizeItem( int sizewidth, int sizeheight )
{
	ItemBase::resizeItem( sizewidth, sizeheight );
	reOrderItems();
	return;
}

void QtLCom::removeCompare()
{
	this->_left->unCompare( this->_right->getBatName() );
	this->_right->unCompare( this->_left->getBatName() );
	needToBeRemoved();
}

void QtLCom::reOrderItems()
{
	int Height = -this->Height()/2 + 10 + textHeight*2;
	int Width = -this->Width()/2;
	int MaxHeight = 0;
	for( int i = 0; i < this->_bars.size(); i++ )
	{
		QRectF curBounding = this->_bars[i]->boundingRect();
		if( Width + curBounding.width() > this->Width()/2 )
		{
			Width = -this->Width()/2;
			Height += MaxHeight + 10;
			MaxHeight = 0;
		}
		this->_bars[i]->setPos( QPoint( Width, Height ) );
		if( MaxHeight < curBounding.height() )
		{
			MaxHeight = curBounding.height();
		}
		Width += curBounding.width();
	}
	return;
}

void QtLCom::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
	mouseOnItems( event->pos() );
	return;
}

void QtLCom::mouseOnItems(QPointF pos)
{
	for( int i = 0; i < this->_bars.size(); i++ )
	{
		this->_bars[i]->mouseOver( pos );
	}
	return;
}

void QtLCom::handleBackspace()
{
	if( _bars.size() > 0 )
	{
		removeLastBar();
	}
	return;
}

void QtLCom::removeAllBars()
{
	for( int i = 0; i < this->_bars.size(); i++ )
	{
		delete this->_bars[i];
	}
	this->_bars.clear();
	return;
}

void QtLCom::removeLastBar()
{
	delete this->_bars.last();
	this->_bars.removeLast();
	return;
}

void QtLCom::saveAsXmlNode( QDomDocument &doc, QDomElement &node )
{
	QDomText text;
	QString str, tmpStr;
	QDomElement fileName, position;

	QDomElement type = doc.createElement( tr("Type") );
	text = doc.createTextNode( tr("ComPlot") );
	type.appendChild( text );
	node.appendChild( type );

	QDomElement leftBat = doc.createElement( tr("LeftBat") );
	fileName = doc.createElement( tr("FileName") );
	text = doc.createTextNode( _left->getBatName() );
	fileName.appendChild( text );
	leftBat.appendChild( fileName);
	position = doc.createElement( tr("Position") );
	str = "(";
	tmpStr.setNum( _left->pos().x() );
	str += tmpStr;
	str += ", ";
	tmpStr.setNum( _left->pos().y() );
	str += tmpStr;
	str += ")";
	text = doc.createTextNode( str );
	position.appendChild( text );
	leftBat.appendChild( position );
	node.appendChild( leftBat );

	QDomElement rightBat = doc.createElement( tr("RightBat") );
	fileName = doc.createElement( tr("FileName") );
	text = doc.createTextNode( _right->getBatName() );
	fileName.appendChild( text );
	rightBat.appendChild( fileName);
	position = doc.createElement( tr("Position") );
	str = "(";
	tmpStr.setNum( _right->pos().x() );
	str += tmpStr;
	str += ", ";
	tmpStr.setNum( _right->pos().y() );
	str += tmpStr;
	str += ")";
	text = doc.createTextNode( str );
	position.appendChild( text );
	rightBat.appendChild( position );
	node.appendChild( rightBat );
}
