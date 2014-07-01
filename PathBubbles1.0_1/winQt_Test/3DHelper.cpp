#include "OpenManager.h"
#include "3DHelper.h"
#include "openglscene.h"
//#include "qtbat.h"

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

myColor3 TDHelper::getInnerColorByType(int type)
{
	myColor3 result;   
	float R,G,B;
	switch( type )
	{
		case PATHBUBBLE1:
			result.a = 230;//203;
			result.b = 250;//244;
			result.c = 251;//245;
			break; 
		case SUBPATHBUBBLE1:
			result.a = 217;//240;
			result.b = 242;//250;
			result.c = 255;//255;
			break;
		case EXPRESSIONBUBBLE:
			result.a = 250;//244;
			result.b = 253;//250;
			result.c = 227;//199;
			break;
        case REACTIONBUBBLE3:
			result.a = 241;
			result.b = 201;
			result.c = 222;
			break;
		case TREERING:
			result.a = 201;
			result.b = 241;
			result.c = 222;
			break;
		case SETTINGS:
			result.a = 255;
			result.b = 249;
			result.c = 251;
			break;
		case SEARCHSETTINGS:
			result.a = 255;
			result.b = 249;
			result.c = 251;
			break;
		case SEARCHFIND:
			result.a = 250;
			result.b = 250;
			result.c = 250;
			break;	
		case CALLOUT:
			result.a = 215; //160;
			result.b = 200; //255;
			result.c = 145; //209;
			break;
		case TEXTBUBBLE2:
			result.a = 250;
			result.b = 242;
			result.c = 226;
			break;
		case WEB:
			result.a = 225;//200;
			result.b = 247;//240;
			result.c = 230;//207;
			break;
		default:
			result.a = rand() % 255;
			result.b = rand() % 255;
			result.c = rand() % 255;
			break;
	}
	return result;
}

QString TDHelper::getHtmlColorByUserName(int &id)
{
	QString c;	
	id=id%6;
	switch( id )
	{
		case 0:
			c="darkgreen";
			break; 
		case 1:
			c="purple";
			break;
		case 2:
			c="darkblue"; 
			break;
        case 3:
			c="maroon"; 
			break;
		case 4:
			c="crimson";
			break;
		case 5:
			c="indigo"; 
			break;				
		default:
			c="crimson"; 
			break;
	}
	return c;
}

myColor3 TDHelper::getColorByUserName(int &id)
{
	myColor3 result;   
	float R,G,B;	
	id=id%6;
	switch( id )
	{
		case 0:
			result.a = 56;//203;
			result.b = 118;//244;
			result.c = 29;//245;
			break; 
		case 1:
			result.a = 19;//240;
			result.b = 79;//250;
			result.c = 92;//255;
			break;
		case 2:
			result.a = 11;//244;
			result.b = 83;//250;
			result.c = 148;//199;
			break;
        case 3:
			result.a = 53;
			result.b = 28;
			result.c = 117;
			break;
		case 4:
			result.a = 116;
			result.b = 27;
			result.c = 71;
			break;
		case 5:
			result.a = 204;
			result.b = 0;
			result.c = 0;
			break;		
		default:
			result.a = rand() % 255;
			result.b = rand() % 255;
			result.c = rand() % 255;
			break;
	}
	return result;
}

myColor3 TDHelper::getInnerColorByType(int type, int id)
{
	if(type==TREERING)
	{
		id=id%7;
	}
	else
		id=id%5;
	myColor3 result;   
	float R,G,B;
	switch( type )
	{
		case PATHBUBBLE1:			
			HLS_TO_RGB(180+(id-2)*5, 0.98, 0.24, R, G, B);
			result.a = R;
			result.b = G;
			result.c = B;
			break; 
		case SUBPATHBUBBLE1:
			HLS_TO_RGB(240+(id-2)*5, 0.88, 0.24, R, G, B);
			result.a = R;
			result.b = G;
			result.c = B;
			break;
		case EXPRESSIONBUBBLE:
			HLS_TO_RGB(60+(id-2)*5, 0.88, 0.24, R, G, B);
			result.a = R;
			result.b = G;
			result.c = B;
			break;		
		case REACTIONBUBBLE3:
			result.a = 241;
			result.b = 201;
			result.c = 222;
			break;
		case TREERING:			
			//result.a = 115;
			//result.b = 215;
			//result.c = 165;
			HLS_TO_RGB(100+(id-3)*10, 0.97, 0.24, R, G, B);
			result.a = R;
			result.b = G;
			result.c = B;
			break;
		case SETTINGS:
			result.a = 255;
			result.b = 249;
			result.c = 251;
			break;
		case SEARCHSETTINGS:
			result.a = 255;
			result.b = 249;
			result.c = 251;
			break;
		case SEARCHFIND:
			result.a = 250;
			result.b = 250;
			result.c = 250;
			break;	
		case CALLOUT:
			result.a = 215; //160;
			result.b = 200; //255;
			result.c = 145; //209;
			break;
		case TEXTBUBBLE2:
			result.a = 250;
			result.b = 242;
			result.c = 226;
			break;
		case WEB:
			result.a = 225;//200;
			result.b = 247;//240;
			result.c = 230;//207;
			break;
		default:
			result.a = rand() % 255;
			result.b = rand() % 255;
			result.c = rand() % 255;
			break;
	}
	return result;
}


