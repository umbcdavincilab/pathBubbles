//#include "qtbox.h"
#include "PieMenuItem.h"
#include "openglscene.h"

PieMenuItem::PieMenuItem(OpenGLScene* scene): QObject()
, _curState( false )
{
	int a=0;
	a=a;
}

PieMenuItem::~PieMenuItem()
{

}



void PieMenuItem::paint( QPainter *painter, QPointF bubblePos)
{
	
	//******************************************************
	//painter->setPen( Qt::NoPen );
	painter->setPen( Qt::black);
	painter->setBrush( QBrush( QColor( 166, 216, 84, 100 ) ) );
	   
	//******************************************************	
	//draw pie
	QFont aFont("Arial", 11, QFont::Normal);
	QFont aFont2("Arial", 9, QFont::Normal);

	float angle=this->_startAngle; 
	float spanAngle=this->_intervalAngle;
	float x=this->_pos.x()-bubblePos.x()-this->_R, y=this->_pos.y()-bubblePos.y()-this->_R, w=this->_R*2, h=this->_R*2;
	float h2=h*2, w2=w*2, x2=this->_pos.x()-bubblePos.x()-this->_R*2, y2=this->_pos.y()-bubblePos.y()-this->_R*2;
	for(int i=0; i<this->_sectorNum;i++)
	{
		 QColor c = this->_color[i];
		 if(i==this->_currentSector&&!this->_disables[i])
         {
			 if(this->_text2[i].size()>0)
			 {
				 //myColor3 _color = TDHelper::getBoarderColorByType(i+1,0);
			     QConicalGradient gradient(this->_pos.x()-bubblePos.x(), this->_pos.y()-bubblePos.y(), angle);
				 float spanAngle2=spanAngle/this->_text2[i].size();
				 float angle2=angle;
			     for(int j=0; j<this->_text2[i].size(); j++)
			     {
				   QColor c2 = this->_color2[i][j];
				   if(j==this->_currentSector2 && !this->_disables2[i][j])
					   c2.setAlpha(155);
				   
				   c.setAlpha(155);
				   gradient.setColorAt(0.0, QColor(0,0,0,155));
                   gradient.setColorAt(0.3, c2);   
			       //painter->setBrush(gradient);    
				   painter->setBrush(QColor(0,0,0,200));    
				   painter->setPen(QColor(255,255,255));
			       painter->drawPie(x2,y2,w2,h2,angle2*16,spanAngle2*16);
				   angle2+=spanAngle2;			     		     
				 }
			 }
			 //myColor3 _color = TDHelper::getBoarderColorByType(i+1,0);
			 //highlight
			 c.setAlpha(155);
			 QConicalGradient gradient(this->_pos.x()-bubblePos.x(), this->_pos.y()-bubblePos.y(), angle);
			 gradient.setColorAt(0.0, QColor(0,0,0, 155));
			 gradient.setColorAt(0.3, c);   
			 //painter->setBrush(gradient);  
			 painter->setBrush(QColor(0,0,0,200));  
			 //painter->setBrush(QColor(_color.a,_color.b,_color.c, _color.o));
		 }
		 else 
		 {
			 c.setAlpha(155);
			 QConicalGradient gradient(this->_pos.x()-bubblePos.x(), this->_pos.y()-bubblePos.y(), angle);
			 gradient.setColorAt(0.0, QColor(0,0,0,155));
			 gradient.setColorAt(0.3, c);   
			 painter->setBrush(QColor(0,0,0,200));  
			 //painter->setBrush(gradient);
		 }	
		 painter->setPen(QColor(240,240,240));
		 painter->drawPie(x,y,w,h,angle*16,spanAngle*16);
		 angle=angle+spanAngle;		
	}
	
	angle=this->_startAngle; 
	for(int i=0; i<this->_sectorNum;i++)
	{
		QFontMetrics fm(aFont);
		QSize size = fm.size(0,this->_text[i]);
		float cangle=(angle+spanAngle/2.0)/180.0*3.1415926;
	    
		if(this->_disables[i])
		{
			painter->setPen( QColor(128,128,128,250) );
		}
		else painter->setPen( QColor(240,240,240,250) );		
		
		float cx=this->_pos.x()-bubblePos.x()+cos(cangle)*this->_R*0.6, cy=this->_pos.y()-bubblePos.y()-sin(cangle)*this->_R*0.6;	

		if( (cangle>3.1415926*0.25 && cangle<=3.1415926*0.75) || (cangle>3.1415926*1.25 && cangle<=3.1415926*1.75) )
		{
			cx=this->_pos.x()-bubblePos.x()+cos(cangle)*this->_R*0.65, cy=this->_pos.y()-bubblePos.y()-sin(cangle)*this->_R*0.65;	
			if(cangle>3.1415926*0.5 && cangle<3.1415926*1.5)
				cx=cx-8;
			else
				cx=cx+8;
		}

		QSize bSize=fm.size(0,"Shape"),aSize=fm.size(0,this->_text[i]);
		if(aSize.width()<bSize.width())
			painter->setFont(aFont);
		else painter->setFont(aFont2);
		
		/*QFontMetrics fm(aFont);
		QSize size = fm.size(0,this->_text[i]);
		QSize bSize=fm.size(0,"Shape"),aSize=fm.size(0,this->_text[i]);
		if(aSize.width()<bSize.width())

		QTextDocument doc;
			doc.setHtml(nodeName);
			fontsize=fontsize+1;	
			do
			{
				fontsize=fontsize-1;	
				QFont f("Arial",fontsize);		
				doc.setDefaultFont(f);
				doc.setTextWidth( rect.width() );
				size = doc.size();	  
			}while(size.width()>rect.width() && fontsize>2);
		
		
		QFont aFont2("Arial", fontsize, QFont::Normal);
		int fontsize=11*aSize.width()/bSize.width();*/
		
		
		painter->drawText(cx-size.width()/2.0, cy-size.height()/2.0 + 10, this->_text[i]);	
		
		painter->setFont(aFont);
		if(i==this->_currentSector&&!this->_disables[i])
        {
			 
			 if(this->_text2[i].size()>0)
		     {
				float spanAngle2=spanAngle/this->_text2[i].size();
				for(int j=0; j<this->_text2[i].size(); j++)
				{
					float cangle2=(angle+spanAngle2*j+spanAngle2/2.0)/180.0*3.1415926;
					float cx,cy;

					if(this->_disables2[i][j])
					{
						painter->setPen( QColor(128,128,128,250) );
					}
					else painter->setPen( QColor(240,240,240,250) );		

					painter->save();	
					
					painter->setFont(aFont);
					aSize=fm.size(0,this->_text2[i][j]);
					if(aSize.width()<bSize.width())
					   painter->setFont(aFont);
					else painter->setFont(aFont2);
					size = fm.size(0, this->_text2[i][j]);
					
					if(cangle2>3.1415926*0.5 && cangle2<=3.1415926*1.0)
					{	
						if(size.width()>35)
						    cx=this->_R*0.9, cy=this->_R*0.9; //for short text
						else 
							cx=this->_R*1.2, cy=this->_R*1.2; //for long text	

						painter->translate(this->_pos.x()-bubblePos.x(), this->_pos.y()-bubblePos.y());
						painter->rotate(180-cangle2/3.1415926*180);
					    painter->drawText(-cx-size.width(), 0, this->_text2[i][j]);				
					}
					else if(cangle2>3.1415926*1.0 && cangle2<=3.1415926*1.5)
					{	
						//cx=this->_R*1.3, cy=this->_R*1.3;	(for long text)						
						if(size.width()>35)
						   cx=this->_R*1.2, cy=this->_R*1.2; //for short text
						else 
							cx=this->_R*1.2, cy=this->_R*1.2; //for long text

						painter->translate(this->_pos.x()-bubblePos.x(), this->_pos.y()-bubblePos.y());
						painter->rotate(180-cangle2/3.1415926*180);
					    painter->drawText(-cx-size.width(), 0, this->_text2[i][j]);				
					}
					else 
					{
						//if(cangle2<3.1415926*1.5)
						//cx=this->_pos.x()-bubblePos.x()+cos(cangle2)*this->_R*0.8, cy=this->_pos.y()-bubblePos.y()-sin(cangle2)*this->_R*0.8;	
						//else 						
						//cx=this->_pos.x()-bubblePos.x()+cos(cangle2)*this->_R*1.0, cy=this->_pos.y()-bubblePos.y()-sin(cangle2)*this->_R*1.0;	//for long text
						cx=this->_pos.x()-bubblePos.x()+cos(cangle2)*this->_R*1.2, cy=this->_pos.y()-bubblePos.y()-sin(cangle2)*this->_R*1.2;	//for short text
						
						painter->translate(cx, cy);
						painter->rotate(-cangle2/3.1415926*180);
						painter->drawText(0, 0, this->_text2[i][j]);				
					}					
					painter->restore();		   
				}
		   }
		
		}		
		//painter->drawText(cx-size.width()/2.0, cy-size.height()/2.0, this->_text[i]);			
		angle=angle+spanAngle;	
	}
	//draw boundary shadow		
	//draw text		
	return;    
}


