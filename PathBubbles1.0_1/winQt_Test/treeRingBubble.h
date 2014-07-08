#ifndef TREERINGBUBBLE_H
#define TREERINGBUBBLE_H

#include "PathBubbleBase.h"
//#include "penstroke.h"
#include "TreeRing.h"
#include "BundlesPrimitive.h"
#include "ExtraLayerMarker.h"
#include "SmallTreeRing.h"
#include "Diagram.h"
#include "Label.h"
#include "SimData.h"
#include <set>
#include <vector>
#include <list>

#define  DIST_THRE  25

using namespace std;

//class TextBubble2;
//class GroupBubble4;
//class ReactionBubble3;
class TreeRing;
class BundlesPrimitive;
class ExtraLayerMarker;
class PathBubbleBase;
class Label;

//typedef boost::adjacency_list<boost::listS, boost::vecS, boost::undirectedS> MyGraph;
//typedef boost::graph_traits<MyGraph>::vertex_descriptor MyVertex;

struct treeRingConnector
{
	//the node in the parent treeRing
	//int layer, idx; 
	int layer,idx;
	ItemBase * childBubble;	 //could be treeRing or pathbubbles
	set<vector<int>> itemSelected;
};


class treeRingBubble : public PlotBase
{
	Q_OBJECT
public:
	treeRingBubble( int size, int x, int y, OpenGLScene* Scene, StatisticManager* manager, OpenManager* open, QString name );
	treeRingBubble( int size, int x, int y, OpenGLScene* Scene, StatisticManager* manager, OpenManager* open, treeRingBubble *parent, set<vector<int>> itemSelected);
	treeRingBubble::~treeRingBubble();

	bool clearSearchSelection();

	//extern CompoundGraph *cg;
	//extern Slider *mySlider;
	//extern GLfloat thre;
	bool expressionCleared;

	void clearUngroup()
	{	
		AFTERUNGROUP = false;	
		UNGROUP = false;			
	}
	void clearSketch()
	{
	    _lines.clear();	
	}
	void saveAsXmlNode( QDomDocument &doc, QDomElement &node );

	int _num_layers;

	vector<struct treeRingConnector>_connections;
	QRectF _minimizeMark, _maximizeMark,originalRect;
	float miniMized;
	void pathExpressionRecord(expressionBubble *ebubble);
	vector<vector<QString>> pathExpressionRead(expressionBubble *ebubble);	
	set<QString> pathExpressionReadPathID(expressionBubble *ebubble);
	bool isMissingFile(QString pathID);

	bool firstMotionLeft;
	bool firstMotionRight;	

	/* globals */
	GLuint    stroke_lasttime;
	GLfloat   stroke_lastposition[3];

	int mousex, mousey, oldMousex, oldMousey;

	GLint     stroke_button;
	GLboolean stroke_tracking;
	GLboolean stroke_animate;

	

	float dist(int x1, int y1, int x2, int y2);
	//void strokeReshape(int width, int height);
	//void strokeMouse(int button, int state, int x, int y);
	void _strokeStartMotion(int x, int y, int button, int time);
    void _strokeStopMotion(int button, unsigned time);
	void strokeAnimate(GLboolean animate);
    void strokeInit(GLuint button);
	//void strokeMotion(int x, int y);
	bool strokeMotionLeft(int x, int y);
	//bool handleMotionLeft(int x, int y);
    void strokeMotionRight(int x, int y);

	void deleteItems();
	void setExpressedGenePos(int orderType);
	
	void updateConnectionAfterDeletion();	
    void updateConnectionAfterMerge(treeRingBubble *treeRing1, treeRingBubble *treeRing2);
	void updateConnectionAfterSubstraction(TreeRing *_curTreeRing);
	bool isSegPreSelected(int layer, int id, set<vector<int>> preItemSelected);
 

