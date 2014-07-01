


//#include <stdio.h>
//#include <string.h>
//#include <stdlib.h>
//#include <GL/glut.h>
//#include <GL/glui.h>
//#include <assert.h>
#include <vector>

#include "LAB2RGB.h"
#include "Point.h"

#include <GL/glut.h>

#ifndef __LABCOLORS_H
#define __LABCOLORS_H

class LABColors  
{
public:
  LABColors(double L, double r, int nc);
  virtual ~LABColors(){};

  void Render(int spx, int spy, float spacing);
  void Render_Hor(int x1, int y1, int x2, int y2, float spacing);
  std::vector<Color> &GetColor(){return _co;}
protected:
private:

  double _l, _a, _b;
  int    _nc;        // number of colors

  std::vector<Color>  _co; // colors 


};

#endif //__LABCOLORS_H

