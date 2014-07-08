#ifndef SUBPATHBUBBLE1_H
#define SUBPATHBUBBLE1_H

#include "PathBubble1.h"
#include <set>
#include <vector>

using namespace std;


class subPathBubble1 : public PathBubble1
{
	Q_OBJECT

public:
	subPathBubble1(QString orthName, int pathwayID, int sizex, int sizey, int x, int y, vector<int> ItemSelected, vector<vector<int>> EdgeSelected, vector<set<vector<int>>> ANodeContains, OpenGLScene* Scene, StatisticManager* manager, OpenManager* open, QString name, QString pathName=tr(""), QString lastStoredGraph=tr(""));	
	subPathBubble1::~subPathBubble1();
	void saveAsXmlNode( QDomDocument &doc, QDomElement &node );
	
	
	void StoreInitSubPathWay();
	//id convert
    vector<int> complexID, proteinID, smallMoleculeID, DnaID, reactionID, physicalEntityID, ANodeID;	

	//PathBubble1 * pathBubbleParent;
	QPointF Dis, Dis2; //the distance each node is moved toward center. 
	vector<vector<int>> edgeSelected;
    QPointF initItemSize(vector<int> ItemSelected, bool mflag=true, int pathwayID=0, int m_pathwayID=0, bool useCurrentPos=false);	
	QRect updateBubbleSize_2();
	QRect updateBubbleSize_3();
	void paintItem(QPainter *painter);
	void paint( QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *widget );
	//void deleteSelectedItems(QGraphicsScene *scene);
	void deleteSelectedItemsbyParent(QGraphicsScene *scene);
	void rebuildBubbleConnect( QDomElement node );
	//QList< subPathBubble1* > _pathBubbles;

	//void drawConnections( QPainter *painter );

   void wheelEvent(QGraphicsSceneWheelEvent *event);
	void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	void subPathWay(vector<int> ItemSelected, vector<vector<int>> EdgeSelected);
	void manuLocateCompartment(int parentID, int pID, vector<QRectF> &CompartmentPos);
	

public slots:
	void selfDelection(PathBubble1* b0, PathBubble1* pb);


private:
	QPointF X0Y0, X1Y1;
};

#endif