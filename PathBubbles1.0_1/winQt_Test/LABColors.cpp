
//#include <stdio.h>
//#include <string.h>
//#include <stdlib.h>

//#include <GL/glui.h>
//#include <assert.h>
//#include <iostream>
#include "LABColors.h"
#include "Stroke.h"

LABColors::LABColors(double L, double r, int nc)
{
  double unit_angle = 2. * M_PI / (double)nc;
  //cerr << "Unit_angle: " << unit_angle << endl;

  _nc = nc;
  for(int i=0; i<nc; i++)
  {
    float RGB[3];    
	Color rgb;

	float a = r * cos(i*unit_angle);
	float b = r * sin(i*unit_angle);
	
	LAB2RGB(RGB, L,a,b);
	rgb = Color(RGB[0], RGB[1], RGB[2]);
	_co.push_back(rgb);

	//cerr << "push color: " << RGB[0]<<" "<<RGB[1]<<" "<<RGB[2]<<endl;
  }; // end for(i)
}


void LABColors::Render(int spx, int spy, float spacing)  // pixel starting / ending point / pixel size
{
  for(int i=0; i<_nc; i++)
  {
    glColor3f(_co[i].r, _co[i].g, _co[i].b);
	drawBox(spx, spy+i*spacing,  spx+10, spy+(i+1)*spacing);
  }; // end for(i)
}

void LABColors::Render_Hor(int x1, int y1, int x2, int y2, float spacing)
{
	for(int i=0; i<_nc; ++i)
	{
		glColor3f(_co[i].r, _co[i].g, _co[i].b);
		drawBox(x1+i*spacing, y1, x1+(i+1)*spacing, y2);
	}
}
