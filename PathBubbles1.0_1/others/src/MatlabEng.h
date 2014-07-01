#ifndef _MATLAB_ENGINE_H_
#define _MATLAB_ENGINE_H_
#ifndef FALSE
#define FALSE	0
#endif
#ifndef TRUE
#define TRUE	1
#endif

#include "engine.h"

//MATLAB library - Linking MATLAB libraries automatically
#pragma comment(lib, "libeng.lib")
#pragma comment(lib, "libmx.lib")
#pragma comment(lib, "libmat.lib")

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMatlabEng  
{
public:
	int OutputBuffer(char *p, int n);
	void OpenSingleUse(const char *startcmd, void *dcom, int *retstatus);
	int GetVisible(bool* value);
	int SetVisible(bool value);
	mxArray* GetVariable(const char* name);
	int PutVariable(const char *name, const mxArray *mp);
	int EvalString(const char* string);
	void Open(const char* StartCmd);
	int Close();
	CMatlabEng();
	virtual ~CMatlabEng();

protected:
	Engine* pEng;
};

#endif // _MATLAB_ENGINE_H_
