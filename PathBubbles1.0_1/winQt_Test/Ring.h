



//#include <iostream>
//#include <fstream>
//#include <assert.h>
#include <vector>
#include <QString>
//#include <stdlib.h>
//#include <string.h>
//#include <math.h>
#include "Point.h"

#include <GL/glut.h>

#ifndef __RING_H
#define __RING_H

using namespace std;

// just holding the ring geometry and label information
struct Ring
{
public:
  Ring();
  virtual ~Ring()
  {}

  Ring & operator=(const Ring & r)
  {
		this->_node_pathID = r._node_pathID;
		this->_node_name = r._node_name;
		this->_node_size= r._node_size;
		this->_node_matched = r._node_matched ;
		this->_color_by_orthology = r._color_by_orthology;
		this->_color_by_expression= r._color_by_expression;
		this->_color_by_crosstalk= r._color_by_crosstalk;
		this->_node_expression= r._node_expression;
		this->_node_crosstalk= r._node_crosstalk;

		this->_color_by_cv = r._color_by_cv;
		this->_color_by_domain = r._color_by_domain;
		this->NodeColorMode = r.NodeColorMode;
		this->_radius = r._radius;
		this->_radial_pos = r._radial_pos;
		this->_centre_pos = r._centre_pos;
		this->_limits = r._limits;
		this->_max_variance_child = r._max_variance_child;
		this->_max_cv_child = r._max_cv_child;
		this->_node_num = r._node_num;
		this->_node_variance = r._node_variance;
		this->_node_orthRate = r._node_orthRate;
		this->_node_cv = r._node_cv;
		this->_node_domain = r._node_domain;
		this->_radial_radius = r._radial_radius;
		this->_nr_children = r._nr_children;
		this->_angle =r._angle;
		this->_child_range = r._child_range;
		this->_bun_pos = r._bun_pos;
		this->_ring_radial_radius = r._ring_radial_radius;
		this->_ring_radius = r._ring_radius;
		return *this;
  }

  void  UpdateNodePos();
  void  SetNodeColorMode(bool nm);
  // set the new Color mode

  void Render(vector <Color> dc, vector <int> nc);
  void Render(vector <Color> dc);


  vector <string> _node_name;
  vector <QString> _node_pathID;
  // name of the node
  vector <int>    _node_size;
  // repetition for each node
  vector<Point> _radial_pos;
  vector< vector<Point> > _pos;
  vector<Point> _centre_pos;
  vector<Point> _bun_pos;
  vector<int> _nr_children; //Record the number of children of each node on this ring
  vector<float> _angle; //Angle of each node on the ring
  vector<float> _radius;
  vector<float> _radial_radius;
  vector<double> _node_variance;
  vector<double> _node_orthRate;
  vector<double> _node_crossTalk;
  vector<double> _node_expression;
  vector<int> _node_crosstalk;
  vector<double>_node_cv;
  vector<double> _node_domain;
 
  vector<int> _max_variance_child;
  vector<int> _max_cv_child;

  vector< pair<float, float> > _limits; //Include bisector limits and tangent limits, this is the angle range of the children of each node.
  vector< pair<int, int> > _child_range; //Children id from first to second
  vector< int > _parent;

  // position them on a ring
  int             _node_count;
  // total number of node - this is 
  // the sum counting repetitations

  vector <int>  _node_matched; //record if a node is matched by expression data
  vector <Color>  _color_by_expression;
  vector <Color>  _color_by_orthology;
  vector <Color>  _color_by_crosstalk;
  vector<Color> _color_by_cv;
  vector<Color> _color_by_domain;
  vector<int> _rateLimitNum;

  vector<Color>::iterator _node_color_iter;
  vector<int>::iterator _node_matched_iter;
  // node colors

  int _node_num;
  bool NodeColorMode;

  float _ring_radius;
  float _ring_radial_radius;
};

#endif //__RING_H
