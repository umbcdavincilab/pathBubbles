

#include "Label.h"
#include "Point.h" // for color

#include <iostream>
using namespace std;

#define HIGHLIGHTCOLOR1 QColor(255, 255, 255, 255) //for seg or node representative being dragging around/ and being mouse over

//extern int windowHeight;
//extern Point compoudgraph_centre;

Label::~Label()
{
}


float Label::RenderHighLighted(QPainter * painter, TreeRing *tr, int layerIndex, vector <int> nodeIndex, Color co)
{ //used
  // get the node position on screen
	if(tr->_ring[layerIndex]._radius.size()<=0)
		return 0;

	float Rds =  tr->_ring[layerIndex]._radius[0];//tr->GetRadius(); //changed when switching innder ring out
	float wdh =  tr-> GetRingWidth();//tr->getWidth();
	QFont f("Arial",10);
	QRect rect;
	QFontMetrics fontMetrics(f);		
	rect = fontMetrics.boundingRect("A"); 
	float w, tSpan, da;
	float firstNodeAngle=-1000;
	
	w=rect.height();
	da = 1-w*w/(2*Rds*Rds);
	da = acos(da)/2;	

	w=w*0.8;
	tSpan= 1-w*w/(2*Rds*Rds);
	tSpan = acos(tSpan); //smallest allowed angle span between labels to avoid overlapping
	
	float Rds1=Rds+wdh;
	float tSpan1= 1-w*w/(2*Rds1*Rds1);
	tSpan1 = acos(tSpan1); //smallest allowed angle span between labels to avoid overlapping
	
	int size = nodeIndex.size();
	vector<float> angleRecord;		
	angleRecord.resize(size);

		
	painter->setPen(QColor(co.r, co.g, co.b, 255));	

	for(int ni = 0; ni < nodeIndex.size(); ni++)
	{

		int preID=0;
		bool overLap=false, overLap1=false;// overLap2=false;
		float angle = getAngle(tr, layerIndex, nodeIndex[ni]), Angle;	
		Angle = angle;		
		Point p = ((tr->_ring)[layerIndex]._centre_pos)[nodeIndex[ni]];
		
		findOverLap(angleRecord, angle, ni, size, tSpan, tSpan1, overLap, overLap1);

		if(overLap1 && ni!=0)
		{	
			preID = ni-1;			 
		}
		angleRecord[ni]=angle;

		float dw = (tr->_ring[tr->_ring.size()-1]._radius[0] - tr->_ring[0]._radius[0]);
		float dx, dy, dx2, dy2;
		dx2 = dw * cos(angle);
		dy2 = dw * sin(angle);

		if( overLap )
		{
			float dx, dy;
			dx = wdh * cos(angle);
			dy = wdh * sin(angle);
			Point op1,op2;
				
			p.x = p.x - dx; 
			p.y = p.y - dy;

			op1 = p;			

			op2= p;
			op1.x = op1.x + dx/2;
			op1.y = op1.y + dy/2;

			op2.x = op2.x + dx/2;
			op2.y = op2.y + dy/2;
				
			painter->drawLine(op1.x+dx2, op1.y+dy2, op2.x+dx2, op2.y+dy2);				
				
			p= op2;
			p.x = p.x + dx; 
			p.y = p.y + dy;
		}
		else if( overLap1 )
		{
			QRect rect;			
			float wdh1 = wdh*2; //rect.width();	
			float dx, dy, dx1, dy1;
			
			dx = wdh * cos(angle);
			dy = wdh * sin(angle);

			dx1 = wdh1 * cos(angle);
			dy1 = wdh1 * sin(angle);

			Point op1,op2;
			p.x = p.x - dx; 
			p.y = p.y - dy;

			op1 = p;				
			op2= p;

			op1.x = op1.x + dx/2;
			op1.y = op1.y + dy/2;

			op2.x = op2.x + dx1;
			op2.y = op2.y + dy1;
				
			painter->drawLine(op1.x+dx2, op1.y+dy2, op2.x+dx2, op2.y+dy2);	

			p= op2;
			p.x = p.x + dx; 
			p.y = p.y + dy;
		}
		string st = ( ((tr->_ring)[layerIndex]._node_name)[nodeIndex[ni]] );
		
		float angle1, angle2;
		angle1 = tr->_ring[layerIndex]._limits[nodeIndex[ni]].first;
		angle2 = tr->_ring[layerIndex]._limits[nodeIndex[ni]].second;
			
		if(angle1 > angle2)
		{
			angle1 = angle1- M_PI*2; 		
		}	
		angle =  (angle1 + angle2)/2;

		
		if(layerIndex==0)
		{
			painter->setBrush(Qt::NoBrush);
			p=posRecord[nodeIndex[ni]];
			Render(painter, st, p, p-((tr->_ring)[layerIndex]._centre_pos)[nodeIndex[ni]], angle, tr->getWidth(), tr->_ring[tr->_ring.size()-1]._radius[0], co, true);			
		}
		else
		{	
			
			Render(painter, st, p, p-((tr->_ring)[layerIndex]._centre_pos)[nodeIndex[ni]], Angle, tr->getWidth(), tr->_ring[layerIndex]._radius[nodeIndex[ni]], co, false); //changed when switching inner ring out
			
		}
		if(firstNodeAngle<=-100)
			firstNodeAngle = angle;

    }; // end for(ni)
	return firstNodeAngle;
}

