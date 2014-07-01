#ifndef PENCAMBER_H
#define PENCAMBER_H

#include <QVector>
#include <QLineF>
#include "PenItem.h"
#include "Camber.h"

class PenCamber : public PenItem
{
public:
	PenCamber( Camber* camber, int marker, int frame, QPointF pos );
	void paint( QPainter* painter );
	void setPos( QPoint pos );
	void setFrame( int frame );
	void switchName( int i = -1 );
	void switchProjection();
	void switchFrame();
private:
	void updateBoundingRect( QPointF pos );
	void initLines();
	void drawName( QPainter* painter );
	QVector<QLineF> _curve;
	QPointF _offset;
	Camber* _camber;
	int _marker;
	int _frame;
	QString frameInString;
	QPointF _pos;
	int _size;
	int _currentName;
	QString _QName;
	int _currentProjection;
	QString _QProjection;
	bool ATTACHFRAME;
};


#endif