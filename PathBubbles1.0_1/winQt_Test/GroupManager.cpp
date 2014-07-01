#include "GroupManager.h"
#include "expressionBubble.h"
#include "treeRingBubble.h"
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

void GroupManager::bubbleDeleted( ItemBase* init, QRectF parentRect)
{
    if ( init == NULL )	return;

	if(init->getType() == PATHBUBBLE1 || init->getType() == SUBPATHBUBBLE1)
	{
		//_scene->clearSearchSelection(init);
		Group *B=getGroup(init);
		B->updateSharedSet(init);
		
		PathBubble1 *pBubble=dynamic_cast<PathBubble1 *> (init);
		_scene->clearSearchSelection(pBubble);
		//pBubble->updateSharedLinkedDIffered();
	}
	//	unhighlight
	//B
	if( this->_selected.contains( init ) )
	{
	
	}
	else
	{
		//if(!init->isMoving())
		{
			unHightLightAll();
		    this->_selected.clear();
		}
	}

	//int type = init->getType();
	//if(type > -1 && ( type != COLOR || type != COMPLOT ))
	int type=init->getType();
	QList<ItemBase *> AList;
	AList.append(init);	
	if(type==TEXTBUBBLE2)
	{
		TextBubble2 *tBubble=dynamic_cast<TextBubble2 *> (init);
		tBubble->getAllSubTextBubble(tBubble, AList);
	}
	else if(type==PATHBUBBLE1 || type==SUBPATHBUBBLE1)
	{
		PathBubble1 *pBubble=dynamic_cast<PathBubble1 *> (init);
		//pBubble->getAllSubPathBubble(pBubble, AList);
		//pBubble->getAllSubTextBubble(pBubble, AList);
	}

	tightenGroupMembers(BUBBLEDELETED, AList[0], QRectF(-100000,-100000,-100000,-100000), -1, true, false, parentRect);
	
	this->_ONMOVING = false;
	return;
}

void GroupManager::itemPosUpdated( ItemBase* init )
{
	if ( init == NULL )	return;

	if(init->getType()==CALLOUT)
		return;

	//			unhighlight
	if( this->_selected.contains( init ) )
	{
	}
	else
	{
		//if(!init->isMoving())
		{
			unHightLightAll();
		    this->_selected.clear();
		}
	}
	int type = init->getType();

	//if(!init->isUnGroup())
	if(type > -1 && ( type != COLOR || type != COMPLOT ))
	{
		bool flag=true;
		while(flag)
		{
		   flag=this->findOverlap( init );		
		}
	}
	this->_ONMOVING = false;
	return;
}

void GroupManager::itemPosUpdatedforSpring( ItemBase* init )
{
	if ( init == NULL )	return;

	if(init->getType()==CALLOUT)
		return;

	//			unhighlight
	if( this->_selected.contains( init ) )
	{
	}
	else
	{
		//if(!init->isMoving())
		{
			unHightLightAll();
		    this->_selected.clear();
		}
	}
	int type = init->getType();

	//if(!init->isUnGroup())
	if(type > -1 && ( type != COLOR || type != COMPLOT ))
	{
		int flag=1;
		while(flag==1)
		{
		   flag=this->findOverlap( init );		
		}
	}
	this->_ONMOVING = false;
	return;
}


void GroupManager::bubbleResized( ItemBase* init, QRectF oriRect, int cornerIndex)
{
	if ( init == NULL )	return;

	if(init->getType()==CALLOUT)
		return;

	//	unhighlight
	if( this->_selected.contains( init ) )
	{
	}
	else
	{
		//if(!init->isMoving())
		{
			unHightLightAll();
		    this->_selected.clear();
		}
	}
	int type = init->getType();
	if(type > -1 && ( type != COLOR && type != COMPLOT && type != CALLOUT ))
	{
		bool bubblechanged1=false, bubblechanged2=false;
		//bubblechanged1 = shiftNeighborforResizing(BUBBLESHIFTED, init, oriRect, cornerIndex, false, true);
		//bubblechanged2 = this->adjustOverlaid(BUBBLERESIZED, init, oriRect, cornerIndex, false, true); //move others around init 		
		//tightenGroupMembers(BUBBLERESIZED, init, oriRect, cornerIndex, false, bubblechanged1||bubblechanged2);		
		shiftNeighborforResizing(BUBBLERESIZED, init, oriRect, cornerIndex, false, true);		
	}
	init->resizeFinished=true;
	this->_ONMOVING = false;
	return;
}


void GroupManager::newBubbleCreated( ItemBase* init )
{
	if ( init == NULL )	return;

	//			unhighlight
	if( this->_selected.contains( init ) )
	{
	}
	else
	{
		//if(!init->isMoving())
		{
			unHightLightAll();
		    this->_selected.clear();
		}
	}

	int type = init->getType();
	if(type > -1 && ( type != COLOR || type != COMPLOT ))
	{
		if(this->moveOverlaped( init )) //this may not be used since new bubble will alway placed not to overlap with othters
		{
		    
		}
	}
	this->_ONMOVING = false;
	return;
}

void GroupManager::itemPosMoved(ItemBase * init, QPointF offset)
{
	findAndHighlight( init );
	this->_ONMOVING = true;
	int gIndex = this->findGroupIndex(init);
	if (gIndex > -1)
	{   
		this->_list[gIndex]->moveOtherMember(init, offset);
		//this->_list[gIndex]->moveAssociateNodeNotes(init, _scene->_nodeNote, offset);
	}
	return;
}

/*void GroupManager::addBatsToScene( QList<QList<ItemBase*>> bats )
{
	for( int i = 0; i < bats.size(); i++ )
	{
		for( int j = 0; j < bats[i].size(); j++ )
		{
			this->newItemAdded(bats[i][j]);
		}
	}
}*/


void GroupManager::encode(float x,float y,int &code,float XL,float XR,float YB,float YT)
{  int c=0;
   if(x<XL)c=c|1;
   else if(x>XR)c=c|2;
   if(y<YB)c=c|4;
   else if(y>YT)c=c|8;
   code=c;
}

bool GroupManager::RectRect(float cx1,float cy1,float cx2, float cy2, float w1, float h1, float w2, float h2)
{ //true: not intesect
	float x, y, XL1, XR1, YB1, YT1, XL2, XR2, YB2, YT2;
	int code1,code2,code3,code4;
	//float hh1=h1/2,hw1=w1/2,hh2=h2/2,hw2=w2/2;
	XL1=cx1, XR1=cx1+w1, YB1=cy1, YT1=cy1+h1;
	XL2=cx2, XR2=cx2+w2, YB2=cy2, YT2=cy2+h2;

	encode(XL2, YB2, code1, XL1, XR1, YB1, YT1);
	encode(XR2, YB2, code2, XL1, XR1, YB1, YT1);
	encode(XR2, YT2, code3, XL1, XR1, YB1, YT1);
	encode(XL2, YT2, code4, XL1, XR1, YB1, YT1);
	if(code1 && code2 && code3 && code4)
	{
		if(((code1|code4)==12)||((code2|code3)==12))
			return false;
		if(((code1|code2)==3)||((code4|code3)==3))
			return false;
		return true;
	}
	else return false;  
}