void Label::BuildScreenGrid(TreeRing *tr)
{
	//Get the longest name in tree ring nodes
	_grid.clear();
	string str = tr->GetLongestName();
	int len = 20; //GetBitmapStringLength(str);

	_cell_height = 12; //10 * 1.2
	_cell_width = /*len * */len;
	_nr_col = _wnd_width / _cell_width;
	_nr_row = _wnd_height / _cell_height;
	if(_wnd_width % _cell_width != 0)  _nr_col++;
	if(_wnd_height % _cell_height != 0)_nr_row++;

	//Build a Grid to decide if a cell has been occupied
	for(int i=0; i<_nr_row; ++i)
	{
		vector<int>v;
		for(int j=0; j<_nr_col; ++j)
		{
			v.push_back(0);
		}
		_grid.push_back(v);
	}
}

void Label::FindNearestNeighborCell(int x, int y, int &ox, int &oy)
{
	int lx = max(x-1, 0);         //left
	int rx = min(_nr_col-1, x+1); //right
	int uy = max(y-1, 0);         //top
	int dy = min(_nr_row-1, y+1); //bottom

	if(!_grid[uy][x]) {ox = x; oy = uy; return;}   //up
	if(!_grid[dy][x]) {ox = x; oy = dy; return;}   //down
	if(!_grid[y][lx]) {ox = lx; oy = y; return;}   //left
	if(!_grid[y][rx]) {ox = rx; oy = y; return;}   //right
	if(!_grid[uy][rx]) {ox = rx; oy = uy; return;} //up right
	if(!_grid[dy][rx]) {ox = rx; oy = dy; return;} //down right
	if(!_grid[uy][lx]) {ox = lx; oy = uy; return;} //up left
	if(!_grid[dy][lx]) {ox = lx; oy = dy; return;} //down left

	ox=-1;
	oy=-1;
}

float Label::getAngle(TreeRing *tr, int layer, int id)
{
	float angle1, angle2, angle;			
	angle1 = tr->_ring[layer]._limits[id].first;
	angle2 = tr->_ring[layer]._limits[id].second;

	string st  =  (((tr->_ring)[layer]._node_name)[id] );
	Point p = ((tr->_ring)[layer]._centre_pos)[id];

	if(angle1 > angle2)
	{
		angle1 = angle1- M_PI*2; 		
	}	
	angle =  (angle1 + angle2)/2;
	return angle;
}

