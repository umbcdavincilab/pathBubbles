#include "Bat.h"

GLfloat DiffuseMaterial[] = {0.5, 0.5, 0.5}; //set the material to red
GLfloat whiteSpecularMaterial[] = {1.0, 1.0, 1.0}; //set the light specular to white
GLfloat mShininess[] = {128}; //set the shininess of the material


Bat::Bat( LabObject *Lab, LabCut *Cut, int Scale, std::string name ) 
	: rotateX( 90 )//-60
, rotateY( 180 )
, cur_frame_num( 0 )
, currentX( 0 )
, currentZ( -4 )
, SURFACE( true )
, WIREFRAME( true )
, SYMBOLS( false )
, _path( name )
, _camberPerfix( "data/Bat/" )
, _SELECTINGWAKE( false )
, _LEVEL1ENABLED( 0.0 )
, _LEVEL2ENABLED( 0.0 )
, _HIGHLIGHTED( -1 )
, _THRESHHOLD( 5 )
, _STATE( 0 )
, frame_color( 0, 0.247*255, 0.16*255 )
, _arrowType(0)
, _arrowCylinderSize(DefaultCylinderSize)
, _arrowSphereSize(DefaultSphereSize)
{
	lab = Lab;
	cut = Cut;
	scale = Scale*1.2;
	norm = new float[3];
	//			for color used in highlight
	_HIGHLIGHTCOLOR[0] = 1.0;
	_HIGHLIGHTCOLOR[1] = 1.0;
	_HIGHLIGHTCOLOR[2] = 0.0;
	_MARKERCOLOR[0] = 0.0;
	_MARKERCOLOR[1] = 1.0;
	_MARKERCOLOR[2] = 1.0;

	markerPositions = new MarkersMotion( name.c_str(), 1000);
	loadMarkers();

	_wingDisplayFlag = new bool[num_markers * 2];
	for ( int i = 0; i < num_markers*2; i ++ )
	{
		_wingDisplayFlag[i] = false;
	}

	//back wing
	_wingDisplayFlag[5] = true;
	_wingDisplayFlag[8] = true;
	_wingDisplayFlag[11] = true;
	_wingDisplayFlag[14] = true;
	_wingDisplayFlag[5 + num_markers] = true;
	_wingDisplayFlag[8 + num_markers] = true;
	_wingDisplayFlag[11 + num_markers] = true;
	_wingDisplayFlag[14 + num_markers] = true;

	loadWake();
	delete markerPositions;

	extractName();

	batTri = new ssv("data/mesh.csv");
	num_tri=batTri->GetNumFrames();
	loadMesh();
	delete batTri;

	camber = new Camber( _camberPath.c_str(), num_frames, scale, lab, cut );
	num_frames = camber->getFrameOfCamber();
	//camber = new Camber( _camberPath.c_str(), 53, scale );
	//			initialize bone structure
	initBones();
	updateShiftVector();
	TDHelper::load44IdentityMatrix( _matrix );
	TDHelper::load44IdentityMatrix( _matrix_shadow );
}

QString Bat::checkPath(QString name)
{
	QString result;
	if( !QFile::exists( name ) )
	{
		result.append( "Marker data cannot be opened!" );
		return result;
	}
	else if( !QFile::exists( "data/mesh.csv" ) )
	{
		result.append( "mesh data cannot be opened!" );
		return result;
	}
	QString camberPath = name.left( name.length() - 4 );
	camberPath.append( "/" );
	result.append( Camber::checkPath( camberPath ) );
	return result;
}

Bat::Bat( Bat& target ) : rotateX( 90 )
, rotateY( 180 )
, cur_frame_num( 0 )
, currentX( 0 )
, currentZ( -3 )
, SURFACE( true )
, WIREFRAME( true )
, _SELECTINGWAKE( false )
, _margin( 0.0 )
, _STATE( 1 )
, _LEVEL1ENABLED( 0.0 )
, _LEVEL2ENABLED( 0.0 )
{
	lab = target.getLab();
	cut = target.getCut();
	scale = target.getScale();
	//******************************
	//	copy markers
	//******************************
	num_markers =  target.getTotalNumberOfMarkers();
	num_frames = target.getTotalNumberOfFrame();
	_fileName = target.getname();
	this->_markers = new double*[num_frames];

	for( int i = 0; i < num_frames; i++ )
	{
		_markers[i] = new double[ 3*num_markers ];
	}
	_wingDisplayFlag = new bool[num_markers * 2];
	for ( int i = 0; i < num_markers*2; i ++ )
	{
		_wingDisplayFlag[i] = false;
	}

	//back wing
	_wingDisplayFlag[5] = true;
	_wingDisplayFlag[8] = true;
	_wingDisplayFlag[11] = true;
	_wingDisplayFlag[14] = true;
	_wingDisplayFlag[5 + num_markers] = true;
	_wingDisplayFlag[8 + num_markers] = true;
	_wingDisplayFlag[11 + num_markers] = true;
	_wingDisplayFlag[14 + num_markers] = true;

	target.writeMarkersData( _markers );
	
	//******************************
	//	copy wake structure
	//******************************
	wakeLeft = new double*[num_frames];
	wakeRight = new double*[num_frames];
	for(int i = 0; i < num_frames; i++)
	{
		wakeLeft[i] = new double[ 4 * 3 ];
		wakeRight[i] = new double[ 4 * 3];
	}
	target.writeWakeData( wakeLeft, wakeRight );
	//******************************
	//	copy mesh
	//******************************
	num_tri = target.getTotalNumberOfTri();
	meshIndex = new int*[num_tri];
	meshLength = new int[num_tri];
	target.writeMeshData( meshIndex, meshLength );
	//******************************
	//	copy camber data
	//******************************
	camber = target.getCamber();
	//******************************
	norm = new float[3];
	//******************************
	shiftVectorForLevel1[0] = shiftVectorForLevel1[1] = shiftVectorForLevel1[2] = 0;
	shiftVectorForLevel2[0] = shiftVectorForLevel2[1] = shiftVectorForLevel2[2] = 0;
	//******************************
	initBones();
	return;
} 

Bat::~Bat()
{
	delete norm;
}

void Bat::setCompareBatColor( QColor color )
{
	this->frame_color = color;
	return;
}

void Bat::loadMarkers()
{
	num_markers =  markerPositions->GetNumMarkers();
	num_frames = markerPositions->GetNumFrames();

	_markers = new double*[num_frames];
	for( int i = 0; i < num_frames; i++ )
	{
		_markers[i] = new double[ 3*num_markers ];
	}
	//			load data
	for( int i = 0; i < num_frames; i++ )
	{
		for( int j = 0; j< num_markers; j++ )
		{
			markerPositions->GetMarkerPositionAtFrame( i, j, _markers[i][j*3], _markers[i][j*3+2], _markers[i][j*3+1]);
			_markers[i][j*3  ] = _markers[i][j*3  ]*this->scale;
			_markers[i][j*3+2] = _markers[i][j*3+2]*this->scale;
			_markers[i][j*3+1] = _markers[i][j*3+1]*this->scale;
		}
	}
}

void Bat::writeMarkersData( double** result )
{
	for( int i = 0; i < num_frames; i++ )
	{
		for( int j = 0; j< num_markers; j++ )
		{
			result[i][j*3  ] = _markers[i][j*3  ];
			result[i][j*3+2] = _markers[i][j*3+2];
			result[i][j*3+1] = _markers[i][j*3+1];
		}
	}
}

void Bat::extractName()
{
	int start = _path.find_last_of( "/" );
	int end = _path.find_last_of( "." );
	int length = end - start - 1;
	_fileName = _path.substr( start + 1, length );

	_camberPath = _path.substr( 0, _path.length() - 4 );
	_camberPath.append( "/" );
	return;
}

void Bat::getboundingBox( int framenum )
{
	xmin = xmax = _markers[framenum][0];
	ymin = ymax = _markers[framenum][2];
	zmin = zmax = _markers[framenum][1];

	for (int i = 1; i < num_markers; i ++)
	{
		if ( _markers[framenum][0] < xmin )
		{
			xmin = _markers[framenum][0];
		}
		if ( _markers[framenum][0] > xmax )
		{
			xmax = _markers[framenum][0];
		}

		if ( _markers[framenum][2] < ymin )
		{
			ymin = _markers[framenum][2];
		}
		if ( _markers[framenum][2] > ymax )
		{
			ymax = _markers[framenum][2];
		}

		if ( _markers[framenum][1] < zmin )
		{
			zmin = _markers[framenum][1];
		}
		if ( _markers[framenum][1] > zmax )
		{
			zmax = _markers[framenum][1];
		}
	}
}

void Bat::renderScene()
{
	//*************************************************************************
	//rotateXYby();
	glTranslatef( 0.0, 0.0, currentZ );

	GLdouble backPlane[4] = {0.0, 0.0, 1.0, 4.0 };
	GLdouble leftPlane[4] = {1.0, 0.0, 0.0, 2.0 };
	GLdouble rightPlane[4] = {-1.0, 0.0, 0.0, 2.0 };
	GLdouble topPlane[4] = { 0.0, 1.0, 0.0, 2.0 };
	GLdouble bottomPlane[4] = { 0.0, -1.0, 0.0, 2.0 };
	glClipPlane( GL_CLIP_PLANE0, backPlane );
	glClipPlane( GL_CLIP_PLANE1, leftPlane );
	glClipPlane( GL_CLIP_PLANE2, rightPlane );
	glClipPlane( GL_CLIP_PLANE3, topPlane );
	glClipPlane( GL_CLIP_PLANE4, bottomPlane );

	drawBox();

	glTranslatef( 0.0, 0.0, -2.0 );
	renderShadow();
	rotateXYby();
	translateBy();

	//rotateXYby();
	//*************************************************************************
	//*************************************************************************

	glEnable( GL_CLIP_PLANE0 );
 	glEnable( GL_CLIP_PLANE1 );
 	glEnable( GL_CLIP_PLANE2 );
 	glEnable( GL_CLIP_PLANE3 );
 	glEnable( GL_CLIP_PLANE4 );

	drawBat();
	//drawMarkers();
	drawWave();
	drawFrame();
	drawCamber();
	drawBatBody();
	//*************************************************************************
	//*************************************************************************
	
	//drawBone();

	//glColor3f(0.0f, 0.0f, 1.0f); // blue reflective propertie
	//GLfloat redDiffuseMaterial[] = {1.0, 0.0, 0.0}; //set the material to red
	//glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, DiffuseMaterial);
	////GLfloat whiteSpecularMaterial[] = {1.0, 1.0, 1.0}; //set the light specular to white
	//glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, whiteSpecularMaterial);
	////GLfloat mShininess[] = {128}; //set the shininess of the material
	//glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mShininess);
	//glTranslatef( 0, 0, 0.4 );
	//glutSolidTeapot( 0.5);//( 0.4, 64, 64 );
	//glTranslatef( 0, 0, -0.4 );
	//glBegin(GL_TRIANGLES);
	//glVertex3f(-1.0f, 0.0f, 0.0f);
	//glVertex3f(0.0f, -1.0f, 0.0f);
	//glVertex3f(1.0f, 0.0f, 0.0f);
	//glEnd();
	//*************************************************************************

	glDisable( GL_CLIP_PLANE0 );
	glDisable( GL_CLIP_PLANE1 );
	glDisable( GL_CLIP_PLANE2 );
	glDisable( GL_CLIP_PLANE3 );
	glDisable( GL_CLIP_PLANE4 );

	restoreTranslation();
	restoreRotation();
	glTranslatef( 0.0, 0.0, 2.0 );
	glTranslatef( 0, 0, -currentZ );
	//*************************************************************************

}

