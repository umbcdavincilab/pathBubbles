#include "Group.h"
#include "math.h"

Group::Group()
{
	layoutType = LINEUP;
	return;
}

Group::~Group()
{
	_members.clear();
	lineUpList.clear();
}

void Group::addToThisGroup(ItemBase *item, bool updatePos)
{
	this->_members.append( item );
	if (this->_members.size() == 1)
	{
		QList<int> tmp;
		tmp.append(0);
		this->lineUpList.append( tmp );
		if(updatePos)
		   item->mySetPos( item->pos() );
		updateBoarderState();
	}
}

bool Group::inThisGroup(ItemBase *item)
{
	for( int i = 0; i < this->_members.size(); i++ )
	{
		if( this->_members[i] == item )
		{
			return true;
		}
	}
	return false;
}

void Group::moveAssociateNodeNotes(ItemBase *init, QList<CalloutNote*> nodeNotes, QPointF offset)
{
	for( int i = 0; i <nodeNotes.size(); i++ )
	{
		int pid=nodeNotes[i]->_pid;
		if( !nodeNotes[i]->deleted )
		{
			for( int j = 0; j < this->_members.size(); j++ )
			{
				if(pid==this->_members[j]->m_pathwayID)
					nodeNotes[i]->mySetPos(nodeNotes[i]->pos()+offset);
			}
		}
	}
	return;
}

void Group::updateSharedSet(ItemBase *init)
{//update after ungrouped or delet
	//vector<set<vector<int>>> sharedSets; 
	if(!init)
		return;
	int pid = init->m_pathwayID;
	if(pid<0)
		return;
		
	for(int i=0; i<sharedSets.size(); i++)
	{
		set<vector<int>> tempSet=sharedSets[i];
	    for(set<vector<int>>::iterator it=sharedSets[i].begin(); it!=sharedSets[i].end(); it++)
		{
			vector<int> item = *it;
		    if(item[0]==pid)
			{
			   	tempSet.erase(item);	
			}
		}
		sharedSets[i]=tempSet;
	}
}

void Group::moveOtherMember(ItemBase *init, QPointF offset)
{
	for( int i = 0; i < this->_members.size(); i++ )
	{
		if( this->_members[i] != init )
		{
			this->getMemberAt(i)->mySetPos( QPointF(_members[i]->pos().x() + offset.x(), _members[i]->pos().y() + offset.y()) );
		}
	}
	return;
}

int Group::NumOfMembers()
{
	return this->_members.size();
}

int Group::getMemberID( ItemBase* member)
{
	for( int i = 0; i < this->_members.size(); i++ )
	{
		if( this->_members[i] == member )
		{
			return i;
		}
	}
	return -1;
}

ItemBase* Group::getMemberAt( int index )
{
	return this->_members[index];
}

QList<ItemBase*> Group::getMembers(  )
{
	return this->_members;
}

void Group::groupCombine( Group* target, bool updatePos) //combine group and alos re-assign color
{
	int indexbegin, indexend;	
	set<int> init, initF;
	indexbegin = this->NumOfMembers();
	set<ItemBase *> addedList;
	for( int i = 0; i < target->NumOfMembers(); i++ )
	{
		if( !this->inThisGroup( target->getMemberAt(i) ) )
		{
			this->addToThisGroup( target->getMemberAt(i), updatePos );			
			addedList.insert(target->getMemberAt(i));			
		}
	}
	
	myColor3 color=getGroupColor(target);
	for( int i = 0; i < this->NumOfMembers(); i++ )
	{
		if( this->getMemberAt(i) )
		{
			this->getMemberAt(i)->setColor( color );
			if(addedList.find(this->getMemberAt(i)) == addedList.end())
				init.insert(i);
		}
	}

	indexend = this->NumOfMembers();
 	for (int i = 0; i < this->NumOfMembers(); i ++)
 	{
		if(updatePos)
 		   this->getMemberAt(i)->mySetPos( this->getMemberAt(i)->pos() );
		ItemBase *item = this->getMemberAt(i);
		item = item;
 	}

	updateLayoutType();

	updateLineUpListAdd( indexbegin, indexend );
	
	if(updatePos)
	if(this->layoutType == LINEUP)
	{
		//put related bubbles (top pathbubble with sub-pathbubble) into family unit		
		
		/*vector<set<int> > familySets = GroupToFamily(this->_members);
		for(set<int>::iterator it=init.begin(); it!=init.end(); it++)
		{
		    initF.insert( getFamilyID(*it, familySets) ); 		
		}		

		vector<QRectF> rectList = assignRectList(this->_members);
		if(isOverlaid( rectList))
		{
			rectList =	lineUpDisplay(this->_members, initF, indexend, familySets, rectList, this->getMemberAt(0)->screenWidth, this->getMemberAt(0)->screenHeight);		
			for(int i=0; i<indexend; i++)
			{
				//this->getMemberAt(i)->setTargetPos( QPointF(rectList[i].x(),rectList[i].y()) );				
				//if(rectList[i].x()>-10000 && rectList[i].x()<10000)
				   this->getMemberAt(i)->mySetPos( QPointF(rectList[i].x()-getMemberAt(i)->realRect().x(),rectList[i].y()-getMemberAt(i)->realRect().y()) );
			}
		}*/
	}
	updateGroupLabelState();
	return;
}

set<int> Group::groupCombine( QList<Group* > targetGroups, bool updatePos) //combine group and alos re-assign color
{
	int indexbegin, indexend;	
	set<int> init;
	indexbegin = this->NumOfMembers();
	set<ItemBase *> addedList;
	for(int j=0; j<targetGroups.size(); j++)
	{
		Group* target=targetGroups[j];
		for( int i = 0; i < target->NumOfMembers(); i++ )
		{
			if( !this->inThisGroup( target->getMemberAt(i) ) )
			{
				this->addToThisGroup( target->getMemberAt(i), updatePos );			
				addedList.insert(target->getMemberAt(i));			
			}
		}
	}

	myColor3 color=getGroupColor(this);
		for( int i = 0; i < this->NumOfMembers(); i++ )
		{
			if( this->getMemberAt(i) )
			{	
				this->getMemberAt(i)->setColor( color );
				if(addedList.find(this->getMemberAt(i)) == addedList.end())
					init.insert(i);
			}
		}

	indexend = this->NumOfMembers();
 	for (int i = 0; i < this->NumOfMembers(); i ++)
 	{
		if(updatePos)
 			this->getMemberAt(i)->mySetPos( this->getMemberAt(i)->pos() );
		ItemBase *item = this->getMemberAt(i);
		item = item;
 	}

	updateLayoutType();
	updateLineUpListAdd( indexbegin, indexend );	
	/*if(updatePos)
	if(this->layoutType == LINEUP)
	{
		vector<set<int> > familySets;	   
	}*/
	updateGroupLabelState();
	return init;
}

vector<QRectF> Group::assignSceneRectList(QList<ItemBase *> members, bool bigRect, int start)
{
	//for(int i=0; i<size; i++)
	vector<QRectF> rectList;
	for(int i=start; i<members.size(); i++)
	{
		QRectF tRect = members[i]->sceneBoundingRect();//this->getMemberAt(i)->sceneBoundingRect();
		QRectF boundingRect=QRectF(tRect.x(), tRect.y(), tRect.width(), tRect.height());
		QRectF realRect = members[i]->realRect();
		QRectF sceneRect1 =QRectF(boundingRect.x()+(boundingRect.width()-realRect.width())/2, boundingRect.y()+(boundingRect.height()-realRect.height())/2, realRect.width(), realRect.height());
		QRectF sceneRect =QRectF(members[i]->pos().x()- members[i]->Width()/2,
								members[i]->pos().y()- members[i]->Height()/2,
								 members[i]->Width(),  members[i]->Height());
		ItemBase *item=members[i];		
		if(bigRect)
		{
			rectList.push_back(boundingRect);			
		}
		else 
		{
			//rectList.push_back(QRectF(boundingRect.x()+(boundingRect.width()-realRect.width())/2, boundingRect.y()+(boundingRect.height()-realRect.height())/2, realRect.width(), realRect.height()));			
			rectList.push_back(sceneRect);			
		}				
	}
	return rectList;
}


vector<QRectF> Group::assignRectList(QList<ItemBase *> members, bool bigRect, int start)
{
	//for(int i=0; i<size; i++)
	vector<QRectF> rectList;
	for(int i=start; i<members.size(); i++)
	{
		//QRectF mRect = members[i]->sceneBoundingRect(); //members[i]->sceneBoundingRect();//this->getMemberAt(i)->sceneBoundingRect();
		QRectF sceneRect = QRectF(members[i]->pos().x()-members[i]->Width()/2.0, members[i]->pos().y()-members[i]->Height()/2.0, members[i]->Width(), members[i]->Height());//members[i]->sceneBoundingRect(); //members[i]->sceneBoundingRect();//this->getMemberAt(i)->sceneBoundingRect();
        QRectF sceneRect1=QRectF(sceneRect.x()-20, sceneRect.y()-20, sceneRect.width()+40, sceneRect.height()+40);
		QRectF sceneRect2=QRectF(sceneRect.x()-10, sceneRect.y()-10, sceneRect.width()+20, sceneRect.height()+20);
		//QRectF realRect = members[i]->realRect();
		//if(tRect.width()!=realRect.width() || tRect.height()!=realRect.height() || mRect.center()!=tRect.center())
		//mRect=mRect;

		//QRectF sceneRect1 =QRectF(boundingRect.x()+(boundingRect.width()-realRect.width())/2, boundingRect.y()+(boundingRect.height()-realRect.height())/2, realRect.width(), realRect.height());
		//QRectF sceneRect =QRectF(members[i]->pos().x()- members[i]->Width()/2,
		//members[i]->pos().y()- members[i]->Height()/2,
		//members[i]->Width(),  members[i]->Height());
		ItemBase *item=members[i];		
		if(bigRect) 
		{
			rectList.push_back(sceneRect1);			
		}
		else if( members[i]->getType() == TEXTBUBBLE2 )
		{
		    rectList.push_back(sceneRect2);
		}
		else
		{
			//rectList.push_back(QRectF(boundingRect.x()+(boundingRect.width()-realRect.width())/2, boundingRect.y()+(boundingRect.height()-realRect.height())/2, realRect.width(), realRect.height()));			
			rectList.push_back(sceneRect);			
		}				
	}
	return rectList;
}


myColor3 Group::getGroupHighlightColor(ItemBase *topNode1 )
{
	//find the color of the toplevel bubble
	//myColor3 _topColor = target->_members[0]->getColor();
	ItemBase *topNode2=this->getMemberAt(0);
	for( int i = 1; i < this->NumOfMembers(); i++ )
	{
		topNode2=compareTopNodeType(topNode2, this->getMemberAt(i));
	}
	topNode1=compareTopNodeType(topNode1, topNode2);
    return TDHelper::getBoarderColorByType(topNode1->getType());
}


myColor3 Group::getGroupColor(Group* target )
{
	//find the color of the toplevel bubble
	//myColor3 _topColor = target->_members[0]->getColor();
	ItemBase *topNode1=target->_members[0];
	for(int i = 1; i < target->_members.size(); i++)
	{
		topNode1=compareTopNodeType(topNode1, target->_members[i]);	    
	}
	ItemBase *topNode2=this->getMemberAt(0);
	for( int i = 1; i < this->NumOfMembers(); i++ )
	{
		topNode2=compareTopNodeType(topNode2, this->getMemberAt(i));
	}
	topNode1=compareTopNodeType(topNode1, topNode2);
	
	int type=topNode1->getType();
	myColor3 color;
	if(type == PATHBUBBLE1 || type == SUBPATHBUBBLE1)
	{
		if(topNode1->toplevel)
			color=TDHelper::getBoarderColorByType(PATHBUBBLE1, topNode1->m_pathwayID);
		else
			color=TDHelper::getBoarderColorByType(SUBPATHBUBBLE1, topNode1->m_pathwayID);
	}
	else if (type == EXPRESSIONBUBBLE)
	{
	    color=TDHelper::getBoarderColorByType(EXPRESSIONBUBBLE, topNode1->m_expressionID);
	}
	else if (type == TREERING)
	{
		//treeRingBubble* tmp1 = dynamic_cast<treeRingBubble*>(topNode1);		
		color=TDHelper::getBoarderColorByType(TREERING, topNode1->m_treeRingBubbleID);	    
	}
	else 
		color=TDHelper::getBoarderColorByType(type);

	return color;
}




ItemBase *Group::compareTopNodeType(ItemBase *item1, ItemBase *item2)
{
    if( item1->getType() == item2->getType())		
	{
		return item1;
	}	
	int score1=getBubbleLevel(item1), score2=getBubbleLevel(item2);
	if(score1>=score2)
		return item1;
	else return item2;
}

int Group::getBubbleLevel(ItemBase *item)
{
	int type=item->getType(), score=-1;
	if(type== TREERING)
	{
	    if(item->toplevel)
		    score=7;
		else 
		    score=6;	
	}
	else if(type== PATHBUBBLE1 || type==SUBPATHBUBBLE1)
	{
		if(item->toplevel)
		    score=5;
		else 
		score=4;
	}
	else if(type== EXPRESSIONBUBBLE)
		score=3;
	else if(type== TEXTBUBBLE2)
		score=2;
	else 
		score=1;
	
	return score;
}

void Group::removeItem(ItemBase *item)
{
	int index = this->_members.indexOf( item );

	updateLayoutType();
	updateLineUpListRemove( index );
	
	this->_members.removeAt( index );

	updateBoarderState();
	updateGroupLabelState();

	return;
}



void Group::tightenItems()
{
	ItemBase *item;
	QList<ItemBase*> list;
	
	updateLayoutType();

	list = this->_members;

	for (int i = 0; i < this->NumOfMembers(); i ++)
	{
		item = this->getMemberAt(i);
		if (this->layoutType == LINEUP)
		{
			item->adjustPosLineUp(list);
		}
		else if (this->layoutType == RANDOM)
		{
			item->tighten(list);
		}
	}
	return;
}

void Group::disperseItems(vector<bool> &dflag)
{   //dflag: true if still need disperse
	ItemBase *item;
	QList<ItemBase*> list;
	updateLayoutType();
	vector<bool> tflag;
	list = this->_members;

	for (int i = 0; i < this->NumOfMembers(); i ++)
	{
		bool flag=false;
		item = this->getMemberAt(i);
		if(!dflag[i])
		{
			if (this->layoutType == LINEUP)
			{
				flag=item->adjustPosLineUp(list);
			}
			else if (this->layoutType == RANDOM)
			{
				flag=item->disperse(list);
			}
		}
		//tflag.push_back(flag);
	}
	//dflag= tflag;
	return ;
}

void Group::tightenItems(vector<bool> &dflag)
{
	ItemBase *item;
	QList<ItemBase*> list;
	bool flag=false;
	updateLayoutType();
	vector<bool> tflag;
	tflag.resize(this->NumOfMembers());	
	
	list = this->_members;

	for (int i = 0; i < this->NumOfMembers(); i ++)
	{
		//if(!dflag[i])
		flag=false;
		{
			item = this->getMemberAt(i);
			if (this->layoutType == LINEUP)
			{
				flag = item->adjustPosLineUp(list);
			}
			else if (this->layoutType == RANDOM)
			{
				flag = item->tighten(list);
			}
		}
		tflag.push_back(flag);
	}
	dflag=tflag;
	return;
}

void Group::disperseItems()
{
	ItemBase *item;
	QList<ItemBase*> list;
	updateLayoutType();
	
	list = this->_members;

	for (int i = 0; i < this->NumOfMembers(); i ++)
	{
		
		item = this->getMemberAt(i);
		if (this->layoutType == LINEUP)
		{
			item->adjustPosLineUp(list);
		}
		else if (this->layoutType == RANDOM)
		{
			item->disperse(list);
		}	
		
	}
	
	return ;
}

