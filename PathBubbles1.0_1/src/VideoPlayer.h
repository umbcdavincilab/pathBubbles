#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QtGui/QMovie>
#include <QtGui/QWidget>
#include <QString>
#include "qtbat.h"
#include "mySlider.h"
#include <QFile>
#include <Phonon/VideoPlayer>
#include <Phonon/MediaSource>

class QAbstractButton;
class QAbstractVideoSurface;
class QSlider;

class myVideoPlayer : public QWidget
{
	Q_OBJECT
public:
	myVideoPlayer( QWidget *parent = 0);
	myVideoPlayer( QWidget *parent, QString name, float begin = 0, float end = 0 );
	void openPath( QString path );
	float getRate();
	QString getName();
	void acceptBat( ItemBase* bat );
	ItemBase* getBat()
	{
		return _bat;
	}
	QRectF getNameRect()
	{
		return QRectF(nameLabel->pos().x()-nameLabel->width()/2, nameLabel->pos().y(), nameLabel->width(), nameLabel->height());
	}
	~myVideoPlayer();
signals:
	void needCapture( myVideoPlayer* );
	void addBatWithVideo( );
	void logPause();
	void logPlay();

public slots:
	void capture();
	void openFile();
	void play();
private slots:
	void updateFrame();
	void setPosition(int frame);
private:
	void initUI();
	QtBat* _bat;
	bool BATADDED;
	QString _name;
	Phonon::VideoPlayer *player;
	bool presentImage(const QImage &image);
	QAbstractVideoSurface *surface;
	QAbstractButton *playButton;
	QAbstractButton *captureButton;
	mySlider *positionSlider;
	QLabel *nameLabel;
	QTimer* timer;
	float _percentage;
	float _begin, _end;
	qint64 totalTime;
};

#endif
