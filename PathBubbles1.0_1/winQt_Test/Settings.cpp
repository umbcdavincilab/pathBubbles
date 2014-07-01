#include "Settings.h"
#include "openglscene.h"
#include <QTextEdit>

Settings::Settings(OpenGLScene *scene,  QString target)
{
	delete this->layout();
	this->_layout = new QGridLayout();
	this->setLayout( this->_layout );
	_scene=scene;
		
	this->_path = new QLineEdit( this );
	this->_layout->addWidget( _path, 0, 0, 1, 7 );
	
	this->_go = new QPushButton( "search", this );
	this->_layout->addWidget( _go, 0, 7 );
	connect( _go, SIGNAL(clicked()), this, SLOT(openURL()) );
	connect( _go, SIGNAL(clicked()), this, SLOT(search()) );
	
	/*
	 QTextEdit *edit=new QTextEdit();
	QCheckBox *font=new QCheckBox();
	
//font->setFontFilters(QFontComboBox::NonScalableFonts);
	font->setAutoExclusive(true);

//font->setWritingSystem(QFontDatabase::Thaana);

connect(font,SIGNAL(currentFontChanged(QFont)),edit,SLOT(setCurrentFont(QFont)));

    this->_layout->addWidget( font, 1, 7 );
    */

	QGridLayout *layout = new QGridLayout;
	this->_path = new QLineEdit( this );
	this->_layout->addWidget( _path, 0, 0, 1, 7 );

    /*typeGroupBox = new QGroupBox(tr("Type"));
    windowRadioButton = createRadioButton(tr("Window"));
    dialogRadioButton = createRadioButton(tr("Dialog"));    
    windowRadioButton->setChecked(true);
    
    layout->addWidget(windowRadioButton, 0, 7);
    layout->addWidget(dialogRadioButton, 1, 7);*/

	hintsGroupBox = new QGroupBox(tr("Diagram Key"));

    //msWindowsFixedSizeDialogCheckBox = createCheckBox(tr("Protein"));
    //x11BypassWindowManagerCheckBox =  createCheckBox(tr("Complex"));
    framelessWindowCheckBox = createCheckBox(tr("Small Molecule"));
    //windowNoShadowCheckBox = createCheckBox(tr("Dna"));
    //windowTitleCheckBox = createCheckBox(tr("Protein Set"));
    
    //layout->addWidget(msWindowsFixedSizeDialogCheckBox, 0, 0);
    //layout->addWidget(x11BypassWindowManagerCheckBox, 1, 0);
    layout->addWidget(framelessWindowCheckBox, 0, 0);
    //layout->addWidget(windowNoShadowCheckBox, 3, 0);
    //layout->addWidget(windowTitleCheckBox, 4, 0);

	//x11BypassWindowManagerCheckBox->setChecked(true);

	//msWindowsFixedSizeDialogCheckBox->setChecked(true);

	if(_scene->drawSmallmolecule)
	    framelessWindowCheckBox->setChecked(true);

    hintsGroupBox->setLayout(layout);
	this->_layout->addWidget(hintsGroupBox);

	//this->_layout->addWidget(typeGroupBox);


	/*
	_view = new QWebView( this );
	if( target.size() != 0 )
	{
		QString str = target;
		if (target.indexOf("://") == -1)
		{
			str = "http://";
			str.append( target );
		}
		_view->load( QUrl(str) );
	}
	else
	{
		_view->load(QUrl("http://www.google.com"));
	}
	this->_layout->addWidget( _view, 1, 0, 1, 8 );
	_view->resize( 200, 100 );
	_view->show();
	//
	return;*/	
	//this->resize(600,600);	
	//resetOriRect();
}

