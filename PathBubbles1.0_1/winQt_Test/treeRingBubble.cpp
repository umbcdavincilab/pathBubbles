//#include "textBubble2.h"
//#include "ReactionBubble3.h"
//#include "PathBubble2.h"
//#include "GroupBubble4.h"
//#include "penstroke.h"
#include "treeRingBubble.h"
#include <QTextDocument>
#include <QMap>

#define BUDDLINGSTRENGTH 0.1
#define MAX(a,b) a>=b?a:b //may cause variable around become statics
//#include <config.hpp>  
//#include <boost/graph/adjacency_list.hpp>  

//using namespace boost;

treeRingBubble::treeRingBubble( int size, int x, int y, OpenGLScene* Scene, StatisticManager* manager, OpenManager* open, QString name)
: PlotBase (size, x, y, Scene, manager)
{
	_treeRingBubbleParent=NULL;
	spanIsOn=false;
	bubbleDraggedout=false;
	lastPressedPos = QPointF(0,-1000000);
    _num_layers=2;
	if(_num_layers<2)
		_num_layers=2;
	else if(_num_layers>5)
		_num_layers=5;
	fixedWidth=size, fixedHeight=size; //fixedWidth/1.618;
	bubbleRect=QRect(0,0,0,0);
	miniMized=false;
	toplevel=true;
	if(fixedSize)
	{
		resizeItem( fixedWidth, fixedHeight);
	}
	_open=open;
	firstMotionLeft=true;
	firstMotionRight=true;
	stroke_button = -1;
	stroke_tracking = GL_FALSE;
	stroke_animate = GL_TRUE;	

	_scene= Scene;	
	_LABEL = false;
	dCenter=QPoint(0,0);	
	
	_name=name; 
	loadFile(_name); 
	_mousePressed=false;
	pressOnBoarder = false;
	firstMotionLeft=true;
    firstMotionRight=true;
	_scene->controlKeyisOn = false;

	compoudgraph_centre.x=0, compoudgraph_centre.y=0;
	
	QString fileName= "data/treeRingData/" + name + "_normal_b_";
	fileName = fileName + QString::number(_num_layers);
	fileName = fileName + ".out"; 		
	string data = fileName.toStdString();
	const char *dname = data.c_str();
	dataName = (char *) dname;	

	fileName = "data/treeRingData/" + name + "_tree_b_";
	fileName = fileName + QString::number(_num_layers);
	fileName = fileName + ".out"; 
	string tree = fileName.toStdString();
	const char *tname=tree.c_str();
	treeName = (char *) tname;	
	
	fileName = "data/treeRingData/" + name + "_graph_b_";
	fileName = fileName + QString::number(_num_layers);
	fileName = fileName + ".out"; 
	string graph = fileName.toStdString();
	const char *gname=graph.c_str();
	graphName= (char *) gname;

	fileName = "data/treeRingData/" + name + "_tree_extraLayer_b_";
	fileName = fileName + QString::number(_num_layers);
	fileName = fileName + ".out"; 
	string layerS = fileName.toStdString();
	const char *lname=layerS.c_str();
	extraLayerName = (char *) lname;

	//dataName= "data/treeRingData/normal_b.out"; //path_normal_b.out"
	//treeName="data/treeRingData/tree_b.out";
	//graphName="data/treeRingData/graph_b.out";
	
	dragout=0;	
	ringRadius=250*0.6;
	ringWidth=39*(0.5+0.6/_num_layers);
	window_w=1000, window_h=1000;
	initTreeRing(0, 0, window_w, window_h);
	storeTreeRing(this);

	m_treeRingBubbleID = _scene->m_treeRingBubbles.size();

	this->_TYPE = TREERING;		
	this->_colorBoarder = TDHelper::getBoarderColorByType(_TYPE, m_treeRingBubbleID);	
	this->_colorInner = TDHelper::getInnerColorByType(_TYPE, m_treeRingBubbleID);	
	resetOriRect();
	
	if(name=="path")
	{
	    _name="Species Reactome Pathway_" + QString::number(m_treeRingBubbleID);	
	}
	else
	{
		_name= "Species Reactome Pathway";
	}	

	_treeRing->SetColorBy(EDGE_BY_ORTHOLOGY);
	_treeRing->ComputeNodeColorByO("data/Reactome_Pathway_Data/orthologyData/gallus_gallus_ortholog.txt");	//use chicken as default
	 SetEdgeBy(EDGE_BY_ORTHOLOGY);

	setCurrentFile(_name);
	draggingItem=0;
	_parentID = -1;

	//_name = name;
	//getExpressionToBePaint(bgRect, bgColor, borderColor, htColor, borderWidth, htRect, fontSize, fontColor, fontPos, fixedSize, dCenter, _scale);
}

treeRingBubble::treeRingBubble( int size, int x, int y, OpenGLScene* Scene, StatisticManager* manager, OpenManager* open, treeRingBubble *parent, set<vector<int>> itemSelected)
: PlotBase (size, x, y, Scene, manager)
{
	spanIsOn=false;
	bubbleDraggedout=false;
	lastPressedPos = QPointF(0,-1000000);
	_num_layers=2;
	if(_num_layers<2)
		_num_layers=2;
	else if(_num_layers>5)
		_num_layers=5;
	_open=open;
	//for subtree Ring
	bubbleRect=QRect(0,0,0,0);
	miniMized=false;
	fixedWidth=size, fixedHeight=size; //fixedWidth=300, fixedHeight=fixedWidth/1.618;
	if(fixedSize)
	{
		resizeItem( fixedWidth, fixedHeight);
	}
	firstMotionLeft=true;
	firstMotionRight=true;
	stroke_button = -1;
	stroke_tracking = GL_FALSE;
	stroke_animate = GL_TRUE;

	window_w=1000, window_h=1000;

	_scene= Scene;	
	_LABEL = false;
	dCenter=QPoint(0,0);	
	
	//_name=name; 
	//loadFile(_name); 
	_mousePressed=false;
	firstMotionLeft=true;
    firstMotionRight=true;

	compoudgraph_centre.x=0, compoudgraph_centre.y=0;
	//dataName= "data/treeRingData/path_normal_b.out"; //path_normal_b.out"
	//treeName="data/treeRingData/path_tree_b.out";
	//graphName="data/treeRingData/path_graph_b.out";
	ringRadius=250*0.67*0.71;
	ringWidth=39*(0.5+0.6/_num_layers)*0.71;

	_name = getTreeRingName(parent, itemSelected);
	initSubTreeRing(parent, itemSelected, 0, 0, window_w, window_h);
	storeTreeRing(this);

	m_treeRingBubbleID = _scene->m_treeRingBubbles.size();
	this->_TYPE = TREERING;		
	this->_colorBoarder = TDHelper::getBoarderColorByType(_TYPE, m_treeRingBubbleID);	
	this->_colorInner = TDHelper::getInnerColorByType(_TYPE, m_treeRingBubbleID);	
	resetOriRect();
		
	//_name="Tree Ring" + QString::number(m_treeRingBubbleID);	
	setCurrentFile(_name);

	draggingItem=0;
	initItemSelected = itemSelected;
	_parentID = parent->m_treeRingBubbleID;
	//getExpressionToBePaint(bgRect, bgColor, borderColor, htColor, borderWidth, htRect, fontSize, fontColor, fontPos, fixedSize, dCenter, _scale);
}

QString treeRingBubble::getTreeRingName(treeRingBubble *parent, set<vector<int>> itemSelected)
{
	QString name="";
	for(set<vector<int>>::iterator it=itemSelected.begin(); it!=itemSelected.end(); it++)
	{
		vector<int> item=*it;
		int layer=item[0], id=item[1];
	    if(layer==0)
		{
			if(name!="")
			{
				name = name + ";";
			}
		    name = name + QString::fromStdString(parent->_treeRing->_ring[layer]._node_name[id]);
		}	
	}
	return name;
}

QString treeRingBubble::getTreeRingName(set<string> nameSelected)
{
	QString name="";
	for(set<string>::iterator it=nameSelected.begin(); it!=nameSelected.end(); it++)
	{
		
	    	if(name!="")
			{
				name = name + ";";
			}
		    name = name + QString::fromStdString(*it);
			
	}
	return name;
}


float treeRingBubble::dist(int x1, int y1, int x2, int y2)
{
  return sqrt(((float)x1-(float)x2)*((float)x1-(float)x2) + ((float)y1-(float)y2)*((float)y1-(float)y2));
}

bool treeRingBubble::strokeMotionLeft(int x, int y)
{
	//cg->UpdateHighlight(&firstMotionLeft, x, y); //If set the firstMotionLeft = false here
	bool flag=UpdateHighlight(&firstMotionLeft, x, y);
	//mySlider->UpdateHighlight(&firstMotionLeft, x, y); //Wrong
	firstMotionLeft = false;	
	return flag;
}

void treeRingBubble::drawCurve( QPainter* painter)
{
	if(_lines.size()>0)
		_lines=_lines;
	painter->save();//start
	//*************************************************************************
	painter->setPen( QPen( QBrush( QColor( 0, 0, 255, 255) ), 2 ) );
	painter->drawLines( _lines );
	//*************************************************************************
	painter->restore();//start
	return;
}

void treeRingBubble::selfDelection(treeRingBubble* b0, treeRingBubble* b1)
{						
	//_scene->bubbleDeleted(this, result);					
	//deleteSelectedItems(_scene);										
	//menuSelection(QPointF(0,0), QPointF(0,0), 49);		
	if(b1!=this)
		return;

		_penisOn=false;	
		_grabisOn=false;	
		_queryisOn=false;
		_upisOn=_downisOn=_evenisOn=false;
		_lines.clear();	

		//_pathBubbleParent=NULL;
		float _zoom_t = b0->_treeRing->_ring[0]._ring_radius;
		MergeTreeRings(b0, b1, window_w, window_h);

		updateConnectionAfterMerge(b0,b1);

		int edge_by=b0->GetEdgeBy();
		if(edge_by==EDGE_BY_ORTHOLOGY)
		{				
			b0->_treeRing->ComputeNodeColorByO(b0->_treeRing->getOrthFileName());	
			b0->_treeRing->SetColorBy(EDGE_BY_ORTHOLOGY);
			//b0->EDGE_BY=EDGE_BY_ORTHOLOGY;		
		}
		else if(edge_by==EDGE_BY_EXPRESSION)
		{
			b0->searchSharedProtein();
		    b0->searchExpressed();	
		}

		_zoom_t = _zoom_t/b0->_treeRing->_ring[0]._ring_radius-1;

		b0->Zoom(_zoom_t);
		//b1->updateConnectionAfterDeletion();		
	    b1->setVisible(false);
		b1->hide();		
		b1->m_treeRingBubbleID=-1;
		
		//tmp2->deleteSelectedItems(_scene);
		return;	
}

void treeRingBubble::drawfills( QPainter* painter )
{
	painter->save();	
	painter->setBrush( QColor( 0, 0, 255, 100) );	
	painter->drawPolygon(_contain);
	painter->restore();	
}


bool treeRingBubble::groupSelection(QVector<QLineF> _lines)
{
	itemSelected.clear();
	//ptest.clear();
	if(_lines.size()<4)
		return false;

	_lines.push_back(_lines[0]); //head to end

	//polygon	
	float xr=-100000,xl=100000,yt=-100000,yb=100000;
	for(int i=0; i<_lines.size(); i++)
	{
	   if(xl>_lines[i].x1()) xl=_lines[i].x1();
	   if(xr<_lines[i].x1()) xr=_lines[i].x1();
	   if(yt>_lines[i].y1()) yt=_lines[i].y1();
	   if(yb<_lines[i].y1()) yb=_lines[i].y1();		   
	   _contain << _lines[i].p1();
	}
  
  //nodeX[MAX_POLY_CORNERS]
  int  nodes, pixelX, pixelY, i, j, swap, polyCorners = _lines.size();
  vector<int> nodeX(polyCorners,0);
  fills.clear();
  //  Loop through the rows of the image.
  for (pixelY=yt; pixelY<yb; pixelY++) {

  //  Build a list of nodes.
  nodes=0; //j=polyCorners-1;
 /* for (i=0; i<polyCorners; i++) 
  {
		j=(i+1)%polyCorners;
		if (_lines[i].y1()<(double) pixelY && _lines[j].y1()>=(double) pixelY
		||  _lines[j].y1()<(double) pixelY && _lines[i].y1()>=(double) pixelY) 
		{
		  nodeX[nodes++]=(int) (_lines[i].x1()+(pixelY-_lines[i].y1())/(_lines[j].y1()-_lines[i].y1())
		  *(_lines[j].x1()-_lines[i].x1())); 
		}

   }*/


  for (i=0; i<polyCorners; i++) 
  {
		j=(i+1)%polyCorners;
		if (_lines[i].y2()<(double) pixelY && _lines[j].y2()>=(double) pixelY
		||  _lines[j].y2()<(double) pixelY && _lines[i].y2()>=(double) pixelY) 
		{
		  nodeX[nodes++]=(int) (_lines[i].x2()+(pixelY-_lines[i].y2())/(_lines[j].y2()-_lines[i].y2())
		  *(_lines[j].x2()-_lines[i].x2())); 
		}

   }


  //  Sort the nodes, via a simple “Bubble” sort.
  i=0;
  while (i<nodes-1) {
    if (nodeX[i]>nodeX[i+1]) {
      swap=nodeX[i]; nodeX[i]=nodeX[i+1]; nodeX[i+1]=swap; if (i) i--; }
    else {
      i++; }}

  //  Fill the pixels between node pairs.  
  for (i=0; i<nodes; i+=2) 
  {
    /*if   (nodeX[i  ]>=xr) break;
    if   (nodeX[i+1]> xl ) 
	{
      if (nodeX[i  ]< xl ) nodeX[i  ]=xl ;
      if (nodeX[i+1]> xr) nodeX[i+1]=xr;
      //for (j=nodeX[i]; j<nodeX[i+1]; j++) 
		  //fillPixel(j,pixelY); 
	  fills.push_back(QLineF(nodeX[i],pixelY,nodeX[i+1],pixelY));
	}*/
	fills.push_back(QLineF(nodeX[i],pixelY,nodeX[i+1],pixelY));
  }
  }
    
  //check overlap  
  for (i=0; i<fills.size(); i+=1) 
  {
	  for(j=fills[i].x1(); j<fills[i].x2(); j+=1)
	  {
		  //ptest.push_back(QPoint(j, fills[i].y1()));  //keqin
		  vector<int> temp= whichItem(QPoint(j, fills[i].y1()));
		  if(temp.size()>=2 && temp[0]>=0 && temp[1]>=0)
		     itemSelected.insert(temp);
	  }
  }

  if(itemSelected.size()==0)
	  return false;
  
  //get how many levels it involve
  set<int> layerSet;
  for(set<vector<int>>::iterator it=itemSelected.begin(); it!=itemSelected.end(); it++)
  {
	   vector<int> item=*it;
	   int layer=item[0];
	   layerSet.insert(layer);
  }
  int lowestlayer=*layerSet.rbegin();
  int highestlayer=*layerSet.begin();
  int layerNum = _treeRing->_ring.size();
  for(int i=0; i<layerNum; i++)
  {
      layerSet.insert(i);
  }

 
  for(set<int>::iterator iti=layerSet.begin(); iti!=layerSet.end(); iti++)
  {
	    int lid= *iti;
		set<vector<int>> toBeInsert;
		for(set<vector<int>>::iterator it=itemSelected.begin(); it!=itemSelected.end(); it++)
		{
			vector<int> item=*it;
			set<vector<int>>::iterator it1=it;
			it1++;				
			if(it1==itemSelected.end())
				break;  
			vector<int> item1=*it1;
			int layer=item[0], id=item[1], layer1=item1[0], id1=item1[1];
			if(layer1==layer && layer1==lid && _treeRing->_ring[lid]._parent[id] == _treeRing->_ring[lid]._parent[id1])	
			{
				int temp;
				if(id>id1)
				{
					temp=layer; layer=layer1; layer1=temp; 
					temp=id; id=id1; id1=temp;		
				} 
				int range=_treeRing->_ring[layer]._node_num;
				if(id1-id<range/2)
				{
					for(int i=id; i<=id1; i++)
					{
						vector<int> item3(2,0);
						item3[0]=layer1; item3[1]=i;
						toBeInsert.insert(item3);
					}		   
				}
				else
				{
					for(int i=0; i<=id; i++)
					{
						vector<int> item3(2,0);
						item3[0]=layer1; item3[1]=i;
						toBeInsert.insert(item3);
					}
					for(int i=id1; i<range; i++)
					{
						vector<int> item3(2,0);
						item3[0]=layer1; item3[1]=i;
						toBeInsert.insert(item3);
					}
		   
				}
			}
		}  
		for(set<vector<int>>::iterator it=toBeInsert.begin(); it!=toBeInsert.end(); it++)
		{    
			itemSelected.insert(*it);
		}
		//put all the child nodes in
		set<vector<int>> toBeInsert1;
		for(set<vector<int>>::iterator it=itemSelected.begin(); it!=itemSelected.end(); it++)
		{
		    vector<int> item=*it;
			int layer=item[0], id=item[1];
			if(layerSet.find(layer+1)!=layerSet.end())
			for(int j=_treeRing->_ring[layer]._child_range[id].first; j<=_treeRing->_ring[layer]._child_range[id].second; j++)
			{
				vector<int> item1(2,0);
			    item1[0] = layer+1; item1[1] = j;
				toBeInsert1.insert(item1);
			}		 
		}
		for(set<vector<int>>::iterator it=toBeInsert1.begin(); it!=toBeInsert1.end(); it++)
		{        
			itemSelected.insert(*it);
		}
  }

  //make section contain all the layers
  for(set<int>::reverse_iterator iti=layerSet.rbegin(); iti!=layerSet.rend(); iti++)
  {
       int lid= *iti;
	   if(lid==0 )
		   continue;
	   
	    set<vector<int>> toBeInsert;
        for(set<vector<int>>::iterator it=itemSelected.begin(); it!=itemSelected.end(); it++)
		{
			vector<int> item=*it;
			vector<int> item1(2,0);
			int layer=item[0], id=item[1];
			if(layer==lid)	
			{
				item1[0]=lid-1; 
				item1[1]=_treeRing->_ring[lid]._parent[id];				 
				toBeInsert.insert(item1);
			}			
		}  
		for(set<vector<int>>::iterator it=toBeInsert.begin(); it!=toBeInsert.end(); it++)
		{        
			itemSelected.insert(*it);
		}
  }
  

    //put all the child nodes in
	set<vector<int>> toBeInsert3;
	for(set<vector<int>>::iterator it=itemSelected.begin(); it!=itemSelected.end(); it++)
	{
		vector<int> item=*it;
		int layer=item[0], id=item[1];
		if(layer==0)
		{
			//put all nodes of all layers under the this node of top layer in the new segment
			_treeRing->findAllDescendantNodes(layer, id, toBeInsert3);
		}
	}

	for(set<vector<int>>::iterator it=toBeInsert3.begin(); it!=toBeInsert3.end(); it++)
	{        
		itemSelected.insert(*it);
	}
    return true;
}

set< vector< int > > treeRingBubble::whichSegment( QPointF pos )
{
	set< vector< int > > output;
	vector<int> item = _treeRing->CheckPointInTreeRing(pos.x(), pos.y());
	if(item.size()<2 || item[0]<0)
		return output;
	int pl=item[0],pid=item[1];
	output = whichSegment(pl, pid);
	return output;
}

bool treeRingBubble::isSegPreSelected(int layer, int id, set<vector<int>> preItemSelected)
{
	if(_hl<0 || _hn<0)
	{
		return false;
	}
	vector<int> item;
	set< vector< int > > segs = whichSegment(layer, id);
	bool flag;
	for(set<vector<int>>::iterator it=segs.begin(); it!=segs.end(); it++)
	{
		flag=false;
		vector<int> item = *it;			
		if (preItemSelected.find(item) != preItemSelected.end() )
		{
				flag=true;
		}	
		else if( searched.find(item) != searched.end() )
		{
			flag=true;
		}									
		/*if(_treeRing->_itemClicked.find(item)!=_treeRing->_itemClicked.end() )
		{
			flag=true;				
		}*/
		for (int j = 0; j <_connections.size(); j++)
        {	
			if(_connections[j].itemSelected.find(item)!=_connections[j].itemSelected.end())
		    {
				flag=true;
				break;
		    }		    
		}
		if(!flag)
			return false;
	}
	return true;
}



