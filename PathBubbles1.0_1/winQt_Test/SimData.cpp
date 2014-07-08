
#include "SimData.h"

//#include <algorithm.h>

#define BUFSIZE  1280

//#define QUERY_THRE  0.0001// query threshold

using namespace std;

void SimData::init()
{
}

SimData::SimData(char *infname, int timeSteps, int begin, int end)
{
	ifstream  inf(infname, ios::in);
	char lineBuf[BUFSIZE], *token, sToken[BUFSIZE];
	int  pIndex; 
	init();

	this->_timeSteps = timeSteps;
	this->_begin = begin;
	this->_end = end;

	while(inf)
	{
		char pName[256];
		double val;

		SimDataElem data;

		inf >> pIndex;
		inf >> pName;

		//cerr << pIndex  << " " << pName << " "  ;
		// read in all data
		for(int ti=0; ti<timeSteps; ti++)
		{
			inf >> val;
			(data._elem).push_back(val);
			//cerr << val << " ";
		}

		// get the stat
		data.SetStat(begin, end); //calculate the mean, variance and cv
		_data.push_back(data);
		

		(data._elem).clear();
	}; 
	this->_np = pIndex+1;

	setStatMinMax();
	setStatFiveNumbers();

	inf.close();
}

SimData::SimData(vector<vector<double>> inputData, int timeSteps, int begin, int end)
{
	//ifstream  inf(infname, ios::in);
	//char lineBuf[BUFSIZE], *token, sToken[BUFSIZE];
	int  pIndex=0; 
	init();

	this->_timeSteps = timeSteps;
	this->_begin = begin;
	this->_end = end;

	for(int i=0; i<inputData.size(); i++) 
	{
        //char pName[256];
		double val;
		SimDataElem data;
				
		//inf >> pIndex;
		//inf >> pName;
		//cerr << pIndex  << " " << pName << " "  ;
		//read in all data
		for(int ti=0; ti<timeSteps; ti++)
		{
			//inf >> val;
			(data._elem).push_back(inputData[i][ti]);
			//cerr << val << " ";
		}
		// get the stat
		data.SetStat(begin, end); //calculate the mean, variance and cv
		_data.push_back(data);
		
		(data._elem).clear();
		pIndex++;
	}
	/*while(inf)
	{
		char pName[256];
		double val;

		SimDataElem data;

		inf >> pIndex;
		inf >> pName;

		//cerr << pIndex  << " " << pName << " "  ;
		// read in all data
		for(int ti=0; ti<timeSteps; ti++)
		{
			inf >> val;
			(data._elem).push_back(val);
			//cerr << val << " ";
		}

		// get the stat
		data.SetStat(begin, end); //calculate the mean, variance and cv
		_data.push_back(data);		

		(data._elem).clear();
	}; */
	this->_np = pIndex;

	setStatMinMax();
	setStatFiveNumbers();

	//inf.close();
}


SimData::SimData(char *infname)
{
	ifstream  inf(infname, ios::in);

	char lineBuf[BUFSIZE], *token, sToken[BUFSIZE];

	// skip those junk lines
	inf.getline(lineBuf, BUFSIZE);
	inf.getline(lineBuf, BUFSIZE);
	inf.getline(lineBuf, BUFSIZE);
	inf.getline(lineBuf, BUFSIZE);
	sscanf(lineBuf, "%s %d %s", sToken, &_timeSteps, sToken);
	while(inf)
	{
		inf.getline(lineBuf, BUFSIZE);
		sscanf(lineBuf, "%s", sToken);
		if( strcmp(sToken, "<name>" )==0 )
		{
			vector <double>  data;
			for(int li =0; li<_timeSteps; li++)
			{
				double val;
				inf.getline(lineBuf, BUFSIZE);
				// scan the data
				sscanf(lineBuf, "%s %lf %s", sToken, &val, sToken);
				data.push_back(val);
			}; // end for(li)
			//_data.push_back(data);
			data.clear();
		}
	} // end while(inf)

	_np = _data.size();

	inf.close();
}


SimData::~SimData()
{
}
// 
// TODO: 
// void SimData::RenderColorLegend(int sx, int sy)
// {
// 	int pointSize = 5;
// 	for(int i=0; i<8; i++)
// 	{
// 		glRasterPos2f(sx + i*5, sy); // render  a horizontal bar
// 		glDisable(GL_TEXTURE_2D);
// 	}
// }

void SimData::Print()
{
}

