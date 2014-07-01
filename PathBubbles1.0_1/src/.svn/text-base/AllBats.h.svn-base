#ifndef ALLBATS_H
#define ALLBATS_H

#include <fstream>
#include <string>
using namespace std;

class AllBats
{
public:
	AllBats();
	virtual ~AllBats(){};

	string getNameByIndex( int index )
	{
		return _name[ index ];
	}
	float getMassByIndex( int index )
	{
		return _mass[index];
	}
	float getSpeedByIndex( int index )
	{
		return _speed[index];
	}
	int getIndexFromName( string name );
	int getTotalNum()
	{
		return 25;
	}
private:
	void loadData();
	string _name[25];
	float _mass[25];
	float _speed[25];
};

#endif