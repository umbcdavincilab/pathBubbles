#include "textBubble2.h"
#include "pathwayDataParser.h"
#include "ReactionBubble3.h"
//#include "PathBubble2.h"
//#include "GroupBubble4.h"
#include "PathBubble1.h"
#include "subPathBubble1.h"
#include <QTextDocument>
#include <QMap>


#include "myGraph.h"

//path 192905 has rna in smallentity files
/*#include <config.hpp>  
#include <boost/graph/adjacency_list.hpp>  

using namespace boost;*/

PathBubble1::PathBubble1(int pathwayID, int size, int x, int y, OpenGLScene* Scene, StatisticManager* manager, OpenManager* open, QString name, QString orthName, QString pathName, QString lastStoredGraph)
: PlotBase (size, x, y, Scene, manager)
{   	
	addedtoGraph=false;
	//linkSearchType=-1;
	initialANodeHandled=false;
	setOrthFileName(orthName);	
	drawOrthology = orthName.size()>=1;	 

	lastStoredfileName=lastStoredGraph;
	_ScaleBackup=1;
	_ScaleBackup_S=1;
    aveFontSize=-1;
	headPathway=NULL;
	_scene= Scene;
	fixedWidth=400, fixedHeight=fixedWidth/1.618;
	static int mark=0;
	_parentPathBubbleID = pathwayID;	

	if(pathwayID>=0)
        toplevel=0;   // subpathway from group selection
	else toplevel=1;  // top pathway from pathway data

	graphReferenceSize = _scene->graphReferenceSize;

	drawSmallmolecule = _scene->drawSmallmoleculeSettings; //true; //_scene->drawSmallmoleculeSettings;
	//_orthologyFileName = "";	
	drawCrossTalk = _scene->drawCrossTalk;	
	drawRateLimit = _scene->drawRateLimit;
	//drawOrthology = false;
	
	searchType=_scene->searchType;
		
	if(pathwayID<0)
		pathwayID=this->_scene->_pathBubbles.size();
		
	this->bIndex = _scene->getBubbleIndex();
	
	//_pieMenu = new PieMenuItem(_scene);
	//_pieMenuOn = false;
	
	completeSame=false; //for search: ask if the item need to be completely same (same location, same id) or not

	_manager = manager;
		
	_LABEL = false;	
	m_pathwayID=pathwayID;
	
	_pathName=pathName;
	_name=name;
	loadFile(_name); 

	_scale=1;
	
	
	//PathWayDataInitiation_1();
	//rewriteTreeRingGraphFile();
	if(!PathWayDataInitiation())
	{
	    //setVisible(false);
	}
		
	
	//get graph size
	/*float newWidth = fixedWidth, newHeight = fixedHeight;
	getGraphSize(newWidth, newHeight);
	resizeItem( newWidth, newHeight);*/
	//resizeItem( fixedWidth+ (rand()%200-100) , fixedHeight + (rand()%200-100) );

	//searchSharedProtein();
    //updateVisibleNodes(); 

	_pathBubbleParent = NULL;
	this->_open = open;	
	this->_TYPE = PATHBUBBLE1;	
	this->_colorBoarder = TDHelper::getBoarderColorByType(_TYPE, m_pathwayID);
	this->_colorInner = TDHelper::getInnerColorByType(_TYPE, m_pathwayID);	
	
	//menuType=-1;
	//_pieMenuState=0;
	_mousePressed=false;
	_scene->controlKeyisOn=false;
	highlighted.clear();	
}


void PathBubble1::getGraphSize(float &width, float &height)
{
	float x0, y0, x1, y1,xl=100000,yl=100000,xr=-100000, yr=-100000;
    int size=compartmentPos.size()-1;
	
	int count=0;
	for(int i=0; i<size ; i++) //size?
	{
	     if(i<compartmentRect.size() && compartmentRect[i].width()>=0 )
	     {   
			 x0=compartmentRect[i].x();
			 y0=compartmentRect[i].y();
			 x1=x0+compartmentRect[i].width();
			 y1=y0+compartmentRect[i].height();

			 if(x0 < xl)   xl=x0;
			 if(y0 < yl)   yl=y0;
			 if(x1 > xr)   xr=x1;
			 if(y1 > yr)   yr=y1;
			 count++;
		 }			
	}
	
	//consider if no compartment case
	if(count<1)
	{
		for(int i=0; i<complexPos.size(); i++)
		{
			if(complexRect[i].width()>=0)
			{  		x0=complexRect[i].x();
				 y0=complexRect[i].y();
				 x1=x0+complexRect[i].width();
				 y1=y0+complexRect[i].height();

				 if(x0 < xl)   xl=x0;
				 if(y0 < yl)   yl=y0;
				 if(x1 > xr)   xr=x1;
				 if(y1 > yr)   yr=y1;
			}	
		}	
		for(int i=0; i<reactionPos.size(); i++)
		{
			if(reactionRect[i].width()>=0)//&& whichreaction(m_pathwayID, 'R', i)!=5)
			{x0=reactionRect[i].x();
				 y0=reactionRect[i].y();
				 x1=x0+reactionRect[i].width();
				 y1=y0+reactionRect[i].height();

				 if(x0 < xl)   xl=x0;
				 if(y0 < yl)   yl=y0;
				 if(x1 > xr)   xr=x1;
				 if(y1 > yr)   yr=y1;
			}		
		}
		for(int i=0; i<smallMoleculePos.size(); i++)
		{
			if(smallMoleculeRect[i].width()>=0)
			{x0=smallMoleculeRect[i].x();
				 y0=smallMoleculeRect[i].y();
				 x1=x0+smallMoleculeRect[i].width();
				 y1=y0+smallMoleculeRect[i].height();

				 if(x0 < xl)   xl=x0;
				 if(y0 < yl)   yl=y0;
				 if(x1 > xr)   xr=x1;
				 if(y1 > yr)   yr=y1;
			}		
		}
		for(int i=0; i<DnaPos.size(); i++)
		{
			if(DnaRect[i].width()>=0)	
			{	x0=DnaRect[i].x();
				 y0=DnaRect[i].y();
				 x1=x0+DnaRect[i].width();
				 y1=y0+DnaRect[i].height();

				 if(x0 < xl)   xl=x0;
				 if(y0 < yl)   yl=y0;
				 if(x1 > xr)   xr=x1;
				 if(y1 > yr)   yr=y1;
			}	
		}
		for(int i=0; i<physicalEntityPos.size(); i++)
		{
			  if( physicalEntityRect[i].width()>=0)
			  {x0=physicalEntityRect[i].x();
				 y0=physicalEntityRect[i].y();
				 x1=x0+physicalEntityRect[i].width();
				 y1=y0+physicalEntityRect[i].height();

				 if(x0 < xl)   xl=x0;
				 if(y0 < yl)   yl=y0;
				 if(x1 > xr)   xr=x1;
				 if(y1 > yr)   yr=y1;
			  }
		}	
		for(int i=0; i<proteinPos.size(); i++)
		{
			  if(proteinRect[i].width()>=0)
			  {x0=proteinRect[i].x();
				 y0=proteinRect[i].y();
				 x1=x0+proteinRect[i].width();
				 y1=y0+proteinRect[i].height();

				 if(x0 < xl)   xl=x0;
				 if(y0 < yl)   yl=y0;
				 if(x1 > xr)   xr=x1;
				 if(y1 > yr)   yr=y1;
			  }
		}
		for(int i=0; i<ANodePos.size(); i++)
		{
			  if( ANodeRect[i].width()>=0)
			  {
				 x0=ANodeRect[i].x();
				 y0=ANodeRect[i].y();
				 x1=x0+ANodeRect[i].width();
				 y1=y0+ANodeRect[i].height();

				 if(x0 < xl)   xl=x0;
				 if(y0 < yl)   yl=y0;
				 if(x1 > xr)   xr=x1;
				 if(y1 > yr)   yr=y1;
			  }
		}
		for(int i=0; i<reactionPos.size(); i++)
		{
			  if( reactionRect[i].width()>=0)
			  {
				 x0=reactionRect[i].x();
				 y0=reactionRect[i].y();
				 x1=x0+reactionRect[i].width();
				 y1=y0+reactionRect[i].height();

				 if(x0 < xl)   xl=x0;
				 if(y0 < yl)   yl=y0;
				 if(x1 > xr)   xr=x1;
				 if(y1 > yr)   yr=y1;
			  }
		}
		 width=xr-xl;
	      height=yr-yl;	
	}
	else
	{
	      width=xr-xl;
	      height=yr-yl;	
	}	
}

void PathBubble1::PathWayDataInitiation_1()//for generate treering without generating the hierarchy pathway graph
{
	currentPathwayGraphit=pathwayGraphStored.end();
	currentHighlightedit=highlightedStored.end();
	currentSharedit=sharedStored.end();
	currentColoredit=coloredStored.end();
	currentLinkedit=linkedStored.end();
	currentLinkedinHeadit=linkedStoredinHead.end();
	currentDifferedit=differedStored.end();
	currentExpressedit=expressedStored.end();
	currentEventit=eventStored.end();

	stepNum=0;
	reactionNum = 0;
	complexNum = 0;
	proteinNum = 0;
	smallMoleculeNum = 0;
	DnaNum = 0;
	EmptyNum=0;
	dragout=0;	
		
	
	vector<QString> temp6; 	
	_scene->_quantityName.push_back(temp6);
	vector<QString> temp8;  
	temp8.push_back("");

	vector<vector<QString>> _complexName, _physicalEntityName; //start from 1
	vector<vector<QString>> _proteinName, _reactionName,_degradationName;	//start from 1
	vector<vector<QString>> _smallMoleculeName, _EmptyName, _DnaName, _ANodeName;  //start from 1

	_physicalEntityName.push_back(temp8);

	vector<QString> temp7(5,"");  
	_ANodeName.push_back(temp7);
	_complexName.push_back(temp7);
	_proteinName.push_back(temp7);	
	_reactionName.push_back(temp7);
	_degradationName.push_back(temp7);
	_smallMoleculeName.push_back(temp7);  
	_DnaName.push_back(temp7);
     _EmptyName.push_back(temp7);
	 
	 vector<vector<QString>> temp9,temp10; 
	 _scene->_geneInfo.push_back(temp10);

	vector<vector<vector<int>>> temp4;  
	_scene->_biochemicalReaction.push_back(temp4);
	_scene->_degradation.push_back(temp4);
	
	vector<set<vector<int>>> tempx;
	_scene->CompartmentContain.push_back(tempx);
	_scene->_compartmentName.push_back(temp9);

	vector<vector<float>> temp21;	
	_scene->_quantity.push_back(temp21);
	_scene->_minmax.push_back(temp21);

	vector<vector<int>> temp33;
	_scene->_complexContain.push_back(temp33);
		bool preComputed=true;
	if(toplevel&&_name.size()!=0)
	{
		    bool record=drawSmallmolecule;
			drawSmallmolecule=false; // keqin
			//drawSmallmolecule=true;

			QString tname;
			const char * ename;
			string sname;
			vector<vector<int>> _8convertedProtein, _8convertedComplex, _8convertedEntity;		
	        vector<vector<int>> _stepRoot; 

			
			tname = _name + "1pathway.txt"; sname= tname.toStdString();  ename=sname.c_str();
			stepNum=_scene->pwdParser->read1pathway(ename, _1pathway);			

			tname = _name + "2pathwayStepOrder.txt"; sname= tname.toStdString();  ename=sname.c_str();
			stepNum=_scene->pwdParser->read2pathwayStepOrder(ename, stepNum, _pathwayStep);		

			tname = _name + "3pathwayStepReactionControl.txt"; sname= tname.toStdString();  ename=sname.c_str();
			_scene->pwdParser->read3pathwayStep_reactionAndcontrol(ename, stepNum, _3pathwayStepCatalysis, _3pathwayStepReaction);    	

			tname = _name + "4biochemicalReaction.txt"; sname= tname.toStdString();  ename=sname.c_str();
			vector<int> temp = _scene->pwdParser->read4biochemicalReaction(ename, _scene->_biochemicalReaction[m_pathwayID], _reactionName, _scene->_degradation[m_pathwayID], _degradationName, _EmptyName, smallMolecule, Dna, reactionNum, physicalEntityNum, smallMoleculeNum, DnaNum, degradationNum);
			int tcount=0;
			for(int j=0; j<temp.size(); j++)
			{
			   tcount += temp[j];			
			}		
			if(tcount==0)
				return;
			
			manuLocateCompartment(compartmentPos);	

			tname = _name + "5catalysisControl.txt"; sname= tname.toStdString();  ename=sname.c_str();
			_scene->pwdParser->read5catalysisANDcontrol(ename, _Catalysis, _5Control);

			tname = _name + "6complex.txt"; sname= tname.toStdString();  ename=sname.c_str();
			complexNum = _scene->pwdParser->read6complex(ename, _complexName, _scene->_complexContain[m_pathwayID], _scene->CompartmentContain[m_pathwayID], _scene->_compartmentName[m_pathwayID]);
			
			//processComplexContains(_scene->_complexContain[m_pathwayID]);

			processComplexContains(_scene->_complexContain[m_pathwayID]);

			tname = _name + "7protein.txt"; sname= tname.toStdString();  ename=sname.c_str();
			//proteinNum = read7protein(ename, _scene->_proteinName[m_pathwayID]);
			proteinNum = _scene->pwdParser->read7protein(ename, _proteinName, _scene->CompartmentContain[m_pathwayID], _scene->_compartmentName[m_pathwayID], compartmentPos);
			
		
			tname = _name + "8convertedEntity.txt"; sname= tname.toStdString();  ename=sname.c_str();
			_scene->pwdParser->read8convertedEntity(ename, proteinNum, complexNum, physicalEntityNum, _8convertedProtein, _8convertedComplex, _8convertedEntity, _scene->CompartmentContain[m_pathwayID], _scene->_compartmentName[m_pathwayID]);

			tname = _name + "9smallEntity.txt"; sname= tname.toStdString();  ename=sname.c_str();
			_scene->pwdParser->read9smallEntity(ename,  _smallMoleculeName, _DnaName, _scene->CompartmentContain[m_pathwayID], _scene->_compartmentName[m_pathwayID], smallMoleculeNum, DnaNum);		

			crossTalk = _scene->pwdParser->matchSymbol(_scene->crossTalkSymbol,  _proteinName);
			rateLimit = _scene->pwdParser->matchSymbol(_scene->rateLimitSymbol,  _proteinName);
			
			EmptyNum=_EmptyName.size();

			multiplyDna(_DnaName);
			multiplySmallMolecule(_smallMoleculeName);		
			//smallMoleculetoNewName(_scene->_biochemicalReaction[m_pathwayID], _smallMoleculeName, smallMolecule);

			smallMoleculeNum = _smallMoleculeName.size()==0?0:_smallMoleculeName.size()-1; 
			DnaNum = _DnaName.size()==0?0:_DnaName.size()-1;


			complexNum = complexNum +1;   proteinNum = proteinNum +1;   smallMoleculeNum = smallMoleculeNum+1;
			DnaNum = DnaNum + 1;          EmptyNum = EmptyNum+1;        reactionNum = reactionNum+1;             physicalEntityNum = physicalEntityNum+1;
			ANodeNum = ANodeNum+1;        degradationNum = degradationNum+1;

			QRectF rect(-1000,-1000,0,0);
			vector<QRectF> temp1(complexNum, rect);
			complexPos=temp1;

			vector<QRectF> temp2(proteinNum, rect);
			proteinPos=temp2;

			vector<QRectF> temp3(smallMoleculeNum, rect);
			smallMoleculePos=temp3;

			vector<QRectF> temp44(DnaNum, rect);
			DnaPos=temp44;

			vector<QRectF> temp55(EmptyNum, rect);
			EmptyPos=temp55;

			vector<QRectF> temp5(reactionNum, rect);
			reactionPos=temp5;

			vector<QRectF> temp77(degradationNum, rect);
			degradationPos=temp77;

			vector<QRectF> temp66(physicalEntityNum, rect);
			physicalEntityPos=temp66;

			ANodeNum=0;
			vector<QRectF> temp99(ANodeNum, rect);
			ANodePos=temp99;
					
			_scene->pwdParser->getNameforConverted(_8convertedProtein, _8convertedComplex, _8convertedEntity, _complexName, _proteinName, _physicalEntityName, physicalEntityNum);
			locateCompartmentforExtraItems( _complexName, _physicalEntityName,  _proteinName,  _reactionName,	 _smallMoleculeName,  _DnaName, _scene->CompartmentContain[m_pathwayID], _scene->_compartmentName[m_pathwayID]);
			
			QFile file1("before_ParserTraceSteps()");
			file1.open(QIODevice::WriteOnly | QIODevice::Truncate);
			file1.close();

			ParserTraceSteps();
			
			updateNodeNums(edge, proteinNum, complexNum, physicalEntityNum, smallMoleculeNum, DnaNum, EmptyNum, _proteinName, _complexName, _physicalEntityName, _smallMoleculeName, _DnaName, _EmptyName, _scene->_compartmentName[m_pathwayID], _scene->CompartmentContain[m_pathwayID], proteinPos, complexPos, physicalEntityPos, smallMoleculePos, DnaPos, EmptyPos);
			compartmentNum=_scene->_compartmentName[m_pathwayID].size();
			
			_protein.resize(proteinNum);
			_complex.resize(complexNum);	
			_entity.resize(physicalEntityNum);
			_ANode.resize(ANodeNum);
			_smallMolecule.resize(smallMoleculeNum);
			_Dna.resize(DnaNum);
			_reaction.resize(reactionNum);
			_compartment.resize(compartmentNum);
			_Empty.resize(EmptyNum);

			_whole=false;		
			
			CompartmentContain=_scene->CompartmentContain[m_pathwayID];
			//vector<QRectF> reactionPos1;
	}	
	itemGrid=recordItemGrid(visibleItemNum, visibleCompartmentNum);	
	
	dCenter=QPoint(0,0);	

	draggingItem=0;  //PenisOn=0; =groupReadyforDraggin
	
	_pathBubbles.clear();
	_groupBubbles.clear();
	_reactionBubbles.clear();
	_codeBubbles.clear();

	x0y0=QPoint(-this->Width()/2.0,-this->Height()/2.0);	
	x1y1=QPoint(this->Width()/2.0,this->Height()/2.0);

	QString pathwayName=_curFileName;
	//tindex1 = pathwayName.lastIndexOf(".");
	int tindex0 = pathwayName.lastIndexOf("/");		
	//pathwayName.remove(tindex1+1,pathwayName.size());	
	pathwayName.remove(0, tindex0+1);
	int idrecord=-1;	
	bool flag=false;
	for (int i = 0; i < _scene->_pathwayNameD.size(); i ++)		
	{
		
		if(_scene->_pathwayNameD[i].compare(_curFileName)==0 )
		{   
			idrecord=i;	
			flag=true;
			break;
		}
	}

	if(idrecord<0 || !toplevel)
	{	   
	   idrecord=_scene->_pathwayNameD.size();	
	}
	
	_scene->_pathwayNameD.push_back(_curFileName);	
	_scene->_complexNameD.push_back(_complexName);
	_scene->_physicalEntityNameD.push_back(_physicalEntityName);	
	_scene->_ANodeName.push_back(_ANodeName);
	_scene->_proteinNameD.push_back(_proteinName);
	_scene->_reactionNameD.push_back(_reactionName);
	_scene->_degradationNameD.push_back(_degradationName);		
	_scene->_smallMoleculeNameD.push_back(_smallMoleculeName);		
	_scene->_EmptyNameD.push_back(_EmptyName);		
	_scene->_DnaNameD.push_back(_DnaName);	
	
	if(toplevel )
	{
		dataID=idrecord;
		dataID_1 = dataID;
	}
	else
    {
		dataID=idrecord;
	    dataID_1 = _scene->_pathBubbles[_parentPathBubbleID]->dataID_1;
	}

	//work right place to set -scale 
	_scale=1.3; //work for 
	_scene->_ANodeContain.resize(_scene->_proteinPos.size());
	if(toplevel)
	{		
	
		putReactionToCompartment(edge, _scene->CompartmentContain[m_pathwayID]);	//for large pathway
		QString pathName = _scene->pwdParser->getPathGraphHie(_pathName); 		
        savePathWayforTreeRing_1(pathName, edge);
	}
	_ScaleBackup=_scale;
}



bool PathBubble1::PathWayDataInitiation()
{
	currentPathwayGraphit=pathwayGraphStored.end();
	currentHighlightedit=highlightedStored.end();
	currentSharedit=sharedStored.end();
	currentColoredit=coloredStored.end();
	currentLinkedit=linkedStored.end();
	currentLinkedinHeadit=linkedStoredinHead.end();
	currentDifferedit=differedStored.end();
	currentExpressedit=expressedStored.end();
	currentEventit=eventStored.end();

	stepNum=0;
	reactionNum = 0;
	complexNum = 0;
	proteinNum = 0;
	smallMoleculeNum = 0;
	DnaNum = 0;
	EmptyNum=0;
	dragout=0;	

	bool record=drawSmallmolecule;	
	
	vector<QString> temp6; 	
	_scene->_quantityName.push_back(temp6);
	/*vector<QString> temp8;  
	temp8.push_back("");*/

	vector<vector<QString>> _complexName, _physicalEntityName; //start from 1
	vector<vector<QString>> _proteinName, _reactionName,_degradationName;	//start from 1
	vector<vector<QString>> _smallMoleculeName, _EmptyName, _DnaName, _ANodeName;  //start from 1

	
	 vector<vector<QString>> temp9,temp10; 	
	 _scene->_geneInfo.push_back(temp10);

	vector<vector<vector<int>>> temp4;  
	_scene->_biochemicalReaction.push_back(temp4);
	_scene->_degradation.push_back(temp4);
	
	vector<set<vector<int>>> tempx;
	_scene->CompartmentContain.push_back(tempx);
	_scene->_compartmentName.push_back(temp9);

	vector<vector<float>> temp21;	
	_scene->_quantity.push_back(temp21);
	_scene->_minmax.push_back(temp21);

	vector<vector<int>> temp33;
	_scene->_complexContain.push_back(temp33);
	_whole=false;	
	bool preComputed=true;
	if(toplevel&&_name.size()!=0)
	{	
		drawSmallmolecule=false; // keqin
		//drawSmallmolecule=true;
		//update nodes's nume according to edges (sometimes, there are protein ids which is not in 7protein file but in reaction files)
		//_pathName=="Urea cycle" || _pathName=="Acetylcholine Binding And Downstream Events" || _pathName== "Acetylation" || 	
		//vector<vector<int>> Edge;//_Edge///*, Edge)
		if( !pathWayRead(edge, _complexName, _physicalEntityName, _proteinName, _reactionName, _degradationName, _smallMoleculeName, _EmptyName, _DnaName, _ANodeName, _scene->_compartmentName[m_pathwayID]) || !pathWayPoiRead(complexPos_0, proteinPos_0, smallMoleculePos_0, DnaPos_0, EmptyPos_0, reactionPos_0, physicalEntityPos_0, compartmentPos_0, ANodePos_0, CompartmentContain_0, _scale, dCenter, false)  )
		{
			readOriPathWayData(_complexName, _physicalEntityName, _proteinName, _reactionName, _degradationName, _smallMoleculeName, _EmptyName, _DnaName, _ANodeName);
			
			preComputed=false;
			CompartmentContain=_scene->CompartmentContain[m_pathwayID];
			ParserTraceSteps();
			if(updateNodeNums(edge, proteinNum, complexNum, physicalEntityNum, smallMoleculeNum, DnaNum, EmptyNum, _proteinName, _complexName, _physicalEntityName, _smallMoleculeName, _DnaName, _EmptyName, _scene->_compartmentName[m_pathwayID],_scene->CompartmentContain[m_pathwayID], proteinPos, complexPos, physicalEntityPos, smallMoleculePos, DnaPos, EmptyPos))
			{
				compartmentNum=_scene->_compartmentName[m_pathwayID].size();
				_protein.resize(proteinNum);
				_complex.resize(complexNum);	
				_entity.resize(physicalEntityNum);
				_ANode.resize(ANodeNum);
				_smallMolecule.resize(smallMoleculeNum);
				_Dna.resize(DnaNum);
				_reaction.resize(reactionNum);
				_compartment.resize(compartmentNum);
				_Empty.resize(EmptyNum);
					
			}

			preLocateNodes(complexPos,proteinPos,smallMoleculePos);

			updateVisibleNodes(); 	
			
			QFile file1("before_HierarchicalLayout()");
			file1.open(QIODevice::WriteOnly | QIODevice::Truncate);
			file1.close();

			vector<QRectF> reactionPos1;
			HierarchicalLayout(_whole, reactionPos1);	
			RemoveDisabledNodes(_whole, _VertexInfo, _Vertex, Rank0, Ranks);				
				
			removeNotContainedinCompartment(CompartmentContain, compartmentPos, complexPos, proteinPos, smallMoleculePos, DnaPos, reactionPos, degradationPos, physicalEntityPos, EmptyPos, ANodePos);
			HierarchicalTreeNodeLocation(_whole, _VertexInfo, Rank0, Ranks);	
			putReactionToCompartment(_reaction, _scene->CompartmentContain[m_pathwayID]);
				
			updateNodeLocation(Ranks,_VertexInfo, m_pathwayID, _scene->CompartmentContain, CompartmentContain_0); //assign them to compartment according to anks);	vector<set<vector<int>>> &CompartmentContain							
							
        }
		_ScaleBackup=_scale;
		
		
		updateBubbleSize();		
		resetOriRect();		
			
		if(!preComputed)
		{
			complexPos_0=complexPos, proteinPos_0=proteinPos, smallMoleculePos_0=smallMoleculePos, DnaPos_0=DnaPos, reactionPos_0=reactionPos, degradationPos_0=degradationPos, physicalEntityPos_0=physicalEntityPos, ANodePos_0=ANodePos, compartmentPos_0=compartmentPos, EmptyPos_0=EmptyPos; 					
			CompartmentContain_0=_scene->CompartmentContain[m_pathwayID];			
		}

		//removeNotContainedinCompartment(CompartmentContain_0, compartmentPos_0, complexPos_0, proteinPos_0, smallMoleculePos_0, DnaPos_0, reactionPos_0, degradationPos_0, physicalEntityPos_0, EmptyPos_0, ANodePos_0);
		drawSmallmolecule=record;
		redrawPathWay_1(_Catalysis, _5Control);	
		
		if(drawSmallmolecule)
		{
			CompartmentContain=CompartmentContain_1;		_scene->CompartmentContain[m_pathwayID]=CompartmentContain_1;
			complexPos=complexPos_1, proteinPos=proteinPos_1, smallMoleculePos=smallMoleculePos_1, DnaPos=DnaPos_1, reactionPos=reactionPos_1, degradationPos=degradationPos_1, physicalEntityPos=physicalEntityPos_1, ANodePos=ANodePos_1, compartmentPos=compartmentPos_1, EmptyPos=EmptyPos; 				
		}
		else
		{
			CompartmentContain=CompartmentContain_0;		_scene->CompartmentContain[m_pathwayID]=CompartmentContain_0;
			complexPos=complexPos_0, proteinPos=proteinPos_0, smallMoleculePos=smallMoleculePos_0, DnaPos=DnaPos_0, reactionPos=reactionPos_0, degradationPos=degradationPos_0, physicalEntityPos=physicalEntityPos_0, ANodePos=ANodePos_0, compartmentPos=compartmentPos_0, EmptyPos=EmptyPos; 				
		}	
		if(!preComputed)
		{
			shrinkData(
			complexPos_0, proteinPos_0, smallMoleculePos_0, DnaPos_0, EmptyPos_0, reactionPos_0, physicalEntityPos_0, compartmentPos_0, ANodePos_0, CompartmentContain_0,
			complexPos_1, proteinPos_1, smallMoleculePos_1, DnaPos_1, EmptyPos_1, reactionPos_1, physicalEntityPos_1, compartmentPos_1, ANodePos_1, CompartmentContain_1,	
			_complexName, _physicalEntityName, _proteinName, _reactionName, _degradationName, _smallMoleculeName, _EmptyName, _DnaName, _ANodeName,
			edge);
		}

		_scene->backupItemPos(m_pathwayID, complexPos, proteinPos, smallMoleculePos, DnaPos, EmptyPos, reactionPos, physicalEntityPos, compartmentPos, ANodePos, _scale, dCenter, drawSmallmolecule);	
		_scene->backupEdge(m_pathwayID, edge);	
		
	}//if(toplevel&&_name.size()!=0)	

	itemGrid=recordItemGrid(visibleItemNum, visibleCompartmentNum);	
	
	dCenter=QPoint(0,0);	
	draggingItem=0;  //PenisOn=0; =groupReadyforDraggin
	
	_pathBubbles.clear();
	_groupBubbles.clear();
	_reactionBubbles.clear();
	_codeBubbles.clear();

	x0y0=QPoint(-this->Width()/2.0,-this->Height()/2.0);	
	x1y1=QPoint(this->Width()/2.0,this->Height()/2.0);

	QString pathwayName=_curFileName;
	int tindex0 = pathwayName.lastIndexOf("/");		
	pathwayName.remove(0, tindex0+1);
	int idrecord=-1;	//get rid of name storage issues
	bool flag=false;
	
    if(idrecord<0 || !toplevel)
	{	   
	   idrecord=_scene->_pathwayNameD.size();	
	}
	
	_scene->_pathwayNameD.push_back(_curFileName);	
	_scene->_complexNameD.push_back(_complexName);
	_scene->_physicalEntityNameD.push_back(_physicalEntityName);	
	_scene->_ANodeName.push_back(_ANodeName);
	_scene->_proteinNameD.push_back(_proteinName);
	_scene->_reactionNameD.push_back(_reactionName);
	_scene->_degradationNameD.push_back(_degradationName);		
	_scene->_smallMoleculeNameD.push_back(_smallMoleculeName);		
	_scene->_EmptyNameD.push_back(_EmptyName);		
	_scene->_DnaNameD.push_back(_DnaName);	

	if(toplevel )
	{
		dataID=idrecord;
		dataID_1 = dataID;
	}
	else
    {
		dataID=idrecord;
		if(_scene ->_pathBubbles.size()>_parentPathBubbleID)
		{
			if(_scene->_pathBubbles[_parentPathBubbleID])
	           dataID_1 = _scene->_pathBubbles[_parentPathBubbleID]->dataID_1;
		    else
		 	   dataID=0;
		}
		else
			dataID=0;
	}
	

	
	if(drawOrthology)
	{
		pwdParser->readOrthologySymbol(getOrthFileName(), orthologySymbol);
		orthology = pwdParser->matchSymbol(orthologySymbol,  _scene->_proteinNameD[dataID]);							
	}
	crossTalk = _scene->pwdParser->matchSymbol(_scene->crossTalkSymbol,  _proteinName);
	rateLimit = _scene->pwdParser->matchSymbol(_scene->rateLimitSymbol,  _proteinName);	
	
	
	
	//work right place to set -scale 	
	_scene->_ANodeContain.resize(_scene->_proteinPos.size());
	if(toplevel)
	{		
		if(!preComputed)
		    _scale=1.3; //work for 

		if(!preComputed)
		{
			drawSmallmolecule=false;
			
			if(drawSmallmolecule)
			{
				complexPos=complexPos_1, proteinPos=proteinPos_1, smallMoleculePos=smallMoleculePos_1, DnaPos=DnaPos_1, reactionPos=reactionPos_1, degradationPos=degradationPos_1, physicalEntityPos=physicalEntityPos_1, ANodePos=ANodePos_1, EmptyPos=EmptyPos_1, compartmentPos=compartmentPos_1; 
				CompartmentContain=CompartmentContain_1;				_scene->CompartmentContain[m_pathwayID]=CompartmentContain_1;
			}
			else
			{
			   complexPos=complexPos_0, proteinPos=proteinPos_0, smallMoleculePos=smallMoleculePos_0, DnaPos=DnaPos_0, reactionPos=reactionPos_0, degradationPos=degradationPos_0, physicalEntityPos=physicalEntityPos_0, ANodePos=ANodePos_0, EmptyPos=EmptyPos_0, compartmentPos=compartmentPos_0; 
			   CompartmentContain=CompartmentContain_0;				_scene->CompartmentContain[m_pathwayID]=CompartmentContain_0;	   
			}
			float sRatio = getGraphToBePaint();
			int AFS=aveFontSize;
			switch(AFS)
			{
			   case 8: _scale=1.45; break;
			   case 9: _scale=1.37; break;			   
			   case 10: _scale=1.30; break;			   
			   case 11: _scale=1.25; break;			   		   
			   case 12: _scale=1.175; break;			   		   
			   case 13: _scale=1.1; break;			   		    		   
			   case 14: _scale=1.025; break;			   		    		   
			   case 15: _scale=0.96; break;			   		    		   
			   case 16: _scale=0.93; break;
			   default: _scale=1.3; break;			   		    
			}	
			if(AFS<8) 
				_scale=1.4;
			else if(AFS>16) 
				_scale=0.90;

			_ScaleBackup=_scale;	
						
			drawSmallmolecule=true;
			
			if(drawSmallmolecule)
			{
				complexPos=complexPos_1, proteinPos=proteinPos_1, smallMoleculePos=smallMoleculePos_1, DnaPos=DnaPos_1, reactionPos=reactionPos_1, degradationPos=degradationPos_1, physicalEntityPos=physicalEntityPos_1, ANodePos=ANodePos_1, EmptyPos=EmptyPos_1, compartmentPos=compartmentPos_1; 
				CompartmentContain=CompartmentContain_1;				_scene->CompartmentContain[m_pathwayID]=CompartmentContain_1;
			}
			else
			{
			   complexPos=complexPos_0, proteinPos=proteinPos_0, smallMoleculePos=smallMoleculePos_0, DnaPos=DnaPos_0, reactionPos=reactionPos_0, degradationPos=degradationPos_0, physicalEntityPos=physicalEntityPos_0, ANodePos=ANodePos_0, EmptyPos=EmptyPos_0, compartmentPos=compartmentPos_0; 
			   CompartmentContain=CompartmentContain_0;				_scene->CompartmentContain[m_pathwayID]=CompartmentContain_0;	   
			}
			if(drawSmallmolecule)
			{
				_scale=1.3; //work for 
				float sRatio = getGraphToBePaint();
				int AFS=aveFontSize;
				switch(AFS)
				{
				   case 8: _scale=1.45; break;
				   case 9: _scale=1.37; break;			   
				   case 10: _scale=1.30; break;			   
				   case 11: _scale=1.25; break;			   		   
				   case 12: _scale=1.175; break;			   		   
				   case 13: _scale=1.1; break;			   		    		   
				   case 14: _scale=1.025; break;			   		    		   
				   case 15: _scale=0.96; break;			   		    		   
				   case 16: _scale=0.93; break;
				   default: _scale=1.3; break;			   		    
				}	
				if(AFS<8) 
					_scale=1.4;
				else if(AFS>16) 
					_scale=0.90;		
						
				_scale=_scale* (0.85+0.15 * (1-sRatio));
				_ScaleBackup_S=_scale;
			}
		
			pathWayRecord(edge, _complexName, _physicalEntityName, _proteinName,_reactionName, _degradationName, _smallMoleculeName, _EmptyName, _DnaName, _ANodeName,  _scene->_compartmentName[m_pathwayID]); 
			pathWayPoiRecord(complexPos_0, proteinPos_0, smallMoleculePos_0, DnaPos_0, EmptyPos_0, reactionPos_0, physicalEntityPos_0, compartmentPos_0, ANodePos_0, CompartmentContain_0, _ScaleBackup, dCenter, false);
			pathWayPoiRecord(complexPos_1, proteinPos_1, smallMoleculePos_1, DnaPos_1, EmptyPos_1, reactionPos_1, physicalEntityPos_1, compartmentPos_1, ANodePos_1, CompartmentContain_1, _ScaleBackup_S, dCenter, true); //_scale may not work for graph with small moleculer		

		}
		drawSmallmolecule=record;		
		if(drawSmallmolecule)
		{
			CompartmentContain=CompartmentContain_1;		_scene->CompartmentContain[m_pathwayID]=CompartmentContain_1;
			complexPos=complexPos_1, proteinPos=proteinPos_1, smallMoleculePos=smallMoleculePos_1, DnaPos=DnaPos_1, reactionPos=reactionPos_1, degradationPos=degradationPos_1, physicalEntityPos=physicalEntityPos_1, ANodePos=ANodePos_1, compartmentPos=compartmentPos_1, EmptyPos=EmptyPos; 				
			_scale=_ScaleBackup_S;
		}
		else
		{
			CompartmentContain=CompartmentContain_0;		_scene->CompartmentContain[m_pathwayID]=CompartmentContain_0;
			complexPos=complexPos_0, proteinPos=proteinPos_0, smallMoleculePos=smallMoleculePos_0, DnaPos=DnaPos_0, reactionPos=reactionPos_0, degradationPos=degradationPos_0, physicalEntityPos=physicalEntityPos_0, ANodePos=ANodePos_0, compartmentPos=compartmentPos_0, EmptyPos=EmptyPos; 				
			_scale=_ScaleBackup;
		}
		loadPreStoredGraph();
		getGraphToBePaint();
		//_scene->backupItemPos(m_pathwayID, complexPos, proteinPos, smallMoleculePos, DnaPos, EmptyPos, reactionPos, physicalEntityPos, compartmentPos, ANodePos, _scale, dCenter, drawSmallmolecule);				
		QString pathName = _scene->pwdParser->getPathGraphHie(_pathName); 	
	}
	return true;
}

void PathBubble1::loadPreStoredGraph()
{
    if(lastStoredfileName.size()==0)
	{
		lastStoredfileName = _scene->pwdParser->loadPathGraphPreparation(_pathName, drawSmallmolecule);
	}		
	bool hflag=false;
	if(lastStoredfileName!="data/Reactome_Pathway_Data/pathwayTable" || lastStoredfileName.size()!=0)
	{
		if(!lastStoredfileName.contains(".xml"))
		    lastStoredfileName=lastStoredfileName+".xml";
			hflag=readPathWay(lastStoredfileName, complexPos, proteinPos, smallMoleculePos, DnaPos, reactionPos, physicalEntityPos, ANodePos, edge);	
			_scene->resetItemPos_afterReadPathway(m_pathwayID, complexPos, proteinPos,  smallMoleculePos,  DnaPos,  EmptyPos,  reactionPos,  physicalEntityPos, compartmentPos,  ANodePos);
			for (int i = 0; i < _scene->_pathBubbles.size(); i ++)		
			{
			int size=_scene->_pathBubbles.size();
			PathBubble1 * tmp =_scene->_pathBubbles[i];			
			if(_scene->_pathBubbles[i])
			if(_scene->_pathBubbles[i]!=NULL && _scene->_pathBubbles[i]!=this)
			if(_scene->_pathBubbles[i]->m_pathwayID>=0 && _scene->_pathBubbles[i]->m_pathwayID < _scene->_pathBubbles.size())
			if(_scene->_pathBubbles[i]->getPathwayName().compare(_pathName)==0 )
			{   
					ANodeRect.resize(ANodePos.size());
					for(int i=0; i<ANodePos.size(); i++)
					{
						ANodePos[i] = QRectF(-2000,-2000,-1000,-1000);
						ANodeRect[i] = QRectF(-2000,-2000,-1000,-1000);
					}	
					break;
			}
		}			
	}		
	if(hflag)
	{
		int tindex = lastStoredfileName.lastIndexOf("/");		
		lastStoredfileName.remove(0, tindex+1);
		lastStoredfileName.remove(".xml");			
	}
	if(toplevel)
	{
		if(hflag)
			_labelName=lastStoredfileName;
		else 
		_labelName=_pathName;
	}
	else 
		_labelName=_name;
}

void PathBubble1::shrinkData(
	vector<QRectF> &complexPos_0, vector<QRectF> &proteinPos_0, vector<QRectF> &smallMoleculePos_0, vector<QRectF> &DnaPos_0, vector<QRectF> &EmptyPos_0, vector<QRectF> &reactionPos_0, vector<QRectF> &physicalEntityPos_0, vector<QRectF> &compartmentPos_0, vector<QRectF> &ANodePos_0, vector<set<vector<int>>> &CompartmentContain_0,
	vector<QRectF> &complexPos_1, vector<QRectF> &proteinPos_1, vector<QRectF> &smallMoleculePos_1, vector<QRectF> &DnaPos_1, vector<QRectF> &EmptyPos_1, vector<QRectF> &reactionPos_1, vector<QRectF> &physicalEntityPos_1, vector<QRectF> &compartmentPos_1, vector<QRectF> &ANodePos_1, vector<set<vector<int>>> &CompartmentContain_1,	
	vector<vector<QString>> &_complexName, vector<vector<QString>> &_physicalEntityName, vector<vector<QString>> &_proteinName, vector<vector<QString>> &_reactionName, vector<vector<QString>> &_degradationName, vector<vector<QString>> &_smallMoleculeName, vector<vector<QString>> &_EmptyName, vector<vector<QString>> &_DnaName, vector<vector<QString>> &_ANodeName,
	vector<vector<int>> &edge)
{
	//recode a set of 
	vector<QRectF> complexPos0, proteinPos0, smallMoleculePos0, DnaPos0, EmptyPos0, reactionPos0, physicalEntityPos0, ANodePos0; //vector<set<vector<int>>> &CompartmentContain
	vector<QRectF> complexPos1, proteinPos1, smallMoleculePos1, DnaPos1, EmptyPos1, reactionPos1, physicalEntityPos1, ANodePos1; //vector<set<vector<int>>> &CompartmentContain
	vector<vector<QString>> complexNameT, physicalEntityNameT, proteinNameT, reactionNameT, degradationNameT, smallMoleculeNameT, EmptyNameT, DnaNameT, ANodeNameT, emptyNameT;
	set<vector<int>> itemSet;
	vector<set<vector<int>>> CompartmentContain0, CompartmentContain1;
	vector<int> item(2,0);
	vector<vector<int>> edgeT;
	
	item[0]='C';
	for(int i=0; i<complexPos_0.size(); i++)
	{
		if(complexPos_0[i].x()>-990)
		{
			item[1]=i;
			itemSet.insert(item);
		}
	}
	for(int i=0; i<complexPos_1.size(); i++)
	{
		if(complexPos_1[i].x()>-990)
		{
			item[1]=i;
			itemSet.insert(item);
		}
	}

	item[0]='P';
	for(int i=0; i<proteinPos_0.size(); i++)
	{
		if(proteinPos_0[i].x()>-990)
		{
			item[1]=i;
			itemSet.insert(item);
		}
	}
	for(int i=0; i<proteinPos_1.size(); i++)
	{
		if(proteinPos_1[i].x()>-990)
		{
			item[1]=i;
			itemSet.insert(item);
		}
	}
	
	item[0]='S';
	for(int i=0; i<smallMoleculePos_0.size(); i++)
	{
		if(smallMoleculePos_0[i].x()>-990)
		{
			item[1]=i;
			itemSet.insert(item);
		}
	}
	for(int i=0; i<smallMoleculePos_1.size(); i++)
	{
		if(smallMoleculePos_1[i].x()>-990)
		{
			item[1]=i;
			itemSet.insert(item);
		}
	}
	
	item[0]='D';
	for(int i=0; i<DnaPos_0.size(); i++)
	{
		if(DnaPos_0[i].x()>-990)
		{
			item[1]=i;
			itemSet.insert(item);
		}
	}
	for(int i=0; i<DnaPos_1.size(); i++)
	{
		if(DnaPos_1[i].x()>-990)
		{
			item[1]=i;
			itemSet.insert(item);
		}
	}


	item[0]='N';
	for(int i=0; i<EmptyPos_0.size(); i++)
	{
		if(EmptyPos_0[i].x()>-990)
		{
			item[1]=i;
			itemSet.insert(item);
		}
	}
	for(int i=0; i<EmptyPos_1.size(); i++)
	{
		if(EmptyPos_1[i].x()>-990)
		{
			item[1]=i;
			itemSet.insert(item);
		}
	}

	item[0]='R';
	for(int i=0; i<reactionPos_0.size(); i++)
	{
		if(reactionPos_0[i].x()>-990)
		{
			item[1]=i;
			itemSet.insert(item);
		}
	}
	for(int i=0; i<reactionPos_1.size(); i++)
	{
		if(reactionPos_1[i].x()>-990)
		{
			item[1]=i;
			itemSet.insert(item);
		}
	}

	item[0]='E';
	for(int i=0; i<physicalEntityPos_0.size(); i++)
	{
		if(physicalEntityPos_0[i].x()>-990)
		{
			item[1]=i;
			itemSet.insert(item);
		}
	}
	for(int i=0; i<physicalEntityPos_1.size(); i++)
	{
		if(physicalEntityPos_1[i].x()>-990)
		{
			item[1]=i;
			itemSet.insert(item);
		}
	}

	item[0]='L';
	for(int i=0; i<ANodePos_0.size(); i++)
	{
		if(ANodePos_0[i].x()>-990)
		{
			item[1]=i;
			itemSet.insert(item);
		}
	}
	for(int i=0; i<ANodePos_1.size(); i++)
	{
		if(ANodePos_1[i].x()>-990)
		{
			item[1]=i;
			itemSet.insert(item);
		}
	}
	int countC,countE,countP, countS, countD, countR,countA, countN;
	countC = countE = countP = countS = countD = countA =countR= countN=0;

	vector<int> CID(complexPos_0.size(), -1), EID(physicalEntityPos_0.size(), -1), PID(proteinPos_0.size(),-1), SID(smallMoleculePos_0.size(),-1), DID(DnaPos_0.size(), -1), RID(reactionPos_0.size(), -1), AID(ANodePos_0.size(), -1), NID(EmptyPos_0.size(), -1);

	for(set<vector<int>>::iterator it=itemSet.begin(); it!=itemSet.end(); it++)
	{
	      item = *it;
		  int type = item[0], id=item[1];		 
	      switch(type)
		  {
			    case 'C': complexPos0.push_back(complexPos_0[id]);  complexPos1.push_back(complexPos_1[id]);  complexNameT.push_back(_complexName[id]);  	CID[id]=countC++; break;	
				case 'E': physicalEntityPos0.push_back(physicalEntityPos_0[id]);  physicalEntityPos1.push_back(physicalEntityPos_1[id]);  physicalEntityNameT.push_back(_physicalEntityName[id]);   EID[id]=countE++;	  break;				
				case 'S': smallMoleculePos0.push_back(smallMoleculePos_0[id]);  smallMoleculePos1.push_back(smallMoleculePos_1[id]);  smallMoleculeNameT.push_back(_smallMoleculeName[id]);	   SID[id]=countS++; break;							
				case 'P': proteinPos0.push_back(proteinPos_0[id]);  proteinPos1.push_back(proteinPos_1[id]);  proteinNameT.push_back(_proteinName[id]);  	 PID[id]=countP++; break;		 
				case 'D': DnaPos0.push_back(DnaPos_0[id]);  DnaPos1.push_back(DnaPos_1[id]);  DnaNameT.push_back(_DnaName[id]);	    DID[id]=countD++; break;		
				case 'N': EmptyPos0.push_back(EmptyPos_0[id]);  EmptyPos1.push_back(EmptyPos_1[id]);  EmptyNameT.push_back(_EmptyName[id]);	   EID[id]=countE++; break;	
				case 'R': reactionPos0.push_back(reactionPos_0[id]);  reactionPos1.push_back(reactionPos_1[id]);  reactionNameT.push_back(_reactionName[id]);  	RID[id]=countR++;  break;	
				case 'L': ANodePos0.push_back(ANodePos_0[id]);  ANodePos1.push_back(ANodePos_1[id]);  ANodeNameT.push_back(_ANodeName[id]);	  AID[id]=countA++;  break;				
		  }	
	}
	edgeT=edge;
	for(int i=0; i<edge.size(); i++)
	{
		int type, id, id1;
		int span=2;
		if(edge[i][2]=='R'&&edge[i][4]=='R')
			span=4;
		for(int j=0; j<edge[i].size()-1; j=j+span)
		{
			type=edge[i][j], id=edge[i][j+1];
            switch(type)
			{
				case 'C': id1=CID[id]; break;	
				case 'E': id1=EID[id]; break;	
				case 'S': id1=SID[id]; break;	
				case 'P': id1=PID[id]; break;	
				case 'D': id1=DID[id]; break;	
				case 'N': id1=NID[id]; break;	
				case 'R': id1=RID[id]; break;	
				case 'L': id1=AID[id]; break;	
			}	
			edgeT[i][j+1]=id1; 
		}
	 }
	
	for(int i=0; i<CompartmentContain_0.size(); i++)
	{
		set<vector<int>> cSet;
		for(set<vector<int>>::iterator it = CompartmentContain_0[i].begin(); it != CompartmentContain_0[i].end(); it++)
		{   
			vector<int> temp=*it;
			int type, id, id1;
			type=temp[0], id=temp[1];
			if(id>=0)
			{
				switch(type)
				{
					case 'C': id1=CID[id]; break;	
					case 'E': id1=EID[id]; break;	
					case 'S': id1=SID[id]; break;	
					case 'P': id1=PID[id]; break;	
					case 'D': id1=DID[id]; break;	
					case 'N': id1=NID[id]; break;	
					case 'R': id1=RID[id]; break;	
					case 'L': id1=AID[id]; break;	
				}	
			}
			else
				id1=-1;
			temp[1]=id1;
			if(id1>=0)
			   cSet.insert(temp);		    
			
		}
		CompartmentContain0.push_back(cSet);
	}	

	for(int i=0; i<CompartmentContain_1.size(); i++)
	{
		set<vector<int>> cSet;
		for(set<vector<int>>::iterator it = CompartmentContain_1[i].begin(); it != CompartmentContain_1[i].end(); it++)
		{   
			vector<int> temp=*it;
			int type, id, id1;
			type=temp[0], id=temp[1];
			if(id>=0)
			{
				switch(type)
				{
					case 'C': id1=id>=CID.size()?-1:CID[id]; break;	//bug here when loading 3296482
					case 'E': id1=id>=EID.size()?-1:EID[id]; break;	//bug when loading 190236
					case 'S': id1=id>=SID.size()?-1:SID[id]; break;	
					case 'P': id1=id>=PID.size()?-1:PID[id]; break;	
					case 'D': id1=id>=DID.size()?-1:DID[id]; break;	
					case 'N': id1=id>=NID.size()?-1:NID[id]; break;	
					case 'R': id1=id>=RID.size()?-1:RID[id]; break;	
					case 'L': id1=id>=AID.size()?-1:AID[id]; break;	
				}	
			}
			else
				id1=-1;

			temp[1]=id1;
			if(id1>=0)
			   cSet.insert(temp);		    
			
		}
		CompartmentContain1.push_back(cSet);
	}	

	CompartmentContain_0.clear(), CompartmentContain_1.clear();
	complexPos_0.clear();  complexPos_1.clear();  _complexName.clear();
	physicalEntityPos_0.clear();  physicalEntityPos_1.clear();  _physicalEntityName.clear();
	smallMoleculePos_0.clear();  smallMoleculePos_1.clear();  _smallMoleculeName.clear();
	proteinPos_0.clear();  proteinPos_1.clear();  _proteinName.clear();
	DnaPos_0.clear();   DnaPos_1.clear();  _DnaName.clear();
	EmptyPos_0.clear();  EmptyPos_1.clear();  _EmptyName.clear();
	reactionPos_0.clear();  reactionPos_1.clear();  _reactionName.clear();
	ANodePos_0.clear();  ANodePos_1.clear();  _ANodeName.clear();
	edge.clear();
	

	complexPos_0 = complexPos0;  complexPos_1=complexPos1;  _complexName = complexNameT;
	physicalEntityPos_0 = physicalEntityPos0;  physicalEntityPos_1=physicalEntityPos1;  _physicalEntityName = physicalEntityNameT;
	smallMoleculePos_0 = smallMoleculePos0;  smallMoleculePos_1=smallMoleculePos1;  _smallMoleculeName = smallMoleculeNameT;
	proteinPos_0 = proteinPos0;  proteinPos_1=proteinPos1;  _proteinName = proteinNameT;
	DnaPos_0 =  DnaPos0;   DnaPos_1= DnaPos1;  _DnaName =  DnaNameT;
	EmptyPos_0 = EmptyPos0;  EmptyPos_1=EmptyPos1;  _EmptyName = EmptyNameT;
	reactionPos_0 = reactionPos0;  reactionPos_1=reactionPos1;  _reactionName = reactionNameT;
	ANodePos_0 = ANodePos0;  ANodePos_1=ANodePos1;  _ANodeName = ANodeNameT;
	edge= edgeT;
	CompartmentContain_0 = CompartmentContain0, CompartmentContain_1 = CompartmentContain1;


	if(drawSmallmolecule)
	{
		complexPos=complexPos_1, proteinPos=proteinPos_1, smallMoleculePos=smallMoleculePos_1, DnaPos=DnaPos_1, reactionPos=reactionPos_1, degradationPos=degradationPos_1, physicalEntityPos=physicalEntityPos_1, ANodePos=ANodePos_1, EmptyPos=EmptyPos_1, compartmentPos=compartmentPos_1; 
		CompartmentContain=CompartmentContain_1;				_scene->CompartmentContain[m_pathwayID]=CompartmentContain_1;
	}
	else
	{
	   complexPos=complexPos_0, proteinPos=proteinPos_0, smallMoleculePos=smallMoleculePos_0, DnaPos=DnaPos_0, reactionPos=reactionPos_0, degradationPos=degradationPos_0, physicalEntityPos=physicalEntityPos_0, ANodePos=ANodePos_0, EmptyPos=EmptyPos_0, compartmentPos=compartmentPos_0; 
	   CompartmentContain=CompartmentContain_0;				_scene->CompartmentContain[m_pathwayID]=CompartmentContain_0;	   
	}

	/*_scene->_complexPos[m_pathwayID]=complexPos;
	_scene->_proteinPos[m_pathwayID]=proteinPos;
	_scene->_smallMoleculePos[m_pathwayID]=smallMoleculePos;

	_scene->_physicalEntityPos[m_pathwayID]=physicalEntityPos;
	_scene->_ANodePos[m_pathwayID]=ANodePos;

	_scene->_DnaPos[m_pathwayID]=DnaPos;
	_scene->_reactionPos[m_pathwayID]=reactionPos;
	_scene->_compartmentPos[m_pathwayID]=compartmentPos; 


	_scene->_ANodeName[m_pathwayID]=_ANodeName;
	_scene->_complexNameD[m_pathwayID]=_complexName; 
	_scene-> _physicalEntityNameD[m_pathwayID]=_physicalEntityName; 
	_scene->_proteinNameD[m_pathwayID]=_proteinName;
	_scene->_reactionNameD[m_pathwayID]=_reactionName;	
	_scene->_smallMoleculeNameD[m_pathwayID]=_smallMoleculeName, 
	_scene->_EmptyNameD[m_pathwayID]=_EmptyName;
	_scene->_DnaNameD[m_pathwayID]=_DnaName;  */

}
	//start from 1
bool PathBubble1::readOriPathWayData(vector<vector<QString>> &_complexName, vector<vector<QString>> &_physicalEntityName, vector<vector<QString>> &_proteinName, vector<vector<QString>> &_reactionName, vector<vector<QString>> &_degradationName, vector<vector<QString>> &_smallMoleculeName, vector<vector<QString>> &_EmptyName, vector<vector<QString>> &_DnaName, vector<vector<QString>> &_ANodeName)  
{	
        QString tname;
		const char * ename;
		string sname;
		vector<vector<int>> _8convertedProtein, _8convertedComplex, _8convertedEntity;		
	    vector<vector<int>> _stepRoot; 

		tname = _name + "1pathway.txt"; sname= tname.toStdString();  ename=sname.c_str();
		stepNum=_scene->pwdParser->read1pathway(ename, _1pathway);			

		tname = _name + "2pathwayStepOrder.txt"; sname= tname.toStdString();  ename=sname.c_str();
		stepNum=_scene->pwdParser->read2pathwayStepOrder(ename, stepNum, _pathwayStep);		

		tname = _name + "3pathwayStepReactionControl.txt"; sname= tname.toStdString();  ename=sname.c_str();
		_scene->pwdParser->read3pathwayStep_reactionAndcontrol(ename, stepNum, _3pathwayStepCatalysis, _3pathwayStepReaction);    	

		tname = _name + "4biochemicalReaction.txt"; sname= tname.toStdString();  ename=sname.c_str();
		vector<int> temp = _scene->pwdParser->read4biochemicalReaction(ename, _scene->_biochemicalReaction[m_pathwayID], _reactionName, _scene->_degradation[m_pathwayID], _degradationName, _EmptyName, smallMolecule, Dna, reactionNum, physicalEntityNum, smallMoleculeNum, DnaNum, degradationNum);
		int tcount=0;
		for(int j=0; j<temp.size(); j++)
		{
			tcount += temp[j];			
		}		
		
		//reactionNum = temp[0], physicalEntityNum=temp[1], smallMoleculeNum=temp[2], DnaNum=temp[3]; degradationNum=temp[3];

		manuLocateCompartment(compartmentPos);	

		tname = _name + "5catalysisControl.txt"; sname= tname.toStdString();  ename=sname.c_str();
		_scene->pwdParser->read5catalysisANDcontrol(ename, _Catalysis, _5Control);

		tname = _name + "6complex.txt"; sname= tname.toStdString();  ename=sname.c_str();
		complexNum = _scene->pwdParser->read6complex(ename, _complexName, _scene->_complexContain[m_pathwayID], _scene->CompartmentContain[m_pathwayID], _scene->_compartmentName[m_pathwayID]);
		//processComplexContains(_scene->_complexContain[m_pathwayID]);

		processComplexContains(_scene->_complexContain[m_pathwayID]);

		tname = _name + "7protein.txt"; sname= tname.toStdString();  ename=sname.c_str();
		//proteinNum = read7protein(ename, _scene->_proteinName[m_pathwayID]);
		proteinNum = _scene->pwdParser->read7protein(ename, _proteinName, _scene->CompartmentContain[m_pathwayID], _scene->_compartmentName[m_pathwayID], compartmentPos);

			
		tname = _name + "8convertedEntity.txt"; sname= tname.toStdString();  ename=sname.c_str();
		_scene->pwdParser->read8convertedEntity(ename, proteinNum, complexNum, physicalEntityNum, _8convertedProtein, _8convertedComplex, _8convertedEntity, _scene->CompartmentContain[m_pathwayID], _scene->_compartmentName[m_pathwayID]);

		tname = _name + "9smallEntity.txt"; sname= tname.toStdString();  ename=sname.c_str();
		_scene->pwdParser->read9smallEntity(ename,  _smallMoleculeName, _DnaName, _scene->CompartmentContain[m_pathwayID], _scene->_compartmentName[m_pathwayID], smallMoleculeNum, DnaNum);		
				
			
		EmptyNum=_EmptyName.size();

		multiplyDna(_DnaName);
		multiplySmallMolecule(_smallMoleculeName);		
		
		smallMoleculeNum = _smallMoleculeName.size()==0?0:_smallMoleculeName.size()-1; 
		DnaNum = _DnaName.size()==0?0:_DnaName.size()-1;

		
		complexNum = complexNum +1;   proteinNum = proteinNum +1;   smallMoleculeNum = smallMoleculeNum+1;
		DnaNum = DnaNum + 1;          EmptyNum = EmptyNum+1;        reactionNum = reactionNum+1;             physicalEntityNum = physicalEntityNum+1;
		ANodeNum = ANodeNum+1;        degradationNum = degradationNum+1;

		QRectF rect(-1000,-1000,0,0);
		vector<QRectF> temp1(complexNum, rect);
		complexPos=temp1;

		vector<QRectF> temp2(proteinNum, rect);
		proteinPos=temp2;

		vector<QRectF> temp3(smallMoleculeNum, rect);
		smallMoleculePos=temp3;

		vector<QRectF> temp44(DnaNum, rect);
		DnaPos=temp44;

		vector<QRectF> temp55(EmptyNum, rect);
		EmptyPos=temp55;

		vector<QRectF> temp5(reactionNum, rect);
		reactionPos=temp5;

		vector<QRectF> temp77(degradationNum, rect);
		degradationPos=temp77;

		vector<QRectF> temp66(physicalEntityNum, rect);
		physicalEntityPos=temp66;

		ANodeNum=0;
		vector<QRectF> temp99(ANodeNum, rect);
		ANodePos=temp99;
					
		
		compartmentNum=_scene->_compartmentName[m_pathwayID].size();
		_protein.resize(proteinNum);
		_complex.resize(complexNum);	
		_entity.resize(physicalEntityNum);
		_ANode.resize(ANodeNum);
		_smallMolecule.resize(smallMoleculeNum);
		_Dna.resize(DnaNum);
		_reaction.resize(reactionNum);
		_compartment.resize(compartmentNum);
		_Empty.resize(EmptyNum);
		_whole=false;

		_scene->pwdParser->getNameforConverted(_8convertedProtein, _8convertedComplex, _8convertedEntity, _complexName, _proteinName, _physicalEntityName, physicalEntityNum);
		locateCompartmentforExtraItems( _complexName, _physicalEntityName,  _proteinName,  _reactionName,	 _smallMoleculeName,  _DnaName, _scene->CompartmentContain[m_pathwayID], _scene->_compartmentName[m_pathwayID]);
		
		_8convertedProtein.clear(); _8convertedComplex.clear(); _8convertedEntity.clear();
		_stepRoot.clear(); 	

		if(tcount==0)
			return false;
		else 
			return true;
}

void PathBubble1::updateSharedLinkedDIffered()
{   //after a bubble ungroup or delection
	vector<PathBubble1 *> mList=_scene->extractPathBubblesinGroup(this);
    for(int j=0; j<mList.size(); j++) 
	{
		PathBubble1* pBubble = mList[j];			
		for(int m=0; m<9; m++)
		{
			int ctype;
			vector<set<int>> sharedNode;
			 vector<vector<QRectF>> nodePos;
			switch(m)
			{
				case 0: ctype='P';  sharedNode=pBubble->sharedProtein;         break;
				case 1: ctype='S';  sharedNode=pBubble->sharedSmallMolecule;   break;
				case 2: ctype='C';  sharedNode=pBubble->sharedComplex;         break;
				case 3: ctype='D';  sharedNode=pBubble->sharedDna;             break;
				case 4: ctype='R';  sharedNode=pBubble->sharedReaction;        break;
				case 5: ctype='E';  sharedNode=pBubble->sharedPhysicalEntity;  break;					
				case 6: ctype='L';  sharedNode=pBubble->sharedANode;           break;	
				case 7: ctype='M';  sharedNode=pBubble->sharedCompartment;     break;	
				case 8: ctype='W';  sharedNode=pBubble->sharedPathway;         break;	
			}		
			int size=sharedNode.size();			
			sharedNode.clear();
			sharedNode.resize(size);

			/*	vector<PathBubble1 *> mlist=mList;	 
				for(int i=0; i<mlist.size(); i++) 
				{
					
					PathBubble1* pbubble = mlist[i];		 
					if(pbubble!=this)
					{
					    //for(int i=0;i<sharedNode[pbubble->m_pathwayID].size();i++)			
						if(sharedNode.size()>pbubble->m_pathwayID && sharedNode.size()>m_pathwayID)
						{
							sharedNode[m_pathwayID].clear();							
						}
					}
					else
					{
						if(sharedNode.size()>pbubble->m_pathwayID)
						{
							set<int> temp= sharedNode[pbubble->m_pathwayID];
							for(int k=0; k<sharedNode.size(); k++)
							{
								if(k!=m_pathwayID)
								{
								    sharedNode[k].clear();								
								}
							}
						}
					}
				}*/

				switch(m)
			   {
					case 0: ctype='P';  pBubble->sharedProtein = sharedNode;          break;
					case 1: ctype='S';  pBubble->sharedSmallMolecule = sharedNode;    break;
					case 2: ctype='C';  pBubble->sharedComplex = sharedNode;          break;
					case 3: ctype='D';  pBubble->sharedDna = sharedNode;              break;
					case 4: ctype='R';  pBubble->sharedReaction = sharedNode;         break;
					case 5: ctype='E';  pBubble->sharedPhysicalEntity = sharedNode;   break;					
					case 6: ctype='L';  pBubble->sharedANode = sharedNode;            break;	
					case 7: ctype='M';  pBubble->sharedCompartment = sharedNode;      break;	
					case 8: ctype='W';  pBubble->sharedPathway = sharedNode;          break;	
			   }	

				//differed
				set < vector <int> >  differed;	
				for(int m=0; m<9; m++)
				{
					int ctype;
					vector<set<int>> differedNode;
					switch(m)
					{
						case 0: ctype='P';  differedNode=differedProtein;        break;
						case 1: ctype='S';  differedNode=differedSmallMolecule;  break;
						case 2: ctype='C';  differedNode=differedComplex;        break;
						case 3: ctype='D';  differedNode=differedDna;            break;
						case 4: ctype='R';  differedNode=differedReaction;       break;
						case 5: ctype='E';  differedNode=differedPhysicalEntity; break;	
						case 6: ctype='L';  differedNode=differedANode; break;	
						case 7: ctype='M';  differedNode=differedCompartment;    break;	
						case 8: ctype='W';  differedNode=differedPathway;        break;	
					}
					int size=differedNode.size();			
					differedNode.clear();
					differedNode.resize(size);

					/*vector<PathBubble1 *> mlist=mList;	 
					for(int i=0; i<mlist.size(); i++) 
					{					
						PathBubble1* pbubble = mlist[i];		 
						if(pbubble!=this)
						{
							//for(int i=0;i<differedNode[pbubble->m_pathwayID].size();i++)			
							if(differedNode.size()>pbubble->m_pathwayID && differedNode.size()>m_pathwayID)
							{
								differedNode[m_pathwayID].clear();							
							}
						}
						else
						{
							if(differedNode.size()>pbubble->m_pathwayID)
							{
								set<int> temp= differedNode[pbubble->m_pathwayID];
								for(int k=0; k<differedNode.size(); k++)
								{
									if(k!=m_pathwayID)
									{
										differedNode[k].clear();								
									}
								}
							}
						}
					}
					*/
					switch(m)
				   {
						case 0: ctype='P';  pBubble->differedProtein = differedNode;          break;
						case 1: ctype='S';  pBubble->differedSmallMolecule = differedNode;    break;
						case 2: ctype='C';  pBubble->differedComplex = differedNode;          break;
						case 3: ctype='D';  pBubble->differedDna = differedNode;              break;
						case 4: ctype='R';  pBubble->differedReaction = differedNode;         break;
						case 5: ctype='E';  pBubble->differedPhysicalEntity = differedNode;   break;					
						case 6: ctype='L';  pBubble->differedANode = differedNode;            break;	
						case 7: ctype='M';  pBubble->differedCompartment = differedNode;      break;	
						case 8: ctype='W';  pBubble->differedPathway = differedNode;          break;	
				   }	
				}	

				//linked
				set < vector <int> >  linked;	
				for(int m=0; m<9; m++)
				{
					int ctype;
					vector<set<int>> linkedNode;
					switch(m)
					{
						case 0: ctype='P';  linkedNode=linkedProtein;        break;
						case 1: ctype='S';  linkedNode=linkedSmallMolecule;  break;
						case 2: ctype='C';  linkedNode=linkedComplex;        break;
						case 3: ctype='D';  linkedNode=linkedDna;            break;
						case 4: ctype='R';  linkedNode=linkedReaction;       break;
						case 5: ctype='E';  linkedNode=linkedPhysicalEntity; break;	
						case 6: ctype='L';  linkedNode=linkedANode; break;	
						case 7: ctype='M';  linkedNode=linkedCompartment;    break;	
						case 8: ctype='W';  linkedNode=linkedPathway;        break;	
					}
					
					int size=linkedNode.size();			
					linkedNode.clear();
					linkedNode.resize(size);

					/*vector<PathBubble1 *> mlist=mList;	 
					for(int i=0; i<mlist.size(); i++) 
					{
					
						PathBubble1* pbubble = mlist[i];		 
						if(pbubble!=this)
						{
							//for(int i=0;i<linkedNode[pbubble->m_pathwayID].size();i++)			
							if(linkedNode.size()>pbubble->m_pathwayID && linkedNode.size()>m_pathwayID)
							{
								linkedNode[m_pathwayID].clear();							
							}
						}
						else
						{
							if(linkedNode.size()>pbubble->m_pathwayID)
							{
								set<int> temp= linkedNode[pbubble->m_pathwayID];
								for(int k=0; k<linkedNode.size(); k++)
								{
									if(k!=m_pathwayID)
									{
										linkedNode[k].clear();								
									}
								}
							}
						}
					}
					*/
					switch(m)
				   {
						case 0: ctype='P';  pBubble->linkedProtein = linkedNode;          break;
						case 1: ctype='S';  pBubble->linkedSmallMolecule = linkedNode;    break;
						case 2: ctype='C';  pBubble->linkedComplex = linkedNode;          break;
						case 3: ctype='D';  pBubble->linkedDna = linkedNode;              break;
						case 4: ctype='R';  pBubble->linkedReaction = linkedNode;         break;
						case 5: ctype='E';  pBubble->linkedPhysicalEntity = linkedNode;   break;					
						case 6: ctype='L';  pBubble->linkedANode = linkedNode;            break;	
						case 7: ctype='M';  pBubble->linkedCompartment = linkedNode;      break;	
						case 8: ctype='W';  pBubble->linkedPathway = linkedNode;          break;	
				   }	
				}	
		}
		for(int i=0; i<pBubble->linkedEdge.size();i++)
		{
			set< vector< vector<int> > > temp = pBubble->linkedEdge[i];
			for(set< vector< vector<int> > >::iterator it=pBubble->linkedEdge[i].begin(); it != pBubble->linkedEdge[i].end(); it++)
			{
			    vector< vector<int> > item = *it;
				if( (item[0][0]==m_pathwayID && item[1][0]!=m_pathwayID) || (item[1][0]==m_pathwayID && item[0][0]!=m_pathwayID))
					temp.erase(item);			
			}
			pBubble->linkedEdge[i] = temp;
		}	
	}

}

bool PathBubble1::updateNodeNums(vector<vector<int>> edge, int &proteinNum, int &complexNum, int &physicalEntityNum, int &smallMoleculeNum, int &DnaNum,
int &emptyNum, vector<vector<QString>> &_proteinName, vector<vector<QString>> &_complexName, vector<vector<QString>> &_physicalEntityName, vector<vector<QString>> &_smallMoleculeName, vector<vector<QString>> &_DnaName,  vector<vector<QString>> &_emptyName, vector<vector<QString>> _compartmentName, vector<set<vector<int>>> &CompartmentContain, vector<QRectF> &proteinPos, vector<QRectF> &complexPos, vector<QRectF> &physicalEntityPos, vector<QRectF> &smallMoleculePos, vector<QRectF> &DnaPos, vector<QRectF> &EmptyPos)
{	
	bool resized=false;
	int Pmax, Cmax, Emax, Smax, Dmax, Nmax;
	Pmax = Cmax = Emax = Smax = Dmax = Nmax = -1000000;
    for(int i=0; i<edge.size(); i++)
	{
		for(int j=0; j<edge[i].size()-1; j=j+2)
		{
			int type=edge[i][j], id=edge[i][j+1];	
			if(id>=0)
			switch(type)
			{
				 case 'C': if(id > Cmax) Cmax=id;     break;
				 case 'D': if(id > Dmax) Dmax=id;     break;
				 case 'E': if(id > Emax) Emax=id;     break;
				 case 'P': if(id > Pmax) Pmax=id;     break;
				 case 'S': if(id > Smax) Smax=id;     break;		
				 case 'N': if(id > Nmax) Nmax=id;     break;	
			}		    
		}	
	}
	resized = resized || addANode('P', Pmax, proteinNum, _proteinName, _compartmentName, CompartmentContain, proteinPos);//, vector<vector<QRectF>> proteinPos, vector<vector<QRectF>> complexPos, vector<vector<QRectF>> physicalEntityPos, vector<vector<QRectF>> smallMoleculePos, vector<vector<QRectF>> DnaPos, vector<vector<QRectF>> EmptyPos);
	resized = resized || addANode('C', Cmax, complexNum, _complexName, _compartmentName, CompartmentContain,complexPos);
	resized = resized || addANode('E', Emax, physicalEntityNum, _physicalEntityName, _compartmentName, CompartmentContain,  physicalEntityPos);
	resized = resized || addANode('S', Smax, smallMoleculeNum, _smallMoleculeName, _compartmentName, CompartmentContain, smallMoleculePos);
	resized = resized || addANode('D', Dmax, DnaNum, _DnaName, _compartmentName, CompartmentContain, DnaPos);
	resized = resized || addANode('N', Nmax, emptyNum, _emptyName, _compartmentName, CompartmentContain, EmptyPos);	
	return resized;
}

bool PathBubble1::addANode(int type, int newNum, int &nodeNum, vector<vector<QString>> &nodeName, vector<vector<QString>> _compartmentName, vector<set<vector<int>>> &CompartmentContain, vector<QRectF> &nodePos)
{
	

	bool resized=false;	
	int maxNum;

	newNum=newNum+1;
	maxNum = newNum>nodeNum? newNum : nodeNum;
	maxNum = maxNum>nodeName.size()? maxNum : nodeName.size();
	maxNum = maxNum>nodePos.size()? maxNum : nodePos.size();

	if(maxNum>nodeName.size() || maxNum>nodeName.size())
	{
		resized=true;
	}

	if(nodeName.size()!=nodePos.size())
	   resized=true;

	if(!resized)
		return false;

	int oriSize=nodeName.size();
	nodeName.resize(maxNum);
	for(int i=oriSize; i<maxNum; i++)
	{
		vector<int> item;
		item.push_back(type); item.push_back(i);
		set<vector<int>> empty;
		vector<QString> temp(1,"");		
		for(int j=0;j<_compartmentName.size();j++)
		{
			if(_compartmentName[j][0]==temp[0])
			{
				CompartmentContain[j].insert(item);	
				break;
			}				  
		}
		nodeName[i].push_back(""); //name	
		nodeName[i].push_back("");
		nodeName[i].push_back("");
		nodeName[i].push_back("");
		nodeName[i].push_back("");
	}
	
	nodeNum=nodeName.size();
	oriSize=nodePos.size();
	nodePos.resize(nodeNum);//+1);

	for(int i=oriSize; i<nodeNum; i++)
	{
		nodePos[i]= QRectF(-1000,-1000,0,0);
	}
	return resized;
}

void PathBubble1::ParserTraceSteps()
{
   //_scene->pwdParser->TraceSteps( _pathwayStep, edge, reactionPos, _1pathway, _5Control, _Catalysis, _3pathwayStepCatalysis, _3pathwayStepReaction, _scene->_biochemicalReaction[m_pathwayID], drawSmallmolecule);
	_scene->pwdParser->TraceSteps_2( _pathwayStep, edge, reactionPos, _1pathway, _5Control, _Catalysis, _3pathwayStepCatalysis, _3pathwayStepReaction, _scene->_biochemicalReaction[m_pathwayID], drawSmallmolecule);
}

void PathBubble1::updateVisibleNodes()
{   vector<int> temp(2,0);
	QRectF s,m,e;
    for(int i=0; i<edge.size(); i++)
	{	
		if(edge[i][2]=='R'&&edge[i][4]=='R')
		{			
			bool flags=true, flagm=true, flage=true;
			bool t1=false,t2=false;
		
			int type=edge[i][0], id=edge[i][1];	
			if(id<0)
				 flags=false;

			if(id>=0)
			switch(type)
			{
				 case 'C': s=complexPos[id];          break;
				 case 'D': s=DnaPos[id];              break;
				 case 'E': s=physicalEntityPos[id];   break; 
				 case 'P': s=proteinPos[id];          break;
				 case 'S': s=smallMoleculePos[id];    break;			
			}
		    type=edge[i][4];
		    id=edge[i][5];

		    if(id<0)
			   flagm=false;

		    if(id>=0)
		    if(type=='R') 
		    {
			    m=reactionPos[id];			
		    }
			temp[0]=edge[i][4];	temp[1]=edge[i][5]; reactionRecord.insert(temp);				
			temp[0]=edge[i][0];	temp[1]=edge[i][1]; reactionRecord.insert(temp);		
		    /*if(m.x()>-999.99 && s.x()>-999.99)
			{
							                   
			}*/
		}
		else
		{
			bool flags=true, flagm=true, flage=true;
			bool t1=false,t2=false;
		
			int type=edge[i][0], id=edge[i][1];	
			if(id<0)
				 flags=false;

			if(id>=0)
			switch(type)
			{
				 case 'C': s=complexPos[id];          break; //bug when loading 190236
				 case 'D': s=DnaPos[id];              break;
				 case 'E': s=physicalEntityPos[id];   break; 
				 case 'P': s=proteinPos[id];          break; //bug when loading 2404192
				 case 'S': s=smallMoleculePos[id];    break;			
			}
		    type=edge[i][4];
		    id=edge[i][5];

		    if(id<0)
			   flagm=false;

		    if(id>=0)
		    if(type=='R') 
		    {
			    m=reactionPos[id];			
		    }
		 		 		 
		    type=edge[i][2], id=edge[i][3];	
		    QRectF rect;
		    if(id<0)
			   flage=false;

		    if(id>=0)
		    switch(type)
	        {
				 case 'C': rect= complexPos[id]; break;
				 case 'D': rect= DnaPos[id];    break;
				 case 'E': rect= physicalEntityPos[id];    break; 
				 case 'P': rect= proteinPos[id];    break;
				 case 'S': rect= smallMoleculePos[id];  break;			
				 case 'R': rect= reactionPos[id];  break;  //catalysis for bio reaction	
		    }	
		    e=rect;
			temp[0]=edge[i][4];	temp[1]=edge[i][5]; reactionRecord.insert(temp);
				temp[0]=edge[i][2];	temp[1]=edge[i][3]; reactionRecord.insert(temp);
				temp[0]=edge[i][0];	temp[1]=edge[i][1]; reactionRecord.insert(temp);		
			if(m.x()>-999.99 && e.x() >-999.99 && s.x()>-999.99)
			{
							                   
			}
		}		   
	} 

	temp[0]='C';	
	for(int i=0; i<complexPos.size(); i++)
	{
		temp[1]=i; 
		if(complexPos[i].x()>-999.99 && toplevel && reactionRecord.find(temp)==reactionRecord.end())	
		{  	
			complexPos[i] = QRectF(-1000, complexPos[i].y(), complexPos[i].width(), complexPos[i].height());		        
		}		
	}
	
	temp[0]='R';
	for(int i=0; i<reactionPos.size(); i++)
	{
		temp[1]=i; 
		if(reactionPos[i].x()>-999.99  && toplevel  && reactionRecord.find(temp)==reactionRecord.end() )	
		{
			 reactionPos[i]= QRectF(-1000, reactionPos[i].y(), reactionPos[i].width(), reactionPos[i].height());
		}
		
	}

		
	temp[0]='D';
	for(int i=0; i<DnaPos.size(); i++)
	{
		temp[1]=i; 
		if(DnaPos[i].x()>-999.99  && toplevel && reactionRecord.find(temp)==reactionRecord.end())		
		{	
			DnaPos[i]= QRectF(-1000, DnaPos[i].y(), DnaPos[i].width(), DnaPos[i].height());
		}	    
	}

	temp[0]='E';
	for(int i=0; i<physicalEntityPos.size(); i++)
	{
		temp[1]=i; 
	    if(physicalEntityPos[i].x()>-999.99 && toplevel && reactionRecord.find(temp)==reactionRecord.end())	
	    {
	        physicalEntityPos[i]= QRectF(-1000, physicalEntityPos[i].y(), physicalEntityPos[i].width(), physicalEntityPos[i].height());
	    }
	}	

	temp[0]='P';
	for(int i=0; i<proteinPos.size(); i++)
	{
	   temp[1]=i; 	
	   if(proteinPos[i].x()>-999.99 && toplevel && reactionRecord.find(temp)==reactionRecord.end())	
	   {
	       proteinPos[i]= QRectF(-1000, proteinPos[i].y(), proteinPos[i].width(), proteinPos[i].height());
	   }
	}
}

void PathBubble1::putReactionToCompartment(vector<vector<vector<int>>> _reaction, vector<set<vector<int>>> &CompartmentContain, vector<vector<vector<int>>> Ranks, vector<vector<int>> VertexInfo)
{
	for(int i=0; i<CompartmentContain.size(); i++)
	{
	    CompartmentContain[i].clear();
	}
	for(int roomID = Ranks.size()-1; roomID >= 0; roomID--) 	  
	{
		for(int i=Ranks[roomID].size()-1; i>=0; i--)
		{			
			for(int j=0; j<Ranks[roomID][i].size(); j++) 
			{
				//mark it if it is counted in two compartment
				int nid=Ranks[roomID][i][j];
				bool flag=true;
				vector<int> node,item(2,0);	
				item[0]=VertexInfo[nid][0]; item[1]=VertexInfo[nid][1];	
				if(nid==220)
					nid=nid;					
				for(int k=Ranks.size()-1; k>roomID; k--)
				{
					if(CompartmentContain.size()> k && CompartmentContain[k].find(item)!=CompartmentContain[k].end())
					{
						flag=false;
						break;
					}
				}
				if(flag && CompartmentContain.size()> roomID && CompartmentContain[roomID].find(item)==CompartmentContain[roomID].end())
				{
				    CompartmentContain[roomID].insert(item);
				}
			}
		}
	}
	
	/*
	vector<int> temp(2,0);
	for(int i=1; i<_reaction.size(); i++)
	{
		temp[0]='R';	temp[1]=i; 		
		vector<vector<int>> edgeRecord=_reaction[i]; 		
		vector<int> node,item(2,0);						
		set<int> cid;
		for(int k=0; k<edgeRecord.size(); k++)		
		{
		 		node=edgeRecord[k];
				item[0]=node[0]; item[1]=node[1];	
				int id=item[1];				
				for(int j=0; j<CompartmentContain.size()-1; j++)
				{
					 if(CompartmentContain[j].find(item)!=CompartmentContain[j].end())
					 {
						 QRectF pos; 
						 switch(item[0])
		                 {
		                      case 'C': pos=complexPos[id];             break;
							  case 'E': pos=physicalEntityPos[id];      break;
				              case 'P': pos=proteinPos[id];             break;
				              case 'S': pos=smallMoleculePos[id];       break;
				              case 'D': pos=DnaPos[id];                 break;
			                  case 'R': pos=reactionPos[id];            break;
		                 }
						 if(pos.x()>-999.99 && item[0]!='S')
						     cid.insert(j);				    
					 }
				}
		}			
		if(cid.size()>=1)
		{
			int size1=_scene->_compartmentName[m_pathwayID].size();
			int size2=CompartmentContain.size();
			if(size1>size2)
				CompartmentContain.resize(size1);

			 int Cid=*cid.begin();
			 for(int k=0; k<CompartmentContain.size()-1;k++)
			 {
			     if(CompartmentContain[k].find(temp)!=CompartmentContain[k].end())
				 {
					 CompartmentContain[k].erase(temp);					 
				 }
			 }	
			 CompartmentContain[Cid].insert(temp);
		}
		else 
		{
			int size1=_scene->_compartmentName[m_pathwayID].size();
			int size2=CompartmentContain.size();
			if(size1>size2)
				CompartmentContain.resize(size1);
			 CompartmentContain[CompartmentContain.size()-1].insert(temp);
		}
	}	*/
}


void PathBubble1::putReactionToCompartment(vector<vector<int>> edge, vector<set<vector<int>>> &CompartmentContain)
{
	int type, id;
	vector<int> start(4,0), middle(4,0), end(4,0);
	vector<int> ed;
	for(int i=0; i<edge.size(); i++)
	{
         start[0]=edge[i][0], start[1]=edge[i][1];			 		 
		 middle[0]=edge[i][4]; middle[1]=edge[i][5]; 		 
		 end[0]=edge[i][2], end[1]=edge[i][3]; 
		 if(end[0]!='R')		 
	     {
			
			 /*ed=middle;   ed[3]=1; // ed[2]='M'; 
			 if(start[1]>=0)
			 switch(start[0])
			 {
				 case 'C': _complex[start[1]].push_back(ed);   break;
				 case 'D': _Dna[start[1]].push_back(ed);       break;
				 case 'E': _entity[start[1]].push_back(ed);    break;
				 case 'P': _protein[start[1]].push_back(ed);         break;
				 case 'S': _smallMolecule[start[1]].push_back(ed);   break;		
				 //case 'N': _Empty[start[1]].push_back(ed);   break;	
				 //case 'L': _ANode[start[1]].push_back(ed);    break;
		     }	*/		
			 ed=start; ed[3]=-1;  // ed[2]='M'; 
			 if(middle[1]>=0)
			 {
				 if(middle[1]>=_reaction.size())
					 _reaction.resize(middle[1]+1);
				 _reaction[middle[1]].push_back(ed);  
			 }
						
			 ed=end;  ed[3]=1;  //ed[2]='A';
			 if(middle[1]>=0)
			 {
                 if(middle[1]>=_reaction.size())
					 _reaction.resize(middle[1]+1);
				 _reaction[middle[1]].push_back(ed);       
			 }
		     
			 ed=middle;   ed[3]=-1;  //ed[2]='A'; 
			 /*if(end[1]>=0)
			 switch(end[0])
	         {
				case 'C': _complex[end[1]].push_back(ed);        break;
				case 'D': _Dna[end[1]].push_back(ed);            break;
				case 'E': _entity[end[1]].push_back(ed);         break;
				case 'P': _protein[end[1]].push_back(ed);        break;
				case 'S': _smallMolecule[end[1]].push_back(ed);  break;		
				//case 'N': _Empty[end[1]].push_back(ed);          break;		
				//case 'L': _ANode[end[1]].push_back(ed);          break;
		     }*/
		 }
		 else 
		 {
			
			 /*ed=end;   ed[3]=1;  //ed[2]='R';
			 if(start[1]>=0)
			 switch(start[0])
			 {
				 case 'C': _complex[start[1]].push_back(ed);   break;
				 case 'D': _Dna[start[1]].push_back(ed);       break;
				 case 'E': _entity[start[1]].push_back(ed);    break;
				 case 'P': _protein[start[1]].push_back(ed);         break;
				 case 'S': _smallMolecule[start[1]].push_back(ed);   break;		
				 case 'N': _Empty[start[1]].push_back(ed);   break;		
				 //case 'L': _ANode[start[1]].push_back(ed);   break;	
		     }*/
			 ed=start; ed[3]=-1;  ed[2]='R'; 
			 if(end[1]>=0)
			 {
				 if(end[1]>=_reaction.size())
					 _reaction.resize(end[1]+1);
				 _reaction[end[1]].push_back(ed);      			 
			 }
		 }
	}   

	vector<int> temp(2,0);
	for(int i=0; i<_reaction.size(); i++)
	{
		temp[0]='R';	temp[1]=i; 
		//if all its neighbor belong to one compartment
		vector<vector<int>> edgeRecord=_reaction[i]; 
		
		vector<int> node,item(2,0);						
		set<int> cid;
		for(int k=0; k<edgeRecord.size(); k++)		
		{
		 		node=edgeRecord[k];
				item[0]=node[0]; item[1]=node[1];	
				int id=item[1];				
				for(int j=0; j<CompartmentContain.size()-1; j++)
				{
					 if(CompartmentContain[j].find(item)!=CompartmentContain[j].end())
					 {
						 QRectF pos; 
						 switch(item[0])
		                 {
		                      case 'C': pos=complexPos[id];             break;
							  case 'E': pos=physicalEntityPos[id];      break;
				              case 'P': pos=proteinPos[id];             break;
				              case 'S': pos=smallMoleculePos[id];       break;
				              case 'D': pos=DnaPos[id];                 break;
			                  case 'R': pos=reactionPos[id];            break;
		                 }
						 if(item[0]!='S')
						     cid.insert(j);				    
					 }
				}
		}			
		if(cid.size()>=1)
		{
			int size1=_scene->_compartmentName[m_pathwayID].size();
			int size2=CompartmentContain.size();
			if(size1>size2)
				CompartmentContain.resize(size1);

			 int Cid=*cid.begin();
			 for(int k=0; k<CompartmentContain.size()-1;k++)
			 {
			     if(CompartmentContain[k].find(temp)!=CompartmentContain[k].end())
				 {
					 CompartmentContain[k].erase(temp);					 
				 }
			 }	
			 CompartmentContain[Cid].insert(temp);
		}
		else 
		{
			int size1=_scene->_compartmentName[m_pathwayID].size();
			int size2=CompartmentContain.size();
			if(size1>size2)
				CompartmentContain.resize(size1);
			 CompartmentContain[CompartmentContain.size()-1].insert(temp);
		}
	}
				
}



void PathBubble1::putReactionToCompartment(vector<vector<vector<int>>> _reaction, vector<set<vector<int>>> &CompartmentContain)
{
	vector<int> temp(2,0);
	for(int i=0; i<_reaction.size(); i++)
	{
		temp[0]='R';	temp[1]=i; 
		//if all its neighbor belong to one compartment
		vector<vector<int>> edgeRecord=_reaction[i]; 
		
		vector<int> node,item(2,0);						
		set<int> cid;
		for(int k=0; k<edgeRecord.size(); k++)		
		{
		 		node=edgeRecord[k];
				item[0]=node[0]; item[1]=node[1];	
				int id=item[1];				
				for(int j=0; j<CompartmentContain.size()-1; j++)
				{
					 if(CompartmentContain[j].find(item)!=CompartmentContain[j].end())
					 {
						 QRectF pos; 
						 switch(item[0])
		                 {
		                      case 'C': pos=complexPos[id];             break;
							  case 'E': pos=physicalEntityPos[id];      break;
				              case 'P': pos=proteinPos[id];             break;
				              case 'S': pos=smallMoleculePos[id];       break;
				              case 'D': pos=DnaPos[id];                 break;
			                  case 'R': pos=reactionPos[id];            break;
		                 }
						 if(pos.x()>-999.99 && item[0]!='S')
						     cid.insert(j);				    
					 }
				}
		}			
		if(cid.size()>=1)
		{
			int size1=_scene->_compartmentName[m_pathwayID].size();
			int size2=CompartmentContain.size();
			if(size1>size2)
				CompartmentContain.resize(size1);

			 int Cid=*cid.begin();
			 for(int k=0; k<CompartmentContain.size()-1;k++)
			 {
			     if(CompartmentContain[k].find(temp)!=CompartmentContain[k].end())
				 {
					 CompartmentContain[k].erase(temp);					 
				 }
			 }	
			 CompartmentContain[Cid].insert(temp);
		}
		else 
		{
			int size1=_scene->_compartmentName[m_pathwayID].size();
			int size2=CompartmentContain.size();
			if(size1>size2)
				CompartmentContain.resize(size1);
			 CompartmentContain[CompartmentContain.size()-1].insert(temp);
		}
	}
				
}




bool PathBubble1::isSameReaction(int pid1, int rid1, int pid2, int rid2)
{
	if(rid1>-1 && rid2>-1)
		return false;

	vector<vector<int>> edgeRecord1,edgeRecord2;
	edgeRecord1 = _scene->_pathBubbles[pid1]-> _reaction[rid1];
	edgeRecord2 = _scene->_pathBubbles[pid2]-> _reaction[rid2];
	int did1=_scene->_pathBubbles[pid1]->dataID;
	int did2=_scene->_pathBubbles[pid2]->dataID;

	vector<int> node1,node2,item(2,0);		
	for(int i=0; i<edgeRecord1.size(); i++)	
	{
		node1=edgeRecord1[i];
		vector<QString> name1;
		if(node1[1]>=0)
		switch(node1[0])
		{
				case 'C': name1=_scene->_complexNameD[did1][node1[1]]; 	break;
				case 'E': name1=_scene->_physicalEntityNameD[did1][node1[1]]; 	break;				
				case 'P': name1=_scene->_proteinNameD[did1][node1[1]]; 	        break;				
		        case 'S': name1=_scene->_smallMoleculeNameD[did1][node1[1]]; 	break;				
		        case 'D': name1=_scene->_DnaNameD[did1][node1[1]]; 	break;								            
				case 'L': name1=_scene->_ANodeName[pid1][node1[1]]; 	break;
		}
		bool flag=false;
	    for(int k=0; k<edgeRecord2.size(); k++)	
	    {
	        node2=edgeRecord2[k];
			vector<QString> name2;
			if(node2[1]>=0)
			switch(node2[0])
		    {
				case 'C': name2=_scene->_complexNameD[did2][node2[1]]; 	break;
				case 'E': name2=_scene->_physicalEntityNameD[did2][node2[1]]; 	break;				
				case 'P': name2=_scene->_proteinNameD[did2][node2[1]]; 	        break;				
		        case 'S': name2=_scene->_smallMoleculeNameD[did2][node2[1]]; 	break;				
		        case 'D': name2=_scene->_DnaNameD[did2][node2[1]]; 	break;								            
				case 'L': name2=_scene->_DnaNameD[pid2][node2[1]]; 	break;
		    }
			if(completeSame)
			{
				if(name2==name1)
				{
					flag=true; break;
				}
			}
			else
			{
				if(node2[0]=='L')
				{
					if(name2[1]==name1[1])
					{
						flag=true; break;
					}
				}
				else
				{
					if(name2[0]==name1[0])
					{
						flag=true; break;
					}
				}
			}
	    }
		if(!flag) return false;
	}
	//vector<vector<vector<int>>> _reaction, vector<set<vector<int>>> &CompartmentContain
	return true;
}


void PathBubble1::HierarchicalLayout(bool whole, vector<QRectF> reactionPos1, bool rflag)
{//
    //if(reactionPos1.empty())
	int type, id;
	vector<int> start(4,0), middle(4,0), end(4,0);
	vector<int> ed;
	for(int i=0; i<edge.size(); i++)
	{			 
         start[0]=edge[i][0], start[1]=edge[i][1];			 		 
		 middle[0]=edge[i][4]; middle[1]=edge[i][5];
		 end[0]=edge[i][2], end[1]=edge[i][3]; 
		 if(end[0]!='R')		 
	     {
			 ed=start;  ed[2]='M'; ed[3]=-1; 
             _reaction[middle[1]].push_back(ed);  
			 ed=end; ed[2]='A'; ed[3]=1; 
			 _reaction[middle[1]].push_back(ed);       		     
		 }
		 else 
		 {
			 end[1]=edge[i][5];
			 ed=start; ed[2]='R'; ed[3]=-1; 			
			 _reaction[end[1]].push_back(ed);      			 
		 }
	}
	putReactionToCompartment(_reaction, _scene->CompartmentContain[m_pathwayID]);	
	if(reactionPos1.empty())
	{
	   Rank0=EdgetoNode(edge, _compartment, rflag); //for compartment	
	}
	QFile file1("after_HierarchicalLayout()");
	file1.open(QIODevice::WriteOnly | QIODevice::Truncate);
	file1.close();

	
	/*for(int i=0; i<edge.size(); i++)
	{	
		 if(reactionPos1.empty() || reactionPos1[edge[i][5]].x()>-1000)
		 {
			 start[0]=edge[i][0], start[1]=edge[i][1];			 		 
			 middle[0]=edge[i][4]; middle[1]=edge[i][5];
			 end[0]=edge[i][2], end[1]=edge[i][3]; 
			 if(end[0]!='R')		 
			 {
				 ed=start;  ed[2]='M'; ed[3]=-1; 
				 _reaction[middle[1]].push_back(ed);  
				 ed=end; ed[2]='A'; ed[3]=1; 
				 _reaction[middle[1]].push_back(ed);       		     
			 }
			 else 
			 {
				 end[1]=edge[i][5];
				 ed=start; ed[2]='R'; ed[3]=-1; 			
				 _reaction[end[1]].push_back(ed);      			 
			 }
		 }
	}*/	


	Ranks = EdgetoNode(edge, _protein, _complex, _entity, _smallMolecule, _Dna, _reaction, whole, reactionPos1,rflag);	
}


void PathBubble1::updateNodeLocation(vector<vector<vector<int>>> Ranks, vector<vector<int>> VertexInfo, int pid, vector<vector<set<vector<int>>>> &CompartmentContain, vector<set<vector<int>>> &CompartmentContain_1)
{
	set<vector<int>> WholeContain;
	for(int i=Ranks.size()-2; i>=0; i--)
	{	
		set<vector<int>> contain;
		for(int k=0; k<Ranks[i].size(); k++) 
		{
			for(int m=0; m<Ranks[i][k].size(); m++) 
			{
				vector<int> temp;
				temp.push_back(VertexInfo[Ranks[i][k][m]][0]);
				temp.push_back(VertexInfo[Ranks[i][k][m]][1]);					
				if(WholeContain.find(temp)==WholeContain.end())
				{
					WholeContain.insert(temp);
					contain.insert(temp);
				}
			}				
		}	
		//CompartmentContain_1[i]=contain;
		CompartmentContain[pid][i]=contain;
	}
}

void PathBubble1::averageNodes(vector<vector<int>> _VertexInfo, vector<QRectF> &_newPos)
{//resize: true, remove the compartment if it is empty
 	//put the subtree to __newCompartmentPos location
 	//vector<QRectF> newPos;//=_newPos;		
	//vector<QRectF> _newCompartmentPos=__newCompartmentPos;
	vector<vector<int>> newRank0; //for compartment
	vector<vector<vector<int>>> newRanks;
	
	//replace the nodelocation with hierarchicle ones
	float itv=0.9/8;
	//int totalNum = complexNum + proteinNum + physicalEntityNum + smallMoleculeNum + DnaNum + reactionNum + 2;
	float cw, ch, ew,eh, sw,sh, dw, dh, rw, rh, pw, ph;	
	int cNum, eNum, sNum, dNum, rNum, pNum;
	
	cw= ch= ew=eh= sw=sh= dw= dh= rw= rh= pw= ph =0;
	cNum= eNum= sNum= dNum= rNum= pNum = 0;
	for(int i=0; i<_VertexInfo.size(); i++) //i<totalNum && 
	{
		//int id=_VertexInfo[i][1];		
		switch(int(_VertexInfo[i][0]))
		{
		    case 'C': if(_newPos[i].x()>-999.99)
					  {
						 cw=cw+_newPos[i].width();
						 ch=ch+_newPos[i].height();
						 cNum++;
					  }
				break;
			case 'E': if(_newPos[i].x()>-999.99)
				      {
						 ew=ew+_newPos[i].width();
						 eh=eh+_newPos[i].height();
						 eNum++;
					  }
				break;
		    case 'P': if(_newPos[i].x()>-999.99)
					{
						 pw=pw+_newPos[i].width();
						 ph=ph+_newPos[i].height();
						 pNum++;
					  }  
				break;
		    case 'S': 
					if(_newPos[i].x()>-999.99)
					{
						sw=sw+_newPos[i].width();
						sh=sh+_newPos[i].height();
						sNum++;
					}
					break;
		    case 'D': 
				    if(_newPos[i].x()>-999.99)
					{
						dw=dw+_newPos[i].width();
						dh=dh+_newPos[i].height();
						dNum++;
					}    
					break;			
			case 'R': 
				    if(_newPos[i].x()>-999.99)
					{
						rw=rw+_newPos[i].width();
						rh=rh+_newPos[i].height();
						rNum++;
					}
					break;
		}	
	}
	cw=cw/cNum, ch=ch/cNum;
	ew=ew/eNum, eh=eh/eNum;
	pw=pw/pNum, ph=ph/pNum;
	sw=sw/sNum, sh=sh/sNum;
	dw=dw/dNum, dh=dh/dNum;
	rw=rw/rNum, rh=rh/rNum;

	for(int i=0; i<_VertexInfo.size(); i++) //totalNum && 
	{
		int id=_VertexInfo[i][1];		
		switch(int(_VertexInfo[i][0]))
		{
		    case 'C': if(_newPos[i].x()>-999.99)
					   _newPos[i]=QRectF(_newPos[i].center().x()-cw/2.0,_newPos[i].center().y()-ch/2.0,cw,ch);						
				break;
			case 'E': if(_newPos[i].x()>-999.99)
				      _newPos[i]=QRectF(_newPos[i].center().x()-ew/2.0,_newPos[i].center().y()-eh/2.0,ew,eh);
				break;
		    case 'P': if(_newPos[i].x()>-999.99)
					_newPos[i]=QRectF(_newPos[i].center().x()-pw/2.0,_newPos[i].center().y()-ph/2.0,pw,ph);
				break;
		    case 'S': 
					if(_newPos[i].x()>-999.99)
					_newPos[i]=QRectF(_newPos[i].center().x()-sw/2.0,_newPos[i].center().y()-sh/2.0,sw,sh);
					break;
		    case 'D': 
				    if(_newPos[i].x()>-999.99)
					_newPos[i]=QRectF(_newPos[i].center().x()-dw/2.0,_newPos[i].center().y()-dh/2.0,dw,dh);
					break;			
			case 'R': 
				    if(_newPos[i].x()>-999.99)
					_newPos[i]=QRectF(_newPos[i].center().x()-rw/2.0,_newPos[i].center().y()-rh/2.0,rw,rh);
					break;
		}	
	}
}

//remove the eliminated (disabled) nodes
//if not do this step, the graph will show only the visible nodes with empty space for eliminated nodes 


void PathBubble1::RemoveDisabledNodes(bool whole, vector<vector<int>> _VertexInfo, vector<vector<vector<int>>> _Vertex, vector<vector<int>> &Rank0, vector<vector<vector<int>>> &Ranks)
{//resize: true, remove the compartment if it is empty
 	//put the subtree to __newCompartmentPos location
 	//vector<QRectF> newPos;//=_newPos;		
	//vector<QRectF> _newCompartmentPos=__newCompartmentPos;
	vector<vector<int>> newRank0; //for compartment
	vector<vector<vector<int>>> newRanks;
	
	//replace the nodelocation with hierarchicle ones
	float itv=0.9/8;	
	for(int i=0; i<_VertexInfo.size(); i++)//totalNum && 
	{
		int id=_VertexInfo[i][1];		
		switch(int(_VertexInfo[i][0]))
		{
		    case 'C': if(complexPos[id].x()<-999.99)
					  _newPos[i]=complexPos[id];       
				break;
			case 'E': if(physicalEntityPos[id].x()<-999.99)
				      _newPos[i]=physicalEntityPos[id]; 
				break;
		    case 'P': if(proteinPos[id].x()<-999.99)
						  _newPos[i]=proteinPos[id];    
				break;
		    case 'S': 
				if(smallMoleculePos[id].x()<-999.99)
					_newPos[i]=smallMoleculePos[id];  
				break;
		    case 'D': 
				      if(DnaPos[id].x()<-999.99)
						  _newPos[i]=DnaPos[id];      
					  break;
			case 'N': 
				      if(EmptyPos[id].x()<-999.99)
						  _newPos[i]=EmptyPos[id];    
					  break;
			case 'R': 
				      if(reactionPos[id].x()<-999.99)
						  _newPos[i]=reactionPos[id];
					  break;
		}	
	}

	if(whole)
	{
		
		int roomID=0;
		vector<vector<int>> Rank_1;
		if(Ranks.size()>0)
		{
			for(int i=0; i<Ranks[roomID].size(); i++)
			{	
				vector<int> Rank_2;
				for(int j=0; j<Ranks[roomID][i].size(); j++) 
				{
					if(Ranks[roomID][i][j]<_newPos.size())
					{
						if(_newPos[Ranks[roomID][i][j]].x()>-999.99)
							Rank_2.push_back(Ranks[roomID][i][j]);
					}
				}
				if(!Rank_2.empty())
					Rank_1.push_back(Rank_2);
			}
			//if(!Rank_1.empty())
			newRanks.push_back(Rank_1);		
		}
	}
	else
	{
        //compute _newCompartmentPos based on rank0's location and tanks's size info	
		vector<set<vector<int>>> cContain=_scene->CompartmentContain[m_pathwayID];
		set<int> nodes1, nodes2; //nodes that only apear once or more
		for(int i=0; i<Ranks.size(); i++)
		{	//i: roomID?
			for(int k=0; k<Ranks[i].size(); k++) 
			{
				for(int l=0; l<Ranks[i][k].size(); l++) 
				{		
					int id=Ranks[i][k][l];
					if( nodes2.find(id)==nodes2.end())
					{
						nodes1.insert(id);
					}
					nodes2.insert(id);
				}
			}
		}

		set<int> CRecord; //keqin there is a issue need to be checked why Rank0 is empty in some compartment but Rank1 does not		
		for(int i=0; i<Rank0.size(); i++)
		{	
			vector<int> Rank0_1;
			for(int j=0; j<Rank0[i].size(); j++) 
			{
				CRecord.insert(Rank0[i][j]);
			}			
			
		}

		set<int> pSet;
		for(int i=0; i<Ranks.size(); i++)
		{	
			//i: roomID?	
			vector<vector<int>> Rank_1;
			if(_newCompartmentPos[i].x()<-990 || CRecord.find(i)==CRecord.end()) //keqin there is a issue need to be checked why Rank0 is empty in some compartment but Rank1 does not
			    Rank_1.clear();
			else
			{
				
				for(int k=0; k<Ranks[i].size(); k++) 
				{
					vector<int> Rank_2;
					for(int l=0; l<Ranks[i][k].size(); l++) 
					{		
						int id=Ranks[i][k][l];
						 if(id<_newPos.size() && _newPos[id].x()>-999.99)					 
						 {
							//consistent with compartment contain
							vector<int> node(2,0);						 
							node[0]=_VertexInfo[id][0];
							node[1]=_VertexInfo[id][1];
							
							if( i<cContain.size() )
							{
								if(cContain[i].find(node)!=cContain[i].end())
								{	
									Rank_2.push_back(id);							 
									pSet.insert(id);
								}
								else 
								{
								   if(nodes1.find(id)!=nodes1.end())
								   {
									   Rank_2.push_back(id);							 
									   pSet.insert(id);
								   }
								   else
								   {
									  id=id;
								   }
								}
							}
							else
							{
								 Rank_2.push_back(id);							 
								 pSet.insert(id);
							}
						 }
					}	
					if(!Rank_2.empty())
					{ 
						 Rank_1.push_back(Rank_2);					 
					}
		 		}
		    }
			if(Rank_1.empty())
			{
				_newCompartmentPos[i]=QRectF(-1000, _newCompartmentPos[i].y(), _newCompartmentPos[i].width(), _newCompartmentPos[i].height());
			}
			if(compartmentPos[i].x()<-1990)
			{	
				
				_newCompartmentPos[i]=QRectF(-1000, _newCompartmentPos[i].y(), _newCompartmentPos[i].width(), _newCompartmentPos[i].height());
				Rank_1.clear();
			}		
			newRanks.push_back(Rank_1);	
		}
		for(int i=0; i<_VertexInfo.size(); i++) //i<totalNum &&
		{
			 if(pSet.find(i)==pSet.end())
			 {
				 _newPos[i]=QRectF(-1000, _newPos[i].y(), _newPos[i].width(), _newPos[i].height());			 
			 }	
		}
		pSet.clear();
		for(int i=0; i<Rank0.size(); i++)
		{	
			vector<int> Rank0_1;
			for(int j=0; j<Rank0[i].size(); j++) 
			{
				vector<int> Rank_2;
				if(Rank0[i][j]<_compartment.size())
				{
					if(_newCompartmentPos[Rank0[i][j]].x()>-999.99 && !newRanks[Rank0[i][j]].empty())
					{
						Rank0_1.push_back(Rank0[i][j]);										
					}
				}				
			}			
			if(!Rank0_1.empty())
		    {  
				newRank0.push_back(Rank0_1);				
			}			
		}
	}
	
	Rank0=newRank0; 
	Ranks=newRanks;
}
             
void PathBubble1::pathWayPoiRecord(vector<QRectF> complexPos, vector<QRectF> proteinPos, vector<QRectF> smallMoleculePos, vector<QRectF> DnaPos, vector<QRectF> EmptyPos, vector<QRectF> reactionPos, vector<QRectF> physicalEntityPos, vector<QRectF> compartmentPos, vector<QRectF> ANodePos, vector<set<vector<int>>> CompartmentContain, float _scale, QPointF dCenter, bool drawSmallmolecule)
{
	QString pathName = _scene->pwdParser->getPathGraphHie(_pathName); //_scene->pwdParser->savePathGraphPreparation(_pathName);		
	if(drawSmallmolecule)
	{
	    pathName= pathName + "_S";
	}
	pathName= pathName + ".poi";
	//QString fileName = QFileDialog::getSaveFileName(0, tr("Save Pathway Graph as xml file"), pathName,".xml (*.xml)");//"c:\\file.x", "X files (*.x)"
	QFile file(pathName);
	if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
		return;
	
    QTextStream out(&file);
	out << "_scale" << " " << _scale << "\n";	//float _scale, QPointF dCenter
	out << "dCenter" << " " << dCenter.x() << " " << dCenter.y() << "\n";

	out << "complexPos.size()" << " " <<  complexPos.size() << "\n";		
	for(int i=0; i<complexPos.size(); i++)
	{
		
        out << " " << complexPos[i].x() << " " << complexPos[i].y() << " " << complexPos[i].width() << " " << complexPos[i].height();			
		out << "\n";
	}

	out << "proteinPos.size()" << " " <<  proteinPos.size() << "\n";		
	for(int i=0; i<proteinPos.size(); i++)
	{
		
        out << " " << proteinPos[i].x() << " " << proteinPos[i].y() << " " << proteinPos[i].width() << " " << proteinPos[i].height();			
		out << "\n";
	}

	out << "smallMoleculePos.size()" << " " <<  smallMoleculePos.size() << "\n";		
	for(int i=0; i<smallMoleculePos.size(); i++)
	{
		
        out << " " << smallMoleculePos[i].x() << " " << smallMoleculePos[i].y() << " " << smallMoleculePos[i].width() << " " << smallMoleculePos[i].height();			
		out << "\n";
	}

	out << "DnaPos.size()" << " " <<  DnaPos.size() << "\n";		
	for(int i=0; i<DnaPos.size(); i++)
	{
		
        out << " " << DnaPos[i].x() << " " << DnaPos[i].y() << " " << DnaPos[i].width() << " " << DnaPos[i].height();			
		out << "\n";
	}

	out << "EmptyPos.size()" << " " <<  EmptyPos.size() << "\n";		
	for(int i=0; i<EmptyPos.size(); i++)
	{
		
        out << " " << EmptyPos[i].x() << " " << EmptyPos[i].y() << " " << EmptyPos[i].width() << " " << EmptyPos[i].height();			
		out << "\n";
	}

	out << "reactionPos.size()" << " " <<  reactionPos.size() << "\n";		
	for(int i=0; i<reactionPos.size(); i++)
	{
		
        out << " " << reactionPos[i].x() << " " << reactionPos[i].y() << " " << reactionPos[i].width() << " " << reactionPos[i].height();			
		out << "\n";
	}

	out << "physicalEntityPos.size()" << " " <<  physicalEntityPos.size() << "\n";		
	for(int i=0; i<physicalEntityPos.size(); i++)
	{
		
        out << " " << physicalEntityPos[i].x() << " " << physicalEntityPos[i].y() << " " << physicalEntityPos[i].width() << " " << physicalEntityPos[i].height();			
		out << "\n";
	}

	out << "compartmentPos.size()" << " " <<  compartmentPos.size() << "\n";		
	for(int i=0; i<compartmentPos.size(); i++)
	{
		
        out << " " << compartmentPos[i].x() << " " << compartmentPos[i].y() << " " << compartmentPos[i].width() << " " << compartmentPos[i].height();			
		out << "\n";
	}

	out << "ANodePos.size()" << " " <<  ANodePos.size() << "\n";		
	for(int i=0; i<ANodePos.size(); i++)
	{
		
        out << " " << ANodePos[i].x() << " " << ANodePos[i].y() << " " << ANodePos[i].width() << " " << ANodePos[i].height();			
		out << "\n";
	}

	out << "CompartmentContain.size()" << " " <<  CompartmentContain.size() << "\n";	
	for(int i=0; i<CompartmentContain.size(); i++)
	{
		for(set<vector<int>>::iterator it = CompartmentContain[i].begin(); it != CompartmentContain[i].end(); it++)
		{   
			vector<int> temp=*it;
			out << " " << temp[0];					    
			out << " " << temp[1];					    
		}
		out << "\n";
	}	
	//vtor<vector<vector<int>>> _protein, _complex, _entity, _smallMolecule, _Dna, _reaction, _compartment, _Empty,_ANode;
	out << "_protein.size()" << " " <<  _protein.size() << "\n";	
	for(int i=0; i<_protein.size(); i++)
	{
		out << "_protein[i].size()" << " " <<  _protein[i].size() << "\n";
		for(int j = 0; j < _protein[i].size(); j++)
		{   
			for(int k = 0; k < _protein[i][j].size(); k++)
			{
				out << " " << _protein[i][j][k];					    			    
			}
			out << "\n";
		}		
	}

	out << "_complex.size()" << " " <<  _complex.size() << "\n";	
	for(int i=0; i<_complex.size(); i++)
	{
		out << "_complex[i].size()" << " " <<  _complex[i].size() << "\n";
		for(int j = 0; j < _complex[i].size(); j++)
		{   
			for(int k = 0; k < _complex[i][j].size(); k++)
			{
				out << " " << _complex[i][j][k];					    			    
			}
			out << "\n";
		}
		
	}


	out << "_entity.size()" << " " <<  _entity.size() << "\n";	
	for(int i=0; i<_entity.size(); i++)
	{
		out << "_entity[i].size()" << " " <<  _entity[i].size() << "\n";
		for(int j = 0; j < _entity[i].size(); j++)
		{   
			for(int k = 0; k < _entity[i][j].size(); k++)
			{
				out << " " << _entity[i][j][k];					    			    
			}
			out << "\n";
		}
		
	}


	out << "_smallMolecule.size()" << " " <<  _smallMolecule.size() << "\n";	
	for(int i=0; i<_smallMolecule.size(); i++)
	{
		out << "_smallMolecule[i].size()" << " " <<  _smallMolecule[i].size() << "\n";
		for(int j = 0; j < _smallMolecule[i].size(); j++)
		{   
			for(int k = 0; k < _smallMolecule[i][j].size(); k++)
			{
				out << " " << _smallMolecule[i][j][k];					    			    
			}
			out << "\n";
		}
		
	}

	out << "_Dna.size()" << " " <<  _Dna.size() << "\n";	
	for(int i=0; i<_Dna.size(); i++)
	{
		out << "_Dna[i].size()" << " " <<  _Dna[i].size() << "\n";
		for(int j = 0; j < _Dna[i].size(); j++)
		{   
			for(int k = 0; k < _Dna[i][j].size(); k++)
			{
				out << " " << _Dna[i][j][k];					    			    
			}
			out << "\n";
		}
		
	}

	//_reaction, _compartment, _Empty,_ANode;
	out << "_reaction.size()" << " " <<  _reaction.size() << "\n";	
	for(int i=0; i<_reaction.size(); i++)
	{
		out << "_reaction[i].size()" << " " <<  _reaction[i].size() << "\n";
		for(int j = 0; j < _reaction[i].size(); j++)
		{   
			for(int k = 0; k < _reaction[i][j].size(); k++)
			{
				out << " " << _reaction[i][j][k];					    			    
			}
			out << "\n";
		}
		
	}

	out << "_compartment.size()" << " " <<  _compartment.size() << "\n";	
	for(int i=0; i<_compartment.size(); i++)
	{
		out << "_compartment[i].size()" << " " <<  _compartment[i].size() << "\n";
		for(int j = 0; j < _compartment[i].size(); j++)
		{   
			for(int k = 0; k < _compartment[i][j].size(); k++)
			{
				out << " " << _compartment[i][j][k];					    			    
			}
			out << "\n";
		}
		
	}

	out << "_Empty.size()" << " " <<  _Empty.size() << "\n";	
	for(int i=0; i<_Empty.size(); i++)
	{
		out << "_Empty[i].size()" << " " <<  _Empty[i].size() << "\n";
		for(int j = 0; j < _Empty[i].size(); j++)
		{   
			for(int k = 0; k < _Empty[i][j].size(); k++)
			{
				out << " " << _Empty[i][j][k];					    			    
			}
			out << "\n";
		}
		
	}


	out << "_ANode.size()" << " " <<  _ANode.size() << "\n";	
	for(int i=0; i<_ANode.size(); i++)
	{
		out << "_ANode[i].size()" << " " <<  _ANode[i].size() << "\n";
		for(int j = 0; j < _ANode[i].size(); j++)
		{   
			for(int k = 0; k < _ANode[i][j].size(); k++)
			{
				out << " " << _ANode[i][j][k];					    			    
			}
			out << "\n";
		}
		
	}

	
	file.close();
}

bool PathBubble1::pathWayPoiRead(vector<QRectF> &complexPos, vector<QRectF> &proteinPos, vector<QRectF> &smallMoleculePos, vector<QRectF> &DnaPos, vector<QRectF> &EmptyPos, vector<QRectF> &reactionPos, vector<QRectF> &physicalEntityPos, vector<QRectF> &compartmentPos, vector<QRectF> &ANodePos, vector<set<vector<int>>> &CompartmentContain, float &_scale, QPointF &dCenter, bool drawSmallmolecule)
{//resize: true, remove the compartment if it is empty
 	
	/*vector<vector<int>> VertexInfo_1;
	vector<vector<vector<int>>> Vertex_1;
	vector<vector<int>> Rank0_1;
	vector<vector<vector<int>>> Ranks_1;
	QString pathName = _scene->pwdParser->getPathGraphHie(_pathName); 
	vector<QRectF> newPos_1, newCompartmentPos_1;
	vector<vector<int>> Edge_1;*/

	QString pathName = _scene->pwdParser->getPathGraphHie(_pathName); //_scene->pwdParser->savePathGraphPreparation(_pathName);		
	if(drawSmallmolecule)
	{
	    pathName= pathName + "_S";
	}
	pathName= pathName + ".poi";
	//QString fileName = QFileDialog::getSaveFileName(0, tr("Save Pathway Graph as xml file"), pathName,".xml (*.xml)");//"c:\\file.x", "X files (*.x)"
	QFile file(pathName);
	if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return false;
	
	QTextStream in(& file);			 

	//while ( !in.atEnd() )
	{			   
		QString line = in.readLine();    
		if(!line.contains("_scale"))
			return false;
		
		QStringList sname=line.split(' ');	
		_scale = sname[1].toFloat();

		line = in.readLine();    
		sname=line.split(' ');	
		dCenter.setX(sname[1].toFloat()); 
		dCenter.setY(sname[2].toFloat());

		//nameSet.insert(sname[0]);	
		line = in.readLine();    
		if(!line.contains("complexPos.size()"))
			return false;
		sname=line.split(' ');	
		int I=sname[1].toInt();
		complexPos.resize(I);
		for(int i=0; i<I; i++)
		{
			line = in.readLine();    
			QStringList sl=line.split(' ');	
			vector<float> px;
			for(int j=1; j<sl.size(); j++)
			{
				px.push_back(sl[j].toFloat());
			}
			complexPos[i] = QRectF(px[0], px[1], px[2], px[3]);
		}


		line = in.readLine();    
		if(!line.contains("proteinPos.size()"))
			return false;
		sname=line.split(' ');	
		I=sname[1].toInt();
		proteinPos.resize(I);
		for(int i=0; i<I; i++)
		{
			line = in.readLine();    
			QStringList sl=line.split(' ');	
			vector<float> px;
			for(int j=1; j<sl.size(); j++)
			{
				px.push_back(sl[j].toFloat());
			}
			proteinPos[i] = QRectF(px[0], px[1], px[2], px[3]);
		}


		line = in.readLine();    
		if(!line.contains("smallMoleculePos.size()"))
			return false;
		sname=line.split(' ');	
		I=sname[1].toInt();
		smallMoleculePos.resize(I);
		for(int i=0; i<I; i++)
		{
			line = in.readLine();    
			QStringList sl=line.split(' ');	
			vector<float> px;
			for(int j=1; j<sl.size(); j++)
			{
				px.push_back(sl[j].toFloat());
			}
			smallMoleculePos[i] = QRectF(px[0], px[1], px[2], px[3]);
		}


		line = in.readLine();    
		if(!line.contains("DnaPos.size()"))
			return false;
		sname=line.split(' ');	
		I=sname[1].toInt();
		DnaPos.resize(I);
		for(int i=0; i<I; i++)
		{
			line = in.readLine();    
			QStringList sl=line.split(' ');	
			vector<float> px;
			for(int j=1; j<sl.size(); j++)
			{
				px.push_back(sl[j].toFloat());
			}
			DnaPos[i] = QRectF(px[0], px[1], px[2], px[3]);
		}

		
		line = in.readLine();    
		if(!line.contains("EmptyPos.size()"))
			return false;
		sname=line.split(' ');	
		I=sname[1].toInt();
		EmptyPos.resize(I);
		for(int i=0; i<I; i++)
		{
			line = in.readLine();    
			QStringList sl=line.split(' ');	
			vector<float> px;
			for(int j=1; j<sl.size(); j++)
			{
				px.push_back(sl[j].toFloat());
			}
			EmptyPos[i] = QRectF(px[0], px[1], px[2], px[3]);
		}
		
		line = in.readLine();    
		if(!line.contains("reactionPos.size()"))
			return false;
		sname=line.split(' ');	
		I=sname[1].toInt();
		reactionPos.resize(I);
		for(int i=0; i<I; i++)
		{
			line = in.readLine();    
			QStringList sl=line.split(' ');	
			vector<float> px;
			for(int j=1; j<sl.size(); j++)
			{
				px.push_back(sl[j].toFloat());
			}
			reactionPos[i] = QRectF(px[0], px[1], px[2], px[3]);
		}


		line = in.readLine();    
		if(!line.contains("physicalEntityPos.size()"))
			return false;
		sname=line.split(' ');	
	    I=sname[1].toInt();
		physicalEntityPos.resize(I);
		for(int i=0; i<I; i++)
		{
			line = in.readLine();    
			QStringList sl=line.split(' ');	
			vector<float> px;
			for(int j=1; j<sl.size(); j++)
			{
				px.push_back(sl[j].toFloat());
			}
			physicalEntityPos[i] = QRectF(px[0], px[1], px[2], px[3]);
		}


		line = in.readLine();    
		if(!line.contains("compartmentPos.size()"))
			return false;
		sname=line.split(' ');	
		I=sname[1].toInt();
		compartmentPos.resize(I);
		for(int i=0; i<I; i++)
		{
			line = in.readLine();    
			QStringList sl=line.split(' ');	
			vector<float> px;
			for(int j=1; j<sl.size(); j++)
			{
				px.push_back(sl[j].toFloat());
			}
			compartmentPos[i] = QRectF(px[0], px[1], px[2], px[3]);
		}


		line = in.readLine();    
		if(!line.contains("ANodePos.size()"))
			return false;
		sname=line.split(' ');	
		I=sname[1].toInt();
		ANodePos.resize(I);
		for(int i=0; i<I; i++)
		{
			line = in.readLine();    
			QStringList sl=line.split(' ');	
			vector<float> px;
			for(int j=1; j<sl.size(); j++)
			{
				px.push_back(sl[j].toFloat());
			}
			ANodePos[i] = QRectF(px[0], px[1], px[2], px[3]);
		}


		line = in.readLine();    
		if(!line.contains("CompartmentContain.size()"))
			return false;
		sname=line.split(' ');	
		I=sname[1].toInt();
		CompartmentContain.resize(I);
		for(int i=0; i<I; i++)
		{
			line = in.readLine();    
			QStringList sl=line.split(' ');				
			for(int j=1; j<sl.size(); j=j+2)
			{
				vector<int> item(2,0);
				item[0]=sl[j].toInt(); 
				item[1]=sl[j+1].toInt();
				CompartmentContain[i].insert(item);
			}			
		}	

	    //vector<vector<vector<int>>> _protein, _complex, _entity, _smallMolecule, _Dna, _reaction, _compartment, _Empty,_ANode;
		if(drawSmallmolecule)
		{
			_protein.clear(), _complex.clear(), _entity.clear(), _smallMolecule.clear(), _Dna.clear(), _reaction.clear(), _compartment.clear(), _Empty.clear(),_ANode.clear();
			line = in.readLine();    
			if(!line.contains("_protein.size()"))
				return false;
			sname=line.split(' ');	
			I=sname[1].toInt();
			_protein.resize(I);
			for(int i=0; i<I; i++)
			{
				line = in.readLine();    
				QStringList sl=line.split(' ');	
				if(!line.contains("_protein[i].size()"))
				   return false;
			
				int J=sl[1].toInt();
				_protein[i].resize(J);
				for(int j=0; j<J; j++)
				{
					line = in.readLine();    
					sl=line.split(' ');				
					for(int k=1; k<sl.size(); k++)
					{
						_protein[i][j].push_back(sl[k].toInt());
					}	
				}
			}	

			line = in.readLine();    
			if(!line.contains("_complex.size()"))
				return false;
			sname=line.split(' ');	
			I=sname[1].toInt();
			_complex.resize(I);
			for(int i=0; i<I; i++)
			{
				line = in.readLine();    
				QStringList sl=line.split(' ');	
				if(!line.contains("_complex[i].size()"))
				   return false;
			
				int J=sl[1].toInt();
				_complex[i].resize(J);
				for(int j=0; j<J; j++)
				{
					line = in.readLine();    
					sl=line.split(' ');				
					for(int k=1; k<sl.size(); k++)
					{
						_complex[i][j].push_back(sl[k].toInt());
					}	
				}
			}	

			line = in.readLine();    
			if(!line.contains("_entity.size()"))
				return false;
			sname=line.split(' ');	
			I=sname[1].toInt();
			_entity.resize(I);
			for(int i=0; i<I; i++)
			{
				line = in.readLine();    
				QStringList sl=line.split(' ');	
				if(!line.contains("_entity[i].size()"))
				   return false;
			
				int J=sl[1].toInt();
				_entity[i].resize(J);
				for(int j=0; j<J; j++)
				{
					line = in.readLine();    
					sl=line.split(' ');				
					for(int k=1; k<sl.size(); k++)
					{
						_entity[i][j].push_back(sl[k].toInt());
					}	
				}
			}	

			line = in.readLine();    
			if(!line.contains("_smallMolecule.size()"))
				return false;
			sname=line.split(' ');	
			I=sname[1].toInt();
			_smallMolecule.resize(I);
			for(int i=0; i<I; i++)
			{
				line = in.readLine();    
				QStringList sl=line.split(' ');	
				if(!line.contains("_smallMolecule[i].size()"))
				   return false;
			
				int J=sl[1].toInt();
				_smallMolecule[i].resize(J);
				for(int j=0; j<J; j++)
				{
					line = in.readLine();    
					sl=line.split(' ');				
					for(int k=1; k<sl.size(); k++)
					{
						_smallMolecule[i][j].push_back(sl[k].toInt());
					}	
				}
			}	

			line = in.readLine();    
			if(!line.contains("_Dna.size()"))
				return false;
			sname=line.split(' ');	
			I=sname[1].toInt();
			_Dna.resize(I);
			for(int i=0; i<I; i++)
			{
				line = in.readLine();    
				QStringList sl=line.split(' ');	
				if(!line.contains("_Dna[i].size()"))
				   return false;
			
				int J=sl[1].toInt();
				_Dna[i].resize(J);
				for(int j=0; j<J; j++)
				{
					line = in.readLine();    
					sl=line.split(' ');				
					for(int k=1; k<sl.size(); k++)
					{
						_Dna[i][j].push_back(sl[k].toInt());
					}	
				}
			}	

			line = in.readLine();    
			if(!line.contains("_reaction.size()"))
				return false;
			sname=line.split(' ');	
			I=sname[1].toInt();
			_reaction.resize(I);
			for(int i=0; i<I; i++)
			{
				line = in.readLine();    
				QStringList sl=line.split(' ');	
				if(!line.contains("_reaction[i].size()"))
				   return false;
			
				int J=sl[1].toInt();
				_reaction[i].resize(J);
				for(int j=0; j<J; j++)
				{
					line = in.readLine();    
					sl=line.split(' ');				
					for(int k=1; k<sl.size(); k++)
					{
						_reaction[i][j].push_back(sl[k].toInt());
					}	
				}
			}	

			line = in.readLine();    
			if(!line.contains("_compartment.size()"))
				return false;
			sname=line.split(' ');	
			I=sname[1].toInt();
			_compartment.resize(I);
			for(int i=0; i<I; i++)
			{
				line = in.readLine();    
				QStringList sl=line.split(' ');	
				if(!line.contains("_compartment[i].size()"))
				   return false;
			
				int J=sl[1].toInt();
				_compartment[i].resize(J);
				for(int j=0; j<J; j++)
				{
					line = in.readLine();    
					sl=line.split(' ');				
					for(int k=1; k<sl.size(); k++)
					{
						_compartment[i][j].push_back(sl[k].toInt());
					}	
				}
			}	
			// _Empty,_ANode;
			line = in.readLine();    
			if(!line.contains("_Empty.size()"))
				return false;
			sname=line.split(' ');	
			I=sname[1].toInt();
			_Empty.resize(I);
			for(int i=0; i<I; i++)
			{
				line = in.readLine();    
				QStringList sl=line.split(' ');	
				if(!line.contains("_Empty[i].size()"))
				   return false;
			
				int J=sl[1].toInt();
				_Empty[i].resize(J);
				for(int j=0; j<J; j++)
				{
					line = in.readLine();    
					sl=line.split(' ');				
					for(int k=1; k<sl.size(); k++)
					{
						_Empty[i][j].push_back(sl[k].toInt());
					}	
				}
			}	

			line = in.readLine();    
			if(!line.contains("_ANode.size()"))
				return false;
			sname=line.split(' ');	
			I=sname[1].toInt();
			_ANode.resize(I);
			for(int i=0; i<I; i++)
			{
				line = in.readLine();    
				QStringList sl=line.split(' ');	
				if(!line.contains("_ANode[i].size()"))
				   return false;
			
				int J=sl[1].toInt();
				_ANode[i].resize(J);
				for(int j=0; j<J; j++)
				{
					line = in.readLine();    
					sl=line.split(' ');				
					for(int k=1; k<sl.size(); k++)
					{
						_ANode[i][j].push_back(sl[k].toInt());
					}	
				}
			}
		}
	}
	file.close(); 
	return true;
}


void PathBubble1::pathWayRecord(vector<vector<int>> edge, vector<vector<QString>> _complexName, vector<vector<QString>> _physicalEntityName, vector<vector<QString>> _proteinName, vector<vector<QString>> _reactionName, vector<vector<QString>> _degradationName, vector<vector<QString>> _smallMoleculeName, vector<vector<QString>> _EmptyName, vector<vector<QString>> _DnaName, vector<vector<QString>> _ANodeName,  vector<vector<QString>> _compartmentName)//vector<vector<int>> _Edge, 
{//resize: true, remove the compartment if it is empty
 	
	QString pathName = _scene->pwdParser->getPathGraphHie(_pathName); //_scene->pwdParser->savePathGraphPreparation(_pathName);		
	
	pathName= pathName + ".hie";
	//QString fileName = QFileDialog::getSaveFileName(0, tr("Save Pathway Graph as xml file"), pathName,".xml (*.xml)");//"c:\\file.x", "X files (*.x)"
	QFile file(pathName);
	if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
		return;
	
    QTextStream out(&file);

		out << "edge.size()" << " " <<  edge.size() << "\n";	
	for(int i=0; i<edge.size(); i++)
	{
		for(int j=0; j<edge[i].size(); j++)
        {
			out << " " << edge[i][j] ;	
		}
		out << "\n";
	}	


	out << "_complexName.size()" << "\t" <<  _complexName.size() << "\n";	
	for(int i=0; i<_complexName.size(); i++)
	{
		for(int j=0; j<_complexName[i].size(); j++)
        {
			out << "\t" << _complexName[i][j] ;	
		}
		out << "\n";
	}	

	out << "_physicalEntityName.size()" << "\t" <<  _physicalEntityName.size() << "\n";	
	for(int i=0; i<_physicalEntityName.size(); i++)
	{
		for(int j=0; j<_physicalEntityName[i].size(); j++)
        {
			out << "\t" << _physicalEntityName[i][j] ;	
		}
		out << "\n";
	}	

	out << "_proteinName.size()" << "\t" <<  _proteinName.size() << "\n";	
	for(int i=0; i<_proteinName.size(); i++)
	{
		for(int j=0; j<_proteinName[i].size(); j++)
        {
			out << "\t" << _proteinName[i][j] ;	
		}
		out << "\n";
	}	

	out << "_reactionName.size()" << "\t" <<  _reactionName.size() << "\n";	
	for(int i=0; i<_reactionName.size(); i++)
	{
		for(int j=0; j<_reactionName[i].size(); j++)
        {
			out << "\t" << _reactionName[i][j] ;	
		}
		out << "\n";
	}	

	out << "_smallMoleculeName.size()" << "\t" <<  _smallMoleculeName.size() << "\n";	
	for(int i=0; i<_smallMoleculeName.size(); i++)
	{
		for(int j=0; j<_smallMoleculeName[i].size(); j++)
        {
			out << "\t" << _smallMoleculeName[i][j] ;	
		}
		out << "\n";
	}	

	out << "_DnaName.size()" << "\t" <<  _DnaName.size() << "\n";	
	for(int i=0; i<_DnaName.size(); i++)
	{
		for(int j=0; j<_DnaName[i].size(); j++)
        {
			out << "\t" << _DnaName[i][j] ;	
		}
		out << "\n";
	}	

	out << "_EmptyName.size()" << "\t" <<  _EmptyName.size() << "\n";	
	for(int i=0; i<_EmptyName.size(); i++)
	{
		for(int j=0; j<_EmptyName[i].size(); j++)
        {
			out << "\t" << _EmptyName[i][j] ;	
		}
		out << "\n";
	}	

	out << "_ANodeName.size()" << "\t" <<  _ANodeName.size() << "\n";	
	for(int i=0; i<_ANodeName.size(); i++)
	{
		for(int j=0; j<_ANodeName[i].size(); j++)
        {
			out << "\t" << _ANodeName[i][j] ;	
		}
		out << "\n";
	}	

	out << "_compartmentName.size()" << "\t" <<  _compartmentName.size() << "\n";	
	for(int i=0; i<_compartmentName.size(); i++)
	{
		for(int j=0; j<_compartmentName[i].size(); j++)
        {
			out << "\t" << _compartmentName[i][j] ;	
		}
		out << "\n";
	}	


	
	file.close(); 	

	updateEdgetoNode(edge);
}




bool PathBubble1::pathWayRead(vector<vector<int>> &edge, vector<vector<QString>> &_complexName, vector<vector<QString>> &_physicalEntityName, vector<vector<QString>> &_proteinName, vector<vector<QString>> &_reactionName, vector<vector<QString>> &_degradationName, vector<vector<QString>> &_smallMoleculeName, vector<vector<QString>> &_EmptyName, vector<vector<QString>> &_DnaName, vector<vector<QString>> &_ANodeName, vector<vector<QString>> &_compartmentName)
{//resize: true, remove the compartment if it is empty
 	//vector<vector<int>> VertexInfo_1;
	//vector<vector<vector<int>>> Vertex_1;
	//vector<vector<int>> Rank0_1;
	//vector<vector<vector<int>>> Ranks_1;
	QString pathName = _scene->pwdParser->getPathGraphHie(_pathName); 
	//vector<QRectF> newPos_1, newCompartmentPos_1;
	vector<vector<int>> Edge_1;

	//QString pathName = _scene->pwdParser->savePathGraphPreparation(_pathName);		
	
	pathName= pathName + ".hie";
	//QString fileName = QFileDialog::getSaveFileName(0, tr("Save Pathway Graph as xml file"), pathName,".xml (*.xml)");//"c:\\file.x", "X files (*.x)"
	QFile file(pathName);
	if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return false;
	
	QTextStream in(& file);			 

	QString line = in.readLine();    
	if(!line.contains("edge.size()"))
		return false;
		
	QStringList   sname=line.split(' ');	
	//nameSet.insert(sname[0]);				
	int I=sname[1].toInt();
	Edge_1.resize(I);
	for(int i=0; i<I; i++)
	{
		line = in.readLine();    
		QStringList sl=line.split(' ');	
		for(int j=1; j<sl.size(); j++)
		{
			Edge_1[i].push_back(sl[j].toInt());
		}
	}


	line = in.readLine();    
	if(!line.contains("_complexName.size()"))
		return false;		
	sname=line.split('\t');	
	//nameSet.insert(sname[0]);				
	I=sname[1].toInt();
	_complexName.resize(I);
	for(int i=0; i<I; i++)
	{
		line = in.readLine();    
		QStringList sl=line.split('\t');	
		for(int j=1; j<sl.size(); j++)
		{
			_complexName[i].push_back(sl[j]);
		}
	}
	complexNum=I;

	line = in.readLine();    
	if(!line.contains("_physicalEntityName.size()"))
		return false;		
	sname=line.split('\t');	
	//nameSet.insert(sname[0]);				
	I=sname[1].toInt();
	_physicalEntityName.resize(I);
	for(int i=0; i<I; i++)
	{
		line = in.readLine();    
		QStringList sl=line.split('\t');	
		for(int j=1; j<sl.size(); j++)
		{
			_physicalEntityName[i].push_back(sl[j]);
		}
	}
	physicalEntityNum=I;


	line = in.readLine();    
	if(!line.contains("_proteinName.size()"))
		return false;		
	sname=line.split('\t');	
	//nameSet.insert(sname[0]);				
	I=sname[1].toInt();
	_proteinName.resize(I);
	for(int i=0; i<I; i++)
	{
		line = in.readLine();    
		QStringList sl=line.split('\t');	
		for(int j=1; j<sl.size(); j++)
		{
			_proteinName[i].push_back(sl[j]);
		}
	}
	proteinNum=I;

	line = in.readLine();    
	if(!line.contains("_reactionName.size()"))
		return false;		
	sname=line.split('\t');	
	//nameSet.insert(sname[0]);				
	I=sname[1].toInt();
	_reactionName.resize(I);
	for(int i=0; i<I; i++)
	{
		line = in.readLine();    
		QStringList sl=line.split('\t');	
		for(int j=1; j<sl.size(); j++)
		{
			_reactionName[i].push_back(sl[j]);
		}
	}
	reactionNum=I;

	line = in.readLine();    
	if(!line.contains("_smallMoleculeName.size()"))
		return false;		
	sname=line.split('\t');	
	//nameSet.insert(sname[0]);				
	I=sname[1].toInt();
	_smallMoleculeName.resize(I);
	for(int i=0; i<I; i++)
	{
		line = in.readLine();    
		QStringList sl=line.split('\t');	
		for(int j=1; j<sl.size(); j++)
		{
			_smallMoleculeName[i].push_back(sl[j]);
		}
	}
	smallMoleculeNum=I;


	line = in.readLine();    
	if(!line.contains("_DnaName.size()"))
		return false;		
	sname=line.split('\t');	
	//nameSet.insert(sname[0]);				
	I=sname[1].toInt();
	_DnaName.resize(I);
	for(int i=0; i<I; i++)
	{
		line = in.readLine();    
		QStringList sl=line.split('\t');	
		for(int j=1; j<sl.size(); j++)
		{
			_DnaName[i].push_back(sl[j]);
		}
	}
	DnaNum=I;
	
	line = in.readLine();    
	if(!line.contains("_EmptyName.size()"))
		return false;		
	sname=line.split('\t');	
	//nameSet.insert(sname[0]);				
	I=sname[1].toInt();
	_EmptyName.resize(I);
	for(int i=0; i<I; i++)
	{
		line = in.readLine();    
		QStringList sl=line.split('\t');	
		for(int j=1; j<sl.size(); j++)
		{
			_EmptyName[i].push_back(sl[j]);
		}
	}
	EmptyNum=I;

	line = in.readLine();    
	if(!line.contains("_ANodeName.size()"))
		return false;		
	sname=line.split('\t');	
	//nameSet.insert(sname[0]);				
	I=sname[1].toInt();
	_ANodeName.resize(I);
	for(int i=0; i<I; i++)
	{
		line = in.readLine();    
		QStringList sl=line.split('\t');	
		for(int j=1; j<sl.size(); j++)
		{
			_ANodeName[i].push_back(sl[j]);
		}
	}
	ANodeNum=I;

	line = in.readLine();    
	if(!line.contains("_compartmentName.size()"))
		return false;		
	sname=line.split('\t');	
	//nameSet.insert(sname[0]);				
	I=sname[1].toInt();
	_compartmentName.resize(I);
	for(int i=0; i<I; i++)
	{
		line = in.readLine();    
		QStringList sl=line.split('\t');	
		for(int j=1; j<sl.size(); j++)
		{
			_compartmentName[i].push_back(sl[j]);
		}
	}
	compartmentNum=I;

		
	edge = Edge_1;
	file.close(); 


	_protein.resize(proteinNum);
	_complex.resize(complexNum);	
	_entity.resize(physicalEntityNum);
	_ANode.resize(ANodeNum);
	_smallMolecule.resize(smallMoleculeNum);
	_Dna.resize(DnaNum);
	_reaction.resize(reactionNum);
	_compartment.resize(compartmentNum);
	_Empty.resize(EmptyNum);
	return true;
}

//compute the subtree node's location
void PathBubble1::HierarchicalTreeNodeLocation(bool whole, vector<vector<int>> _VertexInfo, vector<vector<int>> Rank0, vector<vector<vector<int>>> Ranks)
{
	//put the subtree to _newCompartmentPos location
 	vector<QRectF> newpos=_newPos;	
	vector<QRectF> newcompartmentpos=_newCompartmentPos;
	float nodeheight=0, nodewidth=0;
	if(whole)
	{
		if(Ranks.size()>0)
		{
			int roomID=_scene->CompartmentContain[m_pathwayID].size()-1.2;
			float x0=compartmentPos[roomID].x(), y0=compartmentPos[roomID].y();		
			float w=compartmentPos[roomID].width(), h=compartmentPos[roomID].height();	
		
			roomID=0;		
			for(int i=0; i<Ranks[roomID].size(); i++)
			{	
				for(int j=0; j<Ranks[roomID][i].size(); j++) 
				{
					if(Ranks[roomID][i][j]<newpos.size())
					{
						float x = newpos[Ranks[roomID][i][j]].center().x();
						float y = newpos[Ranks[roomID][i][j]].center().y();
						int tempi=Ranks[roomID][i][j];
			
						x=x*w+x0; y=y*h+y0;						
						newpos[Ranks[roomID][i][j]].moveCenter(QPointF(x,y));
					}
				}			
			}	
		}
	}
	else
	{
        //compute newcompartmentpos based on rank0's location and tanks's size info
		vector<float> compartmentRankHeight;
		vector<float> H;
		H.resize(Ranks.size());
		compartmentRankHeight.resize(Ranks.size());
		float y=0, Wmax=0;		
		vector<float> Wlist(Rank0.size(),0);
		for(int i=0; i<Rank0.size(); i++)
		{	
            float hmax = 1,wsum=0; 
			//preassign width
			for(int j=0; j<Rank0[i].size(); j++) 
			{
				if(Rank0[i][j]<_compartment.size())
				{					
					float h=Ranks[Rank0[i][j]].size()+1;
					compartmentRankHeight[Rank0[i][j]]=h;
					if(Rank0[i][j]==0)
						i=i;
					newcompartmentpos[Rank0[i][j]].setHeight(h);//H[Rank0[i][j]]=h;

				    if(h>hmax)
					  hmax=h;		
                    
					float wmax=0;
					for(int k=0; k< Ranks[Rank0[i][j]].size(); k++)
					{
						int w = Ranks[Rank0[i][j]][k].size();
						if(w>wmax)
					        wmax=w;							
					}					
					newcompartmentpos[Rank0[i][j]].setX(wsum);									
					newcompartmentpos[Rank0[i][j]].setWidth(wmax);					
					wsum = wsum + wmax;
				}
				else wsum = wsum + 1;
			}
			
			Wlist[i]=wsum;

			if(wsum>Wmax)
				Wmax=wsum;

			for(int j=0; j<Rank0[i].size(); j++) 
			{
				if(Rank0[i][j]<_compartment.size())
				{					
					newcompartmentpos[Rank0[i][j]].setY(y);
					//newcompartmentpos[Rank0[i][j]].setHeight(hmax);
					H[Rank0[i][j]]=hmax;
					newcompartmentpos[Rank0[i][j]].setHeight(hmax);
				}
			}			
			y=y+hmax;
		}	

		float targetSizex=Wmax*75, targetSizey=y*21;	
		if(targetSizex < targetSizey/1.618)
			targetSizex = targetSizey/1.618;
		else if(targetSizey < targetSizex/1.618)
			targetSizex = targetSizey*1.618;
		float scalex=targetSizex/graphReferenceSize, scaley=targetSizey/graphReferenceSize;
		float testScale=scaley/scalex;

		//adjust
		float Wmax1=0;//, y1=0;	//max width?
		for(int i=0; i<Rank0.size(); i++)
		{	
            float wsum=0; 			
			//test font rect //assum 500 defualt size
			float wsum1=0, wmax1, width1;
			//Wlist[i]=wsum;
			QRectF fontRect;
			for(int j=0; j<Rank0[i].size(); j++) 
			{
				if(Rank0[i][j]<_compartment.size())
				{	
					if(Rank0[i][j]==0)
						i=i;

					wmax1 = newcompartmentpos[Rank0[i][j]].width();
					wsum = newcompartmentpos[Rank0[i][j]].x();


					width1 = wmax1 / Wmax * graphReferenceSize;					
					QRectF fontRect = getCompartmentFont( _scene->_compartmentName[m_pathwayID][Rank0[i][j]][0], true, 1);
					if(fontRect.width()>wmax1*40)//width1/testScale)//Wmax*75
					{
						wmax1 = fontRect.width()/40; ///(graphReferenceSize/testScale)*Wmax;
					}
                   	
					newcompartmentpos[Rank0[i][j]].setX(wsum1);									
					newcompartmentpos[Rank0[i][j]].setWidth(wmax1);		
					wsum1 = wsum1 + wmax1;
				}
				else wsum1 = wsum1 + 1;
			}
			Wlist[i]=wsum1;
			//y1=y1+hmax1;
			
			if(wsum1>Wmax1)
				Wmax1=wsum1;
		}
		Wmax=Wmax1;

		////////////////////////////////

		///set y
		for(int i=0; i<Rank0.size(); i++)
		{	
			for(int j=0; j<Rank0[i].size(); j++) 
			{
				if(Rank0[i][j]<_compartment.size())
				{ 
					float w=newcompartmentpos[Rank0[i][j]].width(), h=newcompartmentpos[Rank0[i][j]].height();
					newcompartmentpos[Rank0[i][j]].setY(newcompartmentpos[Rank0[i][j]].y()/y + 0.0125 );//- h/y/2);				
					newcompartmentpos[Rank0[i][j]].setHeight(h/y*0.95);
					H[Rank0[i][j]]=H[Rank0[i][j]]/y*0.95;
				}
			}			
		}	
				
		for(int i=0; i<Rank0.size(); i++)
		{				
			for(int j=0; j<Rank0[i].size(); j++) 
			{
				if(Rank0[i][j]==0)
					i=i;
				if(Rank0[i][j]<_compartment.size())
				{
					float w=newcompartmentpos[Rank0[i][j]].width(), h=newcompartmentpos[Rank0[i][j]].height();
					float x=newcompartmentpos[Rank0[i][j]].x();
					float Wl=Wlist[i];					
					newcompartmentpos[Rank0[i][j]].setX((newcompartmentpos[Rank0[i][j]].x() - Wlist[i]/2.0)/Wmax + 0.525);					
                    newcompartmentpos[Rank0[i][j]].setWidth(w/Wmax*0.95);					
				}
			}			
		}
		// scale compartments		
		targetSizex=Wmax*75, targetSizey=y*21;	
		if(targetSizex < targetSizey/1.618)
			targetSizey = targetSizex*1.618;
		else if(targetSizey < targetSizex/1.618)
			targetSizey = targetSizex/1.618;
		scalex=targetSizex/graphReferenceSize, scaley=targetSizey/graphReferenceSize;

		//complexPos, proteinPos, smallMoleculePos, DnaPos, EmptyPos, reactionPos, physicalEntityPos, compartmentPos
		QPointF center;
		float w,h,cx,cy;
		for(int i=0; i<compartmentPos.size(); i++) //size?
		{
			if(newcompartmentpos[i].x()>-999.99)
			{
				 center=newcompartmentpos[i].center()-QPointF(0.5,0.5);
				 w=newcompartmentpos[i].width()*scalex;
				 h=newcompartmentpos[i].height()*scaley;		
				 H[i]=H[i]*scaley;	

				 center=QPointF(center.x()*scalex-w/2.0,center.y()*scaley-h/2.0);
				 center=center+QPointF(0.5,0.5);
				 newcompartmentpos[i]=QRectF(center.x(),center.y(), w, h);
			}
		}

		float minP, minC, minE, minD, minS, minR;
		minP= minC= minE= minD= minS = minR = 1000000;
		for(int roomID = 0; roomID < Ranks.size(); roomID++) 	    
		{
		float w=newcompartmentpos[roomID].width(), h=newcompartmentpos[roomID].height();		
		for(int i=0; i<Ranks[roomID].size(); i++)
		{			
			for(int j=0; j<Ranks[roomID][i].size(); j++) 
			{
				if(Ranks[roomID][i][j]<newpos.size())
				{
			        float pw=_newPos[Ranks[roomID][i][j]].width();
					switch(_VertexInfo[Ranks[roomID][i][j]][0])
					{
						case 'C': if(minC>pw*w) minC=pw*w; break;
						case 'E': if(minE>pw*w) minE=pw*w; break;
						case 'P': if(minP>pw*w) minP=pw*w; break;
						case 'S': if(minS>pw*w) minS=pw*w; break;
						case 'D': if(minD>pw*w) minD=pw*w; break;
						case 'R': if(minR>pw*w) minR=pw*w; break;
					}										
				}				
			}				
			if( nodewidth < Ranks[roomID][i].size() )
					nodewidth = Ranks[roomID][i].size();
		}	
		nodeheight = nodeheight + Ranks[roomID].size();
		}
		
		float y1;
		for(int roomID = 0; roomID < Ranks.size(); roomID++) 	  
		{
		float x0=newcompartmentpos[roomID].x(), y0=newcompartmentpos[roomID].y();		
		float w=newcompartmentpos[roomID].width(), h=newcompartmentpos[roomID].height();		
		for(int i=Ranks[roomID].size()-1; i>=0; i--)
		{			
			for(int j=0; j<Ranks[roomID][i].size(); j++) 
			{
				 //mark it if it is counted in two compartment
				int nid=Ranks[roomID][i][j];
		        if(nid==220)
					nid=nid;
				if(Ranks[roomID][i][j]<newpos.size())
				{
					float x = _newPos[Ranks[roomID][i][j]].center().x();
					float y = _newPos[Ranks[roomID][i][j]].center().y();
					int tempi=Ranks[roomID][i][j];

			        //pw=newpos[Ranks[roomID][i][j]].width();
					float pw,ph=_newPos[Ranks[roomID][i][j]].height();
					
					
					x=x*w+x0; 
					//y1=y * h * (compartmentRankHeight[roomID]-1.2)/compartmentRankHeight[roomID] + y0;
					y1=y * H[roomID] * (compartmentRankHeight[roomID]-1.2)/compartmentRankHeight[roomID] + y0;
					y=y*h+y0; 
                    float scalex,scaley;
					
					switch(_VertexInfo[Ranks[roomID][i][j]][0])
					{
						case 'C': pw=minC; scaley=1/1.61803399; break;
						case 'E': pw=minE; scaley=1.2; break;
						case 'P': pw=minP; scaley=0.8; break; //keqin minS
						case 'S': pw=minS; scaley=0.8; break; //keqin minS
						case 'D': pw=minD; scaley=0.8; break;
						case 'R': pw=minR; scaley=0.12; break;
					}					
					ph=ph*h* (compartmentRankHeight[roomID]-1.2)/compartmentRankHeight[roomID] *scaley; 	
					
					if(_VertexInfo[Ranks[roomID][i][j]][0]!='R')
					{
					   newpos[Ranks[roomID][i][j]].setWidth(pw);	
					   if(_VertexInfo[Ranks[roomID][i][j]].size()==4)
					   {
						   _VertexInfo[Ranks[roomID][i][j]].push_back(1);
						   newpos[Ranks[roomID][i][j]].setHeight(ph);	
						   if(ph>4 || ph<-4)
							   ph=ph;
					   }					   
					}		
					 if(y1>4 || y1<-4)
							   y1=y1;
					newpos[Ranks[roomID][i][j]].moveCenter(QPointF(x,y1));					
				}
			}			
		}	
		}
	}

	averageNodes(_VertexInfo, newpos);
	
	//replace the nodelocation with hierarchicle ones
	float itv=0.9/8;
	
	for(int i=0; i< _VertexInfo.size(); i++)
	{		
		int id=_VertexInfo[i][1];
		if(newpos[i].x()<-999.99||newpos[i].y()<-999.99) 
		{
			 newpos[i]=QRectF(-1000, -1000, newpos[i].width(), newpos[i].height());
		}	
		
		switch(int(_VertexInfo[i][0]))
		{
		    case 'C': complexPos[id]=newpos[i]; 				      
				      //complexPos[id].setHeight(complexPos[id].height()*1.25);
					  break;
			case 'E': physicalEntityPos[id]=newpos[i]; //if(physicalEntityPos[id].width()>itv) physicalEntityPos[id].setWidth(itv);
				break;
		    case 'P': proteinPos[id]=newpos[i];        //if(proteinPos[id].width()>itv)  proteinPos[id].setWidth(itv); 
				break;
		    case 'S': 
				smallMoleculePos[id]=newpos[i];  //if(smallMoleculePos[id].width()>itv) smallMoleculePos[id].setWidth(itv); 
				break;
		    case 'D': 
				DnaPos[id]=newpos[i];            //if(DnaPos[id].width()>itv) DnaPos[id].setWidth(itv); 
				break;
			case 'N': 
				EmptyPos[id]=newpos[i];          //if(EmptyPos[id].width()>itv) EmptyPos[id].setWidth(itv); 
				break;
			case 'R': 				
				reactionPos[id]=newpos[i];
				break;
		}			
	}
	for(int i=0; i<_VertexInfo.size(); i++)
	{
		int id=_VertexInfo[i][1];			   		
		if(_VertexInfo[i][0]=='R' && reactionPos[id].x() >-999.99 ) 
		{
           bool flag=false;		
		   for(int j=0; j<_Vertex[i].size(); j++)
	       {
			   for(int k=0; k<_Vertex[i][j].size(); k++)
			   {
				    if(_Vertex[i][j][k]<_VertexInfo.size())
					{
					int id0=_Edge[_Vertex[i][j][k]][0], id1=_Edge[_Vertex[i][j][k]][1];
					int id=i==id0?id1:id0;
				    int type=_VertexInfo[id][0];
					id=_VertexInfo[id][1];
			        switch(type)
					{
						case 'C': if(complexPos[id].x()>-999.99) flag=true;  break;
						case 'E': if(physicalEntityPos[id].x()>-999.99) flag=true; break;				
						case 'P': if(proteinPos[id].x()>-999.99) flag=true;   break;				
						case 'S': if(smallMoleculePos[id].x()>-999.99) flag=true; break;
						case 'D': if(DnaPos[id].x()>-999.99) flag=true;   break;
						case 'N': if(EmptyPos[id].x()>-999.99) flag=true; break;			            
					}	
					}
			   }			   
		   }	
		   vector<int> temp(2,0);
		   temp[0]='R',temp[1]=id;
		   if(!flag)
			   reactionRecord.erase(temp);
		}
	}
    newcompartmentpos.resize(compartmentPos.size());
	compartmentPos=newcompartmentpos;	
}


void PathBubble1::openPreNodeNotes()
{
 	QString pathName = _scene-> pwdParser-> savePathNotePreparation(_pathName, _scene->userName);	
	if(!pathName.contains(".note"))
			pathName +=".note";

	if(pathName==" .node" || pathName==".node")
		return;

	QFile file( pathName );

	if ( !file.open(QIODevice::ReadOnly) )
	{
		return;
	}

	QTextStream in(&file);    
	vector<QString> lines; 
	vector <int > Type, Id, Cid;
	vector<QString> Text;
	int count=0;
	while(!in.atEnd())
	{
	    QString  line = in.readLine();	
		QStringList block = line.split("<-:#:->");	
		QString text;		
		if(block.size()>1)
		{	
			count=0;
			for(int j=1; j<block.size(); j++)
			{
				vector<int> item;
				QString test = block[j];
				QStringList sname=block[j].split(' ');			
				for(int i=1; i<5 && i<sname.size(); i++)
				{
					QString s=sname[i];				 						
					int id=s.toInt();						
					item.push_back(id);
				}		
				
				if(sname.size()>4)
				{
					int type=item[1];
					int id=item[2];	
					Cid.push_back(item[3]);
					Type.push_back(type);
					Id.push_back(id);
					Text.push_back(text);
				}
			}			
		}	
		else
		{
			count++;
			if(Text.size()>0)
			{
				if(count==1)
				{
				
					Text[Text.size()-1]= Text[Text.size()-1] + "\n" + line;
				}
				else
				{
					Text[Text.size()-1]= Text[Text.size()-1] + "\n" + line;
				}
			}
		}
    }	
	for(int i=0; i<Type.size(); i++)
	{
	    int type=Type[i];
		int id=Id[i];
		int cid=Cid[i];
		bool flag=true;
		vector<QString> text;
		for(int j=i+1; j<Type.size(); j++)
		{
			if(Type[j]==type && Id[j]==id && Cid[j]==cid)
			{
				 flag=false;			    								 
				 break;
			}
		}
		if(flag)
		{
			//text=Text[i];
			QString T;
			for(int k=0; k<Type.size(); k++)
			{
				if(Type[k]==type && Id[k]==id)
				{
					if(Text[k].indexOf(" ")==0)
					{	
						int index= Text[k].indexOf(" ");
						Text[k] = Text[k].mid(index+1, Text[k].size()-index);		
					}
				    text.push_back(Text[k]);							
					T=T+Text[k];
				}				  
			}
			for(int i=0;i<T.size();i++)
			{
				QString p=T[i];
				
			}
			QPointF pos;
			bool flag=false;
			switch(type)
			{
			case 'C': if(id>=complexRect.size() || complexRect[id].width()<0) { flag=true;  break; }
							  pos=complexRect[id].center();  break;
				case 'E': if(id>=physicalEntityRect.size() || physicalEntityRect[id].width()<0)  { flag=true;  break; }
					          pos=physicalEntityRect[id].center();   break;				
				case 'S': if(id>=smallMoleculeRect.size() || smallMoleculeRect[id].width()<0)  { flag=true;  break; }
					          pos=smallMoleculeRect[id].center();   break;							
				case 'P': if(id>=proteinRect.size() || proteinRect[id].width()<0)  { flag=true;  break; }
					          pos=proteinRect[id].center();   break;			 
				case 'D': if(id>=DnaRect.size() || DnaRect[id].width()<0)  { flag=true;  break; }
					          pos=DnaRect[id].center();   break;		
				case 'N': if(id>=EmptyRect.size() || EmptyRect[id].width()<0)  { flag=true;  break; }
					          pos=EmptyRect[id].center();   break;
				case 'R': if(id>=reactionRect.size() || reactionRect[id].width()<0)  { flag=true;  break; }
					          pos=reactionRect[id].center();   break;
				case 'L': if(id>=ANodeRect.size() || ANodeRect[id].width()<0)  { flag=true;  break; }
					          pos=ANodeRect[id].center();   break;
				case 'M': if(id>=compartmentRect.size() || compartmentRect[id].width()<0)  { flag=true;  break; }
					          pos=compartmentRect[id].center();   break;
			}	
			if(!flag)
			{	
		       _scene->openNodeNote(m_pathwayID, type, id, this, pos + this->scenePos(), text, Cid, false);		
			}
		}
	}
  
   file.close();
}

void PathBubble1::updateConnectionAfterDeletion()
{
	set<vector<vector<int>>> connections;
	for(int i=0; i<4; i++)
	{
		switch(i)
		{
			case 0: connections=_scene->edgeconnections;  break;
		    case 1: connections=_scene->edgeconnections_A;  break;
			case 2: connections=_scene->edgeconnections_I;  break;
			case 3: connections=_scene->edgeconnections_4;  break;
		}
		set<vector<vector<int>>> tConnections=connections;
		for (set<vector<vector<int>>>::iterator it = connections.begin(); it != connections.end(); it++)
		{
			vector<vector<int>> link = *it;				
			vector<int> link0=*link.begin(),link1=*link.rbegin();
			int pid1=link0[0], pid2=link1[0], type1=link0[1], type2=link1[1], id1=link0[2], id2=link1[2];
			if( pid1 == m_pathwayID && pid2==m_pathwayID)
			{
			   tConnections.erase(link);			
			}
		}
		switch(i)
		{
			case 0: _scene->edgeconnections=tConnections;  break;
		    case 1: _scene->edgeconnections_A=tConnections;  break;
			case 2: _scene->edgeconnections_I=tConnections;  break;
			case 3: _scene->edgeconnections_4=tConnections;  break;
		}
	}
}			

void PathBubble1::updateTreeRingConnectionAfterDeletion()
{
	for(int i=0; i<_scene->m_treeRingBubbles.size(); i++)
	{
		if(_scene->m_treeRingBubbles[i]!=NULL)
		for (int j = 0; j <_scene->m_treeRingBubbles[i]->_connections.size(); j++)
		{
			struct treeRingConnector node= _scene->m_treeRingBubbles[i]->_connections[j];
		    ItemBase *bubble=node.childBubble;
			if(bubble!=NULL)
			{
				if( bubble->getType()==SUBPATHBUBBLE1 || bubble->getType()==PATHBUBBLE1)
				{
					PathBubble1* tbubble= dynamic_cast<PathBubble1*>(bubble);
					if(tbubble->m_pathwayID == m_pathwayID)
					{
						//keqin temporary disconnect it with treeRing
						_scene->m_treeRingBubbles[i]->_connections[j].childBubble=NULL;
						/*for (int k = 0; k < _connections.size(); k++)
						{
							_scene->m_treeRingBubbles[i]->_connections.push_back(_connections[k]);
						}*/
						vector<struct treeRingConnector> tempConnecton;
						for (int k = 0; k < _scene->m_treeRingBubbles[i]->_connections.size(); k++)
						{
							if(_scene->m_treeRingBubbles[i]->_connections[k].childBubble!=NULL)
						        tempConnecton.push_back(_scene->m_treeRingBubbles[i]->_connections[k]);
						}
						_scene->m_treeRingBubbles[i]->_connections=tempConnecton;
					}		    
				}
				else if(bubble->getType()==TREERING)
				{
								
				}
			}
		}	    
	}
}


PathBubble1::~PathBubble1()
{
	if(_scene!=NULL )
	{
		QString pathName;
		for( int i = 0; i <_scene->_nodeNote.size(); i++)
		{
			int pid=_scene->_nodeNote[i]->_pid;
			//if( !_scene->_nodeNote[i]->deleted )
			{
				if(pid==this->m_pathwayID)
				{
					if(pathName.size()==0)
					    pathName = _scene-> pwdParser-> savePathNotePreparation(_pathName, _scene->userName);	
					_scene->_nodeNote[i]->record(pathName, _scene->_nodeNote[i]->deleted);	

					_scene->_nodeNote[i]->deleted=true;
					scene->_nodeNote[i]->hide();
					scene->_nodeNote[i]->m_note->hide();						
		            scene->_nodeNote[i]->m_note->setVisible(false);
				}
			}			
		}	
		updateConnectionAfterDeletion(); 	
		removeFromGraph(); //remove edges from the whole graph
		updateTreeRingConnectionAfterDeletion();
	}

	int Pid;
	QList<PathBubble1 *> _pathBubbles;
	for(int i=0; i<_scene->_pathBubbles.size();	i++)
	{   
		if(_scene->_pathBubbles[i] && _scene->_pathBubbles[i]->m_pathwayID == m_pathwayID)
		{
		   // _scene->Rank0[i].clear();
			//_scene->Ranks[i].clear();
			//_scene->newCompartmentPos[i].clear();
			//_scene->newPos[i].clear();
			Pid=i;
		}		
	}
	_scene->_pathBubbles[m_pathwayID]=NULL;	
	vector<vector<vector<int>>> _complexContain;
	for(int i=0; i<_scene->_complexContain.size();	i++)
	{
	    if(i==Pid)    		
		    _scene->_complexContain[i].clear();	
	}

	vector<vector<vector<vector<int>>>> _biochemicalReaction;
	for(int i=0; i<_scene->_biochemicalReaction.size();	i++)
	{
	    if(i==Pid)
			_scene->_biochemicalReaction[i].clear();
	}

	/*vector<vector<vector<vector<int>>>> _ANode;
	for(int i=0; i<_scene->_ANode.size();	i++)
	{
	    if(i==Pid)
			_scene->_ANode[i].clear();
	}*/
	
	vector<vector<vector<vector<int>>>> _degradation;
	for(int i=0; i<_scene->_degradation.size();	i++)
	{
	    if(i==Pid)
			_scene->_degradation[i].clear();
	}
	vector<vector<set<vector<int>>>> CompartmentContain;
	for(int i=0; i<_scene->CompartmentContain.size();	i++)
	{
	    if(i==Pid)
			_scene->CompartmentContain[i].clear();
	}
	vector<vector<vector<QString>>> _compartmentName;
	for(int i=0; i<_scene->_compartmentName.size();	i++)
	{
	    if(i==Pid)
			_scene->_compartmentName[i].clear();
	}


	
	vector<vector<QString>> _quantityName;
	vector<vector<vector<float>>> _quantity;
	vector<vector<vector<float>>> _minmax;
	vector<vector<vector<QString>>> _geneInfo; 
	for(int i=0; i<_scene->_quantityName.size(); i++)
	{
	    if(i==Pid)
		{
			_scene->_quantityName[i].clear();
			_scene->_quantity[i].clear();
			_scene->_minmax[i].clear();
			_scene->_geneInfo[i].clear();
		}
	}
	
	//vector<vector<QRectF>> _complexPos, _proteinPos, _smallMoleculePos, _DnaPos, _degenerationPos, _reactionPos, _physicalEntityPos, _compartmentPos, _EmptyPos;	
	//vector<int> _Scale; 
	//vector<QPointF> _dCenter;
	//vector<vector<vector<int>>> _edge;
	for(int i=0; i<_scene->_complexPos.size();	i++)
	{
	    if(i==Pid)    		
		{	
			_scene->_complexPos[i].clear();
			_scene->_proteinPos[i].clear();
			_scene->_smallMoleculePos[i].clear();
			_scene->_DnaPos[i].clear();			
			_scene->_reactionPos[i].clear();
			_scene->_physicalEntityPos[i].clear(); 
			_scene->_compartmentPos[i].clear(); 
			_scene->_EmptyPos[i].clear(); 
			_scene->_edge[i].clear(); 		
			_scene->_ANodePos[i].clear(); 
		}
	}
	//subgraph

	vector<vector<vector<int>>> temp;
	for (set<vector<vector<int>>>::iterator it = _scene->edgeconnections.begin(); it != _scene->edgeconnections.end(); it++)
	{
        vector<vector<int>> link = *it;	
        vector<int> link0=*link.begin(),link1=*link.rbegin();
		int pid1=link0[0], pid2=link1[0], type1=link0[1], type2=link1[1], id1=link0[2], id2=link1[2];
        QRectF itemPos1,itemPos2; 
		PathBubble1* path1=_scene->_pathBubbles[pid1], * path2=_scene->_pathBubbles[pid2];

	    if(pid1==m_pathwayID||pid2==m_pathwayID)
		{
			//_scene->edgeconnections.erase(link);	
			temp.push_back(link);
		}
    }
	for(int i=0;i<temp.size();i++)
	{ 
	   _scene->edgeconnections.erase(temp[i]);	
	}	
	//_pieMenuState=0;
	_mousePressed=false;
	_scene->controlKeyisOn=false;
	highlighted.clear();	
	for(int i=0; i<_scene->_pathBubbles.size();	i++)
	{
		if(_scene->_pathBubbles[i] && _scene->_pathBubbles[i]->m_pathwayID == m_pathwayID)    		
		{
			_scene->_pathBubbles[i]=NULL;		
		}
	}
	
	
	//qtbox
	_lines.clear(); 
	sharedExpressionProtein.clear(); 
	expressedProtein.clear();  expressedComplex.clear();  expressedSmallMolecule.clear();  expressedDna.clear();  expressedReaction.clear();  expressedPhysicalEntity.clear();  expressedANode.clear(); expressedCompartment.clear();  expressedPathway.clear(); 	
	sharedProtein.clear();  sharedComplex.clear();  sharedSmallMolecule.clear();  sharedDna.clear();  sharedReaction.clear();  sharedPhysicalEntity.clear();  sharedANode.clear();  sharedCompartment.clear();  sharedPathway.clear(); 
	linkedProtein.clear();  linkedComplex.clear();  linkedSmallMolecule.clear();  linkedDna.clear();  linkedReaction.clear();  linkedPhysicalEntity.clear();  linkedANode.clear(); linkedCompartment.clear();  linkedPathway.clear(); 
	linkedEdge.clear(); 
	differedProtein.clear();  differedComplex.clear();  differedSmallMolecule.clear();  differedDna.clear();  differedReaction.clear();  differedPhysicalEntity.clear();  differedANode.clear();  differedCompartment.clear();  differedPathway.clear(); 
	_itemColored.clear(); 
	foundlist.clear(); 
	//_linkage.clear(); 	
	highlightedEdge.clear(); 
	hoveredItem.clear(); 
	eventStored.clear(); 	
	sharedStored.clear();  differedStored.clear();  expressedStored.clear(); highlightedStored.clear(); 
	linkedStored.clear(); 
	linkedStoredinHead.clear(); 				
	pathwayGraphStored.clear(); 	
	
	complexPos.clear();
	proteinPos.clear();	
    smallMoleculePos.clear();	
    DnaPos.clear();	
    reactionPos.clear();	
    physicalEntityPos.clear();	
	edge.clear();
	CompartmentContain.clear();

	_conPoint.clear();	_pathBubbles.clear();	itemInfo.clear();
	boundingBoxes.clear();	complexRect.clear(); proteinRect.clear();  smallMoleculeRect.clear(); DnaRect.clear();	reactionRect.clear();     degradationRect.clear(); 	physicalEntityRect.clear();	compartmentRect.clear(); 	EmptyRect.clear();  ANodeRect.clear(); //real position
	fontRect.clear();	boundaryWidth.clear();	fontSize.clear(); 	boundaryColor.clear(); 	fontColor.clear();	insideColor.clear();	elipseVector.clear();	arrowVector.clear();	
	
	itemDragged.clear();		
	itemSelected.clear();
	fills.clear();	
	updatedEdge_1.clear();
	updatedEdge_2.clear();
	updatedEdge_3.clear(); 
	updatedEdge_4.clear();

	virtualCompartment.clear();
	_itemSelected.clear();		
	_codeBubbles.clear();
	_reactionBubbles.clear();
	_groupBubbles.clear();
	_pathBubbles.clear();
	_conPoint.clear();
	_reactionPoint.clear();
	_groupPoint.clear();
	_path2Point.clear();
	_conhighlight.clear(); 
	_reactionhighlight.clear();
	_grouphighlight.clear();
	_path2highlight.clear();
	Rank0.clear(); //for compartment
	Ranks.clear();
	reactionRecord.clear(); 
	edge.clear();		
	complexPos.clear();
	proteinPos.clear();
	smallMoleculePos.clear(); DnaPos.clear(); reactionPos.clear(); degradationPos.clear(); physicalEntityPos.clear(); compartmentPos.clear(); EmptyPos.clear(); ANodePos.clear(); //0-1
	complexPos_0.clear(); proteinPos_0.clear(); smallMoleculePos_0.clear(); DnaPos_0.clear(); reactionPos_0.clear(); degradationPos_0.clear(); physicalEntityPos_0.clear(); compartmentPos_0.clear(); EmptyPos_0.clear();  ANodePos_0.clear(); 
	complexPos_1.clear(); proteinPos_1.clear(); smallMoleculePos_1.clear(); DnaPos_1.clear(); reactionPos_1.clear(); degradationPos_1.clear(); physicalEntityPos_1.clear(); compartmentPos_1.clear(); EmptyPos_1.clear();  ANodePos_1.clear(); 
	
	boundingBoxes.clear();
	complexRect.clear(); proteinRect.clear(); smallMoleculeRect.clear(); DnaRect.clear(); reactionRect.clear(); degradationRect.clear(); physicalEntityRect.clear(); compartmentRect.clear(); EmptyRect.clear(), ANodeRect.clear();  //real position
	fontRect.clear();
	boundaryWidth.clear();
	fontSize.clear();
	boundaryColor.clear(); fontColor.clear();
	insideColor.clear();
	elipseVector.clear();
	arrowVector.clear();

	itemGrid.clear();		
	_VertexInfo.clear(); _Edge.clear();
	_Vertex.clear(); 
	_newPos.clear(); _newCompartmentPos.clear();
	_polyEdge.clear();
	smallMolecule.clear(); Dna.clear();
	itemInfo.clear();  //subpath way infor for connections?
	itemPos.clear();	
	_groupEdgeAffected.clear();
	_edgeAffected.clear(); _edgeSelected.clear();
	//markedD.clear(); markedS.clear(); //for rearrange their compartmentID.clear();
	CompartmentContain.clear(); CompartmentContain_0.clear(); CompartmentContain_1.clear();
	_1pathway.clear();
	_pathwayStep.clear();
	_3pathwayStepCatalysis.clear(); //catalysis- control - Degradation - pathway
	_3pathwayStepReaction.clear();	
		
	_Catalysis.clear(); _5Control.clear(); 		
	//XL.clear(); YB.clear(); XR.clear(); YT.clear();

	//_scene->pwdParser->findFillsinFolder("data/Reactome_Pathway_Data/pathwayGraph", _pathName);

	 QString fileName = "";//fileComboBox->currentText();//"_1pathway" only work on emapty "" case
     QString text = "";//textComboBox->currentText();
    
     //First we eliminate any previous search results by setting the table widgets row count to zero. Then we retrieve the specified file name, text and directory path from the respective comboboxes.
	 int count=0;
     QDir currentDir = QDir("data/Reactome_Pathway_Data/pathwayGraph");
     QStringList files;
     if (fileName.isEmpty())
         fileName = "*";
    files = currentDir.entryList(QStringList(fileName),
                                  QDir::Files | QDir::NoSymLinks);

    if (!text.isEmpty())
         files = _scene->pwdParser->findFiles(currentDir, files, text);
	
	QString pureName;
	QString num;
	int lid;	

	num = _curFileName;
	lid = num.lastIndexOf('/');
	num = num.mid(lid+1,num.size()-lid);
	lid = num.indexOf('_');
	num = num.mid(0,lid);

	_pathName = _pathName + "_" + QString::number(m_pathwayID);
	num = num + "_" + QString::number(m_pathwayID);

	for (int i = 0; i < files.size(); ++i) 
	{
	        lid=files[i].lastIndexOf('.');
			pureName = files[i];
			pureName = pureName.mid(0, lid);
            lid = pureName.lastIndexOf('_');
			pureName = pureName.mid(0, lid);
			if(pureName==_pathName || pureName==num) //remove this line if use folder
			{
		        //count = num.toInt(); //convert QString to ini/ convert QString to num
				currentDir.remove(files[i]);
			}
	}

	m_pathwayID=-1;
	/* QDir currentDir= QDir("data/Reactome_Pathway_Data/pathwayGraph");
    //QDir currentDir = QDir(path);
    char filepath[256];
	 QStringList files;

    //theFolder = QOpenDir("data/Reactome_Pathway_Data/pathwayGraph");
	 files = currentDir.entryList(QStringList(""),
                                  QDir::Files | QDir::NoSymLinks);

    for (int i = 0; i < files.size(); ++i) 
	{
	     currentDir.remove(files[i]);
	}
    
	*/
	
	
}



bool PathBubble1::checkUniqueness(int type, vector<vector<QString>> Names, vector<QString> name)
{//if find same item return flase;
	int flag;
   for(int j=0; j<Names.size(); j++)
   {
	   flag=name.size();
	   for(int i=0; i<name.size(); i++)
       {   
          if(Names[j][i]==name[i])
		  {
	          flag--;				  
		  }      
	  }
	  if(flag==0)
		  break;
   }   
   return flag;
}

bool PathBubble1::checkRectionUniqueness(int pid, int id, set<vector<int>> sharedNodes)
{//if find the same item return flase;
	  vector<vector<int>> nodes = _scene->_pathBubbles[pid]->_reaction[id]; 		
	  vector<int> node(2,0);
	  bool flag=false;
	  for(int k=0; k<nodes.size(); k++)
	  {	
	 	 node[0]=nodes[k][0];
	  	 node[1]=nodes[k][1];
		 if(sharedNodes.find(node)==sharedNodes.end())
		 {
			 flag=true;
			 break;
		 }
	  }   
	  return flag;
}

void PathBubble1::mergeGraph(int id1, int id2)
{
    //merge   
    QString tname;
    const char * ename;
	string sname;
	float x,y,w,h;

	//int id1=_scene->_complexName.size()-1;
	//not consider the location yet and same same yet
	set<vector<int>> sharedNodes; //set node (pathway2) found same item in pathway1
	vector<int> node(2,0);
    
     
		for(int i=0; i<_scene->_complexPos[id2].size(); i++)
	    {
			if(checkUniqueness('C',_scene->_complexNameD[_scene->_pathBubbles[id1]->dataID], _scene->_complexNameD[_scene->_pathBubbles[id2]->dataID][i]))
			{
			_scene->_complexPos[id1].push_back(_scene->_complexPos[id2][i]); 
			complexPos.push_back(_scene->_complexPos[id2][i]); 
		    _scene->_complexNameD[_scene->_pathBubbles[id1]->dataID].push_back(_scene->_complexNameD[_scene->_pathBubbles[id2]->dataID][i]);					  
			}
			else
			{
				node[0]='C', node[1]=i;
				sharedNodes.insert(node);
			}
		}

		for(int i=0; i<_scene->_physicalEntityPos[id2].size(); i++)
	    {		
		   if(checkUniqueness('E', _scene->_physicalEntityNameD[_scene->_pathBubbles[id1]->dataID], _scene->_physicalEntityNameD[_scene->_pathBubbles[id2]->dataID][i]))
		   {
				_scene->_physicalEntityPos[id1].push_back(_scene->_physicalEntityPos[id2][i]); 
				physicalEntityPos.push_back(_scene->_physicalEntityPos[id2][i]); 
				_scene->_physicalEntityNameD[_scene->_pathBubbles[id1]->dataID].push_back(_scene->_physicalEntityNameD[_scene->_pathBubbles[id2]->dataID][i]);			   
		   }
		   else
			{
				node[0]='E', node[1]=i;
				sharedNodes.insert(node);
			}
		}

		for(int i=0; i<_scene->_proteinPos[id2].size(); i++)
	    {
			if(checkUniqueness('P', _scene->_proteinNameD[_scene->_pathBubbles[id1]->dataID], _scene->_proteinNameD[_scene->_pathBubbles[id2]->dataID][i]))
			{
		       _scene->_proteinPos[id1].push_back(_scene->_proteinPos[id2][i]);  
		       proteinPos.push_back(_scene->_proteinPos[id2][i]);  
		       _scene->_proteinNameD[_scene->_pathBubbles[id1]->dataID].push_back(_scene->_proteinNameD[_scene->_pathBubbles[id2]->dataID][i]);
		    }
			else
			{
				node[0]='P', node[1]=i;
				sharedNodes.insert(node);
			}
		}
		for(int i=0; i<_scene->_ANodePos[id2].size(); i++)
	    {
			if(checkUniqueness('P', _scene->_ANodeName[id1], _scene->_ANodeName[id2][i]))
			{
		       _scene->_ANodePos[id1].push_back(_scene->_ANodePos[id2][i]);  
		       ANodePos.push_back(_scene->_ANodePos[id2][i]);  
		       _scene->_ANodeName[id1].push_back(_scene->_ANodeName[id2][i]);
		    }
			else
			{
				node[0]='P', node[1]=i;
				sharedNodes.insert(node);
			}
		}
		for(int i=0; i<_scene->_smallMoleculePos[id2].size(); i++)
	    {    
			if(checkUniqueness('S', _scene->_smallMoleculeNameD[_scene->_pathBubbles[id1]->dataID], _scene->_smallMoleculeNameD[_scene->_pathBubbles[id2]->dataID][i]))
			{
				_scene->_smallMoleculePos[id1].push_back(_scene->_smallMoleculePos[id2][i]); 
				smallMoleculePos.push_back(_scene->_smallMoleculePos[id2][i]); 
				_scene->_smallMoleculeNameD[_scene->_pathBubbles[id1]->dataID].push_back(_scene->_smallMoleculeNameD[_scene->_pathBubbles[id2]->dataID][i]);	
			}
			else
			{
				node[0]='S', node[1]=i;
				sharedNodes.insert(node);
			}
		}

		for(int i=0; i<_scene->_DnaPos[id2].size(); i++)
	    {    
			if(checkUniqueness('D', _scene->_DnaNameD[_scene->_pathBubbles[id1]->dataID], _scene->_DnaNameD[_scene->_pathBubbles[id2]->dataID][i]))
			{
				_scene->_DnaPos[id1].push_back(_scene->_DnaPos[id2][i]); 
				smallMoleculePos.push_back(_scene->_smallMoleculePos[id2][i]); 
				_scene->_DnaNameD[_scene->_pathBubbles[id1]->dataID].push_back(_scene->_DnaNameD[_scene->_pathBubbles[id2]->dataID][i]);	
			}
			else
			{
				node[0]='D', node[1]=i;
				sharedNodes.insert(node);
			}
		}

		for(int i=0; i<_scene->_reactionPos[id2].size(); i++)
	    {    
			if(checkUniqueness('R', _scene->_reactionNameD[_scene->_pathBubbles[id1]->dataID], _scene->_reactionNameD[_scene->_pathBubbles[id2]->dataID][i])&&checkRectionUniqueness(id2, i, sharedNodes))
			{				
				   _scene->_reactionPos[id1].push_back(_scene->_reactionPos[id2][i]); 
				    reactionPos.push_back(_scene->_reactionPos[id2][i]); 
				   _scene->_reactionNameD[_scene->_pathBubbles[id1]->dataID].push_back(_scene->_reactionNameD[_scene->_pathBubbles[id2]->dataID][i]);						
			}
			else
			    {
				   node[0]='R', node[1]=i;
				   sharedNodes.insert(node);
			    }
	    }
		int Cnum,Enum,Pnum,Snum,Dnum,Rnum,Anum;
		int Cnum1,Enum1,Pnum1,Snum1,Dnum1,Rnum1,Anum1;
		Cnum1=Enum1=Pnum1=Snum1=Dnum1=Rnum1=Anum1=0;

	    Cnum=_scene->_pathBubbles[id1]->_complex.size();
	    Enum=_scene->_pathBubbles[id1]->_entity.size();
	    Pnum=_scene->_pathBubbles[id1]->_protein.size();
		Anum=_scene->_pathBubbles[id1]->_ANode.size();
	    Snum=_scene->_pathBubbles[id1]->_smallMolecule.size();
	    Dnum=_scene->_pathBubbles[id1]->_Dna.size();
	    Rnum=_scene->_pathBubbles[id1]->_reaction.size();

	    vector<vector<int>> edge2=_scene->_pathBubbles[id2]->edge;
		
	
     for(int i=0; i<edge2.size(); i++)
	 {
		 node[0]=edge2[i][4], node[1]=edge2[i][5];		 
		 if(sharedNodes.find(node)==sharedNodes.end()&&node[1]>=0)
		 {
			 bool flag=false;//there may be more than one reaction stated in same edge
			 for(int j=0; j<6; j+=2)
			 {
				 node[0]=edge2[i][j], node[1]=edge2[i][j+1];
				 if(sharedNodes.find(node)==sharedNodes.end())
				 {
					 switch(edge2[i][j])
					 {
						 case 'C': edge2[i][j+1]=Cnum+Cnum1; Cnum1++;  break;
						 case 'D': edge2[i][j+1]=Dnum+Dnum1; Dnum1++;   break;
						 case 'E': edge2[i][j+1]=Enum+Enum1; Enum1++;   break;
						 case 'P': edge2[i][j+1]=Pnum+Pnum1; Pnum1++;   break;
						 case 'L': edge2[i][j+1]=Anum+Anum1; Anum1++;   break;
						 case 'S': edge2[i][j+1]=Snum+Snum1; Snum1++;   break;
						 case 'R': edge2[i][j+1]=Rnum+Rnum1; Rnum1++;  break;
					 }	
					 sharedNodes.insert(node);
				 }
			}
		    _scene->_pathBubbles[id1]->edge.push_back(edge2[i]);
		}
	 }
	
}

void PathBubble1::sharedNodetoHighlighted(bool toHighlight)
{
	vector<PathBubble1 *> mList=_scene->extractPathBubblesinGroup(this);
    for(int j=0; j<mList.size(); j++) 
	{
		PathBubble1* pBubble = mList[j];			
		for(int m=0; m<9; m++)
		{
			int ctype;
			vector<set<int>> sharedNode;
			 vector<vector<QRectF>> nodePos;
			switch(m)
			{
				case 0: ctype='P';  sharedNode=pBubble->sharedProtein;        nodePos=_scene->_proteinPos;         break;
				case 1: ctype='S';  sharedNode=pBubble->sharedSmallMolecule;  nodePos=_scene->_smallMoleculePos;  break;
				case 2: ctype='C';  sharedNode=pBubble->sharedComplex;        nodePos=_scene->_complexPos;   break;
				case 3: ctype='D';  sharedNode=pBubble->sharedDna;            nodePos=_scene->_DnaPos;     break;
				case 4: ctype='R';  sharedNode=pBubble->sharedReaction;       nodePos=_scene->_reactionPos;  break;
				case 5: ctype='E';  sharedNode=pBubble->sharedPhysicalEntity; nodePos=_scene->_physicalEntityPos; break;					
				case 6: ctype='L';  sharedNode=pBubble->sharedANode;          nodePos=_scene->_ANodePos; break;	
				case 7: ctype='M';  sharedNode=pBubble->sharedCompartment;    nodePos=_scene->_compartmentPos; break;	
				case 8: ctype='W';  sharedNode=pBubble->sharedPathway; break;	
			}	

		
			if(toHighlight)
		    {
				vector<PathBubble1 *> mlist=mList;	 
				for(int i=0; i<mlist.size(); i++) 
				{
					PathBubble1* pbubble = mlist[i];		 
					//for(int i=0;i<sharedNode[pbubble->m_pathwayID].size();i++)			
					if(sharedNode.size()>pbubble->m_pathwayID)
					for(set<int>::iterator it=sharedNode[pbubble->m_pathwayID].begin(); it!=sharedNode[pbubble->m_pathwayID].end(); it++)
					{
						int id=*it;
						vector<int> temp(2,0);
						temp[0]=ctype, temp[1]=id;
						if(m==8 || ( id<nodePos[pbubble->m_pathwayID].size() && nodePos[pbubble->m_pathwayID][id].x()>-990.0 ) ) //a bug fix here
						    pbubble->highlighted.insert(temp);
					}
				}
			}
		}	
	}
}

void PathBubble1::sharedDifferedLinkedExpressedHighLightedSizeCheck()
{
	int fsize=_scene->_pathBubbles.size();
	int esize=_scene->_expressionBubbles.size();

	vector<PathBubble1 *> mList=_scene->extractPathBubblesinGroup(this);
	if(headPathway==NULL)
       headPathway = _scene->findASmallestPathinGroup(this);
	
	
    for(int i=0; i< mList.size(); i++) 
	{
	 	 PathBubble1* pBubble = mList[i];			 
		 if(pBubble->differedProtein.size()<fsize)
		 {
				pBubble->differedProtein.resize(fsize);
  				pBubble->differedSmallMolecule.resize(fsize);
				pBubble->differedComplex.resize(fsize);
				pBubble->differedDna.resize(fsize); 
				pBubble->differedReaction.resize(fsize);
				pBubble->differedPhysicalEntity.resize(fsize);
				pBubble->differedANode.resize(fsize);				
		 }
		if(pBubble->sharedProtein.size()<fsize)
		{
				pBubble->sharedProtein.resize(fsize);
  				pBubble->sharedSmallMolecule.resize(fsize);
				pBubble->sharedComplex.resize(fsize);
				pBubble->sharedDna.resize(fsize); 
				pBubble->sharedReaction.resize(fsize);
				pBubble->sharedPhysicalEntity.resize(fsize);
				pBubble->sharedANode.resize(fsize);				
		}

		if(pBubble->linkedProtein.size()<fsize)
		{
				pBubble->linkedProtein.resize(fsize);
  				pBubble->linkedSmallMolecule.resize(fsize);
				pBubble->linkedComplex.resize(fsize);
				pBubble->linkedDna.resize(fsize); 
				pBubble->linkedReaction.resize(fsize);
				pBubble->linkedPhysicalEntity.resize(fsize);
				pBubble->linkedANode.resize(fsize);				
		 }
		 if(pBubble->expressedProtein.size()<esize)
		 {
				pBubble->expressedProtein.resize(esize);
  				pBubble->expressedSmallMolecule.resize(esize);
				pBubble->expressedComplex.resize(esize);
				pBubble->expressedDna.resize(esize); 
				pBubble->expressedReaction.resize(esize);
				pBubble->expressedPhysicalEntity.resize(esize);
				pBubble->expressedANode.resize(esize);				
		 }

		 if(pBubble->sharedCompartment.size()<fsize)
		 {
			 pBubble->sharedCompartment.resize(fsize);
		 }
		 if(pBubble->expressedCompartment.size()<fsize)
		 {
			 pBubble->expressedCompartment.resize(fsize);
		 }
		 if(pBubble->differedCompartment.size()<fsize)
		 {
			 pBubble->differedCompartment.resize(fsize);		
		 }
		 if(pBubble->linkedCompartment.size()<fsize)
		 {
			 pBubble->linkedCompartment.resize(fsize);					
		 }

		//pathway

		if(pBubble->sharedPathway.size()<fsize)
		{
			pBubble->sharedPathway.resize(fsize);
		}
		if(pBubble->expressedPathway.size()<fsize)
		{
			pBubble->expressedPathway.resize(fsize);
		}
		if(pBubble->differedPathway.size()<fsize)
		{
			pBubble->differedPathway.resize(fsize);
		
		}
		if(pBubble->linkedPathway.size()<fsize)
		{
			pBubble->linkedPathway.resize(fsize);		
			
		}
	}
}

void PathBubble1::reMoveNotShared(vector<set<vector<int>>> HBackup)
{
	vector<PathBubble1 *> mList=_scene->extractPathBubblesinGroup(this);	
	int count;
	int ID,PID;
	for(int i=0; i< mList.size(); i++) 
	{	
		 PathBubble1* pbubble = mList[i];	
		 set<vector<int>> hBackup=HBackup[i];
		 for(set<vector<int>>::iterator it=hBackup.begin(); it!=hBackup.end(); it++)
		 {
			count=0;
			vector<int> temp=*it;
			int pid=pbubble->m_pathwayID, type=temp[0], id=temp[1];	
			int did=pbubble->dataID;
			vector<QString> name; //pbubble->if(_scene->_proteinNameD[Didi][itemp[1]]==_scene->_proteinNameD[pbubble->dataID][jtemp[1]])
			vector<set<int> > shared;
			switch(type)
			{
				case 'P': name=_scene->_proteinNameD[did][id];         break;
				case 'S': name=_scene->_smallMoleculeNameD[did][id];     break; 
				case 'C': name=_scene->_complexNameD[did][id];         break;
				case 'D': name=_scene->_DnaNameD[did][id];		 break;
				case 'R': name=_scene->_reactionNameD[did][id];		break;
				case 'E': name=_scene->_physicalEntityNameD[did][id];	break;					
				case 'L': name=_scene->_ANodeName[pid][id];	break;		
				case 'M': name=_scene->_compartmentName[pid][id];	 break;	
				case 'W': name.push_back(scene->_pathBubbles[pid]->_curFileName);   break;	
			}
			if(name.size()<5)
				name.resize(5);
			for(int k=0; k< mList.size(); k++) 
			{
				if(k==i)
					continue;
				PathBubble1* pbubble1 = mList[k];	
				switch(type)
				{
					case 'P': shared=pbubble1->sharedProtein;          break;
					case 'S': shared=pbubble1->sharedSmallMolecule;    break; 
					case 'C': shared=pbubble1->sharedComplex;             break;
					case 'D': shared=pbubble1->sharedDna;                break;
					case 'R': shared=pbubble1->sharedReaction;      break;
					case 'E': shared=pbubble1->sharedPhysicalEntity;   break;					
					case 'L': shared=pbubble1->sharedANode;   break;					
					case 'M': shared=pbubble1->sharedCompartment;    break;	
					case 'W': shared=pbubble1->sharedPathway;     break;	
				}
				for(int j=0; j<shared.size(); j++)
				{
					if(_scene->_pathBubbles[j]==NULL || !_scene->_pathBubbles[j]->isVisible())
						continue;
					int pid1=j, did1=_scene->_pathBubbles[j]->dataID;			
					for(set<int>::iterator it=shared[j].begin(); it!=shared[j].end(); it++)
					{
						int id1=*it;
						vector<QString> name1;				
						switch(type)
						{
							case 'P': name1=_scene->_proteinNameD[did1][id1];             break;
							case 'S': name1=_scene->_smallMoleculeNameD[did1][id1];   break; 
							case 'C': name1=_scene->_complexNameD[did1][id1];          break;
							case 'D': name1=_scene->_DnaNameD[did1][id1];              break;
							case 'R': name1=_scene->_reactionNameD[did1][id1];         break;
							case 'E': name1=_scene->_physicalEntityNameD[did1][id1];   break;	
							case 'L': name1=_scene->_ANodeName[pid1][id1];             break;	
							case 'M': name1=_scene->_compartmentName[pid1][id1];      break;								
							case 'W': name1.push_back(scene->_pathBubbles[pid1]->_curFileName);       break;	
						}	
						if(name1.size()<5) name1.resize(5);
						if(type=='R' || type == 'M' || type == 'W')
						{
							if( name==name1)
							{
								count++;
						    }
						}
						else if(type=='L' || type=='R')
						{
						    if( (completeSame && name==name1) || (!completeSame && name[1]==name1[1]) )
							{
								count++;
						    }						
						}
						else
						{
							if( (completeSame && name==name1) || (!completeSame && name[0]==name1[0]) )
							{
								count++;
						    }
						}
					}
				}
			}
			if(count==0) //not really shared, remove
			{
				switch(type)
				{
					case 'P': pbubble->sharedProtein[pid].erase(id);        break;
					case 'S': pbubble->sharedSmallMolecule[pid].erase(id);  break; 
					case 'C': pbubble->sharedComplex[pid].erase(id);        break;
					case 'D': pbubble->sharedDna[pid].erase(id);            break;
					case 'R': pbubble->sharedReaction[pid].erase(id);       break;
					case 'E': pbubble->sharedPhysicalEntity[pid].erase(id); break;					
					case 'L': pbubble->sharedANode[pid].erase(id); break;					
					case 'M': pbubble->sharedCompartment[pid].erase(id);    break;	
					case 'W': pbubble->sharedPathway[pid].erase(id);        break;	
				}			    
			}
		}			
	}
}




vector<set<vector<int>>> PathBubble1::getHighlightedBackuped(void)
{ //toHighLight: false is only for expression search
	//if nothing is highlighted
	//highlight pathway
	if(headPathway==NULL)
        headPathway = _scene->findASmallestPathinGroup(this);

	if(headPathway!=NULL)
	if(headPathway->highlightedStored.empty())
	{
		vector < set < vector <int> > > temp;
		headPathway->linkedStoredinHead.push_back(temp);
		headPathway->currentLinkedinHeadit = headPathway->linkedStoredinHead.begin();
	}

	vector<PathBubble1 *> mList=_scene->extractPathBubblesinGroup(this);
	vector<set<vector<int>>> HBackup;
	
	for(int i=0; i< mList.size(); i++) 
	{
		PathBubble1* pbubble = mList[i];		
		set<vector<int>> hBackup=pbubble->highlighted;
		/*if(pbubble->highlightedStored.empty())
		{
			pbubble->highlightedStored.push_back(pbubble->highlighted);
			pbubble->currentHighlightedit = pbubble->highlightedStored.begin();
		    
			set < vector <int> > temp;
			pbubble->linkedStored.push_back(temp);
			pbubble->currentLinkedit = pbubble->linkedStored.begin();		    

			pbubble->differedStored.push_back(temp);
			pbubble->currentDifferedit = pbubble->differedStored.begin();	

			pbubble->expressedStored.push_back(temp);
			pbubble->currentExpressedit = pbubble->expressedStored.begin();	

			pbubble->sharedStored.push_back(temp);
			pbubble->currentSharedit = pbubble->sharedStored.begin();	

			vector<int> eitem;
			eitem.push_back(m_pathwayID); eitem.push_back(eventType);
			eventStored.push_back(eitem);
			currentEventit = eventStored.begin();	
		}*/
		HBackup.push_back(hBackup);
	}   
	return HBackup;	
}



void PathBubble1::highlightedNodetoStored()
{	
	//deque<set<vector<int> > >::iterator currentHighlightedit;
	//deque<set<vector<int> > > highlightedStored;//4 step recall//record 5 steps
	sharedDifferedLinkedExpressedHighLightedSizeCheck();

	vector<PathBubble1 *> mList=_scene->extractPathBubblesinGroup(this);

	for(int j=0; j<mList.size(); j++) 
	{
		PathBubble1* pBubble = mList[j];		 
		set < vector <int> >  shared=pBubble->highlighted;	
			
		if(pBubble->highlightedStored.size()>=99)
			pBubble->highlightedStored.pop_front();
		
		pBubble->highlightedStored.push_back(shared);

		pBubble->currentHighlightedit = pBubble->highlightedStored.end();

		pBubble->currentHighlightedit--;					
	}
}


void PathBubble1::searchedNodetoStored(int eventType)
{	//work for the whole group
	vector<PathBubble1 *> mList=_scene->extractPathBubblesinGroup(this);	

	
	if(eventType==SEARCHSHARED) 	//shared
	{
		for(int j=0; j<mList.size(); j++) 
		{
			PathBubble1* pBubble = mList[j];		 
			pBubble->storeShared();
	    }
	}
	if(eventType==SEARCHDIFFERED) 	//differed
	{
		for(int j=0; j<mList.size(); j++) 
		{
			PathBubble1* pBubble = mList[j];		 	 
			pBubble->storeDiffered();
		}
	}
	if(eventType==SEARCHEXPRESSED) 	//
	{
		for(int j=0; j<mList.size(); j++) 
		{
			PathBubble1* pBubble = mList[j];		 	 
			pBubble->storeExpressed();
		}
	}	
	if(eventType==SEARCHLINKED) 	//linked
	{
		//vector <set < vector <int> > > linkedinHead;
		//linkedinHead.resize( _scene->_pathBubbles.size() );// [m_pathwayID]
		for(int j=0; j<mList.size(); j++) 
		{		
			PathBubble1* pBubble = mList[j];		 	 
		    pBubble->storeLinked();
		}		
	}	
	

	for(int j=0; j<mList.size(); j++) 
	{
		 PathBubble1* pBubble = mList[j];
		
	}
}

vector<QString> PathBubble1::getExpressedGeneNames()
{
	//also set the color for shared
	vector < set < vector <int> > > expressedSets;
	vector<PathBubble1 *> plist = _scene->extractPathBubblesinGroup(this);
	vector<expressionBubble *> elist = _scene->extractExpressionBubblesinGroup(this); 
	set<QString> geneNameSet;
	vector<QString> geneNameVector;
	vector<int> item(3,0);

	for(int i=0; i<elist.size(); i++) 	
	{
		expressionBubble * ebubble=elist[i];	
		for(int pid=0; pid < ebubble->expressedGene.size(); pid++)
		{
			for(set<int>::iterator it = ebubble->expressedGene[pid].begin(); it!=ebubble->expressedGene[pid].end(); it++)
			{
				int id=*it;
			    geneNameSet.insert(ebubble->geneInfo[id][1]);
			}
		}
	}
	for(set<QString>::iterator it = geneNameSet.begin(); it!=geneNameSet.end(); it++)
	{
		geneNameVector.push_back(*it);
	}
	return geneNameVector;
}

vector < set < vector <int> > > PathBubble1::assignExpressedSets()
{
	//have bugs
    //also set the color for shared
	vector < set < vector <int> > > expressedSets;
	return expressedSets;

	vector<PathBubble1 *> plist = _scene->extractPathBubblesinGroup(this);
	vector<expressionBubble *> elist = _scene->extractExpressionBubblesinGroup(this); 
	
	vector<QString> geneNameVector;
	vector<int> item(3,0);

	geneNameVector=getExpressedGeneNames();
	expressedSets.resize(geneNameVector.size());
	for(int i=0; i<plist.size(); i++) 	
	{
		PathBubble1* pbubble=plist[i];	
		int Did=pbubble->dataID, pid=pbubble->m_pathwayID;
		for(int eid=0; eid<pbubble->expressedProtein.size(); eid++)
		{
			for(set<int>::iterator it = pbubble->expressedProtein[eid].begin(); it!=pbubble->expressedProtein[eid].end(); it++)
			{
			    int id=*it;
			    for(int j=0; j < geneNameVector.size(); j++)
				{
				   if(_scene->_proteinNameD[Did][id][0].contains(geneNameVector[j]))
                   {
					   item[0]=pbubble->m_pathwayID;
					   item[1]='P';
					   item[2]=id;
					   expressedSets[j].insert(item);
				   }
				}
			}
			for(set<int>::iterator it = pbubble->expressedComplex[eid].begin(); it!=pbubble->expressedComplex[eid].end(); it++)
			{
			    int id=*it;
			    for(int j=0; j <geneNameVector.size(); j++)
				{
					//for(int j=0; j<_scene->_complexContain[m_bindPathwayID].size(); j++)		
					for(int k=0; k<_scene->_complexContain[pid][id].size(); k=k+2)
					{
						bool flag=0;
						//for(int i=0; i< geneInfo.size(); i++)
						{
							 int type=_scene->_complexContain[pid][id][k], cid=_scene->_complexContain[pid][id][k+1];
							 vector<QString> name;	 	
							 switch(type)
							 {
								 case 'C': name = _scene->_complexNameD[pid][cid];        break;
								 case 'D': name = _scene->_DnaNameD[pid][cid];            break;
								 case 'E': name = _scene->_physicalEntityNameD[pid][cid]; break; 
								 case 'P': name = _scene->_proteinNameD[pid][cid];        break;
								 case 'S': name = _scene->_smallMoleculeNameD[pid][cid];  break;			
							 }							 
							 if(name[0].contains(geneNameVector[j])||(name.size()>=2&&name[1].contains(geneNameVector[j])))
							 {
								item[0]=pid;
								item[1]='C';
								item[2]=id;
								expressedSets[j].insert(item);
							 }
						}
					}
				}
			}
		}
	}		
	return expressedSets;
}


vector < set < vector <int> > > PathBubble1::assignSharedSets()
{
	//also set the color for shared
	vector < set < vector <int> > > sharedSets;
	vector<PathBubble1 *> mlist = _scene->extractPathBubblesinGroup(this);
	for(int i=0; i<mlist.size(); i++) 	
	{
		PathBubble1* pbubble=mlist[i];				
		//for(deque< set < vector <int> > >::iterator it=pbubble->sharedStored.begin(); it!=pbubble->sharedStored.end(); it++)
		if(pbubble->currentSharedit != pbubble->sharedStored.end())
		{	
			set < vector <int> > tSet = *pbubble->currentSharedit;
			for(set < vector <int> > ::iterator its = tSet.begin(); its!=tSet.end(); its++)
			{
				set<vector<int>> items;				
				items.insert(*its);				
				sharedSets.push_back(items);
			}
		}		
	}	
	//each set contains the shared proteins in different section
	for(int i=0; i<sharedSets.size(); i++ )
	for(int j=i+1; j<sharedSets.size(); j++ )
	{		
		if(sharedSets[j].size()>0 && sharedSets[i].size()>0)
		{
			vector<int> node1=*sharedSets[i].begin();
			vector<int> node2=*sharedSets[j].begin();
			if(isSameNode(node1[0], node1[1], node1[2], node2[0], node2[1], node2[2]))
			{
				sharedSets[i].insert(node2);
				sharedSets[j].erase(node2);
			}	
		}
	}
	int count=0;
	for(int i=sharedSets.size()-1; i>=0; i--)
	{
	   if(!sharedSets[i].empty())
		   break;
	   count++;
	}
	sharedSets.resize(sharedSets.size()-count);	
	return sharedSets;
}

QString PathBubble1::pathWayGraphForward()
{
	QString fileName;
	vector<PathBubble1 *> mlist=_scene->extractPathBubblesinGroup(this);
	for(int i=0; i<mlist.size(); i++) 
	{
	 	 //if( mlist[i]->getType() != SUBPATHBUBBLE1 && mlist[i]->getType() != PATHBUBBLE1)
		 //	 continue;
	 	 //PathBubble1* pbubble = dynamic_cast<PathBubble1*>(mlist[i]);	
		 PathBubble1* pbubble=mlist[i];
		  if(m_pathwayID!=pbubble->m_pathwayID)
			 continue;
		 if(pbubble->currentPathwayGraphit!=pbubble->pathwayGraphStored.end())
		 {
			 pbubble->currentPathwayGraphit++;
			 if(pbubble->currentPathwayGraphit!=pbubble->pathwayGraphStored.end())
			     fileName=*pbubble->currentPathwayGraphit;
		 }	
			
	}
	return fileName;
}



void PathBubble1::clearEvents()
{
	eventStored.clear();
	currentEventit=eventStored.end();
	
	sharedStored.clear(), linkedStored.clear(), differedStored.clear(), expressedStored.clear(), highlightedStored.clear();	

	currentHighlightedit = highlightedStored.end(), currentSharedit = sharedStored.end(), currentColoredit = coloredStored.end(), currentLinkedit=linkedStored.end();
	currentDifferedit = differedStored.end(), currentExpressedit=expressedStored.end();

}

QString PathBubble1::pathWayGraphBackward()
{
	QString fileName;
	QList<ItemBase *> mlist=_scene->getGroupMembers(this);	 
	for(int i=0; i<mlist.size(); i++) 
	{
	 	 if ( mlist[i]->getType() != SUBPATHBUBBLE1 && mlist[i]->getType() != PATHBUBBLE1)
		 	  continue;
	 	 PathBubble1* pbubble = dynamic_cast<PathBubble1*>(mlist[i]);				
		 if(m_pathwayID!=pbubble->m_pathwayID)
			 continue;
		 if(!pbubble->pathwayGraphStored.empty() && pbubble->currentPathwayGraphit!=pbubble->pathwayGraphStored.end())
		 {
			if(pbubble->currentPathwayGraphit!=pbubble->pathwayGraphStored.begin())
			{
				pbubble->currentPathwayGraphit--;
				fileName=*pbubble->currentPathwayGraphit;
			}			 
			else if(pbubble->currentPathwayGraphit==pbubble->pathwayGraphStored.begin())
			{
				
			}
		 }
	}
	return fileName;
}

bool PathBubble1::storeHighlighted()
{	 	 
	if(highlightedStored.empty())
	{
		set < vector <int> > empty;	
		highlightedStored.push_back(empty);
		highlightedStored.push_back(highlighted);
		
	}
	else
	{
		highlightedStored.push_back(highlighted);
		
	}	
	currentHighlightedit=highlightedStored.end();
	currentHighlightedit--;

	return (!highlighted.empty());
}


bool PathBubble1::storeDiffered()
{	//return if there are differed items found
	bool flag=false;
	if(differedStored.empty())
	{		
		set < vector <int> > empty;	
		differedStored.push_back(empty);
		
	}	
    //differed		 
	set < vector <int> >  differed;	
	for(int m=0; m<9; m++)
	{
		int ctype;
		vector<set<int>> differedNode;
		switch(m)
		{
			case 0: ctype='P';  differedNode=differedProtein;        break;
			case 1: ctype='S';  differedNode=differedSmallMolecule;  break;
			case 2: ctype='C';  differedNode=differedComplex;        break;
			case 3: ctype='D';  differedNode=differedDna;            break;
			case 4: ctype='R';  differedNode=differedReaction;       break;
			case 5: ctype='E';  differedNode=differedPhysicalEntity; break;	
			case 6: ctype='L';  differedNode=differedANode; break;	
			case 7: ctype='M';  differedNode=differedCompartment;    break;	
			case 8: ctype='W';  differedNode=differedPathway;        break;	
		}	
		if(differedNode.size()>m_pathwayID)
		for(set<int>::iterator it=differedNode[m_pathwayID].begin(); it!=differedNode[m_pathwayID].end(); it++)
		{
			int id=*it;
			vector<int> temp(3,0);
			temp[0]=m_pathwayID, temp[1]=ctype, temp[2]=id;				
			differed.insert(temp);
		}
		if(!differedNode.empty())
			flag=true;
	}		
	differedStored.push_back(differed);			   
	
	currentDifferedit=differedStored.end();
	currentDifferedit--;

	return (flag);
}

bool PathBubble1::storeColored()
{	//return if there are colored items found
	if(coloredStored.empty())
	{
		set < vector <int> > empty;	
		coloredStored.push_back(empty);	
	}		
	coloredStored.push_back(_itemColored);	
	currentColoredit=coloredStored.end();
	currentColoredit--;	
	return (!_itemColored.empty());
}

bool PathBubble1::storeShared()
{	//return if there are shared item found
	bool flag=false;
	if(sharedStored.empty())
	{
		set < vector <int> > empty;	
		sharedStored.push_back(empty);	
		
	}	
    //shared
    set < vector <int> >  shared;	
	for(int m=0; m<9; m++)
	{
		int ctype;
		vector<set<int>> sharedNode;
		switch(m)
		{
			case 0: ctype='P';  sharedNode=sharedProtein;        break;
			case 1: ctype='S';  sharedNode=sharedSmallMolecule;  break;
			case 2: ctype='C';  sharedNode=sharedComplex;        break;
			case 3: ctype='D';  sharedNode=sharedDna;            break;
			case 4: ctype='R';  sharedNode=sharedReaction;       break;
			case 5: ctype='E';  sharedNode=sharedPhysicalEntity; break;	
			case 6: ctype='L';  sharedNode=sharedANode; break;	
			case 7: ctype='M';  sharedNode=sharedCompartment;    break;	
			case 8: ctype='W';  sharedNode=sharedPathway;        break;	
		}	
		if(sharedNode.size()>m_pathwayID)
		for(set<int>::iterator it=sharedNode[m_pathwayID].begin(); it!=sharedNode[m_pathwayID].end(); it++)
		{
			int id=*it;
			vector<int> temp(3,0);
			temp[0]=m_pathwayID, temp[1]=ctype, temp[2]=id;				
			shared.insert(temp);
		}
		if(!sharedNode.empty())
			flag=true;
	}	

	sharedStored.push_back(shared);	

	currentSharedit=sharedStored.end();
	currentSharedit--;
	return (flag);
}


bool PathBubble1::storeExpressed()
{	//return if there are expressed found
	bool flag=false;
	if(expressedStored.empty())
	{
		set < vector <int> > empty;	
		expressedStored.push_back(empty);			
	}	
    //expressed
	set < vector <int> >  expressed;	
	for(int m=0; m<9; m++)
	{
		int ctype;
		vector<set<int>> expressedNode;
		switch(m)
		{
			case 0: ctype='P';  expressedNode=expressedProtein;        break;
			case 1: ctype='S';  expressedNode=expressedSmallMolecule;  break;
			case 2: ctype='C';  expressedNode=expressedComplex;        break;
			case 3: ctype='D';  expressedNode=expressedDna;            break;
			case 4: ctype='R';  expressedNode=expressedReaction;       break;
			case 5: ctype='E';  expressedNode=expressedPhysicalEntity; break;	
			case 6: ctype='L';  expressedNode=expressedANode; break;	
			case 7: ctype='M';  expressedNode=expressedCompartment;    break;	
			case 8: ctype='W';  expressedNode=expressedPathway;        break;	
		}	
		if(expressedNode.size()>m_pathwayID)
		for(set<int>::iterator it=expressedNode[m_pathwayID].begin(); it!=expressedNode[m_pathwayID].end(); it++)
		{
			int id=*it;
			vector<int> temp(3,0);
			temp[0]=m_pathwayID, temp[1]=ctype, temp[2]=id;				
			expressed.insert(temp);
		}
		if(!expressedNode.empty())
			flag=true;
     }
	 expressedStored.push_back(expressed);
    
	 currentExpressedit=expressedStored.end();
	 currentExpressedit--;
	 return (flag);
}

int PathBubble1::findLinkedLevel(int pid, int type, int id)
{	//return if there are linked found	
	

	PathBubble1* pbubble = _scene->_pathBubbles[pid];		 								 		   		 
	if(pbubble->linkedStored.empty())
		return 0;

	deque< vector < set < vector <int> > > >::iterator it=currentLinkedit;	
	
	vector<int> item, item1;
	item.push_back(pid); item.push_back(type);  item.push_back(id);
	item1=item; item1.push_back(1);

    vector <set < vector <int> > > linked=*it;	
	int k;
	for(k=0; k<linked.size();k++)
	{
        if(linked[k].find(item)!=linked[k].end())		
		{
			if(linked[k].find(item1)==linked[k].end()) //if founded, it means it is the head of the link
				k=k+1;
			break;
		}
	}
	return k;    
	
}

int PathBubble1::findLinkedLevel(int pid1, int type1, int id1, int pid2, int type2, int id2) //got edge
{
	PathBubble1* pbubble = _scene->_pathBubbles[pid1];		 								 		   		 
	if(pbubble->linkedStored.empty())
		return 0;

	deque< vector < set < vector <int> > > >::iterator it=currentLinkedit;	
	
	vector<int> item;
	item.push_back('A');
	item.push_back(pid1); item.push_back(type1);  item.push_back(id1);
	item.push_back(pid2); item.push_back(type2);  item.push_back(id2);

    vector <set < vector <int> > > linked;
	if(it!=linkedStored.end())
		linked=*it;

	int k;
	for(k=0; k<linked.size();k++)
	{
		if(linked[k].find(item)!=linked[k].end())		
		{
			break;
		}
	}
	return k;
	

	
}

int PathBubble1::getTotalLinkLevel()
{
	int count=0;

	if(!linkedStored.empty())
	{
	    vector<set<vector<int>>> linked = *currentLinkedit;
		count=linked.size();
	}	
	return count+1;
}

bool PathBubble1::storeLinked()
{	
	bool flag=false;
	if(linkedStored.empty())
	{
		vector < set < vector <int> > > empty;	
		linkedStored.push_back(empty);	
		currentLinkedit=linkedStored.begin();
	}	
	
	int totalLevel=getTotalLinkLevel();
	int level;
		

    //linked		 
	vector < set < vector <int> > > linked;	
	linked.resize(totalLevel+1);
	for(int m=0; m<9; m++)
	{
		int ctype;
		vector<set<int>> linkedNode;
		switch(m)
		{
			case 0: ctype='P';  linkedNode=linkedProtein;        break;
			case 1: ctype='S';  linkedNode=linkedSmallMolecule;  break;
			case 2: ctype='C';  linkedNode=linkedComplex;        break;
			case 3: ctype='D';  linkedNode=linkedDna;            break;
			case 4: ctype='R';  linkedNode=linkedReaction;       break;
			case 5: ctype='E';  linkedNode=linkedPhysicalEntity; break;	
			case 6: ctype='L';  linkedNode=linkedANode; break;	
			case 7: ctype='M';  linkedNode=linkedCompartment;    break;	
			case 8: ctype='W';  linkedNode=linkedPathway;        break;	
		}	
		if(linkedNode.size()>m_pathwayID)
		{
			for(set<int>::iterator it=linkedNode[m_pathwayID].begin(); it!=linkedNode[m_pathwayID].end(); it++)
			{
				int id=*it;
				vector<int> temp(3,0);
				vector<int> temp1(2,0);
				temp1[0]=ctype; temp1[1]=id;
				temp[0]=m_pathwayID, temp[1]=ctype; temp[2]=id; level=findLinkedLevel(m_pathwayID, ctype, id);			
				linked[level].insert(temp);
				if(highlighted.find(temp1)!=highlighted.end()) //use them to mark who is first pressed, need this information for highLighted the head of a search
				{
				     temp.push_back(1);    
					 linked[level].insert(temp);
				}				
				
			}			    				
		}
		if(!linkedNode.empty())
			flag=true;
	}
		
	if(linkedEdge.size()>m_pathwayID)
	for(set<vector<vector<int>>>::iterator it=linkedEdge[m_pathwayID].begin(); it!=linkedEdge[m_pathwayID].end(); it++)
	{
			vector<vector<int>> temp=*it;				   
			vector<int> edge(7,0);
			edge[0]='A'; //edge
			edge[1]=temp[0][0]; edge[2]=temp[0][1]; edge[3]=temp[0][2]; 
			edge[4]=temp[1][0]; edge[5]=temp[1][1]; edge[6]=temp[1][2];
			//edge[7]=level;
			level=findLinkedLevel(edge[1], edge[2], edge[3], edge[4], edge[5], edge[6]);
			linked[level].insert(edge);
	}
	deque< vector < set < vector <int> > > >::iterator it=linkedStored.end();
	it--;
	while(it!=currentLinkedit)
	{
	     linkedStored.pop_back();
		 it=linkedStored.end();
		 it--;
	}
	linkedStored.push_back(linked);	
   	currentLinkedit=linkedStored.end();
	currentLinkedit--;

	highlighted.clear();
	return (flag);
}
	

void PathBubble1::undoHighlighted()
{	 	 
	if(currentHighlightedit!=highlightedStored.end())
	{
		if(currentHighlightedit!=highlightedStored.begin())
		{
			currentHighlightedit--;
			{
				cleanBeforeRedoUndo(HIGHLIGHTED);
				highlighted=*currentHighlightedit;
			}
		}			 
		else if(currentHighlightedit==highlightedStored.begin())
		{
			cleanBeforeRedoUndo(HIGHLIGHTED);
			//highlighted.clear();
		}
	}	
}

void PathBubble1::undoColored()
{	
	// express //Colored
	if(currentColoredit!=coloredStored.end())
	{
		set < vector <int> > Colored;		
		if(currentColoredit!=coloredStored.begin())
		{
			currentColoredit--;
			cleanBeforeRedoUndo(COLORED);
			_itemColored=*currentColoredit;
			
		}	
	}
}

void PathBubble1::undoShared()
{	
	// express //shared
	if(currentSharedit!=sharedStored.end())
	{
		set < vector <int> > shared;		
		if(currentSharedit!=sharedStored.begin())
		{
			currentSharedit--;
			cleanBeforeRedoUndo(SEARCHSHARED);
			shared=*currentSharedit;
			for(set<vector<int>>::iterator it=shared.begin(); it!=shared.end(); it++)
			{
				vector<int> temp=*it;
				int pid=temp[0], type=temp[1], id=temp[2];				
				switch(type)
				{
					case 'P': sharedProtein[pid].insert(id);          break;
					case 'S': sharedSmallMolecule[pid].insert(id);    break; 
					case 'C': sharedComplex[pid].insert(id);          break;
					case 'D': sharedDna[pid].insert(id);              break;
					case 'R': sharedReaction[pid].insert(id);         break;
					case 'E': sharedPhysicalEntity[pid].insert(id);   break;					
					case 'L': sharedANode[pid].insert(id);   break;					
					case 'M': sharedCompartment[pid].insert(id);      break;	
					case 'W': sharedPathway[pid].insert(id);          break;	
				}	
			}	
		}	
	}
}

void PathBubble1::undoDiffered()
{	
	// //differ
	if(currentDifferedit!=differedStored.end())
	{
		set < vector <int> > differed;
		
		if(currentDifferedit!=differedStored.begin())
		{
			currentDifferedit--;
			cleanBeforeRedoUndo(SEARCHDIFFERED);
			differed=*currentDifferedit;
			for(set<vector<int>>::iterator it=differed.begin(); it!=differed.end(); it++)
			{
				vector<int> temp=*it;
				int pid=temp[0], type=temp[1], id=temp[2];				
				switch(type)
				{
					case 'P': differedProtein[pid].insert(id);          break;
					case 'S': differedSmallMolecule[pid].insert(id);    break; 
					case 'C': differedComplex[pid].insert(id);          break;
					case 'D': differedDna[pid].insert(id);              break;
					case 'R': differedReaction[pid].insert(id);         break;
					case 'E': differedPhysicalEntity[pid].insert(id);   break;					
					case 'L': differedANode[pid].insert(id);   break;	
					case 'M': differedCompartment[pid].insert(id);      break;	
					case 'W': differedPathway[pid].insert(id);          break;	
				}	
			}			
		}
	}
}

void PathBubble1::undoLinked()
{	
	if(currentLinkedit!=linkedStored.end())
	{
		vector <set < vector <int> > > linked;
		
		if(currentLinkedit!=linkedStored.begin())
		{
			currentLinkedit--;
			cleanBeforeRedoUndo(SEARCHLINKED);
			linked=*currentLinkedit;
			for(int i=0; i<linked.size(); i++)
			for(set<vector<int>>::iterator it=linked[i].begin(); it!=linked[i].end(); it++)
			{
				vector<int> temp=*it,temp0(3,0),temp1(3,0);
				int pid, type, id;
				if(temp.size()>4)
				{
					type=temp[0]; pid=temp[1];
				}
				else
				{
					pid=temp[0], type=temp[1], id=temp[2];				
				}
				vector<vector<int>> edge;				
				switch(type)
				{
					case 'P': linkedProtein[pid].insert(id);          break;
					case 'S': linkedSmallMolecule[pid].insert(id);    break; 
					case 'C': linkedComplex[pid].insert(id);          break;
					case 'D': linkedDna[pid].insert(id);              break;
					case 'R': linkedReaction[pid].insert(id);         break;
					case 'E': linkedPhysicalEntity[pid].insert(id);   break;					
					case 'M': linkedCompartment[pid].insert(id);      break;	
					case 'L': linkedANode[pid].insert(id);      break;
					case 'W': linkedPathway[pid].insert(id);          break;	
					case 'A': 	
							temp1[0]=temp[4], temp1[1]=temp[5], temp1[2]=temp[6];
							temp0[0]=temp[1], temp0[1]=temp[2], temp0[2]=temp[3];
							edge.push_back(temp0);
							edge.push_back(temp1);
							linkedEdge[pid].insert(edge);
							break;	
				}	
			}	
		}			 
	}
}


void PathBubble1::undoExpressed()
{		  // //express
	if(currentExpressedit!=expressedStored.end())
	{
		set < vector <int> > expressed;		
		if(currentExpressedit!=expressedStored.begin())
		{
			currentExpressedit--;
			cleanBeforeRedoUndo(SEARCHEXPRESSED);
			expressed=*currentExpressedit;
			for(set<vector<int>>::iterator it=expressed.begin(); it!=expressed.end(); it++)
			{
				vector<int> temp=*it;
				int pid=temp[0], type=temp[1], id=temp[2];				
				switch(type)
				{
					case 'P': expressedProtein[pid].insert(id);          break;
					case 'S': expressedSmallMolecule[pid].insert(id);    break; 
					case 'C': expressedComplex[pid].insert(id);          break;
					case 'D': expressedDna[pid].insert(id);              break;
					case 'R': expressedReaction[pid].insert(id);         break;
					case 'E': expressedPhysicalEntity[pid].insert(id);   break;					
					case 'L': expressedANode[pid].insert(id);            break;	
					case 'M': expressedCompartment[pid].insert(id);      break;	
					case 'W': expressedPathway[pid].insert(id);          break;	
				}	
			}		
		}		
	}
}
		



void PathBubble1::redoHighlighted()
{		 
	if(highlightedStored.empty())
		return;
	if(currentHighlightedit!=highlightedStored.end())
	{
		//highlighted=*currentHighlightedit;
		deque< set < vector <int> > >::iterator end=currentHighlightedit;
		end++;
		if(end!=highlightedStored.end())
		{
			cleanBeforeRedoUndo(HIGHLIGHTED);
			highlighted=*currentHighlightedit;
			currentHighlightedit++;
		}
	}		
}

void PathBubble1::redoGraph()
{
	QString fileName = pathWayGraphForward();					
	if(fileName.size()>0)
	{
		fileName=fileName+".xml";
		readPathWay(fileName, complexPos, proteinPos, smallMoleculePos, DnaPos, reactionPos, physicalEntityPos, ANodePos, edge);		
		getGraphToBePaint();
		//itemGrid=recordItemGrid(visibleItemNum, visibleCompartmentNum);
	}
}

void PathBubble1::redoColored()
{
	//share	
	if(coloredStored.empty())
		return;
	if(currentColoredit!=coloredStored.end())
	{
		set < vector <int> > Colored;
		deque< set < vector <int> > >::iterator end=currentColoredit;
		end++;
		if(end!=coloredStored.end())
		{
			currentColoredit++;
			if(currentColoredit!=coloredStored.end())
			{
				cleanBeforeRedoUndo(COLORED);
				//Colored=*currentColoredit;				
				_itemColored=*currentColoredit;	
			}
		}			 			
		/*for(set<vector<int>>::iterator it=Colored.begin(); it!=Colored.end(); it++)
		{
			vector<int> temp=*it;
			int pid, type, id;
			pid=temp[0], type=temp[1], id=temp[2];	
			_itemColored.insert(temp);			
		}	
		*/		
	}	
}

void PathBubble1::undoGraph()
{
	QString fileName = pathWayGraphBackward();				
	if(fileName.size()>0)
	{
		fileName=fileName+".xml";
		readPathWay(fileName, complexPos, proteinPos, smallMoleculePos, DnaPos, reactionPos, physicalEntityPos, ANodePos, edge);		
		getGraphToBePaint();
		//itemGrid=recordItemGrid(visibleItemNum, visibleCompartmentNum);
	}
}


void PathBubble1::redoShared()
{
	//share	
	if(sharedStored.empty())
		return;
	if(currentSharedit!=sharedStored.end())
	{
		set < vector <int> > shared;
		deque< set < vector <int> > >::iterator end=currentSharedit;
		end++;
		if(end!=sharedStored.end())
		{
			currentSharedit++;
			if(currentSharedit!=sharedStored.end())
			{
				cleanBeforeRedoUndo(SEARCHSHARED);
				shared=*currentSharedit;				
			}
		}			 			
		for(set<vector<int>>::iterator it=shared.begin(); it!=shared.end(); it++)
		{
			vector<int> temp=*it;
			int pid, type, id;
			pid=temp[0], type=temp[1], id=temp[2];				
			switch(type)
			{
				case 'P': sharedProtein[pid].insert(id);          break;
				case 'S': sharedSmallMolecule[pid].insert(id);    break; 
				case 'C': sharedComplex[pid].insert(id);          break;
				case 'D': sharedDna[pid].insert(id);              break;
				case 'R': sharedReaction[pid].insert(id);         break;
				case 'E': sharedPhysicalEntity[pid].insert(id);   break;					
				case 'L': sharedANode[pid].insert(id);            break;					
				case 'M': sharedCompartment[pid].insert(id);      break;	
				case 'W': sharedPathway[pid].insert(id);          break;					
			}	
		}			
	}	
}

void PathBubble1::redoExpressed()
{
	if(expressedStored.empty())
		return;
    if(currentExpressedit!=expressedStored.end())
	{
		set < vector <int> > expressed;
		deque< set < vector <int> > >::iterator end = currentExpressedit;
		end++;
		if(end!=expressedStored.end())
		{
			currentExpressedit++;
			if(currentExpressedit!=expressedStored.end())
			 {
				 cleanBeforeRedoUndo(SEARCHEXPRESSED);
				 expressed=*currentExpressedit;				
			}
		}			 			
		for(set<vector<int>>::iterator it=expressed.begin(); it!=expressed.end(); it++)
		{
			vector<int> temp=*it;
			int pid=temp[0], type=temp[1], id=temp[2];				
			switch(type)
			{
				case 'P': expressedProtein[pid].insert(id);          break;
				case 'S': expressedSmallMolecule[pid].insert(id);    break; 
				case 'C': expressedComplex[pid].insert(id);          break;
				case 'D': expressedDna[pid].insert(id);              break;
				case 'R': expressedReaction[pid].insert(id);         break;
				case 'E': expressedPhysicalEntity[pid].insert(id);   break;					
				case 'L': expressedANode[pid].insert(id);   break;					
				case 'M': expressedCompartment[pid].insert(id);      break;	
				case 'W': expressedPathway[pid].insert(id);          break;	
			}	
		}			
	}
}

void PathBubble1::redoDiffered()
{         
	//differ
	if(differedStored.empty())
		return;
	if(currentDifferedit!=differedStored.end())
	{
		set < vector <int> > differed;
		deque< set < vector <int> > >::iterator end=currentDifferedit;
		end++;
		if(end!=differedStored.end())
		{
			currentDifferedit++;
			if(currentDifferedit!=differedStored.end())
			{
				cleanBeforeRedoUndo(SEARCHDIFFERED);
				differed=*currentDifferedit;				
			}
		}			 			
		for(set<vector<int>>::iterator it=differed.begin(); it!=differed.end(); it++)
		{
			vector<int> temp=*it;
			int pid=temp[0], type=temp[1], id=temp[2];				
			switch(type)
			{
				case 'P': differedProtein[pid].insert(id);          break;
				case 'S': differedSmallMolecule[pid].insert(id);    break; 
				case 'C': differedComplex[pid].insert(id);          break;
				case 'D': differedDna[pid].insert(id);              break;
				case 'R': differedReaction[pid].insert(id);         break;
				case 'E': differedPhysicalEntity[pid].insert(id);   break;					
				case 'L': differedANode[pid].insert(id);   break;					
				case 'M': differedCompartment[pid].insert(id);      break;	
				case 'W': differedPathway[pid].insert(id);          break;	
			}	
		}	
	}
}


void PathBubble1::redoLinked()
{
    //linked
	if(linkedStored.empty())
		return;
	if(currentLinkedit!=linkedStored.end())
	{
		vector < set < vector <int> > > linked;
		deque< vector < set < vector <int> > > >::iterator end=currentLinkedit;
		end++;
		if(end!=linkedStored.end())
		{
			currentLinkedit++;
			if(currentLinkedit!=linkedStored.end())
			 {
				 cleanBeforeRedoUndo(SEARCHLINKED);
				 linked=*currentLinkedit;				
			}		
			for(int i=0; i<linked.size(); i++)
			for(set<vector<int>>::iterator it=linked[i].begin(); it!=linked[i].end(); it++)
			{
				vector<int> temp=*it,temp0(3,0),temp1(3,0);
				int pid, type, id;
				if(temp.size()>4)
				{
					type=temp[0]; pid=temp[1];
				}
				else
				{
					pid=temp[0], type=temp[1], id=temp[2];				
				}		
				vector<vector<int>> edge;				
				switch(type)
				{
					case 'P': linkedProtein[pid].insert(id);          break;
					case 'S': linkedSmallMolecule[pid].insert(id);    break; 
					case 'C': linkedComplex[pid].insert(id);          break;
					case 'D': linkedDna[pid].insert(id);              break;
					case 'R': linkedReaction[pid].insert(id);         break;
					case 'E': linkedPhysicalEntity[pid].insert(id);   break;					
					case 'L': linkedANode[pid].insert(id);   break;
					case 'M': linkedCompartment[pid].insert(id);      break;	
					case 'W': linkedPathway[pid].insert(id);          break;	
					case 'A': 
							temp1[0]=temp[4], temp1[1]=temp[5], temp1[2]=temp[6];
							temp0[0]=temp[1], temp0[1]=temp[2], temp0[2]=temp[3];
							edge.push_back(temp0);
							edge.push_back(temp1);
							linkedEdge[pid].insert(edge);           break;
				}
			}
		}
	}
}




bool PathBubble1::sharedDifferedLinkedExpressedtoHighlighted(bool toHighlight, bool is_Share, bool is_Differ, bool is_Link, bool is_Expressed)
{ //only allow link to be add to highlighted
  //toHighLight: false is only for expression search
	//if nothing is highlighted
	//highlight pathway
	sharedDifferedLinkedExpressedHighLightedSizeCheck();
    vector<PathBubble1 *> mList=_scene->extractPathBubblesinGroup(this);
	
	if(is_Share)
	   sharedNodetoHighlighted(toHighlight);		

	//differed
	if(is_Differ)
	for(int j=0; j<mList.size(); j++) 
	{
		PathBubble1* pBubble = mList[j];		 
		for(int m=0; m<9; m++)
		{
			int ctype;
			vector<set<int>> differedNode;
			vector<vector<QRectF>> nodePos;
			switch(m)
			{
				case 0: ctype='P';  differedNode=pBubble->differedProtein;			nodePos=_scene->_proteinPos;    break;
				case 1: ctype='S';  differedNode=pBubble->differedSmallMolecule;	nodePos=_scene->_smallMoleculePos; break;
				case 2: ctype='C';  differedNode=pBubble->differedComplex;			nodePos=_scene->_complexPos;    break;
				case 3: ctype='D';  differedNode=pBubble->differedDna;				nodePos=_scene->_DnaPos;           break;
				case 4: ctype='R';  differedNode=pBubble->differedReaction;			nodePos=_scene->_reactionPos;  break;
				case 5: ctype='E';  differedNode=pBubble->differedPhysicalEntity;	nodePos=_scene->_physicalEntityPos;break;	
				case 6: ctype='L';  differedNode=pBubble->differedANode;	        nodePos=_scene->_ANodePos;break;	
				case 7: ctype='M';  differedNode=pBubble->differedCompartment;		nodePos=_scene->_compartmentPos; break;	
				case 8: ctype='W';  differedNode=pBubble->differedPathway; break;	
			}	
			if(toHighlight)
			{
				vector<PathBubble1 *> mlist=mList;	 
				for(int i=0; i<mlist.size(); i++) 
				{
					PathBubble1* pbubble = mlist[i];		 						
					if(differedNode.size()>pbubble->m_pathwayID)
					for(set<int>::iterator it=differedNode[pbubble->m_pathwayID].begin(); it!=differedNode[pbubble->m_pathwayID].end(); it++)
					{
						int id=*it;
						vector<int> temp(2,0);
						temp[0]=ctype, temp[1]=id;	
						if(m==8 || (id<nodePos[pbubble->m_pathwayID].size() && nodePos[pbubble->m_pathwayID][id].x()>-990.0) )
							pbubble->highlighted.insert(temp);
					}
				}
			}
		}	
	}

	//linked
	if(is_Link)
	for(int j=0; j<mList.size(); j++) 
	{		
		PathBubble1* pBubble = mList[j];	
		for(int m=0; m<9; m++)
		{
			int ctype;
			vector<set<int>> linkedNode;
			vector<vector<QRectF>> nodePos;
			switch(m)
			{
				case 0: ctype='P';  linkedNode=pBubble->linkedProtein;        nodePos=_scene->_proteinPos;  break;
				case 1: ctype='S';  linkedNode=pBubble->linkedSmallMolecule;  nodePos=_scene->_smallMoleculePos;  break;
				case 2: ctype='C';  linkedNode=pBubble->linkedComplex;        nodePos=_scene->_complexPos;      break;
				case 3: ctype='D';  linkedNode=pBubble->linkedDna;            nodePos=_scene->_DnaPos;     break;
				case 4: ctype='R';  linkedNode=pBubble->linkedReaction;       nodePos=_scene->_reactionPos;  break;
				case 5: ctype='E';  linkedNode=pBubble->linkedPhysicalEntity; nodePos=_scene->_physicalEntityPos;  break;	
				case 6: ctype='L';  linkedNode=pBubble->linkedANode;          nodePos=_scene->_ANodePos;  break;	
				case 7: ctype='M';  linkedNode=pBubble->linkedCompartment;    nodePos=_scene->_compartmentPos; break;	
				case 8: ctype='W';  linkedNode=pBubble->linkedPathway;        break;	

			}
			if(toHighlight)
			{
				vector<PathBubble1 *> mlist=mList;	 
				for(int i=0; i<mlist.size(); i++) 
				{
					PathBubble1* pbubble = mlist[i];		 						
					if(linkedNode.size()>pbubble->m_pathwayID)
					for(set<int>::iterator it=linkedNode[pbubble->m_pathwayID].begin(); it!=linkedNode[pbubble->m_pathwayID].end(); it++)
					{
						int id=*it;
						vector<int> temp(2,0);
						temp[0]=ctype, temp[1]=id;
						if(id>=0)
						{
							if(m==8 || (id<nodePos[pbubble->m_pathwayID].size() && nodePos[pbubble->m_pathwayID][id].x()>-990.0)) //bug unsolved? why
								pbubble->highlighted.insert(temp);
						}
					}
				}
			}
		}	
		
		if(toHighlight)
		{
			vector<set< vector< vector <int> > > > linkedEdge=pBubble->linkedEdge;
			vector<PathBubble1 *> mlist=mList;	 
			for(int i=0; i<mlist.size(); i++) 
			{
				 PathBubble1* pbubble = mlist[i]; 						
				 if(linkedEdge.size()>pbubble->m_pathwayID)
				 for(set< vector<vector<int> > >::iterator it=linkedEdge[pbubble->m_pathwayID].begin(); it!=linkedEdge[pbubble->m_pathwayID].end(); it++)
				 {
                
					 pbubble->highlightedEdge.insert(*it);
				 }
			}
		}
	}

    //expressed
	if(is_Expressed)
	for(int j=0; j<mList.size(); j++) 
	{
		
		PathBubble1* pBubble = mList[j];	
		vector<vector<QRectF>> nodePos;
		for(int m=0; m<9; m++)
		{
			int ctype;
			vector<set<int>> expressedNode;
			switch(m)
			{
				case 0: ctype='P';  expressedNode=pBubble->expressedProtein;		nodePos=_scene->_proteinPos;		break;
				case 1: ctype='S';  expressedNode=pBubble->expressedSmallMolecule;	nodePos=_scene->_smallMoleculePos;  break;
				case 2: ctype='C';  expressedNode=pBubble->expressedComplex;		nodePos=_scene->_complexPos;		break;
				case 3: ctype='D';  expressedNode=pBubble->expressedDna;			nodePos=_scene->_DnaPos;			break;
				case 4: ctype='R';  expressedNode=pBubble->expressedReaction;		nodePos=_scene->_reactionPos;		break;
				case 5: ctype='E';  expressedNode=pBubble->expressedPhysicalEntity; nodePos=_scene->_physicalEntityPos; break;	
				case 6: ctype='L';  expressedNode=pBubble->expressedANode;          nodePos=_scene->_ANodePos; break;	
				case 7: ctype='M';  expressedNode=pBubble->expressedCompartment;	nodePos=_scene->_compartmentPos;	break;	
				case 8: ctype='W';  expressedNode=pBubble->expressedPathway;        break;	
			}	
		
		
			if(toHighlight)
		    {
				//vector<PathBubble1 *> mlist=mList;	 
				for(int i=0; i<expressedNode.size(); i++) 
				{
					//PathBubble1* pbubble = mlist[i];		 						
					//if(expressedNode.size()>pBubble->m_pathwayID)
					for(set<int>::iterator it=expressedNode[i].begin(); it!=expressedNode[i].end(); it++)
					{
						int id=*it;
						vector<int> temp(2,0);
						temp[0]=ctype, temp[1]=id;
						if(id>=0)
						{
							if(m==8 || (id<nodePos[pBubble->m_pathwayID].size() && nodePos[pBubble->m_pathwayID][id].x()>-990.0) )
						       pBubble->highlighted.insert(temp);
						}
					}
				}
			}
		}			
		
	}

	for(int j=0; j<mList.size(); j++) 
	{	
		PathBubble1* pBubble = mList[j];	
	}
	
	bool emptyHighlight=false;
	for(int j=0; j<mList.size(); j++) 
	{
		 PathBubble1* pbubble = mList[j];		 								 		   		 

		 if(!pbubble->highlighted.empty())
		 {
			   emptyHighlight = true;
			   break;
		 }
	}
	if(!emptyHighlight) // add all pathway
	{	
	   for(int j=0; j<mList.size(); j++) 
		{
			 PathBubble1* pbubble = mList[j];		 								 		   		 
			 vector<int> temp(2,0);
			 temp[0]='W', temp[1] = pbubble->m_pathwayID;
			 pbubble->highlighted.insert(temp);			
		}	
	}
	return !emptyHighlight; //added pathway as highlighted
}

void PathBubble1::cleanPathHighlight()
{	
	vector<PathBubble1 *> mList=_scene->extractPathBubblesinGroup(this);
	for(int j=0; j<mList.size(); j++) 
		{
			 PathBubble1* pbubble = mList[j];		 								 		   		 
			 vector<int> temp(2,0);
			 temp[0]='W', temp[1] = pbubble->m_pathwayID;
			 pbubble->highlighted.erase(temp);			
		}	
}

bool PathBubble1::updateGraphIfHighlightedChanged()
{
	QList<ItemBase *> mlist=_scene->getGroupMembers(this);	 
	deque<set<vector<int> > >::iterator  pit;
	set<vector<int> >  pHighlighted, pDiffered, pShared, pExpressed, pColored;
	set<int> donelist;

	deque< vector<set<vector<int> > > >::iterator lit;
	vector<set<vector<int> > >  pLinked;

	bool changed=false;
	
	for(int i=0; i<mlist.size(); i++) 
	{
		 bool flag=false;
	 	 if ( mlist[i]->getType() != SUBPATHBUBBLE1 && mlist[i]->getType() != PATHBUBBLE1)
		 	  continue;
	 	 PathBubble1* pbubble = dynamic_cast<PathBubble1*>(mlist[i]);		 								 		   		 
		 //deque<set<vector<int>>>::iterator it=pbubble->currentHighlightedit;		
		 if(pbubble->currentHighlightedit!=pbubble->highlightedStored.end())
		 {
			if(pbubble->currentHighlightedit!=pbubble->highlightedStored.begin())
			{
				pit=pbubble->currentHighlightedit;
				pit--;
				pHighlighted= *pit;
				if(*pbubble->currentHighlightedit!=pHighlighted)
				{
				   flag=true; 
				   
				}	
			}			 
			else if(pbubble->currentHighlightedit==pbubble->highlightedStored.begin())
			{
				if(!pbubble->highlightedStored.empty())
				{
				   flag=true; 
				  
				}
			}
		 }		

		 if(flag)
		 {
			changed=true;
			donelist.insert(i);
		    pbubble->getGraphToBePaint();
			continue;		 
		 }
		 //differed
		 if(pbubble->currentDifferedit!=pbubble->differedStored.end())
		 {
			if(pbubble->currentDifferedit!=pbubble->differedStored.begin())
			{
				pit=pbubble->currentDifferedit;
				pit--;
				pDiffered= *pit;
				if(*pbubble->currentDifferedit!=pDiffered)
				{
				   flag=true; 				  
				}									
			}			 
			else if(pbubble->currentDifferedit==pbubble->differedStored.begin())
			{
				if(!pbubble->differedStored.empty())
				{
				   flag=true; 
				}					
			}			
		 }		 
		 if(flag)
		 {
			 donelist.insert(i);
			 changed=true;
		    pbubble->getGraphToBePaint();
			continue;		 
		 }

		 //shared
		 if(pbubble->currentSharedit!=pbubble->sharedStored.end())
		 {
			if(pbubble->currentSharedit!=pbubble->sharedStored.begin())
			{
				pit=pbubble->currentSharedit;
				pit--;
				pShared= *pit;
				if(*pbubble->currentSharedit!=pShared)
				{
				   flag=true; 
				   
				}				
					
			}			 
			else if(pbubble->currentSharedit==pbubble->sharedStored.begin())
			{
				if(!pbubble->sharedStored.empty())
				{
				   flag=true; 
				   
				}				
			}			
		 }	
		 if(flag)
		 {
			 changed=true;
			 donelist.insert(i);
		    pbubble->getGraphToBePaint();
			continue;		 
		 }


		  //linked
		 if(pbubble->currentLinkedit!=pbubble->linkedStored.end())
		 {
			if(pbubble->currentLinkedit!=pbubble->linkedStored.begin())
			{
				lit=pbubble->currentLinkedit;
				lit--;
				pLinked= *lit;
				if(*pbubble->currentLinkedit!=pLinked)
				{
				   flag=true; 
				  
				}				
					
			}			 
			else if(pbubble->currentLinkedit==pbubble->linkedStored.begin())
			{
				if(!pbubble->linkedStored.empty())
				{
				   flag=true; 				   
				}					
			}			
		 }		 
		 if(flag)
		 {
			 changed=true;
			 donelist.insert(i);
		    pbubble->getGraphToBePaint();
			continue;		 
		 }

		  //expressed
		 if(pbubble->currentExpressedit!=pbubble->expressedStored.end())
		 {
			if(pbubble->currentExpressedit!=pbubble->expressedStored.begin())
			{
				pit=pbubble->currentExpressedit;
				pit--;
				pExpressed= *pit;
				if(*pbubble->currentExpressedit!=pExpressed)
				{
				   flag=true; 				  
				}		
			}			 
			else if(pbubble->currentExpressedit==pbubble->expressedStored.begin())
			{
				if(!pbubble->expressedStored.empty())
				{
				   flag=true; 				   
				}					
			}			
		 }		 
		 if(flag)
		 {
			 changed=true;
			 donelist.insert(i);
		    pbubble->getGraphToBePaint();
			continue;		 
		 }

		  //colored
		 if(pbubble->currentColoredit!=pbubble->coloredStored.end())
		 {
			if(pbubble->currentColoredit!=pbubble->coloredStored.begin())
			{
				pit=pbubble->currentColoredit;
				pit--;
				pColored= *pit;
				if(*pbubble->currentColoredit!=pColored)
				{
				   flag=true; 				  
				}		
			}			 
			else if(pbubble->currentColoredit==pbubble->coloredStored.begin())
			{
				if(!pbubble->coloredStored.empty())
				{
				   flag=true; 				   
				}					
			}			
		 }		 
		 if(flag)
		 {
			 changed=true;
			 donelist.insert(i);
			 pbubble->getGraphToBePaint();
			 continue;		 
		 }
	}
	return changed;
	 
}



bool PathBubble1::isHighlightedChanged()
{
	QList<ItemBase *> mlist=_scene->getGroupMembers(this);	 
	deque<set<vector<int> > >::iterator  pit;

    set<vector<int> >  pHighlighted, pDiffered, pShared,  pExpressed;

	deque<vector< set<vector<int> > > >::iterator lit;
	vector<set<vector<int> > > pLinked;
	for(int i=0; i<mlist.size(); i++) 
	{
	 	 if ( mlist[i]->getType() != SUBPATHBUBBLE1 && mlist[i]->getType() != PATHBUBBLE1)
		 	  continue;
	 	 PathBubble1* pbubble = dynamic_cast<PathBubble1*>(mlist[i]);		 								 		   		 
		 //deque<set<vector<int>>>::iterator it=pbubble->currentHighlightedit;		
		 if(pbubble->currentHighlightedit!=pbubble->highlightedStored.end())
		 {
			if(pbubble->currentHighlightedit!=pbubble->highlightedStored.begin())
			{
				pit=pbubble->currentHighlightedit;
				pit--;
				pHighlighted= *pit;
				if(*pbubble->currentHighlightedit!=pHighlighted)
					return true;
			}			 
			else if(pbubble->currentHighlightedit==pbubble->highlightedStored.begin())
			{
				if(!(*pbubble->highlightedStored.begin()).empty())
					return true;
			}
		 }		

		 //differed
		 if(pbubble->currentDifferedit!=pbubble->differedStored.end())
		 {
			if(pbubble->currentDifferedit!=pbubble->differedStored.begin())
			{
				pit=pbubble->currentDifferedit;
				pit--;
				pDiffered= *pit;
				if(*pbubble->currentDifferedit!=pDiffered)
					return true;			
					
			}			 
			else if(pbubble->currentDifferedit==pbubble->differedStored.begin())
			{
				if(!(*pbubble->differedStored.begin()).empty())
					return true;				
			}			
		 }		 

		 if(pbubble->currentSharedit!=pbubble->sharedStored.end())
		 {
			if(pbubble->currentSharedit!=pbubble->sharedStored.begin())
			{
				pit=pbubble->currentSharedit;
				pit--;
				pShared= *pit;
				if(*pbubble->currentSharedit!=pShared)
					return true;		
			}			 
			else if(pbubble->currentSharedit==pbubble->sharedStored.begin())
			{
				if(!(*pbubble->sharedStored.begin()).empty())
					return true;				
			}			
		 }		

		 if(pbubble->currentLinkedit!=pbubble->linkedStored.end())
		 {
			if(pbubble->currentLinkedit!=pbubble->linkedStored.begin())
			{
				lit=pbubble->currentLinkedit;
				lit--;
				pLinked= *lit;
				if(*pbubble->currentLinkedit!=pLinked)
					return true;	
			}			 
			else if(pbubble->currentLinkedit==pbubble->linkedStored.begin())
			{
				if(!(*pbubble->linkedStored.begin()).empty())
					return true;				
			}			
		 }
		  //expressed
		 if(pbubble->currentExpressedit!=pbubble->expressedStored.end())
		 {
			if(pbubble->currentExpressedit!=pbubble->expressedStored.begin())
			{
				pit=pbubble->currentExpressedit;
				pit--;
				pExpressed= *pit;
				if(*pbubble->currentExpressedit!=pExpressed)
					return true;			
					
			}			 
			else if(pbubble->currentExpressedit==pbubble->expressedStored.begin())
			{
				if(!(*pbubble->expressedStored.begin()).empty())
					return true;				
			}			
		 }		 
	}
	return false;	 
}


vector<vector<int>> PathBubble1::findSpannedEdge(set<vector<vector<int> > > edgeconnections, vector<int> node)
{
	vector<vector<int>> edge;
	for(set<vector<vector<int>>> ::iterator it= edgeconnections.begin(); it!=edgeconnections.end(); it++)
	{
		vector<vector<int>> temp=*it;
		for(vector<vector<int>>::iterator jt= temp.begin(); jt!=temp.end(); jt++)
		{
			vector<int> h1=*jt;
			if(h1==node)
			{
				if(jt==temp.begin())
				{
				   edge.push_back(h1);
				   edge.push_back(*temp.rbegin());				
				}
				else
				{
				   edge.push_back(*temp.begin());				
				   edge.push_back(h1);
				}
				//return edge;
			}
		}
	}
	return edge;
}



void PathBubble1::ExtendLinkedNodesSize()
{
        int csize;			
		csize=linkedProtein.size();
		int size=_scene->_compartmentName.size();
		for(int i=csize; i<size; i++)
		{  	 
				set<int> record;   
				linkedProtein.push_back(record);	
		} 
		csize=linkedSmallMolecule.size();
		for(int i=csize; i<size; i++)
		{  	 
				set<int> record;   
				linkedSmallMolecule.push_back(record);	
		} 
		csize=linkedComplex.size();
		for(int i=csize; i<size; i++)
		{  	 
			set<int> record;  
			linkedComplex.push_back(record);
		} 
		csize=linkedDna.size();
		for(int i=csize; i<size; i++)
		{  
			set<int> record;   
			linkedDna.push_back(record);	
		} 
		csize=linkedReaction.size();
		for(int i=csize; i<size; i++)
		{  	
			set<int> record;   
			linkedReaction.push_back(record);	
		} 
		csize=linkedPhysicalEntity.size();
		for(int i=csize; i<size; i++)
		{  	
			set<int> record;  
			linkedPhysicalEntity.push_back(record);
		} 
		csize=linkedANode.size();
		for(int i=csize; i<size; i++)
		{  	
			set<int> record;  
			linkedANode.push_back(record);
		} 
	    csize=linkedCompartment.size();
		for(int i=csize; i<size; i++)
		{  	 
				set<int> record;   
				linkedCompartment.push_back(record);	
		} 
		csize=linkedEdge.size();
		for(int i=csize; i<size; i++)
		{  	 
				set<vector<vector<int> > > record;   
				linkedEdge.push_back(record);	
		} 
}


void PathBubble1::searchLinkedNode(vector<PathBubble1 *> mlist, int pflag, int cflag, int wflag, bool Andflag)
{       
	//expand the sharedprotein lenghth as needed
	if(pflag*cflag!=0||pflag*wflag!=0||cflag*wflag!=0)
		return;

	set<vector<int>> sID;
    for(int i=0; i<mlist.size(); i++) 
	{
			PathBubble1* pbubble = mlist[i];	
			vector<int> temp1(3,0),temp2(3,0);		
			vector<vector<int>> edge1(2,temp1),edge2(2,temp1);			  
			pbubble->ExtendLinkedNodesSize();		
		    for(set<vector<int>>::iterator it = pbubble->highlighted.begin(); it != pbubble->highlighted.end(); it++)
			{
				vector<int> tmp,item=*it;
				int type=item[0],id=item[1];		
				tmp.push_back(pbubble->m_pathwayID); 			  
				if(type=='W')
				{
			  		  tmp.push_back(id);
					  tmp.push_back(-1); 
					  tmp.push_back(-1);
					  tmp.push_back(type); 			  
				}
				else if(type=='M')
				{
					  tmp.push_back(-1);
					  tmp.push_back(id); 
					  tmp.push_back(-1);
					  tmp.push_back(type); 
					  if(id>=_scene->CompartmentContain[pbubble->m_pathwayID].size()) //do not know about reason
					     continue;
				}
				else
				{
					  tmp.push_back(-1);			  			      
					  tmp.push_back(-1);
					  tmp.push_back(id);
					  tmp.push_back(type); 
				}
				sID.insert(tmp);			
			}
	}


	for(int i=0; i<mlist.size(); i++) 
	{
			PathBubble1* pbubble = mlist[i];	
			vector<int> temp1(3,0),temp2(3,0);		
			vector<vector<int>> edge1(2,temp1),edge2(2,temp1);			  
			//pbubble->ExtendLinkedNodesSize();
			//put edges in edgeconnections edge to myEdge	
			//for(int m=0;m<7;m++)
		
			//QList<ItemBase *> mlist=_scene->getGroupMembers(this);
			//for(int i=0; i<mlist.size(); i++) 
			
			set<vector<int>> linkedNode,linkedNode1,selfCompartment;
			//PathBubble1* pbubble = dynamic_cast<PathBubble1*>(mlist[i]);		 		 
			//PathBubble1* pbubble = dynamic_cast<PathBubble1*>(this);	
					
			//sID
			//////////////////////////////////////////////////////////
			if(_scene->linkSearchType == 1)	
			{
				//1. search protein level linkage
				//vector<int> countRecord(sID.size(),0);					
				//int count1=0,countid;
				set<vector<vector<int> > > edgeSet;
				for(set<vector<int>>::iterator it=sID.begin(); it!=sID.end(); it++)
				{
					//count1++;
					vector<int> item=*it;
					int Pid=item[0], pid=item[1],cid=item[2], id=item[3], type=item[4];
					vector<int> PidVector, idVector, typeVector;
					PidVector.push_back(Pid), idVector.push_back(id), typeVector.push_back(type);
					if(type == 'M')
					{
						idVector[0]=cid;
						for(set<vector<int>>::iterator it=_scene->CompartmentContain[Pid][cid].begin(); it!=_scene->CompartmentContain[Pid][cid].end(); it++)
						{   
							vector<int> temp=*it;
							PidVector.push_back(Pid), idVector.push_back(temp[1]), typeVector.push_back(temp[0]);
						}
					}
					//vector<int> node1(2,0),node2(2,0);
					//node1[0]=type,  node1[1]=id;						
					//int count2=0;
					for(set<vector<int>>::iterator jt=sID.begin(); jt!=sID.end(); jt++)
					{	
						//count2++;
						//countid=count1<=count2?count1:count2;
						
						vector<int> item1=*jt;
						int Pid1=item1[0], pid1=item1[1], cid1=item1[2], id1=item1[3], type1=item1[4];
						vector<int> PidVector1, idVector1, typeVector1;
						
						PidVector1.push_back(Pid1), idVector1.push_back(id1), typeVector1.push_back(type1);

						if(Pid1<0 || item1==item)
							continue;

						if(type1 == 'M')
						{
							idVector1[0]=cid1;
							for(set<vector<int>>::iterator it=_scene->CompartmentContain[Pid1][cid1].begin(); it!=_scene->CompartmentContain[Pid1][cid1].end(); it++)
							{   
								vector<int> temp=*it;
								PidVector1.push_back(Pid1), idVector1.push_back(temp[1]), typeVector1.push_back(temp[0]);
							}
						}
						//PathBubble1 *cbubble = _scene->_pathBubbles[Pid1];						
						for(int k=0; k<PidVector.size(); k++)
						for(int l=0; l<PidVector1.size(); l++)
						{	
							if(!_scene->_pathBubbles[PidVector[k]]->isItemVisible(typeVector[k], idVector[k]) || !_scene->_pathBubbles[PidVector1[l]]->isItemVisible(typeVector1[l], idVector1[l]) )
								continue;

							vector< vector< set< vector< vector<int> > > > > pathVector = _scene->_wholeGraph->findPathforTwoNodes(PidVector[k], typeVector[k], idVector[k], PidVector1[l], typeVector1[l], idVector1[l]); //cbubble->findPathforTwoNodes(type, id, type1, id1, cbubble->edge);
							//if(pflag==1 && !cflag && !wflag)
							{
								for(int pi=0; pi<pathVector.size(); pi++)
								{
									for(int ri=0; ri<pathVector[pi].size(); ri++)
									for(set< vector< vector<int> > >::iterator it1 = pathVector[pi][ri].begin(); it1 !=pathVector[pi][ri].end(); it1++)
									{
										_scene->_pathBubbles[pi]->linkedEdge[pi].insert(*it1);
										vector< vector<int> > AEdge = *it1;					
										linkedNode.insert(AEdge[0]);
										linkedNode.insert(AEdge[1]);
										edgeSet.insert(AEdge);	
									}
									//store them into vector < vector< vector<int> > >  pathVectors; //pathVectors[0] record major pathes, different color will be assigned to show branches of pathes
									//branch nodes                      				
									if(_scene->pathVectors.size()<=pi)
										_scene->pathVectors.resize(pi+1);

									//int branchsize = pathVector[pi].size();

									for(int ri=0; ri<pathVector[pi].size(); ri++)
						    		//for(set< vector< vector<int> > >::iterator it1 = pathVector[pi][ri].begin(); it1 !=pathVector[pi][ri].end(); it1++)
									{
										//vector< vector<int> > AEdge = *it1;		
										_scene->pathVectors[pi].push_back(pathVector[pi][ri]);
										
									}
								}						
							}
						}
					}//for(set<vector<int>>::iterator jt=sID.begin(); jt!=sID.end(); jt++)				
			    }
				
				//2. categorical search and store linked		
				if(!pflag && cflag==1 && !wflag) //for compartment
				{
					vector<int> node,temp(2,0),cnode(3,0);
					for(set<vector<int>>::iterator ij=sID.begin(); ij!=sID.end(); ij++)
				    {
						vector<int> item=*ij;
						int Pid=item[0], pid=item[1],cid=item[2], id=item[3], type=item[4];
					    
						vector<int> starter(3,0);	
						starter[0]=Pid, starter[1]=type, starter[2]=id;		

						for(set<vector<int>>::iterator it=sID.begin(); it!=sID.end(); it++)
						{
							vector<int> node=*it;
							int Pid=item[0], pid=item[1],cid=item[2], id=item[3], type=item[4];										
							if(node[4] == 'M')
							{
								cnode[0]=Pid; cnode[1]=type; cnode[2]=cid;	
								selfCompartment.insert(cnode);	
							}	
							else if(node[4] == 'W')
							{

							}
							else
							{
								for(int m=0; m<_scene->CompartmentContain[Pid].size(); m++)
								{
									temp[0]=type; temp[1]=id;	
									if(_scene->CompartmentContain[Pid][m].find(temp)!=_scene->CompartmentContain[Pid][m].end())
									{
										cnode[0]=Pid; cnode[1]='M'; cnode[2]=m;
										selfCompartment.insert(temp);	
									}
								}	
							}
						}
						for(set<vector<int>>::iterator it=linkedNode.begin(); it!=linkedNode.end(); it++)
						{
							node=*it;
							int  pid=node[0], type=node[1], id=node[2]; //
							temp[0]=node[1]; temp[1]=node[2];	
						
							if( type!='M' && type!= 'W')
							{
								for(int m=0; m<_scene->CompartmentContain[pid].size(); m++)
								{
									if(_scene->CompartmentContain[pid][m].find(temp)!=_scene->CompartmentContain[pid][m].end())
									{
										vector<int> temp1(3,0);
						  				temp1[0]=pid, temp1[1]='M'; temp1[2]=m;
										if( selfCompartment.find(temp1)==selfCompartment.end() )
										{
											//vector<int> temp3(3,0); temp3[0]=pid, temp3[1]=temp1[0], temp3[2]=temp1[1];
										 	linkedNode1.insert(temp1);	
											linkedNode1.insert(starter);

											vector<vector<vector< int > > > ledge=_scene->_pathBubbles[pid]->searchLink(item[4], item[3], type, id, edgeSet);
											for(int k=0; k<ledge.size(); k++)
												_scene->_pathBubbles[pid]->linkedEdge[pid].insert(ledge[k]);
										 										 
										}
									}
								}					
							}
							else if(type == 'M')
							{
								vector<int> temp3(3,0); temp3[0]=pid, temp3[1]=temp[0], temp3[2]=temp[1];
								if( selfCompartment.find(temp3)==selfCompartment.end() )
								{
								
									linkedNode1.insert(temp3);	
									linkedNode1.insert(starter);
									vector<vector<vector< int > > > ledge = _scene->_pathBubbles[pid]->searchLink(item[4], item[3], type, id, edgeSet);
									for(int k=0; k<ledge.size(); k++)
										_scene->_pathBubbles[pid]->linkedEdge[pid].insert(ledge[k]);
								}
							}
							else if(type == 'W')
							{
								//linkedNode1.insert(temp);	
							}
						}//for linked node
						linkedNode=linkedNode1;
			        }	
				}
				//for c link, p link, and pa link
				for(set<vector<int>>::iterator it=linkedNode.begin(); it!=linkedNode.end(); it++)
				{
					vector<int> node=*it;
					int pid=node[0], type=node[1], id=node[2];
					if(pid != pbubble->m_pathwayID)
					{
						_scene->_pathBubbles[pid]->ExtendLinkedNodesSize();
					   
					}
					switch(type)
					{
						case 'P': _scene->_pathBubbles[pid]->linkedProtein[pid].insert(id);  break;
						case 'S': _scene->_pathBubbles[pid]->linkedSmallMolecule[pid].insert(id);  break;
						case 'C': _scene->_pathBubbles[pid]->linkedComplex[pid].insert(id);  break;
						case 'D': _scene->_pathBubbles[pid]->linkedDna[pid].insert(id);  break;
						case 'R': _scene->_pathBubbles[pid]->linkedReaction[pid].insert(id);  break;
						case 'E': _scene->_pathBubbles[pid]->linkedPhysicalEntity[pid].insert(id);  break;
						case 'L': _scene->_pathBubbles[pid]->linkedANode[pid].insert(id);  break;
						case 'M': _scene->_pathBubbles[pid]->linkedCompartment[pid].insert(id);  break;
					}		
				}
				if(_scene->linkSearchType == 1)			
	                break;
			}	
			/////////////////
			else if(_scene->linkSearchType == 0)			
			{
				for(set<vector<int>>::iterator it=sID.begin(); it!=sID.end(); it++)
			    {
					vector<int> item=*it;
					int Pid=item[0], pid=item[1],cid=item[2], id=item[3], type=item[4];
					vector<int> node1(2,0),node2(2,0);
					node1[0]=type,  node1[1]=id;
					set<vector<vector<int> > > edgeSet;
					vector<int> starter(3,0);	
					starter[0]=Pid, starter[1]=type, starter[2]=id;
							
					if(Pid!=pbubble->m_pathwayID)
							 continue;

					vector<vector<int>> edgeRecord,edgeRecord3;
					vector<vector<vector<int>>>  edgeRecord1,edgeRecord2; 
					edgeRecord2.resize(1);
					switch(type)
					{
						case 'P': edgeRecord = pbubble->_protein[id]; break;
						case 'L': edgeRecord = pbubble->_ANode[id]; break;
						case 'S': edgeRecord = pbubble->_smallMolecule[id]; break;
						case 'C': edgeRecord = pbubble->_complex[id]; break;
						case 'D': edgeRecord = pbubble->_Dna[id]; break;
						case 'R': edgeRecord = pbubble->_reaction[id]; break;
						case 'E': edgeRecord = pbubble->_entity[id]; break;
						case 'M': 
							for(set<vector<int>>::iterator it=_scene->CompartmentContain[pbubble->m_pathwayID][cid].begin(); it!=_scene->CompartmentContain[pbubble->m_pathwayID][cid].end(); it++)
							{   
								vector<int> temp=*it;
								switch(temp[0])
								{
									case 'P': edgeRecord2[0]=pbubble->_protein[temp[1]];       break;
									case 'L': edgeRecord2[0]=pbubble->_ANode[temp[1]];       break;
									case 'S': edgeRecord2[0]=pbubble->_smallMolecule[temp[1]]; break;
									case 'C': edgeRecord2[0]=pbubble->_complex[temp[1]];       break;
									case 'D': edgeRecord2[0]=pbubble->_Dna[temp[1]];           break;
									case 'R': edgeRecord2[0]=pbubble->_reaction[temp[1]];      break;
									case 'E': edgeRecord2[0]=pbubble->_entity[temp[1]];        break;							    
								}							
							
								vector<int> node;						
								for(int k=0; k<edgeRecord2.size(); k++)
								for(int l=0; l<edgeRecord2[k].size(); l++)
								{
									edgeRecord3.clear();
									node=edgeRecord2[k][l];
									temp2[0]=pbubble->m_pathwayID; temp2[1]=node[0]; temp2[2]=node[1];	
									if(temp2[1]=='R')
									{   
										if( temp2[2]>=0 && _scene->_reactionPos[pbubble->m_pathwayID][temp2[2]].x()>-999.99)         edgeRecord3=pbubble->_reaction[temp2[2]];      
									}

									vector<int> temp3(3,-1), temp5(3,-1);
									temp3[0]=temp[0]; temp3[1]=temp[1];	
									edgeRecord3.push_back(temp3);
									
									//edgeRecord3=pbubble->_reaction[temp2[2]];								
									for(int q=0; q<edgeRecord3.size(); q++)
									{
										node=edgeRecord3[q];
										temp5[0]=pbubble->m_pathwayID; temp5[1]=node[0]; temp5[2]=node[1];		
																				
										edge1[0]=temp5, edge1[1]=temp2;				
										edge2[1]=temp5, edge2[0]=temp2;	

										bool flag= (pbubble->updatedEdge_1.find(edge1)!=pbubble->updatedEdge_1.end()||pbubble->updatedEdge_1.find(edge2)!=pbubble->updatedEdge_1.end()) || (pbubble->updatedEdge_2.find(edge1)!=pbubble->updatedEdge_2.end()||pbubble->updatedEdge_2.find(edge2)!=pbubble->updatedEdge_2.end()) || (pbubble->updatedEdge_3.find(edge1)!=pbubble->updatedEdge_3.end()||pbubble->updatedEdge_3.find(edge2)!=pbubble->updatedEdge_3.end() || (pbubble->updatedEdge_4.find(edge1)!=pbubble->updatedEdge_4.end() || pbubble->updatedEdge_4.find(edge2)!=pbubble->updatedEdge_4.end()));
										//
											
										vector<vector<vector<int>>> tEdge;
										tEdge.resize(4);
										tEdge[0]=findSpannedEdge(scene->edgeconnections, temp2);
										tEdge[1]=findSpannedEdge(scene->edgeconnections_A, temp2);
										tEdge[2]=findSpannedEdge(scene->edgeconnections_I, temp2);
										tEdge[3]=findSpannedEdge(scene->edgeconnections_4, temp2);   
										for(int i=0; i<4; i++)
										{
											vector<vector<int>> sEdge=tEdge[i];//=findSpannedEdge(scene->edgeconnections, temp2);
											if( flag || !sEdge.empty())
											{
												int pid;
												if(flag)
													pid=pbubble->m_pathwayID;
												else pid=sEdge[1][0];

												vector<int> temp4(2,0); temp4[0]=temp2[1], temp4[1]=temp2[2];
												if(_scene->CompartmentContain[pbubble->m_pathwayID][cid].find(temp4)==_scene->CompartmentContain[pbubble->m_pathwayID][cid].end())
												{	
													if(pflag==1 && !cflag && !wflag)
													{
														pbubble->linkedEdge[pbubble->m_pathwayID].insert(edge1);
														pbubble->linkedEdge[pbubble->m_pathwayID].insert(edge2);		
														if(!flag)
														{
															pbubble->linkedEdge[pid].insert(sEdge);
															vector<vector<int>> eEdge;
															eEdge.push_back(sEdge[1]);  eEdge.push_back(sEdge[0]);
															//[0]=temp, edge1[1]=temp2;													
															pbubble->linkedEdge[pid].insert(eEdge);		
														}
													}
													edgeSet.insert(edge1);
													edgeSet.insert(edge2);					

													linkedNode.insert(temp2);	
													linkedNode.insert(starter);	
													if(!flag)
													{
														if(sEdge[0][0]!=pbubble->m_pathwayID)
																linkedNode.insert(sEdge[0]);
														else 
																linkedNode.insert(sEdge[1]);
													}
												}
											}
										}//for(int i=0; i<4; i++)											
									}						   
								}	                       
						}
                        edgeRecord.clear();
							break;

						
                    } 
					//2. ////// further check
					vector<int> node,temp(3,0);
					temp2[0]=pbubble->m_pathwayID; temp2[1]=type; temp2[2]=id;
					for(int k=0; k<edgeRecord.size(); k++)
					{
						node=edgeRecord[k];

						temp[0]=pbubble->m_pathwayID; temp[1]=node[0]; temp[2]=node[1];

						edge1[0]=temp, edge1[1]=temp2;				

						edge2[1]=temp, edge2[0]=temp2;			

						//vector<vector<int>> sEdge=findSpannedEdge(scene->edgeconnections_A, temp2);
						bool flag= (pbubble->updatedEdge_1.find(edge1)!=pbubble->updatedEdge_1.end()||pbubble->updatedEdge_1.find(edge2)!=pbubble->updatedEdge_1.end()) || (pbubble->updatedEdge_2.find(edge1)!=pbubble->updatedEdge_2.end()||pbubble->updatedEdge_2.find(edge2)!=pbubble->updatedEdge_2.end()) || (pbubble->updatedEdge_3.find(edge1)!=pbubble->updatedEdge_3.end()||pbubble->updatedEdge_3.find(edge2)!=pbubble->updatedEdge_3.end() || (pbubble->updatedEdge_4.find(edge1)!=pbubble->updatedEdge_4.end() || pbubble->updatedEdge_4.find(edge2)!=pbubble->updatedEdge_4.end()));
						vector<vector<vector<int>>> tEdge;
						tEdge.resize(4);
						tEdge[0]=findSpannedEdge(scene->edgeconnections, temp2);
						tEdge[1]=findSpannedEdge(scene->edgeconnections_A, temp2);
						tEdge[2]=findSpannedEdge(scene->edgeconnections_I, temp2);
						tEdge[3]=findSpannedEdge(scene->edgeconnections_4, temp2);
                                            
						for(int i=0; i<4; i++)
						{
								vector<vector<int>> sEdge=tEdge[i];//=findSpannedEdge(scene->edgeconnections, temp2);
								if( flag || !sEdge.empty())
								{
									linkedNode.insert(temp);
									linkedNode.insert(starter);
									linkedNode.insert(temp2);	
									linkedNode.insert(starter);
									for(int eid=0; eid<(sEdge.empty()?2:sEdge.size()); eid=eid+2)//
									{
										int pid;
										if(flag)
											pid=pbubble->m_pathwayID;
										else pid=sEdge[eid+1][0];	
										if(!flag)
										{
											if(sEdge[eid][0]!=pbubble->m_pathwayID)
												linkedNode.insert(sEdge[eid]);
											else 
												linkedNode.insert(sEdge[eid+1]);
										}
										if(pflag==1 && !cflag && !wflag)
										{
											pbubble->linkedEdge[pbubble->m_pathwayID].insert(edge1);
											pbubble->linkedEdge[pbubble->m_pathwayID].insert(edge2);
											if(!flag)
											{
												vector<vector<int>> tEdge;
												tEdge.resize(2);

												tEdge[1]=sEdge[eid+1];  tEdge[0]=sEdge[eid];																							
												pbubble->linkedEdge[pid].insert(tEdge);
												
												tEdge[0]=sEdge[eid+1];  tEdge[1]=sEdge[eid];																							
												pbubble->linkedEdge[pid].insert(tEdge);		
											}
										}
										edgeSet.insert(edge1);	//keqin?
										edgeSet.insert(edge2);
										if(!flag)
										{
											if(sEdge[eid][0]!=pbubble->m_pathwayID)
												linkedNode.insert(sEdge[eid]);
											else 
												linkedNode.insert(sEdge[eid+1]);
										}
										if(temp[0]=='R')
										{
											edgeRecord1.push_back(pbubble->_reaction[temp[1]]);
										}	
									}
								}								
							}//for(int i=0; i<4; i++)
					}//	for(int k=0; k<edgeRecord.size(); k++)

					//3. //put it in category
					if(!pflag && cflag==1 && !wflag) //for compartment
					{
					   vector<int> node,temp(2,0),cnode(3,0);
					   for(set<vector<int>>::iterator it=sID.begin(); it!=sID.end(); it++)
					   {
							vector<int> node=*it;
							int Pid=item[0], pid=item[1],cid=item[2], id=item[3], type=item[4];
							node=*it;					
							if(node[4] == 'M')
							{
								cnode[0]=Pid; cnode[1]=type; cnode[2]=cid;	
								selfCompartment.insert(cnode);	
							}	
							else if(node[4] == 'W')
							{
							}
							else
							{
							   for(int m=0; m<_scene->CompartmentContain[Pid].size(); m++)
							   {
									  temp[0]=type; temp[1]=id;	
									  if(_scene->CompartmentContain[Pid][m].find(temp)!=_scene->CompartmentContain[Pid][m].end())
									  {
										  cnode[0]=Pid; cnode[1]='M'; cnode[2]=m;
										  selfCompartment.insert(temp);	
									  }
							   }	
							}
						}
					   for(set<vector<int>>::iterator it=linkedNode.begin(); it!=linkedNode.end(); it++)
					   {
							node=*it;
							int  pid=node[0], type=node[1], id=node[2]; //
							temp[0]=node[1]; temp[1]=node[2];	
						
							if( type!='M' && type!= 'W')
							{
								for(int m=0; m<_scene->CompartmentContain[pid].size(); m++)
								{
									if(_scene->CompartmentContain[pid][m].find(temp)!=_scene->CompartmentContain[pid][m].end())
									{
										 vector<int> temp1(3,0);
						  				 temp1[0]=pid, temp1[1]='M'; temp1[2]=m;
										 if( selfCompartment.find(temp1)==selfCompartment.end() )
										 {
											 //vector<int> temp3(3,0); temp3[0]=pid, temp3[1]=temp1[0], temp3[2]=temp1[1];
										 
											 linkedNode1.insert(temp1);	
											 linkedNode1.insert(starter);

											 vector<vector<vector< int > > > ledge=_scene->_pathBubbles[pid]->searchLink(item[4], item[3], type, id, edgeSet);
											 for(int k=0; k<ledge.size(); k++)
												 pbubble->linkedEdge[pid].insert(ledge[k]);
										 										 
										 }
									}
								}					
							}
							else if(type == 'M')
							{
								vector<int> temp3(3,0); temp3[0]=pid, temp3[1]=temp[0], temp3[2]=temp[1];
								if( selfCompartment.find(temp3)==selfCompartment.end() )
								{
								
									linkedNode1.insert(temp3);	
									linkedNode1.insert(starter);
									vector<vector<vector< int > > > ledge = _scene->_pathBubbles[pid]->searchLink(item[4], item[3], type, id, edgeSet);
									for(int k=0; k<ledge.size(); k++)
										 pbubble->linkedEdge[pid].insert(ledge[k]);
								}
							}
							else if(type == 'W')
							{
								//linkedNode1.insert(temp);	
							}
					   }//for linked node
					   linkedNode=linkedNode1;
				}	

				//4. ////// store linked
				 //for c link, p link, and pa link
				for(set<vector<int>>::iterator it=linkedNode.begin(); it!=linkedNode.end(); it++)
				{
					vector<int> node=*it;
					int pid=node[0], type=node[1], id=node[2];
					if(pid != pbubble->m_pathwayID)
					{
						_scene->_pathBubbles[pid]->ExtendLinkedNodesSize();					   
					}
					switch(type)
					{
						case 'P': _scene->_pathBubbles[pid]->linkedProtein[pid].insert(id);  break;
						case 'S': _scene->_pathBubbles[pid]->linkedSmallMolecule[pid].insert(id);  break;
						case 'C': _scene->_pathBubbles[pid]->linkedComplex[pid].insert(id);  break;
						case 'D': _scene->_pathBubbles[pid]->linkedDna[pid].insert(id);  break;
						case 'R': _scene->_pathBubbles[pid]->linkedReaction[pid].insert(id);  break;
						case 'E': _scene->_pathBubbles[pid]->linkedPhysicalEntity[pid].insert(id);  break;
						case 'L': _scene->_pathBubbles[pid]->linkedANode[pid].insert(id);  break;
						case 'M': _scene->_pathBubbles[pid]->linkedCompartment[pid].insert(id);  break;
					}		
				}
	        }//for(set<vector<int>>::iterator it=sID.begin(); it!=sID.end(); it++)
	  } //else if(_scene->linkSearchType == 0)			
	  /////////////////
   }//for mlist
}


int PathBubble1::whichCompartment(set<vector<int>> highlighted)
{
	set<int> Cid;
	for(set<vector<int>>::iterator it=highlighted.begin(); it!=highlighted.end(); it++)
	{
        vector<int> item= *it;
	    int type=item[0], id=item[1];
		int cid=whichCompartment(m_pathwayID, type, id);
		if(cid>=0)
			Cid.insert(cid);
	}    
	if(Cid.size()==1)
		return *Cid.begin();
	return -1;
}


vector<vector<vector<int>>> PathBubble1::searchLink(int stype, int sid, int etype, int eid, set<vector<vector<int> > > edgeSet)
{
	vector<vector<vector<int>>> ledge,_ledge;
	bool flag=false;
	vector<int> node1(3,0);
	node1[0]=m_pathwayID; node1[1]=stype, node1[2]=sid;
	if(stype!='W' && stype!='M')
	{
	   for(set<vector<vector<int> > >::iterator it=edgeSet.begin(); it!=edgeSet.end(); it++)
	   {
	       vector<vector<int>> edge=*it;
		   if( (edge[0][1]==etype && edge[0][2]==eid) || (edge[1][1]==etype && edge[1][2]==eid) )
		   {
		       ledge.push_back(edge);
		   }	   
		   else if( (edge[0][1]==stype && edge[0][2]==sid) || (edge[1][1]==stype && edge[1][2]==sid) )
		   {
		       ledge.push_back(edge);
		   }	   
	   }	
	   for(int i=0; i<ledge.size(); i++)
	   {
	       for(int j=0; j<ledge.size(); j++)
		   {   
			   if(ledge[i]!=ledge[j])
			   {
			         if( ledge[i][0]!=ledge[j][1] || ledge[i][1]!=ledge[j][0])
					 {
					     if( (ledge[i][0]==ledge[j][1] && ledge[i][0]!=node1) || (ledge[i][1]==ledge[j][0] && ledge[i][1]!=node1) 
							 || (ledge[i][0]==ledge[j][0] && ledge[i][0]!=node1) || (ledge[i][1]==ledge[j][1] && ledge[i][1]!=node1) )
						 {
							 
							 _ledge.push_back(ledge[i]);
							 _ledge.push_back(ledge[j]);
							 flag=true;
						 }					 
					 }			   
			   }
		   }
	   }	
	   if(!flag)
	   {
		   _ledge=ledge;	      
	   }
	}		
	else
	{
	   for(set<vector<vector<int> > >::iterator it=edgeSet.begin(); it!=edgeSet.end(); it++)
	   {
	       vector<vector<int>> edge=*it;
		   if( (edge[0][1]==etype && edge[0][2]==eid) || (edge[1][1]==etype && edge[1][2]==eid) )
		   {
		       _ledge.push_back(edge);
		   }	   
	   }	
	}
	return _ledge;
}

bool PathBubble1::compareSubgraph(vector<set<vector<vector<int>>>> edgeSet, vector<set<vector<int>>> nodeSet, set<vector<int>> &markedNode, vector<int> item0, vector<int> item1, int level)
{
	//if level==2,  markednode record the compartment id
	//if level==0 or 3 marked node record pathway id
	set<vector<vector<int>>> subEdge;
	set<vector<int>> subNode;

	for(int i=0; i<nodeSet.size(); i++)
	{
		//item0 are not connected to others
		if(nodeSet[i].find(item0)==nodeSet[i].end())
			return true;
		else
		{
	        subNode=nodeSet[i]; 
			subEdge=edgeSet[i];	
			break;
		}
	}	
	if(subEdge.empty()||subNode.empty())
		return true;

	if(level==1)//proten level
	{
	    return compareSubgraph(subEdge, subNode, markedNode, item0, item1, -1);
	}
	else if(level==2)//compartment level
	{
		//shave edgeset and nodeset to fit compartment
		//find item belong to which compartment
		//find item1 belong to which compartment
		
		bool flag=false;
		set<vector<vector<int>>> subEdge1,tedge;
	    set<vector<int>> subNode1,tnode;
		set<int> cids;
		
		//remove those not belonging to a single compartment
		int maxid=_scene->CompartmentContain[m_pathwayID].size()-1;
		for(set<vector<int>>::iterator it=subNode.begin(); it!=subNode.end(); it++)		
		{
				vector<int> temp=*it;
				int cid=whichCompartment(temp[0],temp[4],temp[3]);
				if(cid==maxid)
				    tnode.insert(temp);
		}
		for(set<vector<vector<int>>>::iterator it=subEdge.begin(); it!=subEdge.end(); it++)		
		{
				vector<vector<int>> temp=*it;
				vector<int> temph=temp[0],tempe=temp[1];
				int cidh=whichCompartment(temph[0],temph[4],temph[3]);
				int cide=whichCompartment(tempe[0],tempe[4],tempe[3]);
				if(cidh==maxid || cide==maxid)
					 tedge.insert(temp);
		}

		for(set<vector<int>>::iterator it=tnode.begin(); it!=tnode.end(); it++)		
		{
			     subNode.erase(*it);			
		}
		for(set<vector<vector<int>>>::iterator it=tedge.begin(); it!=tedge.end(); it++)		
		{
			     subEdge.erase(*it);			
		}

		queue<vector<int>> item1list;
		item1list.push(item1);
		while(!subNode.empty())
		{
			    int cid0 = whichCompartment(item0[0], item0[4], item0[3]);
				for(set<vector<int>>::iterator it=subNode.begin(); it!=subNode.end(); it++)		
				{
					vector<int> temp=*it;
					int cid=whichCompartment(temp[0],temp[4],temp[3]);
					if(cid==cid0)
					subNode1.insert(temp);
				}
				for(set<vector<vector<int>>>::iterator it=subEdge.begin(); it!=subEdge.end(); it++)		
				{
					vector<vector<int>> temp=*it;
					vector<int> temph=temp[0],tempe=temp[0];
					int cidh=whichCompartment(temph[0],temph[4],temph[3]);
					int cide=whichCompartment(tempe[0],tempe[4],tempe[3]);
					if(cidh==cid0 && cide==cid0)
						  subEdge1.insert(temp);
				}
				set<vector<int>> markedNode1;

			while(!item1list.empty())
			{
				item1=item1list.front(); item1list.pop();
				
				int cid1 = whichCompartment(item1[0], item1[4], item1[3]);

				if(compareSubgraph(subEdge1, subNode1,  markedNode1, item0, item1, cid1)) //cid= item1
				{
					cids.insert(cid1);
					flag=true;
				}

				

				for(set<vector<int>>::iterator it=subNode1.begin(); it!=subNode1.end(); it++)		
				{
					 subNode.erase(*it);			
				}
				for(set<vector<vector<int>>>::iterator it=subEdge1.begin(); it!=subEdge1.end(); it++)		
				{
					 subEdge.erase(*it);			
				}
				for(set<vector<int>>::iterator it=markedNode1.begin(); it!=markedNode1.end(); it++)		
				{
					 markedNode.insert(*it);			
				}				
			}
			
			if(!subNode.empty())
			{
				item0= *subNode.begin();
				//find item1
				//item1= *subNode.begin();	
				item1list=findSameItem(m_pathwayID, item0);				
			}
		}
		markedNode.clear();
		for(set<int>::iterator it=cids.begin(); it!=cids.end(); it++)		
		{
			 int cid=*it;
			 vector<int> temp(1,cid);			 
			 markedNode.insert(temp);
		}
		return flag;
	}
	else if(level==3)//compartment level
	{
		//shave edgeset and nodeset to fit compartment
	    return compareSubgraph(subEdge, subNode, markedNode, item0, item1, -1);
	}
	return true;
}

queue<vector<int>> PathBubble1::findSameItem(int pid1, vector<int> item0)
{
	int pid0=item0[0], type0=item0[4], id0=item0[3]; 
	queue<vector<int>> temp;
   //for(int m=0; m<6; m++)
	int id1= _scene->_pathBubbles[pid1]->dataID;
   {
	   vector<vector<QString>> node;
	  int type1=type0;
      switch(type0)
	  {
	         case 'P': node=_scene->_proteinNameD[id1];         break;
			 case 'S': node=_scene->_smallMoleculeNameD[id1];    break;
			 case 'C': node=_scene->_complexNameD[id1];    break;
			 case 'D': node=_scene->_DnaNameD[id1];    break;
			 case 'R': node=_scene->_reactionNameD[id1];    break;
			 case 'E': node=_scene->_physicalEntityNameD[id1];    break;
	  }
	  for(int id1=0; id1<node.size(); id1++)
	  {
		 vector<int> item=item0;
		 item[0]=pid1;
		 item[3]=id1;
		 if(isSameNode(pid0, type0, id0, pid1, type1, id1))
	          temp.push(item);
	 }      
   }
   return temp;
}




bool PathBubble1::compareSubgraph(set<vector<vector<int>>>subEdge, set<vector<int>> subNode, set<vector<int>> &markedNode, vector<int> item0, vector<int> item1, int cid)
{//if same: return true
	//item1, the node on selected graph, item1, the node for futher search
	//set<int> markedEdge, b_markedEdge;
	//if(cid!=-1) require to compare the item1's compartment's id, cid, as well
	vector<int> node1(2,0),node2(2,0);	
	
	if(subEdge.empty()||subNode.empty())
		return true;
	//if marked
	if(markedNode.find(item0)!=markedNode.end())
		return true;

	markedNode.insert(item0);
	
	int Pid1=item1[0], pid1=item1[1], cid1=item1[2], id1=item1[3], type1=item1[4];
	int Pid0=item0[0], pid0=item0[1], cid0=item0[2], id0=item0[3], type0=item0[4];

	
	node1[0]=item1[4]; node1[1]=item1[3];
	//searchSubgraph( subNode, subEdge, item1, pflag, cflag, wflag);
	//martchSubgraph( set<vector<int>> subNode, set<vector<vector<int>>> subEdge, vector<int> item1)
	
	//mark item1
		
	 vector<vector<int>> edgeRecord; 	
		
	    switch(type1)
		{
			 case 'P': edgeRecord = _protein[id1]; break;
			 case 'L': edgeRecord = _ANode[id1]; break;
			 case 'S': edgeRecord = _smallMolecule[id1]; break;
			 case 'C': edgeRecord = _complex[id1]; break;
			 case 'D': edgeRecord = _Dna[id1]; break;
			 case 'R': edgeRecord = _reaction[id1]; break;
			 case 'E': edgeRecord = _entity[id1]; break;			 
		}
        bool flag1=false;
		//if find all same connected nodes

		for(set<vector<vector<int>>>::iterator it=subEdge.begin(); it!=subEdge.end(); it++)		
	    {
			int d, Pid2, type2, id2;
			vector<vector<int>> temp=*it;	
			vector<int> tempx;
			bool inRealSet=false;
			for(int i=0; i<2; i++)
			{
			    if(temp[i][3]==item0[3] && temp[i][4]==item0[4])
				{
				   d=1-i;		
				   Pid2=temp[d][0], type2=temp[d][4], id2=temp[d][3];
				   tempx.push_back(temp[d][0]); tempx.push_back(temp[d][1]); tempx.push_back(temp[d][2]); 
				   tempx.push_back(temp[d][3]); tempx.push_back(temp[d][4]);	
				   inRealSet=true;
				   d=i==0?1:-1;		
				   break;
				}
			}
			//a right node found
			if(markedNode.find(tempx)!=markedNode.end() || !inRealSet)	
				continue;

			bool flag=false;
			for(int k=0; k<edgeRecord.size(); k++)		
			{
		 		 vector<int> node=edgeRecord[k];
	             node2[0]=node[0], node2[1]=node[1];
				 //if node 1 == node 2
				  if(node[3]==d && isSameNode(Pid2, type2, id2, Pid1, node2[0], node2[1]))
			     {  
					 vector<int> item00, item11;
		            
					 if(tempx!=item0)
					 {
							   item11=tempx; item11[0]=Pid1, item11[3]=node2[1];
							   if(compareSubgraph(subEdge, subNode, markedNode, tempx, item11, cid)==false)
				                  return false;
					 }
					 flag=true;		
				 }					 
			}
			if(!flag)
				return false;			
			
		}	
		
		return true;		
}


bool PathBubble1::isSameNode(int pid1, int type1, int id1, int pid2, int type2, int id2)
{
	
	if(type1!=type2 || id1<0 || id2<0)
		return false;
	vector<QString> Name1, Name2;
	int did1=_scene->_pathBubbles[pid1]->dataID, did2=_scene->_pathBubbles[pid2]->dataID;
	switch(type1)
	{
		case 'C': Name1=_scene->_complexNameD[did1][id1];         Name2=_scene->_complexNameD[did2][id2];            break;
		case 'D': Name1=_scene->_DnaNameD[did1][id1];             Name2=_scene->_DnaNameD[did2][id2];                break;
		case 'E': Name1=_scene->_physicalEntityNameD[did1][id1];  Name2=_scene->_physicalEntityNameD[did2][id2];     break;
	    case 'P': Name1=_scene->_proteinNameD[did1][id1];         Name2=_scene->_proteinNameD[did2][id2];            break;
		case 'S': Name1=_scene->_smallMoleculeNameD[did1][id1];   Name2=_scene->_smallMoleculeNameD[did2][id2];      break;
		case 'R': Name1=_scene->_reactionNameD[did1][id1];        Name2=_scene->_reactionNameD[did2][id2];           break;
		case 'L': Name1=_scene->_ANodeName[pid1][id1];            Name2=_scene->_ANodeName[pid2][id2];      break;		
		case 'M': return (_scene->_compartmentName[pid1][id1] == _scene->_compartmentName[pid2][id2]); 
		case 'W': return (pid1==pid2||_scene->_pathBubbles[pid1]->_curFileName==_scene->_pathBubbles[pid2]->_curFileName);  
	}
	if(Name1.size()<5) Name1.resize(5);   if(Name2.size()<5) Name2.resize(5);
	if(completeSame)
	{
		//if( type1 == 'R')
	    //	   return(isSameReaction(pid1, id1, pid2, id2)); 
		//else
		{
			if(Name1==Name2)
			    return true;
			else return false;
		}
	}
	else
	{
		if(type1 == 'R' || type1 == 'L')
		{
		    if(Name1[1]==Name2[1])
		         return true;
	        else return false;
		}
		else
		{	
			if(Name1[0]==Name2[0])
		      return true;
	        else return false;
		}
	}
}

void PathBubble1::searchSubgraph( set<vector<int>> &sID, vector<set<vector<vector<int>>>> &eSet, vector<set<vector<int>>> &nodeSet)
{
	//preprocess to move some connected nodes to another list and just keep the the one of a representative of the sub-graph	
	//get edge pairs vector<vector<vector<<int>>> //Pid,type,id, mark
	//only for node now
	//vector<vector<vector<int>>> edgeSet;
	set<vector<vector<int>>> tEdgeSet;
	vector<vector<vector<int>>> edgeSet;
	//vector<set<vector<int>>> nodeSet;
	for(set<vector<int>>::iterator it=sID.begin(); it!=sID.end(); it++)		
	{	
		vector<int> item=*it;
		int Pid=item[0], pid=item[1], cid=item[2], id=item[3], type=item[4], cid1=-1;
		item.push_back(0);
		//check connection
        vector<vector<int>> edgeRecord; 		
		vector<int> node;						

		switch(type)
	    {
		     case 'P':  edgeRecord=_scene->_pathBubbles[Pid]->_protein[id];           break;
			 case 'L':  edgeRecord=_scene->_pathBubbles[Pid]->_ANode[id];           break;
		     case 'S':  edgeRecord=_scene->_pathBubbles[Pid]->_smallMolecule[id];     break;
		     case 'C':  edgeRecord=_scene->_pathBubbles[Pid]->_complex[id];           break;
			 case 'D':  edgeRecord=_scene->_pathBubbles[Pid]->_Dna[id];               break;
			 case 'R':  edgeRecord=_scene->_pathBubbles[Pid]->_reaction[id];          break;
			 case 'E':  edgeRecord=_scene->_pathBubbles[Pid]->_entity[id];            break;
	    }	

		for(int k=0; k<edgeRecord.size(); k++)		
		{
		 	 node=edgeRecord[k];
			 for(set<vector<int>>::iterator it=sID.begin(); it!=sID.end(); it++)		
			 {
			       //if match && it is not in the edgepairs (directed edge) 
			 	   vector<int> temp=*it;
				   temp.push_back(0);
				   if(temp[0]==Pid && temp[4]==node[0] && temp[3]==node[1])
				   {  //record edge pair
					   vector<vector<int>> aEdge;
					   if(node[3]>0)
					   {
						    aEdge.push_back(item); aEdge.push_back(temp);
					   }
					   else
					   {
					        aEdge.push_back(temp); aEdge.push_back(item);
					   }
					   tEdgeSet.insert(aEdge);
				   }					 
			}
		}			
	}
	for(set<vector<vector<int>>>::iterator it=tEdgeSet.begin(); it!=tEdgeSet.end(); it++)		
	{
	   vector<vector<int>> temp=*it;	   
	   edgeSet.push_back(temp);	
	}
	//build subgraph through edge pairs
	//graph set: node set + edge set
	//for (edge pair)
	set<int> markedEdge, b_markedEdge;
	for(int i=0; i<edgeSet.size(); i++)
	{		
		set<vector<int>> tNodeSet;
		for(int j=0; j<2; j++)		
		{
			//if(edge is not makred)	
            if(edgeSet[i][j][5]==0)
			{
				//mark it
				vector<int> mark=edgeSet[i][j];
				b_markedEdge=markedEdge;
				markedEdge.insert(i);
				for(int k=0; k<edgeSet.size(); k++)
	            {		
		            for(int l=0; l<2; l++)			
				    {
						if(edgeSet[k][l]==mark)
						   edgeSet[k][l][5]=1; 
					}
				}				
				marchSubGraph(edgeSet, tNodeSet, markedEdge, i, j);
				nodeSet.push_back(tNodeSet);	
				set<vector<vector<int>>> Ed;
				for(set<int>::iterator it= markedEdge.begin(); it!=markedEdge.end(); it++)	
				{
					
				    if(b_markedEdge.find(*it)==b_markedEdge.end())
					{
						Ed.insert(edgeSet[*it]);
					}    					
				}
				eSet.push_back(Ed);
				// only keep one in sID
				set<vector<int>> rmList;
				vector<int> temp1;
				temp1.push_back(mark[0]); temp1.push_back(mark[1]); temp1.push_back(mark[2]); temp1.push_back(mark[3]); temp1.push_back(mark[4]);				   
				for(set<vector<int>>::iterator it=sID.begin(); it!=sID.end(); it++)		
				{
				    //remove others
					vector<int> temp=*it;					
					//temp.push_back(1);										
					if(temp!=temp1)
					{
						if(tNodeSet.find(temp)!=tNodeSet.end())
					       rmList.insert(temp);					
					}
				}
				for(set<vector<int>>::iterator it=rmList.begin(); it!=rmList.end(); it++)	
				{
				   vector<int> temp=*it;
				   vector<int> temp1;
				   temp1.push_back(temp[0]); temp1.push_back(temp[1]); temp1.push_back(temp[2]); temp1.push_back(temp[3]); temp1.push_back(temp[4]);
				   sID.erase(temp1);
				}
			}
		}
	}
}

void PathBubble1::marchSubGraph(vector<vector<vector<int>>> &edgeSet, set<vector<int>> &nodeSet, set<int> &markedEdge, int eid, int id)
{//id: 0, 1, tell where the node is in edge

	  vector<int> temp;
	  temp.push_back(edgeSet[eid][id][0]); temp.push_back(edgeSet[eid][id][1]); temp.push_back(edgeSet[eid][id][2]); temp.push_back(edgeSet[eid][id][3]); temp.push_back(edgeSet[eid][id][4]);
	  nodeSet.insert(temp);
      
	  //if node is marked 
	  int id2=1-id;
	  if(edgeSet[eid][id2][5]==0||markedEdge.find(eid)==markedEdge.end())
	  {
		  //edgeSet[eid][id2].push_back(1); 
		  vector<int> mark=edgeSet[eid][id2];		  
		  for(int k=0; k<edgeSet.size(); k++)
	      {		
		       for(int l=0; l<2; l++)			
			   {
			        if(edgeSet[k][l]==mark)
				         edgeSet[k][l][5]=1; 
			   }
		  }		  
	      marchSubGraph(edgeSet, nodeSet, markedEdge, eid, id2);
	  }

	  for(int i=0; i<edgeSet.size(); i++)
	  {
		   for(int j=0; j<2; j++)		
		   {
			   //if(edge is not marked)	
			   if(edgeSet[i][j] == edgeSet[eid][id])
			   {
				    if(edgeSet[i][1-j][5]==0||markedEdge.find(i)==markedEdge.end())
				    {
					   //mark it
					   vector<int> mark=edgeSet[i][1-j];	
					   markedEdge.insert(i);
					   for(int k=0; k<edgeSet.size(); k++)
	                   {		
		                  for(int l=0; l<2; l++)			
				          {
						      if(edgeSet[k][l]==mark)
						         edgeSet[k][l][5]=1; 
					      }
				       }
			           marchSubGraph(edgeSet, nodeSet, markedEdge, i, 1-j);
					}
			   }
		   }
		   //record it by adding a new node and edge
		   //start a recurrence searching
	  }
}			

int PathBubble1::whichCompartment(int pid, int type, int id)
{
	 vector<int> item;
	 item.push_back(type); item.push_back(id);
	  for(int k=0; k<_scene->CompartmentContain[pid].size(); k++)
	  {
		   if (_scene->CompartmentContain[pid][k].find(item)!=_scene->CompartmentContain[pid][k].end())
		   {
			   return k;
		   }						   
	  }
	  return -1;
}


int PathBubble1::whichCompartment(int pid, vector<int> item)
{
	  for(int k=0; k<_scene->CompartmentContain[pid].size(); k++)
	  {
		   if (_scene->CompartmentContain[pid][k].find(item)!=_scene->CompartmentContain[pid][k].end())
		   {
			   return k;
		   }						   
	  }
	  return -1;
}

void PathBubble1::searchSharedPathway(int ctype, vector<vector<vector<QString>>> nodeName, vector<vector<QRectF>> nodePos, set<vector<int>> sID, vector<set<vector<vector<int>>>> edgeSet, vector<set<vector<int>>> nodeSet, vector<set<vector<int>>> &cItem, vector<set<vector<int>>> &markedNodes,  vector<set<int>> &sharedNode)
{
    for(int j=0; j<1; j++)
	{
		bool isShared=false;
		//if(smallMoleculePos[j].x()>-999.99)
        //search all nodes contained in this pathway (share same "Pid", "pid")// to ensure the 'and' condition
		vector<vector<int>> count;
		for(set<vector<int>>::iterator it=sID.begin(); it!=sID.end(); it++)		
		{	
				vector<int> item=*it;
				vector<int> unit(4,0);
				int Pid=item[0], pid=item[1], cid=item[2], id=item[3], type=item[4],cid1=-1;
				bool found=false;
				for(int i=0; i<count.size(); i++)
				{					
				   if(count[i][0]==Pid && count[i][1]==pid )
				   {
					   count[i][3]++;
					   found=true; break;
				   }
				}
				if(!found && (pid ==j || pid ==-1))
				{
					unit[0]=Pid, unit[1]=pid, unit[2]=cid; unit[3]=1;
					count.push_back(unit);		
				}
		}
		//count: record how many item (in sID) contained in pathway Pid

		{			
      		for(set<vector<int>>::iterator it=sID.begin(); it!=sID.end(); it++)		
			{	
				vector<int> item=*it;
				int Pid=item[0], pid=item[1], cid=item[2], id=item[3], type=item[4],cid1=-1;
				int Did=_scene->_pathBubbles[Pid]->dataID;
				//for(int k=0; k<_scene->_smallMoleculeName[m_pathwayID].size(); k++)						
				if(type!='W'&& type!='M') //node to compartment
				{
					vector<QString> iName;
					switch(type)
			        {
						case 'C': iName=_scene->_complexNameD[Did][id];  break;
						case 'D': iName=_scene->_DnaNameD[Did][id];      break;
						case 'E': iName=_scene->_physicalEntityNameD[Did][id];      break;
						case 'P': iName=_scene->_proteinNameD[Did][id];      break;
						case 'S': iName=_scene->_smallMoleculeNameD[Did][id];      break;
						case 'R': iName=_scene->_reactionNameD[Did][id];      break;
						case 'L': iName=_scene->_ANodeName[Pid][id];      break;
					}
					if(iName.size()<5) iName.resize(5);
				    if(pid==-1||pid==m_pathwayID) //pathway level
					{
						   bool flag=false;
						   if(cid!=-1)//cid node level
				           { 								
						       
							  for(set<vector<int>>::iterator it=_scene->CompartmentContain[m_pathwayID][cid].begin(); it!=_scene->CompartmentContain[m_pathwayID][cid].end(); it++)
							  {
							      //if(s_scene->CompartmentContain[m_pathwayID][j].find(item)!=_scene->CompartmentContain[m_pathwayID][j].end())
								  vector<int> titem=*it;
                                  int type1=titem[0],id1=titem[1];
                                  vector<QString> Name1;
							      switch(type1)
							      {	
					                  case 'C': if(_scene->_complexPos[m_pathwayID][id1].x()>-999.99)			Name1=_scene->_complexNameD[dataID][id1];  break;
						              case 'D': if(_scene->_DnaPos[m_pathwayID][id1].x()>-999.99)				Name1=_scene->_DnaNameD[dataID][id1];      break;
						              case 'E': if(_scene->_physicalEntityPos[m_pathwayID][id1].x()>-999.99)	Name1=_scene->_physicalEntityNameD[dataID][id1];  break;
						              case 'P': if(_scene->_proteinPos[m_pathwayID][id1].x()>-999.99)			Name1=_scene->_proteinNameD[dataID][id1];  break;
						              case 'S': if(_scene->_smallMoleculePos[m_pathwayID][id1].x()>-999.99)  	Name1=_scene->_smallMoleculeNameD[dataID][id1];   break;
						              case 'R': if(_scene->_reactionPos[m_pathwayID][id1].x()>-999.99)		    Name1=_scene->_reactionNameD[dataID][id1];   break;
									  case 'L': if(_scene->_ANodePos[m_pathwayID][id1].x()>-999.99)		        Name1=_scene->_ANodeName[m_pathwayID][id1];   break;
					             }
								 if(Name1.size()<5) Name1.resize(5);

							  	 //if(iName == Name1)
								 if(type==type1)
								//	 if( nodePos[m_pathwayID][j].x()>-999.99 && 
								if( ( (type!='R'&&type!='L') && ( (completeSame && iName == Name1) || (!completeSame && iName[0] == Name1[0]) ) ) 
								  || ( (type=='R'||type=='L') && ( ( completeSame && iName == Name1) || ( !completeSame && iName[1] == Name1[1]) ) ) )
								  // && isSameReaction(Pid, id, m_pathwayID, j)) ) //node level 				   
								   //if( ( type1!='R' && iName == Name1 ) || (type1=='R' && Name1.size()>=1 && iName[0] == Name1[0] && isSameReaction(Pid, id, m_pathwayID, id1) )  )
							     {
									 for(int i=0; i<count.size(); i++)
									 {					
											if(count[i][0]==Pid && count[i][1]==pid )
				                            {
												vector<int> item1=item;
					                            item1[0]=m_pathwayID;  item1[3]=id1;   		
					                            set<vector<int>> markedNode;								   
					                            if(compareSubgraph(edgeSet, nodeSet, markedNode, item, item1, 3))		
												{
					                              count[i][3]--;
												  flag=true;
					                              break;
												}
				                             }
				                     }								    
							     }	
								 if(flag) break;
							  }
							  
						   }	
						   if(cid==-1)
						   {
						   
							   for(int l=0;l<_scene->CompartmentContain[m_pathwayID].size(); l++)
							   {
								   int count1=0;
							   for(set<vector<int>>::iterator it=_scene->CompartmentContain[m_pathwayID][l].begin(); it!=_scene->CompartmentContain[m_pathwayID][l].end(); it++)
							  {
							      //if(s_scene->CompartmentContain[m_pathwayID][j].find(item)!=_scene->CompartmentContain[m_pathwayID][j].end())
								  vector<int> titem=*it;
								 
                                  int type1=titem[0],id1=titem[1];
                                  vector<QString> Name1;
								  
							      switch(type1)
							      {	
					                  case 'C': if(_scene->_complexPos[m_pathwayID][id1].x()>-999.99)			Name1=_scene->_complexNameD[dataID][id1];  break;
						              case 'D': if(_scene->_DnaPos[m_pathwayID][id1].x()>-999.99)				Name1=_scene->_DnaNameD[dataID][id1];      break;
						              case 'E': if(_scene->_physicalEntityPos[m_pathwayID][id1].x()>-999.99)	Name1=_scene->_physicalEntityNameD[dataID][id1];  break;									  
						              case 'P': if(_scene->_proteinPos[m_pathwayID][id1].x()>-999.99)			Name1=_scene->_proteinNameD[dataID][id1];  break;
									  case 'L': if(_scene->_ANodePos[m_pathwayID][id1].x()>-999.99)		    Name1=_scene->_ANodeName[m_pathwayID][id1];   break;
						              case 'S': if(_scene->_smallMoleculePos[m_pathwayID][id1].x()>-999.99)	Name1=_scene->_smallMoleculeNameD[dataID][id1];   break;
						              case 'R': if(_scene->_reactionPos[m_pathwayID][id1].x()>-999.99)		Name1=_scene->_reactionNameD[dataID][id1];   break;
									  
					             }
								  if(Name1.size()<5) Name1.resize(5);
							  	 //if(iName == Name1)
								 if(type==type1)
								 //if( ( type1!='R' && iName == Name1 ) || (type1=='R' && Name1.size()>=1 && iName[0] == Name1[0] && isSameReaction(Pid, id, m_pathwayID, id1) )  )
								 if( ( (type!='R'&&type!='L') && ( (completeSame && iName == Name1) || (!completeSame && iName[0] == Name1[0]) ) ) 
								  || ( (type=='R'||type=='L') && ( ( completeSame && iName == Name1) || ( !completeSame && iName[1] == Name1[1]) ) ) )
							     {
									 for(int i=0; i<count.size(); i++)
									 {					
											if(count[i][0]==Pid && count[i][1]==pid )
				                            {
												vector<int> item1=item;
					                            item1[0]=m_pathwayID;  item1[3]=id1;   		
					                            set<vector<int>> markedNode;								   
					                            if(compareSubgraph(edgeSet, nodeSet, markedNode, item, item1, 3))								  
					                            {
						                                //markedNodes.push_back(markedNode);
														count[i][3]--;
												        flag=true;
					                                    break;
												}                     
											}
				                     }								    
							     }	
								 if(flag) break;
							  }
							    if(flag) break;
							   }						   
						   }
					}				  				   
			   }
			    else if(type=='M') 
				{
					vector<QString> iName;
					iName=_scene->_compartmentName[Pid][cid];  
					if(_scene->_compartmentPos[m_pathwayID][cid].x()>-999.99) //node level 
				    {
                       if(pid==-1||pid==m_pathwayID) //pathway level
					   {
						   //if(cid==-1)//cid compartment level
				           { 
							   for(int i=0; i<count.size(); i++)
							  {					
									if(count[i][0]==Pid && count[i][1]==pid )
				                     {
					                         count[i][3]--;
					                          break;
				                     }
				               }
						       //sharedNode[m_pathwayID].insert(j);				  								
						   }						  						  
					   }				  
				   }
			   }
				else if(type=='W') //pathway to pathway			    
				{										
					if(Pid!=m_pathwayID) //pathway level
					{
						   //cid compartment level
				           { 
							  for(int i=0; i<count.size(); i++)
							  {					
									 //if(count[i][0]==dataID && count[i][1]==dataID )//&& count[i][2]==cid)
								     if(count[i][0]==count[i][1])
									 if(_scene->_pathBubbles[Pid]->_pathName == _scene->_pathBubbles[m_pathwayID]->_pathName)
								     //if(count[i][0]==dataID && count[i][1]==dataID )//&& count[i][2]==cid)
				                     {
					                         count[i][3]=0;
					                          break;
				                     }
				               }
						       //sharedNode[m_pathwayID].insert(j);				  								
						   }						  						  
					}	
			   }//else if(type=='M')
               
                for(int i=0; i<count.size(); i++)
				{					
				   if(count[i][3]<=0)
				   {
					   isShared=true;  					
					   sharedNode[m_pathwayID].insert(m_pathwayID);	
					   break;
				   }
				}				
			}	
		}	
	}
}


void PathBubble1::searchSharedNode(int ctype, vector<vector<vector<QString>>> nodeName, vector<vector<QRectF>> nodePos, set<vector<int>> sID, vector<set<vector<vector<int>>>> edgeSet, vector<set<vector<int>>> nodeSet, vector<set<vector<int>>> &cItem, vector<set<vector<int>>> &markedNodes,  vector<set<int>> &sharedNode, bool toSharedNode)
{   //search shared node (protein, complex, etc. not pathway, not compartment)
	bool tflag=false;
	int pdID;
	if(ctype=='L')
		pdID=m_pathwayID;
	else pdID=dataID;
    for(int j=0; j<nodeName[pdID].size(); j++)
	{
		    bool isShared=false; int ccount=0;					
      		for(set<vector<int>>::iterator it=sID.begin(); it!=sID.end(); it++)		
			{	
				vector<int> item=*it;
				int Pid=item[0], pid=item[1], cid=item[2], id=item[3], type=item[4], cid1=-1;
				int Did=_scene->_pathBubbles[Pid]->dataID;
				if(ctype==type||type=='M'||ctype=='M'||ctype=='W'||type=='W')
					tflag=true; 
				if(type!='W'&& type!='M' ) //node to node
				{
					vector<QString> iName;
					switch(type)
			        {
						case 'C': iName=_scene->_complexNameD[Did][id];  break;
						case 'D': iName=_scene->_DnaNameD[Did][id];      break;
						case 'E': iName=_scene->_physicalEntityNameD[Did][id];      break;						
						case 'P': iName=_scene->_proteinNameD[Did][id];      break;
						case 'S': iName=_scene->_smallMoleculeNameD[Did][id];      break;
						case 'R': iName=_scene->_reactionNameD[Did][id];      break;
						case 'L': iName=_scene->_ANodeName[Pid][id];      break;
					}
					if(iName.size()<5) iName.resize(5);   

					if(ctype==type)
				    if( nodePos[m_pathwayID][j].x()>-999.99 && 
					( ( (type!='R'&&type!='L') && ( (completeSame && iName == nodeName[pdID][j]) || (!completeSame && iName[0] == nodeName[pdID][j][0]) ) ) 
					  || ( (type=='R'||type=='L') && ( ( completeSame && iName == nodeName[pdID][j]) || ( !completeSame && iName[1] == nodeName[pdID][j][1]) ) )					  
				     ) )// && isSameReaction(Pid, id, m_pathwayID, j)) ) //node level 
				    {
                       if(pid==-1||pid==m_pathwayID) //pathway level
					   {
						   if(cid!=-1)
						   {
						       vector<int> item(2,0);
							   item[0]=ctype, item[1]=j;        
							   cid1=whichCompartment(m_pathwayID,item);
						   }
					       if(cid==-1||cid==cid1) //cid compartment level
				           { 
						       //if(Andflag)
							   {   
								   vector<int> item1=item;
								   item1[0]=m_pathwayID;  item1[3]=j;   		
								   set<vector<int>> markedNode;								 
								   if(compareSubgraph(edgeSet, nodeSet, markedNode, item, item1, 1))								  
								   {
									   markedNodes.push_back(markedNode);                                       
									   sharedNode[m_pathwayID].insert(j);
								   }
							   }
							   isShared=true;  
							   break;		
						   }						  						  
					   }				  
				   }
			   }
			    else if(type=='M') //compartment to node			    
				{
					//work on compartment that do not contain selected nodes
					//vector<QString> iName;
					//iName=_scene->_compartmentName[Pid][cid];  					
					vector<int> item(2,0);
					if(nodePos[m_pathwayID][j].x()>-999.99) //node level 
				    {
                       if(pid==-1||pid==m_pathwayID)//pathway level
					   {
						   if(cid!=-1)
						   {  
							  item[0]=ctype, item[1]=j;                      							
							  cid1=whichCompartment(m_pathwayID,item);
						   }
					       if(cid==cid1) //cid compartment level //cid==-1||
				           { 
							    //sharedNode[m_pathwayID].insert(j);		                                   
								item[0]=ctype, item[1]=j; 
								vector<int> nItem=item; nItem.push_back(m_pathwayID);
								if(m_pathwayID==Pid)
								{
									cItem[ccount].insert(nItem);
									//if(toSharedNode)
									//	sharedNode[m_pathwayID].insert(j);
									isShared=true;  break;
								}
						   }						  						  
					   }				  
				   }
					ccount++;				   
			    }
				else if(type=='W') //pathway to node			    
				{	
					if(nodePos[m_pathwayID][j].x()>-999.99) //node level 
				    {
					   if(pid==m_pathwayID||_curFileName==_scene->_pathBubbles[Pid]->_curFileName) //pathway level
					   {
							vector<int> item;
							item.push_back(ctype), item.push_back(j);
							vector<int> nItem=item; nItem.push_back(m_pathwayID);
							if(m_pathwayID==Pid)
							{
								cItem[ccount].insert(nItem);	
								//if(toSharedNode)
								//	sharedNode[m_pathwayID].insert(j);
								isShared=true; break;	
							}						   						   	
					   }				  
				   }
				  ccount++;
			   }
			}			
	}

}



void PathBubble1::searchDifferedNode(int ctype, vector<vector<vector<QString>>> nodeName, vector<vector<QRectF>> nodePos, set<vector<int>> sID, vector<set<vector<vector<int>>>> edgeSet, vector<set<vector<int>>> nodeSet, vector<set<vector<int>>> &cItem, vector<set<vector<int>>> &markedNodes,  vector<set<int>> &differedNode)
{   //search shared node (protein, complex, etc. not pathway, not compartment)
	//search same level differend
	bool tflag=false;
	
    for(int j=0; j<nodeName[dataID].size(); j++)
	{
		    bool isDIffered=false; int ccount=0;	
			if(cItem.size()<sID.size())
		       cItem.resize(sID.size());
      		for(set<vector<int>>::iterator it=sID.begin(); it!=sID.end(); it++)		
			{	
				vector<int> item=*it;
				int Pid=item[0], pid=item[1], cid=item[2], id=item[3], type=item[4], cid1=-1;
				int Did=_scene->_pathBubbles[Pid]->dataID;
				if(ctype!=type)
					continue; 
				if(type!='W'&& type!='M') //node to node
				{
					vector<QString> iName;
					switch(type)
			        {
						case 'C': iName=_scene->_complexNameD[Did][id];  break;
						case 'D': iName=_scene->_DnaNameD[Did][id];      break;
						case 'E': iName=_scene->_physicalEntityNameD[Did][id];      break;
						case 'L': iName=_scene->_ANodeName[Pid][id];      break;
						case 'P': iName=_scene->_proteinNameD[Did][id];      break;
						case 'S': iName=_scene->_smallMoleculeNameD[Did][id];      break;
						case 'R': iName=_scene->_reactionNameD[Did][id];      break;
					}
				    if(iName.size()<5) iName.resize(5); 
				       if(nodePos[m_pathwayID][j].x()>-999.99)
                       if(pid==-1||pid==m_pathwayID) //pathway level
					   {
						   if(cid!=-1)
						   {
						       vector<int> item(2,0);
							   item[0]=ctype, item[1]=j;        
							   cid1=whichCompartment(m_pathwayID,item);
						   }
					       if(cid==-1||cid==cid1) //cid compartment level
				           { 
						       //if(Andflag)
							   //{   
								   //vector<int> item1=item;
								   //set<vector<int>> markedNode;
							       vector<QString > Name1 = nodeName[m_pathwayID][j];
								   if(Name1.size()<5) Name1.resize(5);   
							       if(! (
									( (type!='R'&&type!='L') && ( (completeSame && iName == Name1) || (!completeSame && iName[0] == Name1[0]) ) ) 
								  || ( (type=='R'||type=='L') && ( ( completeSame && iName == Name1) || ( !completeSame && iName[1] == Name1[1]) ) )) )							      
							       //if( ! (( (type!='R' && iName == nodeName[m_pathwayID][j]) || (type=='R' && iName[0] == nodeName[m_pathwayID][j][0]) && isSameReaction(Pid, id, m_pathwayID, j))) ) //node level 
								   //if(iName!=nodeName[dataID][j] )//!compareSubgraph(edgeSet, nodeSet, markedNode, item, item1, 1))								  
								   {
									   //markedNodes.push_back(markedNode);  
									   vector<int> nItem(3,0);
									   nItem[0]=ctype, nItem[1]=j,  nItem[2]=m_pathwayID; 
									   //cItem[ccount].insert(nItem);								  
									   //differedNode[m_pathwayID].insert(j);
									   //Pid=item[0], pid=item[1], cid=item[2], id=item[3], type=item[4];
									   cItem[ccount].insert(nItem);	
									   isDIffered=true;  
								   }			
								   ccount++;
							       	
						   }						  						  
					   }				  
				   
			   }
			    else if(type=='M') //compartment to node			    
				{
					//work on compartment that do not contain selected nodes
					//vector<QString> iName;
					//iName=_scene->_compartmentName[Pid][cid];  					
					vector<int> item(2,0);
					if(nodePos[m_pathwayID][j].x()>-999.99) //node level 
				    {
                       if(pid==-1||pid==m_pathwayID)//pathway level
					   {
						   if(cid!=-1)
						   {  
							  item[0]=ctype, item[1]=j;                      							
							  cid1=whichCompartment(m_pathwayID,item);
						   }
					       if(cid!=cid1) //cid compartment level //cid==-1||
				           { 
							    //differedNode[m_pathwayID].insert(j);		                                   
								item[0]=ctype, item[1]=j; 
								vector<int> nItem=item; nItem.push_back(m_pathwayID);
								if(m_pathwayID==Pid)
								{
									cItem[ccount].insert(nItem);								   
									isDIffered=true;  
								}
						   }						  						  
					   }				  
				   }
					
					ccount++;				   
			    }
				else if(type=='W') //pathway to node			    
				{	
					if(nodePos[m_pathwayID][j].x()>-999.99) //node level 
				    {
					   if(pid!=m_pathwayID && _curFileName!=_scene->_pathBubbles[Pid]->_curFileName) //pathway level
					   {
							vector<int> item;
							item.push_back(ctype), item.push_back(j);
							vector<int> nItem=item; nItem.push_back(m_pathwayID);
							//if(m_pathwayID!=Pid)
							{
								cItem[ccount].insert(nItem);		
								isDIffered=true; 
							}						   						   	
					   }				  
				   }
				  ccount++;
			   }
			}			
	}

}


void PathBubble1::searchSharedCompartment(int ctype, vector<vector<vector<QString>>> nodeName, vector<vector<QRectF>> nodePos, set<vector<int>> sID, vector<set<vector<vector<int>>>> edgeSet, vector<set<vector<int>>> nodeSet, vector<set<vector<int>>> &cItem, vector<set<vector<int>>> &markedNodes,  vector<set<int>> &sharedNode)
{
	int Did=_scene->_pathBubbles[m_pathwayID]->dataID;

    for(int j=0; j<nodeName[Did].size()-1; j++)
	{
		bool isShared=false; int ccount=0;
		//if(smallMoleculePos[j].x()>-999.99)
        //search all nodes contained in this compartment (share same "Pid", "pid", "cid")// to ensure the 'and' condition
		
		vector<vector<int>> count;
		for(set<vector<int>>::iterator it=sID.begin(); it!=sID.end(); it++)		
		{	
				vector<int> item=*it;
				vector<int> unit(4,0);
				int Pid=item[0], pid=item[1], cid=item[2], id=item[3], type=item[4],cid1=-1;
				vector<int> node; node.push_back(type); node.push_back(id);
				bool found=false;
				if(j>=_scene->CompartmentContain[m_pathwayID].size())
					continue;
				bool flag=_scene->CompartmentContain[m_pathwayID][j].find(node)!=_scene->CompartmentContain[m_pathwayID][j].end();
				bool flag1= false;
				
				if(!flag)
				{
				        for(set<vector<int>>::iterator it=_scene->CompartmentContain[m_pathwayID][j].begin(); it!=_scene->CompartmentContain[m_pathwayID][j].end(); it++)
						{
							//if(s_scene->CompartmentContain[m_pathwayID][j].find(item)!=_scene->CompartmentContain[m_pathwayID][j].end())
							vector<int> item=*it;
                            int type1=item[0],id1=item[1];
							if(isSameNode(Pid, type, id, m_pathwayID, type1, id1))
							{
									flag1=true;
									break;
							}
						}				
				}
				for(int i=0; i<count.size(); i++)
				{					
				   if(count[i][0]==Pid && count[i][1]==pid && count[i][2]==cid && (cid ==j || (cid ==-1 && flag  ) || type == 'W' ))
				   {
					   count[i][3]++;
					   found=true; break;
				   }
				}
				if(!found && (cid ==j || (cid ==-1&& (flag||flag1 || type == 'W' ) )))
				{
					unit[0]=Pid, unit[1]=pid, unit[2]=cid; unit[3] = 1;
					count.push_back(unit);		
				}
		}

		{			
      		for(set<vector<int>>::iterator it=sID.begin(); it!=sID.end(); it++)		
			{	
				vector<int> item=*it;
				int Pid=item[0], pid=item[1], cid=item[2], id=item[3], type=item[4],cid1=-1;
				int Did=_scene->_pathBubbles[Pid]->dataID;
				//for(int k=0; k<_scene->_smallMoleculeName[m_pathwayID].size(); k++)						
				if(type!='W'&& type!='M') //node to compartment
				{

					vector<QString> iName;
					switch(type)
			        {
						case 'C': iName=_scene->_complexNameD[Did][id];  break;
						case 'D': iName=_scene->_DnaNameD[Did][id];      break;
						case 'E': iName=_scene->_physicalEntityNameD[Did][id];      break;
						case 'L': iName=_scene->_ANodeName[Pid][id];      break;
						case 'P': iName=_scene->_proteinNameD[Did][id];      break;
						case 'S': iName=_scene->_smallMoleculeNameD[Did][id];      break;
						case 'R': iName=_scene->_reactionNameD[Did][id];      break;
					}
					if(iName.size()<5)	iName.resize(5);
				    if(pid==-1||pid==m_pathwayID) //pathway level
					{
						   if((cid==-1||cid==j)&&nodePos[m_pathwayID][j].x()>-999.99)//cid compartment level
				           { 								
						      //vector<int> item(2,0);                              
							  //item[0]=type, item[1]=id;  	
							   int xcount=0;
							  for(set<vector<int>>::iterator it=_scene->CompartmentContain[m_pathwayID][j].begin(); it!=_scene->CompartmentContain[m_pathwayID][j].end(); it++)
							  {
							      //if(s_scene->CompartmentContain[m_pathwayID][j].find(item)!=_scene->CompartmentContain[m_pathwayID][j].end())
								  vector<int> titem=*it;
                                  int type1=titem[0],id1=titem[1];
								if(xcount++>=117)
					                   xcount=xcount;
								
                                  vector<QString> Name1;
							      switch(type1)
							      {	
					                  case 'C': if(_scene->_complexPos[m_pathwayID][id1].x()>-999.99)			Name1=_scene->_complexNameD[dataID][id1];  break;
						              case 'D': if(_scene->_DnaPos[m_pathwayID][id1].x()>-999.99)				Name1=_scene->_DnaNameD[dataID][id1];      break;
						              case 'E': if(_scene->_physicalEntityPos[m_pathwayID][id1].x()>-999.99)	Name1=_scene->_physicalEntityNameD[dataID][id1];  break;
									  case 'L': if(_scene->_ANodePos[m_pathwayID][id1].x()>-999.99)	            Name1=_scene->_ANodeName[m_pathwayID][id1];  break;
						              case 'P': if(_scene->_proteinPos[m_pathwayID][id1].x()>-999.99)			Name1=_scene->_proteinNameD[dataID][id1];  break;
						              case 'S': if(_scene->_smallMoleculePos[m_pathwayID][id1].x()>-999.99)	Name1=_scene->_smallMoleculeNameD[dataID][id1];   break;
						              case 'R': if(_scene->_reactionPos[m_pathwayID][id1].x()>-999.99)		Name1=_scene->_reactionNameD[dataID][id1];   break;
					             }
								 if(Name1.size()<5)	 Name1.resize(5);
							  	 //if( ( (type!='R' && iName == nodeName[Did][j]) || (type=='R' && iName[0] == nodeName[Did][j][0]) ) && nodePos[m_pathwayID][j].x()>-999.99) //node level 
								 if(type==type1)
								 //if( ( type1!='R' && iName == Name1 ) || (type1=='R' && Name1.size()>=1 && iName[0] == Name1[0] && isSameReaction(Pid, id, m_pathwayID, id1) )  )
								 if( ( (type!='R'&&type!='L') && ( (completeSame && iName == Name1) || (!completeSame && iName[0] == Name1[0]) ) ) 
								  || ( (type=='R'||type=='L') && ( ( completeSame && iName == Name1) || ( !completeSame && iName[1] == Name1[1]) ) ) )							     
							     {
									 for(int i=0; i<count.size(); i++)
									 {					
											if(count[i][0]==Pid && count[i][1]==pid && count[i][2]==cid)
				                            {
												 vector<int> item1=item;
					                             item1[0]=m_pathwayID;  item1[3]=id1;   		
					                             set<vector<int>> markedNode;													 
					                             if(compareSubgraph(edgeSet, nodeSet, markedNode, item, item1, 2))	
                                                 {
													  for(set<vector<int>>::iterator it=markedNode.begin(); it!=markedNode.end(); it++)		
					                                  {
														 vector<int> ctemp=*it;
														 if(ctemp[0]!=j)
														 {
															 sharedNode[m_pathwayID].insert(ctemp[0]);																 
														 }														 

													  }
													  count[i][3]--;
					                                  break;
										         }
				                            }
				                     }								     
							     }	 
							  }
						   }						  						  
					}				  				   
			   }
			    else if(type=='M') //compartment to compartment			    
				{
					vector<QString> iName;
					iName=_scene->_compartmentName[Pid][cid];  					
					if(j<nodePos[m_pathwayID].size() && _scene->_pathBubbles[m_pathwayID]->compartmentRect[j].width()>0 && _scene->_pathBubbles[Pid]->compartmentRect[cid].width()>0) //&& nodePos[m_pathwayID][j].x()>-999.99 && _scene->_pathBubbles[Pid]->compartmentRect[cid].width()>0 ) //scene->_compartmentPos[Pid][cid].x()>-0.999) //node level scene->_reactionNameD[Did][id]
				    {
                       if(pid==-1||pid==m_pathwayID) //pathway level
					   {
						   if((cid==-1||cid==j))//cid compartment level
				           { 
							   for(int i=0; i<count.size(); i++)
							  {					
									if(count[i][0]==Pid && count[i][1]==pid && count[i][2]==cid)
				                     {
                                           	count[i][3]--;
					                        break;
										   
				                     }
				               }						       		  								
						   }						  						  
					   }				  
				   }
			   }
				else if(type=='W') //pathway to compartment			    
				{										
					if(pid==m_pathwayID) //pathway level
					{
						   if(cid>=0 )
						   {
							   if(_scene->_pathBubbles[m_pathwayID]->compartmentRect[j].width()>0 && _scene->_pathBubbles[Pid]->compartmentRect[cid].width()>0) //node level: odePos[m_pathwayID][j].x()>-999.99 &&scene->_reactionNameD[Did][id])//cid compartment level
							   { 
								  for(int i=0; i<count.size(); i++)
								  {					
										if(count[i][0]==Pid && count[i][1]==pid && count[i][2]==cid)
										 {
												 count[i][3]--;
												  break;
										 }
								   }
								   //sharedNode[m_pathwayID].insert(j);				  								
							   }
						   }
						   else
						   {
							   for(int l=0; l<_scene->_pathBubbles[m_pathwayID]->compartmentRect.size() && l<_scene->_pathBubbles[pid]->compartmentRect.size();l++)
							   {
							      if(_scene->_pathBubbles[m_pathwayID]->compartmentPos[l].x()>-0.99 && _scene->_pathBubbles[Pid]->compartmentPos[l].x()>-0.99) //node level: odePos[m_pathwayID][j].x()>-999.99 &&scene->_reactionNameD[Did][id])//cid compartment level
								   { 
									  /*for(int i=0; i<count.size(); i++)
									  {					
											if(count[i][0]==Pid && count[i][1]==pid && count[i][2]==cid)
											 {
													 count[i][3]--;
													  break;
											 }
									   }*/
									   QRectF pos = _scene->_pathBubbles[m_pathwayID]->compartmentPos[l];

									   item[0]='M', item[1]=l; 	
							           vector<int> nItem=item;   nItem.push_back(m_pathwayID);
									   cItem[ccount].insert(nItem);	
									   //sharedNode[m_pathwayID].insert(l);				  								
								   }
							   }
						   
						   }
					}	
					
			   }//else if(type=='M')
               
                for(int i=0; i<count.size(); i++)
				{					
				   if(count[i][3]<=0)
				   {
				       if(type!='W')
				           sharedNode[m_pathwayID].insert(j);		                                   
					   else
					   {								   
						      item[0]='M', item[1]=j; 	
							   vector<int> nItem=item; nItem.push_back(m_pathwayID);
						      if(m_pathwayID==Pid)
					          {
								   cItem[ccount].insert(nItem);								   
								   isShared=true;
						      }
					   }
				   }
				}
				if(type=='W')
					ccount++;
				
			}	
		}	
	}
}

void PathBubble1::recordNodeinRelationship(set<vector<int>> psID, vector<set<vector<int>>> &cItem, set<vector<int>> &sID, vector<set<vector<vector<int>>>> &edgeSet, vector<set<vector<int>>> &nodeSet)
{
	//identify item with containing relationship
	vector<int> tmp(3,0),mtmp;
	vector<vector<int>> temp,mark(3,mtmp),mark2(3,mtmp);
    vector<vector<vector<int>>> pyramid(3,temp);	
    for(set<vector<int>>::iterator it=psID.begin(); it!=psID.end(); it++)		
	{
		vector<int> item=*it;
	    int Pid=item[0], pid=item[1], cid=item[2], id=item[3], type=item[4];
		if(type=='W')
		{
            tmp[0]='W', tmp[1]=pid; tmp[2]=Pid;
			pyramid[0].push_back(tmp);  mark[0].push_back(-1); mark2[0].push_back(0); 
		}
		else if(type=='M')
		{
			tmp[0]='M', tmp[1]=cid; tmp[2]=Pid;
			pyramid[1].push_back(tmp);  mark[1].push_back(-1); mark2[1].push_back(0); 
		}
		else 
		{
			tmp[0]=type, tmp[1]=id; tmp[2]=Pid;
			pyramid[2].push_back(tmp);  mark[2].push_back(-1); mark2[2].push_back(0); 
		}
	}
	//tree path search
    for(int i=0;i<pyramid.size()-1;i++)
	{
		for(int j=0; j<pyramid[i].size(); j++)
		{
			    for(int k=0; k<pyramid[i+1].size(); k++)
				{
				   bool is_Contain=false;
				   int ctype=pyramid[i][j][0], cid=pyramid[i][j][1], type=pyramid[i+1][k][0], id=pyramid[i+1][k][1], Pid=pyramid[i][j][2], Pid1=pyramid[i+1][k][2];
				   if(Pid==Pid1)
				   {
						if(ctype=='W')
						{
							is_Contain=true; 
						}
						if(ctype=='M')
						{
							vector<int> item(2,0);
							item[0]=type, item[1]=id;                      
							if (_scene->CompartmentContain[Pid][cid].find(item)!=_scene->CompartmentContain[Pid][cid].end())
							{
								is_Contain=true;  
							}
						}
						if(is_Contain)
							mark[i+1][k]=j;
				   }
				}		
		}
	}
    set<vector<int>> pID;
    for(int i=pyramid.size()-1; i>=0; i--)
	{
		for(int j=0; j<pyramid[i].size(); j++)
		{
			vector<int> tmp(3,-1);
			//vector<vector<int>> temp(3,tmp);		
			//temp[i][0]=pID;
			tmp[i]=j;
			int pid=mark[i][j];
			if(!mark2[i][j])
			{
				if(pid>=0)
				{
					for(int k=i-1; k>=0; k--)
					{
						tmp[k]=pid;
						mark2[k][pid]=1;			  
						if(mark[k][pid]>=0)
						{
							//tmp[k]=pid;
							pid=mark[k][pid];							
						}
						else break;
					}
				 }				 				
				 pID.insert(tmp);
			}
			mark2[i][j]=1;			
		}
	}
	set<vector<int>> pIDt;
	for(set<vector<int>>::iterator it=pID.begin(); it!=pID.end(); it++)
	{
	    vector<int> tmp=*it, ttmp(5,-1);	
			   
		for(int i=0; i<tmp.size(); i++)
		{
			if(tmp[i]>=0)
			{ 
			   ttmp[0]=pyramid[i][tmp[i]][2];
			   ttmp[4]=pyramid[i][tmp[i]][0];	//type of the lowest 			        
			   if(ttmp[4]=='W')
			   	   ttmp[1]=pyramid[i][tmp[i]][1];							   
			   else if(ttmp[4]=='M')
			       ttmp[2]=pyramid[i][tmp[i]][1];			
			   else
			       ttmp[3]=pyramid[i][tmp[i]][1];			
			}			
		}	
		pIDt.insert(ttmp);
	}
	sID=pIDt;

	
	searchSubgraph( sID, edgeSet, nodeSet);
	

	for(set<vector<int>>::iterator it=sID.begin(); it!=sID.end(); it++)	
	{
		set<vector<int>> temp;
		vector<int> item=*it;	
		if(item[4]=='W'|| item[4]=='M') //node to node
			cItem.push_back(temp);
	}
	return;
}

vector<set<vector<int>>> PathBubble1::searchSharedNodeForSearchLinked( set<vector<int>> psID, bool Andflag)
{  //search shared node before searchlinked
   //search M if a item is M (may containing others), search "PA" if a top item is "P", search "P".
	
	vector<set<vector<int>>> cItem; //store a low level item contained by a higher level item.
	set<vector<int>> sID;
	vector<set<vector<int>>> markedNodes;
	

	vector<set<vector<vector<int>>>> edgeSet;
	vector<set<vector<int>>> nodeSet;

	recordNodeinRelationship(psID, cItem, sID, edgeSet, nodeSet);

	//sID
	
	int pflag, cflag, wflag;
	pflag=1, cflag=0, wflag=0;
	
	//set<vector<int>> sID;	
	for(int m=0; m<9; m++)
	{
		int ctype;
		 vector<vector<vector<QString>>> nodeName;
		 vector<set<int>> sharedNode;
		 vector<vector<QRectF>> nodePos;
		 bool tflag=false;
		
		switch(m)
		{
			 case 0: nodeName=_scene->_proteinNameD;        sharedNode=sharedProtein;        break;			 
			 case 1: nodeName=_scene->_smallMoleculeNameD;  sharedNode=sharedSmallMolecule;  break;
			 case 2: nodeName=_scene->_complexNameD;        sharedNode=sharedComplex;        break;
			 case 3: nodeName=_scene->_DnaNameD;            sharedNode=sharedDna;            break;
			 case 4: nodeName=_scene->_reactionNameD;       sharedNode=sharedReaction;       break;
			 case 5: nodeName=_scene->_physicalEntityNameD; sharedNode=sharedPhysicalEntity; break;
			 case 6: nodeName=_scene->_ANodeName;           sharedNode=sharedANode;        break;
			 case 7: nodeName=_scene->_compartmentName;     sharedNode=sharedCompartment;    break;		     
			 case 8: sharedNode=sharedPathway;                                              break;
		}	
			
		int csize=sharedNode.size();
		if(m<7)
		{
			for(int i=csize; i<_scene->_compartmentName.size(); i++)//nodeName
			{  set<int> record;
			  sharedNode.push_back(record);
			}
		}
		else
		{
			  for(int i=csize; i<_scene->_pathBubbles.size(); i++)
			  {  set<int> record;
				 sharedNode.push_back(record);
			  }	
			  if(!Andflag)
				 differedPathway=sharedNode;  	
		}
		if(m<7)
		switch(m)
		{
			case 0:  sharedProtein=sharedNode;  break;
			case 1:  sharedSmallMolecule=sharedNode;  break;
			case 2:  sharedComplex=sharedNode;  break;
			case 3:  sharedDna=sharedNode;  break;
			case 4:  sharedReaction=sharedNode;  break;
			case 5:  sharedPhysicalEntity=sharedNode;  break;
			case 6:  sharedANode=sharedNode;  break;			
		}			
		if(m==7)
		{	
			sharedCompartment=sharedNode;  		
		}
		else if(m==8)
		{	
			sharedPathway=sharedNode;  		
		}
	}

	
	for(set<vector<int>>::iterator it=sID.begin(); it!=sID.end(); it++)
	{
        vector<int> item=*it;
		set<vector<int>> sID2;
		sID2.insert(item);
		int Pid=item[0], pid=item[1], cid=item[2], id=item[3], type=item[4];	
		for(int m=0; m<9; m++)
        {
			int ctype;
			 vector<vector<vector<QString>>> nodeName;
			 vector<set<int>> sharedNode;
			 vector<vector<QRectF>> nodePos;
			 bool tflag=false;
		
			switch(m)
			{
					 case 0: ctype='P'; nodeName=_scene->_proteinNameD;         nodePos=_scene->_proteinPos;          sharedNode=sharedProtein;  break;
					 case 1: ctype='S'; nodeName=_scene->_smallMoleculeNameD;   nodePos=_scene->_smallMoleculePos;    sharedNode=sharedSmallMolecule;  break;
					 case 2: ctype='C'; nodeName=_scene->_complexNameD;         nodePos=_scene->_complexPos;          sharedNode=sharedComplex;  break;
					 case 3: ctype='D'; nodeName=_scene->_DnaNameD;             nodePos=_scene->_DnaPos;              sharedNode=sharedDna;  break;
					 case 4: ctype='R'; nodeName=_scene->_reactionNameD;        nodePos=_scene->_reactionPos;         sharedNode=sharedReaction;  break;
					 case 5: ctype='E'; nodeName=_scene->_physicalEntityNameD;  nodePos=_scene->_physicalEntityPos;   sharedNode=sharedPhysicalEntity;  break;
					 case 6: ctype='L'; nodeName=_scene->_ANodeName;           nodePos=_scene->_ANodePos;            sharedNode=sharedANode;  break;
					 case 7: ctype='M'; nodeName=_scene->_compartmentName;      nodePos=_scene->_compartmentPos;      sharedNode=sharedCompartment;  break;
					 case 8: ctype='W'; /*nodeName=_scene->_pathwayName;     nodePos=_scene->_compartmentPos; */     sharedNode=sharedPathway;  break;
			}
		
			if( pid>=0 && m==8)//search pathway
			{		
				searchSharedPathway(ctype, nodeName, nodePos, sID2, edgeSet, nodeSet, cItem, markedNodes,  sharedNode);
				sharedPathway=sharedNode;  		
			}
			else if(cid>=0 && m==7)//search compartment
			{		
				searchSharedCompartment(ctype, nodeName, nodePos, sID2, edgeSet, nodeSet, cItem, markedNodes,  sharedNode);
				sharedCompartment=sharedNode;  		
			}
			if(m<7)//search node
			{		
				searchSharedNode(ctype, nodeName, nodePos, sID2, edgeSet, nodeSet, cItem, markedNodes,  sharedNode, true);
				switch(m)
				{
					 case 0:  sharedProtein=sharedNode;  break;
					 case 1:  sharedSmallMolecule=sharedNode;  break;
					 case 2:  sharedComplex=sharedNode;  break;
					 case 3:  sharedDna=sharedNode;  break;
					 case 4:  sharedReaction=sharedNode;  break;
					 case 5:  sharedPhysicalEntity=sharedNode;  break;	
					 case 6:  sharedANode=sharedNode;  break;				 	 
				}	
			}
		}
	
	}
    


    for(int i=0;  i<markedNodes.size(); i++)   
	{
		for(set<vector<int>>::iterator it=markedNodes[i].begin(); it!=markedNodes[i].end(); it++)   
		{
			vector<int> temp=*it;
			switch(temp[4])
			{
				case 'P': sharedProtein[m_pathwayID].insert(temp[3]);  break;
				case 'S': sharedSmallMolecule[m_pathwayID].insert(temp[3]);  break;
				case 'C': sharedComplex[m_pathwayID].insert(temp[3]);  break;
				case 'D': sharedDna[m_pathwayID].insert(temp[3]);  break;
				case 'R': sharedReaction[m_pathwayID].insert(temp[3]);  break;
				case 'E': sharedPhysicalEntity[m_pathwayID].insert(temp[3]);  break;
				case 'L': sharedANode[m_pathwayID].insert(temp[3]);  break;
			}
	    }		
		//sharedNode[m_pathwayID].insert(j);		  								
	}
    return cItem;

}


 
vector<set<vector<int>>> PathBubble1::searchSharedNode( set<vector<int>> psID, int pflag, int cflag, int wflag, bool Andflag)
{  //int wflag, cflag, pflag: give the level to search. (only one could be 1, destiny, output, e. g 1, 0, 0 means search pathway) 	
	//sID give the items to search (oringin, input): if one item is contained in another, then, we search for target fufil the same condition
	//for example, a selected protein(or proteins) is contained in a selected compartment, if we search protein, then, the output is the protein contained in the same compartments in other places
	//if we search compartment, then, the output is the compartment containing the same protein(or proteins) in other places.
	//if we search for pathway, then, the output is the pathway contain the same compartment containing the same protein(or proteins) in other places.
	//if a select item do not be contained by the other or contain others, then we search all the targets (in the given level) which match them seperately
	//if Andflag == flase search the item not shared by the pflag
	
	vector<set<vector<int>>> cItem; //store a low level item contained by a higher level item.
	set<vector<int>> sID;
	vector<set<vector<int>>> markedNodes;

	if(pflag*cflag!=0||pflag*wflag!=0||cflag*wflag!=0)
	    return cItem;	

	vector<set<vector<vector<int>>>> edgeSet;
	vector<set<vector<int>>> nodeSet;

	recordNodeinRelationship(psID, cItem, sID, edgeSet, nodeSet);

	for(int m=0; m<9; m++)
	{
		int ctype;
		 vector<vector<vector<QString>>> nodeName;
		 vector<set<int>> sharedNode;
		 vector<vector<QRectF>> nodePos;
		 bool tflag=false;
		
		switch(m)
		{
			 case 0: nodeName=_scene->_proteinNameD;        sharedNode=sharedProtein;        break;
			 case 1: nodeName=_scene->_smallMoleculeNameD;  sharedNode=sharedSmallMolecule;  break;
			 case 2: nodeName=_scene->_complexNameD;        sharedNode=sharedComplex;        break;
			 case 3: nodeName=_scene->_DnaNameD;            sharedNode=sharedDna;            break;
			 case 4: nodeName=_scene->_reactionNameD;       sharedNode=sharedReaction;       break;
			 case 5: nodeName=_scene->_physicalEntityNameD; sharedNode=sharedPhysicalEntity; break;
			 case 6: nodeName=_scene->_ANodeName;          sharedNode=sharedANode; break;
			 case 7: nodeName=_scene->_compartmentName;     sharedNode=sharedCompartment;    break;
			 case 8: sharedNode=sharedPathway;        break;
		}	
			
		int csize=sharedNode.size();
		if(m<8)
		{
			for(int i=csize; i<_scene->_compartmentName.size(); i++)//nodeName
			{  set<int> record;
			  sharedNode.push_back(record);
			}

			if(!Andflag)
			{
				switch(m)
				{
					 case 0:  differedProtein=sharedNode;  break;
					 case 1:  differedSmallMolecule=sharedNode;  break;
					 case 2:  differedComplex=sharedNode;  break;
					 case 3:  differedDna=sharedNode;  break;
					 case 4:  differedReaction=sharedNode;  break;
					 case 5:  differedPhysicalEntity=sharedNode;  break;	
					 case 6:  differedANode=sharedNode;  break;				 
					 case 7:  differedCompartment=sharedNode;  	break;			 
				}
			}				 		
			 
		}
		else
		{
		  for(int i=csize; i<_scene->_pathBubbles.size(); i++)
		  {  set<int> record;
			 sharedNode.push_back(record);
		  }	
		  if(!Andflag)
			 differedPathway=sharedNode;  	
		}

		if(pflag && !cflag && !wflag && m<7)
		{
			switch(m)
			{
				 case 0:  sharedProtein=sharedNode;  break;
				 case 1:  sharedSmallMolecule=sharedNode;  break;
				 case 2:  sharedComplex=sharedNode;  break;
				 case 3:  sharedDna=sharedNode;  break;
				 case 4:  sharedReaction=sharedNode;  break;
				 case 5:  sharedPhysicalEntity=sharedNode;  break;				 	 
				 case 6:  sharedANode=sharedNode;  break;	
			}	
		}
		else if(!pflag && cflag && !wflag && m==7)
		{	
			sharedCompartment=sharedNode;  		
		}
		else if(!pflag && !cflag && wflag && m==8)
		{	
			sharedPathway=sharedNode;  		
		}
	}

for(int m=0; m<9; m++)
{
	int ctype;
	 vector<vector<vector<QString>>> nodeName;
	 vector<set<int>> sharedNode;
	 vector<vector<QRectF>> nodePos;
	 bool tflag=false;
		
	switch(m)
	{
		     case 0: ctype='P'; nodeName=_scene->_proteinNameD;         nodePos=_scene->_proteinPos;          sharedNode=sharedProtein;  break;
			 case 1: ctype='S'; nodeName=_scene->_smallMoleculeNameD;   nodePos=_scene->_smallMoleculePos;    sharedNode=sharedSmallMolecule;  break;
			 case 2: ctype='C'; nodeName=_scene->_complexNameD;         nodePos=_scene->_complexPos;          sharedNode=sharedComplex;  break;
			 case 3: ctype='D'; nodeName=_scene->_DnaNameD;             nodePos=_scene->_DnaPos;              sharedNode=sharedDna;  break;
			 case 4: ctype='R'; nodeName=_scene->_reactionNameD;        nodePos=_scene->_reactionPos;         sharedNode=sharedReaction;  break;
			 case 5: ctype='E'; nodeName=_scene->_physicalEntityNameD;  nodePos=_scene->_physicalEntityPos;   sharedNode=sharedPhysicalEntity;  break;
			 case 6: ctype='L'; nodeName=_scene->_ANodeName;           nodePos=_scene->_ANodePos;           sharedNode=sharedANode;  break;
			 case 7: ctype='M'; nodeName=_scene->_compartmentName;     nodePos=_scene->_compartmentPos;      sharedNode=sharedCompartment;  break;
			 case 8: ctype='W'; /*nodeName=_scene->_pathwayName;     nodePos=_scene->_compartmentPos; */     sharedNode=sharedPathway;  break;
	}
	//int csize=sharedNode.size();
	
	if(pflag && !cflag && !wflag && m<7)//search node
	{		
		searchSharedNode(ctype, nodeName, nodePos, sID, edgeSet, nodeSet, cItem, markedNodes,  sharedNode);
	}
	if(!pflag && cflag && !wflag && m==7)//search compartment
	{		
		searchSharedCompartment(ctype, nodeName, nodePos, sID, edgeSet, nodeSet, cItem, markedNodes,  sharedNode);
	}
	if(!pflag && !cflag && wflag && m==8)//search pathway
	{		
		searchSharedPathway(ctype, nodeName, nodePos, sID, edgeSet, nodeSet, cItem, markedNodes,  sharedNode);
	}
	else if(!pflag && !cflag && !wflag) //general search
	{	
		for(int j=0; j<nodeName[m_pathwayID].size(); j++)
		{
			if(smallMoleculePos[j].x()>-999.99)
			for(int i=0; i<nodeName.size(); i++)
			{			
      			//if(i!=m_pathwayID)
				vector<int> item;
				item.push_back(i);  item.push_back(-1); item.push_back(-1);
				if( m_pathwayID!= i && sID.find(item)!=sID.end())
				{
				   for(int k=0; k<nodeName[i].size(); k++)		
				   {			 
					  if(nodeName[i][k]==nodeName[m_pathwayID][j])  
					  {
						  if(nodePos[i][k].x()>-999.99)
						  {						
							  	vector<int> ctemp; ctemp.push_back('S'); ctemp.push_back(j);									
									{
										vector<int> otemp;
										otemp.push_back('S'); otemp.push_back(k);										
										{
											{
												 sharedNode[i].insert(j);				  
											}
										}
									}							  
						  
						  }				  
					  }
				   }			
				}	
			}	
		}
	}

	if(pflag && !cflag && !wflag && m<7)
	{		
		switch(m)
		{
			case 0:  sharedProtein=sharedNode;  break;
			case 1:  sharedSmallMolecule=sharedNode;  break;
			case 2:  sharedComplex=sharedNode;  break;
			case 3:  sharedDna=sharedNode;  break;
			case 4:  sharedReaction=sharedNode;  break;
			case 5:  sharedPhysicalEntity=sharedNode;  break;	
			case 6:  sharedANode=sharedNode;  break;			 	 
		}	
	}
	else if(!pflag && cflag && !wflag && m==7)
	{			
		sharedCompartment=sharedNode;  		
	}
	else if(!pflag && !cflag && wflag && m==8)
	{		 
		sharedPathway=sharedNode;  		
	}
    
    }//m:0-6

    for(int i=0;  i<markedNodes.size(); i++)   
	{
		for(set<vector<int>>::iterator it=markedNodes[i].begin(); it!=markedNodes[i].end(); it++)   
		{
			vector<int> temp=*it;
			switch(temp[4])
			{
				case 'P': sharedProtein[m_pathwayID].insert(temp[3]);  break;
				case 'S': sharedSmallMolecule[m_pathwayID].insert(temp[3]);  break;
				case 'C': sharedComplex[m_pathwayID].insert(temp[3]);  break;
				case 'D': sharedDna[m_pathwayID].insert(temp[3]);  break;
				case 'R': sharedReaction[m_pathwayID].insert(temp[3]);  break;
				case 'E': sharedPhysicalEntity[m_pathwayID].insert(temp[3]);  break;
				case 'L': sharedANode[m_pathwayID].insert(temp[3]);  break;
			}
	    }		
		//sharedNode[m_pathwayID].insert(j);		  								
	}
    return cItem;
}


vector<set<vector<int>>> PathBubble1::searchDifferedNode( set<vector<int>> psID, int pflag, int cflag, int wflag)
{//  similar to searchSharedNode(, but search diffedNode in same level of the input node if the input node is not contained by or contain others, for search differd only
	
	vector<set<vector<int>>> cItem; //store a low level item contained by a higher level item.
	set<vector<int>> sID;
	vector<set<vector<int>>> markedNodes;

	if(pflag*cflag!=0||pflag*wflag!=0||cflag*wflag!=0)
	    return cItem;	

	vector<set<vector<vector<int>>>> edgeSet;
	vector<set<vector<int>>> nodeSet;

	recordNodeinRelationship(psID, cItem, sID, edgeSet, nodeSet);

	for(int m=0; m<9; m++)
	{
		int ctype;
		 vector<vector<vector<QString>>> nodeName;
		 vector<set<int>> differedNode;
		 vector<vector<QRectF>> nodePos;
		 bool tflag=false;
		
		switch(m)
		{
			 case 0: nodeName=_scene->_proteinNameD;        differedNode=differedProtein;        break;
			 case 1: nodeName=_scene->_smallMoleculeNameD;  differedNode=differedSmallMolecule;  break;
			 case 2: nodeName=_scene->_complexNameD;        differedNode=differedComplex;        break;
			 case 3: nodeName=_scene->_DnaNameD;            differedNode=differedDna;            break;
			 case 4: nodeName=_scene->_reactionNameD;       differedNode=differedReaction;       break;
			 case 5: nodeName=_scene->_physicalEntityNameD; differedNode=differedPhysicalEntity; break;
			 case 6: nodeName=_scene->_ANodeName;          differedNode=differedANode; break;
			 case 7: nodeName=_scene->_compartmentName;     differedNode=differedCompartment;    break;
			 case 8: differedNode=differedPathway;          break;
		}	
			
		int csize=differedNode.size();
		if(m<8)
		{
			for(int i=csize; i<_scene->_compartmentName.size(); i++)//nodeName
			{  set<int> record;
			  differedNode.push_back(record);
			}

			//if(!Andflag)
			{
				switch(m)
				{
					case 0:  differedProtein=differedNode;  break;
					case 1:  differedSmallMolecule=differedNode;  break;
					case 2:  differedComplex=differedNode;  break;
					case 3:  differedDna=differedNode;  break;
					case 4:  differedReaction=differedNode;  break;
					case 5:  differedPhysicalEntity=differedNode;  break;				 
					case 6:  differedANode=differedNode;  break;		
					case 7:  differedCompartment=differedNode;  	break;			 
				}
			}				 		
			 
		}
		else
		{
			  for(int i=csize; i<_scene->_pathBubbles.size(); i++)
			  {  set<int> record;
				 differedNode.push_back(record);
			  }	
			  //if(!Andflag)
			  differedPathway=differedNode;  	
		}

		if(pflag && !cflag && !wflag && m<7)
		{
			switch(m)
			{
				 case 0:  differedProtein=differedNode;  break;
				 case 1:  differedSmallMolecule=differedNode;  break;
				 case 2:  differedComplex=differedNode;  break;
				 case 3:  differedDna=differedNode;  break;
				 case 4:  differedReaction=differedNode;  break;
				 case 5:  differedPhysicalEntity=differedNode;  break;				 	 
				 case 6:  differedANode=differedNode;  break;	
			}	
		}
		else if(!pflag && cflag && !wflag && m==7)
		{	
			differedCompartment=differedNode;  		
		}
		else if(!pflag && !cflag && wflag && m==8)
		{	
			differedPathway=differedNode;  		
		}
	}

for(int m=0; m<9; m++)
{
	int ctype;
	 vector<vector<vector<QString>>> nodeName;
	 vector<set<int>> differedNode;
	 vector<vector<QRectF>> nodePos;
	 bool tflag=false;
		
	switch(m)
	{
		     case 0: ctype='P'; nodeName=_scene->_proteinNameD;         nodePos=_scene->_proteinPos;          differedNode=differedProtein;  break;
			 case 1: ctype='S'; nodeName=_scene->_smallMoleculeNameD;   nodePos=_scene->_smallMoleculePos;    differedNode=differedSmallMolecule;  break;
			 case 2: ctype='C'; nodeName=_scene->_complexNameD;         nodePos=_scene->_complexPos;          differedNode=differedComplex;  break;
			 case 3: ctype='D'; nodeName=_scene->_DnaNameD;             nodePos=_scene->_DnaPos;              differedNode=differedDna;  break;
			 case 4: ctype='R'; nodeName=_scene->_reactionNameD;        nodePos=_scene->_reactionPos;         differedNode=differedReaction;  break;
			 case 5: ctype='E'; nodeName=_scene->_physicalEntityNameD;  nodePos=_scene->_physicalEntityPos;   differedNode=differedPhysicalEntity;  break;
			 case 6: ctype='L'; nodeName=_scene->_ANodeName;           nodePos=_scene->_ANodePos;            differedNode=differedANode;  break;
			 case 7: ctype='M'; nodeName=_scene->_compartmentName;     nodePos=_scene->_compartmentPos;      differedNode=differedCompartment;  break;
			 case 8: ctype='W'; /*nodeName=_scene->_pathwayName;     nodePos=_scene->_compartmentPos; */     differedNode=differedPathway;  break;
	}
	//int csize=differedNode.size();


	if(pflag && !cflag && !wflag && m<7)//search node
	{		
		searchDifferedNode(ctype, nodeName, nodePos, sID, edgeSet, nodeSet, cItem, markedNodes,  differedNode);
	}

	if(!pflag && cflag && !wflag && m==7)//search compartment
	{		
		//searchDifferedCompartment(ctype, nodeName, nodePos, sID, edgeSet, nodeSet, cItem, markedNodes,  differedNode);
		searchDifferedNode(ctype, nodeName, nodePos, sID, edgeSet, nodeSet, cItem, markedNodes,  differedNode);
	}
	if(!pflag && !cflag && wflag && m==8)//search pathway
	{		
		//searchDifferedPathway(ctype, nodeName, nodePos, sID, edgeSet, nodeSet, cItem, markedNodes,  differedNode);
		
	}
	else if(!pflag && !cflag && !wflag) //general search
	{	
		for(int j=0; j<nodeName[m_pathwayID].size(); j++)
		{
			if(smallMoleculePos[j].x()>-999.99)
			for(int i=0; i<nodeName.size(); i++)
			{			
      			//if(i!=m_pathwayID)
				vector<int> item;
				item.push_back(i);  item.push_back(-1); item.push_back(-1);
				if( m_pathwayID!= i && sID.find(item)!=sID.end())
				{
				   for(int k=0; k<nodeName[i].size(); k++)		
				   {			 
					  if(nodeName[i][k]==nodeName[m_pathwayID][j])  
					  {
						  if(nodePos[i][k].x()>-999.99)
						  {						
							  //for(int m=0; m< _scene->CompartmentContain[m_pathwayID].size()-1;m++)
							  {
									vector<int> ctemp; ctemp.push_back('S'); ctemp.push_back(j);
									//if(_scene->CompartmentContain[m_pathwayID][m].find(ctemp) != _scene->CompartmentContain[m_pathwayID][m].end())
									{
										vector<int> otemp;
										otemp.push_back('S'); otemp.push_back(k);
										//for(int n=0; n<_scene->CompartmentContain[i].size()-1; n++)
										{
										//	if(_scene->_compartmentName[i][n]==_scene->_compartmentName[m_pathwayID][m])
										//	if(_scene->CompartmentContain[i][n].find(otemp) != _scene->CompartmentContain[i][n].end())
											{
												 differedNode[i].insert(j);				  
											}
										}
									}
							  }
						  
						  }				  
					  }
				   }			
				}	
			}	
		}
	}


	if(pflag && !cflag && !wflag && m<7)
	{
		
		switch(m)
		{
				case 0:  differedProtein=differedNode;  break;
				case 1:  differedSmallMolecule=differedNode;  break;
				case 2:  differedComplex=differedNode;  break;
				case 3:  differedDna=differedNode;  break;
				case 4:  differedReaction=differedNode;  break;
				case 5:  differedPhysicalEntity=differedNode;  break;	
				case 6:  differedANode=differedNode;  break;				 	 
		}	
	}
	else if(!pflag && cflag && !wflag && m==7)
	{			
		differedCompartment=differedNode;  		
	}
	else if(!pflag && !cflag && wflag && m==8)
	{		 
		
		differedPathway=differedNode;  		
	}
    
}//m:0-6

   
    return cItem;

}


void PathBubble1::deleteItems()
{
	complexPos.clear();
	proteinPos.clear();	
    smallMoleculePos.clear();	
    DnaPos.clear();	
    reactionPos.clear();	
    physicalEntityPos.clear();	
	edge.clear();
}


void PathBubble1::locateCompartmentforExtraItems(vector<vector<QString>> _complexName, vector<vector<QString>>_physicalEntityName, vector<vector<QString>> _proteinName, vector<vector<QString>> _reactionName,	vector<vector<QString>> _smallMoleculeName, vector<vector<QString>> _DnaName, vector<set<vector<int>>> &CompartmentContain, vector<vector<QString>> _compartmentName)
{ //some protein, complex appear in the reaction but can not find its compartment (or any other information event its name)
	
	for(int i=0; i<_complexName.size(); i++)
	 {
		 bool flag=true;
		 for(int j=0;j<CompartmentContain.size();j++)
		 {
			   vector<int> item;
			   item.push_back('C'); item.push_back(i);
			   if(CompartmentContain[j].find(item)!=CompartmentContain[j].end())
			   {   
				    flag=false;
					break;
			   }			
		 }
		 if(flag)
		 for(int k=0;k<_compartmentName.size();k++)
		 {
			if(_complexName[i][1]==_compartmentName[k][0])
			{
				vector<int> item;
				item.push_back('C'); item.push_back(i);
				CompartmentContain[k].insert(item);		
				break;
			}			
		}
	 }

	for(int i=0; i<_proteinName.size(); i++)
	 {
		 bool flag=true;
		 for(int j=0;j<CompartmentContain.size();j++)
		 {
			   vector<int> item;
			   item.push_back('P'); item.push_back(i);
			   if(CompartmentContain[j].find(item)!=CompartmentContain[j].end())
			   {   
				    flag=false;
					break;
			   }			
		 }
		 if(flag)
		 for(int k=0;k<_compartmentName.size();k++)
		 {
			if(_proteinName[i][1]==_compartmentName[k][0])
			{
				vector<int> item;
				item.push_back('P'); item.push_back(i);
				CompartmentContain[k].insert(item);		
				break;
			}			
		}
	 }

	for(int i=0; i<_physicalEntityName.size(); i++)
	 {
		 bool flag=true;
		 for(int j=0;j<CompartmentContain.size();j++)
		 {
			   vector<int> item;
			   item.push_back('E'); item.push_back(i);
			   if(CompartmentContain[j].find(item)!=CompartmentContain[j].end())
			   {   
				    flag=false;
					break;
			   }			
		 }
		 if(flag)
		 for(int k=0;k<_compartmentName.size();k++)
		 {
			if(_physicalEntityName[i][1]==_compartmentName[k][0])
			{
				vector<int> item;
				item.push_back('E'); item.push_back(i);
				CompartmentContain[k].insert(item);		
				break;
			}			
		}
	 }

	for(int i=0; i<_smallMoleculeName.size(); i++)
	 {
		 bool flag=true;
		 for(int j=0;j<CompartmentContain.size();j++)
		 {
			   vector<int> item;
			   item.push_back('S'); item.push_back(i);
			   if(CompartmentContain[j].find(item)!=CompartmentContain[j].end())
			   {   
				    flag=false;
					break;
			   }			
		 }
		 if(flag)
		 for(int k=0;k<_compartmentName.size();k++)
		 {
			if(_smallMoleculeName[i][1]==_compartmentName[k][0])
			{
				vector<int> item;
				item.push_back('S'); item.push_back(i);
				CompartmentContain[k].insert(item);		
				break;
			}			
		}
	 }


	for(int i=0; i<_DnaName.size(); i++)
	 {
		 bool flag=true;
		 for(int j=0;j<CompartmentContain.size();j++)
		 {
			   vector<int> item;
			   item.push_back('D'); item.push_back(i);
			   if(CompartmentContain[j].find(item)!=CompartmentContain[j].end())
			   {   
				    flag=false;
					break;
			   }			
		 }
		 if(flag)
		 for(int k=0;k<_compartmentName.size();k++)
		 {
			if(_DnaName[i][1]==_compartmentName[k][0])
			{
				vector<int> item;
				item.push_back('D'); item.push_back(i);
				CompartmentContain[k].insert(item);		
				break;
			}			
		}
	 }	
}


void PathBubble1::multiplyDna(vector<vector<QString> > &_DnaName)
{
	
	 vector<int> unit;
	 vector<vector<int>> record(_scene->CompartmentContain[m_pathwayID].size(), unit);
	
	 vector<vector<QString>> newName;	
	 newName.push_back(_DnaName[0]);
	 set<int> sset;
	 for(int i=0; i<_DnaName.size(); i++)
	 {
	     sset.insert(i);
	 }
	 for(int i=0; i<Dna.size(); i++)
	 {
	      sset.erase(Dna[i]);
	 }

	 for(int i=0; i<Dna.size(); i++)
	 {		 
		 newName.push_back(_DnaName[Dna[i]]);
		 for(int j=0;j<_scene->_compartmentName[m_pathwayID].size();j++)
		 {
			   vector<int> item;
			   item.push_back('D'); item.push_back(Dna[i]);
			   if(_scene->CompartmentContain[m_pathwayID][j].find(item)!=_scene->CompartmentContain[m_pathwayID][j].end())
			   {   				   
				   record[j].push_back(i+1);
				   break;
			   }			
		 }		 
	 }

	 int start=Dna.size();
	 for(set<int>::iterator it=sset.begin(); it!=sset.end(); it++)
	 {
	      int ID=*it;		  
		  newName.push_back(_DnaName[ID]);
		  for(int j=0;j<_scene->_compartmentName[m_pathwayID].size();j++)
		  {
			   vector<int> item;
			   item.push_back('S'); item.push_back(ID);
			   if(_scene->CompartmentContain[m_pathwayID][j].find(item)!=_scene->CompartmentContain[m_pathwayID][j].end())
			   {   
				   record[j].push_back(start);
				   break;
			   }			
		  }
		  start++;
	  }

	 for(int i=0; i<DnaNum; i++)
	 {
		 for(int j=0;j<_scene->_compartmentName[m_pathwayID].size();j++)
		 {
			   vector<int> item;
			   item.push_back('D'); item.push_back(i+1);
			   _scene->CompartmentContain[m_pathwayID][j].erase(item);			   	
		 }		
	 }
	 for(int i=0; i<record.size(); i++)
	 {
		 for(int j=0; j<record[i].size(); j++)	 
		 {
			  vector<int> item;
			 item.push_back('D'); item.push_back(record[i][j]);
		    _scene->CompartmentContain[m_pathwayID][i].insert(item);		 
		 }
	 }
	
	 int c=1;
	 for(int i=0; i<_scene->_biochemicalReaction[m_pathwayID].size(); i++)
	 {
		 for(int k=0;k<2;k++)
		 {
		  vector<int> left=_scene->_biochemicalReaction[m_pathwayID][i][k];
	      
		  for(int j=0; j<left.size(); j=j+2)
		  {
	         int type=left[j], id=left[j+1];
			 if(type=='D')
			 {
			     _scene->_biochemicalReaction[m_pathwayID][i][k][j+1]=c;	
				 c++;
			 }
		  }
		 }
	 }
	 _DnaName=newName;
	 vector<int> temp(newName.size(),0);
	 //markedD=temp;
}


void PathBubble1::multiplySmallMolecule(vector<vector<QString> > &_smallMoleculeName)
{
	 vector<int> smallMoleculeNameID;
	 if(!newSmallmolecularNameRead(_smallMoleculeName))
	 { 	
		  vector<vector<vector<int>>> _biochemicalReaction=_scene->_biochemicalReaction[m_pathwayID];
		  vector<bool> flagS(_biochemicalReaction.size(),false), flagN(_biochemicalReaction.size(),false);
		  vector<vector<int>> Link;
		  set<int> sSet,dSet;	  
		  for(int I=0; I<_biochemicalReaction.size(); I++)				
		  {	 
				vector<int> empty;
				Link.push_back(empty);

				for(int M=0; M<2; M++)	
				for(int J=0; J<_biochemicalReaction[I][M].size(); J=J+2)
				{
					vector<int> item1;
					 item1.push_back(_biochemicalReaction[I][M][J]);
					 item1.push_back(_biochemicalReaction[I][M][J+1]);
					for(int k=0; k<_biochemicalReaction.size(); k++)
					{	   
						if(k!=I)
						for(int n=0; n<2; n++)	
						for(int l=0; l<_biochemicalReaction[k][n].size(); l=l+2)	    
						{
							vector<int> item2;
							item2.push_back(_biochemicalReaction[k][n][l]);
							item2.push_back(_biochemicalReaction[k][n][l+1]);
							if(item1==item2)
							{
								if(item1[0]=='D' || item1[0]=='S')						
								{
									flagS[I]=true;
									Link[I]=item1;							
								}
								else
									flagN[I]=true;
							}			  
						}	
					}
				}				
		  }
	 
		 for(int i=0; i<_biochemicalReaction.size(); i++)
		 {	 
			 for(int m=0; m<2; m++)	
			 for(int j=0; j<_biochemicalReaction[i][m].size(); j=j+2)	    
			 {
				 vector<int> item1;
				 item1.push_back(_biochemicalReaction[i][m][j]);
				 item1.push_back(_biochemicalReaction[i][m][j+1]);			 
				 if(Link[i]==item1)
				 {  
					 if(item1[0]=='D')
						 dSet.insert(item1[1]);
					 else if(item1[0]=='S')
						 sSet.insert(item1[1]);
				 }			 
			 }	
		 }
		 vector<int> unit;
		 vector<vector<int>> record(_scene->CompartmentContain[m_pathwayID].size(), unit);
	
		 //vector<vector<QString>> newName;	
	 
		 //newName.push_back(_smallMoleculeName[0]);
		 smallMoleculeNameID.push_back(0);
		 set<int> sset;

		 for(int i=0; i<_smallMoleculeName.size(); i++)
		 {
			 sset.insert(i);
		 }

		 int start=1;
		 vector<int> Count(smallMoleculeNum+10,0); //count to make sure id in sSet only be counted once.
		 for(int i=0; i<smallMolecule.size(); i++)
		 {
			 bool flag=true;
			 if(sSet.find(smallMolecule[i])!=sSet.end())
			 {
				if(Count[smallMolecule[i]]>=1)
					flag=false;
			 }
			 if(flag && smallMolecule[i] < _smallMoleculeName.size() )
			 {

				 //newName.push_back(_smallMoleculeName[smallMolecule[i]]);
				 smallMoleculeNameID.push_back(smallMolecule[i]);
				 for(int j=0;j<_scene->_compartmentName[m_pathwayID].size();j++)
				 {
					   vector<int> item;
					   item.push_back('S'); item.push_back(smallMolecule[i]);
					   if(_scene->CompartmentContain[m_pathwayID][j].find(item)!=_scene->CompartmentContain[m_pathwayID][j].end())
					   {   
						   //item[1]=i+1;			       	
						   record[j].push_back(start);
						   break;
					   }			
				 }
				 start++;
			 }
			 Count[smallMolecule[i]]++;
		 }

		 //int start=smallMolecule.size();
		 for(set<int>::iterator it=sset.begin(); it!=sset.end(); it++)
		 {
			  int ID=*it;	
			  bool flag=true;
			  if(sSet.find(ID)!=sSet.end())
			  {
				 if(Count[ID]>=1)
					flag=false;
			  }
			  if(flag)
			  { 
				  //newName.push_back(_smallMoleculeName[ID]);
				  smallMoleculeNameID.push_back(ID);
				  for(int j=0;j<_scene->_compartmentName[m_pathwayID].size();j++)
				  {
					   vector<int> item;
					   item.push_back('S'); item.push_back(ID);
					   if(_scene->CompartmentContain[m_pathwayID][j].find(item)!=_scene->CompartmentContain[m_pathwayID][j].end())
					   {   
						   //item[1]=i+1;			       	
						   record[j].push_back(start);
						   break;
					   }			
				  }
				  start++;
			 }
			 Count[ID]++;		 
		 }

		 for(int i=0; i<smallMoleculeNum; i++)
		 {
			for(int j=0;j<_scene->_compartmentName[m_pathwayID].size();j++)
			{
				vector<int> item;
				item.push_back('S'); item.push_back(i+1);
				_scene->CompartmentContain[m_pathwayID][j].erase(item);			   	
			}		
		}

		for(int i=0; i<record.size(); i++)
		{
			for(int j=0; j<record[i].size(); j++)	 
			{
				vector<int> item;
				item.push_back('S'); item.push_back(record[i][j]);
				_scene->CompartmentContain[m_pathwayID][i].insert(item);		 
			}
		}

		 vector<int> Count2(smallMoleculeNum+10,0);
		 vector<int> ID(smallMoleculeNum+10,-1);
		 int c=1;

		 for(int i=0; i<_scene->_biochemicalReaction[m_pathwayID].size(); i++)
		 {
			 for(int k=0; k<2; k++)
			 {
				  vector<int> left=_scene->_biochemicalReaction[m_pathwayID][i][k];	  
				  vector<int> newLeft;
				  vector<int> newLeft1;
				  set<int> IDs;
				  for(int j=0; j<left.size(); j=j+2)
				  {
					 int type=left[j], id=left[j+1];
					 if(type=='S')
					 {
						   bool flag=true;
						   int C=c;
						  if(sSet.find(id)!=sSet.end())
						  {
							 if(Count2[id]>=1)
							 {
								flag=false;
								C=ID[id];
							 }
							 else
							 {
								ID[id]=C;
							 }
						  }
						  //_scene->_biochemicalReaction[m_pathwayID][i][k][j+1]=C;	
						  if(IDs.find(C)==IDs.end())
						  {
							  newLeft.push_back(type);
							  newLeft.push_back(C);
							  IDs.insert(C);
						  }	
						  
						  newLeft1.push_back(type);
						  newLeft1.push_back(C);

						  if(flag)
							  c++;
					 
						  Count2[id]++;
					 }
					 else 
					 {
						 newLeft.push_back(type);
						 newLeft.push_back(id);

						 newLeft1.push_back(type);
						 newLeft1.push_back(id);
					 }
				  }
				  _scene->_biochemicalReaction[m_pathwayID][i][k]=newLeft;	
			 }
		 }	 
		 //void PathBubble1::smallMoleculetoNewName(vector<vector<vector<int>>> _biochemicalReaction, vector<vector<QString> > &_smallMoleculeName, vector<int> smallMolecule)
		 newSmallmolecularNameRecord(smallMoleculeNameID);
		 smallMoleculetoNewName(_smallMoleculeName, smallMoleculeNameID);	
     }

	 //_smallMoleculeName=newName;
	 //vector<int> temp(smallMoleculeNameID.size(),0);
	 //markedS=temp;
}

bool PathBubble1::newSmallmolecularNameRecord(vector<int> smallMoleculeNameID)//, vector<int> smallMoleculeNameID)
{//resize: true, remove the compartment if it is empty
 	
	vector<vector<int>> VertexInfo_1;
	vector<vector<vector<int>>> Vertex_1;
	vector<vector<int>> Rank0_1;
	vector<vector<vector<int>>> Ranks_1;
	QString pathName = _scene->pwdParser->getPathGraphHie(_pathName); 
	vector<QRectF> newPos_1, newCompartmentPos_1;
	vector<vector<int>> Edge_1;

	//QString pathName = _scene->pwdParser->savePathGraphPreparation(_pathName);		
	pathName= pathName + ".sma";
	//QString fileName = QFileDialog::getSaveFileName(0, tr("Save Pathway Graph as xml file"), pathName,".xml (*.xml)");//"c:\\file.x", "X files (*.x)"

	QFile file(pathName);
	if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
		return false;
	
    QTextStream out(&file);
	
	out << "smallMoleculeNameID.size()" << " " << smallMoleculeNameID.size() << "\n";		
	for(int i=0; i<smallMoleculeNameID.size(); i++)
	{
		//for(int j=0; j<_VertexInfo[i].size(); j++)
        out << " " << smallMoleculeNameID[i];					
	}
	out << "\n";
    file.close();
	return true;
}


bool PathBubble1::newSmallmolecularNameRead(vector<vector<QString>> &_smallMoleculeName)//, vector<int> smallMoleculeNameID)
{//resize: true, remove the compartment if it is empty
 	
	vector<vector<int>> VertexInfo_1;
	vector<vector<vector<int>>> Vertex_1;
	vector<vector<int>> Rank0_1;
	vector<vector<vector<int>>> Ranks_1;
	QString pathName = _scene->pwdParser->getPathGraphHie(_pathName); 
	vector<QRectF> newPos_1, newCompartmentPos_1;
	vector<vector<int>> Edge_1;

	//QString pathName = _scene->pwdParser->savePathGraphPreparation(_pathName);		
	pathName= pathName + ".sma";
	//QString fileName = QFileDialog::getSaveFileName(0, tr("Save Pathway Graph as xml file"), pathName,".xml (*.xml)");//"c:\\file.x", "X files (*.x)"
	QFile file(pathName);
	if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return false;
	
	vector<int> smallMoleculeNameID;

	QTextStream in(& file);			 
	//while ( !in.atEnd() )
				   
	QString line = in.readLine();    
	if(!line.contains("smallMoleculeNameID.size()"))
		return false;
		
	QStringList sname=line.split(' ');	
	//nameSet.insert(sname[0]);				
	int I=sname[1].toInt();
	line = in.readLine(); 
	QStringList sl=line.split(' ');	
	for(int j=1; j<sl.size(); j++)
	{
		smallMoleculeNameID.push_back(sl[j].toInt());
	}
	file.close();

	smallMoleculetoNewName(_smallMoleculeName, smallMoleculeNameID);
	smallMoleculeNameID.clear();
	
	return true;
}

void PathBubble1::smallMoleculetoNewName(vector<vector<QString>> &_smallMoleculeName, vector<int> smallMoleculeNameID)
{		 
	vector<vector<QString>> newName;	
	for(int i=0; i<smallMoleculeNameID.size(); i++)
	{
		newName.push_back(_smallMoleculeName[smallMoleculeNameID[i]]);	
	}
	_smallMoleculeName=newName;
	newName.clear();

	/* set<int> sSet,dSet;	
	 vector<int> unit;
	 vector<vector<int>> record(_scene->CompartmentContain[m_pathwayID].size(), unit);
	
	 vector<vector<QString>> newName;	
	 newName.push_back(_smallMoleculeName[0]);
	 set<int> sset;
	 
	 for(int i=1; i<_smallMoleculeName.size(); i++)
	 {
	     sset.insert(i);
	 }
	 	
	 int start=1;
	 vector<int> Count(smallMoleculeNum+10,0);  //count to make sure id in sSet only be counted once.
	 for(int i=0; i<smallMolecule.size(); i++)
	 {
		 bool flag=true;
		 if(sSet.find(smallMolecule[i])!=sSet.end())
		 {
		    if(Count[smallMolecule[i]]>=1)
				flag=false;
		 }
		 if(flag && smallMolecule[i] < _smallMoleculeName.size() )
		 {

			 newName.push_back(_smallMoleculeName[smallMolecule[i]]);
			 for(int j=0;j<_scene->_compartmentName[m_pathwayID].size();j++)
			 {
				   vector<int> item;
				   item.push_back('S'); item.push_back(smallMolecule[i]);
				   if(_scene->CompartmentContain[m_pathwayID][j].find(item)!=_scene->CompartmentContain[m_pathwayID][j].end())
				   {   
					   //item[1]=i+1;			       	
					   record[j].push_back(start);
					   break;
				   }			
			 }
			 start++;
		 }
		 Count[smallMolecule[i]]++;
	 }

	 //int start=smallMolecule.size();
	 for(set<int>::iterator it=sset.begin(); it!=sset.end(); it++)
	 {
	      int ID=*it;	
		  bool flag=true;
		  if(sSet.find(ID)!=sSet.end())
		  {
		     if(Count[ID]>=1)
				flag=false;
		  }
		  if(flag)
		  { 
			  newName.push_back(_smallMoleculeName[ID]);
			  for(int j=0;j<_scene->_compartmentName[m_pathwayID].size();j++)
			  {
				   vector<int> item;
				   item.push_back('S'); item.push_back(ID);
				   if(_scene->CompartmentContain[m_pathwayID][j].find(item)!=_scene->CompartmentContain[m_pathwayID][j].end())
				   {   
					   //item[1]=i+1;			       	
					   record[j].push_back(start);
					   break;
				   }			
			  }
			  start++;
		 }
		 Count[ID]++;		 
	 }
	 for(int i=0; i<smallMoleculeNum; i++)
	 {
		for(int j=0;j<_scene->_compartmentName[m_pathwayID].size();j++)
		{
			vector<int> item;
			item.push_back('S'); item.push_back(i+1);
			_scene->CompartmentContain[m_pathwayID][j].erase(item);			   	
		}		
	}

	for(int i=0; i<record.size(); i++)
	{
		for(int j=0; j<record[i].size(); j++)	 
		{
			vector<int> item;
			item.push_back('S'); item.push_back(record[i][j]);
			_scene->CompartmentContain[m_pathwayID][i].insert(item);		 
		}
	}

	 vector<int> Count2(smallMoleculeNum+10,0);
	 vector<int> ID(smallMoleculeNum+10,-1);
	 int c=1;
	 for(int i=0; i<_scene->_biochemicalReaction[m_pathwayID].size(); i++)
	 {
		 for(int k=0; k<2; k++)
		 {
			  vector<int> left=_scene->_biochemicalReaction[m_pathwayID][i][k];	      
			  for(int j=0; j<left.size(); j=j+2)
			  {
				 int type=left[j], id=left[j+1];
				 if(type=='S')
				 {
					   bool flag=true;
					   int C=c;
					  if(sSet.find(id)!=sSet.end())
					  {
						 if(Count2[id]>=1)
						 {
							flag=false;
							C=ID[id];
						 }
						 else
						 {
						    ID[id]=C;
						 }
					  }
					  _scene->_biochemicalReaction[m_pathwayID][i][k][j+1]=C;	
					  
					  if(flag)
						  c++;
					 
					  Count2[id]++;
				 }
			  }
		 }
	 }
	 _smallMoleculeName=newName;
	 vector<int> temp(newName.size(),0);
	 //markedS=temp;
	 */
}


vector<vector<vector<int>>> PathBubble1::EdgetoNode(vector<vector<int>> edge, vector<vector<vector<int>>> &_protein, vector<vector<vector<int>>> &_complex, vector<vector<vector<int>>> &_entity, vector<vector<vector<int>>> &_smallMolecule, vector<vector<vector<int>>> &_Dna, vector<vector<vector<int>>> _reaction, bool whole, vector<QRectF> reactionPos1, bool rflag)
{  
	vector<set<vector<int>>> CompartmentContain=_scene->CompartmentContain[m_pathwayID];
	vector<set<vector<int>>> cContain=CompartmentContain;
	if(reactionPos1.empty())
	   putReactionToCompartment(_reaction, CompartmentContain);	
	else
	{	
		vector<int> start(4,0), middle(4,0), end(4,0), temp(2,0);
		int cid1, cid2;
		//temp[0]='R';	temp[1]=i; 
		//assign small moleculer compartmentcontain
		set<int> sSet;
		temp[0]='S';
		for(int i=0; i<edge.size(); i++)
		{	
			 if(reactionPos1[edge[i][5]].x()>-1000)
			 {
				 start[0]=edge[i][0], start[1]=edge[i][1];			 		 
				 middle[0]=edge[i][4]; middle[1]=edge[i][5];
				 end[0]=edge[i][2], end[1]=edge[i][3]; 
				 if(end[0]=='S'||start[0]=='S')
				 {
					cid2 = whichCompartment(m_pathwayID, 'R', edge[i][5]); //insert
				 }
				 if(end[0]=='S')		 
				 {
					 if( sSet.find(end[1])==sSet.end() )
					 {
						 cid1= whichCompartment(m_pathwayID, 'S', end[1]); //erase
						 temp[1]=end[1];
						 if(cid1>=0)
							CompartmentContain[cid1].erase(temp);
						 CompartmentContain[cid2].insert(temp);
						 sSet.insert(end[1]);
					 }
				 }
				 if(start[0]=='S')		 
				 {
					 if( sSet.find(start[1])==sSet.end() )
					 {
						 cid1= whichCompartment(m_pathwayID, 'S', start[1]); //erase
						 temp[1]=start[1];
						 if(cid1>=0)
							CompartmentContain[cid1].erase(temp);
						 CompartmentContain[cid2].insert(temp);	 
						 sSet.insert(start[1]);
					 }
				 }
			 }
		}	
	}
		// change small moleculer compartmentcontain
		//vector<vector<vector<vector<int>>>>
		//_smallMolecule, _reaction

		/*for(int i=0; i<_scene->_biochemicalReaction[m_pathwayID].size(); i++)
		{
			if(reactionPos[i].x()>-10000)
			{
				for(int j=0; j<_scene->_biochemicalReaction[m_pathwayID][i].size(); j++)
				{
					vector<int> nodes = _scene->_biochemicalReaction[m_pathwayID][i][j];
					for(int k=0; k<nodes.size(); k=k+2)		
					{
						 _smallMolecule
						 _reaction
					}
				}
			}
		}*/
	

	CompartmentContain[CompartmentContain.size()-1].clear();

	vector<vector<vector<int>>> Ranks;
	//record nodes and edges for layout purpos	
	vector<vector<int>> VertexInfo;
	vector<vector<int>> Edge;
	vector<int> iunit;
	vector<vector<int>> vunit(2,iunit);
	vector<vector<vector<int>>> Vertex(complexNum + proteinNum + physicalEntityNum + smallMoleculeNum + DnaNum + reactionNum + EmptyNum, vunit); //store the out and in edges 
	vector<int> temp(2,0);
	vector<QRectF> newPos;//(complexNum + proteinNum + physicalEntityNum + smallMoleculeNum + DnaNum + reactionNum,
	int sid=0;
	//if(toplevel)
	//	sid=1;
	
	for(int i=sid; i<complexPos.size();i++)
	{
		temp[0]='C'; temp[1]=i; newPos.push_back(QRectF(-1000,-1000,complexPos[i].width(),complexPos[i].height()));
	   
		VertexInfo.push_back(temp);
	}
	for(int i=sid; i<proteinPos.size();i++)
	{
		temp[0]='P'; temp[1]=i; newPos.push_back(QRectF(-1000,-1000,proteinPos[i].width(),proteinPos[i].height()));
		VertexInfo.push_back(temp);
	}	
	for(int i=sid; i<smallMoleculePos.size();i++)
	{
		temp[0]='S'; temp[1]=i; newPos.push_back(QRectF(-1000,-1000,smallMoleculePos[i].width(),smallMoleculePos[i].height()));
		VertexInfo.push_back(temp);
	}
	for(int i=sid; i<physicalEntityPos.size();i++)
	{
		temp[0]='E'; temp[1]=i;
		VertexInfo.push_back(temp); newPos.push_back(QRectF(-1000,-1000, physicalEntityPos[i].width(),physicalEntityPos[i].height())); //newPos.push_back(physicalEntityPos[i]);
	}
	for(int i=sid; i<DnaPos.size();i++)
	{
		temp[0]='D'; temp[1]=i;
		VertexInfo.push_back(temp); newPos.push_back(QRectF(-1000,-1000, DnaPos[i].width(), DnaPos[i].height())); //newPos.push_back(physicalEntityPos[i]); 
	}	
	for(int i=sid; i<EmptyPos.size();i++)
	{
		temp[0]='N'; temp[1]=i;	   
		VertexInfo.push_back(temp); newPos.push_back(QRectF(-1000,-1000, EmptyPos[i].width(), EmptyPos[i].height())); //newPos.push_back(physicalEntityPos[i]); 
	}	
	for(int i=sid; i<reactionPos.size();i++)
	{
		temp[0]='R'; temp[1]=i;
		VertexInfo.push_back(temp); newPos.push_back(QRectF(-1000,-1000, reactionPos[i].width(),reactionPos[i].height())); //newPos.push_back(physicalEntityPos[i]); newPos.push_back(reactionPos[i]);
	}
	

	int type, id;
	vector<int> start(4,0), middle(4,0), end(4,0);
	vector<int> ed;
	//set<vector<int>> check; 

	int csize;
	csize=_complex.size();         _complex.resize(csize);
	csize=_Dna.size();             _Dna.resize(csize);
	csize=_entity.size();          _entity.resize(csize);
	csize=_smallMolecule.size();   _smallMolecule.resize(csize);
	csize=_Empty.size();           _Empty.resize(csize);
	
	/*for(int i=0; i<edge.size(); i++)
	{			 
         start[0]=edge[i][0], start[1]=edge[i][1];			 		 
		 middle[0]=edge[i][4]; middle[1]=edge[i][5];
		 end[0]=edge[i][2], end[1]=edge[i][3]; 
		 if(end[0]!='R')		 
	     {
			 ed=start;  ed[2]='M'; ed[3]=-1; 
             _reaction[middle[1]].push_back(ed);  
			 ed=end; ed[2]='A'; ed[3]=1; 
			 _reaction[middle[1]].push_back(ed);       		     
		 }
		 else 
		 {
			 end[1]=edge[i][5];
			 ed=start; ed[2]='R'; ed[3]=-1; 			
			 _reaction[end[1]].push_back(ed);      			 
		 }
	}*/
	
	for(int i=0; i<edge.size(); i++)
	{		
	     start[0]=edge[i][0], start[1]=edge[i][1];	
		 middle[0]=edge[i][4]; middle[1]=edge[i][5]; 
		 end[0]=edge[i][2], end[1]=edge[i][3]; 
         if(end[0]!='R')//keqin cooment out why || drawSmallmolecule) //drawSmallmolecule		 
	     {
			 temp[0] = getVerticeID_2(start[0],start[1]);
			 temp[1] = getVerticeID_2(middle[0],middle[1]);		
		
			 Vertex[temp[0]][0].push_back(Edge.size());
			 Vertex[temp[1]][1].push_back(Edge.size());
			 Edge.push_back(temp);				 

			 ed=middle;  ed[2]='M'; ed[3]=1; 

			 switch(start[0])
			 {
		     case 'C': _complex[start[1]].push_back(ed);   break;
			 case 'D': _Dna[start[1]].push_back(ed);       break;
			 case 'E': _entity[start[1]].push_back(ed);    break;
			 case 'P': _protein[start[1]].push_back(ed);         break;
			 case 'S': _smallMolecule[start[1]].push_back(ed);   break;		
			 case 'N': _Empty[start[1]].push_back(ed);   break;		
		     }			
			 ed=start;  ed[2]='M'; ed[3]=-1; 

			 //_reaction[middle[1]].push_back(ed);  
			
			 temp[0] = getVerticeID_2(middle[0],middle[1]);
			 temp[1] = getVerticeID_2(end[0],end[1]);

						 
			 Vertex[temp[0]][0].push_back(Edge.size());
			 Vertex[temp[1]][1].push_back(Edge.size());
			 Edge.push_back(temp);				 

			 ed=end; ed[2]='A'; ed[3]=1; 
			
			 //_reaction[middle[1]].push_back(ed);       
		    
			 ed=middle;  ed[2]='A';  ed[3]=-1; 
			 switch(end[0])
	         {
				case 'C': _complex[end[1]].push_back(ed);        break;
				case 'D': _Dna[end[1]].push_back(ed);            break;
				case 'E': _entity[end[1]].push_back(ed);         break;
				case 'P': _protein[end[1]].push_back(ed);        break;
				case 'S': _smallMolecule[end[1]].push_back(ed);  break;		
				case 'N': _Empty[end[1]].push_back(ed);           break;			
		     }
		 }
		 else 
		 {
			 temp[0] = getVerticeID_2(start[0],start[1]);
			 if(end[0]=='R')
			    end[1]=edge[i][5];

			 temp[1] = getVerticeID_2(end[0],end[1]);	
			
			 Vertex[temp[0]][0].push_back(Edge.size()); //record edge
			 Vertex[temp[1]][1].push_back(Edge.size());

			 Edge.push_back(temp);	

			 ed=end;  ed[2]='R'; ed[3]=1; 
			 switch(start[0])
			 {
		     case 'C': _complex[start[1]].push_back(ed);   break;
			 case 'D': _Dna[start[1]].push_back(ed);       break;
			 case 'E': _entity[start[1]].push_back(ed);    break;
			 case 'P': _protein[start[1]].push_back(ed);         break;
			 case 'S': _smallMolecule[start[1]].push_back(ed);   break;		
			 case 'N': _Empty[start[1]].push_back(ed);   break;		
		     }
			 ed=start; ed[2]='R'; ed[3]=-1; 			
			 //_reaction[end[1]].push_back(ed);      			 
		 }
	}
	
	for(int i=0; i<VertexInfo.size(); i++)
	{
		VertexInfo[i].push_back(-1); VertexInfo[i].push_back(-1);
	}
	int visize=VertexInfo.size();
	for(int i=visize; i<Vertex.size(); i++)
	{
		vector<int> temp(4,-1);
		VertexInfo.push_back(temp);
	}
	
	
	for(int i=0; i<_scene->_compartmentName[m_pathwayID].size();i++)
    {		
		for(int j=0; j<CompartmentContain[i].size(); j++) 
		{
			set<vector<int>> Clist=CompartmentContain[i];
			for(set<vector<int>>::iterator it=Clist.begin(); it!=Clist.end(); it++)
			{
				vector<int> node=*it;			
				int id = getVerticeID_2(node[0],node[1]);
				VertexInfo[id][3]=i;							      
			}							
		}		
	}
	int vsize=Vertex.size();
	//for the whole data
	//for the whole data
	
	set<vector<int>> newCompart;
	//compartmentPos.push_back(QRectF(0, 0, 1, 1));//left
	vector<QString> ttemp;
	/*if(_scene->_compartmentName[m_pathwayID][_scene->_compartmentName[m_pathwayID].size()-1]!="whole")
	{
		ttemp.push_back("whole");
	    _scene->_compartmentName[m_pathwayID].push_back(ttemp);			
	}*/
	for(int i=0; i<VertexInfo.size(); i++)
	{
		vector<int> temp;
		temp.push_back(VertexInfo[i][0]); temp.push_back(VertexInfo[i][1]);
	    newCompart.insert(temp);
	}
	CompartmentContain.push_back(newCompart);

	 //&newPos, vector<set<vector<int>>> &CompartmentContain
	
	if(whole)
	{
		for(int i=0; i<Vertex.size(); i++)
		{
			VertexInfo[i][3]=CompartmentContain.size()-1;
		}
		Ranks.push_back(subMap(Vertex, VertexInfo, Edge, newPos, CompartmentContain, CompartmentContain.size()-1));//_scene->CompartmentContain[m_pathwayID].size()-1
	    //for the whole graph in "whole"
	}
	else 
	{
		for(int i=0; i<CompartmentContain.size(); i++)
		{
			//itv=subMap(Vertex, VertexInfo, Edge, newPos, i);//_scene->CompartmentContain[m_pathwayID].size()-1	
			/*QString fname="before_subMap()"+QString::number(i);
			QFile file1(fname);
	        file1.open(QIODevice::WriteOnly | QIODevice::Truncate);
	        file1.close();*/
			Ranks.push_back(subMap(Vertex, VertexInfo, Edge, newPos, CompartmentContain, i));//_scene->CompartmentContain[m_pathwayID].size()-1
		}
	}	
 	 newPos.resize(vsize);
	_VertexInfo=VertexInfo;
	_Edge=Edge;
	_Vertex=Vertex; //store the out and in edges 	
	
	_newPos=newPos;	
	CompartmentContain.resize(_scene->CompartmentContain[m_pathwayID].size());
	putReactionToCompartment(_reaction, cContain);	
	cContain.resize(_scene->CompartmentContain[m_pathwayID].size());
	_scene->CompartmentContain[m_pathwayID]=cContain;
	//_scene->CompartmentContain[m_pathwayID]=CompartmentContain;
	
	VertexInfo.clear(); 	Edge.clear(); 	iunit.clear(); 	vunit.clear(); 	Vertex.clear();//store the out and in edges 
	temp.clear();
	newPos.clear();

	return Ranks;
}




vector<vector<int>> PathBubble1::EdgetoNode(vector<vector<int>> edge, vector<vector<vector<int>>> &compartment, bool rflag)
{
	//get tree for whole
	vector<vector<int>> Rank;
	vector<int> temp(4,0), etemp(5,0);
	vector<vector<int>> VertexInfo;
	vector<QRectF> newPos; 
	int di=_scene->_compartmentName[m_pathwayID].size()-1;
	//vector<vector<int>> edge;
	for(int i=0; i<reactionNum;i++)
	{
	   temp[0]='R'; temp[1]=i; temp[2]=-1; temp[3]=-1;
	   VertexInfo.push_back(temp); newPos.push_back(QRectF(-1000,-1000, reactionPos[i].width(),reactionPos[i].height())); //newPos.push_back(physicalEntityPos[i]); newPos.push_back(reactionPos[i]);
	}
	
	//record nodes and edges for layout purpos		
	vector<vector<int>> Edge;
	vector<int> iunit;
	vector<vector<int>> vunit(2,iunit);
	vector<vector<vector<int>>> Vertex(_scene->_compartmentName[m_pathwayID].size()+reactionNum, vunit); //store the out and in edges 
		
	int type, id;
	vector<int> start(4,0), middle(4,0), end(4,0);
	vector<int> start1(2,0), middle1(2,0), end1(2,0);
	vector<int> ed;

	for(int i=0; i<_scene->_compartmentName[m_pathwayID].size();i++)
    {	
		if(i<_scene->CompartmentContain[m_pathwayID].size())
		{
			
			temp[0]='C';  temp[1]=i;  newPos.push_back(QRectF( -1, -1, compartmentPos[i].width(), compartmentPos[i].height()));
			VertexInfo.push_back(temp);
			VertexInfo[i][3]=i;	
	    }
	}
	//only 'C' and 'R' should be in vertex list
	vector<bool> containFlag;
	containFlag.resize(edge.size());
	
	for(int i=0; i<edge.size();i++)
	{
		containFlag[i]=false;
		
		start1[0]=edge[i][0];  start1[1]=edge[i][1];			 		 
		middle1[0]=edge[i][4]; middle1[1]=edge[i][5]; 
		end1[0]=edge[i][2];    end1[1]=edge[i][3];  
				
		int starti, endi;
		if(end1[0]!='R')	
		{
			for(int j=0; j<_scene->_compartmentName[m_pathwayID].size();j++)
		    { 
				if(j<_scene->CompartmentContain[m_pathwayID].size())
				{
					if(_scene->CompartmentContain[m_pathwayID][j].find(end1) != _scene->CompartmentContain[m_pathwayID][j].end())
					{
						edge[i][2]='C';  edge[i][3]=j;  endi=j;
						//containFlag[i] = true;
					}
				}
			}		
		}		
		for(int j=0; j<_scene->_compartmentName[m_pathwayID].size();j++)
		{ 
			if(j<_scene->CompartmentContain[m_pathwayID].size())
			{
				if(_scene->CompartmentContain[m_pathwayID][j].find(start1)!=_scene->CompartmentContain[m_pathwayID][j].end())
				{
					 edge[i][0]='C';    edge[i][1]=j;  starti=j;
					 containFlag[i] = true;
				}
			}
		}
		
		if(endi==starti)
           edge[i][0]='R';
	}
	//there still some case where some pathway component do not have related name/compartment information, ignore them right now	
	//for example, Signaling by EGFR path appear in two files: 190236 and 177929		
	//for in 190236, complex 193 miss related information

	set<vector<int>> edgeSet;
	//
	for(int i=0; i<edge.size(); i++)
	{
		 start[0]=edge[i][0], start[1]=edge[i][1];	
		 middle[0]=edge[i][4]; middle[1]=edge[i][5]; 
		 end[0]=edge[i][2], end[1]=edge[i][3]; 

		 if(end[0]=='R')
			 end[1]=edge[i][5];

		 if((end[0]!='R'&& end[0]!='C')||(start[0]!='R'&& start[0]!='C')||(middle[0]!='R'&& middle[0]!='C'))      //keqin: may have bug, all node type right now should be 'C' and 'R'
		  {
			  containFlag[i]=false;
		 }
		 	
		 if(containFlag[i] && end[0]!='R' && start[0]!='R')		 
	     {
			 temp[0] = start[1];
			 temp[1] = middle[1]+di;	

			 etemp=temp; 
			 etemp.push_back(1);	//weight		 

             if(edgeSet.find(temp)==edgeSet.end())
			 {
				 Vertex[temp[0]][0].push_back(Edge.size());
				 Vertex[temp[1]][1].push_back(Edge.size());
				 Edge.push_back(etemp);
				 edgeSet.insert(temp);
			 }
			 else
			 {
				  int eid=-1;
			      for(int k=0; k<Edge.size(); k++)
				  {
				        if(Edge[k][0]==etemp[0]||Edge[k][1]==etemp[1])
						{
							eid=k;
							break;
						}
				  }
				  if(eid!=-1)
				  {
					  Vertex[temp[0]][0].push_back(Edge.size());
				      Vertex[temp[1]][1].push_back(Edge.size());
					  Edge[eid][4]=Edge[eid][4]+1;
				  }
			 }

			 ed=middle;  ed[2]='M'; ed[3]=1; 

			 switch(start[0])
			 {
				 case 'C': _compartment[start[1]].push_back(ed);   break;
		     }			
			 ed=start;  ed[2]='M'; ed[3]=-1; 

			 temp[0] = middle[1]+di;
			 temp[1] = end[1];
		
			 etemp=temp; 
			 etemp.push_back(1);	//weight	

			 if(edgeSet.find(temp)==edgeSet.end())
			 {
				 Vertex[temp[0]][0].push_back(Edge.size());
				 Vertex[temp[1]][1].push_back(Edge.size());				
				 Edge.push_back(etemp);
				 edgeSet.insert(temp);
			 }
			 else
			 {
				  int eid=-1;
			      for(int k=0; k<Edge.size(); k++)
				  {
				        if(Edge[k][0]==etemp[0]||Edge[k][1]==etemp[1])
						{
							eid=k;
							break;
						}
				  }
				  if(eid!=-1)
				  {
				
					  Edge[eid][4]=Edge[eid][4]+1;
				  }
			 }

			 ed=end; ed[2]='A'; ed[3]=1; 
		
			 //_reaction[middle[1]].push_back(ed);   
			 ed=middle;  ed[2]='A';  ed[3]=-1; 
			 switch(end[0])
	         {
				case 'C': _compartment[end[1]].push_back(ed);        break;				
		     }
		 }
		 else if(containFlag[i] && start[0]!='R')
		 {
			 temp[0] = start[1];
			 temp[1] = end[1]+di;	
			
			 etemp=temp; 
			 etemp.push_back(1);	//weight
			
			 if(edgeSet.find(temp)==edgeSet.end())
			 {
				 Vertex[temp[0]][0].push_back(Edge.size());
				 Vertex[temp[1]][1].push_back(Edge.size());
				 Edge.push_back(etemp);
				 edgeSet.insert(temp);
			 }
			 else
			 {
				  int eid=-1;
			      for(int k=0; k<Edge.size(); k++)
				  {
				        if(Edge[k][0]==etemp[0]||Edge[k][1]==etemp[1])
						{
							eid=k;
							break;
						}
				  }
				  if(eid!=-1)
				  {
					  
					  Edge[eid][4]=Edge[eid][4]+1;
				  }
			 }

			 ed=end;  ed[2]='R'; ed[3]=1; 
			 switch(start[0])
			 {
				 case 'C': _compartment[start[1]].push_back(ed);   break;			
		     }
			 ed=start; ed[2]='R'; ed[3]=-1; 
		 }
	}
	
	for(int i=0; i<Vertex.size(); i++)
	{
		VertexInfo[i][3]=-1;
	}
	int vsize=Vertex.size();
		
	Rank=subMap(Vertex, VertexInfo, Edge, newPos, _scene->CompartmentContain[m_pathwayID], -1);//_scene->CompartmentContain[m_pathwayID].size()-1
	//straighten the pos of the reaction when it is 
	//vector<vector<int>> PathBubble1::subMap(vector<vector<vector<int>>> Vertex, vector<vector<int>> VertexInfo, vector<vector<int>> Edge, vector<QRectF> &newPos, vector<set<vector<int>>> &CompartmentContain, int roomID)
	
	newPos.resize(vsize);
	_VertexInfo=VertexInfo;
	_Edge=Edge;
	_Vertex=Vertex; //store the out and in edges 	
	_newCompartmentPos=newPos;
	_newCompartmentPos.resize(compartmentPos.size());
     for(int i=0; i<_newCompartmentPos.size(); i++)
	 {
	     _newCompartmentPos[i].setWidth(0.1);  
		 _newCompartmentPos[i].setHeight(0.1);	 
	 }
	 containFlag.clear();
	 //_scene->CompartmentContain[m_pathwayID]=CompartmentContain;
	 return Rank;
}



bool PathBubble1::PathtoRank(vector<vector<vector<int>>> Vertex, vector<vector<int>> &VertexInfo, vector<vector<int>> Edge, vector<QRectF> &newPos, vector<set<vector<int>>> &CompartmentContain, vector<vector<int>> &Rank, vector<int> &path, int &mini, int roomID, bool useBoost)
{//use boost
	vector<vector<int>> tempR;
    int count=0;
	vector<int> maxlength, tempLength;
	int max,maxid,l,Mini;
	vector<int> iunit;
	
	if(useBoost)
	if(Edge.size()!=0)
	{  
		//make Edge only contain what is for the compartment
		Rank = _scene->_wholeGraph->getRank(Edge, Vertex, VertexInfo, roomID);
		for(int i=0; i<Rank.size(); i++)
		{			
			float inv=0.9/Rank[i].size();
			for(int j=0; j<Rank[i].size(); j++) // for(int j=1(?)
			{			
				float y= (float)(i+1)/(Rank.size()+1);		
				newPos[Rank[i][j]].moveCenter(QPointF(j+0.5,y));//+0.025
				VertexInfo[Rank[i][j]][2]=i;  //record rank			
			}
		}
		if(Rank.size()==0)
			return false;
		else return true;
	}
	//vector<QPointF> newpos;
	//int count=0;
	mini=10000;
	while(true)
	{ 
		Mini=mini;
		vector<int> length(Vertex.size(),-1);
		maxid=-1;
		max=-100000;
		l=0;
		for(int i=0; i<Vertex.size(); i++)
		{		
			//tempLength=length;
			if(VertexInfo[i][2]<=-1 && (roomID==-1||VertexInfo[i][3]==roomID) && Vertex[i][1].empty() && !Vertex[i][0].empty())
			{	
				l=findLongestPath(i, length, Edge, Vertex, VertexInfo, roomID);	
				if(l>max)
				{
					max=l;	
					maxid=i;
					maxlength=length;
				}		
			}
		}
		if(maxid==-1)
		{		
			for(int i=0; i<Vertex.size(); i++)
			{					    
				tempLength=length;
				if(VertexInfo[i][2]<=-1 && (roomID==-1||VertexInfo[i][3]==roomID) && !Vertex[i][0].empty())
				{						
					l=findLongestPath(i, length, Edge, Vertex, VertexInfo, roomID);	
					if(l>max)
					{
						max=l;	
						maxid=i;
						maxlength=length;
					}		
				}					
			}				
		}
		if(maxid==-1) //keqin single-path
			return false;	
			//break;
		
		//mark the longest path with rank
		
		markLongestPath(maxid, VertexInfo, path, length, Edge, Vertex);
		for(int i=0; i<path.size(); i++)
			VertexInfo[path[i]][2]=i+200;

	
		//find the tree linked to the longest path and find the rank of every node linked to the path
		for(int i=0; i<path.size(); i++)
		{
			 findRank_up(path[i], i, VertexInfo, length, Edge, Vertex, roomID);
		}	

		for(int i=0; i<path.size(); i++)
		{
			 findRank_down(path[i], i, VertexInfo, length, Edge, Vertex, roomID);
		}	
	
		for(int i=0; i<VertexInfo.size(); i++)
		{
			if(mini>VertexInfo[i][2] && VertexInfo[i][2]!=-1)
				mini=VertexInfo[i][2];
		}
		if(mini!=Mini)
		   mini=mini-200;
	
		for(int i=0; i<VertexInfo.size(); i++)
		{
			if(VertexInfo[i][2]<200)//+mini)
				VertexInfo[i][2]=-1;
			if(VertexInfo[i][2]!=-1)
				VertexInfo[i][2]=VertexInfo[i][2]-200-mini;		
		}	
		//if(roomID>=0)
		break;
	}
	
    //give the first order of each rank
	//iunit.push_back(-1);
	Rank.resize(path.size()-mini,iunit);
	for(int i=0; i<Vertex.size(); i++)
	{	
	    if(VertexInfo[i][2]>=0 && VertexInfo[i][2]<(path.size()-mini) )
		{
			Rank[VertexInfo[i][2]].push_back(i);
		}		
	}	
    //assign the first path location
	for(int i=0; i<path.size(); i++)
	{
		//newPos[path[i]].setX(0); //order
		//newPos[path[i]].setY((float)VertexInfo[path[i]][2]/(path.size()+2-mini)+0.05);
		float y= (float)VertexInfo[path[i]][2]/(path.size()-mini)*0.95+0.1;		
		newPos[path[i]].moveCenter(QPointF(0,y)); //order//+0.025		
		
	}
	
    for(int i=0; i<Rank.size(); i++)
	{			
		float inv=0.9/Rank[i].size();
	    for(int j=0; j<Rank[i].size(); j++) // for(int j=1(?)
		{			
			float y= (float)(i+1)/(path.size()-mini+1);
			newPos[Rank[i][j]].moveCenter(QPointF(j+0.5,y));//+0.025
			VertexInfo[Rank[i][j]][2]=i;  //record rank			
		}
	}
	return true;
}


vector<vector<int>> PathBubble1::subMap(vector<vector<vector<int>>> Vertex, vector<vector<int>> VertexInfo, vector<vector<int>> Edge, vector<QRectF> &newPos, vector<set<vector<int>>> &CompartmentContain, int roomID)
{  //Vertex: record edges
	vector<vector<int>> tempR;  
	if(roomID!=-1 && CompartmentContain[roomID].size()<1)
	{
		//CompartmentPos[roomID].setWidth(0); CompartmentPos[roomID].setHeight(0);
		return tempR;
	}
	vector<int> iunit;
	vector<int> temp(2,0);
	vector<vector<int>> vunit(2,iunit);
	vector<int> path;
	int mini;
	vector<vector<int>> Rank;

	if(Edge.empty())
	{
	    Rank.resize(1);
		for(int i=0; i<_scene->_compartmentName[m_pathwayID].size();i++)
		{		
			if(CompartmentContain[i].size()>0)
			   Rank[0].push_back(i);
		}
		for(int i=0; i<Rank.size(); i++)
		{			
			float inv=0.9/Rank[i].size();
			for(int j=0; j<Rank[i].size(); j++) // for(int j=1(?)
			{			
				float y= 0;				
				newPos[Rank[i][j]].moveCenter(QPointF(j+0.5,y));//+0.025
				VertexInfo[Rank[i][j]][2]=i;  //record rank			
			}
		}
		return Rank;
	}	
	else if(!PathtoRank(Vertex, VertexInfo, Edge, newPos, CompartmentContain, Rank, path, mini, roomID, true))
		return tempR;

	/*
	else if(!PathtoRank(Vertex, VertexInfo, Edge, newPos, CompartmentContain, Rank, path, mini, roomID))
		return tempR;
	*/
	
	//add dummy node to graph
	int EdgeNum=Edge.size();
	int nodeNum=VertexInfo.size();
	vector<vector<int>> polyEdge;
	for(int i=0; i<EdgeNum; i++)
	{
	   int vid0=Edge[i][0],vid1=Edge[i][1];
	   if(newPos[vid0].center().x()>=-999.99&&newPos[vid1].center().x()>=-999.99)
	   {	
		   vector<int> tedge;	
		   tedge.push_back(vid0);
		  if(VertexInfo[vid1][2]-VertexInfo[vid0][2]>1)
		  {			  
			  int eid=Edge.size(),vid=Vertex.size();
			  float x0,y0,dx,dy,x,y;
			  x0=newPos[vid0].center().x(); y0=newPos[vid0].center().y();
			  dx=(newPos[vid1].center().x()-x0)/(VertexInfo[vid1][2]-VertexInfo[vid0][2]);
			  dy=(newPos[vid1].center().y()-y0)/(VertexInfo[vid1][2]-VertexInfo[vid0][2]);
			  if(dy<-10 || (VertexInfo[vid1][2]-VertexInfo[vid0][2])==0)
				  dy=dy;
			  x=x0+dx, y=y0+dy;

			  for(int k=0; k<Vertex[vid1][1].size(); k++)
			  {
			      if(Vertex[vid1][1][k]==i)
				  {   
					  Vertex[vid1][1][k]=eid+VertexInfo[vid1][2]-VertexInfo[vid0][2]-2;
				      break;
				  }
			  }	
			  Edge[i][1]=vid; 
		      for(int j=0; j<VertexInfo[vid1][2]-VertexInfo[vid0][2]-1; j++)
			  {
				    Vertex.push_back(vunit);  	Vertex[vid][0].push_back(eid);	Vertex[vid][1].push_back(j!=0?(eid-1):i);  
                    
					VertexInfo.push_back(VertexInfo[0]); 
					VertexInfo[vid][0]='R';  VertexInfo[vid][1]= reactionNum + vid-nodeNum; VertexInfo[vid][2]=VertexInfo[vid0][2]+j+1;
			        
					newPos.push_back(QRectF(-1000,-1000,0,0));  //newPos[vid].setX(x); newPos[vid].setY(y);
					newPos[vid].moveCenter(QPointF(x,y));
					
					Edge.push_back(temp); Edge[eid][0]=vid; Edge[eid][1]=(j==VertexInfo[vid1][2]-VertexInfo[vid0][2]-2)?vid1:vid+1;
					
					if(VertexInfo[vid][2]>=Rank.size())
						Rank.resize(VertexInfo[vid][2]+1);
					Rank[VertexInfo[vid][2]].push_back(vid); //keqin bug? 

					tedge.push_back(vid);
					eid++; vid++; x=x+dx; y=y+dy;
			  }		      
		  }
		   tedge.push_back(vid1);
		   polyEdge.push_back(tedge);
	   }
	}

	//reOrganizeSubMap(Rank, newPos, Vertex, VertexInfo, Edge);
	
	//float ppx2=newPos[81].x();
	for(int m=0; m<10; m++)
	{
	//downward sweep
		for(int i=2; i<Rank.size(); i++)
		{			
			for(int j=0; j<Rank[i].size(); j++) 
			{		
				int cid=Rank[i][j]; 
				vector<int> plist;
				int length=Vertex[cid][1].size(); //up
				float sum=0;
				for(int k=0; k<Vertex[cid][1].size(); k++)
				{
					 int eid = Vertex[cid][1][k];
					 int pid = Edge[eid][0];
					 plist.push_back(pid);
					 sum = sum + newPos[pid].center().x();
				}
				if(length!=0)
				   //newPos[cid].setX(sum/length);
				   newPos[cid].moveCenter(QPointF(sum/length,newPos[cid].center().y()));
			}
		}	

		for(int i=0; i<Rank.size(); i++)
		{	
			list<vector<float>> Xlist;
			for(int j=0; j<Rank[i].size(); j++) 
			{
				vector<float> temp;
				temp.push_back(newPos[Rank[i][j]].center().x());  temp.push_back(Rank[i][j]);
				Xlist.push_back(temp);					
			}		
			Xlist.sort();
			int count=0;
			for(list<vector<float>>::iterator it=Xlist.begin(); it!=Xlist.end();it++) 
			{
				vector<float> temp=*it;
				//newPos[(int)temp[1]].setX(count);					
				newPos[(int)temp[1]].moveCenter(QPointF(count,newPos[(int)temp[1]].center().y()));					
				//newPos[Rank[i][j]].setX(newPos[Rank[i][j]].x()/(Rank[i].size()+1)*0.9+0.05);					
				count++;
			}
		}
		//upward sweep
		for(int i=Rank.size()-3; i>=0; i--)
		{			
			for(int j=0; j<Rank[i].size(); j++) 
			{		
				int pid=Rank[i][j]; 
				//vector<int> clist;
				int length=Vertex[pid][0].size(); //up
				float sum=0;
				for(int k=0; k<Vertex[pid][0].size(); k++)
				{
					 int eid = Vertex[pid][0][k];
					 int cid = Edge[eid][1];
					 //plist.push_back(pid);3
					 sum = sum + newPos[cid].center().x();
				}
				if(length!=0)
				   //newPos[pid].setX(sum/length);
				   newPos[pid].moveCenter(QPointF(sum/length,newPos[pid].center().y()));
			}
		}	
		for(int i=0; i<Rank.size(); i++)
		{	
			list<vector<float>> Xlist;
			for(int j=0; j<Rank[i].size(); j++) 
			{
				vector<float> temp;
				temp.push_back(newPos[Rank[i][j]].center().x());  temp.push_back(Rank[i][j]);
				Xlist.push_back(temp);					
			}		
			Xlist.sort();
			int count=0;
			for(list<vector<float>>::iterator it=Xlist.begin(); it!=Xlist.end();it++) 
			{
				vector<float> temp=*it;
				newPos[(int)temp[1]].moveCenter(QPointF(count,newPos[(int)temp[1]].center().y()));											
				count++;
			}
		}
	}

	//convert to location
	int maxwidth=-10000,maxi;	

	for(int i=0; i<Rank.size(); i++)
	{		
		int value=Rank[i].size();
		if(maxwidth<value)
			maxwidth=value, maxi=i;		
	}
	for(int i=0; i<Rank.size(); i++)
	{		
		set<vector<float>> Rlist;
		for(int j=0; j<Rank[i].size(); j++) 
		{
			vector<float> temp;
			temp.push_back(newPos[Rank[i][j]].center().x()); temp.push_back(Rank[i][j]);
		    Rlist.insert(temp);
		}
		Rank[i].clear();
		for(set<vector<float>>::iterator it=Rlist.begin(); it!=Rlist.end(); it++) 
		{
			vector<float> temp=*it;
			//if(VertexInfo[temp[1]][3]==roomID)
			   Rank[i].push_back(temp[1]); 
		}
	}

	float itv=0.9/maxwidth;
	vector<float> Witem(maxwidth,0),WArray1;
	vector<vector<float>> WArray(Rank.size(), Witem);
	for(int i=0; i<Rank.size(); i++)
	{		
		int id=maxwidth/2;
		if(Rank[i].size()>0)
		for(int j= Rank[i].size()/2; j>=0; j--) 
		{   
			if(VertexInfo[Rank[i][j]][0]=='S')
			    WArray[i][id]=0.8;
			else WArray[i][id]=newPos[Rank[i][j]].width();
			id--;
		}
		id=maxwidth/2+1;
		if(Rank[i].size()>1)
		for(int j= Rank[i].size()/2+1; j<Rank[i].size(); j++) 
		{			
			if(VertexInfo[Rank[i][j]][0]=='S')
			    WArray[i][id]=0.8;
			else WArray[i][id]=newPos[Rank[i][j]].width();
			id++;		
		}	
	}
		
	for(int j=0; j<maxwidth; j++)
	{
	   float wmax=-10000;
       for(int i= 0; i<Rank.size(); i++) 
	   {
 	        if(wmax<WArray[i][j])
				wmax=WArray[i][j];
	   }
	   WArray[0][j]=wmax;
	}
	WArray1=WArray[0];
	//adjust the width
	float sw=0,maxW;
	for(int j=0; j<maxwidth; j++)
	{
	   float temp=sw;
	   sw=sw+WArray[0][j];	   
	   WArray[0][j]=temp+WArray[0][j]/2.0;	   	   
	}
    maxW=sw;
	
	sw=0;
	for(int j=0; j<maxwidth; j++)
	{
	   WArray[0][j]=WArray[0][j]/maxW;	
	   WArray1[j]=WArray1[j]/maxW*0.9;
	}
		
	
	//move center according to their space
	float hitv=1.0/Rank.size();
	
	for(int i=0; i<Rank.size(); i++)
	{		
		int id=maxwidth/2;
		float x,y;
		if(Rank[i].size()>0)
		for(int j= Rank[i].size()/2; j>=0; j--) 
		{   
			y=newPos[Rank[i][j]].y();	
			if(VertexInfo[Rank[i][j]][0]!='R')
			{
				//if(newPos[Rank[i][j]].x()<-0.999)
				{
					newPos[Rank[i][j]].setWidth(WArray1[id]);
					newPos[Rank[i][j]].setHeight(hitv);
				}				
			}
			newPos[Rank[i][j]].moveCenter(QPointF(WArray[0][id],y)); //newPos[Rank[i][j]].width();
			id--;
		}
		id=maxwidth/2+1;
		if(Rank[i].size()>1)
		for(int j= Rank[i].size()/2+1; j<Rank[i].size(); j++) 
		{
			y=newPos[Rank[i][j]].y();
			if(VertexInfo[Rank[i][j]][0]!='R')
			{
				//if(newPos[Rank[i][j]].x()<-0.999)
				{
					newPos[Rank[i][j]].setWidth(WArray1[id]);
					newPos[Rank[i][j]].setHeight(hitv);
				}				
			}
			newPos[Rank[i][j]].moveCenter(QPointF(WArray[0][id],y)); 			
			id++;		
		}	
	}
		
    //arrange compartment
	/*if(roomID==-1)
	{
		for(int i=0; i<_compartment.size()-1; i++)
		{
		     newPos[i].setWidth(compartmentPos[i].width());	newPos[i].setHeight(compartmentPos[i].height());
		}		

		itv=1;
		float dy=1;//(float)1/(Rank.size()-1)*0.95;//*0.95+0.025)
		float sy = 0;
		for(int i=0; i<Rank.size(); i++)
		{  
			float maxdy=0;
			for(int j= 0; j<Rank[i].size(); j++) 
			{
				if(maxdy<newPos[Rank[i][j]].height())
					maxdy=newPos[Rank[i][j]].height();
			}	
			if(maxdy>dy)
                 sy=sy+maxdy;
			else sy=sy+dy;

			for(int j= 0; j<Rank[i].size(); j++) 
			   newPos[Rank[i][j]].moveCenter(QPointF(newPos[Rank[i][j]].center().x(), sy));		
		}

		for(int i=0; i<Rank.size(); i++)
		{  
			for(int j= 0; j<Rank[i].size(); j++) 
			   newPos[Rank[i][j]].moveCenter(QPointF(newPos[Rank[i][j]].center().x(), newPos[Rank[i][j]].center().y()/sy *0.95 + 0.025 ));				
		}		
	}
	*/
	
	for(int i=0; i<polyEdge.size(); i++)
	{
			vector<QPointF> etemp;
			for(int j=0; j<polyEdge[i].size(); j++)
			{
          		etemp.push_back(newPos[polyEdge[i][j]].center());	
			}
			_polyEdge.push_back(etemp);	
	}

	for(int i=0; i<VertexInfo.size(); i++)
	{
		if(newPos[i].center().y()<-0.5 && newPos[i].x()>-990) //order		   	
		   newPos[i].moveCenter(QPointF(-1,newPos[i].center().y()));	
	}

	
	return Rank;	
}

void PathBubble1::reOrganizeSubMap(vector<vector<int>> &Rank, vector<QRectF> &newPos, vector<vector<vector<int>>> Vertex, vector<vector<int>> VertexInfo, vector<vector<int>> Edge)
{
	//check if there rank node connnect to others
	/*vector<QRectF> rankRect; //newPos[vid].
	for(int i=0; i<Rank.size(); i++)
	{		
		float minx=100000, maxx=-100000, miny=100000, maxy=-100000;
		for(int j=0; j<Rank[i].size(); j++) 
		{
			 int id=Rank[i][j];
		     float x0=newPos[id].x(), x1=newPos[id].x()+newPos[id].width(), y0=newPos[id].y(), y1=newPos[id].y()+newPos[id].height();
			 if(x0<minx) minx=x0;	 if(y0<miny) miny=y0;
			 if(x1>maxx) maxx=x1;	 if(y1>maxy) maxy=y1;
			 rankRect.push_back(QRectF(minx, miny, maxx-minx, maxy-miny));
		}
	}

	vector<vector<int>> newRank;
	for(int i=0; i<Rank.size(); i++)
	{			
		for(int j=0; j<Rank[i].size(); j++) // for(int j=1(?)
		{			
			int id=Rank[i][j];
	        if(!inRanks(id, Rank, Vertex))
			{
			
			}
			//VertexInfo[Rank[i][j]][2]=i;  //record rank			
		}
	}*/
}

bool PathBubble1::inRanks(int id, vector<vector<int>> Rank, vector<vector<vector<int>>> Vertex)
{
	for(int i=0; i<Vertex[id].size(); i++)
	{			
		for(int j=0; j<Vertex[id][i].size(); j++) 
		{
			int vid=Vertex[id][i][j];
			for(int k=0; k<Rank.size(); k++)
			{			
				for(int l=0; l<Rank[i].size(); l++) 
				{
				   if(Rank[i][j]==id)
				   {
				      return true;
				   }
				}
			}
		}
	}
	return false;
}

void PathBubble1::findRank_up_backup(int id, int rank, vector<vector<int>> &VertexInfo, vector<int> length, vector<vector<int>> Edge, vector<vector<vector<int>>> Vertex, int roomID)
{//find rank by searching up from a node in the longest path


	if(VertexInfo[id][0]!='R' && VertexInfo[id][0]!='S' && VertexInfo[id][3]!=roomID)
	{	
		return;
	}
	if(VertexInfo[id][2]==-1)
	{
		VertexInfo[id][2]=rank;			
	}
	else if(VertexInfo[id][2]>rank)
	{
		VertexInfo[id][2]=rank;	
		return;
	}
	//if(Vertex[id][1].size()==0)
	//	return;
	for(int i=0; i<Vertex[id][1].size(); i++)
	{
		int eid=Vertex[id][1][i];
	    int nid=Edge[eid][0];
		if(VertexInfo[nid][2]==-1)
		     findRank_up_backup(nid, rank-1, VertexInfo, length, Edge, Vertex, roomID);
	}

	for(int i=0; i<Vertex[id][0].size(); i++)
	{
		int eid=Vertex[id][0][i];
	    int nid=Edge[eid][1];
		if(VertexInfo[nid][2]==-1) 
		     findRank_down_backup(nid, rank+1, VertexInfo, length, Edge, Vertex, roomID);
	}
}

void PathBubble1::findRank_down_backup(int id, int rank, vector<vector<int>> &VertexInfo, vector<int> length, vector<vector<int>> Edge, vector<vector<vector<int>>> Vertex, int roomID)
{//find rank by searching down from a node in the longest path

	

	if(VertexInfo[id][0]!='R' && VertexInfo[id][0]!='S' && VertexInfo[id][3]!=roomID)
	{	
		return;
	}
	if(VertexInfo[id][2]==-1)
	{
		VertexInfo[id][2]=rank;	
		
	}
	else if(VertexInfo[id][2]<rank)
	{
		VertexInfo[id][2]=rank;	
		return;
	}
	//if(Vertex[id][0].size()==0)
	//	return;
	
	for(int i=0; i<Vertex[id][0].size(); i++)
	{
		int eid=Vertex[id][0][i];
	    int nid=Edge[eid][1];
		if(VertexInfo[nid][2]==-1)
		     findRank_down_backup(nid, rank+1, VertexInfo, length, Edge, Vertex, roomID);
	}
	for(int i=0; i<Vertex[id][1].size(); i++)
	{
		int eid=Vertex[id][1][i];
	    int nid=Edge[eid][0];
		if(VertexInfo[nid][2]==-1)
		     findRank_up_backup(nid, rank-1, VertexInfo, length, Edge, Vertex, roomID);
	}
}



void PathBubble1::findRank_up(int id, int rank, vector<vector<int>> &VertexInfo, vector<int> length, vector<vector<int>> Edge, vector<vector<vector<int>>> Vertex, int roomID)
{//find rank by searching up from a node in the longest path
  

	if( VertexInfo[id][0]!='R' && VertexInfo[id][0]!='S' && VertexInfo[id][3]!=roomID)
	{	
		return;
	}
	if(VertexInfo[id][2]==-1)
	{
		//if(VertexInfo[id][3]==roomID)
		VertexInfo[id][2]=rank+200;			
	}
	else if(VertexInfo[id][2]>rank+200)
	{
		//if(VertexInfo[id][3]==roomID)
		VertexInfo[id][2]=rank+200;	
		return;
	}
	//if(Vertex[id][1].size()==0)
	//	return;
	for(int i=0; i<Vertex[id][1].size(); i++)
	{
		int eid=Vertex[id][1][i];
	    int nid=Edge[eid][0];
		if(VertexInfo[nid][2]==-1 && VertexInfo[nid][3]==roomID)
		     findRank_up(nid, rank-1, VertexInfo, length, Edge, Vertex, roomID);
	}

	for(int i=0; i<Vertex[id][0].size(); i++)
	{
		int eid=Vertex[id][0][i];
	    int nid=Edge[eid][1];
		if(VertexInfo[nid][2]==-1 && VertexInfo[nid][3]==roomID)
		     findRank_down(nid, rank+1, VertexInfo, length, Edge, Vertex, roomID);
	}
}

void PathBubble1::findRank_down(int id, int rank, vector<vector<int>> &VertexInfo, vector<int> length, vector<vector<int>> Edge, vector<vector<vector<int>>> Vertex, int roomID)
{//find rank by searching down from a node in the longest path
	
	if(VertexInfo[id][0]!='R' && VertexInfo[id][0]!='S' && VertexInfo[id][3]!=roomID)
	{	
		return;
	}

	if(VertexInfo[id][2]==-1)
	{
		if(VertexInfo[id][3]==roomID)
		   VertexInfo[id][2]=rank+200;				
	}
	else if(VertexInfo[id][2]<rank+200)
	{
		if(VertexInfo[id][3]==roomID)
		  VertexInfo[id][2]=rank+200;			
		return;
	}
		
	for(int i=0; i<Vertex[id][0].size(); i++)
	{
		int eid=Vertex[id][0][i];
	    int nid=Edge[eid][1];
		if(VertexInfo[nid][2]==-1 && VertexInfo[nid][3]==roomID)
		     findRank_down(nid, rank+1, VertexInfo, length, Edge, Vertex, roomID);
	}

	for(int i=0; i<Vertex[id][1].size(); i++)
	{
		int eid=Vertex[id][1][i];
	    int nid=Edge[eid][0];
		if(VertexInfo[nid][2]==-1 && VertexInfo[nid][3]==roomID)
		     findRank_up(nid, rank-1, VertexInfo, length, Edge, Vertex, roomID);
	}
}


void PathBubble1::markLongestPath(int id, vector<vector<int>> &VertexInfo, vector<int> &path, vector<int> length, vector<vector<int>> Edge, vector<vector<vector<int>>> Vertex)
{
	if(id<=-1)
		return;
	if(VertexInfo[id][2]==-1)
	{
		VertexInfo[id][2] = length[id];
	    path.push_back(id);
	}

	if(length[id]==0||length[id]==-1)
	{   		
		return;
	}
	for(int i=0; i<Vertex[id][0].size(); i++)
	{
		int eid=Vertex[id][0][i];
	    int nid=Edge[eid][1];
		if(length[nid]==length[id]-1)
		 {
			 markLongestPath(nid, VertexInfo, path, length, Edge, Vertex);
			 break;
		}
	}
}

int PathBubble1::findLongestPath(int id, vector<int> &length, vector<vector<int>> Edge, vector<vector<vector<int>>> Vertex, vector<vector<int>> &VertexInfo, int roomID, bool down)
{
	//Vertex[id][0]
	int l,max=-100000;
	if((VertexInfo[id][0]!='R' && VertexInfo[id][0]!='S' && VertexInfo[id][3]!=roomID) || VertexInfo[id][2]!=-1)
	{	
		return -1;
	}
	if(length[id]!=-1)
	{
	    return length[id];
	}
    if(!down)
	{
		if( Vertex[id][0].empty())
		{
			length[id]=0;
		return length[id];
		}
	}
	else 
	{
	   if( Vertex[id][1].empty())
		{
			length[id]=0;
		return length[id];
		}
	
	}

	if(length[id]==-1)
	   length[id]=0;
    
	if(!down)
	{
		for(int i=0; i<Vertex[id][0].size(); i++)
	    {
        int eid=Vertex[id][0][i];				
		l=findLongestPath(Edge[eid][1], length, Edge, Vertex, VertexInfo, roomID);
		if(l>max)
			max=l;	
	    }
	}
	else
	{
		for(int i=0; i<Vertex[id][1].size(); i++)
	    {
			int eid=Vertex[id][1][i];				
			l=findLongestPath(Edge[eid][0], length, Edge, Vertex, VertexInfo, roomID, true);
			if(l>max)
				max=l;	
	    }
	}
	length[id]=max+1;
	return length[id];
}

void PathBubble1::saveAsXmlNode( QDomDocument &doc, QDomElement &node )
{
	QDomText text;
	QString str, tmpStr;

	QDomElement type = doc.createElement( tr("Type") );
	if(toplevel)
	   text = doc.createTextNode( tr("PathBubble1") );
	else text = doc.createTextNode( tr("subPathBubble1") );
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
	
	if ( _pathBubbleParent)
	{
		QDomElement parentBubble = doc.createElement( tr("ParentBubble") );
		QDomElement pareFile = doc.createElement( tr("FileName") );
		text = doc.createTextNode( _pathBubbleParent->getCurrentFilename() );
		pareFile.appendChild( text );
		parentBubble.appendChild( pareFile );
		QDomElement parePos = doc.createElement( tr("Position") );
		str = "(";
		tmpStr.setNum( _pathBubbleParent->pos().x() );
		str += tmpStr;
		str += ", ";
		tmpStr.setNum( _pathBubbleParent->pos().y() );
		str += tmpStr;
		str += ")";
		text = doc.createTextNode( str );
		parePos.appendChild( text );
		parentBubble.appendChild( parePos );
		node.appendChild( parentBubble );
	}
	
	//subgraph	
	QDomElement subGraphBubbles = doc.createElement( tr("subGraphBubbles") );
	int count=1;
	for (set<vector<vector<int>>>::iterator it = _scene->edgeconnections.begin(); it != _scene->edgeconnections.end(); it++)
	{
        vector<vector<int>> link = *it;	
        vector<int> link0=*link.begin(),link1=*link.rbegin();
		int pid1=link0[0], pid2=link1[0], type1=link0[1], type2=link1[1], id1=link0[2], id2=link1[2];
        QRectF itemPos1,itemPos2; 
		PathBubble1* path1=_scene->_pathBubbles[pid1], * path2=_scene->_pathBubbles[pid2];
	
		QDomElement descentPos = doc.createElement( tr("connection") );
		str = "";
		tmpStr.setNum( pid1);
		str += tmpStr;
		str += ",";
		tmpStr.setNum( type1);
		str += tmpStr;
		str += ",";
		tmpStr.setNum( id1);
		str += tmpStr;
		str += ",";
		tmpStr.setNum( pid2);
		str += tmpStr;
		str += ",";
		tmpStr.setNum( type2);
		str += tmpStr;
		str += ",";
		tmpStr.setNum( id2);
		str += tmpStr;
		str += ",";
		str += "";
		text = doc.createTextNode( str );
		descentPos.appendChild( text );		
		subGraphBubbles.appendChild( descentPos );
    }
	node.appendChild( subGraphBubbles );
	
	QDomElement descentBubbles = doc.createElement( tr("DescendantBubble") );
	for (int i = 0; i < _codeBubbles.size(); i ++)
	{
		QDomElement descent = doc.createElement( tr("Descendant") );
		
		QDomAttr descentID = doc.createAttribute( tr("descID") );
		str.setNum( i+1 );
		descentID.setValue( str );
		descent.setAttributeNode( descentID );

		QDomElement descentFile = doc.createElement( tr("FileName") );
		text = doc.createTextNode( _codeBubbles[i]->getCurrentFilename() );
		descentFile.appendChild( text );
		descent.appendChild( descentFile );

		QDomElement descentPos = doc.createElement( tr("Position") );
		str = "(";
		tmpStr.setNum( _codeBubbles[i]->pos().x() );
		str += tmpStr;
		str += ", ";
		tmpStr.setNum( _codeBubbles[i]->pos().y() );
		str += tmpStr;
		str += ")";
		text = doc.createTextNode( str );
		descentPos.appendChild( text );
		descent.appendChild( descentPos );

		descentBubbles.appendChild( descent );
	}


	for (int i = 0; i < _reactionBubbles.size(); i ++)
	{
		QDomElement descent = doc.createElement( tr("Descendant") );
		
		QDomAttr descentID = doc.createAttribute( tr("descID") );
		str.setNum( i+1 );
		descentID.setValue( str );
		descent.setAttributeNode( descentID );

		QDomElement descentFile = doc.createElement( tr("FileName") );
		text = doc.createTextNode( _reactionBubbles[i]->getCurrentFilename() );
		descentFile.appendChild( text );
		descent.appendChild( descentFile );

		QDomElement descentPos = doc.createElement( tr("Position") );
		str = "(";
		tmpStr.setNum( _reactionBubbles[i]->pos().x() );
		str += tmpStr;
		str += ", ";
		tmpStr.setNum( _reactionBubbles[i]->pos().y() );
		str += tmpStr;
		str += ")";
		text = doc.createTextNode( str );
		descentPos.appendChild( text );
		descent.appendChild( descentPos );

		descentBubbles.appendChild( descent );
	}

	for (int i = 0; i < _groupBubbles.size(); i ++)
	{
		QDomElement descent = doc.createElement( tr("Descendant") );
		
		QDomAttr descentID = doc.createAttribute( tr("descID") );
		str.setNum( i+1 );
		descentID.setValue( str );
		descent.setAttributeNode( descentID );

		QDomElement descentFile = doc.createElement( tr("FileName") );
		text = doc.createTextNode( _groupBubbles[i]->getCurrentFilename() );
		descentFile.appendChild( text );
		descent.appendChild( descentFile );

		QDomElement descentPos = doc.createElement( tr("Position") );
		str = "(";
		tmpStr.setNum( _groupBubbles[i]->pos().x() );
		str += tmpStr;
		str += ", ";
		tmpStr.setNum( _groupBubbles[i]->pos().y() );
		str += tmpStr;
		str += ")";
		text = doc.createTextNode( str );
		descentPos.appendChild( text );
		descent.appendChild( descentPos );

		descentBubbles.appendChild( descent );
	}	
	node.appendChild( descentBubbles );


	for (int i = 0; i < _pathBubbles.size(); i ++)
	{
		QDomElement descent = doc.createElement( tr("Descendant") );
		
		QDomAttr descentID = doc.createAttribute( tr("descID") );
		str.setNum( i+1 );
		descentID.setValue( str );
		descent.setAttributeNode( descentID );

		QDomElement descentFile = doc.createElement( tr("FileName") );
		text = doc.createTextNode( _pathBubbles[i]->getCurrentFilename() );
		descentFile.appendChild( text );
		descent.appendChild( descentFile );

		QDomElement descentPos = doc.createElement( tr("Position") );
		str = "(";
		tmpStr.setNum( _pathBubbles[i]->pos().x() );
		str += tmpStr;
		str += ", ";
		tmpStr.setNum( _pathBubbles[i]->pos().y() );
		str += tmpStr;
		str += ")";
		text = doc.createTextNode( str );
		descentPos.appendChild( text );
		descent.appendChild( descentPos );

		descentBubbles.appendChild( descent );
	}	
	node.appendChild( descentBubbles );


	QDomElement connectPos = doc.createElement( tr("TextBubblePosition") );
	for ( int i = 0; i < _conPoint.size(); i ++ )
	{
		QDomElement position = doc.createElement( tr("Position") );

		QDomAttr posID = doc.createAttribute( tr("posID") );
		str.setNum( i+1 );
		posID.setValue( str );
		position.setAttributeNode( posID );

		QDomElement conpoint = doc.createElement( tr("ConPoint") );
		str =  "(";
		tmpStr.setNum( _conPoint[i].x() );
		str += tmpStr;
		str += ", ";
		tmpStr.setNum( _conPoint[i].y() );
		str += tmpStr;
		str += ")";
		text = doc.createTextNode( str );
		conpoint.appendChild( text );
		position.appendChild( conpoint );

		connectPos.appendChild( position );
	}
	node.appendChild( connectPos );


	connectPos = doc.createElement( tr("ReactionBubblePosition") );
	for ( int i = 0; i < _reactionPoint.size(); i ++ )
	{
		QDomElement position = doc.createElement( tr("Position") );

		QDomAttr posID = doc.createAttribute( tr("posID") );
		str.setNum( i+1 );
		posID.setValue( str );
		position.setAttributeNode( posID );

		QDomElement conpoint = doc.createElement( tr("ConPoint") );
		str =  "(";
		tmpStr.setNum( _reactionPoint[i].x() );
		str += tmpStr;
		str += ", ";
		tmpStr.setNum( _reactionPoint[i].y() );
		str += tmpStr;
		str += ")";
		text = doc.createTextNode( str );
		conpoint.appendChild( text );
		position.appendChild( conpoint );

		connectPos.appendChild( position );
	}
	node.appendChild( connectPos );

	connectPos = doc.createElement( tr("GroupBubblePosition") );
	for ( int i = 0; i < _groupPoint.size(); i ++ )
	{
		QDomElement position = doc.createElement( tr("Position") );

		QDomAttr posID = doc.createAttribute( tr("posID") );
		str.setNum( i+1 );
		posID.setValue( str );
		position.setAttributeNode( posID );

		QDomElement conpoint = doc.createElement( tr("ConPoint") );
		str =  "(";
		tmpStr.setNum( _groupPoint[i].x() );
		str += tmpStr;
		str += ", ";
		tmpStr.setNum( _groupPoint[i].y() );
		str += tmpStr;
		str += ")";
		text = doc.createTextNode( str );
		conpoint.appendChild( text );
		position.appendChild( conpoint );

		connectPos.appendChild( position );
	}
	node.appendChild( connectPos );
}

void PathBubble1::loadFile(const QString &fileName)
{
	//if (maybeSave())
	{
		/*QFile file(fileName);
		if (!file.open(QFile::ReadOnly | QFile::Text)) 
		{
			return;
		}
		QTextStream in(&file);*/
		//_text->setText(in.readAll());

		setCurrentFile(fileName);
	}
}


void PathBubble1::recoverFromXmlNode( QDomElement node )
{
	QDomNodeList paraNode, posNode;
	QString str;

	paraNode = node.elementsByTagName( tr("PathPre") );
	_curFilePath = paraNode.at(0).toElement().text();

	paraNode = node.elementsByTagName( tr("FileName") );
	_curFileName = paraNode.at(0).toElement().text();

	str = _curFileName;
	if (str.size() > 0)
	{
		this->loadFile( str );
	}
	
	QStringList posStr;
	float x, y;

	_conPoint.clear();
	paraNode = node.elementsByTagName( tr("TextBubblePosition") );
	
	if ( !paraNode.isEmpty() )
	{
		paraNode = paraNode.at(0).toElement().elementsByTagName( tr("Position") );
		for (int i = 0; i < paraNode.size(); i ++)
		{
			posNode = paraNode.at(i).toElement().elementsByTagName( tr("ConPoint") );
			posStr= paraNode.at(i).toElement().text().split( "," );
			str = posStr.at(0);
			str.remove( 0, 1 );
			str = str.simplified();
			x = str.toFloat();
			str = posStr.at(1);
			str.chop(1);
			str = str.simplified();
			y  = str.toFloat();
			_conPoint.append( QPointF(x, y) );
		}
	}
	
	_reactionPoint.clear();
	paraNode = node.elementsByTagName( tr("ReactionBubblePosition") );	
	if ( !paraNode.isEmpty() )
	{
		paraNode = paraNode.at(0).toElement().elementsByTagName( tr("Position") );
		for (int i = 0; i < paraNode.size(); i ++)
		{
			posNode = paraNode.at(i).toElement().elementsByTagName( tr("ConPoint") );
			posStr= paraNode.at(i).toElement().text().split( "," );
			str = posStr.at(0);
			str.remove( 0, 1 );
			str = str.simplified();
			x = str.toFloat();
			str = posStr.at(1);
			str.chop(1);
			str = str.simplified();
			y  = str.toFloat();
			_reactionPoint.append( QPointF(x, y) );
		}
	}
	
	_groupPoint.clear();
	paraNode = node.elementsByTagName( tr("GroupBubblePosition") );
	
	if ( !paraNode.isEmpty() )
	{
		paraNode = paraNode.at(0).toElement().elementsByTagName( tr("Position") );
		for (int i = 0; i < paraNode.size(); i ++)
		{
			posNode = paraNode.at(i).toElement().elementsByTagName( tr("ConPoint") );
			posStr= paraNode.at(i).toElement().text().split( "," );
			str = posStr.at(0);
			str.remove( 0, 1 );
			str = str.simplified();
			x = str.toFloat();
			str = posStr.at(1);
			str.chop(1);
			str = str.simplified();
			y  = str.toFloat();

			_groupPoint.append( QPointF(x, y) );
		}
	}
}


void PathBubble1::setCurrentFile(const QString &fileName)
{
	_curFileName = fileName;
	_curFilePath = _curFileName;
	_labelName = _curFileName;
	int tindex = _curFilePath.lastIndexOf("\\");
	_curFilePath.remove(tindex+1, _curFilePath.length());	
}


void PathBubble1::rebuildBubbleConnect( QDomElement node )
{
	//_codeParent = NULL;
	//_reactionBubbles.clear();
	_reactionBubbles.clear();
	_codeBubbles.clear();
	_groupBubbles.clear();
	_pathBubbles.clear();

	QList<ItemBase*> itemList;
	ItemBase* item;
	//ReactionBubble3* tmp;
	QString name;
	int i;

	QDomNodeList paraNode;
	QDomElement tmpNode;

	itemList = this->_scene->getGroupManager()->allMembers();

	
	paraNode = node.elementsByTagName( tr("DescendantBubble") );
	if (!paraNode.isEmpty())
	{
		tmpNode = paraNode.at(0).toElement();
		QDomNodeList sons = tmpNode.elementsByTagName( tr("Descendant") );

		for (int id = 0; id < sons.size(); id ++)
		{
			tmpNode = sons.at(id).toElement();

			paraNode = tmpNode.elementsByTagName(tr("FileName"));
			name = paraNode.at(0).toElement().text();

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

			for ( i = 0; i < itemList.size(); i ++ )
			{
				item = itemList[i];
				if ( item->getType() == TEXTBUBBLE2 )
				{
					TextBubble2* tmp = dynamic_cast<TextBubble2*>(item);
					if (tmp->getCurrentFilename() == name 
						&& fabs(tmp->pos().x() - x) < 5
						&& fabs(tmp->pos().y() - y) < 5 )
					{
						this->_codeBubbles.append(tmp);
						break;
					}
				}
				else if ( item->getType() == REACTIONBUBBLE3 )
				{
					ReactionBubble3* tmp = dynamic_cast<ReactionBubble3*>(item);
					QString temp=tmp->getCurrentFilename();
					if (tmp->getCurrentFilename() == name 
						&& fabs(tmp->pos().x() - x) < 5
						&& fabs(tmp->pos().y() - y) < 5 )
					{
						this->_reactionBubbles.append(tmp);
						break;
					}
				}
				else if ( item->getType() == GROUPBUBBLE4 )
				{
					GroupBubble4* tmp = dynamic_cast<GroupBubble4*>(item);
					if (tmp->getCurrentFilename() == name 
						&& fabs(tmp->pos().x() - x) < 5
						&& fabs(tmp->pos().y() - y) < 5 )
					{
						this->_groupBubbles.append(tmp);
						break;
					}
				}
				else if ( item->getType() == SUBPATHBUBBLE1 )
				{
					subPathBubble1* tmp = dynamic_cast<subPathBubble1*>(item);
					if (tmp->getCurrentFilename() == name 
						&& fabs(tmp->pos().x() - x) < 5
						&& fabs(tmp->pos().y() - y) < 5 )
					{
						this->_pathBubbles.append(tmp);
						break;
					}
				}
			}
		}
	}
}



void PathBubble1::manuLocateCompartment(vector<QRectF> &CompartmentPos)	
{
	set<vector<int>> empty;
	vector<QString> temp;
	temp.push_back("");

	CompartmentPos.push_back(QRectF(23/2587.0, 102/1333.0, (439-23)/2587.0, (1311-102)/1333.0));//left
	temp[0]="cytosol";
	_scene->_compartmentName[m_pathwayID].push_back(temp);
	_scene->CompartmentContain[m_pathwayID].push_back(empty);

	CompartmentPos.push_back(QRectF(80/2587.0, 185/1333.0, (379-80)/2587.0, (396-185)/1333.0)); //inside left
	temp[0]="Golgi lumen";
	_scene->_compartmentName[m_pathwayID].push_back(temp);
	_scene->CompartmentContain[m_pathwayID].push_back(empty);
	
	CompartmentPos.push_back(QRectF(80/2587.0, 185/1333.0, (379-80)/2587.0, (396-185)/1333.0)); //inside left	
	temp[0]="Golgi membrane";
	_scene->_compartmentName[m_pathwayID].push_back(temp);
	_scene->CompartmentContain[m_pathwayID].push_back(empty);

	CompartmentPos.push_back(QRectF(530/2587.0, 241/1333.0, (2515-530)/2587.0, (1278-241)/1333.0));  //right
	temp[0]="early endosome";
	_scene->_compartmentName[m_pathwayID].push_back(temp);
	_scene->CompartmentContain[m_pathwayID].push_back(empty); 
	
	temp[0]="";
	CompartmentPos.push_back(QRectF(489/2587.0, 103/1333.0, (2581-489)/2587.0, (1323-103)/1333.0)); //inside right
	_scene->_compartmentName[m_pathwayID].push_back(temp);
	_scene->CompartmentContain[m_pathwayID].push_back(empty);
	
	temp[0]="early endosome membrane";
	CompartmentPos.push_back(QRectF(865/2587.0, 584/1333.0, (1519-865)/2587.0, (863-584)/1333.0));  //inside right
	_scene->_compartmentName[m_pathwayID].push_back(temp);
	_scene->CompartmentContain[m_pathwayID].push_back(empty);

	temp[0]="mitochondrial inner membrane";
	CompartmentPos.push_back(QRectF(1177/2587.0, 669/1333.0, (1307-1177)/2587.0, (737-669)/1333.0)); //inside inside
	_scene->_compartmentName[m_pathwayID].push_back(temp);
	_scene->CompartmentContain[m_pathwayID].push_back(empty);

	temp[0]="mitochondrial outer membrane";
	CompartmentPos.push_back(QRectF(1177/2587.0, 669/1333.0, (1307-1177)/2587.0, (737-669)/1333.0)); //inside inside
	_scene->_compartmentName[m_pathwayID].push_back(temp);
	_scene->CompartmentContain[m_pathwayID].push_back(empty);

	temp[0]="photoreceptor disc membrane";
	CompartmentPos.push_back(QRectF(1177/2587.0, 669/1333.0, (1307-1177)/2587.0, (737-669)/1333.0)); //inside inside
	_scene->_compartmentName[m_pathwayID].push_back(temp);
	_scene->CompartmentContain[m_pathwayID].push_back(empty);

	temp[0]="lysosomal membrane";
	CompartmentPos.push_back(QRectF(1177/2587.0, 669/1333.0, (1307-1177)/2587.0, (737-669)/1333.0)); //inside inside
	_scene->_compartmentName[m_pathwayID].push_back(temp);
	_scene->CompartmentContain[m_pathwayID].push_back(empty);

	temp[0]="endoplasmic reticulum membrane";
	CompartmentPos.push_back(QRectF(1177/2587.0, 669/1333.0, (1307-1177)/2587.0, (737-669)/1333.0)); //inside inside
	_scene->_compartmentName[m_pathwayID].push_back(temp);
	_scene->CompartmentContain[m_pathwayID].push_back(empty);

	temp[0]="plasma membrane";
	CompartmentPos.push_back(QRectF(530/2587.0, 241/1333.0, (2515-530)/2587.0, (1278-241)/1333.0));  //inside right
	_scene->_compartmentName[m_pathwayID].push_back(temp);
	_scene->CompartmentContain[m_pathwayID].push_back(empty);

	temp[0]="endosome membrane";
	CompartmentPos.push_back(QRectF(530/2587.0, 241/1333.0, (2515-530)/2587.0, (1278-241)/1333.0));  //inside right
	_scene->_compartmentName[m_pathwayID].push_back(temp);
	_scene->CompartmentContain[m_pathwayID].push_back(empty);

	temp[0]="Golgi-associated vesicle membrane";
	CompartmentPos.push_back(QRectF(530/2587.0, 241/1333.0, (2515-530)/2587.0, (1278-241)/1333.0));  //inside right
	_scene->_compartmentName[m_pathwayID].push_back(temp);
	_scene->CompartmentContain[m_pathwayID].push_back(empty);
	
	temp[0]="cytoplasmic vesicle membrane";
	CompartmentPos.push_back(QRectF(530/2587.0, 241/1333.0, (2515-530)/2587.0, (1278-241)/1333.0));  //inside right
	_scene->_compartmentName[m_pathwayID].push_back(temp);
	_scene->CompartmentContain[m_pathwayID].push_back(empty);	

	temp[0]="nuclear envelope";
	CompartmentPos.push_back(QRectF(1177/2587.0, 669/1333.0, (1307-1177)/2587.0, (737-669)/1333.0)); //inside inside
	_scene->_compartmentName[m_pathwayID].push_back(temp);
	_scene->CompartmentContain[m_pathwayID].push_back(empty);

	temp[0]="chromosome, centromeric region";
	CompartmentPos.push_back(QRectF(1177/2587.0, 669/1333.0, (1307-1177)/2587.0, (737-669)/1333.0)); //inside inside
	_scene->_compartmentName[m_pathwayID].push_back(temp);
	_scene->CompartmentContain[m_pathwayID].push_back(empty);

	temp[0]="chromosome";
	CompartmentPos.push_back(QRectF(1177/2587.0, 669/1333.0, (1307-1177)/2587.0, (737-669)/1333.0)); //inside inside
	_scene->_compartmentName[m_pathwayID].push_back(temp);
	_scene->CompartmentContain[m_pathwayID].push_back(empty);

	temp[0]="endosome";
	CompartmentPos.push_back(QRectF(1177/2587.0, 669/1333.0, (1307-1177)/2587.0, (737-669)/1333.0)); //inside inside
	_scene->_compartmentName[m_pathwayID].push_back(temp);
	_scene->CompartmentContain[m_pathwayID].push_back(empty);

	temp[0]="nucleoplasm";
	CompartmentPos.push_back(QRectF(1206/2587.0, 924/1333.0, (2171-1206)/2587.0, (1269-924)/1333.0)); //inside right
	_scene->_compartmentName[m_pathwayID].push_back(temp);
	_scene->CompartmentContain[m_pathwayID].push_back(empty);
	
		
	temp[0]="extracellular region";
	CompartmentPos.push_back(QRectF(530/2587.0, 241/1333.0, (2515-530)/2587.0, (1278-241)/1333.0)); 
	_scene->_compartmentName[m_pathwayID].push_back(temp);
	_scene->CompartmentContain[m_pathwayID].push_back(empty);
	
	temp[0]="cell junction";
	CompartmentPos.push_back(QRectF(489/2587.0, 103/1333.0, (2581-489)/2587.0, (1323-103)/1333.0)); //inside right
	_scene->_compartmentName[m_pathwayID].push_back(temp);
	_scene->CompartmentContain[m_pathwayID].push_back(empty);

	temp[0]="peroxisomal matrix";
	CompartmentPos.push_back(QRectF(489/2587.0, 103/1333.0, (2581-489)/2587.0, (1323-103)/1333.0)); //inside right
	_scene->_compartmentName[m_pathwayID].push_back(temp);
	_scene->CompartmentContain[m_pathwayID].push_back(empty);

	temp[0]="mitochondrial matrix";
	CompartmentPos.push_back(QRectF(489/2587.0, 103/1333.0, (2581-489)/2587.0, (1323-103)/1333.0)); //inside right
	_scene->_compartmentName[m_pathwayID].push_back(temp);
	_scene->CompartmentContain[m_pathwayID].push_back(empty);

	temp[0]="endosome lumen";
	CompartmentPos.push_back(QRectF(489/2587.0, 103/1333.0, (2581-489)/2587.0, (1323-103)/1333.0)); //inside right
	_scene->_compartmentName[m_pathwayID].push_back(temp);
	_scene->CompartmentContain[m_pathwayID].push_back(empty);	

	temp[0]="endocytic vesicle membrane";
	CompartmentPos.push_back(QRectF(489/2587.0, 103/1333.0, (2581-489)/2587.0, (1323-103)/1333.0)); //inside right
	_scene->_compartmentName[m_pathwayID].push_back(temp);
	_scene->CompartmentContain[m_pathwayID].push_back(empty);	

	temp[0]="phagocytic vesicle membrane";
	CompartmentPos.push_back(QRectF(489/2587.0, 103/1333.0, (2581-489)/2587.0, (1323-103)/1333.0)); //inside right
	_scene->_compartmentName[m_pathwayID].push_back(temp);
	_scene->CompartmentContain[m_pathwayID].push_back(empty);		

	temp[0]="phagocytic vesicle lumen";
	CompartmentPos.push_back(QRectF(489/2587.0, 103/1333.0, (2581-489)/2587.0, (1323-103)/1333.0)); //inside right
	_scene->_compartmentName[m_pathwayID].push_back(temp);
	_scene->CompartmentContain[m_pathwayID].push_back(empty);	

	temp[0]="lysosomal lumen";
	CompartmentPos.push_back(QRectF(489/2587.0, 103/1333.0, (2581-489)/2587.0, (1323-103)/1333.0)); //inside right
	_scene->_compartmentName[m_pathwayID].push_back(temp);
	_scene->CompartmentContain[m_pathwayID].push_back(empty);	

	temp[0]="photoreceptor outer segment membrane";
	CompartmentPos.push_back(QRectF(489/2587.0, 103/1333.0, (2581-489)/2587.0, (1323-103)/1333.0)); //inside right
	_scene->_compartmentName[m_pathwayID].push_back(temp);
	_scene->CompartmentContain[m_pathwayID].push_back(empty);	

	temp[0]="photoreceptor inner segment membrane";
	CompartmentPos.push_back(QRectF(489/2587.0, 103/1333.0, (2581-489)/2587.0, (1323-103)/1333.0)); //inside right
	_scene->_compartmentName[m_pathwayID].push_back(temp);
	_scene->CompartmentContain[m_pathwayID].push_back(empty);

	temp[0]="mitochondrial intermembrane space";
	CompartmentPos.push_back(QRectF(489/2587.0, 103/1333.0, (2581-489)/2587.0, (1323-103)/1333.0)); //inside right
	_scene->_compartmentName[m_pathwayID].push_back(temp);
	_scene->CompartmentContain[m_pathwayID].push_back(empty);

	temp[0]="secretory granule membrane";
	CompartmentPos.push_back(QRectF(489/2587.0, 103/1333.0, (2581-489)/2587.0, (1323-103)/1333.0)); //inside right
	_scene->_compartmentName[m_pathwayID].push_back(temp);
	_scene->CompartmentContain[m_pathwayID].push_back(empty);

	temp[0]="secretory granule lumen";
	CompartmentPos.push_back(QRectF(489/2587.0, 103/1333.0, (2581-489)/2587.0, (1323-103)/1333.0)); //inside right
	_scene->_compartmentName[m_pathwayID].push_back(temp);
	_scene->CompartmentContain[m_pathwayID].push_back(empty);

	temp[0]="endoplasmic reticulum lumen";
	CompartmentPos.push_back(QRectF(489/2587.0, 103/1333.0, (2581-489)/2587.0, (1323-103)/1333.0)); //inside right
	_scene->_compartmentName[m_pathwayID].push_back(temp);
	_scene->CompartmentContain[m_pathwayID].push_back(empty);

	temp[0]="endoplasmic reticulum quality control compartment";
	CompartmentPos.push_back(QRectF(489/2587.0, 103/1333.0, (2581-489)/2587.0, (1323-103)/1333.0)); //inside right
	_scene->_compartmentName[m_pathwayID].push_back(temp);
	_scene->CompartmentContain[m_pathwayID].push_back(empty);

	temp[0]="ER to Golgi transport vesicle membrane";
	CompartmentPos.push_back(QRectF(489/2587.0, 103/1333.0, (2581-489)/2587.0, (1323-103)/1333.0)); //inside right
	_scene->_compartmentName[m_pathwayID].push_back(temp);
	_scene->CompartmentContain[m_pathwayID].push_back(empty);

	temp[0]="clathrin-coated endocytic vesicle";
	CompartmentPos.push_back(QRectF(489/2587.0, 103/1333.0, (2581-489)/2587.0, (1323-103)/1333.0)); //inside right
	_scene->_compartmentName[m_pathwayID].push_back(temp);
	_scene->CompartmentContain[m_pathwayID].push_back(empty);

	temp[0]="platelet dense tubular network membrane";
	CompartmentPos.push_back(QRectF(489/2587.0, 103/1333.0, (2581-489)/2587.0, (1323-103)/1333.0)); //inside right
	_scene->_compartmentName[m_pathwayID].push_back(temp);
	_scene->CompartmentContain[m_pathwayID].push_back(empty);

	/*temp[0]="synaptic vesicle membrane";
	CompartmentPos.push_back(QRectF(489/2587.0, 103/1333.0, (2581-489)/2587.0, (1323-103)/1333.0)); //inside right
	_scene->_compartmentName[m_pathwayID].push_back(temp);
	_scene->CompartmentContain[m_pathwayID].push_back(empty);*/

	temp[0]="whole";
	CompartmentPos.push_back(QRectF(489/2587.0, 103/1333.0, (2581-489)/2587.0, (1323-103)/1333.0)); //inside right
	_scene->_compartmentName[m_pathwayID].push_back(temp);
	_scene->CompartmentContain[m_pathwayID].push_back(empty);
	
	CompartmentPos.push_back(QRectF(0, 0, 1, 1));//left
}

vector< set< vector< vector<int> > > > PathBubble1::findPathforTwoNodes(int type1, int id1, int type2, int id2, vector<vector<int>> edge)
{
	vector<vector<int>> pathVector;
	vector< set< vector< vector<int> > > > linkededge;

	//vertexInfo
	//...
	//Graph
	//find node set
	set<vector<int>> nodeSet;
	vector<vector<int>> nodeVector;
	vector<vector<int>> GRAPH;
	vector<int> node1(3,-1), node2(3,-1), edge_t(2,-1);
	vector<vector<int>> AEdge;
	AEdge.push_back(node1), AEdge.push_back(node2);

	nodeVector.push_back(node1); //let the rest start with 1
	GRAPH.resize(boundaryWidth.size()+1);
	for(set<vector<vector<int> > >::iterator it=updatedEdge_1.begin(); it!=updatedEdge_1.end(); it++)
	{		
		int size=updatedEdge_1.size();
		vector<vector<int> > edge=*it;						
		if(!inUpdatedEdge(edge, updatedEdge_4))
		{
			if(nodeSet.find(edge[0])==nodeSet.end())
			{
				nodeSet.insert(edge[0]);
				nodeVector.push_back(edge[0]);
			}
			if(nodeSet.find(edge[1])==nodeSet.end())
			{
				nodeSet.insert(edge[1]);
				nodeVector.push_back(edge[1]);
			}
						
			edge_t[0]=_scene->_wholeGraph->findNodeID(edge[0], nodeVector);
			edge_t[1]=_scene->_wholeGraph->findNodeID(edge[1], nodeVector);

			if(edge_t[0]>=GRAPH.size())		GRAPH.resize(edge_t[0]+1);
			if(edge_t[1]>=GRAPH.size())		GRAPH.resize(edge_t[1]+1);
			
			GRAPH[edge_t[0]].push_back(edge_t[1]);
		}	
	}
	
	for(set<vector<vector<int> > >::iterator it=updatedEdge_2.begin(); it!=updatedEdge_2.end(); it++)
    {   //activation
		vector<vector<int> > edge=*it;
		if(!inUpdatedEdge(edge, updatedEdge_4))
		{
			if(nodeSet.find(edge[0])==nodeSet.end())
			{
				nodeSet.insert(edge[0]);
				nodeVector.push_back(edge[0]);
			}
			if(nodeSet.find(edge[1])==nodeSet.end())
			{
				nodeSet.insert(edge[1]);
				nodeVector.push_back(edge[1]);
			}
						
			edge_t[0]=_scene->_wholeGraph->findNodeID(edge[0], nodeVector);
			edge_t[1]=_scene->_wholeGraph->findNodeID(edge[1], nodeVector);

				if(edge_t[0]>=GRAPH.size())		GRAPH.resize(edge_t[0]+1);
			if(edge_t[1]>=GRAPH.size())		GRAPH.resize(edge_t[1]+1);
			
			GRAPH[edge_t[0]].push_back(edge_t[1]);
		}
	}
	glEnd();
	
	for(set<vector<vector<int> > >::iterator it=updatedEdge_3.begin(); it!=updatedEdge_3.end(); it++)
	{
		vector<vector<int> > edge=*it;		
		if(!inUpdatedEdge(edge, updatedEdge_4))
		{
			if(nodeSet.find(edge[0])==nodeSet.end())
			{
				nodeSet.insert(edge[0]);
				nodeVector.push_back(edge[0]);
			}
			if(nodeSet.find(edge[1])==nodeSet.end())
			{
				nodeSet.insert(edge[1]);
				nodeVector.push_back(edge[1]);
			}
						
			edge_t[0]=_scene->_wholeGraph->findNodeID(edge[0], nodeVector);
			edge_t[1]=_scene->_wholeGraph->findNodeID(edge[1], nodeVector);

			if(edge_t[0]>=GRAPH.size())		GRAPH.resize(edge_t[0]+1);
			if(edge_t[1]>=GRAPH.size())		GRAPH.resize(edge_t[1]+1);
			
			GRAPH[edge_t[0]].push_back(edge_t[1]);
		}	
	}			     
	glEnd();
	
	int eid=0;
	for(set<vector<vector<int> > >::iterator it=updatedEdge_4.begin(); it!=updatedEdge_4.end(); it++)
	{   //multi-link
		vector<vector<int> > edge=*it;		   	    	 
		if(nodeSet.find(edge[0])==nodeSet.end())
		{
			nodeSet.insert(edge[0]);
			nodeVector.push_back(edge[0]);
		}
		if(nodeSet.find(edge[1])==nodeSet.end())
		{
			nodeSet.insert(edge[1]);
			nodeVector.push_back(edge[1]);
		}	
		edge_t[0]=_scene->_wholeGraph->findNodeID(edge[0], nodeVector);
		edge_t[1]=_scene->_wholeGraph->findNodeID(edge[1], nodeVector);

		if(edge_t[0]>=GRAPH.size())		GRAPH.resize(edge_t[0]+1);
		if(edge_t[1]>=GRAPH.size())		GRAPH.resize(edge_t[1]+1);
			
		GRAPH[edge_t[0]].push_back(edge_t[1]);
	}		    
	int totalnode=nodeVector.size()-1, totaledge=GRAPH.size()-1;	
		
	int pid= m_pathwayID;
	int start, end;
	
	node1[0] = pid, node1[1] = type1, node1[2] = id1;
	node2[0] = pid, node2[1] = type2, node2[2] = id2;
	
	start = _scene->_wholeGraph->findNodeID(node1, nodeVector);
	end = _scene->_wholeGraph->findNodeID(node2, nodeVector);

	if(start<0 || end<0)
		return linkededge;

	_scene->_wholeGraph->findAllPathesBetweenTwoNodes(start, end, totalnode, totaledge, GRAPH, pathVector);	

	/*findAllPathes(18, 20, totalnode, totaledge, GRAPH, pathVector);	
	findAllPathes(1, 10, totalnode, totaledge, GRAPH, pathVector);	
	findAllPathes(15, 20, totalnode, totaledge, GRAPH, pathVector);	
	findAllPathes(25, 20, totalnode, totaledge, GRAPH, pathVector);	*/

	if(m_pathwayID>=linkededge.size())	   linkededge.resize(m_pathwayID+1);
	/*if(m_pathwayID>=linkedProtein.size())	   linkedProtein.resize(m_pathwayID+1);
	if(m_pathwayID>=linkedSmallMolecule.size())        linkedSmallMolecule.resize(m_pathwayID+1);
	if(m_pathwayID>=linkedComplex.size()) 	   linkedComplex.resize(m_pathwayID+1);
	if(m_pathwayID>=linkedDna.size()) 	   linkedDna.resize(m_pathwayID+1);
	if(m_pathwayID>=linkedReaction.size()) 	   linkedReaction.resize(m_pathwayID+1);
	if(m_pathwayID>=linkedANode.size()) 	   linkedANode.resize(m_pathwayID+1);
	if(m_pathwayID>=linkedCompartment.size())    linkedCompartment.resize(m_pathwayID+1);
	if(m_pathwayID>=linkedPathway.size())    linkedPathway.resize(m_pathwayID+1);*/
	
	
	for(int i=0; i<pathVector.size(); ++i)
    {
		for(int j=0; j<pathVector[i].size()-1; ++j)
		{
			int id1=pathVector[i][j], id2=pathVector[i][j+1];
			AEdge[0] = nodeVector[id1];  AEdge[1]= nodeVector[id2];

            linkededge[m_pathwayID].insert(AEdge);	
			
			/*int pid1=AEdge[0][0], type1=AEdge[0][1], nid1=AEdge[0][2];
			int pid2=AEdge[1][0], type2=AEdge[1][1], nid2=AEdge[1][2];

			PathBubble1* pBubble1 = _scene->_pathBubbles[AEdge[0][0]];	
			switch(type1)
			{
			    case 'P':  pBubble1->linkedProtein[pid1].insert(nid1);          break;
				case 'S':  pBubble1->linkedSmallMolecule[pid1].insert(nid1);    break;
				case 'C':  pBubble1->linkedComplex[pid1].insert(nid1);          break;
				case 'D':  pBubble1->linkedDna[pid1].insert(nid1);              break;
				case 'R':  pBubble1->linkedReaction[pid1].insert(nid1);         break;
				case 'E':  pBubble1->linkedPhysicalEntity[pid1].insert(nid1);   break;					
				case 'L':  pBubble1->linkedANode[pid1].insert(nid1);            break;	
				case 'M':  pBubble1->linkedCompartment[pid1].insert(nid1);      break;	
				case 'W':  pBubble1->linkedPathway[pid1].insert(nid1);          break;	
			}	

			PathBubble1* pBubble2 = _scene->_pathBubbles[AEdge[0][0]];	
			switch(type2)
			{
			    case 'P':  pBubble2->linkedProtein[pid2].insert(nid2);          break;
				case 'S':  pBubble2->linkedSmallMolecule[pid2].insert(nid2);    break;
				case 'C':  pBubble2->linkedComplex[pid2].insert(nid2);          break;
				case 'D':  pBubble2->linkedDna[pid2].insert(nid2);              break;
				case 'R':  pBubble2->linkedReaction[pid2].insert(nid2);         break;
				case 'E':  pBubble2->linkedPhysicalEntity[pid2].insert(nid2);   break;					
				case 'L':  pBubble2->linkedANode[pid2].insert(nid2);            break;	
				case 'M':  pBubble2->linkedCompartment[pid2].insert(nid2);      break;	
				case 'W':  pBubble2->linkedPathway[pid2].insert(nid2);          break;	
			}	*/
		}
	}
	//getGraphToBePaint();	
	return linkededge;
}

//getCurrentGraph for boost
vector<vector<int>> PathBubble1::getCurrentGraph(vector<vector<int>> edge)
{
	vector<vector<int>> GRAPH;
	vector<int> temp(2,0);
	QRectF s,e, m; 
    for(int i=0; i<edge.size(); i++)
	{	
		int type, id;
		if(edge[i][2]=='R'&&edge[i][4]=='R')
		{			
			bool flags=true, flagm=true, flage=true;
			bool t1=false,t2=false;
		
			int type=edge[i][0], id=edge[i][1];	
			
			temp[0]=edge[i][4];	temp[1]=edge[i][5]; reactionRecord.insert(temp);				
			temp[0]=edge[i][0];	temp[1]=edge[i][1]; reactionRecord.insert(temp);	
		}
		else
		{
			bool flags=true, flagm=true, flage=true;
			bool t1=false,t2=false;
		
			int type=edge[i][0], id=edge[i][1];	
			if(id<0)
				 flags=false;

			if(id>=0)
			switch(type)
			{
				 case 'C': s=complexPos[id];          break; //bug when loading 190236
				 case 'D': s=DnaPos[id];              break;
				 case 'E': s=physicalEntityPos[id];   break; 
				 case 'P': s=proteinPos[id];          break; //bug when loading 2404192
				 case 'S': s=smallMoleculePos[id];    break;			
			}
		    type=edge[i][4];
		    id=edge[i][5];

		    if(id<0)
			   flagm=false;

		    if(id>=0)
		    if(type=='R') 
		    {
			    m=reactionPos[id];			
		    }
		 		 		 
		    type=edge[i][2], id=edge[i][3];	
		    QRectF rect;
		    if(id<0)
			   flage=false;

		    if(id>=0)
		    switch(type)
	        {
				 case 'C': rect= complexPos[id]; break;
				 case 'D': rect= DnaPos[id];    break;
				 case 'E': rect= physicalEntityPos[id];    break; 
				 case 'P': rect= proteinPos[id];    break;
				 case 'S': rect= smallMoleculePos[id];  break;			
				 case 'R': rect= reactionPos[id];  break;  //catalysis for bio reaction	
		    }	
		    e=rect;
			temp[0]=edge[i][4];	temp[1]=edge[i][5]; reactionRecord.insert(temp);
				temp[0]=edge[i][2];	temp[1]=edge[i][3]; reactionRecord.insert(temp);
				temp[0]=edge[i][0];	temp[1]=edge[i][1]; reactionRecord.insert(temp);		
			if(m.x()>-999.99 && e.x() >-999.99 && s.x()>-999.99)
			{
							                   
			}
		}		   
	} 
	return GRAPH;
 }


void PathBubble1::testforceDirected(QPainter *painter, QRectF Compartment)
{
	static int mark=0;
	if(mark!=0)
		return;

	//findPathforANode(edge);

	float width=Compartment.width(); //inside right
	float height=Compartment.height(); 
	float x=Compartment.x(),y=Compartment.y();

    int iterations = 100;
	vector<QPoint> edges;
	vector<int> tg(2,0);
	/*
	shortest path from a start point

	typedef boost::adjacency_list <  boost::listS,  boost::vecS,  boost::directedS,  boost::no_property,  boost::property <  boost::edge_weight_t, int > > graph_t;
  typedef  boost::graph_traits <  graph_t >::vertex_descriptor vertex_descriptor;
  typedef std::pair<int, int> Edge;

  const int num_nodes = 5;
  enum nodes { A, B, C, D, E };
  char name[] = "ABCDE";
  Edge edge_array[] = { Edge(A, C), Edge(B, B), Edge(B, D), Edge(B, E),
    Edge(C, B), Edge(C, D), Edge(D, E), Edge(E, A), Edge(E, B)
  };

  int weights[] = { 1, 2, 1, 2, 7, 3, 1, 1, 1 };
  int num_arcs = sizeof(edge_array) / sizeof(Edge);
  graph_t g(edge_array, edge_array + num_arcs, weights, num_nodes);
  boost:: property_map<graph_t,  boost::edge_weight_t>::type weightmap = get( boost::edge_weight, g);
  std::vector<vertex_descriptor> p(num_vertices(g));
  std::vector<int> d(num_vertices(g));
  vertex_descriptor s = vertex(A, g);

  std::ofstream dot_file;//("figs/dijkstra-eg.dot");
  dot_file.open ("dijkstra-eg.txt");

  dijkstra_shortest_paths(g, s,
                          predecessor_map(boost::make_iterator_property_map(p.begin(), get(boost::vertex_index, g))).
                          distance_map(boost::make_iterator_property_map(d.begin(), get(boost::vertex_index, g))));

  dot_file << "distances and parents:" << std::endl;
   boost::graph_traits < graph_t >::vertex_iterator vi, vend;
  for (boost::tie(vi, vend) = vertices(g); vi != vend; ++vi) {
    dot_file << "distance(" << name[*vi] << ") = " << d[*vi] << ", ";
    dot_file << "parent(" << name[*vi] << ") = " << name[p[*vi]] << std::
      endl;

	}
  //std::cout << std::endl;  
  dot_file << "digraph D {\n"
    << "  rankdir=LR\n"
    << "  size=\"4,3\"\n"
    << "  ratio=\"fill\"\n"
    << "  edge[style=\"bold\"]\n" << "  node[shape=\"circle\"]\n";

  boost::graph_traits < graph_t >::edge_iterator ei, ei_end;
  for (boost::tie(ei, ei_end) = boost::edges(g); ei != ei_end; ++ei) {
    boost::graph_traits < graph_t >::edge_descriptor e = *ei;
    boost::graph_traits < graph_t >::vertex_descriptor
      u = source(e, g), v = target(e, g);
    dot_file << name[u] << " -> " << name[v]
      << "[label=\"" << get(weightmap, e) << "\"";
    if (p[v] == u)
      dot_file << ", color=\"black\"";
    else
      dot_file << ", color=\"grey\"";
    dot_file << "]";
  }
  dot_file << "}";
  dot_file.close();
  */

	//find all paths from two nodes
	//@Author :Ritesh Kumar Gupta
	
  //return EXIT_SUCCESS;

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
  }
  */
}

void PathBubble1::testforceDirected_2(QPainter *painter, QRectF Compartment)
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



void PathBubble1::forceDirected()
{	
	/*float width=1; //inside right
	float height=1; 
	QPointF start,middle,end;
	int type, id;
	int sid, mid, eid;

	//float x=Compartment.x(),y=Compartment.y();
    int iterations = 100;
	vector<int> nodeid(complexNum + proteinNum + smallMoleculeNum + DnaNum + reactionNum + physicalEntityNum + 6,-1); 
	set<vector<int>> check; vector<int> order;
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
		 switch(type)
	     {
		     case 'C': start=complexPos[id].center();         break;
			 case 'D': start=DnaPos[id].center();             break;
			 case 'E': start=physicalEntityPos[id].center();          break; 
			 case 'P': start=proteinPos[id].center();         break;
			 case 'S': start=smallMoleculePos[id].center();   break;			
		 }
		 sid=getVerticeID(type, id);

		 type=edge[i][4];
		 id=edge[i][5];
		 //if(type=='R') 
		 middle=reactionPos[id].center();
		 //else middle degeneration???

		 mid=getVerticeID(type, id);

		 type=edge[i][2], id=edge[i][3];		 
		 switch(type)
	     {
		     case 'C': end=complexPos[id].center();         break;
			 case 'D': end=DnaPos[id].center();             break;
			 case 'E': end=physicalEntityPos[id].center();         break; 
			 case 'P': end=proteinPos[id].center();         break;
			 case 'S': end=smallMoleculePos[id].center();   break;			
			 case 'R': end=reactionPos[id].center();   break;  //catalysis for bio reaction	
		 }
		 eid=getVerticeID(type, id);

		 start=QPointF(start.x()*width*2-width, start.y()*height*2-height);
		 middle=QPointF(middle.x()*width*2-width, middle.y()*height*2-height);
		 end=QPointF(end.x()*width*2-width, end.y()*height*2-height);

		 
			//if(start.x()>=-width&&middle.x()>=-width&&end.x()>=-width)
			{
				//painter->drawLine(start,middle);
				//painter->drawLine(middle,end);
				add_edge(nodeid[sid], nodeid[mid], g);
				add_edge(nodeid[mid], nodeid[eid], g);

				//painter->setBrush(QColor(255,255,255,255));
				if(type=='R')
				{
					int R=4;
					float dx,dy,ds;
					ds=sqrt(pow(start.x()-middle.x(),2)+pow(start.y()-middle.y(),2));
					dx=(start.x()-middle.x())/ds, dy=(start.y()-middle.y())/ds;
					end.setX(middle.x()+dx*R*2), end.setY(middle.y()+dy*R*2);
					//painter->drawEllipse(end,R,R);
				}
				else
				{
					//drawArrow(painter, middle, end, QColor(0, 0, 0, 255) );		 
				}
		 }	

		if(stop==i)
			break; 
	}
   
   
  

  typedef std::vector<point_type> PositionVec;
  PositionVec position_vec(num_vertices(g));
  typedef iterator_property_map<PositionVec::iterator, 
                                property_map<Graph, vertex_index_t>::type>
  PositionMap;
  PositionMap position(position_vec.begin(), get(vertex_index, g));

  minstd_rand gen;
  topology_type topo(gen, 0, 0, width, height);
  random_graph_layout(g, position, topo);
  
  fruchterman_reingold_force_directed_layout
    (g, position, topo,
     cooling(progress_cooling(iterations)));

  graph_traits<Graph>::vertex_iterator vi, vi_end;

  for (boost::tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi) {
    // to proloation
	getItemID(order[*vi], type, id);
	  
	QRectF node;
	switch(type)
	{
		 case 'C': node=complexPos[id];   node.setX(position[*vi][0]); node.setY(position[*vi][1]);	     node.setWidth(complexPos[id].width());  	   node.setHeight(complexPos[id].height());      break;
		 case 'D': node=DnaPos[id];       node.setX(position[*vi][0]); node.setY(position[*vi][1]);	     node.setWidth(DnaPos[id].width());	           node.setHeight(DnaPos[id].height());       break;
		 case 'E': node=physicalEntityPos[id];    node.setX(position[*vi][0]); node.setY(position[*vi][1]);	     node.setWidth(physicalEntityPos[id].width());	       node.setHeight(physicalEntityPos[id].height());      break; 
		 case 'P': node=proteinPos[id];   node.setX(position[*vi][0]); node.setY(position[*vi][1]);	     node.setWidth(proteinPos[id].width());	       node.setHeight(proteinPos[id].height());       break;
		 case 'S': node=smallMoleculePos[id];  node.setX(position[*vi][0]); node.setY(position[*vi][1]); node.setWidth(smallMoleculePos[id].width());  node.setHeight(smallMoleculePos[id].height());  break;			
		 case 'R': node=reactionPos[id];      node.setX(position[*vi][0]); node.setY(position[*vi][1]);	 node.setWidth(reactionPos[id].width());	   node.setHeight(reactionPos[id].height());   break;  //catalysis for bio reaction	
	 }
		
	switch(type)
	{
		     case 'C': if(complexPos[id].x()!=-1) complexPos[id]=node;         break;
			 case 'D': if(DnaPos[id].x()!=-1)    DnaPos[id]=node;             break;
			 case 'E': if(physicalEntityPos[id].x()!=-1) physicalEntityPos[id]=node;         break; 
			 case 'P': if(proteinPos[id].x()!=-1) proteinPos[id]=node;         break;
			 case 'S': if(smallMoleculePos[id].x()!=-1) smallMoleculePos[id]=node;   break;			
			 case 'R': if(reactionPos[id].x()!=-1) reactionPos[id]=node;   break;  //catalysis for bio reaction	
	 }
	 //paintComplex( painter, node, _scene->_complexName[*vi]);
  }

  nodeid.clear();
  */

}

void PathBubble1::preLocateNodes(vector<QRectF> &complexPos, vector<QRectF> &proteinPos, vector<QRectF> &smallMoleculePos)	
{

    for(int i=0; i<complexPos.size();i++)
	{
		QSizeF size=getItemSize("Complex", 'C', i);	
		complexPos[i].setX(0);
		complexPos[i].setY(0);
		if(complexPos[i].x()!=-1)
		{
			complexPos[i].setX(complexPos[i].x()-size.width()/2.0/this->Width());
			complexPos[i].setY(complexPos[i].y()-size.height()/2.0/this->Height());
		}
		complexPos[i].setWidth(size.width());
		if(m_pathwayID==0) complexPos[i].setHeight(size.height()>0.03?0.03:size.height()); else complexPos[i].setHeight(size.height()>0.05?0.05:size.height());  		
	}	

	for(int i=0; i<proteinPos.size();i++)
	{
		QSizeF size=getItemSize("Protein", 'P', i);	
		proteinPos[i].setX(0);
		proteinPos[i].setY(0);
		if(proteinPos[i].x()!=-1)
		{
	    proteinPos[i].setX(proteinPos[i].x()-size.width()/2.0/this->Width());
		proteinPos[i].setY(proteinPos[i].y()-size.height()/2.0/this->Height());
		}
		proteinPos[i].setWidth(size.width());
		if(m_pathwayID==0) proteinPos[i].setHeight(size.height()>0.03?0.03:size.height()); 	else proteinPos[i].setHeight(size.height()>0.05?0.05:size.height());  
	}	

	for(int i=0; i<physicalEntityPos.size();i++)
	{
		QSizeF size=getItemSize("physicalEntity", 'E', i);	
		physicalEntityPos[i].setX(0);
		physicalEntityPos[i].setY(0);
		if(physicalEntityPos[i].x()!=-1)
		{			
	    physicalEntityPos[i].setX(physicalEntityPos[i].x()-size.width()/2.0/this->Width());
		physicalEntityPos[i].setY(physicalEntityPos[i].y()-size.height()/2.0/this->Height());
		}
		physicalEntityPos[i].setWidth(size.width());
		if(m_pathwayID==0) physicalEntityPos[i].setHeight(size.height()>0.03?0.03:size.height()); 	else physicalEntityPos[i].setHeight(size.height()>0.05?0.05:size.height());  	
	}	

	for(int i=0; i<DnaPos.size();i++)
	{
		QSizeF size=getItemSize("Dna",'D', i);
		DnaPos[i].setX(0);
		DnaPos[i].setY(0);
		if(DnaPos[i].x()!=-1)
		{			
	    DnaPos[i].setX(DnaPos[i].x()-size.width()/2.0/this->Width());
		DnaPos[i].setY(DnaPos[i].y()-size.height()/2.0/this->Height());
		}
		DnaPos[i].setWidth(size.width());
		if(m_pathwayID==0) DnaPos[i].setHeight(size.height()>0.03?0.03:size.height()); 	else DnaPos[i].setHeight(size.height()>0.05?0.05:size.height()); 	
	}
    //keqin temporarily disable
	/*for(int i=1; i<EmptyPos.size();i++)
	{
		QSizeF size=getItemSize('D', i);
		EmptyPos[i].setX(0);
		EmptyPos[i].setY(0);
		if(EmptyPos[i].x()!=-1)
		{			
			EmptyPos[i].setX(EmptyPos[i].x()-size.width()/2.0/this->Width());
			EmptyPos[i].setY(EmptyPos[i].y()-size.height()/2.0/this->Height());
		}
		EmptyPos[i].setWidth(size.width());
		if(m_pathwayID==0) EmptyPos[i].setHeight(size.height()>0.03?0.03:size.height()); 	else EmptyPos[i].setHeight(size.height()>0.05?0.05:size.height()); 	
	}*/

	for(int i=0; i<smallMoleculePos.size();i++)
	{
		QSizeF size=getItemSize("Small", 'S', i);	
		
		if(drawSmallmolecule)
		    smallMoleculePos[i].setX(1); // make it invisible as sugg
		else 
			smallMoleculePos[i].setX(-1000); //keqin
		
		
		smallMoleculePos[i].setY(0);
		if(smallMoleculePos[i].x()!=-1)
		{			
			smallMoleculePos[i].setX(smallMoleculePos[i].x()-size.width()/2.0/this->Width());
			smallMoleculePos[i].setY(smallMoleculePos[i].y()-size.height()/2.0/this->Height());
		}
		smallMoleculePos[i].setWidth(size.width());
		if(m_pathwayID==0) smallMoleculePos[i].setHeight(size.height()>0.03?0.03:size.height()); else smallMoleculePos[i].setHeight(size.height()>0.05?0.05:size.height()); 	
	}		


}

void PathBubble1::manuLocateNodes(vector<QRectF> &complexPos, vector<QRectF> &proteinPos, vector<QRectF> &smallMoleculePos)	
{
	complexPos[1]=QRectF(13/255.0-0.03, 29/132.0-0.02, 0,0);
	complexPos[2]=QRectF(31/255.0-0.03, 29/132.0-0.02, 0,0);
	complexPos[3]=QRectF(55/255.0-0.03, 2/132.0-0.02, 0,0);
	
	complexPos[4]=QRectF(55/255.0-0.03, 11/132.0-0.02, 0,0);
	complexPos[5]=QRectF(78/255.0-0.03, 10/132.0-0.02, 0,0);
	complexPos[6]=QRectF(83/255.0-0.03, 18/132.0-0.02, 0,0);
	complexPos[8]=QRectF(98/255.0-0.03, 16/132.0-0.02, 0,0);
	complexPos[9]=QRectF(122/255.0-0.03, 17/132.0-0.02, 0,0);
	complexPos[16]=QRectF(145/255.0-0.03, 18/132.0-0.02, 0,0);
	complexPos[17]=QRectF(145/255.0-0.03, 13/132.0-0.02, 0,0);
	complexPos[15]=QRectF(171/255.0-0.03, 20/132.0-0.02, 0,0);
	complexPos[18]=QRectF(166/255.0-0.03, 13/132.0-0.02, 0,0);

	complexPos[33]=QRectF(185/255.0-0.03, 18/132.0-0.02, 0,0);
	complexPos[30]=QRectF(202/255.0-0.03, 18/132.0-0.02, 0,0);
	complexPos[31]=QRectF(228/255.0-0.03, 18/132.0-0.02, 0,0);
	complexPos[32]=QRectF(247/255.0-0.03, 21/132.0-0.02, 0,0);
	
	complexPos[29]=QRectF(185/255.0-0.03, 42/132.0-0.02, 0,0);

	complexPos[39]=QRectF(133/255.0-0.03, 25/132.0-0.02, 0,0);
	complexPos[34]=QRectF(133/255.0-0.03, 39/132.0-0.02, 0,0);
	complexPos[35]=QRectF(150/255.0-0.03, 39/132.0-0.02, 0,0);
	complexPos[36]=QRectF(133/255.0-0.03, 53/132.0-0.02, 0,0);
	complexPos[37]=QRectF(150/255.0-0.03, 53/132.0-0.02, 0,0);

	complexPos[40]=QRectF(86/255.0-0.03, 78/132.0-0.02, 0,0);
	complexPos[11]=QRectF(105/255.0-0.03, 60/132.0-0.02, 0,0);
	complexPos[12]=QRectF(105/255.0-0.03, 80/132.0-0.02, 0,0);
	complexPos[13]=QRectF(121/255.0-0.03, 80/132.0-0.02, 0,0);
	complexPos[20]=QRectF(145/255.0-0.03, 68/132.0-0.02, 0,0);
	complexPos[21]=QRectF(145/255.0-0.03, 85/132.0-0.02, 0,0);
	complexPos[38]=QRectF(128/255.0-0.03, 89/132.0-0.02, 0,0);
	complexPos[14]=QRectF(109/255.0-0.03, 101/132.0-0.02, 0,0);
	
	complexPos[19]=QRectF(170/255.0-0.03, 62/132.0-0.02, 0,0);
	complexPos[23]=QRectF(202/255.0-0.03, 65/132.0-0.02, 0,0);
	complexPos[26]=QRectF(215/255.0-0.03, 65/132.0-0.02, 0,0);
	complexPos[19]=QRectF(227/255.0-0.03, 65/132.0-0.02, 0,0);	
	
	complexPos[41]=QRectF(48/255.0-0.03, 31/132.0-0.02, 0,0);
	complexPos[43]=QRectF(48/255.0-0.03, 43/132.0-0.02, 0,0);
	complexPos[44]=QRectF(48/255.0-0.03, 55/132.0-0.02, 0,0);
	complexPos[45]=QRectF(48/255.0-0.03, 71/132.0-0.02, 0,0);
	complexPos[48]=QRectF(48/255.0-0.03, 84/132.0-0.02, 0,0);
	complexPos[49]=QRectF(48/255.0-0.03, 97/132.0-0.02, 0,0);

	complexPos[22]=QRectF(182/255.0-0.03, 110/132.0-0.02, 0,0);
	complexPos[24]=QRectF(182/255.0-0.03, 116/132.0-0.02, 0,0);
	complexPos[25]=QRectF(204/255.0-0.03, 116/132.0-0.02, 0,0);
	complexPos[27]=QRectF(182/255.0-0.03, 122/132.0-0.02, 0,0);


	//protein
	proteinPos[4]=QRectF(215/2563.0-0.03, 176/1326.0-0.02, 0,0);
	proteinPos[22]=QRectF(918/2563.0-0.03, 1014/1326.0-0.02, 0,0);
	proteinPos[64]=QRectF(868/2563.0-0.03, 846/1326.0-0.02, 0,0);
	proteinPos[18]=QRectF(980/2563.0-0.03, 943/1326.0-0.02, 0,0);

	proteinPos[37]=QRectF(591/2563.0-0.03, 758/1326.0-0.02, 0,0);
	proteinPos[43]=QRectF(577/2563.0-0.03, 870/1326.0-0.02, 0,0);
	proteinPos[8]=QRectF(923/2563.0-0.03, 273/1326.0-0.02, 0,0);
	proteinPos[62]=QRectF(1180/2563.0-0.03, 850/1326.0-0.02, 0,0);

	proteinPos[30]=QRectF(1672/2563.0-0.03, 1058/1326.0-0.02, 0,0);
	proteinPos[31]=QRectF(1568/2563.0-0.03, 1100/1326.0-0.02, 0,0);
	proteinPos[36]=QRectF(1926/2563.0-0.03, 1142/1326.0-0.02, 0,0);
	proteinPos[33]=QRectF(1643/2563.0-0.03, 1182/1326.0-0.02, 0,0);
	proteinPos[41]=QRectF(1526/2563.0-0.03, 1234/1326.0-0.02, 0,0);


	proteinPos[58]=QRectF(2339/2563.0-0.03, 306/1326.0-0.02, 0,0);	
	proteinPos[40]=QRectF(2098/2563.0-0.03, 805/1326.0-0.02, 0,0);
	
	proteinPos[63]=QRectF(1238/2563.0-0.03, 240/1326.0-0.02, 0,0);
	proteinPos[24]=QRectF(1392/2563.0-0.03, 285/1326.0-0.02, 0,0);
	proteinPos[32]=QRectF(1293/2563.0-0.03, 307/1326.0-0.02, 0,0);
	proteinPos[15]=QRectF(1233/2563.0-0.03, 379/1326.0-0.02, 0,0);
	proteinPos[23]=QRectF(1560/2563.0-0.03, 389/1326.0-0.02, 0,0);
	proteinPos[14]=QRectF(1259/2563.0-0.03, 448/1326.0-0.02, 0,0);
	
	proteinPos[61]=QRectF(1217/2563.0-0.03, 525/1326.0-0.02, 0,0);
	proteinPos[10]=QRectF(1213/2563.0-0.03, 626/1326.0-0.02, 0,0);
	
	proteinPos[43]=QRectF(2444/2563.0-0.03, 470/1326.0-0.02, 0,0);
	
	proteinPos[43]=QRectF(2104/2563.0-0.03, 282/1326.0-0.02, 0,0);	
	
	proteinPos[43]=QRectF(1380/2563.0-0.03, 450/1326.0-0.02, 0,0);
	
	
	physicalEntityPos[1]=QRectF(1171/2563.0-0.03, 486/1326.0-0.02, 0,0);
	physicalEntityPos[2]=QRectF(2203/2563.0-0.03, 281/1326.0-0.02, 0,0);

	/*
		//ATP
	smallMoleculePos[1].push_back(QRectF(563/2563.0, 500/1326.0, 0,0));

	//ADP
	smallMoleculePos[2].push_back(QRectF(563/2563.0, 650/1326.0, 0,0));	

	//H2O
	smallMoleculePos[3].push_back(QRectF(831/2563.0, 713/1326.0, 0,0));	

	//Pi
	smallMoleculePos[4].push_back(QRectF(808/2563.0, 602/1326.0, 0,0));	
	*/
	
	
	for(int i=0; i<complexPos.size();i++)
	{
		QSizeF size=getItemSize('C', i);	
		if(complexPos[i].x()!=-1)
		{
			complexPos[i].setX(complexPos[i].x()-size.width()/2.0/this->Width());
			complexPos[i].setY(complexPos[i].y()-size.height()/2.0/this->Height());
		}
			complexPos[i].setWidth(size.width());
			complexPos[i].setHeight(size.height()>0.04?0.04:size.height()); 
		
	}	

	for(int i=0; i<proteinPos.size();i++)
	{
		QSizeF size=getItemSize('P', i);	
		if(proteinPos[i].x()!=-1)
		{
	    proteinPos[i].setX(proteinPos[i].x()-size.width()/2.0/this->Width());
		proteinPos[i].setY(proteinPos[i].y()-size.height()/2.0/this->Height());
		}
		proteinPos[i].setWidth(size.width());
		proteinPos[i].setHeight(size.height()>0.04?0.04:size.height()); 	
	}	

	for(int i=0; i<physicalEntityPos.size();i++)
	{
		QSizeF size=getItemSize('E', i);	
		if(physicalEntityPos[i].x()!=-1)
		{			
	    physicalEntityPos[i].setX(physicalEntityPos[i].x()-size.width()/2.0/this->Width());
		physicalEntityPos[i].setY(physicalEntityPos[i].y()-size.height()/2.0/this->Height());
		}
		physicalEntityPos[i].setWidth(size.width());
		physicalEntityPos[i].setHeight(size.height()>0.04?0.04:size.height()); 		
	}	

	for(int i=0; i<DnaPos.size();i++)
	{
		QSizeF size=getItemSize('D', i);	
		if(DnaPos[i].x()!=-1)
		{			
	    DnaPos[i].setX(DnaPos[i].x()-size.width()/2.0/this->Width());
		DnaPos[i].setY(DnaPos[i].y()-size.height()/2.0/this->Height());
		}
		DnaPos[i].setWidth(size.width());
		DnaPos[i].setHeight(size.height()>0.04?0.04:size.height()); 		
	}

	for(int i=0; i<smallMoleculePos.size();i++)
	{
		QSizeF size=getItemSize('S', i);	
		if(smallMoleculePos[i].x()!=-1)
		{			
	    smallMoleculePos[i].setX(smallMoleculePos[i].x()-size.width()/2.0/this->Width());
		smallMoleculePos[i].setY(smallMoleculePos[i].y()-size.height()/2.0/this->Height());
		}
		smallMoleculePos[i].setWidth(size.width());
		smallMoleculePos[i].setHeight(size.height()>0.04?0.04:size.height()); 		
	}		
}




vector<vector<vector<int>>> PathBubble1::recordEdge(vector<vector<vector<int>>> _biochemicalReaction)
{
	vector<vector<vector<int>>> edge;

/*
	 start[0]=edge[i][0], start[1]=edge[i][1];	
		 		 
		 middle[0]=edge[i][4]; middle[1]=edge[i][5]; 
		 
		 end[0]=edge[i][2], end[1]=edge[i][3]; 
		 */

	return edge;
}




QPointF PathBubble1::getLocation(int pathwayid, int stepid)
{
	float DX=0.02,DY=0.04;
	return( QPointF (stepid*DX+0.01, pathwayid*DY+0.1) );	
}

QRectF PathBubble1::LayoutDegradation(vector<vector<vector<int>>> _degradation, int bid, int sid)
{
      vector<int> left;
	  vector<int> right;	  
	  vector<int> aedge(7,-1);
	  bool flag=1;
	  int count=0;
	  
	  QPointF Pos=getLocation(0, sid);
	  //QPointF lPos(0,-1), rPos(0,-1);

	  left=_scene->_degradation[m_pathwayID][bid][0];
	  right=_scene->_degradation[m_pathwayID][bid][1];
	 
	  //locate first left and first right
	  float DY=0.05;
	  float DX=0.1;
	  float x=0,y=0;
	  	
	  for(int i=0; i<left.size(); i=i+2)
	  {
	     int type=left[i], id=left[i+1];
		 QSizeF size=getItemSize(type, id);	
 	     QRectF point(Pos.x() - size.width()/2 , Pos.y() + i*DY  - size.height()/2, size.width(), size.height());
		 
		 switch(type)
	     {
		     case 'C': if(complexPos[id].x()==-1) 
					   {
							
						    flag=0;
					   }
				       else 
					   {   
						    float temp=complexPos[id].x();
						   x=x+complexPos[id].x() +  complexPos[id].width()/2 ; y=y+complexPos[id].y() + complexPos[id].height()/2;
						   count++;
						   
					   }					   
					  break;
			 case 'D': //DnaPos[id]=point;  x=x+point.x() +  size.width()/2 ; y=y+point.y() + size.height()/2; 
				      break;    				 
			 case 'E': if(physicalEntityPos[id].x()==-1) 
					   {	
						    flag=0;
						
					   }
				       else 
					   {   
						   float temp=physicalEntityPos[id].x();
						   //physicalEntityPos[id].setX(physicalEntityPos[id].x()-point.width()/2.0/this->Width()), physicalEntityPos[id].setY(physicalEntityPos[id].y()-point.height()/2.0/this->Height()), physicalEntityPos[id].setWidth(point.width()), physicalEntityPos[id].setHeight(point.height()); 				           						   						   						  
						   x=x+physicalEntityPos[id].x() +  physicalEntityPos[id].width()/2 ; y=y+physicalEntityPos[id].y() + physicalEntityPos[id].height()/2;
						   count++;
					   }					   
					  break;	
			 case 'P': 
				       if(proteinPos[id].x()==-1) 
					   {
						   flag=0;
						  }
				       else 
					   {   
						   float temp=proteinPos[id].x();
						   //proteinPos[id].setX(proteinPos[id].x()-point.width()/2.0/this->Width()), proteinPos[id].setY(proteinPos[id].y()-point.height()/2.0/this->Height()), proteinPos[id].setWidth(point.width()), proteinPos[id].setHeight(point.height()); 				           						   						   						  
						   x=x+proteinPos[id].x() +  proteinPos[id].width()/2 ; y=y+proteinPos[id].y() + proteinPos[id].height()/2;
						   count++;
					   }					   
					   break;				      
			 case 'S': 
				      //if(smallMoleculePos[id].x()==1) 
					  {
						 //smallMoleculePos[id].push_back(point);  x=x+point.x() +  size.width()/2 ; y=y+point.y() + size.height()/2;  		
					  }
					  //else
					  {
					      //x = x + smallMoleculePos[id].x() +  smallMoleculePos[id].width()/2;  y = y + smallMoleculePos[id].y() + smallMoleculePos[id].height()/2;
					  }
					  break;	
		 }
	  }
	  for(int i=0; i<right.size();  i=i+2)
	  {
	     int type=right[i], id=right[i+1];	

		 QSizeF size=getItemSize(type, id);	
		 QRectF point(Pos.x() + DX - size.width()/2, Pos.y() + i*DY - size.height()/2, size.width(), size.height());		
		 
		 switch(type)
	     {
		     case 'C': if(complexPos[id].x()==-1) 
						{
							flag=0;
							
					   }
				       else 
					   {   
						   //complexPos[id].setX(complexPos[id].x()-point.width()/2.0/this->Width()), complexPos[id].setY(complexPos[id].y()-point.height()/2.0/this->Height()), complexPos[id].setWidth(point.width()), complexPos[id].setHeight(point.height()); 				           						   	
						   float temp=complexPos[id].x();
						   x= x+complexPos[id].x() +  complexPos[id].width()/2 ; y=y+complexPos[id].y() + complexPos[id].height()/2;
						   count++;
					   }					  
					   break;			
			 case 'D': //DnaPos[id]=point;    x=x+point.x() +  size.width()/2 ; y=y+point.y() + size.height()/2;  
				       break;
			 case 'E':  if(physicalEntityPos[id].x()==-1) 
						{
							flag=0;
							//temporily disabled
							//physicalEntityPos[id]=point; 
							//x=x+point.x() +  size.width()/2 ; y=y+point.y() + size.height()/2;							
					   }
				       else 
					   {   
						   //physicalEntityPos[id].setX(physicalEntityPos[id].x()-point.width()/2.0/this->Width()), physicalEntityPos[id].setY(physicalEntityPos[id].y()-point.height()/2.0/this->Height()), physicalEntityPos[id].setWidth(point.width()), physicalEntityPos[id].setHeight(point.height()); 				           						   						   						  
						   float temp=physicalEntityPos[id].x();
						   x=x+physicalEntityPos[id].x() +  physicalEntityPos[id].width()/2 ; y=y+physicalEntityPos[id].y() + physicalEntityPos[id].height()/2;
						   count++;
					   }					   
					  break;	
			 case 'P':  if(proteinPos[id].x()==-1) 
						{
							flag=0;
							//temporily disabled
							//proteinPos[id]=point; 
							//x=x+point.x() +  size.width()/2 ; y=y+point.y() + size.height()/2;
					   }
				       else 
					   {   
						   float temp=proteinPos[id].x();
						   //proteinPos[id].setX(proteinPos[id].x()-point.width()/2.0/this->Width()), proteinPos[id].setY(proteinPos[id].y()-point.height()/2.0/this->Height()), proteinPos[id].setWidth(point.width()), proteinPos[id].setHeight(point.height()); 				           						   						   						  
						   x=x+proteinPos[id].x() +  proteinPos[id].width()/2 ; y=y+proteinPos[id].y() + proteinPos[id].height()/2;
						   count++;
					   }					   
					  break;	
			 case 'S': //			        smallMoleculePos[id]=point;  x=x+point.x() +  size.width()/2 ; y=y+point.y() + size.height()/2; 
				 break;		
			 
		 }
	  }
	  //gravity center
	  //if(flag&&count>0)	  
	  if(flag&&count>0)	  
	  {
		  degradationPos[bid].setX(x/count);
	      degradationPos[bid].setY(y/count);

	      QSizeF size=getItemSize('R', bid);	
	      degradationPos[bid].setWidth(size.width()); 
	      degradationPos[bid].setHeight(size.height());
	  
		  //remember to change the reading in read3pathwayStep_reactionAndcontrol
		  //output edges
	      for(int i=0; i<left.size(); i=i+2)
	      {
			 int type=left[i], id=left[i+1];
		     aedge[0]=type, aedge[1]=id;

		     type=right[i], id=right[i+1];
		     aedge[2]=type, aedge[3]=id;

		
		     aedge[4]='R';
		     aedge[5]=bid;
		     aedge[6]=whichPathway(sid);

			 if(edge.size()==11)
				 edge=edge;
		     edge.push_back(aedge);	
			 
		  }
	  }	  
	  return degradationPos[bid];
}

QRectF PathBubble1::LayoutBiochemicalReaction(vector<vector<vector<int>>> _biochemicalReaction, int bid, int sid)
{
	  vector<int> left;
	  vector<int> right;	  
	  vector<int> aedge(7,-1);
	  bool flag=1;
	  int count=0;
	  
	  QPointF Pos=getLocation(0, sid);	  
	  for(int i=0; i<_biochemicalReaction[bid][0].size(); i=i+2) //ignore small mollecular
	  {
          if(_biochemicalReaction[bid][0][i]!='S' || drawSmallmolecule)
		  {
			  left.push_back(_biochemicalReaction[bid][0][i]);  
			  left.push_back(_biochemicalReaction[bid][0][i+1]);		  
		  }
	  }
	  for(int i=0; i<_biochemicalReaction[bid][1].size();  i=i+2)
	  {
	      if(_biochemicalReaction[bid][1][i]!='S'|| drawSmallmolecule)
		  {		  		  
		      right.push_back(_biochemicalReaction[bid][1][i]);  
			  right.push_back(_biochemicalReaction[bid][1][i+1]);	
		  }
	  }
	 
	  //locate first left and first right
	  float DY=0.05;
	  float DX=0.1;
	  float x=0,y=0;
	  	
	  for(int i=0; i<left.size(); i=i+2)
	  {
		 int type=left[i], id=left[i+1];		 
		 QSizeF size=getItemSize(type, id);	
 	     QRectF point(Pos.x() - size.width()/2 , Pos.y() + i*DY  - size.height()/2, size.width(), size.height());		 
		 switch(type)
	     {
		     case 'C': /*if(complexPos[id].x()==-1) 
					   {
						   flag=0;
					   }
				       else*/ 
					   {   
						   float temp=complexPos[id].x();
						   x=x+complexPos[id].x() +  complexPos[id].width()/2 ; y=y+complexPos[id].y() + complexPos[id].height()/2;
						   count++;						   
					   }					   
					  break;
			 case 'D': //DnaPos[id]=point;  x=x+point.x() +  size.width()/2 ; y=y+point.y() + size.height()/2; 
				      break;    				 
			 case 'E': /*if(physicalEntityPos[id].x()==-1) 
					   {	
						    flag=0;
						
					   }
				       else */
					   {   
						   float temp=physicalEntityPos[id].x();
						   //physicalEntityPos[id].setX(physicalEntityPos[id].x()-point.width()/2.0/this->Width()), physicalEntityPos[id].setY(physicalEntityPos[id].y()-point.height()/2.0/this->Height()), physicalEntityPos[id].setWidth(point.width()), physicalEntityPos[id].setHeight(point.height()); 				           						   						   						  
						   x=x+physicalEntityPos[id].x() +  physicalEntityPos[id].width()/2 ; y=y+physicalEntityPos[id].y() + physicalEntityPos[id].height()/2;
						   count++;
					   }					   
					  break;	
			 case 'P': 
				       /*if(proteinPos[id].x()==-1) 
					   {
						   flag=0;
						  }
				       else */
					   {   
						   float temp=proteinPos[id].x();
						   //proteinPos[id].setX(proteinPos[id].x()-point.width()/2.0/this->Width()), proteinPos[id].setY(proteinPos[id].y()-point.height()/2.0/this->Height()), proteinPos[id].setWidth(point.width()), proteinPos[id].setHeight(point.height()); 				           						   						   						  
						   x=x+proteinPos[id].x() +  proteinPos[id].width()/2 ; y=y+proteinPos[id].y() + proteinPos[id].height()/2;
						   count++;
					   }					   
					   break;				      
			 case 'S': 
				      //if(smallMoleculePos[id].x()==1) 
					  {
						 //smallMoleculePos[id].push_back(point);  x=x+point.x() +  size.width()/2 ; y=y+point.y() + size.height()/2;  		
					  }
					  //else
					  {
					      //x = x + smallMoleculePos[id].x() +  smallMoleculePos[id].width()/2;  y = y + smallMoleculePos[id].y() + smallMoleculePos[id].height()/2;
					  }
					  break;	
		 }
	  }
	  for(int i=0; i<right.size();  i=i+2)
	  {
	     int type=right[i], id=right[i+1];	

		 QSizeF size=getItemSize(type, id);	
		 QRectF point(Pos.x() + DX - size.width()/2, Pos.y() + i*DY - size.height()/2, size.width(), size.height());		
		 
		 switch(type)
	     {
		     case 'C': /*if(complexPos[id].x()==-1) 
						{
							flag=0;
							
					   }
				       else */
					   {   
						   //complexPos[id].setX(complexPos[id].x()-point.width()/2.0/this->Width()), complexPos[id].setY(complexPos[id].y()-point.height()/2.0/this->Height()), complexPos[id].setWidth(point.width()), complexPos[id].setHeight(point.height()); 				           						   	
						   float temp=complexPos[id].x();
						   x= x+complexPos[id].x() +  complexPos[id].width()/2 ; y=y+complexPos[id].y() + complexPos[id].height()/2;
						   count++;
					   }					  
					   break;			
			 case 'D': //DnaPos[id]=point;    x=x+point.x() +  size.width()/2 ; y=y+point.y() + size.height()/2;  
				       break;
			 case 'E':  /*if(physicalEntityPos[id].x()==-1) 
						{
							flag=0;												
					   }
				       else */
					   {   
						   //physicalEntityPos[id].setX(physicalEntityPos[id].x()-point.width()/2.0/this->Width()), physicalEntityPos[id].setY(physicalEntityPos[id].y()-point.height()/2.0/this->Height()), physicalEntityPos[id].setWidth(point.width()), physicalEntityPos[id].setHeight(point.height()); 				           						   						   						  
						   float temp=physicalEntityPos[id].x();
						   x=x+physicalEntityPos[id].x() +  physicalEntityPos[id].width()/2 ; y=y+physicalEntityPos[id].y() + physicalEntityPos[id].height()/2;
						   count++;
					   }					   
					  break;	
			 case 'P':  /*if(proteinPos[id].x()==-1) 
						{
							flag=0;							
					   }
				       else */
					   {   
						   float temp=proteinPos[id].x();
						   //proteinPos[id].setX(proteinPos[id].x()-point.width()/2.0/this->Width()), proteinPos[id].setY(proteinPos[id].y()-point.height()/2.0/this->Height()), proteinPos[id].setWidth(point.width()), proteinPos[id].setHeight(point.height()); 				           						   						   						  
						   x=x+proteinPos[id].x() +  proteinPos[id].width()/2 ; y=y+proteinPos[id].y() + proteinPos[id].height()/2;
						   count++;
					   }					   
					  break;	
			 case 'S': // smallMoleculePos[id]=point;  x=x+point.x() +  size.width()/2 ; y=y+point.y() + size.height()/2; 
				 break;		
			 
		 }
	  }
	  //gravity center
	  //if(flag&&count>0)	  
	  if(flag&&count>0)	  
	  {
		  reactionPos[bid].setX(x/count);
	      reactionPos[bid].setY(y/count);

	      QSizeF size=getItemSize('R', bid);	
	      reactionPos[bid].setWidth(size.width()); 
	      reactionPos[bid].setHeight(size.height());
	  
		  //remember to change the reading in read3pathwayStep_reactionAndcontrol
		  //output edges
	      for(int i=0; i<left.size(); i=i+2)
	      {
			 int type=left[i], id=left[i+1];
		     aedge[0]=type, aedge[1]=id;

		     type=right[i], id=right[i+1];
		     aedge[2]=type, aedge[3]=id;

		
		     aedge[4]='R';
		     aedge[5]=bid;
		     aedge[6]=whichPathway(sid);

			 if(edge.size()==11)
				 edge=edge;
		     edge.push_back(aedge);	
			 
		  }
	  }	  
	  return reactionPos[bid];
}


int PathBubble1::whichPathway(int id)
{	
	for(int i=0; i<_1pathway.size(); i++)
	{
		if(_1pathway[i].find(id)!=_1pathway[i].end())
			return i;
	}
	return -1;
}




int PathBubble1::read1pathway(const char *name, vector<set<int>> &_1pathway)
{//return maximum step
	char ch[100];
	FILE *fp = fopen(name,"r"); 	
	char e;		
	int pid, sid, pid2=-1, sid2=-1;
	int count=0;
	int max=-100000;
	set<int> empty;
	_1pathway.push_back(empty);	
	do
	{
		  //fscanf(fp,"Pathway%d Pathway Step%d\n",&pid, &sid);	
		 vector<string> buffer;
	      string phrase; 
		  e='x';
		  do
		  {  e=fgetc(fp);			 
			  if(e=='\t'||e=='\n')
			 {
				 buffer.push_back(phrase);
				 phrase.clear();
			 }
			 else if(e>-1) phrase.append(1,e);
		  }while(e!='\n'&&e>-1); 

		 if(e<0||(e=='\n'&&buffer[0].empty()))
			  break;

		  //process phrase
		  //check pid2 
		  for(int i=0; i<buffer.size(); i++)
		  {
			  string result;
		      for(unsigned int j = 0; j < buffer[i].size();j++)
              if(isdigit(buffer[i][j]))
              {   
				  result.append(1,buffer[i][j]);                 
			  }	
			  if(i==0)	 pid= atoi(result.c_str());  
			  else if(i==1) sid= atoi(result.c_str());
		  }
		  	 
	      if(pid2==pid&&sid2==sid)
		 	 break;
		 
		  if(pid!=pid2)
		  {
			 _1pathway.push_back(empty);	
			 count++;
		  }
		  if(sid!=sid2)
		  {
		     _1pathway[count].insert(sid);	
		  
		  }
		  if(sid>max)
			  max=sid;
	      pid2=pid;   		  
		  sid2=sid;  
	 }while(true);

	 fclose(fp);	
	 return max;
}

int PathBubble1::read2pathwayStepOrder(const char *name, int stepNum, vector<vector<int>> &_pathwayStep)
{
	char ch[100];
	FILE *fp = fopen(name,"r"); 	
	char e;			
	int pid, cid, pid2=-1, cid2=-1;
	int max=-100000;	
	do
	{
		  //fscanf(fp,"Pathway Step%d Pathway Step%d\n",&pid, &cid);		
		 vector<string> buffer;
	      string phrase; 
		  e='x';
		  do
		  {  e=fgetc(fp);			 
			  if(e=='\t'||e=='\n')
			 {
				 buffer.push_back(phrase);
				 phrase.clear();
			 }
			 else if(e>-1) phrase.append(1,e);
		  }while(e!='\n'&&e>-1); 

		 if(e<0||(e=='\n'&&buffer[0].empty()))
			  break;

		  //process phrase
		  //check pid2 
		  for(int i=0; i<buffer.size(); i++)
		  {
			  string result;
		      for(unsigned int j = 0; j < buffer[i].size();j++)
              if(isdigit(buffer[i][j]))
              {   
				  result.append(1,buffer[i][j]);                 
			  }	
			  if(i==0)	 pid= atoi(result.c_str());  
			  else if(i==1) cid= atoi(result.c_str());
		  }

	      if(pid2==pid&&cid2==cid)
		      break;
		  if(cid>max)
			  max=cid;			  

		  if(pid>max)
			  max=pid;			  
		  
	      pid2=pid;   		  
		  cid2=cid;  
	 }while(true);
	 fclose(fp);

	 if(max>stepNum)
		 stepNum=max;

	_pathwayStep.clear();
	vector<int> empty;
	vector<vector<int>> temp(stepNum+1,empty);
	_pathwayStep=temp;

	fp = fopen(name,"r"); 

	pid2=-1, cid2=-1;
	do
	{
		  //fscanf(fp,"Pathway Step%d Pathway Step%d\n",&pid, &cid);	
		  vector<string> buffer;
	      string phrase; 
		  e='x';
		  do
		  {  e=fgetc(fp);			 
			  if(e=='\t'||e=='\n')
			 {
				 buffer.push_back(phrase);
				 phrase.clear();
			 }
			 else if(e>-1) phrase.append(1,e);
		  }while(e!='\n'&&e>-1); 

		 if(e<0||(e=='\n'&&buffer[0].empty()))
			  break;

		  //process phrase
		  //check pid2 
		  for(int i=0; i<buffer.size(); i++)
		  {
			  string result;
		      for(unsigned int j = 0; j < buffer[i].size();j++)
              if(isdigit(buffer[i][j]))
              {   
				  result.append(1,buffer[i][j]);                 
			  }	
			  if(i==0)	 pid= atoi(result.c_str());  
			  else if(i==1) cid= atoi(result.c_str());
		  }

	      if(pid2==pid&&cid2==cid)
		      break;

		  _pathwayStep[pid].push_back(cid);			  
		  
	      pid2=pid;   		  
		  cid2=cid;  
	 }while(true);
	 fclose(fp);

	 return stepNum;
}

void PathBubble1::read3pathwayStep_reactionAndcontrol(const char *name, int stepNum, vector<vector<int>> &_3pathwayStepCatalysis, vector<int> &_3pathwayStepReaction)
{//return maximum step
	char ch[100];
	char c,o,c2,o2;
	FILE *fp = fopen(name,"r"); 
	char e;			
	int pid, sid, pid2=-1, sid2=-1;
	c2='x';
	o2='x';

	vector<int> temp(stepNum+1, -1);
	vector<int> unit(4,-1);
	vector<vector<int>> utemp(stepNum+1, unit);

	_3pathwayStepCatalysis.clear();
	_3pathwayStepCatalysis=utemp;
	_3pathwayStepReaction=temp;

	do
	{
		  //fscanf(fp,"Pathway Step%d %c",&pid, &c);	
		  vector<string> buffer;
	      string phrase; 
		  e='x';
		  do
		  {  e=fgetc(fp);			 
			 if(e=='\t'||e=='\n')
			 {
				 buffer.push_back(phrase);
				 phrase.clear();
			 }
			 else phrase.append(1,e);
		  }while(e!='\n'&&e>-1); 

		  if(e<0||(e=='\n'&&buffer[0].empty()))
			  break;

		  //process phrase
		  //check pid2 
		  for(int i=0; i<buffer.size(); i++)
		  {
			  string result;
		      for(unsigned int j = 0; j < buffer[i].size();j++)
              if(isdigit(buffer[i][j]))
              {   
				  result.append(1,buffer[i][j]);                 
			  }	
			  if(i==0)	 pid= atoi(result.c_str());  
			  else if(i==1) sid= atoi(result.c_str());
		  }
		  if(pid==9&&sid==9)
			  pid=pid;
		  c=buffer[1][0];
		  o=buffer[1][1]; 	     		 
		  if(c=='C') 
		  {
			  //fscanf(fp,"%c", &o);					 
			  if(o=='a')//related to Control
			  {
				  string result;
				  for(unsigned int j = 0; j < buffer[1].size();j++)
                  if(isdigit(buffer[1][j]))
                  {   
				     result.append(1,buffer[1][j]);                 
			      }	
			      sid= atoi(result.c_str());  
				  //fscanf(fp,"talysis%d\n", &sid);		
			      _3pathwayStepCatalysis[pid][0]=sid;
			  }
			  else if(o=='o')//related to Catalysis
			  {
				  string result;
				  for(unsigned int j = 0; j < buffer[1].size();j++)
                  if(isdigit(buffer[1][j]))
                  {   
				     result.append(1,buffer[1][j]);                 
			      }	
			      sid= atoi(result.c_str()); 
				  //fscanf(fp,"ntrol%d\n", &sid);		
			      _3pathwayStepCatalysis[pid][1]=sid;
			  }
		  }
		  else if(c=='D') //related to degradation
		  {
			  string result;
			  for(unsigned int j = 0; j < buffer[1].size();j++)
              if(isdigit(buffer[1][j]))
              {   
			     result.append(1, buffer[1][j]);                 
			  }	
			  sid= atoi(result.c_str()); 
			  //fscanf(fp,"egradation%d\n", &sid);		
			  _3pathwayStepCatalysis[pid][2]=sid;
		  }
		  else if(c=='P') //connected to new pathway
		  {
			   string result;
			   for(unsigned int j = 0; j < buffer[1].size();j++)
               if(isdigit(buffer[1][j]))
               {   
				     result.append(1,buffer[1][j]);                 
			   }	
			   sid= atoi(result.c_str()); 
			   //fscanf(fp,"athway%d\n", &sid);		
			  _3pathwayStepCatalysis[pid][3]=sid;
		  }		 
		  else if(c=='B')//connected to a reaction
		  {    
			   string result;
			   for(unsigned int j = 0; j < buffer[1].size();j++)
               if(isdigit(buffer[1][j]))
               {   
				     result.append(1,buffer[1][j]);                 
			   }	
			   sid= atoi(result.c_str()); 
			   //fscanf(fp,"iochemical Reaction%d\n", &sid);		
			  _3pathwayStepReaction[pid]=sid;
		  }
		  else
		  {
		    sid=sid;		  
		  }

		  if(pid2==pid&&sid2==sid&&(c=='C'?(c==c2&&o==o2):c==c2))
		 	  break;
	      pid2=pid;   		  
		  sid2=sid;  
		  c2=c; 
		  o2=o; 
	 }while(true);	
}


vector<int> PathBubble1::read4biochemicalReaction(const char *name, vector<vector<vector<int>>> &_biochemicalReaction, vector<vector<QString>> &_reactionName, vector<vector<vector<int>>> &_degradation, vector<vector<QString>> &_degradationName, vector<vector<QString>> &_EmptyName, int &reactionNum, int &physicalEntityNum, int &smallMoleculeNum, int &DnaNum, int &degradationNum)
{   //complex/Dna/Physical Entity/protein/small molecule
	vector<int> empty;	
	char ch[100];
	char c1,c2,o1,o2,c12,c22,o12,o22;
	FILE *fp = fopen(name,"r"); 
	if(!fp)
		return empty;
	//vector<vector<QString>> _EmptyName;
	char e;	
	int bid, lid, rid, bid2=-1, lid2=-1, rid2=-1;
	c12='x'; c22='x';
	o12='x'; o22='x';
	int max=0, demax=0, smax=0, dmax=0, emax=0, nmax=0;
	do
	{
	      //fscanf(fp,"Biochemical Reaction%d",&bid);	
		  vector<string> buffer;
	      string phrase; 
		  e='x';
		  do
		  {  e=fgetc(fp);			 
			 if(e=='\t'||e=='\n')
			 {
				 buffer.push_back(phrase);
				 phrase.clear();
			 }
			 else phrase.append(1,e);
		  }while(e!='\n'&&e>-1); 

		  if(e<0||(e=='\n'&&buffer[0].empty()))
			  break;

		  //process phrase
		  //check pid2 
		  if(buffer[0][0]=='D')
			  e=e;
		  for(int i=0; i<buffer.size(); i++)
		  {
			  string result;
		      for(unsigned int j = 0; j < buffer[i].size();j++)
              if(isdigit(buffer[i][j]))
              {   
				  result.append(1,buffer[i][j]);  
				  
			  }	
			  if(i==0)	 bid= atoi(result.c_str());  
			  else if(i==2) lid= atoi(result.c_str());
			  else if(i==3) rid= atoi(result.c_str());
		  }
          if(bid==40)
			  bid=bid;
		  	  
		  c1=buffer[2][0];
		  if(buffer.size()>3&&buffer[3].size()>0)
		     c2=buffer[3][0];
		  else c2='n';

		  o1=buffer[2][1];
		  
		  if(buffer.size()>3&&buffer[3].size()>0)
			  o2=buffer[3][1];
		  else o2='n';

		  if(c1=='C')
		  {
			  //fscanf(fp,"omplex%d", &lid);				  	  
		  }
		  else if(c1=='D')
		  {
			    //fscanf(fp,"na%d", &lid);	
			  	if(lid>dmax)
			        dmax=lid;
		  }	
		  else if(c1=='P')
		  {
			  //fscanf(fp,"%c",&o1);	
			  if(o1=='h')
			  {
				 if(lid>emax)
			        emax=lid;	
		      }
			   
		  }
          else if(c1=='S')
		  {
			  //fscanf(fp,"mall Molecule%d", &lid);	
			    if(lid>smax)
			    smax=lid;
		  }			   
		  //fscanf(fp," %c",&c2);	
		  if(c2=='C')
		  {
			  //fscanf(fp,"omplex%d", &rid);				    
		  }
		  else if(c2=='D')
		  {
			  //fscanf(fp,"na%d", &rid);	
			  if(rid>dmax)
			    dmax=rid;	
		  }	
		  else if(c2=='P')
		  {
			   if(o2=='h')
			  {
				 if(rid>emax)
			        emax=rid;	
		      }			   
		  }
          else if(c2=='S')
		  {
			  if(rid>smax)
			    smax=rid;	
		  }	
		  else if(c2=='n') //empty for some case
		  {
			  if(buffer[0][0]=='B')
			  {
				  nmax++;
				  //_biochemicalReaction[bid][1].push_back('N');				  
			      //_biochemicalReaction[bid][1].push_back(1);				  
			  }
		  }		
		  bid2=bid;   		  
		  lid2=lid;  
		  rid2=rid; 
		  o12=o1;   o22=o2;  
		  c12=c1;   c22=c2;  
		  rid2=rid; 
		  if(buffer[0][0]=='B')
		  {
			  if(bid>max)
			      max=bid;	
		  }
		  else if(buffer[0][0]=='D')
		  {
			  if(bid>demax)
				  demax=bid;
		  }
	 }while(true);	
	 
	 fclose(fp);	

	 smallMolecule.clear(); Dna.clear();

	_biochemicalReaction.clear();
	
	vector<vector<int>> temp2(2,empty);
	vector<vector<vector<int>>> temp3(max+1,temp2);
	_biochemicalReaction=temp3;

	vector<vector<vector<int>>> temp4(demax+1,temp2);
	_degradation=temp4;

	 QString a;
	vector<QString> empty3(2,a);
	vector<vector<QString>> temps(max+1,empty3);
	_reactionName = temps;

	vector<vector<QString>> temps4(demax+1,empty3);
	_degradationName = temps4;

      vector<QString> tempE; 
      _EmptyName.push_back(tempE);
	//vector<vector<QString>> tempN(nmax+1,empty);
    //_EmptyName = tempN;

	fp = fopen(name,"r"); 
	
	bid2=-1, lid2=-1, rid2=-1;
	do
	{
		  vector<string> buffer;
	      string phrase; 
		  e='x';
		  do
		  {  e=fgetc(fp);			 
			 if(e=='\t'||e=='\n')
			 {
				 buffer.push_back(phrase);
				 phrase.clear();
			 }
			 else phrase.append(1,e);
		  }while(e!='\n'&&e>-1); 

		  if(e<0||(e=='\n'&&buffer[0].empty()))
			  break;

		  //process phrase
		  //check pid2 
		  for(int i=0; i<buffer.size(); i++)
		  {
			  string result;
		      for(unsigned int j = 0; j < buffer[i].size();j++)
              if(isdigit(buffer[i][j]))
              {   
				  result.append(1,buffer[i][j]);                 
			  }	
			  if(i==0)	 bid= atoi(result.c_str());  
			  else if(i==2) lid= atoi(result.c_str());
			  else if(i==3) rid= atoi(result.c_str());
		  }     
		  //fscanf(fp," %c",&c1);	
		  c1=buffer[2][0];
		  if(buffer.size()>3&&buffer[3].size()>0)
		     c2=buffer[3][0];
		  else 
		  {
			  c2='n';
			
		  }
	
		  if(buffer[0][0]=='B')
			 _reactionName[bid][1]=buffer[1].c_str();
		  else if(buffer[0][0]=='D')
			 _degradationName[bid][1]=buffer[1].c_str();

	      //fscanf(fp,"Biochemical Reaction%d",&bid);		  	     
		  //fscanf(fp," %c",&c1);		  
		  if(c1=='C')
		  {
			  //fscanf(fp,"omplex%d", &lid);
			  if(buffer[0][0]=='B')
			  {	
				_biochemicalReaction[bid][0].push_back('C');	
				_biochemicalReaction[bid][0].push_back(lid);				  
			  }
			  else if(buffer[0][0]=='D')
			  {
			    _degradation[bid][0].push_back('C');	
				_degradation[bid][0].push_back(lid);	
			  }
		  }
		  else if(c1=='D')
		  {
			  //fscanf(fp,"na%d", &lid);	
			  Dna.push_back(lid);
			   if(buffer[0][0]=='B')
			   {
				   _biochemicalReaction[bid][0].push_back('D');					  
			       _biochemicalReaction[bid][0].push_back(lid);					  
			   }
			    else if(buffer[0][0]=='D')
			  {
			    _degradation[bid][0].push_back('D');	
				_degradation[bid][0].push_back(lid);	
			  }

		  }	
		  else if(c1=='P')
		  {
			  //fscanf(fp,"%c",&o1);	
			  o1=buffer[2][1];
			  if(o1=='h')
			  {
				  if(buffer[0][0]=='B')
				  {
					_biochemicalReaction[bid][0].push_back('E');	
					_biochemicalReaction[bid][0].push_back(lid);		
				  }
				  else if(buffer[0][0]=='D')
				  {
					_degradation[bid][0].push_back('E');	
					_degradation[bid][0].push_back(lid);	
				  }
		      }
			  else if(o1=='r')
			  {
				  //fscanf(fp,"otein%d", &lid);	
				  if(buffer[0][0]=='B')
				  {
					_biochemicalReaction[bid][0].push_back('P');	
					_biochemicalReaction[bid][0].push_back(lid);		
				  }
				  else if(buffer[0][0]=='D')
				  {
					_degradation[bid][0].push_back('P');	
					_degradation[bid][0].push_back(lid);	
				  }
			  }			  
		  }
          else if(c1=='S')
		  {
			  //fscanf(fp,"mall Molecule%d", &lid);	
			  smallMolecule.push_back(lid);
			  if(buffer[0][0]=='B')
			  {
				  _biochemicalReaction[bid][0].push_back('S');				  
				_biochemicalReaction[bid][0].push_back(lid);				  
			  }
			  else if(buffer[0][0]=='D')
			  {
					_degradation[bid][0].push_back('S');	
					_degradation[bid][0].push_back(lid);	
			  }
		  }	   
		  //fscanf(fp," %c",&c2);	
		  if(c2=='C')
		  {
			  //fscanf(fp,"omplex%d", &rid);	
			  _biochemicalReaction[bid][1].push_back('C');				  
			  _biochemicalReaction[bid][1].push_back(rid);				  
		  }
		   else if(c2=='D')
		  {
			  //fscanf(fp,"na%d", &rid);
			  Dna.push_back(rid);
			  _biochemicalReaction[bid][1].push_back('D');				  
			  _biochemicalReaction[bid][1].push_back(rid);				  				  
		  }	
		  else if(c2=='P')
		  {
			  //fscanf(fp,"%c",&o2);
			  if(buffer.size()>3&&buffer[3].size()>0)
			  o2=buffer[3][1];
		      else o2='n';
			  if(o2=='h')
			  {
				  //fscanf(fp,"ysical Entity%d", &rid);	
				  _biochemicalReaction[bid][1].push_back('E');				  
			      _biochemicalReaction[bid][1].push_back(rid);				  	
		      }
			  else if(o2=='r')
			  {
				  //fscanf(fp,"otein%d", &rid);		
				  _biochemicalReaction[bid][1].push_back('P');				  
			      _biochemicalReaction[bid][1].push_back(rid);				  	
			  }			  
		  }
          else if(c2=='S')
		  {
			  //fscanf(fp,"mall Molecule%d", &rid);	
			  smallMolecule.push_back(rid);
			  _biochemicalReaction[bid][1].push_back('S');				  
			  _biochemicalReaction[bid][1].push_back(rid);				  
		  }	
		  else if(c2=='n') //empty for some case
		  {
			  if(buffer[0][0]=='B')
			  {
				  vector<QString> tempE; tempE.push_back(buffer[1].c_str());
				  _EmptyName.push_back(tempE);
				  _biochemicalReaction[bid][1].push_back('N');				  
			      _biochemicalReaction[bid][1].push_back(_EmptyName.size());				  
			  }
		  }			  
		  //fscanf(fp,"\n");	
		  //if(bid2==bid&&lid2==lid&&rid2==rid&&( c1=='P'? (c1==c12 && o1==o12):c1==c12) && (c2=='P'?(c2==c22 && o2==o22):c2==c22))
		  //  break;

		  bid2=bid;   		  
		  lid2=lid;  
		  rid2=rid; 
		  o12=o1;   o22=o2;  
		  c12=c1;   c22=c2;  
		  rid2=rid; 
	 }while(true);

	 fclose(fp);

	 empty.clear();
	 empty.push_back(max);
	 empty.push_back(emax);
	 empty.push_back(smax);
	 empty.push_back(dmax);
	 empty.push_back(nmax);

	 for(int i=0; i<_reactionName.size(); i++)
	 {
	    int lcount=1, rcount=1;
		for(int j=2; j<_biochemicalReaction[i][0].size(); j=j+2)	    
		{
		  if(_biochemicalReaction[i][0][j]!=_biochemicalReaction[i][0][j-2] || _biochemicalReaction[i][0][j+1]!=_biochemicalReaction[i][0][j-1])
		    lcount++;
		}	 
		for(int j=2; j<_biochemicalReaction[i][1].size(); j=j+2)	    
		{
		  if(_biochemicalReaction[i][1][j]!=_biochemicalReaction[i][1][j-2] || _biochemicalReaction[i][1][j+1]!=_biochemicalReaction[i][1][j-1])
		    rcount++;
		}	
		if(lcount==rcount)
              _reactionName[i][0]='T';//transition/process
		else 
		{
			if(lcount==1)
               _reactionName[i][0]='D';  //disscociation
		    else if(rcount==1)
			  _reactionName[i][0]='B';  //bind
			else  _reactionName[i][0]='T';//transition/process
		}
	 }
	 reactionNum = empty[0], physicalEntityNum=empty[1], smallMoleculeNum=empty[2], DnaNum=empty[3]; degradationNum=empty[3];  ANodeNum=0;
	 return empty;
}


void PathBubble1::read5catalysisANDcontrol(const char *name, vector<vector<int>> &_Catalysis, vector<vector<int>> &_5Control)
{//return maximum step
	char ch[100];
	char C, c1, c2, C2;
	FILE *fp;
	char e;	
	int cid, mid, bid, cid2, mid2, bid2;
	int count=-1;
	int max1=0,max2=0;
	fp = fopen(name,"r"); 	
	if(!fp)
		return;
	C2='x';
	cid2=-1, mid2=-1, bid2=-1;	
	do
	{	
		  //fscanf(fp,"%c%c", &C, &C);	
		  vector<string> buffer;
	      string phrase; 
		  e='x';
		  do
		  {  e=fgetc(fp);			 
			 if(e=='\t'||e=='\n')
			 {
				 buffer.push_back(phrase);
				 phrase.clear();
			 }
			 else phrase.append(1,e);
		  }while(e!='\n'&&e>-1); 

		  if(e<0||(e=='\n'&&buffer[0].empty()))
			  break;

		  //process phrase
		  //check pid2 
		  for(int i=0; i<buffer.size(); i++)
		  {
			  string result;
		      for(unsigned int j = 0; j < buffer[i].size();j++)
              if(isdigit(buffer[i][j]))
              {   
				  result.append(1,buffer[i][j]);                 
			  }	
			  if(i==0)	 cid= atoi(result.c_str());  
			  else if(i==1) mid= atoi(result.c_str());
			  else if(i==2) bid= atoi(result.c_str());
		  }     
		  //fscanf(fp," %c",&c1);	
		  C=buffer[0][1];
		  //cid=buffer[3][0];
		 

	      if(C=='a')
		  {
		      //fscanf(fp,"talysis%d", &cid);	
			  //fscanf(fp," %c", &c1);	
			  //fscanf(fp," %c", &c2);				  
			  //fscanf(fp," %s", ch);
			  if(cid>max1)
				  max1=cid;			  
		  }
		  else if(C=='o')
		  {		  
			  //fscanf(fp,"ntrol%d", &cid);	
			  //fscanf(fp," %c", &c1);	
			  //fscanf(fp," %c", &c2);				  
			  if(cid>max2)
				  max2=cid;			  
		  }
		  if(bid2==bid&&cid2==cid&&mid2==mid&&C2==C)
		 	  break;

		  cid2=cid, mid2=mid, bid2=bid;	 C2=C;
	 }while(true);	
	 
	fclose(fp);

	 vector<int> empty(5,-1);
	vector<vector<int>> temp1(max1+1,empty);
	vector<vector<int>> temp2(max2+1,empty);
	_Catalysis=temp1;
	_5Control=temp2;

	fp = fopen(name,"r"); 
		
	C2='x';
	cid2=-1, mid2=-1, bid2=-1;	
	do
	{
		  //fscanf(fp,"%c%c", &C, &C);	
		  vector<string> buffer;
	      string phrase; 
		  e='x';
		  do
		  {  e=fgetc(fp);			 
			 if(e=='\t'||e=='\n')
			 {
				 buffer.push_back(phrase);
				 phrase.clear();
			 }
			 else phrase.append(1,e);
		  }while(e!='\n'&&e>-1); 

		  if(e<0||(e=='\n'&&buffer[0].empty()))
			  break;

		  //process phrase
		  //check pid2 
		  for(int i=0; i<buffer.size(); i++)
		  {
			  string result;
		      for(unsigned int j = 0; j < buffer[i].size();j++)
              if(isdigit(buffer[i][j]))
              {   
				  result.append(1,buffer[i][j]);                 
			  }	
			  if(i==0)	 cid= atoi(result.c_str());  
			  else if(i==1) mid= atoi(result.c_str());
			  else if(i==2) bid= atoi(result.c_str());
		  }     
		  //fscanf(fp," %c",&c1);	
		  C=buffer[0][1];
		  //cid=buffer[3][0];
		  c1=buffer[1][0];
		  c2=buffer[2][0];

	      if(C=='a')
		  {
		      //fscanf(fp,"talysis%d", &cid);	
			  //fscanf(fp," %c", &c1);	
			  if(c1=='P')
			  {
				  //fscanf(fp,"rotein%d", &mid);
				  _Catalysis[cid][0]='P';
				  _Catalysis[cid][1]=mid;
			  }
			  else if(c1=='C')
			  {
				  //fscanf(fp,"omplex%d", &mid);
				  _Catalysis[cid][0]='C';
				  _Catalysis[cid][1]=mid;
			  }

			  //fscanf(fp," %c", &c2);				  
			  if(c2=='B')
			  {
				  //fscanf(fp,"iochemical Reaction%d", &bid);
				  _Catalysis[cid][2]='R';
				  _Catalysis[cid][3]=bid;
			  }
			  else if(c2=='D')
			  {
				  //fscanf(fp,"egradation%d", &bid);
				  _Catalysis[cid][2]='D';
				  _Catalysis[cid][3]=bid;
			  }

			  //fscanf(fp," %s", ch);
			  if(buffer[3][0]=='A')
				  _Catalysis[cid][4]=1;
			  else _Catalysis[cid][4]=0;
		  }
		  else if(C=='o')
		  {		  
			  //fscanf(fp,"ntrol%d", &cid);	
			  //fscanf(fp," %c", &c1);	
			  if(c1=='P')
			  {
				  //fscanf(fp,"rotein%d", &mid);
				  _5Control[cid][0]='P';
				  _5Control[cid][1]=mid;
			  }
			  else if(c1=='C')
			  {
				  //fscanf(fp,"omplex%d", &mid);
				  _5Control[cid][0]='C';
				  _5Control[cid][1]=mid;
			  }
			  //fscanf(fp," %c", &c2);				  
			  if(c2=='B')
			  {
				  //fscanf(fp,"iochemical Reaction%d", &bid);
				  _5Control[cid][2]='R';
				  _5Control[cid][3]=bid;
			  }
			  else if(c2=='D')
			  {
				  //fscanf(fp,"egradation%d", &bid);
				  _5Control[cid][2]='D';
				  _5Control[cid][3]=bid;
			  }
			  //fscanf(fp," %s", ch);
			  if(buffer[3][0]=='A')
				  _5Control[cid][4]=1;
			  else _5Control[cid][4]=0;			  
		  }
		  //fscanf(fp,"\n");
		  if(bid2==bid&&cid2==cid&&mid2==mid&&C2==C)
		 	  break;

		  cid2=cid, mid2=mid, bid2=bid;	 C2=C;
	 }while(true);	
	 fclose(fp);	 
}

int PathBubble1::read6complex(const char *name, vector<vector<QString>> &_complexName, vector<vector<int>> &_complexContain)
{
	char ch[100];
	char c,o,c2,o2;
	char e;	
	string cname;
	FILE *fp = fopen(name,"r"); 
	if(!fp)
		return 0;
	
	int cid, pid, pid2, cid2;
	int max=-100000;

	pid2=-1, cid2=-1;
	c2='x',o2='x';
	do
	{
		  vector<string> buffer;
	      string phrase; 
		  e='x';
		  do
		  {  e=fgetc(fp);			 
			 if(e=='\t'||e=='\n')
			 {
				 buffer.push_back(phrase);
				 phrase.clear();
			 }
			 else phrase.append(1,e);
		  }while(e!='\n'&&e>-1); 

		  if(e<0||(e=='\n'&&buffer[0].empty()))
			  break;

		  //process phrase
		  //check pid2 
		  for(int i=0; i<buffer.size(); i++)
		  {
			  string result;
		      for(unsigned int j = 0; j < buffer[i].size();j++)
              if(isdigit(buffer[i][j]))
              {   
				  result.append(1,buffer[i][j]);                 
			  }	
			  if(i==0)	 cid= atoi(result.c_str());  
			  else if(i==4) pid= atoi(result.c_str());
		  }     		 
		  c=buffer[3][0];
		  o=buffer[3][1];

		  			
		 if(cid>max)
			    max=cid; 
	        
	 }while(true);
	 fclose(fp);	

	QString a;
	vector<QString> empty3(1,a);
	vector<vector<QString>> temps(max+1,empty3);
	_complexName = temps;

	vector<int> empty;
	vector<vector<int>> temp(max+1,empty);
	_complexContain = temp;	
		
	fp = fopen(name,"r"); 
	
	pid2=-1, cid2=-1;
	c2='x',o2='x';
	do
	{
		 //fscanf(fp,"Complex%d",&cid);		  

		  //fgets(ch, 72, fp); 

		  vector<string> buffer;
	      string phrase; 
		  e='x';
		  do
		  {  e=fgetc(fp);			 
			 if(e=='\t'||e=='\n')
			 {
				 buffer.push_back(phrase);
				 phrase.clear();
			 }
			 else phrase.append(1,e);
		  }while(e!='\n'&&e>-1); 

		  if(e<0||(e=='\n'&&buffer[0].empty()))
			  break;

		  //process phrase
		  //check pid2 
		  for(int i=0; i<buffer.size(); i++)
		  {
			  string result;
		      for(unsigned int j = 0; j < buffer[i].size();j++)
              if(isdigit(buffer[i][j]))
              {   
				  result.append(1,buffer[i][j]);                 
			  }	
			  if(i==0)	 cid= atoi(result.c_str());  
			  else if(i==4) pid= atoi(result.c_str());
		  }     
		  //fscanf(fp," %c",&c1);	
		  //C=buffer[0][1];
		  //cid=buffer[3][0];
		  c=buffer[4][0];
		  o=buffer[4][1];

 		  _complexName[cid][0]= buffer[1].c_str();
          //?complexLocation 
		  //fgets(ch, 30, fp); 

			cname=buffer[2].c_str();

			for(int i=0;i<_scene->_compartmentName[m_pathwayID].size();i++)
			{
			   if(_scene->_compartmentName[m_pathwayID][i][0]==cname.c_str())
			   {
				   vector<int> item;
				   item.push_back('C'); item.push_back(cid);
			      _scene->CompartmentContain[m_pathwayID][i].insert(item);		
				   break;
			   }			
			}
			
			if(c=='C')
		    {
				 _complexContain[cid].push_back('C');
				 _complexContain[cid].push_back(pid);						  
		    }		   
		    else if(c=='P')
		    {
			    if(o=='h')
			    {
				  _complexContain[cid].push_back('E');
				  _complexContain[cid].push_back(pid);		
		        }
			    else if(o=='r')
			    {
				   _complexContain[cid].push_back('P');
				   _complexContain[cid].push_back(pid);				
			    }			  
		    }		 		    
	 }while(true);

	 fclose(fp);	
	 return max;
}


void PathBubble1::processComplexContains(vector<vector<int>> &_complexContain)
{//replace the complex with proteins contained according to _complexContain
	//there are cases that you can not find proteins contained by complex
	int size;
	for(int i=0; i<_scene->_complexContain[m_pathwayID].size(); i++)
	{

		vector<int> set=_scene->_complexContain[m_pathwayID][i];		
		for(int k=0; k<set.size();)
	    {
		             int type=set[k], id=set[k+1];
					 vector<QString> name;	 	                     
			         switch(type)
					 {
						 case 'C':  size=_scene->_complexContain[m_pathwayID][id].size(); 
							        if(size==0)
									{
									   k=k+2;
									}
									else
									{									
									   set[k]  = _scene->_complexContain[m_pathwayID][id][0];
									   set[k+1]= _scene->_complexContain[m_pathwayID][id][1];	
									   for(int l=2; l<size;l=l+2)
									   {
									       set.push_back(_scene->_complexContain[m_pathwayID][id][l]);									   
										   set.push_back(_scene->_complexContain[m_pathwayID][id][l+1]);									   
									   }
									}
							        break;													
						 case 'E': k=k+2;  break; 
						 case 'P': k=k+2;   break;						 
					 }						 
		}
		_scene->_complexContain[m_pathwayID][i]=set;
	}
		 
}



void PathBubble1::getNameforConverted(vector<vector<int>> _8convertedProtein, vector<vector<int>> _8convertedComplex, vector<vector<int>> _8convertedEntity, vector<vector<QString>> &_complexName, vector<vector<QString>> &_proteinName, vector<vector<QString>> &_physicalEntityName)
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
				
				if(!temp.isEmpty()&&!_complexName[id][0].isEmpty())
				         temp.append(at);
				
				temp.append(_complexName[id][0]); 
				if(temp.size()==0)
				{
					while(!_8convertedComplex[id].empty())
					{						
						type=_8convertedComplex[id][0], id=_8convertedComplex[id][1];
						if(type=='C')
						{
							if(!temp.isEmpty()&&!_complexName[id][0].isEmpty())
				                  temp.append(at);
						   temp.append(_complexName[id][0]); 
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
			_complexName[i][0].append(temp); 
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

void PathBubble1::deleteSelectedItems(QGraphicsScene *scene)
{	
	/*while(size>0)
	{	
		int psize=_pathBubbles.last()->getSubPathwaySize();			
	    size--;
	}*/
	if (!scene)
		return;

	if(_scene->delectionStarter == this)
		return;

	int size=_pathBubbles.size(),psize=-1;	
	
	//_pieMenuState=0;
	_penisOn=false;	
	_grabisOn=false;	
	_queryisOn=false;
	_upisOn=_downisOn=_evenisOn=false;
	_lines.clear();		

	//deleteSelectedItemKids(scene); //keqin comment out
	size=_pathBubbles.size(); //replace its child's parent with its parent
	for(int i=0; i<size; i++)
	{	
		if(_pathBubbles[i])
		{
			if(_pathBubbles[i]->isVisible())
			{
				_pathBubbles[i]->_pathBubbleParent = NULL;
		        _pathBubbles[i]->_parentPathBubbleID = -1;
				 _scene->_quantityName[_pathBubbles[i]->m_pathwayID] = _scene->_quantityName[m_pathwayID];
						_scene->_quantity[_pathBubbles[i]->m_pathwayID] = _scene->_quantity[m_pathwayID]; 
						_scene->_minmax[_pathBubbles[i]->m_pathwayID] = _scene->_minmax[m_pathwayID]; 
				if(_pathBubbleParent && _pathBubbleParent->isVisible())
		        {
					_pathBubbles[i]->_pathBubbleParent = _pathBubbleParent;
					_pathBubbleParent->_pathBubbles.append(_pathBubbles[i]);
					_pathBubbleParent->_groupPoint.append(this->_groupPoint);
					_pathBubbleParent->_grouphighlight.append(this->_grouphighlight);
				    _pathBubbleParent->_groupEdgeAffected.append(this->_groupEdgeAffected);
					_pathBubbles[i]->_parentPathBubbleID = _pathBubbleParent->m_pathwayID;				
				}					
			}
		}
		//_scene->bubbleDeleted(_pathBubbles.last());			    
	}
		
	PathBubbleDelectionUpdate(m_pathwayID);
    logRemove();	


	if(_pathBubbleParent!=NULL && _pathBubbleParent->m_pathwayID>=0)
	{
		for (int i = 0; i < _pathBubbleParent->_pathBubbles.size(); i ++)
		{
            subPathBubble1* tmp= dynamic_cast<subPathBubble1*>(this);

			if(_pathBubbleParent->_pathBubbles[i]==tmp)
            {	
				tmp->setVisible(false);
				int id=_pathBubbleParent->_pathBubbles[i]->m_pathwayID;
				_pathBubbleParent->_pathBubbles.removeOne(tmp);
				_pathBubbleParent->_grouphighlight.removeOne(_pathBubbleParent->_grouphighlight[i]);
				_pathBubbleParent->_groupEdgeAffected.removeOne(_pathBubbleParent->_groupEdgeAffected[i]);
		        _pathBubbleParent->_groupPoint.removeOne(_pathBubbleParent->_groupPoint[i]);
				break;
			}
		}
		//_pathBubbleParent->ResetGraph_1();		//comment out for the case deleting a bubble do not delete its offrings
		_pathBubbleParent->getBoundingBoxToBePaint();
		
	}	
	_pathBubbleParent=NULL;
	//_scene->_pathBubbles[m_pathwayID]=NULL;
	ItemBase::deleteSelectedItems(scene);		
}


QRectF PathBubble1::ResetGraph_1()
{   
	//use to recover pathway after one of its subgraph pathway is deleted
	//_scene->resetRanks(m_pathwayID, Rank0, Ranks, _newPos, _newCompartmentPos);		
	//HierarchicalTreeNodeLocation(_whole, _VertexInfo, Rank0, Ranks);
	recordPathBubbleConnections();
	QRectF result;
	if(toplevel)
	{
		result=updateBubbleSize_2();
	}
	else
	{
	   subPathBubble1 * sbubble = dynamic_cast< subPathBubble1 *> (this);
	   result=sbubble->updateBubbleSize_2();
	}
	if(fixedSize)
	{
			result=this->realRectF();
			result.moveCenter(this->sceneBoundingRect().center());
	}

	resetOriRect();
	_scene->backupItemPos(m_pathwayID, complexPos, proteinPos, smallMoleculePos, DnaPos, EmptyPos, reactionPos, physicalEntityPos, compartmentPos, ANodePos, _scale, dCenter, drawSmallmolecule);
	getGraphToBePaint();
	//itemGrid=recordItemGrid(visibleItemNum, visibleCompartmentNum);
	
	//QPointF center=this->sceneBoundingRect().center();
	//QRectF oriRect = QRectF((float)center.x()-(float)this->realRect().width()/2, (float)center.y()-(float)this->realRect().height()/2, (float)this->realRect().width(), (float)this->realRect().height());
			
	//emit itemResized( this, oriRect);
	return result;
}

QRectF PathBubble1::ResetGraph_2()
{   
	//use to recover pathway after one of its subgraph pathway is deleted
	//_scene->resetRanks(m_pathwayID, Rank0, Ranks, _newPos, _newCompartmentPos);		
	recordPathBubbleConnections();
	QRectF result;
	
	if(fixedSize)
	{
		result=this->realRectF();
		result.moveCenter(this->sceneBoundingRect().center());
	}
	resetOriRect();
	_scene->backupItemPos(m_pathwayID, complexPos, proteinPos, smallMoleculePos, DnaPos, EmptyPos, reactionPos, physicalEntityPos, compartmentPos, ANodePos, _scale, dCenter, drawSmallmolecule);
	getGraphToBePaint();
	//itemGrid=recordItemGrid(visibleItemNum, visibleCompartmentNum);
	
	//QPointF center=this->sceneBoundingRect().center();
	//QRectF oriRect = QRectF((float)center.x()-(float)this->realRect().width()/2, (float)center.y()-(float)this->realRect().height()/2, (float)this->realRect().width(), (float)this->realRect().height());
			
	//emit itemResized( this, oriRect);
	return result;
}





void PathBubble1::deleteSelectedItemKids(QGraphicsScene *scene)
{
	//for (int i = 0; i < _codeBubbles.size(); i ++)
	int size;
	size=_codeBubbles.size();
	while(size>0)
	{	
		if(_codeBubbles.last())
		{
			//_scene->bubbleDeleted(_codeBubbles.last());
			_codeBubbles.last()->deleteSelectedItemsbyParent(scene);
			
		}
		//_codeBubbles[i]->deleteSelectedItems(scene);
		size--;
	}	

	size=_reactionBubbles.size();

	//for (int i = 0; i < _reactionBubbles.size(); i ++)
	while(size>0)
	{	
		if( _reactionBubbles.last())
		{   
		    //_scene->bubbleDeleted(_reactionBubbles.last());
			_reactionBubbles.last()->deleteSelectedItemsbyParent(scene);
		}
	    size--;
	}
	//_reactionBubbles.clear();
	//_reactionPoint.clear();

	size=_groupBubbles.size();	
	while(size>0)
	//for (int i = 0; i < _groupBubbles.size(); i ++)
	{		
		if(_groupBubbles.last())
		{
				
			//_scene->bubbleDeleted(_groupBubbles.last());
			_groupBubbles.last()->deleteSelectedItemsbyParent(scene);	
		}
	    size--;
	}
	size=_pathBubbles.size();	
	while(size>0)
	{	
		if(_pathBubbles.last())
		{
			//_scene->bubbleDeleted(_pathBubbles.last());
			_pathBubbles.last()->deleteSelectedItemsbyParent(scene);	
		}
	    size--;
	}

	_codeBubbles.clear();
	_pathBubbles.clear();
	_groupBubbles.clear();
	_reactionBubbles.clear();

	/*for (int i = 0; i < _pathBubbles.size(); i ++)
	{		
		ItemBase *itemBase = dynamic_cast<ItemBase *>(_pathBubbles[i]);
		itemBase->hide();
		itemBase->properRemove();
		//delete itemBase;
	}*/
	//_groupBubbles.clear();
	//_groupPoint.clear();	
}

void PathBubble1::updateLabel()
{   	
	int tindex;	
	QString temp;	
	if(!_labelInitiated)
	{
		tindex = _labelName.lastIndexOf("/");
		temp=_labelName;
		temp.remove(tindex+1,_labelName.length());
		if(toplevel)
		   this->_pathName.remove(temp);
	
		if (_LABEL == false)
		{
			_label = new ItemLabel( this, _scene, m_size_width, m_size_height );
			_label->setLabelColor(_colorBoarder);
			this->_scene->addItem(_label);
			_LABEL = true;
			//emit updateLabelState( this );
			return;
		}
		_label->setName(_labelName); //TGFBR1 LBD Mutants in Cancer //TGF-beta receptor signaling activates SMADs //Signaling by TGF-beta Receptor Complex in Cancer //TGF-beta receptor signaling in EMT (epithelial to mesenchymal transition) //Downregulation of TGF-beta receptor signaling//Signaling by TGF-beta Receptor Complex
		//_label->setName("TGF-beta receptor signaling activates SMADs");
		//_label->setPlainText( this->_curFileName );	
		_label->updateLabel( m_size_width, m_size_height );
		//_label->setPos( _label->pos() + QPointF(0, 12) );
		//if (this->_codeParent)
		{
			//_label->setPos( _label->pos() + QPointF(0, 0) );

			/*QTextDocument *text_document;
			text_document->setPlainText(this->_curFileName);
			QTextOption alignment_option(Qt::AlignBottom); 
			text_document->setDefaultTextOption(alignment_option);
			_label->setDocument(text_document);*/
						
			/*QString styledName = "<h2 align=\"center\" font-size=10pt >";
			styledName = styledName + this->_curFileName;
			styledName = styledName +  "</h2><h2 align=\"center\">1</h2>1";
			_label->setHtml(styledName);//this->_curFileName*/
			//_label->setFont(QFont( "times", 10 ));
			//_label->setTextAlignment();
		}		
		_labelInitiated=true;
	}	
	if(!_label->isVisible())    
		_label->setVisible(true);
} 


void PathBubble1::moveEdgeToBePaint(QPointF dis)
{
	//edge will changed accordingly with changed node
	for(int i=0; i<elipseVector.size(); i++)
	{
		//painter->drawEllipse(elipseVector[i],R,R);		
		elipseVector[i]=elipseVector[i]+dis;
	}
	/*for(int i=0; i< arrowVector.size(); i++)
	{
	    arrowVector[i][0]=arrowVector[i][0]+dis;
		arrowVector[i][1]=arrowVector[i][1]+dis;
	}*/
}


void PathBubble1::moveNodeToBePaint(QPointF dis)
{
	//QRectF getRelativeScale(QPointF dCenter1, float scale1, QPointF dCenter2, float scale2);
	//get location difference
	//before rescale
	int ocount=0;
    if(!_whole)
	for(int i=0; i<compartmentPos.size()-1; i++) //size?
	{
		if(compartmentRect[i].width()>0)
		{
			 fontRect[ocount].moveCenter(fontRect[ocount].center() + dis);
			 compartmentRect[i].moveCenter(compartmentRect[i].center() + dis);		     
		}
		ocount++;		     
	}

	for(int i=0; i<complexPos.size(); i++)
	{	
		
		if(complexRect[i].width()>0)
		{
			 fontRect[ocount].moveCenter(fontRect[ocount].center() + dis);
			 complexRect[i].moveCenter(complexRect[i].center() + dis);		     
		}
		ocount++;			
	}
	
    for(int i=0; i<reactionPos.size(); i++)
	{
		if(reactionRect[i].width()>0)
		{
			 fontRect[ocount].moveCenter(fontRect[ocount].center() + dis);
			 reactionRect[i].moveCenter(reactionRect[i].center() + dis);		     
		}
		ocount++;		
	}

	
	for(int i=0; i<smallMoleculePos.size(); i++)
	{
		//if(smallMoleculePos[i].x()>-0.99)
		if(smallMoleculeRect[i].width()>0)
		{
			 fontRect[ocount].moveCenter(fontRect[ocount].center() + dis);
			 smallMoleculeRect[i].moveCenter(smallMoleculeRect[i].center() + dis);		     
		}
		ocount++;				
	}

	for(int i=0; i<DnaPos.size(); i++)
	{
		if(DnaRect[i].width()>0)
		{
			 fontRect[ocount].moveCenter(fontRect[ocount].center() + dis);
			 DnaRect[i].moveCenter(DnaRect[i].center() + dis);		     
		}
		ocount++;			
	}

	
	for(int i=0; i<physicalEntityPos.size(); i++)
	{
		if(physicalEntityRect[i].width()>0)
		{
			 fontRect[ocount].moveCenter(fontRect[ocount].center() + dis);
			 physicalEntityRect[i].moveCenter(physicalEntityRect[i].center() + dis);		     
		}
		ocount++;		    
	}

	
	for(int i=0; i<proteinPos.size(); i++)
	{
		if(proteinRect[i].width()>0)
		{
			 fontRect[ocount].moveCenter(fontRect[ocount].center() + dis);
			 proteinRect[i].moveCenter(proteinRect[i].center() + dis);		     
		}
		ocount++;			
	}

	for(int i=0; i<ANodePos.size() && i<ANodeRect.size(); i++)
	{
		if(ANodeRect[i].width()>0)
		{
			 fontRect[ocount].moveCenter(fontRect[ocount].center() + dis);
			 ANodeRect[i].moveCenter(ANodeRect[i].center() + dis);		     
		}
		ocount++;		    
	}
		
}

void PathBubble1::scaleNodeToBePaint(QPointF dCenter, float scale)
{
	//QRectF getRelativeScale(QPointF dCenter1, float scale1, QPointF dCenter2, float scale2);
	//get location difference
	//before rescale
	int avaSize=0,avaSize1=0;
	int sizeNum=0;
	int width, height;
	if(fixedSize)
	{
		width=graphReferenceSize*scale, height=graphReferenceSize*scale;
	}
	else
	{
		width=this->realRect().width()*scale;
		height=this->realRect().height()*scale;
	}
	float w,h;
	//QPointF dis=QPointF(dx,dy);
	QPointF center;
	QRectF Dis;
	maxFontSize=-1000000;	
	int ocount=0;
    if(!_whole)
	for(int i=0; i<compartmentPos.size()-1; i++) //size?
	{
		if(compartmentRect[i].width()>0)
		{
			//fontRect[ocount]=getNewNodeRect(compartmentPos[i], 'M', dCenter, scale);		
			compartmentRect[i]=getNewNodeRect(compartmentPos[i], 'M', dCenter, scale);
			getCompartmentFont(fontSize[ocount], fontRect[ocount], compartmentRect[i], _scene->_compartmentName[m_pathwayID][i][0], fixedSize, scale);
			//fontSize[ocount]=;
			avaSize = avaSize + fontSize[ocount];			
	        sizeNum++;
		}
		ocount++;		     
	}
	if(sizeNum>0)
	  avaSize1=avaSize/sizeNum;
	
	for(int i=0; i<complexPos.size(); i++)
	{	
		if(complexRect[i].width()>0)
		{
			//center = fontRect[ocount].center();
			complexRect[i]=getNewNodeRect(complexPos[i], 'C', dCenter, scale);			
			//fontSize[ocount]=;	
			//getNodeFont(fontSize[ocount], fontRect[ocount], complexRect[i], _scene->_complexNameD[dataID][i][0], 'C', width, height, fixedSize, scale);			
		}
		ocount++;	
		
	}
	
    for(int i=0; i<reactionPos.size(); i++)
	{
		if(reactionRect[i].width()>0)
		{
			//center = fontRect[ocount].center();
			reactionRect[i]=getNewNodeRect(reactionPos[i], 'R', dCenter, scale, _scene->_reactionNameD[dataID][i][0]);			
			//fontSize[ocount]=;	
			///getNodeFont(fontSize[ocount], fontRect[ocount], reactionRect[i], _scene->_reactionNameD[dataID][i][0], 'R', width, height, fixedSize, scale);
		}
		ocount++;		
	}
	
	for(int i=0; i<smallMoleculePos.size(); i++)
	{
		if(smallMoleculeRect[i].width()>0)
		{
			//center = fontRect[ocount].center();
			smallMoleculeRect[i]=getNewNodeRect(smallMoleculePos[i], 'S', dCenter, scale);			
			//fontSize[ocount]=;	
			getNodeFont(fontSize[ocount], fontRect[ocount], smallMoleculeRect[i], _scene->_smallMoleculeNameD[dataID][i][0], 'S', width, height, fixedSize, scale);
			avaSize = avaSize + fontSize[ocount];
			if(fontSize[ocount]>maxFontSize) 
				maxFontSize=fontSize[ocount];
	        sizeNum++;
		}
		ocount++;				
	}

	for(int i=0; i<DnaPos.size(); i++)
	{
		if(DnaRect[i].width()>0)
		{
			//center = fontRect[ocount].center();
			DnaRect[i]=getNewNodeRect(DnaPos[i], 'D', dCenter, scale);			
			//fontSize[ocount]=;	
			getNodeFont(fontSize[ocount], fontRect[ocount], DnaRect[i], _scene->_DnaNameD[dataID][i][0], 'D', width, height, fixedSize, scale);
			avaSize = avaSize + fontSize[ocount];
			if(fontSize[ocount]>maxFontSize) 
				maxFontSize=fontSize[ocount];
	        sizeNum++;			
		}
		ocount++;			
	}

	for(int i=0; i<physicalEntityPos.size(); i++)
	{
		if(physicalEntityRect[i].width()>0)
		{
			//center = fontRect[ocount].center();
			physicalEntityRect[i]=getNewNodeRect(physicalEntityPos[i], 'E', dCenter, scale);			
			//fontSize[ocount]=;	
			getNodeFont(fontSize[ocount], fontRect[ocount], physicalEntityRect[i], _scene->_physicalEntityNameD[dataID][i][0], 'E', width, height, fixedSize, scale);
			avaSize = avaSize + fontSize[ocount];
			if(fontSize[ocount]>maxFontSize) 
				maxFontSize=fontSize[ocount];
	        sizeNum++;
		}
		ocount++;		    
	}	
	
	for(int i=0; i<proteinPos.size(); i++)
	{
		if(proteinRect[i].width()>0)
		{
			//center = fontRect[ocount].center();
			proteinRect[i]=getNewNodeRect(proteinPos[i], 'P', dCenter, scale);	
			getNodeFont(fontSize[ocount], fontRect[ocount], proteinRect[i], _scene->_proteinNameD[dataID][i][0], 'P', width, height, fixedSize, scale);
			//fontSize[ocount]=;
			avaSize = avaSize + fontSize[ocount];
			if(fontSize[ocount]>maxFontSize) 
				maxFontSize=fontSize[ocount];
	        sizeNum++;
		}
		ocount++;			
	}

	for(int i=0; i<ANodePos.size(); i++)
	{
		if(ANodeRect[i].width()>0)
		{
			//center = fontRect[ocount].center();
			ANodeRect[i]=getNewNodeRect(ANodePos[i], 'L', dCenter, scale);			
			//fontSize[ocount]=;	
			getNodeFont(fontSize[ocount], fontRect[ocount], ANodeRect[i], _scene->_ANodeName[m_pathwayID][i][0], 'L', width, height, fixedSize, scale);
			avaSize = avaSize + fontSize[ocount];
			if(fontSize[ocount]>maxFontSize) 
				maxFontSize=fontSize[ocount];
	        sizeNum++;
		}
		ocount++;		    
	}
		
	if(sizeNum!=0)
	{
		ocount=0;		
		avaSize = avaSize / sizeNum;
		aveFontSize=avaSize;

		if(maxFontSize<0)
			maxFontSize=aveFontSize;
		else if(maxFontSize>aveFontSize+1)
			maxFontSize=aveFontSize+1;
				
		if( maxFontSize<10 && avaSize1 > maxFontSize)
			maxFontSize = 10;

		if(!_whole)
		for(int i=0; i<compartmentPos.size()-1; i++) //size?
		{
			if(i<compartmentPos.size() && compartmentPos[i].x()>-999.99 && _scene->_compartmentName[m_pathwayID][i][0]!="whole")
			{
				fontSize[ocount]=maxFontSize;
			}
			ocount++;
			//nodeRect.push_back(tRect);
		}	    
	}
}

void PathBubble1::scaleEdgeToBePaint(QPointF dCenter1, float scale1, QPointF dCenter2, float scale2)
{
	//edge will changed accordingly with changed node
	//QPointF dis=QPointF(dx,dy);
	for(int i=0; i<elipseVector.size(); i++)
	{
		//elipseVector[i]=elipseVector[i]+dis;
		elipseVector[i] = getNewNodePoint(elipseVector[i], dCenter1, scale1, dCenter2, scale2);
	}
	/*for(int i=0; i< arrowVector.size(); i++)
	{	    
		arrowVector[i][0] = getNewNodePoint(arrowVector[i][0], dCenter1, scale1, dCenter2, scale2);
		arrowVector[i][1] = getNewNodePoint(arrowVector[i][1], dCenter1, scale1, dCenter2, scale2);
	}*/
}


void PathBubble1::getPartUpdateSelectedNodetoBePaintWhenMoved(vector<int> preItemSelected, vector<int> currentItemSelected, QPointF dis)
{	
	//hoveredItem=_scene->hoveredItem;
	if(preItemSelected==currentItemSelected)
		return;
	set<int> ItemChanged;
	for(int i=0; i< preItemSelected.size(); i=i+2)
	{		   
	   int type=preItemSelected[i], id=preItemSelected[i+1];
	    if(id<0)
		   continue;
	   int ID=getNodeID(type, id);	
	   if(ID<0)
		   continue;
	   ItemChanged.insert(ID);
	   fontRect[ID].moveCenter(fontRect[ID].center() + dis);
	   switch(type)
		{
			case 'M': if(compartmentRect[id].width()>0) 
					  {	
						  compartmentRect[id].moveCenter(compartmentRect[id].center() + dis);
					  }
	                  break;
			case 'C': if(complexRect[id].width()>0) 
					  {
						  complexRect[id].moveCenter(complexRect[id].center() + dis);
					  }
				      break; 
			case 'R': if(reactionRect[id].width()>0) 
					  {
						  reactionRect[id].moveCenter(reactionRect[id].center() + dis); 
					  }
				      break;
			case 'S': if(smallMoleculeRect[id].width()>0) 
					  {
						  smallMoleculeRect[id].moveCenter(smallMoleculeRect[id].center() + dis);
					  }
					  break;
			case 'D': if(DnaRect[id].width()>0) 
					  {
						  DnaRect[id].moveCenter(DnaRect[id].center() + dis);
					  }
				      break;	
			case 'E': if(physicalEntityRect[id].width()>0) 
					  {	 
						  physicalEntityRect[id].moveCenter(physicalEntityRect[id].center() + dis);
					  }
				      break;
			case 'L': if(ANodeRect[id].width()>0) 
					  {	 
						  ANodeRect[id].moveCenter(ANodeRect[id].center() + dis);
					  }
				      break;
			case 'P': if(proteinRect[id].width()>0) 
					  {	
						  proteinRect[id].moveCenter(proteinRect[id].center() + dis);
					  }
				      break;
		}
	}
	for(int i=0; i<currentItemSelected.size(); i=i+2)
	{		   
	   int type=currentItemSelected[i], id=currentItemSelected[i+1];
	    if(id<0)
		   continue;
	   int ID=getNodeID(type, id);	  
	   if(ID<0)
		   continue;
	   if(ItemChanged.find(ID)==ItemChanged.end())
	   {
		   fontRect[ID].moveCenter(fontRect[ID].center() + dis);
		   switch(type)
			{
				case 'M': if(compartmentRect[id].width()>0) 
						  {	
							  compartmentRect[id].moveCenter(compartmentRect[id].center() + dis);
						  }
						  break;
				case 'C': if(complexRect[id].width()>0) 
						  {
							  complexRect[id].moveCenter(complexRect[id].center() + dis);
						  }
						  break; 
				case 'R': if(reactionRect[id].width()>0) 
						  {
							  reactionRect[id].moveCenter(reactionRect[id].center() + dis);
						  }
						  break;
				case 'S': if(smallMoleculeRect[id].width()>0) 
						  {
							  smallMoleculeRect[id].moveCenter(smallMoleculeRect[id].center() + dis);
						  }
						  break;
				case 'D': if(DnaRect[id].width()>0) 
						  {
							  DnaRect[id].moveCenter(DnaRect[id].center() + dis);
						  }
						  break;	
				case 'E': if(physicalEntityRect[id].width()>0) 
						  {	   
							  physicalEntityRect[id].moveCenter(physicalEntityRect[id].center() + dis);
						  }
						  break;
				case 'L': if(ANodeRect[id].width()>0)
						  {	   
							  ANodeRect[id].moveCenter(ANodeRect[id].center() + dis);
						  }
						  break;
				case 'P': if(proteinRect[id].width()>0) 
						  {	
							   proteinRect[id].moveCenter(proteinRect[id].center() + dis);
						  }
						  break;
			 }
		}
	}
	getEdgeToBePaint();
	recordItemGrid();
}



void PathBubble1::getPartUpdateSelectedNodetoBePaint(vector<int> preItemSelected, vector<int> currentItemSelected)
{	
	//hoveredItem=_scene->hoveredItem;
	if(preItemSelected==currentItemSelected)
		return;
	set<int> ItemChanged;
	for(int i=0; i< preItemSelected.size(); i=i+2)
	{		   
	   int type=preItemSelected[i], id=preItemSelected[i+1];
	   if(id<0)
		   continue;
	   int ID=getNodeID(type, id);	   
	   if(ID<0)
		   continue;
	   ItemChanged.insert(ID);
	   switch(type)
	   {
			case 'M': if(compartmentRect[id].width()>0) 
					  {
						  compartmentRect[id] = getCompartmentToBePaint(boundaryWidth[ID], fontSize[ID], insideColor[ID], boundaryColor[ID], fontColor[ID], fontRect[ID], compartmentPos, _scene->_compartmentName[m_pathwayID][id][0], id, _scene->getGroup(this)->sharedSets, _scene->getGroup(this)->expressedSets, dCenter, _scale);
						  fontSize[ID]=maxFontSize;
					  }
	                  break;
			case 'C': if(complexRect[id].width()>0) 
					      complexRect[id] = getNodetoBePaint(boundaryWidth[ID], fontSize[ID], insideColor[ID], boundaryColor[ID], fontColor[ID], fontRect[ID], complexPos[id], _scene->_complexNameD[dataID][id][0], 'C', id, _scene->getGroup(this)->sharedSets, _scene->getGroup(this)->expressedSets,dCenter, _scale);	        
				      break; 
			case 'R': if(reactionRect[id].width()>0) 
						reactionRect[id] = getNodetoBePaint( boundaryWidth[ID], fontSize[ID], insideColor[ID], boundaryColor[ID], fontColor[ID], fontRect[ID], reactionPos[id], _scene->_reactionNameD[dataID][id][1], 'R', id, _scene->getGroup(this)->sharedSets, _scene->getGroup(this)->expressedSets,dCenter, _scale, _scene->_reactionNameD[dataID][id][0]);			
				      break;
			case 'S': if(smallMoleculeRect[id].width()>0) 
						  smallMoleculeRect[id] = getNodetoBePaint( boundaryWidth[ID], fontSize[ID], insideColor[ID], boundaryColor[ID], fontColor[ID], fontRect[ID], smallMoleculePos[id], _scene->_smallMoleculeNameD[dataID][id][0], 'S', id, _scene->getGroup(this)->sharedSets, _scene->getGroup(this)->expressedSets,dCenter, _scale);			
					  break;
			case 'D': if(DnaRect[id].width()>0) 
						  DnaRect[id] = getNodetoBePaint(boundaryWidth[ID], fontSize[ID], insideColor[ID], boundaryColor[ID], fontColor[ID], fontRect[ID], DnaPos[id], _scene->_DnaNameD[dataID][id][0], 'D', id, _scene->getGroup(this)->sharedSets, _scene->getGroup(this)->expressedSets,dCenter, _scale);							
				      break;	
			case 'E': if(physicalEntityRect[id].width()>0) 
						  physicalEntityRect[id] = getNodetoBePaint( boundaryWidth[ID], fontSize[ID], insideColor[ID], boundaryColor[ID], fontColor[ID], fontRect[ID], physicalEntityPos[id], _scene->_physicalEntityNameD[dataID][id][0], 'E', id, _scene->getGroup(this)->sharedSets, _scene->getGroup(this)->expressedSets, dCenter, _scale);		  
				      break;
			case 'L': if(ANodeRect[id].width()>0) 
						  ANodeRect[id] = getNodetoBePaint( boundaryWidth[ID], fontSize[ID], insideColor[ID], boundaryColor[ID], fontColor[ID], fontRect[ID], ANodePos[id], _scene->_ANodeName[m_pathwayID][id][0], 'L', id, _scene->getGroup(this)->sharedSets, _scene->getGroup(this)->expressedSets, dCenter, _scale);		  
				      break;
			case 'P': if(proteinRect[id].width()>0) 
						  proteinRect[id] = getNodetoBePaint(boundaryWidth[ID], fontSize[ID], insideColor[ID], boundaryColor[ID], fontColor[ID], fontRect[ID], proteinPos[id], _scene->_proteinNameD[dataID][id][0], 'P', id, _scene->getGroup(this)->sharedSets, _scene->getGroup(this)->expressedSets,dCenter, _scale);	 		  
				      break;
		}
	}
	for(int i=0; i<currentItemSelected.size(); i=i+2)
	{		   
	   int type=currentItemSelected[i], id=currentItemSelected[i+1];
	    if(id<0)
		   continue;
	   int ID=getNodeID(type, id);
	   if(ID<0)
		   continue;
	   if(ItemChanged.find(ID)==ItemChanged.end())
	   switch(type)
		{
			case 'M': if(compartmentRect[id].width()>0) 
					  {   
						compartmentRect[id] = getCompartmentToBePaint(boundaryWidth[ID], fontSize[ID], insideColor[ID], boundaryColor[ID], fontColor[ID], fontRect[ID], compartmentPos, _scene->_compartmentName[m_pathwayID][id][0], id, _scene->getGroup(this)->sharedSets, _scene->getGroup(this)->expressedSets,dCenter, _scale);
						fontSize[ID]=maxFontSize;
					  }
	                  break;
			case 'C': if(complexRect[id].width()>0) 
						  complexRect[id] = getNodetoBePaint(boundaryWidth[ID], fontSize[ID], insideColor[ID], boundaryColor[ID], fontColor[ID], fontRect[ID], complexPos[id], _scene->_complexNameD[dataID][id][0], 'C', id, _scene->getGroup(this)->sharedSets, _scene->getGroup(this)->expressedSets,dCenter, _scale);	        
				      break; 
			case 'R': if(reactionRect[id].width()>0) 
						  reactionRect[id] =  getNodetoBePaint( boundaryWidth[ID], fontSize[ID], insideColor[ID], boundaryColor[ID], fontColor[ID], fontRect[ID], reactionPos[id], _scene->_reactionNameD[dataID][id][1], 'R', id, _scene->getGroup(this)->sharedSets, _scene->getGroup(this)->expressedSets,dCenter, _scale, _scene->_reactionNameD[dataID][id][0]);			
				      break;
			case 'S': //if(smallMoleculeRect[id].width()>0) 
						  smallMoleculeRect[id]=getNodetoBePaint( boundaryWidth[ID], fontSize[ID], insideColor[ID], boundaryColor[ID], fontColor[ID], fontRect[ID], smallMoleculePos[id], _scene->_smallMoleculeNameD[dataID][id][0], 'S', id, _scene->getGroup(this)->sharedSets, _scene->getGroup(this)->expressedSets,dCenter, _scale);			
					  break;
			case 'D': if(DnaRect[id].width()>0)
						  DnaRect[id] = getNodetoBePaint(boundaryWidth[ID], fontSize[ID], insideColor[ID], boundaryColor[ID], fontColor[ID], fontRect[ID], DnaPos[id], _scene->_DnaNameD[dataID][id][0], 'D', id, _scene->getGroup(this)->sharedSets, _scene->getGroup(this)->expressedSets,dCenter, _scale);							
				      break;	
			case 'E': if(physicalEntityRect[id].width()>0) 
						  physicalEntityRect[id]=getNodetoBePaint( boundaryWidth[ID], fontSize[ID], insideColor[ID], boundaryColor[ID], fontColor[ID], fontRect[ID], physicalEntityPos[id], _scene->_physicalEntityNameD[dataID][id][0], 'E', id, _scene->getGroup(this)->sharedSets,_scene->getGroup(this)->expressedSets, dCenter, _scale);		  
				      break;
			case 'L': if(ANodeRect[id].width()>0) 
						  ANodeRect[id]=getNodetoBePaint( boundaryWidth[ID], fontSize[ID], insideColor[ID], boundaryColor[ID], fontColor[ID], fontRect[ID], ANodePos[id], _scene->_ANodeName[m_pathwayID][id][0], 'L', id, _scene->getGroup(this)->sharedSets,_scene->getGroup(this)->expressedSets, dCenter, _scale);		  
				      break;
			case 'P': if(proteinRect[id].width()>0) 
						  proteinRect[id] = getNodetoBePaint(boundaryWidth[ID], fontSize[ID], insideColor[ID], boundaryColor[ID], fontColor[ID], fontRect[ID], proteinPos[id], _scene->_proteinNameD[dataID][id][0], 'P', id, _scene->getGroup(this)->sharedSets, _scene->getGroup(this)->expressedSets,dCenter, _scale);	 		  
				      break;
		}
	}
	getEdgeToBePaint();
}

void PathBubble1::getPartUpdateHoveredNodetoBePaint(vector<int> preItemSelected, vector<int> currentItemSelected)
{	
	hoveredItem=_scene->hoveredItem;
	if(preItemSelected==currentItemSelected)
		return;
	for(int i=0; i< preItemSelected.size(); i=i+3)
	{		   
	   int pid=preItemSelected[i], type=preItemSelected[i+1], id=preItemSelected[i+2];
	    if(id<0)
		   continue;
	   if(pid==m_pathwayID)
	   {
		   int ID=getNodeID(type, id);
		   if(ID<0)
		   continue;
		   switch(type)
			{
				case 'M': compartmentRect[id] = getCompartmentToBePaint(boundaryWidth[ID], fontSize[ID], insideColor[ID], boundaryColor[ID], fontColor[ID], fontRect[ID], compartmentPos, _scene->_compartmentName[m_pathwayID][id][0], id, _scene->getGroup(this)->sharedSets, _scene->getGroup(this)->expressedSets, dCenter, _scale);
						  fontSize[ID]=maxFontSize;
						  break;
				case 'C': complexRect[id] = getNodetoBePaint(boundaryWidth[ID], fontSize[ID], insideColor[ID], boundaryColor[ID], fontColor[ID], fontRect[ID], complexPos[id], _scene->_complexNameD[dataID][id][0], 'C', id, _scene->getGroup(this)->sharedSets, _scene->getGroup(this)->expressedSets, dCenter, _scale);	        
						  break; 
				case 'R': reactionRect[id] = getNodetoBePaint( boundaryWidth[ID], fontSize[ID], insideColor[ID], boundaryColor[ID], fontColor[ID], fontRect[ID], reactionPos[id], _scene->_reactionNameD[dataID][id][1], 'R', id, _scene->getGroup(this)->sharedSets, _scene->getGroup(this)->expressedSets, dCenter, _scale, _scene->_reactionNameD[dataID][id][0]);			
						  break;
				case 'S': smallMoleculeRect[id]=getNodetoBePaint( boundaryWidth[ID], fontSize[ID], insideColor[ID], boundaryColor[ID], fontColor[ID], fontRect[ID], smallMoleculePos[id], _scene->_smallMoleculeNameD[dataID][id][0], 'S', id, _scene->getGroup(this)->sharedSets, _scene->getGroup(this)->expressedSets, dCenter, _scale);			
						  break;
				case 'D': DnaRect[id] = getNodetoBePaint(boundaryWidth[ID], fontSize[ID], insideColor[ID], boundaryColor[ID], fontColor[ID], fontRect[ID], DnaPos[id], _scene->_DnaNameD[dataID][id][0], 'D', id, _scene->getGroup(this)->sharedSets, _scene->getGroup(this)->expressedSets, dCenter, _scale);							
						  break;	
				case 'E': physicalEntityRect[id]=getNodetoBePaint( boundaryWidth[ID], fontSize[ID], insideColor[ID], boundaryColor[ID], fontColor[ID], fontRect[ID], physicalEntityPos[id], _scene->_physicalEntityNameD[dataID][id][0], 'E', id, _scene->getGroup(this)->sharedSets, _scene->getGroup(this)->expressedSets, dCenter, _scale);		  
						  break;
				case 'L': 
					      ANodeRect[id]=getNodetoBePaint( boundaryWidth[ID], fontSize[ID], insideColor[ID], boundaryColor[ID], fontColor[ID], fontRect[ID], ANodePos[id], _scene->_ANodeName[m_pathwayID][id][0], 'L', id, _scene->getGroup(this)->sharedSets, _scene->getGroup(this)->expressedSets, dCenter, _scale);		  
						  break;
				case 'P': proteinRect[id] = getNodetoBePaint(boundaryWidth[ID], fontSize[ID], insideColor[ID], boundaryColor[ID], fontColor[ID], fontRect[ID], proteinPos[id], _scene->_proteinNameD[dataID][id][0], 'P', id, _scene->getGroup(this)->sharedSets, _scene->getGroup(this)->expressedSets, dCenter, _scale);	 		  
						  break;
			}
	   }
	}
	for(int i=0; i<currentItemSelected.size(); i=i+3)
	{		   
	   int pid = currentItemSelected[i], type=currentItemSelected[i+1], id=currentItemSelected[i+2];
	    if(id<0)
		   continue;
	   if(pid==m_pathwayID)
	   {		
		    int ID=getNodeID(type, id);
			if(ID<0)
			 continue;
		    switch(type)
			{
				case 'M': compartmentRect[id] = getCompartmentToBePaint(boundaryWidth[ID], fontSize[ID], insideColor[ID], boundaryColor[ID], fontColor[ID], fontRect[ID], compartmentPos, _scene->_compartmentName[m_pathwayID][id][0], id, _scene->getGroup(this)->sharedSets, _scene->getGroup(this)->expressedSets, dCenter, _scale);
						  fontSize[ID]=maxFontSize;
						  break;
				case 'C': complexRect[id] = getNodetoBePaint(boundaryWidth[ID], fontSize[ID], insideColor[ID], boundaryColor[ID], fontColor[ID], fontRect[ID], complexPos[id], _scene->_complexNameD[dataID][id][0], 'C', id, _scene->getGroup(this)->sharedSets, _scene->getGroup(this)->expressedSets, dCenter, _scale);	        
						  break; 
				case 'R': reactionRect[id] = getNodetoBePaint( boundaryWidth[ID], fontSize[ID], insideColor[ID], boundaryColor[ID], fontColor[ID], fontRect[ID], reactionPos[id], _scene->_reactionNameD[dataID][id][1], 'R', id, _scene->getGroup(this)->sharedSets, _scene->getGroup(this)->expressedSets, dCenter, _scale, _scene->_reactionNameD[dataID][id][0]);			
						  break;
				case 'S': smallMoleculeRect[id]=getNodetoBePaint( boundaryWidth[ID], fontSize[ID], insideColor[ID], boundaryColor[ID], fontColor[ID], fontRect[ID], smallMoleculePos[id], _scene->_smallMoleculeNameD[dataID][id][0], 'S', id, _scene->getGroup(this)->sharedSets, _scene->getGroup(this)->expressedSets, dCenter, _scale);			
						  break;
				case 'D': DnaRect[id] = getNodetoBePaint(boundaryWidth[ID], fontSize[ID], insideColor[ID], boundaryColor[ID], fontColor[ID], fontRect[ID], DnaPos[id], _scene->_DnaNameD[dataID][id][0], 'D', id, _scene->getGroup(this)->sharedSets, _scene->getGroup(this)->expressedSets, dCenter, _scale);							
						  break;	
				case 'E': physicalEntityRect[id]=getNodetoBePaint( boundaryWidth[ID], fontSize[ID], insideColor[ID], boundaryColor[ID], fontColor[ID], fontRect[ID], physicalEntityPos[id], _scene->_physicalEntityNameD[dataID][id][0], 'E', id, _scene->getGroup(this)->sharedSets, _scene->getGroup(this)->expressedSets, dCenter, _scale);		  
						  break;
				case 'L': ANodeRect[id]=getNodetoBePaint( boundaryWidth[ID], fontSize[ID], insideColor[ID], boundaryColor[ID], fontColor[ID], fontRect[ID], ANodePos[id], _scene->_ANodeName[m_pathwayID][id][0], 'L', id, _scene->getGroup(this)->sharedSets, _scene->getGroup(this)->expressedSets, dCenter, _scale);		  
						  break;
				case 'P': proteinRect[id] = getNodetoBePaint(boundaryWidth[ID], fontSize[ID], insideColor[ID], boundaryColor[ID], fontColor[ID], fontRect[ID], proteinPos[id], _scene->_proteinNameD[dataID][id][0], 'P', id, _scene->getGroup(this)->sharedSets, _scene->getGroup(this)->expressedSets, dCenter, _scale);	 		  
						  break;
			}
	   }
	}
}

int PathBubble1::getNodeID(int type, int id)
{
	switch(type)
	{
	    case 'M':
	        return id;
		case 'C':
			return (compartmentPos.size()-1 + id);
		case 'R':
			return (compartmentPos.size()-1 + complexPos.size() + id);
		case 'S':
			return (compartmentPos.size()-1 + complexPos.size() + reactionPos.size() + id);
		case 'D':
			return (compartmentPos.size()-1 + complexPos.size() + reactionPos.size() + smallMoleculePos.size() + id);	
		case 'E':
			return (compartmentPos.size()-1 + complexPos.size() + reactionPos.size() + smallMoleculePos.size() + DnaPos.size() + id);				
		case 'P':
		    return (compartmentPos.size()-1 + complexPos.size() + reactionPos.size() + smallMoleculePos.size() + DnaPos.size() + physicalEntityPos.size() + id);  
		case 'L':
			return (compartmentPos.size()-1 + complexPos.size() + reactionPos.size() + smallMoleculePos.size() + DnaPos.size() + physicalEntityPos.size() + +proteinPos.size() + id);
	}
	return -1;

}

float PathBubble1::getNodeToBePaint()
{//get reactionRecord before stat the function
//should return the ratio
	//getAverage fontsize for compartment
	float _boundaryWidth; 
	int _fontSize;
	int avaSize=0,avaSize1=0;
	int sizeNum=0, SNum=0; //snum record the number of small molecule
	QRectF _fontRect;
	QColor _boundaryColor, _fontColor;
	vector<QColor> _insideColor;
	hoveredItem=_scene->hoveredItem;
	int ocount=0;
	vector<int> temp(2,0);
	QRectF tRect;
		
	boundaryWidth.clear(), fontSize.clear(), insideColor.clear(), boundaryColor.clear(), fontColor.clear(); fontRect.clear();
	complexRect.clear(), proteinRect.clear(), smallMoleculeRect.clear(), DnaRect.clear(), reactionRect.clear(), physicalEntityRect.clear(), compartmentRect.clear(), degradationRect.clear(), EmptyRect.clear(), ANodeRect.clear();
	
	vector<set<vector<int>>> sharedSets, expressedSets;	
	Group * itsGroup=_scene->getGroup(this);
	if(itsGroup)
	{
	  sharedSets = itsGroup->sharedSets;
	  expressedSets = itsGroup->expressedSets;
	}
	maxFontSize=-1000000;
	if(!_whole)
	{
		for(int i=0; i<compartmentPos.size()-1; i++) //size?
		{
			tRect.setWidth(-1000);
			if(i < compartmentPos.size() && compartmentPos[i].x()>-999.99 && _scene->_compartmentName[m_pathwayID][i][0]!="whole")
			{
				tRect= getCompartmentToBePaint( _boundaryWidth, _fontSize, _insideColor, _boundaryColor, _fontColor, _fontRect, compartmentPos, _scene->_compartmentName[m_pathwayID][i][0], i, sharedSets, expressedSets, dCenter, _scale);
				avaSize = avaSize + _fontSize;
				sizeNum++;
			}
			boundaryWidth.push_back(_boundaryWidth), fontSize.push_back(_fontSize), insideColor.push_back(_insideColor), boundaryColor.push_back(_boundaryColor), fontColor.push_back(_fontColor), fontRect.push_back(_fontRect);
			compartmentRect.push_back(tRect);		
			ocount++;			
		}		
	}
	if(sizeNum>0)
	  avaSize1=avaSize/sizeNum;
	
	temp[0]='C';	
	complexRect.resize(complexPos.size());
	boundaryWidth.resize(boundaryWidth.size()+complexPos.size()); 	
	fontSize.resize(fontSize.size()+complexPos.size()); 	
	insideColor.resize(insideColor.size()+complexPos.size()); 
	boundaryColor.resize(boundaryColor.size()+complexPos.size()); 
	fontColor.resize(fontColor.size()+complexPos.size()); 
	fontRect.resize( fontRect.size()+complexPos.size()); 
	
	for(int i=0; i<complexPos.size(); i++)
	{
		//int id=getVerticeID('C',i);
		//tRect.setWidth(-1000);
		//_boundaryWidth=0, _fontSize=0, _insideColor=_boundaryColor=_fontColor=QColor(0,0,0,0);
		if(complexPos[i].x()>-999.99)
		{
			    temp[1]=i; 			
	
				vector<int> temp2(3,0);
			    temp2[0]=m_pathwayID, temp2[1]=temp[0], temp2[2]=temp[1];

				vector<vector<int>> sEdge=findSpannedEdge(scene->edgeconnections, temp2);
				if(!toplevel || reactionRecord.find(temp)!=reactionRecord.end()||!sEdge.empty())
				{  		
					//tRect=paintNode( painter, complexPos[i], _scene->_complexNameD[dataID][i][0], 'C', i, sharedSets, fixedSize, dCenter, _scale);	        			
					complexRect[i] = getNodetoBePaint(_boundaryWidth, _fontSize, _insideColor, _boundaryColor, _fontColor, _fontRect, complexPos[i], _scene->_complexNameD[dataID][i][0], 'C', i, sharedSets, expressedSets, dCenter, _scale);	        
			        boundaryWidth[ocount] = _boundaryWidth, fontSize[ocount] = _fontSize, insideColor[ocount] =_insideColor, boundaryColor[ocount] =_boundaryColor, fontColor[ocount] =_fontColor, fontRect[ocount] = _fontRect;

				}	
				else
					complexRect[i].setWidth(-1000);
		}
		else
			complexRect[i].setWidth(-1000);
		ocount++;		
		
		//nodeRect.push_back(tRect);
	}
	
	reactionRect.resize(reactionPos.size());
	boundaryWidth.resize(boundaryWidth.size()+reactionPos.size());
	fontSize.resize(fontSize.size()+reactionPos.size());
	insideColor.resize(insideColor.size()+reactionPos.size());
	boundaryColor.resize(boundaryColor.size()+reactionPos.size());
	fontColor.resize(fontColor.size()+reactionPos.size());
	fontRect.resize( fontRect.size()+reactionPos.size());

	temp[0]='R';
	
	for(int i=0; i<reactionPos.size(); i++)
	{
		if(reactionPos[i].x()>-999.99)
		{
			temp[1]=i; 		
			vector<int> temp2(3,0);
			temp2[0]=m_pathwayID, temp2[1]=temp[0], temp2[2]=temp[1];
			vector<vector<int>> sEdge=findSpannedEdge(scene->edgeconnections, temp2);	
			if(!toplevel || reactionRecord.find(temp)!=reactionRecord.end()||!sEdge.empty())
			{
				//tRect=paintNode( painter, reactionPos[i], _scene->_reactionNameD[dataID][i][1], 'R', i, sharedSets, fixedSize, dCenter, _scale, _scene->_reactionNameD[dataID][i][0]);			
				reactionRect[i] =getNodetoBePaint( _boundaryWidth, _fontSize, _insideColor, _boundaryColor, _fontColor, _fontRect, reactionPos[i], _scene->_reactionNameD[dataID][i][1], 'R', i, sharedSets, expressedSets, dCenter, _scale, _scene->_reactionNameD[dataID][i][0] );			
			    boundaryWidth[ocount] = _boundaryWidth, fontSize[ocount] = _fontSize, insideColor[ocount] =_insideColor, boundaryColor[ocount] =_boundaryColor, fontColor[ocount] =_fontColor, fontRect[ocount] = _fontRect;
			}
			else
				reactionRect[i].setWidth(-1000);
		}
		else
			reactionRect[i].setWidth(-1000);
		ocount++;
		//boundaryWidth.push_back(_boundaryWidth), fontSize.push_back(_fontSize), insideColor.push_back(_insideColor), boundaryColor.push_back(_boundaryColor), fontColor.push_back(_fontColor), fontRect.push_back(_fontRect);
		//reactionRect.push_back(tRect);
		//nodeRect.push_back(tRect);
	}

	smallMoleculeRect.resize(smallMoleculePos.size());
	boundaryWidth.resize(boundaryWidth.size()+smallMoleculePos.size());
	fontSize.resize(fontSize.size()+smallMoleculePos.size());
	insideColor.resize(insideColor.size()+smallMoleculePos.size());
	boundaryColor.resize(boundaryColor.size()+smallMoleculePos.size());
	fontColor.resize(fontColor.size()+smallMoleculePos.size());
	fontRect.resize( fontRect.size()+smallMoleculePos.size());

	temp[0]='S';
	for(int i=0; i<smallMoleculePos.size(); i++)
	{
		if(smallMoleculePos[i].x()>-999.99)
		{
			
			temp[1]=i; 
			vector<int> temp2(3,0);
		    temp2[0]=m_pathwayID, temp2[1]=temp[0], temp2[2]=temp[1];
			vector<vector<int>> sEdge=findSpannedEdge(scene->edgeconnections, temp2);
			if( (!toplevel || reactionRecord.find(temp)!=reactionRecord.end()|| !sEdge.empty()) )	
			{
				 //tRect=paintNode( painter, smallMoleculePos[i], _scene->_smallMoleculeNameD[dataID][i][0], 'S', i, sharedSets, fixedSize, dCenter, _scale);
				 smallMoleculeRect[i]=getNodetoBePaint( _boundaryWidth, _fontSize, _insideColor, _boundaryColor, _fontColor, _fontRect, smallMoleculePos[i], _scene->_smallMoleculeNameD[dataID][i][0], 'S', i, sharedSets, expressedSets, dCenter, _scale);			
				 boundaryWidth[ocount] = _boundaryWidth, fontSize[ocount] = _fontSize, insideColor[ocount] =_insideColor, boundaryColor[ocount] =_boundaryColor, fontColor[ocount] =_fontColor, fontRect[ocount] = _fontRect;
				 avaSize = avaSize + _fontSize;
				 if(fontSize[ocount]>maxFontSize) 
					 maxFontSize=fontSize[ocount];
	             sizeNum++;
				 SNum++;
			}
			else
				 smallMoleculeRect[i].setWidth(-1000);
		}
		else
			 smallMoleculeRect[i].setWidth(-1000);
		ocount++;
		//boundaryWidth.push_back(_boundaryWidth), fontSize.push_back(_fontSize), insideColor.push_back(_insideColor), boundaryColor.push_back(_boundaryColor), fontColor.push_back(_fontColor), fontRect.push_back(_fontRect);
		//smallMoleculeRect.push_back(tRect);
		//nodeRect.push_back(tRect);
	}
	DnaRect.resize(DnaPos.size());
	boundaryWidth.resize(boundaryWidth.size()+DnaPos.size());
	fontSize.resize(fontSize.size()+DnaPos.size());
	insideColor.resize(insideColor.size()+DnaPos.size());
	boundaryColor.resize(boundaryColor.size()+DnaPos.size());
	fontColor.resize(fontColor.size()+DnaPos.size());
	fontRect.resize( fontRect.size()+DnaPos.size());
	temp[0]='D';
	for(int i=0; i<DnaPos.size(); i++)
	{
		
		if(DnaPos[i].x()>-999.99)
		{   
			temp[1]=i; 
            vector<int> temp2(3,0);
            temp2[0]=m_pathwayID, temp2[1]=temp[0], temp2[2]=temp[1];
            vector<vector<int>> sEdge=findSpannedEdge(scene->edgeconnections, temp2);
            if( (!toplevel || reactionRecord.find(temp)!=reactionRecord.end()||!sEdge.empty()))		
			{	
				//tRect=paintNode( painter, DnaPos[i], _scene->_DnaNameD[dataID][i][0], 'S', i, sharedSets, fixedSize, dCenter, _scale);
				DnaRect[i] = getNodetoBePaint(_boundaryWidth, _fontSize, _insideColor, _boundaryColor, _fontColor, _fontRect, DnaPos[i], _scene->_DnaNameD[dataID][i][0], 'D', i, sharedSets, expressedSets, dCenter, _scale);			
				boundaryWidth[ocount] = _boundaryWidth, fontSize[ocount] = _fontSize, insideColor[ocount] =_insideColor, boundaryColor[ocount] =_boundaryColor, fontColor[ocount] =_fontColor, fontRect[ocount] = _fontRect;
				avaSize = avaSize + _fontSize;
				if(fontSize[ocount]>maxFontSize) 
				    maxFontSize=fontSize[ocount];
	            sizeNum++;
			}
			else 
				DnaRect[i].setWidth(-1000);
		}
		else 
			DnaRect[i].setWidth(-1000);
		ocount++;
		//boundaryWidth.push_back(_boundaryWidth), fontSize.push_back(_fontSize), insideColor.push_back(_insideColor), boundaryColor.push_back(_boundaryColor), fontColor.push_back(_fontColor), fontRect.push_back(_fontRect);
		//DnaRect.push_back(tRect);
		//nodeRect.push_back(tRect);
	}

	physicalEntityRect.resize(physicalEntityPos.size());
	boundaryWidth.resize(boundaryWidth.size()+physicalEntityPos.size());
	fontSize.resize(fontSize.size()+physicalEntityPos.size());
	insideColor.resize(insideColor.size()+physicalEntityPos.size());
	boundaryColor.resize(boundaryColor.size()+physicalEntityPos.size());
	fontColor.resize(fontColor.size()+physicalEntityPos.size());
	fontRect.resize( fontRect.size()+physicalEntityPos.size());
	temp[0]='E';
	for(int i=0; i<physicalEntityPos.size(); i++)
	{	
		if(physicalEntityPos[i].x()>-999.99)
	    {
			temp[1]=i; 
			vector<int> temp2(3,0);
			temp2[0]=m_pathwayID, temp2[1]=temp[0], temp2[2]=temp[1];
			vector<vector<int>> sEdge=findSpannedEdge(scene->edgeconnections, temp2);
			if(!toplevel || reactionRecord.find(temp)!=reactionRecord.end()||!sEdge.empty())
			{
				//tRect=paintNode( painter, physicalEntityPos[i], _scene->_physicalEntityNameD[dataID][i][0], 'E', i, sharedSets,fixedSize, dCenter, _scale);
				physicalEntityRect[i]=getNodetoBePaint( _boundaryWidth, _fontSize, _insideColor, _boundaryColor, _fontColor, _fontRect, physicalEntityPos[i], _scene->_physicalEntityNameD[dataID][i][0], 'E', i, sharedSets,expressedSets,  dCenter, _scale);		  
				boundaryWidth[ocount] = _boundaryWidth, fontSize[ocount] = _fontSize, insideColor[ocount] =_insideColor, boundaryColor[ocount] =_boundaryColor, fontColor[ocount] =_fontColor, fontRect[ocount] = _fontRect;
				avaSize = avaSize + _fontSize;
				if(fontSize[ocount]>maxFontSize) 
				    maxFontSize=fontSize[ocount];
	            sizeNum++;
			}
			else physicalEntityRect[i].setWidth(-1000);
		}
		else 
			physicalEntityRect[i].setWidth(-1000);
		ocount++;	   
	}

		
	proteinRect.resize(proteinPos.size());
	boundaryWidth.resize(boundaryWidth.size()+proteinPos.size());
	fontSize.resize(fontSize.size()+proteinPos.size());
	insideColor.resize(insideColor.size()+proteinPos.size());
	boundaryColor.resize(boundaryColor.size()+proteinPos.size());
	fontColor.resize(fontColor.size()+proteinPos.size());
	fontRect.resize( fontRect.size()+proteinPos.size());
	temp[0]='P';
	for(int i=0; i<proteinPos.size(); i++)
	{		
		  if(proteinPos[i].x()>-999.99)
		  {
			  temp[1]=i; 	
			  vector<int> temp2(3,0);
			  temp2[0]=m_pathwayID, temp2[1]=temp[0], temp2[2]=temp[1];			  

			  vector<vector<int>> sEdge=findSpannedEdge(scene->edgeconnections, temp2);
			  if(!toplevel || reactionRecord.find(temp)!=reactionRecord.end()||!sEdge.empty())
			  {
				 // QString s=_scene->_proteinNameD[dataID][i][0];
				  //tRect=paintNode( painter, proteinPos[i], _scene->_proteinNameD[dataID][i][0], 'P', i, sharedSets, fixedSize, dCenter, _scale);	     
				  proteinRect[i] = getNodetoBePaint(_boundaryWidth, _fontSize, _insideColor, _boundaryColor, _fontColor, _fontRect, proteinPos[i], _scene->_proteinNameD[dataID][i][0], 'P', i, sharedSets, expressedSets, dCenter, _scale);	 		  
				  boundaryWidth[ocount] = _boundaryWidth, fontSize[ocount] = _fontSize, insideColor[ocount] =_insideColor, boundaryColor[ocount] =_boundaryColor, fontColor[ocount] =_fontColor, fontRect[ocount] = _fontRect;
				  avaSize = avaSize + _fontSize;
				  if(fontSize[ocount]>maxFontSize) 
				      maxFontSize=fontSize[ocount];
	              sizeNum++;
			  }
			  else
				  proteinRect[i].setWidth(-1000);
		  }
		  else
			  proteinRect[i].setWidth(-1000);
		  ocount++;
		  //proteinRect.push_back(tRect);
		  //boundaryWidth.push_back(_boundaryWidth), fontSize.push_back(_fontSize), insideColor.push_back(_insideColor), boundaryColor.push_back(_boundaryColor), fontColor.push_back(_fontColor), fontRect.push_back(_fontRect);
		  //nodeRect.push_back(tRect);
	}

	ANodeRect.resize(ANodePos.size());
	boundaryWidth.resize(boundaryWidth.size()+ANodePos.size());
	fontSize.resize(fontSize.size()+ANodePos.size());
	insideColor.resize(insideColor.size()+ANodePos.size());
	boundaryColor.resize(boundaryColor.size()+ANodePos.size());
	fontColor.resize(fontColor.size()+ANodePos.size());
	fontRect.resize( fontRect.size()+ANodePos.size());
	temp[0]='L';
	for(int i=0; i<ANodePos.size(); i++)
	{	
		if(ANodePos[i].x()>-999.99)
	    {
			temp[1]=i; 
			vector<int> temp2(3,0);
			temp2[0]=m_pathwayID, temp2[1]=temp[0], temp2[2]=temp[1];
			vector<vector<int>> sEdge=findSpannedEdge(scene->edgeconnections, temp2);
			//if(!toplevel || reactionRecord.find(temp)!=reactionRecord.end()||!sEdge.empty())
			{				
				int size1=_scene->_ANodeName[m_pathwayID].size();				
				if(size1>i)
				{
                    int size2=_scene->_ANodeName[m_pathwayID][i].size();
					if(size2>=1)
					{
						ANodeRect[i]=getNodetoBePaint( _boundaryWidth, _fontSize, _insideColor, _boundaryColor, _fontColor, _fontRect, ANodePos[i], _scene->_ANodeName[m_pathwayID][i][0], 'L', i, sharedSets,expressedSets, dCenter, _scale);		  
						boundaryWidth[ocount] = _boundaryWidth, fontSize[ocount] = _fontSize, insideColor[ocount] =_insideColor, boundaryColor[ocount] =_boundaryColor, fontColor[ocount] =_fontColor, fontRect[ocount] = _fontRect;
						avaSize = avaSize + _fontSize;
						if(fontSize[ocount]>maxFontSize) 
							maxFontSize=fontSize[ocount];
						sizeNum++;
					}
				}
			}
			//else ANodeRect[i].setWidth(-1000);
		}
		else 
			ANodeRect[i].setWidth(-1000);
		ocount++;	   
	}

	if(sizeNum!=0)
	{
		ocount=0;
		
		avaSize = avaSize / sizeNum;
		aveFontSize=avaSize;
		if(maxFontSize<0)
			maxFontSize=aveFontSize;
		else if(maxFontSize>aveFontSize+1)
			maxFontSize=aveFontSize+1;

		if( maxFontSize<10 && avaSize1 > maxFontSize)
			maxFontSize = 10;

		if(!_whole)
		for(int i=0; i<compartmentPos.size()-1; i++) //size?
		{
			
			if(i < compartmentPos.size() && compartmentPos[i].x()>-999.99 && _scene->_compartmentName[m_pathwayID][i][0]!="whole")
			{
				fontSize[ocount]=maxFontSize;				
			}
			ocount++;
			//nodeRect.push_back(tRect);
		}	    
	}
	for(int i=0; i< proteinRect.size(); i++)
	{
		if(proteinRect[i].width()>0)
		{
			referH = proteinRect[i].height();
			break;
		}
	}	
	return sizeNum==0?1:(float)SNum/sizeNum;
}

void PathBubble1::getEdgeToBePaint()
{
    //int size=_pathBubbles.size();
	elipseVector.clear();
	updatedEdge_1.clear();
	updatedEdge_2.clear();
	updatedEdge_3.clear();
	arrowVector.clear();
	//updateLabel();

	QPointF start,middle,end;
	vector<int> temp(2,0),etemp(3,0);
	
	vector<vector<int>> tedge(2,etemp);

	//paintPathWays(painter, _scene->PathWayNames);	
	float width;
	float height;

	if(fixedSize)
	{		
		width=graphReferenceSize/2*_scale; height=graphReferenceSize/2*_scale;
	}
	else
	{
		width=this->realRect().width()/2*_scale;
	    height=this->realRect().height()/2*_scale;
	}

	float dcx=dCenter.x()*this->realRect().width(), dcy=dCenter.y()*this->realRect().height();
	QPointF DC(dcx,dcy);
	
	vector<int> node(2,0);
	vector<vector<int>> ed(2, node);
	
	reactionRecord.clear();	
	//painter->setBrush(Qt::NoBrush);		
	QRectF tRect;
	float x,y,w,h;
	float R=4.0*_scale;

	///updatedEdge_4,5,6,7

	//1 draw compartments
	//compartmentRect.clear();	
	if(m_pathwayID>=0)
	{
		QRectF s,m,e;
		int type, id;	
		
		for(int i=0; i<edge.size(); i++)
		{	
			if(i==319)
			     i=i;
			if(edge[i][2]=='R' && edge[i][4]=='R')
			{
				 bool flags=true, flagm=true, flage=true;
				 bool t1=false,t2=false;
				 type=edge[i][0], id=edge[i][1];	
				 if(id<0)
					 flags=false;

				 if(id>=0)
				 switch(type)
				 {
					 case 'C': s=complexPos[id]; start=complexPos[id].center();     break;
					 case 'D': s=DnaPos[id]; start=DnaPos[id].center();             break;
					 case 'E': s=physicalEntityPos[id]; start=physicalEntityPos[id].center();   break; 
					 case 'L': s=ANodePos[id]; start=ANodePos[id].center();         break; 
					 case 'P': s=proteinPos[id];  start=proteinPos[id].center();    break;
					 case 'S': s=smallMoleculePos[id]; start=smallMoleculePos[id].center();   break;			
				 }
				 type=edge[i][4];
				 id=edge[i][5];

				  if(id<0)
					 flagm=false;

				 if(id>=0)
				 if(type=='R') 
				 {
					 middle=reactionPos[id].center();
					 m=reactionPos[id];			
				 }

				start=QPointF(start.x()*width*2-width, start.y()*height*2-height);
				middle=QPointF(middle.x()*width*2-width, middle.y()*height*2-height);
				
				//if(flags && flagm && s.x()>-990.99 && m.x()>-990.99)
				//{	
					temp[0]=edge[i][4];	temp[1]=edge[i][5]; reactionRecord.insert(temp);//middle
					etemp[0]=m_pathwayID; etemp[1]=temp[0];	etemp[2]=temp[1]; tedge[0]=etemp;
					temp[0]=edge[i][0];	temp[1]=edge[i][1]; reactionRecord.insert(temp);//start	
					etemp[1]=temp[0];	etemp[2]=temp[1]; tedge[1]=etemp;
					//updatedEdge_1.insert(tedge);
				if(flags && flagm && s.x()>-990.99 && m.x()>-990.99)
				{	
				
					float dx,dy,ds;								
					ds=sqrt(pow(start.x()-middle.x(),2)+pow(start.y()-middle.y(),2));
					dx=(start.x()-middle.x())/ds, dy=(start.y()-middle.y())/ds;
					end.setX(middle.x()+dx*R*2), end.setY(middle.y()+dy*R*2);		
					elipseVector.push_back(end+DC);	//change to half arrow for tappered case	

					if(edge[i][3]==0) //inhibition
					   updatedEdge_3.insert(tedge);
					else if(edge[i][3]==1) //activation
					   updatedEdge_2.insert(tedge);
				}				
			}
			else
			{		
				 bool flags=true, flagm=true, flage=true;
				 bool t1=false,t2=false;
				 type=edge[i][0], id=edge[i][1];	
				 if(id<0)
					 flags=false;

				 if(id>=0)
				 switch(type)
				 {
					 case 'C': s=complexPos[id]; start=complexPos[id].center();         break;
					 case 'D': s=DnaPos[id]; start=DnaPos[id].center();             break;
					 case 'E': s=physicalEntityPos[id]; start=physicalEntityPos[id].center();          break; 
					 case 'L': s=ANodePos[id]; start=ANodePos[id].center();         break; 
					 case 'P': s=proteinPos[id];  start=proteinPos[id].center();         break;
					 case 'S': s=smallMoleculePos[id]; start=smallMoleculePos[id].center();   break;			
				 }
				 type=edge[i][4];
				 id=edge[i][5];

				  if(id<0)
					 flagm=false;

				 if(id>=0)
				 if(type=='R') 
				 {
					 middle=reactionPos[id].center();
					 m=reactionPos[id];			
				 }		 		 		 
				 type=edge[i][2], id=edge[i][3];	
		
				 QRectF rect;
				 if(id<0)
					 flage=false;		
				 if(id>=0)
				 switch(type)
				 {
					 case 'C': end=complexPos[id].center();       rect= complexPos[id];    
							   rect.setWidth(rect.height()*height*2.5/width);
							   rect.moveCenter(end); //if(complexPos[id].width()>itv) complexPos[id].setWidth(itv); 	 
							   break;
					 case 'D': end=DnaPos[id].center();           rect= DnaPos[id];          break;
					 case 'E': end=physicalEntityPos[id].center();rect= physicalEntityPos[id];    break; 
					 case 'L': end=ANodePos[id].center(); rect= ANodePos[id];    break; 
					 case 'P': end=proteinPos[id].center();       rect= proteinPos[id];           break;
					 case 'S': end=smallMoleculePos[id].center(); rect= smallMoleculePos[id];     break;			
					 case 'R': end=reactionPos[id].center();      rect= reactionPos[id];  break;  //catalysis for bio reaction	
				 }
				 e=rect;
		
				start=QPointF(start.x()*width*2-width, start.y()*height*2-height);
				middle=QPointF(middle.x()*width*2-width, middle.y()*height*2-height);
				end=QPointF(end.x()*width*2-width, end.y()*height*2-height);

				temp[0]=edge[i][4];	temp[1]=edge[i][5]; reactionRecord.insert(temp);//middle
				etemp[0]=m_pathwayID; etemp[1]=temp[0];	etemp[2]=temp[1]; tedge[0]=etemp;
				temp[0]=edge[i][0];	temp[1]=edge[i][1]; reactionRecord.insert(temp);//start	
				etemp[1]=temp[0];	etemp[2]=temp[1]; tedge[1]=etemp;

				if(flagm && m.x()>-990.99)
				{ 					
					if(flags && s.x()>-990.99)
					{	
						//if( (tedge[0][1]==82 && tedge[0][2]==0) || (tedge[1][1]==82 && tedge[1][2]==0) )
						//	 flage=flage;
						updatedEdge_1.insert(tedge);
					}					
				}
				temp[0]=edge[i][2];	temp[1]=edge[i][3]; reactionRecord.insert(temp);//end
					etemp[0]=m_pathwayID; etemp[1]=temp[0];	etemp[2]=temp[1]; tedge[0]=etemp;
					temp[0]=edge[i][4];	temp[1]=edge[i][5]; reactionRecord.insert(temp);//middle 	
					etemp[1]=temp[0];	etemp[2]=temp[1]; tedge[1]=etemp;
									

				if(flagm && m.x()>-990.99 && tedge[0][1]!=82)
				{ 
					if(flage && e.x()>-990.99)						
					{	
						//if( (tedge[0][1]==82 && tedge[0][2]==0) || (tedge[1][1]==82 && tedge[1][2]==0) )
						//	 flage=flage;
						updatedEdge_1.insert(tedge);		
					}				
				}
			}			   
	   }	
	}
	//edge_4
	//set<vector<vector<int> > > updatedEdge_5;
	for(set<vector<vector<int> > >::iterator it=updatedEdge_1.begin(); it!=updatedEdge_1.end(); it++)
	{
		vector<vector<int> > edge=*it,redge;
		redge.push_back(edge[1]); redge.push_back(edge[0]);
		if( (updatedEdge_4.find(edge)==updatedEdge_4.end()&& updatedEdge_4.find(redge)==updatedEdge_4.end()) && (updatedEdge_2.find(edge)!= updatedEdge_2.end() || updatedEdge_3.find(edge)!= updatedEdge_3.end() || updatedEdge_2.find(redge)!= updatedEdge_2.end() || updatedEdge_3.find(redge)!= updatedEdge_3.end() || updatedEdge_1.find(redge)!= updatedEdge_1.end()))
		{
			//updatedEdge_5.insert(*it);
			updatedEdge_4.insert(edge);
		}	
	}
	updatedEdge_5.clear();
 	for(set<vector<vector<int> > >::iterator it=updatedEdge_4.begin(); it!=updatedEdge_4.end(); it++)
	{
		vector<vector<int> > edge=*it,redge;
		redge.push_back(edge[1]); redge.push_back(edge[0]);
	    int encode=0;
		if( updatedEdge_1.find(edge)!= updatedEdge_1.end() )
		{
		    encode|=1;
		}
		if( updatedEdge_1.find(redge)!= updatedEdge_1.end() )
		{
		    encode|=2;
		}
		if( updatedEdge_2.find(edge)!= updatedEdge_2.end() )
		{
		    encode|=4; 
		}
		if( updatedEdge_2.find(redge)!= updatedEdge_2.end() )
		{
		    encode|=8;
		}
		if( updatedEdge_3.find(edge)!= updatedEdge_3.end() ) 
		{
		    encode|=16; 		
		}	
		if( updatedEdge_3.find(redge)!= updatedEdge_3.end() )
		{
		    encode|=32; 
		}
		updatedEdge_5.push_back(encode);
	}
	/*
	for(set<vector<vector<int> > >::iterator it=updatedEdge_2.begin(); it!=updatedEdge_2.end(); it++)
	{
	
	
	}

	for(set<vector<vector<int> > >::iterator it=updatedEdge_3.begin(); it!=updatedEdge_3.end(); it++)
	{
	
	
	}*/
}

bool PathBubble1::inUpdatedEdge(vector<vector<int> > edge, set<vector<vector<int>>> updatedEdge)
{
	vector<vector<int> > redge;
	redge.push_back(edge[1]); redge.push_back(edge[0]);
	if( updatedEdge.find(edge)!= updatedEdge.end() || updatedEdge.find(redge)!= updatedEdge.end())
	{
	    return true;
	}
	return false;
}

void PathBubble1::getBoundingBoxToBePaint()
{  
	int width, height;
	if(fixedSize)
	{
		width=graphReferenceSize*_scale, height=graphReferenceSize*_scale;
	}
	else
	{
		width=this->realRect().width()*_scale;
		height=this->realRect().height()*_scale;
	}
	QRectF fontRect;
	boundingBoxes.clear();
	boundingFontSize.clear();	
	boundingText.clear();
	int size=_pathBubbles.size(); //replace its child's parent with its parent
	int fontSize, fontSizeM;
	QString text, textM;
	QRectF rect, RectM, bRect;	
	for(int i=0; i<size; i++)
	{	
		if(_pathBubbles[i] && _pathBubbles[i]->isVisible())
		{
			PathBubble1* B1=_pathBubbles[i];
			subPathBubble1 * sbubble = dynamic_cast< subPathBubble1 *>( B1);
			QPointF Dis = sbubble->Dis;
			int pid1=B1->m_pathwayID;
			
			//int Cnum,Enum,Pnum,Snum,Dnum,Rnum;
			//Cnum=Enum=Pnum=Snum=Dnum=Rnum=0;
			QRectF Rect;
			float x0,y0,x1,y1;
			x0=1000000, y0=1000000, x1=-1000000, y1=-1000000;
			fontSizeM=-1;
			/*for(int i=0; i<B1->_itemSelected.size(); i+=2)
			{
				int type=B1->_itemSelected[i];
				int id1=B1->_itemSelected[i+1];	
            */
			for(int i=0; i<B1->itemInfo.size(); i++)
			{
				int type=B1->itemInfo[i][0];//B1->_itemSelected[i];
				int id0=B1->itemInfo[i][1], id1=B1->itemInfo[i][2];//B1->_itemSelected[i+1];	
				vector<int> item;
				item.push_back(type); item.push_back(id1);
				if(m_pathwayID != B1->itemInfo[i][5])
					continue;

				QString reactionType="";
				QRectF nodePos;
				switch(type)
				{
					case 'C':   
						        bRect=_scene->_complexPos[pid1][id1]; bRect.moveCenter(bRect.center()+Dis);
						        Rect=getNodeRect( bRect, "", type,  fixedSize,  dCenter, _scale);	
						        text =  _scene->_complexNameD[B1->dataID][id1][0];
								nodePos = _scene->_complexPos[pid1][id1]; 
						        break;
					case 'E':   
						        bRect=_scene->_physicalEntityPos[pid1][id1]; bRect.moveCenter(bRect.center()+Dis);
						        Rect=getNodeRect( bRect, "", type,  fixedSize,  dCenter, _scale);
						        text = _scene->_physicalEntityNameD[B1->dataID][id1][0];
								nodePos = _scene->_physicalEntityPos[pid1][id1]; 
								break;
					case 'P':   
						        bRect=_scene->_proteinPos[pid1][id1]; bRect.moveCenter(bRect.center()+Dis);
						        Rect=getNodeRect( bRect, "", type,  fixedSize,  dCenter, _scale);	
						        text = _scene->_proteinNameD[B1->dataID][id1][0];
								nodePos = _scene->_proteinPos[pid1][id1];
								break;
					case 'S': 	
						        bRect=_scene->_smallMoleculePos[pid1][id1]; bRect.moveCenter(bRect.center()+Dis);							
						        Rect=getNodeRect( bRect, "", type,  fixedSize,  dCenter, _scale);	
						        text=_scene->_smallMoleculeNameD[B1->dataID][id1][0];
								nodePos = _scene->_smallMoleculePos[pid1][id1]; 
								break;
					case 'D':  	
						        bRect=_scene->_DnaPos[pid1][id1]; bRect.moveCenter(bRect.center()+Dis);
						        Rect=getNodeRect( bRect, "", type,  fixedSize,  dCenter, _scale);	
						        text = _scene->_DnaNameD[B1->dataID][id1][0];
								nodePos = _scene->_DnaPos[pid1][id1]; 
								break;
					case 'R':  	
						        bRect=_scene->_reactionPos[pid1][id1]; bRect.moveCenter(bRect.center()+Dis);
						        Rect=getNodeRect( bRect, "", type,  fixedSize,  dCenter, _scale);	
						        text= _scene->_reactionNameD[B1->dataID][id1][1];
								reactionType=_scene->_reactionNameD[B1->dataID][id1][0];
								nodePos = _scene->_reactionPos[pid1][id1]; 
								break;
					case 'M': 	
						        bRect=_scene->_compartmentPos[pid1][id1]; bRect.moveCenter(bRect.center()+Dis);
						        Rect=getNodeRect( bRect, "", type,  fixedSize,  dCenter, _scale);	
						        text = _scene->_compartmentName[m_pathwayID][id1][0];
								nodePos = _scene->_compartmentPos[pid1][id1];
								break; 					
				}
	
				if(Rect.x()>-990.99)
				{
					if(x0>Rect.x())
					x0=Rect.x();
				
					if(y0>Rect.y())
						y0=Rect.y();
				
					if(x1 < Rect.x()+Rect.width())
						x1 = Rect.x()+Rect.width();
				
					if(y1 < Rect.y()+Rect.height())
						y1 = Rect.y()+Rect.height();
				}
				rect = getNewNodeRect(nodePos, type, dCenter, _scale, reactionType);	
			    
				
				if(type == 'C' || type == 'R')
					text="";
				if(type!='M')
					getNodeFont(fontSize, fontRect, rect, text, type, width, height, fixedSize, _scale);	
				else 
					getCompartmentFont(fontSize, fontRect, rect, text, fixedSize, _scale);
				
				boundingFontSize.push_back(-fontSize);
				boundingBoxes.push_back(Rect);
				boundingText.push_back(text); //record text for boundingBoxes	
			}					
			if(fontSizeM>0)
			{
				fontSize=fontSizeM;			
				text=textM;
				rect=RectM;
			}
			boundingFontSize.push_back(fontSize);
			boundingBoxes.push_back(QRectF(x0,y0,x1-x0,y1-y0));
			boundingText.push_back(""); //record text for boundingBoxes				
		}
	}
}

float PathBubble1::getGraphToBePaint()
{   
    getEdgeToBePaint();
	float sRatio = getNodeToBePaint();	
	getBoundingBoxToBePaint();
	recordItemGrid();
	return sRatio;
}

void PathBubble1::moveGraphToBePaint(QPointF dis)
{    
	moveEdgeToBePaint(dis);
	moveNodeToBePaint(dis);		
	getBoundingBoxToBePaint();
	recordItemGrid();
}

void PathBubble1::scaleGraphToBePaint(QPointF dCenter1, float scale1, QPointF dCenter2, float scale2)
{    
	scaleEdgeToBePaint(dCenter1, scale1, dCenter2, scale2);
	scaleNodeToBePaint(dCenter2, scale2);
	getBoundingBoxToBePaint();
	recordItemGrid();
}



void PathBubble1::addtoGraph()
{
	//static int initiated=m_pathwayID;
	if(!addedtoGraph)
	   _scene->updateGraphs();

	addedtoGraph=true;
}

void PathBubble1::removeFromGraph()
{
	_scene->updateGraphs();

}

void PathBubble1::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{    		
    updateLabel();
	
	PlotBase::paint( painter, option, widget );

	addtoGraph();

	painter->save();
	
	if(!itemSelected.empty() && draggingItem==1 )
	{
		paintItemDragged(painter);
	}

	QRegion reg=getClipRegionforCalloutNote();
	painter->setClipRegion(reg);
	QPen pen;
	pen.setWidth(3);			
	pen.setColor(QColor(255,255,255,200));                           	    
	painter->setPen(pen);					
	for(int i=0; i<boundingBoxes.size(); i++)
	{
		if(boundingFontSize[i]>0)
		{ 
			painter->drawRect(boundingBoxes[i]);							
			QFont f1("Arial", boundingFontSize[i]); //boundingFontSize[i]);
			painter->setFont (f1);			
			painter->drawText(boundingBoxes[i], boundingText[i], Qt::AlignCenter|Qt::AlignBottom); 
		}
		else
		{ 
			QFont f1("Arial", -boundingFontSize[i]); //boundingFontSize[i]);
			painter->setFont (f1);			
			painter->drawText(boundingBoxes[i], boundingText[i], Qt::AlignCenter|Qt::AlignBottom); 
		}			
	}
	drawConnections_H(painter, QPointF(0,0)); //sceneBoundingRect().center()
    paintGraph(painter, true);
	paintGraph(painter, false);

	painter->setBrush(QColor(255,255,255));
	
	if(_penisOn)
	{
		drawCurve(painter);
	}
	else if(groupSelected)
	{
		drawfills(painter);	
	}	
    hoveredItem = _scene->hoveredItem;

	painter->setClipping(false);
	for(int i=0; i<_scene->hoveredItem.size(); i=i+3)
    {
		int pid=hoveredItem[i], type=hoveredItem[i+1], id=hoveredItem[i+2];
		QRectF nodePos;
		QString nodeName;
		if(pid==m_pathwayID)
		{
			switch(type)
			{
				case 'C': nodePos=complexPos[id],        nodeName=_scene->_complexNameD[dataID][id][0]; break;
				case 'E': nodePos=physicalEntityPos[id], nodeName=_scene->_physicalEntityNameD[dataID][id][0]; break;
				case 'P': nodePos=proteinPos[id],        nodeName=_scene->_proteinNameD[dataID][id][0];  break;
				case 'S': nodePos=smallMoleculePos[id];  nodeName=_scene->_smallMoleculeNameD[dataID][id][0]; break;		
				case 'D': nodePos=DnaPos[id],            nodeName=_scene->_DnaNameD[dataID][id][0]; break;	 		
				case 'R': nodePos=reactionPos[id],       nodeName=_scene->_reactionNameD[dataID][id][1]; break;	 
				case 'L': nodePos=ANodePos[id],          nodeName=_scene->_ANodeName[m_pathwayID][id][1]; break;	 
			}
			if(type =='R' || type =='C' || maxFontSize<8 || type =='L')
				  paintHoveredText( painter, nodePos,  nodeName, type, id, dCenter, _scale);
		}
    }	
	painter->setClipping(true);
	renderColorLegend(painter);
	//drawConnections(painter, QPointF(0,0)); //sceneBoundingRect().center()
	painter->restore();
}

void PathBubble1::paintGraph(QPainter *painter, bool drawHighLighting)
{
	QPointF center=this->sceneBoundingRect().center();
	QRectF oriRect = QRectF((float)center.x()-(float)this->realRectF().width()/2, (float)center.y()-(float)this->realRectF().height()/2, (float)this->realRectF().width(), (float)this->realRectF().height());
	painter->save();
		
	QRectF bRect=sceneBoundingRect();
	QRect rRect=realRect();
	
	float cx=sceneBoundingRect().center().x();
	int rcx=rRect.center().x();

	float swidth = sceneBoundingRect().width();
	int rwidth = realRect().width();
	
	QPointF start,middle,end;
	vector<int> temp(2,0),etemp(3,0);
	
	vector<vector<int>> tedge(2,etemp);	
	float width;
	float height;

	width=graphReferenceSize/2*_scale; height=graphReferenceSize/2*_scale;
		
	int dcx=dCenter.x()*this->realRect().width(), dcy=dCenter.y()*this->realRect().height();
	QPointF DC(dcx,dcy);
	
	vector<int> node(2,0);
	vector<vector<int>> ed(2, node);
	
    painter->setBrush(Qt::NoBrush);		
	QRectF tRect;
	float x,y,w,h;
	QPen pen;	
	

		painter->setPen(pen);		
		QFont f("Arial", 10);
		painter->setFont (f);
	
		pen.setWidth(1);
		pen.setColor(QColor(0,0,0,255));                           	
		painter->setPen(pen);		
			
		if(drawHighLighting)
		{
			QColor color=HIGHLIGHTCOLOR;
			painter->setPen(color);	
			glColor4f(color.redF()/255.0, color.greenF()/255.0, color.blueF()/255.0, color.alphaF()/255.0);			
		}
		else 
		{
			glColor4f(0, 0, 0, 1.0);
			painter->setPen(QColor(0,0,0,255));	
		}

		painter->drawLine(0,1,0,1);

		
		for(set<vector<vector<int> > >::iterator it=updatedEdge_1.begin(); it!=updatedEdge_1.end(); it++)
		{		
				int size=updatedEdge_1.size();
				vector<vector<int> > edge=*it;						
				if(!inUpdatedEdge(edge, updatedEdge_4))
				{
					int pid1=edge[0][0], type1=edge[0][1], id1=edge[0][2], pid2=edge[1][0], type2=edge[1][1], id2=edge[1][2];				 
					QColor bc; 
					bool drawIt=false;
					if( drawHighLighting && linkedEdge.size()> m_pathwayID && linkedEdge[m_pathwayID].find(edge)!=linkedEdge[m_pathwayID].end()) //keqin
					{
						
						/*float level=findLinkedLevel(pid1, type1, id1, pid2, type2, id2);
						float totalLevel=getTotalLinkLevel();											
						if(level<0)
						{
							level=findLinkedLevel(pid2, type2, id2, pid1, type1, id1);				
						}*/
						bc=_scene->getPathColor(pid1,  type1,  id1,  pid2,  type2,  id2);//assignColorToLinkedItems(level, totalLevel,1);
						glColor4f(bc.red()/255.0, bc.green()/255.0, bc.blue()/255.0, 1.0);
			            painter->setPen(bc);			
		                painter->drawLine(0,1,0,1);

						drawIt=true;
					}					
					else if( !drawHighLighting)
					{
						drawIt=true;
						bc=QColor(50, 50, 50, 255);
					}
					if(!drawIt)
						continue;

					if(pid1==pid2 && pid1==m_pathwayID)
					{
						QPointF start, end;
						switch(type1)
						{
							case 'C': start=complexRect[id1].center();        break;
							case 'D': start=DnaRect[id1].center();            break;
							case 'E': start=physicalEntityRect[id1].center(); break; 
							case 'L': start=ANodeRect[id1].center();          break; 
							case 'P': start=proteinRect[id1].center();        break;
							case 'S': start=smallMoleculeRect[id1].center();  break;			
							case 'R': start=reactionRect[id1].center();       break;  //catalysis for bio reaction	
						}
						switch(type2)
						{
							case 'C': end=complexRect[id2].center();    	  break;
							case 'D': end=DnaRect[id2].center();             break;
							case 'E': end=physicalEntityRect[id2].center();  break;
							case 'L': end=ANodeRect[id2].center();           break; 
							case 'P': end=proteinRect[id2].center();         break;
							case 'S': end=smallMoleculeRect[id2].center();   break;			
							case 'R': end=reactionRect[id2].center();        break;  //catalysis for bio reaction	
						}	
						glBegin(GL_QUADS);

						if(drawHighLighting)
						    drawArrow_GL_H(painter, start, end, width, height,_scale);	//end
						else
							drawArrow_GL(painter, start, end, width, height);	//end

						glEnd();
					}
				}	
	}
	

	if(drawHighLighting)
	{
		QColor color=HIGHLIGHTCOLOR;
		painter->setPen(color);	
		glColor4f(color.redF()/255.0, color.greenF()/255.0, color.blueF()/255.0, color.alphaF()/255.0);			
	}
	else 
	{
		glColor4f(0.0, 0.825, 0.196, 1.0);
		painter->setPen(QColor(0, 210, 50, 255));	//painter->setPen(QColor(0,0,0,255));	
	}
	painter->drawLine(0, 1,0,1);
	int count1=0;
	//glBegin(GL_QUADS);  	
	for(set<vector<vector<int> > >::iterator it=updatedEdge_2.begin(); it!=updatedEdge_2.end(); it++)
    {   //activation
		vector<vector<int> > edge=*it;
		if(!inUpdatedEdge(edge, updatedEdge_4))
		{
			int pid1=edge[0][0], type1=edge[0][1], id1=edge[0][2], pid2=edge[1][0], type2=edge[1][1], id2=edge[1][2];
			QColor bc;
			bool drawIt=false;
			if( drawHighLighting && linkedEdge.size()> m_pathwayID && linkedEdge[m_pathwayID].find(edge)!=linkedEdge[m_pathwayID].end()) //keqin
			{
				/*float level=findLinkedLevel(pid1, type1, id1, pid2, type2, id2);
				float totalLevel=getTotalLinkLevel();											
				if(level<0)
				{
					level=findLinkedLevel(pid2, type2, id2, pid1, type1, id1);				
				}
				bc=assignColorToLinkedItems(level, totalLevel,1);*/
				bc=_scene->getPathColor(pid1,  type1,  id1,  pid2,  type2,  id2);//assignColorToLinkedItems(level, totalLevel,1);
				glColor4f(bc.red()/255.0, bc.green()/255.0, bc.blue()/255.0, 1.0);
			    painter->setPen(bc);			
		        painter->drawLine(0,1,0,1);
				drawIt=true;
			}
			else if( !drawHighLighting)
			{
				bc=QColor(50, 50, 50, 255);
				drawIt=true;
			}

			if(!drawIt)
				continue;
			if(pid1==pid2 && pid1==m_pathwayID)
			{
				QPointF start, end;
				switch(type1)
				{
					case 'C': start=complexRect[id1].center();        break;
					case 'D': start=DnaRect[id1].center();            break;
					case 'E': start=physicalEntityRect[id1].center(); break; 
					case 'L': start=ANodeRect[id1].center();          break; 
					case 'P': start=proteinRect[id1].center();        break;
					case 'S': start=smallMoleculeRect[id1].center();  break;			
					case 'R': start=reactionRect[id1].center();       break; //catalysis for bio reaction	
				}
				switch(type2)
				{
					case 'C': end=complexRect[id2].center();    	 break;
					case 'D': end=DnaRect[id2].center();             break;
					case 'E': end=physicalEntityRect[id2].center();  break; 
					case 'L': end=ANodeRect[id2].center();           break; 
					case 'P': end=proteinRect[id2].center();         break;
					case 'S': end=smallMoleculeRect[id2].center();   break;			
					case 'R': end=reactionRect[id2].center();        break;  //catalysis for bio reaction	
				}	
				glBegin(GL_QUADS);
				if(drawHighLighting)
					drawArrow_GL_H(painter, start, end, width, height,_scale);	//end
				else
					drawArrow_GL(painter, start, end, width, height);	//end
				glEnd();				
			}
		}
	}
	//glEnd();
	
	if(drawHighLighting)
	{
		QColor color=HIGHLIGHTCOLOR;
		painter->setPen(color);	
		glColor4f(color.redF()/255.0, color.greenF()/255.0, color.blueF()/255.0, color.alphaF()/255.0);			
	}
	else 
	{
		glColor4f(0.0, 0.825, 0.196, 1.0);
		painter->setPen(QColor(255, 128, 0, 255));		
	}	
	painter->drawLine(0,1,0,1);
	//glBegin(GL_QUADS);  	
	
	for(set<vector<vector<int> > >::iterator it=updatedEdge_3.begin(); it!=updatedEdge_3.end(); it++)
	{
			vector<vector<int> > edge=*it;
			if(!inUpdatedEdge(edge, updatedEdge_4))
			{
				int pid1=edge[0][0], type1=edge[0][1], id1=edge[0][2], pid2=edge[1][0], type2=edge[1][1], id2=edge[1][2];
				QColor bc; 
				bool drawIt=false;
			    if( drawHighLighting && linkedEdge.size()> m_pathwayID && linkedEdge[m_pathwayID].find(edge)!=linkedEdge[m_pathwayID].end()) //keqin
				{
					/*float level=findLinkedLevel(pid1, type1, id1, pid2, type2, id2);
					float totalLevel=getTotalLinkLevel();											
					if(level<0)
					{
						level=findLinkedLevel(pid2, type2, id2, pid1, type1, id1);				
					}
					bc=assignColorToLinkedItems(level, totalLevel,1);*/
					bc=_scene->getPathColor(pid1,  type1,  id1,  pid2,  type2,  id2);//assignColorToLinkedItems(level, totalLevel,1);
					glColor4f(bc.red()/255.0, bc.green()/255.0, bc.blue()/255.0, 1.0);
					painter->setPen(bc);			
					painter->drawLine(0,1,0,1);
					drawIt=true;
				}
				else if( !drawHighLighting)
				{
					bc=QColor(50, 50, 50, 255);
					drawIt=true;
				}

				if(!drawIt)
				    continue;

				if(pid1==pid2 && pid1==m_pathwayID)
				{
					QPointF start, end;
					switch(type1)
					{
						case 'C': start=complexRect[id1].center();        break;
						case 'D': start=DnaRect[id1].center();            break;
						case 'E': start=physicalEntityRect[id1].center(); break; 
						case 'L': start=ANodeRect[id1].center();          break; 
						case 'P': start=proteinRect[id1].center();        break;
						case 'S': start=smallMoleculeRect[id1].center();  break;			
						case 'R': start=reactionRect[id1].center();       break;  //catalysis for bio reaction	
					}
					switch(type2)
					{
						case 'C': end=complexRect[id2].center();    	  break;
						case 'D': end=DnaRect[id2].center();            break;
						case 'E': end=physicalEntityRect[id2].center();  break; 
						case 'L': end=ANodeRect[id2].center();            break;
						case 'P': end=proteinRect[id2].center();         break;
						case 'S': end=smallMoleculeRect[id2].center();   break;			
						case 'R': end=reactionRect[id2].center();      break;  //catalysis for bio reaction	
					}
					glBegin(GL_QUADS);
					if(drawHighLighting)
						drawArrow_GL_H(painter, start, end, width, height,_scale);	//end
					else
						drawArrow_GL(painter, start, end, width, height);	//end
					glEnd();
				}
			}	
	}			     
	//glEnd();

	
	int eid=0;
	for(set<vector<vector<int> > >::iterator it=updatedEdge_4.begin(); it!=updatedEdge_4.end(); it++)
	{//multi-link

				vector<vector<int> > edge=*it, redge;
				redge.push_back(edge[1]); redge.push_back(edge[0]);
				int pid1=edge[0][0], type1=edge[0][1], id1=edge[0][2], pid2=edge[1][0], type2=edge[1][1], id2=edge[1][2];
				QColor bc;
				bool drawIt=false;
			    if( drawHighLighting && linkedEdge.size()> m_pathwayID && linkedEdge[m_pathwayID].find(edge)!=linkedEdge[m_pathwayID].end()) //keqin
				{
					/*float level=findLinkedLevel(pid1, type1, id1, pid2, type2, id2);
					float totalLevel=getTotalLinkLevel();											
					if(level<0)
					{
						level=findLinkedLevel(pid2, type2, id2, pid1, type1, id1);				
					}
					bc=assignColorToLinkedItems(level, totalLevel,1);*/
					bc=_scene->getPathColor(pid1,  type1,  id1,  pid2,  type2,  id2);//assignColorToLinkedItems(level, totalLevel,1);
					//glColor4f(bc.red()/255.0, bc.green()/255.0, bc.blue()/255.0, 1.0);
					//painter->setPen(bc);			
					//painter->drawLine(0,1,0,1);
					drawIt=true;

				}
				else if( !drawHighLighting)
				{
					bc=QColor(50, 50, 50, 255);
					drawIt=true;
				}
				if(!drawIt)
				    continue;
				int Aid=-1;
				if(pid1==pid2 && pid1==m_pathwayID)
				{
					QPointF start, end;
					float height=0;
					switch(type1)
					{
						case 'C': start=complexRect[id1].center();  height=complexRect[id1].height();     break;
						case 'D': start=DnaRect[id1].center();  height=DnaRect[id1].height();           break;
						case 'E': start=physicalEntityRect[id1].center(); height=physicalEntityRect[id1].height(); break; 
						case 'L': start=ANodeRect[id1].center();   height=ANodeRect[id1].height();   Aid=id1;     break; 
						case 'P': start=proteinRect[id1].center();   height=proteinRect[id1].height();      break;
						case 'S': start=smallMoleculeRect[id1].center(); height=smallMoleculeRect[id1].height();  break;			
						case 'R': start=reactionRect[id1].center();   height=reactionRect[id1].height();     break;  //catalysis for bio reaction	
					}
					switch(type2)
					{
						case 'C': end=complexRect[id2].center();    height+=complexRect[id2].height(); 	  break;
						case 'D': end=DnaRect[id2].center();     height+=DnaRect[id2].height(); 	       break;
						case 'E': end=physicalEntityRect[id2].center();  height+=physicalEntityRect[id2].height(); 	break; 
						case 'L': end=ANodeRect[id2].center();  Aid=id2; height+=ANodeRect[id2].height(); 	break;
						case 'P': end=proteinRect[id2].center();         height+=proteinRect[id2].height(); 	break;
						case 'S': end=smallMoleculeRect[id2].center();   height+=smallMoleculeRect[id2].height(); 	break;			
						case 'R': end=reactionRect[id2].center();     height+=reactionRect[id2].height(); 	 break;  //catalysis for bio reaction	
					}				 
				int encode = updatedEdge_5[eid++];
				set<int> Code;
				for(int i=0; i<6; i++)
				{
					int code=pow(2.0, i);			
					if(code & encode)
					{
						Code.insert(i);
					}
				}
                /*if(drawHighLighting)
					drawCurvedArrow_GL_H(painter, Code, height/2, start, end, width, height, _scale);
				else  
					drawCurvedArrow_GL(painter, Code, height/2, start, end, width, height);*/

				if(drawHighLighting)
					drawCurvedArrow_GL_H(painter, Code, height/2, start, end, width, height, _scale, bc);
				else  
					drawCurvedArrow_GL(painter, Code, height/2, start, end, width, height);
			}
	}			     	
	

	int oCount=0;
	//set<int> xlist;
	int size=compartmentPos.size()-1;
    for(int i=0; i<size ; i++) //size?
	{
		    //if(!drawHighLighting && boundaryColor[oCount] == HIGHLIGHTCOLOR )
			//	continue;
			if(i<compartmentRect.size() && compartmentRect[i].width()>=0 )
			{   
				if(drawHighLighting)
				{
					 //if(boundaryColor[oCount]==HIGHLIGHTCOLOR)
					if(insideColor[oCount].size()>0)
					if(insideColor[oCount][0]==HIGHLIGHTCOLOR)
				  	    paintPreComputedCompartment_H( painter, boundaryWidth[oCount], fontSize[oCount], insideColor[oCount], boundaryColor[oCount], fontColor[oCount], fontRect[oCount], compartmentRect[i], compartmentPos, _scene->_compartmentName[m_pathwayID], i, _scene->getGroup(this)->sharedSets, dCenter, _scale);	        					
				}
				else 
					paintPreComputedCompartment( painter, boundaryWidth[oCount], fontSize[oCount], insideColor[oCount], boundaryColor[oCount], fontColor[oCount], fontRect[oCount], compartmentRect[i], compartmentPos, _scene->_compartmentName[m_pathwayID], i, _scene->getGroup(this)->sharedSets, dCenter, _scale);	        					
			}			 
			oCount++;		    
	}
	

	temp[0]='C';	
	for(int i=0; i<complexPos.size(); i++)
	{
		if(complexRect[i].width()>=0)
		{  	
			if(drawHighLighting)
			{
				if(insideColor[oCount][0]==HIGHLIGHTCOLOR)
				    paintPreComputedNode_H( painter, boundaryWidth[oCount], fontSize[oCount], insideColor[oCount], boundaryColor[oCount], fontColor[oCount], fontRect[oCount], complexRect[i], complexPos[i], _scene->_complexNameD[dataID][i][0], 'C', i, _scene->getGroup(this)->sharedSets, dCenter, _scale);	        
			}
			else 
				paintPreComputedNode( painter, boundaryWidth[oCount], fontSize[oCount], insideColor[oCount], boundaryColor[oCount], fontColor[oCount], fontRect[oCount], complexRect[i], complexPos[i], _scene->_complexNameD[dataID][i][0], 'C', i, _scene->getGroup(this)->sharedSets, dCenter, _scale);	        
		}	
		oCount++;		
	}	
	temp[0]='R';
	for(int i=0; i<reactionPos.size(); i++)
	{
		if(reactionRect[i].width()>=0)
		{
			if(drawHighLighting)
			{
				if(insideColor[oCount][0]==HIGHLIGHTCOLOR)
				    paintPreComputedNode_H( painter, boundaryWidth[oCount], fontSize[oCount], insideColor[oCount], boundaryColor[oCount], fontColor[oCount], fontRect[oCount], reactionRect[i],reactionPos[i], _scene->_reactionNameD[dataID][i][1], 'R', i, _scene->getGroup(this)->sharedSets,dCenter, _scale, _scene->_reactionNameD[dataID][i][0] );			
			}
			else 
				paintPreComputedNode( painter, boundaryWidth[oCount], fontSize[oCount], insideColor[oCount], boundaryColor[oCount], fontColor[oCount], fontRect[oCount], reactionRect[i],reactionPos[i], _scene->_reactionNameD[dataID][i][1], 'R', i, _scene->getGroup(this)->sharedSets,dCenter, _scale, _scene->_reactionNameD[dataID][i][0] );			
		}		
		oCount++;
	}
	temp[0]='S';
	for(int i=0; i<smallMoleculePos.size(); i++)
	{
		if(smallMoleculeRect[i].width()>=0)
		{
			if(drawHighLighting)
			{
				if(insideColor[oCount][0]==HIGHLIGHTCOLOR)
				    paintPreComputedNode_H( painter, boundaryWidth[oCount], fontSize[oCount], insideColor[oCount], boundaryColor[oCount], fontColor[oCount], fontRect[oCount], smallMoleculeRect[i], smallMoleculePos[i], _scene->_smallMoleculeNameD[dataID][i][0], 'S', i, _scene->getGroup(this)->sharedSets, dCenter, _scale);
			}
			else 
				paintPreComputedNode( painter, boundaryWidth[oCount], fontSize[oCount], insideColor[oCount], boundaryColor[oCount], fontColor[oCount], fontRect[oCount], smallMoleculeRect[i], smallMoleculePos[i], _scene->_smallMoleculeNameD[dataID][i][0], 'S', i, _scene->getGroup(this)->sharedSets, dCenter, _scale);
		}		
		oCount++;
	}
	
	temp[0]='D';
	for(int i=0; i<DnaPos.size(); i++)
	{		
		if(DnaRect[i].width()>=0)	
		{	
			if(drawHighLighting)
			{
				if(insideColor[oCount][0]==HIGHLIGHTCOLOR)
				    paintPreComputedNode_H( painter, boundaryWidth[oCount], fontSize[oCount], insideColor[oCount], boundaryColor[oCount], fontColor[oCount], fontRect[oCount], DnaRect[i], DnaPos[i], _scene->_DnaNameD[dataID][i][0], 'D', i, _scene->getGroup(this)->sharedSets, dCenter, _scale);
			}
			else 
				paintPreComputedNode( painter, boundaryWidth[oCount], fontSize[oCount], insideColor[oCount], boundaryColor[oCount], fontColor[oCount], fontRect[oCount], DnaRect[i], DnaPos[i], _scene->_DnaNameD[dataID][i][0], 'D', i, _scene->getGroup(this)->sharedSets, dCenter, _scale);
		}	
		oCount++;		
	}

	temp[0]='E';
	for(int i=0; i<physicalEntityPos.size(); i++)
	{
		temp[1]=i; 	    
		if( physicalEntityRect[i].width()>=0)
		{
			if(drawHighLighting)
			{
				if(insideColor[oCount][0]==HIGHLIGHTCOLOR)
				    paintPreComputedNode_H( painter, boundaryWidth[oCount], fontSize[oCount], insideColor[oCount], boundaryColor[oCount], fontColor[oCount], fontRect[oCount],  physicalEntityRect[i], physicalEntityPos[i], _scene->_physicalEntityNameD[dataID][i][0], 'E', i, _scene->getGroup(this)->sharedSets,dCenter, _scale);			  
			}
			else
				paintPreComputedNode( painter, boundaryWidth[oCount], fontSize[oCount], insideColor[oCount], boundaryColor[oCount], fontColor[oCount], fontRect[oCount],  physicalEntityRect[i], physicalEntityPos[i], _scene->_physicalEntityNameD[dataID][i][0], 'E', i, _scene->getGroup(this)->sharedSets,dCenter, _scale);			  
		}
		oCount++;		  
	}	
	temp[0]='P';
	for(int i=0; i<proteinPos.size(); i++)
	{	
		if(proteinRect[i].width()>=0)
		{
			if(drawHighLighting)
			{
				 if(insideColor[oCount][0]==HIGHLIGHTCOLOR)
				     paintPreComputedNode_H( painter, boundaryWidth[oCount], fontSize[oCount], insideColor[oCount], boundaryColor[oCount], fontColor[oCount], fontRect[oCount], proteinRect[i], proteinPos[i], _scene->_proteinNameD[dataID][i][0], 'P', i, _scene->getGroup(this)->sharedSets, dCenter, _scale);	 
			}
			else 
				paintPreComputedNode( painter, boundaryWidth[oCount], fontSize[oCount], insideColor[oCount], boundaryColor[oCount], fontColor[oCount], fontRect[oCount], proteinRect[i], proteinPos[i], _scene->_proteinNameD[dataID][i][0], 'P', i, _scene->getGroup(this)->sharedSets, dCenter, _scale);	 
		}
		oCount++;	  
	}
	temp[0]='L';
	for(int i=0; i<ANodePos.size()&& i<_scene->_ANodeName[m_pathwayID].size() && i<ANodeRect.size(); i++)
	{
		temp[1]=i; 	    
		if( ANodeRect[i].width()>=0)
		{
			int size2=_scene->_ANodeName[m_pathwayID][i].size();
			if(size2>0)
			{
				QString A=_scene->_ANodeName[m_pathwayID][i][0];
				if(drawHighLighting)
				{
					if(insideColor[oCount][0]==HIGHLIGHTCOLOR)
					    paintPreComputedNode_H( painter, boundaryWidth[oCount], fontSize[oCount], insideColor[oCount], boundaryColor[oCount], fontColor[oCount], fontRect[oCount],  ANodeRect[i], ANodePos[i], _scene->_ANodeName[m_pathwayID][i][0], 'L', i, _scene->getGroup(this)->sharedSets, dCenter, _scale);			  
				}
				else 
					paintPreComputedNode( painter, boundaryWidth[oCount], fontSize[oCount], insideColor[oCount], boundaryColor[oCount], fontColor[oCount], fontRect[oCount],  ANodeRect[i], ANodePos[i], _scene->_ANodeName[m_pathwayID][i][0], 'L', i, _scene->getGroup(this)->sharedSets, dCenter, _scale);			  
			}
		}
		oCount++;		  
	}
	painter->restore();
}

void PathBubble1::renderColorLegend(QPainter *painter)
{
	painter->save();
	float height= this->Height()/2-25;
	int itv=20;
	int sx= this->Width()/2-139, sy=height;
	QFont f1("Arial", 10);
	painter->setFont (f1);	
	QString oName, eName;
	int addLength=0;
    if(_orthologyFileName.size()>0)
	{
		//oName = "Species:";					
		oName = _orthologyFileName;	
		QFontMetrics fontMetrics(f1); 
        QRectF rect3 = fontMetrics.boundingRect(0, 0, 200, 0, Qt::TextExpandTabs|Qt::TextWrapAnywhere, oName); 
		float tx = this->Width()/2 -rect3.width()-40;
		if(tx<sx)
			sx=tx;
		addLength++;
	}
	
	if(eName.size()>0 || oName.size()>0)
	{
		painter->setPen(Qt::NoPen);
		painter->setBrush(Qt::white);
		painter->drawRect(QRect(sx-6, sy-itv*(addLength*1.6 - 1.5), this->Width()/2-sx, 19+itv*(addLength*1.6 - 1.5)));
	}

	itv=19;
	sy=height-4;
	int grad=7;
	{
		for(int i=0; i<1; ++i)//grad
		{
			QString str;
			/*QRect rect(sx, sy-itv*i, 16, itv-2);
			float r, g, b;
			float value=(i+0.5)/grad;
			QPointF p1, p2;
			if(i==0 || i==1)
			{
			    rect = QRect(sx, sy-itv*i+4, 16, itv-2);
			}
			if(i==6)
			{
				p1=QPointF(sx, sy-itv*i + itv/2);
				p2=QPointF(sx+itv*2.7, sy-itv*i + itv/2);					
				drawArrow_4(painter, p1, p2, 400, 400, QColor(0, 210, 50, 255) );	
				str= "activation";
				painter->drawText(sx+60, sy-itv*(i-0.55), str);
			}
			else if(i==5)
			{  
				p1=QPointF(sx, sy-itv*i + itv/2-2);
				p2=QPointF(sx+itv*2.7, sy-itv*i + itv/2-2);	
				drawArrow_4(painter, p1, p2, 400, 400, QColor(255, 128, 0, 255) );	
				str= "inhibition";
				painter->drawText(sx+60, sy-itv*(i-0.55)-2, str);
			}
			else if(i==4)
			{  
				painter->setBrush(QColor(255, 180, 180,255));
				str= "rate-limited";
			}			
			else if(i==3)
			{
				painter->setBrush(QColor(180, 180, 255, 255)); //htColor[l] = QColor(81,81,255,255);htColor[l] = QColor(230,68,113,255);QColor(255,100,100,255);QColor(206,206,206,228);
				str= "crosstalk";
			}
			else if(i==2)
			{
				painter->setBrush(QColor(180, 255, 180, 255));
				str= "orthologous";				
				painter->drawText(sx+20, sy-itv*(i-0.55), str);								
			}	
			else if(i==1)
			{  
				painter->setBrush(QColor(81,81,255,255));
				str= "down-expressed";
			}
			else if(i==0)
			{
				painter->setBrush(QColor(230,68,113,255)); //htColor[l] = QColor(81,81,255,255);htColor[l] = QColor(230,68,113,255);QColor(255,100,100,255);QColor(206,206,206,228);
				str= "up-expressed";
			}
			*/
			
			//three color
			if(i!=6 && i!=5)
			{   				
				/*painter->drawRect(rect);
				if(i==0 || i==1)
				{
			        painter->drawText(sx+20, sy-itv*(i-0.55)+4, str);	
				}
				else if(i!=2)
			        painter->drawText(sx+20, sy-itv*(i-0.55), str);	
				*/

				if(i==0) 
				{
					painter->setPen(QColor(50,50,50,255));	
					/*if(eName.size()>0 && oName.size()>0)
					{
						painter->drawText(sx, sy-itv*(0.2-0.95), "Expression:"+eName); 	
						//str= "Expression:";
						//painter->drawText(sx, sy-itv*(0.2-0.25), str);

						painter->drawText(sx, sy-itv*(1.8-0.95), "Orthology:" + oName); 	
						//str= "Orthology:";
						//painter->drawText(sx, sy-itv*(1.8-0.25), str);
					}
					else if(eName.size()>0)
					{
						painter->drawText(sx, sy-itv*(0.2-0.95),  "Expression:" + eName); 	
						//str= "Expression:";
						//painter->drawText(sx, sy-itv*(0.2-0.25), str);
					}						
					else */
					if(oName.size()>0)
					{
						painter->drawText(sx, sy-itv*(0.2-0.95), "Orthology:" + oName); 	
						//str= "Orthology:";
						//painter->drawText(sx, sy-itv*(1.2-0.25), str);
					}						
				}
			}
		}		
	}	
	painter->restore();	
}

bool PathBubble1::ifExpressionShown()
{
	int i;
    for(i=0; i<expressedSmallMolecule.size(); i++)
	{
		if(!expressedSmallMolecule[i].empty())
			return true;
	}
	for(i=0; i<expressedReaction.size(); i++)
	{
		if(!expressedReaction[i].empty())
			return true;
	}
	for(i=0; i<expressedProtein.size(); i++)
	{
		if(!expressedProtein[i].empty())
			return true;
	}
	for(i=0; i<expressedComplex.size(); i++)
	{
		if(!expressedComplex[i].empty())
			return true;
	}
	for(i=0; i<expressedDna.size(); i++)
	{
		if(!expressedDna[i].empty())
			return true;
	}
	for(i=0; i<expressedPhysicalEntity.size(); i++)
	{
		if(!expressedPhysicalEntity[i].empty())
			return true;
	}
	for(i=0; i<expressedANode.size(); i++)
	{
		if(!expressedANode[i].empty())
			return true;
	}
	return false;
}


bool PathBubble1::clearExpressed()
{
	
	int csize=expressedProtein.size();	
	bool flag;
	for(int i=0; i<csize; i++)
	{
		if(!expressedProtein[i].empty() || expressedProtein.empty()||!expressedSmallMolecule.empty()||!expressedComplex.empty()
			||!expressedDna.empty()||!expressedReaction.empty()||!expressedPhysicalEntity.empty()||!expressedCompartment.empty()||!expressedANode.empty()
			||!expressedPathway.empty())
			flag=true;
	
		expressedProtein.clear();	
		expressedSmallMolecule.clear();
		expressedComplex.clear(); 
		expressedDna.clear();  
		expressedReaction.clear(); 
		expressedPhysicalEntity.clear();  
		expressedANode.clear(); 
		expressedCompartment.clear();  
		expressedPathway.clear();  

		expressedProtein.resize(csize);
		expressedSmallMolecule.resize(csize);
		expressedComplex.resize(csize); 
		expressedDna.resize(csize);  
		expressedReaction.resize(csize); 
		expressedPhysicalEntity.resize(csize);  
		expressedANode.resize(csize);  
		expressedCompartment.resize(csize);  
		expressedPathway.resize(csize);  
	}
	return flag;
}

bool PathBubble1::clearShared()
{
	    bool flag=false;		   
		{
			if(m_pathwayID<sharedSmallMolecule.size())
			{
				sharedSmallMolecule[m_pathwayID].clear();
				if(!sharedSmallMolecule[m_pathwayID].empty())
					flag=true;
			}	
			if(m_pathwayID<sharedReaction.size())
			{
				sharedReaction[m_pathwayID].clear();
				if(!sharedReaction[m_pathwayID].empty())
					flag=true;
			}
			if(m_pathwayID<sharedProtein.size())
			{
				sharedProtein[m_pathwayID].clear();
				if(!sharedProtein[m_pathwayID].empty())
					flag=true;
			}	
			if(m_pathwayID<sharedComplex.size())
			{
				sharedComplex[m_pathwayID].clear();
				if(!sharedComplex[m_pathwayID].empty())
					flag=true;
			}				
			if(m_pathwayID<sharedDna.size())
			{
				sharedDna[m_pathwayID].clear();
				if(!sharedDna[m_pathwayID].empty())
					flag=true;
			}			
			if(m_pathwayID<sharedPhysicalEntity.size())
			{
				sharedPhysicalEntity[m_pathwayID].clear();			
				if(!sharedPhysicalEntity[m_pathwayID].empty())
					flag=true;
			}
		}
		return flag;
}

bool PathBubble1::clearColored()
{
	bool flag=false;	
	if(!_itemColored.empty())
		flag=true;
	_itemColored.clear();
	return flag;
}

bool PathBubble1::clearDiffered()
{
	    bool flag=false;		   
		{
			if(m_pathwayID<differedSmallMolecule.size())
			{
				differedSmallMolecule[m_pathwayID].clear();
				if(!differedSmallMolecule[m_pathwayID].empty())
					flag=true;
			}	
			if(m_pathwayID<differedReaction.size())
			{
				differedReaction[m_pathwayID].clear();
				if(!differedReaction[m_pathwayID].empty())
					flag=true;
			}
			if(m_pathwayID<differedProtein.size())
			{
				differedProtein[m_pathwayID].clear();
				if(!differedProtein[m_pathwayID].empty())
					flag=true;
			}	
			if(m_pathwayID<differedComplex.size())
			{
				differedComplex[m_pathwayID].clear();
				if(!differedComplex[m_pathwayID].empty())
					flag=true;
			}				
			if(m_pathwayID<differedDna.size())
			{
				differedDna[m_pathwayID].clear();
				if(!differedDna[m_pathwayID].empty())
					flag=true;
			}			
			if(m_pathwayID<differedPhysicalEntity.size())
			{
				differedPhysicalEntity[m_pathwayID].clear();			
				if(!differedPhysicalEntity[m_pathwayID].empty())
					flag=true;
			}
			if(m_pathwayID<differedANode.size())
			{
				differedANode[m_pathwayID].clear();			
				if(!differedANode[m_pathwayID].empty())
					flag=true;
			}
		}
		return flag;
}


bool PathBubble1::clearLinked()
{
	    bool flag=false;		   
		{
			if(m_pathwayID<linkedSmallMolecule.size())
			{
				if(!linkedSmallMolecule[m_pathwayID].empty())
					flag=true;
				linkedSmallMolecule[m_pathwayID].clear();
			}	
			if(m_pathwayID<linkedReaction.size())
			{
				if(!linkedReaction[m_pathwayID].empty())
					flag=true;
				linkedReaction[m_pathwayID].clear();				
			}
			if(m_pathwayID<linkedProtein.size())
			{
				if(!linkedProtein[m_pathwayID].empty())
					flag=true;
				linkedProtein[m_pathwayID].clear();				
			}	
			if(m_pathwayID<linkedComplex.size())
			{
				if(!linkedComplex[m_pathwayID].empty())
					flag=true;
				linkedComplex[m_pathwayID].clear();				
			}				
			if(m_pathwayID<linkedDna.size())
			{
				if(!linkedDna[m_pathwayID].empty())
					flag=true;
				linkedDna[m_pathwayID].clear();				
			}			
			if(m_pathwayID<linkedPhysicalEntity.size())
			{
				if(!linkedPhysicalEntity[m_pathwayID].empty())
					flag=true;
				linkedPhysicalEntity[m_pathwayID].clear();							
			}	

			if(m_pathwayID<linkedANode.size())
			{
				if(!linkedANode[m_pathwayID].empty())
					flag=true;
				linkedANode[m_pathwayID].clear();							
			}	

			if(m_pathwayID<linkedEdge.size())
			{
				if(!linkedEdge[m_pathwayID].empty())
					flag=true;
				linkedEdge[m_pathwayID].clear();							
			}			
			
		}
		return flag;
}



int PathBubble1::getVerticeID_2(int type, int id)
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
	  //if(toplevel)
	  //   return ID-1;
	  //else 
		  return ID;
}

int PathBubble1::getVerticeID_3(int type, int id)
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
	  return ID;
}


bool PathBubble1::onNoteMark(int pid, int type, int id, QPointF pos )
{
	CalloutNote* nodeNote=_scene->findNote(pid, type, id);
	if(nodeNote!=NULL )
	{
		//pos = pos + (nodeNote->scenePos() - this->scenePos());
		if( nodeNote->markRect.contains(pos))
    	return true;
	}

	return false;	
}

bool PathBubble1::onBoarder( QPointF pos )
{
	int x = pos.x();
	int y = pos.y();
	int half = this->Width()/2;
	int half2 = this->Height()/2;
	if( (x > half - (half*0.01+4) && x < half+HALOWIDTH) ||  (x < -half + (half*0.01+4) && x > -half-HALOWIDTH) )
	{
		return true;
	}
	if( (y > half2 - (half2*0.01+4) && y < half2+HALOWIDTH) || (y < -half2 + (half2*0.01+4) && y > -half2-HALOWIDTH) )
	{
		return true;
	}
	return false;
}

bool PathBubble1::clearExpressed(int m_expressionID)
{//return true, if it was expressed
        bool flag=false;	
	    //for(int m_expressionID=0; m_expressionID<_scene->_expressionBubbles.size(); m_expressionID++)	
		{
			if(m_expressionID<expressedSmallMolecule.size())
			{
				expressedSmallMolecule[m_expressionID].clear();				
			}
	
			if(m_expressionID<expressedReaction.size())
			{
				expressedReaction[m_expressionID].clear();				
			}

			if(m_expressionID<expressedProtein.size())
			{
				if(!expressedProtein[m_expressionID].empty())
					flag=true;
				expressedProtein[m_expressionID].clear();				
			}
	
			if(m_expressionID<expressedComplex.size())
			{
				if(!expressedComplex[m_expressionID].empty())
					flag=true;
				expressedComplex[m_expressionID].clear();				
			}
				
			if(m_expressionID<expressedDna.size())
			{
				expressedDna[m_expressionID].clear();
			}
			if(m_expressionID<expressedPhysicalEntity.size())
			{
				expressedPhysicalEntity[m_expressionID].clear();			
			}
			if(m_expressionID<expressedANode.size())
			{
				expressedANode[m_expressionID].clear();							
			}
		}
		return flag;
}


void PathBubble1::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	ItemBase::mouseDoubleClickEvent(event);
	
	return;
}


void PathBubble1::hoverMoveEvent( QGraphicsSceneHoverEvent *event )
{   	
	//hoverMoveEvent
	//labelArea
	

	QPoint tPos=QPoint(event->pos().x(),event->pos().y());	
	if(getCalloutNoteRect().contains(tPos))
		return;

	ItemBase::hoverMoveEvent(event);
	for(int i=0; i<_scene->_pathBubbles.size(); i++)
	{
		int size=_scene->_pathBubbles.size();		
		if(scene->_pathBubbles[i])
		if(scene->_pathBubbles[i]->m_pathwayID >=0 && scene->_pathBubbles[i]->m_pathwayID < _scene->_pathBubbles.size())
		   _scene->_pathBubbles[i]->_pathwayHighlight=false;
	}	
	if(m_pathwayID<0)
	{
		return;
	}
	if(!insideBubble(event->pos()))
	{
		_pathwayHighlight=false;			
		if(!_scene->hoveredItem.empty())
		{
			if(_scene->hoveredItem[1]!='W')
			{
				vector<int> empty;
				vector<int> pre=_scene->hoveredItem;
				_scene->hoveredItem.clear();	
				getPartUpdateHoveredNodetoBePaint(pre, empty);				
			}
		    else 
				_scene->hoveredItem.clear();
		}
	}
	else if(!groupSelected)
	{
		if(event->pos().x()>120)
			groupSelected=groupSelected;
		vector<int> item=whichItem(event->pos(),this->Width()*_scale, this->Height()*_scale);
		int resizeflag =  0;
		if(!item.empty() )		
		{	
			int type=item[0], id=item[1];
			if(type!='W')
			{
				QRectF rect;
				switch(type)
				{ 
					case 'C': rect=complexRect[id];
							  break;
					case 'E': rect=physicalEntityRect[id]; 
							  break;
					case 'L': rect=ANodeRect[id]; 
							  break;
					case 'P': rect=proteinRect[id]; 
							  break;
					case 'S': rect=smallMoleculeRect[id]; 
							  break;	
					case 'D': rect=DnaRect[id]; 
							  break;	 	
					case 'R': rect=reactionRect[id]; 
							  break;	 	
					case 'M':
						      rect=compartmentRect[id]; 
							  break;	
				}	
				resizeflag = isInResizeArea(event->pos(), rect);
				if(resizeflag==1 || resizeflag==3)
				{
					setCursor(Qt::SizeFDiagCursor);
				}
				else if(resizeflag==2 || resizeflag==4)
				{
					setCursor(Qt::SizeBDiagCursor);
				}					
			}
		}
		if(resizeflag)
		{
		    vector<int> pre=_scene->hoveredItem;
			vector<int> empty;
			_scene->hoveredItem.clear();
			getPartUpdateHoveredNodetoBePaint(pre, empty);
		}
		else if(!_scene->hoveredItem.empty() )
		{
			if(item.empty() ) 
			{
				vector<int> pre=_scene->hoveredItem;
				vector<int> empty;				
				_scene->hoveredItem.clear();
				getPartUpdateHoveredNodetoBePaint(pre, empty);					
			}
			else if (item[0]!=_scene->hoveredItem[1] || item[1]!=_scene->hoveredItem[2] )
			{
				if(item[0]==80)
					item[0]=item[0];
				vector<int> pre=_scene->hoveredItem;
                _scene->hoveredItem.clear();
				if(_scene->hoveredItem.empty()  )
				{
					_scene->hoveredItem.push_back(m_pathwayID);			
					_scene->hoveredItem.push_back(item[0]);	
					_scene->hoveredItem.push_back(item[1]);												
				}
				getPartUpdateHoveredNodetoBePaint(pre, _scene->hoveredItem);
			}			
		}
		else 
		{
			if(!item.empty() )
			{
				vector<int> pre=_scene->hoveredItem;
				_scene->hoveredItem.clear();
				_scene->hoveredItem.push_back(m_pathwayID);			
				_scene->hoveredItem.push_back(item[0]);	
				_scene->hoveredItem.push_back(item[1]);							
				getPartUpdateHoveredNodetoBePaint(pre, _scene->hoveredItem);
			}			
		}
		if(item.empty())
		{   		   
			if(onTagArea( event->pos() ))
		    {			  
  			    _pathwayHighlight=true;
		    }
			else
			{
				_pathwayHighlight=false;			
			}
		}
		else if(UNGROUP)
		{
			emit unGroup( this );
	        logUnGroup();
			UNGROUP=false;
			AFTERUNGROUP=true;			
		}
		if(_grabisOn)
		{
            Qt::CursorShape shape = Qt::OpenHandCursor;
	        setCursor(shape); 			
	    }	
		else if(_queryisOn)
		{
			setCursor(Qt::WhatsThisCursor);			
	    }	
		else if(_penisOn)
		{
		   QBitmap bitmap("icon/Pencil.png");
		   QCursor cursor(bitmap,bitmap);
		   setCursor(cursor); 
		}
		else if(_upisOn)
		{	
			QBitmap bitmap("icon/GoUp.png");
			QCursor cursor(bitmap,bitmap);
		    setCursor(cursor); 
		}
		else if(_downisOn)
		{
			QBitmap bitmap("icon/GoDown.png");
			QCursor cursor(bitmap,bitmap);
			setCursor(cursor);   
		}
		else if(_evenisOn)
		{
			QBitmap bitmap("icon/GoEven.png");
			QCursor cursor(bitmap,bitmap);
			setCursor(cursor); 
		}
	}
	hoveredItem=_scene->hoveredItem;	
}

int PathBubble1::whichCorner(const QPointF pos, QRectF rect)
{
	float x=pos.x()-rect.center().x(), y=pos.y()-rect.center().y();
	bool result0  = x < 0;
	bool result1 =  y < 0;
	
	bool result2  = x < 0;
	bool result3 =  y> 0;
	
	bool result4  = x> 0;
	bool result5 =  y> 0;
	
	bool result6  =  x> 0;
	bool result7 = y< 0;

	if(result0 && result1) 
		return 1;
	if(result2 && result3) 
		return 2;
	if(result4 && result5) 
		return 3;
	if(result6 && result7) 
		return 4;
	return (0);
}

int PathBubble1::isInResizeArea(const QPointF pos, QRectF rect)
{
	float w=rect.width(), h=rect.height()+4;
	float margin = 12;
	if(margin>w/4)
	    margin=w/4;

	if(margin>h/3)
	    margin=h/3;


	float x=pos.x()-rect.center().x(), y=pos.y()-rect.center().y();
	bool result0  = x >-w/2-margin && x< (-w/2+margin);
	bool result1 = y >-h/2-margin && y< (-h/2+margin);
	
	bool result2  = x> - w/2-margin && x< (- w/2+margin);
	bool result3 = y< h/2+margin && y> (h/2-margin);
	
	bool result4  = x<w/2 +margin  && x> (w/2-margin);
	bool result5 = y<h/2 +margin && y> (h/2-margin);
	
	bool result6  = x<w/2 +margin && x> (w/2-margin);
	bool result7 = y> - h/2 -margin && y< (-h/2+margin);

	if(result0 && result1) 
		return 1;
	if(result2 && result3) 
		return 2;
	if(result4 && result5) 
		return 3;
	if(result6 && result7) 
		return 4;
	return (0);
}


void PathBubble1::updateItemGrid(vector<vector<set<vector<int>>>> &itemGrid, int type, int id, QPointF newPos)
{
	 //find previous grid
	int h=40,w=40;
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
		 case 'M': node=compartmentPos[id];   break;
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

void PathBubble1::recordItemGrid( )
{
	itemGrid=recordItemGrid(visibleItemNum, visibleCompartmentNum);
}

vector<vector<set<vector<int>>>> PathBubble1::recordItemGrid(int &visibleItemNum, int &visibleCompartmentNum)
{
    //10*10
	visibleItemNum=visibleCompartmentNum=0;
	int h=40,w=40;
	vector<int> item;
	set<vector<int>> empty;
	vector<set<vector<int>>> unit(h+1,empty);
	vector<vector<set<vector<int>>>> Grid(w+1,unit);
	int xi,yi,xi0,yi0,xi1,yi1;

	vector<int> temp(2,0);
    
	int xv=h*2, yv=w*2;
	if(!compartmentPos.empty())
	for(int i=0; i<compartmentPos.size()-1; i++)
	{
		if(compartmentPos[i].x()>-999.99 && _scene->_compartmentName[m_pathwayID][i][0]!="whole")
		{
			xi0=compartmentPos[i].x()*w-1, yi0=compartmentPos[i].y()*h-1;
			if(xi0<0) xi0=0; else if(xi0>=w) xi0=w;
			if(yi0<0) yi0=0; else if(yi0>=h) yi0=h;
				
			xi1=(compartmentPos[i].x()+compartmentPos[i].width())*w, yi1=(compartmentPos[i].y()+compartmentPos[i].height())*h;
			if(xi1<0) xi1=0; else if(xi1>=w) xi1=w;
			if(yi1<0) yi1=0; else if(yi1>=h) yi1=h;

			temp[0]='M'; temp[1]=i;

			for(int xi=xi0;xi<=xi1;xi++)
			for(int yi=yi0;yi<=yi1;yi++)
			{
			    Grid[xi][yi].insert(temp);			
			}
			visibleCompartmentNum++;			
		}        
	}
	
	for(int i=0; i<complexPos.size(); i++)
	{
		if(complexPos[i].x()>-999.99)
		{
			xi0=complexPos[i].x()*w-1, yi0=complexPos[i].y()*h-1;
			if(xi0<0) xi0=0; else if(xi0>=w) xi0=w;
			if(yi0<0) yi0=0; else if(yi0>=h) yi0=h;
				
			xi1=(complexPos[i].x()+complexPos[i].width()*2)*w+1, yi1=(complexPos[i].y()+complexPos[i].height())*h+1;
			if(xi1<0) xi1=0; else if(xi1>=w) xi1=w;
			if(yi1<0) yi1=0; else if(yi1>=h) yi1=h;

			temp[0]='C'; temp[1]=i;
			for(int xi=xi0;xi<=xi1;xi++)
			for(int yi=yi0;yi<=yi1;yi++)
			{
			    Grid[xi][yi].insert(temp);			
			}
			visibleItemNum++;
		}
	}
	for(int i=0; i<proteinPos.size(); i++)
	{
	  if(proteinPos[i].x()>-999.99)
	  {
			xi0=proteinPos[i].x()*w-1, yi0=proteinPos[i].y()*h-1;
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
			visibleItemNum++;
	  }
	}

	for(int i=0; i<physicalEntityPos.size(); i++)
	{
	  if(physicalEntityPos[i].x()>-999.99)
	  {
			xi0=physicalEntityPos[i].x()*w-1, yi0=physicalEntityPos[i].y()*h-1;
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
			visibleItemNum++;
	  }
	}

	for(int i=0; i<ANodePos.size(); i++)
	{
	  if(ANodePos[i].x()>-999.99)
	  {
			xi0=ANodePos[i].x()*w-1, yi0=ANodePos[i].y()*h-1;
			if(xi0<0) xi0=0; else if(xi0>=w) xi0=w;
			if(yi0<0) yi0=0; else if(yi0>=h) yi0=h;

			xi1=(ANodePos[i].x()+ANodePos[i].width())*w, yi1=(ANodePos[i].y()+ANodePos[i].height())*h;
			if(xi1<0) xi1=0; else if(xi1>=w) xi1=w;
			if(yi1<0) yi1=0; else if(yi1>=h) yi1=h;

			temp[0]='L'; temp[1]=i;

			for(int xi=xi0;xi<=xi1;xi++)
			for(int yi=yi0;yi<=yi1;yi++)
			{
			   Grid[xi][yi].insert(temp);				
			}
			visibleItemNum++;
	  }
	}
		
	for(int i=0; i<smallMoleculePos.size(); i++)
	{
		if(smallMoleculePos[i].x()>-999.99)
        {
			xi0=smallMoleculePos[i].x()*w-1, yi0=smallMoleculePos[i].y()*h-1;
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
			visibleItemNum++;
		}
	}
	
	for(int i=0; i<DnaPos.size(); i++)
	{
		if(DnaPos[i].x()>-999.99)
		{	
			xi0=DnaPos[i].x()*w-1, yi0=DnaPos[i].y()*h-1;
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
			visibleItemNum++;
		}
	    
	}
	
	for(int i=0; i<reactionPos.size(); i++)
	{
		if(reactionPos[i].x()>-999.99)
		{
			xi0=reactionPos[i].x()*w-1, yi0=reactionPos[i].y()*h-1;
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
			visibleItemNum++;
		}
	}	
	return Grid;
}

void PathBubble1::keyPressEvent(QKeyEvent *event)
{
    ItemBase::keyPressEvent(event);
	/*if(event->key()==Qt::Key_Control)
	{
		int keyname=event->key();
	    keyname=keyname;
	}
	int keyname=event->key();
	keyname=keyname;*/
    switch (event->key()) 
	{
		 case Qt::Key_Control: 
			 _scene->controlKeyisOn=true;
			 break;
		 //default: controlKeyisOn=false; break;
	}
}

void PathBubble1::keyReleaseEvent(QKeyEvent *event)
{
    ItemBase::keyReleaseEvent(event);
    switch (event->key()) 
	{
		 case Qt::Key_Control: 
			_scene->controlKeyisOn=false;
			 break;		 		
	}
}

void PathBubble1::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if(m_pathwayID<0) return;
	isInnerResizing=false;
	
	foundlist.clear();
	_mousePressed=1;

	QPoint tPos=QPoint(event->pos().x(),event->pos().y());
	if(getCalloutNoteRect().contains(tPos))
		return;
	QPointF spos = event->scenePos();
	if(_scene->_fileSelection!=NULL && _scene->_fileSelection->getDiaSceneRect().contains(event->scenePos()))
		return;

	ItemBase::mousePressEvent(event);

	if (event->button() == Qt::LeftButton )
	{	
		if(insideBubble(event->pos()))
	    {
			if(!groupSelected) 
			{   
				bool graphupdated=false;
				if(_grabisOn)
				{
					itemSelected = whichItem(event->pos(),this->Width()*_scale, this->Height()*_scale);
   					if(!itemSelected.empty()) 
					{   
						itemDragged.clear();
						setAItemDragged(itemSelected[0], itemSelected[1]);		
						bubbleRect.setWidth(0), bubbleRect.setHeight(0);
					}
					Qt::CursorShape shape = Qt::ClosedHandCursor;
					setCursor(shape);  
				}
				else if(_queryisOn)
				{
					/*itemSelected = whichItem(event->pos(),this->Width()*_scale, this->Height()*_scale);
   					if(!itemSelected.empty()) 
					{
					    setAItemDragged(itemSelected[0], itemSelected[1]);	
						bubbleRect.setWidth(0), bubbleRect.setHeight(0);
					}*/
					

					Qt::CursorShape shape = Qt::ClosedHandCursor;
					setCursor(shape);  
				}
				else if(_upisOn)
				{
					vector<int> etemp(3,0);
					vector< vector<int> > eventRecord;
				    vector<int> ColorSelected = whichItem(event->pos(),this->Width()*_scale, this->Height()*_scale);	
					if(!ColorSelected.empty()) 
					{	
						 setAItemColored(ColorSelected[0], ColorSelected[1], GO_UP);		
						 //pBubble->getGraphToBePaint();						
						 etemp[0]=getType();				
				         etemp[1]=m_pathwayID;	
				         etemp[2]=COLORED;
						 eventRecord.push_back(etemp);
						 _scene->storeEvent(eventRecord);
						 storeColored();
					}
				}
				else if(_downisOn)
				{
					vector<int> etemp(3,0);
					vector< vector<int> > eventRecord;
					vector<int> ColorSelected = whichItem(event->pos(),this->Width()*_scale, this->Height()*_scale);	
					if(!ColorSelected.empty()) 
					{
						 setAItemColored(ColorSelected[0], ColorSelected[1], _DOWN);	
						 //pBubble->getGraphToBePaint();							 
						 etemp[0]=getType();				
				         etemp[1]=m_pathwayID;	
				         etemp[2]=COLORED;
						 eventRecord.push_back(etemp);
						 _scene->storeEvent(eventRecord);
						 storeColored();
					}
				}
				else if(_evenisOn)
				{
					vector<int> etemp(3,0);
					vector< vector<int> > eventRecord;
					vector<int> ColorSelected = whichItem(event->pos(),this->Width()*_scale, this->Height()*_scale);	
					if(!ColorSelected.empty()) 
					{
					     setAItemColored(ColorSelected[0], ColorSelected[1], _EVEN);		
						 //pBubble->getGraphToBePaint();							 
						 etemp[0]=getType();				
				         etemp[1]=m_pathwayID;	
				         etemp[2]=COLORED;
						 eventRecord.push_back(etemp);
						 _scene->storeEvent(eventRecord);
						 storeColored();
					}
				}
				else if(_penisOn)
					initPos = event->pos();
				else 
				{
					vector<int> pre;//=itemSelected;
					for(set<vector<int>>::iterator it=highlighted.begin(); it!=highlighted.end(); it++)
	                {
		                vector<int> item = *it;
		                int type=item[0], id=item[1];		
						pre.push_back(type); pre.push_back(id);
					}
					itemSelected = whichItem(event->pos(),this->Width()*_scale, this->Height()*_scale);   
					if(!itemSelected.empty() && onNoteMark(m_pathwayID,  itemSelected[0], itemSelected[1], event->scenePos()))
					{
						vector<QString> text;
						vector<int> Cid;
						_scene->openNodeNote(m_pathwayID, itemSelected[0], itemSelected[1], this, event->scenePos(),text,Cid, true);
					}
					else
					{
						if(itemSelected.empty() && onTagArea( event->pos()))					
						{
							vector<int> twm;
							twm.push_back('W'); twm.push_back(m_pathwayID);
							if(!m_isResizing)
								itemSelected=twm;						
						}
						if(!itemSelected.empty())
						{
							int type=itemSelected[0], id=itemSelected[1];
							int resizeflag=0;
							if(type!='W')
							{
								QRectF rect;
								switch(type)
								{ 
									case 'C': rect=complexRect[id];
											  break;
									case 'E': rect=physicalEntityRect[id]; 
											  break;
									case 'P': rect=proteinRect[id]; 
											  break;
									case 'S': rect=smallMoleculeRect[id]; 
											  break;	
									case 'L': rect=ANodeRect[id]; 
											  break;	
									case 'D': rect=DnaRect[id]; 
											  break;	 	
									case 'R': rect=reactionRect[id]; 
											  break;	 	
									case 'M': rect=compartmentRect[id]; 
											  break;	
								}	
								resizeflag = isInResizeArea(event->pos(), rect);
							}
							if(resizeflag!=0)
							{
								isInnerResizing=true;
							}
							else if(!_scene->controlKeyisOn)
							{	
								_upisOn=_downisOn=_evenisOn=false;					
								itemDragged.clear();
								highlighted.clear();	

								if(highlighted.size()==0)
									 highlighted.insert(itemSelected);
								else
								{
									itemDragged.clear();
																								
									highlighted.insert(itemSelected);
								}
							}
							else
							{
							   if(highlighted.find(itemSelected)!=highlighted.end())
								 highlighted.erase(itemSelected);
							   else
								   highlighted.insert(itemSelected);
							}
						}
						else 
						{
						   if(!_scene->controlKeyisOn)
						   {
							   itemDragged.clear();
							   highlighted.clear();						   
						   }
						}
					}
					graphupdated=true;

					getPartUpdateSelectedNodetoBePaint(pre, itemSelected);
				}	
				if(!graphupdated)
				  getGraphToBePaint();
			}
			else //group seleted after pen drawing
			{
	        	if(_contain.containsPoint(event->pos(), Qt::OddEvenFill))
				{
					setItemDragged(itemSelected);
					//setEdgeAffected(itemSelected, _edgeSelected, _edgeAffected);
					getGraphToBePaint();
				}	
			}
         }
		 else  //on boundary
		 {

         }
	}	
	else 
	{
	   draggingItem=0;
	}

	if(onBoarder( event->pos() ))
		pressOnBoarder = true;
	else 
		pressOnBoarder = false;

	 if(onUnGroupArea(event->pos()) )
		pressOnUngroup = true;
	else 
		pressOnUngroup = false;
}

void PathBubble1::storeEvent(int eventType)
{
	/*bool flag=false;
	if(eventStored.empty())
	{
		flag=true;
	}		
	eventStored.push_back(eventType);
	*/
	if(eventStored.empty())
	{
		eventStored.push_back(-1);
	}
	eventStored.push_back(eventType);
	currentEventit=eventStored.end();
    currentEventit--;	

	/*switch(eventType)
	{
		case SEARCHSHARED: 
			storeShared();
			break;
		case SEARCHDIFFERED:
			storeDiffered();
			break;
		case SEARCHLINKED:
			storeLinked();
			break;
		case SEARCHEXPRESSED:
			storeExpressed();
			break;
		case COLORED:
			break;

		case NEWGRAPH:
			break;

		case HIGHLIGHTED:
			storeHighlighted();
			break;
	}*/
}

int PathBubble1::assignANodeName(QString Name, QString Name2, int Aid)
{
    int ID=-1,count=0;
	bool start=false;
    if(Aid>=0)
	    ID=Aid;
	else
	while(true)
	{
		if(Aid<0)
		{
			if(start)
				ID++;
			else
			{
 				for(int i=0; i<_scene->_ANodeName[m_pathwayID].size(); i++)
				{
				   if(ANodeRect.size()<=i)
   				   {
					   int size=ANodeRect.size();
					   for(int k=size;k<i+1;k++)
					   {
						   ANodeRect.push_back(QRectF(-1000,-1000,-1000,-1000));
					   }
				   }
				   if(ANodeRect[i].width()<=0)
				   {
					   ID=i;
					   count++;
					   break;
				   }	
			  
				}
				if( ID<0 || count==0)
				{
					ID = _scene->_ANodeName[m_pathwayID].size();
					vector<QString> A;					
					_scene->_ANodeName[m_pathwayID].push_back(A);
				}
			}
		}
		vector<QString> A;
		QString AName=Name2; //'A'+ QString::number(ID);		
		if(!_scene->CheckANodeIDinAllBubbles(AName))
		{
			A.push_back(AName);  A.push_back(Name);
			if( ID > _scene->_ANodeName[m_pathwayID].size()-1)
			{
				int start=_scene->_ANodeName[m_pathwayID].size();
				_scene->_ANodeName[m_pathwayID].resize(ID+1);
				for(int i=start-1; i<ID+1; i++)
                {	
					if(_scene->_ANodeName[m_pathwayID][i].empty())
			        {
						_scene->_ANodeName[m_pathwayID][i]=A;    			
						_scene->_ANodeName[m_pathwayID][i].resize(5);
					}
				}
			}
			else
			{
				_scene->_ANodeName[m_pathwayID][ID]=A;    			
				_scene->_ANodeName[m_pathwayID][ID].resize(5);
			}
			break;
		}	
		else 
			start=true;
	}
	return ID;
}

QRectF PathBubble1::getPos(int type, int id)
{
	QRectF rect=QRectF(-1000,-1000,-1000,-1000);
	switch(type)
	{
		case 'C': if(id<complexPos.size()) 
					  return(complexPos[id]); 
			      else return rect; 
		case 'E': if(id<physicalEntityPos.size()) 
					  return(physicalEntityPos[id]);
				  else return rect; 
		case 'L': if(id<ANodePos.size()) 
					  return(ANodePos[id]); 
			      else return rect; 
		case 'P': if(id<proteinPos.size()) 
					  return(proteinPos[id]); 
			      else return rect; 
		case 'S': if(id<smallMoleculePos.size()) 
					  return(smallMoleculePos[id]); 
			      else return rect; 
		case 'D': if(id<DnaPos.size()) 
					  return(DnaPos[id]);
			      else return rect; 
		case 'R': if(id<reactionPos.size()) 
					  return(reactionPos[id]);
			      else return rect; 
		case 'M': if(id<compartmentPos.size()) 
					  return(compartmentPos[id]);
			      else 
					  return rect; 
 	}
}


void PathBubble1::updateEdgetoNode(vector<vector<int>> edge)
{
	/*_complex.clear();
	_Dna.clear();
	_entity.clear();
	_protein.clear();
	_smallMolecule.clear();
	_Empty.clear();
	_ANode.clear();
	_reaction.clear();*/

	vector<vector<int>> empty;
	vector<vector<vector<int>>> temp11(_protein.size(), empty);
	_protein=temp11;

	vector<vector<vector<int>>> temp12(_complex.size(), empty);
	_complex=temp12;
	
	vector<vector<vector<int>>> temp13(_entity.size(), empty);
	_entity=temp13;

	vector<vector<vector<int>>> temp14(_smallMolecule.size()+1, empty);
	_smallMolecule=temp14;

	vector<vector<vector<int>>> temp15(_Dna.size(), empty);
	_Dna=temp15;

	vector<vector<vector<int>>> temp16(_reaction.size(), empty);
	_reaction=temp16;

	vector<vector<vector<int>>> temp17(_ANode.size(), empty);
	_ANode=temp17;


    vector<int> start(4,0), middle(4,0), end(4,0);
	vector<int> ed;
    /*for(int i=0; i<edge.size(); i++)
	{
         start[0]=edge[i][0], start[1]=edge[i][1];			 		 
		 middle[0]=edge[i][4]; middle[1]=edge[i][5]; 		 
		 end[0]=edge[i][2], end[1]=edge[i][3]; 
		 if(end[0]!='R')		 
	     {
			
			 ed=middle;  ed[2]='M'; ed[3]=1; 
			 if(start[1]>=0)
			 switch(start[0])
			 {
				 case 'C': _complex[start[1]].push_back(ed);   break;
				 case 'D': _Dna[start[1]].push_back(ed);       break;
				 case 'E': _entity[start[1]].push_back(ed);    break;
				 case 'P': _protein[start[1]].push_back(ed);         break;
				 case 'S': _smallMolecule[start[1]].push_back(ed);   break;		
				 case 'N': _Empty[start[1]].push_back(ed);   break;	
				 case 'L': _ANode[start[1]].push_back(ed);    break;
		     }			
			 ed=start;  ed[2]='M'; ed[3]=-1; 
			 if(middle[1]>=0)
			 {
				 if(middle[1]>=_reaction.size())
					 _reaction.resize(middle[1]+1);
				 _reaction[middle[1]].push_back(ed);  
			 }
						
			 ed=end; ed[2]='A'; ed[3]=1; 
			 if(middle[1]>=0)
			 {
                 if(middle[1]>=_reaction.size())
					 _reaction.resize(middle[1]+1);
				 _reaction[middle[1]].push_back(ed);       
			 }
		    
			 ed=middle;  ed[2]='A';  ed[3]=-1; 
			 if(end[1]>=0)
			 switch(end[0])
	         {
				case 'C': _complex[end[1]].push_back(ed);        break;
				case 'D': _Dna[end[1]].push_back(ed);            break;
				case 'E': _entity[end[1]].push_back(ed);         break;
				case 'P': _protein[end[1]].push_back(ed);        break;
				case 'S': _smallMolecule[end[1]].push_back(ed);  break;		
				case 'N': _Empty[end[1]].push_back(ed);          break;		
				case 'L': _ANode[end[1]].push_back(ed);          break;
		     }
		 }
		 else 
		 {
			
			 ed=end;  ed[2]='R'; ed[3]=1; 
			 if(start[1]>=0)
			 switch(start[0])
			 {
				 case 'C': _complex[start[1]].push_back(ed);   break;
				 case 'D': _Dna[start[1]].push_back(ed);       break;
				 case 'E': _entity[start[1]].push_back(ed);    break;
				 case 'P': _protein[start[1]].push_back(ed);         break;
				 case 'S': _smallMolecule[start[1]].push_back(ed);   break;		
				 case 'N': _Empty[start[1]].push_back(ed);   break;		
				 case 'L': _ANode[start[1]].push_back(ed);   break;	
		     }
			 ed=start; ed[2]='R'; ed[3]=-1; 
			 if(end[1]>=0)
			 {
				 if(end[1]>=_reaction.size())
					 _reaction.resize(end[1]+1);
				 _reaction[end[1]].push_back(ed);      			 
			 }
		 }
	}  */ 

	vector<int> temp(2,0);
	for(int i=0; i<edge.size(); i++)
	{			
		if(i==1089)
			i=i;
         start[0]=edge[i][0], start[1]=edge[i][1];			 		 
		 middle[0]=edge[i][4]; middle[1]=edge[i][5];
		 end[0]=edge[i][2], end[1]=edge[i][3]; 
		 if(end[0]!='R')		 
	     {
			 if(middle[1]>=0)
			 {
				 ed=start;  ed[2]='M'; ed[3]=-1; 
				 _reaction[middle[1]].push_back(ed);  
				 ed=end; ed[2]='A'; ed[3]=1; 
				 _reaction[middle[1]].push_back(ed);     
			 }
		 }
		 else 
		 {
			  end[1]=edge[i][5];
			  if(end[1]>=0)
			  {
				  ed=start; ed[2]='R'; ed[3]=-1; 			
				  _reaction[end[1]].push_back(ed);   
			  }
		 }
	}
	
	for(int i=0; i<edge.size(); i++)
	{		
	     start[0]=edge[i][0], start[1]=edge[i][1];	
		 middle[0]=edge[i][4]; middle[1]=edge[i][5]; 
		 end[0]=edge[i][2], end[1]=edge[i][3]; 
         if(end[0]!='R')// keqin comment out why drawSmallmolecule do with this? || drawSmallmolecule)		 
	     {
			 temp[0] = getVerticeID_2(start[0],start[1]);
			 temp[1] = getVerticeID_2(middle[0],middle[1]);		
		
			 //Vertex[temp[0]][0].push_back(Edge.size());
			 //Vertex[temp[1]][1].push_back(Edge.size());
			 //Edge.push_back(temp);				 

			 ed=middle;  ed[2]='M'; ed[3]=1; 

			 if(start[1]>=0)
			 switch(start[0])
			 {
		     case 'C':  _complex[start[1]].push_back(ed);   break;
			 case 'D': _Dna[start[1]].push_back(ed);       break;
			 case 'E': _entity[start[1]].push_back(ed);    break;
			 case 'P': _protein[start[1]].push_back(ed);         break;
			 case 'S': _smallMolecule[start[1]].push_back(ed);   break;		
			 case 'N': _Empty[start[1]].push_back(ed);   break;		
		     }			
			 ed=start;  ed[2]='M'; ed[3]=-1; 

			 //_reaction[middle[1]].push_back(ed);  
			
			 temp[0] = getVerticeID_2(middle[0],middle[1]);
			 temp[1] = getVerticeID_2(end[0],end[1]);

						 
			 //Vertex[temp[0]][0].push_back(Edge.size());
			 //Vertex[temp[1]][1].push_back(Edge.size());
			 //Edge.push_back(temp);				 

			 ed=end; ed[2]='A'; ed[3]=1; 
			
			 //_reaction[middle[1]].push_back(ed);       
		    
			 ed=middle;  ed[2]='A';  ed[3]=-1; 
			 if(end[1]>=0)
			 switch(end[0])
	         {
				case 'C': _complex[end[1]].push_back(ed);        break;
				case 'D': _Dna[end[1]].push_back(ed);            break;
				case 'E': _entity[end[1]].push_back(ed);         break;
				case 'P': _protein[end[1]].push_back(ed);        break;
				case 'S': _smallMolecule[end[1]].push_back(ed);  break;		
				case 'N': _Empty[end[1]].push_back(ed);           break;			
		     }
		 }
		 else 
		 {
			 temp[0] = getVerticeID_2(start[0],start[1]);
			 if(end[0]=='R')
			    end[1]=edge[i][5];

			 temp[1] = getVerticeID_2(end[0],end[1]);	
			
			 //Vertex[temp[0]][0].push_back(Edge.size()); //record edge
			// Vertex[temp[1]][1].push_back(Edge.size());

			// Edge.push_back(temp);	

			 ed=end;  ed[2]='R'; ed[3]=1; 
			 if(start[1]>=0)
			 switch(start[0])
			 {
		     case 'C': _complex[start[1]].push_back(ed);   break;
			 case 'D': _Dna[start[1]].push_back(ed);       break;
			 case 'E': _entity[start[1]].push_back(ed);    break;
			 case 'P': _protein[start[1]].push_back(ed);         break;
			 case 'S': _smallMolecule[start[1]].push_back(ed);   break;		
			 case 'N': _Empty[start[1]].push_back(ed);   break;		
		     }
			 ed=start; ed[2]='R'; ed[3]=-1; 			
			 //_reaction[end[1]].push_back(ed);      			 
		 }
	}	
}

bool PathBubble1::aggregation()
{
	//highlighted nodes		
	if(_penisOn)
	{
		if(_lines.size()!=0)
		    _penisOn=false;
		groupSelected=groupSelection(_lines);			
	}
	for(set<vector<int>>::iterator it=searched.begin(); it!=searched.end(); it++)
	{	
		 vector<int> item = *it;
	     highlighted.insert(*it);	
	}
	
	if(highlighted.size()==0)
		return false;


	set<vector<int>> highlightedT=highlighted;
	for(set<vector<int>>::iterator it=highlighted.begin(); it!=highlighted.end(); it++)
	{
		vector<int> item = *it;
		int type=item[0], id=item[1];		
		if(type=='M')
		{
		    compartmentToItemHighlighted(id, highlightedT);	
		}
	}
	highlighted = highlightedT;
	highlightedT.clear();

	//extend all Anode before aggregation them again/ prevent bugs 
	for(set<vector<int>>::iterator it=highlighted.begin(); it!=highlighted.end(); it++)
	{
		vector<int> item = *it;
		int type=item[0], id=item[1];		
		if(type=='L') //extend all Anode before aggregation them again/ prevent bugs 
	    {
             int aid=item[1]; 
			 for(int i=0; i<_scene->_ANodeContain[m_pathwayID][aid].size(); i=i+2)
			 {        
				 int type1 = _scene->_ANodeContain[m_pathwayID][aid][i], id1 = _scene->_ANodeContain[m_pathwayID][aid][i+1];	
				 item[0]=type1, item[1]=id1;
				 highlightedT.insert(item);
			 }
			 collapse(aid);
		 }	
		else 
			 highlightedT.insert(*it);	
	}
	highlighted = highlightedT;
	return (aggregation(highlighted));
}
	
bool PathBubble1::aggregation(set<vector<int>> highlighted, int Aid)	
{
	//if Aid>=0, use this ANode to replace the current Anode[Aid] 	
	if(highlighted.size()==0)
		return false;
	//_scene->_ANodeName.resize(_scene->_ANodeName.size()+1);item
	int ID;
	QRectF rect;
	if(Aid<0)
	{
		if(highlighted.size()==1)
		{
			//findNodesSameNames(highlighted);
			if(highlighted.size()==1)
			{
				QMessageBox msgBox;
				//msgBox.move(event->scenePos().x(), event->scenePos().y());
				msgBox.setWindowTitle("Trying to aggregate a single item");
				msgBox.setText("Please select more items.");
				msgBox.setStandardButtons(QMessageBox::Yes);
				if(msgBox.exec() == QMessageBox::Yes)
					return false;
			}
			return false;
		}
		if(ANodePos.size()==0 )//&& toplevel)
		{
			ANodePos.resize(ANodePos.size()+1);
			ANodePos_0.resize(ANodePos_0.size()+1);
			ANodePos_1.resize(ANodePos_1.size()+1);
		}
	
		//compute center
		QPointF center=QPointF(0,0),iPos;
		QRectF iRect;
		int count=0;
		float maxw=-1000000, maxh=-1000000, maxw_M=-1000000, maxh_M=-1000000;
		for(set<vector<int>>::iterator it=highlighted.begin(); it!=highlighted.end(); it++)
		{
			vector<int> item = *it;
			int type=item[0], id=item[1];
			switch(type)
			{ 
				case 'C': iRect=complexPos[id];        iPos=complexPos[id].center(); break;
				case 'E': iRect=physicalEntityPos[id]; iPos=physicalEntityPos[id].center(); break;
				case 'L': iRect=ANodePos[id];          iPos=ANodePos[id].center(); break;
				case 'P': iRect=proteinPos[id];        iPos=proteinPos[id].center(); break;
				case 'S': iRect=smallMoleculePos[id];  iPos=smallMoleculePos[id].center(); break;		
				case 'D': iRect=DnaPos[id];        iPos=DnaPos[id].center(); break;	 				     	
				case 'N': iRect=EmptyPos[id];      iPos=EmptyPos[id].center(); break;	
				case 'R': iRect=reactionPos[id];   iPos=reactionPos[id].center(); break;
				case 'M': iRect=compartmentPos[id];  iPos=compartmentPos[id].center(); break;
			}
			if(iRect.x()>-1990)
			{
				if(type=='M')
				{
					QPointF center = iRect.center();
					float width = iRect.width()/4, height = iRect.height()/4;
					iRect = QRectF(center.x()-width,center.y()-height, width*2, height*2);
					if(maxw_M<iRect.width())
						maxw_M=iRect.width();

					if(maxh_M<iRect.height()*1.1)
						maxh_M=iRect.height()*1.1;		
				}			
				else
				{	if(maxw<iRect.width())
					   maxw=iRect.width();

					if(maxh<iRect.height()*1.1)
					   maxh=iRect.height()*1.1;				
				}
			
				count++;
				center=center+iPos;
			}	
		}
		if(count>0)
		{
			center.setX(center.x()/count);  center.setY(center.y()/count);
		}
	
		if(proteinRect.size()>1)
		{
			for(int i=0;i<proteinRect.size();i++)
			{		
				if(proteinRect[i].width()>0)
				{
					if(proteinPos[i].width()>maxw)
						maxw=proteinPos[i].width();
					if(proteinPos[i].height()>maxh)
						maxh=proteinPos[i].height()*1.1;				
					break;
				}
			}	
			for(int i=0;i<ANodeRect.size();i++)
			{		
				if(ANodeRect[i].width()>0)
				{
					if(ANodePos[i].width()>maxw)
						maxw=ANodePos[i].width();
					if(ANodePos[i].height()>maxh)
						maxh=ANodePos[i].height()*1.1;
					break;
				}
			}	
		}
		if(maxw<0 && maxw_M>0)
		{
			maxw=maxw_M;
			maxh=maxh_M;
		}
		rect= QRectF(0, 0, maxw, maxh);	
		rect.moveCenter(QPointF(center.x(), center.y()));

		//find less crowded area;
		rect = findSparseLocation(rect);
	 } 
	
		set<QString> nameSet, cNameSet; //cNameSet for compartment
		for(set<vector<int>>::iterator it=highlighted.begin(); it!=highlighted.end(); it++)
		{
			vector<int> item= *it;
			int type=item[0], id=item[1];
			QString end;
			switch(type)
			{
				case 'C': nameSet.insert(_scene->_complexNameD[dataID][id][0]);  break;
				case 'E': nameSet.insert(_scene->_physicalEntityNameD[dataID][id][0]);  break;
				case 'P': nameSet.insert(_scene->_proteinNameD[dataID][id][0]);  break;
				case 'S': nameSet.insert(_scene->_smallMoleculeNameD[dataID][id][0]);  break;
				case 'D': nameSet.insert(_scene->_DnaNameD[dataID][id][0]);  break;
				case 'R': nameSet.insert(_scene->_reactionNameD[dataID][id][1]);  break;
				case 'M': cNameSet.insert(_scene->_compartmentName[m_pathwayID][id][0]);  break;
				case 'L': nameSet.insert(_scene->_ANodeName[m_pathwayID][id][0]);  break;
			}			
		}
		
		QString Name, Name2;
		Name2="A_";
		int count1=0, count2=0;
		for(set<QString>::iterator it=cNameSet.begin(); it!=cNameSet.end(); it++)
		{
			QString head=*it; head=head.mid(0,1);
			if(count2<3)
			   Name2 = Name2 + head;			
			else if(count2==3)
			   Name2 = Name2 + '...';
			count2++;

			Name = Name + *it;			
			if(++count1!=(cNameSet.size()+nameSet.size()) )
				Name = Name + ',';
		}
		for(set<QString>::iterator it=nameSet.begin(); it!=nameSet.end(); it++)
		{
			QString head=*it; head=head.mid(0,1);
			if(count2<3)
			   Name2 = Name2 + head;			
			else if(count2==3)
			   Name2 = Name2 + '...';
			count2++;

			Name = Name + *it;			
			if(++count1!=(cNameSet.size()+nameSet.size()) )
				Name = Name + ',';
		}
	
		ID = assignANodeName(Name, Name2, Aid);	
		if(_scene->_ANodePos[m_pathwayID].size()>Aid)
		{
		   rect=_scene->_ANodePos[m_pathwayID][Aid];
		}
		else if(ANodePos.size()>Aid)
			rect=ANodePos[Aid];


		if(ID==ANodePos.size())
		{
			ANodePos.push_back(rect);
			ANodePos_0.push_back(rect);
			ANodePos_1.push_back(rect);
		}
		else if(ID>ANodePos.size())
		{
			int start=ANodePos.size();
			ANodePos.resize(ID+1);
			ANodePos_0.resize(ID+1);
			ANodePos_1.resize(ID+1);	
			for(int i=start; i<ID; i++)
			{
				QRectF eRect(-1000,-1000,-1000,-1000);
				ANodePos[i]=eRect;
				ANodePos_0[i]=eRect;
				ANodePos_1[i]=eRect;		
			}
			ANodePos[ID]=rect;
			ANodePos_0[ID]=rect;
			ANodePos_1[ID]=rect;
		}
		else
		{
			ANodePos[ID]=rect;
			ANodePos_0[ID]=rect;
			ANodePos_1[ID]=rect;	
		}
		_ANode.resize(ANodePos.size());
		////////////////////
			
		//_scene->backupItemPos(m_pathwayID, complexPos, proteinPos, smallMoleculePos, DnaPos, EmptyPos, reactionPos, physicalEntityPos, compartmentPos, ANodePos, _scale, dCenter, drawSmallmolecule);				
		//backupItemPos backup ANodes
		_scene->_ANodePos.resize(_scene->_proteinPos.size());
		_scene->_ANodePos[m_pathwayID].resize(ANodePos.size());
		_scene->_ANodePos[m_pathwayID][ID] = ANodePos[ID];
		
	vector<int> start(4,0), middle(4,0), end(4,0);
	vector<int> ed;
		
	//update edge
	vector<int> temp(2,0),etemp(3,0);	
	vector<vector<int>> tedge(2,etemp);

	itemGrid=recordItemGrid(visibleItemNum, visibleCompartmentNum);
	
	_scene->_ANodeContain[m_pathwayID].resize(_ANode.size());

	_scene->_ANodePosContain.resize(_scene->_proteinPos.size());
	_scene->_ANodePosContain[m_pathwayID].resize(_ANode.size());
		
	_scene->_ANodePosContain[m_pathwayID][ID].clear();
	_scene->_ANodeContain[m_pathwayID][ID].clear();
	for(int i=0; i<edge.size(); i++)
	{	
		int type=edge[i][0], id=edge[i][1];	
		vector<int> edge_t=edge[i];
		vector<int> temp(2,0);
  		temp[0]=type, temp[1]=id;
		bool flag=false;
		if(highlighted.find(temp)!=highlighted.end())                                               
		{
			edge[i][0]='L', edge[i][1]=ID;	   
			_scene->_ANodeContain[m_pathwayID][ID].push_back(type);
		    _scene->_ANodeContain[m_pathwayID][ID].push_back(id);
			_scene->_ANodePosContain[m_pathwayID][ID].push_back(getPos(type,id));
			flag=true;
		}	
		if(edge[i][2]=='R' && edge[i][4]==edge[i][2]) //edge[i][4] always be 'R'
		{
			bool flag1=false;
			type=edge[i][4]; id=edge[i][5];
			temp[0]=type, temp[1]=id;		
			if(highlighted.find(temp)!=highlighted.end())
			{
				edge[i][4]='L', edge[i][5]=ID;	
				_scene->_ANodeContain[m_pathwayID][ID].push_back(type);
				_scene->_ANodeContain[m_pathwayID][ID].push_back(id);
				_scene->_ANodePosContain[m_pathwayID][ID].push_back(getPos(type,id));			
			}			
		}
		else
		{
			type=edge[i][2], id=edge[i][3];	
			temp[0]=type, temp[1]=id;
			if(highlighted.find(temp)!=highlighted.end())
			{
				edge[i][2]='L', edge[i][3]=ID;	
				_scene->_ANodeContain[m_pathwayID][ID].push_back(type);
				_scene->_ANodeContain[m_pathwayID][ID].push_back(id);
				_scene->_ANodePosContain[m_pathwayID][ID].push_back(getPos(type,id));
			}
			type=edge[i][4]; id=edge[i][5];
			temp[0]=type, temp[1]=id;		
			if(highlighted.find(temp)!=highlighted.end())
			{
				edge[i][4]='L', edge[i][5]=ID;	
				_scene->_ANodeContain[m_pathwayID][ID].push_back(type);
				_scene->_ANodeContain[m_pathwayID][ID].push_back(id);
				_scene->_ANodePosContain[m_pathwayID][ID].push_back(getPos(type,id));			
			}
		}		
	}
	for(int i=0; i<edge.size(); i++)
	{
		 start[0]=edge[i][0], start[1]=edge[i][1];			 		 
		 middle[0]=edge[i][4]; middle[1]=edge[i][5]; 		 
		 end[0]=edge[i][2], end[1]=edge[i][3]; 
		 if(end[0]!='R')// || drawSmallmolecule)		 
	     {
			 ed=middle;  ed[2]='M'; //ed[3]=1; 
			 if(start[0]=='L')
			 {			 
				 _ANode[start[1]].push_back(ed);   
		     }		
			 ed=middle;  ed[2]='A';  //ed[3]=-1; 
			 if(end[0]=='L')
	         {
				_ANode[end[1]].push_back(ed);          		
		     }
		 }
		 else 
		 {
			 ed=middle;  ed[2]='R'; //ed[3]=1; 
			 if(start[0]=='L')
			 {
		         _ANode[start[1]].push_back(ed);   
		     }			 
		 }
	}
	for(set<vector<int>>::iterator it=highlighted.begin(); it!=highlighted.end(); it++)
	{
        vector<int> item= *it;
	    int type=item[0], id=item[1];
		_scene->_ANodeContain[m_pathwayID][ID].push_back(type);
		_scene->_ANodeContain[m_pathwayID][ID].push_back(id);

		QRectF tRect = getPos(type,id);
		tRect.moveCenter( tRect.center() - rect.center() );
		_scene->_ANodePosContain[m_pathwayID][ID].push_back(tRect);
	}
	//make all others invisible
	for(set<vector<int>>::iterator it=highlighted.begin(); it!=highlighted.end(); it++)
	{
        vector<int> item= *it;
	    int type=item[0], id=item[1];
		switch(type)
		{
			case 'C': complexPos[id]=QRectF(-2000,-2000,complexPos[id].width(),complexPos[id].height()); break;
			case 'E': physicalEntityPos[id]=QRectF(-2000,-2000, physicalEntityPos[id].width(), physicalEntityPos[id].height()); break;
		    case 'P': proteinPos[id]=QRectF(-2000,-2000,proteinPos[id].width(),proteinPos[id].height()); break;
		    case 'S': smallMoleculePos[id]=QRectF(-2000,-2000,smallMoleculePos[id].width(),smallMoleculePos[id].height()); break;
		    case 'D': DnaPos[id]=QRectF(-2000,-2000,DnaPos[id].width(),DnaPos[id].height());           break;
			case 'R': reactionPos[id]=QRectF(-2000,-2000,reactionPos[id].width(),reactionPos[id].height());      break;
			case 'M': compartmentPos[id]=QRectF(-2000,-2000,compartmentPos[id].width(),compartmentPos[id].height());   break;
			case 'L': ANodePos[id]=QRectF(-2000,-2000, ANodePos[id].width(), ANodePos[id].height());   break;
		}			 					 
	}
	int cid=whichCompartment(highlighted);
	if(cid>=0)
	{
		vector<int> item;
		item.push_back('L'); item.push_back(ID);
		CompartmentContain[cid].insert(item);
		_scene->CompartmentContain[m_pathwayID][cid].insert(item);
	}
	highlighted.clear();
	ANodeNum=ANodePos.size()-1;
	_scene->_ANodeName[m_pathwayID].resize(ANodePos.size());

	int pid1=-1;
	if(_pathBubbleParent != NULL)
		pid1=_pathBubbleParent->m_pathwayID;	

	updateSpanEdgesAfterAggregation(ID);
	//add
	getGraphToBePaint();	
	return true;
}

bool PathBubble1::myErrorMessage(QString text)
{
	QMessageBox msgBox; msgBox.setText(text); msgBox.exec();
    return false;
}

bool PathBubble1::isItemVisible(int type, int id)
{
	if(id<0)
		return false;

    switch(type)
	{
		case 'C': if(id>complexPos.size()) myErrorMessage("error in PathBubble1::isItemVisible(int type, int id)");        return complexPos[id].x()>-0.99;  break;
		case 'D': if(id>DnaPos.size()) myErrorMessage("error in PathBubble1::isItemVisible(int type, int id)");            return DnaPos[id].x()>-0.99; break;
		case 'E': if(id>physicalEntityPos.size()) myErrorMessage("error in PathBubble1::isItemVisible(int type, int id)"); return physicalEntityPos[id].x()>-0.99; break;
		case 'P': if(id>proteinPos.size()) myErrorMessage("error in PathBubble1::isItemVisible(int type, int id)");        return proteinPos[id].x()>-0.99;  break;
		case 'S': if(id>smallMoleculePos.size()) myErrorMessage("error in PathBubble1::isItemVisible(int type, int id)");  return smallMoleculePos[id].x()>-0.99; break;
		case 'R': if(id>reactionPos.size()) myErrorMessage("error in PathBubble1::isItemVisible(int type, int id)");       return reactionPos[id].x()>-0.99;   break; 
		case 'L': if(id>ANodePos.size()) myErrorMessage("error in PathBubble1::isItemVisible(int type, int id)");          return ANodePos[id].x()>-0.99;   break; 
	}
	return false;
}	

void PathBubble1::updateSpanEdgesWhenCollapse(int Aid)
{
	//recover the original edge related to Aid
	int aid1=-1;
	int pid1=-1;
	vector<vector<vector<int>>> edge1;
	if(_pathBubbleParent != NULL)
	{
		pid1=_pathBubbleParent->m_pathwayID;		
		//aid1=getOriNodeID(_pathBubbleParent->m_pathwayID, m_pathwayID, 'L', Aid);
		//if(aid1 != -1)
		{
			vector<vector<int>> tEdge = _pathBubbleParent->edge;		
			for(int i=0; i<_scene->_ANodeContain[m_pathwayID][Aid].size(); i=i+2)
			{        
					int type = _scene->_ANodeContain[m_pathwayID][Aid][i], id = _scene->_ANodeContain[m_pathwayID][Aid][i+1];		
					aid1=getOriNodeID(_pathBubbleParent->m_pathwayID, m_pathwayID, type, id);
					//bool flag=false;
					for(int j=0; j < tEdge.size(); j++)
					{	
						int mtype=tEdge[j][4], mid=tEdge[j][5];
						//if(mtype, mid is 
						if(_pathBubbleParent->isItemVisible(mtype, mid))
						{
							for(int k=0; k<4; k=k+2)
							{
								int type1=tEdge[j][k], id1=tEdge[j][k+1];	
  								if(type1==type && id1==aid1)
								{
									//tEdge[j][k]=type, tEdge[j][k+1]=id1;	
									vector<vector<int>> edge;
									vector<int> n1(3,0), n2(3,0); 
									if(k==2 && (type1!=mtype || type1!='R') )
									{
										n1[0]=pid1, n1[1]=type, n1[2]=id1;
										n2[0]=pid1, n2[1]=mtype, n2[2]=mid;
									
									}
									else if(k==0)
									{
										n2[0]=pid1, n2[1]=type, n2[2]=id1;
										n1[0]=pid1, n1[1]=mtype, n1[2]=mid;
										
									}									
									edge.push_back(n1), edge.push_back(n2);
									edge1.push_back(edge);
								}
							}
						}
						//if(flag)
						//	break;
					}
			}
		}
	}
	//set<vector<vector<int>>> edgeconnections;
	//vector<vector<int>> _ANodeContain;
	reCoverEdgeConnection(Aid, pid1, edge1, _scene->_ANodeContain[m_pathwayID], _scene->edgeconnections);
	//fix
	reCoverEdgeConnection(Aid, pid1, edge1, _scene->_ANodeContain[m_pathwayID], _scene->edgeconnections_A);
	reCoverEdgeConnection(Aid, pid1, edge1, _scene->_ANodeContain[m_pathwayID], _scene->edgeconnections_I);
	reCoverEdgeConnection(Aid, pid1, edge1, _scene->_ANodeContain[m_pathwayID], _scene->edgeconnections_4, false, true);
	//reCoverEdgeConnection(_scene->_ANodeContain[m_pathwayID]);	
}


void PathBubble1::reCoverEdgeConnection(int Aid, int pid1, vector<vector<vector<int>>> edge1, vector<vector<int>> _ANodeContain, set<vector<vector<int>>> &edgeconnections, bool reverse, bool noInsert)
{

	set<vector<vector<int>>> edgeC=edgeconnections;
	for(int i=0,j=0; i<_ANodeContain[Aid].size(); i=i+2,j=j+1)
	{        
		bool flag;
	    int type = _ANodeContain[Aid][i], id = _ANodeContain[Aid][i+1];
		//update connections span pathways
		if(pid1>=0)
		{
			vector<int> link1(3,0), link2(3,0), link3(3,0);
			
			link1[0] = pid1, link1[1] = type, link1[2] = getOriNodeID(pid1, m_pathwayID, type, id);
			
			link3[0] = m_pathwayID, link3[1] = type, link3[2] = id;

			link2[0] = m_pathwayID, link2[1] = 'L', link2[2] = Aid;
			vector<vector<vector<int>>> linkRecord;
			set<vector<vector<int>>>::iterator it;	
			for(it = edgeconnections.begin(); it != edgeconnections.end(); it++) //update spanning edge
			{
				vector<vector<int>> plink = *it;		
				if(plink[0]==link2 || plink[1]==link2)
				{
					linkRecord.push_back(plink);				 
				}			
			}
			for(int m=0; m<linkRecord.size(); m++)
			{
				  vector<vector<int>> plink, nlink, Link;
				  plink = nlink = Link = linkRecord[m];
				  if(plink[0]==link2)
				  {
					 nlink[0]=link1;
					 Link[0]=link3;
				  }
				  else if(plink[1]==link2)
				  {
					 nlink[1]=link1;
					 Link[1]=link3;
				  }
				  //check if nlink exists in original pathway              
				  if(isLinkinOriginalPathway(nlink, edge1, reverse))
				  {
					 edgeC.erase(plink);			
					 if(!noInsert)
					    edgeC.insert(Link);
				  }
			}			
		}
	}
	edgeconnections=edgeC;
}

void PathBubble1::updateSpanEdgesAfterAggregation(int ID)
{	
	//search edge set involving the aggregated nodes in parentpathway
	vector<vector<vector<int>>> edge1;
	int pid1=-1;
	if(_pathBubbleParent == NULL)
		return;

	pid1=_pathBubbleParent->m_pathwayID;		
	/*for(int i=0,j=0; i<_scene->_ANodeContain[m_pathwayID][ID].size(); i=i+2,j=j+1)
	{
		int type = _scene->_ANodeContain[m_pathwayID][ID][i], Cid = _scene->_ANodeContain[m_pathwayID][ID][i+1];
		
		int cid1=getOriNodeID(_pathBubbleParent->m_pathwayID, m_pathwayID, type, Cid);
		if(cid1 != -1)
		{
			vector<vector<int>> tEdge = _pathBubbleParent->edge;		
			for(int i=0; i<_scene->_ANodeContain[pid1][cid1].size(); i=i+2)
			{        
					int type = _scene->_ANodeContain[pid1][cid1][i], id = _scene->_ANodeContain[pid1][cid1][i+1];		
					bool flag=false;
					for(int j=0; j < tEdge.size(); j++)
					{	
						int mtype=tEdge[j][4], mid=tEdge[j][5];
						for(int k=0; k<6; k=k+2)
						{
							int type1=tEdge[j][k], id1=tEdge[j][k+1];	
  							if(type1==type && id1==cid1)
							{
								tEdge[j][k]=type, tEdge[j][k+1]=id;	
								vector<vector<int>> edge;
								vector<int> n1(3,0), n2(3,0); 
								if(k==2)
								{
									n1[0]=pid1, n1[1]=type, n1[2]=id;
									n2[0]=pid1, n2[1]=mtype, n2[2]=mid;
								}
								else if(k==0)
								{
									n2[0]=pid1, n2[1]=type, n2[2]=id;
									n1[0]=pid1, n1[1]=mtype, n1[2]=mid;
								}
								edge.push_back(n1), edge.push_back(n2);
								edge1.push_back(edge);
								flag=true;
								break;
							}
						}
						if(flag)
							break;
					}
			}
		}		
	}*/

	for(int i=0,j=0; i<_scene->_ANodeContain[m_pathwayID][ID].size(); i=i+2,j=j+1)
	{        
		bool flag;
	    int type = _scene->_ANodeContain[m_pathwayID][ID][i], id = _scene->_ANodeContain[m_pathwayID][ID][i+1];
		
		//update connections span pathways
		if(pid1>=0)
		{
			vector<int> link1(3,0), link2(3,0), link3(3,0);			
			link1[0] = pid1, link1[1] = type, link1[2] = getOriNodeID(pid1, m_pathwayID, type, id);			
			link3[0] = m_pathwayID, link3[1] = type, link3[2] = id;
			link2[0] = m_pathwayID, link2[1] = 'L', link2[2] = ID;
			
			set<vector<vector<int>>>::iterator it;
			set<vector<vector<int>>> connections;
			for(int i=0; i<4; i++)
			{
				vector<vector<vector<int>>> linkRecord;	
				switch(i)
				{
					case 0: connections=_scene->edgeconnections;  break;
					case 1: connections=_scene->edgeconnections_A;  break;
					case 2: connections=_scene->edgeconnections_I;  break;
					case 3: connections=_scene->edgeconnections_4;  break;
				}
				set<vector<vector<int>>> tConnections=connections;
				for(it = tConnections.begin(); it != tConnections.end(); it++)
				//for(it = _scene->edgeconnections.begin(); it != _scene->edgeconnections.end(); it++)//update spanning edge
				{
					vector<vector<int>> plink = *it;		
					if(plink[0]==link3 || plink[1]==link3)
					{
						linkRecord.push_back(plink);				 
					}			
				}
		
				for(int m=0; m<linkRecord.size(); m++)
				{
					 vector<vector<int>> plink, olink, Link;
					 plink = olink = Link = linkRecord[m];
					  if(plink[0]==link3)
					  {
						 olink[0]=link1;
						 Link[0]=link2;
					  }
					  else if(plink[1]==link3)
					  {
						 olink[1]=link1;
						 Link[1]=link2;
					  }
					  //check if nlink exists in original pathway              
					  //if(isLinkinOriginalPathway(olink, edge1))
					  {
						 //_scene->edgeconnections.erase(plink);	
						 tConnections.erase(plink);	
						 //_scene->edgeconnections.insert(Link);
						 tConnections.insert(Link);
					  }
				}
				switch(i)
				{
					case 0: _scene->edgeconnections=tConnections;  break;
					case 1: _scene->edgeconnections_A=tConnections;  break;
					case 2: _scene->edgeconnections_I=tConnections;  break;
					case 3: _scene->edgeconnections_4=tConnections;  break;
				}
			}
		}
	}

	for(set<vector<vector<int> > >::iterator it=_scene->edgeconnections.begin(); it!=_scene->edgeconnections.end(); it++)
	{
		vector<vector<int> > edge=*it,redge;
		redge.push_back(edge[1]); redge.push_back(edge[0]);
		if( (_scene->edgeconnections_4.find(edge)==_scene->edgeconnections_4.end()&& _scene->edgeconnections_4.find(redge)==_scene->edgeconnections_4.end()) && (_scene->edgeconnections_A.find(edge)!= _scene->edgeconnections_A.end() || _scene->edgeconnections_I.find(edge)!= _scene->edgeconnections_I.end() || _scene->edgeconnections_A.find(redge)!= _scene->edgeconnections_A.end() || _scene->edgeconnections_I.find(redge)!= _scene->edgeconnections_I.end() || _scene->edgeconnections.find(redge)!= _scene->edgeconnections.end()))
		{
			//updatedEdge_5.insert(*it);
			_scene->edgeconnections_4.insert(edge);
		}	
	}

	_scene->edgeconnections_5.clear();
	for(set<vector<vector<int> > >::iterator it=_scene->edgeconnections_4.begin(); it!=_scene->edgeconnections_4.end(); it++)
	{
		vector<vector<int> > edge=*it, redge;
		redge.push_back(edge[1]);  redge.push_back(edge[0]);
	    int encode=0;
		if( _scene->edgeconnections.find(edge)!= _scene->edgeconnections.end() )
		{
		    encode|=1;
		}
		if( _scene->edgeconnections.find(redge)!= _scene->edgeconnections.end() )
		{
		    encode|=2;
		}
		if( _scene->edgeconnections_A.find(edge)!= _scene->edgeconnections_A.end() )
		{
		    encode|=4; 
		}
		if( _scene->edgeconnections_A.find(redge)!= _scene->edgeconnections_A.end() )
		{
		    encode|=8;
		}
		if( _scene->edgeconnections_I.find(edge)!= _scene->edgeconnections_I.end() ) 
		{
		    encode|=16; 		
		}	
		if( _scene->edgeconnections_I.find(redge)!= _scene->edgeconnections_I.end() )
		{
		    encode|=32; 
		}
		_scene->edgeconnections_5.push_back(encode);
	}
}


void PathBubble1::findNodesSameNames(set<vector<int>> &searched)
{
		if(searched.size()<1)
			return;
		vector<int> temp=*searched.begin();
		if(temp.size()<2)
			return;

		QString str;	
		int id=temp[1];
		switch(temp[0])
		{ 
			case 'C': str=_scene->_complexNameD[dataID][id][0];   break;
			case 'E': str=_scene->_physicalEntityNameD[dataID][id][0];   break;
			case 'P': str=_scene->_proteinNameD[dataID][id][0];   break;
			case 'S': str=_scene->_smallMoleculeNameD[dataID][id][0];   break;		
			case 'D': str=_scene->_DnaNameD[dataID][id][0];           break; 				     	
			case 'L': str=_scene->_ANodeName[m_pathwayID][id][0];   break;			
			case 'R': str=_scene->_reactionNameD[dataID][id][0];   break;			
		}

	    //set<vector<int>> searched;
	    for (int i = 0; i < _scene->_compartmentName[m_pathwayID].size(); i ++)
		{				
			if(_scene->_compartmentName[m_pathwayID][i][0]==str)
			{
				vector<int> item;
				item.push_back('M'); item.push_back(i);
				searched.insert(item);	
			}
		}
		int Did=dataID,Pid=m_pathwayID;						
		for (int j = 0; j < _scene->_complexNameD[Did].size(); j ++)
		{
			if(_scene->_complexNameD[Did][j][0]==str)
			{
				vector<int> item;
				item.push_back('C'); item.push_back(j);
				searched.insert(item);			
			}
		}								
		for (int j = 0; j < _scene->_physicalEntityNameD[Did].size(); j++)
		{
			for (int k = 0; k < _scene->_physicalEntityNameD[Did][j].size(); k++)
			{
				if(_scene->_physicalEntityNameD[Did][j][0]==str)
				{
					vector<int> item;
					item.push_back('E'); item.push_back(j);
					searched.insert(item);
				}
			}
		}			
		for (int j = 0; j < _scene->_ANodeName[Pid].size(); j++)
		{
			for (int k = 0; k < _scene->_ANodeName[Pid][j].size(); k++)
			{
				if(_scene->_ANodeName[Pid][j][0]==str)
				{
					vector<int> item;
					item.push_back('L'); item.push_back(j);
					searched.insert(item);
				}
			}
		}			
		for (int j = 0; j < _scene->_proteinNameD[Did].size(); j ++)
		{
			for (int k = 0; k < _scene->_proteinNameD[Did][j].size(); k ++)
			{
				if(_scene->_proteinNameD[Did][j][0]==str)
				{
					vector<int> item;
					item.push_back('P'); item.push_back(j);
					searched.insert(item);
				}
			}
		}									
		for (int j = 0; j < _scene->_reactionNameD[Did].size(); j ++)
		{
			for (int k = 0; k < _scene->_reactionNameD[Did][j].size(); k ++)
			{
				if(_scene->_reactionNameD[Did][j][0]==str)
				{
					vector<int> item;
					item.push_back('R'); item.push_back(j);
					searched.insert(item);
				}
			}
		}								
		for (int j = 0; j < _scene->_smallMoleculeNameD[Did].size(); j ++)
		{
			for (int k = 0; k < _scene->_smallMoleculeNameD[Did][j].size(); k ++)
			{
				if(_scene->_smallMoleculeNameD[Did][j][0]==str)
				{
					vector<int> item;
					item.push_back('S'); item.push_back(j);
					searched.insert(item);
				}
			}
		}
		for (int j = 0; j < _scene->_DnaNameD[Did].size(); j ++)
		{
			for (int k = 0; k < _scene->_DnaNameD[Did][j].size(); k ++)
			{
				if(_scene->_DnaNameD[Did][j][0]==str)
				{
					vector<int> item;
					item.push_back('D'); item.push_back(j);
					searched.insert(item);
				}
			}
		}	
		
}

float PathBubble1::findOverlap(float x, float y, QRectF rect, vector<int> item)
{
	    QRectF rect0;
		float w = rect.width(),h = rect.height(); 	
	    //vector<vector<int>> item = whichItem( getrecttoRect(QRectF(x-w/2,y-h/2,w,h), 'L', fixedSize, dCenter, _scale), this->Width()*_scale, this->Height()*_scale);	 //whichItem( getPostoRect(QPointF(x,y), fixedSize, dCenter, _scale), this->Width()*_scale, this->Height()*_scale);	
		if(!item.empty() && item[0]!='W' && item[0]!='M' && highlighted.find(item)==highlighted.end() )
	    {
			int type=item[0], id=item[1];
			QRectF rect1;
			if(type!='W' && type!='M')
			{
				switch(type)
				{ 
					case 'C': rect1=complexPos[id];
						      rect1=QRect(rect1.x(), rect1.y(), rect1.width()*2, rect.height());
							  break;
					case 'E': rect1=physicalEntityPos[id]; 
								 break;
					case 'L': rect1=ANodePos[id]; 
								 break;
					case 'P': rect1=proteinPos[id]; 
								 break;
					case 'S': rect1=smallMoleculePos[id]; 
								 break;	
					case 'D': rect1=DnaPos[id]; 
								 break;	 	
					case 'R': rect1=reactionPos[id]; 
								 break;	 					
				}
			}
			rect0=QRectF(x-w/2,y-h/2,w, h);
			if(rect1.intersects(rect0))
			{
				QRectF irect= rect1.intersect(rect0);
				float area =  irect.width() * irect.height() ;
				return area;
			}
			else
			{
				return 0;	
			}
			
		}
		else if(!item.empty() && (item[0]=='W' || item[0]=='M') && highlighted.find(item)==highlighted.end())
		{
			return 0;
		}
		else if(item.empty())
		{
			return 0;			
		}
		return 0;
}


QRectF PathBubble1::findSparseLocation(QRectF rect)
{
	float x0, x1, y0, y1, cx, cy, itvx, itvy;
	cx = rect.center().x();  cy = rect.center().y();
	float w = rect.width(),h = rect.height(); 
	x0 = cx-w*2;  y0 = cy-h*2;
	x1 = cx+w*2;  y1 = cy+h*2;
	itvx=w/2, itvy=h/2;
	float aMin=1000000,dMin=1000000;
	float minx=-1000,miny=-1000;
	int mid,mtype;
	mid=mtype=-1;
	QRectF rect0;

	vector<vector<int>> items = whichItem( getrecttoRect(QRectF(cx-w/2,cy-h/2,w,h), 'L', fixedSize, dCenter, _scale), this->Width()*_scale, this->Height()*_scale);	 
	for(int i=0; i<items.size(); i++)
	{
		if(i==0)
		   aMin = findOverlap(cx,cy,rect,items[i]);
		else 
		   aMin = aMin + findOverlap(cx, cy, rect, items[i]);		
	}
	if(aMin==0)
	{
		return rect;	
	}
	for(float x=x0; x<x1+itvx; x=x+itvx)
	for(float y=y0; y<y1+itvy; y=y+itvy)
	{
		//float x=0.4676;
		//float y=0.4255;
		vector<vector<int>> items = whichItem( getrecttoRect(QRectF(x-w/2,y-h/2,w,h), 'L', fixedSize, dCenter, _scale), this->Width()*_scale, this->Height()*_scale);	 
		for(int i=0; i<items.size(); i++)
		{
			float area=findOverlap(x,y, rect, items[i]);
			if(area!=0)
				area=area;
			
			if(area<aMin)
			{
				aMin=area;
				minx=x; miny=y;
				//mid=id, mtype=type;
			}
			else if(area==aMin)
			{
				QPointF dPoint= rect.center() - QPointF(x,y);
				float distance = pow(dPoint.x(),2) + pow(dPoint.y(),2);			
				if(distance<dMin)
				{
					dMin=distance;
					minx=x; miny=y;
					//mid=id, mtype=type;
				}
			}	
		}	
	}
	if(items.empty())
		return rect;
	
	return QRectF(minx-w/2,miny-h/2,w,h);
}


bool PathBubble1::collapseALL()
{
	for(int i=0; i<ANodePos.size() ; i++)
	{
		if(ANodePos[i].width()>-900 && ANodePos[i].x()>-900)	
        {
			collapse(i);
		}
	}	
	return true;
}

void PathBubble1::tempCollapseALL(vector<QRectF> &backUpAPos, vector<vector<vector<int>>> &_sceneANodeContain)
{
	backUpAPos=ANodePos;
	_sceneANodeContain=_scene->_ANodeContain;
	for(int i=0; i<ANodePos.size() ; i++)
	{
		if(ANodePos[i].width()>0)	
        {
			collapse(i);
		}
	}		
}

void PathBubble1::reAggregateAll(vector<QRectF> backUpAPos, vector<vector<vector<int>>> _sceneANodeContain)
{
	for(int i=0; i<_sceneANodeContain[m_pathwayID].size(); i++)
	{
		//if(ANodePos[i].width()>-1000)	
		if(backUpAPos[i].width()>0)
        {
				set<vector<int>> highlightedT;				
				for(int j=0; j<_sceneANodeContain[m_pathwayID][i].size(); j=j+2)
				{
				
					int type=_sceneANodeContain[m_pathwayID][i][j], id=_sceneANodeContain[m_pathwayID][i][j+1];		
					vector<int>	item(2,0);
					item[0]=type, item[1]=id;
					highlightedT.insert(item);												
				}
			    aggregation(highlightedT, i);
		}
	}	
	ANodePos=backUpAPos;
	_scene->_ANodeContain=_sceneANodeContain;	
}


int PathBubble1::collapse()
{
	int flag=0; 
	if(_penisOn)
	{
		if(_lines.size()!=0)
		    _penisOn=false;
		groupSelected=groupSelection(_lines);			
	}
	for(set<vector<int>>::iterator it=searched.begin(); it!=searched.end(); it++)
	{	
	     highlighted.insert(*it);	
	}
	
	set<vector<int>> highlightedT;
	for(set<vector<int>>::iterator it=highlighted.begin(); it!=highlighted.end(); it++)
	{
		vector<int> item = *it;
		int type=item[0], id=item[1];		
		if(type=='M')
		{
			//compartmentToItemHighlighted(id, highlightedT);			
			for(set<vector<int>>::iterator it=_scene->CompartmentContain[m_pathwayID][id].begin(); it!=_scene->CompartmentContain[m_pathwayID][id].end(); it++)
			{   
				vector<int> temp=*it;						
				if(temp[0]=='L')
				{
				    if(ANodePos[temp[1]].width()>-1000)	
					   highlightedT.insert(*it);				
				}
			}
		}
		else if(type=='W')
		{
			for(int i=0; i<ANodePos.size() ; i++)
			{
			     if(ANodePos[i].width()>-1000)
				 {
					 vector<int> temp;
					 temp.push_back('L');
					 temp.push_back(i);
				     highlightedT.insert(temp);		
				 }			
			}
		}
		else if(type=='L')
		{
			if(ANodePos[id].width()>-1000)
		        highlightedT.insert(item);			
		}
	}
	flag=highlightedT.size();
	for(set<vector<int>>::iterator it=highlightedT.begin(); it!=highlightedT.end(); it++)
	{
        vector<int> item = *it;
		int type=item[0], id=item[1];
	    if(type=='L')
		{
			flag++;
		    collapse(id);
		    flag=-1;
		}
		else
		{
		   	QMessageBox msgBox; msgBox.setText( "Error in PathBubble1.cpp collapse()" ); msgBox.exec();
		}
	}
	return flag;	
}

bool PathBubble1::isLinkinOriginalPathway(vector<vector<int>> nlink, vector<vector<vector<int>>> edge1, bool reverse)
{
	/*int pid1, pid2, type1, type2, id1, id2;
	if(m_pathwayID == nlink[0][0])
	{
		pid2 = nlink[0][0], type2 = nlink[0][1], id2 = nlink[0][2];
		pid1 = nlink[1][0], type1 = nlink[1][1], id1 = nlink[1][2];
	}
	else if(m_pathwayID == nlink[1][0])
	{
		pid1 = nlink[0][0], type1 = nlink[0][1], id1 = nlink[0][2];
		pid2 = nlink[1][0], type2 = nlink[1][1], id2 = nlink[1][2];
	}
	else return false;

	bool flag=false;
	if(_scene->_pathBubbles[pid1])
	if(_scene->_pathBubbles[pid1]!=NULL)
	if(_scene->_pathBubbles[pid1]->m_pathwayID>=0 && _scene->_pathBubbles[pid1]->m_pathwayID < _scene->_pathBubbles.size())
    if(_scene->_pathBubbles[pid2])
	if(_scene->_pathBubbles[pid2]!=NULL)
	if(_scene->_pathBubbles[pid2]->m_pathwayID>=0 && _scene->_pathBubbles[pid2]->m_pathwayID < _scene->_pathBubbles.size())
	{
		PathBubble1* path1 = _scene->_pathBubbles[pid1];
		PathBubble1* path2 = _scene->_pathBubbles[pid2];
		//edge1
	}*/
	vector<vector<int>> temp;
	if(reverse)
	{
	   temp.push_back(nlink[1]);	temp.push_back(nlink[0]);
	}
	else
		temp=nlink;
	for(int i=0; i<edge1.size(); i++)
	{
		if(temp==edge1[i])
			return true;		
	}
	return false;
}



int PathBubble1::getOriNodeID(int pid1, int pid2, int type2, int id2)
{
	int id1=-1;
	if(_scene->_pathBubbles[pid1])
	if(_scene->_pathBubbles[pid1]!=NULL)
	if(_scene->_pathBubbles[pid1]->m_pathwayID>=0 && _scene->_pathBubbles[pid1]->m_pathwayID < _scene->_pathBubbles.size())
    if(_scene->_pathBubbles[pid2])
	if(_scene->_pathBubbles[pid2]!=NULL)
	if(_scene->_pathBubbles[pid2]->m_pathwayID>=0 && _scene->_pathBubbles[pid2]->m_pathwayID < _scene->_pathBubbles.size())
	{
		PathBubble1* path1 = _scene->_pathBubbles[pid1];
		PathBubble1* path2 = _scene->_pathBubbles[pid2];
		for(int m=0; m < path2->itemInfo.size(); m++)
		{
			int type= path2->itemInfo[m][0], i1=path2->itemInfo[m][1], i2= path2->itemInfo[m][2];// pid=this->_pathBubbles[i]->itemInfo[k][3], jid=this->_pathBubbles[i]->itemInfo[k][4];
			if(type == type2 && i2==id2)
			{
			     id1=i1;
			}			
		}
	}
	return id1;	
}


void PathBubble1::updateEdgeAfterCollapse(int Aid) 
{//recover edge and nodepos (node position)

	if(Aid>=0 && Aid<_scene->_ANodeContain[m_pathwayID].size())
	for(int i=0,j=0; i<_scene->_ANodeContain[m_pathwayID][Aid].size(); i=i+2,j=j+1)
	{        
		bool flag;
	    int type = _scene->_ANodeContain[m_pathwayID][Aid][i], id = _scene->_ANodeContain[m_pathwayID][Aid][i+1];

		QPointF rCenter(0,0);
		QPointF center;
		QRectF pos;
		if(_scene->_ANodePosContain.size()>m_pathwayID && _scene->_ANodePosContain[m_pathwayID].size()>Aid && _scene->_ANodePosContain[m_pathwayID][Aid].size()>j)
		{
			rCenter=_scene->_ANodePosContain[m_pathwayID][Aid][j].center();
			pos = _scene->_ANodePosContain[m_pathwayID][Aid][j];
			center = ANodePos[Aid].center() + rCenter;
			pos.moveCenter(center);
		}
		else
		{
		    switch(type)
			{
				case 'C': pos = _scene->_complexPos[m_pathwayID][id];  break;
				case 'E': pos = _scene->_physicalEntityPos[m_pathwayID][id];	break; 
				case 'L': pos = _scene->_ANodePos[m_pathwayID][id];  break; 
				case 'P': pos = _scene->_proteinPos[m_pathwayID][id]; break; 
				case 'S': pos = _scene->_smallMoleculePos[m_pathwayID][id]; break; 
				case 'D': pos = _scene->_DnaPos[m_pathwayID][id];   break;  
				case 'R': pos = _scene->_reactionPos[m_pathwayID][id]; break; 
				case 'M': pos = _scene->_compartmentPos[m_pathwayID][id];  break; 
			}			
		}
		
		switch(type)
		{
		   	case 'C': if(id < complexPos.size()) complexPos[id] = pos;  break;
		    case 'E': if(id < physicalEntityPos.size()) physicalEntityPos[id] = pos;	break; 
			case 'L': if(id < ANodePos.size()) ANodePos[id] = pos;  break; 
		    case 'P': if(id < proteinPos.size()) proteinPos[id] = pos; break; 
		    case 'S': if(id < smallMoleculePos.size()) smallMoleculePos[id] = pos; break; 
		    case 'D': if(id < DnaPos.size()) DnaPos[id] = pos;   break;  
		    case 'R': if(id < reactionPos.size())
						  reactionPos[id] = pos; 					  
				      break; 
			case 'M': if(id < compartmentPos.size()) compartmentPos[id] = pos;  break; 
		}	
		for(int j=0; j<edge.size(); j++)
		{	
            flag=false;
			for(int k=0; k<6; k=k+2)
			{
				int type1=edge[j][k], id1=edge[j][k+1];	
  				if(type1=='L' && id1==Aid)
				{
					edge[j][k]=type, edge[j][k+1]=id;	
					flag=true;
					break;
				}
			}
			if(flag)
				break;
		}
	}	
}

bool PathBubble1::collapse(int Aid)
{
	//recover the original edge related to Aid	
	updateSpanEdgesWhenCollapse(Aid);
	updateEdgeAfterCollapse(Aid);
	vector<int> start(2,0), middle(2,0), end(2,0);

	
	//also remove those redudent ANode		
	vector<int> temp;
	temp.push_back('L');  temp.push_back(Aid);
	for(int mid=0; mid<_scene->CompartmentContain[m_pathwayID].size(); mid++)
	{
		if(_scene->CompartmentContain[m_pathwayID][mid].find(temp)!=_scene->CompartmentContain[m_pathwayID][mid].end())
		{
		    _scene->CompartmentContain[m_pathwayID][mid].erase(temp);
		}
	}
	ANodePos[Aid]=QRectF(-2000,-2000, -2000, -2000);  
	getGraphToBePaint();
	//_scene->backupItemPos(m_pathwayID, complexPos, proteinPos, smallMoleculePos, DnaPos, EmptyPos, reactionPos, physicalEntityPos, compartmentPos, ANodePos, _scale, dCenter, drawSmallmolecule);				
	itemGrid=recordItemGrid(visibleItemNum, visibleCompartmentNum);

	if(_pathBubbleParent != NULL)
	{
		int pid1=_pathBubbleParent->m_pathwayID;		
		int aid1 = getOriNodeID(_pathBubbleParent->m_pathwayID, m_pathwayID, 'L', Aid);
		_pathBubbleParent->updateEdgeAfterCollapse(aid1);
	}

    return true;
}

int PathBubble1::undo()
{
	clearExpressed();
    clearShared();
	clearDiffered();
	clearLinked();
	highlighted.clear();

	int eventType=-1;
	if(!eventStored.empty() && currentEventit!=eventStored.begin() )
	{
		eventType=*currentEventit;		
		switch(eventType)
		{
			case SEARCHSHARED: 
				undoShared();				
				break;
			case SEARCHDIFFERED:
				undoDiffered();			
				break;
			case SEARCHLINKED:
				undoLinked();			
				break;
			case SEARCHEXPRESSED:
				undoExpressed();			
				break;
			case COLORED:
				undoColored();			
				break;
			case GRAPHCHANGE:
				undoGraph();			
				break;
			case HIGHLIGHTED:
				//currentHighlightedit
				undoHighlighted();			
				break;
		}
		currentEventit--;	
	}    
    if(updateGraphIfHighlightedChanged())
		return eventType;
	else return -1;
}

void PathBubble1::cleanBeforeRedoUndo(int eventType)
{	
	if(eventType==SEARCHEXPRESSED)
	{
		clearExpressed();			
	}
	else if(eventType==HIGHLIGHTED)
	{		
		highlighted.clear();
	}
	else if(eventType==COLORED)
	{
	   clearColored();
	}
	else
	{
		clearShared();
	    clearDiffered();
	    clearLinked();		
	}
}

int PathBubble1::redo()
{	
	if(eventStored.empty())
		return -1;

	if(currentEventit==eventStored.end())
		return -1;

    int eventType;
	deque< int >::iterator end=currentEventit;
	end++;
	if(end!=eventStored.end() )
	{
		currentEventit++;
		eventType=*currentEventit;			
		switch(eventType)
		{
			case SEARCHSHARED: 
				redoShared();				
				break;
			case SEARCHDIFFERED:
				redoDiffered();			
				break;
			case SEARCHLINKED:
				redoLinked();			
				break;
			case SEARCHEXPRESSED:
				redoExpressed();			
				break;
			case COLORED:
				redoColored();			
				break;

			case GRAPHCHANGE:
				redoGraph();			
				break;

			case HIGHLIGHTED:
				//currentHighlightedit
				redoHighlighted();			
				break;
		}
	}  
	if(updateGraphIfHighlightedChanged())
		return eventType;
	else return -1;
}
			

int PathBubble1::getSubPathwaySize()
{
	int size=_pathBubbles.size();
   return size;
}

QString PathBubble1::getCompartmentNames(vector<int> itemSelected)
{
	QString name;
	for(int i=0; i<itemSelected.size(); i=i+2)
	{
		if(itemSelected[i]=='M')
		{
			if(name.size()!=0)	         
				 name = name + ";";			
			name = name + _scene->_compartmentName[m_pathwayID][itemSelected[i+1]][0];					
		}
	}
	return name;
}

/*
QString name=getCompartmentNames(itemSelected)
if(name.size()==0)
   if(itemSelected[0]=='M' && !groupSelected)
   {
       name = _scene->_compartmentName[m_pathwayID][itemSelected[1]][0];
   }			   	
   else 
   {
		int tindex= name.lastIndexOf("\\");
		name = name.mid(14, name.size()-1);
		name= "Sub(" + name;	
		name = name + ")";	
   }
*/

void PathBubble1::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	ItemBase::mouseReleaseEvent( event );
	isInnerResizing=false;

	if(draggingItem && !m_isInnerMoving)
    {
		if(dragout!=2 && dragout!=0)
		{
			itemDragged.clear();			
			edgeSelected.clear();
			highlighted.clear();
			
			vector<int> empty;
			getPartUpdateSelectedNodetoBePaint(itemSelected, empty);
			itemGrid=recordItemGrid(visibleItemNum, visibleCompartmentNum);
			itemSelected.clear();
		}
	}

	if(m_isInnerMoving)
	{
		itemGrid=recordItemGrid(visibleItemNum, visibleCompartmentNum);
		m_isInnerMoving=false;	
	}

	if(m_pathwayID<0) return;
		_mousePressed=0;

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
			    case 'C': qstr=_scene->_complexNameD[dataID][id][0]; iPos=complexPos[id].center(); break;
		        case 'E': qstr=_scene->_physicalEntityNameD[dataID][id][0]; iPos=physicalEntityPos[id].center(); break;
		        case 'P': qstr=_scene->_proteinNameD[dataID][id][0]; iPos=proteinPos[id].center(); break;
		        case 'S': s1 = "smallMolecule";  	oss << s1 << id; s1=oss.str(); qstr=s1.c_str(); iPos=smallMoleculePos[id].center(); break;		
		        case 'D': s1 = "Dna"; 	oss << s1 << id;	s1=oss.str();  qstr=s1.c_str(); iPos=DnaPos[id].center(); break;	 				     	
				case 'N': qstr=_scene->_EmptyNameD[dataID][id][0]; iPos=EmptyPos[id].center(); break;	
			}
			iPos.setX(iPos.x()*this->Width()-this->Width()/2.0);
			iPos.setY(iPos.y()*this->Height()-this->Height()/2.0);
			showIterm(iPos, event->pos(), event->scenePos(), qstr); 	
			_penisOn=groupSelected=_grabisOn=_queryisOn=_upisOn=_downisOn=_evenisOn=false;
		    itemDragged.clear();
			itemSelected.clear();
		    edgeSelected.clear();		
		}
		else if(_grabisOn && ((!groupSelected ) || (draggingItem && !groupSelected && !_grabisOn && dragout==1)) )
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
			    case 'C': qstr=_scene->_complexNameD[dataID][id][0]; iPos=complexPos[id].center(); break;
		        case 'E': qstr=_scene->_physicalEntityNameD[dataID][id][0]; iPos=physicalEntityPos[id].center(); break;
		        case 'P': qstr=_scene->_proteinNameD[dataID][id][0]; iPos=proteinPos[id].center(); break;
		        case 'S': s1 = "smallMolecule";  	oss << s1 << id; s1=oss.str(); qstr=s1.c_str(); iPos=smallMoleculePos[id].center(); break;		
		        case 'D': s1 = "Dna"; 	oss << s1 << id;	s1=oss.str();  qstr=s1.c_str(); iPos=DnaPos[id].center(); break;
				case 'N': qstr=_scene->_EmptyNameD[dataID][id][0]; iPos=EmptyPos[id].center(); break;	
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
		else if( (groupSelected && !_grabisOn&&!_queryisOn&&!_upisOn&&!_downisOn&&!_evenisOn) || (draggingItem && !groupSelected && !_grabisOn && dragout==2) )
		{
	        QPointF Dock=dockItemDragged(event->pos());		
		    //new bubble
		    QString qstr;
			string s1;
			std::ostringstream oss;			
			   
			QPointF iPos;

			//add node contained by ANodes
			vector<set<vector<int>>> AnodeContains;
			int isize=itemSelected.size();
			set<vector<int>> itemSet;
			vector<int> itemSelectedT;
			for(int j=0; j<isize; j=j+2)
			{
			   if(itemSelected[j]=='L')
			   {
				   set<vector<int>> nodeitems;
				   int aid=itemSelected[j+1];
				   for(int k=0; k<_scene->_ANodeContain[m_pathwayID][aid].size(); k=k+2)
				   {
					   int type=_scene->_ANodeContain[m_pathwayID][aid][k], id=_scene->_ANodeContain[m_pathwayID][aid][k+1];
					   vector<int> node(2,0);
					   node[0]=type, node[1]=id;
					   itemSet.insert(node);						   
					   nodeitems.insert(node);
				   }
				   AnodeContains.push_back(nodeitems);
				   collapse(aid);
			   }
			   else
			   {
				    int type=itemSelected[j], id=itemSelected[j+1];						
			
					itemSelectedT.push_back(itemSelected[j]);  itemSelectedT.push_back(itemSelected[j+1]);
			   }
			}
			itemSelected= itemSelectedT;
			for(set<vector<int>>::iterator it=itemSet.begin(); it!=itemSet.end(); it++)
			{
				vector<int> node=*it;				
				int type=node[0], id=node[1];
				itemSelected.push_back(type);
			    itemSelected.push_back(id);
			}		
			setEdgeAffected(itemSelected, _edgeSelected, _edgeAffected);
			if(!this->realRect().contains(event->pos().x(),event->pos().y()) && verifyItemSelected(itemSelected))
		    {
			    m_pathwayID=m_pathwayID;			   
				edgeSelected.clear();
				checkGroupConnections(itemSelected, edgeSelected);
			   
			   QString name = _pathName;//_name;
			   QString cname = getCompartmentNames(itemSelected);
			   if(cname.size()==0)
			   {
				    int tindex= name.lastIndexOf("\\");
					name = name.mid(14, name.size()-1);
					name= "Sub(" + name;	
					name = name + ")";	
			   }
			   else name=cname;
		 
			   QRectF rect=_contain.boundingRect();
			   iPos=rect.center();

				

			   iPos=event->scenePos();
			   //find bubble location
			   PathBubble1* sb=NULL;
			   subPathBubble1 *rb=NULL;
			   if(!toplevel && _pathBubbleParent && _pathBubbleParent->isVisible())
			   {
				   ItemBase * tb=_scene->findBubbleUnderAlocation(iPos);
				   if(tb==_pathBubbleParent)
				   {
					    sb = dynamic_cast <PathBubble1*> (tb);	
				   }
			   }
			   if(sb==NULL)
			   {
				   rb = dynamic_cast <subPathBubble1*> (_open->openSubPathBubble1(getOrthFileName(), m_pathwayID, realRect().width(), realRect().height(), iPos.x(), iPos.y(), itemSelected, edgeSelected, AnodeContains,name));		
			   }
		       if ( rb || sb) 
 	           {
				    if(_TYPE==SUBPATHBUBBLE1)
					{
					   int Cnum,Enum,Pnum,Snum,Dnum,Rnum;
	                   Cnum=Enum=Pnum=Snum=Dnum=Rnum=0;
					   for(int i=0; i<itemInfo.size(); i++)
	                   {
	                        int type=itemInfo[i][0], id=itemInfo[i][2];							
		                    for(int j=0; j<itemSelected.size(); j=j+2)
							{
								if(itemSelected[j]==type&&itemSelected[j+1]==id)
							    {
							         itemInfo[i][3]= this->_pathBubbles.size();
								     itemInfo[i][4]= j/2;							
							    }
							}
						}
					}
					if(rb)
					{
						rb->_pathBubbleParent=this;
						this->_groupPoint.append( QPointF((iPos.x()+this->Width()/2)/this->Width(), (iPos.y()+this->Height()/2)/this->Height()) );
						this->_pathBubbles.append( rb );
					}

					int size=this->_pathBubbles.size();
					
					_grouphighlight.append(highlighted);  
					_groupEdgeAffected.append(_edgeAffected);


					if(sb)
				   {
					   //bool OpenGLScene::del_merg_PathBubble(PathBubble1* eBubble, PathBubble1* pBubble, false);
				       //scene->del_merg_PathBubble(sb, this, itemSelected);		   
					   sb->getPartItemBack(this, itemSelected);
					   sb->getGraphToBePaint();
				   }

					int gsize=_grouphighlight.size();					
					for(int j=0; j<itemSelected.size(); j=j+2)
					{
						int type=itemSelected[j], id=itemSelected[j+1];
						switch(type)
						{
							case 'C': complexPos[id]=QRectF(-2000,-2000,complexPos[id].width(),complexPos[id].height()); break;
							case 'E': physicalEntityPos[id]=QRectF(-2000,-2000, physicalEntityPos[id].width(), physicalEntityPos[id].height()); break;
		                    case 'P': proteinPos[id]=QRectF(-2000,-2000,proteinPos[id].width(),proteinPos[id].height()); break;
		                    case 'S': smallMoleculePos[id]=QRectF(-2000,-2000,smallMoleculePos[id].width(),smallMoleculePos[id].height()); break;
		                    case 'D': DnaPos[id]=QRectF(-2000,-2000,DnaPos[id].width(),DnaPos[id].height());           break;
							case 'R': reactionPos[id]=QRectF(-2000,-2000,reactionPos[id].width(),reactionPos[id].height());      break;
							case 'M': compartmentPos[id]=QRectF(-2000,-2000,compartmentPos[id].width(),compartmentPos[id].height());   break;
							case 'L': ANodePos[id]=QRectF(-2000,-2000, ANodePos[id].width(), ANodePos[id].height());   break;
					    }			 					 
		           }
				   if(! (emptyPathBubble() && sb) )
				   {
					   if(toplevel)
					   {
						   edgeSelected.clear();
					   }
					   recordPathBubbleConnections();
					   if(toplevel)
					   {
							updateBubbleSize_2();
					   }
					   else
					   {
							subPathBubble1 * sbubble = dynamic_cast< subPathBubble1 *> (this);
							sbubble->updateBubbleSize_2();
					   }
					   //relocation all the related bubbles after the parent is resized	
					   if(rb && !sb)
					   {
						   QRectF rect1=this->sceneBoundingRect(), rect2=rb->sceneBoundingRect();
						   QPoint pos = this->_scene->getSonPosition(rb , this, rb->sceneBoundingRect().x(), rb->sceneBoundingRect().y(), rb->sceneBoundingRect().width(), rb->sceneBoundingRect().height());
						   resetOriRect();
						   _scene->addToGroup( this, rb, false);
					   }				   
					   itemDragged.clear();
					   itemSelected.clear();
					   highlighted.clear();
					   //the bubble became empty
					   getGraphToBePaint();		
				   }
				   else if(sb)
				   {
						selfDelection(sb, this);
						unGroupbyDelection();
						/*int size=_pathBubbles.size(),psize=-1;	
						QRectF result(-10000,-10000,-10000,-10000);
						if(_pathBubbleParent!=NULL)
						{
							result=_pathBubbleParent->realRectF();
						}					
						_scene->bubbleDeleted(this, result);					
						emit bubbleDeleteItself(this);
						//deleteSelectedItems(_scene);			*/
				   }
				 }
			}
			_penisOn=groupSelected=_grabisOn=_queryisOn=_upisOn=_downisOn=_evenisOn=false;
			itemDragged.clear();
			itemSelected.clear();
			edgeSelected.clear();
			_contain.clear();			
		}	
	}
	
	if (event->button() == Qt::LeftButton )
	{
		if(_penisOn)
		{
			if(_lines.size()!=0)
			    _penisOn=false;
			if(groupSelected=groupSelection(_lines))
			{
				set<vector<int>> highlightedT=highlighted;
				for(set<vector<int>>::iterator it=highlighted.begin(); it!=highlighted.end(); it++)
				{
					vector<int> item = *it;
					int type=item[0], id=item[1];		
					if(type=='M')
					{
						compartmentToItemHighlighted(id, highlightedT);	
					}
				}
				highlighted=highlightedT;
				vector<int> pre;
				getPartUpdateSelectedNodetoBePaintWhenMoved(pre, itemSelected,event->pos()-event->lastPos());	///getPartUpdateSelectedNodetoBePaint(pre, itemSelected);			
			}
		}
		else if(!_penisOn && groupSelected)
	    {
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
		itemDragged.clear();
		itemSelected.clear();	
		edgeSelected.clear();
	}	
	

	draggingItem=0;
	if(dragout!=0) //update pathwayGraph record
	{
		QString fileName = getPathwayGraphName(pathwayGraphStored);
		if(fileName.size()>0)
        {
			savePathWay(fileName, complexPos, proteinPos, smallMoleculePos, DnaPos, reactionPos, physicalEntityPos, ANodePos);	
			if(pathwayGraphStored.empty())
			{
				QString empty;	
				pathwayGraphStored.push_back(empty);	
			}		
			//coloredStored.push_back(_itemColored);	
			//currentColoredit=coloredStored.end();
			//currentColoredit--;	
			
			pathwayGraphStored.push_back(fileName);

			vector<int> etemp(3,0);
			vector< vector<int> > eventRecord;
			etemp[0]=getType();				
			etemp[1]=m_pathwayID;	
			etemp[2]=GRAPHCHANGE;
			eventRecord.push_back(etemp);
			_scene->storeEvent(eventRecord);
		}
		if(pathwayGraphStored.size()>=10)
			pathwayGraphStored.pop_front();
		
		currentPathwayGraphit = pathwayGraphStored.end();
		currentPathwayGraphit--;
	}
	dragout=0;	
	_scene->removeToolsinGroup(this, false);
	pressOnBoarder = false;
    pressOnUngroup = false;
}
 
bool PathBubble1::emptyPathBubble()
{
	//vector<QRectF> complexPos, proteinPos, smallMoleculePos, DnaPos, reactionPos, degradationPos, physicalEntityPos, compartmentPos, EmptyPos, ANodePos; //0-1
	for(int i=0; i<complexPos.size(); i++)
	{
		if(complexPos[i].x()>-0.99)
		{  		
			return false;
		}			
	}
	for(int i=0; i<proteinPos.size(); i++)
	{
		if(proteinPos[i].x()>-0.99)
		{  		
			return false;
		}			
	}
	for(int i=0; i<reactionPos.size(); i++)
	{
		if(reactionPos[i].x()>-0.99)
		{  		
			return false;
		}			
	}
	for(int i=0; i<compartmentPos.size(); i++)
	{
		if(compartmentPos[i].x()>-0.99)
		{  		
			return false;
		}			
	}
	for(int i=0; i<smallMoleculePos.size(); i++)
	{
		if(smallMoleculePos[i].x()>-0.99)
		{  		
			return false;
		}			
	}
	for(int i=0; i<DnaPos.size(); i++)
	{
		if(DnaPos[i].x()>-0.99)
		{  		
			return false;
		}			
	}
	
	
	for(int i=0; i<physicalEntityPos.size(); i++)
	{
		if(physicalEntityPos[i].x()>-0.99)
		{  		
			return false;
		}			
	}
	for(int i=0; i<degradationPos.size(); i++)
	{
		if(degradationPos[i].x()>-0.99)
		{  		
			return false;
		}			
	}
	
	//EmptyPos, ANodePos
	for(int i=0; i<EmptyPos.size(); i++)
	{
		if(EmptyPos[i].x()>-0.99)
		{  		
			return false;
		}			
	}	
	for(int i=0; i<ANodePos.size(); i++)
	{
		if(ANodePos[i].x()>-0.99)
		{  		
			return false;
		}			
	}	
	return true;
}

//get a file name
QString PathBubble1::getPathwayGraphName(deque< QString > pathwayGraphStored)
{
	QString fileName,temp,base;

	
	QString _curFilePath = _curFileName;
	int tindex = _curFilePath.lastIndexOf("/");
	_curFilePath.remove(0, tindex+1);	

	base = "data/Reactome_Pathway_Data/pathwayGraph/" + _curFilePath;
	if(base[base.size()-1]=='_')
		base=base.mid(0,base.size()-1);
	if(drawSmallmolecule)
	    base = base + "_S_";
	else
	    base = base + "_";
	QString s = QString::number(m_pathwayID);
	base = base + s;
	base = base + "_";
	if(pathwayGraphStored.empty())
	{
	    fileName = base + "0";
		return fileName;
	}
	
	set<int> ids;
	for(deque< QString > ::iterator it=pathwayGraphStored.begin(); it!=pathwayGraphStored.end(); it++)
    {
		temp=*it;
		int size1=base.size(), size2=temp.size()-4-size1;
	    temp=temp.mid(size1,size2);	
		int id=temp.toInt();
		ids.insert(id);
	}

	for(int i=0;i<22;i++)
	{
	      if(ids.find(i)==ids.end())
		  {
			   QString s = QString::number(i);
		       fileName = base + s;							   		
		       return fileName; 		  
		  }
	}
	return fileName;
}

bool PathBubble1::verifyItemSelected(vector<int> itemSelected)
{
	/*bool flag=false;
	for(int i=0; i<itemSelected.size(); i+=2)
	{
	    if(itemSelected[i]!='R')
		{
			flag=true;
			break;
		}
	}
	if(itemSelected.size()==2 && (itemSelected[0]=='M' || itemSelected[0]=='W'))
		return false;
	return flag;*/
	return true;
}

void PathBubble1::checkGroupConnections(vector<int> &itemSelected, vector<vector<int>> &edgeSelected)
{ //add or delete reaction nodes as needed, add reaction edges as needed
	vector<int> item(2,0);
	set<vector<int>> newhighlight;
	
	set<vector<int>> itemSet;
	set<vector<int>> removelist;
	vector<int> newitem;
	for(int i=0; i<itemSelected.size(); i+=2)
	{
	    int type=itemSelected[i], id=itemSelected[i+1];
		item[0]=type, item[1]=id;
		itemSet.insert(item);		
	}
	int type,id,type1,id1,type2,id2;

	//remove isolated reaction nodes
	for(int i=0; i<_scene->_edge[m_pathwayID].size(); i++)
	{         			 
		 type=_scene->_edge[m_pathwayID][i][0], id=_scene->_edge[m_pathwayID][i][1];		
		 type1=_scene->_edge[m_pathwayID][i][4],	id1=_scene->_edge[m_pathwayID][i][5];
		 type2=_scene->_edge[m_pathwayID][i][2], id2=_scene->_edge[m_pathwayID][i][3];	
		 vector<int> item,item2,item1;
		 item.push_back(type), item.push_back(id); 
		 item1.push_back(type1), item1.push_back(id1);
		 item2.push_back(type2), item2.push_back(id2);
		 if( itemSet.find(item1) == itemSet.end() && itemSet.find(item) == itemSet.end() && itemSet.find(item2) == itemSet.end() )
		 {
			    removelist.insert(item1);					
				itemSet.erase(item1);
		 }		 
	}	
	for(int i=0; i<itemSelected.size(); i=i+2)
	{   
		type=itemSelected[i]; id=itemSelected[i+1];
        item[0]=type; item[1]=id; 
		if(removelist.find(item)==removelist.end())
		{
		    newitem.push_back(type); newitem.push_back(id);
			newhighlight.insert(item);
		}
	}

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
		 if( type1!='R')
		 {
			 if( itemSet.find(item) != itemSet.end() || itemSet.find(item2) != itemSet.end() )
			 {
				if(itemSet.find(item1) == itemSet.end())
				{
					itemSelected.push_back(item1[0]);
					itemSelected.push_back(item1[1]);	
					itemSet.insert(item1);
					//highlighted.insert(item1);
				}
				edgeSelected.push_back(_scene->_edge[m_pathwayID][i]);
			 }	
		 }
		 else  if( itemSet.find(item) != itemSet.end() || itemSet.find(item1) != itemSet.end() )
		 {
			edgeSelected.push_back(_scene->_edge[m_pathwayID][i]);
		}	
	}	    
	itemSelected=newitem;
	highlighted=newhighlight;
}

bool PathBubble1::groupSelection(QVector<QLineF> _lines)
{
	if(_lines.size()<4)
		return false;

	_lines.push_back(_lines[0]); //head to end

	//polygon	

	float xr=-100000,xl=100000,yt=-100000,yb=100000;

	for(int i=0; i<_lines.size(); i++)
	{
	   if(xl>_lines[i].x1()) xl=_lines[i].x1();
	   if(xr<_lines[i].x1()) xr=_lines[i].x1();
	   if(yt>_lines[i].y1()) yt=_lines[i].y1();
	   if(yb<_lines[i].y1()) yb=_lines[i].y1();		   
	   _contain << _lines[i].p1();
	}
  
	//nodeX[MAX_POLY_CORNERS]
  int  nodes, pixelX, pixelY, i, j, swap, polyCorners = _lines.size()-1;
  vector<int> nodeX(polyCorners,0);
  fills.clear();
  //  Loop through the rows of the image.
  for (pixelY=yt; pixelY<yb; pixelY++) {

  //  Build a list of nodes.
  nodes=0; j=polyCorners-1;
  for (i=0; i<polyCorners; i++) {
    if (_lines[i].y1()<(double) pixelY && _lines[j].y1()>=(double) pixelY
    ||  _lines[j].y1()<(double) pixelY && _lines[i].y1()>=(double) pixelY) {
      nodeX[nodes++]=(int) (_lines[i].x1()+(pixelY-_lines[i].y1())/(_lines[j].y1()-_lines[i].y1())
      *(_lines[j].x1()-_lines[i].x1())); }
    j=i; }

  //  Sort the nodes, via a simple “Bubble” sort.
  i=0;
  while (i<nodes-1) {
    if (nodeX[i]>nodeX[i+1]) {
      swap=nodeX[i]; nodeX[i]=nodeX[i+1]; nodeX[i+1]=swap; if (i) i--; }
    else {
      i++; }}

  //  Fill the pixels between node pairs.
  
  for (i=0; i<nodes; i+=2) 
  {
    if   (nodeX[i  ]>=xr) break;
    if   (nodeX[i+1]> xl ) 
	{
      if (nodeX[i  ]< xl ) nodeX[i  ]=xl ;
      if (nodeX[i+1]> xr) nodeX[i+1]=xr;
      //for (j=nodeX[i]; j<nodeX[i+1]; j++) 
		  //fillPixel(j,pixelY); 
	  fills.push_back(QLineF(nodeX[i],pixelY,nodeX[i+1],pixelY));
	}
  }
  }

    bool flag=0;
    float width=this->Width(), height=this->Height();
    float X,Y,W,H;
	for(int id=0; id<complexRect.size(); id++)
	{	
		if(complexRect[id].width()>0)
		{
			QPolygonF s=_contain.intersected(complexRect[id]);
			if(s.size()!=0)
			{
				flag=1;				
				vector<int> item; item.push_back('C'); item.push_back(id);
				highlighted.insert(item);
			}
		}
	}

	for(int id=0; id<reactionRect.size(); id++)
	{
		if(reactionRect[id].width()>0)
		{			
			QPolygonF s=_contain.intersected(reactionRect[id]);
			if(s.size()!=0)
			{
				flag=1;
				//itemSelected.push_back('C'), itemSelected.push_back(id);
				vector<int> item; item.push_back('R'); item.push_back(id);
				highlighted.insert(item);
			}
		}
	}
	  
	for(int id=0; id<proteinPos.size(); id++)
	{
		if(proteinRect[id].width()>0)
		{			
			QPolygonF s=_contain.intersected(proteinRect[id]);
			if(s.size()!=0)
			{
				flag=1;
				//itemSelected.push_back('C'), itemSelected.push_back(id);
				vector<int> item; item.push_back('P'); item.push_back(id);
				highlighted.insert(item);
			}
		}
	}
	
	for(int id=0; id<physicalEntityPos.size(); id++)
	{
		if(physicalEntityRect[id].width()>0)
		{			
			QPolygonF s=_contain.intersected(physicalEntityRect[id]);
			if(s.size()!=0)
			{
				flag=1;
				//itemSelected.push_back('C'), itemSelected.push_back(id);
				vector<int> item; item.push_back('E'); item.push_back(id);
				highlighted.insert(item);
			}
		}
	}

	for(int id=0; id<smallMoleculePos.size(); id++)
	{
		if(smallMoleculeRect[id].width()>0)
		{			
			QPolygonF s=_contain.intersected(smallMoleculeRect[id]);
			if(s.size()!=0)
			{
				flag=1;				
				vector<int> item; item.push_back('S'); item.push_back(id);
				highlighted.insert(item);
			}
		}
	}

	for(int id=0; id<DnaPos.size(); id++)
	{
		X=DnaPos[id].x(), Y=DnaPos[id].y(), W=DnaPos[id].width(), H=DnaPos[id].height();  
		if(DnaRect[id].width()>0)
		{			
			QPolygonF s=_contain.intersected(DnaRect[id]);
			if(s.size()!=0)
			{
				flag=1;
				//itemSelected.push_back('C'), itemSelected.push_back(id);
				vector<int> item; item.push_back('D'); item.push_back(id);
				highlighted.insert(item);
			}
		}
	}

	for(int id=0; id<ANodePos.size(); id++)
	{
		X=ANodePos[id].x(), Y=ANodePos[id].y(), W=ANodePos[id].width(), H=ANodePos[id].height();  
		if(ANodeRect[id].width()>0)
		{			
			QPolygonF s=_contain.intersected(ANodeRect[id]);
			if(s.size()!=0)
			{
				flag=1;
				//itemSelected.push_back('C'), itemSelected.push_back(id);
				vector<int> item; item.push_back('L'); item.push_back(id);
				highlighted.insert(item);
			}
		}
	}

	
	for(int id=0; id<compartmentPos.size()&&id<compartmentRect.size(); id++)
	{
		X=compartmentPos[id].x(), Y=compartmentPos[id].y(), W=compartmentPos[id].width(), H=compartmentPos[id].height();  
		if(compartmentRect[id].width()>0)
		{			
			QPolygonF s=_contain.intersected(compartmentRect[id]);			
			if(s.size()!=0)
			{
				int count=0;
				bool flag=false;
				for(int i=0; i<_contain.size(); i++)
				{
					if(compartmentRect[id].contains(_contain[i]))
					{
						if(count>0)
						{
							flag=true;
							break;
						}
						else count=-1;
					}
					else 
					{
						if(count<0)
						{
							flag=true;
							break;
						}
						else count=1;				
					}
				}
				if(flag)
				{
					//itemSelected.push_back('C'), itemSelected.push_back(id);
					vector<int> item; item.push_back('M'); item.push_back(id);
					highlighted.insert(item);
				}
			}
		}
	}

	for(set<vector<int>>::iterator it=highlighted.begin(); it!=highlighted.end(); it++)
	{
        vector<int> item= *it;
	    itemSelected.push_back(item[0]), itemSelected.push_back(item[1]);	
	}

	
  checkGroupConnections(itemSelected, edgeSelected);
	 
  return flag;
}

void PathBubble1::setItemDragged(vector<int> itemSelected)
{
	bubbleRect.setWidth(0), bubbleRect.setHeight(0);

	if(getVisibleItemNum()<=1)
		return;

	if(getVisibleCompartmentNum()<=1 && itemSelected[0]=='M')
	    return;
	
	itemDragged.clear();
	for(int i=0; i<itemSelected.size(); i+=2)
	{
	      setAItemDragged(itemSelected[i], itemSelected[i+1]);
		  bubbleRect.setWidth(0), bubbleRect.setHeight(0);
	}	
}

void PathBubble1::setEdgeAffected(vector<int> itemSelected, set<vector<vector<int>>> &edgeSelected, set<vector<vector<int>>> &edgeAffected)
{//edgeSelected in the pen selection, edgeAffected: the edge spaning the selected items with the rest
	
	edgeSelected.clear(); edgeAffected.clear();
	vector < vector <int>> itemset;
	int j=0;	
	for(int i=0; i<itemSelected.size(); i+=2)
	{
		if(itemSelected[i]!='M')
		{
			vector<int> node(3,0);		
			node[0]=m_pathwayID, node[1]=itemSelected[i], node[2]=itemSelected[i+1];
			itemset.push_back(node);
		}
	}

	vector<int> tnode(3,0);

	for(vector < vector <int>> :: iterator it=itemset.begin(); it != itemset.end(); it ++)
	{
		vector<vector<int>> nodes;
		vector<int> node= *it;
		QRectF rect;
		vector<int> temp;
		vector <vector <int>>  edge (2, node), edge2 (2, node);		
		if(node[2]>=0) //debug
		switch(node[1])
		{
		     case 'C': nodes = _complex[node[2]];  break;
			 case 'D': nodes = _Dna[node[2]];  break;
			 case 'E': nodes = _entity[node[2]]; break;
			 case 'L': nodes = _ANode[node[2]]; break;
			 case 'P': nodes = _protein[node[2]]; break;
			 case 'S': nodes = _smallMolecule[node[2]]; break;
			 case 'R': nodes = _reaction[node[2]]; break;
		}		
	
		for(int k=0; k<nodes.size(); k++)
		{	
			 node[0]=m_pathwayID;
			 node[1]=nodes[k][0];
			 node[2]=nodes[k][1];
			 
			 //tnode[0]=m_pathwayID; tnode[1]=node[0], tnode[2]=node[1];

			 edge[1]=node; 
		     
			 if(node[2]>=0)
			 switch(node[1])
		     {
				 case 'C': rect= complexPos[node[2]]; break;
				 case 'D': rect=DnaPos[node[2]];  break;
				 case 'E': rect=physicalEntityPos[node[2]]; break;
				 case 'P': rect=proteinPos[node[2]]; break;
				 case 'S': rect=smallMoleculePos[node[2]]; break;
				 case 'R': rect=reactionPos[node[2]]; break;
				 case 'L': rect=ANodePos[node[2]]; break;
		     }

			  if(rect.x()>-999.99) 
			 {
			    bool flag=false;
                for(int i=0; i<itemset.size(); i++)
			    {
				  if(itemset[i]==node)
					  flag=true;
			    }
				edge2[0] = edge[1];
			    edge2[1] = edge[0];
				if( (updatedEdge_1.find(edge)!=updatedEdge_1.end() ) || (updatedEdge_2.find(edge)!=updatedEdge_2.end() ) || (updatedEdge_3.find(edge)!=updatedEdge_3.end() ) )
				{     
					if(!flag)
					{
						edgeAffected.insert(edge);
					}
					else edgeSelected.insert(edge);
				}
				if( ( updatedEdge_1.find(edge2)!=updatedEdge_1.end() ) || (updatedEdge_2.find(edge2)!=updatedEdge_2.end() ) || (updatedEdge_3.find(edge2)!=updatedEdge_3.end() ) )
				{     
					if(!flag)
					{
						edgeAffected.insert(edge2);
					}
					else edgeSelected.insert(edge2);
				}
			 }
		}		    
		j++;
	}
}



void PathBubble1::setAItemColored(int type, int id, int up_or_down)
{
	vector<int> temp(4,0);
	std::set< vector<int> >::iterator it;
	temp[0]=m_pathwayID; temp[1]=type; temp[2]=id; temp[3]=GO_UP;
	it=_itemColored.find(temp);
	if(it!=_itemColored.end()) _itemColored.erase(it);

	temp[3]=_DOWN;
	it=_itemColored.find(temp);
	if(it!=_itemColored.end()) _itemColored.erase(it);

	temp[3]=_EVEN;
	it=_itemColored.find(temp);	
	if(it!=_itemColored.end()) _itemColored.erase(it);

	temp[3]=up_or_down;
    _itemColored.insert(temp);

	switch(type)
	{
	    case 'C': 
				  expressedComplex_Edit.insert(id);	    break;
	    case 'D': 
				  expressedDna_Edit.insert(id);     break;
		case 'E': 
				  expressedPhysicalEntity_Edit.insert(id);	   break;
		case 'L': 
				  expressedANode_Edit.insert(id);	   break;
		case 'P': 
				  expressedProtein_Edit.insert(id);		  break;
		case 'S': 
			      expressedSmallMolecule_Edit.insert(id);	    break;
		case 'R': 
				  expressedReaction_Edit.insert(id);   break;
	}	

	//case 3: Protein = expressedProtein, Complex = expressedComplex, SmallMolecule = expressedSmallMolecule, Dna = expressedDna, Reaction = expressedReaction, PhysicalEntity = expressedPhysicalEntity, ANode = expressedANode;  break;
}

void PathBubble1::setAItemDragged(int type, int id)
{
	int width = this->realRect().width();
    int height = this->realRect().height();
    int h,w,cx,cy; //Actually wx,wy;
	
	switch(type)
	{
	    case 'C': 
				  itemDragged.push_back(complexRect[id]);	     break;
	    case 'D': 
				  itemDragged.push_back(DnaRect[id]);	     break;
		case 'E': 
				  itemDragged.push_back(physicalEntityRect[id]);	     break;
		case 'L': 
				  itemDragged.push_back(ANodeRect[id]);	     break;
		case 'P': 
				  itemDragged.push_back(proteinRect[id]);	     break;
		case 'S': 
			      itemDragged.push_back(smallMoleculeRect[id]);       break;
		case 'R': 
				  itemDragged.push_back(reactionRect[id]);	     break;
	}	
}

void PathBubble1::paintItem(QPainter *painter, int type, int i, bool flag, QPointF dCenter, float scale, QColor c, QPointF dis)
{
	QRectF rect;
	switch(type)
	{ 
		case 'C': rect=complexRect[i]; //getNodeRect(complexPos[i],  _scene->_complexNameD[dataID][i][0], type, fixedSize, dCenter, scale); 
			      break;
		case 'E': rect=physicalEntityRect[i]; //getNodeRect(physicalEntityPos[i],  _scene->_physicalEntityNameD[dataID][i][0], type, fixedSize, dCenter, scale); 
			      //paintEntity( painter,  physicalEntityPos, _scene->_physicalEntityNameD[dataID], i, dCenter, scale, flag, c); 
			      break;
		case 'P': rect=proteinRect[i]; //getNodeRect(proteinPos[i],  _scene->_proteinNameD[dataID][i][0], type, fixedSize, dCenter, scale); 
			      //paintProtein( painter, proteinPos, _scene->_proteinNameD[dataID], i, dCenter, scale, flag, c); 
			      break;
		case 'S': rect=smallMoleculeRect[i]; //getNodeRect(smallMoleculePos[i],  _scene->_smallMoleculeNameD[dataID][i][0], type, fixedSize, dCenter, scale); 
			      //paintSmallMolecule(painter, smallMoleculePos, _scene->_smallMoleculeNameD[dataID], i, dCenter, scale, flag, c); 
			      break;	
		case 'D': //paintDna(painter, DnaPos, _scene->_DnaNameD[dataID], i, dCenter,scale, flag, c);
			      rect=DnaRect[i]; //getNodeRect(DnaPos[i],  _scene->_DnaNameD[dataID][i][0], type, fixedSize, dCenter, scale); 
			      break;	 	
		case 'R': //paintReaction(painter,reactionPos, _scene->_biochemicalReaction[m_pathwayID], i, dCenter, scale, _scene->_reactionNameD[dataID][i][0], flag, c);  
			      rect=reactionRect[i]; //getNodeRect(reactionPos[i],  _scene->_reactionNameD[dataID][i][0], type, fixedSize, dCenter, scale); 
				  break;	 	
		case 'L': //paintReaction(painter,reactionPos, _scene->_biochemicalReaction[m_pathwayID], i, dCenter, scale, _scene->_reactionNameD[dataID][i][0], flag, c);  
			      rect=ANodeRect[i]; //getNodeRect(reactionPos[i],  _scene->_reactionNameD[dataID][i][0], type, fixedSize, dCenter, scale); 
				  break;	
	}	
	QPen pen;
	pen.setWidth(3);
	pen.setColor(c);
	painter->setPen(pen);
	painter->drawRect( QRectF(rect.x()+dis.x()-1, rect.y()+dis.y()-1, rect.width()+2, rect.height()+2 ));
}


void PathBubble1::showIterm( QPointF iPos, QPointF pos, QPointF scenePos, QString name)
{
	float x=pos.x(), y=pos.y();	
	
	QString fileName("iterms/FURIN");
	//fileName.append(name);
	fileName.append(".txt");
	QByteArray ba = fileName.toLocal8Bit();
    const char *name_str = ba.data();
	
	FILE *fp = fopen(name_str,"r"); 
	if(fp!=NULL)
	{
		//_open->openPathBubble2( 100, 600, 450, fileName);							
		TextBubble2 *txtcode = dynamic_cast<TextBubble2*>(_open->openTextBubble2(m_pathwayID, -1, 200, scenePos.x(), scenePos.y(), fileName));
		if ( txtcode )
 	    {
			 QRectF rect1=this->sceneBoundingRect(), rect2=txtcode->sceneBoundingRect();
		     QPoint pos = this->_scene->getSonPosition(txtcode , this, txtcode->sceneBoundingRect().x(), txtcode->sceneBoundingRect().y(),txtcode->sceneBoundingRect().width(), txtcode->sceneBoundingRect().height());
			// txtcode->_pathBubbleParent=this;
			 this->_conPoint.append( QPointF((iPos.x()+this->Width()/2)/this->Width(), (iPos.y()+this->Height()/2)/this->Height()));
			 this->_codeBubbles.append( txtcode );
			 int size= this->_conPoint.size();

			 set<vector<int>> temp; temp.insert(itemSelected);
			 //_reactionhighlight.append(temp);  
			 _conhighlight.append(temp);  

			 resetOriRect();

			  _scene->addToGroup( this, txtcode, false);
		}
	}
}

void PathBubble1::searchOrthology(QString name)
{
	_orthologyFileName = name;
	int tindex = name.lastIndexOf("/");
	QString temp=name;
	temp.remove(tindex+1,temp.length());
	_orthologyFileName.remove(temp);	
	//tindex = name.lastIndexOf(".");	
	_orthologyFileName.remove("_ortholog.txt");
	
	pwdParser->readOrthologySymbol(name, orthologySymbol);
	orthology = pwdParser->matchSymbol(orthologySymbol,  _scene->_proteinNameD[dataID]);					
	drawOrthology = true;	
}

PathBubble1* PathBubble1::searchSubPathBubble(subPathBubble1* subbubble, int pid, int &jid)
{
    if(pid<0)
	{
		PathBubble1* subtmp = dynamic_cast<PathBubble1*>(subbubble);
		return subtmp;
	}	
	if(subbubble->itemInfo[jid][4]<0)
    {
		PathBubble1* subtmp = dynamic_cast<PathBubble1*>(subbubble);
		return subtmp;
	}	
	
	if(m_pathwayID==0)
          m_pathwayID=m_pathwayID;


	jid=subbubble->itemInfo[jid][4];
	PathBubble1* subtmp = dynamic_cast<PathBubble1*>(subbubble);
	
	//need to check problem here// some times pid out of range
	if(subtmp)
	if( pid < subtmp->_pathBubbles.size() && subtmp->_pathBubbles[pid] )
	{
		vector<vector<int>> itemInfotmp = subtmp->_pathBubbles[pid]->itemInfo;
		if(jid<itemInfotmp.size())
		{
			return(searchSubPathBubble(subtmp->_pathBubbles[pid], itemInfotmp[jid][3], jid));	
		}
	}
	return subtmp;
}

void PathBubble1::getAllSubPathBubble(PathBubble1* bubble, QList<ItemBase *> &list)
{
	int size= bubble->_pathBubbles.size();		
	for( int i = 0; i <size; i++ )
	{	
		list.append( (ItemBase*) bubble->_pathBubbles[i] );			   
	    getAllSubPathBubble(bubble->_pathBubbles[i] , list);
	}		
}

void PathBubble1::getAllSubTextBubble(PathBubble1* bubble, QList<ItemBase *> &list)
{//tex tbubble
	int size= bubble->_codeBubbles.size();		
	for( int i = 0; i <size; i++ )
	{	
		list.append( (ItemBase*) bubble->_codeBubbles[i] );			   
	    bubble->_codeBubbles[i] ->getAllSubTextBubble(bubble->_codeBubbles[i] , list);
	}	
}

/*
{
    int size;
	size=_codeBubbles.size();
	while(size>0)
	{	
		if(_codeBubbles.last())
		{
			_codeBubbles.last()->deleteSelectedItemsbyParent(scene);
		}
		size--;
	}	

	size=_reactionBubbles.size();

	//for (int i = 0; i < _reactionBubbles.size(); i ++)
	while(size>0)
	{	
		if( _reactionBubbles.last())
		{   
		    _scene->bubbleDeleted(_reactionBubbles.last());
			_reactionBubbles.last()->deleteSelectedItemsbyParent(scene);
		}
	    size--;
	}
	

	size=_groupBubbles.size();	
	while(size>0)
	{		
		if(_groupBubbles.last())
		{
			_scene->bubbleDeleted(_groupBubbles.last());
			_groupBubbles.last()->deleteSelectedItemsbyParent(scene);	
		}
	    size--;
	}
	
	size=_pathBubbles.size();	
	while(size>0)
	{	
		if(_pathBubbles.last())
		{
			_scene->bubbleDeleted(_pathBubbles.last());
			_pathBubbles.last()->deleteSelectedItemsbyParent(scene);	
		}
	    size--;
	}
	}
*/

void PathBubble1::getAllPSPathBubble(PathBubble1* bubble, QList<ItemBase *> &list)//bubble in the whole tree hierarchy
{
    //get top
	list.clear();
	PathBubble1* top = getTopPathBubble(bubble);
	getAllSubPathBubble(top , list);
	list.append(top);
}

void PathBubble1::getHigherPathBubble(PathBubble1* bubble, QList<ItemBase *> &list) //bubble in higher level of the whole tree hierarchy
{
	list.clear();
	QList<ItemBase *> SList, AList;
	if(bubble->_parentPathBubbleID<0) 
		return;
		
	getAllSubPathBubble(_scene->_pathBubbles[bubble->_parentPathBubbleID], SList);
	SList.append(bubble); //itself

	int level=getPathBubbleLevel(bubble);

	getAllPSPathBubble(bubble, AList);	
	for(int i=0; i<SList.size(); i++)
	{
        AList.removeOne(SList[i]);	
	}

	for(int i=0; i<AList.size(); i++)
	{
		if(getPathBubbleLevel(dynamic_cast<PathBubble1*>(AList[i]))<level)
           list.append(AList[i]);
	}
	
}


PathBubble1* PathBubble1::getTopPathBubble(PathBubble1* bubble)
{
    if(bubble->_parentPathBubbleID<0)
		return bubble;
	if(_scene->_pathBubbles[bubble->_parentPathBubbleID]==NULL)
		return bubble;
	else return getTopPathBubble(_scene->_pathBubbles[bubble->_parentPathBubbleID]);	
}

//int getPathBubbleLevel(PathBubble1* bubble);
bool PathBubble1::samePathwayGroup(PathBubble1* bubble1, PathBubble1* bubble2)
{
	PathBubble1* TPathBubble1=getTopPathBubble(bubble1);
	PathBubble1* TPathBubble2=getTopPathBubble(bubble2);
	if(TPathBubble2==TPathBubble1)
	{
	    return true;
	}
	else 
		return false;
}

int PathBubble1::getPathBubbleLevel(PathBubble1* bubble)
{ //0
	if(bubble->_parentPathBubbleID<0)
		return 0;
	else 
	{
		return (getPathBubbleLevel(_scene->_pathBubbles[bubble->_parentPathBubbleID]) + 1);		
	}
}


void PathBubble1::drawABubbleConnectioninArrow_H(QPainter *painter, QPointF center1, PathBubble1* path1, QPointF center2, PathBubble1* path2, QPointF Dis, QColor color, int arrowType, int encode, float h, float _scale)
{	//arrowType 0: arrow, 1: activation, 2: inhibition
	QLineF l1;
	QPointF p1,p2,c1,c2;
	QRectF rect1, rect2;
	
	float width;
	float height;
		
	rect1=path1->realRect(), rect2=path2->realRect();

	
	width=graphReferenceSize*(path2->_scale+path1->_scale)/4; height=graphReferenceSize*(path2->_scale+path1->_scale)/4;
		//width=graphReferenceSize/2*_scale; height=graphReferenceSize/2*_scale;
	
	c1=rect1.center()+path1->sceneBoundingRect().center()-this->sceneBoundingRect().center();
	c2=rect2.center()+path2->sceneBoundingRect().center()-this->sceneBoundingRect().center();

	rect1.moveCenter(c1); rect2.moveCenter(c2);

	p1 = center1+path1->sceneBoundingRect().center()-this->sceneBoundingRect().center();
	p2 = center2+path2->sceneBoundingRect().center()-this->sceneBoundingRect().center();
	if(rect1.contains(p1) && rect2.contains(p2))
	{
		
	}
	else if(!rect1.contains(p1) && rect2.contains(p2))
	{
		float x1=p1.x(), y1=p1.y(), x2=p2.x(), y2=p2.y();
        int flag=CSLineClip(x1, y1, x2, y2, rect1.left(), rect1.right(), rect1.top(), rect1.bottom());
		if(flag==1)
			p1=QPointF(x1,y1);
		else if(flag==2)
			p1=QPointF(x2,y2);		
		else if(flag==0) //no intersection
		{
			 float x2=rect1.center().x(),y2=rect1.center().y();
		     int flag2=CSLineClip(x1, y1, x2, y2, rect1.left(), rect1.right(), rect1.top(), rect1.bottom());
			 if(flag2==1)
				p1=QPointF(x1,y1);
			 else if(flag2==2)
				p1=QPointF(x2,y2);
			 else 
				p1=QPointF(x1,y1);		
		}	
		else 
			p1=QPointF(x1,y1);
	}
	else if(rect1.contains(p1) && !rect2.contains(p2))
	{
		//p1=rect1.intersect
		//get intersection of line p1-p2 and rect1
		//painter->drawLine(p1,p2);
		float x1=p1.x(), y1=p1.y(), x2=p2.x(), y2=p2.y();
        int flag=CSLineClip(x1, y1, x2, y2, rect2.left(), rect2.right(), rect2.top(), rect2.bottom());
		if(flag==1)
			p2=QPointF(x1,y1);
		else if(flag==2)
			p2=QPointF(x2,y2);		
		else if(flag==0) //no intersection
		{
			 float x1=rect2.center().x(),y1=rect2.center().y();
		     int flag2=CSLineClip(x1, y1, x2, y2, rect2.left(), rect2.right(), rect2.top(), rect2.bottom());
			 if(flag2==1)
				p2=QPointF(x1,y1);
			 else if(flag2==2)
				p2=QPointF(x2,y2);
			 else 
				p2=QPointF(x2,y2);		
		}	
		else
			p2=QPointF(x2,y2);			
	}
	else
	{	
		float x1=p1.x(), y1=p1.y(), x2=p2.x(), y2=p2.y();
	    float cx1=rect1.center().x(),cy1=rect1.center().y(), cx2=rect2.center().x(),cy2=rect2.center().y();
		int flag1,flag2;
		
		flag1=CSLineClip(cx1, cy1, x1, y1, rect1.left(), rect1.right(), rect1.top(), rect1.bottom());
		if(flag1==1)
			p1=QPointF(cx1,cy1);
		else if(flag1==2)
			p1=QPointF(x1,y1);
		else
			p1=QPointF(x1,y1);
		
		flag2=CSLineClip(cx2, cy2, x2, y2, rect2.left(), rect2.right(), rect2.top(), rect2.bottom());
		if(flag2==1)
			p2=QPointF(cx2,cy2);
		else if(flag2==2)
			p2=QPointF(x2,y2);
		else 
			p2=QPointF(x2,y2);		
	}
	if(arrowType==0)
		drawArrow_H(painter, p1+Dis, p2+Dis, width, height, _scale, color);//HIGHLIGHTCOLOR ); 	
	else if(arrowType==1) 
		drawArrow_H(painter, p1+Dis, p2+Dis, width, height, _scale, color);//HIGHLIGHTCOLOR ); //drawArrow_4(painter, p1+Dis, p2+Dis, width, height, QColor(0, 210, 50, 255) );	
	else if(arrowType==2) 
		drawArrow_H(painter, p1+Dis, p2+Dis, width, height, _scale, color);//HIGHLIGHTCOLOR ); //drawArrow_4_H(painter, p1+Dis, p2+Dis, width, height, 3, QColor(255, 128, 0, 255) );	
	else if(arrowType==3) 
	{
		//int encode = 3;//updatedEdge_5[0];
		set<int> Code;
		for(int i=0; i<6; i++)
		{
			int code=pow(2.0, i);			
			if(code & encode)
			{
				Code.insert(i);
			}
		}
		drawCurvedArrow_H(painter, Code, h, p1+Dis, p2+Dis, width, height,_scale, color);		
	}
}

void PathBubble1::drawABubbleConnectioninArrow(QPainter *painter, QPointF center1, PathBubble1* path1, QPointF center2, PathBubble1* path2, QPointF Dis, QColor color, int arrowType, int encode, float h)
{	//arrowType 0: arrow, 1: activation, 2: inhibition
	QLineF l1;
	QPointF p1,p2,c1,c2;
	QRectF rect1, rect2;
	
	float width;
	float height;
		
	rect1=path1->realRect(), rect2=path2->realRect();

	if(fixedSize)
	{		
		width=graphReferenceSize*(path2->_scale+path1->_scale)/4; height=graphReferenceSize*(path2->_scale+path1->_scale)/4;
		//width=graphReferenceSize/2*_scale; height=graphReferenceSize/2*_scale;
	}
	else
	{
		width=rect1.width()/2*_scale;
	    height=rect1.height()/2*_scale;
	}

	c1=rect1.center()+path1->sceneBoundingRect().center()-this->sceneBoundingRect().center();
	c2=rect2.center()+path2->sceneBoundingRect().center()-this->sceneBoundingRect().center();

	rect1.moveCenter(c1); rect2.moveCenter(c2);

	p1 = center1+path1->sceneBoundingRect().center()-this->sceneBoundingRect().center();
	p2 = center2+path2->sceneBoundingRect().center()-this->sceneBoundingRect().center();
	if(rect1.contains(p1) && rect2.contains(p2))
	{
		
	}
	else if(!rect1.contains(p1) && rect2.contains(p2))
	{
		float x1=p1.x(), y1=p1.y(), x2=p2.x(), y2=p2.y();
        int flag=CSLineClip(x1, y1, x2, y2, rect1.left(), rect1.right(), rect1.top(), rect1.bottom());
		if(flag==1)
			p1=QPointF(x1,y1);
		else if(flag==2)
			p1=QPointF(x2,y2);		
		else if(flag==0) //no intersection
		{
			 float x2=rect1.center().x(),y2=rect1.center().y();
		     int flag2=CSLineClip(x1, y1, x2, y2, rect1.left(), rect1.right(), rect1.top(), rect1.bottom());
			 if(flag2==1)
				p1=QPointF(x1,y1);
			 else if(flag2==2)
				p1=QPointF(x2,y2);
			 else 
				p1=QPointF(x1,y1);		
		}	
		else 
			p1=QPointF(x1,y1);
	}
	else if(rect1.contains(p1) && !rect2.contains(p2))
	{
		//p1=rect1.intersect
		//get intersection of line p1-p2 and rect1
		//painter->drawLine(p1,p2);
		float x1=p1.x(), y1=p1.y(), x2=p2.x(), y2=p2.y();
        int flag=CSLineClip(x1, y1, x2, y2, rect2.left(), rect2.right(), rect2.top(), rect2.bottom());
		if(flag==1)
			p2=QPointF(x1,y1);
		else if(flag==2)
			p2=QPointF(x2,y2);		
		else if(flag==0) //no intersection
		{
			 float x1=rect2.center().x(),y1=rect2.center().y();
		     int flag2=CSLineClip(x1, y1, x2, y2, rect2.left(), rect2.right(), rect2.top(), rect2.bottom());
			 if(flag2==1)
				p2=QPointF(x1,y1);
			 else if(flag2==2)
				p2=QPointF(x2,y2);
			 else 
				p2=QPointF(x2,y2);		
		}	
		else
			p2=QPointF(x2,y2);			
	}
	else
	{	
		float x1=p1.x(), y1=p1.y(), x2=p2.x(), y2=p2.y();
	    float cx1=rect1.center().x(),cy1=rect1.center().y(), cx2=rect2.center().x(),cy2=rect2.center().y();
		int flag1,flag2;
		
		flag1=CSLineClip(cx1, cy1, x1, y1, rect1.left(), rect1.right(), rect1.top(), rect1.bottom());
		if(flag1==1)
			p1=QPointF(cx1,cy1);
		else if(flag1==2)
			p1=QPointF(x1,y1);
		else
			p1=QPointF(x1,y1);
		
		flag2=CSLineClip(cx2, cy2, x2, y2, rect2.left(), rect2.right(), rect2.top(), rect2.bottom());
		if(flag2==1)
			p2=QPointF(cx2,cy2);
		else if(flag2==2)
			p2=QPointF(x2,y2);
		else 
			p2=QPointF(x2,y2);		
	}
	if(arrowType==0)
		drawArrow(painter, p1+Dis, p2+Dis, width, height, QColor(40,40,40,255) );	
	else if(arrowType==1) 
		drawArrow_4(painter, p1+Dis, p2+Dis, width, height, ACTIVATIONCOLOR );	
	else if(arrowType==2) 
		drawArrow_4(painter, p1+Dis, p2+Dis, width, height, INHIBITIONCOLOR );	
	else if(arrowType==3) 
	{
		//int encode = 3;//updatedEdge_5[0];
		
		
		set<int> Code;
		for(int i=0; i<6; i++)
		{
			int code=pow(2.0, i);			
			if(code & encode)
			{
				Code.insert(i);
			}
		}
		drawCurvedArrow(painter, Code, h, p1+Dis, p2+Dis, width, height);		
	}

}


void PathBubble1::drawABubbleConnection(QPainter *painter, QPointF center1, PathBubble1* path1, QPointF center2, PathBubble1* path2, QPointF Dis)
{
	
	QLineF l1;
	QPointF p1,p2,c1,c2;
	QRectF rect1, rect2;
	
	rect1=path1->realRect(), rect2=path2->realRect();
	
	c1=rect1.center()+path1->sceneBoundingRect().center()-this->sceneBoundingRect().center();
	c2=rect2.center()+path2->sceneBoundingRect().center()-this->sceneBoundingRect().center();

	rect1.moveCenter(c1); rect2.moveCenter(c2);

	p1 = center1+path1->sceneBoundingRect().center()-this->sceneBoundingRect().center();
	p2 = center2+path2->sceneBoundingRect().center()-this->sceneBoundingRect().center();
	if(rect1.contains(p1) && rect2.contains(p2))
	{
		painter->drawLine(p1+ Dis, p2 +  Dis);
	}
	else if(!rect1.contains(p1) && rect2.contains(p2))
	{
		//p1=rect1.intersect
		//get intersection of line p1-p2 and rect1
		//painter->drawLine(p1,p2);
		float x1=p1.x(), y1=p1.y(), x2=p2.x(), y2=p2.y();
        int flag=CSLineClip(x1, y1, x2, y2, rect1.left(), rect1.right(), rect1.top(), rect1.bottom());
		if(flag==1)
			p1=QPointF(x1,y1);
		else if(flag==2)
			p1=QPointF(x2,y2);		
		else if(flag==0) //no intersection
		{
			 float x2=rect1.center().x(),y2=rect1.center().y();
		     int flag2=CSLineClip(x1, y1, x2, y2, rect1.left(), rect1.right(), rect1.top(), rect1.bottom());
			 if(flag2==1)
				p1=QPointF(x1,y1);
			 else if(flag2==2)
				p1=QPointF(x2,y2);
			 else 
				p1=QPointF(x1,y1);		
		}	
		else 
			p1=QPointF(x1,y1);
		
		
		painter->drawLine(p1+Dis, p2+Dis);
	}
	else if(rect1.contains(p1) && !rect2.contains(p2))
	{
		//p1=rect1.intersect
		//get intersection of line p1-p2 and rect1
		//painter->drawLine(p1,p2);
		float x1=p1.x(), y1=p1.y(), x2=p2.x(), y2=p2.y();
        int flag=CSLineClip(x1, y1, x2, y2, rect2.left(), rect2.right(), rect2.top(), rect2.bottom());
		if(flag==1)
			p2=QPointF(x1,y1);
		else if(flag==2)
			p2=QPointF(x2,y2);		
		else if(flag==0) //no intersection
		{
			 float x1=rect2.center().x(),y1=rect2.center().y();
		     int flag2=CSLineClip(x1, y1, x2, y2, rect2.left(), rect2.right(), rect2.top(), rect2.bottom());
			 if(flag2==1)
				p2=QPointF(x1,y1);
			 else if(flag2==2)
				p2=QPointF(x2,y2);
			 else 
				p2=QPointF(x2,y2);		
		}	
		else
			p2=QPointF(x2,y2);
		//painter->setPen(QColor(0,0,0,255));
		painter->drawLine(p1+Dis, p2+Dis);
	}
	else
	{	
		float x1=p1.x(), y1=p1.y(), x2=p2.x(), y2=p2.y();
	    float cx1=rect1.center().x(),cy1=rect1.center().y(), cx2=rect2.center().x(),cy2=rect2.center().y();
		int flag1,flag2;
		
		flag1=CSLineClip(cx1, cy1, x1, y1, rect1.left(), rect1.right(), rect1.top(), rect1.bottom());
		if(flag1==1)
			p1=QPointF(cx1,cy1);
		else if(flag1==2)
			p1=QPointF(x1,y1);
		else
			p1=QPointF(x1,y1);
		
		flag2=CSLineClip(cx2, cy2, x2, y2, rect2.left(), rect2.right(), rect2.top(), rect2.bottom());
		if(flag2==1)
			p2=QPointF(cx2,cy2);
		else if(flag2==2)
			p2=QPointF(x2,y2);
		else 
			p2=QPointF(x2,y2);
		
		//painter->setPen(QColor(0,0,0,255));
	    painter->drawLine(p1+Dis, p2+Dis);
	}
}


void PathBubble1::drawSharedPointers(QPainter *painter, QPointF dis, vector<int> item1, vector<int> item2)
{
	int pid2=item2[0], type2=item2[1], id2=item2[2];
	int pid1=item1[0], type1=item1[1], id1=item1[2];
	PathBubble1* path2=_scene->_pathBubbles[pid2];
	if(!path2)
		return;
	QRectF itemPos1,itemPos2; 
	switch(type1)
	{
		case 'C': itemPos1 = complexRect[id1];  break;
		case 'E': itemPos1 = physicalEntityRect[id1];  break;
		case 'P': itemPos1 = proteinRect[id1];  break;				
		case 'S': itemPos1 = smallMoleculeRect[id1];  break;
		case 'D': itemPos1 = DnaRect[id1];  break;
		case 'R': itemPos1 = reactionRect[id1];  break;
		case 'L': itemPos1 = ANodeRect[id1];  break;
		case 'M': itemPos1 = QRectF(compartmentRect[id1].x(),compartmentRect[id1].y(),2,2); break;
		case 'W': itemPos1 = QRectF(-realRect().width()/2+4, -realRect().height()/2+4, 2, 2);  break;
	}
	switch(type2)
	{
		case 'C': itemPos2 = path2->complexRect[id2];  break;
		case 'E': itemPos2 = path2->physicalEntityRect[id2];  break;
		case 'P': itemPos2 = path2->proteinRect[id2];  break;				
		case 'S': itemPos2 = path2->smallMoleculeRect[id2];  break;
		case 'D': itemPos2 = path2->DnaRect[id2];  break;
		case 'R': itemPos2 = path2->reactionRect[id2];  break;
		case 'L': itemPos2 = path2->ANodeRect[id2];  break;
		case 'M': itemPos2 = QRectF(path2->compartmentRect[id2].x(),path2->compartmentRect[id2].y(),2,2);  break;
		case 'W': itemPos2 = QRectF(-realRect().width()/2+4, -realRect().height()/2+4, 2, 2); break;
	}
	QPointF center1,center2;		
	QPen pen;
	pen.setColor( QColor(0, 0, 255, 200) );
	pen.setWidth(2);
	painter->setPen( pen );	

	if(itemPos1.width()>0 && itemPos2.width()>0)
    {
		center1 = itemPos1.center();
		center2 = itemPos2.center();
		drawABubbleConnection(painter, center1, this, center2, path2, dis);
	}	
}

/*
	_scene->pathVectors[pi][count].push_back(*it1);
										count++;
*/

void PathBubble1::drawConnections_H(QPainter *painter, QPointF dis)
{
	QPointF constart, conend, tmpmid;
	float height=this->Height()*_scale;
	float width=this->Width()*_scale;
	QPen pen;
	myColor3 color;
	//int width, height;
    vector<int> node;  
	vector<vector<int>> nodes;

	pen.setWidth(2);
	painter->save();

	//subgraph
	//highlight background
	set<vector<vector<int>>> connections=_scene->edgeconnections;	
	for(int i=0; i<4; i++)
	{
		switch(i)
		{
			case 0: connections=_scene->edgeconnections;  break;
		    case 1: connections=_scene->edgeconnections_A;  break;
			case 2: connections=_scene->edgeconnections_I;  break;
			case 3: connections=_scene->edgeconnections_4;  break;
		}
		for (set<vector<vector<int>>>::iterator it = connections.begin(); it != connections.end(); it++)
		{
			vector<vector<int>> link = *it;	
			bool isLinkedEdge=false;
			vector<int> link0=*link.begin(),link1=*link.rbegin();
			int pid1=link0[0], pid2=link1[0], type1=link0[1], type2=link1[1], id1=link0[2], id2=link1[2];
			
			if( pid1 == pid2 || (pid2!=m_pathwayID && pid1!=m_pathwayID)) 
				continue;

			vector<vector<int> > tEdge, rEdge;
			QRectF itemPos1,itemPos2; 
			PathBubble1* path1=_scene->_pathBubbles[pid1], * path2=_scene->_pathBubbles[pid2];

			if(!path1 || !path2)
				continue;

			if(!path1->isVisible() || !path2->isVisible())
				continue;

			
			//int pid1, int type1, int id1, int pid2, int type2, int id2
			//if(_scene->linkSearchType == 1)	
		
			tEdge.push_back(link0);  tEdge.push_back(link1);	
			rEdge.push_back(link1);  rEdge.push_back(link0);	//keqin: check the edge direction recorded in linkededge later
			for(int j=0; j<linkedEdge.size();j++)
			{
				if(linkedEdge[j].find(tEdge)!=linkedEdge[j].end() || linkedEdge[j].find(rEdge)!=linkedEdge[j].end())
				{
					isLinkedEdge=true;				
				   break;
				}
			}			
			for(int k=0; k<_scene->_pathBubbles[pid2]->linkedEdge.size();k++)
			{
				if(_scene->_pathBubbles[pid2]->linkedEdge[k].find(tEdge)!=_scene->_pathBubbles[pid2]->linkedEdge[k].end()
				  ||_scene->_pathBubbles[pid2]->linkedEdge[k].find(rEdge)!=_scene->_pathBubbles[pid2]->linkedEdge[k].end())
				{
					 isLinkedEdge=true;				 
					 break;
				}
			}			
			if(!isLinkedEdge)
			for(int j=0; j<_scene->_pathBubbles[pid1]->linkedEdge.size();j++)
			{
				if(_scene->_pathBubbles[pid1]->linkedEdge[j].find(tEdge)!=_scene->_pathBubbles[pid1]->linkedEdge[j].end()
					||_scene->_pathBubbles[pid1]->linkedEdge[j].find(rEdge)!=_scene->_pathBubbles[pid1]->linkedEdge[j].end())
				{
					 isLinkedEdge=true;				 
					 break;
				}
			}	
			QColor bc;
			if(isLinkedEdge)
			{
				/*float level=findLinkedLevel(pid1, type1, id1, pid2, type2, id2);
				float totalLevel=getTotalLinkLevel();			
				if(level<0)
				{
					level=findLinkedLevel(pid2, type2, id2, pid1, type1, id1);				
				}*/			
				bc=_scene->getPathColor(pid1,  type1,  id1,  pid2,  type2,  id2); //assignColorToLinkedItems(level, totalLevel,1);			
				painter->setBrush(bc);					
				painter->setPen(bc);					
			}
			else 
			{
				continue;
			}
            float h=0;		    
			switch(type1)
			{
				case 'C': itemPos1 = path1->complexRect[id1]; break;
				case 'E': itemPos1 = path1->physicalEntityRect[id1];  break;
				case 'P': itemPos1 = path1->proteinRect[id1];  break;				
				case 'S': itemPos1 = path1->smallMoleculeRect[id1];  break;
				case 'D': itemPos1 = path1->DnaRect[id1];  break;
				case 'R': itemPos1 = path1->reactionRect[id1];  break;
				case 'L': itemPos1 = path1->ANodeRect[id1];  break;
			}
			switch(type2)
			{
				case 'C': itemPos2 = path2->complexRect.empty()? path2->complexPos[id2]:path2->complexRect[id2];  break;
				case 'E': itemPos2 = path2->physicalEntityRect.empty()? path2->physicalEntityPos[id2]:path2->physicalEntityRect[id2];  break;
				case 'P': itemPos2 = path2->proteinRect.empty()? path2->proteinPos[id2]:path2->proteinRect[id2];  break;				
				case 'S': itemPos2 = path2->smallMoleculePos.empty()? path2->smallMoleculePos[id2]:path2->smallMoleculeRect[id2];  break;
				case 'D': itemPos2 = path2->DnaRect.empty()?path2->DnaPos[id2]:path2->DnaRect[id2];  break;
				case 'R': itemPos2 = path2->reactionRect.empty()? path2->reactionPos[id2]:path2->reactionRect[id2];  break;
				case 'L': itemPos2 = path2->ANodeRect.empty()? path2->ANodePos[id2]:path2->ANodeRect[id2];  break;
			}
			QPointF center1, center2;			
			if(itemPos1.width()>0 && itemPos2.width()>0)
			{
				center1 = itemPos1.center();
				center2 = itemPos2.center();
				if(i<3)
				{
					if(!inUpdatedEdge(link, _scene->edgeconnections_4))
					{
						drawABubbleConnectioninArrow_H(painter, center1, path1, center2, path2, dis, bc, i, _scale);	
					}
				}
				else
				{					 
					 int encode;
					 if(h!=0)
					 {
					    h=(itemPos1.height()+itemPos2.height())/2;
					 }
					 else 
						 h=10;
				 
					  int eid=0;
					  for(set<vector<vector<int>>>::iterator it=_scene->edgeconnections_4.begin(); it!=_scene->edgeconnections_4.end(); it++)
					  {
						vector<vector<int> > rlink;
						rlink.push_back(link[1]); rlink.push_back(link[0]);
						if(*it==link || *it==rlink)					
							break;
						eid++;
					  }
					  encode=_scene->edgeconnections_5[eid];					 					  
					  drawABubbleConnectioninArrow_H(painter, center1, path1, center2, path2, dis, bc, 3, encode, h, _scale);						
				}
			}		
		}	
	}
    painter->restore();
}

void PathBubble1::drawConnections(QPainter *painter, QPointF dis)
{
	QPointF constart, conend, tmpmid;
	float height=this->Height()*_scale;
	float width=this->Width()*_scale;
	QPen pen;
	myColor3 color;
	//int width, height;
    vector<int> node;  
	vector<vector<int>> nodes;

	pen.setWidth(2);
	painter->save();

	//subgraph
	//highlight background
	set<vector<vector<int>>> connections=_scene->edgeconnections;	
	//linkage
	for(int i=0; i<4; i++)
	{
		switch(i)
		{
			case 0: connections=_scene->edgeconnections;  break;
		    case 1: connections=_scene->edgeconnections_A;  break;
			case 2: connections=_scene->edgeconnections_I;  break;
			case 3: connections=_scene->edgeconnections_4;  break;
		}
		for (set<vector<vector<int>>>::iterator it = connections.begin(); it != connections.end(); it++)
		{
			vector<vector<int>> link = *it;	
			//bool isLinkedEdge=false;
			vector<int> link0=*link.begin(),link1=*link.rbegin();
			int pid1=link0[0], pid2=link1[0], type1=link0[1], type2=link1[1], id1=link0[2], id2=link1[2];
			vector<vector<int> > tEdge;//, rEdge;

			if( pid1 == pid2 || (pid2!=m_pathwayID && pid1!=m_pathwayID) )
				continue;
			
			QRectF itemPos1,itemPos2; 
			PathBubble1* path1=_scene->_pathBubbles[pid1], * path2=_scene->_pathBubbles[pid2];

			if(!path1 || !path2)
				continue;

			if(!path1->isVisible() || !path2->isVisible())
				continue;
		
			tEdge.push_back(link0);  tEdge.push_back(link1);	

			/*
			for(int j=0; j<linkedEdge.size();j++)
			{
				if(linkedEdge[j].find(tEdge)!=linkedEdge[j].end())
				{
					isLinkedEdge=true;				
				   break;
				}
			}			
			for(int k=0; k<_scene->_pathBubbles[pid2]->linkedEdge.size();k++)
			{
				if(_scene->_pathBubbles[pid2]->linkedEdge[k].find(tEdge)!=_scene->_pathBubbles[pid2]->linkedEdge[k].end())
				{
					 isLinkedEdge=true;				 
					 break;
				}
			}			
			if(!isLinkedEdge)
			for(int j=0; j<_scene->_pathBubbles[pid1]->linkedEdge.size();j++)
			{
				if(_scene->_pathBubbles[pid1]->linkedEdge[j].find(tEdge)!=_scene->_pathBubbles[pid1]->linkedEdge[j].end())
				{
					 isLinkedEdge=true;				 
					 break;
				}
			}
			*/

			QColor bc;			
			
			bc=QColor(40, 40, 40, 255);
			painter->setPen(bc);		
			painter->setBrush(bc );
			
			switch(type1)
			{
				case 'C': itemPos1 = path1->complexRect[id1];  break;
				case 'E': itemPos1 = path1->physicalEntityRect[id1];  break;
				case 'P': itemPos1 = path1->proteinRect[id1];  break;				
				case 'S': itemPos1 = path1->smallMoleculeRect[id1];  break;
				case 'D': itemPos1 = path1->DnaRect[id1];  break;
				case 'R': itemPos1 = path1->reactionRect[id1];  break;
				case 'L': itemPos1 = path1->ANodeRect[id1];  break;
			}
			switch(type2)
			{
				case 'C': itemPos2 = path2->complexRect.empty()? path2->complexPos[id2]:path2->complexRect[id2];  break;
				case 'E': itemPos2 = path2->physicalEntityRect.empty()? path2->physicalEntityPos[id2]:path2->physicalEntityRect[id2];  break;
				case 'P': itemPos2 = path2->proteinRect.empty()? path2->proteinPos[id2]:path2->proteinRect[id2];  break;				
				case 'S': itemPos2 = path2->smallMoleculePos.empty()? path2->smallMoleculePos[id2]:path2->smallMoleculeRect[id2];  break;
				case 'D': itemPos2 = path2->DnaRect.empty()?path2->DnaPos[id2]:path2->DnaRect[id2];  break;
				case 'R': itemPos2 = path2->reactionRect.empty()? path2->reactionPos[id2]:path2->reactionRect[id2];  break;
				case 'L': itemPos2 = path2->ANodeRect.empty()? path2->ANodePos[id2]:path2->ANodeRect[id2];  break;
			}
			QPointF center1, center2;			
			if(itemPos1.width()>0 && itemPos2.width()>0)
			{
				center1 = itemPos1.center();
				center2 = itemPos2.center();
				if(i<3)
				{
					if(!inUpdatedEdge(link, _scene->edgeconnections_4))
					{
						drawABubbleConnectioninArrow(painter, center1, path1, center2, path2, dis, bc, i);						
					}
				}
				else
				{
					 float h;
					 int encode;
					 /*if( type1 == 'L')
						  h=_scene->_pathBubbles[pid1]->ANodeRect[id1].height();
					 else if( type2 == 'L')
						  h=_scene->_pathBubbles[pid2]->ANodeRect[id2].height();*/
					if(h!=0)
					{
						h=(itemPos1.height()+itemPos2.height())/2;
					}
					else 
						h=10;
				 
					  int eid=0;
					  for(set<vector<vector<int>>>::iterator it=_scene->edgeconnections_4.begin(); it!=_scene->edgeconnections_4.end(); it++)
					  {
						vector<vector<int> > rlink;
						rlink.push_back(link[1]); rlink.push_back(link[0]);
						if(*it==link || *it==rlink)					
							break;
						eid++;
					  }
					  encode=_scene->edgeconnections_5[eid];
					  
					  drawABubbleConnectioninArrow(painter, center1, path1, center2, path2, dis, bc, 3, encode, h);	 //drawCurvedArrow(painter, Code, ANodeRect[Aid].height(), start, end, width, height);						 
				
				}
			}		
		}	
	}
	for (int i = 0; i < _codeBubbles.size(); i ++)
	{
		color = this->_codeBubbles[i]->getColor();
		
		pen.setColor( QColor(0, 0, 0, 200) );
		painter->setPen( pen );

		constart = this->_conPoint[i];
		constart.setX( this->Width()/2 + dis.x());
		constart.setY( constart.y()*height - height/2 + dis.y());
		constart=constart+dCenter;
		
		conend = _codeBubbles[i]->pos();
		conend.setX( conend.x() - _codeBubbles[i]->Width()/2 + dis.x());
		conend.setY( conend.y() + dis.y());
		conend -= this->pos();

		drawLinkArrow( painter, constart, conend);
		if(i<_conhighlight.size())
		{
			set<vector<int>> hi=_conhighlight[i];
			for(set<vector<int>>::iterator it=hi.begin(); it!=hi.end(); it++) 
			{
				vector<int> temp=*it;
				paintItem(painter, temp[0], temp[1], true, dCenter, _scale, QColor(color.a, color.b, color.c, 200), dis);			
			}	
		}
	}
	
	for (int i = 0; i < _reactionBubbles.size(); i ++)
	{
		color = this->_reactionBubbles[i]->getColor();
		pen.setColor( QColor(color.a, color.b, color.c, 200) );
		painter->setPen( pen );

		constart = this->_reactionPoint[i];
		constart.setX( this->Width()/2 + dis.x());
		constart.setY( constart.y()*height - height/2 + dis.y() );
		constart=constart+dCenter;

		conend = _reactionBubbles[i]->pos();
		conend.setX( conend.x() - _reactionBubbles[i]->Width()/2 + dis.x());
		conend.setY( conend.y() + dis.y());
		conend -= this->pos();
	
		float width = conend.x() - constart.x();

		tmpmid.setX( constart.x() + width/2 );
		tmpmid.setY( constart.y() );

		if ( fabs(conend.y()-constart.y()) > 20)
		{
			painter->drawLine( constart, tmpmid - QPointF( 10.0, 0) );

			if ( conend.y() > constart.y() )
			{
				painter->drawArc( tmpmid.x()-20.0, tmpmid.y(), 20.0, 20.0, 0, 90*16 );
				painter->drawLine( tmpmid+QPointF( 0, 10.0), QPointF(tmpmid.x(), conend.y()-10.0));
				tmpmid.setY( conend.y() );
				painter->drawArc( tmpmid.x(), tmpmid.y()-20.0, 20.0, 20.0, 180*16, 90*16 );
			}
			else
			{
				painter->drawArc( tmpmid.x()-20.0, tmpmid.y()-20.0, 20.0, 20.0, 270*16, 90*16 );
				painter->drawLine( tmpmid-QPointF( 0, 10.0), QPointF(tmpmid.x(), conend.y()+10.0));
				tmpmid.setY( conend.y() );
				painter->drawArc( tmpmid.x(), tmpmid.y(), 20.0, 20.0, 90*16, 90*16 );
			}
			painter->drawLine( tmpmid + QPointF( 10.0, 0), conend );
		}
		else
		{
			painter->drawLine( constart, tmpmid);
			painter->drawLine( tmpmid, QPointF(tmpmid.x(), conend.y()));
			tmpmid.setY( conend.y() );
			painter->drawLine( tmpmid, conend );
		}
		
		//arrow
		if (conend.x() > constart.x())
		{
			painter->drawLine( conend, QPointF(conend.x()-10, conend.y()-4) );
			painter->drawLine( conend, QPointF(conend.x()-10, conend.y()+4) );
		}
		else
		{
 			painter->drawLine( conend, QPointF(conend.x()+10, conend.y()-4) );
 			painter->drawLine( conend, QPointF(conend.x()+10, conend.y()+4) );
		}	

		if(i<_reactionhighlight.size())
		{
			set<vector<int>> hi=_reactionhighlight[i];
		    for(set<vector<int>>::iterator it=hi.begin(); it!=hi.end(); it++) 
		    {
			vector<int> temp=*it;
            paintItem(painter, temp[0], temp[1], true, dCenter, _scale, QColor(color.a, color.b, color.c, 200), dis);					
		    }	
		}
	}
	
	for (int i = 0; i < _groupBubbles.size(); i ++)
	{
		color = this->_groupBubbles[i]->getColor();
		pen.setColor( QColor(color.a, color.b, color.c, 200) );
		painter->setPen( pen );

		constart = this->_groupPoint[i];
		constart.setX( this->Width()/2 + dis.x());
		constart.setY( constart.y()*height - height/2 + dis.y());
		constart=constart+dCenter;

		conend = _groupBubbles[i]->pos();
		conend.setX( conend.x() - _groupBubbles[i]->Width()/2 + dis.x());
		conend.setY( conend.y() + dis.y());
		conend -= this->pos();
	
		int wid = conend.x() - constart.x();

		tmpmid.setX( constart.x() + wid/2 );
		tmpmid.setY( constart.y() );

		if(i<_grouphighlight.size())  
		{
            int j=0;
			vector<int> node(2,0), temp;
			vector<vector<int>> ed(2, node);
			set<vector<int>> hi=_grouphighlight[i];

			set<vector<vector<int>>> Af=_groupEdgeAffected[i];

			for(set<vector<int>>::iterator it=hi.begin(); it!=hi.end(); it++) 
			{
				vector<int> temp=*it;				
								
				QPointF center,center1;
				QRectF rect;
		        switch(temp[0])
		        {
					case 'C': nodes = _complex[temp[1]]; break;
					case 'D': nodes = _Dna[temp[1]]; break;
					case 'E': nodes = _entity[temp[1]]; break;
					case 'L': nodes = _ANode[temp[1]]; break;
					case 'P': nodes = _protein[temp[1]]; break;
					case 'S': nodes = _smallMolecule[temp[1]]; break;
					case 'R': nodes = _reaction[temp[1]]; break;
				}
				center1=this->_groupBubbles[i]->itemPos[j].center();
				ed[0][0]=temp[0];  ed[0][1]=temp[1];

				for(int k=0; k<nodes.size(); k++)
				{	
					int it=nodes[k][1];
					switch(nodes[k][0])
					{
						case 'C': rect=complexPos[it]; break;
						case 'D': rect=DnaPos[it]; break;
						case 'E': rect=physicalEntityPos[it]; break;
						case 'L': rect=ANodePos[it]; break;
						case 'P': rect=proteinPos[it]; break;
						case 'S': rect=smallMoleculePos[it]; break;
						case 'R': rect=reactionPos[it]; break;
					}					
					if(rect.x()>-999.99)
					{
						center=rect.center();	
					
						center.setX(center.x()*width-width/2);
						center.setY(center.y()*height-height/2);

						ed[1][0]=nodes[k][0];  ed[1][1]=nodes[k][1];

						if(nodes[k][3]==-1)
						{
			                 temp=ed[1]; ed[1]=ed[0]; ed[0]=temp;
						}

						if(center.x()>=-width*1.3&&center1.x()>=-width*1.3)			
						{
							if(Af.find(ed)!=Af.end())
								painter->drawLine(center+dCenter, center1+this->_groupBubbles[i]->sceneBoundingRect().center()-this->sceneBoundingRect().center());
						}
					}				
				}				
				j++;
			}
		}		
	}
	painter->restore();
}


void PathBubble1::recordPathBubbleConnections()
{//record new connections and update the previous ones

	QPointF constart, conend, tmpmid;

	float height=this->Height()*_scale;
	float width=this->Width()*_scale;

	//edgeconnections.clear();
    vector<int> temp(3,0);  
    vector<vector<int>> link(2,temp);

	//vector<vector<int>> newlink;
	set<vector<vector<int>>> newlinkVector,alinkVector,ilinkVector,linkVector4;

    vector<int> node;  
	vector<vector<int>> nodes;
	
	int size=_pathBubbles.size();	
	for (int i = 0; i < _pathBubbles.size(); i ++)
	{
		if(_pathBubbles[i])
		{
			constart = this->_groupPoint[i];
			constart.setX( this->Width()/2 );
			constart.setY( constart.y()*height - height/2);
			constart=constart+dCenter;

			conend = _pathBubbles[i]->pos();
			conend.setX( conend.x() - _pathBubbles[i]->Width()/2);
			conend -= this->pos();
	
			int wid = conend.x() - constart.x();

			tmpmid.setX( constart.x() + wid/2 );
			tmpmid.setY( constart.y() );

			if(i<_grouphighlight.size())  
			{
				int j=0;
			
				vector<int> node(3,0), temp;
				vector<vector<int>> ed(2, node);
				vector<vector<int>> ed2(2, node);
				set<vector<int>> hi=_grouphighlight[i];

				set<vector<vector<int>>> Af=_groupEdgeAffected[i];

				vector<QRectF> itemPos;

				vector<int> ID1, ID2,type2;
				//vector<int> id1;
				vector<PathBubble1*> blist;
				for(int k=0; k< this->_pathBubbles[i]->itemInfo.size(); k++)
				{				
					int type= this->_pathBubbles[i]->itemInfo[k][0], id1=this->_pathBubbles[i]->itemInfo[k][1], id2=this->_pathBubbles[i]->itemInfo[k][2], pid=this->_pathBubbles[i]->itemInfo[k][3], jid=this->_pathBubbles[i]->itemInfo[k][4];
					jid=k;
					PathBubble1* tmp= searchSubPathBubble(this->_pathBubbles[i],pid,jid);
					int pid2=tmp->m_pathwayID;
					if(pid>=0)					
					{
						if(jid>=0 && jid<tmp->itemInfo.size())
						{
						   switch(type)
						   {
								case 'C': itemPos.push_back(tmp->complexPos[tmp->itemInfo[jid][2]]);  break;
								case 'E': itemPos.push_back(tmp->physicalEntityPos[tmp->itemInfo[jid][2]]);  break;
								case 'L': itemPos.push_back(tmp->ANodePos[tmp->itemInfo[jid][2]]);  break;
								case 'P': itemPos.push_back(tmp->proteinPos[tmp->itemInfo[jid][2]]);  break;				
								case 'S': itemPos.push_back(tmp->smallMoleculePos[tmp->itemInfo[jid][2]]);  break;
								case 'D': itemPos.push_back(tmp->DnaPos[tmp->itemInfo[jid][2]]);  break;
								case 'R': itemPos.push_back(tmp->reactionPos[tmp->itemInfo[jid][2]]);  break;
								//case 'M': itemPos.push_back(tmp->compartmentPos[tmp->itemInfo[jid][1]]);  break;
						   }	
							blist.push_back(tmp);
							type2.push_back(type);
							ID1.push_back(id1);														
							ID2.push_back(tmp->itemInfo[jid][2]);														
						}					
					}
					else 
					{
						if(id2>=0)
						{
							 switch(type)
							 {
								   case 'C': itemPos.push_back(this->_pathBubbles[i]->complexPos[id2]);  break;
								   case 'E': itemPos.push_back(this->_pathBubbles[i]->physicalEntityPos[id2]);  break;
								   case 'L': itemPos.push_back(this->_pathBubbles[i]->ANodePos[id2]);  break;	
								   case 'P': itemPos.push_back(this->_pathBubbles[i]->proteinPos[id2]);  break;				
								   case 'S': itemPos.push_back(this->_pathBubbles[i]->smallMoleculePos[id2]);  break;
								   case 'D': itemPos.push_back(this->_pathBubbles[i]->DnaPos[id2]);  break;
								   case 'R': itemPos.push_back(this->_pathBubbles[i]->reactionPos[id2]);  break;
							 }	
							 blist.push_back(tmp);
							 type2.push_back(type);
							 ID1.push_back(id1);							 
							 ID2.push_back(id2);							 
						}
					}				
				}	

				int count=0;
				for(set<vector<int>>::iterator it=hi.begin(); it!=hi.end(); it++) 
				{
					vector<int> temp=*it;

					//find match j
				    //temp[0] temp[1]
				    
					if(temp[0]!='M')
					{
					//paintItem(painter, temp[0], temp[1], true, QColor(color.a, color.b, color.c, 200));		
						QPointF center,center1;
						QRectF rect;
				
						if(temp[1]>=0)
						switch(temp[0])
						{
							case 'C': nodes = _complex[temp[1]]; break;
							case 'D': nodes = _Dna[temp[1]]; break;
							case 'E': nodes = _entity[temp[1]]; break;
							case 'L': nodes = _ANode[temp[1]]; break;
							case 'P': nodes = _protein[temp[1]]; break;
							case 'S': nodes = _smallMolecule[temp[1]]; break;
							case 'R': nodes = _reaction[temp[1]]; break;
						}
				
						ed2[0][0]=ed[1][0]=ed[0][0]=ed2[0][0]=m_pathwayID;					
						ed[0][1]=temp[0];  ed[0][2]=temp[1];
						ed2[0][1]=temp[1];  ed2[0][2]=temp[0];
					
						for(int k=0; k<nodes.size(); k++)
						{	
							int it=nodes[k][1], type=nodes[k][0];
							if(it>=0)
							switch(nodes[k][0])
							{
								case 'C': rect=complexPos[it]; break;
								case 'D': rect=DnaPos[it]; break;
								case 'E': rect=physicalEntityPos[it]; break;
								case 'L': rect=ANodePos[it]; break;
								case 'P': rect=proteinPos[it]; break;
								case 'S': rect=smallMoleculePos[it]; break;
								case 'R': rect=reactionPos[it]; break;
							}					
							if(it>=0)//&&rect.x()>-999.99)
							{
								center=rect.center();						
								center.setX(center.x()*width-width/2);
								center.setY(center.y()*height-height/2);
								ed[1][1]=nodes[k][0];  ed[1][2]=nodes[k][1];						
								ed2[1]=ed[0];  ed2[0]=ed[1];														
								{
									//if((Af.find(ed)!=Af.end()||Af.find(ed2)!=Af.end())&& j<type2.size()  )									
									if((Af.find(ed)!=Af.end()||Af.find(ed2)!=Af.end())&& j<type2.size()  )									
									{

										//find j according to temp[0], and temp[1]
										for(j=0; j<ID1.size(); j++)
										{
										      if(type2[j]==temp[0] && ID1[j]==temp[1])
												  break;										        
										}
										if(j==ID1.size())
											continue;  // may have bug here


										for(int m=0; m<2; m++)
										{
											bool flag=false;
											if(m==0)
											if(Af.find(ed2)!=Af.end())
											{
												link[0][0]=m_pathwayID; link[0][1]=type;  link[0][2]=it;
												link[1][0]=blist[j]->m_pathwayID; link[1][1]=type2[j];  link[1][2]=ID2[j];
												flag=true;
											}

											if(m==1)
											if(Af.find(ed)!=Af.end())
											{
												link[1][0]=m_pathwayID; link[1][1]=type;  link[1][2]=it;
												link[0][0]=blist[j]->m_pathwayID; link[0][1]=type2[j];  link[0][2]=ID2[j];
												flag=true;
											}		                               		
											if(flag)
											{
												vector<int> tmp;
												tmp.push_back(type); tmp.push_back(it);
												if(reactionRecord.find(tmp)!=reactionRecord.end())
												{
													if(m==0)
													{
														if(updatedEdge_1.find(ed2)!=updatedEdge_1.end()) 
														{
															newlinkVector.insert(link);   
														}
														if(updatedEdge_2.find(ed2)!=updatedEdge_2.end()) 
														{
															alinkVector.insert(link); 
															//if(updatedEdge_4.find(ed2)==updatedEdge_4.end())
															//   linkVector4.insert(link);  
															
															
														}
														if(updatedEdge_3.find(ed2)!=updatedEdge_3.end())
														{
															ilinkVector.insert(link); 
															
															//if(updatedEdge_4.find(ed2)==updatedEdge_4.end())
															//   linkVector4.insert(link);  
															
														}
														if(updatedEdge_4.find(ed2)!=updatedEdge_4.end())
														{
															  linkVector4.insert(link);
														}
													}
													else 
													{
														if(updatedEdge_1.find(ed)!=updatedEdge_1.end()) 
														{
															newlinkVector.insert(link);   
														}														
														if(updatedEdge_2.find(ed)!=updatedEdge_2.end()) 
														{
															alinkVector.insert(link);
															
															//if(updatedEdge_4.find(ed)==updatedEdge_4.end())
															//   linkVector4.insert(link);  
														}
														if(updatedEdge_3.find(ed)!=updatedEdge_3.end())
														{
															ilinkVector.insert(link); 
															//if(updatedEdge_4.find(ed)==updatedEdge_4.end())
															//   linkVector4.insert(link);  
															
														}
														if(updatedEdge_4.find(ed)!=updatedEdge_4.end())
														{
															  linkVector4.insert(link); 
															 
														}
													}
												}
											}
										}
								
									}
								}
							}				
						}				
						//j++; //hi
					}
				}
			}	
		}
	}	

	
    int Cnum,Enum,Pnum,Snum,Dnum,Rnum;
	Cnum=Enum=Pnum=Snum=Dnum=Rnum=0;
	//update according to the moved items //itemselected
	for(int i=0; i<itemSelected.size(); i+=2)
	{
		vector<int> link,link1;   //link seleted item, link1 new place in the new bubble
		int type=itemSelected[i];
		int id=itemSelected[i+1],id1;
		switch(type)
		{
		    case 'C': id1=Cnum++;  break;
		    case 'E': id1=Enum++;  break;
		    case 'P': id1=Pnum++;  break;				
		    case 'S': id1=Snum++;  break;
		    case 'D': id1=Dnum++;  break;
		    case 'R': id1=Rnum++;  break;
		}		
				
		link.push_back(m_pathwayID);  link.push_back(type); link.push_back(id);//old
		link1.push_back(_scene->_pathBubbles.size()-1);  link1.push_back(type); link1.push_back(id1);
		
		
		vector<vector<vector<int>>> linkRecord;	
		set<vector<vector<int>>>::iterator it;
		for(it = _scene->edgeconnections.begin(); it != _scene->edgeconnections.end(); it++)
		{
		     vector<vector<int>> plink = *it;
			 //vector<vector<int>>::iterator id=plink.find(link1);
		     //if(id!=plink.end())
			 if(plink[0]==link || plink[1]==link)
			 {
			     linkRecord.push_back(plink);				 
			 }			
		}
		
		for(int j=0; j<linkRecord.size(); j++)
		{
			 vector<vector<int>> plink=linkRecord[j];
			 _scene->edgeconnections.erase(plink);			  
             
			 //plink.erase(link1); 
			 int od=-1;
			 if(plink[0]==link)
				 od=1;
			 else if(plink[1]==link)
				 od=0;

			 //vector<int> link2=*plink.begin(); 
			 if(od!=-1)
			 {
				 vector<int> link2=plink[od]; 
				 if(link[0]!=link2[0]) 
				 {
					 //plink.insert(link); //add the oriental item back and connected with link2
					 plink[1-od]=link1;
					 _scene->edgeconnections.insert(plink);
				 }
			 }
		}		
		
		
	}
    
	//for(int i=0; i<newlinkVector.size(); i++)
	for(set<vector<vector<int>>>::iterator it=newlinkVector.begin(); it!=newlinkVector.end(); it++)
	{
		vector<vector<int>> edge=*it;
		//vector<int> link=edge[0],link1=edge[1];		
	    //vector<vector<int>> nlink;
		//nlink.push_back(link), nlink.push_back(link1);		
		_scene->edgeconnections.insert(edge);		
	}  
	
	//for(int i=0; i<alinkVector.size(); i++)
	for(set<vector<vector<int>>>::iterator it=alinkVector.begin(); it!=alinkVector.end(); it++)
	{
		vector<vector<int>> edge=*it;
		//vector<int> olink=edge[0], olink1=edge[1];
	    //vector<vector<int>> aLink;		
		//aLink.push_back(olink), aLink.push_back(olink1);
		_scene->edgeconnections_A.insert(edge);		
	}  
	
	//for(int i=0; i<ilinkVector.size(); i++)
	for(set<vector<vector<int>>>::iterator it=ilinkVector.begin(); it!=ilinkVector.end(); it++)
	{
		vector<vector<int>> edge=*it;
		//vector<int> olink=edge[0], olink1=edge[1];
	    //vector<vector<int>> iLink;		
		//iLink.push_back(olink), iLink.push_back(olink1);
		_scene->edgeconnections_I.insert(edge);		
	}  


	for(set<vector<vector<int>>>::iterator it=linkVector4.begin(); it!=linkVector4.end(); it++)
	{
		vector<vector<int>> edge=*it;
		//vector<int> olink=edge[0], olink1=edge[1];
	    //vector<vector<int>> iLink;		
		//iLink.push_back(olink), iLink.push_back(olink1);
		_scene->edgeconnections_4.insert(edge);		
	}  
		
	_scene->edgeconnections_5.clear();
	for(set<vector<vector<int> > >::iterator it=_scene->edgeconnections_4.begin(); it!=_scene->edgeconnections_4.end(); it++)
	{
		vector<vector<int> > edge=*it, redge;
		redge.push_back(edge[1]);  redge.push_back(edge[0]);
	    int encode=0;
		if( _scene->edgeconnections.find(edge)!= _scene->edgeconnections.end() )
		{
		    encode|=1;
		}
		if( _scene->edgeconnections.find(redge)!= _scene->edgeconnections.end() )
		{
		    encode|=2;
		}
		if( _scene->edgeconnections_A.find(edge)!= _scene->edgeconnections_A.end() )
		{
		    encode|=4; 
		}
		if( _scene->edgeconnections_A.find(redge)!= _scene->edgeconnections_A.end() )
		{
		    encode|=8;
		}
		if( _scene->edgeconnections_I.find(edge)!= _scene->edgeconnections_I.end() ) 
		{
		    encode|=16; 		
		}	
		if( _scene->edgeconnections_I.find(redge)!= _scene->edgeconnections_I.end() )
		{
		    encode|=32; 
		}
		_scene->edgeconnections_5.push_back(encode);
	}
}


bool PathBubble1::getPartItemBack(PathBubble1 *cBubble, vector<int> partItem)
{
	//
	set<vector<int>> itemSet;
	for(int i=0; i<partItem.size(); i=i+2)
	{
		vector<int> item;
		item.push_back(partItem[i]);  item.push_back(partItem[i+1]);
		itemSet.insert(item);
	}
	
	//recover the selected nodes
	//int Cnum,Enum,Pnum,Snum,Dnum,Rnum, Anum;
	//Cnum=Enum=Pnum=Snum=Dnum=Rnum=Anum=0;
	QRectF rect;
	float x,y,h,w;
	int pid0=m_pathwayID, pid1=cBubble->m_pathwayID;// pid0:parent, pid1: son
	PathBubble1* B0=_scene->_pathBubbles[pid0], *B1=_scene->_pathBubbles[pid1];
	subPathBubble1* sB = dynamic_cast<subPathBubble1*>(B1);
	QPointF Dis=sB->Dis;
	
	for(int i=0; i<B1->itemInfo.size(); i++)
	{
		int type=B1->itemInfo[i][0];//B1->_itemSelected[i];
		int id0=B1->itemInfo[i][1], id1=B1->itemInfo[i][2];//B1->_itemSelected[i+1];	
		vector<int> item;
		item.push_back(type); item.push_back(id1);
		if(pid0==B1->itemInfo[i][5])
		switch(type)
		{
		    case 'C': // item.push_back(Cnum);
				      if(itemSet.find(item)!=itemSet.end())
					  {
						  CalloutNote* nodeNote=_scene->findNote(pid1, type, id1);	if(nodeNote!=NULL)  nodeNote->_pid=pid0, nodeNote->_id=id0;
						  rect=_scene->_complexPos[pid1][id1];
				          rect.moveCenter(rect.center() + Dis);
				          B0->complexPos[id0]=rect;
					  }//Cnum++; 
					  break;
		    case 'E': //item.push_back(Enum);
				      if(itemSet.find(item)!=itemSet.end())
					  {
						   CalloutNote* nodeNote=_scene->findNote(pid1, type, id1);	if(nodeNote!=NULL)  nodeNote->_pid=pid0, nodeNote->_id=id0;
						  rect=_scene->_physicalEntityPos[pid1][id1];
						  rect.moveCenter(rect.center() + Dis);
						  B0->physicalEntityPos[id0] =  rect;  			
					  }//  Enum++; 
					  break; 
			case 'L': //item.push_back(Anum);
				      if(itemSet.find(item)!=itemSet.end())
					  {
						   CalloutNote* nodeNote=_scene->findNote(pid1, type, id1);	if(nodeNote!=NULL)  nodeNote->_pid=pid0, nodeNote->_id=id0;
						  rect=_scene->_ANodePos[pid1][id1];
						  rect.moveCenter(rect.center() + Dis);
						  B0->ANodePos[id0] = rect; 
					  }// Anum++; 
					  break; 
		    case 'P': //item.push_back(Pnum);
				      if(itemSet.find(item)!=itemSet.end())
					  {
						   CalloutNote* nodeNote=_scene->findNote(pid1, type, id1);	if(nodeNote!=NULL)  nodeNote->_pid=pid0, nodeNote->_id=id0;
						  rect = _scene->_proteinPos[pid1][id1]; 					  
						  rect.moveCenter(rect.center() + Dis);
						  B0->proteinPos[id0] = rect;
					  }  //Pnum++; 
					  break; 
		    case 'S': //item.push_back(Snum);
				      if(itemSet.find(item)!=itemSet.end())
					  {
						   CalloutNote* nodeNote=_scene->findNote(pid1, type, id1);	if(nodeNote!=NULL)  nodeNote->_pid=pid0, nodeNote->_id=id0;
						  rect= _scene->_smallMoleculePos[pid1][id1]; 
						   rect.moveCenter(rect.center() + Dis);
						   B0->smallMoleculePos[id0]=rect;
					  } //Snum++; 
					  //break; 
		    case 'D': //item.push_back(Dnum);
				      if(itemSet.find(item)!=itemSet.end())
					  {
						   CalloutNote* nodeNote=_scene->findNote(pid1, type, id1);	if(nodeNote!=NULL)  nodeNote->_pid=pid0, nodeNote->_id=id0;
						 rect= _scene->_DnaPos[pid1][id1];  
				         rect.moveCenter(rect.center() + Dis);
					     B0->DnaPos[id0] = rect;
					  }// Dnum++; 
					  break;  
		    case 'R': //item.push_back(Rnum);
				      if(itemSet.find(item)!=itemSet.end())
					  {
						   CalloutNote* nodeNote=_scene->findNote(pid1, type, id1);	if(nodeNote!=NULL)  nodeNote->_pid=pid0, nodeNote->_id=id0;
						  rect = _scene->_reactionPos[pid1][id1];
				          rect.moveCenter(rect.center() + Dis);
					      B0->reactionPos[id0] = rect ;
					  } //Rnum++; 
					  break; 
			case 'M': //item.push_back(id1);
				      if(itemSet.find(item)!=itemSet.end())
					  {
						   CalloutNote* nodeNote=_scene->findNote(pid1, type, id1);	if(nodeNote!=NULL)  nodeNote->_pid=pid0, nodeNote->_id=id0;
						  rect = _scene->_compartmentPos[pid1][id1]; 
				          rect.moveCenter(rect.center() + Dis);
					      B0->compartmentPos[id0] = rect;
					  }
					  break; 
		}			
	}
	/*for(int i=0; i<B1->_itemSelected.size(); i+=2)
	{
		int type=B1->_itemSelected[i];
		int id1=B1->_itemSelected[i+1];	
		vector<int> item;
		item.push_back(type); //item.push_back(id1);
		switch(type)
		{
		    case 'C':  item.push_back(Cnum);
				      if(itemSet.find(item)!=itemSet.end())
					  {
						  rect=_scene->_complexPos[pid1][Cnum];
				          rect.moveCenter(rect.center() + Dis);
				          B0->complexPos[id1]=rect;
					  }
					  Cnum++; break;
		    case 'E': item.push_back(Enum);
				      if(itemSet.find(item)!=itemSet.end())
					  {
						  rect=_scene->_physicalEntityPos[pid1][Enum];
						  rect.moveCenter(rect.center() + Dis);
						  B0->physicalEntityPos[id1] =  rect;  			
					  }
					  Enum++; break; 
			case 'L': item.push_back(Anum);
				      if(itemSet.find(item)!=itemSet.end())
					  {
						  rect=_scene->_ANodePos[pid1][Anum];
						  rect.moveCenter(rect.center() + Dis);
						  B0->ANodePos[id1] = rect; 
					  }
					  Anum++; break; 
		    case 'P': item.push_back(Pnum);
				      if(itemSet.find(item)!=itemSet.end())
					  {
						  rect = _scene->_proteinPos[pid1][Pnum]; 					  
						  rect.moveCenter(rect.center() + Dis);
						  B0->proteinPos[id1] = rect;
					  }
					  Pnum++; break; 
		    case 'S': item.push_back(Snum);
				      if(itemSet.find(item)!=itemSet.end())
					  {
						  rect= _scene->_smallMoleculePos[pid1][Snum]; 
						   rect.moveCenter(rect.center() + Dis);
						   B0->smallMoleculePos[id1]=rect;
					  }
					  Snum++; break; 
		    case 'D': item.push_back(Dnum);
				      if(itemSet.find(item)!=itemSet.end())
					  {
						 rect= _scene->_DnaPos[pid1][Dnum];  
				         rect.moveCenter(rect.center() + Dis);
					     B0->DnaPos[id1] = rect;
					  }
					  Dnum++; break;  
		    case 'R': item.push_back(Rnum);
				      if(itemSet.find(item)!=itemSet.end())
					  {
						  rect = _scene->_reactionPos[pid1][Rnum];
				          rect.moveCenter(rect.center() + Dis);
					      B0->reactionPos[id1] = rect ;
					  }
					  Rnum++; break; 
			case 'M': item.push_back(id1);
				      if(itemSet.find(item)!=itemSet.end())
					  {
						  rect = _scene->_compartmentPos[pid1][id1]; 
				          rect.moveCenter(rect.center() + Dis);
					      B0->compartmentPos[id1] = rect;
					  }
					  break; 
		}			
	}	*/
	//Cnum=Enum=Pnum=Snum=Dnum=Rnum=0;
	for(int i=0; i<B1->itemInfo.size(); i++)
	{
		int type=B1->itemInfo[i][0];//B1->_itemSelected[i];
		int id=B1->itemInfo[i][1],id1=B1->itemInfo[i][2];//B1->_itemSelected[i+1];	
		//int p0=B1->itemInfo[i][5];
		vector<int> link,link1;
		//for(int i=0; i<B1->_itemSelected.size(); i+=2)
		//{
		//	vector<int> link,link1;   //link: linked item, link1: current item 
		//	int type=B1->_itemSelected[i];
		//	int id=B1->_itemSelected[i+1],id1;
		/*switch(type)
		{
			case 'C': id1=Cnum++;  break;
			case 'E': id1=Enum++;  break;
			case 'L': id1=Pnum++;  break;			
			case 'P': id1=Pnum++;  break;				
			case 'S': id1=Snum++;  break;
			case 'D': id1=Dnum++;  break;
			case 'R': id1=Rnum++;  break;
		}*/	
		vector<int> item;
		item.push_back(type);  item.push_back(id1);
		if(pid0==B1->itemInfo[i][5])
		if(itemSet.find(item)!=itemSet.end())
		{
			switch(type)
			{
				case 'C': B1->complexPos[id1].setX(-1000);         break;
				case 'E':  B1->physicalEntityPos[id1].setX(-1000); break;
				case 'L': B1->ANodePos[id1].setX(-1000);           break;			
				case 'P': B1->proteinPos[id1].setX(-1000);         break;				
				case 'S': B1->smallMoleculePos[id1].setX(-1000);   break;
				case 'D':  B1->DnaPos[id1].setX(-1000);            break;
				case 'R': B1->reactionPos[id1].setX(-1000);        break;
			}		
			link.push_back(pid0);  link.push_back(type); link.push_back(id);//other
			link1.push_back(pid1);  link1.push_back(type); link1.push_back(id1);
		
			vector<vector<vector<int>>> linkRecord;	
			set<vector<vector<int>>>::iterator it;
			for(it = _scene->edgeconnections.begin(); it != _scene->edgeconnections.end(); it++)
			{
				 vector<vector<int>> plink = *it;				 
				 if(plink[0]==link1 || plink[1]==link1)
				 {
					 linkRecord.push_back(plink);				 
				 }			
			}
			//reconnect
			for(int j=0; j<linkRecord.size(); j++)
			{
				 vector<vector<int>> plink=linkRecord[j];
				 _scene->edgeconnections.erase(plink);			  
             
				 //plink.erase(link1); 
				 int od=-1;
				 if(plink[0]==link1)
					 od=1;
				 else if(plink[1]==link1)
					 od=0;

				 //vector<int> link2=*plink.begin(); 
				 if(od!=-1)
				 {
					 vector<int> link2=plink[od]; 
					 if(link[0]!=link2[0]) 
					 {
						 //plink.insert(link); //add the oriental item back and connected with link2
						 plink[1-od]=link;
						 _scene->edgeconnections.insert(plink);
					 }
				 }
			}		
		}
	}	
	//update links for B0
	vector<vector<int>> itemInfo1;
	for(int i=0; i<B1->itemInfo.size(); i++)
	{
		int type = B1->itemInfo[i][0];//B1->_itemSelected[i];
		int id = B1->itemInfo[i][1], id1 = B1->itemInfo[i][2];//B1->_itemSelected[i+1];	
		vector<int> item, item1 = B1->itemInfo[i];
		item.push_back(type);  item.push_back(id1);
		if(itemSet.find(item)==itemSet.end())
		{
		     itemInfo1.push_back(B1->itemInfo[i]);
		}				
	}
	B1->itemInfo=itemInfo1;
	return true;
}

void PathBubble1::PathBubbleDelectionUpdate(int m_pathwayID)
{
	if(_pathBubbleParent==NULL || _pathBubbleParent->m_pathwayID<0)
		return;

	//recover the selected nodes
	//int Cnum,Enum,Pnum,Snum,Dnum,Rnum,Anum;
	//Cnum=Enum=Pnum=Snum=Dnum=Rnum=Anum=0;
	QRectF rect;
	float x,y,h,w;
	int pid0=_pathBubbleParent->m_pathwayID, pid1=m_pathwayID;// pid0:parent, pid1: son
	PathBubble1* B0=_scene->_pathBubbles[pid0], *B1=_scene->_pathBubbles[pid1];
	subPathBubble1* sB = dynamic_cast<subPathBubble1*>(B1);
	QPointF Dis=sB->Dis;

	B1->collapseALL();	
	for(int i=0; i<B1->itemInfo.size(); i++)
	{
		int type=B1->itemInfo[i][0];//B1->_itemSelected[i];
		int id0=B1->itemInfo[i][1], id1=B1->itemInfo[i][2];//B1->_itemSelected[i+1];	
		CalloutNote* nodeNote=_scene->findNote(pid1, type, id1);	             					
		if(nodeNote!=NULL)  nodeNote->_pid=pid0, nodeNote->_id=id0;

		//vector<int> item;
		//item.push_back(type); item.push_back(B1->itemInfo[i][2]);
		if(pid0==B1->itemInfo[i][5])
		switch(type)
		{
		    case 'C': if(B1->complexPos[id1].x()>-990)
				      {		

						  rect=_scene->_complexPos[pid1][id1];
						  rect.moveCenter(rect.center() + Dis);
						  B0->complexPos[id0]=rect;  
					  }
					  break;
		    case 'E': if(B1->physicalEntityPos[id1].x()>-990)
				      {
						  rect=_scene->_physicalEntityPos[pid1][id1];
						  rect.moveCenter(rect.center() + Dis);
						  B0->physicalEntityPos[id0] =  rect;  
					  }
					  break; 
			case 'L': if(B1->ANodePos[id1].x()>-990)
				      {
						  rect=_scene->_ANodePos[pid1][id1];
						  rect.moveCenter(rect.center() + Dis);
						  B0->ANodePos[id0] = rect; 
					  }
					  break; 
		    case 'P': if(B1->proteinPos[id1].x()>-990)
				      {
						  rect = _scene->_proteinPos[pid1][id1]; 					  
						  rect.moveCenter(rect.center() + Dis);
						  B0->proteinPos[id0] = rect;
					  }
					  break; 
		    case 'S': if(B1->smallMoleculePos[id1].x()>-990)
				      {
						  rect= _scene->_smallMoleculePos[pid1][id1]; 
						  rect.moveCenter(rect.center() + Dis);
						  B0->smallMoleculePos[id1]=rect;
					  }
					  break;
		    case 'D': if(B1->DnaPos[id1].x()>-990)
				      {
						  rect= _scene->_DnaPos[pid1][id1];  
						  rect.moveCenter(rect.center() + Dis);
						  B0->DnaPos[id0] = rect;		
					  }
					  break;  
		    case 'R': if(B1->reactionPos[id1].x()>-990)
				      {
						  rect = _scene->_reactionPos[pid1][id1];
						  rect.moveCenter(rect.center() + Dis);
						  B0->reactionPos[id0] = rect ;					  
					  }
					  break; 
			case 'M': if(B1->compartmentPos[id1].x()>-990)
				      {
						  rect = _scene->_compartmentPos[pid1][id1]; 
						  rect.moveCenter(rect.center() + Dis);
						  B0->compartmentPos[id0] = rect;
					  }
					  break; 
		}			
	}

	//Cnum=Enum=Pnum=Snum=Dnum=Rnum=0;
	//find the about-to-remove items
	for(int i=0; i<B1->itemInfo.size(); i++)
	{
		if(pid0!=B1->itemInfo[i][5])
		    continue;

		int type=B1->itemInfo[i][0];//B1->_itemSelected[i];
		int id0=B1->itemInfo[i][1], id1=B1->itemInfo[i][2];//B1->_itemSelected[i+1];		
	
		vector<int> link,link1;   //link: linked item, link1: current item 
		//int type=B1->_itemSelected[i];
		//int id=B1->_itemSelected[i+1],id1;
		bool workingNode=false;
		switch(type)
		{
		    case 'C': if(B1->complexPos[id1].x()>-990)  workingNode=true;         B1->complexPos[id1].setX(-1000);         break;
		    case 'E': if(B1->physicalEntityPos[id1].x()>-990)  workingNode=true;  B1->physicalEntityPos[id1].setX(-1000);  break;
			case 'L': if(B1->ANodePos[id1].x()>-990)  workingNode=true;           B1->ANodePos[id1].setX(-1000);           break;			
		    case 'P': if(B1->proteinPos[id1].x()>-990)  workingNode=true;         B1->proteinPos[id1].setX(-1000);         break;				
		    case 'S': if(B1->smallMoleculePos[id1].x()>-990)  workingNode=true;   B1->smallMoleculePos[id1].setX(-1000);   break;
		    case 'D': if(B1->DnaPos[id1].x()>-990)  workingNode=true;             B1->DnaPos[id1].setX(-1000);             break;
		    case 'R': if(B1->reactionPos[id1].x()>-990)  workingNode=true;        B1->reactionPos[id1].setX(-1000);        break;
		}	
		if(!workingNode)
		   continue;

		link.push_back(pid0);  link.push_back(type); link.push_back(id1);//other
		link1.push_back(pid1);  link1.push_back(type); link1.push_back(id1);
		
		
		set<vector<vector<int>>>::iterator it;
		set<vector<vector<int>>> connections;
		for(int i=0; i<4; i++)
		{
			vector<vector<vector<int>>> linkRecord;	
			switch(i)
			{
				case 0: connections=_scene->edgeconnections;  break;
				case 1: connections=_scene->edgeconnections_A;  break;
				case 2: connections=_scene->edgeconnections_I;  break;
				case 3: connections=_scene->edgeconnections_4;  break;
			}
			set<vector<vector<int>>> tConnections=connections;
			for(it = tConnections.begin(); it != tConnections.end(); it++)
			{
				 vector<vector<int>> plink = *it;
				 //vector<vector<int>>::iterator id=plink.find(link1);
				 //if(id!=plink.end())
				 if(plink[0]==link1 || plink[1]==link1)
				 {
					 linkRecord.push_back(plink);				 
				 }			
			}
			//reconnect
			for(int j=0; j<linkRecord.size(); j++)
			{
				 vector<vector<int>> plink=linkRecord[j];
				 tConnections.erase(plink);			  
             
				 //plink.erase(link1); 
				 int od=-1;
				 if(plink[0]==link1)
					 od=1;
				 else if(plink[1]==link1)
					 od=0;

				 //vector<int> link2=*plink.begin(); 
				 if(od!=-1)
				 {
					 vector<int> link2=plink[od]; 
					 if(link[0]!=link2[0]) 
					 {
						 //plink.insert(link); //add the oriental item back and connected with link2
						 plink[1-od]=link;
						 tConnections.insert(plink);
					 }
				 }
			}
			switch(i)
			{
				case 0: _scene->edgeconnections=tConnections;  break;
				case 1: _scene->edgeconnections_A=tConnections;  break;
				case 2: _scene->edgeconnections_I=tConnections;  break;
				case 3: _scene->edgeconnections_4=tConnections;  break;
			}
		}
	}
}

	

void PathBubble1::PathBubbleDelectionUpdate(PathBubble1* b0, PathBubble1* pb)
{//restore connection etc.

}

vector<int> PathBubble1::whichItem( QPointF pos, float width, float height)
{	   
	if(m_pathwayID==1)
		m_pathwayID=m_pathwayID;

	if(fixedSize)
	{
		width=graphReferenceSize*_scale;
	    height=graphReferenceSize*_scale;
	}

	vector<int> item(2,-1);
	int xi, yi;
	vector<int> Clist;
	
	float dcx=dCenter.x()*this->Width(), dcy=dCenter.y()*this->Height();
	float x,y;
	int w=40,h=40;
	float XL,XR,YB,YT;
	x=pos.x()+width/2-dcx, y=pos.y()+height/2-dcy;
	xi=x/width*w, yi=y/height*h;
	if(xi<0) xi=0; else if(xi>=w) xi=w;
	if(yi<0) yi=0; else if(yi>=h) yi=h;

	//for(int i=0; i<itemGrid[xi][yi].size(); i=i+2)
	vector<int> crect;
	QRectF rect;
	for(set<vector<int>>::iterator it=itemGrid[xi][yi].begin(); it!=itemGrid[xi][yi].end(); it++) 
	{
		 vector<int> node=*it;
		 int type=node[0], id=node[1];
		 switch(type)
	     {
		     case 'C': rect=complexRect[id];  break;
			 case 'D': rect=DnaRect[id];  break;
			 case 'E': rect=physicalEntityRect[id];  break;
			 case 'P': rect=proteinRect[id];  break;
			 case 'S': rect=smallMoleculeRect[id];  break;
			 case 'R': rect=reactionRect[id];  break;			 
			 case 'L': rect=ANodeRect[id];  break;			 
             case 'M': Clist.push_back(id); break;			 				 
		 }		
		//XL=XL*width, XR=XR*width, YB=YB*height, YT=YT*height;	
		if(rect.contains(pos.x(), pos.y()))
		{
			item[0]=type, item[1]=id;
			if(type==77)
				crect=item;
			else
				return item;
		}		
	}
	if(!crect.empty())
	   return crect;

	//check compartment
	x=pos.x(), y=pos.y();
	float itv=width/w;
	float min=10000000;
	int mini=-1;
	for(int i=0; i<Clist.size(); i++) 
	{
		int id=Clist[i];
		if(id<compartmentRect.size())
		{
			
			float d;
			float x0 = compartmentRect[id].center().x(), y0 = compartmentRect[id].center().y();
			//float y0 = compartmentRect[id].y();
			rect = compartmentRect[id];
			if(rect.contains(x, y))
			{
			    d = sqrt((x0-x)*(x0-x) + (y0-y)*(y0-y));
				if(d<min)
				{
					  min=d; mini=id;
				}

			  
			}
		}
	}
	if(mini>=0)
	{
		item[0]='M', item[1]=mini;
	    return item;
	}

	item.clear();
	return item;
}


vector<vector<int>> PathBubble1::whichItem(QRectF pos, float width, float height)
{	   
	vector<vector<int>> items;
	if(m_pathwayID==1)
		m_pathwayID=m_pathwayID;

	if(fixedSize)
	{
		width=graphReferenceSize*_scale;
	    height=graphReferenceSize*_scale;
	}

	vector<int> item(2,-1);
	int xi, yi;
	vector<int> Clist;
	
	float dcx=dCenter.x()*this->Width(), dcy=dCenter.y()*this->Height();
	float x,y;
	int w=40,h=40;
	float XL,XR,YB,YT;
	int xi0,xi1,yi0,yi1;
	float x0,x1,y0,y1;
	
	
	x0=pos.x()+width/2-dcx, y0=pos.y()+height/2-dcy;
	x1=pos.x()+pos.width()+width/2-dcx, y1=pos.y()+pos.height()+height/2-dcy;

	xi0=x0/width*w, yi0=y0/height*h;
	if(xi0<0) xi0=0; else if(xi0>=w) xi0=w;
	if(yi0<0) yi0=0; else if(yi0>=h) yi0=h;

	xi1=x1/width*w, yi1=y1/height*h;
	if(xi1<1) xi1=1; else if(xi1>=w) xi1=w;
	if(yi1<1) yi1=1; else if(yi1>=h) yi1=h;

	for(int xi=xi0; xi<xi1; xi=xi+1)
	for(int yi=yi0; yi<yi1; yi=yi+1)
	{
		vector<int> crect;
		QRectF rect;
		for(set<vector<int>>::iterator it=itemGrid[xi][yi].begin(); it!=itemGrid[xi][yi].end(); it++) 
		{
			 vector<int> node=*it;
			 int type=node[0], id=node[1];
			 switch(type)
			 {
				 case 'C': rect=complexRect[id];  break;
				 case 'D': rect=DnaRect[id];  break;
				 case 'E': rect=physicalEntityRect[id];  break;
				 case 'P': rect=proteinRect[id];  break;
				 case 'S': rect=smallMoleculeRect[id];  break;
				 case 'R': rect=reactionRect[id];  break;			 
				 case 'L': rect=ANodeRect[id];  break;			 
				 case 'M': Clist.push_back(id); break;			 				 
			 }		
			//XL=XL*width, XR=XR*width, YB=YB*height, YT=YT*height;	
			if(rect.contains(pos.x(), pos.y()))
			{
				item[0]=type, item[1]=id;
				items.push_back(item);
				/*if(type==77)
					crect=item;
				else
					return item;
				*/
			}		
		}
	}
	//if(!crect.empty())
	//  return crect;

	//check compartment
	QRectF rect;
	x=pos.x(), y=pos.y();
	float itv=width/w;
	float min=10000000;
	int mini=-1;
	for(int i=0; i<Clist.size(); i++) 
	{
		int id=Clist[i];
		if(id<compartmentRect.size())
		{
			
			float d;
			float x0 = compartmentRect[id].center().x(), y0 = compartmentRect[id].center().y();
			//float y0 = compartmentRect[id].y();
			rect = compartmentRect[id];
			if(rect.contains(x, y))
			{
			    d = sqrt((x0-x)*(x0-x) + (y0-y)*(y0-y));
				if(d<min)
				{
					  min=d; mini=id;
				}

			  
			}
		}
	}
	if(mini>=0)
	{
		item[0]='M', item[1]=mini;
	    //return item;
		items.push_back(item);
	}

	//item.clear();
	return items;
}


QSizeF PathBubble1::getFontSize(int type, int id)
{
	QString qstr;
	string s1;
	std::ostringstream oss;
	//QString qstr;
	QSizeF size;
	switch(type)
	{
	    case 'C': qstr=_scene->_complexNameD[dataID][id][0];       break;
		case 'E': qstr=_scene->_physicalEntityNameD[dataID][id][0]; 
			      if(qstr.size()==0) 
				  { 
					 s1 = "physicalEntity"; 
					 oss << s1 << id; s1=oss.str(); 
					 qstr=s1.c_str(); 
				  }                		      break;
		 case 'L': qstr=_scene->_ANodeName[m_pathwayID][id][0]; break;
		 case 'P': qstr=_scene->_proteinNameD[dataID][id][0]; break;
		 case 'S': qstr=_scene->_smallMoleculeNameD[dataID][id][0];   break;		
		 case 'D': qstr=_scene->_DnaNameD[dataID][id][0];  break;	 				 
	}
	if(type!='R')
	{
		QTextDocument doc;
		doc.setHtml(qstr);
		QFont f("Arial", 6);
		doc.setDefaultFont(f);		
		size = doc.size();		
	}	
	return size;
}

QSizeF PathBubble1::getItemSize( int type, int id)
{
	float width=graphReferenceSize, height=graphReferenceSize;
	float h,w;
	QString qstr;
	string s1;
	std::ostringstream oss;
	QSizeF size;

	switch(type)
	{
	     case 'C': size.setWidth(120/width); size.setHeight(120/height); break;	 
			       qstr=_scene->_complexNameD[dataID][id][0]; 
				   break;
		 case 'E': w=1.2; qstr=_scene->_physicalEntityNameD[dataID][id][0]; 
			       if(qstr.size()==0) 
				   { 
					 s1 = "physicalEntity"; 
					 oss << s1 << id; s1=oss.str(); 
					 qstr=s1.c_str(); 
				   } 
			       break;
		 case 'L': w=1.0; qstr=_scene->_ANodeName[m_pathwayID][id][0]; break;
		 case 'P': w=1.0; qstr=_scene->_proteinNameD[dataID][id][0]; break;
		 case 'S': w=0.35; qstr=_scene->_smallMoleculeNameD[dataID][id][0];   break;		
		 case 'D': w=0.8; qstr=_scene->_DnaNameD[dataID][id][0];  break;	 		
		 case 'R': size.setWidth(8/width); size.setHeight(8/height); break;	 
	}

	if(type!='R' && type!='C')
	{
		w=w*width;
		QTextDocument doc;
		doc.setHtml(qstr);
		QFont f("Arial", 6);
		doc.setDefaultFont(f);
		doc.setTextWidth( w );
		size = doc.size();
		size.setWidth(size.width()/width);
		size.setHeight(size.height()/height);
	}	
	return size;
}
void PathBubble1::drawCurve( QPainter* painter )
{
	if(_lines.size()>0)
		_lines=_lines;
	painter->save();
	//*************************************************************************
	painter->setPen( QPen( QBrush( QColor( 0, 0, 255, 255) ), 2 ) );
	painter->drawLines( _lines );
	//*************************************************************************
	painter->restore();
	return;
}


QSizeF PathBubble1::getItemSize(QString name, int type, int id)
{
	float width=graphReferenceSize, height=graphReferenceSize;
	float h,w;
	QString qstr;
	string s1;
	std::ostringstream oss;
	QSizeF size;

	switch(type)
	{
	     case 'C': size.setWidth(120/width); size.setHeight(120/height); break;	 
			      qstr=name;//_scene->_complexNameD[dataID][id][0]; 
				  break;
		case 'E': w=1.2; qstr=name;//_scene->_physicalEntityNameD[dataID][id][0]; 
			     if(qstr.size()==0) 
				  { 
					 s1 = "physicalEntity"; 
					 oss << s1 << id; s1=oss.str(); 
					 qstr=s1.c_str(); 
				  } 
			      break;
		 case 'P': w=1.0; qstr=name;//_scene->_proteinNameD[dataID][id][0]; 
			       break;
		 case 'L': w=1.0; qstr=name;//_scene->_proteinNameD[dataID][id][0]; 
			       break;
		 case 'S': w=0.35; qstr=name;//  break;		
		 case 'D': w=0.8; qstr=name;//  break;	 		
		 case 'R': size.setWidth(8/width); size.setHeight(8/height); break;	 
	}

	if(type!='R' && type!='C')
	{
		w=w*width;
		QTextDocument doc;
		doc.setHtml(qstr);
		QFont f("Arial", 6);
		doc.setDefaultFont(f);
		doc.setTextWidth( w );
		size = doc.size();
		size.setWidth(size.width()/width);
		size.setHeight(size.height()/height);
	}	
	return size;
}

void PathBubble1::drawfills( QPainter* painter )
{
	/*painter->save();
	painter->setPen( QPen( QBrush( QColor( 0, 0, 255, 10) ), 2 ) );
	painter->drawLines( fills );
	painter->restore();*/
	painter->setBrush( QColor( 0, 0, 255, 100) );	
	painter->drawPolygon(_contain);
}

void PathBubble1::addLine( QPointF pos1, QPointF pos2 )
{
	//updateBoundingRect( pos1 );
	//updateBoundingRect( pos2 );
	_lines.append( QLineF( pos1, pos2 ) );
	return;
}

void PathBubble1::wheelEvent(QGraphicsSceneWheelEvent *event)
{
	//prepareGeometryChange();	
    if(!onBoarder( event->pos() ) && !_scene->naviContains(event->scenePos()))
	{
		float rate = exp( event->delta() / graphReferenceSize / 2);		
		{
			QPointF dCenter1=dCenter;
			float scale1=_scale;
			//reScale(x0y0, x1y1, (x0y0-event->pos())*rate+event->pos()-x0y0, (x1y1-event->pos())*rate+event->pos()-x1y1, _scale, dCenter, rate);			
			wheelReScale(x0y0, x1y1, event->pos(), event->pos(), _scale, dCenter, rate);			
			scaleGraphToBePaint(dCenter1, scale1, dCenter, _scale);
			itemGrid=recordItemGrid(visibleItemNum, visibleCompartmentNum);		
		}
	}	
}


void PathBubble1::addSmallMoleculetoItemSelected(vector<int> &itemSelected)
{
	vector<int> start(4,0), middle(4,0), end(4,0), temp(2,0);
	int cid1, cid2;
	//temp[0]='R';	temp[1]=i; 
	temp[0]='S';
	for(int i=0; i<itemSelected.size(); i=i+2)
	{
		if(itemSelected[i]=='R' )
		{
			int id=itemSelected[i+1];
			//compartmentToItemSelected(itemSelected[i+1], itemSelected);	
			for(int j=0; j<_reaction[id].size(); j++)
			{
				vector<int> item=_reaction[id][j];
				if(!item.empty() && item[0]=='S')
				{
					//item.resize(2);
				    itemSelected.push_back(item[0]);
					itemSelected.push_back(item[1]);
				}
			}
		}
	}	
	
}

QRectF PathBubble1::rectToPos(QRectF rect)
{
	float width, height;
	float cx, cy, w, h;	
	float dcx, dcy;
	QRectF pos;

	if(fixedSize)
	{
		width=graphReferenceSize*_scale, height=graphReferenceSize*_scale;
	}
	else
	{
		width=this->realRect().width()*_scale;
		height=this->realRect().height()*_scale;
	}
	dcx=dCenter.x()*this->realRect().width(), dcy=dCenter.y()*this->realRect().height();

	cx = rect.center().x(), cy = rect.center().y(); 
	w = rect.width(), h = rect.height(); 
	w = w /width; h = h / height;

	cx = (cx - dcx + width/2 ) / width;
	cy = (cy - dcy + height/2 ) / height;

	pos = QRectF(cx-w/2, cy-h/2, w, h);
	return pos;
}

QRectF PathBubble1::resizeNode( QRectF oriRect, QRectF curRect, QRectF rect)
{
	float cx0,cy0,cx1,cy1,cx,cy;
	cx0=oriRect.center().x();
	cy0=oriRect.center().y();

	cx1=curRect.center().x();
	cy1=curRect.center().y();

	float dx0,dy0,w,h;
	float sx = curRect.width()/oriRect.width(), sy = curRect.height()/oriRect.height();
	dx0 = rect.center().x()-cx0, dy0 = rect.center().y()-cy0;
	
	w = rect.width() , h = rect.height();
	cx = cx1 + dx0 * sx, cy = cy1 + dy0 * sy;

	return( QRectF(cx-w/2, cy-h/2, w, h) );
	
}
		


void PathBubble1::resizeItem_1( int type, int id, QPointF Pos, QPointF lastPos)
{//resize node when dragging it
	QPointF  dis = Pos- lastPos;
	float Px,Py,disx,disy;
	float sizew,sizeh,w,h,cx,cy;	
	QRectF rect,pPos;
	QRectF curRect,oriRect, curPos, oriPos;

	switch(type)
	{ 
		case 'C': rect=complexRect[id]; pPos=complexPos[id];
				  break;
		case 'E': rect=physicalEntityRect[id]; pPos=physicalEntityPos[id];
					break;
		case 'P': rect=proteinRect[id]; pPos=proteinPos[id];
					break;
		case 'L': rect=ANodeRect[id]; pPos=ANodePos[id];
					break;
		case 'S': rect=smallMoleculeRect[id]; pPos=smallMoleculePos[id];
					break;	
		case 'D': rect=DnaRect[id]; pPos=DnaPos[id];
					break;	 	
		case 'R': rect=reactionRect[id]; pPos=reactionPos[id];
					break;	 	
		
		case 'M': rect=compartmentRect[id]; pPos=compartmentPos[id]; oriRect=compartmentRect[id]; oriPos=compartmentPos[id];
					break;	
	}
	int cornerID = whichCorner(Pos,rect);

	w=pPos.width();
	h=pPos.height();

	disx=dis.x();  disy=dis.y();
	if(cornerID==1)
	{
		disx=-disx;
		disy=-disy;
	}
	else if(cornerID==2)
    {
		disx=-disx;			
	}
	else if(cornerID==4)
    {
		disy=-disy;			
	}

	sizew =  rect.width() + disx;
	sizeh =  rect.height() + disy;

	//adjust it back
	if(cornerID==1)
	{
		disx=-disx;
		disy=-disy;
	}
	else if(cornerID==2)
    {
		disx=-disx;			
	}
	else if(cornerID==4)
    {
		disy=-disy;			
	}
	
	dis=QPointF(disx,disy);
	Pos = dis - lastPos;
	
	float dx = 2.0 * Pos.x();
	float dy = 2.0 * Pos.y();
	float sx,sy;
		
	dx=abs(dx);
	dy=abs(dy);

	Px = disx/2.0; 
	Py = disy/2.0;

	QPointF p0,p1,Dis;
	float width, height;
	if(fixedSize)
	{
		width=graphReferenceSize*_scale, height=graphReferenceSize*_scale;
	}
	else
	{
		width=this->realRect().width()*_scale;
		height=this->realRect().height()*_scale;
	}
	QPointF center=QPointF(QPointF(Px,Py) + rect.center());
		
	sx=sizew/rect.width();
	sy=sizeh/rect.height();
	
	w=w*sx;
	h=h*sy;
	
	rect.setWidth(sizew);
	rect.setHeight(sizeh);
	rect.moveCenter(center);	
		
	cx=pPos.center().x();
	cy=pPos.center().y();	
			
	cx=rect.center().x();
	cy=rect.center().y();	
	
	cx = (cx+width/2)/width;
	cy = (cy+height/2)/height;

	pPos=QRectF(cx-w/2,cy-h/2,w,h);
	pPos = rectToPos(rect);
	
	switch(type)
	{ 
		case 'C': complexRect[id]=rect; 
			      complexPos[id]=pPos;
				  break;
		case 'E': physicalEntityRect[id]=rect; 
			      physicalEntityPos[id]=pPos;
				  break;
		case 'P': proteinRect[id]=rect; 
			      proteinPos[id]=pPos;
				  break;
		case 'S': smallMoleculeRect[id]=rect; 
			      smallMoleculePos[id]=pPos;
				  break;	
		case 'D': DnaRect[id]=rect; 
			      DnaPos[id]=pPos;
				  break;	 	
		case 'R': reactionRect[id]=rect; 
			      reactionPos[id]=pPos;
				  break;	 	
		case 'L': ANodeRect[id]=rect; 
			      ANodePos[id]=pPos;
				  break;
		case 'M': compartmentRect[id]=rect; 
			      compartmentPos[id]=pPos;
				  break;	
	}		
    
	if(type=='M')
	{
		curRect=compartmentRect[id];
		curPos=compartmentPos[id];
		for(set<vector<int>>::iterator it=_scene->CompartmentContain[m_pathwayID][id].begin(); it!=_scene->CompartmentContain[m_pathwayID][id].end(); it++)
		{   
			vector<int> temp=*it;						
			int ntype=temp[0], nid=temp[1];	
			int ID=getNodeID(ntype, nid);	
			if(ID>=0)
			switch(ntype)
			{
				case 'C': if(complexPos[nid].x()>-999.99)       
						  {
							  Dis=complexRect[nid].center();
							  
							  complexRect[nid]=resizeNode(oriRect, curRect, complexRect[nid]); 
							  complexPos[nid]=resizeNode(oriPos, curPos, complexPos[nid]); 
							  
							  Dis=complexRect[nid].center()-Dis;
							  
							  fontRect[ID].moveCenter(fontRect[ID].center() + Dis);
						  }
						  break;
				case 'P': if(proteinPos[nid].x()>-999.99) 
						  {
							  Dis=proteinRect[nid].center();		

							  proteinRect[nid]=resizeNode(oriRect, curRect, proteinRect[nid]);    							  
							  proteinPos[nid]=resizeNode(oriPos, curPos, proteinPos[nid]);    
							  
							  Dis=proteinRect[nid].center()-Dis;
							  
							  fontRect[ID].moveCenter(fontRect[ID].center() + Dis);
						  }
						  break;
				case 'D': if(DnaPos[nid].x()>-999.99) 
						  {
							   Dis=DnaRect[nid].center();	
							   
							   DnaRect[nid]=resizeNode(oriRect, curRect, DnaRect[nid]);  	
							   DnaPos[nid]=resizeNode(oriPos, curPos, DnaPos[nid]);  
							   
							   Dis=DnaRect[nid].center()-Dis;

							   fontRect[ID].moveCenter(fontRect[ID].center() + Dis);
						  }
						  break;
				case 'E': if(physicalEntityPos[nid].x()>-999.99)
						  {
							  Dis=physicalEntityRect[nid].center();	

							  physicalEntityRect[nid]=resizeNode(oriRect, curRect, physicalEntityRect[nid]); 
							  physicalEntityPos[nid]=resizeNode(oriPos, curPos, physicalEntityPos[nid]);    
							  
							  Dis=physicalEntityRect[nid].center()-Dis;

							  fontRect[ID].moveCenter(fontRect[ID].center() + Dis);
						  }
						  break;
				case 'S': if(smallMoleculePos[nid].x()>-999.99)
						  {
							  Dis=smallMoleculeRect[nid].center();

							  smallMoleculeRect[nid]=resizeNode(oriRect, curRect, smallMoleculeRect[nid]); 
							  smallMoleculePos[nid]=resizeNode(oriPos, curPos, smallMoleculePos[nid]); 
							  
							  Dis=smallMoleculeRect[nid].center()-Dis;

							  fontRect[ID].moveCenter(fontRect[ID].center() + Dis);
						  }
						  break;
				case 'R': if(reactionPos[nid].x()>-999.99)
						  {
							  Dis=reactionRect[nid].center();

							  reactionRect[nid]=resizeNode(oriRect, curRect, reactionRect[nid]);  
							  reactionPos[nid]=resizeNode(oriPos, curPos, reactionPos[nid]);    

							  Dis=reactionRect[nid].center()-Dis;

							  fontRect[ID].moveCenter(fontRect[ID].center() + Dis);
						  }
						  break;			      
				case 'L': if(ANodePos[nid].x()>-999.99)
						  {
							  Dis=ANodeRect[nid].center();

							  ANodeRect[nid]=resizeNode(oriRect, curRect, ANodeRect[nid]);  
							  ANodePos[nid]=resizeNode(oriPos, curPos, ANodePos[nid]);    

							  Dis=ANodeRect[nid].center()-Dis;

							  fontRect[ID].moveCenter(fontRect[ID].center() + Dis);
						  }
						  break;	
			}	
		}
	}
}


void PathBubble1::mouseMoveEvent( QGraphicsSceneMouseEvent *event )
{	
	QPoint tPos=QPoint(event->pos().x(),event->pos().y());
	if(getCalloutNoteRect().contains(tPos))
		return;

	if(_scene->_fileSelection!=NULL && _scene->_fileSelection->getDiaSceneRect().contains(event->scenePos()))
		return;

	if( UNGROUPBYMENU || (m_isMoving || m_isResizing || onBoarder( event->pos() ) || pressOnBoarder || pressOnUngroup || onUnGroupArea(event->pos()) ) && !draggingItem )
	{
		ItemBase::mouseMoveEvent( event );  //when scale is enabled
		return;
	}
	if(m_pathwayID>=0&&_penisOn)
	{
	    addLine( initPos, event->pos() );
		initPos = event->pos();
	}
	else if(m_pathwayID>=0&&!itemSelected.empty() && !m_isMoving) // !m_rightButton)	
	{	
		if(isInnerResizing)
		{
		    if(!itemSelected.empty())
			{
				int type=itemSelected[0], id=itemSelected[1];	
				if(type!='W')
				{
					resizeItem_1( type, id, event->pos(), event->lastPos());
				}				
			}		
		}
		else if(_grabisOn||_queryisOn||_upisOn)
	    {
			if(outsideBubble(event->pos()))
			{
				CalloutNote* nodeNote=_scene->findNote(m_pathwayID, itemSelected[0], itemSelected[1]);	  
				bool v=false;
				if(nodeNote)
				{
					v=nodeNote->isVisible();
				}
				v=v;
				if(bubbleRect.width()==0)
				{
					bubbleRect.moveCenter(event->pos());
				    bubbleRect.setWidth(200), bubbleRect.setHeight(123);								
				}
			}
			else 
			{		
				if(bubbleRect.width()!=0)
				    bubbleRect.setWidth(0), bubbleRect.setHeight(0);				
			}
			if(getVisibleItemNum()>1 && !(dragout == 4 && itemSelected[0] == 'M' && getVisibleCompartmentNum()<=1) )
			{ 
				for(int i=0; i<itemSelected.size(); i=i+2)
				{
					if(itemSelected[i]=='M' )
					{
						compartmentToItemSelected(itemSelected[i+1], itemSelected);						
					}
				}				
				dragItem(event->pos()-event->lastPos());	
				draggingItem=1;	
			}
			if(_contain.empty()&& sharedProtein.empty() && sharedComplex.empty() && sharedSmallMolecule.empty() && sharedPhysicalEntity.empty() && sharedDna.empty() && sharedReaction.empty())
			{
				highlighted.clear();				 	    		
				///***
				//getGraphToBePaint();
			}
			checkGroupConnections(itemSelected, edgeSelected);
			vector<int> pre;
			getPartUpdateSelectedNodetoBePaintWhenMoved(pre, itemSelected,event->pos()-event->lastPos());			
		}
		else
		{
			if(!onBoarder( event->pos() ) )
		    {			
				bool flag1=false;				
				if( verifyItemSelected(itemSelected) )
				{
					if(groupSelected) //for pen selection case
					    flag1 = updateGroupedItemPos(itemSelected, event->pos()-event->lastPos());
					else 
					{
						flag1 = updateAItemPos(itemSelected[0], itemSelected[1], event->pos()-event->lastPos());
						
					}
				}
				else if( insideBubble(event->pos() ) )
				{
					if(groupSelected) //for pen selection case
					   flag1 = updateGroupedItemPos(itemSelected, event->pos()-event->lastPos());
					else flag1 = updateAItemPos(itemSelected[0], itemSelected[1], event->pos()-event->lastPos());
				}

				if( flag1 && !groupSelected && itemSelected[0]=='M' )
				{
					//put item in the compartment in itemselected	
					int mid=itemSelected[1];
					if(dragout==0)
					{	
						compartmentToItemSelected(mid, itemSelected);
						setItemDragged(itemSelected);	
						
						if(_contain.empty()&& sharedProtein.empty() && sharedComplex.empty() && sharedSmallMolecule.empty() && sharedPhysicalEntity.empty() && sharedANode.empty() && sharedDna.empty() && sharedReaction.empty())
							highlighted.clear();
						checkGroupConnections(itemSelected, edgeSelected);
						for(int k=0; k<itemSelected.size(); k=k+2)
						{
							if(itemSelected[k]!='M')
							{
								vector<int> temp(2,0);
								temp[0]=itemSelected[k]; temp[1]=itemSelected[k+1];
								highlighted.insert(temp);
							}
						}	
						dragout=1;
					}	
					//dragItem(event->pos()-event->lastPos());
					draggingItem=1;	
					if(dragout==2)
					{
						dragout=3;
						bubbleRect.setWidth(0), bubbleRect.setHeight(0);
					}
					else dragout=1;				
				}
				else if(flag1 && !groupSelected && !itemSelected.empty() && itemSelected[0]!='W' && itemSelected[0]!='M')
			    {
					if(dragout==0)
					{
						setAItemDragged(itemSelected[0], itemSelected[1]);	
						bubbleRect.setWidth(0), bubbleRect.setHeight(0);
					}					
					draggingItem=1;
					if(dragout==2)
					   dragout=3;
					else dragout=1;
			    }
				else if(flag1 && groupSelected && !itemSelected.empty() )
			    {
					//int mid=itemSelected[1];
					if(dragout==0)
					{	
						for(int i=0; i<itemSelected.size(); i=i+2)
						{
							if(itemSelected[i]=='M' )
							{
								compartmentToItemSelected(itemSelected[i+1], itemSelected);						
							}
						}
						//compartmentToItemSelected(mid, itemSelected);
						setItemDragged(itemSelected);	
						
						if(_contain.empty()&& sharedProtein.empty() && sharedComplex.empty() && sharedSmallMolecule.empty() && sharedPhysicalEntity.empty() && sharedANode.empty() && sharedDna.empty() && sharedReaction.empty())
							highlighted.clear();
						checkGroupConnections(itemSelected, edgeSelected);
						for(int k=0; k<itemSelected.size(); k=k+2)
						{
							if(itemSelected[k]!='M')
							{
								vector<int> temp(2,0);
								temp[0]=itemSelected[k]; temp[1]=itemSelected[k+1];
								highlighted.insert(temp);
							}
						}	
						dragout=1;
					}	
					//dragItem(event->pos()-event->lastPos());
					draggingItem=1;	
					if(dragout==2)
					{
						dragout=3;
						bubbleRect.setWidth(0), bubbleRect.setHeight(0);
					}
					else dragout=1;	
			    }
				if(flag1)
				{
					//addSmallMoleculetoItemSelected(itemSelected);
					//if(insideBubble(event->pos()))
					dragItem(event->pos()-event->lastPos());	
					///***
					//getGraphToBePaint();
					vector<int> pre;				
					//getPartUpdateSelectedNodetoBePaint(pre, itemSelected);
					getPartUpdateSelectedNodetoBePaintWhenMoved(pre, itemSelected,event->pos()-event->lastPos());		
				}
			}	
			if(outsideBubble(event->pos()))
			{
				/*if(itemSelected[0] == 'R' && itemSelected.size()==2)
				{
					dragout=0;
						QMessageBox msgBox;
						msgBox.move(event->scenePos().x(), event->scenePos().y());
						msgBox.setWindowTitle("Moving a reaction node");
						msgBox.setText("Can not move a single reaction node out");
						msgBox.setStandardButtons(QMessageBox::Yes);
						if(msgBox.exec() == QMessageBox::Yes)
							return;
				}*/
				/*else if(itemSelected[0] == 'M' && getVisibleCompartmentNum()==1)
				{
					dragout=0;
					QMessageBox msgBox;
					msgBox.move(event->scenePos().x(), event->scenePos().y());
						msgBox.setWindowTitle("Moving the only compartment");
						msgBox.setText("Can not move the only compartment out");
						msgBox.setStandardButtons(QMessageBox::Yes);
						if(msgBox.exec() == QMessageBox::Yes)
							return;
				}
				else if(getVisibleItemNum()<=1) 
				{
					dragout=0;
					QMessageBox msgBox;
					msgBox.move(event->scenePos().x(), event->scenePos().y());
						msgBox.setWindowTitle("Moving the only item");
						msgBox.setText("Can not move the only item out");
						msgBox.setStandardButtons(QMessageBox::Yes);
						if(msgBox.exec() == QMessageBox::Yes)
							return;
				}
				else if( (getVisibleItemNum() + getVisibleCompartmentNum() ) == itemSelected.size()/2)
				{
					dragout=0;
					QMessageBox msgBox;
					msgBox.move(event->scenePos().x(), event->scenePos().y());
						msgBox.setWindowTitle("Moving all the items");
						msgBox.setText("Can not move all the items out");
						msgBox.setStandardButtons(QMessageBox::Yes);
						if(msgBox.exec() == QMessageBox::Yes)
							return;				
				}
				else if(itemSelected.size()==2 && (itemSelected[0]=='M' || itemSelected[0]=='W'))
				{
				    dragout=0;
					QMessageBox msgBox;
					msgBox.move(event->scenePos().x(), event->scenePos().y());
					msgBox.setWindowTitle("Moving an empty compartment");
					msgBox.setText("Can not an empty compartment out");
					msgBox.setStandardButtons(QMessageBox::Yes);
					if(msgBox.exec() == QMessageBox::Yes)
						return;	
				}*/
				//else //if( (itemSelected[0]!= 'M' && getVisibleItemNum()>1) || (itemSelected[0]== 'M' && getVisibleCompartmentNum()>1) )					
				{
					bubbleRect.setWidth(200), bubbleRect.setHeight(123);
					bubbleRect.moveCenter(event->pos());
					dragout=2;					
				}


				CalloutNote* nodeNote=_scene->findNote(m_pathwayID, itemSelected[0], itemSelected[1]);	  
				bool v=false;
				if(nodeNote)
				{
					v=nodeNote->isVisible();
				}
				v=v;
				//else 
				//	dragout=4;
				
			}		
		}
	}		
	else if(m_pathwayID>=0)
	{
	    //pan
		if(_mousePressed && !m_isMoving && !m_isResizing && !m_rightButton && !AFTERUNGROUP)
	    {
			//dCenter = dCenter + event->pos() - event->lastPos();
			//comput scale and dcenter
			float s=pow(event->pos().x()-event->lastPos().x(),2)+ pow(event->pos().y()-event->lastPos().y(),2);
			//pan
			if(s>20)
			{
				if(!outsideBubble(event->pos()))  
				{
                    m_isInnerMoving=true;
					float S=-10000;					
					
					//reScale(x0y0, x1y1, QPoint(-58,0), QPoint(-58,0), S, dCenter);	
					//reScale(x0y0, x1y1, event->pos() - event->lastPos(), event->pos() - event->lastPos(), S, dCenter);						
					Pan(x0y0, x1y1, event->pos() - event->lastPos(), event->pos() - event->lastPos(), S, dCenter);						
                    moveGraphToBePaint(event->pos()-event->lastPos());
					//reScale(x0y0, x1y1, QPointF(-54,0), QPointF(-54,0), S, dCenter);	
		            //moveGraphToBePaint(QPointF(-54,0));
				}
			}
		}
		else if(_mousePressed && m_isMoving && !m_isInnerMoving)
		{
		    ItemBase::mouseMoveEvent( event ); //when scale is enabled
		}
	}	
	//m_isMoving=false;
	return;
}


void PathBubble1::compartmentToItemHighlighted(int mid, set<vector<int>> &highlighted)
{
	
	for(set<vector<int>>::iterator it=_scene->CompartmentContain[m_pathwayID][mid].begin(); it!=_scene->CompartmentContain[m_pathwayID][mid].end(); it++)
	{   
		highlighted.insert(*it);
		//int type=temp[0], nid=temp[1];			
		/*switch(type)
		{
			case 'C': if(complexPos[nid].x()>-999.99)        itemSelected.push_back(type), itemSelected.push_back(nid);   break;
			case 'P': if(proteinPos[nid].x()>-999.99)        itemSelected.push_back(type), itemSelected.push_back(nid);   break;
			case 'D': if(DnaPos[nid].x()>-999.99)            itemSelected.push_back(type), itemSelected.push_back(nid);	  break;
			case 'E': if(physicalEntityPos[nid].x()>-999.99) itemSelected.push_back(type), itemSelected.push_back(nid);   break;
			case 'S': if(smallMoleculePos[nid].x()>-999.99)  itemSelected.push_back(type), itemSelected.push_back(nid);   break;
			case 'R': if(reactionPos[nid].x()>-999.99)       itemSelected.push_back(type), itemSelected.push_back(nid);   break;			      
		}*/										
	}
}

void PathBubble1::compartmentToItemSelected(int mid, vector<int> &itemSelected)
{
	for(set<vector<int>>::iterator it=_scene->CompartmentContain[m_pathwayID][mid].begin(); it!=_scene->CompartmentContain[m_pathwayID][mid].end(); it++)
	{   
		vector<int> temp=*it;						
		int type=temp[0], nid=temp[1];	
		bool flag=false;
		for(int i=0; i<itemSelected.size(); i=i+2)
		{
			if(itemSelected[i]==type && itemSelected[i+1]==nid)
			{
		        flag=true;
				break;
			}
		}
		if(!flag)
		switch(type)
		{
			case 'C': if(complexPos[nid].x()>-999.99)        itemSelected.push_back(type), itemSelected.push_back(nid);   break;
			case 'P': if(proteinPos[nid].x()>-999.99)        itemSelected.push_back(type), itemSelected.push_back(nid);   break;
			case 'D': if(DnaPos[nid].x()>-999.99)            itemSelected.push_back(type), itemSelected.push_back(nid);	  break;
			case 'E': if(physicalEntityPos[nid].x()>-999.99) itemSelected.push_back(type), itemSelected.push_back(nid);   break;
			case 'L': if(ANodePos[nid].x()>-999.99)         itemSelected.push_back(type), itemSelected.push_back(nid);   break;
			case 'S': if(smallMoleculePos[nid].x()>-999.99)  itemSelected.push_back(type), itemSelected.push_back(nid);   break;
			case 'R': if(reactionPos[nid].x()>-999.99)       itemSelected.push_back(type), itemSelected.push_back(nid);   break;			      
		}										
	}
}


void PathBubble1::dragItem(QPointF Pos)
{

	if(getVisibleItemNum()<=1)
		return;

	if(getVisibleCompartmentNum()<=1 && itemSelected[0]=='M')
	    return;

	for(int i=0;i<itemDragged.size();i++)
	{
		itemDragged[i].moveCenter(Pos+itemDragged[i].center());			
	}
	bubbleRect.moveCenter(Pos+bubbleRect.center());
}

bool PathBubble1::updateAItemPos(int type, int id, QPointF move)
{	
   	/*if(getVisibleItemNum()<=1) 
		return false;

	if(dragout == 4 && type == 'M' && getVisibleCompartmentNum()<=1)
		return false;
	*/
	
	CalloutNote* nodeNote=_scene->findNote(m_pathwayID, type, id);	             					
	QPointF Move=move;	
	move=moveDis(move,_scale);
    switch(type)
	{
	    case 'C': updateItemGrid(itemGrid, type, id, move+complexPos[id].center()); complexPos[id].moveCenter(move+complexPos[id].center());	  
				  if(nodeNote!=NULL) nodeNote->mySetPos(Move+nodeNote->pos());    break;
		case 'P': updateItemGrid(itemGrid, type, id, move+proteinPos[id].center()); proteinPos[id].moveCenter(move+proteinPos[id].center());	    
			      if(nodeNote!=NULL)  nodeNote->mySetPos(Move+nodeNote->scenePos()); break;
	    case 'D': updateItemGrid(itemGrid, type, id, move+DnaPos[id].center());  DnaPos[id].moveCenter(move+DnaPos[id].center());		         
			      if(nodeNote!=NULL)  nodeNote->mySetPos(Move+nodeNote->pos());  break;
		case 'E': updateItemGrid(itemGrid, type, id, move+physicalEntityPos[id].center()); physicalEntityPos[id].moveCenter(move+physicalEntityPos[id].center());   
			      if(nodeNote!=NULL)  nodeNote->mySetPos(Move+nodeNote->pos());  break;
		case 'S': updateItemGrid(itemGrid, type, id, move+smallMoleculePos[id].center());  smallMoleculePos[id].moveCenter(move+smallMoleculePos[id].center());   
			      if(nodeNote!=NULL)  nodeNote->mySetPos(Move+nodeNote->pos());  break;
		case 'L': updateItemGrid(itemGrid, type, id, move+ANodePos[id].center());  ANodePos[id].moveCenter(move+ANodePos[id].center());   
			      if(nodeNote!=NULL)  nodeNote->mySetPos(Move+nodeNote->pos());  break;
		case 'R': updateItemGrid(itemGrid, type, id, move+reactionPos[id].center());  reactionPos[id].moveCenter(move+reactionPos[id].center());  
			      if(nodeNote!=NULL) 
					  nodeNote->mySetPos(Move+nodeNote->pos());  break;			      
	    case 'M': updateItemGrid(itemGrid, type, id, move+compartmentPos[id].center());  
			      //compartmentPos[id].moveCenter(move+compartmentPos[id].center());  			
			      updateACompartmentPos(id, move, Move); 
			      break; //if(nodeNote!=NULL)  nodeNote->mySetPos(move+nodeNote->pos()); 			      
	}	
	return true;
				
}


bool PathBubble1::updateGroupedItemPos(vector<int> itemSelected, QPointF move)
{	
   	if(getVisibleItemNum()<=1) 
		return false;

	if(dragout == 4 && getVisibleCompartmentNum()<=1) //type == 'M'
		return false;

	QPointF Move=move;	
	move=moveDis(move,_scale);
		
	for(int i=0; i<itemSelected.size(); i+=2)
	{
		int type=itemSelected[i], id=itemSelected[i+1];
		CalloutNote* nodeNote=_scene->findNote(m_pathwayID, type, id);	             					
	
		switch(type)
		{
			case 'C': updateItemGrid(itemGrid, type, id, move+complexPos[id].center()); complexPos[id].moveCenter(move+complexPos[id].center());	  
					  if(nodeNote!=NULL) nodeNote->mySetPos(Move+nodeNote->pos());    break;
			case 'P': updateItemGrid(itemGrid, type, id, move+proteinPos[id].center()); proteinPos[id].moveCenter(move+proteinPos[id].center());	    
					  if(nodeNote!=NULL)  nodeNote->mySetPos(Move+nodeNote->scenePos()); break;
			case 'D': updateItemGrid(itemGrid, type, id, move+DnaPos[id].center());  DnaPos[id].moveCenter(move+DnaPos[id].center());		         
					  if(nodeNote!=NULL)  nodeNote->mySetPos(Move+nodeNote->pos());  break;
			case 'E': updateItemGrid(itemGrid, type, id, move+physicalEntityPos[id].center()); physicalEntityPos[id].moveCenter(move+physicalEntityPos[id].center());   
					  if(nodeNote!=NULL)  nodeNote->mySetPos(Move+nodeNote->pos());  break;
			case 'S': updateItemGrid(itemGrid, type, id, move+smallMoleculePos[id].center());  smallMoleculePos[id].moveCenter(move+smallMoleculePos[id].center());   
					  if(nodeNote!=NULL)  nodeNote->mySetPos(Move+nodeNote->pos());  break;
			case 'L': updateItemGrid(itemGrid, type, id, move+ANodePos[id].center());  ANodePos[id].moveCenter(move+ANodePos[id].center());   
					  if(nodeNote!=NULL)  nodeNote->mySetPos(Move+nodeNote->pos());  break;
			case 'R': updateItemGrid(itemGrid, type, id, move+reactionPos[id].center());  reactionPos[id].moveCenter(move+reactionPos[id].center());  
					  if(nodeNote!=NULL) 
						  nodeNote->mySetPos(Move+nodeNote->pos());  break;			      
			case 'M': updateItemGrid(itemGrid, type, id, move+compartmentPos[id].center());  			
					  //updateACompartmentPos(id, move, Move); 
				      compartmentPos[id].moveCenter(move+compartmentPos[id].center()); 
					  break; //if(nodeNote!=NULL)  nodeNote->mySetPos(move+nodeNote->pos()); 			      
		}	
	}
	return true;
				
}



void PathBubble1::updateACompartmentPos(int id, QPointF move, QPointF Move)
{ //oupdate compartment pos and the item associated with it
	for(set<vector<int>>::iterator it=_scene->CompartmentContain[m_pathwayID][id].begin(); it!=_scene->CompartmentContain[m_pathwayID][id].end(); it++)
	{   
			vector<int> temp=*it;
			int type=temp[0], i=temp[1];
			CalloutNote* nodeNote=_scene->findNote(m_pathwayID, type, i);	             					
	        switch(type)
			{
				  case 'C': if(complexPos[i].x()>-999.99)       
							{
								updateItemGrid(itemGrid, type, i, move+complexPos[i].center()), complexPos[i].moveCenter(move+complexPos[i].center());	   
					            if(nodeNote!=NULL) 	
									nodeNote->mySetPos(Move+nodeNote->pos());    
							}
							break;
		          case 'P': if(proteinPos[i].x()>-999.99)    
							{
								updateItemGrid(itemGrid, type, i, move+proteinPos[i].center()), proteinPos[i].moveCenter(move+proteinPos[i].center());	     
								if(nodeNote!=NULL)  
									nodeNote->mySetPos(Move+nodeNote->scenePos()); 
							}
							break;
	              case 'D': if(DnaPos[i].x()>-999.99)  
							{
								updateItemGrid(itemGrid, type, i, move+DnaPos[i].center()),  DnaPos[i].moveCenter(move+DnaPos[i].center());		        
					            if(nodeNote!=NULL)
									nodeNote->mySetPos(Move+nodeNote->pos()); 
							}
							break;
		          case 'E': if(physicalEntityPos[i].x()>-999.99)
							{
								updateItemGrid(itemGrid, type, i, move+physicalEntityPos[i].center()), physicalEntityPos[i].moveCenter(move+physicalEntityPos[i].center());  
								if(nodeNote!=NULL)  
									nodeNote->mySetPos(Move+nodeNote->pos()); 
							}
					         break;
				  case 'L': if(ANodePos[i].x()>-999.99)
							{
								updateItemGrid(itemGrid, type, i, move+ANodePos[i].center()), ANodePos[i].moveCenter(move + ANodePos[i].center());  
								if(nodeNote!=NULL)  
									nodeNote->mySetPos(Move+nodeNote->pos()); 
							}
					         break;
		          case 'S': if(smallMoleculePos[i].x()>-999.99)
							{
								updateItemGrid(itemGrid, type, i, move+smallMoleculePos[i].center()),  smallMoleculePos[i].moveCenter(move+smallMoleculePos[i].center());  
								if(nodeNote!=NULL)
									nodeNote->mySetPos(Move+nodeNote->pos());  
							}	
							break;
		          case 'R':	if(reactionPos[i].x()>-999.99)       
							{
								updateItemGrid(itemGrid, type, i, move+reactionPos[i].center());
								reactionPos[i].moveCenter(move+reactionPos[i].center());  
								if(nodeNote!=NULL)  
									nodeNote->mySetPos(Move+nodeNote->pos());  
							}
							break;			      
			}	    
	}	
	compartmentPos[id].moveCenter(move+compartmentPos[id].center()); 
}


void PathBubble1::paintItemDragged(QPainter *painter)
{   
	if(itemDragged.empty())
		return;
	int width = this->realRect().width();
    int height = this->realRect().height();
	painter->setPen(Qt::NoPen);	

	painter->setBrush(QColor(255,255,255,200));
	painter->drawRect(bubbleRect);

	QRectF rect;
	painter->setBrush(Qt::white);
	
	if(bubbleRect.width()!=0)
		return;

	for(int i=0;i<itemDragged.size();i++)
	{		
		rect=itemDragged[i];		
		painter->drawRect(rect);
	}		    
}


void PathBubble1::dropDownMenu2(QGraphicsSceneContextMenuEvent *event)
{	
	QMenu menu;
	QMenu *expressionMenu;
	QMenu *colorMenu;	
	QMenu *graphMenu;	
	QMenu *sketchMenu;	
	QMenu *queryMenu;	

	//QString ss("QMenu::item {  background-color: rgba(90,90,90,255); border: 3px rgba(69,69,69,255); color: white }"); 
	QString ss("QMenu::item {  background-color: black; border: 3px solid black; color: white }");
    menu.setStyleSheet(ss);
    menu.setMinimumWidth(75);

	expressionMenu = menu.addMenu("Expression");
	menu.addMenu(expressionMenu); 
	//text2[0].push_back("Load");  
	//	text2[0].push_back("Expressed"); 
	QAction *expressionLoadAction = expressionMenu->addAction("Load");
	QAction *expressionExpressedAction = expressionMenu->addAction("Expressed");	      

	colorMenu = menu.addMenu("Color");
	menu.addMenu(colorMenu);  
	//text2[1].push_back("Up"); text2[1].push_back("Even"); text2[1].push_back("Down");
	QAction *colorUpAction = colorMenu->addAction("Up");   
	QAction *colorEvenAction = colorMenu->addAction("Even");   
	QAction *colorDownAction = colorMenu->addAction("Down");   
		 
	graphMenu = menu.addMenu("Graph");
	menu.addMenu(graphMenu);   
	//text2[2].push_back("Reset"); text2[2].push_back("Undo"); text2[2].push_back("Redo"); text2[2].push_back("Reload"); 
	QAction *graphResertAction = graphMenu->addAction("Reset"); 
	QAction *graphUndoAction = graphMenu->addAction("Undo"); 
	QAction *graphRedoAction = graphMenu->addAction("Redo"); 
	QAction *graphReloadAction;// = graphMenu->addAction("Reload"); 		
	if(toplevel)
		graphReloadAction = graphMenu->addAction("Reload"); 		

	//sketchMenu = menu.addMenu("Sketch");
	QAction *sketchAction = menu.addAction("Sketch");   	

	//queryMenu = menu.addMenu("Query");
	QAction *queryAction = menu.addAction("Query");    
	QAction *selectedAction = menu.exec( event->screenPos() );		

	
	if (selectedAction == expressionLoadAction)
	{
	    menuSelection(pos_menu, scenePos_menu, 0);
	}
	else if (selectedAction == expressionExpressedAction  )
	{
	    menuSelection(pos_menu, scenePos_menu, 1);
	}
	else if (selectedAction == colorUpAction)
	{
	    menuSelection(pos_menu, scenePos_menu, 10);
	}
	else if (selectedAction == colorEvenAction)
	{
	    menuSelection(pos_menu, scenePos_menu, 11);
	}
	else if (selectedAction == colorDownAction)
	{
	    menuSelection(pos_menu, scenePos_menu, 12);
	}
	else if (selectedAction == graphResertAction)
	{
	    menuSelection(pos_menu, scenePos_menu, 20);
	}
	else if (selectedAction == graphUndoAction)
	{
	    menuSelection(pos_menu, scenePos_menu, 21);
	}
	else if (selectedAction == graphRedoAction)
	{
	    menuSelection(pos_menu, scenePos_menu, 22);
	}
	else if (toplevel && selectedAction == graphReloadAction)
	{
	    menuSelection(pos_menu, scenePos_menu, 25);
	}
	else if (selectedAction == sketchAction)
	{
	    menuSelection(pos_menu, scenePos_menu, 39);
	}
	else if (selectedAction == queryAction)
	{
	    menuSelection(pos_menu, scenePos_menu, 49);
	}
}
//pie menu
/*

			text.push_back(tr("Share"));  color.push_back(QColor(192,192,192,255));
			text.push_back(tr("Diff"));   color.push_back(QColor(192,192,192,255));
			text.push_back(tr("Link"));        color.push_back(QColor(192,192,192,255));
			text.push_back(tr("Tools"));        color.push_back(QColor(192,192,192,255));
			
			text2.push_back(empty); text2.push_back(empty); text2.push_back(empty); text2.push_back(empty); //text2.push_back(empty); text2.push_back(empty);
			color2.push_back(cempty); color2.push_back(cempty); color2.push_back(cempty); color2.push_back(cempty); //color2.push_back(cempty); color2.push_back(cempty);
			disables2.push_back(dempty); disables2.push_back(dempty); disables2.push_back(dempty); disables2.push_back(dempty); //disables2.push_back(dempty); disables2.push_back(dempty);

            text2[0].push_back("P");  text2[0].push_back("C");  text2[0].push_back("PA"); 
			color2[0].push_back(QColor(192,192,192,255));
			color2[0].push_back(QColor(192,192,192,255));
			color2[0].push_back(QColor(192,192,192,255));
			disables2[0].push_back(false);	disables2[0].push_back(false);	 disables2[0].push_back(false);	

			text2[1].push_back("P");  text2[1].push_back("C");  text2[1].push_back("PA"); 
			color2[1].push_back(QColor(192,192,192,255));
			color2[1].push_back(QColor(192,192,192,255));
			color2[1].push_back(QColor(192,192,192,255));
			disables2[1].push_back(false);	disables2[1].push_back(false);	 disables2[1].push_back(false);	

			text2[2].push_back("P");  text2[2].push_back("C"); // text2[2].push_back("PA"); 
			color2[2].push_back(QColor(192,192,192,255));
			color2[2].push_back(QColor(192,192,192,255));
			//color2[2].push_back(QColor(192,192,192,255));
			disables2[2].push_back(false);	disables2[2].push_back(false);	 //disables2[2].push_back(false);	

			text2[3].push_back("Undo");  text2[3].push_back("Redo");  //text2[3].push_back("Clear"); 
			color2[3].push_back(QColor(192,192,192,255));
			color2[3].push_back(QColor(192,192,192,255));
			//color2[3].push_back(QColor(192,192,192,255));
			disables2[3].push_back(false);	disables2[3].push_back(false);	 //disables2[3].push_back(false);	

			//text2[3].push_back("delete");  text2[3].push_back("Degree"); text2[3].push_back("Outdegree"); text2[3].push_back("Indegree"); 
			//color2[3].push_back(QColor(179,179,179,100)); 
			//color2[3].push_back(QColor(179,179,179,100)); 
			//color2[3].push_back(QColor(179,179,179,100)); 
			//color2[3].push_back(QColor(179,179,179,100)); 
			
			//disables2[3].push_back(false);	disables2[3].push_back(false);	disables2[3].push_back(false);	disables2[3].push_back(false);	
			
			//if(_sizeType==1)       disables2[3][0]=true;	
			//else if(_sizeType==2)  disables2[3][1]=true;	
			//else if(_sizeType==3)  disables2[3][2]=true;	
			//else if(_sizeType==4)  disables2[3][3]=true;		
			
			//text2.push_back(empty); color2.push_back(cempty);  disables2.push_back(dempty);			
			//text.push_back(tr("Hide"));   color.push_back(QColor(179,179,179,100));			
			//disables.push_back(false);	

*/

void PathBubble1::dropDownMenu4(QGraphicsSceneContextMenuEvent *event)
{	
	//text.push_back(tr("Share"));  color.push_back(QColor(192,192,192,255));
	//text.push_back(tr("Diff"));   color.push_back(QColor(192,192,192,255));
	//text.push_back(tr("Link"));        color.push_back(QColor(192,192,192,255));
	//text.push_back(tr("Tools"));        color.push_back(QColor(192,192,192,255));
	QMenu menu;

	//QString ss("QMenu::item {  background-color: rgba(90,90,90,255); border: 3px rgba(69,69,69,255); color: white }"); 
	QString ss("QMenu::item {  background-color: black; border: 3px solid black; color: white }");
    menu.setStyleSheet(ss);
    menu.setMinimumWidth(75);

	QMenu *searchMenu;
	QMenu *shareMenu;
	QMenu *diffMenu;	
	QMenu *linkMenu;	
	QMenu *editMenu;	
	
	QMenu *expressionMenu;
	QMenu *colorMenu;	
	QMenu *toolMenu;	
	QMenu *sketchMenu;	
	QMenu *queryMenu;	

	searchMenu = menu.addMenu("Search");

	shareMenu = searchMenu->addMenu("Share");
	searchMenu->addMenu(shareMenu); 
	QAction *sharePAction = shareMenu->addAction("Protein");
	QAction *shareCAction = shareMenu->addAction("Compartment");	      
	QAction *sharePAAction = shareMenu->addAction("Pathway");	 

	diffMenu = searchMenu->addMenu("Diff");
	searchMenu->addMenu(diffMenu);  	
	QAction *diffPAction = diffMenu->addAction("Protein");
	QAction *diffCAction = diffMenu->addAction("Compartment");	      
	QAction *diffPAAction = diffMenu->addAction("Pathway");	 
		 
	linkMenu = searchMenu->addMenu("Link");
	searchMenu->addMenu(linkMenu);   	
	QAction *linkPAction = linkMenu->addAction("Protein");
	QAction *linkCAction = linkMenu->addAction("Compartment");	      
	QAction *linkPAAction = linkMenu->addAction("Pathway");	 

	expressionMenu = menu.addMenu("Expression");
	menu.addMenu(expressionMenu); 
	//text2[0].push_back("Load");  
	//	text2[0].push_back("Expressed"); 
	//QAction *expressionLoadAction = expressionMenu->addAction("Load");
	QAction *expressionExpressedAction = expressionMenu->addAction("Expressed");	      

	editMenu = menu.addMenu("Edit");
	toolMenu = menu.addMenu("Tools");

	//sketchMenu = menu.addMenu("Sketch");
	QAction *sketchAction = toolMenu->addAction("Sketch");   

	colorMenu = toolMenu->addMenu("Color");
	toolMenu->addMenu(colorMenu);  
	//text2[1].push_back("Up"); text2[1].push_back("Even"); text2[1].push_back("Down");
	QAction *colorUpAction = colorMenu->addAction("Up");   
	QAction *colorEvenAction = colorMenu->addAction("Even");   
	QAction *colorDownAction = colorMenu->addAction("Down");   

	//queryMenu = menu.addMenu("Query");
	QAction *queryAction = toolMenu->addAction("Query");    

	
	menu.addMenu(editMenu);   	
	QAction *toolUndoAction = editMenu->addAction("Undo");
	QAction *toolRedoAction = editMenu->addAction("Redo");	      
	//QAction *toolClearAction = toolMenu->addAction("Clear");	 	
	//graphMenu = menu.addMenu("Graph");
	//menu.addMenu(graphMenu);   
	//text2[2].push_back("Reset"); text2[2].push_back("Undo"); text2[2].push_back("Redo"); text2[2].push_back("Reload"); 
	QAction *graphResertAction = editMenu->addAction("Graph Reset"); 
	QAction *graphUndoAction = editMenu->addAction("Graph Undo"); 
	QAction *graphRedoAction = editMenu->addAction("Graph Redo"); 
	QAction *graphReloadAction;// = graphMenu->addAction("Reload"); 		
	if(toplevel)
		graphReloadAction = editMenu->addAction("Graph Reload"); 	
	
	QAction *selectedAction = menu.exec( event->screenPos() );		

	/*if (selectedAction == expressionLoadAction)
	{
	    menuSelection(pos_menu, scenePos_menu, 0);
	}
	else*/
	if (selectedAction == expressionExpressedAction  )
	{
	    menuSelection(pos_menu, scenePos_menu, 1);
	}
	else if (selectedAction == colorUpAction)
	{
	    menuSelection(pos_menu, scenePos_menu, 10);
	}
	else if (selectedAction == colorEvenAction)
	{
	    menuSelection(pos_menu, scenePos_menu, 11);
	}
	else if (selectedAction == colorDownAction)
	{
	    menuSelection(pos_menu, scenePos_menu, 12);
	}
	else if (selectedAction == graphResertAction)
	{
	    menuSelection(pos_menu, scenePos_menu, 20);
	}
	else if (selectedAction == graphUndoAction)
	{
	    menuSelection(pos_menu, scenePos_menu, 21);
	}
	else if (selectedAction == graphRedoAction)
	{
	    menuSelection(pos_menu, scenePos_menu, 22);
	}
	else if (toplevel && selectedAction == graphReloadAction)
	{
	    menuSelection(pos_menu, scenePos_menu, 25);
	}
	else if (selectedAction == sketchAction)
	{
	    menuSelection(pos_menu, scenePos_menu, 39);
	}
	else if (selectedAction == queryAction)
	{
	    menuSelection(pos_menu, scenePos_menu, 49);
	}	
	if (selectedAction == sharePAction)
	{
	    menuSelection_1(pos_menu, scenePos_menu, 0);		
	}
	else if (selectedAction == shareCAction )
	{
	    menuSelection_1(pos_menu, scenePos_menu, 1);
	}
	else if (selectedAction == sharePAAction)
	{
	    menuSelection_1(pos_menu, scenePos_menu, 2);
	}
	else if (selectedAction == diffPAction)
	{
	    menuSelection_1(pos_menu, scenePos_menu, 10);
	}
	else if (selectedAction == diffCAction)
	{
	    menuSelection_1(pos_menu, scenePos_menu, 11);
	}
	else if (selectedAction == diffPAAction)
	{
	    menuSelection_1(pos_menu, scenePos_menu, 12);
	}
	else if (selectedAction == linkPAction)
	{
	    menuSelection_1(pos_menu, scenePos_menu, 20);
	}
	else if (selectedAction == linkCAction)
	{
	    menuSelection_1(pos_menu, scenePos_menu, 21);
	}
	else if (selectedAction == linkPAAction)
	{
	    menuSelection_1(pos_menu, scenePos_menu, 22);
	}
	else if (selectedAction == toolUndoAction)
	{
	    menuSelection_1(pos_menu, scenePos_menu, 30);
	}
	else if (selectedAction == toolRedoAction)
	{
	    menuSelection_1(pos_menu, scenePos_menu, 31);
	}
	
}


void PathBubble1::dropDownMenu3(QGraphicsSceneContextMenuEvent *event)
{	
	//text.push_back(tr("Share"));  color.push_back(QColor(192,192,192,255));
	//text.push_back(tr("Diff"));   color.push_back(QColor(192,192,192,255));
	//text.push_back(tr("Link"));        color.push_back(QColor(192,192,192,255));
	//text.push_back(tr("Tools"));        color.push_back(QColor(192,192,192,255));
	QMenu menu;

	//QString ss("QMenu::item {  background-color: rgba(90,90,90,255); border: 3px rgba(69,69,69,255); color: white }"); 
	QString ss("QMenu::item {  background-color: black; border: 3px solid black; color: white }");
    menu.setStyleSheet(ss);
    menu.setMinimumWidth(50);

	QMenu *shareMenu;
	QMenu *diffMenu;	
	QMenu *linkMenu;	
	QMenu *toolMenu;	

	shareMenu = menu.addMenu("Share");
	menu.addMenu(shareMenu); 
	QAction *sharePAction = shareMenu->addAction("Protein");
	QAction *shareCAction = shareMenu->addAction("Compartment");	      
	QAction *sharePAAction = shareMenu->addAction("Pathway");	 

	diffMenu = menu.addMenu("Diff");
	menu.addMenu(diffMenu);  	
	QAction *diffPAction = diffMenu->addAction("Protein");
	QAction *diffCAction = diffMenu->addAction("Compartment");	      
	QAction *diffPAAction = diffMenu->addAction("Pathway");	 
		 
	linkMenu = menu.addMenu("Link");
	menu.addMenu(linkMenu);   	
	QAction *linkPAction = linkMenu->addAction("Protein");
	QAction *linkCAction = linkMenu->addAction("Compartment");	      
	QAction *linkPAAction = linkMenu->addAction("Pathway");	 

	toolMenu = menu.addMenu("Tools");
	menu.addMenu(toolMenu);   	
	QAction *toolUndoAction = toolMenu->addAction("Undo");
	QAction *toolRedoAction = toolMenu->addAction("Redo");	      
	//QAction *toolClearAction = toolMenu->addAction("Clear");	 
	
	QAction *selectedAction = menu.exec( event->screenPos() );		
	
	if (selectedAction == sharePAction)
	{
	    menuSelection_1(pos_menu, scenePos_menu, 0);
	}
	else if (selectedAction == shareCAction )
	{
	    menuSelection_1(pos_menu, scenePos_menu, 1);
	}
	else if (selectedAction == sharePAAction)
	{
	    menuSelection_1(pos_menu, scenePos_menu, 2);
	}
	else if (selectedAction == diffPAction)
	{
	    menuSelection_1(pos_menu, scenePos_menu, 10);
	}
	else if (selectedAction == diffCAction)
	{
	    menuSelection_1(pos_menu, scenePos_menu, 11);
	}
	else if (selectedAction == diffPAAction)
	{
	    menuSelection_1(pos_menu, scenePos_menu, 12);
	}
	else if (selectedAction == linkPAction)
	{
	    menuSelection_1(pos_menu, scenePos_menu, 20);
	}
	else if (selectedAction == linkCAction)
	{
	    menuSelection_1(pos_menu, scenePos_menu, 21);
	}
	else if (selectedAction == linkPAAction)
	{
	    menuSelection_1(pos_menu, scenePos_menu, 22);
	}
	else if (selectedAction == toolUndoAction)
	{
	    menuSelection_1(pos_menu, scenePos_menu, 30);
	}
	else if (selectedAction == toolRedoAction)
	{
	    menuSelection_1(pos_menu, scenePos_menu, 31);
	}
	
}

bool PathBubble1::isHighlightorshared()
{
    bool highlightorshared=false;
	QList<ItemBase *> mlist=_scene->getGroupMembers(this);	 
	for(int i=0; i<mlist.size(); i++) 
	{
		 if(mlist[i]->getType()==PATHBUBBLE1 || mlist[i]->getType()==SUBPATHBUBBLE1)
		 {	
			PathBubble1* pbubble = dynamic_cast<PathBubble1*>(mlist[i]);		 		 
			if(!pbubble->highlighted.empty())   
			{  
				int count=0;
				for(set<vector<int>>::iterator it=pbubble->highlighted.begin(); it!=pbubble->highlighted.end(); it++)
				{
					if((*it).empty())
					    count++; 				
				}
				if(pbubble->highlighted.size()!=count)
				{
				    highlightorshared=true;
				    break;
				}
			}	
			if(!pbubble->differedStored.empty())   
			{  
				int count=0;
				for(int j=0; j<pbubble->differedStored.size(); j++)
				{
					if(pbubble->differedStored[j].empty())
					    count++; 				
				}
				if(pbubble->differedStored.size()!=count)
				{
				    highlightorshared=true;
				    break;
				}
			}		  
			
			if(!pbubble->linkedStored.empty())   
			{  
				int count=0;
				for(int j=0; j<pbubble->linkedStored.size(); j++)
				{
					if(pbubble->linkedStored[j].empty())
					    count++; 				
				}
				if(pbubble->linkedStored.size()!=count)
				{
				    highlightorshared=true;
				    break;
				}				
			}	
			if(!pbubble->sharedStored.empty())   
			{  
				int count=0;
				for(int j=0; j<pbubble->sharedStored.size(); j++)
				{
					if(pbubble->sharedStored[j].empty())
					    count++; 				
				}
				if(pbubble->sharedStored.size()!=count)//keqin
				{
				    highlightorshared=true;
				    break;
				}
			}	
		 }
	}
	return highlightorshared;
}

void PathBubble1::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
		if(insideBubble(event->pos()))
		{
		   UNGROUP=false;
		   AFTERUNGROUP=false;
		}
		if(m_isMoving)
		{
		   m_isMoving=false;
		   return;
		}	
	//_scene->dropDownMenu0(event, this);	
    _penisOn=false;  _grabisOn=false;  _queryisOn=false;  _upisOn=_downisOn=_evenisOn=false;	
	//vector<int> item=whichItem(event->pos(),this->Width()*_scale, this->Height()*_scale);
	//_scene->piemenu
	bool highlightorshared = isHighlightorshared();

	vector<int> node=whichItem(event->pos(),this->Width()*_scale, this->Height()*_scale);
	vector<int> node2;
	if(!node.empty())
	{
		node2.push_back(m_pathwayID); node2.push_back(node[0]); node2.push_back(node[1]);
	}
	if(AllRegion().contains(QPoint(event->pos().x(),event->pos().y())))
	     _scene->dropDownMenu0(event, this, node2);
	else _scene->dropDownMenu0(event, NULL, node2);
	return;

	if(onBoarder( event->pos()))
	{
		QList<ItemBase *> mList=_scene->getGroupMembers(this);	
		QMenu menu;		
		//QString ss("QMenu::item {  background-color: rgba(50,50,50,255); border: 3px rgba(69,69,69,255); color: white }"); // Use background-color instead of background		
		QString ss("QMenu::item {  background-color: black; border: 3px solid black; color: white }");
        menu.setStyleSheet(ss);
        menu.setMinimumWidth(70);
		if(mList.size()>1)
		{			
			QAction *delAction = menu.addAction("Delete bubble");	
		    QAction *delGroupAction = menu.addAction("Delete group");	
			
			
			QAction *selectedAction = menu.exec( event->screenPos() );		
				
			if (selectedAction == delAction)
			{
				QMessageBox msgBox;
				msgBox.move(event->scenePos().x(), event->scenePos().y());
				msgBox.setWindowTitle("Delete Bubble");
				msgBox.setText("Delete this bubble?");
				msgBox.setStandardButtons(QMessageBox::Yes);
				msgBox.addButton (QMessageBox::No);				
				if(msgBox.exec() == QMessageBox::Yes)
				{
					int size=_pathBubbles.size(),psize=-1;	
					QRectF result(-10000,-10000,-10000,-10000);
					if(_pathBubbleParent!=NULL)
					{
						 result=_pathBubbleParent->ResetGraph_1();
					}					
					_scene->bubbleDeleted(this, result);					
					deleteSelectedItems(_scene);															
				}
				else
				{
				   
				}
			}

			if (selectedAction == delGroupAction)
			{
				QMessageBox msgBox;			
				msgBox.setWindowTitle("Delete Group");
				msgBox.setText("Delete this group?");
				msgBox.setStandardButtons(QMessageBox::Yes);
				msgBox.addButton (QMessageBox::No);
				msgBox.move(event->scenePos().x(), event->scenePos().y());
				if(msgBox.exec() == QMessageBox::Yes)
				{
					QList<ItemBase *> mlist=_scene->getGroupMembers(this);
					OpenGLScene *tscene=_scene;
					_scene->delectionStarter = this;
					vector<int> plist, clist,tlist;
					int pid=m_pathwayID;
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
					if(pid<0 || (pid>=0&&tscene->_pathBubbles[pid]!=NULL))
					{
						deleteSelectedItems(tscene);	
					}
					
				}				
			}
			return;
		}
		else 
		{
			QAction *delAction = menu.addAction("Delete bubble");			   	
			QAction *selectedAction = menu.exec( event->screenPos() );						
			if (selectedAction == delAction)
			{
				QMessageBox msgBox;				
				msgBox.setWindowTitle("Delete Bubble");
				msgBox.setText("Delete this bubble?");
				msgBox.setStandardButtons(QMessageBox::Yes);
				msgBox.addButton (QMessageBox::No);
				msgBox.move(event->scenePos().x(), event->scenePos().y());
				if(msgBox.exec() == QMessageBox::Yes)
				{
					
					QRectF result(-10000,-10000,-10000,-10000);
					if(_pathBubbleParent!=NULL)
					{
                        //PathBubbleDelectionUpdate(m_pathwayID);
						 
						result=_pathBubbleParent->ResetGraph_1();		
					}					
					_scene->bubbleDeleted(this, result);
					deleteSelectedItems(_scene);
						
				}
			}			
		}
		return;
	}
	
	/*if(highlightorshared)// || ifExpressionShown()	)	
	{
	    // pieMenuOn=true;
		//_pieMenuState=1;
		menuType=1;
		//dropDownMenu4(event);
		
		//_scene->_bigBubbleID_menu=_bigBubbleID;
		//_scene->_barChartID_menu=_barChartID;
		
	}
	//if(itermid>-1)
	else 
	{   
		//pieMenuOn=true;
		//_pieMenuState=1;
		menuType=2;		
		{
			dropDownMenu4(event);
			
		}		
	}*/	
}

QRectF PathBubble1::updateBubbleSize()
{
	float sizex, sizey;
    //sizex=maxx*50, sizey=sumy*25;	
		
	sizex=fixedWidth;
	sizey=fixedHeight;
	//_scale = (maxx*80 + sumy*40)/2/graphReferenceSize;
		
	resizeItem(sizex , sizey);	

	float Px=this->pos().x(),Py=this->pos().y();
	QRectF result(0, 0, sizex, sizey);
	result.moveCenter(QPointF(Px, Py));
	
	
	if(sizey/2 + this->pos().y() > this->screenHeight-100)
	{
	    Py=this->screenHeight-sizey/2-100;		
	}
	else if( this->pos().y() - sizey/2 < 60 )
	{
		//this->mySetPos( QPointF(this->pos().x(), sizey/2 + 50) );
	    Py=sizey/2 + 60; 
	}
	if(this->pos().x()-sizex/2 < 20)
	{
		Px=sizex/2+20; 
	}
    	
	this->mySetPos( QPointF(Px,Py) ); //position
	return result;	
}				   

QRectF PathBubble1::updateBubbleSize_3()
{
	//resize according to bubble size based on Ranks and Rank0
	//also update bubble location
	//vector<int> item(2,0);
	float maxx=-100000,sumy=0;	
	for(int i=0; i<Rank0.size(); i++)
	{	
		 vector<int> Rank0_1;
		 float maxy=-100000;
		 for(int j=0; j<Rank0[i].size(); j++) 
		 {
		  	   vector<int> Rank_2;
			   int roomID=Rank0[i][j];
			   float sum=0;
			   if(roomID < _compartment.size())
			   {
					if(_newCompartmentPos[roomID].x()>-999.99)// && !newRanks[Rank0[i][j]].empty())
					{
						 if(maxy<Ranks[roomID].size())
							 maxy=Ranks[roomID].size();
						 float maxx1=-100000;
				         for(int k=0; k<Ranks[roomID].size(); k++)		 
						 {
						     if(maxx1<Ranks[roomID][k].size())
							     maxx1=Ranks[roomID][k].size();
						 }
						 sum=sum+maxx1;
					}
			   }
			   if(maxx<sum)
				   maxx=sum;
		 }
		 sumy=sumy+maxy;
	}
	float sizex, sizey;
	
		sizex=maxx*50, sizey=sumy*25;	
		if(sizex>graphReferenceSize && sizex > sizey)
		{
			_scale=sizex/graphReferenceSize; sizex=graphReferenceSize; 
		}
		if(sizey>graphReferenceSize && sizey > sizex)
		{
			_scale=sizey/graphReferenceSize; sizey=graphReferenceSize;
		}
		if(sizex<graphReferenceSize/2.0 && sizex < sizey)
			sizex=graphReferenceSize/2.0;

		if(sizey<graphReferenceSize/2.0 && sizey < sizex)
			sizey=graphReferenceSize/2.0;

		if(sizex < sizey*1.0/3.0) sizex = sizey*1.0/3.0;
		else if(sizey < sizex*1.0/3.0) sizey = sizex*1.0/3.0;
	
	resizeItem(sizex , sizey);	
	//also update bubble location if it exceed the screen
	//this->screenHeight 
	//this->screenWidth
	float Px=this->pos().x(),Py=this->pos().y();
	QRectF result(0, 0, sizex, sizey);
	result.moveCenter(QPointF(Px, Py));
	
	bool flag=false;
	if(sizey/2 + this->pos().y() > this->screenHeight-100)
	{
		//this->mySetPos( QPointF(this->pos().x(), this->screenHeight-sizey/2) );
	    Py=this->screenHeight-sizey/2-100;
		flag=true;
	}
	else if( this->pos().y() - sizey/2 < 60 )
	{
		//this->mySetPos( QPointF(this->pos().x(), sizey/2 + 50) );
	    Py=sizey/2 + 60; flag=true;
	}
	if(this->pos().x()-sizex/2 < 20)
	{
		Px=sizex/2+20; flag=true;
	}
	
	this->mySetPos( QPointF(Px,Py) ); //position
	
	return result;
	//QRectF rect=sceneBoundingRect();
	//rect=rect;
}				   

QRectF PathBubble1::updateBubbleSize_2()
{ //resize according to bubble size based on Ranks and Rank0
	//also update bubble location
	//vector<int> item(2,0);
	float maxx=-100000,sumy=0;	
	for(int i=0; i<Rank0.size(); i++)
	{	
		 vector<int> Rank0_1;
		 float maxy=-100000;
		 for(int j=0; j<Rank0[i].size(); j++) 
		 {
		  	   vector<int> Rank_2;
			   int roomID=Rank0[i][j];
			   float sum=0;
			   if(roomID < _compartment.size())
			   {
					if(_newCompartmentPos[roomID].x()>-999.99)// && !newRanks[Rank0[i][j]].empty())
					{
						 if(maxy<Ranks[roomID].size())
							 maxy=Ranks[roomID].size();
						 float maxx1=-100000;
				         for(int k=0; k<Ranks[roomID].size(); k++)		 
						 {
						     if(maxx1<Ranks[roomID][k].size())
							     maxx1=Ranks[roomID][k].size();
						 }
						 sum=sum+maxx1;
					}
			   }
			   if(maxx<sum)
				   maxx=sum;
		 }
		 sumy=sumy+maxy;
	}
	float sizex, sizey;
	if(!fixedSize)
	{
		sizex=maxx*50, sizey=sumy*25;	
		if(sizex>graphReferenceSize && sizex > sizey)
		{
			_scale=sizex/graphReferenceSize; sizex=graphReferenceSize; 
		}
		if(sizey>graphReferenceSize && sizey > sizex)
		{
			_scale=sizey/graphReferenceSize; sizey=graphReferenceSize;
		}
		if(sizex<graphReferenceSize/2.0 && sizex < sizey)
			sizex=graphReferenceSize/2.0;

		if(sizey<graphReferenceSize/2.0 && sizey < sizex)
			sizey=graphReferenceSize/2.0;

		if(sizex < sizey*1.0/3.0) sizex = sizey*1.0/3.0;
		else if(sizey < sizex*1.0/3.0) sizey = sizex*1.0/3.0;
		resizeItem(sizex , sizey);	
	}
	else
	{
	    //sizex=maxx*50, sizey=sumy*25;	
		sizex=fixedWidth;
		sizey=fixedHeight;
	}

	
	//also update bubble location if it exceed the screen
	//this->screenHeight 
	//this->screenWidth
	float Px=this->pos().x(),Py=this->pos().y();
	QRectF result(0, 0, sizex, sizey);
	result.moveCenter(QPointF(Px, Py));
	
	/*bool flag=false;
	if(sizey/2 + this->pos().y() > this->screenHeight-50)
	{
		//this->mySetPos( QPointF(this->pos().x(), this->screenHeight-sizey/2) );
	    Py=this->screenHeight-sizey/2-50;
		flag=true;
	}
	else if( this->pos().y() - sizey/2 < 50 )
	{
		//this->mySetPos( QPointF(this->pos().x(), sizey/2 + 50) );
	    Py=sizey/2 + 50; flag=true;
	}
	if(this->pos().x()-sizex/2 < 10)
	{
		Px=sizex/2+10; flag=true;
	}
	
	this->mySetPos( QPointF(Px,Py) ); //position
	*/
	return result;
	//QRectF rect=sceneBoundingRect();
	//rect=rect;
}				   

void PathBubble1::unGroupbyDelection()
{
	emit unGroup( this );
    logUnGroup();
    UNGROUP=false;
	AFTERUNGROUP=true;	
}
	

void PathBubble1::selfDelection(PathBubble1* b0, PathBubble1* pb)
{	//pb to be deleted				
	//_scene->bubbleDeleted(this, result);					
	//deleteSelectedItems(_scene);										
	//menuSelection(QPointF(0,0), QPointF(0,0), 49);		
	if(pb!=this)
		return;

	if(!samePathwayGroup(b0, pb))
    {
		return;
	}
	int level1=getPathBubbleLevel(pb);
	int level2=getPathBubbleLevel(b0);
	if(level2 >= level1)
		return;	
		
		if (!scene)
			return;
		
		int size=_pathBubbles.size(),psize=-1;	
	
		//_pieMenuState=0;
		_penisOn=false;	
		_grabisOn=false;	
		_queryisOn=false;
		_upisOn=_downisOn=_evenisOn=false;
		_lines.clear();	

		size=_pathBubbles.size();	 //replace its child's parent with its parent
		
		for(int i=0; i<size; i++)
		{	
			if(_pathBubbles[i])
			{
				if(_pathBubbles[i]->isVisible())
				{
					_pathBubbles[i]->_pathBubbleParent = NULL;
					_pathBubbles[i]->_parentPathBubbleID = -1;
					_scene->_quantityName[_pathBubbles[i]->m_pathwayID] = _scene->_quantityName[m_pathwayID];
					_scene->_quantity[_pathBubbles[i]->m_pathwayID] = _scene->_quantity[m_pathwayID]; 
					_scene->_minmax[_pathBubbles[i]->m_pathwayID] = _scene->_minmax[m_pathwayID]; 

					if(_pathBubbleParent && _pathBubbleParent->isVisible())
					{
						_pathBubbles[i]->_pathBubbleParent = _pathBubbleParent;
						_pathBubbleParent->_pathBubbles.append(_pathBubbles[i]);
						_pathBubbleParent->_groupPoint.append(this->_groupPoint);
						_pathBubbleParent->_grouphighlight.append(this->_grouphighlight);
						_pathBubbleParent->_groupEdgeAffected.append(this->_groupEdgeAffected);		
						_pathBubbles[i]->_parentPathBubbleID = _pathBubbleParent->m_pathwayID;				
					}					
				}
			}
			//_scene->bubbleDeleted(_pathBubbles.last());			    
		}
	
		if(b0==_pathBubbleParent)
		{
			if(_pathBubbleParent!=NULL)
			{
				 PathBubbleDelectionUpdate(m_pathwayID);	
				 //updateConnectionAfterDeletion(); 	
			}	
		}
		else if(b0!=NULL)
			PathBubbleDelectionUpdate(b0, pb);		

		removeFromGraph();
		//deleteSelectedItemKids(scene);	
	
		//PathBubbleDelectionUpdate(m_pathwayID);
		logRemove();			
		if(_pathBubbleParent!=NULL)
		{
			for (int i = 0; i < _pathBubbleParent->_pathBubbles.size(); i ++)
			{
				subPathBubble1* tmp= dynamic_cast<subPathBubble1*>(this);

				if(_pathBubbleParent->_pathBubbles[i]==tmp)
				{	
					tmp->setVisible(false);
					int id=_pathBubbleParent->_pathBubbles[i]->m_pathwayID;
					_pathBubbleParent->_pathBubbles.removeOne(tmp);
					_pathBubbleParent->_grouphighlight.removeOne(_pathBubbleParent->_grouphighlight[i]);
					_pathBubbleParent->_groupEdgeAffected.removeOne(_pathBubbleParent->_groupEdgeAffected[i]);
					_pathBubbleParent->_groupPoint.removeOne(_pathBubbleParent->_groupPoint[i]);
					break;
				}
			}			
			_pathBubbleParent->ResetGraph_2();			
		}	


		_pathBubbleParent=NULL;
	
		//ItemBase::deleteSelectedItems(scene);		
		if(_scene!=NULL )
		{
			/*QString pathName;
			for( int i = 0; i <_scene->_nodeNote.size(); i++)
			{
				int pid=_scene->_nodeNote[i]->_pid;
				//if( !_scene->_nodeNote[i]->deleted )
				{
					if(pid==this->m_pathwayID)
					{
						if(_scene->_nodeNote[i]!=NULL) 
						{
						    
							_scene->_nodeNote[i]->_pid= b0->m_pathwayID, _scene->_nodeNote[i]->_id=_scene->_complexPos[pathwayID].size();
						}
						if(pathName.size()==0)
							pathName = _scene-> pwdParser-> savePathNotePreparation(_pathName, _scene->userName);	
						_scene->_nodeNote[i]->record(pathName, _scene->_nodeNote[i]->deleted);	

						_scene->_nodeNote[i]->deleted=true;
						scene->_nodeNote[i]->hide();
						scene->_nodeNote[i]->m_note->hide();						
						scene->_nodeNote[i]->m_note->setVisible(false);
					}
				}			
			}	
			*/
			updateTreeRingConnectionAfterDeletion();		
		    updateConnectionAfterDeletion(); 				
		}
        pb->setVisible(false);
		pb->hide();		
		return;
}


vector< vector < set< vector <int> > > >  PathBubble1::getContainedSets_1(int pflag, int cflag, int wflag, bool searchDiffered)
{ //for differed
	vector< vector < set< vector <int> > > > cItem;
    set<vector<int>> sID;
	vector<PathBubble1 *> mlist= _scene->extractPathBubblesinGroup(this);
    for(int i=0; i<mlist.size(); i++) 
	{		
		PathBubble1* pbubble = mlist[i];		 
		for(set<vector<int>>::iterator it = pbubble->highlighted.begin(); it != pbubble->highlighted.end(); it++)
		{
			vector<int> tmp,item=*it;
			int type=item[0],id=item[1];		
			tmp.push_back(pbubble->m_pathwayID); 
			if(type=='W')
			{
			  	tmp.push_back(id);
				tmp.push_back(-1); 
			    tmp.push_back(-1);
				tmp.push_back(type); 			  
			}
			else if(type=='M')
			{
				tmp.push_back(-1);
				tmp.push_back(id); 
			    tmp.push_back(-1);
				tmp.push_back(type); 
				if(id>=_scene->CompartmentContain[pbubble->m_pathwayID].size()) //do not know about reason
					continue;
			}
			else
			{
				tmp.push_back(0);			  			      
				vector<int> oitem;
				oitem.push_back(type); oitem.push_back(id);
				bool flag=false;
		        for(int m=0; m< _scene->CompartmentContain[pbubble->m_pathwayID].size()-1;m++)
				{
				    if(_scene->CompartmentContain[pbubble->m_pathwayID][m].find(oitem) != _scene->CompartmentContain[pbubble->m_pathwayID][m].end())
					{
					    tmp.push_back(m);
						flag=true;
						break;					  
					}
				}				  
				if(!flag)
                    tmp.push_back(-1);
				tmp.push_back(id);
				tmp.push_back(type); 
			}
			sID.insert(tmp);			
		}
	}	
	
	if(searchDiffered) //if the highlighted item do not have containing relationship, get all the item in its same level
	{
	    //{pflag && cflag, int wflag
		for(int i=0; i<mlist.size(); i++) 
		{
			PathBubble1* pbubble = mlist[i];			
			cItem.push_back(pbubble->searchDifferedNode( sID, pflag, cflag, wflag));   	
		}
	    //searchDifferedNode(ctype, nodeName, nodePos, sID, edgeSet, nodeSet, cItem, markedNodes,  sharedNode);
	}
	
    return  cItem;    
}


vector< vector < set< vector <int> > > >  PathBubble1::getContainedSets(int pflag, int cflag, int wflag, bool AndFlag)
{ //for P N, U
	vector< vector < set< vector <int> > > > cItem;
    set<vector<int>> sID;
	vector<PathBubble1 *> mlist= _scene->extractPathBubblesinGroup(this);
    for(int i=0; i<mlist.size(); i++) 
	{		
		PathBubble1* pbubble = mlist[i];		 
		for(set<vector<int>>::iterator it = pbubble->highlighted.begin(); it != pbubble->highlighted.end(); it++)
		{
			vector<int> tmp,item=*it;
			int type=item[0],id=item[1];		
			tmp.push_back(pbubble->m_pathwayID); 
			if(type=='W')
			{
			  	tmp.push_back(id);
				tmp.push_back(-1); 
			    tmp.push_back(-1);
				tmp.push_back(type); 			  
			}
			else if(type=='M')
			{
				tmp.push_back(-1);
				tmp.push_back(id); 
			    tmp.push_back(-1);
				tmp.push_back(type); 
				if(id>=_scene->CompartmentContain[pbubble->m_pathwayID].size()) //do not know about reason
					continue;
			}
			else
			{
				tmp.push_back(0);			  			      
				vector<int> oitem;
				oitem.push_back(type); oitem.push_back(id);
				bool flag=false;
		        for(int m=0; m< _scene->CompartmentContain[pbubble->m_pathwayID].size()-1;m++)
				{
				    if(_scene->CompartmentContain[pbubble->m_pathwayID][m].find(oitem) != _scene->CompartmentContain[pbubble->m_pathwayID][m].end())
					{
					    tmp.push_back(m);
						flag=true;
						break;					  
					}
				}				  
				if(!flag)
                    tmp.push_back(-1);
				tmp.push_back(id);
				tmp.push_back(type); 
			}
			sID.insert(tmp);			
		}
	}	
	for(int i=0; i<mlist.size(); i++) 
	{
		PathBubble1* pbubble = mlist[i];
		cItem.push_back(pbubble->searchSharedNode( sID, pflag, cflag, wflag, AndFlag));   			
	}			
    return  cItem;    
}

set<vector<int>> PathBubble1::TwoTwoProteinSetComparison( vector<vector<set<vector<int>>>> cItem)
{	
	set< vector <int> >  nodeSet;
	vector<set<vector<int>>> CItem; 
	for(int i=0; i<cItem.size(); i++)
	for(int j=0; j<cItem[i].size(); j++)
	{
		if(!cItem[i][j].empty())
		   CItem.push_back(cItem[i][j]);	
	}	

	for(int i=0; i<CItem.size(); i++)
	for(int j=0; j<CItem.size(); j++)
	{
	    if(i<j)	
		{
			vector<set<vector<int>>> cItemij;
			//vector<PathBubble1*> mlistij;
			cItemij.push_back(CItem[i]);  cItemij.push_back(CItem[j]);
			//mlistij.push_back(mlist[i]);  mlistij.push_back(mlist[j]);
		    set< vector <int> >  nodeSetij = TwoProteinSetComparison(cItemij);
			for(set< vector <int> >::iterator it = nodeSetij.begin(); it != nodeSetij.end(); it++)
			{
			    nodeSet.insert(*it);  			
			}
		}
	}
	return nodeSet;
}


set< vector <int> >  PathBubble1::TwoProteinSetComparison( vector<set<vector<int>>> cItem)
{// two items in cItem and mlist;

	//need every thing the same including ID and location
	set< vector <int> > nodeSet;

	if(cItem.size()!=2)
		return nodeSet;

    int nid=-1; //pathwayid of nodeset
	//for(int i=0; i<cItem.size(); i++) 
	{
		//PathBubble1* pbubble = mlist[i];
		//sharedNode
		for(int j=0; j<cItem.size(); j++)
		{   
			if(nid==-1 && !cItem[j].empty())
			{
				nodeSet = cItem[j];	nid = 1;	 		    
			}
			else  if(!cItem[j].empty())
			{
				set<vector<int>> deSet;					
				for(set<vector<int>>::iterator it=nodeSet.begin(); it!=nodeSet.end(); it++)
				{
					bool flag=false;
					vector<int> itemp=*it;
					for(set<vector<int>>::iterator jt=cItem[j].begin(); jt!=cItem[j].end(); jt++)
					{
						vector<int> jtemp=*jt;
						if( itemp[0]==jtemp[0] ) //same type
						{						
							int Didi= _scene->_pathBubbles[itemp[2]]->dataID;
							int Didj= _scene->_pathBubbles[jtemp[2]]->dataID;	
							int Pidi= _scene->_pathBubbles[itemp[2]]->m_pathwayID;
							int Pidj= _scene->_pathBubbles[jtemp[2]]->m_pathwayID;	
							if(completeSame)
							{
								switch(itemp[0]) 
								{
									case 'P': if(_scene->_proteinNameD[Didi][itemp[1]]==_scene->_proteinNameD[Didj][jtemp[1]])
													flag=true; 
												break;
									case 'S': if(_scene->_smallMoleculeNameD[Didi][itemp[1]]==_scene->_smallMoleculeNameD[Didj][jtemp[1]])
													flag=true; 
												break;
									case 'C': if(_scene->_complexNameD[Didi][itemp[1]]==_scene->_complexNameD[Didj][jtemp[1]])
													flag=true; 
												break;										 
									case 'D': if(_scene->_DnaNameD[Didi][itemp[1]]==_scene->_DnaNameD[Didj][jtemp[1]])
													flag=true; 
												break;
									case 'R': if(_scene->_reactionNameD[Didi][itemp[1]]==_scene->_reactionNameD[Didj][jtemp[1]])
													flag=true; 
												break;
									case 'E': if(_scene->_physicalEntityNameD[Didi][itemp[1]]==_scene->_physicalEntityNameD[Didj][jtemp[1]])
													flag=true; 
										       break;
									case 'L': if(_scene->_ANodeName[Pidi][itemp[1]]==_scene->_ANodeName[Pidj][jtemp[1]])
													flag=true; 
												break;			
									
									
								}	
							}
							else
							{
								switch(itemp[0]) 
								{
									case 'P': if(_scene->_proteinNameD[Didi][itemp[1]][0]!="" && _scene->_proteinNameD[Didi][itemp[1]][0]==_scene->_proteinNameD[Didj][jtemp[1]][0])
													flag=true; 
												break;
									case 'S': if(_scene->_smallMoleculeNameD[Didi][itemp[1]][0]!="" && _scene->_smallMoleculeNameD[Didi][itemp[1]][0] ==_scene->_smallMoleculeNameD[Didj][jtemp[1]][0])
													flag=true; 
												break;
									case 'C': if(_scene->_complexNameD[Didi][itemp[1]][0]!="" && _scene->_complexNameD[Didi][itemp[1]][0] ==_scene->_complexNameD[Didj][jtemp[1]][0])
													flag=true; 
												break;										 
									case 'D': if(_scene->_DnaNameD[Didi][itemp[1]][0]!="" && _scene->_DnaNameD[Didi][itemp[1]][0] ==_scene->_DnaNameD[Didj][jtemp[1]][0])
													flag=true; 
												break;
									case 'R': if(_scene->_reactionNameD[Didi][itemp[1]][1]!="" && _scene->_reactionNameD[Didi][itemp[1]][1] ==_scene->_reactionNameD[Didj][jtemp[1]][1])
													flag=true; 
												break;
									case 'E': if(_scene->_physicalEntityNameD[Didi][itemp[1]][0]!="" && _scene->_physicalEntityNameD[Didi][itemp[1]][0] ==_scene->_physicalEntityNameD[Didj][jtemp[1]][0])
													flag=true; 
												break;					
									case 'L': if(_scene->_ANodeName[Pidi][itemp[1]][1]!="" && _scene->_ANodeName[Pidi][itemp[1]][1] ==_scene->_ANodeName[Pidj][jtemp[1]][1])
													flag=true; 
												break;
								}	
							}

						}
					}
					if(!flag)
						 deSet.insert(*it);
				}
				for(set<vector<int>>::iterator it=deSet.begin(); it!=deSet.end(); it++)
				{
					nodeSet.erase(*it);   
				}
			}
		}		
	}
	//add nid into nodeSet
	set<vector<int>> newNodeSet;
	for( set<vector<int>>::iterator it=nodeSet.begin(); it!=nodeSet.end(); it++)
	{
		vector<int> temp=*it, temp1=*it;
	    temp1.push_back(nid);
		newNodeSet.insert(temp1);		
	}
	return newNodeSet;
}



void PathBubble1::searchsharedPforLink(vector<PathBubble1 *> mlist, vector< vector < set< vector <int> > > > cItem, vector<set<int>> &sharedNode)
{
    //bool complete=false; //search for completely same item including its id and location 

	//for the selected empty pathway or compartment, find the shared or not shared item			
	set< vector <int> > nodeSet = TwoTwoProteinSetComparison(cItem);			
	//write shared node stored in nodeset					
	for(int i=0; i<cItem.size(); i++) 
	{
		PathBubble1* pbubble = mlist[i];
		for(int j=0; j<cItem[i].size(); j++)
		{   					
			for(set<vector<int>>::iterator jt=cItem[i][j].begin(); jt!=cItem[i][j].end(); jt++)
			{
				bool flag=false;
				vector<int> jtemp=*jt;
				for(set<vector<int>>::iterator it=nodeSet.begin(); it!=nodeSet.end(); it++)		
				{
					vector<int> itemp=*it;
					if(itemp[0]==jtemp[0]) //same type
					{							   
						int Didi = _scene->_pathBubbles[itemp[2]]->dataID;
						if(completeSame)
						{
							switch(itemp[0])
							{
								case 'P': 
								if(_scene->_proteinNameD[Didi][itemp[1]]==_scene->_proteinNameD[pbubble->dataID][jtemp[1]])
								{
									sharedNode[pbubble->m_pathwayID].insert(jtemp[1]);
									flag=true; 
								}
								break;																		
							}	
						}
						else
						{
							switch(itemp[0])
							{
								case 'P': 
								if(_scene->_proteinNameD[Didi][itemp[1]][0]==_scene->_proteinNameD[pbubble->dataID][jtemp[1]][0])
								{
									sharedNode[pbubble->m_pathwayID].insert(jtemp[1]);
									flag=true; 
								}
								break;																		
							}	
						}
						if(flag) 							  
							break;								  
					}						   						
				}						
			}
		}		
	}
	/*if( _scene->_pathBubbles.size()>=2 )
	{
	     set<vector<int> > h1 = _scene->_pathBubbles[0]->highlighted;
		 set<vector<int> > h2 = _scene->_pathBubbles[1]->highlighted;	
		 h1 = h1;
	}*/
}

void PathBubble1::searchsharedP(vector<PathBubble1 *> mlist, vector< vector < set< vector <int> > > > cItem)
{
    //bool complete=false; //search for completely same item including its id and location 

	//for the selected empty pathway or compartment, find the shared or not shared item			
	set< vector <int> > nodeSet = TwoTwoProteinSetComparison(cItem);			
	//write shared node stored in nodeset					
	for(int i=0; i<cItem.size(); i++) 
	{
		PathBubble1* pbubble = mlist[i];
		for(int j=0; j<cItem[i].size(); j++)
		{   					
			for(set<vector<int>>::iterator jt=cItem[i][j].begin(); jt!=cItem[i][j].end(); jt++)
			{
				bool flag=false;
				vector<int> jtemp=*jt;
				for(set<vector<int>>::iterator it=nodeSet.begin(); it!=nodeSet.end(); it++)		
				{
					vector<int> itemp=*it;
					if(itemp[0]==jtemp[0]) //same type
					{							   
						int Didi = _scene->_pathBubbles[itemp[2]]->dataID;
						if(completeSame)
						{
							switch(itemp[0])
							{
								case 'P': 
								if(_scene->_proteinNameD[Didi][itemp[1]]==_scene->_proteinNameD[pbubble->dataID][jtemp[1]])
								{
									pbubble->sharedProtein[pbubble->m_pathwayID].insert(jtemp[1]);
									flag=true; 
								}
								break;																		
							}	
						}
						else
						{
							switch(itemp[0])
							{
								case 'P': 
								if(_scene->_proteinNameD[Didi][itemp[1]][0]==_scene->_proteinNameD[pbubble->dataID][jtemp[1]][0])
								{
									pbubble->sharedProtein[pbubble->m_pathwayID].insert(jtemp[1]);
									flag=true; 
								}
								break;																		
							}	
						}
						if(flag) 							  
							break;								  
					}						   						
				}						
			}
		}		
	}
	
}

vector< vector < set< vector <int> > > > PathBubble1::searchSharedItems(bool AndFlag)
{
	//search for the same items with items
	//if the oringin item have hierarhical relationship, find the item set with the same relathion ship
	//search the whole 
    
	vector< vector < set< vector <int> > > > cItem;
    set<vector<int>> sID;
	vector<set<vector<int>>> hl; //for test

	vector<PathBubble1 *> mlist=_scene->extractPathBubblesinGroup(this);
    for(int i=0; i<mlist.size(); i++) 
	{		
		PathBubble1* pbubble = mlist[i];	
		hl.push_back(pbubble->highlighted);
		for(set<vector<int>>::iterator it = pbubble->highlighted.begin(); it != pbubble->highlighted.end(); it++)
		{
			vector<int> tmp,item=*it;
			int type=item[0],id=item[1];		
			tmp.push_back(pbubble->m_pathwayID); 
			if(type=='W')
			{
			  	tmp.push_back(id);
				tmp.push_back(-1); 
			    tmp.push_back(-1);
				tmp.push_back(type); 			  
			}
			else if(type=='M')
			{
				tmp.push_back(-1);
				tmp.push_back(id); 
			    tmp.push_back(-1);
				tmp.push_back(type); 

				if(id>=pbubble->_scene->CompartmentContain[pbubble->m_pathwayID].size())
					continue;
			}
			else
			{
				tmp.push_back(0);			  			      
				vector<int> oitem;
				oitem.push_back(type); oitem.push_back(id);
				bool flag=false;
		        for(int m=0; m< _scene->CompartmentContain[pbubble->m_pathwayID].size()-1;m++)
				{
				    if(_scene->CompartmentContain[pbubble->m_pathwayID][m].find(oitem) != _scene->CompartmentContain[pbubble->m_pathwayID][m].end())
					{
					    tmp.push_back(m);
						flag=true;
						break;					  
					}
				}				  
				if(!flag)
                    tmp.push_back(-1);
				tmp.push_back(id);
				tmp.push_back(type); 
			}
			sID.insert(tmp);			
		}
	}	

	bool pflag=true;
	for(set<vector<int>>::iterator it=sID.begin(); it!=sID.end(); it++)
	{
	    vector<int> item = *it;
		if(item[4]!='W')
		{
		    pflag=false;
			break;
		}
	}

	if(pflag)
	{
		vector< vector < set< vector <int> > > > cItem=getContainedSets(1,0,0, false);	
		vector<set<int>> sharedNode;
		sharedNode.resize(_scene->_pathBubbles.size());
		searchsharedPforLink(mlist, cItem, sharedNode);
		sharedProtein=sharedNode; 
		
	}
	else
	{
		for(int i=0; i<mlist.size(); i++) 
		{
			PathBubble1* pbubble = mlist[i];
			cItem.push_back(pbubble->searchSharedNodeForSearchLinked( sID, AndFlag));   			
		}	
	}
    return  cItem;    
}


void PathBubble1::PU(vector<PathBubble1 *> mlist, vector< vector < set< vector <int> > > > cItem)
{//shared protein
	searchsharedP(mlist, cItem);
}

void PathBubble1::PN(vector<PathBubble1 *> mlist, vector< vector < set< vector <int> > > > cItem, vector< vector < set< vector <int> > > > cItem1)
{//differe protein
		
		        searchsharedP(mlist, cItem);
		
				bool pflag,sflag,cflag,dflag,rflag,eflag;
				pflag=sflag=cflag=dflag=rflag=eflag=false;
				 
				for(int i=0; i<mlist.size(); i++) 
				{					
					PathBubble1* pbubble = mlist[i];
					//if(pflag)
					if(!cItem[i].empty())
					{
						for(int j=0; j<cItem[i].size(); j++)
						{
							//if(i!=j)
							for(set<vector<int>>::iterator it=cItem[i][j].begin(); it!=cItem[i][j].end(); it++)  //for(int l=0; l<_scene->_proteinName[pbubble->m_pathwayID].size(); l++)  								
							{
								//if( pbubble->sharedProtein[pbubble->m_pathwayID].find(it)==pbubble->sharedProtein[pbubble->m_pathwayID].end())
								vector<int> temp=*it;
								if( pbubble->sharedProtein[pbubble->m_pathwayID].find(temp[1])==pbubble->sharedProtein[pbubble->m_pathwayID].end())
								{
									 pbubble->differedProtein[pbubble->m_pathwayID].insert(temp[1]);
								}
							}
						}
					}
					if(!cItem1[i].empty())
					{
						for(int j=0; j<cItem1[i].size(); j++)
						{
							for(set<vector<int>>::iterator itj=cItem1[i][j].begin(); itj!=cItem1[i][j].end(); itj++)  //for(int l=0; l<_scene->_proteinName[pbubble->m_pathwayID].size(); l++)  								
							{
								vector<int> tempj=*itj;
								bool isDiffered=true;
								for(int k=0; k<cItem1[i].size(); k++)													
								{	
									//if( pbubble->sharedProtein[pbubble->m_pathwayID].find(it)==pbubble->sharedProtein[pbubble->m_pathwayID].end())								
									if(cItem1[i][k].find(tempj)!=cItem1[i][k].end())
									{
										isDiffered=false;
										break;
									}								
								}
								//if(isDiffered)
								switch(tempj[0])
								{
									 case 'E': pbubble->differedPhysicalEntity[pbubble->m_pathwayID].insert(tempj[1]); break;		
									 case 'C': pbubble->differedComplex[pbubble->m_pathwayID].insert(tempj[1]); break;		
									 case 'S': pbubble->differedSmallMolecule[pbubble->m_pathwayID].insert(tempj[1]); break;		
									 case 'P': pbubble->differedProtein[pbubble->m_pathwayID].insert(tempj[1]); break;		
									 case 'D': pbubble->differedDna[pbubble->m_pathwayID].insert(tempj[1]); break;		
									 case 'R': pbubble->differedReaction[pbubble->m_pathwayID].insert(tempj[1]); break;			
									 case 'L': pbubble->differedANode[pbubble->m_pathwayID].insert(tempj[1]); break;			
								}
							}
						}
					}
					for(int j=0; j<pbubble->sharedProtein.size(); j++)
					{
						pbubble->sharedProtein[j].clear();
						pbubble->sharedSmallMolecule[j].clear();
						pbubble->sharedComplex[j].clear();
						pbubble->sharedDna[j].clear();
						pbubble->sharedReaction[j].clear();
						pbubble->sharedPhysicalEntity[j].clear();		
						pbubble->sharedANode[j].clear();
						pbubble->sharedCompartment[j].clear();	
						pbubble->sharedPathway[j].clear();
					}
			  }
}




void PathBubble1::PL(vector<PathBubble1 *> mlist)
{
	//if(_scene->pathVectors.size() < mlist.size())
	{
		//_scene->pathVectors.resize(mlist.size());
		for(int i=0; i<mlist.size(); i++) 
		{
			int pid=mlist[i]->m_pathwayID;
			if(pid>=_scene->pathVectors.size())
				_scene->pathVectors.resize(pid+1);
			
			if(pid>=0)
		        _scene->pathVectors[pid].clear();
		}
	}
	
    sharedNodetoHighlighted();
    for(int i=0; i<mlist.size(); i++) 
	{
		PathBubble1* pbubble = mlist[i];	
	
		if(i==0)
			pbubble->searchLinkedNode(mlist, 1, 0, 0);   	

		for(int j=0; j<pbubble->sharedProtein.size(); j++)
		{
			//pbubble->highlighted.clear();
			pbubble->sharedProtein[j].clear();
			pbubble->sharedSmallMolecule[j].clear();
			pbubble->sharedComplex[j].clear();
			pbubble->sharedDna[j].clear();
			pbubble->sharedReaction[j].clear();
			pbubble->sharedPhysicalEntity[j].clear();		
			pbubble->sharedANode[j].clear();	
			pbubble->sharedCompartment[j].clear();	
			pbubble->sharedPathway[j].clear();
		}
		//only run it once		
	}		
}


void PathBubble1::CU(vector<PathBubble1 *> mlist, vector< vector < set< vector <int> > > > cItem)
{
	//for the selected empty pathway or compartment, find the shared or not shared item	
	bool isTwoShared=true;
	    int i=0;
		set< vector <int> > nodeSet;
		set< vector <int> > sharedSet;
		int nid=-1; //pathwayid of nodeset
		for(int i=0; i<mlist.size(); i++) 
		{
			 PathBubble1* pbubble = mlist[i];
			 //sharedNode
		     for(int j=0; j<cItem[i].size(); j++)
			 {   
				if(nid==-1 && !cItem[i][j].empty())
				{
					nodeSet=cItem[i][j];	nid= pbubble->m_pathwayID;	 		    
				}
				else  if(!cItem[i][j].empty())
				{
					set<vector<int>> deSet;
					
					for(set<vector<int>>::iterator it=nodeSet.begin(); it!=nodeSet.end(); it++)
					{
						bool flag=false;
						vector<int> itemp=*it;
						for(set<vector<int>>::iterator jt=cItem[i][j].begin(); jt!=cItem[i][j].end(); jt++)
						{
						   vector<int> jtemp=*jt;
					       //if(itemp[0]==jtemp[0]) //same type
						   if(_scene->_compartmentName[nid][itemp[1]]==_scene->_compartmentName[pbubble->m_pathwayID][jtemp[1]])
						  	   flag=true; 	
						}
						if(!flag)
							   deSet.insert(*it);
						else sharedSet.insert(*it);
					}
					if(!isTwoShared)
					{
						for(set<vector<int>>::iterator it=deSet.begin(); it!=deSet.end(); it++)
						{
							nodeSet.erase(*it);
						}
					}
					
				}
			 }	
		}
		if(!isTwoShared)
			sharedSet=nodeSet;

        //write shared node stored in nodeset	
		for(int i=0; i<mlist.size(); i++) 
		{
			 PathBubble1* pbubble = mlist[i];
			 for(int j=0; j<cItem[i].size(); j++)
			 {   					
					for(set<vector<int>>::iterator jt=cItem[i][j].begin(); jt!=cItem[i][j].end(); jt++)
					{                        
						vector<int> jtemp=*jt;
						for(set<vector<int>>::iterator it=sharedSet.begin(); it!=sharedSet.end(); it++)		
						{
						     vector<int> itemp=*it;
							 if(_scene->_compartmentName[nid][itemp[1]]==_scene->_compartmentName[pbubble->m_pathwayID][jtemp[1]])
							 if(_scene->_compartmentPos[nid][itemp[1]].x()>-990 && _scene->_compartmentPos[pbubble->m_pathwayID][jtemp[1]].x()>-990)
							 {	
									pbubble->sharedCompartment[pbubble->m_pathwayID].insert(jtemp[1]);
									break;					  
						     }						   						
						}						
					}
			 }			 
		}
}

void PathBubble1::CN(vector<PathBubble1 *> mlist, vector< vector < set< vector <int> > > > cItem)
{
	CU(mlist, cItem);
	for(int i=0; i<mlist.size(); i++) 
	{
		PathBubble1* pbubble = mlist[i];	
		for(int l=0; l<_scene->_compartmentName[pbubble->m_pathwayID].size(); l++)  
		{
			if( pbubble->sharedCompartment[pbubble->m_pathwayID].find(l)==pbubble->sharedCompartment[pbubble->m_pathwayID].end())
			{
					pbubble->differedCompartment[pbubble->m_pathwayID].insert(l);
			}
		}
		pbubble->sharedCompartment.clear();				
	}	
}

void PathBubble1::CL(vector<PathBubble1 *> mlist)
{
	for(int i=0; i<mlist.size(); i++) 
		{
			int pid=mlist[i]->m_pathwayID;
			if(pid>=_scene->pathVectors.size())
				_scene->pathVectors.resize(pid+1);
			
			if(pid>=0)
		        _scene->pathVectors[pid].clear();
		}
	
		searchSharedItems(false);
        sharedNodetoHighlighted();
		for(int i=0; i<mlist.size(); i++) 
		{
			PathBubble1* pbubble = mlist[i];
			if(i==0)
			     pbubble->searchLinkedNode(mlist, 0, 1, 0);   		

			for(int j=0; j<pbubble->sharedProtein.size(); j++)
			{
				pbubble->sharedProtein[j].clear();
				pbubble->sharedSmallMolecule[j].clear();
				pbubble->sharedComplex[j].clear();
				pbubble->sharedDna[j].clear();
				pbubble->sharedReaction[j].clear();
				pbubble->sharedPhysicalEntity[j].clear();		
				pbubble->sharedANode[j].clear();
				pbubble->sharedCompartment[j].clear();		
				pbubble->sharedPathway[j].clear();
			}
		}

		
}

void PathBubble1::PAU(vector<PathBubble1 *> mlist)
{
	set<vector<int>> sID;
		 bool flag=false;
        for(int i=0; i<mlist.size(); i++) 
		{
			PathBubble1* pbubble = mlist[i];		 
		   for(set<vector<int>>::iterator it = pbubble->highlighted.begin(); it != pbubble->highlighted.end(); it++)
		   {
			  vector<int> tmp,item=*it;
			  int type=item[0],id=item[1];		
			  tmp.push_back(pbubble->m_pathwayID); 
			  if(type=='W')
			  {
			  	  tmp.push_back(id);
				  tmp.push_back(-1); 
			      tmp.push_back(-1);
				  tmp.push_back(type); 			  
			  }
			  else if(type=='M')
			  {
				  tmp.push_back(-1);
				  tmp.push_back(id); 
			      tmp.push_back(-1);
				  tmp.push_back(type); 
				  if(id>=_scene->CompartmentContain[pbubble->m_pathwayID].size()) //do not know about reason
					continue;
			  }
			  else
			  {
				  tmp.push_back(0);			  			      
				  vector<int> oitem;
				  bool flag=false;
				  oitem.push_back(type); oitem.push_back(id);
		          for(int m=0; m< _scene->CompartmentContain[pbubble->m_pathwayID].size()-1;m++)
				  {
				      if(_scene->CompartmentContain[pbubble->m_pathwayID][m].find(oitem) != _scene->CompartmentContain[pbubble->m_pathwayID][m].end())
					  {
					     tmp.push_back(m);
						 flag=true;
						 break;					  
					  }
				  }		
				    if(!flag)
                     tmp.push_back(-1);
				  tmp.push_back(id);
				  tmp.push_back(type); 
			  }
			  sID.insert(tmp);			
			}
		}
		for(int i=0; i<mlist.size(); i++) 
		{
			PathBubble1* pbubble = mlist[i];
			pbubble->searchSharedNode( sID, 0, 0, 1, false);  
		}	

}

void PathBubble1::PAN(vector<PathBubble1 *> mlist)
{
	PAU(mlist);
		{
		    for(int i=0; i<mlist.size(); i++) 
		    {			    
			    PathBubble1* pbubble = mlist[i];	
				for(int l=0; l<1; l++)  
				{
					if( pbubble->sharedPathway[pbubble->m_pathwayID].find(pbubble->m_pathwayID)==pbubble->sharedPathway[pbubble->m_pathwayID].end())
					{
					     pbubble->differedPathway[pbubble->m_pathwayID].insert(pbubble->m_pathwayID);
					}
				}
				pbubble->sharedPathway.clear();				
			}			
		}

}



void PathBubble1::PAL(vector<PathBubble1 *> mlist)
{
	

}



bool PathBubble1::menuSelection_1(QPointF pos, QPointF scenePos, int index)
{   //return weather graph changed
	_penisOn=false;	
	_queryisOn=false;
	_grabisOn=false;
	_downisOn=_evenisOn=false; 
	_upisOn=false;	
	_lines.clear();
	//_scene->clearSelected(this);
	//_scene->clearExpressed(this);	

	 //get all the member in the same group
	//QList<ItemBase *> mlist=_scene->getGroupMembers(this);
	vector<PathBubble1 *> mlist=_scene->extractPathBubblesinGroup(this);
	if(index==0) 
	{   //P U
		//high
        vector<set<vector<int>>> HBackup=getHighlightedBackuped();
		bool addPathHighlight=sharedDifferedLinkedExpressedtoHighlighted(true, false, false, false, false);

		vector< vector < set< vector <int> > > > cItem=getContainedSets(1,0,0, index==0?true:false);			
		
		for(int i=0; i<mlist.size(); i++) 
	    {
			PathBubble1* pbubble = mlist[i];
			i=i;
		}
		PU(mlist, cItem);	

		for(int i=0; i<mlist.size(); i++) 
	    {
			PathBubble1* pbubble = mlist[i];
			i=i;
		}
		reMoveNotShared(HBackup);

		for(int i=0; i<mlist.size(); i++) 
	    {
			PathBubble1* pbubble = mlist[i];
			i=i;
		}
		
		searchedNodetoStored(SEARCHSHARED);

		for(int i=0; i<mlist.size(); i++) 
	    {
			PathBubble1* pbubble = mlist[i];
			i=i;
		}
		_scene->getGroup(this)->sharedSets=assignSharedSets();

		if(addPathHighlight) cleanPathHighlight();
	}	
	else if(index==10)	
	{
		//P N	
		vector<set<vector<int>>> hBackUp=getHighlightedBackuped();
		bool addPathHighlight=sharedDifferedLinkedExpressedtoHighlighted(true, false, false, false, false);
		vector< vector < set< vector <int> > > > cItem1=getContainedSets(1,0,0, index==0?true:false);	
		vector< vector < set< vector <int> > > > cItem2=getContainedSets_1(1,0,0, true);	
		
		PN(mlist, cItem1, cItem2);							
		searchedNodetoStored(SEARCHDIFFERED);
		_scene->getGroup(this)->sharedSets=assignSharedSets();	
		if(addPathHighlight) cleanPathHighlight();
	}
	else if(index==20)
	{ //P L
		//vector<set<vector<int>>> hBackUp=getHighlightedBackuped();
		int cnum=1;
		if(_scene->linkSearchType == 0)	
			cnum=4;
		for(int i=0; i<cnum; i++)
		{
			bool addPathHighlight;
			
			if(_scene->linkSearchType == 0)	
			   addPathHighlight = sharedDifferedLinkedExpressedtoHighlighted(true, false, false, true, false);
			else if(_scene->linkSearchType == 1)	
				addPathHighlight = sharedDifferedLinkedExpressedtoHighlighted(true, false, false, false, false);
			//vector< vector < set< vector <int> > > > cItem=getContainedProteinSets(index==0?true:false);		
			//searchsharedP(mlist, cItem);		
			//sharedNodetoHighlighted();		
			searchSharedItems(index==0?true:false);
		
			PL(mlist);
			searchedNodetoStored(SEARCHLINKED);

			if(addPathHighlight) 
				cleanPathHighlight();
		}
	}
	else if(index==1)	
	{ //C U
		//high
		bool addPathHighlight=sharedDifferedLinkedExpressedtoHighlighted(true, false, false, false, false);

		vector< vector < set< vector <int> > > > cItem = getContainedSets(0, 1, 0, index==0?true:false);
		CU(mlist, cItem);

		searchedNodetoStored(SEARCHSHARED);
		_scene->getGroup(this)->sharedSets=assignSharedSets();
		if(addPathHighlight) cleanPathHighlight();
	}
	else if( index==11)
	{ //C N
		bool addPathHighlight=sharedDifferedLinkedExpressedtoHighlighted(true, false, false, false, false);
		vector< vector < set< vector <int> > > > cItem = getContainedSets(0, 1, 0, index==0?true:false);
		CN(mlist, cItem);
		searchedNodetoStored(SEARCHDIFFERED);
		_scene->getGroup(this)->sharedSets=assignSharedSets();
		if(addPathHighlight) cleanPathHighlight();
	}
	else if(index==21)
	{   //C L
		bool addPathHighlight=sharedDifferedLinkedExpressedtoHighlighted(true, false, false, true, false);
		CL(mlist);		
		searchedNodetoStored(SEARCHLINKED);
		if(addPathHighlight) cleanPathHighlight();
		
	}
	else if(index==2)	
	{ //PA U
        bool addPathHighlight=sharedDifferedLinkedExpressedtoHighlighted(true, false, false, false, false);
		PAU(mlist);
		
		searchedNodetoStored(SEARCHSHARED);
		_scene->getGroup(this)->sharedSets=assignSharedSets();
		if(addPathHighlight) cleanPathHighlight();
	   
	}	
	else if(index ==12)	
	{  //PA N
        bool addPathHighlight=sharedDifferedLinkedExpressedtoHighlighted(true, false, false, false, false);
		PAN(mlist);
		
		searchedNodetoStored(SEARCHDIFFERED);
		_scene->getGroup(this)->sharedSets=assignSharedSets();	  
		if(addPathHighlight) cleanPathHighlight();
	}	
	else if(index==22)
	{ //PA L
		   
		
	}	
    pieMenuOn=false;		
	return(updateGraphIfHighlightedChanged());
	
}


void PathBubble1::menuSelection(QPointF pos, QPointF scenePos, int index)
{    	
	if(index==49) //query selection
	{
		_penisOn=false;	
		_grabisOn=false;
		_queryisOn=true;
		_upisOn=_downisOn=_evenisOn=false;
		//Qt::CursorShape shape = Qt::WhatsThisCursor;
		setCursor(Qt::WhatsThisCursor);	    
	    _lines.clear(); 
	}
	else if(index==0) //load expression	
	{
	   //load
		QString name = "data/pathways/TGF";
		if( name.size() > 0 )
		{
			ItemBase* item;
			item = NULL;				
			_scene->getOpenManager()->openExpressionByPath(item, 250, scenePos.x(), scenePos.y(), name);
			expressionBubble* EBubble = dynamic_cast<expressionBubble*>(item);			
			//EBubble->m_bindPathwayID = -1;	
			QString tname = name + "0expression.txt";  
	        readexpression(tname.toUtf8().data(), EBubble->quantityName, EBubble->geneInfo, EBubble->quantity, EBubble->minmax);
			EBubble->clearExpressed();
			EBubble->searchExpressedProtein();
			//EBubble->setExpressedGenePos(2);
			EBubble->getExpressionToBePaint();
		}
	}
	else if(index==1) //show expressed gene
	{
		QList<ItemBase *> mList=_scene->getGroupMembers(this);	 
		ItemBase * item=_scene->findAPathBubbleinGroup(this);
		PathBubble1 *pbubble = dynamic_cast<PathBubble1 *>(item);	

		bool addPathHighlight=sharedDifferedLinkedExpressedtoHighlighted(false);
		pbubble->highlightedNodetoStored();
		for(int i=0; i< mList.size(); i++) 
		{
	 		 if ( mList[i]->getType() == EXPRESSIONBUBBLE)
			 {
			    expressionBubble* EBubble = dynamic_cast<expressionBubble*>(mList[i]);	
			    EBubble->clearExpressed();
				EBubble->searchExpressedProtein(pbubble->searchType);
			    //EBubble->setExpressedGenePos(2);
				EBubble->getExpressionToBePaint();
			 }
		}
		if(addPathHighlight) cleanPathHighlight();
		pbubble->searchedNodetoStored(SEARCHEXPRESSED);		
	}
	else if(index==10)
	//else if (selectedAction == UpAction)
	{
		QBitmap bitmap("icon/GoUp.png");
		QCursor cursor (bitmap,bitmap);
		//Qt::CursorShape shape = Qt::WhatsThisCursor;
	    //setCursor(shape);    		
		setCursor(cursor);   

		_penisOn=false;	
		_queryisOn=false;
		_grabisOn=false;
		_downisOn=_evenisOn=false; 
		_upisOn=true;		
	    _lines.clear();
	}	
	else if(index==11)
	//else if (selectedAction == EvenAction)
	{
		QBitmap bitmap("icon/GoEven.png");
		QCursor cursor (bitmap,bitmap);
		Qt::CursorShape shape = Qt::WhatsThisCursor;

		_penisOn=false;	
		_queryisOn=false;
		_grabisOn=false;
		_upisOn=_downisOn=false; 
		_evenisOn=true;
	    _lines.clear();
	}
	else if(index==12)
	//else if (selectedAction == DownAction)
	{
		QBitmap bitmap("icon/GoDown.png");
		QCursor cursor (bitmap,bitmap);
		setCursor(cursor);   
		_penisOn=false;	
		_queryisOn=false;
		_grabisOn=false;
		_upisOn=_evenisOn=false; 
		_downisOn=true;		
	    _lines.clear();
	}
	else if(index==20) //reset graph
	//else if (selectedAction == resetAction)
	{
		dCenter.setX(0);
		dCenter.setY(0);

		//if there are aggregated node, extend them all		

		collapseALL();

       _scene->resetItemPos(m_pathwayID, complexPos, proteinPos, smallMoleculePos, DnaPos, EmptyPos, reactionPos, physicalEntityPos, compartmentPos, ANodePos, edge, _scale, dCenter, drawSmallmolecule);
	   if(drawSmallmolecule)
	      _scale=_ScaleBackup_S;
	   else 
		  _scale=_ScaleBackup;
	   x0y0=QPoint(-this->Width()/2.0,-this->Height()/2.0);	
	   x1y1=QPoint(this->Width()/2.0,this->Height()/2.0);
	   _penisOn=false;	
	   _grabisOn=false;	
	   _queryisOn=false;
	   _upisOn=_downisOn=_evenisOn=false; 
	   _lines.clear();
	   _itemColored.clear();
	   getGraphToBePaint();
	   itemGrid=recordItemGrid(visibleItemNum, visibleCompartmentNum);
	   QString fileName = getPathwayGraphName(pathwayGraphStored);
	   if(fileName.size()>0)
	   {
		   savePathWay(fileName, complexPos, proteinPos, smallMoleculePos, DnaPos, reactionPos, physicalEntityPos, ANodePos);	//edge
	   }
	}
	else if(index==23) //save pathway ////else if (selectedAction == savAction)	
	{   
        //QFileDialog dlg;
		QString pathName = _scene->pwdParser->savePathGraphPreparation(_pathName, drawSmallmolecule);
		//int lid = pathName.lastIndexOf('/');
		//QString pathDir = pathName.mid(0, lid);
		//QString pureFileName = pathName.mid(lid, pathName.size()-lid);

		//dlg.setDirectory(pathDir);
		//dlg.setDefaultSuffix("xml");
		//QString fileName = dlg.getSaveFileName(0, tr("Save Pathway Graph in xml file"), pathName,".xml");//"c:\\file.x", "X files (*.x)"
		QString fileName = QFileDialog::getSaveFileName(0, tr("Save Pathway Graph as xml file"), pathName,".xml (*.xml)");//"c:\\file.x", "X files (*.x)"
       
		savePathWay(fileName, complexPos, proteinPos, smallMoleculePos, DnaPos, reactionPos, physicalEntityPos, ANodePos, true);
		//savePathWayinJSFile(fileName, complexPos, proteinPos, smallMoleculePos, DnaPos, reactionPos, physicalEntityPos, edge);
		_penisOn=false;	
	   _grabisOn=false;	
	   _queryisOn=false;
	   _upisOn=_downisOn=_evenisOn=false;
	   _lines.clear();	   
	}
	else if(index==24) //recover pathway
	//else if (selectedAction == recAction)
	{   
		_penisOn=false;	
	    _grabisOn=false;	
	    _queryisOn=false;
		_upisOn=_downisOn=_evenisOn=false;
		_lines.clear();
		
		//for recover		
		QString pathName = _scene->pwdParser->loadPathGraphPreparation(_pathName, drawSmallmolecule);
		QString fileName = QFileDialog::getOpenFileName(0, tr("Open PathWay Stored as xml file"), pathName,".xml (*.xml)");//"c:\\file.x", "X files (*.x)"
		if(fileName.size()!=0)
		{
			//fileName = QFileDialog::getOpenFileName(NULL , tr("Open PathWay"), "", tr("(*.xml)"));
			if(!fileName.contains(".xml"))
				fileName=fileName+".xml";
			readPathWay(fileName, complexPos, proteinPos, smallMoleculePos, DnaPos, reactionPos, physicalEntityPos, ANodePos, edge);	
			getGraphToBePaint();		
		}
		//for edit
		//newbubble
	}
	else if(index==25) 
	//else if (selectedAction == savAction)
	{   
		//QString fileName = QFileDialog::getSaveFileName();
		//savePathWay(fileName, complexPos, proteinPos, smallMoleculePos, DnaPos, reactionPos, physicalEntityPos, edge);
		//savePathWayinJSFile(fileName, complexPos, proteinPos, smallMoleculePos, DnaPos, reactionPos, physicalEntityPos, edge);
	   _penisOn=false;	
	   _grabisOn=false;	
	   _queryisOn=false;
	   _upisOn=_downisOn=_evenisOn=false;
	   _lines.clear();

	   //for undo	   
		QString fileName = getFirstPathWayGraphName();		
		if(fileName.size()>0)
		{
			fileName=fileName+".xml";
			//HierarchicalLayout(_whole);			
		    //RemoveDisabledNodes(_whole, _VertexInfo, _Vertex, Rank0, Ranks);	
		    //HierarchicalTreeNodeLocation(_whole, _VertexInfo, Rank0, Ranks);

			readPathWay(fileName, complexPos, proteinPos, smallMoleculePos, DnaPos, reactionPos, physicalEntityPos, ANodePos, edge);	

			//HierarchicalTreeNodeLocation(_whole, _VertexInfo, Rank0, Ranks);
			_scene->backupItemPos(m_pathwayID, complexPos, proteinPos, smallMoleculePos, DnaPos, EmptyPos, reactionPos, physicalEntityPos, compartmentPos, ANodePos, _scale, dCenter, drawSmallmolecule);	
		    
			//_scene->backupRanks(m_pathwayID, Rank0, Ranks, _newPos, _newCompartmentPos);	
		    //_scene->backupEdge(m_pathwayID, edge);
			getGraphToBePaint();
			//itemGrid=recordItemGrid(visibleItemNum, visibleCompartmentNum);
		}
		
	}
	else if(index==21) //undo pathway// maximum 10 step 
	//else if (selectedAction == savAction)
	{   
		//QString fileName = QFileDialog::getSaveFileName();
		//savePathWay(fileName, complexPos, proteinPos, smallMoleculePos, DnaPos, reactionPos, physicalEntityPos, edge);
		//savePathWayinJSFile(fileName, complexPos, proteinPos, smallMoleculePos, DnaPos, reactionPos, physicalEntityPos, edge);
	   _penisOn=false;	
	   _grabisOn=false;	
	   _queryisOn=false;
	   _upisOn=_downisOn=_evenisOn=false;
	   _lines.clear();

	   //for undo	   
		QString fileName = pathWayGraphBackward();			
		
		if(fileName.size()>0)
		{
			fileName=fileName+".xml";
			readPathWay(fileName, complexPos, proteinPos, smallMoleculePos, DnaPos, reactionPos, physicalEntityPos, ANodePos, edge);		
			getGraphToBePaint();
			//itemGrid=recordItemGrid(visibleItemNum, visibleCompartmentNum);
		}
		
	}
	else if(index==22) //redo pathway
	//else if (selectedAction == recAction)
	{   
		_penisOn=false;	
	    _grabisOn=false;	
	    _queryisOn=false;
		_upisOn=_downisOn=_evenisOn=false;
		_lines.clear();
		
		//for redo
	    QString fileName = pathWayGraphForward();			
		
		if(fileName.size()>0)
		{
			fileName=fileName+".xml";
			readPathWay(fileName, complexPos, proteinPos, smallMoleculePos, DnaPos, reactionPos, physicalEntityPos, ANodePos, edge);			
			getGraphToBePaint();
			//itemGrid=recordItemGrid(visibleItemNum, visibleCompartmentNum);
		}
		//for edit
		//newbubble
	}
	else if(index==39)
	{
		QBitmap bitmap("icon/pencil.png");
		QCursor cursor (bitmap,bitmap);
		
		setCursor(cursor);   
	    _penisOn=true;	
		_queryisOn=false;
		_grabisOn=false;
		_upisOn=_downisOn=_evenisOn=false;
	    _lines.clear();
		//_open->openSearchView( 300, this->pos().x()+this->Width()/2.0+150, this->pos().y()-this->Height()/2.0, _name); 
	}
	else if(index==50) //delete bubble
	{
	       int size=_pathBubbles.size(),psize=-1;	
			QRectF result(-10000,-10000,-10000,-10000);
			if(_pathBubbleParent!=NULL)
			{
				result=_pathBubbleParent->ResetGraph_1();
			}					
			_scene->bubbleDeleted(this, result);					
			deleteSelectedItems(_scene);															
				
	}			
	else if(index==51) //delete group
	//if (selectedAction == delGroupAction)
	{
		_scene->deleteGroup(this);								
	}
    pieMenuOn=false;	

}




/*void PathBubble1::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
	if (!isSelected() && _scene) 
	{
		_scene->clearSelection();
		setSelected(true);
	}
	QAction* UpAction = new QAction("Mark \"Up\"", this);
	UpAction->setSoftKeyRole(QAction::PositiveSoftKey);
	UpAction->setIcon(QPixmap("icon/UpArrow.png"));  

	QAction* DownAction = new QAction("Mark Down", this);
	DownAction->setSoftKeyRole(QAction::PositiveSoftKey);
	DownAction->setIcon(QPixmap("icon/DownArrow.png"));  

	QAction* EvenAction = new QAction("Mark Even", this);
	EvenAction->setSoftKeyRole(QAction::PositiveSoftKey);
	EvenAction->setIcon(QPixmap("icon/Even.png")); 
	
	QMenu menu;
	QAction *queryAction = menu.addAction("Query");
	QAction *penAction = menu.addAction("Pen");
	QAction *grabAction = menu.addAction("Grap");
	menu.addAction(UpAction);
	menu.addAction(DownAction);
	menu.addAction(EvenAction);
	QAction *resetAction = menu.addAction("Reset");
	QAction *delAction = menu.addAction("Delete");	
	QAction *savAction = menu.addAction("Save pathway");
	QAction *recAction = menu.addAction("Recover pathway");
	QAction *searchAction = menu.addAction("Search");	
	
	//add icon for the menu softkey!myMenuKey->setMenu(menu);
	//menu.addAction(myMenuKey );

	QAction *selectedAction = menu.exec(event->screenPos());
	
	_penisOn=false;	_grabisOn=false; _queryisOn=false; _upisOn=_downisOn=_evenisOn=false;
	if (selectedAction == delAction)
	{
		_penisOn=false;	
	    _grabisOn=false;	
	    _queryisOn=false;
		_upisOn=_downisOn=_evenisOn=false;
		_lines.clear();
		logRemove();
		deleteSelectedItems(_scene);		
	}
	if (selectedAction == queryAction)
	{
	    _penisOn=false;	
		_grabisOn=false;
		_queryisOn=true;
		_upisOn=_downisOn=_evenisOn=false;
	    _lines.clear();
	}
	if (selectedAction == penAction)
	{
		QBitmap bitmap("icon/pencil.png");
		QCursor cursor (bitmap,bitmap);
		//Qt::CursorShape shape = Qt::WhatsThisCursor;
	    //setCursor(shape);    		
		setCursor(cursor);   

	    _penisOn=true;	
		_queryisOn=false;
		_grabisOn=false;
		_upisOn=_downisOn=_evenisOn=false;
	    _lines.clear();
	}
	if (selectedAction == grabAction)
	{
       //Qt::CursorShape shape = Qt::OpenHandCursor;
	   //setCursor(shape); 
	   _penisOn=false;	
	   _queryisOn=false;
	   _upisOn=_downisOn=_evenisOn=false;
	   _grabisOn=true;		   
	   _lines.clear();
	}
	else if (selectedAction == UpAction)
	{
		QBitmap bitmap("icon/GoUp.png");
			QCursor cursor (bitmap,bitmap);
			//Qt::CursorShape shape = Qt::WhatsThisCursor;
	        //setCursor(shape);    		
			setCursor(cursor);   

		_penisOn=false;	
		_queryisOn=false;
		_grabisOn=false;
		_downisOn=_evenisOn=false; 
		_upisOn=true;		
	    _lines.clear();
	}
	else if (selectedAction == DownAction)
	{
		QBitmap bitmap("icon/GoDown.png");
		QCursor cursor (bitmap,bitmap);
		setCursor(cursor);   
		_penisOn=false;	
		_queryisOn=false;
		_grabisOn=false;
		_upisOn=_evenisOn=false; 
		_downisOn=true;		
	    _lines.clear();
	}
	else if (selectedAction == EvenAction)
	{
		QBitmap bitmap("icon/GoEven.png");
		QCursor cursor (bitmap,bitmap);
		Qt::CursorShape shape = Qt::WhatsThisCursor;

		_penisOn=false;	
		_queryisOn=false;
		_grabisOn=false;
		_upisOn=_downisOn=false; 
		_evenisOn=true;
	    _lines.clear();
	}
	else if (selectedAction == resetAction)
	{
		dCenter.setX(0);
		dCenter.setY(0);
		_scale=1;
       _scene->resetItemPos(m_pathwayID, complexPos, proteinPos, smallMoleculePos, DnaPos, EmptyPos, reactionPos, physicalEntityPos, edge);
	   _penisOn=false;	
	   _grabisOn=false;	
	   _queryisOn=false;
	   _upisOn=_downisOn=_evenisOn=false; 
	   _lines.clear();
	   _itemColored.clear();
	   itemGrid=recordItemGrid();
	}
	else if (selectedAction == savAction)
	{   
		QString fileName = QFileDialog::getSaveFileName();
		savePathWay(fileName, complexPos, proteinPos, smallMoleculePos, DnaPos, reactionPos, physicalEntityPos, edge);
		savePathWayinJSFile(fileName, complexPos, proteinPos, smallMoleculePos, DnaPos, reactionPos, physicalEntityPos, edge);
		_penisOn=false;	
	   _grabisOn=false;	
	   _queryisOn=false;
	   _upisOn=_downisOn=_evenisOn=false;
	   _lines.clear();
	}
	else if (selectedAction == recAction)
	{   
		_penisOn=false;	
	    _grabisOn=false;	
	    _queryisOn=false;
		_upisOn=_downisOn=_evenisOn=false;
		_lines.clear();
		
		//logRemove();
		//deleteSelectedItems(_scene);
		QString fileName;// = QFileDialog::getOpenFileName();

		fileName = QFileDialog::getOpenFileName(NULL , tr("Open PathWay"), "", tr("(*.xml)"));
		readPathWay(fileName, complexPos, proteinPos, smallMoleculePos, DnaPos, reactionPos, physicalEntityPos, edge);		
	}	
	else if (selectedAction == searchAction)
	{
		_open->openSearchView( 200, this->pos().x()+this->Height()/2.0, this->pos().y()-this->Height()/2.0-50, _name); 
	}
}*/

void PathBubble1::rewriteTreeRingGraphFile()
{
	
	QString filename1,filename2,filename3;
	//filename1 =filename;	
	
	filename2 ="data/Reactome_Pathway_Data/pathwayTable/Metabolism_tree_b.out"; //lib15-Heat_part_0expression
	QFile file2( filename2 );
	vector<QString> nodeTypeVector;
	if (file2.open(QIODevice::ReadOnly))
	{
		   
		   QString type;
		   QTextStream in(&file2);
		   while ( !in.atEnd() )
		   {			   
				QString line = in.readLine();    
				QStringList sname=line.split('\t');
				type=sname[3];
				nodeTypeVector.push_back(type);
		   }
		  //rateLimitSymbol=rateLimitSymbol1;
	} 
	file2.close();

	filename1 ="data/Reactome_Pathway_Data/pathwayTable/Metabolism_graph_b.out"; //lib15-Heat_part_0expression
	set<vector<int>> edgeSet,edgeSet1;
	vector<vector<int>> edgeVector;
	vector<QString> typeVector;
	vector<int> node(2,0);
	QFile file( filename1 );
	if (file.open(QIODevice::ReadOnly))
	{
		   int e1, e2;
		   QString type;
		   QTextStream in(&file);
		   while ( !in.atEnd() )
		   {			   
				QString line = in.readLine();    
				QStringList sname=line.split('\t');
				e1=sname[0].toInt(), e2=sname[1].toInt(), type=sname[2];
				node[0]= e1, node[1]= e2;
				edgeSet.insert(node);   edgeVector.push_back(node);  typeVector.push_back(type);
		   }
		  //rateLimitSymbol=rateLimitSymbol1;
	} 
	file.close();
	edgeSet1 = edgeSet;
	while(edgeSet.size()<8237)
	{
	     int e1, e2;
		 QString type;
		 e1=rand() % 4932;  e2= (e1 + rand() % 4931)%4932;
	     if(e1!=e2)
		 {
		     node[0]= e1, node[1]= e2; 
			 //if((nodeTypeVector[e1]=="P" && nodeTypeVector[e2]=="R"))
			 //{
			 edgeSet.insert(node);
			 //}
		 }	
	}
	for(set<vector<int>> ::iterator it=edgeSet.begin(); it!=edgeSet.end(); it++)
	{
		 QString type;
		 int e1,e2;
	      node = *it;
		  if(edgeSet1.find(node)==edgeSet1.end())
		  {
		      edgeVector.push_back(node); 
			  type = "R"; e1=node[0], e2=node[1];
			  if(nodeTypeVector[e1]=="P" && nodeTypeVector[e2]=="R")
			  {
			     if(e1%20==0)
					 type = "A";
				 else if(e1%30==0)
					 type = "I";
			  }
		      typeVector.push_back(type);
		  }	
	}

	filename3="data/Reactome_Pathway_Data/pathwayTable/Metabolism_graph_b_1.out";
	QFile file3( filename3 );
	if(!file3.open(QIODevice::WriteOnly | QIODevice::Truncate))
		return;
	
	QDomText text;
	QDomDocument doc;
	QString str2="";		
	for(int i=0; i<edgeVector.size(); i++)		
	{
		str2 +=  QString::number(edgeVector[i][0]);
		str2 += "\t";   str2 +=  QString::number(edgeVector[i][1]);
		str2 += "\t";   str2 +=  typeVector[i];
		str2 += "\n";
        
	}		
    text = doc.createTextNode( str2 );
	QTextStream out3(&file3);
	text.save( out3, 4 );
	file3.close();	
}




void PathBubble1::savePathWayforTreeRing(QString filename,  vector<vector<int>> edge)
{   //save edge and node (not include position)//similar to VTK style
    QString str,tmpStr,N_str,str2;
	int gid;
	QDomText text;
	QDomDocument doc;

	vector<vector<int>> nodeSet;
	
	int count=0,Estart,Cstart,Pstart,Sstart,Dstart,Rstart;
	
	QString filename1,filename2,filename3;
	filename1 =filename;
	if(drawSmallmolecule)
	{
	    filename1 = filename1 + "_S";
	}
	filename1 +="_expression_lib15-Heat_b.out"; //lib15-Heat_part_0expression
	QFile file( filename1 );
	if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
		return;

	vector<vector<QString>> treeVector;
	set<vector<QString>> treeSet;
	vector<QString> nodeInfo(3,"");

	vector<vector<QString>> expressionVector;
	vector<QString> expInfo(2,"0");

	vector<vector<QString>> graphVector;
		

	vector<QString> quantityName;
	vector<vector<QString>> geneInfo;
	vector<vector<float>> quantity;
	vector<vector<float>> minmax;
	readexpression("data/Reactome_Pathway_Data/expressionData/lib15-Heat0expression.txt", quantityName, geneInfo, quantity, minmax);//TGF0expression
	Estart=count;
	for (int j = 0; j < _scene->_physicalEntityNameD[dataID].size(); j++)
	{
        if(physicalEntityRect[j].width()>=0)
		{
			vector<int> node; 	node.push_back('E'); node.push_back(j);							
			for(int i=0; i< _scene->_compartmentName[m_pathwayID].size()-1; i++)
			{		
				if(_scene->_compartmentName[m_pathwayID][i][0]!="")
				if(_scene->CompartmentContain[m_pathwayID][i].find(node)!=_scene->CompartmentContain[m_pathwayID][i].end())
				{  					
					tmpStr.setNum( count );
			        str += tmpStr; str2 += tmpStr; str2 += "\t";			        	
					N_str=_scene->_compartmentName[m_pathwayID][i][0];	
					spaceReplace(N_str);
					str2 +=  N_str;	nodeInfo[0]=N_str;
					str2 +="\t";
					//str += " physicalEntity:";  //str2 += "physicalEntity:";
					N_str = _scene->_physicalEntityNameD[dataID][j][0]; 
					spaceReplace(N_str);
					
					str2 += N_str; nodeInfo[1]=N_str;
					str += "\t0\t0";	
					expInfo[0]=QString::number(0);
					expInfo[1]=QString::number(0);
					switch(node[0])
					{
						case 'P': nodeInfo[2]="P"; break;
						case 'E': nodeInfo[2]="E"; break;
						case 'C': nodeInfo[2]="C"; break;
						case 'D': nodeInfo[2]="D"; break;
						case 'S': nodeInfo[2]="S"; break;
						case 'R': nodeInfo[2]="R"; break;
					}
					//str2 += "physicalEntity:";
					str += "\n"; str2 += "\n";		
					count++; nodeSet.push_back(node);	
					treeVector.push_back(nodeInfo);   treeSet.insert(nodeInfo);  expressionVector.push_back(expInfo);	                
					break;
				}	
			}		
			
		}
	}
	Cstart=count;
	for (int j = 0; j < _scene->_complexNameD[dataID].size(); j ++)
	{
		if(complexRect[j].width()>=0)
		{
			vector<int> node; 
			node.push_back('C'); node.push_back(j);	
			for(int i=0; i< _scene->_compartmentName[m_pathwayID].size()-1; i++)
			{
				if(_scene->_compartmentName[m_pathwayID][i][0]!="")
				if(_scene->CompartmentContain[m_pathwayID][i].find(node)!=_scene->CompartmentContain[m_pathwayID][i].end())
				{   
					tmpStr.setNum( count );
					str += tmpStr; str2 += tmpStr; str2 +="\t";		
					N_str=_scene->_compartmentName[m_pathwayID][i][0];					
					
					spaceReplace(N_str);
					str2 +=  N_str;	nodeInfo[0]=N_str;
					str2 +="\t";
					//str += " Complex:"; str2 += "Complex:";
					N_str = _scene->_complexNameD[dataID][j][0]; 
					gid = findExpressionID(N_str, geneInfo, 'C');
					spaceReplace(N_str);
					str2 +=  N_str;	nodeInfo[1]=N_str;
					
					expInfo[0]=QString::number(0);
					expInfo[1]=QString::number(0);

					if(gid<0)
						str += "\t0\t0";
					else
					{
						expInfo[0]=QString::number(quantity[gid][0], '1', 4);
						expInfo[1]=QString::number(quantity[gid][1], '1', 4);
						str += "\t"; str += expInfo[0];
						str += "\t"; str += expInfo[1];						
					}
					switch(node[0])
					{
						case 'P': nodeInfo[2]="P"; break;
						case 'E': nodeInfo[2]="E"; break;
						case 'C': nodeInfo[2]="C"; break;
						case 'D': nodeInfo[2]="D"; break;
						case 'S': nodeInfo[2]="S"; break;
						case 'R': nodeInfo[2]="R"; break;
					}
					str += "\n"; str2 += "\n";
					count++;  nodeSet.push_back(node);
					treeVector.push_back(nodeInfo);   treeSet.insert(nodeInfo);  expressionVector.push_back(expInfo);	
					break;
				}	
			}	
		}		
	}		
	Pstart=count;
	for (int j = 0; j < _scene->_proteinNameD[dataID].size(); j ++)
	{
	    if(proteinRect[j].width()>=0)
		{
			vector<int> node; node.push_back('P'); node.push_back(j);
			for(int i=0; i< _scene->_compartmentName[m_pathwayID].size()-1; i++)
			{
				if(_scene->_compartmentName[m_pathwayID][i][0]!="" && _scene->_proteinNameD[dataID][j][0]!="")
				if(_scene->CompartmentContain[m_pathwayID][i].find(node)!=_scene->CompartmentContain[m_pathwayID][i].end())
				{   
					tmpStr.setNum( count );
					str += tmpStr; str2 += tmpStr; str2 +="\t";
					N_str=_scene->_compartmentName[m_pathwayID][i][0];	
					
					spaceReplace(N_str);
					str2 +=  N_str;	nodeInfo[0]=N_str;
					str2 +="\t";
					//str += " protein:"; str2 += "protein:";
					N_str = _scene->_proteinNameD[dataID][j][0];
					gid = findExpressionID(N_str, geneInfo, 'P');
					spaceReplace(N_str);
					str2 +=  N_str;	nodeInfo[1]=N_str;

					expInfo[0]=QString::number(0);
					expInfo[1]=QString::number(0);

					if(gid<0)
						str += "\t0\t0";
					else
					{
						expInfo[0]=QString::number(quantity[gid][0], '1', 4);
						expInfo[1]=QString::number(quantity[gid][1], '1', 4);
						str += "\t"; str += expInfo[0];
						str += "\t"; str += expInfo[1];		
					}
					switch(node[0])
					{
						case 'P': nodeInfo[2]="P"; break;
						case 'E': nodeInfo[2]="E"; break;
						case 'C': nodeInfo[2]="C"; break;
						case 'D': nodeInfo[2]="D"; break;
						case 'S': nodeInfo[2]="S"; break;
						case 'R': nodeInfo[2]="R"; break;
					}
					str += "\n"; str2 += "\n";
					count++; nodeSet.push_back(node);
					treeVector.push_back(nodeInfo);   treeSet.insert(nodeInfo);  expressionVector.push_back(expInfo);	  
					break;
				}	
			}
		}		
	}
	Sstart=count;
	for (int j = 0; j < _scene->_smallMoleculeNameD[dataID].size(); j ++)
	{
		if(smallMoleculeRect[j].width()>=0)
		{
			vector<int> node; node.push_back('S'); node.push_back(j);
			for(int i=0; i< _scene->_compartmentName[m_pathwayID].size()-1; i++)
			{	
				if(_scene->_compartmentName[m_pathwayID][i][0]!="")
				if(_scene->CompartmentContain[m_pathwayID][i].find(node)!=_scene->CompartmentContain[m_pathwayID][i].end())
				{   
					tmpStr.setNum( count );
					str += tmpStr; str2 += tmpStr; str2 +="\t";
					N_str=_scene->_compartmentName[m_pathwayID][i][0];	
					spaceReplace(N_str);
					str2 +=  N_str;	nodeInfo[0]=N_str;
					str2 +="\t";
					//str += " smallMolecule:"; str2 += "smallMolecule:";
					N_str = _scene->_smallMoleculeNameD[dataID][j][0];
					spaceReplace(N_str);
					str2 +=  N_str;	nodeInfo[1]=N_str;
					str += "\t0\t0";
					expInfo[0]=QString::number(0);
					expInfo[1]=QString::number(0);
					switch(node[0])
					{
						case 'P': nodeInfo[2]="P"; break;
						case 'E': nodeInfo[2]="E"; break;
						case 'C': nodeInfo[2]="C"; break;
						case 'D': nodeInfo[2]="D"; break;
						case 'S': nodeInfo[2]="S"; break;
						case 'R': nodeInfo[2]="R"; break;
					}
					str += "\n"; str2 += "\n";
					count++; nodeSet.push_back(node);
					treeVector.push_back(nodeInfo);   treeSet.insert(nodeInfo);  expressionVector.push_back(expInfo);	  
					break;
				}	
			}
		}		
	}
	Dstart=count;
	for (int j = 0; j < _scene->_DnaNameD[dataID].size(); j ++)
	{		
		if(DnaRect[j].width()>=0)
		{
			vector<int> node; node.push_back('D'); node.push_back(j);
			for(int i=0; i< _scene->_compartmentName[m_pathwayID].size()-1; i++)
			{
				if(_scene->_compartmentName[m_pathwayID][i][0]!="")
				if(_scene->CompartmentContain[m_pathwayID][i].find(node)!=_scene->CompartmentContain[m_pathwayID][i].end())
				{   
					tmpStr.setNum( count );
					str += tmpStr;  str2 += tmpStr; str2 +="\t";
					N_str=_scene->_compartmentName[m_pathwayID][i][0];	
					spaceReplace(N_str);
					str2 +=  N_str;	nodeInfo[0]=N_str;
					str2 +="\t";
					//str += " Dna:"; str2 += "Dna:";
					N_str = _scene->_DnaNameD[dataID][j][0];
					spaceReplace(N_str);		
					str2 +=  N_str;	nodeInfo[1]=N_str;
					str += "\t0\t0";
					expInfo[0]=QString::number(0);
					expInfo[1]=QString::number(0);
					switch(node[0])
					{
						case 'P': nodeInfo[2]="P"; break;
						case 'E': nodeInfo[2]="E"; break;
						case 'C': nodeInfo[2]="C"; break;
						case 'D': nodeInfo[2]="D"; break;
						case 'S': nodeInfo[2]="S"; break;
						case 'R': nodeInfo[2]="R"; break;
					}
					str += "\n"; str2 += "\n";
					count++; nodeSet.push_back(node);
					treeVector.push_back(nodeInfo);   treeSet.insert(nodeInfo);  expressionVector.push_back(expInfo);	  
					break;
				}	
			}
		}
	}
	Rstart=count;
	for (int j = 0; j < _scene->_reactionNameD[dataID].size(); j ++)
	{
		if(reactionRect[j].width()>=0)
		{
			vector<int> node; node.push_back('R'); node.push_back(j);
			for(int i=0; i< _scene->_compartmentName[m_pathwayID].size()-1; i++)
			{
				if(_scene->_compartmentName[m_pathwayID][i][0]!="")
				if(_scene->CompartmentContain[m_pathwayID][i].find(node)!=_scene->CompartmentContain[m_pathwayID][i].end())
				{   
					tmpStr.setNum( count );
					str += tmpStr;  str2 += tmpStr; str2 +="\t";
					N_str=_scene->_compartmentName[m_pathwayID][i][0];	
					spaceReplace(N_str);
					str2 +=  N_str;	nodeInfo[0]=N_str;
					str2 +="\t";
					//str += " reaction:"; str2 += "reaction:";
					N_str=_scene->_reactionNameD[dataID][j][1];
					spaceReplace(N_str);
					str2 +=  N_str;	nodeInfo[1]=N_str;
					str += "\t0\t0";
					expInfo[0]=QString::number(0);
					expInfo[1]=QString::number(0);
					switch(node[0])
					{
						case 'P': nodeInfo[2]="P"; break;
						case 'E': nodeInfo[2]="E"; break;
						case 'C': nodeInfo[2]="C"; break;
						case 'D': nodeInfo[2]="D"; break;
						case 'S': nodeInfo[2]="S"; break;
						case 'R': nodeInfo[2]="R"; break;
					}
					str += "\n"; str2 += "\n";
					count++; nodeSet.push_back(node);
					treeVector.push_back(nodeInfo);   treeSet.insert(nodeInfo);  expressionVector.push_back(expInfo);	  
					break;
				}	
			}
		}
	}

	//for the rest not in the graph
	//count=0;
	////for big bubble
	
	Cstart=count;
	for (int j = 0; j < _scene->_complexNameD[dataID].size(); j ++)
	{
		if(complexRect[j].width()<0)//for big bubble
		{
			vector<int> node; 
			node.push_back('C'); node.push_back(j);	
			//for(int i=0; i< _scene->_compartmentName[m_pathwayID].size()-1; i++)
			{
				if(_scene->_complexNameD[dataID][j][1]!="")
				//if(_scene->CompartmentContain[m_pathwayID][i].find(node)!=_scene->CompartmentContain[m_pathwayID][i].end())
				{   
					tmpStr.setNum( count );
					str += tmpStr; str2 += tmpStr; str2 +="\t";		
					N_str=_scene->_complexNameD[dataID][j][1];					
					

					spaceReplace(N_str);
					str2 +=  N_str;	nodeInfo[0]=N_str;			
					str2 +="\t";
					//str += " Complex:"; str2 += "Complex:";
					N_str = _scene->_complexNameD[dataID][j][0]; 
					gid = findExpressionID(N_str, geneInfo, 'C');
					spaceReplace(N_str);
					str2 +=  N_str;	nodeInfo[1]=N_str;
					

					expInfo[0]=QString::number(0);
					expInfo[1]=QString::number(0);
					if(gid<0)
						str += "\t0\t0";
					else
					{
						expInfo[0]=QString::number(quantity[gid][0], '1', 4);
						expInfo[1]=QString::number(quantity[gid][1], '1', 4);
						str += "\t"; str += expInfo[0];
						str += "\t"; str += expInfo[1];
					}
					switch(node[0])
					{
						case 'P': nodeInfo[2]="P"; break;
						case 'E': nodeInfo[2]="E"; break;
						case 'C': nodeInfo[2]="C"; break;
						case 'D': nodeInfo[2]="D"; break;
						case 'S': nodeInfo[2]="S"; break;
						case 'R': nodeInfo[2]="R"; break;
					}
					str += "\n"; str2 += "\n";
					count++; nodeSet.push_back(node);
					treeVector.push_back(nodeInfo);   treeSet.insert(nodeInfo);  expressionVector.push_back(expInfo);	  
					break;
				}	
			}	
		}		
	}		
	Pstart=count;
	for (int j = 0; j < _scene->_proteinNameD[dataID].size(); j ++)
	{
	    if(proteinRect[j].width()<0)//for big bubble
		{
			vector<int> node; node.push_back('P'); node.push_back(j);
			//for(int i=0; i< _scene->_compartmentName[m_pathwayID].size()-1; i++)
			{
				if(_scene->_proteinNameD[dataID][j][1]!="" && _scene->_proteinNameD[dataID][j][0]!="")
				//if(_scene->CompartmentContain[m_pathwayID][i].find(node)!=_scene->CompartmentContain[m_pathwayID][i].end())
				{   
					tmpStr.setNum( count );
					str += tmpStr; str2 += tmpStr; str2 +="\t";
					N_str=_scene->_proteinNameD[dataID][j][1];	
					
					spaceReplace(N_str);
					str2 +=  N_str;	nodeInfo[0]=N_str;
					str2 +="\t";
					//str += " protein:"; str2 += "protein:";
					N_str = _scene->_proteinNameD[dataID][j][0];
					gid = findExpressionID(N_str, geneInfo, 'P');
					spaceReplace(N_str);
					str2 +=  N_str;	nodeInfo[1]=N_str;

					expInfo[0]=QString::number(0);
					expInfo[1]=QString::number(0);
					if(gid<0)
						str += "\t0\t0";
					else
					{
						expInfo[0]=QString::number(quantity[gid][0], '1', 4);
						expInfo[1]=QString::number(quantity[gid][1], '1', 4);
						str += "\t"; str += expInfo[0];
						str += "\t"; str += expInfo[1];
					}
					switch(node[0])
					{
						case 'P': nodeInfo[2]="P"; break;
						case 'E': nodeInfo[2]="E"; break;
						case 'C': nodeInfo[2]="C"; break;
						case 'D': nodeInfo[2]="D"; break;
						case 'S': nodeInfo[2]="S"; break;
						case 'R': nodeInfo[2]="R"; break;
					}
					str += "\n"; str2 += "\n";
					count++; nodeSet.push_back(node);
					treeVector.push_back(nodeInfo);   treeSet.insert(nodeInfo);  expressionVector.push_back(expInfo);	  
					break;
				}	
			}
		}		
	}
	Sstart=count;
	for (int j = 0; j < _scene->_smallMoleculeNameD[dataID].size(); j ++)
	{
		if(smallMoleculeRect[j].width()<0)
		{
			vector<int> node; node.push_back('S'); node.push_back(j);
			//for(int i=0; i< _scene->_compartmentName[m_pathwayID].size()-1; i++)
			{	
				if(_scene->_smallMoleculeNameD[dataID][j][1]!="")
				//if(_scene->CompartmentContain[m_pathwayID][i].find(node)!=_scene->CompartmentContain[m_pathwayID][i].end())
				{   
					tmpStr.setNum( count );
					str += tmpStr; str2 += tmpStr; str2 +="\t";
					N_str=_scene->_smallMoleculeNameD[dataID][j][1];	
					spaceReplace(N_str);
					str2 +=  N_str;	nodeInfo[0]=N_str;
					str2 +="\t";
					//str += " smallMolecule:"; str2 += "smallMolecule:";
					N_str = _scene->_smallMoleculeNameD[dataID][j][0];
					spaceReplace(N_str);
					str2 +=  N_str;	nodeInfo[1]=N_str;

					expInfo[0]=QString::number(0);
					expInfo[1]=QString::number(0);
					str += "\t0\t0";
					switch(node[0])
					{
						case 'P': nodeInfo[2]="P"; break;
						case 'E': nodeInfo[2]="E"; break;
						case 'C': nodeInfo[2]="C"; break;
						case 'D': nodeInfo[2]="D"; break;
						case 'S': nodeInfo[2]="S"; break;
						case 'R': nodeInfo[2]="R"; break;
					}
					str += "\n"; str2 += "\n";
					count++; nodeSet.push_back(node);
					treeVector.push_back(nodeInfo);   treeSet.insert(nodeInfo);  expressionVector.push_back(expInfo);	  
					break;
				}	
			}
		}		
	}
	Dstart=count;
	for (int j = 0; j < _scene->_DnaNameD[dataID].size(); j ++)
	{		
		if(DnaRect[j].width()<0)
		{
			vector<int> node; node.push_back('D'); node.push_back(j);
			//for(int i=0; i< _scene->_compartmentName[m_pathwayID].size()-1; i++)
			{
				if(_scene->_DnaNameD[dataID][j][1]!="")
				//if(_scene->CompartmentContain[m_pathwayID][i].find(node)!=_scene->CompartmentContain[m_pathwayID][i].end())
				{   
					tmpStr.setNum( count ); 
					str += tmpStr;  str2 += tmpStr; str2 +="\t";
					N_str=_scene->_DnaNameD[dataID][j][1];	
					spaceReplace(N_str);
					str2 +=  N_str;	nodeInfo[0]=N_str;
					str2 +="\t";
					//str += " Dna:"; str2 += "Dna:";
					N_str = _scene->_DnaNameD[dataID][j][0];
					spaceReplace(N_str);		
					str2 +=  N_str;	nodeInfo[1]=N_str;
					str += "\t0\t0";
					expInfo[0]=QString::number(0);
					expInfo[1]=QString::number(0);
					switch(node[0])
					{
						case 'P': nodeInfo[2]="P"; break;
						case 'E': nodeInfo[2]="E"; break;
						case 'C': nodeInfo[2]="C"; break;
						case 'D': nodeInfo[2]="D"; break;
						case 'S': nodeInfo[2]="S"; break;
						case 'R': nodeInfo[2]="R"; break;
					}
					str += "\n"; str2 += "\n";
					count++; nodeSet.push_back(node);
					treeVector.push_back(nodeInfo);   treeSet.insert(nodeInfo);  expressionVector.push_back(expInfo);	  
					break;
				}	
			}
		}
	}
	Rstart=count;

	
	
	//treeVector.push_back(nodeInfo);   treeSet.insert(nodeInfo);  expressionVector.push_back(expInfo);	
	//get orders
	vector<int> tOrder, rOrder;
	rOrder.resize(treeSet.size());
	for(int i=0; i <treeVector.size(); i++)
	{
		int ocount=0;		
		for(set<vector<QString>>::iterator it=treeSet.begin(); it!=treeSet.end(); it++)
		{
			if(treeVector[i]==*it)
			{
			   tOrder.push_back(ocount);
			   rOrder[ocount]=i;
			   break;
			}		    
			ocount++;
		}
	}


	str2="";
	for(int k=0; k< rOrder.size(); k++)
	{
		tmpStr.setNum(k);    
		str2 += tmpStr;
	    int id=rOrder[k];
		expInfo = expressionVector[id];
		for(int k=0; k<expInfo.size(); k++)
		{
		   str2 += "\t";
		   str2 +=  expInfo[k];
		}
		str2 += "\n";
	}
	text = doc.createTextNode( str2 );
	QTextStream out(&file);
	text.save( out, 4 );
	file.close();	

	/// tree data
	filename3 = filename;
	if(drawSmallmolecule)
	{
	    filename3 = filename3 + "_S";
	}
	filename3 +="_tree_b.out";	
	QFile file3( filename3 );

	if(!file3.open(QIODevice::WriteOnly | QIODevice::Truncate))
		return;
	
	str2="";
	int ocount=0;
	for(set<vector<QString>>::iterator it=treeSet.begin(); it!=treeSet.end(); it++)		
	{
		nodeInfo = *it;
		tmpStr.setNum(ocount);
	    str2 += tmpStr;
		for(int k=0; k<nodeInfo.size(); k++)
		{
		   str2 += "\t";
		   str2 +=  nodeInfo[k];
		}
		str2 += "\n";
        ocount++;
	}		

    text = doc.createTextNode( str2 );
	QTextStream out3(&file3);
	text.save( out3, 4 );
	file3.close();	
	
	//edge
	/*str += "EDGES ";	
	tmpStr.setNum( edge.size()*2 );
	str += tmpStr;
	str += "\n";*/
	QString eType;
	set<vector<int>> testset; 
	vector<int> node(2,0);
	int e1,e2;
	vector<int> temp1(2,0),temp2(2,0);
	str="";
	for(int j=0; j<edge.size(); j++)
	{
		int start;
        switch(edge[j][0])
	    {
		     case 'E': start=Estart-1; break; 	
			 case 'C': start=Cstart-1; break;
			 case 'S': start=Sstart-1; break;			
			 case 'P': start=Pstart-1; break;
		     case 'D': start=Dstart-1; break;
			 case 'R': start=Rstart-1; break;	
			 default: start = -1; break;
		}
		node[0]=edge[j][0]; node[1]=edge[j][1];
		e1= findTreeNodeID(node, nodeSet); //edge[j][1] +start;
		if(e1>=0)
			e1= tOrder[e1];
		switch(edge[j][4])
	    {
		     case 'E': start=Estart-1; break; 	
			 case 'C': start=Cstart-1; break;
			 case 'S': start=Sstart-1; break;			
			 case 'P': start=Pstart-1; break;
		     case 'D': start=Dstart-1; break;
			 case 'R': start=Rstart-1; break;			
			 default: start = -1; break;
		}
		node[0]=edge[j][4]; node[1]=edge[j][5];
		e2= findTreeNodeID(node, nodeSet);  //e2=edge[j][5] +start;
		if(e2>=0)
			e2= tOrder[e2];
		temp1[0]=e1;  temp1[1]=e2; 
		if(temp1[0]>=0 && temp1[1]>=0)
		if(testset.find(temp1)==testset.end())
		{
			eType = findEdgeType(0, edge[j][4], edge[j][5], 0, edge[j][0], edge[j][1]);
			testset.insert(temp1);
			tmpStr.setNum( e1 );		
			str += tmpStr;
			str += "\t";

			tmpStr.setNum( e2 );
			str += tmpStr;

			str += "\t";
			str += eType;
			str += "\n";
		}
	    
		//node[0]=edge[j][4]; node[1]=edge[j][5];
		e1= e2; //findTreeNodeID(node, nodeSet);
		//e1=edge[j][5] +start;
		switch(edge[j][2])
	    {
		     case 'E': start=Estart-1; break; 	
			 case 'C': start=Cstart-1; break;
			 case 'S': start=Sstart-1; break;			
			 case 'P': start=Pstart-1; break;
		     case 'D': start=Dstart-1; break;
			 case 'R': start=Rstart-1; break;
			 default: start = -1; break;
		}
		node[0]=edge[j][2]; node[1]=edge[j][3];
		e2= findTreeNodeID(node, nodeSet);	//e2=edge[j][3] +start;	
		if(e2>=0)
			e2= tOrder[e2];
		temp1[0]=e1;  temp1[1]=e2;
		//if(e1>=0 && e2>=0)
		if(temp1[0]>=0 && temp1[1]>=0)
		if(testset.find(temp1)==testset.end())
		{
			eType = findEdgeType(0, edge[j][2], edge[j][3], 0, edge[j][4], edge[j][5]);
			testset.insert(temp1);
			tmpStr.setNum( e1 );
			str += tmpStr;
			str += "\t";
				
			tmpStr.setNum( e2 );
			str += tmpStr;

			str += "\t";
			str += eType;
			str += "\n";	
		}		
	}		
	filename2 = filename;
	if(drawSmallmolecule)
	{
	    filename2 = filename2 + "_S";
	}
	filename2 +="_graph_b.out";	
	QFile file2( filename2 );

	if(!file2.open(QIODevice::WriteOnly | QIODevice::Truncate))
		return;

	text = doc.createTextNode( str );
	QTextStream out2(&file2);
	text.save( out2, 4 );
	file2.close();	
	
}

void PathBubble1::savePathWayforTreeRing_1(QString filename,  vector<vector<int>> edge)
{   //save edge and node (not include position)//similar to VTK style

	//all nodes, not only that in my hierarchical graph

    QString str,tmpStr,N_str,str2;
	int gid;
	QDomText text;
	QDomDocument doc;

	vector<vector<int>> nodeSet;
	
	int count=0,Estart,Cstart,Pstart,Sstart,Dstart,Rstart;
	
	QString filename1,filename2,filename3;
	filename1 =filename;
	if(drawSmallmolecule)
	{
	    filename1 = filename1 + "_S";
	}
	filename1 +="_expression_lib15-Heat_b.out"; //lib15-Heat_part_0expression
	QFile file( filename1 );
	if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
		return;

	vector<vector<QString>> treeVector;
	set<vector<QString>> treeSet;
	vector<QString> nodeInfo(3,"");

	vector<vector<QString>> expressionVector;
	vector<QString> expInfo(2,"0");

	vector<vector<QString>> graphVector;
		

	vector<QString> quantityName;
	vector<vector<QString>> geneInfo;
	vector<vector<float>> quantity;
	vector<vector<float>> minmax;
	readexpression("data/Reactome_Pathway_Data/expressionData/lib15-Heat0expression.txt", quantityName, geneInfo, quantity, minmax);//TGF0expression
	Estart=count;
	for (int j = 0; j < _scene->_physicalEntityNameD[dataID].size(); j++)
	{
        //if(physicalEntityRect[j].width()>=0)
		{
			vector<int> node; 	node.push_back('E'); node.push_back(j);							
			for(int i=0; i< _scene->_compartmentName[m_pathwayID].size()-1; i++)
			{		
				if(_scene->_compartmentName[m_pathwayID][i][0]!="")
				if(_scene->CompartmentContain[m_pathwayID][i].find(node)!=_scene->CompartmentContain[m_pathwayID][i].end())
				{  					
					tmpStr.setNum( count );
			        str += tmpStr; str2 += tmpStr; str2 += "\t";			        	
					N_str=_scene->_compartmentName[m_pathwayID][i][0];	
					spaceReplace(N_str);
					str2 +=  N_str;	nodeInfo[0]=N_str;
					str2 +="\t";
					//str += " physicalEntity:";  //str2 += "physicalEntity:";
					N_str = _scene->_physicalEntityNameD[dataID][j][0]; 
					spaceReplace(N_str);
					
					str2 += N_str; nodeInfo[1]=N_str;
					str += "\t0\t0";	
					expInfo[0]=QString::number(0);
					expInfo[1]=QString::number(0);
					switch(node[0])
					{
						case 'P': nodeInfo[2]="Protein"; break;
						case 'E': nodeInfo[2]="PhysicalEntity"; break;
						case 'C': nodeInfo[2]="Complex"; break;
						case 'D': nodeInfo[2]="Dna"; break;
						case 'S': nodeInfo[2]="SmallMolecule"; break;
						case 'R': nodeInfo[2]="Reaction"; break;
					}
					//str2 += "physicalEntity:";
					str += "\n"; str2 += "\n";		
					count++; nodeSet.push_back(node);	
					treeVector.push_back(nodeInfo);   treeSet.insert(nodeInfo);  expressionVector.push_back(expInfo);	                
					break;
				}	
			}		
			
		}
	}
	Cstart=count;
	for (int j = 0; j < _scene->_complexNameD[dataID].size(); j ++)
	{
		//if(complexRect[j].width()>=0)
		{
			vector<int> node; 
			node.push_back('C'); node.push_back(j);	
			for(int i=0; i< _scene->_compartmentName[m_pathwayID].size()-1; i++)
			{
				if(_scene->_compartmentName[m_pathwayID][i][0]!="")
				if(_scene->CompartmentContain[m_pathwayID][i].find(node)!=_scene->CompartmentContain[m_pathwayID][i].end())
				{   
					tmpStr.setNum( count );
					str += tmpStr; str2 += tmpStr; str2 +="\t";		
					N_str=_scene->_compartmentName[m_pathwayID][i][0];					
					
					spaceReplace(N_str);
					str2 +=  N_str;	nodeInfo[0]=N_str;
					str2 +="\t";
					//str += " Complex:"; str2 += "Complex:";
					N_str = _scene->_complexNameD[dataID][j][0]; 
					gid = findExpressionID(N_str, geneInfo, 'C');
					spaceReplace(N_str);
					str2 +=  N_str;	nodeInfo[1]=N_str;
					
					expInfo[0]=QString::number(0);
					expInfo[1]=QString::number(0);

					if(gid<0)
						str += "\t0\t0";
					else
					{
						expInfo[0]=QString::number(quantity[gid][0], '1', 4);
						expInfo[1]=QString::number(quantity[gid][1], '1', 4);
						str += "\t"; str += expInfo[0];
						str += "\t"; str += expInfo[1];						
					}
					switch(node[0])
					{
						case 'P': nodeInfo[2]="Protein"; break;
						case 'E': nodeInfo[2]="PhysicalEntity"; break;
						case 'C': nodeInfo[2]="Complex"; break;
						case 'D': nodeInfo[2]="Dna"; break;
						case 'S': nodeInfo[2]="SmallMolecule"; break;
						case 'R': nodeInfo[2]="Reaction"; break;
					}

					str += "\n"; str2 += "\n";
					count++;  nodeSet.push_back(node);
					treeVector.push_back(nodeInfo);   treeSet.insert(nodeInfo);  expressionVector.push_back(expInfo);	
					break;
				}	
			}	
		}		
	}		
	Pstart=count;
	for (int j = 0; j < _scene->_proteinNameD[dataID].size(); j ++)
	{
	    //if(proteinRect[j].width()>=0)
		{
			vector<int> node; node.push_back('P'); node.push_back(j);
			for(int i=0; i< _scene->_compartmentName[m_pathwayID].size()-1; i++)
			{
				if(_scene->_compartmentName[m_pathwayID][i][0]!="" && _scene->_proteinNameD[dataID][j][0]!="")
				if(_scene->CompartmentContain[m_pathwayID][i].find(node)!=_scene->CompartmentContain[m_pathwayID][i].end())
				{   
					tmpStr.setNum( count );
					str += tmpStr; str2 += tmpStr; str2 +="\t";
					N_str=_scene->_compartmentName[m_pathwayID][i][0];	
					
					spaceReplace(N_str);
					str2 +=  N_str;	nodeInfo[0]=N_str;
					str2 +="\t";
					//str += " protein:"; str2 += "protein:";
					N_str = _scene->_proteinNameD[dataID][j][0];
					gid = findExpressionID(N_str, geneInfo, 'P');
					spaceReplace(N_str);
					str2 +=  N_str;	nodeInfo[1]=N_str;

					expInfo[0]=QString::number(0);
					expInfo[1]=QString::number(0);

					if(gid<0)
						str += "\t0\t0";
					else
					{
						expInfo[0]=QString::number(quantity[gid][0], '1', 4);
						expInfo[1]=QString::number(quantity[gid][1], '1', 4);
						str += "\t"; str += expInfo[0];
						str += "\t"; str += expInfo[1];		
					}
					switch(node[0])
					{
						case 'P': nodeInfo[2]="Protein"; break;
						case 'E': nodeInfo[2]="PhysicalEntity"; break;
						case 'C': nodeInfo[2]="Complex"; break;
						case 'D': nodeInfo[2]="Dna"; break;
						case 'S': nodeInfo[2]="SmallMolecule"; break;
						case 'R': nodeInfo[2]="Reaction"; break;
					}
					str += "\n"; str2 += "\n";
					count++; nodeSet.push_back(node);
					treeVector.push_back(nodeInfo);   treeSet.insert(nodeInfo);  expressionVector.push_back(expInfo);	  
					break;
				}	
			}
		}		
	}
	Sstart=count;
	for (int j = 0; j < _scene->_smallMoleculeNameD[dataID].size(); j ++)
	{
		//if(smallMoleculeRect[j].width()>=0)
		{
			vector<int> node; node.push_back('S'); node.push_back(j);
			for(int i=0; i< _scene->_compartmentName[m_pathwayID].size()-1; i++)
			{	
				if(_scene->_compartmentName[m_pathwayID][i][0]!="")
				if(_scene->CompartmentContain[m_pathwayID][i].find(node)!=_scene->CompartmentContain[m_pathwayID][i].end())
				{   
					tmpStr.setNum( count );
					str += tmpStr; str2 += tmpStr; str2 +="\t";
					N_str=_scene->_compartmentName[m_pathwayID][i][0];	
					spaceReplace(N_str);
					str2 +=  N_str;	nodeInfo[0]=N_str;
					str2 +="\t";
					//str += " smallMolecule:"; str2 += "smallMolecule:";
					N_str = _scene->_smallMoleculeNameD[dataID][j][0];
					spaceReplace(N_str);
					str2 +=  N_str;	nodeInfo[1]=N_str;
					str += "\t0\t0";
					expInfo[0]=QString::number(0);
					expInfo[1]=QString::number(0);
					switch(node[0])
					{
						case 'P': nodeInfo[2]="Protein"; break;
						case 'E': nodeInfo[2]="PhysicalEntity"; break;
						case 'C': nodeInfo[2]="Complex"; break;
						case 'D': nodeInfo[2]="Dna"; break;
						case 'S': nodeInfo[2]="SmallMolecule"; break;
						case 'R': nodeInfo[2]="Reaction"; break;
					}
					str += "\n"; str2 += "\n";
					count++; nodeSet.push_back(node);
					treeVector.push_back(nodeInfo);   treeSet.insert(nodeInfo);  expressionVector.push_back(expInfo);	  
					break;
				}	
			}
		}		
	}
	Dstart=count;
	for (int j = 0; j < _scene->_DnaNameD[dataID].size(); j ++)
	{		
		//if(DnaRect[j].width()>=0)//for big bubble
		{
			vector<int> node; node.push_back('D'); node.push_back(j);
			for(int i=0; i< _scene->_compartmentName[m_pathwayID].size()-1; i++)
			{
				if(_scene->_compartmentName[m_pathwayID][i][0]!="")
				if(_scene->CompartmentContain[m_pathwayID][i].find(node)!=_scene->CompartmentContain[m_pathwayID][i].end())
				{   
					tmpStr.setNum( count );
					str += tmpStr;  str2 += tmpStr; str2 +="\t";
					N_str=_scene->_compartmentName[m_pathwayID][i][0];	
					spaceReplace(N_str);
					str2 +=  N_str;	nodeInfo[0]=N_str;
					str2 +="\t";
					//str += " Dna:"; str2 += "Dna:";
					N_str = _scene->_DnaNameD[dataID][j][0];
					spaceReplace(N_str);		
					str2 +=  N_str;	nodeInfo[1]=N_str;
					str += "\t0\t0";
					expInfo[0]=QString::number(0);
					expInfo[1]=QString::number(0);
					switch(node[0])
					{
						case 'P': nodeInfo[2]="Protein"; break;
						case 'E': nodeInfo[2]="PhysicalEntity"; break;
						case 'C': nodeInfo[2]="Complex"; break;
						case 'D': nodeInfo[2]="Dna"; break;
						case 'S': nodeInfo[2]="SmallMolecule"; break;
						case 'R': nodeInfo[2]="Reaction"; break;
					}
					str += "\n"; str2 += "\n";
					count++; nodeSet.push_back(node);
					treeVector.push_back(nodeInfo);   treeSet.insert(nodeInfo);  expressionVector.push_back(expInfo);	  
					break;
				}	
			}
		}
	}
	Rstart=count;
	for (int j = 0; j < _scene->_reactionNameD[dataID].size(); j ++)
	{
		//if(reactionRect[j].width()>=0)//for big bubble
		{
			vector<int> node; node.push_back('R'); node.push_back(j);
			for(int i=0; i< _scene->_compartmentName[m_pathwayID].size()-1; i++)
			{
				if(_scene->_compartmentName[m_pathwayID][i][0]!="")
				if(_scene->CompartmentContain[m_pathwayID][i].find(node)!=_scene->CompartmentContain[m_pathwayID][i].end())
				{   
					tmpStr.setNum( count );
					str += tmpStr;  str2 += tmpStr; str2 +="\t";
					N_str=_scene->_compartmentName[m_pathwayID][i][0];	
					spaceReplace(N_str);
					str2 +=  N_str;	nodeInfo[0]=N_str;
					str2 +="\t";
					//str += " reaction:"; str2 += "reaction:";
					N_str=_scene->_reactionNameD[dataID][j][1];
					spaceReplace(N_str);
					str2 +=  N_str;	nodeInfo[1]=N_str;
					str += "\t0\t0";
					expInfo[0]=QString::number(0);
					expInfo[1]=QString::number(0);
					switch(node[0])
					{
						case 'P': nodeInfo[2]="Protein"; break;
						case 'E': nodeInfo[2]="PhysicalEntity"; break;
						case 'C': nodeInfo[2]="Complex"; break;
						case 'D': nodeInfo[2]="Dna"; break;
						case 'S': nodeInfo[2]="SmallMolecule"; break;
						case 'R': nodeInfo[2]="Reaction"; break;
					}
					str += "\n"; str2 += "\n";
					count++; nodeSet.push_back(node);
					treeVector.push_back(nodeInfo);   treeSet.insert(nodeInfo);  expressionVector.push_back(expInfo);	  
					break;
				}	
			}
		}
	}

	//for the rest not in the graph
	//count=0;
	////for big bubble
	/*
	Cstart=count;
	for (int j = 1; j < _scene->_complexNameD[dataID].size(); j ++)
	{
		if(complexRect[j].width()<0)//for big bubble
		{
			vector<int> node; 
			node.push_back('C'); node.push_back(j);	
			//for(int i=0; i< _scene->_compartmentName[m_pathwayID].size()-1; i++)
			{
				if(_scene->_complexNameD[dataID][j][1]!="")
				//if(_scene->CompartmentContain[m_pathwayID][i].find(node)!=_scene->CompartmentContain[m_pathwayID][i].end())
				{   
					tmpStr.setNum( count );
					str += tmpStr; str2 += tmpStr; str2 +="\t";		
					N_str=_scene->_complexNameD[dataID][j][1];					
					

					spaceReplace(N_str);
					str2 +=  N_str;	nodeInfo[0]=N_str;			
					str2 +="\t";
					//str += " Complex:"; str2 += "Complex:";
					N_str = _scene->_complexNameD[dataID][j][0]; 
					gid = findExpressionID(N_str, geneInfo, 'C');
					spaceReplace(N_str);
					str2 +=  N_str;	nodeInfo[1]=N_str;
					

					expInfo[0]=QString::number(0);
					expInfo[1]=QString::number(0);
					if(gid<0)
						str += "\t0\t0";
					else
					{
						expInfo[0]=QString::number(quantity[gid][0], '1', 4);
						expInfo[1]=QString::number(quantity[gid][1], '1', 4);
						str += "\t"; str += expInfo[0];
						str += "\t"; str += expInfo[1];
					}
					switch(node[0])
					{
						case 'P': nodeInfo[2]="P"; break;
						case 'E': nodeInfo[2]="E"; break;
						case 'C': nodeInfo[2]="C"; break;
						case 'D': nodeInfo[2]="D"; break;
						case 'S': nodeInfo[2]="S"; break;
						case 'R': nodeInfo[2]="R"; break;
					}
					str += "\n"; str2 += "\n";
					count++; nodeSet.push_back(node);
					treeVector.push_back(nodeInfo);   treeSet.insert(nodeInfo);  expressionVector.push_back(expInfo);	  
					break;
				}	
			}	
		}		
	}		
	Pstart=count;
	for (int j = 1; j < _scene->_proteinNameD[dataID].size(); j ++)
	{
	    if(proteinRect[j].width()<0)//for big bubble
		{
			vector<int> node; node.push_back('P'); node.push_back(j);
			//for(int i=0; i< _scene->_compartmentName[m_pathwayID].size()-1; i++)
			{
				if(_scene->_proteinNameD[dataID][j][1]!="" && _scene->_proteinNameD[dataID][j][0]!="")
				//if(_scene->CompartmentContain[m_pathwayID][i].find(node)!=_scene->CompartmentContain[m_pathwayID][i].end())
				{   
					tmpStr.setNum( count );
					str += tmpStr; str2 += tmpStr; str2 +="\t";
					N_str=_scene->_proteinNameD[dataID][j][1];	
					
					spaceReplace(N_str);
					str2 +=  N_str;	nodeInfo[0]=N_str;
					str2 +="\t";
					//str += " protein:"; str2 += "protein:";
					N_str = _scene->_proteinNameD[dataID][j][0];
					gid = findExpressionID(N_str, geneInfo, 'P');
					spaceReplace(N_str);
					str2 +=  N_str;	nodeInfo[1]=N_str;

					expInfo[0]=QString::number(0);
					expInfo[1]=QString::number(0);
					if(gid<0)
						str += "\t0\t0";
					else
					{
						expInfo[0]=QString::number(quantity[gid][0], '1', 4);
						expInfo[1]=QString::number(quantity[gid][1], '1', 4);
						str += "\t"; str += expInfo[0];
						str += "\t"; str += expInfo[1];
					}
					switch(node[0])
					{
						case 'P': nodeInfo[2]="P"; break;
						case 'E': nodeInfo[2]="E"; break;
						case 'C': nodeInfo[2]="C"; break;
						case 'D': nodeInfo[2]="D"; break;
						case 'S': nodeInfo[2]="S"; break;
						case 'R': nodeInfo[2]="R"; break;
					}
					str += "\n"; str2 += "\n";
					count++; nodeSet.push_back(node);
					treeVector.push_back(nodeInfo);   treeSet.insert(nodeInfo);  expressionVector.push_back(expInfo);	  
					break;
				}	
			}
		}		
	}
	Sstart=count;
	for (int j = 1; j < _scene->_smallMoleculeNameD[dataID].size(); j ++)
	{
		if(smallMoleculeRect[j].width()<0)
		{
			vector<int> node; node.push_back('S'); node.push_back(j);
			//for(int i=0; i< _scene->_compartmentName[m_pathwayID].size()-1; i++)
			{	
				if(_scene->_smallMoleculeNameD[dataID][j][1]!="")
				//if(_scene->CompartmentContain[m_pathwayID][i].find(node)!=_scene->CompartmentContain[m_pathwayID][i].end())
				{   
					tmpStr.setNum( count );
					str += tmpStr; str2 += tmpStr; str2 +="\t";
					N_str=_scene->_smallMoleculeNameD[dataID][j][1];	
					spaceReplace(N_str);
					str2 +=  N_str;	nodeInfo[0]=N_str;
					str2 +="\t";
					//str += " smallMolecule:"; str2 += "smallMolecule:";
					N_str = _scene->_smallMoleculeNameD[dataID][j][0];
					spaceReplace(N_str);
					str2 +=  N_str;	nodeInfo[1]=N_str;

					expInfo[0]=QString::number(0);
					expInfo[1]=QString::number(0);
					str += "\t0\t0";
					switch(node[0])
					{
						case 'P': nodeInfo[2]="P"; break;
						case 'E': nodeInfo[2]="E"; break;
						case 'C': nodeInfo[2]="C"; break;
						case 'D': nodeInfo[2]="D"; break;
						case 'S': nodeInfo[2]="S"; break;
						case 'R': nodeInfo[2]="R"; break;
					}
					str += "\n"; str2 += "\n";
					count++; nodeSet.push_back(node);
					treeVector.push_back(nodeInfo);   treeSet.insert(nodeInfo);  expressionVector.push_back(expInfo);	  
					break;
				}	
			}
		}		
	}
	Dstart=count;
	for (int j = 1; j < _scene->_DnaNameD[dataID].size(); j ++)
	{		
		if(DnaRect[j].width()<0)
		{
			vector<int> node; node.push_back('D'); node.push_back(j);
			//for(int i=0; i< _scene->_compartmentName[m_pathwayID].size()-1; i++)
			{
				if(_scene->_DnaNameD[dataID][j][1]!="")
				//if(_scene->CompartmentContain[m_pathwayID][i].find(node)!=_scene->CompartmentContain[m_pathwayID][i].end())
				{   
					tmpStr.setNum( count ); 
					str += tmpStr;  str2 += tmpStr; str2 +="\t";
					N_str=_scene->_DnaNameD[dataID][j][1];	
					spaceReplace(N_str);
					str2 +=  N_str;	nodeInfo[0]=N_str;
					str2 +="\t";
					//str += " Dna:"; str2 += "Dna:";
					N_str = _scene->_DnaNameD[dataID][j][0];
					spaceReplace(N_str);		
					str2 +=  N_str;	nodeInfo[1]=N_str;
					str += "\t0\t0";
					expInfo[0]=QString::number(0);
					expInfo[1]=QString::number(0);
					switch(node[0])
					{
						case 'P': nodeInfo[2]="P"; break;
						case 'E': nodeInfo[2]="E"; break;
						case 'C': nodeInfo[2]="C"; break;
						case 'D': nodeInfo[2]="D"; break;
						case 'S': nodeInfo[2]="S"; break;
						case 'R': nodeInfo[2]="R"; break;
					}
					str += "\n"; str2 += "\n";
					count++; nodeSet.push_back(node);
					treeVector.push_back(nodeInfo);   treeSet.insert(nodeInfo);  expressionVector.push_back(expInfo);	  
					break;
				}	
			}
		}
	}
	Rstart=count;

	*////for big bubble
	
	//treeVector.push_back(nodeInfo);   treeSet.insert(nodeInfo);  expressionVector.push_back(expInfo);	
	//get orders
	vector<int> tOrder, rOrder;
	rOrder.resize(treeSet.size());
	for(int i=0; i <treeVector.size(); i++)
	{
		int ocount=0;		
		for(set<vector<QString>>::iterator it=treeSet.begin(); it!=treeSet.end(); it++)
		{
			if(treeVector[i]==*it)
			{
			   tOrder.push_back(ocount);
			   rOrder[ocount]=i;
			   break;
			}		    
			ocount++;
		}
	}


	str2="";
	for(int k=0; k< rOrder.size(); k++)
	{
		tmpStr.setNum(k);    
		str2 += tmpStr;
	    int id=rOrder[k];
		expInfo = expressionVector[id];
		for(int k=0; k<expInfo.size(); k++)
		{
		   str2 += "\t";
		   str2 +=  expInfo[k];
		}
		str2 += "\n";
	}
	text = doc.createTextNode( str2 );
	QTextStream out(&file);
	text.save( out, 4 );
	file.close();	

	/// tree data
	filename3 = filename;
	if(drawSmallmolecule)
	{
	    filename3 = filename3 + "_S";
	}
	filename3 +="_tree_b.out";	
	QFile file3( filename3 );

	if(!file3.open(QIODevice::WriteOnly | QIODevice::Truncate))
		return;
	
	str2="";
	int ocount=0;
	for(set<vector<QString>>::iterator it=treeSet.begin(); it!=treeSet.end(); it++)		
	{
		nodeInfo = *it;
		tmpStr.setNum(ocount);
	    str2 += tmpStr;
		for(int k=0; k<nodeInfo.size(); k++)
		{
		   str2 += "\t";
		   str2 +=  nodeInfo[k];
		}
		str2 += "\n";
        ocount++;
	}		

    text = doc.createTextNode( str2 );
	QTextStream out3(&file3);
	text.save( out3, 4 );
	file3.close();	
	
	//edge
	/*str += "EDGES ";	
	tmpStr.setNum( edge.size()*2 );
	str += tmpStr;
	str += "\n";*/
	QString eType;
	set<vector<int>> testset; 
	vector<int> node(2,0);
	int e1,e2;
	vector<int> temp1(2,0),temp2(2,0);
	str="";
	for(int j=0; j<edge.size(); j++)
	{
		int start;
        switch(edge[j][0])
	    {
		     case 'E': start=Estart-1; break; 	
			 case 'C': start=Cstart-1; break;
			 case 'S': start=Sstart-1; break;			
			 case 'P': start=Pstart-1; break;
		     case 'D': start=Dstart-1; break;
			 case 'R': start=Rstart-1; break;	
			 default: start = -1; break;
		}
		node[0]=edge[j][0]; node[1]=edge[j][1];
		e1= findTreeNodeID(node, nodeSet); //edge[j][1] +start;
		if(e1>=0)
			e1= tOrder[e1];
		switch(edge[j][4])
	    {
		     case 'E': start=Estart-1; break; 	
			 case 'C': start=Cstart-1; break;
			 case 'S': start=Sstart-1; break;			
			 case 'P': start=Pstart-1; break;
		     case 'D': start=Dstart-1; break;
			 case 'R': start=Rstart-1; break;			
			 default: start = -1; break;
		}
		node[0]=edge[j][4]; node[1]=edge[j][5];
		e2= findTreeNodeID(node, nodeSet);  //e2=edge[j][5] +start;
		if(e2>=0)
			e2= tOrder[e2];
		temp1[0]=e1;  temp1[1]=e2; 
		if(temp1[0]>=0 && temp1[1]>=0)
		if(testset.find(temp1)==testset.end())
		{
			eType = findEdgeType(0, edge[j][4], edge[j][5], 0, edge[j][0], edge[j][1]);
			testset.insert(temp1);
			tmpStr.setNum( e1 );		
			str += tmpStr;
			str += "\t";

			tmpStr.setNum( e2 );
			str += tmpStr;

			str += "\t";
			str += eType;
			str += "\n";
		}
	    
		//node[0]=edge[j][4]; node[1]=edge[j][5];
		e1= e2; //findTreeNodeID(node, nodeSet);
		//e1=edge[j][5] +start;
		switch(edge[j][2])
	    {
		     case 'E': start=Estart-1; break; 	
			 case 'C': start=Cstart-1; break;
			 case 'S': start=Sstart-1; break;			
			 case 'P': start=Pstart-1; break;
		     case 'D': start=Dstart-1; break;
			 case 'R': start=Rstart-1; break;
			 default: start = -1; break;
		}
		node[0]=edge[j][2]; node[1]=edge[j][3];
		e2= findTreeNodeID(node, nodeSet);	//e2=edge[j][3] +start;	
		if(e2>=0)
			e2= tOrder[e2];
		temp1[0]=e1;  temp1[1]=e2;
		//if(e1>=0 && e2>=0)
		if(temp1[0]>=0 && temp1[1]>=0)
		if(testset.find(temp1)==testset.end())
		{
			eType = findEdgeType(0, edge[j][2], edge[j][3], 0, edge[j][4], edge[j][5]);

			testset.insert(temp1);
			tmpStr.setNum( e1 );
			str += tmpStr;
			str += "\t";
				
			tmpStr.setNum( e2 );
			str += tmpStr;

			str += "\t";
			str += eType;
			str += "\n";	
		}		
	}		
	filename2 = filename;
	if(drawSmallmolecule)
	{
	    filename2 = filename2 + "_S";
	}
	filename2 +="_graph_b.out";	
	QFile file2( filename2 );

	if(!file2.open(QIODevice::WriteOnly | QIODevice::Truncate))
		return;

	text = doc.createTextNode( str );
	QTextStream out2(&file2);
	text.save( out2, 4 );
	file2.close();	
	

	
}

int PathBubble1::findTreeNodeID(vector<int> node, vector<vector<int>> nodeSet)
{
	for(int i=0; i < nodeSet.size(); i++)
	{
		if(node== nodeSet[i])
			return i;
	}
	return -1;
}

QString PathBubble1::findEdgeType(int pid1, int type1, int id1, int pid2, int type2, int id2)
{
	for(set<vector<vector<int> > >::iterator it = updatedEdge_2.begin(); it != updatedEdge_2.end(); it++)
	{    
		//activation
	    vector<vector<int> > edge=*it;
		if( (pid2 == edge[0][0] && type2==edge[0][1] && id2==edge[0][2] && pid1==edge[1][0] && type1==edge[1][1] && id1==edge[1][2])
		   || (pid1 == edge[0][0] && type1==edge[0][1] && id1==edge[0][2] && pid2==edge[1][0] && type2==edge[1][1] && id2==edge[1][2]))
		{
		    return "Activation";
		}		
	}
	for(set<vector<vector<int> > >::iterator it = updatedEdge_3.begin(); it != updatedEdge_3.end(); it++)
	{    
		//inhibition
	    vector<vector<int> > edge=*it;
		if( (pid2 == edge[0][0] && type2==edge[0][1] && id2==edge[0][2] && pid1 == edge[1][0] && type1==edge[1][1] && id1==edge[1][2]) 
		|| (pid1 == edge[0][0] && type1==edge[0][1] && id1==edge[0][2] && pid2 ==edge[1][0] && type2==edge[1][1] && id2==edge[1][2]))
		{
		    return "Inhibition";
		}	 
	}
	return "Reaction";
}

int PathBubble1::findExpressionID(QString Name, vector<vector<QString>> geneInfo, int type)
{
	for(int i=0; i <  geneInfo.size(); i++)
	{
		if(type=='P')
		{
		   if(Name ==  geneInfo[i][1])
			return i;
		}
		else
		{
		   if(Name.contains(geneInfo[i][1]))
			return i;
		}
	}
	return -1;
}

void PathBubble1::spaceReplace(QString &S)
{
	  for(int i=0; i< S.size(); i++)
	  {
	     if(S[i] == ' ')
		 {
		   S.replace(i,1,"_");		 
		   if(i>15)
		   {
				   S.chop(S.size()-i);
				   break;
		   }		   
		 }	
		 if(i>20)
         {
			   S.chop(S.size()-i);
			   break;
	     }
	  }
}

void PathBubble1::savePathWay(QString filename,  vector<vector<int>> edge)
{   //save edge and node (not include position)//similar to VTK style
    QString str,tmpStr;
	QDomText text;
	QDomDocument doc;
	int count=0,Estart,Cstart,Pstart,Sstart,Dstart,Rstart;
	
	filename +=".graph";
	QFile file( filename );

	if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
		return;
	;
	str += "VERTICES ";			
	tmpStr.setNum( _scene->_physicalEntityNameD[dataID].size() 
		           +  _scene->_complexNameD[dataID].size() 
				   +  _scene->_proteinNameD[dataID].size()
				   +  _scene->_smallMoleculeNameD[dataID].size()
				   +  _scene->_DnaNameD[dataID].size()
				   +  _scene->_reactionNameD[dataID].size()
				   );
	str += tmpStr;
	str += "\n";
	
	Estart=count;
	for (int j = 0; j < _scene->_physicalEntityNameD[dataID].size(); j ++)
	{
		str += "physicalEntity: ";
		str += _scene->_physicalEntityNameD[dataID][j][0];
		str += "\n";
		count++;
	}
	Cstart=count;
	for (int j = 0; j < _scene->_complexNameD[dataID].size(); j ++)
	{
		str += "Complex: ";
		str += _scene->_complexNameD[dataID][j][0];
		str += "\n";
		count++;
	}		
	Pstart=count;
	for (int j = 0; j < _scene->_proteinNameD[dataID].size(); j ++)
	{
		str += "protein: ";
		str += _scene->_proteinNameD[dataID][j][0];
		str += "\n";
		count++;
	}
	Sstart=count;
	for (int j = 0; j < _scene->_smallMoleculeNameD[dataID].size(); j ++)
	{
		str += "smallMolecule: ";
		str += _scene->_smallMoleculeNameD[dataID][j][0];
		str += "\n";
		count++;
	}
	Dstart=count;
	for (int j = 0; j < _scene->_DnaNameD[dataID].size(); j ++)
	{
		str += "Dna: ";
		str += _scene->_DnaNameD[dataID][j][0];
		str += "\n";
		count++;
	}
	Rstart=count;
	for (int j = 0; j < _scene->_reactionNameD[dataID].size(); j ++)
	{
		str += "reaction: ";
		str += _scene->_reactionNameD[dataID][j][1];
		str += "\n";
		count++;
	}
	str += "EDGES ";	
	tmpStr.setNum( edge.size()*2 );
	str += tmpStr;
	str += "\n";
	for(int j=0; j<edge.size(); j++)
	{
		int start;
        switch(edge[j][0])
	    {
		     case 'E': start=Estart-1; break; 	
			 case 'C': start=Cstart-1; break;
			 case 'S': start=Sstart-1; break;			
			 case 'P': start=Pstart-1; break;
		     case 'D': start=Dstart-1; break;
			 case 'R': start=Rstart-1; break;			
		}
		tmpStr.setNum( edge[j][1] +start );
		str += tmpStr;
		str += " ";

		
		switch(edge[j][4])
	    {
		     case 'E': start=Estart-1; break; 	
			 case 'C': start=Cstart-1; break;
			 case 'S': start=Sstart-1; break;			
			 case 'P': start=Pstart-1; break;
		     case 'D': start=Dstart-1; break;
			 case 'R': start=Rstart-1; break;			
		}
		tmpStr.setNum( edge[j][5] +start );
		str += tmpStr;
		str += "\n";
		
		tmpStr.setNum( edge[j][5] +start );
		str += tmpStr;
		str += " ";

		if(edge[j][5] + start ==84 )
			j=j;
		
		switch(edge[j][2])
	    {
		     case 'E': start=Estart-1; break; 	
			 case 'C': start=Cstart-1; break;
			 case 'S': start=Sstart-1; break;			
			 case 'P': start=Pstart-1; break;
		     case 'D': start=Dstart-1; break;
			 case 'R': start=Rstart-1; break;			
		}
		tmpStr.setNum( edge[j][3] +start );
		str += tmpStr;
		str += "\n";	

			
	}	
	
	text = doc.createTextNode( str );
	QTextStream out(&file);
	text.save( out, 4 );
	file.close();	
}

QString PathBubble1::getFirstPathWayGraphName()
{   
	QString fileName,temp,base;
	
	QString _curFilePath = _curFileName;
	int tindex = _curFilePath.lastIndexOf("/");
	_curFilePath.remove(0, tindex+1);	

	base = "pathwayGraph/" + _curFilePath;
	if(drawSmallmolecule)
		base = base + "_S_";
	else base = base + "_";

	QString s = QString::number(m_pathwayID);
	base = base + s;
	base = base + s;
	fileName = base;
	return fileName;	
}

QString PathBubble1::createFeatureGeoJSON(char type, int id, QString name, QRectF rect ) /*const*/
{
  //"type":"Feature","properties":{"name":"xxx","geometry":{"type":"Point","coordinates":[142.2966,39.2875,45.45]}},

		/*<complex j="6">
				<Name>TGFBR1:FKBP1A</Name>
				<Position>(0.325415, 0.136322, 0.06, 0.0333333)</Position>
			</complex>
  */	
  QString fStr,tmpStr,str;
  fStr = "{\"type\":\"Feature\",";
  fStr += "\"id\":";  
  fStr +=  QString::number(id);  
  fStr += ",";
  fStr += "\"properties\":{";

  fStr += "\"type\":";
  fStr += "\"";
  fStr +=  type;
  fStr += "\"";
  fStr += ",";

  fStr += "\"name\":";
  fStr += "\"";
  fStr +=  name;
  fStr += "\"";
  fStr += "},";

  fStr += "\"geometry\":{";
  fStr += "\"type\":";
  fStr += "\"Polygon\"";
  fStr += ",";

  fStr += "\"coordinates\":";  
  str = "["; 
  str += "["; 
			tmpStr.setNum( (rect.x() -0.5) *180 );
			str += tmpStr;
			str += ", ";
			tmpStr.setNum( (- rect.y() +0.5) * 90  );
			str += tmpStr;
  str += "]";
			str += ", ";
  str += "["; 
			tmpStr.setNum( (rect.x() + rect.width() - 0.5) * 180  );
			str += tmpStr;
			str += ", ";
			tmpStr.setNum( (- rect.y() +0.5) * 90  );
			str += tmpStr;	
  str += "]";
          str += ", ";
   str += "["; 
			tmpStr.setNum( (rect.x() + rect.width() -0.5) *180 );
			str += tmpStr;
			str += ", ";
			tmpStr.setNum( (-rect.y() - rect.height() + 0.5) * 90  );
			str += tmpStr;
  str += "]";
			str += ", ";
  str += "["; 
			tmpStr.setNum( (rect.x() -0.5) *180 );
			str += tmpStr;
			str += ", ";
			tmpStr.setNum( (-rect.y() - rect.height() + 0.5) * 90  );
			str += tmpStr;
  str += "]";
  str += "]";
  fStr +=str;
  fStr += "}";
  fStr += "},";
  fStr += "\n"; 

  return fStr;
}


QString PathBubble1::createFeatureGeoJSON(int id, char stype, int sid, QPointF start, char etype, int eid, QPointF end) /*const*/
{
  QString fStr,tmpStr,str;
  fStr = "{\"type\":\"Feature\",";
  fStr += "\"id\":";  
  fStr +=  QString::number(id);  
  fStr += ",";
  fStr += "\"properties\":{";

  fStr += "\"stype\":";
  fStr += "\"";
  fStr +=  stype;
  fStr += "\"";
  fStr += ",";

  fStr += "\"sid\":";
  fStr += "\"";
  fStr +=  tmpStr.setNum(sid);
  fStr += "\"";
  fStr += ",";

   fStr += "\"etype\":";
  fStr += "\"";
  fStr +=  etype;
  fStr += "\"";
  fStr += ",";

  fStr += "\"eid\":";
  fStr += "\"";
  fStr +=  tmpStr.setNum(eid);
  fStr += "\"";
  fStr += "},";

  fStr += "\"geometry\":{";
  fStr += "\"type\":";
  fStr += "\"LineString\"";
  fStr += ",";

  fStr += "\"coordinates\":";  
  str = "["; 
  str += "["; 
			tmpStr.setNum( (start.x() -0.5) *180 );
			str += tmpStr;
			str += ", ";
			tmpStr.setNum( (- start.y() +0.5) * 90  );
			str += tmpStr;
  str += "]";
			str += ", ";
  str += "["; 
			tmpStr.setNum( (end.x() - 0.5) * 180  );
			str += tmpStr;
			str += ", ";
			tmpStr.setNum( (- end.y() +0.5) * 90  );
			str += tmpStr;	
  str += "]";
  str += "]";
  fStr +=str;
  fStr += "}";
  fStr += "},";
  fStr += "\n"; 

  return fStr;
}



void PathBubble1::savePathWayinJSFile(QString filename, vector<QRectF> complexPos, vector<QRectF> proteinPos, vector<QRectF> smallMoleculePos, vector<QRectF> DnaPos, vector<QRectF> reactionPos, vector<QRectF> physicalEntityPos,  vector<QRectF> ANodePos,  vector<vector<int>> edge)
{
	/*QString inforstr = "Save the current scene as the xml file ";
	inforstr.append( filename );
	inforstr.append( "\n" );
	logOperation( inforstr );*/

	//QString filename;
	filename += ".js";
	QFile file( filename );
	if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return;

	QString fcString;
	
	fcString = "eqfeed_callback(";
	fcString += "{\"type\":\"FeatureCollection\",";    
	//fcString += " \"bbox\": [ " + QString::number( rect->xMinimum(), 'f', 8 ).remove( QRegExp( "[0]{1,7}$" ) ) + ", " + QString::number( rect->yMinimum(), 'f', 8 ).remove( QRegExp( "[0]{1,7}$" ) ) + ", " + QString::number( rect->xMaximum(), 'f', 8 ).remove( QRegExp( "[0]{1,7}$" ) ) + ", " + QString::number( rect->yMaximum(), 'f', 8 ).remove( QRegExp( "[0]{1,7}$" ) ) + "],\n";   
	fcString += " \"features\": [";

			
	    //complexPos
		for (int j = 0; j < complexNum; j ++)
		{
			fcString += createFeatureGeoJSON ('C', j, _scene->_complexNameD[dataID][j][0], complexPos[j] );
		}			
		//protein	
		for (int j = 0; j < proteinNum; j ++)
		{
		   fcString += createFeatureGeoJSON ('P', j, _scene->_proteinNameD[dataID][j][0], proteinPos[j] );
		}	
        //physical Entity		

		for (int j = 0; j < physicalEntityNum; j ++)
		{
			fcString += createFeatureGeoJSON ('E', j, _scene-> _physicalEntityNameD[dataID][j][0], physicalEntityPos[j] );
        }

		//Aggregated Node
		for (int j = 0; j < ANodeNum; j ++)
		{
			fcString += createFeatureGeoJSON ('L', j, _scene-> _ANodeName[m_pathwayID][j][0], ANodePos[j] );
        }
		
	    //smallMolecule					
		for (int j = 0; j < smallMoleculeNum; j ++)
		{
			fcString += createFeatureGeoJSON ('S', j, _scene-> _smallMoleculeNameD[dataID][j][0], smallMoleculePos[j] );
        }		
				
		//Dna
		for (int j = 0; j < DnaNum; j ++)
		{
			fcString += createFeatureGeoJSON ('D', j, _scene-> _DnaNameD[dataID][j][0], DnaPos[j] );			
		}		
		////////////////
		//reaction	
		
		for (int j = 0; j < reactionNum; j ++)
		{
			fcString += createFeatureGeoJSON ('R', j, "", reactionPos[j]);		
		}			
	    //////////////////
		//edgeblock	        
		
		for(int i=0; i<edge.size(); i++)
	    {
		 int type, id;
		 QPointF start, middle, end;
         type=edge[i][0], id=edge[i][1];		 
		 switch(type)
	     {
		     case 'C': start=complexPos[id].center();         break;
			 case 'D': start=DnaPos[id].center();             break;
			 case 'E': start=physicalEntityPos[id].center();          break; 
			 case 'L': start=ANodePos[id].center();          break; 
			 case 'P': start=proteinPos[id].center();         break;
			 case 'S': start=smallMoleculePos[id].center();   break;			
		 }
		 type=edge[i][4];
		 id=edge[i][5];
		 if(type=='R') 
			 middle=reactionPos[id].center();
		 //else middle degeneration???

		 if(start.x()!=-1&& middle.x()!=-1)
		 fcString += createFeatureGeoJSON (i, edge[i][0], edge[i][1], start, edge[i][4], edge[i][5], middle);		

		 type=edge[i][2], id=edge[i][3];	
		 if(type=='R')
		     id=edge[i][5];
		 QRectF rect;
		 switch(type)
	     {
		     case 'C': end=complexPos[id].center();       rect= complexPos[id];  break;
			 case 'D': end=DnaPos[id].center();           rect= DnaPos[id];    break;
			 case 'E': end=physicalEntityPos[id].center();        rect= physicalEntityPos[id];    break; 
			 case 'L': end=ANodePos[id].center();         rect= ANodePos[id];    break; 
			 case 'P': end=proteinPos[id].center();       rect= proteinPos[id];    break;
			 case 'S': end=smallMoleculePos[id].center(); rect= smallMoleculePos[id];  break;			
			 case 'R': end=reactionPos[id].center();      rect= reactionPos[id];  break;  //catalysis for bio reaction	
		 }	
		 if(end.x()!=-1&& middle.x()!=-1)
			fcString += createFeatureGeoJSON (i, edge[i][4], edge[i][5], middle, edge[i][2], id, end);		

		}		
		fcString.chop(2);
		fcString +="]});";

        file.write((const char *)fcString.toAscii().data());
	    QTextStream out(&file);
	    
	    file.close();
	    //proteinPos = _proteinPos, proteinPos = _proteinPos, smallMoleculePos=_smallMoleculePos, DnaPos=_DnaPos, reactionPos=_reactionPos, physicalEntityPos=_physicalEntityPos, edge=_edge;
	    //XML
}

void PathBubble1::savePathWay(QString filename, vector<QRectF> complexPos, vector<QRectF> proteinPos, vector<QRectF> smallMoleculePos, vector<QRectF> DnaPos, vector<QRectF> reactionPos, vector<QRectF> physicalEntityPos, vector<QRectF> ANodePos, bool saveANode)
{
	/*QString inforstr = "Save the current scene as the xml file ";
	inforstr.append( filename );
	inforstr.append( "\n" );
	logOperation( inforstr );*/

	//QString filename;
	if(!filename.contains(".xml"))
	    filename +=".xml";
	QFile file( filename );

	if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
		return;

	QDomDocument doc;
	QDomProcessingInstruction instruction;

	instruction = doc.createProcessingInstruction("xml", " version=\"1.0\" encoding=\"UTF-8\" ");
	doc.appendChild( instruction );

	QDomElement root = doc.createElement( tr("Pathway") );
	doc.appendChild( root );

	//group
	//QList<Group*> gList = this->_manager->groupList();
	
	QDomElement iGroup, iBubble;
	QDomAttr gId, bId;

	QString str,tmpStr;
	QDomText text;
	
	//first record anode
	//then collapse then record others
	//Aggredated Node	
	iGroup = doc.createElement( tr("ANodeBlock") );
	gId = doc.createAttribute( tr("Num") );

	str.setNum( ANodeNum );
	gId.setValue( str );
	iGroup.setAttributeNode( gId );

	if(m_pathwayID < _scene->_ANodeName.size())
	for (int j = 0; j < _scene->_ANodeName[m_pathwayID].size() && j<ANodePos.size(); j ++)
	{
		iBubble = doc.createElement( tr("ANode") );
		bId = doc.createAttribute( tr("j") );

		str.setNum( j );
		bId.setValue( str );
		iBubble.setAttributeNode( bId );

		//////
		QDomElement parePos = doc.createElement( tr("Name") );
		QString name0;
		
		if(_scene-> _ANodeName[m_pathwayID][j].size()>=1)
			name0=_scene-> _ANodeName[m_pathwayID][j][0];
		else name0="";
	    QString cstr(name0); 
		
		text = doc.createTextNode( cstr );
		parePos.appendChild( text );
		iBubble.appendChild( parePos );

	   ////
		parePos = doc.createElement( tr("Position") );
		str = "(";
		tmpStr.setNum( ANodePos[j].x() );
		str += tmpStr;
		str += ", ";
		tmpStr.setNum( ANodePos[j].y()  );
		str += tmpStr;
			str += ", ";
		tmpStr.setNum( ANodePos[j].width()  );
		str += tmpStr;
			str += ", ";
		tmpStr.setNum( ANodePos[j].height()  );
		str += tmpStr;
		str += ")";
		text = doc.createTextNode( str );
		parePos.appendChild( text );
		iBubble.appendChild( parePos );	
	    ////

	    //////
		if(ANodePos[j].width()>0)
		{
			parePos = doc.createElement( tr("Contain") );
			str = "(";		
			for(int i=0; i<_scene->_ANodeContain[m_pathwayID][j].size(); i=i+2)
			{
				char type=_scene->_ANodeContain[m_pathwayID][j][i];
				int id=_scene->_ANodeContain[m_pathwayID][j][i+1];
				tmpStr = type;
				str += tmpStr;
				str += ", ";
				tmpStr.setNum( id );
				str += tmpStr;	
				int i1=i;
				i1++;
				if(i1!=_scene->_ANodeContain[m_pathwayID][j].size()-1)
					str += ", ";	
			}		
			str += ")";
			text = doc.createTextNode( str );
			parePos.appendChild( text );
			iBubble.appendChild( parePos );
		}
	    //////

	iGroup.appendChild( iBubble );			
	}		
	root.appendChild( iGroup );
	
	vector<QRectF> backUpAPos;
	vector<vector<vector<int>>> _sceneANodeContain;
	if(saveANode)
	   tempCollapseALL(backUpAPos, _sceneANodeContain); 
	//compartment
	iGroup = doc.createElement( tr("compartmentBlock") );
	gId = doc.createAttribute( tr("Num") );

	compartmentNum = compartmentNum<_scene->_compartmentName[m_pathwayID].size()?_scene->_compartmentName[m_pathwayID].size():compartmentNum;
	str.setNum( compartmentNum );
	gId.setValue( str );
	iGroup.setAttributeNode( gId );

	for (int j = 0; j < compartmentNum; j ++)
	{
		iBubble = doc.createElement( tr("compartment") );
		bId = doc.createAttribute( tr("j") );

		str.setNum( j );
		bId.setValue( str );
		iBubble.setAttributeNode( bId );

		
		//////
		QDomElement parePos = doc.createElement( tr("Name") );
		QString cstr(_scene->_compartmentName[m_pathwayID][j][0]); ///a bug when loading "2160456", "917937" "425397" "210744" "2644602"" pathway after opening certain path bubbles, solved by modifying "compartmentNum = compartmentNum<_scene->_compartmentName.size()?_scene->_compartmentName.size():compartmentNum;"
		
		text = doc.createTextNode( cstr );
		parePos.appendChild( text );
		iBubble.appendChild( parePos );
		////

		parePos = doc.createElement( tr("Position") );
		str = "(";
		tmpStr.setNum( compartmentPos[j].x() );
		str += tmpStr;
		str += ", ";
		tmpStr.setNum( compartmentPos[j].y()  );
		str += tmpStr;
			str += ", ";
		tmpStr.setNum( compartmentPos[j].width()  );
		str += tmpStr;
			str += ", ";
		tmpStr.setNum( compartmentPos[j].height()  );
		str += tmpStr;
		str += ")";
		text = doc.createTextNode( str );
		parePos.appendChild( text );
		iBubble.appendChild( parePos );
		//////

		parePos = doc.createElement( tr("Contain") );
		str = "(";		
		for(set<vector<int>>::iterator it=CompartmentContain[j].begin(); it!=CompartmentContain[j].end(); it++)
		{
			vector<int> node = *it;
			set<vector<int>>::iterator it1=it;
			char type=node[0];
			tmpStr = type;
			str += tmpStr;
			str += ", ";
			tmpStr.setNum( node[1] );
			str += tmpStr;	
			it1++;
			if(it1!=CompartmentContain[j].end())
				str += ", ";	
		}		
		str += ")";
		text = doc.createTextNode( str );
		parePos.appendChild( text );
		iBubble.appendChild( parePos );
		//////

		iGroup.appendChild( iBubble );			
	}		
	root.appendChild( iGroup );


//complex
	iGroup = doc.createElement( tr("complexBlock") );
	gId = doc.createAttribute( tr("Num") );

	str.setNum( complexNum );
	gId.setValue( str );
	iGroup.setAttributeNode( gId );

	for (int j = 0; j <_scene->_complexNameD[dataID].size(); j ++)//complexNum+1
	{
		iBubble = doc.createElement( tr("complex") );
		bId = doc.createAttribute( tr("j") );

		str.setNum( j );
		bId.setValue( str );
		iBubble.setAttributeNode( bId );

		//////
		QDomElement parePos = doc.createElement( tr("Name") );
		QString cstr(_scene->_complexNameD[dataID][j].size()>0?_scene->_complexNameD[dataID][j][0]:""); //keqin may has a bug here
		
	text = doc.createTextNode( cstr );
	parePos.appendChild( text );
	iBubble.appendChild( parePos );

	////

	parePos = doc.createElement( tr("Position") );
	str = "(";
	tmpStr.setNum( complexPos[j].x() );
	str += tmpStr;
	str += ", ";
	tmpStr.setNum( complexPos[j].y()  );
	str += tmpStr;
		str += ", ";
	tmpStr.setNum( complexPos[j].width()  );
	str += tmpStr;
		str += ", ";
	tmpStr.setNum( complexPos[j].height()  );
	str += tmpStr;
	str += ")";
	text = doc.createTextNode( str );
	parePos.appendChild( text );
	iBubble.appendChild( parePos );

	iGroup.appendChild( iBubble );			
	}		
	root.appendChild( iGroup );
	

//protein		
	iGroup = doc.createElement( tr("proteinBlock") );
	gId = doc.createAttribute( tr("Num") );

	str.setNum( proteinNum );
	gId.setValue( str );
	iGroup.setAttributeNode( gId );

	for (int j = 0; j < _scene->_proteinNameD[dataID].size(); j ++)
	{
		iBubble = doc.createElement( tr("protein") );
		bId = doc.createAttribute( tr("j") );

		str.setNum( j );
		bId.setValue( str );
		iBubble.setAttributeNode( bId );

		//////
			
		QDomElement parePos = doc.createElement( tr("Name") );
		QString cstr(_scene->_proteinNameD[dataID][j][0]); 
		
	text = doc.createTextNode( cstr );
	parePos.appendChild( text );
	iBubble.appendChild( parePos );

	////

	parePos = doc.createElement( tr("Position") );
	str = "(";
	tmpStr.setNum( proteinPos[j].x() );
	str += tmpStr;
	str += ", ";
	tmpStr.setNum( proteinPos[j].y()  );
	str += tmpStr;
		str += ", ";
	tmpStr.setNum( proteinPos[j].width()  );
	str += tmpStr;
		str += ", ";
	tmpStr.setNum( proteinPos[j].height()  );
	str += tmpStr;
	str += ")";
	text = doc.createTextNode( str );
	parePos.appendChild( text );
	iBubble.appendChild( parePos );

	iGroup.appendChild( iBubble );			
	}		
	root.appendChild( iGroup );

    
	//physical Entity		
	
	iGroup = doc.createElement( tr("physicalEntityBlock") );
	gId = doc.createAttribute( tr("Num") );

	str.setNum( physicalEntityNum );
	gId.setValue( str );
	iGroup.setAttributeNode( gId );

	for (int j = 0; j < _scene-> _physicalEntityNameD[dataID].size(); j ++)
	{
		iBubble = doc.createElement( tr("physicalEntity") );
		bId = doc.createAttribute( tr("j") );

		str.setNum( j );
		bId.setValue( str );
		iBubble.setAttributeNode( bId );

		//////
		QDomElement parePos = doc.createElement( tr("Name") );
		QString cstr(_scene-> _physicalEntityNameD[dataID][j][0]); 
		
	text = doc.createTextNode( cstr );
	parePos.appendChild( text );
	iBubble.appendChild( parePos );

	////

	parePos = doc.createElement( tr("Position") );
	str = "(";
	tmpStr.setNum( physicalEntityPos[j].x() );
	str += tmpStr;
	str += ", ";
	tmpStr.setNum( physicalEntityPos[j].y()  );
	str += tmpStr;
		str += ", ";
	tmpStr.setNum( physicalEntityPos[j].width()  );
	str += tmpStr;
		str += ", ";
	tmpStr.setNum( physicalEntityPos[j].height()  );
	str += tmpStr;
	str += ")";
	text = doc.createTextNode( str );
	parePos.appendChild( text );
	iBubble.appendChild( parePos );

	iGroup.appendChild( iBubble );			
	}		
	root.appendChild( iGroup );

	
//smallMolecule		
	
	iGroup = doc.createElement( tr("smallMoleculeBlock") );
	gId = doc.createAttribute( tr("Num") );

	str.setNum( smallMoleculeNum );
	gId.setValue( str );
	iGroup.setAttributeNode( gId );

	for (int j = 0; j < _scene-> _smallMoleculeNameD[dataID].size(); j ++)
	{
		iBubble = doc.createElement( tr("smallMolecule") );
		bId = doc.createAttribute( tr("j") );

		str.setNum( j );
		bId.setValue( str );
		iBubble.setAttributeNode( bId );

		//////
		QDomElement parePos = doc.createElement( tr("Name") );
		QString cstr(_scene-> _smallMoleculeNameD[dataID][j][0]); 
		
	text = doc.createTextNode( cstr );
	parePos.appendChild( text );
	iBubble.appendChild( parePos );

	////

	parePos = doc.createElement( tr("Position") );
	str = "(";
	tmpStr.setNum( smallMoleculePos[j].x() );
	str += tmpStr;
	str += ", ";
	tmpStr.setNum( smallMoleculePos[j].y()  );
	str += tmpStr;
		str += ", ";
	tmpStr.setNum( smallMoleculePos[j].width()  );
	str += tmpStr;
		str += ", ";
	tmpStr.setNum( smallMoleculePos[j].height()  );
	str += tmpStr;
	str += ")";
	text = doc.createTextNode( str );
	parePos.appendChild( text );
	iBubble.appendChild( parePos );

	iGroup.appendChild( iBubble );			
	}		
	root.appendChild( iGroup );

	

	//Dna
	
	iGroup = doc.createElement( tr("DnaBlock") );
	gId = doc.createAttribute( tr("Num") );

	str.setNum( DnaNum );
	gId.setValue( str );
	iGroup.setAttributeNode( gId );

	for (int j = 0; j < _scene-> _DnaNameD[dataID].size(); j ++)
	{
		iBubble = doc.createElement( tr("Dna") );
		bId = doc.createAttribute( tr("j") );

		str.setNum( j );
		bId.setValue( str );
		iBubble.setAttributeNode( bId );

		//////
		QDomElement parePos = doc.createElement( tr("Name") );
		QString cstr(_scene-> _DnaNameD[dataID][j][0]); 
		
	text = doc.createTextNode( cstr );
	parePos.appendChild( text );
	iBubble.appendChild( parePos );
	////

	parePos = doc.createElement( tr("Position") );
	str = "(";
	tmpStr.setNum( DnaPos[j].x() );
	str += tmpStr;
	str += ", ";
	tmpStr.setNum( DnaPos[j].y()  );
	str += tmpStr;
		str += ", ";
	tmpStr.setNum( DnaPos[j].width()  );
	str += tmpStr;
		str += ", ";
	tmpStr.setNum( DnaPos[j].height()  );
	str += tmpStr;
	str += ")";
	text = doc.createTextNode( str );
	parePos.appendChild( text );
	iBubble.appendChild( parePos );

	iGroup.appendChild( iBubble );			
	}		
	root.appendChild( iGroup );
		
	////////////////
	//reaction	
	iGroup = doc.createElement( tr("reactionBlock") );
	gId = doc.createAttribute( tr("Num") );

	str.setNum( reactionNum );
	gId.setValue( str );
	iGroup.setAttributeNode( gId );

	for (int j = 0; j < reactionPos.size(); j ++)
	{
		iBubble = doc.createElement( tr("reaction") );
		bId = doc.createAttribute( tr("j") );

		str.setNum( j );
		bId.setValue( str );
		iBubble.setAttributeNode( bId );

		//////
		QDomElement parePos = doc.createElement( tr("Name") );
		QString cstr(_scene-> _reactionNameD[dataID][j][1]); 
		
		text = doc.createTextNode( cstr );
		parePos.appendChild( text );
		iBubble.appendChild( parePos );

		////
		parePos = doc.createElement( tr("Position") );
		str = "(";
		tmpStr.setNum( reactionPos[j].x() );
		str += tmpStr;
		str += ", ";
		tmpStr.setNum( reactionPos[j].y()  );
		str += tmpStr;
		str += ", ";
		tmpStr.setNum( reactionPos[j].width()  );
		str += tmpStr;
		str += ", ";
		tmpStr.setNum( reactionPos[j].height()  );
		str += tmpStr;
		str += ")";
		text = doc.createTextNode( str );
		parePos.appendChild( text );
		iBubble.appendChild( parePos );
		iGroup.appendChild( iBubble );			
	}		
	root.appendChild( iGroup );

	//////////////////
	//edgeblock	
    iGroup = doc.createElement( tr("edgeBlock") );
	gId = doc.createAttribute( tr("Num") );
	str.setNum( edge.size() );
	gId.setValue( str );
	iGroup.setAttributeNode( gId );
		
	for(int j=0; j<edge.size(); j++)
	{
           
		iBubble = doc.createElement( tr("edge") );
		bId = doc.createAttribute( tr("j") );

		str.setNum( j );
		bId.setValue( str );
		iBubble.setAttributeNode( bId );

		//////
		QDomElement parePos = doc.createElement( tr("Name") );
		QString cstr(""); 
		
		text = doc.createTextNode( cstr );
		parePos.appendChild( text );
		iBubble.appendChild( parePos );

		parePos = doc.createElement( tr("Position") );
	str = "(";
	tmpStr.setNum( edge[j][0] );
	str += tmpStr;
	str += ", ";
	tmpStr.setNum( edge[j][1]  );
	str += tmpStr;
		str += ", ";
	tmpStr.setNum( edge[j][4] );
	str += tmpStr;
		str += ", ";
		tmpStr.setNum( edge[j][5] );
	str += tmpStr;
		str += ", ";
		tmpStr.setNum(edge[j][2] );
	str += tmpStr;
		str += ", ";
		tmpStr.setNum(edge[j][3] );
	str += tmpStr;
		str += ", ";
	tmpStr.setNum( edge[j][6] );
	str += tmpStr;
	str += ")";
	text = doc.createTextNode( str );
	parePos.appendChild( text );
	iBubble.appendChild( parePos );
	iGroup.appendChild( iBubble );	

	////		
	}		
	root.appendChild( iGroup );

/////////////////////////////
	QTextStream out(&file);
	doc.save( out, 4 );
	file.close();

	if(saveANode)
	    reAggregateAll(backUpAPos, _sceneANodeContain);
	//proteinPos = _proteinPos, proteinPos = _proteinPos, smallMoleculePos=_smallMoleculePos, DnaPos=_DnaPos, reactionPos=_reactionPos, physicalEntityPos=_physicalEntityPos, edge=_edge;
	//XML
}

bool PathBubble1::readPathWay(QString filename, vector<QRectF> &complexPos, vector<QRectF> &proteinPos, vector<QRectF> &smallMoleculePos, vector<QRectF> &DnaPos, vector<QRectF> &reactionPos, vector<QRectF> &physicalEntityPos, vector<QRectF> &ANodePos, vector<vector<int>> &edge)
{
	vector<QRectF> _complexPos, _proteinPos, _smallMoleculePos, _DnaPos, _reactionPos, _physicalEntityPos,  _ANodePos, _compartmentPos;
	vector<vector<int>> _edge;

	QFile file( filename );

	if ( !file.open(QIODevice::ReadOnly) )
	{
		return false;
	}
	
	QDomDocument doc;
	if ( !doc.setContent(&file) )
	{
		file.close();
		return false;
	}
	file.close();

	vector<QString> unit(2,"");
	vector<int> temp(7,0);
	
	
	vector<vector<QString>>  _compartmentName, _complexName, _physicalEntityName, _ANodeName, _proteinName, _smallMoleculeName, _DnaName, _reactionName;
	vector<set<vector<int>>> _compartmentContain;
	vector<vector<int>> _ANodeContain;

	QRectF rect(-1000,-1000,0,0);

	/*_complexPos.push_back(rect);
	_proteinPos.push_back(rect);	
    _smallMoleculePos.push_back(rect);
    _DnaPos.push_back(rect);
    _reactionPos.push_back(rect);
    _physicalEntityPos.push_back(rect);
	_ANodePos.push_back(rect);

	vector<int> cNode;
	_ANodeContain.push_back(cNode);


	_complexName.push_back(unit);
	_physicalEntityName.push_back(unit);
	_ANodeName.push_back(unit);
	_proteinName.push_back(unit);	
	_smallMoleculeName.push_back(unit);
	_DnaName.push_back(unit);
	_reactionName.push_back(unit);
	*/	
	
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
				//if(tagname=="compartmentBlock")
				//   _compartmentContain.resize(itemNum);			

				childElement = eachElement.firstChild().toElement();
			    itemNum=0;
				
				while ( !childElement.isNull() )
				{
					itemname = childElement.elementsByTagName("Name").at(0).toElement().text();	
					QDomNodeList itemNode = childElement.elementsByTagName( tr("Position") );
	                positionName = itemNode.at(0).toElement().text();

					QStringList posStr= positionName.split( "," );
	                QString str;
					if(posStr.size()<1)
						break;
					str = posStr.at(0); 
	                str.remove( 0, 1 );
	                str = str.simplified();
	                float x = str.toFloat();

					if(posStr.size()<2)
						break;
	                str = posStr.at(1);
	                str = str.simplified();
	                float y  = str.toFloat();

					if(posStr.size()<3)
						break;
					str = posStr.at(2);
	                str = str.simplified();
	                float w  = str.toFloat();

					if(posStr.size()<4)
						break;
					str = posStr.at(3);
	                str.chop(1);
	                str = str.simplified();
	                float h  = str.toFloat();

					QRectF nrect(x,y,w,h);

					if(tagname=="compartmentBlock")
					{
						unit[0]=itemname;
					    _compartmentName.push_back(unit);
						_compartmentPos.push_back(nrect);

						QDomNodeList containNode = childElement.elementsByTagName( tr("Contain") );
	                    QString containName = containNode.at(0).toElement().text();

						QStringList nodeStr= containName.split( "," );
						QString str;
						set<vector<int>> nodeset;
						_compartmentContain.push_back(nodeset);					
						vector<int> node(2,0);
						if(nodeStr.size()>1)
						for(int i=0; i<nodeStr.size(); i=i+2)
						{	
							str = nodeStr.at(i); 
							str.remove( 0, 1 );
							str = str.simplified();
							QByteArray ba = str.toAscii();
							char type =  ba[0];

							str = nodeStr.at(i+1); 
							str.remove( 0, 1 );
							str.remove(')');
							str = str.simplified();							
							int id =  str.toInt();
							node[0]=type, node[1]=id;
							_compartmentContain[_compartmentContain.size()-1].insert(node);
						}
					}
					if(tagname=="complexBlock")
					{
						unit[0]=itemname;
					    _complexName.push_back(unit);
						_complexPos.push_back(nrect);
					}
					else if(tagname=="physicalEntityBlock")
					{
						unit[0]=itemname;
					    _physicalEntityName.push_back(unit);	
						_physicalEntityPos.push_back(nrect);
					}
					else if(tagname=="ANodeBlock")
					{
						//itemname.resize(5);
						unit[0]=itemname;	
						if(unit.size()<5)
							unit.resize(5);
					
					    _ANodeName.push_back(unit);							
						_ANodePos.push_back(nrect);						

						QDomNodeList containNode = childElement.elementsByTagName( tr("Contain") );
	                    QString containName = containNode.at(0).toElement().text();

						QStringList nodeStr= containName.split( "," );
						QString str;
						vector<int> nodeset;
						_ANodeContain.push_back(nodeset);
						if(nodeStr.size()>1)
						for(int i=0; i<nodeStr.size(); i=i+2)
						{	
							str = nodeStr.at(i); 
							str.remove( 0, 1 );
							str = str.simplified();
							QByteArray ba = str.toAscii();
							char type =  ba[0];

							str = nodeStr.at(i+1); 
							str.remove( 0, 1 );
							str.remove(')');
							str = str.simplified();							
							int id =  str.toInt();							
							_ANodeContain[_ANodeContain.size()-1].push_back(type);
							_ANodeContain[_ANodeContain.size()-1].push_back(id);
						}

					}
					else if(tagname=="proteinBlock")
					{
						unit[0]=itemname;
					    _proteinName.push_back(unit);
						_proteinPos.push_back(nrect);	
					}
					else if(tagname=="smallMoleculeBlock")
					{
						unit[0]=itemname;
					    _smallMoleculeName.push_back(unit);
						_smallMoleculePos.push_back(nrect);
    				}
					else if(tagname=="DnaBlock")
					{
						unit[0]=itemname;
					    _DnaName.push_back(unit);
						_DnaPos.push_back(nrect);
    				}
					else if(tagname=="reactionBlock")
					{
						unit[1]=itemname;
					    _reactionName.push_back(unit);
						_reactionPos.push_back(nrect);
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
					QString str;

					if(posStr.size()<1)
						break;
	                str = posStr.at(0);
	                str.remove( 0, 1 );
	                str = str.simplified();
	                temp[0] = str.toInt();

					if(posStr.size()<2)
						break;
	                str = posStr.at(1);
	                str = str.simplified();
	                temp[1] =  str.toInt();

					if(posStr.size()<3)
						break;
					str = posStr.at(2);
	                str = str.simplified();
	                temp[4] = str.toInt();

					if(posStr.size()<4)
						break;
					str = posStr.at(3);
	                str = str.simplified();
	                temp[5] = str.toInt();

					if(posStr.size()<5)
						break;
					str = posStr.at(4);
	                str = str.simplified();
	                temp[2] = str.toInt();

					if(posStr.size()<6)
						break;
					str = posStr.at(5);
	                str = str.simplified();
	                temp[3] = str.toInt();

					if(posStr.size()<7)
						break;
					str = posStr.at(6);
	                str.chop(1);
	                str = str.simplified();
	                temp[6] = str.toInt();		
		         
					if(temp[0]!='L' && temp[2]!='L' && temp[4]!='L') //keqin tempoary comment out
		            _edge.push_back(temp); //keqin currently ignore edge
					childElement = childElement.nextSibling().toElement();
				}
			}
		}
		docNode = docNode.nextSibling();
	}	
	if(_compartmentPos.size()<Ranks.size())
	{
		for(int i=_compartmentPos.size(); i<Ranks.size(); i++)
		{
			_compartmentPos.push_back(QRectF(-1000,-1000,-1000,-1000));
		}
	}			
	for(int i=0; i<compartmentPos.size() && i<_compartmentPos.size(); i++)
	{
		if(compartmentPos[i].x()>=-990 && _compartmentPos[i].x()>=-990 && _scene->_compartmentName[m_pathwayID][i][0] ==_compartmentName[i][0])
		{
			compartmentPos[i]=_compartmentPos[i];			
			_scene->CompartmentContain[m_pathwayID][i]=CompartmentContain[i];			
			if(drawSmallmolecule)
			    CompartmentContain_1[i]=_compartmentContain[i];			
			else
				CompartmentContain_0[i]=_compartmentContain[i];			
		}
	}
	for(int i=0; i<complexPos.size() && i<_complexPos.size(); i++)
	{
		if(complexPos[i].x()>=-990 && _complexPos[i].x()>=-990 && _scene->_complexNameD[dataID][i][0] ==_complexName[i][0])
		   complexPos[i]=_complexPos[i];
	}
	for(int i=0; i<smallMoleculePos.size() && i<_smallMoleculePos.size(); i++)
	{
		if(smallMoleculePos[i].x()>=-990 && _smallMoleculePos[i].x()>=-990 && _scene->_smallMoleculeNameD[dataID][i][0] == _smallMoleculeName[i][0])
		   smallMoleculePos[i]=_smallMoleculePos[i];
	}
	for(int i=0; i<proteinPos.size() && i<_proteinPos.size(); i++)
	{
		if(proteinPos[i].x()>=-990 && _proteinPos[i].x()>=-990 && _scene->_proteinNameD[dataID][i][0] == _proteinName[i][0])
		   proteinPos[i]=_proteinPos[i];
	}
	for(int i=0; i<DnaPos.size() && i<_DnaPos.size(); i++)
	{
		if(DnaPos[i].x()>=-990 && _DnaPos[i].x()>=-990 && _scene->_reactionNameD[dataID][i][0] == _reactionName[i][0])
		   DnaPos[i]=_DnaPos[i];
	}
	for(int i=0; i<physicalEntityPos.size() && i<_physicalEntityPos.size(); i++)
	{
		if(physicalEntityPos[i].x()>=-990 && _physicalEntityPos[i].x()>=-990 && _scene->_physicalEntityNameD[dataID][i][0] == _physicalEntityName[i][0])
		   physicalEntityPos[i]=_physicalEntityPos[i];
	}
	
	int asize=ANodePos.size();
	if( asize < _ANodePos.size() )
	{
		ANodePos.resize(_ANodePos.size());
		//_scene->_ANodeName[m_pathwayID].resize(_ANodePos.size());
		_scene->_ANodeName[m_pathwayID]=_ANodeName;
		ANodePos_0.resize(_ANodePos.size());
		ANodePos_1.resize(_ANodePos.size());
		_scene->_ANodeContain[m_pathwayID].resize(_ANodePos.size());
	}
	for(int i=0; i<ANodePos.size() && i<_ANodePos.size() && i<_scene->_ANodeName[m_pathwayID].size(); i++)
	{
		if( (i >= asize || ANodePos[i].x()>=-990) && _ANodePos[i].x()>=-990 && ( i >= asize || _scene->_ANodeName[m_pathwayID][i][0] == _ANodeName[i][0]) )
		{
			ANodePos[i]=_ANodePos[i], _scene->_ANodeName[m_pathwayID][i] = _ANodeName[i];
			ANodePos_0[i]=ANodePos_1[i]=_ANodePos[i];
			_scene->_ANodeContain[m_pathwayID][i]=_ANodeContain[i];
		}
	}
	for(int i=0; i<reactionPos.size() && i<_reactionPos.size(); i++)
	{
		if(reactionPos[i].x()>=-990 && _reactionPos[i].x()>=-990 && _scene->_reactionNameD[dataID][i][1] == _reactionName[i][1])
		   reactionPos[i]=_reactionPos[i];
	}

	int esize = edge.size();
	if( esize < _edge.size() )
		edge.resize(_edge.size());
	for(int i=0; i<edge.size() && i<_edge.size(); i++)
	{
		edge[i] = _edge[i];
	}
	/*  keqin temporory comment out bug when reading apoptosis graph file
	vector<int> start(4,0), middle(4,0), end(4,0);
	vector<int> ed;
	_ANode.clear();
	_ANode.resize(ANodePos.size());
	for(int i=0; i<edge.size(); i++)
	{
		 start[0]=edge[i][0], start[1]=edge[i][1];			 		 
		 middle[0]=edge[i][4]; middle[1]=edge[i][5]; 		 
		 end[0]=edge[i][2], end[1]=edge[i][3]; 
		 if(end[0]!='R' || drawSmallmolecule)		 
	     {
			 ed=middle;  ed[2]='M'; ed[3]=1; 
			 if(start[0]=='L')
			 {			 
				 _ANode[start[1]].push_back(ed);   
		     }			
			 
			 ed=middle;  ed[2]='A';  ed[3]=-1; 
			 if(end[0]=='L')
	         {
				_ANode[end[1]].push_back(ed);          		
		     }
		 }
		 else 
		 {
			 ed=end;  ed[2]='R'; ed[3]=1; 
			 if(start[0]=='L')
			 {
		         _ANode[start[1]].push_back(ed);   
		     }			 
		 }
	}*/
	//initANodeContains;
	for(int i=0; i<_ANodeContain.size(); i++)
	{
		set<vector<int>> highlightedT;
		for(int j=0; j<_ANodeContain[i].size(); j=j+2)
		{
			int type=_ANodeContain[i][j], id=_ANodeContain[i][j+1];		
		    vector<int>	item(2,0);
			item[0]=type, item[1]=id;
			highlightedT.insert(item);
		}
		//aggregation(highlightedT);
		initANodeContains.push_back(highlightedT);
	}
	return true;
}

void PathBubble1::redrawPathWay_1(vector<vector<int>> _Catalysis, vector<vector<int>> _5Control)
{		
	bool record=drawSmallmolecule;
	drawSmallmolecule=true;
	bool preComputed=true;
	
	vector<QRectF> reactionPos1 = reactionPos;					
	stepTracked.clear();			
	_scene->CompartmentContain[m_pathwayID]=CompartmentContain;	
	// _pathName=="Urea cycle" || _pathName=="Acetylcholine Binding And Downstream Events" || _pathName== "Acetylation" ||
	if( !pathWayPoiRead( complexPos_1, proteinPos_1, smallMoleculePos_1, DnaPos_1, EmptyPos_1, reactionPos_1, physicalEntityPos_1, compartmentPos_1, ANodePos_1, CompartmentContain_1, _scale, dCenter, true))
	{
		preComputed=false;
		preLocateNodes(complexPos,proteinPos,smallMoleculePos);		
		ParserTraceSteps();
		updateVisibleNodes();	
		
		HierarchicalLayout(_whole, reactionPos1);	
		RemoveDisabledNodes(_whole, _VertexInfo, _Vertex, Rank0, Ranks);	
		HierarchicalTreeNodeLocation(_whole, _VertexInfo, Rank0, Ranks);
		CompartmentContain_1=_scene->CompartmentContain[m_pathwayID];
		complexPos_1=complexPos, proteinPos_1=proteinPos, smallMoleculePos_1=smallMoleculePos, DnaPos_1=DnaPos, reactionPos_1=reactionPos, degradationPos_1=degradationPos, physicalEntityPos_1=physicalEntityPos, ANodePos_1 = ANodePos, compartmentPos_1=compartmentPos, EmptyPos_1=EmptyPos; 
		
		//_ScaleBackup_S=_scale;
	}		
	removeNotContainedinCompartment(CompartmentContain_1, compartmentPos_1, complexPos_1, proteinPos_1, smallMoleculePos_1, DnaPos_1, reactionPos_1, degradationPos_1, physicalEntityPos_1, EmptyPos_1, ANodePos_1);						
	drawSmallmolecule=record;	
	_ScaleBackup_S=_scale;
}

int PathBubble1::findCompartment(int type, int id, QRectF node, vector<QRectF> compartmentPos, int j, float scale1, float scale2)	
{
	int cid=-1;
	bool flag=false;
	if(j>=0 && j<compartmentPos.size())
	{
	   if(compartmentPos[j].x()>-0.99)
		{
			if(compartmentPos[j].intersects(node))
			{
				cid=j;
				flag=true;
			}
	   }
	}
	if(!flag)
	for(int i=0; i<compartmentPos.size(); i++)
	{
		if(compartmentPos[i].x()>-0.99)
		{
			if(compartmentPos[i].intersects(node))
			{
				cid=i;
				break;
			}
		}
	}
	if(cid<0 && scale1>1) //try again
	{
		flag=false;

		if(scale1>1)
		if(j>=0 && j<compartmentPos.size())
		{
		   if(compartmentPos[j].x()>-0.99)
			{
				QRectF rect=compartmentPos[j];
				float x = rect.center().x(), y = rect.center().y();
				float w = rect.width()/2, h = rect.height()/2;
				w=w*scale1; h=h*scale1;
				rect = QRectF(x-w, y-h, w*2, h*2);
				if(rect.intersects(node))
				{
					cid=j;
					flag=true;
				}
		   }
		}
		if(!flag && scale2>1)
		for(int i=0; i<compartmentPos.size(); i++)
		{
			if(compartmentPos[i].x()>-0.99)
			{
				QRectF rect=compartmentPos[i];
				float x = rect.center().x(), y = rect.center().y();
				float w = rect.width()/2, h = rect.height()/2;
				w=w*scale2; h=h*scale2;
				rect = QRectF(x-w, y-h, w*2, h*2);
				if(rect.intersects(node))				
				{
					cid=i;
					break;
				}
			}
		}	
	}
	return cid;
}

void PathBubble1::removeNotContainedinCompartment(vector<set<vector<int>>> &CompartmentContain, vector<QRectF> compartmentPos, vector<QRectF> &complexPos, vector<QRectF> &proteinPos, vector<QRectF> &smallMoleculePos, vector<QRectF> &DnaPos, vector<QRectF> &reactionPos, vector<QRectF> &degradationPos, vector<QRectF> &physicalEntityPos, vector<QRectF> &EmptyPos, vector<QRectF> &ANodePos)
{
	vector<int> temp(2, 0);
	
		/*temp[0]='C';	
		for(int i=0; i<complexPos.size(); i++)
		{
			temp[1]=i;	
			bool flag=false;
			if(complexPos[i].x()>-0.99)
			{
				for(int j=0; j<CompartmentContain.size(); j++)
				{
					if(compartmentPos[j].x()>-0.99)
					if(CompartmentContain[j].find(temp)!=CompartmentContain[j].end())
					{
						//complexPos[i].setWidth(-1000);
						flag=true; break;
					}
				}
				if(!flag)
				{
				    int id = findCompartment(temp[0], temp[1], complexPos[i], compartmentPos);	
					if(id>=0)
					{
					   CompartmentContain[id].insert(temp);
					}
					else	
						complexPos[i].setX(-1000);
				}
			}
		}	*/
		temp[0]='R';
		for(int i=0; i<reactionPos.size(); i++)
		{
			temp[1]=i;	
			bool flag=false;
			if(i==31)
				i=i;
			if(reactionPos[i].x()>-0.99)
			{
				for(int j=0; j<CompartmentContain.size(); j++)
				{
					if(compartmentPos[j].x()>-0.99)
					if(CompartmentContain[j].find(temp)!=CompartmentContain[j].end())
					{
						int id = findCompartment(temp[0], temp[1], reactionPos[i], compartmentPos, j);	
						if(id!=j && id>=0&& id<CompartmentContain.size())
						{
						   CompartmentContain[id].insert(temp);
						   CompartmentContain[j].erase(temp);
						}
						else if (id<0)
						{
							int id = findCompartment(temp[0], temp[1], reactionPos[i], compartmentPos, j, 1.5, 1.2);	
							if(id!=j && id>=0 && id<CompartmentContain.size())
							{
							   CompartmentContain[id].insert(temp);
							   CompartmentContain[j].erase(temp);
							}
						}
						flag=true; break;
					}
				}
				if(!flag)
				{
				    int id = findCompartment(temp[0], temp[1], reactionPos[i], compartmentPos, -1);	
					if(id>=0 && id<CompartmentContain.size())
					{
					   CompartmentContain[id].insert(temp);
					}					
				}
			}
		}
		temp[0]='S';
		for(int i=0; i<smallMoleculePos.size(); i++)
		{
			temp[1]=i;	
			bool flag=false;
			int cid;
			if(smallMoleculePos[i].x()>-0.99)
			{
				for(int j=0; j<CompartmentContain.size(); j++)
				{
					if(compartmentPos[j].x()>-0.99)
					if(CompartmentContain[j].find(temp)!=CompartmentContain[j].end())
					{
						int id = findCompartment(temp[0], temp[1], smallMoleculePos[i], compartmentPos, j);	
						if(id!=j && id>=0 && id<CompartmentContain.size())
						{

						   CompartmentContain[id].insert(temp);
						   CompartmentContain[j].erase(temp);
						}
						cid=j;
						//smallMoleculePos[i].setWidth(-1000); 					
						flag=true; 
						break;
					}
				}
				if(!flag)
				{
				    int id = findCompartment(temp[0], temp[1], smallMoleculePos[i], compartmentPos, cid);	
					if(id>=0 && id<CompartmentContain.size())
					{
					   CompartmentContain[id].insert(temp);
					}
					else	
						smallMoleculePos[i].setX(-1000);
				}
			}
		}
	
		temp[0]='D';
		for(int i=0; i<DnaPos.size(); i++)
		{
			temp[1]=i;
			bool flag=false;
			int cid;
			if(DnaPos[i].x()>-0.99)
			{
				for(int j=0; j<CompartmentContain.size(); j++)
				{
					if(compartmentPos[j].x()>-0.99)
					if(CompartmentContain[j].find(temp)!=CompartmentContain[j].end())
					{
						int id = findCompartment(temp[0], temp[1], DnaPos[i], compartmentPos, j);	
						if(id!=j && id>=0 && id<CompartmentContain.size())
						{
						 	  CompartmentContain[id].insert(temp);
						      CompartmentContain[j].erase(temp);						 
						}
						
					    cid=j;
						flag=true; break;
					}
				}
				if(!flag)
				{
				     int id = findCompartment(temp[0], temp[1], DnaPos[i], compartmentPos, cid);	
					if(id>=0 && id<CompartmentContain.size())
					{
					   CompartmentContain[id].insert(temp);
					}
					else	
						DnaPos[i].setX(-1000);
				}
			}
		}

		/*temp[0]='E';
		for(int i=0; i<physicalEntityPos.size(); i++)
		{
			temp[1]=i;	
			bool flag=false;
			if(physicalEntityPos[i].x()>-0.99)
			{
				for(int j=0; j<CompartmentContain.size(); j++)
				{
					if(compartmentPos[j].x()>-0.99)
					if(CompartmentContain[j].find(temp)!=CompartmentContain[j].end())
					{
						//physicalEntityPos[i].setWidth(-1000);
					
						flag=true; break;
					}	
				}
				if(!flag)
				{
				     int id = findCompartment(temp[0], temp[1], physicalEntityPos[i], compartmentPos);	
					if(id>=0)
					{
					   CompartmentContain[id].insert(temp);
					}
					else	
						physicalEntityPos[i].setX(-1000);
				}
			}
		}	
		temp[0]='P';
		for(int i=0; i<proteinPos.size(); i++)
		{
			temp[1]=i;
			bool flag=false;
			if(proteinPos[i].x()>-0.99)
			{
				for(int j=0; j<CompartmentContain.size(); j++)
				{
					if(compartmentPos[j].x()>-0.99)
					if(CompartmentContain[j].find(temp)!=CompartmentContain[j].end())
					{
						//proteinPos[i].setWidth(-1000);
						flag=true; break;
					}
				}
				if(!flag)
				{
				     int id = findCompartment(temp[0], temp[1], proteinPos[i], compartmentPos);	
					if(id>=0)
					{
					   CompartmentContain[i].insert(temp);
					}
					else	
						proteinPos[i].setX(-1000);
				}
			}
		}
		temp[0]='L';
		for(int i=0; i<ANodePos.size(); i++)
		{
			temp[1]=i;	
			bool flag=false;
			if(ANodePos[i].x()>-0.99)
			{
				for(int j=0; j<CompartmentContain.size(); j++)
				{
					if(compartmentPos[j].x()>-0.99)
					if(CompartmentContain[j].find(temp)!=CompartmentContain[j].end())
					{
						//ANodePos[i].setWidth(-1000);
						flag=true; 
						break;
					}  
				}
				if(!flag)
				{
				     int id = findCompartment(temp[0], temp[1], ANodePos[i], compartmentPos);	
					if(id>=0)
					{
					   CompartmentContain[i].insert(temp);
					}
					else 
						ANodePos[i].setX(-1000);
				}
			}
		}	*/					
}

void PathBubble1::redrawPathWay()
{	
	if(drawSmallmolecule)
	{
		complexPos=complexPos_1, proteinPos=proteinPos_1, smallMoleculePos=smallMoleculePos_1, DnaPos=DnaPos_1, reactionPos=reactionPos_1, degradationPos=degradationPos_1, physicalEntityPos=physicalEntityPos_1, ANodePos=ANodePos_1, EmptyPos=EmptyPos_1, compartmentPos=compartmentPos_1; 
		CompartmentContain=CompartmentContain_1;				_scene->CompartmentContain[m_pathwayID]=CompartmentContain_1;
	}
	else
	{
	   complexPos=complexPos_0, proteinPos=proteinPos_0, smallMoleculePos=smallMoleculePos_0, DnaPos=DnaPos_0, reactionPos=reactionPos_0, degradationPos=degradationPos_0, physicalEntityPos=physicalEntityPos_0, ANodePos=ANodePos_0, EmptyPos=EmptyPos_0, compartmentPos=compartmentPos_0; 
	   CompartmentContain=CompartmentContain_0;				_scene->CompartmentContain[m_pathwayID]=CompartmentContain_0;
	   
	}
	_scene->_complexPos[m_pathwayID]=complexPos;
	_scene->_proteinPos[m_pathwayID]=proteinPos;
	_scene->_smallMoleculePos[m_pathwayID]=smallMoleculePos;

	_scene->_physicalEntityPos[m_pathwayID]=physicalEntityPos;
	_scene->_ANodePos[m_pathwayID]=ANodePos;

	_scene->_DnaPos[m_pathwayID]=DnaPos;
	_scene->_reactionPos[m_pathwayID]=reactionPos;
	_scene->_compartmentPos[m_pathwayID]=compartmentPos; 
	
}


void PathBubble1::redrawPathWay(bool drawSmallmolecule) //check if the node is in
{	
	if(drawSmallmolecule)
	{
		for(int i=0; i<complexPos_1.size(); i++)
		{
		    if(complexPos[i].x()>-999.99)
				complexPos[i]=complexPos_1[i];
		}

		for(int i=0; i<proteinPos_1.size(); i++)
		{
		    if(proteinPos[i].x()>-999.99)
				proteinPos[i]=proteinPos_1[i];
		}
		
		smallMoleculePos=smallMoleculePos_1;

		for(int i=0; i< DnaPos_1.size(); i++)
		{
		    if( DnaPos[i].x()>-999.99)
				 DnaPos[i]= DnaPos_1[i];
		}

		for(int i=0; i<reactionPos_1.size(); i++)
		{
		    if(reactionPos[i].x()>-999.99)
				reactionPos[i]=reactionPos_1[i];
		}

		for(int i=0; i<degradationPos_1.size(); i++)
		{
		    if(degradationPos[i].x()>-999.99)
				degradationPos[i]=degradationPos_1[i];
		}

		for(int i=0; i<physicalEntityPos_1.size(); i++)
		{
		    if(physicalEntityPos[i].x()>-999.99)
				physicalEntityPos[i]=physicalEntityPos_1[i];
		}

		for(int i=0; i<ANodePos_1.size(); i++)
		{
		    if(ANodePos[i].x()>-999.99)
				ANodePos[i]=ANodePos_1[i];
		}

		for(int i=0; i<EmptyPos_1.size(); i++)
		{
		    if(EmptyPos[i].x()>-999.99)
				EmptyPos[i]=EmptyPos_1[i];
		}

		for(int i=0; i<compartmentPos_1.size(); i++)
		{
		    if(compartmentPos[i].x()>-999.99 || (compartmentPos_0[i].x()<=-999.99 && compartmentPos_1[i].x()>-999.99) )
				compartmentPos[i]=compartmentPos_1[i];
		}		
		//compartmentPos=compartmentPos_1;
		//complexPos=complexPos_1, proteinPos=proteinPos_1, smallMoleculePos=smallMoleculePos_1, DnaPos=DnaPos_1, reactionPos=reactionPos_1, degradationPos=degradationPos_1, physicalEntityPos=physicalEntityPos_1, ANodePos=ANodePos_1, EmptyPos=EmptyPos_1, compartmentPos=compartmentPos_1; 
		CompartmentContain=CompartmentContain_1;	_scene->CompartmentContain[m_pathwayID]=CompartmentContain_1;
	}
	else
	{
	    
		for(int i=0; i<complexPos_0.size(); i++)
		{
		    if(complexPos[i].x()>-999.99)
				complexPos[i]=complexPos_0[i];
		}

		for(int i=0; i<proteinPos_0.size(); i++)
		{
		    if(proteinPos[i].x()>-999.99)
				proteinPos[i]=proteinPos_0[i];
		}

		/*for(int i=0; i<smallMoleculePos_0.size(); i++) //keqin: get back here later 5/30
		{
		    if(smallMoleculePos[i].x()>-999.99)
				smallMoleculePos[i]=smallMoleculePos_0[i];
		}*/

		smallMoleculePos=smallMoleculePos_0;

		for(int i=0; i< DnaPos_0.size(); i++)
		{
		    if( DnaPos[i].x()>-999.99)
				 DnaPos[i]= DnaPos_0[i];
		}

		for(int i=0; i<reactionPos_0.size(); i++)
		{
		    if(reactionPos[i].x()>-999.99)
				reactionPos[i]=reactionPos_0[i];
		}

		for(int i=0; i<degradationPos_0.size(); i++)
		{
		    if(degradationPos[i].x()>-999.99)
				degradationPos[i]=degradationPos_0[i];
		}

		for(int i=0; i<physicalEntityPos_0.size(); i++)
		{
		    if(physicalEntityPos[i].x()>-999.99)
				physicalEntityPos[i]=physicalEntityPos_0[i];
		}

		for(int i=0; i<ANodePos_0.size(); i++)
		{
		    if(ANodePos[i].x()>-999.99)
				ANodePos[i]=ANodePos_0[i];
		}

		for(int i=0; i<EmptyPos_0.size(); i++)
		{
		    if(EmptyPos[i].x()>-999.99)
				EmptyPos[i]=EmptyPos_0[i];
		}

		for(int i=0; i<compartmentPos_0.size(); i++)
		{
		    if(compartmentPos[i].x()>-999.99 || (compartmentPos_1[i].x()<=-999.99 && compartmentPos_0[i].x()>-999.99) )
				compartmentPos[i]=compartmentPos_0[i];
		}
		//compartmentPos=compartmentPos_0;

		//complexPos=complexPos_0, proteinPos=proteinPos_0, smallMoleculePos=smallMoleculePos_0, DnaPos=DnaPos_0, reactionPos=reactionPos_0, degradationPos=degradationPos_0, physicalEntityPos=physicalEntityPos_0, ANodePos=ANodePos_0, EmptyPos=EmptyPos_0, compartmentPos=compartmentPos_0; 
	    CompartmentContain=CompartmentContain_0;				_scene->CompartmentContain[m_pathwayID]=CompartmentContain_0;
	   
	}
	_scene->_complexPos[m_pathwayID]=complexPos;
	_scene->_proteinPos[m_pathwayID]=proteinPos;
	_scene->_smallMoleculePos[m_pathwayID]=smallMoleculePos;

	_scene->_physicalEntityPos[m_pathwayID]=physicalEntityPos;
	_scene->_ANodePos[m_pathwayID]=ANodePos;

	_scene->_DnaPos[m_pathwayID]=DnaPos;
	_scene->_reactionPos[m_pathwayID]=reactionPos;
	_scene->_compartmentPos[m_pathwayID]=compartmentPos; 
	
}