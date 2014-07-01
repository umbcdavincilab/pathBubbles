#ifndef MATLABENGTHREAD_H
#define MATLABENGTHREAD_H

#include <QThread>
#include <QString>
#include "MatlabEng.h"
#include "matPara.h"
#include <QList>

struct MatResult
{
	QString name;
	QList<double> data;
	int m;
	int n;
};

class MatlabEngThread : public QThread
{
	Q_OBJECT
public:
	MatlabEngThread( QString code );
	~MatlabEngThread();
	QList< QList<MatResult> > getResult();
	void run();
	void setParaName( QList< QStringList > name );

signals:
	void execEnd();

private:
	QString _code;
	QList< QStringList > _paraName;
	CMatlabEng* _matlab;
	matPara _para;
	QList< QList<MatResult> > _results;
	bool _MATLAB;
};

#endif