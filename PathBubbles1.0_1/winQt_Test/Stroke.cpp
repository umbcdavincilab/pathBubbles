
//#include <iostream>
//#include <stdio.h>
//#include <stdlib.h>
#include "Stroke.h"
#include "drand48.h"
#ifdef SCAN_CONVERT
#include "polyfill.h"
#endif

#ifndef SCAN_CONVERT
//GLUquadricObj * Stroke::qobj = NULL;
#endif

//using namespace std;

Stroke::Stroke()
{
	highlightIndex = -1;
	highlightCurveIndex = -1;
	limit = temp = NULL;
	computed = GL_FALSE;
	curveType = CUBIC_BSPLINE;	
	useTexture = true;

	numLevels = 3;

	//EnableLineColor=false;
	EnableLineColor=true;

	if_highlight = false;
	z = 0;

	color = Color(DEFAULT_R, DEFAULT_G, DEFAULT_B, DEFAULT_A);

#ifndef SCAN_CONVERT
	/*
	if (qobj == NULL)
	{
	qobj = gluNewQuadric();
	}
	*/
#endif

	ufreq = .005;
	vfreq = .1;
	ustart = .1;
	vstart = drand48();
}



Stroke::~Stroke()
{
	vector<Point> test;
	if (limit != NULL)
	{
		limit->clear();
		if(!limit->empty())
		   delete limit;
	}

	if (temp != NULL)
	{
		temp->clear();
		if(!temp->empty())
			delete temp;
	}
}

void Stroke::add(float x,float y)
{
	//if (control.empty() || (control.back().x != x && control.back().y != y))
	//
	//
	if(!control.empty() && (fabs(control.back().x - x) <= 1e-4 && fabs(control.back().y - y) <= 1e-4))
		x-=10, y-=10;
	control.push_back(Point(x,y));
	computed = GL_FALSE;
}

void Stroke::updateCtrlPoints(int pi, float x, float y)
{
	(control[pi]).x = x;
	(control[pi]).y = y;
	computed = GL_FALSE;
}

void Stroke::printCtrlPoints()
{
	printf("======== printCtrlPoints() ==========\n");
	for(int i=0;i<control.size();i++)
	{
		Point & p = (control)[i];
		printf("%d: %f %f\n", i, (float)(p.x),(float)(p.y));
	}; 

	printf("======================================\n");
}

void Stroke::clear()
{
	computed = GL_FALSE;
	control.erase(control.begin(),control.end());
	limit->erase(control.begin(),control.end());
	ustart = .1;
	vstart = drand48();
}



void Stroke::drawPoints(vector<Point> *curve)
{
	//float c[3];
	glDisable(GL_TEXTURE_2D);

	//glGetFloatv(GL_CURRENT_COLOR, &c);
	glBegin(GL_POINTS);
	for(int i=0;i<curve->size();i++)
	{
		Point & p = (*curve)[i];

		if(highlightIndex!=-1)
		{
			if(i==highlightIndex)
				glColor3ub(255, 0, 0);
			else
				glColor3ub(0, 255, 255);

			glVertex2i(p.x,p.y);
		}
		else if(highlightCurveIndex!=-1)
		{
			if(i==highlightCurveIndex || i== highlightCurveIndex+1)
			{
				glColor3ub(255, 255, 0);
			}
			else
				glColor3ub(0, 255, 255);

			glVertex2i(p.x,p.y);
		}
		else
		{
			glColor3ub(0, 255, 255);
			glVertex2i(p.x,p.y);
		}; // end if(highlightIndex)
	}; // end for(i)
	glEnd();
}

