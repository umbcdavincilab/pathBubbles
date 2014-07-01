#include "OpenManager.h"
#include "3DHelper.h"
#include "openglscene.h"
#include "qtbat.h"

void TDHelper::matrix44By44(double *left, double *right, double *result)
{
	for( int i = 0; i < 4; i++ )
	{
		for( int j = 0; j < 4; j++ ) 
		{
			result[i*4+j] = 0;
			for( int k = 0; k < 4; k++ )
			{
				result[i*4+j] += left[i * 4 + k]*right[k * 4 + j];//[i][k]   [j][k]
			}
		}
	}
	return;
}

void TDHelper::matrix44By41( double* left, double* right, double* result )
{
	for( int i = 0; i < 4; i++ )
	{
		result[i] = 0;
		for( int j = 0; j < 4; j++ )
		{
			result[i] += left[i*4 + j] * right[j];
		}
	}
}

void TDHelper::load44IdentityMatrix( double* matrix )
{
	for( int i = 0; i < 16; i++ )
	{
		matrix[i] = 0;
	}
	matrix[0] = 1;
	matrix[5] = 1;
	matrix[10] = 1;
	matrix[15] = 1;
	return;
}

void TDHelper::text(char* string, void* font, float x, float y)
{
   char* p; // Temp pointer to the string varible passed in

   // Specify the initial position on the screen where the text
   // will be diaplayed
   glRasterPos2f(x, y);

   // Parse the string passed in and display to screen one char at a time
   for (p = string; *p; p++)
      glutBitmapCharacter( font , *p);
}

void TDHelper::drawHUD(GLfloat x, GLfloat y, char *message, float r, float g, float b)
{
	GLdouble vmat[16], pmat[16];
    GLint viewport[4];

    //void *font =  GLUT_BITMAP_TIMES_ROMAN_24;
    void *font =  GLUT_BITMAP_HELVETICA_12;
    glGetDoublev(GL_MODELVIEW_MATRIX, vmat);
    glGetDoublev(GL_PROJECTION_MATRIX, pmat);
    glGetIntegerv(GL_VIEWPORT, viewport);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.f, (GLfloat)viewport[2], (GLfloat) viewport[3], 0.f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3f(r, g, b);
    text(message, font, x, y);

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(pmat);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(vmat);

}

float TDHelper::distance2(QPointF x, QPointF y)
{
	return sqrt( pow( x.x() - y.x(), 2 ) + pow( x.y() - y.y(), 2) );
}

myColor3 TDHelper::getBoarderColor()
{
	myColor3 result;
	result.a = rand() % 255;
	result.b = rand() % 255;
	result.c = rand() % 255;
	return result;
}

QColor TDHelper::getBoarderColor( int alpha )
{
	return QColor( rand() % 255, rand() % 255, rand() % 255, alpha );
}

QColor TDHelper::getColorByType(int type)
{
	switch( type )
	{
	case 1:
		return QColor( 100, 0, 0, 100 );
	case 2:
		return QColor( 0, 100, 0, 100 );
	case 3:
		return QColor( 0, 0, 100, 100 );
	default:
		return QColor( 0, 0, 0, 20 );
	}
}

float* TDHelper::getNormal(float *norm, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3)
{
	//float *norm = new float[3];
	float v1x=x2-x1;
	float v1y=y2-y1;
	float v1z=z2-z1;

	float v2x=x3-x1;
	float v2y=y3-y1;
	float v2z=z3-z1;

	norm[0]=v1y*v2z-v1z*v2y;
	norm[1]=v1z*v2x-v2z*v1x;
	norm[2]=v1x*v2y-v1y*v2x;

	float length=sqrt(norm[0]*norm[0]+norm[1]*norm[1]+norm[2]*norm[2]);

	norm[0]=norm[0]/length;
	norm[1]=norm[1]/length;
	norm[2]=norm[2]/length;
	return norm;
}

double TDHelper::fromDegreeToRadio( int degree )
{
	return ( degree %360 ) / 180.0 * PI;
}

int TDHelper::Max( int a, int b )
{
	if( a > b )
	{
		return a;
	}
	else
	{
		return b;
	}
}

float TDHelper::Max( float a, float b )
{
	if( a > b )
	{
		return a;
	}
	else
	{
		return b;
	}
}

int TDHelper::Min( int a, int b )
{
	if( a > b )
	{
		return b;
	}
	else
	{
		return a;
	}
}

float TDHelper::Min(float a, float b)
{
	if( a > b )
	{
		return b;
	}
	else
	{
		return a;
	}
}

std::string TDHelper::getDataPrefix()
{
	return "data/";
}

bool TDHelper::notInBouding( QPointF pos, int width, int height )
{
	if( pos.x() > width )
	{
		return true;
	}
	if( pos.y() > height )
	{
		return true;
	}
	return false;
}

bool TDHelper::inBounding( QPointF pos, int width, int height )
{
	if( pos.x() < width )
	{
		if( pos.y() < height )
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

QDialog* TDHelper::createDialog(const QString &windowTitle)
{
	QDialog *dialog = new QDialog(0, Qt::CustomizeWindowHint | Qt::WindowTitleHint);
	dialog->setWindowOpacity(1.0);
	dialog->setWindowTitle(windowTitle);
	return dialog;
}

int TDHelper::LengthOfQPointF( QPointF pos )
{
	return sqrt( pow( pos.x(), 2 ) + pow( pos.y(), 2 ) );
}

float TDHelper::myAbs( float input )
{
	if( input > 0.0 )
	{
		return input;
	}
	else
	{
		return -input;
	}
}
//void TDHelper::initOpenManager( OpenGLScene* scene, LabObject* lab, LabCut* cut, GroupManager* manager, AllBats* bats )
//{
//	TDHelper::_openManager = new OpenManager( scene, lab, cut, manager, bats );
//	return;
//}
//
//OpenManager* TDHelper::getOpenManager()
//{
//	return TDHelper::_openManager;
//}