void Group::updateGroupLabelState()
{
	if (this->NumOfMembers() == 0)
	{
		return;
	}
	ItemBase *item,*hasLabel;
	ItemLabel *label;
	QList<ItemBase*> list;

	list = this->_members;

	hasLabel = this->getMemberAt( 0 );

	for (int i = 1; i < this->NumOfMembers(); i ++)
	{
		item = this->getMemberAt( i );
	
		if ((item->pos().x() < hasLabel->pos().x()) || 
			(item->pos().x() == hasLabel->pos().x() && item->pos().y() < hasLabel->pos().y()))
		{
			label = hasLabel->getLabel();
			if (label && label->isVisible())// && item->_TYPE != SUBPATHBUBBLE1)
			{
				if(item->getType()!=SUBPATHBUBBLE1 && item->getType()!=PATHBUBBLE1 )
				    label->hide();
			}

			hasLabel = item;
		}
		else
		{
			label = item->getLabel();
			if (label && label->isVisible())// && item->_TYPE != SUBPATHBUBBLE1)
			{
				if(item->getType()!=SUBPATHBUBBLE1 && item->getType()!=PATHBUBBLE1 )
				   label->hide();
			}
		}
	}

	label = hasLabel->getLabel();
	if (label && (!label->isVisible()))
	{
		label->show();
	}
}
/*
void Group::setBatGroupFrameState(float rate, int number )
{
	QList<ItemBase*> list;
	ItemBase *item;
	QtBat *bat;

	list = this->_members;
	for (int i = 0; i < this->NumOfMembers(); i ++)
	{
		item = this->getMemberAt( i );
		if (item && item->getType() == BAT)
		{
			bat = dynamic_cast<QtBat*>(item);
			bat->setFrame( rate, number );
		}
	}
}

void Group::setBatGroupSurfaceState(bool state)
{
	QList<ItemBase*> list;
	ItemBase *item;
	QtBat *bat;

	list = this->_members;
	for (int i = 0; i < this->NumOfMembers(); i ++)
	{
		item = this->getMemberAt( i );
		if (item && item->getType() == BAT)
		{
			bat = dynamic_cast<QtBat*>(item);
			bat->setSurface( state );
		}
	}
}

void Group::setBatGroupWireframeState( bool state )
{
	QList<ItemBase*> list;
	ItemBase *item;
	QtBat *bat;

	list = this->_members;
	for (int i = 0; i < this->NumOfMembers(); i ++)
	{
		item = this->getMemberAt( i );
		if (item && item->getType() == BAT)
		{
			bat = dynamic_cast<QtBat*>(item);
			bat->setWireframe( state );
		}
	}
}

void Group::setBatGroupCamberState(int camberindex, bool state)
{
	QList<ItemBase*> list;
	ItemBase *item;
	QtBat *bat;

	list = this->_members;
	for (int i = 0; i < this->NumOfMembers(); i ++)
	{
		item = this->getMemberAt( i );
		if (item && item->getType() == BAT)
		{
			bat = dynamic_cast<QtBat*>(item);
			bat->setCamberState( camberindex, state );
		}
	}
}

void Group::setBatGroupSymbolState( bool state )
{
	QList<ItemBase*> list;
	ItemBase *item;
	QtBat *bat;

	list = this->_members;
	for (int i = 0; i < this->NumOfMembers(); i ++)
	{
		item = this->getMemberAt( i );
		if (item && item->getType() == BAT)
		{
			bat = dynamic_cast<QtBat*>(item);
			bat->setSymbol( state );
		}
	}
}

void Group::updateBatGroupWingShapePara( ArrowGen* agen )
{
	QList<ItemBase*> list;
	ItemBase *item;
	QtBat *bat;

	list = this->_members;
	for (int i = 0; i < this->NumOfMembers(); i ++)
	{
		item = this->getMemberAt( i );
		if (item && item->getType() == BAT)
		{
			bat = dynamic_cast<QtBat*>(item);
			bat->updateWingShapePara( agen );
		}
	}
}
*/
void Group::updateGroupRotation( QPointF offset )
{
	QList<ItemBase*> list;
	ItemBase *item;
	//QtBat *bat;

	list = this->_members;
	for (int i = 0; i < this->NumOfMembers(); i ++)
	{
		item = this->getMemberAt( i );
		//if ( item && item->getType() == BAT )
		{
			//bat = dynamic_cast<QtBat*>(item);
			//bat->acceptRotation( offset );
		}
	}
}

void Group::updateGroupPCColorState( int input )
{
	QList<ItemBase*> list;
	ItemBase *item;
	QtSM *pcoor;

	list = this->_members;
	for (int i = 0; i < this->NumOfMembers(); i ++)
	{
		item = this->getMemberAt( i );
		if ( item && item->getType() == PC )
		{
			pcoor = dynamic_cast<QtSM*>(item);
			pcoor->getSM()->setColorState( input );
		}
	}
}

void Group::swapPCBars( int source, int target )
{
	QList<ItemBase*> list;
	ItemBase *item;
	QtSM *pcoor;

	list = this->_members;
	for (int i = 0; i < this->NumOfMembers(); i ++)
	{
		item = this->getMemberAt( i );
		if ( item && item->getType() == PC )
		{
			pcoor = dynamic_cast<QtSM*>(item);
			pcoor->needSwapBars( source, target );
		}
	}
}

void Group::setPCHighlightState( bool state )
{
	QList<ItemBase*> list;
	ItemBase *item;
	QtSM *pcoor;

	list = this->_members;
	for (int i = 0; i < this->NumOfMembers(); i ++)
	{
		item = this->getMemberAt( i );
		if ( item && item->getType() == PC )
		{
			pcoor = dynamic_cast<QtSM*>(item);
			pcoor->setHightlightState( state );
		}
	}
}

void Group::pressHightlight( QRectF rect )
{
	QList<ItemBase*> list;
	ItemBase *item;
	QtSM *pcoor;

	list = this->_members;
	for (int i = 0; i < this->NumOfMembers(); i ++)
	{
		item = this->getMemberAt( i );
		if ( item && item->getType() == PC )
		{
			pcoor = dynamic_cast<QtSM*>(item);
			pcoor->pressHightLight( rect );
		}
	}
}

void Group::moveHightlight( QRectF rect )
{
	QList<ItemBase*> list;
	ItemBase *item;
	QtSM *pcoor;

	list = this->_members;
	for (int i = 0; i < this->NumOfMembers(); i ++)
	{
		item = this->getMemberAt( i );
		if ( item && item->getType() == PC )
		{
			pcoor = dynamic_cast<QtSM*>(item);
			pcoor->moveHightLight( rect );
		}
	}
}

void Group::releaseHighlight()
{
	QList<ItemBase*> list;
	ItemBase *item;
	QtSM *pcoor;

	list = this->_members;
	for (int i = 0; i < this->NumOfMembers(); i ++)
	{
		item = this->getMemberAt( i );
		if ( item && item->getType() == PC )
		{
			pcoor = dynamic_cast<QtSM*>(item);
			pcoor->releaseHighLight();
		}
	}
}

//insertIntoLineUpList( i );
//}

float Group::getMostHighPos(set<int> family, vector<QRectF> rectList)
{
	float max=-1000000;	
	for(set<int>::iterator it=family.begin(); it!=family.end(); it++)
	{
		int temp=rectList[*it].y()+rectList[*it].height();
		if(temp>max)
		{
			max=temp;
		}
	}
	return max;
}

float Group::getMostLeftPos(set<int> family, vector<QRectF> rectList)
{
	float min=1000000;	
	for(set<int>::iterator it=family.begin(); it!=family.end(); it++)
	{
		int temp=rectList[*it].x();
		if(temp<min)
		{
			min=temp;
		}
	}
	return min;
}

float Group::getMostRightPos(set<int> family, vector<QRectF> rectList)
{
	float max=-1000000;	
	for(set<int>::iterator it=family.begin(); it!=family.end(); it++)
	{
		int temp=rectList[*it].x()+rectList[*it].width();
		if(temp>max)
		{
			max=temp;
		}
	}
	return max;
}



float Group::getMostLowPos(set<int> family, vector<QRectF> rectList)
{
	float min=1000000;	
	for(set<int>::iterator it=family.begin(); it!=family.end(); it++)
	{
		int temp=rectList[*it].y();
		if(temp<min)
		{
			min=temp;
		}
	}
	return min;
}

int Group::getFamilyID(int id, vector<set<int> > familySets)
{
	for(int i=0; i<familySets.size(); i++)
	{
		if(familySets[i].find(id)!=familySets[i].end())
		   return i;
	}
	return -1;
}

vector<QRectF> Group::getFamiltyRects(int fid, set<int> familySet, vector<QRectF> rectList)
{
	vector<QRectF> result;
	for(set<int>::iterator it=familySet.begin(); it!=familySet.end(); it++)
	{
		result.push_back(rectList[*it]);
	}
	return result;
}

bool Group::isSameFamily(int id1, int id2, vector<set<int> > familySets)
{
	int fid1= getFamilyID(id1, familySets);
	int fid2= getFamilyID(id2, familySets);
	if(fid1!=-1 && fid1==fid2)
		return true;
	return false;
}

vector<int> Group::findNeighbor(int i, QList<ItemBase *> members, set<int> initF, vector<set<int> > familySets)
{//find the neibour before move
	set<int> tmpNei;
	vector<int> nei;
	bool isNeighbor=false;
	for(int j=0; j<familySets.size(); j++)
	{
		  if( i!=j) //initF.find(j)==initF.end() 
		  {
				if(tmpNei.find(j)==tmpNei.end())
				{	
					for(set<int>::iterator it1=familySets[i].begin(); it1!=familySets[i].end(); it1++) 
					{
						int id1=*it1;
						ItemBase *node1 = members[id1];
						for(set<int>::iterator it2=familySets[j].begin(); it2!=familySets[j].end(); it2++) 
						{
							int id2=*it2;
							ItemBase *node2=members[id2];
							int tar_size = node1->Width();
							int cur_size = node2->Width();
							int tar_size2 = node1->Height();
							int cur_size2 = node2->Height();
							QPointF tar = QPointF( node1->boundingRect().center().x(), node1->boundingRect().center().y());
							QPointF cur = QPointF( node2->boundingRect().center().x(), node2->boundingRect().center().y());
							if( !outBox( tar, tar_size, tar_size2, cur, cur_size, cur_size2, 0 ) )
							{
								tmpNei.insert(j);
								isNeighbor=true;
								break;
							}										
						}
						if(isNeighbor)
						{										
							break;
						}
					}
			}						
		}
	}
	for(set<int>::iterator it=tmpNei.begin(); it!=tmpNei.end(); it++)  
	{
		nei.push_back(*it);		
	}
	return nei;
}

bool Group::isConnectionF(int i, int justMoved, vector<set<int> > familySets, vector<QRectF>  rectList)
{
	set<int> tmpNei=findConnectionFSet(i, familySets, rectList, 1);
	if(tmpNei.find(justMoved)==tmpNei.end())
	{
		return false;
	}
	return true;
}

int Group::encode(float x,float y, float XL, float XR, float YB, float YT)
{  
   int code;
   int c=0;
   if(x<XL)c=c|1;
   else if(x>XR)c=c|2;
   if(y<YB)c=c|4;
   else if(y>YT)c=c|8;
   code=c;

   return code;
}

int Group::connectionIndex(int code)
{   
	int index;
	switch(code)
	{
	   case 5: index=0; break;
	   case 4: index=1; break;
	   case 6: index=2; break;
	   case 2: index=3; break;
	   case 10: index=4; break;
	   case 8: index=5; break;
	   case 9: index=6; break;
	   case 1: index=7; break;
	   case 0: index=8; break;
	}
	return index;
}

int Group::getConnectionType(QPointF tar, float width1, float height1, QPointF cur, float width2, float height2, float margin)
{ 
  //assum cur is tar's neighbor, find which side of tar is attached by cur
  float tx0, tx1, ty0, ty1, cx0, cx1, cy0, cy1;    
  cx0=cur.x(), cx1=cur.x()+width2, cy0=cur.y(), cy1=cur.y()+height2;
  tx0=tar.x()+margin, tx1=tar.x()+width1-margin, ty0=tar.y()+margin, ty1=tar.y()+height1 - margin;
  
  set<int> code;
  set<int> index1;
  vector<int> index;
  code.insert(encode(cx0, cy0, tx0,tx1,ty0,ty1));
  code.insert(encode(cx0, cy1, tx0,tx1,ty0,ty1));
  code.insert(encode(cx1, cy1, tx0,tx1,ty0,ty1));
  code.insert(encode(cx1, cy0, tx0,tx1,ty0,ty1));

  for(set<int>::iterator it = code.begin(); it!=code.end(); it++) 
  {
      index1.insert(connectionIndex(*it));
  }

  for(set<int>::iterator it = index1.begin(); it!=index1.end(); it++) 
  {
      index.push_back(*it);
  }

  if(index.size()==1)
  {
	  if(index[0]==6)
		  index[0]=6;
	   return index[0];  
  }
  else
  {
	 
	 bool flag=false;
	 int count=0;
     for(int i=0; i<index.size(); i++)
	 {
	    if(index[i]%2==1)	 
		{
		  flag=true;		  
		}
		else
		  count++;
	 }  
	 if(count==2)
		 flag=false;

	 vector<int> temp;
	 for(int i=0; i<index.size(); i++)
	 {
		if(index[i]==8)	 
		{
	 	   
		}
		else if(index[i]%2==0 && flag)	 
		{
		   
		}
		else 
			temp.push_back(index[i]);
	 }  	 
	 index=temp;

  }
  if(index.size()==1)
  {
	  if(index[0]==6)
		  index[0]=6;
	 return index[0];  
  }

  if(index.size()==2)
  {
     int id1=-1,id2=-1;
	 int id;
	 for(int i=0; i<index.size(); i++)
     {
		 if(index[i]%2==1)
		    id1 = index[i];
		 else id2 = index[i];
	 }	 
	 if(id1!=-1)
	 {
	 	 return id1;	 
	 }	
	 if(index[1] - index[0]>3)
	 {
	     index[0]=index[0]+8;
	 }
	 id=int((index[0] + index[1])/2.0 + 0.5);
	
	 if(id%8==6)
		  id=id;
	 return (id%8);	 
  }
  else if(index.size()>=3)
  {
	  if(index.size()==4)
		  index=index;
     int id;
	 float sum=0, dis=0;
	 for(int i=1; i<index.size(); i++)
     {
		 dis= index[i]-index[i-1];
		 if(dis>3)
			 index[i-1]=index[i-1]+8;
		 sum = sum + index[i-1];
	 }	 
	 sum = sum + index[index.size()-1];
	 
	 id = int(sum / index.size() + 0.5);

	  if(id%8==6)
		  id=id;
	 return (id%8);	
  }  
  return -1;
}

set<int> Group::findConnectionFSet(int i, vector<set<int> > familySets, vector<QRectF>  rectList, int dis)
{ //find the neibour after move //connection type leftbottom, mid bottom, ...
	//for family
	set<int> tmpNei;
	bool isNeighbor=false;
	//for(set<int>::iterator it0=notMoved.begin(); it0!=notMoved.end(); it0++) 
	for(int j=0; j<familySets.size(); j++)
	{
		//int j = *it0;		  
		if(tmpNei.find(j)==tmpNei.end() && i!=j )
		{	
			for(set<int>::iterator it1=familySets[i].begin(); it1!=familySets[i].end(); it1++) 
			{
				int id1=*it1;				
				for(set<int>::iterator it2=familySets[j].begin(); it2!=familySets[j].end(); it2++) 
				{
					int id2=*it2;					
					int tar_size = rectList[id1].width();
					int cur_size = rectList[id2].width();
					int tar_size2 = rectList[id1].height();
					int cur_size2 = rectList[id2].height();
					QPointF tar = QPointF( rectList[id1].center().x(), rectList[id1].center().y());
					QPointF cur = QPointF( rectList[id2].center().x(), rectList[id2].center().y());
					if( !outBox( tar, tar_size, tar_size2, cur, cur_size, cur_size2, dis) )
					{	
						tmpNei.insert(j);
						isNeighbor=true;
						break;
					}										
				}
				if(isNeighbor)
				{										
					break;
				}
			}
		}
	}	
	return tmpNei;
}


vector<int> Group::findConnectionFSet(int i, vector<set<int> > familySets, vector<QRectF>  rectList, vector<int> &Index, vector<int> &headNeighbor, int dis)
{ //find the neibour after move //connection type leftbottom, mid bottom, ...
	//for family
	set<int> tmpNei;
	vector<int> Index1;
	vector<int> tmpNei1,tmpNei2;
	vector<int> headNeighbor1;
	Index.clear();
	bool isNeighbor=false;
	
	//for(set<int>::iterator it0=notMoved.begin(); it0!=notMoved.end(); it0++) 
	for(int j=0; j<familySets.size(); j++)
	{
		//int j = *it0;	
		
		if(tmpNei.find(j)==tmpNei.end() && i!=j )
		{	
			for(set<int>::iterator it1=familySets[i].begin(); it1!=familySets[i].end(); it1++) 
			{
				int id1=*it1;				
				for(set<int>::iterator it2=familySets[j].begin(); it2!=familySets[j].end(); it2++) 
				{
					int id2=*it2;	
					if(id1==id2)
						continue;
					int tar_size = rectList[id1].width();
					int cur_size = rectList[id2].width();
					int tar_size2 = rectList[id1].height();
					int cur_size2 = rectList[id2].height();
					QPointF tar = QPointF( rectList[id1].center().x(), rectList[id1].center().y());
					QPointF cur = QPointF( rectList[id2].center().x(), rectList[id2].center().y());
					if( !outBox( tar, tar_size, tar_size2, cur, cur_size, cur_size2, dis) )
					{
						Index1.push_back(getConnectionType(rectList[id1].topLeft(), tar_size, tar_size2, rectList[id2].topLeft(), cur_size, cur_size2, 2.0));
						tmpNei1.push_back(j);
						tmpNei.insert(j);
						headNeighbor1.push_back(id2);
						isNeighbor=true;
						break;
					}										
				}
				if(isNeighbor)
				{										
					break;
				}
			}
		}
	}	
	for(set<int>::iterator it=tmpNei.begin(); it!=tmpNei.end(); it++) 
	{
		for(int i=0; i<tmpNei1.size(); i++)
		{
			if(tmpNei1[i]==*it)
			{
		    	tmpNei2.push_back(tmpNei1[i]);
				Index.push_back(Index1[i]);
				headNeighbor.push_back(headNeighbor1[i]);
				break;
			}
		}
	}
	return tmpNei2;
}

