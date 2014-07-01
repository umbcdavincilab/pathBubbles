#include "Camber.h"
#include <QPainter>

Camber::Camber(string name, int num, int scale, LabObject *lab, LabCut *cut ) : currentSelected( 0 ) 
{
	//		global norm vector
	norm = new float[3];
	NUMOFCAMBERSIZE[0]=0;
	NUMOFCAMBERSIZE[1]=0;
	NUMOFCAMBERSIZE[2]=0;
	//		flags
	_flag = new bool[num];
	for( int i = 0; i < num; i++ )
	{
		_flag[ i ] = false;
	}
	Num = num;
	Name = name;
	Scale = scale;
	_lab = lab;
	_cut = cut;
	init();
	//	here we need to load m1, m2 and m3
	load();
	loadSize();
}

Camber::Camber( Camber& copy )
{
	return;
}

Camber::~Camber()
{
	delete norm;
}

void Camber::init()
{
	//how many points containing by each frame
	area = new double[Num];
	//	initialize data structure
	for( int j = 0; j < 3; j++)
	{
		size[j] = new double[Num];
		data[j] = new double*[Num];
		numOfList[j] = new int[Num];
	}
	for(int i = 0; i < Num; i++)
	{
		for( int j = 0; j < 3; j++)
		{
			data[j][i] = new double[6 * 3];
			size[j][i] = -1;
		}
	}
}

QString Camber::checkPath( QString path )
{
	QString result;
	int sum[3];
	sum[0] = sum[1] = sum[2] = 0;
	for( int i = 0; i < 3; i++ )
	{
		int index = 0;
		while( true )
		{
			QString currentPath = path;
			if( i == 0 )
			{
				currentPath.append( "m1.f" );
			}
			else if( i == 1 )
			{
				currentPath.append( "m2.f" );
			}
			else if( i == 2 )
			{
				currentPath.append( "m3.f" );
			}
			QString num;
			num.setNum( index );
			currentPath.append( num + ".txt" );
			if( !QFile::exists( currentPath ) )
			{
				//result.append( "Cannot open: " + currentPath );
				break;
			}
			else
			{
				index ++;
			}
			//
		}
		sum[i] = index;
		//
		QString currentSize = path;
		currentSize.remove( currentSize.length() - 1, 1 );
		if( i == 0 )
		{
			currentSize.append( "_m1.camber" );
		}
		else if( i == 1 )
		{
			currentSize.append( "_m2.camber" );
		}
		else if( i == 2 )
		{
			currentSize.append( "_m3.camber" );
		}
		if( !QFile::exists( currentSize ) )
		{
			result.append( "Cannot open camber size file." );
			return result;
		}
	}
	//*************************************************************
	if( sum[0] == sum[1] && sum[2] == sum[1] )
	{
		if( sum[0] == 0 )
		{
			result.append( "Cannot open camber." );
		}
		else
		{
			result.append( "Success" );
		}
	}
	else
	{
		result.append( "Camber number dis-match!" );
	}
	return result;
}

void Camber::loadCamberPoints( int index )
{
	int actualNum = 0;
 	string currentPath = Name;
	if( index == 0 )
	{
		currentPath.append( "m1.f" );
	}
	else if( index == 1 )
	{
		currentPath.append( "m2.f" );
	}
	else if( index == 2 )
	{
		currentPath.append( "m3.f" );
	}
	for(int i = 0; i < Num; i++)
	{
		stringstream sstrm;
		sstrm<<i<<".txt";

		string fileToBeOpen = currentPath;
		fileToBeOpen.append(sstrm.str());

		ifstream input;
		input.open( fileToBeOpen.c_str() );
		if( input.fail() == true )
		{
			continue;
		}
		else
		{
			_flag[ i ] = true;
			actualNum += 1;
		}

		string temp;
		int temoInt;

		while(!input.eof())
		{
			input>>temp;
			if(temp == "#TotalNumOfList:")
			{
				int numOfPts;
				input>>numOfPts;
				numOfList[index][i] = numOfPts;
				for(int j = 0; j < numOfList[index][i]; j++)
				{
					input>>data[index][i][j*3  ];
					input>>data[index][i][j*3+1];
					input>>data[index][i][j*3+2];
				}
			}
		}
		input.close();
	}
	this->Num = actualNum;
	//for(int i = 0; i < Num; i++)
	//{
	//	area[i] = getSize(i);
	//}
}
void Camber::load()
{
	//loadSize();
	for( int i = 0; i < 3; i++ )
	{
		loadCamberPoints( i );
	}
}

