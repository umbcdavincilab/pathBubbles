

#include "BundlesWH.h"

BundlesWH::BundlesWH(TreeRing *tr, char  *inf, int numOfNode, int numOfChangedNode, float ringRadius, int w, int h, vector <int> nc) : BundlesPrimitive(tr, inf, numOfNode, ringRadius, w, h)
{
  _nc = numOfChangedNode;
  _drawCpIndex=0;
}

void BundlesWH::DrawBSplineControlPoint(int index)
{
  glBegin(GL_POINTS);

  glColor3ub(255, 0, 0);
  glPointSize(10);
  glVertex2i(_cp[index].x, _cp[index].y);
  glEnd();
}

void BundlesWH::setWeightedCP(vector<Point> nodePos, vector <int> nodeSize,
               vector <double>  cv, vector <int> binCat, vector <double> bins, double rr, double rw,
			   double tranx, double trany)
{
  // find the mid point of each categories
  int nss = nodeSize.size();  // find out the number of groups
  int oldParSize = 0, curParSize=0;
  int binsize =  bins.size();
  int counter=0; // debug

  cerr << "node size: " <<  nss << " bin size: " << binsize << endl;
  cerr << "------- the CV bins -------" << endl;
  for(int bi=0; bi<binsize; bi++)
  {
    cerr << bi << ": " << bins[bi] << endl;
  }

  for(int nsi=0; nsi<nss; nsi++)
  {
    // the numbers in the current group
	curParSize += nodeSize[nsi];
    cerr << "curParSize: " << curParSize << endl;

	// get the mid-point between the first and the last position
	// then the rest of the points should be on this line
	Point midPoint = ( nodePos[oldParSize] + nodePos[curParSize-1] ) * 0.5; 
	double dis = sqrt((tranx-midPoint.x)*(tranx-midPoint.x)+(trany-midPoint.y)*(trany-midPoint.y));

    cerr << "start: " << nodePos[oldParSize].x << " " << nodePos[oldParSize].y << endl;
	cerr << "midPoint : " << midPoint.x << " " << midPoint.y <<  " dis to center: " << dis << endl;
    cerr << "end: " << nodePos[curParSize-1].x << " " << nodePos[curParSize-1].y << endl;

	// local control points along the line
	//  start from the midpoint and move towards the center
	//  by "rw" in each time step
	cerr << "---> bins: " << endl;
	vector <Point> lcp;  
    for(int bini = 0; bini<binsize; bini++)
	{
	  Point lp;
	  double cr = dis- (bini+1) *rw; // distance to the center

	  lp.x = (midPoint.x - tranx) / dis * cr + tranx;
	  lp.y = (midPoint.y - trany) / dis * cr + trany;

	  cerr << "  " << bini << ": " << lp.x <<" " << lp.y << endl;
	  lcp.push_back(lp);
	}; // end for(bini)

	// calculate all other points on this line
	for(int cpsi = oldParSize; cpsi < curParSize; cpsi++)
	{
	  Point cp;
	  cp .x= lcp[binCat[cpsi]].x+ 
	      cv[cpsi]*(lcp[binCat[cpsi]+1].x-lcp[binCat[cpsi]].x)/(bins[binCat[cpsi] +1]-bins[binCat[cpsi]]);
	  cp .y= lcp[binCat[cpsi]].y+
	      cv[cpsi]*(lcp[binCat[cpsi]+1].y-lcp[binCat[cpsi]].y)/(bins[binCat[cpsi] +1]-bins[binCat[cpsi]]);
	  _cp.push_back(cp);
	  cerr << "===> counter " << counter << ": " << cv[cpsi] << " bin: " 
	             << bins[binCat[cpsi]] << "  "<< bins[binCat[cpsi]+1] << "binCat: " << binCat[cpsi] <<  endl;
	  cerr << "===> counter " << counter << ": " << cp.x << " " << cp.y << endl;
	  counter++;
	}; // end for(cpsi)

	lcp.clear();

	oldParSize = curParSize; // move the pointer
  }; // end for(nsi)
}

void BundlesWH::Print()
{
  int cps = _cp.size();
  cerr << "=============== _CP ==============" << endl;
  for(int i=0; i<cps; i++)
  {
    cerr << i << ": " << _cp[i].x << " " << _cp[i].y << endl;
  }; 
}



