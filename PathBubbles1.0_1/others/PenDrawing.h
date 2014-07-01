#ifndef PENDRAWING_H
#define PENDRAWING_H

#include <QPainter>
#include <QObject>
#include <QPointF>
#include <QList>
#include <QRectF>
#include <QString>
#include <QChar>
#include <QFontMetrics>
#include "Discreptor.h"
//#include "ScriptEngin.h"
#include "PenItem.h"

#define SPACE_PENDRWING_CODE 10

class PenDrawing : public PenItem
{
public:
	PenDrawing( /*ScriptEngin* engin*/ );
	PenDrawing();
	//void setEngine( ScriptEngin* engin );
	//~PenDrawing();
	//QRectF boundingRect();
	void paint( QPainter* painter );
	//*****************************************************************
	void addLine( QPointF pos1, QPointF pos2 );
	bool contains( QPointF pos )
	{
		return this->_bounding.contains( pos );
	}
	void finalize();
	void finalize( float camber );
	void setPos( QPoint pos );
	QString toQString();
private:
	void getHitPos( double* pointer );
	void runScript();
	void drawAnalysis( QPainter* painter );
	void drawCurve( QPainter* painter );
	void updateBoundingRect( QPointF pos );
	void initDescriptor();
	void initDescriptor( float camber );
	void formalizeDrawing();
	void setHighlight();
	QVector<QLineF> _lines;
	QLineF _normalVector[10];
	QPointF _controlPos[10];
	bool _FORMAL;
	QLineF _link;
	QPointF _intersection[10];
	DESCRIPTOR _dis;
	bool _MOUSEOVER;
	bool _MOUSEONRUN;
	QList<QString> _codeContents;
	QFont _currentFont;
	QFontMetrics _fontMetrics;
	int _fontHeight;
	ScriptEngin* _engin;
};

#endif