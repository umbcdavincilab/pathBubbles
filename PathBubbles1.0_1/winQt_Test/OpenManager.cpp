#include "3DHelper.h"
//#include "OpenManager.h"
#include "openglscene.h"
//#include "qtbat.h"
#include "SM.h"
#include "qtsm.h"
#include "LabCut.h"
#include "LabObject.h"
//#include "AllBats.h"
#include "GroupManager.h"
#include "SearchFind.h"
#include "webQuery.h"
//#include "matBlock.h"
#include "webBlock.h"
#include "VideoBlock.h"
//#include "PenBlock.h"
#include "VolumBlock.h"
#include "ControlBlock.h"
#include "ImageBlock.h"
#include "VideoPlayer.h"
#include "Camber.h"
#include "ScatterBubble.h"
#include "BarChartBubble.h"
#include "LineChartBubble.h"
#include "PathBubble0.h"
#include "PathBubble1.h"
#include "treeRingBubble.h"
#include "subPathBubble1.h"
//#include "PathBubble2.h"
#include "textBubble2.h"
#include "reactionBubble3.h"
#include "GroupBubble4.h"
#include "NoteBlock.h"
#include "CalloutNote.h"
#include "expressionBubble.h"

#include "pathwayDataParser.h"

OpenManager::OpenManager( OpenGLScene* scene, LabObject* lab, LabCut* cut, GroupManager* manager) 
: QObject()
, _batPrefix( tr( TDHelper::getDataPrefix().c_str() ) )
, _pcPrefix( tr( TDHelper::getDataPrefix().c_str() ) )
, _videoPrefix( tr( TDHelper::getDataPrefix().c_str() ) )
//, _CAMBER( false )
, _NEW( true )
{
	this->_batPrefix.append( "Bat/" );
	this->_pcPrefix.append( "PC/" );
	this->_videoPrefix.append( "Video/" );
	this->_scene = scene;
	this->_lab = lab;
	this->_cut = cut;
	this->_manager = manager;
	//this->_allBats = bats;
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
	connect( img, SIGNAL(itemResized( ItemBase*, QRectF, int)), this->_manager, SLOT(bubbleResized( ItemBase*, QRectF, int) ) );

	img->logOpen();
	return img;
}

ItemBase* OpenManager::openTreeRingBubble(int sizex, int x, int y)
{	
    QPointF pos; 
	treeRingBubble* TreeRingBubble = new treeRingBubble(sizex, x, y, this->_scene, this->_scene->getStaManager(), this, "path");
	this->_scene->myAddItem( TreeRingBubble );
	this->_scene->m_treeRingBubbles.push_back(TreeRingBubble);	
	//this->_scene->_expressionBubbles.push_back(TreeRingBubble);

	connect( TreeRingBubble, SIGNAL(unGroup(ItemBase*)), this->_manager, SLOT(unGroup( ItemBase*)) );
	connect( TreeRingBubble, SIGNAL(itemRemoved( ItemBase*)), this->_manager, SLOT(itemRemoved( ItemBase* )) );
	connect( TreeRingBubble, SIGNAL(itemPosMoved( ItemBase*, QPointF)), this->_manager, SLOT(itemPosMoved( ItemBase*, QPointF)) );
	connect( TreeRingBubble, SIGNAL( itemPosUpdated(ItemBase*) ), this->_manager, SLOT( itemPosUpdated(ItemBase*)) );
	connect( TreeRingBubble, SIGNAL( updateLabelState( ItemBase*) ), this->_manager, SLOT( updateLabelState( ItemBase* ) ) );
	connect( TreeRingBubble, SIGNAL( logInfor( QString ) ), this->_scene, SLOT( logInfor( QString ) ) );

	connect( TreeRingBubble, SIGNAL( logInfor( QString ) ), this->_scene, SLOT( logInfor( QString ) ) );
	connect( TreeRingBubble, SIGNAL(itemResized( ItemBase*, QRectF, int)), this->_manager, SLOT(bubbleResized( ItemBase*, QRectF, int) ) );

	TreeRingBubble->logOpen();	
	return TreeRingBubble;
}


/*ItemBase* OpenManager::openTreeRingBubble(int sizex, int x, int y)
{	
    QPointF pos; 
	treeRingBubble* TreeRingBubble = new treeRingBubble(sizex, x, y, this->_scene, this->_scene->getStaManager(), this, "path");
	this->_scene->myAddItem( TreeRingBubble );
	//this->_scene->_expressionBubbles.push_back(TreeRingBubble);

	connect( TreeRingBubble, SIGNAL(unGroup(ItemBase*)), this->_manager, SLOT(unGroup( ItemBase*)) );
	connect( TreeRingBubble, SIGNAL(itemRemoved( ItemBase*)), this->_manager, SLOT(itemRemoved( ItemBase* )) );
	connect( TreeRingBubble, SIGNAL(itemPosMoved( ItemBase*, QPointF)), this->_manager, SLOT(itemPosMoved( ItemBase*, QPointF)) );
	connect( TreeRingBubble, SIGNAL( itemPosUpdated(ItemBase*) ), this->_manager, SLOT( itemPosUpdated(ItemBase*)) );
	connect( TreeRingBubble, SIGNAL( updateLabelState( ItemBase*) ), this->_manager, SLOT( updateLabelState( ItemBase* ) ) );
	connect( TreeRingBubble, SIGNAL( logInfor( QString ) ), this->_scene, SLOT( logInfor( QString ) ) );

	connect( TreeRingBubble, SIGNAL( logInfor( QString ) ), this->_scene, SLOT( logInfor( QString ) ) );
	connect( TreeRingBubble, SIGNAL(itemResized( ItemBase*, QRectF, int)), this->_manager, SLOT(bubbleResized( ItemBase*, QRectF, int) ) );

	TreeRingBubble->logOpen();	
	return TreeRingBubble;
}
*/

ItemBase* OpenManager::openExpressionBubble(int size, int x, int y, QString name)
{

    QPointF pos;// = this->_space->getPosition();
	
		
	expressionBubble* ExpressionBubble = new expressionBubble( size, x, y, this->_scene, this->_scene->getStaManager(), this, name );
	this->_scene->myAddItem( ExpressionBubble );
	this->_scene->_expressionBubbles.push_back(ExpressionBubble);

	connect( ExpressionBubble, SIGNAL(unGroup(ItemBase*)), this->_manager, SLOT(unGroup( ItemBase*)) );
	connect( ExpressionBubble, SIGNAL(itemRemoved( ItemBase*)), this->_manager, SLOT(itemRemoved( ItemBase* )) );
	connect( ExpressionBubble, SIGNAL(itemPosMoved( ItemBase*, QPointF)), this->_manager, SLOT(itemPosMoved( ItemBase*, QPointF)) );
	connect( ExpressionBubble, SIGNAL( itemPosUpdated(ItemBase*) ), this->_manager, SLOT( itemPosUpdated(ItemBase*)) );
	connect( ExpressionBubble, SIGNAL( updateLabelState( ItemBase*) ), this->_manager, SLOT( updateLabelState( ItemBase* ) ) );
	connect( ExpressionBubble, SIGNAL( logInfor( QString ) ), this->_scene, SLOT( logInfor( QString ) ) );

	connect( ExpressionBubble, SIGNAL( logInfor( QString ) ), this->_scene, SLOT( logInfor( QString ) ) );
	connect( ExpressionBubble, SIGNAL(itemResized( ItemBase*, QRectF, int)), this->_manager, SLOT(bubbleResized( ItemBase*, QRectF, int) ) );

	ExpressionBubble->logOpen();
	//this->_manager->newBubbleCreated(ExpressionBubble);	
	return ExpressionBubble;

}