float TDHelper::max1(float r, float g, float b)
{
	if(r>=g)
		if(r>=b) return r;
		else return b;
	else if(r<g)
			if(b>g) return b;
			else return g;
}

float TDHelper::min1(float r, float g, float b)
{
	if(r<=g)
		if(r<=b) return r;
		else return b;
		else if(r>g)
			if(b<=g) return b;
			else return g;
}

void TDHelper::HLS_TO_RGB(float h,float l,float ss,float &r,float &g,float &b)
{
	float m1,m2;
	if(l <= 0.5) m2 = l * (1 + ss);
	else m2 = l + ss - l * ss;
	m1 = 2 * l - m2;
	if(ss == 0) 	  
	    r = g = b = 255 * l;	   
	else
	{
	    r = 255 * value(m1,m2,h + 120);
	    g = 255 * value(m1,m2,h);
		b = 255 * value(m1,m2,h - 120 );
	}
}

float TDHelper::value(float n1,float n2,float hue)
{
	float v;
	if(hue > 360) hue = hue - 360;
	if(hue < 0)   hue = hue + 360;
    if(hue < 60)
		v = n1 + ( n2 - n1) * hue / 60;
	else if(hue < 180) 
		v = n2;
	else if(hue < 240)
		v = n1 + (n2 - n1) * (240 - hue) / 60;
	else 
		v = n1;
	return v;
}

bool TDHelper::overlaid(QRectF left, QRectF right)
{
	int disX = abs( left.center().x() - right.center().x() );
	int disY = abs( left.center().y() - right.center().y() );
	
	int thresh = ( left.width() + right.width() ) / 2;
	int thresh2 = ( left.height() + right.height() ) / 2;
	if( disX < thresh && disY < thresh2 )
	{
		return true;
	}
	return false;	
}


void TDHelper::RGB_TO_HLS(float r,float g,float b,float &h,float &l,float &s)
{//r,g,b:0-255
	/*known: r,g,b
	find: h in[0,360],l and s in[0,1],
	except if s=0, then h=undefined;
    }*/
	float m,n;
	float delta;	
	b/=255; g/=255; r/=255;  
    
	m=max1(r,g,b);
	n=min1(r,g,b);
	l=(m+n)/2;
	if(m==n)
	{
		s=0;h=-1;
	}
	else
	{
		if(l<=0.5)
			s=(m-n)/(m+n);
		else
			s=(m-n)/(2-m-n);
		delta=m-n;
		if(r==m)
			h=(g-b)/delta;
		else if(g==m)
			h=2+(b-r)/delta;
		else if(b==m)
			h=4+(r-g)/delta;
		h=h*60;
		if(h<0.0)
			h=h+360;
	}

}


