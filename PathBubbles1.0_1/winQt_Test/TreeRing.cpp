/**
*/

#include <iostream>
#include <fstream>
#include <assert.h>
#include <vector>
#include <queue>
#include <set>
#include <QFile>
#include <QTextStream>
//#include <stdlib.h>
#include <GL/glut.h>

#include "Parser/TreeNameParser.h"

#include "TreeRing.h"
#include "LAB2RGB.h"
#include "Stroke.h"
#include "pathwayDataParser.h"

#define RES 6
#define HANDLERANGE 0.4
#define RATELIMITCOLOR QColor(255, 140, 140, 255)

#define COMPLETECOLOR QColor(0,90,50) 
#define INCOMPLETECOLOR QColor(161,217,155) 	
#define MISSINGCOLOR QColor(255,255,255,255)

#define MIDIUM_HIGHLIGHTCOLOR QColor(213,106,0,255) //QColor(20,20,255,255)
#define HIGHLIGHTCOLOR1 QColor(255,213,170,159) //QColor(220, 230, 255, 159) //for seg or node representative being dragging around/ and being mouse over
#define EDGECOLOR QColor(20,20,20,255)
#define MARKERCOLOR QColor(60,60,60,255)

TreeRing::TreeRing( char *tinf,  float r, float w, SimData *sim_data, int EDGE_BY, Point centre) 
{
	shrinked=false;
	layerStored=-1;

	expressionMin=0; 
	expressionMax=0.000001;	
	_radius = r;  
	_width =  w;
	_opaque = false;
	_ENABLE_controlPolygon = false;
	_ENABLE_controlPoint = true;
	_ENABLE_FULL = true;
	highlightLayer = -1; highlightIndex = -1;
	handleLayer = -1; handleIndex = -1;
	_centre = Point(0,0);
	tree_infile = strdup(tinf);
	_sim_data = sim_data;
	_edge_by = EDGE_BY;
	_pre_edge_by  = EDGE_BY;
	
	

	STILL_UPDATE = true;
	readInputTree();   // read the input file and set the ring
	Translate(centre.x, centre.y); //Here also change the _centre to the new centre
	ComputeNodeVariance();
	initNodeOrthRate();
	//GetNodeOrthRate("data/gallus_gallus_ortholog.txt");
	ComputeNodeCv();
	ComputeNodeDomain();
	ComputeNodeColor();
	
	nodePicked.push_back(-1); nodePicked.push_back(-1);

	STILL_UPDATE=false;
}



TreeRing::TreeRing( vector<vector<string>> inputName,  vector<vector<QString>> inputPathID, vector<vector<int>> inputNodeSize,   vector<vector<int>> rateLimitNum, float r, float w, SimData *sim_data, int EDGE_BY, Point centre, QString orthologyFileName) 
{
	shrinked=false;
	layerStored=-1;

	expressionMin=0; 
	expressionMax=0.000001;
	_radius = r;  
	_width =  w;
	_opaque = false;
	_ENABLE_controlPolygon = false;
	_ENABLE_controlPoint = true;
	_ENABLE_FULL = true;
	highlightLayer = -1; highlightIndex = -1;
	handleLayer = -1; handleIndex = -1;
	_centre = Point(0,0);
	
	//tree_infile = strdup(tinf);
	
	_sim_data = sim_data;
	_edge_by = EDGE_BY;

	STILL_UPDATE = true;

	storeInputTree(inputName, inputPathID, inputNodeSize, rateLimitNum);   // read the input file and set the ring

	Translate(centre.x, centre.y); //Here also change the _centre to the new centre
	ComputeNodeVariance();
	
	initNodeOrthRate();
	//GetNodeOrthRate("data/gallus_gallus_ortholog.txt");
	ComputeNodeCv();
	ComputeNodeDomain();
	ComputeNodeColor();
	if(orthologyFileName!="")
	{
		ComputeNodeColorByO(orthologyFileName);
	}
	nodePicked.push_back(-1); nodePicked.push_back(-1);
	STILL_UPDATE=false;
}


void TreeRing::Passive(float thre, int mx, int my)
{
	getClosest(thre, mx, my);
	CheckPointInNode(mx, my);

//Highlight selected node and all of its children
// 	if(highlightLayer==0)
// 	{
// 		_ring[0]._node_color[highlightIndex] = Color(0,0,0);
// 		for(int i=_ring[0]._child_range[highlightIndex].first; i<=_ring[0]._child_range[highlightIndex].second; ++i)
// 		{
// 			_ring[1]._node_color[i] = Color(0,0,0);
// 		}
// 
// 		for(int i=_ring[0]._child_range[highlightIndex].first; i<=_ring[0]._child_range[highlightIndex].second; ++i)
// 		{
// 			for(int j=_ring[1]._child_range[i].first; j<=_ring[1]._child_range[i].second; ++j)
// 				_ring[2]._node_color[j] = Color(0,0,0);
// 		}
// 	}
	
}

void  TreeRing::findAllDescendantNodes(int layer, int id, vector<vector<int>> &output)
{	
	if(layer==_ring.size()-1)
		return;
	if(layer+1 >= output.size())
	{
	    output.resize(layer+1+1);	
	}
	for(int j=_ring[layer]._child_range[id].first; j<=_ring[layer]._child_range[id].second; j++)
	{
		//item1[0] = layer+1; item1[1] = j;		
		output[layer+1].push_back(j);
		findAllDescendantNodes(layer+1, j, output);
	}		
}


void TreeRing::findAllDescendantNodes(int layer, int id, set<vector<int>> &toBeInsert)
{
	if(layer==_ring.size()-1)
		return;
	for(int j=_ring[layer]._child_range[id].first; j<=_ring[layer]._child_range[id].second; j++)
	{
		vector<int> item1(2,0);
		item1[0] = layer+1; item1[1] = j;
		toBeInsert.insert(item1);
		findAllDescendantNodes(layer+1, j, toBeInsert);
	}	
}

vector<string> TreeRing::findNameList(int layer, int id)
{//return the name list of topest layer to current node
	vector<string> nameList;
	nameList.resize(layer+1);
	int pid, pLayer;
	pLayer=layer, pid=id;
	while(pLayer!=0)
	{
		nameList[pLayer] = _ring[pLayer]._node_name[pid];
		pLayer = pLayer-1; 
	    pid = GetParentIndex(layer, pid);
	}
	nameList[pLayer] = _ring[pLayer]._node_name[pid];	
	return nameList;
}


void TreeRing::locateNodeByNameList(vector<string> Names, int layer, int &id, int startid, int endid)
{//return the name list of topest layer to current node	    
	if(layer==Names.size())
		return;
	
	if(layer==0) 
	{
		for(int j = 0; j< _ring[layer]._node_name.size(); j++)
	    {
			if( _ring[layer]._node_name[j] == Names[layer] )
			{
				id=j;
				if(layer==Names.size()-1)
					break;
	            locateNodeByNameList(Names, layer+1, id, _ring[layer]._child_range[j].first, _ring[layer]._child_range[j].second);		
				break;
			}
			
	    }
	}
	else
	{
	    for(int j = startid; j<= endid; j++)
	    {
			if( _ring[layer]._node_name[j] == Names[layer] )
			{
				id=j;
				if(layer==Names.size()-1)
					break;
	            locateNodeByNameList(Names, layer+1, id, _ring[layer]._child_range[j].first, _ring[layer]._child_range[j].second);
				break;
			}			
	    }	
	}
}


int TreeRing::GetGlobalNodeIndexByLayer( int slayer, int sNodeIndex,
										int tlayer, int tNodeIndexRange[2])
{
	assert(slayer<_num_layers && tlayer<_num_layers);
	int ssum=0, tsum=0;
	int ti=0;
	int tmpIndexRange[2];

	for(int si=0; si<sNodeIndex; si++) // count the source layer
	{
		ssum += (_ring[slayer]._node_size)[si];
	}; // end for(si)

	while(1)
	{
		tsum += (_ring[tlayer]._node_size)[ti];
		if(tsum >= ssum)
			break;
		ti++;
	}; // end while(1)

	tmpIndexRange[0]=ti;

	if(slayer<tlayer)
	{
		ssum += (_ring[slayer]._node_size)[sNodeIndex];
		while(1) {
			if(ti >= _ring[tlayer]._node_size.size()-1)
				break;
			ti++;			
			tsum += (_ring[tlayer]._node_size)[ti];
			if(tsum >= ssum)
				break;
		}; // end while(1)
		tmpIndexRange[1]=ti;
	}
	else
	{
		tmpIndexRange[1]=tmpIndexRange[0];
	};// end if(slayer<tlayer)

	tNodeIndexRange[0]=toGlobalNodeIndex(tlayer, tmpIndexRange[0]);
	tNodeIndexRange[1]=toGlobalNodeIndex(tlayer, tmpIndexRange[1]);
	//
	//  toEdgeIndex(tmpIndexRange[0], tmpIndexRange[1]);
	return 1;
}

int TreeRing::toGlobalNodeIndex(int layer, int bNodeIndex)
{
	int result=0;

	for(int bi=0; bi<=bNodeIndex; bi++)
		result += ((_ring[layer])._node_size)[bi];

	return (result-1);
}


// 
int TreeRing::GetParentIndex(int layerIndex, int nodeIndex)
{
	//assert(layerIndex>0);  //  layer 0 does not have parents
	if(layerIndex<=0)
		return nodeIndex;
	int _parent_layer = layerIndex - 1;
	for(int i=0; i<_ring[_parent_layer]._child_range.size(); ++i)
	{
		pair<int, int> &range = _ring[_parent_layer]._child_range[i];
		if(nodeIndex >= range.first && nodeIndex <= range.second)
			return i;
	}
}

int TreeRing::GetAChildInMiddle(int layerIndex, int nodeIndex)
{
	//assert(layerIndex>0);  //  layer 0 does not have parents
	if(layerIndex=_num_layers-1)
		return nodeIndex;
	int _child_layer = layerIndex + 1;
	return((_ring[layerIndex]._child_range[nodeIndex].first + _ring[layerIndex]._child_range[nodeIndex].second)/2);
	
}


int TreeRing::GetNodeIndexByLayer( int slayer, int sNodeIndex,
								  int tlayer, int tNodeIndexRange[2])
{
	assert(slayer<_num_layers && tlayer<_num_layers);
	int ssum=0, tsum=0;
	int ti=0;

	for(int si=0; si<sNodeIndex; si++) // count the source layer
	{
		ssum += (_ring[slayer]._node_size)[si];
	}; // end for(si)

	while(1)
	{
		tsum += (_ring[tlayer]._node_size)[ti];
		if(tsum >= ssum)
			break;
		ti++;
	}; // end while(1)

	tNodeIndexRange[0]=ti;

	if(slayer<tlayer)
	{
		ssum += (_ring[slayer]._node_size)[sNodeIndex];
		while(1) {
			ti++;
			if(ti>=_ring[tlayer]._node_size.size())
				break;
			tsum += (_ring[tlayer]._node_size)[ti]; //a bug here ti exceed the node_size.size() keqin
			if(tsum >= ssum)
				break;
		}; // end while(1)
		tNodeIndexRange[1]=ti;
	}
	else
	{
		tNodeIndexRange[1]=tNodeIndexRange[0];
	};// end if(slayer<tlayer)
	return 1;
}

int TreeRing::getAncestorNodebyLayer(int cLayer, int cIdx, int aLayer )
{
	if(cLayer==0)
		return cIdx;
	
    int pid=cIdx;
	int player=cLayer;
	while(player>aLayer)
	{
		pid=_ring[player]._parent[pid];	
		player--;
	}
	return pid;
}

vector<int> TreeRing::getChildrenNodes(int layer, int idx)//get leaves
{
	vector<int> nodes;
	int _num_layers = GetNrLayers();

	queue<int> q;
	q.push(idx);  //Index queue
	queue<int> ql;
	ql.push(layer);  //Layer queue

	while(!q.empty() && !ql.empty())
	{
		int index = q.front();//Get current index
		q.pop();  
		int layer = ql.front();//Get current layer
		ql.pop();  

		if(layer == _num_layers - 1)  //No children any more
			break;

		//Every child of this node
		for(int j=_ring[layer]._child_range[index].first; j<=_ring[layer]._child_range[index].second; ++j)
		{
			
			if(layer + 1 == _num_layers - 1)
			{
				//vector<int> item(2,0);
				//item[0]=layer;  item[1]=j;
				nodes.push_back(j);
			}
			q.push(j);
			ql.push(layer + 1);
		}
	}
	return(nodes);
}

vector<int> TreeRing::getParentNode(int layer, int idx)
{
	vector<int> nodes;
	int _num_layers = GetNrLayers();

	queue<int> q;
	q.push(idx);  //Index queue
	queue<int> ql;
	ql.push(layer);  //Layer queue

	while(!q.empty() && !ql.empty())
	{
		int index = q.front();//Get current index
		q.pop();  
		int layer = ql.front();//Get current layer
		ql.pop();  

		if(layer == _num_layers - 1)  //No children any more
			break;

		//Every child of this node
		for(int j=_ring[layer]._child_range[index].first; j<=_ring[layer]._child_range[index].second; ++j)
		{
			for(int k=0; k<(_ring[layer + 1]._pos[j]).size(); ++k)
			{
				//(_ring[layer + 1]._pos[j])[k].x += dx;
				//(_ring[layer + 1]._pos[j])[k].y += dy;
				vector<int> item(2,0);
				item[0]=layer;  item[1]=idx;
				//nodes.insert(item);
			}
			//Update the node radius
			Point &p = (_ring[layer+1]._pos[j])[0];
			_ring[layer+1]._radius[j] = sqrtf((p.x-_centre.x)*(p.x-_centre.x)+
												(p.y-_centre.y)*(p.y-_centre.y));
			/*if(layer + 1 == _num_layers - 1)
			{
				//Update the bundle starting position
				//_ring[layer+1]._bun_pos[j] += Point(dx, dy);
				vector<int> item(2,0);
				item[0]=layer;  item[1]=idx;
				//nodes.insert(item);
			}*/

			q.push(j);
			ql.push(layer + 1);
		}
	}
	return nodes;
}

QString TreeRing::getPathID(QString pathName)
{
		pathName = pathName.replace("_"," ");
		QString pathName1;
			
		QString tname;
		const char * ename;
		string sname;
		//QString lastStoredfileName;
		//struct expressedPathwayInfo infor;

		//pathName = pathName.replace(",","");
		QString fileName = "data/Reactome_Pathway_Data/pathwayTable/"+ pathName + ".path";
		QString num;
		QFile inputFile(fileName);
		if(inputFile.open(QIODevice::ReadOnly))
		{
			QTextStream in(&inputFile);
			QString line = in.readLine();  
			line = line.split('\n')[0];
			num=line;			  
		}					
		// e. g. "428359") //Binding of RNA by Insulin-like Growth Factor-2 mRNA Binding Proteins (IGF2BPs/IMPs/VICKZs)
		if(num =="")
		{
			pathName1 = pathName;
			pathName1 = pathName1.replace(",","");
			fileName = "data/Reactome_Pathway_Data/pathwayTable/"+ pathName1 + ".path";
			QFile inputFile(fileName);
			if(inputFile.open(QIODevice::ReadOnly))
			{
				QTextStream in(&inputFile);
				QString line = in.readLine();  
				line = line.split('\n')[0];
				num=line;			  
			}				
		}
		if(num=="")
		{
			fileName = "data/treeRingData/MissingPathFile.txt";
			QFile inputFile(fileName);
			if(inputFile.open(QIODevice::ReadOnly))
			{
			    QTextStream in(&inputFile);
				QString line = in.readLine();  			    
			    while ( !in.atEnd() )
			    {			   
					line = in.readLine();  
					QStringList sname=line.split('\t');	
  			        QString qstr=sname[1];			
					if(qstr==pathName || pathName1==qstr)
					{
					    num=sname[0];
						break;
					}
				}
			}
			inputFile.close();
			//search missing file
			//num=num;
		}
		if(num=="")
			num=num;
		inputFile.close();
		return num;
}

void TreeRing::UpdateHighlightedPointToMouse(int mx, int my, int ox, int oy)
{
	if(highlightState == HL_NODE)
		return;

	if(highlightState ==0) 
		return; 

	if(highlightState == HL_NODE)
	{
		float nx, ny, n2x, n2y;

		//_curve[highlightLayer].updateCtrlPoints(highlightIndex, mx, my);
		Point n1, n2, n3; //Use vector calculation to decide dx,dy along a direction
		n1.x = _ring[highlightLayer]._centre_pos[highlightIndex].x - _centre.x;
		n1.y = _ring[highlightLayer]._centre_pos[highlightIndex].y - _centre.y;
		n2.x = float(ox); //ox, oy is the distance the mouse has moved
		n2.y = float(oy);
		
// 		float dn1 = sqrtf(n1.x*n1.x + n1.y*n1.y);
// 		float d = (n1.x*n2.x + n1.y*n2.y) / dn1;
// 		n3.x = n1.x / dn1;
// 		n3.y = n1.y / dn1;
// 		
// 		int dx = int(n3.x * d + 0.5f);
// 		int dy = int(n3.y * d + 0.5f);
		//Using formula above
		float dx =  ( (n1.x * n2.x + n1.y * n2.y) * n1.x ) / (n1.x * n1.x + n1.y * n1.y);
		float dy =  ( (n1.x * n2.x + n1.y * n2.y) * n1.y ) / (n1.x * n1.x + n1.y * n1.y);

		for(int i=0; i<(_ring[highlightLayer]._pos[highlightIndex]).size(); ++i)
		{
			(_ring[highlightLayer]._pos[highlightIndex])[i].x += dx;
			(_ring[highlightLayer]._pos[highlightIndex])[i].y += dy;
		}
		//Update the radius
		Point &p = (_ring[highlightLayer]._pos[highlightIndex])[0];
		_ring[highlightLayer]._radius[highlightIndex] = sqrtf((p.x-_centre.x)*(p.x-_centre.x)+
														      (p.y-_centre.y)*(p.y-_centre.y));

		//If this node is in the layer 2, update the bundle start position
		//if(highlightLayer == _num_layers - 1)
			(_ring[highlightLayer]._bun_pos[highlightIndex]) += Point(dx, dy);
		
		//------------Move all the children with this highlighted node------------------------------------------
		queue<int> q;
		q.push(highlightIndex);  //Index queue
		queue<int> ql;
		ql.push(highlightLayer);  //Layer queue

		while(!q.empty() && !ql.empty())
		{
			int index = q.front();//Get current index
			q.pop();  
			int layer = ql.front();//Get current layer
			ql.pop();  

			if(layer == _num_layers - 1)  //No children any more
				break;

			//Every child of this node
			for(int j=_ring[layer]._child_range[index].first; j<=_ring[layer]._child_range[index].second; ++j)
			{
				for(int k=0; k<(_ring[layer + 1]._pos[j]).size(); ++k)
				{
					(_ring[layer + 1]._pos[j])[k].x += dx;
					(_ring[layer + 1]._pos[j])[k].y += dy;
				}
				//Update the node radius
				Point &p = (_ring[layer+1]._pos[j])[0];
				_ring[layer+1]._radius[j] = sqrtf((p.x-_centre.x)*(p.x-_centre.x)+
												   (p.y-_centre.y)*(p.y-_centre.y));
				//if(layer + 1 == _num_layers - 1)
				{
					//Update the bundle starting position
					_ring[layer+1]._bun_pos[j] += Point(dx, dy);
				}
				q.push(j);
				ql.push(layer + 1);
			}
		}
		//------------Move all the children with this highlighted node---------------------------------------------
	}
	else if(highlightState == HL_CURVE)
	{
		float n1x, n1y, n2x, n2y;
		_curve[highlightLayer].getPointByIndex(highlightIndex, &n1x, &n1y);
		_curve[highlightLayer].updateCtrlPoints(highlightIndex, n1x+ox, n1y-oy);
		_curve[highlightLayer].getPointByIndex( highlightIndex+1, &n2x, &n2y);
		_curve[highlightLayer].updateCtrlPoints( highlightIndex+1, n2x+ox, n2y-oy);
	}// end if(highlightState)
	else if(highlightState == HL_HANDLE)
	{
		//------------span all the children with this highlighted node---------------------------------------------	
		float nx, ny, n2x, n2y;

		//_curve[handleLayer].updateCtrlPoints(handleIndex, mx, my);
		Point n1, n2, n3; //Use vector calculation to decide dx,dy along a direction
		n1.x = _ring[handleLayer]._centre_pos[handleIndex].x - _centre.x;
		n1.y = _ring[handleLayer]._centre_pos[handleIndex].y - _centre.y;
		n2.x = float(ox); //ox, oy is the distance the mouse has moved
		n2.y = float(oy);

		//get new angles
		double newangle;
		newangle = atan2( my + oy - _centre.y, mx+ox - _centre.x );
		if(newangle<0)// && handleIndex==16)
			newangle=M_PI*2 + newangle;
		
		//double angle10= getAngleSpan( _ring[handleLayer]._limits[10].first,  _ring[handleLayer]._limits[10].second);		
		vector<double> preSpan; //perserve previous span for node size caculation
		for(int i=0; i<_ring[handleLayer]._node_num; i++)
		{
			preSpan.push_back(getAngleSpan(_ring[handleLayer]._limits[i].first, _ring[handleLayer]._limits[i].second));	
		}
		//spanAnode(handleLayer, handleIndex, handlet, newangle);
		spanAnode(handleLayer, handleIndex, handlet, newangle);
		reRadialLayout(preSpan);
	    reCalculateNodePosition();		
	}// end if(handleState)
}

