#ifndef QTBOX_H
#define QTBOX_H

#include <QtGui>
#include "3DHelper.h"
#include <QtGui/qvector3d.h>
#include <exception>
#include <QList>
#include <QtXml>
#include <QFontMetrics>
#include <QTextStream>
#include "glbuffers.h"
#include "TextNote.h"
#include "ItemLabel.h"
#include <sstream>
#include <QString>
#include <set>
#include <deque>

/*
#include <boost/config.hpp>  
#include <boost/graph/adjacency_list.hpp>  
#include <boost/tuple/tuple.hpp>
#include <boost/graph/fruchterman_reingold.hpp>
#include <boost/graph/random_layout.hpp>
#include <boost/graph/kamada_kawai_spring_layout.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topology.hpp>
#include <boost/lexical_cast.hpp>
#include <QString>
#include <iostream>
#include <map>
#include <vector>
#include <boost/random/linear_congruential.hpp>
#include <boost/progress.hpp>
#include <boost/shared_ptr.hpp>

using namespace boost;
*/

const int MAX_ITEM_SIZE = 1200;
const int MIN_ITEM_SIZE = 50;
const int MATSIZE = 260;
const int BOARDERSIZE = 6;
const int HALOWIDTH = 16;

#define GO_UP		1
#define _DOWN	-1
#define _EVEN	0
#define BREWERCOLOR1  2  //light
#define BREWERCOLOR2  12  //dark
#define SATURATECOLOR  22  //6 color, red, blue, green, yellow, purple, and light blue

#define LIGHT_HIGHLIGHTCOLOR QColor(255,213,170,159)// QColor(220, 230, 255, 159) 
#define MIDIUM_HIGHLIGHTCOLOR QColor(255,128,0,255) //QColor(20,20,255,255)
#define PRIME_PATH_COLOR   QColor(254,153,41,127)

#define RATELIMITCOLOR  QColor(255, 160, 160,255)
#define CROSSTALKCOLOR  QColor(160, 160, 255, 255)
#define ORTHOLOGYCOLOR  QColor(102,194,165, 255)
#define DOWNEXPRESSIONCOLOR QColor(64,64,255,255)
#define UPEXPRESSIONCOLOR QColor(230,68,113,255)
 
#define PROTEINCOLOR QColor(255,255,204,255) 
#define COMPLEXCOLOR QColor(255,226,183,255) 	
#define SMALLMOLECULECOLOR QColor(214,215,202,255)
#define PHYSICALENTITYCOLOR QColor(230,234,172,255)
#define DNACOLOR QColor(214,234,172,255)
#define SETCOLOR QColor(254,202,131,255)
#define REACTIONCOLOR QColor(255,255,255,255)

#define ACTIVATIONCOLOR QColor(0, 210, 50, 255)
#define INHIBITIONCOLOR QColor(255, 128, 0, 255)
#define BUBBLELINKCOLOR QColor(40,40,40,200)

#define HIGHLIGHTCOLOR QColor(255,213,170,159) //QColor(120, 130, 255, 127) //for shared, linked, differed

//	type of object
#define BAT		1
#define PC		2
#define COMPLOT	3
#define MATLAB	4
#define WEB		5
#define COLOR	6
#define WAKE	7
#define VTK		8
#define VIDEO	9
#define NOTE	10
#define PEN		11
#define VOLUM	12
#define CON		13
#define IMAGE	14
#define DATAPLOT 15
#define BARCHART 17
#define LINECHART 18
#define SCATTER 19
#define PATHBUBBLE0 20
#define PATHBUBBLE1 21
#define PATHBUBBLE2 22
#define TEXTBUBBLE2 23
#define REACTIONBUBBLE3 24
#define GROUPBUBBLE4 25
#define EXPRESSIONBUBBLE 26
#define SUBPATHBUBBLE1 27
#define SEARCHFIND 28 
#define SETTINGS 29
#define SEARCHSETTINGS 30
#define SEARCHSETTINGS2 31
#define CALLOUT	32
#define TREERING 33
#define EGIFT 34 

