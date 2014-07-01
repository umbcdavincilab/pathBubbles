#ifndef SSV_H
#define SSV_H

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#include <assert.h>
#include <string.h>

using namespace std;


/*
read the file new4.csv
the structure of the trangle mesh
*/

class ssv {
public:
	ssv() { _num_frames=0; }
	ssv(char *fn);
	virtual ~ssv()
	{
		for(int i=0; i<_num_frames; i++)
			delete [] _data[i];
		delete _data;
	}

	virtual int GetNumFrames () const {return _num_frames;}
	virtual int GetNumColumnsByIndex (int i) const {return _nc[i];}
	virtual int GetNumWords(char junk[]);
	virtual bool GetDataByIndex(int frame_index, int column_index, float *output) const
	{
		assert(frame_index<_num_frames);
		assert(column_index<_nc[frame_index]);
		*output = _data[frame_index][column_index];
		return true;
	}
	virtual void Print();
	virtual void Read(char *fn);

protected:
	int _num_frames;
	vector<int> _nc; // num of columns
	double ** _data; // save in a matrix form

	void trim(char *str); // remove start and ending space
private:
};

#endif // SSV_H
