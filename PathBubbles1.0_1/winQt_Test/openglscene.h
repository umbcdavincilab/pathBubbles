#ifndef OPENGLSCENE_H
#define OPENGLSCENE_H
//#include "PieMenuItem.h"
#include "myGraph.h"
#include "point3d.h"
#include <QGraphicsScene>
#include <QLabel>
#include <QTime>
#include <QGraphicsProxyWidget>
#include <QtGui>
#include <QtOpenGL>
#include <QFileDialog>
#include "SM.h"
#include "LabCut.h"
#include "LabObject.h"
#include "qtsm.h"
#include "OrderManager.h"
//#include "qtbat.h"
#include "ControlManager.h"
//#include "AllBats.h"
//#include "qtLCom.h"
#include "GroupManager.h"
#include "SearchFind.h"
//#include "matBlock.h"
#include "NoteBlock.h"
#include "webBlock.h"
#include "VideoBlock.h"
//#include "PenBlock.h"
#include "VolumBlock.h"
#include "ControlBlock.h"
#include "NavigationBlock.h"
#include "OpenManager.h"
#include "FileManager.h"
#include "SymbolColorPanel.h"
#include "fileSelection.h"
#include "StatisticManager.h"
#include "StatisticBar.h"
#include "ArrowGen.h"
#include "BatContext.h"

#include "CalloutNote.h"
#include "textBubble2.h"
#include "expressionBubble.h"
#include "ReactionBubble3.h"
#include "GroupBubble4.h"
#include "PathBubble1.h"
#include "PathBubble0.h"
#include "treeRingBubble.h"

#include "pathwayDataParser.h"

#ifndef QT_NO_CONCURRENT
#include <QFutureWatcher>
#endif

#ifndef OUT
#define OUT 1
#define HIDDEN 2
#endif

class Model;
class myGraph;
class TextBubble2;
class GroupBubble4;
class PathBubble1;
class PathBubble0;
class ReactionBubble3;
class expressionBubble;
class GroupManager;
class PWDParser;
class TreeRing;
class BundlesPrimitive;
class SmallTreeRing;
class Diagram;
class treeRingBubble;


class OpenGLScene : public QGraphicsScene
{
	Q_OBJECT

public:
	int _colorID;
	bool controlKeyisOn;
	QString userName;
	bool fileSelectionOpened;
	bool menuloaded;
	QMenu menu;
	QAction *pathwayAction, *loadExpressionAction,*webAction,*loadNoteAction,*loadTreeRingAction;
	QMenu *loadMenu;
	vector<int> menuInputNote;
	bool needToReOpenMenu;
	int openCount;
	
	myGraph *_wholeGraph;
	//vector<vector<int>> GRAPH; //keep an overgraph for all pathways
	//int totalNode, totalEdge;	

	void updateGraphs();
	QColor getPathColor(int pid1, int type1, int id1, int pid2, int type2, int id2);
	//a structure recording path colors (major path, non-major path use different color)
	vector < vector < set < vector< vector<int> > > > > pathVectors; //pathVectors[0] record major pathes


	void setDrawCrossTalk(bool flag);
    void setDrawRateLimit(bool flag);
	void setDrawOrthology(bool flag);

	//void loadMenus(QGraphicsSceneContextMenuEvent * contextMenuEvent, ItemBase *item, vector<int> node);
	void loadMenus(QPointF eventScenePos, QPoint eventScreenPos, ItemBase *item, vector<int> node);
	//deque< vector< vector<int> > >::iterator currentEventit;
	//deque< vector<vector< vector<int> > > > eventStored;

	void setTopItem(ItemBase *item); //set the item's z value to be highest and set the rest the same, except the callout bubble which is always on the topest
	fileSelection* _fileSelection;
	bool undoExpresssionBubble(int eid, int eventType);
	bool redoExpresssionBubble(int eid, int eventType);
	bool undoPathbubble(int pid, int eventType);
	bool redoPathbubble(int pid, int eventType);
	void storeEvent(vector< vector<int> > eventRecord);	
	void redo(vector< vector<int> > redoRecord);
	void undo(vector< vector<int> > redoRecord);

