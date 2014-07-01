#include "GroupBubble4.h"
#include "openglscene.h"
#include "PathBubble1.h"
//#include "matrix.h"

GroupBubble4::GroupBubble4(int pathwayID, int size, int x, int y, vector<int> ItemSelected, vector<vector<int>> EdgeSelected, OpenGLScene* scene, OpenManager* open, QString name) : ItemBase( size, x, y)
{
	//this->_REMOVECONTROL = true;
	this->_scene = scene;
	this->_TYPE = GROUPBUBBLE4;
	this->bIndex = _scene->getBubbleIndex();
	
	this->_name = name;
	this->_open = open;
	
	this->edgeSelected=EdgeSelected;
	this->m_pathwayID = pathwayID;
	
  	_LABEL = false;
	_codeParent = NULL;
	_pathBubbleParent = NULL; 

	initItemPos(ItemSelected);
	initUI();
	
	this->resizeItem( size, size*H/W);	
	return;
}


void GroupBubble4::initItemPos(vector<int> ItemSelected)
{
	//vector<int> item(2,0);
	L=100000, B=100000;
	float R=-100000, T=-100000;
	vector<int> node(2,0);	
	for(int i=0; i<ItemSelected.size(); i+=2)
	{
	    int type=ItemSelected[i], id=ItemSelected[i+1];		
		float xl,yb,xr,yt;		
		switch(type)
		{ 
		    case 'C': xl=_scene->_complexPos[m_pathwayID][id].left();        yb=_scene->_complexPos[m_pathwayID][id].bottom();        xr=_scene->_complexPos[m_pathwayID][id].right();        yt=_scene->_complexPos[m_pathwayID][id].top();       break;
		    case 'E': xl=_scene->_physicalEntityPos[m_pathwayID][id].left(); yb=_scene->_physicalEntityPos[m_pathwayID][id].bottom(); xr=_scene->_physicalEntityPos[m_pathwayID][id].right(); yt=_scene->_physicalEntityPos[m_pathwayID][id].top();break;
		    case 'P': xl=_scene->_proteinPos[m_pathwayID][id].left();        yb=_scene->_proteinPos[m_pathwayID][id].bottom();        xr=_scene->_proteinPos[m_pathwayID][id].right();        yt=_scene->_proteinPos[m_pathwayID][id].top();       break; 
		    case 'S': xl=_scene->_smallMoleculePos[m_pathwayID][id].left();  yb=_scene->_smallMoleculePos[m_pathwayID][id].bottom();  xr=_scene->_smallMoleculePos[m_pathwayID][id].right();  yt=_scene->_smallMoleculePos[m_pathwayID][id].top(); break; 
		    case 'D': xl=_scene->_DnaPos[m_pathwayID][id].left();            yb=_scene->_DnaPos[m_pathwayID][id].bottom();            xr=_scene->_DnaPos[m_pathwayID][id].right();            yt=_scene->_DnaPos[m_pathwayID][id].top();           break; 
		    case 'R': xl=_scene->_reactionPos[m_pathwayID][id].left();       yb=_scene->_reactionPos[m_pathwayID][id].bottom();       xr=_scene->_reactionPos[m_pathwayID][id].right();       yt=_scene->_reactionPos[m_pathwayID][id].top();      break;
		}
		if(L>xl)  L=xl;  if(R<xr) R=xr;  
		if(B>yt)  B=yt;  if(T<yb) T=yb;	
		
		node[0]=type, node[1]=id;
		itemInfo.push_back(node);
	}	
	W = R-L;  H = T-B;
	//S= W>H ? W:H;
}

GroupBubble4::~GroupBubble4()
{
	//delete this->_check;
}

void GroupBubble4::deleteSelectedItems(QGraphicsScene *scene)
{
	if (!scene)
		return;

	for (int i = 0; i < _groupbubbles.size(); i ++)
	{		
		ItemBase *itemBase = dynamic_cast<ItemBase *>(_groupbubbles[i]);
		itemBase->hide();
		itemBase->properRemove();
		//delete itemBase;
	}
	if(_pathBubbleParent!=NULL)
	{
		for (int i = 0; i < _pathBubbleParent->_groupBubbles.size(); i ++)
		{
			if(_pathBubbleParent->_groupBubbles[i]==this)
            {	
				_pathBubbleParent->_groupBubbles.removeOne(this);
		        _pathBubbleParent->_groupPoint.removeOne(_pathBubbleParent->_groupPoint[i]);
				break;
			}
		}
	}
	_groupbubbles.clear();
	_conPoint.clear();
	ItemBase::deleteSelectedItems(scene);	
}

void GroupBubble4::deleteSelectedItemsbyParent(QGraphicsScene *scene)
{
	if (!scene)
		return;

	for (int i = 0; i < _groupbubbles.size(); i ++)
	{		
		ItemBase *itemBase = dynamic_cast<ItemBase *>(_groupbubbles[i]);
		itemBase->hide();
		itemBase->properRemove();		
		//delete itemBase;
	}
	if(_pathBubbleParent!=NULL)
	{
		for (int i = 0; i < _pathBubbleParent->_groupBubbles.size(); i ++)
		{
			if(_pathBubbleParent->_groupBubbles[i]==this)
            {	
				_pathBubbleParent->_groupBubbles.removeOne(this);
		        _pathBubbleParent->_groupPoint.removeOne(_pathBubbleParent->_groupPoint[i]);
				break;
			}
		}
	}
	_groupbubbles.clear();
	_conPoint.clear();

	ItemBase *itemBase = dynamic_cast<ItemBase *>(this);
		itemBase->hide();
		itemBase->properRemove();	
		//delete itemBase;
	//ItemBase::deleteSelectedItems(scene);	
}