int Camber::getNumOfPointAtFrame( int marker, int index )
{
	if(index < Num)
	{
		return numOfList[marker][index];
	}
	else
	{
		return 0;
	}
}

void Camber::drawCamber( int cur_frame_num )
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	myColor3 color;
	for( int j = 0; j < 3; j++ )
	{
		if( !_ENABLED[j] )
		{
			continue;
		}
		if( cur_frame_num > Num || _flag[ cur_frame_num ] == false )
		{
			return;
		}

		double percentage = this->getPercentage( j, cur_frame_num );
		Lab3D pos = _cut->get_Position( percentage );
		_lab->get_Color( pos, color);
		//		color
		glColor3f( color.a/255.0, color.b/255.0, color.c/255.0 );
		//		matrial
		//GLfloat mat_diffuse[]={ color.a/255.0, color.b/255.0, color.c/255.0, 0.8 };
		//GLfloat mat_ambient[]={ color.a/255.0, color.b/255.0, color.c/255.0, 0.8 };
		//GLfloat mat_specular[]={ color.a/255.0, color.b/255.0, color.c/255.0, 0.8 };
		//GLfloat mat_shininess[]={30.0};
		//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
		//glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
		//glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
		//glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

		norm = TDHelper::getNormal( norm, (float)data[currentSelected][cur_frame_num][0]*Scale,
			(float)(data[currentSelected][cur_frame_num][2]*Scale),
			-(float)(data[currentSelected][cur_frame_num][1]*Scale),
			(float)data[currentSelected][cur_frame_num][3]*Scale,
			(float)(data[currentSelected][cur_frame_num][5]*Scale),
			-(float)(data[currentSelected][cur_frame_num][4]*Scale),
			(float)data[currentSelected][cur_frame_num][6]*Scale,
			(float)(data[currentSelected][cur_frame_num][8]*Scale),
			-(float)(data[currentSelected][cur_frame_num][7]*Scale));
		glNormal3f( norm[0], norm[1], norm[2] );
		glBegin(GL_POLYGON);
		for(int i=0; i < numOfList[j][cur_frame_num]; i++)
		{
			glVertex3f((float)data[j][cur_frame_num][i*3]*Scale,(float)(data[j][cur_frame_num][i*3+2]*Scale),-(float)(data[j][cur_frame_num][i*3+1]*Scale));
		}

		glEnd();

		glNormal3f( -norm[0], -norm[1], -norm[2] );
		glBegin(GL_POLYGON);
		for(int i = numOfList[j][cur_frame_num] - 1; i >= 0; i--)
		{		
			glVertex3f((float)data[j][cur_frame_num][i*3]*Scale,(float)(data[j][cur_frame_num][i*3+2]*Scale),-(float)(data[j][cur_frame_num][i*3+1]*Scale));
		}
		glEnd();
	}
}

void Camber::getPointsAtFrame(int index, int marker, double *points)
{
	if( marker > 2 )
	{
		marker = 2;
	}
	for(int i = 0; i <getNumOfPointAtFrame(0, index); i++)
	{
		points[ i*3 + 0] = data[marker][index][i*3 + 0] * Scale;
		points[ i*3 + 1] = data[marker][index][i*3 + 2] * Scale;
		points[ i*3 + 2] = data[marker][index][i*3 + 1] * Scale;
	}
}

double Camber::getPercentage( int which, int index)
{
	return (size[which][index] - Min[which])/(Max[which] - Min[which]);
}