vector<int> TreeRing::getSegs(vector<int> respondLayer, vector<int> respondIndex, bool selected)
{	//if selected is 1: get ring segments for the selected node
	//if selected is 2: get ring segments for the rest node
	int layer, id;
	vector<int> item(2,0), item2, item3;
	vector<int> segEnds;
	vector<vector<int>> ends; //record start and end node of each layer
	set<vector<int>> itemSelected;
	bool contituous = true;

	if(respondLayer.empty())
		return segEnds;

	for(int i=0; i<respondLayer.size(); i++)
	{
		layer=respondLayer[i], id=respondIndex[i];
		item[0]=layer, item[1]=id;
		itemSelected.insert(item);
	}
	if(!selected)
	{
		set<vector<int>> itemSelected1;
		layer=respondLayer[0];		
		for(int i=0; i<_ring[layer]._node_num; i++)
	    {
			//layer=respondLayer[i], id=respondIndex[i];
			item[0]=layer, item[1]=i;
			if(itemSelected.find(item)==itemSelected.end())
			{
				itemSelected1.insert(item);
			}
	    }
		itemSelected = itemSelected1;
	}

	if(itemSelected.empty())
		return segEnds;

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
		
	//layer=0; 
	item.resize(2); item2.resize(2); item3.resize(2);
	item[0]=item2[0]=item3[0]=layer;
			
	int maxj=_ring[layer]._node_num;
	if(ends[layer][0]== 0 && ends[layer][1] == maxj-1 && contituous)
	{
		segEnds.push_back(ends[layer][0]);		
		segEnds.push_back(ends[layer][1]);	
	}
	else
	{
		for(int j=ends[layer][0]; j<=ends[layer][1]; j++)
		{
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
					if(j==ends[layer][1] && segEnds[0]==-1)
					{
						segEnds[0]=j;			 
					}
					else
						segEnds.push_back(j);		
				}
				if(j==ends[layer][1] && !flag1 && !flag2 )
				{
					if(segEnds[0]==-1)
					{
						segEnds[0]=segEnds[segEnds.size()-1];	
						segEnds.resize(segEnds.size()-1);
					}
				}				 
			}		
		}
	}		
	/*for(int j=0; j< segEnds.size(); j=j+2)
	{
		int id1=segEnds[j], id2=segEnds[j+1];				
		int cornerID1, cornerID2;
		//segPoints.push_back(getSegPoint(0, _num_layers-1, id1, id2, Pos.x(), Pos.y(), cornerID1, cornerID2));		
		cornerNode_2.push_back(cornerID1);
		cornerNode_2.push_back(cornerID2);		
	}*/
	return segEnds;
}

void TreeRing::unShrinkNodes()
{
	if(layerStored<0 || _angleStored.empty())
		return;

	_ring[layerStored]._angle = _angleStored; 
    _ring[layerStored]._limits = _limitsStored;
	_ring[layerStored]._radial_pos = _radial_posStored;

	layerStored=-1;
	_angleStored.clear();
	_limitsStored.clear();
	_radial_posStored.clear();

	reCalculateNodePosition(layerStored);		
}

void TreeRing::shrinkNodes(vector<int> respondLayer, vector<int> respondIndex)
{
	//change ring._limits, ring._angle (ring._limits[i].first) and ring._radial_pos
	if(respondLayer.empty())
	{
		shrinked=false;
		return;
	}
	//recode the data before changed;
	vector<QPolygon> segPoints;

	vector<int> cornerNode_2; 
	//vector<int> idSet1 = getSegs(respondLayer, respondIndex, 1); //idSet1 segs that are highlighted
	vector<int> idSet2 = getSegs(respondLayer, respondIndex, 0); //idSet1 segs that are highlighted
	static const double Pi2 = 3.14159265358979323846264338327950288419717*2; 
	int layer = respondLayer[0];
	Ring ring = _ring[layer];
	int nodeNum = ring._node_num; 
	float averageAngle = Pi2/nodeNum;

	if(averageAngle>0.15)
		averageAngle=0.15;
	else if(averageAngle<0.08)
		averageAngle=0.08;

	for(int j=0; j< idSet2.size(); j=j+2)
	{
		int id1=idSet2[j], id2=idSet2[j+1];
		float angle1=ring._limits[id1].first, angle2=ring._limits[id2].second;
		if(angle1>angle2)
		{
		   angle2= Pi2 + angle2;
		   id2 = nodeNum + id2;
		}
		//get the center
	    float cAngle = (angle1 + angle2)/2;

		//if(cAngle>=Pi2)
		//	cAngle=cAngle-Pi2;

		float oriSpan = angle2 - angle1;
		float scale = (oriSpan-averageAngle)/oriSpan;
		if(scale<=0.1)
			 scale=0.1;
		for(int i=id1; i<=id2; i++)
		{
			int id=i;
		    if(id>=nodeNum)
				id=id-nodeNum;

			float first = ring._limits[id].first;
			float second = ring._limits[id].second;

			if( first<angle1 || first>angle2)  // not in the span
			{
			    first = first + Pi2;
			}
			if( first<angle1 || first>angle2)  // maybe error case
			{
				break;
			}
			if( second<angle1 || second>angle2)  // not in the span
			{
			    second = second + Pi2;
			}
			if( second<angle1 || second>angle2)  // maybe error case
			{
				break;
			}
			
			first = cAngle + (first-cAngle)*scale;
			if(first >= Pi2)
			    first = first - Pi2;

			ring._limits[id].first = first;

			second = cAngle + (second-cAngle)*scale;
			if(second>=Pi2)
			    second = second - Pi2;
			
			ring._limits[id].second = second;

			ring._angle[id] = ring._limits[id].first; //ring._limits[id].second)/2;

			Point p;	
					
			p.x = cosf(ring._angle[id] ) * ring._ring_radial_radius;
			p.y = sinf(ring._angle[id] ) * ring._ring_radial_radius;	

			ring._radial_pos[id] = p + _centre;		
		}
	}
	Ring ring2=_ring[layer]; //for debug
	layerStored = layer;
	_angleStored = ring2._angle; //Angle of each node on the ring
    _limitsStored = ring2._limits;
	_radial_posStored = ring2._radial_pos;

	/*vector<float> _angle1 = ring._angle; //Angle of each node on the ring
    vector< pair<float, float> > _limits1=ring._limits;
	vector<Point> _radial_pos1=ring._radial_pos;*/

	_ring[layer] = ring;

	reCalculateNodePosition(layer);		

	//adjust ring._limits, _ring[handleLayer]._pos, ._angle[

    /* 
	// changes _ring[x]._pos, _ring[x]._limites,  _ring[x]._angle

	//span all those nodes
	//regenerate all nodes

	//float newA=newangle;
	//float testA=newangle; //for debug
	float angle;
	
	float first=_ring[handleLayer]._limits[handleIndex].first;
	float second=_ring[handleLayer]._limits[handleIndex].second;
	if(handleIndex==0)
	{
		angle = _ring[handleLayer]._angle[handleIndex];
		first = _ring[handleLayer]._limits[handleIndex].first;
		second = _ring[handleLayer]._limits[handleIndex].second;		
	}	
	
	if( handlet==0 )
	{
		if( newA - _ring[handleLayer]._limits[handleIndex].first > M_PI) //make sure the moving note pass the second point
		{
			if(newA - 2. * M_PI>=0)
			   newA = newA - 2. * M_PI;
		}
		if( _ring[handleLayer]._limits[handleIndex].first - newA > M_PI) //make sure the moving not pass the second point
		{
			if(newA + 2. * M_PI <= 2. * M_PI)
			   newA = newA + 2. * M_PI;
		}	
		if( first <= second )
		{			
			 if( newA -_ring[handleLayer]._limits[handleIndex].first < M_PI 
				&& newA > _ring[handleLayer]._limits[handleIndex].first
				&& _ring[handleLayer]._limits[handleIndex].second - newA <0.03)		        				 
			 {
				 newangle = _ring[handleLayer]._limits[handleIndex].first;
			 }			 
		}
		
		Point p;
		int preID = (handleIndex-1+_ring[handleLayer]._node_num)%_ring[handleLayer]._node_num;
		
		p.x = _ring[handleLayer]._radius[handleIndex] * cosf(newangle);
		p.y = _ring[handleLayer]._radius[handleIndex] * sinf(newangle);
		
		_ring[handleLayer]._pos[handleIndex][0]=p;
		_ring[handleLayer]._pos[preID][2]=p;

		p.x = (_ring[handleLayer]._radius[handleIndex] - _width) * cosf(newangle);
		p.y = (_ring[handleLayer]._radius[handleIndex] - _width) * sinf(newangle);
		_ring[handleLayer]._pos[handleIndex][1]=p;
		_ring[handleLayer]._pos[preID][3]=p;

		_ring[handleLayer]._limits[handleIndex].first=newangle;
		
		_ring[handleLayer]._limits[preID].second=newangle;
		_ring[handleLayer]._angle[handleIndex]=newangle;
		
	}
	else if (handlet==1 )
	{
		if( newA - _ring[handleLayer]._limits[handleIndex].second > M_PI) //make sure the moving not pass the second point
		{
			if(newA - 2. * M_PI>=0)
			   newA=newA - 2. * M_PI;
		}
		if( _ring[handleLayer]._limits[handleIndex].second - newA > M_PI) //make sure the moving not pass the second point
		{
			if(newA + 2. * M_PI <= 2. * M_PI)
			    newA = newA + 2. * M_PI;
		}		
		if( _ring[handleLayer]._limits[handleIndex].second - newA  < M_PI 
		&& newA < _ring[handleLayer]._limits[handleIndex].second 
		&& newA - _ring[handleLayer]._limits[handleIndex].first <0.03)		        
		{
				newangle = _ring[handleLayer]._limits[handleIndex].second;				
		}
		Point p;
		int nextID = (handleIndex+1)%_ring[handleLayer]._node_num;
		p.x = _ring[handleLayer]._radius[nextID] * cosf(newangle);
		p.y = _ring[handleLayer]._radius[nextID] * sinf(newangle);
		//v.push_back(p);
		_ring[handleLayer]._pos[nextID][0]=p;
		_ring[handleLayer]._pos[handleIndex][2]=p;

		p.x = (_ring[handleLayer]._radius[nextID] - _width) * cosf(newangle);
		p.y = (_ring[handleLayer]._radius[nextID] - _width) * sinf(newangle);
		_ring[handleLayer]._pos[nextID][1]=p;
		_ring[handleLayer]._pos[handleIndex][3]=p;

        _ring[handleLayer]._limits[handleIndex].second=newangle;
		_ring[handleLayer]._limits[nextID].first=newangle;
		_ring[handleLayer]._angle[nextID]=newangle;

	}
	else if(handlet==2 ) //second == first case for those tree ring only have one top-level nodes
	{
		Point p;
		int nextID = (handleIndex+1)%_ring[handleLayer]._node_num;
		p.x = _ring[handleLayer]._radius[nextID] * cosf(newangle);
		p.y = _ring[handleLayer]._radius[nextID] * sinf(newangle);
		//v.push_back(p);
		_ring[handleLayer]._pos[nextID][0]=p;
		_ring[handleLayer]._pos[handleIndex][2]=p;

		//p.x = (_ring[handleLayer]._radius[nextID] - _width) * cosf(newangle);
		//p.y = (_ring[handleLayer]._radius[nextID] - _width) * sinf(newangle);
		_ring[handleLayer]._pos[nextID][1]=p;
		

        _ring[handleLayer]._limits[handleIndex].second=newangle;
		_ring[handleLayer]._limits[nextID].first=newangle;
		_ring[handleLayer]._angle[nextID]=newangle;

		if( testA - newangle > M_PI )
			testA = testA;
	}
	*/
}


void TreeRing::spanAnode(int handleLayer, int handleIndex, int handlet, float newangle)
{
	//adjust ring._limits, _ring[handleLayer]._pos, ._angle[	
	float newA=newangle;
	float testA=newangle; //for debug
	float angle;
	
	float first=_ring[handleLayer]._limits[handleIndex].first;
	float second=_ring[handleLayer]._limits[handleIndex].second;
	if(handleIndex==0)
	{
		angle = _ring[handleLayer]._angle[handleIndex];
		first = _ring[handleLayer]._limits[handleIndex].first;
		second = _ring[handleLayer]._limits[handleIndex].second;		
	}	
	
	if( handlet==0 )
	{
		if( newA - _ring[handleLayer]._limits[handleIndex].first > M_PI) //make sure the moving note pass the second point
		{
			if(newA - 2. * M_PI>=0)
			   newA = newA - 2. * M_PI;
		}
		if( _ring[handleLayer]._limits[handleIndex].first - newA > M_PI) //make sure the moving not pass the second point
		{
			if(newA + 2. * M_PI <= 2. * M_PI)
			   newA = newA + 2. * M_PI;
		}	
		if( first <= second )
		{			
			 if( newA -_ring[handleLayer]._limits[handleIndex].first < M_PI 
				&& newA > _ring[handleLayer]._limits[handleIndex].first
				&& _ring[handleLayer]._limits[handleIndex].second - newA <0.03)		        				 
			 {
				 newangle = _ring[handleLayer]._limits[handleIndex].first;
			 }			 
		}
		

		Point p;
		int preID = (handleIndex-1+_ring[handleLayer]._node_num)%_ring[handleLayer]._node_num;
		
		p.x = _ring[handleLayer]._radius[handleIndex] * cosf(newangle);
		p.y = _ring[handleLayer]._radius[handleIndex] * sinf(newangle);
		
		_ring[handleLayer]._pos[handleIndex][0]=p;
		_ring[handleLayer]._pos[preID][2]=p;

		p.x = (_ring[handleLayer]._radius[handleIndex] - _width) * cosf(newangle);
		p.y = (_ring[handleLayer]._radius[handleIndex] - _width) * sinf(newangle);
		_ring[handleLayer]._pos[handleIndex][1]=p;
		_ring[handleLayer]._pos[preID][3]=p;

		_ring[handleLayer]._limits[handleIndex].first=newangle;
		
		_ring[handleLayer]._limits[preID].second=newangle;
		_ring[handleLayer]._angle[handleIndex]=newangle;
	}
	else if (handlet==1 )
	{
		if( newA - _ring[handleLayer]._limits[handleIndex].second > M_PI) //make sure the moving not pass the second point
		{
			if(newA - 2. * M_PI>=0)
			   newA=newA - 2. * M_PI;
		}
		if( _ring[handleLayer]._limits[handleIndex].second - newA > M_PI) //make sure the moving not pass the second point
		{
			if(newA + 2. * M_PI <= 2. * M_PI)
			    newA = newA + 2. * M_PI;
		}		
		if( _ring[handleLayer]._limits[handleIndex].second - newA  < M_PI 
		&& newA < _ring[handleLayer]._limits[handleIndex].second 
		&& newA - _ring[handleLayer]._limits[handleIndex].first <0.03)		        
		{
				newangle = _ring[handleLayer]._limits[handleIndex].second;				
		}
		Point p;
		int nextID = (handleIndex+1)%_ring[handleLayer]._node_num;
		p.x = _ring[handleLayer]._radius[nextID] * cosf(newangle);
		p.y = _ring[handleLayer]._radius[nextID] * sinf(newangle);
		//v.push_back(p);
		_ring[handleLayer]._pos[nextID][0]=p;
		_ring[handleLayer]._pos[handleIndex][2]=p;

		p.x = (_ring[handleLayer]._radius[nextID] - _width) * cosf(newangle);
		p.y = (_ring[handleLayer]._radius[nextID] - _width) * sinf(newangle);
		_ring[handleLayer]._pos[nextID][1]=p;
		_ring[handleLayer]._pos[handleIndex][3]=p;

        _ring[handleLayer]._limits[handleIndex].second=newangle;
		_ring[handleLayer]._limits[nextID].first=newangle;
		_ring[handleLayer]._angle[nextID]=newangle;

	}
	else if(handlet==2 ) //second == first case for those tree ring only have one top-level nodes
	{
		Point p;
		int nextID = (handleIndex+1)%_ring[handleLayer]._node_num;
		p.x = _ring[handleLayer]._radius[nextID] * cosf(newangle);
		p.y = _ring[handleLayer]._radius[nextID] * sinf(newangle);
		//v.push_back(p);
		_ring[handleLayer]._pos[nextID][0]=p;
		_ring[handleLayer]._pos[handleIndex][2]=p;

		//p.x = (_ring[handleLayer]._radius[nextID] - _width) * cosf(newangle);
		//p.y = (_ring[handleLayer]._radius[nextID] - _width) * sinf(newangle);
		_ring[handleLayer]._pos[nextID][1]=p;
		/*_ring[handleLayer]._pos[handleIndex][3]=p;*/

        _ring[handleLayer]._limits[handleIndex].second=newangle;
		_ring[handleLayer]._limits[nextID].first=newangle;
		_ring[handleLayer]._angle[nextID]=newangle;

		if( testA - newangle > M_PI )
			testA = testA;
	}
	
}

void TreeRing::Print()
{
	int cs = _curve.size();
	for(int ci=0; ci<cs; ci++)
	{
		printf(" ======= %d ======\n", ci);
		_curve[ci].printCtrlPoints();
	}; 
}


void TreeRing::SetEnableControlPolygon(int newState)
{
	_ENABLE_controlPolygon = newState;
}

void TreeRing::SetEnableControlPoint(int newState)
{
	_ENABLE_controlPoint = newState;
}

