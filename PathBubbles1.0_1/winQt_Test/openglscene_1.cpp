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
, drawSmallmoleculeSettings(false)
, searchType('P')
, drawDna(false)
, bubbleIndex( 0 )
//, PenisOn(0)
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

	//allbats = new AllBats();

	_manager = new GroupManager( this );

	this->_perfix = TDHelper::getDataPrefix();

	_openManager = new OpenManager( this, lab, cut, _manager);

	initFloatingControl();

	//recoverSceneFromXmlFile("xmlTest/videoTest.xml");

	m_time.start();
	//
	this->timer.start( 30 );
	connect( &timer, SIGNAL(timeout()), this, SLOT(update()) );
	connect( this, SIGNAL(pathBubbleStarted(QRectF)), this, SLOT(showPathBubble(QRectF)) );	
	connect( this, SIGNAL(prePathBubbleStarted(QRectF)), this, SLOT(showPreOpentRect(QRectF)) );		
}


void OpenGLScene::showPreOpentRect(QRect rect)
{
	preOpenRects.push_back(rect);
	
}

void OpenGLScene::showPathBubble(QRect rect)
{
	QString name = "data/pathways/TGF";
	if( name.size() > 0 )
	{		
			ItemBase* item;
	        item = NULL;								
			getOpenManager()->openPathwayByPath(item, 200, rect.x(), rect.y(),name);			
	}	
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

/*void OpenGLScene::logPasteMatBubble( QPointF pos )
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
}*/

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

bool OpenGLScene::naviContains(QPointF pos)
{
	QRectF rectF=_navi->boundingRect();
	return _navi->boundingRect().contains(pos);	
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
	/*this->_file = new FileManager( QSizeF( 600, 200 ), this, _openManager );
	connect( _file, SIGNAL(orderByWeight()), this, SLOT(orderViews()) );
	connect( _file, SIGNAL(orderBySpeed()), this, SLOT(mapViews()) );
	connect( _file, SIGNAL( logInfor( QString ) ), this, SLOT( logInfor( QString ) ) );
	this->addItem( _file );
	_file->setZValue( 99 );
	_file->logInit();*/

	//statistic bar
	//*************************************************************************
	/*this->_statistic = new StatisticManager( "data/statistical.txt" );
	this->_staBar = new StatisticBar( QSize(220, 630), QPoint( 100, 200 ), this->_statistic );
	this->connect( _staBar, SIGNAL( logInfor( QString ) ), this, SLOT( logInfor( QString ) ) );
	connect( _staBar, SIGNAL( needStatisticalData( QPointF, StatisticManager*, int ) ), this, SLOT(needStatisticalData( QPointF, StatisticManager*, int)) );
	this->addItem( _staBar );
	this->_staBar->setZValue( 100 );
	_staBar->logInit();*/

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
	linearGrad.setColorAt(0, QColor( 0, 0, 0, 200) );
	linearGrad.setColorAt(1, QColor( 172, 216, 243, 200 ) );
	painter->setBrush( QBrush( linearGrad ) );
	painter->drawRect( rect );

	this->_navi->updateSize( QSizeF( this->width(), this->_navi->getSize().height() ) );

	/* glutSolidSphere( 1, 16, 16); */
	if( !this->_manager->isOnMoving() )
	{
		springEvent();
		if(!preOpenRects.empty())
	       painter->drawRect( preOpenRects[0] );
	}
	int tmpTime;
	tmpTime = m_time.elapsed() - m_lastTime;
	if ( tmpTime > 10*1000 && recordingScene == false )
	{
		recordingScene = true;
		m_lastTime = m_time.elapsed();
		
		//recordSceneAsXmlFile( "RecentScene.xml" );//keqin 

		writeLogFile();

		recordingScene = false;
	}
	painter->setBrush( QColor() );
	
	this->_manager->drawAllGroupBoarder( painter );	
	
}

void OpenGLScene::drawForeground(QPainter *painter, const QRectF &rect)
{
	for(int i=0; i<_pathBubbles.size();i++)
	{
		if( _pathBubbles[i] )
		if( _pathBubbles[i]->m_pathwayID>=0 && _pathBubbles[i]->m_pathwayID<_pathBubbles.size()) 
		{
			_pathBubbles[i]->drawConnections(painter, _pathBubbles[i]->sceneBoundingRect().center());
		}
	}
	this->_manager->drawAllLabel( painter );
	
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
	{
		int size=_pathBubbles.size();
		for(int i = 0; i<_pathBubbles.size(); i++)
		{
			PathBubble1 *tmp=_pathBubbles[i];
			if( _pathBubbles[i] )
			if( _pathBubbles[i]->m_pathwayID>=0 && _pathBubbles[i]->m_pathwayID<_pathBubbles.size()) //&& _pathBubbles[i]->isVisible())
			{
				int id=_pathBubbles[i]->m_pathwayID;
				if(_pathBubbles[i]->_pieMenuState==1) 
				{
					if(_pathBubbles[i]->_pieMenu->inPie(event->scenePos()))
					{
						_pathBubbles[i]->_pieMenu->setSectorID(event->scenePos());		
						_pathBubbles[i]->_pieMenu->setSectorID2(event->scenePos());	
					}
					else if(_pathBubbles[i]->_pieMenu->inPie2(event->scenePos()))
					{
						vector<int> id=_pathBubbles[i]->_pieMenu->getSectorID2(event->scenePos());
						vector<bool> disables=_pathBubbles[i]->_pieMenu->getDisables();

						if(id[1]<0||disables[id[0]])
							_pathBubbles[i]->_pieMenuState=0;

						_pathBubbles[i]->_pieMenu->setSectorID(event->scenePos());		
						_pathBubbles[i]->_pieMenu->setSectorID2(event->scenePos());			   
					}
					else 
					   _pathBubbles[i]->_pieMenuState=0;
				}
						   
			}
		}
	}

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

int OpenGLScene::findTopBubble(int pid)
{
	set<int> passed;
	return findTopBubble(pid, passed);
}

int OpenGLScene::findTopBubble(int pid, set<int> &passed)
{
	if(_pathBubbles[pid]->_parentPathBubbleID==-1)
		return pid;

    if(_pathBubbles[pid])
	{
		if(passed.find(pid)==passed.end())
		{
			passed.insert(pid);
	        return findTopBubble(_pathBubbles[pid]->_parentPathBubbleID, passed);
		}
	}
	else return -1;
}

void OpenGLScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsScene::mousePressEvent(event);
	if (event->button() == Qt::LeftButton )
	{
		for(int i = 0; i<_pathBubbles.size(); i++)
		{
			if(_pathBubbles[i])
			if(_pathBubbles[i]->m_pathwayID>=0 &&  _pathBubbles[i]->m_pathwayID<_pathBubbles.size() && _pathBubbles[i]->_pieMenuState==1) //&& _pathBubbles[i]->isVisible()
			{
			  
			   if(_pathBubbles[i]->menuType==1)
			   {
			        if(_pathBubbles[i]->_pieMenu->inPie(event->scenePos()))
					{
					
					}					
					else if(_pathBubbles[i]->_pieMenu->inPie2(event->scenePos()))
					{
						vector<int> sectorID=_pathBubbles[i]->_pieMenu->getSectorID2(event->scenePos());	
						vector<bool> disables=_pathBubbles[i]->_pieMenu->getDisables();
						vector<vector<bool>> disables2=_pathBubbles[i]->_pieMenu->getDisables2();
						switch(sectorID[0])
						{   
								case 0:	if(!disables[0]) 
								{
									if(!disables2[0][0]&&sectorID[1]==0) 
									{
										_pathBubbles[i]->menuSelection_1(pos_menu, scenePos_menu,0);
										_pathBubbles[i]->_pieMenuState=0;										
									}
									else if(!disables2[0][1]&&sectorID[1]==1) 
									{
										_pathBubbles[i]->menuSelection_1(pos_menu, scenePos_menu,1);
										_pathBubbles[i]->_pieMenuState=0;										
									}
									//else if(!disables2[0][1]&&sectorID[1]==2) 
									//{
									//	_pathBubbles[i]->menuSelection_1(pos_menu, scenePos_menu,2);
									//	_pathBubbles[i]->_pieMenuState=0;										
									//}
								}
								break;	
								case 1:   if(!disables[1])
								{
									if(!disables2[1][0]&&sectorID[1]==0) 
									{ 
										_pathBubbles[i]->menuSelection_1(pos_menu, scenePos_menu,10);
										_pathBubbles[i]->_pieMenuState=0;	
									}
									else if(!disables2[1][1]&&sectorID[1]==1) 
									{ 
										_pathBubbles[i]->menuSelection_1(pos_menu, scenePos_menu,11);
										_pathBubbles[i]->_pieMenuState=0;	
									}
									else if(!disables2[1][1]&&sectorID[1]==2)
									{
										_pathBubbles[i]->menuSelection_1(pos_menu, scenePos_menu,12);
										_pathBubbles[i]->_pieMenuState=0;	
									}											
								}
								break;	
								 case 2:	if(!disables[2])
								{
									if(!disables2[2][0]&&sectorID[1]==0) 
				                    {
						                _pathBubbles[i]->menuSelection_1(pos_menu, scenePos_menu,20);
										_pathBubbles[i]->_pieMenuState=0;
						            }																
				                    else if(!disables2[2][1]&&sectorID[1]==1) 
				               		{  
										_pathBubbles[i]->menuSelection_1(pos_menu, scenePos_menu,21);
								        _pathBubbles[i]->_pieMenuState=0;									    				
									}		
									else if(!disables2[2][1]&&sectorID[1]==2) 
				               		{  
										_pathBubbles[i]->menuSelection_1(pos_menu, scenePos_menu,22);
								        _pathBubbles[i]->_pieMenuState=0;									    				
									}		
								}		
								break;			
						     }			   
						}
				   }
					else if(_pathBubbles[i]->menuType==2)
					{
						if(_pathBubbles[i]->_pieMenu->inPie(event->scenePos()))
						{
							int sectorID=_pathBubbles[i]->_pieMenu->getSectorID(event->scenePos());							
							vector<bool> disables=_pathBubbles[i]->_pieMenu->getDisables();
							switch(sectorID)
							{
								case 3: if(!disables[sectorID])//pen //"subGraph"
						        {
									_pathBubbles[i]->menuSelection(pos_menu, scenePos_menu,39);
								}
							    break;		
								case 4: if(!disables[sectorID]) //delete
						        {
									_pathBubbles[i]->menuSelection(pos_menu, scenePos_menu,49);
								}
							    break;	
						   }	
					   }					
					   else if(_pathBubbles[i]->_pieMenu->inPie2(event->scenePos()))
					   {
						vector<int> sectorID=_pathBubbles[i]->_pieMenu->getSectorID2(event->scenePos());	
						vector<bool> disables=_pathBubbles[i]->_pieMenu->getDisables();
						vector<vector<bool>> disables2=_pathBubbles[i]->_pieMenu->getDisables2();
						switch(sectorID[0])
						{   
							case 0:	if(!disables[0]) 
							{
								if(!disables2[0][0]&&sectorID[1]==0) 
								{
										_pathBubbles[i]->menuSelection(pos_menu, event->scenePos(),0);
										_pathBubbles[i]->_pieMenuState=0;										
								}
								else if(!disables2[0][1]&&sectorID[1]==1) 
								{
										_pathBubbles[i]->menuSelection(pos_menu, event->scenePos(),1);
										_pathBubbles[i]->_pieMenuState=0;										
								}	
								else if(!disables2[0][1]&&sectorID[1]==2) 
								{
										_pathBubbles[i]->menuSelection(pos_menu, event->scenePos(),2);
										_pathBubbles[i]->_pieMenuState=0;										
								}	
							}
							break;	
							case 1:  
								if(!disables[1])
								{
									if(!disables2[1][0]&&sectorID[1]==0) 
									{ 
										_pathBubbles[i]->menuSelection(pos_menu, event->scenePos(),10);
										_pathBubbles[i]->_pieMenuState=0;	
									}
									else if(!disables2[1][1]&&sectorID[1]==1) 
									{ 
										_pathBubbles[i]->menuSelection(pos_menu, event->scenePos(),11);
										_pathBubbles[i]->_pieMenuState=0;	
									}
									else if(!disables2[1][1]&&sectorID[1]==2)
									{
										_pathBubbles[i]->menuSelection(pos_menu, event->scenePos(),12);
										_pathBubbles[i]->_pieMenuState=0;	
									}											
								}
								break;	
								case 2:	if(!disables[2])
								{
									if(!disables2[2][0]&&sectorID[1]==0) 
				                    {
						                _pathBubbles[i]->menuSelection(pos_menu, event->scenePos(),20);
										_pathBubbles[i]->_pieMenuState=0;
						            }																
				                    else if(!disables2[2][1]&&sectorID[1]==1) 
				               		{  
										_pathBubbles[i]->menuSelection(pos_menu, event->scenePos(),21);
								        _pathBubbles[i]->_pieMenuState=0;									    				
									}		
									else if(!disables2[2][1]&&sectorID[1]==2) 
				               		{  
										_pathBubbles[i]->menuSelection(pos_menu, event->scenePos(),22);
								        _pathBubbles[i]->_pieMenuState=0;									    				
									}	
									else if(!disables2[2][1]&&sectorID[1]==3) 
				               		{  
										_pathBubbles[i]->menuSelection(pos_menu, event->scenePos(),25);
								        _pathBubbles[i]->_pieMenuState=0;									    				
									}	
								}		
								break;			
							}			   
						 }
				 }
			}	
		}//if mouse
	}//if left bubble
	
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


QPoint OpenGLScene::getSonPosition(TextBubble2 *newItem, ItemBase *parent, int x, int y, int width, int height)
{   //before newitem is added into group    
	int Width = this->width();
	int Height = this->height();
	
	//get all group member
	vector<set<int> > familySets;
	class Group *B= new Group();
	set<int> initSet;
	int initID;
	QList<ItemBase *> list=getGroupMembers(parent);
	int pID=list.indexOf((ItemBase*)parent);

	ItemBase* init=(ItemBase*)(newItem);	
	list.append(init);
    initID=list.size()-1;
		
	QRectF sRect = parent->sceneBoundingRect();
	QRectF rRect = parent->realRect();
	QRectF spRect = QRectF(sRect.x() + (sRect.width()-rRect.width())/2, sRect.y() + (sRect.height()-rRect.height())/2, rRect.width(), rRect.height());	
		
	vector<QRectF> rectList = B->assignRectList(list);			
	if(rectList[initID].x()<spRect.x()+spRect.width())
	     rectList[initID]= QRectF(spRect.x()+spRect.width(), rectList[initID].y(), rectList[initID].width(), rectList[initID].height());

	//int pIDF=B->getFamilyID(pID, familySets);
	bool flag=false;
	//test location
	{
		initSet.clear();		
		for(int i=0; i<list.size(); i++)
		{
			if(i!=initID)
			{
			   initSet.insert(i);			   
			}
		}		
		familySets.push_back(initSet);		
		set<int> initSetF;
		initSetF.insert(0);
		set<int> tSet;
		tSet.insert(initID); 
		familySets.push_back(tSet);

		vector<int> parentList = _manager->getParentList(list, familySets);		
		vector<QRectF> bList;
		bList = B->lineUpDisplay(SUBBUBBLECREATED, list, parentList, initSetF, list.size(), familySets, rectList, list[0]->screenWidth, list[0]->screenHeight, QRectF(-10000000,-10000000,-10000000,-10000000));//list should include initSet	
		float disx = fabs(bList[initID].x() - rectList[initID].x());
		float disy = fabs(bList[initID].y() - rectList[initID].y());
		if( B->isbridgeBroken(rectList) || (disx<rectList[initID].width()/2.0 && disy<rectList[initID].height()/2.0) )
		{
			//float x = rectList[initID].x() == bList[initID].x()? list[initID]->pos().x() : float(bList[initID].x())+float(bList[initID].width())/2.0;
			//float y = rectList[initID].y() == bList[initID].y()? list[initID]->pos().y() : float(bList[initID].y())+float(bList[initID].height())/2.0;
			//list[initID]->setTargetPos( bList[initID].center());		
			list[initID]->mySetPos( bList[initID].center());		
			rectList[initID].moveCenter(bList[initID].center());
			flag=true;
		
		}
		if(!flag) //further adjust
		{
			int mini=initID;
			float min=1000000;
			float dis,Dis;
			bool oflag=false;
			//
			float lx=spRect.x()+spRect.width();
			QRectF initRect=rectList[initID];
			QRectF testRect=QRectF(lx, initRect.y(), initRect.x()+initRect.width()-lx, initRect.height());
			for(int i=0; i<list.size(); i++)
			{
				dis=1000000;
				if(i!=initID)
				{
				   if(B->overlaid(testRect, rectList[i]))
				   {   
				       float x1=rectList[i].x(), x2=rectList[i].x()+rectList[i].width();
					   float dis1=fabs(initRect.x()-x1),dis2=fabs(initRect.x()-x2);
					   if(dis1<dis2)
						   dis=x1-initRect.x();
					   else dis=x2-initRect.x();
					   oflag=true;
					   //the mose close x to rectList[initID].x()
				   }
				}
				if(fabs(dis)<=min)
			    {
					min=fabs(dis);
					Dis=dis;
					mini=i;
				}
			}	
			if(oflag)
			   rectList[initID]= QRectF(rectList[initID].x()+Dis, rectList[initID].y(), rectList[initID].width(), rectList[initID].height());
			
		}
	}

	//return QPoint( rectList[initID].x(), rectList[initID].y() );

	vector<QRectF> aList;
	if(!flag)
	{
		initSet.clear();
		familySets.clear();
		familySets=_manager->GroupToFamily(list);//, init);
		int initIDF=B->getFamilyID(initID, familySets);			
		initSet.insert(initIDF); 
		vector<int> parentList = _manager->getParentList(list, familySets);		
		vector<QRectF> aList=rectList;
		rectList = 	B->lineUpDisplay(SUBBUBBLECREATED, list, parentList, initSet, list.size(), familySets, rectList, list[0]->screenWidth, list[0]->screenHeight, QRectF(-10000000,-10000000,-10000000,-10000000));//list should include initSet

		//shift then according to parent
		QPointF tempPos= rectList[pID].center() - aList[pID].center();	    		
		//need to get this out and set target after all the bubble location are settled
		for(int i=0; i<list.size(); i++)
		{
			//this->getMemberAt(i)->setTargetPos( QPointF(rectList[i].x()-getMemberAt(i)->realRect().x(),rectList[i].y()-getMemberAt(i)->realRect().y()) );		
		    if(rectList[i].x()>-10000 && rectList[i].x()<10000 && (aList[i]!=rectList[i] || i==initID  ))
			{
					//QPointF center(AllRectList[i].x()+AllRectList[i].width()/2)			
					//float x = rectList[i].x() == aList[i].x()? list[i]->pos().x() : float(rectList[i].x())+float(rectList[i].width())/2.0;
					//float y = rectList[i].y() == aList[i].y()? list[i]->pos().y() : float(rectList[i].y())+float(rectList[i].height())/2.0;
				    float x = float(rectList[i].x())+float(rectList[i].width())/2.0;
					float y = float(rectList[i].y())+float(rectList[i].height())/2.0;
					//if(setPos)
					//	list[i]->mySetPos( QPointF(x, y));		
					//else 
					list[i]->mySetPos( QPointF(x, y) - tempPos);	
					rectList[i].moveCenter(QPointF(x, y) - tempPos);
			}		   
		}
	}
	_manager->lineUpDisplayGroups(list, rectList, true); 
	delete B;
	return QPoint( rectList[initID].x(), rectList[initID].y() );

   
}


QPoint OpenGLScene::getGroupedBubblePosition(ItemBase *newItem, ItemBase *parent, int x, int y, int width, int height)
{   //before newitem is added into group    
	int Width = this->width();
	int Height = this->height();
	
	//get all group member
	vector<set<int> > familySets;
	class Group *B= new Group();
	set<int> initSet;
	int initID;
	QList<ItemBase *> list=getGroupMembers(parent);
	int pID=list.indexOf((ItemBase*)parent);

	ItemBase* init=(ItemBase*)(newItem);	
	list.append(init);
    initID=list.size()-1;
		
	vector<QRectF> rectList = B->assignRectList(list);	

	initSet.clear();		
	for(int i=0; i<list.size(); i++)
	{
		if(i!=initID)
		{
			initSet.insert(i);			   
		}
	}		
	familySets.push_back(initSet);		
	set<int> initSetF;
	initSetF.insert(0);
	set<int> tSet;
	tSet.insert(initID); 
	familySets.push_back(tSet);

	vector<int> parentList = _manager->getParentList(list, familySets);		
	vector<QRectF> bList;
	bList = B->lineUpDisplay(SUBBUBBLECREATED, list, parentList, initSetF, list.size(), familySets, rectList, list[0]->screenWidth, list[0]->screenHeight, QRectF(-10000000,-10000000,-10000000,-10000000));//list should include initSet	
	float disx = fabs(bList[initID].x() - rectList[initID].x());
	float disy = fabs(bList[initID].y() - rectList[initID].y());
	//if( B->isbridgeBroken(rectList) || (disx<rectList[initID].width()/2.0 && disy<rectList[initID].height()/2.0) )
	{
		//float x = rectList[initID].x() == bList[initID].x()? list[initID]->pos().x() : float(bList[initID].x())+float(bList[initID].width())/2.0;
		//float y = rectList[initID].y() == bList[initID].y()? list[initID]->pos().y() : float(bList[initID].y())+float(bList[initID].height())/2.0;
			
		list[initID]->setTargetPos( bList[initID].center());					
		rectList[initID].moveCenter(bList[initID].center());						
	}		
	_manager->lineUpDisplayGroups(list, rectList, true); //temp keqin	
	return QPoint( rectList[initID].center().x(), rectList[initID].center().y() );   
}



QPoint OpenGLScene::getSonPosition(PathBubble1 *newItem, PathBubble1 *parent, int x, int y, int width, int height)
{   //before newitem is added into group
    //_manager->adjustOverlaid( parent, QRectF(-100000,-100000,-100000,-100000), true);
    //_manager->tightenGroupMembers(parent, QRectF(-100000,-100000,-100000,-100000), false, true);    

	int Width = this->width();
	int Height = this->height();


	//get all group member
	vector<set<int> > familySets;
	class Group *B= new Group();
	set<int> initSet;
	int initID;
	QList<ItemBase *> list=getGroupMembers(parent);

	int pID=list.indexOf((ItemBase*)parent);

	ItemBase* init=(ItemBase*)(newItem);	
	list.append(init);

    initID=list.size()-1;
	
	
	QRectF sRect = parent->sceneBoundingRect();
	QRectF rRect = parent->realRect();
	QRectF spRect = QRectF(sRect.x() + (sRect.width()-rRect.width())/2, sRect.y() + (sRect.height()-rRect.height())/2, rRect.width(), rRect.height());	
		
	vector<QRectF> rectList = B->assignRectList(list);			
	if(rectList[initID].x()<spRect.x()+spRect.width())
	     rectList[initID]= QRectF(spRect.x()+spRect.width(), rectList[initID].y(), rectList[initID].width(), rectList[initID].height());

	//int pIDF=B->getFamilyID(pID, familySets);
	bool flag=false;

	//test location
	{
		initSet.clear();		
		for(int i=0; i<list.size(); i++)
		{
			if(i!=initID)
			{
			   initSet.insert(i);			   
			}
		}		
		familySets.push_back(initSet);		
		set<int> initSetF;
		initSetF.insert(0);
		set<int> tSet;
		tSet.insert(initID); 
		familySets.push_back(tSet);

		vector<int> parentList = _manager->getParentList(list, familySets);		
		vector<QRectF> bList;
		bList = B->lineUpDisplay(SUBBUBBLECREATED, list, parentList, initSetF, list.size(), familySets, rectList, list[0]->screenWidth, list[0]->screenHeight, QRectF(-10000000,-10000000,-10000000,-10000000));//list should include initSet	
		float disx = fabs(bList[initID].x() - rectList[initID].x());
		float disy = fabs(bList[initID].y() - rectList[initID].y());
		if( B->isbridgeBroken(rectList) || (disx<rectList[initID].width()/2.0 && disy<rectList[initID].height()/2.0) )
		{
			//float x = rectList[initID].x() == bList[initID].x()? list[initID]->pos().x() : float(bList[initID].x())+float(bList[initID].width())/2.0;
			//float y = rectList[initID].y() == bList[initID].y()? list[initID]->pos().y() : float(bList[initID].y())+float(bList[initID].height())/2.0;
			list[initID]->mySetPos( bList[initID].center());		
			rectList[initID].moveCenter(bList[initID].center());
			flag=true;
		
		}
		if(!flag) //further adjust
		{
			int mini=initID;
			float min=1000000;
			float dis,Dis;
			bool oflag=false;
			//
			float lx=spRect.x()+spRect.width();
			QRectF initRect=rectList[initID];
			QRectF testRect=QRectF(lx, initRect.y(), initRect.x()+initRect.width()-lx, initRect.height());
			for(int i=0; i<list.size(); i++)
			{
				dis=1000000;
				if(i!=initID)
				{
				   if(B->overlaid(testRect, rectList[i]))
				   {   
				       float x1=rectList[i].x(), x2=rectList[i].x()+rectList[i].width();
					   float dis1=fabs(initRect.x()-x1),dis2=fabs(initRect.x()-x2);
					   if(dis1<dis2)
						   dis=x1-initRect.x();
					   else dis=x2-initRect.x();
					   oflag=true;
					   //the mose close x to rectList[initID].x()
				   }
				}
				if(fabs(dis)<=min)
			    {
					min=fabs(dis);
					Dis=dis;
					mini=i;
				}
			}	
			if(oflag)
			   rectList[initID]= QRectF(rectList[initID].x()+Dis, rectList[initID].y(), rectList[initID].width(), rectList[initID].height());
			
		}
	}

	vector<QRectF> aList;
	if(!flag)
	{
		initSet.clear();
		familySets.clear();
		familySets=_manager->GroupToFamily(list);//, init);
		int initIDF=B->getFamilyID(initID, familySets);			
		initSet.insert(initIDF); 
		vector<int> parentList = _manager->getParentList(list, familySets);		
		vector<QRectF> aList=rectList;
		rectList = 	B->lineUpDisplay(SUBBUBBLECREATED, list, parentList, initSet, list.size(), familySets, rectList, list[0]->screenWidth, list[0]->screenHeight, QRectF(-10000000,-10000000,-10000000,-10000000));//list should include initSet

		//shift then according to parent
		QPointF tempPos= rectList[pID].center() - aList[pID].center();	    		
		//need to get this out and set target after all the bubble location are settled
		for(int i=0; i<list.size(); i++)
		{
			//this->getMemberAt(i)->setTargetPos( QPointF(rectList[i].x()-getMemberAt(i)->realRect().x(),rectList[i].y()-getMemberAt(i)->realRect().y()) );		
		    if(rectList[i].x()>-10000 && rectList[i].x()<10000 && (aList[i]!=rectList[i] || i==initID  ))
			{
					//QPointF center(AllRectList[i].x()+AllRectList[i].width()/2)			
					//float x = rectList[i].x() == aList[i].x()? list[i]->pos().x() : float(rectList[i].x())+float(rectList[i].width())/2.0;
					//float y = rectList[i].y() == aList[i].y()? list[i]->pos().y() : float(rectList[i].y())+float(rectList[i].height())/2.0;
				    float x = float(rectList[i].x())+float(rectList[i].width())/2.0;
					float y = float(rectList[i].y())+float(rectList[i].height())/2.0;
					//if(setPos)
					//	list[i]->mySetPos( QPointF(x, y));		
					//else 
					list[i]->mySetPos( QPointF(x, y) - tempPos);	
					rectList[i].moveCenter(QPointF(x, y) - tempPos);
			}		   
		}
	}
	_manager->lineUpDisplayGroups(list, rectList, true); 
	delete B;
	return QPoint( rectList[initID].x(), rectList[initID].y() );
}

vector<expressionBubble *> OpenGLScene::extractExpressionBubblesinGroup(ItemBase * item)
{
	vector<expressionBubble *> outputList;
	QList<ItemBase *> mlist = getGroupMembers(item);
	for(int i=0; i<mlist.size(); i++) 
	{
		if(mlist[i]->getType() != EXPRESSIONBUBBLE)
			continue;
		expressionBubble* pbubble = dynamic_cast<expressionBubble *>(mlist[i]);
		outputList.push_back(pbubble);
	}
	return outputList;
}


vector<PathBubble1 *>  OpenGLScene::extractPathBubblesinGroup(ItemBase * item)
{
	vector<PathBubble1 *> outputList;
	QList<ItemBase *> mlist = getGroupMembers(item);
	for(int i=0; i<mlist.size(); i++) 
	{
		if(mlist[i]->getType() != SUBPATHBUBBLE1 && mlist[i]->getType() != PATHBUBBLE1)
			continue;
		PathBubble1* pbubble = dynamic_cast<PathBubble1*>(mlist[i]);
		outputList.push_back(pbubble);
	}
	return outputList;
}


void OpenGLScene::clearSharedSets(ItemBase *item)
{
	vector<PathBubble1 *> mlist = extractPathBubblesinGroup(item);
	vector <vector <int>> toErase;
	for(int i=0; i<mlist.size(); i++) 	
	{
		int pid=mlist[i]->m_pathwayID;		
		for(int k=0; k< getGroup(mlist[i])->sharedSets.size(); k++)
		{
			 for(set < vector <int> > ::iterator it = getGroup(mlist[i])->sharedSets[k].begin(); it!=getGroup(mlist[i])->sharedSets[k].end(); it++)
			 {
				 vector <int>  item=*it;				 
				 if(item[0]==pid)
				 {
					 toErase.push_back(item);
				 }			 
			 }
			 for(int j=0; j<toErase.size(); j++)
				 getGroup(mlist[i])->sharedSets[k].erase(toErase[j]);
		}
	}
}

void OpenGLScene::clearExpressedSets(ItemBase *item)
{
	vector<PathBubble1 *> mlist = extractPathBubblesinGroup(item);
	vector <vector <int>> toErase;
	for(int i=0; i<mlist.size(); i++) 	
	{
		int pid=mlist[i]->m_pathwayID;		
		for(int k=0; k< getGroup(mlist[i])->expressedSets.size(); k++)
		{
			 for(set < vector <int> > ::iterator it = getGroup(mlist[i])->expressedSets[k].begin(); it!=getGroup(mlist[i])->expressedSets[k].end(); it++)
			 {
				 vector <int>  item=*it;				 
				 if(item[0]==pid)
				 {
					 toErase.push_back(item);
				 }			 
			 }
			 for(int j=0; j<toErase.size(); j++)
				 getGroup(mlist[i])->expressedSets[k].erase(toErase[j]);
		}
	}
}


void OpenGLScene::cleanSelected(ItemBase *item)
{
	    clearSharedSets(item);
		clearExpressedSets(item);

	    QList<ItemBase *> mList=getGroupMembers(item);
		for(int i=0; i<mList.size(); i++) 
		{
			if( mList[i]->getType()==PATHBUBBLE1 || mList[i]->getType()==SUBPATHBUBBLE1 )
			{
				PathBubble1* pbubble = dynamic_cast<PathBubble1*> (mList[i]);	
				pbubble->highlighted.clear(); 
				pbubble->searched.clear(); 

				int csize=pbubble->sharedProtein.size();				
				pbubble->sharedProtein.clear();			pbubble->sharedProtein.resize(csize);
				pbubble->sharedSmallMolecule.clear();	pbubble->sharedSmallMolecule.resize(csize);
				pbubble->sharedComplex.clear();         pbubble->sharedSmallMolecule.resize(csize);
				pbubble->sharedDna.clear();             pbubble->sharedSmallMolecule.resize(csize);
				pbubble->sharedReaction.clear();        pbubble->sharedSmallMolecule.resize(csize);
				pbubble->sharedPhysicalEntity.clear();  pbubble->sharedSmallMolecule.resize(csize);
				pbubble->sharedCompartment.clear();     pbubble->sharedSmallMolecule.resize(csize);
				pbubble->sharedPathway.clear();         pbubble->sharedSmallMolecule.resize(csize);
				

				csize=pbubble->linkedProtein.size();	
				pbubble->linkedProtein.clear();         
				pbubble->linkedSmallMolecule.clear();
				pbubble->linkedComplex.clear(); 
				pbubble->linkedDna.clear();  
				pbubble->linkedReaction.clear(); 
				pbubble->linkedPhysicalEntity.clear();  
				pbubble->linkedCompartment.clear();  
				pbubble->linkedPathway.clear();  
				pbubble->linkedEdge.clear();  

				pbubble->linkedProtein.resize(csize);
				pbubble->linkedSmallMolecule.resize(csize);
				pbubble->linkedComplex.resize(csize); 
				pbubble->linkedDna.resize(csize);  
				pbubble->linkedReaction.resize(csize); 
				pbubble->linkedPhysicalEntity.resize(csize);  
				pbubble->linkedCompartment.resize(csize);  
				pbubble->linkedPathway.resize(csize);  
				pbubble->linkedEdge.resize(csize);  

				csize=pbubble->differedProtein.size();	
				pbubble->differedProtein.clear();
				pbubble->differedSmallMolecule.clear();
				pbubble->differedComplex.clear(); 
				pbubble->differedDna.clear();  
				pbubble->differedReaction.clear(); 
				pbubble->differedPhysicalEntity.clear();  
				pbubble->differedCompartment.clear();  
				pbubble->differedPathway.clear();  

				pbubble->differedProtein.resize(csize);
				pbubble->differedSmallMolecule.resize(csize);
				pbubble->differedComplex.resize(csize); 
				pbubble->differedDna.resize(csize);  
				pbubble->differedReaction.resize(csize); 
				pbubble->differedPhysicalEntity.resize(csize);  
				pbubble->differedCompartment.resize(csize);  
				pbubble->differedPathway.resize(csize);  


				csize=pbubble->expressedProtein.size();	
				pbubble->expressedProtein.clear();
				pbubble->expressedSmallMolecule.clear();
				pbubble->expressedComplex.clear(); 
				pbubble->expressedDna.clear();  
				pbubble->expressedReaction.clear(); 
				pbubble->expressedPhysicalEntity.clear();  
				pbubble->expressedCompartment.clear();  
				pbubble->expressedPathway.clear();  

				pbubble->expressedProtein.resize(csize);
				pbubble->expressedSmallMolecule.resize(csize);
				pbubble->expressedComplex.resize(csize); 
				pbubble->expressedDna.resize(csize);  
				pbubble->expressedReaction.resize(csize); 
				pbubble->expressedPhysicalEntity.resize(csize);  
				pbubble->expressedCompartment.resize(csize);  
				pbubble->expressedPathway.resize(csize);  
			}
		}				

}

void OpenGLScene::clearUndoRedoRecord(ItemBase *item)				
{
			QList<ItemBase *> mList=getGroupMembers(item);
			for(int i=0; i<mList.size(); i++) 
			{
				if( mList[i]->getType()==PATHBUBBLE1 || mList[i]->getType()==SUBPATHBUBBLE1 )
				{
					PathBubble1* pbubble = dynamic_cast<PathBubble1*> (mList[i]);			
					pbubble->highlightedStored.clear();
					pbubble->currentHighlightedit=pbubble->highlightedStored.end();
					 
					pbubble->linkedStored.clear();
					pbubble->currentLinkedit=pbubble->linkedStored.end();

					pbubble->sharedStored.clear();
					pbubble->currentSharedit=pbubble->sharedStored.end();

					pbubble->linkedStored.clear();
					pbubble->currentLinkedit=pbubble->linkedStored.end();
					
				}
				
			}	
				
}

void OpenGLScene::clearColored(ItemBase *item)
{
    vector<PathBubble1 *> mList= extractPathBubblesinGroup(item);
    for(int i=0; i< mList.size(); i++) 
	{
	 	PathBubble1* pbubble = mList[i];				 	
	    pbubble->_itemColored.clear();			
					
	}
}			

void OpenGLScene::clearExpressed(ItemBase *item) //while updata graph
{
        QList<ItemBase *> mList=getGroupMembers(item);	 
		for(int i=0; i< mList.size(); i++) 
		{
	 		if( mList[i]->getType()==PATHBUBBLE1 || mList[i]->getType()==SUBPATHBUBBLE1 )
			{
				PathBubble1* pbubble = dynamic_cast<PathBubble1*> (mList[i]);
				int csize=pbubble->expressedProtein.size();	
				pbubble->expressedProtein.clear();
				pbubble->expressedSmallMolecule.clear();
				pbubble->expressedComplex.clear(); 
				pbubble->expressedDna.clear();  
				pbubble->expressedReaction.clear(); 
				pbubble->expressedPhysicalEntity.clear();  
				pbubble->expressedCompartment.clear();  
				pbubble->expressedPathway.clear();  

				pbubble->expressedProtein.resize(csize);
				pbubble->expressedSmallMolecule.resize(csize);
				pbubble->expressedComplex.resize(csize); 
				pbubble->expressedDna.resize(csize);  
				pbubble->expressedReaction.resize(csize); 
				pbubble->expressedPhysicalEntity.resize(csize);  
				pbubble->expressedCompartment.resize(csize);  
				pbubble->expressedPathway.resize(csize);  
				pbubble->getNodeToBePaint();
			}
			else if( mList[i]->getType()==EXPRESSIONBUBBLE )
			{
				expressionBubble * ebubble = dynamic_cast<expressionBubble *> (mList[i]);
				ebubble->clearExpressed();	
				ebubble->expressionCleared=true;
			}
		}		
}




/*
QPoint OpenGLScene::getSonPosition(PathBubble1 *newItem, PathBubble1 *parent, int x, int y, int width, int height)
{   //before newitem is added into group
     //_manager->adjustOverlaid( parent, QRectF(-100000,-100000,-100000,-100000), true);
     //_manager->tightenGroupMembers(parent, QRectF(-100000,-100000,-100000,-100000), false, true);    

	int Width = this->width();
	int Height = this->height();

	
	QList<ItemBase*> AList, HList, list; 
	int initID=-1;
	
	newItem->getAllPSPathBubble(newItem, AList );//getAllSubPathBubble(parent, list);
	newItem->getHigherPathBubble(newItem, HList);
	list=AList;
	
	int size= list.size();	
	for( int i = 0; i <size; i++ )
	{	
		if( (PathBubble1 *) list[i] == newItem)
			initID=i;		
	}
	
	if(initID==-1) 
	{
		initID=size;
		list.append( (ItemBase*) newItem);
	}

	QRectF sRect = parent->sceneBoundingRect();
	QRectF rRect = parent->realRect();
	QRectF spRect = QRectF(sRect.x() + (sRect.width()-rRect.width())/2, sRect.y() + (sRect.height()-rRect.height())/2, rRect.width(), rRect.height());	
	
	class Group *group= new Group();
	
	//add parent
	//list.append(parent);
	vector<QRectF> rectList = group->assignRectList(list);			
	if(rectList[initID].x()<spRect.x()+spRect.width())
	  rectList[initID]= QRectF(spRect.x()+spRect.width(), rectList[initID].y(), rectList[initID].width(), rectList[initID].height());
	
	
	for( int i = 0; i < list.size(); i++ )
	{
		if(list[i]!=newItem)
		//if( rectList[i].intersects(rectList[initID]) )
		{
			set<int> init; 
			//add bubble in higher level ancesters
			for( int i = 0; i < list.size(); i++ )
			{
				//in HList
				for( int j = 0; j < HList.size(); j++ )
				{
					if(list[i]==HList[j])
					{
						init.insert(i);								
						break;
					}
				}
			}
			//add itself
			init.insert(initID);
			//init.insert(list.size()-1); //parent

			vector<set<int> > familySets;
			for(int i=0; i<list.size(); i++)
			{
				set<int> tset;
				tset.insert(i);
				familySets.push_back(tset);	
			}	

			set<int> initF;	
			for(set<int>::iterator it=init.begin(); it!=init.end(); it++)
			{
				int fid=group->getFamilyID(*it,familySets);
				if(fid<0)
				{
				   //add new family
					set<int> newset; newset.insert(*it);
					familySets.push_back(newset);
					fid=familySets.size()-1;
					initF.insert(fid);
				}
				else initF.insert(fid);		
			}
			rectList=group->lineUpDisplay(list, initF, list.size(), familySets, rectList, list[0]->screenWidth, list[0]->screenHeight);	 			
			break;
			
		}
	}

	for(int i=0; i<list.size(); i++)
	{
		if(rectList[i].x()>-10000 && rectList[i].x()<10000)
			 //   list[i]->setTargetPos( QPointF(rectList[i].x()-list[i]->realRect().x(),rectList[i].y()-list[i]->realRect().y()) );						
			 list[i]->setTargetPos( rectList[i].center() );						
    }

	
	
	//arrange others
	vector<set<int> > familySets;
	set<int> currentF;
	for(int i=0; i<list.size(); i++)
	{
		currentF.insert(i);	
	}			
	familySets.push_back(currentF);	 //first added family
	int aStart=list.size();
	//get all group member
	QList<ItemBase *> mlist=getGroupMembers(parent);
	int msize=mlist.size();
	for(int i=0; i<mlist.size(); i++)
	{
		//top level
		set<int> tSet;
		if( (mlist[i]->getType()==PATHBUBBLE1 || mlist[i]->getType()==SUBPATHBUBBLE1) )
		{
			if(mlist[i]->toplevel)
			{
				PathBubble1 *pBubble= dynamic_cast <PathBubble1 *>(mlist[i]);
				if(pBubble!= parent->getTopPathBubble(parent))
				{
					pBubble->getAllPSPathBubble(pBubble, AList );	
					//append A list to list
				
					//and append familySets
					int start=list.size();
					int asize=AList.size();
					for(int j=0; j<AList.size(); j++)
					{
						tSet.insert(start+j);	
					}			
					list.append(AList);
					familySets.push_back(tSet);	 //first family
				}
			}
		}
		else 
		{
		   list.push_back(mlist[i]);		
		   tSet.insert(list.size()-1);
		   familySets.push_back(tSet);
		}
	}

	
	vector<QRectF> TList=group->assignRectList(list, false, aStart);	
	for(int j=0; j<TList.size(); j++)
	{
		rectList.push_back(TList[j]);
	}
	if(group->isOverlaid(rectList))
	{
		set<int> init; 	 
		init.insert(0);
        rectList=group->lineUpDisplay(list, init, list.size(), familySets, rectList, list[0]->screenWidth, list[0]->screenHeight);	 	
		for(int i=0; i<list.size(); i++)
	    {
			//list[i]->setTargetPos( QPointF(rectList[i].x()-list[i]->realRect().x(),rectList[i].y()-list[i]->realRect().y()) );						
			 list[i]->setTargetPos( rectList[i].center() );		
        }
	}
		
	//_manager->lineUpDisplayGroups(list, rectList, true);

	delete group;
	return QPoint( rectList[initID].x(), rectList[initID].y() );
}

*/

void OpenGLScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	if ( _wantsCompare )
	{
		//findCompareTarget( event->scenePos() );
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

	static bool itemsMoved = false; //keqin make it static
	bool flag0=itemsMoved;
	bool flag1=false;
	foreach (ItemBase *node, nodes) 
	{
		if (node->advance())
		{
			flag1=true;
			itemsMoved = true;
			this->_manager->itemPosUpdated(node);
		}
	}
    //if(flag0!=itemsMoved)
	//  this->_manager->updateItemsPos();

	if (!flag1)
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

/*void OpenGLScene::addCompare(QtBat *object)
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
*/

void OpenGLScene::orderViews()
{
	int length = 0;
	float width = 0, height = 0;
	//QList<QtBat*> batList[5];
	foreach (QGraphicsItem *item, this->items() )
	{
		/*QtBat *itemBase = dynamic_cast< QtBat* >(item);
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
			//int index = allbats->getIndexFromName( itemBase->getBat()->getname() );
			//batList[index/5].append( itemBase );
		}*/
		//height = item->boundingRect().height();
	}
	int curHeight = 300;
	for( int i = 0; i < 5; i++ )
	{
		int curWidth = 200;
		/*if( batList[i].count() == 0 )
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
		}*/
		curHeight += 180;
	}
}

void OpenGLScene::mapViews()
{
	float maxSpeed = -999;
	float minSpeed = 999;
	int length = 0;
	float width = 0, height = 0;
	//QList<QtBat*> batList[5];
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
			/*QtBat* tempBat = dynamic_cast<QtBat*>(item);
			if( maxSpeed < tempBat->getSpeed() )
			{
				maxSpeed = tempBat->getSpeed();
			}
			if( minSpeed > tempBat->getSpeed() )
			{
				minSpeed = tempBat->getSpeed();
			}
			int index = allbats->getIndexFromName( tempBat->getBat()->getname() );
			batList[index/5].append( tempBat );*/
		}
		//height = item->boundingRect().height();
	}
	int curHeight = 300;
	for( int i = 0; i < 5; i++ )
	{
		int multiple = 300;
		int lastX = 100;
		//if( batList[i].count() == 0 )
		{
			continue;
		}
		/*while( batList[i].count() != 0 )
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
		}*/
		curHeight += 180;
	}
	return;
}

ItemBase* OpenGLScene::openWebNoteBubblebyMenu(int type, ItemBase *item, QPointF scenePos)
{
        ItemBase* nitem=NULL;
		QList<ItemBase *> mlist=allMembers();	 
		for(int i=0; i<mlist.size(); i++) 
		{
			//int type=mlist[i]->getType();
			PathBubble1* pbubble = dynamic_cast<PathBubble1*>(mlist[i]);
			if ( mlist[i]->getType() == type )
			{
				QList<ItemBase *> glist=getGroupMembers(mlist[i]);
				if(glist.size()==1)
				{
					/*if(type==WEB)
					{
						nitem=getOpenManager()->openWebView(item, 200,scenePos.x(),scenePos.y(), ""); 		 
					}
					else if(type==SEARCHFIND)
						nitem=getOpenManager()->openSearchView(item, 200,scenePos.x(),scenePos.y(), ""); 		 
					else if(type==NOTE)
						nitem=getOpenManager()->openNote(item, 200,scenePos.x(),scenePos.y()); 
					else if(type==SETTINGS)
						nitem=getOpenManager()->openSettingsView(item, 200,scenePos.x(),scenePos.y(), ""); 
					else if(type==SEARCHSETTINGS)
					*/
					//WebBlock* sbubble =  dynamic_cast<WebBlock*>(item);
					mlist[i]->setVisible(true);
					//sbubble->_search->setVisible(true);
					mlist[i]->unGroup();
					//mlist[i]->mySetPos( contextMenuEvent->scenePos()); 				
					//flag=false;		
					nitem=mlist[i];
				}
			}
		}	

		if(nitem==NULL)
		{
			if(type==WEB)
				nitem=getOpenManager()->openWebView(item, 200,scenePos.x(),scenePos.y(), ""); 		 
			else if(type==SEARCHFIND)
				nitem=getOpenManager()->openSearchView(item, 200,scenePos.x(),scenePos.y(), ""); 		 
			else if(type==NOTE)
				nitem=getOpenManager()->openNote(item, 200,scenePos.x(),scenePos.y()); 
			else if(type==SETTINGS)
				nitem=getOpenManager()->openSettingsView(item, 200,scenePos.x(),scenePos.y(), ""); 
			else if(type==SEARCHSETTINGS)
				nitem=getOpenManager()->openSearchSettingsView(item, 200,scenePos.x(),scenePos.y(), ""); 
			
		}
		if(item==NULL)
		{
			QPointF pos;
			float x=scenePos.x(), y=scenePos.y();
			pos = this->_manager->getPosition(nitem, x, y, nitem ->sceneBoundingRect().width(), nitem ->sceneBoundingRect().height());
			if(pos.x()!=-1 && pos.y()!=-1)
			{
				x = pos.x();
				y = pos.y();	
			}
			nitem->setTargetPos(QPointF(x + nitem->sceneBoundingRect().width()/2, y + nitem->sceneBoundingRect().height()/2)); 
		}
		else 
		{
			QPointF pos = getGroupedBubblePosition(nitem,item, nitem->sceneBoundingRect().x(), nitem->sceneBoundingRect().y(), nitem->sceneBoundingRect().width(), nitem->sceneBoundingRect().height());			
			addToGroup( nitem, item, false);
		}
		return nitem;
}


void OpenGLScene::openPathBubblebyMenu(QString name, ItemBase *item, QPointF scenePos)
{
	if( name.size() > 0 )
		{		
			ItemBase* nitem;
	        nitem = NULL;					
			//emit prePathBubbleStarted(QRectF( contextMenuEvent->scenePos().x(), contextMenuEvent->scenePos().y(), 100, 100));
			//emit pathBubbleStarted(QRectF( contextMenuEvent->scenePos().x(), contextMenuEvent->scenePos().y(), 100, 100));
			bool opened = getOpenManager()->openPathwayByPath(nitem, 200, scenePos.x(), scenePos.y(),name);			
			if(opened)			
			{
				PathBubble1* pBubble = dynamic_cast<PathBubble1*>(nitem);				
				if(item==NULL)
				{
						QPointF pos = this->_manager->getPosition(pBubble, pBubble->sceneBoundingRect().x(), pBubble->sceneBoundingRect().y(), pBubble->sceneBoundingRect().width(), pBubble->sceneBoundingRect().height());						
						float x= scenePos.x(), y=scenePos.y();
						if(pos.x()!=-1 && pos.y()!=-1)
						{
						   x = pos.x();
						   y = pos.y();	
						}
						pBubble->setTargetPos(QPointF(x + pBubble->sceneBoundingRect().width()/2, y + pBubble->sceneBoundingRect().height()/2)); 	
				}
				else 
				{
					QPointF pos = getGroupedBubblePosition(pBubble,item, pBubble->sceneBoundingRect().x(), pBubble->sceneBoundingRect().y(), pBubble->sceneBoundingRect().width(), pBubble->sceneBoundingRect().height());
					/*float x= scenePos.x(), y=scenePos.y();
					if(pos.x()!=-1 && pos.y()!=-1)
					{
					   x = pos.x();
					   y = pos.y();	
					}
					pBubble->setTargetPos(QPointF(x, y)); */
					addToGroup( pBubble, item, false);
				}		
			}
		}
}

ItemBase *OpenGLScene::findAPathBubbleinGroup(ItemBase *init)
{
	ItemBase *pItem=NULL;
	if(init->getType()==PATHBUBBLE1 || init->getType()== SUBPATHBUBBLE1)
	{
	    return init;
	}
    QList<ItemBase *> mList = getGroupMembers(init);
		
	for(int i=0; i<mList.size(); i++)
	{
		if(mList[i]->getType()==PATHBUBBLE1 || mList[i]->getType()== SUBPATHBUBBLE1)
		{
			pItem=mList[i];
			break;
		}			
	}
    return pItem;
}

ItemBase *OpenGLScene::findAExpressionBubbleinGroup(ItemBase *init)
{
	ItemBase *pItem=NULL;
	if(init->getType() == EXPRESSIONBUBBLE)
	{
	    return init;
	}
    QList<ItemBase *> mList = getGroupMembers(init);
		
	for(int i=0; i<mList.size(); i++)
	{
		if(mList[i]->getType()==EXPRESSIONBUBBLE)
		{
			pItem=mList[i];
			break;
		}			
	}
    return pItem;
}

PathBubble1 *OpenGLScene::findAPathBubbleinGroup(Group *B)
{
	PathBubble1 *pItem=NULL;
	QList<ItemBase *> mList=B->getMembers();
	for(int i=0; i<mList.size(); i++)
	{
		if(mList[i]->getType()==PATHBUBBLE1 || mList[i]->getType()== SUBPATHBUBBLE1)
		{
			pItem=dynamic_cast<PathBubble1 *> (mList[i]);
			break;
		}			
	}
    return pItem;
}

void OpenGLScene::dropDownMenu0(QGraphicsSceneContextMenuEvent * contextMenuEvent, ItemBase *item)
{
	QMenu menu;
	QMenu *loadMenu;
	QMenu *pathwayMenu;	
	QMenu *searchMenu;		
	QMenu *editMenu;
	QMenu *deleteMenu;

	QMenu *sceneMenu;	
	QMenu *viewMenu;	
	QMenu *shareMenu;
	QMenu *diffMenu;	
	QMenu *linkMenu;	

	QMenu *expressionMenu;
	QMenu *colorMenu;	
	QMenu *toolMenu;	
	QMenu *sketchMenu;	
	QMenu *queryMenu;	

	QAction *loadExpressionAction,*webAction,*loadNoteAction,*G2Pathway,*BMPPathway,*TGFPathway,*shareAction,*diffAction,*linkAction,
		*expressionExpressedAction,*searchSettingsAction,*colorUpAction, *colorEvenAction,*colorDownAction,*toolUndoAction,
		*toolRedoAction,*graphResertAction,*graphReloadAction, *sketchAction,*queryAction,*delAction,*delGroupAction,*removeAction, 
		*saveScene, *recoverScene, *searchPathwayAction,*settingsAction,*selectedAction;

	bool loadExpressionActionflag,webActionflag,loadNoteActionflag,G2Pathwayflag,BMPPathwayflag,TGFPathwayflag,shareActionflag,diffActionflag,linkActionflag,
		expressionExpressedActionflag,searchSettingsActionflag,colorUpActionflag, colorEvenActionflag,colorDownActionflag,toolUndoActionflag,
		toolRedoActionflag,graphResertActionflag,graphReloadActionflag, sketchActionflag,queryActionflag,delActionflag,delGroupActionflag,removeActionflag, 
		saveSceneflag, recoverSceneflag, searchPathwayActionflag,settingsActionflag,selectedActionflag;

	bool sceneMenuflag, viewMenuflag, shareMenuflag, diffMenuflag, linkMenuflag, expressionMenuflag, colorMenuflag, toolMenuflag, sketchMenuflag, queryMenuflag, deleteMenuflag, searchMenuflag, editMenuflag, loadMenuflag, pathwayMenuflag;	

	loadExpressionActionflag=webActionflag=loadNoteActionflag=G2Pathwayflag=BMPPathwayflag=TGFPathwayflag=shareActionflag=diffActionflag=linkActionflag=
		expressionExpressedActionflag=searchSettingsActionflag=colorUpActionflag= colorEvenActionflag=colorDownActionflag=toolUndoActionflag=
		toolRedoActionflag=graphResertActionflag=graphReloadActionflag= sketchActionflag=queryActionflag=delActionflag=delGroupActionflag=removeActionflag= 
		saveSceneflag= recoverSceneflag= searchPathwayActionflag=settingsActionflag=selectedActionflag=true;

	sceneMenuflag= viewMenuflag= shareMenuflag= diffMenuflag= linkMenuflag= expressionMenuflag= colorMenuflag= toolMenuflag= sketchMenuflag= queryMenuflag= deleteMenuflag = searchMenuflag = editMenuflag = loadMenuflag = pathwayMenuflag = true;

	QString ss("QMenu::item {  background-color: black; border: 3px solid black; color: white }"); // Use background-color instead of background
    menu.setStyleSheet(ss);
	menu.setMinimumWidth(70);	



    bool toplevel=-1;
	if(item!=NULL && (item->getType()==PATHBUBBLE1 || item->getType()==SUBPATHBUBBLE1))
	{	
		PathBubble1 *pBubble = dynamic_cast<PathBubble1 *> (item);
		toplevel=pBubble->toplevel; //contextMenuEvent
	}
	
	PathBubble1 *pBubble=NULL;
	expressionBubble *eBubble=NULL;
	ItemBase *pItem=NULL, *eItem=NULL;
	if(item!=NULL)
	{   
		//a bubble is clicked
		if(item->getType()==PATHBUBBLE1 || item->getType()==SUBPATHBUBBLE1)
	        pItem=item;
		else 
			pItem = findAPathBubbleinGroup(item);

		if(item->getType()==EXPRESSIONBUBBLE)
	        eItem=item;
		else 
			eItem = findAExpressionBubbleinGroup(item);

		if(pItem!=NULL)
		{
			pBubble = dynamic_cast<PathBubble1 *> (pItem);
			toplevel=pBubble->toplevel; //contextMenuEvent
		}
		if(eItem!=NULL)
		{
			eBubble = dynamic_cast<expressionBubble *> (eItem);
		}
	}
	
	//selete action to be invisible	
	if(item!=NULL)
	{	  
		sceneMenuflag=false;
        if(pBubble!=NULL)
		{
			
		}
		else 
		{
			searchMenuflag=false;
		    editMenuflag=false;
			sketchActionflag=false;
		    queryActionflag=false;
			settingsActionflag=false;
		    if(eBubble==NULL)				
			{	
				QAction *searchPathwayAction = toolMenu->addAction("Quick Find");		        
			}
		}
	}
	else
	{
	    sketchActionflag=false;
		queryActionflag=false; //e-gift query
		deleteMenuflag=false;
	}


	if(loadMenuflag)
	   loadMenu = menu.addMenu("Load");
	
	if(pathwayMenuflag)
		pathwayMenu = loadMenu->addMenu("Pathway\t");   	
	if(loadExpressionActionflag)
		loadExpressionAction = loadMenu->addAction("Expression");	
	if(webActionflag)
		webAction = loadMenu->addAction("Web query");	
	if(loadNoteActionflag)
		loadNoteAction = loadMenu->addAction("Note");
	
	
	        if(G2Pathwayflag)
				G2Pathway = pathwayMenu->addAction("MitoticG2-G2");
			if(BMPPathwayflag)
				BMPPathway = pathwayMenu->addAction("BMP");
			if(TGFPathwayflag)
				TGFPathway = pathwayMenu->addAction("TGF");

  
	if(searchMenuflag)				
	searchMenu = menu.addMenu("Search");
		
	if(shareActionflag)
		shareAction = searchMenu->addAction("Share");
	if(diffActionflag)
		diffAction = searchMenu->addAction("Difference");
	if(linkActionflag)
		linkAction = searchMenu->addAction("Edge links");
	if(expressionExpressedActionflag)
		expressionExpressedAction =searchMenu->addAction("Expression");	
	if(searchMenuflag)
		searchMenu->addSeparator();
	if(searchSettingsActionflag)
		searchSettingsAction =searchMenu->addAction("Settings");	


	selectedAction = menu.exec( contextMenuEvent->screenPos() );
	

			
	if ( selectedAction == loadNoteAction )
	{
	     openWebNoteBubblebyMenu(NOTE, item, contextMenuEvent->scenePos());	
	}
	else if( selectedAction == webAction )
	{
		openWebNoteBubblebyMenu(WEB, item, contextMenuEvent->scenePos());
	}		
	else if ( selectedAction == loadExpressionAction )
	{
		QString name = "data/pathways/TGF";
		if( name.size() > 0 )
		{
			ItemBase* nitem;
			nitem = NULL;	
			bool opened;
			opened = getOpenManager()->openExpressionByPath(nitem, 300, contextMenuEvent->scenePos().x(), contextMenuEvent->scenePos().y(), name);
			if(opened)
			{
				expressionBubble* ExpressionBubble = dynamic_cast<expressionBubble*>(nitem);
				//EBubble->m_bindPathwayID = -1;	
				QString tname = name + "0expression.txt";  			
				pwdParser->readexpression(tname.toUtf8().data(), ExpressionBubble->quantityName, ExpressionBubble->geneInfo, ExpressionBubble->quantity, ExpressionBubble->minmax);
				/*ExpressionBubble->searchExpressedProtein();
				if(item!=NULL)
				   clearExpressed(item);				
				ExpressionBubble->clearExpressed();	*/			
				ExpressionBubble->setExpressedGenePos(1);

				if(item==NULL)
				{
					QPointF pos = this->_manager->getPosition(ExpressionBubble,ExpressionBubble->sceneBoundingRect().x(), ExpressionBubble->sceneBoundingRect().y(), ExpressionBubble->sceneBoundingRect().width(), ExpressionBubble->sceneBoundingRect().height());
					float x= contextMenuEvent->scenePos().x(), y=contextMenuEvent->scenePos().y();
					if(pos.x()!=-1 && pos.y()!=-1)
					{
					   x = pos.x();
					   y = pos.y();	
					}
					ExpressionBubble->setTargetPos(QPointF(x + ExpressionBubble->sceneBoundingRect().width()/2, y + ExpressionBubble->sceneBoundingRect().height()/2)); 
				}
				else 
				{
					QPointF pos = getGroupedBubblePosition(ExpressionBubble,item, ExpressionBubble->sceneBoundingRect().x(), ExpressionBubble->sceneBoundingRect().y(), ExpressionBubble->sceneBoundingRect().width(), ExpressionBubble->sceneBoundingRect().height());				
					addToGroup( ExpressionBubble, item, false);
				}				
			}					
		}		
    }
	else if ( selectedAction == G2Pathway )
	{	
		QString name = "data/pathways/MitoticG2-G2";
		openPathBubblebyMenu(name, item, contextMenuEvent->scenePos());
    }	
	else if ( selectedAction == BMPPathway )
	{	
		QString name = "data/pathways/BMP";
		openPathBubblebyMenu(name, item, contextMenuEvent->scenePos());
    }
	else if ( selectedAction == TGFPathway )
	{	
		QString name = "data/pathways/TGF";
		openPathBubblebyMenu(name, item, contextMenuEvent->scenePos());
    }
	else if(selectedAction == searchSettingsAction)
	{
		openWebNoteBubblebyMenu(SEARCHSETTINGS, NULL, contextMenuEvent->scenePos());
	}	
	if(item!=NULL)
	{	   
		
		if(pBubble!=NULL) //(search + edit + part of tool))
		{
		    
			if (selectedAction == shareAction)
			{
				if(pBubble->searchType=='P')
				{
					pBubble->menuSelection_1(pos_menu, scenePos_menu, 0); //p					
				}
				else if(pBubble->searchType=='M')
					pBubble->menuSelection_1(pos_menu, scenePos_menu, 1); //C
				else if(pBubble->searchType=='W')
					pBubble->menuSelection_1(pos_menu, scenePos_menu, 2); //PA
			}
			else if (selectedAction == diffAction)
			{
				if(pBubble->searchType=='P')
					pBubble->menuSelection_1(pos_menu, scenePos_menu, 10); //P
				else if(pBubble->searchType=='M')
					pBubble->menuSelection_1(pos_menu, scenePos_menu, 11); //C
				else if(pBubble->searchType=='W')
					pBubble->menuSelection_1(pos_menu, scenePos_menu, 12); //PA
			}
			else if (selectedAction == linkAction)
			{ 
				if(pBubble->searchType=='P')
					pBubble->menuSelection_1(pos_menu, scenePos_menu, 20); //P
				else if(pBubble->searchType=='M')
					pBubble->menuSelection_1(pos_menu, scenePos_menu, 21); //C
				else if(pBubble->searchType=='W')
					pBubble->menuSelection_1(pos_menu, scenePos_menu, 22); //PA
			}
			else if (selectedAction == expressionExpressedAction )
			{
				pBubble->menuSelection(pos_menu, scenePos_menu, 1);
				expressedGeneNames = pBubble->getExpressedGeneNames();
				getGroup(pBubble)->expressedSets = pBubble->assignExpressedSets();
				vector<PathBubble1 *> plist = extractPathBubblesinGroup(pBubble);
				for(int i=0; i<plist.size(); i++) 	
				{
					PathBubble1* pbubble=plist[i];	
					int Did=pbubble->dataID, pid=pbubble->m_pathwayID;
				    if(!pbubble->expressedProtein.empty() || !pbubble->expressedComplex.empty())
					    pbubble->getGraphToBePaint();				
				}				
			}
			else if (selectedAction == searchSettingsAction)
			{
				ItemBase *newItem = openWebNoteBubblebyMenu(SEARCHSETTINGS, item, contextMenuEvent->scenePos());
				WebBlock *settingItem = dynamic_cast<WebBlock *>(newItem);
				settingItem->_settings->setInvisible();
			}
		}
	}
	
	
	return;
	
		

			//searchMenu->addMenu(shareMenu); 
			/*sharePAction = proteinMenu->addAction("Shared");
			shareCAction = compartmentMenu->addAction("Shared");	      
			sharePAAction = pathwaySearchMenu->addAction("Shared");	 
			
			//searchMenu->addMenu(diffMenu);  	
			diffPAction = proteinMenu->addAction("Difference");
			diffCAction = compartmentMenu->addAction("Difference");	      
			diffPAAction = pathwaySearchMenu->addAction("Difference");	 
		 
			
			//searchMenu->addMenu(linkMenu);   	
			linkPAction = proteinMenu->addAction("Linked");
			linkCAction = compartmentMenu->addAction("Linked");	      
			linkPAAction = pathwaySearchMenu->addAction("Linked");	 
			*/
        if(editMenuflag)	
	    {
			editMenu = menu.addMenu("Edit");
		    editMenu->setMinimumWidth(120);
		}
		    if(colorMenuflag)
				colorMenu = editMenu->addMenu("Regulation \n by Color");
			//toolMenu->addMenu(colorMenu);  
			//text2[1].push_back("Up"); text2[1].push_back("Even"); text2[1].push_back("Down");				
			if(colorUpActionflag)
				colorUpAction = colorMenu->addAction("Up");   
			if(colorEvenActionflag)
				colorEvenAction = colorMenu->addAction("Even");   
			if(colorDownAction)
				colorDownAction = colorMenu->addAction("Down");   

			//queryMenu = menu.addMenu("Query");	 	
			if(toolUndoAction)
				toolUndoAction = editMenu->addAction("Undo");
			if(toolRedoActionflag)
				toolRedoAction = editMenu->addAction("Redo");	      
			//toolClearAction = toolMenu->addAction("Clear");	 	
			//graphMenu = menu.addMenu("Graph");
			//menu.addMenu(graphMenu);   
			//text2[2].push_back("Reset"); text2[2].push_back("Undo"); text2[2].push_back("Redo"); text2[2].push_back("Reload"); 
	
			if(graphResertActionflag)
				graphResertAction = editMenu->addAction("Reset"); 
			/*graphUndoAction = editMenu->addAction("Graph Undo"); 
			graphRedoAction = editMenu->addAction("Graph Redo"); */
			//graphReloadAction;// = graphMenu->addAction("Reload"); 	
	     
	                                             
	
	     if(toolMenuflag)
			 toolMenu = menu.addMenu("Tools");
		 if(sketchActionflag)
			 sketchAction = toolMenu->addAction("Pen Selection"); 
		 if(queryActionflag)
			 queryAction = toolMenu->addAction("E-gift query"); 
		
		 if(deleteMenuflag)
			 deleteMenu = toolMenu->addMenu("Delete");
			 if(delActionflag)
				 delAction = deleteMenu->addAction("bubble");	
			 if(delGroupActionflag)
				 delGroupAction = deleteMenu->addAction("group");				 

		 if(sceneMenuflag)
			 sceneMenu = toolMenu->addMenu("Display");
				if(removeActionflag)
					removeAction = sceneMenu->addAction("Clear");	
				if(saveSceneflag)
					saveScene = sceneMenu->addAction("Save");
				if(recoverSceneflag)
					recoverScene = sceneMenu->addAction("Load");

		//viewMenu = toolMenu->addMenu("View");
	    if(searchPathwayActionflag)
			searchPathwayAction = toolMenu->addAction("Quick Find");
		if(settingsActionflag)
			settingsAction = toolMenu->addAction("View"); //settings
		//settingsAction->deleteLater();
	
		if(toplevel && settingsActionflag)
		   graphReloadAction = editMenu->addAction("Graph Reload"); 	

		

	selectedAction = menu.exec( contextMenuEvent->screenPos() );
	
	//public //load 
	
	if ( selectedAction == removeAction )
	{
		QMessageBox msgBox;
		msgBox.setWindowTitle("Delete");
		msgBox.setText("Are you sure you want to remove all the bubbles in the scene?");
		msgBox.setStandardButtons(QMessageBox::Yes);
		//msgBox.setDefaultButton(QMessageBox::No);
		msgBox.addButton (QMessageBox::No);
		if(msgBox.exec() == QMessageBox::Yes)
		{  
			logOperation( "Remove all the bubbles in the scene\n" );
			removeAllItems();
		}
	}
	else if ( selectedAction == saveScene )
	{
		/*QString fileName = QFileDialog::getSaveFileName();
		this->recordSceneAsXmlFile( fileName );*/
	}
	else if ( selectedAction == recoverScene )
	{
		/*QString fileName = QFileDialog::getOpenFileName();
		this->recoverSceneFromXmlFile( fileName );*/
	}
	else if ( selectedAction == settingsAction )
	{
		openWebNoteBubblebyMenu(SETTINGS, item, contextMenuEvent->scenePos());				
	}	    		
	

	if(item!=NULL)
	{	   
		
		if(pBubble!=NULL) //(search + edit + part of tool))
		{
		    
			if (selectedAction == shareAction)
			{
				if(pBubble->searchType=='P')
				{
					pBubble->menuSelection_1(pos_menu, scenePos_menu, 0); //p					
				}
				else if(pBubble->searchType=='M')
					pBubble->menuSelection_1(pos_menu, scenePos_menu, 1); //C
				else if(pBubble->searchType=='W')
					pBubble->menuSelection_1(pos_menu, scenePos_menu, 2); //PA
			}
			else if (selectedAction == diffAction)
			{
				if(pBubble->searchType=='P')
					pBubble->menuSelection_1(pos_menu, scenePos_menu, 10); //P
				else if(pBubble->searchType=='M')
					pBubble->menuSelection_1(pos_menu, scenePos_menu, 11); //C
				else if(pBubble->searchType=='W')
					pBubble->menuSelection_1(pos_menu, scenePos_menu, 12); //PA
			}
			else if (selectedAction == linkAction)
			{ 
				if(pBubble->searchType=='P')
					pBubble->menuSelection_1(pos_menu, scenePos_menu, 20); //P
				else if(pBubble->searchType=='M')
					pBubble->menuSelection_1(pos_menu, scenePos_menu, 21); //C
				else if(pBubble->searchType=='W')
					pBubble->menuSelection_1(pos_menu, scenePos_menu, 22); //PA
			}
			else if (selectedAction == expressionExpressedAction )
			{
				pBubble->menuSelection(pos_menu, scenePos_menu, 1);
				expressedGeneNames = pBubble->getExpressedGeneNames();
				getGroup(pBubble)->expressedSets = pBubble->assignExpressedSets();
				vector<PathBubble1 *> plist = extractPathBubblesinGroup(pBubble);
				for(int i=0; i<plist.size(); i++) 	
				{
					PathBubble1* pbubble=plist[i];	
					int Did=pbubble->dataID, pid=pbubble->m_pathwayID;
				    if(!pbubble->expressedProtein.empty() || !pbubble->expressedComplex.empty())
					    pbubble->getGraphToBePaint();				
				}				
			}
			else if (selectedAction == searchSettingsAction)
			{
				ItemBase *newItem = openWebNoteBubblebyMenu(SEARCHSETTINGS, item, contextMenuEvent->scenePos());
				WebBlock *settingItem = dynamic_cast<WebBlock *>(newItem);
				settingItem->_settings->setInvisible();
			}
			else if (selectedAction == colorUpAction)
			{
				pBubble->menuSelection(pos_menu, scenePos_menu, 10);
			}
			else if (selectedAction == colorEvenAction)
			{
				pBubble->menuSelection(pos_menu, scenePos_menu, 11);
			}
			else if (selectedAction == colorDownAction)
			{
				pBubble->menuSelection(pos_menu, scenePos_menu, 12);
			}
			else if (selectedAction == graphResertAction)
			{
				pBubble->menuSelection(pos_menu, scenePos_menu, 20);
			}			
			else if (toplevel && selectedAction == graphReloadAction)
			{
				pBubble->menuSelection(pos_menu, scenePos_menu, 25);
			}
			else if (selectedAction == toolUndoAction)
			{
				if(*pBubble->currentEventit == SEARCH)
				{
					pBubble->menuSelection_1(pos_menu, scenePos_menu, 30);
				}
				else
				{
					pBubble->menuSelection(pos_menu, scenePos_menu, 21);
				}
			}
			else if (selectedAction == toolRedoAction)
			{
				if(*pBubble->currentEventit == SEARCH)
				{
				    pBubble->menuSelection_1(pos_menu, scenePos_menu, 31);
				}
				else 
				{
					pBubble->menuSelection(pos_menu, scenePos_menu, 22);
				}
			}			
			else if ( selectedAction == settingsAction )
			{
				ItemBase *newItem = openWebNoteBubblebyMenu(SETTINGS, item, contextMenuEvent->scenePos());
				WebBlock *settingItem = dynamic_cast<WebBlock *>(newItem);
				settingItem->_settings->setInvisible(); 
			}			
         }
		 if (selectedAction == delAction)
		 {
			   pBubble->menuSelection(pos_menu, scenePos_menu, 50);

		 }
		 else if (selectedAction == delGroupAction)
		 {			
			   pBubble->menuSelection(pos_menu, scenePos_menu, 51);
		 }
		 if (selectedAction == sketchAction)
		{
			pBubble->menuSelection(pos_menu, scenePos_menu, 39);
		}
		else if (selectedAction == queryAction)
		{
			pBubble->menuSelection(pos_menu, scenePos_menu, 49);
		}
		else if ( selectedAction == searchPathwayAction )
		{
			openWebNoteBubblebyMenu(SEARCHFIND, item, contextMenuEvent->scenePos());					
		}
	     
	}
	else //at open space (search + edit + part of tool)
	{
		QList<Group*> groupList = this->_manager->groupList();
		for ( int i = 0; i < groupList.size(); i ++)
		{	
				PathBubble1 *pBubble = findAPathBubbleinGroup(groupList[i]);
				if(pBubble==NULL)
					continue;
				
				if (selectedAction == graphResertAction)
				{
					pBubble->menuSelection(pos_menu, scenePos_menu, 20);
				}			
				else if (toplevel && selectedAction == graphReloadAction)
				{
					pBubble->menuSelection(pos_menu, scenePos_menu, 25);
				}
				else if (selectedAction == shareAction)
				{
					if(pBubble->searchType=='P')
					{
						pBubble->menuSelection_1(pos_menu, scenePos_menu, 0); //p					
					}
					else if(pBubble->searchType=='M')
						pBubble->menuSelection_1(pos_menu, scenePos_menu, 1); //C
					else if(pBubble->searchType=='W')
						pBubble->menuSelection_1(pos_menu, scenePos_menu, 2); //PA
				}
				else if (selectedAction == diffAction)
				{
					if(pBubble->searchType=='P')
						pBubble->menuSelection_1(pos_menu, scenePos_menu, 10); //P
					else if(pBubble->searchType=='M')
						pBubble->menuSelection_1(pos_menu, scenePos_menu, 11); //C
					else if(pBubble->searchType=='W')
						pBubble->menuSelection_1(pos_menu, scenePos_menu, 12); //PA
				}
				else if (selectedAction == linkAction)
				{ 
					if(pBubble->searchType=='P')
						pBubble->menuSelection_1(pos_menu, scenePos_menu, 20); //P
					else if(pBubble->searchType=='M')
						pBubble->menuSelection_1(pos_menu, scenePos_menu, 21); //C
					else if(pBubble->searchType=='W')
						pBubble->menuSelection_1(pos_menu, scenePos_menu, 22); //PA
				}
				else if (selectedAction == expressionExpressedAction  )
				{
					pBubble->menuSelection(pos_menu, scenePos_menu, 1);
					expressedGeneNames = pBubble->getExpressedGeneNames();
					getGroup(pBubble)->expressedSets = pBubble->assignExpressedSets();
					vector<PathBubble1 *> plist = extractPathBubblesinGroup(pBubble);
					for(int i=0; i<plist.size(); i++) 	
					{
						PathBubble1* pbubble=plist[i];	
						int Did=pbubble->dataID, pid=pbubble->m_pathwayID;
						if(!pbubble->expressedProtein.empty() || !pbubble->expressedComplex.empty())
							pbubble->getGraphToBePaint();				
					}				
				}			
				else if (selectedAction == toolUndoAction)
				{
					if(*pBubble->currentEventit == SEARCH)
					{
						pBubble->menuSelection_1(pos_menu, scenePos_menu, 30);
					}
					else
					{
						pBubble->menuSelection(pos_menu, scenePos_menu, 21);
					}
				}
				else if (selectedAction == toolRedoAction)
				{
					if(*pBubble->currentEventit == SEARCH)
					{
					   pBubble->menuSelection_1(pos_menu, scenePos_menu, 31);
					}
					else 
					{
						pBubble->menuSelection(pos_menu, scenePos_menu, 22);
					}
				}				
		}		
    }
	

}

void OpenGLScene::contextMenuEvent( QGraphicsSceneContextMenuEvent * contextMenuEvent )
{
	QGraphicsScene::contextMenuEvent( contextMenuEvent );	
	if( contextMenuEvent->isAccepted() )
	{
		return;
	}
	dropDownMenu0(contextMenuEvent, NULL);
	
	return;
}

void OpenGLScene::removeAllItems()
{
	QList<ItemBase*> list; 
	QList<QGraphicsItem*> allItem = this->items();
	for( int i = 0; i < allItem.size(); i++ )	
	{
		QGraphicsItem *item = allItem[i];
		ItemBase *itemBase = dynamic_cast<ItemBase*>(item);
		if( itemBase )
		{
			list.append( itemBase );
		}
	}
	//_manager->_list.removeAll( removeList[i] );
	//foreach ( ItemBase *itemBase, list )
	for( int i = 0; i < list.size(); i++ )
	{
		ItemBase *itemBase = list[i];
		if( itemBase && !itemBase->isDelegated() )
		{
			int TYPE = itemBase->getType();
			if( TYPE == PATHBUBBLE1 || TYPE == SUBPATHBUBBLE1 || TYPE == PATHBUBBLE2  || TYPE == TEXTBUBBLE2 || TYPE == REACTIONBUBBLE3 || TYPE == GROUPBUBBLE4 || TYPE == EXPRESSIONBUBBLE || TYPE == SEARCHFIND || TYPE == SETTINGS || TYPE == WEB || TYPE == NOTE )
			{
				if(TYPE == TEXTBUBBLE2)
				{
					TextBubble2 *tb = dynamic_cast<TextBubble2*>(itemBase);
					if(itemBase->isVisible())
					{
						delete [] tb->_text;						
					}					
				}
				if(TYPE == WEB)
				{
					WebBlock *tb = dynamic_cast<WebBlock*>(itemBase);
					if(itemBase->isVisible())
					{
						//delete [] tb->_search;						
						//delete [] tb->_web;
						//tb->deleteItems();
						itemBase->unGroup();
						tb->_web->hide();
					}
				}
				if(TYPE == SEARCHFIND)
				{
					WebBlock *tb = dynamic_cast<WebBlock*>(itemBase);
					if(itemBase->isVisible())
					{
						itemBase->unGroup();
						tb->_search->hide();
					}
				}
				if(TYPE == SETTINGS)
				{
					WebBlock *tb = dynamic_cast<WebBlock*>(itemBase);
					if(itemBase->isVisible())
					{
						itemBase->unGroup();
						tb->_settings->hide();
					}
				}
				if(TYPE == NOTE)
				{
					NoteBlock *nb = dynamic_cast<NoteBlock*>(itemBase);
					if(itemBase->isVisible())
					{
						itemBase->unGroup();
						nb->m_note->hide();
					}
				}
				if(TYPE == PATHBUBBLE1 || TYPE == SUBPATHBUBBLE1)
				{
					PathBubble1 *tb = dynamic_cast<PathBubble1*>(itemBase);
					if(itemBase->isVisible())
					{
						tb->deleteItems();
					}
					//tb->m_pathwayID=-1;
				}									
				itemBase->hide();
				//itemBase->properRemove();
				//itemBase->deleteSelectedItems(this);
			}
		}
	}
	
	this->_manager->clearAll(); 
	_pathBubbles.clear();	
	_complexContain.clear();
	
	/*_complexNameD.clear();
	_physicalEntityNameD.clear();
	_proteinNameD.clear();
	_reactionNameD.clear();
	_degradationNameD.clear();	//start from 1
	_smallMoleculeNameD.clear();
	_EmptyNameD.clear(); 
	_DnaNameD.clear();  //start from 1
	*/
	_biochemicalReaction.clear(); //start from 1
	_degradation.clear();
	CompartmentContain.clear(); //start from 0
	_compartmentName.clear(); //start from 0
	
	//for expression
	_quantityName.clear();
	_quantity.clear();
	_minmax.clear();
	_geneInfo.clear(); //gene_id	symbol

	_complexPos.clear(); _proteinPos.clear(); _smallMoleculePos.clear(); _DnaPos.clear(); _degenerationPos.clear(); _reactionPos.clear(); _physicalEntityPos.clear(); _compartmentPos.clear(); _EmptyPos.clear();	
	_edge.clear();
	edgeconnections.clear();
	_Scale.clear(); 
	_dCenter.clear();
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
	/*QString lastPos;
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
	return;*/
}

/*
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
*/

void OpenGLScene::updateCoverage( float rate )
{
	foreach (QGraphicsItem *item, this->items() )
	{
		ItemBase *itembase = dynamic_cast< ItemBase* >(item);
		if( itembase )
		{
			/*QPointF curPos = itembase->getInitPos() + this->_TopLeft;
			QRectF rect = itembase->sceneBoundingRect();
			float x0,y0,x1,y1,cx,cy;
			x0=rect.x(); y0=rect.y(); x1=x0+rect.width(); y1=y0+rect.height();
			cx=(x0+x1)/rate/2, cy=(y0+y1)/rate/2;
			itembase->mySetPos( QPointF(cx,cy));
			//int newSize_w = int( float(  itembase->boundingRect().width() ) / rate + 0.5 ), newSize_h = int( float(  itembase->boundingRect().height() ) / rate + 0.5 );
			int newSize_w = int( (x1-x0)/rate  + 0.5 ), newSize_h = int( (y1-y0)/rate  + 0.5 );*/
			if(rate>0)
			   itembase->scaleUpBasedonOriRect();
			else 
				itembase->scaleDownBasedonOriRect();
			
			//itembase->resizeItem( newSize_w, newSize_h );
		}
	}
	//this->update( QRectF( 0, 0, this->width(), this->height() ) );
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
					//QtLCom* com = dynamic_cast<QtLCom*>(base);										
					//com->acceptStatisticData( manager, index );
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

/*void OpenGLScene::setCurrentBatAttribute( QtBat *object )
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
}*/

/*
void OpenGLScene::setMatFileBubble( MatBlock* currentMat )
{
	hasMatBubble = true;
	matCurrent = currentMat; 
}*/
QList<ItemBase*> OpenGLScene::allMembers()
{
	return _manager->allMembers();
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
			//str.setNum( j+1 );
			str.setNum( gList[i]->getMemberAt(j)->_parentPathBubbleID );
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
	this->removeAllItems();
	QDomDocument doc;
	if ( !doc.setContent(&file) )
	{
		file.close();
		return;
	}
	file.close();

	QDomElement rootNode = doc.documentElement();
	QDomNode docNode = rootNode.firstChild();
	QString tagname,bubbleID;

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

	//rebuild parent and son bubble for Bubble
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
				QDomElement eachBubble = childElement.toElement();
				bubbleID = eachBubble.tagName();
				bubbleID = eachBubble.attribute("bubbleID");
				while ( !childElement.isNull() )
				{
					tagname = childElement.elementsByTagName("Type").at(0).toElement().text();					
					if (tagname == "PathBubble1"||tagname == "subPathBubble1"||tagname == "TextBubble2"||tagname == "ReactionBubble3"||tagname == "GroupBubble4")
					{
						buildMatBubbleConnect( childElement );
						buildSubGraphBubbleConnect(childElement);
					}					
					childElement = childElement.nextSibling().toElement();
				}
			}
		}
		docNode = docNode.nextSibling();
	}	
}

