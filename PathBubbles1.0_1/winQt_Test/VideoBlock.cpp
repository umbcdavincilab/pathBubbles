#include "VideoBlock.h"
#include "openglscene.h"

VideoBlock::VideoBlock(int x, int y, OpenGLScene *scene, QString path, float begin, float end )
: ControlBubble( x, y, scene )
, _begin( begin )
, _end( end )
, _LABEL( false )
{
	this->_path = path;
	initUI();
	this->_TYPE = VIDEO;
	this->bIndex = scene->getBubbleIndex();
	this->_REMOVECONTROL = true;
	
	nameRect = _player->getNameRect();

	_label = NULL;

	//rawDataView = new BatDataView( scene->getStaManager() );
	_RAWDATAVIEW = false;

	return;
}

void VideoBlock::logOpen()
{
	ItemBase::logOpen();
	
	QString logtext;
	logtext.append( " with bat name " );
	logtext.append( this->getPlayerWidget()->getName() );
	logtext.append( "\n" );
	
	emit logInfor( logtext );
}

void VideoBlock::logPause()
{
	QString logtext;
	
	logtext.append( headofLogMsg() );
	logtext.append( "The video pause");
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void VideoBlock::logPlay()
{
	QString logtext;

	logtext.append( headofLogMsg() );
	logtext.append( "The video is playing");
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void VideoBlock::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	//updateLabel();
	ControlBubble::paint( painter, option, widget );
	
	if (_RAWDATAVIEW)
	{
		//rawDataView->paintDataView( painter );
	}
	return;
}

QString VideoBlock::checkPath( QString path )
{
	QString result;
	if( !QFile::exists( path ) )
	{
		result.append( "Video file cannot be opened!" );
	}
	else
	{
		result.append( "Success" );
	}
	return result;
}

void VideoBlock::initUI()
{
	this->_player = new myVideoPlayer( 0, _path, _begin, _end );
	this->_player->resize( CONTROL - 20, CONTROL - 20 );
	_player->setMask( roundRect(QRect(0, 0, m_size_width, m_size_height), 20 ) );
	this->_myControl = this->_scene->addWidget( _player );
	return;
}

void VideoBlock::updateLabel()
{
	if (_LABEL == false)
	{
		_label = new ItemLabel(this, _scene, m_size_width, m_size_height);
		_label->setLabelColor(_colorBoarder);
		this->_scene->addItem(_label);
		_LABEL = true;

		emit updateLabelState( this );

		return;
	}
	_label->updateLabel(m_size_width, m_size_height);
}

myVideoPlayer* VideoBlock::getPlayerWidget()
{
	return this->_player;
}

void VideoBlock::saveAsXmlNode( QDomDocument &doc, QDomElement &node )
{
	QDomText text;

	QDomElement type = doc.createElement( tr("Type") );
	text = doc.createTextNode( tr("Video") );
	type.appendChild( text );
	node.appendChild( type );

	ItemBase::saveAsXmlNode( doc, node );

	QDomElement path = doc.createElement( tr("Path") );
	text = doc.createTextNode( this->_path );
	path.appendChild( text );
	node.appendChild( path );
}

void VideoBlock::recoverFromXmlNode( QDomElement node )
{
	ItemBase::recoverFromXmlNode( node );
}

void VideoBlock::resizeItem( int size1, int size2 )
{
	ItemBase::resizeItem( size1, size2 );
	this->_myControl->resize( size1, size2 );
	_player->setMask( roundRect(QRect(0, 0, m_size_width, m_size_height), 20 ) );
}

void VideoBlock::addBatWithVideo()
{
	/*QtBat* bat = this->_scene->getGroupManager()->getBatLinkWithVideo( this );

	if ( bat != NULL )
	{
		this->_player->acceptBat( bat );
	}*/
}

void VideoBlock::hoverMoveEvent( QGraphicsSceneHoverEvent *event )
{
	if ( nameRect.contains( event->pos() ) )
	{
		_RAWDATAVIEW = true;
		//rawDataView->setPos( event->pos() );
		//rawDataView->setBatName( _player->getName() );
	}
	else
	{
		_RAWDATAVIEW = false;
	}
} 