QVector<QString> PieMenuItem::getText()
{
	return this->_text;
}


int PieMenuItem::getSectorNum()
{
  return this->_sectorNum;
}

float PieMenuItem::getStartAngle()
{
   return this->_startAngle;
}


float PieMenuItem::getIntervalAngle()
{
	return this->_intervalAngle;
}

void PieMenuItem::setPieMenu(QPointF pos, float R, QVector<QString> text, QVector<QVector<QString>> text2, QVector<QColor> color, QVector<QVector<QColor>> color2, vector<bool> disables, vector<vector<bool>> disables2)
{//text 1 the menu level 1, text 2 menu level 2, if text2 is empty, then only one level menu
	this->_pos=pos;
	this->_R=R;
	this->_text=text;
	this->_text2=text2;
	this->_currentSector=-1;
	this->_sectorNum = this->_text.size();	
	this->_disables=disables; 
	this->_disables2=disables2; 
	
	this->_color=color; 
	this->_color2=color2; 

	if(this->_sectorNum==0) return;	
	this->_intervalAngle = 360.0/this->_sectorNum;
	this->_startAngle = 90.0 -this->_intervalAngle/2.0;	
}
	
bool PieMenuItem::inPie(QPointF curPos)
{
	float x0=this->_pos.x(), y0=this->_pos.y(), x1=curPos.x(), y1=curPos.y();
    if(pow(x1-x0,2)+pow(y1-y0,2)<=pow(this->_R, 2))
		return true;
	else return false;
}

