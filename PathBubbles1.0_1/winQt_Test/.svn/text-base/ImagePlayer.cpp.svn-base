#include "ImagePlayer.h"

#include "ImageWidget.h"

#include <QtMultimedia>

ImagePlayer::ImagePlayer(QWidget *parent, QString name )
    : QWidget(parent)
    , surface(0)
    , playButton(0)
    , positionSlider(0)
{
    connect(&movie, SIGNAL(stateChanged(QMovie::MovieState)),
            this, SLOT(movieStateChanged(QMovie::MovieState)));
    connect(&movie, SIGNAL(frameChanged(int)),
            this, SLOT(frameChanged(int)));

    ImageWidget *imageWidget = new ImageWidget( parent );
    surface = imageWidget->videoSurface();

    playButton = new QPushButton;
    playButton->setEnabled(false);
    playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

    connect(playButton, SIGNAL(clicked()),
            this, SLOT(play()));

    positionSlider = new QSlider(Qt::Horizontal);
    positionSlider->setRange(0, 0);

    connect(positionSlider, SIGNAL(sliderMoved(int)),
            this, SLOT(setPosition(int)));

    connect(&movie, SIGNAL(frameChanged(int)),
            positionSlider, SLOT(setValue(int)));

    QBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->setMargin(0);
    //controlLayout->addWidget(openButton);
    controlLayout->addWidget(playButton);
    controlLayout->addWidget(positionSlider);

    QBoxLayout *layout = new QVBoxLayout;
    layout->addWidget( imageWidget );
    layout->addLayout( controlLayout );

    setLayout(layout);
	//
	_name = name;
	openFile();
}

ImagePlayer::~ImagePlayer()
{
}

void ImagePlayer::openFile()
{
    //QString fileName = QFileDialog::getOpenFileName(this, tr("Open Movie"));

    if (!_name.isEmpty()) {
        surface->stop();

        movie.setFileName(_name);

        playButton->setEnabled(true);
        positionSlider->setMaximum(movie.frameCount());

        movie.jumpToFrame(10);
    }
}

void ImagePlayer::play()
{
	int curIndex;
    switch(movie.state()) {
    case QMovie::NotRunning:
		curIndex = movie.currentFrameNumber();
        movie.start();
        break;
    case QMovie::Paused:
        movie.setPaused(false);
        break;
    case QMovie::Running:
        movie.setPaused(true);
        break;
    }
}

void ImagePlayer::movieStateChanged(QMovie::MovieState state)
{
    switch(state) {
    case QMovie::NotRunning:
    case QMovie::Paused:
        playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        break;
    case QMovie::Running:
        playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        break;
    }
}

void ImagePlayer::frameChanged(int frame)
{
    if (!presentImage(movie.currentImage())) {
        movie.stop();
        playButton->setEnabled(false);
        positionSlider->setMaximum(0);
    } else {
        positionSlider->setValue(frame);
    }
}

void ImagePlayer::setPosition(int frame)
{
    bool result = movie.jumpToFrame(frame);
	if( result )
	{
		return;
	}
	else
	{
		//			errers
		return;
	}
}

bool ImagePlayer::presentImage(const QImage &image)
{
    QVideoFrame frame(image);

    if (!frame.isValid())
        return false;

    QVideoSurfaceFormat currentFormat = surface->surfaceFormat();

    if (frame.pixelFormat() != currentFormat.pixelFormat()
            || frame.size() != currentFormat.frameSize()) {
        QVideoSurfaceFormat format(frame.size(), frame.pixelFormat());

        if (!surface->start(format))
            return false;
    }

    if (!surface->present(frame)) {
        surface->stop();

        return false;
    } else {
        return true;
    }
}

void ImagePlayer::setFrameByRate( float rate )
{
	int frame = movie.frameCount();
	int cur = frame * rate;
	this->setPosition( cur );
	this->positionSlider->setValue( movie.frameCount() * rate );
	return;
}