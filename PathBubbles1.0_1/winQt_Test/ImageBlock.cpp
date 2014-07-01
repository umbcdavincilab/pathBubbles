#include "ImageBlock.h"
#include "openglscene.h"

ImageBlock::ImageBlock(int x, int y, OpenGLScene *scene, QString path, float rate )
: ControlBubble( x, y , scene )
{
	_path = path;
	initUI();
	this->_TYPE = IMAGE;
	bIndex = scene->getBubbleIndex();
	this->_REMOVECONTROL = true;
	imagePlayer->setFrameByRate( rate );

	return;
}

void ImageBlock::logOpen()
{
	ItemBase::logOpen();
	
	emit logInfor("\n");
}

void ImageBlock::initUI()
{
	QWidget* widget = TDHelper::createDialog( tr("") );
	widget->resize( CONTROL, CONTROL );
	delete widget->layout();
	QGridLayout* layout = new QGridLayout();
	widget->setLayout( layout );
	//		init UI first
	imagePlayer = new ImagePlayer( widget, _path );
	this->imagePlayer->resize( CONTROL - 20, CONTROL - 20 );
	layout->addWidget( this->imagePlayer, 0, 0 );
	//************************************************
	this->_myControl = this->_scene->addWidget( widget );
	return;
}

void ImageBlock::setFrameByPercentate(float rate)
{
	imagePlayer->setFrameByRate( rate );
	return;
}
