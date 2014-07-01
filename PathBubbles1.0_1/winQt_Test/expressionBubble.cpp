#include <config.hpp>  
#include <boost/graph/adjacency_list.hpp>  

#include "textBubble2.h"
#include "ReactionBubble3.h"
#include "PathBubble2.h"
#include "GroupBubble4.h"
#include "expressionBubble.h"
#include <QTextDocument>
#include <QMap>
#define FIXEDWIDTH 250
#define FIXEDHEIGHT 250

#define EXPRESSIONBACKGROUNDCOLOR QColor(255,255,255,255) //QColor(255,237,160,255)

//#include <config.hpp>  
//#include <boost/graph/adjacency_list.hpp>  

//using namespace boost;

expressionBubble::expressionBubble( int size, int x, int y, OpenGLScene* Scene, StatisticManager* manager, OpenManager* open, QString name)
: PlotBase (size, x, y, Scene, manager)
{	 
	 fixedWidth=FIXEDHEIGHT + 12, fixedHeight=FIXEDHEIGHT + 12; //fixedWidth/1.618;
	 _layoutType=1; //horizontal  or grid

	 fixedWidth = fixedWidth;// +180;  //for legend
	 fixedHeight = fixedHeight;// +180;

	 resizeItem( fixedWidth, fixedHeight);
	
	 initWidth = fixedWidth;
	
	 expressionCleared=true;
	_scene= Scene;	
	_LABEL = false;
	_scale=2;
	dCenter=QPoint(0,0);	

	_name=name; 
	loadFile(_name); 

	static int expressionID=-1;
	expressionID++;

	m_expressionID=expressionID;
	
    m_bindPathwayID=-1;

    _mousePressed=false; pressOnBoarder = false;
    _pathBubbleParent = NULL;
	this->_open = open;
	this->_TYPE = EXPRESSIONBUBBLE;		
	this->_colorBoarder = TDHelper::getBoarderColorByType(_TYPE, m_expressionID);	
	this->_colorInner = TDHelper::getInnerColorByType(_TYPE, m_expressionID);	
	
	hoverGeneOrderID=-1;//draw text when hover is the current symbol is not shown
			    
	resetOriRect();
	//getExpressionToBePaint(bgRect, bgColor, borderColor, htColor, borderWidth, htRect, fontSize, fontColor, fontPos, fixedSize, dCenter, _scale);
}

void expressionBubble::TransferExpression(int m_bindPathwayID)
{
	geneInfo = geneInfo;
	int pid;
	if(!_scene->_pathBubbles[m_bindPathwayID]->toplevel)
	{
		pid = _scene->findTopBubble(m_bindPathwayID);	
	}
	else pid=m_bindPathwayID;
	if(pid==-1)
		pid=m_bindPathwayID;
}

expressionBubble::~expressionBubble()
{
	int Pid;	
	for(int i=0; i<_scene->_expressionBubbles.size();	i++)
	{
		if(_scene->_expressionBubbles[i])
	    if(_scene->_expressionBubbles[i]->m_expressionID!=m_expressionID)    		
		{
			//_expressionBubbles		
		}
		else
		{		  
			_scene->_expressionBubbles[i]->m_bindPathwayID=-1;
			_scene->_expressionBubbles[i]=NULL;
			Pid=i;
		}
	}
	
	//qtbox
	_lines.clear(); 
	sharedExpressionProtein.clear(); 
	expressedProtein.clear();  expressedComplex.clear();  expressedSmallMolecule.clear();  expressedDna.clear();  expressedReaction.clear();  expressedPhysicalEntity.clear();  expressedANode.clear();  expressedCompartment.clear();  expressedPathway.clear(); 	
	sharedProtein.clear();  sharedComplex.clear();  sharedSmallMolecule.clear();  sharedDna.clear();  sharedReaction.clear();  sharedPhysicalEntity.clear();  sharedANode.clear(); sharedCompartment.clear();  sharedPathway.clear(); 
	linkedProtein.clear();  linkedComplex.clear();  linkedSmallMolecule.clear();  linkedDna.clear();  linkedReaction.clear();  linkedPhysicalEntity.clear();  linkedANode.clear(); linkedCompartment.clear();  linkedPathway.clear(); 	
	differedProtein.clear();  differedComplex.clear();  differedSmallMolecule.clear();  differedDna.clear();  differedReaction.clear();  differedPhysicalEntity.clear(); differedANode.clear();  differedCompartment.clear();  differedPathway.clear(); 
	linkedEdge.clear(); 
	_itemColored.clear(); 
	foundlist.clear(); 
	//_linkage.clear(); 	
	highlightedEdge.clear(); 
	hoveredItem.clear(); 
	eventStored.clear(); 	
	sharedStored.clear();  
	differedStored.clear();  
	expressedStored.clear(); 
	highlightedStored.clear(); 
	linkedStored.clear(); 
	linkedStoredinHead.clear(); 				
	pathwayGraphStored.clear(); 	
	
	expressedGene.clear();	
	expressedProtein.clear();
	expressedComplex.clear();
	expressionCleared=true;
	
	quantityName.clear();	
	geneInfo.clear();
	quantity.clear();
	minmax.clear();
	_conhighlight;
	_reactionhighlight.clear();
	_grouphighlight.clear();
	_path2highlight.clear();
	
	sharedExpressionProtein.clear();
	sharedExpressionGene.clear();
	matchedGene_TreeRing_Expression.clear();
	expresseGeneLinkedtoTreeRing.clear();
	geneContainer.clear();
	expressedGenePos.clear();

	_complexContain.clear();
	itemSelected.clear();
	
	expressedGeneTextPos.clear();
	expressedGeneTextColumeRect.clear();
	expressedGeneOrder.clear();

	bgRect.clear();
	bgColor.clear();
	borderColor.clear();
	htColor.clear();
	fontColor.clear();
	borderWidth.clear();
	htRect.clear();
	fontSize.clear();
	fontPos.clear();
	
}

void expressionBubble::searchSharedProtein(int excludePid, bool shareflag)
{
	QList<ItemBase *> mList=_scene->getGroupMembers(this);	
	for( int i = 0; i < mList.size(); i ++)	
	{
		ItemBase *item = mList[i];
		if((item->getType()==PATHBUBBLE1 || item->getType()==SUBPATHBUBBLE1) && item->m_pathwayID != excludePid)
		{
			PathBubble1 *ptmp = dynamic_cast<PathBubble1 *>(item);			
			searchSharedProteininAPathway(ptmp->m_pathwayID);			
		}
	}	
}

void expressionBubble::searchExpressedProtein(char type, int excludePid, bool shareflag)
{
	for(set<vector<int>>::iterator it=coloredNode.begin(); it!=coloredNode.end(); it++)
	{
		vector<int> node=*it;
	    setAItemColored(node[0], node[1], node[2], _EVEN);
	}
	coloredNode.clear();
	QList<ItemBase *> mList=_scene->getGroupMembers(this);	
	for( int i = 0; i < mList.size(); i ++)	
	{
		ItemBase *item = mList[i];
		if(item->getType()==PATHBUBBLE1 || item->getType()==SUBPATHBUBBLE1)
		{
			PathBubble1 *ptmp = dynamic_cast<PathBubble1 *>(item);						
			searchExpressedGene(ptmp->m_pathwayID);	
			searchExpressedProtein(ptmp->m_pathwayID,excludePid);
		}
	}
	
	QString eName = _curFileName;
	int tindex = eName.lastIndexOf("/");
	QString temp=eName;
	temp.remove(tindex+1,temp.length());
	eName.remove(temp);	
	if(eName[eName.size()-1]=='_')
	   eName.remove(eName.size()-1,1);	

	for( int i = 0; i < mList.size(); i ++)	
	{
		ItemBase *item = mList[i];
		if(item->getType()==PATHBUBBLE1 || item->getType()==SUBPATHBUBBLE1)
		{
			PathBubble1 *ptmp = dynamic_cast<PathBubble1 *>(item);			
			//searchExpressedProtein(ptmp->m_pathwayID);
			
		//_label->setPlainText("Expression");//this->_curFileName
		
			ptmp -> setExpressionName(eName);
			if(type=='W')
			{
				if(isPathwayExpressed(ptmp->m_pathwayID))
			    {
					_scene->_pathBubbles[ptmp->m_pathwayID]->expressedPathway[m_expressionID].insert(ptmp->m_pathwayID);			
					vector<int> temp;
					temp.push_back('W');  temp.push_back(ptmp->m_pathwayID);
					_scene->_pathBubbles[ptmp->m_pathwayID]->highlighted.insert(temp);
				}
			}
			else if(type=='M')
			{
				getCompartementExpressed(ptmp->m_pathwayID);	
			}
			if(type!='P')
			{
				_scene->_pathBubbles[ptmp->m_pathwayID]->expressedProtein[m_expressionID].clear();
				_scene->_pathBubbles[ptmp->m_pathwayID]->expressedComplex[m_expressionID].clear();	
				_scene->_pathBubbles[ptmp->m_pathwayID]->expressedDna[m_expressionID].clear();	
				_scene->_pathBubbles[ptmp->m_pathwayID]->expressedSmallMolecule[m_expressionID].clear();	
				_scene->_pathBubbles[ptmp->m_pathwayID]->expressedPhysicalEntity[m_expressionID].clear();	
				_scene->_pathBubbles[ptmp->m_pathwayID]->expressedReaction[m_expressionID].clear();	
				vector<int> temp;
				temp.push_back('W');  temp.push_back(ptmp->m_pathwayID);
				_scene->_pathBubbles[ptmp->m_pathwayID]->highlighted.erase(temp);
			}
			if(!ptmp->expressedProtein.empty())
			{
				ptmp->getGraphToBePaint();				
			}
		}
	}

	if(shareflag)	
	for( int i = 0; i < mList.size(); i ++)	
	{
		ItemBase *item = mList[i];
		if(item->getType()==PATHBUBBLE1 || item->getType()==SUBPATHBUBBLE1)
		{
			PathBubble1 *ptmp = dynamic_cast<PathBubble1 *>(item);	
			if(type=='P')
			{
				for(int j=1; j<_scene->_proteinNameD[ptmp->dataID].size(); j++)
				{
					checkSharedExpressedProtein(ptmp->m_pathwayID, m_expressionID, _scene->_proteinNameD[ptmp->dataID][j][0], _scene->_proteinNameD[ptmp->dataID][j][4], type, j, excludePid);				
				}			
				for(int j=1; j<_scene->_complexNameD[ptmp->dataID].size(); j++)
				{
				    checkSharedExpressedProtein(ptmp->m_pathwayID, m_expressionID, _scene->_complexNameD[ptmp->dataID][j][0], _scene->_complexNameD[ptmp->dataID][j][2], 'C', j, excludePid);
				}
			}
			if(type=='C')
			for(int j=1; j<_scene->_complexNameD[ptmp->dataID].size(); j++)
			{
				checkSharedExpressedProtein(ptmp->m_pathwayID, m_expressionID, _scene->_complexNameD[ptmp->dataID][j][0], _scene->_complexNameD[ptmp->dataID][j][2], type, j, excludePid);
			}
			if(type=='M')
			{
				for(int j=0; j<_scene->_compartmentName[ptmp->m_pathwayID].size(); j++)
				{
					checkSharedExpressedProtein(ptmp->m_pathwayID, m_expressionID, _scene->_compartmentName[ptmp->dataID][j][0], _scene->_compartmentName[ptmp->dataID][j][0], type, j, excludePid);
				}
			}
			if(type=='W')
				checkSharedExpressedProtein(ptmp->m_pathwayID, m_expressionID, ptmp->_pathName, ptmp->_pathName, type, 0, excludePid);						
			
		}
	}
	expressionCleared=false;
}

void expressionBubble::undo()
{
	clearExpressed();
	getExpressionToBePaint();
}

void expressionBubble::redo()
{
	clearExpressed();
	getExpressionToBePaint();
}

void expressionBubble::getCompartementExpressed(int pid)
{
	
	set < vector <int> > itemSet;
	vector<int> item(2,0);
	int id;
	item[0]='P';
    for(int i=0; i<_scene->_pathBubbles[pid]->expressedProtein.size(); i++)
	{
	   for(set<int>::iterator it=_scene->_pathBubbles[pid]->expressedProtein[i].begin(); it!=_scene->_pathBubbles[pid]->expressedProtein[i].end(); it++)
	   {
		   item[1] = *it;		   
		   itemSet.insert(item);
	   }
	}

	item[0]='C';
	for(int i=0; i<_scene->_pathBubbles[pid]->expressedComplex.size(); i++)
	{
	   for(set<int>::iterator it=_scene->_pathBubbles[pid]->expressedComplex[i].begin(); it!=_scene->_pathBubbles[pid]->expressedComplex[i].end(); it++)
	   {
		   item[1] = *it;		   
		   itemSet.insert(item);
	   }
	}

	item[0]='D';
	for(int i=0; i<_scene->_pathBubbles[pid]->expressedDna.size(); i++)
	{
	   for(set<int>::iterator it=_scene->_pathBubbles[pid]->expressedDna[i].begin(); it!=_scene->_pathBubbles[pid]->expressedDna[i].end(); it++)
	   {
		   item[1] = *it;		   
		   itemSet.insert(item);
	   }
	}

	item[0]='S';
	for(int i=0; i<_scene->_pathBubbles[pid]->expressedSmallMolecule.size(); i++)
	{
	   for(set<int>::iterator it=_scene->_pathBubbles[pid]->expressedSmallMolecule[i].begin(); it!=_scene->_pathBubbles[pid]->expressedSmallMolecule[i].end(); it++)
	   {
		   item[1] = *it;		   
		   itemSet.insert(item);
	   }
	}

	item[0]='E';
	for(int i=0; i<_scene->_pathBubbles[pid]->expressedPhysicalEntity.size(); i++)
	{
	   for(set<int>::iterator it=_scene->_pathBubbles[pid]->expressedPhysicalEntity[i].begin(); it!=_scene->_pathBubbles[pid]->expressedPhysicalEntity[i].end(); it++)
	   {
		   item[1] = *it;		   
		   itemSet.insert(item);
	   }
	}

	item[0]='R';
	for(int i=0; i<_scene->_pathBubbles[pid]->expressedReaction.size(); i++)
	{
	   for(set<int>::iterator it=_scene->_pathBubbles[pid]->expressedReaction[i].begin(); it!=_scene->_pathBubbles[pid]->expressedReaction[i].end(); it++)
	   {
		   item[1] = *it;		   
		   itemSet.insert(item);
	   }
	}

	for(int m=0; m< _scene->CompartmentContain[pid].size()-1;m++)
	{	 
		for(set<vector<int>>::iterator it=itemSet.begin(); it!=itemSet.end(); it++)
		{
			item=*it;
			if(_scene->CompartmentContain[pid][m].find(item) != _scene->CompartmentContain[pid][m].end())
			{
				_scene->_pathBubbles[pid]->expressedCompartment[m_expressionID].insert(m);
			}	
		}
	}		
}


bool expressionBubble::isPathwayExpressed(int pid)
{
	for(int i=0; i<_scene->_pathBubbles[pid]->expressedProtein.size(); i++)
	{
	   if(!_scene->_pathBubbles[pid]->expressedProtein[i].empty())
		   return true;
	}
	for(int i=0; i<_scene->_pathBubbles[pid]->expressedComplex.size(); i++)
	{
	   if(!_scene->_pathBubbles[pid]->expressedComplex[i].empty())
		   return true;
	}
	for(int i=0; i<_scene->_pathBubbles[pid]->expressedDna.size(); i++)
	{
	   if(!_scene->_pathBubbles[pid]->expressedDna[i].empty())
		   return true;
	}
	for(int i=0; i<_scene->_pathBubbles[pid]->expressedPhysicalEntity.size(); i++)
	{
	   if(!_scene->_pathBubbles[pid]->expressedPhysicalEntity[i].empty())
		   return true;
	}
	for(int i=0; i<_scene->_pathBubbles[pid]->expressedSmallMolecule.size(); i++)
	{
	   if(!_scene->_pathBubbles[pid]->expressedSmallMolecule[i].empty())
		   return true;
	}
	for(int i=0; i<_scene->_pathBubbles[pid]->expressedReaction.size(); i++)
	{
	   if(!_scene->_pathBubbles[pid]->expressedReaction[i].empty())
		   return true;
	}
	return false;
}

void expressionBubble::checkExpressedProteinSize()
{
	QList<ItemBase *> mList=_scene->getGroupMembers(this);	
	for( int i = 0; i < mList.size(); i ++)	
	{
		ItemBase *item = mList[i];
		if(item->getType()==PATHBUBBLE1 || item->getType()==SUBPATHBUBBLE1)
		{
			PathBubble1 *ptmp = dynamic_cast<PathBubble1 *>(item);								
	        checkExpressedProteinSize(ptmp->m_pathwayID);	
		}
	}		
}

void expressionBubble::checkSharedProteinSize(int m_bindPathwayID)//and complex
{
	int csize;
	csize=sharedExpressionProtein.size();
	for(int i=csize; i<_scene->_pathBubbles.size(); i++)
	{ 
		set<int> record;
		sharedExpressionProtein.push_back(record);
	}
		
	csize=_scene->_pathBubbles[m_bindPathwayID]->sharedExpressionProtein.size();
	for(int i=csize; i<_scene->_expressionBubbles.size(); i++)
	{  set<int> record;
	   _scene->_pathBubbles[m_bindPathwayID]->sharedExpressionProtein.push_back(record);
	}

	csize=sharedExpressionGene.size();
	for(int i=csize; i<_scene->_pathBubbles.size(); i++)
	{  
		set<int> record;
	    sharedExpressionGene.push_back(record);
	}		
}

void expressionBubble::checkSharedExpressionProteinSize(int m_bindPathwayID)//and complex
{
	int csize;
	csize=sharedExpressionProtein.size();
	for(int i=csize; i<_scene->_pathBubbles.size(); i++)
	{ 
		set<int> record;
		sharedExpressionProtein.push_back(record);
	}
	
	csize=_scene->_pathBubbles[m_bindPathwayID]->sharedExpressionProtein.size();
	for(int i=csize; i<_scene->_expressionBubbles.size(); i++)
	{  
		set<int> record;
	   _scene->_pathBubbles[m_bindPathwayID]->sharedExpressionProtein.push_back(record);
	}
}

