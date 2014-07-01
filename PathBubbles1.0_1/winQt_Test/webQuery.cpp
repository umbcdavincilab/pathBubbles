#include "webQuery.h"
#include "openglscene.h"
#include <QTextEdit>

WebQuery::WebQuery(OpenGLScene *scene,  QString target)
{
	delete this->layout();
	this->_layout = new QGridLayout();
	this->setLayout( this->_layout );
	_scene=scene;
	//
	//_move = new moveButton( this );
	//this->_layout->addWidget( _move, 0, 0, 1, 1, Qt::AlignLeft );
	//connect( _move, SIGNAL(pressed()), this, SLOT(moveButtonPress()) );
	//connect( _move, SIGNAL(released()), this, SLOT(moveButtonRelease()) );
	//
	//this->_hide = new QPushButton( "Hide", this );
	//this->_layout->addWidget( _hide, 0, 1);
	//connect( _hide, SIGNAL(clicked()), this, SLOT( needHide() ) );
	//
	
	this->_path = new QLineEdit( this );
	//this->_layout->addWidget( _path, 0, 0, 1, 7 );
	//
	
	//this->_go = new QPushButton( "search in pathBubble", this );
	//this->_layout->addWidget( _go, 0, 7 );
	//connect( _go, SIGNAL(clicked()), this, SLOT(openURL()) );
	//connect( _go, SIGNAL(clicked()), this, SLOT(search()) );
	

	/*QTextEdit* _text;
	_text = new QTextEdit( );	
	_text->setText( tr("ddfdf") );
	_text->setReadOnly(true); 	
	this->_layout->addWidget( _text, 0, 5);*/
	//connect( _go, SIGNAL(clicked()), this, SLOT(search()) );
	//connect( _go, SIGNAL(clicked()),  _scene, SLOT(searchInPathBubble()) );
	
	//
	_view = new QWebView( this );
	if( target.size() != 0 )
	{
		/*QString str = target;
		if (target.indexOf("://") == -1)
		{
			str = "http://";
			str.append( target );
		}
		_view->load( QUrl(str) );*/

		QString links= "http://www.csee.umbc.edu/~keqin/Link_eGift_php/LinktoEgift.php?symbol=";		
		_view->load(QUrl(links+target));	
	}
	else
	{
		//QUrl fromLocalFile ( "data/Reactome_Pathway_Data/sample2.php" );
		//File myFile=new File("/tmp/myfile");
        //URL myUrl = myFile.toURI().toURL();
		//QString links= "http://www.csee.umbc.edu/~keqin/Link_eGift_php/LinktoEgift.php?symbol=";		
		_view->load(QUrl("http://www.google.com"));		
		//); //http://www.csee.umbc.edu/~keqin/Link_eGift_php/sample.php"));//http://www.csee.umbc.edu/~keqin/Link_eGift_php/sample.php//"http://www.csee.umbc.edu/~jichen/tmp/TestPhp/testQt.php//http://biotm.cis.udel.edu/eGIFT/index.php?cat=searchiterm//?user=liang&pass=iTermPageForLiang&gene=gro"));//"http://www.google.com"));
		//_view->load(QUrl("data/Reactome_Pathway_Data/sample2.php"));//http://www.csee.umbc.edu/~keqin/Link_eGift_php/TestPhp/testQt.php//"http://www.csee.umbc.edu/~jichen/tmp/TestPhp/testQt.php//http://biotm.cis.udel.edu/eGIFT/index.php?cat=searchiterm//?user=liang&pass=iTermPageForLiang&gene=gro"));//"http://www.google.com"));
	}
	this->_layout->addWidget( _view, 1, 0, 1, 8 );
	_view->resize( 200, 200 );
	_view->show();
	//
	return;
}

void WebQuery::needHide()
{
	emit needSetToHide();
}

void WebQuery::openURL()
{
	QString str;
	str = this->_path->text();

 	if (str.indexOf("://") == -1)
 	{
 		str = "http://";
 		str.append( this->_path->text() );
 	}

	//_scene->searchInPathBubble(str);
	_view->load(QUrl( str ));
} 


void WebQuery::search()
{
	QString str;
	str = this->_path->text();
	set<int> empty;
	_scene->searchInPathBubble(str,empty, "all");
	//_view->load(QUrl( str ));
} 


void WebQuery::deleteItems()
{
	delete [] this->_layout;
} 
