#include "fileSelection.h"
#include "openglscene.h"

//static const QPointF ControlBound[4] = {
//     QPointF(200, 0),
//     QPointF(400, 0),
//     QPointF(380, 18),
//     QPointF(220, 18)
// };

fileSelection::fileSelection( QSizeF size, OpenGLScene* scene, OpenManager* open )
: BarBase( size, 10, 60 )
, _FileTrans( 0, 0 )
, _heightPercent( 0 )
, _DRAGFILE( false )
, _Control( size.width()/3, 0, size.width()/3, 18 )
, _ONMOVE( false )
, _HIGHLIGHTCONTROL( false )
, _VISINDEX( -1 )
, _EllipseMove( size.width()/3*2 - 36, 0 , 18, 18)
, _EllipseHide( size.width()/3*2 - 18, 0 , 18, 18)
, _EllipseMap( size.width()/3*2 - 54, 0 , 18, 18)
, _EllipseColor( size.width()/3*2 - 72, 0 , 18, 18)
, SHIFTFILEVIEW( false )
, _DIRECTPREOPEN( false )
, _ONCONTROLTYPE( -1 )
, CLICKINDEX(-1)
, COLORBAR( false )
, _CurrentFileName( "" )
, lastIndex(0)
, lastValue(0)
, _CTRL( false )
{
	_dia=NULL;
	setFlag( QGraphicsItem::ItemIsMovable, true);
	setFlag( QGraphicsItem::ItemIsSelectable, true);
	setFlag( QGraphicsItem::ItemIsFocusable, true);
	initViewRect();
	initIcons();
	this->_scene = scene;
	this->_open = open;
	controlName[0] = QString( tr("") );
	controlName[1] = QString( tr("") );
	controlName[2] = QString( tr("") );
	controlName[3] = QString( tr("") );
	//
	initManager();
	return;
}

fileSelection::~fileSelection()
{
	delete [] this->_dia; 
	delete [] this->_layout; 
}


QString fileSelection::tailofLogMsg()
{
	QString logtext;

	logtext.append( " (Model-Composition-View Bubble)\n" );

	return logtext;
}

void fileSelection::logHide()
{
	QString logtext;

	logtext.append( headofLogMsg() );
	logtext.append( "Hide the Open Manager" );
	logtext.append( tailofLogMsg() );

	//emit logInfor( logtext );
}

void fileSelection::logOrderByWeight()
{
	QString logtext;

	logtext.append( headofLogMsg() );
	logtext.append( "Order the bubbles by weight" );
	logtext.append( tailofLogMsg() );

	//emit logInfor( logtext );
}

void fileSelection::logOrderBySpeed()
{
	QString logtext;

	logtext.append( headofLogMsg() );
	logtext.append( "Order the bubbles by speed" );
	logtext.append( tailofLogMsg() );

	//emit logInfor( logtext );
}

void fileSelection::logShowColorBar()
{
	QString logtext;

	logtext.append( headofLogMsg() );
	logtext.append( "Show the Color Bar" );
	logtext.append( tailofLogMsg() );

	//emit logInfor( logtext );
}

void fileSelection::logHideColorBar()
{
	QString logtext;

	logtext.append( headofLogMsg() );
	logtext.append( "Hide the Color Bar" );
	logtext.append( tailofLogMsg() );

	//emit logInfor( logtext );
}

void fileSelection::logDragFileToCompose( QString filename )
{
	QString logtext;

	logtext.append( headofLogMsg() );
	logtext.append( "Drag the file " );
	logtext.append( filename );
	logtext.append( " to Composition bubble" );
	logtext.append( tailofLogMsg() );

	//emit logInfor( logtext );
}

void fileSelection::logDragVisToCompose( int index, int vis )
{
	QString logtext;
	QString tmp;

	tmp = getVisNameByIndex( vis );

	logtext.append( headofLogMsg() );
	logtext.append( "Drag the vis method " );
	logtext.append( tmp );
	logtext.append( " to the ");
	tmp.setNum( index );
	logtext.append( tmp );
	logtext.append( "th item in the Composition bubble" );
	logtext.append( tailofLogMsg() );

	//emit logInfor( logtext );
}

void fileSelection::logDragItemtoScene( int index, QPointF pos )
{
	QString logtext;
	QString str, tmp;

	logtext.append( headofLogMsg() );
	logtext.append( "Drag the " );
	str.setNum( index );
	logtext.append( str );
	logtext.append( "th item to " );
	str = "(";
	tmp.setNum( pos.x() );
	str.append( tmp );
	str.append( ", ");
	tmp.setNum( pos.y() );
	str.append( tmp );
	str.append( ")" );
	logtext.append( str );
	logtext.append( tailofLogMsg() );

	//emit logInfor( logtext );
}