void Bat::getCurrentRotaTransMatrix()
{
	TDHelper::load44IdentityMatrix( _matrix );
	TDHelper::load44IdentityMatrix( _matrix_shadow );
	updateTranslateMatrix( 0.0, 0.0, currentZ-2.0 );
	updateTranslateMatrix_Shadow( 0.0, 0.0, currentZ-2.0 );
	updateRotationMatrix();
	updateRotationMatrix_Shadow();
	updateTranslateMatrix( -currentX, 0.0, 0.0 );
	updateTranslateMatrix_Shadow( -currentX, 0.0, 0.0 );
}

void Bat::updateTranslateMatrix( double x, double y, double z )
{
	//double translateMatrix[] = {
	//       1.0, 0.0, 0.0, 0.0,
	//       0.0, 1.0, 0.0, 0.0,
	//       0.0, 0.0, 1.0, 0.0,
	//       x, y, z, 1.0
	//   };
	double translateMatrix[] = {
		1.0, 0.0, 0.0, x,
		0.0, 1.0, 0.0, y,
		0.0, 0.0, 1.0, z,
		0.0, 0.0, 0.0, 1.0
	};
	//double moveMatrix[] = {
	//       1.0, 0.0, 0.0, 0.0,
	//       0.0, 1.0, 0.0, 0.0,
	//       0.0, 0.0, 1.0, currentZ,
	//       0.0, 0.0, 0.0, 1.0
	//   };
	double tempMatrix[16];
	for( int i = 0; i < 16; i++ )
	{
		tempMatrix[i] = _matrix[i];
	}
	TDHelper::matrix44By44( tempMatrix, translateMatrix, _matrix );
	return;
}

void Bat::updateTranslateMatrix_Shadow( double x, double y, double z )
{
	//double translateMatrix[] = {
	//       1.0, 0.0, 0.0, 0.0,
	//       0.0, 1.0, 0.0, 0.0,
	//       0.0, 0.0, 1.0, 0.0,
	//       x, y, z, 1.0
	//   };
	double translateMatrix[] = {
		1.0, 0.0, 0.0, x,
		0.0, 1.0, 0.0, y,
		0.0, 0.0, 1.0, z,
		0.0, 0.0, 0.0, 1.0
	};
	//double moveMatrix[] = {
	//       1.0, 0.0, 0.0, 0.0,
	//       0.0, 1.0, 0.0, 0.0,
	//       0.0, 0.0, 1.0, currentZ,
	//       0.0, 0.0, 0.0, 1.0
	//   };
	double tempMatrix[16];
	for( int i = 0; i < 16; i++ )
	{
		tempMatrix[i] = this->_matrix_shadow[i];
	}
	TDHelper::matrix44By44( tempMatrix, translateMatrix, _matrix_shadow );
	return;
}

void Bat::updateRotationMatrix()
{
	double rotateYMatrix[] = {
		cos(TDHelper::fromDegreeToRadio(rotateX)), 0.0, sin(TDHelper::fromDegreeToRadio(rotateX)), 0.0,
		0.0, 1.0,                0.0, 0.0,
		-sin(TDHelper::fromDegreeToRadio(rotateX)), 0.0, cos(TDHelper::fromDegreeToRadio(rotateX)), 0.0,
		0.0, 0.0,                0.0, 1.0
	};
	double rotateXMatrix[] = {
		1.0, 0.0, 0.0, 0.0,
		0.0, cos(TDHelper::fromDegreeToRadio(rotateY)), -sin(TDHelper::fromDegreeToRadio(rotateY)), 0.0,
		0.0, sin(TDHelper::fromDegreeToRadio(rotateY)),  cos(TDHelper::fromDegreeToRadio(rotateY)), 0.0,
		0.0, 0.0, 0.0, 1.0
	};
	double tempMatrix[16];
	for( int i = 0; i < 16; i++ )
	{
		tempMatrix[i] = _matrix[i];
	}
	TDHelper::matrix44By44( tempMatrix, rotateXMatrix, _matrix );
	for( int i = 0; i < 16; i++ )
	{
		tempMatrix[i] = _matrix[i];
	}
	TDHelper::matrix44By44( tempMatrix, rotateYMatrix, _matrix );
}

void Bat::updateRotationMatrix_Shadow()
{
	double rotateYMatrix[] = {
		 cos(TDHelper::fromDegreeToRadio(rotateX)), sin(TDHelper::fromDegreeToRadio(rotateX)), 0.0, 0.0,
		 -sin(TDHelper::fromDegreeToRadio(rotateX)), cos(TDHelper::fromDegreeToRadio(rotateX)), 0.0, 0.0,
		                                       0.0,                                       0.0, 1.0, 0.0,
		                                       0.0,                                       0.0, 0.0, 1.0
	};
	double rotateXMatrix[] = {
		1.0, 0.0, 0.0, 0.0,
		0.0, cos(TDHelper::fromDegreeToRadio(-rotateY)), -sin(TDHelper::fromDegreeToRadio(-rotateY)), 0.0,
		0.0, sin(TDHelper::fromDegreeToRadio(-rotateY)),  cos(TDHelper::fromDegreeToRadio(-rotateY)), 0.0,
		0.0, 0.0, 0.0, 1.0
	};
	double tempMatrix[16];
	for( int i = 0; i < 16; i++ )
	{
		tempMatrix[i] = _matrix_shadow[i];
	}
	TDHelper::matrix44By44( tempMatrix, rotateXMatrix, _matrix_shadow );
	for( int i = 0; i < 16; i++ )
	{
		tempMatrix[i] = _matrix_shadow[i];
	}
	TDHelper::matrix44By44( tempMatrix, rotateYMatrix, _matrix_shadow );
}
void Bat::loadMesh()
{
	meshIndex = new int*[num_tri];
	meshLength = new int[num_tri];
	float temp;
	for(int i = 0; i < num_tri; i++)
	{
		int length = batTri->GetNumColumnsByIndex(i);
		meshLength[i] = length;
		meshIndex[i] = new int[length];
		for(int j = 0; j < length; j++)
		{
			batTri->GetDataByIndex(i, j, &temp);
			meshIndex[i][j] = temp;
		}
	}
	return;
}

void Bat::writeMeshData( int** index, int* lenght )
{
	for(int i = 0; i < num_tri; i++)
	{
		lenght[i] = meshLength[i];
		index[i] = new int[meshLength[i]];
		for(int j = 0; j < meshLength[i]; j++)
		{
			index[i][j] = meshIndex[i][j];
		}
	}
	return;
}

bool Bat::isBone(int index1, int index2)
{
	for(int i = 0; i < 11; i++)
	{
		if( (bone[i*2] == index1 && bone[i*2 + 1] == index2) || (bone[i*2] == index2 && bone[i*2 + 1] == index1) )
		{
			return true;
		}
	}
	return false;
}

bool Bat::isJoint(int index)
{
	for(int i = 0; i < 11; i++)
	{
		if( bone[ i * 2 ] == index || bone[ i * 2 + 1] == index )
		{
			return true;
		}
	}
	return false;
}

void Bat::Key_backward()
{
	if(cur_frame_num > 0)
	{
		cur_frame_num -= 1;
		updateShiftVector();
	}
}

void Bat::Key_forward()
{
	if(cur_frame_num < num_frames - 1 )
	{
		cur_frame_num += 1;
		updateShiftVector();
	}
}

