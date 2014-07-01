#ifndef VOLUMBLOCK_H
#define VOLUMBLOCK_H

#include <qtbox.h>
#include "Bat.h"
#include "Discreptor.h"

class OpenGLScene;

#define VOLUMSIZE 200

class VolumBlock : public ItemBase
{
	Q_OBJECT
public:
	VolumBlock( int size, int x, int y, OpenGLScene* scene, Bat* dat );
	void acceptDiscreptor( DESCRIPTOR dis )
	{
		this->_bat->getCamber()->acceptDiscreptor( dis );
	}
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
protected:
	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
	void mousePressEvent( QGraphicsSceneMouseEvent *event );
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent( QGraphicsSceneMouseEvent *event );
	void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
private:
	Bat* _bat;
	OpenGLScene* _scene;
	bool _ONCONTROL;
};
#endif