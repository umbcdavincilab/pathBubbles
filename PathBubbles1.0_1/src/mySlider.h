#ifndef MYSLIDER_H
#define MYSLIDER_H

#include <QSlider>
#include <QPaintEvent>
#include <QRectF>
#include <QPainter>

class mySlider : public QSlider
{
public:
	mySlider( QWidget* parent, float begin = 0, float end = 0);
	void paintEvent( QPaintEvent * ev );
	void setHighlight( float begin, float end, int total, int stFrame, int edFrame );
private:
	float _start, _end;
	QString min;
	QString max;
};

#endif