#include <QTextEdit>
#include "openglscene.h"
#include "CalloutNote.h"


CalloutNote::CalloutNote( int pid, int type, int id, int size, int x, int y, OpenGLScene* Scene ):ItemBase(size, x, y)
{	
	fixedWidth=size, fixedHeight=fixedWidth/1.618;
	_pid=pid, _type=type, _id=id;
	this->_TYPE = CALLOUT;
	_size=size;	
	_scene = Scene;
	this->bIndex = _scene->getBubbleIndex();	
	//this->_myControl->setPos( x - size/2, y - size/2 );
	this->_colorBoarder = TDHelper::getBoarderColorByType(_TYPE);
	this->_colorInner = TDHelper::getInnerColorByType(_TYPE);	
	_REMOVECONTROL = true;	
	initUI( Scene );
	m_noteID=_scene->_nodeNote.size();

	if(_scene->_colorID==-1)
	{
		_cid=getColorByUserName(_scene->userName);		
		_fontColor=TDHelper::getHtmlColorByUserName(_cid);	
		_scene->_colorID=_cid;
	}
	else
	{
	     _cid=_scene->_colorID;
	     _fontColor = TDHelper::getHtmlColorByUserName(_cid);	
	}	
	//m_noteID
}

void CalloutNote::logOpen()
{
	ItemBase::logOpen();
	emit logInfor("\n");
}

/*
CalloutNote::~CalloutNote()
{
	int pid=this->_pid;
	if(!_scene)
	{
		if(_scene->_pathBubbles[pid])
	    {
			QString pathName = _scene-> pwdParser->savePathNotePreparation(_scene->_pathBubbles[pid]->_pathName, _scene->userName);	
			this->record(pathName);		
		}	
	}
}
*/

void CalloutNote::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
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
	ItemBase * item = _scene->findBubbleUnderNote(event->scenePos());
	_scene->dropDownMenu0(event, item, node);
	return;	
}



void CalloutNote::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	/*ItemBase::mouseDoubleClickEvent(event);	
	AFTERUNGROUP = false;	
	UNGROUP = false;

	_scene->clearSelected(this);
	_scene->clearExpressed(this);	
	_scene->clearColored(this);
	_scene->clearUndoRedoRecord(this);	
	_scene->hoveredItem.clear();

	_scene->removeToolsinGroup(this, true);*/
	return;
}

bool CalloutNote::compare(QString &preText, QString &curText)
{
	int index=-1;
	while(preText.indexOf("\n")==0)
	{	
		index= preText.indexOf("\n");
		preText = preText.mid(index+1,preText.size()-index);		
	}

	if(preText.indexOf(" ")==0)
	{	
		index= preText.indexOf(" ");
		preText = preText.mid(index+1,preText.size()-index);		
	}
	
	while(preText.lastIndexOf("\n")==preText.size()-1)
	{	
		index= preText.lastIndexOf("\n");				
		preText = preText.mid(0, index);		
		if(preText.size()==0)
			break;
	}

	curText = curText.remove(lastInputTime);

	while(curText.indexOf("\n")==0)
	{	
		index= curText.indexOf("\n");
		curText = curText.mid(index+1,curText.size()-index);		
	}

	if(curText.indexOf(" ")==0)
	{	
		index= curText.indexOf(" ");
		curText = curText.mid(index+1,curText.size()-index);		
	}

	while(curText.lastIndexOf("\n")==curText.size()-1)
	{	
		index= curText.lastIndexOf("\n");
		curText = curText.mid(0, index);		
		if(curText.size()==0)
			break;
	}
	int size1=preText.size();
	int size2=curText.size();
	int index1= preText.lastIndexOf("\n");
	int index2= curText.lastIndexOf("\n");
	int index3= preText.indexOf("\n");
	int index4= curText.indexOf("\n");
	int index6= preText.lastIndexOf(" ");
	int index7= curText.lastIndexOf(" ");
	
	for(int i=0;i<preText.size()||i<curText.size();i++)
	{
	    QString p=preText[i];
		QString c=curText[i];		
	}
	
	if(preText!=curText)
	{
		int index = curText.indexOf(preText);
		if(index>=0)
		    curText=curText.mid(0, index) + curText.mid(index+preText.size(),curText.size()-index-preText.size());

		return false;
	}
	else 
		return true;
}

