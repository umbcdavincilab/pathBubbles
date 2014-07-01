#include "openglscene.h"
#include "qtbox.h"
#include "qtcolorbar.h"
#include "moveButton.h"
#include <QSlider>
#include <QLabel>
#include <QLCDNumber>
#include <QCheckBox>
#include <QtXml>

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

//const int MAX_ITEM_SIZE = 512;
//const int MIN_ITEM_SIZE = 16;

QDialog* OpenGLScene::createDialog(const QString &windowTitle) const
{
	QDialog *dialog = new QDialog(0, Qt::CustomizeWindowHint | Qt::WindowTitleHint);

	dialog->setWindowOpacity(1.0);
	dialog->setWindowTitle(windowTitle);
	dialog->setLayout(new QVBoxLayout);

	return dialog;
}

OpenGLScene::OpenGLScene( LabObject* input2, LabCut* input3)
: QGraphicsScene()
, m_wireframeEnabled(false)
, m_normalsEnabled(false)
, m_modelColor(153, 255, 0)
, m_backgroundColor(100, 100, 100)
// , m_model(0)
, m_lastTime(0)
, m_distance(1.4f)
, m_angularMomentum(0, 40, 0)
, timerId(0)
, _MOVEFLOATING( false )
, _BATATTACHED( false )
//, floatingSize( 0, 0 )
, _wantsCompare( false )
, _WEB( false )
, _MOVEBUTTON( false )
, _PCINDEX( -1 )
, _BATSPECIES( -1 )
, _TopLeft( 0, 0 )
//, _ONDRAG( false )
, _MultiSelect( false )
, _selectRect()
, hasBatAttribute( false )
, hasMatBubble( false )
, recordingScene( false )
, bubbleIndex( 0 )
{
	logFile.setFileName( "logFile.txt" );
	if ( logFile.open( QIODevice::WriteOnly | QIODevice::Text ) )
	{
		log_output.setDevice( &logFile );
		log_output << "";
	}

	QString timestr = m_time.currentTime().toString( "h:m:s ap" );
	logStr.append( timestr );
	logStr.append( "   " );
	logStr.append( "Start a new VisBubble Application\n" );

	lab = input2;

	cut = input3;

	ani = new animateIt( 500 );

	order = new OrderManager();

	allbats = new AllBats();

	_manager = new GroupManager( this );

	this->_perfix = TDHelper::getDataPrefix();

	_openManager = new OpenManager( this, lab, cut, _manager, allbats );

	initFloatingControl();

	//recoverSceneFromXmlFile("xmlTest/videoTest.xml");

	m_time.start();
	//
	this->timer.start( 30 );
	connect( &timer, SIGNAL(timeout()), this, SLOT(update()) );
}

void OpenGLScene::appHasClosed()
{
	QString timestr = this->getCurTime().toString( "h:m:s ap" );
	QString logtext;
	logtext.append( timestr );
	logtext.append( "     " );
	logtext.append( "Close this VisBubble Application" );
	logtext.append( "\n" );
	this->addLogText( logtext );

	writeLogFile();
}

void OpenGLScene::logOperation( QString str )
{
	QString timestr = this->getCurTime().toString( "h:m:s ap" );
	QString logtext;
	logtext.append( timestr );
	logtext.append( "     " );
	logtext.append( str );
	this->addLogText( logtext );
}

void OpenGLScene::logPasteMatBubble( QPointF pos )
{
	QString timestr = this->getCurTime().toString( "h:m:s ap" );
	QString logtext, str, tmp;
	logtext.append( timestr );
	logtext.append( "     " );
	logtext.append( "Paste the matlab bubble at ");
	str = "( ";
	tmp.setNum( pos.x() );
	str.append( tmp );
	str.append(", ");
	tmp.setNum( pos.y() );
	str.append( tmp );
	str.append(" )");

	logtext.append( str );
	this->addLogText( logtext );
}

void OpenGLScene::writeLogFile()
{
	if ( !logStr.isNull() )
	{
		logFile.setFileName( "logFile.txt" );
		if ( logFile.open(QIODevice::WriteOnly | QIODevice::Text | QFile::Append ) )
		{
			log_output.setDevice( &logFile );
			log_output << logStr;

			logStr = "";
			logFile.close();
		}
	}
}