void Stroke::drawPoints(vector<Point> *curve, vector <Color> dc)
{
	// float c[3];
	glDisable(GL_TEXTURE_2D);

	//glGetFloatv(GL_CURRENT_COLOR, &c);
	glBegin(GL_POINTS);
	for(int i=0;i<curve->size();i++)
	{
		Point & p = (*curve)[i];

		if(highlightIndex!=-1)
		{
			if(i==highlightIndex)
				glColor3ub(255, 0, 0);
			else
			{
				//glColor3ub(0, 255, 255);
				glColor3ub(dc[i].r,dc[i].g,dc[i].b);
				//cerr << "here 3"<<endl;
			}

			glVertex2i(p.x,p.y);
		}
		else if(highlightCurveIndex!=-1)
		{
			if(i==highlightCurveIndex || i== highlightCurveIndex+1)
			{
				glColor3ub(255, 255, 0);
			}
			else
			{
				//  glColor3ub(0, 255, 255);
				glColor3ub(dc[i].r,dc[i].g,dc[i].b);
				////cerr << "here 2"<<endl;
			}

			glVertex2i(p.x,p.y);
		}
		else
		{
			//cerr << "here 1"<<endl;
			glColor3ub(dc[i].r,dc[i].g,dc[i].b);
			//glColor3ub(0, 255, 255);
			glVertex2i(p.x,p.y);
		}; // end if(highlightIndex)
	}; // end for(i)
	glEnd();
}


void Stroke::drawPoints(vector<Point> *curve, vector <Color> dc, vector <int> nc)
{
	//  float c[3];
	glDisable(GL_TEXTURE_2D);

	//glGetFloatv(GL_CURRENT_COLOR, &c);
	glBegin(GL_POINTS);
	for(int i=0;i<curve->size();i++)
	{
		if(nc[i])
		{
			Point & p = (*curve)[i];

			if(highlightIndex!=-1)
			{
				if(i==highlightIndex)
					glColor3ub(255, 0, 0);
				else
				{
					//glColor3ub(0, 255, 255);
					glColor3ub(dc[i].r,dc[i].g,dc[i].b);
				}

				glVertex2i(p.x,p.y);
			}
			else if(highlightCurveIndex!=-1)
			{
				if(i==highlightCurveIndex || i== highlightCurveIndex+1)
				{
					glColor3ub(255, 255, 0);
				}
				else
				{
					//  glColor3ub(0, 255, 255);
					glColor3ub(dc[i].r,dc[i].g,dc[i].b);
					////cerr << "here 2"<<endl;
				}

				glVertex2i(p.x,p.y);
			}
			else
			{
				//cerr << "here 1"<<endl;
				glColor3ub(dc[i].r,dc[i].g,dc[i].b);
				//glColor3ub(0, 255, 255);

				glVertex2i(p.x,p.y);
			}; // end if(highlightIndex)
		}; // end if(nc[i])
	}; // end for(i);
	glEnd();
}


void Stroke::drawLines(QPainter *painter, vector<Point> *curve)
{
	//glDisable(GL_TEXTURE_2D);	
	if(!if_highlight)
	{		
		//painter->setPen(QColor(color.r*255, color.g*255, color.b*255, 255));	
		painter->setPen(QColor(0,0,0, 255));	
	    painter->drawLine(0,1,0,1);		    
	}
	int size = curve->size();
	if(size!=0)
	{
		glBegin(GL_LINE_STRIP);		
		for(int i=0;i<curve->size();i=i+1)
		{
			Point & p = (*curve)[i];			
			glVertex2i(p.x,p.y);			
		}	
	    glEnd();
	}	
	/*for(int i=0;i<curve->size()-1;i=i+1)
	{
		if(if_highlight)
		{	
			//glColor4ub(highlight_color.r, highlight_color.g, highlight_color.b, highlight_color.a);
			//painter->setPen( QColor( 255, 20, 20, 255 ) );
			//painter->setPen( QColor( highlight_color.r, highlight_color.g, highlight_color.b, highlight_color.a ) );
			Point & p = (*curve)[i];
			Point & p1 = (*curve)[i+1];
			painter->drawLine(p.x,p.y,p1.x,p1.y);
		}
		else
		{	//glColor4ub(interpolated_color[i].r, interpolated_color[i].g, interpolated_color[i].b, interpolated_color[i].a);
			//painter->setPen( QColor( interpolated_color[i].r*255, interpolated_color[i].g*255, interpolated_color[i].b*255, interpolated_color[i].a*255 ) );
		    //painter->setPen( QColor( 255, 0, 0, 255) );
		}		
		
	}	*/
}


void Stroke::drawLines(vector<Point> *curve, Color newcolor)
{
	glDisable(GL_TEXTURE_2D);

	glColor4ub(newcolor.r, newcolor.g, newcolor.b, newcolor.a);
	glBegin(GL_LINE_STRIP);
	for(int i=0;i<curve->size();i++)
	{
		Point & p = (*curve)[i];
		glVertex2i(p.x,p.y);
	}
	glEnd();
}


