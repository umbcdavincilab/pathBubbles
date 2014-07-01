#ifndef PATHBUBBLE1_H
#define PATHBUBBLE1_H

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

class PathBubble1 : public PlotBase
{
	Q_OBJECT
public:
	void deleteItems();
	QString _name;
	PathBubble1( int size, int x, int y, OpenGLScene* Scene, StatisticManager* manager, OpenManager* open, QString name );
	PathBubble1::~PathBubble1();
	void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget );	
	void deleteSelectedItems(QGraphicsScene *scene);
    void deleteSelectedItemKids(QGraphicsScene *scene);
	PathBubble1 * _pathBubbleParent;
	QList< TextBubble2* > _codeBubbles;
	QList< ReactionBubble3* > _reactionBubbles;
	QList< GroupBubble4* > _groupBubbles;
	QList< QPointF > _conPoint,_reactionPoint,_groupPoint,_path2Point;
	QList< set <vector <int> > > _conhighlight, _reactionhighlight, _grouphighlight, _path2highlight;
	QPointF dCenter;
	//vector<int> _reactionType;

	void multiplyDna();
void multiplySmallMolecule();

vector<vector<int>> _VertexInfo,_Edge;
vector<vector<vector<int>>> _Vertex; 
vector<QRectF> _newPos, _newCompartmentPos;

vector<vector<QPointF>> _polyEdge;

vector<int> smallMolecule,Dna;


void findRank_up_backup(int id, int rank, vector<vector<int>> &VertexInfo, vector<int> length, vector<vector<int>> Edge, vector<vector<vector<int>>> Vertex, int roomID);
void findRank_down_backup(int id, int rank, vector<vector<int>> &VertexInfo, vector<int> length, vector<vector<int>> Edge, vector<vector<vector<int>>> Vertex, int roomID);