/*void Settings::resetOriRect()
{
	int s=this->geometry().width();
	//his->
	//int x=this->_view->pos().x();
	//this->resize(100,100);
	//	WebBlock* web = new WebBlock( x, y, this->_scene, 2, name );
	//web->resizeItem( size, size/2 );
	//_oriRect=QRectF(sceneBoundingRect().x(), sceneBoundingRect().y(), sceneBoundingRect().width(), sceneBoundingRect().height()); //never change
	_curScale=1.0; //never change	
}

void Settings::scaleUpBasedonOriRect()
{
	_curScale=_curScale*1.10;
	QRectF rect=QRectF(_oriRect.x()*_curScale, _oriRect.y()*_curScale, _oriRect.width()*_curScale, _oriRect.height()*_curScale); //never change
	//mySetPos( QPointF(rect.center().x(), rect.center().y()));
	//resizeItem( rect.width(), rect.height() );
}

void Settings::scaleDownBasedonOriRect()
{
	_curScale=_curScale/1.10;
	QRectF rect=QRectF(_oriRect.x()*_curScale, _oriRect.y()*_curScale, _oriRect.width()*_curScale, _oriRect.height()*_curScale); //never change
	//mySetPos( QPointF(rect.center().x(), rect.center().y()));
	//resizeItem( rect.width(), rect.height() );
}*/



/*void expressionBubble::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
	if (!isSelected() && _scene) 
	{
		_scene->clearSelection();
		setSelected(true);
	}
	QMenu menu;
	QAction *delAction = menu.addAction("Delete");	
	
	QAction *selectedAction = menu.exec(event->screenPos());

	if (selectedAction == delAction)
	{
		QMessageBox msgBox;
		msgBox.setWindowTitle("Delete Bubble");
		msgBox.setText("Will you delete it?");
		msgBox.setStandardButtons(QMessageBox::Yes);
		msgBox.addButton (QMessageBox::No);
		if(msgBox.exec() == QMessageBox::Yes)
			deleteSelectedItems(_scene);	
	}	
}*/


Settings::~Settings( )
{
	this->_layout->removeWidget(_go);
	this->_layout->removeWidget(_path);
	delete [] this->_layout;
    //QDialog::~QDialog();	
}

void Settings::updatePreview()
{
    bool flags = 0;

    //if (msWindowsFixedSizeDialogCheckBox->isChecked())
    //    flags = 1;
    //if (x11BypassWindowManagerCheckBox->isChecked())
    //    flags = 1;
    if (framelessWindowCheckBox->isChecked())
    {
		_scene->drawSmallmolecule=true;
		_scene->redrawPathWay();
	}
	else
	{
		_scene->drawSmallmolecule=false;
		_scene->redrawPathWay();
	}

    //if (windowNoShadowCheckBox->isChecked())
    //    _scene->drawDna=true;
	//else _scene->drawDna=false;
   // if (windowTitleCheckBox->isChecked())
   //     flags = 1;
    

    /*previewWindow->setWindowFlags(flags);
    QPoint pos = previewWindow->pos();
    if (pos.x() < 0)
        pos.setX(0);
    if (pos.y() < 0)
        pos.setY(0);
    previewWindow->move(pos);
    previewWindow->show();*/
}

QCheckBox *Settings::createCheckBox(const QString &text)
{
    QCheckBox *checkBox = new QCheckBox(text);
    connect(checkBox, SIGNAL(clicked()), this, SLOT(updatePreview()));
    return checkBox;
}

QRadioButton *Settings::createRadioButton(const QString &text)
{
    QRadioButton *button = new QRadioButton(text);
    connect(button, SIGNAL(clicked()), this, SLOT(updatePreview()));
    return button;
}

void Settings::needHide()
{
	emit needSetToHide();
}

void Settings::openURL()
{
	QString str;
	str = this->_path->text();

 	if (str.indexOf("://") == -1)
 	{
 		str = "http://";
 		str.append( this->_path->text() );
 	}

	//_scene->searchInPathBubble(str);
	//_view->load(QUrl( str ));
} 


void Settings::search()
{
	int count=0;
	QString str;
	str = this->_path->text();

	QList<ItemBase *> mlist=_scene->getGroupMembers(_webItem);	 
	for(int i=0; i<mlist.size(); i++) 
	{
		int type=mlist[i]->getType();
	    if ( mlist[i]->getType() == SUBPATHBUBBLE1 || mlist[i]->getType() == PATHBUBBLE1)
		{
	         _scene->searchInPathBubble(str, "", mlist[i]);				
		}
		 count++;
	}
	if(count==1) 
	{
		_scene->searchInPathBubble(str, "all", NULL);	
	}	
	//_view->load(QUrl( str ));
} 
