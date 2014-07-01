#ifndef CODESEGMENT_H
#define CODESEGMENT_H

#include "engine.h"
#include <QString>
#include <QList>
#include <QStringList>

#pragma comment(lib, "libeng.lib")
#pragma comment(lib, "libmx.lib")
#pragma comment(lib, "libmat.lib")

class matPara
{
public:
	matPara();
	void clearAll();
	void setCode( QString code );
	int numOfResult();
	QString getName( int index );
private:
	void process();
	void equalHit( QString code );
	QString _code;
	QList<QString> _paras;
};

#endif