#include "videoplayer.h"
//#include "Video.h"
#include <QtMultimedia>

myVideoPlayer::myVideoPlayer(QWidget *parent)
: QWidget(parent)
, surface(0)
, playButton(0)
, positionSlider(0)
, _name( tr("") )
, _percentage( 0.0 )
, BATADDED( false )
{
	//connect(&movie, SIGNAL(stateChanged(QMovie::MovieState)), this, SLOT(movieStateChanged(QMovie::MovieState)));
	//connect(&movie, SIGNAL(frameChanged(int)), this, SLOT(frameChanged(int)));
	//Video *videoWidget = new Video;
	//surface = videoWidget->videoSurface();
	initUI();
}

myVideoPlayer::myVideoPlayer( QWidget *parent, QString name, float begin, float end )
: QWidget(parent)
, surface(0)
, playButton(0)
, positionSlider(0)
, BATADDED( false )
{
	if ( begin > end )
	{
		swap( begin, end );
	}

	this->_name = name;
	this->_begin = begin;
	this->_end = end;
	initUI();
	this->openPath( name );
	return;
}

myVideoPlayer::~myVideoPlayer()
{
	delete this->_bat;
}

void myVideoPlayer::acceptBat( ItemBase* bat )
{
	_bat = dynamic_cast<QtBat*>( bat );
	_bat->show();
	this->BATADDED = true;
	return;
}

void myVideoPlayer::initUI()
{
	player = new Phonon::VideoPlayer(Phonon::VideoCategory);
	this->connect( this, SIGNAL( seek ( qint64 ) ), player, SLOT( seek ( qint64 ) ) );
	//player->play();
	QBoxLayout *controlLayout = new QHBoxLayout;
	controlLayout->setMargin(0);
	this->timer = new QTimer();
	connect( timer, SIGNAL(timeout()), this, SLOT(updateFrame()) );
	this->timer->start( 50 );
	//
	if( this->_name.size() == 0 )
	{
		QAbstractButton *openButton = new QPushButton(tr("Open"));
		connect(openButton, SIGNAL(clicked()), this, SLOT(openFile()));
		controlLayout->addWidget( openButton );
	}
	//*******************************************************************
	//captureButton = new QPushButton(tr("Capture"));
	//connect( captureButton, SIGNAL(clicked()), this, SLOT(capture()) );
	//*******************************************************************
	playButton = new QPushButton;
	playButton->setEnabled(false);
	playButton->setIcon(style()->standardIcon( QStyle::SP_MediaPlay ));
	connect( playButton, SIGNAL(clicked()), this, SLOT(play()));
	//positionSlider = new QSlider(Qt::Horizontal);
	positionSlider = new mySlider( this );
	positionSlider->setRange(0, 100);
	connect( positionSlider, SIGNAL( sliderMoved(int) ), this, SLOT( setPosition(int) ));
	//************************************************************
	controlLayout->addWidget( playButton );
	//controlLayout->addWidget( captureButton );
	controlLayout->addWidget(positionSlider);

	QString namefordisplay = _name;
	int index = namefordisplay.indexOf("_");
	namefordisplay.remove( 0, index+1 );
	index = namefordisplay.lastIndexOf(".");
	namefordisplay.chop( namefordisplay.size() - index );
	nameLabel = new QLabel;
	nameLabel->setAlignment( Qt::AlignCenter );
	nameLabel->setText( namefordisplay );

	QBoxLayout *layout = new QVBoxLayout;
	layout->addWidget( nameLabel );
	layout->addWidget( player );
	layout->addLayout(controlLayout);
	setLayout(layout);
}

void myVideoPlayer::openPath(QString path)
{
	if (!path.isEmpty()) 
	{
		_name = path;
		//player->play(  Phonon::MediaSource( fileName ) );
		this->player->load( Phonon::MediaSource( path ) );
		playButton->setEnabled(true);
		player->seek( 0 );
		play();
	}
	return;
}

void myVideoPlayer::openFile()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Movie"));
	this->_name = fileName;
	if (!fileName.isEmpty()) 
	{
		//player->play(  Phonon::MediaSource( fileName ) );
		this->player->load( Phonon::MediaSource( fileName ) );
		playButton->setEnabled(true);
		player->seek( 0 );
		//movie.jumpToFrame(0);
	}
}

void myVideoPlayer::play()
{
	if( this->player->isPlaying() == true )
	{
		this->player->pause();
		playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
		emit logPause();
	}
	else
	{
		this->player->play();
		playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
		emit logPlay();
	}
}

void myVideoPlayer::updateFrame()
{
	totalTime = this->player->totalTime();
	int frame = float(this->player->currentTime()) / float(totalTime) * 100;
	if( this->player->isPlaying() )
	{
		this->positionSlider->setValue( frame );
	}
	_percentage = float( frame ) / 100.0;
	float begin = _begin*30.0/totalTime;
	float end = _end*30.0/totalTime;
	positionSlider->setHighlight( begin, end, totalTime/1000, _begin, _end );
	this->timer->start( 50 );
	//********************************************************

	qint64 curTime = this->player->currentTime();
	if( curTime > _begin*30 && curTime < _end*30 )
	{
		if ( this->BATADDED == false )
		{
			emit addBatWithVideo();
		}
		if ( this->BATADDED && this->_bat )
		{
			float rate = ( this->player->currentTime() - _begin*30 ) / ( ( _end - _begin) * 30.0 );
			this->_bat->setFrame( rate, (int)(rate*100) );
		}
	}
	return;
}

void myVideoPlayer::setPosition(int frame)
{
	if( this->player->isPlaying() == true )
	{
		this->player->pause();
		playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
	}
	//***************************************************
	_percentage = float( frame ) / 100.0;
	totalTime = this->player->totalTime();
	this->player->seek( _percentage * totalTime );
	//***************************************************
}

bool myVideoPlayer::presentImage(const QImage &image)
{
	QVideoFrame frame(image);

	if (!frame.isValid())
		return false;

	QVideoSurfaceFormat currentFormat = surface->surfaceFormat();

	if (frame.pixelFormat() != currentFormat.pixelFormat()
		|| frame.size() != currentFormat.frameSize()) 
	{
		QVideoSurfaceFormat format(frame.size(), frame.pixelFormat());

		if (!surface->start(format))
			return false;
	}

	if (!surface->present(frame)) 
	{
		surface->stop();

		return false;
	} 
	else
	{
		return true;
	}
}

void myVideoPlayer::capture()
{
	emit needCapture( this );
	return;
}

float myVideoPlayer::getRate()
{
	return this->_percentage;
}

QString myVideoPlayer::getName()
{
	return this->_name;
}