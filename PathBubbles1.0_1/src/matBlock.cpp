#include "matBlock.h"
#include "openglscene.h"
#include "matrix.h"

MatBlock::MatBlock(int x, int y, OpenGLScene* scene ) : ItemBase( MATSIZE, x, y)
{
	this->_REMOVECONTROL = true;
	this->_scene = scene;
	this->_TYPE = MATLAB;
	this->bIndex = _scene->getBubbleIndex();
	this->_thread = NULL;
	initUI();
	this->_myControl->setPos( x - MATSIZE/2, y - MATSIZE/2 );

	_LABEL = false;
	_codeParent = NULL;

	_dataVis = NULL;

	return;
}

MatBlock::~MatBlock()
{
	delete this->_check;
}

void MatBlock::logOpen()
{
	ItemBase::logOpen();
	
	emit logInfor("\n");
}

void MatBlock::logFindDeclaration( QString str )
{
	QString logtext;
	
	logtext.append( headofLogMsg() );
	logtext.append( "Find the declaration of the function " );
	logtext.append( str );
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void MatBlock::logAddtoParaBar( QString str )
{
	QString logtext;
	
	logtext.append( headofLogMsg() );
	logtext.append( "Add " );
	logtext.append( str );
	logtext.append( " to the Parameter Bar");
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void MatBlock::logRunCode()
{
	QString logtext;

	logtext.append( headofLogMsg() );
	logtext.append( "Run the matlab code" );
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void MatBlock::logOpenFile( QString str )
{
	QString logtext;

	logtext.append( headofLogMsg() );
	logtext.append( "Open the file " );
	logtext.append( str );
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void MatBlock::logSaveFile( QString str )
{
	QString logtext;

	logtext.append( headofLogMsg() );
	logtext.append( "Save the content in the file " );
	logtext.append( str );
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void MatBlock::logNewFile()
{
	QString logtext;

	logtext.append( headofLogMsg() );
	logtext.append( "Open a new file" );
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void MatBlock::logShowCheck()
{
	QString logtext;

	logtext.append( headofLogMsg() );
	logtext.append( "Show the bubble for result checking" );
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void MatBlock::logHideCheck()
{
	QString logtext;

	logtext.append( headofLogMsg() );
	logtext.append( "Hide the bubble for result checking" );
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void MatBlock::logCopy()
{
	QString logtext;

	logtext.append( headofLogMsg() );
	logtext.append( "Copy the matlab bubble" );
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void MatBlock::initUI()
{
	this->_check = new VariableCheck( this->pos().x() + MATSIZE, this->pos().y(), this->_scene );
	this->_check->myHide();

	this->_scene->addItem( this->_check );
	connect( this->_check, SIGNAL(query(QString)), this, SLOT(checkByName(QString)) );

	//			init UI first
	this->_text = new QTextEdit( );
	_text->setStyleSheet( "background-color: rgb(240, 250, 250)");
	_text->setMask( this->roundRect( QRect(0, 0, m_size_width, m_size_height), 20) );
	this->_hightlight = new Highlighter( this->_text->document());
	connect( this->_text, SIGNAL(textChanged()), this, SLOT(updatePath()) );
	connect( this->_text, SIGNAL( ), this, updatePos() );

	this->_myControl = this->_scene->addWidget( _text );

	return;
}

void MatBlock::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
	if( onBoarder( event->pos() ) )
	{
		ItemBase::mousePressEvent( event );
	}
	else
	{
		event->setAccepted( false );
	}
	return;
}

void MatBlock::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	if( onBoarder( event->pos() ) )
	{
		ItemBase::mouseReleaseEvent( event );
	}
	else
	{
		event->setAccepted( false );
	}
	return;
}

void MatBlock::mouseMoveEvent( QGraphicsSceneMouseEvent *event )
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

void MatBlock::matItemPosMoved( QPointF offset )
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

void MatBlock::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	painter->save();

// 	int textheight = this->_text->document()->size().height()+2;
// 	if ( textheight > 260 )
// 	{
// 		textheight = 260;
// 	}
// 
// 	this->resizeItem( this->Width(), textheight );

	updateLabel();
	if( this->_check && this->_check->isVisible() )
	{
		this->_check->setPos( this->pos() - QPointF( this->Width()/2+this->_check->Width()/2, 0 ) );
		this->_check->resizeItem( this->_check->Width(), this->Height() );
		this->_check->setColor( this->getColor() );
	}
	if (this->_dataVis)
	{
		this->_dataVis->mySetPos( this->pos() + QPointF( this->Width()/2+this->_dataVis->Width()/2+BOARDERSIZE, this->_dataVis->Height()/2-this->Height()/2 ));
		this->_dataVis->setColor( this->getColor() );
		this->_dataVis->resizeItem( this->_dataVis->Width(), this->Height() );
	}
	this->_myControl->setPos( this->pos().x() - this->Width()/2, this->pos().y() - this->Height()/2 );

	this->drawConnections( painter );	
	
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
	
	painter->restore();
}

void MatBlock::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
	ItemBase::hoverMoveEvent( event);
	return;
}

void MatBlock::setVisable()
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

void MatBlock::finishMatlab()
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

void MatBlock::runMatlab()
{
	this->_thread = new MatlabEngThread( this->_text->toPlainText() );
	this->_thread->setParaName( paraName );
	this->_thread->start();
	connect( this->_thread, SIGNAL(execEnd()), this, SLOT(finishMatlab()) );

	logRunCode();

	return;
}

void MatBlock::checkByName( QString name )
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

void MatBlock::openMatlabFile()
{
	QString fileName = QFileDialog::getOpenFileName();
	if (!fileName.isEmpty())
		loadFile(fileName);

	int index = fileName.lastIndexOf( "/" );
	fileName.remove( 0, index+1 );

	logOpenFile( fileName );
}

void MatBlock::saveMatlabFile()
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

void MatBlock::loadFile(const QString &fileName)
{
	if (maybeSave())
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

void MatBlock::saveFile(const QString &fileName)
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

void MatBlock::setCurrentFile(const QString &fileName)
{
	_curFileName = fileName;
	_curFilePath = _curFileName;
	int tindex = _curFilePath.lastIndexOf("\\");
	_curFilePath.remove(tindex+1, _curFilePath.length());
}

void MatBlock::newMatlabFile()
{
	if (maybeSave())
	{
		_curFileName.clear();
		_curFilePath.clear();
		this->_text->setText( tr("") );

		logNewFile();
	}
}

void MatBlock::saveasMatlabFile()
{
	QString fileName = QFileDialog::getSaveFileName();
	if (!fileName.isEmpty())
	{
		saveFile(fileName);
	}
}

bool MatBlock::maybeSave()
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

void MatBlock::updateLabel()
{
	if (_LABEL == false)
	{
		_label = new ItemLabel( this, _scene, m_size_width, m_size_height );
		_label->setLabelColor(_colorBoarder);
		this->_scene->addItem(_label);
		_LABEL = true;

		emit updateLabelState( this );

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

void MatBlock::contextMenuEvent(QGraphicsSceneContextMenuEvent *event )
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
			logRemove();

			deleteSelectedItems( _scene );
		}
		else if ( selectedAction == ungroupAction )
		{
			emit unGroup(this);
		}
		else if ( selectedAction == copyAction )
		{
			emit setMatFileBubble( this );
			
			logCopy();
		}
	}
	else if ( _text->textCursor().hasSelection() )
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

			MatBlock *para;
			para = this;
			while ( para->getCodeParent() )
			{
				para = para->getCodeParent();
			}
			if ( ! hasOpenMatOrNot( para, selectedtext ) )
			{
				MatBlock *matcode = dynamic_cast<MatBlock*>(this->_scene->getOpenManager()->openMatlabByPath( MATSIZE, px, py, selectedtext));
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
					emit addParameterFromMat( selectedtext, str, paraResult );
				}
			}
		}
	}
	else
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
	}
}

void MatBlock::drawConnections(QPainter *painter)
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
		pen.setColor( QColor(color.a, color.b, color.c, 100) );
		painter->setPen( pen );

		constart = this->_conPoint[i];
		constart.setX( this->Width()/2 );

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
	}

	painter->restore();
}