int OpenGLScene::getBubbleIndex()
{
	return bubbleIndex++;
}

void OpenGLScene::initFloatingControl()
{
	//navigation bar
	//************************************************************************************
	_navi = new NavigationBlock( QSizeF( this->width(), 50 ) );
	this->connect( _navi, SIGNAL( shiftViewport( QPoint )), this, SLOT( shiftViewport( QPoint ) ) );
	this->connect( _navi, SIGNAL( updateCoverage( float )), this, SLOT( updateCoverage( float ) ) );
	this->connect( _navi, SIGNAL( logInfor( QString ) ), this, SLOT( logInfor( QString ) ) );
	this->addItem( _navi );
	_navi->setZValue( 99 );
	_navi->logInit();

	//file control
	//*************************************************************************
	this->_file = new FileManager( QSizeF( 600, 200 ), this, _openManager );
	connect( _file, SIGNAL(orderByWeight()), this, SLOT(orderViews()) );
	connect( _file, SIGNAL(orderBySpeed()), this, SLOT(mapViews()) );
	connect( _file, SIGNAL( logInfor( QString ) ), this, SLOT( logInfor( QString ) ) );
	this->addItem( _file );
	_file->setZValue( 99 );
	_file->logInit();

	//statistic bar
	//*************************************************************************
	this->_statistic = new StatisticManager( "data/statistical.txt" );
	this->_staBar = new StatisticBar( QSize(220, 630), QPoint( 100, 200 ), this->_statistic );
	this->connect( _staBar, SIGNAL( logInfor( QString ) ), this, SLOT( logInfor( QString ) ) );
	connect( _staBar, SIGNAL( needStatisticalData( QPointF, StatisticManager*, int ) ), this, SLOT(needStatisticalData( QPointF, StatisticManager*, int)) );
	this->addItem( _staBar );
	this->_staBar->setZValue( 100 );
	_staBar->logInit();

	//wing graphics bar
	//*************************************************************************
	this->_arrow = new ArrowGen( QSizeF(200, 100), this );
	this->addItem( _arrow );
	this->_arrow->setZValue( 99 );
	connect( _arrow, SIGNAL( setWingShapePara( ArrowGen*, QPointF ) ), this, SLOT( setWingShapePara( ArrowGen*, QPointF ) ) );
	connect( _arrow, SIGNAL( logInfor( QString ) ), this, SLOT( logInfor( QString ) ) );
	_arrow->logInit();

	return;
}

void OpenGLScene::drawBackground(QPainter *painter, const QRectF & rect)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawSelecting( painter );

	int width = this->width();
	int height = this->height();
	QLinearGradient linearGrad(QPointF( width/2, 0 ), QPointF( width/2, height));
	linearGrad.setColorAt(0, QColor( 0, 0, 100, 100) );
	linearGrad.setColorAt(1, QColor( 200, 200, 200, 200 ) );
	painter->setBrush( QBrush( linearGrad ) );
	painter->drawRect( rect );

	this->_navi->updateSize( QSizeF( this->width(), this->_navi->getSize().height() ) );

	/* glutSolidSphere( 1, 16, 16); */
	if( !this->_manager->isOnMoving() )
	{
		springEvent();
	}
	int tmpTime;
	tmpTime = m_time.elapsed() - m_lastTime;
	if ( tmpTime > 10*1000 && recordingScene == false )
	{
		recordingScene = true;
		m_lastTime = m_time.elapsed();
		recordSceneAsXmlFile( "RecentScene.xml" );

		writeLogFile();

		recordingScene = false;
	}
	//QTimer::singleShot(20, this, SLOT(update()));
}

void OpenGLScene::drawForeground(QPainter *painter, const QRectF &rect)
{
	//this->_manager->drawAllGroupBoarder( painter );
}

void OpenGLScene::setBackgroundColor()
{
	const QColor color = QColorDialog::getColor(m_backgroundColor);
	if ( color.isValid() )
	{
		m_backgroundColor = color;
		update();
	}
}

void OpenGLScene::resizeEvent(QResizeEvent *event)
{
	// 	QSizeF tsize = event->size();
	// 
	// 	this->_navi->updateSize( QSizeF( tsize.width(), this->_navi->getSize().height() ) );
	// 	this->_staBar->updateSize( QSizeF( tsize.width(), this->_staBar->getSize().height() ) );
}

void OpenGLScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	//if( _MOVEBUTTON )
	//{
	//	QPointF offset = event->lastScenePos() - event->scenePos();
	//	this->_webView->setPos( this->_webView->pos() - offset );
	//}
	QGraphicsScene::mouseMoveEvent(event);

	if( !event->isAccepted() && _MultiSelect )
	{
		expendRect( event->scenePos() );
	}
	if (event->isAccepted())
	{
		return;
	}
	if (event->buttons() & Qt::LeftButton)
	{
		const QPointF delta = event->scenePos() - event->lastScenePos();
		const Point3d angularImpulse = Point3d(delta.y(), delta.x(), 0) * 0.1;

		m_rotation += angularImpulse;
		m_accumulatedMomentum += angularImpulse;

		event->accept();
		update();
	}
}

void OpenGLScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsScene::mousePressEvent(event);
	if( !event->isAccepted() && event->button() == Qt::RightButton )
	{
		_MultiSelect = true;
		_initSelectPoint = event->scenePos();
	}
	//if ( !event->isAccepted() && event->button() == Qt::LeftButton )
	//{
	//	_ONDRAG = true;
	//}
	if (event->isAccepted())
	{
		return;
	}

	m_mouseEventTime = m_time.elapsed();
	m_angularMomentum = m_accumulatedMomentum = Point3d();
	event->accept();
}

void OpenGLScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	if ( _wantsCompare )
	{
		findCompareTarget( event->scenePos() );
	}
	if ( this->_MultiSelect )
	{
		findSelected();
		this->_MultiSelect = false;
	}
	QGraphicsScene::mouseReleaseEvent(event);
	if ( !event->isAccepted() )
	{
		//_ONDRAG = false;
		this->_selectRect = QRectF( 0, 0, 0, 0 );
	}
	if ( event->isAccepted() )
		return;

	const int delta = m_time.elapsed() - m_mouseEventTime;
	m_angularMomentum = m_accumulatedMomentum * (1000.0 / qMax(1, delta));
	
	event->accept();
	update();
}

void OpenGLScene::wheelEvent(QGraphicsSceneWheelEvent *event)
{
	QGraphicsScene::wheelEvent(event);

	if (event->isAccepted())
	{
		return;
	}
	m_distance *= qPow(1.2, -event->delta() / 120);
	event->accept();
	update();
}

void OpenGLScene::index_recieved(int index)
{
	emit item_clicked( index );
	return;
}

void OpenGLScene::springEvent()
{
	QList<ItemBase*> nodes;
	foreach (QGraphicsItem *item, this->items()) 
	{
		if (ItemBase *node = dynamic_cast<ItemBase*>(item))
		{
			nodes << node;
		}
	}

// 	foreach (ItemBase* node, nodes)
// 	{
// 		node->calculateForces();
// 	}

	this->_manager->updateItemsPos();

	bool itemsMoved = false;
	foreach (ItemBase *node, nodes) 
	{
		if (node->advance())
			itemsMoved = true;
	}

	if (!itemsMoved)
	{
		killTimer(timerId);
		timerId = 0;
	}
}

void OpenGLScene::itemMoved()
{
	if (!timerId)
		timerId = startTimer(1000 / 25);
}

void OpenGLScene::floatingButtonPushed()
{
	_MOVEFLOATING = true;
}

void OpenGLScene::shiftViewport(QPoint offset)
{
	QPointF Add( -offset.x(), -offset.y() );
	this->_TopLeft = this->_TopLeft + Add;
	foreach (QGraphicsItem *item, this->items() )
	{
		ItemBase *base = dynamic_cast< ItemBase* >(item);
		if( base )
		{
			if( base->canBeShift() )
			{
				base->mySetPos( base->pos() + Add );
			}
		}
	}
}

void OpenGLScene::addCompare(QtBat *object)
{
	batWantsCompare = object;
	_wantsCompare = true;
}