bool treeRingBubble::isSegHighlighted(set< vector< int > > segs)
{
	bool flag=true;
	//set< vector< int > > segs = whichSegment(layer, id);
	for(set<vector<int>>::iterator it=segs.begin(); it!=segs.end(); it++)
	{
	    vector<int> item = *it;
		int layer=item[0], id=item[1];
	    if(!_treeRing->isNodeHighlighted(layer, id))
		{
		   flag=false;
		   break;
		}
	}
	return flag;
}

bool treeRingBubble::isSegHighlighted(int layer, int id)
{
	if(layer<0 || id<0)
		return false;
	bool flag=true;
	set< vector< int > > segs = whichSegment(layer, id);
	return(isSegHighlighted(segs));
}

set< vector< int > > treeRingBubble::whichSegment( int layer, int id )
{
	set< vector< int > > output;
	
	if(layer<0 || id<0)
		return output;

	vector<int> item(2,0);
	
	while(layer>0 && layer<_num_layers)
	{
		id = _treeRing->GetParentIndex(layer, id);
		layer = layer-1;
	}
	vector<vector<int>> output1;	
	_treeRing->findAllDescendantNodes(layer, id, output1);	
	item[0]=layer, item[1]=id;
	output.insert(item);
	for(int i = 0; i<output1.size(); i++)
	for(int j = 0; j<output1[i].size(); j++)
	{
		item[0]=i, item[1]=output1[i][j];
	    output.insert(item);
	}
	return output;
}

vector<int> treeRingBubble::whichItem( QPointF pos)
{	   
	return _treeRing->CheckPointInTreeRing(pos.x(), pos.y());
}

vector<int> treeRingBubble::whichHandle( QPointF pos)
{	   
	return _treeRing->CheckHandleInTreeRing(pos.x(), pos.y());
}

void treeRingBubble::strokeMotionRight(int x, int y)
{
  int ox = 0, oy = 0;
  if(firstMotionRight)  // init the mouse locations
  {
	oldMousex = x;
	oldMousey = y;
	mousex = x;
	mousey = y;
	firstMotionRight=false;
  }

  mousex = x;
  mousey = y;

  if (stroke_tracking == GL_FALSE)
    return;


  ox = mousex - oldMousex;
  oy = mousey - oldMousey;

  oldMousex = mousex;
  oldMousey = mousey;
  //curve1.add(mousex,windowHeight-mousey);
  //}
	//cg->Pan(ox, oy);
}

void treeRingBubble::_strokeStartMotion(int x, int y, int button, int time)
{
  assert(stroke_button != -1);
  stroke_tracking = GL_TRUE;
  stroke_lasttime = time;
}

void treeRingBubble::_strokeStopMotion(int button, unsigned time)
{
  assert(stroke_button != -1);
  stroke_tracking = GL_FALSE;

  /* if (time == stroke_lasttime && stroke_animate) {
    glutIdleFunc(_strokeAnimate);
  } else {
    if (stroke_animate)
      glutIdleFunc(0);
  }*/
}

void treeRingBubble::strokeAnimate(GLboolean animate)
{
  stroke_animate = animate;
}

void treeRingBubble::strokeInit(GLuint button)
{
  stroke_button = button;
  oldMousex = 0;
  oldMousey = 0;
  mousex = 0;
  mousey = 0;
}


void treeRingBubble::searchSharedProtein(int excludePid, bool shareflag)
{

	QList<ItemBase *> mList=_scene->getGroupMembers(this);	
	for( int i = 0; i < mList.size(); i ++)	
	{
		ItemBase *item = mList[i];
		if(item->getType()==EXPRESSIONBUBBLE)
		{
			//PathBubble1 *ptmp = dynamic_cast<PathBubble1 *>(item);			
			//searchSharedProteininAPathway(ptmp->m_pathwayID);			
			expressionBubble *etmp = dynamic_cast<expressionBubble *>(item);				
			searchSharedProteininAExpressionBubble(etmp);	
		}
		if((item->getType()==PATHBUBBLE1 || item->getType()==SUBPATHBUBBLE1) && item->m_pathwayID != excludePid)
		{
			//PathBubble1 *ptmp = dynamic_cast<PathBubble1 *>(item);			
			//searchSharedProteininAPathway(ptmp->m_pathwayID);			
		}
	}	
}

void treeRingBubble::searchExpressed()
{
	QList<ItemBase *> mList=_scene->getGroupMembers(this);	
	for( int i = 0; i < mList.size(); i ++)	
	{
		ItemBase *item = mList[i];
		if(item->getType()==EXPRESSIONBUBBLE)
		{
			//PathBubble1 *ptmp = dynamic_cast<PathBubble1 *>(item);			
			//searchSharedProteininAPathway(ptmp->m_pathwayID);			
			expressionBubble *etmp = dynamic_cast<expressionBubble *>(item);				
			searchExpressedinAExpressionBubble(etmp);	
			_treeRing->SetColorBy(EDGE_BY_EXPRESSION);
			//getPathExpressionFile(etmp->getCurrentFilename());
			//pathExpressionRecord(etmp);
		}		
	}	
	
}

void treeRingBubble::searchSharedProteininAExpressionBubble(expressionBubble *item)
{
	//for(int j=0; j<_scene->_proteinNameD[_scene->_pathBubbles[m_bindPathwayID]->dataID].size(); j++)
	//vector<vector<QString>> pathInfo =pathExpressionRead(item);	
	//set<QString> pathID = pathExpressionReadPathID(item);
	QString eName = item->getCurrentFilename();
	_treeRing->searchMatchedProtein(eName, item->geneInfo,  item->quantity, item->matchedPathways, item->matchedGene_TreeRing_Expression);
	//vector<struct expressedPathwayInfo> infor=_treeRing->searchExpressed(item->geneInfo, item->quantity, item->matchedPathways);
}

void treeRingBubble::searchExpressedinAExpressionBubble(expressionBubble *item)
{
	//for(int j=0; j<_scene->_proteinNameD[_scene->_pathBubbles[m_bindPathwayID]->dataID].size(); j++)
	//_treeRing->searchExpressed(item->geneInfo, item->matchedPathways);
	//vector<vector<QString>> pathInfo =pathExpressionRead(item);	
	//set<QString> pathID = pathExpressionReadPathID(item);
	QString eName = item->getCurrentFilename();
	int tindex = eName.lastIndexOf("/");
	QString temp=eName;
	temp.remove(tindex+1,eName.length());
	_expressionName = eName;
	_expressionName.remove(temp);

	item->expressedPathways = _treeRing->searchExpressed(eName, item->geneInfo, item->quantity, item->matchedPathways, item->expresseGeneLinkedtoTreeRing);
	item->expressionCleared=false;
}

void treeRingBubble::SetEdgeBy(int newToken)
{
	EDGE_BY = newToken;
	if(newToken==EDGE_BY_ORTHOLOGY)
	{
		if(_treeRingBubbleParent==NULL)
		{
			_orthologyFileName=_treeRing->_orthName;
			_name = "Species Reactome Pathway (" + _orthologyFileName + ")";
			if(_labelInitiated)
				resetLabel("Species Reactome Pathway (" + _orthologyFileName + ")");
		}
	}
	_treeRing->SetColorBy(newToken);
}

int treeRingBubble::GetEdgeBy()
{
	//EDGE_BY = newToken;
	//_treeRing->SetColorBy(newToken);
	return _treeRing->GetColorBy();
}

int treeRingBubble::GetPreEdgeBy()
{
	//EDGE_BY = newToken;
	//_treeRing->SetColorBy(newToken);
	return _treeRing->GetPreColorBy();
}	

void treeRingBubble::updateEdgeList(vector<string> oldNodeName, vector<string> newNodeName, vector<vector<int>> &edgeList)
{
	vector<vector<int>> newList;
	for(int i=0; i<edgeList.size(); i++)
	{
		 vector<int> edges;
	     for(int j=0; j<edgeList[i].size(); j++)
		 {
			 string name = oldNodeName[edgeList[i][j]]; 
			 for(int k=0; k < newNodeName.size(); k++)
		     {
				 if(name == newNodeName[k])
				 {
					 edges.push_back(k);
					 break;
				 }
			 }
		 }
		 newList.push_back(edges);
	}
	edgeList=newList;
}

//updateOutList(parent, inList, inputTree[layerNum-1]);

void treeRingBubble::storeTreeRing(treeRingBubble *parent)
{
	// load the simulation results--------------------------

	//int edgeNum=parent->_bun->_rr;
	int layerNum=parent->_treeRing->_ring.size();
	//inputData.resize(layerNum);
	parent->_inputTree_Backup.resize(layerNum);
	parent->_inputNodeSize_Backup.resize(layerNum);
	vector < SimDataElem > _data_Backup = parent->_treeRing->GetData();

	for(int layer=0; layer<layerNum; layer++)
	{
		for(int id=0; id< parent->_treeRing->_ring[layer]._node_num; ++id)
		{
			parent->_inputTree_Backup[layer].push_back(parent->_treeRing->_ring[layer]._node_name[id]);	
			parent->_inputNodeSize_Backup[layer].push_back(parent->_treeRing->_ring[layer]._node_size[id]);	
			//int nodesize = reCountNodeSize(parent->_treeRing, layer, id, itemSelected, false);
		    //int testsize = parent->_treeRing->_ring[layer]._node_size[id];
		    //inputNodeSize[layer].push_back(nodesize); //parent->_treeRing->_ring[layer]._node_size[id]
			if(layer==layerNum-1)
			{
				vector<double> variables = _data_Backup[id]._elem;			
				parent->_inputData_Backup.push_back(variables);		
				
			}
			parent->_outList_Backup.push_back(parent->_bun[layer]->GetOutList(id));//make _bun a vector// changed when switching inner ring out
			parent->_inList_Backup.push_back(parent->_bun[layer]->GetInList(id));
		}	
	}
	
}


void treeRingBubble::updateParentTreeRingAfterCollapse(treeRingBubble *parent, set<vector<int>> itemSelected, int window_w, int window_h)
{
	int type=0;
	if(type==0) //keep selected segment
	{
	    //itemRemoved=itemSelected;
		//_treeRing->itemRemoved=itemSelected;
	
	}
	else if(type==1) //remove selected
	{
	//for _treeRing
	int layerNum=parent->_treeRing->_ring.size();	
    vector<vector<double>> inputData;
	vector<vector<int>> inputNodeSize;
	vector<vector<int>> rateLimitNum;
	vector<vector<string>> inputTree;
	vector<vector<QString>> inputPathID;
	vector<vector<int>> outList, inList;// //_out_list/in_node_list//_rr
	vector<bool> markerFlag;
	vector < SimDataElem > _data = parent->_treeRing->GetData();
	
	inputTree.resize(layerNum);
	inputPathID.resize(layerNum);
	inputNodeSize.resize(layerNum);
	rateLimitNum.resize(layerNum);
	int totalNodes0=0,totalNodes1=0;
	int nodeCount=0;

	for(int layer=0; layer<layerNum; layer++)
	{
		for(int id=0; id< parent->_treeRing->_ring[layer]._node_num; ++id)
		{
			vector<int> item(2,0);
			item[0]=layer; item[1]=id;
			if( itemSelected.find(item)==itemSelected.end() )
			{
				inputTree[layer].push_back( parent->_treeRing->_ring[layer]._node_name[id]);
				inputPathID[layer].push_back( parent->_treeRing->_ring[layer]._node_pathID[id]);
				//inputNodeSize[layer].push_back( parent->_treeRing->_ring[layer]._node_size[id]);
				int nodesize = reCountNodeSize(parent->_treeRing, layer, id, itemSelected, true);
				int testsize=parent->_treeRing->_ring[layer]._node_size[id];
				inputNodeSize[layer].push_back(nodesize); //parent->_treeRing->_ring[layer]._node_size[id]
				rateLimitNum[layer].push_back(parent->_treeRing->_ring[layer]._rateLimitNum[id]);

				if(layer==layerNum-2)
					totalNodes1 = totalNodes1 + parent->_treeRing->_ring[layer]._node_size[id];					
				if(layer==layerNum-3)
					totalNodes0 = totalNodes0 + parent->_treeRing->_ring[layer]._node_size[id];	

				if(layer==layerNum-1)
				{
					nodeCount++;
					vector<double> variables = _data[id]._elem;			
					inputData.push_back(variables);							
					markerFlag.push_back(parent->_lMarker->getMarkerFlag(id));
				}
				outList.push_back(parent->_bun[layer]->GetOutList(id));
				inList.push_back(parent->_bun[layer]->GetInList(id));
			}
		}
	}

	//get ride of link on included
	updateEdgeList(parent->_treeRing->_ring[layerNum-1]._node_name, inputTree[layerNum-1], outList);
	updateEdgeList(parent->_treeRing->_ring[layerNum-1]._node_name, inputTree[layerNum-1], inList);

	//SimData::~SimData();
	delete [] _simData;
	_simData = new SimData(inputData, 18, 0, 18); // keqin's data only has 2 values (2,0,2)
	//_simData = new SimData(dataName, 18, 0, 18); // TODO: OK fixed to 18.
	_simData->PrintIntervalDistribution();

	//TreeRing----------------------------------------------
	//_treeRing = new TreeRing(inputTree, ringRadius, ringWidth, _simData, EDGE_BY, _centre);
	delete [] _treeRing;
	_treeRing =  new TreeRing(inputTree, inputPathID, inputNodeSize, rateLimitNum, ringRadius, ringWidth, _simData, EDGE_BY, _centre);
	this->_node_num = _treeRing->GetNumOfNodes(); // nodes count
	
	// bundle information ----------------------------------
	//int br = _treeRing->GetRingRadius(_treeRing->GetNrLayers()-1); 
	int br = _treeRing->GetRingRadius(0); //changed when switching inner ring out
	
	_bun.clear();
	_bun.resize(layerNum);
	for(int i=0; i<_num_layers; i++)
	{
		_bun[i] = new BundlesPrimitive(_treeRing, outList, inList, _treeRing->_ring[i]._node_num, i, br, _simData, EDGE_BY, _centre);
	    _bun[i]->GenerateCurveBundles(_treeRing, BUDDLINGSTRENGTH);
	}
	//_bun[0]->CalModel();

	// markers ----------------------------------	
	delete [] _lMarker;
	_lMarker = new ExtraLayerMarker(_treeRing, markerFlag, _node_num, br, _simData, _centre);
	
	// labels ----------------------------------
	delete [] _Label;
	_Label = new Label(window_w, window_h, compoudgraph_centre);
	_Label->setScale(1);
	_treeRing->CalculateLabelDisplayOrder();
	_Label->BuildScreenGrid(_treeRing);
	_Label->_num_layers= _treeRing->GetNrLayers();
	}
}

void treeRingBubble::updateConnectionAfterDeletion()
{
	for(int i=0; i<_scene->m_treeRingBubbles.size(); i++)
	{
		if(_scene->m_treeRingBubbles[i]!=NULL)
		{
						
			for (int j = 0; j <_scene->m_treeRingBubbles[i]->_connections.size(); j++)
			{
				struct treeRingConnector node= _scene->m_treeRingBubbles[i]->_connections[j];
				ItemBase *bubble=node.childBubble;				
				if(bubble!=NULL)
				{
					if( bubble->getType()==SUBPATHBUBBLE1 || bubble->getType()==PATHBUBBLE1)
					{	

					}
					else if(bubble->getType()==TREERING)
					{
						treeRingBubble* tbubble= dynamic_cast<treeRingBubble*>(bubble);
						if(tbubble->m_treeRingBubbleID == m_treeRingBubbleID)
						{
							_scene->m_treeRingBubbles[i]->_connections[j].childBubble=NULL;						
							
							_scene->m_treeRingBubbles[i]->_treeRing->itemSelected.erase(node.itemSelected);															
						}
					}
				}				
			}
			vector<struct treeRingConnector> tempConnecton;			
			for (int k = 0; k < _scene->m_treeRingBubbles[i]->_connections.size(); k++)
			{
				if(_scene->m_treeRingBubbles[i]->_connections[k].childBubble!=NULL)
					tempConnecton.push_back(_scene->m_treeRingBubbles[i]->_connections[k]);
			}
			_scene->m_treeRingBubbles[i]->_connections = tempConnecton;											
		}
	}
}


void treeRingBubble::updateConnectionAfterMerge(treeRingBubble *treeRing1, treeRingBubble *treeRing2)
{ //merge treeRing2 to treeRing1

	//let child bubble of treering2 to be child bubble of treering1
	for (int j = 0; j <treeRing2->_connections.size(); j++)
	{
		struct treeRingConnector node= treeRing2->_connections[j];
		ItemBase *bubble=node.childBubble;
		if(bubble!=NULL)
		{
			if( bubble->getType()==SUBPATHBUBBLE1 || bubble->getType()==PATHBUBBLE1)
			{	
				treeRingBubble* tbubble= dynamic_cast<treeRingBubble*>(bubble);
				if(tbubble!=treeRing1)
				{
					treeRing1->_connections.push_back(node);					
				}				
			}
			else if(bubble->getType()==TREERING)
			{
				treeRingBubble* tbubble= dynamic_cast<treeRingBubble*>(bubble);
				if(tbubble!=treeRing1)
				{
					treeRing1->_connections.push_back(node);					
				}						
			}
		}
	}	
	for(int i=0; i<_scene->m_treeRingBubbles.size(); i++)
	{
		//let parent bubble of treering2 to be parent bubble of treering1
		if(_scene->m_treeRingBubbles[i]!=NULL)
		{
			for (int j = 0; j <_scene->m_treeRingBubbles[i]->_connections.size(); j++)
		    {
				struct treeRingConnector node= _scene->m_treeRingBubbles[i]->_connections[j];
				ItemBase *bubble=node.childBubble;
				if(bubble!=NULL)
				{
					if(bubble->getType()==TREERING)
					{
						treeRingBubble* tbubble= dynamic_cast<treeRingBubble*>(bubble);
						if(_scene->m_treeRingBubbles[i]!=treeRing1)
						{
							if(tbubble->m_treeRingBubbleID == treeRing2->m_treeRingBubbleID)
							{
								if(_scene->m_treeRingBubbles[i]->_connections[j].childBubble==treeRing2)
									_scene->m_treeRingBubbles[i]->_connections[j].childBubble=treeRing1;	
								//current itemselected
								//_scene->m_treeRingBubbles[i]->_treeRing->itemSelected.erase(node.itemSelected);															
							}
						}
						else
						{
							if(tbubble->m_treeRingBubbleID == treeRing2->m_treeRingBubbleID)
							{
								 _scene->m_treeRingBubbles[i]->_connections[j].childBubble=NULL;	
							     _scene->m_treeRingBubbles[i]->_treeRing->itemSelected.erase(node.itemSelected);
							}
						}
					}
				}
			}
			vector<struct treeRingConnector> tempConnecton;
			for (int k = 0; k < _scene->m_treeRingBubbles[i]->_connections.size(); k++)
			{
				if(_scene->m_treeRingBubbles[i]->_connections[k].childBubble!=NULL)
				{
					tempConnecton.push_back(_scene->m_treeRingBubbles[i]->_connections[k]);
				}
			}
			_scene->m_treeRingBubbles[i]->_connections=tempConnecton;	
		}
	}
}