void Bat::writeWakeData( double** left, double** right )
{
	for(int i = 0; i < num_frames; i++)
	{
		//		left
		left[i][ 0 * 3 + 0] = wakeLeft[i][ 0 * 3 + 0];
		left[i][ 0 * 3 + 1] = wakeLeft[i][ 0 * 3 + 1];
		left[i][ 0 * 3 + 2] = wakeLeft[i][ 0 * 3 + 2];
		//		right
		right[i][ 0 * 3 + 0] = wakeRight[i][ 0 * 3 + 0];
		right[i][ 0 * 3 + 1] = wakeRight[i][ 0 * 3 + 1];
		right[i][ 0 * 3 + 2] = wakeRight[i][ 0 * 3 + 2];

		//		left
		left[i][ 1 * 3 + 0] = wakeLeft[i][ 1 * 3 + 0];
		left[i][ 1 * 3 + 1] = wakeLeft[i][ 1 * 3 + 1];
		left[i][ 1 * 3 + 2] = wakeLeft[i][ 1 * 3 + 2];
		//		right
		right[i][ 1 * 3 + 0] = wakeRight[i][ 1 * 3 + 0];
		right[i][ 1 * 3 + 1] = wakeRight[i][ 1 * 3 + 1];
		right[i][ 1 * 3 + 2] = wakeRight[i][ 1 * 3 + 2];

		//		left
		left[i][ 2 * 3 + 0] = wakeLeft[i][ 2 * 3 + 0];
		left[i][ 2 * 3 + 1] = wakeLeft[i][ 2 * 3 + 1];
		left[i][ 2 * 3 + 2] = wakeLeft[i][ 2 * 3 + 2];
		//		right
		right[i][ 2 * 3 + 0] = wakeRight[i][ 2 * 3 + 0];
		right[i][ 2 * 3 + 1] = wakeRight[i][ 2 * 3 + 1];
		right[i][ 2 * 3 + 2] = wakeRight[i][ 2 * 3 + 2];

		//		left
		left[i][ 3 * 3 + 0] = wakeLeft[i][ 3 * 3 + 0];
		left[i][ 3 * 3 + 1] = wakeLeft[i][ 3 * 3 + 1];
		left[i][ 3 * 3 + 2] = wakeLeft[i][ 3 * 3 + 2];
		//		right
		right[i][ 3 * 3 + 0] = wakeRight[i][ 3 * 3 + 0];
		right[i][ 3 * 3 + 1] = wakeRight[i][ 3 * 3 + 1];
		right[i][ 3 * 3 + 2] = wakeRight[i][ 3 * 3 + 2];
	}
	return;
}
void Bat::loadWake()
{
	wakeLeft = new double*[num_frames];
	wakeRight = new double*[num_frames];
	for(int i = 0; i < num_frames; i++)
	{
		wakeLeft[i] = new double[ 4 * 3 ];
		wakeRight[i] = new double[ 4 * 3];
	}
	//			the structure of the wake is [[ 2 * 4 * 3 ]]
	for(int i = 0; i < num_frames; i++)
	{
		//markerPositions->GetMarkerPositionAtFrame(i, 5, xx, yy, zz);
		//		left
		wakeLeft[i][ 0 * 3 + 0] = _markers[i][15];
		wakeLeft[i][ 0 * 3 + 1] = _markers[i][16];
		wakeLeft[i][ 0 * 3 + 2] = _markers[i][17];
		//		right
		wakeRight[i][ 0 * 3 + 0] = _markers[i][15];
		wakeRight[i][ 0 * 3 + 1] = _markers[i][16];
		wakeRight[i][ 0 * 3 + 2] = -_markers[i][17];

		//		left
		wakeLeft[i][ 1 * 3 + 0] = _markers[i][24];
		wakeLeft[i][ 1 * 3 + 1] = _markers[i][25];
		wakeLeft[i][ 1 * 3 + 2] = _markers[i][26];
		//		right
		wakeRight[i][ 1 * 3 + 0] = _markers[i][24];
		wakeRight[i][ 1 * 3 + 1] = _markers[i][25];
		wakeRight[i][ 1 * 3 + 2] = -_markers[i][26];

		//		left
		wakeLeft[i][ 2 * 3 + 0] = _markers[i][33];
		wakeLeft[i][ 2 * 3 + 1] = _markers[i][34];
		wakeLeft[i][ 2 * 3 + 2] = _markers[i][35];
		//		right
		wakeRight[i][ 2 * 3 + 0] = _markers[i][33];
		wakeRight[i][ 2 * 3 + 1] = _markers[i][34];
		wakeRight[i][ 2 * 3 + 2] = -_markers[i][35];

		//		left
		wakeLeft[i][ 3 * 3 + 0] = _markers[i][42];
		wakeLeft[i][ 3 * 3 + 1] = _markers[i][43];
		wakeLeft[i][ 3 * 3 + 2] = _markers[i][44];
		//		right
		wakeRight[i][ 3 * 3 + 0] = _markers[i][42];
		wakeRight[i][ 3 * 3 + 1] = _markers[i][43];
		wakeRight[i][ 3 * 3 + 2] = -_markers[i][44];
	}
}

void Bat::drawTriangle(double *v1, double *v2, double *v3)
{
	glBegin(GL_POLYGON);
	//glNormal3dv(v1);
	glVertex3dv(v1);

	//glNormal3dv(v2);
	glVertex3dv(v2);

	//glNormal3dv(v3);
	glVertex3dv(v3);
	glEnd();
}

void Bat::drawSurface(double *v1, double *v2, double *v3, double *v4)
{
	glBegin(GL_POLYGON);
	glNormal3dv(v1);
	glVertex3dv(v1);

	glNormal3dv(v2);
	glVertex3dv(v2);

	glNormal3dv(v3);
	glVertex3dv(v3);

	glNormal3dv(v4);
	glVertex3dv(v4);
	glEnd();
	glBegin(GL_POLYGON);
	glNormal3dv(v4);
	glVertex3dv(v4);

	glNormal3dv(v3);
	glVertex3dv(v3);

	glNormal3dv(v2);
	glVertex3dv(v2);

	glNormal3dv(v1);
	glVertex3dv(v1);
	glEnd();
	//ignore the surface of the beginning of the cylinder
}

float Bat::Alpha(int index)
{
	float alpha = (float)index / (float)cur_frame_num;
	return alpha;
}

void Bat::reOrder(double *x, double *y, double *z, double *p1, double *p2, double *p3)
{
	p1[0] = x[0];
	p2[0] = x[1];
	p3[0] = x[2];

	p1[1] = y[0];
	p2[1] = y[1];
	p3[1] = y[2];

	p1[2] = z[0];
	p2[2] = z[1];
	p3[2] = z[2];
}

void Bat::drawCamber()
{
	for(int i = 0; i <= cur_frame_num; i++)
	{
		if( i > camber->getMaxNum( 0 ) )
		{
			continue;
		}
		camber->drawCamber( i );
	}
}

void Bat::setCurrentNumberOfFrame( float input )
{
	if( input <= 1.0 )
	{
		cur_frame_num = int( input * float(num_frames));
		updateShiftVector();
	}
	currentX = _markers[cur_frame_num][45];
	return;
}
//void Bat::drawCube(GLuint cube, GLdouble dx, GLdouble dy, GLdouble dz,
//                        GLdouble angle)
//{
//    glTranslated(dx, dy, dz);
//    glRotated(angle, 0.0, 0.0, 1.0);
//    glCallList(cube);
//}
//
//void Bat::normalizeAngle(int *angle)
//{
//    while (*angle < 0)
//        *angle += 360 * 16;
//    while (*angle > 360 * 16)
//        *angle -= 360 * 16;
//}
//
//void Bat::drawGround()
//{
//    GLfloat mat_diffuse[]={0.4, 0.4, 0.4, 1.0};
//    GLfloat mat_ambient[]={0.4, 0.4, 0.4, 1.0};
//    GLfloat mat_specular[]={0.4, 0.4, 0.4, 1.0};
//    GLfloat mat_shininess[]={100.0};
//    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
//    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
//    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
//    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
//    //glPushMatrix();
//    glColor4f(0.5274f, 0.5274f, 0.5274f, 0.9f);
//    glBegin(GL_QUADS);
//        glNormal3f(0, 1, 0);
//        glVertex3f(-400, -100, -400);
//        glVertex3f(-400, -100, 400);
//        glVertex3f( 400, -100, 400);
//        glVertex3f( 400, -100, -400);
//    glEnd();
//    //*******************************************************
//    glDisable(GL_LIGHTING);
//    glColor3f(0.0f, 0.149f, 1.0f);
//    for(int i=0; i<11; i++)
//    {
//        glBegin(GL_LINES);
//            glVertex3f(-400+i*80, -100, -400);
//            glVertex3f(-400+i*80, -100, 400);
//        glEnd();
//    }
//    for(int i=0; i<11; i++)
//    {
//        glBegin(GL_LINES);
//            glVertex3f(-400, -100, -400+i*80);
//            glVertex3f(400, -100, -400+i*80);
//        glEnd();
//    }
//    glEnable(GL_LIGHTING);
//    //glPopMatrix();
//}
//
//
//void Bat::drawBar(int i)
//{
//    glBegin(GL_QUADS);
//        glVertex3f(-0.1+i*1.25-4, 4.0, 0.0);
//        glVertex3f(0.1+i*1.25-4, 4.0, 0.0);
//        glVertex3f(0.1+i*1.25-4, -4.0, 0.0);
//        glVertex3f(-0.1+i*1.25-4, -4.0, 0.0);
//    glEnd();
//}
//
//
void Bat::getNormal(float *norm, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3)
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
}
//
void Bat::drawBat()
{
	//		need to be updated
	//glColor3f( 0.5, 0.5, 0.5 );
	//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   bat_ambient);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   bat_diffuse);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  bat_specular);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, bat_shininess);
	glColor3f(0.4f, 0.4f, 0.4f);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, DiffuseMaterial);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, whiteSpecularMaterial);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mShininess);
	int index=0;
	double xx,yy,zz;
	float vT;
	int v[4];
	double x[4], y[4], z[4];
	for(int i=0; i<num_tri; i++)
	{
		for(int j=0; j<meshLength[i]; j++)
		{
			v[j]=meshIndex[i][j];
		}
		for(int j=0; j<meshLength[i]; j++)
		{
			x[j] = _markers[cur_frame_num][v[j]*3];
			y[j] = _markers[cur_frame_num][v[j]*3+1];
			z[j] = _markers[cur_frame_num][v[j]*3+2];
		}
		if(meshLength[i]==3)
		{
			//posShiftForLeft( i );
			getNormal( norm, (float)x[0],(float)y[0],(float)z[0],(float)x[1],(float)y[1],(float)z[1],(float)x[2],(float)y[2],(float)z[2]);
			//******************************************************
			glNormal3f(-norm[0], -norm[1], -norm[2]);
			glBegin(GL_TRIANGLES);
			glVertex3f((float)_markers[cur_frame_num][v[0]*3], (float)_markers[cur_frame_num][v[0]*3+1], (float)_markers[cur_frame_num][v[0]*3+2]);
			glVertex3f((float)_markers[cur_frame_num][v[1]*3], (float)_markers[cur_frame_num][v[1]*3+1], (float)_markers[cur_frame_num][v[1]*3+2]);
			glVertex3f((float)_markers[cur_frame_num][v[2]*3], (float)_markers[cur_frame_num][v[2]*3+1], (float)_markers[cur_frame_num][v[2]*3+2]);
			glEnd();
			//******************************************************
			glNormal3f(norm[0], norm[1], norm[2]);
			glBegin(GL_TRIANGLES);
			glVertex3f((float)_markers[cur_frame_num][v[0]*3], (float)_markers[cur_frame_num][v[0]*3+1], (float)_markers[cur_frame_num][v[0]*3+2]);
			glVertex3f((float)_markers[cur_frame_num][v[2]*3], (float)_markers[cur_frame_num][v[2]*3+1], (float)_markers[cur_frame_num][v[2]*3+2]);
			glVertex3f((float)_markers[cur_frame_num][v[1]*3], (float)_markers[cur_frame_num][v[1]*3+1], (float)_markers[cur_frame_num][v[1]*3+2]);
			glEnd();

			//negShiftForLeft( i );
			posShiftForRight( i );

			getNormal( norm, (float)x[0],(float)y[0],-(float)z[0],(float)x[1],(float)y[1],-(float)z[1],(float)x[2],(float)y[2],-(float)z[2]);
			glNormal3f(-norm[0], -norm[1], -norm[2]);
			glBegin(GL_TRIANGLES);
			glVertex3f((float)_markers[cur_frame_num][v[0]*3], (float)_markers[cur_frame_num][v[0]*3+1], -(float)_markers[cur_frame_num][v[0]*3+2]);
			glVertex3f((float)_markers[cur_frame_num][v[1]*3], (float)_markers[cur_frame_num][v[1]*3+1], -(float)_markers[cur_frame_num][v[1]*3+2]);
			glVertex3f((float)_markers[cur_frame_num][v[2]*3], (float)_markers[cur_frame_num][v[2]*3+1], -(float)_markers[cur_frame_num][v[2]*3+2]);
			glEnd();
			//**************************************************************
			glNormal3f(norm[0], norm[1], norm[2]);
			glBegin(GL_TRIANGLES);
			glVertex3f((float)_markers[cur_frame_num][v[0]*3], (float)_markers[cur_frame_num][v[0]*3+1], -(float)_markers[cur_frame_num][v[0]*3+2]);
			glVertex3f((float)_markers[cur_frame_num][v[2]*3], (float)_markers[cur_frame_num][v[2]*3+1], -(float)_markers[cur_frame_num][v[2]*3+2]);
			glVertex3f((float)_markers[cur_frame_num][v[1]*3], (float)_markers[cur_frame_num][v[1]*3+1], -(float)_markers[cur_frame_num][v[1]*3+2]);
			glEnd();
			negShiftForRight( i );
		}
	}
}
//
void Bat::drawMarkers()
{
	//			need to be updated
	glColor3f( _MARKERCOLOR[0], _MARKERCOLOR[1], _MARKERCOLOR[2] );
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, DiffuseMaterial);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, whiteSpecularMaterial);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mShininess);
	double xx,yy,zz;
	for(int i=0; i<num_markers; i++)
	{
		//markerPositions->GetMarkerPositionAtFrame(cur_frame_num, i, xx, yy, zz);
		drawMarker( float(_markers[cur_frame_num][i*3]), float(_markers[cur_frame_num][i*3+1]), float(_markers[cur_frame_num][i*3+2]));
		drawMarker( float(_markers[cur_frame_num][i*3]), float(_markers[cur_frame_num][i*3+1]), -float(_markers[cur_frame_num][i*3+2]));
	}
}