void TreeRing::storeInputTree(vector<vector<string>> inputName, vector<vector<QString>> inputPathID, vector<vector<int>> inputNodeSize, vector<vector<int>> rateLimitNum)
{
	//TreeNameParser *tnp = new TreeNameParser(tree_infile);
	vector<Ring> myr;//1, myr2, myr3;
	// this is the outside ring - the highest level parents
	myr.resize(inputName.size());
	for(int i=0; i<inputName.size(); i++)
	{
		for(int j=0; j<inputName[i].size(); j++)
		{
			myr[i]._node_name.push_back(inputName[i][j]); 	
			/////keqin
		}
	}
	for(int i=0; i<inputPathID.size(); i++)
	{
		for(int j=0; j<inputPathID[i].size(); j++)
		{
			myr[i]._node_pathID.push_back(inputPathID[i][j]); 	
			/////keqin
		}
	}
	for(int i=0; i<inputNodeSize.size(); i++)
	{
		for(int j=0; j<inputNodeSize[i].size(); j++)
			myr[i]._node_size.push_back(inputNodeSize[i][j]); 		    
	}
	for(int i=0; i<rateLimitNum.size(); i++)
	{
		for(int j=0; j<rateLimitNum[i].size(); j++)
			myr[i]._rateLimitNum.push_back(rateLimitNum[i][j]); 	
	}
	
	//myr1._node_size = inputName[0].size(); //tnp->GetLevelXParCount(0);
	//myr2._node_size = tnp->GetLevelXParCount(1);
	//myr3._node_size = tnp->GetLevelXParCount(2);
	float ring_dist = 3.;
	int n_layers = myr.size()-1;
	for(int i=0; i<myr.size(); i++)
	{
		myr[i]._node_num = myr[i]._node_size.size();
		myr[i]._ring_radius = _radius - (n_layers-i) * _width - (n_layers-i) * ring_dist; //changed when switching inner ring out // _radius - i * _width - i * ring_dist
		for(int j=0; j<myr[i]._node_num; ++j)
			myr[i]._radius.push_back(myr[i]._ring_radius);

		myr[i]._ring_radial_radius = _radius - (5-(n_layers-i) ) *_width - (5- (n_layers-i) ) * ring_dist;//changed when switching inner ring out // _radius - i * _width - i * ring_dist
		for(int j=0; j<myr[i]._node_num; ++j)
			myr[i]._radial_radius.push_back(myr[i]._ring_radial_radius);

		_ring.push_back(myr[i]);
	}

	//myr2._node_num = myr2._node_size.size();
	//myr3._node_num = myr3._node_size.size();
	_node_count = myr[myr.size()-1]._node_num;
		
	/*float ring_dist = 5.;
		
	myr3._ring_radius = _radius - 2.*_width - 2*ring_dist;
	for(int i=0; i<myr3._node_num; ++i)
		myr3._radius.push_back(myr3._ring_radius);

	myr2._ring_radius = _radius - _width - ring_dist;
	for(int i=0; i<myr2._node_num; ++i)
		myr2._radius.push_back(myr2._ring_radius);

	myr1._ring_radius = _radius;
	for(int i=0; i<myr1._node_num; ++i)
		myr1._radius.push_back(myr1._ring_radius);

	myr1._ring_radial_radius = _radius - 5.*_width - 5.*ring_dist;
	for(int i=0; i<myr1._node_num; ++i)
		myr1._radial_radius.push_back(myr1._ring_radial_radius);

	myr2._ring_radial_radius = _radius - 4.*_width - 4.*ring_dist;
	for(int i=0; i<myr2._node_num; ++i)
		myr2._radial_radius.push_back(myr2._ring_radial_radius);

	myr3._ring_radial_radius = _radius - 3.*_width - 3.*ring_dist;
	for(int i=0; i<myr3._node_num; ++i)
		myr3._radial_radius.push_back(myr3._ring_radial_radius);
    
	_ring.push_back(myr1);
	_ring.push_back(myr2);
	_ring.push_back(myr3);
	*/

	_num_layers=_ring.size();

	CalculateNrChildren();
	RadialLayout();
	CalculateNodePosition();
    
	//delete tnp;
}

void TreeRing::loadRateLimit()
{
	set<QString> rateLimitSymbol;
	//pwdParser->readRateLimitSymbol("data/Reactome_Pathway_Data/rateLimitsymbol.txt", limitRateSymbol);
	pwdParser->readRateLimitSymbol("data/Reactome_Pathway_Data/ratelimitsymbol.txt", rateLimitSymbol);

	//use number of cross-talk proteins	
	int min=1000000, max=-1000000;
	//vector<vector<int>> rateLimitNum; 
	//rateLimitNum.resize(_num_layers);
	for(int layer=0; layer<_num_layers; ++layer)
	{
		//expressionLevel[layer].resize(_ring[layer]._node_num);
		//rateLimitNum[layer].resize(_ring[layer]._node_num);
		_ring[layer]._rateLimitNum.resize(_ring[layer]._node_num);
		for(int i=0; i<_ring[layer]._node_num, i<_ring[layer]._pos.size();++i)
		{	
			string name=GetNodeName(layer,i);
			QString num=GetPathID(layer, i);
			/*QString pathName=QString::fromStdString(name);
			pathName = pathName.replace("_"," ");
			
			//QString tname;
			//const char * ename;
			//string sname;
			//struct expressedPathwayInfo infor;

			QString fileName = "data/Reactome_Pathway_Data/pathwayTable/"+ pathName + ".path";
			QString num;
			QFile inputFile(fileName);
		    if(inputFile.open(QIODevice::ReadOnly))
			{
				QTextStream in(&inputFile);
				QString line = in.readLine();  
				line = line.split('\n')[0];
				num=line;			  
			}			
			*/
			//searchexpressed
			//QString num=*it;
			QString fname = "data/Reactome_Pathway_Data/pathwayTable/" + num + "_";			
			QString tname = fname + "7protein.txt"; 
			string sname= tname.toStdString();  
			const char * ename=sname.c_str();					
				
			vector<vector<QString>> _proteinName;
			vector<set<vector<int>>> CompartmentContain;
			vector<vector<QString>> _compartmentName;	
			vector<QRectF> _compartmentPos;	
			
			int count=0;
			if(num!="")
			{
				PWDParser *pwdParser;
				int proteinNum = pwdParser->read7protein(ename, _proteinName, CompartmentContain, _compartmentName, _compartmentPos);
				int pNum = proteinNum ;				
				for(int k=1; k<= pNum; k++)
				{
					//for(int j=0; j< symbol.size(); j++)
					{
						if(rateLimitSymbol.find(_proteinName[k][0])!=rateLimitSymbol.end() ||  rateLimitSymbol.find(_proteinName[k][4])!=rateLimitSymbol.end())						
						{							 
							count++;								
						}
						
					}					
				}	
			}
			else
			{
			   count=0;			
			}
			//infor.pathIDs=num;
			//infor.expressionLevel=proteinNum==0?0:float(count)/proteinNum;
			//rateLimitNum[layer][i]=count;
			_ring[layer]._rateLimitNum[i]=count;
			if(count<min)
				min= count;
			else if(count>max)
				max= count;
		}		
	}
	//float length=max-min;
	rateLimitMin=min; 
	rateLimitMax=max;
	/*for(int layer=0; layer<_num_layers; ++layer)
	{
		Ring &ring = _ring[layer];
		for(int i=0; i<_ring[layer]._node_num, i<_ring[layer]._pos.size();++i)
		{	
			float value = length==0?0:(rateLimitNum[layer][i]-min)/length;
			float r, g, b;
			//if(value>0.1)
			//	value=1;
			//else value=value*10;			
			TDHelper::GetMultHue(value, 2, r, g, b); //yellow_to_blue
			ring._color_by_crosstalk.push_back(Color(r/255, g/255, b/255, 0.8));
		}
	}*/

}
	

void TreeRing::readInputTree()
{
	TreeNameParser *tnp = new TreeNameParser(tree_infile);
	
	// this is the outside ring - the highest level parents
	int mlev = tnp->GetMaxLevel();	
	for(int i=0; i<mlev; i++)
	{
		Ring myr;
		myr._node_name = tnp->GetLevelXParameters(i); 	
				
		//change upper case to lower case
		for(int j=0; j<myr._node_name.size(); j++)
		{
			QString num= getPathID(QString::fromStdString(myr._node_name[j]));
			myr._node_pathID.push_back(num);

            QString qstr = QString::fromStdString( myr._node_name[j] );
			//QString qstr1= qstr.mid(0,1); 
			//QString qstr2= qstr.mid(1,qstr.size());
			//vector<QString> qstr1;
            
			QStringList sname=qstr.split('_');	
			bool flag=false;
			for(int k=1; k<sname.size(); k++)
			{
				if(sname[k].size()>=2)
				{
					if(! sname[k][0].isUpper() || !sname[k][1].isUpper())
				   {
					   sname[k] = sname[k].toLower();
					   flag=true;
					}
				}
				else 
				{
					sname[k] = sname[k].toLower();
					flag=true;
				}
			}
			if(flag)
			{
				qstr = sname[0];
				for(int k=1; k<sname.size(); k++)
				{
					qstr = qstr + "_" + sname[k];				
				}
			}
			qstr.replace("_"," ");
			myr._node_name[j] = qstr.toStdString();			
		}
		
		myr._node_size = tnp->GetLevelXParCount(i);
		myr._node_num = myr._node_size.size();
		
		if(i==mlev-1)
		  _node_count = myr._node_num;
		
		float ring_dist = 3.;
		int  _num_layers= mlev-1;
		myr._ring_radius = _radius - (_num_layers-i)*_width - (_num_layers-i)*ring_dist;//changed when switching inner ring out // _radius - i * _width - i * ring_dist
		for(int i=0; i<myr._node_num; ++i)
			myr._radius.push_back(myr._ring_radius);

		myr._ring_radial_radius = _radius - (5-(_num_layers-i))*_width - (5-(_num_layers-i))*ring_dist;//changed when switching inner ring out //_radius - (5-i)*_width - (5-i)*ring_dist;
		for(int i=0; i<myr._node_num; ++i)
			myr._radial_radius.push_back(myr._ring_radial_radius);

		_ring.push_back(myr);		
    }
	_num_layers=_ring.size();

	CalculateNrChildren();
	RadialLayout();
	CalculateNodePosition();
	loadRateLimit();
	delete tnp;
}

void TreeRing::getClosest(float thre, int mx, int my)
{
	// give the outer ring the highest priority
	int cs = _curve.size();
	for(int ci=0; ci<cs; ci++)
	{
		_curve[ci].getClosest(thre, mx, my);
		if(_curve[ci].highlightIndex!=-1)
		{
			this->highlightLayer = ci; 
			this->highlightIndex = _curve[ci].highlightIndex;
			highlightState = HL_NODE;
			//printf("highlight node: %d %d -- ", this->highlightLayer, this->highlightIndex);
			return;
		}
		else if(_curve[ci].highlightCurveIndex!=-1)
		{
			this->highlightLayer = ci; 
			this->highlightIndex = _curve[ci].highlightCurveIndex;
			highlightState = HL_CURVE;
			//printf("highlight curve: %d %d -- ", this->highlightLayer, this->highlightIndex);
			return;
		}; // end if(_curve[ci]
	}; // end for(ci)

	// Update the highlighted node positions

	// nothing gets highlighted
	this->highlightLayer=-1;
	this->highlightIndex=-1;
	this->handleLayer=-1;
	this->handleIndex=-1;
	highlightState = 0;     
	//printf("highlight: %d %d -- ", this->highlightLayer, this->highlightIndex);
}

void TreeRing::cleanStrokeCurve()
{
	_curve.clear();
}


vector < SimDataElem > TreeRing::GetData()
{
	return _sim_data->GetData();
}

void TreeRing::initNodeOrthRate()
{
	QString tname;
	const char * ename;
	string sname;
	for(int layer=0; layer<_num_layers; layer++)
	{	
		for(int i=0; i<_ring[layer]._node_num; ++i)
		{
			float rate=0;
			_ring[layer]._node_orthRate.push_back(rate); //sim_data->GetVariance(i));			
		}
	}
	//_node_orthRate    
}

int TreeRing::SearchFind(vector<QString> IDs)
{
	//searched.clear();
	for(int j=0; j< IDs.size(); j++)
	   getNodeIDsbyID(IDs[j], searched);	
	return searched.size();
}

void TreeRing::getNodeIDsbyID(QString ID, set<vector<int>>  &searched)
{	
	set<vector<int>> outPut;
	//searched.clear();	
	vector<int> item(2, 0);
	for(int layer=0; layer<_num_layers; layer++)
	{	
		for(int i=0; i<_ring[layer]._node_num; ++i)
		{
			//get name of the Node		
			if(ID == _ring[layer]._node_pathID[i])			
			{
				item[0]=layer; item[1]=i;
				searched.insert(item);
			}
		}
	}		
	//searched=outPut;	
	//return outPut;	
}


int TreeRing::SearchFind(QString str, int matchType)
{	
	int count=0;
	searched.clear();
	for(int layer=0; layer<_num_layers; layer++)
	{	
		for(int i=0; i<_ring[layer]._node_num; ++i)
		{
			//get name of the Node		
			QString pathName = QString::fromStdString(_ring[layer]._node_name[i]), p0, s0;			
			//if(pathName.contains("Resolution"))
			//	pathName = pathName;
			pathName = pathName.replace('_',' ');

			bool flag=false, flag1, flag2;	
			int idx=0;
			switch(matchType)
			{
				case 0: flag = pathName.contains(str,Qt::CaseInsensitive) && pathName.size()==str.size(); break;
				case 1: flag = pathName.contains(str,Qt::CaseInsensitive); break;
				case 2: //idx = pathName.indexOf(str, 0, Qt::CaseInsensitive);
						flag1 = pathName.indexOf(str, 0, Qt::CaseInsensitive) ==0; 
					    p0 = pathName.mid(0,1), s0=str.mid(0,1);
					    flag2 = QString::compare(p0, s0, Qt::CaseInsensitive)==0;
					    flag = (flag1 && flag2);
					    break;
				case 3: //flag =  && pathName[pathName.size()-1]==str[str.size()-1];  
					    flag1 = pathName.lastIndexOf(str, -1, Qt::CaseInsensitive)>=0; 
					    p0 = pathName.mid(pathName.size()-1,1), s0=str.mid(str.size()-1,1);
					    flag2 = QString::compare(p0, s0, Qt::CaseInsensitive)==0;
					    flag = (flag1 && flag2);
					    break;								
			}
			if(flag)
			{
				vector<int> item;
				item.push_back(layer); item.push_back(i);
				searched.insert(item);
				count++;
			}
		}
	}
	return count;
}

void TreeRing::GetNodeOrthRate(QString onthName)
{
	QString tname;
	const char * ename;
	string sname;

	int idx1,idx2;
	idx1=onthName.lastIndexOf("/");
	idx2=onthName.lastIndexOf(".");
	QString onthName_t;
	onthName_t=onthName.mid(idx1+1,idx2-idx1-1);

	QString nfName="data/treeRingData/path_normal_b_11.out";	
	//QString onthName="data/gallus_gallus_ortholog.txt";	
	QString toName="data/treeRingData/path_"+ onthName_t + "_b.txt";	
	
	//QFile NFile(nfName);
	QFile OnthFile(onthName);

	QFile TOFile(toName);

	if(!TOFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
	     pwdParser->buildTreeRingData(onthName);
	     OnthFile.open(QIODevice::ReadOnly | QIODevice::Text);	
	}
	
	//QFile TOFile(toName);

	
	//TOFile.open(QIODevice::WriteOnly | QIODevice::Text);
	
	vector<QString> pathSet;
	vector<float> rateSet;
	QTextStream in2(& TOFile);			 
	while ( !in2.atEnd() )
	{			   
		QString line = in2.readLine();    
		QStringList sname=line.split(' ');	

		
            QString qstr=sname[1];
			QString qstr1= qstr.mid(0,1); 
			QString qstr2= qstr.mid(1,qstr.size());
			qstr2 = qstr2.toLower();
			qstr=qstr1+qstr2;
			qstr = qstr.replace( "_" , " ");	
			//myr._node_name[j]=qstr.toStdString();

		pathSet.push_back(qstr);
		
		QString rate=sname[2];
		rateSet.push_back(rate.toFloat());
		//nameSet.insert(sname[0]);
		//out6 << sname[1] << " ";	
	}
			
	for(int layer=0; layer<_num_layers; layer++)
	{	
		for(int i=0; i<_ring[layer]._node_num; ++i)
		{
			/*for(int j=_ring[layer]._child_range[i].first; j<= _ring[layer]._child_range[i].second; ++j)
			{
			//double max_variance = -1000;			
			//for(int j=_ring[layer]._child_range[i].first; j<= _ring[layer]._child_range[i].second; ++j)			
			//get Name of the Node
			}*/
			//get name of the Node			
			
			QString pathName = QString::fromStdString(_ring[layer]._node_name[i]);
			
			float rate=0;

			for(int k=0; k<pathSet.size(); k++)
	        {				
				if(pathSet[k]==pathName)
				{
					rate=rateSet[k];
					if(!(rate>=0 && rate<=1.0) )
						rate = 0;
					break;
				}				
			}

		
            _ring[layer]._node_orthRate[i]=rate; //sim_data->GetVariance(i));			
		}
	}
	//_node_orthRate    
}

void TreeRing::ComputeNodeVariance()
{
	//inner most layers
	for(int i=0; i<_ring[_num_layers - 1]._node_num; ++i)
			_ring[_num_layers - 1]._node_variance.push_back(_sim_data->GetVariance(i));


	//outer layers 
	for(int layer=_num_layers-2; layer>=0; --layer)
	{	
		for(int i=0; i<_ring[layer]._node_num; ++i)
		{
			double max_variance = -1000;
			int child_id;

			for(int j=_ring[layer]._child_range[i].first; j<= _ring[layer]._child_range[i].second; ++j)
				if(max_variance < _ring[layer+1]._node_variance[j])
				{
					max_variance = _ring[layer+1]._node_variance[j];
					child_id = j;
				}

			_ring[layer]._node_variance.push_back(max_variance);
			_ring[layer]._max_variance_child.push_back(child_id);
		}
	}
}

void TreeRing::ComputeNodeCv()
{
	//layer 2
	for(int i=0; i<_ring[_num_layers - 1]._node_num; ++i)
			_ring[_num_layers - 1]._node_cv.push_back(_sim_data->GetCV(i));

	//layer 1, layer 0
	for(int layer=_num_layers-2; layer>=0; --layer)
	{	
		for(int i=0; i<_ring[layer]._node_num; ++i)
		{
			double max_cv = -1000;
			int child_id;

			for(int j=_ring[layer]._child_range[i].first; j<= _ring[layer]._child_range[i].second; ++j)
				if(max_cv < _ring[layer+1]._node_cv[j])
				{
					max_cv = _ring[layer+1]._node_cv[j];
					child_id = j;
				}

				_ring[layer]._node_cv.push_back(max_cv);
				_ring[layer]._max_cv_child.push_back(child_id);
		}
	}
}

