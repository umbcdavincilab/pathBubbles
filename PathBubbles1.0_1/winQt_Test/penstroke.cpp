/*
 * stroke : 
 */

#if !defined(GLUT_WHEEL_UP)
#  define GLUT_WHEEL_UP   3
#  define GLUT_WHEEL_DOWN 4
#endif


/* includes */
#include <math.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

//#include "CompoundGraph.h"
#include "penstroke.h"
//#include "Slider.h"
//#include "Diagram.h"
//#include "Stroke.h"

#include <GL/glut.h>

#define  DIST_THRE  25
//extern CompoundGraph *cg;
//extern Slider *mySlider;
//extern GLfloat thre;

bool firstMotionLeft=true;
bool firstMotionRight=true;

/* globals */
static GLuint    stroke_lasttime;
static GLfloat   stroke_lastposition[3];

int mousex, mousey, oldMousex, oldMousey;

static GLint     stroke_button = -1;
static GLboolean stroke_tracking = GL_FALSE;
static GLboolean stroke_animate = GL_TRUE;

/* functions */
static void
_strokeAnimate(void)
{
  glutPostRedisplay();
}

void
_strokeStartMotion(int x, int y, int button, int time)
{
  assert(stroke_button != -1);
  stroke_tracking = GL_TRUE;
  stroke_lasttime = time;
}

void
_strokeStopMotion(int button, unsigned time)
{
  assert(stroke_button != -1);
  stroke_tracking = GL_FALSE;

  if (time == stroke_lasttime && stroke_animate) {
    glutIdleFunc(_strokeAnimate);
  } else {
    if (stroke_animate)
      glutIdleFunc(0);
  }
}

void
strokeAnimate(GLboolean animate)
{
  stroke_animate = animate;
}

void strokeInit(GLuint button)
{
  stroke_button = button;
  oldMousex = 0;
  oldMousey = 0;
  mousex = 0;
  mousey = 0;
}

void
strokeReshape(int width, int height)
{
  assert(stroke_button != -1);

  //stroke_width  = width;
  //stroke_height = height;
}

void
strokeMouse(int button, int state, int x, int y)
{
  assert(stroke_button != -1);

  stroke_button = button;

  if (state == GLUT_DOWN )
  {
    _strokeStartMotion(x, y, button, glutGet(GLUT_ELAPSED_TIME));
	//cg->CheckDiagramSelect(x, y);

  }
  else if (state == GLUT_UP)
  {
    _strokeStopMotion(button, glutGet(GLUT_ELAPSED_TIME));
    firstMotionLeft=true;
    firstMotionRight=true;
	if(button == GLUT_WHEEL_UP)
	{
		//cg->Zoom(0.1);
	}
	else if(button == GLUT_WHEEL_DOWN)
	{
		//cg->Zoom(-0.1);
	}
  }
}


float dist(int x1, int y1, int x2, int y2)
{
  return sqrt(((float)x1-(float)x2)*((float)x1-(float)x2) + ((float)y1-(float)y2)*((float)y1-(float)y2));
}


void strokeMotionLeft(int x, int y)
{
  //cg->UpdateHighlight(&firstMotionLeft, x, y); //If set the firstMotionLeft = false here
  //mySlider->UpdateHighlight(&firstMotionLeft, x, y); //Wrong
  firstMotionLeft = false;
}

void strokeMotionRight(int x, int y)
{
int ox = 0, oy = 0;
  if(firstMotionRight)  // init the mouse locations
  {
    oldMousex = x;
	oldMousey = y;
	mousex = x;
	mousey = y;
	firstMotionRight=false;
  }

  mousex = x;
  mousey = y;

  if (stroke_tracking == GL_FALSE)
    return;


  ox = mousex - oldMousex;
  oy = mousey - oldMousey;

  //if(dist(oldMousex, oldMousey, mousex, mousey) >thre)
//   if(dist(oldMousex, oldMousey, mousex, mousey) >DIST_THRE)
//   {
    oldMousex = mousex;
    oldMousey = mousey;
    //curve1.add(mousex,windowHeight-mousey);
  //}

	//cg->Pan(ox, oy);

}

void strokeMotion(int x, int y)
{
  assert(stroke_button != -1);
  if(stroke_button == GLUT_RIGHT_BUTTON)
  {
    strokeMotionRight(x, y);
  }
  else
  {
    strokeMotionLeft(x, y);
  }

  /* remember to draw new position */
  glutPostRedisplay();
}