void TDHelper::GetMultHue(float value, int type, float &r, float &g, float &b )//http://colorbrewer2.org/
{  //sequental color 1, 2, 3, 4, 5: multihue
	//6,7,8,9,10 single hue color
	//value=1-value;
	int H1[9][3]={{255,247,243},{253,224,221},{252,197,192},
	              {250,159,181},{247,104,161},{221,52,151},
	              {174,1,126},{122,1,119},{73,0,106}};  //pink-purple
	int H2[9][3]={{255,255,229},{247,252,185},{217,240,163},
	              {173,221,142},{120,198,121},{65,171,93},
	              {35,132,67},{0,104,55},{0,69,41}};  //yellow-green
	int H3[9][3]={{255,255,217},{237,248,217},{199,233,150},
	              {127,205,187},{65,182,196},{29,145,192},
	              {34,94,168},{37,52,148},{8,29,88}};  //yellow-blue
	int H4[9][3]={{255,255,229},{255,247,188},{254,227,145},
	              {254,196,79},{254,153,41},{236,112,20},
	              {204,76,2},{153,52,4},{102,37,6}};  //yellow-brown
	int H5[9][3]={{255,255,204},{255,237,160},{254,217,118},
	              {254,178,76},{253,141,60},{252,78,42},
	              {227,26,28},{189,0,38},{128,0,38}};  //yellow-red
	int H6[9][3]={{255, 245, 240},{254, 224, 210},{252, 187, 161},
	              {252, 146, 114},{251, 106, 74},{239, 59, 44},
	              {203, 24, 29},{65, 15, 21},{103, 0, 13}};   //white-red
	int H7[9][3]={{252,251,253},{239,237,245},{218,218,235},
	            {188,189,220},{158,154,200},{128,125,186},
	            {106,81,163},{84,39,143},{63,0,125}};  //white-purple
	int H8[9][3]={{255,245,235},{254,230,206},{253,208,162},
	        {253,174,107},{253,141,60},{241,105,19},
	        {217,72,1},{166,54,3},{127,39,4}};  //white-orange

	int H9[9][3]={{247,252,245},{229,245,224},{199,233,192},
	        {161,217,155},{116,196,118},{65,171,93},
	        {35,139,69},{0,109,44},{0,68,27}};  //white-green

	int H10[9][3]={{247,251,255},{222,235,247},{198,219,239},
	        {158,202,225},{107,174,214},{66,146,198},
	        {33,113,181},{8,81,156},{8,48,107}};  //white-blue
    float R[9],L[9],S[9],R1,R2,L1,L2,S1,S2,R0,L0,S0;

	int H[9][3];
	switch(type)
	{
	    case 1: for(int i=0; i<9;i++) for(int j=0; j<3;j++) H[i][j]=H1[i][j]; break;
		case 2: for(int i=0; i<9;i++) for(int j=0; j<3;j++) H[i][j]=H2[i][j]; break;
		case 3: for(int i=0; i<9;i++) for(int j=0; j<3;j++) H[i][j]=H3[i][j]; break;
		case 4: for(int i=0; i<9;i++) for(int j=0; j<3;j++) H[i][j]=H4[i][j]; break;
        case 5: for(int i=0; i<9;i++) for(int j=0; j<3;j++) H[i][j]=H5[i][j]; break;
		case 6: for(int i=0; i<9;i++) for(int j=0; j<3;j++) H[i][j]=H6[i][j]; break;
		case 7: for(int i=0; i<9;i++) for(int j=0; j<3;j++) H[i][j]=H7[i][j]; break;
		case 8: for(int i=0; i<9;i++) for(int j=0; j<3;j++) H[i][j]=H8[i][j]; break;
		case 9: for(int i=0; i<9;i++) for(int j=0; j<3;j++) H[i][j]=H9[i][j]; break;
		case 10: for(int i=0; i<9;i++) for(int j=0; j<3;j++) H[i][j]=H10[i][j]; break;		
		default:for(int i=0; i<9;i++) for(int j=0; j<3;j++) H[i][j]=H6[i][j]; break;
	}
    switch(type)
	{
	    case 1: RGB_TO_HLS(H1[0][0],H1[0][1],H1[0][2],R[0],L[0],S[0]); 
		        RGB_TO_HLS(H1[1][0],H1[1][1],H1[1][2],R[1],L[1],S[1]); 		
				RGB_TO_HLS(H1[2][0],H1[2][1],H1[2][2],R[2],L[2],S[2]);
	            RGB_TO_HLS(H1[3][0],H1[3][1],H1[3][2],R[3],L[3],S[3]);
	            RGB_TO_HLS(H1[4][0],H1[4][1],H1[4][2],R[4],L[4],S[4]);
	            RGB_TO_HLS(H1[5][0],H1[5][1],H1[5][2],R[5],L[5],S[5]);
	            RGB_TO_HLS(H1[6][0],H1[6][1],H1[6][2],R[6],L[6],S[6]);
	            RGB_TO_HLS(H1[7][0],H1[7][1],H1[7][2],R[7],L[7],S[7]);
	            RGB_TO_HLS(H1[8][0],H1[8][1],H1[8][2],R[8],L[8],S[8]);
                break;
		case 2: RGB_TO_HLS(H2[0][0],H2[0][1],H2[0][2],R[0],L[0],S[0]); 
		        RGB_TO_HLS(H2[1][0],H2[1][1],H2[1][2],R[1],L[1],S[1]); 		
				RGB_TO_HLS(H2[2][0],H2[2][1],H2[2][2],R[2],L[2],S[2]);
	            RGB_TO_HLS(H2[3][0],H2[3][1],H2[3][2],R[3],L[3],S[3]);
	            RGB_TO_HLS(H2[4][0],H2[4][1],H2[4][2],R[4],L[4],S[4]);
	            RGB_TO_HLS(H2[5][0],H2[5][1],H2[5][2],R[5],L[5],S[5]);
	            RGB_TO_HLS(H2[6][0],H2[6][1],H2[6][2],R[6],L[6],S[6]);
	            RGB_TO_HLS(H2[7][0],H2[7][1],H2[7][2],R[7],L[7],S[7]);
	            RGB_TO_HLS(H2[8][0],H2[8][1],H2[8][2],R[8],L[8],S[8]);
				break;
		case 3: RGB_TO_HLS(H3[0][0],H3[0][1],H3[0][2],R[0],L[0],S[0]); 
		        RGB_TO_HLS(H3[1][0],H3[1][1],H3[1][2],R[1],L[1],S[1]); 		
				RGB_TO_HLS(H3[2][0],H3[2][1],H3[2][2],R[2],L[2],S[2]);
	            RGB_TO_HLS(H3[3][0],H3[3][1],H3[3][2],R[3],L[3],S[3]);
	            RGB_TO_HLS(H3[4][0],H3[4][1],H3[4][2],R[4],L[4],S[4]);
	            RGB_TO_HLS(H3[5][0],H3[5][1],H3[5][2],R[5],L[5],S[5]);
	            RGB_TO_HLS(H3[6][0],H3[6][1],H3[6][2],R[6],L[6],S[6]);
	            RGB_TO_HLS(H3[7][0],H3[7][1],H3[7][2],R[7],L[7],S[7]);
	            RGB_TO_HLS(H3[8][0],H3[8][1],H3[8][2],R[8],L[8],S[8]);
				break;
		case 4: RGB_TO_HLS(H4[0][0],H4[0][1],H4[0][2],R[0],L[0],S[0]); 
		        RGB_TO_HLS(H4[1][0],H4[1][1],H4[1][2],R[1],L[1],S[1]); 		
				RGB_TO_HLS(H4[2][0],H4[2][1],H4[2][2],R[2],L[2],S[2]);
	            RGB_TO_HLS(H4[3][0],H4[3][1],H4[3][2],R[3],L[3],S[3]);
	            RGB_TO_HLS(H4[4][0],H4[4][1],H4[4][2],R[4],L[4],S[4]);
	            RGB_TO_HLS(H4[5][0],H4[5][1],H4[5][2],R[5],L[5],S[5]);
	            RGB_TO_HLS(H4[6][0],H4[6][1],H4[6][2],R[6],L[6],S[6]);
	            RGB_TO_HLS(H4[7][0],H4[7][1],H4[7][2],R[7],L[7],S[7]);
	            RGB_TO_HLS(H4[8][0],H4[8][1],H4[8][2],R[8],L[8],S[8]);
				break;
		case 5:       
			    RGB_TO_HLS(H5[0][0],H5[0][1],H5[0][2],R[0],L[0],S[0]); 
		        RGB_TO_HLS(H5[1][0],H5[1][1],H5[1][2],R[1],L[1],S[1]); 		
				RGB_TO_HLS(H5[2][0],H5[2][1],H5[2][2],R[2],L[2],S[2]);
	            RGB_TO_HLS(H5[3][0],H5[3][1],H5[3][2],R[3],L[3],S[3]);
	            RGB_TO_HLS(H5[4][0],H5[4][1],H5[4][2],R[4],L[4],S[4]);
	            RGB_TO_HLS(H5[5][0],H5[5][1],H5[5][2],R[5],L[5],S[5]);
	            RGB_TO_HLS(H5[6][0],H5[6][1],H5[6][2],R[6],L[6],S[6]);
	            RGB_TO_HLS(H5[7][0],H5[7][1],H5[7][2],R[7],L[7],S[7]);
	            RGB_TO_HLS(H5[8][0],H5[8][1],H5[8][2],R[8],L[8],S[8]);
				break;
		case 7:       
			    RGB_TO_HLS(H7[0][0],H7[0][1],H7[0][2],R[0],L[0],S[0]); 
		        RGB_TO_HLS(H7[1][0],H7[1][1],H7[1][2],R[1],L[1],S[1]); 		
				RGB_TO_HLS(H7[2][0],H7[2][1],H7[2][2],R[2],L[2],S[2]);
	            RGB_TO_HLS(H7[3][0],H7[3][1],H7[3][2],R[3],L[3],S[3]);
	            RGB_TO_HLS(H7[4][0],H7[4][1],H7[4][2],R[4],L[4],S[4]);
	            RGB_TO_HLS(H7[5][0],H7[5][1],H7[5][2],R[5],L[5],S[5]);
	            RGB_TO_HLS(H7[6][0],H7[6][1],H7[6][2],R[6],L[6],S[6]);
	            RGB_TO_HLS(H7[7][0],H7[7][1],H7[7][2],R[7],L[7],S[7]);
	            RGB_TO_HLS(H7[8][0],H7[8][1],H7[8][2],R[8],L[8],S[8]);
				break;
		case 8:       
			    RGB_TO_HLS(H8[0][0],H8[0][1],H8[0][2],R[0],L[0],S[0]); 
		        RGB_TO_HLS(H8[1][0],H8[1][1],H8[1][2],R[1],L[1],S[1]); 		
				RGB_TO_HLS(H8[2][0],H8[2][1],H8[2][2],R[2],L[2],S[2]);
	            RGB_TO_HLS(H8[3][0],H8[3][1],H8[3][2],R[3],L[3],S[3]);
	            RGB_TO_HLS(H8[4][0],H8[4][1],H8[4][2],R[4],L[4],S[4]);
	            RGB_TO_HLS(H8[5][0],H8[5][1],H8[5][2],R[5],L[5],S[5]);
	            RGB_TO_HLS(H8[6][0],H8[6][1],H8[6][2],R[6],L[6],S[6]);
	            RGB_TO_HLS(H8[7][0],H8[7][1],H8[7][2],R[7],L[7],S[7]);
	            RGB_TO_HLS(H8[8][0],H8[8][1],H8[8][2],R[8],L[8],S[8]);
				break;
		case 9:       
			    RGB_TO_HLS(H9[0][0],H9[0][1],H9[0][2],R[0],L[0],S[0]); 
		        RGB_TO_HLS(H9[1][0],H9[1][1],H9[1][2],R[1],L[1],S[1]); 		
				RGB_TO_HLS(H9[2][0],H9[2][1],H9[2][2],R[2],L[2],S[2]);
	            RGB_TO_HLS(H9[3][0],H9[3][1],H9[3][2],R[3],L[3],S[3]);
	            RGB_TO_HLS(H9[4][0],H9[4][1],H9[4][2],R[4],L[4],S[4]);
	            RGB_TO_HLS(H9[5][0],H9[5][1],H9[5][2],R[5],L[5],S[5]);
	            RGB_TO_HLS(H9[6][0],H9[6][1],H9[6][2],R[6],L[6],S[6]);
	            RGB_TO_HLS(H9[7][0],H9[7][1],H9[7][2],R[7],L[7],S[7]);
	            RGB_TO_HLS(H9[8][0],H9[8][1],H9[8][2],R[8],L[8],S[8]);
				break;
		case 10:       
			    RGB_TO_HLS(H10[0][0],H10[0][1],H10[0][2],R[0],L[0],S[0]); 
		        RGB_TO_HLS(H10[1][0],H10[1][1],H10[1][2],R[1],L[1],S[1]); 		
				RGB_TO_HLS(H10[2][0],H10[2][1],H10[2][2],R[2],L[2],S[2]);
	            RGB_TO_HLS(H10[3][0],H10[3][1],H10[3][2],R[3],L[3],S[3]);
	            RGB_TO_HLS(H10[4][0],H10[4][1],H10[4][2],R[4],L[4],S[4]);
	            RGB_TO_HLS(H10[5][0],H10[5][1],H10[5][2],R[5],L[5],S[5]);
	            RGB_TO_HLS(H10[6][0],H10[6][1],H10[6][2],R[6],L[6],S[6]);
	            RGB_TO_HLS(H10[7][0],H10[7][1],H10[7][2],R[7],L[7],S[7]);
	            RGB_TO_HLS(H10[8][0],H10[8][1],H10[8][2],R[8],L[8],S[8]);
				break;
		case 6:
        default:
			    RGB_TO_HLS(H6[0][0],H6[0][1],H6[0][2],R[0],L[0],S[0]); 
		        RGB_TO_HLS(H6[1][0],H6[1][1],H6[1][2],R[1],L[1],S[1]); 		
				RGB_TO_HLS(H6[2][0],H6[2][1],H6[2][2],R[2],L[2],S[2]);
	            RGB_TO_HLS(H6[3][0],H6[3][1],H6[3][2],R[3],L[3],S[3]);
	            RGB_TO_HLS(H6[4][0],H6[4][1],H6[4][2],R[4],L[4],S[4]);
	            RGB_TO_HLS(H6[5][0],H6[5][1],H6[5][2],R[5],L[5],S[5]);
	            RGB_TO_HLS(H6[6][0],H6[6][1],H6[6][2],R[6],L[6],S[6]);
	            RGB_TO_HLS(H6[7][0],H6[7][1],H6[7][2],R[7],L[7],S[7]);
	            RGB_TO_HLS(H6[8][0],H6[8][1],H6[8][2],R[8],L[8],S[8]);
				break;
	}
	//chazhi
    int slot=int(value*8);
	float temp=value*8.0-slot;
	switch(slot)
	{ 
	    case 0: R1=R[0],L1=L[0],S1=S[0]; 
			    R2=R[1],L2=L[1],S2=S[1]; 				
			    break;
		case 1: R1=R[1],L1=L[1],S1=S[1]; 
			    R2=R[2],L2=L[2],S2=S[2]; 			    
				break;
		case 2: R1=R[2],L1=L[2],S1=S[2]; 
			    R2=R[3],L2=L[3],S2=S[3]; 			    
				break;
		case 3: R1=R[3],L1=L[3],S1=S[3]; 
			    R2=R[4],L2=L[4],S2=S[4]; 
				break;
		case 4: R1=R[4],L1=L[4],S1=S[4]; 
			    R2=R[5],L2=L[5],S2=S[5]; 
				break;
		case 5: R1=R[5],L1=L[5],S1=S[5]; 
			    R2=R[6],L2=L[6],S2=S[6]; 
				break;
		case 6: R1=R[6],L1=L[6],S1=S[6]; 
			    R2=R[7],L2=L[7],S2=S[7]; 
				break;
		case 7:
	    default: R1=R[7],L1=L[7],S1=S[7]; 
			    R2=R[8],L2=L[8],S2=S[8]; 
				break;
	}
	R0=R1*(1-temp)+temp*R2, L0=L1*(1-temp)+temp*L2, S0=S1*(1-temp)+temp*S2;	
	HLS_TO_RGB(R0,L0,S0,r,g,b);
}