void fileSelection::logDragFileToScene( QString filename, QPointF pos )
{
	QString logtext;
	QString str, tmp;

	logtext.append( headofLogMsg() );
	logtext.append( "Drag the file " );
	logtext.append( filename );
	logtext.append( "to " );
	str = "(";
	tmp.setNum( pos.x() );
	str.append( tmp );
	str.append( ", ");
	tmp.setNum( pos.y() );
	str.append( tmp );
	str.append( ")" );
	logtext.append( str );
	logtext.append( tailofLogMsg() );

	//emit logInfor( logtext );
}

void fileSelection::logDragVisToScene( int vis, QPointF pos )
{
	QString logtext;
	QString str, tmp;

	tmp = getVisNameByIndex( vis );

	logtext.append( headofLogMsg() );
	logtext.append( "Drag the vis method " );
	logtext.append( tmp );
	logtext.append( "to " );
	str = "(";
	tmp.setNum( pos.x() );
	str.append( tmp );
	str.append( ", ");
	tmp.setNum( pos.y() );
	str.append( tmp );
	str.append( ")" );
	logtext.append( str );
	logtext.append( tailofLogMsg() );

	//emit logInfor( logtext );
}

QString fileSelection::getVisNameByIndex( int vis )
{
	switch ( vis )
	{
	case 0:
		return "Geometry";
	case 1:
		return "Video-Batc";
	case 2:
		return "Video-Batg";
	case 3:
		return "Video-Bath";
	case 4:
		return "PC-elbow";
	case 5:
		return "PC-wrist";
	case 6:
		return "PC-MCP3";
	case 7:
		return "BarChart";
	case 8:
		return "LineChart";
	case 9:
		return "Scatter";
	case 10:
		return "Web";
	case 11:
		return "Matlab";
	case 12:
		return "Pen";
	case 13:
		return "Note";
    case 14:
		return "PathBubble1";
	}
	return "";
}

void fileSelection::setDiaVisible(bool flag)
{ 
	if(_dia!=NULL)
	 {
		 _dia->setVisible(flag); 
		 if(_scene->fileSelectionOpened)
		 {
			 /*if(!_scene->menu.isVisible())
			 {
				_scene->menu.setVisible(flag);	
				_scene->menu.blockSignals(flag);
			 }
			 if(!_scene->loadMenu->isVisible())
			 {
				_scene->loadMenu->setVisible(flag);	
			 }			 
			 //_scene->menu.menuI
			 _scene->pathwayAction->setChecked(true);
			 */
		 }
	}
}

QRectF fileSelection::boundingRect() const
{
	return QRectF( QPointF( _size.width()/3, 0 ), QSizeF(_size.width()/3, _size.height()) );
}


void fileSelection::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{	
	//why this function is not executed?
	//BarBase::paint( painter, option, widget );
	painter->save();
	//painter->setPen( Qt::NoPen );
	//painter->setBrush( QBrush( QColor( 255, 255, 255, 255) ) );
	//painter->drawRect( this->shadow() );
	painter->setBrush( QBrush( QColor( 0, 0, 0, 255) ) );
	//painter->drawRect( this->boundingRect() );
	this->setZValue(97);
	this->_myControl->setZValue(98);
	//this->_dia->setZValue(99);
	//this->_myControl->resize( _size.width()/3, _size.height());
	//this->_myControl->setPos( _size.width()/3+this->pos().x(), this->pos().y());
	/*if(_scene->fileSelectionOpened)
	{
		QRect rect=getDiaRect();	
		rect=QRect(rect.x(),rect.y(),1000, rect.height()+1000);
		painter->drawRect(rect);		
		//_scene->menu.setVisible(true);	
		//_scene->pathwayAction->setVisible(true);
		//_scene->pathwayAction->setChecked(true);
	}*/

	/*
	painter->save();
	//*********************************************************
	drawDropArea( painter );
	drawFileArea( painter );
	drawVisArea( painter );
	renderItem( painter );
	drawControl( painter );
	drawViews( painter );
	drawColorBar( painter );
	//*********************************************************
	painter->restore();
	//*********************************************************
	*/
	painter->restore();
	return;
}

void fileSelection::drawDropArea( QPainter *painter )
{
	painter->save();
	//*********************************************************
	painter->setPen( Qt::NoPen );
	painter->setBrush( QBrush( QColor( 0, 0, 60, 60) ) );
	painter->drawRect( this->getDorpArea() );
	//*********************************************************
	painter->restore();
	return;
}

void fileSelection::drawFileArea( QPainter *painter )
{
	painter->save();
	//*********************************************************
	this->_manager->renderRoot( painter, scaleArea(), getPaintArea()); 
	//*********************************************************
	painter->restore();
	return;
}

void fileSelection::drawVisArea( QPainter *painter )
{
	painter->save();
	//*********************************************************
	//*********************************************************
	painter->restore();
	return;
}