void CalloutNote::record(QString pathName, bool ifDeleted)
{
	//get current date and time
	QDateTime dateTime = QDateTime::currentDateTime();
	QString dateTimeString = dateTime.toString("dd/mm/yy hh:mm");
	QStringList sNames = dateTimeString.split(" ");
    //QString timeNumbers = sNames[1];

	QString text;
	storeText(_pid,_type,_id);
	bool flag=true;
	bool write=false;
	int size=nodeText.size();
	//for(int i=0; i<nodeText.size(); i++)
	{
	    //text = nodeText[i]->getPlainText();	   
		text = m_note->toPlainText();
		if((text.size()>0 && !compare(_preText,text)) || ifDeleted) 
		{
			if(!pathName.contains(".note"))
				pathName +=".note";
	
			QFile infile(pathName);
			vector<QString> out1;
			if(infile.open(QIODevice::ReadOnly))
			{
			    QTextStream in(&infile);
                 
				 while(!in.atEnd())
			     {
				    QString  line = in.readLine();
				    out1.push_back(line);					
				 }
			}
			infile.close();

			QFile outfile( pathName );
			if(!outfile.open(QIODevice::WriteOnly | QIODevice::Truncate))
			return;

			//return;
			QTextStream out(&outfile); 
			
			
			for(int j=0; j<out1.size(); j++)
			{  
				QStringList sname=out1[j].split(' ');
				vector<int> item;
				for(int i=0; i<5 && i<sname.size(); i++)
				{
					QString s=sname[i];				 						
					int id=s.toInt();						
					item.push_back(id);
				}	
				if(item.size()>4 && item[2]==_type && item[3]==_id)
				{
					flag=false;	
					if(!ifDeleted)
					{
						if(!write) //if not written in this time, write the text back
						{
							 //out << "<-:#:-> " << _pid << " " <<  _type << " " << _id << " " << _cid << " " << _scene->userName << " " << dateTimeString << " " << text << "\n"; 
							 out <<  "<-:#:-> " << _pid << " " <<  _type << " " << _id << " " << _cid << "\n";  
							 out << m_note->toHtml();
							 out << "\n"; 
							 write =true;
							 
						}
						//out << out1[j];// << "\n";  
					    //if(out1[j][0]!='\n')
						//out << "\n";  										    			
					}
					int J=j;
					for(int k=j+1; k<out1.size(); k++)
					{
						if(out1[k][0]!='<' || out1[k][1]!= '-' || out1[k][2]!= ':'  || out1[k][3]!= '#'  || out1[k][4]!=  ':'  || out1[k][5]!= '-'  || out1[k][6]!= '>')
						{
						   J=k;     
						}
						else
						   break;
					}
					j=J;
				}
				else 
				{
					out << out1[j];// << "\n";  
					if(out1[j][0]!='\n')
						out << "\n";  
				}
			}
			if(flag)
			{
				//out <<  "<-:#:-> " << _pid << " " <<  _type << " " << _id << " " << _cid << " " << _scene->userName << " " << dateTimeString << " " << text << "\n";  			
				out <<  "<-:#:-> " << _pid << " " <<  _type << " " << _id << " " << _cid << "\n";  			
				out << m_note->toHtml();
				out << "\n"; 
				return;
			}
			
			outfile.close();
			
			//break;
			
		}		
	}
}

int CalloutNote::getNodeID(vector< vector <int> > nodeIDs, int pid, int type, int id)
{
	vector<int> temp;
	temp.push_back(pid); temp.push_back(type); temp.push_back(id);
	for(int i=0; i<nodeIDs.size(); i++)
	{
	   if(nodeIDs[i]==temp)
		   return i;
	}
	return -1;
}

