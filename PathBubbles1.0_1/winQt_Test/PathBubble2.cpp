
#include "PathBubble2.h"


PathBubble2::PathBubble2( int size, int x, int y, OpenGLScene* Scene, StatisticManager* manager, OpenManager* open, QString name)
: PlotBase(size, x, y, Scene, manager)
{
	_scene= Scene;
	this->_open = open;
	this->_TYPE = PATHBUBBLE2;

     this->_text = new QTextEdit( );
   _text->setStyleSheet( "background-color: rgb(240, 250, 250)");
   _text->setMask( this->roundRect( QRect(0, 0, m_size_width, m_size_height), 20) );
   
  
   this->_myControl = this->_scene->addWidget( _text );
   
   loadFile(name);

   this->_myControl->setPos( this->pos().x() - this->Width()/2, this->pos().y() - this->Height()/2 );

   this->hide();

}

void PathBubble2::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
	PlotBase::paint( painter, option, widget );

	this->_myControl->setPos( this->pos().x() - this->Width()/2, this->pos().y() - this->Height()/2 );

	//_text->show();
}


void PathBubble2::hoverMoveEvent( QGraphicsSceneHoverEvent *event )
{
	ItemBase::hoverMoveEvent(event);	
	//highlighted = whichItem(event->pos(),this->Width(), this->Height());
	int num=_text->textCursor().LineUnderCursor;
	num=num;
}


void PathBubble2::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	ItemBase::mousePressEvent(event);
	//itemSelected = whichItem(event->pos(),this->Width(), this->Height());
}

void PathBubble2::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	ItemBase::mouseReleaseEvent( event );

	_text->textCursor().hasSelection();
	QString selectedtext = _text->textCursor().selectedText();
	/*if( itemSelected == whichItem(event->pos(),this->Width(), this->Height()))
	{		
		if(itemSelected[0]!=-1)
		{
			QString qstr;
			string s1;
			std::ostringstream oss;
			int type=itemSelected[0], id=itemSelected[1];
			switch(type)
			{ 
			    case 'C': qstr=_complexName[id].c_str(); break;
		        case 'E': qstr=_physicalEntityName[id][0].c_str(); break;
		        case 'P': 
					qstr=_proteinName[id][0].c_str(); break;
		        case 'S': s1 = "smallMolecule";  	oss << s1 << id; s1=oss.str(); qstr=s1.c_str(); break;		
		        case 'D': s1 = "Dna"; 	oss << s1 << id;	s1=oss.str();  qstr=s1.c_str(); break;	 				     	
			}
			showIterm(this->pos(), this->scenePos(), qstr); 	
		}
	}*/
}





void PathBubble2::loadFile(const QString &fileName)
{
	QFile file(fileName);
	if (!file.open(QFile::ReadOnly | QFile::Text)) 
	{
			return;
	}
	QTextStream in(&file);
	_text->setText(in.readAll()); 
	setCurrentFile(fileName);	
}


void PathBubble2::setCurrentFile(const QString &fileName)
{
	/*_curFileName = fileName;
	_curFilePath = _curFileName;
	int tindex = _curFilePath.lastIndexOf("\\");
	_curFilePath.remove(tindex+1, _curFilePath.length());*/
}

void PathBubble2::mouseMoveEvent( QGraphicsSceneMouseEvent *event )
{
	if( onBoarder( event->pos() ) )
	{
		ItemBase::mouseMoveEvent( event );
	}
	else
	{
		//QGraphicsItem::mouseMoveEvent(event);
		this->_initPos = this->pos();
		QPointF offset = event->pos() - event->lastPos();
		this->mySetPos( this->pos()+ offset );

		textItemPosMoved( offset );
		
		emit itemPosMoved( this, offset );
	}
	return;
}


void PathBubble2::resizeItem( int size1, int size2 )
{
	ItemBase::resizeItem( size1, size2 );
	this->_myControl->resize( size1, size2 );
	_text->setMask( roundRect( QRect(0, 0, m_size_width, m_size_height), 20) );
}

void PathBubble2::textItemPosMoved( QPointF offset )
{
	mySetPos( this->pos() + offset );

}