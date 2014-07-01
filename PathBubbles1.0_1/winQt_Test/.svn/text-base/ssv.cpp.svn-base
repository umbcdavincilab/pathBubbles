#include "ssv.h"
#include <fstream>
#include <iostream>
#include <ctype.h>

using namespace std;

ssv::ssv(char *fn)
{
	_num_frames = 0;
	Read(fn);
}

void ssv::trim(char *str)
{
	int n, i;

	/* remove trailing space */
	for (n= strlen(str)-1; n>=0; n--)
		if(str[n] != ' ' && str[n] != '\t' && str[n] != '\n')
			break;
	str[n+1] = '\0';

	/* remove beginning space */
	for(i=0; i<strlen(str); i++) 
		if(str[i]!='\t' && str[i]!=' ') 
			break;

	n=0;
	for(;str[i]!='\0';)
		str[n++]=str[i++];

	str[n]='\0';
}

int ssv::GetNumWords(char junk[])
{
	int len = strlen(junk);
	int nc=0;
	trim(junk);

	for(int i=0; i<len; i++)
	{
		// find space
		if(isspace(junk[i]) )
		{
			while(isspace(junk[i+1]))
			{
				i++;  // skip all space
			}; // end while(isspace)
			nc++;
		}; // end if(isspace)
	};// end for(i)
	nc++;

	return nc;
}

void ssv::Read(char *fn)
{
#define LINE_SIZE 512
	char junk[LINE_SIZE];
	ifstream inf(fn);
	inf.getline(junk, LINE_SIZE); // skip the command line

	// count the number of frames
	while(inf.getline(junk, LINE_SIZE)) {
		_num_frames++;
	};

	// rewind
	inf.clear();
	inf.seekg(0,std::ios::beg);
	inf.getline(junk, LINE_SIZE); // skip the command line

	// malloc the space
	_data = new double * [_num_frames];
	for(int i=0; i<_num_frames; i++)
	{
		inf.getline(junk, LINE_SIZE);
		int nw = GetNumWords(junk);
		//cerr << junk << " nw = " << nw << endl;
		_nc.push_back(nw);
		_data[i] =  new double[nw];
	}

	// rewind
	inf.clear();
	inf.seekg(0,std::ios::beg);
	inf.getline(junk, LINE_SIZE); // skip the command line
	for(int i=0; i<_num_frames; i++)
		for(int j=0; j<_nc[i]; j++)
			inf >> _data[i][j];

	inf.close();
#undef LINE_SIZE
}

void ssv::Print()
{
	cerr << "----------ssv::Print()------------" << endl;
	cerr << "Num of frames = " << _num_frames << endl;
	//cerr << "Num of columns = " <<_num_columns<< endl;

	for(int i=0; i<_num_frames; i++)
	{
		cerr << i << ": " << _nc[i]<< ": ";
		for(int j=0; j<_nc[i]; j++)
		{
			cerr <<  _data[i][j] << " ";
		}
		cerr << endl;
	};
	cerr << "-----end of----ssv::Print()------------" << endl;
}