QString CalloutNote::getText(int pid, int type, int id)
{
	int ID=getNodeID(nodeIDs, pid, type, id);
	if(ID<0)
		return "";
	else return nodeText[ID];		
}


int CalloutNote::getColorByUserName(QString userName)
{
	int id=-1;
	QString pathName = _scene-> pwdParser-> savePathNotePreparation(_scene->_pathBubbles[_pid]->_pathName, _scene->userName);	
	//read *.note name
	if(!pathName.contains(".note"))
		pathName +=".note";

	QFile infile(pathName);
	set<int> cid;
	if(infile.open(QIODevice::ReadOnly))
	{
		QTextStream in(&infile);
	   	while(!in.atEnd())
		{
			QString  line = in.readLine();	  
			QStringList block = line.split("<-:#:->");	
			QString text;		
			if(block.size()>1)
			{	
				for(int j=1; j<block.size(); j++)
				{
					vector<int> item;
					QString test = block[j];
					QStringList sname=block[j].split(' ');			
					for(int i=0; i<6 && i<sname.size(); i++)
					{
						QString s=sname[i];				 						
						int id=s.toInt();						
						item.push_back(id);
					}		
					text="";					
					if(sname.size()>5)
					{
						if(sname[5]==_scene->userName)
						{
							id=item[4];
							break;
						}						
						cid.insert(item[4]);
					}
				}			
			}	
			else
			{
				
			}
		}
		if(id<0)
		{
			int i=0;
			for(set<int>::iterator it=cid.begin(); it!=cid.end(); it++)
			{
				if(*it!=i)
				{
					id=i;
					break;
				}
			    i++;
			}			
		}
	}
	if(id<0)
		id=rand();
	infile.close();	
	//other wise	
	return(id);
}

void CalloutNote::storeText(int pid, int type, int id, bool plain)
{
	int ID = getNodeID(nodeIDs, pid, type, id);
	vector<int> temp;
	temp.push_back(pid), temp.push_back(type), temp.push_back(id);
	if(ID<0)
	{
		nodeIDs.push_back(temp);
		QString sName = m_note->toPlainText();
		//QString pName = m_note->toPlainText(); //m_note->toPlainText();
		int index= sName.indexOf("\n");
		sName = sName.mid(index, sName.size());		
		if(compare(_preText,sName))
		{
		   sName ="";
		}
		nodeText.push_back(sName);
	}
	else
	{
		QString sName = plain? m_note->toPlainText(): m_note->toHtml(); //m_note->toPlainText();
		QString pName = m_note->toPlainText(); //m_note->toPlainText();
		int index= sName.indexOf("\n");
		sName = sName.mid(index, sName.size());		
		while(sName.lastIndexOf("\n")==sName.size()-1)
		{	
		    index= sName.lastIndexOf("\n");
			sName = sName.mid(0, index);	
			if(sName.size()==0)
				break;
		}
		if(compare(_preText,pName))
		{
		   //sName ="";
		}
	    else 
		{
			nodeText.resize(ID+1);
			nodeText[ID]=sName;
		}
		int size1=pName.size(),size2=_preText.size();
		size1=size1;
	}	
}

