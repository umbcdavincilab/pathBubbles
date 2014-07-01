#ifndef SCATTERBUBBLE_H
#define SCATTERBUBBLE_H

#include "plotBase.h"

class ScatterBubble : public PlotBase
{
	Q_OBJECT

public:
	ScatterBubble( int size, int x, int y, OpenGLScene* Scene, StatisticManager* manager, QString name );
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

#endif //SCATTERBUBBLE_H