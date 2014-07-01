#include "SymbolColorPanel.h"
#include "openglscene.h"

#define RATELIMITCOLOR  QColor(255, 160, 160,255)
#define CROSSTALKCOLOR  QColor(160, 160, 255, 255)
#define ORTHOLOGYCOLOR  QColor(102,194,165, 255)

#define DOWNEXPRESSIONCOLOR QColor(64,64,255,255)
#define UPEXPRESSIONCOLOR QColor(230,68,113,255)
#define EXPRESSIONBACKGROUNDCOLOR QColor(255,237,160,255)
#define HIGHLIGHTCOLOR QColor(120, 130, 255, 127)

#define COMPLETECOLOR QColor(0,90,50) 
#define INCOMPLETECOLOR QColor(161,217,155) 	
#define MISSINGCOLOR QColor(255,255,255,255)

#define PROTEINCOLOR QColor(255,255,204,255) 
#define COMPLEXCOLOR QColor(255,226,183,255) 	
#define SMALLMOLECULECOLOR QColor(214,215,202,255)
#define PHYSICALENTITYCOLOR QColor(230,234,172,255)
#define DNACOLOR QColor(214,234,172,255)
#define SETCOLOR QColor(254,202,131,255)

SymbolColorPanel::SymbolColorPanel( QSizeF size, int px, int py, OpenGLScene* scene, OpenManager* open )
: BarBase( size, px, py )
{
	QSizeF s=this->getSize();
	int SX=s.width()-80, SY=30;
	hideButton=QRect(SX,SY,50,50);
	hideIt=false;
}


QString SymbolColorPanel::tailofLogMsg()
{
	QString logtext;
	logtext.append( " (Model-Composition-View Bubble)\n" );
	return logtext;
}

void SymbolColorPanel::drawAFan(QPainter *painter, QPoint center, float radius1, float radius2, float angle1, float angle2, QColor color, int shapeType)
{//shapeType 0: fan
//shape type 1: Mesh
	if(radius1>=radius2)
		return;

	
	if(angle1>=angle2)
		return;
	
	static const double Pi = 3.141592; 
    static double TwoPi = 2.0 * Pi;
	float grad=(angle2-angle1)/10;
	float rd=radius2-radius1;
	int num=8;
	QPointF avg;
	float ax=0, ay=0;
	QVector<QPointF> points;
	int tnum=(num+1)*2;
	points.resize(tnum);
    for(int i=0; i<=num;  i++)
	{
	    float x,y;
		x=radius2*cos(angle1+i*grad);
		y=-radius2*sin(angle1+i*grad);
		points[i]=QPoint(x,y);
		ax=ax+x; ay=ay+y;
	}
	if(shapeType==1)
		radius1=radius2;

	for(int i=0; i<=num;  i++)
	{
	    float x,y;
		x=radius1*cos(angle1+i*grad);
		y=-radius1*sin(angle1+i*grad);
		if(shapeType==1)
			y=y+rd;
		points[tnum-1-i]=QPoint(x,y);
		ax=ax+x; ay=ay+y;


	}

	avg=QPointF(ax/tnum,ay/tnum); 
	QPointF Dis=center-avg;

	/*for(int i=0; i<v.size(); i=i+2)
	{	
		points.push_back(QPoint(v[i+1].x, v[i+1].y));						
	}
	for(int i=v.size()-2; i>=0; i=i-2)
	{	
		points.push_back(QPoint(v[i].x, v[i].y));						
	}*/

	for(int i=0; i<points.size(); i++)
	{
	    points[i]=points[i]+Dis;
	}

	QPolygonF whole(points);
	painter->setBrush(color);
	painter->drawPolygon(whole);
		
}

QRegion SymbolColorPanel::roundRect(QRect &rect, int radius)
{
	QRegion reg;

	reg += rect.adjusted( radius, 0, -radius, 0 );
	reg += rect.adjusted( 0, radius, 0, -radius );

 	QRect cc(rect.topLeft(), QSize(2*radius, 2*radius));
 	QRegion corner( cc, QRegion::Ellipse );
 	reg += corner;
 	corner.translate( rect.width()-2*radius, 0 );
 	reg += corner;
 	corner.translate( 0, rect.height()-2*radius);
 	reg += corner;
 	corner.translate( -rect.width()+2*radius, 0 );
 	reg += corner;

	return reg;
}
void SymbolColorPanel::drawArrow_4(QPainter *painter, QPointF start, QPointF end, float width, float height, QColor color)
{//doted line
	QPointF temp;
	temp = start; start =end; end =temp;	
	QLineF line(start, end);	
	float ds = line.length(); //sqrt(line.dx()*line.dx() + line.dy()*line.dy());  
	
	float size=1+4*(width+height)/600;	
	//qreal arrowSize(size);

	float itv;
	float dotSize=size*2.6;
    int count=ds/dotSize;
	if(count%2!=1)
		count=count-1;
	
	if(count<3)
		count=3;
	
	count=count*2;
	itv=ds/count;
	
	bool flag=true;
	if(size/ds<0.2)
	   flag=false;

	float l1,l2, as1, as2, scale;
	QPointF source, dest;
	for(int i=0; i<count; i=i+2)
	{
		l1=i*itv;
		scale=l1/ds;
		as1=size*scale;
		source= end + (start-end)*scale;

		l2=(i+1)*itv;
		scale=l2/ds;
		as2=size*scale;
		dest= end + (start - end)*scale;
		//drawATrapezoid(painter, source, dest, as1, as2, color);		
		int w=(as1+as2)/2;
		if(w<1)
			w=1;
		painter->setPen(QPen(color, w)); //; Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
		painter->drawLine(source, dest);		
	}	
		
}


