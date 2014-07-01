#include "AllBats.h"

AllBats::AllBats()
{
	loadData();
	return;
}

void AllBats::loadData()
{
	ifstream input;
	input.open("data/bat_Infor.csv");
	if( input.fail() )
	{
		return;
	}
	for( int i = 0; i < 25; i++ )
	{
		input>>_name[i];
	}
	for( int i = 0; i < 25; i++ )
	{
		input>>_mass[i];
	}
	for( int i = 0; i < 25; i++ )
	{
		input>>_speed[i];
	}
	return;
}

int AllBats::getIndexFromName( string name )
{
	for( int i = 0; i < 25; i++ )
	{
		int index = _name[i].find( name );
		if( index >= 0 )
		{
			return i;
		}
	}
	return -1;
}