void Bat::drawFrame()
{
	glEnable(GL_LINE_SMOOTH);
	int index=0;
	double xx,yy,zz;
	float vT;
	int v[4];
	double x[4], y[4], z[4];
	glLineWidth(0.5);
	//glLineWidth( 1 );
	
 	for(int i=0; i<num_tri; i++)
 	{
 		// draw the wire frame of the bat
 		if(meshLength[i]==3)
 		{
 			for(int j=0; j<meshLength[i]; j++)
 			{
 				v[j] = meshIndex[i][j];
 			}
 			for(int j=0; j<meshLength[i]; j++)
 			{
 				x[j] = _markers[cur_frame_num][v[j]*3];
 				y[j] = _markers[cur_frame_num][v[j]*3+1];
 				z[j] = _markers[cur_frame_num][v[j]*3+2];
 			}

 			glColor4f( frame_color.redF(), frame_color.greenF(), frame_color.blueF(), 0.6f );

 			if( isBone( v[0], v[1] ) )
 			{
 				drawCylinder((float)x[0],(float)y[0], (float)z[0],(float)x[1],(float)y[1], (float)z[1], 0.02);
 			}
 			//
 			if( isBone( v[0], v[2] ) )
 			{
 				drawCylinder((float)x[0],(float)y[0], (float)z[0],(float)x[2],(float)y[2], (float)z[2], 0.02);
 			}
 			//
 			if( isBone( v[1], v[2] ) )
 			{
 				drawCylinder((float)x[2],(float)y[2], (float)z[2],(float)x[1],(float)y[1], (float)z[1], 0.02);
 			}
 			posShiftForRight( i );
 
 			if( isBone( v[0], v[1] ) )
 			{
 				drawCylinder((float)x[0],(float)y[0], -(float)z[0],(float)x[1],(float)y[1], -(float)z[1], 0.02);
 			}
 			//
 			if( isBone( v[0], v[2] ) )
 			{
 				drawCylinder((float)x[0],(float)y[0], -(float)z[0],(float)x[2],(float)y[2], -(float)z[2], 0.02);
 			}
 			//
 			if( isBone( v[1], v[2] ) )
 			{
 				drawCylinder((float)x[2],(float)y[2], -(float)z[2],(float)x[1],(float)y[1], -(float)z[1], 0.02);
 			}
 			negShiftForRight( i );
 		}
 	}

	//**************************************************
	for(int i=0; i<num_tri; i++)
	{
		// draw the wire frame of the bat
		if(meshLength[i]==3)
		{
			for(int j=0; j<meshLength[i]; j++)
			{
				v[j] = meshIndex[i][j];
			}
			for(int j=0; j<meshLength[i]; j++)
			{
				x[j] = _markers[cur_frame_num][v[j]*3];
				y[j] = _markers[cur_frame_num][v[j]*3+1];
				z[j] = _markers[cur_frame_num][v[j]*3+2];

				if( v[j] == this->_HIGHLIGHTED - num_markers)
				{
					glColor3f( _HIGHLIGHTCOLOR[0], _HIGHLIGHTCOLOR[1], _HIGHLIGHTCOLOR[2] );
				}
				else
				{
					glColor3f( _MARKERCOLOR[0], _MARKERCOLOR[1], _MARKERCOLOR[2] );
				}
				//drawMarker( x[j], y[j], z[j] );

				posShiftForRight( i );
				if( v[j] == this->_HIGHLIGHTED)
				{
					glColor3f( _HIGHLIGHTCOLOR[0], _HIGHLIGHTCOLOR[1], _HIGHLIGHTCOLOR[2] );
				}
				else
				{
					glColor3f( _MARKERCOLOR[0], _MARKERCOLOR[1], _MARKERCOLOR[2] );
				}
				//drawMarker( x[j], y[j], -z[j]);
				negShiftForRight( i );
			}
			glColor4f( frame_color.redF(), frame_color.greenF(), frame_color.blueF(), 0.6f );
			if( !isBone( v[0], v[1] ) )
			{
				glBegin(GL_LINES);
				glVertex3f( (float)x[0], (float)y[0], (float)z[0] );
				glVertex3f( (float)x[1], (float)y[1], (float)z[1] );
				glEnd();
			}
			//
			if( !isBone( v[0], v[2] ) )
			{
				glBegin(GL_LINES);
				glVertex3f( (float)x[0], (float)y[0], (float)z[0] );
				glVertex3f( (float)x[2], (float)y[2], (float)z[2] );
				glEnd();
			}
			//
			if( !isBone( v[1], v[2] ) )
			{
				glBegin(GL_LINES);
				glVertex3f( (float)x[1], (float)y[1], (float)z[1] );
				glVertex3f( (float)x[2], (float)y[2], (float)z[2] );
				glEnd();
			}
			//******************************************************
			posShiftForRight( i );
			if( !isBone( v[0], v[1] ) )
			{
				glBegin(GL_LINES);
				glVertex3f( (float)x[0], (float)y[0], -(float)z[0] );
				glVertex3f( (float)x[1], (float)y[1], -(float)z[1] );
				glEnd();
			}
			//
			if( !isBone( v[0], v[2] ) )
			{
				glBegin(GL_LINES);
				glVertex3f( (float)x[0], (float)y[0], -(float)z[0] );
				glVertex3f( (float)x[2], (float)y[2], -(float)z[2] );
				glEnd();
			}
			//
			if( !isBone( v[1], v[2] ) )
			{
				glBegin(GL_LINES);
				glVertex3f( (float)x[1], (float)y[1], -(float)z[1] );
				glVertex3f( (float)x[2], (float)y[2], -(float)z[2] );
				glEnd();
			}
			negShiftForRight( i );
		}
	}

	//glLineWidth( 0.5 );
	glDisable(GL_LINE_SMOOTH);
}
//
void Bat::drawBone()
{
	//			need to be updated
	//glPushMatrix();
	double xx,yy,zz;
	double x[2], y[2], z[2];
	glColor4f(0.2f, 0.247f, 0.16f, 0.9f);
	for(int i=0; i<11; i++)
	{
		//markerPositions->GetMarkerPositionAtFrame(cur_frame_num, bone[i*2], xx, yy, zz);
		x[0] = _markers[cur_frame_num][bone[i*2]*3];
		y[0] = _markers[cur_frame_num][bone[i*2]*3 + 1];
		z[0] = _markers[cur_frame_num][bone[i*2]*3 + 2];
		//markerPositions->GetMarkerPositionAtFrame(cur_frame_num, bone[i*2+1], xx, yy, zz);
		x[1] = _markers[cur_frame_num][bone[i*2+1]*3];
		y[1] = _markers[cur_frame_num][bone[i*2+1]*3 + 1];
		z[1] = _markers[cur_frame_num][bone[i*2+1]*3 + 2];

		drawCylinder((float)x[0],(float)y[0], (float)z[0],(float)x[1],(float)y[1], (float)z[1],0.02);
		drawCylinder((float)x[0],(float)y[0],-(float)z[0],(float)x[1],(float)y[1],-(float)z[1],0.02);
	}
	//glPopMatrix();
}
//
void Bat::drawMarker(float x, float y, float z)
{
	glColor4f(0.0f, 0.347f, 1.0f, 0.9f);
	glTranslatef(x, y, z);
	glutSolidSphere( 0.03, 8, 8);
	glTranslatef(-x, -y, -z);
}

void Bat::drawCylinder(double x1, double y1, double z1, double x2, double y2, double z2, double size)
{
	double p1[3], p2[3];

	p1[0] = x1;
	p1[1] = y1;
	p1[2] = z1;

	p2[0] = x2;
	p2[1] = y2;
	p2[2] = z2;

	glColor4f(0.1f, 0.147f, 0.16f, 0.6f);
	drawCylinder(p1, p2, size);
// 	glColor4f(0.1f, 0.147f, 0.46f, 0.9f);
// 	glEnable(GL_LINE_SMOOTH);
// 	glLineWidth( size );
// 	glBegin(GL_LINES);
// 
// 	glVertex3f( x1, y1, z1 );
// 	glVertex3f( x2, y2, z2 );
// 
// 	glEnd();
// 	glDisable(GL_LINE_SMOOTH);
}

