#ifndef NAVIGATIONBAR_H
#define NAVIGATIONBAR_H

#include <QMutex>
#include <QGLWidget>
#include <QCursor>
#include "qtbox.h"
#include "qtbat.h"
#include "qtsm.h"

class OpenGLScene;

#define MINRATE 0.8

class NavigationBar : public QGLWidget
{
	Q_OBJECT

public:
	NavigationBar( QWidget *parent, OpenGLScene* scene );
	QSize sizeHint() const;
	float getOffset()
	{
		return this->rectOffset;
	}
	void updateSize( QSize size )
	{
		this->_curSize = size;
		_rect.setWidth( size.width() / _scale * rectOffset + 1 );
		_rect.setHeight( size.height() /_scale * rectOffset + 1 ); 
		this->update();
	}
	void addItemRect();
	void shiftViewportByOffset( QPoint offset, int TYPE = 0 );
signals:
	void shiftViewport( QPoint offset );
	void updateCoverage( float rate );
protected:
	void mousePressEvent( QMouseEvent * event );
	void mouseReleaseEvent( QMouseEvent * event);
	void mouseMoveEvent( QMouseEvent * event);
    void paintEvent(QPaintEvent *event);
	void wheelEvent( QWheelEvent* event);
	void initializeGL();
	void paintGL();
	void drawItemRects();
	void drawRect( QRect input, int type, bool highlight = false );
private:
	bool contains( QPoint pos );
	void onHighLight();
	bool onResize( QPoint pos );
	bool touchBoundary();
	int _ITEMRECT;
	int hitItemRect( QPoint pos );
	int _scale;
	QRect _rect;
	QPoint lastPos;
	bool _HIT;
	bool _RESIZE;
	QList< ItemBase* > allItems;
	OpenGLScene* _scene;
	QList< QRect > renderArray;
	QList< int > typeArray;
	QList< ItemBase* > holderArray;
	ItemBase* SELECTED;
	QRect SELECTEDBOARD;
	QMutex LOCKER;
	float rectOffset;
	QSize _curSize;
	int increamentalX, increamentalY;
	int _HIGHLIGHTINDEX;
};

#endif