	float graphReferenceSize;
	ItemBase *findAPathBubbleinGroup(ItemBase *init);
	ItemBase *findAExpressionBubbleinGroup(ItemBase *init);
	ItemBase *findATypeBubbleinGroup(ItemBase *init, int Type);

	void dropDownMenu0(QGraphicsSceneContextMenuEvent * contextMenuEvent, ItemBase *item, vector<int> node);
	void removeToolsinGroup(ItemBase * item, bool flag);
	void clearSearchSelection(ItemBase * item);
	
	ItemBase* openTreeRingBubblebyMenu(bool setPos=false, bool rightSide=false);
	ItemBase* openAutoTreeRingBubble(float x, float y, bool rightSide);
	
	void deleteGroup(ItemBase *item);
	void setSearchType(int type);
	void setDrawSmallMolecule(bool flag);

	bool searchShared, searchDiffered, searchLinked, searchExpressed;	
	int linkSearchType; //0: search all pathes from a start, 1: search pathes for two nodes
	vector<treeRingBubble *> m_treeRingBubbles;

	//PieMenuItem* _pieMenu;
	//bool _pieMenuOn;
	//int _pieMenuState;	
	void clearSelected(ItemBase *item);
	void clearUndoRedoRecord(ItemBase *item);	
	void clearColored(ItemBase *item);
	void clearExpressed(ItemBase *item); //while updata graph
	void clearSharedSets(ItemBase *item);
	void clearExpressedSets(ItemBase *item);

	vector<ItemBase *>  extractTypeBubblesinGroup(ItemBase * item, int Type);
	vector<PathBubble1 *>  extractPathBubblesinGroup(ItemBase * item);
	
	PathBubble1 *findAPathBubbleinGroup(Group *B);	
	ItemBase *findATypeBubbleinGroup(Group *B, int type);
	PathBubble1 *findASmallestPathinGroup(ItemBase *init);
	vector<expressionBubble *>  extractExpressionBubblesinGroup(ItemBase * item);
	//void clearSharedSets(ItemBase *item);
	void openPathBubblebyMenu(QString name, QString pname, QString lastStoredGraph, ItemBase *item, QPointF scenePos, bool setPos=false, bool rightSide=false);
	void openPathBubblebyMenu(QString name, QString pname, QString lastStoredGraph, bool setPos=false, bool rightSide=false);
	
	ItemBase *openWebNoteBubblebyMenu(int type, ItemBase *item, QPointF scenePos, int pid=-1, int ptype=-1, int id=-1);
	QList<ItemBase*> allMembers();

	QPoint getGroupedBubblePosition(ItemBase *newItem, ItemBase *parent, int x, int y, int width, int height, bool setPos=false);
	QPoint getSonPosition(PathBubble1 *newItem, PathBubble1 *parent, int x, int y, int width, int height);		
	QPoint getSonPosition(TextBubble2 *newItem, ItemBase *parent, int x, int y, int width, int height);
	//QPoint getSonPosition(treeRingBubble *newItem, treeRingBubble *parent, int x, int y, int width, int height);
	QPoint getSonPositionforTreeRing(ItemBase *newItem, ItemBase *parent, int x, int y, int width, int height);
	
    ItemBase *menuSelectedItem;
	bool drawSmallmoleculeSettings,drawRateLimit, drawCrossTalk, drawOrthology;
	
	char searchType;
	vector<bool> drawSmallmolecule;
	bool drawDna;
	int findTopBubble(int pid, set<int> &passed);
	int findTopBubble(int pid);

	QPointF pos_menu, scenePos_menu;	

	PWDParser *pwdParser;
	//TreeRing *_treeRing;
	//BundlesPrimitive  *_bun;
	vector<SmallTreeRing *> _small_tree_ring;
    vector<Diagram *> _diagram;

	set<QString> crossTalkSymbol;
	set<QString> rateLimitSymbol;
	set<QString> orthologySymbol;
	
