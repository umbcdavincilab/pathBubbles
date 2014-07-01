#include <boost/config.hpp>
#include <iostream>
#include <fstream>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/property_map/property_map.hpp>

#include <boost/graph/connected_components.hpp>

#include "myGraph.h"

#define MIDIUM_HIGHLIGHTCOLOR QColor(255,128,0,255) //QColor(20,20,255,255)

myGraph::myGraph()
{
	//_wholeGraph
	//findConectedComponnets();
	testShortestPath();
}

bool myGraph::isadjacency_node_not_present_in_current_path(int node,vector<int>path)
{
    for(int i=0;i<path.size();++i)
    {
        if(path[i]==node)
        return false;
    }
    return true;
}

int myGraph::findNodeID(vector<int> node, vector<vector<int>> nodeVector)
{
	for(int i=nodeVector.size()-1; i>=0; i--)
	{
	   if(node==nodeVector[i])
		   return i;	
	}
	return -1;
}

QColor myGraph::getPathColor(int id)
{
	QColor c;
	id = id % 7;
    switch( id )
	{
		case 0:
			//"light orange";
			c= QColor(254,153,41,127);
			break;
		case 1:
			//light blue"; 
			c= QColor(65,182,196,127);
			break;
		case 2:
			//"light rose";
			c= QColor(223,101,176,127);
			break;
        case 3:
			//"pink";
			c= QColor(247,104,161,127);
			break;
		case 4:
			//"light green"; 
			c= QColor(120,198,121,127);
			break;
		case 5:
		   //"light purple"; 
			c= QColor(158,154,200,127);
			break;				
		case 6:
		default:
			//"light red";
			c= QColor(251,106,74,127);			 
			break;
	}
	return c;
}


//******/// find all path for two nodes in a graph**//
/*example
	/*vector<vector<int>> GRAPH;
	GRAPH.resize(7);
	GRAPH[1].push_back(2);	
	GRAPH[1].push_back(5);
	GRAPH[2].push_back(1);
	GRAPH[2].push_back(3);
	GRAPH[2].push_back(4);
	GRAPH[3].push_back(4);
	GRAPH[4].push_back(3);
	GRAPH[5].push_back(6);
	GRAPH[5].push_back(4);
	GRAPH[6].push_back(3);
	GRAPH[1].push_back(4);

	vector<vector<int>> pathVector;
	findAllPathes(1, 4, 6, 11, GRAPH, pathVector);

	//testforceDirected(painter, QRectF(0,0,200,200));
	//pathVector = findPathforANode(edge);
*/


/*

  This example demonstrates the usage of the connected_components
  algorithm on a undirected graph. The example graphs come from
  "Introduction to Algorithms", Cormen, Leiserson, and Rivest p. 87
  (though we number the vertices from zero instead of one).

  Sample output:

  Total number of components: 3
  Vertex 0 is in component 0
  Vertex 1 is in component 0
  Vertex 2 is in component 1
  Vertex 3 is in component 2
  Vertex 4 is in component 0
  Vertex 5 is in component 1

 */



int myGraph::findConectedComponnets()
{
    typedef boost::adjacency_list <boost::vecS, boost::vecS, boost::undirectedS> Graph;
    Graph G;
    add_edge(0, 1, G);
    add_edge(1, 4, G);
    add_edge(4, 0, G);
    add_edge(2, 5, G);
    
    std::vector<int> component(num_vertices(G));
    int num = connected_components(G, &component[0]);
    
    std::vector<int>::size_type i;
    cout << "Total number of components: " << num << endl;
    for (i = 0; i != component.size(); ++i)
      cout << "Vertex " << i <<" is in component " << component[i] << endl;
    cout << endl;

  return num;
}

