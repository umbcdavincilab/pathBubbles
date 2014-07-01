
/*---------------------------------------------------------
 *    Jian Chen May 2011
 *    jian.chen@usm.edu
 *
 *    Read the sim file in the HumMod .sol format
 *
 *---------------------------------------------------------
*/


#include <iostream>
#include <fstream>
#include <assert.h>
#include <vector>
#include <algorithm>
//#include <stdlib.h>
//#include <string.h>
//#include <math.h>
#include "Point.h"
#include "Ring.h"
#include "Stroke.h"

#include "SimDataElem.h"


#ifndef __SIM_DATA_H
#define __SIM_DATA_H

using namespace std;

#define DEFAULT_SUB_BIN_SIZE 10
#define EDGE_BY_V  0
#define EDGE_BY_CV 1
#define EDGE_BY_DOMAIN 2
#define EDGE_BY_EXPRESSION 3
#define EDGE_BY_ORTHOLOGY 4
#define EDGE_BY_ORTHOLOGY_2 5

// #define COLOR_BY_V 0
// #define COLOR_BY_CV 1
// #define COLOR_BY_DOMAIN 2
//
// simulation data only
// read and store them
//
class SimData
{

public:
  SimData(char *inf);
  // read the .sol file and set the parameters
  SimData(char *inf, int timeSteps, int begin, int end);
  SimData(vector<vector<double>> inputData, int timeSteps, int begin, int end);
  // read the Yubao generated format and set the parameters

  virtual ~SimData();

  //TODO: public data should be private
  //vector <Color>            _color;        // set colors of the node
                      // use the colorBrewer


  void Print();
  void PrintDataElem();   // print the coefficient of variation of those changed
  void PrintSumStat();
  void PrintIntervalDistribution();


  int   GetTimeStep()        {return _timeSteps;}
  float GetNumOfParameters() {return _np;}
  double GetMinVariance() {return _variance[0];}
  double GetMaxVariance() {return _variance[1];}
  double GetMinCCV() {return _cv[0];}
  double GetMaxCCV() {return _cv[1];}
  double GetVariance(int index) {return _data[index]._variance;}
  //double GetOrthRate(int index) {return _data[index]._orthRate;} 
  double GetCV(int index) {return _data[index]._cv;}
  int    GetDomain(int index){ return _data[index]._domain;}
  double GetSd(int index){ return _data[index]._sd;}
  double GetMean(int index){ return _data[index]._mean;}
  vector<double> GetElem(int index){ return _data[index]._elem;}
  vector < SimDataElem > GetData(){return _data;}

  void GetFiveNumbersCV(float data[6]) 
  {
    for(int i=0; i<6; i++)
	{
       data[i]=_cFiveNumbers[i]; 
	}
  }; 

  void GetFiveNumbersV(float data[6])
  {
	  for(int i=0; i<6; i++)
	  {
		  data[i]=_vFiveNumbers[i]; 
	  }
  }
  void GetFiveNumbersAndCounter(float data[6], int count[6]) 
  {  
    for(int i=0; i<6; i++)
	{
      data[i]=_cFiveNumbers[i]; 
      count[i]=_cBinElemCounter[i]; 
	}
  }
  //void RenderColorLegend(int sx, int sy);
  // render it on the screen from location (sx, sy)
  //vector<Color> GetColor();
  int GetBinCat(int index);

  double CalculatePearsonCorrelation(int x, int y);

protected:
  // SETS and GETS
  void setStatMinMax();
  void setStatFiveNumbers();
  void setElemColor();

  int  findCVBin(double val);

  void init();

private:
  int                      _timeSteps; // sim timeSteps
  int                      _begin, _end; // sample a range 
  int                      _np;  // number of parameters
  //=================
  vector < SimDataElem >   _data;    // all sim data
  // summary stat for all entries
  double _mean[2], _variance[2], _cv[2];

  //=================
  vector < SimDataElem >   _cdata;   // those changed
  // summary stat for the changed entries
  //double _cmean[2], _cvariance[2], _ccv[2];
  float  _cFiveNumbers[6];     // stat: five number rules + IQR
  float _vFiveNumbers[6];
  int    _cBinElemCounter[6];     // stat: counter
  // how many falling between these five numbers
  //_cv_lower;0, _cv_Q1;1, _cv_median;2, _cv_Q3;3, _cv_upper;4, _cv_IQR;5
};

#endif //__SIM_DATA_H