void OpenGLScene::searchInPathBubble(QString str, set<int> searchList, QString name, ItemBase *item)
{	
	for (int i = 0; i < _pathBubbles.size(); i ++)		
	{
		if(_pathBubbles[i]==NULL || !_pathBubbles[i]->isVisible())
			continue;
		ItemBase* member=NULL;
		if(item!=NULL)
			member = dynamic_cast<ItemBase*>(_pathBubbles[i]);
		if(_pathBubbles[i]->_name.compare(name)==0 || name.compare("all")==0 || ( name.compare("")==0 && member==item))
		{
		    _pathBubbles[i]->searched.clear();
		    _pathBubbles[i]->_linkage.clear();
		}		
	}
		
    if(searchList.find('M')!=searchList.end())
	for (int i = 0; i < _compartmentName.size(); i ++)
	{		
		ItemBase* member=NULL;
		if(item!=NULL)
			member = dynamic_cast<ItemBase*>(_pathBubbles[i]);
		if(_pathBubbles[i]==NULL  || !_pathBubbles[i]->isVisible())
			continue;
		if(_pathBubbles[i]->_name.compare(name)==0 || name.compare("all")==0 || ( name.compare("")==0 && member==item))
		for (int j = 0; j < _compartmentName[i].size(); j ++)
		{
			if(_compartmentName[i][j][0].contains(str,Qt::CaseInsensitive))
		    {
				vector<int> item;
		        item.push_back('M'); item.push_back(j);
                _pathBubbles[i]->searched.insert(item);
		    }
		}
	}
	if(searchList.find('C')!=searchList.end())
	for (int i = 0; i < _compartmentName.size(); i ++) //_complexName
	{
		ItemBase* member=NULL;
		if(item!=NULL)
			member = dynamic_cast<ItemBase*>(_pathBubbles[i]);
		if(_pathBubbles[i]==NULL  || !_pathBubbles[i]->isVisible())
			continue;
		if(_pathBubbles[i]->_name.compare(name)==0 || name.compare("all")==0|| ( name.compare("")==0 && member==item))
		{
			int Did=_pathBubbles[i]->dataID;
			for (int j = 0; j < _complexNameD[Did].size(); j ++)
			{
				for (int k = 0; k < _complexNameD[Did][j].size(); k ++)
				{
				   if(_complexNameD[Did][j][k].contains(str,Qt::CaseInsensitive))
				   {
					 vector<int> item;
					item.push_back('C'); item.push_back(j);
					_pathBubbles[i]->searched.insert(item);
				  }
				}
			}
		}
	}
	if(searchList.find('E')!=searchList.end())
	//for (int i = 0; i < _physicalEntityName.size(); i ++)
	for (int i = 0; i < _compartmentName.size(); i ++) 
	{
		ItemBase* member=NULL;
		if(item!=NULL)
			member = dynamic_cast<ItemBase*>(_pathBubbles[i]);
		if(_pathBubbles[i]==NULL  || !_pathBubbles[i]->isVisible())
			continue;
		if(_pathBubbles[i]->_name.compare(name)==0 || name.compare("all")==0)
		{
			int Did=_pathBubbles[i]->dataID;
			for (int j = 0; j < _physicalEntityNameD[Did].size(); j ++)
			{
				for (int k = 0; k < _physicalEntityNameD[Did][j].size(); k ++)
				{
				   if(_physicalEntityNameD[Did][j][k].contains(str,Qt::CaseInsensitive))
				   {
					  vector<int> item;
					item.push_back('E'); item.push_back(j);
					_pathBubbles[i]->searched.insert(item);
				  }
				}
			}
		}
	}
	if(searchList.find('P')!=searchList.end())
	for (int i = 0; i < _compartmentName.size(); i ++) 
	{
		ItemBase* member=NULL;
		if(item!=NULL)
			member = dynamic_cast<ItemBase*>(_pathBubbles[i]);
		if(_pathBubbles[i]==NULL  || !_pathBubbles[i]->isVisible())
			continue;
		if(_pathBubbles[i]->_name.compare(name)==0 || name.compare("all")==0|| ( name.compare("")==0 && member==item))
		{
			int Did=_pathBubbles[i]->dataID;
			for (int j = 0; j < _proteinNameD[Did].size(); j ++)
			{
				for (int k = 0; k < _proteinNameD[Did][j].size(); k ++)
				{
				   if(_proteinNameD[Did][j][k].contains(str,Qt::CaseInsensitive))
				   {
					 vector<int> item;
					item.push_back('P'); item.push_back(j);
					_pathBubbles[i]->searched.insert(item);
				  }
				}
			}	
		}
	}
	if(searchList.find('R')!=searchList.end())
	for (int i = 0; i < _compartmentName.size(); i ++) 
	{
		ItemBase* member=NULL;
		if(item!=NULL)
			member = dynamic_cast<ItemBase*>(_pathBubbles[i]);
		if(_pathBubbles[i]==NULL  || !_pathBubbles[i]->isVisible())
			continue;
		if(_pathBubbles[i]->_name.compare(name)==0 || name.compare("all")==0|| ( name.compare("")==0 && member==item))
		{
			int Did=_pathBubbles[i]->dataID;
			for (int j = 0; j < _reactionNameD[Did].size(); j ++)
			{
				for (int k = 0; k < _reactionNameD[Did][j].size(); k ++)
				{
				   if(_reactionNameD[Did][j][k].contains(str,Qt::CaseInsensitive))
				   {
					  vector<int> item;
					  item.push_back('R'); item.push_back(j);
					  _pathBubbles[i]->searched.insert(item);
				  }
				}
			}
		}
	}
	if(searchList.find('S')!=searchList.end())
	for (int i = 0; i < _compartmentName.size(); i ++) 
	{
		ItemBase* member=NULL;
		if(item!=NULL)
			member = dynamic_cast<ItemBase*>(_pathBubbles[i]);
		if(_pathBubbles[i]==NULL  || !_pathBubbles[i]->isVisible())
			continue;
		if(_pathBubbles[i]->_name.compare(name)==0 || name.compare("all")==0|| ( name.compare("")==0 && member==item))
		{
			int Did=_pathBubbles[i]->dataID;
			for (int j = 0; j < _smallMoleculeNameD[Did].size(); j ++)
			{
				for (int k = 0; k < _smallMoleculeNameD[Did][j].size(); k ++)
				{
				   if(_smallMoleculeNameD[Did][j][k].contains(str,Qt::CaseInsensitive))
				   {
					  vector<int> item;
					  item.push_back('S'); item.push_back(j);
					  _pathBubbles[i]->searched.insert(item);
				  }
				}
			}	
		}
	}
	if(searchList.find('D')!=searchList.end())
	for (int i = 0; i < _compartmentName.size(); i ++) 
	{
		ItemBase* member=NULL;
		if(item!=NULL)
			member = dynamic_cast<ItemBase*>(_pathBubbles[i]);
		if(_pathBubbles[i]==NULL  || !_pathBubbles[i]->isVisible())
			continue;
		if(_pathBubbles[i]->_name.compare(name)==0 || name.compare("all")==0|| ( name.compare("")==0 && member==item))
		{
			int Did=_pathBubbles[i]->dataID;
			for (int j = 0; j < _DnaNameD[Did].size(); j ++)
			{
				for (int k = 0; k < _DnaNameD[Did][j].size(); k ++)
				{
				   if(_DnaNameD[Did][j][k].contains(str,Qt::CaseInsensitive))
				   {
					   vector<int> item;
					   item.push_back('D'); item.push_back(j);
					   _pathBubbles[i]->searched.insert(item);
				   }
				}
			}	
		}
	}
	
	for (int i = 0; i < _pathBubbles.size(); i ++)		
	{
		if(_pathBubbles[i]!=NULL  && _pathBubbles[i]->isVisible())
		if(!_pathBubbles[i]->searched.empty())
		{
			_pathBubbles[i]->getGraphToBePaint();				
		}	
	}
		
}


