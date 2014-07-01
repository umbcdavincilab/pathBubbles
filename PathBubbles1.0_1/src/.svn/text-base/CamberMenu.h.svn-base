#ifndef CAMBERMENU_H
#define CAMBERMENU_H

#include "PenItem.h"
#include "PenCamber.h"
#include <QString>
#include <QFont>
#include <QPointF>
#include <QFontMetrics>

class CamberMenu : public PenItem
{
public:
	CamberMenu( int index, PenCamber* camber );
	~CamberMenu();
	int getIndex();
	void paint( QPainter* painter );
	bool mouseOver( QPoint pos );
	void setFrame( int index );
	void setPos( QPoint pos );
	void switchProjection();
	void switchMarker();
	void switchFrame();
private:
	void updateBoundingRect();
	int _index;
	QFont _curFont;
	PenCamber* _camber;
	QFontMetrics _metrics;
	QString _indexInString;
};

#endif