void Stroke::getClosest(float thre, int mx, int my)
{
	vector<Point> *curve = &control;
	//printf("get closest ");

	for(int i=0;i<curve->size();i++)
	{
		Point & p = (*curve)[i];
		float dist = sqrt((float)(mx-p.x) *(float)(mx-p.x) + (float)(my-p.y)*(float)(my-p.y));
		if( dist < thre)
		{
			highlightIndex = i;
			highlightCurveIndex=-1;
			return;
		}
	};

	// OK let's check if mouse cursor intersects with the mid-point of a curve
	for(int i=0;i<curve->size()-1;i++)
	{
		Point & p1 = (*curve)[i];
		Point & p2 = (*curve)[i+1];
		Point  pm;
		pm.x = (p1.x+p2.x)/2.;
		pm.y = (p1.y+p2.y)/2.;

		float dist = sqrt((float)(mx-pm.x) *(float)(mx-pm.x) + (float)(my-pm.y)*(float)(my-pm.y));
		if( dist < thre*2)
		{
			highlightCurveIndex = i;
			highlightIndex=-1;
			return ;
		}
	};

	highlightIndex=-1;
	highlightCurveIndex=-1;
	return ;
}

void Stroke::forceRecompute()
{
	computed = false;
}

void Stroke::getPointByIndex(int idx, float *x, float *y)
{
	*x = control[idx].x;
	*y = control[idx].y;
}

void Stroke::discPoint(float x,float y,float brushRadius)
{
#ifdef SCAN_CONVERT
	// really ought to implement scan-conversion of the circle here

	glColor3ub(0,0,255);
	drawCap(Point(x,y),1,0,0,0);
	drawCap(Point(x,y),-1,0,0,0);
#else
	/*
	glPushMatrix();
	glTranslatef(x,y,z);
	gluDisk(qobj,0,brushRadius,NUM_SLICES,1);
	glPopMatrix();
	*/
#endif
}

void Stroke::drawCap(const Point & p0, float dx, float dy,
					 float texU,float texV)
{
#ifdef SCAN_CONVERT
	glDisable(GL_TEXTURE_2D);

	float theta = atan2(-dy,-dx);

	Point p1(p0.x - radius * dx, p0.y - radius*dy);

	for(int i=1;i<NUM_SLICES-1;i++)
	{
		float dx1 = cos(theta + i * M_PI / NUM_SLICES);
		float dy1 = sin(theta + i * M_PI / NUM_SLICES);

		Point p2(p0.x + radius * dx1, p0.y + radius*dy1);

		drawTriangle(p0,p1,p2);

		p1 = p2;
	}

	drawTriangle(p0,p1,Point(p0.x+radius*dx,p0.y+radius*dy));
#else
	float theta = atan2(-dy,-dx);

	glPushMatrix();
	glTranslatef(p0.x,p0.y,z);

	glBegin(GL_TRIANGLE_FAN);
	//        glVertex3f(p0->x, p0->y,z);
	glTexCoord2f(texU,texV);
	glVertex3i(0,0,0);


	glTexCoord2f(texU+ufreq*radius * dx,texV+vfreq*dy);
	glVertex3f(-radius*dx,-radius*dy,0);

	for(int i=1;i<=NUM_SLICES-1;i++)
	{
		float dx1 = cos(theta + i * M_PI / NUM_SLICES);
		float dy1 = sin(theta + i * M_PI / NUM_SLICES);

		// these 
		glTexCoord2f(texU-ufreq*radius * dx1,texV-vfreq*dy1);

		glVertex3i(radius * dx1, radius * dy1, 0);
		//glVertex3f(p0->x + radius * dx1, p0->y + radius * dy1, z);
	}

	glTexCoord2f(texU-ufreq*radius * dx,texV-vfreq*dy);
	glVertex3f(radius*dx,radius*dy,0);

	glEnd();

	glPopMatrix();
#endif
}

