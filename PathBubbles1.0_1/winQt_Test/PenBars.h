#ifndef PENBARS_H
#define PENBARS_H

#include "PenItem.h"
#include <QString>
#include <QList>
#include <QColor>
#include <QFontMetrics>
#include <QFont>

#define BARMARGIN 10
#define BARSIZE 20
#define BARHEIGHT 50

class PenBars : public PenItem
{
public:
	PenBars();
	void acceptData( QList<QString> name, QList<float> data, QList<QColor> color, QString para );
	void paint( QPainter* painter );
	void setPos( QPoint pos );
	void setShowName( bool bit );
	void setShowPara( bool bit );
private:
	void updateBoundingRect();
	void drawFonts( QPainter* painter );
	void drawBars( QPainter* painter );
	QList<QString> _name;
	QList<float> _data;
	QList<QString> _dataString;
	QList<QColor> _color;
	QFont _font;
	int _fontHeight;
	float _max;
	float _min;
	QString _para;
	//
	int _barOffsetX;
};

#endif