
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <iostream>
#ifndef __SOL_PARSER
#define __SOL_PARSER
using namespace std;
class SolParser
{
  public:
    SolParser(char *fn);
	virtual ~SolParser();

	void Print();

    // Gets
	vector < string > GetNames() {return _names;}
	vector < vector <float > > GetValues() {return _values; }
	vector < float> GetValues(int index) {return _values[index]; }

  protected:
	float setVariances(int startIndex, int endIndex);

  private:
    vector < vector <float> > _values;
	vector < string > _names;

	vector <float> _variance;
};

#endif //__SOL_PARSER