void Camber::loadSizeFor( int which )
{
	string currentPath = Name;
	currentPath.erase( Name.length()-1, 1);
	if( which == 0 )
	{
		currentPath.append( "_m1.camber" );
	}
	else if( which == 1 )
	{
		currentPath.append( "_m2.camber" );
	}
	else if( which == 2 )
	{
		currentPath.append( "_m3.camber" );
	}
	Max[which] = -9999;
	Min[which] = 9999;
	fstream input(currentPath.c_str());
	int index = 0;
	string temp;
	for(int i = 0; i < 5; i++)
	{
		input>>temp;
	}
	while(!input.eof())
	{
		NUMOFCAMBERSIZE[which]+=1;
		input>>index;
		input>>temp;
		input>>size[which][index];
		input>>temp;
		input>>temp;
	}
	NUMOFCAMBERSIZE[which] = index + 1;
	for(int i =0; i < NUMOFCAMBERSIZE[which]; i++)
	{
		if(size[which][i] != -1)
		{
			continue;
		}
		if(i == 0)
		{
			size[which][i] = size[which][i+1];
			continue;
		}
		if(i == (Num-1))
		{
			size[which][i] = size[which][i-1];
			continue;
		}
		size[which][i] = ( size[which][i + 1] + size[which][i - 1] )/2;
	}
	for(int i = 0; i < NUMOFCAMBERSIZE[which]; i++)
	{
		double tempValue = size[which][i];
		std::cerr<<tempValue<<std::endl;
		if(Max[which] < tempValue)
		{
			Max[which] = tempValue;
		}
		if(Min[which] > tempValue)
		{
			Min[which] = tempValue;
		}
	}
	return;
}

float Camber::getCurrentCamberAt( int marker, int cur_frame )
{
	return this->size[marker][cur_frame];
}

void Camber::loadSize()
{
	for( int i = 0; i < 3; i++ )
	{
		loadSizeFor( i );
	}
}

float Camber::getSize(int index)
{
	float area=0;
	int node=numOfList[0][index];
	for(int i=0; i<node-2; i++)
	{
		//glVertex3f((float)data[frameIndex][i*3]*scale,(float)(data[frameIndex][i*3+2]*scale),(float)(data[frameIndex][i*3+1]*scale));
		float v1[2], v2[2];
		v1[0]=data[index][(node-2-i)*3  ] - data[index][(node-1)*3  ];
		v1[1]=data[index][(node-2-i)*3+1] - data[index][(node-1)*3+1];

		v2[0]=data[index][(node-3-i)*3  ] - data[index][(node-1)*3  ];
		v2[1]=data[index][(node-3-i)*3+1] - data[index][(node-1)*3+1];

		float root1 = sqrt(v2[0]*v2[0] + v2[1]*v2[1]);
		float root2 = sqrt(v1[0]*v1[0] + v1[1]*v1[1]);
		float angle1 = acos((v1[0]*v2[0] + v1[1]*v2[1])/(root1*root2));
		float projection = root1*root2*sin(angle1)/2;
		area+= projection;
		//float angle2 = atan(normal[index][3]/sqrt(normal[index][0]*normal[index][0]+normal[index][1]*normal[index][1]));
		//area+=projection/cos(angle2);
	}
	return area;
}

