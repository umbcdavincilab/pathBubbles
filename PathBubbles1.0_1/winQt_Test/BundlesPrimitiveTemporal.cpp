

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include "BundlesPrimitiveTemporal.h"


#define HOLTEN_POINTS 4


BundlesPrimitiveTemporal::BundlesPrimitiveTemporal(TreeRing *tr, char *inf, int numOfNode, float ringRadius, int w, int h)
{
  ENABLE_DRAW_CHANGED = false;
  // read the graph information
  int num_nodes = tr->GetNumOfNodes();
  GraphIndexParser2 *gip = new GraphIndexParser2(inf, num_nodes);
  int ne = gip->GetNumOfEdges(); // get the number of edges

  _rr = ringRadius;
  _nn = numOfNode;
  _ww = w;
  _wh = h;

  _hl_from = -1;
  _hl_to = -1;

  // create the _out_list;
  for(int i=0; i<_nn; i++)
  {
    vector <int> outNodeList = gip->GetOutNodeIndex(i);
  // we know all nodes pointing out from the current node
  // from the adjacency list. these are the node index
  // for each node (i)
    _out_list.push_back(outNodeList);
  }; 
  delete gip;

  createEdgeTable();
  GenerateCurveBundles(tr);

}

BundlesPrimitiveTemporal::BundlesPrimitiveTemporal(TreeRing *tr, char *inf, int numOfNode, int numOfChangedNode, float ringRadius, 
                   int w, int h, vector <int> nc)
{
  ENABLE_DRAW_CHANGED = true;
  // read the graph information
  GraphIndexParser2 *gip = new GraphIndexParser2(inf);
  int ne = gip->GetNumOfEdges(); // get the number of edges

  _rr = ringRadius;
  _nn = numOfNode;
  _nc = numOfChangedNode;
  _ww = w;
  _wh = h;

  _hl_from = -1;
  _hl_to = -1;

  // create the _out_list;
  for(int i=0; i<_nn; i++)
  {
    vector <int> outNodeList = gip->GetOutNodeIndex(i);
  // we know all nodes pointing out from the current node
  // from the adjacency list. these are the node index
  // for each node (i)
    _out_list.push_back(outNodeList);
  }; 
  delete gip;

  createEdgeTable();
  GenerateCurveBundles(tr, nc);
}


// add control points to form a stroke
// let's use four nodes first -
// the first and the last points coordinate is the ring node on the treeRing 
void BundlesPrimitiveTemporal::GenerateCurveBundles(TreeRing *tr)
{
  ENABLE_DRAW_CHANGED = false;

  //for(int i=0; i<_nn; i++) 
  // TODO: did not save the last one - silly me! need to fix the 
  // tree reading prog.. 
  for(int i=0; i<_nn-1; i++) 
  {
	int sumnc = (_out_list[i]).size();
	float  sx, sy;

	if(sumnc>1) // has edge out
	{
	  getNodeCoordinate(tr, 2, i, &sx, &sy);
      //printf("node: %d: %f %f\n", i, sx, sy);

	  // add the stroke - start from 1, since 0 is the index of the source node
	  for(int nc=1; nc<sumnc; nc++)
	  {
        Stroke  curve;
	    float  nx, ny;

        //printf("out node  %d\n", outNodeList[nc]);
	    // get the source / target coord 
	    getNodeCoordinate(tr, 2, _out_list[i][nc], &nx, &ny);

	    // find the control points based on the Danny Holten paper 
	    // hierarchical edge bundles
	    insertHoltenPoints(&curve, _out_list[i][0], sx, sy, _out_list[i][nc], nx, ny);

        bun.push_back(curve);
	  }; // end for(nc)
	}; // end for(sumc)
  }; // end for(i)
}


// add control points to form a stroke
// let's use four nodes first -
// the first and the last points coordinate is the ring node on the treeRing 
void BundlesPrimitiveTemporal::GenerateCurveBundles(TreeRing *tr, vector <int> node_changed)
{
  ENABLE_DRAW_CHANGED = true;

  //for(int i=0; i<_nn; i++) 
  // TODO: did not save the last one - silly me! need to fix the 
  // tree reading prog.. 
  for(int i=0; i<_nn-1; i++) 
  {
	int sumnc = (_out_list[i]).size();
	float  sx, sy;

	if(sumnc>1) // has edge out
	{
	  getNodeCoordinate(tr, 2, i, &sx, &sy);
      //printf("node: %d: %f %f\n", i, sx, sy);

	  // add the stroke - start from 1, since 0 is the index of the source node
	  for(int nc=1; nc<sumnc; nc++)
	  {
        Stroke  curve;
	    float  nx, ny;

        //printf("out node  %d\n", outNodeList[nc]);
	    // get the source / target coord 
	    getNodeCoordinate(tr, 2, _out_list[i][nc], &nx, &ny);

	    // find the control points based on the Danny Holten paper 
	    // hierarchical edge bundles
	    insertHoltenPoints(&curve, _out_list[i][0], sx, sy, _out_list[i][nc], nx, ny);

		if(node_changed[i] &&  node_changed[_out_list[i][nc]]) 
		{
		  _changed.push_back(true);
		}
		else _changed.push_back(false);

        bun.push_back(curve);
	  }; // end for(nc)
	}; // end for(sumc)
  }; // end for(i)
}


