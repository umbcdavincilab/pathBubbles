#ifndef VIDEO_H
#define VIDEO_H

#include "VideoWidget.h"
#include <QtGui/QWidget>
class QAbstractVideoSurface;
class VideoWidget;

class Video : public QWidget
{
	Q_OBJECT
public:
	Video(QWidget *parent = 0);
	~Video();
	QAbstractVideoSurface *videoSurface() const { return surface; }
	QSize sizeHint() const;
protected:
	void paintEvent(QPaintEvent *event);
	void resizeEvent(QResizeEvent *event);
private:
	VideoWidget *surface;
};

#endif