void CalloutNote::drawNoteNodeConnector(QPainter *painter, bool visible)
{	
	float width;
	float height;
	QRectF itemPos;
	if(_scene->_pathBubbles[_pid]==NULL) //does not work
		return;
	PathBubble1* path=_scene->_pathBubbles[_pid];
	if(!path || path==NULL || !path->isVisible())
		return;

	QPointF dis1=this->sceneBoundingRect().center();
	QPointF dis2=path->sceneBoundingRect().center();

	if(fixedSize)
	{		
		width=graphReferenceSize/2*path->_scale; height=graphReferenceSize/2*path->_scale;
	}
	else
	{
		width=this->realRect().width()/2*path->_scale;
	    height=this->realRect().height()/2*path->_scale;
	}

	if(_type == 'L' && _id>=path->ANodeRect.size())
		return;

	switch(_type)
	{
		 case 'C': itemPos = path->complexRect[_id];  break;
		 case 'E': itemPos = path->physicalEntityRect[_id];  break;
		 case 'P': itemPos = path->proteinRect[_id];  break;				
		 case 'S': itemPos = path->smallMoleculeRect[_id];  break;
		 case 'D': itemPos = path->DnaRect[_id];  break;
		 case 'R': itemPos = path->reactionRect[_id];  break;
		 case 'L': itemPos = path->ANodeRect[_id];  break;
		 case 'M': itemPos = path->compartmentRect[_id];  break;
	}

	
	QPointF start,end;
	
	start=QPointF(0,0);
	end=itemPos.center();

	start=start+dis1;
	end=end+dis2;

	QRectF noteRect=this->sceneBoundingRect();	
	QRectF pathRect=path->sceneBoundingRect();
	
	float w,h;
	w=realRect().width(),  h=realRect().height();
	noteRect=QRectF(noteRect.center().x()-w/2, noteRect.center().y()-h/2, w, h );

	QPointF markPos(end.x()+itemPos.width()*0.5, end.y()-itemPos.height()*0.1);
	_deleteMark = QRectF(noteRect.x()+noteRect.width()*0.90, noteRect.y()+noteRect.height()*0.03, noteRect.width()*0.08, noteRect.width()*0.08);	
	_minimizeMark = QRectF(noteRect.x()+noteRect.width()*0.80, noteRect.y()+noteRect.height()*0.03, noteRect.width()*0.08, noteRect.width()*0.08);	

	

	//clip	    
	QColor c=QColor(_colorBoarder.a, _colorBoarder.b, _colorBoarder.c, 255);
	if(visible)
	{		
		
		if(!noteRect.contains(end))
		{
			drawArrow_5(painter, start, markPos-QPointF(itemPos.width()*0.04,0), QRect(noteRect.center().x()-w/2,noteRect.center().y()-h/2, w, h ), width, height, c );						
		}		
		drawCross(painter,_deleteMark, c);
		drawMinus(painter,_minimizeMark, c);
	}
	else 
	{
		//if within the path bubble
		if(pathRect.contains(start)|| pathRect.contains(end))
		   drawNoteMark(painter, start, end,  QRect(noteRect.center().x()-w/2,noteRect.center().y()-h/2, w, h ), markPos, markRect, width, height, c );						
	}		
}


