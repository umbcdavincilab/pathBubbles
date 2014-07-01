#ifndef EXPRESSIONBUBBLE_H
#define EXPRESSIONBUBBLE_H

#include "PathBubbleBase.h"
#include <set>
#include <vector>

using namespace std;

class TextBubble2;
class GroupBubble4;
class ReactionBubble3;
class PathBubbleBase;


//typedef boost::adjacency_list<boost::listS, boost::vecS, boost::undirectedS> MyGraph;
//typedef boost::graph_traits<MyGraph>::vertex_descriptor MyVertex;


class expressionBubble : public PlotBase
{
	Q_OBJECT
public:
	void deleteItems();
	int _layoutType;
	void setExpressedGenePos(int orderType);
	set<int> matchedGene_TreeRing_Expression, expresseGeneLinkedtoTreeRing;	
	expressionBubble( int size, int x, int y, OpenGLScene* Scene, StatisticManager* manager, OpenManager* open, QString name );
	expressionBubble::~expressionBubble();
	void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget );	
	void deleteSelectedItems(QGraphicsScene *scene);
    void deleteSelectedItemKids(QGraphicsScene *scene);
	void clearExpressed();
	void clearTreeExpressed();
	void clearSearched();
	void clearSharedExpression();
	void clearHighlighted();
	void checkExpressedProteinSize(int m_bindPathwayID); //update the storage
	void checkSharedExpressionProteinSize(int m_bindPathwayID);
	void checkExpressedProteinSize();
	void TransferExpression(int m_bindPathwayID);
	bool outsideBubble( QPointF pos );
	void search(QString str);
	bool isExpressedMatchedGene(int i);

	void undo();
    void redo();
	
	expressionBubble * _pathBubbleParent;

	vector<QRectF> bgRect;
	vector<QColor> bgColor, borderColor, htColor, fontColor;
	vector<int> borderWidth;
	vector<vector<QRectF>> htRect;
	vector<int> fontSize;
	vector<QPointF> fontPos;
	int hoverGeneOrderID;
	void drawHighLightNode(QPainter *painter, int hl);
	void drawHoveredSymbol(QPainter *painer, int hl);

	set<QString> matchedPathways;
	vector<struct expressedPathwayInfo> expressedPathways;

	vector<QString> quantityName;
	vector<vector<QString>> geneInfo;
	vector<vector<float>> quantity, minmax;
	//QList< TextBubble2* > _codeBubbles;
	//QList< ReactionBubble3* > _reactionBubbles;
	//QList< GroupBubble4* > _groupBubbles;
	//QList< QPointF > _conPoint,_reactionPoint,_groupPoint,_path2Point;
	QList< set <vector <int> > > _conhighlight, _reactionhighlight, _grouphighlight, _path2highlight;
	vector<set<int>> expressedProtein, expressedComplex, expressedGene,sharedExpressionProtein,sharedExpressionGene;
	
	set<vector<int>> coloredNode;
	vector<vector<int>> geneContainer;
	vector<QRectF> expressedGenePos;

	bool isSharedGene(int type, QString n1, QString n2, int excludePid=-1); //used to judge whether the node match the shared expressed gene across bubble in a group
	void checkSharedExpressedProtein(int Pid, int Eid, QString n1, QString n2, int type, int j, int excludePid=-1);
	
	QRectF nodeRect(QRectF nodeRect, bool fixedSize,  QPointF dCenter, float scale, bool enLarge=false);
	QPointF textPos(QPointF nodePos, QRectF nodeRect, bool fixedSize,  QPointF dCenter, float scale, bool Enlarge=false);

	bool expressionCleared;
	int m_bindPathwayID;
	float exMax,exMin; //expression level

	//vector<int> _reactionType;
	//vector<vector<int>> _VertexInfo,_Edge;
	//vector<vector<vector<int>>> _Vertex; 
	//vector<QRectF> _newPos, _newCompartmentPos;
	//vector<vector<QPointF>> _polyEdge;
	

	//vector<int> smallMolecule,Dna;
	
	bool _mousePressed;
	int getVerticeID_2(int type, int id);

	void setAItemColored(int pid, int type, int id, int up_or_down);	
	void searchExpressedProtein(int Pid, int excludePid=-1, bool shareflag=true);
	void searchExpressedProtein(char type='P', int excludePid=-1, bool shareflag=false); //search whole
	void searchSharedProteininAPathway(int m_bindPathwayID);
	void searchExpressedGene(int m_bindPathwayID);
	void checkSharedProteinSize(int m_bindPathwayID);

    //set<vector<vector<int>>> _edgeAffected, _edgeSelected;
	bool verifyItemSelected(vector<int> itemSelected);
	void searchSharedProtein(int excludePid=-1, bool shareflag=true);
		
	void saveAsXmlNode( QDomDocument &doc, QDomElement &node );
	
	void paintItem(QPainter *painter, int type, int i, bool flag, QPointF dCenter, float _scale, QColor c);
	//void savePathWay(QString filename, vector<QRectF> complexPos, vector<QRectF> proteinPos, vector<QRectF> smallMoleculePos, vector<QRectF> DnaPos, vector<QRectF> reactionPos, vector<QRectF> physicalEntityPos, vector<vector<int>> edge);
	//void savePathWayinJSFile(QString filename, vector<QRectF> complexPos, vector<QRectF> proteinPos, vector<QRectF> smallMoleculePos, vector<QRectF> DnaPos, vector<QRectF> reactionPos, vector<QRectF> physicalEntityPos, vector<vector<int>> edge);
	int getIDinExpressedSets(QString name);
	//void paintExpression(QPainter *painter, int m_bindPathwayID);
	void getExpressionToBePaint(vector<QRectF> &bgRect, vector<QColor> &bgColor, vector<QColor> &borderColor, vector<QColor> &htColor, vector<int> &borderWidth, vector<vector<QRectF>> &htRect, vector<int> &fontSize, vector<QColor> &fontColor, vector<QPointF> &fontPos, bool fixedSize, QPointF dCenter, float scale, int excludePid=-1);
	void moveExpressionToBePaint(QPointF dis);
    
	void moveExpressionToBePaint(QPointF dis, vector<QRectF> &bgRect, vector<vector<QRectF>> &htRect, vector<QPointF> &fontPos, vector<int> &fontSize);
    void resizeExpressionToBePaint(QRect oriRect);
	void resizeExpressionToBePaint(QRect oriRect, vector<QRectF> &bgRect, vector<vector<QRectF>> &htRect, vector<QPointF> &fontPos, vector<int> &fontSize);

	void scaleExpressionToBePaint(QPointF dCenter, float scale);
  
	//excludePid: for the pathway that is not to be considered (e. g. in the ungroup() )
	void scaleExpressionToBePaint(vector<QRectF> &bgRect, vector<vector<QRectF>> &htRect, vector<QColor> &fontColor, vector<QPointF> &fontPos, QPointF dCenter, float scale);


	void paintExpression(QPainter *painter, vector<QRectF> bgRect, vector<QColor> bgColor, vector<QColor> borderColor, vector<QColor> htColor, vector<int> borderWidth, vector<vector<QRectF>> htRect, vector<int> fontSize, vector<QPointF> fontPos);
	void getExpressionToBePaint(int excludePid=-1);	
    void renderColorLegend(QPainter *painter);
	//void paintSharedExpression(QPainter *painter, int m_bindPathwayID);

	//QString createFeatureGeoJSON(char type, int id, QString name, QRectF rect ); 
	//QString createFeatureGeoJSON(int id, char stype, int sid, QPointF start, char etype, int eid, QPointF end); 

	//void subMap_backup(vector<vector<vector<int>>> Vertex, vector<vector<int>> VertexInfo, vector<vector<int>> Edge, vector<QRectF> &newPos, int roomID);
	vector<vector<vector<int>>> _protein, _complex, _entity, _smallMolecule, _Dna, _reaction, _compartment, _Empty;
	bool isPathwayExpressed(int pid);
	void getCompartementExpressed(int pid);
	//void searchSharedProtein();
	void updateLabel();
	bool onBoarder( QPointF pos );
	
	QString getCurrentFilename()
	{
		return _curFileName;
	}
	//void rebuildBubbleConnect( QDomElement node );
	void setCurrentFile(const QString &fileName);
	void loadFile(const QString &fileName);