ItemBase* OpenManager::openPathBubble0( int size, int x, int y, QString name)
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
		
	PathBubble0* pathBubble0 = new PathBubble0( size, x, y, this->_scene, this->_scene->getStaManager(), this, name );
	this->_scene->myAddItem( pathBubble0 );

	connect( pathBubble0, SIGNAL(unGroup(ItemBase*)), this->_manager, SLOT(unGroup( ItemBase*)) );
	connect( pathBubble0, SIGNAL(itemRemoved( ItemBase*)), this->_manager, SLOT(itemRemoved( ItemBase* )) );
	connect( pathBubble0, SIGNAL(itemPosMoved( ItemBase*, QPointF)), this->_manager, SLOT(itemPosMoved( ItemBase*, QPointF)) );
	connect( pathBubble0, SIGNAL( itemPosUpdated(ItemBase*) ), this->_manager, SLOT( itemPosUpdated(ItemBase*)) );
	connect( pathBubble0, SIGNAL( updateLabelState( ItemBase*) ), this->_manager, SLOT( updateLabelState( ItemBase* ) ) );
	connect( pathBubble0, SIGNAL( logInfor( QString ) ), this->_scene, SLOT( logInfor( QString ) ) );

	pathBubble0->logOpen();
	return pathBubble0;
}

ItemBase* OpenManager::openPathBubble1( int size, int x, int y, QString name)
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
		
	PathBubble1* pathBubble1 = new PathBubble1( this->_scene->_pathBubbles.size(), size, x, y, this->_scene, this->_scene->getStaManager(), this, name );
	this->_scene->myAddItem( pathBubble1 );
	this->_scene->_pathBubbles.push_back(pathBubble1);
	pathBubble1->setParentPathWayID(-1);

	/*
	QList<PathBubble1*> _pathBubbles;
	QList< TextBubble2* > _codeBubbles;
	QList< ReactionBubble3* > _reactionBubbles;
	QList< GroupBubble4* > _groupBubbles;
	*/

	
	connect( this->_manager, SIGNAL(findEnclosed(PathBubble1*)), pathBubble1, SLOT(selfDelection(PathBubble1*)) );
	connect( pathBubble1, SIGNAL(unGroup(ItemBase*)), this->_manager, SLOT(unGroup( ItemBase*)) );
	connect( pathBubble1, SIGNAL(itemRemoved( ItemBase*)), this->_manager, SLOT(itemRemoved( ItemBase* )) );
	connect( pathBubble1, SIGNAL(itemPosMoved( ItemBase*, QPointF)), this->_manager, SLOT(itemPosMoved( ItemBase*, QPointF)) );
	connect( pathBubble1, SIGNAL( itemPosUpdated(ItemBase*) ), this->_manager, SLOT( itemPosUpdated(ItemBase*)) );
	connect( pathBubble1, SIGNAL( updateLabelState( ItemBase*) ), this->_manager, SLOT( updateLabelState( ItemBase* ) ) );
	connect( pathBubble1, SIGNAL( logInfor( QString ) ), this->_scene, SLOT( logInfor( QString ) ) );
	

	pathBubble1->logOpen();
	return pathBubble1;
}

ItemBase* OpenManager::openNote(ItemBase * item, int size, int x, int y )
{
	NoteBlock* noteBlock=NULL;
	QList<ItemBase *> itemList;
	bool grouped=false;
	itemList = _manager->getGroupMembers(item);
	for (int i = 0; i < itemList.size(); i ++)
	{		
		if ( itemList[i]->getType() == NOTE)
		{
			grouped=true;
            QMessageBox msgBox;
			msgBox.setWindowTitle("Open Note");
			msgBox.setText("A Note bubble for Find is already opened in this group. Would you like to continue?");
			msgBox.setStandardButtons(QMessageBox::Yes);
			msgBox.addButton (QMessageBox::No);
			if(msgBox.exec() == QMessageBox::No)
				return noteBlock;
			else 
				break;
		}
	}
	if(!grouped)
	{
		itemList = _manager->allMembers();
		for (int i = 0; i < itemList.size(); i ++)
		{		
			if ( itemList[i]->getType() ==NOTE)
			{
				QMessageBox msgBox;
				msgBox.setWindowTitle("Open Note");
				msgBox.setText("A Note bubble is already opened. Would you like to continue?");
				msgBox.setStandardButtons(QMessageBox::Yes);
				msgBox.addButton (QMessageBox::No);
				if(msgBox.exec() == QMessageBox::No)
					return noteBlock;
				else 
					break;
			}
		}
	}
	
	noteBlock = new NoteBlock( size, x, y, this->_scene );
	this->_scene->myAddItem( noteBlock );
	noteBlock->resizeItem(size, size/1.618);
	//noteBlock->setZValue(1);	

	QPointF pos = this->_manager->getPosition(noteBlock, noteBlock ->sceneBoundingRect().x(), noteBlock ->sceneBoundingRect().y(), noteBlock ->sceneBoundingRect().width(), noteBlock ->sceneBoundingRect().height());
	if( x == 0 )
	{
		x = pos.x();
	}
	if( y == 0 )
	{
		y = pos.y();
	}
	if(pos.x()!=-1 && pos.y()!=-1)
	{
	   x = pos.x();
	   y = pos.y();	
	}
	QRectF preRect=noteBlock->sceneBoundingRect();
	QRectF targetRect=preRect;
	QPointF newCenter=QPointF(x + noteBlock->sceneBoundingRect().width()/2, y + noteBlock->sceneBoundingRect().height()/2);
	targetRect.moveCenter(newCenter);
	QPointF stepSize=_manager->getSynchronizedSpeed(noteBlock, preRect, targetRect);						
	noteBlock->setTargetPos(newCenter,stepSize,true); 
                    
	//noteBlock->setTargetPos(QPointF(x + noteBlock->sceneBoundingRect().width()/2, y + noteBlock->sceneBoundingRect().height()/2)); 


	connect( noteBlock, SIGNAL(unGroup(ItemBase*)), this->_manager, SLOT(unGroup( ItemBase*)) );
	connect( noteBlock, SIGNAL(itemRemoved( ItemBase*)), this->_manager, SLOT(itemRemoved( ItemBase* )) );
	connect( noteBlock, SIGNAL(itemPosMoved( ItemBase*, QPointF)), this->_manager, SLOT(itemPosMoved( ItemBase*, QPointF)) );
	connect( noteBlock, SIGNAL( itemPosUpdated(ItemBase*) ), this->_manager, SLOT( itemPosUpdated(ItemBase*)) );
	connect( noteBlock, SIGNAL( logInfor( QString ) ), this->_scene, SLOT( logInfor( QString ) ) );
	connect( noteBlock, SIGNAL(itemResized( ItemBase*, QRectF, int)), this->_manager, SLOT(bubbleResized( ItemBase*, QRectF, int) ) );

	noteBlock->logOpen();
	//this->_manager->itemPosUpdated(noteBlock );
	this->_manager->newBubbleCreated(noteBlock);	
	return noteBlock;
}

ItemBase* OpenManager::openNote_2(int pid, int type, int id, ItemBase * item, int size, int x, int y )
{
	CalloutNote* calloutNote=NULL;
	QList<ItemBase *> itemList;
	bool grouped=false;

	//get position

	calloutNote = new CalloutNote(pid, type, id, size, x, y, this->_scene );
	
	this->_scene->myAddItem( calloutNote );
	
	
	calloutNote->setZValue(100);
	
	connect( calloutNote, SIGNAL(unGroup(ItemBase*)), this->_manager, SLOT(unGroup( ItemBase*)) );
	connect( calloutNote, SIGNAL(itemRemoved( ItemBase*)), this->_manager, SLOT(itemRemoved( ItemBase* )) );
	connect( calloutNote, SIGNAL(itemPosMoved( ItemBase*, QPointF)), this->_manager, SLOT(itemPosMoved( ItemBase*, QPointF)) );
	connect( calloutNote, SIGNAL( itemPosUpdated(ItemBase*) ), this->_manager, SLOT( itemPosUpdated(ItemBase*)) );
	connect( calloutNote, SIGNAL( logInfor( QString ) ), this->_scene, SLOT( logInfor( QString ) ) );
	connect( calloutNote, SIGNAL(itemResized( ItemBase*, QRectF, int)), this->_manager, SLOT(bubbleResized( ItemBase*, QRectF, int) ) );

	calloutNote->logOpen();	
	return calloutNote;
}