vector<int> Group::findConnectionF(int i, vector<set<int> > familySets, vector<QRectF>  rectList, int dis)
{ //find the neibour after move
	//for family
	vector<int> nei;
	set<int> tmpNei=findConnectionFSet(i, familySets, rectList, dis);
	for(set<int>::iterator it=tmpNei.begin(); it!=tmpNei.end(); it++)  
	{
		nei.push_back(*it);		
	}
	return nei;
}

vector<int> Group::findConnectionF(int i, vector<set<int> > familySets, vector<QRectF>  rectList, vector<int> &Index, vector<int> &headNeighbor, int dis)
{ //find the neibour after move
	//for family
	vector<int> tmpNei=findConnectionFSet(i, familySets, rectList, Index, headNeighbor, dis);	
	return tmpNei;
}

void Group::getNeighbor(set<int> initF, vector<set<int> > familySets, vector<QRectF>  RectList, int cornerIndex, vector<vector<int>> &referIndex, vector<vector<int>> &headID, vector<vector<QPointF> > &relativeLocation, vector<vector<int> > &neighbor, bool isResize)
{
	//referIndex
	//0 (leftbottom), 1(midd bottom), 2(rightbottom), 3(right mide), 4, 5, 6, 7 ...
	relativeLocation.clear();	neighbor.clear();
	referIndex.clear();
	set<int> testSet1=initF, testSet2;
	for(int i=0; i<familySets.size(); i++)
	{
		if(testSet1.find(i)==testSet1.end())
			testSet2.insert(i);
	}

	for(int i=0; i<familySets.size(); i++)
	{		 
		 vector<int> nei,Nei;
		 set<int> testSet;
		 if(testSet1.find(i)!=testSet1.end())
			 testSet=testSet1;
		 else 
			 testSet=testSet2;
		
		//nei=findNeighbor(i, members, initF, familySets);
		//for bubble resized case	
		vector<int> Index,HeadNeighbor,headNeighbor, index;
		nei=findConnectionF(i, familySets, RectList, index, headNeighbor, 2);		

		for(int j=0; j<nei.size(); j++)
	    {
		   if(testSet.find(nei[j])!=testSet.end())
		   {
				Nei.push_back(nei[j]);
				Index.push_back(index[j]);
				HeadNeighbor.push_back(headNeighbor[j]);
		   }
	    }
		if(isResize) //for resize
		{
			Nei=nei;
			Index=index;
			HeadNeighbor=headNeighbor;
		}
		else if(cornerIndex>=0)
			cornerIndex = cornerIndex;

		QPointF center, topRight;
		vector<QPointF> dis;
		set<int> nindex;
		if(cornerIndex >= 0 && initF.find(i)!=initF.end())
		{
		   nindex.insert((cornerIndex+1+8)%8);		
		   nindex.insert((cornerIndex-1+8)%8);
		   nindex.insert(cornerIndex);
		   if(cornerIndex == 0)
		   {
			   nindex.insert(6);		
			   nindex.insert(5);			   
		   }
		   else if(cornerIndex == 6)
		   {
			   nindex.insert(0);		
			   nindex.insert(1);			   
		   }
		}
		for(int j=0; j<Nei.size(); j++)  
		{
			//if(cornerIndex<0 || initF.find(i)==initF.end())
			//{
			     //Index[j]=-1;			
			//}			
			if(cornerIndex >= 0 && Index[j]!=-1 && !nindex.empty())
			{
				if(nindex.find(Index[j])==nindex.end())
				{
			        Index[j]=-2;			
				}
			}
			int id1=*familySets[i].begin();
				int id2=*familySets[Nei[j]].begin();
				QPointF p1, p2;
				QPointF temp;
			if(cornerIndex < 0) //not resizing
			{
			    QPointF p1, p2;
			    p1=RectList[id2].center();
				p2=RectList[id1].center();
				dis.push_back(p1- p2);	
			}
			else
			{
				
				switch(Index[j])
				{
					case -1:
					case -2: p1=RectList[id2].center();
							 p2=RectList[id1].center();
							 dis.push_back(p1- p2);	
							 break; //mid left)
					default:					  
						int nodeid,indexj=Index[j];
						if(indexj == cornerIndex)
						{
							nodeid=cornerIndex;					
						}
						else if(indexj<cornerIndex)
						{  
							if( cornerIndex - indexj > 3 )
							{
								nodeid=(cornerIndex+2)%8;					
							}
							else 
								nodeid=(cornerIndex-2)%8;					
						}
						else if(indexj>cornerIndex)
						{  
							if( indexj - cornerIndex > 3 )
							{	
								//cornerIndex = cornerIndex+8;	
								nodeid=(cornerIndex+8-2)%8;					
							}
							else 
								nodeid=(cornerIndex+2)%8;					
						}
						switch (nodeid)
						{
						   case 0: dis.push_back(RectList[id2].center()-RectList[id1].topLeft());//	Index[j]=0;	
								   break;
						   case 2: //temp=RectList[id2].center()-RectList[id1].topRight();  
								   //if(temp.x()<=0) 
								   //  dis.push_back(QPointF(-RectList[id2].width()/2.0, temp.y()));
								   //else
								   //  dis.push_back(QPointF(RectList[id2].width()/2.0, temp.y()));
								   dis.push_back(RectList[id2].center()-RectList[id1].topRight());
								   //Index[j]=2;	 
								   break;	
						   case 4: dis.push_back(RectList[id2].center()-RectList[id1].bottomRight());	
								   //Index[j]=4;	
								   break;
						   case 6: dis.push_back(RectList[id2].center()-RectList[id1].bottomLeft());   
								   //Index[j]=6;	
								   break;
						}
						break;
				}
			}
		}				
		referIndex.push_back(Index);
		relativeLocation.push_back(dis);
		neighbor.push_back(Nei);
		headID.push_back(HeadNeighbor);		
   }
	
}

vector<QRectF> Group::lineUpDisplay(int eventType, QList<ItemBase *> members, vector<int> parentList, set<int> initF, int size, vector<set<int> > familySets, vector<QRectF>  rectList, int screenWidth, int screenHeight, QRectF oriRect, int cornerIndex, int screenPosx, int screenPosy, bool bigRect)
{   //QList<ItemBase *> members: all the item to be arranged
    //size: item size
	//set<int> init, all the id of item in the members
    //bigRect: false, use realrect (which is smaller)
    //true: boundingrect which is bigger than realrect	
    //members should include init set
	//sx, sy, bounding straint: (sx, sy, screenwidth, screenheight)

	if(initF.empty())
		return rectList;

	for(int i=0; i<rectList.size(); i++) //keqin debug
	{
			float x,y;				
			x=rectList[i].center().x();
			y=rectList[i].center().y();	

			float disx=x-rectList[0].center().x();
			float width=rectList[i].width();
			int Idisx=x-rectList[0].center().x();
			int Iwidth=rectList[i].width();
			x=x;
	}

	int justMoved,justMovedF;
	set<int> movedSet,notMovedSet;
	int size1=members.size();
	//set<int> initF;

	//check adjacency relationship for those not in initF
	vector<vector<QPointF> > relativeLocation;
	vector<vector<int> > neighbor;
	vector<vector<int> > referIndex,headID;

	vector<QRectF>  RectList=rectList;
	if(*initF.begin()<0)
		return rectList;
	int initID=*familySets[*initF.begin()].begin();		

	if(oriRect.width()>0 && initF.size()==1 && familySets[*initF.begin()].size()==1)
	    RectList[initID]=oriRect;

	if( !(eventType==BUBBLERESIZED || eventType==BUBBLESHIFTED) && cornerIndex>=0)
		cornerIndex=cornerIndex;

	getNeighbor(initF, familySets, RectList, cornerIndex, referIndex, headID, relativeLocation,	neighbor,  eventType==BUBBLERESIZED || eventType==BUBBLESHIFTED);	
	
   //vector<QRectF>  RectList;	
   vector<int> directionMark;    
   //RectList=rectList;

   if(eventType==BUBBLESHIFTED)
   {
        justMovedF=*initF.begin();			
		int size1=members.size();
	    
		movedSet=initF;
		for(int i=0; i<familySets.size(); i++)
		{
			if(initF.find(i)==initF.end())
				notMovedSet.insert(i);
		}
		directionMark.resize(familySets.size());
		for(int i=0; i<familySets.size(); i++)
		{
			directionMark[i]=0;
		}
		set<int> shifted=initF;
		QRectF oRect, curRect;
		oRect=oriRect, curRect=rectList[initID];			
			
		if(shifted.find(justMovedF)!= shifted.end() && oriRect!=curRect)
	    {		
		        shiftNeighbor(-1, QPointF(-10000,-10000), oriRect, curRect, justMovedF, movedSet, notMovedSet, shifted, neighbor, cornerIndex, referIndex, headID, relativeLocation, rectList, parentList, directionMark, familySets, screenWidth, screenHeight, screenPosx, screenPosy);		   
	    }	   
		return rectList;
   }

   bool sflag=false;
   if( eventType==BUBBLERESIZED && oriRect.width()>0 && initF.size()==1 && familySets[*initF.begin()].size()==1) //for resized to smaller and change connection cases
	{
		//bigRect Used for place newly created bubble
		if(isChangeConnection(initID, oriRect, rectList))
		{
			justMovedF=*initF.begin();			
			int size1=members.size();
	    
			movedSet=initF;
			for(int i=0; i<familySets.size(); i++)
			{
				if(initF.find(i)==initF.end())
				   notMovedSet.insert(i);
			}
			directionMark.resize(familySets.size());
			for(int i=0; i<familySets.size(); i++)
			{
			   directionMark[i]=0;
			}
			set<int> shifted=initF;
			QRectF oRect, curRect;
			oRect=oriRect, curRect=rectList[initID];			
			tSpacer(eventType, justMovedF, oriRect, curRect, movedSet, notMovedSet, shifted, neighbor, cornerIndex, referIndex, headID, relativeLocation, rectList, parentList, directionMark, familySets, screenWidth, screenHeight, screenPosx, screenPosy);	 						
			sflag=true;
		}		
	}
		
    if(!sflag)
	for(set<int>::iterator it=initF.begin(); it!=initF.end(); it++ )
	{
		justMovedF=*it;
		//justMovedF=getFamilyID(justMoved, familySets);
		int size1=members.size();
	    
		movedSet=initF;
		for(int i=0; i<familySets.size(); i++)
		{
			if(initF.find(i)==initF.end())
			   notMovedSet.insert(i);
		}
		directionMark.resize(familySets.size());
		for(int i=0; i<familySets.size(); i++)
		{
		   directionMark[i]=0;
		}
		set<int> shifted=movedSet;
		
		QRectF oRect, curRect;
		if(eventType==BUBBLERESIZED)
		   oRect=oriRect, curRect=rectList[initID];		
		else 
		   oRect=curRect=rectList[initID];		
	    dSpacer(eventType, justMovedF, oRect, curRect, movedSet, notMovedSet, shifted, neighbor, cornerIndex, referIndex, headID, relativeLocation,  rectList, parentList, directionMark, familySets, screenWidth, screenHeight, screenPosx, screenPosy);	 					
	} //test k
	

	if(!sflag)
	if(!bigRect) //bigRect Used for place newly created bubble
	if(isbridgeBroken(rectList))
	{
		//need to reorganize family
		notMovedSet.clear();
		movedSet.clear();
		
		QList<ItemBase *> list=getMembers();
		vector<set<int> > segList = getGroupSegment(rectList); 
	    vector<set<int> > NewfamilySets;	
		set<int> initSet;
		
		//vector<int> initSet;
	    int size=segList.size();
		for(int i=0; i<size; i++)
		{
			NewfamilySets.push_back(segList[i]);	
			int initID= *familySets[*initF.begin()].begin();
			if(segList[i].find(initID)!=segList[i].end())
			{	
				initSet=segList[i];
			}
		}
		
		initF.clear();
		for(set<int>::iterator it=initSet.begin(); it!=initSet.end(); it++)
		{
			initF.insert(getFamilyID(*it,NewfamilySets));		
		}

		getNeighbor(initF,	NewfamilySets, RectList, cornerIndex, referIndex, headID, relativeLocation,	neighbor, oriRect.width()>0);	

		for(int i=0; i<NewfamilySets.size(); i++)
		{
			if(initF.find(i)==initF.end())
				notMovedSet.insert(i);
		}
		movedSet=initF;	    
		directionMark.resize(NewfamilySets.size());
		for(int i=0; i< directionMark.size(); i++)
		{
		    directionMark[i]=0;
		}
		justMovedF=*initF.begin();
		set<int> shifted=movedSet;
	
		QRectF oRect, curRect;
		oRect=curRect=rectList[initID];					
		tSpacer(eventType, justMovedF, oRect, curRect, movedSet, notMovedSet, shifted, neighbor, cornerIndex, referIndex, headID, relativeLocation, rectList, parentList, directionMark, NewfamilySets, screenWidth, screenHeight, screenPosx, screenPosy);					
	}	

	for(int i=0; i<rectList.size(); i++) //keqin debug
	{
		float x,y;				
		x=rectList[i].center().x();
		y=rectList[i].center().y();	

		if(rectList[i].x()-int(rectList[i].x())>0.001)
			i=i;

		float disx=x-rectList[0].center().x();
		float width=rectList[i].width();
		int Idisx=x-rectList[0].center().x();
		int Iwidth=rectList[i].width();
		x=x;
	}


	return rectList;	
}

/*
vector<QRectF> Group::lineUpDisplay(int eventType, QList<ItemBase *> members, vector<int> parentList, set<int> initF, int size, vector<set<int> > familySets, vector<QRectF>  rectList, int screenWidth, int screenHeight, QRectF oriRect, int cornerIndex, int screenPosx, int screenPosy, bool bigRect)
{   //QList<ItemBase *> members: all the item to be arranged
    //size: item size
	//set<int> init, all the id of item in the members
    //bigRect: false, use realrect (which is smaller)
    //true: boundingrect which is bigger than realrect	
    //members should include init set
	//sx, sy, bounding straint: (sx, sy, screenwidth, screenheight)
	int justMoved,justMovedF;
	set<int> movedSet,notMovedSet;
	int size1=members.size();
	//set<int> initF;

	//check adjacency relationship for those not in initF
	vector<vector<QPointF> > relativeLocation;
	vector<vector<int> > neighbor;
	vector<vector<int> > referIndex,headID;

	vector<QRectF>  RectList=rectList;
	if(*initF.begin()<0)
		return RectList;
	int initID=*familySets[*initF.begin()].begin();		

	if(oriRect.width()>0 && initF.size()==1 && familySets[*initF.begin()].size()==1)
	    RectList[initID]=oriRect;

	if( !(eventType==BUBBLERESIZED || eventType==BUBBLESHIFTED) && cornerIndex>=0)
		cornerIndex=cornerIndex;

	getNeighbor(initF, familySets, RectList, cornerIndex, referIndex, headID, relativeLocation,	neighbor,  eventType==BUBBLERESIZED || eventType==BUBBLESHIFTED);	
	
   //vector<QRectF>  RectList;	
   vector<int> directionMark;    
   //RectList=rectList;

   if(eventType==BUBBLESHIFTED)
   {
        justMovedF=*initF.begin();			
		int size1=members.size();
	    
		movedSet=initF;
		for(int i=0; i<familySets.size(); i++)
		{
			if(initF.find(i)==initF.end())
				notMovedSet.insert(i);
		}
		directionMark.resize(familySets.size());
		for(int i=0; i<familySets.size(); i++)
		{
			directionMark[i]=0;
		}
		set<int> shifted=initF;
		QRectF oRect, curRect;
		oRect=oriRect, curRect=rectList[initID];			
			
		if(shifted.find(justMovedF)!= shifted.end() && oriRect!=curRect)
	    {		
		        shiftNeighbor(-1, QPointF(-10000,-10000), oriRect, curRect, justMovedF, movedSet, notMovedSet, shifted, neighbor, cornerIndex, referIndex, headID, relativeLocation, rectList, parentList, directionMark, familySets, screenWidth, screenHeight, screenPosx, screenPosy);		   
	    }	   
		return rectList;
   }

   bool sflag=false;
   if( eventType==BUBBLERESIZED && oriRect.width()>0 && initF.size()==1 && familySets[*initF.begin()].size()==1) //for resized to smaller and change connection cases
	{
		//bigRect Used for place newly created bubble
		if(isChangeConnection(initID, oriRect, rectList))
		{
			justMovedF=*initF.begin();			
			int size1=members.size();
	    
			movedSet=initF;
			for(int i=0; i<familySets.size(); i++)
			{
				if(initF.find(i)==initF.end())
				   notMovedSet.insert(i);
			}
			directionMark.resize(familySets.size());
			for(int i=0; i<familySets.size(); i++)
			{
			   directionMark[i]=0;
			}
			set<int> shifted=initF;
			QRectF oRect, curRect;
			oRect=oriRect, curRect=rectList[initID];			
			tSpacer(eventType, justMovedF, oriRect, curRect, movedSet, notMovedSet, shifted, neighbor, cornerIndex, referIndex, headID, relativeLocation, rectList, parentList, directionMark, familySets, screenWidth, screenHeight, screenPosx, screenPosy);	 						
			sflag=true;
		}		
	}
		
    if(!sflag)
	for(set<int>::iterator it=initF.begin(); it!=initF.end(); it++ )
	{
		justMovedF=*it;
		//justMovedF=getFamilyID(justMoved, familySets);
		int size1=members.size();
	    
		movedSet=initF;
		for(int i=0; i<familySets.size(); i++)
		{
			if(initF.find(i)==initF.end())
			   notMovedSet.insert(i);
		}
		directionMark.resize(familySets.size());
		for(int i=0; i<familySets.size(); i++)
		{
		   directionMark[i]=0;
		}
		set<int> shifted=movedSet;
		
		QRectF oRect, curRect;
		if(eventType==BUBBLERESIZED)
		   oRect=oriRect, curRect=rectList[initID];		
		else 
		   oRect=curRect=rectList[initID];		
	    dSpacer(eventType, justMovedF, oRect, curRect, movedSet, notMovedSet, shifted, neighbor, cornerIndex, referIndex, headID, relativeLocation,  rectList, parentList, directionMark, familySets, screenWidth, screenHeight, screenPosx, screenPosy);	 					
	} //test k
	

	if(!sflag)
	if(!bigRect) //bigRect Used for place newly created bubble
	if(isbridgeBroken(rectList))
	{
		//need to reorganize family
		notMovedSet.clear();
		movedSet.clear();
		
		QList<ItemBase *> list=getMembers();
		vector<set<int> > segList = getGroupSegment(rectList); 
	    vector<set<int> > NewfamilySets;	
		set<int> initSet;
		
		//vector<int> initSet;
	    int size=segList.size();
		for(int i=0; i<size; i++)
		{
			NewfamilySets.push_back(segList[i]);	
			int initID= *familySets[*initF.begin()].begin();
			if(segList[i].find(initID)!=segList[i].end())
			{	
				initSet=segList[i];
			}
		}
		
		initF.clear();
		for(set<int>::iterator it=initSet.begin(); it!=initSet.end(); it++)
		{
			initF.insert(getFamilyID(*it,NewfamilySets));		
		}

		getNeighbor(initF,	NewfamilySets, RectList, cornerIndex, referIndex, headID, relativeLocation,	neighbor, oriRect.width()>0);	

		for(int i=0; i<NewfamilySets.size(); i++)
		{
			if(initF.find(i)==initF.end())
				notMovedSet.insert(i);
		}
		movedSet=initF;	    
		directionMark.resize(NewfamilySets.size());
		for(int i=0; i< directionMark.size(); i++)
		{
		    directionMark[i]=0;
		}
		justMovedF=*initF.begin();
		set<int> shifted=movedSet;
	
		QRectF oRect, curRect;
		oRect=curRect=rectList[initID];					
		tSpacer(eventType, justMovedF, oRect, curRect, movedSet, notMovedSet, shifted, neighbor, cornerIndex, referIndex, headID, relativeLocation, rectList, parentList, directionMark, NewfamilySets, screenWidth, screenHeight, screenPosx, screenPosy);					
	}	
	
	
	return rectList;	
}
*/