void SymbolColorPanel::drawPathwayReactionTypes(QPainter *painter, int sx , int sy)
{
	int itv;	
	QFont f1("Arial", 10);
	painter->setFont (f1);	
	QString oName, eName;
	int addLength=0;
   
   
	itv=19;
	
	painter->drawText(sx+0, sy+0, "Reaction Types" );
	int grad=3;
	{
		for(int i=0; i<grad; ++i)
		{
			QString str;
			int SX,SY;
			SX= sx + 0;
			SY= sy + 13;
			 painter->setPen(QColor(50,50,50,255));	
			if(i==2)
			{  
				//painter->setBrush(QColor(255, 180, 180,255));
				str= "Dissociation";
			}	
			if(i==1)
			{  
				//painter->setBrush(QColor(255, 180, 180,255));
				str="Association";
			}			
			else if(i==0)
			{
				//painter->setBrush(QColor(180, 180, 255, 255)); 
				str="Transition";
			}
			painter->drawText(SX+itv*5-3, SY + itv*(i+0.6), str);	
			
			//painter->drawRect(rect);			

			painter->setBrush(QColor(0,0,0,255));
			painter->setPen(Qt::NoPen);

			QVector<QPointF> points;
			points.resize(3);
						
			points[0]=QPointF(SX+itv*(0), SY+itv*(i+0.2)+4);
			points[1]=QPointF(SX+itv*(2)+6, SY+itv*(i+0.2));
			points[2]=QPointF(SX+itv*(0), SY+itv*(i+0.2)-3);

			QPolygonF whole1(points);			
			painter->drawPolygon(whole1);

			points[0]=QPointF(SX+itv*(2)+4, SY+itv*(i+0.2)+4);
			points[1]=QPointF(SX+itv*(4)+10, SY+itv*(i+0.2));
			points[2]=QPointF(SX+itv*(2)+4, SY+itv*(i+0.2)-3);

			QPolygonF whole2(points);			
			painter->drawPolygon(whole2);

			//edge
			painter->setPen(QColor(0,0,0,255));
			painter->setBrush(QColor(255, 255, 255, 255)); 
			//reaction node
			QRect rect(SX+itv*(2)-6, SY+itv*(i+0.2)-6, 12, 12);
			if(i==2)
			{  
				drawDissociation(painter, rect);
				//str= "Dissociation";
			}	
			if(i==1)
			{  
				rect = QRect(SX+itv*(2)-7, SY+itv*(i+0.2)-7, 14, 14);
				painter->drawEllipse( rect);
				//str= "Association";
			}			
			else if(i==0)
			{
				painter->drawRect( rect );
				//str= "Transition";
			}			
		}		
	}
}

void SymbolColorPanel::drawDissociation(QPainter *painter, QRectF rect)
{	
	QVector<QPoint> points;	

    float w=rect.width(), h=rect.height();
	float cx=rect.center().x(), cy=rect.center().y();
	
	points.append(QPoint(cx-w*0.6,cy+h*0.6));
	points.append(QPoint(cx+w*0.6,cy+h*0.6));
	points.append(QPoint(cx,cy-h*0.6));
	
	QPolygon whole(points);
	painter->drawPolygon(whole);
}

void SymbolColorPanel::drawExpressionGlyphs(QPainter *painter, int sx , int sy)
{
	int itv;	
	QFont f1("Arial", 10);
	painter->setFont (f1);	
	QString oName, eName;
	int addLength=0;
   
    painter->setPen(QColor(50,50,50,255));	
	itv=19;
	
	painter->drawText(sx+0, sy+0, "Expression" );
	int grad=3;
	{
		for(int i=0; i<grad; ++i)
		{
			QString str;
			int SX,SY;
			SX= sx + 0;
			SY= sy + 13;			
			QRect rect(SX, SY+itv*i, 16, itv-3);			
			if(i==0)
			{
				//painter->drawText(sx, sy-itv*(grad-0.55), "rpkm");  //Bar Chart
				str= "RPKM (reads per kilobase per million)";				
				painter->drawText(SX, SY + itv*(i+0.6), str);	
			}
			else if(i==1)
			{
				painter->setBrush(UPEXPRESSIONCOLOR);
				str= "Up-expressed";
				painter->drawRect(rect);
				painter->drawText(SX+itv+2, SY + itv*(i+0.6), str);		
			}
			else if(i==2)
			{  
				painter->setBrush(DOWNEXPRESSIONCOLOR);
				str= "Down-expressed";
				painter->drawRect(rect);
				painter->drawText(SX+itv+2, SY + itv*(i+0.6), str);		
				
			}			
		}		
	}
	
	/*sx=sx+140;
	grad=3;
	{
		for(int i=0; i<grad; ++i)
		{
			QString str;
			int SX,SY;
			SX= sx + 0;
			SY= sy + 13;
			
			QRect rect(SX, SY+itv*i, 16, itv-3);
			
			if(i==0)
			{
				//painter->drawText(sx, sy-itv*(grad-0.55), "rpkm");  //Bar Chart
				//str= "rpkm";				
				//painter->drawText(SX+itv+5, SY + itv*(i+0.6), str);	
			}
			else if(i==1)
			{
				painter->setBrush(QColor(0,0,0,255));
				str= "Unchanged";
				painter->drawRect(rect);
				painter->drawText(SX+itv+2, SY + itv*(i+0.6), str);		
			}
			else if(i==2)
			{   painter->setBrush(QColor(188,188,188,255)); //htColor[l] = QColor(81,81,255,255);htColor[l] = QColor(230,68,113,255);QColor(255,100,100,255);QColor(206,206,206,228);
				str= "Missing in Pathways";
				painter->drawRect(rect);
				painter->drawText(SX+itv+2, SY + itv*(i+0.6), str);						
			}			
		}		
	}*/

	/*sx=sx+160;
	grad=3;
	{
		for(int i=0; i<grad; ++i)
		{
			QString str;
			int SX,SY;
			SX= sx + 0;
			SY= sy + 13;
			
			QRect rect(SX, SY+itv*i, 16, itv-3);
			
			if(i==0)
			{
				//painter->drawText(sx, sy-itv*(grad-0.55), "rpkm");  //Bar Chart
				str= "Background";				
				painter->drawText(SX, SY + itv*(i+0.6), str);	
			}
			else if(i==1)
			{
				painter->setBrush(EXPRESSIONBACKGROUNDCOLOR);
			    str= "Expressed";
				painter->drawRect(rect);
				painter->drawText(SX+itv+2, SY + itv*(i+0.6), str);	
			}
			else if(i==2)
			{  
				painter->setBrush(QColor(255,255,255,255));
			    str= "Unchanged";
				painter->drawRect(rect);
				painter->drawText(SX+itv+2, SY + itv*(i+0.6), str);	
			}			
		}		
	}*/
	
	/*sx=sx+120;
	grad=2;
	{
		for(int i=0; i<grad; ++i)
		{
			QString str;
			int SX,SY;
			SX= sx + 0;
			SY= sy + 13;
			
			QRect rect(SX, SY+itv*i, 16, itv-3);
			
			if(i==0)
			{
				//painter->drawText(sx, sy-itv*(grad-0.55), "rpkm");  //Bar Chart
				//str= "Background";				
				//painter->drawText(SX+itv+5, SY + itv*(i+0.6), str);	
			}
			else if(i==1)
			{
				painter->setBrush(QColor(206,206,206,228));// painter->setBrush(Qt::NoBrush); //htColor[l] = QColor(81,81,255,255);htColor[l] = QColor(230,68,113,255);QColor(255,100,100,255);QColor(206,206,206,228);
			    str= "not found";
				painter->drawRect(rect);
				painter->drawText(SX+itv+2, SY + itv*(i+0.6), str);		
			}	
		}		
	}	*/	
}