void GroupBubble4::logOpen()
{
	ItemBase::logOpen();
	
	emit logInfor("\n");
}

void GroupBubble4::logFindDeclaration( QString str )
{
	QString logtext;
	
	logtext.append( headofLogMsg() );
	logtext.append( "Find the declaration of the function " );
	logtext.append( str );
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void GroupBubble4::logAddtoParaBar( QString str )
{
	QString logtext;
	
	logtext.append( headofLogMsg() );
	logtext.append( "Add " );
	logtext.append( str );
	logtext.append( " to the Parameter Bar");
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void GroupBubble4::logRunCode()
{
	QString logtext;

	logtext.append( headofLogMsg() );
	logtext.append( "Run the matlab code" );
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void GroupBubble4::logOpenFile( QString str )
{
	QString logtext;

	logtext.append( headofLogMsg() );
	logtext.append( "Open the file " );
	logtext.append( str );
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void GroupBubble4::logSaveFile( QString str )
{
	QString logtext;

	logtext.append( headofLogMsg() );
	logtext.append( "Save the content in the file " );
	logtext.append( str );
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void GroupBubble4::logNewFile()
{
	QString logtext;

	logtext.append( headofLogMsg() );
	logtext.append( "Open a new file" );
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void GroupBubble4::logShowCheck()
{
	QString logtext;

	logtext.append( headofLogMsg() );
	logtext.append( "Show the bubble for result checking" );
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void GroupBubble4::logHideCheck()
{
	QString logtext;

	logtext.append( headofLogMsg() );
	logtext.append( "Hide the bubble for result checking" );
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void GroupBubble4::logCopy()
{
	QString logtext;

	logtext.append( headofLogMsg() );
	logtext.append( "Copy the matlab bubble" );
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void GroupBubble4::initUI()
{
	/*this->_check = new VariableCheck( this->pos().x() + MATSIZE, this->pos().y(), this->_scene );
	this->_check->myHide();

	this->_scene->addItem( this->_check );
	connect( this->_check, SIGNAL(query(QString)), this, SLOT(checkByName(QString)) );

	//			init UI first
	this->_text = new QTextEdit( );
	_text->setStyleSheet( "background-color: rgb(240, 250, 250)");

	//_text->setStyleSheet( "background: qlineargradient(x1: 0, y1: 100, x2: 0, y2: 0, stop: 0 #77c3e5, stop: 1 #5ea9cc");

	_text->setMask( this->roundRect( QRect(0, 0, m_size_width, m_size_height), 20) );
	this->_hightlight = new Highlighter( this->_text->document());
	connect( this->_text, SIGNAL(textChanged()), this, SLOT(updatePath()) );
	//connect( this->_text, SIGNAL( ), this, updatePos() );//command off by keqqin to fix the error 'cannot convert from 'void' to 'const char *'
	
	this->_myControl = this->_scene->addWidget( _text );*/

	return;
}

void GroupBubble4::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
	ItemBase::mousePressEvent( event );
	if( onBoarder( event->pos() ) )
	{
		ItemBase::mousePressEvent( event );
	}
	else
	{
		event->setAccepted( false );
	}
	//QTextLine line=currentTextLine(_text->textCursor());

	/*_text->textCursor().setPosition(2);
	QTextBlock block=_text->document()->findBlockByLineNumber(1);

	const QTextLayout *layout = block.layout();
	
	QTextLine line=layout->lineForTextPosition(9);

	QPointF pos=line.position();
	int witdh=line.width();
	int height=line.height();
	//QPointF pos=block.position();

	int num=block.position();
	int linenum=line.lineNumber();
	QRectF rect=line.naturalTextRect();
	

	QTextCursor cursor(_text->textCursor());
	cursor.movePosition(QTextCursor::Start);

	QPoint p(event->pos().x(),event->pos().y());
	QString s = _text->anchorAt( p ) ;
  
    s=s;
	bool flag=block.isVisible();
	*/
	////////////////////////////////////	
}

void GroupBubble4::showIterm( QPointF pos, QPointF scenePos, QString name)
{
	/*float x=pos.x(), y=pos.y();	
	
	QString fileName("");
	fileName.append(_name);
	fileName.chop(4);
	fileName.append("&");
	fileName.append(name);
	fileName.append(".txt");
	QByteArray ba = fileName.toLocal8Bit();
    const char *name_str = ba.data();
	
	FILE *fp = fopen(name_str,"r"); 
	if(fp!=NULL)
	{
		_open->openGroupBubble4(100, 100, 150, fileName);
	}*/
}


void GroupBubble4::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	ItemBase::mouseReleaseEvent( event );
	if( onBoarder( event->pos() ) )
	{
		//ItemBase::mouseReleaseEvent( event );
	}
	else
	{

		//QTextLine line=currentTextLine(_text->textCursor());
		event->setAccepted( false );
	}
	return;
}


void GroupBubble4::contentsMouseMoveEvent(QMouseEvent* event)
{
   QPoint p = event->pos();

   
   //QString s = _text->anchorAt( p ) ;
  
   //s=s;
}



QTextLine GroupBubble4::currentTextLine(const QTextCursor &cursor)
{
	const QTextBlock block = cursor.block();
	if (!block.isValid())
		return QTextLine();
	const QTextLayout *layout = block.layout();
	if (!layout)
		return QTextLine();
	const int relativePos = cursor.position() - block.position();
	return layout->lineForTextPosition(relativePos);
}

void GroupBubble4::mouseMoveEvent( QGraphicsSceneMouseEvent *event )
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

		if ( rightClicked )
		{
			emit unGroup( this );
		}
		else
		{
			matItemPosMoved( offset );
		}

		emit itemPosMoved( this, offset );
	}
	return;
}

void GroupBubble4::matItemPosMoved( QPointF offset )
{
	if (this->_groupbubbles.size() > 0)
	{
		for (int i = 0; i < this->_groupbubbles.size(); i ++)
		{
			this->_groupbubbles[i]->mySetPos( _groupbubbles[i]->pos() + offset );
			this->_groupbubbles[i]->matItemPosMoved( offset );	
		}
	}
}

void GroupBubble4::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	painter->save();



	this->drawConnections( painter );	
	
	if (this->_codeParent == NULL)
	{
		ItemBase::paint( painter, option, widget );
	}
	else
	{
		this->drawBubbleWithoutHalo( painter, option, widget );
		for (int i = 0; i < this->_groupbubbles.size(); i ++)
		{
			this->_groupbubbles[i]->setColor( this->getColor() );
		}
	}
	//painter->drawRect( realRect());
	paintItem(painter);	
	painter->restore();
}

void GroupBubble4::paintItem(QPainter *painter)
{
	//4 edge	
	int width=this->realRect().width()/2;
    int height=this->realRect().height()/2;
	QPointF start,middle,end;
	int type, id, type1, id1, type2, id2;
	float x,y;

	for(int i=0; i<edgeSelected.size(); i++)
	{
         //painter->drawRect(reactionPos[i].x(),reactionPos[i].y(),2,2);				 
		 type=edgeSelected[i][0], id=edgeSelected[i][1];		
		 type1=edgeSelected[i][4],	id1=edgeSelected[i][5];
		 type2=edgeSelected[i][2], id2=edgeSelected[i][3];	
		
		 //if( itemSet.find(item) != itemSet.end() && itemSet.find(item2) != itemSet.end() )
		 {
			
			QColor color=getEdgeColorByPathway(edgeSelected[i][6]);
			painter->setPen(color);
			switch(type)
			{
		     case 'C': start=_scene->_complexPos[m_pathwayID][id].center();         break;
			 case 'D': start=_scene->_DnaPos[m_pathwayID][id].center();             break;
			 case 'E': start=_scene->_physicalEntityPos[m_pathwayID][id].center();         break; 
			 case 'P': start=_scene->_proteinPos[m_pathwayID][id].center();         break;
			 case 'S': start=_scene->_smallMoleculePos[m_pathwayID][id].center();   break;			
			}
		
			if(type1=='R') 
				 middle=_scene->_reactionPos[m_pathwayID][id1].center();
			//else middle degeneration???

		 	switch(type2)
			{
		     case 'C': end=_scene->_complexPos[m_pathwayID][id2].center();         break;
			 case 'D': end=_scene->_DnaPos[m_pathwayID][id2].center();             break;
			 case 'E': end=_scene->_physicalEntityPos[m_pathwayID][id2].center();         break; 
			 case 'P': end=_scene->_proteinPos[m_pathwayID][id2].center();         break;
			 case 'S': end=_scene->_smallMoleculePos[m_pathwayID][id2].center();   break;			
			 case 'R': end=_scene->_reactionPos[m_pathwayID][id2].center();   break;  //catalysis for bio reaction	
			}
		 
			x=(start.x()-L)/W; y=(start.y()-B)/H;
			start=QPointF(x*width*2-width, y*height*2-height);
			x=(middle.x()-L)/W; y=(middle.y()-B)/H;
			middle=QPointF(x*width*2-width, y*height*2-height);
			x=(end.x()-L)/W; y=(end.y()-B)/H;
			end=QPointF(x*width*2-width, y*height*2-height);

		 	if(start.x()>=-width&&middle.x()>=-width&&end.x()>=-width)
			{
				painter->drawLine(start,middle);
				painter->drawLine(middle,end);		 

				painter->setBrush(QColor(255,255,255,255));
				if(type=='R')
				{
					int R=4;
					float dx,dy,ds;
					ds=sqrt(pow(start.x()-middle.x(),2)+pow(start.y()-middle.y(),2));
					dx=(start.x()-middle.x())/ds, dy=(start.y()-middle.y())/ds;
					end.setX(middle.x()+dx*R*2), end.setY(middle.y()+dy*R*2);
					painter->drawEllipse(end,R,R);
				}
				else
				{
					drawArrow(painter, middle, end, width, height, QColor(0, 0, 0, 255) );		 
				}
			}
			else
			{
			
			  end=end;
			}
		}
	}
	itemPos.clear();	

	for(int i=0; i<itemInfo.size(); i++)
	{
	    int type=itemInfo[i][0], id=itemInfo[i][1];
		switch(type)
		{   
		    case 'C': itemPos.push_back(paintNode( painter, _scene->_complexPos[m_pathwayID][id], _scene->_complexNameD[dataID][id][0], type, id,_scene->getGroup(this)->sharedSets,_scene->getGroup(this)->expressedSets, fixedSize, QPointF(0,0), 1.0, "", L, B, W, H)); break;
		    case 'E': itemPos.push_back(paintNode( painter,  _scene->_physicalEntityPos[m_pathwayID][id], _scene->_physicalEntityNameD[dataID][id][0], type, id,_scene->getGroup(this)->sharedSets,_scene->getGroup(this)->expressedSets, fixedSize, QPointF(0,0), 1.0, "", L, B, W, H)); break;
		    case 'P': itemPos.push_back(paintNode( painter, _scene->_proteinPos[m_pathwayID][id], _scene->_proteinNameD[dataID][id][0], type, id,_scene->getGroup(this)->sharedSets,_scene->getGroup(this)->expressedSets, fixedSize, QPointF(0,0), 1.0,  "", L, B, W, H)); break;
		    case 'S': itemPos.push_back(paintNode(painter, _scene->_smallMoleculePos[m_pathwayID][id], _scene-> _smallMoleculeNameD[dataID][id][0], type, id,_scene->getGroup(this)->sharedSets,_scene->getGroup(this)->expressedSets,fixedSize,  QPointF(0,0), 1.0,  "", L, B, W, H)); break;
		    case 'D': itemPos.push_back(paintNode(painter, _scene->_DnaPos[m_pathwayID][id], _scene-> _DnaNameD[dataID][id][0], type, id,_scene->getGroup(this)->sharedSets,_scene->getGroup(this)->expressedSets, fixedSize, QPointF(0,0), 1.0,  "", L, B, W, H)); break;
		    case 'R': itemPos.push_back(paintNode(painter,_scene->_reactionPos[m_pathwayID][id], "", type, id,_scene->getGroup(this)->sharedSets,_scene->getGroup(this)->expressedSets, fixedSize, QPointF(0,0), 1.0,  _scene->_reactionNameD[dataID][id][0], L, B, W, H));  break;
			//case 'R':	itemPos.push_back(paintReaction(painter,_scene->_reactionPos[m_pathwayID], _scene->_biochemicalReaction[m_pathwayID], id, L, B, W, H));  break;
		}		
	}
	
}

void GroupBubble4::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
	ItemBase::hoverMoveEvent(event);	
	/*//highlighted = whichItem(event->pos(),this->Width(), this->Height());
	//int num=_text->textCursor().LineUnderCursor;
	
//QString allText = _text->toPlainText();



int count=_text->document()->blockCount();
float height=_text->document()->size().height();
int y=event->pos().y()+this->Height()/2;
if(y>50)
	y=y;
int lineid=y/(height/count);
QTextBlock block=_text->document()->findBlockByLineNumber(lineid);
block.setVisible(false);
return;*/
}

/*
void GroupBubble4::setVisable()
{
	if( this->_check->isVisible() )
	{
		this->_check->myHide();
		logHideCheck();
	}
	else
	{
		this->_check->myShow();
		logShowCheck();
	}
}
*/

/*
void GroupBubble4::finishMatlab()
{
	_results = this->_thread->getResult();
	setPlotPara();

	QString para, suffix, str;
	int st;
	for (int i  = 0; i < paraName.size(); i ++)
	{
		str = codes[i];
		st = str.indexOf("(");
		str = str.right( str.length()-st-1 );
		st = str.indexOf(")");
		str = str.left( st );

		suffix = "(run_num=";
		suffix += str;
		suffix += ")";

		for (int j = 0; j < paraName[i].size(); j ++)
		{
			para = paraName[i][j] + suffix;
			emit updateDataFromMat( para, _results[i][j].data );
		}
	}
	return;	
}
*/

/*
void GroupBubble4::runMatlab()
{
	/*this->_thread = new MatlabEngThread( this->_text->toPlainText() );
	this->_thread->setParaName( paraName );
	this->_thread->start();
	connect( this->_thread, SIGNAL(execEnd()), this, SLOT(finishMatlab()) );

	logRunCode();

	return;
}
*/

/*
void GroupBubble4::checkByName( QString name )
{
	for (int i = 0; i < paraName.size(); i ++)
	{
		for (int j = 0; j < paraName[i].size(); j ++)
		{
			if (paraName[i][j] == name)
			{
				this->_check->acceptResult( _results[i][j] );
				return;
			}
		}
	}
	return;
}
*/

/*void GroupBubble4::openMatlabFile()
{
	QString fileName = QFileDialog::getOpenFileName();
	if (!fileName.isEmpty())
		loadFile(fileName);

	int index = fileName.lastIndexOf( "/" );
	fileName.remove( 0, index+1 );

	logOpenFile( fileName );
	
}*/

/*
void GroupBubble4::saveMatlabFile()
{
	if (_curFileName.isEmpty())
	{
		saveasMatlabFile();
	}
	else
	{
		saveFile(_curFileName);
	}
}
*/

void GroupBubble4::saveFile(const QString &fileName)
{
	/*QFile file(fileName);
	if (!file.open(QFile::WriteOnly | QFile::Text))
	{
		return;
	}

	QTextStream out(&file);
	out << _text->toPlainText();

	setCurrentFile(fileName);

	QString str = fileName;
	int index = str.lastIndexOf( "/" );
	str.remove( 0, index+1 );
	logSaveFile( str );*/
}

void GroupBubble4::setCurrentFile(const QString &fileName)
{
	_curFileName = fileName;
	_curFilePath = _curFileName;
	int tindex = _curFilePath.lastIndexOf("\\");
	_curFilePath.remove(tindex+1, _curFilePath.length());
}


/*
void GroupBubble4::newMatlabFile()
{
	if (maybeSave())
	{
		_curFileName.clear();
		_curFilePath.clear();
		this->_text->setText( tr("") );

		logNewFile();
	}
}
*/

/*
void GroupBubble4::saveasMatlabFile()
{
	QString fileName = QFileDialog::getSaveFileName();
	if (!fileName.isEmpty())
	{
		saveFile(fileName);
	}	
}
*/

/*
bool GroupBubble4::maybeSave()
{
	if (_text->document()->isModified()) 
	{
		QMessageBox::StandardButton ret;
		ret = QMessageBox::warning(0, tr("MatFile"),
			tr("The file has been modified.\n"
			"Do you want to save your changes?"),
			QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

		if (ret == QMessageBox::Save)
			saveMatlabFile();
		else if (ret == QMessageBox::Cancel)
			return false;
	}
	return true;
}
*/

void GroupBubble4::updateLabel()
{
	if (_LABEL == false)
	{
		_label = new ItemLabel( this, _scene, m_size_width, m_size_height );
		_label->setLabelColor(_colorBoarder);
		this->_scene->addItem(_label);
		_LABEL = true;

		//emit updateLabelState( this );

		return;
	}
	_label->updateLabel( m_size_width, m_size_height );
	
	if (this->_codeParent)
	{
		_label->setPos( _label->pos() + QPointF(0, 12) );
		
		QString funName, tmpName;
		tmpName = this->_curFileName;
		int st = tmpName.lastIndexOf("\\");
		funName = tmpName.remove(0, st+1);
		funName.chop( 2 );
		_label->setPlainText( funName );
	}
} 

void GroupBubble4::contextMenuEvent(QGraphicsSceneContextMenuEvent *event )
{
	if (!isSelected()) 
	{
		_scene->clearSelection();
		setSelected(true);
	}
	if (this->onBoarder( event->pos() ))
	{
		QMenu menu;
		QAction *delAction = menu.addAction("Delete");
		QAction *ungroupAction = menu.addAction("Ungroup");
		QAction *copyAction = menu.addAction("Copy");
		
		if ( this->_codeParent == NULL )
		{
			copyAction->setDisabled( true );
		}

		QAction *selectedAction = menu.exec(event->screenPos());

		if (selectedAction == delAction)
		{
				QMessageBox msgBox;
				msgBox.setWindowTitle("Delete Bubble");
				msgBox.setText("Delete this bubble?");
				msgBox.setStandardButtons(QMessageBox::Yes);
				//msgBox.setDefaultButton(QMessageBox::No);
msgBox.addButton (QMessageBox::No);
if(msgBox.exec() == QMessageBox::Yes){
 logRemove();

			deleteSelectedItems( _scene );
}else {
  // do something else
}
			
		}
		else if ( selectedAction == ungroupAction )
		{
			emit unGroup(this);
		}
		else if ( selectedAction == copyAction )
		{
			//emit setMatFileBubble( this );			
			logCopy();
		}
	}	
	/*else
	{
		QMenu menu;
		QAction *openAction = menu.addAction("Open");
		QAction *newAction = menu.addAction("New");
		QAction *saveAction = menu.addAction("Save");
		QAction *saveasAction = menu.addAction("SaveAs");
		QAction *runAction = menu.addAction("Run");
		QAction *checkAction  = menu.addAction("Check");

		if (this->_codeParent )
		{
			runAction->setDisabled( true );
			checkAction->setDisabled( true );
		}
		
		QAction *selectedAction = menu.exec(event->screenPos());

		if (selectedAction == runAction )
		{
			runMatlab();
		}
		else if (selectedAction == checkAction )
		{
			setVisable();
		}
		else if (selectedAction == openAction )
		{
			openMatlabFile();
		}
		else if (selectedAction == newAction )
		{
			newMatlabFile();
		}
		else if (selectedAction == saveAction )
		{
			saveMatlabFile();
		}
		else if (selectedAction == saveasAction )
		{
			saveasMatlabFile();
		}
	}*/
}

void GroupBubble4::drawConnections(QPainter *painter)
{
	QPointF constart, conend, tmpmid;
	int width;
	QPen pen;
	myColor3 color;

	pen.setWidth(2);
	painter->save();

	for (int i = 0; i < _groupbubbles.size(); i ++)
	{
		color = this->_groupbubbles[i]->getColor();
		pen.setColor( QColor(color.a, color.b, color.c, 100) );
		painter->setPen( pen );

		constart = this->_conPoint[i];
		constart.setX( this->Width()/2 );

		conend = _groupbubbles[i]->pos();
		conend.setX( conend.x() - _groupbubbles[i]->Width()/2);
		conend -= this->pos();
	
		width = conend.x() - constart.x();

		tmpmid.setX( constart.x() + width/2 );
		tmpmid.setY( constart.y() );

		if ( fabs(conend.y()-constart.y()) > 20)
		{
			painter->drawLine( constart, tmpmid - QPointF( 10.0, 0) );

			if ( conend.y() > constart.y() )
			{
				painter->drawArc( tmpmid.x()-20.0, tmpmid.y(), 20.0, 20.0, 0, 90*16 );
				painter->drawLine( tmpmid+QPointF( 0, 10.0), QPointF(tmpmid.x(), conend.y()-10.0));
				tmpmid.setY( conend.y() );
				painter->drawArc( tmpmid.x(), tmpmid.y()-20.0, 20.0, 20.0, 180*16, 90*16 );
			}
			else
			{
				painter->drawArc( tmpmid.x()-20.0, tmpmid.y()-20.0, 20.0, 20.0, 270*16, 90*16 );
				painter->drawLine( tmpmid-QPointF( 0, 10.0), QPointF(tmpmid.x(), conend.y()+10.0));
				tmpmid.setY( conend.y() );
				painter->drawArc( tmpmid.x(), tmpmid.y(), 20.0, 20.0, 90*16, 90*16 );
			}

			painter->drawLine( tmpmid + QPointF( 10.0, 0), conend );
		}
		else
		{
			painter->drawLine( constart, tmpmid);
			painter->drawLine( tmpmid, QPointF(tmpmid.x(), conend.y()));
			tmpmid.setY( conend.y() );
			painter->drawLine( tmpmid, conend );
		}
		

		//arrow
		if (conend.x() > constart.x())
		{
			painter->drawLine( conend, QPointF(conend.x()-10, conend.y()-4) );
			painter->drawLine( conend, QPointF(conend.x()-10, conend.y()+4) );
		}
		else
		{
 			painter->drawLine( conend, QPointF(conend.x()+10, conend.y()-4) );
 			painter->drawLine( conend, QPointF(conend.x()+10, conend.y()+4) );
		}
	}

	painter->restore();
}

void GroupBubble4::removeCodeCon( GroupBubble4 *codeBubble )
{
	if (this == NULL)	return;

	for (int i = 0; i < _groupbubbles.size(); i ++)
	{
		if (_groupbubbles[i]->_curFileName == codeBubble->_curFileName)
		{
			_groupbubbles.removeAt(i);
			_conPoint.removeAt(i);
			return ;
		}
	}
}

void GroupBubble4::removeAllCodeSon()
{
	GroupBubble4 *tmp;

	for (int i = 0; i < _groupbubbles.size(); )
	{
		tmp = _groupbubbles[i];
		
		_groupbubbles.removeAt( i );
		_conPoint.removeAt( i );

		tmp->properRemove();
		delete tmp;
	}
}

bool GroupBubble4::hasOpenMatOrNot( GroupBubble4 *para, QString name )
{
	if (para->_curFileName == name)
	{
		return true;
	}
	for (int i = 0; i < para->_groupbubbles.size(); i ++)
	{
		if ( hasOpenMatOrNot(para->_groupbubbles[i], name) )
		{
			return true;
		}
	}
	return false;
}

void GroupBubble4::updatePath()
{
	/*QString str = this->_text->toPlainText();
	int sindex, eindex;
	sindex = str.lastIndexOf("cd('");
	eindex = str.lastIndexOf("');");

	_curFilePath = str;
	_curFilePath.remove(eindex, _curFilePath.length()-eindex);
	_curFilePath.remove(0, sindex+4);
	_curFilePath += '\\';

	findFunction();*/
}

void GroupBubble4::updatePos()
{
	if ( this->_codeParent == NULL )
		return;
}

void GroupBubble4::findFunction()
{
	/*QString str = this->_text->toPlainText();
	QRegExp funExp = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
	QRegExp paraExp = QRegExp("\\s+");
	QString tmpFun, tmpStr;
	QStringList para;

	int tindex, matchlen, tt;

	codes = str.split(";");
	functionName.clear();
	paraName.clear();

	for (int i = 0; i < codes.size();)
	{
		tindex = codes[i].indexOf( funExp );
		if (tindex > -1)
		{
			matchlen = funExp.matchedLength();
			tmpFun = codes[i].left( tindex + matchlen );
			tmpFun.remove(0, tindex);
			functionName.append( tmpFun );

			tmpStr = codes[i];
			tmpStr = codes[i].left( tindex );
			para = tmpStr.split( paraExp );
			
			for (int j = 0; j < para.size();)
			{
				if (para[j] == "" || para[j] == "[" || para[j] == "]" || para[j] == "=")
				{
					para.removeAt(j);
					continue;
				}
				tt = para[j].indexOf(",");
				if (tt > -1)
				{
					para[j].remove(tt, 1);
				}
				j ++;
			}
			paraName.push_back( para );

			i++;
		}
		else
		{
			codes.removeAt(i);
		}
	}*/
}

/*void GroupBubble4::setPlotPara()
{
	GroupBubble4* tmpCodeBubble;
	
	QString fileName;

	for (int i = 0; i < functionName.size(); i ++)
	{
		if (paraName[i].size() == 2)
		{
			fileName = _curFilePath + functionName[i] + ".m";
			tmpCodeBubble = findCodeBubblebyName( fileName );

			if ( tmpCodeBubble == NULL )
			{
				break;
			}

			if (tmpCodeBubble->_dataVis == NULL)
			{
				tmpCodeBubble->_dataVis = new MatDataVisBlock( 200, tmpCodeBubble->pos().x()+tmpCodeBubble->Width()/2+100, tmpCodeBubble->pos().y(), this->_scene );
			}
			tmpCodeBubble->_dataVis->setXYValue( _results[i][0].data, _results[i][1].data );
			tmpCodeBubble->_dataVis->setXYName( paraName[i][0], paraName[i][1] );
			tmpCodeBubble->_dataVis->setCodeLine( codes[i] );
			this->_scene->addItem( tmpCodeBubble->_dataVis );
		}
	}
}
*/

GroupBubble4* GroupBubble4::findCodeBubblebyName( QString fileName )
{
	if (this->_curFileName == fileName)
	{
		return this;
	}

	GroupBubble4* mat;
	for (int i = 0; i < _groupbubbles.size(); i ++)
	{
		mat = _groupbubbles[i]->findCodeBubblebyName( fileName );
		if (mat)	return mat;
	}

	return NULL;
} 

void GroupBubble4::drawBubbleWithoutHalo( QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
	QPen pen;
	int lineWidth = BOARDERSIZE;

	painter->save();
	//painter->setRenderHint(QPainter::Antialiasing, true);

	if (option->state & QStyle::State_Selected) 
	{
		pen.setColor( QColor( _colorBoarder.a, _colorBoarder.b, _colorBoarder.c, 50 ) );
		painter->setPen( pen );
		ItemBase::drawControlHint( painter );
		painter->drawRect(realRect());
		pen.setWidth( lineWidth );
		painter->setPen( pen );
		drawBoarder( painter );
	}
	else
	{
		if( this->_highLight )
		{
			pen.setColor( QColor( _hiColor.a, _hiColor.b, _hiColor.c, 100 ) );	
			pen.setWidth( lineWidth );
			painter->setPen( pen );
			drawBoarder( painter );
		}
		else
		{
			pen.setColor( QColor( _colorBoarder.a, _colorBoarder.b, _colorBoarder.c, 100 ) );
			pen.setWidth( lineWidth );
			painter->setPen( pen );
			drawBoarder( painter );
		}
	}
	//painter->setRenderHint(QPainter::Antialiasing, false);
	ItemBase::ControlAutoHide();

	painter->restore();
}

void GroupBubble4::drawBoarder( QPainter* painter )
{
// 	painter->drawLine( QPointF(-Width()/2-BOARDERSIZE/2, -Height()/2-BOARDERSIZE/2), QPointF(Width()/2+BOARDERSIZE/2, -Height()/2-BOARDERSIZE/2) );
// 	painter->drawLine( QPointF(-Width()/2-BOARDERSIZE/2, Height()/2+BOARDERSIZE/2), QPointF(Width()/2+BOARDERSIZE/2, Height()/2+BOARDERSIZE/2) );
// 	painter->drawLine( QPointF(-Width()/2-BOARDERSIZE/2, -Height()/2 ), QPointF(-Width()/2-BOARDERSIZE/2, Height()/2 ) );
// 	painter->drawLine( QPointF(Width()/2+BOARDERSIZE/2, -Height()/2 ), QPointF(Width()/2+BOARDERSIZE/2, Height()/2 ) );

	painter->drawRoundedRect( this->realRect(), 20, 20 );
}


bool GroupBubble4::onBoarder( QPointF pos )
{
	int x = pos.x();
	int y = pos.y();
	int half = this->Width()/2;
	int half2 = this->Height()/2;
	if( (x > half - BOARDERSIZE) || (x < -half + BOARDERSIZE) )
	{
		return true;
	}
	if( (y > half2 - BOARDERSIZE) || (y < -half2 + BOARDERSIZE) )
	{
		return true;
	}
	return false;
}

void GroupBubble4::copyAttribute( GroupBubble4 *another )
{
	QString filename, tmpname,tnum, codetext, newcodetext;
	int num;

	/*filename = another->_curFileName;
	this->_curFilePath = another->_curFilePath;
	this->loadFile( filename );

	filename.remove( filename.length()-2, 2 );
	num = 1;
	while (1)
	{
		tnum.setNum(num);
		tmpname = filename + "_" + tnum + ".m";
		if ( !QFile::exists(tmpname) )
		{
			_curFileName = tmpname;
			break;
		}
		num ++;
	}

	codetext = this->_text->toPlainText();
	int index = filename.lastIndexOf("\\");
	filename.remove( 0, index+1 );
	index = codetext.indexOf( filename );
	newcodetext = codetext.left( index );
	newcodetext += filename + "_" + tnum;
	codetext.remove( 0, index + filename.length() );
	newcodetext += codetext;
	this->_text->setText( newcodetext );

	this->saveFile( _curFileName );*/

	this->_codeParent = another->_codeParent;
	this->_groupbubbles = another->_groupbubbles;
	this->_conPoint = another->_conPoint;

	GroupBubble4* _para = this->_codeParent;
	if (_para)
	{
		_para->_groupbubbles.append( this );
		_para->_conPoint.append( QPointF(_para->Width()/2, 0) );
	}
}

void GroupBubble4::removeDataVis()
{
	/*if (this->_dataVis)
	{
		this->_dataVis->properRemove();
		delete this->_dataVis;
	}*/
}

int GroupBubble4::codeIndexofParameter( QString str )
{
	int i, index;

	for (i = 0; i < paraName.size(); i ++)
	{
		index = paraName[i].indexOf( str );
		if (index > -1)	return i;
	}
	return -1;
}

void GroupBubble4::saveAsXmlNode( QDomDocument &doc, QDomElement &node )
{
	QDomText text;
	QString str, tmpStr;

	QDomElement type = doc.createElement( tr("Type") );
	text = doc.createTextNode( tr("GroupBubble4") );
	type.appendChild( text );
	node.appendChild( type );

	ItemBase::saveAsXmlNode( doc, node );

	QDomElement pathPre = doc.createElement( tr("PathPre") );
	text = doc.createTextNode( this->_curFilePath );
	pathPre.appendChild( text );
	node.appendChild( pathPre );

	QDomElement curFile = doc.createElement( tr("FileName") );
	if (_curFileName == "")
	{
		//saveasMatlabFile();
	}
	text = doc.createTextNode( this->_curFileName );
	curFile.appendChild( text );
	node.appendChild( curFile );

	if (_pathBubbleParent)
	{
		QDomElement parentBubble = doc.createElement( tr("ParentBubble") );
		QDomElement pareFile = doc.createElement( tr("FileName") );
		text = doc.createTextNode( _pathBubbleParent->getCurrentFilename() );
		pareFile.appendChild( text );
		parentBubble.appendChild( pareFile );
		QDomElement parePos = doc.createElement( tr("Position") );
		str = "(";
		tmpStr.setNum( _pathBubbleParent->pos().x() );
		str += tmpStr;
		str += ", ";
		tmpStr.setNum( _pathBubbleParent->pos().y() );
		str += tmpStr;
		str += ")";
		text = doc.createTextNode( str );
		parePos.appendChild( text );
		parentBubble.appendChild( parePos );
		node.appendChild( parentBubble );
	}

	QDomElement descentBubbles = doc.createElement( tr("DescendantBubble") );
	for (int i = 0; i < _groupbubbles.size(); i ++)
	{
		QDomElement descent = doc.createElement( tr("Descendant") );
		
		QDomAttr descentID = doc.createAttribute( tr("descID") );
		str.setNum( i+1 );
		descentID.setValue( str );
		descent.setAttributeNode( descentID );

		QDomElement descentFile = doc.createElement( tr("FileName") );
		text = doc.createTextNode( _groupbubbles[i]->getCurrentFilename() );
		descentFile.appendChild( text );
		descent.appendChild( descentFile );

		QDomElement descentPos = doc.createElement( tr("Position") );
		str = "(";
		tmpStr.setNum( _groupbubbles[i]->pos().x() );
		str += tmpStr;
		str += ", ";
		tmpStr.setNum( _groupbubbles[i]->pos().y() );
		str += tmpStr;
		str += ")";
		text = doc.createTextNode( str );
		descentPos.appendChild( text );
		descent.appendChild( descentPos );

		descentBubbles.appendChild( descent );
	}
	node.appendChild( descentBubbles );

	QDomElement connectPos = doc.createElement( tr("ConnectPosition") );
	for ( int i = 0; i < _conPoint.size(); i ++ )
	{
		QDomElement position = doc.createElement( tr("Position") );

		QDomAttr posID = doc.createAttribute( tr("posID") );
		str.setNum( i+1 );
		posID.setValue( str );
		position.setAttributeNode( posID );

		QDomElement conpoint = doc.createElement( tr("ConPoint") );
		str =  "(";
		tmpStr.setNum( _conPoint[i].x() );
		str += tmpStr;
		str += ", ";
		tmpStr.setNum( _conPoint[i].y() );
		str += tmpStr;
		str += ")";
		text = doc.createTextNode( str );
		conpoint.appendChild( text );
		position.appendChild( conpoint );

		connectPos.appendChild( position );
	}
	node.appendChild( connectPos );
}

void GroupBubble4::recoverFromXmlNode( QDomElement node )
{
	QDomNodeList paraNode, posNode;
	QString str;

	paraNode = node.elementsByTagName( tr("PathPre") );
	_curFilePath = paraNode.at(0).toElement().text();

	paraNode = node.elementsByTagName( tr("FileName") );
	_curFileName = paraNode.at(0).toElement().text();

	str = _curFileName;
	if (str.size() > 0)
	{
		//this->loadFile( str );
	}

	QStringList posStr;
	float x, y;

	_conPoint.clear();
	paraNode = node.elementsByTagName( tr("ConnectPosition") );
	
	if ( !paraNode.isEmpty() )
	{
		paraNode = paraNode.at(0).toElement().elementsByTagName( tr("Position") );
		for (int i = 0; i < paraNode.size(); i ++)
		{
			posNode = paraNode.at(i).toElement().elementsByTagName( tr("ConPoint") );
			posStr= paraNode.at(i).toElement().text().split( "," );
			str = posStr.at(0);
			str.remove( 0, 1 );
			str = str.simplified();
			x = str.toFloat();
			str = posStr.at(1);
			str.chop(1);
			str = str.simplified();
			y  = str.toFloat();

			_conPoint.append( QPointF(x, y) );
		}
	}
}

void GroupBubble4::rebuildBubbleConnect( QDomElement node )
{
	_codeParent = NULL;
	_groupbubbles.clear();

	QList<ItemBase*> itemList;
	ItemBase* item;
	GroupBubble4* tmp;
	QString name;
	int i;

	QDomNodeList paraNode;
	QDomElement tmpNode;

	itemList = this->_scene->getGroupManager()->allMembers();

	paraNode = node.elementsByTagName( tr("ParentBubble") );
	
	if ( !paraNode.isEmpty() )
	{
		tmpNode = paraNode.at(0).toElement();
		paraNode = tmpNode.elementsByTagName(tr("FileName"));
		name = paraNode.at(0).toElement().text();

		paraNode = tmpNode.elementsByTagName( tr("Position") );
		QStringList posStr= paraNode.at(0).toElement().text().split( "," );
		QString str = posStr.at(0);
		str.remove( 0, 1 );
		str = str.simplified();
		float x = str.toFloat();
		str = posStr.at(1);
		str.chop(1);
		str = str.simplified();
		float y  = str.toFloat();

		for (i = 0; i < itemList.size(); i ++)
		{
			item = itemList[i];
			if ( item->getType() == PATHBUBBLE1 )
			{
				PathBubble1* tmp = dynamic_cast<PathBubble1*>(item);
				if (//ptmp->getCurrentFilename() == name && 
					fabs(tmp->pos().x() - x) < 3
					&& fabs(tmp->pos().y() - y) < 3 )
				{
					this->_pathBubbleParent = tmp;
					break;
				}
			}
		}
	}
	
	paraNode = node.elementsByTagName( tr("DescendantBubble") );
	if (!paraNode.isEmpty())
	{
		tmpNode = paraNode.at(0).toElement();
		QDomNodeList sons = tmpNode.elementsByTagName( tr("Descendant") );

		for (int id = 0; id < sons.size(); id ++)
		{
			tmpNode = sons.at(id).toElement();

			paraNode = tmpNode.elementsByTagName(tr("FileName"));
			name = paraNode.at(0).toElement().text();

			paraNode = tmpNode.elementsByTagName( tr("Position") );
			QStringList posStr= paraNode.at(0).toElement().text().split( "," );
			QString str = posStr.at(0);
			str.remove( 0, 1 );
			str = str.simplified();
			float x = str.toFloat();
			str = posStr.at(1);
			str.chop(1);
			str = str.simplified();
			float y  = str.toFloat();

			for ( i = 0; i < itemList.size(); i ++ )
			{
				item = itemList[i];
				if ( item->getType() == REACTIONBUBBLE3)
				{
					tmp = dynamic_cast<GroupBubble4*>(item);
					if (tmp->getCurrentFilename() == name 
						&& fabs(tmp->pos().x() - x) < 5
						&& fabs(tmp->pos().y() - y) < 5 )
					{
						this->_groupbubbles.append(tmp);
						break;
					}
				}
			}
		}
	}
}

/*void GroupBubble4::resizeItem( int size1, int size2 )
{
	ItemBase::resizeItem( size1, size2 );
	this->_myControl->resize( size1, size2 );
	//_text->setMask( roundRect( QRect(0, 0, m_size_width, m_size_height), 20) );
}*/