void expressionBubble::checkExpressedProteinSize(int m_bindPathwayID)//and complex
{
	int csize;
	csize=expressedProtein.size();
	for(int i=csize; i<_scene->_pathBubbles.size(); i++)
	{ 
		set<int> record;
		expressedProtein.push_back(record);
	}
		
	csize=expressedComplex.size();
	for(int i=csize; i<_scene->_pathBubbles.size(); i++)
	{  
		set<int> record;
	    expressedComplex.push_back(record);
	}
	
	csize=expressedGene.size();
	for(int i=csize; i<_scene->_pathBubbles.size(); i++)
	{  
		set<int> record;
	    expressedGene.push_back(record);
	}	
	
	csize=_scene->_pathBubbles[m_bindPathwayID]->expressedProtein.size();
	for(int i=csize; i<_scene->_expressionBubbles.size(); i++)
	{  set<int> record;
	   _scene->_pathBubbles[m_bindPathwayID]->expressedProtein.push_back(record);
	}
	
	csize=_scene->_pathBubbles[m_bindPathwayID]->expressedComplex.size();	
	for(int i=csize; i<_scene->_expressionBubbles.size(); i++)
	{  
	   set<int> record;	   
	  _scene->_pathBubbles[m_bindPathwayID]->expressedComplex.push_back(record);
	}
	
	csize=_scene->_pathBubbles[m_bindPathwayID]->expressedDna.size();	
	for(int i=csize; i<_scene->_expressionBubbles.size(); i++)
	{  
	   set<int> record;	   
	  _scene->_pathBubbles[m_bindPathwayID]->expressedDna.push_back(record);
	}
	
	csize=_scene->_pathBubbles[m_bindPathwayID]->expressedSmallMolecule.size();	
	for(int i=csize; i<_scene->_expressionBubbles.size(); i++)
	{  
	   set<int> record;	   
	  _scene->_pathBubbles[m_bindPathwayID]->expressedSmallMolecule.push_back(record);
	}
	
	csize=_scene->_pathBubbles[m_bindPathwayID]->expressedPhysicalEntity.size();	
	for(int i=csize; i<_scene->_expressionBubbles.size(); i++)
	{  
	   set<int> record;	   
	  _scene->_pathBubbles[m_bindPathwayID]->expressedPhysicalEntity.push_back(record);
	}
	
	csize=_scene->_pathBubbles[m_bindPathwayID]->expressedReaction.size();	
	for(int i=csize; i<_scene->_expressionBubbles.size(); i++)
	{  
	   set<int> record;	   
	  _scene->_pathBubbles[m_bindPathwayID]->expressedReaction.push_back(record);
	}
	
	csize=_scene->_pathBubbles[m_bindPathwayID]->expressedPathway.size();	
	for(int i=csize; i<_scene->_expressionBubbles.size(); i++)
	{  
	   set<int> record;	   
	  _scene->_pathBubbles[m_bindPathwayID]->expressedPathway.push_back(record);
	}
	
	csize=_scene->_pathBubbles[m_bindPathwayID]->expressedCompartment.size();	
	for(int i=csize; i<_scene->_expressionBubbles.size(); i++)
	{  
	   set<int> record;	   
	  _scene->_pathBubbles[m_bindPathwayID]->expressedCompartment.push_back(record);
	}
	geneContainer.resize(geneInfo.size());
}

void expressionBubble::checkSharedExpressedProtein(int Pid, int Eid, QString n1, QString n2, int type, int j, int excludePid)
{
    if(!isSharedGene(type, n1, n2, excludePid) ) //keqin
	{
		if(type=='P')
	       _scene->_pathBubbles[Pid]->expressedProtein[Eid].erase(j);		
		else if(type=='C')
	       _scene->_pathBubbles[Pid]->expressedComplex[Eid].erase(j);		
		else if(type=='M')
	       _scene->_pathBubbles[Pid]->expressedCompartment[Eid].erase(j);		
		else if(type=='W')
	       _scene->_pathBubbles[Pid]->expressedPathway[Eid].erase(j);		
	}
}


void expressionBubble::searchExpressedProtein(int m_bindPathwayID, int excludePid, bool shareflag)
//shareflag: search only shared expressed gene in all the pathbubbles in the group, the default is true
{   	
	vector<int> node(3,0);
	checkExpressedProteinSize(m_bindPathwayID);		
	_scene->_pathBubbles[m_bindPathwayID]->expressedProtein[m_expressionID].clear();	
	_scene->_pathBubbles[m_bindPathwayID]->expressedComplex[m_expressionID].clear();	
	_scene->_pathBubbles[m_bindPathwayID]->expressedDna[m_expressionID].clear();			
	_scene->_pathBubbles[m_bindPathwayID]->expressedSmallMolecule[m_expressionID].clear();	
	_scene->_pathBubbles[m_bindPathwayID]->expressedPhysicalEntity[m_expressionID].clear();		
	_scene->_pathBubbles[m_bindPathwayID]->expressedReaction[m_expressionID].clear();	

	for(int j=0; j<_scene->_proteinNameD[_scene->_pathBubbles[m_bindPathwayID]->dataID].size(); j++)
	{
	
		if(_scene->_pathBubbles[m_bindPathwayID]->proteinPos[j].x()>-999.99)
		{
			for(int i=0; i< geneInfo.size(); i++)
			{
				if(_scene->_proteinNameD[_scene->_pathBubbles[m_bindPathwayID]->dataID][j][0].contains(geneInfo[i][1])||_scene->_proteinNameD[_scene->_pathBubbles[m_bindPathwayID]->dataID][j][4].contains(geneInfo[i][1]))
				{
				     //expressedProtein[m_bindPathwayID].insert(j);	
					 //geneContainer[i].push_back('P'); geneContainer[i].push_back(j);
					 //if(!shareflag || isSharedGene('P', j, excludePid) )
					 {					 
						 /*if(quantity[i][0]!=0 || quantity[i][1]!=0)
						 {
							 //expressedGene[m_bindPathwayID].insert(i); //keqin changed 1/6/2014
							 _scene->_pathBubbles[m_bindPathwayID]->expressedProtein[m_expressionID].insert(j);						
						 }*/
						 if(quantity[i][0]!=0)
						 {
							  if(quantity[i][1]>quantity[i][0] && quantity[i][1]/quantity[i][0]>=2.0)
							  {
								 node[0]=m_bindPathwayID, node[1]='P', node[2]=j;
								 coloredNode.insert(node);
								 setAItemColored(m_bindPathwayID, 'P', j, GO_UP);
								 _scene->_pathBubbles[m_bindPathwayID]->expressedProtein[m_expressionID].insert(j);		
							  }
							  else if(quantity[i][1]<quantity[i][0] && (quantity[i][0]-quantity[i][1])/quantity[i][0]>=0.5)
							  {
									node[0]=m_bindPathwayID, node[1]='P', node[2]=j;
									coloredNode.insert(node);
									setAItemColored(m_bindPathwayID, 'P', j, _DOWN);
									_scene->_pathBubbles[m_bindPathwayID]->expressedProtein[m_expressionID].insert(j);		
							  }
						 }	
						 else if(quantity[i][1]>0)
						 {
							 node[0]=m_bindPathwayID, node[1]='P', node[2]=j;
							 coloredNode.insert(node);
							 setAItemColored(m_bindPathwayID, 'P', j, GO_UP);	
							 _scene->_pathBubbles[m_bindPathwayID]->expressedProtein[m_expressionID].insert(j);		
						 }
					 }
				}
			}			
		}			
	}		
	int pid = _scene->_pathBubbles[m_bindPathwayID]->dataID_1; //_scene->findTopBubble(m_bindPathwayID);	
	/* for(int j=0; j<_scene->_complexContain[m_bindPathwayID].size(); j++) //keqin come back later after store complex contains
	{
		
		if(_scene->_pathBubbles[m_bindPathwayID]->complexPos[j].x()>-999.99)
		{
			   ector<int> component;
				for(int k=0; k<_scene->_complexContain[m_bindPathwayID][j].size(); k=k+2)
				{
					bool flag=0;
					for(int i=0; i< geneInfo.size(); i++)
					{
						 int type=_scene->_complexContain[m_bindPathwayID][j][k], id=_scene->_complexContain[m_bindPathwayID][j][k+1];
						 vector<QString> name;	 	
						 switch(type)
						 {
							 case 'C': name = _scene->_complexNameD[pid][id];        break;
							 case 'D': name = _scene->_DnaNameD[pid][id];            break;
							 case 'E': name = _scene->_physicalEntityNameD[pid][id]; break; 
							 case 'P': name = _scene->_proteinNameD[pid][id];        break;
							 case 'S': name = _scene->_smallMoleculeNameD[pid][id];  break;			
						 }							 
						 if(name[0].contains(geneInfo[i][1])||(name.size()>=2&&name[1].contains(geneInfo[i][1])))
						 {
							 //expressedComplex[m_bindPathwayID].insert(j);								 
							 //geneContainer[i].push_back('C'); geneContainer[i].push_back(j);
							 //expressedGene[m_bindPathwayID].insert(i);
						     //if(!shareflag || isSharedGene('C', j, excludePid) )
							 {
								 if(quantity[i][0]!=0)
								 {
									   if(quantity[i][1]>quantity[i][0] && quantity[i][1]/quantity[i][0]>=2.0)
									   {
										   component.push_back(GO_UP);								  									  
										   _scene->_pathBubbles[m_bindPathwayID]->expressedComplex[m_expressionID].insert(j);		
									   }
									   else if(quantity[i][1]<quantity[i][0] && (quantity[i][0]-quantity[i][1])/quantity[i][0]>=0.5)								  		 
									   { 
										   component.push_back(_DOWN);
										   _scene->_pathBubbles[m_bindPathwayID]->expressedComplex[m_expressionID].insert(j);		
									   }
								 }	
								 else if(quantity[i][1]>0)								 
									 component.push_back(GO_UP);
								 else component.push_back(_EVEN);
							 }
						}
						if(flag)
						   break;
					}
					if(!flag)
						component.push_back(_EVEN);
				}	
				
				int downnum,upnum,evennum;
				downnum=upnum=evennum=0;
				for(int i=0; i<component.size(); i++)
				{
					if(component[i]==_DOWN)
					{
						downnum++;
					}
					else if(component[i]==GO_UP)
					{
					    upnum++;
					}
					else if(component[i]==_EVEN)
					{
					    evennum++;
					}
				}

				if(downnum!=0)
				 {
					  node[0]=m_bindPathwayID, node[1]='C', node[2]=j;
					 coloredNode.insert(node);
					 setAItemColored(m_bindPathwayID, 'C', j, _DOWN);
				}
				else if(evennum==component.size())
					; 
				else
				 {
					  node[0]=m_bindPathwayID, node[1]='C', node[2]=j;
					coloredNode.insert(node);
					 setAItemColored(m_bindPathwayID, 'C', j, GO_UP);
				}
		}			
	}	*/
}

bool expressionBubble::isSharedGene(int type, QString n1, QString n2, int excludePid)
{
	bool flag=true;
	if(type=='W')
		return true;

	QList<ItemBase *> mList=_scene->getGroupMembers(this);	
	bool useIt=true,haveIt=false;	
	int count=0;
	for( int i = 0; i < mList.size(); i ++)	
	{
	    ItemBase *item = mList[i];
		if(item->getType()==PATHBUBBLE1 || item->getType()==SUBPATHBUBBLE1)
		{
			int pid=item->m_pathwayID;
			count++;
			if(excludePid==pid)
				haveIt=true;
		}	
	}
	if(count==1 && haveIt)
	{
		useIt=false;
	    return false;	
	}

	vector<int> ebList;
	if(type=='M')
	{
		for( int i = 0; i < mList.size(); i ++)	
		{
			ItemBase *item = mList[i];
			set<QString> nameList;
			if(item->getType()==EXPRESSIONBUBBLE)
			{
				ebList.push_back(item->m_expressionID);
			}
		}
	}

	for( int i = 0; i < mList.size(); i ++)	
	{
		ItemBase *item = mList[i];
		set<QString> nameList;
		if(item->getType()==PATHBUBBLE1 || item->getType()==SUBPATHBUBBLE1)
		{
			int pid=item->m_pathwayID;
			if(type=='P')
			{
				for(int j=1; j<_scene->_proteinNameD[_scene->_pathBubbles[pid]->dataID].size(); j++)
				{
					nameList.insert(_scene->_proteinNameD[_scene->_pathBubbles[pid]->dataID][j][0]);
					nameList.insert(_scene->_proteinNameD[_scene->_pathBubbles[pid]->dataID][j][4]);
				}
			}
			else if(type=='C')
			{
				for(int j=1; j<_scene->_complexNameD[_scene->_pathBubbles[pid]->dataID].size(); j++)
				{
					nameList.insert(_scene->_complexNameD[_scene->_pathBubbles[pid]->dataID][j][0]);
					nameList.insert(_scene->_complexNameD[_scene->_pathBubbles[pid]->dataID][j][2]);
				}
			}
			else if(type=='M')
			{
				for(int j=0; j<ebList.size(); j++)
				{
					int eid=ebList[j];
					for(set<int>::iterator it=_scene->_pathBubbles[pid]->expressedCompartment[eid].begin(); it!=_scene->_pathBubbles[pid]->expressedCompartment[eid].end(); it++)
					{
						int cid=*it;
						nameList.insert(_scene->_compartmentName[pid][cid][0]);					
					}
				}
			}
			/*else if(type=='W')
			{
				nameList.insert(_scene->_pathBubbles[pid]->_pathName);		
			}*/
			if(nameList.empty() && (excludePid!=pid && useIt) )
			{
				flag=false;   break;
			}
			else if(nameList.find(n1)==nameList.end() && nameList.find(n2)==nameList.end() && (excludePid!=pid && useIt) )
			{
				flag=false;   break;
			}	
		}
	}
	return flag;
}

void expressionBubble::searchExpressedGene(int m_bindPathwayID)
{  
	//only search expressed gene in expression data	
	checkExpressedProteinSize(m_bindPathwayID);		
	/*_scene->_pathBubbles[m_bindPathwayID]->expressedProtein[m_expressionID].clear();	
	_scene->_pathBubbles[m_bindPathwayID]->expressedComplex[m_expressionID].clear();	
	_scene->_pathBubbles[m_bindPathwayID]->expressedDna[m_expressionID].clear();			
	_scene->_pathBubbles[m_bindPathwayID]->expressedSmallMolecule[m_expressionID].clear();	
	_scene->_pathBubbles[m_bindPathwayID]->expressedPhysicalEntity[m_expressionID].clear();		
	_scene->_pathBubbles[m_bindPathwayID]->expressedReaction[m_expressionID].clear();	*/
	
	for(int j=0; j<_scene->_proteinNameD[_scene->_pathBubbles[m_bindPathwayID]->dataID].size(); j++)
	{
		//if(_scene->_proteinNameD[_scene->_pathBubbles[m_bindPathwayID]->dataID][j][0].contains("SMAD4") || _scene->_proteinNameD[_scene->_pathBubbles[m_bindPathwayID]->dataID][j][1].contains("SMAD4"))
		//	j=j;
		if(_scene->_pathBubbles[m_bindPathwayID]->proteinPos[j].x()>-999.99)
		{
			for(int i=0; i< geneInfo.size(); i++)
			{
				if(_scene->_proteinNameD[_scene->_pathBubbles[m_bindPathwayID]->dataID][j][0].contains(geneInfo[i][1])||_scene->_proteinNameD[_scene->_pathBubbles[m_bindPathwayID]->dataID][j][4].contains(geneInfo[i][1]))
				{
				     expressedProtein[m_bindPathwayID].insert(j);	
					 geneContainer[i].push_back('P'); geneContainer[i].push_back(j);					 
					 /*if(quantity[i][0]!=0 || quantity[i][1]!=0)
					 {
						 //expressedGene[m_bindPathwayID].insert(i); 
						 //_scene->_pathBubbles[m_bindPathwayID]->expressedProtein[m_expressionID].insert(j);						
					 }*/
					 if(quantity[i][0]!=0)
					 {
						  if(quantity[i][1]>quantity[i][0] && quantity[i][1]/quantity[i][0]>=2.0)
						  {
						      expressedGene[m_bindPathwayID].insert(i);
						  }
		                  else if(quantity[i][1]<quantity[i][0] && (quantity[i][0]-quantity[i][1])/quantity[i][0]>=0.5)
						  {
						      expressedGene[m_bindPathwayID].insert(i);
						  }
					 }	
					 else if(quantity[i][1]>0)
                         setAItemColored(m_bindPathwayID, 'P', j, GO_UP);			     	 
					 
				}
			}			
		}			
	}		
	int pid = _scene->_pathBubbles[m_bindPathwayID]->dataID_1; //_scene->findTopBubble(m_bindPathwayID);	
	 //come back later after store complex contains
	/*
	for(int j=0; j<_scene->_complexContain[m_bindPathwayID].size(); j++)
	{
		if(_scene->_pathBubbles[m_bindPathwayID]->complexPos[j].x()>-999.99)
		{
			    vector<int> component;
				for(int k=0; k<_scene->_complexContain[m_bindPathwayID][j].size(); k=k+2)
				{
					bool flag=0;
					for(int i=0; i< geneInfo.size(); i++)
					{
						 int type=_scene->_complexContain[m_bindPathwayID][j][k], id=_scene->_complexContain[m_bindPathwayID][j][k+1];
						 vector<QString> name;	 	
						 switch(type)
						 {
							 case 'C': name = _scene->_complexNameD[pid][id];        break;
							 case 'D': name = _scene->_DnaNameD[pid][id];            break;
							 case 'E': name = _scene->_physicalEntityNameD[pid][id]; break; 
							 case 'P': name = _scene->_proteinNameD[pid][id];        break;
							 case 'S': name = _scene->_smallMoleculeNameD[pid][id];  break;			
						 }							 
						 if(name[0].contains(geneInfo[i][1])||(name.size()>=2&&name[1].contains(geneInfo[i][1])))
						 {
							 expressedComplex[m_bindPathwayID].insert(j);	
							 //geneContainer[i].clear();
							 geneContainer[i].push_back('C'); geneContainer[i].push_back(j);
							 //expressedGene[m_bindPathwayID].insert(i);
							 if(quantity[i][0]!=0 || quantity[i][1]!=0)
							 {
								 flag=1;
								 //_scene->_pathBubbles[m_bindPathwayID]->expressedComplex[m_expressionID].insert(j);							
							 }
							 if(quantity[i][0]!=0)
							 {
								  if(quantity[i][1]>quantity[i][0] && quantity[i][1]/quantity[i][0]>=2.0)
								  {
									  expressedGene[m_bindPathwayID].insert(i);
								  }
									  //component.push_back(GO_UP);								  									  
								  else if(quantity[i][1]<quantity[i][0] && (quantity[i][0]-quantity[i][1])/quantity[i][0]>=0.5)						  		 
								  {
									 //component.push_back(_DOWN);
									  expressedGene[m_bindPathwayID].insert(i);
								  }
							 }	
							 else if(quantity[i][1]>0)								 
								 component.push_back(GO_UP);
							 else component.push_back(_EVEN);
						}
						if(flag)
						   break;
					}
					if(!flag)
						component.push_back(_EVEN);
				}	
				int downnum,upnum,evennum;
				downnum=upnum=evennum=0;
				for(int i=0; i<component.size(); i++)
				{
					if(component[i]==_DOWN)
					{
						downnum++;
					}
					else if(component[i]==GO_UP)
					{
					    upnum++;
					}
					else if(component[i]==_EVEN)
					{
					    evennum++;
					}
				}
				
		}		
		
	}	*/
}

