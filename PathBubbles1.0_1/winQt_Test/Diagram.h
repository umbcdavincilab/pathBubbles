#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <QPainter>
#include "Point.h"
#include "Label.h"
#include "SimData.h"
using namespace std;

class Diagram
{
public:
	//Diagram(Label *label, SimData *simdata, double max_var, double min_var);
    Diagram(Label *label, double max_var, double min_var);
	
	~Diagram(void);
	void add(string parent_name, string child_name, double parent_var, double child_var, 
		     Color co, int p_index, int c_index);
	void Render(QPainter *painter);
	bool CheckMouse(int x, int y, int &id);

	int GetParentIndex(int id){return _index[id].first;}
	int GetChildIndex(int id){return _index[id].second;}
protected:
	float MapVarianceToDistance(double var);
private:
	vector< pair<string, string> > _name;
	vector< pair<int, int> > _index; //index in the origin treering
	vector<Color> _color;
	vector< pair<Point, Point> > _pos;
	vector< pair<double, double> > _variance;
	vector<double> _correlation;  //Store the correlation between two node
	double _max_var, _min_var;
	Label *_label;
	SimData *_sim_data;
	float _point_size; //node size in this diagram
};