void Stroke::drawThickCurve(vector<Point> * curve, float radius,bool cap)
{

	if (useTexture)
		glEnable(GL_TEXTURE_2D);
	else
		glDisable(GL_TEXTURE_2D);

	int i=0;
	float dx,dy,mag;
	Point p0;
	Point p1;
	Point p2;

	if (curve->empty())
		return;

	p0 = (*curve)[0];

	if (curve->size() == 1)
	{
		if (cap)
			discPoint(p0.x,p0.y,radius);

		return;
	}

	p1 = (*curve)[1];

	dx = p1.y - p0.y;
	dy = p0.x - p1.x;

	mag = sqrt(dx*dx + dy*dy);

	dx /= mag;
	dy /= mag;

	float textureU = ustart;
	float textureV = vstart;

	glColor3ub(0,0,255);

	if (cap)
		drawCap(p0, dx, dy,textureU,textureV);

#ifdef SCAN_CONVERT
	glDisable(GL_TEXTURE_2D);

	Point v0(p0.x + radius * dx, p0.y + radius * dy);
	Point v1(p0.x - radius * dx, p0.y - radius * dy);

#else
	glBegin(GL_TRIANGLE_STRIP);

	glTexCoord2f(textureU,textureV + vfreq);
	glVertex3f(p0.x + radius * dx, p0.y + radius * dy,z);

	glTexCoord2f(textureU,textureV - vfreq);
	glVertex3f(p0.x - radius * dx, p0.y - radius * dy,z);
#endif

	textureU += ufreq;

#ifdef SCAN_CONVERT
	// draw the patch in between with extra subdivision to match the caps,
	// to prevent holes from appearing in the stroke
	if (curve->size() >= 2)
	{
		float dist = sqrt((p1.x-p0.x)*(p1.x-p0.x)+(p1.y-p0.y)*(p1.y-p0.y));
		textureU += ufreq * dist;

		v0 = Point(p0.x + radius * dx, p0.y + radius * dy);
		v1 = Point(p0.x - radius * dx, p0.y - radius * dy);
		Point v2(p1.x + radius * dx, p1.y + radius * dy);
		Point v3(p1.x - radius * dx, p1.y - radius * dy);

		glColor3ub(255,0,0);
		//      drawTriangle(v2,v3,v0);
		drawTriangle(v2,p1,v0);
		glColor3ub(255,255,0);
		drawTriangle(p1,v3,v0);

		glColor3ub(0,255,0);
		//      drawTriangle(v0,v1,v3);
		drawTriangle(v0,p0,v3);
		glColor3ub(0,255,255);
		drawTriangle(p0,v1,v3);

		glColor3ub(0,0,255);

		if (curve->size() == 2)
		{
			drawCap(p1,-dx,-dy,textureU,textureV);
			return;
		}


		v0 = v2;
		v1 = v3;
	}
#endif

#ifdef SCAN_CONVERT
	for(i=2;i<curve->size()-1;i++)
#else
	for(i=1;i<curve->size()-1;i++)
#endif
	{
		p0 = (*curve)[i-1];
		p1 = (*curve)[i];
		p2 = (*curve)[i+1];

		dx = p2.y - p0.y;
		dy = p0.x - p2.x;

		mag = sqrt(dx*dx + dy*dy);

		dx /= mag;
		dy /= mag;

		float dist = sqrt((p1.x-p0.x)*(p1.x-p0.x)+(p1.y-p0.y)*(p1.y-p0.y));
		textureU += ufreq * dist;

#ifdef SCAN_CONVERT
		Point v2(p1.x + radius * dx, p1.y + radius * dy);
		Point v3(p1.x - radius * dx, p1.y - radius * dy);

		glColor3ub(255,0,0);
		drawTriangle(v2,v3,v0);

		glColor3ub(0,255,0);
		drawTriangle(v0,v1,v3);

		v0 = v2;
		v1 = v3;
#else
		glTexCoord2f(textureU,textureV + vfreq);
		glVertex3f(p1.x + radius * dx, p1.y + radius * dy,z);

		glTexCoord2f(textureU,textureV - vfreq);
		glVertex3f(p1.x - radius * dx, p1.y - radius * dy,z);
#endif

	}

	p0 = (*curve)[curve->size()-2];
	p1 = (*curve)[curve->size()-1];

	dx = p1.y - p0.y;
	dy = p0.x - p1.x;

	mag = sqrt(dx*dx + dy*dy);

	dx /= mag;
	dy /= mag;

	textureU += ufreq *mag;

#ifdef SCAN_CONVERT
	Point v2(p1.x + radius * dx, p1.y + radius * dy);
	Point v3(p1.x - radius * dx, p1.y - radius * dy);

	glColor3ub(255,0,0);
	//  drawTriangle(v2,v3,v0);
	drawTriangle(v2,p1,v0);
	glColor3ub(255,255,0);
	drawTriangle(p1,v3,v0);

	glColor3ub(0,255,0);
	drawTriangle(v0,v3,v1);
	//  drawTriangle(v0,p0,v3);
	//  glColor3ub(0,255,255);
	//  drawTriangle(p0,v1,v3);

#else
	glTexCoord2f(textureU,textureV + vfreq);
	glVertex3f(p1.x + radius * dx, p1.y + radius * dy,z);
	glTexCoord2f(textureU,textureV - vfreq);
	glVertex3f(p1.x - radius * dx, p1.y - radius * dy,z);

	glEnd();
#endif



	glColor3ub(0,0,255);

	if (cap)
	{
		drawCap(p1, -dx, -dy,textureU,textureV);
	}

}