void expressionBubble::searchSharedProteininAPathway(int m_bindPathwayID)//and complex
{   	
	checkSharedProteinSize(m_bindPathwayID);			
	for(int j=1; j<_scene->_proteinNameD[_scene->_pathBubbles[m_bindPathwayID]->dataID].size(); j++)
	{
		//vector<QString> name = _scene->_proteinNameD[_scene->_pathBubbles[m_bindPathwayID]->dataID][j];
		//if(_scene->_pathBubbles[m_bindPathwayID]->proteinPos[j].x()>-999.99)
		{
			for(int i=0; i< geneInfo.size(); i++)
			{
				if(_scene->_proteinNameD[_scene->_pathBubbles[m_bindPathwayID]->dataID][j][0].contains(geneInfo[i][1])||_scene->_proteinNameD[_scene->_pathBubbles[m_bindPathwayID]->dataID][j][4].contains(geneInfo[i][1]))
				{
				     sharedExpressionProtein[m_bindPathwayID].insert(j);						 
					 sharedExpressionGene[m_bindPathwayID].insert(i);					 					 
				}
			}			
		}			
	}			
}


void expressionBubble::deleteItems()
{
	/*complexPos.clear();
	proteinPos.clear();	
    smallMoleculePos.clear();	
    DnaPos.clear();	
    reactionPos.clear();	
    physicalEntityPos.clear();	
	edge.clear();*/
}

void expressionBubble::loadFile(const QString &fileName)
{
	setCurrentFile(fileName);	
}

void expressionBubble::setCurrentFile(const QString &fileName)
{
	_curFileName = fileName;
	_curFilePath = _curFileName;
	int tindex = _curFilePath.lastIndexOf("\\");
	_curFilePath.remove(tindex+1, _curFilePath.length());
}

void expressionBubble::testforceDirected(QPainter *painter, QRectF Compartment)
{	
	float width=Compartment.width(); //inside right
	float height=Compartment.height(); 
	float x=Compartment.x(),y=Compartment.y();

    int iterations = 100;
	vector<QPoint> edges;
	vector<int> tg(2,0);
	

	/*Graph g(8);
 
    //digraph g(8);

   // add some edges
   add_edge(0, 1, g);   edges.push_back(QPoint(0,1));
   //add_edge(1, 3, g);  edges.push_back(QPoint(1,3));
   add_edge(4, 2, g); edges.push_back(QPoint(4,2));
   add_edge(2, 3, g); edges.push_back(QPoint(2, 3));
   add_edge(2, 1, g); edges.push_back(QPoint(2, 1)); 
   //add_edge(4, 3, g); edges.push_back(QPoint(4, 3));
   //add_edge(5, 1, g); edges.push_back(QPoint(5, 1));
   add_edge(6, 3, g);edges.push_back(QPoint(6, 3));
   //add_edge(3, 4, g); edges.push_back(QPoint(3, 4));
   add_edge(5, 2, g); edges.push_back(QPoint(5, 2));
   //add_edge(6, 4, g);edges.push_back(QPoint(6, 4));
   add_edge(7, 4, g);edges.push_back(QPoint(7, 4));
   add_edge(7, 2, g);edges.push_back(QPoint(7, 2));
   

  typedef std::vector<point_type> PositionVec;
  PositionVec position_vec(num_vertices(g));
  typedef iterator_property_map<PositionVec::iterator, 
                                property_map<Graph, vertex_index_t>::type>
  PositionMap;
  PositionMap position(position_vec.begin(), get(vertex_index, g));

  minstd_rand gen;
  topology_type topo(gen, -width/2, -height/2, width/2, height/2);
  random_graph_layout(g, position, topo);
  //kamada_kawai_spring_layout(g, position, topo);
  fruchterman_reingold_force_directed_layout
    (g, position, topo,
     cooling(progress_cooling(iterations)));

  graph_traits<Graph>::vertex_iterator vi, vi_end;
  for (boost::tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi) {
    
	QRectF node;//=complexPos[*vi];
	node.setX(position[*vi][0]);
	node.setY(position[*vi][1]);
	node.setWidth(4);
	node.setHeight(4);
	painter->drawRect(node);
  }
  for(int i=0; i<edges.size(); i++)
  {
      painter->drawLine(QPointF(position[edges[i].x()][0],position[edges[i].x()][1]),QPointF(position[edges[i].y()][0],position[edges[i].y()][1]));  
  }*/
  
}

void expressionBubble::testforceDirected_2(QPainter *painter, QRectF Compartment)
{
	/*
	
	float width=Compartment.width(); //inside right
	float height=Compartment.height(); 
	//float x=Compartment.x(),y=Compartment.y();


	QPointF start,middle,end;
	int type, id;
	int sid, mid, eid;

	//float x=Compartment.x(),y=Compartment.y();
    int iterations = 100;
	vector<int> nodeid(complexNum + proteinNum + smallMoleculeNum + DnaNum + reactionNum + physicalEntityNum + 6,-1); 
	set<vector<int>> check; vector<int> order;
	// int iterations = 100;
	vector<QPoint> edges;
	//Graph g(complexNum + proteinNum + smallMoleculeNum + DnaNum + reactionNum + physicalEntityNum + 6); 
    //digraph g(8);
	//assign new index
	int count,stop;
	for(int i=0; i<edge.size(); i++)
	{
		 vector<int> temp(2,-1);
         //painter->drawRect(reactionPos[i].x(),reactionPos[i].y(),2,2);
		 type=edge[i][0], id=edge[i][1];		
		 temp[0]=type,  temp[1]=id;
		 if(check.find(temp)==check.end())
		 {   
			 count=check.size();
		     check.insert(temp);
		     sid=getVerticeID(type, id);		 
		     nodeid[sid]=count;
			 order.push_back(sid);
		 }

		 type=edge[i][4];	 id=edge[i][5];
		 temp[0]=type,  temp[1]=id;
		 if(check.find(temp)==check.end())
		 {   
			 count=check.size();
		     check.insert(temp);
		     mid=getVerticeID(type, id);
		     nodeid[mid]=count;
			 order.push_back(mid);
		 }		 

		 type=edge[i][2], id=edge[i][3];		 		 
		 temp[0]=type,  temp[1]=id;
		  if(check.find(temp)==check.end())
		 {   
			 count=check.size();
		     check.insert(temp);
		     eid=getVerticeID(type, id);
		     nodeid[eid]=count;		
			 order.push_back(eid);
		 }	
		 if(check.size()>=10)
		 { 
			  stop=i;  
			  break; 
		 }
	}
	Graph g(check.size());
    //add some edges


	for(int i=0; i<edge.size(); i++)
	{
         //painter->drawRect(reactionPos[i].x(),reactionPos[i].y(),2,2);
		 type=edge[i][0], id=edge[i][1];		 
		 sid=getVerticeID(type, id);

		 type=edge[i][4]; id=edge[i][5];		 
		 mid=getVerticeID(type, id);

         type=edge[i][2], id=edge[i][3];		 
		 eid=getVerticeID(type, id);

		 //add_edge(nodeid[sid], nodeid[mid], g); edges.push_back(QPoint(nodeid[sid], nodeid[mid]));
		 //add_edge(nodeid[mid], nodeid[eid], g); edges.push_back(QPoint(nodeid[mid], nodeid[eid]));

		 if(stop==i)
			break; 
	}
   
  
	  
   add_edge(0, 1, g);   edges.push_back(QPoint(0,1));
   add_edge(1, 3, g);  edges.push_back(QPoint(1,3));
   add_edge(4, 2, g); edges.push_back(QPoint(4,2));
   add_edge(2, 3, g); edges.push_back(QPoint(2, 3));
   add_edge(2, 1, g); edges.push_back(QPoint(2, 1)); 
   add_edge(4, 3, g); edges.push_back(QPoint(4, 3));
   add_edge(5, 1, g); edges.push_back(QPoint(5, 1));
   add_edge(6, 3, g);edges.push_back(QPoint(6, 3));
   add_edge(3, 4, g); edges.push_back(QPoint(3, 4));
   add_edge(5, 2, g); edges.push_back(QPoint(5, 2));
   add_edge(6, 4, g);edges.push_back(QPoint(6, 4));
   add_edge(7, 4, g);edges.push_back(QPoint(7, 4));
   add_edge(7, 2, g);edges.push_back(QPoint(7, 2));

  

  typedef std::vector<point_type> PositionVec;
  PositionVec position_vec(num_vertices(g));
  typedef iterator_property_map<PositionVec::iterator, 
                                property_map<Graph, vertex_index_t>::type>
  PositionMap;
  PositionMap position(position_vec.begin(), get(vertex_index, g));

  minstd_rand gen;
  topology_type topo(gen, -width/2, -height/2, width/2, height/2);
  random_graph_layout(g, position, topo);
  //kamada_kawai_spring_layout(g, position, topo);
  fruchterman_reingold_force_directed_layout
    (g, position, topo,
     cooling(progress_cooling(iterations)));

  graph_traits<Graph>::vertex_iterator vi, vi_end;
  for (boost::tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi) {
    
	QRectF node;
	
	
	node.setX(position[*vi][0]);
	node.setY(position[*vi][1]);
	node.setWidth(4);
	node.setHeight(4);

	

	 painter->drawRect(node);
  }

  
  for(int i=0; i<edges.size(); i++)
  {
      painter->drawLine(QPointF(position[edges[i].x()][0],position[edges[i].x()][1]),QPointF(position[edges[i].y()][0],position[edges[i].y()][1]));  
  }
  */
}



void expressionBubble::forceDirected()
{	


}



vector<vector<vector<int>>> expressionBubble::recordEdge(vector<vector<vector<int>>> _biochemicalReaction)
{
	vector<vector<vector<int>>> edge;

    /*
	 start[0]=edge[i][0], start[1]=edge[i][1];	
		 		 
		 middle[0]=edge[i][4]; middle[1]=edge[i][5]; 
		 
		 end[0]=edge[i][2], end[1]=edge[i][3]; 
	*/

	return edge;
}

void expressionBubble::getNameforConverted(vector<vector<int>> _8convertedProtein, vector<vector<int>> _8convertedComplex, vector<vector<int>> _8convertedEntity, vector<QString> &_complexName, vector<vector<QString>> &_proteinName, vector<vector<QString>> &_physicalEntityName)
{   //only keep the last one
	//void &_complexName, vector<vector<QString>> &_proteinName, vector<vector<QString>> &_physicalEntityName
	//_proteinName
	QString at('/');

	for(int i=0;i<_8convertedProtein.size();i++)
	{
		QString temp,temp1;
		
		if(!_8convertedProtein[i].empty())
		{
			for(int j=_8convertedProtein[i].size()-2; j<_8convertedProtein[i].size(); j=j+2)
			{
				int type=_8convertedProtein[i][j], id=_8convertedProtein[i][j+1];		    
		        if(type=='P')
		        {
                  if(!temp.isEmpty()&&!_proteinName[id][0].isEmpty())
				     temp.append(at);
				  
				  temp.append(_proteinName[id][0]); temp1=_proteinName[id][1];
				  if(temp.size()==0)
				  { 
					 while(!_8convertedProtein[id].empty())
					 {						
						type=_8convertedProtein[id][0], id=_8convertedProtein[id][1];
						if(type=='P')
						{
							if(!temp.isEmpty()&&!_proteinName[id][0].isEmpty())
				                      temp.append(at);
				  	       temp.append(_proteinName[id][0]); temp1=_proteinName[id][1];
						   if(temp.size()!=0)
							   break;
						}
					 }
				  }
		        }
		        else
		        {
				   //protein is converted to complex?? (is it possible?)		
		        }			    
			}	
			_proteinName[i][0].append(temp); _proteinName[i][1]=temp1;
		}
	}	
		
	for(int i=0;i<_8convertedComplex.size();i++)
	{
		QString temp;
		if(!_8convertedComplex[i].empty())
		{
			for(int j=0; j<_8convertedComplex[i].size(); j=j+2)
			{
				int type=_8convertedComplex[i][j], id=_8convertedComplex[i][j+1];
		    
				if(type=='C')
				{
				
				if(!temp.isEmpty()&&!_complexName[id].isEmpty())
				         temp.append(at);
				
				temp.append(_complexName[id]); 
				if(temp.size()==0)
				{
					while(!_8convertedComplex[id].empty())
					{						
						type=_8convertedComplex[id][0], id=_8convertedComplex[id][1];
						if(type=='C')
						{
							if(!temp.isEmpty()&&!_complexName[id].isEmpty())
				                  temp.append(at);
						   temp.append(_complexName[id]); 
						   if(temp.size()!=0)
							   break;
						}
					}
				}
				}
				else
				{
				//complex is converted to protein?? (is it possible?)		
				}
			}
			_complexName[i].append(temp); 
		}
		
	}

	QString a;
	vector<QString> empty(2,a);
	vector<vector<QString>> temps(physicalEntityNum+1,empty);
	_physicalEntityName = temps;

	for(int i=0;i<_8convertedEntity.size();i++)
	{
		QString temp,temp1;
		if(i>=4)
			i=i;
		if(!_8convertedEntity[i].empty())
		{
			for(int j=0; j<_8convertedEntity[i].size(); j=j+2)
			{
				int type=_8convertedEntity[i][j], id=_8convertedEntity[i][j+1];
		    
				if(type=='P')
				{
					if(!temp.isEmpty()&&!_proteinName[id][0].isEmpty())
				            temp.append(at);
					temp.append(_proteinName[id][0]); temp1=_proteinName[id][1];
					if(temp.size()==0)
					{
						while(!_8convertedProtein[id].empty())
						{						
							type=_8convertedProtein[id][0], id=_8convertedProtein[id][1];
							if(type=='P')
							{
								if(!temp.isEmpty()&&!_proteinName[id][0].isEmpty())
				                      temp.append(at);
								temp.append(_proteinName[id][0]); temp1=_proteinName[id][1];
								if(temp.size()!=0)
									break;
							}
						}
					}
				}			
				else
				{
				
				}
			}
			_physicalEntityName[i][0].append(temp);  _physicalEntityName[i][1]=temp1; 
		}
		
	}

}

void expressionBubble::deleteSelectedItems(QGraphicsScene *scene)
{
	if (!scene)
		return;

	if(_scene->delectionStarter == this)
		return;

	_penisOn=false;	
	_grabisOn=false;	
	_queryisOn=false;
	_upisOn=_downisOn=_evenisOn=false;
	_lines.clear();
	logRemove();

	deleteSelectedItemKids(scene);
	
	ItemBase::deleteSelectedItems(scene);	
	
}

void expressionBubble::deleteSelectedItemKids(QGraphicsScene *scene)
{
	//no kids for expresssion bubble
	/*
	int size;
	size=_codeBubbles.size();
	while(size>0)
	{		
		_codeBubbles.last()->deleteSelectedItemsbyParent(scene);
		//_codeBubbles[i]->deleteSelectedItems(scene);
		size--;
	}
	//odeBubbles.clear();
	//_conPoint.clear();

	size=_reactionBubbles.size();

	//for (int i = 0; i < _reactionBubbles.size(); i ++)
	while(size>0)
	{		
		_reactionBubbles.last()->deleteSelectedItemsbyParent(scene);
	    size--;
	}
	//_reactionBubbles.clear();
	//_reactionPoint.clear();

	size=_groupBubbles.size();	
	while(size>0)
	//for (int i = 0; i < _groupBubbles.size(); i ++)
	{		
		_groupBubbles.last()->deleteSelectedItemsbyParent(scene);		
	    size--;
	}*/	
}

void expressionBubble::updateLabel()
{
	int tindex;	
	QString temp;
	static int mark=-1;	
	if(mark!=m_expressionID)
	{
		tindex = _curFileName.lastIndexOf("/");
		temp=_curFileName;
		temp.remove(tindex+1,_curFileName.length());
		this->_curFileName.remove(temp);	
		if (_LABEL == false)
		{
			_label = new ItemLabel( this, _scene, m_size_width, m_size_height );
			_label->setLabelColor(_colorBoarder);
			this->_scene->addItem(_label);
			_LABEL = true;
			return;
		}
		//_label->setPlainText("Expression");//this->_curFileName
		_label->setPlainText(this->_curFileName);
		_label->updateLabel( m_size_width, m_size_height );
		//_label->setPos( _label->pos() + QPointF(0, 10) );		
		mark=m_expressionID;
	}	
	if(!_label->isVisible())    
		_label->setVisible(true);
} 


void expressionBubble::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{	
	updateLabel();
	QString eName = getCurrentFilename();
	int tindex = eName.lastIndexOf("/");
	QString eName1=eName;
	eName1 = eName1.remove(tindex+1, eName.length());

	int tindex1 = eName.lastIndexOf("//");
	QString eName2=eName;
	eName2 = eName2.remove(tindex1+1, eName.length());

	int tindex2 = eName.lastIndexOf("\\");
	QString eName3=eName;
	eName3 = eName3.remove(tindex2+1, eName.length());


	QPointF start,middle,end;
	vector<int> temp(2,0);
	PlotBase::paint( painter, option, widget );
	float width,height;

	set<vector<int> > reactionRecord; 
	vector<int> node(2,0);
	vector<vector<int>> ed(2, node);

	QRegion reg=getClipRegionforCalloutNote();
	painter->setClipRegion(reg);

	painter->setRenderHint(QPainter::Antialiasing, true);
	//QList<ItemBase *> mList=_scene->getGroupMembers(this);
	
	paintExpression(painter, bgRect, bgColor, borderColor, htColor, borderWidth, htRect, fontSize, fontPos);

	drawHoveredSymbol(painter, hoverGeneOrderID);

	painter->setClipping(false);
	
}

/*
void expressionBubble::wheelEvent(QGraphicsSceneWheelEvent *event)
{
	//prepareGeometryChange();	
    if(!onBoarder( event->pos() ) && !_scene->naviContains(event->scenePos()))
	{
		float rate = exp(-event->delta() / 500.0);		
		{
			reScale(x0y0, x1y1, (x0y0-event->pos())*rate+event->pos()-x0y0, (x1y1-event->pos())*rate+event->pos()-x1y1, _scale, dCenter);
			//itemGrid=recordItemGrid(visibleItemNum, visibleCompartmentNum);		
		}
	}	
}*/