void Camber::drawCurve( int cur_frame_num )
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	myColor3 color;
	//glTranslatef( -_center[0], -_center[1], -_center[2] );
	for( int j = 0; j < 3; j++ )
	{
		if( !_ENABLED[j] )
		{
			continue;
		}
		if( cur_frame_num > Num || _flag[ cur_frame_num ] == false )
		{
			return;
		}

		//double percentage = this->getPercentage( j, cur_frame_num );
		//Lab3D pos = _cut->get_Position( percentage );
		//_lab->get_Color( pos, color);
		//		color
		//glColor3f( color.a/255.0, color.b/255.0, color.c/255.0 );
		if( match.contains(cur_frame_num) )
		{
			glColor3f( 1.0, 0.0, 0.0 );
		}
		else
		{
			glColor3f( 0.0, 0.2, 0.9 );
		}

		//norm = TDHelper::getNormal( norm, (float)data[currentSelected][cur_frame_num][0]*Scale,(float)(data[currentSelected][cur_frame_num][2]*Scale),-(float)(data[currentSelected][cur_frame_num][1]*Scale),(float)data[currentSelected][cur_frame_num][3]*Scale,(float)(data[currentSelected][cur_frame_num][5]*Scale),-(float)(data[currentSelected][cur_frame_num][4]*Scale),(float)data[currentSelected][cur_frame_num][6]*Scale,(float)(data[currentSelected][cur_frame_num][8]*Scale),-(float)(data[currentSelected][cur_frame_num][7]*Scale));
		//glNormal3f( norm[0], norm[1], norm[2] );
		glBegin( GL_LINES );
		for(int i=1; i<numOfList[j][cur_frame_num]; i++)
		{
			glVertex3f((float)data[j][cur_frame_num][(i-1)*3]*Scale,(float)(data[j][cur_frame_num][(i-1)*3+2]*Scale),-(float)(data[j][cur_frame_num][(i-1)*3+1]*Scale));
			glVertex3f((float)data[j][cur_frame_num][(i)*3]*Scale,(float)(data[j][cur_frame_num][(i)*3+2]*Scale),-(float)(data[j][cur_frame_num][(i)*3+1]*Scale));
		}
		glEnd();
	}
	//glTranslatef( _center[0], _center[1], _center[2] );
}

void Camber::initVolum()
{
	_center[0] = ( (float)data[0][0][0] + (float)data[0][NUMOFCAMBERSIZE[0]-1][0] )*(float)Scale/2.0;
	_center[1] = ( (float)data[0][0][2] + (float)data[0][NUMOFCAMBERSIZE[0]-1][2] )*(float)Scale/2.0;
	_center[2] = ( (float)data[0][0][1] + (float)data[0][NUMOFCAMBERSIZE[0]-1][1] )*(float)Scale/2.0;
	return;
}

void Camber::findMatchDiscreptor( DESCRIPTOR dis, int marker, float &rate, int &Oindex )
{
	this->_dis = dis;
	float matchrate = 9999.9;
	int index = -1;
	for( int i = 0; i < NUMOFCAMBERSIZE[marker] - 1; i++ )
	{
		float rate = matchRate( i, marker );
		if( rate == 0.0 )
		{
			//matchRate( i );
			continue;
		}
		if( matchrate > rate )
		{
			matchrate = rate;
			index = i;
		}
	}
	rate = matchrate;
	Oindex = index;
	return;
}

void Camber::acceptDiscreptor( DESCRIPTOR dis )
{
	this->_dis = dis;
	float matchrate = 9999;
	int index = -1;
	for( int i = 0; i < NUMOFCAMBERSIZE[0]; i++ )
	{
		if( matchrate > matchRate( i, 0 ) )
		{
			matchrate = matchRate( i, 0 );
			index = i;
		}
	}
	this->match.append( index );
	return;
}

void Camber::drawCamberMatching( QPainter* painter )
{
	//QVector<QLineF> curve;
	//QLineF baseLine( QPointF( (float)data[0][20][0]*10000, (float)data[0][20][1]*10000 ), QPointF( (float)data[0][20][(numOfList[0][0]-1)*3+0]*10000, (float)data[0][20][(numOfList[0][0]-1)*3+1]*10000 ) );
 //	float scale = this->_dis.length / baseLine.length();
	//for(int i=1; i<numOfList[0][20]; i++)
	//{
	//	curve.append( QLineF( (float)data[0][20][(i-1)*3+0]*10000, (float)data[0][20][(i-1)*3+1]*10000, (float)data[0][20][(i)*3+0]*10000, (float)data[0][20][(i)*3+1]*10000) );
	//	//glVertex3f((float)data[j][cur_frame_num][(i-1)*3]*Scale, (float)(data[j][cur_frame_num][(i-1)*3+2]*Scale), -(float)(data[j][cur_frame_num][(i-1)*3+1]*Scale));
	//	//glVertex3f((float)data[j][cur_frame_num][(i)*3]*Scale,(float)(data[j][cur_frame_num][(i)*3+2]*Scale),-(float)(data[j][cur_frame_num][(i)*3+1]*Scale));
	//}
	////
	//painter->drawLines( curve );
	//painter->setPen( QColor( 255, 0, 0, 255 ) );
	//painter->drawLine( baseLine );
	////
	//QPointF base[2], intersection[2];
	//base[0] = baseLine.pointAt( 3.6 / 7.2 );
	//base[1] = baseLine.pointAt( 6.1 / 7.2 );
	//for( int i = 0; i < 2; i++ )
	//{
	//	QLineF nor = baseLine.normalVector();
	//	nor.translate( base[i] - baseLine.p1() );
	//	int Num = 0;
	//	int loop = 0;
	//	while( Num != 1 && loop < 5 )
	//	{
	//		for( int k = 0; k < curve.size(); k++ )
	//		{
	//			int type = curve[k].intersect( nor, &intersection[i] );
	//			if( type == 1 )
	//			{
	//				Num += 1;
	//				break;
	//			}
	//		}
	//		nor.setP1( nor.p1() + ( nor.p1() - nor.p2() ) );
	//		nor.setP2( nor.p2() + ( nor.p2() - nor.p1() ) );
	//		loop++;
	//	}
	//	painter->setPen( QColor( 0, 255, 0, 255 ) );
	//	painter->drawLine( nor );
	//}
	//painter->drawEllipse( intersection[0], 2, 2 );
	//painter->drawEllipse( intersection[1], 2, 2 );
	return;
}

