#ifndef QTSM_H
#define QTSM_H

#include <qtbox.h>
#include <SM.h>
#include <GL/glut.h>
#include "animateit.h"
//#include "qtbat.h"
//#include "BatDataView.h"

class OpenGLScene;
//class BatDataView;
class QtSM : public ItemBase
{
	Q_OBJECT

public:
	QtSM(int size, int x, int y, OpenGLScene* Scene);
	virtual ~QtSM();
	static QString checkPath( QString name );
	static QString checkPathWayPath( QString name );
	void setSM( SM* input);
	SM* getSM()
	{
		return sm;
	}
	void setAnimation( QPoint Distination, int size)
	{
		QPointF pt = this->pos();
		ani->start( QPoint( pt.x(), pt.y() ), Distination, m_size_width, size);
	}
	void updateFont()
	{
		sm->size_hint( (float)m_size_width/(float)MAX_ITEM_SIZE );
	}
	bool between( int tar, int left, int right)
	{
		if( tar > left && tar < right)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	void setHightlightState( bool state )
	{
		_NEEDHI = state;
	}
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	QString getName()
	{
		QString str = QString::fromStdString(sm->getName());
		return str;
	}
	void saveAsXmlNode( QDomDocument &doc, QDomElement &node );
	void recoverFromXmlNode( QDomElement node );

	void logOpen();
	void logSwapBars( int st, int ed );
	void logHighlight( QList<int> selected );
	void pressHightLight( QRectF rect );
	void moveHightLight( QRectF rect );
	void releaseHighLight();

public slots:
	void switchColorMethod();
	void needSwapBars( int source, int target );

signals:
	void updateLabelState( ItemBase *item );
	void updatePCColorState( ItemBase *item, int input );
	void HighlightChangeInGroup( ItemBase *item, bool sta );
	void pressHightlightInGroup( ItemBase *item, QRect rect );
	void moveHightlightInGroup( ItemBase *item, QRectF rect );
	void releaseHightlightInGroup( ItemBase *item );

protected:
	void mousePressEvent( QGraphicsSceneMouseEvent *event );
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent( QGraphicsSceneMouseEvent *event );
	void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

private:
	int half( int input )
	{
		return (int)((float)input / 2.0 + 0.5);
	}
	void updateControlPos();
	//void tryToLinkWithBat();
	void updateLabel();
	SM* sm;
	void initFloatingUI();
	QPointF controlLastPos;
	int lastSize_width, lastSize_height;
	bool ONDRAG;
	bool _ONCONTROL;
	bool _NEEDHI;
	QRectF _highLighted;
	QList<int> enableHighlight();
	void disableHighlight();
	animateIt* ani;
	OpenGLScene* scene;
	QPointF newPos;
	//QtBat* _linked;
	bool LINKED;
	bool _DISRECT;
	QRadioButton *radioButForInd;
	QRadioButton *radioButForGlo;

	bool _LABEL;

	//BatDataView* rawDataView;
	bool _RAWDATAVIEW;
	QRectF nameRect;
};

#endif // QTSM_H
