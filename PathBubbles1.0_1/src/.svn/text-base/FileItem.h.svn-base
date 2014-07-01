#ifndef FILEITEM_H
#define FILEITEM_H

#include "PenItem.h"
#include "OpenManager.h"
#include <QVector>
#include <QFont>
#include <QFontMetrics>

class FileItem : public PenItem
{
public:
	FileItem( QString name, int type, OpenManager* open );
	void paint( QPainter* painter );
	bool contains( QPointF pos );
	void setPos( QPoint pos );
	QString toQString();
	void enablePreOpen();
	void addView( int index, QColor color );
	void disablePreOpen( QPointF pos, bool open = false );
	void drawPreOpen( QPointF pos, bool flag = false );
private:
	void drawName( QPainter* painter );
	QList<int> removeOpened( QList<int> view );
	QList<int> removeOpenedForPreOpen( QList<int> view );
	void drawView( QPainter* painter );
	void drawPreOpen( QPainter *painter );
	QString _name;
	int TYPE;
	bool PREOPEN;
	bool SIMUOPEN;
	QRectF _preRect;
	QPointF _fontBounding;
	OpenManager* _open;
	QVector<QRectF> _rectList;
	QList<int> _view;
	QList<QColor> _color;
	QVector<QRectF> _viewRects;
	int ViewStep;
	QFont _font;
	QFontMetrics _metrics;
	QList<int> _openedView;
};

#endif