#define BUBBLECREATED 0
#define BUBBLEDELETED 1
#define BUBBLERESIZED 2
#define BUBBLEGROUPED 3
#define BUBBLEUNGROUPED 4
#define GROUPOVERLAID 5
#define BUBBLESHIFTED 6
#define SUBBUBBLECREATED 7
#define SUBTREERINGCREATED 8

//eventtype
#define GRAPHCHANGE 0
#define SEARCHSHARED 1
#define SEARCHDIFFERED 2
#define SEARCHLINKED 3
#define SEARCHEXPRESSED 4
#define COLORED 5
#define NEWGRAPH 6
#define HIGHLIGHTED 7

#define HBWIDTH 8 //for the width of highlighted background
//#define BREWERCOLOR1 0;
//#define BREWERCOLOR2 10;


/*typedef boost::rectangle_topology<> topology_type;
typedef topology_type::point_type point_type;

typedef adjacency_list<listS, vecS, undirectedS, 
                       property<vertex_name_t, std::string> > Graph;

typedef graph_traits<Graph>::vertex_descriptor Vertex;

typedef std::map<std::string, Vertex> NameToVertex;


class progress_cooling : public linear_cooling<double>
{
  typedef linear_cooling<double> inherited;

 public:
  explicit progress_cooling(std::size_t iterations) : inherited(iterations) 
  {
    display.reset(new progress_display(iterations + 1, std::cerr));
  }

  double operator()()
  {
    ++(*display);
    return inherited::operator()();
  }

  private:
  boost::shared_ptr<boost::progress_display> display;
};*/

class ItemBase :  public QObject, public QGraphicsItem
{
	Q_OBJECT
		Q_INTERFACES( QGraphicsItem )

public:
	bool deleted;
	TextNote* _note;
	bool AFTERUNGROUP;
	bool _penisOn;
	bool _upisOn,_evenisOn,_downisOn;
	bool _grabisOn;
	bool _queryisOn;
	bool groupSelected;
	OpenGLScene* _scene;	
	QTextEdit* m_note;

	float initWidth;
	void drawArrow_4(QPainter *painter, QPointF start, QPointF end, float width, float height, QColor color); // doted line
    void drawArrow_4(QPainter *painter, QPointF start, float c, QPointF end, float width, float height, QColor color);

   //vector<PathBubble1 *>  extractPathBubblesinGroup(ItemBase * item);
	//bool onBoarder( QPointF pos );
	bool insideBubble( QPointF pos );
	bool outsideBubble( QPointF pos );
	bool onTagArea( QPointF pos );
	bool onUnGroupArea( QPointF pos );
	void drawAngleText(QPainter *painter, int x, int y, QString qstr, float angle);
	void drawAngleRect(QPainter *painter, QRectF rect, float angle);

	bool resizeFinished;

	void deleteSelectedItems(QGraphicsScene *scene);
	void deleteSelectedItems();
	enum { Type = UserType + 1 };
	int screenWidth, screenHeight;
	ItemBase(int size, int x, int y);
	void finishInit();	
	void unGroup()
	{
	   unGroup(this);
	   logUnGroup();
	}
	bool isDelegated()
	{
		return this->DELEGATE;
	}
	void setDelegate( bool bit )
	{
		this->DELEGATE = bit;
	}
	void hightLight( myColor3 hiColor )
	{
		this->_highLight = true;
		this->_hiColor = hiColor;
	}
	void unHighLight()
	{
		//if(this->_highLight)
		//	this->_highLight=this->_highLight;
		this->_highLight = false;
	}
	float Width()
	{
		return m_size_width;
	}
	float Height()
	{
		return m_size_height;
	}
	QPointF getInitPos()
	{
		return _initPos;
	}
	void properRemove()
	{
		emit itemRemoved( this );
	}
	myColor3 getColor()
	{
		if(this->_color.a>=0)
		   return this->_color;
		else return this->_colorBoarder;
	}
	myColor3 getBoarderColor()
	{
		return this->_colorBoarder;
	}
	int getAlpha()
	{
		return this->_colorBoarderAlpha;
	}
	void setHiColor( myColor3 input )
	{
		this->_hiColor = input;		
	}