void CalloutNote::updateNodeFirstTime(int pid, int type, int id, bool setVisible)
{
	//storeText(_pid,_type,_id);
	_pid=pid, _type=type, _id=id;

	int ID = getNodeID(nodeIDs, pid, type, id);
	vector<int> temp;
	temp.push_back(pid), temp.push_back(type), temp.push_back(id);
	if(ID<0)
	{
		nodeIDs.push_back(temp);
	}

	QDateTime dateTime = QDateTime::currentDateTime();
	QString dateTimeString = dateTime.toString("dd/mm/yy hh:mm");
	QStringList sNames = dateTimeString.split(" ");			

	//m_note->setText(_scene->userName + ": " + dateTimeString + "\n" + "\n" + getText(_pid, _type, _id));	
	//	m_note->insertHtml(_scene->userName + ": " + dateTimeString + "\n" + "\n" + getText(_pid, _type, _id));
	//m_note->insertHtml("<font color=\"DeepPink\">");
		
	QString line, hLine;	
	if(nodeText.size()>0 && nodeText[0].size()>0  )
	{
		line=nodeText[0];	
	    //line = getText(_pid, _type, _id);
		m_note->setHtml(line);
		line = m_note->toPlainText();
		_preText = line;

		hLine = m_note->toHtml();	

		//_preText = m_note->toHtml();	
		//m_note->setPlainText("");
	}
	else
	{
		_preText="";
		hLine ="";
		//_preText = m_note->toHtml();	
	}

	//if(setVisible)
	{	
		/*QString endHtml = "</font><br>";
		line = "<br><font color=\"Black\">" + line;
		line = line + endHtml; //+ "<br>"
		//line = line + line;
		line.replace("\n","<br>");	*/
		if(hLine=="")
		{
			QString endHtml = "</font><br>";
			line = "<br><font color=\"Black\">" + line;
			line = line + endHtml; //+ "<br>"
			//line = line + line;
			line.replace("\n","<br>");
		}
		else
		{
			m_note->setHtml("<br>"+hLine);
		}
		
		line = _scene->userName + ": " + dateTimeString;
		lastInputTime = line;
		line = line + "<br>";
		//line = "<font color=\"Lime\">" + line;
		//line = "<font color=\"" + "#008000\">" + line;
		line =  "\">" + line;
		line =  _fontColor + line;
		line = "<font color=\"" + line;//_fontColor
		m_note->insertHtml(line);
		
		//line = m_note->toPlainText();
		//_preText=line;  

		//_preText = m_note->toHtml();	
		
	}
	//_fontColor
		
    //position where you want it			
	resizeItem( fixedWidth, fixedHeight );//add to make resizing work
	_myControl->setVisible(setVisible);
    m_note->setVisible(setVisible);			
	this->setVisible(setVisible);

	if(setVisible)
		this->show();
	else 
		this->hide();
	m_note->setFocus();

	if(setVisible)
	{
		QTextCursor txtCursor = m_note->textCursor();
	   txtCursor.setPosition(_scene->userName.size() + 2 + 15);
        m_note->setTextCursor(txtCursor);
	}
}


void CalloutNote::updateNode(int pid, int type, int id, bool setVisible)
{
	/*storeText(_pid,_type,_id,false);
	_pid=pid, _type=type, _id=id;

	QDateTime dateTime = QDateTime::currentDateTime();
	QString dateTimeString = dateTime.toString("dd/mm/yy hh:mm");
	QStringList sNames = dateTimeString.split(" ");			

	QString line;
	line = getText(_pid, _type, _id);
	m_note->setHtml(line);
	
	QString endHtml = "</font>"; //<br>
	line = _scene->userName + ": " + dateTimeString;
	line =  "\">" + line;
	line =  _fontColor + line;
	line = "<font color=\"" + line;//_fontColor
	line = line + "<br>" + endHtml;
	m_note->insertHtml(line);

	line = m_note->toPlainText();
	_preText=line;  
	*/
	//line = m_note->toPlainText();

	/*QString endHtml = "</font><br>";
    QString line;
	line = getText(_pid, _type, _id);
    line.replace("\n","<br>");
	m_note->setHtml(line);

	line = _scene->userName + ": " + dateTimeString;
	line =  "\">" + line;
	line =  _fontColor + line;
	line = "<font color=\"" + line;//_fontColor
	line = line + "<br>" + endHtml;
	m_note->insertHtml(line);
	line = m_note->toPlainText();
     _preText=line;  
	//_preText = m_note->toHtml();	

    //line = m_note->toPlainText();
	//_fontColor
	*/

	QTextCursor txtCursor = m_note->textCursor();
    //position where you want it
	
	resizeItem( fixedWidth, fixedHeight );//add to make resizing work
	_myControl->setVisible(setVisible);
    m_note->setVisible(setVisible);			
	this->setVisible(setVisible);

	if(setVisible)
		this->show();
	else 
		this->hide();
	m_note->setFocus();

	int pos = m_note->find(" ");
    txtCursor.setPosition(_scene->userName.size() + 2 + 15);
    m_note->setTextCursor(txtCursor);
}

