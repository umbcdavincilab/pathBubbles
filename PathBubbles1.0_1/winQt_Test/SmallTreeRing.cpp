#include "SmallTreeRing.h"
#include <QPainter>
//extern Point compoudgraph_centre;

SmallTreeRing::SmallTreeRing(vector<float> &node_angle, vector<Color> &node_color, vector<string> &node_name,
							 Stroke stroke, float radius, float width, float scale, Point centre, int num_layers)
{
	_node_color = node_color;
	_node_angle = node_angle;
	_node_name = node_name;
	_stroke = stroke; //The coordinate of this stroke is calculated in the big tree ring, so need to translate
	_centre = centre;
	_num_layers = num_layers;
	_node_num = node_angle.size();
	_scale = scale;
	_width = width;
	_radius[0] = radius;
	_radius[1] = radius - width;
	_radius[2] = _radius[1] - width;
	CalculateNodePos();
}


SmallTreeRing::~SmallTreeRing(void)
{
}

void SmallTreeRing::CalculateNodePos()
{
	Point p;
	for(int i=0; i<_num_layers; ++i)
	{
		p.x = _radius[i + abs(_num_layers - 3)] * cosf(_node_angle[i]);
		p.y = _radius[i + abs(_num_layers - 3)] * sinf(_node_angle[i]);
		_node_pos.push_back(p);
	}

	for(int i=_num_layers; i<_node_num; ++i)
	{
		p.x = _radius[2] * cosf(_node_angle[i]);
		p.y = _radius[2] * sinf(_node_angle[i]);
		_node_pos.push_back(p);
	}

	//TranslateStroke(); //Translate stroke to origin
	Scale();          //Then scale both node pos and stroke control pos
	Translate();     //Tanslate all of them to small tree ring center

	//Calculate curve control point
	/*vector<Point>::iterator iter = _node_pos.end();
	_stroke.add((iter-2)->x, (iter-2)->y);
	_stroke.add(_centre.x, _centre.y);
	_stroke.add((iter-1)->x, (iter-1)->y);
	_stroke.printCtrlPoints();*/	
}

void SmallTreeRing::Translate()
{
	for(int i=0; i<_node_num; ++i)
	{
		_node_pos[i] += _centre;
	}
	//_stroke.TranslateControl(_centre);
}

void SmallTreeRing::Scale()
{
	for(int i=0; i<_node_num; ++i)
	{
		_node_pos[i]  = _node_pos[i] * _scale;
	}
	_radius[0] *= _scale;
	_radius[1] *= _scale;
	_radius[2] *= _scale;
	_width *= _scale;
	//_stroke.Scale(_scale);
}

void SmallTreeRing::Render(QPainter *painter)
{
	DrawCircle();

	glPointSize(8);
	glBegin(GL_POINTS);
	for(int i=0; i<_node_num; ++i)
	{
		glColor3f(_node_color[i].r, _node_color[i].g, _node_color[i].b);
		glVertex3f(_node_pos[i].x, _node_pos[i].y, 0);
	}; // end for(i)
	glEnd();

	//Render edge
	glLineWidth(1);
	glBegin(GL_LINES);
	for(int i=0/*_num_layers-1*/; i<_node_num-1; ++i)
	{
		glColor3ub(40,100,20);
		glVertex3f(_node_pos[i].x, _node_pos[i].y, 0);
		glVertex3f(_node_pos[i+1].x, _node_pos[i+1].y, 0);
	}
	glEnd();

	glLineWidth(1.);
	//_stroke.render();

	//Render label
	void *font = GLUT_BITMAP_HELVETICA_10;
	for(int i=0; i<_node_num; ++i)
	{
		char *str = new char[_node_name[i].length() + 2];
		int si;
		for(si=0; si<_node_name[i].length(); si++)
		{
			str[si]=_node_name[i][si];
		}
		str[si]='\0';

		glColor3f(0.1,0.1,1);
		char* p; 
		glRasterPos2f(_node_pos[i].x+4, _node_pos[i].y);
		glDisable(GL_TEXTURE_2D);
		for (p = str; *p; p++)
			glutBitmapCharacter(font, *p);
	}

}

void SmallTreeRing::DrawCircle()
{
	int n = 100;

	for(int i=0; i<3; ++i)
	{
		//  		float radius = sqrtf( (_node_pos[i].x - _centre.x) * (_node_pos[i].x - _centre.x) 
		//  							  + (_node_pos[i].y - _centre.y) * (_node_pos[i].y - _centre.y));

		for(int j=0; j<n; ++j)
		{
			glLineWidth(1);
			glBegin(GL_LINE_LOOP);
			glColor4ub(71, 116, 175, 128);
			glVertex2f(_centre.x + _radius[i]*cosf(2*M_PI/n*j), _centre.y + _radius[i]*sinf(2*M_PI/n*j));
		}
		glEnd();
	}

}

void SmallTreeRing::Translate(Point p)
{
	Point p_;
	p_.x = p.x - _centre.x;
	p_.y = p.y - _centre.y;
	for(int i=0; i<_node_num; ++i)
	{
		_node_pos[i] += p_;
	}
	_centre.x = p.x;
	_centre.y = p.y;
	_stroke.TranslateControl(p_);
}

//Translate stroke to (0,0)
void SmallTreeRing::TranslateStroke()
{
	Point p;
	p.x = - compoudgraph_centre.x;
	p.y = - compoudgraph_centre.y;
	_stroke.TranslateControl(p);
}