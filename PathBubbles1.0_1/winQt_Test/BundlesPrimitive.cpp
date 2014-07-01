

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include "BundlesPrimitive.h"

#define COMPLETECOLOR QColor(0,90,50) 
#define INCOMPLETECOLOR QColor(161,217,155) 	
#define MISSINGCOLOR QColor(255,255,255,255)

#define MIDIUM_HIGHLIGHTCOLOR QColor(213,106,0,255) //QColor(20,20,255,255)

BundlesPrimitive::BundlesPrimitive(TreeRing *tr, char *inf, int numOfNode, int numOfAllNode, int layer, float ringRadius, SimData *simData, int EDGE_BY, Point centre)
{
	_layer=layer;
	_num_layers= tr->GetNrLayers();
	// read the graph information
	GraphIndexParser2 *gip = new GraphIndexParser2(inf, numOfAllNode);
	int ne = gip->GetNumOfEdges(); // get the number of edges

	_rr = ringRadius;
	_nn = numOfNode;
	
	_edge_by = EDGE_BY;
	_centre = centre;

	_hl_from = -1;
	_hl_to = -1;

	//_sim_data = simData;
	for(int i=0; i<_nn; i++)
	{
		vector<int> outNodeList, in_node_list;
	    //_in_list.push_back(in_node_list);
		if(layer!=_num_layers-1)
		{  
			 set<int> outNodeList_1, in_node_list_1;
			 vector<int> child = tr->getChildrenNodes(layer, i);
             for(int j=0; j<child.size(); j++)
			 {
				 vector<int> temp1 = gip->GetOutNodeIndex(child[j]);
		         vector<int> temp2 = gip->GetInNodeIndex(child[j]);
				 for(int k=0; k<temp1.size(); k++)
                 {
					 outNodeList_1.insert(temp1[k]);				     
				 }
				 for(int k=0; k<temp2.size(); k++)
                 {
					 in_node_list_1.insert(temp2[k]);
				 }
			 }	
			 set<int> outNodeList_2;
			 for(set<int>::iterator it=outNodeList_1.begin(); it!=outNodeList_1.end(); it++)
			 {
				 int id=tr->getAncestorNodebyLayer(_num_layers-1, *it, layer);
				 if(id!=i)
				    outNodeList_2.insert(id);	 
			 }
			 for(set<int>::iterator it=in_node_list_1.begin(); it!=in_node_list_1.end(); it++)
			 {
				 int id=tr->getAncestorNodebyLayer(_num_layers-1, *it, layer) ;
				 if(id!=i)
					  outNodeList_2.insert(id);		         
			 }	
			 outNodeList.push_back(i);
			 for(set<int>::iterator it=outNodeList_2.begin(); it!=outNodeList_2.end(); it++)
			 {
			     outNodeList.push_back(*it);			 
			 }			 
				
		}
		else
		{
			outNodeList = gip->GetOutNodeIndex(i);
		    in_node_list = gip->GetInNodeIndex(i);
		    for(int j=1; j<in_node_list.size(); j++)
			    outNodeList.push_back(in_node_list[j]);
		}

		_out_list.push_back(outNodeList); //keqin for pathbubbles case since no direction

		in_node_list.clear();
		_in_list.push_back(in_node_list);
	}
	
	//consider no edge direction in pathbubbles
	/*for(int i=0; i<_nn; i++)
	{
	    vector <int> outNodeList = gip->GetOutNodeIndex(i);
		//_out_list.push_back(outNodeList);

		
		vector<int> in_node_list = gip->GetInNodeIndex(i);
		//_in_list.push_back(in_node_list);

		
		for(int j=1; j<in_node_list.size(); j++)
			outNodeList.push_back(in_node_list[j]);

		_out_list.push_back(outNodeList); //keqin for pathbubbles case since no direction

		in_node_list.clear();
		_in_list.push_back(in_node_list);
	}
	*/
	delete gip;

	createEdgeTable();
	//ComputeIncomingEdgesOfNodes();
}

int BundlesPrimitive::GetNumOfEdges()
{
	int edgeNum=0;
	for(int i=0; i<_nn; i++)
	{
	    edgeNum +=_out_list[i].size();	
	}
	return edgeNum;
}

