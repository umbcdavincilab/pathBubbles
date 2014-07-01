#include "SearchFind.h"
#include "openglscene.h"
#include <QTextEdit>

SearchFind::SearchFind(OpenGLScene *scene,  QString target, int type)
{ //type 1: search, type 2: setting 
	
	delete this->layout();
	this->_layout = new QGridLayout();
	this->setLayout( this->_layout );
	_scene=scene;
		
	if(type==2)
	{
		this->_path = new QLineEdit( this );
		this->_path->setMaximumWidth(100);
		this->_layout->addWidget( _path, 0, 0, 1, 3 );			
		this->_go = new QPushButton( "Search", this );	
		
		this->_layout->addWidget( _go, 0, 2 );		
		connect( _go, SIGNAL(clicked()), this, SLOT(search()) );
		/*connect(&m_eQuery, SIGNAL(finished(bool,QString,QString))  
            ,this, SLOT(onQueryFinished(bool,QString,QString))); 

		connect( _go, SIGNAL(clicked()), 
            this, SLOT(onQueryButton()));  */

		msWindowsFixedSizeDialogCheckBox = createCheckBox(tr("small Molecule"));//draw/search protein
	    //x11BypassWindowManagerCheckBox = createCheckBox(tr("Complex"));  //draw/search complex/physical entity
		//windowNoShadowCheckBox = createCheckBox(tr("smallMoleculer"));  //draw/search complex/physical entity
		//framelessWindowCheckBox = createCheckBox(tr("Small Molecule"));
		//msWindowsFixedSizeDialogCheckBox->setChecked(false);
		//SearchList.insert('P');
         SearchList.insert('C');	SearchList.insert('P'); SearchList.insert('D'); SearchList.insert('S'); SearchList.insert('E');  SearchList.insert('R');	SearchList.insert('M');				 
		//_layout->addWidget(msWindowsFixedSizeDialogCheckBox, 2, 1);

		/* QComboBox *comboBox = new QComboBox;
    comboBox->addItem(tr("item 1"));
    comboBox->addItem(tr("item 2"));
    comboBox->addItem(tr("item 3"));
 
    QComboBox *iconComboBox = new QComboBox;
    iconComboBox->addItem(QIcon(":/images/bad.svg"), tr("Bad"));
    iconComboBox->addItem(QIcon(":/images/heart.svg"), tr("Heart"));
    iconComboBox->addItem(QIcon(":/images/trash.svg"), tr("Trash"));

		
	_layout->addWidget(iconComboBox);*/

	QListWidget* listBox = new QListWidget;
	//background: #eeeeee; border: 1px solid #afafaf;border-radius: 10px;margin: 10px 0 0 20px;max-width: 305px;min-height: 100px;} QListWidget::item{ background-image: url(:/pictures/attach_icon.png);background-repeat: no-repeat;background-position: left center; color: #212121;border: 1px solid #afafaf;min-height: 54px;max-height: 54px;padding-left: 54px;}QListWidget::item:selected{background-image: url(:/pictures/attach_icon.png);background-repeat: no-repeat;background-position: left center;background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #cdcdcd, stop: 1 #ababab);border-top: 1px solid;border-top-color: #a8abad;border-bottom: 1px solid;border-bottom-color: #bfc7ce;color: #333538;}");
	//listBox->setStyleSheet("QListWidget{ border: 1px solid #afafaf; border-radius: 10px}");
	
	
	listBox->addItem(tr("item 1"));
    listBox->addItem(tr("item 2"));
    listBox->addItem(tr("item 3")); 
    
	
	//_layout->addWidget(listBox);

		//_layout->addWidget(x11BypassWindowManagerCheckBox, 3, 1);
		//_layout->addWidget(windowNoShadowCheckBox, 4, 1);
		//x11BypassWindowManagerCheckBox->setChecked(true);
	}
	else if(type==3)
	{
		//Display small molecule
		QGridLayout *layout = new QGridLayout;	
		hintsGroupBox = new QGroupBox(tr("Display Objects"));	
		hintsGroupBox->setLayout(layout);		
		this->_layout->addWidget(hintsGroupBox,0,0,13,8);

	    layout->setAlignment(Qt::AlignTop);
		framelessWindowCheckBox = createCheckBox(tr("Small Molecule"));
		//framelessWindowCheckBox->setVisible(false);		
		layout->addWidget(framelessWindowCheckBox, 1, 0);
		if(_scene->drawSmallmoleculeSettings)
		{
			framelessWindowCheckBox->setChecked(true);			
		}	
		
		setNewRadioButton = createRadioButton(tr("apply only to new groups"));		
		layout->addWidget(setNewRadioButton, 2, 0);

		setAllRadioButton = createRadioButton(tr("apply to all groups"));
		layout->addWidget(setAllRadioButton, 3, 0);
		
		setNewRadioButton->setChecked(true);

		layout->setSpacing(2);

		/*
		//Setting about rescale mode			
		WindowDragResize= new QGroupBox(tr("Drag_To_Resize"));				 //rag_To_Resize
		WindowDragResize->setLayout(layout);
		this->_layout->addWidget(WindowDragResize,3,0);	

		dragResizeMode1 = createRadioButton(tr("Uniform scale")); //Word file mode // InterNet explorer mode		
		layout->addWidget(dragResizeMode1, 8, 0);

		dragResizeMode2 = createRadioButton(tr("Word file style")); //Word file mode // InterNet explorer mode		
		layout->addWidget(dragResizeMode2, 15, 0);

		dragResizeMode3 = createRadioButton(tr("Explorer syle"));  //Word file mode // InterNet explorer mode		
		layout->addWidget(dragResizeMode3, 16, 0);
		//if(_scene->resizeMode)
		{
			dragResizeMode2->setChecked(true);			
		}
		*/

		//Size of the bubbles
		//fixed size 
		//top level pathway
		//rest
		//Setting about rescale mode			
		/*
		initBubbleSize = new QGroupBox(tr("Bubble Size"));				 //rag_To_Resize
		initBubbleSize->setLayout(layout);
		this->_layout->addWidget(initBubbleSize,6,0);	

		fixedSizeCheckBox = createCheckBox(tr("Fixed initial size"));
		layout->addWidget(fixedSizeCheckBox, 18, 0);
		//if(_scene->drawSmallmoleculeSettings)
		{
			fixedSizeCheckBox->setChecked(true);			
		}	
		topSizeInput = new QLineEdit( this );
		_layout->addWidget( topSizeInput, 20,0 );

		sonSizeInput = new QLineEdit( this ); //Word file mode // InterNet explorer mode		
		layout->addWidget(sonSizeInput, 22, 0);

		ApplyButton = new QPushButton( "Apply", this );
		this->_layout->addWidget( ApplyButton, 24, 0 );		
		//connect( ApplyButton, SIGNAL(clicked()), this, SLOT(search()) );
		*/
	}		
	else if(type==4)
	{
		//Display small molecule
		QGridLayout *layout = new QGridLayout;	
		hintsGroupBox = new QGroupBox(tr("Search Objects"));	
		hintsGroupBox->setLayout(layout);

		layout->setSpacing(0);
		layout->setMargin(0);

		QButtonGroup *bg1 = new QButtonGroup(this);
		QButtonGroup *bg2 = new QButtonGroup(this);
		
		_layout->addWidget(hintsGroupBox,0,0,5,1);
		
		layout->setAlignment(Qt::AlignTop);
		proteinRadioButton = createRadioButton(tr("Protein"));
		layout->addWidget(proteinRadioButton, 1, 0);
		
		compartmentRadioButton = createRadioButton(tr("Compartment"));
		layout->addWidget(compartmentRadioButton, 2, 0);

		pathwayRadioButton = createRadioButton(tr("Pathway"));
		layout->addWidget(pathwayRadioButton, 3, 0);	


		bg1->addButton(proteinRadioButton);
		bg1->addButton(compartmentRadioButton);
		bg1->addButton(pathwayRadioButton);

		if(_scene->searchType == 'P')
		{
			proteinRadioButton->setChecked(true);			
		}	
		else if(_scene->searchType == 'M')
		{
			compartmentRadioButton->setChecked(true);			
		}
		else if(_scene->searchType == 'W')
		{
			pathwayRadioButton->setChecked(true);			
		}		
		setNewRadioButton = createRadioButton(tr("apply only to new groups"));		
		layout->addWidget(setNewRadioButton, 4, 0);

		setAllRadioButton = createRadioButton(tr("apply to all groups"));
		layout->addWidget(setAllRadioButton, 5, 0);	

		bg2->addButton(setNewRadioButton);
		bg2->addButton(setAllRadioButton);

		setNewRadioButton->setChecked(true);
	}	
	else if(type==5)
	{   //search setting 2
		//
		//Display small molecule
		QGridLayout *layout = new QGridLayout;	
		hintsGroupBox = new QGroupBox(tr("Search Relations"));	
		hintsGroupBox->setLayout(layout);		
		this->_layout->addWidget(hintsGroupBox,0,0,13,8);

	    layout->setAlignment(Qt::AlignTop);
		
		sharedCheckBox = createCheckBox(tr("shared"));
		layout->addWidget(sharedCheckBox, 1, 0);

		differedCheckBox = createCheckBox(tr("differed"));
		layout->addWidget(differedCheckBox, 1, 0);

		linkedCheckBox = createCheckBox(tr("linked"));
		layout->addWidget(linkedCheckBox, 1, 0);

		expressedCheckBox = createCheckBox(tr("expressed"));
		layout->addWidget(expressedCheckBox, 1, 0);
		
		if(_scene->searchShared)
		{
			sharedCheckBox->setChecked(true);			
		}
		if(_scene->searchDiffered)
		{
			differedCheckBox->setChecked(true);			
		}
		if(_scene->searchLinked)
		{
			linkedCheckBox->setChecked(true);			
		}
		if(_scene->searchExpressed)
		{
			expressedCheckBox->setChecked(true);			
		}
				
		setNewRadioButton = createRadioButton(tr("apply only to new groups"));		
		layout->addWidget(setNewRadioButton, 2, 0);

		setAllRadioButton = createRadioButton(tr("apply to all groups"));
		layout->addWidget(setAllRadioButton, 3, 0);
		
		setNewRadioButton->setChecked(true);

		layout->setSpacing(2);
	}	
	/*if(type==6)
	{
		this->_path = new QLineEdit( this );
		this->_path->setMaximumWidth(100);
		this->_layout->addWidget( _path, 0, 0, 1, 3 );			
		this->_go = new QPushButton( "Search", this );		
		this->_layout->addWidget( _go, 0, 2 );		
		connect( _go, SIGNAL(clicked()), this, SLOT(search()) );

		msWindowsFixedSizeDialogCheckBox = createCheckBox(tr("small Molecule"));//draw/search protein
	    //x11BypassWindowManagerCheckBox = createCheckBox(tr("Complex"));  //draw/search complex/physical entity
		//windowNoShadowCheckBox = createCheckBox(tr("smallMoleculer"));  //draw/search complex/physical entity
		//framelessWindowCheckBox = createCheckBox(tr("Small Molecule"));
		//msWindowsFixedSizeDialogCheckBox->setChecked(false);
		//SearchList.insert('P');
         SearchList.insert('C');	SearchList.insert('P'); SearchList.insert('D');  SearchList.insert('E');  SearchList.insert('R');	SearchList.insert('M');		
		//_layout->addWidget(msWindowsFixedSizeDialogCheckBox, 2, 1);

		
		//_layout->addWidget(x11BypassWindowManagerCheckBox, 3, 1);
		//_layout->addWidget(windowNoShadowCheckBox, 4, 1);
		//x11BypassWindowManagerCheckBox->setChecked(true);
	}*/
}