void expressionBubble::getExpressionToBePaint(int excludePid)
{   
	//if it is first time, resize the bubble

	//excludePid: for the pathway that is not to be considered (e. g. in the ungroup() )
	setExpressedGenePos(1); // order according to the expression level
	getExpressionToBePaint(bgRect, bgColor, borderColor, htColor, borderWidth, htRect, fontSize, fontColor, fontPos, fixedSize, dCenter, _scale, excludePid);
}

void expressionBubble::moveExpressionToBePaint(QPointF dis)
{   //excludePid: for the pathway that is not to be considered (e. g. in the ungroup() )
	moveExpressionToBePaint(dis, bgRect, htRect, fontPos, fontSize);
}

void expressionBubble::scaleExpressionToBePaint(QPointF dCenter, float scale)
{   
	//excludePid: for the pathway that is not to be considered (e. g. in the ungroup() )
	scaleExpressionToBePaint(bgRect, htRect, fontColor, fontPos, dCenter, scale);
}

void expressionBubble::scaleExpressionToBePaint(vector<QRectF> &bgRect, vector<vector<QRectF>> &htRect, vector<QColor> &fontColor, vector<QPointF> &fontPos, QPointF dCenter, float scale)
{   
   //change scale, zoom in or out the visualization
	//resizeExpressionToBePaint: change bubble size
   bool isOr=true; //flag for if we color the expressed one shared by all the bubbles (And relationship) or color them in (Or relationship)
    float wThreshold; //node width threhold for drawing text;
   if(_layoutType==1)
       wThreshold=8;
   else
       wThreshold=22;


   int height, width;
  
	width=fixedWidth;
	height=fixedHeight;
  
	float H, W, L=expressedGeneOrder.size();//expressedGene.size(); 
   float K=expressedGeneOrder.size(); //geneInfo.size();
   H=sqrt(K); W=K/H; W=int(W); H=int(H); if(W*H<K) W=W+1; if(W*H<K) H=H+1;
  float X,Y,x,y;
   float yit=height/H, xit=width/W;
   int count=0;
   int i,j,id;
   
   QRectF rect;
   int esize=expressedGeneOrder.size();

   QList<ItemBase *> mlist=_scene->getGroupMembers(this);

   bool useIt=true,haveIt=false;	  
   //do not consider matching between grouped bubble keqin
	int pcount=0;
	for( int i = 0; i < mlist.size(); i ++)	
	{
	    ItemBase *item = mlist[i];
		if(item->getType()==PATHBUBBLE1 || item->getType()==SUBPATHBUBBLE1)
		{
			int pid=item->m_pathwayID;
			pcount++;			
		}	
	}
	if(pcount==1 && haveIt)
	{
	    useIt=false;	
	}

   QRectF rRect = QRectF(this->realRect().x(), this->realRect().y(), this->realRect().width(), this->realRect().height());
   float Sx, Sy;
   float Ys = yit*_scale * 0.3;
   Sy=this->realRect().height()*2, Sx=this->realRect().width();
   int fontsize=-1;
   for(int l=0; l<esize; l++)
   {            
		id=expressedGeneOrder[l];		    
		rect=nodeRect(expressedGenePos[id], fixedSize,  dCenter, scale);
		bgRect[l]=rect;
		htRect[l].clear();			
		for(int i=0; i<quantity[id].size(); i++)
		{							
			QPointF texpos = textPos(expressedGeneTextPos[id], expressedGeneTextColumeRect[id][i], fixedSize,  dCenter, scale);			
			fontPos[l]=texpos;
			float min=exMin, max=exMax;							
			rect=nodeRect(expressedGeneTextColumeRect[id][i], fixedSize,  dCenter, scale);
			htRect[l].push_back(rect);
		}	

		if(fontsize<0 || geneInfo[id][1].size()>4)
		{
			int fontsize1=10;
			float Xs = bgRect[l].width()*1.1;
			if(bgRect[l].width()>wThreshold)	//( bgRect[l].x()> -width-bgRect[l].width() && bgRect[l].x() < width && bgRect[l].y()> -height - bgRect[l].height() && bgRect[l].y() < height) )		 
			{
				QSizeF size;
				QString str;
				if(geneInfo[id][1].size()<5)
					str="test";
				else str=geneInfo[id][1];
				size=getFontSize( str, fontsize1);
				while(
						( (_layoutType==0 && (size.height()>Ys || size.width()>Xs) )
						|| (_layoutType==1 && (size.height()>Xs || size.width()>Ys) ) )
						&& fontsize1>6)
				{
					fontsize1=fontsize1-1;
					size=getFontSize( str, fontsize1);
				}
			}
			else
				fontsize1=0;

			if(geneInfo[id][1].size()<5 && fontsize<0)
				fontsize =  fontsize1;
			else 
				fontSize[l]=fontsize1;
		}
		if( !rRect.intersects(bgRect[l]) && !rRect.contains(bgRect[l]) )					
			fontSize[l] = 0;
		else 
		{
			if(geneInfo[id][1].size()<5)
				fontSize[l] = fontsize;
		}				
		++count;				
   }    
}

void expressionBubble::resizeExpressionToBePaint(QRect oriRect)
{
	resizeExpressionToBePaint(oriRect, bgRect, htRect, fontPos, fontSize);
}

void expressionBubble::resizeExpressionToBePaint(QRect oriRect, vector<QRectF> &bgRect, vector<vector<QRectF>> &htRect, vector<QPointF> &fontPos, vector<int> &fontSize)
{   
	float wThreshold; //node width threhold for drawing text;
   if(_layoutType==1)
       wThreshold=8;
   else
       wThreshold=22;

	//change bubble size
	//scaleExpressionToBePaint: change scale, zoom in or out the visualization
    vector<bool> fVisible(fontSize.size(),true);
    QRectF rRect = QRectF(this->realRect().x(), this->realRect().y(), this->realRect().width(), this->realRect().height());
	QRectF oRect = QRectF(oriRect.x(), oriRect.y(), oriRect.width(), oriRect.height());

   
	for( int i = 0; i < fontSize.size(); i++)	
	{
		if(!oRect.intersects(bgRect[i]) && !oRect.contains(bgRect[i]) )
			fVisible[i]=false;
		else 
			fVisible[i]=true;
	} 
	
    int fontsize=-1;

   int width=fixedWidth;
   int	height=fixedHeight;
   float H, W, L=expressedGene.size(); 
   //QRectF rRect = QRectF(this->realRect().x(), this->realRect().y(), this->realRect().width(), this->realRect().height());
   float K=geneInfo.size();
   H=sqrt(K); W=K/H; W=int(W); H=int(H); if(W*H<K) W=W+1; if(W*H<K) H=H+1;
   float yit=height/H, xit=width/W;
    float Ys = yit*_scale * 0.3;
    for( int i = 0; i < fontSize.size(); i++)	
	{
		int id=expressedGeneOrder[i];
		if(fVisible[i])			
		{
			if(fontSize[i]!=0)
				fontsize=fontSize[i];
			if(!rRect.intersects(bgRect[i]) && !rRect.contains(bgRect[i]))
			   fontSize[i]=0;
			
		}
		else 
		{
			if(rRect.intersects(bgRect[i]) || rRect.contains(bgRect[i]))
			{
			   if(fontsize<0 || geneInfo[id][1].size()>4)
				{
					int fontsize1=10;
					float Xs = bgRect[i].width()*1.1;
					
					if(bgRect[i].width()>wThreshold)	//( bgRect[l].x()> -width-bgRect[l].width() && bgRect[l].x() < width && bgRect[l].y()> -height - bgRect[l].height() && bgRect[l].y() < height) )		 
					{
						QSizeF size;
						QString str;
						if(geneInfo[id][1].size()<5)
							str="test";
						else str=geneInfo[id][1];
						size=getFontSize( str, fontsize1);
						while(
						( (_layoutType==0 && (size.height()>Ys || size.width()>Xs) )
						|| (_layoutType==1 && (size.height()>Xs || size.width()>Ys) ) )
						&& fontsize1>6)
						{
							fontsize1=fontsize1-1;
							size=getFontSize( str, fontsize1);
						}
					}
					else
					   fontsize1=0;

					if(geneInfo[id][1].size()<5 && fontsize<0)
						fontsize =  fontsize1;
					else  
						fontSize[i]=fontsize1;
				}
				if(geneInfo[id][1].size()<5)
				    fontSize[i] = fontsize;
				
			}
		}
	} 
    

}


void expressionBubble::moveExpressionToBePaint(QPointF dis, vector<QRectF> &bgRect, vector<vector<QRectF>> &htRect, vector<QPointF> &fontPos, vector<int> &fontSize)
{  
	float wThreshold; //node width threhold for drawing text;
   if(_layoutType==1)
       wThreshold=8;
   else
       wThreshold=22;

    vector<bool> fVisible(fontSize.size(),true);
    QRectF rRect = QRectF(this->realRect().x(), this->realRect().y(), this->realRect().width(), this->realRect().height());

    if(dis.x()==0 && dis.y()==0)
	    return;
	for( int i = 0; i < fontSize.size(); i++)	
	{
		if(!rRect.intersects(bgRect[i]) && !rRect.contains(bgRect[i]) )
			fVisible[i]=false;
		else 
			fVisible[i]=true;
	} 
	for( int i = 0; i <fontPos.size(); i++)	
	{
		fontPos[i] = fontPos[i]+dis;
	}
	for( int i = 0; i < bgRect.size(); i++)	
	{   
		bgRect[i].moveCenter(bgRect[i].center()+dis);
	}
	for( int i = 0; i < htRect.size(); i++)	
	{
		for( int j = 0; j< htRect[i].size(); j++)	
			htRect[i][j].moveCenter(htRect[i][j].center()+dis);
	}   
    int fontsize=-1;

   int width=fixedWidth;
   int	height=fixedHeight;
   float H, W, L=expressedGeneOrder.size();   
   float K=expressedGeneOrder.size();
   H=sqrt(K); W=K/H; W=int(W); H=int(H); if(W*H<K) W=W+1; if(W*H<K) H=H+1;

   float yit=height/H, xit=width/W;
     float Ys = yit*_scale * 0.3;
    for( int i = 0; i < fontSize.size(); i++)	
	{
		int id=expressedGeneOrder[i];
		if(fVisible[i])			
		{
			if(fontSize[i]!=0)
				fontsize=fontSize[i];
			if(!rRect.intersects(bgRect[i]) && !rRect.contains(bgRect[i]))
			   fontSize[i]=0;
		}
		else 
		{
			if(rRect.intersects(bgRect[i]) || rRect.contains(bgRect[i]))
			{
			   if(fontsize<0 || geneInfo[id][1].size()>4)
				{
					int fontsize1=10;
					float Xs = bgRect[i].width()*1.1;
					if(bgRect[i].width()>wThreshold)	//( bgRect[l].x()> -width-bgRect[l].width() && bgRect[l].x() < width && bgRect[l].y()> -height - bgRect[l].height() && bgRect[l].y() < height) )		 
					{
						QSizeF size;
						QString str;
						if(geneInfo[id][1].size()<5)
							str="test";
						else str=geneInfo[id][1];
						size=getFontSize( str, fontsize1);
						while(
						( (_layoutType==0 && (size.height()>Ys || size.width()>Xs) )
						|| (_layoutType==1 && (size.height()>Xs || size.width()>Ys) ) )
						&& fontsize1>6)
						{
							fontsize1=fontsize1-1;
							size=getFontSize( str, fontsize1);
						}
					}
					else
					   fontsize1=0;

					if(geneInfo[id][1].size()<5 && fontsize<0)
						fontsize =  fontsize1;
					else  
						fontSize[i]=fontsize1;
				}
				if(geneInfo[id][1].size()<5)
				    fontSize[i] = fontsize;
				
			}
		}
	} 
}


void expressionBubble::getExpressionToBePaint(vector<QRectF> &bgRect, vector<QColor> &bgColor, vector<QColor> &borderColor, vector<QColor> &htColor, vector<int> &borderWidth, vector<vector<QRectF>> &htRect, vector<int> &fontSize, vector<QColor> &fontColor, vector<QPointF> &fontPos, bool fixedSize, QPointF dCenter, float scale, int excludePid)
{   
   bool isOr=true; //flag for if we color the expressed one shared by all the bubbles (And relationship) or color them in (Or relationship)
   bool onlyExpressed=true; //if only expressed node get in 
    
   float wThreshold; //node width threhold for drawing text;
   if(_layoutType==1)
       wThreshold=8;
   else
       wThreshold=22;

   int height, width;
  
	width=fixedWidth;
	height=fixedHeight;
	QRectF rRect = QRectF(this->realRect().x(), this->realRect().y(), this->realRect().width(), this->realRect().height());
  
	float H, W, L=expressedGeneOrder.size();//expressedGene.size(); 
   float K=expressedGeneOrder.size(); //geneInfo.size();
   H=sqrt(K); W=K/H; W=int(W); H=int(H); if(W*H<K) W=W+1; if(W*H<K) H=H+1;
   float X,Y,x,y;
   float yit=height/H, xit=width/W;
   int count=0;
   int i,j,id;
   
   QRectF rect;
   int esize=expressedGeneOrder.size();
   bgRect.resize(esize);
   bgColor.resize(esize);
   borderColor.resize(esize);
   borderWidth.resize(esize);
   htRect.clear();
   htRect.resize(esize);
   fontSize.resize(esize);
   fontPos.resize(esize);
   htColor.resize(esize);
   fontColor.resize(esize);

   QList<ItemBase *> mlist=_scene->getGroupMembers(this);

   bool useIt=true,haveIt=false;	  
   //do not consider matching between grouped bubble keqin
	int pcount=0;
	for( int i = 0; i < mlist.size(); i ++)	
	{
	    ItemBase *item = mlist[i];
		if(item->getType()==PATHBUBBLE1 || item->getType()==SUBPATHBUBBLE1)
		{
			int pid=item->m_pathwayID;
			pcount++;
			if(excludePid==pid)
				haveIt=true;
		}	
	}
	if(pcount==1 && haveIt)
	{
	    useIt=false;	
	}

   int Sx, Sy;
   Sy=this->realRect().height()*2, Sx=this->realRect().width();

   for(int l=0; l<esize; l++)
   {            
		id=expressedGeneOrder[l];
		i=l%int(W), j=l/int(W);
		//x=-width/2+xit*i; y=-height/2+yit*j;		
	    		
		rect=nodeRect(expressedGenePos[id], fixedSize,  dCenter, _scale);
		bgRect[l]=rect;
		bgColor[l]=QColor(206,206,206,228);
		//painter->drawRect(rect);
	}
	
   float Ys = yit*_scale * 0.3;
   for(int l=0; l<esize; l++)
   {            
	   id=expressedGeneOrder[l];
	   //i=l%int(W), j=l/int(W);
	   //x=-width/2+xit*i; y=-height/2+yit*j;	
	
	    vector<int> node;
	    node.push_back('P');  node.push_back(id);
		if(highlighted.find(node)!=highlighted.end())
		{
						
			borderColor[l]=MIDIUM_HIGHLIGHTCOLOR;//QColor(255,100,100,255);
			borderWidth[l]=4;
		}
		else if(searched.find(node)!=searched.end())
		{	
			borderColor[l]=MIDIUM_HIGHLIGHTCOLOR;
			borderWidth[l]=4;
		}
		else
		{
			borderColor[l]=QColor(QColor(0,0,0,255));
			borderWidth[l]=0;
		}		
		//rect=nodeRect(expressedGenePos[id], fixedSize,  dCenter, _scale);		
		//bgRect[l]=rect;
		
		//painter->setPen(QColor(0,0,0,255));		
		int msize=mlist.size();
		bool flag, flag1, flag2, flag3; //flag shared gene, flag1: shared expressed gene in pathway, flag2: shared gene in tree ring, flag3: shared expressed gene in the tree ring
		//int count=0;
		int fontsize=-1;
	

		if(onlyExpressed)
		{
		     bgColor[l]=QColor(QColor(255,255,255,255));
		     //x=-width/2+xit*i; y=-height/2+yit*j;	
//			 rect=nodeRect(expressedGenePos[id], fixedSize,  dCenter, _scale);		
			for(int i=0; i<quantity[id].size(); i++)
			{			
				float min=exMin, max=exMax;													
				rect=nodeRect(expressedGeneTextColumeRect[id][i], fixedSize,  dCenter, _scale);			
				htRect[l].push_back(rect);												
			}
			if(fontsize<0 || geneInfo[id][1].size()>4)
		    {
				int fontsize1=10;
				float Xs = bgRect[l].width()*1.0;
				if(bgRect[l].width()>wThreshold)	//( bgRect[l].x()> -width-bgRect[l].width() && bgRect[l].x() < width && bgRect[l].y()> -height - bgRect[l].height() && bgRect[l].y() < height) )		 
			    {
					QSizeF size;
					QString str;
					if(geneInfo[id][1].size()<5)
						str="test";
					else str=geneInfo[id][1];
					size=getFontSize( str, fontsize1);
					while(
						( (_layoutType==0 && (size.height()>Ys || size.width()>Xs) )
						|| (_layoutType==1 && (size.height()>Xs || size.width()>Ys) ) )
						&& fontsize1>6)
					{
						fontsize1=fontsize1-1;
						size=getFontSize( str, fontsize1);
					}
			    }
				else
				   fontsize1=0;

				if(geneInfo[id][1].size()<5 && fontsize<0)
					fontsize =  fontsize1;
			    else  
					fontSize[l]=fontsize1;
			}

			if( !rRect.intersects(bgRect[l]) && !rRect.contains(bgRect[l]) )					
			    fontSize[l] = 0;
			else 
			{
				if(geneInfo[id][1].size()<5)
				   fontSize[l] = fontsize;
			}

			fontColor[l]=QColor(0,0,0,255);
			for(int i=0; i<quantity[id].size(); i++)
			{			
				QPointF texpos = textPos(expressedGeneTextPos[id], expressedGeneTextColumeRect[id][i], fixedSize,  dCenter, _scale);			
				fontPos[l]=texpos;
			}
			if(quantity[id][0]!=0)
			{
				if(quantity[id][1]>quantity[id][0] && quantity[id][1]/quantity[id][0]>=2.0)
				{
					htColor[l] = QColor(230,68,113,255);
				}
				else if(quantity[id][1]<quantity[id][0] && (quantity[id][0]-quantity[id][1])/quantity[id][0]>=0.5)
				{
					htColor[l] = QColor(81,81,255,255);						
				}
				else
					htColor[l] = QColor(0,0,0,255);
			}	
			else if(quantity[id][1]>0)
			{
				node[0]=m_bindPathwayID, node[1]='P';
				if(node.size()<3)
					node.push_back(j);
				else node[2]=j;
				htColor[l] = QColor(230,68,113,255);						
			}
			else
				htColor[l] = QColor(0,0,0,255);			

			/*for(int i=0; i<quantity[id].size(); i++)
			{			
				float min=exMin, max=exMax;							
				rect=nodeRect(expressedGeneTextColumeRect[id][i], fixedSize,  dCenter, _scale);				
			}*/	
		}		
		else 
		{
			if(!useIt || flag || flag1 || flag2 || flag3)
			{
				if( flag || flag2)
				   bgColor[l]=QColor(QColor(255,255,255,255));

				if(flag1 || flag3)
				{
					set<vector<int>> itemColored;
					bgColor[l]=QColor(assignColorToExpressedItems(0,0,0, _scene->getGroup(this)->expressedSets, itemColored, EXPRESSIONBACKGROUNDCOLOR ));//, getIDinExpressedSets(geneInfo[id][1])) );
				}
			
				//x=-width/2+xit*i; y=-height/2+yit*j;	
				//rect=nodeRect(expressedGenePos[id], fixedSize,  dCenter, _scale);
			
				for(int i=0; i<quantity[id].size(); i++)
				{			
					float min=exMin, max=exMax;													
					rect=nodeRect(expressedGeneTextColumeRect[id][i], fixedSize,  dCenter, _scale);			
					htRect[l].push_back(rect);												
				}

				if(fontsize<0 || geneInfo[id][1].size()>4)
				{
					int fontsize1=10;
					float Xs = bgRect[l].width()*1.1;
					if(bgRect[l].width()>wThreshold)	//( bgRect[l].x()> -width-bgRect[l].width() && bgRect[l].x() < width && bgRect[l].y()> -height - bgRect[l].height() && bgRect[l].y() < height) )		 
					{
						QSizeF size;
						QString str;
						if(geneInfo[id][1].size()<5)
							str="test";
						else str=geneInfo[id][1];
						size=getFontSize( str, fontsize1);
						while(
						( (_layoutType==0 && (size.height()>Ys || size.width()>Xs) )
						|| (_layoutType==1 && (size.height()>Xs || size.width()>Ys) ) )
						&& fontsize1>6)
						{
							fontsize1=fontsize1-1;
							size=getFontSize( str, fontsize1);
						}
					}
					else
					   fontsize1=0;

					if(geneInfo[id][1].size()<5 && fontsize<0)
						fontsize =  fontsize1;
					else  
						fontSize[l]=fontsize1;
				}

				if( !rRect.intersects(bgRect[l]) && !rRect.contains(bgRect[l]) )					
					fontSize[l] = 0;
				else 
				{
					if(geneInfo[id][1].size()<5)
					   fontSize[l] = fontsize;
				}

				fontColor[l]=QColor(0,0,0,255);
				for(int i=0; i<quantity[id].size(); i++)
				{			
					QPointF texpos = textPos(expressedGeneTextPos[id], expressedGeneTextColumeRect[id][i], fixedSize,  dCenter, _scale);			
					fontPos[l]=texpos;
				}
		
				if( flag1 || flag3)
				{
					if(quantity[id][0]!=0)
					{
						if(quantity[id][1]>quantity[id][0] && quantity[id][1]/quantity[id][0]>=2.0)
						{
							htColor[l] = QColor(230,68,113,255);
						}
						else if(quantity[id][1]<quantity[id][0] && (quantity[id][0]-quantity[id][1])/quantity[id][0]>=0.5)
						{
							htColor[l] = QColor(81,81,255,255);						
						}
						else
							htColor[l] = QColor(0,0,0,255);
					}	
					else if(quantity[id][1]>0)
					{
						node[0]=m_bindPathwayID, node[1]='P';
						if(node.size()<3)
							node.push_back(j);
						else node[2]=j;
						htColor[l] = QColor(230,68,113,255);						
					}
					else
						htColor[l] = QColor(0,0,0,255);
				}

				/*for(int i=0; i<quantity[id].size(); i++)
				{			
					float min=exMin, max=exMax;							
					rect=nodeRect(expressedGeneTextColumeRect[id][i], fixedSize,  dCenter, _scale);				
				}*/	
			}
		    else
			{
				if(flag)
					 htColor[l] = QColor(0,0,0,255);			
				else 
					htColor[l]=QColor(188,188,188,255); 
			
				if(fontsize<0 || geneInfo[id][1].size()>4)
				{
					int fontsize1=10;
					float Xs = bgRect[l].width()*1.1;
					if(bgRect[l].width()>wThreshold)	//( bgRect[l].x()> -width-bgRect[l].width() && bgRect[l].x() < width && bgRect[l].y()> -height - bgRect[l].height() && bgRect[l].y() < height) )		 
					{
						QSizeF size;
						QString str;
						if(geneInfo[id][1].size()<5)
							str="test";
						else str=geneInfo[id][1];
						size=getFontSize( str, fontsize1);
						while(
						( (_layoutType==0 && (size.height()>Ys || size.width()>Xs) )
						|| (_layoutType==1 && (size.height()>Xs || size.width()>Ys) ) )
						&& fontsize1>6)
						{
							fontsize1=fontsize1-1;
							size=getFontSize( str, fontsize1);
						}
					}
					else
					   fontsize1=0;

					if(geneInfo[id][1].size()<5 && fontsize<0)
					fontsize =  fontsize1;
					else  
					   fontSize[l]=fontsize1;
				}

				if( !rRect.intersects(bgRect[l]) && !rRect.contains(bgRect[l]) )					
					fontSize[l] = 0;
				else 
				{
					if(geneInfo[id][1].size()<5)
					   fontSize[l] = fontsize;
				}
			
				fontColor[l]=QColor(188,188,188,255);

				for(int i=0; i<quantity[id].size(); i++)
				{							
					QPointF texpos = textPos(expressedGeneTextPos[id], expressedGeneTextColumeRect[id][i], fixedSize,  dCenter, _scale);			
					fontPos[l]=texpos;
				}				
				for(int i=0; i<quantity[id].size(); i++)
				{			
					float min=exMin, max=exMax;							
					rect=nodeRect(expressedGeneTextColumeRect[id][i], fixedSize,  dCenter, _scale);
					htRect[l].push_back(rect);
				}				
			}
		
		}
		//borderColor[l]=QColor(QColor(0,0,0,255));
		//x=x+xit/2;
		//++count;				
   }    
}

