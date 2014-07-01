#include <vector>
#include <set>
#include <queue>
#include <QColor>

#ifndef MYGRAPH_H
#define MYGRAPH_H

using namespace std;

class myGraph
{
public:
	myGraph();
	//vector<vector<int>> getRank(vector<vector<int>> Edge, vector<vector<vector<int>>> Vertex, vector<vector<int>> VertexInfo, int roomID);
	vector<vector<int>> getRank(vector<vector<int>> Edge, vector<vector<vector<int>>> Vertex, vector<vector<int>> VertexInfo, int roomID);
	int findConectedComponnets();
	bool testShortestPath();
	//vector<vector<int>> getShortestPath(int start, int &length, vector<vector<int>> &Edge);//, set<int> edgeSet, set<int> nodeSet);
	bool isadjacency_node_not_present_in_current_path(int node, vector<int>path);
    int findNodeID(vector<int> node, vector<vector<int>> nodeVector);
    void findAllPathesBetweenTwoNodes(int source, int target, int totalnode, int totaledge, vector<vector<int>> GRAPH, vector<vector<int>> &pathVector);
	vector< vector < set < vector< vector<int> > > > > findPathforTwoNodes(int pid1, int type1, int id1, int pid2, int type2, int di1);
	void subPathes(set<int> &preNodeSet, vector<vector<int>> &Edge_t, set<int>& nodeSet, vector<int> &nodeRanks);
	
	vector<vector<vector<int>>> getShortestPath(int start, int &length, vector<vector<int>> &edge);

	QColor getPathColor(int i);	
	vector<vector<int>> _Graph; //keep an overgraph for all pathways
	vector<vector<int>> nodeVector; //record node info [pid0, type0, id0], [pid1, type1, id1], [pid2, type2, id2]
	int totalNode, totalEdge;	
};

#endif