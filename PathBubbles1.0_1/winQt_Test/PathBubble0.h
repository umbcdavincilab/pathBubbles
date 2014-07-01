#ifndef PATHBUBBLE0_H
#define PATHBUBBLE0_H

#include "plotBase.h"
#include "openglscene.h"
#include "OpenManager.h"

class PathBubble0 : public PlotBase
{
	Q_OBJECT

public:
	PathBubble0( int size, int x, int y, OpenGLScene* Scene, StatisticManager* manager, OpenManager* open, QString name );
	void saveAsXmlNode( QDomDocument &doc, QDomElement &node );
	void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget );	
	

private:
	//OpenGLScene* _scene;	
	OpenManager* _open;
	vector<int> XL,YB,XR,YT;
	int highlighted;
	void readPathWays(const char *pathWays);
	void resizeFont(QString aText, QString fontType , int &fontSize, int style, int width, int height);
    void paintPathWays(QPainter *painter, vector<QString> PathWayNames);
	bool insideRect(int x, int y, int xl, int yb, int xr, int yt);
    int whichTerm( QPointF pos);	
	int pathwaySelected;
	void showPathway( QPointF pos, QPointF scenePos);
	void hoverMoveEvent(QGraphicsSceneHoverEvent *event);

	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

};

#endif