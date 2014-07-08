#include "openglscene.h"
#include "qtbox.h"
#include "qtcolorbar.h"
#include "moveButton.h"
//#include "windows.h"
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
#define MEDIUM_HIGHLIGHTCOLOR QColor(255,128,0,255) //QColor(20,20,255,255)

QDialog* OpenGLScene::createDialog(const QString &windowTitle) const
{
	QDialog *dialog = new QDialog(0, Qt::CustomizeWindowHint | Qt::WindowTitleHint );
	dialog->setWindowOpacity(1.0);
	dialog->setWindowTitle(windowTitle);
	dialog->setLayout(new QVBoxLayout);	
	dialog->resize(1000,1000);
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
, drawCrossTalk(true)
, drawRateLimit(true)
, searchShared(true)
, searchDiffered(false)
, searchLinked(false)
, searchExpressed(false)		
, searchType('P')
, drawDna(false)
, bubbleIndex( 0 )
, graphReferenceSize(600)
, fileSelectionOpened(false)
, menuloaded(false)
, loadMenuOpened(false)
, needToReOpenMenu(false)
, controlKeyisOn(false)
, delKeyisOn(false)
 ,shiftKeyisOn(false)
, bubbleDraggedout(false)
, m_rightButton(false)
, linkSearchType(-1)
//, rightClickFlag(0)
//, PenisOn(0)
{

	_wholeGraph = new myGraph();
	/*QDesktopWidget *mydesk = QApplication::desktop () ;
    screenWidth=mydesk->screenGeometry().width(); 
	screenHeight=mydesk->screenGeometry().height();*/
	
	lastPressedPos = QPointF(0,-1000000);
	openCount=0;
	this->_fileSelection=NULL;
	//setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
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

	char username[200];
	DWORD username_len = 200;
	GetUserName(username, &username_len);
	userName = username;

	lab = input2;
	cut = input3;

	ani = new animateIt( 500 );
	order = new OrderManager();

	//allbats = new AllBats();

	_manager = new GroupManager( this );

	this->_perfix = TDHelper::getDataPrefix();

	_openManager = new OpenManager( this, lab, cut, _manager);

	initFloatingControl();

	/*int dl,num;
	float mitv;
	float min=0, max=0;
	TDHelper::getLegendParameter(min, max, 'F', 10, num, dl, mitv);*/
	//TDHelper::getLegendParameter(eMin, eMax, 'F', 10, eNum, eDL, eItv);

	pwdParser->readRateLimitSymbol("data/Reactome_Pathway_Data/ratelimitsymbol.txt", rateLimitSymbol);
	pwdParser->readCrossTalkSymbol("data/Reactome_Pathway_Data/crosstalksymbol.txt", crossTalkSymbol);
	//pwdParser->readOrthologySymbol("data/gallus_gallus_ortholog.txt", orthologySymbol);
		
	//recoverSceneFromXmlFile("xmlTest/videoTest.xml");
	m_time.start();
	//
	this->timer.start( 30 );
	connect( &timer, SIGNAL(timeout()), this, SLOT(update()) );
	connect( this, SIGNAL(pathBubbleStarted(QRectF)), this, SLOT(showPathBubble(QRectF)) );	
	connect( this, SIGNAL(prePathBubbleStarted(QRectF)), this, SLOT(showPreOpentRect(QRectF)) );	
	connect( this, SIGNAL(pleaseReopenMenu()), this, SLOT(reopenMenu()) );	
	menuImage0 = QPixmap( tr("icon/menu0.bmp") );
	menuImage1 = QPixmap( tr("icon/menu1.png") );
	pathwayMenuImage = QPixmap( tr("icon/pathwayMenu.png") );
	
    //for redo undo
	//vector< vector<int> > emptyEvent;
	//eventStored.push_back(emptyEvent);
	//currentEventit=eventStored.begin();	
	clearTempGraphFile();
	_colorID=-1;
	openAutoTreeRingBubble(120, 160, false);
}

void OpenGLScene::clearTempGraphFile()
{
	 QString fileName = "";//fileComboBox->currentText();//"_1pathway" only work on emapty "" case
     QString text = "";//textComboBox->currentText();
    
     //First we eliminate any previous search results by setting the table widgets row count to zero. Then we retrieve the specified file name, text and directory path from the respective comboboxes.
	 int count=0;
     QDir currentDir = QDir("data/Reactome_Pathway_Data/tempPathwayGraph");
     QStringList files;
     if (fileName.isEmpty())
         fileName = "*";
    files = currentDir.entryList(QStringList(fileName),
                                  QDir::Files | QDir::NoSymLinks);

    if (!text.isEmpty())
         files = pwdParser->findFiles(currentDir, files, text);
	
	QString pureName;
	QString num;
	int lid;	

	for (int i = 0; i < files.size(); ++i) 
	{
	        lid=files[i].lastIndexOf('.');		
			currentDir.remove(files[i]);
	}
}


OpenGLScene::~OpenGLScene()
{
    /*struct dirent *next_file;
    QDir currentDir= QDir("data/Reactome_Pathway_Data/temp_PathwayGraph");
    //QDir currentDir = QDir(path);
    char filepath[256];
	 QStringList files;

    //theFolder = QOpenDir("data/Reactome_Pathway_Data/temp_PathwayGraph");
	 files = currentDir.entryList(QStringList(""),
                                  QDir::Files | QDir::NoSymLinks);

    for (int i = 0; i < files.size(); ++i) 
	{
	     currentDir.remove(files[i]);
	}
    
	
	QGraphicsScene::~QGraphicsScene();*/
}

void OpenGLScene::showPreOpentRect(QRect rect)
{
	preOpenRects.push_back(rect);
	
}

void OpenGLScene::showPathBubble(QRect rect)
{
	/*QString name = "data/pathways/TGF";
	if( name.size() > 0 )
	{		
			ItemBase* item;
	        item = NULL;								
			getOpenManager()->openPathwayByPath(item, 200, rect.x(), rect.y(),name);			
	}*/	
}

/*
void OpenGLScene::webBubbleResized(ItemBase * item)
{
	if(item->getType()==WEB)
	{
		WebBlock* wbubble =  dynamic_cast<WebBlock*>(item); 
		wbubble->resizeItem( item->Width(), item->Height());
	}
    else if( item->getType()==NOTE )
	{
		NoteBlock* nbubble =  dynamic_cast<NoteBlock*>(item); 
		nbubble->resizeItem( item->Width(), item->Height());
	}
}
*/

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

ItemBase *OpenGLScene::findBubbleUnderNote( QPointF eventPos )
{
	ItemBase * item;
	QList<ItemBase*> list = this->allMembers();
	for( int i = 0; i < list.size(); i++ )
	{
		if(list[i]->getType()!=CALLOUT)
		{
			if( list[i]->sceneBoundingRect().contains( eventPos ) )
			{
				return list[i];
			}
		}
	}
	return NULL;
}

ItemBase *OpenGLScene::findBubbleUnderAlocation( QPointF eventPos )
{
	ItemBase * item;
	QList<ItemBase*> list = this->allMembers();
	for( int i = 0; i < list.size(); i++ )
	{
		if(list[i]->getType()!=CALLOUT)
		{
			QRectF rect=list[i]->sceneBoundingRect();
			float w=list[i]->realRect().width(), h=list[i]->realRect().height();
			rect=QRectF(rect.center().x()-w/2, rect.center().y()-h/2, w, h);
			if( rect.contains( eventPos ) )
			{
				return list[i];
			}
		}
	}
	return NULL;
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
	//this->connect( _navi, SIGNAL(itemPosUpdated(ItemBase *)), this->_manager, SLOT(itemPosUpdated(ItemBase*)) );
	_navi->passGroupManager(_manager);
	
	this->addItem( _navi );
	_navi->setZValue( 99 );
	_navi->logInit();

    pwdParser->findFillsinFolder(); //make pathwayID file, make .path file, make the cross-talking protein-to-pathwayList
	//file selection
	this->_fileSelection = new fileSelection( QSizeF( 200, 200 ), this, _openManager ); //QSizeF( 846, 216 )
	connect( _fileSelection, SIGNAL(orderByWeight()), this, SLOT(orderViews()) );
	connect( _fileSelection, SIGNAL(orderBySpeed()), this, SLOT(mapViews()) );
	connect( _fileSelection, SIGNAL( logInfor( QString ) ), this, SLOT( logInfor( QString ) ) );
	this->addItem( _fileSelection );
	_fileSelection->setZValue( 99 );
	_fileSelection->logInit();
	//_fileSelection->setVisible(false);
	_fileSelection->setDiaVisible(true);
	//_fileSelection->_myControl->setVisible(true);
	_fileSelection->_myControl->setPos(-2000,-1000); //keep it visible but in a unseem location, otherwise, the menu will disappear when show it		
		
	//file control
	//*************************************************************************
	/*this->_file = new FileManager( QSizeF( 600, 200 ), this, _openManager );
	connect( _file, SIGNAL(orderByWeight()), this, SLOT(orderViews()) );
	connect( _file, SIGNAL(orderBySpeed()), this, SLOT(mapViews()) );
	connect( _file, SIGNAL( logInfor( QString ) ), this, SLOT( logInfor( QString ) ) );
	this->addItem( _file );
	_file->setZValue( 99 );
	_file->logInit();
	this->_file->setVisible(true);*/

	QDesktopWidget *mydesk = QApplication::desktop () ;
    int screenWidth=mydesk->screenGeometry().width(); 
	int screenHeight=mydesk->screenGeometry().height();

	this->_colorPanel = new SymbolColorPanel( QSizeF(screenWidth, 166), 0, screenHeight-166, this, _openManager );	
	this->addItem( _colorPanel );
	_colorPanel->setZValue( 99 );
	_colorPanel->logInit();
	this->_colorPanel->setVisible(true);

	//statistic bar
	//*************************************************************************
	/*this->_statistic = new StatisticManager( "data/statistical.txt" );
	this->_staBar = new StatisticBar( QSize(220, 630), QPoint( 100, 200 ), this->_statistic );
	this->connect( _staBar, SIGNAL( logInfor( QString ) ), this, SLOT( logInfor( QString ) ) );
	connect( _staBar, SIGNAL( needStatisticalData( QPointF, StatisticManager*, int ) ), this, SLOT(needStatisticalData( QPointF, StatisticManager*, int)) );
	this->addItem( _staBar );
	this->_staBar->setZValue( 100 );
	_staBar->logInit();
	*/

	//wing graphics bar
	//*************************************************************************
	/*this->_arrow = new ArrowGen( QSizeF(200, 100), this );
	this->addItem( _arrow );
	this->_arrow->setZValue( 99 );
	connect( _arrow, SIGNAL( setWingShapePara( ArrowGen*, QPointF ) ), this, SLOT( setWingShapePara( ArrowGen*, QPointF ) ) );
	connect( _arrow, SIGNAL( logInfor( QString ) ), this, SLOT( logInfor( QString ) ) );
	_arrow->logInit();*/
	
	return;
}

bool isadjacency_node_not_present_in_current_path(int node,vector<int>path)
{
    for(int i=0;i<path.size();++i)
    {
        if(path[i]==node)
        return false;
    }
    return true;
}

int findNodeID(vector<int> node, vector<vector<int>> nodeVector)
{
	for(int i=nodeVector.size()-1; i>=0; i--)
	{
	   if(node==nodeVector[i])
		   return i;	
	}
	return -1;
}


vector<QColor> OpenGLScene::getPathEndsColor(int pid, int type, int id)
{
	vector<QColor> pColor;
	if(linkSearchType == 1)	
	{
	  	vector<int> node;
		node.push_back(pid); node.push_back(type); node.push_back(id);
		//node2.push_back(pid2); node2.push_back(type2); node2.push_back(id2);

		bool flag=false;
		for(int i= 0; i< pathEnds[pid].size(); i++)
		{
			for(int j=0; j<pathEnds[pid][i].size(); j++ )
			{
				if(pathEnds[pid][i][j] == node)
				{
					pColor.push_back(_wholeGraph->getPathColor(i));
					flag=true;
					break;
				}
			}			
		}		
		if(!flag)
			flag=flag;
	}	
	return pColor;
}

bool OpenGLScene::isPathEnds(int pid, int type, int id)
{
	vector<QColor> pColor;
	if(linkSearchType == 1)	
	{
	  	vector<int> node;
		node.push_back(pid); node.push_back(type); node.push_back(id);
		//node2.push_back(pid2); node2.push_back(type2); node2.push_back(id2);

		bool flag=false;
		if(pathEnds.size()>pid)
		for(int i= 0; i< pathEnds[pid].size(); i++)
		{
            if(pathEnds[pid][i][0]==node || pathEnds[pid][i][pathEnds[pid][i].size()-1]==node)
				return true;			
		}		
	}	
	return false;
}

vector<QColor> OpenGLScene::getPathColor(int pid1, int type1, int id1, int pid2, int type2, int id2)
{
	vector<QColor> pColor;
	if(linkSearchType == 1)	
	{
	  	int pid, type, id; 
		if(pid2<pid1)
			pid=pid2, type=type2, id=id2;
		else 
			pid=pid1, type=type1, id=id1;

		vector<int> node1, node2;
		node1.push_back(pid1); node1.push_back(type1); node1.push_back(id1);
		node2.push_back(pid2); node2.push_back(type2); node2.push_back(id2);

		vector < vector <int> > aEdge;
		aEdge.push_back(node1);   aEdge.push_back(node2);
		bool flag=false;
		for(int i= 0; i<pathVectors[pid1].size(); i++)
		{
			if(pathVectors[pid1][i].find(aEdge) != pathVectors[pid1][i].end())
			{
		        pColor.push_back(_wholeGraph->getPathColor(i));
				flag=true;
			}
		}
		if(!flag)
		   pColor.push_back(_wholeGraph->getPathColor(5));		
	}
	else 
		pColor.push_back(_wholeGraph->getPathColor(5));

	return pColor;
}

void OpenGLScene::updateGraphs()
{
	int num=0;
	//get graph size
	for(int i=0; i<_pathBubbles.size();i++)
	{        
		if( _pathBubbles[i] )
		if( _pathBubbles[i]->m_pathwayID>=0 && _pathBubbles[i]->m_pathwayID<_pathBubbles.size()) 
		{			
			num +=  _pathBubbles[i]->boundaryWidth.size()+1;
		}
	}
	set<vector<int>> nodeSet;
	vector<vector<int>> nodeVector;
	vector<vector<int>> GRAPH;
	vector<int> node1(3,-1), node2(3,-1), edge_t(2,-1);	
	nodeVector.push_back(node1); //let the rest start with 1
	
	for(int i=0; i<_pathBubbles.size();i++)
	{
		if( _pathBubbles[i] )
		if( _pathBubbles[i]->m_pathwayID>=0 && _pathBubbles[i]->m_pathwayID<_pathBubbles.size()) 
		{
			
			//get nodes and edges 
			PathBubble1 *pBubbles=_pathBubbles[i];
			for(set<vector<vector<int> > >::iterator it=pBubbles->updatedEdge_1.begin(); it!=pBubbles->updatedEdge_1.end(); it++)
			{		
				int size=pBubbles->updatedEdge_1.size();
				vector<vector<int> > edge=*it;						
				if(!pBubbles->inUpdatedEdge(edge, pBubbles->updatedEdge_4))
				{
					if(nodeSet.find(edge[0])==nodeSet.end())
					{
						nodeSet.insert(edge[0]);
						nodeVector.push_back(edge[0]);
					}
					if(nodeSet.find(edge[1])==nodeSet.end())
					{
						nodeSet.insert(edge[1]);
						nodeVector.push_back(edge[1]);
					}
						
					edge_t[0]=findNodeID(edge[0], nodeVector);
					edge_t[1]=findNodeID(edge[1], nodeVector);

					if(edge_t[0]>=GRAPH.size())		GRAPH.resize(edge_t[0]+1);
					if(edge_t[1]>=GRAPH.size())		GRAPH.resize(edge_t[1]+1);
			
					GRAPH[edge_t[0]].push_back(edge_t[1]);
				}	
			}
			for(set<vector<vector<int> > >::iterator it=pBubbles->updatedEdge_2.begin(); it!=pBubbles->updatedEdge_2.end(); it++)
			{   //activation
				vector<vector<int> > edge=*it;
				if(!pBubbles->inUpdatedEdge(edge, pBubbles->updatedEdge_4))
				{
					if(nodeSet.find(edge[0])==nodeSet.end())
					{
						nodeSet.insert(edge[0]);
						nodeVector.push_back(edge[0]);
					}
					if(nodeSet.find(edge[1])==nodeSet.end())
					{
						nodeSet.insert(edge[1]);
						nodeVector.push_back(edge[1]);
					}
						
					edge_t[0]=findNodeID(edge[0], nodeVector);
					edge_t[1]=findNodeID(edge[1], nodeVector);

					if(edge_t[0]>=GRAPH.size())		GRAPH.resize(edge_t[0]+1);
					if(edge_t[1]>=GRAPH.size())		GRAPH.resize(edge_t[1]+1);
			
					GRAPH[edge_t[0]].push_back(edge_t[1]);
				}
			}
			
	
			for(set<vector<vector<int> > >::iterator it=pBubbles->updatedEdge_3.begin(); it!=pBubbles->updatedEdge_3.end(); it++)
			{
				vector<vector<int> > edge=*it;		
				if(!pBubbles->inUpdatedEdge(edge, pBubbles->updatedEdge_4))
				{
					if(nodeSet.find(edge[0])==nodeSet.end())
					{
						nodeSet.insert(edge[0]);
						nodeVector.push_back(edge[0]);
					}
					if(nodeSet.find(edge[1])==nodeSet.end())
					{
						nodeSet.insert(edge[1]);
						nodeVector.push_back(edge[1]);
					}
						
					edge_t[0]=findNodeID(edge[0], nodeVector);
					edge_t[1]=findNodeID(edge[1], nodeVector);

						if(edge_t[0]>=GRAPH.size())		GRAPH.resize(edge_t[0]+1);
					if(edge_t[1]>=GRAPH.size())		GRAPH.resize(edge_t[1]+1);
			
					GRAPH[edge_t[0]].push_back(edge_t[1]);
				}	
			}			     
			
	
			int eid=0;
			for(set<vector<vector<int> > >::iterator it=pBubbles->updatedEdge_4.begin(); it!=pBubbles->updatedEdge_4.end(); it++)
			{//multi-link
				vector<vector<int> > edge=*it;		   	    	 
				if(nodeSet.find(edge[0])==nodeSet.end())
				{
					nodeSet.insert(edge[0]);
					nodeVector.push_back(edge[0]);
				}
				if(nodeSet.find(edge[1])==nodeSet.end())
				{
					nodeSet.insert(edge[1]);
					nodeVector.push_back(edge[1]);
				}	
				edge_t[0]=findNodeID(edge[0], nodeVector);
				edge_t[1]=findNodeID(edge[1], nodeVector);

				if(edge_t[0]>=GRAPH.size())		GRAPH.resize(edge_t[0]+1);
				if(edge_t[1]>=GRAPH.size())		GRAPH.resize(edge_t[1]+1);
			
				GRAPH[edge_t[0]].push_back(edge_t[1]);
			}		    
			//int totalnode=nodeVector.size()-1, totaledge=GRAPH.size()-1;	
		}
	}
	set<vector<vector<int>>> connections;
	for(int i=0; i<3; i++)
	{
		switch(i)
		{
			case 0: connections=edgeconnections;  break;
		    case 1: connections=edgeconnections_A;  break;
			case 2: connections=edgeconnections_I;  break;
			case 3: connections=edgeconnections_4;  break; //no need edgeconnections_4
		}
		for (set<vector<vector<int>>>::iterator it = connections.begin(); it != connections.end(); it++)
		{
			vector<vector<int>> link = *it;	
			//bool isLinkedEdge=false;
			vector<int> link0=*link.begin(),link1=*link.rbegin();
			int pid1=link0[0], pid2=link1[0], type1=link0[1], type2=link1[1], id1=link0[2], id2=link1[2];
			
			
			QRectF itemPos1,itemPos2; 
			PathBubble1* path1=_pathBubbles[pid1], * path2=_pathBubbles[pid2];

			if(!path1 || !path2)
				continue;

			if(!path1->isVisible() || !path2->isVisible())
				continue;
		
			vector<vector<int> > edge;//, rEdge;
			edge.push_back(link0);  edge.push_back(link1);	

		    if(nodeSet.find(edge[0])==nodeSet.end())
			{
				nodeSet.insert(edge[0]);
				nodeVector.push_back(edge[0]);
			}
			if(nodeSet.find(edge[1])==nodeSet.end())
			{
				nodeSet.insert(edge[1]);
				nodeVector.push_back(edge[1]);
			}	
			edge_t[0]=findNodeID(edge[0], nodeVector);
			edge_t[1]=findNodeID(edge[1], nodeVector);

			if(edge_t[0]>=GRAPH.size())		GRAPH.resize(edge_t[0]+1);
			if(edge_t[1]>=GRAPH.size())		GRAPH.resize(edge_t[1]+1);
			
			GRAPH[edge_t[0]].push_back(edge_t[1]);
		}
	}

	_wholeGraph->_Graph = GRAPH;
	_wholeGraph->totalNode = nodeVector.size()-1;
	_wholeGraph->totalEdge = GRAPH.size()-1;
	_wholeGraph->nodeVector=nodeVector;

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

	for(int i=0; i<m_treeRingBubbles.size();i++)
	{
		if( m_treeRingBubbles[i] && !m_treeRingBubbles[i]->miniMized)		
		{
            if(m_treeRingBubbles[i]->m_treeRingBubbleID>=0)
			  m_treeRingBubbles[i]->drawConnections(painter, m_treeRingBubbles[i]->sceneBoundingRect().center());
			else
			{
				m_treeRingBubbles[i]->deleteSelectedItems(this);
				m_treeRingBubbles[i]=NULL;
			}
		}
	}

	for(int i=0; i<_eGiftBubbles.size();i++)
	{
		if( _eGiftBubbles[i] )		
		{
			
			if(_eGiftBubbles[i]->_pid>=0)
			if(_pathBubbles[_eGiftBubbles[i]->_pid] && _pathBubbles[_eGiftBubbles[i]->_pid]->m_pathwayID>=0 && _pathBubbles[_eGiftBubbles[i]->_pid]->isVisible())
                 _eGiftBubbles[i]->drawConnections(painter, _pathBubbles[_eGiftBubbles[i]->_pid]->sceneBoundingRect().center());
		}
		else
		{
			//_eGiftBubbles[i]->deleteSelectedItems(this);
			_eGiftBubbles[i]=NULL;
		
		}
	}

	QList<Group*> groups=_manager->allGroups();
	
	//draw sharedset the connector between shared items (after searching "shared")
	for(int j=0; j<groups.size();j++)
	{
		vector<set<vector<int>>> sharedSets=groups[j]->sharedSets;		
		for(int i=0; i< sharedSets.size(); i++)
		{
			vector<int> item1(2,0);
	        vector<vector<int>> heads;
			int Pid=-1;
			for(set<vector<int>>::iterator it=sharedSets[i].begin(); it!=sharedSets[i].end(); it++)
			{
				vector<int> item=*it;
				int pid=item[0], type=item[1], id=item[2];
				item1[0]=type; item1[1]=id;
				
				if(_pathBubbles[pid])
				if(Pid==pid && _pathBubbles[pid]->highlighted.find(item1)!=_pathBubbles[pid]->highlighted.end())
				{
					heads.push_back(item);
				}				
			}
			if(heads.empty()&&!sharedSets[i].empty() )
			{
			    heads.push_back(*sharedSets[i].begin());  
			}
			for(int k=0; k<heads.size(); k++)
			{
			    for(set<vector<int>>::iterator it=sharedSets[i].begin(); it!=sharedSets[i].end(); it++)
				{
					vector<int> item=*it;
					if(heads[k]!=item)
					{
						//draw links
						if(_pathBubbles[heads[k][0]])
						_pathBubbles[heads[k][0]]->drawSharedPointers(painter, _pathBubbles[heads[k][0]]->sceneBoundingRect().center(), heads[k], item);
					}
				}
			}
		}
	}
    //draw connector between note and node
	for(int i=0; i<_nodeNote.size(); i++)
	{
		if(_nodeNote[i]!=NULL && !_nodeNote[i]->deleted)
		{
			if(_nodeNote[i]->isVisible())
	           _nodeNote[i]->drawNoteNodeConnector(painter,true);
			else _nodeNote[i]->drawNoteNodeConnector(painter, false);	
		}
	}	
}

void OpenGLScene::drawBackground(QPainter *painter, const QRectF & rect)
{
	//glShadeModel( GL_SMOOTH );
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawSelecting( painter );
		
	int width = this->width();
	int height = this->height();
	QLinearGradient linearGrad(QPointF( width/2, 0 ), QPointF( width/2, height));
	linearGrad.setColorAt(0, QColor( 0, 0, 0, 200) );
	linearGrad.setColorAt(1, QColor( 172, 216, 243, 200 ) );
	//linearGrad.setColorAt(0, QColor( 255, 255, 255, 255) );
	//linearGrad.setColorAt(1, QColor( 255, 255, 255, 255 ) );
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

	this->_manager->drawAllLabel( painter );	
	
	for(int i=0; i<_pathBubbles.size();i++)
	{
		if( _pathBubbles[i] )
		if( _pathBubbles[i]->m_pathwayID>=0 && _pathBubbles[i]->m_pathwayID<_pathBubbles.size()) 
		{
			_pathBubbles[i]->drawConnections_H(painter, _pathBubbles[i]->sceneBoundingRect().center());
		}
	}
	
	for(int i=0; i<_pathBubbles.size();i++)
	{
		if( _pathBubbles[i] )
		if( _pathBubbles[i]->m_pathwayID>=0 && _pathBubbles[i]->m_pathwayID<_pathBubbles.size()) 
		{
			_pathBubbles[i]->drawConnections(painter, _pathBubbles[i]->sceneBoundingRect().center());
		}
	}
	
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
	QGraphicsScene::mouseMoveEvent(event);

	

	/*{
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
	}*/
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

void OpenGLScene::keyPressEvent(QKeyEvent *event)
{
    QGraphicsScene::keyPressEvent(event);
	/*if(event->key()==Qt::Key_Control)
	{
		int keyname=event->key();
	    keyname=keyname;
	}
	int keyname=event->key();
	keyname=keyname;*/
    switch (event->key()) 
	{
		 case Qt::Key_Control: 
			 controlKeyisOn=true;
			 break;
		 case Qt::Key_Shift: 
			 shiftKeyisOn=true;
			 break;
		  case Qt::Key_Delete: 
			 delKeyisOn=true;
			 break;
		 //default: controlKeyisOn=false; break;
	}
}

void OpenGLScene::keyReleaseEvent(QKeyEvent *event)
{
    QGraphicsScene::keyReleaseEvent(event);
    switch (event->key()) 
	{
		 case Qt::Key_Control: 
			 controlKeyisOn=false;
			 break;	
		 case Qt::Key_Shift: 
			 shiftKeyisOn=false;
			 break;
	}
}

void OpenGLScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	lastPressedPos = event->scenePos();
	if (event->button() == Qt::RightButton) 
	{
		m_rightButton=true;
		lastPressedPos = event->scenePos();
	}
	else m_rightButton=false;		

	if(menu.isVisible() && fileSelectionOpened)
	{
		menu.setVisible(false);
	}
	if(!event->isAccepted())
	{
		//if(nodeNote!=NULL)
	    //   nodeNote->setSelected(true);
	}	
	QGraphicsScene::mousePressEvent(event);
	/*if (event->button() == Qt::LeftButton )
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
	*/
	if( !event->isAccepted() && event->button() == Qt::RightButton )
	{
		_MultiSelect = true;
		_initSelectPoint = event->scenePos();
	}
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
	
	    //adjust
		vector<QRectF> cList=rectList;
        cList=rectList;
		float testHeight = cList[initID].height()*1.5;
		cList[initID] = QRectF( cList[initID].x(), cList[initID].center().y()-testHeight/2, cList[initID].width(), testHeight);
		int oid = B->findOverlaid(initID, cList);
		if(oid > 0) //initID is very close to oid
		{
			float cx=rectList[initID].center().x(), cy=rectList[oid].center().y(), w=rectList[initID].width(), h=rectList[oid].height();
		    QPointF tempPos=B->getSmallestDistance(QRectF(cx-w/2,cy-h/2,w,h), rectList[initID]);  	
			//minPos= oldPos + tempPos;			
			rectList[initID]=QRectF(tempPos.x(), tempPos.y(), rectList[initID].width(), rectList[initID].height());	
		}
		vector<QRectF> aList;

		if(flag)
		{
			list[initID]->mySetPos( bList[initID].center());		
			
		}
	    else 
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
					if(list[i]->getType()!=CALLOUT)
					{
						list[i]->mySetPos( QPointF(x, y) - tempPos);	
						rectList[i].moveCenter(QPointF(x, y) - tempPos);
					}
			}		   
		}
	}
	_manager->lineUpDisplayGroups(list, rectList, true); 
	delete B;
	return QPoint( rectList[initID].x(), rectList[initID].y() );   
}


