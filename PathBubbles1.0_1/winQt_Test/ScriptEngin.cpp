#include "ScriptEngin.h"
#include "OpenManager.h"

ScriptEngin::ScriptEngin( OpenManager* manager )
: QObject()
, DIS( false )
{
	this->_openManager = manager;
	this->_PARA[0] = 26;
	this->_PARA[1] = 1;
	return;
}

ScriptEngin::~ScriptEngin()
{
	return;
}

QString ScriptEngin::runScript( QList<PenItem*> script )
{
	QString result;
	if( script.size() < 1 || script[0]->getType() == 0 )
	{
		return result;
	}
	if( script[0]->getType() == 2 && script[0]->toQString() == QString( QObject::tr("open") ) )
	{
		if( handelOpen( script ) )
		{
			return result;
		}
		else
		{
			return errorAt( script[0]->toQString() );
		}
	}
	else if( script[0]->getType() == 2 && script[0]->toQString() == QString( QObject::tr("find") ) )
	{
		QList<QList<ItemBase*>> bats;
		QList<QList<float>> rates;
		if( handelFind( script, bats, rates ) )
		{
			emit needSlider( bats, rates );
			return result;
		}
		else
		{
			return errorAt( script[0]->toQString() );
		}
	}
	else if( script[0]->getType() == 2 && script[0]->toQString() == QString( QObject::tr("set") ) )
	{
		if( handelSet( script ) )
		{
			return result;
		}
		else
		{
			return errorAt( script[0]->toQString() );
		}
	}
	else
	{
		result.append( tr( "Unreconized command at " ) );
		result.append( script[0]->toQString() );
		return result;
	}
}

bool ScriptEngin::runCommand( QStringList words )
{
	/*if( words[0] == QString( QObject::tr("open") ) )
	{
		if( handelOpen( words ) )
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else if( words[0] == QString( QObject::tr("find") ) )
	{
		if( handelFind( words ) )
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else if( words[0] == QString( QObject::tr("set") ) )
	{
		if( handelSet( words ) )
		{
			return true;
		}
		else
		{
			return false;
		}
	}*/
	return false;
}

QString ScriptEngin::errorAt(QString name)
{
	QString result = QObject::tr( "Error at " );
	result.append( name );
	return result;
}

bool ScriptEngin::handelOpen( QList<PenItem*> words )
{
	int num = 0;
	int numberOfPara = words.size();
	for( int i = 1; i < numberOfPara; i++ )
	{
		num += this->_openManager->openBatByToken( words[i]->toQString() ).size();
	}
	if( num == 0 )
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool ScriptEngin::handelFind( QList<PenItem*> words, QList<QList<ItemBase*>> &bats, QList<QList<float>> &rates )
{
	bats.clear();
	rates.clear();
	int numberOfPara = words.size();
	if( numberOfPara < 3 )
	{
		return false;
	}
	//int markerIndex = this->resolveMarker( words[1] );
	for( int i = 3; i < numberOfPara; i++ )
	{
		QList<ItemBase*> bat;
		QList<float> rate;
		this->_openManager->findBatByToken( words[i]->toQString(), this->_discreptor, _PARA[0], _PARA[1], bat, rate );
		bats.append( bat );
		rates.append( rate );
	}
	return true;
}

void ScriptEngin::setDiscreptor( DESCRIPTOR dis )
{
	this->_discreptor = dis;
	this->DIS = true;
	return;
}

OpenManager* ScriptEngin::getOpenManager()
{
	return this->_openManager;
}

int ScriptEngin::resolveMarker(QString name)
{
	if( name == QString( tr("elbow") ) )
	{
		return 0;
	}
	else if( name == QString( tr("wrist") ) )
	{
		return 1;
	}
	else if( name == QString( tr("mcp3") ) )
	{
		return 2;
	}
	else
	{
		return -1;
	}
}

bool ScriptEngin::handelSet( QList<PenItem*> words )
{
	int numberOfPara = words.size();
	if( numberOfPara < 2 )
	{
		return false;
	}
	int paraIndex = this->resolveParameter( words[1]->toQString() );
	if( paraIndex < 0 )
	{
		return false;
	}
	this->_PARA[paraIndex] = words[2]->toQString().toInt();
	return true;
}

int ScriptEngin::resolveParameter(QString name)
{
	if( name == QString( tr("number") ) )
	{
		return 0;
	}
	else
	{
		return -1;
	}
}