

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include "ExtraLayerMarker.h"
#define MARKERCOLOR QColor(60,60,60,255)
#define HIGHLIGHTCOLOR QColor(213,106,0,255) //QColor(60,60,255,255)

ExtraLayerMarker::ExtraLayerMarker(TreeRing *tr, char *inf, int numOfNode, float ringRadius, SimData *simData, Point centre)
{
	_num_layers= tr->GetNrLayers();
	_treeRing=tr;
	// read the graph information
	GraphIndexParser2 *gip = new GraphIndexParser2(inf, numOfNode);
	//int ne = gip->GetNumOfEdges(); // get the number of edges

	//extraLayerName
	fileName = QString(inf);

	_rr = ringRadius;
	_nn = numOfNode;
	_ww = tr-> getWidth();
	//_income_edges_of_node.resize(_nn);

	//_edge_by = EDGE_BY;
	_centre = centre;

	_hl_from = -1;
	_hl_to = -1;
	highlightIndex = -1;

	_sim_data = simData;

	// create the _out_list;
	for(int i=0; i<_nn; i++)
	{
		vector <int> outNodeList = gip->GetOutNodeIndex(i);
		_out_list.push_back(outNodeList);

		vector<int> in_node_list = gip->GetInNodeIndex(i);
		_in_list.push_back(in_node_list);
	}; 
	delete gip;

	//createEdgeTable();
	GenerateMarkers(_treeRing);
	readExtraLayerFile( fileName, marker);
	//ComputeIncomingEdgesOfNodes();
}

bool ExtraLayerMarker::readExtraLayerFile( QString fileName, vector <struct elMarker_str> &marker)
{
    QFile inputFile(fileName);
	QString pName;	
	
	if (inputFile.open(QIODevice::ReadOnly))
	{
		   int count=0;
		   QTextStream in(&inputFile);
		   while ( !in.atEnd() )
		   {			   
				QString line = in.readLine();    
				{
					QStringList sname=line.split(' ');
					/*for(int i=0; i<sname.size(); i++)
					{
						QString phrase=sname[i];
						phrase=phrase;
					}*/
					if(count<marker.size())
					{
						if(sname[1]=="0")
						  marker[count].moreLayer=false;
						else 
						  marker[count].moreLayer=true;
					}
					//convert QString to number, convert QString to integer
					
				}				
				count++;
		   }
		  //rateLimitSymbol=rateLimitSymbol1;
	}    
	else
		return false;

	return true;
}

int ExtraLayerMarker::GetNumOfEdges()
{
	int edgeNum=0;
	for(int i=0; i<_nn; i++)
	{
	    edgeNum +=_out_list[i].size();	
	}
	return edgeNum;
}

ExtraLayerMarker::ExtraLayerMarker(TreeRing *tr, vector<bool> markerFlag, int numOfNode, float ringRadius, SimData *simData, Point centre)
{
	_treeRing=tr;
	_num_layers= tr->GetNrLayers();	
	int ne = GetNumOfEdges(); // get the number of edges

	_ww = tr-> getWidth();
	_rr = ringRadius;
	_nn = numOfNode;
	//_income_edges_of_node.resize(_nn);
		
	_centre = centre;
	_hl_from = -1;
	_hl_to = -1;

	_sim_data = simData;
	
	reGenerateMarkers(_treeRing, markerFlag);
	
}


void ExtraLayerMarker::GenerateMarkers(TreeRing *tr)
{
	marker.clear();
	for(int i=0; i<_nn; i++)  //_nn is the number of leaf nodes
	{
		float sx,sy,angle;
		struct elMarker_str mk;
		getNodeCoordinate(tr, _num_layers-1, i, &sx, &sy, &angle);	
		mk.sx=sx, mk.sy=sy;
		mk.layer=_num_layers-1, mk.idx=i;
		mk.angle=angle;		
		marker.push_back(mk);			
	}; // end for(i)	
}

void ExtraLayerMarker::reGenerateMarkers(TreeRing *tr, vector<bool> markerFlag)
{
	marker.clear();
	for(int i=0; i<_nn; i++)  //_nn is the number of leaf nodes
	{
		float sx,sy,angle;
		struct elMarker_str mk;
		getNodeCoordinate(tr, _num_layers-1, i, &sx, &sy, &angle);	
		mk.sx=sx, mk.sy=sy;
		mk.layer=_num_layers-1, mk.idx=i;
		mk.angle=angle;	
		if(i<markerFlag.size())
		   mk.moreLayer=markerFlag[i];
		else
			mk.moreLayer=false;
		marker.push_back(mk);				
	}; // end for(i)
}

void ExtraLayerMarker::updateMarkerAngle(TreeRing *tr)
{
	
	for(int i=0; i<_nn; i++)  //_nn is the number of leaf nodes
	{
		float sx,sy,angle;
		struct elMarker_str mk;
		getNodeCoordinate(tr, _num_layers-1, i, &sx, &sy, &angle);	
		marker[i].sx=sx, marker[i].sy=sy;
		//mk.layer=_num_layers-1, mk.idx=i;
		marker[i].angle=angle;					
	};

}

