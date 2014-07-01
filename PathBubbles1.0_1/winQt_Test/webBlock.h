#ifndef WEBBLOCK_H
#define WEBBLOCK_H

#include "qtbox.h"
#include "webQuery.h"
#include "SearchFind.h"
#define WEBSIZE 200

class OpenGLScene;
class SearchFind;
class WebBlock : public ItemBase
{
	Q_OBJECT
public:
	//void resetOriRect();
	//void scaleUpBasedonOriRect();
	//void scaleDownBasedonOriRect();
	void deleteItems();
	//WebBlock::~WebBlock();
	WebBlock( int size, int x, int y, OpenGLScene* scene, int type=1, QString target = tr(""));
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);


void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
	void initUI(int type);
	void resizeItem( int size1, int size2 );
	void saveAsXmlNode( QDomDocument &doc, QDomElement &node );
	void recoverFromXmlNode( QDomElement node );
	void logOpen();
	
	WebQuery * _web;
	class SearchFind * _search; //_blocktype 1
	class SearchFind * _settings; //_blocktype 2



protected:
	//QRectF _oriRect; 
	//float _curScale; 
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	void mousePressEvent( QGraphicsSceneMouseEvent *event );
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent( QGraphicsSceneMouseEvent *event );
	void hoverMoveEvent(QGraphicsSceneHoverEvent *event);

private:
	
	QString _target;
	//OpenGLScene* _scene;

};
#endif