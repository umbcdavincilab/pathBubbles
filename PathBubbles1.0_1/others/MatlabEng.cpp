#include "MatlabEng.h"

CMatlabEng::CMatlabEng()
{
	pEng = NULL;
}

CMatlabEng::~CMatlabEng()
{
	if (pEng!=NULL)
		Close();
}

void CMatlabEng::Open(const char *StartCmd)
{
	//pEng = engOpen(StartCmd); //keqin
}

int CMatlabEng::Close()
{
	int Result = 1; //engClose(pEng); //keqin
	if (Result==0)	//Success
		pEng=NULL;
	return Result;
}

int CMatlabEng::EvalString(const char *string)
{
	return (1/*engEvalString(pEng, string)*/); //keqin
}

mxArray* CMatlabEng::GetVariable(const char *name)
{
	return ( NULL /*engGetVariable( pEng, name )*/); ////keqin
}


int CMatlabEng::GetVisible(bool* value)
{
	return (1/*engGetVisible(pEng, value)*/); //keqin
}

void CMatlabEng::OpenSingleUse(const char *startcmd, void *dcom, int *retstatus)
{
	//pEng=engOpenSingleUse(startcmd, dcom, retstatus); //keqin
}

int CMatlabEng::OutputBuffer(char *p, int n)
{
	return (1/*engOutputBuffer(pEng, p, n)*/);////keqin
}

int CMatlabEng::PutVariable(const char *name, const mxArray *mp)
{
	return (1/*engPutVariable(pEng, name, mp)*/); //keqin
}

int CMatlabEng::SetVisible(bool value)
{
	return (1/*engSetVisible(pEng, value)*/); //keqin
}