void OpenGLScene::findCompareTarget( QPointF pos )
{
	if( !_wantsCompare )
	{
		return;
	}
	bool GETBAT = false;
	foreach (QGraphicsItem *item, this->items())
	{
		QtBat *bat = dynamic_cast< QtBat* >(item);
		if( bat && bat->getType() == BAT )
		{
			if( bat->boundingRect().contains( pos - bat->pos() ) )
			{
				//add the camber comparing object
				addLComObject( batWantsCompare, bat );
				GETBAT = true;
				_wantsCompare = false;
			}
		}
	}
	if( !GETBAT )
	{
		QMessageBox msgBox;
		msgBox.setText("Please select a 3D bat flight bubble!");
		msgBox.exec();
	}
	_wantsCompare = false;
}

void OpenGLScene::orderViews()
{
	int length = 0;
	float width = 0, height = 0;
	QList<QtBat*> batList[5];
	foreach (QGraphicsItem *item, this->items() )
	{
		QtBat *itemBase = dynamic_cast< QtBat* >(item);
		if( !itemBase )
		{
			continue;
		}
		if( TDHelper::notInBouding( itemBase->pos(), this->width(), this->height() ) )
		{
			continue;
		}
		if ( itemBase->getType() == BAT )
		{
			//QtBat* tempBat = dynamic_cast<QtBat*>(item);
			int index = allbats->getIndexFromName( itemBase->getBat()->getname() );
			batList[index/5].append( itemBase );
		}
		//height = item->boundingRect().height();
	}
	int curHeight = 300;
	for( int i = 0; i < 5; i++ )
	{
		int curWidth = 200;
		if( batList[i].count() == 0 )
		{
			continue;
		}
		while( batList[i].count() != 0 )
		{
			int curIndex;
			float curSpeed = -999;
			for( int j = 0; j < batList[i].count(); j++ )
			{
				if( batList[i][j]->getSpeed() > curSpeed )
				{
					curIndex = j;
					curSpeed = batList[i][j]->getSpeed();
				}
			}
			batList[i][curIndex]->setPos( curWidth,curHeight );
			curWidth += 200;
			batList[i].removeAt( curIndex );
		}
		curHeight += 180;
	}
}

void OpenGLScene::mapViews()
{
	float maxSpeed = -999;
	float minSpeed = 999;
	int length = 0;
	float width = 0, height = 0;
	QList<QtBat*> batList[5];
	foreach (QGraphicsItem *item, items())
	{
		ItemBase *itemBase = dynamic_cast<ItemBase*>(item);
		if(!itemBase)
		{
			continue;
		}
		if( TDHelper::notInBouding( itemBase->pos(), this->width(), this->height() ) )
		{
			continue;
		}
		if ( itemBase->getType() == BAT )
		{
			QtBat* tempBat = dynamic_cast<QtBat*>(item);
			if( maxSpeed < tempBat->getSpeed() )
			{
				maxSpeed = tempBat->getSpeed();
			}
			if( minSpeed > tempBat->getSpeed() )
			{
				minSpeed = tempBat->getSpeed();
			}
			int index = allbats->getIndexFromName( tempBat->getBat()->getname() );
			batList[index/5].append( tempBat );
		}
		//height = item->boundingRect().height();
	}
	int curHeight = 300;
	for( int i = 0; i < 5; i++ )
	{
		int multiple = 300;
		int lastX = 100;
		if( batList[i].count() == 0 )
		{
			continue;
		}
		while( batList[i].count() != 0 )
		{
			int curIndex;
			float curSpeed = 999;
			for( int j = 0; j < batList[i].count(); j++ )
			{
				if( batList[i][j]->getSpeed() < curSpeed )
				{
					curIndex = j;
					curSpeed = batList[i][j]->getSpeed();
				}
			}
			int proposedX = ( batList[i][curIndex]->getSpeed() - minSpeed )*multiple + 100;
			if( proposedX - lastX < 150 )
			{
				proposedX = lastX + 150;
				lastX += 150;
			}
			else
			{
				lastX = proposedX;
			}
			batList[i][curIndex]->setPos( proposedX, curHeight );
			batList[i].removeAt( curIndex );
		}
		curHeight += 180;
	}
	return;
}

