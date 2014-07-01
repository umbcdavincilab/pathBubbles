//#include <stdlib.h>
#include <QString>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include "TreeNameParser.h"

void TreeNameParser::resetParCount(int li)
{
	assert(li<MAX_LEVEL); //error check max level
	parCount[li]=0;
}

TreeNameParser::TreeNameParser(char *filename)//Normal.txt
{
	ifstream inf(filename, ios::in);
	// from the name, get the layer number
	QString fName(filename);
	fName.remove(".out");
	int idx = fName.lastIndexOf("_");
	fName = fName.mid(idx+1,fName.size()-idx);
	int layerNum=fName.toUInt();

	//int bufsize = BUFSIZE * 3;
	char lineBuf[BUFSIZE3], *token, sToken[BUFSIZE3];
	char delim[]=".";  // delimited by space
	int  iToken;

	for(int i=0; i<MAX_LEVEL; i++) resetParCount(i);

	while(inf)
	{
		inf.getline(lineBuf, BUFSIZE3);
		for(int j = 0; j < BUFSIZE3; ++j)sToken[j] = '\0';

		// separate the index and string token
		sscanf(lineBuf, "%d %s", &iToken, sToken);

		// save the length 
		int lev= getLevels(sToken), lc=0, wc=0; 
		// lc: level counter ; wc: word counter
		_lev.push_back(lev); // now everything is 3

		if(lev==1) break; // feature. TODO:..last line.. 

		if(lev==2)
		{	
			char temp[BUFSIZE3];
			int i = 0;
			for(int j = 0; j < BUFSIZE3; ++j)temp[j] = '\0';

			for(; sToken[i] != '.'; ++i)
				temp[i] = sToken[i];
			temp[i] = sToken[i];
			++i;
			for(int j = 0; sToken[j] != '\0'; ++j, ++i)
				temp[i] = sToken[j];
			strcpy(sToken, temp);
			lev++;

		}
		// split by "."
		_tmpToken.clear();
		token = strtok(sToken, delim);

	    if(lev==4)
		{
			// get rid of the first level
		}

		if(lev == layerNum)
		{
			_tmpToken.push_back(token); // keep what it is
			pushbackLp(lc++, token, _tmpToken); // save to _lp1
		}

		while( (token = strtok(NULL, delim))!=NULL )
		{
			_tmpToken.push_back(token);
			pushbackLp(lc++, token, _tmpToken); // save to _lp2, _lp3 and _lp4
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

void TreeNameParser::pushbackLp(int levelIndex, char *token, vector<string> _tmpToken)
{
	//if(levelIndex==2)

	// the first time - always push
	if(tmp_lp[levelIndex].size() == 0)
	{
		tmp_lp[levelIndex].push_back(token);
		parCount[levelIndex]++;
		return;
	};
    //same token. don't push
	if(levelIndex==1)
	{
		if( (tmp_lp[levelIndex][tmp_lp[levelIndex].size()-1].compare(token) == 0 ) 
			&& tmp_lp[levelIndex-1][tmp_lp[levelIndex-1].size()-1].compare(_tmpToken[_tmpToken.size()-2]) == 0
			&& levelIndex!=2)
		{
			string s2=tmp_lp[levelIndex][tmp_lp[levelIndex].size()-1];
			string s1=tmp_lp[levelIndex-1][tmp_lp[levelIndex-1].size()-1];
			parCount[levelIndex]++; // count 
			return; 
		}
		else  //a new token. then push both the new token and the count
		{
			tmp_lp[levelIndex].push_back(token);
			tmp_lpc[levelIndex].push_back(parCount[levelIndex]);
			parCount[levelIndex]=1; //recount
		};
	}
	else
	{
		if( (tmp_lp[levelIndex][tmp_lp[levelIndex].size()-1].compare(token) == 0)  && levelIndex!=2)
		{
			parCount[levelIndex]++; //count 
			return; 
		}
		else  // a new token. then push both the new token and the count
		{
			tmp_lp[levelIndex].push_back(token);
			tmp_lpc[levelIndex].push_back(parCount[levelIndex]);
			parCount[levelIndex]=1; // recount
		};
	}
}

TreeNameParser::~TreeNameParser()
{
	// clean up
}

//returns the highest level of the data
int TreeNameParser::GetMaxLevel()
{
	int ls = _lev.size(), result=-1;

	for(int i=0; i<ls; i++)
	{
		if(_lev[i]>result) 
			result = _lev[i];
	}

	return result;
}

vector<int> TreeNameParser::GetLevelXParCount(int levelIndex)
{
	int lc = GetMaxLevel();
	assert(levelIndex<lc); // OK. now hand <10 levels

	return tmp_lpc[levelIndex];
}

//returns name of level the given level
vector<string> TreeNameParser::GetLevelXParameters(int levelIndex)
{
	int lc = GetMaxLevel();//returns highest level from data
	if(levelIndex>=lc)
	{
		vector<string> empty;
		return empty;
	}
	assert(levelIndex<lc); // OK. now hand <10 levels
				//make sure level index is no more than 
				//the maximum level

	return tmp_lp[levelIndex];
}

int TreeNameParser::getLevels(char par[BUFSIZE])
{
	int count=1, len=strlen(par);

	for(int ii=0; ii<len; ii++)
	{
		if(par[ii]=='.') count++;
	}

	return count;
}


void TreeNameParser::PrintByTH()
{
  int lc = GetMaxLevel();

  cerr << "Num of Tree Levels: " <<  lc << endl;

  vector<string>::iterator iter_th;  // iterator on tree hierarchy
  for(int li=0; li<lc; li++) // level index
  {
    cerr << "--> level "<< li << endl;
	int jj=0;
    for(iter_th=tmp_lp[li].begin(); iter_th!=tmp_lp[li].end(); iter_th++)
	{
	  cerr << tmp_lpc[li][jj] <<  *iter_th << endl ;
	  jj++;
	}; // end for(iter_th)
  cerr << endl;
  }; // end for(li)
}

void TreeNameParser::Print()
{
  vector< vector<string> >::iterator iter_ii;
  vector<string>::iterator iter_jj;
  vector<int>::iterator iter_lev;

  int li=0;

  cerr << "NUM_OF_GLOBAL_PAR=" << _lev.size() << endl;

  for(iter_ii=_names.begin(); iter_ii!=_names.end(); iter_ii++)
  {
     // cerr << _lev[li]; 
	  li++;
      for(iter_jj=(*iter_ii).begin(); iter_jj!=(*iter_ii).end(); iter_jj++)
      {
		 cerr << *iter_jj << "," ;
      }
  cerr << endl;
  }; // end for(iter_ii)
}