//
// first test if they blong to the same sub tree
// if yes, skip this edge, and return 
// if no, test if the parent belong to the same tree
//   if yes, add the two parent nodes
//   if not, add the two parent and the two grandparents nodes
// 
vector <Point> BundlesWH::getCtrlPoints(TreeRing *tr,
               vector <double>  cv, vector <int> binCat, vector <double> bins, double rr, double rw,
			   double tranx, double trany, int fromIndex, int toIndex)
{

#define OFFSET 60
  vector <Point> result;

  int nss = ((tr->_ring[2])._node_size).size();  
  // find out the number of nodes 

  int oldParSize = 0, curParSize=0;
  int binsize = bins.size();
  int counter=0;  // debug

  Point fromPa, toPa, fromPa0, toPa0;

  int fromParentIndex = tr->GetParentIndex(2, fromIndex);
  int toParentIndex = tr->GetParentIndex(2, toIndex);

  cerr <<"fromTo: "<<fromIndex<<" "<<fromParentIndex<<" "<<toIndex<<" "<<toParentIndex<<endl;

  if(fromParentIndex!=toParentIndex)
  {
	fromPa = tr->CalcParentNodePosAtR(2, fromParentIndex, &fromParentIndex, rr-OFFSET);
	toPa = tr->CalcParentNodePosAtR(2, toParentIndex, &fromParentIndex, rr-OFFSET);

	result.push_back(fromPa);

	int fromParentIndex0 = tr->GetParentIndex(1, fromParentIndex);
	int toParentIndex0 = tr->GetParentIndex(1, toParentIndex);

    if(fromParentIndex0 != toParentIndex0)
	{
	  fromPa0 = tr->CalcParentNodePosAtR(2, fromParentIndex, &fromParentIndex, rr-OFFSET);
	  toPa0 = tr->CalcParentNodePosAtR(2, toParentIndex, &fromParentIndex, rr-OFFSET);

	  result.push_back(fromPa0);
	  result.push_back(toPa0);
	}
	else
	{
	  fromPa0 = tr->CalcParentNodePosAtR(2, fromParentIndex, &fromParentIndex, rr-OFFSET);
	  result.push_back(fromPa0);
	}

	result.push_back(toPa);
	// find the next parent

	return result;
  }
  else
  {
	fromPa = tr->CalcParentNodePosAtR(2, fromParentIndex, &fromParentIndex, rr-OFFSET);
	toPa = fromPa;
	result.push_back(fromPa);
  }; // end if(fromParentIndex)

  return result;

}



// add control points to form a stroke
// let's use four nodes first -
// the first and the last points coordinate is the ring node on the treeRing 
void BundlesWH::GenerateCurveBundles(TreeRing *tr, vector <double>  cv, 
        vector <int> binCat, vector <double> bins, double rr, double rw, double tranx, double trany)
{
  //ENABLE_DRAW_CHANGED = true;
  ENABLE_DRAW_CHANGED = false;

  cerr << "-- generate curve bundles --- " << endl;

  // find where to insert the control points

  //for(int i=0; i<_nn; i++) 
  // TODO: did not save the last one - silly me! need to fix the 
  // graph reading prog.. 
  for(int i=0; i<_nn-1; i++) 
  {
	int sumnc = (_out_list[i]).size();
	float  sx, sy;

	if(sumnc>1) // has edge out
	{
	  sx = ((tr->_ring)[2]._node_pos)[i].x;
	  sy = ((tr->_ring)[2]._node_pos)[i].y;
	  //sy = nodePos[i].y;
      //printf("node: %d: %f %f\n", i, sx, sy);

	  // add the stroke - start from 1, since 0 is the index of the source node
	  for(int nc=1; nc<sumnc; nc++)
	  {
        Stroke  curve;
		vector <Point> ctrlPoints;
	    float  nx, ny;

        //printf("out node  %d\n", outNodeList[nc]);
	    // get the source / target coord 
	    //nx = nodePos[  _out_list[i][nc] ].x;
	    //ny = nodePos[  _out_list[i][nc] ].y;
	    nx = ((tr->_ring)[2]._node_pos)[ _out_list[i][nc] ].x;
	    ny = ((tr->_ring)[2]._node_pos)[ _out_list[i][nc] ].y;

// find out the control point locations based on the from/to nodes
ctrlPoints = getCtrlPoints(tr, cv, binCat, bins, rr, rw, tranx, trany, i,  _out_list[i][nc] );

	    // find the control points based on the Danny Holten paper 
	    // hierarchical edge bundles
		
	    //insertHoltenPoints(&curve, _out_list[i][0], sx, sy, _out_list[i][nc], nx, ny);
		curve.add(sx, sy);
		for(int cpi=0; cpi < ctrlPoints.size(); cpi++)
		{
		  curve.add((ctrlPoints[cpi]).x, (ctrlPoints[cpi]).y);
		}
		curve.add(nx, ny);


		if( (binCat[i]>0) &&  (binCat[_out_list[i][nc]]>0) ) 
		{
		  _changed.push_back(true);
		}
		else _changed.push_back(false);

        bun.push_back(curve);
	  }; // end for(nc)

	}; // end for(sumc)
  }; // end for(i)
}

void BundlesWH::UpdateCurveBundles()
{
}

void BundlesWH::Render()
{
  int bs = bun.size(); 
  //printf("bs=%d\n", bs);
  // size of the bundles - for now, this is the # of strokes
  //printf("_hl_fromTo =%d %d\n", _hl_from, _hl_to);

  //DrawBSplineControlPoint(_drawCpIndex);

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


