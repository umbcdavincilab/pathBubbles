#include "textBubble2.h"
#include "PathBubble1.h"
#include "openglscene.h"
//#include "matrix.h"

TextBubble2::TextBubble2(int parentPathID, int parentCodeID, int size, int x, int y, OpenGLScene* scene, OpenManager* open, QString name) : ItemBase( MATSIZE, x, y)
{
	_pathBubbleParent=NULL;
	_codeParent=NULL;

	this->_REMOVECONTROL = true;
	this->_scene = scene;
	this->_TYPE = TEXTBUBBLE2;
	this->bIndex = _scene->getBubbleIndex();
	//this->_thread = NULL;
	this->_name = name;
	this->_open = open;

	m_codeBubbleID=this->_scene->_codeBubbles.size();
	
	m_parentTextBubbleID=parentCodeID;
	m_parentPathBubbleID=parentPathID;
	if(parentCodeID>=0)
	   this->_codeParent = this->_scene->_codeBubbles[parentCodeID];	
	
	if(parentPathID>=0)
	    this->_pathBubbleParent = this->_scene->_pathBubbles[parentPathID];

	_queryPos=QPointF(0,0), _queryScenePos=QPointF(0,0);
	initUI();
	loadFile(name);
	
	QTextCursor cursor(_text->textCursor());
	cursor.movePosition(QTextCursor::Start);

	_LABEL = false;
	//_codeParent = NULL;
	
	connect(_text, SIGNAL(cursorPositionChanged()), this, SLOT(on_textEdit_clicked()));

	this->resizeItem( size, size/1.618);
	this->_colorBoarder = TDHelper::getBoarderColorByType(_TYPE);

	//updateLabel();
	return;
}


TextBubble2::~TextBubble2()
{
	_codeBubbles.clear();
	_conPoint.clear();
	//delete [] this->_text;
	//int a;
	//a=1;
}

QString TextBubble2::on_textEdit_clicked()
{   
	QTextCursor cursor;
	//QString text;
	cursor = _text->textCursor();
	cursor.movePosition(QTextCursor::StartOfBlock);
	cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);

	QString qstr;
		
	qstr =	cursor.selectedText(); //on_textEdit_clicked();
	if(qstr.size()!=0)
	{
		int end=qstr.indexOf(" ", 0); 
		qstr.chop(qstr.length() - end);
	    showIterm(_queryPos, _queryScenePos, qstr); 
	}
	_lineClicked=whichLine(_queryPos);
	
	return(cursor.selectedText());
	//ui->label->setText(text);
}

void TextBubble2::logOpen()
{
	ItemBase::logOpen();
	
	emit logInfor("\n");
}