//Each parameter has 18 values, Calculate its standard deviation, 
//Then decide which domain the value is in, these two steps were compeleted in simdata
//Each node will be given a color according to the domain of the corresponding parameter
//The color of parent node is determined by the child node has the largest absolute value (distance to the mean)
//There are 5 intervals, so use 5-scale colormap
void TreeRing::ComputeNodeDomain()
{
	//Create a vector to store the absolute value of each node
	vector< vector<double> > distance_to_mean;
	vector<double>tmp;
	for(int i=0; i<_num_layers; ++i)
		distance_to_mean.push_back(tmp);

	//Create a tmp vector to store the level of interval
	//Two symmetrical intervals have the same level but the different interval id
	//This vector is used to help my calculation
	vector< vector<int> > domain_level;
	vector<int> tmp2;
	for(int i=0; i<_num_layers; ++i)
		domain_level.push_back(tmp2);

	//layer 2----------------------------------------------------------------------
	for(int i=0; i<_ring[_num_layers - 1]._node_num; ++i)
	{
		double domain_value = _sim_data->GetDomain(i);
		_ring[_num_layers - 1]._node_domain.push_back(domain_value);

		distance_to_mean[_num_layers - 1].push_back(fabs(_sim_data->GetSd(i) - _sim_data->GetMean(i)));

		//calculate the interval level
		if(domain_value == 0)
			domain_level[_num_layers - 1].push_back(0);
		else if(domain_value == 1 || domain_value == 2)
			domain_level[_num_layers - 1].push_back(1);
		else if(domain_value == 3 || domain_value == 4)
			domain_level[_num_layers - 1].push_back(2);
	}

	//the other layers
	for(int layer = _num_layers - 2; layer >= 0; --layer)
	{
		for(int i=0; i<_ring[layer]._node_num; ++i)
		{
			//first find the max interval
			int max_domain = -1;
			for(int j=_ring[layer]._child_range[i].first; j<= _ring[layer]._child_range[i].second; ++j)
				if(max_domain < domain_level[layer+1][j])
					max_domain = domain_level[layer+1][j];
			
			//then find the node in this interval with largest absolute value
			int max_ab_value = -999999;
			int child_id;
			for(int j=_ring[layer]._child_range[i].first; j<= _ring[layer]._child_range[i].second; ++j)
			{
				if(domain_level[layer+1][j] == max_domain && 
				   max_ab_value < distance_to_mean[layer + 1][j])
				{
					max_ab_value = distance_to_mean[layer + 1][j];
					child_id = j;
				}
			}
			_ring[layer]._node_domain.push_back(_ring[layer + 1]._node_domain[child_id]);			
			domain_level[layer].push_back(max_domain);
			distance_to_mean[layer].push_back(max_ab_value);
		}
	}
}




void TreeRing::InitNodeColorByE()
{	
	for(int layer=0; layer<_num_layers; ++layer)
	{
		Ring &ring = _ring[layer];

		for(int i=0; i<ring._node_num; ++i)
		{
			double var = ring._node_variance[i];
			var = log(var);

			Color co=Color(0.75,0.75,0.75,0.75);
			ring._color_by_expression.push_back(co);			
			ring._node_expression.push_back(0);	
			ring._node_matched.push_back(0);	
		}
	}
}


void TreeRing::InitNodeColorByO()
{
	for(int layer=0; layer<_num_layers; ++layer)
	{
		Ring &ring = _ring[layer];

		for(int i=0; i<ring._node_num; ++i)
		{
			//double var = ring._node_variance[i];
			//var = log(var);
			Color co=Color(0.75,0.75,0.75,0.75);
			ring._color_by_orthology.push_back(co);			
			ring._node_matched.push_back(0);	
		}
	}
}


void TreeRing::ComputeNodeColorByO(QString oName)
{
	_orthologyFileName = oName;
	GetNodeOrthRate(oName);	
	_orthName=oName;	
	int tindex = oName.lastIndexOf("/");
	QString temp=oName;
	temp.remove(tindex+1,temp.length());
	_orthName.remove(temp);	
	//tindex = name.lastIndexOf(".");	
	_orthName.remove("_ortholog.txt");
	
	QColor missingColor=MISSINGCOLOR, incompleteColor=INCOMPLETECOLOR, completeColor=COMPLETECOLOR;
	for(int layer=0; layer<_num_layers; ++layer)
	{
		Ring &ring = _ring[layer];
		for(int i=0; i<ring._node_num; ++i)
		{
			double value = ring._node_orthRate[i];
			value=value;			
			float r, g, b, a;			
			//TDHelper::GetMultHue(value, 3, r, g, b); //yellow_to_blue
			if(value==1)
				r=8,g=29,b=88;
			
			if(value==1)
				r=completeColor.red(), g=completeColor.green(), b=completeColor.blue(), a=completeColor.alpha();  //r=missingColor.red(), g=177, b=211, a=1; //complete 49,163,84
			else if(value==0 && layer!=0) //keqin
				r=missingColor.red(), g=missingColor.green(), b=missingColor.blue(), a=missingColor.alpha(); //missing 229,245,224
				//r=0, g=0, b=0, a=0.0; //missing
			else r=incompleteColor.red(), g=incompleteColor.green(), b=incompleteColor.blue(), a=incompleteColor.alpha(); //r=141,g=211, b=199, a=1; //imcomplete ,217,155
				 //three color	
			
			 ring._color_by_orthology[i]=Color(r/255, g/255, b/255, a);			
		}
	}
}

/*void TreeRing::ComputeNodeColorByO(ddddd)
{
	for(int layer=0; layer<_num_layers; ++layer)
	{
		Ring &ring = _ring[layer];
		for(int i=0; i<ring._node_num; ++i)
		{
			double value = ring._node_orthRate[i];
			value=value;
			float r, g, b;			
			TDHelper::GetMultHue(value, 3, r, g, b); //yellow_to_blue
			//Color co=Color(0.5,0.5,0.5,0.5);
			//if(_ring[layer]._node_matched[i]==1)
			//{
			 ring._color_by_orthology[i]=Color(r/255, g/255, b/255, 0.8);
			//}
            //ring._color_by_orthology.push_back(co);			
			//ring._node_matched.push_back(0);	
		}
	}
}
*/

void TreeRing::ComputeNodeColorByV()
{   
	set<QString> crossTalkSymbol;
	pwdParser->readCrossTalkSymbol("data/Reactome_Pathway_Data/crosstalksymbol.txt", crossTalkSymbol);
	//use number of cross-talk proteins	
	int min=1000000, max=-1000000;
	vector<vector<int>> crossTalkNum; 
	crossTalkNum.resize(_num_layers);
	
	for(int layer=0; layer<_num_layers; ++layer)
	{
		//expressionLevel[layer].resize(_ring[layer]._node_num);
		_ring[layer]._node_crossTalk.clear();
		crossTalkNum[layer].resize(_ring[layer]._node_num);
		for(int i=0; i<_ring[layer]._node_num, i<_ring[layer]._pos.size(); ++i)
		{	
			string name=GetNodeName(layer,i);
			
			QString num = GetPathID(layer,i);
			//searchexpressed
			//QString num=*it;
			QString fname = "data/Reactome_Pathway_Data/pathwayTable/" + num + "_";			
			QString tname = fname + "7protein.txt"; 
			string sname= tname.toStdString();  
			const char * ename=sname.c_str();					
				
			vector<vector<QString>> _proteinName;
			vector<set<vector<int>>> CompartmentContain;
			vector<vector<QString>> _compartmentName;	
			vector<QRectF> _compartmentPos;	
			
			int count=0;
			if(num!="")
			{
				PWDParser *pwdParser;
				int proteinNum = pwdParser->read7protein(ename, _proteinName, CompartmentContain, _compartmentName, _compartmentPos);
				int pNum = proteinNum ;				
				for(int k=1; k<= pNum; k++)
				{
					if(crossTalkSymbol.find(_proteinName[k][0])!=crossTalkSymbol.end() ||  crossTalkSymbol.find(_proteinName[k][4])!=crossTalkSymbol.end())
					{							 
						count++;								
					}
				}	
			}
			else
			{
			   count=0;			
			}			
			crossTalkNum[layer][i]=count;
			if(count<min)
				min= count;
			else if(count>max)
				max= count;
		}		
	}
	
	crossTalkMin=min; 
	crossTalkMax=max;
	cMin=int(min); 
	cMax=int(max);
	cMarkers = TDHelper::getLegendParameter(cMin, cMax, 'I', 10, cNum, cDL, cItv);	
	float length=cMax-cMin;
	for(int layer=0; layer<_num_layers; ++layer)
	{
		Ring &ring = _ring[layer];
		for(int i=0; i<_ring[layer]._node_num, i<_ring[layer]._pos.size();++i)
		{	
			float r, g, b;
			float value;
			if(length==0)
				value=0;			
			else
			{
				//value =(crossTalkNum[layer][i]-cMin)/length;
			    //value = float (int (value * cNum))/cNum*0.9;	

				value =(crossTalkNum[layer][i]-cMin)/length;
			    value = float (int (value * length))/length*0.86+0.1;	
			}
			//float value=float(i+0.5)/cnum;
			//if(value>0.1)
			//	value=1;
			//else value=value*10;			
			TDHelper::GetMultHue(value, 7, r, g, b); 
			ring._color_by_crosstalk.push_back(Color(r/255, g/255, b/255, 0.8));
			ring._node_crossTalk.push_back(crossTalkNum[layer][i]);			
		}
	}	
}

void TreeRing::ComputeNodeColorByCv()
{
	double unit_angle_cv = 2. * M_PI / (_sim_data->GetMaxCCV() - _sim_data->GetMinCCV());

    float five_numbers[6];
	_sim_data->GetFiveNumbersCV(five_numbers);

	//Get the color map
	int color_num = 100;
	LABColors *lab_color = new LABColors(75, 100, color_num);
	vector<Color> color = lab_color->GetColor();
	delete lab_color;

	//Divide the color map into 4 zones
	int c[5];
	c[0] = 0;
	c[1] = color_num / 4 - 1; 
	c[2] = color_num / 2 - 1; 
	c[3] = color_num / 2 + color_num / 4 - 1; 
	c[4] = color_num - 1;

	int k = color_num / 4;

	for(int layer=0; layer<_num_layers; ++layer)
	{
		Ring &ring = _ring[layer];

		for(int i=0; i<ring._node_num; ++i)
		{
			double cv = ring._node_cv[i];
			cv = log(cv);
			Color co;
			if(cv >= five_numbers[0] && cv < five_numbers[1])
			{
				co = color[c[0] + (cv - five_numbers[0]) / (five_numbers[1] - five_numbers[0]) * k];
			}
			else if(cv >= five_numbers[1] && cv < five_numbers[2])
			{
				co = color[c[1] + (cv - five_numbers[1]) / (five_numbers[2] - five_numbers[1]) * k];
			}
			else if(cv >= five_numbers[2] && cv < five_numbers[3])
			{
				co = color[c[2] + (cv - five_numbers[2]) / (five_numbers[3] - five_numbers[2]) * k];
			}
			else if(cv >= five_numbers[3] && cv <= five_numbers[4])
			{
				co = color[c[3] + (cv - five_numbers[3]) / (five_numbers[4] - five_numbers[3]) * k];
			}
			else 
			{
				co = Color(0.5,0.5,0.5,0.5);
			};
			ring._color_by_cv.push_back(co);
		}		
	}
}


void TreeRing::ComputeNodeColorByDomain()
{
	for(int layer=0; layer<_num_layers; ++layer)
	{
		Ring &ring = _ring[layer];

		for(int i=0; i<ring._node_num; ++i)
		{
			float gray = 0.2 * ring._node_domain[i];

			ring._color_by_domain.push_back(Color(gray, gray, gray, 1.0f));
		}
	}
}

//Map variance or cv to a color map
void TreeRing::ComputeNodeColor()
{
    ComputeNodeColorByV();
	ComputeNodeColorByCv();
	ComputeNodeColorByDomain();

	InitNodeColorByE();
	InitNodeColorByO();
	//GetNodeOrthRate("data/gallus_gallus_ortholog.txt");
	//ComputeNodeColorByO();
	//ComputeNodeColorByO_2(); (rate by number of matched protein/total number in orthology table)	
}

/*
vector<vector<QString>>TreeRing::pathExpressionRead(expressionBubble *ebubble)
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

set<QString> TreeRing::pathExpressionReadPathID(expressionBubble *ebubble)
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
*/

vector<struct expressedPathwayInfo> TreeRing::searchExpressed(vector<vector<QString>> geneInfo, vector<vector<float>> quantity, set<QString> matchedPathways, set<QString> pathID, vector<vector<QString>> pathInfo, set<int> &expresseGeneLinkedtoTreeRing)
{//search expressed pathway and compute expressed pathway's color
	vector<struct expressedPathwayInfo>  expressedPathways;
	float min=100000, max=-100000;		
	
	expressionLevel.resize(_num_layers);
	for(int layer=0; layer<_num_layers; ++layer)
	{
		expressionLevel[layer].resize(_ring[layer]._node_num);
		_ring[layer]._node_expression.resize(_ring[layer]._node_num);
		for(int i=0; i<_ring[layer]._node_num, i<_ring[layer]._pos.size();++i)
		{	
			struct expressedPathwayInfo infor;
			string name=GetNodeName(layer,i);			
			QString num = GetPathID(layer,i);
			if(matchedPathways.find(num)!=matchedPathways.end() ) 
			{
				if(pathID.find(num)!= pathID.end())
				{
					for(int k=0; k<pathInfo.size(); k++)
					{				
						if( pathInfo[k][0]== num)
						{					
							infor.pathIDs = num;
							infor.expressionLevel = pathInfo[k][1].toFloat();
							expressedPathways.push_back(infor);
							expressionLevel[layer][i] = infor.expressionLevel;							
							break;					
						}
					}
				}
				else
				{
					QString fname = "data/Reactome_Pathway_Data/pathwayTable/" + num + "_";			
					QString tname = fname + "7protein.txt"; 
					string sname= tname.toStdString();  
					const char * ename=sname.c_str();					
				
					vector<vector<QString>> _proteinName;
					vector<set<vector<int>>> CompartmentContain;
					vector<vector<QString>> _compartmentName;	
					vector<QRectF> _compartmentPos;	

					PWDParser *pwdParser;
					int proteinNum = pwdParser->read7protein(ename, _proteinName, CompartmentContain, _compartmentName, _compartmentPos);
					int pNum = proteinNum ;
					int count=0;
					for(int k=1; k<= pNum; k++)
					{
						for(int j=0; j< geneInfo.size(); j++)
						{
							if(_proteinName[k][0].contains(geneInfo[j][1])||_proteinName[k][4].contains(geneInfo[j][1]))
							{
								 if(quantity[j][0]!=0 || quantity[j][1]!=0)
								 {
									 if(quantity[j][0]!=0)
									 {
										  if((quantity[j][1]-quantity[j][0])/quantity[j][0]>2.0) //keqin
										  {
												count++;	
												expresseGeneLinkedtoTreeRing.insert(j);											
										  }
										  else if((quantity[j][1]-quantity[j][0])/quantity[j][0]<0.5)
										  {
												count++;	
												expresseGeneLinkedtoTreeRing.insert(j);											
										  }
									 }						
								 }												
							}						
						}					
					}		
					infor.pathIDs=num;
					infor.expressionLevel=proteinNum==0?0:float(count)/proteinNum;
					expressedPathways.push_back(infor);
					expressionLevel[layer][i]=infor.expressionLevel;		
				}
			}
			else
			{
				expressionLevel[layer][i]=0;				
			}	
			if(expressionLevel[layer][i]<min)
				min= expressionLevel[layer][i];
			else if(expressionLevel[layer][i]>max)
				max= expressionLevel[layer][i];
		}
	}	
	expressionMin=min; 
	expressionMax=max;
	eMin=min; 
	eMax=max;	
	eMarkers = TDHelper::getLegendParameter(eMin, eMax, 'F', 10, eNum, eDL, eItv);
	float length=eMax-eMin;

	float gitv = eItv / 5.0;
	int grad = 5;
	length=length*5;

	for(int layer=0; layer<_num_layers; ++layer)
	{
		for(int i=0; i<_ring[layer]._node_num, i<_ring[layer]._pos.size();++i)
		{	
			float value = length==0? 0: float(int((expressionLevel[layer][i]-eMin)*5))/length;
			float r, g, b;
			//value = float (int (value * eNum) )/eNum;			
			
			value=value*0.9;
			if(value>1)
				value=value;

			TDHelper::GetMultHue(value, 8, r, g, b); //white_to_orange
			_ring[layer]._color_by_expression[i]=Color(r/255, g/255, b/255, 0.8);	
			_ring[layer]._node_expression[i]=expressionLevel[layer][i];	
		}
	}
	return expressedPathways;
}

vector<struct expressedPathwayInfo> TreeRing::searchExpressed(QString eName, vector<vector<QString>> geneInfo, vector<vector<float>> quantity, set<QString> matchedPathways, set<int> &expresseGeneLinkedtoTreeRing)
{
	vector<struct expressedPathwayInfo>  expressedPathways;
	float min=100000, max=-100000;		
	
	expressionLevel.resize(_num_layers);	

	QString tname;
	const char * ename;
	string sname;

	int tindex = eName.lastIndexOf("/");
	QString temp=eName;
	temp.remove(tindex+1,eName.length());
	tname=eName;
	tname.remove(temp);

	//QString nfName="data/treeRingData/path_normal_b_11.out";	
	//QString onthName="data/gallus_gallus_ortholog.txt";	
	QString toName="data/treeRingData/path_"+ tname + "_expression_b.txt";	
	
	QFile TOFile(toName);

	if(!TOFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
	     pwdParser->buildTreeRingData_by_E(eName, geneInfo, quantity);
	     //eFile.open(QIODevice::ReadOnly | QIODevice::Text);	
	}	

	TOFile.open(QIODevice::ReadOnly | QIODevice::Text);
	//QFile TOFile(toName);
	//TOFile.open(QIODevice::WriteOnly | QIODevice::Text);
	
	vector<QString> pathIDSet;	
	vector<int> matchedSet;
	vector<float> expressionSet;
	QTextStream in2(& TOFile);			 
	while ( !in2.atEnd() )
	{			   
		QString line = in2.readLine();    
		QStringList sname=line.split(' ');	
		if(sname[0] == "expresseGeneLinkedtoTreeRing:")
		{
			for(int i=1; i<sname.size(); i++)
			{
				QString names=sname[i];	
				expresseGeneLinkedtoTreeRing.insert(names.toInt());
			}		
		}
		else if(sname[0] == "matchedGene_TreeRing_Expression:")
		{
		    	
		}
		else
		{
			QString qstr=sname[1];
			pathIDSet.push_back(qstr);
		
			if(sname.size()>3)
			{
				QString rate=sname[3];
				QString level=sname[4];

				expressionSet.push_back(level.toFloat());		
				matchedSet.push_back(rate.toInt());		
			}
			else
			{
				expressionSet.push_back(0);		
				matchedSet.push_back(0);		
			}
		}
	}		
	//Color co;	
	for(int layer=0; layer<_num_layers; ++layer)
	{
		expressionLevel[layer].resize(_ring[layer]._node_num);
		for(int i=0; i<_ring[layer]._node_num, i<_ring[layer]._pos.size();++i)
		{	
			struct expressedPathwayInfo infor;			
			QString num = GetPathID(layer,i);
			if(num.size()==0)
				continue;
			for(int k=0; k<pathIDSet.size(); k++)
	        {				
				if(pathIDSet[k]==num)
				{
					infor.pathIDs = num;
					infor.expressionLevel = expressionSet[k];
					expressedPathways.push_back(infor);
					expressionLevel[layer][i] = infor.expressionLevel;		
					break;
				}				
			}	
			if(expressionLevel[layer][i]<min)
				min= expressionLevel[layer][i];
			else if(expressionLevel[layer][i]>max)
				max= expressionLevel[layer][i];
		}
	}
	expressionMin=min; 
	expressionMax=max;
	eMin=min; 
	eMax=max;
	
	eMarkers = TDHelper::getLegendParameter(eMin, eMax, 'F', 10, eNum, eDL, eItv);
	float length=eMax-eMin;

	for(int layer=0; layer<_num_layers; ++layer)
	{
		for(int i=0; i<_ring[layer]._node_num, i<_ring[layer]._pos.size();++i)
		{	
			float value = length==0? 0:(expressionLevel[layer][i]-eMin)/length;
			float r, g, b;
			//value = float (int (value * eNum) +0.5 )/eNum;		
			//if(length==0)
			//	value=0;
			value=value*0.9;
			if(value>1)
				value=1;
			TDHelper::GetMultHue(value, 8, r, g, b); //white_to_orange
			_ring[layer]._color_by_expression[i]=Color(r/255, g/255, b/255, 0.8);			
		}
	}
	return expressedPathways;
}

