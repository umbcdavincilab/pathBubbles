#include "3DHelper.h"
#include "OpenManager.h"
#include "openglscene.h"
#include "qtbat.h"
#include "SM.h"
#include "qtsm.h"
#include "LabCut.h"
#include "LabObject.h"
#include "AllBats.h"
#include "GroupManager.h"
#include "webQuery.h"
#include "matBlock.h"
#include "webBlock.h"
#include "VideoBlock.h"
#include "PenBlock.h"
#include "VolumBlock.h"
#include "ControlBlock.h"
#include "ImageBlock.h"
#include "VideoPlayer.h"
#include "Camber.h"
#include "ScatterBubble.h"
#include "BarChartBubble.h"
#include "LineChartBubble.h"
#include "NoteBlock.h"

OpenManager::OpenManager( OpenGLScene* scene, LabObject* lab, LabCut* cut, GroupManager* manager, AllBats* bats ) 
: QObject()
, _batPrefix( tr( TDHelper::getDataPrefix().c_str() ) )
, _pcPrefix( tr( TDHelper::getDataPrefix().c_str() ) )
, _videoPrefix( tr( TDHelper::getDataPrefix().c_str() ) )
, _CAMBER( false )
, _NEW( true )
{
	this->_batPrefix.append( "Bat/" );
	this->_pcPrefix.append( "PC/" );
	this->_videoPrefix.append( "Video/" );
	this->_scene = scene;
	this->_lab = lab;
	this->_cut = cut;
	this->_manager = manager;
	this->_allBats = bats;
	//
	loadName();
	//
	this->_space = new SpaceManager( scene );
	return;
}

void OpenManager::loadName()
{
	std::ifstream file( "data/name.txt" );
	for( int i = 0; i < 6; i++ )
	{
		std::string fullname;
		file>>fullname;
		this->_fullName.append( QObject::tr( fullname.c_str() ) );
	}
	while( !file.eof() )
	{
		std::string filename;
		file>>filename;
		if( filename.length() > 1 )
		{
			this->_fileName.append( QObject::tr( filename.c_str() ) );
		}
	}

	std::ifstream nfra( "data/nFrame.txt" );
	int frame;
	for( int i = 0; i < this->_fileName.size(); i++ )
	{
		nfra>>frame;
		this->_startFrame.append( frame );
	}
	for( int i = 0; i < this->_fileName.size(); i++ )
	{
		nfra>>frame;
		this->_endFrame.append( frame );
	}
	return;
}

ItemBase* OpenManager::openImage( int size, int x, int y, QString path, float rate )
{
	QPoint pos = this->_space->getPosition();
	if( x == 0 )
	{
		x = pos.x();
	}
	if( y == 0 )
	{
		y = pos.y();
	}
	ImageBlock *img = new ImageBlock( x, y, this->_scene, path, rate );
	this->_scene->myAddItem( img );
	this->_scene->update( this->_scene->sceneRect() );
	//
	connect( img, SIGNAL( unGroup(ItemBase*)), this->_manager, SLOT(unGroup( ItemBase*)) );
	connect( img, SIGNAL( itemRemoved( ItemBase*)), this->_manager, SLOT(itemRemoved( ItemBase* )) );
	connect( img, SIGNAL( itemPosMoved( ItemBase*, QPointF)), this->_manager, SLOT(itemPosMoved( ItemBase*, QPointF)) );
	connect( img, SIGNAL( itemPosUpdated(ItemBase*) ), this->_manager, SLOT( itemPosUpdated(ItemBase*)) );
	connect( img, SIGNAL( logInfor( QString ) ), this->_scene, SLOT( logInfor( QString ) ) );
	
	img->logOpen();
	return img;
}

bool OpenManager::openBatByPath( ItemBase* &result, int size, int x, int y, QString name, int _index )
{
	//if( name.size() == 0 )
	//{
	//	return;
	//}
	QString output = this->checkBatByPath( name );
	if( output != "Success" )
	{
		QMessageBox msgBox;
		msgBox.setText( output );
		msgBox.exec();
		return false;
	}
	//********************************************************
	QPoint pos = this->_space->getPosition();
	if( x == 0 )
	{
		x = pos.x();
	}
	if( y == 0 )
	{
		y = pos.y();
	}
	QtBat *bat = new QtBat( size, x, y, this->_scene, name.toStdString() , this->_lab, this->_cut );
	bat->setFrame( float(_index)/bat->getTotalNumberOfFrame(), _index );
	int index = this->_allBats->getIndexFromName( bat->getBat()->getname() );
	bat->setMassAndSpeed( this->_allBats->getMassByIndex( index ), this->_allBats->getSpeedByIndex( index ) );
	this->_scene->myAddItem( bat );
	this->_scene->update( this->_scene->sceneRect() );
	//
	connect( bat, SIGNAL(unGroup(ItemBase*)), this->_manager, SLOT(unGroup( ItemBase*)) );
	connect( bat, SIGNAL(itemRemoved( ItemBase*)), this->_manager, SLOT(itemRemoved( ItemBase* )) );
	connect( bat, SIGNAL(itemPosMoved( ItemBase*, QPointF)), this->_manager, SLOT(itemPosMoved( ItemBase*, QPointF)) );
	connect( bat, SIGNAL( needCompare( QtBat* )), this->_scene, SLOT( addCompare( QtBat* )) );
	connect( bat, SIGNAL( itemPosUpdated(ItemBase*) ), this->_manager, SLOT( itemPosUpdated(ItemBase*)) );
	connect( bat, SIGNAL( updateRotation(ItemBase*, QPointF) ), this->_manager, SLOT(updateSelectBats(ItemBase*, QPointF)) );
	connect( bat, SIGNAL( setCurrentBatAttribute(QtBat*) ), this->_scene, SLOT( setCurrentBatAttribute(QtBat*) ) );
	connect( bat, SIGNAL( getNewBatAttribute( QtBat*) ), this->_scene, SLOT( getNewBatAttribute( QtBat*) ) );
	connect( bat, SIGNAL( updateLabelState( ItemBase*) ), this->_manager, SLOT( updateLabelState( ItemBase* ) ) );
	connect( bat, SIGNAL( logInfor( QString ) ), this->_scene, SLOT( logInfor( QString ) ) );
	//
	bat->logOpen();
	result = bat;
	return true;
}

QString OpenManager::checkBatByPath( QString name )
{
	QString result = QtBat::checkPath( name );
	return result;
}

QString OpenManager::checkPCByPath(QString name)
{
	QString result = QtSM::checkPath( name );
	result.append( SM::checkPath( name ) );
	return result;
}

