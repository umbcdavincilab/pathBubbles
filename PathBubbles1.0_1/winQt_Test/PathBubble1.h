
#ifndef PATHBUBBLE1_H
#define PATHBUBBLE1_H


#include "PathBubbleBase.h"
#include "pathwayDataParser.h"
#include "PieMenuItem.h"
#include <set>
#include <vector>
#include <queue>

using namespace std;

class TextBubble2;
class GroupBubble4;
class ReactionBubble3;
class PathBubbleBase;
class subPathBubble1;
class StatisticManager;
class PWDParser;

//typedef boost::adjacency_list<boost::listS, boost::vecS, boost::undirectedS> MyGraph;
//typedef boost::graph_traits<MyGraph>::vertex_descriptor MyVertex;


class PathBubble1 : public PlotBase
{
	Q_OBJECT
public:
	
	PathBubble1(int pathid, int size, int x, int y, OpenGLScene* Scene, StatisticManager* manager, OpenManager* open, QString name, QString orthName = tr(""), QString pathName = tr(""), QString lastStoredGraph=tr(""));	
	PathBubble1::~PathBubble1();
	void updateTreeRingConnectionAfterDeletion();
	void updateConnectionAfterDeletion();
	
	void openPreNodeNotes();
	
	PathBubble1 *headPathway;
	bool onBoarder( QPointF pos );
	bool onNoteMark(int pid, int type, int id, QPointF pos);

	void updateSpanEdgesAfterAggregation(int ID);
	void updateSpanEdgesWhenCollapse(int Aid);	
    void reCoverEdgeConnection(int Aid, int pid1, vector<vector<vector<int>>> edge1, vector<vector<int>> _ANodeContain, set<vector<vector<int>>> &edgeconnections, bool reverse=false, bool noInsert=false);

	void recordPathBubbleConnections();
	void drawConnections(QPainter *painter, QPointF dis);
	void drawConnections_H(QPainter *painter, QPointF dis);
	void drawSharedPointers(QPainter *painter, QPointF dis, vector<int> item1, vector<int> item2);
	vector<set<vector<int>>> initANodeContains;
	bool initialANodeHandled;

	set<vector<vector<int>>> updatedEdge_1, updatedEdge_2, updatedEdge_3, updatedEdge_4; //updatedEdge_1 record visible edge; updatedEdge_2,updatedEdge_3 duplicatedly record the edge if it is activat or inhibiant, updatedEdge_4: with two or more overlapped edge (e.g. reaction edge and hibition edge/aggregation edge, or multi-edges resulted from aggreation)
	vector<int> updatedEdge_5; //record edge encoding for updatedEdge_4 (make sure the order matched)
	bool inUpdatedEdge(vector<vector<int> > edge, set<vector<vector<int>>> updatedEdge);

	QPointF getx0y0() {return x0y0; }
	QPointF getx1y1() {return x1y1; }
	
	void getBoundingBoxToBePaint();

	vector < set < vector <int> > > assignSharedSets();
	
	int undo();
	int redo();

	QRectF getPos(int type, int id);
	bool aggregation();
	void updateEdgetoNode(vector<vector<int>> edge); 
	bool aggregation(set < vector <int> > highlighted, int Aid=-1);
	void tempCollapseALL(vector<QRectF> &backUpAPos, vector<vector<vector<int>>> &_sceneANodeContain);
    void reAggregateAll(vector<QRectF> backUpAPos, vector<vector<vector<int>>> _sceneANodeContain);


	bool collapse(int Aid);
	bool collapseALL();
	void updateEdgeAfterCollapse(int Aid);
	bool isLinkinOriginalPathway(vector<vector<int>> nlink, vector<vector<vector<int>>> edge, bool reverse=false);
	int collapse();
	int getOriNodeID(int pid1, int pid2, int type2, int id2);
	void findNodesSameNames(set<vector<int>> &searched);
	int assignANodeName(QString Name, QString Name1, int Aid=-1);

	bool storeHighlighted();
	bool storeShared();
	bool storeColored();
	bool storeExpressed();
	bool storeDiffered();
	bool storeLinked();

	void redoHighlighted();
	void redoShared();
	void redoColored();
	void redoExpressed();
	void redoDiffered();
	void redoLinked();
	void redoGraph();

	void undoHighlighted();
	void undoShared();
	void undoColored();
	void undoExpressed();
	void undoDiffered();
	void undoLinked();
	void undoGraph();

	void clearEvents();
	int getTotalLinkLevel();
	int findLinkedLevel(int pid, int type, int id);
	int findLinkedLevel(int pid1, int type1, int id1, int pid2, int type2, int id2) ;
	void cleanBeforeRedoUndo(int eventType);
	vector < set < vector <int> > > assignExpressedSets();
	vector<QString> getExpressedGeneNames();

	PWDParser *pwdParser;
	QString getOrthFileName() { return _orthologyFileName; }
	void setOrthFileName(QString name) { _orthologyFileName=name; }

	void recordItemGrid();
	void getAllSubPathBubble(PathBubble1* bubble, QList<ItemBase *> &list);
	void getAllPSPathBubble(PathBubble1* bubble, QList<ItemBase *> &list); //bubble in the whole tree hierarchy
	void getHigherPathBubble(PathBubble1* bubble, QList<ItemBase *> &list); //bubble in higher level of the whole tree hierarchy	
	int getPathBubbleLevel(PathBubble1* bubble);
	bool samePathwayGroup(PathBubble1* bubble1, PathBubble1* bubble2);
	void getAllSubTextBubble(PathBubble1* bubble, QList<ItemBase *> &list);