ItemBase* OpenManager::openTextBubble2(int parentPathID, int parentCodeID, int size, int x, int y, QString name)
{
	//QPoint pos = this->_space->getPosition();
	/*if( x == 0 )
	{
		x = pos.x();
	}
	if( y == 0 )
	{
		y = pos.y();
	}*/
	TextBubble2* textBubble2 = new TextBubble2(parentPathID, parentCodeID, size, x, y, this->_scene, this, name);
	this->_scene->myAddItem( textBubble2 );
	this->_scene->_codeBubbles.push_back(textBubble2);
	
	
	connect( textBubble2, SIGNAL(unGroup(ItemBase*)), this->_manager, SLOT(unGroup( ItemBase*)) );
	connect( textBubble2, SIGNAL(itemRemoved( ItemBase*)), this->_manager, SLOT(itemRemoved( ItemBase* )) );
	connect( textBubble2, SIGNAL(itemPosMoved( ItemBase*, QPointF)), this->_manager, SLOT(itemPosMoved( ItemBase*, QPointF)) );
	connect( textBubble2, SIGNAL( itemPosUpdated(ItemBase*) ), this->_manager, SLOT( itemPosUpdated(ItemBase*)) );
	connect( textBubble2, SIGNAL( updateLabelState( ItemBase* ) ), this->_manager, SLOT( updateLabelState( ItemBase* ) ) );
	//connect( textBubble2, SIGNAL( setMatFileBubble( MatBlock* ) ), this->_scene, SLOT( setMatFileBubble( MatBlock* ) ) );
	//connect( textBubble2, SIGNAL( addParameterFromMat( QString , QString , QList<double>) ), this->_scene->getStatisticBar(), SLOT(addParameterFromMat( QString , QString , QList<double> ) ));
	//connect( textBubble2, SIGNAL( updateDataFromMat( QString, QList<double>) ), this->_scene->getStatisticBar(), SLOT( updateDataFromMat( QString, QList<double>) ));
	connect( textBubble2, SIGNAL( logInfor( QString ) ), this->_scene, SLOT( logInfor( QString ) ) );
	connect( textBubble2, SIGNAL(itemResized( ItemBase*, QRectF, int)), this->_manager, SLOT(bubbleResized( ItemBase*, QRectF, int) ) );
	
	textBubble2->logOpen();
	//this->_manager->itemPosUpdated(textBubble2);	
	//this->_manager->newBubbleCreated(textBubble2);
	return textBubble2;
}

ItemBase* OpenManager::openSubTreeRing(treeRingBubble *parent, int size, int x, int y, set<vector<int>> ItemSelected)
{
	treeRingBubble* subTreeRing = new treeRingBubble(size, x, y, this->_scene, this->_scene->getStaManager(), this, parent, ItemSelected);
	this->_scene->myAddItem( subTreeRing );
	subTreeRing->toplevel=false;
	//subTreeRing->setParentPathWayID(pathwayid);
	//subTreeRing->setZValue(2);
	
	this->_scene->m_treeRingBubbles.push_back(subTreeRing);	
	connect( this->_manager, SIGNAL(findEnclosedTreeRing(treeRingBubble*, treeRingBubble*)), subTreeRing, SLOT(selfDelection(treeRingBubble*, treeRingBubble*)));
	connect( subTreeRing, SIGNAL(unGroup(ItemBase*)), this->_manager, SLOT(unGroup(ItemBase*)) );
	connect( subTreeRing, SIGNAL(itemRemoved( ItemBase*)), this->_manager, SLOT(itemRemoved( ItemBase* )) );
	connect( subTreeRing, SIGNAL(itemPosMoved( ItemBase*, QPointF)), this->_manager, SLOT(itemPosMoved( ItemBase*, QPointF)) );
	connect( subTreeRing, SIGNAL( itemPosUpdated(ItemBase*) ), this->_manager, SLOT( itemPosUpdated(ItemBase*)) );
	connect( subTreeRing, SIGNAL( updateLabelState( ItemBase*) ), this->_manager, SLOT( updateLabelState( ItemBase* ) ) );
	connect( subTreeRing, SIGNAL( logInfor( QString ) ), this->_scene, SLOT( logInfor( QString ) ) );
	connect( subTreeRing, SIGNAL(itemResized( ItemBase*, QRectF, int)), this->_manager, SLOT(bubbleResized( ItemBase*, QRectF, int) ) );
		
	subTreeRing->logOpen();
	_scene->preOpenRects.clear();
	
	return subTreeRing;
}

ItemBase* OpenManager::openSubPathBubble1( QString orthName, int pathwayid, int sizex, int sizey, int x, int y, vector<int> ItemSelected, vector<vector<int>> EdgeSelected,  vector<set<vector<int>>> AnodeContains, QString name, QString pathName, QString lastStoredfileName)
{
	/*QPoint pos = this->_space->getPosition();
	if( x == 0 )
	{  x = pos.x();
	}
	if( y == 0 )
	{  y = pos.y();
	}
	*/
    
	subPathBubble1* pathBubble1 = new subPathBubble1(orthName, pathwayid, sizex, sizey, x, y,  ItemSelected, EdgeSelected, AnodeContains, this->_scene, this->_scene->getStaManager(), this, name, pathName, lastStoredfileName);
	this->_scene->myAddItem( pathBubble1 );
	//pathBubble1->setParentPathWayID(pathwayid);
	//pathBubble1->setZValue(2);
	this->_scene->_pathBubbles.push_back(dynamic_cast<PathBubble1*> (pathBubble1));
	
	connect( _scene, SIGNAL(findEnclosed(PathBubble1*, PathBubble1*)), pathBubble1, SLOT(selfDelection(PathBubble1*, PathBubble1*)) );
	connect( this->_manager, SIGNAL(findEnclosed(PathBubble1*, PathBubble1*)), pathBubble1, SLOT(selfDelection(PathBubble1*, PathBubble1*)) );
	connect( pathBubble1, SIGNAL(unGroup(ItemBase*)), this->_manager, SLOT(unGroup( ItemBase*)) );
	connect( pathBubble1, SIGNAL(itemRemoved( ItemBase*)), this->_manager, SLOT(itemRemoved( ItemBase* )) );
	connect( pathBubble1, SIGNAL(itemPosMoved( ItemBase*, QPointF)), this->_manager, SLOT(itemPosMoved( ItemBase*, QPointF)) );
	connect( pathBubble1, SIGNAL( itemPosUpdated(ItemBase*) ), this->_manager, SLOT( itemPosUpdated(ItemBase*)) );
	connect( pathBubble1, SIGNAL( updateLabelState( ItemBase*) ), this->_manager, SLOT( updateLabelState( ItemBase* ) ) );
	connect( pathBubble1, SIGNAL( logInfor( QString ) ), this->_scene, SLOT( logInfor( QString ) ) );
	connect( pathBubble1, SIGNAL(itemResized( ItemBase*, QRectF, int)), this->_manager, SLOT(bubbleResized( ItemBase*, QRectF, int) ) );
	connect( pathBubble1, SIGNAL( bubbleDeleteItself(ItemBase *item ) ), this->_scene, SLOT(bubbleDeleteItself(ItemBase *item) ) );
	pathBubble1->logOpen();
	_scene->preOpenRects.clear();
	
	pathBubble1->openPreNodeNotes();
	
	return pathBubble1;
}

/*ItemBase* OpenManager::openPathBubble2( int size, int x, int y, QString name)
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
		
	PathBubble2* pathBubble2 = new PathBubble2( size, x, y, this->_scene, this->_scene->getStaManager(), this, name );
	this->_scene->myAddItem( pathBubble2 );

	connect( pathBubble2, SIGNAL(unGroup(ItemBase*)), this->_manager, SLOT(unGroup( ItemBase*)) );
	connect( pathBubble2, SIGNAL(itemRemoved( ItemBase*)), this->_manager, SLOT(itemRemoved( ItemBase* )) );
	connect( pathBubble2, SIGNAL(itemPosMoved( ItemBase*, QPointF)), this->_manager, SLOT(itemPosMoved( ItemBase*, QPointF)) );
	connect( pathBubble2, SIGNAL( itemPosUpdated(ItemBase*) ), this->_manager, SLOT( itemPosUpdated(ItemBase*)) );
	connect( pathBubble2, SIGNAL( updateLabelState( ItemBase*) ), this->_manager, SLOT( updateLabelState( ItemBase* ) ) );
	connect( pathBubble2, SIGNAL( logInfor( QString ) ), this->_scene, SLOT( logInfor( QString ) ) );

	pathBubble2->logOpen();
	return pathBubble2;
}*/


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
	/*
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
	*/
	return true;
}