void OpenGLScene::contextMenuEvent( QGraphicsSceneContextMenuEvent * contextMenuEvent )
{
	QGraphicsScene::contextMenuEvent( contextMenuEvent );
	if( contextMenuEvent->isAccepted() )
	{
		return;
	}
	QMenu menu;
	QAction *removeAction = menu.addAction("Remove all");
	QAction *webAction = menu.addAction("Web query");
	QAction *matAction = menu.addAction("Matlab");
	QAction *penAction = menu.addAction("Pen");
	QAction *fileAction = menu.addAction("Open Manager");
	QAction *dataMapAction = menu.addAction("Data Mapping");
	QAction *statisticDataAction = menu.addAction("Parameter Bar");
	QAction *pasteMatAction = menu.addAction("Paste MatBubble");
	QAction *saveScene = menu.addAction("Save Scene");
	QAction *recoverScene = menu.addAction("Recover Scene");

	QAction *selectedAction = menu.exec( contextMenuEvent->screenPos() );

	if ( selectedAction == removeAction )
	{
		logOperation( "Remove all the bubbles in the scene\n" );
		removeAllItems();
	}
	else if( selectedAction == webAction )
	{
		this->_openManager->openWebView( 200, contextMenuEvent->scenePos().x(), contextMenuEvent->scenePos().y() );
	}
	else if( selectedAction == matAction )
	{
		this->_openManager->openMatlab( MATSIZE, contextMenuEvent->scenePos().x(), contextMenuEvent->scenePos().y() );
	}
	else if( selectedAction == penAction )
	{
		this->_openManager->openPen( 200, contextMenuEvent->scenePos().x(), contextMenuEvent->scenePos().y() );
	}
	else if( selectedAction == fileAction )
	{
		this->_file->show();
		logOperation( "Show the Model-Composition-View Bubble\n" );
	}
	else if ( selectedAction == dataMapAction )
	{
		this->_arrow->show();
		logOperation( "Show the Data Mapping Bubble\n" );
	}
	else if ( selectedAction == statisticDataAction )
	{
		this->_staBar->show();
		logOperation( "Show the Parameter Bar\n" );
	}
	else if ( selectedAction == pasteMatAction )
	{
		if (hasMatBubble)
		{
			this->_openManager->openMatlabCopy( MATSIZE, contextMenuEvent->scenePos().x(), contextMenuEvent->scenePos().y(), this->matCurrent );
			logPasteMatBubble( contextMenuEvent->scenePos() );
		}
		hasMatBubble = false;
	}
	else if ( selectedAction == saveScene )
	{
		QString fileName = QFileDialog::getSaveFileName();
		this->recordSceneAsXmlFile( fileName );
	}
	else if ( selectedAction == recoverScene )
	{
		this->removeAllItems();
		QString fileName = QFileDialog::getOpenFileName();
		this->recoverSceneFromXmlFile( fileName );
	}
	return;
}

void OpenGLScene::removeAllItems()
{
	QList<ItemBase*> list; 
	QList<QGraphicsItem*> allItem = this->items();
	for( int i = 0; i < allItem.size(); i++ )
		//foreach ( , this->items() )
	{
		QGraphicsItem *item = allItem[i];
		ItemBase *itemBase = dynamic_cast<ItemBase*>(item);
		if( itemBase )
		{
			list.append( itemBase );
		}
	}
	//foreach ( ItemBase *itemBase, list )
	for( int i = 0; i < list.size(); i++ )
	{
		ItemBase *itemBase = list[i];
		if( itemBase && !itemBase->isDelegated() )
		{
			int TYPE = itemBase->getType();
			if( TYPE == BAT || TYPE == PC || TYPE == COMPLOT || TYPE == DATAPLOT || TYPE == MATLAB || TYPE == WEB || TYPE == WAKE || TYPE == IMAGE || TYPE == PEN || TYPE == VOLUM || TYPE == NOTE )
			{
				itemBase->hide();
				itemBase->properRemove();
				delete itemBase;
			}
		}
	}
	this->_manager->clearAll();
	return;
}

void OpenGLScene::moveButtonState( bool state )
{
	_MOVEBUTTON = state;
}

void OpenGLScene::hideWebView()
{
	this->_webView->hide();
}