	PathBubble1* getTopPathBubble(PathBubble1* bubble); 
	//int getTotalLevel(int pid, int type);
	int getSubPathwaySize();

    //PieMenuItem* _pieMenu;
	//bool _pieMenuOn;
	//int _pieMenuState;
	//int menuType;
	QPointF pos_menu, scenePos_menu;
	StatisticManager* _manager;
	QRectF bubbleRect;
	QString lastStoredfileName;
	
	vector<vector<float>> AVCRGBs;
	//int m_pathwayID;
	QSizeF getFontSize(int type, int id);

	bool getPartItemBack(PathBubble1 *cBubble, vector<int> itemSelected);

	void deleteItems();
	bool pieMenuOn;
	void ParserTraceSteps();
	void spaceReplace(QString &S);
	void savePathWayforTreeRing(QString filename,  vector<vector<int>> edge);	
	void savePathWayforTreeRing_1(QString filename,  vector<vector<int>> edge);
	void putReactionToCompartment(vector<vector<int>> edge, vector<set<vector<int>>> &CompartmentContain);

    int findTreeNodeID(vector<int> node, vector<vector<int>> nodeSet);
	int findExpressionID(QString Name, vector<vector<QString>> geneInfo, int type);
	QString findEdgeType(int pid1, int type1, int id1, int pid2, int type2, int id2);

	void addtoGraph();
	void removeFromGraph();	