bool OpenManager::openBatByPathWithoutRegister( ItemBase* &result, int size, int x, int y, QString name, int _index )
{
	QString output = this->checkBatByPath( name );
	if( output != "Success" )
	{
		QMessageBox msgBox;
		msgBox.setText( output );
		msgBox.exec();
		return false;
	}
	//********************************************************************
	QPoint pos = this->_space->getPosition();
	if( x == 0 )
	{
		x = pos.x();
	}
	if( y == 0 )
	{
		y = pos.y();
	}
	QtBat *bat = new QtBat( size, x, y, this->_scene, name.toStdString() , this->_lab, this->_cut );
	bat->setFrame( float(_index)/bat->getTotalNumberOfFrame(), _index );
	bat->setDelegate( true );
	int index = this->_allBats->getIndexFromName( bat->getBat()->getname() );
	bat->setMassAndSpeed( this->_allBats->getMassByIndex( index ), this->_allBats->getSpeedByIndex( index ) );
	//this->_scene->myAddItem( bat );
	this->_scene->addItem( bat );
	this->_scene->update( this->_scene->sceneRect() );
	//
	connect( bat, SIGNAL( unGroup(ItemBase*) ), this->_manager, SLOT(unGroup( ItemBase*)) );
	connect( bat, SIGNAL( itemRemoved( ItemBase*) ), this->_manager, SLOT(itemRemoved( ItemBase* )) );
	connect( bat, SIGNAL( itemPosMoved( ItemBase*, QPointF) ), this->_manager, SLOT(itemPosMoved( ItemBase*, QPointF)) );
	connect( bat, SIGNAL( needCompare( QtBat* ) ), this->_scene, SLOT( addCompare( QtBat* )) );
	connect( bat, SIGNAL( itemPosUpdated(ItemBase*) ), this->_manager, SLOT( itemPosUpdated(ItemBase*)) );
	connect( bat, SIGNAL( setCurrentBatAttribute(QtBat*) ), this->_scene, SLOT( setCurrentBatAttribute(QtBat*) ) );
	connect( bat, SIGNAL( getNewBatAttribute( QtBat*) ), this->_scene, SLOT( getNewBatAttribute( QtBat*) ) );
	connect( bat, SIGNAL( updateLabelState( ItemBase*) ), this->_manager, SLOT( updateLabelState( ItemBase* ) ) );
	connect( bat, SIGNAL( logInfor( QString ) ), this->_scene, SLOT( logInfor( QString ) ) );

	bat->logOpen();

	result = bat;
	return true;
}

bool OpenManager::openPCByPath( ItemBase *&result, int size, int x, int y, QString name)
{
	QString output = checkPCByPath( name );
	if( output != "Success" )
	{
		QMessageBox msgBox;
		msgBox.setText( output );
		msgBox.exec();
		return false;
	}
	//********************************************************************
	QPoint pos = this->_space->getPosition();
	if( x == 0 )
	{
		x = pos.x();
	}
	if( y == 0 )
	{
		y = pos.y();
	}
	QtSM* qtsm;
	qtsm = new QtSM( size, x, y, this->_scene );

	SM* sm = new SM( name.toStdString(), qtsm->Width() );
	sm->size_hint( (float)100/(float)MAX_ITEM_SIZE );
	sm->setColor( this->_cut, this->_lab);
	connect( sm, SIGNAL(swapBars( SM*, int, int)), this->_manager, SLOT(swapSelectPCs( SM*, int, int )) );
	qtsm->setSM( sm );
	connect( qtsm, SIGNAL(unGroup(ItemBase*)), this->_manager, SLOT(unGroup( ItemBase*)) );
	connect( qtsm, SIGNAL(itemRemoved( ItemBase*)), this->_manager, SLOT(itemRemoved( ItemBase* )) );
	connect( qtsm, SIGNAL(itemPosMoved( ItemBase*, QPointF)), this->_manager, SLOT(itemPosMoved( ItemBase*, QPointF)) );
	connect( qtsm, SIGNAL(itemPosUpdated(ItemBase*)), this->_manager, SLOT(itemPosUpdated(ItemBase*)));
	connect( qtsm, SIGNAL( updateLabelState( ItemBase*) ), this->_manager, SLOT( updateLabelState( ItemBase* ) ) );
	connect( qtsm, SIGNAL( updatePCColorState( ItemBase*, int ) ), this->_manager, SLOT( updatePCColorState( ItemBase*, int ) ) );
	connect( qtsm, SIGNAL( logInfor( QString ) ), this->_scene, SLOT( logInfor( QString ) ) );
	connect( qtsm, SIGNAL( HighlightChangeInGroup(ItemBase*, bool) ), this->_manager, SLOT( HighlightChangeinGroup(ItemBase*, bool ) ) );
	connect( qtsm, SIGNAL( pressHightlightInGroup(ItemBase*, QRect) ), this->_manager, SLOT( pressHightlightinGroup(ItemBase*, QRect ) ) );
	connect( qtsm, SIGNAL( moveHightlightInGroup(ItemBase*, QRect) ), this->_manager, SLOT( moveHightlightinGroup(ItemBase*, QRect ) ) );
	connect( qtsm, SIGNAL( releaseHightlightInGroup(ItemBase*) ), this->_manager, SLOT( releaseHightlightinGroup(ItemBase*) ) );
	this->_scene->myAddItem( qtsm );
	qtsm->logOpen();
	result = qtsm;
	return true;
}

ItemBase* OpenManager::openWebView(int size, int x, int y, QString name )
{
	QPoint pos = this->_space->getPosition();
	if( x == 0 )
	{
		x = pos.x();
	}
	if( y == 0 )
	{
		y = pos.y();
	}
	WebBlock* web = new WebBlock( x, y, this->_scene, name );
	web->resizeItem( size, size );
	this->_scene->myAddItem( web );
	connect( web, SIGNAL(unGroup(ItemBase*)), this->_manager, SLOT(unGroup( ItemBase*)) );
	connect( web, SIGNAL(itemRemoved( ItemBase*)), this->_manager, SLOT(itemRemoved( ItemBase* )) );
	connect( web, SIGNAL(itemPosMoved( ItemBase*, QPointF)), this->_manager, SLOT(itemPosMoved( ItemBase*, QPointF)) );
	connect( web, SIGNAL( itemPosUpdated(ItemBase*) ), this->_manager, SLOT( itemPosUpdated(ItemBase*)) );
	connect( web, SIGNAL( updateLabelState( ItemBase*) ), this->_manager, SLOT( updateLabelState( ItemBase* ) ) );
	connect( web, SIGNAL( logInfor( QString ) ), this->_scene, SLOT( logInfor( QString ) ) );

	web->logOpen();

	return web;
}