void Label::findOverLap(vector<float> &angleRecord, float angle, int j,  int size, float tSpan, float tSpan1, bool &overLap, bool &overLap1)
{			
	if(j==0)
	{
		overLap=false;
	}
	else 
	{
		if(fabs(angleRecord[j-1]-angle)<tSpan)
			overLap=true;
		if(!overLap && j==size-1)
		{
			if(fabs(angleRecord[0]-angle)<tSpan)
				overLap=true;				
		}
	}
	if(overLap && j!=0)
	{
		if(fabs(angleRecord[j-1]-angle)<tSpan1)
		{
			overLap1=true;
		}
		if(j==size-1)
		{
			if(fabs(angleRecord[0]-angle)<tSpan1)
				overLap1=true;								
		}				
	}
	if(overLap1)
		overLap=false;				
			
}
			

//Render labels of all nodes in treering
void Label::RenderTreeRingNodeLabels(QPainter *painter, TreeRing *tr)
{
	//get angle overlap	
	_num_layers = tr->_ring.size();
	if(tr->_ring[_num_layers-1]._radius.size()<=0)
		return;
	float Rds =  tr->_ring[_num_layers-1]._radius[0];//tr->GetRadius(); //changed when switching innder ring out
	float wdh =  tr-> GetRingWidth();//tr->getWidth();
	QFont f("Arial",10);
	QRect rect;
	QFontMetrics fontMetrics(f);		
	rect = fontMetrics.boundingRect("A"); 
	float w, tSpan, da;
	
	
	w=rect.height();
	da = 1-w*w/(2*Rds*Rds);
	da = acos(da)/2;	

	w=w*0.8;
	tSpan= 1-w*w/(2*Rds*Rds);
	tSpan = acos(tSpan); //smallest allowed angle span between labels to avoid overlapping
	
	float Rds1=Rds+wdh;
	float tSpan1= 1-w*w/(2*Rds1*Rds1);
	tSpan1 = acos(tSpan1); //smallest allowed angle span between labels to avoid overlapping
	
	//Reset 
	//static vector<Point> pList1;
	//vector<Point> pList2;
	for(int i=0; i<_grid.size(); ++i)
		for(int j=0; j<_grid[i].size(); ++j)
			_grid[i][j] = 0;


	int size=(tr->_ring)[0]._limits.size();
	posRecord.resize(size);

	for(int layer=0; layer<1; layer++)
	{
		int size=(tr->_ring)[layer]._limits.size();
		vector<float> angleRecord;		
		angleRecord.resize(size);
		for(int j=0; j<size; j++)
		{			
			bool overLap=false, overLap1=false;// overLap2=false;
			float angle = getAngle(tr, layer, j), Angle;	
			Angle = angle;
			int preID=0;
			string st  =  (((tr->_ring)[layer]._node_name)[j] );
			Point p = ((tr->_ring)[layer]._centre_pos)[j];
		
			//void findOverLap(vector<float> &angleRecord, float angle, bool &overLap, bool &overLap1)
			findOverLap(angleRecord, angle, j, size, tSpan, tSpan1, overLap, overLap1);
			/*{
				if(j==0)
				{
					 overLap=false;
				}
				else 
				{
					if(fabs(angleRecord[j-1]-angle)<tSpan)
						overLap=true;
					if(!overLap && j==size-1)
					{
						if(fabs(angleRecord[0]-angle)<tSpan)
						  overLap=true;				
					}
				}
				if(overLap && j!=0)
				{
					if(fabs(angleRecord[j-1]-angle)<tSpan1)
					{
						overLap1=true;
					}
					if(j==size-1)
					{
						if(fabs(angleRecord[0]-angle)<tSpan1)
						  overLap1=true;								
					}				
				}
				if(overLap1)
				   overLap=false;				
			}*/
			
			
			if(overLap1 && j!=0)
			{
				
			    preID = j-1;			 
			}
			angleRecord[j]=angle;
			float dw = (tr->_ring[tr->_ring.size()-1]._radius[0] - tr->_ring[0]._radius[0]);
			float dx, dy, dx2, dy2;
			dx2 = dw * cos(angle);
			dy2 = dw * sin(angle);

			if( overLap )
			{
				float dx, dy;
				dx = wdh * cos(angle);
				dy = wdh * sin(angle);
				Point op1,op2;
				op1 = p;
				p.x = p.x + dx; 
				p.y = p.y + dy;

				//Point dp=p-((tr->_ring)[layer]._centre_pos)[j];

				op2= p;
				op1.x = op1.x + dx/2;
				op1.y = op1.y + dy/2;

				op2.x = op2.x + dx/2;
				op2.y = op2.y + dy/2;
				//draw label link to node
				painter->setPen(QColor(0, 0, 0, 255));
				
				painter->drawLine(op1.x+dx2, op1.y+dy2, op2.x+dx2, op2.y+dy2);				
				p= op2;
			}
			else if( overLap1 )
			{
				QRect rect;			
				float wdh1 = wdh*2; //rect.width();	
				float dx, dy, dx1, dy1;
				

				dx = wdh * cos(angle);
				dy = wdh * sin(angle);

				dx1 = wdh1 * cos(angle);
				dy1 = wdh1 * sin(angle);

				Point op1,op2;
				op1 = p;
				p.x = p.x + dx; 
				p.y = p.y + dy;

				
				op2= p;
				op1.x = op1.x + dx/2;
				op1.y = op1.y + dy/2;

				op2.x = op2.x + dx1;
				op2.y = op2.y + dy1;
				
				p= op2;
				//draw label link to node
				painter->setPen(QColor(0, 0, 0, 255));
				
				painter->drawLine(op1.x+dx2, op1.y+dy2, op2.x+dx2, op2.y+dy2);				
			}
			posRecord[j]=p;
			//Render(painter, st, p, p-((tr->_ring)[layer]._centre_pos)[j], Angle, tr->getWidth(), tr->_ring[layer]._radius[j], Color(0,0,0));
			//if(layer==0)	
			painter->setBrush( Qt::NoBrush);
			Render(painter, st, p, p-((tr->_ring)[layer]._centre_pos)[j], Angle, tr->getWidth(), tr->_ring[tr->_ring.size()-1]._radius[0], Color(0,0,0), true); //changed when switching inner ring out
			
		}
	}
	//pList1=pList2;
}