void MatBlock::removeCodeCon( MatBlock *codeBubble )
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

void MatBlock::removeAllCodeSon()
{
	MatBlock *tmp;

	for (int i = 0; i < _codeBubbles.size(); )
	{
		tmp = _codeBubbles[i];
		
		_codeBubbles.removeAt( i );
		_conPoint.removeAt( i );

		tmp->properRemove();
		delete tmp;
	}
}

bool MatBlock::hasOpenMatOrNot( MatBlock *para, QString name )
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

void MatBlock::updatePath()
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

void MatBlock::updatePos()
{
	if ( this->_codeParent == NULL )
		return;
}

void MatBlock::findFunction()
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

void MatBlock::setPlotPara()
{
	MatBlock* tmpCodeBubble;
	
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

MatBlock* MatBlock::findCodeBubblebyName( QString fileName )
{
	if (this->_curFileName == fileName)
	{
		return this;
	}

	MatBlock* mat;
	for (int i = 0; i < _codeBubbles.size(); i ++)
	{
		mat = _codeBubbles[i]->findCodeBubblebyName( fileName );
		if (mat)	return mat;
	}

	return NULL;
} 

void MatBlock::drawBubbleWithoutHalo( QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
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
	painter->setRenderHint(QPainter::Antialiasing, false);
	ItemBase::ControlAutoHide();

	painter->restore();
}

void MatBlock::drawBoarder( QPainter* painter )
{
// 	painter->drawLine( QPointF(-Width()/2-BOARDERSIZE/2, -Height()/2-BOARDERSIZE/2), QPointF(Width()/2+BOARDERSIZE/2, -Height()/2-BOARDERSIZE/2) );
// 	painter->drawLine( QPointF(-Width()/2-BOARDERSIZE/2, Height()/2+BOARDERSIZE/2), QPointF(Width()/2+BOARDERSIZE/2, Height()/2+BOARDERSIZE/2) );
// 	painter->drawLine( QPointF(-Width()/2-BOARDERSIZE/2, -Height()/2 ), QPointF(-Width()/2-BOARDERSIZE/2, Height()/2 ) );
// 	painter->drawLine( QPointF(Width()/2+BOARDERSIZE/2, -Height()/2 ), QPointF(Width()/2+BOARDERSIZE/2, Height()/2 ) );

	painter->drawRoundedRect( this->realRect(), 20, 20 );
}


bool MatBlock::onBoarder( QPointF pos )
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

void MatBlock::copyAttribute( MatBlock *another )
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

	MatBlock* _para = this->_codeParent;
	if (_para)
	{
		_para->_codeBubbles.append( this );
		_para->_conPoint.append( QPointF(_para->Width()/2, 0) );
	}
}