QString OpenManager::checkBatByPath( QString name )
{
	QString result;// = QtBat::checkPath( name );
	return result;
}

QString OpenManager::checkPCByPath(QString name)
{
	QString result = QtSM::checkPath( name );
	result.append( SM::checkPath( name ) );
	return result;
}

QString OpenManager::checkPathwayByPath(QString name)
{
	QString result = QtSM::checkPath( name );
	result.append( SM::checkPathwayPath( name ) );
	return result;
}

/*
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
*/

bool OpenManager::openPathwayByPath( ItemBase *&result, QString orthName, int size, int x, int y, QString name, QString pathName, QString lastStoredGraph)
{
	//_curFileName = fileName;
	//_curFilePath = _curFileName;
	int tindex0,tindex1; 
	QString pathwayName=name;
	//tindex1 = pathwayName.lastIndexOf(".");
	tindex0 = pathwayName.lastIndexOf("/");		
	//pathwayName.remove(tindex1+1,pathwayName.size());	
	pathwayName.remove(0, tindex0+1);
	vector<set<vector<int>>> AnodeContains;
	for (int i = 0; i < _scene->_pathBubbles.size(); i ++)		
	{
		int size=_scene->_pathBubbles.size();
		PathBubble1 * tmp =_scene->_pathBubbles[i];
		//ItemBase *tmp1= dynamic_cast <ItemBase *> (tmp);					
		//int pid=_scene->_pathBubbles[i]->m_pathwayID;

		if(_scene->_pathBubbles[i])
		if(_scene->_pathBubbles[i]!=NULL)
		if(_scene->_pathBubbles[i]->m_pathwayID>=0 && _scene->_pathBubbles[i]->m_pathwayID < _scene->_pathBubbles.size())
		if(_scene->_pathBubbles[i]->getPathwayName().compare(pathName)==0 )
		{   
			AnodeContains.resize(1);

			QMessageBox msgBox;
			msgBox.setWindowTitle("Open Pathway");
			msgBox.setText("The pathway is already opened. Would you like to continue?");
			msgBox.setStandardButtons(QMessageBox::Yes);
			msgBox.addButton (QMessageBox::No);
			if(msgBox.exec() == QMessageBox::No)
				return false;
			else 
				break;			
		}
	}
	//result = openPathBubble1( 400, x, y, name);
	vector<int> itemSelected;
	vector<vector<int>> edgeSelected;
	
	result = openSubPathBubble1(orthName, -1, 400, 400, x, y, itemSelected, edgeSelected, AnodeContains, name, pathName, lastStoredGraph);		
	//this->_manager->newBubbleCreated(result);

	return true;
}

bool OpenManager::openExpressionByPath( ItemBase *&result, int size, int x, int y, QString name)
{
	/*QString output = checkPathwayByPath( name );
	if( output != "Success" )
	{
		QMessageBox msgBox;
		msgBox.setText( output );
		msgBox.exec();
		return false;
	}*/
	//********************************************************************
	
	QString expressionName=name;
	//tindex1 = pathwayName.lastIndexOf(".");
	int tindex0 = expressionName.lastIndexOf("/");		
	//pathwayName.remove(tindex1+1,pathwayName.size());	
	expressionName.remove(0, tindex0+1);
	
	//QList<ItemBase*> items = getCurrentItems( init );	
	for (int m = 0; m < _scene->_expressionBubbles.size(); m++)
	{				
		if(_scene->_expressionBubbles[m])
		if(_scene->_expressionBubbles[m]!=NULL)
		if(_scene->_expressionBubbles[m]->m_pathwayID>=0 && _scene->_expressionBubbles[m]->m_pathwayID < _scene->_expressionBubbles.size())		
		if(_scene->_expressionBubbles[m]->getCurrentFilename().compare(expressionName)==0 )		
		{	
			QMessageBox msgBox;
			msgBox.setWindowTitle("Open Expresssion");
			msgBox.setText("The expression data is already opened. Would you like to continue?");
			msgBox.setStandardButtons(QMessageBox::Yes);
			msgBox.addButton (QMessageBox::No);
			if(msgBox.exec() == QMessageBox::No)
				return false;
			else 
				break;
		}
		
	}			
	
	result= openExpressionBubble(size, x, y, name);
	
	//vector<int> itemSelected;
	//vector<vector<int>> edgeSelected;

	//result = openSubPathBubble1(-1, 400, x, y, itemSelected, edgeSelected, name);		
	//this->_manager->itemPosUpdated(result);	
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
	connect( qtsm, SIGNAL( moveHightlightInGroup(ItemBase*, QRectF) ), this->_manager, SLOT( moveHightlightinGroup(ItemBase*, QRectF ) ) );
	connect( qtsm, SIGNAL( releaseHightlightInGroup(ItemBase*) ), this->_manager, SLOT( releaseHightlightinGroup(ItemBase*) ) );
	this->_scene->myAddItem( qtsm );
	qtsm->logOpen();
	result = qtsm;
	return true;
}

ItemBase* OpenManager::openSearchView(ItemBase * item, int size, int x, int y, QString name)
{
	WebBlock* web=NULL;
	QList<ItemBase *> itemList;
	bool grouped=false;
	itemList = _manager->getGroupMembers(item);
	for (int i = 0; i < itemList.size(); i ++)
	{		
		if ( itemList[i]->getType() == SEARCHFIND)
		{
			grouped=true;
            QMessageBox msgBox;
			msgBox.setWindowTitle("Open Find");
			msgBox.setText("A Find bubble is already opened in this group. Would you like to continue?");
			msgBox.setStandardButtons(QMessageBox::Yes);
			msgBox.addButton (QMessageBox::No);
			if(msgBox.exec() == QMessageBox::No)
				return web;
			else 
				break;
		}
	}
	if(!grouped)
	{
		itemList = _manager->allMembers();
		for (int i = 0; i < itemList.size(); i ++)
		{		
			if ( itemList[i]->getType() == SEARCHFIND)
			{
				QMessageBox msgBox;
				msgBox.setWindowTitle("Open Find");
				msgBox.setText("A Find bubble is already opened. Would you like to continue?");
				msgBox.setStandardButtons(QMessageBox::Yes);
				msgBox.addButton (QMessageBox::No);
				if(msgBox.exec() == QMessageBox::No)
					return web;
				else 
					break;
			}
		}
	}

	web = new WebBlock( size, x, y, this->_scene, 2, name);	

	web->resetOriRect();
	this->_scene->myAddItem( web );

	
	connect( web, SIGNAL(unGroup(ItemBase*)), this->_manager, SLOT(unGroup( ItemBase*)) );
	connect( web, SIGNAL(itemRemoved( ItemBase*)), this->_manager, SLOT(itemRemoved( ItemBase* )) );
	connect( web, SIGNAL(itemPosMoved( ItemBase*, QPointF)), this->_manager, SLOT(itemPosMoved( ItemBase*, QPointF)) );
	connect( web, SIGNAL( itemPosUpdated(ItemBase*) ), this->_manager, SLOT( itemPosUpdated(ItemBase*)) );
	connect( web, SIGNAL( updateLabelState( ItemBase*) ), this->_manager, SLOT( updateLabelState( ItemBase* ) ) );
	connect( web, SIGNAL( logInfor( QString ) ), this->_scene, SLOT( logInfor( QString ) ) );
	connect(  web, SIGNAL(itemResized( ItemBase*, QRectF, int)), this->_manager, SLOT(bubbleResized( ItemBase*, QRectF, int) ) );
	
	
	//this->_manager->newBubbleCreated(web);	
	web->logOpen();
	return web;
}