/*void fileSelection::initManager()
{
	_scene->pwdParser->findFillsinFolder();
}*/

void fileSelection::BoxClicked()
{
	//_scene->menu.setVisible(true);
	listBoxClicked=true;
}


void fileSelection::go()
{
	//blockSignals(true);
	/*else if ( selectedAction == G2Pathway )
	{	
		QString name = "data/pathways/MitoticG2-G2";
		openPathBubblebyMenu(name, item, item==NULL?contextMenuEvent->scenePos():item->sceneBoundingRect().center()+ QPointF(item->realRect().width(),0));//contextMenuEvent->scenePos());	
    }	
	else if ( selectedAction == BMPPathway )
	{	
		QString name = "data/pathways/BMP";
		openPathBubblebyMenu(name, item, item==NULL?contextMenuEvent->scenePos():item->sceneBoundingRect().center()+ QPointF(item->realRect().width(),0));//contextMenuEvent->scenePos());	
    }
	else if ( selectedAction == TGFPathway )
	{	
		QString name = "data/pathways/68874_";
		openPathBubblebyMenu(name, item, item==NULL?contextMenuEvent->scenePos():item->sceneBoundingRect().center()+ QPointF(item->realRect().width(),0));//contextMenuEvent->scenePos());	
    }*/		
	//QString name=comboBox->currentText();
	int index=listBox->currentRow();

	lastIndex=index;
	lastValue=listBox->verticalScrollBar()->value();
	float totalValue=listBox->verticalScrollBar()->maximum()-listBox->verticalScrollBar()->minimum();
	lastValue=lastValue/totalValue*pathNames.size(); //find the current top item 
	lastValue=lastIndex-lastValue; //the distance to the selected item
  	//pathIDs[index];
	//QTextStream out;
	//out << pathIDs[index] << "_1pathway.txt";
	//QString name= out.readAll(); does not work
	QString num;
	
	//big ones : "109581"(apoptosis), 453277(Mitotic M-M/G1 phases)
	QString fname ="data/Reactome_Pathway_Data/pathwayTable/" + num + "157279" + "_"; //1566977
	//keqin: check 199991 it crash //157858//190239
	//mitotic anaphase: a midium simple example
	//num= QString::number(pathIDs[index]);
	//QString fname = "data/Reactome_Pathway_Data/pathwayTable/" + num + "_";

	QString pname = pathNames[index];
	_scene->closeFileSelection();
	_scene->openPathBubblebyMenu(fname, pname, "");
	_myControl->setPos(-2000,-1000);
}

