#include "StatisticManager.h"

StatisticManager::StatisticManager( QString path )
: _path( path )
{
	this->load();
	return;
}

StatisticManager::~StatisticManager()
{
}

void StatisticManager::load()
{
	 QFile file( this->_path );
	 if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	 {
		 return;
	 }
	 //
     QTextStream in(&file);
	 QString nameLine = in.readLine();
	 _names.append( nameLine.split( "\t" ) );
     while (!in.atEnd()) 
	 {
         QString line = in.readLine();
		 QStringList list = line.split( "\t" );
		 this->_table.append( list );
     }
	 //**********************************************************
	 QFile file2( "data/1D_parameterData.txt" );
	 if (!file2.open(QIODevice::ReadOnly | QIODevice::Text))
	 {
		 return;
	 }
	 QTextStream in2(&file2);
	 while (!in2.atEnd()) 
	 {
         QString line = in2.readLine();
		 _1DparaName.append( line );
     }

	 QFile file3( "data/1DT_parameterData.txt" );
	 if (!file3.open(QIODevice::ReadOnly | QIODevice::Text))
	 {
		 return;
	 }
	 QTextStream in3(&file3);
	 while (!in3.atEnd()) 
	 {
		 QString line = in3.readLine();
		 _1DTparaName.append( line );
	 }

	return;
}

QStringList StatisticManager::get1DNameList()
{
	return _1DparaName;
}

QStringList StatisticManager::get1DTNameList()
{
	return _1DTparaName;
}

QStringList StatisticManager::get1DFromMatNameList()
{
	return _1DparaFromMatName;
}

QStringList StatisticManager::get1DTFromMatNameList()
{
	return _1DTparaFromMatName;
}

float StatisticManager::getValueAt( int x, int y )
{
	return this->_table[x][y].toFloat();
}

int StatisticManager::checkIndexByName( std::string sname )
{
	QString name( sname.c_str() );
	int result;
	//*****************************************
	for( int i = 0; i < this->_table.size(); i++ )
	{
		if( this->_table[i].contains( name ) )
		{
			result = i;
			return (result-1);
		}
	}
	//*****************************************
	return -1;
}

QString StatisticManager::getNameByIndex( int index )
{
	if (index < _1DparaName.size())
	{
		return _1DparaName[index];
	}
	else
	{
		index -= _1DparaName.size();
		if ( index < _1DTparaName.size() )
		{
			return _1DTparaName[index];
		}
		else
		{
			index -= _1DTparaName.size();
			if ( index < _1DparaFromMatName.size() )
			{
				return _1DparaFromMatName[index];
			}
			else 
			{
				index -= _1DparaFromMatName.size();
				if (index < _1DTparaFromMatName.size() )
				{
					return _1DTparaFromMatName[index];
				}
				else return "";
			}
		}
	}
}

void StatisticManager::indexAcceptedUpdate( int index )
{
	emit indexAccepted( index );
	return;
}

float StatisticManager::getValueAt(QString file, QString para)
{
	int yindex;
	
	yindex = _names.indexOf( para );
	for( int i = 0; i < this->_table.size(); i++ )
	{
		if( this->_table[i].contains( file ) )
		{
			return _table[i][yindex].toFloat();
		}
	}
	return 0;
}

void StatisticManager::addParaFromMat(QString para, QList<double> dataResult )
{
	if (dataResult.size() > 1)
	{
		_1DTparaFromMatName.append( para );
		_1DTResultFromMat.append( dataResult );
	}
	else
	{
		_1DparaFromMatName.append( para );
		_1DResultFromMat.append( dataResult );
	}
}

QList<double> StatisticManager::getParaValueFromMat( QString para )
{
	for (int i = 0; i < _1DparaFromMatName.size(); i ++)
	{
		if ( _1DparaFromMatName[i] == para )
		{
			return _1DResultFromMat[i];
		}
	}
	for (int i = 0; i < _1DTparaFromMatName.size(); i ++)
	{
		if ( _1DTparaFromMatName[i] == para )
		{
			return _1DTResultFromMat[i];
		}
	}
	QList<double> empty;
	return empty;
}

void StatisticManager::removePara( int typeIndex, int nameIndex )
{
	if ( typeIndex == 1 )
	{
		_1DparaName.removeAt( nameIndex );
	}
	else if (typeIndex == 2)
	{
		_1DTparaName.removeAt( nameIndex );
	}
	else if (typeIndex == 3)
	{
		_1DparaFromMatName.removeAt( nameIndex );
		_1DResultFromMat.removeAt( nameIndex );
	}
	else if (typeIndex == 4)
	{
		_1DTparaFromMatName.removeAt( nameIndex );
		_1DTResultFromMat.removeAt( nameIndex );
	}
}

void StatisticManager::updateMatData(QString pName, QList<double> dataResult)
{
	if (dataResult.size() > 1)
	{
		for (int i = 0; i < _1DTparaFromMatName.size(); i ++)
		{
			if (_1DTparaFromMatName[i] == pName)
			{
				_1DTResultFromMat[i] = dataResult;
			}
		}
	}
	else
	{
		for (int i = 0; i < _1DparaFromMatName.size(); i ++)
		{
			if (_1DparaFromMatName[i] == pName)
			{
				_1DResultFromMat[i] = dataResult;
			}
		}
	}
}