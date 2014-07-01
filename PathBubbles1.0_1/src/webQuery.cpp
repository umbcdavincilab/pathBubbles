#include "webQuery.h"

WebQuery::WebQuery( QString target )
{
	delete this->layout();
	this->_layout = new QGridLayout();
	this->setLayout( this->_layout );
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
	this->_go = new QPushButton( "Go", this );
	this->_layout->addWidget( _go, 0, 7 );
	connect( _go, SIGNAL(clicked()), this, SLOT(openURL()) );
	//
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
		_view->load(QUrl("http://www.google.com/"));
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

	_view->load(QUrl( str ));
} 
