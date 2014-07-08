

//#include <iostream>
//#include <fstream>
//#include <assert.h>
#include <vector>
#include <set>
//#include <stdlib.h>
//#include <string.h>
//#include <math.h>
#include <QPainter>
#include <QPen>
#include "Point.h"
#include "Ring.h"
#include "Stroke.h"
#include "SimData.h"
#include "LAB2RGB.h"
#include "LABColors.h"
#include "qpainter.h"
#include "3DHelper.h"
#include "pathwayDataParser.h"

#ifndef __TREERING_H
#define __TREERING_H
#include <QString>
using namespace std;

#define HL_NODE   1
#define HL_CURVE  2
#define HL_HANDLE   3
#define RADIUS_LIMIT 20.

struct expressedPathwayInfo
{
	QString pathIDs;
	float expressionLevel;	
};

class PWDParser;

class TreeRing 
{
public:
	vector<vector<float>> expressionLevel;
	float expressionMin, expressionMax; //for expression legend
	float eMin, eMax, cMin, cMax, eItv, cItv;
	int eNum, cNum, eDL, cDL; //legend levels and digital number
	int crossTalkMin, crossTalkMax, rateLimitMin, rateLimitMax;
	vector<QString> eMarkers, cMarkers;
	bool shrinked;

	///////////// for handle selection to change fan portion
	int  handleLayer, handleIndex, handlet;
	
	TreeRing(char *tinf, float r, float w, SimData *sim_data, int EDGE_BY, Point centre);
	TreeRing( vector<vector<string>> inputName,  vector<vector<QString>> inputPathID, vector<vector<int>> inputNodeSize,  vector<vector<int>> rateLimitNum, float r, float w, SimData *sim_data, int EDGE_BY, Point centre, QString orthologyFileName = "") ;
	TreeRing(){}
	virtual ~TreeRing(){}
	
	void Print();
	//void PrintRingNodePositions();
	//void PrintEdgesBetweenLayers();
	void CalculateLabelDisplayOrder();

	void loadRateLimit();

	vector<int> CheckPointInTreeRing(float x, float y);
	vector<int> CheckHandleInTreeRing(float x, float y);
	int getHighlightState() 
	{
		return highlightState;  
	}
	void spanAnode(int handleLayer, int handleIndex, int handlet, float newangle);
	void shrinkNodes(vector<int> respondLayer, vector<int> respondIndex);	
	void unShrinkNodes();
	int layerStored; 
	vector<float> _angleStored; //store information before shrinkNodes for unshrinkNodes()
    vector< pair<float, float> > _limitsStored;
	vector<Point> _radial_posStored;

	vector<int> getSegs(vector<int> respondLayer, vector<int> respondIndex, bool selected);

	void reCalculateNodePosition(int speclayer=-1);
	void reRadialLayout(vector<double> preSpans);
	float getAngleSpan(float first, float second);
	int SearchFind(QString str, int matchType);
	set<vector<int>> searched;
	vector<Point> getNode(int layer, int idx, Point p0, Point p1, Point p2, Point p3, int Half);
    QPolygon getSegPoint(int topLayer, int bottomLayer, int idx1, int idx2, float dx, float dy, int &cornerID1, int &cornerID2);
	void findAllDescendantNodes(int layer, int id, set<vector<int>> &toBeInsert);	
	void  findAllDescendantNodes(int layer, int id, vector<vector<int>> &output);
	vector<string> findNameList(int layer, int id);
	void locateNodeByNameList(vector<string> Names, int layer, int &id, int startid, int endid);
	//void UpdateHandlePointToMouse(int mx, int my, int ox, int oy);
	
	void CopyRingAndStroke()
	{
		_org_ring= _ring;
		_org_curve= _curve;
	}

	set< set< vector<int > > > itemSelected;//for those dragged out
	set< vector<int > >  _itemClicked;
	set< vector<int > > _itemDragged; //for those are dragged
	//set< vector<int > > itemRemoved;
	vector <int> nodePicked;
	vector <int> handleSelected;

	PWDParser *pwdParser;
	// CALLBACK routines
	virtual void Passive(float thre, int mx, int my); 
	// passive callback

	//virtual void Render();
	float getWidth() { return _width; }
	virtual void Render(QPainter *painter);

	virtual void UpdateHighlightedPointToMouse(int mx, int my, int ox, int oy);
	
	//vector<int> FindPathByVariance(int id, int layer);
	//vector<int> FindPathByCv(int id, int layer);

	void ComputeNodeColorByO(QString);
	// update to new node positions
	// if a node data varies, display that node;
	//  otherwise, assign the location to the current 
	//  node location  (basically stack nodes together and
	//  hide the no-change ones to the background)
	//
	// need to first know how many are changed so we can
	// decide the angles

	// GETS
	virtual int GetGlobalNodeIndexByLayer( int slayer, int sNodeIndex,
		int tlayer, int tNodeIndexRange[2]);
	// find the target edge index given slayer, sNodeIndex, and tlayer

