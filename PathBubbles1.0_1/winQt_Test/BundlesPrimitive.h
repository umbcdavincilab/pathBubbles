
#ifndef __BUNDLE_H
#define __BUNDLE_H


#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include "TreeRing.h"
#include "Stroke.h"
#include "Parser/GraphIndexParser2.h"
#include "SimData.h"
#define HOLTEN_POINTS 4
#define FLT_MAX 3.402823466e+38F 

class BundlesPrimitive
{
public:
	int _num_layers;
	int _layer;
	BundlesPrimitive(TreeRing *tr, char  *inf, int numOfNode, int numOfAllNode, int layer, float ringRadius, SimData *simData, int EDGE_BY, Point centre);
	BundlesPrimitive(TreeRing *tr, vector<vector<int>> outlist, vector<vector<int>> inList, int numOfNode, int layer, float ringRadius, SimData *simData, int EDGE_BY, Point centre);
	virtual ~BundlesPrimitive();

	virtual void Render(QPainter *painter);     
	//virtual void Render(QPainter *painter, int index);

	virtual void Highlight(int from, int to);
	virtual void respondNodes(int from, int to, int layer, vector<int> &respondLayer, vector<int> &respondNode);
	void clearHighlight()
	{
		_hl_from = -1;
		_hl_to = -1;
	}
	// in:  from , 
	//      to   nodeIndex range
	// need to find the stroke indices first;

	virtual void GenerateCurveBundles(TreeRing *tr, float bundling_strength);
	virtual void reGenerateCurveBundles(TreeRing *tr, float bundling_strength);
	void UpdateAllBundles(int handle_layer, int handle_index, int handlet, TreeRing *tr);
	// in: tr (TreeRing data)
	// 
	// delete old curves / generate spline curves


	virtual void GetEdgeIndexByLayer(int layer, int nodeIndex[2], int edgeIndex[2]);
	// in:  layer (0-2 layer of the ring)
	//      nodeIndex 
	// out: edgeIndex  (the real edges in bun at layer2) 
	//      obtained from the adjacency list

	virtual vector <int> GetOutNodeListByIndex(int nodeIndex)
	{
		//assert(nodeIndex < _out_list.size());
		if(nodeIndex >= _out_list.size())
			return _out_list[nodeIndex%_out_list.size()];

		if(nodeIndex == _out_list.size())
			return _out_list[nodeIndex-1];

		return _out_list[nodeIndex];
	};

	virtual void UpdateBundles(int highlight_layer, int highlight_index, TreeRing *tr);

	void Scale(float zoom){
		for(int i=0; i<bun.size(); ++i)
		{
			bun[i].TranslateControl(Point(-_centre.x, -_centre.y));
			bun[i].Scale(1+zoom);
			bun[i].TranslateControl(Point(_centre.x, _centre.y));
		}
	}

	void Translate(float x, float y)
	{
		if(_layer==1)
			_layer=_layer;
		for(int i=0; i<bun.size(); ++i)
		{
			bun[i].TranslateControl(Point(x, y));
		}
		_centre.x += x;
		_centre.y += y;
	}

	void moveToCurrentCenter(Point _centre);

	int getHFrom() {return _hl_from; }
	int getHTo() {return _hl_to; }
	//void UpdateShowVariance(double v1, double v2, int TYPE);

	//vector<int> FindPathByVariance(int id, bool out);

	vector<int> &GetOutList(int index){return _out_list[index];}
	
	vector<int> &GetInList(int index){return _in_list[index];}

	Stroke &GetStroke(int src_id, int dst_id);

	int GetNumOfEdges();

	//vector<bool> &GetShow(){return _show;}

	//Find the best model to discribe the change of two parameter
	//void CalModel();

protected:

	void insertHoltenPoints_2(Stroke *curve, TreeRing *tr, int si, float sx, float sy, 
										  int ni, float nx, float ny, float beta);
	virtual void getNodeCoordinate(TreeRing *tr, int trLayer, int nodeIndex, float *x, float *y);
	virtual void insertHoltenPoints(Stroke *curve, TreeRing *tr, int si, float sx, float sy, 
		int ni, float nx, float ny,float beta);
	virtual void createEdgeTable();

	//void ComputeIncomingEdgesOfNodes();

	//------------------------Model calculation and selection-----------------------------

	//Calculate the model equation of two changing parameters
	void CalEquation(vector<double> &dx, vector<double> &dy, int ex, double coefficient[]);

	//Calculate the square sum of residual
	double CalSqSumResidual(vector<double> &array_x, vector<double> &array_y, double *coeff, int ex);

	//Calculate the AIC of a model
	double CalAIC(int n, double residual, double k);

	//-----------------------Model calculation and selection-------------------------------

	// create the _nodeToEdgeTable 
	//   must create the _in_list or _out_list first
	//   before calling this routine
	// TODO: error checking


	/////////////////////////////////////
	// PARAMETERS
	/////////////////////////////////////

	//vector < vector <Stroke> >  bun;
	vector <Stroke> bun;

	vector< vector <int> > _in_list;  
	// NOT IMPLEMENTED
	// save the source nodes; the list a node
	// is pointed towards.

	vector< vector <int> > _out_list; 
	// save the destination nodes; the list a node
	// is pointing to.
	// contains _nn of nodes

	vector <int>   _nodeToEdgeTable; //_nodeToEdgeTable[i] + j - 1 表示结点i连接结点_out_list[i][j]的边的序号，
	vector <set <int>>  _edgeToNodeTable;
	// a look up table that saves the starting edge index of a node

	//A table that saves the index of incoming edges of a node
	//vector< vector<int> >_income_edges_of_node;

	///////// drawing parameters
	float  _rr;  // tree node ring radius 
	double _variance_to_show; //show bundles above variance of _variance_to_show

	int    _nn;  // number of tree leaf node

	int  _hl_from;  // node from  
	int  _hl_to;    // node to



	int _edge_by;

	Color  _hl_c; // highlight color
	Color  _c;    // regular display

	Point _centre;

	SimData *_sim_data;

	//Store the id of two nodes of an edge
	vector<int> src_id, dst_id;

	//The changing model of each edge(each two nodes/parameters)
	vector< vector<double> > _coefficient;

	///////// from the graph representation ////////////// changed parameters
	//vector <bool>  _changed;

	//vector<bool>_show; //variance related
	// the edge comes from a node that is changing with time or not
	//  true:  changing
	//  false: not changing

	int    _nc;  // number of changed leaf nodes

	bool ENABLE_DRAW_CHANGED;
};


#endif //__BUNDLE_H