void treeRingBubble::MergeTreeRings(treeRingBubble *treeRing1, treeRingBubble *treeRing2, int window_w, int window_h)
{
	//for _treeRing
	//delete treeRing2, and merge it to treeRing1;
	vector<vector<int>> recordOrder;
	vector<vector<int>> itemSelected;
	vector<vector<int>> newItems;
	
	int layerNum1=treeRing1->_treeRing->_ring.size();	
    //vector < SimDataElem > _data1 = treeRing1->_treeRing->GetData();
	
	//for(int layer=0; layer<layerNum1; layer++)
	//check if this treeRing2 node is already in treeRing1
	int layer=layerNum1-1;
	{
		for(int id=0; id< treeRing2->_treeRing->_ring[layer]._node_num; ++id)
		{
			vector<string> names;
			vector<int> ids;
			ids.resize(layerNum1);
			int pid=id;
			for(int k=layer; k>=0; k--)
			{
				names.push_back(treeRing2->_treeRing->_ring[k]._node_name[pid]);
				ids[k]=pid;
				pid=treeRing2->_treeRing->_ring[k]._parent[pid];
			}
			bool flag=false;
			for(int id1=0; id1< treeRing1->_treeRing->_ring[layer]._node_num; ++id1)
			{
				flag=true;
				string name1;
				int pid=id1;
				
				for(int k=layer; k>=0; k--)
				{
					name1 = treeRing1->_treeRing->_ring[k]._node_name[pid];
					if(name1!=names[layer-k])
					{
						flag=false;
						break;
					}
					pid = treeRing1->_treeRing->_ring[k]._parent[pid];
				}
				if(flag)
				{
					break;
				}
			}
			if(!flag) //add it to the list
			{
				itemSelected.push_back(ids);
			}
		}
	}
    vector<vector<int>> order;
	order.resize(layerNum1);
	/*for(int layer=0; layer<layerNum1; layer++)
	{
		for(int id=0; id < treeRing1->_treeRing->_ring[layer]._node_num; ++id)
		{
			order[layer].push_back(id);	
		}
	}*/
    vector < SimDataElem > _data = treeRing2->_treeRing->GetData();

	//get names of top levels
	set<vector<QString>> topNames;
	
	{
		for(int id=0; id < treeRing1->_treeRing->_ring[0]._node_num; ++id)
		{
			vector<QString> item;
			string n1=treeRing1->_treeRing->_ring[0]._node_name[id];
			QString Name1=QString::fromStdString(n1);			
			item.push_back(Name1);

			QString ID=QString::number(id);			
			item.push_back(ID);
			topNames.insert(item);			
		}
	}

	
	for(vector<vector<int>>::iterator it=itemSelected.begin(); it!=itemSelected.end(); it++)
	{
		int ring_1_Size=treeRing1->_treeRing->_ring[0]._node_name.size();
	    vector<int> item = *it;
	    int pid=-1;
		int oriPid;
		for(int i=0; i<item.size(); i++)
		{   
			int layer=i, id=item[i];
			if(layer==0)
			{
				vector<QString> Names;
				string n1 = treeRing2->_treeRing->_ring[0]._node_name[id];
				QString Name1=QString::fromStdString(n1);			
			    Names.push_back(Name1);

				QString ID=QString::number(id+ring_1_Size);			
			    Names.push_back(ID);
				topNames.insert(Names);				
			}			
		}		
	}
	order.resize(_treeRing->GetNrLayers());
	for(set<vector<QString>>::iterator it=topNames.begin(); it != topNames.end(); it++)
	{
		vector<QString> Names = *it;	 
		int id = Names[1].toUInt();
		int ring_1_Size = treeRing1->_treeRing->_ring[0]._node_name.size();
		order[0].push_back(id);	
		vector<vector<int>> output;
		if(id < ring_1_Size )
		{
			treeRing1->_treeRing->findAllDescendantNodes(0, id, output);
			for(int i=1; i<output.size();i++)
			{
				for(int j=0; j<output[i].size();j++)
				{
					order[i].push_back(output[i][j]);
				}
			}
		}
		else
		{
		    treeRing2->_treeRing->findAllDescendantNodes(0, id-ring_1_Size, output);
			for(int i=1; i<output.size();i++)
			{
				int ring_2_Size = treeRing1->_treeRing->_ring[i]._node_name.size();
				for(int j=0; j<output[i].size();j++)
				{
					order[i].push_back(output[i][j]+ring_2_Size);
				}
			}
		}
	}
	for(int layer=0; layer<order.size(); layer++)
	{
		int ring_1_Size=treeRing1->_treeRing->_ring[layer]._node_name.size();
		for(int id=0; id < order[layer].size(); ++id)
		{
			//order[layer].push_back(id);
			int ID=order[layer][id];	
			vector<int> item(2,0);
			item[0]=layer; item[1]=ID;
			newItems.push_back(item);			
		}
	}
    
	// load the simulation results--------------------------
	vector<vector<double>> inputData;
	vector<vector<int>> inputNodeSize;
	vector<vector<int>> rateLimitNum;
	vector<vector<string>> inputTree;
	vector<vector<QString>> inputPathID;
	
	vector<set<string>> treeSet;

	vector<vector<int>> outList, inList, outList1, inList1, outList2, inList2;// //_out_list/in_node_list//_rr
	vector<bool> markerFlag, markerFlag1, markerFlag2;
	//int edgeNum=parent->_bun->_rr;
	int layerNum=treeRing1->_treeRing->_ring.size();
	//inputData.resize(layerNum);
	inputTree.resize(layerNum);
	inputPathID.resize(layerNum);
	treeSet.resize(layerNum); 
	inputNodeSize.resize(layerNum);
	rateLimitNum.resize(layerNum);
	vector < SimDataElem > _data1 = treeRing1->_treeRing->GetData();
	vector < SimDataElem > _data2 = treeRing2->_treeRing->GetData();
	int totalNodes0=0,totalNodes1=0;
	int nodeCount=0;
	
	//vector<set<int>> IDs;
	vector<vector<int>> nodeIDs;
	vector<set<int>> nodeIDSet;
	nodeIDs.resize(layerNum);
	nodeIDSet.resize(layerNum);
	//IDs.resize(layerNum);
	for(vector<vector<int>>::iterator it=newItems.begin(); it!=newItems.end(); it++)
	{
	    vector<int> item = *it;
		int layer=item[0], id=item[1];
		int ring_1_Size=treeRing1->_treeRing->_ring[layer]._node_name.size();		
		//IDs[layer].insert(id);
		if(id < ring_1_Size )
		{
			string pname=treeRing1->_treeRing->_ring[layer]._node_name[id];
			QString pnum=treeRing1->_treeRing->_ring[layer]._node_pathID[id];
						   
			//IDs[layer].insert(id);
			treeSet[layer].insert(pname);
			inputTree[layer].push_back( pname);	
			inputPathID[layer].push_back( pnum);	
			inputNodeSize[layer].push_back( treeRing1->_treeRing->_ring[layer]._node_size[id]);
			rateLimitNum[layer].push_back( treeRing1->_treeRing->_ring[layer]._rateLimitNum[id]);
			nodeIDs[layer].push_back(id);
			 nodeIDSet[layer].insert(id);
			if(layer==layerNum-1)
			{			
				vector<double> variables = _data1[id]._elem;			
				inputData.push_back(variables);		
				//outList1.push_back(treeRing1->_bun->GetOutList(id));
				//inList1.push_back(treeRing1->_bun->GetInList(id));
				
				markerFlag1.push_back(treeRing1->_lMarker->getMarkerFlag(id));
			}			
        }
		else 
		{
			id=id-ring_1_Size;	
			string pname=treeRing2->_treeRing->_ring[layer]._node_name[id];
			QString pnum=treeRing2->_treeRing->_ring[layer]._node_pathID[id];
            { 
				//IDs[layer].insert(id+ring_1_Size);
				treeSet[layer].insert(pname);
				inputTree[layer].push_back(pname);	
				inputPathID[layer].push_back(pnum);	
				inputNodeSize[layer].push_back( treeRing2->_treeRing->_ring[layer]._node_size[id]);
				rateLimitNum[layer].push_back( treeRing2->_treeRing->_ring[layer]._rateLimitNum[id]);
				
				nodeIDs[layer].push_back(id+ring_1_Size);
			    nodeIDSet[layer].insert(id+ring_1_Size);
				if(layer==layerNum-1)
				{
					vector<double> variables = _data2[id]._elem;			
					inputData.push_back(variables);		
					//outList2.push_back(treeRing2->_bun->GetOutList(id));
					//inList2.push_back(treeRing2->_bun->GetInList(id));
					
					markerFlag2.push_back(treeRing2->_lMarker->getMarkerFlag(id));					
				}
			}
		}		
	}
	
	for(vector<vector<int>>::iterator it=newItems.begin(); it!=newItems.end(); it++)
	{
	    vector<int> item = *it;
		int layer=item[0], id=item[1];
		int ring_1_Size=treeRing1->_treeRing->_ring[layer]._node_name.size();		
		//IDs[layer].insert(id);
		if(id < ring_1_Size )
		{
			if(layer==layerNum-1)
			{	
				vector<int> oID=treeRing1->_bun[layer]->GetOutList(id), iID=treeRing1->_bun[layer]->GetInList(id);			        			
				vector<int> oid= getNewNodeID(layer, oID, nodeIDs[layer], nodeIDSet[layer], 0, treeRing1->_treeRing, treeRing2->_treeRing), iid= getNewNodeID(layer, iID, nodeIDs[layer], nodeIDSet[layer], 0, treeRing1->_treeRing, treeRing2->_treeRing);
				outList1.push_back(oid);			
		        inList1.push_back(iid);				
			}			
        }
		else 
		{
			id=id-ring_1_Size;				
            { 
				if(layer==layerNum-1)
				{
					vector<int> oID=treeRing2->_bun[layer]->GetOutList(id), iID=treeRing2->_bun[layer]->GetInList(id);			        			
					vector<int> oid= getNewNodeID(layer, oID, nodeIDs[layer], nodeIDSet[layer], ring_1_Size, treeRing1->_treeRing, treeRing2->_treeRing), iid= getNewNodeID(layer, iID, nodeIDs[layer], nodeIDSet[layer], ring_1_Size, treeRing1->_treeRing, treeRing2->_treeRing);
					outList2.push_back(oid);			
		            inList2.push_back(iid);
				}
			}
		}		
	}

	for (int j = 0; j <treeRing2->_connections.size(); j++)
	{
		struct treeRingConnector node= treeRing2->_connections[j];		
		int ring_1_Size=treeRing1->_treeRing->_ring[layer]._node_name.size();	
		int layer=node.layer;
		node.idx= getNewNodeID(layer, node.idx, nodeIDs[layer], nodeIDSet[layer], ring_1_Size, treeRing1->_treeRing, treeRing2->_treeRing);			
		set<vector<int>> newItemSelected;
		for(set<vector<int>>::iterator it=node.itemSelected.begin(); it!=node.itemSelected.end(); it++)
		{
			vector<int> item = *it;
			for(int j =0 ; j< item.size(); j=j+2)
			{
				int clayer=item[j], cid=item[j+1];
				item[j+1] = getNewNodeID(clayer, cid, nodeIDs[clayer], nodeIDSet[clayer], treeRing1->_treeRing->_ring[clayer]._node_name.size(), treeRing1->_treeRing, treeRing2->_treeRing);						   
			}
 		    newItemSelected.insert(item);
		}
		node.itemSelected = newItemSelected;
		treeRing2->_connections[j] = node; 
	}

	for (int j = 0; j <treeRing1->_connections.size(); j++)
	{
		struct treeRingConnector node= treeRing1->_connections[j];		
		//int ring_1_Size=treeRing1->_treeRing->_ring[layer]._node_name.size();	
		int layer=node.layer;
		node.idx= getNewNodeID(layer, node.idx, nodeIDs[layer], nodeIDSet[layer], 0, treeRing1->_treeRing, treeRing2->_treeRing);			
		set<vector<int>> newItemSelected;
		for(set<vector<int>>::iterator it=node.itemSelected.begin(); it!=node.itemSelected.end(); it++)
		{
			vector<int> item = *it;
			for(int j =0 ; j< item.size(); j=j+2)
			{
				int clayer=item[j], cid=item[j+1];
				item[j+1] = getNewNodeID(clayer, cid, nodeIDs[clayer], nodeIDSet[clayer], 0, treeRing1->_treeRing, treeRing2->_treeRing);						   
			}
 		    newItemSelected.insert(item);
		}
		node.itemSelected = newItemSelected;
		treeRing1->_connections[j] = node; 
	}
	//get ride of link on included
	//updateEdgeList(treeRing1->_treeRing->_ring[layerNum-1]._node_name, inputTree[layerNum-1], outList1);
	//updateEdgeList(treeRing1->_treeRing->_ring[layerNum-1]._node_name, inputTree[layerNum-1], inList1);

	//updateEdgeList(treeRing2->_treeRing->_ring[layerNum-1]._node_name, inputTree[layerNum-1], outList2);
	//updateEdgeList(treeRing2->_treeRing->_ring[layerNum-1]._node_name, inputTree[layerNum-1], inList2);

	outList=outList1, inList=inList1, markerFlag = markerFlag1;
	for(int i=0; i<outList2.size(); i++)
	{
	    outList.push_back(outList2[i]);
		inList.push_back(inList2[i]);
		markerFlag.push_back(markerFlag2[i]);
	}
	//--- updateEdgeList(treeRing1->_treeRing->_ring[layerNum-1]._node_name, inputTree[layerNum-1], outList);//
	//--- updateEdgeList(treeRing1->_treeRing->_ring[layerNum-1]._node_name, inputTree[layerNum-1], inList);//
		
	//SimData::~SimData();
	delete [] treeRing1->_simData;
	treeRing1->_simData = new SimData(inputData, 18, 0, 18); // keqin's data only has 2 values (2,0,2)
	//_simData = new SimData(dataName, 18, 0, 18); // TODO: OK fixed to 18.
	treeRing1->_simData->PrintIntervalDistribution();

	//TreeRing----------------------------------------------
	//_treeRing = new TreeRing(inputTree, ringRadius, ringWidth, _simData, EDGE_BY, _centre);
	QString oFileName=treeRing1->_treeRing->getOrthFileName();
	delete [] treeRing1->_treeRing;
	treeRing1->_treeRing =  new TreeRing(inputTree, inputPathID, inputNodeSize, rateLimitNum, treeRing1->ringRadius, treeRing1->ringWidth, treeRing1->_simData, treeRing1->EDGE_BY, treeRing1->_centre, oFileName);
	treeRing1->_node_num = treeRing1->_treeRing->GetNumOfNodes(); // nodes count
	
	float min=_treeRing->eMin, max=_treeRing->eMax;

	// bundle information ----------------------------------
	//int br = treeRing1->_treeRing->GetRingRadius(treeRing1->_treeRing->GetNrLayers()-1);
	int br = _treeRing->GetRingRadius(0); //changed when switching inner ring out
	
	treeRing1->_bun.clear();	
	treeRing1->_bun.resize(layerNum1);
	for(int i=0; i<_num_layers; i++)
	{
		treeRing1->_bun[i] = new BundlesPrimitive(treeRing1->_treeRing, outList, inList, treeRing1->_treeRing->_ring[i]._node_num, i, br, treeRing1->_simData, treeRing1->EDGE_BY, treeRing1->_centre);
		//treeRing1->_bun->_num_layers= treeRing1->_treeRing->GetNrLayers();
		//_bun = new BundlesPrimitive(_treeRing, edgeNum, outList, inList, graphName, _node_num, br, _simData, EDGE_BY, _centre);
		treeRing1->_bun[i]->GenerateCurveBundles(treeRing1->_treeRing, BUDDLINGSTRENGTH);
	}
	//treeRing1->_bun[0]->CalModel();
	
	// makers ----------------------------------
	delete [] treeRing1->_lMarker;
	treeRing1->_lMarker = new ExtraLayerMarker(treeRing1->_treeRing, markerFlag, treeRing1->_node_num, br, _simData,  _centre);
	//treeRing1->_lMarker->GenerateMarkers(treeRing1->_treeRing);
	// labels ----------------------------------
	delete [] treeRing1->_Label;
	treeRing1->_Label = new Label(window_w, window_h, treeRing1->compoudgraph_centre);
	treeRing1->_Label->setScale(1);
	treeRing1->_treeRing->CalculateLabelDisplayOrder();
	treeRing1->_Label->BuildScreenGrid(treeRing1->_treeRing);
	treeRing1->_Label->_num_layers= treeRing1->_treeRing->GetNrLayers();

	if(!treeRing1->_name.contains("Species Reactome Pathway"))	
	{
		//treeRing1->_name = ;
		treeRing1->resetLabel(treeRing1->getTreeRingName(treeSet[0]));
	}

	treeSet.clear();
	inputData.clear();
	inputNodeSize.clear();
	rateLimitNum.clear();
	inputTree.clear();
	inputPathID.clear();
	
	outList.clear();
	inList.clear(); 
	outList1.clear(); 
	inList1.clear(); 
	outList2.clear();
	inList2.clear();// //_out_list/in_node_list//_rr
	markerFlag.clear();
	markerFlag1.clear(); 
	markerFlag2.clear();
	//int edgeNum=parent->_bun->_rr;
	
	_data1.clear();
	_data2.clear();	
}

vector<int> treeRingBubble::getNewNodeID(int layer, vector<int> oID, vector<int> nodeIDs, set<int> nodeIDSet, int ring_1_Size, TreeRing *_treeRing1, TreeRing *_treeRing2)
{
    vector<int> oid;
	int id;
	int ID;
	if(!oID.empty())
	{
		for(int j =0 ; j< oID.size(); j++ )
		{
			ID = oID[j] + ring_1_Size;
			if(nodeIDSet.find(ID)!=nodeIDSet.end())
			{
				id=-1;
				for(int i=0;i<nodeIDs.size(); i++)
				{
					if(nodeIDs[i] == ID)
					{	
						id=i;						
						break;
					}
				}	
				if(id<0)
				{
				   //search inputtree
					vector<string> Names = _treeRing2-> findNameList(layer, oID[j]);
					id = oID[j];
		            _treeRing1->locateNodeByNameList(Names, 0, id, 0, 0);
					if(nodeIDSet.find(id)!=nodeIDSet.end())
					{
						for(int i=0;i<nodeIDs.size(); i++)
						{
							if(nodeIDs[i] == id)
							{	
								id=i;						
								break;
							}
						}	
					}
				}
                if(id>=0)
					oid.push_back(id);									
			}
		}
	}	
	return oid;
}

int treeRingBubble::getNewNodeID(int layer, int oID, vector<int> nodeIDs, set<int> nodeIDSet, int ring_1_Size, TreeRing *_treeRing1, TreeRing *_treeRing2)
{
    int oid=-1;
	int ID;
	if(oID>=0)
	{
			ID = oID + ring_1_Size;
			if(nodeIDSet.find(ID)!=nodeIDSet.end())
			{
				for(int i=0;i<nodeIDs.size(); i++)
				{
					if(nodeIDs[i] == ID)
					{	
						oid=i;									
						break;
					}
				}	
			}
	}
	if(oid<0)
	{  
	    //search inputTree
		//get name list
		vector<string> Names = _treeRing2-> findNameList(layer, oID);
		oid = oID;
		_treeRing1->locateNodeByNameList(Names, 0, oid, 0, 0);
		if(nodeIDSet.find(oid)!=nodeIDSet.end())
		{
			for(int i=0;i<nodeIDs.size(); i++)
			{
				if(nodeIDs[i] == oid)
				{	
					oid=i;						
					break;
				}
			}	
		}
	}
	return oid;
}

int treeRingBubble::getNewNodeID(int layer, int oID, TreeRing *_preTreeRing, TreeRing *_curtreeRing)
{
    int oid=-1;
	int ID;
	
	//search inputTree
	//get name list
	vector<string> Names = _preTreeRing-> findNameList(layer, oID);
	//oid = oID;
	_curtreeRing->locateNodeByNameList(Names, 0, oid, 0, 0);	
	return oid;
}

void treeRingBubble::InsertToOrder(int pid, int count, vector<int> &order)
{
	if(pid<=order.size() &&pid>=1)
	{
		if(order[pid-1] ==count )
			return;
	}
	if(pid>=order.size())
	{
		order.push_back(count);
	}
	else
	{
		order.resize(order.size()+1);
		for(int k=order.size()-1; k>pid; k--)
		{
			order[k]=order[k-1];
		}
		order[pid]=count;
	}
}