	virtual int GetNodeIndexByLayer( int slayer, int sNodeIndex,
		int tlayer, int tNodeIndexRange[2]);
	// find the target node index range given slayer, sNodeIndex

	vector <int>  GetRingNodeSizeByIndex(int layerIndex)
	{
		return (_ring[layerIndex])._node_size;
	}

	void searchMatchedProtein(QString eName, vector<vector<QString>> geneInfo, vector<vector<float>> quantity, set<QString> &matchedPathways, set<int> &matchedGene_TreeRing_Expression);
	void searchMatchedProtein(vector<vector<QString>> geneInfo, set<QString> pathID, vector<vector<QString>> pathInfo, set<QString> &matchedPathways, set<int> &matchedGene_TreeRing_Expression);	

	vector<struct expressedPathwayInfo> searchExpressed(vector<vector<QString>> geneInfo, vector<vector<float>> quantity, set<QString> matchedPathways, set<QString> pathID, vector<vector<QString>> pathInfo, set<int> &expresseGeneLinkedtoTreeRing);
	vector<struct expressedPathwayInfo> searchExpressed(QString eName, vector<vector<QString>> geneInfo, vector<vector<float>> quantity, set<QString> matchedPathways, set<int> &expresseGeneLinkedtoTreeRing);
	//////////////// parent node information
	int GetAChildInMiddle(int layerIndex, int nodeIndex);
	int GetParentIndex(int layerIndex, int nodeIndex);	
	void paintNode_1(QPainter *painter, int layer, int idx, Point p0, Point p1, Point p2, Point p3, bool onlyNode);

	// return the parent layer index of the nodeIndex in the layerIndex

	//Point CalcParentNodePosAtR(int layerIndex, int nodeIndex, 
	//	int *parentIndex, float newR);
	// return the parent layer index and parent position 
	void clearHighLight(	) 
	{
		highlightLayer=-1;
		highlightIndex=-1;
		handleLayer = -1; handleIndex = -1;
		highlightState = 0;   
		 respondLayer.clear();
		 respondIndex.clear();
	}

	int GetHighlightState() { return highlightState; }
	// highlight state
	int GetNumOfNodes() { return _node_count; }
	// number of nodes
	int GetHighlightLayer() {return highlightLayer;}

	int GetHighlightIndex() {return highlightIndex;}

	vector < SimDataElem > GetData();

	// SETS - 
	void  SetEnableControlPolygon(int newState);
	// display the b-spline polygon or not
	void  SetEnableControlPoint(int newState);
	// display the control points or not
	
	void CheckPointInNode(float x, float y);

	void SetColorBy(int newToken){ if(newToken!=_edge_by) _pre_edge_by=_edge_by;  _edge_by = newToken;}

	int GetColorBy(){ return _edge_by;}
	int GetPreColorBy(){ return _pre_edge_by;}

	//const Point &GetMirrorPos(int layer, int index){return _ring[layer]._mirror_pos[index];};

	const Point &GetCentrePos(int layer, int index){return _ring[layer]._centre_pos[index];}

	const Point &GetRadialPos(int layer, int index){return _ring[layer]._radial_pos[index];}

	Color &GetColor(int layer, int index){return *(_ring[layer]._node_color_iter + index);}
	
	const Point GetTreeRingCenter(){return _centre;}

	int GetRingWidth(){return _width;}

	int GetNrLayers(){return _num_layers;}

	vector<int> getChildrenNodes(int layer, int idx); //leaves	

	vector<int> getParentNode(int layer, int idx);
	int getAncestorNodebyLayer(int cLayer, int cIdx, int aLayer );

	float GetRadius(){ return _radius; }

	float GetAngle(int layer, int index){return _ring[layer]._angle[index];}
	//void paintNode(QPainter *painter, Point p1, Point p2, Point p3, Point p4);
    bool paintNode(QPainter *painter, int layer, int idx, Point p0, Point p1, Point p2, Point p3, vector<QColor> rlNode, bool onlynode=false, int sideNode=0,bool highLightLayer=false);
	void drawHandle(QPainter *painter, int layer, int idx, Point p0, Point p3, QColor color);
	void drawATrapezoid(QPainter *painter, QPointF start, QPointF end, float size1, float size2, QColor color);
	vector<QPointF> getATrapezoid(QPointF start, QPointF end, float size1, float size2);
	vector<QPointF> getATrapezoid(float angle, float angle1, float angle2, float radius, float width);
	vector<QPointF> TrapezoidtoNode(QPointF p0, QPointF p1, QPointF p2, QPointF p3, float radius);

	bool isNodeHighlighted(int layer, int id);
	QPolygon getANode( int layer, int idx);//., (_ring[layer]._pos[id])[0], (_ring[layer]._pos[id])[1], (_ring[layer]._pos[id])[2], (_ring[layer]._pos[id])[3]);
	/*pair<float,float> &GetLimits(int layer, int index){return _ring[layer]._limits[index];}*/