bool PieMenuItem::inPie2(QPointF curPos)
{
	float x0=this->_pos.x(), y0=this->_pos.y(), x1=curPos.x(), y1=curPos.y();
    if(pow(x1-x0,2)+pow(y1-y0,2)<=pow(this->_R*2, 2))
		return true;
	else return false;
}

void PieMenuItem::testSignal()
{
    //emit test();
	int a=0;
	a++;
}

int PieMenuItem::getSectorID(QPointF curPos)
{    
	float x0=this->_pos.x(), y0=this->_pos.y(), x1=curPos.x(), y1=curPos.y();
	if(inPie(curPos))
	{
	    float angle=atan2(y0-y1, x1-x0)/3.1415926*180;
		if(angle<this->_startAngle)
			angle  = angle + 360;
		angle=angle-this->_startAngle;
		return(angle/this->_intervalAngle);
	}
	return -1;
}

vector<int> PieMenuItem::getSectorID2(QPointF curPos)
{    
	float x0=this->_pos.x(), y0=this->_pos.y(), x1=curPos.x(), y1=curPos.y();
	vector<int> sectorid;
	if(inPie(curPos))
	{
		sectorid.push_back(-1); sectorid.push_back(-1);
		return sectorid;
	}
	else if(inPie2(curPos))
	{
	    float angle=atan2(y0-y1, x1-x0)/3.1415926*180;
		if(angle<this->_startAngle)
			angle  = angle + 360;
		angle=angle-this->_startAngle;
		int sector = (angle/this->_intervalAngle);
		if(this->_text2[sector].empty())
		{
			sectorid.push_back(-1); sectorid.push_back(-1);
			return sectorid;
		}
		float intervalAngle2=this->_intervalAngle/this->_text2[sector].size();
		int sector2 = ( angle - sector * this->_intervalAngle ) / intervalAngle2;
		sectorid.push_back(sector); sectorid.push_back(sector2);
		return sectorid;
	}
	sectorid.push_back(-1); sectorid.push_back(-1);
	return sectorid;
}

vector<bool> PieMenuItem::getDisables()
{
	return this->_disables;
}

vector<vector<bool>> PieMenuItem::getDisables2()
{
	return this->_disables2;
}

void PieMenuItem::setSectorID(QPointF curPos)
{    
	this->_currentSector=getSectorID(curPos);	
	if(this->_currentSector<0)
	{
		vector<int> id=getSectorID2(curPos);
		this->_currentSector=id[0];
	}	
}

void PieMenuItem::setSectorID2(QPointF curPos)
{    
	vector<int> id=getSectorID2(curPos);	
	this->_currentSector2= id[1];
}

QPointF PieMenuItem::getPos()
{
	return this->_pos;
}

bool PieMenuItem::mouseOver( QPointF pos )
{
	this->_curState = inPie( pos );
	return _curState;
}

int PieMenuItem::getType()
{
	return this->_TYPE;
}

QString PieMenuItem::toQString()
{
	return QObject::tr( "Undefined toString()" );
}

void PieMenuItem::click()
{
	return;
}

void PieMenuItem::release()
{
	return;
}