#include "Diagram.h"

Diagram::Diagram(Label *label, double max_var, double min_var)
{
	_label = label;

	_max_var = max_var;
	_min_var = min_var;
}

Diagram::~Diagram(void)
{
}


void Diagram::add(string parent_name, string child_name, double parent_var, double child_var, 
				  Color co, int p_index, int c_index)
{
	pair<string, string> name;
	name.first = parent_name;
	name.second = child_name;

	_name.push_back(name);


	pair<int, int> li;
	li.first = p_index;
	li.second = c_index;
	_index.push_back(li);

	pair<double, double> var;
	var.first = parent_var;
	var.second = child_var;
	_variance.push_back(var);

	_color.push_back(co);

	//Calculate the pos of new nodes
	pair<Point, Point> pos;
	if(_pos.empty())
	{
		Point p;
		p.x = 250;
		p.y = 200;
		pos.first = p; //Parent node

		p.x += MapVarianceToDistance(child_var) + 10/2 + 1;
		pos.second = p; //Child node

		_pos.push_back(pos);
	}
	else
	{
		pair<Point, Point> pos0 = _pos.back();

		Point p;
		p.x = pos0.second.x;
		//If variance of this node is bigger, draw it upside
		if(child_var > (_variance.back()).second)
			p.y = pos0.second.y + 50;
		else 
			p.y = pos0.second.y - 50;

		pos.first = p;

		p.x += MapVarianceToDistance(child_var) + 10/2 + 1;

		pos.second = p;

		_pos.push_back(pos);
	}	

}

void Diagram::Render(QPainter *painter)
{
	for(int i=0; i<_name.size(); ++i)
	{
		//Draw node use point
		glPointSize(10.);
		glBegin(GL_POINTS);
		glColor3f(_color[i].r, _color[i].g, _color[i].b);
		glVertex2f(_pos[i].first.x, _pos[i].first.y);
		glVertex2f(_pos[i].second.x, _pos[i].second.y);
		glEnd();
		
		//Draw horizental edge between nodes
	
		glLineWidth(2);
		glBegin(GL_LINES);
		glColor3f(_color[i].r, _color[i].g, _color[i].b);
		glVertex2f(_pos[i].first.x, _pos[i].first.y);
		glVertex2f(_pos[i].second.x, _pos[i].second.y);
		glEnd();

		//Draw vertical line
		if(i > 0)
		{
			glLineWidth(3.);
			glBegin(GL_LINES);
			glColor3f(_color[i].r, _color[i].g, _color[i].b);
			glVertex2f(_pos[i-1].second.x, _pos[i-1].second.y);
			glVertex2f(_pos[i].first.x, _pos[i].first.y);
			glEnd();
		}
		//Draw labels, if the pos is out of screen, here will cause a break
		//Because the size of grid is equal to the size of screen

		//_label->Render(_name[i].first, _pos[i].first, _color[i]);
		//_label->Render(_name[i].second, _pos[i].second, _color[i]);
	}
}

float Diagram::MapVarianceToDistance(double var)
{
	double ratio = var / (_max_var - _min_var);
	float dist = 100 * ratio;
	cout<<"dist:"<<dist<<endl;
	return dist;
}

bool Diagram::CheckMouse(int x, int y, int &id)
{
	for(int i=0; i<_pos.size(); ++i)
	{
		if((x-_pos[i].second.x)*(x-_pos[i].second.x) + (y-_pos[i].second.y)*(y-_pos[i].second.y) <= 100)
		{
			id = i;
			return true;
		}
	}
	return false;
}