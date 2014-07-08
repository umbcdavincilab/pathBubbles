#ifndef ITEMLABEL_H_
#define ITEMLABEL_H_

#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsTextItem>
#include <QPainter>
#include <QImage>
#include <QFileDialog>
#include <QMenu>
#include <QString>
#include "myColor3.h"

class ItemLabel: public QGraphicsTextItem
{
public:
	ItemLabel( QGraphicsItem *parent, QGraphicsScene *scene, int sizewidth, int sizeheight );
	QRectF boundingRect() const;
	QRectF imageRect() const;
	void paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget );
	void loadLabelImage( const QString imageName)
	{
		m_labelImage.load( imageName );
	}
	void setLabelColor(myColor3 color)
	{
		m_labelColor = color;
	}
	void updateLabel( int widht, int height );	

	QString adjustText(QString text);
	

	QRectF LabelRect;

	QPointF imagePos();

	void setName(QString name);
	
	bool mouseOvered;
	int mouseClicked;

protected:
	void focusOutEvent(QFocusEvent *event);
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
	void hoverMoveEvent( QGraphicsSceneHoverEvent *event );	
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
	//void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
	//void keyPressEvent ( QKeyEvent * event ); 
	//void keyReleaseEvent(QKeyEvent * event );
private:	
	QString backupName;
	QImage m_labelImage;
	myColor3 m_labelColor;
	int m_size_width, m_size_height;
	int m_width, m_height;
};

#endif