BundlesPrimitive::BundlesPrimitive(TreeRing *tr, vector<vector<int>> outList, vector<vector<int>> inList, int numOfNode, int layer,  float ringRadius, SimData *simData, int EDGE_BY, Point centre)
{
	_num_layers= tr->GetNrLayers();
	_layer=layer;

	// read the graph information
	//GraphIndexParser2 *gip = new GraphIndexParser2(inf, numOfNode);
	int ne = GetNumOfEdges(); // get the number of edges

	_rr = ringRadius;
	_nn = numOfNode;
	//_income_edges_of_node.resize(_nn);

	_edge_by = EDGE_BY;
	_centre = centre;

	_hl_from = -1;
	_hl_to = -1;


	for(int i=0; i<_nn; i++)
	{

		vector<int> outNodeList, in_node_list;
	    
		if(layer!=_num_layers-1)
		{  
			 set<int> outNodeList_1, in_node_list_1;
			 vector<int> child = tr->getChildrenNodes(layer, i);
             for(int j=0; j<child.size(); j++)
			 {
				 vector<int> temp1 =  outList[child[j]];
		         vector<int> temp2 =  inList[child[j]];
				 for(int k=0; k<temp1.size(); k++)
                 {
					 outNodeList_1.insert(temp1[k]);				     
				 }
				 for(int k=0; k<temp2.size(); k++)
                 {
					 in_node_list_1.insert(temp2[k]);
				 }
			 }	
			 set<int> outNodeList_2;
			 for(set<int>::iterator it=outNodeList_1.begin(); it!=outNodeList_1.end(); it++)
			 {
				 int id=tr->getAncestorNodebyLayer(_num_layers-1, *it, layer);
				 if(id!=i)
				    outNodeList_2.insert(id);	 
			 }
			 for(set<int>::iterator it=in_node_list_1.begin(); it!=in_node_list_1.end(); it++)
			 {
				 int id=tr->getAncestorNodebyLayer(_num_layers-1, *it, layer) ;
				  if(id!=i)
					  outNodeList_2.insert(id);		         
			 }	
			 outNodeList.push_back(i);
			 for(set<int>::iterator it=outNodeList_2.begin(); it!=outNodeList_2.end(); it++)
			 {
			     outNodeList.push_back(*it);			 
			 }			 
				
		}
		else
		{
			in_node_list = inList[i];

			//outNodeList.push_back(i);
			for(int j=0; j<outList[i].size(); j++)
				outNodeList.push_back(outList[i][j]);

			for(int j=1; j<in_node_list.size(); j++)
			    outNodeList.push_back(in_node_list[j]);
		
		}
		_out_list.push_back(outNodeList); //keqin for pathbubbles case since no direction

		in_node_list.clear();
		_in_list.push_back(in_node_list);
	}
	//delete gip;

	createEdgeTable();
	//ComputeIncomingEdgesOfNodes();
}