void OpenGLScene::searchInExpressionBubble(QString str, set<int> searchList, QString name, ItemBase *item)
{	
	if(name.size()==0)
		return;
	for (int i = 0; i < _expressionBubbles.size(); i ++)		
	{
		if(!_expressionBubbles[i]->isVisible())
			continue;
		ItemBase* member=NULL;
		if(item!=NULL)
			member = dynamic_cast<ItemBase*>(_expressionBubbles[i]);
		if(_expressionBubbles[i]->_name.compare(name)==0 || name.compare("all")==0 || ( name.compare("")==0 && member==item))
		{
		    _expressionBubbles[i]->searched.clear();
		    _expressionBubbles[i]->_linkage.clear();
		}		
	}

	
    /*if(searchList.find('M')!=searchList.end())
	for (int i = 0; i < _compartmentName.size(); i ++)
	{		
		ItemBase* member=NULL;
		if(item!=NULL)
			member = dynamic_cast<ItemBase*>(_expressionBubbles[i]);
		if(!_expressionBubbles[i]->isVisible())
			continue;
		if(_expressionBubbles[i]->_name.compare(name)==0 || name.compare("all")==0 || ( name.compare("")==0 && member==item))
		for (int j = 0; j < _compartmentName[i].size(); j ++)
		{
			if(_compartmentName[i][j][0].contains(str,Qt::CaseInsensitive))
		    {
				vector<int> item;
		        item.push_back('M'); item.push_back(j);
                _expressionBubbles[i]->searched.insert(item);
		    }
		}
	}
	if(searchList.find('C')!=searchList.end())
	for (int i = 0; i < _complexName.size(); i ++)
	{
		ItemBase* member=NULL;
		if(item!=NULL)
			member = dynamic_cast<ItemBase*>(_expressionBubbles[i]);
		if(!_expressionBubbles[i]->isVisible())
			continue;
		if(_expressionBubbles[i]->_name.compare(name)==0 || name.compare("all")==0|| ( name.compare("")==0 && member==item))
		for (int j = 0; j < _complexName[i].size(); j ++)
		{
			if(_complexName[i][j][0].contains(str,Qt::CaseInsensitive))
		    {
				vector<int> item;
		        item.push_back('C'); item.push_back(j);
                _expressionBubbles[i]->searched.insert(item);
		    }
		}
	}
	if(searchList.find('E')!=searchList.end())
	for (int i = 0; i < _physicalEntityName.size(); i ++)
	{
		ItemBase* member=NULL;
		if(item!=NULL)
			member = dynamic_cast<ItemBase*>(_expressionBubbles[i]);
		if(!_expressionBubbles[i]->isVisible())
			continue;
		if(_expressionBubbles[i]->_name.compare(name)==0 || name.compare("all")==0)
		for (int j = 0; j < _physicalEntityName[i].size(); j ++)
		{
			for (int k = 0; k < _physicalEntityName[i][j].size(); k ++)
			{
			   if(_physicalEntityName[i][j][k].contains(str,Qt::CaseInsensitive))
		       {
		          vector<int> item;
		        item.push_back('E'); item.push_back(j);
                _expressionBubbles[i]->searched.insert(item);
		      }
			}
		}
	}*/
	if(searchList.find('P')!=searchList.end())
	for (int i = 0; i < _expressionBubbles.size(); i ++)		
	{		
		ItemBase* member=NULL;
		if(item!=NULL)
			member = dynamic_cast<ItemBase*>(_expressionBubbles[i]);
	    if(!_expressionBubbles[i]->isVisible())
			continue;
		if(_expressionBubbles[i]->_name.compare(name)==0 || name.compare("all")==0|| ( name.compare("")==0 && member==item))
		for (int j = 0; j <_expressionBubbles[i]->geneInfo.size(); j ++)
		{
			for (int k = 0; k < _expressionBubbles[i]->geneInfo[j].size(); k ++)
			{
			   if(_expressionBubbles[i]->geneInfo[j][k].contains(str,Qt::CaseInsensitive))
		       {
		         vector<int> item;
		         item.push_back('P'); item.push_back(j);
                 _expressionBubbles[i]->searched.insert(item);
		      }
			}
		}	
	}
	/*if(searchList.find('R')!=searchList.end())
	for (int i = 0; i < _reactionName.size(); i ++)
	{
		ItemBase* member=NULL;
		if(item!=NULL)
			member = dynamic_cast<ItemBase*>(_expressionBubbles[i]);
		if(!_expressionBubbles[i]->isVisible())
			continue;
		if(_expressionBubbles[i]->_name.compare(name)==0 || name.compare("all")==0|| ( name.compare("")==0 && member==item))
		for (int j = 0; j < _reactionName[i].size(); j ++)
		{
			for (int k = 0; k < _reactionName[i][j].size(); k ++)
			{
			   if(_reactionName[i][j][k].contains(str,Qt::CaseInsensitive))
		       {
		          vector<int> item;
		          item.push_back('R'); item.push_back(j);
                  _expressionBubbles[i]->searched.insert(item);
		      }
			}
		}	
	}
	if(searchList.find('S')!=searchList.end())
	for (int i = 0; i < _smallMoleculeName.size(); i ++)
	{
		ItemBase* member=NULL;
		if(item!=NULL)
			member = dynamic_cast<ItemBase*>(_expressionBubbles[i]);
		if(!_expressionBubbles[i]->isVisible())
			continue;
		if(_expressionBubbles[i]->_name.compare(name)==0 || name.compare("all")==0|| ( name.compare("")==0 && member==item))
		for (int j = 0; j < _smallMoleculeName[i].size(); j ++)
		{
			for (int k = 0; k < _smallMoleculeName[i][j].size(); k ++)
			{
			   if(_smallMoleculeName[i][j][k].contains(str,Qt::CaseInsensitive))
		       {
		          vector<int> item;
		          item.push_back('S'); item.push_back(j);
                  _expressionBubbles[i]->searched.insert(item);
		      }
			}
		}	
	}
	if(searchList.find('D')!=searchList.end())
	for (int i = 0; i < _DnaName.size(); i ++)
	{
		ItemBase* member=NULL;
		if(item!=NULL)
			member = dynamic_cast<ItemBase*>(_expressionBubbles[i]);
		if(!_expressionBubbles[i]->isVisible())
			continue;
		if(_expressionBubbles[i]->_name.compare(name)==0 || name.compare("all")==0|| ( name.compare("")==0 && member==item))
		for (int j = 0; j < _DnaName[i].size(); j ++)
		{
			for (int k = 0; k < _DnaName[i][j].size(); k ++)
			{
			   if(_DnaName[i][j][k].contains(str,Qt::CaseInsensitive))
		       {
		           vector<int> item;
		           item.push_back('D'); item.push_back(j);
                   _expressionBubbles[i]->searched.insert(item);
		       }
			}
		}		
	}*/

}

