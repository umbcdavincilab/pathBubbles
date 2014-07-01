

#include <stdio.h>

#include "GraphIndexParser2.h"

using namespace std;

//GraphIndexParser2::GraphIndexParser2(char *fn, TreeRing *tr)
GraphIndexParser2::GraphIndexParser2(char *fn, int num_nodes)
{
  ifstream inf(fn, ios::in);
  int   sou, des;
  char lineBuf[BUFSIZE];

 // _num_nodes = 0;

  while(inf.getline(lineBuf, BUFSIZE))
  {
    sscanf(lineBuf, "%d %d", &sou, &des);
	_in.push_back(sou);
	_out.push_back(des);

//     if(sou>_num_nodes) _num_nodes=sou;   //����_num_nodesֻ����graph�ļ��г��ֵ���󶥵����
//     if(des>_num_nodes) _num_nodes=des;
  }; // end while

  //_num_nodes++;
  //_num_nodes = tr->GetNumOfNodes();
  _num_nodes = num_nodes;
  inf.clear();
  inf.seekg(0, ios::beg); // rewind the pointer to the beginning
  inf.close();

  buildLinkedList(fn);
}

GraphIndexParser2::~GraphIndexParser2()
{
  // TODO: clean up
}

int GraphIndexParser2::GetNumOfEdges()
{
  return _in.size();
}

void GraphIndexParser2::GetEdgeIndex(int edgeIndex, int *ine, int *oute)
{
  *ine = _in[edgeIndex];
  *oute = _out[edgeIndex];
}

void GraphIndexParser2::buildLinkedList(char *fn)
{
  ifstream inf(fn, ios::in);
  cerr << "num_nodes = " << _num_nodes << endl;

  //  build the out/in graph
  for(int i=0; i<_num_nodes; i++)
  {
     vector <int> onodes;
     vector <int> inodes;
     int   sou, des;
     char lineBuf[BUFSIZE];

     inf.clear();
     inf.seekg(0, ios::beg); // rewind the pointer to the beginning

	 onodes.push_back(i);  // init the first par the node index
	 inodes.push_back(i);

     while(inf.getline(lineBuf, BUFSIZE))
     {
       sscanf(lineBuf, "%d %d", &sou, &des);
	   if(sou == i)  onodes.push_back(des);
	   if(des == i)  inodes.push_back(sou);
     }; // end while

	 _out_list.push_back(onodes);
	 _in_list.push_back(inodes);
	 
	 onodes.clear();
	 inodes.clear();
  };

  inf.close();
}


void GraphIndexParser2::PrintInList()
{
  vector< vector<int> >::iterator iter_ii;
  vector<int>::iterator iter_jj;
  int counter=0;

  for(iter_ii=_in_list.begin(); iter_ii!=_in_list.end(); iter_ii++)
  {
    cerr << counter << ": " ;

    for(iter_jj=(*iter_ii).begin(); iter_jj!=(*iter_ii).end(); iter_jj++)
	{
	  cerr << *iter_jj << ", " ;
	};

	counter++;
  }
  cerr << endl;
}

void GraphIndexParser2::PrintInList(int index)
{
  vector<int>::iterator iter_jj;
  for(iter_jj=_in_list[index].begin(); iter_jj!=_in_list[index].end(); iter_jj++)
  {
	  cerr << *iter_jj << ", " ;
  }
}

void GraphIndexParser2::PrintOutList()
{
  vector< vector<int> >::iterator iter_ii;
  vector<int>::iterator iter_jj;
  int counter=0;

  for(iter_ii=_out_list.begin(); iter_ii!=_out_list.end(); iter_ii++)
  {
    cerr << counter << ": " ;

    for(iter_jj=(*iter_ii).begin(); iter_jj!=(*iter_ii).end(); iter_jj++)
	{
	  cerr << *iter_jj << ", " ;
	};

	counter++;
  }
  cerr << endl;

}

void GraphIndexParser2::PrintOutList(int index)
{
  vector<int>::iterator iter_jj;
  for(iter_jj=_out_list[index].begin(); iter_jj!=_out_list[index].end(); iter_jj++)
  {
	  cerr << *iter_jj << ", " ;
  }
}

vector <int> GraphIndexParser2::GetInNodeIndex(int index)
{
  return _in_list[index];
}

vector <int> GraphIndexParser2::GetOutNodeIndex(int index)
{
  return _out_list[index];
}