	void setColor( myColor3 input )
	{
		this->_colorBoarder = input;
		if (this->_label)
		{
			this->_label->setLabelColor( input );
		}
	}
	int getInitSize()
	{
		return _initSize;
	}
	bool needRemoveControl()
	{
		return this->_REMOVECONTROL;
	}
	bool canBeShift()
	{
		return this->_CANBESHIFTED;
	}
	bool isMoving()
	{
	   return m_isMoving;
	}	
	QString getPathID(QString pathName);	

	void getLinkPoint(QPointF start, QPointF &end, QPointF dis, QRectF startRect, QRectF endRect);
	void addToGroup( ItemBase* target, bool updatePos=false);
	virtual ~ItemBase();
	virtual void calculateForces();
	virtual void resizeItem( int size1, int size2 );
	virtual void resizeItem_1( QPointF pos, QPointF lastPos, int cornerID );
	virtual bool collision( ItemBase* node );
	virtual bool tighten(QList<ItemBase*> groupMember);
	virtual bool disperse(QList<ItemBase*> groupMember);
	virtual bool needtoTighten( ItemBase* node);
	virtual bool advance();
	virtual bool isbridgeBroken(QList<ItemBase*> groupMember);
	virtual void initializeGL();
	void resetOriRect();
	void scaleUpBasedonOriRect();
	void scaleDownBasedonOriRect();

	QRectF transRect(QRectF rect, float scale, QPointF dCenter, int width, int height);	
	QSizeF getFontSize(QString qstr, int fontsize);
	QString _name;
	set<vector<int>> highlighted,searched;
	set<int> rateLimit, crossTalk, orthology;
	bool drawCrossTalk, drawRateLimit, drawOrthology;
	
	int resizeMode; //1: enlarged uniformly while dragging corner. 2: extend the corner only

	vector< set<int> > sharedExpressionProtein;
	set<int> expressedComplex_Edit, expressedDna_Edit, expressedPhysicalEntity_Edit, expressedANode_Edit, expressedProtein_Edit, expressedSmallMolecule_Edit, expressedReaction_Edit;   
	vector< set<int> > expressedProtein, expressedComplex, expressedSmallMolecule, expressedDna, expressedReaction, expressedPhysicalEntity, expressedCompartment, expressedPathway, expressedANode;	
	vector< set<int> > sharedProtein, sharedComplex, sharedSmallMolecule, sharedDna, sharedReaction, sharedPhysicalEntity, sharedCompartment, sharedPathway, sharedANode;
	vector< set<int> > linkedProtein, linkedComplex, linkedSmallMolecule, linkedDna, linkedReaction, linkedPhysicalEntity, linkedCompartment, linkedPathway, linkedANode;
	vector< set< vector< vector<int> > > > linkedEdge; //linkedEdgebetween pathways
	vector< set<int> > differedProtein, differedComplex, differedSmallMolecule, differedDna, differedReaction, differedPhysicalEntity, differedCompartment, differedPathway, differedANode;
	set<vector<int>> _itemColored;
	set< vector<int> > foundlist;
	//set<int> _linkage;
	
	set<vector<vector<int> > > highlightedEdge;
	vector<int> hoveredItem;

	deque< int >::iterator currentEventit;	
	deque< int > eventStored;	
	
	deque< set < vector <int> > >::iterator currentSharedit, currentHighlightedit, currentDifferedit, currentExpressedit;
	deque< set < vector <int> > > sharedStored, differedStored, expressedStored, highlightedStored;

	deque< set < vector <int> > > coloredStored;			
	deque< set < vector <int> > >::iterator currentColoredit;

	deque< vector < set < vector <int> > > > linkedStored;
	deque< vector < set < vector <int> > > >::iterator currentLinkedit;
	
	deque< vector <set < vector <int> > > > linkedStoredinHead;			
	deque< vector < set < vector <int> > > >::iterator currentLinkedinHeadit;
	

	
	deque< QString >::iterator currentPathwayGraphit;
	deque< QString > pathwayGraphStored;	
	
	float graphReferenceSize;

	bool pressOnBoarder, pressOnUngroup;
	