QPoint OpenGLScene::getGroupedBubblePosition(ItemBase *newItem, ItemBase *parent, int x, int y, int width, int height, bool setPos)
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
		else
		{
		   rectList[i]=QRectF(x,rectList[i].y(),rectList[i].width(), rectList[i].height());
		}
	}		
	familySets.push_back(initSet);		
	set<int> initSetF;
	initSetF.insert(0);
	set<int> tSet;
	tSet.insert(initID); 
	familySets.push_back(tSet);

	vector<int> parentList = _manager->getParentList(list, familySets,parent);		
	vector<QRectF> bList;
	bList = B->lineUpDisplay(SUBBUBBLECREATED,list, parentList, initSetF, list.size(), familySets, rectList, list[0]->screenWidth*0.1, list[0]->screenHeight*0.9, QRectF(-10000000,-10000000,-10000000,-10000000), 1, 60, 60);//list should include initSet	
	float disx = fabs(bList[initID].x() - rectList[initID].x());
	float disy = fabs(bList[initID].y() - rectList[initID].y());
	//if( B->isbridgeBroken(rectList) || (disx<rectList[initID].width()/2.0 && disy<rectList[initID].height()/2.0) )
	{
		//float x = rectList[initID].x() == bList[initID].x()? list[initID]->pos().x() : float(bList[initID].x())+float(bList[initID].width())/2.0;
		//float y = rectList[initID].y() == bList[initID].y()? list[initID]->pos().y() : float(bList[initID].y())+float(bList[initID].height())/2.0;
		
	    if(setPos)	
			list[initID]->mySetPos( bList[initID].center());					
		else 
			list[initID]->setTargetPos( bList[initID].center());					
		rectList[initID].moveCenter(bList[initID].center());						
	}		
	_manager->lineUpDisplayGroups(list, rectList, true); 	
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

		vector<int> parentList = _manager->getParentList(list, familySets,parent);		
		vector<QRectF> bList;
		bList = B->lineUpDisplay(SUBBUBBLECREATED, list, parentList, initSetF, list.size(), familySets, rectList, list[0]->screenWidth, list[0]->screenHeight, QRectF(-10000000,-10000000,-10000000,-10000000));//list should include initSet	
		float disx = fabs(bList[initID].x() - rectList[initID].x());
		float disy = fabs(bList[initID].y() - rectList[initID].y());
		if( B->isbridgeBroken(rectList) || (disx<rectList[initID].width()/2.0 && disy<rectList[initID].height()/2.0) )
		{
			//float x = rectList[initID].x() == bList[initID].x()? list[initID]->pos().x() : float(bList[initID].x())+float(bList[initID].width())/2.0;
			//float y = rectList[initID].y() == bList[initID].y()? list[initID]->pos().y() : float(bList[initID].y())+float(bList[initID].height())/2.0;
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
	

	//further adjust
    vector<QRectF> cList=rectList;
	float testHeight = cList[initID].height()*1.5;
	cList[initID] = QRectF( cList[initID].x(), cList[initID].center().y()-testHeight/2, cList[initID].width(), testHeight);
	int oid = B->findOverlaid(initID, cList);
	if(oid > 0) //Rect initID is very close to Rect oid
	{
		float cx=rectList[initID].center().x(), cy=rectList[oid].center().y(), w=rectList[initID].width(), h=rectList[oid].height();
		QPointF tempPos=B->getSmallestDistance(QRectF(cx-w/2,cy-h/2,w,h), rectList[initID]);  	
		//minPos= oldPos + tempPos;			
		rectList[initID]=QRectF(tempPos.x(), tempPos.y(), rectList[initID].width(), rectList[initID].height());	
	}
	vector<QRectF> aList;

	if(flag)
	{
		list[initID]->mySetPos( rectList[initID].center());	
	}	
	else
	{
		initSet.clear();
		familySets.clear();
		familySets=_manager->GroupToFamily(list);//, init);
		int initIDF=B->getFamilyID(initID, familySets);			
		initSet.insert(initIDF); 
		vector<int> parentList = _manager->getParentList(list, familySets,parent);	
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

				QRectF preRect=newItem->sceneBoundingRect();
				QRectF targetRect=preRect;
				QPointF newCenter=QPointF(x + newItem->sceneBoundingRect().width()/2, y + newItem->sceneBoundingRect().height()/2);
				targetRect.moveCenter(newCenter);
				QPointF stepSize=_manager->getSynchronizedSpeed(newItem, preRect, targetRect);	

				//if(setPos)
				//	list[i]->mySetPos( QPointF(x, y));		
				//else 
				if(list[i]->getType()!=CALLOUT)
				{
					list[i]->mySetPos( QPointF(x, y) - tempPos);	

					//list[i]->setTargetPos( QPointF(x, y) - tempPos, stepSize, true);	//cause problem like repeated add to group
					rectList[i].moveCenter(QPointF(x, y) - tempPos);
				}
			}		   
		}
	}
	_manager->lineUpDisplayGroups(list, rectList, true); 
	delete B;
	return QPoint( rectList[initID].x(), rectList[initID].y() );
}

