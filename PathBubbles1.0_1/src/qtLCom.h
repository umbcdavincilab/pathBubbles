#ifndef QTLCOM_H
#define QTLCOM_H

#include <qtbox.h>
#include "qtbat.h"
#include <GL/freeglut.h>
#include <string>
#include <QGraphicsProxyWidget>
#include <QFont>
#include "LabObject.h"
#include "LabCut.h"
#include "Lab3D.h"
#include "SMOBject.h"
#include "StatisticManager.h"
#include "PenBars.h"

class QtLCom : public ItemBase
{
	Q_OBJECT
public:
	QtLCom( QtBat* left, QtBat* right, OpenGLScene* Scene );
	~QtLCom();
	virtual void loadData();
	void initUI();
	void paintCurve( QPainter* painter );
	void acceptStatisticData( StatisticManager* manager, int index );
	void paintCurveFrom( QPainter* painter, int height, int index );
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	void saveAsXmlNode( QDomDocument &doc, QDomElement &node );
	
	void logOpen();
	void logAddToComAttr( QString str );

public slots:
	void needToBeRemoved();
	void removeCompare();
	void swithPlot();
protected:
	void handleBackspace();
	void resizeItem( int sizewidth, int sizeheight );
	void keyPressEvent( QKeyEvent* event );
	void mouseMoveEvent( QGraphicsSceneEvent *event );
	void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
private:
	//void removeCompare();
	void updatePos();
	void removeLastBar();
	void removeAllBars();
	void drawBars( QPainter* painter );
	void reOrderItems();
	void mouseOnItems( QPointF pos );
	//
	QtBat *_left, *_right;
	OpenGLScene *_scene;
	std::string _nameForLeft;
	std::string _nameForRight;
	SMObject **_dataForLeft;
	SMObject **_dataForRight;
	QPointF _LineForLeft[6][50];
	QPointF _LineForRight[6][50];
	QColor _colorLeft;
	QColor _colorRight;
	QColor _backGround[3];
	float _stepHeight;
	int textHeight;
	int textWidth;
	int numHeight;
	QFont _fontForNumber;
	QFont _fontForName;
	QString marker[3];
	QList<PenBars*> _bars;
	int INDEXOFFSET;
	QString _curParaName;
	//QRadioButton* radioButCamber;
	//QRadioButton* radioButSpeed;
};
#endif