#ifndef CAMBER_H
#define CAMBER_H

#include <iostream>
#include <sstream>
#include <QString>
#include <QFile>
#include <fstream>
#include <string>
#include <QPointF>
#include <cstdlib>
#include <math.h>
#include <GL/freeglut.h>
#include <3DHelper.h>
#include "LabObject.h"
#include "LabCut.h"
#include "Lab3D.h"
#include "Discreptor.h"

using namespace std;

class Camber
{
public:
	Camber(string name, int num = 100, int scale = 1, LabObject *lab = 0, LabCut *cut = 0 );
	explicit Camber( Camber& copy );
	static QString checkPath( QString );
	~Camber();
	void acceptDiscreptor( DESCRIPTOR dis );
	int getNumOfPointAtFrame( int marker, int index);
	void translateCurve()
	{
		glTranslatef( _center[0], _center[1], _center[2] );
	}
	void restoreCurve()
	{
		glTranslatef( -_center[0], -_center[1], -_center[2] );
	}
	float getCurrentCamberAt( int marker, int cur_frame );
	void initVolum();
	void getPointsAtFrame(int index, int marker, double *points);
	double getPercentage(int which, int index);
	bool getEnabled( int index )
	{
		return _ENABLED[index];
	}
	float getSize(int index);
	void loadSize();
	void loadSizeFor( int index );
	int getMaxNum( int index )
	{
		return NUMOFCAMBERSIZE[index];
	}
	void setCamberDisplay( int index, bool type )
	{
		_ENABLED[index] = type;
	}
	int getFrameOfCamber()
	{
		return Num;
	}
	LabCut* getCut()
	{
		return _cut;
	}
	LabObject* getLab()
	{
		return _lab;
	}

	void drawCamber( int cur_frame_num );
	void drawCurve( int cur_frame_num );
	void findMatchDiscreptor( DESCRIPTOR dis, int marker, float &rate, int &index );
	void drawCamberMatching( QPainter* painter );
private:
	float matchRate( int index, int marker );
	void init();
	void load();
	void loadCamberPoints( int index );
	int *numOfList[3];
	bool *_flag;
	bool _ENABLED[3];
	double **data[3];
	int NUMOFCAMBERSIZE[3];
	float *norm;
	double *size[3];
	double *area;
	double Max[3], Min[3];
	int Num;
	int currentSelected;
	string Name;
	double Scale;
	LabObject *_lab;
	LabCut *_cut;
	QList<int> match;
	DESCRIPTOR _dis;
	float _center[3];
};

#endif