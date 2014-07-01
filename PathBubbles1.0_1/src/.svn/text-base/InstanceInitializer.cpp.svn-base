#include "InstanceInitializer.h"

InstanceInitializer::InstanceInitializer()
{
	// 
	loadFile();
	return;
}

void InstanceInitializer::initInstance(InstanceManager *manager)
{
	for( int i = 0; i < this->_fileName.size(); i++ )
	{
		BubbleInstance *instance = new BubbleInstance( getSpe(i), getInd(i), getRun(i)  );
		manager->addInstance( instance );
	}
	return;
}

void InstanceInitializer::loadFile()
{
	std::ifstream file( "data/name.txt" );
	for( int i = 0; i < 6; i++ )
	{
		std::string fullname;
		file>>fullname;
		this->_fullName.append( QObject::tr( fullname.c_str() ) );
	}
	while( !file.eof() )
	{
		std::string filename;
		file>>filename;
		if( filename.length() > 1 )
		{
			this->_fileName.append( QObject::tr( filename.c_str() ) );
		}
	}
	return;
}

QString InstanceInitializer::getInd( int index )
{
	QStringList list = this->_fileName[index].split( QObject::tr("_") );
	return list.at(1);
}

QString InstanceInitializer::getRun( int index )
{
	QStringList list = this->_fileName[index].split( QObject::tr("_") );
	return list.at(2);
}

QString InstanceInitializer::getSpe( int index )
{
	QStringList list = this->_fileName[index].split( QObject::tr("_") );
	return list.at(1).left( 2 );
}