#pragma once
#include <iostream>
#include <vector>
#include "Point.h"
#include "Stroke.h"
#include <QPainter>
#include <gl/glut.h>

using namespace std;

class SmallTreeRing
{
public:
	SmallTreeRing(vector<float> &node_angle, vector<Color> &node_color, vector<string> &node_name,
			      Stroke stroke, float radius, float width, float scale, Point centre, int num_layers);
	~SmallTreeRing(void);

	void Render(QPainter *painter);
	void CalculateNodePos();
	void Translate();
	void Translate(Point p);
	void Scale();
	void DrawCircle();
	void SetCentre(Point centre){_centre = centre;}
	Point &GetCentre(){return _centre;}
	void TranslateStroke();

	Point compoudgraph_centre;
private:
	vector<Point> _node_pos;
	vector<Color> _node_color;
	vector<string> _node_name;
	vector<float> _node_angle;
	float _radius[3];
	float _scale;
	float _width;
	Point _centre;
	Stroke _stroke;
	int _node_num;
	int _num_layers;
};
