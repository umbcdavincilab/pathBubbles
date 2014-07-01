

#ifndef __POINT_H__
#define __POINT_H__

#include <math.h>

#define drawOneLine(x1,y1,x2,y2)  glBegin(GL_LINES);  \
	glVertex2f ((x1),(y1)); glVertex2f ((x2),(y2)); glEnd();
//points must be diagonal from each other
#define drawBox(x1,y1,x2,y2)  glBegin(GL_LINE_LOOP);  \
	glVertex2f ((x1),(y1)); glVertex2f((x2), (y1)); \
	glVertex2f ((x2),(y2)); glVertex2f ((x1),(y2)); glEnd();
#define drawTri(x1,y1,x2,y2)  glBegin(GL_LINE_LOOP);  \
	glVertex2f ((x1),(y1)); \
	glVertex2f((x2)-(((y2)- (y1))*0.3), (y2)); \
	glVertex2f((x2)+(((y2)- (y1))*0.3), (y2)); \
	glEnd();

struct Point
{
	float x,y;

	Point() { };
	Point(float x,float y) { this->x = x; this->y = y; }
	Point operator+(const Point & v) const { return Point(x+v.x,y+v.y); }
	Point operator-(const Point & v) const { return Point(x-v.x,y-v.y); }
	Point operator*(float v) const { return Point(x*v,y*v); }
	Point operator/(float v) const { return Point(x/v,y/v); }

	Point & operator=(const Point & v) { x = v.x;y=v.y; return *this; }
	Point & operator+=(const Point & v) { x += v.x;y+=v.y; return *this; }

	double dis(const Point & v) { sqrt ((v.x -x) * (v.x-x) + (v.y-y) * (v.y-y)); }
};

// look up table 
struct Lookup
{
	int x, y;
	Lookup() {};
	Lookup(int x, int y) {this->x=x; this->y=y;}
};

struct Color
{
	float r, g, b, a;
	Color() {};
	Color(float r, float g, float b)
	{this->r=r; this->g=g; this->b=b;this->a=1.0;}
	Color(float r, float g, float b, float a)
	{this->r=r; this->g=g; this->b=b;this->a=a;}

	Color & operator=(const Color & c) { r=c.r; g=c.g; b=c.b; a=c.a; return *this; }
	Color operator+(const Color &c) const {return Color(r+c.r, g+c.g, b+c.b, a);}
	Color operator*(float v) const {return Color(r*v, g*v, b*v, a);}
};

#endif