void SymbolColorPanel::drawGlyphTemplat(QPainter *painter, int sx , int sy)
{
	int itv;	
	QFont f1("Arial", 10);
	painter->setFont (f1);	
	QString oName, eName;
	int addLength=0;
   
    painter->setPen(QColor(50,50,50,255));	
	itv=19;
	
	painter->drawText(sx+0, sy+0, "Protein Attributes" );
	int grad=3;
	{
		for(int i=0; i<grad; ++i)
		{
			QString str;
			int SX,SY;
			SX= sx + 0;
			SY= sy + 13;
			
			QRect rect(SX, SY+itv*i, 16, itv-3);
			if(i==2)
			{
				painter->setBrush(ORTHOLOGYCOLOR); 
				str= "Orthologous";
			}
			else if(i==1)
			{  
				painter->setBrush(RATELIMITCOLOR);
				str= "Rate-limited";
			}			
			else if(i==0)
			{
				painter->setBrush(CROSSTALKCOLOR); 
				str= "Cross-talking";
			}			
			painter->drawRect(rect);
			painter->drawText(SX+itv+5, SY + itv*(i+0.6), str);					
		}		
	}

}

void SymbolColorPanel::drawPathwayReactionAttrubutes(QPainter *painter, int sx , int sy)
{
	int itv;	
	QFont f1("Arial", 10);
	painter->setFont (f1);	
	QString oName, eName;
	int addLength=0;
   
    painter->setPen(QColor(50,50,50,255));	
	itv=19;
	
	painter->drawText(sx+0, sy+0, "Reaction Attributes" );
	int grad=2;
	{
		for(int i=0; i<grad; ++i)
		{
			QString str;
			QPointF p1,p2;
			int SX,SY;
			SX= sx + 0;
			SY= sy + 13;
			
			//QRect rect(SX, SY+itv*(i+0.5), 16, itv-3);
			p1=QPointF(SX-7, SY+itv*(i+0.35));
			p2=QPointF(SX+itv*2.7+3, SY+itv*(i+0.35));		

			if(i==1)
			{				
				drawArrow_4(painter, p2, p1, 600, 600, QColor(0, 210, 50, 255) );	
				str= "Activation";				
			}
			else if(i==0)
			{  				
				drawArrow_4(painter, p2, p1, 600, 600, QColor(255, 128, 0, 255) );	
				str= "Inhibition";				
			}
			//painter->drawRect(rect);
			 painter->setPen(QColor(50,50,50,255));	
			painter->drawText(SX+70, SY + itv*(i+0.6), str);					
		}		
	}	
}

void SymbolColorPanel::drawPathwayNodeTypes(QPainter *painter, int sx , int sy)
{
	int itv;	
	QFont f1("Arial", 10);
	painter->setFont (f1);	
	QString oName, eName;
	int addLength=0;
   
    painter->setPen(QColor(50,50,50,255));	
	itv=19;
		
	painter->drawText(sx+0, sy+0, "Pathway Component Types" );
	int grad=3;
	{
		for(int i=0; i<grad; ++i)
		{
			QString str;
			int SX,SY;
			SX= sx + 0;
			SY= sy + 13;
			QRect rect1(SX, SY+itv*i-2, 36, 16);
			QPoint center=rect1.center();
			if(i==0)
			{
				
				painter->setBrush(PROTEINCOLOR); 
				painter->drawRoundedRect( rect1, 8, 5 );   
				str= "Protein";
			}
			else if(i==1)
			{  
				QRect rect(SX, SY+itv*i, 18, 10);
				rect.moveCenter(center);
				painter->setBrush(COMPLEXCOLOR);
				painter->drawRect(rect);
				str= "Complex";
			}
			else if(i==2)
			{  
				QRect rect(SX, SY+itv*i, 18, 14);
				rect.moveCenter(center);
				painter->setBrush(SMALLMOLECULECOLOR);
				painter->drawEllipse( rect);    	
				str= "Small Molecule";
			}
			//painter->drawRect(rect);
			painter->drawText(SX+itv+20, SY + itv*(i+0.6), str);					
		}		
	}	

	sx=sx+140;
	grad=3;
	{
		for(int i=0; i<grad; ++i)
		{
			QString str;
			int SX,SY;
			SX= sx + 0;
			SY= sy + 13;			
			QRect rect1(SX, SY+itv*i-2, 32, 18);
			QPoint center=rect1.center();
			if(i==0)
			{  
				painter->setBrush(PHYSICALENTITYCOLOR);
				float w,w2,h,h2;
				w=itv/2*1.3, h=itv/2*1.3;
				w2=w-w*0.4; h2=h-h*0.4;
				const QPointF points[8] = 
				{
					QPointF(center.x()-w2, center.y()-h),
					QPointF(center.x()-w, center.y()-h2),		
					QPointF(center.x()-w, center.y()+h2),
					QPointF(center.x()-w2, center.y()+h),
					QPointF(center.x()+w2, center.y()+h),
					QPointF(center.x()+w, center.y()+h2),
					QPointF(center.x()+w, center.y()-h2),
					QPointF(center.x()+w2, center.y()-h)
				}; 
				painter->drawPolygon(points, 8);
				str= "Physical Entity";
			}
			if(i==1)
			{  
				QRect rect(SX, SY+itv*i, 18, 14);
				rect.moveCenter(center);
				painter->setBrush(DNACOLOR);
				painter->drawEllipse( rect);    					
				str= "DNA";
			}			
			else if(i==2)
			{
				painter->setBrush(SETCOLOR); 
				float w, h;				
				w=itv/2*1.1, h=itv/2;				
				const QPointF point6[6] = 
				{
					QPointF(center.x()-w/1.5, center.y()-h),
					QPointF(center.x()-w, center.y()),		
					QPointF(center.x()-w/1.5, center.y()+h),
					QPointF(center.x()+w/1.5, center.y()+h),
					QPointF(center.x()+w, center.y()),	
					QPointF(center.x()+w/1.5, center.y()-h),
				}; 				
				painter->drawPolygon(point6, 6);		
				str= "Set";
			}
			//painter->drawRect(rect);
			painter->drawText(SX+itv+15, SY + itv*(i+0.6), str);					
		}		
	}	
	    /*
				painter->drawRoundedRect( rect, 6, 6 );   
		        break; 
		case 'C':			
				painter->drawPolygon(points, 8);
				break;
		case 'S':  
		   	
				painter->drawEllipse( rect);    			  
				break;
		case 'D':   
		   	
				painter->drawEllipse( rect );  		   
		         break; 
		case 'R':
			   if(reactionType=="D") // dissociation
			   {
                   drawDissociation(painter, rect);	    
			   }
			   // binding
			   else if(reactionType=="B")
			   {
				   painter->drawEllipse( rect);
			   }
			   else if(reactionType=="T")
			   {   
			       painter->drawRect( rect );
			   }  		      		   
			   break;
		case 'L':			 
			  painter->drawPolygon(point6, 6);			
			  break;
		case 'E':  
			  painter->drawPolygon(points, 8); break;		   
		      break;
	*/
}



