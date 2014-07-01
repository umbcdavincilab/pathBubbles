#ifndef PENTEXT_H
#define PENTEXT_H

#include "PenItem.h"
#include <QString>
#include <QFont>
#include <QPointF>
#include <QFontMetrics>

class PenText : public PenItem
{
public:
	PenText( bool background = false );
	void paint( QPainter* painter );
	void appendText( QString text );
	bool removeText();
	void setPos( QPoint pos );
	void setText( QString text );
	QString getText();
	QString toQString();
	QPoint getPos();
private:
	void updateBoundingRect();
	QString _text;
	QFont _curFont;
	QFontMetrics _metrics;
	bool _backGround;
};

#endif