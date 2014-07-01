#include "MatlabEngThread.h"
#include <QMessageBox>

MatlabEngThread::MatlabEngThread( QString code )
{
	this->_matlab = NULL;
	this->_code = code;
	return;
}

MatlabEngThread::~MatlabEngThread()
{
	delete this->_matlab;
	return;
}

void MatlabEngThread::setParaName( QList<QStringList> name )
{
	_paraName = name;

	this->start();

	return;
}

QList< QList<MatResult> > MatlabEngThread::getResult()
{
	return _results;
}

void MatlabEngThread::run()
{
	//keqin
	/*
	//*************************************************
	this->_matlab = new CMatlabEng();
	_matlab->Open(NULL);
	_matlab->SetVisible(false);
	//**************************************************
	this->_para.setCode( this->_code );
	_matlab->EvalString( this->_code.toStdString().c_str() );

	MatResult tmpResult;
	QList<MatResult> tmpReList;

	for (int i = 0; i < _paraName.size(); i ++)
	{
		tmpReList.clear();

		for (int j = 0; j < _paraName[i].size(); j ++)
		{
			QString name = this->_paraName[i][j];
			mxArray* data = this->_matlab->GetVariable( name.toStdString().c_str() );
			if( !data || mxIsEmpty( data ) )
			{
				tmpResult.m = tmpResult.n = 0;
				return;
			}
			tmpResult.m = mxGetM( data );
			tmpResult.n = mxGetN( data );
			tmpResult.name = _paraName[i][j];
			double* prt = mxGetPr( data );
			tmpResult.data.clear();
			for( int i = 0; i < tmpResult.m*tmpResult.n; i++ )
			{
				tmpResult.data.append( prt[i] );
			}

			tmpReList.append( tmpResult );
		}
		_results.append( tmpReList );
	}
	
	emit execEnd();
	*/
	return;
}