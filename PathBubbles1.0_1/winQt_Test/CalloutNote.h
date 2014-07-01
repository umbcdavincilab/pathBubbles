#ifndef CALLOUTNOTE_H
#define CALLOUTNOTE_H

#include <QTextEdit>
#include <QGraphicsProxyWidget>
#include <QGraphicsTextItem>
//#include "TextNote.h"
//#include "3DHelper.h"
#include "qtbox.h"
 
class CalloutNote : public ItemBase
{
public:	
	QTextCursor myCursor;
	QRectF _deleteMark,_minimizeMark;
	vector< vector <int> > nodeIDs;
	int m_noteID;
	int _pid, _type, _id, _cid;	
	QString _preText;//_preTime;
	QString _fontColor;
	vector< QString > nodeText;
	QString lastInputTime;
	QString getText(int pid, int type, int id);
	void storeText(int pid, int type, int id, bool plain=true);
    int getNodeID(vector< vector <int> > nodeIDs, int pid, int type, int id);
	void record(QString pathName, bool ifDeleted=false);	int getColorByUserName(QString userName);
	bool isVisible() {  return m_note->isVisible(); }

	QImage m_labelImage;
	CalloutNote(int pid, int type, int id, int size, int x, int y, OpenGLScene* Scene);
	//CalloutNote::~CalloutNote();
	bool compare(QString &preText, QString &curText);
	void updateNode(int pid, int type, int id, bool setVisible=true);
	void updateNodeFirstTime(int pid, int type, int id, bool setVisible=true);
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	void resizeItem( int size1, int size2 );
	void drawNoteNodeConnector(QPainter *painter, bool visible);
	void initUI( OpenGLScene* Scene );
	void saveAsXmlNode( QDomDocument &doc, QDomElement &node );
	void recoverFromXmlNode( QDomElement node );
	bool onBoarder( QPointF pos );
	void logOpen();
	QTextEdit* m_note;
	QRectF crossRect, minusRect;
	QRectF markRect;
	float _size;
protected:
	void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
	void mousePressEvent( QGraphicsSceneMouseEvent *event );
	void mouseReleaseEvent( QGraphicsSceneMouseEvent *event );
	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);	
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);	
    void mouseMoveEvent( QGraphicsSceneMouseEvent *event );
private:
	//OpenGLScene* _scene;
	
};

#endif