bool GroupManager::outBox(float x1, float y1, float width1, float height1, float x2, float y2, float width2, float height2, float margin)
{//cur tar are center point of the rects
	int half_cur = (width1 / 2.0 + 0.5) + margin; 
	int half_tar =  (width2 / 2.0 + 0.5) + margin;
	int half_cur2 = (height1 / 2.0 + 0.5) + margin; 
	int half_tar2 =  (height2 / 2.0 + 0.5) + margin;

	int pointX = 0;
	int pointY = 0;

	if(x1-half_cur >= x2+half_tar)
	{
		pointX += 1;
	}
	if(x1+half_cur <= x2-half_tar)
	{
		pointX += 1;
	}
	//			Y boundary
	if(y1-half_cur2 >= y2+half_tar2)
	{
		pointY += 1;
	}
	if(y1+half_cur2 <= y2-half_tar2)
	{
		pointY += 1;
	}

	// determine
	if( pointX == 0 && pointY == 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}



float GroupManager::OverlapArea(float cx1, float cy1, float cx2, float cy2, float w1, float h1, float w2, float h2)
{   //true: not intesect
	if(outBox(cx1, cy1, w1, h1, cx2, cy2, w2, h2, 0))
	   return 0;

	list<float> x,y;
	float x1,x2, y1, y2;
	
	x.push_back(cx1);    x.push_back(cx2); 	 x.push_back(cx1+w1);    x.push_back(cx2+w2);
	y.push_back(cy1);    y.push_back(cy2); 	 y.push_back(cy1+h1);    y.push_back(cy2+h2);

	x.sort();   
	y.sort();
	list<float>::iterator xit = x.begin(), yit = y.begin();

	xit++;	x1=*xit;	xit++;	x2=*xit;
	yit++;	y1=*yit;	yit++;	y2=*yit;
	
	return (x2-x1)*(y2-y1);
	
}

bool GroupManager::Enclosed(ItemBase *left, ItemBase *right)
{
	
    float area=OverlapArea(left->pos().x()-left->Width()/2, left->pos().y()-left->Height()/2, right->pos().x()-right->Width()/2, right->pos().y()-right->Height()/2, left->Width(), left->Height(), right->Width(), right->Height());
	float a1=left->Height()*left->Width(), a2=right->Width()*right->Height();
	float smallarea=a1<a2?a1:a2;
	if(area/smallarea>0.8)
		return true;
	else return false;
}


bool GroupManager::Enclosed(ItemBase *left, QList<ItemBase *> right)
{
	int count=0;


	if (left->getType() != TREERING && left->getType() != PATHBUBBLE1 && left->getType() != SUBPATHBUBBLE1 )
		return false;
	
	for(int i=0; i<right.size(); i++)
	{
		if(left->getType() == TREERING)
		{
			if( right[i]->getType() != TREERING)
			{
				return false;
			}
		}
		else
		{
			if( right[i]->getType() != PATHBUBBLE1 && right[i]->getType() != SUBPATHBUBBLE1)
			{
				return false;
			}
		}
	}
    
	float area=0, a2=0, a1=left->Height()*left->Width();
	for(int i=0; i<right.size(); i++)
    {
		area = area + OverlapArea(left->pos().x()-left->Width()/2, left->pos().y()-left->Height()/2, right[i]->pos().x()-right[i]->Width()/2, right[i]->pos().y()-right[i]->Height()/2, left->Width(), left->Height(), right[i]->Width(), right[i]->Height());
		a2 = a2 + right[i]->Width()*right[i]->Height();		
	}
	float smallarea=a1<a2?a1:a2;		
	if(area/smallarea>0.75)
		return true;
	return false;
}


int GroupManager::findOverlap( ItemBase* init )
{
	bool overlaidSelfGroup=false;
	//			all items
	QList<ItemBase*> members = allMembers();
	if(members.isEmpty()) { return false;} //J.C.

	if(init->getType()==CALLOUT)
		return 0;
	//			current group members
	QList<ItemBase*> items = getCurrentItems( init );

	// overlap members
	QList<ItemBase*> needToAdd;
	set<ItemBase*> needToAddSet;
	//set<ItemBase*> overLaidwithSameGroup;

	bool flag=false; //pflag=false; //pflag: are they bubbles
	vector<expressionBubble*> eList;
	vector<treeRingBubble*> tList;
	//	find overlapped member

	//if( members[i] == items[j] || sameGroup( members[i], items[j] ) )
	set<ItemBase*> enClosed;
	int size1=members.size();
	int size2=items.size();
	for( int i = 0; i < members.size(); i++ )
	{
		for( int j = 0; j < items.size(); j++ )
		{
			if(enClosed.find(members[i])!=enClosed.end())
				continue;

			if(enClosed.find(items[j])!=enClosed.end()) 
				continue;
			

			/*if (( members[i]->getType() == PATHBUBBLE1 || members[i]->getType() == SUBPATHBUBBLE1 || members[i]->getType() == EXPRESSIONBUBBLE) && ( items[j]->getType() == PATHBUBBLE1 || items[j]->getType() == SUBPATHBUBBLE1 || items[j]->getType() == EXPRESSIONBUBBLE ) )
			{ 
				pflag=true;
			}
			else pflag=false;*/

			if(members[i] == items[j])
			{
				continue;
			}
			if(sameGroup( members[i], items[j] ))
			{
				if(Overlaid( members[i], items[j]))
				{
					if(members[i]->pos()==members[i]->getTargetPos() && items[j]->pos()==items[j]->getTargetPos())
				       overlaidSelfGroup=false; //debug
					else 
					   overlaidSelfGroup=true;
				}
				continue;
			}
  			/*if( getCurrentItems( members[i] ).size() == 1 && needCompare(  members[i], items[j] ) )
  			{
  				//setCompareWithOutPlot( members[i], items[j] );
  			}*/
			if( members[i]->isVisible() && items[j]->isVisible() && Overlaid( members[i], items[j] ))
			{
				if( (items.size()==1 && Enclosed(members[i], items[j])) || Enclosed(members[i], items) )
				{	
					//b0->AFTERUNGROUP=true;
					//tmp->AFTERUNGROUP=true;
                    int bIndex1=-1, bIndex2=-1, p1, p2;
					if ( members[i]->getType() == TREERING && items[j]->getType() == TREERING )
			        {
				          treeRingBubble* tmp1 = dynamic_cast<treeRingBubble*>(members[i]);					
						  treeRingBubble* tmp2 = dynamic_cast<treeRingBubble*>(items[j]);
						  enClosed.insert(tmp2);								
						  emit findEnclosedTreeRing(tmp1,tmp2);	
						  //tmp1->selfDelection(tmp1,tmp2);
						  //tmp2->deleteSelectedItems(_scene);
						  flag=true;

				    }
					else
					{
						if ( members[i]->getType() == PATHBUBBLE1 || members[i]->getType() == SUBPATHBUBBLE1 )
						{
							  PathBubble1* tmp = dynamic_cast<PathBubble1*>(members[i]);					
							  bIndex1 = tmp->bIndex; 
							  p1 = tmp->m_pathwayID;						  
						}
						if ( items[j]->getType() == PATHBUBBLE1 || items[j]->getType() == SUBPATHBUBBLE1 )
						{
							  PathBubble1* tmp = dynamic_cast<PathBubble1*>(items[j]);					
							  bIndex2 = tmp->bIndex; 
							  p2 = tmp->m_pathwayID;						  
						}
						if(bIndex1!=bIndex2&&bIndex2!=-1&&bIndex1!=-1)
						{
							flag = true;		
							PathBubble1* tmp = dynamic_cast<PathBubble1*>(items[j]);						   
							PathBubble1* b0 = dynamic_cast<PathBubble1*>(members[i]);	

						   	if(!b0->samePathwayGroup(b0, tmp))
							{
								flag=false;
							}
							int level1=b0->getPathBubbleLevel(tmp);
							int level2=b0->getPathBubbleLevel(b0);
							if(level2 >= level1)
								flag=false;

							if(flag)
							{
									enClosed.insert(tmp);								
									emit findEnclosed(b0,tmp);						
							}	
						}	
					}
				}				
				if(!flag)
				{
					if( !isInThisList( needToAdd, members[i]) )
					{
						if(needToAddSet.find(members[i])==needToAddSet.end() && members[i]->getType()!=CALLOUT)
						{
							needToAdd.append( members[i] );
					  	    needToAddSet.insert(members[i]);
						}
					}
					if ( members[i]->getType() == PATHBUBBLE1 || members[i]->getType() == SUBPATHBUBBLE1 )
					{   
						PathBubble1* tmp;					
						tmp = dynamic_cast<PathBubble1*>(members[i]);		
 						for (int k = 0; k < _scene->_pathBubbles.size(); k++)
						{
							//if(_scene->_pathBubbles[k]->_name.compare(items[j]->_name)==0&&items[j]->_name.size()!=0)		
							if(_scene->_pathBubbles[k])
							if(_scene->_pathBubbles[k]!=NULL)
							if(_scene->_pathBubbles[k]->m_pathwayID>=0 && _scene->_pathBubbles[k]->m_pathwayID < _scene->_pathBubbles.size())
							if(_scene->_pathBubbles[k]->m_pathwayID == tmp->m_pathwayID)// members[k]->// compare(members[k]->_name)==0)
							{
								if(items[j]->getType()==EXPRESSIONBUBBLE)
								{
									expressionBubble* ite = dynamic_cast<expressionBubble*> (items[j]);
									for (int m = 0; m < _scene->_expressionBubbles.size(); m++)
									{		
										if(_scene->_expressionBubbles[m])
										if(_scene->_expressionBubbles[m]->m_expressionID == ite->m_expressionID)
										{								
											_scene->_expressionBubbles[m]->m_bindPathwayID = k;	
											//_scene->_expressionBubbles[m]->TransferExpression(k);
											//_scene->_expressionBubbles[m]->getExpressionToBePaint();
											eList.push_back(_scene->_expressionBubbles[m]);																			
										}
									}
								}
							}
						}
					}
					else if ( members[i]->getType() == EXPRESSIONBUBBLE )
					{   
						expressionBubble* tmp = dynamic_cast<expressionBubble*>(members[i]);		
 						for (int k = 0; k < _scene->_expressionBubbles.size(); k++)
						{
							if(_scene->_expressionBubbles[k])
							if(_scene->_expressionBubbles[k]->m_expressionID == tmp->m_expressionID)
							{
								if(items[j]->getType()==PATHBUBBLE1 || items[j]->getType()==SUBPATHBUBBLE1)
								{
									PathBubble1* ite = dynamic_cast<PathBubble1*> (items[j]);
									tmp->m_bindPathwayID = ite->m_pathwayID;	
									for (int m = 0; m < _scene->_pathBubbles.size(); m++)
									{				
										if(_scene->_pathBubbles[m])
										if(_scene->_pathBubbles[m]!=NULL)
										if(_scene->_pathBubbles[m]->m_pathwayID>=0 && _scene->_pathBubbles[m]->m_pathwayID < _scene->_pathBubbles.size())
										if(_scene->_pathBubbles[m]->m_pathwayID == ite->m_pathwayID)
										{								
											//_scene->_expressionBubbles[k]->TransferExpression(ite->m_pathwayID);
											//_scene->_expressionBubbles[k]->getExpressionToBePaint();
											eList.push_back(_scene->_expressionBubbles[k]);																			
										}
									}
								}
							}
						}
					}
					else if ( members[i]->getType() == TREERING )
					{
						treeRingBubble* tmp = dynamic_cast<treeRingBubble*>(members[i]);  //assume only one tree ring bubble		
						tList.push_back(tmp);		 						
					}
				}
			}			
		}
	}
	
	int size=needToAdd.size();
	this->addToGroup( needToAdd, init);
	for(int i=0; i < eList.size(); i++) 
	{
		eList[i]->searchSharedProtein();
	}
	
	for(int i=0; i < tList.size(); i++) 
	{
		tList[i]->searchSharedProtein();
	}

	if(size>0)
	   return 1;
	else 
	{
		if(overlaidSelfGroup)
			return 2;
		else return 0;
	}
}


bool GroupManager::tightenGroupMembers(int eventType, ItemBase* init, QRectF oriRect, int cornerIndex, bool deleted, bool setPos, QRectF parentRect, bool groupSpaceCheck)
{ //if broken in same group
//if call this function before init is deleted, remove it
	//setPos: set it otherwise, set target to allow bubble more or overrid by later setpos operation
  //tighten
	Group *B=getGroup(init);
	QList<ItemBase*> list = B->getMembers(); 
	int size,initID;
	bool flag=false;
	
    //get rid of init if deleted
	if(deleted)
	   list.removeOne(init);

	if(list.empty())
	{
		return false;
	}

	size=list.size();
	initID=-1;
	vector<QRectF> rectList = B->assignRectList(list);	

	/*for(int i=0; i<list.size(); i++) //debug
		{
			float x,y;				
			x=rectList[i].center().x();
			y=rectList[i].center().y();	

			float disx=x-rectList[0].center().x();
			float width=rectList[i].width();
			int Idisx=x-rectList[0].center().x();
			int Iwidth=rectList[i].width();
			x=x;
			
		}*/

	//vector<QRectF> rectList = B->assignSceneRectList(list);
	ItemBase *parent=NULL;
	if(init->getType()==SUBPATHBUBBLE1 || init->getType()==PATHBUBBLE1)
	{
		if(init->m_pathwayID>=0)
		{
			PathBubble1* pbubble = dynamic_cast<PathBubble1*>(init);				     
			if(pbubble->_pathBubbleParent)
			{
				parent=dynamic_cast<ItemBase* >(pbubble->_pathBubbleParent);			
				
			}
		}			
	}
	
	for(int i=0; i<size; i++)
	{
		if(!deleted)
		if(list[i]==init)
		{
			initID=i;						
		}
		if(deleted && parentRect.width()>-100)
		if(parent==list[i])
		{
		   initID=i;	
		   rectList[i]=parentRect;		
		}
	}
     if(initID<0) //?
		initID=0;

	vector<QRectF> aList=rectList;

	if(B->isbridgeBroken(rectList))
	{	
		vector<set<int> > segList = B->getGroupSegment(rectList); 
		set<int> initSet;
		vector<set<int> > familySets;			
		if(!deleted || segList.size()!=1)
		{	
			int size=segList.size();
			for(int i=0; i<size; i++)
			{
				familySets.push_back(segList[i]);	
				if(segList[i].find(initID)!=segList[i].end())
				{    
					initSet.insert(i);
				}
			}
			//for(int i=0; i<size; i++)
			{
				//move toward each other to fill the gap	
				vector<int> parentList = getParentList(list, familySets);					
				rectList = B->lineUpDisplay(eventType, list, parentList, initSet, list.size(), familySets, rectList, list[0]->screenWidth, list[0]->screenHeight, oriRect, cornerIndex);//list should include initSet
			}
			flag=true;
		}
		//break;
	}

	//move 
	if(flag)
	{	
		//use init as a center
		
		initID=-1;
		QPointF aCenterPos=gravityCenter(aList,initID);
		QPointF CenterPos=gravityCenter(rectList,initID);
		QPointF Dis=aCenterPos-CenterPos;
		for(int i=0; i<list.size(); i++)
		{
			float x=rectList[i].center().x();
			float y=rectList[i].center().y();
			rectList[i].moveCenter(QPointF(x+int(Dis.x()),y+int(Dis.y())));	
		}

		vector<QPointF> stepSize;
		if(!setPos)
		{
			stepSize = getSynchronizedSpeed(list, aList, rectList);
		}
		for(int i=0; i<list.size(); i++)
		{
			float x,y;				
			x=rectList[i].center().x();
			y=rectList[i].center().y();	

			/*float disx=x-rectList[0].center().x();
			float width=rectList[i].width();
			int Idisx=x-rectList[0].center().x();
			int Iwidth=rectList[i].width();
			if(Idisx % Iwidth!=0)
				Idisx=Idisx;*/

			if(list[i]->getType()!=CALLOUT)
			{
				if(setPos)
						list[i]->mySetPos( QPointF(x, y));		
				else 
						list[i]->setTargetPos( QPointF(x, y), stepSize[i], true);		
			}
		}
	}
	return flag;
}

QPointF GroupManager::gravityCenter(vector<QRectF> rectList, int init)
{
	float sumx=0,sumy=0, area, suma=0;	
	for(int i=0; i<rectList.size(); i++)
	{
	    if(init!=i)
		{
			area=rectList[i].width()*rectList[i].height();
			sumx = sumx + area * rectList[i].center().x();
			sumy = sumy + area * rectList[i].center().y();
			suma = suma + area;
		}	
	}
	return QPointF(sumx/suma, sumy/suma);
}
//topelevel -> leftmost
vector<QPointF> GroupManager::getSynchronizedSpeed(QList<ItemBase*> list, vector<QRectF> preRect, vector<QRectF> newRect)
{			
	vector<QPointF> stepSize,Dis;
	float max=-100000, distance;
	QPointF dis;
	vector<QPointF> speed;
	for(int i=0; i<preRect.size(); i++)
	{
		if(newRect[i].x()>-10000 && newRect[i].x()<10000 && preRect[i]!=newRect[i])
		{
			//QPointF center(AllnewRect[i].x()+AllnewRect[i].width()/2)			
			float x = newRect[i].x() == preRect[i].x()? list[i]->pos().x() : float(newRect[i].x())+float(newRect[i].width())/2.0;
			float y = newRect[i].y() == preRect[i].y()? list[i]->pos().y() : float(newRect[i].y())+float(newRect[i].height())/2.0;
			dis= QPointF(x,y)-list[i]->pos();
			distance= sqrt(dis.x()*dis.x() + dis.y()*dis.y());
			Dis.push_back(dis);
			if( distance<max)			
				max = distance;						
		}
		else
			Dis.push_back(QPointF(0,0));
	}	
	int step;
	if(distance > 1000)
		step=50;
	else if (distance<400)
		step = 20;
	else step=distance/20.00;

	for(int i=0; i<preRect.size(); i++)
	{	
		stepSize.push_back(QPointF(Dis[i].x()/step, Dis[i].y()/step));	
	}	
	return stepSize;
}

QPointF GroupManager::getSynchronizedSpeed(ItemBase* list, QRectF preRect, QRectF newRect)
{			
	QPointF stepSize,Dis;
	float distance=0;
	QPointF dis;
	vector<QPointF> speed;
	_ONMOVING=false;
	//for(int i=0; i<preRect.size(); i++)
	{
		if(newRect.x()>-10000 && newRect.x()<10000 && preRect!=newRect)
		{
			//QPointF center(AllnewRect[i].x()+AllnewRect[i].width()/2)			
			float x = newRect.x() == preRect.x()? list->pos().x() : float(newRect.x())+float(newRect.width())/2.0;
			float y = newRect.y() == preRect.y()? list->pos().y() : float(newRect.y())+float(newRect.height())/2.0;
			dis= QPointF(x,y)-list->pos();
			distance= sqrt(dis.x()*dis.x() + dis.y()*dis.y());
			Dis=dis;
								
		}
		else
			Dis=QPointF(0,0);
	}	
	int step;
	if(distance > 1000)
		step=50;
	else if (distance<400)
		step = 20;
	else step=distance/20.00;

	//for(int i=0; i<preRect.size(); i++)
	{	
		stepSize=QPointF(distance/step, distance/step);	
	}	
	return stepSize;
}

bool GroupManager::lineUpDisplayGroups(QList<ItemBase *> items, vector<QRectF>  rectList, bool setPos)
{  //check if the new display interfere with other existing group
	//all items
	//return false;

	if(items.size()!=rectList.size())
		return false;
	vector<QRectF>  AllRectList;
	QList<ItemBase *> AllList;

	QList<Group*> groups = allGroups();

	if(groups.isEmpty()) 
		return false;
	
	int size=groups.size();	
	int size2=items.size();
	
	set<int> toBeMoved;	
	bool flag=false;
	for( int k = 0; k < groups.size(); k++ )
	{
		QList<ItemBase*> members=groups[k]->getMembers();
		bool overlaid=false;
		for( int i = 0; i < members.size(); i++ )
		{
			
			for( int j = 0; j < items.size(); j++ )
			{
				if(members[i] == items[j])
				{
					continue;
				}
				if(!members[i])
					continue;
				if(!sameGroup( members[i], items[j]) ) 
				{
						
					toBeMoved.insert(k);					   
					if( groups[k]->overlaid( members[i]->sceneBoundingRect(), rectList[j]) )				
					{
					   overlaid=true;
					   flag=true;
					   break;
					}
					//toBeMoved.insert(k);
				}
			}
			if(overlaid)
			   break;
		}
	}
	if(!flag)
		return false;
	//move the group in toBeMoved
	//get familySet
	vector<set<int> > familySets;
	set<int> aSet;
	for( int i = 0; i < items.size(); i++ )
	{
		aSet.insert(i);
		//make it bigger so not overlap
		//QPointF center1=rectList[i].center();
		QRectF rect=QRect( rectList[i].x()-20, rectList[i].y()-20, rectList[i].width()+40, rectList[i].height()+40);						
		//QPointF center2=rectList[i].center();
		AllRectList.push_back(rect);
	}

	familySets.push_back(aSet);
	
	AllList = items;
	//AllRectList=rectList;
	for(set<int>::iterator it = toBeMoved.begin(); it != toBeMoved.end(); it++)
	{
		QList<ItemBase*> members=groups[*it]->getMembers();
		set<int> tSet;
		for( int i = 0; i < members.size(); i++ )
		{
		    if(members[i] && members[i]->getType()!=CALLOUT)
			{
				bool flag=true;
				for(int j=0; j<items.size(); j++)
				{
					if(members[i] == items[j])
					{
						flag = false;
						break;
					}
				}
				if(flag)
				{
					AllList.append(members[i]);
							
					AllRectList.push_back(members[i]->sceneBoundingRect());
					tSet.insert(AllList.size()-1);
				}
			}
		}
		if(!tSet.empty())
			familySets.push_back(tSet);
	}
	set<int> initF;
	initF.insert(0);

	vector<QRectF> aList=AllRectList;
	vector<int> parentList = getParentList(AllList, familySets);
	AllRectList=groups[0]->lineUpDisplay(GROUPOVERLAID, AllList, parentList, initF, AllList.size(), familySets, AllRectList, AllList[0]->screenWidth, AllList[0]->screenHeight, QRectF(-100000,-100000,-100000,-100000), -1, 0, 0, true);

	for(int i=items.size(); i<AllList.size(); i++)
	{
		//this->getMemberAt(i)->setTargetPos( QPointF(rectList[i].x()-getMemberAt(i)->realRect().x(),rectList[i].y()-getMemberAt(i)->realRect().y()) );				
		if(AllRectList[i].x()>-10000 && AllRectList[i].x()<10000 && aList[i]!=AllRectList[i])
		{
			//QPointF center(AllRectList[i].x()+AllRectList[i].width()/2)			
			float x = AllRectList[i].x() == aList[i].x()? AllList[i]->pos().x() : float(AllRectList[i].x())+float(AllRectList[i].width())/2.0;
			float y = AllRectList[i].y() == aList[i].y()? AllList[i]->pos().y() : float(AllRectList[i].y())+float(AllRectList[i].height())/2.0;
			
			//if(AllList[i]->getType()!=CALLOUT)
			{
			   if(setPos)
				  AllList[i]->mySetPos( QPointF(x, y));		
			   else 
				  AllList[i]->setTargetPos( QPointF(x, y));			
			}
		}
	}
	aList.clear();
    AllList.clear();
	AllRectList.clear();
	return true;
}

QPointF GroupManager::getPosition(ItemBase *newitem, int x, int y, int width, int height)
{
	int Width = this->_scene->width();
	int Height = this->_scene->height();
	// get all items in view
	QList<ItemBase*> list; 
	QList<QGraphicsItem*> allItem = this->_scene->items();
	int size=allItem.size();
	for( int i = 0; i < allItem.size(); i++ )
	{
		ItemBase *itemBase = dynamic_cast<ItemBase*>( allItem[i] );
		if( itemBase && itemBase->isVisible())
		{
			//if( itemBase->pos().x() > 0 && itemBase->pos().x() < Width )
			int type=itemBase->getType();
			if(type > -1 && ( type != COLOR && type != COMPLOT && type != CALLOUT ))
			{
				//if( itemBase->pos().y() > 0 && itemBase->pos().y() < Height )
				{
					list.append( itemBase );
				}
			}
		}
	}

	if(x< 0)	
	{
	    x = 0;
	}
	if( y + height + 60 > newitem->screenHeight)
	{
	    y = newitem->screenHeight - height - 60;
	}
	
	for( int i = 0; i < list.size(); i++ )
	{
		QRectF bounding = list[i]->mapRectToScene( list[i]->boundingRect() );
		if(list[i]!=newitem)
		if( bounding.intersects(QRectF(x, y, width, height)) )
		{
			class Group *group= new Group();
			set<int> init; 
			int newID;
			for( int i = 0; i < list.size(); i++ )
			{
				if(list[i]!=newitem)
				  init.insert(i);
				else newID=i;
			}
			
			vector<set<int> > familySets;
			for(int i=0; i<list.size(); i++)
			{
				set<int> tset;
				tset.insert(i);
				familySets.push_back(tset);	
			}
			//for(int i=0; i<size; i++)
		
			vector<QRectF> rectList = group->assignRectList(list,true);	
			rectList[newID]=QRectF(x, y, width, height);
			vector<int> parentList = getParentList(list, familySets);
			rectList=group->lineUpDisplay(BUBBLECREATED, list, parentList, init, list.size(), familySets, rectList, list[0]->screenWidth, list[0]->screenHeight, QRectF(-100000,-100000,-100000,-100000), -1, 0, 0, true);	 
			return(QPointF(rectList[newID].x(), rectList[newID].y()));
		}
	}
	return QPointF( x, y );
}


QPointF GroupManager::getPosition(ItemBase *parent, ItemBase *newitem, int x, int y, int width, int height)
{
	int Width = this->_scene->width();
	int Height = this->_scene->height();
	// get all items in view
	QList<ItemBase*> list; 
	QList<QGraphicsItem*> allItem = this->_scene->items();
	int size=allItem.size();
	for( int i = 0; i < allItem.size(); i++ )
	{
		ItemBase *itemBase = dynamic_cast<ItemBase*>( allItem[i] );
		if( itemBase && itemBase->isVisible())
		{
			//if( itemBase->pos().x() > 0 && itemBase->pos().x() < Width )
			int type=itemBase->getType();
			if(type > -1 && ( type != COLOR && type != COMPLOT && type != CALLOUT ))
			{
				list.append( itemBase );				
			}
		}
	}
	
	for( int i = 0; i < list.size(); i++ )
	{
		QRectF bounding = list[i]->mapRectToScene( list[i]->boundingRect() );
		if(list[i]!=newitem)
		if( bounding.intersects(QRectF(x, y, width, height)) )
		{
			class Group *group= new Group();
			set<int> init; 
			int newID;
			for( int i = 0; i < list.size(); i++ )
			{
				if(list[i]!=newitem)
				  init.insert(i);
				else newID=i;
			}			
			vector<set<int> > familySets;
			for(int i=0; i<list.size(); i++)
			{
				set<int> tset;
				tset.insert(i);
				familySets.push_back(tset);	
			}					
			vector<QRectF> rectList = group->assignRectList(list,false);	
			//rectList[newID]=QRectF(x, y, width, height);
			vector<int> parentList = getParentList(list, familySets,parent);
			rectList=group->lineUpDisplay(BUBBLECREATED, list, parentList, init, list.size(), familySets, rectList, list[0]->screenWidth, list[0]->screenHeight, QRectF(-100000,-100000,-100000,-100000), -1, 0, 0, false);	 
			return(QPointF(rectList[newID].x(), rectList[newID].y()));
		}
	}
	return QPointF( x, y );
}

bool GroupManager::shiftNeighborforResizing(int eventType, ItemBase* init, QRectF oriRect, int cornerIndex, bool notLayoutGroup, bool setPos)
{
	Group *B;
	QList<ItemBase*> list; 
	vector<QRectF> rectList;	
	bool shifted=false;
	//vector<QRectF> rectList = B->assignSceneRectList(list);
	
	
		B=getGroup(init);	  
		list = B->getMembers(); 
        rectList = B->assignRectList(list);	

		set<int> initSet; 
	
		vector<set<int> > familySets = GroupToFamily(list);//, init);
		int initID=B->getFamilyID(B->getMemberID(init), familySets);
		if(initID>=0)
		   initSet.insert(initID); 
		if(initSet.empty())
			return false;

		//if(!initSet.empty())
		{
			for(int i=0; i<familySets.size(); i++)
			{
			   if(familySets[i].size()>1)
			   if(familySets[i].find(initID)!=familySets[i].end())
			   {
				  familySets[i].erase(initID);
				  set<int> tSet; 
				  tSet.insert(initID);
				  familySets.push_back(tSet);
				  break;
			   }
			}
			vector<QRectF> aList=rectList;
		
		
			vector<int> parentList = getParentList(list, familySets);
			rectList = 	B->lineUpDisplay(eventType, list, parentList, initSet, list.size(), familySets, rectList, list[0]->screenWidth, list[0]->screenHeight, oriRect, cornerIndex);//list should include initSet
			B->adjustOverlaid(initID, rectList, parentList, familySets, list[0]->screenWidth, list[0]->screenHeight, oriRect, cornerIndex, 0, 0);//not shift neighborsrectList = 	B->lineUpDisplay(eventType, list, parentList, initSet, list.size(), familySets, rectList, list[0]->screenWidth, list[0]->screenHeight, oriRect, cornerIndex);//list should include initSet		
			//need to get this out and set target after all the bubble location are settled
			vector<QPointF> stepSize;
			if(!setPos)
			{
				stepSize = getSynchronizedSpeed(list, aList, rectList);
			}
			for(int i=0; i<list.size(); i++)
			{
				//this->getMemberAt(i)->setTargetPos( QPointF(rectList[i].x()-getMemberAt(i)->realRect().x(),rectList[i].y()-getMemberAt(i)->realRect().y()) );		
				if(rectList[i].x()>-10000 && rectList[i].x()<10000 && aList[i]!=rectList[i])
				{
					//QPointF center(AllRectList[i].x()+AllRectList[i].width()/2)			
					float x = rectList[i].x() == aList[i].x()? list[i]->pos().x() : float(rectList[i].x())+float(rectList[i].width())/2.0;
					float y = rectList[i].y() == aList[i].y()? list[i]->pos().y() : float(rectList[i].y())+float(rectList[i].height())/2.0;

					if(list[i]->getType()!=CALLOUT)
					{
						if(setPos)
							list[i]->mySetPos( QPointF(x, y));		
						else				
							list[i]->setTargetPos( QPointF(x, y), stepSize[i], true);	
						shifted=true;
					}				
				
				}	
			}
			if(!notLayoutGroup)
				lineUpDisplayGroups(list, rectList, false);	
		}		
	    return shifted;	
}

bool GroupManager::adjustOverlaid(int eventType, ItemBase* init, QRectF oriRect, int cornerIndex, bool setPos, bool notLayoutGroup)
{   //solve overlap with init in same group  
	//do not add to new group
	//move bubble arround init
	//setPos: set it otherwise, set target to allow bubble more or overrid by later setpos operation

	QList<ItemBase*> members = allMembers();
	if(members.isEmpty()) { return false;} //J.C.
	//			current group members
	QList<ItemBase*> items = getCurrentItems( init );

	// overlap members
	QList<ItemBase*> needToAdd;
	set<ItemBase*> overLaidwithSameGroup;

	bool flag=false,pflag=false; //pflag: are they bubbles
	vector<expressionBubble*> eList;
	//	find overlapped member

	//if( members[i] == items[j] || sameGroup( members[i], items[j] ) )
	int size1=members.size();
	int size2=items.size();
	for( int i = 0; i < members.size(); i++ )
	{
		for( int j = 0; j < items.size(); j++ )
		{
			
			if(members[i] == items[j])
			{
				continue;
			}
			if(sameGroup( members[i], items[j] ))
			{
				if(Overlaid( members[i], items[j]))
				{
					overLaidwithSameGroup.insert(items[j]);
					overLaidwithSameGroup.insert(members[i]);
				}
			}
		}
	}
	
	//adjust the location of overlapped bubble in same group
	
	 Group *B;
	QList<ItemBase*> list; 
	vector<QRectF> rectList;			
	//vector<QRectF> rectList = B->assignSceneRectList(list);
	
	if(overLaidwithSameGroup.size()>0)
	//for(set<ItemBase*>::iterator it=overLaidwithSameGroup.begin(); it!=overLaidwithSameGroup.end(); it++)
	{
		B=getGroup(init);	  
		list = B->getMembers(); 
        rectList = B->assignRectList(list);	

		//list.removeOne(*it);
		set<int> initSet; 
		//initSet.insert(B->getMemberID( init)); 

		vector<set<int> > familySets = GroupToFamily(list);//, init);
	    /*for(int i=0; i<list.size(); i++)
	    {
			set<int> tset;
			tset.insert(i);
			familySets.push_back(tset);			
	    }*/
		int initID=B->getFamilyID(B->getMemberID(init), familySets);
		if(initID>=0)
		   initSet.insert(initID); 

		if(initSet.empty())
			return false;

		//int getFamilyID(int id, vector<set<int> > familySets);

		//if initSet is within a family, separate them into two family
		for(int i=0; i<familySets.size(); i++)
		{
		   if(familySets[i].size()>1)
		   if(familySets[i].find(initID)!=familySets[i].end())
		   {
              familySets[i].erase(initID);
			  set<int> tSet; 
			  tSet.insert(initID);
			  familySets.push_back(tSet);
		      break;
		   }
		}
		vector<QRectF> aList=rectList;
		
		vector<int> parentList = getParentList(list, familySets);
		rectList = 	B->lineUpDisplay(eventType, list, parentList, initSet, list.size(), familySets, rectList, list[0]->screenWidth, list[0]->screenHeight, oriRect, cornerIndex);//list should include initSet
				
		//need to get this out and set target after all the bubble location are settled
		vector<QPointF> stepSize;
		if(!setPos)
		{
			stepSize = getSynchronizedSpeed(list, aList, rectList);
		}
		for(int i=0; i<list.size(); i++)
		{
			//this->getMemberAt(i)->setTargetPos( QPointF(rectList[i].x()-getMemberAt(i)->realRect().x(),rectList[i].y()-getMemberAt(i)->realRect().y()) );		
            if(rectList[i].x()>-10000 && rectList[i].x()<10000 && aList[i]!=rectList[i])
			{
					//QPointF center(AllRectList[i].x()+AllRectList[i].width()/2)			
					float x = rectList[i].x() == aList[i].x()? list[i]->pos().x() : float(rectList[i].x())+float(rectList[i].width())/2.0;
					float y = rectList[i].y() == aList[i].y()? list[i]->pos().y() : float(rectList[i].y())+float(rectList[i].height())/2.0;

					if(list[i]->getType()!=CALLOUT)
					{
						if(setPos)
							list[i]->mySetPos( QPointF(x, y));		
						else list[i]->setTargetPos( QPointF(x, y), stepSize[i], true);		
					}
			}		    
			
		}
		if(!notLayoutGroup)
	        lineUpDisplayGroups(list, rectList, false);	
		//lineUpDisplay(list,	init, list.size(), list[0]->screenWidth, list[0]->screenHeight, true);	
	}
	
	//move other group around
			
	//moved

	/*int size=needToAdd.size();	
	this->addToGroup( needToAdd, init);
	for(int i=0; i < eList.size(); i++) 
	{
		eList[i]->searchExpressedProtein();
	}
	if(size>0)
	   return true;
	else return false;*/

	if(overLaidwithSameGroup.size()>0)
	   return true;
	else 
		return false;
}



bool GroupManager::moveOverlaped( ItemBase* init ) //but do not add to group
{   
	//			all items
	QList<ItemBase*> members = allMembers();
	if(members.isEmpty()) { return false;} //J.C.
	//			current group members
	QList<ItemBase*> items = getCurrentItems( init );

	// overlap members
	QList<ItemBase*> needToAdd;

	bool flag=false,pflag=false; //pflag: are they bubbles
	vector<expressionBubble*> eList;
	//	find overlapped member

	//if( members[i] == items[j] || sameGroup( members[i], items[j] ) )
	int size1=members.size();
	int size2=items.size();
	for( int i = 0; i < members.size(); i++ )
	{
		for( int j = 0; j < items.size(); j++ )
		{
			
			if ( ( members[i]->getType() == PATHBUBBLE1 || members[i]->getType() == SUBPATHBUBBLE1 || members[i]->getType() == EXPRESSIONBUBBLE || members[i]->getType() == TREERING) && ( items[j]->getType() == PATHBUBBLE1 || items[j]->getType() == SUBPATHBUBBLE1 || items[j]->getType() == EXPRESSIONBUBBLE || items[j]->getType() == TREERING ) )
			{ 
				pflag=true;
			}
			else pflag=false;

			if(members[i] == items[j] )
			{
				continue;
			}
			if( sameGroup( members[i], items[j]))
			{
			   if(Overlaid( members[i], items[j] ))
			   {
			       //adjustOverlaid(init);
				   //break;
			   }
			}

  			if( getCurrentItems( members[i] ).size() == 1 && needCompare(  members[i], items[j] ) )
  			{
  				//setCompareWithOutPlot( members[i], items[j] );
  			}
			if( members[i]->isVisible() && items[j]->isVisible() && Overlaid( members[i], items[j] ))
			{
	
				if(!flag)
				{
					if( !isInThisList( needToAdd, members[i]) )
					{
						needToAdd.append( members[i] );
					}
				}
			}			
		}
	}
	/*this->addToGroup( needToAdd, init);
	for(int i=0; i < eList.size(); i++) 
	{
		eList[i]->searchExpressedProtein();
	}*/
	return flag;
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

void GroupManager::addToGroup( ItemBase* add, ItemBase* init, bool updatePos)
{
	QList<ItemBase*> addlist;
	addlist.append( add );	
	if(addlist.size()==1)
	{
	//	addlist.append(this->_scene->getOpenManager()->openReactionBubble3( 50, addlist[0]->scenePos().x()+init->scenePos().x(),addlist[0]->scenePos().y(), QString("???")));
		//ReactionBubble3 *rb = dynamic_cast <ReactionBubble3*> (_open->openReactionBubble3(50, event->scenePos().x(),event->scenePos().y(), qstr));		
	}
	addToGroup( addlist, init, updatePos);
}

void GroupManager::addToGroup(QList<ItemBase*> members, ItemBase *init, bool updatePos, bool setPos)
{
	if (members.size() == 0)	
	{
		return;
	}
	vector<expressionBubble*> eList;
	int Type=init->getType();
	QList<ItemBase*> items = getCurrentItems( init );
	QList<ItemBase*> newMembers;
	QList<ItemBase*> nMembers;
	set<ItemBase*> newMembersSet;
	QString oName,sTemp;
	for( int i = 0; i < members.size(); i++ )
	{
		//b0->AFTERUNGROUP=true;
		//tmp->AFTERUNGROUP=true;
		if (  members[i]->getType() ==  PATHBUBBLE1 ||  members[i]->getType() ==  SUBPATHBUBBLE1)
		{
			PathBubble1* pBubble = dynamic_cast<PathBubble1*>( members[i] );
			sTemp = pBubble->getOrthFileName();
			if(sTemp!="")
				oName = sTemp;
			pBubble ->AFTERUNGROUP=false;
		}
		nMembers=getCurrentItems( members[i] );
		for(int j=0; j < nMembers.size(); j++)
	    {
			if(newMembersSet.find( nMembers[j])==newMembersSet.end())
		    {
			   newMembers.append( nMembers[j]);
		       newMembersSet.insert( nMembers[j]);
			}
		}
	}

	for(int i=0; i<items.size(); i++) 
		{
			if ( items[i]->getType() == SUBPATHBUBBLE1 || items[i]->getType() == PATHBUBBLE1)
			{
				PathBubble1* pBubble = dynamic_cast<PathBubble1*>(items[i]);				//test
				pBubble ->AFTERUNGROUP=false;
				//searchType=pBubble->searchType;			
				//seedPathBubble=pBubble;
				//break;
			}
		}
    //1. fix draw small_moelcule settings
	set<WebBlock *> sListNew, sListOld, ssList;	
	
	for( int j = 0; j < items.size(); j++ )
	{
		if ( items[j]->getType() == SETTINGS  )
		{
			WebBlock * ite = dynamic_cast<WebBlock *> (items[j]);										
		    sListOld.insert(ite);																																				
		}	
		else if ( items[j]->getType() == SEARCHSETTINGS  )
		{
			WebBlock * ite = dynamic_cast<WebBlock *> (items[j]);										
		    ssList.insert(ite);																																				
		}	
	}
	for( int j = 0; j < newMembers.size(); j++ )
	{
		if ( newMembers[j]->getType() == SETTINGS )
		{
			WebBlock * ite = dynamic_cast<WebBlock *> (newMembers[j]);										
		    sListNew.insert(ite);																																				
		}	
		else if ( newMembers[j]->getType() == SEARCHSETTINGS )
		{
			WebBlock * ite = dynamic_cast<WebBlock *> (newMembers[j]);										
		    ssList.insert(ite);																																				
		}	
	}
	set<WebBlock *> sList;	
	QList<ItemBase*> mList;


	for(set<WebBlock *>::iterator it=ssList.begin(); it!=ssList.end(); it++)
	{	
		(*it)->_settings->setInvisible();
	}

	for(set<WebBlock *>::iterator it=sListOld.begin(); it!=sListOld.end(); it++)
	{	
		(*it)->_settings->setInvisible();
	}

	mList=items;
	if(sListNew.size()<sListOld.size())
	{
	   sList=sListNew; sListNew=sListOld; sListOld=sList;
	   mList=newMembers; 
	}
		//change old set
	if(!sListNew.empty())
	{
		bool checked = (*sListNew.begin())->_settings->framelessWindowCheckBox->isChecked();
		for(set<WebBlock *>::iterator it=sListOld.begin(); it!=sListOld.end(); it++)
		{
			if(checked)
			{
				if(!(*it)->_settings->framelessWindowCheckBox->isChecked())		
					(*it)->_settings->framelessWindowCheckBox->setChecked(true);					
			}
			else
            {
				if((*it)->_settings->framelessWindowCheckBox->isChecked())		
					(*it)->_settings->framelessWindowCheckBox->setChecked(false);					
			}
		}
		for( int j = 0; j <  mList.size(); j++ )
		{
			if (  mList[j]->getType() ==  PATHBUBBLE1 ||  mList[j]->getType() ==  SUBPATHBUBBLE1)
			{
				PathBubble1* pBubble = dynamic_cast<PathBubble1*>( mList[j]);	
				if(checked)
				{
					if(!pBubble->drawSmallmolecule)		
					{
						pBubble->drawSmallmolecule=true;						
						pBubble->redrawPathWay();	
						pBubble->getGraphToBePaint();
					}	
				}
				else
				{
					if(pBubble->drawSmallmolecule)		
					{
						pBubble->drawSmallmolecule=false;	
						pBubble->redrawPathWay();	
						pBubble->getGraphToBePaint();
					}
				}			
			}
		}
	}
	//2. fix expression
	set<expressionBubble*> eListNew, eListOld;
	int size1=items.size(), size2=newMembers.size();
	for( int j = 0; j < items.size(); j++ )
	{
		if( items[j]->getType() == EXPRESSIONBUBBLE )
		{
			expressionBubble* ite = dynamic_cast<expressionBubble*> (items[j]);
			for (int m = 0; m < _scene->_expressionBubbles.size(); m++)
			{		
				if(_scene->_expressionBubbles[m])
				if(_scene->_expressionBubbles[m]->m_expressionID == ite->m_expressionID)
				{								
					eListOld.insert(_scene->_expressionBubbles[m]);																												
				}
			}					
		}
	    for( int i = 0; i < newMembers.size(); i++ )
		{	  
				if ( newMembers[i]->getType() == PATHBUBBLE1 || newMembers[i]->getType() == SUBPATHBUBBLE1 )
				{   
					PathBubble1* tmp;					
					tmp = dynamic_cast<PathBubble1*>(newMembers[i]);		
 					for (int k = 0; k < _scene->_pathBubbles.size(); k++)
					{
						//if(_scene->_pathBubbles[k]->_name.compare(items[j]->_name)==0&&items[j]->_name.size()!=0)					
						if(_scene->_pathBubbles[k])
						if(_scene->_pathBubbles[k]->m_pathwayID == tmp->m_pathwayID)// newMembers[i]->// compare(newMembers[i]->_name)==0)
						{
							if(items[j]->getType()==EXPRESSIONBUBBLE)
							{
								expressionBubble* ite = dynamic_cast<expressionBubble*> (items[j]);
								for (int m = 0; m < _scene->_expressionBubbles.size(); m++)
								{			
									if(_scene->_expressionBubbles[m])
									if(_scene->_expressionBubbles[m]->m_expressionID == ite->m_expressionID)
									{								
										_scene->_expressionBubbles[m]->m_bindPathwayID = k;	
										_scene->_expressionBubbles[m]->TransferExpression(k);	
										eList.push_back(_scene->_expressionBubbles[m]);
									}
								}
							}
						}
					}
				}
				if ( newMembers[i]->getType() == EXPRESSIONBUBBLE )
				{  
					expressionBubble* tmp = dynamic_cast<expressionBubble*>(newMembers[i]);		
 					for (int k = 0; k < _scene->_expressionBubbles.size(); k++)
					{
						if(_scene->_expressionBubbles[k])
						if(_scene->_expressionBubbles[k]->m_expressionID == tmp->m_expressionID)
						{
							eListNew.insert(_scene->_expressionBubbles[k]);		
							if(items[j]->getType()==PATHBUBBLE1 || items[j]->getType()==SUBPATHBUBBLE1)
							{
								PathBubble1* ite = dynamic_cast<PathBubble1*> (items[j]);
								tmp->m_bindPathwayID = ite->m_pathwayID;	
								for (int m = 0; m < _scene->_pathBubbles.size(); m++)
								{							
									if(_scene->_pathBubbles[m])
									if(_scene->_pathBubbles[m]->m_pathwayID == ite->m_pathwayID)
									{								
										_scene->_expressionBubbles[k]->TransferExpression(ite->m_pathwayID);	
										eList.push_back(_scene->_expressionBubbles[k]);
									}
								}
							}
						}
					}
				}
				
		}	
	}
	
	if(newMembers.size()==1)
	if(init->getType() ==REACTIONBUBBLE3)
	if(newMembers.last()->getType() == REACTIONBUBBLE3)
	{
		vector<int> item(2,0);
		this->_scene->getOpenManager()->openReactionBubble3(0, 50, (newMembers.last()->scenePos().x()+init->scenePos().x())/2.0,newMembers.last()->scenePos().y()+50, item, QString("???"));
		//keqin
		//ReactionBubble3 *rb = dynamic_cast <ReactionBubble3*> (_open->openReactionBubble3(50, event->scenePos().x(),event->scenePos().y(), qstr));		
	}
		
	//	in which group
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
	int nsize=newMembers.size();
	
	logAddToGroup( newMembers, index );
	QList<Group*> removeList;
	QList<Group*> toBeCombinedList;
	if ( newMembers.size() > 1 && isSameGroup )
	{
		int addedIndex = findGroupIndex( newMembers[0] );		
		//this->_list[index]->groupCombine( this->_list[addedIndex] );//this->_list[addedIndex]->groupCombine( this->_list[index] );
		toBeCombinedList.append(this->_list[addedIndex]);
		removeList.append( _list[addedIndex] );
	}
	else
	{
		int nsize=newMembers.size();
		int lisize=this->_list.size();		
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
						//this->_list[index]->groupCombine( _list[j] ); //Include Change colorboarder
						toBeCombinedList.append(_list[j]);
						if( !removeList.contains( _list[j] ) )
						{
							removeList.append( _list[j] );
						}
					}
				}
			}
		}		
	}
	
	set<int> Init= this->_list[index]->groupCombine( toBeCombinedList, updatePos);
	if(updatePos)
	{
		//bubbleGrouped(this->_list[index], toBeCombinedList);
		bubbleGrouped(this->_list[index], init, Init, false, false);
	}	
	for ( int i = 1; i < newMembers.size(); i ++ )
	{
		if ( ! sameGroup( newMembers[i-1], newMembers[i] ) )
		{
			isSameGroup = false;
			break;
		}
	}
			
	for( int i = 0; i < removeList.size(); i++ )
	{
		this->_list.removeAll( removeList[i] );
	}
	
	bool expressionCleared=true;
	char searchType='P';
	PathBubble1* seedPathBubble=NULL;
	//if(items.size()>=newMembers.size()) //if one bubble show expression, then all the bubble must show expression
	{
		for(int i=0; i<items.size(); i++) 
		{
			if ( items[i]->getType() == SUBPATHBUBBLE1 || items[i]->getType() == PATHBUBBLE1)
			{
				PathBubble1* pBubble = dynamic_cast<PathBubble1*>(items[i]);				
				searchType=pBubble->searchType;			
				seedPathBubble=pBubble;
				break;
			}
		}
		int esize=eListOld.size();
		for(set<expressionBubble*>::iterator it=eListOld.begin(); it != eListOld.end(); it++) 
		{
			(*it)->checkExpressedProteinSize();	
		}
        for(set<expressionBubble*>::iterator it=eListOld.begin(); it != eListOld.end(); it++) 
		{
			 if(!(*it)->expressionCleared)
			  {
				  expressionCleared=false;
				  break;
			   }						
		}
		
	}
	//else
	{
		for(int i=0; i<newMembers.size(); i++) 
		{
			if ( newMembers[i]->getType() == SUBPATHBUBBLE1 || newMembers[i]->getType() == PATHBUBBLE1)
			{
				PathBubble1* pBubble = dynamic_cast<PathBubble1*>(newMembers[i]);				
				searchType=pBubble->searchType;
				seedPathBubble=pBubble;
				break;
			}
		}	
		int esize=eListNew.size();
		for(set<expressionBubble*>::iterator it=eListNew.begin(); it != eListNew.end(); it++) 
		{
			(*it)->checkExpressedProteinSize();	
		}
		for(set<expressionBubble*>::iterator it=eListNew.begin(); it != eListNew.end(); it++) 
		{
			//if(init!=*it)
			{
			   if(!(*it)->expressionCleared)
			   {
				  expressionCleared=false;
				  break;
			   }
			}			
		}		
	}	
	if(eListNew.size()>0 || eListOld.size()>0)
	    expressionCleared=false; //always search groupped
	
	bool searched=false;
	int eosize=eListOld.size();
	for(set<expressionBubble*>::iterator it=eListOld.begin(); it != eListOld.end(); it++) 
	{
		expressionBubble* ebubble=*it;			
		ebubble->expressionCleared=expressionCleared;					
		if(!ebubble->expressionCleared)
		{
			bool addPathHighlight=false;
		
			if(!searched && seedPathBubble!=NULL)
			{
				addPathHighlight=seedPathBubble->sharedDifferedLinkedExpressedtoHighlighted(false);
			    seedPathBubble->highlightedNodetoStored();	
			}
			ebubble->clearExpressed();
			ebubble->searchExpressedProtein(searchType);	
			if(addPathHighlight && !searched && seedPathBubble!=NULL)
				seedPathBubble->cleanPathHighlight();
			//ebubble->getExpressionToBePaint();
			searched=true;
		}			
		ebubble->checkExpressedProteinSize();
		
	}
	int ensize=eListNew.size();
	for(set<expressionBubble*>::iterator it=eListNew.begin(); it != eListNew.end(); it++) 
	{
		expressionBubble* ebubble=*it;
		ebubble->expressionCleared=expressionCleared;		
		if(!ebubble->expressionCleared)
		{
			bool addPathHighlight=false;
			if(!searched && seedPathBubble!=NULL)
			{
				addPathHighlight=seedPathBubble->sharedDifferedLinkedExpressedtoHighlighted(false);
			    seedPathBubble->highlightedNodetoStored();	
			}
			ebubble->clearExpressed();
			ebubble->searchExpressedProtein(searchType);
			if(addPathHighlight && !searched && seedPathBubble!=NULL)
				seedPathBubble->cleanPathHighlight();
			//ebubble->getExpressionToBePaint();
			searched=true;
		}
		ebubble->checkExpressedProteinSize();	
		
	}
	if(searched && seedPathBubble!=NULL)
	{
		seedPathBubble->searchedNodetoStored(SEARCHEXPRESSED);
		_scene->expressedGeneNames = seedPathBubble->getExpressedGeneNames();
		_scene->getGroup(seedPathBubble)->expressedSets = seedPathBubble->assignExpressedSets();
	}	
	
	//vector<ItemBase *>  OpenGLScene::extractTypeBubblesinGroup(ItemBase * item, int Type)
	
	int tEdgeBy=-1;
	vector<ItemBase *> tList = _scene->extractTypeBubblesinGroup(init, TREERING);
	for(int i=0; i < tList.size(); i++) 
	{
		treeRingBubble* titem = dynamic_cast<treeRingBubble* >(tList[i]);
		sTemp = titem->_treeRing->getOrthFileName(); 
		int edge_by=titem->GetEdgeBy();
		if(sTemp!="")
		{
			oName = sTemp;
		}
		
		if(tList[i]!=init)
		   tEdgeBy = edge_by;
	}

	for(int i=0; i < tList.size(); i++) 
	{
		treeRingBubble* titem = dynamic_cast<treeRingBubble* >(tList[i]);
		int edge_by=titem->GetEdgeBy();
		titem->searchSharedProtein();
		titem->searchExpressed();
		sTemp = titem->_treeRing->getOrthFileName(); 
		if(sTemp=="" && oName!="")
		{
		    titem->_treeRing->ComputeNodeColorByO(oName);	
			//titem->_treeRing->SetColorBy(EDGE_BY_ORTHOLOGY);
			//titem->EDGE_BY=EDGE_BY_ORTHOLOGY;		
			titem->SetEdgeBy(EDGE_BY_ORTHOLOGY);
		}		
		if(Type!=EXPRESSIONBUBBLE && Type!=TREERING)
		{
			if(tEdgeBy>0)
			{
				titem->SetEdgeBy(tEdgeBy);
			}
			else if( edge_by != EDGE_BY_EXPRESSION && Type!=EXPRESSIONBUBBLE)
			{
				titem->SetEdgeBy(edge_by);
			}
		}
		else if(Type==TREERING)
		{
		    if(tEdgeBy>0)
			{
				titem->SetEdgeBy(tEdgeBy);
			}		
		}	
		else
		   titem->SetEdgeBy(EDGE_BY_EXPRESSION);
		   		
	}
	
	index = findGroupIndex( init );
	if(index>=0)
	if(init->getType()==EXPRESSIONBUBBLE || init->getType()==PATHBUBBLE1 || init->getType()==SUBPATHBUBBLE1)
	{  
		for( int i = 0; i < _list[index]->NumOfMembers(); i ++)	
		{
			ItemBase *item = _list[index]->getMemberAt(i);
			
			if( item->getType()==PATHBUBBLE1 || item->getType() == SUBPATHBUBBLE1)	
			{
				PathBubble1 *ptmp = dynamic_cast<PathBubble1 *>(item);
				if(oName!="")
				   ptmp->searchOrthology(oName);			
				ptmp->getGraphToBePaint();				
			}
		}
	}

	for (int m = 0; m < _scene->_expressionBubbles.size(); m++)
	{			
		 if(_scene->_expressionBubbles[m])
		 {
			_scene->_expressionBubbles[m]->searchSharedProtein();
		   _scene->_expressionBubbles[m]->getExpressionToBePaint();	
		 }
	}
	return;
}

