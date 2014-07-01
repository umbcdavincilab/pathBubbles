#include "SimDataElem.h"

SimDataElem & SimDataElem::operator=(const SimDataElem& b)
{
  if(this == &b)
    return *this;

  _elem = b._elem;
  _mean = b._mean;
  _variance = b._variance;
  _cv = b._cv;
  _sd = b._sd;
  _domain = b._domain;

  return *this;
}

void SimDataElem::Print()
{
     int nb = _elem.size();
     cerr << "SimDataElem: count:  " << nb << endl;
     vector<double>::iterator it;

     for (it=_elem.begin(); it!=_elem.end(); ++it)
     {
       cerr << " " << *it ;
     }; // end for(it)

	 cerr << endl;

	 cerr << " mean: " << _mean << endl;
	 cerr << " variance: " << _variance << endl;
	 cerr << " coefficient of variation: " << _cv << endl;
	 cerr << " standard deviation: " << _sd << endl;
};


void SimDataElem::SetStat(int begin, int end)
{
	double sum=0., variance=0., cv=0;
	int cts = end-begin;  // count the total number of elements

	// mean
	for(int di=begin; di<end; di++) 
	{
		sum +=_elem[di];
	}; // end for(di)
	_mean = sum/(double)cts;

	// variance
	for(int di=begin; di<end; di++)
	{
		variance += (_elem[di]-_mean)*(_elem[di]-_mean);
	}; // end for(di)

	variance /= (double)cts;
	_variance = variance;
	_cv = sqrt(variance)/fabs(_mean);

	//standard deviation
	_sd = sqrtf(variance);

	//Calculate which interval the deviation of the data element is in
	//First, find the element with the max absolute
	int index = 0, max_elem = 0;
	for(int di=begin; di<end; ++di)
	{
		if(max_elem < fabs(_elem[di]))
		{
			max_elem = _elem[di];
			index = di;
		}
	}

	//Second, decide which interval the data is in according to this element
	//The interval: 
	//(-#, -2*sd]    ---> 0
	//(-2*sd, -1*sd] ---> 1
	//(-1*sd, 1*sd]  ---> 2
	//(1*sd, 2*sd]   ---> 3
	//(2*sd, +#)     ---> 4
	if(_elem[index] < -2*_sd + _mean)
	{
		_domain = 3;
	}
	else if(_elem[index] >= -2*_sd + _mean && _elem[index] < -1*_sd + _mean)
	{
		_domain = 1;
	}
	else if(_elem[index] >= -1*_sd + _mean && _elem[index] < 1*_sd + _mean)
	{
		_domain = 0;
	}
	else if(_elem[index] >= 1*_sd + _mean && _elem[index] < 2*_sd + _mean)
	{
		_domain = 2;
	}
	else if(_elem[index] > 2*_sd + _mean)
	{
		_domain = 4;
	}

}


