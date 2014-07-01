#ifndef PLOTBASE_H
#define PLOTBASE_H

#include "qtbox.h"
#include "StatisticManager.h"
#include "BatDataView.h"

#define INTERNALNUM 5
#define ZEROD 0
#define ONED 1
#define ONEDT 2
#define ONEDMAT 3
#define ONEDTMAT 4

class OpenGLScene;

class PlotBase : public ItemBase
{
	Q_OBJECT
public:
	PlotBase( int size, int x, int y, OpenGLScene* Scene, StatisticManager* manager = NULL );
	virtual ~PlotBase();
	virtual void paint( QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *widget );
	virtual void plotXY( QPainter* painter );
	void setXYValue( QList<double> xlist, QList<double> ylist );
	void setXYName( QString xName, QString yName );
	void setXValue( QList<double> xlist );
	void setYValue( QList<double> ylist );
	void setXName( QString xName );
	void setYName( QString yName );
	QRectF XAxisBoundingRect();
	QRectF YAxisBoundingRect();
	QRectF plotAreaBoundingRect();
	QRectF getNameRect();
	void resizeItem( int size1, int size2 );
	void changeAttriFromFile( QPointF pos, QList<QString> names );
	void changeAttriFromStat( QPointF pos, int index );
	QList<double> loadCamberData( QString str );
	QList<double> loadSpeedData( QString str );
	virtual void saveAsXmlNode( QDomDocument &doc, QDomElement &node );
	virtual void recoverFromXmlNode( QDomElement node );
	void updateLabel();

	void logDragOnX( QString para );
	void logDragOnY( QString para );
	void logBadData( );

signals:
	void updateLabelState( ItemBase *item );

protected:
	void wheelEvent(QGraphicsSceneWheelEvent *event);

private:
	void displayBadData( QPainter* painter );
	void drawXAxis( QPainter* painter );
	void drawYAxis( QPainter* painter );
	void displayName( QPainter* painter );
	void updatePlotData();
	void calInterval();
	void genDataPoint();
	double getMinValue( QList<double> value );
	double getMaxValue( QList<double> value );
	void updateXYParameter();
	QString getCamberFilePath( QString str );
	QString getSpeedFilePath( QString str );

protected:
	QList<double> xVaule, xAxisData;
	QList<double> yVaule, yAxisData;
	int dataNum;
	QList<QPointF> dataPoint;
	double xmin, xmax, ymin, ymax;
	double xIntervalVal, yIntervalVal, xIntervalDis, yIntervalDis;
	QString xParaName, yParaName;
	QList<QString> nameList;
	StatisticManager* staManager;
	int xDataType, yDataType;

	OpenGLScene* scene;
	bool _LABEL;
	bool BADDATA;

	BatDataView* rawDataView;
	bool _RAWDATAVIEW;

	QRectF nameRect;
};

#endif //PLOTBASE_H