//void BundlesPrimitiveTemporal::ReplaceCurve(int index, float start[2], float end[2])
//{
//}

void BundlesPrimitiveTemporal::createEdgeTable()
{
  //  build the curve for each edge originated from a node
  int  ec=0; // edge count;
  _nodeToEdgeTable.push_back(0); // the first one starts from 0

  for(int i=0; i<_nn-1; i++)
  {
	int outNode_c = _out_list[i].size();
	// count the out node

	ec += (outNode_c-1);
	_nodeToEdgeTable.push_back(ec);
  }//end for(i)
  _nodeToEdgeTable.push_back(ec); // the last one is the same as the second last one
}


BundlesPrimitiveTemporal::~BundlesPrimitiveTemporal()
{
}

void BundlesPrimitiveTemporal::Render(int index)
{
  int bs = bun.size(); 
  bun[index].render();
  bun[index].drawControl(CP);
}

void BundlesPrimitiveTemporal::Render()
{
  int bs = bun.size(); 
  //printf("bs=%d\n", bs);
  // size of the bundles - for now, this is the # of strokes
  //printf("_hl_fromTo =%d %d\n", _hl_from, _hl_to);

#if 1
  glLineWidth(1.);
  for(int i=0; i<bs; i++)
  {
    if(i<_hl_from || i>_hl_to)
    {
	  if(!ENABLE_DRAW_CHANGED)
        bun[i].render();
	  else
	  {
	    if(_changed[i])
		{
          bun[i].render();
		}
		else
		{
		  //glColor4ub(153, 216, 201, 0.5);
          bun[i].render(Color(153, 216, 201, 0.5));
		}; // end if(_changed)
	  }; // end if(ENABLE_DRAW_CHANGED)
	};  // end if(i<..)
  }; // end for(i)

  // render the highlighted ones last 
  // so they will stand out
  glLineWidth(3.);
  if(_hl_from>=0)
  {
    for(int hi=_hl_from; hi<_hl_to; hi++)
    {
	  if(!ENABLE_DRAW_CHANGED)
	  {
        bun[hi].render();
	  }
	  else
	  {
	    if(_changed[hi])
		{
          bun[hi].render();
		}
		else
		{
		  //glColor4ub(153, 216, 201, 0.8);
          //bun[hi].render();
          bun[hi].render(Color(153, 216, 201, 0.8));
		};  // end if(_changed)
	  }; // end if(ENABLE_...)
    }; // end for(hi)
  }; // end if(_hl..)
#endif
}

void BundlesPrimitiveTemporal::GetEdgeIndexByLayer(int layer, int nodeIndex[2], int edgeIndex[2])
{
   edgeIndex[0] = _nodeToEdgeTable[nodeIndex[0]];
   edgeIndex[1] = _nodeToEdgeTable[nodeIndex[1]];
}

void BundlesPrimitiveTemporal::Highlight(int from, int to)
{
  _hl_from = from;
  _hl_to = to;

  for(int hi=0; hi<bun.size(); hi++)
  {
    if(hi>=_hl_from && hi<=_hl_to)
    {
    	bun[hi].SetStrokeColor(166, 189, 219, 128);
    }
    else
    {
    	bun[hi].SetStrokeColor(DEFAULT_R, DEFAULT_G, DEFAULT_B, DEFAULT_A);
    }; 
  }
}



#define  H_RADIUS  20