void fileSelection::initManager()
{
	_layout = new QGridLayout();

	listBox = new QListWidget;
	//background: #eeeeee; border: 1px solid #afafaf;border-radius: 10px;margin: 10px 0 0 20px;max-width: 305px;min-height: 100px;} QListWidget::item{ background-image: url(:/pictures/attach_icon.png);background-repeat: no-repeat;background-position: left center; color: #212121;border: 1px solid #afafaf;min-height: 54px;max-height: 54px;padding-left: 54px;}QListWidget::item:selected{background-image: url(:/pictures/attach_icon.png);background-repeat: no-repeat;background-position: left center;background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #cdcdcd, stop: 1 #ababab);border-top: 1px solid;border-top-color: #a8abad;border-bottom: 1px solid;border-bottom-color: #bfc7ce;color: #333538;}");
	//listBox->setStyleSheet("QListWidget{ border: 1px solid #afafaf; border-radius: 10px}");
	
	_scene->pwdParser->readNewPathwayNameFiles(pathIDs, pathNames);
	for(int i=0; i<pathNames.size();i++)//
	{
		listBox->addItem(pathNames[i]);		
	}

	//listBox->setStyleSheet(QString("QScrollBar:horizontal { border: 2px solid grey; background: #32CC99; height: 15px; margin: 0px 20px 0 20px; }"));
 	
	this->_dia = new QDialog();
	this->_dia->setLayout(_layout);		
	QPalette Pal(QColor(0,0,0,255));	
	//Pal.setColor(QPalette::Background, Qt::black);	
	//this->_dia->setAutoFillBackground(true);
	this->_dia->setPalette(Pal);			
	//Pal.setColor(QPalette::Shadow, Qt::gray);
	this->_dia->setPalette(Pal);
	_layout->addWidget(listBox, 0, 0, 1, 1 );
	
	connect( listBox, SIGNAL(itemClicked(QListWidgetItem * )), this, SLOT(go()) );    
	//connect( this->listBox, SIGNAL(clicked()), this, SLOT(BoxClicked()) ); 
	QScrollBar *vBar=listBox->verticalScrollBar();
	QScrollBar *hBar=listBox->horizontalScrollBar();

	//this->listBox->setFrameStyle(QFrame::Panel | QFrame::Raised);
     //label.setLineWidth(2);

	_dia->setContentsMargins(0,0,0,0);

	QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect();
	effect->setOffset(4);
	effect->setBlurRadius(2);
	this->setGraphicsEffect(effect);

	//QRect rect=listBox->frameRect();	
	//listBox->setFrameRect(QRect(rect.x()-20,rect.y()-20, rect.width()+40, rect.height()+40));	
	//listBox->setFrameShadow(QFrame::Sunken);
	//listBox->setFrameShape(QFrame::StyledPanel);

	//listBox->blockSignals(true);
    //QScrollBar:vertical { background: solid black; font: white} QScrollBar:down-arrow:vertical {background: white;} 
    //vBar->setStyleSheet("QScrollBar::handle {background: white}");//ScrollBar::handle:vertical {b: gray}"
	//vBar->setStyleSheet("QScrollBar:vertical { background: solid black}; QScrollBar:down-arrow:vertical {background: white}");	
	
	//vBar->setStyleSheet("");

	/*QScrollBar:horizontal {border: 2px solid green; background: cyan; height: 15px;  margin: 0px 40px 0 0px;}

    QScrollBar::handle:horizontal {background: gray; min-width: 20px;}

	QScrollBar::add-line:horizontal {background: blue; width: 16px; subcontrol-position: right; subcontrol-origin: margin; border: 2px solid black;}

    QScrollBar::sub-line:horizontal {background: magenta; width: 16px; subcontrol-position: top right; subcontrol-origin: margin;  border: 2px solid black;    position: absolute; right: 20px;}

	QScrollBar:left-arrow:horizontal, QScrollBar::right-arrow:horizontal {width: 3px; height: 3px; background: pink; }

    QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal {background: none;}*/

	vBar->setStyleSheet("QScrollBar:vertical {border: 2px white;}");
	hBar->setStyleSheet("QScrollBar:horizontal {border: 2px white;}");
	
		
	//Pal.setColor(QPalette::border, Qt::white);
	//_dia->setPalette(Pal);

	Pal.setColor(QPalette::ButtonText, Qt::white);
	vBar->setPalette(Pal);
	Pal.setColor(QPalette::Background, Qt::white);
	vBar->setPalette(Pal);
	
	Pal.setColor(QPalette::ButtonText, Qt::white);
	hBar->setPalette(Pal);
	Pal.setColor(QPalette::Background, Qt::white);
	hBar->setPalette(Pal);

	//Pal.setColor(QPalette::ButtonText, Qt::white);
	//hBar->setPalette(Pal);
	
	connect(vBar, SIGNAL(valueChanged(int)), this, SLOT(BoxClicked()) ); 
    listBoxClicked=false;

	QPushButton *_go = new QPushButton("OK");			
	//this->_layout->addWidget( _go, 0, 1);		//keqin
	 connect( _go, SIGNAL(clicked()), this, SLOT(go()) );    
     this->_myControl = this->_scene->addWidget( _dia );	
	
	listBox->setMaximumWidth(280);		
	//this->_dia->setMaximumWidth(280);
	_go->setMaximumWidth(20);
	//this->_myControl->show();

	this->_manager = new InstanceManager();
	//
	this->_init = new InstanceInitializer();
	this->_init->initInstance( this->_manager );
	//
	this->_manager->initTree();
	return;
}

QRectF fileSelection::getPaintArea()
{
	QRectF result( QPointF( 0, 0 ), QSize( this->boundingRect().width()/3, this->boundingRect().height() ) );
	return result;
}

QRectF fileSelection::getDorpArea()
{
	return QRectF(QPointF( this->boundingRect().width()/3, 0 ), QSize( this->boundingRect().width()/3, this->boundingRect().height() ) );
}

QRectF fileSelection::getVisArea()
{
	return QRectF(QPointF( 2 * this->boundingRect().width()/3, 0 ), QSize( this->boundingRect().width()/3, this->boundingRect().height() ) );
}

QRectF fileSelection::scaleArea()
{
	int ScaleFactor = this->getSize().height() * _heightPercent * 2;
	QRectF bounding = this->getPaintArea();
	bounding.setLeft( bounding.left() - ScaleFactor );
	bounding.setRight( bounding.right() + ScaleFactor );
	bounding.setTop( bounding.top() - ScaleFactor );
	bounding.setBottom( bounding.bottom() + ScaleFactor );
	bounding.translate( this->_FileTrans.x(), this->_FileTrans.y() );
	return bounding;
}

bool fileSelection::inFileView( QPointF pos )
{
	return this->getPaintArea().contains( pos );
}

bool fileSelection::inDropView(QPointF pos)
{
	return this->getDorpArea().contains( pos );
}