void TreeRing::searchMatchedProtein(QString eName, vector<vector<QString>> geneInfo, vector<vector<float>> quantity, set<QString> &matchedPathways, set<int> &matchedGene_TreeRing_Expression)
{	
	QString tname;
	const char * ename;
	string sname;

	int tindex = eName.lastIndexOf("/");
	QString temp=eName;
	temp.remove(tindex+1,eName.length());
	tname=eName;
	tname.remove(temp);

	//QString nfName="data/treeRingData/path_normal_b_11.out";	
	//QString onthName="data/gallus_gallus_ortholog.txt";	
	QString toName="data/treeRingData/path_"+ tname + "_expression_b.txt";	
	
	//QFile NFile(nfName);
	//QFile eFile(eName);	
	QFile TOFile(toName);
	if(!TOFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
	     pwdParser->buildTreeRingData_by_E(eName, geneInfo, quantity);
	     //eFile.open(QIODevice::ReadOnly | QIODevice::Text);	
	}	
	//QFile TOFile(toName);
	TOFile.open(QIODevice::ReadOnly | QIODevice::Text);
	
	vector<QString> pathIDSet;	
	vector<int> matchedSet;
	QTextStream in2(& TOFile);			 
	while ( !in2.atEnd() )
	{		
		QString line = in2.readLine();    
		QStringList sname=line.split(' ');
		if(sname[0] == "expresseGeneLinkedtoTreeRing:")
		{
			
		}
		else if(sname[0] == "matchedGene_TreeRing_Expression:")
		{
		    for(int i=1; i<sname.size(); i++)
			{
				QString names=sname[i];		
				matchedGene_TreeRing_Expression.insert(names.toInt());
			}		
		}
		else
		{
			
			QString qstr=sname[1];
			pathIDSet.push_back(qstr);
		
			if(sname.size()>3)
			{
				QString rate=sname[3];			
			    matchedSet.push_back(rate.toInt());		
			}
			else
			{
			     matchedSet.push_back(0);
			}
		}
	}
	

	int count=0;
	//Color co;
	for(int layer=0; layer<_num_layers; ++layer)
	{
		count=0;
		for(int i=0; i<_ring[layer]._node_num, i<_ring[layer]._pos.size();++i)
	    {
			_ring[layer]._node_matched[count]=0;
			count++;
		}
	}	
	for(int layer=0; layer<_num_layers; ++layer)
	{
		//count=0;
		for(int i=0; i<_ring[layer]._node_num, i<_ring[layer]._pos.size();++i)
		{	
			
			QString num = GetPathID(layer,i);

			if(num.size()==0)
				continue;
			for(int k=0; k<pathIDSet.size(); k++)
	        {				
				if(pathIDSet[k]==num)
				{
					if(matchedSet[k]>0)
					{
					    matchedPathways.insert(num);
					}
					_ring[layer]._node_matched[i]=matchedSet[k];	
					break;
				}				
			}	
		}
	}
	//matchedGene_TreeRing_Expression.insert(j);		
}



void TreeRing::searchMatchedProtein(vector<vector<QString>> geneInfo, set<QString> pathID, vector<vector<QString>> pathInfo, set<QString> &matchedPathways, set<int> &matchedGene_TreeRing_Expression)
{	
	int count=0;
	//Color co;
	for(int layer=0; layer<_num_layers; ++layer)
	{
		count=0;
		for(int i=0; i<_ring[layer]._node_num, i<_ring[layer]._pos.size();++i)
	    {
			_ring[layer]._node_matched[count]=0;
			count++;
		}
	}	
	for(int layer=0; layer<_num_layers; ++layer)
	{
		//count=0;
		for(int i=0; i<_ring[layer]._node_num, i<_ring[layer]._pos.size();++i)
		{	
			string name=GetNodeName(layer,i);
	
			vector<vector<QString>> _proteinName;
			vector<set<vector<int>>> CompartmentContain;
			vector<vector<QString>> _compartmentName;
			vector<QRectF> _compartmentPos;

			QString tname;
			const char * ename;
			string sname;
			QString num = GetPathID(layer,i);

			if(num.size()==0)
				continue;
			
			bool flag=false;
			if(pathID.find(num)!=pathID.end())
			{
				for(int k=0; k<pathInfo.size(); k++)
				{				
				    if( pathInfo[k][0]== num)
					{					
						if(pathInfo[k][2].toInt()>0)
						{
							flag=true;
						    break;						
						}						
						
					}
				}				
			}

			if(flag)
			{
				QString fname = "data/Reactome_Pathway_Data/pathwayTable/" + num + "_";			
				tname = fname + "7protein.txt"; 
				sname= tname.toStdString();  ename=sname.c_str();
				PWDParser *pwdParser;
				int proteinNum = pwdParser->read7protein(ename, _proteinName, CompartmentContain, _compartmentName, _compartmentPos);
				bool flag=false;
				for(int k=1; k<=proteinNum; k++)
				{
					for(int j=0; j< geneInfo.size(); j++)
					{
						if(_proteinName[k][0].contains(geneInfo[j][1])||_proteinName[k][4].contains(geneInfo[j][1]))
						{
							matchedPathways.insert(num);
							_ring[layer]._node_matched[i]=1;	
							 //sharedExpressionProtein[m_bindPathwayID].insert(j);						 
							matchedGene_TreeRing_Expression.insert(j);		
							flag=true;
							break;
						}
					}	
					if(flag)
						break;
				}			
			}
			//count++;
		}
	}
}

void TreeRing::Render(QPainter *painter)
{
	//painter->setBrush( QColor( 0.8*255, 0.8*255, 0.8*255, 0.5*255 ) );	
	painter->save();
	int start,end;
	//all handle
	for(int layer=0; layer<1; layer++)//_num_layers
	{
		int nsize=_ring[layer]._pos.size();
		for(int i=0; i<_ring[layer]._node_num, i<_ring[layer]._pos.size();++i)
		{
           	vector<int> cids = getChildrenNodes(layer, i); //leaves
			int id1=cids[0];
			int clayer=_num_layers-1;							
			drawHandle(painter, clayer, i, (_ring[clayer]._pos[id1])[0], (_ring[clayer]._pos[id1])[1], MARKERCOLOR); 			
		}
	}	
	//highlighted handle
	if(handleLayer== 0 && handleIndex>=0)
	{
		vector<int> cids = getChildrenNodes(0, handleIndex); //leaves
		int id1=cids[0], id2=cids[cids.size()-1];
		int clayer=_num_layers-1;	
		drawHandle(painter, clayer, id1, (_ring[clayer]._pos[id1])[0], (_ring[clayer]._pos[id1])[1], MIDIUM_HIGHLIGHTCOLOR); 
		drawHandle(painter, clayer, id2, (_ring[clayer]._pos[id2])[2], (_ring[clayer]._pos[id2])[3], MIDIUM_HIGHLIGHTCOLOR); 
	}	

	vector<QColor> ringmark(4,QColor(0,0,0,0));
	for(int layer=0; layer<_num_layers; layer++) //_num_layers
	{
		painter->setBrush(Qt::NoBrush);
	    painter->setPen(Qt::NoPen);
		int nsize=_ring[layer]._pos.size();
		
       	for(int i=0; i<_ring[layer]._node_num, i<_ring[layer]._pos.size();++i)
		{
			vector<int> item(2,0);
			item[0]=layer; item[1]=i;
			//bool flag=false;
			//if(_ring[layer]._rateLimitNum[i]>0)
			{
				int isSideNode=0;
				if(layer!=0)
				{	
					int pid=_ring[layer]._parent[i];
					start=_ring[layer-1]._child_range[pid].first, end=_ring[layer-1]._child_range[pid].second;
					if(i==start  && _ring[layer]._node_num!=1)
						isSideNode=-1;
					else if(_ring[layer]._node_num!=1 && i==end)
						isSideNode=1;
				}	
				if(_ring[layer]._rateLimitNum[i]>0)
				    ringmark[1]=RATELIMITCOLOR;
				else
					ringmark[1]=QColor(255,255,255,255);

				//if(_ring[layer]._rateLimitNum[i]>0)
				ringmark[0]=QColor( _ring[layer]._color_by_crosstalk[i].r*255, _ring[layer]._color_by_crosstalk[i].g*255, _ring[layer]._color_by_crosstalk[i].b*255, 255);
				ringmark[2]=QColor( _ring[layer]._color_by_orthology[i].r*255, _ring[layer]._color_by_orthology[i].g*255, _ring[layer]._color_by_orthology[i].b*255, 255);
				ringmark[3]=QColor( _ring[layer]._color_by_expression[i].r*255, _ring[layer]._color_by_expression[i].g*255, _ring[layer]._color_by_expression[i].b*255, 255);
				
				if(paintNode(painter, layer, i, (_ring[layer]._pos[i])[0], (_ring[layer]._pos[i])[1], (_ring[layer]._pos[i])[2], (_ring[layer]._pos[i])[3], ringmark, _ring[layer]._node_num==1, isSideNode))
				{
					Point Ds=(_ring[layer]._pos[start])[1]-(_ring[layer]._pos[end])[3];
					float ds= pow(Ds.x,2)+pow(Ds.y,2);
					painter->setPen(EDGECOLOR);	
					if(ds>=RES)
					{
						if(isSideNode==-1)
						   painter->drawLine((_ring[layer]._pos[i])[0].x, (_ring[layer]._pos[i])[0].y, (_ring[layer]._pos[i])[1].x, (_ring[layer]._pos[i])[1].y);	
						else if(isSideNode==1)
						   painter->drawLine((_ring[layer]._pos[i])[2].x, (_ring[layer]._pos[i])[2].y, (_ring[layer]._pos[i])[3].x, (_ring[layer]._pos[i])[3].y);	
					}
				}
				else
				{
					painter->setPen(EDGECOLOR);	
					if(isSideNode==-1)
						 painter->drawLine((_ring[layer]._pos[i])[0].x, (_ring[layer]._pos[i])[0].y, (_ring[layer]._pos[i])[1].x, (_ring[layer]._pos[i])[1].y);	
					else if(isSideNode==1)
				         painter->drawLine((_ring[layer]._pos[i])[2].x, (_ring[layer]._pos[i])[2].y, (_ring[layer]._pos[i])[3].x, (_ring[layer]._pos[i])[3].y);						
				}	
				if(layer!=_num_layers-1 && !shrinked) //paint the separator to tell they belong to same toplevel pathway
				{
					int c1 = _ring[layer]._child_range[i].first, c2=_ring[layer]._child_range[i].second;
					int clayer=layer+1;
					painter->setPen(EDGECOLOR);						
					painter->drawLine((_ring[layer]._pos[i])[0].x, (_ring[layer]._pos[i])[0].y, (_ring[clayer]._pos[c1])[1].x, (_ring[clayer]._pos[c1])[1].y);	
					painter->drawLine((_ring[layer]._pos[i])[2].x, (_ring[layer]._pos[i])[2].y, (_ring[clayer]._pos[c2])[3].x, (_ring[clayer]._pos[c2])[3].y);						
				}
			}	
			
			//paint the node border again if clicked, selected, ..
			bool flag0=false, flag1=false;
			
			for( set< set< vector<int > > >::iterator it=itemSelected.begin(); it!=itemSelected.end();  it++)
			{
				set< vector<int > > iS = *it;
			    if(iS.find(item)!=iS.end())
			    {
					flag0=true;
					break;
				}
			}
		
			if( searched.find(item) != searched.end() )
			{
			    flag1=true;
			}						
			
			if(_itemClicked.find(item)!=_itemClicked.end() )
			{
				flag1=true;				
			}
			if(flag1 && !flag0)
			{
				QPen pen;
			    painter->setBrush(Qt::NoBrush);
				pen.setColor(MIDIUM_HIGHLIGHTCOLOR);
			    pen.setWidth(2);
				painter->setPen(pen);	
				paintNode_1(painter, layer, i, (_ring[layer]._pos[i])[0], (_ring[layer]._pos[i])[1], (_ring[layer]._pos[i])[2], (_ring[layer]._pos[i])[3],  _ring[layer]._node_num==1);			    				
			}
			
		}		
	}	

	//paint high light
	if(highlightLayer >=0 && highlightIndex >=0)
	{
		int layer=highlightLayer, i=highlightIndex;
		//render a light blue transparant node
		painter->setBrush( HIGHLIGHTCOLOR1 );		
		paintNode(painter, layer, i, (_ring[layer]._pos[i])[0], (_ring[layer]._pos[i])[1], (_ring[layer]._pos[i])[2], (_ring[layer]._pos[i])[3], ringmark, false, false, true);			    		
	}

	for(int i=0; i<respondLayer.size() && i<respondIndex.size(); i++)
	{
	    int layer=respondLayer[i], id=respondIndex[i];
		if( layer == highlightLayer && id == highlightIndex )
		{ 
		    continue;
		}
		//render a light blue transparant node
		painter->setBrush( HIGHLIGHTCOLOR1 );		
		paintNode(painter, layer, id, (_ring[layer]._pos[id])[0], (_ring[layer]._pos[id])[1], (_ring[layer]._pos[id])[2], (_ring[layer]._pos[id])[3], ringmark, false, false, true);			    			
	}

	painter->setPen(QColor(0,0,0,255));	
	painter->drawLine(0,1,0,1);
	painter->restore();
}


QPolygon TreeRing::getANode(int layer, int idx)
{	
	if(layer<0 || layer>=_ring.size() || idx>=_ring[layer]._pos.size() || idx<0)
	{
		QVector<QPoint> points;	   	
	    QPolygon empty(points);
		return empty;
	}
	Point p0=(_ring[layer]._pos[idx])[0], p1=(_ring[layer]._pos[idx])[1], p2=(_ring[layer]._pos[idx])[2], p3=(_ring[layer]._pos[idx])[3];
	float start=_ring[layer]._limits[idx].first, end=_ring[layer]._limits[idx].second;
	float angleSpan=start - end;
    //float angleSpan=_ring[layer]._limits[idx].first - _ring[layer]._limits[idx].second;
	int d=angleSpan<0?-1:1;
	if(_ring[layer]._limits[idx].first > _ring[layer]._limits[idx].second)
	{
		float temp=start;
		end = end +	M_PI*2;
		angleSpan = start - end;	    
	}
	angleSpan = fabs(angleSpan);
	float itv=0.1;
	int seg = angleSpan/itv;
	float angle; // = GetAngle(layer, i);//, angle2 = GetAngle(layer, (i+1)%nsize);			   
	float _width = GetRingWidth();	           
	float radius = _ring[layer]._radius[idx];	
	//if(angleSpan>0.2)
	
	vector<Point> v;
	Point p;
		
	v.push_back(p0);
	v.push_back(p1);
	angle=_ring[layer]._limits[idx].first;
	itv = angleSpan/seg;
	for(int i=0; i<seg-1; i++)
	{
		angle= angle + itv;
		p.x = radius * cosf(angle) + _centre.x;
		p.y = radius * sinf(angle) + _centre.y;
		v.push_back(p);

		p.x = (radius - _width) * cosf(angle) + _centre.x;
		p.y = (radius - _width) * sinf(angle) + _centre.y;
		v.push_back(p);
	}
	v.push_back(p2);
	v.push_back(p3);

	QVector<QPoint> points;	   
	for(int i=0; i<v.size(); i=i+2)
	{	
		points.push_back(QPoint(v[i+1].x, v[i+1].y));						
	}
	for(int i=v.size()-2; i>=0; i=i-2)
	{	
		points.push_back(QPoint(v[i].x, v[i].y));						
	}
	QPolygon whole(points);
	//painter->drawPolygon(whole);
	
	return whole;

}

void TreeRing::drawHandle(QPainter *painter, int layer, int idx, Point p0, Point p1, QColor color)
{
	QPointF end = QPointF(p0.x, p0.y), start = QPointF(p1.x, p1.y);
	QPointF dis1=(end-start)*0.3;
	
	float ds = sqrt(dis1.x()*dis1.x() + dis1.y()*dis1.y());
	int size = ds*0.5;
	if(size<5) size=5;
	else if(size>20) size=20;
	//painter->drawLine(end, end+dis0);
	drawATrapezoid(painter, end, end+dis1, 1, size, color);		
}

vector<QPointF> TreeRing::getATrapezoid(QPointF start, QPointF end, float size1, float size2)
{
	static const double Pi = 3.14159265358979323846264338327950288419717; 
    static double TwoPi = 2.0 * Pi;
	vector<QPointF> outPut;

    QLineF line(start, end);
    double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0)
		angle = TwoPi - angle;

	QPointF sourceArrowP1 = start + QPointF(sin(angle - Pi / 2.5) * size1,
													  cos(angle - Pi / 2.5) * size1);                                                  
	QPointF sourceArrowP2 = start + QPointF(sin(angle - Pi + Pi / 2.5) * size1,
													cos(angle - Pi + Pi / 2.5) * size1);   
	QPointF dis = (sourceArrowP1 + sourceArrowP2)/2;
	dis=start-dis;
	sourceArrowP1 = sourceArrowP1 + dis;
	sourceArrowP2 = sourceArrowP2 + dis;
				
	QPointF destArrowP1 = end + QPointF(sin(angle - Pi / 2.6) * size2, cos(angle - Pi / 2.6) * size2);
	QPointF destArrowP2 = end + QPointF(sin(angle - Pi + Pi / 2.6) * size2, cos(angle - Pi + Pi / 2.6) * size2);

	dis = (destArrowP1 + destArrowP2)/2;
	dis=end-dis;
	destArrowP1 = destArrowP1 + dis;
	destArrowP2 = destArrowP2 + dis;
	
	//painter->drawPolygon(QPolygonF() << destArrowP1 << sourceArrowP1 << sourceArrowP2 << destArrowP2);
    //painter->restore(); 
	outPut.push_back(destArrowP1);  outPut.push_back(sourceArrowP1);  outPut.push_back(sourceArrowP2);   outPut.push_back(destArrowP2);
	return outPut;
}

vector<QPointF> TreeRing::getATrapezoid(float angle, float angle1, float angle2, float radius, float width)
{
	vector<QPointF> outPut;	
	outPut.resize(4);
	
	if(angle1 > angle2)
	{
		angle1 = angle1- M_PI*2; 		
	}	

	float angleD = fabs(angle1 - angle2)/4;
	float angleD1 = fabs(angle1 - angle2)/4;

	int resolution = _ring[_num_layers-1]._node_num;
	if(resolution>0)
	{
		float itv = 3.1415/resolution;
		if(angleD>itv)
			angleD=itv;
		if(angleD1>itv)
			angleD1=itv;
	}	
	if(angleD>0.1)
		angleD=0.1;
	if(angleD1>0.1)
		angleD1=0.1;
	

	outPut[0] = QPointF(radius*cos(angle - angleD1) + _centre.x, radius*sin(angle - angleD1) + _centre.y);
	outPut[1] = QPointF(radius*cos(angle + angleD1) + _centre.x, radius*sin(angle + angleD1) + _centre.y);

	outPut[2] = QPointF((radius+width)*cos(angle-angleD) + _centre.x, (radius+width)*sin(angle-angleD) + _centre.y); 
	outPut[3] = QPointF((radius+width)*cos(angle+angleD) + _centre.x, (radius+width)*sin(angle+angleD) + _centre.y); 

	/*outPut[0] = QPointF(radius*cos(angle1) + _centre.x, radius*sin(angle1)+ _centre.y);
	outPut[1] = QPointF(radius*cos(angle2) + _centre.x, radius*sin(angle2)+ _centre.y);

	outPut[2] = QPointF((radius+width)*cos(angle-angleD) + _centre.x, (radius+width)*sin(angle-angleD) + _centre.y); //changed when switching inner ring out //(radius-width)*);
	outPut[3] = QPointF((radius+width)*cos(angle+angleD) + _centre.x, (radius+width)*sin(angle+angleD) + _centre.y);*/
	return outPut;
}