void BundlesPrimitiveTemporal::insertHoltenPoints(Stroke *curve, int si, float sx, float sy, 
                                                int ni, float nx, float ny)
{
  curve->setEnableLineColor(true);
  curve -> add( sx, sy);

  // settle to define a square that is slightly smaller than
  // the smallest ring radius
  float insert_x[2],insert_y[2];
  insert_x[0] = _rr * 0.7;
  insert_x[1] = -1. * _rr * 0.7;
  insert_y[0] = _rr * 0.7;
  insert_y[1] = -1. * _rr * 0.7;

  // add the first control points
  float  sca;
  float  nca;
  //
  // this causes the skewed curves 
  // need to update the location
  /*
  if(ENABLE_DRAW_CHANGED)
  {
    sca = (float)si / (float)_nc;
    nca = (float)ni / (float)_nc;
	cerr << "EEEEEEEEEEEEEEEEEEEEEEEEEEEE" <<  _nc << endl;
  }
  else
  */
  {
    sca = (float)si / (float)_nn;
    nca = (float)ni / (float)_nn;
	//cerr << "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA" << endl;
  }; // end if(ENABLE..)

  if(sca < 0.125 || sca > 0.875)  // first quadrant
  {
    curve -> add ( insert_x[0]+ (float)_ww/2.,  (float)_wh/2.);
  }
  else if(sca >=0.125 && sca <= 0.375)
  {
    curve -> add (  (float)_ww/2., insert_y[0]+ (float)_wh/2. );
  }
  else if(sca >= 0.375 && sca <= 0.625)
  {
    curve -> add ( insert_x[1]+ (float)_ww/2.,  (float)_wh/2.);
  }
  else if(sca >= 0.625 && sca <= 0.875)
  {
    curve -> add (  (float)_ww/2., insert_y[1]+ (float)_wh/2. );
  }; // end if(sca)

// repeat for the ending point
  if(nca < 0.125 || nca > 0.875)  // first quadrant
  {
    curve -> add ( insert_x[0]+ (float)_ww/2.,  (float)_wh/2.);
  }
  else if(nca >=0.125 && nca <= 0.375)
  {
    curve -> add (  (float)_ww/2., insert_y[0]+ (float)_wh/2. );
  }
  else if(nca >= 0.375 && nca <= 0.625)
  {
    curve -> add ( insert_x[1]+ (float)_ww/2.,  (float)_wh/2.);
  }
  else if(nca >= 0.625 && nca <= 0.875)
  {
    curve -> add (  (float)_ww/2., insert_y[1]+ (float)_wh/2. );
  }; // end if(nca)

  //curve -> add(nx+ (float)_ww/2., ny+ (float)_wh/2.);
  curve -> add(nx, ny);
}



#if 0
void BundlesPrimitiveTemporal::insertWeightedHoltenPoints(Stroke *curve, 
               int si, float sx, float sy, 
               int ni, float nx, float ny)
{
  curve->setEnableLineColor(true);
  curve -> add( sx, sy);

  // settle to define a square that is slightly smaller than
  // the smallest ring radius
  float insert_x[2],insert_y[2];
  insert_x[0] = _rr * 0.7;
  insert_x[1] = -1. * _rr * 0.7;
  insert_y[0] = _rr * 0.7;
  insert_y[1] = -1. * _rr * 0.7;


// add the first control points
  float  sca = (float)si / (float)_nn;
  float  nca = (float)ni / (float)_nn;
  if(sca < 0.125 || sca > 0.875)  // first quadrant
  {
    curve -> add ( insert_x[0]+ (float)_ww/2.,  (float)_wh/2.);
  }
  else if(sca >=0.125 && sca <= 0.375)
  {
    curve -> add (  (float)_ww/2., insert_y[0]+ (float)_wh/2. );
  }
  else if(sca >= 0.375 && sca <= 0.625)
  {
    curve -> add ( insert_x[1]+ (float)_ww/2.,  (float)_wh/2.);
  }
  else if(sca >= 0.625 && sca <= 0.875)
  {
    curve -> add (  (float)_ww/2., insert_y[1]+ (float)_wh/2. );
  }; // end if(sca)

// repeat for the ending point
  if(nca < 0.125 || nca > 0.875)  // first quadrant
  {
    curve -> add ( insert_x[0]+ (float)_ww/2.,  (float)_wh/2.);
  }
  else if(nca >=0.125 && nca <= 0.375)
  {
    curve -> add (  (float)_ww/2., insert_y[0]+ (float)_wh/2. );
  }
  else if(nca >= 0.375 && nca <= 0.625)
  {
    curve -> add ( insert_x[1]+ (float)_ww/2.,  (float)_wh/2.);
  }
  else if(nca >= 0.625 && nca <= 0.875)
  {
    curve -> add (  (float)_ww/2., insert_y[1]+ (float)_wh/2. );
  }; // end if(nca)

  //curve -> add(nx+ (float)_ww/2., ny+ (float)_wh/2.);
  curve -> add(nx, ny);
}
#endif



void BundlesPrimitiveTemporal::getNodeCoordinate(TreeRing *tr, int trLayer, int nodeIndex, float *x, float *y)
{
  *x = ((tr->_ring)[trLayer]._node_pos)[nodeIndex].x;
  *y = ((tr->_ring)[trLayer]._node_pos)[nodeIndex].y;

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

