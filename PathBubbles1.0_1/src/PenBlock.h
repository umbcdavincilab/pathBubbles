#ifndef PENBLOCK_H
#define PENBLOCK_H

#include <QImage>
#include "qtbox.h"
#include "VolumBlock.h"
#include "PenDrawing.h"
#include "ScriptEngin.h"
#include "PenItem.h"

class OpenGLScene;
class OpenManager;

#define PENBLOCK 200

class PenBlock : public ItemBase
{
	Q_OBJECT

public:
	PenBlock( int x, int y, OpenGLScene* scene, OpenManager* openmanager );
	~PenBlock();
	void AcceptDrawing( PenDrawing* drawing, float camber );
	void logOpen();
	void logSearch();

protected:
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	void mousePressEvent( QGraphicsSceneMouseEvent *event );
	void mouseReleaseEvent( QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent( QGraphicsSceneMouseEvent *event );
	void hoverMoveEvent( QGraphicsSceneHoverEvent *event);
	void keyPressEvent( QKeyEvent * event );

signals:
	void addBatsToScene( QList<QList<ItemBase*>> bats );
	void updateLabelState( ItemBase *item );

public slots:
	void needSlider( QList<QList<ItemBase*>> bats, QList<QList<float>> rates );

private:
	void updateLabel();
	void clickItem();
	void releaseItem();
	void initEngin();
	void runScript();
	void handleEnter();
	void handleSpace();
	void handleBackspace();
	void handleKeyBoard( QKeyEvent* event );
	void reOrderItems();
	void paintDrawing( QPainter *painter );
	void drawRun( QPainter* painter );
	bool highLight( QPointF pos );
	bool onExisted( QPointF pos );
	void setHighlight();
	OpenGLScene* _scene;
	VolumBlock* _volum;
	PenDrawing* _currentDrawing;
	bool VOLUMED;
	bool _ONEXISTED;
	bool _ONACTION;
	bool _ONADDPOINT;
	bool _ONRUN;
	bool _NEEDNEW;
	int _EXISTINDEX;
	bool _ONCREATING;
	QList<PenItem*> _curLine;
	QList<QList<PenItem*>> _lines;
	QPointF initPos;
	QPoint currentIndex;
	int _PreviousSize;
	//
	ScriptEngin* _engin;
	OpenManager* _openManager;
	//
	QImage _run;
	QRectF _rectRun;
	int GLOBAL_INDEX;

	bool _LABEL;
};

#endif