void SearchFind::setInvisible()
{
	setAllRadioButton->setVisible(false);
	setNewRadioButton->setVisible(false);
}

void SearchFind::setVisible()
{
	setAllRadioButton->setVisible(true);
	setNewRadioButton->setVisible(true);
}

void SearchFind::deleteItems( )
{
	if(_webItem->getType()==SEARCHFIND)
	{
		this->_layout->removeWidget(_go);
	    this->_layout->removeWidget(_path);
	}
	else if(_webItem->getType()==SETTINGS)
	{
		this->_layout->removeWidget(hintsGroupBox);	
	}
	else if(_webItem->getType()==SEARCHSETTINGS)
	{
		this->_layout->removeWidget(hintsGroupBox);	
	}
	delete [] this->_layout; 
}

void SearchFind::setDrawSmallMolecule(bool flag)
{	 
	 framelessWindowCheckBox->setChecked(flag);
	 //flag=flag;
}

void SearchFind::updatePreview_2()
{//radio button
	if(_webItem==NULL)
		return;

    if(_webItem->getType()==SETTINGS)
	{
		bool flags = 0;
		int count=0;	
		QList<ItemBase *> mlist;
		if(setAllRadioButton->isChecked())
		{
			mlist=_scene->allMembers();	 			
		}
		else
		{
			mlist=_scene->getGroupMembers(_webItem);	 
		}
		for(int i=0; i<mlist.size(); i++) 		
		{	
			int type=mlist[i]->getType();
			if ( type == SUBPATHBUBBLE1 || type == PATHBUBBLE1)
			{
				PathBubble1* pbubble = dynamic_cast<PathBubble1*>(mlist[i]);	
				if (setAllRadioButton->isChecked())
				{	
					if(framelessWindowCheckBox->isChecked())
					{
						if(!pbubble->drawSmallmolecule)
						{
							pbubble->drawSmallmolecule=true;
							pbubble->redrawPathWay();	
							pbubble->getGraphToBePaint();						
						}								
					}
					else
					{
					    if(pbubble->drawSmallmolecule)
						{
							pbubble->drawSmallmolecule=false;
							pbubble->redrawPathWay();	
							pbubble->getGraphToBePaint();						
						}					
					}
				}
			}
			if ( type == SETTINGS)
			{
				if (setAllRadioButton->isChecked())
				{	
					WebBlock *settingItem = dynamic_cast<WebBlock *>( mlist[i]);
					if(framelessWindowCheckBox->isChecked())
				         settingItem->_settings->framelessWindowCheckBox->setChecked(true);		
					else 
					{
						settingItem->_settings->framelessWindowCheckBox->setChecked(false);			
					}								
				}				
			}
		}
	} 
	else if(_webItem->getType()==SEARCHSETTINGS)
	{		
			
		if(proteinRadioButton->isChecked())
		{
		    _scene->searchType='P';		
		}
		else if(compartmentRadioButton->isChecked())
		{
			_scene->searchType='M';
		}	
		else if(pathwayRadioButton->isChecked())
		{
			_scene->searchType='W';
		}

		QList<ItemBase *> mlist;
		if(setAllRadioButton->isChecked())
		{
			mlist=_scene->allMembers();	 			
		}
		else
		{
			mlist=_scene->getGroupMembers(_webItem);	 
		}
		for(int i=0; i<mlist.size(); i++) 
		{
			int type=mlist[i]->getType();
			PathBubble1* pbubble = dynamic_cast<PathBubble1*>(mlist[i]);
			if ( mlist[i]->getType() == SUBPATHBUBBLE1 || mlist[i]->getType() == PATHBUBBLE1)
			{
				if(proteinRadioButton->isChecked())
				{
		             pbubble->searchType='P';		
				}
				else if(compartmentRadioButton->isChecked())
				{
					pbubble->searchType='M';
				}	
				else if(pathwayRadioButton->isChecked())
				{
					pbubble->searchType='W';
				}
			}
		}
	}
}