void expressionBubble::paintExpression(QPainter *painter, vector<QRectF> bgRect, vector<QColor> bgColor, vector<QColor> borderColor, vector<QColor> htColor, vector<int> borderWidth, vector<vector<QRectF>> htRect, vector<int> fontSize, vector<QPointF> fontPos)
{   
   int height, width;
  
   width=fixedWidth;
   height=fixedHeight;

   int count=0;
   int i,j,id;   
   
   painter->save();

   QRectF rect;
   int esize=expressedGeneOrder.size();
   if(esize==0)
   {
	   QFont f1("Arial", 10);
       painter->setFont (f1);
	   QList<ItemBase *> mlist=_scene->getGroupMembers(this);	     
	   painter->drawText(-105, -15, "No gene is differentially expressed.");			
	  
	   bool flag=false;
	   for(int l=0; l<esize; l++)
	   {   
			if(mlist[l]->getType() == SUBPATHBUBBLE1 || mlist[l]->getType() == PATHBUBBLE1 || mlist[l]->getType() == SUBPATHBUBBLE1)
			{
			    flag=true;
				break;
			}
	   }	   
	   if(!flag)
		 painter->drawText(-115, 15, "Please link it to tree ring or pathway data.");	
   }
   
   if(borderColor.size()==0)
	{
		painter->restore();
		return;
   }
   
   height=this->realRect().height()/2, width=this->realRect().width()/2;
   QRectF rRect = QRectF(this->realRect().x(), this->realRect().y(), this->realRect().width(), this->realRect().height());

   QPen pen;
   QBrush brush;
   QList<ItemBase *> mlist=_scene->getGroupMembers(this);
   
   float hScale=0.9;
   int row=quantity[0].size();
   float Height = bgRect[0].height()*row/((row-1)+hScale);
   float dy=Height/row;

  

    QFont f("Arial", 12);
    painter->setFont (f);
   if(_layoutType==1)	
   {	
	    float ex = bgRect[0].x()-20, ey = bgRect[0].y();
		vector<QString> qstr(2,"");
		qstr[0] = "control"; //"rpkm"
		qstr[1] = "heat";	
		
        pen.setColor(QColor(0,0,0,255));
		painter->setPen(pen);
		
		for(int i=0; i<row; i++)
		{	
			drawAngleText(painter, ex, ey+20, qstr[i], 90);
			ey=ey+dy;
		}	
		float tx=bgRect[0].x();
		if(tx< -width + 10)
			tx = -width +10;
		 painter->drawText(tx, bgRect[0].y()-5, "rpkm");		
	}		
    
    for(int l=0; l<esize; l++)
    {            
	   if( !rRect.intersects(bgRect[l]) && !rRect.contains(bgRect[l]) )
		   continue;

		id=expressedGeneOrder[l];
		pen.setColor(borderColor[l]);
		pen.setWidth(borderWidth[l]);
		
		painter->setPen(Qt::NoPen);
		painter->setBrush(bgColor[l]);

		//background
		 if(_layoutType==1)
		 {
			float ex=bgRect[l].x(), ey=bgRect[l].y(), w=bgRect[l].width(), h=dy*hScale;
			for(int i=0; i<row; i++)
			{
				QRectF rect=QRectF(ex, ey, w, h);
				painter->drawRect(rect);
				ey=ey+dy;
			}
		 }
		 else
		 {
		    painter->drawRect(bgRect[l]);   
		 }
		 
		 if(borderWidth[l]!=0)
		 {
			painter->setBrush(Qt::NoBrush);
			painter->setPen(pen);
			for(int i=0; i<row; i++)
			{
				painter->drawRect(bgRect[l]);
			}
		 }
   } 
	QFont f1("Arial", fontSize[0]);
    painter->setFont (f1);
	QFontMetrics fontMetrics(f1);
    QRect fontRect = fontMetrics.boundingRect("test"); 
	float fDY = bgRect[0].height() + fontRect.height()*0.5;

   for(int l=0; l<esize; l++)
   {            
	   if( !rRect.intersects(bgRect[l]) && !rRect.contains(bgRect[l]) )
		   continue;
	   		 
	   id=expressedGeneOrder[l];
	   
	   for(int i=0; i<quantity[id].size(); i++)
	   {		
            pen.setColor(borderColor[l]);
			pen.setWidth(borderWidth[l]);
			
			if(borderWidth[l]==0)
				painter->setPen(Qt::NoPen);
			else
				painter->setPen(pen);
			painter->setPen(Qt::NoPen);

			painter->setBrush(htColor[l]);
			QRectF drect=htRect[l][i];
			if(drect.height()>-1 && quantity[id][i]>0)
			{
			   drect = QRectF(drect.x(), drect.y(), drect.width(), -1);
			}
			painter->drawRect(drect);
		}		
		painter->setPen(borderColor[l]);		
				
		if(fontSize[l]!=0)
		{		
			 if(_layoutType==1)			 
			     drawAngleText(painter, fontPos[l].x(), fontPos[l].y()+10, geneInfo[id][1], 45);
			 else
			 {
				 painter->drawText(fontPos[l].x(), bgRect[l].y()+fDY, geneInfo[id][1]);
			 }
		}
   }  
   painter->restore();
}

void expressionBubble::drawHighLightNode(QPainter *painter, int hl)
{
		
	int id=expressedGeneOrder[hl];
	//pen.setColor(borderColor[l]);
	//pen.setWidth(borderWidth[l]);
		
	//	painter->setPen(Qt::NoPen);
    painter->setBrush(LIGHT_HIGHLIGHTCOLOR);

   float hScale=0.9;
   int row=quantity[0].size();
   float Height = bgRect[0].height()*row/((row-1)+hScale);
   float dy=Height/row;

	if(_layoutType==1)
	{
		//backgroud
		if(borderWidth[hl]==0)
			painter->setPen(Qt::NoPen);
		else
			painter->setPen(borderColor[hl]);
				
        float ex=bgRect[hl].center().x(), ey=bgRect[hl].center().y(), w=bgRect[hl].width()<10?10:bgRect[hl].width(), h=dy*hScale;//bgRect[hl].width()
	    float scalex=w/bgRect[hl].width();
        float scaley=scalex>1?1.1:1.0;
		float y=bgRect[hl].y()-(scaley-1)*Height;		
		h = h*scaley;
		dy = dy*scaley;

		if(scalex>1 || scaley>1)
		{
			QRectF brect=QRectF(ex-w/2, y, w, bgRect[0].height()*scaley);
			painter->setBrush(QColor(255,255,255,255));
			painter->drawRect(brect);
			painter->setBrush(LIGHT_HIGHLIGHTCOLOR);
		}	
		for(int i=0; i<row; i++)
		{
			QRectF rect=QRectF(ex-w/2, y, w, h);
			painter->drawRect(rect);
			y=y+dy;
		}
		//draw histogram
		for(int i=0; i<row; i++)
		{
			painter->setPen(Qt::NoPen);
			painter->setBrush(htColor[hl]);
			QRectF drect=htRect[hl][i];
			drect = QRectF( (drect.x()-ex)*scalex+ex, (drect.y()-ey)*scaley+ey-(scaley-1)*Height*0.5, drect.width()*scalex, drect.height()*scaley);
			if(drect.height()>-1 && quantity[id][i]>0)
			{
				drect = QRectF(drect.x(), drect.y(), drect.width(), -1);
			}
			painter->drawRect(drect);
		}		
	}
	else
	{
		float ex=bgRect[hl].center().x(), ey=bgRect[hl].center().y(), w=bgRect[hl].width()<20?20:bgRect[hl].width(), h=bgRect[hl].height()<20?20:bgRect[hl].height();
        float scalex=w/bgRect[hl].width();		
        float scaley=h/bgRect[hl].height();	

		//backgroud
		if(borderWidth[hl]==0)
			painter->setPen(Qt::NoPen);
		else
			painter->setPen(borderColor[hl]);


		QRectF rect=QRectF(ex-w/2, ey-h/2, w, h);

		if(scalex>1 || scaley>1)
		{
		    painter->setBrush(QColor(255,255,255,255));
			painter->drawRect(rect);
			painter->setBrush(LIGHT_HIGHLIGHTCOLOR);
		}	
		painter->drawRect(rect);
	    
		//draw histogram
		for(int i=0; i<row; i++)
		{
			painter->setPen(Qt::NoPen);
			painter->setBrush(htColor[hl]);
			QRectF drect=htRect[hl][i];
			drect = QRectF( (drect.x()-ex)*scalex+ex, (drect.y()-ey)*scaley+ey, drect.width()*scalex, drect.height()*scaley);

			if(drect.height()>-1 && quantity[id][i]>0)
			{
				drect = QRectF(drect.x(), drect.y(), drect.width(), -1);
			}
			painter->drawRect(drect);
		}
	}
}

