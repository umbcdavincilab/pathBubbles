
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include <math.h>
#include <vector>
#include <fstream>
#include <iostream>

#define PI (3.141592653589793)


void ShuffleStatistics(GraphIndexParser2 gip, vector<int> permutation)
{
  
  int _num_nodes = gip.GetNumOfNodes();

  //Initialize Statistics
  long CBW = 0;      //Circular Bandwidth
  double SSEL = 0;    //Sum of Squared Edge Lengths

  //Set to uniform spacing (given permutation)
  vector<double> point (2,0);
  vector< vector<double> > points (_num_nodes, point);
  double theta;
  for (int j = 0; j< _num_nodes; j++){
      theta = ((double) permutation[j])*(2*PI/(_num_nodes));
      points [j][0] = cos(theta);
      points [j][1] = sin(theta);
   }

  int node_1, node_2, p1, p2;
  double SEL = 0;

  //Iterate over all nodes
  for(int ii = 0; ii<_num_nodes; ii++){
       
    node_1 = ii;

    vector<int> neighborList = gip.GetOutNodeIndex(ii);
    if( neighborList.size() > 1){

       for(int jj=1; jj < neighborList.size() ; jj++){

          node_2 = neighborList[jj];
           
          //Determine square of distance between points on circle 
          SEL = pow((points[node_1][0] - points[node_2][0]),2) + pow((points[node_1][1] - points[node_2][1]),2);
          SSEL = SEL + SSEL;
          

          //Determine (minimum) distance in order around circle
          p1 = permutation[node_1];
          p2 = permutation[node_2];
          if (p1 < p2){
            if (2*p2 < 2*p1 + _num_nodes)
                   { CBW = CBW + (p2 - p1);}
            else   { CBW = CBW + (p1 - p2) + _num_nodes;}
            }
          else {
            if (2*p1 < 2*p2 + _num_nodes)
                   { CBW = CBW + (p1 - p2);}
            else   { CBW = CBW + (p2 - p1) + _num_nodes;}
          }
	}
    }
  }

  cout<<"Sum of Squared Edges Lengths (straight-line circular ordering): "<<SSEL<<endl;
  cout<<"Circular Bandwidth: "<<CBW<<endl;
}


bool infoCompare (vector<double> info1,vector<double> info2) {
    return (info1[2]<info2[2]); }


vector<int> GraphShuffle(GraphIndexParser2 gip, int numIter)
{
  
  //Algorithm from the following paper...
  //     Improved Circular Layouts
  //  Emden R. Gansner and Yehuda Koren
  
  int _num_nodes = gip.GetNumOfNodes();

  vector<double> nodeData (4,0);
  vector< vector<double> > nodeInfo (_num_nodes, nodeData);
  //Node Info: 0: x-value   1: y-value    2: angle   3: Original Label
  //***Nodes should be labeled so that sorted according to [3]
  //***until the end, when sorted by 4 to print

  vector<int> permutation (_num_nodes, 0);
  //permutation[original position] = new position

  //Initialize positions along unit circle and node labels
  cout<<"Initializing Graph Shuffle ..."<<endl;
  double theta, x, y;

  for (int j = 0; j< _num_nodes; j++){
    theta = ((double) j)*(2*PI/(_num_nodes));
    x = cos(theta);
    y = sin(theta);  

    nodeInfo [j][0] = x;
    nodeInfo [j][1] = y;
    nodeInfo [j][2] = theta;
    nodeInfo [j][3] = (double) j;
    permutation[j] = j;
  }

  int iter = 0;
  int currNode;
  int neighbor;

   while (iter < numIter){
 
      //Choose three anchor points to avoid collapse to single point
      //(trivial solution to minimizing squared edge lengths)
      double offset = (((double)_num_nodes)/((double) numIter))*((double) iter);
      int anchor1 = (int) offset;
      int anchor2 = (((int)offset) + (_num_nodes/3))%(_num_nodes);
      int anchor3 = (((int)offset) + 2*(_num_nodes/3))%(_num_nodes);

      for(int ii = 0; ii<_num_nodes; ii++){
       
       //Find new label of node at iterator
       currNode = permutation[ii];

       if ((currNode != anchor1)&&( currNode != anchor2)&&( currNode != anchor3)){
        
          vector<int> neighborList = gip.GetOutNodeIndex(ii);
          if( neighborList.size() > 1){
 
           //Find barycentric center of neighbors
           double baryx= 0; double baryy = 0; x = 0; y = 0;

           for(int jj=1; jj < neighborList.size() ; jj++)
	   {
	     //Find sum of x- y- values of (permuted) neighbors
             neighbor = permutation[neighborList[jj]]; 
             x = x + nodeInfo[neighbor][0];
             y = y + nodeInfo[neighbor][1];
             
	   };
           //Average over neighbors to get barycentric values
           baryx = x / (neighborList.size()-1); 
           baryy = y / (neighborList.size()-1);

           //Normalize - Project back onto circle.
           x = baryx/(sqrt(baryx*baryx + baryy*baryy));
           y = baryy/(sqrt(baryx*baryx + baryy*baryy));

           //Update x and y values at new label
           //   and calculate new angle, 0 <= theta <= 2*PI
           nodeInfo [currNode][0] = x;
           nodeInfo [currNode][1] = y;
           if (atan2( y, x) > 0) { nodeInfo [currNode][2] = atan2( y, x);}
           else                  { nodeInfo [currNode][2] = 2*PI + atan2( y, x);}
        }
      }
     }//end for loop

     
     if (iter%10 == 0){
        //In this case, Update permutation and Re-initialize nodes.

        //Sort nodes by position (by angle theta)
        vector< vector<double> >::iterator iter_kk;
        sort (nodeInfo.begin(), nodeInfo.end(), infoCompare);

        //Iterate sorted list, updating the permutation (node labels), 
        double label = 0;
        for(iter_kk=nodeInfo.begin(); iter_kk!=nodeInfo.end(); iter_kk++){
           permutation[(*iter_kk)[3]] = (int) label;  //Keep permutation current
           label++;
        }

        //Reset to uniform spacing
       for (int j = 0; j< _num_nodes; j++){
          theta = ((double) permutation[ nodeInfo[j][3] ])*(2*PI/(_num_nodes));
          x = cos(theta);
          y = sin(theta);  

          nodeInfo [j][0] = x;
          nodeInfo [j][1] = y;
          nodeInfo [j][2] = theta;
         }
          
       if (iter%(numIter/10) == 0){
            // ShuffleStatistics(gip, permutation);
       }
   }
   iter++;
  }//End Iterations


  //Final Sort of nodes by position (by angle theta)
  // (in case numIter not multiple of 10)
  vector< vector<double> >::iterator iter_kk;
  sort (nodeInfo.begin(), nodeInfo.end(), infoCompare);
  double label = 0;
  for(iter_kk=nodeInfo.begin(); iter_kk!=nodeInfo.end(); iter_kk++){
      permutation[(*iter_kk)[3]] = (int) label;  //Get Current Permutation 
      label++;
   }

  cout<<"Finished Graph Shuffle ..."<<endl;
  return permutation;
}
