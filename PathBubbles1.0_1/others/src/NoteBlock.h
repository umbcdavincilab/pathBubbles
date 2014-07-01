#ifndef NOTEBLOCK_H
#define NOTEBLOCK_H

#include <QTextEdit>
#include <QGraphicsProxyWidget>
#include "TextNote.h"
#include "qtbox.h"
 
class NoteBlock : public ItemBase
{
public:
	NoteBlock(int size, int x, int y, OpenGLScene* Scene);
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	void resizeItem( int size1, int size2 );
	void initUI( OpenGLScene* Scene );
	void saveAsXmlNode( QDomDocument &doc, QDomElement &node );
	void recoverFromXmlNode( QDomElement node );
	void logOpen();

protected:
	void mousePressEvent( QGraphicsSceneMouseEvent *event );
	void mouseReleaseEvent( QGraphicsSceneMouseEvent *event );

private:
	OpenGLScene* _scene;
	QTextEdit* m_note;
};

#endif