	QList<expressionBubble*> _expressionBubbles;
	QList<PathBubble1*> _pathBubbles;
	QList< TextBubble2* > _codeBubbles;
	QList< ReactionBubble3* > _reactionBubbles;
	QList< GroupBubble4* > _groupBubbles;
	QList <CalloutNote*> _nodeNote;

	set<vector<vector<int>>> edgeconnections;
	set<vector<vector<int>>> edgeconnections_A; //store the activation edge (which is also in edgeconnections)
	set<vector<vector<int>>> edgeconnections_I; //store the inhibition edge (which is also in edgeconnections)
	set<vector<vector<int>>> edgeconnections_4; //store the overlapped edge (which is also in edgeconnections)
	vector<int> edgeconnections_5;
	vector<int> hoveredItem;
	
   // set< vector<int> > foundlist;
	
	vector< QString > expressedGeneNames;
	vector<vector<vector<int>>> _complexContain;
	vector<vector<vector<int>>> _ANodeContain;
	vector<vector<vector<QRectF>>> _ANodePosContain;
	//vector<vector<vector<QString>>> _complexName; //start from 1
	//vector<vector<vector<QString>>> _physicalEntityName; //start from 1
	//vector<vector<vector<QString>>> _proteinName, _reactionName,_degradationName;	//start from 1
	//vector<vector<vector<QString>>> _smallMoleculeName, _EmptyName, _DnaName;  //start from 1

	vector<vector<vector<vector<int>>>> _biochemicalReaction; //start from 1
	vector<vector<vector<vector<int>>>> _degradation;
	vector<vector<set<vector<int>>>> CompartmentContain; //start from 0
	vector<vector<vector<QString>>> _compartmentName; //start from 0
	vector<vector<vector<int>>> Rank0; //for compartment
	vector<vector<vector<vector<int>>>> Ranks;
	//vector<vector<QRectF>> newPos, newCompartmentPos;
	
	vector<vector<vector<QString>>> _ANodeName; //start from 1
	vector<vector<vector<QString>>> _complexNameD; //start from 1
	vector<vector<vector<QString>>> _physicalEntityNameD; //start from 1
	vector<vector<vector<QString>>> _proteinNameD, _reactionNameD,_degradationNameD;	//start from 1
	vector<vector<vector<QString>>> _smallMoleculeNameD, _EmptyNameD, _DnaNameD;  //start from 1	

	
	vector<QString> _pathwayNameD;
	//for expression
	vector<vector<QString>> _quantityName;
	vector<vector<vector<float>>> _quantity;
	vector<vector<vector<float>>> _minmax;
	vector<vector<vector<QString>>> _geneInfo; //gene_id	symbol

	OpenGLScene( LabObject* input2, LabCut* input3 );
	OpenGLScene::~OpenGLScene();

	void drawBackground(QPainter *painter, const QRectF &rect); //QGraphicsScene
	void drawForeground(QPainter *painter, const QRectF &rect);
	void itemMoved();
	//bool PenisOn;
	vector<QRect> preOpenRects;
	bool CheckANodeIDinAllBubbles(QString AName);
	bool m_rightButton;

	vector<vector<QRectF>> _complexPos, _proteinPos, _smallMoleculePos, _DnaPos, _degenerationPos, _reactionPos, _physicalEntityPos, _compartmentPos, _EmptyPos, _ANodePos;	
	vector<float> _Scale; 
	vector<QPointF> _dCenter;
	vector<vector<vector<int>>> _edge;
	ItemBase *delectionStarter; 
	bool bubbleDraggedout;
	QPointF lastPressedPos;
	
