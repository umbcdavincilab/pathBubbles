#ifndef __TREE_NAME_PARSER2_H
#define __TREE_NAME_PARSER2_H

/************************************************************************
  NameParser2:

  Create a Parser to load the node names. we can query any string in the
  list;

  Jian Chen March 2011

*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <assert.h>

#define BUFSIZE 256
#define MAX_LEVEL 10

using namespace std;

class TreeNameParser2
{
  public:
    TreeNameParser2(char *filename);
	virtual ~TreeNameParser2();

    //****** Gets
    int GetLevelByIndex(int i) {return _lev[i];}
	// get the depth for parameter index i

	int GetMaxLevel();
	// return the height of the highest sub-tree

	int GetNumOfParameters() {return _lev.size();}
	// get the number of parameters

	string GetString(int pi, int si) { assert(si<_lev[pi]); return _names[si][pi]; }
	// get the String by the index;
	//  pi: parameter index
	//  si: string index; must less than the count

	vector<string> GetLevelXParameters(int levelIndex);
	// return the number of parameters in that level, and the string in "lp"
	//     no replication

    //****** Print
    void Print();
	// will print all names

	void PrintByTH();
	// print by the tree hierachies

  protected:
    int getLevels(char par[BUFSIZE]);
	// check the levels of a parameter (full name on each line)

	void resetParCount(int li);
	// reset parCount to 0. parCount[li]=0; 

	void pushbackLp(int levelIndex, char *token);
	// push the token into the corresponding vector <string> at level of "levelIndex"

	vector<string> _tmpToken;
	// tmpToken used to store the tokens on each line.. used by the _lp

	int parCount[MAX_LEVEL];


  private:

    // **** save by parameters
    vector< vector<string> > _names; 
	// name of the parameters
	// divided by '.'
	vector<int> _lev;
	// level of each parameter divided by '.'

/////////////OK.. junk tmp_lp and tmp_lpc unless the tree index is saved... 
///////////// simply pass from the _lev ... leave these here anyway.
	// **** save by tree structure - categorical
    vector<string> tmp_lp[MAX_LEVEL];
	// save the levels
    vector<int > tmp_lpc[MAX_LEVEL];
	// save the counter for each higher level nodes. Otherwise, I won't 
	// know who is whose children because I only save a higher-level node 
	// once. 

///////////// wait a second, at least I know how many nodes I should 
////////////  create on each level if I reverse the order
	// **** save by tree structure - categorical - reversed order
	//  save the last one the first level
	//       the second last one the second level etc.
    vector<string> _rv_tmp_lp[MAX_LEVEL];
	// save the levels
    vector<int > _rv_tmp_lpc[MAX_LEVEL];
	// save the counter for each higher level nodes. Otherwise, I won't 
	// know who is whose children because I only save a higher-level node 
	// once. 

//////////////////////////
};

template<typename T, typename P>
T remove_if(T beg, T end, P pred)
{
    T dest = beg;
    for (T itr = beg;itr != end; ++itr)
        if (!pred(*itr))
            *(dest++) = *itr;
    return dest;
}

#endif //__TREE_NAME_PARSER2_H