void SimData::PrintDataElem()
{
	cerr << "========= SimData::PrintDataElem ========" << endl;
	for(int di=0; di<_np; di++)
	{
		_data[di].Print();
	}
	cerr << endl;
	cerr << endl;
}

void SimData::PrintSumStat()
{
	{

		cerr << "======== all data stat ========" << endl;
		cerr << "mean range: " << _mean[0] << ", " <<  _mean[1] << endl;
		cerr << "Variance range: " 
			<< _variance[0]<< ", " <<  _variance[1]<<endl;
		cerr << "coefficient of variation range: " 
			<< _cv[0]<< ", " <<  _cv[1]<<endl;

		cerr << "======== changed data stat ========" << endl;
		cerr << "num of elements changed: " << _cdata.size();
		cerr << "mean range: " << _mean[0] << ", " <<  _mean[1] << endl;
		cerr << "Variance range: " 
			<< _variance[0]<< ", " <<  _variance[1]<<endl;
		cerr << "coefficient of variation range: " 
			<< _cv[0]<< ", " <<  _cv[1]<<endl;

		cerr << "_cFiveNumbers_lower= "  << _cFiveNumbers[0] 
		<< "\n_cFiveNumbers_Q1= " << _cFiveNumbers[1] 
		<< "\n_cFiveNumbers_median= "  << _cFiveNumbers[2] 
		<< "\n_cFiveNumbers_Q3= "  << _cFiveNumbers[3] 
		<< "\n_cFiveNumbers_upper= " << _cFiveNumbers[4] 
		<< "\n_cFiveNumbers_IQR= " << _cFiveNumbers[5] << endl;

		cerr << "=== count the distribution === " << endl;
		cerr << "_cBinElemCounter: " <<_cBinElemCounter[0]<<", "
			<<_cBinElemCounter[1]<<" "<<_cBinElemCounter[2]<<" "
			<<_cBinElemCounter[3]<<", "<<_cBinElemCounter[4]<<" "
			<<_cBinElemCounter[5]<<endl;
	}

}

void SimData::PrintIntervalDistribution()
{
	int count[5] = {0};
	for(int di=0; di<_np; di++)
	{
		if(_data[di]._domain == 0) count[0]++;
		else if(_data[di]._domain == 1) count[1]++;
		else if(_data[di]._domain == 2) count[2]++;
		else if(_data[di]._domain == 3) count[3]++;
		else if(_data[di]._domain == 4) count[4]++;
	}
	for(int i=0; i<5; ++i)
	{
		cerr<<"interval:"<<i<<" "<<count[i]<<endl;
	}
}


void SimData::setStatMinMax()
{
	assert(_begin>=0 && _end<=_timeSteps && _begin<=_end); // within range
	_mean[0]=9999;_mean[1]=-9999; _variance[0]=9999; _variance[1]=-9999;
	_cv[0]=9999;_cv[1]=-9999;

	// set the  max and min values
	for(int pi=0; pi<_np; pi++)
	{
		if(_data[pi]._mean > _mean[1]) _mean[2]=_data[pi]._mean;
		if(_data[pi]._mean < _mean[0]) _mean[0]=_data[pi]._mean;
		if(_data[pi]._variance > _variance[1]) _variance[1]=_data[pi]._variance;
		if(_data[pi]._variance < _variance[0]) _variance[0]=_data[pi]._variance;
		if(_data[pi]._cv > _cv[1]) _cv[1]=_data[pi]._cv;
		if(_data[pi]._cv < _cv[0]) _cv[0]=_data[pi]._cv;
	}; // end for(pi)
};

bool mysortcv(SimDataElem a, SimDataElem b)
{
	return (a._cv < b._cv);
}

bool mysortv(SimDataElem a, SimDataElem b)
{
	return (a._variance < b._variance);
}