void BundlesPrimitive::GenerateCurveBundles(TreeRing *tr, float bundling_strength)
{
	//if(_layer==0)
	//	return;
	//ENABLE_DRAW_CHANGED = false;

	//for(int i=0; i<_nn; i++) 
	// TODO: did not save the last one - silly me! need to fix the 
	// tree reading prog.. 
	int idx;
	for(int i=0; i<_nn; i++)  //_nn is the number of leaf nodes
	{
		int sumnc = (_out_list[i]).size();
		float  sx, sy;
		//if(i>50&&i<60)
		if(sumnc>1) // has edge out
		{
			getNodeCoordinate(tr, _layer, i, &sx, &sy);		//_num_layers-1
			//idx =  tr->GetParentIndex(_num_layers-1, i); //get the nodeid of its parent
			//getNodeCoordinate(tr, 0, idx, &sx, &sy);

			//printf("node: %d: %f %f\n", i, sx, sy);

			// add the stroke - start from 1, since 0 is the index of the source node
			for(int nc=1; nc<sumnc; nc++)
			{
				Stroke  curve;
				float  nx, ny;
				//printf("out node  %d\n", outNodeList[nc]);
				// get the target coord 
				
				getNodeCoordinate(tr, _layer, _out_list[i][nc], &nx, &ny);
				//idx =  tr->GetParentIndex(_num_layers-1, _out_list[i][nc]); //get the nodeid of its parent
				//getNodeCoordinate(tr, 0, idx, &nx, &ny);

				// find the control points based on the Danny Holten paper 
				// hierarchical edge bundles
				insertHoltenPoints_2(&curve, tr, _out_list[i][0], sx, sy, 
				                               _out_list[i][nc], nx, ny, bundling_strength);

/*cerr << "HoltenPoints: " << _out_list[i][0]<<" " << _out_list[i][nc]<<endl;*/

				float dist = (sx - nx)*(sx - nx) + (sy - ny)*(sy - ny);
				float sqr_ring_radius = 4 * _rr * _rr;
				float alpha = dist/sqr_ring_radius;
				curve.alpha = (1-alpha)*DEFAULT_A;

				src_id.push_back(_out_list[i][0]);
				dst_id.push_back(_out_list[i][nc]);

				//_edgeToNodeTable[ecount]
				int id1 = _out_list[i][0];
				curve.SetStrokeColor(tr->_ring[_layer]._color_by_crosstalk[id1].r,tr->_ring[_layer]._color_by_crosstalk[id1].g, tr->_ring[_layer]._color_by_crosstalk[id1].b, tr->_ring[_layer]._color_by_crosstalk[id1].a );
				
				bun.push_back(curve);
				//break; //for debug
			}; // end for(nc)
			//break; //for debug
		}; // end for(sumc)		
	};  
	//cerr<<"SDF"<<endl;
}

void BundlesPrimitive::reGenerateCurveBundles(TreeRing *tr, float bundling_strength)
{
	//ENABLE_DRAW_CHANGED = false;

	//for(int i=0; i<_nn; i++) 
	// TODO: did not save the last one - silly me! need to fix the 
	// tree reading prog.. 
	bun.clear();
	src_id.clear();
	dst_id.clear();	
	for(int i=0; i<_nn; i++)  //_nn is the number of leaf nodes
	{
		int sumnc = (_out_list[i]).size();
		float  sx, sy;

		if(sumnc>1) // has edge out
		{
			//getNodeCoordinate(tr, _num_layers-1, i, &sx, &sy);//changed when switching inner ring out
			getNodeCoordinate(tr, _layer, i, &sx, &sy);

			//printf("node: %d: %f %f\n", i, sx, sy);
			//sx = sx + _centre.x;
			//sy = sy + _centre.y;
			// add the stroke - start from 1, since 0 is the index of the source node
			for(int nc=1; nc<sumnc; nc++)
			{
				Stroke  curve;
				float  nx, ny;
				//printf("out node  %d\n", outNodeList[nc]);
				// get the target coord 
				//getNodeCoordinate(tr, _num_layers-1, _out_list[i][nc], &nx, &ny); ////changed when switching inner ring out
				getNodeCoordinate(tr, _layer, _out_list[i][nc], &nx, &ny);
				//nx = nx + _centre.x;
			    //ny = ny + _centre.y;
				// find the control points based on the Danny Holten paper 
				// hierarchical edge bundles
				//nx = nx + _centre.x;
			    //ny = ny + _centre.y;

				insertHoltenPoints_2(&curve, tr, _out_list[i][0], sx, sy, 
				                               _out_list[i][nc], nx, ny, bundling_strength);
				

/*cerr << "HoltenPoints: " << _out_list[i][0]<<" " << _out_list[i][nc]<<endl;*/

				float dist = (sx - nx)*(sx - nx) + (sy - ny)*(sy - ny);
				float sqr_ring_radius = 4 * _rr * _rr;
				float alpha = dist/sqr_ring_radius;
				curve.alpha = (1-alpha)*DEFAULT_A;

				src_id.push_back(_out_list[i][0]);
				dst_id.push_back(_out_list[i][nc]);

				int id1 = _out_list[i][0];
				curve.SetStrokeColor(tr->_ring[_layer]._color_by_crosstalk[id1].r,tr->_ring[_layer]._color_by_crosstalk[id1].g, tr->_ring[_layer]._color_by_crosstalk[id1].b, tr->_ring[_layer]._color_by_crosstalk[id1].a );
				bun.push_back(curve);
				//break; //for debug
			}; // end for(nc)	
			//break; //for debug
		}; // end for(sumc)
	}; // end for(i)
    moveToCurrentCenter(_centre);
	Highlight(_hl_from, _hl_to);
	cerr<<"SDF"<<endl;
}