	float _ScaleBackup, _ScaleBackup_S;
	int m_codeBubbleID;
	int m_pathwayID;
	int m_treeRingBubbleID;
	int m_expressionID;
	int dataID,dataID_1; //dataID_1
	int _parentPathBubbleID;
	int fixedWidth, fixedHeight;
    

	QPointF dockItemDragged(QPointF pos);
	bool collision(ItemBase *node, QPointF pos);

	virtual QRectF boundingRect() const;
	QRect realRect();
	QRectF realRectF();
	virtual int half( int input )
	{
		return (int)((float)input / 2.0 + 0.5);
	}
	virtual bool between( int tar, int left, int right)
	{
		if( tar > left && tar < right)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	virtual bool outRange(int tar, int left, int right)
	{
		if (tar < left && tar > right)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	virtual bool inBox(QPointF cur, int width1, int height1, QPointF tar, int widht2, int height2 );
	virtual bool inBox(QPointF cur, int cur_size, QPointF tar, int tar_size);
	virtual bool outBox(QPointF cur, int width1, int height1, QPointF tar, int widht2, int height2 );
	virtual bool outBox(QPointF cur, int cur_size, QPointF tar, int tar_size);
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	virtual int getType()
	{
		return _TYPE;
	}
	virtual QGraphicsProxyWidget* getControl()
	{
		return _myControl;
	}
	void removeControl()
	{
		delete _myControl;
	}
	void mySetPos( QPointF pos);
	void movePosFromNavigationBar(QPointF pos);
	bool onHalo( QPointF pos );
	QPointF LeftTop()
	{
		QPointF tmp;
		tmp = this->pos() + this->boundingRect().topLeft();

		return tmp;
	}
	ItemLabel* getLabel()
	{
		return this->_label;
	}
	QPointF getTargetPos()
	{
		return this->_targetPos;
	}
	void setTargetPos( QPointF pos, QPointF stepSize=QPointF(1000000,1000000), bool synchronizedSpeed=false );
	bool adjustPosLineUp(QList<ItemBase*> groupMember);
	void setBoarderLeftFlag( bool flag )
	{
		boarderLeft = flag;
	}
	void setBoarderRightFlag( bool flag )
	{
		boarderRight = flag;
	}
	void setBoarderUpFlag( bool flag )
	{
		boarderUp = flag;
	}
	void setBoarderDownFlag( bool flag )
	{
		boarderDown = flag;
	}
	QString posString()
	{
		QString str, tmp;
		str = "(";
		tmp.setNum( this->pos().x() );
		str.append( tmp );
		str.append( ", ");
		tmp.setNum( this->pos().y() );
		str.append( tmp );
		str.append( ")" );
		return str;
	}
	QString sizeString()
	{
		QString str, tmp;
		str = "(";
		tmp.setNum( this->Width() );
		str.append( tmp );
		str.append( ", ");
		tmp.setNum( this->Height() );
		str.append( tmp );
		str.append( ")" );
		return str;
	}
	virtual void saveAsXmlNode( QDomDocument &doc, QDomElement &node );
	virtual void recoverFromXmlNode( QDomElement node );
	QRegion AllRegion();
	QRegion AllRegionSmallHalo();
	QRegion roundRect( QRect& rect, int radius );
	QString BubbleIdentifier();
	
	virtual void logOpen();
	virtual void logRemove();
	virtual void logMoveTo();
	virtual void logResizeTo();
	virtual void logUnGroup();
	QString headofLogMsg();
	QString tailofLogMsg();
	
	QPointF dCenter;
	bool toplevel;
	
	QSizeF getItemSize_2( int type, int id, QString qstr);
	int bIndex;
	void readexpression(const char *name, vector<QString> &quantityName, vector<vector<QString>> &geneInfo, vector<vector<float>> &quantity, vector<vector<float>> &minmax);

	void unGroupbyMenu();
	bool isUnGroup() {
		return UNGROUPBYMENU; 
	}

signals:
	void itemPosUpdated( ItemBase* init );
	void unGroup( ItemBase* init );
	void itemPosMoved( ItemBase* init, QPointF offset );
	void itemResized( ItemBase* init, QRectF initRect);
	void itemResized( ItemBase* init, QRectF initRect, int cornerID);
	void itemResized( ItemBase* init);
	void itemRemoved( ItemBase* init);
	void bubbleDeleteItself(ItemBase* init);
	void logInfor( QString str );	

protected:

	float referH; //a protein rect height/ for text hovered
	QColor ColorPalette(int i);
	bool onUngroup( QPointF pos );
	void ControlAutoHide();
	void drawHalo( QPainter* painter );
	virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
	virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	virtual void keyPressEvent(QKeyEvent *event);
	virtual void keyReleaseEvent(QKeyEvent *event);
	virtual void wheelEvent(QGraphicsSceneWheelEvent *event);
	virtual bool onBoarder( QPointF pos );

	int isInResizeArea(const QPointF &pos);
	int whichCorner(const QPointF &pos);
	bool isInLabelArea(const QPointF &pos);
	void drawControlHint( QPainter *painter );

	
	static void growSelectedItems(QGraphicsScene *scene);
	static void shrinkSelectedItems(QGraphicsScene *scene);
	//
	bool onMove( QPointF pos );
	bool onControl( QPointF pos );
	bool _Base_ONMOVE;
	bool _Base_ONCONTROL;
	bool _REMOVECONTROL;
	bool _CANBESHIFTED;
	void initControlsAnimation( QPointF pos );
	QTime m_startTime;
	bool m_isResizing;

	bool m_isMoving;//move bubble
	bool m_rightButton;//move bubble
	bool m_isInnerMoving; //move pathbubble side
	int m_size_width;
	int m_size_height;
	
	bool _LABEL;
	int _TYPE;
	//int _id;
	QPointF newPos;
	QGraphicsProxyWidget* _myControl;
	myColor3 _colorBoarder;
	myColor3 _color;
	int _colorBoarderAlpha;
	myColor3 _colorInner;
	
	int _initSize;
	int _initSize2;
	QPointF _initPos;
	QRectF _oriRect; //seldom change
	float _curScale; //seldom change
	
	bool fixedSize; //if the size of item shoudl be fixed as the bubble is created
	
	QVector<QLineF> _lines;
	bool _highLight,_pathwayHighlight;
	bool LASTOPENSTATE;
	bool CONTROLLOCK;
	myColor3 _hiColor;
	
	int getCalloutNote(QPointF pos);
	bool UNGROUP,UNGROUPBYMENU;
	bool DELEGATE;
	bool _labelInitiated;
	ItemLabel* _label; 
	QPointF _targetPos;
	QPointF _moveStepSize;
	bool _synchronizedSpeed;
	bool boarderLeft, boarderRight, boarderUp, boarderDown;

	QPointF x0y0, x1y1; //record the location of the map leftbottom and righttop

	QSizeF getItemSize( int type, int id, QString qstr);

void reScale(QPointF &x0y0, QPointF &x1y1, QPointF dxy0, QPointF dxy1, float &_scale, QPointF &dCenter, float rate=1);
void Pan(QPointF &x0y0, QPointF &x1y1, QPointF dxy0, QPointF dxy1, float &_scale, QPointF &dCenter);
void wheelReScale(QPointF &x0y0, QPointF &x1y1, QPointF dxy0, QPointF dxy1, float &_scale, QPointF &dCenter, float rate=1);

//void paintReaction( QPainter *painter, vector<QRectF> reactionPos, vector<vector<vector<int>>> _biochemicalReaction, int id, QPointF dCenter = QPointF(0,0), float scale=1.0, QString type = "T", bool flag=false,  QColor c=QColor(102,102,102,255));
//void paintSmallMolecule( QPainter *painter, vector<QRectF> smallMoleculePos, vector<vector<QString>> _smallMoleculeName, int id, QPointF dCenter = QPointF(0,0), float scale=1.0, bool flag=false,  QColor c=QColor(102,102,102,255));
//void paintDna( QPainter *painter, vector<QRectF> DnaPos, vector<vector<QString>> _DnaName, int id, QPointF dCenter = QPointF(0,0), float scale=1.0, bool flag=false,  QColor c=QColor(102,102,102,255));
//void paintEmpty( QPainter *painter, vector<QRectF> EmptyPos, vector<vector<QString>> _9EmptyName, int id, QPointF dCenter = QPointF(0,0), float scale=1.0, bool flag=false,  QColor c=QColor(102,102,102,255));

QColor assignColorToSharedItems(int pid, int type, int id, vector<set<vector<int>>> sharedSets);
QColor assignColorToDifferedItems(int pid, int type, int id);
QColor assignColorToExpressedItems(int pid, int type, int id, vector<set<vector<int>>> expressedSets, set<vector<int>> _itemColored, QColor assignColorToExpressedItems=QColor(255,255,255));
QColor assignColorToLinkedItems(float level, float totalLevel, int colorType=0);

void paintHoveredText( QPainter *painter, QRectF nodePos, QString nodeName, int type, int id, QPointF dCenter= QPointF(0,0), float scale=1.0, float L=0.0, float B=0.0, float W=1.0, float H=1.0, bool flag=false,  QColor c=QColor(102,102,102,255));

//QRectF getRelativeScale(QRectF oriRect, QPointF dCenter1, float scale1, QPointF dCenter2, float scale2);
QRectF getNewNodeRect(QRectF nodePos, int type, QPointF dCenter, float scale, QString reactionType="T");
QPointF getNewNodePoint(QPointF point, QPointF dCenter1, float scale1, QPointF dCenter2, float scale2);
QRectF getNodeRect(QRectF nodePos, QString nodeName, int type, bool fixedSize,  QPointF dCenter, float scale);
QRectF getrecttoRect(QRectF nodeRect, int type, bool fixedSize,  QPointF dCenter, float scale);

QRectF paintPreComputedNode( QPainter *painter,  float boundaryWidth, int fontSize, vector<QColor> insideColor, QColor boundaryColor, QColor fontColor, QRectF fontRect, QRectF nodeRect, QRectF nodePos, QString nodeName, int type, int id, vector<set<vector<int>>> sharedSets, QPointF dCenter = QPointF(0,0), float scale=1.0, QString reactionType="", float L=0.0, float B=0.0, float W=1.0, float H=1.0, bool flag=false,  QColor c=QColor(102,102,102,255));
QRectF paintPreComputedNode_H( QPainter *painter,  float boundaryWidth, int fontSize, vector<QColor> insideColor, QColor boundaryColor, QColor fontColor, QRectF fontRect, QRectF nodeRect, QRectF nodePos, QString nodeName, int type, int id, vector<set<vector<int>>> sharedSets, QPointF dCenter = QPointF(0,0), float scale=1.0, QString reactionType="", float L=0.0, float B=0.0, float W=1.0, float H=1.0, bool flag=false,  QColor c=QColor(102,102,102,255));
QRectF getNodetoBePaint( float &boundaryWidth, int &fontSize, vector<QColor> &insideColor, QColor &boundColor, QColor &fountColor, QRectF &fontRect, QRectF nodePos, QString nodeName, int type, int id, vector<set<vector<int>>> sharedSets,  vector<set<vector<int>>> expressedSets, QPointF dCenter = QPointF(0,0), float scale=1.0, QString reactionType="", float L=0.0, float B=0.0, float W=1.0, float H=1.0, bool flag=false,  QColor c=QColor(102,102,102,255));
QRectF paintNode( QPainter *painter, QRectF nodePos, QString nodeName, int type, int id, vector<set<vector<int>>> sharedSet,vector<set<vector<int>>> expressedSets,  bool fixedSize=true, QPointF dCenter = QPointF(0,0), float scale=1.0, QString reactionType="", float L=0.0, float B=0.0, float W=1.0, float H=1.0, bool flag=false,  QColor c=QColor(102,102,102,255));
QPointF moveDis( QPointF move, float scale);

void paintProtein( QPainter *painter, vector<QRectF> proteinPos, vector<vector<QString>> _proteinName, int id, QPointF dCenter = QPointF(0,0),float scale=1.0, bool flag=false,  QColor c=QColor(102,102,102,255));

void getNodeFont(int &fontSize, QRectF &fontRect, QRectF nodeRect, QString nodeName, int type, int width, int height, bool fixedSize, float scale=1.0, bool showOhers=false);
void getCompartmentFont(int &fontSize, QRectF &fontRect, QRectF compartmentRect, QString _compartmentName, bool fixedSize, float scale=1.0);
QRectF getCompartmentFont(QString compartmentName, bool fixedSize, float scale);
QRectF getCompartmentToBePaint(float &boundaryWidth, int &fontSize, vector<QColor> &insideColor, QColor &boundaryColor, QColor &fontColor, QRectF &fontRect,vector<QRectF> compartmentPos, QString _compartmentName, int id,vector<set<vector<int>>> sharedSet, vector<set<vector<int>>> expressedSet,  QPointF dCenter = QPointF(0,0), float scale=1.0, bool flag=false,  QColor c=QColor(102,102,102,255));

QRectF paintPreComputedCompartment( QPainter *painter,float boundaryWidth, int fontSize, vector<QColor> insideColor, QColor boundaryColor, QColor fontColor, QRectF fontRect, QRectF nodeRect,vector<QRectF> compartmentPos, vector<vector<QString>> _compartmentName, int id,vector<set<vector<int>>> sharedSet, QPointF dCenter = QPointF(0,0), float scale=1.0, bool flag=false,  QColor c=QColor(102,102,102,255));
QRectF paintPreComputedCompartment_H(  QPainter *painter,float boundaryWidth, int fontSize, vector<QColor> insideColor, QColor boundaryColor, QColor fontColor, QRectF fontRect, QRectF nodeRect,vector<QRectF> compartmentPos, vector<vector<QString>> _compartmentName, int id,vector<set<vector<int>>> sharedSet, QPointF dCenter = QPointF(0,0), float scale=1.0, bool flag=false,  QColor c=QColor(102,102,102,255));

QRectF paintCompartment( QPainter *painter, vector<QRectF> compartmentPos, vector<vector<QString>> _compartmentName, int id,vector<set<vector<int>>> sharedSet, vector<set<vector<int>>> expressedSets, bool fixedSize, QPointF dCenter = QPointF(0,0), float scale=1.0, bool flag=false,  QColor c=QColor(102,102,102,255));

//QRectF paintReaction( QPainter *painter, vector<QRectF> reactionPos, vector<vector<vector<int>>> _biochemicalReaction, int id, float L, float B, float W, float H, QPointF dCenter = QPointF(0,0), bool flag=false, QColor c=QColor(102,102,102,255));
//QRectF paintSmallMolecule( QPainter *painter, vector<QRectF> smallMoleculePos, vector<vector<QString>> _smallMoleculeName, int id, float L, float B, float W, float H, QPointF dCenter = QPointF(0,0), bool flag=false,  QColor c=QColor(102,102,102,255));
//QRectF paintDna( QPainter *painter, vector<QRectF> DnaPos, vector<vector<QString>> _DnaName, int id, float L, float B, float W, float H, QPointF dCenter = QPointF(0,0), bool flag=false,  QColor c=QColor(102,102,102,255));
//QRectF paintEntity( QPainter *painter, vector<QRectF> physicalEntityPos, vector<vector<QString>>  _physicalEntityName, int id, float L, float B, float W, float H, QPointF dCenter = QPointF(0,0), bool flag=false,  QColor c=QColor(102,102,102,255));
QRectF paintProtein( QPainter *painter, vector<QRectF> proteinPos, vector<vector<QString>> _proteinName, int id, float L, float B, float W, float H, QPointF dCenter = QPointF(0,0), bool flag=false, QColor c=QColor(102,102,102,255));
void drawLinkArrow( QPainter *painter, QPointF start, QPointF end ) ;
void drawLinkArrow_2(QPainter *painter, QPointF start, QPointF end );

void drawCurvedArrow(QPainter *painter, set<int> code, float h, QPointF start, QPointF end, float width, float height);
void drawCurvedArrow_H(QPainter *painter, set<int> code, float h, QPointF start, QPointF end, float width, float height, float _scale, QColor color);
void drawArrow(QPainter *painter, QPointF start, QPointF end, float width, float height, QColor color, QRegion clipRegion=QRect(0,0,0,0));
void drawArrow_H(QPainter *painter, QPointF start, QPointF end, float width, float height, float _scale,  QColor color, QRegion clipRegion=QRect(0,0,0,0));
void drawArrow_GL(QPainter *painter, QPointF start, QPointF end, float width, float height);
void drawArrow_GL_H(QPainter *painter, QPointF start, QPointF end, float width, float height, float _scale=1);
void drawArrow(QPainter *painter, QPointF start, float h, QPointF end, float width, float height, QColor color, QRegion clipRegion=QRect(0,0,0,0));
void drawArrow_2(QPainter *painter, QPointF start, QPointF end, float width, float height, QColor color);
void drawArrow_3(QPainter *painter, QPointF start, QPointF end, float width, float height, QColor color); //polyline
//void drawArrow_4(QPainter *painter, QPointF start, QPointF end, float width, float height, QColor color); // doted line

void drawArrow_4_GL(QPainter *painter, QPointF start, QPointF end, float width, float height);
void drawATrapezoid_GL(QPainter *painter, QPointF start, QPointF end, float size1, float size2);

QVector<QPointF> getArrow(QPointF start, QPointF end, float width, float height, float _scale=1);
QVector<QPointF> getArrow_H(QPointF start, QPointF end, float width, float height, float _scale=1);
QVector<QPointF> getArrow_4_H(QPointF start, float h, QPointF end, float width, float height, float _scale);
QVector<QPointF> getArrow_H(QPointF start, float h, QPointF end, float width, float height, float _scale);
void getATrapezoid(QPointF start, QPointF end, float size1, float size2, QVector<QPointF> &points);
QVector<QPointF> getArrow_4(QPointF start, QPointF end, float width, float height, float _scale=1);
QVector<QPointF> getArrow_4(QPointF start, float h, QPointF end, float width, float height, float _scale=1);
QVector<QPointF> getArrow(QPointF start, float h, QPointF end, float width, float height, float _scale=1);

void drawCurvedArrow_GL(QPainter *painter, set<int> code, float h, QPointF start, QPointF end, float width, float height, float _scale=1);
void drawCurvedArrow_GL_H(QPainter *painter, set<int> code, float h, QPointF start, QPointF end, float width, float height, float _scale=1, QColor color=MIDIUM_HIGHLIGHTCOLOR);

void drawArrow_5(QPainter *painter, QPointF start, QPointF end, QRect noteRect, float width, float height, QColor color);
void drawNoteMark(QPainter *painter, QPointF start, QPointF end, QRect noteRect, QPointF markPos, QRectF &markRect, float width, float height, QColor color);
void drawCross(QPainter *painter, QRectF cross, QColor color);
void drawMinus(QPainter *painter, QRectF minus, QColor color);
void drawMaximize(QPainter *painter, QRectF minus, QColor color);
QPointF Interpolation(vector<QPointF> &points,  vector<QPointF>::iterator &start, float x);
QPointF Interpolation(vector<QPointF> &points, vector<QPointF>::iterator &start, float x, float &Dx);

void drawATrapezoid(QPainter *painter, QPointF start, QPointF end, float size1, float size2, QColor color);

void getMids(set<vector<float>> &points);
vector<QPointF> getCurveLine(QPointF start, QPointF mid, QPointF end);


void drawDissociation(QPainter *painter, QRectF rect);
QRegion getClipRegionforCalloutNote();
QRegion getCalloutNoteRect();
QRegion getCalloutNoteRectforNote();


QColor getEdgeColorByPathway(int id);

void encode(float x,float y,int &code,float XL,float XR,float YB,float YT);
int CSLineClip(float &X1,float &Y1,float &X2,float &Y2,float XL,float XR,float YB,float YT);

};

class QtBox : public ItemBase
{
public:
	QtBox(int size, int x, int y);
	virtual ~QtBox();
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
protected:
private:
	QVector3D m_vertices[8];
	QVector3D m_texCoords[4];
	QVector3D m_normals[6];
	GLTexture *m_texture;
};

class CircleItem : public ItemBase
{
public:
	CircleItem(int size, int x, int y);
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
protected:
	QColor m_color;
};

class SquareItem : public ItemBase
{
public:
	SquareItem(int size, int x, int y);
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
protected:
	QPixmap m_image;
};

#endif