void findRank_up(int id, int rank, vector<vector<int>> &VertexInfo, vector<int> length, vector<vector<int>> Edge, vector<vector<vector<int>>> Vertex, int roomID);
void findRank_down(int id, int rank, vector<vector<int>> &VertexInfo, vector<int> length, vector<vector<int>> Edge, vector<vector<vector<int>>> Vertex, int roomID);
void markLongestPath(int id, vector<vector<int>> &VertexInfo, vector<int> &path, vector<int> length, vector<vector<int>> Edge, vector<vector<vector<int>>> Vertex);
int findLongestPath(int id, vector<int> &length, vector<vector<int>> Edge, vector<vector<vector<int>>> Vertex, vector<vector<int>> &VertexInfo, int roomID);

	bool _mousePressed;
	int getVerticeID_2(int type, int id);

	void setAItemColored(int type, int id, int up_or_down);
	QList<set<vector<vector<int>>>> _groupEdgeAffected;
	

	set<vector<vector<int>>> _edgeAffected, _edgeSelected;
	void setEdgeAffected(vector<int> itemSelected, set<vector<vector<int>>> &edgeSelected, set<vector<vector<int>>> &edgeAffected);
	bool varifyItemSelected(vector<int> itemSelected);
	
	void saveAsXmlNode( QDomDocument &doc, QDomElement &node );
	void recoverFromXmlNode( QDomElement node );
	void paintItem(QPainter *painter, int type, int i, bool flag, QPointF dCenter, float _scale, QColor c);
	void savePathWay(QString filename, vector<QRectF> complexPos, vector<QRectF> proteinPos, vector<QRectF> smallMoleculePos, vector<QRectF> DnaPos, vector<QRectF> reactionPos, vector<QRectF> physicalEntityPos, vector<vector<int>> edge);
	void savePathWayinJSFile(QString filename, vector<QRectF> complexPos, vector<QRectF> proteinPos, vector<QRectF> smallMoleculePos, vector<QRectF> DnaPos, vector<QRectF> reactionPos, vector<QRectF> physicalEntityPos, vector<vector<int>> edge);

	void readPathWay(QString filename, vector<QRectF> &complexPos, vector<QRectF> &proteinPos, vector<QRectF> &smallMoleculePos, vector<QRectF> &DnaPos, vector<QRectF> &reactionPos, vector<QRectF> &physicalEntityPos, vector<vector<int>> &edge);
    QString createFeatureGeoJSON(char type, int id, QString name, QRectF rect ); 
	QString createFeatureGeoJSON(int id, char stype, int sid, QPointF start, char etype, int eid, QPointF end); 

	vector<vector<vector<int>>> EdgetoNode(vector<vector<int>> edge, vector<vector<vector<int>>> &_protein, vector<vector<vector<int>>> &_complex, vector<vector<vector<int>>> &_entity, vector<vector<vector<int>>> &_smallMolecule, vector<vector<vector<int>>> &_Dna, vector<vector<vector<int>>> &_reaction, bool whole);
	vector<vector<int>> EdgetoNode(vector<vector<int>> edge, vector<vector<vector<int>>> & compartment);
	void HierarchicalLayout(bool whole);

	vector<vector<int>> subMap(vector<vector<vector<int>>> Vertex, vector<vector<int>> VertexInfo, vector<vector<int>> Edge, vector<QRectF> &newPos, int roomID);
	void subMap_backup(vector<vector<vector<int>>> Vertex, vector<vector<int>> VertexInfo, vector<vector<int>> Edge, vector<QRectF> &newPos, int roomID);
	vector<vector<vector<int>>> _protein, _complex, _entity, _smallMolecule, _Dna, _reaction, _compartment, _Empty;
	void searchSharedProtein();
	void updateLabel();
	bool onBoarder( QPointF pos );
	

	QString getCurrentFilename()
	{
		return _curFileName;
	}
	void rebuildBubbleConnect( QDomElement node );
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
   
	int getVerticeID(int type, int id);
	void getItemID(int ID, int &type, int &id);
	OpenGLScene* _scene;	
	OpenManager* _open;
	vector<int> XL,YB,XR,YT;
	int stepNum, reactionNum, proteinNum, complexNum, physicalEntityNum, smallMoleculeNum, DnaNum, EmptyNum;
	//vector<QRectF> Compartment;
	//vector<QRectF> CompartmentPos;
	
	
	//void readAPathway(const char *aPathway);
	vector<set<int>> _1pathway;
	vector<vector<int>> _2pathwayStep;
	vector<vector<int>> _3pathwayStepCatalysis; //catalysis- control - Degradation - pathway
	vector<int> _3pathwayStepReaction;
	
	vector<vector<int>> _5Catalysis;
	vector<vector<int>> _5Control;
	
	vector<vector<int>> _6complexContain;

	QString _curFileName;
	QString _curFilePath;

	float _scale;
	
	vector<vector<int>> _8entity;
	vector<vector<int>> _8convertedProtein, _8convertedComplex, _8convertedEntity;

	vector<vector<int>> edge;	
	
	vector<int> itemSelected;
	vector<vector<int>> edgeSelected;	
	bool draggingItem;// groupReadyforDragging;
	set<int> stepTracked;	
	QPointF initPos;
	QVector<QLineF> fills;
	QPolygonF _contain;
	bool groupSelected;
	int m_pathwayID;

	vector<QRectF> complexPos, proteinPos, smallMoleculePos, DnaPos, reactionPos, physicalEntityPos, compartmentPos,EmptyPos;	
	//vector<QRectF> complexDragged, proteinDragged, smallMoleculeDragged, DnaDragged, reactionDragged, entityDragged;
	vector<QRectF> itemDragged;	

	int read1pathway(const char *name, vector<set<int>> &_1pathway);
	int read2pathwayStepOrder(const char *name, int stepNum, vector<vector<int>> &_2pathwayStep);
	void read3pathwayStep_reactionAndcontrol(const char *name, int stepNum, vector<vector<int>> &_3pathwayStepCatalysis, vector<int> &_3pathwayStepReaction);
	vector<int> read4biochemicalReaction(const char *name, vector<vector<vector<int>>> &_4biochemicalReaction, vector<vector<QString>> &_4reactionName, vector<vector<vector<int>>> &_4degradation, vector<vector<QString>> &_4degradationName, vector<vector<QString>> &_4EmptyName);
	void read5catalysisANDcontrol(const char *name, vector<vector<int>> &_5Catalysis, vector<vector<int>> &_5Control);
	int read6complex(const char *name, vector<QString> &_6complexName, vector<vector<int>> &_6complexContain);
	int read7protein(const char *name, vector<vector<QString>> &_7proteinName);
	void read8convertedEntity(const char *name, int &proteinNum, int &complexNum, int &physicalEntityNum, vector<vector<int>> &_8convertedProtein, vector<vector<int>> &_8convertedComplex, vector<vector<int>> &_8convertedEntity);
	void read9smallEntity(const char *name, vector<vector<QString>> & _9smallMoleculeName, vector<vector<QString>> &_9DnaName);
	void getNameforConverted(vector<vector<int>> _8convertedProtein, vector<vector<int>> _8convertedComplex, vector<vector<int>> _8convertedEntity, vector<QString> &_6complexName, vector<vector<QString>> &_7proteinName, vector<vector<QString>> &_8physicalEntityName);
	vector<vector<vector<int>>> recordEdge(vector<vector<vector<int>>> _4biochemicalReaction);
	
	QPointF getLocation(int pathwayid, int stepid);
    QRectF LayoutBiochemicalReaction(vector<vector<vector<int>>> _4biochemicalReaction, int bid, int sid);
	void LayoutCatalysis(vector<vector<int>> _5Catalysis,  QRectF rPos, int cid, int sid);
	void LayoutControl(vector<vector<int>> _5Catalysis,  QRectF rPos, int cid, int sid);

	void trackstep(int id, vector<vector<int>> _2pathwayStep, vector<vector<int>> _3pathwayStepCatalysis, vector<int> _3pathwayStepReaction, vector<vector<vector<int>>> _4biochemicalReaction);
	int whichPathway(int id);
	vector<vector<set<vector<int>>>> recordItemGrid( );
	void updateItemGrid(vector<vector<set<vector<int>>>> &itemGrid, int type, int id, QPointF newPos);
	
	void wheelEvent(QGraphicsSceneWheelEvent *event);
	void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	void manuLocateCompartment(vector<QRectF> &Compartment)	;
	void manuLocateNodes(vector<QRectF> &complexPos, vector<QRectF> &proteinPos, vector<QRectF> &smallMoleculePos);
	void preLocateNodes(vector<QRectF> &complexPos, vector<QRectF> &proteinPos, vector<QRectF> &smallMoleculePos);	
	
	vector<int> whichItem( QPointF pos, float width, float height);
	vector<vector<set<vector<int>>>> itemGrid;	
	QSizeF getItemSize( int type, int id);

	void showIterm( QPointF iPos, QPointF pos, QPointF scenePos, QString name);
	void drawConnections(QPainter *painter);

	//interactions
	void mouseMoveEvent( QGraphicsSceneMouseEvent *event );
	void dragItem(QPointF Pos);
	void setAItemDragged(int type, int id);

	void paintItemDragged(QPainter *painter);	
	void drawCurve( QPainter* painter );
	void drawfills( QPainter* painter );
    void addLine( QPointF pos1, QPointF pos2 );
	bool groupSeletion(QVector<QLineF> _lines);
	void setItemDragged(vector<int> itemSelected);

	void checkGroupConnections(vector<int> &itemSelected, vector<vector<int>> &edgeSelected);
	void updateAItemPos(int type, int id, QPointF move);
	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

	bool _LABEL;
};

#endif