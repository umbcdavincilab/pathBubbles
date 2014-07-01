
#ifndef __SIM_DATA_ELEM_H
#define __SIM_DATA_ELEM_H


#define NEG_JUNK_DATA  -99999
#define POS_JUNK_DATA   99999
#define NOISE_THRE  0.0001

#include <vector>
#include <stdio.h>
#include <iostream>
#include "Point.h"
using namespace std;
//=====================================================
struct SimDataElem
{
  SimDataElem & operator=(const SimDataElem& b);

  // ========== variables ==========
  vector <double>  _elem; 
  // time-varying data

  float  _mean, _variance, _cv, _sd;
  // mean, variance, and coefficient of variation

  //Indicate which interval the data is in
  //(-#, -2*sd]	   ---> 0
  //(-2*sd, -1*sd] ---> 1
  //(-1*sd, 1*sd]  ---> 2
  //(1*sd, 2*sd]   ---> 3
  //(2*sd, +#)     ---> 4
  int _domain;

  // ========== functions ==========
  void SetStat(int begin, int end);  // begin frame and end frame number
  
  // set the _mean, _variance, _cv

  //-------------------------
  void Print();
  
}; 

// =========== sampling statistics ==========
struct  SimDataStat
{
  float   _mean[2]; 
  // min/max mean

  float   _variance[2];
  // min/max variance

  float   _cv[2];
  // min/max coefficient of variate

  void Print()
  {
    
  }

/*
  float                    _minMean;      
  // min mean
  float                    _maxMean;
  // max mean

  float                    _minVariance;
  // min variance
  float                    _maxVariance;
  // max variance

  float                    _mincv;
  // min coefficience of 
  float                    _maxcv;
  */
};

/*
  // those changed
  // vector <double>           _cmean;        // mean
  // vector <double>           _cvariance;    // variance
  // vector <double>           _ccv;          // coefficient of variation

  float                    _cminMean;      // get the range of mean,
  float                    _cmaxMean;   
  float                    _cminVariance;  // variance,
  float                    _cmaxVariance;
  float                    _cmincv;        // and coefficient of variation
  float                    _cmaxcv;
}; 
*/

#endif //__SIM_DATA_ELEM_H