	pair<int, int> &GetChildRange(int layer, int index){return _ring[layer]._child_range[index];}

	string GetNodeName(int layer, int index){return _ring[layer]._node_name[index];}
	QString GetPathID(int layer, int index){return _ring[layer]._node_pathID[index];}

	QString _orthName;
	int GetNodeNum(int layer){return _ring[layer]._node_num;}
	
	string GetLongestName();

	int GetLayer0ParentIndex(int layer, int index);


	void Scale(float scale);
	void Translate(float x, float y); //Transform treering to x,y position
	float GetRingRadius(int layer){return _ring[layer]._ring_radius;}
	float GetNodeRadius(int layer, int index){return _ring[layer]._radius[index];}

	pair<int, int> GetLeafNodeRange(int layer, int index);

	QString getOrthFileName() { return _orthologyFileName; }
	QString getPathID(QString pathName);
	void getNodeIDsbyID(QString ID, set<vector<int>>  &searched);
	int SearchFind(vector<QString> IDs);
	
	vector <Ring>    _ring;   // the ring geometries
	vector <Ring>    _org_ring;   // save the initial ring geometries
	vector< vector<int> > _label_display_order; //From layer _num_layers-1 to layer 0
	vector<int>  respondLayer, respondIndex; //record the nodes linked to the highlighted node
protected:
	int STILL_UPDATE;

	///////////// for curve point selection //////////
	int    highlightLayer, highlightIndex;

	


	
	// layer index, node/curve index
	int    highlightState;
	// 0: nothing
	// 1: HL_NODE
	// 2: HL_CURVE

	void storeInputTree(vector<vector<string>> inputName, vector<vector<QString>> inputPathID, vector<vector<int>> inputNodeSize, vector<vector<int>> rateLimitNum);
	void  readInputTree();
	// read the iput file
	//  - set  _curve
	//  - set ENABLE_FULL = true;
	//

	//void  readInputTree(vector <int> changed);
	// read the input file 
	//  - set _curve AND _changedCurve AND _curveToChangedCurve 
	//  - set ENABLE_FULL = false;
	//

	//void  setTree(int layerIndex); 
	// set the tree structure on the ring

	

	void GetNodeOrthRate(QString onthName);
	void initNodeOrthRate();
	void ComputeNodeVariance();
	void ComputeNodeCv();
	void ComputeNodeDomain();
	
	void ComputeNodeColor();

	void ComputeNodeColorByV();
	void ComputeNodeColorByO();

	//void ComputeNodeColorByO_2();
	void ComputeNodeColorByCv();
	void ComputeNodeColorByDomain();
	void InitNodeColorByE();
	void InitNodeColorByO();

	//vector<vector<QString>> pathExpressionRead(expressionBubble *ebubble);	
	//set<QString> pathExpressionReadPathID(expressionBubble *ebubble);

	//void  setNodePositionWhenHighlighted(Ring *r, float ra);
	// TODO: not implemented. will assign the node position when
	// the node is highlighted..
	//  the node should be re-arranged so they are legible while
	// the rest of the nodes squeezed...

	//void  updateParentNodePosition(int layerIndex, float ra, float rw);
	// 
	void  cleanStrokeCurve();
	// delete the _curve content
	// useful when the node position gets updated
	// and the curve may also needs to be updated;
	//void  updateStrokeCurve();
	// whenever the control points is updated

	// 
	//void  setStrokeCurve();
	// set _curve (using _ring points)
	//  that defines the control points on the 
	//  b-spline curve; 
	//
	//  need to be called AFTER the _node_pos
	//  is changed
	//  or 
	//  whenever we recalculated the _node_pos,
	//  we MUST call this function
	//  to show the correct stroke positions
	//  

	int toGlobalNodeIndex(int tlayer, int bNodeIndex);

	///////////// for curve point selection //////////
	void getClosest(float thre, int mx, int my);

	void ChangePointOrder(vector<Point> &out, vector<Point> &in);
	
	void CalculateNrChildren();

	void RadialLayout();

	void CalculateNodePosition();

private:
	QString _orthologyFileName;
	float _radius, _width;
	int   _num_layers; 
	int   _node_count; // total number of nodes (including both
	// changed and not changed)
	
	int _edge_by,_pre_edge_by;

	Point _centre;

	SimData *_sim_data;

	vector <Stroke>  _curve;
	// save all node curves

	vector <Stroke>  _org_curve;
	// save the initial state

	char  *tree_infile;

	///////////// _curve stroke properties ///////////
	bool   _opaque;
	bool   _ENABLE_controlPolygon;
	// default: false  (don't show the b-spline polygon
	//        
	bool   _ENABLE_controlPoint;
	// default: true (show the control b-spline control points);

	bool   _ENABLE_FULL; 
	// true: display all nodes
	// false: only displayed those changed

	//, *graph_infile;
};

#endif // __TREERING_H