#include <math.h>
#include <vector>
#include "Point.h"
#include <QPainter>
#include <GL/glut.h>


#ifndef __STROKE_H
#define __STROKE_H

using namespace std;

#define M_PI 3.1415926
#define NUM_SLICES 20
#define CLL  0
#define CP  1

typedef enum { CUBIC_BSPLINE, FOUR_POINT } CurveType;
#define DEFAULT_R  253
#define DEFAULT_G  200
#define DEFAULT_B  100
#define DEFAULT_A  120


class Stroke
{
private:
  vector<Point> control;
  vector<Point> * limit;
  vector<Point> * temp;
  vector<Color> interpolated_color;


  
  int numLevels;

  bool EnableLineColor;
  Color color;
  //Color highlight_color;
  bool if_highlight;
public:
	bool computed;
  Stroke();
  virtual ~Stroke();

  void SetStrokeColor(float r, float g, float b, float a)
  {
     color.r=r; color.g=g; color.b=b; color.a=a;
  }; 

  void SetHighlight(bool b)
  {
	  if_highlight = b;
  }

  /*void SetHighlightColor(float r, float g, float b, float a)
  {
	  highlight_color.r = r;
	  highlight_color.g = g;
	  highlight_color.b = b;
	  highlight_color.a = a;
  }*/

  int GetNrControlPoints(){return control.size();}

  void UpdateControl(int index, Point newp)
  {
    control[index] = newp;
	computed = false;
  }

 
  void TranslateControl(Point p)
  {
	  for(int i=0; i<control.size(); ++i)
	  {
		  control[i].x += p.x;
		  control[i].y += p.y;
	  }
	  computed = false;
  }

  
  void Scale(float scale)
  {
	  for(int i=0; i<control.size(); ++i)
	  {
		  control[i].x *= scale;
		  control[i].y *= scale;
	  }
	  computed = false;
  }

  int highlightIndex, highlightCurveIndex;
  float z;
  float radius;
	float alpha;//Control the stroke's transparency
  bool useTexture;
  float ufreq;
  float vfreq;
  float ustart;
  float vstart;

  CurveType curveType;


  void add(float x, float y);
  void clear();
 // static 
  void drawLines(QPainter *painter, vector<Point> * curve);
  void drawLines(vector<Point> * curve, Color newcolor);
 // static 
  void drawPoints(vector<Point> * curve);
  void drawPoints(vector<Point> * curve, vector <Color> dc);
  void drawPoints(vector<Point> * curve, vector <Color> dc, vector <int> nc);

  void forceRecompute();
  void discPoint(float x,float y,float brushRadius);
  void drawCap(const Point & p0, float dx, float dy,float texU,float texV);
  void drawThickCurve(vector<Point> * curve, float radius,bool cap=true);
  void drawControl(QPainter *painter, int primType);
  void drawControl(QPainter *painter, int primType, vector <Color> dc);
  void drawControl(QPainter *painter, int primType, vector <Color> dc, vector <int> nc);
  void drawLineCurve(QPainter *painter);
  void render(QPainter *painter);
  void render(Color newcolor);
  void subdivideCubicBSpline(vector<Point> * inputCurve, 
			     vector<Point> * outputCurve);
  void subdivideFourPoint(vector<Point> * inputCurve, 
			  vector<Point> * outputCurve);
  void subdivide(vector<Point> * inputCurve, 
		 vector<Point> * outputCurve);
  void computeLimitCurve();
  void TranslateLimit(Point p);  

  void InterpolateColor();

  void printCtrlPoints(); // print all control points
  void updateCtrlPoints(int pi, float x, float y);

  // for selection 
  void getClosest(float thre, int mx, int my);
  void getPointByIndex(int idx, float *x, float *y);

/*
  int findClosest() 
  {
    if(highlightIndex!=-1) return 1;
	else if(highlightCurveIndex!=-1) return 2;
	return 0;
  }
  */

  void setEnableLineColor(bool mode)
  {
    EnableLineColor = mode;
    //forceRecompute();
  }

};


#endif