ItemBase* OpenManager::openPen(int size, int x, int y)
{
	QPoint pos = this->_space->getPosition();
	if( x == 0 )
	{
		x = pos.x();
	}
	if( y == 0 )
	{
		y = pos.y();
	}
	PenBlock* pen = new PenBlock( x, y, this->_scene, this );
	pen->resizeItem( size, size );
	this->_scene->myAddItem( pen );
	connect( pen, SIGNAL(unGroup(ItemBase*)), this->_manager, SLOT(unGroup( ItemBase*)) );
	connect( pen, SIGNAL(itemRemoved( ItemBase*)), this->_manager, SLOT(itemRemoved( ItemBase* )) );
	connect( pen, SIGNAL(itemPosMoved( ItemBase*, QPointF)), this->_manager, SLOT(itemPosMoved( ItemBase*, QPointF)) );
	connect( pen, SIGNAL( itemPosUpdated(ItemBase*) ), this->_manager, SLOT( itemPosUpdated(ItemBase*)) );
	connect( pen, SIGNAL( addBatsToScene(QList<QList<ItemBase*>>)), this->_manager, SLOT(addBatsToScene(QList<QList<ItemBase*>>)));
	connect( pen, SIGNAL( updateLabelState( ItemBase*) ), this->_manager, SLOT( updateLabelState( ItemBase* ) ) );
	connect( pen, SIGNAL( logInfor( QString ) ), this->_scene, SLOT( logInfor( QString ) ) );

	pen->logOpen();

	return pen;
}

QString OpenManager::checkVideoByPath( QString name )
{
	QString result = VideoBlock::checkPath( name );
	return result;
}

bool OpenManager::openVideoByPath( ItemBase* &result, int size, int x, int y, QString name)
{
	QString output = this->checkVideoByPath( name );
	if( output != "Success" )
	{
		QMessageBox msgBox;
		msgBox.setText( output );
		msgBox.exec();
		return false;
	}
	//******************************************************************
	QPoint pos = this->_space->getPosition();
	if( x == 0 )
	{
		x = pos.x();
	}
	if( y == 0 )
	{
		y = pos.y();
	}
	int index = getIndexByToken( name );
	VideoBlock *video = new VideoBlock( x, y, this->_scene, name, _startFrame[index], _endFrame[index] );
	this->_scene->myAddItem( video );

	connect( video->getPlayerWidget(), SIGNAL(needCapture(myVideoPlayer*)), this, SLOT(needCapture(myVideoPlayer*)) );
	connect( video->getPlayerWidget(), SIGNAL( addBatWithVideo() ), video, SLOT( addBatWithVideo() ) );
	connect( video->getPlayerWidget(), SIGNAL( logPause() ), video, SLOT( logPlay() ) );
	connect( video, SIGNAL(unGroup(ItemBase*)), this->_manager, SLOT(unGroup( ItemBase*)) );
	connect( video, SIGNAL(itemRemoved( ItemBase*)), this->_manager, SLOT(itemRemoved( ItemBase* )) );
	connect( video, SIGNAL(itemPosMoved( ItemBase*, QPointF)), this->_manager, SLOT(itemPosMoved( ItemBase*, QPointF)) );
	connect( video, SIGNAL(itemPosUpdated(ItemBase*) ), this->_manager, SLOT( itemPosUpdated(ItemBase*)) );
	connect( video, SIGNAL( updateLabelState( ItemBase*) ), this->_manager, SLOT( updateLabelState( ItemBase* ) ) );
	connect( video, SIGNAL( logInfor( QString ) ), this->_scene, SLOT( logInfor( QString ) ) );
	
	video->logOpen();

	result = video;
	return true;
}

ItemBase* OpenManager::openMatlab(int size, int x, int y)
{
	QPoint pos = this->_space->getPosition();
	if( x == 0 )
	{
		x = pos.x();
	}
	if( y == 0 )
	{
		y = pos.y();
	}
	MatBlock* mat = new MatBlock( x, y, this->_scene );
	this->_scene->myAddItem( mat );
	connect( mat, SIGNAL(unGroup(ItemBase*)), this->_manager, SLOT(unGroup( ItemBase*)) );
	connect( mat, SIGNAL(itemRemoved( ItemBase*)), this->_manager, SLOT(itemRemoved( ItemBase* )) );
	connect( mat, SIGNAL(itemPosMoved( ItemBase*, QPointF)), this->_manager, SLOT(itemPosMoved( ItemBase*, QPointF)) );
	connect( mat, SIGNAL( itemPosUpdated(ItemBase*) ), this->_manager, SLOT( itemPosUpdated(ItemBase*)) );
	connect( mat, SIGNAL( updateLabelState( ItemBase* ) ), this->_manager, SLOT( updateLabelState( ItemBase* ) ) );
	connect( mat, SIGNAL( setMatFileBubble( MatBlock* ) ), this->_scene, SLOT( setMatFileBubble( MatBlock* ) ) );
	connect( mat, SIGNAL( addParameterFromMat( QString , QString , QList<double>) ), this->_scene->getStatisticBar(), SLOT(addParameterFromMat( QString , QString , QList<double> ) ));
	connect( mat, SIGNAL( updateDataFromMat( QString, QList<double>) ), this->_scene->getStatisticBar(), SLOT( updateDataFromMat( QString, QList<double>) ));
	connect( mat, SIGNAL( logInfor( QString ) ), this->_scene, SLOT( logInfor( QString ) ) );
	
	mat->logOpen();
	return mat;
}

ItemBase* OpenManager::openMatlabByPath( int size, int x, int y, QString name )
{
	if ( !QFile::exists(name) )
	{
		return NULL;
	}

	QPoint pos = this->_space->getPosition();
	if( x == 0 )
	{
		x = pos.x();
	}
	if( y == 0 )
	{
		y = pos.y();
	}
	MatBlock *mat = new MatBlock( x, y, this->_scene );
	mat->loadFile( name );
	this->_scene->myAddItem( mat );
	connect( mat, SIGNAL(unGroup(ItemBase*)), this->_manager, SLOT(unGroup( ItemBase*)) );
	connect( mat, SIGNAL(itemRemoved( ItemBase*)), this->_manager, SLOT(itemRemoved( ItemBase* )) );
	connect( mat, SIGNAL(itemPosMoved( ItemBase*, QPointF)), this->_manager, SLOT(itemPosMoved( ItemBase*, QPointF)) );
	connect( mat, SIGNAL( itemPosUpdated(ItemBase*) ), this->_manager, SLOT( itemPosUpdated(ItemBase*)) );
	connect( mat, SIGNAL( updateLabelState( ItemBase*) ), this->_manager, SLOT( updateLabelState( ItemBase* ) ) );
	connect( mat, SIGNAL( setMatFileBubble( MatBlock* ) ), this->_scene, SLOT( setMatFileBubble( MatBlock* ) ) );
	connect( mat, SIGNAL( addParameterFromMat( QString , QString , bool ) ), this->_scene, SLOT(addParameterFromMat( QString , QString , bool ) ));
	connect( mat, SIGNAL( logInfor( QString ) ), this->_scene, SLOT( logInfor( QString ) ) );

	mat->logOpen();

	return mat;
}

