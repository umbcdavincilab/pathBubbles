#ifndef IMAGEPLAYER_H
#define IMAGEPLAYER_H

#include <QtGui/QMovie>
#include <QtGui/QWidget>

class QAbstractButton;
class QAbstractVideoSurface;
class QSlider;

class ImagePlayer : public QWidget
{
    Q_OBJECT
public:
    ImagePlayer(QWidget *parent = 0, QString name = 0 );
	void setFrameByRate( float rate );
    ~ImagePlayer();
public slots:
    void openFile();
    void play();
private slots:
    void movieStateChanged(QMovie::MovieState state);
    void frameChanged(int frame);
    void setPosition(int frame);
private:
    bool presentImage(const QImage &image);
    QMovie movie;
    QAbstractVideoSurface *surface;
    QAbstractButton *playButton;
    QSlider *positionSlider;
	QString _name;
};

#endif