void Stroke::drawControl(QPainter *painter, int primType)
{
	painter->setPen( QColor( 0, 0, 0, 255 ) );

	glColor3ub(255,0,0);
	if(primType == CLL)
	{
		drawLines(painter, &control);
	}
	else
	{
		glPointSize(10);
		drawPoints(&control);
	}; 
}


void Stroke::drawControl(QPainter *painter, int primType, vector <Color> dc)
{
	glColor3ub(255,0,0);
	if(primType == CLL)
	{
		drawLines(painter, &control);
	}
	else
	{
		glPointSize(10);
		drawPoints(&control, dc);
	}; 
}

void Stroke::drawControl(QPainter *painter, int primType, vector <Color> dc, vector <int> nc)
{
	glColor3ub(255,0,0);
	if(primType == CLL)
	{
		drawLines(painter, &control);
	}
	else
	{
		glPointSize(10);
		drawPoints(&control, dc, nc);
	}; 
}

void Stroke::drawLineCurve(QPainter *painter)
{
	drawLines(painter, limit);
}


void Stroke::render(Color newcolor)
{
	if (!computed)
	{
		computeLimitCurve();
		computed = GL_TRUE;
	}
	drawLines(limit, newcolor);
	//drawThickCurve(limit,radius);
}

void Stroke::render(QPainter *painter)
{
	if (!computed)
	{
		computeLimitCurve();
		InterpolateColor();
		computed = GL_TRUE;
	}
	drawLines(painter, limit);
	//drawThickCurve(limit,radius);
}

void Stroke::subdivideCubicBSpline(vector<Point> * inputCurve, 
								   vector<Point> * outputCurve)
{
	outputCurve->erase(outputCurve->begin(),outputCurve->end());

	//    printf("ic-count=%d\n",inputCurve->count);

	if (inputCurve->size() < 1)
		return;

	Point pi0;
	Point pi1;
	Point pi2;

	pi0 = (*inputCurve)[0];

	outputCurve->push_back(Point(pi0.x,pi0.y));

	if (inputCurve->size() == 1)
		return;

	if (inputCurve->size() == 2)
	{
		pi1 = (*inputCurve)[1];

		outputCurve->push_back(Point(pi1.x,pi1.y));

		return;
	}

	pi1 = (*inputCurve)[1];

	outputCurve->push_back(Point((pi0.x + pi1.x)/2,(pi0.y + pi1.y)/2));

	for(int i=1;i<inputCurve->size()-1;i++)
	{
		pi0 = (*inputCurve)[i-1];
		pi1 = (*inputCurve)[i];
		pi2 = (*inputCurve)[i+1];

		outputCurve->push_back(Point( (pi0.x + 6*pi1.x + pi2.x)/8,
			(pi0.y + 6*pi1.y + pi2.y)/8));

		outputCurve->push_back(Point( (pi1.x + pi2.x)/2,(pi1.y + pi2.y)/2));
	}

	outputCurve->push_back(Point(pi2.x,pi2.y));
}