void expressionBubble::setExpressedGenePos(int orderType)
{   //add orderType
    //0: no order, 1: the more expressed one first, 2: put those alpahbci order
	//only put those both matched and differentially expressed in the expressedGeneOrder 

	//if(_layoutType==0)  //grid layout horizontal text
	// else if(_layoutType==1) //vertical layout vertical
	
	vector<int> temp;
	for(int id=0; id<geneInfo.size(); id++)
	{	
		if(isExpressedMatchedGene(id))
		{
			temp.push_back(id);
		}
    } 
	if(expressedGeneOrder==temp)
		return;
	else
	{   
		expressedGeneOrder=temp;
	}
	
	expressedGeneOrder.clear();
	exMax=-1000000; exMin=1000000;   
    for(int i=0; i< geneInfo.size(); i++)
	{
		//expressedGene.insert(i);
		if(quantity[i][0]>exMax)
		     exMax=quantity[i][0];
		else if(quantity[i][0]<exMin)
			 exMin=quantity[i][0];
		if(quantity[i][1]>exMax)
		     exMax=quantity[i][1];
		else if(quantity[i][1]<exMin)
			 exMin=quantity[i][1];
	}
   
    float width, height;
    int esize=geneInfo.size();

	width=FIXEDWIDTH/2*_scale;
	height=FIXEDHEIGHT/2*_scale;
	
	x0y0=QPoint(-this->Width()/2.0,-this->Height()/2.0);	
	x1y1=QPoint(this->Width()/2.0,this->Height()/2.0);
	  
   int count=0;
   int i,j,id;
  
   QRectF rect;
   if(orderType==0)
   {
        for(id=0; id<geneInfo.size(); id++)
		{	
			if(isExpressedMatchedGene(id))
			{
				 expressedGeneOrder.push_back(id);
			}
	    } 	   
   }
   else if(orderType==1)
   {
	   float v;
	   list<vector<float>> Vlist;
	   for(id=0; id<geneInfo.size(); id++)
	   {	
		   //if(expressedGene[m_bindPathwayID].find(id) != expressedGene[m_bindPathwayID].end())
		   if(isExpressedMatchedGene(id))
		   {
			   if(quantity[id][0]!=0)
			    v=quantity[id][1]/quantity[id][0];
				else if(quantity[id][1]==0) v=0;
				else v=10000;

				vector<float> temp;
				temp.push_back(v);  temp.push_back(id);
				Vlist.push_back(temp);
		   }
	   } 
	   Vlist.sort();
	   for(list<vector<float>>::reverse_iterator it=Vlist.rbegin(); it!=Vlist.rend(); it++)
	   {
	       vector<float> temp=*it;
	       expressedGeneOrder.push_back(int(temp[1]));
	   }
   } 
   else if(orderType==2)
   {
	   float v;
	   set<vector<QString>> Vlist;
	   for(id=0; id<geneInfo.size(); id++)
	   {	
		    if(isExpressedMatchedGene(id))
	        {
				vector<QString> temp;
				temp.push_back(geneInfo[id][1]);  temp.push_back(QString::number(id));
				Vlist.insert(temp);
	        }
	   } 	  
	   for(set<vector<QString>>::iterator it=Vlist.begin(); it!=Vlist.end(); it++)
	   {
	       vector<QString> temp=*it;
	       expressedGeneOrder.push_back(temp[1].toInt());
	   } 
   }   
	expressedGenePos.resize(esize);

	expressedGeneTextPos.clear(); //be sure to clear before resize
	expressedGeneTextColumeRect.clear(); //be sure to clear before resize

	expressedGeneTextPos.resize(esize);
	expressedGeneTextColumeRect.resize(esize);

   
   if(_layoutType==0)  //grid layout horizontal text
   {
	   float H, W, L=expressedGeneOrder.size(); 
	   float K=expressedGeneOrder.size(); 
	   H=sqrt(K); W=K/H; W=int(W); H=int(H); if(W*H<K) W=W+1; if(W*H<K) H=H+1;
	   float X,Y,x,y;
	   float yit=height/H, xit=width/W;
	   float dx=0, dy=0;
	   if(xit>30)
	   {
		   yit = yit/xit*30;	 
		   dx = (width  - width/xit*30)/2;
		   dy = (height - height/yit*30)/2;
		   xit=30;
	   }	   

	   float brWidthScale=0.9;
	   for(int l=0; l<expressedGeneOrder.size(); l++)
	   {
			id=expressedGeneOrder[l];
			i=l%int(W), j=l/int(W);
			x=-width/2+xit*i+dx; y=-height/2+yit*j+dy;		
	    		
			float w=xit/width, h=yit/height;	
			x=x/width, y=y/height;
			rect=QRectF(x, y, w, h);		
			expressedGenePos[id] = QRectF(rect.center().x()-w*brWidthScale/2, rect.center().y()+h*0.3, w*brWidthScale, h*0.8);		
			float ex=expressedGenePos[id].x(), ey=expressedGenePos[id].y() + expressedGenePos[id].height();
		
			expressedGeneTextPos[id]=QPointF(ex, ey+h*0.2);
		
			for(int i=0; i<quantity[id].size(); i++)
			{			
				float min=exMin, max=exMax;
				float Height=(quantity[id][i]-min)/(max-min) * expressedGenePos[id].height();
				float Width=expressedGenePos[id].width()*0.55;
				//if(Height<1&&Height!=0) Height=1;
				
				QRectF erect = QRectF(ex, ey, expressedGenePos[id].width()*brWidthScale/2, -Height);		//QRectF((x+xit*0.075)/width, (y+yit*0.78)/height, (xit*0.85*0.5)/width, -Height/height);		
				expressedGeneTextColumeRect[id].push_back(erect);
				ex=ex+Width;
		   }					
	   }  
   }
   else if(_layoutType==1) //vertical layout vertical
   {	
       float W, L,K; 
	   K=W=L=expressedGeneOrder.size(); 
	   float X,Y,x,y;
	   float wScale=0.95, hScale=0.5;
	   float yit=height/quantity[0].size()*hScale, xit=width/W*wScale;
	   float dx=0, dy=0;
	   
	   if(xit>30)
	   {
		   //yit = yit/xit*30;	 
		   dx = (width  - width/xit*30)/2;		   
		   xit=30;
	   }
	   if(xit<2)
	   {
		   dx = (width  - width/xit*2)/2;		   
		   xit=2;	   
	   }
	   dy = (height - height*hScale)/2;
	   float w=xit/width, h=hScale;	
			
	   float hd=yit/height*hScale;
	   for(int l=0; l<expressedGeneOrder.size(); l++)
	   {
		    i=l;
			id=expressedGeneOrder[l];
			x=-width/2+xit*i+dx; y=-height/2+dy;		
			x=x/width, y=y/height;
			rect=QRectF(x, y, w, h);		

			expressedGenePos[id] = QRectF(rect.x(), rect.y(), w, h*0.9);	
			float hd=expressedGenePos[id].height()/2;
			float ex=expressedGenePos[id].x(), ey=expressedGenePos[id].y() + hd;	

			for(int k=0; k<quantity[id].size(); k++)
			{
				ey=expressedGenePos[id].y() + (k+1)*hd;	
				float min=exMin, max=exMax;
				float Height=(quantity[id][k]-min)/(max-min) * expressedGenePos[id].height()/2*0.9;
				float Width=expressedGenePos[id].width();				
				QRectF erect = QRectF(ex, ey, Width, -Height);		
				expressedGeneTextColumeRect[id].push_back(erect);
				if(k==quantity[id].size()-1) 
					expressedGeneTextPos[id]=QPointF(ex+Width*0.5, ey);
		   }
		   expressedGenePos[id]=QRectF(expressedGenePos[id].x(), expressedGenePos[id].y()+hd*0.1, expressedGenePos[id].width(), expressedGenePos[id].height()-hd*0.1);
	   }  	   
   }
 
}

int expressionBubble::getIDinExpressedSets(QString name)
{
	for(int id = 0; id<_scene->expressedGeneNames.size(); id++)
	{
	  if(_scene->expressedGeneNames[id] == name)     
	      return id;
	}
}


QRectF expressionBubble::nodeRect(QRectF nodePos, bool fixedSize,  QPointF dCenter, float scale, bool enLarge)
{	
	/*int width, height;	
	float dw,dh;
	float lFactor=0;
	if(enLarge)	lFactor=0.2;
	
	
	//width=FIXEDWIDTH*scale, height=FIXEDHEIGHT*scale;	
	
	float h=nodePos.height(), w=nodePos.width();	
    float cx=nodePos.x(), cy=nodePos.y();
	
	//h=h*height, w=w*width;	
	//dw=w*lFactor; dh=h*lFactor;
    cx=cx*width, cy=cy*height;//-width/2  -height/2		
		
	float dcx = dCenter.x() * this->realRect().width(), dcy = dCenter.y()*this->realRect().height();
    QRectF rect = QRectF(cx + dcx, cy + dcy, w - dw, h + dh);// - dw/*/

	int width, height;
	width=FIXEDWIDTH*scale, height=FIXEDHEIGHT*scale;	

	float h=nodePos.height(), w=nodePos.width();	
    float cx=nodePos.x(), cy=nodePos.y();
	
	h=h*height, w=w*width;	
    cx=cx*width, cy=cy*height;
	
	
	int dcx=dCenter.x()*this->realRect().width(), dcy=dCenter.y()*this->realRect().height();	
    QRect rect = QRect(cx+ dcx, cy+ dcy, w, h);

    return rect;
}

QPointF expressionBubble::textPos(QPointF nodePos, QRectF nodeRect, bool fixedSize,  QPointF dCenter, float scale, bool enLarge)
{		
	int width, height;
	width=FIXEDWIDTH*scale, height=FIXEDHEIGHT*scale;	

	//float h=nodePos.height(), w=nodePos.width();	
    float cx=nodePos.x(), cy=nodePos.y();
	
	//h=h*height, w=w*width;	
    cx=cx*width, cy=cy*height;
	
	
	int dcx=dCenter.x()*this->realRect().width(), dcy=dCenter.y()*this->realRect().height();
    QPointF node = QPointF(cx+ dcx, cy+ dcy);

    return node;

	/*float dw,dh;
	int width, height;
	float lFactor=0;
	if(enLarge)	
		lFactor=0.2;

	
	width=FIXEDWIDTH*scale, height=FIXEDHEIGHT*scale;

	float h=nodeRect.height(), w=nodeRect.width();	
    float cx=nodePos.x(), cy=nodePos.y();	
	h=h*height, w=w*width;	
	dw=w*lFactor; dh=h*lFactor;	

    cx=cx*width, cy=cy*height;
	int dcx=dCenter.x()*this->realRect().width(), dcy=dCenter.y()*this->realRect().height();
    QPointF textpos = QPointF(cx+ dcx - dw/2, cy+ dcy - dh*4);
    return textpos;*/
}


int expressionBubble::getVerticeID_2(int type, int id)
{
	  int ID;
	  switch(type)
	  {
		 case 'C': ID = id;  break;			 
		 case 'P': ID = complexNum + id;  break;
		 case 'S': ID = complexNum + proteinNum + id ;  break;
		 case 'E': ID = complexNum + proteinNum + smallMoleculeNum + id;  break;			
		 case 'D': ID = complexNum + proteinNum + smallMoleculeNum + physicalEntityNum + id;  break; 		 
		 case 'N': ID = complexNum + proteinNum + smallMoleculeNum + physicalEntityNum + DnaNum + id ;  break;		 
		 case 'R': ID = complexNum + proteinNum + smallMoleculeNum + physicalEntityNum + DnaNum + EmptyNum + id ;  break;		 
	  }
	  return ID-1;
}


void expressionBubble::hoverMoveEvent(QGraphicsSceneHoverEvent *event )
{ 
	hoverGeneOrderID=-1;
	//static bool grabon=false;	
	ItemBase::hoverMoveEvent(event);
	if( onBoarder( event->pos() ) )
	{
		
	}
	else
	{
		for(int l=0; l<expressedGeneOrder.size(); l++)
		{
			int id=expressedGeneOrder[l];
			QRectF rect=nodeRect(expressedGenePos[id], fixedSize,  dCenter, _scale);
			if(rect.contains(event->pos()))
			{
				hoverGeneOrderID=l;
			}	
		}
	}	
}

void expressionBubble::drawHoveredSymbol(QPainter *painter, int hl)
{
     if(hl<0 || expressedGeneOrder.empty())
		return;

	int id = expressedGeneOrder[hl];

	painter->save();

	drawHighLightNode(painter, hl);

    QString str = geneInfo[id][1];
	if(str.size()!=0)
	{		
		QFont f("Arial",10);	
		painter->setFont(f);	
		QFontMetrics fontMetrics(f);
		QRect fontRect = fontMetrics.boundingRect(str); 	

		int fontRectHeight=fontRect.height();
			
		if(_layoutType==1)			 
		{
			QRectF rect1=QRectF(fontPos[hl].x(), fontPos[hl].y()+10, fontRect.width(), -fontRect.height()*0.9);		

			painter->setPen(Qt::NoPen);	
			painter->setBrush(QColor(255,255,255,255));
				
			drawAngleRect(painter, rect1, 45);

			painter->setPen(MIDIUM_HIGHLIGHTCOLOR);				
			drawAngleText(painter, fontPos[hl].x(), fontPos[hl].y()+10, str, 45);
		}
		else
		{
			QRectF rect1=QRectF(fontPos[hl].x(), bgRect[hl].y()+bgRect[hl].height()+fontRect.height()*0.85, fontRect.width(), -fontRect.height()*0.8);		

			painter->setPen(Qt::NoPen);	
			painter->setBrush(QColor(255,255,255,255));
			painter->drawRect(rect1);

			painter->setPen(MIDIUM_HIGHLIGHTCOLOR);	
			painter->drawText(fontPos[hl].x(), bgRect[hl].y()+bgRect[hl].height()+fontRect.height()*0.85, str);
		}
	}
	painter->restore();
}

/*

void expressionBubble::updateItemGrid(vector<vector<set<vector<int>>>> &itemGrid, int type, int id, QPointF newPos)
{
	 //find previous grid
	int h=20,w=20;
	 int xi,yi,xi0,yi0,xi1,yi1;
	 vector<int> temp;
	 temp.push_back(type); temp.push_back(id);

	 QRectF node;

	switch(type)
	{
		 case 'C': node=complexPos[id];   break;
		 case 'D': node=DnaPos[id];       break;
		 case 'E': node=physicalEntityPos[id];   break;
		 case 'P': node=proteinPos[id];          break;
		 case 'S': node=smallMoleculePos[id];    break;
		 case 'R': node=reactionPos[id];   break;
	 }

	 xi0=node.x()*w, yi0=node.y()*h;
	 if(xi0<0) xi0=0; else if(xi0>=w) xi0=w;
	 if(yi0<0) yi0=0; else if(yi0>=h) yi0=h;
				
	 xi1=(node.x()+node.width())*w, yi1=(node.y()+node.height())*h;
	 if(xi1<0) xi1=0; else if(xi1>=w) xi1=w;
	 if(yi1<0) yi1=0; else if(yi1>=h) yi1=h;

	 for(int xi=xi0;xi<=xi1;xi++)
	 for(int yi=yi0;yi<=yi1;yi++)
	 {
		 //Grid[xi][yi].push_back('C'); Grid[xi][yi].push_back(i);			 
		 itemGrid[xi][yi].erase(temp);		 
	 }

	 xi0=newPos.x()*w, yi0=newPos.y()*h;
	 if(xi0<0) xi0=0; else if(xi0>=w) xi0=w;
	 if(yi0<0) yi0=0; else if(yi0>=h) yi0=h;
				
	 xi1=(newPos.x()+node.width())*w, yi1=(newPos.y()+node.height())*h;
	 if(xi1<0) xi1=0; else if(xi1>=w) xi1=w;
	 if(yi1<0) yi1=0; else if(yi1>=h) yi1=h;

	 for(int xi=xi0;xi<=xi1;xi++)
	 for(int yi=yi0;yi<=yi1;yi++)
	 {
		 //Grid[xi][yi].push_back('C'); Grid[xi][yi].push_back(i);			 
		 itemGrid[xi][yi].insert(temp);		 
	 }
	 //remove
		
	 //add new grid

	 //find the item grid
}
*/

/*vector<vector<set<vector<int>>>> expressionBubble::recordItemGrid( )
{
    //10*10
	int h=20,w=20;
	vector<int> item;
	set<vector<int>> empty;
	vector<set<vector<int>>> unit(h+1,empty);
	vector<vector<set<vector<int>>>> Grid(w+1,unit);
	int xi,yi,xi0,yi0,xi1,yi1;

	vector<int> temp(2,0);
	
	for(int i=0; i<complexPos.size(); i++)
	{
		if(complexPos[i].x()!=-1)
		{
			xi0=complexPos[i].x()*w, yi0=complexPos[i].y()*h;
			if(xi0<0) xi0=0; else if(xi0>=w) xi0=w;
			if(yi0<0) yi0=0; else if(yi0>=h) yi0=h;
				
			xi1=(complexPos[i].x()+complexPos[i].width())*w, yi1=(complexPos[i].y()+complexPos[i].height())*h;
			if(xi1<0) xi1=0; else if(xi1>=w) xi1=w;
			if(yi1<0) yi1=0; else if(yi1>=h) yi1=h;

			temp[0]='C'; temp[1]=i;

			for(int xi=xi0;xi<=xi1;xi++)
			for(int yi=yi0;yi<=yi1;yi++)
			{
			    Grid[xi][yi].insert(temp);			
			}
		}
	}
	for(int i=0; i<proteinPos.size(); i++)
	{
	  if(proteinPos[i].x()!=-1)
	  {
			xi0=proteinPos[i].x()*w, yi0=proteinPos[i].y()*h;
			if(xi0<0) xi0=0; else if(xi0>=w) xi0=w;
			if(yi0<0) yi0=0; else if(yi0>=h) yi0=h;

			xi1=(proteinPos[i].x()+proteinPos[i].width())*w, yi1=(proteinPos[i].y()+proteinPos[i].height())*h;
			if(xi1<0) xi1=0; else if(xi1>=w) xi1=w;
			if(yi1<0) yi1=0; else if(yi1>=h) yi1=h;

			temp[0]='P'; temp[1]=i;
			for(int xi=xi0;xi<=xi1;xi++)
			for(int yi=yi0;yi<=yi1;yi++)
			{
				Grid[xi][yi].insert(temp);			
			}
	  }
	}

	for(int i=0; i<physicalEntityPos.size(); i++)
	{
	  if(physicalEntityPos[i].x()!=-1)
	  {
			xi0=physicalEntityPos[i].x()*w, yi0=physicalEntityPos[i].y()*h;
			if(xi0<0) xi0=0; else if(xi0>=w) xi0=w;
			if(yi0<0) yi0=0; else if(yi0>=h) yi0=h;

			xi1=(physicalEntityPos[i].x()+physicalEntityPos[i].width())*w, yi1=(physicalEntityPos[i].y()+physicalEntityPos[i].height())*h;
			if(xi1<0) xi1=0; else if(xi1>=w) xi1=w;
			if(yi1<0) yi1=0; else if(yi1>=h) yi1=h;

			temp[0]='E'; temp[1]=i;

			for(int xi=xi0;xi<=xi1;xi++)
			for(int yi=yi0;yi<=yi1;yi++)
			{
			   Grid[xi][yi].insert(temp);				
			}
	  }
	}
	
	
	for(int i=0; i<smallMoleculePos.size(); i++)
	{
		if(smallMoleculePos[i].x()!=-1)
        {
			xi0=smallMoleculePos[i].x()*w, yi0=smallMoleculePos[i].y()*h;
			if(xi0<0) xi0=0; else if(xi0>=w) xi0=w;
			if(yi0<0) yi0=0; else if(yi0>=h) yi0=h;
			

			xi1=(smallMoleculePos[i].x()+smallMoleculePos[i].width())*w, yi1=(smallMoleculePos[i].y()+smallMoleculePos[i].height())*h;
			if(xi1<0) xi1=0; else if(xi1>=w) xi1=w;
			if(yi1<0) yi1=0; else if(yi1>=h) yi1=h;

			temp[0]='S'; temp[1]=i;

			for(int xi=xi0;xi<=xi1;xi++)
			for(int yi=yi0;yi<=yi1;yi++)
			{
				Grid[xi][yi].insert(temp);		
			}		
		}
	}
	
	for(int i=0; i<DnaPos.size(); i++)
	{
		if(DnaPos[i].x()!=-1)
		{	
			xi0=DnaPos[i].x()*w, yi0=DnaPos[i].y()*h;
			if(xi0<0) xi0=0; else if(xi0>=w) xi0=w;
			if(yi0<0) yi0=0; else if(yi0>=h) yi0=h;
			//Grid[xi0][yi0].push_back('D'); Grid[xi0][yi0].push_back(i);					

			xi1=(DnaPos[i].x()+DnaPos[i].width())*w, yi1=(DnaPos[i].y()+DnaPos[i].height())*h;
			if(xi1<0) xi1=0; else if(xi1>=w) xi1=w;
			if(yi1<0) yi1=0; else if(yi1>=h) yi1=h;

			temp[0]='D'; temp[1]=i;

			for(int xi=xi0;xi<=xi1;xi++)
			for(int yi=yi0;yi<=yi1;yi++)
			{
				Grid[xi][yi].insert(temp);			
			}
		}
	    
	}
	
	for(int i=0; i<reactionPos.size(); i++)
	{
		if(reactionPos[i].x()!=-1)
		{
			xi0=reactionPos[i].x()*w, yi0=reactionPos[i].y()*h;
			if(xi0<0) xi0=0; else if(xi0>=w) xi0=w;
			if(yi0<0) yi0=0; else if(yi0>=h) yi0=h;
				
			xi1=(reactionPos[i].x()+reactionPos[i].width())*w, yi1=(reactionPos[i].y()+reactionPos[i].height())*h;
			if(xi1<0) xi1=0; else if(xi1>=w) xi1=w;
			if(yi1<0) yi1=0; else if(yi1>=h) yi1=h;

			temp[0]='R'; temp[1]=i;

			for(int xi=xi0;xi<=xi1;xi++)
			for(int yi=yi0;yi<=yi1;yi++)
			{
				Grid[xi][yi].insert(temp);				
			}
		}
	}	
	return Grid;
}
*/
void expressionBubble::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	hoverGeneOrderID=-1;
	ItemBase::mousePressEvent(event);		
	//void expressionBubble::setAItemDragged(int type, int id)updateBubblePath(event->pos()-event->lastPos());
	_mousePressed=true;
	if( !onBoarder( event->pos() ) )
	{
	
		bool mark=0;		
		for(int i=0; i<expressedGeneOrder.size(); i++)
		{
			//if(expressedGene[m_bindPathwayID].find(i) != expressedGene[m_bindPathwayID].end())			
			{
			   float width=this->Width(), height=this->Height();
			   
			   width=fixedWidth, height=fixedHeight;
			   int id=expressedGeneOrder[i];

			   QRectF rect=nodeRect(expressedGenePos[id], fixedSize,  dCenter, _scale);
			   //QRectF rect=QRect(expressedGenePos[i].x()*width, expressedGenePos[i].y()*height, expressedGenePos[i].width()*width, expressedGenePos[i].height()*height);				
			   if(rect.contains(event->pos()))
			   {
			       //_scene->_pathBubbles[m_bindPathwayID]->hoveredItem.clear();				   
				   	search(geneInfo[id][1]);
				   /*for(int k=0; k<geneContainer[i].size(); k=k+2)
				   {
					    vector<int> itemSelected;						
				        itemSelected.push_back(geneContainer[i][k]);
						itemSelected.push_back(geneContainer[i][k+1]);
					    if(!itemSelected.empty())
						{
							//if(!_scene->_pathBubbles[m_bindPathwayID]->controlKeyisOn)
							if(_scene->controlKeyisOn)
							{
								if(_scene->_pathBubbles[m_bindPathwayID]->highlighted.size()==0)
									 _scene->_pathBubbles[m_bindPathwayID]->highlighted.insert(itemSelected);
								else
								{
									if(mark==0)
									{
										_scene->_pathBubbles[m_bindPathwayID]->highlighted.clear();																	
										mark=1;
									}
									_scene->_pathBubbles[m_bindPathwayID]->highlighted.insert(itemSelected);									
								}
							}
							else
							{
								_scene->_pathBubbles[m_bindPathwayID]->highlighted.insert(itemSelected);									
							}
						}
				   }*/
			   }			   
			}
		}		
	}

	/*if(m_bindPathwayID>=0)
	{
		vector<int> pre;//=itemSelected;
					for(set<vector<int>>::iterator it=highlighted.begin(); it!=highlighted.end(); it++)
	                {
		                vector<int> item = *it;
		                int type=item[0], id=item[1];		
						pre.push_back(type); pre.push_back(id);
					}
					getPartUpdateSelectedNodetoBePaint(pre, itemSelected);
		_scene->_pathBubbles[m_bindPathwayID]->getGraphToBePaint();
	}*/

	if(onBoarder( event->pos() ))
		pressOnBoarder = true;
	else 
		pressOnBoarder = false;

	 if(onUnGroupArea(event->pos()) )
		pressOnUngroup = true;
	else 
		pressOnUngroup = false;
}