ItemBase* OpenManager::openSettingsView(ItemBase * item, int size, int x, int y, QString name)
{
	WebBlock* web=NULL;
	QList<ItemBase *> itemList;
	bool grouped=false;
	itemList = _manager->getGroupMembers(item);
	for (int i = 0; i < itemList.size(); i ++)
	{		
		if ( itemList[i]->getType() == SETTINGS)
		{
			grouped=true;
            QMessageBox msgBox;
			msgBox.setWindowTitle("Open Settings for Graph Display");
			msgBox.setText("A Settings bubble for Graph Display is already opened in this group. Would you like to continue?");
			msgBox.setStandardButtons(QMessageBox::Yes);
			msgBox.addButton (QMessageBox::No);
			if(msgBox.exec() == QMessageBox::No)
				return web;
			else 
				break;
		}
	}
	if(!grouped)
	{
		itemList = _manager->allMembers();
		for (int i = 0; i < itemList.size(); i ++)
		{		
			if ( itemList[i]->getType() == SETTINGS)
			{
				QMessageBox msgBox;
				msgBox.setWindowTitle("Open Settings for Graph Display");
				msgBox.setText("A Settings bubble for Graph Display is already opened. Would you like to continue?");
				msgBox.setStandardButtons(QMessageBox::Yes);
				msgBox.addButton (QMessageBox::No);
				if(msgBox.exec() == QMessageBox::No)
					return web;
				else 
					break;
			}
		}
	}

	web = new WebBlock(size, x, y, this->_scene, 3, name);
    web->resetOriRect();
	this->_scene->myAddItem( web );

	QPointF pos = this->_manager->getPosition(web, web ->sceneBoundingRect().x(), web ->sceneBoundingRect().y(), web ->sceneBoundingRect().width(), web ->sceneBoundingRect().height());
	if( x == 0 )
	{
		x = pos.x();
	}
	if( y == 0 )
	{
		y = pos.y();
	}
	if(pos.x()!=-1 && pos.y()!=-1)
	{
	   x = pos.x();
	   y = pos.y();	
	}
	QRectF preRect=web->sceneBoundingRect();
	QRectF targetRect=preRect;
	QPointF newCenter=QPointF(x + web->sceneBoundingRect().width()/2, y + web->sceneBoundingRect().height()/2);
	targetRect.moveCenter(newCenter);
	QPointF stepSize=_manager->getSynchronizedSpeed(web, preRect, targetRect);						
	web->setTargetPos(newCenter,stepSize,true); 
    
	//web->setTargetPos(QPointF(x + web->sceneBoundingRect().width()/2, y + web->sceneBoundingRect().height()/2)); 

	connect( web, SIGNAL(unGroup(ItemBase*)), this->_manager, SLOT(unGroup( ItemBase*)) );
	connect( web, SIGNAL(itemRemoved( ItemBase*)), this->_manager, SLOT(itemRemoved( ItemBase* )) );
	connect( web, SIGNAL(itemPosMoved( ItemBase*, QPointF)), this->_manager, SLOT(itemPosMoved( ItemBase*, QPointF)) );
	connect( web, SIGNAL( itemPosUpdated(ItemBase*) ), this->_manager, SLOT( itemPosUpdated(ItemBase*)) );
	connect( web, SIGNAL( updateLabelState( ItemBase*) ), this->_manager, SLOT( updateLabelState( ItemBase* ) ) );
	connect( web, SIGNAL( logInfor( QString ) ), this->_scene, SLOT( logInfor( QString ) ) );
	connect(  web, SIGNAL(itemResized( ItemBase*, QRectF, int)), this->_manager, SLOT(bubbleResized( ItemBase*, QRectF, int) ) );

	this->_manager->newBubbleCreated(web);	
	web->logOpen();
	return web;
}


ItemBase* OpenManager::openSearchSettingsView(ItemBase * item, int size, int x, int y, QString name)
{
	WebBlock* web=NULL;
	QList<ItemBase *> itemList;
	bool grouped=false;
	itemList = _manager->getGroupMembers(item);
	for (int i = 0; i < itemList.size(); i ++)
	{		
		if ( itemList[i]->getType() == SEARCHSETTINGS)
		{
			grouped=true;
            QMessageBox msgBox;
			msgBox.setWindowTitle("Open Settings for Graph Display");
			msgBox.setText("A Settings bubble for Graph Display is already opened in this group. Would you like to continue?");
			msgBox.setStandardButtons(QMessageBox::Yes);
			msgBox.addButton (QMessageBox::No);
			if(msgBox.exec() == QMessageBox::No)
				return web;
			else 
				break;
		}
	}
	if(!grouped)
	{
		itemList = _manager->allMembers();
		for (int i = 0; i < itemList.size(); i ++)
		{		
			if ( itemList[i]->getType() == SEARCHSETTINGS)
			{
				QMessageBox msgBox;
				msgBox.setWindowTitle("Open Settings for Graph Display");
				msgBox.setText("A Settings bubble for Graph Display is already opened. Would you like to continue?");
				msgBox.setStandardButtons(QMessageBox::Yes);
				msgBox.addButton (QMessageBox::No);
				if(msgBox.exec() == QMessageBox::No)
					return web;
				else 
					break;
			}
		}
	}

	web = new WebBlock(size, x, y, this->_scene, 4, name);
    web->resetOriRect();
	this->_scene->myAddItem( web );

	QPointF pos = this->_manager->getPosition(web, web ->sceneBoundingRect().x(), web ->sceneBoundingRect().y(), web ->sceneBoundingRect().width(), web ->sceneBoundingRect().height());
	if( x == 0 )
	{
		x = pos.x();
	}
	if( y == 0 )
	{
		y = pos.y();
	}
	if(pos.x()!=-1 && pos.y()!=-1)
	{
	   x = pos.x();
	   y = pos.y();	
	}
	QRectF preRect=web->sceneBoundingRect();
	QRectF targetRect=preRect;
	QPointF newCenter=QPointF(x + web->sceneBoundingRect().width()/2, y + web->sceneBoundingRect().height()/2);
	targetRect.moveCenter(newCenter);
	QPointF stepSize=_manager->getSynchronizedSpeed(web, preRect, targetRect);						
	web->setTargetPos(newCenter,stepSize,true); 
    

	//web->setTargetPos(QPointF(x + web->sceneBoundingRect().width()/2, y + web->sceneBoundingRect().height()/2)); 

	connect( web, SIGNAL(unGroup(ItemBase*)), this->_manager, SLOT(unGroup( ItemBase*)) );
	connect( web, SIGNAL(itemRemoved( ItemBase*)), this->_manager, SLOT(itemRemoved( ItemBase* )) );
	connect( web, SIGNAL(itemPosMoved( ItemBase*, QPointF)), this->_manager, SLOT(itemPosMoved( ItemBase*, QPointF)) );
	connect( web, SIGNAL( itemPosUpdated(ItemBase*) ), this->_manager, SLOT( itemPosUpdated(ItemBase*)) );
	connect( web, SIGNAL( updateLabelState( ItemBase*) ), this->_manager, SLOT( updateLabelState( ItemBase* ) ) );
	connect( web, SIGNAL( logInfor( QString ) ), this->_scene, SLOT( logInfor( QString ) ) );
	connect(  web, SIGNAL(itemResized( ItemBase*, QRectF, int)), this->_manager, SLOT(bubbleResized( ItemBase*, QRectF, int) ) );

	this->_manager->newBubbleCreated(web);	
	web->logOpen();
	return web;
}