bool GroupManager::bubbleGrouped(Group *aGroup, ItemBase *init, set<int> Init, bool notLayoutGroup, bool setPos)
{		
		bool bubbleChanged=false;
		//if(aGroup->getLayoutType() == LINEUP)
		{
			//put related bubbles (top pathbubble with sub-pathbubble) into family unit
			QList<ItemBase *> members=aGroup->getMembers();			
		    set<int> initF;
			vector<set<int> > familySets = GroupToFamily(members);//, init); // do not need to separate a familty member even is init is in one			
			if(familySets.size()>members.size())
			{
			    familySets=familySets;
			}
			set<int> InitBackup;

			vector< set<int> > familySetsBackup = familySets;
			if(Init.size()<members.size()-Init.size()) //bigger size group priority
			{
			    //switch Affected and Init here
				set<int> Affected; //
			    for( int i = 0; i < members.size(); i++ )
				{
					if( Init.find(i) == Init.end() )
					{
						Affected.insert(i);
					}							
				}
				InitBackup=Init;
				Init=Affected;
			}
			
			vector<QRectF> rectList = aGroup->assignRectList(aGroup->getMembers());						
			vector<QRectF> aList=rectList;

			//if initSet is within a family, separate them into two family			
			for(set<int>::iterator it=Init.begin(); it!=Init.end(); it++)
		    {
				int id=*it;				
				for(int i=0; i<familySets.size(); i++)
				{
				   if(familySets[i].size()>1)
				   if(familySets[i].find(id)!=familySets[i].end())
				   {
					  familySets[i].erase(id);
					  set<int> tSet; 
					  tSet.insert(id);
					  familySets.push_back(tSet);
					  break;
				   }
				}
			}

			for(set<int>::iterator it=Init.begin(); it!=Init.end(); it++)
			{
				int id=aGroup->getFamilyID(*it, familySets);
				if(id>=0)
				  initF.insert(id); 	
				
			}		
			
			if(aGroup->isOverlaid( rectList))
			{
				vector<int> parentList = getParentList(members, familySets);
				vector<QRectF> testList = aGroup->lineUpDisplay(BUBBLEGROUPED, members, parentList, initF, members.size(), familySets, rectList, members[0]->screenWidth, members[0]->screenHeight);		
                bool redoflag=false;
				if(!InitBackup.empty())
                {	 
					int id;
					id=*InitBackup.begin();
					float disx = fabs(testList[id].x() - rectList[id].x());
					float disy = fabs(testList[id].y() - rectList[id].y());
				    if(disx > testList[id].width()/2.0 || disy > testList[id].height()/2.0)
					{	
						redoflag=true;

						Init=InitBackup;
						familySets = familySetsBackup;
						initF.clear();
									//if initSet is within a family, separate them into two family			
						for(set<int>::iterator it=Init.begin(); it!=Init.end(); it++)
						{
							int id=*it;				
							for(int i=0; i<familySets.size(); i++)
							{
							   if(familySets[i].size()>1)
							   if(familySets[i].find(id)!=familySets[i].end())
							   {
								  familySets[i].erase(id);
								  set<int> tSet; 
								  tSet.insert(id);
								  familySets.push_back(tSet);
								  break;
							   }
							}
						}

						for(set<int>::iterator it=Init.begin(); it!=Init.end(); it++)
						{
							int id = aGroup->getFamilyID(*it, familySets); 
							if(id>=0)
							   initF.insert(id); 		
						}		
						if(!initF.empty())
						{
							parentList = getParentList(members, familySets);
				            rectList = aGroup->lineUpDisplay(BUBBLEGROUPED, members, parentList, initF, members.size(), familySets, rectList, members[0]->screenWidth, members[0]->screenHeight);		                
						}
					}
					else
						rectList=testList;
				}
				else
				{
				    rectList=testList;
				}
				int initID=aGroup->getFamilyID(aGroup->getMemberID(init), familySets);
				if(initID>=0)
				    aGroup->adjustOverlaid(initID, rectList, parentList, familySets, members[0]->screenWidth, members[0]->screenHeight, QRectF(-100000,-100000,-100000,-100000), -1, 0, 0);
				//add it to prevent overlap since that is a bug when group bubble, still some overlaid

				vector<QPointF> stepSize;
				if(!setPos)
				{
					stepSize = getSynchronizedSpeed(members, aList, rectList);
				}
				for(int i=0; i<members.size(); i++)
				{
					if(rectList[i].x()>-10000 && rectList[i].x()<10000 && aList[i]!=rectList[i])
					{
						float x = rectList[i].x() == aList[i].x()? members[i]->pos().x() : float(rectList[i].x())+float(rectList[i].width())/2.0;
						float y = rectList[i].y() == aList[i].y()? members[i]->pos().y() : float(rectList[i].y())+float(rectList[i].height())/2.0;
						if(members[i]->getType()!=CALLOUT)
						{
							//if(setPos)
							   members[i]->mySetPos( QPointF(x, y));		
							//else 
							//	members[i]->setTargetPos( QPointF(x, y),stepSize[i], true);	
							bubbleChanged=true;
						}
						
					}
				}

				if(!notLayoutGroup && bubbleChanged)
				    lineUpDisplayGroups(members, rectList, false);//keqin
			}
    }		
	return bubbleChanged;
}