void SimData::setStatFiveNumbers() 
{
	vector<SimDataElem> sorted_cv= _data;
	sort(sorted_cv.begin(), sorted_cv.end(), mysortcv);
	int size = sorted_cv.size();

	//cerr<< "-------------------CV-------------------" << endl;
	for(int i=0; i<sorted_cv.size(); i++)
	{
		sorted_cv[i]._cv = log(sorted_cv[i]._cv);
		//cerr << sorted_cv[i]._cv << endl;
	}
	//cerr<< "---------end ----------CV-------------------" << endl;
	if(size==0)
	{	
		_cFiveNumbers[1]=0;
		_cFiveNumbers[2]=0;
		_cFiveNumbers[3]=0;
		_cFiveNumbers[5]=0;
		_cFiveNumbers[0]=0;
		return;
    }

	_cFiveNumbers[1]=sorted_cv[(int)( (float)(size)*0.25)]._cv;
	_cFiveNumbers[2]=sorted_cv[(int)( (float)(size)*0.5)]._cv;
	_cFiveNumbers[3]=sorted_cv[(int)( (float)(size)*0.75)]._cv;
	_cFiveNumbers[5]= (_cFiveNumbers[3]-_cFiveNumbers[1]) * 1.5;
	_cFiveNumbers[0]=_cFiveNumbers[1]-_cFiveNumbers[5];

	// 	if(_cFiveNumbers[0] < QUERY_THRE)   // set to the data range
	// 	{
	// 		_cFiveNumbers[0]= QUERY_THRE;
	// 	}
	_cFiveNumbers[4]=_cFiveNumbers[3]+_cFiveNumbers[5];

	// assign the bin index to SimDataElem
	vector<SimDataElem>::iterator dit;
	//for(dit=_data.begin(); dit!=_data.end(); ++dit)
	//{
	//(*dit)._cv_bin_index = findCVBin( (*dit)._cv);
	//};

	//Compute variance five numbers
	sort(sorted_cv.begin(), sorted_cv.end(), mysortv);
	// 	cerr<< "-------------------V-------------------" << endl;
	// 	for(int i=0; i<sorted_cv.size(); i++)
	// 	{
	// 		sorted_cv[i]._cv = log(sorted_cv[i]._cv);
	// 		cerr << sorted_cv[i]._cv << endl;
	// 	}
	// 	cerr<< "---------end ---------V-------------------" << endl;

	_vFiveNumbers[1]=sorted_cv[(int)( (float)(size)*0.25)]._variance;
	_vFiveNumbers[2]=sorted_cv[(int)( (float)(size)*0.5)]._variance;
	_vFiveNumbers[3]=sorted_cv[(int)( (float)(size)*0.75)]._variance;
	_vFiveNumbers[5]= (_vFiveNumbers[3]-_vFiveNumbers[1]) * 1.5;
	_vFiveNumbers[0]=_vFiveNumbers[1]-_vFiveNumbers[5];

	// 	if(_cFiveNumbers[0] < QUERY_THRE)   // set to the data range
	// 	{
	// 		_cFiveNumbers[0]= QUERY_THRE;
	// 	}
	_vFiveNumbers[4]=_vFiveNumbers[3]+_vFiveNumbers[5];
}


int  SimData::findCVBin(double data)
{
	for(int i=0; i<5; i++)
	{
		if(data<_cFiveNumbers[0]) 
			return 0;
		else if(data>_cFiveNumbers[i]  && data<_cFiveNumbers[i+1])
			return i;
	};
	return 5;  //last one
}
// 
// void SimData::setElemColor()
// {
// 	for(int pi=0; pi<_np; pi++)
// 	{
// 		if(_data[pi]._changed!=1) // junk or not changed
// 		{
// 			(_data[pi]._co).r = mc[0].r;
// 			(_data[pi]._co).g = mc[0].g;
// 			(_data[pi]._co).b = mc[0].b;
// 			(_data[pi]._co).a = mc[0].a;
// 		}
// 		else
// 		{
// 			(_data[pi]._co).r = mc[(_data[pi])._cv_bin_index].r;
// 			(_data[pi]._co).g = mc[(_data[pi])._cv_bin_index].g;
// 			(_data[pi]._co).b = mc[(_data[pi])._cv_bin_index].b;
// 			(_data[pi]._co).a = mc[(_data[pi])._cv_bin_index].a;
// 		}; // end if(_changed[pi]
// 	}; // end for(pi)
// }

//Pearson correlation: p(x,y) = cov(x,y) / sx*sy;  sx is the standard deviation of x
double SimData::CalculatePearsonCorrelation(int x, int y)
{
	vector<double> &vx = _data[x]._elem;
	vector<double> &vy = _data[y]._elem;
	double ux = _data[x]._mean;
	double uy = _data[y]._mean;
	double sx = sqrt(_data[x]._variance);
	double sy = sqrt(_data[y]._variance);

	double cov = 0;
	for(int i=_begin; i<_end; ++i)
		cov += ((vx[i] - ux) * (vy[i] - uy));

	double p = cov / (sx * sy * (_end - _begin));
	cerr<<"Pearson correlation is:"<<p<<endl;
	return p;
}