void Label::DrawLine(float x1, float y1, float x2, float y2)
{
	glLineWidth(1.);
	glColor3ub(100, 100, 255);
	glBegin(GL_LINES);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glEnd();
}

float Label::Render(QPainter *painter, string name, Point pos, Point posD, float angle, float ringWidth, float ringradius, Color color, bool outest)
{
	//used
	//If the position is out of the screen, don't render
	if(pos.x < -_wnd_width/2 || pos.x > _wnd_width/2 ||
	   pos.y < -_wnd_height/2 || pos.y > _wnd_height/2)
	   return -1000;
	
	//float d = pow(posD.x(), 2);
	float rot=false;
	float ds = sqrt( pow(posD.x, 2)) + sqrt(pow(posD.y, 2));
	if(outest)
	     ringradius = ringradius -ringWidth/2 + ds;
	else		
	     //ringradius = ringradius -ringWidth + ds;
		 ringradius = ringradius - ringWidth* 0.5 + ds;


	//------Compute which cell this position is in-----------------------------
	float x= pos.x + _wnd_width/2;
	float y= _wnd_height - (pos.y  + _wnd_height/2);
	int cell_col = int(x / _cell_width);
	int cell_row = int(y / _cell_height);

	//Check if this cell has been occupied
	int cr, cc;
	if(_grid[cell_row][cell_col])
		FindNearestNeighborCell(cell_col, cell_row, cc, cr);
	else
	{
		cc = cell_col;
		cr = cell_row;
	}

	//If the 8 cells around current cell were occupied, don't label this node
	if(cc == -1)
		return -1000;
	//------Compute which cell this position is in-----------------------------


	//Calculate the position of label, if the labels are on the left of the circle, 
	//the last letter will end in the center of that block.


	QFont f("Arial", 10);// * _scale);
	QString qstr = QString::fromStdString(name);
	//qstr=qstr.replace("_"," ");
	//qstr = "  " + qstr;
	
	QRect rect2;
	float strx = pos.x, stry = pos.y;
		
	/*if(pos.x - compoudgraph_centre.x < 0) //right
	{				
			QFontMetrics fontMetrics(f);			
			
			rect2 = fontMetrics.boundingRect(qstr); 					
			
			strx = pos.x;// - rect2.width(); //GetBitmapStringLength(name);

			if(strx < -_wnd_width/2)
				return;
	}*/	
	if( angle>M_PI/2 && angle<3*M_PI/2 )
	{	
		//stry  =  stry - rect2.height()/2;
        //angle= angle - angleD;
		rot=true;
		
		if(outest)
		    qstr = qstr + "      ";
		else qstr = qstr + "   ";

		QFontMetrics fontMetrics(f);						
		rect2 = fontMetrics.boundingRect(qstr); 	
		float angleD= rect2.height()/2/ringradius/3;
		angle= angle - angleD;

		strx = ringradius*cos(angle);
		stry = ringradius*sin(angle);	
		
		float w=rect2.width() + ringWidth/2;		//ringWidth/2 + 
		strx = strx + w*cos(angle) + compoudgraph_centre.x;
		stry = stry + w*sin(angle) + compoudgraph_centre.y;		
       
	}
	else
	{		
		if(outest)
		     qstr = "     " + qstr;	
		else 
			qstr = "  " + qstr;

		QFontMetrics fontMetrics(f);						
		rect2 = fontMetrics.boundingRect(qstr); 	
		float angleD= rect2.height()/2/ringradius/3;
		angle= angle + angleD;

		strx = ringradius*cos(angle);
		stry = ringradius*sin(angle);			
		
	    strx =  strx + ringWidth/2*cos(angle) + compoudgraph_centre.x;
		stry =  stry + ringWidth/2*sin(angle) + compoudgraph_centre.y;			
	}
	

	//Draw text
	char* ss = new char[name.length()+2];
	int si;
	for(si=0; si<name.length(); si++)
	{
		ss[si]=name[si];
	}
	ss[si+1]='\0';


	char* p;  //Temp pointer to the string varible passed in

	//glDisable(GL_LIGHTING);
	//glColor4ub(color.r, color.g, color.b, 128);
	
	// Specify the initial position on the screen where the text
	// will be diaplayed
	
	//glRasterPos2f(strx, stry);

	//glDisable(GL_TEXTURE_2D);
	// Parse the string passed in and display to screen one char at a time
	//for (p = ss; *p; p++)
		//glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10 , *p);

	//painter->drawText(strx, stry, qstr);

	painter->setFont(f);	
	painter->save();
	painter->translate(strx, stry);

	float fh=rect2.height()*0.8;
	rect2 = QRect(rect2.x(), rect2.center().y()-fh/2, rect2.width(), fh);
	
	
	//painter->setBrush(QColor(255, 255, 255,  155));

	//float dx,dy;
	//dx=dy=0;
	
	//if( angle>M_PI/2 && angle<3*M_PI/2 )
	if(rot)
	{
		//QFontMetrics fontMetrics(f);						
		//rect2 = fontMetrics.boundingRect(qstr); 					
		//float w=rect2.width();		
        angle = angle + M_PI;
		//dx = w*cos(angle); dy=w*sin(angle);
	}

	painter->rotate(angle/M_PI*180);
	
	painter->setPen(Qt::NoPen);
	painter->setBrush( HIGHLIGHTCOLOR1 );

	painter->drawRect(rect2);
	painter->setPen(QColor(color.r, color.g, color.b, 255));
	//painter->drawText(rect2, Qt::AlignCenter, qstr);
	painter->drawText(0, 0, qstr);

	//painter->endNativePainting();
	//painter->drawText(strx, stry, qstr);
	painter->restore();	

	delete ss;

	_grid[cr][cc] = 1;

	return angle;
}

int Label::GetBitmapStringLength(string str)
{
	int len;
	unsigned char *cstr = new unsigned char[str.length() + 1];
	int si;
	for(si=0; si<str.length(); si++)
		cstr[si]=str[si];

	cstr[si]='\0';
	len = glutBitmapLength(GLUT_BITMAP_HELVETICA_10, cstr);
	delete cstr;
	return len;
}
