#ifndef TRACKBALL_H
#define TRACKBALL_H

//#include <G3D/G3DAll.h>
//#include <GLG3D/GLG3D.h>
#include "myVector.h"

#define PI 3.1415926

class TrackBall
{
public:
	TrackBall();
	TrackBall( int step, int R, myVector3 origan);
	void setStep( int step );
	void setR( int r )
	{
		R = r;
	}
	float getR()
	{
		return R;
	}
	myVector3 getOrigin();
	myVector3 getStart();
	myVector3 left();
	myVector3 right();
	myVector3 up();
	myVector3 down();
	myVector3 upper();
	void setXY( float x, float y );
	myVector3 fromXY();
private:
	float R;
	int Step;
	int angleX;
	int angleY;
	myVector3 Origin;
	myVector3 curPos;
	myVector3 calculatePos();
	void formalizeAngles();
	float Height();
};

#endif