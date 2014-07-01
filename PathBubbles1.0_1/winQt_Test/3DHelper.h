#ifndef TDHELPER_H
#define TDHELPER_H

#include <math.h>
#include <string>
#include "myColor3.h"
#include <GL/glut.h>
#include <QPointF>
#include <QDialog>
#include <QColor>
#include "LabCut.h"
#include "LabObject.h"
//#include "AllBats.h"
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
	static myColor3 getBoarderColorByType(int type);
	static myColor3 getBoarderColorByType(int type, int id);

	static myColor3 getInnerColorByType(int type);
	static myColor3 getInnerColorByType(int type, int id);

	static int getNumberNDigital(float test, int Nb);
	static int getfirstPartNumber(float test, int Nb);
	static QDialog* createDialog(const QString &windowTitle);
	static int LengthOfQPointF( QPointF pos );
	static QColor getColorByType( int type );
	static float myAbs( float input );
	static void initOpenManager( OpenGLScene* scene, LabObject* lab, LabCut* cut, GroupManager* manager);
	static bool overlaid(QRectF left, QRectF right);
	static float value(float n1,float n2,float hue);
    static void HLS_TO_RGB(float h,float l,float ss,float &r,float &g,float &b);
	static float max1(float r, float g, float b);
    static float min1(float r, float g, float b);
    static void RGB_TO_HLS(float r,float g,float b,float &h,float &l,float &s);
	static void GetMultHue(float value, int type, float &r, float &g, float &b );//http://colorbrewer2.org/	
    static myColor3 getColorByUserName(int &id);
	static QString getHtmlColorByUserName(int &id);
	static vector<QString> getLegendParameter(float &min, float &max, int type, int preferedItv, int &itv, int &dl, float &mitv);// caculate the scale number and diginal length after . for a legend
	//static OpenManager *getOpenManager();
};

#endif