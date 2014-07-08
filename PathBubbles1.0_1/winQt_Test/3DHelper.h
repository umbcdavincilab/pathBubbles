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

#define MEDIUM_HIGHLIGHTcolor Color(255,128,0) //Color(20,20,255)
#define LIGHT_HIGHLIGHTCOLOR QColor(255,213,170,127)// QColor(220, 230, 255, 159) 
#define MEDIUM_HIGHLIGHTCOLOR QColor(255,128,0,255) //QColor(20,20,255,255)
#define HIGHLIGHTCOLOR QColor(255,213,170,159) //QColor(120, 130, 255, 127) //for shared, linked, differed
#define PRIME_PATH_COLOR   QColor(254,153,41,127)

#define RATELIMITCOLOR  QColor(255, 160, 160,255)
#define CROSSTALKCOLOR  QColor(160, 160, 255, 255)
#define ORTHOLOGYCOLOR  QColor(102,194,165, 255)
#define DOWNEXPRESSIONCOLOR QColor(64,64,255,255)
#define UPEXPRESSIONCOLOR QColor(230,68,113,255)
 
#define PROTEINCOLOR QColor(255,255,204,255) 
#define COMPLEXCOLOR QColor(255,226,183,255) 	
#define SMALLMOLECULECOLOR QColor(214,215,202,255)
#define PHYSICALENTITYCOLOR QColor(230,234,172,255)
#define DNACOLOR QColor(214,234,172,255)
#define SETCOLOR QColor(254,202,131,255)
#define REACTIONCOLOR QColor(255,255,255,255)

#define ACTIVATIONCOLOR QColor(0, 210, 50, 255)
#define INHIBITIONCOLOR QColor(255, 128, 0, 255)
#define BUBBLELINKCOLOR QColor(40,40,40,200)

#define COMPLETECOLOR QColor(0,90,50) 
#define INCOMPLETECOLOR QColor(161,217,155) 	
#define MISSINGCOLOR QColor(255,255,255,255)

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