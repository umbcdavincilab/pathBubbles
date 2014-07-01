#ifndef TDHELPER_H
#define TDHELPER_H

#include <math.h>
#include <string>
#include "myColor3.h"
#include <GL/freeglut.h>
#include <QPointF>
#include <QDialog>
#include <QColor>
#include "LabCut.h"
#include "LabObject.h"
#include "AllBats.h"
//#include "OpenManager.h"

#define PI 3.1415926
//class QtBat;
class OpenGLScene;
class GroupManager;
//class AllBats;
//class LabObject;

class TDHelper
{
public:
	static float* getNormal(float *norm, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);
	static void matrix44By44( double* left, double* right, double* result );
	static void matrix44By41( double* left, double* right, double* result );
	static void load44IdentityMatrix( double* matrix );
	static double fromDegreeToRadio( int degree );
	static void text(char* string, void* font, float x, float y);
	static void drawHUD(GLfloat x, GLfloat y, char *message, float r, float g, float b);
	static float distance2( QPointF x, QPointF y );
	static int Max( int a, int b );
	static float Max( float a, float b );
	static int Min( int a, int b );
	static float Min( float a, float b );
	static std::string getDataPrefix();
	static bool notInBouding( QPointF pos, int width, int height );
	static bool inBounding( QPointF pos, int width, int height );
	static myColor3 getBoarderColor();
	static QColor getBoarderColor( int alpha );
	static QDialog* createDialog(const QString &windowTitle);
	static int LengthOfQPointF( QPointF pos );
	static QColor getColorByType( int type );
	static float myAbs( float input );
	static void initOpenManager( OpenGLScene* scene, LabObject* lab, LabCut* cut, GroupManager* manager, AllBats* bats );
	//static OpenManager *getOpenManager();
};

#endif