void SymbolColorPanel::drawPathwayProteinAttributes(QPainter *painter, int sx , int sy)
{
	int itv;	
	QFont f1("Arial", 10);
	painter->setFont (f1);	
	QString oName, eName;
	int addLength=0;
   
    painter->setPen(QColor(50,50,50,255));	
	itv=19;
	
	painter->drawText(sx+0, sy+0, "Protein Attributes" );
	int grad=3;
	{
		for(int i=0; i<grad; ++i)
		{
			QString str;
			int SX,SY;
			SX= sx + 0;
			SY= sy + 13;
								
			 		
			//pen.setColor(boundaryColor);
			//pen.setWidthF(boundaryWidth);
	        //painter->setPen(pen);
			
			painter->setBrush(QColor(255,255,204,255));
			QRect rect(SX, SY+itv*i-2, 36, 16);
			if(i==2)
			{
				painter->drawRoundedRect( rect, 6, 6 ); 
				float dx=6;
			    float x= rect.x()+rect.width()-dx;
				
				QPainterPath path;
						
				QRect cRect = QRect(x, rect.y(), dx, rect.height());						 
				QRect rRect = QRect(rect.x(), rect.y(), rect.width(), rect.height());
				QRegion cRegion = cRect;
				QRegion rRegion = roundRect(rRect, 6);

				path.addRegion(cRegion.intersect(rRegion));
				painter->fillPath( path, QBrush( ORTHOLOGYCOLOR) );				
				str= "Orthologous";

				painter->setBrush(Qt::NoBrush);
			    painter->drawRoundedRect( rect, 8, 5 ); 
				
			}
			else if(i==1)
			{  
				painter->drawRoundedRect( rect, 6, 6 ); 
				float dx=6;
			    float x= rect.x()+rect.width()-dx;
				
				QPainterPath path;
						
				QRect cRect = QRect(x, rect.y(), dx, rect.height());						 
				QRect rRect = QRect(rect.x(), rect.y(), rect.width(), rect.height());
				QRegion cRegion = cRect;
				QRegion rRegion = roundRect(rRect, 6);

				path.addRegion(cRegion.intersect(rRegion));
				painter->fillPath( path, QBrush( RATELIMITCOLOR) );				
				str= "Rate-limited";

				painter->setBrush(Qt::NoBrush);
			    painter->drawRoundedRect( rect, 8, 5 );  
			}			
			else if(i==0)
			{
				painter->drawRoundedRect( rect, 6, 6 ); 
				float dx=6;
			    float x= rect.x()+rect.width()-dx;
				
				QPainterPath path;
						
				QRect cRect = QRect(x, rect.y(), dx, rect.height());						 
				QRect rRect = QRect(rect.x(), rect.y(), rect.width(), rect.height());
				QRegion cRegion = cRect;
				QRegion rRegion = roundRect(rRect, 6);

				path.addRegion(cRegion.intersect(rRegion));
				painter->fillPath( path, QBrush(CROSSTALKCOLOR) );							
				str= "Cross-talking";

				painter->setBrush(Qt::NoBrush);
			    painter->drawRoundedRect( rect, 8, 5 );  
			}
			//painter->drawRoundedRect( rect, 8, 5 ); 
			
			//painter->drawRect(rect);
			painter->drawText(SX+itv+22, SY + itv*(i+0.6), str);					
		}		
	}	
}