void OpenGLScene::redrawPathWay(void)
{	
	for (int i = 0; i < _pathBubbles.size(); i ++)		
	{
		if(_pathBubbles[i]->toplevel)
		{
			if(_compartmentName[_pathBubbles[i]->m_pathwayID][_compartmentName[_pathBubbles[i]->m_pathwayID].size()-1][0]=="whole")
				_compartmentName[_pathBubbles[i]->m_pathwayID].resize(_compartmentName[_pathBubbles[i]->m_pathwayID].size()-1);
			
			_pathBubbles[i]->preLocateNodes(_pathBubbles[i]->complexPos,_pathBubbles[i]->proteinPos,_pathBubbles[i]->smallMoleculePos);			
			_pathBubbles[i]->stepTracked.clear();

			/*
			 for(int j=1; j<_pathBubbles[i]->_pathwayStep.size();j++)
			{
				if(_pathBubbles[i]->stepTracked.find(j)==_pathBubbles[i]->stepTracked.end())
					_pathBubbles[i]->trackstep(j, _pathBubbles[i]->_pathwayStep, _pathBubbles[i]->_3pathwayStepCatalysis, _pathBubbles[i]->_3pathwayStepReaction, _biochemicalReaction[_pathBubbles[i]->m_pathwayID]);
			}*/
			_pathBubbles[i]->ParserTraceSteps();
			//pwdParser->TraceSteps( _pathBubbles[i]->_pathwayStep, _pathBubbles[i]->edge, _pathBubbles[i]->reactionPos, _pathBubbles[i]->_1pathway, _pathBubbles[i]->_5Control, _pathBubbles[i]->_Catalysis, _pathBubbles[i]->_3pathwayStepCatalysis, _pathBubbles[i]->_3pathwayStepReaction, _biochemicalReaction[_pathBubbles[i]->m_pathwayID], drawSmallmolecule);
			_pathBubbles[i]->updateVisibleNodes();	
			CompartmentContain[i]=_pathBubbles[i]->CompartmentContain;
			_pathBubbles[i]->HierarchicalLayout(_pathBubbles[i]->_whole, _pathBubbles[i]->reactionPos);	
			_pathBubbles[i]->RemoveDisabledNodes(_pathBubbles[i]->_whole, _pathBubbles[i]->_VertexInfo, _pathBubbles[i]->_Vertex, _pathBubbles[i]->Rank0, _pathBubbles[i]->Ranks);	
			_pathBubbles[i]->HierarchicalTreeNodeLocation(_pathBubbles[i]->_whole, _pathBubbles[i]->_VertexInfo, _pathBubbles[i]->Rank0, _pathBubbles[i]->Ranks);
			_pathBubbles[i]->updateBubbleSize_2();	
			backupItemPos(_pathBubbles[i]->m_pathwayID, _pathBubbles[i]->complexPos, _pathBubbles[i]->proteinPos, _pathBubbles[i]->smallMoleculePos, _pathBubbles[i]->DnaPos, _pathBubbles[i]->EmptyPos, _pathBubbles[i]->reactionPos, _pathBubbles[i]->physicalEntityPos, _pathBubbles[i]->compartmentPos, _pathBubbles[i]->_scale, _pathBubbles[i]->dCenter, _pathBubbles[i]->drawSmallmolecule);
			backupRanks(_pathBubbles[i]->m_pathwayID, _pathBubbles[i]->Rank0, _pathBubbles[i]->Ranks, _pathBubbles[i]->_newPos, _pathBubbles[i]->_newCompartmentPos);
		}		
	}	
}