	void backupEdge(int m_pathwayID, vector<vector<int>> edge);
	void backupItemPos(int m_pathwayID, vector<QRectF> complexPos, vector<QRectF> proteinPos, vector<QRectF> smallMoleculePos, vector<QRectF> DnaPos, vector<QRectF> EmptyPos, vector<QRectF> reactionPos, vector<QRectF> physicalEntityPos, vector<QRectF> compartmentPos, vector<QRectF> ANodePos, float scale, QPointF dCenter, bool drawSmallmoleculeSettings);
	void resetItemPos(int m_pathwayID, vector<QRectF> &complexPos, vector<QRectF> &proteinPos, vector<QRectF> &smallMoleculePos, vector<QRectF> &DnaPos, vector<QRectF> &EmptyPos, vector<QRectF> &reactionPos, vector<QRectF> &physicalEntityPos,vector<QRectF> &compartmentPos, vector<QRectF> &ANodePos, vector<vector<int>> &edge, float &scale, QPointF &dCenter, bool &drawSmallmolecule);
	void resetItemPos(vector<QRectF> &pos1, vector<QRectF> pos2);

	void resetItemPos_1(vector<QRectF> &pos1, vector<QRectF> pos2);
    void resetItemPos_afterReadPathway(int m_pathwayID, vector<QRectF> complexPos, vector<QRectF> proteinPos, vector<QRectF> smallMoleculePos, vector<QRectF> DnaPos, vector<QRectF> EmptyPos, vector<QRectF> reactionPos, vector<QRectF> physicalEntityPos,vector<QRectF> compartmentPos, vector<QRectF> ANodePos);


	CalloutNote* findNote(int pid, int type, int id);	
	vector<QString> PathWayNames;

	QWidget* getDialog( )
	{
		return this->createDialog( tr("Bat Controls") );
	}
	void myAddItem(  ItemBase * item )
	{
		this->addItem( item );
		this->_manager->newItemAdded( item );
	}
	QList<ItemBase *> getGroupMembers(  ItemBase * item )
	{
		return this->_manager->getGroupMembers( item );
	}

	Group * getGroup(  ItemBase * item )
	{
		return this->_manager->getGroup( item );
	}

	LabObject* getLab() 
	{ 
		return lab; 
	}
	LabCut* getCut()
	{
		return cut; 
	}
	GroupManager* getGroupManager()
	{
		return this->_manager;
	}
	OpenManager* getOpenManager()
	{
		return _openManager;
	}
	StatisticBar* getStatisticBar()
	{
		return _staBar;
	}
	StatisticManager* getStaManager()
	{
		return _statistic;
	}
	bool changePlotAttribute( QPointF pos, QString name );
	
	void bubbleDeleted( ItemBase* init, QRectF parentRect=QRectF(-10000,-10000,-10000,-10000))
	{
	
	    _manager->bubbleDeleted(init, parentRect);
	}

	//void addLComObjectFromXmlNode( QDomElement node );

	QTime getCurTime()
	{
		return m_time.currentTime( );
	}
	void addLogText( QString str)
	{
		logStr.append( str );
	}

	void logOperation( QString str );
	//void logPasteMatBubble( QPointF pos );
	bool naviContains(QPointF pos);
	void openNodeNote(int pid, int type, int id, ItemBase *item, QPointF pos, vector<QString> text, vector<int> Cid, bool setVisible);

public slots:
	//void webBubbleResized(ItemBase *item);
	void bubbleDeleteItself(ItemBase *item);
	void openFileSelection();
	void closeFileSelection();
	void logInfor( QString str );
	void setWingShapePara(ArrowGen* aGen, QPointF pos);
	void needStatisticalData( QPointF pos, StatisticManager* manager, int index );
	void moveButtonState( bool );
	void updateCoverage( float );
	void hideWebView();
	void setBackgroundColor();
	void orderViews();
	void floatingButtonPushed();
	void index_recieved( int index );
	void shiftViewport( QPoint offset );
	//void addCompare( QtBat* object );
	void openAllPCs();
	void mapViews();
	void openCBRY()
	{
		_BATSPECIES = 1;
	}
	void openCBOR()
	{ 
		_BATSPECIES = 2;
	}
	void openCBDG()
	{ 
		_BATSPECIES = 3;
	}
	void openCBMA()
	{ 
		_BATSPECIES = 4;
	}
	void openCBYR()
	{ 
		_BATSPECIES = 5;
	}
	void openPCM1()
	{ 
		_PCINDEX = 1;
	}
	void openPCM2()
	{ 
		_PCINDEX = 2;
	}
	void openPCM3()
	{ 
		_PCINDEX = 3;
	}