bool fileSelection::inVisView( QPointF pos )
{
	return this->getVisArea().contains( pos );
}

void fileSelection::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
   int a=0;
}

/*
void fileSelection::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{

}



void fileSelection::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	_myControl->setZValue(99);
	bool flag=true;
	//if(inFileView( event->pos() ) )
	{
		_ONMOVE = true;		
	}
	BarBase::mousePressEvent( event );
	
		
}


void fileSelection::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	
	if( _ONMOVE )
	{
		BarBase::mouseMoveEvent( event );		
	}
	
}

void fileSelection::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	if( CLICKINDEX >= 0 )
	{
		QPointF pos = event->scenePos();
		this->_lines[CLICKINDEX]->disablePreOpen( pos, this->boundingRect().contains( event->pos() ) );
		
		logDragItemtoScene( CLICKINDEX+1, pos );

		CLICKINDEX = -1;
	}
	if( _DIRECTPREOPEN )
	{
		if (_CurrentFileName != NULL)
		{
			this->OpenFileDirectly( event->scenePos() );

			logDragFileToScene( _CurrentFileName, event->scenePos() );
		}
		else if (_VISINDEX > -1)
		{
			if (_VISINDEX >= 0)
			{
				this->openViewDirectly( event->scenePos() );
			}
			else
			{
				this->changeView( event->scenePos(), _VISINDEX );
			}

			logDragVisToScene( _VISINDEX, event->scenePos() );
		}

		_DIRECTPREOPEN = false;
	}
	if( inFileView( event->pos() ) || inDropView( event->pos() ) )
	{
		if( !handleReleaseFileView( event->pos() ) )
		{
		}
	}
	if( ( inDropView( event->pos() )|| inVisView( event->pos() ) ) && _VISINDEX > -1 )
	{
		if( !handleReleaseVisView( event->pos() ) )
		{
		}
	}
	_ONMOVE = false;
	_VISINDEX = -1;
	_CurrentFileName = "";
	BarBase::mouseReleaseEvent( event );
	SHIFTFILEVIEW = false;
}

void fileSelection::keyPressEvent(QKeyEvent *event)
{
	BarBase::keyPressEvent( event );

	_CTRL = event->modifiers().testFlag(Qt::ControlModifier);
}

void fileSelection::keyReleaseEvent(QKeyEvent *event)
{
	BarBase::keyReleaseEvent( event );

	_CTRL = event->modifiers().testFlag(Qt::ControlModifier);
}

void fileSelection::wheelEvent(QGraphicsSceneWheelEvent *event)
{
	if( event->delta() > 0 )
	{
		_heightPercent += 0.05;
	}
	else
	{
		_heightPercent -= 0.05;
	}
	BarBase::wheelEvent( event );
}

void fileSelection::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
   if(event->pos().x()>50)
	{
		if(_dia!=NULL)
			_dia->setVisible(false);
	}
	if( inFileView( event->pos() ) )
	{
		if( !handleHoverFileView( event->pos() ) )
		{
			BarBase::hoverMoveEvent( event );
		}
	}
	else if( inDropView( event->pos() ) )
	{
		if( !handleHoverDropView( event->pos() ) )
		{
			BarBase::hoverMoveEvent( event );
		}
	}
	else if( inVisView( event->pos() ) )
	{
		if( !handleHoverVisView( event->pos() ) )
		{
			BarBase::hoverMoveEvent( event );
		}
	}
	else
	{
		BarBase::hoverMoveEvent( event );
	}
}
*/
bool fileSelection::handleHoverDropView(QPointF pos)
{
	//		highlight item
	for( int i = 0; i < this->_lines.size(); i++ )
	{
		this->_lines[i]->mouseOver( pos );
	}
	if( _Control.contains( pos ) )
	{
		if( _EllipseMap.contains( pos ) )
		{
			_ONCONTROLTYPE = 0;
		}
		else if( _EllipseMove.contains( pos ) )
		{
			_ONCONTROLTYPE = 1;
		}
		else if( _EllipseHide.contains( pos ) )
		{
			_ONCONTROLTYPE = 2;
		}
		else if( _EllipseColor.contains( pos ) )
		{
			_ONCONTROLTYPE = 3;
		}
		else
		{
			_ONCONTROLTYPE = -1;
		}
		_HIGHLIGHTCONTROL = true;
	}
	else
	{
		_HIGHLIGHTCONTROL = false;
	}
	return false;
}

bool fileSelection::handleHoverFileView(QPointF pos)
{
	this->_manager->highLightInstance( pos );
	return false;
}

bool fileSelection::handleHoverVisView(QPointF pos)
{
	return false;
}

bool fileSelection::handlePressFileView(QPointF pos )
{
	_DRAGFILE = true;
	_initDragPos = pos;
	this->_manager->getRoot()->enableDrag();

	_CurrentFileName = "";
	this->_manager->getHighLighted(_CurrentFileName);

	return true;
}

