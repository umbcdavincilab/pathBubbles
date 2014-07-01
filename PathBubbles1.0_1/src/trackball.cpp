#include "trackball.h"
#include <math.h>

TrackBall::TrackBall(int step, int r, myVector3 origin)
{
	Step = step;
	Origin = origin;
	angleX = 0;
	angleY = 0;;
	R = r;
}

TrackBall::TrackBall()
{
	//TrackBall( 1, G3D::Vector3(), G3D::Vector3(10, 0, 0));
	Step = 1;
	//Origin.x = Origin.y = Origin.z = 0;
	angleX = 0;
	angleY = 0;
	R = 5;
}

void TrackBall::setStep(int step)
{
	Step = step;
}

myVector3 TrackBall::getOrigin()
{
	return Origin;
}

myVector3 TrackBall::getStart()
{
	return calculatePos();
	//return Start;
}

myVector3 TrackBall::up()
{
	angleX += Step;
	curPos = calculatePos();
	return curPos;
}

myVector3 TrackBall::left()
{
	angleY -= Step;
	curPos = calculatePos();
	return curPos;
}

myVector3 TrackBall::down()
{
	angleX -= Step;
	curPos = calculatePos();
	return curPos;
}

myVector3 TrackBall::right()
{
	angleY += Step;
	curPos = calculatePos();
	return curPos;
}

myVector3 TrackBall::calculatePos()
{
	formalizeAngles();
	myVector3 result;
	float offY = R * sin( (float)angleX*PI/180.0 );
	float projection = R * cos( (float)angleX*PI/180.0 );
	float offZ = - projection * sin( (float)angleY*PI/180.0 );
	float offX = projection * cos( (float)angleY * PI/180.0 );
	result.x = offX;
	result.y = offY;
	result.z = offZ;
	return result;
}

void TrackBall::formalizeAngles()
{
	if( angleX < 0 )
	{
		angleX = 360 + angleX;
	}
	if(angleY < 0 )
	{
		angleY = 360 + angleY;
	}
	angleX = angleX%360;
	angleY = angleY%360;
}

myVector3 TrackBall::upper()
{
	myVector3 result;
	float length = abs(Height());
	myVector3 unitZ( 0, length, 0);
	result.x = unitZ.x - curPos.x;
	result.y = unitZ.y - curPos.y;
	result.z = unitZ.z - curPos.z;
	return result;
	//if(angleX > 90 && angleX <= 270)
	//{
	//	return G3D::Vector3( 0, -1, 0);
	//}
	//else
	//{
	//	return G3D::Vector3( 0, 1, 0);
	//}
	//if( curPos.x < 0 )
	//{
	//	return G3D::Vector3( 0, -1, 0);
	//}
	//else
	//{
		//return G3D::Vector3( 0,  1, 0);
	//}
}

float TrackBall::Height()
{
	float angle;
	if( angleX < 90 && angleX >= 0)
	{
		angle = angleX;
	}
	else if( angleX < 180 && angleX >= 90)
	{
		angle = 180 - angleX;
	}
	else if( angleX < 270 && angleX > 180)
	{
		angle = angleX -180;
	}
	else
	{
		angle = 360 - angleX;
	}
	float result = R / cos((float)angle * PI / 180.0 );
	if(angleX > 180 && angleX < 360)
	{
		result = -result;
	}
	return result;
}

void TrackBall::setXY( float x, float y )
{
	angleX += (int)(x + 0.5);
	if( angleX > -80 && angleX < 80)
	{
	}
	else
	{
		angleX -= (int)(x + 0.5);
	}
	angleY += (int)(y + 0.5);
	angleY = angleY%360;
}

myVector3 TrackBall::fromXY()
{
	curPos = calculatePos();
	return curPos;
}