bool Group::isOverlaid( QList<ItemBase*>  members)
{	
	for( int i = 0; i < members.size(); i++ )
	{
		for( int j = i+1; j < members.size(); j++ )
		{
			if(overlaid( members[i], members[j]))
			{
			   return true;
			}  			
		}
	}	
	return false;	
}

bool Group::isOverlaid( vector<QRectF> rectlist)
{	
	for( int i = 0; i < rectlist.size(); i++ )
	{
		for( int j = i+1; j < rectlist.size(); j++ )
		{
			if(overlaid( i, j, rectlist))
			{
			   return true;
			}  			
		}
	}	
	return false;	
}

int Group::findOverlaid( int id, vector<QRectF> rectlist)
{	//find overlaid, return the distance for to move closer
	float min=10000000, dis;
	int mini=0;
	for( int i = 0; i < rectlist.size(); i++ )
	{
		if(id!=i)
		if(overlaid( i, id, rectlist))
		{
			dis = fabs(rectlist[i].center().y() - rectlist[id].center().y());
			if(min>dis)
				min=dis, mini=i;
      	}		
	}
	
		return mini;
}

//(int justMoved, set<int> &movedSet, set<int> &notMovedSet, vector<QRectF> &rectList, vector<int> parentList, vector<int> &directionMark, vector<set<int> > familySets, int screenWidth, int screenHeight, int screenPosx, int screenPosy)
bool Group::adjustOverlaid(int initF, vector<QRectF>  &rectList, vector<int> parentList, vector<set<int> > familySets,  int screenWidth, int screenHeight, QRectF oriRect, int cornerIndex, int screenPosx, int screenPosy)//not shift neighbors
{
	bool flag=false;
	set<int> overlaidSets;
	for( int i = 0; i < rectList.size(); i++ )
	{
		for( int j = i+1; j < rectList.size(); j++ )
		{
				if(overlaid(  rectList[i],  rectList[j]))
				{	
					int id1=getFamilyID(i, familySets);
					int id2=getFamilyID(j, familySets);
					if(id2==initF)
						id1=initF;
					int fid=getFamilyID(id1, familySets);
					if(fid>=0)
					   overlaidSets.insert(fid);
					flag=true;
				}
			
		}
	}
    set<int> movedSet,notMovedSet;			
	
	if(!flag)
		return false;

	if(overlaidSets.size()==0)
		return false;
	movedSet.insert(*overlaidSets.begin());
	movedSet.insert(initF);
	int justMoved;
	for( int i = 0; i < familySets.size(); i++ )
	{ 
		 if(movedSet.find(i)==movedSet.end())
		    notMovedSet.insert(i);
	}
	
	vector<int> directionMark;
	directionMark.resize(familySets.size());
	for(int i=0; i<familySets.size(); i++)
	{
		directionMark[i]=0;
	}
	//void Group::dSpacer(int eventType, int justMoved, QRectF oriRect, QRectF curRect, set<int> &movedSet, set<int> &notMovedSet, set<int> &shifted, vector<vector<int> > neighbor, int cornerIndex, vector<vector<int> > referIndex, vector<vector<int> > headID, vector<vector<QPointF> > relativeLocation, vector<QRectF> &rectList, vector<int> parentList, vector<int> &directionMark, vector<set<int> > familySets, int screenWidth, int screenHeight, int screenPosx, int screenPosy)
    for(set<int>::iterator it = overlaidSets.begin(); it!=overlaidSets.end(); it++)
	{
		int id=*it;		
		if(movedSet.find(id)!=movedSet.end())
		{
			justMoved=id;			
	        dSpacer(justMoved, movedSet, notMovedSet, rectList, parentList, directionMark, familySets, screenWidth, screenHeight, screenPosx, screenPosy);	
		}
	}
	return flag;
}

void Group::dSpacer(int justMoved, set<int> &movedSet, set<int> &notMovedSet, vector<QRectF> &rectList, vector<int> parentList, vector<int> &directionMark, vector<set<int> > familySets, int screenWidth, int screenHeight, int screenPosx, int screenPosy) //not shift neighbors
{//dspacer for remove overlapping after resizing
	//if(eventType!=BUBBLERESIZED)
	while(!notMovedSet.empty())
	{
		bool flag=false;
	    for(set<int>::iterator it = notMovedSet.begin(); it!=notMovedSet.end(); it++)	
		{
			int id=*it;
			if(overlaid(id, justMoved, rectList, familySets))			
			{
				moveOverlaid(BUBBLERESIZED, justMoved, id, movedSet, notMovedSet, rectList, parentList, directionMark, familySets, screenWidth, screenHeight, screenPosx, screenPosy, true);					
				movedSet.insert(id);
	            notMovedSet.erase(id);				
	            dSpacer(id, movedSet, notMovedSet, rectList, parentList, directionMark, familySets, screenWidth, screenHeight, screenPosx, screenPosy);										
				flag=true;				
			}	
			if(flag)
			{			
				break;
			}
		}
		if(!flag)
			break;
	}
}



void Group::dSpacer(int eventType, int justMoved, QRectF oriRect, QRectF curRect, set<int> &movedSet, set<int> &notMovedSet, set<int> &shifted, vector<vector<int> > neighbor, int cornerIndex, vector<vector<int> > referIndex, vector<vector<int> > headID, vector<vector<QPointF> > relativeLocation, vector<QRectF> &rectList, vector<int> parentList, vector<int> &directionMark, vector<set<int> > familySets, int screenWidth, int screenHeight, int screenPosx, int screenPosy)
{//justMoved movedSet notMovedSet, shift set all store family id
	if(eventType!=SUBBUBBLECREATED && eventType!=BUBBLECREATED && eventType!=SUBTREERINGCREATED)
	if(shifted.find(justMoved)!= shifted.end() && oriRect!=curRect)
	{
		shiftNeighbor(-1, QPointF(-100000,-100000), oriRect, curRect, justMoved, movedSet, notMovedSet, shifted, neighbor, cornerIndex, referIndex, headID, relativeLocation, rectList, parentList, directionMark, familySets, screenWidth, screenHeight, screenPosx, screenPosy);		  		
	}
	//if(eventType!=BUBBLERESIZED)
	QRectF tRect;
	while(!notMovedSet.empty())
	{
		bool flag=false;		
	    for(set<int>::iterator it = notMovedSet.begin(); it!=notMovedSet.end(); it++)	
		{
			int id=*it;
			
			if(overlaid(id, justMoved, rectList, familySets))			
			{
				tRect=rectList[*familySets[id].begin()];
				moveOverlaid(eventType, justMoved, id, movedSet, notMovedSet, rectList, parentList, directionMark, familySets, screenWidth, screenHeight, screenPosx, screenPosy, true);					
				movedSet.insert(id);
	            notMovedSet.erase(id);
				shifted.insert(id);	
				//tRect, rectList[*familySets[id].begin()]
				//oriRect, curRect,
	            dSpacer(eventType, id, tRect, rectList[*familySets[id].begin()], movedSet, notMovedSet, shifted, neighbor, cornerIndex, referIndex, headID, relativeLocation, rectList, parentList, directionMark, familySets, screenWidth, screenHeight, screenPosx, screenPosy);										
				flag=true;				
			}
			if(flag)
			{			
				break;
			}
		}
		if(!flag)
			break;
	}
}


bool Group::isChangeConnection(int i, vector<vector<int> > neighbor, vector<set<int> > familySets, vector<QRectF>  rectList)
{
    vector<int> cNei = findConnectionF(i, familySets, rectList, 1);							 	
	if(cNei != neighbor[i])
		return true;
	return false;
}

bool Group::isChangeConnection(int initID, QRectF oriRect, vector<QRectF> rectList)
{
	 vector<set<int> > familySets;
	 familySets.resize(rectList.size());

	// familySets[0].insert(initID);
	 for(int i=0; i<rectList.size(); i++)
	 {
		 //if(i!=initID)
		 set<int> tSet;
	     tSet.insert(i);
		 familySets[i]=tSet;
	 }     

	 vector<int> cNei1 = findConnectionF(initID, familySets, rectList, 1);	
	 rectList[initID]=oriRect;
	 vector<int> cNei2 = findConnectionF(initID, familySets, rectList, 4);	
	 if(cNei1 != cNei2)
		return true;
	return 
		false;
}


bool Group::isOneWayConnection(int testID, int referIndex, vector<QRectF> rectList)
{//referIndex: connectionType, test to see if a bubble connected to the resized bubble in one way
        QRectF testRect=rectList[testID];	
	    switch(referIndex)
		{
		    case 1: 				
			case 2: testRect = QRectF(testRect.x(), testRect.y(), testRect.width(), testRect.height()/2.0);  break;					
			case 3: 
			case 4: testRect = QRectF(testRect.x() + testRect.width()/2.0, testRect.y(), testRect.width()/2.0, testRect.height());  break;					
			case 5:
		    case 6: testRect = QRectF(testRect.x(), testRect.y()+testRect.height()/2.0, testRect.width(), testRect.height()/2.0);  break;	
            case 0: 		    
			case 7: testRect = QRectF(testRect.x(), testRect.y(), testRect.width()/2.0, testRect.height());  break;						
			
		}   
		rectList[testID]=testRect;
		return(isbridgeBroken(rectList));
}



bool Group::ifShareKeyNeighbor(int resizedID, int testID, int cornerID, int testReferIndex, vector<vector<int> > neighbor, vector<vector<int> > referIndex)
{
	//referIndex: connectionType, test to see if a bubble connected to the resized bubble in one way	
	for(int i=0; i<neighbor[resizedID].size(); i++)
	for(int j=0; j<neighbor[testID].size(); j++)
	{	
		if(neighbor[resizedID][i]==neighbor[testID][j])
		{   
			int nID=neighbor[resizedID][i];
			if(referIndex[resizedID][i]==referIndex[testID][j] || referIndex[resizedID][i]<0 || referIndex[testID][j]<0)
			{
				int rID1, rID2;
				rID1=rID2=-1;
				if(referIndex[resizedID][i]!=referIndex[testID][j])
				{
					for(int k=0; k<neighbor[nID].size(); k++)
					{
						if(neighbor[nID][k]==resizedID) 
							rID1=referIndex[nID][k];
						else if(neighbor[nID][k]==testID) 
							rID2=referIndex[nID][k];
					}
				}				
				if(cornerID==0 && testReferIndex==7)// && rID1==5)
				{	
					//it is supposed to be moved along the draged corner (bottom aligna)															
					//return true;																	
				}
				else if(cornerID==2 && testReferIndex==0)// && rID1==5)
				{	//it is supposed to be moved along the draged corner (bottom aligna)															
					//return true;																	
				}
				else if(cornerID==4 && testReferIndex==3)
				{	
					//it is supposed to be moved along the draged corner (bottom aligna)															
					if( rID1==rID2 ||  (rID1==5 && rID2==6) ||  (rID1==6 && rID2==5) )
					   return true;																	
				}
				else if(cornerID==6 && testReferIndex==7)
				{	
					//it is supposed to be moved along the draged corner (bottom aligna)
					if(rID1==rID2 || (rID1==5 && rID2==4) ||  (rID1==4 && rID2==5) )
					   return true;																	
				}			    
			}
		}
	}		
	/*}
	}

	else if(cornerID==0) //corner mouse dragged corner
	{
		switch(referIndex)
		{
		    case 1: x = curRect.x() - oriRect.x();	
					y = curRect.y() - headRect.height()/2.0 -headRect.center().y();							
					refPos = QPointF(x,y); 
					break; //move up (left align)
			case 0: x = curRect.x()- headRect.width()/2.0 - headRect.center().x();
					y = curRect.y() - headRect.height()/2.0 -headRect.center().y();							
					refPos = QPointF(x,y); 		
					break; //move up/left
			case 7: x = curRect.x() - headRect.width()/2.0 -headRect.center().x();
				    y = 0;	
					refPos = QPointF(x,y); //headRect.center()+ topRight()- oriRect.topRight();
					break; //move left (bottom fix))
			//for left align
			case 5:
		    case 6: x = curRect.x() - oriRect.x();	
				    y = 0;
					refPos = QPointF(x,y); //headRect.center()+ topRight()- oriRect.topRight();					
					break; //move left (left align)		
			default:
					refPos = curRect.center() - oriRect.center();	
					break; //mid left
		}
	}
	else if(cornerID==2)
	{
		switch(referIndex)
		{
			case 1: x = 0;//curRect.x() - oriRect.x();	
					y = curRect.y() - headRect.height()/2.0 -headRect.center().y();	
					refPos = QPointF(x,y); 
					break; //move up (left fix)
			case 2: x = curRect.bottomRight().x()+ headRect.width()/2.0 - headRect.center().x();
					y = curRect.y() - headRect.height()/2.0 -headRect.center().y();		
					refPos = QPointF(x,y); 		
					break; //move up/right
			case 3: x = curRect.bottomRight().x()+ headRect.width()/2.0 - headRect.center().x();
				    y = 0;
				    refPos = QPointF(x,y); //headRect.center()+ topRight()- oriRect.topRight();
					break; //move right (bottom fix)
			default:
					refPos = curRect.center() - oriRect.center();	
					break; //mid left
		}
	}
	else if(cornerID==4)
	{
		switch(referIndex)
		{
			case 3: x = curRect.bottomRight().x()+ headRect.width()/2.0 - headRect.center().x();
					y = curRect.bottomRight().y() - oriRect.bottomRight().y();							
					refPos = QPointF(x,y); 
					break; //move right (bottom aligna)
			case 4: x = curRect.bottomRight().x()+ headRect.width()/2.0 - headRect.center().x();
					y = curRect.bottomRight().y() + headRect.height()/2.0 - headRect.center().y();	
					refPos = QPointF(x,y); 		
					break; //move down/right
			case 5: x = 0;
				    y = curRect.bottomRight().y() + headRect.height()/2.0 -headRect.center().y();						
				    refPos = QPointF(x,y); //headRect.center()+ topRight()- oriRect.topRight();
					break; //move down (left fix)
			default:
					refPos = curRect.center() - oriRect.center();	
					break; //mid left
		}
	}
	else if(cornerID==6)
	{
		switch(referIndex)
		{
			case 7: x = curRect.x() - headRect.width()/2.0 -headRect.center().x();
				    y = curRect.bottomRight().y() - oriRect.bottomRight().y();
					refPos = QPointF(x,y); //headRect.center()+ topRight()- oriRect.topRight();					
					break; //move left (bottom align)
			case 6: x = curRect.x() - headRect.width()/2.0 -headRect.center().x();
					y = curRect.bottomRight().y() + headRect.height()/2.0 -headRect.center().y();							
					refPos = QPointF(x,y); 		
					break; //move down/left
			case 5: x = curRect.x() - oriRect.x();	
				    y = curRect.bottomRight().y() + headRect.height()/2.0 -headRect.center().y();	
				    refPos = QPointF(x,y); //headRect.center()+ topRight()- oriRect.topRight();
					break; //move down (left align)
            //for left align					
			case 0: 
			case 1: x = curRect.x() - oriRect.x();	
				    y = 0;
					refPos = QPointF(x,y); //headRect.center()+ topRight()- oriRect.topRight();					
					break; //(left align)			
			default:
					refPos = curRect.center() - oriRect.center();	
					break; //mid left
		}
	}
	{
	    case 0:
			switch(referIndex)
			{
				case 1: 				
				case 2: testRect = QRectF(testRect.x(), testRect.y(), testRect.width(), testRect.height()/2.0);  break;					
				case 3: 
				case 4: testRect = QRectF(testRect.x() + testRect.width()/2.0, testRect.y(), testRect.width()/2.0, testRect.height());  break;					
				case 5:
				case 6: testRect = QRectF(testRect.x(), testRect.y()+testRect.height()/2.0, testRect.width(), testRect.height()/2.0);  break;	
				case 0: 		    
				case 7: testRect = QRectF(testRect.x(), testRect.y(), testRect.width()/2.0, testRect.height());  break;	
				if(refer)
			}   
			break;
	}
	rectList[testID]=testRect;
	return(isbridgeBroken(rectList));*/
    return false;
}

