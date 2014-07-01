#ifndef __LABEL_H
#define __LABEL_H


#include <math.h>
#include <vector>
#include <string>
#include "Point.h"
#include "TreeRing.h"
#include <QPainter>
#include <GL/glut.h>
using namespace std;

class Label
{
public:
  Label(int w, int h, Point c) : _wnd_width(w), _wnd_height(h), compoudgraph_centre(c){};
  virtual ~Label();
  int _num_layers;

  	void Translate(float x, float y)
	{
		
		compoudgraph_centre.x += x;
		compoudgraph_centre.y += y;
	}

  Point compoudgraph_centre;

  //void Render(QPainter *painter, TreeRing *tr, int layerIndex, vector <int> nodeIndex);
  
  //void RenderHighLighted(QPainter *painter, TreeRing *tr, int layerIndex, int ni, Color co);
  void findOverLap(vector<float> &angleRecord, float angle, int j,  int size, float tSpan, float tSpan1, bool &overLap, bool &overLap1);
  float RenderHighLighted(QPainter * painter, TreeRing *tr, int layerIndex, vector <int> nodeIndex, Color color);
  //void RenderHighLighted(QPainter *painter, string name, Point pos, float angle, Color color);
  // display the labels on LayerIndex / nodeIndex

  //void Render(QPainter *painter, TreeRing *tr, int layerIndex, vector <int> nodeIndex, Color co);
  // display the labeles in the nodeIndex list in color co
  // also highlight / label those nodes pointed to

  //void Render(QPainter *painter, TreeRing *tr, int layerIndex, int ni, Color co);
  // display the labels at a single node ni in color co

  void RenderTreeRingNodeLabels(QPainter *painter, TreeRing *tr);
  
  float Render(QPainter *painter, string name, Point pos, Point posD, float angle, float ringWidth, float ringRadius, Color color, bool outest); //return label angle
  //void Render(QPainter *painter, string name, Point pos, float angle, Color color);
  //void Render(QPainter *painter, string name, Point pos, Color color);
  
  void BuildScreenGrid(TreeRing *tr);

  void DrawLine(float x1, float y1, float x2, float y2);
  float getAngle(TreeRing *tr, int layer, int id);
  int GetBitmapStringLength(string str);

  void Scale(float zoom)
  {
	 _scale = _scale*(0.4+0.6*(1+zoom));	
  }
  void setScale(float scale)
  {
	 _scale = scale;	
  }

protected:
  void FindNearestNeighborCell(int x, int y, int &ox, int &oy);
  //void text(string str, void* font, float x, float y);
   vector<Point> posRecord;
private:
	int _wnd_width, _wnd_height;
	int _cell_height, _cell_width;
	int _nr_row, _nr_col;
	int _scale;
	vector< vector<int> >_grid;

};
#endif //__LABEL_H