void BundlesPrimitive::moveToCurrentCenter(Point _centre)
{	
	for(int i=0; i<bun.size(); ++i)
	{
		if (!bun[i].computed)
		{
			//	printf("Computing curve \n");
			//bun[i].TranslateControl(_centre);
			bun[i].computeLimitCurve();
			bun[i].InterpolateColor();
			bun[i].computed = GL_TRUE;
			//bun[i].TranslateLimit(_centre);
		}
	}	
}

#define  H_RADIUS  20

void BundlesPrimitive::insertHoltenPoints(Stroke *curve, TreeRing *tr, int si, float sx, float sy, 
										  int ni, float nx, float ny, float beta)
{
	curve->setEnableLineColor(true);
	curve -> add( sx, sy);

	//control points LCA in HOLTEN PAPER
	
    int layer_index   = _layer; //_num_layers-1;//_num_layers-1;
	int source_parent, dest_parent;
	if(layer_index!=_num_layers-1)
	{
		source_parent = si,
        dest_parent = ni;
		while(layer_index!=_num_layers-1)
		{
			source_parent = tr->GetAChildInMiddle(layer_index, source_parent);
	        dest_parent   = tr->GetAChildInMiddle(layer_index, dest_parent);			
		    layer_index++;
		}
	}
	else
	{
		source_parent = tr->GetParentIndex(layer_index, si);
	    dest_parent   = tr->GetParentIndex(layer_index, ni);
	}

	vector<Point>src_tmp, dest_tmp; //vector
	Point s_tmp, d_tmp, ds, dt;

	while(source_parent != dest_parent)
	{
		if(tr->_ring[layer_index-1]._radial_pos.size()<=dest_parent)
			break;

		s_tmp =Point((tr->GetRadialPos(layer_index-1, source_parent)).x, 
						(tr->GetRadialPos(layer_index-1, source_parent)).y);


		d_tmp = Point((tr->GetRadialPos(layer_index-1, dest_parent)).x,
						(tr->GetRadialPos(layer_index-1, dest_parent)).y);

		/*ds = s_tmp-Point(sx,sy);
		dt = d_tmp-Point(sx,sy);

		s_tmp = Point(sx,sy) - ds; 
		d_tmp = Point(sx,sy) - dt;*/

		src_tmp.push_back(s_tmp);
		dest_tmp.push_back(d_tmp);
		
		--layer_index;
		if(layer_index <= 0)
		{
			src_tmp.push_back(_centre);  //
			//src_tmp.push_back(Point(100,10)); 
			break;
		} 		
		source_parent = tr->GetParentIndex(layer_index, source_parent);
		dest_parent   = tr->GetParentIndex(layer_index, dest_parent);
	}

	if(source_parent == dest_parent )
	{
		if(tr->_ring[layer_index-1]._radial_pos.size() > source_parent)
		src_tmp.push_back(Point((tr->GetRadialPos(layer_index-1, source_parent)).x, 
							(tr->GetRadialPos(layer_index-1, source_parent)).y));	
		//src_tmp.push_back(_centre); 
	}

	//apply bundling strength
	int num_control_points = src_tmp.size() + dest_tmp.size();

	if(num_control_points == 1)
	{
		curve->add(src_tmp[0].x, src_tmp[0].y);
		curve->add(nx, ny); 
		return;
	}

	Point &p0 = src_tmp[0];
	Point &pn = dest_tmp.empty() ? src_tmp.back() : dest_tmp[0];
	int i;

	for(i = 0; i < src_tmp.size(); ++i)
	{
		src_tmp[i] = src_tmp[i] * beta + (p0 + (pn-p0) * (1.0*i/(num_control_points-1))) * (1 - beta);
		curve->add(src_tmp[i].x, src_tmp[i].y);
	}

	for(int j = dest_tmp.size()-1; j >= 0; --j, ++i)
	{
		dest_tmp[j] = dest_tmp[j] * beta + (p0 + (pn-p0) * (1.0*i/(num_control_points-1))) * (1 - beta);
		curve->add(dest_tmp[j].x, dest_tmp[j].y);
	}
	curve -> add(nx, ny);
}