private:

	void testforceDirected(QPainter *painter, QRectF Compartment);
	void testforceDirected_2(QPainter *painter, QRectF Compartment);
	void forceDirected();

	/*Vertex get_vertex(const std::string& name, Graph& g, NameToVertex& names)
    {
    NameToVertex::iterator i = names.find(name);
    if (i == names.end())
    i = names.insert(std::make_pair(name, add_vertex(name, g))).first;
    return i->second;
    };*/
   
	
	OpenManager* _open;

	int stepNum, reactionNum, proteinNum, complexNum, physicalEntityNum, smallMoleculeNum, DnaNum, EmptyNum,degradationNum;
	
	//void readAPathway(const char *aPathway);
	//vector<set<int>> _1pathway;
	//vector<vector<int>> _pathwayStep;
	//vector<vector<int>> _3pathwayStepCatalysis; //catalysis- control - Degradation - pathway
	//vector<int> _3pathwayStepReaction;	
	//vector<vector<int>> _Catalysis;
	//vector<vector<int>> _5Control;
	//vector<vector<int>> _stepRoot; //store the steps

	vector<vector<int>> _complexContain;
	vector<int> itemSelected;
	vector<QPointF> expressedGeneTextPos;
	vector<vector<QRectF>> expressedGeneTextColumeRect;
	vector<int> expressedGeneOrder;
	
	QString _curFileName;
	QString _curFilePath;
	float _scale;
	
	//vector<vector<int>> _8entity;
	//vector<vector<int>> _8convertedProtein, _8convertedComplex, _8convertedEntity;
	//vector<vector<int>> edge;		
	
    
	//vector<vector<int>> edgeSelected;	
	bool draggingItem;// groupReadyforDragging;
	//set<int> stepTracked;	
	QPointF initPos;
	

	//vector<QRectF> complexPos, proteinPos, smallMoleculePos, DnaPos, reactionPos, degradationPos, physicalEntityPos, compartmentPos,EmptyPos;		
	//vector<QRectF> itemDragged;	

	/*int read1pathway(const char *name, vector<set<int>> &_1pathway);
	int read2pathwayStepOrder(const char *name, int stepNum, vector<vector<int>> &_pathwayStep);
	void read3pathwayStep_reactionAndcontrol(const char *name, int stepNum, vector<vector<int>> &_3pathwayStepCatalysis, vector<int> &_3pathwayStepReaction);
	vector<int> read4biochemicalReaction(const char *name, vector<vector<vector<int>>> &_biochemicalReaction, vector<vector<QString>> &_reactionName, vector<vector<vector<int>>> &_degradation, vector<vector<QString>> &_degradationName, vector<vector<QString>> &_EmptyName);
	void read5catalysisANDcontrol(const char *name, vector<vector<int>> &_Catalysis, vector<vector<int>> &_5Control);
	int read6complex(const char *name, vector<QString> &_complexName, vector<vector<int>> &_complexContain);
	int read7protein(const char *name, vector<vector<QString>> &_proteinName);
	void read8convertedEntity(const char *name, int &proteinNum, int &complexNum, int &physicalEntityNum, vector<vector<int>> &_8convertedProtein, vector<vector<int>> &_8convertedComplex, vector<vector<int>> &_8convertedEntity);
	void read9smallEntity(const char *name, vector<vector<QString>> & _smallMoleculeName, vector<vector<QString>> &_DnaName);
	*/
	void getNameforConverted(vector<vector<int>> _8convertedProtein, vector<vector<int>> _8convertedComplex, vector<vector<int>> _8convertedEntity, vector<QString> &_complexName, vector<vector<QString>> &_proteinName, vector<vector<QString>> &_physicalEntityName);
	vector<vector<vector<int>>> recordEdge(vector<vector<vector<int>>> _biochemicalReaction);


	/*QPointF getLocation(int pathwayid, int stepid);
    QRectF LayoutBiochemicalReaction(vector<vector<vector<int>>> _biochemicalReaction, int bid, int sid);
	QRectF LayoutDegradation(vector<vector<vector<int>>> _degradation, int bid, int sid);
	void LayoutCatalysis(vector<vector<int>> _Catalysis,  QRectF rPos, int cid, int sid);
	void LayoutControl(vector<vector<int>> _Catalysis,  QRectF rPos, int cid, int sid);

	void trackstep(int id, vector<vector<int>> _pathwayStep, vector<vector<int>> _3pathwayStepCatalysis, vector<int> _3pathwayStepReaction, vector<vector<vector<int>>> _biochemicalReaction);
	int whichPathway(int id);
	*/

	vector<vector<set<vector<int>>>> recordItemGrid( );
	void updateItemGrid(vector<vector<set<vector<int>>>> &itemGrid, int type, int id, QPointF newPos);
	
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
	void wheelEvent(QGraphicsSceneWheelEvent *event);
	void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);	
	void manuLocateNodes(vector<QRectF> &complexPos, vector<QRectF> &proteinPos, vector<QRectF> &smallMoleculePos);
	void preLocateNodes(vector<QRectF> &complexPos, vector<QRectF> &proteinPos, vector<QRectF> &smallMoleculePos);	
	
	void showIterm( QPointF iPos, QPointF pos, QPointF scenePos, QString name);	
	//interactions
	void mouseMoveEvent( QGraphicsSceneMouseEvent *event );
	
	/*
	void dragItem(QPointF Pos);
	void setAItemDragged(int type, int id);
	void paintItemDragged(QPainter *painter);	
	void drawCurve( QPainter* painter );
	void drawfills( QPainter* painter );
    void addLine( QPointF pos1, QPointF pos2 );	
	void setItemDragged(vector<int> itemSelected);
	void updateAItemPos(int type, int id, QPointF move);
	void setEdgeAffected(vector<int> itemSelected, set<vector<vector<int>>> &edgeSelected, set<vector<vector<int>>> &edgeAffected);
	*/

	void checkGroupConnections(vector<int> &itemSelected, vector<vector<int>> &edgeSelected);
	
	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

	bool _LABEL;
};

#endif