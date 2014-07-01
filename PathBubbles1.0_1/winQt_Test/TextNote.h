#ifndef TEXTNOTE_H
#define TEXTNOTE_H

#include <QTextEdit>
#include <QGraphicsTextItem>
#include <QPainter>
#include <QPicture>

class TextNote : public QGraphicsTextItem
{
public:
	TextNote( QGraphicsItem *parent = 0, QGraphicsScene *scene = 0 );
	QRectF boundingRect() const;
	void paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget );
	void resizeItem( int width, int height );
	QTextEdit* m_note;
protected:
	void focusOutEvent(QFocusEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
private:
	QImage _note_Background;
	int m_width, m_height;
};

#endif