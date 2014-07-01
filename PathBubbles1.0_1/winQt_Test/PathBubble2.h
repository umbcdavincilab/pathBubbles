#ifndef PATHBUBBLE2_H
#define PATHBUBBLE2_H

#include <QTextEdit>
#include "plotBase.h"
#include "openglscene.h"
#include "OpenManager.h"

class PathBubble2 : public PlotBase
{
	//Q_OBJECT
public:
	PathBubble2( int size, int x, int y, OpenGLScene* Scene, StatisticManager* manager, OpenManager* open, QString name );
	void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget );	
	void loadFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);

private:
	OpenGLScene* _scene;	
	OpenManager* _open;	
	QTextEdit* _text;

	void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

	void resizeItem(int size1, int size2 );
	void mouseMoveEvent( QGraphicsSceneMouseEvent *event );	
	void textItemPosMoved( QPointF offset );
};

#endif