QPointF Group::setNeighborPos(QRectF Rect, QPointF tobeMoved, QPointF relativePos, int index)
{
	QPointF newPos,refPos;
	switch(index)
	{
		case 0: newPos = Rect.topLeft() +  relativePos;	
			    refPos = newPos - tobeMoved;
				break; //left bottom)
		case 1: newPos = (Rect.topLeft() + Rect.topRight())/2.0 +  relativePos;			  
				refPos = newPos - tobeMoved;
				break; //(mid bottom)

		case 2: newPos = Rect.topRight() +  relativePos;			  
				refPos = newPos - tobeMoved;
				refPos = QPointF(refPos.x(), 0);
			    break; //bottom right)

		case 3: newPos = (Rect.topRight() + Rect.bottomRight())/2.0 +  relativePos;	
				refPos = newPos - tobeMoved;
				break; //(mid right)						

		case 4: newPos = Rect.bottomRight() +  relativePos;	
				refPos = newPos - tobeMoved;
				break; //topRight)

		case 5: newPos = (Rect.bottomLeft() + Rect.bottomRight())/2.0 +  relativePos;	
				refPos = newPos - tobeMoved;
				break; //(mid top)											    

		case 6: newPos = Rect.bottomLeft() +  relativePos;	
				refPos = newPos - tobeMoved;
				break; //left top)

		case 7: newPos = (Rect.bottomLeft() + Rect.topLeft())/2.0 +  relativePos;					
				refPos = newPos - tobeMoved;
				break; //mid left)
        case -1: 
		case -2: newPos = Rect.center() +  relativePos;					
			     refPos = newPos - tobeMoved;
				break; //mid left
	}		
	
	return refPos;
	//QPointF tempPos = rectList[initID].center() + p + rectList[id3].center() -Head;
    //QPointF tempPos = setNeighborPos(initID, id3, referIndex,relativeLocation,rectList);
}

void Group::shiftNeighbor(int parentID, QPointF moveCenterPos, QRectF oriRect, QRectF curRect, int justMoved,  set<int> &movedSet, set<int> &notMovedSet, set<int> &shifted, vector<vector<int> > neighbor, int cornerIndex, vector<vector<int> > referIndex, vector<vector<int> > headID, vector<vector<QPointF> > relativeLocation, vector<QRectF> &rectList, vector<int> parentList, vector<int> directionMark, vector<set<int> > familySets, int screenWidth, int screenHeight, int screenPosx, int screenPosy)
{		
	//if (id2 was in id's neightourhood)	
	int initID=*familySets[justMoved].begin();
	QPointF rePos;
	vector<bool> canBecontinued(neighbor[justMoved].size(), false);
	vector<QPointF> RPos; //for debug
	vector<int> sID;//for debug
	QPointF move;
	vector<QRectF> sList=rectList;//for debug
	bool foundOverLap1=false, foundOverLap2=false, isMovable=false;
	RPos.resize(neighbor[justMoved].size());

	/*if(neighbor[justMoved].size()==2) //debug
	{
		int ID=*familySets[justMoved].begin();
		if(rectList[ID].width()==200)
			ID=ID;
		vector<int> nid(2,0),bid(2,0);
		for( int i =0; i<neighbor[justMoved].size(); i++)
		{
			nid[i]=neighbor[justMoved][i];
			bid[i]==*familySets[nid[i]].begin(); //infamilyID(justMoved			
		}
	}*/

	for( int i =0; i<neighbor[justMoved].size(); i++)
	{	 
		int toBeShifted = neighbor[justMoved][i];
	   if(referIndex[justMoved][i]==-2)
       {
		   shifted.insert(toBeShifted);		
		   movedSet.insert(toBeShifted); 
	       notMovedSet.erase(toBeShifted);	
		   continue;
	   }	
	   isMovable=false;
       if( notMovedSet.find(toBeShifted) != notMovedSet.end() && shifted.find(toBeShifted) == shifted.end() ) 
	   if(parentID<0 || overlaid(justMoved, toBeShifted, rectList, familySets) || isOneWayConnection(headID[justMoved][i], referIndex[justMoved][i], rectList) )
	   {  
		    QPointF Head=rectList[*familySets[toBeShifted].begin()].center();
			QPointF p=relativeLocation[justMoved][i];
			QRectF initRect = rectList[initID];			
			if(parentID<0)
			{
				int id = *familySets[toBeShifted].begin();
				rePos=getShiftedPos(justMoved, toBeShifted, cornerIndex, referIndex[justMoved][i], rectList[headID[justMoved][i]], oriRect, curRect, neighbor, referIndex);				
			}
			else 
				rePos=moveCenterPos;
		
			/*if(RPos.size()>0)
			{
			   if(rePos!=RPos[0])
				   rePos=rePos;			
			}*/
			//RPos.push_back(rePos);
			//sID.push_back(toBeShifted);
			//get 3 c
			vector<QPointF> moveList;			
			moveList.push_back(rePos);

			if(rePos.x()*rePos.y()!=0)
			{
			    moveList.push_back(QPointF(rePos.x(), 0));			
				moveList.push_back(QPointF(0, rePos.y()));	
			}
						
			vector<QRectF> RectList;
			int j;
            for(j=0; j<moveList.size(); j++) //test move steps (first, both in x, y direction; if it overlap with moved bubble, then, try x and y directly respectily )
			{
				foundOverLap1=false, foundOverLap2=false;
				RectList=rectList;
				move=moveList[j];
				//RPos.push_back(move);
				for(set<int>::iterator it3 = familySets[toBeShifted].begin(); it3!=familySets[toBeShifted].end(); it3++)	
				{
					int id3=*it3;	
					RectList[id3] = QRectF( RectList[id3].x() + move.x(),  RectList[id3].y() + move.y(),  RectList[id3].width(), RectList[id3].height());				
				}
			
				for(set<int>::iterator it=movedSet.begin(); it!=movedSet.end(); it++)
				{
					if(justMoved!=*it)
					if(overlaid(toBeShifted, *it, RectList, familySets))
					{
						foundOverLap1=true;
						break;
					}				
				}
				if(!foundOverLap1 )
				{
					/*for(int k=0; k<neighbor[justMoved].size(); k++)
					{
						if(movedSet.find(neighbor[justMoved][k])==movedSet.end() && toBeShifted!=neighbor[justMoved][k])
						if(overlaid(toBeShifted, neighbor[justMoved][k], RectList, familySets))
						{
						    foundOverLap2=true;
							break;
						}				
					}	
					if(!foundOverLap2)*/
					{
						isMovable=true;
						//canBecontinued[i]=true;					
					}
				}
				if(isMovable)
					break;
			}
			if(isMovable)
			{
				shifted.insert(toBeShifted);		
				movedSet.insert(toBeShifted); 
				notMovedSet.erase(toBeShifted);					
				rectList = RectList;
				RPos[i]=move;
				canBecontinued[i]=true;		
			}
			else
				isMovable=isMovable;	  
			
				for(int i=0; i<sID.size(); i++)
				for(set<int>::iterator it3 = familySets[sID[i]].begin(); it3!=familySets[sID[i]].end(); it3++)	
				{
					int id3=*it3;	
					if(rectList[id3].x()-sList[id3].x()!=RPos[i].x()) 				
						id3=id3;
					if(rectList[id3].y()-sList[id3].y()!=RPos[i].y()) 				
						id3=id3;
				}
	   }
	   else
			isMovable=isMovable;	   
    }
	/*if(RPos.size()!=2)
		RPos=RPos;
	else if(RPos[0]!=RPos[1])
	    RPos=RPos;
	else
	{
		for(int i=0; i<sID.size(); i++)
		for(set<int>::iterator it3 = familySets[sID[i]].begin(); it3!=familySets[sID[i]].end(); it3++)	
		{
			int id3=*it3;	
			if(rectList[id3].x()-sList[id3].x()!=RPos[i].x()) 				
			{
				//rectList[id3] = QRectF( sList[id3].x() + RPos[i].x(),  rectList[id3].y(),  rectList[id3].width(), rectList[id3].height());	
				id3=id3;
			}
			if(rectList[id3].y()-sList[id3].y()!=RPos[i].y()) 				
			{
				//rectList[id3] = QRectF( rectList[id3].x(),  sList[id3].y() + RPos[i].y(),  rectList[id3].width(), rectList[id3].height());	
				id3=id3;
			}
		}
	}*/
	////
	/////////	
	for( int i =0; i<neighbor[justMoved].size(); i++)
	{	 
		if(canBecontinued[i])
		{
			int toBeShifted = neighbor[justMoved][i];
			shiftNeighbor(justMoved, RPos[i], oriRect, curRect, toBeShifted, movedSet, notMovedSet, shifted, neighbor, cornerIndex, referIndex, headID, relativeLocation, rectList, parentList, directionMark, familySets, screenWidth, screenHeight, screenPosx, screenPosy);					
			
		}
	}
}

QPointF Group::getShiftedPos(int resizedID, int testID, int cornerID, int referID, QRectF headRect, QRectF oriRect, QRectF curRect, vector<vector<int> > neighbor, vector<vector<int> > referIndex)
{
    QPointF refPos;
	float x,y, br, hh, hc;
	if(referID<0 || cornerID<0)
	{
		refPos = curRect.center() - oriRect.center();	
		//move center
	}
	else if(cornerID==0) //corner mouse dragged corner
	{
		switch(referID)
		{
			case 1: x = curRect.x() - oriRect.x();	
					y = curRect.y() - headRect.height()/2.0 -headRect.center().y();							
					refPos = QPointF(x,y); 
					break; //move up (left align)			
			case 0: //x = curRect.x()- headRect.width()/2.0 - headRect.center().x();
					//y = curRect.y() - headRect.height()/2.0 -headRect.center().y();							
					//refPos = QPointF(x,y); 		
					//break; //move up/left
			case 7: x = curRect.x() - headRect.width()/2.0 -headRect.center().x();
				    y = 0;	
					refPos = QPointF(x,y); //headRect.center()+ topRight()- oriRect.topRight();
					break; //move left (bottom fix))
			//for left align
			case 5:
		    case 6: x = curRect.x() - oriRect.x();	
				    y = 0;
					refPos = QPointF(x,y); //headRect.center()+ topRight()- oriRect.topRight();					
					break; //move left (left align)		
			default:
					refPos = curRect.center() - oriRect.center();	
					break; //mid left
		}
	}
	else if(cornerID==2)
	{
		switch(referID)
		{
			case 2: //x = curRect.bottomRight().x()+ headRect.width()/2.0 - headRect.center().x();
					//y = curRect.y() - headRect.height()/2.0 -headRect.center().y();		
					//refPos = QPointF(x,y); 		
					//break; //move up/right
			case 1: x = 0;//curRect.x() - oriRect.x();	
					y = curRect.y() - headRect.height()/2.0 -headRect.center().y();	
					refPos = QPointF(x,y); 
					break; //move up (left fix)			
			case 3: x = curRect.bottomRight().x()+ headRect.width()/2.0 - headRect.center().x();
				    y = 0;
				    refPos = QPointF(x,y); //headRect.center()+ topRight()- oriRect.topRight();
					break; //move right (bottom fix)
			default:
					refPos = curRect.center() - oriRect.center();	
					break; //mid left
		}
	}
	else if(cornerID==4)
	{
		switch(referID)
		{
			case 3: 
				    x = curRect.bottomRight().x()+ headRect.width()/2.0 - headRect.center().x();
					if(!ifShareKeyNeighbor(resizedID, testID, cornerID, referID, neighbor, referIndex))
					    y = curRect.bottomRight().y() - oriRect.bottomRight().y();							
					refPos = QPointF(x,y); 
					break; //move right (bottom align)
			case 4: //x = curRect.bottomRight().x()+ headRect.width()/2.0 - headRect.center().x();
					//y = curRect.bottomRight().y() + headRect.height()/2.0 - headRect.center().y();	
					//refPos = QPointF(x,y); 		
					//break; //move down/right
			case 5: x = 0;
				    y = curRect.bottomRight().y() + headRect.height()/2.0 -headRect.center().y();						
				    refPos = QPointF(x,y); //headRect.center()+ topRight()- oriRect.topRight();
					break; //move down (left fix)
			default:
					refPos = curRect.center() - oriRect.center();	
					break; //mid left
		}
	}
	else if(cornerID==6)
	{
		switch(referID)
		{
			case 6: //x = curRect.x() - headRect.width()/2.0 -headRect.center().x();
					//y = curRect.bottomRight().y() + headRect.height()/2.0 -headRect.center().y();							
					//refPos = QPointF(x,y); 		
					//break; //move down/left
			case 7: x = curRect.x() - headRect.width()/2.0 -headRect.center().x();
				    if(!ifShareKeyNeighbor(resizedID, testID, cornerID, referID, neighbor, referIndex))
						y = curRect.bottomRight().y() - oriRect.bottomRight().y();
					refPos = QPointF(x,y); //headRect.center()+ topRight()- oriRect.topRight();					
					break; //move left (bottom align)
			
			case 5: x = curRect.x() - oriRect.x();	
				    y = curRect.bottomRight().y() + headRect.height()/2.0 -headRect.center().y();	
				    refPos = QPointF(x,y); //headRect.center()+ topRight()- oriRect.topRight();
					break; //move down (left align)
            //for left align					
			case 0: 
			case 1: x = curRect.x() - oriRect.x();	
				    y = 0;
					refPos = QPointF(x,y); //headRect.center()+ topRight()- oriRect.topRight();					
					break; //(left align)			
			default:
					refPos = curRect.center() - oriRect.center();	
					break; //mid left
		}
	}
	return refPos;
}



void Group::tSpacer(int eventType, int justMoved, QRectF oriRect, QRectF curRect, set<int> &movedSet, set<int> &notMovedSet, set<int> &shifted, vector<vector<int> > neighbor, int cornerIndex, vector<vector<int> > referIndex,vector<vector<int> > headID, vector<vector<QPointF> > relativeLocation,  vector<QRectF> &rectList, vector<int> parentList, vector<int> &directionMark, vector<set<int> > familySets, int screenWidth, int screenHeight, int screenPosx, int screenPosy)
{ //justMoved movedSet notMovedSet, shift set all store family id
	if(eventType!=BUBBLECREATED && eventType!=SUBBUBBLECREATED && eventType!=SUBTREERINGCREATED)
	if(shifted.find(justMoved)!= shifted.end() && oriRect!=curRect)
	{
		//QPointF moveCenterPos = getShiftedPos(eventType, cornerIndex, referIndex, oriRect, rectList[familySets[justMoved].begin()]);
		shiftNeighbor(-1, QPointF(-10000,-10000), oriRect, curRect, justMoved, movedSet, notMovedSet, shifted, neighbor, cornerIndex, referIndex, headID, relativeLocation, rectList, parentList, directionMark, familySets, screenWidth, screenHeight, screenPosx, screenPosy);		   
	}
	//if(eventType!=BUBBLERESIZED)
    while(!notMovedSet.empty())
	{
		bool flag=false;
	    for(set<int>::iterator it = notMovedSet.begin(); it!=notMovedSet.end(); it++)	
		{
			int id=*it;
			if(justMoved>=0 && overlaid(justMoved, id, rectList, familySets)) //overlaid(id, movedSet, rectList, familySets)) //
			{
				moveOverlaid(eventType, justMoved, id, movedSet, notMovedSet, rectList, parentList, directionMark, familySets, screenWidth, screenHeight, screenPosx, screenPosy, true);	
				movedSet.insert(id);
	            notMovedSet.erase(id);
				shifted.insert(id);	

				//movedPos=getShiftedPos(referIndex[justMoved][i], QRectF oriRect, QRectF curRect);
	            dSpacer(eventType, id, oriRect, curRect, movedSet, notMovedSet, shifted, neighbor, cornerIndex, referIndex, headID, relativeLocation,  rectList, parentList, directionMark, familySets, screenWidth, screenHeight, screenPosx, screenPosy);		
				flag=true;
				//break;
			}			
			if(justMoved>=0 && !flag && isbridgeBroken(rectList))
			{
				moveOverlaid(eventType, justMoved, id, movedSet, notMovedSet, rectList, parentList, directionMark, familySets, screenWidth, screenHeight, screenPosx, screenPosy, false);	
				movedSet.insert(id);
	            notMovedSet.erase(id);
				shifted.insert(id);	
	            tSpacer(eventType, id, oriRect, curRect,  movedSet, notMovedSet, shifted, neighbor, cornerIndex, referIndex, headID, relativeLocation, rectList, parentList, directionMark, familySets, screenWidth, screenHeight, screenPosx, screenPosy);		
				flag=true;				
			}			
			if(shifted.find(id)!=shifted.end() || flag)
			{
			    		
			}			
			if(flag)
			{
				
				break;
			}
		}
		if(!flag)
			break;
	}
	
}

