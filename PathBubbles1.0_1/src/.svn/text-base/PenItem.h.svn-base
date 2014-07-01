#ifndef PENITEM_H
#define PENITEM_H

#include <QRectF>
#include <QPainter>
#include <QString>
#include <QObject>

class PenItem : public QObject
{
public:
	PenItem();
	virtual ~PenItem();
	virtual QRectF boundingRect();
	virtual QPoint initPos();
	virtual void setPos( QPoint pos );
	virtual QPoint getPos();
	virtual void paint( QPainter* painter );
	virtual bool mouseOver( QPointF pos );
	virtual int getType();
	virtual QString toQString();
	virtual void click();
	virtual void release();
	virtual QString getText();
	virtual void setText( QString text );
protected:
	QRectF _bounding;
	QPoint _pos;
	bool _curState;
	int _TYPE;
};

#endif