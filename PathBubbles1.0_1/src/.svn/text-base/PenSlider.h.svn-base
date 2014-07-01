#ifndef PENSLIDER_H
#define PENSLIDER_H

#include "PenItem.h"
#include "qtbox.h"
#include <QList>
#include <QString>
#include <QFont>
#include <QPointF>
#include <QFontMetrics>

class ItemBase;

class PenSlider : public PenItem
{
	Q_OBJECT
public:
	PenSlider( QPointF initPos, QList<QList<ItemBase*> > bats, QList<QList<float> > rates );
	~PenSlider();
	void paint( QPainter* painter );
	QString toQString();
	bool mouseOver( QPointF pos );
	void click();
	void release();
	void setPos( QPoint pos );
public slots:
	void posUpdate( ItemBase* init );
private:
	void hideAllItems();
	void initScale();
	void showItemAbove( float rate );
	void updateRate();
	QString _text;
	QFont _curFont;
	QFontMetrics _metrics;
	QList<QList<ItemBase*> > _bats;
	QList<QList<float> > _rates;
	bool _ONDRAG;
	bool _ONSLIDER;
	QRectF _curSlider;
	QRectF _background;
	QPointF _lastPos;
	float MaxRate;
	float MinRate;
	QVector<QLineF> _scale;
	QList<QString> _scaleString;
	QPointF _iniPos;
};

#endif