vector<QPointF> TreeRing::TrapezoidtoNode(QPointF p0, QPointF p1, QPointF p2, QPointF p3, float radius)
{	//
	vector<vector<QPointF>> P;
	vector<QPointF> PP1,PP2,V;
	PP1.resize(2);
	PP2.resize(2);

	PP1[0]=p0; PP1[1]=p1; //QPoint p1, QPoint p2, QPoint p3
	PP2[0]=p3; PP2[1]=p2;
	P.push_back(PP1); P.push_back(PP2);

	float dradius = sqrt( pow( p0.y()-_centre.y,2) + pow (p0.x()-_centre.x,2)) - sqrt( pow( p3.y()-_centre.y,2) + pow (p3.x()-_centre.x,2));
	float Radius[2];
	Radius[0]=radius;
	Radius[1]=radius-dradius;

	for(int i=0; i<2; i++)
	{		
		float angle1, angle2;	
		float start, end;
		float angleSpan, radius=Radius[i];
		int d;
		float tSpan;
		p0=P[i][0]; p1=P[i][1];
		angle1 = atan2(p0.y()-_centre.y, p0.x()- _centre.x);
		angle2 = atan2(p1.y()-_centre.y, p1.x()-_centre.x);
		

		start = angle1, end=angle2;
		angleSpan = start - end;
		d = angleSpan<0?-1:1;

		if(angle1 > angle2)
		{
			float temp=start;
			end = end +	M_PI*2;
			angleSpan = start - end;	    
		}
		angleSpan = fabs(angleSpan);
		float itv=0.1;
		int seg = angleSpan/itv;
		float angle; // = GetAngle(layer, i);//, angle2 = GetAngle(layer, (i+1)%nsize);			   
		//float _width = GetRingWidth();	           
		//float radius = _ring[layer]._radius[idx];	

		vector<QPointF> v;
		QPointF p;

		v.push_back(p0);	
		angle=start;//_ring[layer]._limits[idx].first;		
		itv = angleSpan/seg;
		for(int i=0; i<seg-1; i++)
		{
			angle= angle + itv;
			p.setX(radius * cosf(angle) + _centre.x);
			p.setY(radius * sinf(angle) + _centre.y);
			v.push_back(p);
			/*p.setX(innerRadius * cosf(angle) + _centre.x);
			p.setY(innerRadius * sinf(angle) + _centre.y);
			v.push_back(p);*/
		}
		v.push_back(p1);
		//v.push_back(p3);
		if(i==0)
		{
			for(int j=0; j<v.size(); j++)
			{	
				V.push_back(v[j]);						
			}	
		}
		else
		{
			for(int j=v.size()-1; j>=0; j--)
			{	
				V.push_back(v[j]);						
			}	
		}
	}
	//QPolygon whole(points);
	//painter->drawPolygon(whole);
	return V;
}


void TreeRing::drawATrapezoid(QPainter *painter, QPointF start, QPointF end, float size1, float size2, QColor color)
{
	painter->save(); 
	
	painter->setPen(color);
	painter->setBrush(color);

	static const double Pi = 3.14159265358979323846264338327950288419717; 
    static double TwoPi = 2.0 * Pi;

    QLineF line(start, end);
    double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0)
		angle = TwoPi - angle;

	QPointF sourceArrowP1 = start + QPointF(sin(angle - Pi / 2.5) * size1,
													  cos(angle - Pi / 2.5) * size1);                                                  
	QPointF sourceArrowP2 = start + QPointF(sin(angle - Pi + Pi / 2.5) * size1,
													cos(angle - Pi + Pi / 2.5) * size1);   
	QPointF dis = (sourceArrowP1 + sourceArrowP2)/2;
	dis=start-dis;
	sourceArrowP1 = sourceArrowP1 + dis;
	sourceArrowP2 = sourceArrowP2 + dis;
				
	QPointF destArrowP1 = end + QPointF(sin(angle - Pi / 2.6) * size2, cos(angle - Pi / 2.6) * size2);
	QPointF destArrowP2 = end + QPointF(sin(angle - Pi + Pi / 2.6) * size2, cos(angle - Pi + Pi / 2.6) * size2);

	dis = (destArrowP1 + destArrowP2)/2;
	dis=end-dis;
	destArrowP1 = destArrowP1 + dis;
	destArrowP2 = destArrowP2 + dis;
	
	painter->drawPolygon(QPolygonF() << destArrowP1 << sourceArrowP1 << sourceArrowP2 << destArrowP2);
    painter->restore(); 
}

bool TreeRing::paintNode(QPainter *painter, int layer, int idx, Point p0, Point p1, Point p2, Point p3, vector<QColor> rlNode, bool onlyNode, int sideNode, bool highLightLayer)
{	//return if it needs to draw sidenode
	//rlNode[0]: rate-limit, rlNode[1]: orthology, rlNode[2]: expression
	painter->save();
	float start=_ring[layer]._limits[idx].first, end=_ring[layer]._limits[idx].second;
	float angleSpan=start - end;
	int d=angleSpan<0?-1:1;
	float tSpan;
	if(onlyNode)
	{
		angleSpan = 2.*M_PI;
		end = start + 2.*M_PI;
	}
	else
	{
		if(_ring[layer]._limits[idx].first > _ring[layer]._limits[idx].second)
		{
			float temp=start;
			end = end +	M_PI*2;
			angleSpan = start - end;	    
		}
	}
	angleSpan = fabs(angleSpan);
	float itv=0.1;
	int seg = angleSpan/itv;
	float angle; // = GetAngle(layer, i);//, angle2 = GetAngle(layer, (i+1)%nsize);			   
	float _width = GetRingWidth();	           
	float radius = _ring[layer]._radius[idx];	
	
	vector<Point> v;
	vector<vector<Point>> v1;
	v1.resize(rlNode.size());
	Point p;
	//rlNode	
	v.push_back(p0);
	v.push_back(p1);
	angle=start;//_ring[layer]._limits[idx].first;
	itv = angleSpan/seg;
	for(int i=0; i<seg-1; i++)
	{
		angle= angle + itv;
		p.x = radius * cosf(angle) + _centre.x;
		p.y = radius * sinf(angle) + _centre.y;
		v.push_back(p);

		p.x = (radius - _width) * cosf(angle) + _centre.x;
		p.y = (radius - _width) * sinf(angle) + _centre.y;
		v.push_back(p);
	}
	v.push_back(p2);
	v.push_back(p3);
	float ds;

	if(!highLightLayer)
	{
		float ratio = 0.25;
		for(int k=0; k<rlNode.size(); k++)//rlNode.size()
		{
			//if(rlNode[k]!=QColor(0,0,0,0))//rate limit
			{
				Point p4, p5, p6, p7;
				p4.x= p1.x + (p0.x - p1.x)*ratio*(k+1);
				p4.y= p1.y + (p0.y - p1.y)*ratio*(k+1);

				p5.x= p3.x + (p2.x - p3.x)*ratio*(k+1);
				p5.y= p3.y + (p2.y - p3.y)*ratio*(k+1);

				p6.x= p1.x + (p0.x - p1.x)*ratio*(k);
				p6.y= p1.y + (p0.y - p1.y)*ratio*(k);

				p7.x= p3.x + (p2.x - p3.x)*ratio*(k);
				p7.y= p3.y + (p2.y - p3.y)*ratio*(k);

				v1[k].push_back(p4);
				v1[k].push_back(p6);
				angle=start;//_ring[layer]._limits[idx].first;		
				for(int i=0; i<seg-1; i++)
				{
					angle= angle + itv;
					p.x = (radius- _width*(1-ratio*(k+1))) * cosf(angle) + _centre.x;
					p.y = (radius- _width*(1-ratio*(k+1))) * sinf(angle) + _centre.y;
					v1[k].push_back(p);

					p.x = (radius - _width*(1-ratio*k)) * cosf(angle) + _centre.x;
					p.y = (radius - _width*(1-ratio*k)) * sinf(angle) + _centre.y;
					v1[k].push_back(p);
				}
				v1[k].push_back(p5);
				v1[k].push_back(p7);	
			}
		}

		Point Ds=p1-p3;
		ds= pow(Ds.x,2)+pow(Ds.y,2);

		//if(ds<RES)
		{
			painter->setPen(EDGECOLOR);
			for(int i=0; i<v.size()-2; i=i+2)
			{	
				//points.push_back(QPoint(v[i+1].x, v[i+1].y));		
				 painter->drawLine(v[i+1].x, v[i+1].y, v[i+3].x, v[i+3].y);	
			}
			for(int i=v.size()-2; i>=2; i=i-2)
			{	
				//points.push_back(QPoint(v[i].x, v[i].y));				
				painter->drawLine(v[i].x, v[i].y, v[i-2].x, v[i-2].y);	
			}		
			if(ds<RES)
				painter->setPen(Qt::NoPen);	 		
		}
	}
	QVector<QPoint> points;	   
	QVector<QVector<QPoint>> points1;
	points1.resize(rlNode.size());
	for(int i=0; i<v.size(); i=i+2)
	{	
		points.push_back(QPoint(v[i+1].x, v[i+1].y));						
	}
	for(int i=v.size()-2; i>=0; i=i-2)
	{	
		points.push_back(QPoint(v[i].x, v[i].y));						
	}	
	QPolygon whole(points);
	if(highLightLayer)
	   painter->setPen(Qt::NoPen);
	
	painter->drawPolygon(whole);		
	if(!highLightLayer)
	{
		for(int k=0; k<rlNode.size(); k++) //rlNode.size()
		{
			//if(rlNode[k]==QColor(0,0,0,0))
			//rlNode[k]=QColor(127,127,127,255);
			{
				painter->setPen(Qt::NoPen);
				painter->setBrush(rlNode[k]);
				for(int i=0; i<v1[k].size(); i=i+2)
				{	
					points1[k].push_back(QPoint(v1[k][i+1].x, v1[k][i+1].y));						
				}
				for(int i=v1[k].size()-2; i>=0; i=i-2)
				{	
					points1[k].push_back(QPoint(v1[k][i].x, v1[k][i].y));						
				}
				QPolygon mark(points1[k]);
				painter->drawPolygon(mark);	
			}
		}
		painter->setBrush(Qt::NoBrush);
		if(ds>RES)
			painter->setPen(EDGECOLOR);	
		painter->drawPolygon(whole);	
	}
	painter->restore();
	return ds<RES && sideNode;
}

void TreeRing::paintNode_1(QPainter *painter, int layer, int idx, Point p0, Point p1, Point p2, Point p3, bool onlyNode)
{	//return if it needs to draw sidenode
	painter->save();
	float start=_ring[layer]._limits[idx].first, end=_ring[layer]._limits[idx].second;
	float angleSpan=start - end;
	int d=angleSpan<0?-1:1;
	float tSpan;
	if(onlyNode)
	{
		angleSpan = 2.*M_PI;
		end = start + 2.*M_PI;
	}
	else
	{
		if(_ring[layer]._limits[idx].first > _ring[layer]._limits[idx].second)
		{
			float temp=start;
			end = end +	M_PI*2;
			angleSpan = start - end;	    
		}
	}
	angleSpan = fabs(angleSpan);
	float itv=0.1;
	int seg = angleSpan/itv;
	float angle; // = GetAngle(layer, i);//, angle2 = GetAngle(layer, (i+1)%nsize);			   
	float _width = GetRingWidth();	           
	float radius = _ring[layer]._radius[idx];	
	
	vector<Point> v,v1;
	Point p;
	//rlNode	
	v.push_back(p0);
	v.push_back(p1);
	angle=start;//_ring[layer]._limits[idx].first;
	itv = angleSpan/seg;
	for(int i=0; i<seg-1; i++)
	{
		angle= angle + itv;
		p.x = radius * cosf(angle) + _centre.x;
		p.y = radius * sinf(angle) + _centre.y;
		v.push_back(p);

		p.x = (radius - _width) * cosf(angle) + _centre.x;
		p.y = (radius - _width) * sinf(angle) + _centre.y;
		v.push_back(p);
	}
	v.push_back(p2);
	v.push_back(p3);
	
	QVector<QPoint> points;
	for(int i=0; i<v.size(); i=i+2)
	{	
		points.push_back(QPoint(v[i+1].x, v[i+1].y));						
	}
	for(int i=v.size()-2; i>=0; i=i-2)
	{	
		points.push_back(QPoint(v[i].x, v[i].y));						
	}	
	QPolygon mark(points);
	painter->drawPolygon(mark);		
	painter->restore();
	
}

vector<Point> TreeRing::getNode(int layer, int idx, Point p0, Point p1, Point p2, Point p3, int Half)
{	//Half: 0, lower half, 1: upper half, 2: two half 
	
	float start=_ring[layer]._limits[idx].first, end=_ring[layer]._limits[idx].second;
	float angleSpan=start - end;
	int d=angleSpan<0?-1:1;
	float tSpan;
	if(_ring[layer]._limits[idx].first > _ring[layer]._limits[idx].second)
	{
		float temp=start;
		end = end +	M_PI*2;
		angleSpan = start - end;	    
	}
	angleSpan = fabs(angleSpan);
	float itv=0.1;
	int seg = angleSpan/itv;
	float angle; // = GetAngle(layer, i);//, angle2 = GetAngle(layer, (i+1)%nsize);			   
	float _width = GetRingWidth();	           
	float radius = _ring[layer]._radius[idx];	
	
	vector<Point> v;
	Point p;
		
	if(Half==2)
	{
		v.push_back(p0);
	    v.push_back(p1);
		angle=start;//_ring[layer]._limits[idx].first;
		itv = angleSpan/seg;
		for(int i=0; i<seg-1; i++)
		{
			angle= angle + itv;
			p.x = radius * cosf(angle) + _centre.x;
			p.y = radius * sinf(angle) + _centre.y;
			v.push_back(p);

			p.x = (radius - _width) * cosf(angle) + _centre.x;
			p.y = (radius - _width) * sinf(angle) + _centre.y;
			v.push_back(p);
		}
		v.push_back(p2);
		v.push_back(p3);
	}
	else if(Half==1)
	{
		v.push_back(p0);
	    //v.push_back(p1);
		angle=start;//_ring[layer]._limits[idx].first;
		itv = angleSpan/seg;
		for(int i=0; i<seg-1; i++)
		{
			angle= angle + itv;
			p.x = radius * cosf(angle) + _centre.x;
			p.y = radius * sinf(angle) + _centre.y;
			v.push_back(p);

			/*p.x = (radius - _width) * cosf(angle) + _centre.x;
			p.y = (radius - _width) * sinf(angle) + _centre.y;
			v.push_back(p);*/
		}
		v.push_back(p2);
		//v.push_back(p3);
	}
	else if(Half==0)
	{
		v.push_back(p1);	    
		angle=start;//_ring[layer]._limits[idx].first;
		itv = angleSpan/seg;
		for(int i=0; i<seg-1; i++)
		{
			angle= angle + itv;
			/*p.x = radius * cosf(angle) + _centre.x;
			p.y = radius * sinf(angle) + _centre.y;
			v.push_back(p);*/
			p.x = (radius - _width) * cosf(angle) + _centre.x;
			p.y = (radius - _width) * sinf(angle) + _centre.y;
			v.push_back(p);
		}		
		v.push_back(p3);
	}
	return v;
}

QPolygon TreeRing::getSegPoint(int topLayer, int bottomLayer, int idx1, int idx2, float dx, float dy, int &cornerID1, int &cornerID2)
{	
	//int layer2 = GetNrLayers()-1;		
	vector<Point> V,V1;
	int layer=topLayer;
	int layer1=bottomLayer; //output.size()-1;
	int nodeNum = _ring[layer]._node_num;
	if(idx2<idx1)
		idx2 = idx2 + nodeNum;

	for(int i=idx1; i<=idx2; i++)
	{
		int id=i;
		if(id>=nodeNum)
			id=id-nodeNum;			
		vector<Point> v= getNode(layer, id, (_ring[layer]._pos[id])[0], (_ring[layer]._pos[id])[1], (_ring[layer]._pos[id])[2], (_ring[layer]._pos[id])[3], 0);	//0: get inner boarder of node			
		for(int i=0; i<v.size(); i++)
		{	
			V.push_back(v[i]);						
		}
       
		if(bottomLayer!=topLayer)
	    {
			 vector<vector<int>> output;
			findAllDescendantNodes(layer, id, output);			
			if(layer1<output.size()-1)
				layer1=output.size()-1;;
			for(int j = 0; j<output[layer1].size(); j++)
			{
				int id1=output[layer1][j];
				vector<Point> v1 = getNode(layer1, id1, (_ring[layer1]._pos[id1])[0], (_ring[layer1]._pos[id1])[1], (_ring[layer1]._pos[id1])[2], (_ring[layer1]._pos[id1])[3], 1);	//1: get outer boarder of node	
				for(int i=0; i<v1.size(); i++)
				{	
					V1.push_back(v1[i]);						
				}
				
			}
		}
		else
		{
			vector<Point> v1 = getNode(layer, id, (_ring[layer]._pos[id])[0], (_ring[layer]._pos[id])[1], (_ring[layer]._pos[id])[2], (_ring[layer]._pos[id])[3], 1);		//1: get outer boarder of node				
			for(int i=0; i<v1.size(); i++)
			{	
				V1.push_back(v1[i]);						
			}		
		}

	}	
	QVector<QPoint> points;	   
	for(int i=0; i<V.size(); i++)
	{	
		points.push_back(QPoint(V[i].x+dx, V[i].y+dy));						
	}
	//cornerID = V.size()-1; //before put inner ring out

    cornerID1 = points.size();
	for(int i=V1.size()-1; i>=0; i=i-1)
	{	
		points.push_back(QPoint(V1[i].x+dx, V1[i].y+dy));						
	}
	cornerID2 = points.size()-1;
	if(cornerID2>=70)
		cornerID2=cornerID2;	
	QPolygon whole(points);
	return whole;
}


void TreeRing::ChangePointOrder(vector<Point> &out, vector<Point> &in)
{
	int i, j;
	for(i = 0, j = 0; j < in.size() - 1; j +=2, ++i)
		out[i] = in[j];
	for(--j; j > 0; j -= 2, ++i)
		out[i] = in[j];

	out[i]  =in[0]; //多加了一个顶点，注意
}

vector<int> TreeRing::CheckPointInTreeRing(float x, float y)
{
	vector<int> output(2,-1);
	Point &o = _centre;
	float dist = sqrtf((x - o.x)*(x - o.x) + (y - o.y)*(y - o.y));	
	{
		//this->highlightLayer=-1;
		//this->highlightIndex=-1;
		//highlightState = 0;   		
	}
	for(int layer=0; layer<_num_layers; ++layer)
	{
		Ring &ring = _ring[layer];
		for(int i=0; i<ring._node_num; ++i)
		{
			if(dist >= ring._radius[i] - _width && dist <= ring._radius[i])
			{
				float alpha;
				alpha = acosf( (x - o.x) / dist);
				if(y < o.y)
					alpha = 2 * M_PI - alpha;

				if(alpha >=ring._limits[i].first && alpha <= ring._limits[i].second)
				{
					//this->highlightIndex = i;
					cout<<"index is:"<<i<<endl;
					output[0]=layer;
					output[1]=i;
					//highlightState = HL_NODE;
					return output;
				}
			}
		}
	}
	return output;
}