void treeRingBubble::initSubTreeRing(treeRingBubble *parent, set<vector<int>> itemSelected, int x, int y, int window_w, int window_h)
{
	this->_ring_radius = ringRadius;
	this->_ring_width = ringWidth;
	this->_hl = -1;
	this->_hn = -1;
	this->EDGE_BY = parent->EDGE_BY; //EDGE_BY_V; //EDGE_BY_ORTHOLOGY;	//keqin
	
	this->_centre.x=x, this->_centre.y=y;
		
	// load the simulation results--------------------------
	vector<vector<double>> inputData;
	vector<vector<int>> inputNodeSize;
	vector<vector<string>> inputTree;
	vector<vector<QString>> inputPathID;
	vector<vector<int>> rateLimitNum;
	vector<vector<int>> outList, inList;// //_out_list/in_node_list//_rr
	vector<bool> markerFlag;
	//int edgeNum=parent->_bun->_rr;
	int layerNum=parent->_treeRing->_ring.size();
	//inputData.resize(layerNum);
	inputTree.resize(layerNum);
	inputPathID.resize(layerNum);

	inputNodeSize.resize(layerNum);
	rateLimitNum.resize(layerNum);
	vector < SimDataElem > _data = parent->_treeRing->GetData();
	int totalNodes0=0,totalNodes1=0;
	int nodeCount=0;
	vector<int> nodeIDs;
	set<int> nodeIDSet;
	for(set<vector<int>>::iterator it=itemSelected.begin(); it!=itemSelected.end(); it++)
	{
	    vector<int> item = *it;
		int layer=item[0], id=item[1];
		inputTree[layer].push_back(parent->_treeRing->_ring[layer]._node_name[id]);	
		inputPathID[layer].push_back(parent->_treeRing->_ring[layer]._node_pathID[id]);
		
		int nodesize = reCountNodeSize(parent->_treeRing, layer, id, itemSelected, false);
		int testsize=parent->_treeRing->_ring[layer]._node_size[id];
		
		inputNodeSize[layer].push_back(nodesize); //parent->_treeRing->_ring[layer]._node_size[id]
		rateLimitNum[layer].push_back(parent->_treeRing->_ring[layer]._rateLimitNum[id]);	

		if(layer==layerNum-2)
		    totalNodes1 = totalNodes1 + parent->_treeRing->_ring[layer]._node_size[id];
		if(layer==layerNum-3)
		    totalNodes0 = totalNodes0 + parent->_treeRing->_ring[layer]._node_size[id];
		if(layer==layerNum-1)
		{
			nodeCount++;
		}
        if(layer==layerNum-1)
		{
			vector<double> variables = _data[id]._elem;			
			inputData.push_back(variables);	
			nodeIDs.push_back(id);
			nodeIDSet.insert(id);
			//outList.push_back(parent->_bun->GetOutList(id));
		    //inList.push_back(parent->_bun->GetInList(id));
			markerFlag.push_back(parent->_lMarker->getMarkerFlag(id));
		}
	}
	//edge list
	for(set<vector<int>>::iterator it=itemSelected.begin(); it!=itemSelected.end(); it++)
	{
	    vector<int> item = *it;
		int layer=item[0], id=item[1];
		if(layer==layerNum-1)
		{	
			vector<int> oID=parent->_bun[layer]->GetOutList(id), iID=parent->_bun[layer]->GetInList(id), oid, iid;
			if(!oID.empty())
			{
				for(int j =0 ; j< oID.size(); j++ )
				{
					if(nodeIDSet.find(oID[j])!=nodeIDSet.end())
					{
						for(int i=0;i<nodeIDs.size(); i++)
						{
							if(nodeIDs[i] == oID[j])
							{	
								oid.push_back(i);									
								break;
							}
						}	
					}
				}
			}
			
			if(!iID.empty())
			{
				for(int j =0 ; j< iID.size(); j++ )
				{
					if(nodeIDSet.find(iID[j])!=nodeIDSet.end())
					{
						for(int i=0;i<nodeIDs.size(); i++)
						{
							if(nodeIDs[i] == iID[j])
							{	
								iid.push_back(iID[j]);									
								break;
							}
						}	
					}
				}
			}
			outList.push_back(oid);			
		    inList.push_back(iid);			
		}
	}
	//get ride of link on included
	//updateEdgeList(parent->_treeRing->_ring[layerNum-1]._node_name, inputTree[layerNum-1], outList);
	//updateEdgeList(parent->_treeRing->_ring[layerNum-1]._node_name, inputTree[layerNum-1], inList);

	_simData = new SimData(inputData, 18, 0, 18); // keqin's data only has 2 values (2,0,2)
	//_simData = new SimData(dataName, 18, 0, 18); // TODO: OK fixed to 18.
	_simData->PrintIntervalDistribution();
	
	//TreeRing----------------------------------------------
	//_treeRing = new TreeRing(inputTree, ringRadius, ringWidth, _simData, EDGE_BY, _centre);
	_treeRing =  new TreeRing(inputTree, inputPathID, inputNodeSize, rateLimitNum, ringRadius, ringWidth, _simData, EDGE_BY, _centre, parent->_treeRing->getOrthFileName());
	this->_node_num = _treeRing->GetNumOfNodes(); // nodes count
	_num_layers = _treeRing->GetNrLayers();
	// bundle information ----------------------------------
	//int br = _treeRing->GetRingRadius(_treeRing->GetNrLayers()-1);
	int br = _treeRing->GetRingRadius(0); //changed when switching inner ring out
	
	_bun.resize(_num_layers);
	for(int i=0; i<_num_layers; i++)
	{
		_bun[i] = new BundlesPrimitive(_treeRing, outList, inList, _treeRing->_ring[i]._node_num, i, br, _simData, EDGE_BY, _centre);
		//_bun->_num_layers= _treeRing->GetNrLayers();
		//_bun = new BundlesPrimitive(_treeRing, edgeNum, outList, inList, graphName, _node_num, br, _simData, EDGE_BY, _centre);
		_bun[i]->GenerateCurveBundles(_treeRing, BUDDLINGSTRENGTH/*bundling strength*/);
	}
	//_bun[0]->CalModel();


	// makers ----------------------------------
	//delete [] _lMarker;
	_lMarker = new ExtraLayerMarker(_treeRing, markerFlag, _node_num, br, _simData, _centre);
	//_lMarker->GenerateMarkers(_treeRing);

	// labels ----------------------------------
	_Label = new Label(window_w, window_h, compoudgraph_centre);
	_Label->setScale(1);
	_treeRing->CalculateLabelDisplayOrder();
	_Label->BuildScreenGrid(_treeRing);
	_Label->_num_layers= _treeRing->GetNrLayers();
	//FINISH_INIT = true;
}

int treeRingBubble::reCountNodeSize(TreeRing *_treeRing, int layer, int id, set<vector<int>> itemSelected, bool Nflag)
{
	//if Nflag==true, count those not in itemselected
	if(layer == _treeRing->_ring.size()-1)
		return 1;

	set<vector<int>> toBeInsert;
	int count=0;
	_treeRing->findAllDescendantNodes(layer, id, toBeInsert);
	for(set<vector<int>>::iterator it=toBeInsert.begin(); it!=toBeInsert.end(); it++)
	{    
		vector<int> item = *it;
		if(item[0]==_treeRing->_ring.size()-1)
		{
			if(Nflag)
			{
					if(itemSelected.find(item)==itemSelected.end())
					count++;
			}
			else
			{
					if(itemSelected.find(item)!=itemSelected.end())
					count++;
			}
		}
	}
	 /*   
	int count=0;
	if(layer==0 && _num_layers > 2 ) 
	{

		for(int j=_treeRing->_ring[layer]._child_range[id].first; j<=_treeRing->_ring[layer]._child_range[id].second; j++)
		{
			for(int k=_treeRing->_ring[layer+1]._child_range[j].first; k<=_treeRing->_ring[layer+1]._child_range[j].second; k++)
			{
				vector<int> item(2,0);
			    item[0]=layer+2, item[1]=k;
			    if(Nflag)
				{
					 if(itemSelected.find(item)==itemSelected.end())
						count++;
				}
				else
				{
					 if(itemSelected.find(item)!=itemSelected.end())
						count++;
				}
			}
		}
	}
	else
	{
		for(int j=_treeRing->_ring[layer]._child_range[id].first; j<=_treeRing->_ring[layer]._child_range[id].second; j++)
		{
			 vector<int> item(2,0);
			 item[0]=layer+1, item[1]=j;
			 if(Nflag)
			 {
				 if(itemSelected.find(item)==itemSelected.end())
				    count++;
			 }
		     else
			 {
				 if(itemSelected.find(item)!=itemSelected.end())
				    count++;
			 }
		}
	}	*/
	return count;
}

void treeRingBubble::initTreeRing(int x, int y, int window_w, int window_h)
{	
	this->_ring_radius = ringRadius;
	this->_ring_width = ringWidth;
	this->_hl = -1;
	this->_hn = -1;
	this->EDGE_BY = EDGE_BY_V;	//EDGE_BY_ORTHOLOGY
	
	this->_centre.x=x, this->_centre.y=y; //_out_list/in_node_list//_rr
		
	// load the simulation results--------------------------
	_simData = new SimData(dataName, 18, 0, 18); // keqin's data only has 2 values (2,0,2)
	//_simData = new SimData(dataName, 18, 0, 18); // TODO: OK fixed to 18.
	_simData->PrintIntervalDistribution();

	//TreeRing----------------------------------------------
	_treeRing = new TreeRing(treeName, ringRadius, ringWidth, _simData, EDGE_BY, _centre);
	this->_node_num = _treeRing->GetNumOfNodes(); // nodes count
	_num_layers = _treeRing->GetNrLayers();
	// bundle information ----------------------------------	
	int br = _treeRing->GetRingRadius(0); //changed when switching inner ring out
	_bun.resize(_num_layers);
	for(int i=0; i<_num_layers; i++)
	{
		//_bun[i] = new BundlesPrimitive(_treeRing, graphName, _node_num, i, br, _simData, EDGE_BY, _centre);
		_bun[i] = new BundlesPrimitive(_treeRing, graphName, _treeRing->_ring[i]._node_num, _node_num, i, _treeRing->GetRingRadius(i), _simData, EDGE_BY, _centre);
		_bun[i]->GenerateCurveBundles(_treeRing, BUDDLINGSTRENGTH);
		//_bun[i]->CalModel();
	}

	// makers ----------------------------------
	//delete [] _lMarker;
	_lMarker = new ExtraLayerMarker(_treeRing, extraLayerName, _node_num, br, _simData, _centre);
	//_lMarker->GenerateMarkers(_treeRing);
	// labels ----------------------------------
	_Label = new Label(window_w, window_h, compoudgraph_centre);
	_Label->setScale(1);
	_treeRing->CalculateLabelDisplayOrder();
	_Label->BuildScreenGrid(_treeRing);
	_Label->_num_layers= _treeRing->GetNrLayers();
	//FINISH_INIT = true;
}




treeRingBubble::~treeRingBubble()
{	
	
	updateConnectionAfterDeletion();
	//qtbox
	itemSelected.clear();
	_contain.clear();
	_lines.clear(); 
	fills.clear();
	_treeRing->itemSelected.clear();

	sharedExpressionProtein.clear(); 
	expressedProtein.clear();  expressedComplex.clear();  expressedSmallMolecule.clear();  expressedDna.clear();  expressedReaction.clear();  expressedPhysicalEntity.clear();  expressedANode.clear();  expressedCompartment.clear();  expressedPathway.clear(); 	
	sharedProtein.clear();  sharedComplex.clear();  sharedSmallMolecule.clear();  sharedDna.clear();  sharedReaction.clear();  sharedPhysicalEntity.clear();  sharedANode.clear(); sharedCompartment.clear();  sharedPathway.clear(); 
	linkedProtein.clear();  linkedComplex.clear();  linkedSmallMolecule.clear();  linkedDna.clear();  linkedReaction.clear();  linkedPhysicalEntity.clear();  linkedANode.clear(); linkedCompartment.clear();  linkedPathway.clear(); 	
	differedProtein.clear();  differedComplex.clear();  differedSmallMolecule.clear();  differedDna.clear();  differedReaction.clear();  differedPhysicalEntity.clear(); differedANode.clear();  differedCompartment.clear();  differedPathway.clear(); 
	linkedEdge.clear(); 
	_itemColored.clear(); 
	foundlist.clear(); 
	//_linkage.clear(); 	
	highlightedEdge.clear(); 
	hoveredItem.clear(); 
	eventStored.clear(); 	
	sharedStored.clear();  
	differedStored.clear();  
	expressedStored.clear(); 
	highlightedStored.clear(); 
	linkedStored.clear(); 
	linkedStoredinHead.clear(); 				
	pathwayGraphStored.clear(); 	

	if(m_treeRingBubbleID>=0)
	   _scene->m_treeRingBubbles[m_treeRingBubbleID]=NULL;	
	m_treeRingBubbleID=-1;

	delete _treeRing;
	_bun.clear();
	delete _lMarker;	
}


void treeRingBubble::undo()
{
	//clearExpressed();
	//getExpressionToBePaint();
}

void treeRingBubble::redo()
{
	//clearExpressed();
	//getExpressionToBePaint();
}

void treeRingBubble::deleteItems()
{
	/*complexPos.clear();
	proteinPos.clear();	
    smallMoleculePos.clear();	
    DnaPos.clear();	
    reactionPos.clear();	
    physicalEntityPos.clear();	
	edge.clear();*/
}

void treeRingBubble::loadFile(const QString &fileName)
{
	setCurrentFile(fileName);	
}

void treeRingBubble::resetLabel(QString name)
{
	int tindex;	
	QString temp;	
	_name = name;
	_label->setName(_name);		
	_label->updateLabel( m_size_width, m_size_height );			
}

void treeRingBubble::updateLabel()
{   	
	int tindex;	
	QString temp;	
	if(!_labelInitiated)
	{
		if (_LABEL == false)
		{
			_label = new ItemLabel( this, _scene, m_size_width, m_size_height );
			_label->setLabelColor(_colorBoarder);
			this->_scene->addItem(_label);
			_LABEL = true;
			//emit updateLabelState( this );
			return;
		}
		_label->setName(_name);		
		_label->updateLabel( m_size_width, m_size_height );		
		_labelInitiated=true;
	}
	if(!_label->isVisible())    
		_label->setVisible(true);
} 

void treeRingBubble::saveAsXmlNode( QDomDocument &doc, QDomElement &node )
{
	QDomText text;
	QString str, tmpStr;

	QDomElement type = doc.createElement( tr("Type") );
	text = doc.createTextNode( tr("TreeRing") );
	//if(toplevel)
	//   text = doc.createTextNode( tr("PathBubble1") );
	//else text = doc.createTextNode( tr("subPathBubble1") );
	type.appendChild( text );
	node.appendChild( type );

	ItemBase::saveAsXmlNode( doc, node );

	QDomElement pathPre = doc.createElement( tr("PathPre") );
	text = doc.createTextNode( this->_curFilePath );
	pathPre.appendChild( text );
	node.appendChild( pathPre );

	QDomElement curFile = doc.createElement( tr("FileName") );	
	text = doc.createTextNode( this->_curFileName );
	curFile.appendChild( text );
	node.appendChild( curFile );
		
	//sub treeRing
	for (int i = 0; i <_connections.size(); i ++)
	{
		struct treeRingConnector node=_connections[i];
		int layer,idx; //start
		QPointF start1, end1, start2, end2, tmpmid;
		ItemBase *bubble=node.childBubble;
		
		layer=node.layer; idx=node.idx;
		if(layer<0)
			continue;
		
		QRectF sRect = realRect();
		sRect.moveCenter(sceneBoundingRect().center());
		QPolygonF iRect = sRect;
		QPolygon fRect=iRect.toPolygon();
		if(bubble->getType()==TREERING)
		{
			
		}	    
	}	

	//sub pathway
	for (int i = 0; i <_connections.size(); i ++)
	{
		struct treeRingConnector node=_connections[i];
		int layer,idx; //start
		QPointF start1, end1, start2, end2, tmpmid;
		ItemBase *bubble=node.childBubble;
				
		layer=node.layer; idx=node.idx;
		if(layer<0)
			continue;
		
		QRectF sRect = realRect();
		sRect.moveCenter(sceneBoundingRect().center());
		QPolygonF iRect = sRect;
		QPolygon fRect=iRect.toPolygon();
		if(bubble->getType()==SUBPATHBUBBLE1 || bubble->getType()== PATHBUBBLE1)
		{
			
		}
	}
	
//for SubtreeRing
	if(!toplevel)
	{
		QDomElement initItems = doc.createElement( tr("initItems") );		
		for (set<vector<int>>::iterator it = initItemSelected.begin(); it != initItemSelected.end(); it++)
		//for (int i = 0; i < initItemSelected.size(); i=i+2)
		{
			vector<int> node= *it;
			//QDomElement Edge = doc.createElement( tr("start") );
        	QDomElement descentPos = doc.createElement( tr("item") );
			str = "";
			int layer=node[0], id=node[1];		
			tmpStr.setNum( layer );
			str += tmpStr;
			str += ",";
			tmpStr.setNum( id );
			str += tmpStr;			
			str += ",";			
			str += "";
			text = doc.createTextNode( str );
			descentPos.appendChild( text );			
			initItems.appendChild( descentPos );
		}				
		node.appendChild( initItems );
	}

	if(_parentID>=0 && _scene->m_treeRingBubbles[_parentID])//kill parent
	{
		QDomElement parentBubble = doc.createElement( tr("ParentBubble") );
		QDomElement pareFile = doc.createElement( tr("FileName") );
		text = doc.createTextNode( _scene->m_treeRingBubbles[_parentID]->getCurrentFilename() );
		pareFile.appendChild( text );
		parentBubble.appendChild( pareFile );
		QDomElement parePos = doc.createElement( tr("Position") );
		str = "(";
		tmpStr.setNum( _scene->m_treeRingBubbles[_parentID]->pos().x() );
		str += tmpStr;
		str += ", ";
		tmpStr.setNum( _scene->m_treeRingBubbles[_parentID]->pos().y() );
		str += tmpStr;
		str += ")";
		text = doc.createTextNode( str );
		parePos.appendChild( text );
		parentBubble.appendChild( parePos );
		node.appendChild( parentBubble );
	}

	/*
	//subgraph	
	QDomElement subGraphBubbles = doc.createElement( tr("subGraphBubbles") );
	int count=1;
	for (set<vector<vector<int>>>::iterator it = _scene->edgeconnections.begin(); it != _scene->edgeconnections.end(); it++)
	{
        vector<vector<int>> link = *it;	
        vector<int> link0=*link.begin(),link1=*link.rbegin();
		int pid1=link0[0], pid2=link1[0], type1=link0[1], type2=link1[1], id1=link0[2], id2=link1[2];
        QRectF itemPos1,itemPos2; 
		PathBubble1* path1=_scene->_pathBubbles[pid1], * path2=_scene->_pathBubbles[pid2];
	
		QDomElement descentPos = doc.createElement( tr("connection") );
		str = "";
		tmpStr.setNum( pid1);
		str += tmpStr;
		str += ",";
		tmpStr.setNum( type1);
		str += tmpStr;
		str += ",";
		tmpStr.setNum( id1);
		str += tmpStr;
		str += ",";
		tmpStr.setNum( pid2);
		str += tmpStr;
		str += ",";
		tmpStr.setNum( type2);
		str += tmpStr;
		str += ",";
		tmpStr.setNum( id2);
		str += tmpStr;
		str += ",";
		str += "";
		text = doc.createTextNode( str );
		descentPos.appendChild( text );		
		subGraphBubbles.appendChild( descentPos );
    }
	node.appendChild( subGraphBubbles );
	*/

	/*QDomElement descentBubbles = doc.createElement( tr("DescendantBubble") );
	for (int i = 0; i < _codeBubbles.size(); i++)
	{
		QDomElement descent = doc.createElement( tr("Descendant") );
		
		QDomAttr descentID = doc.createAttribute( tr("descID") );
		str.setNum( i+1 );
		descentID.setValue( str );
		descent.setAttributeNode( descentID );

		QDomElement descentFile = doc.createElement( tr("FileName") );
		text = doc.createTextNode( _codeBubbles[i]->getCurrentFilename() );
		descentFile.appendChild( text );
		descent.appendChild( descentFile );

		QDomElement descentPos = doc.createElement( tr("Position") );
		str = "(";
		tmpStr.setNum( _codeBubbles[i]->pos().x() );
		str += tmpStr;
		str += ", ";
		tmpStr.setNum( _codeBubbles[i]->pos().y() );
		str += tmpStr;
		str += ")";
		text = doc.createTextNode( str );
		descentPos.appendChild( text );
		descent.appendChild( descentPos );

		descentBubbles.appendChild( descent );
	}
	*/
	
	/*for (int i = 0; i < _reactionBubbles.size(); i ++)
	{
		QDomElement descent = doc.createElement( tr("Descendant") );
		
		QDomAttr descentID = doc.createAttribute( tr("descID") );
		str.setNum( i+1 );
		descentID.setValue( str );
		descent.setAttributeNode( descentID );

		QDomElement descentFile = doc.createElement( tr("FileName") );
		text = doc.createTextNode( _reactionBubbles[i]->getCurrentFilename() );
		descentFile.appendChild( text );
		descent.appendChild( descentFile );

		QDomElement descentPos = doc.createElement( tr("Position") );
		str = "(";
		tmpStr.setNum( _reactionBubbles[i]->pos().x() );
		str += tmpStr;
		str += ", ";
		tmpStr.setNum( _reactionBubbles[i]->pos().y() );
		str += tmpStr;
		str += ")";
		text = doc.createTextNode( str );
		descentPos.appendChild( text );
		descent.appendChild( descentPos );

		descentBubbles.appendChild( descent );
	}
	*/
	
	/*for (int i = 0; i < _groupBubbles.size(); i ++)
	{
		QDomElement descent = doc.createElement( tr("Descendant") );
		
		QDomAttr descentID = doc.createAttribute( tr("descID") );
		str.setNum( i+1 );
		descentID.setValue( str );
		descent.setAttributeNode( descentID );

		QDomElement descentFile = doc.createElement( tr("FileName") );
		text = doc.createTextNode( _groupBubbles[i]->getCurrentFilename() );
		descentFile.appendChild( text );
		descent.appendChild( descentFile );

		QDomElement descentPos = doc.createElement( tr("Position") );
		str = "(";
		tmpStr.setNum( _groupBubbles[i]->pos().x() );
		str += tmpStr;
		str += ", ";
		tmpStr.setNum( _groupBubbles[i]->pos().y() );
		str += tmpStr;
		str += ")";
		text = doc.createTextNode( str );
		descentPos.appendChild( text );
		descent.appendChild( descentPos );

		descentBubbles.appendChild( descent );
	}	
	node.appendChild( descentBubbles );
	*/
	
	/*
	for (int i = 0; i < _pathBubbles.size(); i ++)
	{
		QDomElement descent = doc.createElement( tr("Descendant") );
		
		QDomAttr descentID = doc.createAttribute( tr("descID") );
		str.setNum( i+1 );
		descentID.setValue( str );
		descent.setAttributeNode( descentID );

		QDomElement descentFile = doc.createElement( tr("FileName") );
		text = doc.createTextNode( _pathBubbles[i]->getCurrentFilename() );
		descentFile.appendChild( text );
		descent.appendChild( descentFile );

		QDomElement descentPos = doc.createElement( tr("Position") );
		str = "(";
		tmpStr.setNum( _pathBubbles[i]->pos().x() );
		str += tmpStr;
		str += ", ";
		tmpStr.setNum( _pathBubbles[i]->pos().y() );
		str += tmpStr;
		str += ")";
		text = doc.createTextNode( str );
		descentPos.appendChild( text );
		descent.appendChild( descentPos );

		descentBubbles.appendChild( descent );
	}	
	node.appendChild( descentBubbles );
	*/
	
	/*QDomElement connectPos = doc.createElement( tr("TextBubblePosition") );
	for ( int i = 0; i < _conPoint.size(); i ++ )
	{
		QDomElement position = doc.createElement( tr("Position") );

		QDomAttr posID = doc.createAttribute( tr("posID") );
		str.setNum( i+1 );
		posID.setValue( str );
		position.setAttributeNode( posID );

		QDomElement conpoint = doc.createElement( tr("ConPoint") );
		str =  "(";
		tmpStr.setNum( _conPoint[i].x() );
		str += tmpStr;
		str += ", ";
		tmpStr.setNum( _conPoint[i].y() );
		str += tmpStr;
		str += ")";
		text = doc.createTextNode( str );
		conpoint.appendChild( text );
		position.appendChild( conpoint );

		connectPos.appendChild( position );
	}
	node.appendChild( connectPos );
	
	connectPos = doc.createElement( tr("ReactionBubblePosition") );
	for ( int i = 0; i < _reactionPoint.size(); i ++ )
	{
		QDomElement position = doc.createElement( tr("Position") );

		QDomAttr posID = doc.createAttribute( tr("posID") );
		str.setNum( i+1 );
		posID.setValue( str );
		position.setAttributeNode( posID );

		QDomElement conpoint = doc.createElement( tr("ConPoint") );
		str =  "(";
		tmpStr.setNum( _reactionPoint[i].x() );
		str += tmpStr;
		str += ", ";
		tmpStr.setNum( _reactionPoint[i].y() );
		str += tmpStr;
		str += ")";
		text = doc.createTextNode( str );
		conpoint.appendChild( text );
		position.appendChild( conpoint );

		connectPos.appendChild( position );
	}
	node.appendChild( connectPos );

	connectPos = doc.createElement( tr("GroupBubblePosition") );
	for ( int i = 0; i < _groupPoint.size(); i ++ )
	{
		QDomElement position = doc.createElement( tr("Position") );

		QDomAttr posID = doc.createAttribute( tr("posID") );
		str.setNum( i+1 );
		posID.setValue( str );
		position.setAttributeNode( posID );

		QDomElement conpoint = doc.createElement( tr("ConPoint") );
		str =  "(";
		tmpStr.setNum( _groupPoint[i].x() );
		str += tmpStr;
		str += ", ";
		tmpStr.setNum( _groupPoint[i].y() );
		str += tmpStr;
		str += ")";
		text = doc.createTextNode( str );
		conpoint.appendChild( text );
		position.appendChild( conpoint );

		connectPos.appendChild( position );
	}
	node.appendChild( connectPos );
	*/
}