//#define  H_RADIUS  20


ExtraLayerMarker::~ExtraLayerMarker()
{
}

void ExtraLayerMarker::Render(QPainter *painter)
{
	int nr_makers = marker.size();
	float outerRadious=  _treeRing->_ring[_num_layers-1]._radius[0];// //changed when switching inner ring out // + _treeRing-> getWidth(); 
	float msize1 = outerRadious/60, msize2; //marker's size
	if(msize1<3)
		msize1=3;
	else if(msize1>25)
		msize1=25;
			
	msize2 = outerRadious * M_PI  / _nn * 3;
	msize2 = int(msize2+0.5);
	if(msize2 < 3)
		msize2 = 3;	

	painter->setPen(MARKERCOLOR);
	painter->setBrush(MARKERCOLOR);

	for(int i=0; i< nr_makers; i++)  //_nn is the number of leaf nodes
	{
		if(marker[i].moreLayer)
		{
			renderMarker(painter, marker[i].sx, marker[i].sy, marker[i].angle,  (_treeRing->_ring[_num_layers-1]._limits)[i].first, (_treeRing->_ring[_num_layers-1]._limits)[i].second, msize1, msize2, outerRadious);		
			//break;
		}
	}; // end for(i)
	

	QPen pen;	
	pen.setWidth(2);
	pen.setColor(HIGHLIGHTCOLOR);
	painter->setPen( pen );
	painter->setBrush(HIGHLIGHTCOLOR);
	if(highlightIndex>=0)
	{
		int hi = highlightIndex;
		renderMarker(painter, marker[hi].sx, marker[hi].sy, marker[hi].angle, (_treeRing->_ring[_num_layers-1]._limits)[hi].first, (_treeRing->_ring[_num_layers-1]._limits)[hi].second, msize1, msize2, outerRadious);			
	}; // end if(_hl..)	
}

void ExtraLayerMarker::renderMarker(QPainter *painter, float sx, float sy, float angle, float angle1, float angle2, int width, int height, float radius)
{
	//width=30;
	//QColor color=QColor(10,10,10);
	QPoint start,end;
	float dx,dy;
	dx = width*cos(angle);
	dy = width*sin(angle);
	start=QPoint(sx-dx,sy-dy);

	dx = 1*cos(angle);
	dy = 1*sin(angle);
	end=QPoint(sx+dx,sy+dy);
		

	vector<QPointF> outPut1 = _treeRing->getATrapezoid(angle, angle1, angle2, radius, width); //_treeRing->getATrapezoid(end, start, height, height/2);	//painter->drawPolygon();
	if(outPut1.size()<4)
		return;
		
	vector<QPointF> outPut2 = _treeRing->TrapezoidtoNode(outPut1[0], outPut1[1], outPut1[3], outPut1[2], radius);

	QPolygonF QP=QPolygonF();// << outPut2[0] << outPut[1] << outPut[2] << outPut[3]
	for(int i=0; i<outPut2.size(); i++)
	{
		QP = QP << outPut2[i];		
	}
	painter->drawPolygon(QP);
}



void ExtraLayerMarker::drawArrow(QPainter *painter, QPointF start, QPointF end, float height, QColor color)
{//single arrow edge
	QPointF temp;
	temp = start; start =end; end =temp;
	QLineF line(start, end);

	float ds = sqrt(line.dx()*line.dx() + line.dy()*line.dy());  
	QPointF Dis = QPointF(6*line.dx()/ds, 6*line.dy()/ds);
	//start=start-Dis;

    if (qFuzzyCompare(line.length(), qreal(0.)))
        return;

	painter->save (); 
	painter->setPen(color);
	painter->setBrush(color);
	
	painter->setRenderHint(QPainter::Antialiasing, true);
    
	float size=height/2;	
	qreal arrowSize(size*1.5);

	static const double Pi = 3.14159265358979323846264338327950288419717; 
    static double TwoPi = 2.0 * Pi;

    //painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    //painter->drawLine(line);

	double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0)
		angle = TwoPi - angle;

    QPointF sourceArrowP1 = start + QPointF(sin(angle - Pi / 2.5) * arrowSize,
                                                  cos(angle - Pi / 2.5) * arrowSize);                                                  
    QPointF sourceArrowP2 = start + QPointF(sin(angle - Pi + Pi / 2.5) * arrowSize,
                                                  cos(angle - Pi + Pi / 2.5) * arrowSize);   
    //QPointF destArrowP1 = end + QPointF(sin(angle - Pi / 2.6) * arrowSize, cos(angle - Pi / 2.6) * arrowSize);
    //QPointF destArrowP2 = end + QPointF(sin(angle - Pi + Pi / 2.6) * arrowSize, cos(angle - Pi + Pi / 2.6) * arrowSize);
	
	QPointF dis = (sourceArrowP1 + sourceArrowP2)/2;
    dis=start-dis;
	sourceArrowP1 = sourceArrowP1 + dis;
	sourceArrowP2 = sourceArrowP2 + dis;
	//painter->setBrush(color);
    //painter->drawPolygon(QPolygonF() << line.p2() << sourceArrowP1 << sourceArrowP2);

	QVector<QPoint> points;	         
	points.append(QPoint(line.p2().x()+0.5,line.p2().y()+0.5));
	points.append(QPoint(sourceArrowP1.x()+0.5, sourceArrowP1.y()+0.5));
	points.append(QPoint(sourceArrowP2.x()+0.5, sourceArrowP2.y()+0.5));
	QRegion arrow(points);
	
	//arrow = arrow -	noteRect;
	//QRegion reg=getCalloutNoteRectforNote();	
	

	QPainterPath path;
 	path.addRegion( arrow); 	
 	painter->fillPath( path, QBrush( color ) );
	
	//painter->drawLine(sourceArrowP1,line.p2());
	//painter->setRenderHint(QPainter::Antialiasing, false);
	painter->restore(); 
	
}

