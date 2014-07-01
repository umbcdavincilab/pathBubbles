#include "animateit.h"

animateIt::animateIt(int Duration)
{
    duration = Duration;
}

void animateIt::start( QPoint start, QPoint Distination, int Cur_size, int dis_size)
{
	this->_start = start;
	this->_distination = Distination;
    cur_time = QTime::currentTime();
    cur_size = Cur_size; 
    Start = start;
    stepX = (float)(Distination.x() - start.x())/(float)duration;
    stepY = (float)(Distination.y() - start.y())/(float)duration;
    stepS = (float)(dis_size - cur_size)/(float)duration;
}

bool animateIt::isAnimated()
{
    int time_past = cur_time.msecsTo(QTime::currentTime());
    if(  time_past > duration * 1.1 )
    {
        return false;
    }
    else
    {
        return true;
    }
}

float animateIt::animatePercentage()
{
	int time_past = cur_time.msecsTo(QTime::currentTime());
	return (float)time_past/(float)duration;
}

int animateIt::animateS()
{
    int time_past = cur_time.msecsTo(QTime::currentTime());
    return cur_size + time_past * stepS;
}

QPoint animateIt::animateP()
{
    int time_past = cur_time.msecsTo(QTime::currentTime());
    //float percentage = (float)time_past / (float)duration;
	if( time_past >= this->duration )
	{
		return this->_distination;
	}
	else
	{
		return QPoint( Start.x() + time_past * stepX, Start.y() + time_past * stepY);
	}
}
/*
    int dt = m_startTime.msecsTo(QTime::currentTime());
    if (dt < 500)
        glScalef(dt / 500.0f, dt / 500.0f, dt / 500.0f);
  */