void treeRingBubble::setCurrentFile(const QString &fileName)
{
	_curFileName = fileName;
	_curFilePath = _curFileName;
	int tindex = _curFilePath.lastIndexOf("\\");
	_curFilePath.remove(tindex+1, _curFilePath.length());
}

void treeRingBubble::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{	
	//ptest.push_back(QPoint(j, fills[i].y1()));  //keqin	

	int size1 = _lines.size();
	int size2 = fills.size();
	updateLabel();

	QPointF start,middle,end;
	vector<int> temp(2,0);
	PlotBase::paint( painter, option, widget );

	float width,height;

	set<vector<int> > reactionRecord; 
	vector<int> node(2,0);
	vector<vector<int>> ed(2, node);

	QRegion reg=getClipRegionforCalloutNote();
	painter->setClipRegion(reg);
	painter->setRenderHint(QPainter::Antialiasing, true);
	
	Render(painter);
	if(_penisOn)
	{
		drawCurve(painter);
	}
	else if(groupSelected)
	{
		drawfills(painter);	
	}
    painter->setClipping(false);

	if(!itemDragged.empty() && draggingItem==1 )
	{
		paintItemDragged(painter, itemDragged);
	}
    float w,h;
	w=realRect().width(),  h=realRect().height();
	
	QColor c=QColor(_colorBoarder.a, _colorBoarder.b, _colorBoarder.c, _colorBoarderAlpha);	
	painter->setPen( c );
	painter->setBrush( Qt::NoBrush );
	painter->drawRect( QRect(realRect().x()+1, realRect().y()+1, realRect().width()-2, realRect().height()-2));

	/*painter->setPen(QColor(0,0,0,255));	
    for(int i=0; i<ptest.size(); i++) 
	{
	    painter->drawPoint(ptest[i].x(), ptest[i].y());
	}*/
	
}

void treeRingBubble::paintItemDragged(QPainter *painter, vector<QPolygon> itemDragged)
{   
	if(itemDragged.empty())
		return;
	int width = this->realRect().width();
    int height = this->realRect().height();
	painter->setPen(Qt::NoPen);	

	painter->setBrush(LIGHT_HIGHLIGHTCOLOR);
	painter->drawRect(bubbleRect);

	QRectF rect;
	//painter->setBrush(Qt::white);
	
	if(bubbleRect.width()!=0)
		return;

	for(int i=0;i<itemDragged.size();i++)
	{		
		painter->drawPolygon(itemDragged[i]);
	}		    
}

void treeRingBubble::drawConnections(QPainter *painter, QPointF dis)
{
	QPen pen;
	myColor3 color;
	
	vector<int> node;  
	vector<vector<int>> nodes;

	pen.setWidth(2);
	painter->save();
	
	for (int i = 0; i <_connections.size(); i ++)
	{
		struct treeRingConnector node=_connections[i];
		int layer,idx; //start
		QPointF start1, end1, start2, end2, tmpmid;
		ItemBase *bubble=node.childBubble;
				
		layer=node.layer; idx=node.idx;
		if(layer<0 || idx<0)
			continue;
		
		QRectF sRect = realRect();
		sRect.moveCenter(sceneBoundingRect().center());
		QPolygonF iRect = sRect;
		QPolygon fRect=iRect.toPolygon();
		if(bubble->getType()==TREERING)
		{
			vector<QPolygon> segPoints;
			vector<int> cornerNode_2; //record the other corner point
			getStartSegsNodes(node.itemSelected, segPoints,  cornerNode_2, this->pos());
			
			for(int j=0; j<segPoints.size(); j++)
			{
				pen.setColor( MEDIUM_HIGHLIGHTCOLOR);
				pen.setWidth(3);
				painter->setPen( pen );

				int cSize = segPoints[j].size();

				painter->drawPolygon(segPoints[j].intersected(fRect));

				pen.setColor( BUBBLELINKCOLOR );
				painter->setPen( pen );

				float x, y, x1, y1, x2, y2;
				//if(cornerNode_2[1]>=segPoints[j].size())
				//	j=j;
				x1=segPoints[j][cornerNode_2[j*2]].x(); //(_treeRing->_ring[layer]._pos[idx][0].x + _treeRing->_ring[layer]._pos[idx][1].x + _treeRing->_ring[layer]._pos[idx][2].x + _treeRing->_ring[layer]._pos[idx][3].x)/4;
				y1=segPoints[j][cornerNode_2[j*2]].y(); //(_treeRing->_ring[layer]._pos[idx][0].y + _treeRing->_ring[layer]._pos[idx][1].y + _treeRing->_ring[layer]._pos[idx][2].y + _treeRing->_ring[layer]._pos[idx][3].y)/4;
		
				x2=segPoints[j][cornerNode_2[j*2+1]].x(); //(_treeRing->_ring[layer]._pos[idx][0].x + _treeRing->_ring[layer]._pos[idx][1].x + _treeRing->_ring[layer]._pos[idx][2].x + _treeRing->_ring[layer]._pos[idx][3].x)/4;
				y2=segPoints[j][cornerNode_2[j*2+1]].y(); //(_treeRing->_ring[layer]._pos[idx][0].y + _treeRing->_ring[layer]._pos[idx][1].y + _treeRing->_ring[layer]._pos[idx][2].y + _treeRing->_ring[layer]._pos[idx][3].y)/4;
		
				start1=QPointF(x1, y1);
				start2=QPointF(x2, y2);
				//start = start + this->pos();		
				getLinkPoint(start1, end1, dis, sceneBoundingRect(), bubble->sceneBoundingRect());
				getLinkPoint(start2, end2, dis, sceneBoundingRect(), bubble->sceneBoundingRect());
				QPointF D1=start1-end1, D2=start2-end2;
			
				if(D1.x()*D1.x() + D1.y()*D1.y() <= D2.x()*D2.x() + D2.y()*D2.y())
					drawABubbleConnection(painter, start1, this, end1, bubble, dis);
				else
					drawABubbleConnection(painter, start2, this, end2, bubble, dis);
				
			}
		}
	    else if(bubble->getType()==SUBPATHBUBBLE1 || bubble->getType()== PATHBUBBLE1)
		{
			int cornerID1, cornerID2; 
			//vector<int> item = *node.itemSelected.begin();
			QPolygon segPoint = _treeRing->getSegPoint(node.layer, node.layer, node.idx, node.idx, this->pos().x(), this->pos().y(), cornerID1, cornerID2);	
			
			
			pen.setColor( MEDIUM_HIGHLIGHTCOLOR);
			painter->setPen( pen );
			
			painter->drawPolygon(segPoint.intersected(fRect));

			pen.setColor( BUBBLELINKCOLOR );
			painter->setPen( pen );

	        float x, y, x1, y1, x2, y2;
			x1=segPoint[cornerID1].x();//(_treeRing->_ring[layer]._pos[idx][0].x + _treeRing->_ring[layer]._pos[idx][1].x + _treeRing->_ring[layer]._pos[idx][2].x + _treeRing->_ring[layer]._pos[idx][3].x)/4;
			y1=segPoint[cornerID1].y();//(_treeRing->_ring[layer]._pos[idx][0].y + _treeRing->_ring[layer]._pos[idx][1].y + _treeRing->_ring[layer]._pos[idx][2].y + _treeRing->_ring[layer]._pos[idx][3].y)/4;
		
			x2=segPoint[cornerID2].x();//(_treeRing->_ring[layer]._pos[idx][0].x + _treeRing->_ring[layer]._pos[idx][1].x + _treeRing->_ring[layer]._pos[idx][2].x + _treeRing->_ring[layer]._pos[idx][3].x)/4;
			y2=segPoint[cornerID2].y();//(_treeRing->_ring[layer]._pos[idx][0].y + _treeRing->_ring[layer]._pos[idx][1].y + _treeRing->_ring[layer]._pos[idx][2].y + _treeRing->_ring[layer]._pos[idx][3].y)/4;
				
			start1=QPointF(x1, y1);
			start2=QPointF(x2, y2);

			getLinkPoint(start1, end1, dis, sceneBoundingRect(), bubble->sceneBoundingRect());
			getLinkPoint(start2, end2, dis, sceneBoundingRect(), bubble->sceneBoundingRect());
			QPointF D1=start1-end1, D2=start2-end2;
			if(D1.x()*D1.x() + D1.y()*D1.y() <= D2.x()*D2.x() + D2.y()*D2.y())
				drawABubbleConnection(painter, start1, this, end1, bubble, dis);
			else
				drawABubbleConnection(painter, start2, this, end2, bubble, dis);
			
		}
	}	
	painter->restore();
}

/*
void treeRingBubble::paintSegs(painter, layer, i, (_ring[layer]._pos[i])[0], (_ring[layer]._pos[i])[1], (_ring[layer]._pos[i])[2], (_ring[layer]._pos[i])[3]);					
{

	void paintSegs(painter, layer, i, (_ring[layer]._pos[i])[0], (_ring[layer]._pos[i])[1], (_ring[layer]._pos[i])[2], (_ring[layer]._pos[i])[3]);					

}
*/

void treeRingBubble::getStartSegsNodes(set<vector<int>> itemSelected, vector<QPolygon> &segPoints, vector<int> &cornerNode_2, QPointF Pos)
{	
	    int layer, id;
		vector<int> item,item2, item3;
		vector<int> segEnds;
		vector<vector<int>> ends; //record start and end node of each layer
		bool contituous = true;
	    for(set<vector<int>>::iterator it=itemSelected.begin(); it!=itemSelected.end(); it++)
		{
			item=*it;
			layer=item[0], id=item[1];
			if(layer>=ends.size())
			{
				ends.resize(layer+1);
				ends[layer].resize(2);			
				ends[layer][0] = id;
				ends[layer][1] = id;
			}	
			if( id-ends[layer][1] != 1)
			{
				contituous = false;
			}
			ends[layer][1] = id;
		}
		

		layer=0; 
		item.resize(2); item2.resize(2); item3.resize(2);
		item[0]=layer;
		//segEnds.push_back(ends[0][0]);	
		int maxj=_treeRing->_ring[0]._node_num;
		if(ends[0][0]== 0 && ends[0][1] == maxj-1 && contituous)
		{
			segEnds.push_back(ends[0][0]);		
			segEnds.push_back(ends[0][1]);	
		}
		else
		{
			for(int j=ends[0][0]; j<=ends[0][1]; j++)
			{
				if(ends[0][0] == ends[0][1])
					j=j;

				 item[1]=j; item2[1]=j-1; item3[1]=j+1;
				 if(item2[1]<0)
					 item2[1]=maxj-1;
				 if(item3[1]>=maxj)
					 item3[1]=0;
				 if( itemSelected.find(item)!=itemSelected.end() )
				 {
					 bool flag1=false, flag2=false;
					 if( itemSelected.find(item3)==itemSelected.end() )
					 {					 
						  flag1=true;
					 }
					 if( itemSelected.find(item2)==itemSelected.end() )
					 {
						  flag2=true;
					 }
					 if(flag1 && flag2)
					 {
						 segEnds.push_back(j);
						 segEnds.push_back(j);				 
					 }
					 else if(flag1)
					 {
						  if(segEnds.empty())
						  {
							 segEnds.push_back(-1);				 
							 segEnds.push_back(j);				 
						  }
						  else
							  segEnds.push_back(j);	
				 
					 }
					 else if(flag2)
					 {
						 if(j==ends[0][1] && segEnds[0]==-1)
						 {
							 segEnds[0]=j;			 
						 }
						 else
							 segEnds.push_back(j);		
					 }
					 if(j==ends[0][1] && !flag1 && !flag2 )
					 {
						 if(segEnds.size()==0)
						 {
							segEnds.push_back(j);
							segEnds.push_back(j);
						 }
						 else if(segEnds[0]==-1)
						 {
							 segEnds[0]=segEnds[segEnds.size()-1];	
							 segEnds.resize(segEnds.size()-1);
						 }
					 }				 
				 }		
			}
		}
		
		for(int j=0; j< segEnds.size(); j=j+2)
		{
			int id1=segEnds[j], id2=segEnds[j+1];		
			//segAngles.push_back(_treeRing->_ring[layer]._limits[id1].first);
			//segAngles.push_back(_treeRing->_ring[layer]._limits[id2].second);
			int cornerID1, cornerID2;
			segPoints.push_back(_treeRing->getSegPoint(0, _num_layers-1, id1, id2, Pos.x(), Pos.y(), cornerID1, cornerID2));				
			cornerNode_2.push_back(cornerID1);
			cornerNode_2.push_back(cornerID2);
	
		}

}



void treeRingBubble::Render(QPainter *painter)
{
	QString sTemp = _treeRing->getOrthFileName(); 
	
	//painter->setBrush(QColor(255,255,255,255));
	painter->setBrush(QColor(_colorInner.a,this->_colorInner.b,this->_colorInner.c,228));
	painter->drawRect( QRect(realRect().x()+1, realRect().y()+1, realRect().width()-2, realRect().height()-2));

	_treeRing->Render(painter);
		
	
	
	//renderSmallTreeRing(painter);
	//renderDiagram(painter); 
	if(_treeRing->shrinked)
	{
		_treeRing->shrinked=_treeRing->shrinked;
	}

	if(_hl<0)
	{		
		_bun[0]->Render(painter);		
	}
	else
	{
	    _bun[_hl]->Render(painter);
	}

	
		
	_Label->RenderTreeRingNodeLabels(painter, _treeRing);

	painter->setClipping(false);
	if(_hl>=0 && _hl<_num_layers)
 	{		
		vector <int> nodeIdx;	
		bool flag=false;
		for(int li=0; li<_treeRing->respondLayer.size(); li++)
		{
			if(_treeRing->respondIndex[li] == _hn)
			{
				if(!flag)
				   nodeIdx.push_back(_hn); 				
				flag=true;
			}
			else
				nodeIdx.push_back(_treeRing->respondIndex[li]); 	

			if(!flag && _hn>_treeRing->respondIndex[li]) //insert _hl, _hn at right place
			{
			    nodeIdx.push_back(_hn);
				flag=true;
			}			
		}
		if(!flag)
			nodeIdx.push_back(_hn); 		
		
		_Label->RenderHighLighted(painter, _treeRing, _hl, nodeIdx, MEDIUM_HIGHLIGHTcolor); 	

 	} // end if(_hl)	
	
	painter->setClipping(true);

	_lMarker->Render(painter);

	renderColorLegend(painter);
	
	if(_hl>=0 && _hl<_num_layers && !_treeRing->shrinked && (_treeRing->GetHighlightState() == HL_NODE || _treeRing->GetHighlightState() == HL_HANDLE) )	
	{
		float angle = _treeRing->_ring[_hl]._angle[_hn]; 	
		drawStatisticsText(painter,  _hx, _hy,  angle, _hl, _hn);
	}
}


void treeRingBubble::drawStatisticsText(QPainter *painter, float x, float y, float angle, int layer, int idx)
{	
	float dy=5;
	//float x,y;
	//x=_treeRing->_ring[layer]._centre_pos[idx].x;   y=_treeRing->_ring[layer]._centre_pos[idx].y;

	if(angle>=0 && angle<=3.1415926)
		y=y-dy;
	else 
		y=y+dy;	
					
	QFont f("Arial",10);					
	QFontMetrics fontMetrics(f);
	QRect fontRect, fontRect1, fontRect2;
	painter->setPen(Qt::NoPen);	

	QString eInfo, cInfo="Number of cross-talking proteins: " + QString::number(_treeRing->_ring[layer]._node_crossTalk[idx]);
	fontRect1 = fontMetrics.boundingRect(cInfo); 	

	if(!_expressionName.isEmpty())
	{
		eInfo="Expression level: " + QString::number(_treeRing->_ring[layer]._node_expression[idx]);
		fontRect2 = fontMetrics.boundingRect(eInfo); 		
	}
	float h=fontRect1.height()+fontRect2.height();
	float w=fontRect1.width()>fontRect2.width() ? fontRect1.width() : fontRect2.width();
	fontRect = QRect(x, y+3-fontRect1.height(), w, h);
	
	float dx=0;
	angle = angle/3.1415926 * 180;	
	if(angle> 135 && angle <= 180)
		//angle = 60;
		dx = -fontRect1.width()*0.9;
	else if(angle> 180 && angle < 225)
		dx = -fontRect1.width()*0.9;
		//angle = 60;
	/*else if(angle< 45)
		angle = 60;
	else if (angle > 315)
		angle = 60;
	else
		angle=0;*/

	painter->setPen(Qt::NoPen);
	painter->setBrush(QColor(255,255,255,187));
	drawAngleRect(painter, QRectF(fontRect.x()+dx, fontRect.y(), fontRect.width(), fontRect.height()), 0);

	painter->setPen(MEDIUM_HIGHLIGHTCOLOR);
	drawAngleText(painter, x+dx, y, cInfo, 0);
	drawAngleText(painter, x+dx, y+fontRect1.height(), eInfo, 0);

}