myColor3 TDHelper::getBoarderColorByType(int type)
{
	myColor3 result;  
	switch( type )
	{
	case PATHBUBBLE1:
		result.a = 0;
	    result.b = 255;
	    result.c = 255;
		break; 
	case SUBPATHBUBBLE1:
		result.a = 130;
	    result.b = 179;
	    result.c = 253;
		break;
	case EXPRESSIONBUBBLE:
		result.a = 218;
	    result.b = 230;
	    result.c = 70;
		break;
	case TREERING:
	    result.a = 115;
	    result.b = 215;
	    result.c = 165;
		break;
	case REACTIONBUBBLE3:
	    result.a = 215;
	    result.b = 115;
	    result.c = 165;
		break;
	case SEARCHSETTINGS:
		result.a = 253;
	    result.b = 172;
	    result.c = 199;
		break;
	case SETTINGS:
		result.a = 253;
	    result.b = 172;
	    result.c = 199;
		break;		
	case SEARCHFIND:
	    result.a = 215;
	    result.b = 215;
	    result.c = 215;
		break;
	case NOTE:
	    result.a = 109;
	    result.b = 204;
	    result.c = 223;
		break;
	case CALLOUT:
	    result.a = 0;
	    result.b = 128;
	    result.c = 255;
		break;
	case WEB:
	    result.a = 125;
	    result.b = 225;
	    result.c = 143;
		break;
	case TEXTBUBBLE2:
	    result.a = 240;
	    result.b = 205;
	    result.c = 90;
		break;
	default:
		result.a = rand() % 255;
	    result.b = rand() % 255;
	    result.c = rand() % 255;
		break;
	}
	return result;
}