void SearchFind::updatePreview_1()
{//check box
	if(_webItem==NULL)
		return;
	if(_webItem->getType()==SETTINGS)
	{
		bool flags = 0;
		int count=0;	
		QList<ItemBase *> mlist;
		if(setAllRadioButton->isChecked())
		{
			mlist=_scene->allMembers();	 			
		}
		else
		{
			mlist=_scene->getGroupMembers(_webItem);	 
		}
		for(int i=0; i<mlist.size(); i++) 
		{
			int type=mlist[i]->getType();
			if ( type == SUBPATHBUBBLE1 || type == PATHBUBBLE1)
			{
				PathBubble1* pbubble = dynamic_cast<PathBubble1*>(mlist[i]);	
				if (framelessWindowCheckBox->isChecked())
				{				
					if(!pbubble->drawSmallmolecule)
					{
						pbubble->drawSmallmolecule=true;
						pbubble->redrawPathWay();	
						pbubble->getGraphToBePaint();
					}
				}
				else
				{
					if(pbubble->drawSmallmolecule)
					{
						pbubble->drawSmallmolecule=false;
						pbubble->redrawPathWay();	
						pbubble->getGraphToBePaint();
					}
				}
			}
			if ( type == SETTINGS)
			{
				if(framelessWindowCheckBox->isChecked())
			    {
					WebBlock *settingItem = dynamic_cast<WebBlock *>( mlist[i]);
				    settingItem->_settings->framelessWindowCheckBox->setChecked(true);		
					//->framelessWindowCheckBox->setChecked(true);			
				}
				else 
				{
					WebBlock *settingItem = dynamic_cast<WebBlock *>( mlist[i]);
				    //settingItem->_settings->setInvisible(); 
					//->framelessWindowCheckBox->setChecked(true);			
					settingItem->_settings->framelessWindowCheckBox->setChecked(false);			
				}
			}
			count++;
		}		
		if(count==1) 
		{
			//_scene->searchInPathBubble(str, "all", NULL);	
			if (framelessWindowCheckBox->isChecked())
			{
				_scene->drawSmallmoleculeSettings=true;	
			}
			else
			{
				_scene->drawSmallmoleculeSettings=false;			
			}
		}	
	} 
	else if(_webItem->getType()==SEARCHFIND)
	{			
		if(msWindowsFixedSizeDialogCheckBox->isChecked())
		{
			SearchList.insert('S');
		}	
		else
			SearchList.erase('S');		
	}
}