void BundlesPrimitive::insertHoltenPoints_2(Stroke *curve, TreeRing *tr, int si, float sx, float sy, 
										  int ni, float nx, float ny, float beta)
{//for inner ring out
	static const double Pi2 = 3.14159265358979323846264338327950288419717*2; 
	vector<Point>src_tmp, dest_tmp; //vector
	Point s_tmp, d_tmp, ds, dt;
	int layer_index   = _layer; //_num_layers-1;//_num_layers-1;
	int source_parent, dest_parent;

	curve->setEnableLineColor(true);
	
	curve -> add( sx, sy);

	if(layer_index!=_num_layers-1)
	{
		source_parent = si,
        dest_parent = ni;
		while(layer_index!=_num_layers-1)
		{
			source_parent = tr->GetAChildInMiddle(layer_index, source_parent);
	        dest_parent   = tr->GetAChildInMiddle(layer_index, dest_parent);			
		    layer_index++;
		}
		//curve -> add( s_tmp.x, s_tmp.y);
		
		//src_tmp.push_back(s_tmp);
		//dest_tmp.push_back(d_tmp);
	}
	else
	{
		source_parent = tr->GetParentIndex(layer_index, si);
	    dest_parent   = tr->GetParentIndex(layer_index, ni);

		/*s_tmp =Point((tr->GetRadialPos(layer_index-1, source_parent)).x, 
						(tr->GetRadialPos(layer_index-1, source_parent)).y);


		d_tmp = Point((tr->GetRadialPos(layer_index-1, dest_parent)).x,
						(tr->GetRadialPos(layer_index-1, dest_parent)).y);

		
		src_tmp.push_back(s_tmp);
		dest_tmp.push_back(d_tmp);*/
	}
		
	
	while(source_parent != dest_parent)
	{
		Ring _ring = tr->_ring[layer_index-1];
		if(_ring._radial_pos.size()<=dest_parent)
			break;

		Point p;
		float start, end, angle, radius1, radius2, radius, r1, r2;
		//int rnum = _layer - (layer_index-1);
		//float curveScale=0.5;
		//s_tmp =Point((tr->GetRadialPos(layer_index-1, source_parent)).x, (tr->GetRadialPos(layer_index-1, source_parent)).y);		
		//d_tmp = Point((tr->GetRadialPos(layer_index-1, dest_parent)).x, (tr->GetRadialPos(layer_index-1, dest_parent)).y);
		//radius2 = tr->_ring[_layer]._ring_radial_radius; //_ring_radial_radius: encoding group info
		radius1 = _ring._ring_radial_radius*2;
		//radius = radius2 + (radius1 - radius2)*curveScale;  
		
		//r2 = tr->_ring[_layer]._ring_radial_radius;
		//r1 = _ring._ring_radial_radius;
		
		p.x = cosf(_ring._angle[source_parent] ) * radius1;
		p.y = sinf(_ring._angle[source_parent] ) * radius1;	

		s_tmp = p + _centre;		

		p.x = cosf(_ring._angle[dest_parent] ) * radius1;
		p.y = sinf(_ring._angle[dest_parent] ) * radius1;	

		d_tmp = p + _centre;	

		//use point closer to the start and ending point to avoid the curve become too close to the center				
		src_tmp.push_back(s_tmp);
		dest_tmp.push_back(d_tmp);
		
		--layer_index;
		if(layer_index <= 0)
		{
			/*radius = tr->_ring[0]._radius[0]/4;	
			float start=tr->_ring[_layer]._limits[si].first, endA=tr->_ring[_layer]._limits[ni].second;
			if(endA<start)
				endA=Pi2+end;

			angle = (start + end)/2;
			if(angle >= Pi2)
				angle = angle-Pi2;

			p.x = radius * cosf(angle) + _centre.x;
			p.y = radius * sinf(angle) + _centre.y;
			*/
			//sometimes, using center point make unneccesary curve
			//src_tmp.push_back(p);  	

			//src_tmp.push_back(_centre);  	
			break;
		} 		
		source_parent = tr->GetParentIndex(layer_index, source_parent);
		dest_parent   = tr->GetParentIndex(layer_index, dest_parent);
	}

	if(source_parent == dest_parent )
	{
		if(tr->_ring[layer_index-1]._radial_pos.size() > source_parent)
		src_tmp.push_back(Point((tr->GetRadialPos(layer_index-1, source_parent)).x, 
							(tr->GetRadialPos(layer_index-1, source_parent)).y));	
		
	}	
	//src_tmp.push_back(_centre);  	

	//src_tmp.push_back(p);  
	
	int num_control_points = src_tmp.size() + dest_tmp.size();

	if(num_control_points == 1)
	{
		curve->add(src_tmp[0].x, src_tmp[0].y);
		curve->add(nx, ny); 
		return;
	}

	Point &p0 = src_tmp[0];
	Point &pn = dest_tmp.empty() ? src_tmp.back() : dest_tmp[0];
	int i;

	for(i = 0; i < src_tmp.size(); ++i)
	{
		//src_tmp[i] = src_tmp[i] * beta + (p0 + (pn-p0) * (1.0*i/(num_control_points-1))) * (1 - beta);
		curve->add(src_tmp[i].x, src_tmp[i].y);
	}

	for(int j = dest_tmp.size()-1; j >= 0; --j, ++i)
	{
		//dest_tmp[j] = dest_tmp[j] * beta + (p0 + (pn-p0) * (1.0*i/(num_control_points-1))) * (1 - beta);
		curve->add(dest_tmp[j].x, dest_tmp[j].y);
	}
	curve -> add(nx, ny);
}


