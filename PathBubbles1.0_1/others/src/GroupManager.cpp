#include "GroupManager.h"
#include "qtbox.h"
#include "openglscene.h"

GroupManager::GroupManager( OpenGLScene* scene ) : QObject()
,  _ONMOVING( false )
{
	this->_scene = scene;
	this->_highLightColor.a = 150;
	this->_highLightColor.b = 150;
	return;
}

void GroupManager::logAddToGroup( QList<ItemBase*> newMembers, int index )
{
	QString logtext;

	logtext.append( newMembers[0]->headofLogMsg() );
	logtext.append( "Add bubbles ( ");
	for (int i = 0; i < newMembers.size(); i ++)
	{
		logtext.append( newMembers[i]->BubbleIdentifier() );
		if ( i < newMembers.size()-1 )
		{
			logtext.append( ". " );
		}
	}
	logtext.append( " )" );
	logtext.append( " to the group ( " );

	for (int i = 0; i < _list[index]->NumOfMembers(); i ++)
	{
		logtext.append( _list[index]->getMemberAt(i)->BubbleIdentifier() );
		if ( i < _list[index]->NumOfMembers()-1 )
		{
			logtext.append( ", " );
		}
	}
	logtext.append( ")\n" );

	_scene->addLogText( logtext );
}

void GroupManager::logRemoveItem( ItemBase* item, int index )
{
	QString logtext;
	
	logtext.append( item->headofLogMsg() );
	logtext.append( "Remove bubble ");
	logtext.append( item->BubbleIdentifier() );
	logtext.append( " from the group ( " );

	for (int i = 0; i < _list[index]->NumOfMembers(); i ++)
	{
		logtext.append( _list[index]->getMemberAt(i)->BubbleIdentifier() );
		if ( i < _list[index]->NumOfMembers()-1 )
		{
			logtext.append( ", " );
		}
	}
	logtext.append( ")\n" );

	_scene->addLogText( logtext );
}

void GroupManager::itemPosUpdated( ItemBase* init )
{
	if ( init == NULL )	return;

	//			unhighlight
	if( this->_selected.contains( init ) )
	{
	}
	else
	{
		unHightLightAll();
		this->_selected.clear();
	}

	int type = init->getType();
	if(type > -1 && ( type != COLOR || type != COMPLOT ))
	{
		this->findOverlap( init );
	}
	this->_ONMOVING = false;
	return;
}

void GroupManager::itemPosMoved(ItemBase *init, QPointF offset)
{
	//
	findAndHighlight( init );
	//
	this->_ONMOVING = true;
	//
	int gIndex = this->findGroupIndex(init);
	if (gIndex > -1)
	{
		this->_list[gIndex]->moveOtherMember(init, offset);
	}

	return;
}

void GroupManager::addBatsToScene( QList<QList<ItemBase*>> bats )
{
	for( int i = 0; i < bats.size(); i++ )
	{
		for( int j = 0; j < bats[i].size(); j++ )
		{
			this->newItemAdded(bats[i][j]);
		}
	}
}

void GroupManager::findOverlap( ItemBase* init )
{
	//			all items
	QList<ItemBase*> members = allMembers();
	if(members.isEmpty()) { return;} //J.C.
	//			current group members
	QList<ItemBase*> items = getCurrentItems( init );

	//			overlap members
	QList<ItemBase*> needToAdd;
	//			find overlapped member
	for( int i = 0; i < members.size(); i++ )
	{
		for( int j = 0; j < items.size(); j++ )
		{
			if(members[i] == items[j])
			{
				continue;
			}
  			if( getCurrentItems( members[i] ).size() == 1 && needCompare(  members[i], items[j] ) )
  			{
  				setCompareWithOutPlot( members[i], items[j] );
  			}
			if( Overlaied( members[i], items[j] ) )
			{
				if( !isInThisList( needToAdd, members[i]) )
				{
					needToAdd.append( members[i] );
				}
			}
		}
	}
	this->addToGroup( needToAdd, init );
	return;
}

void GroupManager::setCompareWithOutPlot( ItemBase* target, ItemBase* init )
{
	return;
	QtBat* tar = dynamic_cast<QtBat*>( target );
	QtBat* ini = dynamic_cast<QtBat*>( init );
	tar->acceptNewBat( ini );
	delete ini->getControl();
	ini->properRemove();
	delete ini;
	return;
}

