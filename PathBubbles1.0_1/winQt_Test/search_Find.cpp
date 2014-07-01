#include "SearchFind.h"
#include "openglscene.h"
#include <QTextEdit>

SearchFind::SearchFind(OpenGLScene *scene,  QString target)
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
	this->_layout->addWidget( _path, 0, 0, 1, 7 );
	//
	
	this->_go = new QPushButton( "search in pathBubble", this );
	this->_layout->addWidget( _go, 0, 7 );
	connect( _go, SIGNAL(clicked()), this, SLOT(openURL()) );
	connect( _go, SIGNAL(clicked()), this, SLOT(search()) );


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
	_view->resize( 200, 200 );
	_view->show();
	//
	return;
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

	//_scene->searchInPathBubble(str);
	_view->load(QUrl( str ));
} 


void SearchFind::search()
{
	QString str;
	str = this->_path->text();

	_scene->searchInPathBubble(str);
	//_view->load(QUrl( str ));
} 