void BundlesPrimitive::createEdgeTable()
{
	//  build the curve for each edge originated from a node
	int  ec=0; // edge count;

	_nodeToEdgeTable.push_back(0); // the first one starts from 0
	
	//vector<int> emptylist;
	//_edgeToNodeTable.push_back(emptylist);

	for(int i=0; i<_nn-1; i++)
	{
		int outNode_c = _out_list[i].size();			
        ec += (outNode_c-1);
		_nodeToEdgeTable.push_back(ec);
	}
	_nodeToEdgeTable.push_back(ec); // the last one is the same as the second last one
	//_edgeToNodeTable.push_back(emptylist);
	//_edgeToNodeTable.resize(ec+1);

	int ecount=0;
	if(ec>0)
	{
		_edgeToNodeTable.resize(ec);
		for(int i=0; i<_nn-1; i++)
		{
			set<int> nodelist;		
			for(int j=1; j<_out_list[i].size(); j++)
			{
				_edgeToNodeTable[ecount].insert(_out_list[i][0]);
				_edgeToNodeTable[ecount].insert(_out_list[i][j]);
				ecount++;
			}		
		}
	}
	if(ecount!=ec)
	{
	    //error;
	}
}


BundlesPrimitive::~BundlesPrimitive()
{
}


void BundlesPrimitive::Render(QPainter *painter)
{
	
	int nr_edges = bun.size(); 

	//printf("bs=%d\n", bs);
	// size of the bundles - for now, this is the # of strokes
	//printf("_hl_fromTo =%d %d\n", _hl_from, _hl_to);
	QPen pen;	
	//pen.setWidth(2);
	
	
#if 1
	//glLineWidth(1.);
	painter->save();
	glLineWidth(1);
	for(int i=0; i<nr_edges; i++)
	{
		if((i<_hl_from || i>_hl_to))
		{
			bun[i].render(painter);		
		};  // end if(i<..)
	}; // end for(i)

	// render the highlighted ones last 
	// so they will stand out
	//glLineWidth(3.);
		
	//pen.setWidth(2);
	QColor color=MIDIUM_HIGHLIGHTCOLOR;
	pen.setColor(QColor( color.red(), color.green(), color.blue(), color.alpha() ));
	painter->setPen( pen );
	painter->drawLine(0,1,0,1);
	glLineWidth(2);
	if(_hl_from>=0)
	{
		for(int hi=_hl_from; hi<_hl_to && hi < bun.size(); hi++)
		{
			 bun[hi].render(painter);				
		}; // end for(hi)
	}; // end if(_hl..)
	painter->restore();
	#endif
}

