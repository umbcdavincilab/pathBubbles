
#ifndef __GRAPH_INDEX_PARSER_H
#define __GRAPH_INDEX_PARSER_H

/************************************************************************
  GraphIndexParser2:

  Create a Parser to load the graph edges.

  Jian Chen March 2011

*/

#include <stdio.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <assert.h>
//#include "../TreeRing.h"

#define BUFSIZE 256

using namespace  std;


class GraphIndexParser2 
{
  public:
    //GraphIndexParser2(char *fn, TreeRing *tr);
    GraphIndexParser2(char *fn, int num_nodes);
	virtual ~GraphIndexParser2();

	int GetNumOfEdges();
	// return # of edges in the graph

	void GetEdgeIndex(int edgeIndex, int *ine, int *oute);
	// in: edgeIndex: start from 0;
	// out: ine: source index 
	//      oute: destination index

    vector <int> GetInNodeIndex(int index);
    vector <int> GetOutNodeIndex(int index);


	// prints
	void PrintInList();
	void PrintInList(int index);
	void PrintOutList();
	void PrintOutList(int index);

  protected:
    void buildLinkedList(char *fn);
	// build a linked list (actually a 2D vector):
	//      the vertical list is the source node. 
	//      the linked list following each node is the destination node.
	//      If we are interested in all out nodes, we can simply use 
	//      the linked list following the
	//
	//       [0] -> [10]->[11]->[23]->[356]
	//       [1] -> [0]
	//       [2] ....
	// 
	//      If we like to get all in node, we will have to go through the
	//      linked list to find out. OK. why don't I also construct a 
	//      in list. 
	//
	//

  private:
    vector<int> _in, _out;

	vector< vector <int> > _in_list;  // save the source nodes; the list a node
	                                  // is pointed towards.
    vector< vector <int> > _out_list; // save the destination nodes; the list a node,  _out_list[i]这个vector表示节点i可达的所有节点序号
						              // is pointing to.

	int _num_nodes;
};

#endif //__GRAPH_INDEX_PARSER_H