void treeRingBubble::renderColorLegend(QPainter *painter)
{
	float width= this->Width()/2-60;//_treeRing->GetRingWidth() * _treeRing->_ring.size() * 2.1; //4.5; //2.1
	float height= this->Height()/2-30;
	//float cwidth=this->Width()/2;
	QFont f1("Arial", 10);
	painter->setFont (f1);	
	int sx,sy;		
	if(_treeRing->GetColorBy() == EDGE_BY_EXPRESSION)
	{
		float min=_treeRing->eMin, max=_treeRing->eMax;//_treeRing->expressionMin, max=_treeRing->expressionMax;
		float mitv=_treeRing->eItv;
		float mark=min;
		int num=_treeRing->eNum, dl=_treeRing->eDL;
		//vector<QString> markers = TDHelper::getLegendParameter(min, max, 'F', 10, num, dl, mitv);
		vector<QString> markers = _treeRing->eMarkers;
		
		float length= num*mitv;
		float length2= _treeRing->eMax-_treeRing->eMin;
		//float scale = length2==0? 0: length/length2;
		//mitv=(max-min)/num;		
		int itv=180/num;
		sx=width-30-70, sy=height;
		if(num<=10)		
			itv=18;
		//painter->drawText(sx-10, sy-itv*(num-0.55), );

		QString eName=_expressionName;
		if(eName.size()>0)
		{
			QString tName;
			tName = "Data:";					
			tName = tName + eName;	
			QFontMetrics fontMetrics(f1); 
			QRectF rect3 = fontMetrics.boundingRect(tName); 
			QRectF rect4 = fontMetrics.boundingRect("Expression Level"); 			
			//int mad = rect4.width()> rect3.width()
			if(rect3.width()>rect4.width())
			   sx = width - 64 - rect3.width();// - MAX(rect4.width(),rect3.width());
			else
			   sx = width - 64 - rect4.width();// - MAX(rect4.width(),rect3.width());
			
		}	

		painter->setPen(Qt::NoPen);
		painter->setBrush(Qt::white);
		painter->drawRect(QRect(sx-15, sy-itv*(num + 1.8), this->Width()/2-sx+13, 27+itv*(num + 1.8)));		

		painter->setPen(QColor(50,50,50,255));

		QString str="Expression Level";		
		if(eName.size()>0)
		{
			painter->drawText(sx-10, sy-itv*(num + 0.2 - 0.15), str);
			str = "Data:";
			str = str + eName;
		    painter->drawText(sx-10, sy-itv*(num + 0.2 + 0.75), str);
		}		
		else 
			painter->drawText(sx-10, sy-itv*(num + 0.2-0.25), str);
		
		if(markers.size()>0)
		   painter->drawText(sx+20, sy-itv*(-1-0.1), markers[0]);//QString::number(mark,'g',dl));
		painter->setPen(Qt::NoPen);
		float gitv = itv / 5.0;
		int grad = 5;
		
		for(int i=0; i<num; ++i)
		{
			for(int j=0; j< grad; j++)
			{
				QRect rect(sx, sy-itv*i+(grad-1-j)*gitv, 16, gitv+1);
				float r, g, b;
				float value=float(int((i+1.0/grad*j)*grad))/(num*grad)*0.9;
				if(length2==0)
					value=0;
				if(value>1)
					value=value;
				TDHelper::GetMultHue(value, 8, r, g, b); 
				painter->setBrush(QColor(r,g,b,255));
				painter->drawRect(rect);
			}
		}		
		painter->setBrush(Qt::NoBrush);
		painter->setPen(QColor(50,50,50,255));
		for(int i=0; i<num; ++i)
		{
			//mark=min+(i+1)*mitv;
			QRect rect(sx, sy-itv*i, 16, itv);

			QString str= markers[i+1];//QString::number(mark, 'g', dl);		
			if(length2!=0)
			   painter->drawText(sx+24, sy-itv*(i-0.1), str);

			painter->drawLine(rect.x()+rect.width(),rect.y(), rect.x()+rect.width()+5, rect.y());	
		}	
		QRect rect = QRect(sx, sy-itv*(num-1), 16, itv*num);
		painter->drawRect(rect);
		
	}
	//else if(_treeRing->GetColorBy() == EDGE_BY_V)
	{					
		float min=_treeRing->cMin, max=_treeRing->cMax;//_treeRing->expressionMin, max=_treeRing->expressionMax;
		float mitv=_treeRing->cItv;
		float mark=min;
		int num=_treeRing->cNum, dl=_treeRing->cDL;
		//vector<QString> markers = TDHelper::getLegendParameter(min, max, 'I', 10, num, dl, mitv);
		vector<QString> markers = _treeRing->cMarkers;
					
		float length= num*mitv;
		float length1=_treeRing->cMax-_treeRing->cMin;
		float scale = length1==0?0:length/length1;

		int itv=18;
		sx=width-54, sy=height;
							
        int size = length1==0? 1:num;
		int grad= length1/size;
		float gitv;
		//size = length1==0? 1:num;
		int size1=size;
		int size2=size;

		if(length1==0)
		{
			num=1;
		    grad=1;			
		}
		else if(length1==1)
		{
		   grad=1;
		   
		   size2 = num+1;	
		   size1=size+1;
		}
		else if(grad==1)
		{
			size1=size+1;			
			//size = size +1;					    
			//size = length1==0? 1:num+1;
		}
		
		itv=180/size1;
		if(grad<=1)
			itv=18;
		gitv=float(itv)/grad;
		sy = sy-itv; 

		painter->setPen(Qt::NoPen);
		painter->setBrush(Qt::white);
		

		

		if(grad==1)
		{
			painter->drawRect(QRect(sx-15, sy-itv*(size1 + 0.6), this->Width()/2-sx+13, 27+itv*(size1 + 0.6)+16));			
			painter->setPen(QColor(50,50,50,255));
			painter->drawText(sx-10, sy-itv*(size)-10, "Number of");		
		    painter->drawText(sx-10, sy-itv*(size)+3, "Cross-talking Proteins");				
		}
		else
        {	
			painter->drawRect(QRect(sx-15, sy-itv*(size1 - 0.8)-20, this->Width()/2-sx+13, 27+itv*(size1 - 0.8)+56));
			painter->setPen(QColor(50,50,50,255));
			painter->drawText(sx-10, sy-itv*(size)+27, "Number of");		
		    painter->drawText(sx-10, sy-itv*(size)+40, "Cross-talking Proteins");				
		}

		sy = sy+0.5*itv; 
		painter->setPen(Qt::NoPen);		
		if(grad<=1)
		{
			for(int i=0; i< size1; ++i)
			{
				for(int j=0; j< grad; j++)
				{
					int gy = sy-itv*i-(j-grad+1)*gitv;
					QRect rect(sx, gy, 16, gitv+1);
					float r, g, b;
					float value=float(i+(float)j/grad)/num*0.86+0.1;
					if(value>1)
						value=1;
					TDHelper::GetMultHue(value, 7, r, g, b); 
					painter->setBrush(QColor(r,g,b,255));
					painter->drawRect(rect);	
				
				}
			}
			painter->setBrush(Qt::NoBrush);
			painter->setPen(QColor(50,50,50,255));
			for(int i=0; i<size+1; ++i)
			{
				QRect rect(sx, sy-itv*(i-1), 16, itv);
				
				painter->drawLine(rect.x()+rect.width(),rect.y(), rect.x()+rect.width()+5, rect.y());	
				if( markers.size()>i && ( i!=size || (size==1 && i!=1 ) || ( size == i && markers[markers.size()-1]!=markers[markers.size()-2])) )
				{
					QString str=  markers[i]; 
					painter->drawText(sx+24, sy-itv*(i-0.7), str);
				}
			}
			if(size>1)
			{
				QRect rect(sx, sy-itv*size, 16, itv);				
				painter->drawLine(rect.x()+rect.width(),rect.y(), rect.x()+rect.width()+5, rect.y());	
			}

			QRect rect = QRect(sx, sy-itv*(size1-1.0), 16, itv*(size1));
			painter->drawRect(rect);
		}	
	
		else
		{
			for(int i=0; i< size1; ++i)
			{
				for(int j=0; j< ((i==size1-1)?grad+1:grad); j++)
				{
					int gy = sy-itv*i-(j-grad+1)*gitv;
					QRect rect(sx, gy+1*gitv, 16, gitv+1);
					float r, g, b;
					float value=float(i+(float)j/grad)/num*0.86+0.1;
					if(value>1)
						value=1;
					TDHelper::GetMultHue(value, 7, r, g, b); 
					painter->setBrush(QColor(r,g,b,255));
					painter->drawRect(rect);					
				}
			}
			painter->setBrush(Qt::NoBrush);
			painter->setPen(QColor(50,50,50,255));
			for(int i=0; i<size+1; ++i)
			{
				QRect rect(sx, sy-itv*(i-1)-0.5, 16, itv);
				float r, g, b;				
				painter->drawLine(rect.x()+rect.width(),rect.y()+1*gitv, rect.x()+rect.width()+5, rect.y()+1*gitv);	
				if( markers.size()>i && ( i!=size || (size==1 && i!=1 ) || ( size == i && markers[markers.size()-1]!=markers[markers.size()-2])) )
				{
					QString str=  markers[i]; 
					painter->drawText(sx+24, sy-itv*(i-1.0)+1*gitv, str);
				}
			}	
			QRect rect = QRect(sx, sy-itv*(size1-1.0), 16, itv*(size1)+gitv);
			painter->drawRect(rect);
		}			
	}	
}

void treeRingBubble::keyReleaseEvent(QKeyEvent *event)
{
	int a=1;
	a=0;
	//set< vector<int > > segs = whichSegment( _hl, _hn ); 
	if(event->key() == Qt::Key_Delete)
	{
		subStarctTreeRing();		
	}
}

void treeRingBubble::subStarctTreeRing()
{//remove a tree ring segment
	set< vector<int > > segs = whichSegment( _hl, _hn ); 
	if(segs.size()>=1)
	{
		QMessageBox msgBox;		
		msgBox.setWindowTitle("Delete Ring Segment");
		msgBox.setText("Delete this ring segment?");
		msgBox.setStandardButtons(QMessageBox::Yes);
		msgBox.addButton (QMessageBox::No);
		msgBox.move(this->scenePos().x(),this->scenePos().y());
		if(msgBox.exec() == QMessageBox::No)
			return;
	}
	else
	{
		QMessageBox msgBox;				
		msgBox.setWindowTitle("Select Ring Segment");
		msgBox.setText("Please select a ring segment");
		msgBox.move(this->scenePos().x(),this->scenePos().y());
		msgBox.setStandardButtons(QMessageBox::Yes);
		if(msgBox.exec() == QMessageBox::Yes)
			return ;
	    
	}
	//if(isSegPreSelected(_hl,_hn))
	{
		nodePicked.clear();
        subStarctTreeRing(segs);
		
	}
}

void treeRingBubble::subStarctTreeRing(set<vector<int>> itemSelected)
{
    /*  this->_ring_radius = ringRadius;
	this->_ring_width = ringWidth;*/
	this->_hl = -1;
	this->_hn = -1;
	//this->EDGE_BY = parent->EDGE_BY; //EDGE_BY_V; //EDGE_BY_ORTHOLOGY;	//keqin
	
	//this->_centre.x=x, this->_centre.y=y;
	set<vector<int>> tempSelected;
	for(int layer=0; layer<_treeRing->_ring.size(); layer++)
    {	
		for(int id=0; id<_treeRing->_ring[layer]._node_num; ++id)
		{
			vector<int> item(2,0);
			item[0]=layer; item[1]=id;
			if(itemSelected.find(item)==itemSelected.end())
			{
			   tempSelected.insert(item);		
			}
		}  		
    }
	itemSelected = tempSelected;

	// load the simulation results--------------------------
	vector<vector<double>> inputData;
	vector<vector<int>> inputNodeSize;
	vector<vector<string>> inputTree;
	vector<vector<QString>> inputPathID;
	vector<vector<int>> rateLimitNum;
	vector<vector<int>> outList, inList;// //_out_list/in_node_list//_rr
	vector<bool> markerFlag;
	//int edgeNum=parent->_bun->_rr;
	int layerNum=_treeRing->_ring.size();
	//inputData.resize(layerNum);
	inputTree.resize(layerNum);
	inputPathID.resize(layerNum);

	inputNodeSize.resize(layerNum);
	rateLimitNum.resize(layerNum);
	vector < SimDataElem > _data = _treeRing->GetData();
	int totalNodes0=0,totalNodes1=0;
	int nodeCount=0;
	vector<int> nodeIDs;
	set<int> nodeIDSet;
	for(set<vector<int>>::iterator it=itemSelected.begin(); it!=itemSelected.end(); it++)
	{
	    vector<int> item = *it;
		int layer=item[0], id=item[1];
		inputTree[layer].push_back(_treeRing->_ring[layer]._node_name[id]);	
		inputPathID[layer].push_back(_treeRing->_ring[layer]._node_pathID[id]);
		
		int nodesize = reCountNodeSize(_treeRing, layer, id, itemSelected, false);
		int testsize=_treeRing->_ring[layer]._node_size[id];
		
		inputNodeSize[layer].push_back(nodesize); //_treeRing->_ring[layer]._node_size[id]
		rateLimitNum[layer].push_back(_treeRing->_ring[layer]._rateLimitNum[id]);	

		if(layer==layerNum-2)
		    totalNodes1 = totalNodes1 + _treeRing->_ring[layer]._node_size[id];
		if(layer==layerNum-3)
		    totalNodes0 = totalNodes0 + _treeRing->_ring[layer]._node_size[id];
		if(layer==layerNum-1)
		{
			nodeCount++;
		}
        if(layer==layerNum-1)
		{
			vector<double> variables = _data[id]._elem;			
			inputData.push_back(variables);	
			nodeIDs.push_back(id);
			nodeIDSet.insert(id);
			//outList.push_back(_bun->GetOutList(id));
		    //inList.push_back(_bun->GetInList(id));
			markerFlag.push_back(_lMarker->getMarkerFlag(id));
		}
	}
	//edge list
	for(set<vector<int>>::iterator it=itemSelected.begin(); it!=itemSelected.end(); it++)
	{
	    vector<int> item = *it;
		int layer=item[0], id=item[1];
		if(layer==layerNum-1)
		{	
			vector<int> oID=_bun[layer]->GetOutList(id), iID=_bun[layer]->GetInList(id), oid, iid;
			if(!oID.empty())
			{
				for(int j =0 ; j< oID.size(); j++ )
				{
					if(nodeIDSet.find(oID[j])!=nodeIDSet.end())
					{
						for(int i=0;i<nodeIDs.size(); i++)
						{
							if(nodeIDs[i] == oID[j])
							{	
								oid.push_back(i);									
								break;
							}
						}	
					}
				}
			}
			
			if(!iID.empty())
			{
				for(int j =0 ; j< iID.size(); j++ )
				{
					if(nodeIDSet.find(iID[j])!=nodeIDSet.end())
					{
						for(int i=0;i<nodeIDs.size(); i++)
						{
							if(nodeIDs[i] == iID[j])
							{	
								iid.push_back(iID[j]);									
								break;
							}
						}	
					}
				}
			}
			outList.push_back(oid);			
		    inList.push_back(iid);			
		}
	}
	//get ride of link on included
	//updateEdgeList(_treeRing->_ring[layerNum-1]._node_name, inputTree[layerNum-1], outList);
	//updateEdgeList(_treeRing->_ring[layerNum-1]._node_name, inputTree[layerNum-1], inList);

	_simData = new SimData(inputData, 18, 0, 18); // keqin's data only has 2 values (2,0,2)
	//_simData = new SimData(dataName, 18, 0, 18); // TODO: OK fixed to 18.
	_simData->PrintIntervalDistribution();
	
	//TreeRing----------------------------------------------
	//_treeRing = new TreeRing(inputTree, ringRadius, ringWidth, _simData, EDGE_BY, _centre);

	//getNewNodeID(
	
	float _zoom_t = _treeRing->_ring[0]._ring_radius;
	

	TreeRing *_preTreeRing=_treeRing;
	_treeRing =  new TreeRing(inputTree, inputPathID, inputNodeSize, rateLimitNum, ringRadius, ringWidth, _simData, EDGE_BY, _centre, _treeRing->getOrthFileName());
	this->_node_num = _treeRing->GetNumOfNodes(); // nodes count
	_num_layers = _treeRing->GetNrLayers();
	
	
	updateConnectionAfterSubstraction(_preTreeRing);

	// bundle information ----------------------------------
	//int br = _treeRing->GetRingRadius(_treeRing->GetNrLayers()-1);
	int br = _treeRing->GetRingRadius(0); //changed when switching inner ring out
	
	_bun.resize(_num_layers);
	for(int i=0; i<_num_layers; i++)
	{
		_bun[i] = new BundlesPrimitive(_treeRing, outList, inList, _treeRing->_ring[i]._node_num, i, br, _simData, EDGE_BY, _centre);
		//_bun->_num_layers= _treeRing->GetNrLayers();
		//_bun = new BundlesPrimitive(_treeRing, edgeNum, outList, inList, graphName, _node_num, br, _simData, EDGE_BY, _centre);
		_bun[i]->GenerateCurveBundles(_treeRing, BUDDLINGSTRENGTH/*bundling strength*/);
	}
	//_bun[0]->CalModel();


	// makers ----------------------------------
	//delete [] _lMarker;
	_lMarker = new ExtraLayerMarker(_treeRing, markerFlag, _node_num, br, _simData, _centre);
	//_lMarker->GenerateMarkers(_treeRing);

	// labels ----------------------------------
	
	_Label = new Label(window_w, window_h, compoudgraph_centre);
	
	_treeRing->CalculateLabelDisplayOrder();
	_Label->BuildScreenGrid(_treeRing);
	_Label->_num_layers= _treeRing->GetNrLayers();
	//FINISH_INIT = true;
	itemSelected.clear();

	int edge_by=GetEdgeBy();
	if(edge_by==EDGE_BY_ORTHOLOGY)
	{				
		_treeRing->ComputeNodeColorByO(_treeRing->getOrthFileName());	
		_treeRing->SetColorBy(EDGE_BY_ORTHOLOGY);
		//b0->EDGE_BY=EDGE_BY_ORTHOLOGY;		
	}
	else if(edge_by==EDGE_BY_EXPRESSION)
	{
		searchSharedProtein();
		searchExpressed();	
	}
	_zoom_t = _zoom_t/_treeRing->_ring[0]._ring_radius-1;
	Zoom(_zoom_t);	

	//Zoom(_zoom);
}

void treeRingBubble::updateConnectionAfterSubstraction(TreeRing *_preTreeRing)
{ 
	//let child bubble of _preTreeRing to be child bubble of _curTreeRing
	vector<struct treeRingConnector> nconnections;
	for (int j = 0; j <_connections.size(); j++)
	{
		struct treeRingConnector node=_connections[j];		
		int nidx;
		set<vector<int>> nItemSelected;

		nidx = getNewNodeID(node.layer, node.idx, _preTreeRing, _treeRing); //node.layer, 
		node.idx = nidx;
		
		for(set<vector<int>>::iterator it=node.itemSelected.begin(); it!=node.itemSelected.end(); it++)
		{
			vector<int> item = *it;
			int layer= item[0], id=item[1];
			nidx = getNewNodeID(layer, id, _preTreeRing, _treeRing); 
			item[1]=nidx;
			if(nidx>-1)
			  nItemSelected.insert(item);
		}
		node.itemSelected = nItemSelected;

		ItemBase *bubble=node.childBubble;
		if(bubble!=NULL)
		{
			if( bubble->getType()==SUBPATHBUBBLE1 || bubble->getType()==PATHBUBBLE1)
			{	
				treeRingBubble* tbubble= dynamic_cast<treeRingBubble*>(bubble);
				if(tbubble!=this)
				{
					nconnections.push_back(node);					
				}				
			}
			else if(bubble->getType()==TREERING)
			{
				treeRingBubble* tbubble= dynamic_cast<treeRingBubble*>(bubble);
				if(tbubble!=this)
				{
					nconnections.push_back(node);					
				}						
			}
		}		
	}	
	_connections = nconnections;
}