void BundlesPrimitive::GetEdgeIndexByLayer(int layer, int nodeIndex[2], int edgeIndex[2])
{
	edgeIndex[0] = _nodeToEdgeTable[nodeIndex[0]];
	edgeIndex[1] = _nodeToEdgeTable[nodeIndex[1]];
}

void BundlesPrimitive::Highlight(int from, int to)
{
	_hl_from = from;
	_hl_to = to;

	for(int hi=0; hi<bun.size(); hi++)
	{
		if(hi>=_hl_from && hi<=_hl_to)
		{
			//bun[hi].SetHighlightColor(/*166, 189, 219, 128*/0, 8, 29, 255);//221
			bun[hi].SetHighlight(true);
		}
		else
		{
			bun[hi].SetHighlight(false);
		}; 
	}
}

void BundlesPrimitive::respondNodes(int from, int to, int layer, vector<int> &respondLayer, vector<int> &respondNode)
{
	if(from == to)
		return;

	set<int> respondNodeSet;
	for(int hi=0; hi<bun.size(); hi++)
	{
		if(hi>=from && hi<=to)
		{		
			if(hi<_edgeToNodeTable.size())
			for(set<int>::iterator it=_edgeToNodeTable[hi].begin(); it!=_edgeToNodeTable[hi].end(); it++)
			{
				respondNodeSet.insert(*it);
				
			}
		}			
	}
	for(set<int>::iterator it=respondNodeSet.begin(); it!=respondNodeSet.end(); it++)
	{
		respondNode.push_back(*it);
		respondLayer.push_back(layer);
	}
	

}

void BundlesPrimitive::getNodeCoordinate(TreeRing *tr, int trLayer, int nodeIndex, float *x, float *y)
{
	if(trLayer != _layer)//_num_layers-1)//
	//if(trLayer != 0) no need to be changed when switching inner ring out since the bun_pos has it
	{
		cout<<"Compute bundles starting point error!\n"<<endl;
		return;
	}
	// 	*x = ((tr->_ring)[trLayer]._node_pos)[nodeIndex].x;
	// 	*y = ((tr->_ring)[trLayer]._node_pos)[nodeIndex].y;

	if(((tr->_ring)[trLayer]._bun_pos).size()<=nodeIndex)
	{
		 cout << "nodeIndex exceed _bun_pos.size()" <<endl;
	     return;
	}

 	*x = ((tr->_ring)[trLayer]._bun_pos)[nodeIndex].x;
 	*y = ((tr->_ring)[trLayer]._bun_pos)[nodeIndex].y;

	// DEBUG
#if 0
	double unit_angle=2. * M_PI / (double) _nn;
	//printf("nodeIndex=%d\n", nodeIndex);

	float tmpx, tmpy, t2x, t2y;
	tmpx = *x - _ww/2.;
	tmpy = *x - _ww/2.;

	t2x= _rr * cos ( nodeIndex * unit_angle );
	t2y = _rr * sin ( nodeIndex * unit_angle );

	printf("nodeIndex: %d: (%f %f) (%f %f) (%f %f)\n", nodeIndex,
		*x, *y, tmpx, tmpy, t2x, t2y);
#endif
}

void BundlesPrimitive::UpdateAllBundles(int handle_layer, int handle_index, int handlet, TreeRing *tr)
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
				bun[ _nodeToEdgeTable[i] + j - 1 ].UpdateControl(0, p);
			}	
		}
	}
    cerr<<"SDF"<<endl;
}

void BundlesPrimitive::UpdateBundles(int highlight_layer, int highlight_index, TreeRing *tr)
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
			bun[ _nodeToEdgeTable[i] + j - 1 ].UpdateControl(0, p);
		}
	
	}
    cerr<<"SDF"<<endl;
}