ItemBase* OpenManager::openMatlabCopy( int size, int x, int y, MatBlock* matCur )
{
	QPoint pos = this->_space->getPosition();
	if( x == 0 )
	{
		x = pos.x();
	}
	if( y == 0 )
	{
		y = pos.y();
	}
	MatBlock* mat = new MatBlock( x, y, this->_scene );
	this->_scene->myAddItem( mat );
	mat->copyAttribute( matCur );

	connect( mat, SIGNAL(unGroup(ItemBase*)), this->_manager, SLOT(unGroup( ItemBase*)) );
	connect( mat, SIGNAL(itemRemoved( ItemBase*)), this->_manager, SLOT(itemRemoved( ItemBase* )) );
	connect( mat, SIGNAL(itemPosMoved( ItemBase*, QPointF)), this->_manager, SLOT(itemPosMoved( ItemBase*, QPointF)) );
	connect( mat, SIGNAL( itemPosUpdated(ItemBase*) ), this->_manager, SLOT( itemPosUpdated(ItemBase*)) );
	connect( mat, SIGNAL( updateLabelState( ItemBase* ) ), this->_manager, SLOT( updateLabelState( ItemBase* ) ) );
	connect( mat, SIGNAL( setMatFileBubble( MatBlock* ) ), this->_scene, SLOT( setMatFileBubble( MatBlock* ) ) );
	connect( mat, SIGNAL( logInfor( QString ) ), this->_scene, SLOT( logInfor( QString ) ) );

	mat->logOpen();

	return mat;
}

//
//ItemBase* OpenManager::openVTK(int size, int x, int y)
//{
//	QPoint pos = this->_space->getPosition();
//	if( x == 0 )
//	{
//		x = pos.x();
//	}
//	if( y == 0 )
//	{
//		y = pos.y();
//	}
//	VTKBlock *vtk = new VTKBlock( x, y, this->_scene );
//	this->_scene->myAddItem( vtk );
//	connect( vtk, SIGNAL(unGroup(ItemBase*)), this->_manager, SLOT(unGroup( ItemBase*)) );
//	connect( vtk, SIGNAL(itemRemoved( ItemBase*)), this->_manager, SLOT(itemRemoved( ItemBase* )) );
//	connect( vtk, SIGNAL(itemPosMoved( ItemBase*, QPointF)), this->_manager, SLOT(itemPosMoved( ItemBase*, QPointF)) );
//	connect( vtk, SIGNAL( itemPosUpdated(ItemBase*) ), this->_manager, SLOT( itemPosUpdated(ItemBase*)) );
//	connect( vtk, SIGNAL( logInfor( QString ) ), this->_scene, SLOT( logInfor( QString ) ) );	
//	return vtk;
//}

QList<QString> OpenManager::matchFileName( QString token )
{
	QList<QString> names;
	for( int i = 0; i < this->_fileName.size(); i++ )
	{
		if( _fileName[i].contains( token ) )
		{
			names.append( _fileName[i] );
		}
	}
	return names;
}

int OpenManager::getIndexByToken(QString name)
{
	name = extractFullName( name );
	for( int i = 0; i < this->_fileName.size(); i++ )
	{
		if( _fileName[i].contains( name ) )
		{
			return i;
		}
	}
	return -1;
}

QString OpenManager::extractFullName( QString path )
{
	QStringList list = path.split( tr("/") );
	for( int i = 0; i < list.size(); i++ )
	{
		if( list[i].contains("run") )
		{
			return list[i].left( 23 );
		}
	}
}

QString OpenManager::getFullName( QString abber )
{
	QString result;
	for( int i = 0; i < this->_fileName.size(); i++ )
	{
		if( _fileName[i].contains( abber ) )
		{
			if( i >= 0 && i <= 21 )
			{
				result.append( tr("Eidolon helvum") );
			}
			else if( i >= 22 && i <= 35 )
			{
				result.append( tr("Rousettus aegyptiacus") );
			}
			else if( i >= 36 && i <= 53 )
			{
				result.append( tr("Pteropus vampyrus") );
			}
			else if( i >= 54 && i <= 67 )
			{
				result.append( tr("Pteropus hypomelanus") );
			}
			else if( i >= 68 && i <= 79 )
			{
				result.append( tr("Pteropus pumilus") );
			}
			else if( i >= 80 && i <= 103 )
			{
				result.append( tr("Cynopterus brachyotis") );
			}
		}
	}
	return result;
}

QList<QString> OpenManager::getBatPathByToken(QString token)
{
	QList<QString> result;
	QList<QString> names = this->matchFileName( token );
	for( int i = 0; i < names.size(); i++ )
	{
		QString currentPath = this->_batPrefix;
		currentPath.append( names[i] );
		currentPath.append( ".csv" );
		result.append( currentPath );
	}
	return result;
}

QList<QString> OpenManager::getVideoPathByToken( QString name, int type )
{
	QList<QString> result;
	QString currentPath = this->_videoPrefix;
	currentPath.append( name );
	QString QType;
	switch( type )
	{
	case 1:
		currentPath.append( "_camc.avi" );
		break;
	case 2:
		currentPath.append( "_camg.avi" );
		break;
	case 3:
		currentPath.append( "_camh.avi" );
		break;
	};
	result.append( currentPath );
	return result;
}

QList<QString> OpenManager::getPCPathByToken( QString token, int type )
{
	QList<QString> result;
	QString currentPath = this->_pcPrefix;
	currentPath.append( token );
	currentPath.append( tr("_m") );
	QString QType;
	QType.setNum( type - 3 );
	currentPath.append( QType);
	result.append( currentPath );
	return result;
}

QList<ItemBase*> OpenManager::openBatByToken(QString name)
{
	QList<ItemBase*> result;
	QList<QString> names = this->getBatPathByToken( name );
	for( int i = 0; i < names.size(); i++ )
	{
		ItemBase* pointer;
		if( this->openBatByPath( pointer, 150, i * 200 + 200, 200, names[i] ) )
		{
			result.append( pointer );
		}
	}

	return result;
}

bool OpenManager::findBatByToken( QString name, DESCRIPTOR dis, int number, int index, QList<ItemBase*> &bats, QList<float> &rates )
{
	ItemBase* item;
	bats.clear();
	rates.clear();
	QList<QString> names = this->matchFileName( name );
	QList<CurveMatch> matches = this->getMatchBatPath( names, dis, number, index, rates );
	QVector<QRectF> rects = this->_space->queryPreOpen( QPointF( 0, 0 ), matches.size() );
	for( int i = 0; i < matches.size(); i++ )
	{
		if( this->openBatByPathWithoutRegister( item, 200, rects[i].center().x() + 200, rects[i].center().y() + 200, matches[i].name, matches[i].index ) )
		{
			bats.append( item );
		}
	}
	return true;
}