void Bat::drawCylinder( double *pos1, double *pos2, double size )
{
	double Vertex1[32][3], Vertex2[32][3];
	for( int i=0; i<32; i++)
	{
		Vertex1[i][1]=(GLdouble)cos(theta*i)*size+pos1[1];
		Vertex2[i][1]=(GLdouble)cos(theta*i)*size+pos2[1];
		Vertex1[i][2]=(GLdouble)sin(theta*i)*size+pos1[2];
		Vertex2[i][2]=(GLdouble)sin(theta*i)*size+pos2[2];
		Vertex1[i][0]=pos1[0];
		Vertex2[i][0]=pos2[0];
	}
	for(int i=0;i<32-1;i++)
	{
		drawSurface(&Vertex1[i+1][0],&Vertex1[i][0],&Vertex2[i][0],&Vertex2[i+1][0]);
	}
	drawSurface(&Vertex1[0][0],&Vertex1[N-1][0],&Vertex2[N-1][0],&Vertex2[0][0]);
	return;
}

void Bat::drawArrow(double *a, double *b)
{
	if (_arrowType == 0 || _arrowType == 2)
	{
		drawCylinder((float)a[0], (float)a[1], (float)a[2], (float)b[0], (float)b[1], (float)b[2], _arrowCylinderSize);
	}

	if (_arrowType == 0 || _arrowType == 1)
	{
		glPushMatrix();
		glTranslated((float)b[0], (float)b[1], (float)b[2]);
		glutSolidSphere(_arrowSphereSize, 16, 16);
		glPopMatrix();
	}
}

void Bat::drawWave()
{
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, DiffuseMaterial);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, whiteSpecularMaterial);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mShininess);

	//			need to be updated
	//glEnable (GL_BLEND);
	//glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	float trans = 1;
	double faces[2][4][3];
	if(cur_frame_num!=0)
	{
		//markerPositions->GetMarkerPositionAtFrame(0, 14, xx, yy, zz);
		faces[0][0][0] = _markers[0][42];
		faces[0][0][1] = _markers[0][43];
		faces[0][0][2] = _markers[0][44];

		faces[0][1][0] = _markers[0][33];
		faces[0][1][1] = _markers[0][34];
		faces[0][1][2] = _markers[0][35];

		faces[0][2][0] = _markers[0][24];
		faces[0][2][1] = _markers[0][25];
		faces[0][2][2] = _markers[0][26];

		faces[0][3][0] = _markers[0][15];
		faces[0][3][1] = _markers[0][16];
		faces[0][3][2] = _markers[0][17];
	}
	for(int i=1; i<=cur_frame_num-1; i++)
	{
		faces[1][0][0] = _markers[i][42];
		faces[1][0][1] = _markers[i][43];
		faces[1][0][2] = _markers[i][44];

		faces[1][1][0] = _markers[i][33];
		faces[1][1][1] = _markers[i][34];
		faces[1][1][2] = _markers[i][35];

		faces[1][2][0] = _markers[i][24];
		faces[1][2][1] = _markers[i][25];
		faces[1][2][2] = _markers[i][26];

		faces[1][3][0] = _markers[i][15];
		faces[1][3][1] = _markers[i][16];
		faces[1][3][2] = _markers[i][17];


		trans=(float)i/(float)cur_frame_num;

		//***********************************************************************
		//              drawing surface1
		//***********************************************************************
		if(SURFACE)
		{
			if( i >= _wakeStart && i <= _wakeEnd )
			{
				glColor4f( 1.0f, 0.0f, 0.0f, 1.0);
			}
			else
			{
				glColor4f(0.4558f, 0.5215f, 1.0f, trans);
			}
			if( needToBeHighlighted( i ) )
			{
				glColor4f(0.8558f, 0.5215f, 1.0f, trans);
			}
			//
			//drawMarker( faces[1][3][0], faces[1][3][1], faces[1][3][2] );
			drawSurface(faces[1][1],faces[1][0], faces[0][0],faces[0][1]);
			drawSurface(faces[0][1],faces[0][2], faces[1][2],faces[1][1]);
			drawSurface(faces[0][2],faces[0][3], faces[1][3],faces[1][2]);
		}
		//***********************************************************************
		//          drawing wire frame1
		//***********************************************************************
		if(WIREFRAME)
		{
			if( i >= _wakeStart && i <= _wakeEnd )
			{
				glColor4f( 1.0f, 0.0f, 0.0f, 1.0);
			}
			else
			{
				glColor4f(0.1f, 0.1f, 0.5f, trans);
			}
			//glColor4f(0.1f, 0.1f, 0.5f, trans);
			if( needToBeHighlighted( i ) )
			{
				glColor4f(0.6f, 0.1f, 0.5f, trans);
			}
			drawCylinder(faces[0][0][0],faces[0][0][1],faces[0][0][2],faces[0][1][0],faces[0][1][1],faces[0][1][2], 0.01 );
			//drawCylinder( (float*)faces[0][0], (float*)faces[0][1], 0.01 );
			drawCylinder(faces[0][1][0],faces[0][1][1],faces[0][1][2],faces[0][2][0],faces[0][2][1],faces[0][2][2], 0.01 );
			//drawCylinder( (float*)faces[0][1], (float*)faces[0][2], 0.01 );
			drawCylinder(faces[0][2][0],faces[0][2][1],faces[0][2][2],faces[0][3][0],faces[0][3][1],faces[0][3][2], 0.01 );
			//drawCylinder( (float*)faces[0][2], (float*)faces[0][3], 0.01 );
			//glLineWidth(1);
		}
		//***********************************************************************
		for(int j=0; j<4; j++)
		{
			faces[0][j][2] = -faces[0][j][2];
			faces[1][j][2] = -faces[1][j][2];
			faces[2][j][2] = -faces[2][j][2];
		}
		//***********************************************************************
		//              drawing surface2
		//***********************************************************************
		if(SURFACE)
		{
			if( i >= _wakeStart && i <= _wakeEnd )
			{
				glColor4f( 1.0f, 0.0f, 0.0f, 1.0);
			}
			else
			{
				glColor4f(0.4558f, 0.5215f, 1.0f, trans);
			}
			if( needToBeHighlighted( i ) )
			{
				glColor4f(0.8558f, 0.5215f, 1.0f, trans);
			}
			drawSurface(faces[1][1],faces[1][0], faces[0][0],faces[0][1]);
			posShiftForRight( 8 );
			drawSurface(faces[0][1],faces[0][2], faces[1][2],faces[1][1]);
			negShiftForRight( 8 );

			posShiftForRight( 14 );
			drawSurface(faces[0][2],faces[0][3], faces[1][3],faces[1][2]);
			negShiftForRight( 14 );
		}
		////***********************************************************************
		////          drawing wire frame2
		////***********************************************************************
		if(WIREFRAME)
		{
			if( i >= _wakeStart && i <= _wakeEnd )
			{
				glColor4f( 1.0f, 0.0f, 0.0f, 1.0);
			}
			else
			{
				glColor4f(0.1f, 0.1f, 0.5f, trans);
			}
			//glColor4f(0.1f, 0.1f, 0.5f, trans);
			if( needToBeHighlighted( i ) )
			{
				glColor4f(0.6f, 0.1f, 0.5f, trans);
			}
			drawCylinder(faces[0][0][0],faces[0][0][1],faces[0][0][2],faces[0][1][0],faces[0][1][1],faces[0][1][2], 0.01);
			posShiftForRight( 8 );
			drawCylinder(faces[0][1][0],faces[0][1][1],faces[0][1][2],faces[0][2][0],faces[0][2][1],faces[0][2][2], 0.01);
			negShiftForRight( 8 );

			posShiftForRight( 14 );
			drawCylinder(faces[0][2][0],faces[0][2][1],faces[0][2][2],faces[0][3][0],faces[0][3][1],faces[0][3][2], 0.01);
			negShiftForRight( 14 );
		}

		//***********************************************************************
		for(int j=0; j<4; j++)
		{
			faces[0][j][2] = -faces[0][j][2];
			faces[1][j][2] = -faces[1][j][2];
			faces[2][j][2] = -faces[2][j][2];
		}

		//*************************************************************
		//          Exchange for next frame
		//*************************************************************
		for(int j=0; j<4; j++)
		{
			faces[0][j][0] = faces[1][j][0];
			faces[0][j][1] = faces[1][j][1];
			faces[0][j][2] = faces[1][j][2];
			faces[0][j][3] = faces[1][j][3];
		}
	}

	//glDisable(GL_BLEND);

	if (SYMBOLS)
	{
		drawWavewithSymbols();
	}
}

void Bat::drawWavewithSymbols()
{
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, DiffuseMaterial);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, whiteSpecularMaterial);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mShininess);

	double faces[3][40][3];

	if(cur_frame_num != 0)
	{
		for (int j = 0; j < num_markers; j ++)
		{
			faces[0][j][0] = _markers[0][j*3];
			faces[0][j][1] = _markers[0][j*3+1];
			faces[0][j][2] = _markers[0][j*3+2];
		}
	}
	for(int i=1; i<=cur_frame_num-1; i++)
	{
		for (int j = 0; j < num_markers; j ++)
		{
			faces[1][j][0] = _markers[i][j*3];
			faces[1][j][1] = _markers[i][j*3+1];
			faces[1][j][2] = _markers[i][j*3+2];
		}

		for (int j = 0; j < num_markers; j ++)
		{
			for (int k = 0; k < 3; k ++)
			{
				faces[2][j][k] = 0.6 * faces[0][j][k] + 0.4 * faces[1][j][k];
			}
		}

		for (int j = 0; j < num_markers; j ++)
		{
			if (_wingDisplayFlag[j + num_markers])
			{
				setSymbolColor(j, i);
				drawArrow( faces[2][j], faces[1][j] );
			}
		}

		//***********************************************************************
		for(int j = 0; j < num_markers; j ++)
		{
			faces[0][j][2] = -faces[0][j][2];
			faces[1][j][2] = -faces[1][j][2];
			faces[2][j][2] = -faces[2][j][2];
		}
		//***********************************************************************
		//              drawing surface2
		//***********************************************************************

		for (int j = 0; j < num_markers; j ++)
		{
			if (_wingDisplayFlag[j])
			{
				setSymbolColor(j, i);
				if (j < 3 || (j > 8 && j < 17) )
				{
					drawArrow(faces[2][j], faces[1][j]);
				}
				else if (j > 5 && j < 9)
				{
					posShiftForRight( 8 );
					drawArrow(faces[2][j], faces[1][j]);
					negShiftForRight( 8 );
				}
				else
				{
					posShiftForRight( 14 );
					drawArrow( faces[2][j], faces[1][j] );
					negShiftForRight( 14 );
				}
			}
		}

		//***********************************************************************
		for(int j = 0; j < num_markers; j ++)
		{
			faces[0][j][2] = -faces[0][j][2];
			faces[1][j][2] = -faces[1][j][2];
			faces[2][j][2] = -faces[2][j][2];
		}

		//*************************************************************
		//          Exchange for next frame
		//*************************************************************
		for(int j = 0; j < num_markers; j ++)
		{
			faces[0][j][0] = faces[1][j][0];
			faces[0][j][1] = faces[1][j][1];
			faces[0][j][2] = faces[1][j][2];
			faces[0][j][3] = faces[1][j][3];
		}
	}
}