bool Group::overlaid(ItemBase *left, ItemBase *right)
{
	int disX = abs( left->sceneBoundingRect().center().x() - right->sceneBoundingRect().center().x() );
	int disY = abs( left->sceneBoundingRect().center().y() - right->sceneBoundingRect().center().y() );
	
	//int disX = abs( left->realRect().x() - right->realRect().x() );
	//int disY = abs( left->realRect().y() - right->realRect().y() );
	int thresh = ( left->realRect().width() + right->realRect().width() ) / 2;
	int thresh2 = ( left->realRect().height() + right->realRect().height() ) / 2;
	if( disX < thresh && disY < thresh2 )
	{
		return true;
	}
	return false;
	//return(RectRect(left->pos().x(),left->pos().y(),right->pos().x(),right->pos().y(),left->Width(),left->Height(),right->Width(),right->Height()));
}

bool Group::overlaid(QRectF left, QRectF right)
{
	int disX = abs( left.center().x() - right.center().x() );
	int disY = abs( left.center().y() - right.center().y() );
	
	//int disX = abs( left->realRect().x() - right->realRect().x() );
	//int disY = abs( left->realRect().y() - right->realRect().y() );

	int thresh = ( left.width() + right.width() ) / 2;
	int thresh2 = ( left.height() + right.height() ) / 2;
	if( disX < thresh && disY < thresh2 )
	{
		return true;
	}
	return false;
	//return(RectRect(left->pos().x(),left->pos().y(),right->pos().x(),right->pos().y(),left->Width(),left->Height(),right->Width(),right->Height()));
}

bool Group::overlaid(int l, int r, vector<QRectF> rectList, vector<set<int> > familySets)
{   
	set<int> lSet,rSet;
	
	lSet=familySets[l]; rSet=familySets[r];
	
	for(set<int>::iterator rit=rSet.begin(); rit!=rSet.end(); rit++)
	for(set<int>::iterator lit=lSet.begin(); lit!=lSet.end(); lit++)
	{
	    if( overlaid( *lit, *rit, rectList) )
			return true;
	}
	return false;

}

bool Group::overlaid(int l, set<int> movedSet, vector<QRectF> rectList, vector<set<int> > familySets)
{   
	set<int> lSet,rSet;	
	lSet=familySets[l]; 	
	for(set<int>::iterator it=movedSet.begin(); it!=movedSet.end(); it++)
	{
	    if(overlaid(l, *it, rectList, familySets))
			return true;
	}
	return false;
}