void treeRingBubble::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	ItemBase::mousePressEvent(event);			
	_mousePressed=true;
	preItemSelected = _treeRing->_itemClicked;
	if( !onBoarder( event->pos() ) )
	{
		if (event->button() == Qt::RightButton) 
		{  
		   m_rightButton=true;
		   lastPressedPos = event->pos();
		}
		else m_rightButton=false;	
		
		if(!m_rightButton)
		{
			if(_penisOn)
			   initPos = event->pos();
			else if(groupSelected)
			{
				if(_contain.containsPoint(event->pos(), Qt::OddEvenFill))
				{
					setItemDragged(itemSelected);						
				}	
				else
				{
					draggingItem=0;
					_treeRing->_itemDragged.clear();
	                _treeRing->_itemClicked.clear();	
				}
			}			
			else 
			{
				
					handleSelected = whichHandle( event->pos() );
					_treeRing->handleSelected = handleSelected;	

					//if(handleSelected[0]<0)
					nodePicked = whichItem( event->pos() );
					_hl=nodePicked[0], _hn=nodePicked[1];
					if(_hl>=0)
					{
						multiItemSelected.clear();
						setItemDragged(nodePicked);
						draggingItem=0;

						if(_hl>=0 && _hn>=0)
						{
							if(_treeRing->respondLayer.size()==0)
							{
							   _treeRing->respondLayer.push_back(_hl);
							   _treeRing->respondIndex.push_back(_hn);
							}
							_treeRing->shrinkNodes(_treeRing->respondLayer, _treeRing->respondIndex);
						
							_bun[_hl]->reGenerateCurveBundles(_treeRing, BUDDLINGSTRENGTH);
							if(_hl==_num_layers-1)
							   _lMarker->updateMarkerAngle(_treeRing);
							_treeRing->shrinked = true;
						}
					}
					if(_hl<0)
					{
						_treeRing->_itemDragged.clear();
						_treeRing->_itemClicked.clear();	
					}
					draggingItem=0;
					if(_scene->controlKeyisOn)
					{
						vector < set< vector<int > > > tempSelected;
						set< vector<int > > segs;
						segs.insert(nodePicked);
						//itemSelected.clear();			
						int di=-1;
						for(int j = 0; j<multiItemSelected.size(); j++)
						{
							if(multiItemSelected[j]==segs)
							{
								di=j;						
							}
							else
							   tempSelected.push_back(multiItemSelected[j]);
						}	
						if(di<0)
						   tempSelected.push_back(segs);

						multiItemSelected=tempSelected;
						for(int j = 0; j<multiItemSelected.size(); j++)
						{
							for(set< vector<int > >::iterator it = multiItemSelected[j].begin(); it != multiItemSelected[j].end(); it++)
							{
									itemSelected.insert(*it);
							}	
						}									
						setItemDragged(itemSelected);					
					}
					else if(_scene->shiftKeyisOn)
					{
						vector<int> item(2,-1);
						itemSelected.clear();				
						bool flag=false;
						for(int li=0; li<_treeRing->respondLayer.size(); li++)
						{
							item[0] = _treeRing->respondLayer[li]; 
							item[1] = _treeRing->respondIndex[li];
							itemSelected.insert(item);
							if(_treeRing->respondIndex[li] == _hn)
							{   
								nodePicked.resize(2);				
								nodePicked[0]=_hl; nodePicked[1]=_hn;					
								flag=true;
							}
							if(!flag && _hn>_treeRing->respondIndex[li]) 
							{
								flag=true;
							}			
						}
						if(!flag)
						{
							item[0]=_hl, item[1] = _hn;
							itemSelected.insert(item);									
						}
						multiItemSelected.clear();
						setItemDragged(itemSelected);
					
					}
			    
			}
		}
		else
		{
			if(_scene->controlKeyisOn)
			{
				_penisOn=false;
				groupSelected=false;
				set< vector<int > > segs = whichSegment( event->pos() ); 

				if(!segs.empty())
				{
					itemSelected.clear();
					//_scene->rightClickFlag=true;
					vector < set< vector<int > > > tempSelected;
					//itemSelected.clear();			
					int di=-1;
					for(int j = 0; j<multiItemSelected.size(); j++)
					{
						if(_scene->controlKeyisOn && multiItemSelected[j]==segs)
						{
							di=j;						
						}
						else
						   tempSelected.push_back(multiItemSelected[j]);
					}	
					if(di<0)
					   tempSelected.push_back(segs);

					multiItemSelected=tempSelected;
					for(int j = 0; j<multiItemSelected.size(); j++)
					{
						for(set< vector<int > >::iterator it = multiItemSelected[j].begin(); it != multiItemSelected[j].end(); it++)
						{
							itemSelected.insert(*it);
						}	
					}									
					setItemDragged(itemSelected);
				}  
				else
				{
				    _treeRing->_itemDragged.clear();
	                _treeRing->_itemClicked.clear();	
				}
				draggingItem=0;	
				nodePicked.clear();
			}
			else 
			{
			    _penisOn=false;
				groupSelected=false;

				multiItemSelected.clear();
				_treeRing->_itemDragged.clear();
	            _treeRing->_itemClicked.clear();	
				nodePicked.clear();
				

				nodePicked = whichItem( event->pos() );
				_hl=nodePicked[0], _hn=nodePicked[1];
				nodePicked.clear();
				if(_hl>=0)
				{
					setItemDragged(nodePicked);
					draggingItem=0;

					if(_hl>=0 && _hn>=0)
					{
						if(_treeRing->respondLayer.size()==0)
						{
							_treeRing->respondLayer.push_back(_hl);
							_treeRing->respondIndex.push_back(_hn);
						}
						_treeRing->shrinkNodes(_treeRing->respondLayer, _treeRing->respondIndex);
						
						_bun[_hl]->reGenerateCurveBundles(_treeRing, BUDDLINGSTRENGTH);
						if(_hl==_num_layers-1)
							_lMarker->updateMarkerAngle(_treeRing);
						_treeRing->shrinked = true;
					}
				}
				if(_scene->shiftKeyisOn)
				{
					/////////////////
					vector<int> item(2,-1);
					bool flag=false;

					vector<int> respondLayer = _treeRing->respondLayer;
					vector<int> respondIndex = _treeRing->respondIndex;
					for(int li=0; li<respondLayer.size(); li++)
					{
						if(respondLayer[li]==_hl && respondIndex[li]==_hn)
						{
							flag = true;
						}
					}
					if(!flag && _hl>=0 && _hn>=0)
					{
					   respondLayer.push_back(_hl);
					   respondIndex.push_back(_hn);
					}

					for(int li=0; li<respondLayer.size(); li++)
					{
						item[0] = respondLayer[li]; 
						item[1] = respondIndex[li];
						//itemSelected.insert(item);
						/*if(_treeRing->respondIndex[li] == _hn)
						{   
							nodePicked.resize(2);				
							nodePicked[0]=_hl; nodePicked[1]=_hn;					
							flag=true;
						}*/
						///////////////////
						
						set< vector<int > > segs = whichSegment( item[0], item[1] ); 
						if(!segs.empty())
						{
							vector < set< vector<int > > > tempSelected;								
							int di=-1;
							for(int j = 0; j<multiItemSelected.size(); j++)
							{
								if(_scene->controlKeyisOn && multiItemSelected[j]==segs)
								{
									di=j;						
								}
								else
								   tempSelected.push_back(multiItemSelected[j]);
							}	
							if(di<0)
							   tempSelected.push_back(segs);

							multiItemSelected=tempSelected;
							for(int j = 0; j<multiItemSelected.size(); j++)
							{
								for(set< vector<int > >::iterator it = multiItemSelected[j].begin(); it != multiItemSelected[j].end(); it++)
								{
									 itemSelected.insert(*it);
								}	
							}									
							setItemDragged(itemSelected);
						}  
						else
						{
							_treeRing->_itemDragged.clear();
							_treeRing->_itemClicked.clear();	
						}
						
						///////////////////
							
					}
					
					
					/////////////////
					/*vector<int> item(2,-1);
					itemSelected.clear();				
					bool flag=false;
					for(int li=0; li<_treeRing->respondLayer.size(); li++)
					{
						item[0] = _treeRing->respondLayer[li]; 
						item[1] = _treeRing->respondIndex[li];
						itemSelected.insert(item);
						if(_treeRing->respondIndex[li] == _hn)
						{   
							nodePicked.resize(2);				
							nodePicked[0]=_hl; nodePicked[1]=_hn;					
							flag=true;
						}
						if(!flag && _hn>_treeRing->respondIndex[li]) 
						{
							flag=true;
						}			
					}
					if(!flag)
					{
						item[0]=_hl, item[1] = _hn;
						itemSelected.insert(item);									
					}
					multiItemSelected.clear();
					setItemDragged(itemSelected);	*/				
				}
				else
				{
					set< vector<int > > segs = whichSegment( event->pos() ); 				
					if(!segs.empty())
					{
						vector < set< vector<int > > > tempSelected;					
						tempSelected.push_back(segs);
						multiItemSelected=tempSelected;
						for(int j = 0; j<multiItemSelected.size(); j++)
						{
							for(set< vector<int > >::iterator it = multiItemSelected[j].begin(); it != multiItemSelected[j].end(); it++)
							{
								itemSelected.insert(*it);
							}	
						}									
						setItemDragged(itemSelected);
					} 
				}				
			}
		}
	} 
	else
	{
		draggingItem=0;
		//_treeRing->_itemDragged.clear();
	    //_treeRing->_itemClicked.clear();
	}

    stroke_tracking = GL_TRUE;
	if(onBoarder( event->pos() ))
		pressOnBoarder = true;
	else 
		pressOnBoarder = false;

	 if(onUnGroupArea(event->pos()) )
		pressOnUngroup = true;
	else 
		pressOnUngroup = false;
}

void treeRingBubble::setItemDragged(vector<int> nodePicked)
{
	bubbleRect.setWidth(0), bubbleRect.setHeight(0);
	if(nodePicked.size()<2)
		return;
	int layer=nodePicked[0], idx=nodePicked[1];
	itemDragged.clear();	
	_treeRing->_itemDragged.clear();
	_treeRing->_itemClicked.clear();	
	setAItemDragged(layer, idx);	
	_treeRing->_itemClicked.insert(nodePicked);
}

void treeRingBubble::setItemDragged(set<vector<int>> itemSelected)
{
	bubbleRect.setWidth(0), bubbleRect.setHeight(0);
    itemDragged.clear();
	_itemDragged.clear();
	_treeRing->_itemDragged.clear();
	for(set<vector<int>>::iterator it=itemSelected.begin(); it!=itemSelected.end(); it++)
	{
		vector<int> item=*it;
	    setAItemDragged(item[0], item[1]);	
	}			
	_treeRing->_itemClicked = itemSelected;
	_itemDragged=itemSelected;
}

set<vector<int>> treeRingBubble::getItemDragged()
{
	return _itemDragged;	
}

void treeRingBubble::setAItemDragged(int layer, int id)
{
	if(layer<0 || layer>=_treeRing->_ring.size() || id>=_treeRing->_ring[layer]._pos.size() || id<0)
		return;

	QPolygon node=_treeRing->getANode(layer, id); //, (_ring[layer]._pos[id])[0], (_ring[layer]._pos[id])[1], (_ring[layer]._pos[id])[2], (_ring[layer]._pos[id])[3]);
	itemDragged.push_back(node);		
}


void treeRingBubble::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	if (!m_rightButton) 
	{
		bubbleDraggedout=false;		
	    lastPressedPos = QPointF(0,-1000000);
	}

	ItemBase::mouseReleaseEvent( event );
	_mousePressed=false;
	//bubbleDraggedout=false;
	//_strokeStopMotion();
	//m_isMoving=false;
	m_isInnerMoving=false;	

	stroke_tracking = GL_FALSE;
	firstMotionLeft=true;
    firstMotionRight=true;
	
	if(handleSelected.size()>=3 && handleSelected[0]>=0)
		handleSelected=handleSelected;

	if(draggingItem && !m_isInnerMoving)
    {
		if(dragout!=2 && dragout!=0)
		{
			itemDragged.clear();						
			highlighted.clear();
			itemSelected.clear();
			multiItemSelected.clear();	
			_treeRing->_itemDragged.clear();
		}
	}
	if( draggingItem && (groupSelected || !itemSelected.empty()) && !_grabisOn && dragout==2 && !(nodePicked.size()>=2 && nodePicked[0]>-1) )
	{
		    if(!itemSelected.empty())
			{
				QPointF Dock=dockItemDragged(event->pos());		
				//new bubble
				QString qstr;
				string s1;
				std::ostringstream oss;			
			   
				QPointF iPos;
				iPos=event->scenePos();
				bubbleDraggedout=true;
				treeRingBubble *rb = dynamic_cast <treeRingBubble*> (_open->openSubTreeRing(this, 400, iPos.x(), iPos.y(), itemSelected));
				rb->_treeRingBubbleParent=this;
				if(rb)
				{
					for (int j = 0; j <this->_connections.size(); j++)
					{
						struct treeRingConnector node= _connections[j];
						ItemBase *bubble=node.childBubble;
						if(bubble!=NULL)
						{
							_treeRing->itemSelected.insert(node.itemSelected);									
						}
					} 
					_treeRing->itemSelected.insert(itemSelected);	//color 
					
					QPoint pos = this->_scene->getSonPositionforTreeRing(rb , this, rb->sceneBoundingRect().x(), rb->sceneBoundingRect().y(), rb->sceneBoundingRect().width(), rb->sceneBoundingRect().height());
					//itemGrid=recordItemGrid(visibleItemNum, visibleCompartmentNum);
					resetOriRect();
					_scene->addToGroup( this, rb, false);//keqin temp

					struct treeRingConnector node;
					//get center
					vector<int> anode=*itemSelected.begin();
					node.itemSelected=itemSelected;
					node.layer=anode[0];
					node.idx=anode[1];
					node.childBubble=rb;
					_connections.push_back(node);	

					//update parent tree ring
				    updateParentTreeRingAfterCollapse(this, itemSelected, window_w, window_h);	
				}
			}	
			_penisOn=groupSelected=_grabisOn=_queryisOn=_upisOn=_downisOn=_evenisOn=false;
			itemDragged.clear();
			_treeRing->_itemDragged.clear();
			itemSelected.clear();
			multiItemSelected.clear();
			_lines.clear();
			fills.clear();
			_contain.clear();	
	}	
	else if( draggingItem && !groupSelected && !_grabisOn && dragout==2 )
	{		 
		if(nodePicked.size()>=2 && nodePicked[0]>-1)
	    {
			itemSelected.insert(nodePicked);
			QPointF Dock=dockItemDragged(event->pos());		
			//new bubble
			QString qstr;
			string s1;
			std::ostringstream oss;			
			 			
			ItemBase* nitem = NULL;	
			QPointF iPos;
			iPos=event->scenePos();

			for (set<vector<int>>::iterator it = itemSelected.begin(); it != itemSelected.end(); it++ )
			{
				vector<int> item=*it;
                int layer=item[0], id=item[1];
				///------------------
				while(true)
				{
					///------------------
					QString lastStoredfileName;
					string name=_treeRing->GetNodeName(layer,id);				
					QString pathName=QString::fromStdString(name);
					QString num = _treeRing->GetPathID(layer,id);	//getPathID(pathName);
			
					//if(num == "382551") //transmembrane transport of small molecules
					//	 continue;
					//if(num == "1643685") //disease
					//	 break;
				
					/*if(num == "428359") //Binding of RNA by Insulin-like Growth Factor-2 mRNA Binding Proteins (IGF2BPs/IMPs/VICKZs)
						continue;
					if(num == "1430728") //Metabolism
						 continue;				
					if(num == "168256")//immune system
						 continue;
					//if(num == "162582") //signal tranduction
					//	continue;
					if(num == "163318") // an empty file
						 continue;*/
				
					/*if(num == "2404192") //checked
						break;	

					if(num == "425397") //checked
						break;	

					if(num == "2160456") //checked
						break;	
							
					if(num == "190236")//checked
						break;				

					if(num == "917937")//checked
						break;				

					if(num == "3296482") //checked
						break;

					if(num == "2160456") //checked
						break;

					if(num == "432047") //checked
						break;*/

					//if(num == "774815") //checked: Nucleosome assembly
					//	break;
				
					//if(num == "157579") //Telomere Maintenance: take long to see result when generating graph 
					//	break;				

					if(num == "") //Metabolism
						break;				
					//if(num == "1430728") //Metabolism
					//	break;				
					//if(num == "168256")//immune system
					//	break;
					//if(num == "162582") //signal tranduction
					//	continue;
					//if(num == "163318") // an empty file
					//	break;

					if(num !="")
					{	
						QString fname = "data/Reactome_Pathway_Data/pathwayTable/tableFile/" + num + "_";					
						bool opened = _open->openPathwayByPath(nitem, _treeRing->getOrthFileName(), 200, iPos.x(), iPos.y(), fname, pathName, lastStoredfileName);	
						if(opened && nitem)
						{
							QPoint pos = this->_scene->getSonPositionforTreeRing(nitem , this, nitem->sceneBoundingRect().x(), nitem->sceneBoundingRect().y(), nitem->sceneBoundingRect().width(), nitem->sceneBoundingRect().height());
							//itemGrid=recordItemGrid(visibleItemNum, visibleCompartmentNum);
							resetOriRect();
							_scene->addToGroup( this, nitem, false);//keqin temp
					
							struct treeRingConnector node;
							//get center
							//vector<int> anode=nodePicked;
							node.itemSelected=itemSelected;
							node.layer=layer;
							node.idx=id;
							node.childBubble=nitem;
							_connections.push_back(node);	
							nodePicked[0] = nodePicked[1] = -1;								
						}
					}
					break;//keqin
				}
				//update parent tree ring
				//updateParentTreeRingAfterCollapse(this, itemSelected, window_w, window_h);
				}
		}	
		itemDragged.clear();
		_treeRing->_itemDragged.clear();
		itemSelected.clear();
		multiItemSelected.clear();
		_lines.clear();
		fills.clear();
		_contain.clear();	
		_penisOn=groupSelected=_grabisOn=_queryisOn=_upisOn=_downisOn=_evenisOn=false;
	}
	else if(_penisOn)
	{
		if(_lines.size()!=0)
			_penisOn=false;
		groupSelected=groupSelection(_lines);			
	}
	else if(groupSelected)
	{
		groupSelected=0;	
		itemSelected.clear();
		multiItemSelected.clear();
	}			
	if(!groupSelected && !_scene->controlKeyisOn)
	{
		itemSelected.clear();
		multiItemSelected.clear();		
	}
	draggingItem=0;	
	nodePicked.clear();	
	_treeRing->clearHighLight();
	//nodePicked[0] = nodePicked[1] = -1;
	_scene->removeToolsinGroup(this, false);
	dragout=0;	
	
	pressOnBoarder = false;
	pressOnUngroup = false;
	//_scene->rightClickFlag=false;

	spanIsOn=false;
	if(_treeRing->shrinked)
	{
         _treeRing->shrinked = false;
		 _treeRing->unShrinkNodes();
	}
}

bool treeRingBubble::clearSearchSelection()
{
	_treeRing->clearHighLight();
	nodePicked.clear();	
	itemSelected.clear();
	multiItemSelected.clear();	
	searched.clear();
	_treeRing->searched.clear();
	_treeRing->_itemClicked.clear();

	return true;
}

bool treeRingBubble::isMissingFile(QString pathID)
{
		QFile inputFile("data/treeRingData/MissingPathFile.txt");
		if(inputFile.open(QIODevice::ReadOnly))
		{
			QTextStream in(&inputFile);
			QString line = in.readLine();  			    
			while ( !in.atEnd() )
			{			   
				line = in.readLine();  
				QStringList sname=line.split('\t');	
  				QString qstr=sname[0];			
				if(qstr==pathID)
				{
					
					return true;
				}
			}
		}
		return false;	
}

