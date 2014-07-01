#ifndef ARROWGEN_H
#define ARROWGEN_H

#include "BarBase.h"
#include "OpenManager.h"
#include <QImage>

enum RNAME {NONE, SPEED, MASS, SPHERE, CYLINDER, ARROW};

class ArrowGen : public BarBase
{
	Q_OBJECT

public:
	ArrowGen(QSizeF size, OpenGLScene* scene);
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	RNAME getCurrentData()
	{
		return _currentData;
	}
	RNAME getSphereInfor()
	{
		return _sphereInfor;
	}
	RNAME getCylinderInfor()
	{
		return _cylinderInfor;
	}
	QString tailofLogMsg();
	void logDragTo( QString s1, QString s2 );
	void logDragToScene( QString str, QPointF pos );

protected:
 	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
 	void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
 	void mousePressEvent(QGraphicsSceneMouseEvent *event);
 	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

signals:
	void setWingShapePara(ArrowGen* aGen, QPointF pos);

private:
	void drawDataArea( QPainter* painter );
	void drawGraphicsArea( QPainter* painter );
	void drawDragRect( QPainter* painter );
	
	QRectF getDataArea();
	QRectF getDataArea_Shadow();
	QRectF getGraphicsArea();
	QRectF getGraphicsArea_Shadow();
	QRectF getArrowArea();
	QRectF getArrowArea_Shadow();

	bool inDataArea( QPointF pos );
	bool inGraphicsArea( QPointF pos );
	bool inSpeedArea( QPointF pos );
	bool inMassArea( QPointF pos );
	bool inSphereArea( QPointF pos );
	bool inCylinderArea( QPointF pos );
	bool inArrowArea( QPointF pos );

	void initGraphicsItem();
	void initDataArea();
	void initGItemArea();

	OpenGLScene* _scene;
	QList<QRectF> _areaData;
	QList<QRectF> _areaData_Shadow;
	QList<QRectF> _rectsGItem;
	QList<QRectF> _areaGItem;
	QImage GItem[2];
	RNAME _hoverRect;
	bool _ONMOVE;
	RNAME _currentData;
	QRectF _dragRect;
	RNAME _sphereInfor;
	RNAME _cylinderInfor;
};

#endif //ARROWGEN_H