vector<int> TreeRing::CheckHandleInTreeRing(float x, float y)
{
	vector<int> output(3,-1);
	Point &o = _centre;
	float dist = sqrtf((x - o.x)*(x - o.x) + (y - o.y)*(y - o.y));	
	
	/*for(int layer=0; layer<_num_layers; ++layer)
	{
		Ring &ring = _ring[layer];
		for(int i=0; i<ring._node_num; ++i)
		{
			if(dist >= ring._radius[i] - _width && dist <= ring._radius[i])
			{
				float alpha;
				alpha = acosf( (x - o.x) / dist);
				if(y < o.y)
					alpha = 2 * M_PI - alpha;

				if(alpha >=ring._limits[i].first && alpha <= ring._limits[i].second)
				{
					//this->highlightIndex = i;
					cout<<"index is:"<<i<<endl;
					output[0]=layer;
					output[1]=i;
					//highlightState = HL_NODE;
					return output;
				}
			}
		}
	}*/

	//for(int layer=0; layer<1; ++layer)//change when switching inner ring out
	//for(int layer=0; layer>1; layer--)
	int layer=_num_layers-1;
	{
		Ring &ring = _ring[layer];
		float w=_width*HANDLERANGE;
		for(int i=0; i<ring._node_num; ++i)
		{		
			if(dist >= ring._radius[i] - w && dist <= ring._radius[i]+w*3)
			{
				float alpha;
				alpha = acosf( (x - o.x) / dist);
				if(y < o.y)
					alpha = 2 * M_PI - alpha;

				float angle1= ring._limits[i].first, angle2=ring._limits[i].second;
				if(angle1==angle2)
				{
					if( fabs(alpha-angle1)<= 0.03)
					{
					    output[0]=layer;
					    output[1]=i;	
						/*if(alpha < angle1)
					       output[2]=0;	
						else */
							output[2]=2;
						return output;
					}
					else if(fabs(alpha-angle1)> (2.*M_PI-0.03))
					{
					    output[0]=layer;
					    output[1]=i;
						/*if(alpha > angle1)
					       output[2]=0;	
						else*/ output[2]=2;
						return output;
					}
				}				
				else if((angle1>angle2 && (alpha >=angle1 || alpha <= angle2) ) || ( angle1<angle2 && alpha >=angle1 && alpha <= angle2) )
				{
					output[0]=layer;
					output[1]=i;	
					if(fabs(alpha - ring._limits[i].first) < fabs(alpha - ring._limits[i].second))
					    output[2]=0;	
					else 
						output[2]=1;	
					return output;
				}
			}
		}	
	}
	return output;
}


void TreeRing::CheckPointInNode(float x, float y)
{//check hover over treering node and treering handle

	Point &o = _centre;
	float dist = sqrtf((x - o.x)*(x - o.x) + (y - o.y)*(y - o.y));
	//if(dist > _radius || dist < _radius - _num_layers*_width)
	{
		this->highlightLayer=-1;
		this->highlightIndex=-1;
		handleLayer = -1; handleIndex = -1;
		highlightState = 0;   
		//return;
	}
	
#if 1
	for(int layer=0; layer<_num_layers; ++layer)
	{
		Ring &ring = _ring[layer];

		for(int i=0; i<ring._node_num; ++i)
		{
			if(dist >= ring._radius[i] - _width && dist <= ring._radius[i])
			{
				float alpha;
				float first, second;
				alpha = acosf( (x - o.x) / dist);
				if(y < o.y)
					alpha = 2 * M_PI - alpha;

				first=ring._limits[i].first; second=ring._limits[i].second;
				if( first > second )
				{
				     second = 2 * M_PI + second;
				}
				////changed when switching inner ring out
				if( (first == second && layer==0 && ring._node_num==1) || (first == second && (fabs(first-alpha)<0.03 || fabs(first-alpha)>2.*M_PI-0.3)) || (alpha >= first && alpha <= second) )				
				{
					this->highlightLayer = layer; 
					this->highlightIndex = i;
					
					cout<<"index is:"<<i<<endl;
					highlightState = HL_NODE;
					break;
				}
			}
		}
		//for(int layer=0; layer<1; ++layer)////changed when switching inner ring out
		//for(int layer=_num_layers-1; layer<_num_layers; ++layer)
		if(layer==_num_layers-1)
	    {
			int topLayer=0;
			Ring &ring = _ring[topLayer];
			float w=_width*HANDLERANGE;
			for(int i=0; i<ring._node_num; ++i)
			{		
				if(dist >= _ring[layer]._radius[i] - w && dist <= _ring[layer]._radius[i]+w*3)
				{
					float alpha;
					alpha = acosf( (x - o.x) / dist);
					if(y < o.y)
						alpha = 2 * M_PI - alpha;

					float first, second;
					first=ring._limits[i].first;  second=ring._limits[i].second;
					if( first > second )
					{
						 second = 2 * M_PI + second;
					}					
					if( (first == second && layer==_num_layers-1 && ring._node_num==1) || (first == second && (fabs(first-alpha)<0.03 || fabs(first-alpha)>2.*M_PI-0.3)) || (alpha >= first && alpha <= second) )
					{
						this->handleLayer = topLayer; 
						this->handleIndex = i;
						this->handlet = fabs(alpha-ring._limits[i].first) < fabs(alpha-ring._limits[i].second)? 0:1;
						if(first == second && layer==_num_layers-1 && ring._node_num==1) 
							this->handlet = 2;
						
						highlightState = HL_HANDLE;						
						return;
					}
				}
			}	
		}
	}

#endif


#if 0

	for(int layer=0; layer<_num_layers; ++layer)
	{
		//check every node of this layer
		for(int index=0; index<_ring[layer]._node_num; ++index)
		{
			float node_radius = _ring[layer]._radius[index];

			if( !(dist >= node_radius - _width && dist <= node_radius) )
				continue;

			int n = (_ring[layer]._pos[index]).size();
			vector<Point> p;
			p.resize(n+1);

			ChangePointOrder(p, _ring[layer]._pos[index]);

			//--------------弧长法检测点在多边形内---------------------------------------------
			int i;
			for(vector<Point>::iterator i_p = p.begin(); i_p != p.end(); ++i_p)
			{
				i_p->x -= x;
				i_p->y -= y;
			}

			int t1, t2, sum;
			float f;

			t1 = p[0].x >= 0 ? (p[0].y >= 0 ? 0 : 3) : (p[0].y >= 0 ? 1 : 2); //计算象限 


			for(sum = 0,i = 1; i <= n; i++)
			{
				if(!p[i].x && !p[i].y )  //被测点为多边形顶点
					break; 

				f = p[i].y * p[i - 1].x - p[i].x * p[i-1].y;  //计算叉积           

				if(f < 0.00001f && p[i-1].x * p[i].x <= 0 && p[i - 1].y * p[i].y <= 0) //被测点在多边形边上
					break;

				t2 = p[i].x >= 0 ? (p[i].y >= 0 ? 0 : 3) : (p[i].y >= 0 ? 1 : 2);   //计算象限

				if(t2==(t1+1)%4)     //情况1
					sum+=1;
				else if(t2==(t1+3)%4)     //情况2
					sum-=1;
				else if(t2==(t1+2)%4)     //情况3
				{
					if(f>0) 
						sum+=2 ; 
					else
						sum-=2;
				} 
				t1=t2;
			}

			if(i <= n || sum)    //被测点在多边形边上或者在多边形内部
			{
				this->highlightLayer = layer; 
				this->highlightIndex = index;
				cout<<"index is:"<<index<<endl;
				highlightState = HL_NODE;
				return;
			}


			//--------------弧长法检测点在多边形内---------------------------------------------
		}
	}//end for
#endif
}

void TreeRing::CalculateNrChildren()
{
	
	for(int layer=0; layer<_num_layers-1; ++layer)
	{
		int id=0;
		for(int i=0; i<_ring[layer]._node_num; ++i)
		{
			int sum=0, count=0;
			//while(sum < _ring[layer]._node_size[i] && id + count < _ring[layer+1]._node_size.size())
			while(sum < _ring[layer]._node_size[i] && id + count < _ring[layer+1]._node_size.size())
			{
				sum += _ring[layer + 1]._node_size[id + count];
				count++;
			}
			if(count==0)
			    count=count;

			_ring[layer]._nr_children.push_back(count);
			pair<int, int> p;
			p.first = id;
			id += count;
			p.second = id-1;
			_ring[layer]._child_range.push_back(p);
		}
	}
	for(int layer=0; layer<_num_layers-1; ++layer)
	{
		int sum=0;
		for(int i=0; i<_ring[layer]._node_num; ++i)
		{
			sum = sum + _ring[layer]._nr_children[i];
		}
		if(sum!=_ring[layer+1]._node_num)
		   sum=sum;
	}
}

//Paper "Radial Tree Graph Drawing Algorithm for Representing Large Hierarchies"
void TreeRing::RadialLayout()
{//compute ring._angle, ring.limits, ring._radial_pos, 	ring.parent
	for(int layer=0; layer<_num_layers; ++layer)
	{
		Ring &ring = _ring[layer];
		if(0 == layer)
		{
			ring._parent.resize(ring._node_num);
			int node_size = 0;
			for(int j=0; j<ring._node_num; ++j)
			{
				node_size += ring._node_size[j];
				_ring[layer]._parent[j]=-1;
			}
			double unit_angle = 2.*M_PI / node_size;// *0.75;
			double compensate = 2.*M_PI / ring._node_num*0;// * 0.25;
			double cur_angle = 0.;
			for(int i=0; i<ring._node_num; ++i) //Loop through all nodes at this level
			{
				double angle = ring._node_size[i] * unit_angle;
				Point p;
				//Calculate x, y position
				p.x = cosf(cur_angle) * ring._ring_radial_radius;
				p.y = sinf(cur_angle) * ring._ring_radial_radius;
				ring._radial_pos.push_back(p);
				ring._angle.push_back(cur_angle);
				cur_angle += (angle + compensate);
			}
			for(int k=0; k<ring._node_num; ++k) //Loop through all nodes at this level
			{
				//If this node has children, calculate bisector limits and tangent limits for the node
				int i= k % ring._node_num;
				if(_ring[layer]._nr_children[i] > 0)
				{
					pair<float, float> bisector, tangent, limits;					
					/*int im1 = i - 1 < 0 ? ring._node_num - 1 : i - 1;
					int ip1 = (i+1)%ring._node_num;

					//Calculate bisector limit---------------------------------------------------------------
					if(ring._angle[i] < ring._angle[im1]) //Angle i < angle i - 1
						bisector.first = ring._angle[i] - (ring._angle[i] - ring._angle[im1] + 2.*M_PI) / 2;
					else
						bisector.first = ring._angle[i] - (ring._angle[i] - ring._angle[im1]) / 2;

					if(ring._angle[i] > ring._angle[ip1]) //Angle i > angle i + 1
						bisector.second = ring._angle[i] + (ring._angle[ip1] - ring._angle[i] + 2.*M_PI) / 2;
					else 
						bisector.second = ring._angle[i] + (ring._angle[ip1] - ring._angle[i]) / 2;
					//Calculate bisector limit---------------------------------------------------------------

					float tan_angle = acosf(ring._ring_radial_radius / _ring[layer+1]._ring_radial_radius);
					tangent.first = ring._angle[i] - tan_angle;
					tangent.second = ring._angle[i] +  tan_angle;*/

					//Calculate limits
					limits.first = ring._angle[i]; //max(bisector.first, tangent.first); //keqin modified
					limits.second = ring._angle[(i+1)%ring._node_num];//-unit_angle/4; //min(bisector.second, tangent.second);
					if(limits.second<=0)
						limits.second = 2.*M_PI - limits.second;

					if(limits.second<limits.first)
						 limits.second=limits.first;
										
					ring._limits.push_back(limits);
				}
			}
		}
		else
		{
			Ring &parent = _ring[layer-1];
			ring._parent.resize(ring._node_num);
			for(int i=0; i<parent._node_num; ++i)
			{
				for(int j=parent._child_range[i].first; j<=parent._child_range[i].second; j++)
				{
				    ring._parent[j]=i;		
				}
			}						
			int cur = 0;
			for(int i=0; i<parent._node_num && i<parent._limits.size(); ++i)
			{
				int nr_children = parent._nr_children[i];
				if(0 == nr_children)
					continue;

				int node_size = 0;
				for(int j = cur; j < cur + nr_children; ++j)
					node_size += ring._node_size[j];

				float unit_angle = (parent._limits[i].second - parent._limits[i].first) / node_size;
				
				//Loop through all nodes for this parent
				float cur_angle = parent._limits[i].first;
				for(int j=cur; j<cur+nr_children; ++j)
				{
					float angle = ring._node_size[j]*unit_angle;
					Point p;
					
					//Calculate x,y position
					p.x = cosf(cur_angle) * ring._ring_radial_radius;
					p.y = sinf(cur_angle) * ring._ring_radial_radius;

					ring._radial_pos.push_back(p);
					ring._angle.push_back(cur_angle);

					cur_angle += angle;
				}
				cur+=nr_children;
			}
			cur = 0;
			for(int i=0; i<parent._node_num; ++i)
			{
				int nr_children = parent._nr_children[i];
				//double span = getAngleSpan(parent._limits[i].first, parent._limits[i].second); //( parent._angle[i],  parent._angle[ (i+1) % parent._node_num]);
				//double unit_angle = span / nr_children; 
                //Loop through all nodes for this parent
				for(int j=cur; j<cur+nr_children && j<ring._angle.size(); ++j)
				{
					pair<float, float> bisector, tangent, limits;
	
					int jm1 = j - 1 < 0 ? ring._angle.size() - 1 : j - 1;
					int jp1 = (j+1)%ring._angle.size();

					//If this child node has children, calculate bisector and tangent limits
					if(ring._nr_children.empty() != true && ring._nr_children[j] > 0)
					{
						limits.first = ring._angle[j]; //max(bisector.first, tangent.first); //keqin modified
						if(j+1==cur+nr_children)
						    limits.second = parent._limits[i].second; //min(bisector.second, tangent.second);
						else 
							limits.second = ring._angle[(j+1)]; //min(bisector.second, tangent.second);

						if( nr_children == 1)
						{
							limits.first = parent._limits[i].first;
							limits.second = parent._limits[i].second;
						}
						ring._limits.push_back(limits);
					}
					else //The last layer
					{
						limits.first = ring._angle[j] - fabs(ring._angle[j] - ring._angle[jm1]) / 2;
						limits.second = ring._angle[j] + fabs(ring._angle[j] - ring._angle[jp1]) / 2;

						limits.first = max(limits.first, parent._limits[i].first);
						limits.second = min(limits.second, parent._limits[i].second);

						limits.first = ring._angle[j]; //max(bisector.first, tangent.first); //keqin modified

					    if(j+1==cur+nr_children)
						    limits.second = parent._limits[i].second; //min(bisector.second, tangent.second);
						else 
							limits.second = ring._angle[(j+1)]; //min(bisector.second, tangent.second);
						
						if( nr_children == 1)
						{
							limits.first = parent._limits[i].first;
							limits.second = parent._limits[i].second;
						}
						/*if( nr_children == 1)
						{
							limits.first = parent._limits[i].first;
							limits.second = parent._limits[i].second;
						}*/
						ring._limits.push_back(limits);						
					}
				}
				cur += nr_children;
			}
		}
		
	}
}

