#include "matPara.h"

matPara::matPara()
{
	return;
}

void matPara::setCode(QString code)
{
	this->_code = code;
	process();
	return;
}

QString matPara::getName(int index)
{
	return this->_paras[index];
}

int matPara::numOfResult()
{
	return this->_paras.size();
}

void matPara::process()
{
	QStringList list = this->_code.split( ";" );
	for( int i = 0; i < list.size(); i++ )
	{
		if( list[i].contains( "=" ) )
		{
			equalHit( list[i] );
		}
	}
	return;
}

void matPara::equalHit(QString code)
{
	QStringList list = code.split( " " );
	for( int i = 0; i < list.size(); i++ )
	{
		if( list[i].contains( "=" ) )
		{
			if( list[i].size() == 1 )
			{
				if( i > 0 && !this->_paras.contains(list[i]) )
				{
					this->_paras.append( list[i-1] );
				}
				return;
			}
			else
			{
				QStringList equ = list[i].split( "=" );
				if( i >= 0 && !this->_paras.contains( equ.first() ) )
				{
					this->_paras.append( equ.first() );
				}
				return;
			}
		}
	}
}

void matPara::clearAll()
{
	this->_paras.clear();
}