void SymbolColorPanel::drawPathwayNodeAttributes(QPainter *painter, int sx , int sy)
{
	int itv;	
	QFont f1("Arial", 10);
	painter->setFont (f1);	
	QString oName, eName;
	int addLength=0;
   
    painter->setPen(QColor(50,50,50,255));	
	itv=19;

	int grad=2;
	{
		for(int i=0; i<grad; ++i)
		{
			QString str;
			int SX,SY;
			SX= sx + 0;
			SY= sy + 13;
			 		
			//pen.setColor(boundaryColor);
			//pen.setWidthF(boundaryWidth);
	        //painter->setPen(pen);			
			painter->setBrush(QColor(255,255,204,255));
			QRect rect(SX, SY+itv*i-2, 36, 16);
			
			 if(i==0)
			{
				painter->drawRoundedRect( rect, 6, 6 ); 
				float dx=6;
			    float x= rect.x();//+rect.width()-dx;
				
				QPainterPath path;
						
				QRect cRect = QRect(x, rect.y(), dx, rect.height());						 
				QRect rRect = QRect(rect.x(), rect.y(), rect.width(), rect.height());
				QRegion cRegion = cRect;
				QRegion rRegion = roundRect(rRect, 6);

				path.addRegion(cRegion.intersect(rRegion));
				painter->fillPath( path, QBrush(UPEXPRESSIONCOLOR) );							
				str= "Up-expressed";

				painter->setBrush(Qt::NoBrush);
			    painter->drawRoundedRect( rect, 8, 5 );  
			}
			else if(i==1)
			{  
				painter->drawRoundedRect( rect, 6, 6 ); 
				float dx=6;
			    float x= rect.x();//+rect.width()-dx;
				
				QPainterPath path;
						
				QRect cRect = QRect(x, rect.y(), dx, rect.height());						 
				QRect rRect = QRect(rect.x(), rect.y(), rect.width(), rect.height());
				QRegion cRegion = cRect;
				QRegion rRegion = roundRect(rRect, 6);

				path.addRegion(cRegion.intersect(rRegion));
				painter->fillPath( path, QBrush( DOWNEXPRESSIONCOLOR) );				
				str= "Down-expressed";

				painter->setBrush(Qt::NoBrush);
			    painter->drawRoundedRect( rect, 8, 5 );  
			}			
			
			//painter->drawRoundedRect( rect, 8, 5 ); 
			
			//painter->drawRect(rect);
			painter->drawText(SX+itv+22, SY + itv*(i+0.6), str);					
		}		
	}	
	
	//painter->drawText(sx+0, sy+0, "Expressed Nodes" );
	/*int grad=2;
	{
		for(int i=0; i<grad; ++i)
		{
			QString str;
			int SX,SY;
			SX= sx + 0;
			SY= sy + 13;
			
			QRect rect(SX, SY+itv*i, 16, itv-3);
			if(i==1)
			{  
				painter->setBrush(QColor(81,81,255,255));
				str= "Down-expressed";
			}
			else if(i==0)
			{
				painter->setBrush(QColor(230,68,113,255)); 
				str= "Up-expressed";
			}
			painter->drawRect(rect);
			painter->drawText(SX+itv+5, SY + itv*(i+0.6), str);					
		}		
	}*/	
}

void SymbolColorPanel::drawOrthologyPanel(QPainter *painter, int sx , int sy)
{
	int itv=19;
	QColor color;
	QFont f1("Arial", 10);
	painter->setFont (f1);	
	//int sx=width-30, sy=height;
	painter->drawText(sx+0, sy+0, "Orthology Completeness" );
	for(int i=0; i<3; ++i)
	{
		int SX,SY;
		SX= sx + 0;
		SY= sy + 13;
		QString str;		
		float r, g, b;
		float value=(i+0.5)/3;
		//TDHelper::GetMultHue(value, 3, r, g, b); 
		//painter->setBrush(QColor(r, g, b,255));
		
		QRect rect(SX, SY+itv*i, 16, itv-3);
		if(i==2)
		{
			color= MISSINGCOLOR;
			//color=QColor(0,0,0,0);
			str= "Missing";
		}		
		else if(i==1)
		{
			color=INCOMPLETECOLOR;
			str= "Incomplect";
		}
		else
		{  
			color=COMPLETECOLOR;
			str= "Complete";
		}
		//three color
		painter->setBrush(color);
		//painter->drawRect(rect);
		drawAFan(painter, rect.center(), (itv-4)*1.2, (itv-4)*2, 3.1415*0.38, 3.1415*0.62, color,1);
		//QString str= QString::number((i+1)/8.0, 'g', 5);
		painter->drawText(SX+itv+5, SY+itv*(i+0.6), str);
	}			
}

void SymbolColorPanel::drawTreeRingNodePanel(QPainter *painter, int sx , int sy)
{
	int itv=19;
	QColor color;
	QFont f1("Arial", 10);
	painter->setFont (f1);	
	
	for(int i=0; i<1; ++i)
	{
		int SX,SY;
		SX= sx + 0;
		SY= sy + 13;
		QString str;		
		float r, g, b;
		float value=(i+0.5)/3;
			
		QRect rect(SX, SY+itv*i, 16, itv-3);
		if(i==0)
		{
			color=QColor(128, 177, 211,255);
			str= "Contain Rate-limited Proteins";
			painter->drawText(SX+itv+5, SY+itv*(i+0.6), str);
		}	
		drawAFan(painter, rect.center(), (itv-4)*1.2, (itv-4)*2, 3.1415*0.38, 3.1415*0.62, QColor(255,140,140,255),1);		
		/*drawAFan(painter, rect.center(), (itv-4)*1, (itv-4)*2, 3.1415*0.35, 3.1415*0.65, QColor(0,0,0,0));

		painter->setPen(Qt::NoPen);
		drawAFan(painter, rect.center()+ QPoint(0,itv/2-2), (itv-4)*1, (itv-4)*1.15, 3.1415*0.34, 3.1415*0.65, QColor(255,140,140));*/
		
		
	}			
}

void SymbolColorPanel::drawHideButton(QPainter *painter)
{
	painter->setPen(QColor(20,20,20,200));
	painter->setBrush(Qt::NoBrush);
	painter->drawRect(hideButton);	
	int x=hideButton.x(), y=hideButton.y();
	int w=hideButton.width(), h=hideButton.height();
	
	if(!hideIt)
	{

		//QRect rect=QRect(x+w*0.1, y+h*0.05, w*0.25, h*0.9);
		QVector<QPointF> points;
		points.resize(3);
						
		points[0]=QPointF(x+w*0.25, y+h*0.95);
		points[1]=QPointF(x+w*0.70, y+h*0.5);
		points[2]=QPointF(x+w*0.25, y+h*0.05);

		painter->setBrush(QColor(127, 127, 127, 200));
		QPolygonF whole1(points);			
		painter->drawPolygon(whole1);	
		//painter->drawRect(rect);	
	}
	else
    {

		//QRect rect=QRect(x+w*0.65, y+h*0.05, w*0.25, h*0.9);

		QVector<QPointF> points;
		points.resize(3);
						
		points[0]=QPointF(x+w*0.75, y+h*0.95);
		points[1]=QPointF(x+w*0.30, y+h*0.5);
		points[2]=QPointF(x+w*0.75, y+h*0.05);

		painter->setBrush(QColor(127, 127, 127, 200));
		QPolygonF whole1(points);			
		painter->drawPolygon(whole1);
		//painter->drawRect(rect);	
	}
}

