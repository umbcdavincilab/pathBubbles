#include "ItemLabel.h"
#include "QTextCursor.h"

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
	mouseOvered = false;
	mouseClicked = 0;
}

/*void ItemLabel::keyPressEvent (  QKeyEvent * event )
{
	backupName = this->toPlainText();
	backupName = backupName;
}*/

/*void ItemLabel::keyReleaseEvent ( QKeyEvent * event )
{
	QGraphicsTextItem::keyReleaseEvent(event);

	//backupName = this->toPlainText();	
	//QTextCursor myCursor = this->textCursor();
	
	//myCursor.setKeepPositionOnInsert(true);	
	//bool flag = myCursor.keepPositionOnInsert();
	//bool mflag = myCursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, 1);
	//myCursor.setPosition(10, QTextCursor::MoveAnchor);
}*/

//remove the key event fix the "cursor alway jump to the front error"

QRectF ItemLabel::boundingRect() const
{
	return QRectF(0, 0, m_width, m_height);
}

QRectF ItemLabel::imageRect() const
{
	return QRectF(0, 0, m_size_width*0.15, m_size_height*0.15);
}

void ItemLabel::setName(QString name)
{
	backupName=name;	
}

void ItemLabel::updateLabel( int width, int height )
{
	m_size_width = width;
	m_size_height = height;

	m_width = width * 0.5;
	m_height = 20;
	
    QString st = backupName; 
	if(backupName.size()==0)
		st=this->toPlainText();
	
	st=adjustText(st);
	setPlainText(st);

	QFontMetrics fontMetrics(this->font());	
	QRect rect = fontMetrics.boundingRect( this->toPlainText() );
	QRectF rect2=this->boundingRect(); 	
	if(rect.width()>=width)
	{
		m_width=width;
	    setPos( -m_width/2 ,  -m_height-m_size_height /2);		
	}
	else if ( rect.width()+20 != boundingRect().width() )
	{
		m_width = rect.width()+20;	
		setPos( -m_width/2 ,  -m_height-m_size_height /2);
		int margin=20;
		if(rect.width() < 80 && rect.width()< m_size_width/2)
			margin = (80 - rect.width() );
	
		rect2=QRectF(-margin/2, rect2.y(), rect.width()+20 + margin, rect2.height());		
		LabelRect = rect2;	
	}
			
}

QString ItemLabel::adjustText(QString text)
{
	QFontMetrics fontMetrics(text);			
	QString st = text, st1=st;
	QRect rect = fontMetrics.boundingRect(st);		
	int tindex=-1;
	rect = fontMetrics.boundingRect(st);
	
	while (rect.width() > m_size_width*0.9)
	{   
		tindex = st.lastIndexOf(";");
		//st.resize(st.size()*(m_size_width/2.0)/rect.width());
		if(tindex<0)
		{
			//break;
			tindex = st.size()-1;
		}
		if(tindex<0)
		   break;

		st1=st;
		st = st.mid(0, tindex);		
		rect = fontMetrics.boundingRect(st);	
	}
	if(st!=text)
	{
	    rect = fontMetrics.boundingRect(st1);	
		if(rect.width() > m_size_width*0.95 )
		   st1=st;
		st1=st1+"...";
	}	
	return st1;
}

void ItemLabel::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{
	setZValue(1000);
	//QString st = this->toPlainText();
	//st=adjustText(st);
	painter->setBrush(QColor(m_labelColor.a, m_labelColor.b, m_labelColor.c, 150));
	painter->setPen(Qt::NoPen);
	painter->drawRoundedRect(LabelRect, 15, 10);	
	painter->drawImage( LabelRect, this->m_labelImage, QRectF(QPointF(0, 0), m_labelImage.size()) );//this->imageRect()		
	QString st = this->toPlainText();    
	if(mouseOvered)    
	{
		QFont f("times", 10);					
		QFontMetrics fontMetrics(f);
		QRect fontRect = fontMetrics.boundingRect(0, 0, LabelRect.width(), 0, Qt::TextExpandTabs|Qt::TextWrapAnywhere, backupName); 
		painter->setBrush(QColor(255, 255, 255, 250));
		painter->drawRect(fontRect);		
		painter->setPen(QColor(0,0,0,255));	
		painter->drawText(fontRect, Qt::TextExpandTabs|Qt::TextWrapAnywhere, backupName);
	}
	else
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
	QTextCursor myCursor = this->textCursor();
	if (textInteractionFlags() == Qt::NoTextInteraction)
	{
		//QPoint p = this->textCursor()->pos();
		setTextInteractionFlags(Qt::TextEditorInteraction);//|Qt::LinksAccessibleByMouse|Qt::TextSelectableByKeyboard|Qt::TextSelectableByMouse|Qt::TextEditable);
		setTextInteractionFlags(Qt::TextEditable);	
		
		//myCursor->setCursorMoveStyle;
	    //cursor.movePosition(QTextCursor::Start);

		//setTextInteractionFlags(Qt::TextEditable);	
		//this->cursor().setS
		//QTextCursor tt = ;  
		//this->setMQt::LogicalMoveStyle.
		//this->textCursor()->setTabChangesFocus(true);
		//this->cursor().setCursorMoveStyle();
		//this->textCursor().setPos(p);				
	}
	/*myCursor.setKeepPositionOnInsert(true);	
	bool flag = myCursor.keepPositionOnInsert();
	bool mflag = myCursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, 1);*/
	QGraphicsTextItem::mouseDoubleClickEvent(event);
	//myCursor.setPosition(10, QTextCursor::MoveAnchor);
	return;
}

void ItemLabel::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsTextItem::mousePressEvent(event);	
	mouseClicked=1;
}

void ItemLabel::hoverMoveEvent( QGraphicsSceneHoverEvent *event )
{
	//painter->drawText(fontRect, Qt::AlignCenter|Qt::TextExpandTabs|Qt::TextWrapAnywhere, nodeName);//_complexName[id]);	
	QPointF lp = event->lastPos();
	QRectF br = this->boundingRect();
	if(!br.contains(lp))
	    mouseClicked=0;

	float cx=LabelRect.center().x(), cy=LabelRect.center().y(), w=LabelRect.width()*0.5, h=LabelRect.height()*0.5;
	QRectF rect=QRectF(cx-w/2,cy-h/2,w,h);
	if(rect.contains(event->pos().x(),event->pos().y()))
	{
		QString st = this->toPlainText();    
		if(st.contains("..."))
		{
			if(mouseClicked==0)
			{
				mouseOvered=true;			    
			}
			else
				mouseOvered=false;			    
		}
	}
	else
	{
	    mouseOvered=false;
		//mouseClicked=0;
	}

}

/*void ItemLabel::contextMenuEvent( QGraphicsSceneContextMenuEvent *event )
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
}*/