void TreeRing::reRadialLayout(vector<double> preSpans)
{//used with spanAnode
	//span all the children with this highlighted node
	int minus1 = 0, minus2 =0, start=0; //span preserved for fan highlighted handle 
	float angleSpan=2.*M_PI, preAngleSpan=0, preSpan=0; //preAngleSpan: previous int newSize1=0;
	if( highlightState == HL_HANDLE && handleLayer >-1 )
	{
		 minus1 = _ring[handleLayer]._node_size[handleIndex];		 
		 minus2 =  1; //
		 //start = handlet==1? handleIndex-1 : handleIndex;
		 start = handleIndex;		 
		 //if(start<0)
		 //	 start = _ring[handleLayer]._node_num-1; 
		 
		 if(handleLayer == _num_layers-1 &&_ring[handleLayer]._node_num==1)//changed when switching inner ring out
			 angleSpan = 0;
		 else 
			 angleSpan = angleSpan - getAngleSpan( _ring[handleLayer]._limits[handleIndex].first,  _ring[handleLayer]._limits[handleIndex].second);

		 preSpan=preSpans[handleIndex];
	}
	

	int newSize1=0;
	for(int layer=0; layer<_num_layers; ++layer)
	{
		Ring &ring = _ring[layer];
		if(0 == layer)
		{
			//ring._parent.resize(ring._node_num);
			int node_size = 0;
			//newAngles,oldAngles;
			preAngleSpan=0;
			for(int j=0; j<ring._node_num; ++j)
			{				
				node_size += ring._node_size[j];		
				double span= getAngleSpan( ring._limits[j].first,  ring._limits[j].second);				
				
				/*if(handleIndex!=j)
				preAngleSpan +=span;*/
				preSpans.push_back(span);
				//oldAngles.push_back(ring._angle[j]);
			}
			if(preAngleSpan!=angleSpan)
				angleSpan=angleSpan;

			preAngleSpan = 2.*M_PI-preSpan;
		    newSize1 = (node_size - minus1);
			if(_ring[handleLayer]._node_num==1) //only one toplevel node 
			{
			
				double cur_angle = ring._limits[0].second;
				if(cur_angle > 2.*M_PI)
					cur_angle = cur_angle - 2.*M_PI;

				Point p;						
				p.x = cosf(cur_angle) * ring._ring_radial_radius;
				p.y = sinf(cur_angle) * ring._ring_radial_radius;	

				ring._radial_pos[0] = p + _centre;
				
				ring._angle[0] = cur_angle;			
				ring._limits[0].first= ring._limits[0].second = cur_angle;
			}
			else
			{
				double unit_angle = angleSpan / newSize1; 
				double compensate = 0;//angleSpan / (ring._node_num - minus2)*0;// * 0.25;
				double cur_angle = 0.;
				double scale = angleSpan /preAngleSpan;
				//newAngles.resize(ring._node_num);
				for(int j=start; j<ring._node_num + start; ++j) //Loop through all nodes at this level
				{
					int i=j%ring._node_num;	
					
					if( highlightState == HL_HANDLE && handleLayer==layer && j==start )// ||  (handleIndex=(i+1)%ring._node_num && handlet==1)) ) )
					{ 
						//angle =  ring._limits[i].second - ring._limits[i].first;
						 cur_angle = ring._limits[i].second ; //(angle + compensate);		
						 //newAngles[i]=cur_angle;
					}
					else 
					{
						double angle,angle1;// = ring._node_size[i] * unit_angle;
						Point p;
						//Calculate x, y position
						p.x = cosf(cur_angle) * ring._ring_radial_radius;
						p.y = sinf(cur_angle) * ring._ring_radial_radius;	

						//angle1 = angleSpan * getAngleSpan( ring._limits[i].first,  ring._limits[i].second)/preAngleSpan; // new span should be comparable with previously modified span //ring._node_size[i] * unit_angle;						
						angle = preSpans[i] * scale; // new span should be comparable with previously modified span //ring._node_size[i] * unit_angle;
						//angle = ring._node_size[i] * unit_angle;
						
						ring._radial_pos[i]=p + _centre;	
						
						ring._angle[i]=cur_angle;
						
						//newAngles[i]=cur_angle;
						
						cur_angle += (angle + compensate);	
						
						if(cur_angle > 2.*M_PI)
							cur_angle = cur_angle - 2.*M_PI;
					}	
				}
			
				for(int k=0; k<ring._node_num; ++k)//Loop through all nodes at this level
				{
					//If this node has children, calculate bisector limits and tangent limits for the node
					int i= k % ring._node_num;
					float t1=ring._limits[i].first, t2=ring._limits[i].second;

					if(_ring[layer]._nr_children[i] > 0)
					{
						pair<float, float> bisector, tangent, limits;
					    
						/*
						int im1 = i - 1 < 0 ? ring._node_num - 1 : i - 1;
						int ip1 = (i+1)%ring._node_num;

						//Calculate bisector limit---------------------------------------------------------------
						if(ring._angle[i] < ring._angle[im1]) //Angle i < angle i - 1
							bisector.first = ring._angle[i] - (ring._angle[i] - ring._angle[im1] + 2.*M_PI) / 2;
						else
							bisector.first = ring._angle[i] - (ring._angle[i] - ring._angle[im1]) / 2;

						if(ring._angle[i] > ring._angle[ip1]) //Angle i > angle i + 1
							bisector.second = ring._angle[i] + (ring._angle[ip1] - ring._angle[i] + 2.*M_PI) / 2;
						else 
							bisector.second = ring._angle[i] + (ring._angle[ip1] - ring._angle[i]) / 2;
						//Calculate bisector limit---------------------------------------------------------------

						float tan_angle = acosf(ring._ring_radial_radius / _ring[layer+1]._ring_radial_radius);
						tangent.first = ring._angle[i] - tan_angle;
						tangent.second = ring._angle[i] +  tan_angle;*/

						
						//Calculate limits
						limits.first = ring._angle[i]; //max(bisector.first, tangent.first); //keqin modified
						limits.second = ring._angle[(i+1)%ring._node_num];//-unit_angle/4; //min(bisector.second, tangent.second);
					
						if( limits.second<=0 )
							limits.second = 2.*M_PI + limits.second;						

						if(limits.first<=0)
							limits.first = 2.*M_PI + limits.first;
					
						ring._limits[i]=limits;
					}				
				}
			}
			
		}
		else
		{
			Ring &parent = _ring[layer-1];
			ring._parent.resize(ring._node_num);
			for(int i=0; i<parent._node_num; ++i)
			{
				for(int j=parent._child_range[i].first; j<=parent._child_range[i].second; j++)
				{
				    ring._parent[j]=i;		
				}
			}
						
			int cur = 0;
			for(int i=0; i<parent._node_num && i<parent._limits.size(); ++i)
			{
			
				int nr_children = parent._nr_children[i];
				if(0 == nr_children)
					continue;

				int node_size = 0;
				for(int j = cur; j < cur + nr_children; ++j)
					node_size += ring._node_size[j];

				float span = newSize1==0? 2.*M_PI: getAngleSpan( parent._limits[i].first,  parent._limits[i].second);

				float unit_angle = span / node_size;//(parent._limits[i].second - parent._limits[i].first)
				
				//Loop through all nodes for this parent
				float cur_angle = parent._limits[i].first;
				for(int j=cur; j<cur+nr_children; ++j)
				{
					float angle = ring._node_size[j]*unit_angle;
					Point p;
					
					//Calculate x,y position
					p.x = cosf(cur_angle) * ring._ring_radial_radius;
					p.y = sinf(cur_angle) * ring._ring_radial_radius;

					ring._radial_pos[j]=p + _centre;
					ring._angle[j]=cur_angle;

					cur_angle += angle;
					if(cur_angle > 2.*M_PI)
						cur_angle = cur_angle - 2.*M_PI;
				}
				cur+=nr_children;
			}
			cur = 0;
			for(int i=0; i<parent._node_num; ++i)
			{
				int nr_children = parent._nr_children[i];
				//double span = getAngleSpan(parent._limits[i].first, parent._limits[i].second); //( parent._angle[i],  parent._angle[ (i+1) % parent._node_num]);
				//double unit_angle = span / nr_children; 
                //Loop through all nodes for this parent
				for(int j=cur; j<cur+nr_children && j<ring._angle.size(); ++j)
				{
					pair<float, float> bisector, tangent, limits;
	
					int jm1 = j - 1 < 0 ? ring._angle.size() - 1 : j - 1;
					int jp1 = (j+1)%ring._angle.size();

					//If this child node has children, calculate bisector and tangent limits
					if(ring._nr_children.empty() != true && ring._nr_children[j] > 0)
					{
						//Calculate bisector limit---------------------------------------------------------------
						/*if(ring._angle[j] < ring._angle[jm1]) //Angle j < angle j - 1
							bisector.first = ring._angle[j] - (ring._angle[j] - ring._angle[jm1] + 2.*M_PI) / 2;
						else
							bisector.first = ring._angle[j] - (ring._angle[j] - ring._angle[jm1]) / 2;

						if(ring._angle[j] > ring._angle[jp1]) //Angle j > angle j + 1
							bisector.second = ring._angle[j] + (ring._angle[jp1] - ring._angle[j] + 2.*M_PI) / 2;
						else 
							bisector.second = ring._angle[j] + (ring._angle[jp1] - ring._angle[j]) / 2;
						//Calculate bisector limit---------------------------------------------------------------

						float tan_angle = acosf(ring._ring_radial_radius / _ring[layer+1]._ring_radial_radius);
						tangent.first = ring._angle[j] - tan_angle;
						tangent.second = ring._angle[j] +  tan_angle;

						//Calculate limits
						limits.first = max(bisector.first, tangent.first);
						limits.second = min(bisector.second, tangent.second);

						limits.first = max(limits.first, parent._limits[i].first);
						limits.second = min(limits.second, parent._limits[i].second);*/

						limits.first = ring._angle[j]; //max(bisector.first, tangent.first); //keqin modified
					    limits.second = ring._angle[(j+1)%ring._node_num];//-unit_angle/4; //min(bisector.second, tangent.second);
					
						if( limits.second<=0 )
							limits.second = 2.*M_PI + limits.second;						

						if(limits.first<=0)
							limits.first = 2.*M_PI + limits.first;

					
						ring._limits[j]=limits;
					}
					else //The last layer
					{
						limits.first = ring._angle[j] - fabs(ring._angle[j] - ring._angle[jm1]) / 2;
						limits.second = ring._angle[j] + fabs(ring._angle[j] - ring._angle[jp1]) / 2;

						limits.first = max(limits.first, parent._limits[i].first);
						limits.second = min(limits.second, parent._limits[i].second);

						limits.first = ring._angle[j]; //max(bisector.first, tangent.first); //keqin modified
					    limits.second = ring._angle[(j+1)%ring._node_num];//-unit_angle/4; //min(bisector.second, tangent.second);
					
						if( limits.second<=0 )
							limits.second = 2.*M_PI + limits.second;						

						if(limits.first<=0)
							limits.first = 2.*M_PI + limits.first;
					
						ring._limits[j]=limits;
					}
				}
				cur += nr_children;
			}
		}
		
	}
}

float TreeRing::getAngleSpan(float first, float second)
{
	float output;
	if(second < first)
		second = second + 2.*M_PI;
	output = second - first; // second - parent._limits[i].first
	return output;
}

//Call behind RadialLayout(), change radial tree to tree ring
void TreeRing::reCalculateNodePosition(int specLayer)
{ // if specLayer =-1 do all the layer
	for(int layer=0; layer<_num_layers; ++layer)
	{
		if(specLayer!=-1 && layer!=specLayer)
			continue;

		Ring &ring = _ring[layer];
        //Every node on this layer
		for(int i=0; i<ring._node_num, i<ring._limits.size(); ++i)
		{
			//calculate node's centre point on inner outline
			Point centre;
			float centre_angle = ring._limits[i].second - 0.5 * (ring._limits[i].second - ring._limits[i].first);
			float angle1, angle2;
			angle1 = ring._limits[i].first;
			angle2 = ring._limits[i].second;

			if(angle1 > angle2)
			{
				angle1 = angle1- M_PI*2; 		
			}	
			centre_angle =  (angle1 + angle2)/2;

			centre.x = (ring._radius[i] - _width/2) * cosf(centre_angle);
			centre.y = (ring._radius[i] - _width/2) * sinf(centre_angle);

			//ring._centre_pos.push_back(centre);
			ring._centre_pos[i]=centre + _centre;

			//if(layer == _num_layers - 1)//changed when switching inner ring out
			{
				centre.x = (ring._radius[i] - _width) * cosf(centre_angle);
				centre.y = (ring._radius[i] - _width) * sinf(centre_angle);
				
				/*int idx =  GetParentIndex(_num_layers-1, i); //get the nodeid of its parent
				centre.x = (_ring[0]._radius[idx] - _width) * cosf(centre_angle);
				centre.y = (_ring[0]._radius[idx] - _width) * sinf(centre_angle);*/
				ring._bun_pos[i]=centre + _centre;
			}

			vector<Point> v;
			Point p;
			p.x = ring._radius[i] * cosf(ring._limits[i].first) + _centre.x;
			p.y = ring._radius[i] * sinf(ring._limits[i].first) + _centre.y;
			v.push_back(p);

			p.x = (ring._radius[i] - _width) * cosf(ring._limits[i].first) + _centre.x;
			p.y = (ring._radius[i] - _width) * sinf(ring._limits[i].first) + _centre.y;
			v.push_back(p);

			p.x = ring._radius[i] * cosf(ring._limits[i].second)+ _centre.x;
			p.y = ring._radius[i] * sinf(ring._limits[i].second)+ _centre.y;
			v.push_back(p);

			p.x = (ring._radius[i] - _width) * cosf(ring._limits[i].second) + _centre.x;
			p.y = (ring._radius[i] - _width) * sinf(ring._limits[i].second) + _centre.y;
			v.push_back(p);

			//ring._pos.push_back(v);
			ring._pos[i]=v;
		}

		if(layer == _num_layers - 1)continue;
		//
		//Calculate group radial pos
		//
		int group_scale = 5; //How many nodes should to be grouped
		int mod = ring._node_num % group_scale;
		int n = ring._node_num - mod;
		if(n!=0)
		{
			ring._radial_pos.resize(ring._node_num);
			for(int i=0; i<n; i += group_scale)
			{
				cout<<i<<endl;
				Point &p = ring._radial_pos[i + group_scale / 2];
				for(int j=i; j<i+group_scale; ++j)
					ring._radial_pos[j] = p;
			}

			Point &p = ring._radial_pos[n + mod / 2 - 1];
			for(int i=n; i<ring._node_num; ++i)
			{
				ring._radial_pos[i] = p;
			}
		}		
	}
}

//Call behind RadialLayout(), change radial tree to tree ring
void TreeRing::CalculateNodePosition()
{
	for(int layer=0; layer<_num_layers; ++layer)
	{
		Ring &ring = _ring[layer];

		//Every node on this layer
		for(int i=0; i<ring._node_num, i<ring._limits.size(); ++i)
		{
			vector<Point> v;
			Point p;
			p.x = ring._radius[i] * cosf(ring._limits[i].first);
			p.y = ring._radius[i] * sinf(ring._limits[i].first);
			v.push_back(p);

			p.x = (ring._radius[i] - _width) * cosf(ring._limits[i].first);
			p.y = (ring._radius[i] - _width) * sinf(ring._limits[i].first);
			v.push_back(p);

			p.x = ring._radius[i] * cosf(ring._limits[i].second);
			p.y = ring._radius[i] * sinf(ring._limits[i].second);
			v.push_back(p);

			p.x = (ring._radius[i] - _width) * cosf(ring._limits[i].second);
			p.y = (ring._radius[i] - _width) * sinf(ring._limits[i].second);
			v.push_back(p);

			ring._pos.push_back(v);

			//calculate node's centre point on inner outline
			Point centre;
			float centre_angle = ring._limits[i].second - 0.5 * (ring._limits[i].second - ring._limits[i].first);
			centre.x = (ring._radius[i] - _width/2) * cosf(centre_angle);
			centre.y = (ring._radius[i] - _width/2) * sinf(centre_angle);
			ring._centre_pos.push_back(centre);

			//if(layer == _num_layers - 1) //changed when switching inner node out
			{
				centre.x = (ring._radius[i] - _width) * cosf(centre_angle);
				centre.y = (ring._radius[i] - _width) * sinf(centre_angle);
				
				/*int idx =  GetParentIndex(_num_layers-1, i); //get the nodeid of its parent
				centre.x = (_ring[0]._radius[idx] - _width) * cosf(centre_angle);
				centre.y = (_ring[0]._radius[idx] - _width) * sinf(centre_angle);*/
				ring._bun_pos.push_back(centre);
			}
		}

		if(layer == _num_layers - 1) continue;
		//
		//Calculate group radial pos
		//
		int group_scale = 5; //How many nodes should to be grouped
		int mod = ring._node_num % group_scale;
		int n = ring._node_num - mod;
		if(n!=0)
		{
			ring._radial_pos.resize(ring._node_num);
			for(int i=0; i<n; i += group_scale)
			{
				cout<<i<<endl;
				Point &p = ring._radial_pos[i + group_scale / 2];
				for(int j=i; j<i+group_scale; ++j)
					ring._radial_pos[j] = p;
			}

			Point &p = ring._radial_pos[n + mod / 2 - 1];
			for(int i=n; i<ring._node_num; ++i)
			{
				ring._radial_pos[i] = p;
			}
		}		
	}
}


string TreeRing::GetLongestName()
{
	int len = 0;
	string str;
	//Find the length of the longest name in these nodes 
	for(int layer=0; layer<_num_layers; ++layer)
		for(int i=0; i<_ring[layer]._node_num; ++i)
			if(len < (_ring[layer]._node_name[i]).length())
			{
				len = (_ring[layer]._node_name[i]).length();
				str = _ring[layer]._node_name[i];
			}
	return str;
}

//Node with larger variance has the priority to display 
void TreeRing::CalculateLabelDisplayOrder()
{
	for(int layer = _num_layers - 1; layer >= 0; --layer)
	{
		vector<int> order;
		for(int i=0; i<_ring[layer]._node_num; ++i)
			order.push_back(i);

		//Bubble sort for vector order, this vector likes a set of pointers
		for(int i = 0; i < order.size() - 1; i++)
		{
			int flag = 1;
			for(int j = 0; j < order.size()-i-1; j++)
			{
				if(_ring[layer]._node_variance[order[j]] < _ring[layer]._node_variance[order[j+1]])
				{
					int temp = order[j];
					order[j] = order[j+1];
					order[j+1] = temp;
					flag = 0;
				}
			}
			if(1 == flag)
				break;
		}
		//From the most inner layer to the outer layer, here is from layer2 to layer 0
		_label_display_order.push_back(order);
	}
}

int TreeRing::GetLayer0ParentIndex(int layer, int index)
{
	assert(layer > 0);

	int sum = 0;
	for(int i=0; i<_ring[layer]._node_size.size(); ++i)
		sum += _ring[layer]._node_size[i];

	int i = 0, sum2 = 0;

	for(; i<_ring[0]._node_size.size(); ++i)
	{
		sum2 += _ring[0]._node_size[i];
		if(sum2 >= sum)
			break;
	}
	
	return i;
}

//Translate the whole treering to (x,y)
void TreeRing::Translate(float x, float y)
{
	for(int layer=0; layer<_num_layers; ++layer)
	{
		for(int i=0; i<_ring[layer]._node_num, i<_ring[layer]._radial_pos.size(), i<_ring[layer]._centre_pos.size(); ++i)
		{
			Point p;
			p.x = x; p.y = y;

			_ring[layer]._radial_pos[i] += p;
			_ring[layer]._centre_pos[i] += p;

			//if(layer == _num_layers - 1)
			_ring[layer]._bun_pos[i] += p;

			vector<Point>::iterator iter = _ring[layer]._pos[i].begin();
			for(; iter != _ring[layer]._pos[i].end(); ++iter)
				(*iter) += p;
		}
	}
	_centre += Point(x, y);
	cout<<"centre:"<<_centre.x<<" "<<_centre.y<<endl;
}

/*//Translate the whole treering to (x,y)
void TreeRing::moveCenter(float x, float y)
{
	for(int layer=0; layer<_num_layers; ++layer)
	{
		for(int i=0; i<_ring[layer]._node_num, i<_ring[layer]._radial_pos.size(), i<_ring[layer]._centre_pos.size(); ++i)
		{
			Point p;
			p.x = x; p.y = y;

			_ring[layer]._radial_pos[i] += p;
			_ring[layer]._centre_pos[i] += p;

			if(layer == _num_layers - 1)
				_ring[layer]._bun_pos[i] += p;

			vector<Point>::iterator iter = _ring[layer]._pos[i].begin();
			for(; iter != _ring[layer]._pos[i].end(); ++iter)
				(*iter) += p;
		}
	}
	_centre = Point(x, y);
	cout<<"centre:"<<_centre.x<<" "<<_centre.y<<endl;
}*/

void TreeRing::Scale(float scale)
{
	
    scale += 1;
	for(int layer=0; layer<_num_layers; ++layer)
	{
		for(int i=0; i<_ring[layer]._node_num; ++i)
		{
			_ring[layer]._radial_pos[i] = _ring[layer]._radial_pos[i] * scale; //radial pos
			_ring[layer]._centre_pos[i] = _ring[layer]._centre_pos[i] * scale; //center of node
			//if(layer == _num_layers - 1)
				_ring[layer]._bun_pos[i] = _ring[layer]._bun_pos[i] * scale;  //bundle pos

			vector<Point>::iterator iter = _ring[layer]._pos[i].begin();  //node pos
			for(; iter != _ring[layer]._pos[i].end(); ++iter)
				(*iter) = (*iter) * scale;
			
			
			_ring[layer]._radius[i] *= scale;
			_ring[layer]._radial_radius[i] *= scale;
		}
		//Origin radius of the ring and the radial layout
		_ring[layer]._ring_radius *= scale;
		_ring[layer]._ring_radial_radius *= scale;		
	}
	//Ring width
	_width *= scale;
	cout<<"width:"<<_width<<endl;
}


//Get the index range of leaf node
pair<int, int> TreeRing::GetLeafNodeRange(int layer, int index)
{
	pair<int, int> range;
	if(layer == _num_layers - 1)
	{
		range.first = index;
		range.second = index;
		return range;
	}

	range = _ring[layer]._child_range[index];

	for(int i = layer+1; i<_num_layers-1; ++i)
	{
		if(range.first < _ring[i]._child_range[range.first].first)
			range.first = _ring[i]._child_range[range.first].first;

		if(range.second < _ring[i]._child_range[range.second].second)
			range.second = _ring[i]._child_range[range.second].second;
	}
	return range;
}

/*for(int i=0; i<_ring[layer]._node_num;++i)
		{
			//glBegin(GL_QUAD_STRIP);
			if(highlightLayer == layer && highlightIndex == i)
				painter->setPen( QColor( 0.8*255, 0.8*255, 0.8*255, 0.5*255 ) ); //glColor4f(0.8,0.8,0.8, 0.5);
			else
				painter->setPen( QColor( (_ring[layer]._node_color_iter + i)->r*255, (_ring[layer]._node_color_iter + i)->g*255, (_ring[layer]._node_color_iter + i)->b*255, 0.8*255 ) ); //glColor4f((_ring[layer]._node_color_iter + i)->r, (_ring[layer]._node_color_iter + i)->g,
						  //(_ring[layer]._node_color_iter + i)->b, 0.8);

			for(int j=0; j<(_ring[layer]._pos[i]).size()-1; ++j)
				//glVertex3f((_ring[layer]._pos[i])[j].x, (_ring[layer]._pos[i])[j].y, 0.0f);
				painter->drawLine((_ring[layer]._pos[i])[j].x, (_ring[layer]._pos[i])[j].y, (_ring[layer]._pos[i])[j+1].x, (_ring[layer]._pos[i])[j+1].y);
			//glEnd();
			
		}
*/