void Stroke::subdivideFourPoint(vector<Point> * inputCurve, vector<Point> * outputCurve)
{
	outputCurve->erase(outputCurve->begin(),outputCurve->end());

	if (inputCurve->size() < 1)
		return;

	Point pi0;
	Point pi1;
	Point pi2;
	Point pi3;

	if (inputCurve->size() == 1) //等于1不用算了，把这个点输出
	{
		pi0 = (*inputCurve)[0];
		outputCurve->push_back(Point(pi0.x,pi0.y));

		return;
	}

	if (inputCurve->size() == 2)//输入有两个点，中间插一个就OK
	{
		pi0 = (*inputCurve)[0];
		pi1 = (*inputCurve)[1];

		outputCurve->push_back(Point(pi0.x,pi0.y));
		outputCurve->push_back(Point((pi0.x+pi1.x)/2,(pi0.y+pi1.y)/2));
		outputCurve->push_back(Point(pi1.x,pi1.y));

		return;
	}

	pi0 = (*inputCurve)[0];
	pi1 = (*inputCurve)[1];

	Point piminus1(2*pi0.x - pi1.x,2*pi0.y - pi1.y);

	pi0 = (*inputCurve)[inputCurve->size()-1];
	pi1 = (*inputCurve)[inputCurve->size()-2];

	Point piplus1(2*pi0.x - pi1.x,2*pi0.y - pi1.y);

	for(int i=0;i<inputCurve->size()-1;i++)
	{
		pi0 = (i==0 ? piminus1 : (*inputCurve)[i-1]);
		pi1 = (*inputCurve)[i];
		pi2 = (*inputCurve)[i+1];
		pi3 = (i==inputCurve->size()-2? piplus1:(*inputCurve)[i+2]);

		outputCurve->push_back(Point( pi1.x, pi1.y));

		outputCurve->push_back(Point( (-pi0.x + 9*pi1.x + 9*pi2.x - pi3.x)/16,
			(-pi0.y + 9*pi1.y + 9*pi2.y - pi3.y)/16));
	}

	pi0 = inputCurve->back();

	outputCurve->push_back(Point(pi0.x,pi0.y));
}

void Stroke::subdivide(vector<Point> * inputCurve, vector<Point> * outputCurve)
{
	switch(curveType)
	{
	case CUBIC_BSPLINE:
		subdivideCubicBSpline(inputCurve,outputCurve);
		break;

	case FOUR_POINT:
		subdivideFourPoint(inputCurve,outputCurve);
		break;

	default:
		printf( "Illegal subdivision scheme selected\n");
		exit(-1);
	}
}

void Stroke::TranslateLimit(Point p)
{
	/*for(int i=0; i<control.size(); ++i)
	{
		control[i].x += p.x;
		control[i].y += p.y;
	}
	*/
	for(int i=0;i<limit->size();i=i+1)
	{
			
		//glColor4ub(highlight_color.r, highlight_color.g, highlight_color.b, highlight_color.a);
		//painter->setPen( QColor( 255, 20, 20, 255 ) );
		//painter->setPen( QColor( highlight_color.r, highlight_color.g, highlight_color.b, highlight_color.a ) );
		//Point & p = (*curve)[i];
		//Point & p1 = (*curve)[i+1];
		//painter->drawLine(p.x,p.y,p1.x,p1.y);
		(*limit)[i] = (*limit)[i] + p;
		
	}
	//computed = false;
}

void Stroke::computeLimitCurve()
{
	//    printf("Computing limit curve.  Input length = %d\n",control.count);

	//if (limit == NULL)
	limit = new vector<Point>();

	//if (temp == NULL)
	temp = new vector<Point>();

	subdivide(&control,limit);

	//    limit->print();
	//    printf(" count = %d\n",limit->size());

	for(int i=0;i<numLevels/2;i++)
	{
		subdivide(limit,temp);
		subdivide(temp,limit);
	}
}

void Stroke::InterpolateColor()
{
	Color src_color = Color(255,0,0, alpha), dst_color = Color(0,255,0, alpha);
	for(int i=0; i<limit->size(); ++i)
	{
		float ratio = (float)i / (float)(limit->size());

		Color co = src_color * (1-ratio)+ dst_color * ratio;
		interpolated_color.push_back(co);
	}
}