QCheckBox *SearchFind::createCheckBox(const QString &text)
{
    QCheckBox *checkBox = new QCheckBox(text);
    connect(checkBox, SIGNAL(clicked()), this, SLOT(updatePreview_1()));

	_webItem=NULL;
    return checkBox;
}

QRadioButton *SearchFind::createRadioButton(const QString &text)
{
    QRadioButton *button = new QRadioButton(text);
    connect(button, SIGNAL( toggled(bool)), this, SLOT(updatePreview_2()));
	_webItem=NULL;
    return button;
}

void SearchFind::needHide()
{
	emit needSetToHide();
}

void SearchFind::openURL()
{
	QString str;
	str = this->_path->text();

 	if (str.indexOf("://") == -1)
 	{
 		str = "http://";
 		str.append( this->_path->text() );
 	}
} 

void SearchFind::onQueryButton()  
{  
    QString ip = "125.76.23.5";//"gro";//m_ipEdit->text();  //
    if(!ip.isEmpty())  
    {  
        m_eQuery.query(ip);  
       // m_ipEdit->setDisabled(true);  
       // m_queryButton->setDisabled(true);  
    }  
  
}  

void SearchFind::onQueryFinished(bool bOK, QString ip, QString area)
{	
	/*if(bOK)  
    {  
        m_areaLabel->setText(area);  
    }  
    else  
    {  
        m_areaLabel->setText("喔哟，出错了");  
    }  
    m_ipEdit->setEnabled(true);  
    m_queryButton->setEnabled(true);  
	*/
}


