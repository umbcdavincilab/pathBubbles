#include "WidgetManager.h"

myWidgetManager::myWidgetManager(int iLength)
{
	array = new QGroupBox*[ 1 ];
	Length = 0;
}

myWidgetManager::~myWidgetManager()
{
	delete array;
}

void myWidgetManager::addWidget(QGroupBox* input)
{
	array[ Length ] = input;
	Length += 1;
}

void myWidgetManager::removeWidget(QGroupBox *widget)
{
	for (int i = 0 ; i < Length; i++)
	{
		if( array[ i ] == widget)
		{
			for (int j = i+1; j < Length; j++)
			{
				array[ j-1 ] = array[ j ];
			}
			Length -= 1;
			break;
		}
	}
}

QGroupBox* myWidgetManager::queryWidget(int index)
{
	return array[index];
}