bool SymbolColorPanel::isInHideButton(int x, int y)
{
	return hideButton.contains(QPointF(x,y));
}

void SymbolColorPanel::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	BarBase::paint( painter, option, widget );
	painter->save();

	//*********************************************************
	if(!hideIt)
	{
		drawOrthologyPanel(painter, 10, 15);
		drawTreeRingNodePanel(painter, 160, 15);
		drawPathwayNodeTypes(painter, 380 , 15);		
		drawPathwayProteinAttributes(painter, 680, 15);
		drawPathwayNodeAttributes(painter, 820, 15);
		drawPathwayReactionTypes(painter, 1000, 15);
		drawPathwayReactionAttrubutes(painter, 1180, 15);		
		drawExpressionGlyphs(painter, 1360, 15);
	}
	//*********************************************************
    drawHideButton(painter);

	//*********************************************************
	/*drawDropArea( painter );
	drawFileArea( painter );
	drawVisArea( painter );
	renderItem( painter );
	drawControl( painter );
	drawViews( painter );
	drawColorBar( painter );*/
	//*********************************************************
	painter->restore();
	//*********************************************************
	return;
}

void SymbolColorPanel::drawDropArea( QPainter *painter )
{
	/*painter->save();
	//*********************************************************
	painter->setPen( Qt::NoPen );
	painter->setBrush( QBrush( QColor( 0, 0, 60, 60) ) );
	painter->drawRect( this->getDorpArea() );
	//*********************************************************
	painter->restore();
	return;*/
}

void SymbolColorPanel::drawFileArea( QPainter *painter )
{
	/*painter->save();
	//*********************************************************
	this->_manager->renderRoot( painter, scaleArea(), getPaintArea()); 
	//*********************************************************
	painter->restore();
	return;*/
}

void SymbolColorPanel::drawVisArea( QPainter *painter )
{
	/*painter->save();
	//*********************************************************
	//*********************************************************
	painter->restore();
	return;*/
}

void SymbolColorPanel::initManager()
{
	/*this->_manager = new InstanceManager();
	//
	this->_init = new InstanceInitializer();
	this->_init->initInstance( this->_manager );
	//
	this->_manager->initTree();
	return;*/
}

QRectF SymbolColorPanel::getPaintArea()
{
	QRectF result( QPointF( 0, 0 ), QSize( this->boundingRect().width()/3, this->boundingRect().height() ) );
	return result;
}

QRectF SymbolColorPanel::getDorpArea()
{
	return QRectF(QPointF( this->boundingRect().width()/3, 0 ), QSize( this->boundingRect().width()/3, this->boundingRect().height() ) );
}

QRectF SymbolColorPanel::getVisArea()
{
	return QRectF(QPointF( 2 * this->boundingRect().width()/3, 0 ), QSize( this->boundingRect().width()/3, this->boundingRect().height() ) );
}

QRectF SymbolColorPanel::scaleArea()
{
	int ScaleFactor = this->getSize().height() * _heightPercent * 2;
	QRectF bounding = this->getPaintArea();
	bounding.setLeft( bounding.left() - ScaleFactor );
	bounding.setRight( bounding.right() + ScaleFactor );
	bounding.setTop( bounding.top() - ScaleFactor );
	bounding.setBottom( bounding.bottom() + ScaleFactor );
	bounding.translate( this->_FileTrans.x(), this->_FileTrans.y() );
	return bounding;
}

bool SymbolColorPanel::inFileView( QPointF pos )
{
	return this->getPaintArea().contains( pos );
}

bool SymbolColorPanel::inDropView(QPointF pos)
{
	return this->getDorpArea().contains( pos );
}

bool SymbolColorPanel::inVisView( QPointF pos )
{
	return this->getVisArea().contains( pos );
}

void SymbolColorPanel::renderItem( QPainter* painter )
{
	for( int i = 0; i < this->_lines.size(); i++ )
	{
		this->_lines[i]->paint( painter );
	}
}

void SymbolColorPanel::addNewToDrag()
{
	//			add to the drop area
	if( this->_lines.size() > 7 )
	{
		delete this->_lines.first();
		this->_lines.removeAt( 0 );
	}
	FileItem* item = new FileItem( _CurrentFileName, 1, _open );
	this->_lines.append( item );
	//

	//logDragFileToCompose( _CurrentFileName );

	reOrder();
	return;
}

void SymbolColorPanel::reOrder()
{
	int Height = 20;
	int Width = this->boundingRect().width()/3+10;
	for( int i = 0; i < this->_lines.size(); i++ )
	{
		QRectF curBounding = this->_lines[i]->boundingRect();
		if( Width + curBounding.width() > this->boundingRect().width()/3*2 )
		{
			Width = this->boundingRect().width()/3+10;
			Height += curBounding.height() + 5;
		}
		this->_lines[i]->setPos( QPoint( Width, Height ) );
		Width += curBounding.width() + 3;
	}
}

int SymbolColorPanel::handlePressDropView( QPointF pos )
{
	CLICKINDEX = -1;
	for( int i = 0; i < this->_lines.size(); i++ )
	{
		if (this->_lines[i]->contains( pos ) )
		{
			CLICKINDEX = i;
			this->_lines[i]->enablePreOpen();
			break;
		}
	}
	return CLICKINDEX;
}

void SymbolColorPanel::handleDragInDrop(QPointF pos)
{
	if( this->boundingRect().contains( pos ) )
	{
		this->_lines[CLICKINDEX]->drawPreOpen( pos, false );
	}
	else
	{
		QPointF ScenePos = this->boundingRect().topLeft() + pos;
		this->_lines[CLICKINDEX]->drawPreOpen( ScenePos, true );
	}
	return;
}

