#include "TextNote.h"

TextNote::TextNote( QGraphicsItem *parent, QGraphicsScene *scene ) : QGraphicsTextItem( tr(""), parent, scene )
, _note_Background( "icon/note.png" )
{
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
	this->_note_Background = this->_note_Background.scaled( 150, 150 );
	this->setTextWidth( 150 );
	m_width = m_height = 150;
	
	//m_note = new QTextEdit();
	//m_note->setStyleSheet( "background-color: rgb(215, 200, 145)");
	//scene->addWidget(m_note);
	//m_note->setMask( QRect(0, 0, 150, 150), 20 ) );	
	//this->_myControl = _scene->addWidget( m_note );	
	//this->_myControl->setPos(pos().x(),pos().y());
}

QRectF TextNote::boundingRect() const
{
	return QRectF( -m_width/2, -m_height/2, m_width, m_height);
}

void TextNote::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{
	this->setPos( 200, 200 );
	painter->drawImage( QPointF( -m_width/2, -m_height/2 ), this->_note_Background );
	this->setDefaultTextColor( QColor( 0, 0, 0 ) );
	this->setFont( QFont( "times", 10 ) );
	QGraphicsTextItem::paint( painter, option, widget);
	//painter->setBrush( QColor( 10, 10, 40, 250) );
	//painter->drawRect( this->boundingRect() );

	return;
}

void TextNote::focusOutEvent(QFocusEvent *event)
{
	setTextInteractionFlags(Qt::NoTextInteraction);
    QGraphicsTextItem::focusOutEvent(event);
	return;
}

void TextNote::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	if (textInteractionFlags() == Qt::NoTextInteraction)
	{
        setTextInteractionFlags(Qt::TextEditorInteraction);
	}
    QGraphicsTextItem::mouseDoubleClickEvent(event);
	return;
}

void TextNote::resizeItem( int width, int height )
{
	m_width = width-6;
	m_height = height-6;

	this->_note_Background = this->_note_Background.scaled( m_width, m_height );
	this->setTextWidth( m_width );
}