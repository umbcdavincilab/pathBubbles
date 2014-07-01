
#ifndef __MARKER_H
#define __MARKER_H


#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include "TreeRing.h"
//#include "Stroke.h"
#include "Parser/GraphIndexParser2.h"
#include "SimData.h"
#define HOLTEN_POINTS 4
#define FLT_MAX 3.402823466e+38F 


struct elMarker_str
{
	bool moreLayer;
	int layer,idx;
	float sx,sy,angle;	
};


class ExtraLayerMarker
{
public:
	int _num_layers;
	QString fileName;
	ExtraLayerMarker(TreeRing *tr, char  *inf, int numOfNode, float ringRadius, SimData *simData, Point centre);
	ExtraLayerMarker(TreeRing *tr, vector<bool> markerFlag, int numOfNode, float ringRadius, SimData *simData, Point centre);
	void renderMarker(QPainter *painter, float sx, float sy, float angle, float angle1, float angle2, int size1, int size2, float radius);
	virtual ~ExtraLayerMarker();

	virtual void Render(QPainter *painter);   	
	bool readExtraLayerFile( QString fileName, vector <struct elMarker_str> &marker);
	//virtual void Render(QPainter *painter, int index);

	virtual void Highlight(int hi);
	void clearHighlight()
	{
	    highlightIndex = -1;
	}
	// in:  from , 
	//      to   nodeIndex range
	// need to find the stroke indices first;

	//virtual void GenerateCurveBundles(TreeRing *tr, float bundling_strength);
	//virtual void reGenerateCurveBundles(TreeRing *tr, float bundling_strength);
	void UpdateAllBundles(int handle_layer, int handle_index, int handlet, TreeRing *tr);
	// in: tr (TreeRing data)
	// 
	// delete old curves / generate spline curves
	TreeRing * _treeRing;
	virtual void reGenerateMarkers(TreeRing *tr, vector<bool> markerFlag);
	virtual void updateMarkerAngle(TreeRing *tr);
    virtual void GenerateMarkers(TreeRing *tr);
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

	virtual void UpdateBundles(int highlight_layer, int highlight_index, TreeRing *tr);
	void Scale(float zoom){
		/*for(int i=0; i<bun.size(); ++i)
		{
			bun[i].TranslateControl(Point(-_centre.x, -_centre.y));
			bun[i].Scale(1+zoom);
			bun[i].TranslateControl(Point(_centre.x, _centre.y));
		}*/
		for(int i=0; i<marker.size(); ++i)
		{
			marker[i].sx = marker[i].sx - _centre.x;
			marker[i].sy = marker[i].sy - _centre.y;

			marker[i].sx = marker[i].sx * (1+zoom);
			marker[i].sy = marker[i].sy * (1+zoom);

			marker[i].sx=marker[i].sx + _centre.x;
			marker[i].sy=marker[i].sy + _centre.y;
		}
	}

	void Translate(float x, float y)
	{
		for(int i=0; i<marker.size(); ++i)
		{
			marker[i].sx=marker[i].sx + x;
			marker[i].sy=marker[i].sy + y;
		}
		
		_centre.x += x;
		_centre.y += y;
	}

	bool getMarkerFlag(int i)
	{
	    return marker[i].moreLayer;
	}

	void moveToCurrentCenter(Point _centre);

	//void UpdateShowVariance(double v1, double v2, int TYPE);	
	vector<int> &GetOutList(int index){return _out_list[index];}	
	vector<int> &GetInList(int index){return _in_list[index];}
	//Stroke &GetStroke(int src_id, int dst_id);

	int GetNumOfEdges();
	//vector<bool> &GetShow(){return _show;}
	
protected:
	virtual void getNodeCoordinate(TreeRing *tr, int trLayer, int nodeIndex, float *x, float *y, float *angle);
	//virtual void insertHoltenPoints(Stroke *curve, TreeRing *tr, int si, float sx, float sy, 
	//	int ni, float nx, float ny,float beta);
	

	void drawArrow(QPainter *painter, QPointF start, QPointF end, float width, QColor color);
	//-----------------------Model calculation and selection-------------------------------

	// create the _nodeToEdgeTable 
	//   must create the _in_list or _out_list first
	//   before calling this routine
	// TODO: error checking


	/////////////////////////////////////
	// PARAMETERS
	/////////////////////////////////////

	//vector < vector <Stroke> >  bun;
	//vector <Stroke> bun;
	vector <struct elMarker_str> marker;

	vector< vector <int> > _in_list;  
	// NOT IMPLEMENTED
	// save the source nodes; the list a node
	// is pointed towards.

	vector< vector <int> > _out_list; 
	// save the destination nodes; the list a node
	// is pointing to.
	// contains _nn of nodes

	vector <int>   _nodeToEdgeTable; //_nodeToEdgeTable[i] + j - 1 表示结点i连接结点_out_list[i][j]的边的序号，
	// a look up table that saves the starting edge index of a node

	//A table that saves the index of incoming edges of a node
	//vector< vector<int> >_income_edges_of_node;

	///////// drawing parameters
	float  _rr;  // tree node ring radius 
	double _variance_to_show; //show bundles above variance of _variance_to_show
	float _ww;

	int    _nn;  // number of tree leaf node

	int  _hl_from;  // node from  
	int  _hl_to;    // node to
	int highlightIndex;
	//int _edge_by;

	Color  _hl_c; // highlight color
	Color  _c;    // regular display

	Point _centre;

	SimData *_sim_data;



	///////// from the graph representation ////////////// changed parameters
	//vector <bool>  _changed;

	//vector<bool>_show; //variance related
	// the edge comes from a node that is changing with time or not
	//  true:  changing
	//  false: not changing

	int    _nc;  // number of changed leaf nodes

	bool ENABLE_DRAW_CHANGED;
};


#endif //__MARKER_H