void CalloutNote::initUI( OpenGLScene* Scene )
{
	this->m_note = new QTextEdit();		
	this->m_note->setText(getText(_pid, _type, _id));
	m_note->setStyleSheet( "background-color: rgb(215, 200, 145)");			
	this->_myControl = Scene->addWidget( m_note );	
	resizeItem(_size, _size/1.618);	
	//m_note->setMask( this->roundRect( QRect(0, 0, this->Width(),this->Height()), 20)); //QRegion(0, 0, this->Width()/2, this->Height()/2, QRegion::Ellipse) 	
	m_note->setMask( QRect(0, 0, this->Width(),this->Height())); //QRegion(0, 0, this->Width()/2, this->Height()/2, QRegion::Ellipse) 	
	m_note->setFocus();
	myCursor = m_note->textCursor(); //critical to make cursor visible
	m_note->ensureCursorVisible();//useless here
	
	crossRect = QRectF(this->realRect().x()+this->realRect().width()*0.90, this->realRect().y()+this->realRect().height()*0.03, this->realRect().width()*0.08, this->realRect().width()*0.08);	
	minusRect = QRectF(this->realRect().x()+this->realRect().width()*0.80, this->realRect().y()+this->realRect().height()*0.03, this->realRect().width()*0.08, this->realRect().width()*0.08);	

    /* myCursor.insertText(tr("We can emphasize text by "));	
    myCursor.setPosition(QTextCursor::Right, QTextCursor::KeepAnchor);*/

	return;
}

void CalloutNote::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
	//bool v= isVisible();
	this->_myControl->resize( this->Width(), this->Height() );//add to make resizing work	
	this->_myControl->setPos( this->pos().x() - this->Width()/2, this->pos().y() - this->Height()/2 );		
	crossRect = QRectF(this->realRect().x()+this->realRect().width()*0.90, this->realRect().y()+this->realRect().height()*0.03, this->realRect().width()*0.08, this->realRect().width()*0.08);	
	minusRect = QRectF(this->realRect().x()+this->realRect().width()*0.80, this->realRect().y()+this->realRect().height()*0.03, this->realRect().width()*0.08, this->realRect().width()*0.08);	
     

}

bool CalloutNote::onBoarder( QPointF pos )
{
	int x = pos.x();
	int y = pos.y();
	int xhalf = this->Width()/2;
	int yhalf = this->Height()/2;
	int margin=3;
	if( (x > xhalf-margin) || (x < -xhalf+margin) )
	{
		return true;
	}
	if( (y > yhalf-margin) || (y < -yhalf+margin) )
	{
		return true;
	}
	return false;
}

void CalloutNote::mouseMoveEvent( QGraphicsSceneMouseEvent *event )
{
	
	if(m_isResizing)
	{
		resizeItem( this->Width(), this->Height() );
		ItemBase::mouseMoveEvent( event );
	}
	else 
	{
		QRect rect = m_note->verticalScrollBar()->rect();
		QPointF rectPos = QPointF(m_note->width()/2-rect.width(), -m_note->height()/2);
		rect = QRect(rect.x()+rectPos.x(),rect.y()+rectPos.y(),rect.width(), rect.height());
		QPointF lastPos = event->lastPos(), pos = event->pos();
		if( rect.contains(lastPos.x(), lastPos.y()) && rect.contains(pos.x(), pos.y()))
		{
			//m_note->verticalScrollBar()
			//m_note->verticalScrollBar(
			//document length = maximum() - minimum() + pageStep()	
			int v1 = m_note->verticalScrollBar()->maximum();
			int v2 = m_note->verticalScrollBar()->minimum();
			int pageStep =m_note->verticalScrollBar()->pageStep();
			int length = v1 - v2 + pageStep;
			if(pos.y()<lastPos.y())
		      m_note->verticalScrollBar()->setValue(m_note->verticalScrollBar()->value() - length/50);		
			else if(pos.y()>lastPos.y())
				m_note->verticalScrollBar()->setValue(m_note->verticalScrollBar()->value() + length/50);		
		}
		else 
		{
			ItemBase::mouseMoveEvent( event );
			m_note->setFocus();		
		}
	}
}

