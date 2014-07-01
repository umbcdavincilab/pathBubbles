#ifndef BARCHARTBUBBLE_H
#define BARCHARTBUBBLE_H

#include "plotBase.h"

class BarChartBubble : public PlotBase
{
	Q_OBJECT

public:
	BarChartBubble( int size, int x, int y, OpenGLScene* Scene, StatisticManager* manager, QString name );
	void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget );
	void plotXY( QPainter *painter );
	void saveAsXmlNode( QDomDocument &doc, QDomElement &node );
	void recoverFromXmlNode( QDomElement node );
	void logOpen();

protected:
	void hoverMoveEvent( QGraphicsSceneHoverEvent *event );

private:
	void MouseOverBars( QPointF pos );

private:
	QString batName;
};

#endif