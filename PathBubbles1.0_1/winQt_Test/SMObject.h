#ifndef SMOBJECT_H
#define SMOBJECT_H

#include <string>
#include <fstream>
#include <QVector>

class SMObject 
{
public:
	SMObject( std::string path, std::string name ) : _path( path )
		, _total_Number( 0 )
		, _name( name )
	{
		_tempData = new float[150];
		for( int i = 0; i < 150; i++ )
		{
			_tempData[i] = -999;
		}
		for( int i = 0; i < 50; i++ )
		{
			_data[i] = -999;
		}
		return;
	}
	~SMObject()
	{
		delete _tempData;
	}
	virtual std::string getName()
	{
		return this->_name;
	}
	virtual int getType()
	{
		return this->_TYPE;
	}
	virtual float getMAX()
	{
		return _MAX;
	}
	virtual float getMIN()
	{
		return _MIN;
	}
	virtual int total_Number()
	{
		return _total_Number;
	}
	virtual void formalizeData();
	virtual float percentage_At( int index )
	{
		return _data[ index ];
	}
	virtual void loadData()
	{
		return;
	}
	virtual float getRealValueAt( int index )
	{
		//if( _data[index] > 1 )
		//{
		//	_data[index] = 1;
		//}
		//if( _data[index] < 0 )
		//{
		//	_data[index] = 0;
		//}
		return ( _MAX - _MIN ) * _data[index] + _MIN;
	}
protected:
	std::string _name;
	float *_tempData;
	float _data[50];
	std::string _path;
	int _total_Number;
	float _MAX, _MIN;
	int _TYPE;
};

class SMSpeedObject : public SMObject
{
public:
	SMSpeedObject( std::string path );
	void loadData();
};

class SMCamberObject : public SMObject
{
public:
	SMCamberObject( std::string path );
	void loadData();
};

class SMFrameObject : public SMObject
{
public:
	SMFrameObject( int MAX );
	void loadData();
};

#endif