bool Group::overlaid(int l, int r, vector<QRectF> rectList)
{
	QRectF left=rectList[l],right=rectList[r];	
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

bool Group::isTwoRectConnected(QRectF justMovedRect, QRectF toBeMovedRect)
{
	int tar_size = justMovedRect.width();
	int cur_size = toBeMovedRect.width();
	int tar_size2 = justMovedRect.height();
	int cur_size2 = toBeMovedRect.height();
	QPointF tar = QPointF(justMovedRect.center().x(), justMovedRect.center().y());
	QPointF cur = QPointF( toBeMovedRect.center().x(), toBeMovedRect.center().y());
    if( !outBox( tar, tar_size, tar_size2, cur, cur_size, cur_size2 ) )
		return true;
	else
		return false;
}

QPointF Group::getSmallestDistance(QRectF justMovedRect, QRectF toBeMovedRect)
{	
	//get all dis justMovedRect toBeMovedRect	
	vector<QPointF> jPos, tPos;
	QPointF oldPos=QPointF(toBeMovedRect.x(), toBeMovedRect.y());	

	jPos.resize(4); tPos.resize(4);
	
	jPos[0]=QPointF(justMovedRect.x(), justMovedRect.y());
	jPos[1]=QPointF(justMovedRect.x()+justMovedRect.width(), justMovedRect.y());
	jPos[2]=QPointF(justMovedRect.x()+justMovedRect.width(), justMovedRect.y() + justMovedRect.height());
	jPos[3]=QPointF(justMovedRect.x(), justMovedRect.y() + justMovedRect.height());

	tPos[0]=QPointF(toBeMovedRect.x(), toBeMovedRect.y());
	tPos[1]=QPointF(toBeMovedRect.x()+toBeMovedRect.width(), toBeMovedRect.y());
	tPos[2]=QPointF(toBeMovedRect.x()+toBeMovedRect.width(), toBeMovedRect.y() + toBeMovedRect.height());
	tPos[3]=QPointF(toBeMovedRect.x(), toBeMovedRect.y() + toBeMovedRect.height());

	QPointF minPos, tempPos;
	int min=10000000, temp;
	for(int j=0; j<4; j++)
	for(int t=0; t<4; t++)
	{ 
		tempPos=jPos[j]-tPos[t];	
		temp=pow(float(tempPos.x()),2) + pow(float(tempPos.y()),2);
		if(min>temp )
		{
			min=temp;
			minPos= oldPos + tempPos;
			
		}
	}
	return minPos;
}

void Group::moveToClosest(QRectF justMovedRect, QRectF &toBeMovedRect, int direction, bool disperse)
{
	/*vector<int> mark;
	for(int i=0; i<4; i++)
    {
		int ri=(i+2)%4;
		ri= pow(2.0,ri);
		if(direction & ri )
		   mark.push_back(0);
		else 
		   mark.push_back(1);			
	}
	
	vector<QPointF> newPos;		
	QPointF oldPos, tempPos;
	vector<int> disList;	
	vector<QRectF> possibleRect;
	
	//oldPos = QPointF(toBeMoved[0].x(), toBeMoved[0].y());
	for(int i=0; i<4; i++)
	{
		float distance;
		QPointF tempPos=QPointF(-1000000,-1000000);
		if(mark[i]) //always could move to right
	       tempPos = moveOneorTwoSteps(eventType, justMovedRect, toBeMovedRect, i, disperse);
	    		
		//QPointF dis = tempPos-oldPos;		
		possibleRect.push_back(QRectF( tempPos.x(), tempPos.y(), toBeMovedRect.width(), toBeMovedRect.height()));		    				
	}

	int mini=-1,min=1000000;
	for(int i=0; i< possibleRect.size(); i++)
	{
		float dx,dy;
		dx = possibleRect[i].x()-toBeMovedRect.x();
		dy = possibleRect[i].y()-toBeMovedRect.y();
		float dis=sqrt(float(dx*dx + dy*dy));
		
	    if(min>dis)
		{
		   min=dis;
		   mini=i;
		}
	}
	
    if(mini!=-1)
    {
	   toBeMovedRect=possibleRect[mini];	
	}
	*/
}


QPointF Group::moveOneorTwoSteps(int eventType, QRectF justMovedRect, QRectF toBeMovedRect,  int direction, int &lineuped, bool disperse, float parentX)
{
	    lineuped=0;
	    float toTop, justTop, toLeft, justLeft, dis, toBottom, justBottom;
	    QPointF tempPos=QPointF(0,0);		
		QPointF oldPos=QPointF(toBeMovedRect.x(), toBeMovedRect.y());	
		if( eventType!=GROUPOVERLAID && (toBeMovedRect.width()==justMovedRect.width() && toBeMovedRect.height()==justMovedRect.height()))// same size line up
		{
			lineuped=2;
			switch(direction)
			{
				case 0: tempPos = QPointF(justMovedRect.x() - toBeMovedRect.width(), justMovedRect.y()); 	   				   			    					    
						break; //left
				case 1: /*toLeft= toBeMovedRect.y();
						justLeft= justMovedRect.y();
						dis=fabs(toLeft-justLeft);
						if(dis<justMovedRect.width()/2.0)*/
					    tempPos = QPointF(justMovedRect.x(), justMovedRect.y()-toBeMovedRect.height());  
						lineuped=1;
						//else tempPos = QPointF(toBeMovedRect.x(), justMovedRect.y()-toBeMovedRect.height());    				    						
						break;  //down
				case 2: tempPos = QPointF(justMovedRect.x() + justMovedRect.width(), justMovedRect.y());  					    
						break; //right
				case 3: /*toLeft= toBeMovedRect.y();
						justLeft= justMovedRect.y();
						dis=fabs(toLeft-justLeft);
						if(dis<justMovedRect.width()/2.0)*/
					    tempPos = QPointF(justMovedRect.x(), justMovedRect.y()+justMovedRect.height());     
						lineuped=1;
						//else tempPos = QPointF(toBeMovedRect.x(), justMovedRect.y()+justMovedRect.height());     											    
					    break; //up
			}
		}
		else if(eventType!=GROUPOVERLAID && eventType!=BUBBLECREATED )
		{
			switch(direction)
			{
                        case 0: toTop= toBeMovedRect.y();
						justTop= justMovedRect.y();
						dis=fabs(toTop-justTop);
						if(dis<justMovedRect.height()/3.0 && dis<toBeMovedRect.height()/2.0)
						{
							tempPos = QPointF(justMovedRect.x() - toBeMovedRect.width(), justMovedRect.y()); //top/or bottom alignment	   				   			    
							lineuped=4;
						}
						else 
						{
							toBottom=toBeMovedRect.y() + toBeMovedRect.height(), justBottom=justMovedRect.y() + justMovedRect.height();
							dis=fabs(toBottom-justBottom);
							if(dis<justMovedRect.height()/3.0 && dis<toBeMovedRect.height()/2.0)
							{
								tempPos = QPointF(justMovedRect.x() - toBeMovedRect.width(), justBottom-toBeMovedRect.height()); 
								lineuped=4;
							}
							else tempPos = QPointF(justMovedRect.x() - toBeMovedRect.width(), toBeMovedRect.y()); 	   				   			    													
			            }						
						break; //left //
				case 1: 
						tempPos = QPointF(justMovedRect.x(), justMovedRect.y()-toBeMovedRect.height());
						lineuped=3;
						break;  //down //left alignment
				case 2: toTop= toBeMovedRect.y();
						justTop= justMovedRect.y();
						dis=fabs(toTop-justTop);
						if(dis<justMovedRect.height()/3.0 && dis<toBeMovedRect.height()/3.0)
						{
							tempPos = QPointF(justMovedRect.x() + justMovedRect.width(), justMovedRect.y());  					    
							lineuped=4;
						}
						else 
						{
							toBottom=toBeMovedRect.y() + toBeMovedRect.height(), justBottom=justMovedRect.y() + justMovedRect.height();
							dis=fabs(toBottom-justBottom);
							if(dis<justMovedRect.height()/3.0 && dis<toBeMovedRect.height()/2.0)
							{
								tempPos = QPointF(justMovedRect.x() + justMovedRect.width(), justBottom-toBeMovedRect.height()); 
								lineuped=4;
							}
							else tempPos = QPointF(justMovedRect.x() + justMovedRect.width(),toBeMovedRect.y());  	//top or bottom alignment				    
						}
						break; //right //
				case 3: tempPos = QPointF(justMovedRect.x(), justMovedRect.y()+justMovedRect.height());     
					     lineuped=3;
						break; //up //left alignment
			}
		}
		else //if(eventType!=GROUPOVERLAID ) do not need to line up
		{
			switch(direction)
			{
				case 0: tempPos = QPointF(justMovedRect.x() - toBeMovedRect.width(), toBeMovedRect.y()); 	   				   			    							
						break; //left //
				case 1: tempPos = QPointF(toBeMovedRect.x(), justMovedRect.y()-toBeMovedRect.height());     				    
						break;  //down 
				case 2: tempPos = QPointF(justMovedRect.x() + justMovedRect.width(),toBeMovedRect.y());  					    
						break; //right //
				case 3: tempPos = QPointF(toBeMovedRect.x(), justMovedRect.y()+justMovedRect.height());     
						break; //up 
			}
		}
		if(!disperse)
		{	
			QPointF dis = tempPos-oldPos;
			toBeMovedRect = QRectF(toBeMovedRect.x()+dis.x(), toBeMovedRect.y()+dis.y(), toBeMovedRect.width(), toBeMovedRect.height());				
			if( !isTwoRectConnected(justMovedRect, toBeMovedRect) )
			{
				//get all dis to toBeMovedRect
		 		tempPos=getSmallestDistance(justMovedRect, toBeMovedRect);  
				lineuped=0;
			}			
		}
		return tempPos;
}

QPointF Group::moveMoreSteps(int eventType, int justMovedID, int toBeMovedID, set<int> justMoved, vector<QRectF> rectList, int direction, int &lineuped, bool disperse, float parentX)
{
	
	QRectF justMovedRect, toBeMovedRect;
	QPointF tempPos;
	QPointF dis;
	bool oneDirection=true;
	lineuped=0;

	while(!justMoved.empty())
	{
		justMovedRect=rectList[justMovedID];
		toBeMovedRect=rectList[toBeMovedID];
		if(!oneDirection)
		{
			vector<QPointF> poslist;
			vector<int> lineupRecord;
			for(int i=0; i<4; i++)
	        {
				if(i!=(direction+2)%4)
				{
				   poslist.push_back(moveMoreSteps(eventType, justMovedID, toBeMovedID, justMoved, rectList, i, lineuped, disperse, parentX));		
				   lineupRecord.push_back(lineuped);
				}
		    }
			int mini=0;
			float min=100000000,s;
			for(int i=0; i<poslist.size(); i++)
			{
				dis = poslist[i] - QPointF(toBeMovedRect.x(), toBeMovedRect.y());
				s = sqrt(dis.x()*dis.x() + dis.y()* dis.y());
				if(s<min)
					min=s, mini=i, lineuped=lineupRecord[i];
			}		    
			rectList[toBeMovedID] = QRectF(poslist[mini].x(), poslist[mini].y(), rectList[toBeMovedID].width(), rectList[toBeMovedID].height());	
			tempPos = poslist[mini];
		}
		else
		{
			tempPos = moveOneorTwoSteps(eventType, justMovedRect, toBeMovedRect, direction, lineuped, disperse, parentX);			
			rectList[toBeMovedID] = QRectF(tempPos.x(), tempPos.y(), rectList[toBeMovedID].width(), rectList[toBeMovedID].height());	
			oneDirection=false;
		}
		
		justMoved.erase(justMovedID);
		bool isOverLaid=false;
		for(set<int>::iterator it=justMoved.begin(); it!=justMoved.end(); it++)	
		{
		    if(overlaid(*it, toBeMovedID, rectList))
		    {
				isOverLaid=true;		
				justMovedID=*it;	
		
				break;
			}
		}
		if(!isOverLaid)
			break;
	}		
	return tempPos;
}


QPointF Group::moveFamilyOneDirection(int eventType, int justMovedFID, int toBeMovedFID, set<int> movedSet, vector<QRectF> rectList, vector<int > parentList, vector<set<int> > familySets, int direction, int &lineuped, bool disperse, int screenWidth, int screenHeight, int screenPosx, int screenPosy)
{ //move the tobemoved family at one direction through justmoved family and all family inside movedSet
	//QRectF toBeMoved=rectList[toBeMovedID],justMoved;	
	
	if(justMovedFID >= rectList.size())
	{
		justMovedFID=justMovedFID;	
	}
	if(toBeMovedFID >= rectList.size())
	{
		toBeMovedFID=toBeMovedFID;	
	}
	set<int> toBeMoved=familySets[toBeMovedFID];
	set<int> justMoved;//then added it with all the bubble id in movedSet	
	int justMovedID=*familySets[justMovedFID].begin();
	int toBeMovedID=*familySets[toBeMovedFID].begin();

	for(set<int>::iterator fit = movedSet.begin(); fit!=movedSet.end(); fit++)	
	{
		set<int> family=familySets[*fit];
		for(set<int>::iterator it = family.begin(); it!=family.end(); it++)	
	        justMoved.insert(*it); 
	}

	//vector<QRectF> toBeMoved = getFamiltyRects(toBeMovedID, toBeMovedF, rectList);
	QPointF tempPos,oldPos;
	vector<QPointF> possiblePos;
	vector<int> lineupRecord;
	QRectF justMovedRect;
	
	for(set<int>::iterator jit=justMoved.begin(); jit!=justMoved.end(); jit++)
	{
		for(set<int>::iterator tit=toBeMoved.begin(); tit!=toBeMoved.end(); tit++)	
		{
			//justMoved=rectList[justMovedID];	
            QRectF justMovedRect = rectList[*jit];	
			QRectF toBeMovedRect = rectList[*tit];	
			int pid=parentList[*tit];
			//pid=0;
			//float px= (pid>=0 && (eventType == BUBBLECREATED || eventType == SUBBUBBLECREATED))? rectList[pid].bottomRight().x():-1000000;
			float px= (pid>=0 && (eventType == BUBBLECREATED || eventType == SUBBUBBLECREATED))? rectList[pid].bottomRight().x():-1000000;
			QRectF prect= (pid>=0 && (eventType == SUBTREERINGCREATED))? rectList[pid]:QRectF(-100000,-10000,0,0);

			QPointF oldPos=QPointF(toBeMovedRect.x(), toBeMovedRect.y());
					
			if(overlaid(justMovedRect, toBeMovedRect))
				tempPos = moveMoreSteps(eventType, *jit, *tit, justMoved, rectList, direction, lineuped, disperse, px);
			else 
			    tempPos = moveOneorTwoSteps(eventType, justMovedRect, toBeMovedRect, direction, lineuped, disperse, px);

			QPointF dis = tempPos-oldPos;
			/*if(eventType==BUBBLECREATED) //adjust new bubble location against the windows border
			{
			     //if(getMostLeftPos(familySets[toBeMovedID], RectList) < 10 + screenPosx && tempPos.x()<0)
				 if(tempPos.x() < 10 + screenPosx)
				 {	

					tempPos=QPointF(-100000, 0);	
					dis = tempPos-oldPos;
				 }
				 //if( getMostLowPos(familySets[toBeMovedID], RectList)  < 60 + screenPosy && tempPos.y()<0)
				 else if( tempPos.y() < 60 + screenPosy)
				 {	
					 tempPos=QPointF(-100000, 0);	
					 dis = tempPos-oldPos;
				 }			
				 //if( getMostHighPos(familySets[toBeMovedID], RectList) > screenHeight - 100 && tempPos.y()>0)
				 else if(  tempPos.y() > screenHeight - 100)
				 {	
					 tempPos=QPointF(-100000, 0);	
					 dis = tempPos-oldPos;
				 }										
			}
			else */
			if(eventType!=BUBBLERESIZED )
			{
				if(tempPos.x()<px)
			    {
					//passParent=true;		
					tempPos=QPointF(-100000, 0);	
					dis = tempPos-oldPos;
			    }
				else if(eventType==SUBTREERINGCREATED)
				{
					if(*tit!=pid)
					{
						QRectF tempRect = QRectF(tempPos.x(), tempPos.y(), toBeMovedRect.width(), toBeMovedRect.height());					
						if(prect.intersects(tempRect))
						{
						   tempPos=QPointF(-100000, 0);	
						   dis = tempPos-oldPos;					
						}
					}
				}
			}
			
			//update pos of tobemoved family
			//toBeMovedRect = QRectF(tempPos.x(),tempPos.y(), toBeMovedRect.width(), toBeMovedRect.height());
			vector<QRectF> tempRectList=rectList;
			//QPointF dis = tempPos-oldPos;
			for(set<int>::iterator it=toBeMoved.begin(); it!=toBeMoved.end(); it++)
			{
				int id=*it;
				tempRectList[id] = QRectF(rectList[id].x()+dis.x(), rectList[id].y()+dis.y(), rectList[id].width(), rectList[id].height());	
			}		
			bool foundOverLap=false;
			bool gapped=false;		

			for(set<int>::iterator jt=toBeMoved.begin(); jt!=toBeMoved.end(); jt++)	
			for(set<int>::iterator it=justMoved.begin(); it!=justMoved.end(); it++)
			{
				if(overlaid(*it, *jt, tempRectList))
				{
					foundOverLap=true;
					break;
				}
			}
			if(!disperse)
			{
				   vector<QRectF> tList;
				   for(set<int>::iterator it=justMoved.begin(); it!=justMoved.end(); it++)
					   tList.push_back(tempRectList[*it]);

				   for(set<int>::iterator it=toBeMoved.begin(); it!=toBeMoved.end(); it++)
					   tList.push_back(tempRectList[*it]);

				   if(isbridgeBroken(tList))
				   {
						gapped=true;
				   }
			}
			if(!foundOverLap && !gapped)
			{
				lineupRecord.push_back(lineuped);	
				possiblePos.push_back(dis);	
			}

	     }		    
	}
	//find the smallest movedpos;
	int mini=-1,min=10000000;
	for(int i=0; i<possiblePos.size(); i++)
	{
		int dis=sqrt(float(possiblePos[i].x()*possiblePos[i].x() + possiblePos[i].y()*possiblePos[i].y()));		
	    if(min>dis)
		{
		   min=dis;
		   mini=i;
		   lineuped=lineupRecord[i];
		}
	}
	
	if(mini==-1)
    {
	   lineuped=0;
	   return QPointF(-100000, 0);	
	}
	else 
		return possiblePos[mini];
}

QPointF  Group::moveOneDirection(int justMovedID, int toBeMovedID, set<int> movedSet, vector<QRectF> rectList, int direction)
{
	QRectF toBeMoved=rectList[toBeMovedID],justMoved;
	QPointF tempPos=QPointF(toBeMoved.x(),toBeMoved.y());	
	bool flag=true;
	while(flag)	
	{
		justMoved=rectList[justMovedID];
	    switch(direction)
		{
			case 0: tempPos = QPointF(justMoved.x()-toBeMoved.width(), toBeMoved.y()); 	    break; //left
			case 1: tempPos = QPointF(toBeMoved.x(),justMoved.y()-toBeMoved.height());      break;  //down
			case 2: tempPos = QPointF(justMoved.x() + justMoved.width() ,toBeMoved.y());  	break; //right
			case 3: tempPos = QPointF(toBeMoved.x(), justMoved.y()+justMoved.height());     break; //up
		}
		toBeMoved=QRectF(tempPos.x(),tempPos.y(), toBeMoved.width(), toBeMoved.height());
		rectList[toBeMovedID]=toBeMoved;		
		flag=false;
		for(set<int>::iterator it=movedSet.begin(); it!=movedSet.end(); it++)
		{
			if(justMovedID!=*it && overlaid(*it, toBeMovedID, rectList))
		    {
				justMovedID=*it;
			    flag=true;
				break;
			}
		}
	}	
	return tempPos;
}


QPointF Group::moveOverlaid(int eventType, int justMovedID, int toBeMovedID, set<int> &movedSet, set<int> &notMovedSet, vector<QRectF> &rectList, vector<int> parentList, vector<int> &directionMark, vector<set<int> > familySets, int screenWidth, int screenHeight, int screenPosx, int screenPosy, bool disperse)
{   //if justMoved intersect with a bubble family in notMovedSet
	//move this bubble family i to a place which make it not interseted with any bubble in movedSet
	vector<int> mark1, mark2, mark; 
	for(int i=0; i<4; i++)
    {
		int ri=(i+2)%4;
		ri= pow(2.0,ri);
		if(directionMark[justMovedID] & ri )
		   mark1.push_back(0);
		else 
		   mark1.push_back(1);	

		if(directionMark[toBeMovedID] & ri )
		   mark2.push_back(0);
		else 
		   mark2.push_back(1);	

		mark.push_back(0);
	}
	
	vector<QPointF> newPos;		
	QPointF oldPos, tempPos;
	vector<int> disList;	
	vector<QRectF> RectList;
	int lineuped=0;
	//oldPos = QPointF(toBeMoved[0].x(), toBeMoved[0].y());
	for(int i=0; i<4; i++)
	{
		float distance;
		if(i==2 || mark1[i] && mark2[i]) //always could move to right
		{	  
			tempPos = moveFamilyOneDirection(eventType, justMovedID, toBeMovedID, movedSet, rectList, parentList, familySets, i, lineuped, disperse, screenWidth, screenHeight, screenPosx, screenPosy); //tempPos: relative (moved) location
		    mark[i]=pow(2.0,i);		
			RectList=rectList;
			for(set<int>::iterator it=familySets[toBeMovedID].begin(); it!=familySets[toBeMovedID].end(); it++)	
			{
				int id=*it;
     			RectList[id] = QRectF(RectList[id].x()+tempPos.x(), RectList[id].y()+tempPos.y(), RectList[id].width(), RectList[id].height());	
			}	
			//screen boundary constraint: if it does not fit the constrainly previously, allow the move if it levigate the contraint	
			distance = sqrt(float(tempPos.x()*tempPos.x() + tempPos.y()*tempPos.y())); 	
			if(lineuped==1)
				distance = distance * 0.5;
			else if(lineuped==2)
				distance = distance * 0.6;
			else if(lineuped==3)
				distance = distance * 0.68;
			else if(lineuped==4)
				distance = distance * 0.78;
			/*if(eventType==BUBBLECREATED || (eventType==SUBTREERINGCREATED) //adjust new bubble location against the windows border
			{
			     //if(getMostLeftPos(familySets[toBeMovedID], RectList) < 10 + screenPosx && tempPos.x()<0)
				  if(getMostLeftPos(familySets[toBeMovedID], RectList) + tempPos.x() < 10 + screenPosx)
					 distance = 10000000; 									 				 
				 //if( getMostLowPos(familySets[toBeMovedID], RectList)  < 60 + screenPosy && tempPos.y()<0)
				  if( getMostLowPos(familySets[toBeMovedID], RectList) + tempPos.y() < 60 + screenPosy)
				    distance = 10000000; 			
				 //if( getMostHighPos(familySets[toBeMovedID], RectList) > screenHeight - 100 && tempPos.y()>0)
				  if( getMostHighPos(familySets[toBeMovedID], RectList) + tempPos.y() > screenHeight - 100)
				    distance = 10000000; 										
			}
			else*/
			{
				switch(i)
				{
					case 0:  //if(getMostRightPos(familySets[toBeMovedID], RectList) < 10 + screenPosx && tempPos.x()<0)
						     if(getMostRightPos(familySets[toBeMovedID], RectList) + tempPos.x() < 10 + screenPosx)
							 {
								 distance = 10000000; 									 				 
							 }
							 break; //move left
					case 1:  if( getMostHighPos(familySets[toBeMovedID], RectList) + tempPos.y() < 60 + screenPosy)
								  distance = 10000000; 			
							 break; //move down					 
					case 2:  break;  //move right
 					case 3:  if( getMostLowPos(familySets[toBeMovedID], RectList) + tempPos.y()> screenHeight -100 )//&& tempPos.y()>0
								  distance = 10000000; 						
							 break;  //move up
				} 
			}
		}
		else 
		{		    
			distance = 10000000; 		
		}
		newPos.push_back(tempPos); 
		disList.push_back(distance);
	}
	//find smallest distance one	
	int mini=0, mind=100000000;
	for(int i=0; i<4; i++)
	{
	    if( disList[i]<mind )
		{
		    mind=disList[i];
			mini=i;
		}
	}
	tempPos=newPos[mini];	
	//toBeMoved=QRectF(tempPos.x(),tempPos.y(), toBeMoved.width(), toBeMoved.height());
	//rectList[toBeMovedID]=toBeMoved;			
	
	for(set<int>::iterator it=familySets[toBeMovedID].begin(); it!=familySets[toBeMovedID].end(); it++)	
	{
		int id=*it;
     	rectList[id] = QRectF(rectList[id].x()+tempPos.x(), rectList[id].y()+tempPos.y(), rectList[id].width(), rectList[id].height());	
	}	
	
	directionMark[toBeMovedID]=mark[mini];
	return tempPos;		
}


QRectF Group::moveOverlaid(int justMovedID, int toBeMovedID, set<int> &movedSet, set<int> &notMovedSet, vector<QRectF> &rectList, vector<int> &directionMark, int screenWidth, int screenHeight)
{   //if justMoved intersect with a bubble in notMovedSet
	//move this bubble i to a place which make it not interseted with any bubble in movedSet
	// not consider family sets
	//disperse: true, false: tighten
	vector<int> mark1, mark2, mark; 
	for(int i=0; i<4; i++)
    {
		int ri=(i+2)%4;
		ri= pow(2.0,ri);
		if(directionMark[justMovedID] & ri )
		   mark1.push_back(0);
		else 
		   mark1.push_back(1);	

		if(directionMark[toBeMovedID] & ri )
		   mark2.push_back(0);
		else 
		   mark2.push_back(1);	

		mark.push_back(0);
	}
		
	//test four direction
	QRectF toBeMoved=rectList[toBeMovedID], justMoved=rectList[justMovedID];
	vector<QPointF> newPos;		
	QPointF oldPos=QPointF(toBeMoved.x(),toBeMoved.y()),tempPos;
	vector<int> disList;	
	for(int i=0; i<4; i++)
	{
		float distance;
		if(i==2 || mark1[i] && mark2[i]) //always could move to right
		{	        
		    switch(i)
			{
			    case 0:   tempPos = moveOneDirection(justMovedID, toBeMovedID, movedSet,  rectList, 0);
					     //tempPos = QPointF(justMoved.x()-toBeMoved.width(),toBeMoved.y()); 					    
						 mark[i]=1;			     						 
						 break; //move left
			    case 1: tempPos = moveOneDirection(justMovedID, toBeMovedID, movedSet,  rectList, 1);
					     //QPointF(toBeMoved.x(),justMoved.y()-toBeMoved.height()); 					  
					     mark[i]=2;
						 break;  //move down
			    case 2:  tempPos = moveOneDirection(justMovedID, toBeMovedID, movedSet,  rectList, 2);
					     mark[i]=4;					      
						 break;  //move right
 			    case 3:  tempPos = moveOneDirection(justMovedID, toBeMovedID, movedSet,  rectList, 3);					     
					     mark[i]=8;
					     break;  //move up
			}
			 newPos.push_back(tempPos); 
			
			//screen boundary constraint			
			switch(i)
			{
			    case 0:  if(tempPos.x() < 10)
						     distance = 10000000; 			
						 else
						 { tempPos = tempPos-oldPos; 
								 distance = sqrt(float(tempPos.x()*tempPos.x() + tempPos.y()*tempPos.y())); 
						 }
						 break; //move left
			    case 1:  if( tempPos.y() < 50 )
				               distance = 10000000; 			
						 else
						 {
							  tempPos = tempPos-oldPos; 
							 distance = sqrt(float(tempPos.x()*tempPos.x() + tempPos.y()*tempPos.y())); 					    
						 }
						 break;  //move down
			    case 2:   tempPos = tempPos-oldPos; 
					     distance = sqrt(float(tempPos.x()*tempPos.x() + tempPos.y()*tempPos.y())); 					    				      
						 break;  //move right
 			    case 3:  if( tempPos.y() + toBeMoved.height() > screenHeight -50)
				              distance = 10000000; 						
						 else 
					     {
							  tempPos = tempPos-oldPos; 
							  distance = sqrt(float(tempPos.x()*tempPos.x() + tempPos.y()*tempPos.y())); 					     
						 }
					     break;  //move up
			} 
		}
		else 
		{
		    newPos.push_back(tempPos);   
			distance = 10000000; 		
		}
		disList.push_back(distance);
	}
	//find smallest distance one	
	int mini=0, mind=100000000;
	for(int i=0; i<4; i++)
	{
	    if( disList[i]<mind )
		{
		    mind=disList[i];
			mini=i;
		}
	}
	tempPos=newPos[mini];
	toBeMoved=QRectF(tempPos.x(),tempPos.y(), toBeMoved.width(), toBeMoved.height());

	rectList[toBeMovedID]=toBeMoved;
	directionMark[toBeMovedID]=mark[mini];
	return toBeMoved;
		
}


QRectF Group::moveOverlaid(int justMovedID, int toBeMovedID, vector<QRectF> &rectList, vector<int> &directionMark)
{ 
	vector<int> mark1, mark2, mark; 
	for(int i=0; i<4; i++)
    {
		int ri=(i+2)%4;
		ri= pow(2.0,ri);
		if(directionMark[justMovedID] & ri )
		   mark1.push_back(0);
		else 
		   mark1.push_back(1);	

		if(directionMark[toBeMovedID] & ri )
		   mark2.push_back(0);
		else 
		   mark2.push_back(1);	

		mark.push_back(0);
	}
		
	//test four direction
	QRectF toBeMoved=rectList[toBeMovedID], justMoved=rectList[justMovedID];
	vector<QPointF> newPos;		
	QPointF oldPos=QPointF(toBeMoved.x(),toBeMoved.y()),tempPos;
	vector<int> disList;	
	for(int i=0; i<4; i++)
	{
		float distance;
		if(mark1[i] && mark2[i])
		{	        
		    switch(i)
			{
			    case 0:  tempPos = QPointF(justMoved.x()-toBeMoved.width(),toBeMoved.y()); mark[i]=1;			     						 
						 break; //move left
			    case 1:  tempPos = QPointF(toBeMoved.x(),justMoved.y()-toBeMoved.height()); mark[i]=2;
						 break;  //move down
			    case 2:  tempPos = QPointF(justMoved.x() + justMoved.width() ,toBeMoved.y()); mark[i]=4;					      
						 break;  //move right
 			    case 3:  tempPos = QPointF(toBeMoved.x(), justMoved.y()+justMoved.height()); mark[i]=8;
					     break;  //move up
			}
			newPos.push_back(tempPos); 
			tempPos = tempPos-oldPos; 
			distance = sqrt(float(tempPos.x()*tempPos.x() + tempPos.y()*tempPos.y())); 
		}
		else 
		{
		    newPos.push_back(tempPos); 
			distance = 10000000; 		
		}
		disList.push_back(distance);
	}
	//find smallest distance one	
	int mini=0, mind=10000000;
	for(int i=0; i<4; i++)
	{
	    if( disList[i]<mind )
		{
		    mind=disList[i];
			mini=i;
		}
	}
	tempPos=newPos[mini];
	toBeMoved=QRectF(tempPos.x(),tempPos.y(), toBeMoved.width(), toBeMoved.height());

	rectList[toBeMovedID]=toBeMoved;
	directionMark[toBeMovedID]=mark[mini];
	return toBeMoved;
}


void Group::insertIntoLineUpList( int index )
{   //unsert a bubble to the lineup
	int ti, tj, esize, esize2, inLi, inLj, i, j;
	QPointF firstPos, nowPos, tmpPos;

	esize = (this->_members[0]->Width()+this->_members[index]->Width())/2;
	esize2 = (this->_members[0]->Height()+this->_members[index]->Height())/2;

	//firstPos = this->_members[lineUpList[0][0]]->pos();
	firstPos = this->_members[lineUpList[0][0]]-> pos();//sometimes the pos() is 
	nowPos = this->_members[index]->pos();

	tj = (int)((nowPos.x() - firstPos.x()) / (esize+10));
	ti = (int)((nowPos.y() - firstPos.y()) / (esize2+10));

	tmpPos.setX( firstPos.x() + esize2*tj );
	tmpPos.setY( firstPos.y() + esize*ti );

	if ( nowPos.y() < tmpPos.y() - esize2/2 )
	{
		inLi = -1;
	}
	else if ( nowPos.y() > tmpPos.y() + esize2/2 )
	{
		inLi = 1;
	}
	else
	{
		inLi = 0;
	}

	if ( nowPos.x() < tmpPos.x() - esize/2 )
	{
		inLj = -1;
	}
	else if ( nowPos.x() > tmpPos.x() + esize/2 )
	{
		inLj = 1;
	}
	else
	{
		inLj = 0;
	}

	if ( ti + inLi == -1 )
	{
		QList<int> tmp;
		tmp.append( index );
		lineUpList.insert( 0, tmp );
		this->getMemberAt(index)->setTargetPos( QPointF(firstPos.x(), tmpPos.y()-esize2 ) );
	}
	else if ( ti+inLi == lineUpList.size() )
	{
		QList<int> tmp;
		tmp.append( index );
		lineUpList.append( tmp );
		this->getMemberAt(index)->setTargetPos( QPointF(firstPos.x(), tmpPos.y()+esize2 ) );
	}
	else if(inLi + ti>=0)
	{
		if ( inLj + tj < 0 )
		{
			QPointF ppos = this->getMemberAt( lineUpList[inLi+ti][0] )->pos();
			this->lineUpList[inLi+ti].insert( 0, index );
			this->getMemberAt(index)->setTargetPos( QPointF( ppos.x()-esize, tmpPos.y()+inLi*esize2 ) );
		}
		else if ( inLj + tj >= this->lineUpList[inLi + ti].size())
		{
			QPointF ppos = this->getMemberAt( lineUpList[inLi+ti][this->lineUpList[inLi + ti].size()-1] )->pos();
			this->lineUpList[inLi+ti].append( index );
			this->getMemberAt(index)->setTargetPos( QPointF( ppos.x()+esize, tmpPos.y()+inLi*esize2 ) );
		}
		else
		{
			if (inLj < 0)
			{
				lineUpList[inLi+ti].insert( tj, index );

				for (i = 0; i < tj; i ++)
				{
					int tindex = lineUpList[inLi+ti][i];
					QPointF ppos = this->getMemberAt(tindex)->pos();
					this->getMemberAt(tindex)->setTargetPos( QPointF( ppos.x()-esize, ppos.y() ) );
				}
				this->getMemberAt(index)->setTargetPos( QPointF(tmpPos.x()+inLj*esize, tmpPos.y()+inLi*esize2 ) );
			}
			else
			{
				this->lineUpList[inLi + ti].insert( inLj+tj, index );
				for (i = inLj+tj+1; i < lineUpList[inLi+ti].size(); i ++)
				{
					int tindex = lineUpList[inLi+ti][i];
					QPointF ppos = this->getMemberAt(tindex)->pos();
					this->getMemberAt(tindex)->setTargetPos( QPointF( ppos.x()+esize, ppos.y() ) );
				}
				this->getMemberAt(index)->setTargetPos( QPointF(tmpPos.x()+inLj*esize, tmpPos.y()+inLi*esize2 ) );
			}
		}
	}
}

void Group::updateLineUpListAdd( int ibegin, int iend )
{
	if (this->layoutType != LINEUP)
	{
		return;
	}
	int st, i;

	if (this->lineUpList.size() == 0)
	{
		QList<int> tmp;
		tmp.append( ibegin );
		this->lineUpList.append( tmp );
		st = ibegin+1;
	}
	else
	{
		st = ibegin;
	}

	/*for ( i = st; i < iend; i ++ )
	{
		insertIntoLineUpList( i );
	}*/
	//lineUpDisplay(0, iend);

	updateBoarderState();
}



bool Group::isbridgeBroken(ItemBase* init)
{	
	vector< set <ItemBase *> > groupSeg = getGroupSegment(init);
	if(groupSeg.size()!=1)
		return true;
	else return false;
}

bool Group::isbridgeBroken(vector<QRectF> rectList)
{	
	vector< set <int> > groupSeg = getGroupSegment(rectList);
	if(groupSeg.size()!=1)
		return true;
	else return false;
}

int Group::getUnionID(vector<set<ItemBase *> > unions, int id)
{
    for(int i=0; i<unions.size(); i++)
	{
	    if(unions[i].find(getMemberAt(i))!=unions[i].end() )
	        return( i );
	}
	return -1;
}

int Group::getUnionID(vector<set<int> > unions, int id)
{
    for(int i=0; i<unions.size(); i++)
	{
	    if(unions[i].find(id)!=unions[i].end() )
	        return( i );
	}
	return -1;
}

bool Group::outBox(QPointF cur, int width1, int height1, QPointF tar, int width2, int height2, int margin)
{//cur tar are center point of the rects
	int half_cur = (int)((float)width1 / 2.0 + 0.5) + margin; 
	int half_tar =  (int)((float)width2 / 2.0 + 0.5) + margin;
	int half_cur2 = (int)((float)height1 / 2.0 + 0.5) + margin; 
	int half_tar2 =  (int)((float)height2 / 2.0 + 0.5) + margin;

	int pointX = 0;
	int pointY = 0;

	if(cur.x()-half_cur >= tar.x()+half_tar)
	{
		pointX += 1;
	}
	if(cur.x()+half_cur <= tar.x()-half_tar)
	{
		pointX += 1;
	}
	//			Y boundary
	if(cur.y()-half_cur2 >= tar.y()+half_tar2)
	{
		pointY += 1;
	}
	if(cur.y()+half_cur2 <= tar.y()-half_tar2)
	{
		pointY += 1;
	}

	//			determine
	if( pointX == 0 && pointY == 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}


vector< set <ItemBase *> > Group::getGroupSegment(ItemBase* init)
{	//check if it is separate in space
	QList<ItemBase*> groupMember=getMembers();
	
	int size=groupMember.size();
	
	vector< set <ItemBase *> > unionSet;
	unionSet.resize(size);
	for(int i=0; i<size; i++)
	{
		unionSet[i].insert(getMemberAt(i));	
	}	
	for(int i=0; i<size; i++)
	{
		int ID=getUnionID(unionSet,i);
         for(int j=i+1; j<size; j++)
		{
			 ItemBase* node=groupMember[j];
		     for(set<ItemBase *>::iterator it=unionSet[ID].begin(); it!=unionSet[ID].end(); it++ )
			 {
				int tar_size = node->Width();
				int cur_size = (*it)->Width();
				int tar_size2 = node->Height();
				int cur_size2 = (*it)->Height();
				QPointF tar = QPointF( node->boundingRect().center().x(), node->boundingRect().center().y());
				QPointF cur = QPointF( (*it)->boundingRect().center().x(), (*it)->boundingRect().center().y());
				if( !outBox( tar, tar_size, tar_size2, cur, cur_size, cur_size2 ) )
				{
					unionSet[ID].insert(node);	
					unionSet[j].clear();
					break;
				}					
			}		
		}
	}
	vector< set <ItemBase *> > result;	
	for(int i=0; i<size; i++)
	{
	     if(!unionSet[i].empty())	
			 result.push_back(unionSet[i]);
	}
	if(result.size()!=1) 
		result=result;
	return result;
}


vector< set <int> > Group::getGroupSegment(vector<QRectF> rectList)
{	//check if it is separate in space
	//QList<ItemBase*> groupMember=getMembers();	
	int size=rectList.size();
	
	vector< set <int> > unionSet;
	unionSet.resize(size);
	for(int i=0; i<size; i++)
	{
		unionSet[i].insert(i);	
	}	
	for(int i=0; i<size; i++)
	{
		
        for(int j=0; j<size; j++)
		{
			 //ItemBase* node=groupMember[j];
			 if(i==j)
				 continue;			 

		     int ID=getUnionID(unionSet,i);	 
			 int JD=getUnionID(unionSet,j);	 

			 if(ID==JD)
				 continue;	

			 if(unionSet[JD].empty())
				 continue;

			 bool flag=false;
		     for(set<int>::iterator it=unionSet[ID].begin(); it!=unionSet[ID].end(); it++ )
			 {
				 for(set<int>::iterator jt=unionSet[JD].begin(); jt!=unionSet[JD].end(); jt++ )
				 {
					int tar_size = rectList[*jt].width();
					int cur_size = rectList[*it].width();
					int tar_size2 = rectList[*jt].height();
					int cur_size2 = rectList[*it].height();
					QPointF tar = rectList[*jt].center();
					QPointF cur = rectList[*it].center();
					if( !outBox( tar, tar_size, tar_size2, cur, cur_size, cur_size2 ) )
					{
					
						if(ID>JD)
						{
						   int temp=JD;
						   JD=ID; ID=temp;
						}
						//join
						for(set<int>::iterator jt1=unionSet[JD].begin(); jt1!=unionSet[JD].end(); jt1++ )
							unionSet[ID].insert(*jt1);	

						unionSet[JD].clear();

						flag=true;
						break;
					}					
				 }
				 if(flag)
					break;
			}	
		}
	}
	vector< set <int> > result;	
	for(int i=0; i<size; i++)
	{
	     if(!unionSet[i].empty())	
			 result.push_back(unionSet[i]);
	}
	if(result.size()!=1) 
		result=result;
	return result;
}

void Group::updateLineUpListRemove( int index )
{
	if (this->layoutType != LINEUP || NumOfMembers() == 0 )
	{
		return;
	}
	int i, j, k, tindex, esize, esize2;
	QPointF tpos;

	k = -1;
	esize = this->getMemberAt(0)->Width();
	esize2 = this->getMemberAt(0)->Height();

	for (i = 0; i < lineUpList.size(); i ++)
	{
		for (j = 0; j < lineUpList[i].size(); j ++)
		{
			if ( lineUpList[i][j] == index )
			{
				lineUpList[i].removeAt( j );
				k = j;
				break;
			}
		}
		if (k > -1)
		{
			for (j = k; j < lineUpList[i].size(); j ++)
			{
				tindex = lineUpList[i][j];
				tpos = this->getMemberAt(tindex)->pos();

				this->getMemberAt(tindex)->setTargetPos( QPointF( tpos.x() - esize, tpos.y() ) );
			}
			break;
		}
	}

	for (i = 0; i < lineUpList.size(); i ++)
	{
		for (j = 0; j < lineUpList[i].size(); j ++)
		{
			if (lineUpList[i][j] > index)
			{
				lineUpList[i][j] --;
			}
		}
	}
	updateBoarderState();
}

void Group::updateLayoutType()
{
	int num = this->NumOfMembers();
	
	if (num == 0)
	{
		return;
	}

	int size = this->_members[0]->Width();
	int size2 = this->_members[0]->Height();
	this->layoutType = LINEUP;

	for (int i = 1; i < this->NumOfMembers(); i ++)
	{
		if (this->_members[i]->Width() != size || this->_members[i]->Height()!= size2)
		{
			//this->layoutType = RANDOM;
			this->layoutType = LINEUP;
			break;
		}
	}

	if (this->layoutType == RANDOM)
	{
		this->lineUpList.clear();
		updateBoarderState();
	}
	else 
	if (this->layoutType == LINEUP)
	{
		if (this->lineUpList.size() == 0)
		{
			updateLineUpListAdd(0, this->NumOfMembers());
		}
	}
}

void Group::updateBoarderState()
{
	int tindex;
	ItemBase* item;

	if ( this->layoutType == LINEUP )
	{
		for (int i = 0; i < lineUpList.size(); i ++)
		{
			for (int j = 0; j < lineUpList[i].size(); j ++)
			{
				tindex = lineUpList[i][j];
				item = this->getMemberAt(tindex);
				if ( i == 0 )
				{
					item->setBoarderUpFlag( true );
				}
				else
				{
					if ( j >= lineUpList[i-1].size() )
					{
						item->setBoarderUpFlag( true );
					}
					else
					{
						item->setBoarderUpFlag( false );
					}
				}

				if ( i== lineUpList.size()-1 )
				{
					item->setBoarderDownFlag( true );
				}
				else
				{
					if ( j >= lineUpList[i+1].size() )
					{
						item->setBoarderDownFlag( true );
					}
					else
					{
						item->setBoarderDownFlag( false );
					}
				}

				if ( j == 0 )
				{
					item->setBoarderLeftFlag( true );
				}
				else
				{
					item->setBoarderLeftFlag( false );
				}

				if ( j == lineUpList[i].size()-1 )
				{
					item->setBoarderRightFlag( true );
				}
				else
				{
					item->setBoarderRightFlag( false );
				}
			}
		}
	}
	else if (this->layoutType == RANDOM)
	{
		for (int i = 0; i < NumOfMembers(); i ++)
		{
			item = this->getMemberAt( i );

			item->setBoarderDownFlag( true );
			item->setBoarderLeftFlag( true );
			item->setBoarderRightFlag( true );
			item->setBoarderUpFlag( true );
		}
	}
}

void Group::drawGroupBoarder(QPainter *painter)
{
	if( NumOfMembers() == 0)
		return;

	ItemBase* item = this->getMemberAt(0);
	QRegion reg, tmpreg;
	
	int dx, dy;
	painter->setBrush(Qt::NoBrush);
	QPen pen;
	pen.setColor(QColor( item->getColor().a, item->getColor().b, item->getColor().c, item->getAlpha()) );
	pen.setWidth(2);
	painter->setPen( pen );
	bool flag=false;
	for ( int i = 0; i < NumOfMembers(); i ++ )
	{
		item = this->getMemberAt(i);
		if(item->isUnGroup())
		{
		   flag=true;
		}		
		if(item->isVisible())
		{
			tmpreg = item->AllRegion();
			dx = item->pos().x();
			dy = item->pos().y();
			reg += tmpreg.translated( dx, dy );
			QRectF pRect=QRectF(item->realRect().x()+1+dx, item->realRect().y()+1+dy, item->realRect().width()-2, item->realRect().height()-2);		 
			//painter->drawRoundedRect( pRect, 16, 16 );
		}
	}
	
	for ( int i = 0; i < NumOfMembers(); i ++ )
	{
		item = this->getMemberAt(i);
		if(item->isVisible())
		{
            tmpreg = item->roundRect(item->realRect(), 20);
			dx = item->pos().x();
			dy = item->pos().y();
			reg -= tmpreg.translated( dx, dy );
		}		
	}	
 	QPainterPath path;
 	path.addRegion( reg ); 	
 	painter->fillPath( path, QBrush( QColor( item->getColor().a, item->getColor().b, item->getColor().c, flag?255:item->getAlpha() ) ) );

}

/*QtBat* Group::getBatLinkWithPC( QtSM* item )
{
	ItemBase* node;
	QtBat* bat;

	int pos;

	for (int i = 0; i < NumOfMembers(); i ++)
	{
		node = this->getMemberAt( i );

		if ( !node )
		{
			continue;
		}

		if( node->getType() == BAT )
		{
			bat = dynamic_cast<QtBat*>( node );
			int pos = item->getSM()->getName().find( bat->getBat()->getname() );
			if (pos >= 0)
			{
				return bat;
			}
		}
	}

	return NULL;
}

QtBat* Group::getBatLinkWithVideo( VideoBlock* item )
{
	ItemBase* node;
	QtBat* bat;

	int pos;

	for (int i = 0; i < NumOfMembers(); i ++)
	{
		node = this->getMemberAt( i );

		if ( !node )
		{
			continue;
		}

		if( node->getType() == BAT )
		{
			bat = dynamic_cast<QtBat*>( node );
			int pos = item->getPlayerWidget()->getName().toStdString().find( bat->getBat()->getname() );
			if (pos >= 0)
			{
				return bat;
			}
		}
	}

	return NULL;
}*/