bool fileSelection::handleMoveFileView( QPointF pos )
{
	if( inVisView( pos ) )
	{
		return false;
	}
	this->_manager->getRoot()->drawInitViewPos( pos - _initDragPos );

	if (_DIRECTPREOPEN== false && !this->boundingRect().contains( pos ))
	{
		_DIRECTPREOPEN = true;
	}

	return true;
}

bool fileSelection::handleReleaseFileView(QPointF pos)
{
	if( _DRAGFILE )
	{
		if( inDropView( pos ) )
		{
			addNewToDrag();
		}
	}
	_DRAGFILE = false;
	this->_manager->getRoot()->disableDrag();
	return true;
}

void fileSelection::renderItem( QPainter* painter )
{
	for( int i = 0; i < this->_lines.size(); i++ )
	{
		this->_lines[i]->paint( painter );
	}
}

void fileSelection::addNewToDrag()
{
	//			add to the drop area
	if( this->_lines.size() > 7 )
	{
		delete this->_lines.first();
		this->_lines.removeAt( 0 );
	}
	FileItem* item = new FileItem( _CurrentFileName, 1, _open );
	this->_lines.append( item );
	//

	logDragFileToCompose( _CurrentFileName );

	reOrder();
	return;
}

void fileSelection::reOrder()
{
	int Height = 20;
	int Width = this->boundingRect().width()/3+10;
	for( int i = 0; i < this->_lines.size(); i++ )
	{
		QRectF curBounding = this->_lines[i]->boundingRect();
		if( Width + curBounding.width() > this->boundingRect().width()/3*2 )
		{
			Width = this->boundingRect().width()/3+10;
			Height += curBounding.height() + 5;
		}
		this->_lines[i]->setPos( QPoint( Width, Height ) );
		Width += curBounding.width() + 3;
	}
}

int fileSelection::handlePressDropView( QPointF pos )
{
	CLICKINDEX = -1;
	for( int i = 0; i < this->_lines.size(); i++ )
	{
		if (this->_lines[i]->contains( pos ) )
		{
			CLICKINDEX = i;
			this->_lines[i]->enablePreOpen();
			break;
		}
	}
	return CLICKINDEX;
}

void fileSelection::handleDragInDrop(QPointF pos)
{
	if( this->boundingRect().contains( pos ) )
	{
		this->_lines[CLICKINDEX]->drawPreOpen( pos, false );
	}
	else
	{
		QPointF ScenePos = this->boundingRect().topLeft() + pos;
		this->_lines[CLICKINDEX]->drawPreOpen( ScenePos, true );
	}
	return;
}

void fileSelection::drawControl( QPainter* painter )
{
	painter->save();
	//************************************
	painter->setPen( Qt::NoPen );
	if( !_HIGHLIGHTCONTROL )
	{
		painter->setBrush( QBrush( QColor( 10, 10, 60, 100 ) ) );
	}
	else
	{
		painter->setBrush( QBrush( QColor( 10, 10, 60, 200 ) ) );
	}
	painter->drawRect( _Control );
	painter->setPen( Qt::RoundJoin );
	painter->setBrush( QBrush( QColor( 150, 150, 150, 100 ) ) );
	painter->setPen( QColor( 150, 150, 150, 100 ) );
	painter->drawRect( _EllipseMap );
	painter->drawRect( _EllipseColor );
	painter->drawRect( _EllipseMove );
	painter->setPen( QColor( 250, 250, 250, 200 ) );
	painter->drawText( this->_EllipseMap.bottomLeft() + QPointF( 5, -5 ), "W" );
	painter->drawText( this->_EllipseMove.bottomLeft() + QPointF( 5, -5 ), "S" );
	painter->drawText( this->_EllipseColor.bottomLeft() + QPointF( 5, -5 ), "C" );
	painter->drawText( this->_EllipseHide.bottomLeft() + QPointF( 5, -5 ), "H" );
	painter->setBrush( QBrush( QColor( 200, 10, 10, 100 ) ) );
	painter->setPen( QColor( 200, 10, 10, 100 ) );
	painter->drawRect( _EllipseHide );
	//************************************
	if( _ONCONTROLTYPE > -1 )
	{
		painter->setPen( QColor( 255, 255, 255, 255 ) );
		painter->drawText( this->_Control.bottomLeft()+QPointF(5, -3), controlName[_ONCONTROLTYPE] );
	}
	painter->restore();
	return;
}