QPoint OpenGLScene::getSonPositionforTreeRing(ItemBase *newItem, ItemBase *parent, int x, int y, int width, int height)
{  
	//before newitem is added into group
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
	//if(rectList[initID].x()<spRect.x()+spRect.width())
	//     rectList[initID]= QRectF(spRect.x()+spRect.width(), rectList[initID].y(), rectList[initID].width(), rectList[initID].height());

	//int pIDF=B->getFamilyID(pID, familySets);
	bool flag=false;

	//test location
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

	vector<int> parentList = _manager->getParentList(list, familySets, parent);		
	vector<QRectF> bList;
	
	flag=false;
	if( B->isbridgeBroken(rectList) )
	   flag=true;

	/*if(!flag) //further adjust
	{
		int mini=initID;
		float min=1000000;
		float dis,Dis;
		bool oflag=false;
		
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
	}*/
	

	//further adjust
    vector<QRectF> cList=rectList;
	float testHeight = cList[initID].height()*1.5;
	cList[initID] = QRectF( cList[initID].x(), cList[initID].center().y()-testHeight/2, cList[initID].width(), testHeight);
	int oid = B->findOverlaid(initID, cList);
	if(oid > 0) //Rect initID is very close to Rect oid
	{
		float cx=rectList[initID].center().x(), cy=rectList[oid].center().y(), w=rectList[initID].width(), h=rectList[oid].height();
		QPointF tempPos=B->getSmallestDistance(QRectF(cx-w/2,cy-h/2,w,h), rectList[initID]);  	
		//minPos= oldPos + tempPos;			
		rectList[initID]=QRectF(tempPos.x(), tempPos.y(), rectList[initID].width(), rectList[initID].height());	
	}
	vector<QRectF> aList;

	/*if(flag)
	{
		list[initID]->mySetPos( rectList[initID].center());	
	}	
	else*/
	{
		initSet.clear();
		familySets.clear();
		familySets=_manager->GroupToFamily(list);//, init);
		int initIDF=B->getFamilyID(initID, familySets);			
		initSet.insert(initIDF); 
		vector<int> parentList = _manager->getParentList(list, familySets,parent);	
		vector<QRectF> aList=rectList;
		rectList = 	B->lineUpDisplay(SUBTREERINGCREATED, list, parentList, initSet, list.size(), familySets, rectList, list[0]->screenWidth, list[0]->screenHeight, QRectF(-10000000,-10000000,-10000000,-10000000));//list should include initSet

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

				QRectF preRect=newItem->sceneBoundingRect();
				QRectF targetRect=preRect;
				QPointF newCenter=QPointF(x + newItem->sceneBoundingRect().width()/2, y + newItem->sceneBoundingRect().height()/2);
				targetRect.moveCenter(newCenter);
				QPointF stepSize=_manager->getSynchronizedSpeed(newItem, preRect, targetRect);	

				//if(setPos)
				//	list[i]->mySetPos( QPointF(x, y));		
				//else 
				if(list[i]->getType()!=CALLOUT)
				{
					//list[i]->mySetPos( QPointF(x, y) - tempPos);	

					list[i]->mySetPos( QPointF(x, y) - tempPos);	
					rectList[i].moveCenter(QPointF(x, y) - tempPos);
				}
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

vector<ItemBase *>  OpenGLScene::extractTypeBubblesinGroup(ItemBase * item, int Type)
{
	vector<ItemBase *> outputList;
	QList<ItemBase *> mlist = getGroupMembers(item);
	for(int i=0; i<mlist.size(); i++) 
	{
		if(mlist[i]->getType() != Type)
			continue;		
		outputList.push_back(mlist[i]);
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

void OpenGLScene::clearSelected(ItemBase *item)
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
				pbubble->sharedComplex.clear();         pbubble->sharedComplex.resize(csize);
				pbubble->sharedDna.clear();             pbubble->sharedDna.resize(csize);
				pbubble->sharedReaction.clear();        pbubble->sharedReaction.resize(csize);
				pbubble->sharedPhysicalEntity.clear();  pbubble->sharedPhysicalEntity.resize(csize);
				pbubble->sharedANode.clear();           pbubble->sharedANode.resize(csize);
				pbubble->sharedCompartment.clear();     pbubble->sharedCompartment.resize(csize);
				pbubble->sharedPathway.clear();         pbubble->sharedPathway.resize(csize);
				

				csize=pbubble->linkedProtein.size();	
				pbubble->linkedProtein.clear();         
				pbubble->linkedSmallMolecule.clear();
				pbubble->linkedComplex.clear(); 
				pbubble->linkedDna.clear();  
				pbubble->linkedReaction.clear(); 
				pbubble->linkedPhysicalEntity.clear();  
				pbubble->linkedANode.clear();  
				pbubble->linkedCompartment.clear();  
				pbubble->linkedPathway.clear();  
				pbubble->linkedEdge.clear();  

				pbubble->linkedProtein.resize(csize);
				pbubble->linkedSmallMolecule.resize(csize);
				pbubble->linkedComplex.resize(csize); 
				pbubble->linkedDna.resize(csize);  
				pbubble->linkedReaction.resize(csize); 
				pbubble->linkedPhysicalEntity.resize(csize);  
				pbubble->linkedANode.resize(csize);  
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
				pbubble->differedANode.clear();  
				pbubble->differedCompartment.clear();  
				pbubble->differedPathway.clear();  

				pbubble->differedProtein.resize(csize);
				pbubble->differedSmallMolecule.resize(csize);
				pbubble->differedComplex.resize(csize); 
				pbubble->differedDna.resize(csize);  
				pbubble->differedReaction.resize(csize); 
				pbubble->differedPhysicalEntity.resize(csize);  
				pbubble->differedANode.resize(csize);  
				pbubble->differedCompartment.resize(csize);  
				pbubble->differedPathway.resize(csize);  
				
				csize=pbubble->expressedProtein.size();	
				pbubble->expressedProtein.clear();
				pbubble->expressedSmallMolecule.clear();
				pbubble->expressedComplex.clear(); 
				pbubble->expressedDna.clear();  
				pbubble->expressedReaction.clear(); 
				pbubble->expressedPhysicalEntity.clear();  
				pbubble->expressedANode.clear();  
				pbubble->expressedCompartment.clear();  
				pbubble->expressedPathway.clear();  

				pbubble->expressedProtein.resize(csize);
				pbubble->expressedSmallMolecule.resize(csize);
				pbubble->expressedComplex.resize(csize); 
				pbubble->expressedDna.resize(csize);  
				pbubble->expressedReaction.resize(csize); 
				pbubble->expressedPhysicalEntity.resize(csize);  
				pbubble->expressedANode.resize(csize);  
				pbubble->expressedCompartment.resize(csize);  
				pbubble->expressedPathway.resize(csize);  
			}
		}	
}

void OpenGLScene::clearUndoRedoRecord(ItemBase *item)				
{
	QList<ItemBase *> mList=getGroupMembers(item);
	bool flag=false;
	for(int i=0; i<mList.size(); i++) 
	{
		if( mList[i]->getType()==PATHBUBBLE1 || mList[i]->getType()==SUBPATHBUBBLE1 )
		{					
			PathBubble1* pbubble = dynamic_cast<PathBubble1*> (mList[i]);			
			
			if(!flag)
			if(pbubble->headPathway!=NULL && pbubble->headPathway->m_pathwayID>=0)
			{
				pbubble->headPathway->linkedStoredinHead.clear();
			    pbubble->headPathway->currentLinkedinHeadit = pbubble->headPathway->linkedStoredinHead.end();
				flag=true;
			}					
			pbubble->highlightedStored.clear();
			pbubble->currentHighlightedit=pbubble->highlightedStored.end();
					 
			pbubble->linkedStored.clear();
			pbubble->currentLinkedit=pbubble->linkedStored.end();

			pbubble->sharedStored.clear();
			pbubble->currentSharedit=pbubble->sharedStored.end();

			pbubble->differedStored.clear();
			pbubble->currentDifferedit=pbubble->differedStored.end();					

			pbubble->pathwayGraphStored.clear();
			pbubble->currentPathwayGraphit = pbubble->pathwayGraphStored.end();		    

			//pbubble->graphStoredifferedStored.clear();
			//pbubble->currentDifferedit=pbubble->differedStored.end();	
		}	
		else if( mList[i]->getType()==EXPRESSIONBUBBLE )
		{
		     expressionBubble* ebubble = dynamic_cast<expressionBubble*> (mList[i]);			
		     //ebubble->clearExpressed();
	         //ebubble->clearSharedExpression();	
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
		pbubble->expressedProtein_Edit.clear();
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
				if(pbubble->clearExpressed())				
		             pbubble->getNodeToBePaint();
			}
			else if( mList[i]->getType()==EXPRESSIONBUBBLE )
			{
				expressionBubble * ebubble = dynamic_cast<expressionBubble *> (mList[i]);
				//ebubble->clearExpressed();	
				ebubble->clearSearched();
				ebubble->clearSharedExpression();	
				ebubble->clearHighlighted();
			}
		}		
}

void OpenGLScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{

	if (!m_rightButton) 
	{
		bubbleDraggedout=false;
		lastPressedPos = QPointF(0,-1000000);
	}
	/*if(_fileSelection!=NULL)
	{
		_fileSelection->setVisible(false);		
	    _fileSelection->setDiaVisible(false);
	}*/
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

	this->_manager->updateItemsPos(); //move item to its destination for setTargetPos()

	static bool itemsMoved = false; //keqin make it static
	bool flag0=itemsMoved;
	bool flag1=false;
	foreach (ItemBase *node, nodes) 
	{
		if (node->advance())
		{
			flag1=true;
			itemsMoved = true;
			//if(node->getTargetPos() == node->pos())
			this->_manager->itemPosUpdatedforSpring(node);
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
			if(base->getType()!=CALLOUT)
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

ItemBase* OpenGLScene::openWebNoteBubblebyMenu(int type, ItemBase *item, QPointF scenePos, int pid, int ptype, int id)
{
    ItemBase* nitem=NULL;
	QList<ItemBase *> mlist=allMembers();	 
				
	if(type==WEB)
		nitem=getOpenManager()->openWebView(item, 300,scenePos.x(),scenePos.y(), ""); 		 
	else if(type==SEARCHFIND)
		nitem=getOpenManager()->openSearchView(item, 200,scenePos.x(),scenePos.y(), ""); 		 
	else if(type==NOTE)
		nitem=getOpenManager()->openNote(item, 200,scenePos.x(),scenePos.y()); 	
	else if(type==EGIFT && ptype=='P') 
	{
		if(pid>=0)
		if(_pathBubbles[pid])
		if(_pathBubbles[pid]!=NULL)
		if(_pathBubbles[pid]->m_pathwayID>=0 && _pathBubbles[pid]->m_pathwayID < _pathBubbles.size())
		{	
			if(id<_proteinNameD[pid].size())
			{
				nitem=getOpenManager()->openWebView(item, 400, scenePos.x(), scenePos.y(), _proteinNameD[pid][id][4], pid, ptype, id); 	
				PathBubble1* pBubble = dynamic_cast<PathBubble1*>(item);	
				WebBlock* wBubble = dynamic_cast<WebBlock*>(nitem);
				//pBubble->_eGiftBubbles.push_back(wBubble);
				//_eGiftBubbles.push_back(wBubble);
			}
		}
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
		QRectF preRect=nitem->sceneBoundingRect();
		QRectF targetRect=preRect;
		QPointF newCenter=QPointF(x + nitem->sceneBoundingRect().width()/2, y + nitem->sceneBoundingRect().height()/2);
		targetRect.moveCenter(newCenter);
		QPointF stepSize=_manager->getSynchronizedSpeed(nitem, preRect, targetRect);						
		nitem->setTargetPos(newCenter,stepSize,true);    

		if(x== scenePos.x() && y== scenePos.y())
		    nitem->mySetPos(newCenter); 	
		else 
		    nitem->setTargetPos(newCenter, stepSize, true); 	
		//nitem->setTargetPos(QPointF(x + nitem->sceneBoundingRect().width()/2, y + nitem->sceneBoundingRect().height()/2)); 
	}
	else if(nitem!=NULL)
	{
		QPointF pos = getGroupedBubblePosition(nitem,item, item->sceneBoundingRect().x()+item->sceneBoundingRect().width(), nitem->sceneBoundingRect().y(), nitem->sceneBoundingRect().width(), nitem->sceneBoundingRect().height());			
		if(item->getType()!=CALLOUT)
			addToGroup( item, nitem, false);
	}
	return nitem;
}

void OpenGLScene::openPathBubblebyMenu(QString name, QString pname, QString lastStoredGraph, bool setPos, bool rightSide)
{
   openPathBubblebyMenu(name, pname, lastStoredGraph, menuSelectedItem, menuPos, setPos, rightSide);
}

void OpenGLScene::openPathBubblebyMenu(QString name, QString pname, QString lastStoredGraph, ItemBase *item, QPointF scenePos, bool setPos, bool rightSide)
{
		if( name.size() > 0 )
		{		
			ItemBase* nitem;
	        nitem = NULL;					
			
			//bool opened = getOpenManager()->openPathwayByPath(nitem, NULL, 400, scenePos.x()+220, scenePos.y()-136, name, pname, lastStoredGraph);			
			bool opened = getOpenManager()->openPathwayByPath(nitem, NULL, 400, scenePos.x()+220, scenePos.y()+136, name, pname, lastStoredGraph);			
			if(opened)			
			{				
				PathBubble1* pBubble = dynamic_cast<PathBubble1*>(nitem);	
				pBubble->mySetPos(QPoint(scenePos.x()+pBubble->sceneBoundingRect().width()/2, scenePos.y()+pBubble->sceneBoundingRect().height()/2)); 	
				if(item==NULL)
				{
					//QPointF pos = this->_manager->getPosition(pBubble, pBubble->sceneBoundingRect().x(), pBubble->sceneBoundingRect().y(), pBubble->sceneBoundingRect().width(), pBubble->sceneBoundingRect().height());						
					QPointF pos = this->_manager->getPosition(pBubble, scenePos.x(), scenePos.y(), pBubble->sceneBoundingRect().width(), pBubble->sceneBoundingRect().height());						
					float x= scenePos.x(), y=scenePos.y();
					if(pos.x()!=-1 && pos.y()!=-1)
					{
						x = pos.x();
						y = pos.y();	
					}	
					//y=850;
					QRectF preRect=pBubble->sceneBoundingRect();
					QRectF targetRect=preRect;
					QPointF newCenter=QPointF(x + pBubble->sceneBoundingRect().width()/2, y + pBubble->sceneBoundingRect().height()/2);
					targetRect.moveCenter(newCenter);
					QPointF stepSize=_manager->getSynchronizedSpeed(nitem, preRect, targetRect);	
						
					if(x== scenePos.x() && y== scenePos.y())
						pBubble->mySetPos(newCenter); 	
					else 
						pBubble->setTargetPos(newCenter, stepSize, true); 	
				}
				else if(item->getType()!=CALLOUT) 
				{
					//QPointF pos = getGroupedBubblePosition(pBubble,item, item->sceneBoundingRect().x() + pBubble->sceneBoundingRect().width(), pBubble->sceneBoundingRect().y(), pBubble->sceneBoundingRect().width(), pBubble->sceneBoundingRect().height(), setPos);					
					//QPointF pos = getGroupedBubblePosition(nitem,item, item->sceneBoundingRect().x()+nitem->sceneBoundingRect().width(), nitem->sceneBoundingRect().y(), nitem->sceneBoundingRect().width(), nitem->sceneBoundingRect().height());			
					if(rightSide)
			        {
						QPoint pos = getSonPosition(pBubble, (PathBubble1*)item, pBubble->sceneBoundingRect().x(), pBubble->sceneBoundingRect().y(), pBubble->sceneBoundingRect().width(), pBubble->sceneBoundingRect().height());								       
						//QPointF pos = this->_manager->getPosition(item, pBubble, pBubble->sceneBoundingRect().x(), pBubble->sceneBoundingRect().y(), pBubble->sceneBoundingRect().width(), pBubble->sceneBoundingRect().height());						
						float x= scenePos.x(), y=scenePos.y();
						if(pos.x()!=-1 && pos.y()!=-1)
						{
						   x = pos.x();
						   y = pos.y();	
						}
						QRectF preRect=pBubble->sceneBoundingRect();
						QRectF targetRect=preRect;
						QPointF newCenter=QPointF(x + pBubble->realRect().width()/2, y + pBubble->realRect().height()/2);
						targetRect.moveCenter(newCenter);
						QPointF stepSize=_manager->getSynchronizedSpeed(nitem, preRect, targetRect);	
						if(setPos)
						    pBubble->mySetPos(newCenter); 	
						else
							pBubble->setTargetPos(newCenter, stepSize, true); 	
					}
					else
					{
					    QPointF pos = getGroupedBubblePosition(pBubble,item, item->sceneBoundingRect().x() + pBubble->sceneBoundingRect().width(), pBubble->sceneBoundingRect().y(), pBubble->sceneBoundingRect().width(), pBubble->sceneBoundingRect().height(), setPos);					
					}
					if(item->getType()!=CALLOUT)
					   addToGroup( item, pBubble, false);
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

PathBubble1 * OpenGLScene::findASmallestPathinGroup(ItemBase *init)
{
	PathBubble1 *pItem=NULL;
	QList<ItemBase *> mList = getGroupMembers(init);		
	set<int> pid;
	for(int i=0; i<mList.size(); i++)
	{
		if(mList[i]->getType()==PATHBUBBLE1 || mList[i]->getType()== SUBPATHBUBBLE1)
		{
			pid.insert(mList[i]->m_pathwayID);			
		}			
	}
	if(pid.size()>0)
	{
	    pItem=dynamic_cast<PathBubble1 *> (_pathBubbles[*pid.begin()]);
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

ItemBase *OpenGLScene::findATypeBubbleinGroup(ItemBase *init, int Type)
{
	ItemBase *pItem=NULL;
	if(init->getType() == Type)
	{
	    return init;
	}
    QList<ItemBase *> mList = getGroupMembers(init);
		
	for(int i=0; i<mList.size(); i++)
	{
		if(mList[i]->getType()==Type)
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

ItemBase *OpenGLScene::findATypeBubbleinGroup(Group *B, int type)
{
	ItemBase *pItem=NULL;
	QList<ItemBase *> mList=B->getMembers();
	for(int i=0; i<mList.size(); i++)
	{
		if(mList[i]->getType()== type)
		{
			pItem=mList[i];
			break;
		}			
	}
    return pItem;
}

void OpenGLScene::removeToolsinGroup(ItemBase * item, bool setFalseflag)
{
	vector<PathBubble1 *> mList= extractPathBubblesinGroup(item);
	for(int m=0; m<mList.size(); m++)
	{
		PathBubble1* tmp = dynamic_cast<PathBubble1*>(mList[m]);	
		if(!setFalseflag)
		{
			tmp->_penisOn=item->_penisOn;
			tmp->groupSelected=item->groupSelected;
			tmp->_grabisOn=item->_grabisOn;
			tmp->_queryisOn=item->_queryisOn;
			tmp->_upisOn=item->_upisOn;		
			tmp->_downisOn=item->_downisOn;
			tmp->_evenisOn=item->_evenisOn;		
		}
		else
		{
			tmp->_penisOn=false;
			tmp->groupSelected=false;
			tmp->_grabisOn=false;
			tmp->_queryisOn=false;
			tmp->_upisOn=false;		
			tmp->_downisOn=false;
			tmp->_evenisOn=false;	
			tmp->clearUngroup();	
	        tmp->clearSketch();
		}		
	}
	vector<ItemBase *> tlist = extractTypeBubblesinGroup(item, TREERING);
	for(int i=0; i<tlist.size(); i++) 	
	{
		treeRingBubble* tmp = dynamic_cast<treeRingBubble *>(tlist[i]);							
		if(!setFalseflag)
		{
			tmp->_penisOn=item->_penisOn;
			tmp->groupSelected=item->groupSelected;
			tmp->_grabisOn=item->_grabisOn;
			tmp->_queryisOn=item->_queryisOn;
			tmp->_upisOn=item->_upisOn;		
			tmp->_downisOn=item->_downisOn;
			tmp->_evenisOn=item->_evenisOn;		
		}
		else
		{
			tmp->_penisOn=false;
			tmp->groupSelected=false;
			tmp->_grabisOn=false;
			tmp->_queryisOn=false;
			tmp->_upisOn=false;		
			tmp->_downisOn=false;
			tmp->_evenisOn=false;	
			tmp->clearUngroup();	
	        tmp->clearSketch();
		}							
	}			
}

void OpenGLScene::clearSearchSelection(ItemBase * item)
{
		clearSelected(item);
		clearExpressed(item);		
		clearColored(item);
		clearUndoRedoRecord(item);
		hoveredItem.clear();
		hoveredItem.clear();

		for(int i= 0; i< pathEnds.size(); i++)
		for(int j= 0; j< pathEnds[i].size(); j++)
		{
            pathEnds[i].clear();
		}		

		removeToolsinGroup(item, true);

		QList<ItemBase *> mList=getGroupMembers(item);
		for(int i=0; i<mList.size(); i++) 
		{
			if( mList[i]->getType()==PATHBUBBLE1 || mList[i]->getType()==SUBPATHBUBBLE1 )
			{
				PathBubble1* pbubble = dynamic_cast<PathBubble1*> (mList[i]);			
				pbubble->getGraphToBePaint();
			}
			if( mList[i]->getType()==EXPRESSIONBUBBLE )
			{
				expressionBubble* ebubble = dynamic_cast<expressionBubble*> (mList[i]);		
				ebubble->getExpressionToBePaint();
			}
			if( mList[i]->getType()==TREERING )
			{
				treeRingBubble* tbubble = dynamic_cast<treeRingBubble*> (mList[i]);		
				tbubble->clearSearchSelection();
			}
		}

		
}



CalloutNote* OpenGLScene::findNote(int pid, int type, int id)
{
	bool flag=false;
	for(int i=0; i<_nodeNote.size(); i++)
	{
		if(_nodeNote[i]!=NULL && !_nodeNote[i]->deleted)
		{
	        if(_nodeNote[i]->_pid==pid && _nodeNote[i]->_type==type && _nodeNote[i]->_id==id)
			{
				flag=true;
				return _nodeNote[i];
			}
		}
	}
	if(!flag)
		return NULL;
}

WebBlock* OpenGLScene::findEgiftBubble(int pid, int type, int id)
{
	bool flag=false;
	for(int i=0; i<_eGiftBubbles.size(); i++)
	{
		if(_eGiftBubbles[i]!=NULL && !_eGiftBubbles[i]->deleted)
		{
	        if(_eGiftBubbles[i]->_pid==pid && _eGiftBubbles[i]->_type==type && _eGiftBubbles[i]->_id==id)
			{
				flag=true;
				return _eGiftBubbles[i];
			}
		}
	}
	if(!flag)
		return NULL;
}

void OpenGLScene::openNodeNote(int pid, int type, int id, ItemBase * item, QPointF pos, vector<QString> text, vector<int> Cid, bool setVisible)
{
	CalloutNote* nodeNote=findNote(pid, type, id);	
	if(nodeNote==NULL)
	{		
		ItemBase *tnote = getOpenManager()->openNote_2(pid, type, id, item, 150, pos.x(), pos.y()); 
		nodeNote = dynamic_cast<CalloutNote*>(tnote);				
		if(text.size()>0)
		{
			nodeNote->nodeText.resize(1);
			QString Text;
			for(int i=0; i<text.size(); i++)
			{
				QString _fontColor=TDHelper::getHtmlColorByUserName(Cid[i]);	
				QString test=text[i];
				test =  "\">" + test;
				test =  _fontColor + test;				
				test = "<font color=\"" + test;
				test = test + "</font>";
			    Text = Text+ test;//text[i];
			}
			nodeNote->nodeText[0] = Text;							
			nodeNote->updateNodeFirstTime(pid, type, id, false);
		}
		else 
			nodeNote->updateNodeFirstTime(pid, type, id, true);
		_nodeNote.push_back(nodeNote);				
	}
	else
	{
		bool v= nodeNote->isVisible();
		nodeNote->setPos(pos.x(), pos.y());
		nodeNote->updateNode(pid, type, id, setVisible? true: nodeNote->m_note->isVisible());
	}

	float w=nodeNote->Width(), h=nodeNote->Height();
	float Px=nodeNote->pos().x()+w/2+30, Py=nodeNote->pos().y()-h/2-30;

	if(h/2 + Py > nodeNote->screenHeight-60)
	{
		Py= nodeNote->screenHeight-h/2-60;		
	}
	else if( Py - h/2 < 60 )
	{
		Py=h/2 + 60; 
	}
	if(Px-w/2 < 10)
	{
		Px=w/2+10; 
	}
	nodeNote->mySetPos(QPointF(Px,Py));		
}

void OpenGLScene::reopenMenu()
{
	menu.clear();	
	loadMenuOpened=false;
	loadMenu->clear();
	loadMenus(menuPos, menuScreenPos, menuSelectedItem, menuInputNote);

	menu.clear();	
	loadMenu->clear();
	loadMenus(menuPos, menuScreenPos, menuSelectedItem, menuInputNote);

	if(fileSelectionOpened)
		closeFileSelection();	

	needToReOpenMenu=false;
}

void OpenGLScene::closeFileSelection()
{
	fileSelectionOpened=false;
	//_fileSelection->setVisible(false);
	_fileSelection->setDiaVisible(false);
	_fileSelection->_myControl->setPos(-2000,-1000); 
	update();

	//menuloaded = false;
	//_fileSelection->setPos(-2000,-1000);
	/*if(menu.isVisible())
		menu.setVisible(false);
	if(loadMenu->isVisible())
		loadMenu->setVisible(false);
	*/
	//_fileSelection->_myControl->setVisible(false);
}


void OpenGLScene::openFileSelection()
{
	openCount++;
	if(fileSelectionOpened)
		fileSelectionOpened=fileSelectionOpened;

	fileSelectionOpened=true;
	//_fileSelection->setVisible(true);
	_fileSelection->setDiaVisible(true);
	//_fileSelection->setVisible(true);
	QPointF offset(138,9);
	_fileSelection->_myControl->setPos(menuPos+offset);		
	_fileSelection->listBoxClicked=false;	
	loadMenuOpened=true;
	//loadMenu->setVisible(false);
	//_fileSelection->setPos(_fileSelection->getDiaPositon()-QPoint(283,0));	
	update();

}

void OpenGLScene::dropDownMenu0(QGraphicsSceneContextMenuEvent * contextMenuEvent, ItemBase *item, vector<int> node)
{		
	menuPos=contextMenuEvent->scenePos();
    menuScreenPos = contextMenuEvent->screenPos();
	menuSelectedItem = item;
	menuInputNote = node;

	//_fileSelection->setVisible(true);
	//_fileSelection->setDiaVisible(true);
	
	//if(!menuloaded)
	 //loadMenu->clear();
	 menu.clear();
	 
	if(loadMenuOpened)
		 loadMenuOpened=loadMenuOpened;
	 
	menuloaded=true;
	loadMenuOpened=false;
	//QMenu menu;	 
	//loadMenus(contextMenuEvent, item, node);
	loadMenus(contextMenuEvent->scenePos(), contextMenuEvent->screenPos(), item, node);
		
}


ItemBase* OpenGLScene::openTreeRingBubblebyMenu(bool setPos, bool rightSide)
{
	ItemBase* tBubble=getOpenManager()->openTreeRingBubble(600, menuPos.x()+320, menuPos.y()+320);	
	if(tBubble)
	{
		if( menuSelectedItem ==NULL)
		{
			QPointF pos = this->_manager->getPosition(tBubble,  menuPos.x(), menuPos.y(), tBubble->sceneBoundingRect().width(), tBubble->sceneBoundingRect().height());						
			float x= menuPos.x(), y=menuPos.y();
			if(pos.x()!=-1 && pos.y()!=-1)
			{
				x = pos.x();
				y = pos.y();	
			}
			QRectF preRect=tBubble->sceneBoundingRect();
			QRectF targetRect=preRect;
			QPointF newCenter=QPointF(x + tBubble->sceneBoundingRect().width()/2, y + tBubble->sceneBoundingRect().height()/2);
			targetRect.moveCenter(newCenter);
			QPointF stepSize = _manager->getSynchronizedSpeed(tBubble, preRect, targetRect);	
			if(x== menuPos.x() && y== menuPos.y())
				tBubble->mySetPos(newCenter); 	
			else 
				tBubble->setTargetPos(newCenter,stepSize,true);
		}
		else if(menuSelectedItem->getType()!=CALLOUT) 
		{
			QPointF pos = getGroupedBubblePosition(tBubble,menuSelectedItem, menuSelectedItem->sceneBoundingRect().x() + tBubble->sceneBoundingRect().width(), tBubble->sceneBoundingRect().y(), tBubble->sceneBoundingRect().width(), tBubble->sceneBoundingRect().height(), setPos);					
			if(menuSelectedItem->getType()!=CALLOUT)
				addToGroup( menuSelectedItem, tBubble, false);
		}	
	}
	return tBubble;
}

ItemBase* OpenGLScene::openAutoTreeRingBubble(float x, float y, bool rightSide)
{
	ItemBase* tBubble=getOpenManager()->openTreeRingBubble(600, x+225, y+225);	

	//QPointF pos = this->_manager->getPosition(tBubble, tBubble->sceneBoundingRect().x(), tBubble->sceneBoundingRect().y(), tBubble->sceneBoundingRect().width(), tBubble->sceneBoundingRect().height());						
	//float x= menuPos.x(), y=menuPos.y();
	//x = x + tBubble->sceneBoundingRect().width()/2; y = y + tBubble->sceneBoundingRect().height()/2;
	/*if(pos.x()!=-1 && pos.y()!=-1)
	{
		x = pos.x();
		y = pos.y();	
	}
	QRectF preRect=tBubble->sceneBoundingRect();
	QRectF targetRect=preRect;
	QPointF newCenter=QPointF(x + tBubble->sceneBoundingRect().width()/2, y + tBubble->sceneBoundingRect().height()/2);
	targetRect.moveCenter(newCenter);
	QPointF stepSize=_manager->getSynchronizedSpeed(tBubble, preRect, targetRect);								
	tBubble->setTargetPos(newCenter,stepSize,true); */

	return tBubble;
}

void OpenGLScene::loadMenus(QPointF eventScenePos, QPoint eventScreenPos, ItemBase *item, vector<int> node)
{
    QMenu *pathwayMenu;	
	QMenu *crossTalkPathwayMenu;
	QMenu *searchMenu;		
	QMenu *editMenu;
	QMenu *deleteMenu;
	QMenu *saveMenu;

	QMenu *sceneMenu;	
	QMenu *viewMenu;	
	QMenu *shareMenu;
	QMenu *diffMenu;	
	QMenu *linkMenu;	

	QMenu *expressionMenu;	
	QMenu *colorMenu;	
	QMenu *searchSettingsMenu;
	
	QMenu *toolMenu;		
	QMenu *settingsMenu;	
	
	vector<int> etemp(3,0);
	vector< vector<int> > eventRecord;
	
	vector<int> rtemp(2,0),utemp(2,0);
	vector< vector<int> > undoRecord, redoRecord;

	//for cross-talk pathway
	vector<QAction *> crossTalkPathwayActions;
	vector<QAction *> orthologyActions;
		
	QAction *loadOrthologyAction, *G2Pathway,*BMPPathway,*TGFPathway,*shareAction,*diffAction,*linkAction, 
		*expressionExpressedAction,*clearSearchAction,*colorUpAction, *colorEvenAction,*colorDownAction,*toolUndoAction, *aggregationAction, *collapseAction, 
		*toolRedoAction,*graphResetAction,*graphReloadAction, *sketchAction, *queryAction, *delRingAction, *delAction, *delGroupAction, *unGroupAction, *removeAction, *saveGraph, *loadGraph,
		*saveScene, *recoverScene, *searchPathwayAction,*viewSmallMoleculeAction, *viewCrossTalkAction, *viewRateLimitAction, *selectedAction,*searchSettingsProteinAction,*searchSettingsCompartmentAction,*searchSettingsPathwayAction,
		*linkPath, *linkNode; //*searchSettingsAction *settingsAction

	bool loadOrthologyActionflag, loadExpressionActionflag,webActionflag,loadNoteActionflag,G2Pathwayflag,BMPPathwayflag,TGFPathwayflag,shareActionflag,diffActionflag,linkActionflag,pathwayActionflag,loadTreeRingActionflag,crossTalkPathwayMenuflag,
		expressionExpressedActionflag,clearSearchActionflag, colorUpActionflag, colorEvenActionflag,colorDownActionflag,toolUndoActionflag, aggregationActionflag, collapseActionflag, 
		toolRedoActionflag,graphResetActionflag,graphReloadActionflag, sketchActionflag,queryActionflag,delRingActionflag, delActionflag,delGroupActionflag, unGroupActionflag, removeActionflag, saveGraphflag, loadGraphflag,
		saveSceneflag, recoverSceneflag, searchPathwayActionflag,viewSmallMoleculeActionflag, viewRateLimitActionflag, viewCrossTalkActionflag, selectedActionflag,searchSettingsProteinActionflag,searchSettingsCompartmentActionflag,searchSettingsPathwayActionflag,
		linkPathflag, linkNodeflag; //searchSettingsActionflagif(settingsMenuflag)
		
	bool sceneMenuflag, viewMenuflag, shareMenuflag, diffMenuflag, linkMenuflag, expressionMenuflag, colorMenuflag, toolMenuflag, sketchMenuflag, queryMenuflag, deleteMenuflag, searchMenuflag, editMenuflag, loadMenuflag, pathwayMenuflag, searchSettingsMenuflag, settingsMenuflag;	

	loadOrthologyActionflag=loadExpressionActionflag=webActionflag=loadNoteActionflag=G2Pathwayflag=BMPPathwayflag=TGFPathwayflag=shareActionflag=diffActionflag=linkActionflag=pathwayActionflag=crossTalkPathwayMenuflag=
		expressionExpressedActionflag=clearSearchActionflag=colorUpActionflag= colorEvenActionflag=colorDownActionflag=toolUndoActionflag=saveGraphflag=loadGraphflag=aggregationActionflag=collapseActionflag=
		toolRedoActionflag=graphResetActionflag=graphReloadActionflag= sketchActionflag=queryActionflag= delRingActionflag = delActionflag= delGroupActionflag= unGroupActionflag= removeActionflag= 
		saveSceneflag= recoverSceneflag= searchPathwayActionflag=loadTreeRingActionflag=viewSmallMoleculeActionflag=viewSmallMoleculeActionflag=viewSmallMoleculeActionflag=selectedActionflag=searchSettingsProteinActionflag=searchSettingsCompartmentActionflag=
		searchSettingsPathwayActionflag=
		linkPathflag = linkNodeflag = true; //searchSettingsActionflag= //settingsActionflag

	sceneMenuflag= viewMenuflag= shareMenuflag= diffMenuflag= linkMenuflag= expressionMenuflag= colorMenuflag= toolMenuflag= sketchMenuflag= queryMenuflag= deleteMenuflag = searchMenuflag = editMenuflag = loadMenuflag = pathwayMenuflag = searchSettingsMenuflag = settingsMenuflag = true;
	
	QString ss("QMenu::item {background-color: black; color: white; border: 2px; height: 16px} QMenu::item:selected {border-color: darkblue; background: rgba(128, 128, 128, 150);} QMenu::separator {height: 2px; background: rgba(100, 100, 100, 150);  margin-left: 0px;  margin-right: 0px; } QMenu::item:disabled { color: gray;}");// QMenu::indicator:non-exclusive:checked { image: checkbox_checked.jpeg;}");
    //QString ss("QMenu::indicator:non-exclusive:checked { image: url(:);}");
	//QString ss("QMenu{background-color: black}");

	//loadTreeRingActionflag = false;

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
	treeRingBubble *tBubble=NULL;
	//ItemBase *OpenGLScene::findATypeBubbleinGroup(Group *B, int type)
	ItemBase *pItem=NULL, *eItem=NULL, *tItem=NULL;
	if(item!=NULL)
	{   
		//a bubble is clicked
		//if(item->getType()==PATHBUBBLE1 || item->getType()==SUBPATHBUBBLE1)
	    //    pItem=item;
		//else 
		pItem = findAPathBubbleinGroup(item);

		//if(item->getType()==EXPRESSIONBUBBLE)
	    //    eItem=item;
		//else 
		eItem = findAExpressionBubbleinGroup(item);

		//if(item->getType()==TREERING)
	    //    tItem=item;
		//else 
		tItem = findATypeBubbleinGroup(item, TREERING);

		if(pItem!=NULL)
		{
			pBubble = dynamic_cast<PathBubble1 *> (pItem);
			toplevel = pBubble->toplevel; //contextMenuEvent
		}	
		if(eItem!=NULL)
		{
			eBubble = dynamic_cast<expressionBubble *> (eItem);
		}
		if(tItem!=NULL)
		{
			tBubble = dynamic_cast<treeRingBubble *> (tItem);
		}
	}

	//treeRingBubble *_tb;
	
	//saveSceneflag=false;			
	recoverSceneflag=true;
	if(item==NULL)
	{
	   //searchPathwayActionflag=false;
	   colorMenuflag = false;
	   sketchActionflag=false;
	   queryActionflag=false;	
	   loadNoteActionflag=false;
	   //deleteMenuflag=false;
	   delGroupActionflag=false;
	   unGroupActionflag=false;
	   delRingActionflag=false;
	   delActionflag=false;
	   saveGraphflag=false;
	   loadGraphflag=false;
	   if(allMembers().size()==0)
	   {
		   searchMenuflag=false;
		   //editMenuflag=false;
		      aggregationActionflag=collapseActionflag= graphResetActionflag = false;
		   saveSceneflag=false;
		   removeActionflag=false;
	   }
	   loadOrthologyActionflag=false;
	}	
	else
	{	  
		
		/*sceneMenuflag=false;
		removeActionflag=false;*/
		if(getGroupMembers(item).size()<=1)
		{
			delGroupActionflag=false;
			unGroupActionflag = false;
		}
			
		saveSceneflag=true;
		removeActionflag=true;

		if(pBubble==NULL && tBubble==NULL && eBubble==NULL) 
			clearSearchActionflag=false;

        if(pBubble!=NULL)
		{
			if(pBubble->toplevel)
			{
				saveGraphflag=true;
				loadGraphflag=true;
			}
			else
			{
			    saveGraphflag=false;
				loadGraphflag=false;
			}
			
		}
		else 
		{
			saveGraphflag=false;
	        loadGraphflag=false;

			searchMenuflag=false;
				shareActionflag=false;
				diffActionflag=false;
				linkActionflag=false;	
				linkMenuflag=false;
				    linkPathflag=false;
					linkNodeflag=false;
				expressionExpressedActionflag=false;	
				
				searchMenuflag=false;		
				searchSettingsMenuflag=false;			

		    //editMenuflag=false;
				aggregationActionflag = collapseActionflag= graphResetActionflag = false;
			    colorMenuflag=false;				
					colorUpActionflag=false;
					colorEvenActionflag=false;					
					colorDownActionflag=false;					
			    
				toolUndoActionflag=false;				
			    toolRedoActionflag=false;
				toolRedoAction=false;
			    
				
			sketchActionflag=false;
			colorMenuflag = false;
		    //queryActionflag=false;
			settingsMenuflag=false;
		    if(eBubble==NULL)				
			{	
				searchMenuflag=true;				
			}

			if(eBubble==NULL && tBubble==NULL)				
			{
				searchPathwayActionflag=false;
			}
			
			if(tBubble==NULL)		
				loadOrthologyActionflag=false;
		}
		
		if(eBubble==NULL)
			expressionExpressedActionflag=false;
		if( eBubble==NULL && pBubble==NULL && tBubble==NULL)
		{
			searchMenuflag=false;
			searchPathwayActionflag=false;
		}		
		if(tBubble!=NULL)
		{
			sketchActionflag=true;		  
			//colorMenuflag = true;
			searchMenuflag=true;
		    if(eBubble!=NULL)	
			{
				searchMenuflag=true;
				expressionExpressedActionflag=true;					
			}
			delRingActionflag=true;
		}
		if(!node.empty() && node[1]=='P')
		    queryActionflag=true;
		else
			queryActionflag=false;

		if(!node.empty())
		    loadNoteActionflag = true;
		else
			loadNoteActionflag = false;
			
		
		//sceneMenuflag=false;				 		
	}
	recoverSceneflag = saveSceneflag= false; //keqin
	/*pathwayMenu = loadMenu->addMenu("Pathway\t");   	
	pathwayMenu->setMinimumWidth(60);			
	BMPPathway = pathwayMenu->addAction("BMP");			
	G2Pathway = pathwayMenu->addAction("MitoticG2-G2");
	TGFPathway = pathwayMenu->addAction("68874_");//"TGF"
	*/
	//delAction->setC
	loadMenu = menu.addMenu("Open");	
	loadMenu->setMinimumWidth(90);
	loadTreeRingAction=loadMenu->addAction("Species Reactome Pathway");   
	pathwayAction=loadMenu->addAction("Pathway");   	
	crossTalkPathwayMenu = loadMenu->addMenu("Cross-talking Pathway");	
	loadExpressionAction = loadMenu->addAction("Expression");	
	loadOrthologyAction = loadMenu->addAction("Ortholog");	
	loadMenu->addSeparator();
	webAction = loadMenu->addAction("Web Browser");	
	loadNoteAction = loadMenu->addAction("Note");
	loadMenu->addSeparator();
	loadGraph = loadMenu->addAction("Graph");	
	//recoverScene = loadMenu->addAction("Scene");	
	
	searchMenu = menu.addMenu("Selection");		
		searchMenu->setMinimumWidth(60);
		shareAction = searchMenu->addAction("Share");
		diffAction = searchMenu->addAction("Difference");
		//linkAction = searchMenu->addAction("Link");
		linkMenu = searchMenu->addMenu("Link");
		    linkPath = linkMenu->addAction("All Pathes");
		    linkNode = linkMenu->addAction("Between Nodes");

		expressionExpressedAction =searchMenu->addAction("Expression");	
		clearSearchAction=searchMenu->addAction("Clear");	
	searchMenu->addSeparator();	    
		searchSettingsMenu=searchMenu->addMenu("Settings");
			searchSettingsMenu->setMinimumWidth(60);
		searchSettingsProteinAction = searchSettingsMenu->addAction("     Protein");	
			searchSettingsProteinAction->setCheckable(true);				
			
			
			searchSettingsCompartmentAction = searchSettingsMenu->addAction("     Compartment");				
				searchSettingsCompartmentAction->setCheckable(true);			
				
			
			searchSettingsPathwayAction = searchSettingsMenu->addAction("     Pathway");				
				searchSettingsPathwayAction->setCheckable(true);
				
     if(searchType=='P')     
		 searchSettingsProteinAction->setChecked(true);
	 else if(searchType=='M')     
		 searchSettingsCompartmentAction->setChecked(true);
	 else if(searchType=='W')     
		 searchSettingsPathwayAction->setChecked(true);


	editMenu = menu.addMenu("View");
	    editMenu->setMinimumWidth(123);	
	   
		aggregationAction = editMenu->addAction("Aggregate"); 
		collapseAction = editMenu->addAction("Expand"); 
		
        editMenu->addSeparator();
		//toolUndoAction = editMenu->addAction("Undo");
		//toolRedoAction = editMenu->addAction("Redo");	      
		
		graphResetAction = editMenu->addAction("Reset Graph"); 	

		//settingsMenu = toolMenu->addMenu("View");
		if(drawSmallmoleculeSettings)
		{
			viewSmallMoleculeAction =  editMenu->addAction("     Show small Molecule ");			
			viewSmallMoleculeAction->setCheckable(true);	
			viewSmallMoleculeAction->setChecked(true);
		}
		else
		{
			viewSmallMoleculeAction =  editMenu->addAction("Show small Molecule ");			
			viewSmallMoleculeAction->setCheckable(true);	
			viewSmallMoleculeAction->setChecked(false);
		}
		

		
		//editMenu->addSeparator();				
		//editMenu->addSeparator();		
		//unGroupAction = editMenu->addAction("Un-Group");
	   
		   // graphReloadAction = editMenu->addAction("Graph Reload"); 	
	    toolMenu = menu.addMenu("Tools");
		toolMenu->setMinimumWidth(123);
		sketchAction = toolMenu->addAction("Pen Selection"); 
		queryAction = toolMenu->addAction("eGIFT Query"); 
		searchPathwayAction = toolMenu->addAction("Find");
		
		colorMenu = toolMenu->addMenu("Regulation by Color");	
		colorMenu->setMinimumWidth(60);
		colorUpAction = colorMenu->addAction("Up");   
		colorEvenAction = colorMenu->addAction("Even");   
		colorDownAction = colorMenu->addAction("Down");   

		toolMenu->addSeparator();
		deleteMenu =  toolMenu->addMenu("Delete");
		deleteMenu->setMinimumWidth(60);		
		delAction = deleteMenu->addAction("Bubble");	
		delGroupAction = deleteMenu->addAction("Group");
		removeAction = deleteMenu->addAction("Scene");
		deleteMenu->addSeparator();
		delRingAction = deleteMenu->addAction("Ring Segment");	
		
			
		//sceneMenu = toolMenu->addMenu("Display");
		//sceneMenu->setMinimumWidth(60);

		//delAction = sceneMenu->addAction("delete bubble");	
		//delGroupAction = sceneMenu->addAction("delete group");	
		//sceneMenu->addSeparator();
		//saveGraph = sceneMenu->addAction("save graph");	
		
		//sceneMenu->addSeparator();
		//saveScene = sceneMenu->addAction("Save scene");
		//removeAction = sceneMenu->addAction("Clear scene");	

			
	/*viewRateLimitAction = settingsMenu->addAction("     Rate-limited Protein");
		viewRateLimitAction->setCheckable(true);	
	if(drawRateLimit)
		viewRateLimitAction->setChecked(true);

	viewCrossTalkAction = settingsMenu->addAction("     Cross-talking Protein");
		viewCrossTalkAction->setCheckable(true);	
	if(drawCrossTalk)
		viewCrossTalkAction->setChecked(true);*/

	saveMenu = menu.addMenu("Save");
	saveMenu->setMinimumWidth(60);
	saveGraph = saveMenu->addAction("graph");			
	//sceneMenu->addSeparator();
	//saveScene = saveMenu->addAction("Scene");

	if(!node.empty() && node[1]=='P' && item->crossTalk.find(node[2])!=item->crossTalk.end())
	{
		crossTalkPathwayMenuflag=true;	
		set<QString> nameSet = pwdParser->crossTalkSymbolToPathway(_proteinNameD[pBubble->dataID][node[2]][4]);
		if(!nameSet.empty())
		{
			nameSet.erase(pBubble->_pathName);		
			set<QString>::iterator it=nameSet.begin();
			if(nameSet.size()>0)
			{
				crossTalkPathwayActions.resize(nameSet.size()+1);				
				for(int i=0; i<crossTalkPathwayActions.size()-1; i++)
				{
					crossTalkPathwayActions[i] = crossTalkPathwayMenu->addAction(*it);
					it++;
				}		
				crossTalkPathwayActions[nameSet.size()] = crossTalkPathwayMenu->addAction("Open All");
			}
			else 
			{
				crossTalkPathwayActions.resize(1);
				crossTalkPathwayActions[0] = crossTalkPathwayMenu->addAction(*nameSet.begin());			
			}
		}
	}
	else 
	{
		crossTalkPathwayMenuflag=false;
	}

	if(!crossTalkPathwayMenuflag)
		crossTalkPathwayMenu->setEnabled(false);
		
	if(!searchMenuflag)						
		searchMenu->setEnabled(false);

	if(!expressionExpressedActionflag)
		expressionExpressedAction->setEnabled(false);	

	if(!shareActionflag)
		shareAction->setEnabled(false);	

	if(!linkMenuflag) 
		linkMenu->setEnabled(false);

	if(!linkPathflag) 
		linkPath->setEnabled(false);
	
	if(!linkNodeflag) 
		linkNode->setEnabled(false);
	
	if(!loadOrthologyActionflag)
		loadOrthologyAction->setEnabled(false);
	//if(!linkActionflag)
	//	linkAction->setEnabled(false);	

	if(!diffActionflag)
		diffAction->setEnabled(false);	

	if(!clearSearchActionflag)
		clearSearchAction->setEnabled(false);	
	
	if(!editMenuflag)	
		editMenu->setEnabled(false);
	
	if(!colorMenuflag)
		colorMenu->setEnabled(false);
	
	//if(!toolUndoActionflag)
	//	toolUndoAction->setEnabled(false);

	//if(!toolRedoActionflag)
	//	toolRedoAction->setEnabled(false);

	if(!aggregationActionflag)
		aggregationAction->setEnabled(false);

	if(!collapseActionflag)
		collapseAction->setEnabled(false);

	if(!graphResetActionflag)
		graphResetAction->setEnabled(false);

	//if(!settingsMenuflag)
	//	settingsMenu->setEnabled(false);
	
	if(!sketchActionflag)
		sketchAction->setEnabled(false);
	if(!queryActionflag)
		queryAction->setEnabled(false);	
	//if(!deleteMenuflag)
	//	deleteMenu->setEnabled(false);
	if(!delActionflag)
		delAction->setEnabled(false);
	if(!delGroupActionflag)
	{
		delGroupAction->setEnabled(false);		
	}
	/*if(!unGroupActionflag)
	{
		unGroupAction->setEnabled(false);		
	}*/

	if(!saveGraphflag)
		saveGraph->setEnabled(false);
	if(!loadGraphflag)
		loadGraph->setEnabled(false);
		
	if(!sceneMenuflag)
		sceneMenu->setEnabled(false);	
		if(!removeActionflag)
			removeAction->setEnabled(false);	
		//if(!saveSceneflag)
		//	saveScene->setEnabled(false);	
		//if(!recoverSceneflag)
		//	recoverScene->setEnabled(false);
	
	if(!searchPathwayActionflag)
		searchPathwayAction->setEnabled(false);

	if(!loadTreeRingActionflag)
		loadTreeRingAction->setEnabled(false);
	
	//if(!settingsMenuflag)
	//	settingsMenu->setEnabled(false);

	//connect(pathwayAction, SIGNAL(hovered()), this, SLOT(openFileSelection()) );
	/*connect(loadExpressionAction, SIGNAL(hovered()), this, SLOT(closeFileSelection()) );	
	connect(editMenu, SIGNAL(hovered()), this, SLOT(closeFileSelection()) );
	connect(toolMenu, SIGNAL(hovered()), this, SLOT(closeFileSelection()) );
	connect(searchMenu, SIGNAL(hovered()), this, SLOT(closeFileSelection()) );
	connect(webAction, SIGNAL(hovered()), this, SLOT(closeFileSelection()) );
	connect(loadNoteAction, SIGNAL(hovered()), this, SLOT(closeFileSelection()) );	*/
	
    //selectedAction = menu.exec(QCursor::pos());
	selectedAction = menu.exec(eventScreenPos);	//do not need to use QCursor::pos(), if use it, the menu will appear at the QCursor::pos() instead of the input location
	
	//actions seleted
	if(selectedAction!=NULL)
	for(int i=0; i<crossTalkPathwayActions.size(); i++)
	{
		QString textname = selectedAction->text();
		QString textname1 = crossTalkPathwayActions[crossTalkPathwayActions.size()-1]->text();
		int size=crossTalkPathwayActions.size();
		if(selectedAction ==crossTalkPathwayActions[i])
		{
			
			if(crossTalkPathwayActions[i]->text()=="Open All")
			{
		
				for(int j=0; j<crossTalkPathwayActions.size()-1; j++)
				{
				   QString fileName = "data/Reactome_Pathway_Data/pathwayTable/pathFile/"+crossTalkPathwayActions[j]->text() + ".path";
				   QString num;
				   QFile inputFile(fileName);
				   if(inputFile.open(QIODevice::ReadOnly))
				   {
					   QTextStream in(&inputFile);
					   QString line = in.readLine();  
					   line = line.split('\n')[0];
					   num=line;			  
				   }
				   else					   
			       {			
					    inputFile.close();
					   fileName = crossTalkPathwayActions[j]->text();
						fileName = fileName.remove(':');			
						fileName = fileName.remove(',');		
						fileName = fileName.remove('/');
						fileName = fileName.remove('\'');
						fileName = "data/Reactome_Pathway_Data/pathwayTable/pathFile/" + fileName + ".path";

					    QFile inputFile1(fileName);
						
						if(inputFile1.open(QIODevice::ReadOnly))
					    {
						   QTextStream in(&inputFile1);
						   QString line = in.readLine();  
						   line = line.split('\n')[0];
						   num=line;			  
					    }	
						 inputFile1.close();
			       }
				  

				   int st = fileName.lastIndexOf("/");
				   fileName = fileName.remove(0, st+1);
				   fileName.chop(5);
		
				   if(num.size()==0)
					  continue;
				   
					QString fname = "data/Reactome_Pathway_Data/pathwayTable/tableFile" + num + "_";
					QString pname = fileName;	
					
					/*bool opened=false;
					for (int k = 0; k < _pathBubbles.size(); k ++)		
					{
						int size=_pathBubbles.size();
						PathBubble1 * tmp =_pathBubbles[k];

						if(_pathBubbles[k])
						if(_pathBubbles[k]!=NULL)
						if(_pathBubbles[k]->m_pathwayID>=0 && _pathBubbles[k]->m_pathwayID < _pathBubbles.size())
						if(_pathBubbles[k]->getPathwayName().compare(pname)==0 )
						{
							opened=true; 
							break;
						}
					}
					if(!opened)*/
					   openPathBubblebyMenu(fname, pname, "", true, true);			
				}
			}
			else 
			{
			   QString fileName = "data/Reactome_Pathway_Data/pathwayTable/pathFile/" + crossTalkPathwayActions[i]->text() + ".path";
			   QString num;
			   QFile inputFile(fileName);
			   if(inputFile.open(QIODevice::ReadOnly))
			   {
				   QTextStream in(&inputFile);
				   QString line = in.readLine();  
				   line = line.split('\n')[0];
				   num=line;			  
			   }
			   else
			   {
				    inputFile.close();

				    fileName = crossTalkPathwayActions[i]->text();
				    fileName = fileName.remove(':');			
			        fileName = fileName.remove(',');		
			        fileName = fileName.remove('/');
					fileName = fileName.remove('\'');
					fileName = "data/Reactome_Pathway_Data/pathwayTable/pathFile/" + fileName + ".path";
			        QFile inputFile1(fileName);
		           
					if(inputFile1.open(QIODevice::ReadOnly))
				   {
					   QTextStream in(&inputFile1);
					   QString line = in.readLine();  
					   line = line.split('\n')[0];
					   num=line;			  
				   }
				   inputFile1.close();
			   }
			   

			   if(num=="")
				   return;

			   int st = fileName.lastIndexOf("/");
			   fileName = fileName.remove(0, st+1);
			   fileName.chop(5);
		
			   
			   
			   // if(num == "157579")
				//	   continue;
			 
			   //if(num == "1643685") //disease
				//	 continue;
				//if(num == "1430728") //Metabolism
				//	 continue;
				//if(num == "382551") //transmembrane transport of small molecules
				//	 continue;
				//if(num == "168256")//immune system
				//	 continue;
				//if(num == "162582") //signal tranduction
				//	continue;
				//if(num == "163318") // an empty file
				//	 continue;

				QString fname = "data/Reactome_Pathway_Data/pathwayTable/tableFile/" + num + "_";
				QString pname = fileName;		
				openPathBubblebyMenu(fname, pname, "", false, true);			
			}	
		}
	}
	
	if(selectedAction == loadTreeRingAction)
	{
	    openTreeRingBubblebyMenu();	
	}
	if ( selectedAction == loadNoteAction )
	{
		if(node.empty())
	    {
			openWebNoteBubblebyMenu(NOTE, item, item==NULL?eventScenePos:item->sceneBoundingRect().center()+ QPointF(item->realRect().width(),0));//contextMenuEvent->scenePos());	
		}
		else
		{	
			vector<QString> text;
			vector<int> Cid;
			openNodeNote(node[0], node[1], node[2], item, eventScenePos, text, Cid, true);		   
		}
	}
	else if(selectedAction == pathwayAction)
	{
		//QString fileName = QFileDialog::getOpenFileName(0, tr("Load Pathway data"), "data/Reactome_Pathway_Data/pathwayTable/",".path(*.path);; ;; .txt(*.txt);; All files (*.*)");//"c:\\file.x", "X files (*.x)"
		if( item!=NULL && item->getType() == TREERING && !node.empty() )
		{
			int layer=node[0], id=node[1];
			treeRingBubble *titem = dynamic_cast<treeRingBubble *>(item);
			string name=titem->_treeRing->GetNodeName(layer,id);
			QString pathName=QString::fromStdString(name);
			pathName = pathName.replace("_"," ");
			
			QString tname;
			const char * ename;
			string sname;
			QString lastStoredfileName;
			struct expressedPathwayInfo infor;

			QString fileName = "data/Reactome_Pathway_Data/pathwayTable/pathFile/"+ pathName + ".path";
			QString num;
			QFile inputFile(fileName);
		    if(inputFile.open(QIODevice::ReadOnly))
			{
				QTextStream in(&inputFile);
				QString line = in.readLine();  
				line = line.split('\n')[0];
				num=line;			  
			}	
			/*if(num == "1643685") //keqin temorary comment out
			   return;
*/
			//if(num == "1643685") //disease
			//		 return;
				//if(num == "1430728") //Metabolism
			//		 return;
				//if(num == "382551") //transmembrane transport of small molecules
				//	 return;
				//if(num == "168256")//immune system
				//	 return;
				//if(num == "162582") //signal tranduction
				//	return;
				//if(num == "163318") // an empty file
				//	 return;
            
			// if(num == "157579")
			//		return;

			//if(num == "163318") //keqin temorary comment out
			//   return;

			QString fname = "data/Reactome_Pathway_Data/pathwayTable/tableFile/" + num + "_";			
			openPathBubblebyMenu(fname, pathName, lastStoredfileName);
			
		}
		else
		{
			QFileDialog dlg;
			dlg.setWindowTitle("Load Pathway data");
			dlg.setAcceptMode(QFileDialog::AcceptOpen);
 			
			float px=eventScreenPos.x(), py=eventScreenPos.y();
			
			if(px>width()-620)
				px=width()-620;

			if(py>height()-413)
				py=height()-413;

			dlg.move(px,py);

			dlg.setFileMode(QFileDialog::AnyFile);
			dlg.setViewMode(QFileDialog::List);
			dlg.setDirectory("data/Reactome_Pathway_Data/pathwayTable/pathFile/");
			dlg.setFilter(".path(*.path);; .xml(*.xml);; .txt(*.txt);; All files (*.*)");
			dlg.resize(620,383);
			QStringList fileNames;
			QString fileName;

			if(dlg.exec())
				 fileNames = dlg.selectedFiles();
			if(fileNames.size()>0)
				fileName= fileNames[0];
	 	
			QString lastStoredfileName;

			QString num;
			QString Surfix = fileName.mid(fileName.size()-3, 3);
			if (Surfix=="ath" )
			{
			   QFile inputFile(fileName);
			   if(inputFile.open(QIODevice::ReadOnly))
			   {
				   QTextStream in(&inputFile);
				   QString line = in.readLine();  
				   line = line.split('\n')[0];
				   num=line;			  
			   }
			   int st = fileName.lastIndexOf("/");
			   fileName = fileName.remove(0, st+1);
			   fileName.chop(5);
			}
			else if (Surfix=="txt")
			{
				int st = fileName.lastIndexOf("/");
				fileName = fileName.remove(0, st+1);
				st = fileName.lastIndexOf("_");
				num = fileName.remove(st,fileName.size() - st);
				int size=this->_fileSelection->pathIDs.size();
				for(int j=0; j<this->_fileSelection->pathIDs.size(); j++)
				{
					QString ID=QString::number(this->_fileSelection->pathIDs[j]);
					if(ID==num)
					{
						fileName=this->_fileSelection->pathNames[j];
					}
				}
			}
			else if (Surfix=="xml" )
			{
				lastStoredfileName=fileName;
				int st = fileName.lastIndexOf("/");
				fileName = fileName.remove(0, st+1);
				st = fileName.lastIndexOf("_");		   
				fileName = fileName.remove(st,fileName.size());
				int size=this->_fileSelection->pathNames.size();
				for(int j=0; j<this->_fileSelection->pathNames.size(); j++)
				{
					QString name = this->_fileSelection->pathNames[j];
					QString pathName1= name;
					pathName1 = pathName1.remove(':');			
			        pathName1 = pathName1.remove(',');		
			        pathName1 = pathName1.remove('/');
					pathName1 = pathName1.remove('\'');
					if( fileName==pathName1 )
					{
						num = QString::number(this->_fileSelection->pathIDs[j]);
					}
				}			
			}
		
			if(num.size()==0)
			{
				lastStoredfileName="";
				return;
			}

			// if(num == "157579")
			//	 return;

			/*if(num == "1643685") //keqin temorary comment out
			   return;
			*/
			//if(num == "1643685") //disease
			//		 return;
			//	if(num == "1430728") //Metabolism
				//	 return;
				//if(num == "382551") //transmembrane transport of small molecules
				//	 return;
				//if(num == "168256")//immune system
				//	 return;
				//if(num == "162582") //signal tranduction
				//	return;
				//if(num == "163318") // an empty file
				//	 return;
			
			/*if(num == "163318") //keqin temorary comment out
			   return;
			 */
			//num = "1430728";
			QString fname = "data/Reactome_Pathway_Data/pathwayTable/tableFile/" + num + "_";
			QString pname = fileName;		
			openPathBubblebyMenu(fname, pname, lastStoredfileName);
		}
	}
	/*else if(selectedAction == crossTalkPathwayMenu)
	{
		
	}*/
	else if(selectedAction == webAction )
	{
		openWebNoteBubblebyMenu(WEB, item, item==NULL?eventScenePos:item->sceneBoundingRect().center()+ QPointF(item->realRect().width(),0));//contextMenuEvent->scenePos());	
	}		
	else if ( selectedAction == loadExpressionAction )
	{
		//QString name = QFileDialog::getOpenFileName(0, tr("Load Expression data"), "data/Reactome_Pathway_Data/expressionData/",".txt(*.txt);; All files (*.*)");//"c:\\file.x", "X files (*.x)"				
		QFileDialog dlg;
		float px=eventScreenPos.x(), py=eventScreenPos.y();
			
		if(px>width()-620)
			px=width()-620;

		if(py>height()-413)
			py=height()-413;

		dlg.move(px,py);
		dlg.setWindowTitle("Load Expression data");
		dlg.setAcceptMode(QFileDialog::AcceptOpen);
 
		dlg.setFileMode(QFileDialog::AnyFile);
		dlg.setViewMode(QFileDialog::List);
		dlg.setDirectory("data/Reactome_Pathway_Data/expressionData/");
		dlg.setFilter(".txt(*.txt);; All files (*.*)");
		dlg.resize(620,383);
		QStringList fileNames;
		QString name;

		if(dlg.exec())
			 fileNames = dlg.selectedFiles();
		if(fileNames.size()>0)
			name= fileNames[0];
	 
		QString Surfix = name.mid(name.size()-15, 15);
	    if (Surfix!="0expression.txt" )
		{
		 	return;
		}	
		if( name.size() > 0 )
		{
			ItemBase* nitem;
			nitem = NULL;	
			bool opened;
			QPointF openPos=item==NULL?eventScenePos:item->sceneBoundingRect().center()+ QPointF(item->realRect().width(),0);//contextMenuEvent->scenePos());	
			name = name.remove("0expression.txt");  	
			opened = getOpenManager()->openExpressionByPath(nitem, 250, openPos.x(), openPos.y(), name);
			if(opened)
			{
				expressionBubble* ExpressionBubble = dynamic_cast<expressionBubble*>(nitem);
				QString tname = name + "0expression.txt";  		
				pwdParser->readexpression(tname.toUtf8().data(), ExpressionBubble->quantityName, ExpressionBubble->geneInfo, ExpressionBubble->quantity, ExpressionBubble->minmax);
				if(item==NULL)
				{
					QPointF pos = this->_manager->getPosition(ExpressionBubble,ExpressionBubble->sceneBoundingRect().x(), ExpressionBubble->sceneBoundingRect().y(), ExpressionBubble->sceneBoundingRect().width(), ExpressionBubble->sceneBoundingRect().height());
					float x= eventScenePos.x(), y=eventScenePos.y();
					if(pos.x()!=-1 && pos.y()!=-1)
					{
					   x = pos.x();
					   y = pos.y();	
					}
					QRectF preRect=ExpressionBubble->sceneBoundingRect();
					QRectF targetRect=preRect;
					QPointF newCenter=QPointF(x + ExpressionBubble->sceneBoundingRect().width()/2, y + ExpressionBubble->sceneBoundingRect().height()/2);
					targetRect.moveCenter(newCenter);
					QPointF stepSize=_manager->getSynchronizedSpeed(nitem, preRect, targetRect);						
					ExpressionBubble->setTargetPos(newCenter,stepSize,true);                    
					
				}
				else 
				{
					QPointF pos = getGroupedBubblePosition(ExpressionBubble,item, ExpressionBubble->sceneBoundingRect().x(), ExpressionBubble->sceneBoundingRect().y(), ExpressionBubble->sceneBoundingRect().width(), ExpressionBubble->sceneBoundingRect().height());				
					if(item->getType()!=CALLOUT)
						addToGroup( item, ExpressionBubble, false);
				}	
				ExpressionBubble->getExpressionToBePaint();
			}					
		}		
    }
	else if ( selectedAction == loadOrthologyAction )
	{
		//QString name = QFileDialog::getOpenFileName(0, tr("Load Expression data"), "data/Reactome_Pathway_Data/expressionData/",".txt(*.txt);; All files (*.*)");//"c:\\file.x", "X files (*.x)"				
		QFileDialog dlg;
		float px=eventScreenPos.x(), py=eventScreenPos.y();
			
		if(px>width()-620)
			px=width()-620;

		if(py>height()-413)
			py=height()-413;

		dlg.move(px,py);

		dlg.setWindowTitle("Load Orthology data");
		dlg.setAcceptMode(QFileDialog::AcceptOpen);
 
		dlg.setFileMode(QFileDialog::AnyFile);
		dlg.setViewMode(QFileDialog::List);
		dlg.setDirectory("data/Reactome_Pathway_Data/orthologyData");
		dlg.setFilter(".txt(*.txt);; All files (*.*)");
		dlg.resize(620,383);
		QStringList fileNames;
		QString name;

		if(dlg.exec())
			 fileNames = dlg.selectedFiles();
		if(fileNames.size()>0)
			name= fileNames[0];
	 
		QString Surfix = name.mid(name.size()-12, 12);
		//if (Surfix!="gallus_gallus_ortholog.txt" )
	    if (Surfix!="ortholog.txt" )
		{
		 	return;
		}	
		if( name.size() > 0 )	
		{			
			vector<ItemBase *> tlist = extractTypeBubblesinGroup(tBubble, TREERING);
			for(int i=0; i<tlist.size(); i++) 	
			{
				treeRingBubble* titem = dynamic_cast<treeRingBubble *>(tlist[i]);							
				if(titem)
				{	
					titem->_treeRing->SetColorBy(EDGE_BY_ORTHOLOGY);
					titem->_treeRing->ComputeNodeColorByO(name);	
					//titem->EDGE_BY=EDGE_BY_ORTHOLOGY;	
					titem->SetEdgeBy(EDGE_BY_ORTHOLOGY);
				}						
			}
			vector<PathBubble1 *> plist = extractPathBubblesinGroup(item);
			for(int i=0; i<plist.size(); i++) 	
			{
				if(plist[i])
				{	
					//pwdParser->readOrthologySymbol(name, plist[i]->orthologySymbol);
					//plist[i]->orthology = pwdParser->matchSymbol(plist[i]->orthologySymbol,  _proteinNameD[plist[i]->dataID]);					
					//plist[i]->drawOrthology = true;
					plist[i]->searchOrthology(name);
					plist[i]->getGraphToBePaint();
					//titem->_treeRing->SetColorBy(EDGE_BY_ORTHOLOGY);
					//titem->_treeRing->ComputeNodeColorByO(name);	
					//titem->EDGE_BY=EDGE_BY_ORTHOLOGY;						
				}						
			}			
		}
	}
	/*else if ( selectedAction == G2Pathway )
	{	
		QString name = "data/pathways/MitoticG2-G2";
		openPathBubblebyMenu(name, item, item==NULL?contextMenuEvent->scenePos():item->sceneBoundingRect().center()+ QPointF(item->realRect().width(),0));//contextMenuEvent->scenePos());	
    }	
	else if ( selectedAction == BMPPathway )
	{	
		QString name = "data/pathways/BMP";
		openPathBubblebyMenu(name, item, item==NULL?contextMenuEvent->scenePos():item->sceneBoundingRect().center()+ QPointF(item->realRect().width(),0));//contextMenuEvent->scenePos());	
    }
	else if ( selectedAction == TGFPathway )
	{	
		QString name = "data/pathways/68874_";
		openPathBubblebyMenu(name, item, item==NULL?contextMenuEvent->scenePos():item->sceneBoundingRect().center()+ QPointF(item->realRect().width(),0));//contextMenuEvent->scenePos());	
    }*/			
	else if ( selectedAction == removeAction )
	{
		QMessageBox msgBox;
		msgBox.setWindowTitle("Delete");
		msgBox.setText("Remove all the bubbles in the scene?");
		msgBox.setStandardButtons(QMessageBox::Yes);
		//msgBox.setDefaultButton(QMessageBox::No);
		msgBox.addButton (QMessageBox::No);
		if(msgBox.exec() == QMessageBox::Yes)
		{  
			logOperation( "Remove all the bubbles in the scene\n" );
			removeAllItems();
		}
	}
	/*else if ( selectedAction == saveScene )
	{
		QString fileName = QFileDialog::getSaveFileName();
		this->recordSceneAsXmlFile( fileName );
	}
	else if ( selectedAction == recoverScene )
	{
		QString fileName = QFileDialog::getOpenFileName();
		this->recoverSceneFromXmlFile( fileName );
	}*/
	if (selectedAction == searchSettingsProteinAction)
	{
		/*searchSettingsProteinAction->setChecked(true);		
		searchSettingsCompartmentAction->setChecked(false);
		searchSettingsPathwayAction->setChecked(false);*/
		setSearchType('P');
	}
	if (selectedAction == searchSettingsCompartmentAction)
	{
	    /*searchSettingsProteinAction->setChecked(false);
		searchSettingsCompartmentAction->setChecked(true);	
		searchSettingsPathwayAction->setChecked(false);*/
	    setSearchType('M');
	}
	if (selectedAction == searchSettingsPathwayAction)
	{
	    /*searchSettingsProteinAction->setChecked(false);
		searchSettingsCompartmentAction->setChecked(false);
		searchSettingsPathwayAction->setChecked(true);*/
		setSearchType('W');
	}

	if(item!=NULL)
	{	   
		if(tBubble!=NULL)
		{
		    if(selectedAction == expressionExpressedAction)
			{
				vector<ItemBase *> tlist = extractTypeBubblesinGroup(tBubble, TREERING);
				for(int i=0; i<tlist.size(); i++) 	
				{
					treeRingBubble* titem = dynamic_cast<treeRingBubble *>(tlist[i]);							
					if(titem)
					{	
						etemp[0]=TREERING;				
						etemp[1]=0;	
						etemp[2]=SEARCHEXPRESSED;
						eventRecord.push_back(etemp);
						//titem->getGraphToBePaint();
						titem->searchExpressed();
					}						
				}
			}
		
		}
		if(pBubble!=NULL) //(search + edit + part of tool))
		{
		    
			if (selectedAction == shareAction)
			{
				etemp[0]=pBubble->getType();
				etemp[1]=pBubble->m_pathwayID;	
				etemp[2]=SEARCHSHARED;				
				if(pBubble->searchType=='P')
				{
					if(pBubble->menuSelection_1(pos_menu, scenePos_menu, 0)) //p					
					{						
						eventRecord.push_back(etemp);
					}
				}
				else if(pBubble->searchType=='M')
				{
					if(pBubble->menuSelection_1(pos_menu, scenePos_menu, 1)) //C
					{
						eventRecord.push_back(etemp);
					}
				}
				else if(pBubble->searchType=='W')
			    {
				    if(pBubble->menuSelection_1(pos_menu, scenePos_menu, 2)) //PA
					{
						eventRecord.push_back(etemp);
				    }
				}
			}
			else if (selectedAction == diffAction)
			{
				etemp[0]=pBubble->getType();				
				etemp[1]=pBubble->m_pathwayID;	
				etemp[2]=SEARCHDIFFERED;
				if(pBubble->searchType=='P')
				{
					etemp[2]=SEARCHDIFFERED;
					if(pBubble->menuSelection_1(pos_menu, scenePos_menu, 10)) //P
					{
						eventRecord.push_back(etemp);					
					}
				}
				else if(pBubble->searchType=='M')
				{
					if(pBubble->menuSelection_1(pos_menu, scenePos_menu, 11)) //C
						eventRecord.push_back(etemp);
				}
				else if(pBubble->searchType=='W')
				{
					if(pBubble->menuSelection_1(pos_menu, scenePos_menu, 12)) //PA
						eventRecord.push_back(etemp);
				}
			}
			else if (selectedAction == linkNode)
			{ 
				 linkSearchType=1;
				if(pBubble->searchType=='P')
				{
					if(pBubble->menuSelection_1(pos_menu, scenePos_menu, 20)) //P
					{
						vector<PathBubble1 *> plist = extractPathBubblesinGroup(pBubble);
						for(int i=0; i<plist.size(); i++) 	
				        {
							    PathBubble1* pb=plist[i];	
								etemp[0]=pb->getType();				
								etemp[1]=pb->m_pathwayID;	
								etemp[2]=SEARCHLINKED;
								eventRecord.push_back(etemp);
						}
					}
				}
				else if(pBubble->searchType=='M')
				{
					if(pBubble->menuSelection_1(pos_menu, scenePos_menu, 21)) //C
					{
						vector<PathBubble1 *> plist = extractPathBubblesinGroup(pBubble);
						for(int i=0; i<plist.size(); i++) 	
				        {
							    PathBubble1* pb=plist[i];	
								etemp[0]=pb->getType();				
								etemp[1]=pb->m_pathwayID;	
								etemp[2]=SEARCHLINKED;
								eventRecord.push_back(etemp);
						}
					}
				}
				else if(pBubble->searchType=='W')
				{
					if(pBubble->menuSelection_1(pos_menu, scenePos_menu, 22)) //PA
					{

						eventRecord.push_back(etemp);
					}
				}
			}
			else if (selectedAction == linkPath)
			{ 
				 

				if(linkSearchType == 1)	
				{
					linkSearchType=3;
				}
				else linkSearchType=0;

				if(pBubble->searchType=='P')
				{
					if(pBubble->menuSelection_1(pos_menu, scenePos_menu, 20)) //P
					{
						vector<PathBubble1 *> plist = extractPathBubblesinGroup(pBubble);
						for(int i=0; i<plist.size(); i++) 	
				        {
							    PathBubble1* pb=plist[i];	
								etemp[0]=pb->getType();				
								etemp[1]=pb->m_pathwayID;	
								etemp[2]=SEARCHLINKED;
								eventRecord.push_back(etemp);
						}
					}
				}
				else if(pBubble->searchType=='M')
				{
					if(pBubble->menuSelection_1(pos_menu, scenePos_menu, 21)) //C
					{
						vector<PathBubble1 *> plist = extractPathBubblesinGroup(pBubble);
						for(int i=0; i<plist.size(); i++) 	
				        {
							    PathBubble1* pb=plist[i];	
								etemp[0]=pb->getType();				
								etemp[1]=pb->m_pathwayID;	
								etemp[2]=SEARCHLINKED;
								eventRecord.push_back(etemp);
						}
					}
				}
				else if(pBubble->searchType=='W')
				{
					if(pBubble->menuSelection_1(pos_menu, scenePos_menu, 22)) //PA
					{

						eventRecord.push_back(etemp);
					}
				}
			}			
			else if(selectedAction == expressionExpressedAction)
			{
				pBubble->menuSelection(pos_menu, scenePos_menu, 1);
				expressedGeneNames = pBubble->getExpressedGeneNames();
				getGroup(pBubble)->expressedSets = pBubble->assignExpressedSets();
				vector<PathBubble1 *> plist = extractPathBubblesinGroup(pBubble);

				etemp[0]=pBubble->getType();		
				etemp[2]=SEARCHEXPRESSED;

				for(int i=0; i<plist.size(); i++) 	
				{
					PathBubble1* pBubble=plist[i];	
					int Did=pBubble->dataID, pid=pBubble->m_pathwayID;
				    if(!pBubble->expressedProtein.empty() || !pBubble->expressedComplex.empty() || !pBubble->expressedDna.empty() || !pBubble->expressedCompartment.empty()  || !pBubble->expressedPathway.empty() )
					{
						 pBubble->getGraphToBePaint();	
						 etemp[1]=pBubble->m_pathwayID;	
						 eventRecord.push_back(etemp);
						 pBubble->storeColored();
					}
				}
								
				
			}
			else if (selectedAction == clearSearchAction )
			{
				clearSearchSelection(pBubble);
				//keqin
			}
			else if (selectedAction == colorUpAction)
			{
				vector<PathBubble1 *> mList= extractPathBubblesinGroup(item);
				for(int m=0; m<mList.size(); m++)
				{
					PathBubble1* tmp = dynamic_cast<PathBubble1*>(mList[m]);
					tmp->menuSelection(pos_menu, scenePos_menu, 10);
				}	
			}
			else if (selectedAction == colorEvenAction)
			{
				vector<PathBubble1 *> mList= extractPathBubblesinGroup(item);
				for(int m=0; m<mList.size(); m++)
				{
					PathBubble1* tmp = dynamic_cast<PathBubble1*>(mList[m]);
					tmp->menuSelection(pos_menu, scenePos_menu, 11);
				}
			}
			else if (selectedAction == colorDownAction)
			{
				vector<PathBubble1 *> mList= extractPathBubblesinGroup(item);
				for(int m=0; m<mList.size(); m++)
				{
					PathBubble1* tmp = dynamic_cast<PathBubble1*>(mList[m]);
					tmp->menuSelection(pos_menu, scenePos_menu, 12);
				}				
			}
			else if (selectedAction == graphResetAction)
			{
				pBubble->menuSelection(pos_menu, scenePos_menu, 20);
				clearSearchSelection(pBubble);
			}
			else if (selectedAction == aggregationAction)
			{
				PathBubble1* ABubble=NULL;
				QList<ItemBase *> mList = getGroupMembers(item);
				for(int m=0; m<mList.size(); m++)
				{
					
					rtemp[0]=mList[m]->getType(); 
					if( rtemp[0]==PATHBUBBLE1 || rtemp[0]==SUBPATHBUBBLE1)
					{
						PathBubble1* tmp = dynamic_cast<PathBubble1*>(mList[m]);
						tmp->aggregation();
						ABubble=tmp;
					}
				
				}	
				//if(ABubble!=NULL)
				//ABubble->recordPathBubbleConnections();				
			}
			else if (selectedAction == collapseAction)
			{
				if(!node.empty() && node[1]=='L')
				{
					QList<ItemBase *> mList = getGroupMembers(item);
					PathBubble1* ABubble=NULL;
					for(int m=0; m<mList.size(); m++)
					{
						rtemp[0]=mList[m]->getType(); 
						if( rtemp[0]==PATHBUBBLE1 || rtemp[0]==SUBPATHBUBBLE1)
						{
							////
							PathBubble1* tmp = dynamic_cast<PathBubble1*>(mList[m]);	
							if(tmp->m_pathwayID==node[0])
							{
								tmp->collapse(node[2]);
								ABubble=tmp;
							}
						}
					}
				}
				else
				{
					QList<ItemBase *> mList = getGroupMembers(item);
					
					int flag=0;
					for(int m=0; m<mList.size(); m++)
					{
						rtemp[0]=mList[m]->getType(); 
						if( rtemp[0]==PATHBUBBLE1 || rtemp[0]==SUBPATHBUBBLE1)
						{
							 PathBubble1* tmp = dynamic_cast<PathBubble1*>(mList[m]);
							 int flag1= tmp->collapse();
							 if(flag1==-1)
							    flag=-1;
							 if(flag1>0)
                             {
								 if(flag>=0)
								    flag=1; //  the case no aggragated selected but thare are aggregated nodes in the bubbles
							 }
						}
					}
					if(flag==0)
					{
						QMessageBox msgBox;			
						msgBox.setWindowTitle("No aggregated node is selected");
						if(mList.size()>1)
						    msgBox.setText("Do you want to extend all aggregated nodes in this group?");
						else
							msgBox.setText("Do you want to extend all aggregated nodes in this bubble?");
						msgBox.addButton (QMessageBox::No);
						msgBox.setStandardButtons(QMessageBox::Yes);
						if(msgBox.exec() == QMessageBox::Yes)
						{
						    for(int m=0; m<mList.size(); m++)
							{
								rtemp[0]=mList[m]->getType(); 
								if( rtemp[0]==PATHBUBBLE1 || rtemp[0]==SUBPATHBUBBLE1)
								{
										PathBubble1* tmp = dynamic_cast<PathBubble1*>(mList[m]);
										tmp->collapseALL();
								}
							}
						}						
					}
					
				}

			}
			/*else if (selectedAction == toolUndoAction)
			{
				QList<ItemBase *> mList = getGroupMembers(item);
				for(int m=0; m<mList.size(); m++)
				{
					rtemp[0]=mList[m]->getType(); 
					if( rtemp[0]==PATHBUBBLE1 || rtemp[0]==SUBPATHBUBBLE1)
					{
						rtemp[1]=mList[m]->m_pathwayID;
				        undoRecord.push_back(rtemp);								    
					}
					else if( rtemp[0]==EXPRESSIONBUBBLE )
					{
					    rtemp[1]=mList[m]->m_expressionID;
				        undoRecord.push_back(rtemp);								    					
					}
				}	
				undo(undoRecord);					
			}
			else if (selectedAction == toolRedoAction)
			{
				QList<ItemBase *> mList = getGroupMembers(item);
				for(int m=0; m<mList.size(); m++)
				{
					rtemp[0]=mList[m]->getType(); 
					if( rtemp[0]==PATHBUBBLE1 || rtemp[0]==SUBPATHBUBBLE1)
					{
						rtemp[1]=mList[m]->m_pathwayID;
				        redoRecord.push_back(rtemp);								    
					}
					else if( rtemp[0]==EXPRESSIONBUBBLE )
					{
					    rtemp[1]=mList[m]->m_expressionID;
				        redoRecord.push_back(rtemp);								    					
					}
					//rtemp[0]=mList[m]->getType(); rtemp[1]=mList[m]->m_pathwayID;
				    //redoRecord.push_back(rtemp);								    
				}	
				redo(redoRecord);	
			}*/				
			//else if (selectedAction == searchSettingsAction)
			//{
			//	ItemBase *newItem = openWebNoteBubblebyMenu(SEARCHSETTINGS, item, contextMenuEvent->scenePos());
			//	WebBlock *settingItem = dynamic_cast<WebBlock *>(newItem);
			//	settingItem->_settings->setInvisible();
			//}
			/*else if ( selectedAction == viewRateLimitAction )
			{
				//ItemBase *newItem = openWebNoteBubblebyMenu(SETTINGS, item, contextMenuEvent->scenePos());
				//WebBlock *settingItem = dynamic_cast<WebBlock *>(newItem);
				//settingItem->_settings->setInvisible(); 
				if(drawRateLimit)
				{
					drawRateLimit=false;
					setDrawRateLimit(false);					
				}
				else
				{
					drawRateLimit=true;
					setDrawRateLimit(true);									
				}
			}			
			else if ( selectedAction == viewCrossTalkAction )
			{
				//ItemBase *newItem = openWebNoteBubblebyMenu(SETTINGS, item, contextMenuEvent->scenePos());
				//WebBlock *settingItem = dynamic_cast<WebBlock *>(newItem);
				//settingItem->_settings->setInvisible(); 
				if(drawCrossTalk)
				{
					drawCrossTalk=false;
					setDrawCrossTalk(false);					
				}
				else
				{
					drawCrossTalk=true;
					setDrawCrossTalk(true);									
				}
			}	*/
         }
		
		 if (selectedAction == delAction)
		 {
			QMessageBox msgBox;			
			msgBox.setWindowTitle("Delete Bubble");
			msgBox.setText("Delete this bubble?");
			msgBox.setStandardButtons(QMessageBox::Yes);
			msgBox.addButton (QMessageBox::No);
			msgBox.move(eventScreenPos.x(), eventScreenPos.y());
			if(msgBox.exec() == QMessageBox::Yes)
			{
				 if(item->getType()==SUBPATHBUBBLE1||item->getType()==PATHBUBBLE1) 
				 {
					 //if deleting a child bubble, then merge it back to father
					 //findEnclosed( pBubble					 
					 if(!delAChildPathBubble(pBubble))
					    pBubble->menuSelection(pos_menu, scenePos_menu, 50);
				 }
				 else if(item->getType()==EXPRESSIONBUBBLE) 
				 {
					 expressionBubble * ebubble = dynamic_cast<expressionBubble *> (item);
					 eBubble->deleteSelectedItems(this);
				 }
				 else if(item->getType()==EXPRESSIONBUBBLE) 
				 {
					 TextBubble2 * tbubble = dynamic_cast< TextBubble2 *> (item);
					 tbubble->deleteSelectedItems(this);
				 }
				 else if(item->getType()==CALLOUT) 
				 {
					 item->deleted=true;
					 CalloutNote * cbubble = dynamic_cast< CalloutNote *> (item);
					 item->unGroup();
					cbubble->hide();
					cbubble->m_note->hide();
					//nb->_myControl->setVisible(false);
		            cbubble->m_note->setVisible(false);
					  //keqin hide, better to be deleted, handle this later
				 }
				 else  if(item->getType()==TREERING) 
				 {	 
					 bubbleDeleted(item);	
				     item->deleteSelectedItems(this);
				 }
			     else
					 item->deleteSelectedItems(this);
			}
		 }
		 /*else if(selectedAction == unGroupAction)
		 {		
			item->unGroupbyMenu();	//menuSelection(pos_menu, scenePos_menu, 51);			
		 }*/
		 else if(selectedAction == delRingAction)
		 {
		       tBubble->subStarctTreeRing();		 
		 }
		 else if(selectedAction == delGroupAction)
		 {		
			QMessageBox msgBox;
			msgBox.setWindowTitle("Delete Group");
			msgBox.setText("Delete this group?");
			msgBox.setStandardButtons(QMessageBox::Yes);
			msgBox.addButton (QMessageBox::No);
			msgBox.move(eventScreenPos.x(), eventScreenPos.y());
			if(msgBox.exec() == QMessageBox::Yes)
			{
				if(pBubble!=NULL)
				{
					pBubble->menuSelection(pos_menu, scenePos_menu, 51);
				}
			}
		 }
		 else if (selectedAction == saveGraph)
		 {
		     pBubble->menuSelection(pos_menu, scenePos_menu, 23);
		 }
		 else if (selectedAction == loadGraph)
		 {
		     pBubble->menuSelection(pos_menu, scenePos_menu, 24);
		 }
		 if (selectedAction == sketchAction)
		 {
			vector<PathBubble1 *> mList= extractPathBubblesinGroup(item);
			for(int m=0; m<mList.size(); m++)
			{
				PathBubble1* tmp = dynamic_cast<PathBubble1*>(mList[m]);
				tmp->menuSelection(pos_menu, scenePos_menu, 39);
			}
			vector<ItemBase *> tlist = extractTypeBubblesinGroup(tBubble, TREERING);
			for(int i=0; i<tlist.size(); i++) 	
			{
				treeRingBubble* titem = dynamic_cast<treeRingBubble *>(tlist[i]);							
				if(titem)
				{	
					etemp[0]=TREERING;				
					etemp[1]=0;	
					etemp[2]=SEARCHEXPRESSED;
					eventRecord.push_back(etemp);					
					titem->setPenOn();
				}						
			}			
		}
		else if (selectedAction == queryAction)
		{
			/*vector<PathBubble1 *> mList= extractPathBubblesinGroup(item);
			for(int m=0; m<mList.size(); m++)
			{
				PathBubble1* tmp = dynamic_cast<PathBubble1*>(mList[m]);
				tmp->menuSelection(pos_menu, scenePos_menu, 49);
			}*/
			if(node.size()>=3)
			   openWebNoteBubblebyMenu(EGIFT, item, item==NULL?eventScenePos:item->sceneBoundingRect().center()+ QPointF(item->realRect().width(),0), node[0], node[1], node[2]);
		}
		if(selectedAction == viewSmallMoleculeAction)
		{
			if(drawSmallmoleculeSettings)
			{
				drawSmallmoleculeSettings=false;
				setDrawSmallMolecule(false);					
			}
			else
			{
				drawSmallmoleculeSettings=true;
				setDrawSmallMolecule(true);									
			}
		}		
		else if ( selectedAction == searchPathwayAction )
		{
			openWebNoteBubblebyMenu(SEARCHFIND, item, eventScenePos);					
		}
	     
	}
	else //at open space (search + edit + part of tool)
	{
		if(selectedAction == viewSmallMoleculeAction)
		{
			if(drawSmallmoleculeSettings)
			{
				drawSmallmoleculeSettings=false;
				setDrawSmallMolecule(false);					
			}
			else
			{
				drawSmallmoleculeSettings=true;
				setDrawSmallMolecule(true);									
			}
		}		
		else if ( selectedAction == searchPathwayAction )
		{
			openWebNoteBubblebyMenu(SEARCHFIND, NULL, eventScenePos);					
		}
		QList<Group*> groupList = this->_manager->groupList();
		for ( int i = 0; i < groupList.size(); i ++)
		{	
				PathBubble1 *pBubble = findAPathBubbleinGroup(groupList[i]);
				if(pBubble==NULL)
				{
					/*if (selectedAction == expressionExpressedAction  )
					{
						vector<ItemBase *> tlist = extractTypeBubblesinGroup(pBubble, TREERING);
						for(int i=0; i<tlist.size(); i++) 	
						{
							TreeRing* titem = dynamic_cast<TreeRing*>(tlist[i]);							
							if(!titem)
							{	
							    etemp[0]=pBubble->getType();				
								etemp[1]=pBubble->m_pathwayID;	
								etemp[2]=SEARCHEXPRESSED;
								eventRecord.push_back(etemp);
								titem->getGraphToBePaint();
							}
						}
					}
					else
					{
					
					
					}*/
					continue;
				}
				if (selectedAction == graphResetAction)
				{
					pBubble->menuSelection(pos_menu, scenePos_menu, 20);
					clearSearchSelection(pBubble);
				}			
				else if (selectedAction == shareAction)
				{
					etemp[0]=pBubble->getType();				
				    etemp[1]=pBubble->m_pathwayID;	
				    etemp[2]=SEARCHSHARED;
					if(pBubble->searchType=='P')
					{
						if(pBubble->menuSelection_1(pos_menu, scenePos_menu, 0)) //p					
							eventRecord.push_back(etemp);
					}
					else if(pBubble->searchType=='M')
					{
						if(pBubble->menuSelection_1(pos_menu, scenePos_menu, 1))//C
						  eventRecord.push_back(etemp);
					}
					else if(pBubble->searchType=='W')
					{
						if(pBubble->menuSelection_1(pos_menu, scenePos_menu, 2)) //PA
						    eventRecord.push_back(etemp);
					}
				}
				else if (selectedAction == diffAction)
				{
					etemp[0]=pBubble->getType();				
				    etemp[1]=pBubble->m_pathwayID;	
				    etemp[2]=SEARCHDIFFERED;
					if(pBubble->searchType=='P')
					{
						if(pBubble->menuSelection_1(pos_menu, scenePos_menu, 10)) //P
							eventRecord.push_back(etemp);
					}
					else if(pBubble->searchType=='M')
					{
						if(pBubble->menuSelection_1(pos_menu, scenePos_menu, 11)) //C
							eventRecord.push_back(etemp);
					}
					else if(pBubble->searchType=='W')
					{
						if(pBubble->menuSelection_1(pos_menu, scenePos_menu, 12)) //PA
							eventRecord.push_back(etemp);
					}
				}
				else if (selectedAction == linkNode)
			    { 
				    linkSearchType=1;
					etemp[0]=pBubble->getType();				
				    etemp[1]=pBubble->m_pathwayID;	
				    etemp[2]=SEARCHLINKED;
					if(pBubble->searchType=='P')
					{
						if(pBubble->menuSelection_1(pos_menu, scenePos_menu, 20)) //P
						    eventRecord.push_back(etemp);
					}
					else if(pBubble->searchType=='M')
					{
						if(pBubble->menuSelection_1(pos_menu, scenePos_menu, 21)) //C
							eventRecord.push_back(etemp);
					}
					else if(pBubble->searchType=='W')
					{
						if(pBubble->menuSelection_1(pos_menu, scenePos_menu, 22)) //PA
						   eventRecord.push_back(etemp);
					}
			    }			
				else if (selectedAction == linkPath)
				{ 
					linkSearchType=0;
					
					etemp[0]=pBubble->getType();				
				    etemp[1]=pBubble->m_pathwayID;	
				    etemp[2]=SEARCHLINKED;
					if(pBubble->searchType=='P')
					{
						if(pBubble->menuSelection_1(pos_menu, scenePos_menu, 20)) //P
						    eventRecord.push_back(etemp);
					}
					else if(pBubble->searchType=='M')
					{
						if(pBubble->menuSelection_1(pos_menu, scenePos_menu, 21)) //C
							eventRecord.push_back(etemp);
					}
					else if(pBubble->searchType=='W')
					{
						if(pBubble->menuSelection_1(pos_menu, scenePos_menu, 22)) //PA
						   eventRecord.push_back(etemp);
					}
				}				
				else if (selectedAction == expressionExpressedAction  )
				{
					etemp[0]=pBubble->getType();				
				    etemp[1]=pBubble->m_pathwayID;	
				    etemp[2]=SEARCHEXPRESSED;

					pBubble->menuSelection(pos_menu, scenePos_menu, 1);
					expressedGeneNames = pBubble->getExpressedGeneNames();
					getGroup(pBubble)->expressedSets = pBubble->assignExpressedSets();
					vector<PathBubble1 *> plist = extractPathBubblesinGroup(pBubble);
					for(int i=0; i<plist.size(); i++) 	
					{
						PathBubble1* pbubble=plist[i];	
						int Did=pbubble->dataID, pid=pbubble->m_pathwayID;
						if(!pbubble->expressedProtein.empty() || !pbubble->expressedComplex.empty() || !pbubble->expressedDna.empty() || !pbubble->expressedCompartment.empty() || !pbubble->expressedPathway.empty())
						{	
							eventRecord.push_back(etemp);
							pbubble->getGraphToBePaint();
						}
					}						
				}
				else if (selectedAction == clearSearchAction )
				{
					clearSearchSelection(pBubble);
					//keqin
				}
				else if (selectedAction == aggregationAction)
				{
					PathBubble1* ABubble=NULL;
					QList<ItemBase *> pList= groupList[i]->getMembers();
					for(int k=0; k<pList.size(); k++) 
					{						
						if ( pList[i]->getType() == SUBPATHBUBBLE1 || pList[i]->getType() == PATHBUBBLE1)
						{
							PathBubble1* pbubble = dynamic_cast<PathBubble1*>(pList[i]);							
							pbubble->aggregation();
							ABubble = pbubble;
						}
					}
					//if(ABubble!=NULL)
					//   ABubble->recordPathBubbleConnections();
				}
				else if (selectedAction == collapseAction)
				{
					PathBubble1* ABubble=NULL;
					QList<ItemBase *> pList= groupList[i]->getMembers();
					for(int k=0; k<pList.size(); k++) 
					{						
						if ( pList[i]->getType() == SUBPATHBUBBLE1 || pList[i]->getType() == PATHBUBBLE1)
						{
							PathBubble1* pbubble = dynamic_cast<PathBubble1*>(pList[i]);							
							for(int id=0; id<pbubble->ANodePos.size(); id++)
							{
								if(pbubble->ANodeRect[id].width()>0)
								{			
									pbubble->collapse(id);
									ABubble = pbubble;
								}
							}							
						}
					}	
					//if(ABubble!=NULL)
					 //  ABubble->recordPathBubbleConnections();
				}	
				/*else if (selectedAction == toolUndoAction)
				{
					QList<ItemBase *> pList= groupList[i]->getMembers();
					for(int k=0; k<pList.size(); k++) 
					{						
						
						rtemp[0]=pList[k]->getType(); 
						if ( rtemp[0] == SUBPATHBUBBLE1 || rtemp[0] == PATHBUBBLE1)
						{
							rtemp[1]=pList[k]->m_pathwayID;
							undoRecord.push_back(rtemp);							
						}						
						else if( rtemp[0]==EXPRESSIONBUBBLE )
						{
							rtemp[1]=pList[k]->m_expressionID;
							undoRecord.push_back(rtemp);								    					
						}
					}
					undo(undoRecord);					
				}
				else if (selectedAction == toolRedoAction)
				{
					
					QList<ItemBase *> pList= groupList[i]->getMembers();
					for(int k=0; k<pList.size(); k++) 
					{		
						rtemp[0]=pList[k]->getType(); 
						if ( rtemp[0] == SUBPATHBUBBLE1 || rtemp[0] == PATHBUBBLE1)
						{
							rtemp[1]=pList[k]->m_pathwayID;
							redoRecord.push_back(rtemp);							
						}						
						else if( rtemp[0]==EXPRESSIONBUBBLE )
						{
							rtemp[1]=pList[k]->m_expressionID;
							redoRecord.push_back(rtemp);								    					
						}
					}
					redo(redoRecord);
				}	*/			
		}		
    }
	if(!eventRecord.empty())
	   storeEvent(eventRecord);
	else
	   eventRecord=eventRecord;
}

bool OpenGLScene::delAChildPathBubble(PathBubble1* pBubble, bool Ungroup)
 {
     if(!pBubble->toplevel && pBubble->_pathBubbleParent && pBubble->_pathBubbleParent->isVisible())
	 {
		 if(Ungroup)
		    pBubble->unGroupbyDelection();
		 /*emit unGroup( this );
	     logUnGroup();
		 UNGROUP=false;
		 AFTERUNGROUP=true;	*/		
		 emit findEnclosed(pBubble->_pathBubbleParent, pBubble);
		 return true;
	 }
	 else return false;		 
	 //void findEnclosed(PathBubble1* b0, PathBubble1* tmp);
 }


bool OpenGLScene::del_merg_PathBubble(PathBubble1* eBubble, PathBubble1* pBubble, bool Ungroup)
 {
    if(Ungroup)
	     pBubble->unGroupbyDelection();
		 /*emit unGroup( this );
	     logUnGroup();
		 UNGROUP=false;
		 AFTERUNGROUP=true;	*/		
    emit findEnclosed(eBubble, pBubble);	 
	return true;		 
	 //void findEnclosed(PathBubble1* b0, PathBubble1* tmp);
 }

bool OpenGLScene::CheckANodeIDinAllBubbles(QString AName)
{
	for (int k = 0; k < _pathBubbles.size(); k ++)		
	{
		int size=_pathBubbles.size();
		PathBubble1 * tmp =_pathBubbles[k];			
		if(_pathBubbles[k])
		if(_pathBubbles[k]!=NULL)
		if(_pathBubbles[k]->m_pathwayID>=0 && _pathBubbles[k]->m_pathwayID < _pathBubbles.size())
		{
			for(int i=1; i<_ANodeName[k].size(); i++)
			{
				if(k<_ANodeName.size() && i<_ANodeName[k].size() && 0<_ANodeName[k][i].size())
				if(_pathBubbles[k]->ANodeRect[i].x()>-990 && _ANodeName[k][i][0]==AName)
			    {
				    return true;
			    }
			}
		}
	}
	return false;
}


void OpenGLScene::storeEvent(vector< vector<int> > eventRecord)
{
	for ( int i = 0; i <eventRecord.size(); i ++)
	{
	    vector<int> etemp=eventRecord[i];
		int btype=etemp[0], bid=etemp[1], bevent=etemp[2];
		if(btype==PATHBUBBLE1 ||  btype==SUBPATHBUBBLE1)
		{
			PathBubble1* pbubble=dynamic_cast<PathBubble1*>(_pathBubbles[bid]);						
			pbubble->storeEvent(eventRecord[i][2]);
	       
			//case EXPRESSIONBUBBLE:
			//	break;
		}
	}

}

void OpenGLScene::undo(vector< vector<int> > undoRecord)
{
	int eventType=-1,temp;
	vector<PathBubble1*> pRecord;
	for ( int i = 0; i <undoRecord.size(); i ++)
	{
	   vector<int> etemp=undoRecord[i];
	   int btype=etemp[0], bid=etemp[1];
	   if(btype==PATHBUBBLE1 || btype==SUBPATHBUBBLE1)
	   {
			PathBubble1* pbubble=_pathBubbles[bid];						
			temp=pbubble->undo();
			if(temp>=0)
			{
				pRecord.push_back(pbubble); //suppose that the share event only apply to one bubble in a group
				eventType=temp;
			}
	   }
	   if(btype==EXPRESSIONBUBBLE)
	   {
		  _expressionBubbles[bid]->undo();
	   }
	}
	if(eventType == SEARCHSHARED)
	{
	    for ( int i = 0; i <pRecord.size(); i ++)
		{	  
			PathBubble1* pbubble=pRecord[i];
			getGroup(pbubble)->sharedSets.clear();
			getGroup(pbubble)->sharedSets=pbubble->assignSharedSets();		
		}
	}
}

void OpenGLScene::redo(vector< vector<int> > redoRecord)
{//undo anyting that change the graph (node, edge color, text, node location)
	int eventType=-1,temp;
	vector<PathBubble1*> pRecord;
	for ( int i = 0; i <redoRecord.size(); i ++)
	{
	   vector<int> etemp=redoRecord[i];
	   int btype=etemp[0], bid=etemp[1];
	   if(btype==PATHBUBBLE1 || btype==SUBPATHBUBBLE1)
	   {
			PathBubble1* pbubble=_pathBubbles[bid];						
			temp=pbubble->redo();	 
			if(temp>=0)
			{
				pRecord.push_back(pbubble); //suppose that the share event only apply to one bubble in a group
				eventType=temp;
			}
	   }
	   if(btype==EXPRESSIONBUBBLE)
	   {
		   _expressionBubbles[bid]->redo();
	   }
	}
	if(eventType == SEARCHSHARED)
	{
	    for ( int i = 0; i <pRecord.size(); i ++)
		{	  
			PathBubble1* pbubble=pRecord[i];
			getGroup(pbubble)->sharedSets.clear();
			getGroup(pbubble)->sharedSets=pbubble->assignSharedSets();		
		}
	}
}

bool OpenGLScene::isBubbleinCurrentWindow(ItemBase *B)
{
	bool flag=false;
	float x=0, y=0, w=B->screenWidth, h=B->screenHeight;
	QRectF sceneRect=QRectF(x,y,w,h);
	QPointF center=B->sceneBoundingRect().center();
	if(sceneRect.contains(center))
	{
		flag=true;
	}	
	return flag;
}

void OpenGLScene::setSearchType(int type)
{
	    searchType=type;
        QList<ItemBase *> mlist;
		mlist=allMembers();	 			
		for(int i=0; i<mlist.size(); i++) 
		{
			//int type=mlist[i]->getType();
			PathBubble1* pbubble = dynamic_cast<PathBubble1*>(mlist[i]);
			if ( mlist[i]->getType() == SUBPATHBUBBLE1 || mlist[i]->getType() == PATHBUBBLE1)
			{
				pbubble->searchType=type;		
			}
		}
}

void OpenGLScene::setDrawSmallMolecule(bool flag)
{	   
    QList<ItemBase *> mlist;
	mlist=allMembers();	 			
	for(int i=0; i<mlist.size(); i++) 
	{
		int type=mlist[i]->getType();
		PathBubble1* pbubble = dynamic_cast<PathBubble1*>(mlist[i]);
		if ( mlist[i]->getType() == SUBPATHBUBBLE1 || mlist[i]->getType() == PATHBUBBLE1)
		{
			clearUndoRedoRecord(pbubble);
			float cScale=pbubble->_scale;
			pbubble->drawSmallmolecule=flag;	
			float sRatio = pbubble->getGraphToBePaint();	
            
			pbubble->redrawPathWay(flag);

			/*
			float AFS2, AFS0;
			float cScale=pbubble->_scale;
			AFS0=pbubble->aveFontSize;

			pbubble->_scale=1.3;
			float sRatio = pbubble->getGraphToBePaint();				
			
			int AFS1=pbubble->aveFontSize;
			
			switch(AFS1)
			{
			   case 8: pbubble->_scale=1.45; break;
			   case 9: pbubble->_scale=1.37; break;			   
			   case 10: pbubble->_scale=1.30; break;			   
			   case 11: pbubble->_scale=1.25; break;			   		   
			   case 12: pbubble->_scale=1.175; break;			   		   
			   case 13: pbubble->_scale=1.1; break;			   		    		   
			   case 14: pbubble->_scale=1.025; break;			   		    		   
			   case 15: pbubble->_scale=0.96; break;			   		    		   
			   case 16: pbubble->_scale=0.93; break;
			   default: pbubble->_scale=1.3; break;			   		    
			}
			if(AFS1<8) 
				pbubble->_scale=1.4;
			else if(AFS1>16) 
				pbubble->_scale=0.90;
            */
			if(pbubble->drawSmallmolecule)
	        {
				//pbubble->_scale = pbubble->_scale * (0.85+0.15 * (1-sRatio));
				//pbubble->_scale = pbubble->_scale * (cScale/pbubble->_ScaleBackup);	
				pbubble->_scale = pbubble->_scale * (0.85+0.15 * (1-sRatio));
				pbubble->_scale = pbubble->_ScaleBackup_S * (cScale/pbubble->_ScaleBackup);					
			}
			else 				
	        {
				//pbubble->_scale = pbubble->_scale * (cScale/pbubble->_ScaleBackup_S);
				pbubble->_scale = pbubble->_ScaleBackup * (cScale/pbubble->_ScaleBackup_S);
				
			}
			if( pbubble->_scale <= 0.12*pbubble->_ScaleBackup)
				 pbubble->_scale =1.2*pbubble->_ScaleBackup;
			if( pbubble->_scale >= 3.8*pbubble->_ScaleBackup)
				 pbubble->_scale =3.8*pbubble->_ScaleBackup;

			pbubble->getGraphToBePaint();
		}
	}	
}

void OpenGLScene::setDrawCrossTalk(bool flag)
{	   
    QList<ItemBase *> mlist;
	mlist=allMembers();	 			
	for(int i=0; i<mlist.size(); i++) 
	{
		int type=mlist[i]->getType();
		PathBubble1* pbubble = dynamic_cast<PathBubble1*>(mlist[i]);
		if ( mlist[i]->getType() == SUBPATHBUBBLE1 || mlist[i]->getType() == PATHBUBBLE1)
		{
			pbubble->drawCrossTalk=flag;	
			//pbubble->redrawPathWay();	
			pbubble->getGraphToBePaint();	
		}
	}
}

void OpenGLScene::setDrawOrthology(bool flag)
{	   
    QList<ItemBase *> mlist;
	mlist=allMembers();	 			
	for(int i=0; i<mlist.size(); i++) 
	{
		int type=mlist[i]->getType();
		PathBubble1* pbubble = dynamic_cast<PathBubble1*>(mlist[i]);
		if ( mlist[i]->getType() == SUBPATHBUBBLE1 || mlist[i]->getType() == PATHBUBBLE1)
		{
			pbubble->drawOrthology=flag;	
			//pbubble->redrawPathWay();	
			pbubble->getGraphToBePaint();	
		}
	}
}

void OpenGLScene::setDrawRateLimit(bool flag)
{	   
    QList<ItemBase *> mlist;
	mlist=allMembers();	 			
	for(int i=0; i<mlist.size(); i++) 
	{
		int type=mlist[i]->getType();
		PathBubble1* pbubble = dynamic_cast<PathBubble1*>(mlist[i]);
		if ( mlist[i]->getType() == SUBPATHBUBBLE1 || mlist[i]->getType() == PATHBUBBLE1)
		{
			pbubble->drawRateLimit=flag;	
			//pbubble->redrawPathWay();	
			pbubble->getGraphToBePaint();	
		}
	}
}

bool OpenGLScene::isGroupinCurrentWindow(Group *B)
{
	bool flag=false;
	QList<ItemBase *> mList=B->getGroupMembers();
	vector<QRectF> rectList=B->assignRectList(mList);
    if(mList.size()>0)
	{
		float x=0, y=0, w=mList[0]->screenWidth, h=mList[0]->screenHeight;
		QRectF sceneRect=QRectF(x,y,w,h);
		for(int i=0; i<rectList.size(); i++)
		{
		   QPointF center=rectList[i].center();
		   if(sceneRect.contains(center))
		   {
			   flag=true;
		       break;
		   }
		}
	}
	return flag;
}

void OpenGLScene::deleteGroup(ItemBase *item)
{

        QList<ItemBase *> mlist=getGroupMembers(item);
		OpenGLScene *tscene=this;
		this->delectionStarter = item;
		vector<int> plist, clist,tlist;
		int pid=item->m_pathwayID;
		for(int i=0; i<mlist.size(); i++) 
		{
			if(mlist[i]->getType() == SUBPATHBUBBLE1 || mlist[i]->getType() == PATHBUBBLE1)
				plist.push_back(mlist[i]->m_pathwayID);						
			else
				plist.push_back(-1);

			if(mlist[i]->getType() == TEXTBUBBLE2)
				clist.push_back(mlist[i]->m_codeBubbleID);
			else 
				clist.push_back(-1);
	
			tlist.push_back(mlist[i]->getType());												

		}
		bool flag=true;
		for(int i=0; i<mlist.size(); i++) 
		{
			if(mlist[i]!=item)
            {
					if(tlist[i] == SUBPATHBUBBLE1 ||tlist[i] == PATHBUBBLE1)
					{
						if(tscene->_pathBubbles[plist[i]]!=NULL) 
						{
							PathBubble1* pbubble = dynamic_cast<PathBubble1*>(mlist[i]);
							pbubble->deleteSelectedItems(tscene);
						}
					}
					else if(tlist[i] == EXPRESSIONBUBBLE )
					{
						expressionBubble* ebubble = dynamic_cast<expressionBubble*>(mlist[i]);
						ebubble->deleteSelectedItems(tscene);
					}
					else if(tlist[i] == TEXTBUBBLE2 )
					{
						if(tscene->_codeBubbles[clist[i]]!=NULL)
						{
							TextBubble2* tbubble = dynamic_cast<TextBubble2*>(mlist[i]);
							tbubble->deleteSelectedItems(tscene);
						}
					}
					else								   
						mlist[i]->deleteSelectedItems(tscene);						  
			}
		}
		tscene->delectionStarter=NULL;
		if(pid>=0&&tscene->_pathBubbles[pid]!=NULL)
		{
			tscene->_pathBubbles[pid]->deleteSelectedItems(tscene);	
		}

}

void OpenGLScene::contextMenuEvent( QGraphicsSceneContextMenuEvent * contextMenuEvent )
{

	QPointF dis = contextMenuEvent->scenePos() - lastPressedPos;
	if( dis.x()*dis.x() +  dis.y()*dis.y() >400)
		bubbleDraggedout=true;  //not allowing menu opened if dragged a distance
	else 
	{
		bubbleDraggedout=false;
		lastPressedPos = QPointF(0,-1000000);
	}

	if(bubbleDraggedout)
	{
		bubbleDraggedout=false;
		return;
	}

	QGraphicsScene::contextMenuEvent( contextMenuEvent );	
	if( contextMenuEvent->isAccepted() )
	{
		return;
	}
	vector<int> node;
	dropDownMenu0(contextMenuEvent, NULL, node);
	
	return;
}

void OpenGLScene::setTopItem(ItemBase *item)
{
	QList<ItemBase*> itemList = this->_manager->allMembers();
	//item->setZValue(10);
	for( int i = 0; i < itemList.size(); i++ )	
	{
		if(itemList[i]!=item)
		   itemList[i]->setZValue(1);
		if(itemList[i]->getType()==CALLOUT)
		   itemList[i]->setZValue(100);
	}
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
			if( TYPE == PATHBUBBLE1 || TYPE == SUBPATHBUBBLE1 || TYPE == PATHBUBBLE2  || TYPE == TEXTBUBBLE2 || TYPE == REACTIONBUBBLE3 || TYPE == GROUPBUBBLE4 || TYPE == EXPRESSIONBUBBLE || TYPE == SEARCHFIND || TYPE == SETTINGS || TYPE == WEB || TYPE == NOTE  || TYPE == CALLOUT || TYPE == TREERING)
			{
				if(TYPE == TEXTBUBBLE2)
				{ 
					TextBubble2 *tb = dynamic_cast<TextBubble2*>(itemBase);
					itemBase->deleted=true;
					if(itemBase->isVisible())
					{
						delete [] tb->_text;						
					}					
				}
				if(TYPE == WEB)
				{
					WebBlock *tb = dynamic_cast<WebBlock*>(itemBase);
					itemBase->deleted=true;
					if(itemBase->isVisible())
					{
						itemBase->unGroup();
						tb->_web->hide();
					}
				}
				if(TYPE == SEARCHFIND)
				{
					WebBlock *tb = dynamic_cast<WebBlock*>(itemBase);
					itemBase->deleted=true;
					if(itemBase->isVisible())
					{
						itemBase->unGroup();
						tb->_search->hide();
					}
				}
				if(TYPE == SETTINGS)
				{
					WebBlock *tb = dynamic_cast<WebBlock*>(itemBase);
					itemBase->deleted=true;
					if(itemBase->isVisible())
					{
						itemBase->unGroup();
						tb->_settings->hide();
					}
				}
				if(TYPE == NOTE)
				{
					NoteBlock *nb = dynamic_cast<NoteBlock*>(itemBase);
					itemBase->deleted=true;
					if(itemBase->isVisible())
					{
						itemBase->unGroup();
						nb->m_note->hide();
					}
				}
				if(TYPE == CALLOUT)
				{
					CalloutNote *nb = dynamic_cast<CalloutNote*>(itemBase);
					//itemBase->deleted=true;					
					{
						int pid=nb->_pid;
						/*if(_pathBubbles[pid])
						{
							QString pathName = pwdParser->savePathNotePreparation(_pathBubbles[pid]->_pathName, userName);	
							nb->record(pathName);		
						}*/						
						itemBase->unGroup();
						nb->hide();
						nb->m_note->hide();						
		                nb->m_note->setVisible(false);
						//nb->~CalloutNote();	
					}
				}
				if(TYPE == PATHBUBBLE1 || TYPE == SUBPATHBUBBLE1)
				{
					PathBubble1 *tb = dynamic_cast<PathBubble1*>(itemBase);
					itemBase->deleted=true;
					
					_pathBubbles[tb->m_pathwayID]=NULL;
					if(itemBase->isVisible())
					{
						tb->deleteItems();
					}
					if(_pathBubbles[tb->m_pathwayID]==NULL)
					{
					   TYPE=TYPE;					
					}
					
					QString pathName;
					for( int i = 0; i <_nodeNote.size(); i++)
					{
						int pid=_nodeNote[i]->_pid;
						if( !_nodeNote[i]->deleted )
						{
							if(pid==tb->m_pathwayID)
							{
								if(pathName.size()==0)
									pathName = pwdParser-> savePathNotePreparation(tb->_pathName, userName);	
								if(!_nodeNote[i]->deleted)
								 {
									 _nodeNote[i]->record(pathName, _nodeNote[i]->deleted);				
									 _nodeNote[i]->deleted=true;
								}
							}
						}
					}
					//tb->m_pathwayID=-1;
				}	
				if(TYPE == TREERING)
				{
					treeRingBubble *tb = dynamic_cast<treeRingBubble*>(itemBase);
				    tb->_connections.clear();
				}
				itemBase->hide();
				//itemBase->properRemove();
				//itemBase->deleteSelectedItems(this);
			}
		}
	}
	
    edgeconnections.clear();
	edgeconnections_A.clear();
	edgeconnections_I.clear();
	edgeconnections_4.clear();
	
	
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
	

	 _proteinNameD.clear();        
	 _smallMoleculeNameD.clear(); 
	  _complexNameD.clear();
	 _DnaNameD.clear();
	 _reactionNameD.clear();
	 _physicalEntityNameD.clear();
	 _ANodeName.clear();
	_compartmentName.clear();
	_pathwayNameD.clear();
			 

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

	//reorder group and its members according to the time the bubble are created. 
	//QList<Group*> gList
	
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
			if(gList[i]->getMemberAt(j)->getType()==TREERING)
				str.setNum( gList[i]->getMemberAt(j)->m_treeRingBubbleID );
			else 
				str.setNum( gList[i]->getMemberAt(j)->m_pathwayID );
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
				_manager->addNewGroupFromXmlNode( eachElement ); //recover the bubbles
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


int OpenGLScene::searchInPathBubble(QString str, set<int> searchList, QString name, ItemBase *item)
{	
	bool Mflag=searchList.find('M')!=searchList.end(), Sflag=searchList.find('S')!=searchList.end(),
		Cflag=searchList.find('C')!=searchList.end(), Eflag=searchList.find('E')!=searchList.end(), 
		Pflag=searchList.find('P')!=searchList.end(), Aflag=searchList.find('L')!=searchList.end(),
		Rflag=searchList.find('R')!=searchList.end(), Dflag=searchList.find('D')!=searchList.end();
    //vector<bool> searchedRecord(_pathBubbles.size(), false);
	vector<set<vector<int>>> preSearched;
	preSearched.resize(_pathBubbles.size());
	QString p0,s0;

	int index;
	int count=0;
	vector<QRectF> nodePos;
	//searched
	for (int i = 0; i < _pathBubbles.size(); i++)		
	{
		if(_pathBubbles[i]==NULL || !_pathBubbles[i]->isVisible())
			continue;
		ItemBase* member=NULL;
		if(item!=NULL)
			member = dynamic_cast<ItemBase*>(_pathBubbles[i]);
		if(_pathBubbles[i]->_name.compare(name)==0 || name.compare("all")==0 || ( name.compare("")==0 && member==item))
		{
			if(!_pathBubbles[i]->searched.empty())//|| !_pathBubbles[i]->_linkage.empty())
			{
				preSearched[i]=_pathBubbles[i]->searched;
				_pathBubbles[i]->searched.clear();		      
			}		    
		}		
	}
	if(str.lastIndexOf("*") == str.size()-1 && str.indexOf("*") == 0)
	{	
		str = str.mid(1, str.size());		
		str = str.mid(0, str.size()-1);		
		
		for (int i = 0; i < _compartmentName.size(); i ++)
		{		
			if(_pathBubbles[i]==NULL  || !_pathBubbles[i]->isVisible())
				continue;
		     //= _pathBubbles[i]->compartmentPos;
			ItemBase* member=NULL;
			if(item!=NULL)
				member = dynamic_cast<ItemBase*>(_pathBubbles[i]);
			if(_pathBubbles[i]->_name.compare(name)==0 || name.compare("all")==0 || ( name.compare("")==0 && member==item))
			{
				int Did=_pathBubbles[i]->dataID;
				int Pid=i;
				
				nodePos= _pathBubbles[i]->compartmentPos;
				if(Mflag) 
				for (int j = 0; j < _compartmentName[i].size(); j ++)
				{	
					if(nodePos[j].x()>-999.99)
					if( _compartmentName[i][j][0].contains(str, Qt::CaseInsensitive) )
					{
						vector<int> item;
						item.push_back('M'); item.push_back(j);
						_pathBubbles[i]->searched.insert(item);
						count++;
					}
				}
				if(Cflag)
				{		
					nodePos= _pathBubbles[i]->complexPos;
					for (int j = 0; j < _complexNameD[Did].size(); j ++)
					{
						//for (int k = 0; k < _complexNameD[Did][j].size(); k ++)
						{
							if(nodePos[j].x()>-999.99)
						   if(_complexNameD[Did][j][0].contains(str, Qt::CaseInsensitive) )
						   {
							   vector<int> item;
							   item.push_back('C'); item.push_back(j);
							   _pathBubbles[i]->searched.insert(item);
							   count++;
						  }
						}
					}
				}
				if(Eflag)
				{		
					nodePos= _pathBubbles[i]->physicalEntityPos;
					for (int j = 0; j < _physicalEntityNameD[Did].size(); j ++)
					{
						//for (int k = 0; k < _physicalEntityNameD[Did][j].size(); k ++)
						{
						   if(nodePos[j].x()>-999.99)
						   if(_physicalEntityNameD[Did][j][0].contains(str, Qt::CaseInsensitive) )
						   {
							  vector<int> item;
							item.push_back('E'); item.push_back(j);
							_pathBubbles[i]->searched.insert(item);
							count++;
						  }
						}
					}
				}
				if(Aflag)
				{			
					nodePos= _pathBubbles[i]->ANodePos;
					for (int j = 0; j < _ANodeName[Pid].size(); j ++)
					{
						//for (int k = 0; k < _ANodeName[Pid][j].size(); k ++)
						{
							 if(nodePos[j].x()>-999.99)
						   if(_ANodeName[Pid][j][0].contains(str, Qt::CaseInsensitive) )
						   {
							  vector<int> item;
							  item.push_back('L'); item.push_back(j);
							  _pathBubbles[i]->searched.insert(item);
							  count++;
						  }
						}
					}
				}
				if(Pflag)
				{			
					nodePos= _pathBubbles[i]->proteinPos;
					for (int j = 0; j < _proteinNameD[Did].size(); j ++)
					{
						//for (int k = 0; k < _proteinNameD[Did][j].size(); k ++)
						{
							 if(nodePos[j].x()>-999.99)
						   if(_proteinNameD[Did][j][0].contains(str, Qt::CaseInsensitive) )
						   {
							 vector<int> item;
							item.push_back('P'); item.push_back(j);
							_pathBubbles[i]->searched.insert(item);
							count++;
						  }
						}
					}	
				}
				if(Rflag)
				{		
					nodePos= _pathBubbles[i]->reactionPos;
					for (int j = 0; j < _reactionNameD[Did].size(); j ++)
					{//for (int k = 0; k < _reactionNameD[Did][j].size(); k ++)
						{
							 if(nodePos[j].x()>-999.99)
						   if(_reactionNameD[Did][j][1].contains(str, Qt::CaseInsensitive) )
						   {
							  vector<int> item;
							  item.push_back('R'); item.push_back(j);
							  _pathBubbles[i]->searched.insert(item);
							  count++;
						  }
						}
					}
				}
				if(Sflag)
				{			
					nodePos= _pathBubbles[i]->smallMoleculePos;
					for (int j = 0; j < _smallMoleculeNameD[Did].size(); j ++)
					{
						//for (int k = 0; k < _smallMoleculeNameD[Did][j].size(); k ++)
						{
							if(nodePos[j].x()>-999.99)
						   if(_smallMoleculeNameD[Did][j][0].contains(str, Qt::CaseInsensitive) )
						   {
							  vector<int> item;
							  item.push_back('S'); item.push_back(j);
							  _pathBubbles[i]->searched.insert(item);
							  count++;
						  }
						}
					}
				}
				if(Dflag)
				{	
					nodePos= _pathBubbles[i]->DnaPos;
					for (int j = 0; j < _DnaNameD[Did].size(); j ++)
					{
						//for (int k = 0; k < _DnaNameD[Did][j].size(); k ++)
						{
							if(nodePos[j].x()>-999.99)
						   if(_DnaNameD[Did][j][0].contains(str, Qt::CaseInsensitive) )
						   {
							   vector<int> item;
							   item.push_back('D'); item.push_back(j);
							   _pathBubbles[i]->searched.insert(item);
							   count++;
						   }
						}
					}
				}
			}
		}
	}
	else if(str.lastIndexOf("*") == str.size()-1)
	{
		str = str.mid(0, str.size()-1);			
		for (int i = 0; i < _compartmentName.size(); i ++)
		{		
			if(_pathBubbles[i]==NULL  || !_pathBubbles[i]->isVisible())
				continue;
		
			ItemBase* member=NULL;
			if(item!=NULL)
				member = dynamic_cast<ItemBase*>(_pathBubbles[i]);

			if(_pathBubbles[i]->_name.compare(name)==0 || name.compare("all")==0 || (name.compare("")==0 && member==item))
			{
				int Did=_pathBubbles[i]->dataID;
				nodePos= _pathBubbles[i]->compartmentPos;
				if(Mflag)
				for (int j = 0; j < _compartmentName[i].size(); j ++)
				{	

					if(nodePos[j].x()>-999.99)
					if( _compartmentName[i][j][0].indexOf(str, 0, Qt::CaseInsensitive)==0 ) //&& _compartmentName[i][j][0][0]==str[0] )
					{
						p0 = _compartmentName[i][j][0].mid(0,1), s0=str.mid(0,1);
					    if(QString::compare(p0, s0, Qt::CaseInsensitive)==0)
						{
							vector<int> item;
							item.push_back('M'); item.push_back(j);
							_pathBubbles[i]->searched.insert(item);
							count++;
						}
					}
				}
				if(Cflag)
				{				
					nodePos= _pathBubbles[i]->complexPos;
					for (int j = 0; j < _complexNameD[Did].size(); j ++)
					{
						//for (int k = 0; k < _complexNameD[Did][j].size(); k ++)
						{
							if(nodePos[j].x()>-999.99)
							if(_complexNameD[Did][j][0].indexOf(str, 0, Qt::CaseInsensitive)==0)// && _complexNameD[i][j][0][0]==str[0] )
							{
								p0 = _complexNameD[Did][j][0].mid(0,1), s0=str.mid(0,1);
								if(QString::compare(p0, s0, Qt::CaseInsensitive)==0)
								{
									vector<int> item;
									item.push_back('C'); item.push_back(j);
									_pathBubbles[i]->searched.insert(item);
									count++;
								}
							}
						}
					}
				}
				if(Eflag)
				{		
					nodePos= _pathBubbles[i]->physicalEntityPos;
					for (int j = 0; j < _physicalEntityNameD[Did].size(); j ++)
					{
						//for (int k = 0; k < _physicalEntityNameD[Did][j].size(); k ++)
						{
							if(nodePos[j].x()>-999.99)
						   if(_physicalEntityNameD[Did][j][0].indexOf(str, 0, Qt::CaseInsensitive)==0 )//&& _physicalEntityNameD[i][j][0][0]==str[0] )
						   {
							    p0 =_physicalEntityNameD[Did][j][0].mid(0,1), s0=str.mid(0,1);
								if(QString::compare(p0, s0, Qt::CaseInsensitive)==0)
								{
									vector<int> item;
									item.push_back('E'); item.push_back(j);
									_pathBubbles[i]->searched.insert(item);
									count++;
								}
						  }
						}
					}
				}
				if(Pflag)
				{			
					nodePos= _pathBubbles[i]->proteinPos;
					for (int j = 0; j < _proteinNameD[Did].size(); j ++)
					{
						//or (int k = 0; k < _proteinNameD[Did][j].size(); k ++)
						{
							   if(nodePos[j].x()>-999.99)
							   if(_proteinNameD[Did][j][0].indexOf(str, 0, Qt::CaseInsensitive)==0 )//&& _proteinNameD[i][j][0][0]==str[0] )
							   {
									p0 = _proteinNameD[Did][j][0].mid(0,1), s0=str.mid(0,1);
									if(QString::compare(p0, s0, Qt::CaseInsensitive)==0)
									{
										 vector<int> item;
										item.push_back('P'); item.push_back(j);
										_pathBubbles[i]->searched.insert(item);
										count++;
									}
							  }
						}
					}	
				}
				if(Rflag)
				{		
					nodePos= _pathBubbles[i]->reactionPos;
					for (int j = 0; j < _reactionNameD[Did].size(); j ++)
					{
						//for (int k = 0; k < _reactionNameD[Did][j].size(); k ++)
						{
							 if(nodePos[j].x()>-999.99)
						   if(_reactionNameD[Did][j][1].indexOf(str, 0, Qt::CaseInsensitive)==0 )//&& _reactionNameD[i][j][0][0]==str[0] )
						   {
							   p0 = _reactionNameD[Did][j][1].mid(0,1), s0=str.mid(0,1);
							   if(QString::compare(p0, s0, Qt::CaseInsensitive)==0)
							   {
									vector<int> item;
									item.push_back('R'); item.push_back(j);
									_pathBubbles[i]->searched.insert(item);
									count++;
							   }
						  }
						}
					}
				}
				if(Sflag)
				{		
					nodePos= _pathBubbles[i]->smallMoleculePos;
					for (int j = 0; j < _smallMoleculeNameD[Did].size(); j ++)
					{
						//for (int k = 0; k < _smallMoleculeNameD[Did][j].size(); k ++)
						{
						   if(nodePos[j].x()>-999.99)
						   if(_smallMoleculeNameD[Did][j][0].indexOf(str, 0, Qt::CaseInsensitive)==0 )//&& _smallMoleculeNameD[i][j][0][0]==str[0] )
						   {
							    p0 = _smallMoleculeNameD[Did][j][0].mid(0,1), s0=str.mid(0,1);
								if(QString::compare(p0, s0, Qt::CaseInsensitive)==0)
								{
									vector<int> item;
									item.push_back('S'); item.push_back(j);
									_pathBubbles[i]->searched.insert(item);
									count++;
								}
						  }
						}
					}
				}
				if(Dflag)
				{		
					nodePos= _pathBubbles[i]->DnaPos;
					for (int j = 0; j < _DnaNameD[Did].size(); j ++)
					{
						//for (int k = 0; k < _DnaNameD[Did][j].size(); k ++)
						{
							if(nodePos[j].x()>-999.99)
						   if(_DnaNameD[Did][j][0].indexOf(str, 0, Qt::CaseInsensitive)==0 )//&& _DnaNameD[i][j][0][0]==str[0] )
						   {
							    p0 = _DnaNameD[Did][j][0].mid(0,1), s0=str.mid(0,1);
								if(QString::compare(p0, s0, Qt::CaseInsensitive)==0)
								{
									vector<int> item;
									item.push_back('D'); item.push_back(j);
									_pathBubbles[i]->searched.insert(item);
									count++;
								}
						   }
						}
					}
				}
			}
		}
	}
	else if(str.indexOf("*") == 0)
	{
		str = str.mid(1, str.size()-1);	
		//QString last=str[str.size()-1];
		for (int i = 0; i < _compartmentName.size(); i ++)
		{		
			if(_pathBubbles[i]==NULL  || !_pathBubbles[i]->isVisible())
				continue;
		
			ItemBase* member=NULL;
			if(item!=NULL)
				member = dynamic_cast<ItemBase*>(_pathBubbles[i]);
			if(_pathBubbles[i]->_name.compare(name)==0 || name.compare("all")==0 || ( name.compare("")==0 && member==item))
			{
				int Did=_pathBubbles[i]->dataID;
				int Pid=i;
				nodePos= _pathBubbles[i]->compartmentPos;
				if(Mflag)
				for (int j = 0; j < _compartmentName[i].size(); j++)
				{	
					if(nodePos[j].x()>-999.99)
					if( _compartmentName[i][j][0].lastIndexOf(str, -1, Qt::CaseInsensitive)>=0 )//&& _compartmentName[i][j][0][_compartmentName[i][j][0].size()-1]==str[str.size()-1] )
					{
						p0 = _compartmentName[i][j][0].mid(_compartmentName[i][j][0].size()-1,1), s0=str.mid(str.size()-1,1);					    
						if(QString::compare(p0, s0, Qt::CaseInsensitive)==0)
						{
							vector<int> item;
							item.push_back('M'); item.push_back(j);
							_pathBubbles[i]->searched.insert(item);
							count++;
						}
					}
				}
				if(Cflag)
				{			
					nodePos= _pathBubbles[i]->complexPos;
					for (int j = 0; j < _complexNameD[Did].size(); j ++)
					{
						//for (int k = 0; k < _complexNameD[Did][j].size(); k ++)
						{
							if(nodePos[j].x()>-999.99)
						   if(_complexNameD[Did][j][0].lastIndexOf(str, -1, Qt::CaseInsensitive)>=0 )//&& _complexNameD[i][j][k][_complexNameD[i][j][k].size()-1]==str[str.size()-1] )
						   {
							   p0 = _complexNameD[Did][j][0].mid(_complexNameD[Did][j][0].size()-1,1), s0=str.mid(str.size()-1,1);	
							   if(QString::compare(p0, s0, Qt::CaseInsensitive)==0)
							   {
								   vector<int> item;
								   item.push_back('C'); item.push_back(j);
								   _pathBubbles[i]->searched.insert(item);
								   count++;
							   }
						  }
						}
					}
				}
				if(Eflag)
				{		
					nodePos = _pathBubbles[i]->physicalEntityPos;
					for (int j = 0; j < _physicalEntityNameD[Did].size(); j ++)
					{
						//for (int k = 0; k < _physicalEntityNameD[Did][j].size(); k ++)
						{
							if(nodePos[j].x()>-999.99)
						   if(_physicalEntityNameD[Did][j][0].lastIndexOf(str, -1, Qt::CaseInsensitive)>=0 )//&& _physicalEntityNameD[i][j][0][_physicalEntityNameD[i][j][0].size()-1]==str[str.size()-1] )
						   {
							   p0 = _physicalEntityNameD[Did][j][0].mid(_physicalEntityNameD[Did][j][0].size()-1,1), s0=str.mid(str.size()-1,1);	
							   if(QString::compare(p0, s0, Qt::CaseInsensitive)==0)
						       {
									vector<int> item;
									item.push_back('E'); item.push_back(j);
									_pathBubbles[i]->searched.insert(item);
									count++;
							   }
						  }
						}
					}
				}
				if(Aflag)
				{			
					nodePos = _pathBubbles[i]->ANodePos;
					for (int j = 0; j < _ANodeName[Pid].size(); j ++)
					{
						//for (int k = 0; k < _ANodeName[Pid][j].size(); k ++)
						{
							if(nodePos[j].x()>-999.99)
						   if(_ANodeName[Pid][j][0].lastIndexOf(str, -1, Qt::CaseInsensitive)>=0 )//&& _ANodeName[i][j][0][_ANodeName[i][j][0].size()-1]==str[str.size()-1] )
						   {
							    p0 = _ANodeName[Pid][j][0].mid(_ANodeName[Pid][j][0].size()-1,1), s0=str.mid(str.size()-1,1);	
							    if(QString::compare(p0, s0, Qt::CaseInsensitive)==0)
						        {
									vector<int> item;
									item.push_back('L'); item.push_back(j);
									_pathBubbles[i]->searched.insert(item);
									count++;
								}
						  }
						}
					}
				}
				if(Pflag)
				{		
					nodePos = _pathBubbles[i]->proteinPos;
					for (int j = 0; j < _proteinNameD[Did].size(); j ++)
					{
						//for (int k = 0; k < _proteinNameD[Did][j].size(); k ++)
						{
							if(nodePos[j].x()>-999.99)
						   if(_proteinNameD[Did][j][0].lastIndexOf(str, -1, Qt::CaseInsensitive)>=0 )//&& _proteinNameD[i][j][0][_proteinNameD[i][j][0].size()-1]==str[str.size()-1] )
						   {
							    p0 = _proteinNameD[Did][j][0].mid(_proteinNameD[Did][j][0].size()-1,1), s0=str.mid(str.size()-1,1);	
							    if(QString::compare(p0, s0, Qt::CaseInsensitive)==0)
						        {
									 vector<int> item;
									item.push_back('P'); item.push_back(j);
									_pathBubbles[i]->searched.insert(item);
									count++;
								}
						  }
						}
					}	
				}
				if(Rflag)
				{		
					nodePos = _pathBubbles[i]->reactionPos;
					for (int j = 0; j < _reactionNameD[Did].size(); j ++)
					{
						//for (int k = 0; k < _reactionNameD[Did][j].size(); k ++)
						{
							if(nodePos[j].x()>-999.99)
						   if(_reactionNameD[Did][j][1].lastIndexOf(str, -1, Qt::CaseInsensitive)>=0) // && _reactionNameD[i][j][0][_reactionNameD[i][j][0].size()-1]==str[str.size()-1] )
						   {
							   p0 = _reactionNameD[Did][j][1].mid(_reactionNameD[Did][j][1].size()-1,1), s0=str.mid(str.size()-1,1);	
							   if(QString::compare(p0, s0, Qt::CaseInsensitive)==0)
						       {
									vector<int> item;
									item.push_back('R'); item.push_back(j);
									_pathBubbles[i]->searched.insert(item);
									count++;
							   }
						   }
						}
					}
				}
				if(Sflag)
				{			
					nodePos = _pathBubbles[i]->smallMoleculePos;
					for (int j = 0; j < _smallMoleculeNameD[Did].size(); j ++)
					{
						//for (int k = 0; k < _smallMoleculeNameD[Did][j].size(); k ++)
						{
							if(nodePos[j].x()>-999.99)
						   if(_smallMoleculeNameD[Did][j][0].lastIndexOf(str, -1, Qt::CaseInsensitive)>=0 )//&& _smallMoleculeNameD[i][j][0][_smallMoleculeNameD[i][j][0].size()-1]==str[str.size()-1] )
						   {
							   p0 = _smallMoleculeNameD[Did][j][0].mid(_smallMoleculeNameD[Did][j][0].size()-1,1), s0=str.mid(str.size()-1,1);	
							   if(QString::compare(p0, s0, Qt::CaseInsensitive)==0)
						       {
									vector<int> item;
									item.push_back('S'); item.push_back(j);
									_pathBubbles[i]->searched.insert(item);
									count++;
							   }
						  }
						}
					}
				}
				if(Dflag)
				{		
					nodePos = _pathBubbles[i]->DnaPos;
					for (int j = 0; j < _DnaNameD[Did].size(); j ++)
					{
						//for (int k = 0; k < _DnaNameD[Did][j].size(); k ++)
						{
							if(nodePos[j].x()>-999.99)
						   if(_DnaNameD[Did][j][0].lastIndexOf(str, -1, Qt::CaseInsensitive)>=0 )//&& _DnaNameD[i][j][0][_DnaNameD[i][j][0].size()-1]==str[str.size()-1] )
						   {
							   p0 = _smallMoleculeNameD[Did][j][0].mid(_smallMoleculeNameD[Did][j][0].size()-1,1), s0=str.mid(str.size()-1,1);	
							   if(QString::compare(p0, s0, Qt::CaseInsensitive)==0)
						       {
								   vector<int> item;
								   item.push_back('D'); item.push_back(j);
								   _pathBubbles[i]->searched.insert(item);
								   count++;
							   }
						   }
						}
					}
				}
			}
		}
	}	   
	else
	for (int i = 0; i < _compartmentName.size(); i ++)
	{		
		if(_pathBubbles[i]==NULL  || !_pathBubbles[i]->isVisible())
			continue;
		
		ItemBase* member=NULL;
		if(item!=NULL)
			member = dynamic_cast<ItemBase*>(_pathBubbles[i]);
		if(_pathBubbles[i]->_name.compare(name)==0 || name.compare("all")==0 || ( name.compare("")==0 && member==item))
		{
			int Did=_pathBubbles[i]->dataID;
			int Pid=i;
			nodePos = _pathBubbles[i]->compartmentPos;
			if(Mflag)
			for (int j = 0; j < _compartmentName[i].size(); j ++)
			{	
				if(nodePos[j].x()>-999.99)
				if( _compartmentName[i][j][0].contains(str,Qt::CaseInsensitive) && _compartmentName[i][j][0].size()==str.size() )
				{
					vector<int> item;
					item.push_back('M'); item.push_back(j);
					_pathBubbles[i]->searched.insert(item);
					count++;
				}
			}
			if(Cflag)
			{			
				nodePos = _pathBubbles[i]->complexPos;
				for (int j = 0; j < _complexNameD[Did].size(); j ++)
				{
					//for (int k = 0; k < _complexNameD[Did][j].size(); k ++)
					{
						if(nodePos[j].x()>-999.99)
					   if(_complexNameD[Did][j][0].contains(str,Qt::CaseInsensitive) && _complexNameD[Did][j][0].size()==str.size() )
					   {
						   vector<int> item;
						   item.push_back('C'); item.push_back(j);
						   _pathBubbles[i]->searched.insert(item);
						   count++;
					  }
					}
				}
			}
			if(Eflag)
			{			
				nodePos = _pathBubbles[i]->physicalEntityPos;
				for (int j = 0; j < _physicalEntityNameD[Did].size(); j ++)
				{
					//for (int k = 0; k < _physicalEntityNameD[Did][j].size(); k ++)
					{
						if(nodePos[j].x()>-999.99)
					   if(_physicalEntityNameD[Did][j][0].contains(str,Qt::CaseInsensitive) && _physicalEntityNameD[Did][j][0].size()==str.size() )
					   {
						  vector<int> item;
						item.push_back('E'); item.push_back(j);
						_pathBubbles[i]->searched.insert(item);
						count++;
					  }
					}
				}
			}
			if(Aflag)
			{		
				nodePos = _pathBubbles[i]->ANodePos;
				for (int j = 0; j < _ANodeName[Pid].size(); j ++)
				{
					//for (int k = 0; k < _ANodeName[Pid][j].size(); k ++)
					{
						if(nodePos[j].x()>-999.99)
					   if(_ANodeName[Pid][j][0].contains(str,Qt::CaseInsensitive) && _ANodeName[Pid][j][0].size()==str.size() )
					   {
						  vector<int> item;
						item.push_back('L'); item.push_back(j);
						_pathBubbles[i]->searched.insert(item);
						count++;
					  }
					}
				}
			}
			if(Pflag)
			{				
				nodePos = _pathBubbles[i]->proteinPos;
				for (int j = 0; j < _proteinNameD[Did].size(); j ++)
				{
					//for (int k = 0; k < _proteinNameD[Did][j].size(); k ++)
					{
						if(nodePos[j].x()>-999.99)
					   if(_proteinNameD[Did][j][0].contains(str,Qt::CaseInsensitive) && _proteinNameD[Did][j][0].size()==str.size() )
					   {
						 vector<int> item;
						item.push_back('P'); item.push_back(j);
						_pathBubbles[i]->searched.insert(item);
						count++;
					  }
					}
				}	
			}
			if(Rflag)
			{		
				nodePos = _pathBubbles[i]->reactionPos;
				for (int j = 0; j < _reactionNameD[Did].size(); j ++)
				{
					//for (int k = 0; k < _reactionNameD[Did][j].size(); k ++)
					{
						if(nodePos[j].x()>-999.99)
					   if(_reactionNameD[Did][j][1].contains(str,Qt::CaseInsensitive) && _reactionNameD[Did][j][1].size()==str.size() )
					   {
						  vector<int> item;
						  item.push_back('R'); item.push_back(j);
						  _pathBubbles[i]->searched.insert(item);
						  count++;
					  }
					}
				}
			}
			if(Sflag)
			{		
				nodePos = _pathBubbles[i]->smallMoleculePos;
				for (int j = 0; j < _smallMoleculeNameD[Did].size(); j ++)
				{
					//for (int k = 0; k < _smallMoleculeNameD[Did][j].size(); k ++)
					{
						if(nodePos[j].x()>-999.99)
					   if(_smallMoleculeNameD[Did][j][0].contains(str,Qt::CaseInsensitive) && _smallMoleculeNameD[Did][j][0].size()==str.size() )
					   {
						  vector<int> item;
						  item.push_back('S'); item.push_back(j);
						  _pathBubbles[i]->searched.insert(item);
						  count++;
					   }
					}
				}
			}
			if(Dflag)
			{				
				nodePos = _pathBubbles[i]->DnaPos;
				for (int j = 0; j < _DnaNameD[Did].size(); j ++)
				{
					//for (int k = 0; k < _DnaNameD[Did][j].size(); k ++)
					{
						if(nodePos[j].x()>-999.99)
					   if(_DnaNameD[Did][j][0].contains(str,Qt::CaseInsensitive) && _DnaNameD[Did][j][0].size()==str.size() )
					   {
						   vector<int> item;
						   item.push_back('D'); item.push_back(j);
						   _pathBubbles[i]->searched.insert(item);
						   count++;
					   }
					}
				}
			}
		}
	}	
	for (int i = 0; i < _pathBubbles.size(); i ++)		
	{
		if(_pathBubbles[i]!=NULL  && _pathBubbles[i]->isVisible())
		if( preSearched[i] != _pathBubbles[i]->searched )
		{
			_pathBubbles[i]->getGraphToBePaint();		
			
		}	
	}
	return count;	
}


int OpenGLScene::searchInExpressionBubble(QString str, set<int> searchList, QString name, ItemBase *item)
{	
	// vector<bool> searchedRecord(_pathBubbles.size(), false);
	//bool Pflag=searchList.find('P')!=searchList.end();
	//vector<bool> searchedRecord(_expressionBubbles.size(), false);
	vector<set<vector<int>>> preSearched;
	preSearched.resize(_expressionBubbles.size());
	int count=0;
	QString Str;
	for (int i = 0; i < _expressionBubbles.size(); i ++)		
	{
		Str=str;
		if(_expressionBubbles[i]==NULL || !_expressionBubbles[i]->isVisible())
			continue;
	
		if(Str.lastIndexOf("*") == Str.size()-1 && Str.indexOf("*") == 0)
	    {	
			Str = Str.mid(1, Str.size());		
			Str = Str.mid(0, Str.size()-1);		
			count = count + searchInExpressionBubble(Str, name, preSearched, i, item, 1);			
		}
		else if(Str.lastIndexOf("*") == Str.size()-1)
	    {
			Str = Str.mid(0, Str.size()-1);	
			count = count + searchInExpressionBubble(Str, name, preSearched, i, item, 2);
		}
		else if(Str.indexOf("*") == 0)
	    {
		     Str = Str.mid(1, Str.size()-1);	
			count = count + searchInExpressionBubble(Str, name, preSearched, i, item, 3);
		}
		else
		{		
			count = count + searchInExpressionBubble(Str, name, preSearched, i, item, 0);
		}
	}
	for (int i = 0; i < _expressionBubbles.size(); i ++)		
	{
		if(_expressionBubbles[i]==NULL || !_expressionBubbles[i]->isVisible())
			continue;
		if( preSearched[i] != _expressionBubbles[i]->searched)
		{
			_expressionBubbles[i]->getExpressionToBePaint();			
		}	
	}
	return count;
}

int OpenGLScene::searchPathwayInTreeRingforASymbol(QString str, set<int> searchList, QString name, ItemBase *item)
{	
	// vector<bool> searchedRecord(_pathBubbles.size(), false);
	//bool Pflag=searchList.find('P')!=searchList.end();
	//vector<bool> searchedRecord(_expressionBubbles.size(), false);
	int count=0;
	vector<QString> IDs = pwdParser->findPathwayFileforASymbol(str);

    //vector<QString> PWDParser::findPathwayFileforASymbol(QString symbol)
	for (int i = 0; i < m_treeRingBubbles.size(); i ++)		
	{
		if(m_treeRingBubbles[i]==NULL || !m_treeRingBubbles[i]->isVisible())
			continue;
		/*if( preSearched[i] != m_treeRingBubbles[i]->searched)
		{
			//m_treeRingBubbles[i]->getExpressionToBePaint();			
		}*/	
		//for(int j=0; j< IDs.size(); j++)
		count = count + m_treeRingBubbles[i]->_treeRing -> SearchFind(IDs);
	}
	return count;
}


int OpenGLScene::searchInTreeringBubble(QString str, set<int> searchList, QString name, ItemBase *item)
{	
	// vector<bool> searchedRecord(_pathBubbles.size(), false);
	//bool Pflag=searchList.find('P')!=searchList.end();
	//vector<bool> searchedRecord(_expressionBubbles.size(), false);
	int count=0;
	vector<set<vector<int>>> preSearched;
	preSearched.resize(m_treeRingBubbles.size());
	QString Str;
	for (int i = 0; i < m_treeRingBubbles.size(); i ++)		
	{
		Str=str;

		if(m_treeRingBubbles[i]==NULL || !m_treeRingBubbles[i]->isVisible())
			continue;
	
		if(Str.lastIndexOf("*") == Str.size()-1 && Str.indexOf("*") == 0)
	    {	
			 Str = Str.mid(1, Str.size());		
			 Str = Str.mid(0, Str.size()-1);
			 count = count + searchInTreeringBubble(Str, name, preSearched, i, item, 1);			
		}
		else if(Str.lastIndexOf("*") == Str.size()-1)
	    {
			 Str = Str.mid(0, Str.size()-1);	
			 count = count + searchInTreeringBubble(Str, name, preSearched, i, item, 2);
		}
		else if(Str.indexOf("*") == 0)
	    {
		     Str = Str.mid(1, Str.size()-1);	
			 count = count + searchInTreeringBubble(Str, name, preSearched, i, item, 3);
		}
		else
		{	
		    count = count + searchInTreeringBubble(Str, name, preSearched, i, item, 0);		
		}
	}
	for (int i = 0; i < m_treeRingBubbles.size(); i ++)		
	{
		if(m_treeRingBubbles[i]==NULL || !m_treeRingBubbles[i]->isVisible())
			continue;
		if( preSearched[i] != m_treeRingBubbles[i]->searched)
		{
			//m_treeRingBubbles[i]->getExpressionToBePaint();			
		}	
	}
	return count;
}

int OpenGLScene::searchInTreeringBubble(QString str, QString name, vector<set<vector<int>>> &preSearched, int i, ItemBase *item, int matchType)
{
	//vector<set<vector<int>>> preSearched;
	
	ItemBase* member=NULL;
	if(item!=NULL)
		member = dynamic_cast<ItemBase*>(m_treeRingBubbles[i]);
	int count=0;
	
	if(m_treeRingBubbles[i]->_name.compare(name)==0 || name.compare("all")==0 || ( name.compare("")==0 && member==item))
	{
		if(!m_treeRingBubbles[i]->searched.empty())//|| !_expressionBubbles[i]->_linkage.empty())
		{
			preSearched[i]=	m_treeRingBubbles[i]->searched;
			m_treeRingBubbles[i]->searched.clear();								
		}
		count = count + m_treeRingBubbles[i]->_treeRing->SearchFind(str,matchType);		
	}	
	return count;
}


int OpenGLScene::searchInExpressionBubble(QString str, QString name, vector<set<vector<int>>> &preSearched, int i, ItemBase *item, int matchType)
{
	//vector<set<vector<int>>> preSearched;
	ItemBase* member=NULL;
	if(item!=NULL)
		member = dynamic_cast<ItemBase*>(_expressionBubbles[i]);
	int count=0;
	if(_expressionBubbles[i]->_name.compare(name)==0 || name.compare("all")==0 || ( name.compare("")==0 && member==item))
	{
		if(!_expressionBubbles[i]->searched.empty())//|| !_expressionBubbles[i]->_linkage.empty())
		{
			preSearched[i]=	_expressionBubbles[i]->searched;
			_expressionBubbles[i]->searched.clear();
			//_expressionBubbles[i]->_linkage.clear();
			//searchedRecord[i]=true;							
		}
		//if(Pflag)
		for (int j = 0; j <_expressionBubbles[i]->geneInfo.size(); j ++)
		{
			for (int k = 0; k < _expressionBubbles[i]->geneInfo[j].size(); k ++)
			{
				bool flag=false,flag1,flag2;
				QString p0,s0;
				switch(matchType)
				{
				     case 0: flag = _expressionBubbles[i]->geneInfo[j][k].contains(str,Qt::CaseInsensitive) && _expressionBubbles[i]->geneInfo[j][k].size()==str.size(); break;
				     case 1: flag = _expressionBubbles[i]->geneInfo[j][k].contains(str,Qt::CaseInsensitive);  break;
					 case 2: flag1 = _expressionBubbles[i]->geneInfo[j][k].indexOf(str, 0, Qt::CaseInsensitive)==0;
							 p0 = _expressionBubbles[i]->geneInfo[j][k].mid(0,1), s0=str.mid(0,1);
							 flag2 = QString::compare(p0, s0, Qt::CaseInsensitive)==0;
							 flag = (flag1 && flag2);
							 break;
					 case 3: flag1 = _expressionBubbles[i]->geneInfo[j][k].lastIndexOf(str, -1, Qt::CaseInsensitive)>=0;
							 p0 = _expressionBubbles[i]->geneInfo[j][k].mid(_expressionBubbles[i]->geneInfo[j][k].size()-1,1), s0=str.mid(str.size()-1,1);
							 flag2 = QString::compare(p0, s0, Qt::CaseInsensitive)==0;
							 flag2 = QString::compare(p0, s0, Qt::CaseInsensitive)==0;
					         flag = (flag1 && flag2);
							 break;								
				}
				if(flag)
				{
					vector<int> item;
					item.push_back('P'); item.push_back(j);
					_expressionBubbles[i]->searched.insert(item);
					count++;
				}
			}
		}	
	}
	return count;
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
			backupItemPos(_pathBubbles[i]->m_pathwayID, _pathBubbles[i]->complexPos, _pathBubbles[i]->proteinPos, _pathBubbles[i]->smallMoleculePos, _pathBubbles[i]->DnaPos, _pathBubbles[i]->EmptyPos, _pathBubbles[i]->reactionPos, _pathBubbles[i]->physicalEntityPos, _pathBubbles[i]->compartmentPos, _pathBubbles[i]->ANodePos, _pathBubbles[i]->_scale, _pathBubbles[i]->dCenter, _pathBubbles[i]->drawSmallmolecule);
			
		}		
	}	
}

void OpenGLScene::backupEdge(int m_pathwayID, vector<vector<int>> edge)
{
	_edge.push_back(edge);
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
			vector<vector<int>> link;

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
			
			link.push_back(link0);
			link.push_back(link1);
		    
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
void OpenGLScene::backupItemPos(int m_pathwayID, vector<QRectF> complexPos, vector<QRectF> proteinPos, vector<QRectF> smallMoleculePos, vector<QRectF> DnaPos, vector<QRectF> EmptyPos, vector<QRectF> reactionPos, vector<QRectF> physicalEntityPos, vector<QRectF> compartmentPos, vector<QRectF> ANodePos, float scale, QPointF dCenter, bool drawSmallmoleculeSettings)
{  
	  //for subgraph, there is no edge yet
	  if(m_pathwayID==_complexPos.size())
	  {
		  _complexPos.push_back(complexPos), _proteinPos.push_back(proteinPos), _smallMoleculePos.push_back(smallMoleculePos), _DnaPos.push_back(DnaPos),  _EmptyPos.push_back(EmptyPos), _reactionPos.push_back(reactionPos), _physicalEntityPos.push_back(physicalEntityPos),  _ANodePos.push_back(ANodePos);
		  _compartmentPos.push_back(compartmentPos);
		  _Scale.push_back(scale);
		  _dCenter.push_back(dCenter);		  
		  drawSmallmolecule.push_back(drawSmallmoleculeSettings);
	  }
	  else if(m_pathwayID<_complexPos.size())
	  {
	      _complexPos[m_pathwayID]=complexPos, _proteinPos[m_pathwayID]=proteinPos, _smallMoleculePos[m_pathwayID]=smallMoleculePos, _DnaPos[m_pathwayID]=DnaPos,  _EmptyPos[m_pathwayID]=EmptyPos, _reactionPos[m_pathwayID]=reactionPos, _physicalEntityPos[m_pathwayID]=physicalEntityPos, _ANodePos[m_pathwayID]=ANodePos;
		  _compartmentPos[m_pathwayID]=compartmentPos;
		  _Scale[m_pathwayID]=scale;
		  _dCenter[m_pathwayID]=dCenter;
		 
		  drawSmallmolecule[m_pathwayID]=drawSmallmoleculeSettings;	  
	  }
	  else
		  return;
}


void OpenGLScene::resetItemPos(vector<QRectF> &pos1, vector<QRectF> pos2)
{
	for(int i=0; i<pos1.size() && i<pos2.size(); i++)
	{
	    if(pos1[i].x()>-990.99)
			pos1[i]=pos2[i];	
	}
}

void OpenGLScene::resetItemPos_1(vector<QRectF> &pos1, vector<QRectF> pos2)
{
	for(int i=0; i<pos1.size() && i<pos2.size(); i++)
	{
	    if(pos1[i].x()<-990.99 && pos2[i].x()>-900.99)
			pos1[i]=pos2[i];	
	}
}

void OpenGLScene::resetItemPos_afterReadPathway(int m_pathwayID, vector<QRectF> complexPos, vector<QRectF> proteinPos, vector<QRectF> smallMoleculePos, vector<QRectF> DnaPos, vector<QRectF> EmptyPos, vector<QRectF> reactionPos, vector<QRectF> physicalEntityPos,vector<QRectF> compartmentPos, vector<QRectF> ANodePos)
{	
	//complexPos=_complexPos[m_pathwayID], proteinPos=_proteinPos[m_pathwayID], smallMoleculePos=_smallMoleculePos[m_pathwayID], DnaPos=_DnaPos[m_pathwayID], reactionPos=_EmptyPos[m_pathwayID], reactionPos=_reactionPos[m_pathwayID], physicalEntityPos=_physicalEntityPos[m_pathwayID], edge=_edge[m_pathwayID];
	resetItemPos_1(_complexPos[m_pathwayID], complexPos);
	resetItemPos_1(_proteinPos[m_pathwayID], proteinPos);
	resetItemPos_1(_smallMoleculePos[m_pathwayID], smallMoleculePos);
	resetItemPos_1(_DnaPos[m_pathwayID], DnaPos);
	resetItemPos_1(_EmptyPos[m_pathwayID], EmptyPos);
	resetItemPos_1(_physicalEntityPos[m_pathwayID], physicalEntityPos);
	resetItemPos_1(_ANodePos[m_pathwayID], ANodePos);
	resetItemPos_1(_reactionPos[m_pathwayID], reactionPos);	
	resetItemPos_1(_compartmentPos[m_pathwayID], compartmentPos);	
}

void OpenGLScene::bubbleDeleteItself(ItemBase *item)
{
	item->deleteSelectedItems();
}

void OpenGLScene::resetItemPos(int m_pathwayID, vector<QRectF> &complexPos, vector<QRectF> &proteinPos, vector<QRectF> &smallMoleculePos, vector<QRectF> &DnaPos, vector<QRectF> &EmptyPos, vector<QRectF> &reactionPos, vector<QRectF> &physicalEntityPos,vector<QRectF> &compartmentPos, vector<QRectF> &ANodePos,vector<vector<int>> &edge, float &scale, QPointF &dCenter, bool &drawSmallmoleculeSettings)
{	
	//complexPos=_complexPos[m_pathwayID], proteinPos=_proteinPos[m_pathwayID], smallMoleculePos=_smallMoleculePos[m_pathwayID], DnaPos=_DnaPos[m_pathwayID], reactionPos=_EmptyPos[m_pathwayID], reactionPos=_reactionPos[m_pathwayID], physicalEntityPos=_physicalEntityPos[m_pathwayID], edge=_edge[m_pathwayID];
	resetItemPos(complexPos, _complexPos[m_pathwayID]);
	resetItemPos(proteinPos, _proteinPos[m_pathwayID]);
	resetItemPos(smallMoleculePos, _smallMoleculePos[m_pathwayID]);
	resetItemPos(DnaPos, _DnaPos[m_pathwayID]);
	resetItemPos(EmptyPos, _EmptyPos[m_pathwayID]);
	resetItemPos(physicalEntityPos, _physicalEntityPos[m_pathwayID]);
	resetItemPos(ANodePos, _ANodePos[m_pathwayID]);
	resetItemPos(reactionPos, _reactionPos[m_pathwayID]);
	
	//compartmentPos=_compartmentPos[m_pathwayID];
	resetItemPos(compartmentPos, _compartmentPos[m_pathwayID]);	
 	
	scale=_Scale[m_pathwayID], dCenter=_dCenter[m_pathwayID];	
	//_Rank0=Rank0[m_pathwayID]; //for compartment
    //_Ranks=Ranks[m_pathwayID];
	//_newPos=newPos[m_pathwayID];
	//_newCompartmentPos=newCompartmentPos[m_pathwayID];
	//drawSmallmoleculeSettings=drawSmallmolecule[m_pathwayID];
}


	