void ExtraLayerMarker::GetEdgeIndexByLayer(int layer, int nodeIndex[2], int edgeIndex[2])
{
	edgeIndex[0] = _nodeToEdgeTable[nodeIndex[0]];
	edgeIndex[1] = _nodeToEdgeTable[nodeIndex[1]];
}

void ExtraLayerMarker::Highlight(int hi)
{
	highlightIndex = hi;
	
}

void ExtraLayerMarker::getNodeCoordinate(TreeRing *tr, int trLayer, int nodeIndex, float *x, float *y, float *angle)
{
	if(trLayer != _num_layers-1)
	{
		cout<<"Compute bundles starting point error!\n"<<endl;
		return;
	}
 	//*x = ((tr->_ring)[trLayer]._node_pos)[nodeIndex].x;
 	//*y = ((tr->_ring)[trLayer]._node_pos)[nodeIndex].y;

	if(((tr->_ring)[trLayer]._bun_pos).size()<=nodeIndex)
	{
		 cout << "nodeIndex exceed _bun_pos.size()" <<endl;
	     return;
	}

 	//*x = ((tr->_ring)[trLayer]._bun_pos)[nodeIndex].x;
 	//*y = ((tr->_ring)[trLayer]._bun_pos)[nodeIndex].y;

	// DEBUG

	double unit_angle=2. * M_PI / (double) _nn;
	//*angle = ((tr->_ring)[trLayer]._angle)[nodeIndex];

	float angle1, angle2;
	angle1 = ((tr->_ring)[trLayer]._limits)[nodeIndex].first;
	angle2 = ((tr->_ring)[trLayer]._limits)[nodeIndex].second; //ring._limits[i].second;

	if(angle1 > angle2)
	{
		angle1 = angle1- M_PI*2; 		
	}	
	*angle =  (angle1 + angle2)/2;

	float radius = _treeRing->_ring[trLayer]._radius[0] + tr-> getWidth();

	*x = radius * cos(*angle);
 	*y = radius * sin (*angle);


}

void ExtraLayerMarker::UpdateAllBundles(int handle_layer, int handle_index, int handlet, TreeRing *tr)
{//not work well
	if(handle_layer == -1 || handle_index == -1)
		return;
	
	for(int k=0; k<tr->GetNrLayers(); k++)
	//int k=handle_layer;
	for(int l=0; l<tr->_ring[k]._node_num; l++)
	{
		pair<int, int>range = tr->GetLeafNodeRange(k, l);
		int out_layer = tr->GetNrLayers()-1;
		for(int i=range.first; i<=range.second; ++i)
		{	
			//Update the out edge of these children
			Point p = tr->_ring[out_layer]._bun_pos[i];
			for(int j=1; j<_out_list[i].size(); ++j)
			{
				//bun[ _nodeToEdgeTable[i] + j - 1 ].UpdateControl(0, p);
			}	
		}
	}
    cerr<<"SDF"<<endl;
}

void ExtraLayerMarker::UpdateBundles(int highlight_layer, int highlight_index, TreeRing *tr)
{
	if(highlight_layer == -1 || highlight_index == -1)
		return;
	 //Find the index of children on the outest layer
	pair<int, int>range = tr->GetLeafNodeRange(highlight_layer, highlight_index);
	int out_layer = tr->GetNrLayers()-1;

	for(int i=range.first; i<=range.second; ++i)
	{	
		//Update the out edge of these children
		Point p = tr->_ring[out_layer]._bun_pos[i];
		for(int j=1; j<_out_list[i].size(); ++j)
		{
			//bun[ _nodeToEdgeTable[i] + j - 1 ].UpdateControl(0, p);
		}
	
	}
    cerr<<"SDF"<<endl;
}