vector<set<int> > GroupManager::GroupToFamily(QList<ItemBase *> list)
{
    vector<set<int> > familySets;
	//Group *B=new Group();
	for(int i=0; i<list.size(); i++)
	{
		//top level
		//if(B->getFamilyID(i,familySets)>=0)
		//	continue;			

		if(list[i]->isVisible() && list[i]->getType() >= 0 && (list[i]->getType() != COLOR || list[i]->getType() != COMPLOT ))
		{
		   set<int> tSet;
		   tSet.insert(i);
		   familySets.push_back(tSet);
		}
	}
	/*set<int> currentF;
	//vector<int> list;
	Group *B=new Group();
	//get all group member	
	for(int i=0; i<list.size(); i++)
	{
		//top level
		if(B->getFamilyID(i,familySets)>=0)
			continue;			
		if( (list[i]->getType()==PATHBUBBLE1 || list[i]->getType()==SUBPATHBUBBLE1) )
		{
			PathBubble1 *pBubble= dynamic_cast <PathBubble1 *>(list[i]);				
			QList<ItemBase *> AList;
			pBubble->getAllPSPathBubble(pBubble, AList );					
			//append A list to list and append familySets	
			set<int> tSet;
			for(int k=0; k<list.size(); k++)
			{
				bool flag=false;
				for(int j=0; j<AList.size(); j++)
				{
					if(list[k]==AList[j])
					{
						tSet.insert(k);	
						flag=true;
						break;
					}
				}
			    if(!flag)
				{					
				}
			}	
			if(!tSet.empty())
			{
				familySets.push_back(tSet);	 //first family							
			}
		}
		else if(list[i]->isVisible() && list[i]->getType() >= 0 && (list[i]->getType() != COLOR || list[i]->getType() != COMPLOT ))
		{
		   set<int> tSet;
		   tSet.insert(i);
		   familySets.push_back(tSet);
		}
	}

	    //note: there are pathbubble who is not toplevel and is ungroup with his parents
	delete B;*/

	return familySets;
	
}



