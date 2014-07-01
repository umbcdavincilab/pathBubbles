#ifndef WEBBLOCK_H
#define WEBBLOCK_H

#include "qtbox.h"
#include "webQuery.h"

class OpenGLScene;
#define WEBSIZE 200
class WebBlock : public ItemBase
{
	Q_OBJECT
public:
	WebBlock( int x, int y, OpenGLScene* scene, QString target = tr("") );
	void initUI();
	void resizeItem( int size1, int size2 );
	void saveAsXmlNode( QDomDocument &doc, QDomElement &node );
	void recoverFromXmlNode( QDomElement node );
	void logOpen();

protected:
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	void mousePressEvent( QGraphicsSceneMouseEvent *event );
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent( QGraphicsSceneMouseEvent *event );
	void hoverMoveEvent(QGraphicsSceneHoverEvent *event);

private:
	WebQuery* _web;
	QString _target;
	OpenGLScene* _scene;
};
#endif