vector<vector<int>> myGraph::getRank(vector<vector<int>> Edge, vector<vector<vector<int>>> Vertex, vector<vector<int>> VertexInfo, int roomID)
{   
	typedef boost::adjacency_list <boost::vecS, boost::vecS, boost::undirectedS> Graph;
	vector<vector<int>> Rank;
    Graph G;
	vector<Graph> subG;
	vector<int> nodeRanks(Vertex.size(),20000);
	
	float maxWeight=-1000000;
    //Edge to graph
	/*add_edge(0, 1, G);
    add_edge(1, 4, G);
    add_edge(4, 0, G);
    add_edge(2, 5, G);*/
	if(roomID==0)
		roomID=roomID;

	int count=0;
	for(int k=0; k<Edge.size(); k++)
	{
	    //if(Edge[k][0]==etemp[0] || Edge[k][1]==etemp[1])
		if(roomID<0 || (VertexInfo[Edge[k][0]][3] ==  roomID && VertexInfo[Edge[k][1]][3] == roomID ) )			
		{
			if(Edge[k].size()>=4)
			{
				if(maxWeight<Edge[k][4])
				   maxWeight=Edge[k][4];
			}
			else 
				maxWeight=1;
		    add_edge(Edge[k][0], Edge[k][1], G);
			count++;
		}
	}
	if(count==0)
		return Rank;

	vector<vector<int>> Edge_t;//=Edge;
	//change weight:
	
	//for each commpoent
	//find longestPath (reverse the weight) for all node	

	//get connected components
	std::vector<int> component(num_vertices(G));
    int num = connected_components(G, &component[0]);
	//subG.resize(num);
	vector<set<int>> edgeSet;	
	vector<set<int>> nodeSet;
	edgeSet.resize(num);
	nodeSet.resize(num);

	std::vector<int>::size_type i;
    for (i = 0; i != component.size(); ++i)
    {
		for(int j=0; j<Vertex[i].size(); j++)
		for(int k=0; k<Vertex[i][j].size(); k++)		
		{
			int eid = Vertex[i][j][k];
			vector<int> edge=Edge[eid];
			if(VertexInfo[edge[0]][3] ==  roomID && VertexInfo[edge[1]][3] ==  roomID)
				edgeSet[component[i]].insert(eid);
		}
		nodeSet[component[i]].insert(i);
	}		
	for (int sid = 0; sid < edgeSet.size(); ++sid)
	{	
		if(edgeSet[sid].size()>0)
		{
			Edge_t.clear();
			for(set<int>::iterator it=edgeSet[sid].begin(); it!=edgeSet[sid].end(); it++)	
			{   
				int eid=*it;
				vector<int> edge = Edge[eid];
				if(edge.size()<4)
					edge.push_back(1);
				else 
				    edge[4]= maxWeight+1-edge[4];
				Edge_t.push_back(edge);
			}
			//init			
			set<int> Node_t=nodeSet[sid];
			set<int> preNodeSet;
			while(!Edge_t.empty() && !Node_t.empty())
			{
                //get ranks for sub pathes
				subPathes(preNodeSet, Edge_t, Node_t, nodeRanks);
			}
		}
	}	
	//put rank according the ditance to the start node on the path
	for (int nid = 0; nid < nodeRanks.size(); ++nid)
	{
		if(nodeRanks[nid]==20000)
			continue;

		int level=nodeRanks[nid];		
		if(Rank.size() <= level)
		{	
		     Rank.resize(level+1);
		}
		Rank[level].push_back(nid);
	}	
    return Rank;
}