ItemBase* OpenManager::openWebView(ItemBase * item, int size, int x, int y, QString name )
{	
	WebBlock* web=NULL;
	QList<ItemBase *> itemList;
	bool grouped=false;
	/*itemList = _manager->getGroupMembers(item);
	for (int i = 0; i < itemList.size(); i ++)
	{		
		if ( itemList[i]->getType() == WEB)
		{
			grouped=true;
            QMessageBox msgBox;
			msgBox.setWindowTitle("Open Web Browser");
			msgBox.setText("A Web Browser bubble is already opened in this group. Would you like to continue?");
			msgBox.setStandardButtons(QMessageBox::Yes);
			msgBox.addButton (QMessageBox::No);
			if(msgBox.exec() == QMessageBox::No)
				return web;
			else 
				break;
		}
	}*/
	if(!grouped)
	{
		itemList = _manager->allMembers();
		for (int i = 0; i < itemList.size(); i ++)
		{		
			if ( itemList[i]->getType() == WEB)
			{
				QMessageBox msgBox;
				msgBox.setWindowTitle("Open Web Browser");
				msgBox.setText("A Web Browser bubble is already opened. Would you like to continue?");
				msgBox.setStandardButtons(QMessageBox::Yes);
				msgBox.addButton (QMessageBox::No);
				if(msgBox.exec() == QMessageBox::No)
					return web;
				else 
					break;
			}
		}
	}

	
	web = new WebBlock(size, x, y, this->_scene, 1, name );
	this->_scene->myAddItem( web );
	
	connect( web, SIGNAL(unGroup(ItemBase*)), this->_manager, SLOT(unGroup( ItemBase*)) );
	connect( web, SIGNAL(itemRemoved( ItemBase*)), this->_manager, SLOT(itemRemoved( ItemBase* )) );
	connect( web, SIGNAL(itemPosMoved( ItemBase*, QPointF)), this->_manager, SLOT(itemPosMoved( ItemBase*, QPointF)) );
	connect( web, SIGNAL( itemPosUpdated(ItemBase*) ), this->_manager, SLOT( itemPosUpdated(ItemBase*)) );
	connect( web, SIGNAL( updateLabelState( ItemBase*) ), this->_manager, SLOT( updateLabelState( ItemBase* ) ) );
	connect( web, SIGNAL( logInfor( QString ) ), this->_scene, SLOT( logInfor( QString ) ) );
	connect( web, SIGNAL(itemResized( ItemBase*, QRectF, int)), this->_manager, SLOT(bubbleResized( ItemBase*, QRectF, int) ) );
	
	this->_manager->newBubbleCreated(web);	
	web->logOpen();

	return web;
}

/*ItemBase* OpenManager::openPen(int size, int x, int y)
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
}*/

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

/*ItemBase* OpenManager::openMatlab(int size, int x, int y)
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
}*/



ItemBase* OpenManager::openReactionBubble3(int m_pathwayID, int size, int x, int y, vector<int> itemSelected, QString name)
{
	//QPoint pos = this->_space->getPosition();
	/*if( x == 0 )
	{
		x = pos.x();
	}
	if( y == 0 )
	{
		y = pos.y();
	}*/
	ReactionBubble3* reactionBubble3 = new ReactionBubble3(m_pathwayID, size, x, y, itemSelected, this->_scene,  this, name);
	this->_scene->myAddItem( reactionBubble3 );
	this->_scene->_reactionBubbles.push_back(reactionBubble3);

	connect( reactionBubble3, SIGNAL(unGroup(ItemBase*)), this->_manager, SLOT(unGroup( ItemBase*)) );
	connect( reactionBubble3, SIGNAL(itemRemoved( ItemBase*)), this->_manager, SLOT(itemRemoved( ItemBase* )) );
	connect( reactionBubble3, SIGNAL(itemPosMoved( ItemBase*, QPointF)), this->_manager, SLOT(itemPosMoved( ItemBase*, QPointF)) );
	connect( reactionBubble3, SIGNAL( itemPosUpdated(ItemBase*) ), this->_manager, SLOT( itemPosUpdated(ItemBase*)) );
	connect( reactionBubble3, SIGNAL( updateLabelState( ItemBase* ) ), this->_manager, SLOT( updateLabelState( ItemBase* ) ) );
	//connect( reactionBubble3, SIGNAL( setMatFileBubble( MatBlock* ) ), this->_scene, SLOT( setMatFileBubble( MatBlock* ) ) );
	//connect( reactionBubble3, SIGNAL( addParameterFromMat( QString , QString , QList<double>) ), this->_scene->getStatisticBar(), SLOT(addParameterFromMat( QString , QString , QList<double> ) ));
	//connect( reactionBubble3, SIGNAL( updateDataFromMat( QString, QList<double>) ), this->_scene->getStatisticBar(), SLOT( updateDataFromMat( QString, QList<double>) ));
	connect( reactionBubble3, SIGNAL( logInfor( QString ) ), this->_scene, SLOT( logInfor( QString ) ) );
	
	reactionBubble3->logOpen();
	return reactionBubble3;
}


ItemBase* OpenManager::openGroupBubble4(int m_pathwayID, int size, int x, int y, vector<int> itemSelected, vector<vector<int>> edgeSelected, QString name)
{
	//QPoint pos = this->_space->getPosition();	
	GroupBubble4* groupBubble4 = new GroupBubble4( m_pathwayID, size, x, y, itemSelected, edgeSelected, this->_scene, this, name);
	this->_scene->myAddItem( groupBubble4 );
	this->_scene->_groupBubbles.push_back(groupBubble4);

	connect( groupBubble4, SIGNAL(unGroup(ItemBase*)), this->_manager, SLOT(unGroup( ItemBase*)) );
	connect( groupBubble4, SIGNAL(itemRemoved( ItemBase*)), this->_manager, SLOT(itemRemoved( ItemBase* )) );
	connect( groupBubble4, SIGNAL(itemPosMoved( ItemBase*, QPointF)), this->_manager, SLOT(itemPosMoved( ItemBase*, QPointF)) );
	connect( groupBubble4, SIGNAL( itemPosUpdated(ItemBase*) ), this->_manager, SLOT( itemPosUpdated(ItemBase*)) );
	connect( groupBubble4, SIGNAL( updateLabelState( ItemBase* ) ), this->_manager, SLOT( updateLabelState( ItemBase* ) ) );
	//connect( groupBubble4, SIGNAL( setMatFileBubble( MatBlock* ) ), this->_scene, SLOT( setMatFileBubble( MatBlock* ) ) );
	//connect( groupBubble4, SIGNAL( addParameterFromMat( QString , QString , QList<double>) ), this->_scene->getStatisticBar(), SLOT(addParameterFromMat( QString , QString , QList<double> ) ));
	//connect( groupBubble4, SIGNAL( updateDataFromMat( QString, QList<double>) ), this->_scene->getStatisticBar(), SLOT( updateDataFromMat( QString, QList<double>) ));
	connect( groupBubble4, SIGNAL( logInfor( QString ) ), this->_scene, SLOT( logInfor( QString ) ) );
	
	groupBubble4->logOpen();
	return groupBubble4;
}


/*ItemBase* OpenManager::openMatlabByPath( int size, int x, int y, QString name )
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
}*/

/*
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
}*/

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

QList<QString> OpenManager::getPathwayPathByToken( QString token, int type )
{
	QList<QString> result;
	QString currentPath = "data/pathways/";//this->_pcPrefix;
	token.replace(0,24,"");
	currentPath.append( token );

	//currentPath.append( tr("pathways.txt") );
	//QString QType;
	//QType.setNum( type - 3 );
	//currentPath.append( QType);
	result.append( currentPath );
	return result;
}

QList<QString> OpenManager::getExpresssionByToken( QString token, int type )
{
	QList<QString> result;
	QString currentPath = "data/pathways/";//this->_pcPrefix;
	token.replace(0,24,"");
	currentPath.append( token );

	//currentPath.append( tr("pathways.txt") );
	//QString QType;
	//QType.setNum( type - 3 );
	//currentPath.append( QType);
	result.append( currentPath );
	return result;
}


/*QList<ItemBase*> OpenManager::openBatByToken(QString name)
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
*/

/*
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
*/

/*
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
*/
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