void TextBubble2::logFindDeclaration( QString str )
{
	QString logtext;
	
	logtext.append( headofLogMsg() );
	logtext.append( "Find the declaration of the function " );
	logtext.append( str );
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void TextBubble2::logAddtoParaBar( QString str )
{
	QString logtext;
	
	logtext.append( headofLogMsg() );
	logtext.append( "Add " );
	logtext.append( str );
	logtext.append( " to the Parameter Bar");
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void TextBubble2::logRunCode()
{
	QString logtext;

	logtext.append( headofLogMsg() );
	logtext.append( "Run the matlab code" );
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void TextBubble2::logOpenFile( QString str )
{
	QString logtext;

	logtext.append( headofLogMsg() );
	logtext.append( "Open the file " );
	logtext.append( str );
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void TextBubble2::logSaveFile( QString str )
{
	QString logtext;

	logtext.append( headofLogMsg() );
	logtext.append( "Save the content in the file " );
	logtext.append( str );
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void TextBubble2::logNewFile()
{
	QString logtext;

	logtext.append( headofLogMsg() );
	logtext.append( "Open a new file" );
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void TextBubble2::logShowCheck()
{
	QString logtext;

	logtext.append( headofLogMsg() );
	logtext.append( "Show the bubble for result checking" );
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void TextBubble2::logHideCheck()
{
	QString logtext;

	logtext.append( headofLogMsg() );
	logtext.append( "Hide the bubble for result checking" );
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void TextBubble2::logCopy()
{
	QString logtext;

	logtext.append( headofLogMsg() );
	logtext.append( "Copy the matlab bubble" );
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void TextBubble2::initUI()
{
	/*this->_check = new VariableCheck( this->pos().x() + MATSIZE, this->pos().y(), this->_scene );
	this->_check->myHide();

	this->_scene->addItem( this->_check );
	connect( this->_check, SIGNAL(query(QString)), this, SLOT(checkByName(QString)) );*/

	//			init UI first
	this->_text = new QTextEdit( );
	_text->setStyleSheet( "background-color: rgb(240, 250, 250)");

	//_text->setStyleSheet( "background: qlineargradient(x1: 0, y1: 100, x2: 0, y2: 0, stop: 0 #77c3e5, stop: 1 #5ea9cc");

	//_text->setMask( this->roundRect( QRect(0, 0, m_size_width, m_size_height), 20) );
	this->_hightlight = new Highlighter( this->_text->document());
	connect( this->_text, SIGNAL(textChanged()), this, SLOT(updatePath()) );
	//connect( this->_text, SIGNAL( ), this, updatePos() );//command off by keqqin to fix the error 'cannot convert from 'void' to 'const char *'

	this->_myControl = this->_scene->addWidget( _text );

	return;
}

bool TextBubble2::onBoarder( QPointF pos )
{
	int x = pos.x();
	int y = pos.y();
	int half = this->Width()/2;
	int half2 = this->Height()/2;
	
	if( (x > half  && x < half+HALOWIDTH) && boarderRight) 		
	{
		return true;
	}
	/*if( (x < -half && x > -half-HALOWIDTH) && boarderLeft) 
	{
	    return true;	
	}*/

	if( (y > half2  && y < half2+HALOWIDTH) && boarderUp  )
	{
		return true;
	}
	if( (y < -half2  && y > -half2-HALOWIDTH) && boarderDown )
		return true;
	return false;
}

void TextBubble2::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
	//ItemBase::mousePressEvent( event );
	if( onBoarder( event->pos() ) )
	{
		ItemBase::mousePressEvent( event );
		//event->setAccepted( false );
	}
	else
	{
		event->setAccepted( false );
	}
	
	_queryPos=event->pos(), _queryScenePos=event->scenePos();
	return;
	
}

int TextBubble2::whichLine(QPointF pos)
{
	//QString qstr;
		
	/*qstr =	on_textEdit_clicked();
	if(qstr.size()!=0)
	{
		int end=qstr.indexOf(" ", 0); 
		qstr.chop(qstr.length() - end);
	    showIterm(event->pos(), event->scenePos(), qstr); 
	}
	*/

	QTextBlock block=_text->document()->findBlockByLineNumber(0);
	const QTextLayout * layout = block.layout();	
	QTextLine line=layout->lineForTextPosition(0);
	int height=line.height();
	QPointF fPos=layout->position();

	QScrollBar *vScrollBar = _text->verticalScrollBar();
	float value = vScrollBar->value();
	float ma=vScrollBar->maximum(),mi=vScrollBar->minimum(),ps=vScrollBar->pageStep();
	int bnum=_text->document()->blockCount();
	int y=pos.y()+this->Height()/2 + value/(ma-mi) * (bnum*height - ps);

	return(y>0?(y-fPos.y())/height:-1);	
}

void TextBubble2::getAllPSTextBubble(TextBubble2* bubble, QList<ItemBase *> &list)//bubble in the whole tree hierarchy
{
    //get top
	list.clear();
	TextBubble2* top = getTopTextBubble(bubble);
	getAllSubTextBubble(top , list);
	list.append(top);
}

void TextBubble2::getHigherTextBubble(TextBubble2* bubble, QList<ItemBase *> &list) //bubble in higher level of the whole tree hierarchy
{
	list.clear();
	QList<ItemBase *> SList, AList;
	if(bubble->m_parentTextBubbleID<0) 
		return;
		
	getAllSubTextBubble(_scene->_codeBubbles[m_parentTextBubbleID], SList);
	SList.append(bubble); //itself

	int level=getTextBubbleLevel(bubble);

	getAllPSTextBubble(bubble, AList);	
	for(int i=0; i<SList.size(); i++)
	{
        AList.removeOne(SList[i]);	
	}

	for(int i=0; i<AList.size(); i++)
	{
		if(getTextBubbleLevel(dynamic_cast<TextBubble2*>(AList[i]))<level)
           list.append(AList[i]);
	}
	
}

TextBubble2* TextBubble2::getTopTextBubble(TextBubble2* bubble)
{
	if(bubble->m_parentTextBubbleID<0)
		return bubble;
	return getTopTextBubble(_scene->_codeBubbles[bubble->m_parentTextBubbleID]);
}

PathBubble1* TextBubble2::getTopPathBubble(TextBubble2* bubble)
{
	TextBubble2* tbubble=bubble->getTopTextBubble(bubble);	
	return _scene->_pathBubbles[bubble->m_parentPathBubbleID];
}


int TextBubble2::getTextBubbleLevel(TextBubble2* bubble)
{ //1
	if(bubble->m_parentTextBubbleID<0)
		return 1;
	else 
	{
		return (getTextBubbleLevel(_scene->_codeBubbles[bubble->m_parentTextBubbleID]) + 1);		
	}
}



void TextBubble2::showIterm( QPointF Pos, QPointF scenePos, QString name)
{
	float x=Pos.x(), y=Pos.y();	
	
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
		//assign pos to textBubble
		int px, py;
		if (_codeBubbles.size() == 0)
		{
			px = this->Width();				
			py = pos().y(); // - this->Height()/2;
	    }
	    else
	    {
		  	px =  this->Width(); //_codeBubbles.last()->pos().x();
		 	py =  pos().y();	//_codeBubbles.last()->pos().y()+_codeBubbles.last()->Height()/2 + MATSIZE*0.618/2+20;
	    }
	    TextBubble2 *para;
	    para = this;
	    while ( para->getCodeParent() )
	    {
		    para = para->getCodeParent();
	    }
	    if ( ! hasOpenMatOrNot( para, fileName ) )
	    {
		  TextBubble2 *txtcode = dynamic_cast<TextBubble2*>(this->_scene->getOpenManager()->openTextBubble2(-1, m_codeBubbleID, 200, px, py, fileName)); //openMatlabByPath( MATSIZE, px, py, selectedtext));
		  if ( txtcode )
		  {
			 QPoint pos = this->_scene->getSonPosition(txtcode, this, txtcode->sceneBoundingRect().x(), txtcode->sceneBoundingRect().y(), txtcode->sceneBoundingRect().width(), txtcode->sceneBoundingRect().height());
			 resetOriRect();
			 //txtcode->_codeParent = this;			 
			 this->_conPoint.append( QPointF((Pos.x()+this->Width()/2)/this->Width(), (Pos.y()+this->Height()/2)/this->Height()));			 
			 this->_codeBubbles.append( txtcode );
			 this->_bubbleLine.append(_lineClicked);
			 _scene->addToGroup( this, txtcode, false);
		  }
	  }		
	}
	
}


void TextBubble2::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	ItemBase::mouseReleaseEvent( event );
	if( onBoarder( event->pos() ) )
	{
		//ItemBase::mouseReleaseEvent( event );
	}
	else
	{
		QTextLine line=currentTextLine(_text->textCursor());
		event->setAccepted( false );
	}
	return;
}


void TextBubble2::contentsMouseMoveEvent(QMouseEvent* event)
{
   QPoint p = event->pos();

   
   QString s = _text->anchorAt( p ) ;
  
   s=s;
}



QTextLine TextBubble2::currentTextLine(const QTextCursor &cursor)
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

void TextBubble2::mouseMoveEvent( QGraphicsSceneMouseEvent *event )
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

void TextBubble2::matItemPosMoved( QPointF offset )
{
	if (this->_codeBubbles.size() > 0)
	{
		for (int i = 0; i < this->_codeBubbles.size(); i ++)
		{
			this->_codeBubbles[i]->mySetPos( _codeBubbles[i]->pos() + offset );
			this->_codeBubbles[i]->matItemPosMoved( offset );	
		}
	}
}

void TextBubble2::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	painter->save();
    
	//updateLabel();
	QRegion reg=getClipRegionforCalloutNote();
	painter->setClipRegion(reg);

	this->_myControl->setPos( this->pos().x() - this->Width()/2, this->pos().y() - this->Height()/2 );

	if (this->_codeParent == NULL)
	{
		ItemBase::paint( painter, option, widget );
	}
	else
	{
		this->drawBubbleWithoutHalo( painter, option, widget );
		for (int i = 0; i < this->_codeBubbles.size(); i ++)
		{
			this->_codeBubbles[i]->setColor( this->getColor() );
		}
	}
	drawConnections( painter );	
	
	highlightLineClicked(painter, _lineClicked, QColor( _colorBoarder.a, _colorBoarder.b, _colorBoarder.c, 200 ) );
	
	//painter->drawRect( realRect());	
	painter->restore();
}


void TextBubble2::highlightLineClicked(QPainter *painter, int lineClicked, QColor c)
{
	if(lineClicked<0)
		return;

	QTextBlock block=_text->document()->findBlockByLineNumber(0);
	const QTextLayout * layout = block.layout();	
	QTextLine line=layout->lineForTextPosition(0);
	int height=line.height();
	QPointF fPos=layout->position();

	//QScrollBar *vScrollBar = _text->verticalScrollBar();
	//float value = vScrollBar->value();
	//float ma= vScrollBar->maximum(), mi=vScrollBar->minimum(), ps=vScrollBar->pageStep();
	//int bnum= _text->document()->blockCount();
	int y = lineClicked * height + fPos.y() - this->Height()/2 ;
	int x= - this->Width()/2;
	int w=  this->Width();

	painter->setPen(c);
	painter->drawRect(x,y,w,height);

	
}

void TextBubble2::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
	ItemBase::hoverMoveEvent(event);	
	//highlighted = whichItem(event->pos(),this->Width(), this->Height());
	int num=_text->textCursor().LineUnderCursor;

	if(realRect().contains(event->pos().x(),event->pos().y()))
	   _lineClicked=whichLine(event->pos());
	
    return;
}


/*
void TextBubble2::setVisable()
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
void TextBubble2::finishMatlab()
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
void TextBubble2::runMatlab()
{
	this->_thread = new MatlabEngThread( this->_text->toPlainText() );
	this->_thread->setParaName( paraName );
	this->_thread->start();
	connect( this->_thread, SIGNAL(execEnd()), this, SLOT(finishMatlab()) );

	logRunCode();

	return;
}
*/

/*
void TextBubble2::checkByName( QString name )
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

/*void TextBubble2::openMatlabFile()
{
	QString fileName = QFileDialog::getOpenFileName();
	if (!fileName.isEmpty())
		loadFile(fileName);

	int index = fileName.lastIndexOf( "/" );
	fileName.remove( 0, index+1 );

	logOpenFile( fileName );
}*/

/*void TextBubble2::saveMatlabFile()
{
	if (_curFileName.isEmpty())
	{
		saveasMatlabFile();
	}
	else
	{
		saveFile(_curFileName);
	}
}*/


void TextBubble2::loadFile(const QString &fileName)
{
	//if (maybeSave())
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
}

void TextBubble2::saveFile(const QString &fileName)
{
	QFile file(fileName);
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
	logSaveFile( str );
}

void TextBubble2::setCurrentFile(const QString &fileName)
{
	_curFileName = fileName;
	_curFilePath = _curFileName;
	int tindex = _curFilePath.lastIndexOf("\\");
	_curFilePath.remove(tindex+1, _curFilePath.length());
}

/*void TextBubble2::newMatlabFile()
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
void TextBubble2::saveasMatlabFile()
{
	QString fileName = QFileDialog::getSaveFileName();
	if (!fileName.isEmpty())
	{
		saveFile(fileName);
	}
}
*/

/*
bool TextBubble2::maybeSave()
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

void TextBubble2::updateLabel()
{
	int tindex;	
	QString temp;	
	if(!_labelInitiated)
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

		QString funName, tmpName;
		tmpName = this->_curFileName;
		int st;
		if (this->_codeParent)
		   st = tmpName.lastIndexOf("&");
		else st = tmpName.lastIndexOf("/");		
		funName = tmpName.remove(0, st+1);
		funName.chop( 4 );
		_label->setName( funName );

		//_label->setName(this->_curFileName);
		//_label->setPlainText( this->_curFileName );	
		_label->updateLabel( m_size_width, m_size_height );
		//_label->setPos( _label->pos() + QPointF(0, 12) );		
		_labelInitiated=true;
	}	
	if(!_label->isVisible())    
		_label->setVisible(true);
	/*if (_LABEL == false)
	{
		_label = new ItemLabel( this, _scene, m_size_width, m_size_height );
		_label->setLabelColor(_colorBoarder);
		this->_scene->addItem(_label);
		_LABEL = true;

		//emit updateLabelState( this );
		return;
	}
	//_LABEL = true;

	_label->updateLabel( m_size_width, m_size_height );
	
	//if (this->_codeParent)
	{
		_label->setPos( _label->pos() + QPointF(0, 12) );
		
		QString funName, tmpName;
		tmpName = this->_curFileName;
		int st;
		if (this->_codeParent)
		   st = tmpName.lastIndexOf("&");
		else st = tmpName.lastIndexOf("/");		
		funName = tmpName.remove(0, st+1);
		funName.chop( 4 );
		_label->setPlainText( funName );
	}*/
} 

void TextBubble2::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	ItemBase::mouseDoubleClickEvent(event);			
	//_scene->removeToolsinGroup(this, true);
	return;
}


void TextBubble2::contextMenuEvent(QGraphicsSceneContextMenuEvent *event )
{
	if(insideBubble(event->pos()))
	{
	   UNGROUP=false;
	   AFTERUNGROUP=false;
	}
	if(AFTERUNGROUP || m_isMoving)
	{
	   m_isMoving=false;
	   return;
	}	
	if (!isSelected()) 
	{
		_scene->clearSelection();
		setSelected(true);
	}
	vector<int> node;
	_scene->dropDownMenu0(event, this, node);
	return;
	if (this->onBoarder( event->pos() ))
	{
		QMenu menu;
		QString ss("QMenu::item {  background-color: rgba(100,100,100,255); border: 3px rgba(69,69,69,255); color: white }"); // Use background-color instead of background
        menu.setStyleSheet(ss);
        menu.setMinimumWidth(70);
		
		QList<ItemBase *> mList=_scene->getGroupMembers(this);	
		if(mList.size()>1)
		{

			QAction *delAction = menu.addAction("Delete");
			
			//QAction *ungroupAction = menu.addAction("Ungroup");
			
			QAction *selectedAction = menu.exec(event->screenPos());
			if (selectedAction == delAction)
			{
				QMessageBox msgBox;
				msgBox.setWindowTitle("Delete Bubble");
				msgBox.setText("Delete this bubble?");
				msgBox.setStandardButtons(QMessageBox::Yes);
				//msgBox.setDefaultButton(QMessageBox::No);
				msgBox.addButton (QMessageBox::No);
				msgBox.move(event->scenePos().x(), event->scenePos().y());
				if(msgBox.exec() == QMessageBox::Yes){
					 logRemove();
				deleteSelectedItems(_scene);	
			     }
			     else {
				   // do something else
			    }
			}
			//else if (selectedAction == ungroupAction)
			//{
			//   emit unGroup(this);
			//}
		}		
	    else
	    {
			QAction *delAction = menu.addAction("Delete");
			//QAction *ungroupAction = menu.addAction("Ungroup");
			
			QAction *selectedAction = menu.exec(event->screenPos());
			if (selectedAction == delAction)
			{
				QMessageBox msgBox;
				msgBox.setWindowTitle("Delete Bubble");
				msgBox.setText("Delete this bubble?");
				msgBox.setStandardButtons(QMessageBox::Yes);
				msgBox.move(event->scenePos().x(), event->scenePos().y());
				//msgBox.setDefaultButton(QMessageBox::No);
				msgBox.addButton (QMessageBox::No);
				msgBox.move(event->scenePos().x(), event->scenePos().y());
				if(msgBox.exec() == QMessageBox::Yes){
					 logRemove();
				     deleteSelectedItems(_scene);	
			     }
			     else {
				   // do something else
			    }
			}		
		}
	}

			//else if ( selectedAction == copyAction )
			//{
				//emit setMatFileBubble( this );			
			//	logCopy();
			//}
	
	/*else if ( _text->textCursor().hasSelection() )
	{
		QMenu menu;
		QAction *declarationAction = menu.addAction("go to declaration");
		QAction *addParameterAction = menu.addAction("Add to parameter bar");
		
		QAction *selectedAction = menu.exec( event->screenPos() );

		QString selectedtext = _text->textCursor().selectedText();
		if (selectedAction == declarationAction)
		{
			logFindDeclaration( selectedtext );

			selectedtext += ".m";
			selectedtext = _curFilePath + selectedtext;
		
			if (selectedtext == _curFileName)
			{
				return ;
			}
			int px, py;
			if (_codeBubbles.size() == 0)
			{
				px = pos().x() + this->Width()/2 + MATSIZE/2 + 10;
				if (this->_dataVis)
				{
					px += this->_dataVis->Width();
				}
				py = pos().y() - this->Height()/2;
			}
			else
			{
				px = _codeBubbles.last()->pos().x();
				py = _codeBubbles.last()->pos().y()+_codeBubbles.last()->Height()/2 + MATSIZE/2;
			}

			TextBubble2 *para;
			para = this;
			while ( para->getCodeParent() )
			{
				para = para->getCodeParent();
			}
			if ( ! hasOpenMatOrNot( para, selectedtext ) )
			{
				TextBubble2 *matcode = dynamic_cast<TextBubble2*>(this->_scene->getOpenManager()->openMatlabByPath( MATSIZE, px, py, selectedtext));
				if ( matcode )
				{
					matcode->_codeParent = this;
					this->_conPoint.append( event->pos() );					
					this->_codeBubbles.append( matcode );
				}
			}
		}
		else if ( selectedAction == addParameterAction )
		{
			int index = codeIndexofParameter( selectedtext );

			if (index > 0)
			{
				logAddtoParaBar( selectedtext );
				QString str = codes[index];
				int st;
				st = str.indexOf("(");
				str = str.right( str.length()-st-1 );
				st = str.indexOf(")");
				str = str.left( st );

				QList<double> paraResult;
				for (int i = 0; i < paraName[index].size(); i ++)
				{
					if ( paraName[index][i] == selectedtext )
					{
						paraResult = _results[index][i].data;
						break;
					}
				}
				if (paraResult.size() > 0)
				{
					//emit addParameterFromMat( selectedtext, str, paraResult );
				}
			}
		
	}
	*/
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


void TextBubble2::deleteSelectedItems(QGraphicsScene *scene)
{
	if (!scene)
		return;
	
	deleteSelectedItemKids(scene);
	
	logRemove();	
	
	for (int i = 0; i < _codeBubbles.size(); i ++)
	{
		ItemBase *itemBase = dynamic_cast<ItemBase *>(_codeBubbles[i]);
		delete [] _codeBubbles[i]->_text;
		itemBase->hide();
		itemBase->properRemove();		
	}
	if(_codeParent!=NULL)
	{
		for (int i = 0; i < _codeParent->_codeBubbles.size(); i ++)
		{
			if(_codeParent->_codeBubbles[i]==this)
            {	
				_codeParent->_codeBubbles.removeOne(this);
		        _codeParent->_conPoint.removeOne(_codeParent->_conPoint[i]);
				break;
			}
		}
	}
	else if(_pathBubbleParent!=NULL)
	{
		for (int i = 0; i < _pathBubbleParent->_codeBubbles.size(); i ++)
		{
			if(_pathBubbleParent->_codeBubbles[i]==this)
            {	
				_pathBubbleParent->_codeBubbles.removeOne(this);
		        _pathBubbleParent->_conPoint.removeOne(_pathBubbleParent->_conPoint[i]);
				break;
			}
		}
	}
	_codeParent=NULL;
	_pathBubbleParent=NULL;
	_scene->_codeBubbles[m_codeBubbleID]=NULL;
	ItemBase::deleteSelectedItems(scene);		
}

void TextBubble2::getAllSubTextBubble(TextBubble2* bubble, QList<ItemBase *> &list)
{//tex tbubble
	int size= bubble->_codeBubbles.size();		
	for( int i = 0; i <size; i++ )
	{	
		list.append( (ItemBase*) bubble->_codeBubbles[i] );			   
	    getAllSubTextBubble(bubble->_codeBubbles[i] , list);
	}	
}

void TextBubble2::deleteSelectedItemsbyParent(QGraphicsScene *scene)
{
	if (!scene)
		return;

	for (int i = 0; i < _codeBubbles.size(); i ++)
	{		
		_codeBubbles[i]->deleteSelectedItemsbyParent(scene);
	}
	
	if(_codeParent!=NULL)
	{
		for (int i = 0; i < _codeParent->_codeBubbles.size(); i ++)
		{
			if(_codeParent->_codeBubbles[i]==this)
            {	
				_codeParent->_codeBubbles.removeOne(this);				
		        _codeParent->_conPoint.removeOne(_codeParent->_conPoint[i]);				
				break;
			}
		}
	}	

	if(_pathBubbleParent!=NULL)
	{
		for (int i = 0; i < _pathBubbleParent->_codeBubbles.size(); i ++)
		{
			if(_pathBubbleParent->_codeBubbles[i]==this)
            {	
				_pathBubbleParent->_codeBubbles.removeOne(this);				
		        _pathBubbleParent->_conPoint.removeOne(_pathBubbleParent->_conPoint[i]);
				break;
			}
		}
	}	
    //m_pathwayID=-1;
	_codeBubbles.clear();
	_conPoint.clear();

	ItemBase *itemBase = dynamic_cast<ItemBase *>(this);
	itemBase->setVisible(false);
	itemBase->hide();
	itemBase->properRemove();	
	_pathBubbleParent=NULL;
	_codeParent=NULL;

	//delete itemBase;
	deleteSelectedItems(_scene);		
	
}



void TextBubble2::deleteSelectedItemKids(QGraphicsScene *scene)
{
	/*QList<TextBubble2 *> _codeBubbles=curBubble->_codeBubbles;
	TextBubble2 *_codeParent = curBubble->_codeParent;
	PathBubble1 *_pathBubbleParent=curBubble->_pathBubbleParent;
	QList<QPointF> _conPoint = curBubble->_conPoint;
	QList<int> _bubbleLine = curBubble->_bubbleLine;
	*/
	int size=_codeBubbles.size();
	while(size>0)
	{	
		if(_codeBubbles.last())
			_codeBubbles.last()->deleteSelectedItemsbyParent(_scene);
		//_codeBubbles[i]->deleteSelectedItems(scene);
		size--;
	}	

	_codeBubbles.clear();
	_conPoint.clear();
	_bubbleLine.clear();

	/*
	for (int i = 0; i < _codeBubbles.size(); i ++)
	{		
		deleteSelectedItemKids(_codeBubbles[i]);		
		ItemBase *itemBase = dynamic_cast<ItemBase *>(_codeBubbles[i]);
		delete [] _codeBubbles[i]->_text;
		itemBase->hide();
		itemBase->properRemove();	
		//delete itemBase;
	}
	if(_codeParent!=NULL)
	{
		for (int i = 0; i < _codeParent->_codeBubbles.size(); i ++)
		{
			if(_codeParent->_codeBubbles[i]==curBubble)
            {	
				_codeParent->_codeBubbles.removeOne(curBubble);
		        _codeParent->_conPoint.removeOne(_codeParent->_conPoint[i]);
				if(!_codeParent->_bubbleLine.empty())
				  _codeParent->_bubbleLine.removeOne(_codeParent->_bubbleLine[i]);
				break;
			}
		}
	}
	else if(_pathBubbleParent!=NULL)
	{
		for (int i = 0; i < _pathBubbleParent->_codeBubbles.size(); i ++)
		{
			if(_pathBubbleParent->_codeBubbles[i]==curBubble)
            {	
				_pathBubbleParent->_codeBubbles.removeOne(curBubble);
				int pid=_pathBubbleParent->m_pathwayID;
				int size=_pathBubbleParent->_conPoint.size();
		        _pathBubbleParent->_conPoint.removeOne(_pathBubbleParent->_conPoint[i]);
				//_codeParent->_bubbleLine.removeOne(_codeParent->_bubbleLine[i]);
				break;
			}
		}
	}*/

}

void TextBubble2::drawConnections(QPainter *painter)
{
	QPointF constart, conend, tmpmid;
	int width;
	QPen pen;
	myColor3 color;

	pen.setWidth(2);
	painter->save();

	for (int i = 0; i < _codeBubbles.size(); i ++)
	{
		color = this->_codeBubbles[i]->getColor();
		//pen.setColor( QColor(color.a, color.b, color.c, 200) );
		pen.setColor( QColor(0, 0, 0, 200) );
		painter->setPen( pen );

		constart = this->_conPoint[i];
		constart.setX( this->Width()/2 );
		constart.setY( constart.y()*this->Height() - this->Height()/2 );
		//constart.setY(100);

		conend = _codeBubbles[i]->pos();
		conend.setX( conend.x() - _codeBubbles[i]->Width()/2);
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
		if(!_bubbleLine.empty())
		   highlightLineClicked(painter, _bubbleLine[i], QColor( _codeBubbles[i]->_colorBoarder.a, _codeBubbles[i]->_colorBoarder.b, _codeBubbles[i]->_colorBoarder.c, 200 ) );
	}

	painter->restore();
}

/*void TextBubble2::removeCodeCon( TextBubble2 *codeBubble )
{
	if (this == NULL)	return;

	for (int i = 0; i < _codeBubbles.size(); i ++)
	{
		if (_codeBubbles[i]->_curFileName == codeBubble->_curFileName)
		{
			_codeBubbles.removeAt(i);
			_conPoint.removeAt(i);
			return ;
		}
	}
}

void TextBubble2::removeAllCodeSon()
{
	TextBubble2 *tmp;

	for (int i = 0; i < _codeBubbles.size(); )
	{
		tmp = _codeBubbles[i];
		
		_codeBubbles.removeAt( i );
		_conPoint.removeAt( i );

		tmp->properRemove();
		delete tmp;
	}
}
*/
bool TextBubble2::hasOpenMatOrNot( TextBubble2 *para, QString name )
{
	if (para->_curFileName == name)
	{
		return true;
	}
	for (int i = 0; i < para->_codeBubbles.size(); i ++)
	{
		if ( hasOpenMatOrNot(para->_codeBubbles[i], name) )
		{
			return true;
		}
	}
	return false;
}

void TextBubble2::updatePath()
{
	QString str = this->_text->toPlainText();
	int sindex, eindex;
	sindex = str.lastIndexOf("cd('");
	eindex = str.lastIndexOf("');");

	_curFilePath = str;
	_curFilePath.remove(eindex, _curFilePath.length()-eindex);
	_curFilePath.remove(0, sindex+4);
	_curFilePath += '\\';

	findFunction();
}

void TextBubble2::updatePos()
{
	if ( this->_codeParent == NULL )
		return;
}

void TextBubble2::findFunction()
{
	QString str = this->_text->toPlainText();
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
	}
}

/*void TextBubble2::setPlotPara()
{
	TextBubble2* tmpCodeBubble;
	
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
}*/

TextBubble2* TextBubble2::findCodeBubblebyName( QString fileName )
{
	if (this->_curFileName == fileName)
	{
		return this;
	}

	TextBubble2* mat;
	for (int i = 0; i < _codeBubbles.size(); i ++)
	{
		mat = _codeBubbles[i]->findCodeBubblebyName( fileName );
		if (mat)	return mat;
	}

	return NULL;
} 

void TextBubble2::drawBubbleWithoutHalo( QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
	QPen pen;
	int lineWidth = BOARDERSIZE;

	painter->save();
	painter->setRenderHint(QPainter::Antialiasing, true);

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

void TextBubble2::drawBoarder( QPainter* painter )
{
// 	painter->drawLine( QPointF(-Width()/2-BOARDERSIZE/2, -Height()/2-BOARDERSIZE/2), QPointF(Width()/2+BOARDERSIZE/2, -Height()/2-BOARDERSIZE/2) );
// 	painter->drawLine( QPointF(-Width()/2-BOARDERSIZE/2, Height()/2+BOARDERSIZE/2), QPointF(Width()/2+BOARDERSIZE/2, Height()/2+BOARDERSIZE/2) );
// 	painter->drawLine( QPointF(-Width()/2-BOARDERSIZE/2, -Height()/2 ), QPointF(-Width()/2-BOARDERSIZE/2, Height()/2 ) );
// 	painter->drawLine( QPointF(Width()/2+BOARDERSIZE/2, -Height()/2 ), QPointF(Width()/2+BOARDERSIZE/2, Height()/2 ) );

	painter->drawRoundedRect( this->realRect(), 20, 20 );
}



void TextBubble2::copyAttribute( TextBubble2 *another )
{
	QString filename, tmpname,tnum, codetext, newcodetext;
	int num;

	filename = another->_curFileName;
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

	this->saveFile( _curFileName );

	this->_codeParent = another->_codeParent;
	this->_codeBubbles = another->_codeBubbles;
	this->_conPoint = another->_conPoint;

	TextBubble2* _para = this->_codeParent;
	if (_para)
	{
		_para->_codeBubbles.append( this );
		_para->_conPoint.append( QPointF(_para->Width()/2, 0) );
	}
}

/*void TextBubble2::removeDataVis()
{
	if (this->_dataVis)
	{
		this->_dataVis->properRemove();
		delete this->_dataVis;
	}
}
*/
int TextBubble2::codeIndexofParameter( QString str )
{
	int i, index;

	for (i = 0; i < paraName.size(); i ++)
	{
		index = paraName[i].indexOf( str );
		if (index > -1)	return i;
	}
	return -1;
}

void TextBubble2::saveAsXmlNode( QDomDocument &doc, QDomElement &node )
{
	QDomText text;
	QString str, tmpStr;

	QDomElement type = doc.createElement( tr("Type") );
	text = doc.createTextNode( tr("TextBubble2") );
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

	if (_codeParent)
	{
		QDomElement parentBubble = doc.createElement( tr("ParentBubble") );
		QDomElement pareFile = doc.createElement( tr("FileName") );
		text = doc.createTextNode( _codeParent->getCurrentFilename() );
		pareFile.appendChild( text );
		parentBubble.appendChild( pareFile );
		QDomElement parePos = doc.createElement( tr("Position") );
		str = "(";
		tmpStr.setNum( _codeParent->pos().x() );
		str += tmpStr;
		str += ", ";
		tmpStr.setNum( _codeParent->pos().y() );
		str += tmpStr;
		str += ")";
		text = doc.createTextNode( str );
		parePos.appendChild( text );
		parentBubble.appendChild( parePos );
		node.appendChild( parentBubble );
	}
	if (_pathBubbleParent)
	{
		QDomElement parentBubble = doc.createElement( tr("ParentBubble") );
		QDomElement pareFile = doc.createElement( tr("FileName") );
		text = doc.createTextNode(_pathBubbleParent->getCurrentFilename() );
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
	for (int i = 0; i < _codeBubbles.size(); i ++)
	{
		QDomElement descent = doc.createElement( tr("Descendant") );
		
		QDomAttr descentID = doc.createAttribute( tr("descID") );
		str.setNum( i+1 );
		descentID.setValue( str );
		descent.setAttributeNode( descentID );

		QDomElement descentFile = doc.createElement( tr("FileName") );
		text = doc.createTextNode( _codeBubbles[i]->getCurrentFilename() );
		descentFile.appendChild( text );
		descent.appendChild( descentFile );

		QDomElement descentPos = doc.createElement( tr("Position") );
		str = "(";
		tmpStr.setNum( _codeBubbles[i]->pos().x() );
		str += tmpStr;
		str += ", ";
		tmpStr.setNum( _codeBubbles[i]->pos().y() );
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

void TextBubble2::recoverFromXmlNode( QDomElement node )
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
		this->loadFile( str );
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

void TextBubble2::rebuildBubbleConnect( QDomElement node )
{
	_codeParent = NULL;
	_codeBubbles.clear();

	QList<ItemBase*> itemList;
	ItemBase* item;
	TextBubble2* tmp;
	PathBubble1* ptmp;
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
			if ( item->getType() == TEXTBUBBLE2 )
			{
				tmp = dynamic_cast<TextBubble2*>(item);
				if (tmp->getCurrentFilename() == name 
					&& fabs(tmp->pos().x() - x) < 3
					&& fabs(tmp->pos().y() - y) < 3 )
				{
					this->_codeParent = tmp;
					break;
				}
			}
			else if ( item->getType() == PATHBUBBLE1 )
			{
				ptmp = dynamic_cast<PathBubble1*>(item);
				if (//ptmp->getCurrentFilename() == name && 
					fabs(ptmp->pos().x() - x) < 3
					&& fabs(ptmp->pos().y() - y) < 3 )
				{
					this->_pathBubbleParent = ptmp;
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
				if ( item->getType() == TEXTBUBBLE2 )
				{
					tmp = dynamic_cast<TextBubble2*>(item);
					if (tmp->getCurrentFilename() == name 
						&& fabs(tmp->pos().x() - x) < 5
						&& fabs(tmp->pos().y() - y) < 5 )
					{
						this->_codeBubbles.append(tmp);
						break;
					}
				}
			}
		}
	}
}

void TextBubble2::resizeItem( int size1, int size2 )
{
	ItemBase::resizeItem( size1, size2 );
	this->_myControl->resize( size1, size2 );
	_text->setMask( roundRect( QRect(0, 0, m_size_width, m_size_height), 20) );
}