bool GroupManager::sameFamilySet(int id1, int id2, vector< set<int> > familySets)
{
	int id1F, id2F;
	Group *B;	
    if (B->getFamilyID(id1, familySets) == B->getFamilyID(id2, familySets))
		return true;
	else return false;
	
}

vector<int> GroupManager::getParentList(QList<ItemBase *> list, vector< set<int> > familySets, ItemBase *parent)
{
	int size=list.size();
	vector<int> pList(size,-1);
	int pi=-1;
	for(int i=0; i<list.size(); i++)
	{
	   if(list[i]==parent)
	   {
		  pi=i;
		  break;
	   }
	}
	//get all group member	
	for(int i=0; i<list.size(); i++)
	{
		if( (list[i]->getType()==PATHBUBBLE1 || list[i]->getType()==SUBPATHBUBBLE1) )
		{
			PathBubble1 *pBubble= dynamic_cast <PathBubble1 *>(list[i]);				
			PathBubble1 *tBubble=pBubble->_pathBubbleParent;					
			ItemBase * tItem = dynamic_cast<ItemBase *>(tBubble);
			if(sameGroup(tItem,list[i])  )
			{
				int id1=list.indexOf(tItem);
				if(!sameFamilySet(i,id1,familySets))
				{
					pList[i]=id1;
				}
			}
		}		
		else if( list[i]->getType()==TEXTBUBBLE2 )
		{
			TextBubble2 *tBubble= dynamic_cast <TextBubble2 *>(list[i]);				
			ItemBase * tItem;
			if(tBubble->_pathBubbleParent!=NULL)
			{
				PathBubble1 *pBubble=tBubble->_pathBubbleParent;	
				tItem = dynamic_cast<ItemBase *>(pBubble);			
			}	
			else if(tBubble->getCodeParent()!=NULL)
			{
				TextBubble2 *pBubble=tBubble->getCodeParent();	
				tItem = dynamic_cast<ItemBase *>(pBubble);			
			}				
			else if(sameGroup(tItem,list[i])  )
			{
				int id1=list.indexOf(tItem);
				if(!sameFamilySet(i,id1,familySets))
				{
					pList[i]=id1;
				}
			}			
		}
		else if( list[i]->getType()==TREERING )
		{
			treeRingBubble *pBubble= dynamic_cast <treeRingBubble *>(list[i]);				
			treeRingBubble *tBubble=pBubble->_treeRingBubbleParent;					
			ItemBase * tItem = dynamic_cast<ItemBase *>(tBubble);
			if(sameGroup(tItem,list[i])  )
			{
				int id1=list.indexOf(tItem);
				if(!sameFamilySet(i,id1,familySets))
				{
					pList[i]=id1;
				}
			}	
		}
		if(pList[i]==-1 && pi!=-1)
		{
		    pList[i]=pi;		
		}
	}	
	return pList;
}