	void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget );	
	void paintGraph(QPainter *painter, bool drawHighlighting=false);

	void renderColorLegend(QPainter *painter);
	float getGraphToBePaint( );
	void deleteSelectedItems(QGraphicsScene *scene);
    void deleteSelectedItemKids(QGraphicsScene *scene);
	bool ifExpressionShown();
	void PathBubbleDelectionUpdate(int m_pathwayID);
    void PathBubbleDelectionUpdate(PathBubble1* b0, PathBubble1* pb);
	void savePathWay(QString filename, vector<vector<int>> edge);
	
	QString getFirstPathWayGraphName();
	QString getPathwayGraphName(deque< QString > pathwayGraphStored);
	QRectF updateBubbleSize_3();
	QRectF updateBubbleSize(); //when first created
	QRectF updateBubbleSize_2(); //when delection and creation of a subbubbleway bubble
	void putReactionToCompartment(vector<vector<vector<int>>> _reaction, vector<set<vector<int>>> &CompartmentContain);
	void putReactionToCompartment(vector<vector<vector<int>>> _reaction, vector<set<vector<int>>> &CompartmentContain, vector<vector<vector<int>>> Ranks, vector<vector<int>> VertexInfo);
	void marchSubGraph(vector<vector<vector<int>>> &edgeSet, set<vector<int>> &nodeSet, set<int> &markedEdge, int eid, int id);
	
	QRectF ResetGraph_1();
	QRectF ResetGraph_2();
	
	float aveFontSize;
	float maxFontSize;

	float L,B,W,H;//,bL,bB,bW,bH;


	void updateVisibleNodes();	
	float getNodeToBePaint();
    void getEdgeToBePaint();

	void getPartUpdateHoveredNodetoBePaint(vector<int> preItemSelected, vector<int> currentItemSelected);
	void getPartUpdateSelectedNodetoBePaint(vector<int> preItemSelected, vector<int> currentItemSelected);
	void getPartUpdateSelectedNodetoBePaintWhenMoved(vector<int> preItemSelected, vector<int> currentItemSelected, QPointF dis);


	void moveGraphToBePaint(QPointF dis);
	void moveEdgeToBePaint(QPointF dis);
	void moveNodeToBePaint(QPointF dis);	
	
	void scaleGraphToBePaint(QPointF dCenter1, float scale1, QPointF dCenter2, float scale2);
	void scaleNodeToBePaint(QPointF dCenter, float scale);
	void scaleEdgeToBePaint(QPointF dCenter1, float scale1, QPointF dCenter2, float scale2);
	float findOverlap(float x, float y, QRectF rect, vector<int> item);
	
	bool drawSmallmolecule;
	char searchType;
	PathBubble1 * _pathBubbleParent;

	set<QString> orthologySymbol;
	set<int> virtualCompartment;
	vector<int> _itemSelected;			
	QList< TextBubble2* > _codeBubbles;
	QList< ReactionBubble3* > _reactionBubbles;
	QList< GroupBubble4* > _groupBubbles;
	QList< subPathBubble1* > _pathBubbles;
	QList< QPointF > _conPoint,_reactionPoint,_groupPoint,_path2Point;
	QList< set <vector <int> > > _conhighlight, _reactionhighlight, _grouphighlight, _path2highlight;
	vector<vector<int>> Rank0; //for compartment
	vector<vector<vector<int>>> Ranks;
	set<vector<int> > reactionRecord; 
	vector<vector<int>> edge;		
	vector<QRectF> complexPos, proteinPos, smallMoleculePos, DnaPos, reactionPos, degradationPos, physicalEntityPos, compartmentPos, EmptyPos, ANodePos; //0-1
	vector<QRectF> complexPos_0, proteinPos_0, smallMoleculePos_0, DnaPos_0, reactionPos_0, degradationPos_0, physicalEntityPos_0, compartmentPos_0, EmptyPos_0, ANodePos_0; 
	vector<QRectF> complexPos_1, proteinPos_1, smallMoleculePos_1, DnaPos_1, reactionPos_1, degradationPos_1, physicalEntityPos_1, compartmentPos_1, EmptyPos_1, ANodePos_1; 
	
	vector<QRectF> boundingBoxes;
	vector<QString> boundingText; //record text for boundingBoxes
	vector<int> boundingFontSize; //record text for boundingBoxes

	vector<QRectF> complexRect, proteinRect, smallMoleculeRect, DnaRect, reactionRect, degradationRect, physicalEntityRect, compartmentRect, EmptyRect,ANodeRect; //real position
	vector<QRectF> fontRect;
	vector<float>  boundaryWidth;
	vector<int> fontSize;
	vector<QColor> boundaryColor, fontColor;
	vector<vector<QColor> > insideColor;
	vector<QPointF> elipseVector;
	vector<vector<QPointF>> arrowVector;

	vector<vector<set<vector<int>>>> itemGrid;	
	vector<vector<int>> _VertexInfo,_Edge;
	vector<vector<vector<int>>> _Vertex; 
	vector<QRectF> _newPos, _newCompartmentPos;
	vector<vector<QPointF>> _polyEdge;
	vector<int> smallMolecule, Dna; //record smallMolecule, Dna appear in reactions, used in multiplySmallMolecule and multiplyDna
	vector<vector<int>> itemInfo;  //subpath way infor for_connections? [0] node type, [1]: node pid, [2] current id, [3] it is in which of this->_pathbubbles, [4] and which of its itemselected node, [5] what is the nodes connected parent right now
	vector<QRectF> itemPos;	
	QList<set<vector<vector<int>>>> _groupEdgeAffected;
	set<vector<vector<int>>> _edgeAffected, _edgeSelected;
	//vector<int> markedD,markedS; //for rearrange their compartmentID;
	vector<set<vector<int>>> CompartmentContain,CompartmentContain_0,CompartmentContain_1;
	vector<set<int>> _1pathway;
	vector<vector<int>> _pathwayStep;
	vector<vector<int>> _3pathwayStepCatalysis; //catalysis- control - Degradation - pathway
	vector<int> _3pathwayStepReaction;	

	//void removeNotContainedinCompartment(vector<set<vector<int>>> CompartmentContain);
	void removeNotContainedinCompartment(vector<set<vector<int>>> &CompartmentContain, vector<QRectF> compartmentPos, vector<QRectF> &complexPos, vector<QRectF> &proteinPos, vector<QRectF> &smallMoleculePos, vector<QRectF> &DnaPos, vector<QRectF> &reactionPos, vector<QRectF> &degradationPos, vector<QRectF> &physicalEntityPos, vector<QRectF> &EmptyPos, vector<QRectF> &ANodePos);
	int findCompartment(int type, int id, QRectF node, vector<QRectF> compartmentPos, int j, float scale1=1, float scale2=1);	
	//bool checkIdentity(int type, vector<vector<QString>> Names, vector<QString> name);
	set<vector<int>>  TwoProteinSetComparison( vector<set<vector<int>>> cItem);
	set<vector<int>> TwoTwoProteinSetComparison( vector<vector<set<vector<int>>>> cItem);
	vector< vector < set< vector <int> > > > searchSharedItems(bool AndFlag);

	 void addSmallMoleculetoItemSelected(vector<int> &itemSelected);
	void HierarchicalTreeNodeLocation(bool whole, vector<vector<int>> _VertexInfo, vector<vector<int>> Rank0, vector<vector<vector<int>>> Ranks);
	void RemoveDisabledNodes(bool whole, vector<vector<int>> _VertexInfo, vector<vector<vector<int>>> _Vertex, vector<vector<int>> &Rank0, vector<vector<vector<int>>> &Ranks);
	void locateCompartmentforExtraItems(vector<vector<QString>> _complexName, vector<vector<QString>>_physicalEntityName, vector<vector<QString>> _proteinName, vector<vector<QString>> _reactionName,	vector<vector<QString>> _smallMoleculeName, vector<vector<QString>> _DnaName, vector<set<vector<int>>> &CompartmentContain, vector<vector<QString>> _compartmentName);
	//set<int> visibleReaction;
	vector<vector<vector<int>>> searchLink(int stype, int sid, int etype, int eid, set<vector<vector<int> > > edgeSet);
	bool checkRectionUniqueness(int pid, int id, set<vector<int>> sharedNodes);
	void searchLinkedNode(vector<PathBubble1 *> mlist,  int pflag, int cflag, int wflag, bool Andflag=true);

    bool checkUniqueness(int type, vector<vector<QString>> Names, vector<QString> name);
	//void pathWayRecord(bool whole, vector<vector<int>> edge, bool drawSmallmolecule=false); //vector<vector<int>> _Edge, 
	bool pathWayRead(vector<vector<int>> &edge, vector<vector<QString>> &_complexName, vector<vector<QString>> &_physicalEntityName, vector<vector<QString>> &_proteinName, vector<vector<QString>> &_reactionName, vector<vector<QString>> &_degradationName, vector<vector<QString>> &_smallMoleculeName, vector<vector<QString>> &_EmptyName, vector<vector<QString>> &_DnaName, vector<vector<QString>> &_ANodeName, vector<vector<QString>> &_compartmentName);
    void pathWayRecord(vector<vector<int>> edge, vector<vector<QString>> _complexName, vector<vector<QString>> _physicalEntityName, vector<vector<QString>> _proteinName, vector<vector<QString>> _reactionName, vector<vector<QString>> _degradationName, vector<vector<QString>> _smallMoleculeName, vector<vector<QString>> _EmptyName, vector<vector<QString>> _DnaName, vector<vector<QString>> _ANodeName, vector<vector<QString>> _compartmentName);//vector<vector<int>> _Edge, 
  
	void pathWayPoiRecord(vector<QRectF> complexPos, vector<QRectF> proteinPos, vector<QRectF> smallMoleculePos, vector<QRectF> DnaPos, vector<QRectF> EmptyPos, vector<QRectF> reactionPos, vector<QRectF> physicalEntityPos, vector<QRectF> compartmentPos, vector<QRectF> ANodePos, vector<set<vector<int>>> CompartmentContain, float _scale, QPointF dCenter, bool drawSmallmolecule=false);
	bool pathWayPoiRead(vector<QRectF> &complexPos, vector<QRectF> &proteinPos, vector<QRectF> &smallMoleculePos, vector<QRectF> &DnaPos, vector<QRectF> &EmptyPos, vector<QRectF> &reactionPos, vector<QRectF> &physicalEntityPos, vector<QRectF> &compartmentPos, vector<QRectF> &ANodePos, vector<set<vector<int>>> &CompartmentContain, float &_scale, QPointF &dCenter, bool drawSmallmolecule=false);
	void smallMoleculetoNewName(vector<vector<QString> > &_smallMoleculeName, vector<int> smallMoleculeNameID);
	bool newSmallmolecularNameRecord(vector<int> smallMoleculeNameID);
    bool newSmallmolecularNameRead(vector<vector<QString>> &_smallMoleculeName);//, vector<int> smallMoleculeNameID)
	//record pathway
	
	vector<vector<set<vector<int>>>> recordItemGrid(int &visibleItemNum, int &visibleCompartmentNum);	
	void multiplyDna(vector<vector<QString> > &_DnaName);
	void multiplySmallMolecule(vector<vector<QString> > &_smallMoleculeName);
	void redrawPathWay(void);
	void redrawPathWay(bool drawSmallmolecule);
	void redrawPathWay_1(vector<vector<int>> _Catalysis, vector<vector<int>> _5Control);
	
	int stepNum, reactionNum, proteinNum, complexNum, physicalEntityNum, smallMoleculeNum, DnaNum, EmptyNum,degradationNum, compartmentNum, ANodeNum;
	bool _whole;
	//bool controlKeyisOn;
	bool isSameNode(int pid1, int type1, int id1, int pid2, int type2, int id2);

	void searchOrthology(QString name);
	void findRank_up_backup(int id, int rank, vector<vector<int>> &VertexInfo, vector<int> length, vector<vector<int>> Edge, vector<vector<vector<int>>> Vertex, int roomID);
	void findRank_down_backup(int id, int rank, vector<vector<int>> &VertexInfo, vector<int> length, vector<vector<int>> Edge, vector<vector<vector<int>>> Vertex, int roomID);
	void reOrganizeSubMap(vector<vector<int>> &Rank, vector<QRectF> &newPos, vector<vector<vector<int>>> Vertex, vector<vector<int>> VertexInfo, vector<vector<int>> Edge);
	bool inRanks(int id, vector<vector<int>> Rank, vector<vector<vector<int>>> Vertex);
	void rewriteTreeRingGraphFile();
	bool PathWayDataInitiation();
	bool loadPreStoredGraph();
	bool readOriPathWayData(vector<vector<QString>> &_complexName, vector<vector<QString>> &_physicalEntityName, vector<vector<QString>> &_proteinName, vector<vector<QString>> &_reactionName, vector<vector<QString>> &_degradationName, vector<vector<QString>> &_smallMoleculeName, vector<vector<QString>> &_EmptyName, vector<vector<QString>> &_DnaName, vector<vector<QString>> &_ANodeName) ;
	void shrinkData(
	vector<QRectF> &complexPos_0, vector<QRectF> &proteinPos_0, vector<QRectF> &smallMoleculePos_0, vector<QRectF> &DnaPos_0, vector<QRectF> &EmptyPos_0, vector<QRectF> &reactionPos_0, vector<QRectF> &physicalEntityPos_0, vector<QRectF> &compartmentPos_0, vector<QRectF> &ANodePos_0, vector<set<vector<int>>> &CompartmentContain_0,
	vector<QRectF> &complexPos_1, vector<QRectF> &proteinPos_1, vector<QRectF> &smallMoleculePos_1, vector<QRectF> &DnaPos_1, vector<QRectF> &EmptyPos_1, vector<QRectF> &reactionPos_1, vector<QRectF> &physicalEntityPos_1, vector<QRectF> &compartmentPos_1, vector<QRectF> &ANodePos_1, vector<set<vector<int>>> &CompartmentContain_1,	
	vector<vector<QString>> &_complexName, vector<vector<QString>> &_physicalEntityName, vector<vector<QString>> &_proteinName, vector<vector<QString>> &_reactionName, vector<vector<QString>> &_degradationName, vector<vector<QString>> &_smallMoleculeName, vector<vector<QString>> &_EmptyName, vector<vector<QString>> &_DnaName, vector<vector<QString>> &_ANodeName,
	vector<vector<int>> &edge);//save a copy of data which only contains those possible shown in the graph (remove those not shown in the graph)

	void GraphSize(float &X, float &Y, float &W, float &H);
	void PathWayDataInitiation_1();//for generate treering without generating the hierarchical pathway graph
	vector< vector < set< vector <int> > > >  getContainedSets(int pflag, int cflag, int wflag, bool AndFlag);
	vector< vector < set< vector <int> > > >  getContainedSets_1(int pflag, int cflag, int wflag, bool isSearchDiffered);
	
	void searchDifferedNode(int ctype, vector<vector<vector<QString>>> nodeName, vector<vector<QRectF>> nodePos, set<vector<int>> sID, vector<set<vector<vector<int>>>> edgeSet, vector<set<vector<int>>> nodeSet, vector<set<vector<int>>> &cItem, vector<set<vector<int>>> &markedNodes,  vector<set<int>> &sharedNode);
	void searchSharedNode(int ctype, vector<vector<vector<QString>>> nodeName, vector<vector<QRectF>> nodePos, set<vector<int>> sID, vector<set<vector<vector<int>>>> edgeSet, vector<set<vector<int>>> nodeSet, vector<set<vector<int>>> &cItem, vector<set<vector<int>>> &markedNodes,  vector<set<int>> &sharedNode, bool toSharedNode=false);
	void searchSharedCompartment(int ctype, vector<vector<vector<QString>>> nodeName, vector<vector<QRectF>> nodePos, set<vector<int>> sID, vector<set<vector<vector<int>>>> edgeSet, vector<set<vector<int>>> nodeSet, vector<set<vector<int>>> &cItem, vector<set<vector<int>>> &markedNodes,  vector<set<int>> &sharedNode);
	void searchSharedPathway(int ctype, vector<vector<vector<QString>>> nodeName, vector<vector<QRectF>> nodePos, set<vector<int>> sID, vector<set<vector<vector<int>>>> edgeSet, vector<set<vector<int>>> nodeSet, vector<set<vector<int>>> &cItem, vector<set<vector<int>>> &markedNodes,  vector<set<int>> &sharedNode);

	bool isSameReaction(int pid1, int rid1, int pid2, int rid2);

	

	void findRank_up(int id, int rank, vector<vector<int>> &VertexInfo, vector<int> length, vector<vector<int>> Edge, vector<vector<vector<int>>> Vertex, int roomID);
	void findRank_down(int id, int rank, vector<vector<int>> &VertexInfo, vector<int> length, vector<vector<int>> Edge, vector<vector<vector<int>>> Vertex, int roomID);
	void markLongestPath(int id, vector<vector<int>> &VertexInfo, vector<int> &path, vector<int> length, vector<vector<int>> Edge, vector<vector<vector<int>>> Vertex);
	int findLongestPath(int id, vector<int> &length, vector<vector<int>> Edge, vector<vector<vector<int>>> Vertex, vector<vector<int>> &VertexInfo, int roomID, bool down=false);

	bool compareSubgraph(set<vector<vector<int>>>subEdge, set<vector<int>> subNode, set<vector<int>> &markedNode, vector<int> item0, vector<int> item1, int cid);
	bool compareSubgraph(vector<set<vector<vector<int>>>> edgeSet, vector<set<vector<int>>> nodeSet, set<vector<int>> &markedNode, vector<int> item0, vector<int> item1, int level);
	void searchSubgraph( set<vector<int>> &sID, vector<set<vector<vector<int>>>> &eSet, vector<set<vector<int>>> &nodeSet);
	PathBubble1* searchSubPathBubble(subPathBubble1* subbubble, int pid, int &jid);

	int visibleItemNum, visibleCompartmentNum;

	int getVisibleItemNum() 
	{
	   return visibleItemNum;
	}

	bool emptyPathBubble();
	bool isItemVisible_1(int type, int id);
	bool isItemVisible(int type, int id);
    bool myErrorMessage(QString text);

	int getVisibleCompartmentNum()
	{
	   return visibleCompartmentNum;
	}

	void setExpressionName(QString name)
	{
	    _expressionName = name;
	}

    QString _labelName;
    
	bool _mousePressed;
	int getVerticeID_2(int type, int id);
	int getVerticeID_3(int type, int id);

	int whichCompartment(int pid, int type, int id);
    int whichCompartment(int pid, vector<int> item);
	void setAItemColored(int type, int id, int up_or_down);
	queue<vector<int>> findSameItem(int pid1, vector<int> item0);

	void updateACompartmentPos(int id, QPointF move, QPointF Move);
    void menuSelection(QPointF pos, QPointF scenePos, int index);

	void searchsharedP(vector<PathBubble1 *> mlist, vector< vector < set< vector <int> > > > cItem);
	void searchsharedPforLink(vector<PathBubble1 *> mlist, vector< vector < set< vector <int> > > > cItem, vector<set<int>> &sharedNode);
	bool menuSelection_1(QPointF pos, QPointF scenePos, int index);

	
	void setEdgeAffected(vector<int> itemSelected, set<vector<vector<int>>> &edgeSelected, set<vector<vector<int>>> &edgeAffected);
	bool verifyItemSelected(vector<int> itemSelected);
	bool verifyItemSelectedforCompartment(vector<int> itemSelected);
	
	bool isHighlightorshared();
	void saveAsXmlNode( QDomDocument &doc, QDomElement &node );
	void recoverFromXmlNode( QDomElement node );
	void paintItem(QPainter *painter, int type, int i, bool flag, QPointF dCenter, float scale, QColor c, QPointF dis);
	
	void savePathWayforOthers(QString filename, vector<QRectF> complexPos, vector<QRectF> proteinPos, vector<QRectF> smallMoleculePos, vector<QRectF> DnaPos, vector<QRectF> reactionPos, vector<QRectF> physicalEntityPos, vector<QRectF> ANodePos, bool solveANode=false); //same as the void savePathWay() except the edge is recorded in different way
	void savePathWay(QString filename, vector<QRectF> complexPos, vector<QRectF> proteinPos, vector<QRectF> smallMoleculePos, vector<QRectF> DnaPos, vector<QRectF> reactionPos, vector<QRectF> physicalEntityPos, vector<QRectF> ANodePos, bool solveANode=false);
	void savePathWayinJSFile(QString filename, vector<QRectF> complexPos, vector<QRectF> proteinPos, vector<QRectF> smallMoleculePos, vector<QRectF> DnaPos, vector<QRectF> reactionPos, vector<QRectF> physicalEntityPos, vector<QRectF> ANodePos, vector<vector<int>> edge);

	bool readPathWay(QString filename, vector<QRectF> &complexPos, vector<QRectF> &proteinPos, vector<QRectF> &smallMoleculePos, vector<QRectF> &DnaPos, vector<QRectF> &reactionPos, vector<QRectF> &physicalEntityPos, vector<QRectF> &ANodePos, vector<vector<int>> &edge);
    QString createFeatureGeoJSON(char type, int id, QString name, QRectF rect ); 
	QString createFeatureGeoJSON(int id, char stype, int sid, QPointF start, char etype, int eid, QPointF end); 

	vector<vector<vector<int>>> EdgetoNode(vector<vector<int>> edge, vector<vector<vector<int>>> &_protein, vector<vector<vector<int>>> &_complex, vector<vector<vector<int>>> &_entity, vector<vector<vector<int>>> &_smallMolecule, vector<vector<vector<int>>> &_Dna, vector<vector<vector<int>>> _reaction, bool whole,  vector<QRectF> reactionPos1, bool flag=false);//flag if read graph from .hie file
	vector<vector<int>> EdgetoNode(vector<vector<int>> edge, vector<vector<vector<int>>> & compartment, bool flag=false);//flag : if read graph from .hie file
	void HierarchicalLayout(bool whole, vector<QRectF> reactionPos1, bool flag=false); //flag: if read graph from .hie file
	void updateNodeLocation(vector<vector<vector<int>>> Ranks, vector<vector<int>> VertexInfo, int pid, vector<vector<set<vector<int>>>> &CompartmentContain, vector<set<vector<int>>> &CompartmentContain_1);

	void averageNodes(vector<vector<int>> _VertexInfo, vector<QRectF> &_newPos);
	vector<vector<int>> subMap(vector<vector<vector<int>>> Vertex, vector<vector<int>> VertexInfo, vector<vector<int>> Edge, vector<QRectF> &newPos, vector<set<vector<int>>> &CompartmentContain, int roomID);
	//void subMap_backup(vector<vector<vector<int>>> Vertex, vector<vector<int>> VertexInfo, vector<vector<int>> Edge, vector<QRectF> &newPos, int roomID);
	vector<vector<vector<int>>> _protein, _complex, _entity, _smallMolecule, _Dna, _reaction, _compartment, _Empty,_ANode;//recode edge?

	void mergeGraph(int id1, int id2);

	bool clearColored();
	bool clearExpressed();
	bool clearExpressed(int m_expressionID);
	bool clearShared();
	bool clearDiffered();
	bool clearLinked();
	void storeEvent(int eventType);
	//void storeEvent(int eventType);

	void searchSharedCompartment(set<vector<int>> sID, int pflag, int cflag, int wflag);
	bool sharedDifferedLinkedExpressedtoHighlighted(bool toHighlight=true, bool isShare=true, bool isDiffer=true, bool is_Link=true, bool is_Expressed=true);
	void cleanPathHighlight();
	void sharedNodetoHighlighted(bool toHighlight=true);	
	void updateSharedLinkedDIffered();
	void searchSharedItems(vector<PathBubble1 *> mlist);
	void highlightedNodetoStored();
	void sharedDifferedLinkedExpressedHighLightedSizeCheck();
	void searchedNodetoStored(int eventType);
	//void HighlightedForward();
	bool isHighlightedChanged();
	bool updateGraphIfHighlightedChanged();

	//void searchSharedProtein(set<vector<int>> sID, int pflag, int cflag, int wflag);
	//void searchSharedComplex(set<vector<int>> sID, int pflag, int cflag, int wflag);
	//void searchSharedDna(set<vector<int>> sID, int pflag, int cflag, int wflag);
    //void searchSharedSmallMolecule(set<vector<int>> sID, int pflag, int cflag, int wflag);
    //void searchSharedPhysicalEntity(set<vector<int>> sID, int pflag, int cflag, int wflag);
    //void searchSharedReaction(set<vector<int>> sID, int pflag, int cflag, int wflag);
	
    vector<set<vector<int>>> searchSharedNode( set<vector<int>> sID, int pflag, int cflag, int wflag, bool Andflag=true);
	vector<set<vector<int>>> searchDifferedNode( set<vector<int>> psID, int pflag, int cflag, int wflag);
	void PN(vector<PathBubble1 *> mlist, vector< vector < set< vector <int > > > > cItem, vector< vector < set< vector <int> > > > cItem1);
    void PU(vector<PathBubble1 *> mlist, vector< vector < set< vector <int > > > > cItem);
	void PAL(vector<PathBubble1 *> mlist);
	void PAN(vector<PathBubble1 *> mlist);
	void PAU(vector<PathBubble1 *> mlist);
	void CL(vector<PathBubble1 *> mlist);
	void CN(vector<PathBubble1 *> mlist, vector< vector < set< vector <int> > > > cItem);
	void CU(vector<PathBubble1 *> mlist, vector< vector < set< vector <int> > > > cItem);
	void PL(vector<PathBubble1 *> mlist);

	vector<set<vector<int>>> getHighlightedBackuped();
	void reMoveNotShared(vector<set<vector<int>>> HBackup);
	 
	void updateLabel();
	
	QString _pathName;
	
	QString getCurrentFilename()
	{
		return _curFileName;
	}

	QString getPathwayName()
	{
		return _pathName;
	}

	void rebuildBubbleConnect( QDomElement node );
	void setCurrentFile(const QString &fileName);
	void loadFile(const QString &fileName);
	

	//int m_pathwayID;
	vector<vector<int>> edgeSelected;
	//OpenGLScene* _scene;	
	OpenManager* _open;
	set<int> stepTracked;	

	virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
	virtual void wheelEvent(QGraphicsSceneWheelEvent *event);
	virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
	virtual void keyPressEvent(QKeyEvent *event);
	virtual void keyReleaseEvent(QKeyEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

	void getGraphSize(float &width, float &height);
	void clearUngroup()
	{	
		this->AFTERUNGROUP = false;	
			this->UNGROUP = false;
			
	}
	void clearSketch()
	{
	    this->_lines.clear();
	
	}

	void preLocateNodes(vector<QRectF> &complexPos, vector<QRectF> &proteinPos, vector<QRectF> &smallMoleculePos);	
	
	void trackstep(int id, vector<vector<int>> _pathwayStep, vector<vector<int>> _3pathwayStepCatalysis, vector<int> _3pathwayStepReaction, vector<vector<vector<int>>> _biochemicalReaction);
	
    void setParentPathWayID(int id)
	{
	   _parentPathBubbleID=id;
	}
	
	float _scale;
	//void clearSelected();
    //void clearExpressed();
	//void clearUndoRedoRecord();
	
	void unGroupbyDelection();
	void selfDelection_1(PathBubble1* b0, PathBubble1* pb);

public slots:
	void selfDelection(PathBubble1* b0, PathBubble1* pb);
	
protected:
	void manuLocateCompartment(vector<QRectF> &Compartment)	;
	void recordNodeinRelationship(set<vector<int>> psID, vector<set<vector<int>>> &cItem, set<vector<int>> &sID, vector<set<vector<vector<int>>>> &edgeSet, vector<set<vector<int>>> &nodeSet);
	QRectF findSparseLocation(QRectF rect);
    vector<set<vector<int>>> searchSharedNodeForSearchLinked( set<vector<int>> psID, bool Andflag);

private:
	bool completeSame;
	QString _orthologyFileName, _expressionName, _orthName;
	//vector<QRectF> complexDragged, proteinDragged, smallMoleculeDragged, DnaDragged, reactionDragged, entityDragged;
	int whichCompartment(set<vector<int>> highlighted);
    //int whichCompartment(int Pid, int type, int id);

	vector<QRectF> itemDragged;	
	//vector<vector<int>> _8convertedProtein, _8convertedComplex, _8convertedEntity;
	vector<int> itemSelected;	
	QVector<QLineF> fills;
	vector<vector<int>> _Catalysis, _5Control;	
	//vector<int> XL,YB,XR,YT;
	//vector<vector<int>> _Catalysis;
	//vector<vector<int>> _5Control;	
	//vector<vector<int>> _stepRoot; //store the steps
	
		
	QString getCompartmentNames(vector<int> itemSelected);
	void compartmentToItemSelected(int mid, vector<int> &itemSelected);
	//void compartmentToItemHighlighted(int mid, set<vector<int>> &highlighted);
	void compartmentToItemHighlighted(int mid, set<vector<int>> &highlighted);

	void testforceDirected(QPainter *painter, QRectF Compartment);
	void testforceDirected_2(QPainter *painter, QRectF Compartment);
	//void findAllPathes(int source, int target, int totalnode, int totaledge, vector<vector<int>> GRAPH, vector<vector<int>> &pathVector);
	vector<vector<int>> getCurrentGraph(vector<vector<int>> edge);
	vector< set< vector< vector<int> > > > findPathforTwoNodes(int type1, int id1, int type2, int di1, vector<vector<int>> edge);	
	//int linkSearchType; //0: search all pathes from a start, 1: search pathes for two nodes

	void forceDirected();

	

	/*Vertex get_vertex(const std::string& name, Graph& g, NameToVertex& names)
    {
    NameToVertex::iterator i = names.find(name);
    if (i == names.end())
    i = names.insert(std::make_pair(name, add_vertex(name, g))).first;
    return i->second;
    };*/
    
	//void clearSharedSets(); //only findPathforTwoNodesclean the set in current group;

		
	//vector<QRectF> Compartment;
	//vector<QRectF> CompartmentPos;
		
	//void readAPathway(const char *aPathway);
	//vector<vector<int>> _complexContain;

	QString _curFileName;
	QString _curFilePath;	
	
	bool draggingItem;// groupReadyforDragging;	
	QPointF initPos;	
	
	QPolygonF _contain;	
	int dragout; //1: drag an item out of the region, 2: drag an compartment out of region		
	
	void processComplexContains(vector<vector<int>> &_complexContain);
	void deleteGroup();

	int read1pathway(const char *name, vector<set<int>> &_1pathway);
	
	bool updateNodeNums(vector<vector<int>> edge, int &proteinNum, int &complexNum, int &physicalEntityNum, int &smallMoleculeNum, int &DnaNum,int &emptyNum, vector<vector<QString>> &_proteinName, vector<vector<QString>> &_complexName, vector<vector<QString>> &_physicalEntityName, vector<vector<QString>> &_smallMoleculeName, vector<vector<QString>> &_DnaName,  vector<vector<QString>> &_emptyName, vector<vector<QString>> _compartmentName, vector<set<vector<int>>> &CompartmentContain, vector<QRectF> &proteinPos, vector<QRectF> &complexPos, vector<QRectF> &physicalEntityPos, vector<QRectF> &smallMoleculePos, vector<QRectF> &DnaPos, vector<QRectF> &EmptyPos);
	bool addANode(int type, int newNum, int &nodeNum, vector<vector<QString>> &nodeName, vector<vector<QString>> _compartmentName, vector<set<vector<int>>> &CompartmentContain, vector<QRectF> &nodePos);

	/*int read7protein(const char *name, vector<vector<QString>> &_proteinName);
	void read8convertedEntity(const char *name, int &proteinNum, int &complexNum, int &physicalEntityNum, vector<vector<int>> &_8convertedProtein, vector<vector<int>> &_8convertedComplex, vector<vector<int>> &_8convertedEntity);
	void read9smallEntity(const char *name, vector<vector<QString>> & _smallMoleculeName, vector<vector<QString>> &_DnaName);*/
	//void read9smallEntity(const char *name, vector<vector<QString>> & _smallMoleculeName, vector<vector<QString>> &_DnaName, vector<set<vector<int>>> &CompartmentContain, vector<vector<QString>> _compartmentName, int &smallMoleculeNum, int &DnaNum);
	void getNameforConverted(vector<vector<int>> _8convertedProtein, vector<vector<int>> _8convertedComplex, vector<vector<int>> _8convertedEntity, vector<vector<QString>> &_complexName, vector<vector<QString>> &_proteinName, vector<vector<QString>> &_physicalEntityName);
	vector<vector<vector<int>>> recordEdge(vector<vector<vector<int>>> _biochemicalReaction);
	bool PathtoRank(vector<vector<vector<int>>> Vertex, vector<vector<int>> &VertexInfo, vector<vector<int>> Edge, vector<QRectF> &newPos, vector<set<vector<int>>> &CompartmentContain, vector<vector<int>> &Rank, vector<int> &path, int &mini, int roomID, bool useBoost);

	bool addedtoGraph;
	bool highLightedtoItemSelected(set<vector<int>> highlighted, vector<int> &itemSelected);
	void addItemSelectedtoHighLighted(vector<int> itemSelected, set<vector<int>> &highlighted);

	set<vector<int>> findNeighorsForAProtein(int type, int id);
	QPointF getLocation(int pathwayid, int stepid);
    QRectF LayoutBiochemicalReaction(vector<vector<vector<int>>> _biochemicalReaction, int bid, int sid);
	QRectF LayoutDegradation(vector<vector<vector<int>>> _degradation, int bid, int sid);	

	int whichPathway(int id);		
	vector<vector<int>> findSpannedEdge(set< vector < vector<int> > > edgeconnections, vector<int> node);
	void ExtendLinkedNodesSize();
	
	void updateItemGrid(vector<vector<set<vector<int>>>> &itemGrid, int type, int id, QPointF newPos);
	void manuLocateNodes(vector<QRectF> &complexPos, vector<QRectF> &proteinPos, vector<QRectF> &smallMoleculePos);
	
	vector<int> whichItem( QPointF pos, float width, float height);	
	vector<vector<int>> whichItem( QRectF pos, float width, float height);
	QSizeF getItemSize( int type, int id);
	QSizeF getItemSize(QString name, int type, int id);

	void showIterm( QPointF iPos, QPointF pos, QPointF scenePos, QString name);
	void resizeItem_1( int type, int id, QPointF Pos, QPointF lastPos);
	QRectF resizeNode( QRectF oriRect, QRectF curRect, QRectF rect);
	QRectF rectToPos(QRectF rect);

	void drawABubbleConnection(QPainter *painter, QPointF center1, PathBubble1* path1, QPointF center2, PathBubble1* path2, QPointF Dis);
	void drawABubbleConnectioninArrow(QPainter *painter, QPointF center1, PathBubble1* path1, QPointF center2, PathBubble1* path2, QPointF Dis, QColor color, int arrowType, int encode=0, float h=0);
	void drawABubbleConnectioninArrow_H(QPainter *painter, QPointF center1, PathBubble1* path1, QPointF center2, PathBubble1* path2, QPointF Dis, QColor color, int arrowType, int encode=0, float h=0, float _scale=1.0);
	

	int getNodeID(int type, int id);


	QString pathWayGraphForward();
	QString pathWayGraphBackward();
	int isInResizeArea(const QPointF pos, QRectF rect, int type=-1);
	int whichCorner(const QPointF pos, QRectF rect);
	bool isInnerResizing;	
	//QTextEdit* m_note_1;

	//interactions
	void mouseMoveEvent( QGraphicsSceneMouseEvent *event );
	void dragItem(QPointF Pos);
	void setAItemDragged(int type, int id);

	void paintItemDragged(QPainter *painter);	
	void drawCurve( QPainter* painter );
	void drawfills( QPainter* painter );
    void addLine( QPointF pos1, QPointF pos2 );
	bool groupSelection(QVector<QLineF> _lines);
	void setItemDragged(vector<int> itemSelected);

	void checkGroupConnections(vector<int> &itemSelected, vector<vector<int>> &edgeSelected);
	bool updateAItemPos(int type, int id, QPointF move);
	bool updateGroupedItemPos(vector<int> itemSelected, QPointF move);
	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
	void dropDownMenu2(QGraphicsSceneContextMenuEvent *event);
	void dropDownMenu3(QGraphicsSceneContextMenuEvent *event);
	void dropDownMenu4(QGraphicsSceneContextMenuEvent *event);	
	//bool _LABEL;
};

#endif