void OpenGLScene::openAllPCs()
{
	QString lastPos;
	int batIndexBegin, batIndexEnd;
	batIndexBegin = 0;
	batIndexEnd = allbats->getTotalNum();

	if( _PCINDEX == 1 )
	{
		lastPos = tr("_m1.camber");
	}
	else if( _PCINDEX == 2 )
	{
		lastPos = tr("_m2.camber");
	}
	else if( _PCINDEX == 3 )
	{
		lastPos = tr("_m3.camber");
	}
	int countor = 0;
	for( int i = batIndexBegin; i < batIndexEnd; i++ )
	{
		int col = countor/5;
		int row = countor%5;
		std::string curName = this->_perfix;
		curName.append( "PC/" );
		curName.append( allbats->getNameByIndex(i) );
		curName.append( lastPos.toStdString() );
		//curName.remove( dir.length() - 7, 7);
		curName.erase( curName.length() - 7, 7 );
		//
		//this->_openManager->openPCByPath( 150, 200 + row*180, 100+col*180, tr( curName.c_str() ) );
		//this->_openManager->openPCByPath( 150, 0, 0, tr( curName.c_str() ) );
		//
		countor += 1;
	}
	_PCINDEX = -1;
	return;
}

void OpenGLScene::addLComObject(QtBat *left, QtBat *right)
{
	QtLCom* com = new QtLCom( left, right, this );
	
	this->myAddItem( com );
	left->addBat( right );

	connect( left, SIGNAL(removeCompare()), com, SLOT(removeCompare()) );
	connect( right, SIGNAL(removeCompare()), com, SLOT(removeCompare()) );
	connect( com, SIGNAL( logInfor( QString ) ), this, SLOT( logInfor( QString ) ) );

	com->logOpen();
	
	return;
}

void OpenGLScene::updateCoverage( float rate )
{
	foreach (QGraphicsItem *item, this->items() )
	{
		ItemBase *itembase = dynamic_cast< ItemBase* >(item);
		if( itembase )
		{
			//if( itembase->getType() == COLOR )
			//{
			//	continue;
			//}
			QPointF curPos = itembase->getInitPos() + this->_TopLeft;
			//QPointF curPos = itembase->pos();
			float disX = curPos.x() / rate;
			float disY = curPos.y() / rate;
			itembase->mySetPos( QPointF( disX, disY) );
			//
			//
			int newSize = int( float( itembase->getInitSize() ) / rate + 0.5 );
			itembase->resizeItem( newSize, newSize );
		}
	}
	this->update( QRectF( 0, 0, this->width(), this->height() ) );
	return;
}

void OpenGLScene::expendRect(QPointF pos)
{
	this->_selectRect = QRectF( pos, this->_initSelectPoint );
}

void OpenGLScene::findSelected()
{
	this->_manager->acceptSelectedArea( this->_selectRect );
	return;
}

void OpenGLScene::drawSelecting(QPainter *painter)
{
	if( this->_MultiSelect )
	{
		painter->save();
		painter->setBrush( QBrush( QColor( 0, 0, 200, 150 ) ) );
		painter->setPen( Qt::NoPen );
		painter->drawRect( this->_selectRect );
		painter->restore();
	}
	return;
}

void OpenGLScene::needStatisticalData( QPointF pos, StatisticManager* manager, int index )
{
	QList<QGraphicsItem *> allItems;
	allItems = this->items();

	foreach (QGraphicsItem *item, allItems)
	{
		ItemBase *base = dynamic_cast< ItemBase *>(item);
		if( base )
		{
			if( base->boundingRect().contains( pos - base->pos() ))
			{
				if (base->getType() == COMPLOT )
				{
					//do some thing
					QtLCom* com = dynamic_cast<QtLCom*>(base);
					//manager->indexAcceptedUpdate( index );
					com->acceptStatisticData( manager, index );
				}
				else if ( base->getType() == BARCHART || base->getType() == LINECHART || base->getType() == SCATTER )
				{
					PlotBase* plot = dynamic_cast<PlotBase*>(base);
					plot->changeAttriFromStat( pos - base->pos(), index );
				}
			}
		}
	}
	return;
}

void OpenGLScene::setWingShapePara(ArrowGen *aGen, QPointF pos)
{
	QList<QGraphicsItem *> allItems;
	allItems = this->items();

	foreach ( QGraphicsItem *item, allItems )
	{
		ItemBase *base = dynamic_cast< ItemBase *>( item );

		if (base &&  base->boundingRect().contains( pos - base->pos() ) && base->getType() == BAT)
		{
			_manager->updateWingShapeParainGroup( base, aGen );
			break;
		}
	}
}

void OpenGLScene::setCurrentBatAttribute( QtBat *object )
{
	batAttribute = object->getBatContext();
	hasBatAttribute = true;
}