QList<CurveMatch> OpenManager::getMatchBatPath( QList<QString> names, DESCRIPTOR dis, int number, int index, QList<float> &rates )
{
	QList<CurveMatch> result;
	QList<QString> camPath = getCamberPath( names, index );
	QList<int> indexs;
	float rate;
	int _index, which;
	for( int i = 0; i < names.size(); i++ )
	{
		Camber* camber = new Camber( camPath[i].toStdString() );
		if( !_CAMBER )
		{
			this->_Camber = camber;
			this->_CAMBER = true;
		}
		camber->findMatchDiscreptor( dis, index, rate, _index );
		rates.append( rate );
		indexs.append( _index );
		delete camber;
	}
	int TouchSize = 0;
	if( number > names.size() )
	{
		TouchSize = names.size();
	}
	else
	{
		TouchSize = number;
	}
	QList<float> order = rates;
	QList<float> tempRateResult;
	for( int i = 0; i < TouchSize; i++ )
	{
		this->getIndexsByMaxRate( order, indexs, which, index );
		CurveMatch current;
		current.index = index;
		current.name = getBatPathByToken( names[which] )[0];
		result.append( current );
		tempRateResult.append( rates[which] );
	}
	rates = tempRateResult;
	return result;
}

void OpenManager::getIndexsByMaxRate(QList<float> &rates, QList<int> indexs, int &which, int &index)
{
	float MIN = 9998;
	for( int i = 0; i < rates.size(); i++ )
	{
		if( MIN > rates[i] )
		{
			MIN = rates[i];
			which = i;
			index = indexs[i];
		}
	}
	rates.replace( which, 9999);
	return;
}

void OpenManager::getIndexsByMinRate(QList<float> &rates, QList<int> indexs, int &which, int &index)
{
	float MIN = 9998;
	for( int i = 0; i < rates.size(); i++ )
	{
		if( MIN > rates[i] )
		{
			MIN = rates[i];
			which = i;
			index = indexs[i];
		}
	}
	rates.replace( which, 9999);
	return;
}

QList<QString> OpenManager::getCamberPath(QList<QString> names, int number)
{
	QList<QString> result;
	for( int i = 0; i < names.size(); i++ )
	{
		QString currentPath = _batPrefix;
		//currentPath.append( "\\Bat" );
		currentPath.append( names[i] );
		currentPath.append( "/" );
		//
		result.append( currentPath );
	}
	return result;
}

bool OpenManager::getCAMBER()
{
	return _CAMBER;
}
Camber* OpenManager::getCurrentCamber()
{
	return this->_Camber;
}

void OpenManager::needCapture( myVideoPlayer* video )
{
	QString name = video->getName();
	float rate = video->getRate();
	//			create a image bubble
	name.remove( name.length() - 3, 3 );
	name.append( tr("gif") );
	this->openImage( 0, 200, 200, name, rate );
	return;
}

int OpenManager::queryPossibleOpen(QString name)
{
	return matchFileName( name ).size();
}

QVector<QRectF> OpenManager::queryPreOpen( QPointF pos, int num )
{
	return this->_space->queryPreOpen( pos, num );
}

QVector<QRectF> OpenManager::queryPreOpen( QPointF pos, int row, int col )
{
	return this->_space->queryPreOpen( pos, row, col );
}

void OpenManager::openDataInViewAt(QString name, QList<int> views, QPointF pos)
{
	QList<QString> names = matchFileName( name );
	QVector<QRectF> rect;
	ItemBase *tmp, *first;
	int i, j, index;
	QPointF tPos;

	if (names.size() <= views.size())
	{
		for ( i = 0; i < names.size(); i ++)
		{
			tPos.setX( pos.x() );
			tPos.setY( pos.y() + STEP*i );
			rect = queryPreOpen( tPos, views.size() );
			index = 0;

			for ( j = 0; j < views.size(); j ++ )
			{
				tmp = this->openSpecifiedViewByName( names[i], views[j], rect[index].center() );
				index += 1;

				if ( i == 0 && j == 0)
				{
					first = tmp;
				}
				else
				{
					this->_manager->addToGroup( tmp, first );
				}
			}
		}
	}
	else
	{
		for ( i = 0; i < views.size(); i ++)
		{
			tPos.setX( pos.x() );
			tPos.setY( pos.y() + STEP*i );
			rect = queryPreOpen( tPos, names.size() );
			index = 0;

			for ( j = 0; j < names.size(); j ++ )
			{
				tmp = this->openSpecifiedViewByName( names[j], views[i], rect[index].center() );
				index += 1;

				if ( i == 0 && j == 0)
				{
					first = tmp;
				}
				else
				{
					this->_manager->addToGroup( tmp, first );
				}
			}
		}
	}
	
	return;
}

ItemBase* OpenManager::openSpecifiedViewByName( QString name, int view, QPointF pos )
{
	//			3D vis view
	ItemBase* item;
	item = NULL;

	if( view == 0 )
	{
		QList<QString> paths = getBatPathByToken( name );
		if( paths.size() > 0 )
		{
			this->openBatByPath( item, 200, pos.x(), pos.y(), paths[0] );
		}
	}
	else if( view == 4 ) // PC 1
	{
		QList<QString> paths = getPCPathByToken( name, view );
		if( paths.size() > 0 )
		{
			this->openPCByPath( item, 200, pos.x(), pos.y(), paths[0] );
		}
	}
	else if( view == 5 ) // PC 2
	{
		QList<QString> paths = getPCPathByToken( name, view );
		if( paths.size() > 0 )
		{
			this->openPCByPath( item, 200, pos.x(), pos.y(), paths[0] );
		}
	}
	else if( view == 6 ) // PC 3
	{
		QList<QString> paths = getPCPathByToken( name, view );
		if( paths.size() > 0 )
		{
			this->openPCByPath( item, 200, pos.x(), pos.y(), paths[0] );
		}
	}
	else if( view == 1 ) // video 1
	{
		QList<QString> paths = getVideoPathByToken( name, view );
		if( paths.size() > 0 )
		{
			this->openVideoByPath( item, 200, pos.x(), pos.y(), paths[0] );
		}
	}
	else if( view == 2 ) // video 2
	{
		QList<QString> paths = getVideoPathByToken( name, view );
		if( paths.size() > 0 )
		{
			this->openVideoByPath( item, 200, pos.x(), pos.y(), paths[0] );
		}
	}
	else if( view == 3 ) // video 3
	{
		QList<QString> paths = getVideoPathByToken( name, view );
		if( paths.size() > 0 )
		{
			this->openVideoByPath( item, 200, pos.x(), pos.y(), paths[0] );
		}
	}
	else if( view == 7 ) // bar chart
	{
		item = this->openBarChart( 200, pos.x(), pos.y(), name );
	}
	else if ( view == 8 ) // line chart
	{
		item = this->openLineChart( 200, pos.x(), pos.y(), name );
	}
	else if ( view == 9 ) // scatter plot
	{
		item = this->openScatterPlot( 200, pos.x(), pos.y(), name );
	}

	return item;
}