bool GroupManager::oneOverAnother( ItemBase *left, ItemBase *right )
{
	//int dis = TDHelper::LengthOfQPointF( left->pos() - right->pos() );
	QPointF dis = left->pos() - right->pos();
	int threshhold = TDHelper::Min( left->Width(), right->Width() ) * 0.2;
	int threshhold2 = TDHelper::Min( left->Height(), right->Height() ) * 0.2;
	if( abs(dis.x()) < threshhold || abs(dis.y()) < threshhold2 )
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool GroupManager::needCompare(ItemBase *left, ItemBase *right)
{
	//			two bats overlaied
	int typeLeft = left->getType();
	int typeRight = right->getType();
	if(  typeLeft != BAT || typeRight != BAT || typeLeft == typeRight )
	{
		return false;
	}
	//			determine size of overlaied
	//int dis = TDHelper::LengthOfQPointF( left->pos() - right->pos() );
	QPointF dis = left->pos() - right->pos();
	int threshhold = TDHelper::Min( left->Width(), right->Width() ) * 0.2;
	int threshhold2 = TDHelper::Min( left->Height(), right->Height() ) * 0.2;
	if( abs(dis.x()) < threshhold || abs(dis.y()) < threshhold2 )
	{
		return true;
	}
	else
	{
		return false;
	}
}

void GroupManager::addToGroup( ItemBase* add, ItemBase* init )
{
	QList<ItemBase*> addlist;
	addlist.append( add );

	addToGroup( addlist, init );
}

void GroupManager::addToGroup(QList<ItemBase*> newMembers, ItemBase *init)
{
	if (newMembers.size() == 0)	
	{
		return;
	}

	//			in which group
	int index = findGroupIndex( init );

	bool isSameGroup = true;
	for ( int i = 1; i < newMembers.size(); i ++ )
	{
		if ( ! sameGroup( newMembers[i-1], newMembers[i] ) )
		{
			isSameGroup = false;
			break;
		}
	}

	logAddToGroup( newMembers, index );
	QList<Group*> removeList;

	if ( newMembers.size() > 1 && isSameGroup )
	{
		int addedIndex = findGroupIndex( newMembers[0] );
		this->_list[addedIndex]->groupCombine( this->_list[index] );
		removeList.append( _list[index] );
	}
	else
	{
		for( int i = 0; i < newMembers.size(); i++ )
		{
			for( int j = 0; j < this->_list.size(); j++ )
			{
				if ( removeList.contains( _list[j] ) )
				{
					continue;
				}
				if( this->_list[j]->inThisGroup( newMembers[i]) ) 
				{
					if( index != j )
					{
						this->_list[index]->groupCombine( _list[j] );
						if( !removeList.contains( _list[j] ) )
						{
							removeList.append( _list[j] );
						}
					}
				}
			}
		}
	}
	
	for( int i = 0; i < removeList.size(); i++ )
	{
		this->_list.removeAll( removeList[i] );
	}

	return;
}

bool GroupManager::isInThisList( QList<ItemBase*> list, ItemBase* init )
{
	for( int i = 0; i < list.size(); i++ )
	{
		if( list[i] == init )
		{
			return true;
		}
	}
	return false;
}

bool GroupManager::Overlaied(ItemBase *left, ItemBase *right)
{
	int disX = abs( left->pos().x() - right->pos().x() );
	int disY = abs( left->pos().y() - right->pos().y() );
	int thresh = ( left->Width() + right->Width() ) / 2;
	int thresh2 = ( left->Height() + right->Height() ) / 2;
	if( disX < thresh && disY < thresh2 )
	{
		return true;
	}
	return false;
}

void GroupManager::newItemAdded( ItemBase* item )
{
	Group* newGrp = new Group();
	newGrp->addToThisGroup( item );
	_list.append( newGrp );
	return;
}

int GroupManager::findGroupIndex( ItemBase* init )
{
	int index = -1;
	for( int i = 0; i < this->_list.size(); i++ )
	{
		if( _list[i]->inThisGroup( init ) )
		{
			index = i;
			break;
		}
	}
	return index;
}

QList<ItemBase*> GroupManager::getCurrentItems( ItemBase* init )
{
	//			in which group
	int index = findGroupIndex( init );

	//			other objects in this group
	QList<ItemBase*> curItems;
	if( index == -1 )
	{
		return curItems;
	}
	for( int i = 0; i < this->_list[index]->NumOfMembers(); i++)
	{
		curItems.append( _list[index]->getMemberAt( i ) );
	}
	return curItems;
}

QList<ItemBase*> GroupManager::allMembers()
{
	QList<ItemBase*> result; //J.C.
	ItemBase *base;

	for (int i = 0; i < this->_list.size(); i ++)
	{
		for (int j = 0; j < this->_list[i]->NumOfMembers(); j ++)
		{
			base = this->_list[i]->getMemberAt(j) ;
			if ( base )
			{
				result.append( base );
			}
		}
	}
	return result;
}

QList<Group*> GroupManager::groupList()
{
	return _list;
}

void GroupManager::itemRemoved( ItemBase *item )
{
	this->removeItem( item );
	return;
}

void GroupManager::removeItem( ItemBase* item )
{
	ItemBase* tmpItem;
	MatBlock* mat;
	QList< MatBlock* > matBubbles;

	if (item->getType() == MATLAB)
	{
		mat = dynamic_cast<MatBlock*>(item);
		mat->getCodeParent()->removeCodeCon( mat );
		mat->removeAllCodeSon();

		mat->removeDataVis();
	}

	int index = this->findGroupIndex( item );
	if( index < 0)
	{
		return;
	}

	logRemoveItem( item, index );

	this->_list[index]->removeItem( item );
	if( this->_list[index]->NumOfMembers() == 0 )
	{
		delete this->_list[index];
		this->_list.removeAt( index );
	}

	return;
}

void GroupManager::unGroup(ItemBase *init)
{
	int index = findGroupIndex( init );
	if( index < 0 || this->_list[index]->NumOfMembers() == 1 )
	{
		return;
	}
	this->_list[index]->removeItem( init );
	newItemAdded( init );
	init->setColor( TDHelper::getBoarderColor() );
	if ( init->getLabel() != NULL )
	{
		init->getLabel()->show();
	}
	return;
}

void GroupManager::unHightLightAll()
{
	for( int i = 0; i < this->_list.size(); i++ )
	{
		this->_list[i]->unHightLight();
	}
	return;
}

void GroupManager::findAndHighlight( ItemBase* init )
{
	//
	unHightLightAll();
	//
	QList<ItemBase*> members = allMembers();
	if(members.isEmpty()) 
	{ 
		return;
	}
	QList<ItemBase*> items = getCurrentItems( init );
	QList<ItemBase*> needToAdd;
	for( int i = 0; i < members.size(); i++ )
	{
		for( int j = 0; j < items.size(); j++ )
		{
			if( members[i] == items[j] || sameGroup( members[i], items[j] ) )
			{
				continue;
			}
			else if( Overlaied( members[i], items[j] ) )
			{
				if( !isInThisList( needToAdd, members[i]) )
				{
					needToAdd.append( members[i] );
				}
			}
		}
	}
	//
	this->highLightGroup( needToAdd, init );
	return;
}

bool GroupManager::sameGroup( ItemBase* left, ItemBase* right )
{
	int group1 = findGroupIndex( left );
	int group2 = findGroupIndex( right );
	if( group1 == group2 )
	{
		return true;
	}
	else
	{
		return false;
	}
}

void GroupManager::highLightGroup(QList<ItemBase*> newMembers, ItemBase* init )
{
	for( int i = 0; i < newMembers.size(); i++ )
	{
		for( int j = 0; j < this->_list.size(); j++ )
		{
			if( this->_list[j]->inThisGroup( newMembers[i]) )
			{
				this->_list[j]->highLight( init->getColor() );
			}
		}
	}
	return;
}

void GroupManager::acceptSelectedArea( QRectF rect )
{
	unHightLightAll();
	this->_selected.clear();
	//
	QList<ItemBase*> list = this->allMembers();
	for( int i = 0; i < list.size(); i++ )
	{
		if( rect.contains( list[i]->pos() ) )
		{
			_selected.append( list[i] );
			list[i]->hightLight( _highLightColor );
		}
	}
	return;
}

//void GroupManager::mousePressed(QGraphicsSceneMouseEvent* event)
//{
//	for( int i = 0; i < this->_selected.size(); i++ )
//	{
//		QPointF newPos = event->pos() + _selected[i]->pos();
//		QMouseEvent event( QEvent::MouseButtonPress, QPoint( newPos.x(), newPos.y()), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
//		QApplication::sendEvent( this->_scene, &event);
//	}
//	return;
//}
//
//void GroupManager::mouseMoved(QGraphicsSceneMouseEvent* event)
//{
//	for( int i = 0; i < this->_selected.size(); i++ )
//	{
//		QPointF newPos = event->pos() + _selected[i]->pos();
//		QMouseEvent event(QEvent::MouseMove, QPoint( newPos.x(), newPos.y()), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
//		QApplication::sendEvent( this->_scene, &event);
//	}
//	return;
//}
//
//void GroupManager::mouseReleased(QGraphicsSceneMouseEvent* event)
//{
//	for( int i = 0; i < this->_selected.size(); i++ )
//	{
//		QPointF newPos = event->pos() + _selected[i]->pos();
//		QMouseEvent event(QEvent::MouseButtonRelease, QPoint( newPos.x(), newPos.y()), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
//		QApplication::sendEvent( this->_scene, &event);
//	}
//	return;
//}

void GroupManager::updateSelectBats( ItemBase* bat, QPointF offset )
{
	int gindex = this->findGroupIndex( bat );

	_list[gindex]->updateGroupRotation( offset );

	return;
}

void GroupManager::swapSelectPCs( SM* sm, int source, int target )
{
	int tindex;
	ItemBase *item;
	QtSM* pc;
	QList<ItemBase*> list = this->allMembers();
	
	for (int i = 0; i < list.size(); i ++)
	{
		item = list.at(i);
		if ( item && item->getType()==PC )
		{
			pc = dynamic_cast<QtSM*>(item);
			if (pc->getSM() == sm)
			{
				break;
			}
		}
	}

	tindex = findGroupIndex( item );
	this->_list[tindex]->swapPCBars( source, target );

	return;
}

void GroupManager::tightenItemsinSameGroup()
{
	for (int i = 0; i < this->_list.size(); i ++)
	{
		this->_list[i]->tightenItems();
	}
	return;
}

void GroupManager::disperseItemsinSameGroup()
{
	for (int i = 0; i < this->_list.size(); i ++)
	{
		this->_list[i]->disperseItems();
	}
	return;
}

void GroupManager::sliderChanged(ItemBase *bat, int number )
{
	float rate = float( number ) / 100.0;
	int tindex = findGroupIndex( bat );
	if (tindex > -1)
	{
		this->_list[tindex]->setBatGroupFrameState( rate, number );
	}
	return;
}

void GroupManager::surfaceChecked(ItemBase *bat, int state )
{
	int tindex = findGroupIndex( bat );
	if (tindex > -1)
	{
		this->_list[tindex]->setBatGroupSurfaceState( state );
	}
	return;
}

void GroupManager::wireframeChecked(ItemBase *bat, int state )
{
	int tindex = findGroupIndex( bat );
	if (tindex > -1)
	{
		this->_list[tindex]->setBatGroupWireframeState( state );
	}
	return;
}

void GroupManager::symbolChecked(ItemBase *bat, int state)
{
	int tindex = findGroupIndex( bat );
	if (tindex > -1)
	{
		this->_list[tindex]->setBatGroupSymbolState( state );
	}
	return;
}

void GroupManager::camber1Checked(ItemBase *bat, int state )
{
	int tindex = findGroupIndex( bat );
	if (tindex > -1)
	{
		this->_list[tindex]->setBatGroupCamberState( 0, state );
	}
	return;
}
void GroupManager::camber2Checked(ItemBase *bat, int state )
{
	int tindex = findGroupIndex( bat );
	if (tindex > -1)
	{
		this->_list[tindex]->setBatGroupCamberState( 1, state );
	}
	return;
}
void GroupManager::camber3Checked(ItemBase *bat, int state )
{
	int tindex = findGroupIndex( bat );
	if (tindex > -1)
	{
		this->_list[tindex]->setBatGroupCamberState( 2, state );
	}
	return;
}

void GroupManager::updateWingShapeParainGroup( ItemBase* bat, ArrowGen* agen )
{
	int tindex = findGroupIndex( bat );
	if (tindex > -1)
	{
		this->_list[tindex]->updateBatGroupWingShapePara( agen );
	}
	return;
}

void GroupManager::HighlightChangeinGroup( ItemBase* item, bool sta )
{
	int tindex = findGroupIndex( item );
	if (tindex > -1)
	{
		this->_list[tindex]->setPCHighlightState( sta );
	}
	return ;
}

void GroupManager::pressHightlightinGroup( ItemBase *item, QRect rect )
{
	int tindex = findGroupIndex( item );
	if ( tindex > -1 )
	{
		this->_list[tindex]->pressHightlight( rect );
	}
	return;
}

void GroupManager::moveHightlightinGroup( ItemBase *item, QRect rect )
{
	int tindex = findGroupIndex( item );
	if ( tindex > -1 )
	{
		this->_list[tindex]->moveHightlight( rect );
	}
	return;
}

void GroupManager::releaseHightlightinGroup( ItemBase *item )
{
	int tindex = findGroupIndex( item );
	if ( tindex > -1 )
	{
		this->_list[tindex]->releaseHighlight();
	}
	return;
}

void GroupManager::updateLabelState(ItemBase *init)
{
	int tindex = findGroupIndex( init );
	if (tindex > -1)
	{
		this->_list[tindex]->updateGroupLabelState();
	}	
	return;
}

void GroupManager::updatePCColorState( ItemBase* init, int input )
{
	int index = findGroupIndex( init );
	if ( index > -1 )
	{
		this->_list[index]->updateGroupPCColorState( input );
	}
	return ;
}

void GroupManager::updateItemsPos()
{
	this->disperseItemsinSameGroup();
	//this->tightenItemsinSameGroup();
}

void GroupManager::addNewGroupFromXmlNode( QDomElement node )
{
	QDomElement childElement, tmpPara;
	QList<QDomElement> ComNodeList;

	QDomNodeList para;
	QString type;

	ComNodeList.clear();
	childElement = node.firstChildElement();
	while ( !childElement.isNull())
	{
		if (childElement.tagName() == "Bubble")
		{
			ItemBase* item; 
			para = childElement.elementsByTagName( tr("Type") );
			if ( para.size() == 1)
			{
				tmpPara = para.at(0).toElement();
				type = tmpPara.text();

				if ( type == "Bat" )
				{
					item = this->_scene->getOpenManager()->openBatFromXmlNode( childElement );
				}
				else if ( type == "Video" )
				{
					item = this->_scene->getOpenManager()->openVideoFromXmlNode( childElement );
				}
				else if ( type == "Web" )
				{
					item = this->_scene->getOpenManager()->openWebFromXmlNode( childElement );
				}
				else if ( type == "Bar Chart" )
				{
					item = this->_scene->getOpenManager()->openBarChartFromXmlNode( childElement );
				}
				else if ( type == "Line Chart" )
				{
					item = this->_scene->getOpenManager()->openLineChartFromXmlNode( childElement );
				}
				else if ( type == "Scatter Plot" )
				{
					item = this->_scene->getOpenManager()->openScatterFromXmlNode( childElement );
				}
				else if ( type == "ComPlot" )
				{
					ComNodeList.append( childElement );
				}
				else if ( type == "Matlab" )
				{
					item = this->_scene->getOpenManager()->openMatlabFromXmlNode( childElement );
				}
				else if ( type == "Note" )
				{
					item = this->_scene->getOpenManager()->openNoteFromXmlNode( childElement );
				}
			}
		}
		childElement = childElement.nextSiblingElement();
	}

	for (int i = 0; i < ComNodeList.size(); i ++)
	{
		childElement = ComNodeList.at(i);
		this->_scene->addLComObjectFromXmlNode( childElement );
	}
}

void GroupManager::drawAllGroupBoarder(QPainter *painter)
{
	for (int i = 0; i < this->_list.size(); i ++)
	{
		this->_list[i]->drawGroupBoarder( painter );
	}
	return;
}

QtBat* GroupManager::getBatLinkWithPC( QtSM* item )
{
	int gIndex = this->findGroupIndex( item );
	return this->_list[gIndex]->getBatLinkWithPC( item );
}

QtBat* GroupManager::getBatLinkWithVideo( VideoBlock *item )
{
	int gIndex = this->findGroupIndex( item );
	return this->_list[gIndex]->getBatLinkWithVideo( item );
}