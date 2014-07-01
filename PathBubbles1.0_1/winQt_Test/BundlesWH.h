#ifndef __BUNDLE_WH_H
#define __BUNDLE_WH_H



#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include "BundlesPrimitive.h"
#include "TreeRing.h"

class BundlesWH : public BundlesPrimitive
{
public:
  BundlesWH(TreeRing *tr, char  *inf, int numOfNode, int numOfChangedNode, float ringRadius, int w, int h, vector <int> nc);

  virtual ~BundlesWH(){};

  virtual void GenerateCurveBundles(TreeRing *tr, 
                  vector <double>  cv, vector <int> binCat, vector <double> bins, 
				  double rr, double rw, double tranx, double trany);
  // in: nodeSize (TreeRing data size)  - decide how data are grouped
  //     binCat:  bin category of the leaf nodes
  //     bins:    where the data gets splitted
  //
  // out: _weightedCP      (control point position for each parameter
  //         here, we
  //         scale the control points "locally" within each bin.
  //         based on the bin range value and its cv value
  // 
  //         also we group the bundles within each categories together 
  //         thus the points on the same radius lines 
  // 
  // delete old curves / generate spline curves
  // description: read the tree hierachies and find out 
  // 

  void  DrawBSplineControlPoint(int index);

  virtual void UpdateCurveBundles();
  // TODO: Not implemented

  virtual void Print();

  virtual void Render();

  void SetDrawCpIndex(int newIndex) {_drawCpIndex = newIndex;}

  
protected:
  void  setWeightedCP(vector<Point> nodePos, vector <int> nodeSize,
               vector <double>  cv, vector <int> binCat, vector <double> bins, double rr, double rw,
			   double tranx, double trany);

  vector <Point> getCtrlPoints(TreeRing *tr,
               vector <double>  cv, vector <int> binCat, vector <double> bins, double rr, double rw,
			   double tranx, double trany, int fromIndex, int toIndex);


  // control points for "each" point of "each" parameter
  //  build a look up table first to figure out the location of the inserted points
  //  the location will be weighted by the parameter passed in when 
  //  "GenerateCurveBundles() ... e.g., CV.
  vector <Point> _cp;  

  int _drawCpIndex;

};

#endif //__BUNDLE_WH_H