void OpenManager::changeView( QPointF pos, int view )
{
	QList<ItemBase*> itemList;
	ItemBase* item;
	QPointF prepos, tmp;
	QString name;
	int i;
	itemList = this->_manager->allMembers();

	for (i = 0; i < itemList.size(); i ++)
	{
		item = itemList[i];

		if (item->getType() != BAT && item->getType() != PC && item->getType() != VIDEO)
		{
			continue;
		}

		tmp = pos - item->pos();

		if (item->boundingRect().contains(tmp))
		{
			break;
		}
	}

	if (i == itemList.size())
	{
		return;
	}

	prepos = item->pos();
	if (item->getType() == BAT)
	{
		QtBat* bat = dynamic_cast<QtBat*>(item);
		name = bat->getBatName();
	}
	else if (item->getType() == PC)
	{
		QtSM* sm = dynamic_cast<QtSM*>(item);
		name = sm->getName();
		name = getNameFromPath(name);
	}
	else if (item->getType() == VIDEO)
	{
		VideoBlock* videobubble = dynamic_cast<VideoBlock*>(item);
		name = videobubble->getPath();
		name = getNameFromPath(name);
	}

	if (this->_NEW)
	{
		item->properRemove();
		delete item;
	}

	this->openSpecifiedViewByName(name, view, prepos);
}

bool OpenManager::changeFile(QPointF pos, QString name)
{
	QList<ItemBase*> itemList;
	ItemBase* item;
	QPointF prepos, tmp;
	QString path;
	int view, i;
	bool success = false;

	itemList = this->_manager->allMembers();

	for (i = 0; i < itemList.size(); i ++)
	{
		item = itemList[i];

		if (item->getType() != BAT && item->getType() != PC && item->getType() != VIDEO)
		{
			continue;
		}

		tmp = pos - item->pos();

		if (item->boundingRect().contains(tmp))
		{
			break;
		}
	}

	if (i == itemList.size())
	{
		return false;
	}

	prepos = item->LeftTop();
	if (item->getType() == BAT)
	{
		view = 0;

		success = true;
	}
	else if (item->getType() == PC)
	{
		QtSM *sm = dynamic_cast<QtSM*>(item);
		path = sm->getName();

		int tindex = path.lastIndexOf("_");
		if (path.at(tindex+2) == '1')
		{
			view = 4;
		}
		else if (path.at(tindex+2) == '2')
		{
			view = 5;
		}
		else if (path.at(tindex+2) == '3')
		{
			view = 6;
		}

		success = true;
	}
	else if (item->getType() == VIDEO)
	{
		VideoBlock* videobubble = dynamic_cast<VideoBlock*>(item);
		path = videobubble->getPath();

		if (path.lastIndexOf("_camc") > -1)
		{
			view = 1;
		}
		else if (path.lastIndexOf("_camg") > -1)
		{
			view = 2;
		}
		else if (path.lastIndexOf("_camh") > -1)
		{
			view = 3;
		}

		success = true;
	}

	if (this->_NEW)
	{
		item->properRemove();
		delete item;
	}

	QList<int> viewList;
	viewList.push_back(view);

	this->openDataInViewAt(name, viewList, prepos);

	return success;
}

ItemBase* OpenManager::openParallelCoordinate( int size, int x, int y, QString name )
{
	ItemBase* item=NULL;

	return item;
}

ItemBase* OpenManager::openScatterPlot( int size, int x, int y, QString name )
{
	QPoint pos = this->_space->getPosition();
	if( x == 0 )
	{
		x = pos.x();
	}
	if( y == 0 )
	{
		y = pos.y();
	}

	ScatterBubble* scatter = new ScatterBubble( size, x, y, this->_scene, this->_scene->getStaManager(), name );
	this->_scene->myAddItem( scatter );

	connect( scatter, SIGNAL(unGroup(ItemBase*)), this->_manager, SLOT(unGroup( ItemBase*)) );
	connect( scatter, SIGNAL(itemRemoved( ItemBase*)), this->_manager, SLOT(itemRemoved( ItemBase* )) );
	connect( scatter, SIGNAL(itemPosMoved( ItemBase*, QPointF)), this->_manager, SLOT(itemPosMoved( ItemBase*, QPointF)) );
	connect( scatter, SIGNAL( itemPosUpdated(ItemBase*) ), this->_manager, SLOT( itemPosUpdated(ItemBase*)) );
	connect( scatter, SIGNAL( updateLabelState( ItemBase*) ), this->_manager, SLOT( updateLabelState( ItemBase* ) ) );
	connect( scatter, SIGNAL( logInfor( QString ) ), this->_scene, SLOT( logInfor( QString ) ) );

	scatter->logOpen();

	return scatter;
}

ItemBase* OpenManager::openBarChart( int size, int x, int y, QString name)
{
	QPoint pos = this->_space->getPosition();
	if( x == 0 )
	{
		x = pos.x();
	}
	if( y == 0 )
	{
		y = pos.y();
	}

	BarChartBubble* barChart = new BarChartBubble( size, x, y, this->_scene, this->_scene->getStaManager(), name );
	this->_scene->myAddItem( barChart );

	connect( barChart, SIGNAL(unGroup(ItemBase*)), this->_manager, SLOT(unGroup( ItemBase*)) );
	connect( barChart, SIGNAL(itemRemoved( ItemBase*)), this->_manager, SLOT(itemRemoved( ItemBase* )) );
	connect( barChart, SIGNAL(itemPosMoved( ItemBase*, QPointF)), this->_manager, SLOT(itemPosMoved( ItemBase*, QPointF)) );
	connect( barChart, SIGNAL( itemPosUpdated(ItemBase*) ), this->_manager, SLOT( itemPosUpdated(ItemBase*)) );
	connect( barChart, SIGNAL( updateLabelState( ItemBase*) ), this->_manager, SLOT( updateLabelState( ItemBase* ) ) );
	connect( barChart, SIGNAL( logInfor( QString ) ), this->_scene, SLOT( logInfor( QString ) ) );

	barChart->logOpen();

	return barChart;
}

