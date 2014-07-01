#ifndef MATDATAVISBLOCK_H
#define MATDATAVISBLOCK_H

#include "plotBase.h"

class OpenGLScene;

class MatDataVisBlock : public PlotBase
{
	Q_OBJECT

public:
	MatDataVisBlock( int size, int x, int y, OpenGLScene* scene );
	void paint( QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *widget );
	void plotXY( QPainter* painter );
	void setCodeLine( QString str );
	void saveAsXmlNode( QDomDocument &doc, QDomElement &node );
private:
	void drawBoarder( QPainter* painter );
	bool onBoarder( QPointF pos );

	QString codeLine;
};

#endif  //MATDATAVISBLOCK_H