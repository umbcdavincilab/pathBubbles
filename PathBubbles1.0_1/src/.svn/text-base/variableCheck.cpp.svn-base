#include "variableCheck.h"
#include "openglscene.h"

VariableCheck::VariableCheck( int x, int y, OpenGLScene* scene )
: ControlBubble( x, y, scene )
{
	initUI();
	this->_REMOVECONTROL = true;
	return;
}

void VariableCheck::acceptResult( MatResult result )
{
	if( this->_name->text().size() == 0 || result.name != this->_name->text() )
	{
		return;
	}
	this->_text->clear();
	this->_text->append( QObject::tr("Name:") + result.name );
	QString m;
	m.setNum( result.m );
	this->_text->append( QObject::tr("M:") + m );
	QString n;
	n.setNum( result.n );
	this->_text->append( QObject::tr("N:") + m );
	//****************************************************
	displayVariables( result );
	//****************************************************
	return;
}

void VariableCheck::initUI()
{
	QWidget* widget = TDHelper::createDialog( tr("") );
	widget->resize( CONTROL, CONTROL );
	delete widget->layout();
	QGridLayout* layout = new QGridLayout();
	widget->setLayout( layout );
	//
	this->_text = new QTextEdit( widget );
	layout->addWidget( this->_text, 0, 0, 1, 2 );
	//
	this->_name = new QLineEdit( widget );
	layout->addWidget( this->_name, 1, 0 );
	//
	this->_go = new QPushButton( QObject::tr("Check"), widget );
	layout->addWidget( this->_go, 1, 1 );
	connect( this->_go, SIGNAL(clicked()), this, SLOT(needQuery()) );
	//
	this->_myControl = _scene->addWidget( widget );
	return;
}

void VariableCheck::needQuery()
{
	emit query( this->_name->text() );
	return;
}

void VariableCheck::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
	return;
}

void VariableCheck::displayVariables( MatResult result )
{
	for( int i = 0; i < result.m; i++ )
	{
		QString title = makeTitle( i*result.n, (i+1)*result.n );
		this->_text->append( title );
		QString data;
		for( int j = 0; j < result.n; j++ )
		{
			QString doubleNum;
			doubleNum.setNum( result.data[i*result.n + j] );
			data.append( doubleNum + QObject::tr(" ") );
		}
		this->_text->append( data );
	}
	return;
}

QString VariableCheck::makeTitle(int m, int n)
{
	QString result( tr("From ") );
	//***********************************
	QString _M;
	QString _N;
	_M.setNum( m );
	_N.setNum( n-1 );
	result.append( _M + tr(" to ") );
	result.append( _N );
	//***********************************
	return result;
}