void fileSelection::initViewRect()
{
	//************************************************
	/*this->_view.append( tr("3D") );
	this->_colors.append( QColor( 10,10,150,250) );
	this->_view.append( tr("Video1") );
	this->_colors.append( QColor( 120,20,80,250) );
	this->_view.append( tr("Video2") );
	this->_colors.append( QColor( 120,20,80,250) );
	this->_view.append( tr("Video3") );
	this->_colors.append( QColor( 120,20,80,250) );
	this->_view.append( tr("PC1") );
	this->_colors.append( QColor( 20,160,20,250) );
	this->_view.append( tr("PC2") );
	this->_colors.append( QColor( 20,160,20,250) );
	this->_view.append( tr("PC3") );	
	this->_colors.append( QColor( 20,160,20,250) );*/
	this->_view.append( tr("barChart") );
	this->_colors.append( QColor( 80,160,160,250) );
	this->_view.append( tr("lineChart") );
	this->_colors.append( QColor( 80,160,160,250) );
	this->_view.append( tr("scatterPlot"));
	this->_colors.append( QColor( 80,160,160,250) );
	this->_view.append( tr("Web"));
	this->_colors.append( QColor( 40,20,80,250) );
	/*this->_view.append( tr("Matlab"));
	this->_colors.append( QColor( 40,20,80,250) );
	this->_view.append( tr("Pen"));
	this->_colors.append( QColor( 40,20,80,250) );*/
	this->_view.append( tr("Note"));
	this->_colors.append( QColor( 40,20,80,250) );
	this->_view.append( tr("PathBubble1"));
	this->_colors.append( QColor( 40,20,80,250) );

	this->_view.append( tr("expression"));
	this->_colors.append( QColor( 40,20,80,250) );
	
	//************************************************
	int x_step = 40;
	int y_step = 60;
	int width_step = 35;
	int height_step = 50;
	int Num_Item = 5;
	//************************************************
	int initX = this->getSize().width() / 3 * 2;
	for( int i = 0; i < this->_view.size(); i++ )
	{
		this->_rectsView.append( QRectF( i%Num_Item*x_step + initX + 5, i/Num_Item*y_step + 5, width_step, height_step ) );
		this->_shadow.append( QRectF( i%Num_Item*x_step + initX + 7, i/Num_Item*y_step + 7, width_step, height_step ) );
		this->_iconArea.append( QRectF( i%Num_Item*x_step + initX + 5, i/Num_Item*y_step + 10, width_step, height_step-10 ) );
	}
	return;
}

void fileSelection::drawViews(QPainter *painter)
{
	painter->save();
	//**************************************
	painter->setPen( Qt::NoPen );
	for( int i = 0; i < this->_rectsView.size(); i++ )
	{
		painter->setBrush( QColor( 20, 20, 20, 200) );
		painter->drawRect( this->_shadow[i] );
		painter->setBrush( this->_colors[i] );
		painter->drawRect( this->_rectsView[i] );
		painter->drawImage( _iconArea[i], this->_icons[i], _iconRects[i] );
	}
	//**************************************
	if( _VISINDEX >= 0 )
	{
		if( _DIRECTPREOPEN && _VISINDEX > 0)
		{
			painter->setBrush( this->_colors[_VISINDEX] );
			painter->drawRect( _preOpenViewDirectly );
		}
		else
		{
			painter->setBrush( this->_colors[_VISINDEX] );
			painter->drawRect( _visDragRect );

		}
	}
	//**************************************
	painter->restore();
	return;
}

bool fileSelection::handlePressVisView(QPointF pos)
{
	for( int i = 0; i < this->_rectsView.size(); i++ )
	{
		if( this->_rectsView[i].contains( pos ) )
		{
			this->_VISINDEX = i;
			_visDragRect = this->_rectsView[i];
			return true;
		}
	}
	return false;
}

bool fileSelection::handleMoveVisView( QPointF pos )
{
	if (inFileView(pos))
	{
		return false;
	}

	if( _VISINDEX >= 0 )
	{
		this->_visDragRect.moveTo( pos );
		return true;
	}
	return false;
}

bool fileSelection::handleReleaseVisView(QPointF pos)
{
	for( int i = 0; i < this->_lines.size(); i++ )
	{
		if( this->_lines[i]->mouseOver( pos ) )
		{
			this->_lines[i]->addView( _VISINDEX, _colors[_VISINDEX] );

			logDragVisToCompose( i, _VISINDEX );
			_VISINDEX = -1;
		}
	}
	_VISINDEX = -1;
	return false;
}

