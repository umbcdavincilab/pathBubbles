#include "ItemLabel.h"


ItemLabel::ItemLabel(QGraphicsItem *parent, QGraphicsScene *scene, int sizewidth, int sizeheight )
: QGraphicsTextItem( tr(""), parent, scene),
  m_size_width(0),
  m_size_height(0)
{
	setFlag(QGraphicsItem::ItemIsMovable, false);
	setFlag(QGraphicsItem::ItemIsSelectable, true);
	
	updateLabel( sizewidth, sizeheight );

	//m_size = size;

	this->setDefaultTextColor( QColor( 0, 0, 0 ) );
	this->setFont( QFont( "times", 10 ) );
}

QRectF ItemLabel::boundingRect() const
{
	return QRectF(0, 0, m_width, m_height);
}

QRectF ItemLabel::imageRect() const
{
	return QRectF(0, 0, m_size_width*0.15, m_size_height*0.15);
}

void ItemLabel::updateLabel( int width, int height )
{
	m_size_width = width;
	m_size_height = height;

	m_width = width * 0.5;
	m_height = 20;//height * 0.15;
	
	setPos( -m_width/2, -m_height-height/2-12 );
	
	return;
}

void ItemLabel::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{
	QRect rect = painter->fontMetrics().boundingRect( this->toPlainText() );
	
	if (rect.width() > 80)
	{
		m_width = rect.width() + 30;
	}

	painter->setBrush(QColor(m_labelColor.a, m_labelColor.b, m_labelColor.c, 100));
	painter->setPen(Qt::NoPen);
	painter->drawRoundedRect(this->boundingRect(), 15, 10);

	painter->drawImage( this->imageRect(), this->m_labelImage, QRectF(QPointF(0, 0), m_labelImage.size()) );

	QGraphicsTextItem::paint( painter, option, widget);
}

void ItemLabel::focusOutEvent(QFocusEvent *event)
{
	setTextInteractionFlags(Qt::NoTextInteraction);
	QGraphicsTextItem::focusOutEvent(event);

	return;
}

void ItemLabel::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	if (textInteractionFlags() == Qt::NoTextInteraction)
	{
		setTextInteractionFlags(Qt::TextEditorInteraction);
	}

	QGraphicsTextItem::mouseDoubleClickEvent(event);

	return;
}

void ItemLabel::contextMenuEvent( QGraphicsSceneContextMenuEvent *event )
{
	QMenu menu;
	QAction *insertAction = menu.addAction("insert image");
	QAction *selectedAction = menu.exec(event->screenPos());

	if (selectedAction == insertAction)
	{
		QString fileName = QFileDialog::getOpenFileName();
		if ( !fileName.isEmpty() )
			loadLabelImage(fileName);
	}
}