void MatBlock::removeDataVis()
{
	if (this->_dataVis)
	{
		this->_dataVis->properRemove();
		delete this->_dataVis;
	}
}

int MatBlock::codeIndexofParameter( QString str )
{
	int i, index;

	for (i = 0; i < paraName.size(); i ++)
	{
		index = paraName[i].indexOf( str );
		if (index > -1)	return i;
	}
	return -1;
}

void MatBlock::saveAsXmlNode( QDomDocument &doc, QDomElement &node )
{
	QDomText text;
	QString str, tmpStr;

	QDomElement type = doc.createElement( tr("Type") );
	text = doc.createTextNode( tr("Matlab") );
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
		saveasMatlabFile();
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

void MatBlock::recoverFromXmlNode( QDomElement node )
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

void MatBlock::rebuildBubbleConnect( QDomElement node )
{
	_codeParent = NULL;
	_codeBubbles.clear();

	QList<ItemBase*> itemList;
	ItemBase* item;
	MatBlock* tmp;
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
			if ( item->getType() == MATLAB )
			{
				tmp = dynamic_cast<MatBlock*>(item);
				if (tmp->getCurrentFilename() == name 
					&& fabs(tmp->pos().x() - x) < 3
					&& fabs(tmp->pos().y() - y) < 3 )
				{
					this->_codeParent = tmp;
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
				if ( item->getType() == MATLAB )
				{
					tmp = dynamic_cast<MatBlock*>(item);
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

void MatBlock::resizeItem( int size1, int size2 )
{
	ItemBase::resizeItem( size1, size2 );
	this->_myControl->resize( size1, size2 );
	_text->setMask( roundRect( QRect(0, 0, m_size_width, m_size_height), 20) );
}