void expressionBubble::renderColorLegend(QPainter *painter)
{
	float width= this->Width()/2-75; //_treeRing->GetRingWidth() * _treeRing->_ring.size() * 2.1; //4.5; //2.1
	float height= this->Height()/2-30;
	int itv=20;
	int sx=width-45, sy=height;
	
	painter->setPen(Qt::NoPen);
	painter->setBrush(Qt::white);
	painter->drawRect(QRect(sx-5, sy-itv*(8.5), this->Width()/2-sx+3, 22+itv*(8.5)));

	painter->setPen(QColor(50,50,50,255));	
	QFont f1("Arial", 10);
	painter->setFont (f1);	
	int grad=4;
	{		
		painter->drawText(sx, sy-itv*(grad-0.55), "rpkm");//Bar Chart
		for(int i=0; i<grad; ++i)
		{
			QString str;
			QRect rect(sx, sy-itv*i, 16, itv);
			float r, g, b;
			float value=(i+0.5)/grad;
			
			if(i==3)
			{
				painter->setBrush(QColor(217,217,217,255)); //htColor[l] = QColor(81,81,255,255);htColor[l] = QColor(230,68,113,255);QColor(255,100,100,255);QColor(206,206,206,228);
				str= "not found";
			}
			else if(i==2)
			{  
				painter->setBrush(QColor(0,0,0,255));
				str= "unchanged";
			}
			else if(i==1)
			{
				painter->setBrush(QColor(81,81,255,255));
				str= "down-expressed";
			}			
			else if(i==0)
			{
				painter->setBrush(QColor(230,68,113,255));
				str= "up-expressed";
			}			
			//three color
			painter->drawRect(rect);
			//QString str= QString::number((i+1)/8.0, 'g', 5);
			painter->drawText(sx+20, sy-itv*(i-0.55), str);
		}		
	}	
	
	sy=sy-itv*(grad+1.5);
	grad=3;
	//sx=width-60;
	painter->drawText(sx, sy-itv*(grad-0.55), "Background");
	for(int i=0; i<grad; ++i)
		{
			QString str;
			QRect rect(sx, sy-itv*i, 16, itv);
			float r, g, b;
			float value=(i+0.5)/grad;
			
			if(i==2)
			{
				painter->setBrush(QColor(217,217,217,228));// painter->setBrush(Qt::NoBrush); //htColor[l] = QColor(81,81,255,255);htColor[l] = QColor(230,68,113,255);QColor(255,100,100,255);QColor(206,206,206,228);
				str= "not found";
			}
			else if(i==1)
			{  
				painter->setBrush(QColor(255,255,255,255));
				str= "unchanged";
			}					
			else if(i==0)
			{
				painter->setBrush(EXPRESSIONBACKGROUNDCOLOR);
				str= "expressed";
			}			
			//three color
			painter->drawRect(rect);
			painter->drawText(sx+20, sy-itv*(i-0.55), str);
			//QString str= QString::number((i+1)/8.0, 'g', 5);
			//painter->drawText(sx+25, sy-itv*(i-0.4), str);
		}		
}

void expressionBubble::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	ItemBase::mouseReleaseEvent( event );
	_mousePressed=false;
	//m_isMoving=false;
	m_isInnerMoving=false;	

	pressOnBoarder = false;
	pressOnUngroup = false;
	/*_mousePressed=0;
	
	if( itemSelected == whichItem(event->pos(),this->Width(), this->Height()))
	{		
		if(!itemSelected.empty())
		{
			QString qstr;
			string s1;
			std::ostringstream oss;
			int type=itemSelected[0], id=itemSelected[1];
			switch(type)
			{ 
			    case 'C': qstr=_scene->_complexName[id].c_str(); break;
		        case 'E': qstr=_scene->_physicalEntityName[id][0].c_str(); break;
		        case 'P': qstr=_scene->_proteinName[id][0].c_str(); break;
		        case 'S': s1 = "smallMolecule";  	oss << s1 << id; s1=oss.str(); qstr=s1.c_str(); break;		
		        case 'D': s1 = "Dna"; 	oss << s1 << id;	s1=oss.str();  qstr=s1.c_str(); break;	 				     	
			}
			showIterm(event->pos(), event->scenePos(), qstr); 	
		}
		itemDragged.clear();
		//itemSelected[0]=-1;
		itemSelected.clear();
		edgeSelected.clear();
	}
	if(!itemSelected.empty())
	{
		if(!groupSelected&&_queryisOn)
		{
	        QPointF Dock=dockItemDragged(event->pos());		
		    QString qstr;
			string s1;
			std::ostringstream oss;
			QPointF iPos;
			int type=itemSelected[0], id=itemSelected[1];
			switch(type)
			{ 
			    case 'C': qstr=_scene->_complexName[m_pathwayID][id]; iPos=complexPos[id].center(); break;
		        case 'E': qstr=_scene->_physicalEntityName[m_pathwayID][id][0]; iPos=physicalEntityPos[id].center(); break;
		        case 'P': qstr=_scene->_proteinName[m_pathwayID][id][0]; iPos=proteinPos[id].center(); break;
		        case 'S': s1 = "smallMolecule";  	oss << s1 << id; s1=oss.str(); qstr=s1.c_str(); iPos=smallMoleculePos[id].center(); break;		
		        case 'D': s1 = "Dna"; 	oss << s1 << id;	s1=oss.str();  qstr=s1.c_str(); iPos=DnaPos[id].center(); break;	 				     	
				case 'N': qstr=_scene->_EmptyName[m_pathwayID][id][0]; iPos=EmptyPos[id].center(); break;	
			}
			iPos.setX(iPos.x()*this->Width()-this->Width()/2.0);
			iPos.setY(iPos.y()*this->Height()-this->Height()/2.0);
			showIterm(iPos, event->pos(), event->scenePos(), qstr); 	
			_penisOn=groupSelected=_grabisOn=_queryisOn=_upisOn=_downisOn=_evenisOn=false;
		    itemDragged.clear();
			itemSelected.clear();
		    edgeSelected.clear();		
		}
		else if(!groupSelected&&_grabisOn)
		{
	     QPointF Dock=dockItemDragged(event->pos());		
		 //new bubble
		 QString qstr;
			string s1;
			std::ostringstream oss;
			int type=itemSelected[0], id=itemSelected[1];
			QPointF iPos;
			switch(type)
			{ 
			    case 'C': qstr=_scene->_complexName[m_pathwayID][id]; iPos=complexPos[id].center(); break;
		        case 'E': qstr=_scene->_physicalEntityName[m_pathwayID][id][0]; iPos=physicalEntityPos[id].center(); break;
		        case 'P': qstr=_scene->_proteinName[m_pathwayID][id][0]; iPos=proteinPos[id].center(); break;
		        case 'S': s1 = "smallMolecule";  	oss << s1 << id; s1=oss.str(); qstr=s1.c_str(); iPos=smallMoleculePos[id].center(); break;		
		        case 'D': s1 = "Dna"; 	oss << s1 << id;	s1=oss.str();  qstr=s1.c_str(); iPos=DnaPos[id].center(); break;
				case 'N': qstr=_scene->_EmptyName[m_pathwayID][id][0]; iPos=EmptyPos[id].center(); break;	
			}
			iPos.setX(iPos.x()*this->Width()-this->Width()/2.0);
			iPos.setY(iPos.y()*this->Height()-this->Height()/2.0);
		
			if(!this->realRect().contains(event->pos().x(),event->pos().y()))
		   {
			   ReactionBubble3 *rb = dynamic_cast <ReactionBubble3*> (_open->openReactionBubble3(m_pathwayID, 50, event->scenePos().x(), event->scenePos().y(), itemSelected, qstr));		
		       if ( rb )
 	           {
				    rb->_pathBubbleParent=this;
			        this->_reactionPoint.append( QPointF((iPos.x()+this->Width()/2)/this->Width(), (iPos.y()+this->Height()/2)/this->Height()) );
			        this->_reactionBubbles.append( rb );
					set<vector<int>> temp; temp.insert(itemSelected);
					_reactionhighlight.append(temp);  
		       }
			}			   			
			_penisOn=groupSelected=_grabisOn=_upisOn=_queryisOn=false;
			itemDragged.clear();
			itemSelected.clear();
			edgeSelected.clear();
		}
		else if(groupSelected&&!_grabisOn&&!_queryisOn&&!_upisOn&&!_downisOn&&!_evenisOn)
		{
	        QPointF Dock=dockItemDragged(event->pos());		
		    //new bubble
		    QString qstr;
			string s1;
			std::ostringstream oss;			
			   
			QPointF iPos;
		
			QRectF rect=_contain.boundingRect();
			iPos=rect.center();
			
			if(!this->realRect().contains(event->pos().x(),event->pos().y()) && verifyItemSelected(itemSelected))
		    {
			   GroupBubble4 *rb = dynamic_cast <GroupBubble4*> (_open->openGroupBubble4(m_pathwayID, 200, event->scenePos().x(), event->scenePos().y(), itemSelected, edgeSelected));		
		       if ( rb )
 	           {
				    
				    //this->_conPoint.append( QPointF((iPos.x()+this->Width()/2)/this->Width(), (iPos.y()+this->Height()/2)/this->Height()));
				    rb->_pathBubbleParent=this;
			        this->_groupPoint.append( QPointF((iPos.x()+this->Width()/2)/this->Width(), (iPos.y()+this->Height()/2)/this->Height()) );
			        this->_groupBubbles.append( rb );
					_grouphighlight.append(highlighted);  
					_groupEdgeAffected.append(_edgeAffected);

					for(set<vector<int>>::iterator it=highlighted.begin(); it!=highlighted.end(); it++) 
					{
						vector<int> node=*it;
						int type=node[0], id=node[1];
						switch(type)
						{
							case 'C': complexPos[id].setX(-1000); break;
							case 'E': physicalEntityPos[id].setX(-1000); break;
		                    case 'P': proteinPos[id].setX(-1000); break;
		                    case 'S': smallMoleculePos[id].setX(-1000); break;
		                    case 'D': DnaPos[id].setX(-1000); break;
							case 'R': reactionPos[id].setX(-1000); break;
					    }			 					 
		           }
														
					//change selected bubble to same location
					
			   }
			}
			_penisOn=groupSelected=_grabisOn=_queryisOn=_upisOn=_downisOn=_evenisOn=false;
			itemDragged.clear();
			itemSelected.clear();
			edgeSelected.clear();
			_contain.clear();
			highlighted.clear();
		}
	
	}
	

	if (event->button() == Qt::LeftButton )
	{
		if(_penisOn)
		{
			_penisOn=false;
			//if(_lines.size()>6)
			//{
				groupSelected=groupSelection(_lines);
			//}
		}
		else if(!_penisOn && groupSelected)
	   {
		  //_contain.clear();
	      //itemSelected.clear();		
		   groupSelected=0;	
	   }		
	}

	if (event->button() == Qt::RightButton )
	{
		_grabisOn=false;
		_queryisOn=false;
		_upisOn=_downisOn=_evenisOn=false;
		groupSelected=0;
		_contain.clear();
		itemSelected.clear();	
		edgeSelected.clear();
	}
	
	draggingItem=0;
	//MenuisOn=0;
	*/
}

bool expressionBubble::verifyItemSelected(vector<int> itemSelected)
{
	bool flag=false;
	for(int i=0; i<itemSelected.size(); i+=2)
	{
	    if(itemSelected[i]!='R')
		   return true;
	}
	return flag;
}

void expressionBubble::checkGroupConnections(vector<int> &itemSelected, vector<vector<int>> &edgeSelected)
{
	vector<int> item(2,0);
	set<vector<int>> itemSet;
	for(int i=0; i<itemSelected.size(); i+=2)
	{
	    int type=itemSelected[i], id=itemSelected[i+1];
		item[0]=type, item[1]=id;
		itemSet.insert(item);		
	}
	int type,id,type1,id1,type2,id2;
    for(int i=0; i<_scene->_edge[m_pathwayID].size(); i++)
	{
         //painter->drawRect(reactionPos[i].x(),reactionPos[i].y(),2,2);				 
		 type=_scene->_edge[m_pathwayID][i][0], id=_scene->_edge[m_pathwayID][i][1];		
		 type1=_scene->_edge[m_pathwayID][i][4],	id1=_scene->_edge[m_pathwayID][i][5];
		 type2=_scene->_edge[m_pathwayID][i][2], id2=_scene->_edge[m_pathwayID][i][3];	
		 vector<int> item,item2,item1;
		 item.push_back(type), item.push_back(id); 
		 item1.push_back(type1), item1.push_back(id1);
		 item2.push_back(type2), item2.push_back(id2);
		 if( itemSet.find(item) != itemSet.end() && itemSet.find(item2) != itemSet.end() )
		 {
			if(itemSet.find(item1) == itemSet.end())
			{
			    itemSelected.push_back(item1[0]);
			    itemSelected.push_back(item1[1]);	
				itemSet.insert(item1);
				highlighted.insert(item1);
			}
			edgeSelected.push_back(_scene->_edge[m_pathwayID][i]);
		 }
	}
}


void expressionBubble::saveAsXmlNode( QDomDocument &doc, QDomElement &node )
{
	QDomText text;
	QString str, tmpStr;

	QDomElement type = doc.createElement( tr("Type") );
	text = doc.createTextNode( tr("expressionBubble") );
	type.appendChild( text );
	node.appendChild( type );

	ItemBase::saveAsXmlNode( doc, node );

	QDomElement pathPre = doc.createElement( tr("PathPre") );
	text = doc.createTextNode( this->_curFilePath );
	pathPre.appendChild( text );
	node.appendChild( pathPre );

	QDomElement curFile = doc.createElement( tr("FileName") );
	if (_curFileName == "")
	{
		//saveasMatlabFile();
	}
	text = doc.createTextNode( this->_curFileName );
	curFile.appendChild( text );
	node.appendChild( curFile );
	
	pathPre = doc.createElement( tr("bindPathwayID") );
	str.setNum( m_bindPathwayID );	
	text = doc.createTextNode( str );
	pathPre.appendChild( text );
	node.appendChild( pathPre );	
}


void expressionBubble::setAItemColored(int pid, int type, int id, int up_or_down)
{
	if(pid<0)
		return;
	vector<int> temp(4,0);
	std::set< vector<int> >::iterator it;
	temp[0]=pid; temp[1]=type; temp[2]=id; temp[3]=GO_UP;
	it=_scene->_pathBubbles[pid]->_itemColored.find(temp);
	if(it!=_scene->_pathBubbles[pid]->_itemColored.end()) _scene->_pathBubbles[pid]->_itemColored.erase(it);

	temp[3]=_DOWN;
	it=_scene->_pathBubbles[pid]->_itemColored.find(temp);
	if(it!=_scene->_pathBubbles[pid]->_itemColored.end()) 
		_scene->_pathBubbles[pid]->_itemColored.erase(it);

	temp[3]=_EVEN;
	it=_scene->_pathBubbles[pid]->_itemColored.find(temp);	
	if(it!=_scene->_pathBubbles[pid]->_itemColored.end()) 
		_scene->_pathBubbles[pid]->_itemColored.erase(it);

	temp[3]=up_or_down;
    _scene->_pathBubbles[pid]->_itemColored.insert(temp);
}