Stroke &BundlesPrimitive::GetStroke(int src_id, int dst_id)
{
	int id = 0;
	for(int i=0; i<_out_list[src_id].size(); ++i)
	{
		if(_out_list[src_id][i] == dst_id)
		{
			id = i;
			break;
		}
	}

	int node_index[2], from_to[2];
	node_index[0] = src_id;
	node_index[1] = dst_id;
	GetEdgeIndexByLayer(2, node_index, from_to);

	return bun[from_to[0] + id];
}

/*void BundlesPrimitive::CalModel()
{
	for(int ei = 0; ei <bun.size(); ++ei)
	{
		int si = src_id[ei];
		int di = dst_id[ei];

		//Get the 18 values of the two parameters
		vector<double> x = _sim_data->GetElem(si);
		vector<double> y = _sim_data->GetElem(di);

		//Linear regression to find three candidate models
		double coeff[3][10];
		for(int i = 0; i < 3; ++i)
		{
			//Polynomial, Quadratic, Cubic Polynomial, here i+2 is for the convinient calculation
 			CalEquation(x, y, i + 2, coeff[i]);
		}

		//Use AIC to find a best model
		double sq_sum[3], aic[3];
		for(int i = 0; i < 3; ++i)
		{
			sq_sum[i] = CalSqSumResidual(x, y, coeff[i], i + 2);
			aic[i] = CalAIC(x.size(), sq_sum[i], i + 2);
		}
		
		//Find the model with the minimum aic value
		double value = FLT_MAX;
		int model_id = -1;
		for(int i = 0; i < 3; ++i)
		{
			if(value > aic[i])
			{
				value = aic[i];
				model_id = i;
			}
		}

		//Finally store the model coefficient
		vector<double> v;
		for(int i = 0; i < coeff[model_id][0]; ++i)
			v.push_back(coeff[model_id][i+1]);

		_coefficient.push_back(v);
	}
}*/

void BundlesPrimitive::CalEquation(vector<double> &dx, vector<double> &dy, int ex, double coefficient[])
{
	double Em[10][10];
	int n = dx.size(); //number of vector

	//coefficient[0] store the number of coefficients
	coefficient[0] = ex;

	for(int i=1;i<=ex;i++)
	{

		for(int j=1;j<=ex;j++)
		{
			Em[i][j] = 0;
			for(int k=0; k<n; ++k) 
				Em[i][j] += pow(dx[k], i+j-2);

		}

		Em[i][ex+1] = 0;
		for(int k=0; k<n; ++k)
			Em[i][ex+1] += pow(dx[k], i-1) * dy[k];

	}
	Em[1][1]=n;


	for(int k=1;k<ex;k++) //消元过程
	{
		for(int i=k+1;i<ex+1;i++)
		{
			double p1=0;

			if(Em[k][k]!=0)
				p1=Em[i][k]/Em[k][k];

			for(int j=k;j<ex+2;j++) 
				Em[i][j]=Em[i][j]-Em[k][j]*p1;
		}
	}

	coefficient[ex]=Em[ex][ex + 1] / Em[ex][ex];
	for(int l=ex-1;l>=1;l--)   //回代求解
	{
		double sum = 0;
		for(int i = l + 1; i <= ex; ++i)
			sum += Em[l][i] * coefficient[i];

		coefficient[l]=(Em[l][ex + 1] - sum) / Em[l][l];
	}
}

double BundlesPrimitive::CalSqSumResidual(vector<double> &array_x, vector<double> &array_y, double *coeff, int ex)
{
	int n = array_x.size();
	double sq_sum = 0;
	double f = 0;

	for(int i=0; i<n; ++i)
	{
		f=0;
		for(int e=1; e<=ex; ++e)
			f += coeff[e] * pow(array_x[i], e);

		sq_sum += (f - array_y[i]) * (f - array_y[i]);
	}

	return sq_sum;
}

double BundlesPrimitive::CalAIC(int n, double residual, double k)
{
	return n * log(residual) + 2 * k;
}
