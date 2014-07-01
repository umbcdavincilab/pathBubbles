#ifndef PathBubbleBase_H
#define PathBubbleBase_H


#include "plotBase.h"
#include "openglscene.h"
#include "OpenManager.h"
#include <set>
#include <vector>

using namespace std;
//class TextBubble2;
//class GroupBubble4;
//class PathBubble2;
//class ReactionBubble3;
//class progress_cooling;
//typedef boost::adjacency_list<boost::listS, boost::vecS, boost::undirectedS> MyGraph;
//typedef boost::graph_traits<MyGraph>::vertex_descriptor MyVertex;


class PathBubbleBase : public PlotBase
{
	Q_OBJECT
public:
	//void deleteItems();
	QString _name;		
	PathBubbleBase( int size, int x, int y, OpenGLScene* Scene, StatisticManager* manager=NULL, OpenManager* open=NULL, QString name="");
	virtual PathBubbleBase::~PathBubbleBase();	

	virtual void paint( QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *widget );	
		
	QString getCurrentFilename()
	{
		return _curFileName;
	}
	
	void setCurrentFile(const QString &fileName);
	void loadFile(const QString &fileName);

private:
    //vector<int> XL,YB,XR,YT;

	QString _curFileName;
	QString _curFilePath;

		
	QPointF initPos;
	QVector<QLineF> fills;
	QPolygonF _contain;
	bool groupSelected;

	//vector<QRectF> complexPos, proteinPos, smallMoleculePos, DnaPos, reactionPos, physicalEntityPos;	
	//vector<QRectF> complexDragged, proteinDragged, smallMoleculeDragged, DnaDragged, reactionDragged, entityDragged;
	vector<QRectF> itemDragged;
	//vector<vector<QString>>  _9smallMoleculeName, _9DnaName;

		
	//interactions
	
	void drawCurve( QPainter* painter );
	void drawfills( QPainter* painter );
    void addLine( QPointF pos1, QPointF pos2 );
	

};

#endif