void Bat::setSymbolColor(int marker, int cur_frame)
{
	myColor3 color;
	float trans = 1.0;

	if (marker > 0 && marker < 4) // camber
	{
		double percentage = this->getCamber()->getPercentage( marker-1, cur_frame );
		Lab3D pos = this->getCamber()->getCut()->get_Position( percentage );
		this->getCamber()->getLab()->get_Color( pos, color);
		//		color
		glColor4f( color.a/255.0, color.b/255.0, color.c/255.0, trans );
	}
	else
	{
		trans = (float)cur_frame/(float)cur_frame_num;

		if( cur_frame >= _wakeStart && cur_frame <= _wakeEnd )
		{
			glColor4f( 1.0f, 0.0f, 0.0f, 1.0);
		}
		else
		{
			glColor4f(0.78f, 0.5f, 0.6f, trans);
		}
		if( needToBeHighlighted( cur_frame ) )
		{
			glColor4f(0.8558f, 0.5215f, 1.0f, trans);
		}
	}
}

BatContext Bat::getBatContext()
{
	BatContext result;
	result.cur_frame_num = cur_frame_num;
	result.frame_nums = num_frames;
	result.rotateX = rotateX;
	result.rotateY = rotateY;
	result.surface = SURFACE;
	result.wireframe = WIREFRAME;
	result.symbols = SYMBOLS;
	result.scale = scale;
	result.currentX = currentX;
	result.currentZ = currentZ;
	result.currentX = this->currentX;
	result.camber[0] = this->camber->getEnabled(0);
	result.camber[1] = this->camber->getEnabled(1);
	result.camber[2] = this->camber->getEnabled(2);
	return result;
}

void Bat::acceptContext(BatContext context)
{
	float rate = 1.0*context.cur_frame_num/context.frame_nums;
	cur_frame_num = (int)(num_frames * rate);
	rotateX = context.rotateX;
	rotateY = context.rotateY;
	SURFACE = context.surface;
	WIREFRAME = context.wireframe;
	SYMBOLS = context.symbols;
	scale = context.scale;
	currentX = context.currentX;
	currentZ = context.currentZ;
	this->camber->setCamberDisplay( 0, context.camber[0] );
	this->camber->setCamberDisplay( 1, context.camber[1] );
	this->camber->setCamberDisplay( 2, context.camber[2] );

	return;
}

void Bat::initBones()
{
	bone[0]=0;
	bone[1]=bone[2]=1;
	bone[3]=bone[4]=2;
	bone[5]=bone[6]=9;
	bone[7]=bone[8]=10;
	bone[9]=11;
	bone[10]=2;
	bone[11]=bone[12]=6;
	bone[13]=bone[14]=7;
	bone[15]=8;
	bone[16]=2;
	bone[17]=bone[18]=3;
	bone[19]=bone[20]=4;
	bone[21]=5;

	return;
}

void Bat::highlightSelectedWake( QRect selectedRect )
{
	getCurrentRotaTransMatrix();
	GLint viewport[4];
	GLdouble posX, posY, posZ;
	GLdouble modelview[16];
	GLdouble projection[16];

	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	glGetDoublev( GL_PROJECTION_MATRIX, projection );
	glGetIntegerv( GL_VIEWPORT, viewport );

	double point[4], point2[4];
	_wakeStart = 999;
	_wakeEnd = -999;
	for( int i = 0; i <= this->cur_frame_num; i++ )
	{
		point[0] = this->_markers[i][15];
		point[1] = this->_markers[i][16];
		point[2] = -this->_markers[i][17];
		point[3] = 1;
		TDHelper::matrix44By41( _matrix, point, point2);
		gluProject( point2[0], point2[1], point2[2], modelview, projection, viewport, &posX, &posY, &posZ );

		posY = _sceneWidth - posY;

		if( selectedRect.contains( posX, posY ) )
		{
			if( _wakeStart > i )
			{
				_wakeStart = i;
			}
			if( _wakeEnd < i )
			{
				_wakeEnd = i;
			}
		}
	}
	//gluProject( point2[0], point2[1], point2[2], modelview, projection, viewport, &posX, &posY, &posZ );
}

void Bat::beginSelectWake( int sceneWidth )
{
	_sceneWidth = sceneWidth;
}

void Bat::endSelectWake( int &begin, int &end )
{
	if ( _wakeStart > 0 && _wakeStart < getTotalNumberOfFrame()
		&& _wakeEnd > 0 && _wakeEnd < getTotalNumberOfFrame() )
	{
		begin = _wakeStart;
		end = _wakeEnd;
	}
	return;
}

void Bat::getMarker5ToSceneAtFrame( int index, int* pos )
{
	getCurrentRotaTransMatrix();
	GLint viewport[4];
	GLdouble posX, posY, posZ;
	GLdouble modelview[16];
	GLdouble projection[16];

	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	glGetDoublev( GL_PROJECTION_MATRIX, projection );
	glGetIntegerv( GL_VIEWPORT, viewport );

	double point[4], point2[4];
	point[0] = this->_markers[index][15];
	point[1] = this->_markers[index][16];
	point[2] = -this->_markers[index][17];
	point[3] = 1;
	TDHelper::matrix44By41( _matrix, point, point2); 
	gluProject( point2[0], point2[1], point2[2], modelview, projection, viewport, &posX, &posY, &posZ );

	pos[0] = posX;
	pos[1] = posY;
	return;
}

void Bat::getMarkerXToSceneAtFrame( int index, int marker, int* pos )
{
	getCurrentRotaTransMatrix();
	GLint viewport[4];
	GLdouble posX, posY, posZ;
	GLdouble modelview[16];
	GLdouble projection[16];

	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	glGetDoublev( GL_PROJECTION_MATRIX, projection );
	glGetIntegerv( GL_VIEWPORT, viewport );

	double point[4], point2[4];
	double offset[3];
	offset[0] = offset[1] = offset[2] = 0;
	if( marker == 9 )
	{
		offset[0] =  _LEVEL1ENABLED*shiftVectorForLevel1[0];
		offset[1] =  _LEVEL1ENABLED*shiftVectorForLevel1[1];
		offset[2] =  -_LEVEL1ENABLED*shiftVectorForLevel1[2];
	}
	else if( marker == 6 )
	{
		offset[0] =  _LEVEL1ENABLED*shiftVectorForLevel1[0] + _LEVEL2ENABLED*shiftVectorForLevel2[0];
		offset[1] =  _LEVEL1ENABLED*shiftVectorForLevel1[1] + _LEVEL2ENABLED*shiftVectorForLevel2[1];
		offset[2] =  -_LEVEL1ENABLED*shiftVectorForLevel1[2] - _LEVEL2ENABLED*shiftVectorForLevel2[2];
	}
	point[0] = this->_markers[index][marker*3] + offset[0];
	point[1] = this->_markers[index][marker*3+1] + offset[1];
	point[2] = -this->_markers[index][marker*3+2] + offset[2];
	point[3] = 1;
	TDHelper::matrix44By41( _matrix, point, point2);
	gluProject( point2[0], point2[1], point2[2], modelview, projection, viewport, &posX, &posY, &posZ );

	pos[0] = posX;
	pos[1] = posY;
	return;
}

void Bat::highlightNearMarkers( QPointF currentMouseOn, QPointF itemPos, int Height )
{
	getCurrentRotaTransMatrix();

	GLint viewport[4];
	GLdouble posX, posY, posZ;
	GLdouble modelview[16];
	GLdouble projection[16];

	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	glGetDoublev( GL_PROJECTION_MATRIX, projection );
	glGetIntegerv( GL_VIEWPORT, viewport );

	_HIGHLIGHTED = -1;
	double point[4], point2[4];
	int curDis = this->_THRESHHOLD;

	for( int i = 0; i < this->num_markers; i++ )
	{
		getCurrentRotaTransMatrix();
		point[0] = this->_markers[this->cur_frame_num][i*3];
		point[1] = this->_markers[this->cur_frame_num][i*3 + 1];
		point[2] = -this->_markers[this->cur_frame_num][i*3 + 2];
		point[3] = 1;

		TDHelper::matrix44By41( _matrix, point, point2);
		gluProject( point2[0], point2[1], point2[2], modelview, projection, viewport, &posX, &posY, &posZ );
		posY = Height - posY;
		posX = posX - itemPos.x();
		posY = posY - itemPos.y();
		float tempDis = TDHelper::distance2( currentMouseOn, QPointF( posX, posY ) );
		if( tempDis < curDis )
		{
			curDis = tempDis;
			_HIGHLIGHTED = i;
		}
		
		// check right marker
 		point[2] = - point[2];
 		if (i > 5 && i < 9)
 		{
 			posShiftForRight( 8 );
 		}
 		else if ( i > 2 && i < 6)
 		{
 			posShiftForRight( 14 );
 		}
 
 		getCurrentRotaTransMatrix();
 		TDHelper::matrix44By41( _matrix, point, point2);
 		gluProject( point2[0], point2[1], point2[2], modelview, projection, viewport, &posX, &posY, &posZ );
 		posY = Height - posY;
 		posX = posX - itemPos.x();
 		posY = posY - itemPos.y();
 		tempDis = TDHelper::distance2( currentMouseOn, QPointF( posX, posY ) );
 		if( tempDis < curDis )
 		{
 			curDis = tempDis;
 			_HIGHLIGHTED = i + getTotalNumberOfMarkers();
 		}
 
 		if (i > 5 && i < 9)
 		{
 			negShiftForRight( 8 );
 		}
 		else
 		{
 			negShiftForRight( 14 );
 		}
	}
	return;
}