void myGraph::subPathes(set<int> &preNodeSet, vector<vector<int>> &Edge_t, set<int>& nodeSet, vector<int> &nodeRanks)
{
	float maxlength=-100000;
	int maxid=-1;
	vector<vector<vector<int>>> maxPath;	
	bool haveHead=false;
	set<int> headSet, endSet;
		
	for(int i=0; i<Edge_t.size(); i++)
	{
		headSet.insert(Edge_t[i][0]); endSet.insert(Edge_t[i][1]);
	}		
	for(set<int>::iterator it=nodeSet.begin(); it!=nodeSet.end(); it++)			
	{    
		int start=*it;
		if(headSet.find(start)!=headSet.end() && endSet.find(start)==endSet.end()) //a top node
		{
			int length;
			vector<vector<vector<int>>> path = getShortestPath(start, length, Edge_t); //edgeSet, nodeSet); 					
			if(length > maxlength)
			{
				maxlength = length;
				maxid = start;
				maxPath=path;
			}
			haveHead=true;
		}
	}	
	if(!haveHead)
	for(set<int>::iterator it=nodeSet.begin(); it!=nodeSet.end(); it++)			
	{    
		int start=*it;
		{
			int length;
			vector<vector<vector<int>>> path = getShortestPath(start, length, Edge_t); //edgeSet[sid], nodeSet); 					
			if(length > maxlength)
			{
				maxlength = length;
				maxid = start;
				maxPath=path;
			}
			haveHead=true;
		}
	}	
	//initial rank
	//get the rank by redo the shortest path search for the minimal span tree
	//Graph initG;
	vector<vector<int>> Edge_i=maxPath[0];
	for(int k=0; k<Edge_i.size(); k++)
	{
		//if(Edge[k][0]==etemp[0] || Edge[k][1]==etemp[1])
		//add_edge(maxPath[k][0], maxPath[k][1], 1, initG);
		Edge_i[k].push_back(1);
	}
	int length;
	vector<vector<vector<int>>> initPath = getShortestPath(maxid, length, Edge_i);//, edgeSet[sid], nodeSet); 
	//iniPath should have rank info
	vector<int> nodeRanks_t(nodeRanks.size(),20000);

	nodeRanks_t[maxid]=0;
	for(int k=0; k<initPath[0].size(); k++)
	{
		int v=initPath[0][k][1], d=initPath[0][k][initPath[0][k].size()-1];
		nodeRanks_t[v]=d;
	}						
	if(initPath[0].size()!=Edge_i.size())
	{
		//error
	}

	//reduce the searched graph
	Edge_t=maxPath[1];
	for(int i=0; i<maxPath[0].size(); i++)
	{
		preNodeSet.insert(maxPath[0][i][0]);  preNodeSet.insert(maxPath[0][i][1]);
	}
	if(preNodeSet.size()==nodeSet.size())
	{
		nodeSet.clear();
	}
	else
	{
		for(int i=0; i<Edge_t.size(); i++)
	    {
		  nodeSet.insert(Edge_t[i][0]);  nodeSet.insert(Edge_t[i][1]);
	    }
	}
	//reassign rank level to nodeRanks_t
	int markid=-1, markLevel=20000, dL=20000;
	for(int i=0; i<nodeRanks_t.size(); i++)
	{
	   if(nodeRanks_t[i]!=20000 && nodeRanks[i]!=20000 && nodeRanks_t[i]!=nodeRanks[i])
	   {
	       markid=i;  
		   dL = nodeRanks[i]-nodeRanks_t[i];
		   markLevel=nodeRanks_t[i];
	   }
	}
	if(markid>=0)
	for(int i=0; i<nodeRanks_t.size(); i++)
	{
	   if(nodeRanks_t[i]!=20000)
	   {
	       nodeRanks_t[i]=nodeRanks_t[i] + dL;
	   }
	}
	int negFlag=0;
	for(int i=0; i<nodeRanks.size(); i++)
	{
	   if(nodeRanks[i]==20000  && nodeRanks_t[i]!=20000)
	   {
	       nodeRanks[i]=nodeRanks_t[i];
		   if(nodeRanks_t[i]<negFlag)
			{
				negFlag=nodeRanks_t[i];
		   }
	   }
	}	
	if( negFlag<0)
	{
	    negFlag = -negFlag;
		for(int i=0; i<nodeRanks.size(); i++)
		{
			if(nodeRanks[i]!=20000)
			   nodeRanks[i]= nodeRanks[i]+negFlag;
		}
	}
}


vector<vector<vector<int>>> myGraph::getShortestPath(int start, int &length, vector<vector<int>> &edge)
{
	 vector<vector<int>> path,path_b;
	 vector<vector<vector<int>>> pathes;
	  //shortest path from a start point
	 //typedef boost::adjacency_list <  boost::listS,  boost::vecS,  boost::directedS,  boost::no_property,  boost::property <  boost::edge_weight_t, int > > graph_t;
	 //typedef  boost::graph_traits <  graph_t >::vertex_descriptor vertex_descriptor;
	 //typedef std::pair<int, int> Edge;

	 typedef boost::property<boost::edge_weight_t, int> EdgeWeightProperty;
	 typedef boost::adjacency_list<boost::listS,boost:: vecS, boost::directedS, boost::no_property,
	 EdgeWeightProperty > Graph;
	 typedef  boost::graph_traits <  Graph >::vertex_descriptor vertex_descriptor;

	 Graph G;
	 //add_edge(0, 1, 18, G);
	 
	  /*const int num_nodes = 5;//nodeSet.size(); //???
	  enum nodes { A, B, C, D, E };
	  char name[] = "ABCDE";
	  Edge edge_array[] = { Edge(A, C), Edge(B, B), Edge(B, E)};*/
	  
	  for(int i=0; i<edge.size(); i++)
	  {
		  //edge_array.push_back
		  //add_edge(edge[*it][0], edge[*it][1], temp);
		  add_edge(edge[i][0], edge[i][1], edge[i][edge[i].size()-1], G);
	  }

	  //int weights[] = { 1, 2, 1, 2, 7, 3, 1, 1, 1 };
	  //int num_arcs = sizeof(edge_array) / sizeof(Edge);
	  //graph_t g(edge_array, edge_array + num_arcs, weights, num_nodes);
	  
	  boost:: property_map<Graph,  boost::edge_weight_t>::type weightmap = get( boost::edge_weight, G);
	  std::vector<vertex_descriptor> p(num_vertices(G));
	  std::vector<int> d(num_vertices(G));
	  vertex_descriptor s = vertex(start, G);

	  std::ofstream dot_file;//("figs/dijkstra-eg.dot");
	  dot_file.open ("dijkstra-eg.txt");

	  dijkstra_shortest_paths(G, s,
							  predecessor_map(boost::make_iterator_property_map(p.begin(), get(boost::vertex_index, G))).
							  distance_map(boost::make_iterator_property_map(d.begin(), get(boost::vertex_index, G))));
	  	
	  vector<int> testPath;
	  length=0;
	  boost::graph_traits < Graph >::edge_iterator ei, ei_end;
	  for (boost::tie(ei, ei_end) = boost::edges(G); ei != ei_end; ++ei)
	  {
		  boost::graph_traits < Graph >::edge_descriptor e = *ei;
		  boost::graph_traits < Graph >::vertex_descriptor
		  u = source(e, G), v = target(e, G);
		  if (p[v] == u)
		  //dot_file << ", color=\"black\"";// in path
          {
			  vector<int> edge;
			  edge.push_back(u);   edge.push_back(v);     edge.push_back(d[v]);
			  path.push_back(edge);				 
			  length += get(weightmap, e);
		  }	
		  else 
		  {
		      vector<int> edge;
			  edge.push_back(u);   edge.push_back(v);     
			  path_b.push_back(edge);				 
		  }
	  }	  
	  pathes.push_back(path); pathes.push_back(path_b); 
	  return pathes;
}


