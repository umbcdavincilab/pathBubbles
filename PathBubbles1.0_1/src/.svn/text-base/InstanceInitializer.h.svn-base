#ifndef INSTANCEINITIALIZER_H
#define INSTANCEINITIALIZER_H

#include <fstream>
#include <string>
#include <QList>
#include <QString>
#include <QObject>
#include "InstanceManager.h"

class InstanceInitializer
{
public:
	InstanceInitializer();
	void initInstance( InstanceManager* manager );
private:
	void loadFile();
	QString getName( int index )
	{
		return this->_fullName[index];
	}
	QString getInd( int index );
	QString getRun( int index );
	QString getSpe( int index );
	QList<QString> _fullName;
	QList<QString> _fileName;
};

#endif