	int searchInPathBubble(QString str, set<int> searchList, QString name = "", ItemBase *item = NULL);
	int searchInExpressionBubble(QString str, set<int> searchList, QString name, ItemBase *item);
	int searchInExpressionBubble(QString str, QString name, vector<set<vector<int>>> &preSearched, int i, ItemBase *item, int matchType);

	int searchPathwayInTreeRingforASymbol(QString str, set<int> searchList, QString name, ItemBase *item);
	int searchInTreeringBubble(QString str, set<int> searchList, QString name, ItemBase *item);
	int searchInTreeringBubble(QString str, QString name, vector<set<vector<int>>> &preSearched, int i, ItemBase *item, int matchType);

	void redrawPathWay(void);
	//void setCurrentBatAttribute( QtBat *object );
	//void getNewBatAttribute( QtBat *object );
	//void setMatFileBubble( MatBlock* currentMat );
	int getBubbleIndex() { return bubbleIndex++; }
	
	ItemBase *findBubbleUnderNote( QPointF eventPos );	
	ItemBase *findBubbleUnderAlocation( QPointF eventPos );
	
	bool delAChildPathBubble(PathBubble1* pBubble, bool ungroup=true);
	bool del_merg_PathBubble(PathBubble1* eBubble, PathBubble1* pBubble, bool Ungroup);
signals:
	void findEnclosed(PathBubble1* b0, PathBubble1* tmp);
	void item_clicked( int flag );
	void prePathBubbleStarted(QRect rect);
	void pathBubbleStarted(QRect rect);
	void pleaseReopenMenu();	
public slots:
		void appHasClosed();
		void showPreOpentRect(QRect rect);
		void showPathBubble(QRect rect);
		void reopenMenu();	

protected:	
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void wheelEvent(QGraphicsSceneWheelEvent * wheelEvent);
	void contextMenuEvent( QGraphicsSceneContextMenuEvent * contextMenuEvent );
	void resizeEvent (QResizeEvent *event );
	void springEvent();
	void keyPressEvent(QKeyEvent *event);
	void keyReleaseEvent(QKeyEvent *event);

private:
	
	
	QPointF menuPos;
	QPoint menuScreenPos;
	bool loadMenuOpened;
	
	//QMenu menu;
	
	bool isBubbleinCurrentWindow(ItemBase *B);
	bool isGroupinCurrentWindow(Group *B);
	void buildMatBubbleConnect( QDomElement node );
	void buildSubGraphBubbleConnect( QDomElement node );
	void initFloatingControl();
	void findSelected();
		
	//void addLComObject( QtBat* left, QtBat* right );
	//void findCompareTarget( QPointF pos );
	void removeAllItems();
	QDialog *createDialog(const QString &windowTitle) const;
	void setModel(Model *model);
	void recordSceneAsXmlFile( QString filename );
	void recoverSceneFromXmlFile( QString fileName );
	void writeLogFile();

	bool m_wireframeEnabled;
	bool m_normalsEnabled;
	bool _MOVEFLOATING;

	QColor m_modelColor;
	QColor m_backgroundColor;

	QTime m_time;
	QTimer timer;
	int m_lastTime;
	int m_mouseEventTime;

	float m_distance;
	Point3d m_rotation;
	Point3d m_angularMomentum;
	Point3d m_accumulatedMomentum;

	QCheckBox* surface;
	QCheckBox* wireframe;
	QLabel *m_labels[4];
	QWidget *floatControls;
	QSizeF _iniFloatingControlsSize;
	LabObject *lab;
	LabCut *cut;
	int STATE;
	int SHIFT;
	QWidget* Controls;
	animateIt* ani;
	QLCDNumber* lcdNumber;
	QSlider* slider;
	OrderManager* order;
	int timerId;
	QPixmap menuImage0,menuImage1, pathwayMenuImage;
	//reserved items in scene
	//QtBat* _batAttached;
	bool _BATATTACHED;

