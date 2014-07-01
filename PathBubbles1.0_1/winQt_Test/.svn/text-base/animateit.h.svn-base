#ifndef ANIMATEIT_H
#define ANIMATEIT_H

#include <QPoint>
#include <QTime>

class animateIt
{
public:
    animateIt( int duration);
	virtual ~animateIt(){};//J.C.

    void start( QPoint Start, QPoint Distination, int size1, int size2);
    bool isAnimated();
    QPoint animateP();
	float animatePercentage();
    int animateS();
private:
    int duration, cur_size;
    QTime cur_time;
    float stepX, stepY, stepS;
    QPoint Start;
	QPoint _start;
	QPoint _distination;
};

#endif // ANIMATEIT_H