float Camber::matchRate( int cur_frame_num, int marker )
{
	float mainValue;
	if( this->_dis.value1 > this->_dis.value2 )
	{
		mainValue = this->_dis.value1;
	}
	else
	{
		mainValue = this->_dis.value1;
	}
	if( numOfList[marker][cur_frame_num] == 2 )
	{
		return 9999.9;
	}
	float totalRate = sqrt( pow( this->size[marker][cur_frame_num] - mainValue, 2 ) );
	//QList<QLineF> curve;
	//QPointF init( (float)data[marker][cur_frame_num][0], (float)data[marker][cur_frame_num][1] );
	//int indexForEnd = ( numOfList[marker][cur_frame_num] - 1 )*3;
	//QPointF end( (float)data[marker][cur_frame_num][indexForEnd], (float)data[marker][cur_frame_num][indexForEnd + 1] );
	//QLineF baseLine( init, end );
	////
 //	float scale = this->_dis.length / baseLine.length();
	////
	//for(int i=1; i< numOfList[0][cur_frame_num]; i++)
	//{
	//	curve.append( QLineF( (float)data[marker][cur_frame_num][(i-1)*2], (float)data[marker][cur_frame_num][(i-1)*3+1], (float)data[marker][cur_frame_num][(i)*3+0], (float)data[marker][cur_frame_num][(i)*3+1]) );
	//}
	//QPointF base[2], intersection[2];
	//base[0] = baseLine.pointAt( 3.6 / 7.2 );
	//base[1] = baseLine.pointAt( 6.1 / 7.2 );
	//for( int i = 0; i < 2; i++ )
	//{
	//	QLineF nor = baseLine.normalVector();
	//	nor.translate( base[i] - baseLine.p1() );
	//	int Num = 0;
	//	int loop = 0;
	//	while( Num != 1 && loop < 5 )
	//	{
	//		for( int k = 0; k < curve.size(); k++ )
	//		{
	//			int type = curve[k].intersect( nor, &intersection[i] );
	//			if( type == 1 )
	//			{
	//				Num += 1;
	//				break;
	//			}
	//		}
	//		nor.setP1( nor.p1() + ( nor.p1() - nor.p2() ) );
	//		nor.setP2( nor.p2() + ( nor.p2() - nor.p1() ) );
	//		loop++;
	//	}
	//	if( Num == 0 )
	//	{
	//		intersection[i] = base[i];
	//	}
	//}
	//float length = baseLine.length() * scale;
	//float height1 = QLineF( base[0], intersection[0] ).length() * scale;
	//float height2 = QLineF( base[1], intersection[1] ).length() * scale;
	//totalRate = sqrt( pow( float(_dis.height1 - height1), 2 ) + pow( float(_dis.height2 - height2), 2 ) );
	return totalRate;
}