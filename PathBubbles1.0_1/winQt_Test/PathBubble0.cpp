
#include "PathBubble0.h"


PathBubble0::PathBubble0( int size, int x, int y, OpenGLScene* Scene, StatisticManager* manager, OpenManager* open, QString name)
: PlotBase(size, x, y, Scene, manager)
{
	_scene= Scene;
	_scene->PathWayNames.clear();
	readPathWays("pathways.txt");
	pathwaySelected=-1;
	highlighted = -1;

	this->_open = open;
	this->_TYPE = PATHBUBBLE0;
}

void PathBubble0::hoverMoveEvent( QGraphicsSceneHoverEvent *event )
{
	ItemBase::hoverMoveEvent(event);	
	//float x=event->pos().x(),y=event->pos().y();	
	highlighted = whichTerm(event->pos());
}

void PathBubble0::saveAsXmlNode( QDomDocument &doc, QDomElement &node )
{
	QDomText text;
	QString str, tmpStr;

	QDomElement type = doc.createElement( tr("Type") );
	text = doc.createTextNode( tr("PathBubble0") );
	type.appendChild( text );
	node.appendChild( type );

	ItemBase::saveAsXmlNode( doc, node );
}

void PathBubble0::readPathWays(const char *pathWays)
{
	char ch[100];
	FILE *fp = fopen(pathWays,"r"); 
	int id,id2=-1;
	do
	{
		  fscanf(fp,"%d", &id);
	      if(id2==id)
		  {
			 break;
		  }
	      id2=id;   		  
		  fscanf(fp,"%s\n",&ch);		  
		  string s2(ch);   
		  _scene->PathWayNames.push_back(s2.c_str());
	 }while(true);
}


void PathBubble0::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
	PlotBase::paint( painter, option, widget );
	paintPathWays(painter, _scene->PathWayNames);	
}


void PathBubble0::paintPathWays(QPainter *painter, vector<QString> PathWayNames)
{
	int width=this->realRect().width();
	int height=this->realRect().height();
	int num=PathWayNames.size();
	int column=sqrt(float(num));
	int low=num/column;
	if(column*low<num)
		column+=1;

	int l,c;
	float x,y;
	int w=width/column;
	int h=height/low;
	int rw,rh;

	for(int i=0; i<PathWayNames.size(); i++)
	{
	     l=i/column;
		 c=i%column;

		 x=l*w-width/2.0;
		 y=c*h-height/2.0;

		 rw=w-2, rh=h-2;

		 QString qstr=PathWayNames[i];	

		 QString fontType="Arial";
		 int fontSize=20;
		 int style=QFont::Bold;
		
		 if(rw<5)
			 rw=5;
		 if(rh<5)
			 rh=5;

		 x=x+1, y=y+1;

		 
		 if(highlighted==i)
		 {
			 QLinearGradient linearGradient(x, y, x, y+h);
             linearGradient.setColorAt(0.0,  QColor(150,150,150,255));
             linearGradient.setColorAt(1.0,  QColor(138,138,168,255));    
			 painter->setPen(QColor(148,148,178,255));
		     painter->setBrush( linearGradient );			 
		 }
		 else
		 {
			 painter->setPen(Qt::NoPen);
			 painter->setBrush( QColor(128,128,128,255) );
		 }

		 painter->drawRect( x, y, rw, rh);	//rect

		 resizeFont(qstr, fontType , fontSize, style, rw, rh);
		 QFont f(fontType, fontSize, style); //Arial, Courier, Arial Narrow, MS Gothic
		 painter->setFont (f);

		 painter->setFont(f);	
		 painter->setPen( QColor(255,255,255,255) );
		 painter->drawText( x, y, rw, rh, Qt::AlignCenter, qstr);	//rect	

		 XL.push_back(x),  XR.push_back(x+rw),  YB.push_back(y), YT.push_back(y+rh);

		
	}
}


bool PathBubble0::insideRect(int x, int y, int xl, int yb, int xr, int yt)
{
   if(x>=xl&&x<=xr&&y>=yb&&y<=yt)
	   return true;
   else return false;
}

int PathBubble0::whichTerm( QPointF pos)
{	   
	float x=pos.x(), y=pos.y();	
	for(int i=0; i<XL.size(); i++)
	{
		if(insideRect(x, y, XL[i], YB[i], XR[i], YT[i]))
		{
			return i;
		}
	}
	return -1;
}

void PathBubble0::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	ItemBase::mousePressEvent(event);
	pathwaySelected = whichTerm(event->pos());
}

void PathBubble0::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	ItemBase::mouseReleaseEvent( event );
	
	if( pathwaySelected == whichTerm(event->pos()))
	{		
		if(pathwaySelected!=-1)
		    showPathway(this->pos(), this->scenePos()); 		
	}
}

void PathBubble0::showPathway( QPointF pos, QPointF scenePos)
{
	float x=pos.x(), y=pos.y();	
	_open->openPathBubble1( 400, 400, 450 );							
}


void PathBubble0::resizeFont(QString aText, QString fontType , int &fontSize, int style, int width, int height)
{
    int newFontSize;
	QFont aFont(fontType, fontSize, style);
	QFontMetrics qfm(aFont);	
	QSize size = qfm.size(0, aText);	
	newFontSize=fontSize;
	if(size.width()>width*0.95||size.width()<width*0.85)
	    fontSize= fontSize*width*0.9/size.width();
	
	QFont aFont2(fontType, fontSize, style);
	QFontMetrics qfm2(aFont2);	
	size = qfm2.size(0, aText);	
	if(size.height()>height*0.95)
	{
		fontSize = fontSize*height*0.95/size.height();		
	}
	if(fontSize<1)
		fontSize=1;
}