void OpenGLScene::buildSubGraphBubbleConnect( QDomElement node )
{	
	QDomNodeList paraNode;	
	QStringList posStr;
	QString str;


	QDomElement tmpNode;

	paraNode = node.elementsByTagName( tr("subGraphBubbles") );
	
	if ( !paraNode.isEmpty() )
	{
		//paraNode = paraNode.at(0).toElement().elementsByTagName( tr("edge") );		
		paraNode = paraNode.at(0).toElement().elementsByTagName( tr("connection") );
		int size=paraNode.size();
		for (int i = 0; i < paraNode.size(); i ++)
		{
			int pid0,pid1,id0,id1,type0,type1;
			vector<int> link1,link0;
			set<vector<int>> link;

			QDomNodeList posNode = paraNode.at(i).toElement().elementsByTagName( tr("connection") );
			posStr= paraNode.at(i).toElement().text().split( "," );
			
			str = posStr.at(0);			
			pid0 = str.toInt();
			str = posStr.at(1);			
			type0 = str.toInt();
			str = posStr.at(2);
			if(str[0]=='-')
			{
				str=str.mid(1,str.size()-1);	
				id0  = str.toInt();
				id0=-id0;
			}
			else
			{
			    id0  = str.toInt();
			}
			str = posStr.at(3);			
			pid1 = str.toInt();
			str = posStr.at(4);			
			type1 = str.toInt();
			str = posStr.at(5);
			if(str[0]=='-')
			{
				str=str.mid(1,str.size()-1);	
				id1  = str.toInt();
				id1=-id1;
			}
			else
			{
			   id1  = str.toInt();
			}
			link0.push_back(pid0);
			link0.push_back(type0);
			link0.push_back(id0);

			link1.push_back( pid1);
			link1.push_back( type1);
			link1.push_back( id1);
			
			link.insert(link0);
			link.insert(link1);
		    
			edgeconnections.insert(link);			
		}
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
	

	itemList = this->_manager->allMembers();
	for (int i = 0; i < itemList.size(); i ++)
	{
		item = itemList[i];
		int type = item->getType();

		/*#define PATHBUBBLE0 20
#define PATHBUBBLE1 21
#define PATHBUBBLE2 22
#define TEXTBUBBLE2 23
#define REACTIONBUBBLE3 24
#define GROUPBUBBLE4 25*/
		if ( type == PATHBUBBLE1 || type == TEXTBUBBLE2 || type == REACTIONBUBBLE3 || type == GROUPBUBBLE4)
		{
			/*if ( type == MATLAB)
			{ 
				MatBlock* tmp = dynamic_cast<MatBlock*>(item);
				if (tmp->getCurrentFilename() == name 
				&& fabs(tmp->pos().x() - x) < 3
				&& fabs(tmp->pos().y() - y) < 3 )
				{
					tmp->rebuildBubbleConnect( node );
					break;
				}
			}
			else */
			if ( type == PATHBUBBLE1)
			{ 
				PathBubble1* tmp = dynamic_cast<PathBubble1*>(item);
				if (tmp->getCurrentFilename() == name &&
					fabs(tmp->pos().x() - x) < 3
				&& fabs(tmp->pos().y() - y) < 3 )
				{
					tmp->rebuildBubbleConnect( node );
					break;
				}
			}
			else if ( type == TEXTBUBBLE2)
			{ 
				TextBubble2* tmp = dynamic_cast<TextBubble2*>(item);
				if (tmp->getCurrentFilename() == name 
				&& fabs(tmp->pos().x() - x) < 3
				&& fabs(tmp->pos().y() - y) < 3 )
				{
					tmp->rebuildBubbleConnect( node );
					break;
				}
			}
			else if ( type == REACTIONBUBBLE3)
			{ 
				ReactionBubble3* tmp = dynamic_cast<ReactionBubble3*>(item);
				if (tmp->getCurrentFilename() == name 
				&& fabs(tmp->pos().x() - x) < 3
				&& fabs(tmp->pos().y() - y) < 3 )
				{
					tmp->rebuildBubbleConnect( node );
					break;
				}
			}
			else if ( type == GROUPBUBBLE4)
			{ 
				GroupBubble4* tmp = dynamic_cast<GroupBubble4*>(item);
				if (tmp->getCurrentFilename() == name 
				&& fabs(tmp->pos().x() - x) < 3
				&& fabs(tmp->pos().y() - y) < 3 )
				{
					tmp->rebuildBubbleConnect( node );
					break;
				}
			}
			
		}
	}
	return;
}

/*
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
}*/

void OpenGLScene::logInfor( QString str )
{
	this->addLogText( str );
}

//vector<QRectF> complexPos, proteinPos, smallMoleculePos, DnaPos, reactionPos, physicalEntityPos;	
void OpenGLScene::backupItemPos(int m_pathwayID, vector<QRectF> complexPos, vector<QRectF> proteinPos, vector<QRectF> smallMoleculePos, vector<QRectF> DnaPos, vector<QRectF> EmptyPos, vector<QRectF> reactionPos, vector<QRectF> physicalEntityPos, vector<QRectF> compartmentPos, float scale, QPointF dCenter, bool drawSmallmoleculeSettings)
{  
	  //for subgraph, there is no edge yet
	  if(m_pathwayID==_complexPos.size())
	  {
		  _complexPos.push_back(complexPos), _proteinPos.push_back(proteinPos), _smallMoleculePos.push_back(smallMoleculePos), _DnaPos.push_back(DnaPos),  _EmptyPos.push_back(EmptyPos), _reactionPos.push_back(reactionPos), _physicalEntityPos.push_back(physicalEntityPos);
		  _compartmentPos.push_back(compartmentPos);
		  _Scale.push_back(scale);
		  _dCenter.push_back(dCenter);		  
		  drawSmallmolecule.push_back(drawSmallmoleculeSettings);
	  }
	  else if(m_pathwayID<_complexPos.size())
	  {
	      _complexPos[m_pathwayID]=complexPos, _proteinPos[m_pathwayID]=proteinPos, _smallMoleculePos[m_pathwayID]=smallMoleculePos, _DnaPos[m_pathwayID]=DnaPos,  _EmptyPos[m_pathwayID]=EmptyPos, _reactionPos[m_pathwayID]=reactionPos, _physicalEntityPos[m_pathwayID]=physicalEntityPos;
		  _compartmentPos[m_pathwayID]=compartmentPos;
		  _Scale[m_pathwayID]=scale;
		  _dCenter[m_pathwayID]=dCenter;
		 
		  drawSmallmolecule[m_pathwayID]=drawSmallmoleculeSettings;	  
	  }
	  else
		  return;
}

void OpenGLScene::backupRanks(int m_pathwayID, vector<vector<int>> _Rank0, vector<vector<vector<int>>> _Ranks, vector<QRectF> _newPos, vector<QRectF> _newCompartmentPos)
{
	if(m_pathwayID==Rank0.size())
	{
		  Rank0.push_back(_Rank0); //for compartment
		  Ranks.push_back(_Ranks);
		  newPos.push_back(_newPos);
		  newCompartmentPos.push_back(_newCompartmentPos);
	}
	else if(m_pathwayID<Rank0.size())
	{
		  Rank0[m_pathwayID]=_Rank0; //for compartment
		  Ranks[m_pathwayID]=_Ranks;
		  newPos[m_pathwayID]=_newPos;
		  newCompartmentPos[m_pathwayID]=_newCompartmentPos;
	}

}


void OpenGLScene::backupEdge(int m_pathwayID, vector<vector<int>> edge)
{
	_edge.push_back(edge);
}

void OpenGLScene::resetEdge(int m_pathwayID, vector<vector<int>> &edge)
{
	edge=_edge[m_pathwayID];
}

void OpenGLScene::resetItemPos(vector<QRectF> &pos1, vector<QRectF> pos2)
{
	for(int i=0; i<pos1.size(); i++)
	{
	    if(pos1[i].x()>-990.99)
			pos1[i]=pos2[i];	
	}
}

void OpenGLScene::resetItemPos(int m_pathwayID, vector<QRectF> &complexPos, vector<QRectF> &proteinPos, vector<QRectF> &smallMoleculePos, vector<QRectF> &DnaPos, vector<QRectF> &EmptyPos, vector<QRectF> &reactionPos, vector<QRectF> &physicalEntityPos,vector<QRectF> &compartmentPos, vector<vector<int>> &edge, vector<vector<int>> &_Rank0, vector<vector<vector<int>>> &_Ranks, vector<QRectF> &_newPos, vector<QRectF> &_newCompartmentPos, float &scale, QPointF &dCenter, bool &drawSmallmoleculeSettings)
{	
	//complexPos=_complexPos[m_pathwayID], proteinPos=_proteinPos[m_pathwayID], smallMoleculePos=_smallMoleculePos[m_pathwayID], DnaPos=_DnaPos[m_pathwayID], reactionPos=_EmptyPos[m_pathwayID], reactionPos=_reactionPos[m_pathwayID], physicalEntityPos=_physicalEntityPos[m_pathwayID], edge=_edge[m_pathwayID];
	resetItemPos(complexPos, _complexPos[m_pathwayID]);
	resetItemPos(proteinPos, _proteinPos[m_pathwayID]);
	resetItemPos(smallMoleculePos, _smallMoleculePos[m_pathwayID]);
	resetItemPos(DnaPos, _DnaPos[m_pathwayID]);
	resetItemPos(EmptyPos, _EmptyPos[m_pathwayID]);
	resetItemPos(physicalEntityPos, _physicalEntityPos[m_pathwayID]);
	resetItemPos(reactionPos, _reactionPos[m_pathwayID]);
	
	//compartmentPos=_compartmentPos[m_pathwayID];
	resetItemPos(compartmentPos, _compartmentPos[m_pathwayID]);	
 	
	scale=_Scale[m_pathwayID], dCenter=_dCenter[m_pathwayID];	
	_Rank0=Rank0[m_pathwayID]; //for compartment
    _Ranks=Ranks[m_pathwayID];
	_newPos=newPos[m_pathwayID];
	_newCompartmentPos=newCompartmentPos[m_pathwayID];
	drawSmallmoleculeSettings=drawSmallmolecule[m_pathwayID];
}

void OpenGLScene::resetRanks(int m_pathwayID, vector<vector<int>> &_Rank0, vector<vector<vector<int>>> &_Ranks, vector<QRectF> &_newPos, vector<QRectF> &_newCompartmentPos)
{
	_Rank0=Rank0[m_pathwayID]; //for compartment
    _Ranks=Ranks[m_pathwayID];
	_newPos=newPos[m_pathwayID];
	_newCompartmentPos=newCompartmentPos[m_pathwayID];
}
	