#include "SMObject.h"

void SMObject::formalizeData()
{
	float MAX = -999, MIN = 999;
	for( int i = 0 ; i < this->_total_Number; i++ )
	{
		if( _tempData[i] == -999 )
		{
			if( i > 0 )
			{
				_tempData[i] = _tempData[i-1];
			}
		}
		if( MAX < _tempData[i] && _tempData[i] != -999 )
		{
			MAX = _tempData[i];
		}
		if( MIN > _tempData[i] && _tempData[i] != -999 )
		{
			MIN = _tempData[i];
		}
	}
	this->_MAX = MAX;
	this->_MIN = MIN;
	float step = _total_Number / 50.0;
	for( int i = 0; i < 50; i++)
	{
		if( _tempData[int( i * step + 0.5 )] == -999 )
		{
			_tempData[int( i * step + 0.5 )] = ( _tempData[int( i * step + 0.5 ) + 1] + _tempData[int( i * step + 0.5 ) - 1 ]) / 2.0;
		}
		_data[i] = ( _tempData[int( i * step + 0.5 )] - MIN ) / ( MAX - MIN );
	}
	return;
}

SMSpeedObject::SMSpeedObject( std::string path ) : SMObject( path, "Speed" )
{
	_TYPE = 3;
	loadData();
	return;
}

void SMSpeedObject::loadData()
{
	std::ifstream sinput( this->_path.c_str() );
	std::string temp;
	int index;
	for(int i = 0; i < 2; i++)
	{
		sinput>>temp;
	}
	while(!sinput.eof())
	{
		sinput>>index;
		sinput>>_tempData[index];
		_total_Number += 1;
	}
	_total_Number -= 1;
	formalizeData();
	return;
}

SMCamberObject::SMCamberObject( std::string path ) : SMObject( path, "Camber" )
{
	_TYPE = 1;
	loadData();
	return;
}

void SMCamberObject::loadData()
{
	std::ifstream cinput( this->_path.c_str() );
	std::string temp;
	int index;
	//			load camber
	for(int i = 0; i < 5; i++)
	{
		cinput>>temp;
	}
	while(!cinput.eof())
	{
		cinput>>index;
		cinput>>temp;
		cinput>>_tempData[index];
		cinput>>temp;
		cinput>>temp;
		_total_Number += 1;
	}
	_total_Number -= 1;
	formalizeData();
	return;
}

SMFrameObject::SMFrameObject( int MAX ) : SMObject( "", "Frame" )
{
	_TYPE = 2;
	_total_Number = MAX;
	loadData();
	return;
}

void SMFrameObject::loadData()
{
	for( int i = 0; i < this->_total_Number; i++ )
	{
		_tempData[i] = i;
	}
	formalizeData();
	return;
}