void expressionBubble::showIterm( QPointF iPos, QPointF pos, QPointF scenePos, QString name)
{
	float x=pos.x(), y=pos.y();	
	
	QString fileName("iterms/");
	fileName.append(name);
	fileName.append(".txt");
	QByteArray ba = fileName.toLocal8Bit();
    const char *name_str = ba.data();
	
	FILE *fp = fopen(name_str,"r"); 
	/*if(fp!=NULL)
	{
		//_open->openPathBubble2( 100, 600, 450, fileName);							
		TextBubble2 *matcode = dynamic_cast<TextBubble2*>(_open->openTextBubble2(100, scenePos.x(), scenePos.y(), fileName));
		if ( matcode )
 	    {
			 //matcode->_ItemBaseParent=this;			 
			 matcode->_pathBubbleParent=this;
			 //float x=pos().x(),y=pos().y();
			 //pos().setY(Pos.y());
			 //y=pos().y();
			 this->_conPoint.append( QPointF((iPos.x()+this->Width()/2)/this->Width(), (iPos.y()+this->Height()/2)/this->Height()));
			 this->_codeBubbles.append( matcode );

			 set<vector<int>> temp; temp.insert(itemSelected);
			 //_reactionhighlight.append(temp);  
			 _conhighlight.append(temp);  			 
			 
		}
	}*/
}


void expressionBubble::wheelEvent(QGraphicsSceneWheelEvent *event)
{
	 if(!onBoarder( event->pos() ) && !_scene->naviContains(event->scenePos()))
	 {
		float rate = exp(event->delta() / 500.0);	//float rate = exp( event->delta() / graphReferenceSize / 2);			
		{
			float _scale_t=_scale;	
			
			if(rate > 1)
			{

				_scale_t=_scale_t+0.2*(_scale>1?_scale:1);
				if(_scale < 100)
				   _scale=_scale_t;
			}
			else if(rate<1)
			{
				_scale_t=_scale_t-0.2;
				if(_scale > 0.01)
				   _scale=_scale_t;
			}							
			//getExpressionToBePaint();
			scaleExpressionToBePaint(dCenter, _scale);
		}
	}
}


bool expressionBubble::onBoarder( QPointF pos )
{
	int x = pos.x();
	int y = pos.y();
	int half = this->Width()/2;
	int half2 = this->Height()/2;
	if( (x > half - (HALOWIDTH+half*0.01+2)) || (x < -half + (HALOWIDTH+half*0.01+2)) )
	{
		return true;
	}
	if( (y > half2 - (HALOWIDTH+half2*0.01+2)) || (y < -half2 + (HALOWIDTH+half2*0.01+2)) )
	{
		return true;
	}
	return false;
}

bool expressionBubble::outsideBubble( QPointF pos )
{
	int x = pos.x();
	int y = pos.y();
	int half = this->Width()/2;
	int half2 = this->Height()/2;
	if( (x > half ) || ( x < -half ) )
	{
		return true;
	}
	if( (y > half2 ) || ( y < -half2 ) )
	{
		return true;
	}
	return false;
}

void expressionBubble::mouseMoveEvent( QGraphicsSceneMouseEvent *event )
{	
	QRect oriRect;//=this->realRect();
	if(pressOnBoarder || UNGROUPBYMENU)
	{
		oriRect=this->realRect();
		ItemBase::mouseMoveEvent( event );	
	}
	else if(_mousePressed && !m_isMoving && !m_isResizing && !m_rightButton && !outsideBubble(event->pos()))
	{	
		m_isInnerMoving=true;
		float S;			
		Pan(x0y0, x1y1, event->pos() - event->lastPos(), event->pos() - event->lastPos(), S, dCenter);				
		moveExpressionToBePaint(event->pos()-event->lastPos());
	}	
	if(m_isResizing)
	{
		resizeExpressionToBePaint(oriRect);
	}				
}

bool expressionBubble::isExpressedMatchedGene(int id)
{
	if(expresseGeneLinkedtoTreeRing.find(id)!=expresseGeneLinkedtoTreeRing.end())
		return true;

	QList<ItemBase *> mlist=_scene->getGroupMembers(this);
	int msize=mlist.size();
	for(int k=0; k<msize; k++) 
	{
		if(mlist[k]->getType() == SUBPATHBUBBLE1 || mlist[k]->getType() == PATHBUBBLE1)
		{
			int pid=mlist[k]->m_pathwayID;			
			/*if(sharedExpressionGene[pid].find(id) != sharedExpressionGene[pid].end() )
			{
				flag=true;	
			}*/
			if(expressedGene[pid].find(id) != expressedGene[pid].end() )
			{
				return true;
			}	
		}
	}
}

void expressionBubble::search(QString str)
{
	 
	int count=0,itemCount=0;
	
	if(str.size()==0)
	{
		//_scene->searchInPathBubble();
		return;
	}
	set<int> SearchList;
	SearchList.insert('P'); //SearchList.insert('D'); SearchList.insert('S'); SearchList.insert('E');  SearchList.insert('R');	SearchList.insert('M');				 
	int index;
	if(str.contains(" *"))
	while(str.indexOf(" ")==0)
	{	
		index = str.indexOf(" ");
		str = str.mid(index+1, str.size()-index);		
	}
	
	if(str.contains("* "))
	while(str.lastIndexOf(" ")==str.size()-1)
	{	
		index= str.lastIndexOf(" ");				
		str = str.mid(0, index);		
		if(str.size()==0)
			break;
	}	
	QList<ItemBase *> mlist=_scene->getGroupMembers(this);	 

	for(int i=0; i<mlist.size(); i++) 
	{
		int type=mlist[i]->getType();
	    if ( mlist[i]->getType() == SUBPATHBUBBLE1 || mlist[i]->getType() == PATHBUBBLE1)
		{			
			itemCount += _scene->searchInPathBubble(str, SearchList, "",  mlist[i]);	
		}
		if ( mlist[i]->getType() == EXPRESSIONBUBBLE)
		{			
			itemCount +=_scene->searchInExpressionBubble(str, SearchList, "",  mlist[i]);	
		}
		if ( mlist[i]->getType() == TREERING)
		{			
			itemCount +=_scene->searchInTreeringBubble(str, SearchList, "",  mlist[i]);	
			itemCount +=_scene->searchPathwayInTreeRingforASymbol(str, SearchList, "",  mlist[i]);	
		}
		count++;
	}
} 

void expressionBubble::clearExpressed()
{
	int csize=expressedProtein.size();
    expressedProtein.clear(); expressedProtein.resize(csize);
	expressedComplex.clear(); expressedComplex.resize(csize);
	expressedGene.clear();	expressedGene.resize(csize);
	expressedCompartment.clear(); expressedCompartment.resize(csize);
	expressedPathway.clear(); expressedPathway.resize(csize);
	
	for(set<vector<int>>::iterator it=coloredNode.begin(); it!=coloredNode.end(); it++)
	{
		vector<int> node=*it;
	    setAItemColored(node[0], node[1], node[2], _EVEN);
	}
	coloredNode.clear();
	expressionCleared=true;	
	//clear color 
}

void expressionBubble::clearTreeExpressed()
{
	matchedGene_TreeRing_Expression.clear();
	expresseGeneLinkedtoTreeRing.clear();
}

void expressionBubble::clearHighlighted()
{
	highlighted.clear();
}

void expressionBubble::clearSearched()
{
	searched.clear();
}

void expressionBubble::clearSharedExpression()
{
	int csize=sharedExpressionProtein.size();
    sharedExpressionProtein.clear(); 	sharedExpressionProtein.resize(csize); 	
	sharedExpressionGene.clear();	sharedExpressionGene.resize(csize);
	//matchedGene_TreeRing_Expression.clear();	//matchedGene_TreeRing_Expression.resize(csize);
	//expresseGeneLinkedtoTreeRing.clear();
}

void expressionBubble::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
	if(insideBubble(event->pos()))
	{
	   UNGROUP=false;
	   AFTERUNGROUP=false;
	}
	if(AFTERUNGROUP || m_isMoving)
	{
		m_isMoving=false;
	   return;
	}
    if (!isSelected() && _scene) 
	{
		_scene->clearSelection();
		setSelected(true);
	}
	vector<int> node;
	_scene->dropDownMenu0(event, this, node);
	/*

	QMenu menu;
	QString ss("QMenu::item {  background-color: black; border: 3px solid black; color: white }");
	//QString ss("QMenu::item {  background-color: rgba(100,100,100,255); border: 3px rgba(69,69,69,255); color: white }"); // Use background-color instead of background
    menu.setStyleSheet(ss);
    menu.setMinimumWidth(70);
			
	QList<ItemBase *> mList=_scene->getGroupMembers(this);		
	_penisOn=false;	_grabisOn=false; _queryisOn=false; _upisOn=_downisOn=_evenisOn=false;
	if(mList.size()>1)
	{	
		QAction *delAction = menu.addAction("Delete");		
		//QAction *ungroupAction = menu.addAction("unGroup");
		 QAction *delGroupAction = menu.addAction("Delete group");			
		QAction *selectedAction = menu.exec(event->screenPos());
	
		if (selectedAction == delAction)
	    {
			QMessageBox msgBox;
			msgBox.setWindowTitle("Delete Bubble");
			msgBox.setText("Are you sure you want to delete this bubble?");
			msgBox.setStandardButtons(QMessageBox::Yes);
			msgBox.addButton (QMessageBox::No);
			if(msgBox.exec() == QMessageBox::Yes)
			{
				_scene->bubbleDeleted(this);
				deleteSelectedItems(_scene);				
			}				
	   }
		if (selectedAction == delGroupAction)
		{
				QMessageBox msgBox;
				msgBox.setWindowTitle("Delete Group");
				msgBox.setText("Are you sure you want to delete this group?");
				msgBox.setStandardButtons(QMessageBox::Yes);
				msgBox.addButton (QMessageBox::No);
				if(msgBox.exec() == QMessageBox::Yes)
				{
					QList<ItemBase *> mlist=_scene->getGroupMembers(this);
					OpenGLScene *tscene=_scene;
					_scene->delectionStarter = this;
					vector<int> plist, clist,tlist;
					int pid=m_expressionID;
					for(int i=0; i<mlist.size(); i++) 
					{
						if(mlist[i]->getType() == SUBPATHBUBBLE1 || mlist[i]->getType() == PATHBUBBLE1)
						    plist.push_back(mlist[i]->m_pathwayID);						
						else
							plist.push_back(-1);

						if(mlist[i]->getType() == TEXTBUBBLE2)
						    clist.push_back(mlist[i]->m_codeBubbleID);
						else 
							clist.push_back(-1);
	
						tlist.push_back(mlist[i]->getType());												

					}
					bool flag=true;
					for(int i=0; i<mlist.size(); i++) 
					{
						if(mlist[i]!=this)
                        {
						      if(tlist[i] == SUBPATHBUBBLE1 ||tlist[i] == PATHBUBBLE1)
							   {
								   if(tscene->_pathBubbles[plist[i]]!=NULL) 
								   {
									   PathBubble1* pbubble = dynamic_cast<PathBubble1*>(mlist[i]);
									   pbubble->deleteSelectedItems(tscene);
								   }
							   }
							   else if(tlist[i] == EXPRESSIONBUBBLE )
							   {
								   expressionBubble* ebubble = dynamic_cast<expressionBubble*>(mlist[i]);
							       ebubble->deleteSelectedItems(tscene);
							   }
							   else if(tlist[i] == TEXTBUBBLE2 )
							   {
								   if(tscene->_codeBubbles[clist[i]]!=NULL)
								   {
									   TextBubble2* tbubble = dynamic_cast<TextBubble2*>(mlist[i]);
								       tbubble->deleteSelectedItems(tscene);
								   }
							   }
							   else								   
								   mlist[i]->deleteSelectedItems(tscene);
						  
						}
					}
					tscene->delectionStarter=NULL;
					if(pid<0 || (pid>=0&&tscene->_expressionBubbles[pid]!=NULL))
					{
						deleteSelectedItems(tscene);	
					}
					
				}				
			}
	}
	else 
	{
		QAction *delAction = menu.addAction("Delete");				
		QAction *selectedAction = menu.exec(event->screenPos());
		if (selectedAction == delAction)
	    {
			QMessageBox msgBox;
			msgBox.setWindowTitle("Delete Bubble");
			msgBox.setText("Are you sure you want to delete this bubble?");
			msgBox.setStandardButtons(QMessageBox::Yes);
			msgBox.addButton (QMessageBox::No);
			if(msgBox.exec() == QMessageBox::Yes)
			{
				_scene->bubbleDeleted(this);
				deleteSelectedItems(_scene);					
			}
		}			
	}
	//else if (selectedAction == searchAction)
	//{
	//	_open->openSearchView( 300, this->pos().x()+this->Width()/2.0, this->pos().y()-this->Height()/2.0, _name); 
	//}
	*/
}

void expressionBubble::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	ItemBase::mouseDoubleClickEvent(event);	
	
	/*AFTERUNGROUP = false;	
	UNGROUP = false;

	_scene->clearSelected(this);
	_scene->clearExpressed(this);	
	_scene->clearColored(this);
	_scene->clearUndoRedoRecord(this);	
	_scene->hoveredItem.clear();


	_scene->removeToolsinGroup(this, true);*/
	
}



/*void expressionBubble::readPathWay(QString filename, vector<QRectF> &complexPos, vector<QRectF> &proteinPos, vector<QRectF> &smallMoleculePos, vector<QRectF> &DnaPos, vector<QRectF> &reactionPos, vector<QRectF> &physicalEntityPos, vector<vector<int>> &edge)
{
	//QString inforstr = "Recover the scene from the xml file ";
	//inforstr.append( fileName );
	//inforstr.append( "\n" );
	//logOperation( inforstr );

	//instruction = doc.createProcessingInstruction("xml", " version=\"1.0\" encoding=\"UTF-8\" ");
	//doc.appendChild( instruction );
	QFile file( filename );

	if ( !file.open(QIODevice::ReadOnly) )
	{
		return;
	}
		

	QDomDocument doc;
	if ( !doc.setContent(&file) )
	{
		file.close();
		return;
	}
	file.close();

	vector<QString> unit(2,"");
	vector<int> temp(7,0);
	
	_scene-> _complexName.clear();
	_scene->  _physicalEntityName.clear();
	_scene->  _proteinName.clear();
	_scene->  _smallMoleculeName.clear();
	_scene-> _DnaName.clear();
	

	QRectF rect(-1000,-1000,0,0);

	complexPos.clear();
	proteinPos.clear();	
    smallMoleculePos.clear();	
    DnaPos.clear();	
    reactionPos.clear();	
    physicalEntityPos.clear();	
	edge.clear();


	complexPos.push_back(rect);
	proteinPos.push_back(rect);	
    smallMoleculePos.push_back(rect);
    DnaPos.push_back(rect);
    reactionPos.push_back(rect);
    physicalEntityPos.push_back(rect);
	

	
	_scene-> _complexName[m_pathwayID].push_back(unit);
	_scene->  _physicalEntityName[m_pathwayID].push_back(unit);
	_scene->  _proteinName[m_pathwayID].push_back(unit);	
	_scene->  _smallMoleculeName[m_pathwayID].push_back(unit);
	_scene-> _DnaName[m_pathwayID].push_back(unit);

	QDomElement rootNode = doc.documentElement();
	QDomNode docNode = rootNode.firstChild();
	QString tagname;
	QString itemname,positionName;

	QDomElement childElement;
	
	docNode = rootNode.firstChild();


	while ( !docNode.isNull() )
	{
		if ( docNode.isElement() )
		{
			QDomElement eachElement = docNode.toElement();

			//recover each node
			tagname = eachElement.tagName();
			if (tagname != "edgeBlock")			
			{
				QString Num;
				eachElement.attribute(tr("Num"),Num);
				int itemNum = Num.toInt();
				childElement = eachElement.firstChild().toElement();
			    itemNum=0;
						
				while ( !childElement.isNull() )
				{
					itemname = childElement.elementsByTagName("Name").at(0).toElement().text();	
					QDomNodeList itemNode = childElement.elementsByTagName( tr("Position") );
	                positionName = itemNode.at(0).toElement().text();

					QStringList posStr= positionName.split( "," );
	                QString str = posStr.at(0);
	                str.remove( 0, 1 );
	                str = str.simplified();
	                float x = str.toFloat();
	                str = posStr.at(1);
	                str = str.simplified();
	                float y  = str.toFloat();
					str = posStr.at(2);
	                str = str.simplified();
	                float w  = str.toFloat();
					str = posStr.at(3);
	                str.chop(1);
	                str = str.simplified();
	                float h  = str.toFloat();

					QRectF nrect(x,y,w,h);

					if(tagname=="complexBlock")
					{
						unit[0]=itemname;
					    _scene-> _complexName[m_pathwayID].push_back(unit);
						complexPos.push_back(nrect);
					}
					else if(tagname=="physicalEntityBlock")
					{
						unit[0]=itemname;
					    _scene-> _physicalEntityName[m_pathwayID].push_back(unit);	
						physicalEntityPos.push_back(nrect);
					}
					else if(tagname=="proteinBlock")
					{
						unit[0]=itemname;
					    _scene->  _proteinName[m_pathwayID].push_back(unit);
						proteinPos.push_back(nrect);	
					}
					else if(tagname=="smallMoleculeBlock")
					{
						unit[0]=itemname;
					    _scene->  _smallMoleculeName[m_pathwayID].push_back(unit);
						smallMoleculePos.push_back(nrect);
    				}
					else if(tagname=="DnaBlock")
					{
						unit[0]=itemname;
					    _scene->_DnaName[m_pathwayID].push_back(unit);
						DnaPos.push_back(nrect);
    				}
					else if(tagname=="reactionBlock")
					{
						reactionPos.push_back(nrect);
					}			
					childElement = childElement.nextSibling().toElement();
				}
			}
			else if (tagname == "edgeBlock")		
			{
			        QString Num;
				eachElement.attribute(tr("Num"),Num);
				int itemNum = Num.toInt();
				childElement = eachElement.firstChild().toElement();
			    itemNum=0;
						
				while ( !childElement.isNull() )
				{
					itemname = childElement.elementsByTagName("Name").at(0).toElement().text();	
				    QDomNodeList itemNode = childElement.elementsByTagName( tr("Position") );
	                positionName = itemNode.at(0).toElement().text();

					QStringList posStr= positionName.split( "," );
	                QString str = posStr.at(0);
	                str.remove( 0, 1 );
	                str = str.simplified();
	                temp[0] = str.toInt();
	                str = posStr.at(1);
	                str = str.simplified();
	                temp[1] =  str.toInt();
					str = posStr.at(2);
	                str = str.simplified();
	                temp[4] = str.toInt();
					str = posStr.at(3);
	                str = str.simplified();
	                temp[5] = str.toInt();
					str = posStr.at(4);
	                str = str.simplified();
	                temp[2] = str.toInt();
					str = posStr.at(5);
	                str = str.simplified();
	                temp[3] = str.toInt();
					str = posStr.at(6);
	                str.chop(1);
	                str = str.simplified();
	                temp[6] = str.toInt();		
		         
		            edge.push_back(temp);

					childElement = childElement.nextSibling().toElement();
				}
			}
		}
		docNode = docNode.nextSibling();
	}	
}
*/
