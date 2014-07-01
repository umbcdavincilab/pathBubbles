#ifndef LINECHARTBUBBLE_H
#define LINECHARTBUBBLE_H

#include "plotBase.h"

class LineChartBubble : public PlotBase
{
	Q_OBJECT

public:
	LineChartBubble( int size, int x, int y, OpenGLScene* Scene, StatisticManager* manager, QString name );
	void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget );
	void plotXY( QPainter *painter );
	void saveAsXmlNode( QDomDocument &doc, QDomElement &node );
	void recoverFromXmlNode( QDomElement node );
	void logOpen();

protected:
	void hoverMoveEvent( QGraphicsSceneHoverEvent *event );

private:
	QString batName;

};

#endif //LINECHARTBUBBLE_H