/*
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
*/
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
	
	int viewsize = views.size();
	QList<QString> names = matchFileName( name );
	int namesize = names.size();
	QVector<QRectF> rect;
	ItemBase *tmp, *first;
	int i, j, index;
	QPointF tPos;

	namesize = names.size();	


	if (names.size() <= views.size() )
	{
		for ( i = 0; i < names.size(); i ++)
		{
			tPos.setX( pos.x() );
			tPos.setY( pos.y() + STEP*i );
			rect = queryPreOpen( tPos, views.size() );
			index = 0;
			int m_bindPathwayID=-1;
			for ( j = 0; j < views.size(); j ++ )
			{
				if(views[j]!=6) //not expression
				{
					tmp = this->openSpecifiedViewByName( names[i], views[j], rect[index].center() );
					if(tmp)
					{
						PathBubble1* pBubble = dynamic_cast<PathBubble1*>(tmp);		
						if(!_scene->_geneInfo[pBubble->m_pathwayID].empty())
							m_bindPathwayID=pBubble->m_pathwayID;
						index += 1;
					}				    

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
			for ( j = 0; j < views.size(); j ++ )
			{
				if(views[j]==6) //expression
				{
					tmp = this->openSpecifiedViewByName( names[i], views[j], rect[index].center() );
					if(tmp)
					{
					expressionBubble* eBubble = dynamic_cast<expressionBubble*>(tmp);		
					index += 1;
					if ( i == 0 && j == 0)
					{
						first = tmp;
					}
					else
					{
						this->_manager->addToGroup( tmp, first );
					}
					eBubble->m_bindPathwayID=m_bindPathwayID;
					eBubble->TransferExpression(m_bindPathwayID);
					eBubble->searchExpressedProtein();
					//eBubble->setExpressedGenePos(2);
					}
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
				if(tmp)
				{
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
	}	
	return;
}

ItemBase* OpenManager::openSpecifiedViewByName( QString name, int view, QPointF pos )
{
	//3D vis view
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
		QList<QString> paths = getPathwayPathByToken( name, view );
		if( paths.size() > 0 )
		{
			//this->openPCByPath( item, 200, pos.x(), pos.y(), paths[0] );
			//this->openPathwayByPath(item, 200, pos.x(), pos.y(), paths[0]);
		}
	}
	else if( view == 6 ) // PC 3
	{
		QList<QString> paths = getExpresssionByToken( name, view );
		
		if( paths.size() > 0 )
		{
			//this->openPCByPath( item, 200, pos.x(), pos.y(), paths[0] );
			this->openExpressionByPath(item, 250, pos.x(), pos.y(), paths[0]);
			expressionBubble* EBubble = dynamic_cast<expressionBubble*>(item);			
			//EBubble->m_bindPathwayID = k;						
			//EBubble->searchExpressedProtein();
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
		//QtBat* bat = dynamic_cast<QtBat*>(item);
		//name = bat->getBatName();
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
		//view = 0;

		//success = true;
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


/*
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
*/
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
	item = openWebView(NULL, sizewidth, x, y, name );
	if ( item )
	{
		item->resizeItem( sizewidth, sizeheight );
		item->recoverFromXmlNode( node );
		return item;
	}
	return NULL;
}
ItemBase* OpenManager::openPathBubble0FromXmlNode( QDomElement node)
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
	item = openPathBubble0(sizewidth, x, y);
	
	if (item)
	{
		item->resizeItem( sizewidth, sizeheight );
		
		//item->recoverFromXmlNode( node );
		return item;
	}

	return NULL;
}

ItemBase* OpenManager::openPathBubble1FromXmlNode( QDomElement node)
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
	item = openPathBubble1(sizewidth, x, y);
	
	if (item)
	{
		item->resizeItem( sizewidth, sizeheight );
		
		item->recoverFromXmlNode( node );
		return item;
	}

	return NULL;

}

void OpenManager::recoverEdgeFromXmlNode( QDomNodeList paraNode, vector<vector<int>> &edge)
{
	QDomNodeList posNode;
	QString str;
	QStringList posStr;	
		
	if ( !paraNode.isEmpty() )
	{
		paraNode = paraNode.at(0).toElement().elementsByTagName( tr("edge") );
		int size=paraNode.size();
		for (int i = 0; i < paraNode.size(); i ++)
		{
			int type0,type1,type2,id0,id1,id2,d;
			vector<int> temp;

			posNode = paraNode.at(i).toElement().elementsByTagName( tr("edge") );
			posStr= paraNode.at(i).toElement().text().split( "," );
			
			str = posStr.at(0);			
			type0 = str.toInt();
			str = posStr.at(1);
			if(str[0]=='-')
			{
				str=str.mid(1,str.size()-1);	
				id0  = str.toInt();
				id0=-id0;
			}
			else
			{
			   id0  = str.toInt();
			}

			str = posStr.at(2);
			
			type1 = str.toInt();
			str = posStr.at(3);
			//str.chop(1);
			//str = str.simplified();
			if(str[0]=='-')
			{
				str=str.mid(1,str.size()-1);	
				id1=str.toInt();
				id1=-id1;
			}
			else 
			id1  = str.toInt();

			str = posStr.at(4);
			//str.remove( 0, 1 );
			//str = str.simplified();
			type2 = str.toInt();
			str = posStr.at(5);
			//str.chop(1);
			//str = str.simplified();
			if(str[0]=='-')
			{
				str=str.mid(1,str.size()-1);			
				id2 = str.toInt();
				id2 = - id2;
			}
			else 
				id2  = str.toInt();

			str = posStr.at(6);
			//str.chop(1);
			//str = str.simplified();
			if(str[0]=='-')
			{
				str=str.mid(1,str.size()-1);			
				d = str.toInt();
				d = - d;
			}
			else 
				d  = str.toInt();
			
			temp.push_back( type0);
			temp.push_back( id0);
			temp.push_back( type1);
			temp.push_back( id1);
			temp.push_back( type2);
			temp.push_back( id2);
			temp.push_back( d);
			edge.push_back(temp);
		}
	}
}


void OpenManager::recoverItemFromXmlNode( QDomNodeList paraNode, vector<int> &item)
{
	QDomNodeList posNode;
	QString str;
	QStringList posStr;	
		
	//paraNode = node.elementsByTagName( tr("initEdges") );	
	if ( !paraNode.isEmpty() )
	{
		//paraNode = paraNode.at(0).toElement().elementsByTagName( tr("edge") );		
		paraNode = paraNode.at(0).toElement().elementsByTagName( tr("item") );
		int size=paraNode.size();
		for (int i = 0; i < paraNode.size(); i ++)
		{
			int type0,id0;
			vector<int> temp;

			posNode = paraNode.at(i).toElement().elementsByTagName( tr("item") );
			posStr= paraNode.at(i).toElement().text().split( "," );
			
			str = posStr.at(0);			
			type0 = str.toInt();
			str = posStr.at(1);
			if(str[0]=='-')
			{
				str=str.mid(1,str.size()-1);	
				id0  = str.toInt();
				id0=-id0;
			}
			else
			{
			   id0  = str.toInt();
			}
			item.push_back( type0);
			item.push_back( id0);						
		}
	}
		

}

ItemBase* OpenManager::openExpressionBubbleFromXmlNode( QDomElement node, int pathwayID)
//ItemBase* OpenManager::openExpressionBubble(int size, int x, int y, QString name)
{
	QDomNodeList paraNode;
	QDomElement tmpPara;
	
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
	QString name =  "data/Reactome_Pathway_Data/expressionData/" + str;
	
	paraNode = node.elementsByTagName( tr("bindPathwayID") );
	tmpPara = paraNode.at(0).toElement();
	str = tmpPara.text();
	int bID;
	if(str[0]=='-')
	{
		str=str.mid(1,str.size()-1);	
		bID  = str.toInt();
		bID=-bID;
	}
	else
	{
	    bID  = str.toInt();
	}
	ItemBase *item = openExpressionBubble(sizewidth, x, y, name);

	expressionBubble* ExpressionBubble = dynamic_cast<expressionBubble*>(item);
	QString tname = name + "0expression.txt";  		
	_scene->pwdParser->readexpression(tname.toUtf8().data(), ExpressionBubble->quantityName, ExpressionBubble->geneInfo, ExpressionBubble->quantity, ExpressionBubble->minmax);
	//ExpressionBubble->setExpressedGenePos(2);
	ExpressionBubble->getExpressionToBePaint();
	/*expressionBubble* eBubble= dynamic_cast<expressionBubble*>(item);

	QString tname = name + "0expression.txt";  
	eBubble->readexpression(tname.toUtf8().data(), eBubble->quantityName, eBubble->geneInfo, eBubble->quantity, eBubble->minmax);			
	eBubble->searchExpressedProtein();
	eBubble->setExpressedGenePos(2);
	eBubble->m_bindPathwayID=bID;*/
	return item;
}



ItemBase* OpenManager::openSubPathBubble1FromXmlNode( QDomElement node, int pathwayID,int toplevel)
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

	ItemBase *item;
	vector<int> itemSelected;
	vector<vector<int>> edgeSelected;	


	paraNode = node.elementsByTagName( tr("initEdges") );	
	if ( !paraNode.isEmpty() )	
	    recoverEdgeFromXmlNode(paraNode, edgeSelected);

	paraNode = node.elementsByTagName( tr("initItems") );	
	if ( !paraNode.isEmpty() )	
	    recoverItemFromXmlNode(paraNode, itemSelected);
	

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
	QString name =  "data/pathways/" + str;

	if(!toplevel)
		toplevel=toplevel;

	if(toplevel)
	{
		QString fname = str;
		QString pname = str;
		int tindex0 = pname.lastIndexOf("/");	
		//QString path = pname;
		//path.remove(tindex0,path.size()-tindex0);
		pname.remove(0, tindex0+1);
		QString num= pname.remove(pname.size()-1,1);
		int ID=num.toInt();
		//int st = fileName.lastIndexOf("/");
		//fileName = fileName.remove(0, st+1);
		//st = fileName.lastIndexOf("_");		   
		//fileName = fileName.remove(st,fileName.size());
		int size=_scene->_fileSelection->pathNames.size();
		for(int j=0; j<_scene->_fileSelection->pathNames.size(); j++)
		{
			QString name=_scene->_fileSelection->pathIDs[j];
			if( ID==_scene->_fileSelection->pathIDs[j] )
			{
				pname= _scene->_fileSelection->pathNames[j];
				//pname=path+pname;
				break;
			}
		}
	    //pname=fname;
        openPathwayByPath( item, NULL, 400, x, y, fname, pname, "");
	}
	else
	{
	    //_scene->openPathBubblebyMenu(QString name, QString pname, QString lastStoredGraph, ItemBase *item, QPointF scenePos, bool setPos, bool rightSide)
		 vector<set<vector<int>>> AnodeContains;
        item = openSubPathBubble1("", 0, 400, 400, x, y, itemSelected, edgeSelected, AnodeContains, name);		//toplevel?-1:pathwayID KEQIN TEST USING 0 AS PARENT id
	}
	this->_manager->itemPosUpdated(item);
	
	if (item)
	{
		item->resizeItem( sizewidth, sizeheight );
		
		item->recoverFromXmlNode( node );
		return item;
	}

	return NULL;

}


void OpenManager::recoverItemFromXmlNode( QDomNodeList paraNode, set<vector<int>> &item)
{
	QDomNodeList posNode;
	QString str;
	QStringList posStr;	
	vector<int> node(2,0);
	//paraNode = node.elementsByTagName( tr("initEdges") );	
	if ( !paraNode.isEmpty() )
	{
		//paraNode = paraNode.at(0).toElement().elementsByTagName( tr("edge") );		
		paraNode = paraNode.at(0).toElement().elementsByTagName( tr("item") );
		int size=paraNode.size();
		for (int i = 0; i < paraNode.size(); i ++)
		{
			int type0,id0;
			vector<int> temp;

			posNode = paraNode.at(i).toElement().elementsByTagName( tr("item") );
			posStr= paraNode.at(i).toElement().text().split( "," );
			
			str = posStr.at(0);			
			type0 = str.toInt();
			str = posStr.at(1);
			if(str[0]=='-')
			{
				str=str.mid(1,str.size()-1);	
				id0  = str.toInt();
				id0=-id0;
			}
			else
			{
			   id0  = str.toInt();
			}
			node[0]= type0;
			node[1] = id0;						
			item.insert(node);
		}
	}
}

ItemBase* OpenManager::openTreeRingFromXmlNode( QDomElement node, int treeRingID)
{	
	/////////////////////
    QDomElement tmpNode;
	QDomNodeList paraNode;
	set<vector<int>> itemSelected;
	ItemBase *item;
	treeRingBubble *tBubble=NULL;
	bool toplevel = true;
    paraNode = node.elementsByTagName( tr("initItems") );	
	if ( !paraNode.isEmpty() )	
	{
		toplevel=false;
		recoverItemFromXmlNode(paraNode, itemSelected);	
	}

	/////////////////////	
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

	paraNode = node.elementsByTagName( tr("ParentBubble") );
	
	if ( !paraNode.isEmpty() )
	{
		tmpNode = paraNode.at(0).toElement();
		paraNode = tmpNode.elementsByTagName(tr("FileName"));
		QString name = paraNode.at(0).toElement().text();

		paraNode = tmpNode.elementsByTagName( tr("Position") );
		QStringList posStr= paraNode.at(0).toElement().text().split( "," );
		QString str = posStr.at(0);
		str.remove( 0, 1 );
		str = str.simplified();
		float x = str.toFloat();
		str = posStr.at(1);
		str.chop(1);
		str = str.simplified();
		float y  = str.toFloat();
		QList<ItemBase*> itemList = this->_scene->getGroupManager()->allMembers();
		for (int i = 0; i < itemList.size(); i ++)
		{
			item = itemList[i];
			if ( item->getType() == TREERING )
			{
				treeRingBubble* tmp = dynamic_cast<treeRingBubble*>(item);
				if ( tmp->getCurrentFilename() == name)					
				{
					tBubble = tmp;
					break;
				}
			}
		}
	}
		
	
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
	//QString name =  "data/pathways/" + str;
	
	if(toplevel)
	{
		QString fname = str;
		QString pname = str;
		int tindex0 = pname.lastIndexOf("/");	
		//QString path = pname;
		//path.remove(tindex0,path.size()-tindex0);
		pname.remove(0, tindex0+1);
		QString num= pname.remove(pname.size()-1,1);
		int ID=num.toInt();		
		int size=_scene->_fileSelection->pathNames.size();
		for(int j=0; j<_scene->_fileSelection->pathNames.size(); j++)
		{
			QString name=_scene->_fileSelection->pathIDs[j];
			if( ID==_scene->_fileSelection->pathIDs[j] )
			{
				pname= _scene->_fileSelection->pathNames[j];
				//pname=path+pname;
				break;
			}
		}
	    //pname=fname;
		item = openTreeRingBubble(600, x, y);	
        //openTreeRingByPath( item, NULL, 400, x, y, fname, pname, "");
	}
	else
	{
		if(!tBubble)
		{		
		    ItemBase * titem = openTreeRingBubble(600, -1000, -1000);			
			tBubble =  dynamic_cast<treeRingBubble*>(titem);		
			item = openSubTreeRing(tBubble, 400, x, y, itemSelected);	
		    _scene->bubbleDeleted(titem);	
		    titem->deleteSelectedItems(_scene);
		}
		else
		{
			item = openSubTreeRing(tBubble, 400, x, y, itemSelected);	
		}
		//openSubTreeRingBubble(500, x, y);	
        //item = openSubPathBubble1("", treeRingID, 400, 400, x, y, itemSelected, edgeSelected, name);		
	}
	this->_manager->itemPosUpdated(item);	
	if (item)
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

ItemBase* OpenManager::openTextBubble2FromXmlNode( QDomElement node )
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
	item = openTextBubble2(-1, -1, sizewidth, x, y);
	
	if (item)
	{
		item->resizeItem( sizewidth, sizeheight );
		
		item->recoverFromXmlNode( node );
		return item;
	}

	return NULL;
}

ItemBase* OpenManager::openReactionBubble3FromXmlNode( QDomElement node )
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
	/*item = openReactionBubble3(sizewidth, x, y);
	
	if (item)
	{
		item->resizeItem( sizewidth, sizeheight );		
		item->recoverFromXmlNode( node );
		return item;
	}
	*/
	return NULL;
}
ItemBase* OpenManager::openGroupBubble4FromXmlNode( QDomElement node )
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
	
	/*item = openGroupBubble4(sizewidth, x, y);
	//openGroupBubble4(int size, int x, int y, vector<int> itemSelected, vector<vector<int>> edgeSelected, QString name)
	
	if (item)
	{
		item->resizeItem( sizewidth, sizeheight );
		
		item->recoverFromXmlNode( node );
		return item;
	}
	*/
	return NULL;
}



/*ItemBase* OpenManager::openMatlabFromXmlNode( QDomElement node )
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
}*/

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
	item = openNote(NULL, sizewidth, x, y);

	if (item)
	{
		item->resizeItem( sizewidth, sizeheight );
		item->recoverFromXmlNode( node );
		return item;
	}

	return NULL;
}