void OpenGLScene::getNewBatAttribute( QtBat *object )
{
	if (hasBatAttribute)
	{
		object->setBatContext( batAttribute );
	}
}

void OpenGLScene::setMatFileBubble( MatBlock* currentMat )
{
	hasMatBubble = true;
	matCurrent = currentMat; 
}

bool OpenGLScene::changePlotAttribute( QPointF pos, QString name )
{
	QList<ItemBase*> itemList;
	ItemBase* item;
	QPointF tmp;
	int i;

	itemList = this->_manager->allMembers();

	for ( i = 0; i < itemList.size(); i ++)
	{
		item = itemList[i];
		if (item->getType() == BARCHART || item->getType() == LINECHART || item->getType() == SCATTER)
		{
			tmp = pos - item->pos();
			if (item->boundingRect().contains(tmp))
			{
				PlotBase* plotBubble = dynamic_cast<PlotBase*>(item);
				QList<QString> names = _openManager->matchFileName( name );
				plotBubble->changeAttriFromFile( pos - item->pos() , names );
				
				return true;
			}
		}
	}
	return false;
}

void OpenGLScene::recordSceneAsXmlFile( QString filename)
{
	QString inforstr = "Save the current scene as the xml file ";
	inforstr.append( filename );
	inforstr.append( "\n" );
	logOperation( inforstr );

	QFile file( filename );

	if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
		return;

	QDomDocument doc;
	QDomProcessingInstruction instruction;

	instruction = doc.createProcessingInstruction("xml", " version=\"1.0\" encoding=\"UTF-8\" ");
	doc.appendChild( instruction );

	QDomElement root = doc.createElement( tr("Scene") );
	doc.appendChild( root );

	//navigation block
	QDomElement navNode = doc.createElement( tr("NavigationBlock") );
	_navi->saveAsXmlNode( doc, navNode );
	root.appendChild( navNode );

	//group
	QList<Group*> gList = this->_manager->groupList();
	
	QDomElement iGroup, iBubble;
	QDomAttr gId, bId;

	QString str;

	for (int i = 0; i < gList.size(); i ++)
	{
		iGroup = doc.createElement( tr("Group") );
		gId = doc.createAttribute( tr("groupID") );

		str.setNum( i+1 );
		gId.setValue( str );
		iGroup.setAttributeNode( gId );

		for (int j = 0; j < gList[i]->NumOfMembers(); j ++)
		{
			iBubble = doc.createElement( tr("Bubble") );
			bId = doc.createAttribute( tr("bubbleID") );

			str.setNum( j+1 );
			bId.setValue( str );
			iBubble.setAttributeNode( bId );

			gList[i]->getMemberAt(j)->saveAsXmlNode( doc, iBubble );

			iGroup.appendChild( iBubble );
		}
		root.appendChild( iGroup );
	}

	QTextStream out(&file);
	doc.save( out, 4 );
	file.close();
}

void OpenGLScene::recoverSceneFromXmlFile( QString fileName )
{
	QString inforstr = "Recover the scene from the xml file ";
	inforstr.append( fileName );
	inforstr.append( "\n" );
	logOperation( inforstr );

	QFile file(fileName);

	if ( !file.open(QIODevice::ReadOnly) )
	{
		return;
	}

	QDomDocument doc;
	if ( !doc.setContent(&file) )
	{
		file.close();
		return;
	}
	file.close();

	QDomElement rootNode = doc.documentElement();
	QDomNode docNode = rootNode.firstChild();
	QString tagname;

	while ( !docNode.isNull() )
	{
		if ( docNode.isElement() )
		{
			QDomElement eachElement = docNode.toElement();
		
			//recover each node
			tagname = eachElement.tagName();
			if ( tagname == "NavigationBlock" )
			{
				_navi->recoverFromXmlNode( eachElement );
			}
			else if ( tagname == "Group" )
			{
				_manager->addNewGroupFromXmlNode( eachElement );
			}
		}

		docNode = docNode.nextSibling();
	}

	//rebuild parent and son bubble for matBubble
	QDomElement childElement;
	
	docNode = rootNode.firstChild();
	while ( !docNode.isNull() )
	{
		if ( docNode.isElement() )
		{
			QDomElement eachElement = docNode.toElement();

			//recover each node
			tagname = eachElement.tagName();

			if ( tagname == "Group" )
			{
				childElement = eachElement.firstChild().toElement();

				while ( !childElement.isNull() )
				{
					tagname = childElement.elementsByTagName("Type").at(0).toElement().text();
					
					if (tagname == "Matlab")
					{
						buildMatBubbleConnect( childElement );
					}
					childElement = childElement.nextSibling().toElement();
				}
			}
		}
		docNode = docNode.nextSibling();
	}
}