vector<vector<QString>> treeRingBubble::pathExpressionRead(expressionBubble *ebubble)
{//resize: true, remove the compartment if it is empty
	vector<vector<QString>> outPut;
	QString eName = ebubble->getCurrentFilename();
	int tindex = eName.lastIndexOf("/");
	QString temp=eName;
	temp.remove(tindex+1,eName.length());
	eName.remove(temp);

 	QString pathName = _scene->pwdParser->getPathExpressionFile(eName); //_scene->pwdParser->savePathGraphPreparation(_pathName);			
	pathName= pathName + ".epr";
	//QString fileName = QFileDialog::getSaveFileName(0, tr("Save Pathway Graph as xml file"), pathName,".xml (*.xml)");//"c:\\file.x", "X files (*.x)"
	QFile file(pathName);
	if(!file.open(QIODevice::ReadOnly | QIODevice::Truncate))
		return outPut;
	
    QTextStream in(&file);
	QString line = in.readLine();  			    
	while ( !in.atEnd() )
	{			   
		line = in.readLine();  
		QStringList sname=line.split('\t');	
		vector<QString> item;
  		item.push_back(sname[0]);	item.push_back(sname[1]);	item.push_back(sname[2]);		
		outPut.push_back(item);
	}
	file.close();
	return outPut;
}

set<QString> treeRingBubble::pathExpressionReadPathID(expressionBubble *ebubble)
{//resize: true, remove the compartment if it is empty
	set<QString> outPut;
	QString eName = ebubble->getCurrentFilename();
	int tindex = eName.lastIndexOf("/");
	QString temp=eName;
	temp.remove(tindex+1,eName.length());
	eName.remove(temp);

 	QString pathName = _scene->pwdParser->getPathExpressionFile(eName); //_scene->pwdParser->savePathGraphPreparation(_pathName);			
	pathName = pathName + ".epr";
	//QString fileName = QFileDialog::getSaveFileName(0, tr("Save Pathway Graph as xml file"), pathName,".xml (*.xml)");//"c:\\file.x", "X files (*.x)"
	QFile file(pathName);
	if(!file.open(QIODevice::ReadOnly | QIODevice::Truncate))
		return outPut;

		
    QTextStream in(&file);
	QString line = in.readLine();  			    
	while ( !in.atEnd() )
	{			   
		line = in.readLine();  
		QStringList sname=line.split('\t');			  		
		outPut.insert(sname[0]);
	}
	file.close();
	return outPut;
}


void treeRingBubble::pathExpressionRecord(expressionBubble *ebubble)
{//resize: true, remove the compartment if it is empty
	QString eName = ebubble->getCurrentFilename();
	int tindex = eName.lastIndexOf("/");
	QString temp=eName;
	temp.remove(tindex+1,eName.length());
	eName.remove(temp);

 	QString pathName = _scene->pwdParser->getPathExpressionFile(eName); //_scene->pwdParser->savePathGraphPreparation(_pathName);			
	pathName= pathName + ".epr";
	//QString fileName = QFileDialog::getSaveFileName(0, tr("Save Pathway Graph as xml file"), pathName,".xml (*.xml)");//"c:\\file.x", "X files (*.x)"

	set<QString> outPut = pathExpressionReadPathID(ebubble);
	vector<vector<QString>> outPut2 = pathExpressionRead(ebubble);

	QFile file(pathName);
	if(!file.open(QIODevice::WriteOnly))
		return;

    QTextStream out(&file);
	//out << "_VertexInfo.size()" << " " <<  _VertexInfo.size() << "\n";		
	
	for(int layer=0; layer<_num_layers; ++layer)
	{
		//expressionLevel[layer].resize(_ring[layer]._node_num);
		for(int i=0; i<_treeRing ->_ring[layer]._node_num; ++i)
		{
			//QString name = _treeRing -> _ring[layer]._node_name[i];
			QString pID = _treeRing -> _ring[layer]._node_pathID[i];
			if(outPut.find(pID)==outPut.end())
			{
				float elevel = _treeRing -> expressionLevel[layer][i];
				int matched = _treeRing -> _ring[layer]._node_matched[i];	        
				out << pID << "\t" <<  elevel << "\t" <<  matched << "\n";	
			}
		}
	}
	for(int i=0; i<outPut2.size(); i++)
	{
	    out << outPut2[i][0] << "\t" <<  outPut2[i][1] << "\t" <<  outPut2[i][2] << "\n";		
	}
	file.close(); 	
}




void treeRingBubble::setPenOn()
{
	QBitmap bitmap("icon/pencil.png");
	QCursor cursor (bitmap,bitmap);
		
	setCursor(cursor);   
	_penisOn=true;	
	_queryisOn=false;
	_grabisOn=false;
	_upisOn=_downisOn=_evenisOn=false;
	_contain.clear();
	_lines.clear();
	fills.clear();
}

void treeRingBubble::hoverMoveEvent( QGraphicsSceneHoverEvent *event )
{   
	//_label->mouseOvered=false;
	ItemBase::hoverMoveEvent(event);
	if( onBoarder( event->pos() ) )
	{
		 _treeRing->clearHighLight(	);	
		 for(int i=0; i<_num_layers; i++)
		    _bun[i]->clearHighlight();

		 _lMarker->clearHighlight();
	}
	else
	{
	    _treeRing->Passive(10, event->pos().x(), event->pos().y());//¼ÆËãÊó±êÑ¡ÖÐÁËtreeringÖÐµÄÄÄ¸önode£¬¼´control point
		if(_treeRing->GetHighlightState()!=0)
		{
			int hl = _treeRing->GetHighlightLayer();
			int hi = _treeRing->GetHighlightIndex();
			int tmp[2];
			int gnode[2], fromTo[2];
			if(hl>-1 && hi>-1)
			{	
				if(hl==0 && hi ==14)
					hl = hl;

				_treeRing->GetNodeIndexByLayer( hl, hi, hl, tmp); //changed when switching inner ring out
				_treeRing->GetGlobalNodeIndexByLayer( hl, hi, hl, gnode);//global index
				
				gnode[0]=hi, gnode[1]=hi+1;		
				if(_treeRing->_ring[hl]._node_num==2) //a fix to a special case
					gnode[0]=0, gnode[1]=1;		

				_bun[hl]->GetEdgeIndexByLayer(hl, gnode, fromTo); //indexÎªfromTo[0]µ½fromTo[1]µÄËùÓÐ±ß¸ßÁÁ
				_bun[hl]->Highlight(fromTo[0], fromTo[1]);

				if(hl==_num_layers-1)
				   _lMarker->Highlight(hi);
					
				_treeRing->respondLayer.clear(), _treeRing->respondIndex.clear();					
				_bun[hl]->respondNodes(fromTo[0], fromTo[1], hl, _treeRing->respondLayer, _treeRing->respondIndex);
				if(_treeRing->respondIndex.size()>2)
					_hl = _hl;
								
				_hl = hl;
				_hn = hi;
				_hx=event->pos().x(), _hy=event->pos().y();
				_nodeIndex[0]=tmp[0];
				_nodeIndex[1]=tmp[1];
				if(_hl!=_num_layers-1)
					_lMarker->clearHighlight();
			}
			else if(_treeRing->GetHighlightState()==HL_HANDLE)
			{
				gnode[0]=_treeRing->handleIndex, gnode[1]=_treeRing->handleIndex+1;		
				if(_treeRing->_ring[_treeRing->handleLayer]._node_num==2) //a fix to a special case
					gnode[0]=0, gnode[1]=1;		

				_bun[_treeRing->handleLayer]->GetEdgeIndexByLayer(_treeRing->handleLayer, gnode, fromTo); //indexÎªfromTo[0]µ½fromTo[1]µÄËùÓÐ±ß¸ßÁÁ
				_bun[_treeRing->handleLayer]->Highlight(fromTo[0], fromTo[1]);
			}
			else
			{
				_lMarker->clearHighlight();
				for(int i=0; i<_num_layers; i++)
				     _bun[i]->Highlight(-1, -1);
			}			
		}
		else
		{
			_lMarker->clearHighlight();
			//_bun[0]->Highlight(-1, -1);
			for(int i=0; i<_num_layers; i++)
				_bun[i]->Highlight(-1, -1);
			_hl=-1;
			_hn=-1;
			_treeRing->clearHighLight();
		}		
	}
	//int from = _bun[0]->getHFrom();
	//int to= _bun[0]->getHTo();
	//to = to;
}

void treeRingBubble::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{	
	//event->pos()-event->lastPos()
	_lines.clear(); fills.clear(); _contain.clear();	
	if(insideBubble(event->pos()))
	{
	   UNGROUP=false;
	   AFTERUNGROUP=false;	
	}
	if(AFTERUNGROUP || m_isMoving)
	{
		m_isMoving=false;
	   return;
	}
    if (!isSelected() && _scene) 
	{
		_scene->clearSelection();
		setSelected(true);
	}	
	QPointF dis = lastPressedPos == QPointF(0,-1000000)?QPointF(0,0): event->pos() - lastPressedPos;
	if( dis.x()*dis.x() +  dis.y()*dis.y() >400)
		bubbleDraggedout=true; //to disable menu if the right mouse button being dragged		
	else 
	{
		bubbleDraggedout=false;
	    lastPressedPos = QPointF(0,-1000000);
	}
	if(bubbleDraggedout)
	{
		bubbleDraggedout=false;
		return;
	}
	if(_scene->controlKeyisOn)
		return;

	vector<int> node;	
	
	if( ((_hl>=0 || _hn>=0) && isSegPreSelected(_hl, _hn, preItemSelected) ) || _hl<0 || _hn<0)	
	{
		_scene->dropDownMenu0(event, this, node);
	}
	
	
	
	//itemSelected.clear();
	//_treeRing->itemSelected.clear();
}

void treeRingBubble::wheelEvent(QGraphicsSceneWheelEvent *event)
{
	 if(!onBoarder( event->pos() ) && !_scene->naviContains(event->scenePos()))
	{
		float rate = event->delta();		
		if(rate>0)
		    rate= 0.1; //rate = - exp( - rate / 50.0);		
		else
		    rate= -0.1; //rate = exp( - rate / 50.0);		
		
			//reScale(x0y0, x1y1, (x0y0-event->pos())*rate+event->pos()-x0y0, (x1y1-event->pos())*rate+event->pos()-x1y1, _scale, dCenter);			
			Zoom(rate);
		
	}	
}


void treeRingBubble::Zoom(float zoom)
{
	if(_treeRing->_ring.size()==0 || _treeRing->_ring[0]._radius.size()==0)
		return;
	   
	_zoom = zoom;
	Point center = _treeRing->GetTreeRingCenter();
	float dx = -center.x;
	float dy = -center.y;
	
	if( (zoom>0 && _treeRing->_ring[0]._radius[0] > _treeRing->GetRadius()*4) || (zoom<0 && _treeRing->_ring[0]._radius[0] < _treeRing->GetRadius()*0.1) )
	{
	    return;
	}
	_treeRing->Translate(dx, dy);
	_treeRing->Scale(zoom);
	_treeRing->Translate(-dx, -dy);
	for(int i=0; i<_num_layers; i++)
	   _bun[i]->Scale(zoom);	
	_lMarker->Scale(zoom);	

	_Label->Scale(_zoom);
}

void treeRingBubble::Pan(float x, float y)
{
	_treeRing->Translate(x, y);
	
	for(int i=0; i<_num_layers; i++)
	   _bun[i]->Translate(x, y);

	_lMarker->Translate(x, y);
	_Label->Translate(x, y);
}

bool treeRingBubble::onBoarder( QPointF pos )
{
	int x = pos.x();
	int y = pos.y();
	int half = this->Width()/2;
	int half2 = this->Height()/2;
	if( (x > half - (HALOWIDTH+half*0.01+2)) || (x < -half + (HALOWIDTH+half*0.01+2)) )
	{
		return true;
	}
	if( (y > half2 - (HALOWIDTH+half2*0.01+2)) || (y < -half2 + (HALOWIDTH+half2*0.01+2)) )
	{
		return true;
	}
	return false;
}

bool treeRingBubble::outsideBubble( QPointF pos )
{
	int x = pos.x();
	int y = pos.y();
	int half = this->Width()/2;
	int half2 = this->Height()/2;
	if( (x > half ) || ( x < -half ) )
	{
		return true;
	}
	if( (y > half2 ) || ( y < -half2 ) )
	{
		return true;
	}
	return false;
}

bool treeRingBubble::UpdateHighlight(bool *firstMotion, int mx, int my)
{
	// get the offset
	static int oldMousex, oldMousey;
	int ox=0., oy=0.;

	if(*firstMotion)
	{
		oldMousex = mx;
		oldMousey = my;
		ox = 0.;
		oy = 0.;		
		return false;
	} 

	ox = mx-oldMousex;
	oy = my-oldMousey;
	oldMousex = mx;
	oldMousey = my;

	cerr << "compound graph: udpateHighlight"<<endl;	
		
	_treeRing->UpdateHighlightedPointToMouse(mx, my, ox, oy);
	
	if(_treeRing->getHighlightState() == HL_NODE)
		_bun[_hl]->UpdateBundles(_hl, _hn, _treeRing);	
	else if(_treeRing->getHighlightState() == HL_HANDLE)
	{
		//_bun->UpdateAllBundles(handleSelected[0], handleSelected[1], handleSelected[2], _treeRing);
		for(int i=0; i< _num_layers; i++)
		   _bun[i]->reGenerateCurveBundles(_treeRing, BUDDLINGSTRENGTH);
		_lMarker->updateMarkerAngle(_treeRing);
		spanIsOn=true;	
	}
	return false;
}


/*bool treeRingBubble::UpdateHandle(bool *firstMotion, int mx, int my)
{
	// get the offset
	static int oldMousex, oldMousey;
	int ox=0., oy=0.;

	if(*firstMotion)
	{
		oldMousex = mx;
		oldMousey = my;
		ox = 0.;
		oy = 0.;		
		return false;
	} 

	ox = mx-oldMousex;
	oy = my-oldMousey;
	oldMousex = mx;
	oldMousey = my;

	cerr << "compound graph: udpateHighlight"<<endl;	

	_treeRing->UpdateHandlePointToMouse(mx, my, ox, oy);
	_bun->UpdateBundles(_hl, _hn, _treeRing);

	if(_hl == 0 && _hn >= 0)
 	{
		float r1=_treeRing->GetNodeRadius(_hl, _hn), r2= _treeRing->GetRingRadius(_hl);
		if(_treeRing->GetNodeRadius(_hl, _hn) - _treeRing->GetRingRadius(_hl) >= RADIUS_LIMIT)
		{
			for(int i=0; i<_add.size(); ++i)
			{
				pair<int,int> p;
				p.first = _hl; p.second = _hn;
				if(p == _add[i])
					return true;
			}
			//AddSmallTreeRing();
			//AddDiagram();
		}
		return true;
	}
	return false;

}*/

void treeRingBubble::addLine( QPointF pos1, QPointF pos2 )
{
	_lines.append( QLineF( pos1, pos2 ) );
	return;
}

void treeRingBubble::mouseMoveEvent( QGraphicsSceneMouseEvent *event )
{		
	if( (m_isMoving || m_isResizing || onBoarder( event->pos() ) || pressOnBoarder || pressOnUngroup || onUnGroupArea(event->pos()) ) && !draggingItem && !_penisOn && !spanIsOn )
	{
		ItemBase::mouseMoveEvent( event );  //when scale is enabled
		//nodePicked.clear();	
		return;
	}
	if(_mousePressed && !m_isMoving && !m_isResizing && !outsideBubble(event->pos()))
	{	
		float S;
		//reScale(x0y0, x1y1, event->pos() - event->lastPos(), event->pos() - event->lastPos(), S, dCenter);				
		if (!m_rightButton) 
		{
			if(_penisOn)
		    {
				addLine( initPos, event->pos() );
				initPos = event->pos();
				nodePicked.clear();	
				_treeRing->clearHighLight();
			}			
			else if(!itemSelected.empty())
			{
				draggingItem=1;					
				dragItem(event->pos()-event->lastPos());	
				if(!onBoarder( event->pos() ) )
					dragout=1;
				if(dragout==2)
				{
					dragout=3;
					bubbleRect.setWidth(0), bubbleRect.setHeight(0);
				}
				else dragout=1;									
			}
			else if( handleSelected.size()>=3 && handleSelected[0]>-1)
			{
				if(!strokeMotionLeft(event->pos().x(), event->pos().y()))
			       m_isInnerMoving=true;
				nodePicked.clear();	
				//_treeRing->clearHighLight();
			}
			else if(nodePicked.size()>=2 && nodePicked[0]>-1)
			{
				draggingItem=1;
				_treeRing->_itemDragged=getItemDragged();
				dragItem(event->pos()-event->lastPos());	
				if(!onBoarder( event->pos() ) )
				    dragout=1;
			    if(dragout==2)
				{
					dragout=3;
					bubbleRect.setWidth(0), bubbleRect.setHeight(0);
				}
				else dragout=1;	
			}	
			else
			{
				QPointF Dis = event->pos() - event->lastPos();		
				Pan(Dis.x(), Dis.y());				
				nodePicked.clear();	
				_treeRing->clearHighLight();
			}
		}
		else if (m_rightButton) 
		{	
			if(!multiItemSelected.empty())
			{
				QPointF dis=event->pos()- event->lastPos();
			    if(fabs(float(dis.x()))>5 || fabs(float(dis.y()))>5) 
				{
					if(draggingItem==0)
					{
						_treeRing->_itemDragged = getItemDragged();
						draggingItem=1;
					}
					dragItem(event->pos()-event->lastPos());	
				
					if(!onBoarder( event->pos() ) )
						dragout=1;
					if(dragout==2)
					{
						dragout=3;
						bubbleRect.setWidth(0), bubbleRect.setHeight(0);
					}
					else dragout=1;		
					nodePicked.clear();	
					_treeRing->clearHighLight();
				}
			} 
           
		 }	
    }
	else if(_mousePressed && !m_isMoving && !m_isResizing && !_penisOn && !spanIsOn)
	{		
		if(!m_rightButton)
		{
			if(outsideBubble(event->pos()))
			{
				if(!itemSelected.empty())
				{
					if(bubbleRect.width()==0)
					{
						bubbleRect.setWidth(200), bubbleRect.setHeight(123);								
					}
					bubbleRect.moveCenter(event->pos());
					dragout=2;
				}
				else if(!nodePicked.empty() && nodePicked[0]!=-1)
				{
					if(bubbleRect.width()==0)
					{					
						bubbleRect.setWidth(200), bubbleRect.setHeight(123);								
					}
					bubbleRect.moveCenter(event->pos());
					dragout=2;
				}
				else 			 
				{		
					//if(bubbleRect.width()!=0)
					bubbleRect.setWidth(0), bubbleRect.setHeight(0);	
					ItemBase::mouseMoveEvent( event );
				}
			}
			else
			{				
			     nodePicked.clear();
				 _treeRing->clearHighLight();
			}
		}
		else
		{
			if(outsideBubble(event->pos()))
			{
				if(!multiItemSelected.empty())
				{
					if(bubbleRect.width()==0)
					{
						bubbleRect.setWidth(200), bubbleRect.setHeight(123);								
					}
					bubbleRect.moveCenter(event->pos());
					dragout=2;
				}
				else if(!nodePicked.empty() && nodePicked[0]!=-1)
				{
					if(bubbleRect.width()==0)
					{					
						bubbleRect.setWidth(200), bubbleRect.setHeight(123);								
					}
					bubbleRect.moveCenter(event->pos());
					dragout=2;
				}
				else if(!spanIsOn)			 
				{		
					//if(bubbleRect.width()!=0)
					bubbleRect.setWidth(0), bubbleRect.setHeight(0);	
					ItemBase::mouseMoveEvent( event );
					nodePicked.clear();	
					_treeRing->clearHighLight();
				}
			}
			else
			{		
				nodePicked.clear();	
				_treeRing->clearHighLight();
			}
		}
	}	
	else if(!_penisOn && !spanIsOn)
	{
	    ItemBase::mouseMoveEvent( event );		
		nodePicked.clear();	
		_treeRing->clearHighLight();
	}	
	
}

void treeRingBubble::dragItem(QPointF Pos)
{

	/*if(getVisibleItemNum()<=1)
		return;

	if(getVisibleCompartmentNum()<=1 && itemSelected[0]=='M')
	    return;
    */
	for(int i=0;i<itemDragged.size();i++)
	{
		//itemDragged[i].moveCenter(Pos+itemDragged[i].center());			
		//itemDragged[i].
		  QTransform trans;
          trans=trans.translate(Pos.x(), Pos.y());
          itemDragged[i]=trans.map(itemDragged[i]);
	}	
}

void treeRingBubble::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	ItemBase::mouseDoubleClickEvent(event);		
}


