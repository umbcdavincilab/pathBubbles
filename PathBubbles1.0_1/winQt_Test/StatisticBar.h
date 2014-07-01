#ifndef STATISTICBAR_H
#define STATISTICBAR_H

#include <QStringList>
#include "BarBase.h"
#include "StatisticManager.h"
#include "animateIt.h"
#include "PenText.h"
#include "PenItem.h"
#include <QImage>
#include <QScrollArea>

class StatisticBar : public BarBase
{
	Q_OBJECT

public:
	StatisticBar( QSizeF size, QPoint pos, StatisticManager* manager );
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	void initNameItem();
	void drawItems( QPainter *painter );
	void updateSize( QSizeF newsize );

	QString tailofLogMsg();
	void logHide();
	void logDeleteItem( QString str );

protected:
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	void keyPressEvent(QKeyEvent *event);
	void wheelEvent(QGraphicsSceneWheelEvent *event);
	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
public slots:
	void indexAccepted( int index );
	void addParameterFromMat( QString pName, QString run_num, QList<double> dataResult );
	void updateDataFromMat( QString pName, QList<double> dataResult ); 
signals:
	void needStatisticalData( QPointF pos, StatisticManager* manager, int index );
private:
	void mouseMoveOnItems( QPointF pos );
	void drawDrag( QPainter* painter );
	void drawWholeRect( QPainter* painter );
	void drawControlRect( QPainter* painter );
	void reOrderItems();
	
	StatisticManager* _manager;
	QList<PenItem*> _1Ditems;
	QList<PenItem*> _1DTitems;
	QList<PenItem*> _1DitemsFromMat;
	QList<PenItem*> _1DTitemsFromMat;
	int _curState;
	int _curSelectedParameter;
	int _selectedParameter;
	QRectF _curSelectedRect;
	bool _displayDrag;
	bool ONMOVE;
	QRectF _control;

};

#endif