ItemBase* OpenManager::openLineChart( int size, int x, int y, QString name )
{
	QPoint pos = this->_space->getPosition();
	if( x == 0 )
	{
		x = pos.x();
	}
	if( y == 0 )
	{
		y = pos.y();
	}

	LineChartBubble* lineChart = new LineChartBubble( size, x, y, this->_scene, this->_scene->getStaManager(), name );
	this->_scene->myAddItem( lineChart );

	connect( lineChart, SIGNAL(unGroup(ItemBase*)), this->_manager, SLOT(unGroup( ItemBase*)) );
	connect( lineChart, SIGNAL(itemRemoved( ItemBase*)), this->_manager, SLOT(itemRemoved( ItemBase* )) );
	connect( lineChart, SIGNAL(itemPosMoved( ItemBase*, QPointF)), this->_manager, SLOT(itemPosMoved( ItemBase*, QPointF)) );
	connect( lineChart, SIGNAL( itemPosUpdated(ItemBase*) ), this->_manager, SLOT( itemPosUpdated(ItemBase*)) );
	connect( lineChart, SIGNAL( updateLabelState( ItemBase*) ), this->_manager, SLOT( updateLabelState( ItemBase* ) ) );
	connect( lineChart, SIGNAL( logInfor( QString ) ), this->_scene, SLOT( logInfor( QString ) ) );

	lineChart->logOpen();

	return lineChart;
} 

ItemBase* OpenManager::openNote( int size, int x, int y )
{
	QPoint pos = this->_space->getPosition();
	if( x == 0 )
	{
		x = pos.x();
	}
	if( y == 0 )
	{
		y = pos.y();
	}

	NoteBlock* noteBlock = new NoteBlock( size, x, y, this->_scene );
	this->_scene->myAddItem( noteBlock );

	connect( noteBlock, SIGNAL(unGroup(ItemBase*)), this->_manager, SLOT(unGroup( ItemBase*)) );
	connect( noteBlock, SIGNAL(itemRemoved( ItemBase*)), this->_manager, SLOT(itemRemoved( ItemBase* )) );
	connect( noteBlock, SIGNAL(itemPosMoved( ItemBase*, QPointF)), this->_manager, SLOT(itemPosMoved( ItemBase*, QPointF)) );
	connect( noteBlock, SIGNAL( itemPosUpdated(ItemBase*) ), this->_manager, SLOT( itemPosUpdated(ItemBase*)) );
	connect( noteBlock, SIGNAL( logInfor( QString ) ), this->_scene, SLOT( logInfor( QString ) ) );

	noteBlock->logOpen();

	return noteBlock;
}

ItemBase* OpenManager::openBatFromXmlNode( QDomElement node )
{
	QDomNodeList paraNode;

	paraNode = node.elementsByTagName( tr("Size") );
	QStringList sizeStr= paraNode.at(0).toElement().text().split( "," );
	QString str = sizeStr.at(0);
	str.remove( 0, 1 );
	str = str.simplified();
	int sizewidth = str.toInt();
	str = sizeStr.at(1);
	str.chop(1);
	str = str.simplified();
	int sizeheight  = str.toInt();

	paraNode = node.elementsByTagName( tr("Position") );
	QStringList posStr= paraNode.at(0).toElement().text().split( "," );
	str = posStr.at(0);
	str.remove( 0, 1 );
	str = str.simplified();
	float x = str.toFloat();
	str = posStr.at(1);
	str.chop(1);
	str = str.simplified();
	float y  = str.toFloat();

	paraNode = node.elementsByTagName( tr("FileName") );
	str = paraNode.at(0).toElement().text();
	QString name = _batPrefix + str + ".csv";

	ItemBase *item;

	if ( openBatByPath( item, sizewidth, x, y, name ) )
	{
		item->resizeItem( sizewidth, sizeheight );
		item->recoverFromXmlNode( node );
		return item;
	}
	return NULL;
}

ItemBase* OpenManager::openPCFromXmlNode( QDomElement node )
{
	QDomNodeList paraNode;

	paraNode = node.elementsByTagName( tr("Size") );
	QStringList sizeStr= paraNode.at(0).toElement().text().split( "," );
	QString str = sizeStr.at(0);
	str.remove( 0, 1 );
	str = str.simplified();
	int sizewidth = str.toInt();
	str = sizeStr.at(1);
	str.chop(1);
	str = str.simplified();
	int sizeheight  = str.toInt();

	paraNode = node.elementsByTagName( tr("Position") );
	QStringList posStr= paraNode.at(0).toElement().text().split( "," );
	str = posStr.at(0);
	str.remove( 0, 1 );
	str = str.simplified();
	float x = str.toFloat();
	str = posStr.at(1);
	str.chop(1);
	str = str.simplified();
	float y  = str.toFloat();

	paraNode = node.elementsByTagName( tr("FileName") );
	str = paraNode.at(0).toElement().text();
	QString name = _pcPrefix + str;

	ItemBase *item;
	if ( openPCByPath( item, sizewidth, x, y, name ) )
	{
		item->resizeItem( sizewidth, sizeheight );
		item->recoverFromXmlNode( node );
		return item;
	}
	return NULL;
}

ItemBase* OpenManager::openVideoFromXmlNode( QDomElement node )
{
	QDomNodeList paraNode;

	paraNode = node.elementsByTagName( tr("Size") );
	QStringList sizeStr= paraNode.at(0).toElement().text().split( "," );
	QString str = sizeStr.at(0);
	str.remove( 0, 1 );
	str = str.simplified();
	int sizewidth = str.toInt();
	str = sizeStr.at(1);
	str.chop(1);
	str = str.simplified();
	int sizeheight  = str.toInt();

	paraNode = node.elementsByTagName( tr("Position") );
	QStringList posStr= paraNode.at(0).toElement().text().split( "," );
	str = posStr.at(0);
	str.remove( 0, 1 );
	str = str.simplified();
	float x = str.toFloat();
	str = posStr.at(1);
	str.chop(1);
	str = str.simplified();
	float y  = str.toFloat();

	paraNode = node.elementsByTagName( tr("Path") );
	str = paraNode.at(0).toElement().text();
	QString name = str;

	ItemBase *item;
	if ( openVideoByPath( item, sizewidth, x, y, name ) )
	{
		item->resizeItem( sizewidth, sizeheight );
		item->recoverFromXmlNode( node );
		return item;
	}
	return NULL;
}