	void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget );

	void initTreeRing(int x, int y, int window_w, int window_h);
	void initSubTreeRing(treeRingBubble *parent, set<vector<int>> itemSelected, int x, int y, int window_w, int window_h);
	int reCountNodeSize(TreeRing *_treeRing, int layer, int id, set<vector<int>> itemSelected, bool Nflag);
	
	void SetEdgeBy(int newToken);
	void Render(QPainter *painter);
	void drawConnections(QPainter *painter, QPointF dis);
	void Zoom(float zoom);
    void Pan(float x, float y);
	float _zoom;

	Point compoudgraph_centre;

	char *treeName, *graphName, *dataName, *extraLayerName;
	float ringRadius, ringWidth;
	int EDGE_BY;
	int GetPreEdgeBy();
	int GetEdgeBy();
	
	Point _centre;	
	Label     *_Label;      // node labels
    SimData   *_simData;  // simulation data
	
    vector<SmallTreeRing *> _small_tree_ring;
    vector<Diagram *> _diagram;
    vector< pair<int,int> > _add; 

	set<vector<int>> initItemSelected;
	//treeRingBubble *_parent;
	int _parentID;

    // highlighted 
    int    _hl, _hn;
	float _hx, _hy; //mouse location of x 
    int    _nodeIndex[2]; // the node index in the layer 2 of the current 
                          // highlighted edges

	// set the _nodecolor and _nodeChanged
	// all in the function SetNodeColor
	// these are the colors/changed in every hierachy; not just the leaf nodes
    vector <  vector <Color> > _nodeColor; 
	//vector <  vector <int> >   _nodeChanged; 
   
    // this is the tree node information
    float  _ring_radius;  // tree node ring radius 
    float  _ring_width;  // tree ring width - space between two adjacent rings
    int    _node_num;   // number of tree node - or phy parameteres -
	int window_w, window_h; //for tree ring sizex
	

	void undo();
    void redo();
	
	vector<BundlesPrimitive *> _bun;
	ExtraLayerMarker *_lMarker; //mark more layer
	TreeRing *_treeRing;
	treeRingBubble * _treeRingBubbleParent;	
	QString _orthologyFileName, _expressionName;
	QString getCurrentFilename()
	{
		return _curFileName;
	}
	//void rebuildBubbleConnect( QDomElement node );
	QString _curFileName;
	QString _curFilePath;
	void setCurrentFile(const QString &fileName);
	void loadFile(const QString &fileName);

	bool UpdateHighlight(bool *firstMotion, int mx, int my);
	//bool UpdateHandle(bool *firstMotion, int mx, int my);
		
	void renderColorLegend(QPainter *painter);
	void drawStatisticsText(QPainter *painter, float x, float y, float angle, int layer, int idx);
	
	//void AddSmallTreeRing();
	//void renderSmallTreeRing(QPainter *painter);
	//void renderDiagram(QPainter *painter);
	//void CheckDiagramSelect(int x, int y);	
	//void AddDiagram();

	void searchSharedProtein(int excludePid=-1, bool shareflag=true);
	void searchExpressed();
	void searchSharedProteininAExpressionBubble(expressionBubble *item);
	void searchExpressedinAExpressionBubble(expressionBubble *item);

	set<vector<int>> getItemDragged();
	set<vector<int>> _itemDragged;
	bool spanIsOn; 

	void setPenOn();
	vector<int> whichItem( QPointF pos);
	set< vector< int > > whichSegment( int layer, int id );
	vector<int> whichHandle( QPointF pos);
	vector < set< vector<int > > > multiItemSelected;
	set< vector<int > > itemSelected;
	set< vector<int > > itemRemoved;
	vector<int > handleSelected;
	vector<int> nodePicked; //for open pathtways	
	//vector<float> segAngles;
	set< vector< int > > whichSegment( QPointF pos );
	bool bubbleDraggedout;
	QPointF lastPressedPos;
	void subStarctTreeRing();

	// temp variable to test group selection by pen
	//vector<QPointF> ptest;

public slots:
	void selfDelection(treeRingBubble* b0, treeRingBubble* b1);

private:
	float _scale;
	void getStartSegsNodes(set<vector<int>> itemSelected, vector<QPolygon> &segPoints, vector<int> &cornerNode_2, QPointF Pos);	
	//ItemLabel* _Label; 

	QString getTreeRingName(treeRingBubble *parent, set<vector<int>> itemSelected);
	QString getTreeRingName(set<string> nameSelected);
	void updateLabel();
	void resetLabel(QString name);
	void drawfills( QPainter* painter);
	void InsertToOrder(int pid, int count, vector<int> &order);
	void MergeTreeRings(treeRingBubble *treeRing1, treeRingBubble *treeRing2, int window_w, int window_h);
	
	int getNewNodeID(int layer, int oID, TreeRing *_preTreeRing, TreeRing *_curtreeRing);
	set<vector<int>> preItemSelected;
	bool isSegHighlighted(int layer, int id);
	bool isSegHighlighted(set< vector< int > > segs);
	
	void subStarctTreeRing(set<vector<int>> itemSelected);
    QPointF initPos;
	QPolygonF _contain;	
	QVector<QLineF> fills;
	void drawCurve( QPainter* painter );
	void addLine( QPointF pos1, QPointF pos2 );
	bool groupSelection(QVector<QLineF> _lines);
	void updateEdgeList(vector<string> oldNodeName, vector<string> newNodeName, vector<vector<int>> &edgeList);
	void updateParentTreeRingAfterCollapse(treeRingBubble *parent, set<vector<int>> itemSelected, int window_w, int window_h);	
	vector<int> getNewNodeID(int layer, vector<int> oID, vector<int> nodeIDs, set<int> nodeIDSet, int ring_1_Size, TreeRing * _treeRing1,TreeRing *_treeRing2);
	int getNewNodeID(int layer, int oID, vector<int> nodeIDs, set<int> nodeIDSet, int ring_1_Size, TreeRing * _treeRing1, TreeRing *_treeRing2);
	bool draggingItem;
	int dragout; //1: drag an item out of the region, 2: drag an compartment out of region		
	OpenManager* _open;

	vector<vector<double>> _inputData_Backup;
	vector<vector<int>> _inputNodeSize_Backup;
	vector<vector<string>> _inputTree_Backup;
	vector<vector<int>> _outList_Backup, _inList_Backup; //_out_list/in_node_list//_rr	
    void storeTreeRing(treeRingBubble *parent); //store data infor to backups

	bool onBoarder( QPointF pos );
    bool outsideBubble( QPointF pos );
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event );
	void wheelEvent(QGraphicsSceneWheelEvent *event );
	void hoverMoveEvent(QGraphicsSceneHoverEvent *event );
	virtual void keyReleaseEvent(QKeyEvent *event);
	void mousePressEvent(QGraphicsSceneMouseEvent *event );
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event );	
	void mouseMoveEvent( QGraphicsSceneMouseEvent *event );

	//virtual void keyPressEvent(QKeyEvent *event);
	//virtual void keyReleaseEvent(QKeyEvent *event);
	//bool controlKeyisOn;
	
	void setItemDragged(vector<int> nodePicked);
	void setItemDragged(set< vector<int > > itemSelected);
    void setAItemDragged(int layer, int id);
	void paintItemDragged(QPainter *painter, vector<QPolygon> itemDragged);
	vector<QPolygon> itemDragged;
	void dragItem(QPointF Pos);
	QRectF bubbleRect;    
    //void drawABubbleConnection(QPainter *painter, QPointF center1, ItemBase* bubble1, QPointF center2, ItemBase* bubble2, QPointF Dis);	
	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
	bool _mousePressed;	
};

#endif