vector<set<int> > GroupManager::GroupToFamily(QList<ItemBase *> list, ItemBase *init)
{	
	// if a init is within a family, than separate its higherlevel member, it, and individual others into different family
    vector<set<int> > familySets;
	set<int> currentF;
	//initID
	
	//vector<int> list;
	Group *B=new Group();
	int size=list.size();
	//get all group member	
	for(int i=0; i<list.size(); i++)
	{
		//top level
		if(B->getFamilyID(i,familySets)>=0)
			continue;			
		if( (list[i]->getType()==PATHBUBBLE1 || list[i]->getType()==SUBPATHBUBBLE1) )
		{
			PathBubble1 *pBubble= dynamic_cast <PathBubble1 *>(list[i]);				
			QList<ItemBase *> AList;
			pBubble->getAllPSPathBubble(pBubble, AList );					
			//append A list to list and append familySets	
			set<int> tSet;
			for(int k=0; k<list.size(); k++)
			{
				for(int j=0; j<AList.size(); j++)
				{
					if(list[k]==AList[j])
					{
						tSet.insert(k);	
						break;
					}
				}			    
			}
			//get init id
			int initID=-1;
			for(int j=0; j<list.size(); j++)
			{
			    if(list[j] == init)
					initID=j;			
			}
			if(tSet.size()>1 && initID!=-1 && tSet.find(initID)!=tSet.end() ) //seperate it
			{
			    //familySets.push_back(init)
				set<int> aSet; 
				aSet.insert(initID); 
				familySets.push_back(aSet);
				tSet.erase(initID);

                //upperlevel is one set : familySets.push_back
				PathBubble1 *aBubble= dynamic_cast <PathBubble1 *>(init);				
				QList<ItemBase *> TList;
				aBubble->getHigherPathBubble(aBubble,TList);
				set<int> bSet; 
				for(int k=0; k<TList.size(); k++)
				{
			        //get same level: for ( familtySets.push_back
					for(int j=0; j<list.size(); j++)
					{
						 if(list[j] == TList[k] && tSet.find(j) != tSet.end() )
						 {
							 bSet.insert(j);
							 tSet.erase(j);
						 }
					}
				}
				if(!bSet.empty())
				{
					if(familySets.size()>list.size())
			             familySets=familySets;
					familySets.push_back(bSet);
				}
				
				//put rest (a same level bubble with its offsprings) into individule bubbles				
				while(!tSet.empty())
				{
					set<int> cSet; 
					set<int>::iterator m=tSet.begin();
					PathBubble1 *bubble= dynamic_cast <PathBubble1 *>(list[*m]);	
					
					bool hasFatherinGroup=false;
					QList<ItemBase *> AList;
				    aBubble->getHigherPathBubble(aBubble,AList);
				    set<int> bSet; 
				    for(int k=0; k<AList.size(); k++)
					{
						for(set<int>::iterator it=tSet.begin(); it!=tSet.end(); it++)
						{
							 if(list[*it] == AList[k])
							 {
								 hasFatherinGroup=true;
								 break;
							 }
						}					
					}
					if(!hasFatherinGroup)
					{
						QList<ItemBase *> TList;
						bubble->getAllSubPathBubble(bubble, TList);
						for(int k=0; k< TList.size(); k++)
						{
							 for(int j=0; j<list.size(); j++)
							{
								 if(list[j] == TList[k])
								 {
									 cSet.insert(j);
									 tSet.erase(j);
								 }
							}
						} 					
						cSet.insert(*m);						
						familySets.push_back(cSet); 
						if(familySets.size()>list.size())
			               familySets=familySets;
					}
					tSet.erase(*m);					
				}
			}
			else if(!tSet.empty()) 
			{
				familySets.push_back(tSet);	 //first family							
				if(familySets.size()>list.size())
			            familySets=familySets;
			}
		}
		else if(list[i]->isVisible() && list[i]->getType() >= 0 && (list[i]->getType() != COLOR || list[i]->getType() != COMPLOT ))
		{
		   set<int> tSet;
		   tSet.insert(i);
		   if(familySets.size()>list.size())
			   familySets=familySets;
		   familySets.push_back(tSet);
		}
	}
	
	for(int i=0; i<familySets.size(); i++)
	{
		for(int j=i+1; j<familySets.size(); j++)
		{
			for(set<int>::iterator it=familySets[i].begin(); it!=familySets[i].end(); it++)
			{
			   if(familySets[j].find(*it)!=familySets[j].end())
			   {
				   //familySets[j].erase(*it);
			       j=j;
			   }    
			}			
		}
	}
	 
	vector<set<int> > FamilySets;
	for(int i=0; i<familySets.size(); i++)
	{
		if(!familySets[i].empty())
		    FamilySets.push_back(familySets[i]);
		else
		{
		   i=i;
		}
	}
	
	//note: there are pathbubble who is not toplevel and is ungroup with his parents
	delete B;	
	return familySets;	
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

bool GroupManager::Overlaid(ItemBase *left, ItemBase *right)
{
	//if(left->isUnGroup() || right->isUnGroup())
	//	return false;
	//use even number for bubble size in case problems since scenebounding rect are RectF while realRect are QRect
	int disX = abs( left->sceneBoundingRect().center().x() - right->sceneBoundingRect().center().x() );
	int disY = abs( left->sceneBoundingRect().center().y() - right->sceneBoundingRect().center().y() );

	/*QRectF lsRect=left->sceneBoundingRect(), rsRect=right->sceneBoundingRect();
	QRect lRect=left->realRect(), rRect=right->realRect();
	
	QPointF lcenter= left->sceneBoundingRect().center(), rscenter= right->sceneBoundingRect().center();
	int lwidth= left->realRect().width(), rwidth= right->sceneBoundingRect().width();*/

	//int disX = abs( left->realRect().x() - right->realRect().x() );
	//int disY = abs( left->realRect().y() - right->realRect().y() );

	int thresh = ( left->realRect().width() + right->realRect().width() ) / 2;
	int thresh2 = ( left->realRect().height() + right->realRect().height() ) / 2;
	if( disX < thresh && disY < thresh2 )
	{
		//if( left->getType() ==TREERING !left->isVisible() || !right->isVisible() || right->m_treeRingBubbleID<0 || right->m_treeRingBubbleID>10 || left->m_treeRingBubbleID<0 || left->m_treeRingBubbleID>10)
		int lType=left->getType(), rType=right->getType(), rVisible=right->isVisible(), lVisible=left->isVisible(), rid=right->m_treeRingBubbleID, lid=left->m_treeRingBubbleID;		

		return true;
	}
	return false;
	//return(RectRect(left->pos().x(),left->pos().y(),right->pos().x(),right->pos().y(),left->Width(),left->Height(),right->Width(),right->Height()));
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


QList<Group*> GroupManager::allGroups()
{
	QList<Group*> result; //J.C.	
	for (int i = 0; i < this->_list.size(); i ++)
	{
		if ( this->_list[i] )
		{
			result.append( this->_list[i] );
		}
	}
	return result;
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

QList<ItemBase*> GroupManager::getGroupMembers(ItemBase* init)
{
	QList<ItemBase*> Blist;
		for( int j = 0; j < this->_list.size(); j++ )
		{
			if( this->_list[j]->inThisGroup( init) )
			{
				return(this->_list[j]->getMembers());
			}			
		}	
	return Blist;
}

Group* GroupManager::getGroup(ItemBase* init)
{
	Group* B=NULL;
	for( int j = 0; j < this->_list.size(); j++ )
	{
		if( this->_list[j]->inThisGroup( init) )
		{
			 return(_list[j]);
		}			
	}	
	return B;
}

QList<Group*> GroupManager::groupList()
{
	return _list;
}

void GroupManager::itemRemoved( ItemBase *item )
{
	if(item->getType()==EXPRESSIONBUBBLE)
	{
		expressionBubble *etmp = dynamic_cast<expressionBubble *>(item);
		QList<ItemBase *> mList=_scene->getGroupMembers(item);	
		for( int i = 0; i < mList.size(); i ++)	
		{
			ItemBase *tmp = mList[i];
			if(tmp->getType()==PATHBUBBLE1 || tmp->getType()==SUBPATHBUBBLE1)
			{
				PathBubble1 *ptmp = dynamic_cast<PathBubble1 *>(tmp);
				ptmp->clearExpressed(etmp->m_expressionID);
				//etmp->clearExpressed(etmp->m_bindPathwayID);
			}
		}
	}
	//also update the expressed items
	if(item->getType()==PATHBUBBLE1 || item->getType()==SUBPATHBUBBLE1)
	{
		PathBubble1 *ptmp = dynamic_cast<PathBubble1 *>(item);		
		QList<ItemBase *> mList=_scene->getGroupMembers(item);	
		for( int i = 0; i < mList.size(); i ++)	
		{
			ItemBase *tmp = mList[i];
			if(item->getType()==EXPRESSIONBUBBLE)
			{
				expressionBubble *etmp = dynamic_cast<expressionBubble *>(item);
				ptmp->clearExpressed(etmp->m_expressionID);
				//etmp->clearExpressed(etmp->m_bindPathwayID);
			}
		}		
	}	
	this->removeItem( item );
	return;
}

void GroupManager::removeItem( ItemBase* item )
{
	ItemBase* tmpItem;
	//MatBlock* mat;
	//QList< MatBlock* > matBubbles;

	/*if (item->getType() == MATLAB)
	{
		mat = dynamic_cast<MatBlock*>(item);
		mat->getCodeParent()->removeCodeCon( mat );
		mat->removeAllCodeSon();

		mat->removeDataVis();
	}
	*/
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

void GroupManager::removeGroup( Group* group)
{
	ItemBase* tmpItem;
	//MatBlock* mat;
	//QList< MatBlock* > matBubbles;

	/*if (item->getType() == MATLAB)
	{
		mat = dynamic_cast<MatBlock*>(item);
		mat->getCodeParent()->removeCodeCon( mat );
		mat->removeAllCodeSon();

		mat->removeDataVis();
	}
	*/	
	this->_list.removeAt(this->_list.indexOf(group));	
	return;
}

void GroupManager::unGroup(ItemBase *init)
{
	//update sharedset
	if(init->getType()==PATHBUBBLE1 || init->getType()== SUBPATHBUBBLE1)
	{		
		Group *B=getGroup(init);
		B->updateSharedSet(init);
		PathBubble1 *pBubble=dynamic_cast<PathBubble1 *> (init);
		//pBubble->updateSharedLinkedDIffered();
		_scene->clearSearchSelection(pBubble);
		pBubble->setExpressionName("");
	}	
	QList<ItemBase *> mList = getGroupMembers(init);
	int excludePid=-1;
	int EdgeBy=-1;
	int Type = init->getType();
	if( init->getType()==SUBPATHBUBBLE1 || init->getType()==PATHBUBBLE1) 
	{
		excludePid = init->m_pathwayID;
	}	
	if(init->getType()==SEARCHSETTINGS || init->getType()==SETTINGS)
	{
		WebBlock *settingItem = dynamic_cast<WebBlock *>( init);			   
		settingItem->_settings->setVisible();		
	}
	else
	{
		int unSettingCount=0;
		for(int i=0; i<mList.size(); i++)
		{
			int type=mList[i]->getType();		
			if( mList[i]!=init && (type==SUBPATHBUBBLE1 || type==PATHBUBBLE1)  ) 
			{
				PathBubble1 *pBubble=dynamic_cast<PathBubble1 *> (mList[i]);
				pBubble->setExpressionName("");
				unSettingCount++;
			}
		}	
		if(unSettingCount==0)
		{
			for(int i=0; i<mList.size(); i++)
			{
				int type=mList[i]->getType();	
				if( type==SEARCHSETTINGS || type==SETTINGS) 
				{
					WebBlock *settingItem = dynamic_cast<WebBlock *>(mList[i]);			   
			        settingItem->_settings->setVisible();	
				}
			}	
		}
	}
	vector<ItemBase *> tList = _scene->extractTypeBubblesinGroup(init, TREERING);
	for(int i=0; i < tList.size(); i++) 
	{
		treeRingBubble* titem = dynamic_cast<treeRingBubble* >(tList[i]);
		EdgeBy = titem->GetEdgeBy();
	}
	int count1=0;
	for(int i=0; i<mList.size(); i++)
	{
		if(mList[i]->getType()==EXPRESSIONBUBBLE && mList[i]!=init)
		{
			count1++;
		}
	}
	if(init->getType()==PATHBUBBLE1 || init->getType()== SUBPATHBUBBLE1)
	{		
		int countp=0,countt=0;
		bool expressionCleared=true;
		char searchType='P';
		
		for(int i=0; i<mList.size(); i++)
		{
			int type=mList[i]->getType();
			if(type==PATHBUBBLE1 || type== SUBPATHBUBBLE1)
			{
			   countp++;
			   PathBubble1* pbubble= dynamic_cast<PathBubble1*>(mList[i]);			
			   searchType=pbubble->searchType;
			}
			else if(type==EXPRESSIONBUBBLE)
			{
				expressionBubble* ebubble = dynamic_cast<expressionBubble*>(mList[i]);
		        expressionCleared = ebubble->expressionCleared;				
			}	
			else if(type==TREERING)
			{
			    countt++;
			}
		}
		if(!expressionCleared)
		{
			if(countp==1)
			{
				for(int i=0; i<mList.size(); i++)
				{
					if(mList[i]->getType()==EXPRESSIONBUBBLE)
					{
						 expressionBubble* ebubble = dynamic_cast<expressionBubble*>(mList[i]);
						 ebubble->expressionCleared = true;
						 ebubble->clearExpressed();	
						 ebubble->clearSharedExpression();
					}					
				}	
			}
			else 
			{
				bool flag=false;
				for(int i=0; i<mList.size(); i++)
				{
					if(mList[i]->getType()==EXPRESSIONBUBBLE && mList[i]!=init)
					{
						//expressionBubble* ebubble = dynamic_cast<expressionBubble*>(mList[i]);
						//ebubble->searchExpressedProtein(searchType,excludePid);		
						flag=true;
					}
				}
				if(flag)
				{
					vector<ItemBase *> tList = _scene->extractTypeBubblesinGroup(init, TREERING);
					for(int i=0; i < tList.size(); i++) 
					{
						treeRingBubble* titem = dynamic_cast<treeRingBubble* >(tList[i]);
						int edge_by=titem->GetEdgeBy();
						titem->searchSharedProtein();
						titem->searchExpressed();						
						if( edge_by != EDGE_BY_EXPRESSION)
					         titem->SetEdgeBy(edge_by);
					}
				}
			}
		}
		
	}
	else if(init->getType()==TREERING)
	{		
		int count=0;
		bool expressionCleared=true;
		char searchType='P';
		
		for(int i=0; i<mList.size(); i++)
		{
			int type=mList[i]->getType();
			if(type==EXPRESSIONBUBBLE)
			{
				expressionBubble* ebubble = dynamic_cast<expressionBubble*>(mList[i]);
		        expressionCleared = ebubble->expressionCleared;				
			}	
			else if(type==TREERING)
			{
			    count++;
			}
		}
		if(!expressionCleared)
		{
			if(count==1)
			{
				for(int i=0; i<mList.size(); i++)
				{
					if(mList[i]->getType()==EXPRESSIONBUBBLE)
					{
						 expressionBubble* ebubble = dynamic_cast<expressionBubble*>(mList[i]);
						 ebubble->expressionCleared = true;
						 ebubble->clearTreeExpressed();		
					}					
				}	
			}
			else
			{
				if(count1>0)
				{
					vector<ItemBase *> tList = _scene->extractTypeBubblesinGroup(init, TREERING);
					for(int i=0; i < tList.size(); i++) 
					{
						treeRingBubble* titem = dynamic_cast<treeRingBubble* >(tList[i]);
						int edge_by=titem->GetEdgeBy();
						titem->searchSharedProtein();
						titem->searchExpressed();						
						if( edge_by != EDGE_BY_EXPRESSION)
					         titem->SetEdgeBy(edge_by);	
					}
				}
			}
		}		
		if(count==1)
		{
			for(int i=0; i<mList.size(); i++)
			{
				if(mList[i]->getType()==EXPRESSIONBUBBLE)
				{
					expressionBubble* ebubble = dynamic_cast<expressionBubble*>(mList[i]);
					ebubble->clearTreeExpressed();						
					ebubble->getExpressionToBePaint(excludePid);
				}
			}	
		}
		else
		{
			bool flag=false;
			for(int i=0; i<mList.size(); i++)
			{
				if(mList[i]->getType()==EXPRESSIONBUBBLE)
				{
					
					expressionBubble* ebubble = dynamic_cast<expressionBubble*>(mList[i]);
					if(!ebubble->expressionCleared)
					{
						flag=true;
						break;
					}
				}
			}
			if(flag)
			{
				vector<ItemBase *> tList = _scene->extractTypeBubblesinGroup(init, TREERING);
				for(int i=0; i < tList.size(); i++) 
				{
					
					treeRingBubble* titem = dynamic_cast<treeRingBubble* >(tList[i]);
					int edge_by=titem->GetEdgeBy();
					titem->searchSharedProtein();
					titem->searchExpressed();
					if( edge_by != EDGE_BY_EXPRESSION)
					    titem->SetEdgeBy(edge_by);		
				}
			}
		}		
	}
	else if(init->getType()==EXPRESSIONBUBBLE)
	{
		 expressionBubble* ebubble = dynamic_cast<expressionBubble*>(init);
		 ebubble->expressionCleared=true;
		 ebubble->clearExpressed();	
		 ebubble->clearTreeExpressed();	
		 ebubble->clearSharedExpression();
		 ebubble->getExpressionToBePaint(excludePid);
	}

	int index = findGroupIndex( init );
	if( index < 0 || this->_list[index]->NumOfMembers() == 1 )
	{
		return;
	}	
	if(init->getType()==EXPRESSIONBUBBLE)
	{
		expressionBubble *etmp = dynamic_cast<expressionBubble *>(init);
		for( int i = 0; i < _list[index]->NumOfMembers(); i ++)	
		{
			ItemBase *item = _list[index]->getMemberAt(i);	            
			if(item->getType()==PATHBUBBLE1 || item->getType()==SUBPATHBUBBLE1)
			{
				PathBubble1 *ptmp = dynamic_cast<PathBubble1 *>(item);				
			    //etmp->clearExpressed(etmp->m_bindPathwayID);
				if(!ptmp->expressedProtein.empty())
				{
					ptmp->clearExpressed(etmp->m_expressionID);
					ptmp->getGraphToBePaint();				
				}
			}
			else if(item->getType()==TREERING)
			{
				treeRingBubble* titem = dynamic_cast<treeRingBubble* >(item);	
				if(count1>0)
				{
					int edge_by=titem->GetEdgeBy();
					titem->searchSharedProtein();
					titem->searchExpressed();					
					if( edge_by != EDGE_BY_EXPRESSION)
					    titem->SetEdgeBy(edge_by);	
				}
				else
				{
					int edge_by=titem->GetPreEdgeBy();
					if( edge_by != EDGE_BY_EXPRESSION && Type==EXPRESSIONBUBBLE)
					    titem->SetEdgeBy(edge_by);		
				    //titem->EDGE_BY == 
				    //titem->SetEdgeBy(EDGE_BY_V);				
				    //titem->_treeRing->ComputeNodeColorByV();
				}
			}
		}	
		etmp->m_bindPathwayID=-1;		                                  
	}
	if(init->getType()==EXPRESSIONBUBBLE || init->getType()==PATHBUBBLE1 || init->getType()==SUBPATHBUBBLE1)
	{  
		for( int i = 0; i < _list[index]->NumOfMembers(); i ++)	
		{
			ItemBase *item = _list[index]->getMemberAt(i);
			if(item->getType()==EXPRESSIONBUBBLE)			
			{
				if( init->getType()==PATHBUBBLE1 || init->getType()==SUBPATHBUBBLE1)
				{
					PathBubble1 *ptmp = dynamic_cast<PathBubble1 *>(init);
					expressionBubble *etmp = dynamic_cast<expressionBubble *>(item);
					ptmp->clearExpressed(etmp->m_expressionID);					
				}
			}
			if( item->getType()==PATHBUBBLE1 || item->getType() == SUBPATHBUBBLE1)	
			{
				PathBubble1 *ptmp = dynamic_cast<PathBubble1 *>(item);
				ptmp->getGraphToBePaint();				
			}
		}
	}
	else if(init->getType()==TREERING)
	{
		treeRingBubble* titem = dynamic_cast<treeRingBubble* >(init);	
		if(EdgeBy>0 && EdgeBy!= EDGE_BY_EXPRESSION)
	       titem->SetEdgeBy(EdgeBy);		
		else 
		   titem->SetEdgeBy(EDGE_BY_V);		
	}
	this->_list[index]->removeItem( init );
	newItemAdded( init );

	for(int i=0; i<mList.size(); i++)
	{
		if(mList[i]->getType()==EXPRESSIONBUBBLE)
		{
			expressionBubble* ebubble = dynamic_cast<expressionBubble*>(mList[i]);
			//ebubble->clearExpressed();	
			ebubble->searchSharedProtein();
			ebubble->searchExpressedProtein();
			ebubble->getExpressionToBePaint(excludePid);
		}
	}	
		
	myColor3 color=this->_list[index]->getGroupColor(this->_list[index]);	
	for( int i = 0; i < _list[index]->NumOfMembers(); i ++)	
	{
		ItemBase *item = _list[index]->getMemberAt(i);	
		item -> setColor( color );
	}

	int type=init->getType();
	myColor3 initColor;
	if(type == PATHBUBBLE1 || type == SUBPATHBUBBLE1)
	{
		if(init->toplevel)
			initColor=TDHelper::getBoarderColorByType(PATHBUBBLE1, init->m_pathwayID);
		else
			initColor=TDHelper::getBoarderColorByType(SUBPATHBUBBLE1, init->m_pathwayID);
	}
	else if (type == EXPRESSIONBUBBLE)
	{
	    initColor=TDHelper::getBoarderColorByType(EXPRESSIONBUBBLE, init->m_expressionID);
	}
	else if (type == TREERING)
	{
		//treeRingBubble* tmp1 = dynamic_cast<treeRingBubble*>(init);		
		initColor=TDHelper::getBoarderColorByType(TREERING, init->m_treeRingBubbleID);
	}
	else
		initColor=TDHelper::getBoarderColorByType(type);
	
	init->setColor(initColor);
	if ( init->getLabel() != NULL )
	{
		init->getLabel()->show();
	}

	if(init->getType() > -1 && ( init->getType() != COLOR || init->getType() != COMPLOT ))
	{
		if(this->_list[index]->NumOfMembers()>0)
		if(tightenGroupMembers(BUBBLEUNGROUPED, this->_list[index]->getMemberAt(0), QRectF(-100000,-100000,-100000,-100000), -1, false,false,QRectF(-100000,-100000,-100000,-100000),false))
		{

		}				
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

void GroupManager::findAndHighlight( ItemBase* &init )
{
	QList<ItemBase*> members = allMembers();
	if(members.isEmpty()) 
	{ 
		return;
	}
	QList<ItemBase*> items = getCurrentItems( init );
	QList<ItemBase*> needToAdd;
	bool flag=false;
	int msize=0, isize=0;
	msize=members.size(); 
	isize=items.size();
	for( int i = 0; i < members.size(); i++ )
	{
		for( int j = 0; j < items.size(); j++ )
		{
			if( members[i] == items[j] || sameGroup( members[i], items[j] ) )
			{
				continue;
			}
			else if( members[i]->isVisible() && items[j]->isVisible() && Overlaid( members[i], items[j] ) ) // && !Enclosed(members[i], items[j]) ) 
			{
				if( !isInThisList( needToAdd, members[i]) )
				{
					needToAdd.append( members[i] );
					flag = true;
				}
			}
		}
	}
	//
	if(needToAdd.empty())
	{
		unHightLightAll();	    
	}
	else 
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

void GroupManager::highLightGroup(QList<ItemBase*> newMembers, ItemBase* &init )
{
	for( int i = 0; i < newMembers.size(); i++ )
	{
		myColor3 color;
		bool flag=false;
		for( int j = 0; j < this->_list.size(); j++ )
		{
			if( this->_list[j]->inThisGroup( newMembers[i]) )
			{
				this->_list[j]->highLight( color=this->_list[j]->getGroupHighlightColor(init) );//init->getColor() 
				flag=true;
				init->hightLight(color);
				//init->update();
			}
		}
		if(flag)
		{
			newMembers[i]->hightLight(color);
			//newMembers[i]->update();
		}
	}	
	return;
}

//myColor3 color=getGroupColor(target);

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
//		QMouseEvent event( QEvent::MouseButtonPress, QPointF( newPos.x(), newPos.y()), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
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
//		QMouseEvent event(QEvent::MouseMove, QPointF( newPos.x(), newPos.y()), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
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
//		QMouseEvent event(QEvent::MouseButtonRelease, QPointF( newPos.x(), newPos.y()), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
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

void GroupManager::DisperseAndTighteninSameGroup()
{ //disperse and tighten  //keqin
	vector <vector <bool> > record;
	record.resize(this->_list.size());
	
	for (int i = 0; i < this->_list.size(); i ++)
	{
		this->_list[i]->tightenItems(record[i]);
	}

	for (int i = 0; i < this->_list.size(); i ++)
	{
		this->_list[i]->disperseItems(record[i]);
	}

	return;
}

void GroupManager::sliderChanged(ItemBase *bat, int number )
{
	float rate = float( number ) / 100.0;
	int tindex = findGroupIndex( bat );
	if (tindex > -1)
	{
		//this->_list[tindex]->setBatGroupFrameState( rate, number );
	}
	return;
}

void GroupManager::surfaceChecked(ItemBase *bat, int state )
{
	int tindex = findGroupIndex( bat );
	if (tindex > -1)
	{
		//this->_list[tindex]->setBatGroupSurfaceState( state );
	}
	return;
}

void GroupManager::wireframeChecked(ItemBase *bat, int state )
{
	int tindex = findGroupIndex( bat );
	if (tindex > -1)
	{
		//this->_list[tindex]->setBatGroupWireframeState( state );
	}
	return;
}

void GroupManager::symbolChecked(ItemBase *bat, int state)
{
	int tindex = findGroupIndex( bat );
	if (tindex > -1)
	{
		//this->_list[tindex]->setBatGroupSymbolState( state );
	}
	return;
}

void GroupManager::camber1Checked(ItemBase *bat, int state )
{
	int tindex = findGroupIndex( bat );
	if (tindex > -1)
	{
		//this->_list[tindex]->setBatGroupCamberState( 0, state );
	}
	return;
}
void GroupManager::camber2Checked(ItemBase *bat, int state )
{
	int tindex = findGroupIndex( bat );
	if (tindex > -1)
	{
		//this->_list[tindex]->setBatGroupCamberState( 1, state );
	}
	return;
}
void GroupManager::camber3Checked(ItemBase *bat, int state )
{
	int tindex = findGroupIndex( bat );
	if (tindex > -1)
	{
		//this->_list[tindex]->setBatGroupCamberState( 2, state );
	}
	return;
}

void GroupManager::updateWingShapeParainGroup( ItemBase* bat, ArrowGen* agen )
{
	int tindex = findGroupIndex( bat );
	if (tindex > -1)
	{
		//this->_list[tindex]->updateBatGroupWingShapePara( agen );
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

void GroupManager::pressHightlightinGroup( ItemBase *item, QRectF rect )
{
	int tindex = findGroupIndex( item );
	if ( tindex > -1 )
	{
		this->_list[tindex]->pressHightlight( rect );
	}
	return;
}

void GroupManager::moveHightlightinGroup( ItemBase *item, QRectF rect )
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
    // move bubbles toward its destination
	vector <vector <bool> > record;
	record.resize(this->_list.size());
	
	for (int i = 0; i < this->_list.size(); i ++)
	{
		this->_list[i]->tightenItems(record[i]);
	}
	
	//this->disperseItemsinSameGroup();	
	//this->tightenItemsinSameGroup();	
	//this->DisperseAndTighteninSameGroup(); //keqin
}

void GroupManager::addNewGroupFromXmlNode( QDomElement node )
{
	QDomElement childElement, tmpPara;
	QList<QDomElement> ComNodeList;

	QDomNodeList para;
	QString type;

	ItemBase *add, *init;
	QList<ItemBase*> needToAdd;
	ComNodeList.clear();
	childElement = node.firstChildElement();
	int count=0;
	while ( !childElement.isNull())
	{
		if (childElement.tagName() == "Bubble")
		{
			//childElement = eachElement.firstChild().toElement();
			//QDomElement eachBubble = childElement.toElement();
			//bubbleID = eachBubble.tagName();
			QString bubbleID = childElement.attribute("bubbleID");	

			ItemBase* item; 
			para = childElement.elementsByTagName( tr("Type") );
			if ( para.size() == 1)
			{
				tmpPara = para.at(0).toElement();
				type = tmpPara.text();

				if ( type == "Bat" )
				{
					//item = this->_scene->getOpenManager()->openBatFromXmlNode( childElement );
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
					//item = this->_scene->getOpenManager()->openMatlabFromXmlNode( childElement );
				}
				else if ( type == "Note" )
				{
					item = this->_scene->getOpenManager()->openNoteFromXmlNode( childElement );
				}
				else if ( type == "PathBubble0" )
				{
					item = this->_scene->getOpenManager()->openPathBubble0FromXmlNode( childElement );
				}
				else if ( type == "PathBubble1" )
				{
					//item = this->_scene->getOpenManager()->openPathBubble1FromXmlNode( childElement );										
					item = this->_scene->getOpenManager()->openSubPathBubble1FromXmlNode( childElement, bubbleID.toInt(), 1);									
				}
				else if ( type == "TreeRing" )
				{
					//item = this->_scene->getOpenManager()->openPathBubble1FromXmlNode( childElement );										
					item = this->_scene->getOpenManager()->openTreeRingFromXmlNode( childElement, bubbleID.toInt());									
				}
				else if ( type == "subPathBubble1" )
				{
					//item = this->_scene->getOpenManager()->openPathBubble1FromXmlNode( childElement );					
					item = this->_scene->getOpenManager()->openSubPathBubble1FromXmlNode( childElement, bubbleID.toInt(), 0);					
				}
				else if ( type == "expressionBubble" )
				{
					item = this->_scene->getOpenManager()->openExpressionBubbleFromXmlNode( childElement, bubbleID.toInt());					
				}
				else if ( type == "TextBubble2" )
				{
					item = this->_scene->getOpenManager()->openTextBubble2FromXmlNode( childElement );
				}
				else if ( type == "ReactionBubble3" )
				{
					item = this->_scene->getOpenManager()->openReactionBubble3FromXmlNode( childElement );
				}
				else if ( type == "GroupBubble4" )
				{
					item = this->_scene->getOpenManager()->openGroupBubble4FromXmlNode( childElement );
				}
			}
			if(count++==0)
			     init=item;
			else 
				addToGroup(init, item );
		}
		childElement = childElement.nextSiblingElement();
	}

	/*for (int i = 0; i < ComNodeList.size(); i ++)
	{
		childElement = ComNodeList.at(i);
		this->_scene->addLComObjectFromXmlNode( childElement );
	}*/
}

void GroupManager::drawAllGroupBoarder(QPainter *painter)
{
	for (int i = 0; i < _list.size(); i ++)
	{
		if(!_list[i]->isUnGroup())
		    _list[i]->drawGroupBoarder( painter );
	}	
	return;
}

void GroupManager::drawAllLabel(QPainter *painter)
{
	for (int i = 0; i < this->_list.size(); i ++)
	{
		for (int j = 0; j < this->_list[i]->NumOfMembers(); j ++)
		{
			ItemBase *item = this->_list[i]->getMemberAt(j);
			if(item->isVisible())
			{
				if(item->getType()==PATHBUBBLE1 || item->getType()==PATHBUBBLE1 )
				{
					PathBubble1 *bubble=dynamic_cast<PathBubble1 *> (item);
					bubble->updateLabel();
					//bubble->m_note->update();
				}
				else if(item->getType()==EXPRESSIONBUBBLE)
				{
					expressionBubble* bubble = dynamic_cast<expressionBubble*>(item);	
					bubble->updateLabel();
				}
				else if(item->getType()==TEXTBUBBLE2)
				{
					TextBubble2 *bubble=dynamic_cast<TextBubble2 *> (item);
					bubble->updateLabel();
				}
				else if(item->getType()==CALLOUT )
				{
				   CalloutNote *bubble=dynamic_cast<CalloutNote *> (item);
				   bubble->update();

				}
			}
		}
	}
	return;
}

/*QtBat* GroupManager::getBatLinkWithPC( QtSM* item )
{
	int gIndex = this->findGroupIndex( item );
	return this->_list[gIndex]->getBatLinkWithPC( item );
}

QtBat* GroupManager::getBatLinkWithVideo( VideoBlock *item )
{
	int gIndex = this->findGroupIndex( item );
	return this->_list[gIndex]->getBatLinkWithVideo( item );
}*/