void SymbolColorPanel::drawControl( QPainter* painter )
{
	painter->save();
	//************************************
	painter->setPen( Qt::NoPen );
	if( !_HIGHLIGHTCONTROL )
	{
		painter->setBrush( QBrush( QColor( 10, 10, 60, 100 ) ) );
	}
	else
	{
		painter->setBrush( QBrush( QColor( 10, 10, 60, 200 ) ) );
	}
	painter->drawRect( _Control );
	painter->setPen( Qt::RoundJoin );
	painter->setBrush( QBrush( QColor( 150, 150, 150, 100 ) ) );
	painter->setPen( QColor( 150, 150, 150, 100 ) );
	painter->drawRect( _EllipseMap );
	painter->drawRect( _EllipseColor );
	painter->drawRect( _EllipseMove );
	painter->setPen( QColor( 250, 250, 250, 200 ) );
	painter->drawText( this->_EllipseMap.bottomLeft() + QPointF( 5, -5 ), "W" );
	painter->drawText( this->_EllipseMove.bottomLeft() + QPointF( 5, -5 ), "S" );
	painter->drawText( this->_EllipseColor.bottomLeft() + QPointF( 5, -5 ), "C" );
	painter->drawText( this->_EllipseHide.bottomLeft() + QPointF( 5, -5 ), "H" );
	painter->setBrush( QBrush( QColor( 200, 10, 10, 100 ) ) );
	painter->setPen( QColor( 200, 10, 10, 100 ) );
	painter->drawRect( _EllipseHide );
	//************************************
	if( _ONCONTROLTYPE > -1 )
	{
		painter->setPen( QColor( 255, 255, 255, 255 ) );
		painter->drawText( this->_Control.bottomLeft()+QPointF(5, -3), controlName[_ONCONTROLTYPE] );
	}
	painter->restore();
	return;
}

void SymbolColorPanel::initViewRect()
{
	//************************************************
	/*this->_view.append( tr("3D") );
	this->_colors.append( QColor( 10,10,150,250) );
	this->_view.append( tr("Video1") );
	this->_colors.append( QColor( 120,20,80,250) );
	this->_view.append( tr("Video2") );
	this->_colors.append( QColor( 120,20,80,250) );
	this->_view.append( tr("Video3") );
	this->_colors.append( QColor( 120,20,80,250) );
	this->_view.append( tr("PC1") );
	this->_colors.append( QColor( 20,160,20,250) );
	this->_view.append( tr("PC2") );
	this->_colors.append( QColor( 20,160,20,250) );
	this->_view.append( tr("PC3") );	
	this->_colors.append( QColor( 20,160,20,250) );*/
	this->_view.append( tr("barChart") );
	this->_colors.append( QColor( 80,160,160,250) );
	this->_view.append( tr("lineChart") );
	this->_colors.append( QColor( 80,160,160,250) );
	this->_view.append( tr("scatterPlot"));
	this->_colors.append( QColor( 80,160,160,250) );
	this->_view.append( tr("Web"));
	this->_colors.append( QColor( 40,20,80,250) );
	/*this->_view.append( tr("Matlab"));
	this->_colors.append( QColor( 40,20,80,250) );
	this->_view.append( tr("Pen"));
	this->_colors.append( QColor( 40,20,80,250) );*/
	this->_view.append( tr("Note"));
	this->_colors.append( QColor( 40,20,80,250) );
	this->_view.append( tr("PathBubble1"));
	this->_colors.append( QColor( 40,20,80,250) );

	this->_view.append( tr("expression"));
	this->_colors.append( QColor( 40,20,80,250) );
	
	//************************************************
	int x_step = 40;
	int y_step = 60;
	int width_step = 35;
	int height_step = 50;
	int Num_Item = 5;
	//************************************************
	int initX = this->getSize().width() / 3 * 2;
	for( int i = 0; i < this->_view.size(); i++ )
	{
		this->_rectsView.append( QRectF( i%Num_Item*x_step + initX + 5, i/Num_Item*y_step + 5, width_step, height_step ) );
		this->_shadow.append( QRectF( i%Num_Item*x_step + initX + 7, i/Num_Item*y_step + 7, width_step, height_step ) );
		this->_iconArea.append( QRectF( i%Num_Item*x_step + initX + 5, i/Num_Item*y_step + 10, width_step, height_step-10 ) );
	}
	return;
}

void SymbolColorPanel::drawViews(QPainter *painter)
{
	painter->save();
	//**************************************
	painter->setPen( Qt::NoPen );
	for( int i = 0; i < this->_rectsView.size(); i++ )
	{
		painter->setBrush( QColor( 20, 20, 20, 200) );
		painter->drawRect( this->_shadow[i] );
		painter->setBrush( this->_colors[i] );
		painter->drawRect( this->_rectsView[i] );
		painter->drawImage( _iconArea[i], this->_icons[i], _iconRects[i] );
	}
	//**************************************
	if( _VISINDEX >= 0 )
	{
		if( _DIRECTPREOPEN && _VISINDEX > 0)
		{
			painter->setBrush( this->_colors[_VISINDEX] );
			painter->drawRect( _preOpenViewDirectly );
		}
		else
		{
			painter->setBrush( this->_colors[_VISINDEX] );
			painter->drawRect( _visDragRect );

		}
	}
	//**************************************
	painter->restore();
	return;
}

bool SymbolColorPanel::handlePressVisView(QPointF pos)
{
	for( int i = 0; i < this->_rectsView.size(); i++ )
	{
		if( this->_rectsView[i].contains( pos ) )
		{
			this->_VISINDEX = i;
			_visDragRect = this->_rectsView[i];
			return true;
		}
	}
	return false;
}

bool SymbolColorPanel::handleMoveVisView( QPointF pos )
{
	if (inFileView(pos))
	{
		return false;
	}

	if( _VISINDEX >= 0 )
	{
		this->_visDragRect.moveTo( pos );
		return true;
	}
	return false;
}

/*bool SymbolColorPanel::handleReleaseVisView(QPointF pos)
{
	for( int i = 0; i < this->_lines.size(); i++ )
	{
		if( this->_lines[i]->mouseOver( pos ) )
		{
			this->_lines[i]->addView( _VISINDEX, _colors[_VISINDEX] );

			logDragVisToCompose( i, _VISINDEX );
			_VISINDEX = -1;
		}
	}
	_VISINDEX = -1;
	return false;
}*/