void OpenGLScene::buildMatBubbleConnect( QDomElement node )
{
	QString name;

	QDomNodeList paraNode = node.elementsByTagName( tr("FileName") );
	name = paraNode.at(0).toElement().text();

	paraNode = node.elementsByTagName( tr("Position") );
	QStringList posStr= paraNode.at(0).toElement().text().split( "," );
	QString str = posStr.at(0);
	str.remove( 0, 1 );
	str = str.simplified();
	float x = str.toFloat();
	str = posStr.at(1);
	str.chop(1);
	str = str.simplified();
	float y  = str.toFloat();

	QList<ItemBase*> itemList;
	ItemBase* item;
	MatBlock* tmp;

	itemList = this->_manager->allMembers();
	for (int i = 0; i < itemList.size(); i ++)
	{
		item = itemList[i];
		if ( item->getType() == MATLAB )
		{
			tmp = dynamic_cast<MatBlock*>(item);
			if (tmp->getCurrentFilename() == name 
				&& fabs(tmp->pos().x() - x) < 3
				&& fabs(tmp->pos().y() - y) < 3 )
			{
				tmp->rebuildBubbleConnect( node );
				break;
			}
		}
	}

	return;
}

void OpenGLScene::addLComObjectFromXmlNode( QDomElement node )
{
	QDomElement paraNode;
	QDomNodeList tmpNode;

	QStringList posStr;
	QString name, str;
	float x, y;

	QList<ItemBase*> itemList;
	ItemBase* item;
	QtBat *tmpBat, *left, *right;

	paraNode = node.elementsByTagName( tr("LeftBat") ).at(0).toElement();
	tmpNode = paraNode.elementsByTagName( tr("FileName") );
	name = tmpNode.at(0).toElement().text();

	tmpNode = paraNode.elementsByTagName( tr("Position") );
	posStr = tmpNode.at(0).toElement().text().split(",");
	str = posStr.at(0);
	str.remove( 0, 1 );
	str = str.simplified();
	x = str.toFloat();
	str = posStr.at(1);
	str.chop(1);
	str = str.simplified();
	y  = str.toFloat();

	itemList = this->_manager->allMembers();
	for (int i = 0; i < itemList.size(); i ++)
	{
		item = itemList[i];
		if ( item->getType() == BAT )
		{
			tmpBat = dynamic_cast<QtBat*>(item);
			if ( name == tmpBat->getBatName()
				&& fabs(tmpBat->pos().x() - x) < 5
				&& fabs(tmpBat->pos().y() - y) < 5 )
			{
				left = tmpBat;
				break;
			}
		}
	}

	paraNode = node.elementsByTagName( tr("RightBat") ).at(0).toElement();
	tmpNode = paraNode.elementsByTagName( tr("FileName") );
	name = tmpNode.at(0).toElement().text();

	tmpNode = paraNode.elementsByTagName( tr("Position") );
	posStr = tmpNode.at(0).toElement().text().split(",");
	str = posStr.at(0);
	str.remove( 0, 1 );
	str = str.simplified();
	x = str.toFloat();
	str = posStr.at(1);
	str.chop(1);
	str = str.simplified();
	y  = str.toFloat();

	itemList = this->_manager->allMembers();
	for (int i = 0; i < itemList.size(); i ++)
	{
		item = itemList[i];
		if ( item->getType() == BAT )
		{
			tmpBat = dynamic_cast<QtBat*>(item);
			if ( name == tmpBat->getBatName()
				&& fabs(tmpBat->pos().x() - x) < 5
				&& fabs(tmpBat->pos().y() - y) < 5 )
			{
				right = tmpBat;
				break;
			}
		}
	}

	this->addLComObject( left, right );
}

void OpenGLScene::logInfor( QString str )
{
	this->addLogText( str );
}