bool myGraph::testShortestPath()
{
	  //shortest path from a start point
	  typedef boost::adjacency_list <  boost::listS,  boost::vecS,  boost::directedS,  boost::no_property,  boost::property <  boost::edge_weight_t, int > > graph_t;
	  typedef  boost::graph_traits <  graph_t >::vertex_descriptor vertex_descriptor;
	  typedef std::pair<int, int> Edge;

	  const int num_nodes = 5;
	  enum nodes { A, B, C, D, E };
	  char name[] = "ABCDE";
	  Edge edge_array[] = { Edge(A, C), Edge(B, B), Edge(B, E),
		 Edge(C, D), Edge(B, D),Edge(C, B),Edge(D, E), Edge(E, A), Edge(E, B)
	  };

	  //Edge(B, D),Edge(C, B),Edge(D, E), Edge(E, A), Edge(E, B)

	  int weights[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	  int num_arcs = sizeof(edge_array) / sizeof(Edge);
	  graph_t g(edge_array, edge_array + num_arcs, weights, num_nodes);
	  boost:: property_map<graph_t,  boost::edge_weight_t>::type weightmap = get( boost::edge_weight, g);
	  std::vector<vertex_descriptor> p(num_vertices(g));
	  std::vector<int> d(num_vertices(g));
	  vertex_descriptor s = vertex(A, g);

	  std::ofstream dot_file;//("figs/dijkstra-eg.dot");
	  dot_file.open ("dijkstra-eg.txt");

	  dijkstra_shortest_paths(g, s,
							  predecessor_map(boost::make_iterator_property_map(p.begin(), get(boost::vertex_index, g))).
							  distance_map(boost::make_iterator_property_map(d.begin(), get(boost::vertex_index, g))));

	  dot_file << "distances and parents:" << std::endl;
	   boost::graph_traits < graph_t >::vertex_iterator vi, vend;
	  for (boost::tie(vi, vend) = vertices(g); vi != vend; ++vi) {
		dot_file << "distance(" << name[*vi] << ") = " << d[*vi] << ", ";
		dot_file << "parent(" << name[*vi] << ") = " << name[p[*vi]] << std::
		  endl;

		}
	  //std::cout << std::endl;  
	  dot_file << "digraph D {\n"
		<< "  rankdir=LR\n"
		<< "  size=\"4,3\"\n"
		<< "  ratio=\"fill\"\n"
		<< "  edge[style=\"bold\"]\n" << "  node[shape=\"circle\"]\n";

	  boost::graph_traits < graph_t >::edge_iterator ei, ei_end;
	  for (boost::tie(ei, ei_end) = boost::edges(g); ei != ei_end; ++ei) {
		boost::graph_traits < graph_t >::edge_descriptor e = *ei;
		boost::graph_traits < graph_t >::vertex_descriptor
		  u = source(e, g), v = target(e, g);
		dot_file << name[u] << " -> " << name[v]
		  << "[label=\"" << get(weightmap, e) << "\"";
		if (p[v] == u)
		  dot_file << ", color=\"black\"";
		else
		  dot_file << ", color=\"grey\"";
		dot_file << "]";
	  }
	  dot_file << "}";
	  dot_file.close(); 
	  return true;
}

vector< vector < set < vector< vector<int> > > > > myGraph::findPathforTwoNodes(int pid1, int type1, int id1, int pid2, int type2, int id2)
{
	vector<vector<int>> pathVector;
	vector< vector < set < vector< vector<int> > > > > linkededge;

	//vertexInfo
	//...
	//Graph
	//find node set

	vector<int> node1(3,-1), node2(3,-1), edge_t(2,-1);
	vector<vector<int>> AEdge;
	AEdge.push_back(node1), AEdge.push_back(node2);
		
	
	int start, end;
	
	node1[0] = pid1, node1[1] = type1, node1[2] = id1;
	node2[0] = pid2, node2[1] = type2, node2[2] = id2;
	
	start = findNodeID(node1, nodeVector);
	end = findNodeID(node2, nodeVector);

	if(start<0 || end<0)
		return linkededge;

	findAllPathesBetweenTwoNodes(start, end, totalNode, totalEdge, _Graph, pathVector);	

	/*findAllPathes(18, 20, totalnode, totaledge, GRAPH, pathVector);	
	findAllPathes(1, 10, totalnode, totaledge, GRAPH, pathVector);	
	findAllPathes(15, 20, totalnode, totaledge, GRAPH, pathVector);	
	findAllPathes(25, 20, totalnode, totaledge, GRAPH, pathVector);	*/

	//if(pid1 >= linkededge.size())	   linkededge.resize(pid1+1);
	//if(pid2 >= linkededge.size())	   linkededge.resize(pid2+1);

	/*if(m_pathwayID>=linkedProtein.size())	   linkedProtein.resize(m_pathwayID+1);
	if(m_pathwayID>=linkedSmallMolecule.size())        linkedSmallMolecule.resize(m_pathwayID+1);
	if(m_pathwayID>=linkedComplex.size()) 	   linkedComplex.resize(m_pathwayID+1);
	if(m_pathwayID>=linkedDna.size()) 	   linkedDna.resize(m_pathwayID+1);
	if(m_pathwayID>=linkedReaction.size()) 	   linkedReaction.resize(m_pathwayID+1);
	if(m_pathwayID>=linkedANode.size()) 	   linkedANode.resize(m_pathwayID+1);
	if(m_pathwayID>=linkedCompartment.size())    linkedCompartment.resize(m_pathwayID+1);
	if(m_pathwayID>=linkedPathway.size())    linkedPathway.resize(m_pathwayID+1);*/
	
	for(int i=0; i<pathVector.size(); ++i)
    {
		set < vector < vector<int> > > path;
		set <int> pids;
		for(int j=0; j<pathVector[i].size()-1; ++j)
		{
			int id1=pathVector[i][j], id2=pathVector[i][j+1];
			AEdge[0] = nodeVector[id1];  AEdge[1]= nodeVector[id2];
            path.insert(AEdge);	
			pids.insert(AEdge[0][0]);
			pids.insert(AEdge[1][0]);
		}	
		for(set<int>::iterator it=pids.begin(); it!=pids.end(); it++)
        {
			int ptid = *it;
			if(ptid>=linkededge.size())
				linkededge.resize(ptid+1);

			linkededge[ptid].push_back(path);			
		}		
	}
	//getGraphToBePaint();	
	return linkededge;
}

void myGraph::findAllPathesBetweenTwoNodes(int source, int target, int totalnode, int totaledge, vector<vector<int>> GRAPH, vector<vector<int>> &pathVector)
{    //sort the path from smallest length to longest
	set<vector<int>> pathInfoSet;
	vector<vector<int>> pathVector_t;
    vector<int>path;
    path.push_back(source);
    queue<vector<int> >q;
    q.push(path);
    int count=0;
	while(!q.empty())
    {
        path=q.front();
        q.pop();

        int last_nodeof_path=path[path.size()-1];
        if(last_nodeof_path==target)
        {
			vector<int> pathInfo;
			pathInfo.push_back(path.size());
			pathInfo.push_back(count);
			pathInfoSet.insert(pathInfo);

            pathVector_t.push_back(path); //a valid path
			
			count++;
        }
        for(int i=0;i<GRAPH[last_nodeof_path].size();++i)
        {
            if(isadjacency_node_not_present_in_current_path(GRAPH[last_nodeof_path][i],path))
            {
                vector<int>new_path(path.begin(),path.end());
                new_path.push_back(GRAPH[last_nodeof_path][i]);
                q.push(new_path);
            }
        }
	}	
	for(set<vector<int>>::iterator it=pathInfoSet.begin(); it!=pathInfoSet.end(); it++)
	{
	    vector<int>	item = *it;
	    pathVector.push_back(pathVector_t[item[1]]);
	}
}