void SearchFind::search()
{
	 
	int count=0,itemCount=0;
	QString str;
	str = this->_path->text();
	if(str.size()==0)
	{
		//_scene->searchInPathBubble();
		return;
	}
	
	int index;
	if(str.contains(" *"))
	while(str.indexOf(" ")==0)
	{	
		index = str.indexOf(" ");
		str = str.mid(index+1, str.size()-index);		
	}
	
	if(str.contains("* "))
	while(str.lastIndexOf(" ")==str.size()-1)
	{	
		index= str.lastIndexOf(" ");				
		str = str.mid(0, index);		
		if(str.size()==0)
			break;
	}	
	QList<ItemBase *> mlist=_scene->getGroupMembers(_webItem);	 
	for(int i=0; i<mlist.size(); i++) 
	{
		int type=mlist[i]->getType();
	    if ( mlist[i]->getType() == SUBPATHBUBBLE1 || mlist[i]->getType() == PATHBUBBLE1)
		{			
			itemCount += _scene->searchInPathBubble(str, SearchList, "",  mlist[i]);	
		}
		if ( mlist[i]->getType() == EXPRESSIONBUBBLE)
		{			
			itemCount +=_scene->searchInExpressionBubble(str, SearchList, "",  mlist[i]);	
		}
		if ( mlist[i]->getType() == TREERING)
		{			
			itemCount +=_scene->searchInTreeringBubble(str, SearchList, "",  mlist[i]);	
			itemCount +=_scene->searchPathwayInTreeRingforASymbol(str, SearchList, "",  mlist[i]);	
		}
		count++;
	}
	/*if(count==1) 
	{
		_scene->searchInPathBubble(str, SearchList, "all", NULL);
		_scene->searchInExpressionBubble(str, SearchList, "all",  NULL);	
	}*/
	if(itemCount==0)
	{
	       QMessageBox msgBox;
			msgBox.setWindowTitle("No item found");
			msgBox.setText("No item is found");
			msgBox.setStandardButtons(QMessageBox::Yes);
			if(msgBox.exec() == QMessageBox::Yes)
				return;
			/*msgBox.addButton (QMessageBox::No);
			if(msgBox.exec() == QMessageBox::No)
				return false;
			else 
				break;*/
	
	}
} 
