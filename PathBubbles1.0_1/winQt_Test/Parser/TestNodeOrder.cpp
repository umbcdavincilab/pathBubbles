
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <assert.h>

#define BUFSIZE 256
#define MAX_LEVEL 10

#include "GraphIndexParser2.cpp"
#include "GraphNodeOrder.cpp"
#include "TreeNameParser.h"
#include "TreeNameParser.cpp"

using namespace std;

vector< vector<int> >  GetLevelXEdgeList(TreeNameParser tnp, GraphIndexParser2 gip, int X){
   //Likely only ok for level 0 graph   


   vector<int> Edge(2,0);
   vector< vector<int> > EdgeList;

   vector<string> level_X_names = tnp.GetLevelXParameters(X);
   int num_level_X_nodes = level_X_names.size();
   cout<<"  Number of nodes of level "<<X<<" graph is " << num_level_X_nodes <<endl;

   vector<int> groups = tnp.GetLevelXParCount(X);
   vector<int> start (num_level_X_nodes, 0);
   vector<int> end (num_level_X_nodes, 0);
  
   //Determine beginning and end of amalgamated nodes
   int currVal = 0;
   for (int i = 0; i < num_level_X_nodes; i++){
      start[i] = currVal;
      end[i] = currVal + groups[i] - 1;
      currVal = end[i] + 1;
   }

   int edges = 0;
   //Check adjacency between i, j in reduced graph
   for (int i = 0; i< num_level_X_nodes; i++){
     for (int j = 0; j < num_level_X_nodes; j++){
        bool adjacent = false;
        //Check all nodes amalgamated to i
        for (int node_1 = start[i]; node_1 <= end[i]; node_1++){
            vector<int> neighborList = gip.GetOutNodeIndex(node_1);
            for(int k=1; k < neighborList.size() ; k++){
	        int neighbor = neighborList[k];
                if ((neighbor >= start[j])&&(neighbor <= end[j])){
                    adjacent = true;
                }
            }
        }
       if(adjacent){ 
          Edge[0] = i; 
          Edge[1] = j;
          EdgeList.push_back(Edge);
          edges++;
       }
     }
   }
 
   cout<<"  Number of edges of level "<<X<<" graph is "<<edges<<endl;

  return EdgeList;


}





int main(int argc, char *argv[])
{

   using namespace std;

   if(argc!=5)
   {
     printf("Usage: ./TestNodeOrder ./Graph.txt ./TreeNames.txt ./OutputGraph.txt ./OutputTreeNames.txt\n");
	exit(-1);
   }
   cout<<"*****************************"<<endl;

   GraphIndexParser2 gip =  GraphIndexParser2(argv[1]); 
   TreeNameParser tnp = TreeNameParser(argv[2]);
   int shuffleIterations = 10000;


   vector<int> identity (gip.GetNumOfNodes(), 0);
   for(int i = 0; i<gip.GetNumOfNodes(); i++){
     identity[i] = i;
   }
       
   cout<<"*Statistics for original graph*"<<endl;
   ShuffleStatistics(gip, identity);
   cout<<endl;


   //Get Level 0 Graph
   cout<<"Getting Edge List for Level 0 Graph"<<endl;
   vector< vector<int> > EdgeList = GetLevelXEdgeList(tnp, gip, 0);
   vector<int> sou, des;

   for (int i = 0; i<EdgeList.size(); i++){
       sou.push_back(EdgeList[i][0]);
       des.push_back(EdgeList[i][1]);
   }
   GraphIndexParser2 gip2 = GraphIndexParser2(sou, des);

   //Shuffle Nodes of Level 0 Graph to get partial Permutation
   cout<<"Shuffling with "<<shuffleIterations<<" iterations"<<endl;
   vector<int> partialPermutation = GraphShuffle(gip2, shuffleIterations);
   cout<<endl;

   //Extend partial Permutation to full Permutation
   vector<string> _names_reduced = tnp.GetLevelXParameters(0);
   int _num_nodes_reduced = _names_reduced.size();

   vector<int> groups = tnp.GetLevelXParCount(0);
   vector<int> start (_num_nodes_reduced, 0);
   vector<int> end (_num_nodes_reduced, 0);
  
   //Determine start and end of level 0 groups (amalgamated nodes)
   int currVal = 0;
   for (int i = 0; i < _num_nodes_reduced; i++){
      start[i] = currVal;
      end[i] = currVal + groups[i]-1;
      currVal = end[i] + 1;
   }

   vector<int> fullPermutation ( gip.GetNumOfNodes(), 0);

   int counter = 0;
   for (int i = 0; i< _num_nodes_reduced; i++){
     for (int k = 0; k < _num_nodes_reduced; k++){
         if (partialPermutation[k] == i){
            for (int j = start[k]; j <= end[k]; j++){
               fullPermutation[j] = counter;
               counter++;
            }
         }
      }
   }

   cout<<"Checking shuffle statistics for full permutation ..."<<endl;
   ShuffleStatistics(gip, fullPermutation);

   //Output Shuffled graph
   gip.Shuffle(fullPermutation);
   gip.PrintEdgeListToFile(argv[3]);
   tnp.PrintShuffleToFile(fullPermutation, argv[4]);
   cout<<"Output Shuffled Edge List to "<<argv[3]<<endl;
   cout<<"Output Shuffled Node Names to "<<argv[4]<<endl<<endl;


   //Doublecheck statistics for output graph
   cout<<"Reading back output graph to check"<<endl;
   GraphIndexParser2 gip3 =  GraphIndexParser2(argv[3]); 
   ShuffleStatistics(gip3, identity);

return 0;
}