	//
	bool _wantsCompare;
	//QtBat* batWantsCompare;

	//AllBats* allbats;
	std::string _perfix;
	//			specified open state
	int _BATSPECIES;
	int _PCINDEX;

	//			group manager
	GroupManager* _manager;

	//
	QGraphicsProxyWidget* _webView;
	bool _WEB;
	bool _MOVEBUTTON;
	//
	QPointF _TopLeft;
	//
	//bool _ONDRAG;
	//
	OpenManager* _openManager;
	//
	NavigationBlock* _navi;
	//
	FileManager* _file;
	SymbolColorPanel* _colorPanel;
	
	//
	ArrowGen* _arrow;
	//
	void expendRect( QPointF pos );
	void drawSelecting( QPainter* painter );
	bool _MultiSelect;
	QPointF _initSelectPoint;
	QRectF _selectRect;
	//
	StatisticManager* _statistic;
	StatisticBar* _staBar;

	BatContext batAttribute;
	bool hasBatAttribute;

	//MatBlock* matCurrent;
	bool hasMatBubble;

	bool recordingScene;

	QFile logFile;
	QTextStream log_output;
	QString logStr;

	int bubbleIndex;

public:
	//int rightClickFlag; //0: contextmenu on, 1: in treeRing, 2: treering rightclick
	void addToGroup( ItemBase * target, ItemBase * init, bool updatePos=true)
	{
		QList<ItemBase *> list;
		list.push_back(target);
	   _manager->addToGroup(target, init, updatePos);
	}

#ifndef QT_NO_CONCURRENT
	QFutureWatcher<Model *> m_modelLoader;

#endif
};


#endif // OPENGLSCENE_H

/*
customize ways to open context menu
myWidget->setContextMenuPolicy(Qt::CustomContextMenu);
connect(myWidget, SIGNAL(customContextMenuRequested(const QPoint&)),
    this, SLOT(ShowContextMenu(const QPoint&)));
Next, note that the const QPoint& pos parameter in the customContextMenuRequested() signal is normally in widget coordinates. However, also note that classes which inherit from QAbstractScrollArea1 instead use the coordinates of their viewport(). Either way, you will need to map these coordinates to global coordinates using mapToGlobal().

Finally, simply either popup() or exec() your QMenu. Remember that popup() is non-blocking, so if you wish to use that, make sure your QMenu is created on the heap (or some other way of guaranteeing that the QMenu's lifetime outlasts the scope of the function)!

void MyClass::ShowContextMenu(const QPoint& pos) // this is a slot
{
    // for most widgets
    QPoint globalPos = myWidget->mapToGlobal(pos);
    // for QAbstractScrollArea and derived classes you would use:
    // QPoint globalPos = myWidget->viewport()->mapToGlobal(pos);

    QMenu myMenu;
    myMenu.addAction("Menu Item 1");
    // ...

    QAction* selectedItem = myMenu.exec(globalPos);
    if (selectedItem)
    {
        // something was chosen, do stuff
    }
    else
    {
        // nothing was chosen
    }
}*/

/*
//image:  for good qulity do not set sourcerect targetrect which may disform the image
use drawPixmap instead of drawImage

//painter->setRenderHints(QPainter::SmoothPixmapTransform, true);
	QRectF sourceRect=QRectF( QPointF( 0, 0 ), menuImage0.size() );
	QRectF targetRect=QRectF( QPointF( 0, 0 ), menuImage0.size() );
	targetRect=sourceRect;
	targetRect.moveCenter(QPointF(100,100));
	painter->drawPixmap( targetRect.center(), menuImage0); //good quality
	painter->drawPixmap( sourceRect, menuImage0, targetRect);  //bad quality
	painter->drawImage( sourceRect.center(), menuImage1); //bad quality
//
*/