ItemBase* OpenManager::openWebFromXmlNode( QDomElement node )
{
	QDomNodeList paraNode;

	paraNode = node.elementsByTagName( tr("Size") );
	QStringList sizeStr= paraNode.at(0).toElement().text().split( "," );
	QString str = sizeStr.at(0);
	str.remove( 0, 1 );
	str = str.simplified();
	int sizewidth = str.toInt();
	str = sizeStr.at(1);
	str.chop(1);
	str = str.simplified();
	int sizeheight  = str.toInt();

	paraNode = node.elementsByTagName( tr("Position") );
	QStringList posStr= paraNode.at(0).toElement().text().split( "," );
	str = posStr.at(0);
	str.remove( 0, 1 );
	str = str.simplified();
	float x = str.toFloat();
	str = posStr.at(1);
	str.chop(1);
	str = str.simplified();
	float y  = str.toFloat();

	paraNode = node.elementsByTagName( tr("Path") );
	str = paraNode.at(0).toElement().text();
	QString name = str;

	ItemBase *item;
	item = openWebView( sizewidth, x, y, name );
	if ( item )
	{
		item->resizeItem( sizewidth, sizeheight );
		item->recoverFromXmlNode( node );
		return item;
	}
	return NULL;
}

ItemBase* OpenManager::openBarChartFromXmlNode( QDomElement node )
{
	QDomNodeList paraNode;

	paraNode = node.elementsByTagName( tr("Size") );
	QStringList sizeStr= paraNode.at(0).toElement().text().split( "," );
	QString str = sizeStr.at(0);
	str.remove( 0, 1 );
	str = str.simplified();
	int sizewidth = str.toInt();
	str = sizeStr.at(1);
	str.chop(1);
	str = str.simplified();
	int sizeheight  = str.toInt();

	paraNode = node.elementsByTagName( tr("Position") );
	QStringList posStr= paraNode.at(0).toElement().text().split( "," );
	str = posStr.at(0);
	str.remove( 0, 1 );
	str = str.simplified();
	float x = str.toFloat();
	str = posStr.at(1);
	str.chop(1);
	str = str.simplified();
	float y  = str.toFloat();

	paraNode = node.elementsByTagName( tr("BatName") );
	str = paraNode.at(0).toElement().text();
	QString name = str;

	ItemBase *item;

	item = openBarChart( sizewidth, x, y, name );
	if ( item )
	{
		item->resizeItem( sizewidth, sizeheight );
		item->recoverFromXmlNode( node );
		return item;
	}
	return NULL;
}

ItemBase* OpenManager::openLineChartFromXmlNode( QDomElement node )
{
	QDomNodeList paraNode;

	paraNode = node.elementsByTagName( tr("Size") );
	QStringList sizeStr= paraNode.at(0).toElement().text().split( "," );
	QString str = sizeStr.at(0);
	str.remove( 0, 1 );
	str = str.simplified();
	int sizewidth = str.toInt();
	str = sizeStr.at(1);
	str.chop(1);
	str = str.simplified();
	int sizeheight  = str.toInt();

	paraNode = node.elementsByTagName( tr("Position") );
	QStringList posStr= paraNode.at(0).toElement().text().split( "," );
	str = posStr.at(0);
	str.remove( 0, 1 );
	str = str.simplified();
	float x = str.toFloat();
	str = posStr.at(1);
	str.chop(1);
	str = str.simplified();
	float y  = str.toFloat();

	paraNode = node.elementsByTagName( tr("BatName") );
	str = paraNode.at(0).toElement().text();
	QString name = str;

	ItemBase *item;
	
	item = openLineChart( sizewidth, x, y, name );
	if ( item )
	{
		item->resizeItem( sizewidth, sizeheight );
		item->recoverFromXmlNode( node );
		return item;
	}
	return NULL;
}

ItemBase* OpenManager::openScatterFromXmlNode( QDomElement node )
{
	QDomNodeList paraNode;

	paraNode = node.elementsByTagName( tr("Size") );
	QStringList sizeStr= paraNode.at(0).toElement().text().split( "," );
	QString str = sizeStr.at(0);
	str.remove( 0, 1 );
	str = str.simplified();
	int sizewidth = str.toInt();
	str = sizeStr.at(1);
	str.chop(1);
	str = str.simplified();
	int sizeheight  = str.toInt();

	paraNode = node.elementsByTagName( tr("Position") );
	QStringList posStr= paraNode.at(0).toElement().text().split( "," );
	str = posStr.at(0);
	str.remove( 0, 1 );
	str = str.simplified();
	float x = str.toFloat();
	str = posStr.at(1);
	str.chop(1);
	str = str.simplified();
	float y  = str.toFloat();

	paraNode = node.elementsByTagName( tr("BatName") );
	str = paraNode.at(0).toElement().text();
	QString name = str;

	ItemBase *item;
	item = openScatterPlot( sizewidth, x, y, name );
	if ( item )
	{
		item->resizeItem( sizewidth, sizeheight );
		item->recoverFromXmlNode( node );
		return item;
	}
	return NULL;
}

ItemBase* OpenManager::openMatlabFromXmlNode( QDomElement node )
{
	QDomNodeList paraNode;

	paraNode = node.elementsByTagName( tr("Size") );
	QStringList sizeStr= paraNode.at(0).toElement().text().split( "," );
	QString str = sizeStr.at(0);
	str.remove( 0, 1 );
	str = str.simplified();
	int sizewidth = str.toInt();
	str = sizeStr.at(1);
	str.chop(1);
	str = str.simplified();
	int sizeheight  = str.toInt();

	paraNode = node.elementsByTagName( tr("Position") );
	QStringList posStr= paraNode.at(0).toElement().text().split( "," );
	str = posStr.at(0);
	str.remove( 0, 1 );
	str = str.simplified();
	float x = str.toFloat();
	str = posStr.at(1);
	str.chop(1);
	str = str.simplified();
	float y  = str.toFloat();

	ItemBase *item;
	item = openMatlab(sizewidth, x, y);
	
	if (item)
	{
		item->resizeItem( sizewidth, sizeheight );
		
		item->recoverFromXmlNode( node );
		return item;
	}

	return NULL;
}

ItemBase* OpenManager::openNoteFromXmlNode( QDomElement node )
{
	QDomNodeList paraNode;

	paraNode = node.elementsByTagName( tr("Size") );
	QStringList sizeStr= paraNode.at(0).toElement().text().split( "," );
	QString str = sizeStr.at(0);
	str.remove( 0, 1 );
	str = str.simplified();
	int sizewidth = str.toInt();
	str = sizeStr.at(1);
	str.chop(1);
	str = str.simplified();
	int sizeheight  = str.toInt();

	paraNode = node.elementsByTagName( tr("Position") );
	QStringList posStr= paraNode.at(0).toElement().text().split( "," );
	str = posStr.at(0);
	str.remove( 0, 1 );
	str = str.simplified();
	float x = str.toFloat();
	str = posStr.at(1);
	str.chop(1);
	str = str.simplified();
	float y  = str.toFloat();

	ItemBase *item;
	item = openNote(sizewidth, x, y);

	if (item)
	{
		item->resizeItem( sizewidth, sizeheight );
		item->recoverFromXmlNode( node );
		return item;
	}

	return NULL;
}