void Bat::renderWakeFromTo()
{
	//			the current center
	if(_wakeRenderStart== 999 || _wakeRenderEnd == -999)
	{
	}
	else
	{
		float tempCenterX = ( _markers[ _wakeRenderStart ][45] + _markers[ _wakeRenderEnd ][45] ) / 2.0;
		float tempCenterY = ( _markers[ _wakeRenderStart ][46] + _markers[ _wakeRenderEnd ][46] ) / 2.0;
		float step = ( _wakeRenderEnd - _wakeRenderStart ) / 4.0;
		int currentFrame = -1;
		//			the length of the margin
		//************************************************************
		rotateXYby();
		//translateBy();
		//if( ( _wakeRenderEnd - _wakeRenderStart ) < 5 )
		//for( int i = _wakeRenderStart; i <= _wakeRenderEnd; i++ )
		for( int i = 0; i < 5; i++ )
		{
			currentFrame = int( i*step +0.5 ) + _wakeRenderStart;
			double curPosX, curPosY;
			this->cur_frame_num = currentFrame;
			curPosX = _markers[ this->cur_frame_num ][45];
			curPosY = _markers[ this->cur_frame_num ][46];
			double actualX = ( curPosX - tempCenterX ) * (this->_margin + 20.0) / 20.0;
			double actualY = ( curPosY - tempCenterY ) * (this->_margin + 20.0) / 20.0;
			glTranslatef( -curPosX, -curPosY, 0 );
			glTranslatef( actualX, actualY, 0 );
			drawBat();
			drawFrame();
			glTranslatef( -actualX, -actualY, 0 );
			glTranslatef( curPosX, curPosY, 0 );
		}
		//************************************************************
		//for( int i = _wakeRenderStart; i <= _wakeRenderEnd; i++ )
		//************************************************************
		//restoreTranslation();
		restoreRotation();
		//************************************************************
	}
}

void Bat::posShiftForLeft(int triangleIndex)
{
	if( triangleIndex > 7 && triangleIndex < 13 )
	{
		glTranslatef( 0, _LEVEL1ENABLED*shiftVectorForLevel1[1], _LEVEL1ENABLED*shiftVectorForLevel1[2]);
	}
	if( triangleIndex > 12 )
	{
		glTranslatef( 0, _LEVEL1ENABLED*shiftVectorForLevel1[1] + _LEVEL2ENABLED*shiftVectorForLevel2[1], _LEVEL1ENABLED*shiftVectorForLevel1[2] + _LEVEL2ENABLED*shiftVectorForLevel2[2]);
	}
}

void Bat::negShiftForLeft(int triangleIndex)
{
	if( triangleIndex > 7 && triangleIndex < 13 )
	{
		glTranslatef( 0, -_LEVEL1ENABLED*shiftVectorForLevel1[1], -_LEVEL1ENABLED*shiftVectorForLevel1[2]);
	}
	if( triangleIndex > 12 )
	{
		glTranslatef( 0, -_LEVEL1ENABLED*shiftVectorForLevel1[1] - _LEVEL2ENABLED*shiftVectorForLevel2[1], -_LEVEL1ENABLED*shiftVectorForLevel1[2] - _LEVEL2ENABLED*shiftVectorForLevel2[2]);
	}
}

void Bat::posShiftForRight(int triangleIndex)
{
	if( triangleIndex > 7 && triangleIndex < 13 )
	{
		glTranslatef( 0, _LEVEL1ENABLED*shiftVectorForLevel1[1], -_LEVEL1ENABLED*shiftVectorForLevel1[2]);
	}
	if( triangleIndex > 12 )
	{
		glTranslatef( 0, _LEVEL1ENABLED*shiftVectorForLevel1[1] + _LEVEL2ENABLED*shiftVectorForLevel2[1], -_LEVEL1ENABLED*shiftVectorForLevel1[2] - _LEVEL2ENABLED*shiftVectorForLevel2[2]);
	}
}

void Bat::negShiftForRight(int triangleIndex)
{
	if( triangleIndex > 7 && triangleIndex < 13 )
	{
		glTranslatef( 0, -_LEVEL1ENABLED*shiftVectorForLevel1[1], _LEVEL1ENABLED*shiftVectorForLevel1[2]);
	}
	if( triangleIndex > 12 )
	{
		glTranslatef( 0, -_LEVEL1ENABLED*shiftVectorForLevel1[1] - _LEVEL2ENABLED*shiftVectorForLevel2[1], _LEVEL1ENABLED*shiftVectorForLevel1[2] + _LEVEL2ENABLED*shiftVectorForLevel2[2]);
	}
}

void Bat::updateShiftVector()
{
	shiftVectorForLevel1[0] = 0;
	shiftVectorForLevel2[0] = 0;

	shiftVectorForLevel1[1] = this->_markers[cur_frame_num][2*3+1] - this->_markers[cur_frame_num][1*3+1];
	shiftVectorForLevel1[2] = this->_markers[cur_frame_num][2*3+2] - this->_markers[cur_frame_num][1*3+2];
	float length = sqrt( pow( shiftVectorForLevel1[1], 2) + pow( shiftVectorForLevel1[2], 2) );
	shiftVectorForLevel1[1] = shiftVectorForLevel1[1]/length * 0.3;
	shiftVectorForLevel1[2] = shiftVectorForLevel1[2]/length * 0.3;

	shiftVectorForLevel2[1] = this->_markers[cur_frame_num][6*3+1] - this->_markers[cur_frame_num][2*3+1];
	shiftVectorForLevel2[2] = this->_markers[cur_frame_num][6*3+2] - this->_markers[cur_frame_num][2*3+2];
	length = sqrt( pow( shiftVectorForLevel2[1], 2) + pow( shiftVectorForLevel2[2], 2) );
	shiftVectorForLevel2[1] = shiftVectorForLevel2[1]/length * 0.3;
	shiftVectorForLevel2[2] = shiftVectorForLevel2[2]/length * 0.3;
}

bool Bat::needToBeHighlighted( int index )
{
	if( _highLightList.count() == 0 )
	{
		return false;
	}
	if( _highLightList.contains( index ) )
	{
		return true;
	}
	if( _highLightList.contains( index+1 ) && _highLightList.contains( index-1 ) )
	{
		return true;
	}
	return false;
}

void Bat::renderVolum()
{
	camber->setCamberDisplay( 1, false );
	camber->setCamberDisplay( 2, false );
	rotateXYby();
	translateBy();
	camber->restoreCurve();
	//camber->translateCurve();
	//***********************************************************************
	for(int i = 0; i < num_frames; i++)
	{
		if( i > camber->getMaxNum( 0 ) )
		{
			continue;
		}
		camber->drawCurve( i );
	}
	//***********************************************************************
	//camber->restoreCurve();
	camber->translateCurve();
	restoreTranslation();
	restoreRotation();
}

void Bat::drawBatBody()
{
	
	//**********************************************
	double posleft[8][3], posRight[8][3];

	posRight[0][0] = _markers[cur_frame_num][0*3+0];
	posRight[0][1] = _markers[cur_frame_num][0*3+1];
	posRight[0][2] = _markers[cur_frame_num][0*3+2];
	
	for (int i = 1; i < 6; i ++)
	{
		posRight[i][0] = _markers[cur_frame_num][(i+11)*3];
		posRight[i][1] = _markers[cur_frame_num][(i+11)*3+1];
		posRight[i][2] = _markers[cur_frame_num][(i+11)*3+2];
	}

	posRight[6][0] = posRight[0][0];
	posRight[6][1] = posRight[0][1]+0.05;
	posRight[6][2] = posRight[0][2];
	posRight[7][0] = posRight[1][0];
	posRight[7][1] = posRight[1][1]+0.05;
	posRight[7][2] = posRight[1][2];

	for (int i = 0; i < 8; i++)
	{
		posleft[i][0] = posRight[i][0];
		posleft[i][1] = posRight[i][1];
		posleft[i][2] = -posRight[i][2];
	}

	glColor4f( 0.3, 0.3, 0.4, 1.0 );
	drawSurface( posRight[6], posleft[6], posleft[7], posRight[7] );
	drawSurface( posRight[0], posRight[6], posRight[7], posRight[1] );
	drawSurface( posRight[1], posRight[7], posleft[7], posleft[1] );
	drawSurface( posleft[1], posleft[7], posleft[6], posleft[0] );
	drawSurface( posleft[0], posleft[6], posRight[6], posRight[0] );
	drawSurface( posRight[4], posRight[5], posleft[5], posleft[4] );
	drawSurface( posRight[0], posRight[4], posleft[4], posleft[0] );
	drawSurface( posRight[1], posRight[5], posleft[5], posleft[1] );
	drawSurface( posRight[0], posRight[1], posRight[5], posRight[4] );
	drawSurface( posleft[0], posleft[4], posleft[5], posleft[1] );
	//**********************************************

	drawCylinder( posleft[1], posleft[2], 0.02 );
	drawCylinder( posleft[2], posleft[3], 0.02 );

	drawCylinder( posRight[1], posRight[2], 0.02 );
	drawCylinder( posRight[2], posRight[3], 0.02 );

	glTranslatef(posRight[0][0]+0.07, posRight[0][1], 0.0);
	glutSolidSphere(0.07, 32, 32);
	glTranslatef(-posRight[0][0]-0.07, -posRight[0][1], 0.0);

	glColor4f( 0.5, 0.6, 0.4, 1.0 );
	drawCylinder( posRight[0], posRight[6], 0.01 );
	drawCylinder( posRight[0], posRight[1], 0.01  );
	drawCylinder( posRight[1], posRight[7], 0.01  );
	drawCylinder( posRight[6], posRight[7], 0.01  );
	drawCylinder( posleft[0], posleft[6], 0.01  );
	drawCylinder( posleft[0], posleft[1], 0.01  );
	drawCylinder( posleft[1], posleft[7], 0.01  );
	drawCylinder( posleft[6], posleft[7], 0.01  );
	drawCylinder( posRight[0], posleft[0], 0.01  );
	drawCylinder( posRight[6], posleft[6], 0.01  );
	drawCylinder( posRight[1], posleft[1], 0.01  );
	drawCylinder( posRight[7], posleft[7], 0.01  );
	drawCylinder( posRight[4], posleft[4], 0.01  );
	drawCylinder( posRight[5], posleft[5], 0.01  );
	drawCylinder( posRight[0], posRight[4], 0.01  );
	drawCylinder( posRight[4], posRight[5], 0.01  );
	drawCylinder( posRight[5], posRight[1], 0.01  );
	drawCylinder( posleft[0], posleft[4], 0.01  );
	drawCylinder( posleft[4], posleft[5], 0.01  );
	drawCylinder( posleft[5], posleft[1], 0.01  );
	//**********************************************
	return;
}