myColor3 TDHelper::getBoarderColorByType(int type, int id)
{
	myColor3 result;  
	float R,G,B;
	if(type==TREERING)
	{
		id=id%7;
	}
	else
		id=id%5;
	switch( type )
	{
	case PATHBUBBLE1:
		HLS_TO_RGB(180+(id-2)*4, 0.5, 1.0, R, G, B);
		result.a = R;
	    result.b = G;
	    result.c = B;
		break; 
	case SUBPATHBUBBLE1:
		HLS_TO_RGB(240+(id-2)*10, 0.7, 0.6, R, G, B);
		result.a = R;
	    result.b = G;
	    result.c = B;
		break;
	case EXPRESSIONBUBBLE:
		HLS_TO_RGB(60+(id-2)*5, 0.6, 0.8, R, G, B);
		result.a = R;
	    result.b = G;
	    result.c = B;
		break;
	case TREERING:
	    //result.a = 115;
	    //result.b = 215;
	    //result.c = 165;
		HLS_TO_RGB(100+(id-3)*10, 0.7, 0.9, R, G, B);
		result.a = R;
	    result.b = G;
	    result.c = B;
		break;
	case REACTIONBUBBLE3:
	    result.a = 215;
	    result.b = 115;
	    result.c = 165;
		break;
	case SEARCHSETTINGS:
		result.a = 253;
	    result.b = 172;
	    result.c = 199;
		break;
	case SETTINGS:
		result.a = 253;
	    result.b = 172;
	    result.c = 199;
		break;		
	case SEARCHFIND:
	    result.a = 215;
	    result.b = 215;
	    result.c = 215;
		break;
	case NOTE:
	    result.a = 109;
	    result.b = 204;
	    result.c = 223;
		break;
	case CALLOUT:
	    result.a = 0;
	    result.b = 128;
	    result.c = 255;
		break;
	case WEB:
	    result.a = 125;
	    result.b = 225;
	    result.c = 143;
		break;
	case TEXTBUBBLE2:
	    result.a = 240;
	    result.b = 205;
	    result.c = 90;
		break;
	default:
		result.a = rand() % 255;
	    result.b = rand() % 255;
	    result.c = rand() % 255;
		break;
	}
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

int TDHelper::getNumberNDigital(float test, int Nb)
{
	if(test==0)
		return 0;
	int count=1;	
	if(test<0)
		test=-test;
	test=test-int(test);
	if(test==0)
		return 0;
	while(test<1)
	{
		test=test*10;
		count++;	
	}	
	QString str= QString::number(test, 'g', count);		
	if( str.size()< Nb )
	{
	   return 0;	
	}
	str= str.remove('.');
	QString DN;
	if(Nb<str.size())
	    DN=str.at(Nb);	
	else
		DN="0";	//keqin
	return DN.toInt();
}

int TDHelper::getfirstPartNumber(float test, int Nb)
{
	vector<int> ds;
	for(int i=0; i<Nb; i++)
	{
		ds.push_back(getNumberNDigital(test, i));			
	}
	int newNumber=0;
	for(int i=0; i<Nb; i++)
	{
	    newNumber = newNumber + pow(float(10),(Nb-i-1))*ds[i];
	}	
	return newNumber;
}

vector<QString> TDHelper::getLegendParameter(float &min, float &max, int type, int preferedNum, int &num, int &dl, float &mitv)
//void getLegendParameter(float min, float max, int type, int preferedNum, int &num, int &dl)
{
	if(preferedNum<=0)
		preferedNum=1;
	bool ifRounded=true;
	float length = fabs(max - min);
	float interval;
	int count;
	int Max, Min;
	vector<QString> markers;

	if(type=='F')
	{
		/*float newMin, newMax;
		float span = length/preferedNum;	
		for();
		*/	
		//if do not rounded
		if(ifRounded)
		{
			//get the a 2 digital number
		    int d1, d2; 
			float nd1,nd2;
			float scale;
			float Itv;
			nd2 = getfirstPartNumber(max,2);
			
			if(nd2==0)
				nd2=max;

			if(max==0)
			  scale=1;
			else
			{
				scale =float(nd2)/max;
				if(nd2==0)
					scale=1;
			}
			
			nd1 = min*scale;				
			if(nd2-nd1==1)
			{
			   num=preferedNum; 
			   Itv=1.0/preferedNum;
			} 
			else 
			    getLegendParameter(nd1, nd2, 'I', preferedNum, num, dl, Itv);
			
			min = nd1/scale;
			max = nd2!=0?nd2/scale:max;				
			mitv = Itv/scale;
			dl=2;

			//float length1= max - min;
			
			markers.resize(num+1);	
			//float length= num*mitv;			
			//float scale = length1==0?0:length/length1;			
			
			int mark=nd1;
			markers[0]= QString::number(min,'g',mark<=10? dl-1: dl); //QString::number(mark));
			for(int i=0; i<num; ++i)
			{
				mark=nd1+(i+1)*Itv;				
				markers[i+1]=QString::number(min+(i+1)*mitv,'g',mark<=10? dl-1: dl);
			}
			min = markers[0].toFloat();
			max = markers[num].toFloat();
			//mitv = mitv
		}
		else
		{
		   length = fabs(max - min); 		
			float  test;
			do
			{
				interval = length/preferedNum;	
				count=1;
				test=interval-int(interval);
				while(test<1)
				{
					test=test*10;
					count++;	
				}	
				preferedNum = preferedNum-1;
			} 
			while(count>6 && preferedNum>2);		
			preferedNum = preferedNum + 1;
			dl = count;
			num = preferedNum;
			mitv=(max-min)/num;		
			//float length1= max - min;
			
			markers.resize(num+1);	
			//float length= num*mitv;			
			//float scale = length1==0?0:length/length1;			
			markers[0]= QString::number(min,'g',dl); //QString::number(mark));
			for(int i=0; i<num; ++i)
			{
				markers[i+1]=QString::number(min+(i+1)*mitv,'g',dl);
			}			
			min = markers[0].toFloat();
			max = markers[num].toFloat();
		}
	}
	else if(type=='I')
	{
		
		length = int(max) - int(min);
		if(length==0)
		{
		   num=1, dl=0;	
		   mitv=0;
		}
		else if(length==1)
		{
		   num=1, dl=0;	
		   mitv=int(0.5+(max-min)/num);
		}
		else 
		{
			//length = int (length + 0.5);
			if(length > preferedNum)
			{
				int itv=1;
				int test;
				do
				{
					itv++;
					test = length/ itv;					
				}
				while( test<2 || test*(itv+1) < length || (length / itv + 1 >10) );
				dl= 0;
				num = length / itv + 1;
				mitv=int(0.5+(max-min)/num);
				/*int mark=min+num*mitv;
				if(min+num*mitv<max)
					num=num+1;
				else if(min + (num-1)*mitv>=max)
					num = num-1;
				*/
			}		
			else
			{
			   num = length;
			   dl=0;	
			   mitv=int(0.5+(max-min)/num);
			}
			QString dNumS = QString::number(int(mitv));
			int dNum=dNumS.size();
			int nMitv=dNum==1?mitv:mitv/(pow(float(10),dNum-1));
			if(nMitv>5)
			{
				nMitv=10;			
			}
			else if(nMitv>2)
			{
				nMitv=5;			
			}
			mitv=nMitv*pow(float(10),dNum-1);
			if(length > preferedNum)
			{
				int mNum=min/mitv;				
				min = int(mNum*mitv);
				mNum=max/mitv;
				int nMax = int(mNum*mitv);
				if(nMax<max)
					max=nMax+mitv;
				else max = nMax;

				num = (max-min) / mitv;
				
				/*int mark=min+num*mitv;
				if(min+num*mitv<max)
					num=num+1;
				else if(min + (num-1)*mitv>=max)
					num = num-1;				
				*/
			}					
		}
		markers.resize(num+1);	
		//float length= num*mitv;			
		//float scale = length1==0?0:length/length1;			
		markers[0]= QString::number(int(min)); //QString::number(mark));
		for(int i=0; i<num; ++i)
		{
			markers[i+1]=QString::number(int(min+(i+1)*mitv));
		}	
		min = int(min);
		max = int(min+ num*mitv);
	}
	
    return markers;
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