void SymbolColorPanel::initIcons()
{
	/*this->_icons[0] = QImage( tr("icon/3d.png") );
	this->_icons[1] = QImage( tr("icon/batc.png") );
	this->_icons[2] = QImage( tr("icon/batg.png") );
	this->_icons[3] = QImage( tr("icon/bath.png") );
	this->_icons[4] = QImage( tr("icon/PC1.png") );
	this->_icons[5] = QImage( tr("icon/PC2.png") );
	this->_icons[6] = QImage( tr("icon/PC3.png") );*/
	this->_icons[0] = QImage( tr("icon/bar chart.png") );
	this->_icons[1] = QImage( tr("icon/line chart.png") );
	this->_icons[2] = QImage( tr("icon/scatter plot.png") );
	this->_icons[3] = QImage( tr("icon/web.png") );
	//this->_icons[11] = QImage( tr("icon/mat.png") );
	//this->_icons[12] = QImage( tr("icon/pen.png") );
	this->_icons[4] = QImage( tr("icon/note.png") );
	this->_icons[5] = QImage( tr("icon/PathBubble1.png") );
	this->_icons[6] = QImage( tr("icon/expression.png") );

	for( int i = 0; i < _view.size(); i++ )
	{
		this->_iconRects.append( QRectF( QPointF( 0, 0 ), this->_icons[i].size() ) );
	}
	return;
}

void SymbolColorPanel::openViewDirectly( QPointF pos )
{
	int size = _preOpenViewDirectly.height();
	
	if( _VISINDEX == 0 )
	{
		this->_open->_manager->itemPosUpdated(this->_open->openBarChart( size, pos.x() + size/2, pos.y() + size/2 ));
	}
	else if( _VISINDEX == 1 )
	{
		this->_open->_manager->itemPosUpdated(this->_open->openLineChart( size, pos.x() + size/2, pos.y() + size/2 ));
	}
	else if ( _VISINDEX == 2 )
	{
		this->_open->_manager->itemPosUpdated(this->_open->openScatterPlot( size, pos.x() + size/2, pos.y() + size/2 ));
	}
	else if( _VISINDEX == 3 )
	{
		this->_open->_manager->itemPosUpdated(this->_open->openWebView( NULL, size, pos.x() + size/2, pos.y() + size/2 ));
	}
	/*else if( _VISINDEX == 4 )
	{
		//this->_open->openMatlab( size, pos.x() + size/2, pos.y() + size/2 );
	}
	else if( _VISINDEX == 5 )
	{
		//this->_open->openPen( size, pos.x() + size/2, pos.y() + size/2 );
	}*/
	else if ( _VISINDEX == 4 )
	{
		 this->_open->_manager->itemPosUpdated(this->_open->openNote( NULL, size, pos.x() + size/2, pos.y() + size/2 ));
		 
	}
	else if ( _VISINDEX == 5 )
	{
	     this->_open->_manager->itemPosUpdated(this->_open->openPathBubble1( 400, pos.x() + size/2, pos.y() + size/2));
	}
	else if ( _VISINDEX == 6 )
	{
		this->_open->_manager->itemPosUpdated(this->_open->openExpressionBubble( size, pos.x() + size/2, pos.y() + size/2 ));
	}
	return;
}

void SymbolColorPanel::drawPreOpenViewDirectly(QPointF pos)
{
	if( this->boundingRect().contains( pos ))
	{
		return ;
	}
	else
	{
		_preOpenViewDirectly = this->_open->queryPreOpen( pos, 1 )[0];
	}
}

void SymbolColorPanel::drawColorBar( QPainter* painter )
{
	if( !COLORBAR )
	{
		return;
	}
	painter->save();
	//*********************************************
	myColor3 color;
	for( int i = 0; i<100; i++)
	{
		Lab3D point = this->_scene->getCut()->get_Position( float( i /100.0) );
		this->_scene->getLab()->get_Color( point, color );
		painter->setBrush( QColor( color.a, color.b, color.c, 255) );
		painter->setPen( QColor( color.a, color.b, color.c, 255) );
		painter->drawRect( this->getSize().width()/3 + i*2, this->getSize().height(), 2, 20 );
	}
	//*********************************************
	painter->setFont( QFont( "times", 12) );
	painter->setPen( QColor( 255, 255, 255, 255) );
	painter->drawText( this->getSize().width()/3 + 165, this->getSize().height(), tr("100%") );
	painter->drawText( this->getSize().width()/3, this->getSize().height(), tr("0%") );
	painter->restore();
	return;
}

void SymbolColorPanel::OpenFileDirectly(QPointF pos)
{
	_DRAGFILE = false;
	this->_manager->getRoot()->disableDrag();

	if (this->_open->changeFile(pos, _CurrentFileName))
	{
		return ;
	}
	else if ( this->_scene->changePlotAttribute(pos, _CurrentFileName) )
	{
		return ;
	}
	/*else
	{
		QList<int> views;
		views.append(0);	//default 3D view

		addNewToDrag();
		int lindex = this->_lines.size()-1;
		this->_lines[lindex]->addView(0, _colors[0]);

		this->_open->openDataInViewAt(_CurrentFileName, views, pos);
	}*/
}

void SymbolColorPanel::changeView( QPointF pos, int visindex )
{
	this->_open->changeView(pos, visindex);
}

void SymbolColorPanel::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	BarBase::mousePressEvent( event );
	if(isInHideButton(event->pos().x(),event->pos().y()))
	{
		if(hideIt)
		{
			hideIt=false;			
			QDesktopWidget *mydesk = QApplication::desktop ();
			int screenWidth=mydesk->screenGeometry().width(); 
			int screenHeight=mydesk->screenGeometry().height();
			this->resizeItem( QSizeF(screenWidth, 166) );	
			this->setPos( 0, screenHeight-166);
			
			QSizeF s=this->getSize();
			int SX=s.width()-80, SY=30;
			hideButton=QRect(SX,SY,50,50);
	        //this->_colorPanel = new SymbolColorPanel( QSizeF(screenWidth, 166), 0, screenHeight-166, this, _openManager );	
		}
		else 
		{
			hideIt=true;	
			QDesktopWidget *mydesk = QApplication::desktop ();
			int screenWidth=mydesk->screenGeometry().width(); 
			int screenHeight=mydesk->screenGeometry().height();
			this->resizeItem( QSize(hideButton.size().width()+20, 146));
			this->setPos( screenWidth-(hideButton.size().width()+40), screenHeight-146);
			
			QSizeF s=this->getSize();
			int SX=s.width()-60, SY=10;
			hideButton=QRect(SX,SY,50,50);
		}		
	    //setVisible(false);	
	}
}