void Bat::switchMarkerState()
{
	if (_HIGHLIGHTED > -1 && belongtoWing(_HIGHLIGHTED) )
	{
		_wingDisplayFlag[_HIGHLIGHTED] = !_wingDisplayFlag[_HIGHLIGHTED];
	}
}

bool Bat::belongtoWing( int index )
{
	int t = index;
	if (t < 5 || t == 8 || t == 11 || t == 14)
	{
		return true;
	}
	t -= num_markers;
	if (t < 5 || t == 8 || t == 11 || t == 14)
	{
		return true;
	}
	return false;
}

//void Bat::drawCube(GLuint cube, GLdouble dx, GLdouble dy, GLdouble dz,
//                        GLdouble angle)
//{
//    glTranslated(dx, dy, dz);
//    glRotated(angle, 0.0, 0.0, 1.0);
//    glCallList(cube);
//}
//
//void Bat::normalizeAngle(int *angle)
//{
//    while (*angle < 0)
//        *angle += 360 * 16;
//    while (*angle > 360 * 16)
//        *angle -= 360 * 16;
//}

void Bat::drawBox()
{
	GLfloat mat_diffuse[]={0.4, 0.4, 0.4, 1.0};
	GLfloat mat_ambient[]={0.4, 0.4, 0.4, 1.0};
	GLfloat mat_specular[]={0.4, 0.4, 0.4, 1.0};
	GLfloat mat_shininess[]={100.0};
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
    glPushMatrix();

	glDisable(GL_LIGHTING);
    glColor4f(0.5274f, 0.5274f, 0.5274f, 0.9f);
	
	//back
	glBegin(GL_POLYGON);
		//glNormal3f(0, 0, 1);
		glVertex3f(-2.1, -2.1, -4.2);
		glVertex3f(-2.1, 2.1, -4.2);
		glVertex3f( 2.1, 2.1, -4.2);
		glVertex3f( 2.1, -2.1, -4.2);
	glEnd();

	//left
	glBegin(GL_POLYGON);
		//glNormal3f(1, 0, 0);	
		glVertex3f(-2, -2, 0);
		glVertex3f(-2, 2, 0);
		glVertex3f(-2, 2, -4);
		glVertex3f(-2, -2, -4);
	glEnd();

	//right
	glBegin(GL_POLYGON);
		//glNormal3f(1, 0, 0);	
		glVertex3f(2, -2, 0);
		glVertex3f(2, -2, -4);
		glVertex3f(2, 2, -4);
		glVertex3f(2, 2, 0);
	glEnd();

	//top
	glBegin(GL_POLYGON);
		//glNormal3f(0, 1, 0);
		glVertex3f(-2, -2, -4);
		glVertex3f( 2, -2, -4);
		glVertex3f( 2, -2, 0);
		glVertex3f(-2, -2, 0);
	glEnd();

	glColor3f(0.7f, 0.7f, 0.7f);
	glLineWidth( 2.0 );
	
	glBegin(GL_LINES);
	glVertex3f(-1.9, -1.9, -4);
	glVertex3f(-1.9, 1.9, -4);
	glEnd();
	 
	glBegin(GL_LINES);
	glVertex3f(-1.9, 1.9, -4);
	glVertex3f( 1.9, 1.9, -4);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f( 1.9, 1.9, -4);
	glVertex3f( 1.9, -1.9, -4);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f( 1.9, -1.9, -4);
	glVertex3f(-1.9, -1.9, -4);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(-1.9, -1.9, -4);
	glVertex3f(-1.9, -1.9, -0.2);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(-1.9, 1.9, -4);
	glVertex3f(-1.9, 1.9, -0.2);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(1.9, 1.9, -4);
	glVertex3f(1.9, 1.9, -0.2);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(1.9, -1.9, -4);
	glVertex3f(1.9, -1.9, -0.2);
	glEnd();


    glColor3f(0.0f, 0.149f, 0.6f);
	glLineWidth(1.0);
    for(int i=0; i<11; i++)
    {
        glBegin(GL_LINES);
            glVertex3f(-2+i*0.4, 2, -4);
            glVertex3f(-2+i*0.4, 2, 0);
        glEnd();
    }
    for(int i=0; i<11; i++)
    {
        glBegin(GL_LINES);
            glVertex3f(-2, 2, -4+i*0.4);
            glVertex3f(2, 2, -4+i*0.4);
        glEnd();
    }
    glEnable(GL_LIGHTING);
	glPopMatrix();
	 //			draw Bat shadow by HANYU
	//double point[4], point2[4];
	//for( int i = 0; i <= this->cur_frame_num; i++ )
	//{
	//	point[0] = this->_markers[i][15];
	//	point[1] = this->_markers[i][16];
	//	point[2] = -this->_markers[i][17];
	//	point[3] = 1;
	//	TDHelper::matrix44By41( _matrix, point, point2);
	//}
	//for(int i=0; i<num_markers; i++)
	//{
	//	//markerPositions->GetMarkerPositionAtFrame(cur_frame_num, i, xx, yy, zz);
	//	TDHelper::matrix44By41();
	//	drawMarker( float(_markers[cur_frame_num][i*3]), float(_markers[cur_frame_num][i*3+1]), float(_markers[cur_frame_num][i*3+2]));
	//	drawMarker( float(_markers[cur_frame_num][i*3]), float(_markers[cur_frame_num][i*3+1]), -float(_markers[cur_frame_num][i*3+2]));
	//}
}
//
//
//void Bat::drawBar(int i)
//{
//    glBegin(GL_QUADS);
//        glVertex3f(-0.1+i*1.25-4, 4.0, 0.0);
//        glVertex3f(0.1+i*1.25-4, 4.0, 0.0);
//        glVertex3f(0.1+i*1.25-4, -4.0, 0.0);
//        glVertex3f(-0.1+i*1.25-4, -4.0, 0.0);
//    glEnd();
//}

void Bat::renderShadow()
{
	//for(int i=0; i<num_tri; i++)
	//{
	//	for(int j=0; j<meshLength[i]; j++)
	//	{
	//		v[j]=meshIndex[i][j];
	//	}
	//	for(int j=0; j<meshLength[i]; j++)
	//	{
	//		x[j] = _markers[cur_frame_num][v[j]*3];
	//		y[j] = _markers[cur_frame_num][v[j]*3+1];
	//		z[j] = _markers[cur_frame_num][v[j]*3+2];
	//	}
	//	if(meshLength[i]==3)
	//	{
	//		//posShiftForLeft( i );
	//		getNormal( norm, (float)x[0],(float)y[0],(float)z[0],(float)x[1],(float)y[1],(float)z[1],(float)x[2],(float)y[2],(float)z[2]);
	//		//******************************************************
	//		glNormal3f(-norm[0], -norm[1], -norm[2]);
	//		glBegin(GL_TRIANGLES);
	//		glVertex3f((float)_markers[cur_frame_num][v[0]*3], (float)_markers[cur_frame_num][v[0]*3+1], (float)_markers[cur_frame_num][v[0]*3+2]);
	//		glVertex3f((float)_markers[cur_frame_num][v[1]*3], (float)_markers[cur_frame_num][v[1]*3+1], (float)_markers[cur_frame_num][v[1]*3+2]);
	//		glVertex3f((float)_markers[cur_frame_num][v[2]*3], (float)_markers[cur_frame_num][v[2]*3+1], (float)_markers[cur_frame_num][v[2]*3+2]);
	//		glEnd();
	//		//******************************************************
	//		glNormal3f(norm[0], norm[1], norm[2]);
	//		glBegin(GL_TRIANGLES);
	//		glVertex3f((float)_markers[cur_frame_num][v[0]*3], (float)_markers[cur_frame_num][v[0]*3+1], (float)_markers[cur_frame_num][v[0]*3+2]);
	//		glVertex3f((float)_markers[cur_frame_num][v[2]*3], (float)_markers[cur_frame_num][v[2]*3+1], (float)_markers[cur_frame_num][v[2]*3+2]);
	//		glVertex3f((float)_markers[cur_frame_num][v[1]*3], (float)_markers[cur_frame_num][v[1]*3+1], (float)_markers[cur_frame_num][v[1]*3+2]);
	//		glEnd();
	//	}
	//}
	double x[3], y[3], z[3], x2[3], y2[3], z2[3];
	double pt[4], pt_map[4];
	int v[3];
	for(int i=0; i<num_tri; i++)
 	{
		for(int j=0; j<meshLength[i]; j++)
		{
			v[j]=meshIndex[i][j];
		}
		for(int j=0; j<meshLength[i]; j++)
		{
			x[j] = _markers[cur_frame_num][v[j]*3];
			y[j] = _markers[cur_frame_num][v[j]*3+1];
			z[j] = _markers[cur_frame_num][v[j]*3+2];
		}
 		// draw the wire frame of the bat
 		if(meshLength[i]==3)
 		{
 			for(int j=0; j<meshLength[i]; j++)
 			{
 				v[j] = meshIndex[i][j];
 			}
 			for(int j=0; j<meshLength[i]; j++)
 			{
				pt[0] = _markers[cur_frame_num][v[j]*3+0];
				pt[1] = _markers[cur_frame_num][v[j]*3+2];
				pt[2] = _markers[cur_frame_num][v[j]*3+1];
				pt[3] = 1;
				TDHelper::matrix44By41( this->_matrix_shadow, pt, pt_map );
				x[j] = pt_map[0];
				y[j] = pt_map[1];
				z[j] = 1.99;//pt_map[2];
				//
				pt[0] = _markers[cur_frame_num][v[j]*3+0];
				pt[1] =-_markers[cur_frame_num][v[j]*3+2];
				pt[2] = _markers[cur_frame_num][v[j]*3+1];
				pt[3] = 1;
				TDHelper::matrix44By41( this->_matrix_shadow, pt, pt_map );
				x2[j] = pt_map[0];
				y2[j] = pt_map[1];
				z2[j] = 1.99;//pt_map[2];
 			}
			double point1[3], point2[3], point3[3];
			glColor3f( 0.2, 0.2 , 0.2 );
			//			left
			reOrder(x, z, y, point1, point2, point3);

			drawTriangle(point1, point2, point3);
			drawTriangle(point1, point3, point2);

			reOrder(x2, z2, y2, point1, point2, point3);

			drawTriangle(point1, point2, point3);
			drawTriangle(point1, point3, point2);
 		}
 	}
}
