#ifndef BAT_H
#define BAT_H


//#include <G3D/G3DAll.h>
//#include <GLG3D/GLG3D.h>
#include <GL/freeglut.h>
#include <QFile>
#include <QPointF>
#include <QRect>
#include "ssv.h"
#include "MarkersMotion.h"
#include "LabObject.h"
#include "LabCut.h"
#include "Lab3D.h"
#include "myColor3.h"
#include "Camber.h"
#include "BatContext.h"
#include <QList>
#include <iostream>
#include <string>
#include <math.h>

#ifndef PI
#define PI 3.14159265358979
#endif
#ifndef N
#define N 16
#endif
#ifndef theta
#define theta PI/N
#endif

#ifndef DefaultCylinderSize
#define DefaultCylinderSize 0.02
#endif

#ifndef DefaultSphereSize
#define DefaultSphereSize 0.01
#endif

class Bat
{
public:
	Bat( LabObject *Lab, LabCut *Cut,int Scale, std::string name );
	static QString checkPath( QString name );
	explicit Bat( Bat& target );
    ~Bat();

    //*****************************************************************
    //          My function
    //*****************************************************************
    void renderScene();   //	overall enter point of bat flight
	void renderVolum();
	void renderWakeFromTo();
	void determineShift();
	void drawBatBody();
	void setCompareBatColor( QColor color );
	void switchMarkerState();
	void getboundingBox( int framenum );
	QColor getCompareBatColor()
	{
		return this->frame_color;
	}
	int getNumOfMarkers()
	{
		return this->num_markers;
	}
	void switchLevel1Shift()
	{
		if( _LEVEL1ENABLED == 0.0 )
		{
			_LEVEL1ENABLED = 1.0;
		}
		else
		{
			_LEVEL1ENABLED = 0.0;
		}
	}
	void switchLevel2Shift()
	{
		if( _LEVEL2ENABLED == 0.0 )
		{
			_LEVEL2ENABLED = 1.0;
		}
		else
		{
			_LEVEL2ENABLED = 0.0;
		}
	}
	void setWakeRender( int begin, int to )
	{
		_wakeRenderStart = begin;
		_wakeRenderEnd = to;
	}
	void setWakeMargin( float margin )
	{
		_margin = margin;
	}
	int getTotalNumberOfFrame()
	{
		return num_frames;
	}
	void resetCurrentNumberOfFrame()
	{
		currentX = 0;
		cur_frame_num = 0;
		return;
	}
	void setHighlight( QList<int> list )
	{
		_highLightList = list;
	}
	void highlightSelectedWake( QRect selectedRect );
	Camber* getCamber()
	{
		return camber;
	}
	int getTotalNumberOfMarkers()
	{
		return num_markers;
	}
	int getTotalNumberOfTri()
	{
		return num_tri;
	}
	int getScale()
	{
		return scale;
	}
	LabObject* getLab()
	{
		return lab;
	}
	LabCut* getCut()
	{
		return cut;
	}
	void setWireframe( bool input )
	{
		WIREFRAME = input;
	}
	void setSurface( bool input )
	{
		SURFACE = input;
	}
	void setSymbols( bool input )
	{
		SYMBOLS = input;
	}
	bool getSymbols()
	{
		return SYMBOLS;
	}
	void rotateXY( int offX, int offY )
	{
		rotateX -= offX;
		rotateY -= offY;
		updateRotationMatrix();
		updateRotationMatrix_Shadow();
	}
	BatContext getBatContext();
	void setCurrentNumberOfFrame( float input );
	int getCurrentNumberOfFrame()
	{
		return cur_frame_num;
	}
	std::string getname()
	{
		return this->_fileName;
	}
	QString getQName()
	{
		return QString( this->_fileName.c_str() );
	}
	void Key_forward();
	void Key_backward();
	void acceptContext( BatContext context );
	void writeMarkersData( double** returns );
	void writeWakeData( double** left, double** right );
	void writeMeshData( int** index, int* length );
	void updateRotationMatrix( );
	void updateRotationMatrix_Shadow();
	void rotateXYby()
	{
		glRotated( rotateY % 360, 1.0, 0.0, 0.0);
		glRotated( rotateX % 360, 0.0, 1.0, 0.0);
	}
	void restoreRotation()
	{
		glRotated( -rotateX % 360, 0.0, 1.0, 0.0);
		glRotated( -rotateY % 360, 1.0, 0.0, 0.0);
	}
	void updateTranslateMatrix( double x, double y, double z );
	void updateTranslateMatrix_Shadow( double x, double y, double z );
	void translateBy()
	{
		glTranslatef( -currentX, 0, 0 );
		//glTranslatef( -currentX, 0, currentZ );
	}
	void restoreTranslation()
	{
		glTranslatef( currentX, 0, 0 );
		//glTranslatef( currentX, 0, -currentZ );
	}
	void getCurrentRotaTransMatrix();
	void beginSelectWake( int index );
	void endSelectWake( int &begin, int &end );
	void getMarker5ToSceneAtFrame( int index, int* pos );
	void getMarkerXToSceneAtFrame( int index, int marker, int* pos );
	void highlightNearMarkers( QPointF currentMouseOn, QPointF itemPos, int Height );
	void setCylinderPara(float csize)
	{
		_arrowCylinderSize = csize;
	}
	void setSpherePara(float ssize)
	{
		_arrowSphereSize = ssize;
	}
	float getCylinderSize()
	{
		return _arrowCylinderSize;
	}
	float getSphereSize()
	{
		return _arrowSphereSize;
	}
	void setArrowType(int atype)
	{
		_arrowType = atype;
	}
	int getArrowType()
	{
		return _arrowType;
	}
	void setSymbolColor(int marker, int cur_frame);
protected:
	//******************************
	void loadWake();
	void loadMarkers();
	void loadMesh();
	//******************************
	void drawBat();
	void drawFrame();
	void drawCamber();
	void drawMarkers();
	void drawBone();
	void drawWave();
    //************************************************************************************
    //              Private data and functions
    //************************************************************************************
private:
	//*****************************************
	double xmin, xmax, ymin, ymax, zmin, zmax;
	//*****************************************
	double currentX;
	double currentZ;
	int num_markers;
    int num_frames;
	int cur_frame_num;
	int num_tri;
	int scale;
	double _matrix[16];
	double _matrix_shadow[16];
	//*****************************************
	int rotateX, rotateY;
	//*****************************************
	//			wake structure
	//*****************************************
	double **wakeLeft, **wakeRight;
	//*****************************************
	//			markers data
	//*****************************************
	double **_markers;
	//*****************************************
	//			tri data
	//*****************************************
	int **meshIndex;
	int *meshLength;
	//*****************************************
	//*****************************************
	//			state data for each box
	//*****************************************
	bool SURFACE, WIREFRAME, SYMBOLS;
	int bone[22];
	QList<int> _highLightList;
	//*****************************************
	std::string _fileName;
	std::string _path;
	std::string _camberPerfix;
	std::string _camberPath;
	//*****************************************
	bool _SELECTINGWAKE;
	int _wakeStart, _wakeEnd;
	int _sceneWidth;
	//*****************************************
	//			camber data
	//*****************************************
	//*****************************************
    ssv *batTri;
	LabObject *lab;
	LabCut *cut;
    MarkersMotion *markerPositions;
	float *norm;
	Camber *camber;
	//*****************************************
	int _wakeRenderStart;
	int _wakeRenderEnd;
	float _margin;
	//*****************************************
	float shiftVectorForLevel1[3];
	float shiftVectorForLevel2[3];
	float _LEVEL1ENABLED;
	float _LEVEL2ENABLED;
	//*****************************************
	int _HIGHLIGHTED;
	float _HIGHLIGHTCOLOR[3];
	float _MARKERCOLOR[3];
	float _THRESHHOLD;
	//*****************************************
	//			private functions
	//*****************************************
	bool belongtoWing(int index);
	void drawWavewithSymbols();
	bool needToBeHighlighted( int index );
	void updateShiftVector();
	void posShiftForLeft( int triangleIndex );
	void negShiftForLeft( int triangleIndex );
	void posShiftForRight( int triangleIndex );
	void negShiftForRight( int triangleIndex );
	bool isBone(int index1, int index2);
	bool isJoint(int index);
	void extractName();
	void drawBox();
	void drawSurface(double *v1, double *v2, double *v3, double *v4);
	void drawTriangle(double *v1, double *v2, double *v3);
	void drawArrow(double *a, double *b);
	void reOrder(double *x, double *y, double *z, double *p1, double *p2, double *p3);
	void renderShadow();
	void getNormal(float *norm, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);
	void Shift( double *v )
	{ 
		double temp = v[1]; 
		v[1] = v[2]; 
		v[2] = temp;
	}
	void drawCylinder(double x1, double y1, double z1, double x2, double y2, double z2, double size);
 	void drawCylinder( double *pos1, double *pos2, double size);
	void drawMarker(float x, float y, float z);
	void initTexture();
	void initCoordinate();
	void initBones();
	float Alpha(int index);
	int _STATE; // 0 for default
	QColor frame_color;

	float _arrowCylinderSize;
	float _arrowSphereSize;
	int _arrowType; //0 arrow, 1 sphere, 2 cylinder
	bool *_wingDisplayFlag; //display or not
};

#endif