void fileSelection::initIcons()
{
	/*this->_icons[0] = QImage( tr("icon/3d.png") );
	this->_icons[1] = QImage( tr("icon/batc.png") );
	this->_icons[2] = QImage( tr("icon/batg.png") );
	this->_icons[3] = QImage( tr("icon/bath.png") );
	this->_icons[4] = QImage( tr("icon/PC1.png") );
	this->_icons[5] = QImage( tr("icon/PC2.png") );
	this->_icons[6] = QImage( tr("icon/PC3.png") );*/
	this->_icons[0] = QImage( tr("icon/bar chart.png") );
	this->_icons[1] = QImage( tr("icon/line chart.png") );
	this->_icons[2] = QImage( tr("icon/scatter plot.png") );
	this->_icons[3] = QImage( tr("icon/web.png") );
	//this->_icons[11] = QImage( tr("icon/mat.png") );
	//this->_icons[12] = QImage( tr("icon/pen.png") );
	this->_icons[4] = QImage( tr("icon/note.png") );
	this->_icons[5] = QImage( tr("icon/PathBubble1.png") );
	this->_icons[6] = QImage( tr("icon/expression.png") );

	for( int i = 0; i < _view.size(); i++ )
	{
		this->_iconRects.append( QRectF( QPointF( 0, 0 ), this->_icons[i].size() ) );
	}
	return;
}

void fileSelection::openViewDirectly( QPointF pos )
{
	int size = _preOpenViewDirectly.height();
	
	if( _VISINDEX == 0 )
	{
		this->_open->_manager->itemPosUpdated(this->_open->openBarChart( size, pos.x() + size/2, pos.y() + size/2 ));
	}
	else if( _VISINDEX == 1 )
	{
		this->_open->_manager->itemPosUpdated(this->_open->openLineChart( size, pos.x() + size/2, pos.y() + size/2 ));
	}
	else if ( _VISINDEX == 2 )
	{
		this->_open->_manager->itemPosUpdated(this->_open->openScatterPlot( size, pos.x() + size/2, pos.y() + size/2 ));
	}
	else if( _VISINDEX == 3 )
	{
		this->_open->_manager->itemPosUpdated(this->_open->openWebView( NULL, size, pos.x() + size/2, pos.y() + size/2 ));
	}
	/*else if( _VISINDEX == 4 )
	{
		//this->_open->openMatlab( size, pos.x() + size/2, pos.y() + size/2 );
	}
	else if( _VISINDEX == 5 )
	{
		//this->_open->openPen( size, pos.x() + size/2, pos.y() + size/2 );
	}*/
	else if ( _VISINDEX == 4 )
	{
		 this->_open->_manager->itemPosUpdated(this->_open->openNote( NULL, size, pos.x() + size/2, pos.y() + size/2 ));
		 
	}
	else if ( _VISINDEX == 5 )
	{
	     this->_open->_manager->itemPosUpdated(this->_open->openPathBubble1( 400, pos.x() + size/2, pos.y() + size/2));
	}
	else if ( _VISINDEX == 6 )
	{
		this->_open->_manager->itemPosUpdated(this->_open->openExpressionBubble( size, pos.x() + size/2, pos.y() + size/2 ));
	}
	return;
}

void fileSelection::drawPreOpenViewDirectly(QPointF pos)
{
	if( this->boundingRect().contains( pos ))
	{
		return ;
	}
	else
	{
		_preOpenViewDirectly = this->_open->queryPreOpen( pos, 1 )[0];
	}
}

void fileSelection::drawColorBar( QPainter* painter )
{
	if( !COLORBAR )
	{
		return;
	}
	painter->save();
	//*********************************************
	myColor3 color;
	for( int i = 0; i<100; i++)
	{
		Lab3D point = this->_scene->getCut()->get_Position( float( i /100.0) );
		this->_scene->getLab()->get_Color( point, color );
		painter->setBrush( QColor( color.a, color.b, color.c, 255) );
		painter->setPen( QColor( color.a, color.b, color.c, 255) );
		painter->drawRect( this->getSize().width()/3 + i*2, this->getSize().height(), 2, 20 );
	}
	//*********************************************
	painter->setFont( QFont( "times", 12) );
	painter->setPen( QColor( 255, 255, 255, 255) );
	painter->drawText( this->getSize().width()/3 + 165, this->getSize().height(), tr("100%") );
	painter->drawText( this->getSize().width()/3, this->getSize().height(), tr("0%") );
	painter->restore();
	return;
}

void fileSelection::OpenFileDirectly(QPointF pos)
{
	_DRAGFILE = false;
	this->_manager->getRoot()->disableDrag();

	if (this->_open->changeFile(pos, _CurrentFileName))
	{
		return ;
	}
	else if ( this->_scene->changePlotAttribute(pos, _CurrentFileName) )
	{
		return ;
	}
	/*else
	{
		QList<int> views;
		views.append(0);	//default 3D view

		addNewToDrag();
		int lindex = this->_lines.size()-1;
		this->_lines[lindex]->addView(0, _colors[0]);

		this->_open->openDataInViewAt(_CurrentFileName, views, pos);
	}*/
}

void fileSelection::changeView( QPointF pos, int visindex )
{
	this->_open->changeView(pos, visindex);
}