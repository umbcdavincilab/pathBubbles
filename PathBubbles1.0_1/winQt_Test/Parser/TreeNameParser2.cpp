//creates parser and levels from filename
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <fstream>

#include "TreeNameParser2.h"

//resests parCount at the given parameter to 0
void TreeNameParser2::resetParCount(int li)
{
  assert(li<MAX_LEVEL);
  parCount[li]=0;
}

//creates parser and get necessary information from filename
TreeNameParser2::TreeNameParser2(char *filename)
{
  ifstream inf(filename, ios::in);
  char lineBuf[BUFSIZE], *token, sToken[BUFSIZE];
  char delim[]=".";  // delimited by space
  int  iToken;

  for(int i=0; i<MAX_LEVEL; i++) resetParCount(i);

  while(inf)
  {
    inf.getline(lineBuf, BUFSIZE);
	sToken[0]='\0'; // clean up

	// separate the index and string token
	sscanf(lineBuf, "%d %s", &iToken, sToken);

	// save the length 
	int lev= getLevels(sToken), lc=0, wc=0; // lc: level counter ; wc: word counter
    _lev.push_back(lev);

	if(lev==1) break; // feature. TODO:..last line.. 

    // split by "."
	_tmpToken.clear();
	token = strtok(sToken, delim);
	_tmpToken.push_back(token);

	pushbackLp(lc++, token); // save to _lp1

	while( (token = strtok(NULL, delim))!=NULL )
	{
	  _tmpToken.push_back(token);
	  pushbackLp(lc++, token); // save to _lp2, _lp3 and _lp4
	};
	_names.push_back(_tmpToken); // save the last one
  }; // end while(inf)

  int lcc= GetMaxLevel();
  for(int i=0; i<lcc; i++)
  {
	tmp_lpc[i].push_back(parCount[i]); // save the last one
  }

  inf.close();
}

void TreeNameParser2::pushbackLp(int levelIndex, char *token)
{
  // the first time - always push
  if(tmp_lp[levelIndex].size() == 0) 
  {
	tmp_lp[levelIndex].push_back(token);
	parCount[levelIndex]++;
	return;
  };

  // same token. don't push
  if(tmp_lp[levelIndex][tmp_lp[levelIndex].size()-1].compare(token) == 0) 
  {
	parCount[levelIndex]++; // count 
    return; 
  }
  else  // a new token. then push both the new token and the count
  {
    tmp_lp[levelIndex].push_back(token);
	tmp_lpc[levelIndex].push_back(parCount[levelIndex]);
	parCount[levelIndex]=1; // recount
  };
}

//destructor; takes care of anything that needs
//to happen when the object is no longer needed
TreeNameParser2::~TreeNameParser2()
{
  // clean up
}


int TreeNameParser2::GetMaxLevel()
{
  int ls = _lev.size(), result=-1;

  for(int i=0; i<ls; i++)
  {
    if(_lev[i]>result) 
	  result = _lev[i];
  }

  return result;
}


vector<string> TreeNameParser2::GetLevelXParameters(int levelIndex)
{
  int lc = GetMaxLevel();
  assert(levelIndex<lc); // OK. now hand <10 levels

  return tmp_lp[levelIndex];
}

//counts number of levels  in given array
int TreeNameParser2::getLevels(char par[BUFSIZE])
{
  int count=1, len=strlen(par);

  for(int ii=0; ii<len; ii++)
  {
    if(par[ii]=='.') count++;
  }

  return count;
}

// void TreeNameParser2::PrintByTH()
// {
//   int lc = GetMaxLevel();
// 
//   cerr << "Num of Tree Levels: " <<  lc << endl;
// 
//   vector<string>::iterator iter_th;  // iterator on tree hierarchy
//   for(int li=0; li<lc; li++) // level index
//   {
//     cerr << "--> level "<< li << endl;
// 	int jj=0;
//     for(iter_th=tmp_lp[li].begin(); iter_th!=tmp_lp[li].end(); iter_th++)
// 	{
// 	  cerr << tmp_lpc[li][jj] <<  *iter_th << endl ;
// 	  jj++;
// 	}; // end for(iter_th)
// 	cerr << endl;
//   }; // end for(li)
// }

// void TreeNameParser2::Print()
// {
//   vector< vector<string> >::iterator iter_ii;
//   vector<string>::iterator iter_jj;
//   vector<int>::iterator iter_lev;
// 
//   int li=0;
// 
//   cerr << "NUM_OF_GLOBAL_PAR=" << _lev.size() << endl;
// 
//   for(iter_ii=_names.begin(); iter_ii!=_names.end(); iter_ii++)
//   {
//       cerr << _lev[li]; li++;
//       for(iter_jj=(*iter_ii).begin(); iter_jj!=(*iter_ii).end(); iter_jj++)
//       {
// 		 cerr << " " << *iter_jj;
//       }
// 	  cerr << endl;
//   }; // end for(iter_ii)
// }
