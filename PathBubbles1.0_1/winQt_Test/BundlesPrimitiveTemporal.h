
#ifndef __BUNDLE_H
#define __BUNDLE_H


#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include "TreeRing.h"
#include "Stroke.h"
#include "parser/GraphIndexParser2.h"

// TODO: this function needs to inheriate BundlesPrimtive

class BundlesPrimitiveTemporal
{
public:
  BundlesPrimitiveTemporal(TreeRing *tr, char  *inf, int numOfNode, float ringRadius, int w, int h);
  // build the tree first.. then we know numOfNode

  BundlesPrimitiveTemporal(TreeRing *tr, char  *inf, int numOfNode, int numOfChangedNode, float ringRadius, 
             int w, int h, vector <int> nc);

  virtual ~BundlesPrimitiveTemporal();

  virtual void Render();     
  virtual void Render(int index);

  virtual void Highlight(int from, int to);
  // in:  from , 
  //      to   nodeIndex range
  // need to find the stroke indices first;

  virtual void GenerateCurveBundles(TreeRing *tr);
  // in: tr (TreeRing data)
  // 
  // delete old curves / generate spline curves

  virtual void GenerateCurveBundles(TreeRing *tr, vector <int> nc);
  // in: tr (TreeRing data)
  //     nc (node varied with time or not)
  // delete old curves / generate spline curves


  virtual void GetEdgeIndexByLayer(int layer, int nodeIndex[2], int edgeIndex[2]);
  // in:  layer (0-2 layer of the ring)
  //      nodeIndex 
  // out: edgeIndex  (the real edges in bun at layer2) 
  //      obtained from the adjacency list

  virtual vector <int> GetOutNodeListByIndex(int nodeIndex)
  {
    assert(nodeIndex < _out_list.size());
	if(nodeIndex == _out_list.size())
      return _out_list[nodeIndex-1];
    return _out_list[nodeIndex];
  };


protected:
  virtual void getNodeCoordinate(TreeRing *tr, int trLayer, int nodeIndex, float *x, float *y);
  virtual void insertHoltenPoints(Stroke *curve, int si, float sx, float sy, int ni, float nx, float ny);
  //virtual void insertWeightedHoltenPoints(Stroke *curve, int si, float sx, float sy, int ni, float nx, float ny);
  virtual void createEdgeTable();
  // create the _nodeToEdgeTable 
  //   must create the _in_list or _out_list first
  //   before calling this routine
  // TODO: error checking


  /////////////////////////////////////
  // PARAMETERS
  /////////////////////////////////////

  //vector < vector <Stroke> >  bun;
  vector <Stroke> bun;

///////// from the graph representation
  vector <bool>  _changed;
  // the edge comes from a node that is changing with time or not
  //  true:  changing
  //  false: not changing

  bool ENABLE_DRAW_CHANGED;

  vector< vector <int> > _in_list;  
  // NOT IMPLEMENTED
  // save the source nodes; the list a node
  // is pointed towards.

  vector< vector <int> > _out_list; 
  // save the destination nodes; the list a node
  // is pointing to.
  // contains _nn of nodes

  vector <int>   _nodeToEdgeTable;
  // a look up table that saves the starting edge index of a node
  
///////// drawing parameters
  float  _rr;  // tree node ring radius 

  int    _nc;  // number of changed leaf nodes
  int    _nn;  // number of tree leaf node
  int    _ww;  // window width
  int    _wh;  // window height

  int  _hl_from;  // node from  
  int  _hl_to;    // node to

  Color  _hl_c; // highlight color
  Color  _c;    // regular display
};


#endif //__BUNDLE_H