void CalloutNote::resizeItem( int size1, int size2 )
{
	ItemBase::resizeItem( size1, size2 );
	this->_myControl->resize( size1, size2 );
	//m_note->setMask( roundRect(QRect(0, 0, m_size_width, m_size_height), 20 ) );
	m_note->setMask( QRect(0, 0, m_size_width, m_size_height) );
	m_note->setFocus();
}

void CalloutNote::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
	ItemBase::hoverMoveEvent( event );
	event->setAccepted( true );
}

void CalloutNote::mousePressEvent( QGraphicsSceneMouseEvent *event )
{    
    //m_note->verticalScrollBar()->setEnabled(true);
	//m_note
	if(minusRect.contains(event->pos()))
	{
	    _myControl->setVisible(false);
		m_note->setVisible(false);
		this->hide();
		return;
	}
	else if(crossRect.contains(event->pos()))
	{
		QMessageBox msgBox;
		msgBox.move(event->scenePos().x(), event->scenePos().y());
		msgBox.setWindowTitle("Delete Note");
		msgBox.setText("Delete this note?");
	    msgBox.setStandardButtons(QMessageBox::Yes);
		msgBox.addButton (QMessageBox::No);		
		if(msgBox.exec() == QMessageBox::No)
				return;
	    _myControl->setVisible(false);
		m_note->setVisible(false);
		this->deleted=true;		
		this->hide();		
		return;
	}	
	QRect rect = m_note->verticalScrollBar()->rect();
	//float h=m_note->height();
	QPointF rectPos = QPointF(m_note->width()/2-rect.width(), m_note->height()/2-rect.width());	
	rect = QRect(rect.x()+rectPos.x(),rect.y()+rectPos.y(), rect.width(), rect.width());
	//scollbar bottom arrow 
	if( rect.contains(event->pos().x(), event->pos().y()) )
	{	
	    //  m_note->verticalScrollBar()->setValue(m_note->verticalScrollBar()->value() - length/100);		
		//	else if(pos.y()>lastPos.y())
		int v1 = m_note->verticalScrollBar()->maximum();
		int v2 = m_note->verticalScrollBar()->minimum();
		int pageStep =m_note->verticalScrollBar()->pageStep();
		int length = v1 - v2 + pageStep;
		m_note->verticalScrollBar()->setValue(m_note->verticalScrollBar()->value() + length/20);	
	}
		
    ItemBase::mousePressEvent( event );
	
	//QTextCursor cur=m_note->cursorForPosition(QPoint(0,0));
	//cur.setPosition(0);
	
	//else
	{
		event->setAccepted( true );
	}
	
	return;
}

void CalloutNote::mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
{
	//if( onBoarder( event->pos() ) )
	{
		ItemBase::mouseReleaseEvent( event );
	}
	event->setAccepted( true );
	m_note->setFocus();
	//m_note->verticalScrollBar()->setZsetZvalue();
	//m_note->verticalScrollBar()->setValue(20);
	return;
}

void CalloutNote::saveAsXmlNode( QDomDocument &doc, QDomElement &node )
{
	QDomText text;
	QString str, tmpStr;

	QDomElement type = doc.createElement( tr("Type") );
	text = doc.createTextNode( tr("Note") );
	type.appendChild( text );
	node.appendChild( type );

	ItemBase::saveAsXmlNode( doc, node );

	QDomElement content = doc.createElement( tr("Content") );
	str = m_note->toPlainText();
	text = doc.createTextNode( str );
	content.appendChild( text );
	node.appendChild( content );
}

void CalloutNote::recoverFromXmlNode( QDomElement node )
{
	QDomNodeList paraNode;
	QString str;

	paraNode = node.elementsByTagName( tr("Content") );
	str = paraNode.at(0).toElement().text();

	m_note->setText( str );
}