#include "textBubble2.h"
#include "ReactionBubble3.h"
#include "PathBubble2.h"
#include "GroupBubble4.h"
#include "PathBubble1.h"
#include <QTextDocument>
#include <QMap>

PathBubble1::PathBubble1( int size, int x, int y, OpenGLScene* Scene, StatisticManager* manager, OpenManager* open, QString name)
: PlotBase (size, x, y, Scene, manager)
{
	_scene= Scene;	
	_LABEL = false;
	_scale=1.0;
	
	static int pathwayID=-1;
	pathwayID++;
	pathwayID=pathwayID%3;
	m_pathwayID=pathwayID;

	vector<QString> temp6; 
	_scene->_6complexName.push_back(temp6);
	vector<vector<QString>> temp8;  
	_scene->_8physicalEntityName.push_back(temp8);
	vector<vector<QString>> temp7;  
	_scene->_7proteinName.push_back(temp7);	
	_scene->_4reactionName.push_back(temp7);
	_scene->_4degradationName.push_back(temp7);

	vector<vector<QString>> temp9,temp10; 
	_scene->_9smallMoleculeName.push_back(temp9);  
	_scene->_9DnaName.push_back(temp10);
     _scene->_4EmptyName.push_back(temp10);

	vector<vector<vector<int>>> temp4;  
	_scene->_4biochemicalReaction.push_back(temp4);
	_scene->_4degradation.push_back(temp4);

	

	vector<set<vector<int>>> tempx;
	_scene->CompartmentContain.push_back(tempx);
	_scene->CompartmentName.push_back(temp6);




	

	if(m_pathwayID==0)
		this->_name = "TGF";
	else if(m_pathwayID==1) 
	    this->_name = "MitoticG2-G2";
	else if(m_pathwayID==2)
	    this->_name = "BMP";

	loadFile(_name); 

	if(m_pathwayID==0)
	{			
        stepNum=read2pathwayStepOrder("pathways/TGF2pathwayStepOrder.txt", stepNum, _2pathwayStep);
		read3pathwayStep_reactionAndcontrol("pathways/TGF3pathwayStepReactionControl.txt", stepNum, _3pathwayStepCatalysis, _3pathwayStepReaction);    	
		vector<int> temp = read4biochemicalReaction("pathways/TGF4biochemicalReaction.txt", _scene->_4biochemicalReaction[pathwayID], _scene->_4reactionName[pathwayID], _scene->_4degradation[pathwayID], _scene->_4degradationName[pathwayID], _scene->_4EmptyName[pathwayID]);
		reactionNum = temp[0], smallMoleculeNum=temp[1], DnaNum=temp[2];
		manuLocateCompartment(compartmentPos);	
		read5catalysisANDcontrol("pathways/TGF5catalysisControl.txt", _5Catalysis, _5Control);
		complexNum = read6complex("pathways/TGF6complex.txt", _scene->_6complexName[pathwayID], _6complexContain);
		proteinNum = read7protein("pathways/TGF7protein.txt", _scene->_7proteinName[pathwayID]);
		read8convertedEntity("pathways/TGF8convertedEntity.txt", proteinNum, complexNum,physicalEntityNum, _8convertedProtein, _8convertedComplex, _8convertedEntity);
		read9smallEntity("pathways/TGF9smallEntity.txt",  _scene->_9smallMoleculeName[pathwayID], _scene->_9DnaName[pathwayID]);			
		
	}
	else if(m_pathwayID==1)
	{
		stepNum=read1pathway("pathways/BMP1pathway.txt", _1pathway); //1pathway.txt 	
		stepNum=read2pathwayStepOrder("pathways/BMP2pathwayStepOrder.txt", stepNum, _2pathwayStep);
 		read3pathwayStep_reactionAndcontrol("pathways/BMP3pathwayStepReactionControl.txt", stepNum, _3pathwayStepCatalysis, _3pathwayStepReaction);    	
		vector<int> temp = read4biochemicalReaction("pathways/BMP4biochemicalReaction.txt", _scene->_4biochemicalReaction[pathwayID], _scene->_4reactionName[pathwayID], _scene->_4degradation[pathwayID], _scene->_4degradationName[pathwayID], _scene->_4EmptyName[pathwayID]);
		reactionNum = temp[0], smallMoleculeNum=temp[1], DnaNum=temp[2];
		manuLocateCompartment(compartmentPos);	
		read5catalysisANDcontrol("pathways/BMP5catalysisControl.txt", _5Catalysis, _5Control);
		complexNum = read6complex("pathways/BMP6complex.txt", _scene->_6complexName[pathwayID], _6complexContain);
		proteinNum = read7protein("pathways/BMP7protein.txt", _scene->_7proteinName[pathwayID]);
		read8convertedEntity("pathways/BMP8convertedEntity.txt", proteinNum, complexNum, physicalEntityNum, _8convertedProtein, _8convertedComplex, _8convertedEntity);
		read9smallEntity("pathways/BMP9smallEntity.txt",  _scene->_9smallMoleculeName[pathwayID], _scene->_9DnaName[pathwayID]);  stepNum=read1pathway("pathways/TGF1pathway.txt", _1pathway); //1pathway.txt 	  	 			
	}
	else if(m_pathwayID==2)
	{	
		stepNum=read1pathway("pathways/MitoticG2-G21pathway.txt", _1pathway); //1pathway.txt 	
		stepNum=read2pathwayStepOrder("pathways/MitoticG2-G22pathwayStepOrder.txt", stepNum, _2pathwayStep);
		read3pathwayStep_reactionAndcontrol("pathways/MitoticG2-G23pathwayStepReactionControl.txt", stepNum, _3pathwayStepCatalysis, _3pathwayStepReaction);    	
		vector<int> temp = read4biochemicalReaction("pathways/MitoticG2-G24biochemicalReaction.txt", _scene->_4biochemicalReaction[pathwayID], _scene->_4reactionName[pathwayID], _scene->_4degradation[pathwayID], _scene->_4degradationName[pathwayID], _scene->_4EmptyName[pathwayID]);
		reactionNum = temp[0], physicalEntityNum=temp[1], smallMoleculeNum=temp[2], DnaNum=temp[3];
		manuLocateCompartment(compartmentPos);	
		read5catalysisANDcontrol("pathways/MitoticG2-G25catalysisControl.txt", _5Catalysis, _5Control);
		complexNum = read6complex("pathways/MitoticG2-G26complex.txt", _scene->_6complexName[pathwayID], _6complexContain);
		proteinNum = read7protein("pathways/MitoticG2-G27protein.txt", _scene->_7proteinName[pathwayID]);
		read8convertedEntity("pathways/MitoticG2-G28convertedEntity.txt", proteinNum, complexNum,physicalEntityNum, _8convertedProtein, _8convertedComplex, _8convertedEntity);		
		read9smallEntity("pathways/MitoticG2-G29smallEntity.txt",  _scene->_9smallMoleculeName[pathwayID], _scene->_9DnaName[pathwayID]);						
	}	
	
	multiplyDna();
	multiplySmallMolecule();


	smallMoleculeNum = _scene->_9smallMoleculeName[pathwayID].size()==0?0:_scene->_9smallMoleculeName[pathwayID].size()-1; 
	DnaNum = _scene->_9DnaName[pathwayID].size()==0?0:_scene->_9DnaName[pathwayID].size()-1;
	getNameforConverted(_8convertedProtein, _8convertedComplex, _8convertedEntity, _scene->_6complexName[pathwayID], _scene->_7proteinName[pathwayID], _scene->_8physicalEntityName[pathwayID]);
    EmptyNum=_scene->_4EmptyName[pathwayID].size();
	
	QRectF rect(-1,-1,0,0);
	vector<QRectF> temp1(complexNum+1, rect);
	complexPos=temp1;

	vector<QRectF> temp2(proteinNum+1, rect);
	proteinPos=temp2;

	vector<QRectF> temp3(smallMoleculeNum+1, rect);
	smallMoleculePos=temp3;

	vector<QRectF> temp44(DnaNum+1, rect);
	DnaPos=temp44;

	vector<QRectF> temp55(EmptyNum+1, rect);
	EmptyPos=temp55;

	vector<QRectF> temp5(reactionNum+1, rect);
	reactionPos=temp5;

	vector<QRectF> temp66(physicalEntityNum+1, rect);
	physicalEntityPos=temp66;

	//manuLocateNodes(complexPos,proteinPos,smallMoleculePos);
	preLocateNodes(complexPos,proteinPos,smallMoleculePos);
	
    for(int i=1; i<_2pathwayStep.size();i++)
	{
		if(i>=80)
			i=i;
		if(stepTracked.find(i)==stepTracked.end())
	      trackstep(i, _2pathwayStep, _3pathwayStepCatalysis, _3pathwayStepReaction, _scene->_4biochemicalReaction[pathwayID]);
	}	

	vector<vector<int>> empty;
	vector<vector<vector<int>>> temp11(proteinNum+1, empty);
	_protein=temp11;

	vector<vector<vector<int>>> temp12(complexNum+1, empty);
	_complex=temp12;
	
	vector<vector<vector<int>>> temp13(physicalEntityNum+1, empty);
	_entity=temp13;

	vector<vector<vector<int>>> temp14(smallMoleculeNum+1, empty);
	_smallMolecule=temp14;

	vector<vector<vector<int>>> temp15(DnaNum+1, empty);
	_Dna=temp15;

	vector<vector<vector<int>>> temp16(reactionNum+1, empty);
	_reaction=temp16;

	vector<vector<vector<int>>> temp17(scene->CompartmentName[m_pathwayID].size(), empty);
	_compartment=temp17;

	vector<vector<vector<int>>> temp20(EmptyNum+1, empty);
	_Empty=temp20;

	//vector<QRectF> temp18(scene->CompartmentName[m_pathwayID].size(),QRectF(-1,-1,0,0));
	//CompartmentPos = temp18;

	//join Golgi membrane with 
	//CompartmentContain[1] 
	//CompartmentContain[2];
    //_newCompartmentPos

	/*for(int i=0; i<CompartmentPos.size(); i++) 
	{
		CompartmentPos[i].setWidth(compartmentPos[i].width());
		CompartmentPos[i].setHeight(compartmentPos[i].height());
	}*/
	HierarchicalLayout(false);
		
 	itemGrid=recordItemGrid();
	_scene->backupItemPos(m_pathwayID, complexPos, proteinPos, smallMoleculePos, DnaPos, EmptyPos, reactionPos, physicalEntityPos, edge);
	
	dCenter=QPoint(0,0);
		
	//highlighted.push_back(-1);  highlighted.push_back(-1);
	//itemSelected.push_back(-1); itemSelected.push_back(-1);

	draggingItem=groupSelected=0;  //PenisOn=0; =groupReadyforDraggin
	//MenuisOn=0;
	_groupBubbles.clear();
	_reactionBubbles.clear();
	_codeBubbles.clear();

	x0y0=QPoint(-this->Width()/2.0,-this->Height()/2.0);	
	x1y1=QPoint(this->Width()/2.0,this->Height()/2.0);

	_mousePressed=false;

	searchSharedProtein();

	_pathBubbleParent = NULL;
	this->_open = open;
	this->_TYPE = PATHBUBBLE1;		
	
}


void PathBubble1::HierarchicalLayout(bool whole)
{
	 vector<vector<int>> Rank0 = EdgetoNode(edge, _compartment); //for compartment
	vector<vector<vector<int>>> Ranks = EdgetoNode(edge, _protein, _complex, _entity, _smallMolecule, _Dna, _reaction, whole);
	

	//put the subtree to _newCompartmentPos location
	if(whole)
	{
		int roomID=_scene->CompartmentContain[m_pathwayID].size()-1;
		//_newCompartmentPos[roomID].setWidth(maxwidth);
		//_newCompartmentPos[roomID].setHeight(Rank.size());//temp
		//float x0=_newCompartmentPos[roomID].x(), y0=_newCompartmentPos[roomID].y();		
		//float w=_newCompartmentPos[roomID].width(), h=_newCompartmentPos[roomID].height();		

		float x0=compartmentPos[roomID].x(), y0=compartmentPos[roomID].y();		
		float w=compartmentPos[roomID].width(), h=compartmentPos[roomID].height();	
		
		roomID=0;

		for(int i=0; i<Ranks[roomID].size(); i++)
		{			
			for(int j=0; j<Ranks[roomID][i].size(); j++) 
			{
				if(Ranks[roomID][i][j]<_newPos.size())
				{
				float x = _newPos[Ranks[roomID][i][j]].center().x();
			    float y = _newPos[Ranks[roomID][i][j]].center().y();
				int tempi=Ranks[roomID][i][j];
			
		        x=x*w+x0; y=y*h+y0;						
			    _newPos[Ranks[roomID][i][j]].moveCenter(QPointF(x,y));
				}
			}			
		}	
		

	}
	else
	{
        //comput _newcompartmentpos based on rank0's location and tanks's size info
		float y=0, Wmax=0;
		vector<float> Wlist(Rank0.size(),0);
		for(int i=0; i<Rank0.size(); i++)
		{	

            float hmax = 1,wsum=0; 

			for(int j=0; j<Rank0[i].size(); j++) 
			{
				if(Rank0[i][j]<_compartment.size())
				{
					
					int h=Ranks[Rank0[i][j]].size();
				    if(h>hmax)
					  hmax=h;		
                    
					float wmax=0;
					for(int k=0; k< Ranks[Rank0[i][j]].size(); k++)
					{
						int w = Ranks[Rank0[i][j]][k].size();
						if(w>wmax)
					        wmax=w;							
					}
					
					_newCompartmentPos[Rank0[i][j]].setX(wsum);
					_newCompartmentPos[Rank0[i][j]].setWidth(wmax);

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
					
					_newCompartmentPos[Rank0[i][j]].setY(y);
					_newCompartmentPos[Rank0[i][j]].setHeight(hmax);
					
					
				}
			}
			
			y=y+hmax;
		}	
 ///set y
		/*for(int i=0; i<Rank0.size(); i++)
		{	
			for(int j=0; j<Rank0[i].size(); j++) 
			{
				if(Rank0[i][j]<_compartment.size())
		 	       _newCompartmentPos[Rank0[i][j]].setY(_newCompartmentPos[Rank0[i][j]].y()/y);
			}			
		}		
//set x
		for(int i=0; i<Rank0.size(); i++)
		{	

			for(int j=0; j<Rank0[i].size(); j++) 
			{
				if(Rank0[i][j]<_compartment.size())
		 	       _newCompartmentPos[Rank0[i][j]].setX( _newCompartmentPos[Rank0[i][j]].x()  - Wlist[i]/2.0  );
			}			
		}*/

		///set y
		for(int i=0; i<Rank0.size(); i++)
		{	
			for(int j=0; j<Rank0[i].size(); j++) 
			{
				if(Rank0[i][j]<_compartment.size())
				{ 
					float w=_newCompartmentPos[Rank0[i][j]].width(), h=_newCompartmentPos[Rank0[i][j]].height();
					_newCompartmentPos[Rank0[i][j]].setY(_newCompartmentPos[Rank0[i][j]].y()/y );//- h/y/2);
					//_newCompartmentPos[Rank0[i][j]].setHeight(0.18);
					//_newCompartmentPos[Rank0[i][j]].setWidth(w/Wmax);
					_newCompartmentPos[Rank0[i][j]].setHeight(h/y);
				}
			}			
		}		
		//set x
		for(int i=0; i<Rank0.size(); i++)
		{	

			for(int j=0; j<Rank0[i].size(); j++) 
			{
				if(Rank0[i][j]<_compartment.size())
				{
					float w=_newCompartmentPos[Rank0[i][j]].width(), h=_newCompartmentPos[Rank0[i][j]].height();
					float x=_newCompartmentPos[Rank0[i][j]].x();
					float Wl=Wlist[i];
					_newCompartmentPos[Rank0[i][j]].setX((_newCompartmentPos[Rank0[i][j]].x() - Wlist[i]/2.0)/Wmax + 0.5);
					//_newCompartmentPos[Rank0[i][j]].setWidth(0.2);
                    _newCompartmentPos[Rank0[i][j]].setWidth(w/Wmax);
					//_newCompartmentPos[Rank0[i][j]].setHeight(h);
					
				}
			}			
		}

		
		for(int roomID = 0; roomID < Ranks.size(); roomID++) 	    
		{
		//int roomID=_scene->CompartmentContain[m_pathwayID].size()-1;
		//_newCompartmentPos[roomID].setWidth(maxwidth);
		//_newCompartmentPos[roomID].setHeight(Rank.size());//temp
		float x0=_newCompartmentPos[roomID].x(), y0=_newCompartmentPos[roomID].y();		
		float w=_newCompartmentPos[roomID].width(), h=_newCompartmentPos[roomID].height();		

		//float x0=compartmentPos[roomID].x(), y0=compartmentPos[roomID].y();		
		//float w=compartmentPos[roomID].width(), h=compartmentPos[roomID].height();	
		//roomID=0;
		for(int i=0; i<Ranks[roomID].size(); i++)
		{			
			for(int j=0; j<Ranks[roomID][i].size(); j++) 
			{
				if(Ranks[roomID][i][j]<_newPos.size())
				{
				float x = _newPos[Ranks[roomID][i][j]].center().x();
			    float y = _newPos[Ranks[roomID][i][j]].center().y();
				int tempi=Ranks[roomID][i][j];
			
		        x=x*w+x0; y=y*h+y0;						
			    _newPos[Ranks[roomID][i][j]].moveCenter(QPointF(x,y));

				}
			}			
		}	      

		}

	}



	
	//replace the nodelocation with hierarchicle ones
	float itv=0.9/8;

	int totalNum = complexNum + proteinNum + physicalEntityNum + smallMoleculeNum + DnaNum + reactionNum;
	for(int i=0; i<totalNum; i++)
	{
		int id=_VertexInfo[i][1];
		float posx=_newPos[i].x()<-0.99?-1:_newPos[i].x(), posy=_newPos[i].y()<-0.99?-1:_newPos[i].y();
		QPointF newp;
		newp.setX(posx); newp.setY(posy);		
		if(_newPos[i].center().x()<-0.99||_newPos[i].center().y()<-0.99) 
		{
			 _newPos[i].setX(-1); _newPos[i].setY(-1);
		}
		
		if(itv<0.03)
			itv=0.03;
		
		if(m_pathwayID==0)
		switch(int(_VertexInfo[i][0]))
		{
		    case 'C': complexPos[id]=_newPos[i];        if(complexPos[id].width()>itv) complexPos[id].setWidth(itv); 
				break;
			case 'E': physicalEntityPos[id]=_newPos[i]; if(physicalEntityPos[id].width()>itv) physicalEntityPos[id].setWidth(itv);
				break;
		    case 'P': proteinPos[id]=_newPos[i];        if(proteinPos[id].width()>itv)  proteinPos[id].setWidth(itv); 
				break;
		    case 'S': smallMoleculePos[id]=_newPos[i];  if(smallMoleculePos[id].width()>itv) smallMoleculePos[id].setWidth(itv); 
				break;
		    case 'D': 
				      DnaPos[id]=_newPos[i];            if(DnaPos[id].width()>itv) DnaPos[id].setWidth(itv); 
					  break;
			case 'N': 
				      EmptyPos[id]=_newPos[i];          if(EmptyPos[id].width()>itv) EmptyPos[id].setWidth(itv); 
					  break;
			case 'R': reactionPos[id]=_newPos[i];       break;
		}
		else if(m_pathwayID==1)
		switch(int(_VertexInfo[i][0]))
		{
		    case 'C': complexPos[id]=_newPos[i];        if(complexPos[id].width()>0.05) complexPos[id].setWidth(0.05); 
				break;
			case 'E': physicalEntityPos[id]=_newPos[i]; if(physicalEntityPos[id].width()>0.05) physicalEntityPos[id].setWidth(0.05);
				break;
		    case 'P': proteinPos[id]=_newPos[i];        if(proteinPos[id].width()>0.05)  proteinPos[id].setWidth(0.05); 
				break;
		    case 'S': smallMoleculePos[id]=_newPos[i];  if(smallMoleculePos[id].width()>0.05) smallMoleculePos[id].setWidth(0.05); 
				break;
		    case 'D': 
				      DnaPos[id]=_newPos[i];            if(DnaPos[id].width()>0.05) DnaPos[id].setWidth(0.05); 
					  break;
			case 'R': reactionPos[id]=_newPos[i];       break;
		}
		else if(m_pathwayID==2)
		switch(int(_VertexInfo[i][0]))
		{
		    case 'C': complexPos[id]=_newPos[i];        if(complexPos[id].width()>0.08) complexPos[id].setWidth(0.08); 
				break;
			case 'E': physicalEntityPos[id]=_newPos[i]; if(physicalEntityPos[id].width()>0.08) physicalEntityPos[id].setWidth(0.08);
				break;
		    case 'P': proteinPos[id]=_newPos[i];        if(proteinPos[id].width()>0.08)  proteinPos[id].setWidth(0.08); 
				break;
		    case 'S': smallMoleculePos[id]=_newPos[i];  if(smallMoleculePos[id].width()>0.08) smallMoleculePos[id].setWidth(0.08); 
				break;
		    case 'D': 
				      DnaPos[id]=_newPos[i];            if(DnaPos[id].width()>0.08) DnaPos[id].setWidth(0.08); 
					  break;
			case 'R': reactionPos[id]=_newPos[i];       break;
		}
	}	


}


PathBubble1::~PathBubble1()
{
	complexPos.clear();
	proteinPos.clear();	
    smallMoleculePos.clear();	
    DnaPos.clear();	
    reactionPos.clear();	
    physicalEntityPos.clear();	
	edge.clear();
}

void PathBubble1::searchSharedProtein()
{   	
	sharedProtein.clear();
	for(int i=0; i<_scene->_7proteinName.size(); i++)
	{  set<int> record;
	   sharedProtein.push_back(record);
	}

	for(int j=1; j<_scene->_7proteinName[m_pathwayID].size(); j++)
	{
		if(proteinPos[j].x()>-0.99)
		for(int i=0; i<_scene->_7proteinName.size(); i++)
		{			
      		if(i!=m_pathwayID)
			{
			   for(int k=1; k<_scene->_7proteinName[i].size(); k++)		
			   {			 
				  if(_scene->_7proteinName[i][k]==_scene->_7proteinName[m_pathwayID][j])  
				  {
					  if(_scene->_proteinPos[i][k].x()>-0.99)
				      {						
						  for(int m=0; m< _scene->CompartmentContain[m_pathwayID].size()-1;m++)
						  {
							    vector<int> ctemp; ctemp.push_back('P'); ctemp.push_back(j);
								if(_scene->CompartmentContain[m_pathwayID][m].find(ctemp) != _scene->CompartmentContain[m_pathwayID][m].end())
								{
									vector<int> otemp;
									otemp.push_back('P'); otemp.push_back(k);
									for(int n=0; n<_scene->CompartmentContain[i].size()-1; n++)
									{
										if(_scene->CompartmentName[i][n]==_scene->CompartmentName[m_pathwayID][m])
										if(_scene->CompartmentContain[i][n].find(otemp) != _scene->CompartmentContain[i][n].end())
										{
											 sharedProtein[i].insert(j);				  
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


void PathBubble1::multiplyDna()
{
	 vector<vector<QString>> newName;
	 vector<int> unit;
	 vector<vector<int>> record(_scene->CompartmentContain[m_pathwayID].size(), unit);
	 newName.push_back(_scene->_9DnaName[m_pathwayID][0]);
	 for(int i=0; i<Dna.size(); i++)
	 {		 
		 newName.push_back(_scene->_9DnaName[m_pathwayID][Dna[i]]);
		 for(int j=0;j<_scene->CompartmentName[m_pathwayID].size();j++)
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
	  for(int i=0; i<DnaNum; i++)
	   {
		 for(int j=0;j<_scene->CompartmentName[m_pathwayID].size();j++)
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
	 for(int i=0; i<_scene->_4biochemicalReaction[m_pathwayID].size(); i++)
	 {
		 for(int k=0;k<2;k++)
		 {
		  vector<int> left=_scene->_4biochemicalReaction[m_pathwayID][i][k];
	      
		  for(int j=0; j<left.size(); j=j+2)
		  {
	         int type=left[j], id=left[j+1];
			 if(type=='D')
			 {
			     _scene->_4biochemicalReaction[m_pathwayID][i][k][j+1]=c;	
				 c++;
			 }
		  }
		 }
	 }
	 _scene->_9DnaName[m_pathwayID]=newName;
}

void PathBubble1::multiplySmallMolecule()
{
	vector<vector<QString>> newName;	
	 vector<int> unit;
	 vector<vector<int>> record(_scene->CompartmentContain[m_pathwayID].size(), unit);
	
	 newName.push_back(_scene->_9smallMoleculeName[m_pathwayID][0]);
	 for(int i=0; i<smallMolecule.size(); i++)
	 {
		 newName.push_back(_scene->_9smallMoleculeName[m_pathwayID][smallMolecule[i]]);
		 for(int j=0;j<_scene->CompartmentName[m_pathwayID].size();j++)
		 {
			   vector<int> item;
			   item.push_back('S'); item.push_back(smallMolecule[i]);
			   if(_scene->CompartmentContain[m_pathwayID][j].find(item)!=_scene->CompartmentContain[m_pathwayID][j].end())
			   {   
				   //item[1]=i+1;			       	
				   record[j].push_back(i+1);
				   break;
			   }			
		 }
	 }

	   for(int i=0; i<smallMoleculeNum; i++)
	   {
		 for(int j=0;j<_scene->CompartmentName[m_pathwayID].size();j++)
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

	  int c=1;
	 for(int i=0; i<_scene->_4biochemicalReaction[m_pathwayID].size(); i++)
	 {
		 for(int k=0;k<2;k++)
		 {
		  vector<int> left=_scene->_4biochemicalReaction[m_pathwayID][i][k];
	      
		  for(int j=0; j<left.size(); j=j+2)
		  {
	         int type=left[j], id=left[j+1];
			 if(type=='S')
			 {
			     _scene->_4biochemicalReaction[m_pathwayID][i][k][j+1]=c;	
				 c++;
			 }
		  }
		 }
	 }

	 _scene->_9smallMoleculeName[m_pathwayID]=newName;

}


vector<vector<vector<int>>> PathBubble1::EdgetoNode(vector<vector<int>> edge, vector<vector<vector<int>>> &_protein, vector<vector<vector<int>>> &_complex, vector<vector<vector<int>>> &_entity, vector<vector<vector<int>>> &_smallMolecule, vector<vector<vector<int>>> &_Dna, vector<vector<vector<int>>> &_reaction, bool whole)
{   vector<vector<vector<int>>> Ranks;
	//record nodes and edges for layout purpos	
	vector<vector<int>> VertexInfo;
	vector<vector<int>> Edge;
	vector<int> iunit;
	vector<vector<int>> vunit(2,iunit);
	vector<vector<vector<int>>> Vertex(complexNum + proteinNum + physicalEntityNum + smallMoleculeNum + DnaNum + reactionNum + EmptyNum, vunit); //store the out and in edges 
	vector<int> temp(2,0);
	vector<QRectF> newPos;//(complexNum + proteinNum + physicalEntityNum + smallMoleculeNum + DnaNum + reactionNum, QRectF(-1,-1, 0, 0));

	for(int i=1; i<=complexNum;i++)
	{
	   temp[0]='C'; temp[1]=i; newPos.push_back(QRectF(-1,-1,complexPos[i].width(),complexPos[i].height()));
	   VertexInfo.push_back(temp);
	}

	for(int i=1; i<=proteinNum;i++)
	{
	   temp[0]='P'; temp[1]=i; newPos.push_back(QRectF(-1,-1,proteinPos[i].width(),proteinPos[i].height()));
	   VertexInfo.push_back(temp);
	}
	
	for(int i=1; i<=smallMoleculeNum;i++)
	{
	   temp[0]='S'; temp[1]=i; newPos.push_back(QRectF(-1,-1,smallMoleculePos[i].width(),smallMoleculePos[i].height()));
	   VertexInfo.push_back(temp);
	}

	for(int i=1; i<=physicalEntityNum;i++)
	{
	   temp[0]='E'; temp[1]=i;
	   VertexInfo.push_back(temp); newPos.push_back(QRectF(-1,-1, physicalEntityPos[i].width(),physicalEntityPos[i].height())); //newPos.push_back(physicalEntityPos[i]);
	}

	for(int i=1; i<=DnaNum;i++)
	{
	   temp[0]='D'; temp[1]=i;
	   VertexInfo.push_back(temp); newPos.push_back(QRectF(-1,-1, DnaPos[i].width(), DnaPos[i].height())); //newPos.push_back(physicalEntityPos[i]); 
	}
	
	for(int i=1; i<=EmptyNum;i++)
	{
	   temp[0]='N'; temp[1]=i;	   
	   VertexInfo.push_back(temp); newPos.push_back(QRectF(-1,-1, EmptyPos[i].width(), EmptyPos[i].height())); //newPos.push_back(physicalEntityPos[i]); 
	}
	
	for(int i=1; i<=reactionNum;i++)
	{
	   temp[0]='R'; temp[1]=i;
	   VertexInfo.push_back(temp); newPos.push_back(QRectF(-1,-1, reactionPos[i].width(),reactionPos[i].height())); //newPos.push_back(physicalEntityPos[i]); newPos.push_back(reactionPos[i]);
	}
	int type, id;
	vector<int> start(4,0), middle(4,0), end(4,0);
	vector<int> ed;
	//set<vector<int>> check; 
	
	for(int i=0; i<edge.size(); i++)
	{
         start[0]=edge[i][0], start[1]=edge[i][1];	
		 		 
		 middle[0]=edge[i][4]; middle[1]=edge[i][5]; 
		 
		 end[0]=edge[i][2], end[1]=edge[i][3]; 

		 if(end[0]!='R')		 
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
			 _reaction[middle[1]].push_back(ed);  
			
			 temp[0] = getVerticeID_2(middle[0],middle[1]);
			 temp[1] = getVerticeID_2(end[0],end[1]);

			 if(i>206)
			    i=i;
			 
			 Vertex[temp[0]][0].push_back(Edge.size());
			 Vertex[temp[1]][1].push_back(Edge.size());
			 Edge.push_back(temp);				 

			 ed=end; ed[2]='A'; ed[3]=1; 
			 _reaction[middle[1]].push_back(ed);       
		    
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
			 temp[1] = getVerticeID_2(end[0],end[1]);	
			
			 Vertex[temp[0]][0].push_back(Edge.size());
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
			 _reaction[end[1]].push_back(ed);      			 
		 }
	}

	for(int i=0; i<Vertex.size(); i++)
	{
		VertexInfo[i].push_back(-1); VertexInfo[i].push_back(-1);
	}
	
	for(int i=0; i<_scene->CompartmentName[m_pathwayID].size();i++)
    {		
		for(int j=0; j<_scene->CompartmentContain[m_pathwayID][i].size(); j++) 
		{
			set<vector<int>> Clist=_scene->CompartmentContain[m_pathwayID][i];
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
	compartmentPos.push_back(QRectF(0, 0, 1, 1));//left
	_scene->CompartmentName[m_pathwayID].push_back("whole");			
	for(int i=0; i<VertexInfo.size(); i++)
	{
		vector<int> temp;
		temp.push_back(VertexInfo[i][0]); temp.push_back(VertexInfo[i][1]);
	    newCompart.insert(temp);
	}
	_scene->CompartmentContain[m_pathwayID].push_back(newCompart);
	
	if(whole)
	{
		for(int i=0; i<Vertex.size(); i++)
		{
		VertexInfo[i][3]=_scene->CompartmentContain[m_pathwayID].size()-1;
		}

		Ranks.push_back(subMap(Vertex, VertexInfo, Edge, newPos, _scene->CompartmentContain[m_pathwayID].size()-1));//_scene->CompartmentContain[m_pathwayID].size()-1
	//for the whole graph in "whole"
	}
	else 
	{
		for(int i=0; i<_scene->CompartmentContain[m_pathwayID].size(); i++)
		{
			//itv=subMap(Vertex, VertexInfo, Edge, newPos, i);//_scene->CompartmentContain[m_pathwayID].size()-1	
			Ranks.push_back(subMap(Vertex, VertexInfo, Edge, newPos, i));//_scene->CompartmentContain[m_pathwayID].size()-1
		}
	}


	newPos.resize(vsize);

	_VertexInfo=VertexInfo;
	_Edge=Edge;
	_Vertex=Vertex; //store the out and in edges 	
	_newPos=newPos;
	return Ranks;
}


vector<vector<int>> PathBubble1::EdgetoNode(vector<vector<int>> edge, vector<vector<vector<int>>> &compartment)
{


	vector<vector<int>> Rank;
	vector<int> temp(4,0);
	vector<vector<int>> VertexInfo;
	vector<QRectF> newPos; 
	int di=_scene->CompartmentName[m_pathwayID].size()-1;
	//vector<vector<int>> edge;
	for(int i=1; i<=reactionNum;i++)
	{
	   temp[0]='R'; temp[1]=i; temp[2]=-1; temp[3]=-1;
	   VertexInfo.push_back(temp); newPos.push_back(QRectF(-1,-1, reactionPos[i].width(),reactionPos[i].height())); //newPos.push_back(physicalEntityPos[i]); newPos.push_back(reactionPos[i]);
	}
	
	//record nodes and edges for layout purpos		
	vector<vector<int>> Edge;
	vector<int> iunit;
	vector<vector<int>> vunit(2,iunit);
	vector<vector<vector<int>>> Vertex(_scene->CompartmentName[m_pathwayID].size()+reactionNum, vunit); //store the out and in edges 
		
	int type, id;
	vector<int> start(4,0), middle(4,0), end(4,0);
	vector<int> start1(2,0), middle1(2,0), end1(2,0);
	vector<int> ed;

	for(int i=0; i<_scene->CompartmentName[m_pathwayID].size();i++)
    {	
		for(int j=0; j<_scene->CompartmentContain[m_pathwayID][i].size(); j++) 
		{
			set<vector<int>> Clist=_scene->CompartmentContain[m_pathwayID][i];
			for(set<vector<int>>::iterator it=Clist.begin(); it!=Clist.end(); it++)
			{
				vector<int> node=*it;	
				int id, type;
				type=node[0], id=node[1];		
			}
		}	
		temp[0]='C';  temp[1]=i;  newPos.push_back(QRectF( -1, -1, compartmentPos[i].width(), compartmentPos[i].height()));
	    VertexInfo.push_back(temp);
		VertexInfo[i][3]=i;	
		/*if(_scene->CompartmentContain[m_pathwayID][i].size()>0)
		{
			
		}*/
	}

			
	for(int i=0; i<edge.size();i++)
	{
		start1[0]=edge[i][0];  start1[1]=edge[i][1];			 		 
		middle1[0]=edge[i][4]; middle1[1]=edge[i][5]; 
		end1[0]=edge[i][2];    end1[1]=edge[i][3];  

		 if(i>=73)
			 i=i;
		 int starti, endi;

		if(end[0]!='R')	
		{
			for(int j=0; j<_scene->CompartmentName[m_pathwayID].size();j++)
		    { 
				if(_scene->CompartmentContain[m_pathwayID][j].find(end1)!=_scene->CompartmentContain[m_pathwayID][j].end())
				{
					 edge[i][2]='C';    edge[i][3]=j;  endi=j;
				}
			}		
		}		
		for(int j=0; j<_scene->CompartmentName[m_pathwayID].size();j++)
		{ 
				if(_scene->CompartmentContain[m_pathwayID][j].find(start1)!=_scene->CompartmentContain[m_pathwayID][j].end())
				{
					 edge[i][0]='C';    edge[i][1]=j;  starti=j;
				}
		}
		if(endi==starti)
           edge[i][0]='R';
	}

		
	for(int i=0; i<edge.size(); i++)
	{
         start[0]=edge[i][0], start[1]=edge[i][1];	
		 		 
		 middle[0]=edge[i][4]; middle[1]=edge[i][5]; 
		 
		 end[0]=edge[i][2], end[1]=edge[i][3]; 

		 if(i>=73)
			 i=i;
		 
		 if(end[0]!='R'&&start[0]!='R')		 
	     {
			 temp[0] = start[1];
			 temp[1] = middle[1]+di;			 
			 Vertex[temp[0]][0].push_back(Edge.size());
			 Vertex[temp[1]][1].push_back(Edge.size());
			 Edge.push_back(temp);				 

			 ed=middle;  ed[2]='M'; ed[3]=1; 

			 switch(start[0])
			 {
				 case 'C': _compartment[start[1]].push_back(ed);   break;
		     }			
			 ed=start;  ed[2]='M'; ed[3]=-1; 
			 _reaction[middle[1]].push_back(ed);  

			 temp[0] = middle[1]+di;
			 temp[1] = end[1];

		

			 Vertex[temp[0]][0].push_back(Edge.size());
			 Vertex[temp[1]][1].push_back(Edge.size());

			 Edge.push_back(temp);				 

			 ed=end; ed[2]='A'; ed[3]=1; 
			 _reaction[middle[1]].push_back(ed);       
		    
			 ed=middle;  ed[2]='A';  ed[3]=-1; 
			 switch(end[0])
	         {
				case 'C': _compartment[end[1]].push_back(ed);        break;				
		     }
		 }
		 else if(start[0]!='R')
		 {
			 temp[0] = start[1];
			 temp[1] = end[1]+di;	
			 Vertex[temp[0]][0].push_back(Edge.size());
			 Vertex[temp[1]][1].push_back(Edge.size());

			 Edge.push_back(temp);	

			 ed=end;  ed[2]='R'; ed[3]=1; 
			 switch(start[0])
			 {
		     case 'C': _compartment[start[1]].push_back(ed);   break;			
		     }
			 ed=start; ed[2]='R'; ed[3]=-1; 
			 _reaction[end[1]].push_back(ed);      			 
		 }
	}

	/*for(int i=0; i<Vertex.size(); i++)
	{
		VertexInfo[i].push_back(-1); VertexInfo[i].push_back(-1);
	}
	
	for(int i=0; i<_scene->CompartmentName[m_pathwayID].size();i++)
    {				
		VertexInfo[i][3]=i;					      
		
	}*/


	for(int i=0; i<Vertex.size(); i++)
	{
		VertexInfo[i][3]=-1;
	}

	int vsize=Vertex.size();

	

	Rank=subMap(Vertex, VertexInfo, Edge, newPos, -1);//_scene->CompartmentContain[m_pathwayID].size()-1
	
	//straighten the pos of the reaction when it is 
	
	newPos.resize(vsize);

	_VertexInfo=VertexInfo;
	_Edge=Edge;
	_Vertex=Vertex; //store the out and in edges 	
	_newCompartmentPos=newPos;

     for(int i=0; i<compartmentPos.size(); i++)
	 {
	     //_newCompartmentPos[i].setWidth(compartmentPos[i].width());  
		 //_newCompartmentPos[i].setHeight(compartmentPos[i].height());	 
		 _newCompartmentPos[i].setWidth(0.1);  
		 _newCompartmentPos[i].setHeight(0.1);	 
	 }
	 return Rank;
}


void PathBubble1::subMap_backup(vector<vector<vector<int>>> Vertex, vector<vector<int>> VertexInfo, vector<vector<int>> Edge, vector<QRectF> &newPos, int roomID)
{
	/*
	if(scene->CompartmentContain[m_pathwayID][roomID].size()<1)
		return;

	vector<int> length(Vertex.size(),-1);
	int max=-100000,maxid,l;
	vector<int> iunit;
	vector<int> temp(2,0);
	vector<vector<int>> vunit(2,iunit);
	
	for(int i=0; i<Vertex.size(); i++)
	{
		if(VertexInfo[i][3]==roomID&&Vertex[i][1].empty()&&!Vertex[i][0].empty())
		{		
		    l=findLongestPath(i, length, Edge, Vertex, VertexInfo, roomID);	
			if(l>max)
			{
				max=l;	
				maxid=i;
			}		
		}	
		
	}

	//mark the longest path with rank
	vector<int> path;
	markLongestPath(maxid, VertexInfo, path, length, Edge, Vertex);
	for(int i=0; i<path.size(); i++)
		VertexInfo[path[i]][2]=i;

	//find the tree linked to the longest path and find the rank of every node linked to the path
	for(int i=0; i<path.size(); i++)
	{
	   findRank_up_backup(path[i], i, VertexInfo, length, Edge, Vertex, roomID);
	}	

	for(int i=0; i<path.size(); i++)
	{
	   findRank_down_backup(path[i], i, VertexInfo, length, Edge, Vertex, roomID);
	}	
	
	//assign the first path location
	for(int i=0; i<path.size(); i++)
	{
	    //newpos[path[i]].setX(0.5);
		//newpos[path[i]].setX(0); //order
		//newpos[path[i]].setY((float)i/(path.size()+2)+0.05);

		newPos[path[i]].moveCenter(QPointF(0,(float)i/(path.size()+2)+0.05));
	}

	//give the first order of each rank
	iunit.push_back(-1);
	vector<vector<int>> Rank(path.size(),iunit);
	for(int i=0; i<Vertex.size(); i++)
	{	
	    if(VertexInfo[i][2]>=0&&VertexInfo[i][2]<path.size())
		{
			 if(newPos[i].center().x()>-0.99)
				 Rank[VertexInfo[i][2]][0]=i;
			else 
				Rank[VertexInfo[i][2]].push_back(i);
		}
		else if(VertexInfo[i][2]>=path.size())
			i=i;	
	}
	
	for(int i=0; i<Rank.size(); i++)
	{			
		float inv=0.8/Rank[i].size();
	    for(int j=1; j<Rank[i].size(); j++) 
		{
			//newpos[Rank[i][j]].setX(j);
			//newpos[Rank[i][j]].setY((float)i/(path.size()+2)+0.05);
			newPos[Rank[i][j]].moveCenter(QPointF(j,(float)i/(path.size()+2)+0.05));
			VertexInfo[Rank[i][j]][2]=i;  //record rank
		}
	}
	
	//add dummy node to graph
	int EdgeNum=Edge.size();
	int nodeNum=VertexInfo.size();
	vector<vector<int>> polyEdge;
	for(int i=0; i<EdgeNum; i++)
	{
	   int vid0=Edge[i][0],vid1=Edge[i][1];
	   if(newPos[vid0].center().x()>=-0.99&&newPos[vid1].center().x()>=-0.99)
	   {	
		   vector<int> tedge;	
		   tedge.push_back(vid0);
		  if(VertexInfo[vid1][2]-VertexInfo[vid0][2]>1)
		  {			  
			  int eid=Edge.size(),vid=Vertex.size();
			  float x0,y0,dx,dy,x,y;
			  x0=newPos[vid0].center().x(); y0=newPos[vid0].center().y();
			  dx=(newPos[vid1].center().x()-x0)/(VertexInfo[vid1][2]-VertexInfo[vid0][2]); dy=(newPos[vid1].center().y()-y0)/(VertexInfo[vid1][2]-VertexInfo[vid0][2]);
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
			        
					newPos.push_back(QRectF(-1,-1,0,0));  //newPos[vid].setX(x); newPos[vid].setY(y);
					newPos[vid].moveCenter(QPointF(x,y));
					
					Edge.push_back(temp); Edge[eid][0]=vid; Edge[eid][1]=(j==VertexInfo[vid1][2]-VertexInfo[vid0][2]-2)?vid1:vid+1;
					
					Rank[VertexInfo[vid][2]].push_back(vid);

					tedge.push_back(vid);
					eid++; vid++; x=x+dx; y=y+dy;
			  }		      
		  }
		   tedge.push_back(vid1);
		   polyEdge.push_back(tedge);
	   }
	}

	//float ppx2=newPos[81].x();
	for(int m=0; m<3; m++)
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
	int maxwidth=-10000;	
	for(int i=0; i<Rank.size(); i++)
	{		
		int value=Rank[i].size();
		if(maxwidth<value)
			maxwidth=value;		
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
			Rank[i].push_back(temp[1]);
		}
	}
		
	
	for(int i=0; i<Rank.size(); i++)
	{			
		for(int j=0; j<Rank[i].size(); j++) 
		{
			float x= newPos[Rank[i][j]].center().x()-int(Rank[i].size()/2.0) + maxwidth/2.0;
			x=x/maxwidth*0.9+0.05;
			newPos[Rank[i][j]].moveCenter(QPointF(x,newPos[Rank[i][j]].center().y()));						
		}			
	}
		
	
	//convert to location
	float itv=0.95/maxwidth;

	float maxwidthf=0;	
	vector<float> rankWidth;

	for(int i=0; i<Rank.size(); i++)
	{		
		float widthf=0;
		for(int j=0; j<Rank[i].size(); j++) 
		{
			if(newPos[Rank[i][j]].width()>itv)
			     widthf=widthf+2*itv;
			else widthf=widthf+itv;			
		}	
		rankWidth.push_back(widthf);
		if(maxwidthf<widthf)
			maxwidthf=widthf;	
	}

	float scale=1;
	if(maxwidthf>1.0)
	   scale = 1/maxwidthf;

	for(int i=0; i<Vertex.size(); i++)
	{	
	    newPos[i].setWidth(newPos[i].width()*scale);
		newPos[i].setHeight(newPos[i].height()*scale);
	}

	itv=itv*scale;
	
	for(int i=0; i<Rank.size(); i++)
	{			
		float px=0;
		for(int j=0; j<Rank[i].size(); j++) 
		{

			float y= newPos[Rank[i][j]].center().x()-int(Rank[i].size()/2.0) + maxwidth/2.0;
			y=y/maxwidth*0.9+0.05;

			float py=newPos[Rank[i][j]].center().x(), cy=Rank[i].size()/2.0; 
			y=y;

			float x = px - rankWidth[i]/2.0 + maxwidthf/2.0+0.05;				
			//newPos[Rank[i][j]].setX(x);
			newPos[Rank[i][j]].moveCenter(QPointF(x,newPos[Rank[i][j]].center().y()));
			px = px + (newPos[Rank[i][j]].width()>itv? itv*2: itv); 
			
			//newPos[Rank[i][j]].moveCenter(QPointF(x,newPos[Rank[i][j]].center().y()));		
		}			
	}
	
	//further put in compartment
	float x0=compartmentPos[roomID].x(), y0=compartmentPos[roomID].y();
	float w=compartmentPos[roomID].width(), h=compartmentPos[roomID].height();		
	for(int i=0; i<Rank.size(); i++)
	{			
		for(int j=0; j<Rank[i].size(); j++) 
		{
			float x = newPos[Rank[i][j]].center().x();
			float y = newPos[Rank[i][j]].center().y();
		    x=x*w+x0; y=y*h+y0;									
		    //newPos[Rank[i][j]].setX(x);	
			//newPos[Rank[i][j]].setY(y);	
			newPos[Rank[i][j]].moveCenter(QPointF(x,y));
		}			
	}
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
		if(newPos[i].center().y()<-0.5) //order
		   //newPos[i].setX(-1);	
		   newPos[i].moveCenter(QPointF(-1,newPos[i].center().y()));	
	}
	
	*/
}

vector<vector<int>> PathBubble1::subMap(vector<vector<vector<int>>> Vertex, vector<vector<int>> VertexInfo, vector<vector<int>> Edge, vector<QRectF> &newPos, int roomID)
{   vector<vector<int>> tempR;
	
	if(roomID!=-1 && scene->CompartmentContain[m_pathwayID][roomID].size()<1)
	{
		//CompartmentPos[roomID].setWidth(0); CompartmentPos[roomID].setHeight(0);
		return tempR;
	}
	
	vector<int> length(Vertex.size(),-1);
	int max=-100000,maxid=-1,l;
	vector<int> iunit;
	vector<int> temp(2,0);
	vector<vector<int>> vunit(2,iunit);
	//vector<QPointF> newpos;

	for(int i=0; i<Vertex.size(); i++)
	{
		
		if((roomID==-1||VertexInfo[i][3]==roomID)&&Vertex[i][1].empty()&&!Vertex[i][0].empty())
		{		
			
		    l=findLongestPath(i, length, Edge, Vertex, VertexInfo, roomID);	
			if(l>max)
			{
				max=l;	
				maxid=i;
			}		
		}	
		//newpos.push_back(QPointF(-1,-1));
	}
	if(maxid==-1)
	{
		
		for(int i=0; i<Vertex.size(); i++)
	{
		
		if((roomID==-1||VertexInfo[i][3]==roomID)&&!Vertex[i][0].empty())
		{		
			
		    l=findLongestPath(i, length, Edge, Vertex, VertexInfo, roomID);	
			if(l>max)
			{
				max=l;	
				maxid=i;
			}		
		}	
		//newpos.push_back(QPointF(-1,-1));
	}
		if(maxid==-1)
		   return tempR;
	}

	//mark the longest path with rank
	vector<int> path;
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
	
	int mini=10000;
	for(int i=0; i<VertexInfo.size(); i++)
	{
	    if(mini>VertexInfo[i][2]&&VertexInfo[i][2]!=-1)
			mini=VertexInfo[i][2];
	}
	mini=mini-200;
	
	for(int i=0; i<VertexInfo.size(); i++)
	{
		if(VertexInfo[i][2]<200+mini)
			VertexInfo[i][2]=-1;
		if(VertexInfo[i][2]!=-1)
		    VertexInfo[i][2]=VertexInfo[i][2]-200-mini;		
	}

	//assign the first path location
	for(int i=0; i<path.size(); i++)
	{
		//newPos[path[i]].setX(0); //order
		//newPos[path[i]].setY((float)VertexInfo[path[i]][2]/(path.size()+2-mini)+0.05);
		newPos[path[i]].moveCenter(QPointF(0,(float)VertexInfo[path[i]][2]/(path.size()-mini-1)*0.95+0.025)); //order
	}
	//give the first order of each rank
	//iunit.push_back(-1);
	vector<vector<int>> Rank(path.size()-mini,iunit);
	for(int i=0; i<Vertex.size(); i++)
	{	
	    if(VertexInfo[i][2]>=0&&VertexInfo[i][2]<(path.size()-mini))
		{
			Rank[VertexInfo[i][2]].push_back(i);
		}
		else if(VertexInfo[i][2]>=(path.size()-mini))
			i=i;	
	}	


	for(int i=0; i<Rank.size(); i++)
	{			
		float inv=0.9/Rank[i].size();
	    for(int j=0; j<Rank[i].size(); j++) // for(int j=1(?)
		{			
			//newPos[Rank[i][j]].setX(j);
			//newPos[Rank[i][j]].setY((float)i/(path.size()+1-mini)+0.05);
			newPos[Rank[i][j]].moveCenter(QPointF(j,(float)i/(path.size()-mini-1)*0.95+0.025));
			VertexInfo[Rank[i][j]][2]=i;  //record rank
		}
	}


	//add dummy node to graph
	int EdgeNum=Edge.size();
	int nodeNum=VertexInfo.size();
	vector<vector<int>> polyEdge;
	for(int i=0; i<EdgeNum; i++)
	{
	   int vid0=Edge[i][0],vid1=Edge[i][1];
	   if(newPos[vid0].center().x()>=-0.99&&newPos[vid1].center().x()>=-0.99)
	   {	
		   vector<int> tedge;	
		   tedge.push_back(vid0);
		  if(VertexInfo[vid1][2]-VertexInfo[vid0][2]>1)
		  {			  
			  int eid=Edge.size(),vid=Vertex.size();
			  float x0,y0,dx,dy,x,y;
			  x0=newPos[vid0].center().x(); y0=newPos[vid0].center().y();
			  dx=(newPos[vid1].center().x()-x0)/(VertexInfo[vid1][2]-VertexInfo[vid0][2]); dy=(newPos[vid1].center().y()-y0)/(VertexInfo[vid1][2]-VertexInfo[vid0][2]);
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
			        
					newPos.push_back(QRectF(-1,-1,0,0));  //newPos[vid].setX(x); newPos[vid].setY(y);
					newPos[vid].moveCenter(QPointF(x,y));
					
					Edge.push_back(temp); Edge[eid][0]=vid; Edge[eid][1]=(j==VertexInfo[vid1][2]-VertexInfo[vid0][2]-2)?vid1:vid+1;
					
					Rank[VertexInfo[vid][2]].push_back(vid);

					tedge.push_back(vid);
					eid++; vid++; x=x+dx; y=y+dy;
			  }		      
		  }
		   tedge.push_back(vid1);
		   polyEdge.push_back(tedge);
	   }
	}

	//float ppx2=newPos[81].x();
	for(int m=0; m<5; m++)
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
			Rank[i].push_back(temp[1]);
		}
	}
		
	
	for(int i=0; i<Rank.size(); i++)
	{			
		for(int j=0; j<Rank[i].size(); j++) 
		{
			float x= newPos[Rank[i][j]].center().x()-int(Rank[i].size()/2.0) + maxwidth/2.0;
			x=x/maxwidth*0.9+0.05;
			newPos[Rank[i][j]].moveCenter(QPointF(x,newPos[Rank[i][j]].center().y()));						
		}			
	}
		
	//move to prevent overlapping
	float itv=0.9/maxwidth;
	for(int i=0; i<Rank.size(); i++)
	{			
		/*if(Rank[i].size()>1)
		for(int j= Rank[i].size()/2; j>1; j--) 
		{   if(VertexInfo[Rank[i][j]][0]!='R')
			if(newPos[Rank[i][j-1]].center().x()+newPos[Rank[i][j-1]].width()/2.0>newPos[Rank[i][j]].center().x()-newPos[Rank[i][j]].width()/2.0)
			{
				
				newPos[Rank[i][j-1]].moveCenter(QPointF(newPos[Rank[i][j-1]].center().x()-itv, newPos[Rank[i][j-1]].center().y()));	
			}				
		}			
	
		if(Rank[i].size()>1)
		for(int j= Rank[i].size()/2+1; j<Rank[i].size()-1; j++) 
		{
			if(VertexInfo[Rank[i][j]][0]!='R')	
			if(newPos[Rank[i][j+1]].center().x()-newPos[Rank[i][j+1]].width()/2.0<newPos[Rank[i][j]].center().x()+newPos[Rank[i][j]].width()/2.0)
			{
				newPos[Rank[i][j+1]].moveCenter(QPointF(newPos[Rank[i][j+1]].center().x()+itv, newPos[Rank[i][j+1]].center().y()));	
			}	
		}*/	
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
		if(newPos[i].center().y()<-0.5) //order		   	
		   newPos[i].moveCenter(QPointF(-1,newPos[i].center().y()));	
	}

	
	return Rank;	
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
if(VertexInfo[id][0]=='C' && VertexInfo[id][1]== 17)
		    id=id;

	if(VertexInfo[id][0]=='R' && VertexInfo[id][1]== 17)
		    id=id;

	if(VertexInfo[id][0]=='P' && VertexInfo[id][1]== 39)
		    id=id;

	if(VertexInfo[id][0]!='R' && VertexInfo[id][0]!='S' && VertexInfo[id][3]!=roomID)
	{	
		return;
	}
	if(VertexInfo[id][2]==-1)
	{
		VertexInfo[id][2]=rank+200;			
	}
	else if(VertexInfo[id][2]>rank+200)
	{
		VertexInfo[id][2]=rank+200;	
		return;
	}
	//if(Vertex[id][1].size()==0)
	//	return;
	for(int i=0; i<Vertex[id][1].size(); i++)
	{
		int eid=Vertex[id][1][i];
	    int nid=Edge[eid][0];
		if(VertexInfo[nid][2]==-1)
		     findRank_up(nid, rank-1, VertexInfo, length, Edge, Vertex, roomID);
	}

	for(int i=0; i<Vertex[id][0].size(); i++)
	{
		int eid=Vertex[id][0][i];
	    int nid=Edge[eid][1];
		if(VertexInfo[nid][2]==-1) 
		     findRank_down(nid, rank+1, VertexInfo, length, Edge, Vertex, roomID);
	}
}

void PathBubble1::findRank_down(int id, int rank, vector<vector<int>> &VertexInfo, vector<int> length, vector<vector<int>> Edge, vector<vector<vector<int>>> Vertex, int roomID)
{//find rank by searching down from a node in the longest path

	if(VertexInfo[id][0]=='C' && VertexInfo[id][1]== 17)
		    id=id;

	if(VertexInfo[id][0]=='R' && VertexInfo[id][1]== 17)
		    id=id;

	if(VertexInfo[id][0]=='P' && VertexInfo[id][1]== 39)
		    id=id;

	if(VertexInfo[id][0]!='R' && VertexInfo[id][0]!='S' && VertexInfo[id][3]!=roomID)
	{	
		return;
	}
	if(VertexInfo[id][2]==-1)
	{
		VertexInfo[id][2]=rank+200;	
		
	}
	else if(VertexInfo[id][2]<rank+200)
	{
		VertexInfo[id][2]=rank+200;	
		return;
	}
	//if(Vertex[id][0].size()==0)
	//	return;
	
	for(int i=0; i<Vertex[id][0].size(); i++)
	{
		int eid=Vertex[id][0][i];
	    int nid=Edge[eid][1];

		if(VertexInfo[id][0]=='R' && VertexInfo[id][1]== 17)
		    id=id;

		if(VertexInfo[nid][2]==-1)
		     findRank_down(nid, rank+1, VertexInfo, length, Edge, Vertex, roomID);
	}
	for(int i=0; i<Vertex[id][1].size(); i++)
	{
		if(VertexInfo[id][0]=='R' && VertexInfo[id][1]== 17)
		    id=id;

		int eid=Vertex[id][1][i];
	    int nid=Edge[eid][0];
		if(VertexInfo[nid][2]==-1)
		     findRank_up(nid, rank-1, VertexInfo, length, Edge, Vertex, roomID);
	}
}


void PathBubble1::markLongestPath(int id, vector<vector<int>> &VertexInfo, vector<int> &path, vector<int> length, vector<vector<int>> Edge, vector<vector<vector<int>>> Vertex)
{
	VertexInfo[id][2] = length[id];
	path.push_back(id);
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

int PathBubble1::findLongestPath(int id, vector<int> &length, vector<vector<int>> Edge, vector<vector<vector<int>>> Vertex, vector<vector<int>> &VertexInfo, int roomID)
{
	//Vertex[id][0]
	int l,max=-100000;
	if(VertexInfo[id][0]!='R' && VertexInfo[id][0]!='S' && VertexInfo[id][3]!=roomID)
	{	
		return -1;
	}
	if(length[id]!=-1)
	{
	    return length[id];
	}
    if(Vertex[id][0].empty())
	{
		length[id]=0;
		return length[id];
	}
	if(length[id]==-1)
	   length[id]=0;
 	for(int i=0; i<Vertex[id][0].size(); i++)
	{
        int eid=Vertex[id][0][i];				
		l=findLongestPath(Edge[eid][1], length, Edge, Vertex, VertexInfo, roomID);
		if(l>max)
			max=l;	
	}
	length[id]=max+1;
	return length[id];
}

void PathBubble1::saveAsXmlNode( QDomDocument &doc, QDomElement &node )
{
	QDomText text;
	QString str, tmpStr;

	QDomElement type = doc.createElement( tr("Type") );
	text = doc.createTextNode( tr("PathBubble1") );
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

	QList<ItemBase*> itemList;
	ItemBase* item;
	//ReactionBubble3* tmp;
	QString name;
	int i;

	QDomNodeList paraNode;
	QDomElement tmpNode;

	itemList = this->_scene->getGroupManager()->allMembers();

	/*paraNode = node.elementsByTagName( tr("ParentBubble") );
	
	if ( !paraNode.isEmpty() )
	{
		tmpNode = paraNode.at(0).toElement();
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

		for (i = 0; i < itemList.size(); i ++)
		{
			item = itemList[i];
			if ( item->getType() == REACTIONBUBBLE3 )
			{
				tmp = dynamic_cast<ReactionBubble3*>(item);
				if (tmp->getCurrentFilename() == name 
					&& fabs(tmp->pos().x() - x) < 3
					&& fabs(tmp->pos().y() - y) < 3 )
				{
					this->_codeParent = tmp;
					break;
				}
			}
		}
	}*/
	
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
			}
		}
	}
}



void PathBubble1::manuLocateCompartment(vector<QRectF> &Compartment)	
{
	set<vector<int>> empty;
	Compartment.push_back(QRectF(23/2587.0, 102/1333.0, (439-23)/2587.0, (1311-102)/1333.0));//left
	_scene->CompartmentName[m_pathwayID].push_back("cytosol");
	_scene->CompartmentContain[m_pathwayID].push_back(empty);

	Compartment.push_back(QRectF(80/2587.0, 185/1333.0, (379-80)/2587.0, (396-185)/1333.0)); //inside left
	_scene->CompartmentName[m_pathwayID].push_back("Golgi lumen");
	_scene->CompartmentContain[m_pathwayID].push_back(empty);

	Compartment.push_back(QRectF(80/2587.0, 185/1333.0, (379-80)/2587.0, (396-185)/1333.0)); //inside left
	_scene->CompartmentName[m_pathwayID].push_back("Golgi Golgi membrane");
	_scene->CompartmentContain[m_pathwayID].push_back(empty);

	Compartment.push_back(QRectF(530/2587.0, 241/1333.0, (2515-530)/2587.0, (1278-241)/1333.0));  //right
	_scene->CompartmentName[m_pathwayID].push_back("cytosol");
	_scene->CompartmentContain[m_pathwayID].push_back(empty); 
	
	Compartment.push_back(QRectF(489/2587.0, 103/1333.0, (2581-489)/2587.0, (1323-103)/1333.0)); //inside right
	_scene->CompartmentName[m_pathwayID].push_back("");
	_scene->CompartmentContain[m_pathwayID].push_back(empty);
	
	Compartment.push_back(QRectF(865/2587.0, 584/1333.0, (1519-865)/2587.0, (863-584)/1333.0));  //inside right
	_scene->CompartmentName[m_pathwayID].push_back("early endosome membrane");
	_scene->CompartmentContain[m_pathwayID].push_back(empty);

	Compartment.push_back(QRectF(1177/2587.0, 669/1333.0, (1307-1177)/2587.0, (737-669)/1333.0)); //inside inside
	_scene->CompartmentName[m_pathwayID].push_back("early endosome");
	_scene->CompartmentContain[m_pathwayID].push_back(empty);

	Compartment.push_back(QRectF(1206/2587.0, 924/1333.0, (2171-1206)/2587.0, (1269-924)/1333.0)); //inside right
	_scene->CompartmentName[m_pathwayID].push_back("nucleoplasm");
	_scene->CompartmentContain[m_pathwayID].push_back(empty);

	Compartment.push_back(QRectF(530/2587.0, 241/1333.0, (2515-530)/2587.0, (1278-241)/1333.0));  //inside right
	_scene->CompartmentName[m_pathwayID].push_back("plasma membrane");
	_scene->CompartmentContain[m_pathwayID].push_back(empty);
	
	Compartment.push_back(QRectF(530/2587.0, 241/1333.0, (2515-530)/2587.0, (1278-241)/1333.0)); 
	_scene->CompartmentName[m_pathwayID].push_back("extracellular region");
	_scene->CompartmentContain[m_pathwayID].push_back(empty);
	
	Compartment.push_back(QRectF(489/2587.0, 103/1333.0, (2581-489)/2587.0, (1323-103)/1333.0)); //inside right
	_scene->CompartmentName[m_pathwayID].push_back("cell junction");
	_scene->CompartmentContain[m_pathwayID].push_back(empty);
}


void PathBubble1::testforceDirected(QPainter *painter, QRectF Compartment)
{
	
	/*float width=Compartment.width(); //inside right
	float height=Compartment.height(); 
	float x=Compartment.x(),y=Compartment.y();

    int iterations = 100;
	vector<QPoint> edges;
	vector<int> tg(2,0);

	Graph g(8);
 
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
	 //paintComplex( painter, node, _scene->_6complexName[*vi]);
  }

  nodeid.clear();
  */

}

void PathBubble1::preLocateNodes(vector<QRectF> &complexPos, vector<QRectF> &proteinPos, vector<QRectF> &smallMoleculePos)	
{

	for(int i=1; i<complexPos.size();i++)
	{
		QSizeF size=getItemSize('C', i);	
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

	for(int i=1; i<proteinPos.size();i++)
	{
		QSizeF size=getItemSize('P', i);	
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

	for(int i=1; i<physicalEntityPos.size();i++)
	{
		QSizeF size=getItemSize('E', i);	
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

	for(int i=1; i<DnaPos.size();i++)
	{
		QSizeF size=getItemSize('D', i);
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

	for(int i=1; i<EmptyPos.size();i++)
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
	}

	for(int i=1; i<smallMoleculePos.size();i++)
	{
		QSizeF size=getItemSize('S', i);	
		smallMoleculePos[i].setX(0);
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
	smallMoleculePos[1].push_back(QRectF(563/2563.0, 600/1326.0, 0,0));

	//ADP
	smallMoleculePos[2].push_back(QRectF(563/2563.0, 650/1326.0, 0,0));	

	//H2O
	smallMoleculePos[3].push_back(QRectF(831/2563.0, 713/1326.0, 0,0));	

	//Pi
	smallMoleculePos[4].push_back(QRectF(808/2563.0, 602/1326.0, 0,0));	
	*/
	
	
	for(int i=1; i<complexPos.size();i++)
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

	for(int i=1; i<proteinPos.size();i++)
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

	for(int i=1; i<physicalEntityPos.size();i++)
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

	for(int i=1; i<DnaPos.size();i++)
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

	for(int i=1; i<smallMoleculePos.size();i++)
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




vector<vector<vector<int>>> PathBubble1::recordEdge(vector<vector<vector<int>>> _4biochemicalReaction)
{
	vector<vector<vector<int>>> edge;

/*
	 start[0]=edge[i][0], start[1]=edge[i][1];	
		 		 
		 middle[0]=edge[i][4]; middle[1]=edge[i][5]; 
		 
		 end[0]=edge[i][2], end[1]=edge[i][3]; 
		 */

	return edge;
}


void PathBubble1::trackstep(int id, vector<vector<int>> _2pathwayStep, vector<vector<int>> _3pathwayStepCatalysis, vector<int> _3pathwayStepReaction, vector<vector<vector<int>>> _4biochemicalReaction)
{
	static int count=0;
	int nid;//next step

	if(stepTracked.find(id)==stepTracked.end())
	    stepTracked.insert(id);
	else 
		return;

		
	count++;	
	QRectF rPos;

 
	if(_3pathwayStepReaction[id]!=-1)
	   rPos=LayoutBiochemicalReaction(_scene->_4biochemicalReaction[m_pathwayID], _3pathwayStepReaction[id], id);

	if(_3pathwayStepCatalysis[id][0]!=-1)
	   LayoutCatalysis(_5Catalysis, rPos, _3pathwayStepCatalysis[id][0], id);	

	if(_3pathwayStepCatalysis[id][1]!=-1)
	   LayoutControl(_5Control, rPos, _3pathwayStepCatalysis[id][1], id);	

	if(_2pathwayStep[id].empty())
		return;
		
	for(int i=0; i<_2pathwayStep[id].size(); i++)
	{
		nid = _2pathwayStep[id][i];	
		trackstep(nid, _2pathwayStep, _3pathwayStepCatalysis, _3pathwayStepReaction, _scene->_4biochemicalReaction[m_pathwayID]);
	}
	
}

void PathBubble1::LayoutControl(vector<vector<int>> _5Control,  QRectF rPos, int cid, int sid)
{
	int type,id,type2,id2;
	float DY=0.05;
	bool flag=1;
	
	vector<int> aedge(7,-1);
	type=_5Control[cid][0], id=_5Control[cid][1];

	QSizeF size=getItemSize(type, id);	
	QRectF point(rPos.x()  - size.width()/2 , rPos.y() - DY  - size.height()/2, size.width(), size.height());

	if(type=='P')
	{
		if(proteinPos[id].x()==-1)
	    {
			////temporily disabled
			//proteinPos[id]=point;   	
			flag=0;
		}
		//else complexPos[id].setX(complexPos[id].x()-point.width()/2.0/this->Width()), complexPos[id].setY(complexPos[id].y()-point.height()/2.0/this->Height()), complexPos[id].setWidth(point.width()), complexPos[id].setHeight(point.height()); 
	}
	else if(type=='C')
	{
		if(complexPos[id].x()==-1)
	    {
			//temporily disabled
			//complexPos[id]=point;  
			flag=0;
		}
		//else complexPos[id].setX(complexPos[id].x()-point.width()/2.0/this->Width()), complexPos[id].setY(complexPos[id].y()-point.height()/2.0/this->Height()), complexPos[id].setWidth(point.width()), complexPos[id].setHeight(point.height()); 		
	}
	else flag=0;

	if(flag)
	{
		type2=_5Control[cid][2], id2=_5Control[cid][3];	
		if(type2=='R')
	    {
			aedge[0]=type, aedge[1]=id;
			aedge[2]=type2, aedge[3]=id2;
			aedge[4]=type2;
			aedge[5]=id2;
			aedge[6]=whichPathway(sid);
			edge.push_back(aedge);		 
	    }
	    else if(type2=='D')
	    {
		
	    }
	    //_5Catalysis[cid][4]//ACTIVATION or //INHIBITION 
	}
}

void PathBubble1::LayoutCatalysis(vector<vector<int>> _5Control,  QRectF rPos, int cid, int sid)
{
	int type,id,type2,id2;
	float DY=0.05;
	bool flag=1;
	
	vector<int> aedge(7,-1);
	type=_5Catalysis[cid][0], id=_5Catalysis[cid][1];

	QSizeF size=getItemSize(type, id);	
	QRectF point(rPos.x()  - size.width()/2 , rPos.y() - DY  - size.height()/2, size.width(), size.height());

	if(type=='P')
	{
		if(proteinPos[id].x()==-1)
	    {
			////temporily disabled
			//proteinPos[id]=point;   	
			flag=0;
		}
		//else complexPos[id].setX(complexPos[id].x()-point.width()/2.0/this->Width()), complexPos[id].setY(complexPos[id].y()-point.height()/2.0/this->Height()), complexPos[id].setWidth(point.width()), complexPos[id].setHeight(point.height()); 
	}
	else if(type=='C')
	{
		if(complexPos[id].x()==-1)
	    {
			//temporily disabled
			//complexPos[id]=point;  
			flag=0;
		}
		//else complexPos[id].setX(complexPos[id].x()-point.width()/2.0/this->Width()), complexPos[id].setY(complexPos[id].y()-point.height()/2.0/this->Height()), complexPos[id].setWidth(point.width()), complexPos[id].setHeight(point.height()); 		
	}
	else flag=0;

	if(flag)
	{
		type2=_5Catalysis[cid][2], id2=_5Catalysis[cid][3];	
		if(type2=='R')
	    {
			aedge[0]=type, aedge[1]=id;
			aedge[2]=type2, aedge[3]=id2;
			aedge[4]=type2;
			aedge[5]=id2;
			aedge[6]=whichPathway(sid);
			edge.push_back(aedge);		 
	    }
	    else if(type2=='D')
	    {
			//?????
			//Degradation	
	    }
	    //_5Catalysis[cid][4]//ACTIVATION or //INHIBITION 
	}
}


QPointF PathBubble1::getLocation(int pathwayid, int stepid)
{
	float DX=0.02,DY=0.04;
	return( QPointF (stepid*DX+0.01, pathwayid*DY+0.1) );	
}

QRectF PathBubble1::LayoutBiochemicalReaction(vector<vector<vector<int>>> _4biochemicalReaction, int bid, int sid)
{
	  vector<int> left;
	  vector<int> right;	  
	  vector<int> aedge(7,-1);
	  bool flag=1;
	  int count=0;
	  
	  QPointF Pos=getLocation(0, sid);
	  //QPointF lPos(0,-1), rPos(0,-1);

	  left=_scene->_4biochemicalReaction[m_pathwayID][bid][0];
	  right=_scene->_4biochemicalReaction[m_pathwayID][bid][1];
	 
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

int PathBubble1::read2pathwayStepOrder(const char *name, int stepNum, vector<vector<int>> &_2pathwayStep)
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

	_2pathwayStep.clear();
	vector<int> empty;
	vector<vector<int>> temp(stepNum+1,empty);
	_2pathwayStep=temp;

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

		  _2pathwayStep[pid].push_back(cid);			  
		  
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


vector<int> PathBubble1::read4biochemicalReaction(const char *name, vector<vector<vector<int>>> &_4biochemicalReaction, vector<vector<QString>> &_4reactionName, vector<vector<vector<int>>> &_4degradation, vector<vector<QString>> &_4degradationName, vector<vector<QString>> &_4EmptyName)
{   //complex/Dna/Physical Entity/protein/small molecule
	char ch[100];
	char c1,c2,o1,o2,c12,c22,o12,o22;
	FILE *fp = fopen(name,"r"); 
	//vector<vector<QString>> _4EmptyName;
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
				  //_4biochemicalReaction[bid][1].push_back('N');				  
			      //_4biochemicalReaction[bid][1].push_back(1);				  
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

	_4biochemicalReaction.clear();
	vector<int> empty;	
	vector<vector<int>> temp2(2,empty);
	vector<vector<vector<int>>> temp3(max+1,temp2);
	_4biochemicalReaction=temp3;

	vector<vector<vector<int>>> temp4(demax+1,temp2);
	_4degradation=temp4;

	 QString a;
	vector<QString> empty3(2,a);
	vector<vector<QString>> temps(max+1,empty3);
	_4reactionName = temps;

	vector<vector<QString>> temps4(demax+1,empty3);
	_4degradationName = temps4;

      vector<QString> tempE; 
      _4EmptyName.push_back(tempE);
	//vector<vector<QString>> tempN(nmax+1,empty);
    //_4EmptyName = tempN;

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
			 _4reactionName[bid][1]=buffer[1].c_str();
		  else if(buffer[0][0]=='D')
			 _4degradationName[bid][1]=buffer[1].c_str();

	      //fscanf(fp,"Biochemical Reaction%d",&bid);		  	     
		  //fscanf(fp," %c",&c1);		  
		  if(c1=='C')
		  {
			  //fscanf(fp,"omplex%d", &lid);
			  if(buffer[0][0]=='B')
			  {	
				_4biochemicalReaction[bid][0].push_back('C');	
				_4biochemicalReaction[bid][0].push_back(lid);				  
			  }
			  else if(buffer[0][0]=='D')
			  {
			    _4degradation[bid][0].push_back('C');	
				_4degradation[bid][0].push_back(lid);	
			  }
		  }
		  else if(c1=='D')
		  {
			  //fscanf(fp,"na%d", &lid);	
			  Dna.push_back(lid);
			   if(buffer[0][0]=='B')
			   {
				   _4biochemicalReaction[bid][0].push_back('D');					  
			       _4biochemicalReaction[bid][0].push_back(lid);					  
			   }
			    else if(buffer[0][0]=='D')
			  {
			    _4degradation[bid][0].push_back('D');	
				_4degradation[bid][0].push_back(lid);	
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
					_4biochemicalReaction[bid][0].push_back('E');	
					_4biochemicalReaction[bid][0].push_back(lid);		
				  }
				  else if(buffer[0][0]=='D')
				  {
					_4degradation[bid][0].push_back('E');	
					_4degradation[bid][0].push_back(lid);	
				  }
		      }
			  else if(o1=='r')
			  {
				  //fscanf(fp,"otein%d", &lid);	
				  if(buffer[0][0]=='B')
				  {
					_4biochemicalReaction[bid][0].push_back('P');	
					_4biochemicalReaction[bid][0].push_back(lid);		
				  }
				  else if(buffer[0][0]=='D')
				  {
					_4degradation[bid][0].push_back('P');	
					_4degradation[bid][0].push_back(lid);	
				  }
			  }			  
		  }
          else if(c1=='S')
		  {
			  //fscanf(fp,"mall Molecule%d", &lid);	
			  smallMolecule.push_back(lid);
			  if(buffer[0][0]=='B')
			  {
				  _4biochemicalReaction[bid][0].push_back('S');				  
				_4biochemicalReaction[bid][0].push_back(lid);				  
			  }
			  else if(buffer[0][0]=='D')
			  {
					_4degradation[bid][0].push_back('S');	
					_4degradation[bid][0].push_back(lid);	
			  }
		  }	   
		  //fscanf(fp," %c",&c2);	
		  if(c2=='C')
		  {
			  //fscanf(fp,"omplex%d", &rid);	
			  _4biochemicalReaction[bid][1].push_back('C');				  
			  _4biochemicalReaction[bid][1].push_back(rid);				  
		  }
		   else if(c2=='D')
		  {
			  //fscanf(fp,"na%d", &rid);
			  Dna.push_back(rid);
			  _4biochemicalReaction[bid][1].push_back('D');				  
			  _4biochemicalReaction[bid][1].push_back(rid);				  				  
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
				  _4biochemicalReaction[bid][1].push_back('E');				  
			      _4biochemicalReaction[bid][1].push_back(rid);				  	
		      }
			  else if(o2=='r')
			  {
				  //fscanf(fp,"otein%d", &rid);		
				  _4biochemicalReaction[bid][1].push_back('P');				  
			      _4biochemicalReaction[bid][1].push_back(rid);				  	
			  }			  
		  }
          else if(c2=='S')
		  {
			  //fscanf(fp,"mall Molecule%d", &rid);	
			  smallMolecule.push_back(rid);
			  _4biochemicalReaction[bid][1].push_back('S');				  
			  _4biochemicalReaction[bid][1].push_back(rid);				  
		  }	
		  else if(c2=='n') //empty for some case
		  {
			  if(buffer[0][0]=='B')
			  {
				  vector<QString> tempE; tempE.push_back(buffer[1].c_str());
				  _4EmptyName.push_back(tempE);
				  _4biochemicalReaction[bid][1].push_back('N');				  
			      _4biochemicalReaction[bid][1].push_back(_4EmptyName.size());				  
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

	 for(int i=0; i<_4reactionName.size(); i++)
	 {
	    int lcount=1, rcount=1;
		for(int j=2; j<_4biochemicalReaction[i][0].size(); j=j+2)	    
		{
		  if(_4biochemicalReaction[i][0][j]!=_4biochemicalReaction[i][0][j-2] || _4biochemicalReaction[i][0][j+1]!=_4biochemicalReaction[i][0][j-1])
		    lcount++;
		}	 
		for(int j=2; j<_4biochemicalReaction[i][1].size(); j=j+2)	    
		{
		  if(_4biochemicalReaction[i][1][j]!=_4biochemicalReaction[i][1][j-2] || _4biochemicalReaction[i][1][j+1]!=_4biochemicalReaction[i][1][j-1])
		    rcount++;
		}	
		if(lcount==rcount)
              _4reactionName[i][0]='T';//transition/process
		else 
		{
			if(lcount==1)
               _4reactionName[i][0]='D';  //disscociation
		    else if(rcount==1)
			  _4reactionName[i][0]='B';  //bind
			else  _4reactionName[i][0]='T';//transition/process
		}
	 }
	 return empty;
}


void PathBubble1::read5catalysisANDcontrol(const char *name, vector<vector<int>> &_5Catalysis, vector<vector<int>> &_5Control)
{//return maximum step
	char ch[100];
	char C, c1, c2, C2;
	FILE *fp;
	char e;	
	int cid, mid, bid, cid2, mid2, bid2;
	int count=-1;
	int max1=0,max2=0;
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
	_5Catalysis=temp1;
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
				  _5Catalysis[cid][0]='P';
				  _5Catalysis[cid][1]=mid;
			  }
			  else if(c1=='C')
			  {
				  //fscanf(fp,"omplex%d", &mid);
				  _5Catalysis[cid][0]='C';
				  _5Catalysis[cid][1]=mid;
			  }

			  //fscanf(fp," %c", &c2);				  
			  if(c2=='B')
			  {
				  //fscanf(fp,"iochemical Reaction%d", &bid);
				  _5Catalysis[cid][2]='R';
				  _5Catalysis[cid][3]=bid;
			  }
			  else if(c2=='D')
			  {
				  //fscanf(fp,"egradation%d", &bid);
				  _5Catalysis[cid][2]='D';
				  _5Catalysis[cid][3]=bid;
			  }

			  //fscanf(fp," %s", ch);
			  if(buffer[3][0]=='A')
				  _5Catalysis[cid][4]=1;
			  else _5Catalysis[cid][4]=0;
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

int PathBubble1::read6complex(const char *name, vector<QString> &_6complexName, vector<vector<int>> &_6complexContain)
{
	char ch[100];
	char c,o,c2,o2;
	char e;	
	string cname;
	FILE *fp = fopen(name,"r"); 
	
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
		  //fscanf(fp," %c",&c1);	
		  //C=buffer[0][1];
		  //cid=buffer[3][0];
		  c=buffer[3][0];
		  o=buffer[3][1];

		  			
		 if(cid>max)
			    max=cid; 
	        
	 }while(true);
	 fclose(fp);	

	QString a;
	vector<QString> temps(max+1,a);
	_6complexName = temps;
	vector<int> empty;
	vector<vector<int>> temp(max+1,empty);
	_6complexContain = temp;	
		
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


			_6complexName[cid]= buffer[1].c_str();

			//?complexLocation 
			//fgets(ch, 30, fp); 
			cname=buffer[2].c_str();

			for(int i=0;i<_scene->CompartmentName[m_pathwayID].size();i++)
			{
			   if(_scene->CompartmentName[m_pathwayID][i]==cname.c_str())
			   {
				   vector<int> item;
				   item.push_back('C'); item.push_back(cid);
			      _scene->CompartmentContain[m_pathwayID][i].insert(item);		
				   break;
			   }			
			}
				
			if(c=='C')
		    {
				 
				 _6complexContain[cid].push_back('C');
				 _6complexContain[cid].push_back(pid);						  
		    }		   
		    else if(c=='P')
		    {
			    if(o=='h')
			    {
				  _6complexContain[cid].push_back('E');
				  _6complexContain[cid].push_back(pid);		
		        }
			    else if(o=='r')
			    {
				   _6complexContain[cid].push_back('P');
				   _6complexContain[cid].push_back(pid);				
			    }			  
		    }		 		    
	 }while(true);

	 fclose(fp);	
	 return max;
}

int PathBubble1::read7protein(const char *name, vector<vector<QString>> &_7proteinName)
{
	char ch[255], ch2[255];	
	string n1,n2,n12,n22,n3;
	char e;	
	FILE *fp = fopen(name,"r"); 

	int pid, pid2;
	int max=-100000;

	pid2=-1;
	n12='x',n22='x';	
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
		  for(int i=0; i<buffer.size(); i++)
		  {
			  string result;
		      for(unsigned int j = 0; j < buffer[i].size();j++)
              if(isdigit(buffer[i][j]))
              {   
				  result.append(1,buffer[i][j]);                 
			  }	
			  if(i==0)	 pid= atoi(result.c_str());  			  
		  }     		  		
		  if(pid>max)
			  max=pid;	
	 }while(true);
	 fclose(fp);	

	 QString a;
	vector<QString> empty(2,a);
	vector<vector<QString>> temps(max+1,empty);
	_7proteinName = temps;
		
	fp = fopen(name,"r"); 
	do
	{	 
		//fscanf(fp,"Protein%d",&pid);				 
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
		  for(int i=0; i<buffer.size(); i++)
		  {
			  string result;
		      for(unsigned int j = 0; j < buffer[i].size();j++)
              if(isdigit(buffer[i][j]))
              {   
				  result.append(1,buffer[i][j]);                 
			  }	
			  if(i==0)	 
				  pid= atoi(result.c_str());  			 
		  }     

		    n3=buffer[4].c_str();
			for(int i=0;i<_scene->CompartmentName[m_pathwayID].size();i++)
			{
			   if(_scene->CompartmentName[m_pathwayID][i]==n3.c_str())
			   {
				   vector<int> item;
				   item.push_back('P'); item.push_back(pid);
			       _scene->CompartmentContain[m_pathwayID][i].insert(item);		
				   break;
			   }			
			}
					
			_7proteinName[pid][0]=buffer[1].c_str();
			_7proteinName[pid][1]=buffer[2].c_str();
			if(pid>max)
			    max=pid;
			
	      

	 }while(true);
	 fclose(fp);		 	
	 return max;
}


void PathBubble1::read8convertedEntity(const char *name, int &proteinNum, int &complexNum, int &physicalEntityNum, vector<vector<int>> &_8convertedProtein, vector<vector<int>> &_8convertedComplex, vector<vector<int>> &_8convertedEntity)
{
	char ch[100];
	char c,d,o,c2,d2;
	char e;
	string cname,n1;
	FILE *fp; 
	int lid, rid, lid2, rid2;
	int pmax=proteinNum, cmax=complexNum, emax=physicalEntityNum;

	fp = fopen(name,"r"); 


	lid2=-1, rid2=-1;
	c2='x', d2='x';
	int count=0;
	do
	{
		    //fscanf(fp," %c",&c);	
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
			  if(i==0)	 lid= atoi(result.c_str());  	
			  else if(i==3) rid=atoi(result.c_str()); 
		  }     
		  c=buffer[0][0];
			d=buffer[3][0];
			if(c=='C')
		    {
				 
				n1=buffer[1];
				for(int i=0;i<_scene->CompartmentName[m_pathwayID].size();i++)
					{
						if(_scene->CompartmentName[m_pathwayID][i]==n1.c_str())
						{
							vector<int> item;
							item.push_back('C'); item.push_back(lid);
							_scene->CompartmentContain[m_pathwayID][i].insert(item);		
							break;
						}			
				   }

				
				 if(d=='C')
				 {					
					if(rid>cmax)
			            cmax=rid;
				 }
				 else if(d=='P')
		         {
			       if(rid>pmax)
			            pmax=rid;
		         }	
				 if(lid>cmax)
			       cmax=lid;
			 
			}
		    else if(c=='P')
		    {
			    o=buffer[0][1];
			    if(o=='h')
			    {
					
					n1=buffer[1];
					for(int i=0;i<_scene->CompartmentName[m_pathwayID].size();i++)
					{
						if(_scene->CompartmentName[m_pathwayID][i]==n1.c_str())
						{
							vector<int> item;
							item.push_back('E'); item.push_back(lid);
							_scene->CompartmentContain[m_pathwayID][i].insert(item);		
							break;
						}			
				   }
				   
					if(d=='C')
				    {
						if(rid>cmax)
			            cmax=rid;
				    }
				    else if(d=='P')
		            {
						if(rid>pmax)
			              pmax=rid;
		            }	
					if(lid>emax)
			         emax=lid;
		        }
			    else if(o=='r')
			    {					
					
					n1=buffer[1];
					for(int i=0;i<_scene->CompartmentName[m_pathwayID].size();i++)
					{
						if(_scene->CompartmentName[m_pathwayID][i]==n1.c_str())
						{
							vector<int> item;
							item.push_back('P'); item.push_back(lid);
							_scene->CompartmentContain[m_pathwayID][i].insert(item);		
							break;
						}			
				   }
				   
					if(d=='C')
				    {
												
						if(rid>cmax)
			            cmax=rid;
				    }
				    else if(d=='P')
		            {						
						
						if(rid>pmax)
			              pmax=rid;
		            }	
					if(lid>pmax)
			          pmax=lid;
			    }
				 
		    }	
		
	 }while(true);

	fclose(fp);

	if(pmax>proteinNum)
		proteinNum=pmax;
	
	if(cmax>complexNum)
		complexNum=cmax;

	if(emax>physicalEntityNum)
		physicalEntityNum=emax;

	vector<int> unit;
	vector<vector<int>> temp1(proteinNum+1,unit);
	vector<vector<int>> temp2(complexNum+1,unit);
	vector<vector<int>> temp3(emax+1,unit);
	_8convertedProtein=temp1;
	_8convertedComplex=temp2;
	_8convertedEntity=temp3;
	

    fp = fopen(name,"r"); 
	

	lid2=-1, rid2=-1;
	c2='x', d2='x';
	count=0;
	do
	{//fscanf(fp," %c",&c);	
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
			  if(i==0)	 lid= atoi(result.c_str());  	
			  else if(i==3) rid=atoi(result.c_str()); 
		  }     
		    c=buffer[0][0];
			d=buffer[3][0];
		    
		    if(c=='C')
		    {
				 if(d=='C')
				 {
					
					_8convertedComplex[lid].push_back('C');
				    _8convertedComplex[lid].push_back(rid);						  
				 }
				 else if(d=='P')
		         {
			        _8convertedComplex[lid].push_back('P');
				    _8convertedComplex[lid].push_back(rid);	
		         }	
			}
		    else if(c=='P')
		    {
				o=buffer[0][1];
			    if(o=='h')
			    {
					if(d=='C')
				    {
						_8convertedEntity[lid].push_back('C');
						_8convertedEntity[lid].push_back(rid);						  
				    }
				    else if(d=='P')
		            {
						_8convertedEntity[lid].push_back('P');
						_8convertedEntity[lid].push_back(rid);	
		            }		
		        }
			    else if(o=='r')
			    {
					
					if(d=='C')
				    {
						
						_8convertedProtein[lid].push_back('C');
						_8convertedProtein[lid].push_back(rid);						  
				    }
				    else if(d=='P')
		            {
						
						_8convertedProtein[lid].push_back('P');
						_8convertedProtein[lid].push_back(rid);	
		            }			
			    }			  
		    }		 
		   
	 }while(true);

	 fclose(fp);	
	 //return emax;

}


void PathBubble1::read9smallEntity(const char *name, vector<vector<QString>> & _9smallMoleculeName, vector<vector<QString>> &_9DnaName)
{
char ch[255], ch2[255];	
char c;
char e;	
	string n1,n2,n12,n22,n3;
	FILE *fp = fopen(name,"r"); 
	
	int pid, pid2;
	char c2;
	int Dmax=0, Smax=0;

	pid2=-1; c2='x';
	n12='x',n22='x';	
	do
	{
		 if(pid==8)
			  pid=pid;
		//fscanf(fp,"%c",&c);
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
			  if(i==0)	 pid = atoi(result.c_str());  				  
		  }     
		  c=buffer[0][0];
	
		if(c=='D')
		{
			
			if(pid>Dmax)
		    Dmax=pid;
		}
		else if(c=='S')
		{
			if(pid>Smax)
		    Smax=pid;
		}	
		
	 }while(true);
	 fclose(fp);	

	 QString a;
	vector<QString> empty(2,a);
	vector<vector<QString>> temps(Smax+1,empty);
	vector<vector<QString>> tempd(Dmax+1,empty);
	
	_9DnaName = tempd;
	 _9smallMoleculeName  = temps;
	

	 if(Smax>smallMoleculeNum) 
		 smallMoleculeNum=Smax;

	  if(Dmax>DnaNum) 
		 DnaNum=Dmax;

	fp = fopen(name,"r"); 

	pid2=-1;
	n12='x',n22='x';	

	do
	{
		//fscanf(fp,"%c",&c);
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
			  if(i==0)	 pid = atoi(result.c_str());  				  
		  }     
		  c=buffer[0][0];			
		  
		  n3=buffer[4];
			for(int i=0;i<_scene->CompartmentName[m_pathwayID].size();i++)
			{
			   if(_scene->CompartmentName[m_pathwayID][i]==n3.c_str())
			   {
				   vector<int> item;
				   
				   item.push_back(c); item.push_back(pid);
			       _scene->CompartmentContain[m_pathwayID][i].insert(item);		
				   break;
			   }			
			}
		
			n1=buffer[1];
			n2=buffer[2];
			if(c=='D')
		    {
				_9DnaName[pid][0]=n1.c_str();
				_9DnaName[pid][1]=n2.c_str();
		    }
		    else if(c=='S')
			{
				 _9smallMoleculeName[pid][0]=n1.c_str();
				 _9smallMoleculeName[pid][1]=n2.c_str();
			}	

	 }while(true);
	 fclose(fp);	 		 
}



void PathBubble1::getNameforConverted(vector<vector<int>> _8convertedProtein, vector<vector<int>> _8convertedComplex, vector<vector<int>> _8convertedEntity, vector<QString> &_6complexName, vector<vector<QString>> &_7proteinName, vector<vector<QString>> &_8physicalEntityName)
{
	//void &_6complexName, vector<vector<QString>> &_7proteinName, vector<vector<QString>> &_8physicalEntityName
	//_7proteinName
	QString at('/');

	for(int i=0;i<_8convertedProtein.size();i++)
	{
		QString temp,temp1;
		
		if(!_8convertedProtein[i].empty())
		{
			for(int j=0; j<_8convertedProtein[i].size(); j=j+2)
			{
				int type=_8convertedProtein[i][j], id=_8convertedProtein[i][j+1];		    
		        if(type=='P')
		        {
                  if(!temp.isEmpty()&&!_7proteinName[id][0].isEmpty())
				     temp.append(at);
				  
				  temp.append(_7proteinName[id][0]); temp1=_7proteinName[id][1];
				  if(temp.size()==0)
				  { 
					 while(!_8convertedProtein[id].empty())
					 {						
						type=_8convertedProtein[id][0], id=_8convertedProtein[id][1];
						if(type=='P')
						{
							if(!temp.isEmpty()&&!_7proteinName[id][0].isEmpty())
				                      temp.append(at);
				  	       temp.append(_7proteinName[id][0]); temp1=_7proteinName[id][1];
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
			_7proteinName[i][0].append(temp); _7proteinName[i][1]=temp1;
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
				
				if(!temp.isEmpty()&&!_6complexName[id].isEmpty())
				         temp.append(at);
				
				temp.append(_6complexName[id]); 
				if(temp.size()==0)
				{
					while(!_8convertedComplex[id].empty())
					{						
						type=_8convertedComplex[id][0], id=_8convertedComplex[id][1];
						if(type=='C')
						{
							if(!temp.isEmpty()&&!_6complexName[id].isEmpty())
				                  temp.append(at);
						   temp.append(_6complexName[id]); 
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
			_6complexName[i].append(temp); 
		}
		
	}

	QString a;
	vector<QString> empty(2,a);
	vector<vector<QString>> temps(physicalEntityNum+1,empty);
	_8physicalEntityName = temps;

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
					if(!temp.isEmpty()&&!_7proteinName[id][0].isEmpty())
				            temp.append(at);
					temp.append(_7proteinName[id][0]); temp1=_7proteinName[id][1];
					if(temp.size()==0)
					{
						while(!_8convertedProtein[id].empty())
						{						
							type=_8convertedProtein[id][0], id=_8convertedProtein[id][1];
							if(type=='P')
							{
								if(!temp.isEmpty()&&!_7proteinName[id][0].isEmpty())
				                      temp.append(at);
								temp.append(_7proteinName[id][0]); temp1=_7proteinName[id][1];
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
			_8physicalEntityName[i][0].append(temp);  _8physicalEntityName[i][1]=temp1; 
		}

		
	}

}

void PathBubble1::deleteSelectedItems(QGraphicsScene *scene)
{
	if (!scene)
		return;

	deleteSelectedItemKids(scene);
	ItemBase::deleteSelectedItems(scene);	
}

void PathBubble1::deleteSelectedItemKids(QGraphicsScene *scene)
{
	//for (int i = 0; i < _codeBubbles.size(); i ++)
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
	}
	//_groupBubbles.clear();
	//_groupPoint.clear();	
}

void PathBubble1::updateLabel()
{
	if (_LABEL == false)
	{
		_label = new ItemLabel( this, _scene, m_size_width, m_size_height );
		_label->setLabelColor(_colorBoarder);
		this->_scene->addItem(_label);
		_LABEL = true;

		//emit updateLabelState( this );

		return;
	}
	_label->updateLabel( m_size_width, m_size_height );

	//_label->setPos( _label->pos() + QPointF(0, 12) );
		
	
	//if (this->_codeParent)
	{
		_label->setPos( _label->pos() + QPointF(0, 10) );
		_label->setPlainText( this->_curFileName );
	}
} 


void PathBubble1::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
	updateLabel();

	if(sharedProtein.size()<_scene->_7proteinName.size())
	{		
		searchSharedProtein();
	}
	
	QPointF start,middle,end;
	vector<int> temp(2,0);
	PlotBase::paint( painter, option, widget );
	//paintPathWays(painter, _scene->PathWayNames);	
	float width=this->realRect().width()/2*_scale;
	float height=this->realRect().height()/2*_scale;
	set<vector<int> > reactionRecord; 
	vector<int> node(2,0);
	vector<vector<int>> ed(2, node);

	painter->setClipRect(realRect());

	painter->setClipping(true);

	//make text as a path or drawtext can solve the "clipping not work for textdrawing issue"	/ doc.draw does not work with clipping
	 //painter->drawText(dCenter.x(), dCenter.y(), QString("Qt")); 

	 
	//setFlag(QGraphicsItem::ItemClipsToShape, true);	
		
	painter->setRenderHint(QPainter::Antialiasing, true);

	/*
	for(int i=0; i<_polyEdge.size(); i++)
	{
		vector<QPointF> etemp;
		for(int j=0; j<_polyEdge[i].size()-1; j++)
	    {
		  start=_polyEdge[i][j];
		  end=_polyEdge[i][j+1];

          start=QPointF(start.x()*width*2-width, start.y()*height*2-height);
		  end=QPointF(end.x()*width*2-width, end.y()*height*2-height);
		  if(j==_polyEdge[i].size()-2)
	         drawArrow(painter, start+dCenter, end+dCenter, QColor(0, 0, 0, 255) );		 		  
		  else painter->drawLine(start+dCenter,end+dCenter);
		}		
	}

	
	for(int i=0; i<_newPos.size(); i++)
	{
	    if(_newPos[i].x()>=-0.99)
		{		   
		   painter->drawRect(_newPos[i].x()*width*2-width+dCenter.x(),_newPos[i].y()*height*2-height+dCenter.y(),4,4);
		}
	}

	for(int i=0; i<_Edge.size(); i++)
	{
	   int vid0=_Edge[i][0],vid1=_Edge[i][1];
	   if(_newPos[vid0].x()>=-0.99&&_newPos[vid1].x()>=-0.99)
	   {
		  start=QPointF(_newPos[vid0].x()*width*2-width, _newPos[vid0].y()*height*2-height);
		  end=QPointF(_newPos[vid1].x()*width*2-width, _newPos[vid1].y()*height*2-height);
	      drawArrow(painter, start+dCenter, end+dCenter, QColor(0, 0, 0, 255) );		 
	   }
	}
	
	*/
	//return;

	painter->setBrush(Qt::NoBrush);

	//1 draw compartments	
	float x,y,w,h;
	QPen pen(QColor(255,157,108,255));
	pen.setWidth(2);
	painter->setPen(pen);	
	
	QFont f("Arial", 10);
	painter->setFont (f);

	for(int i=0; i<compartmentPos.size()-1; i++) //size?
	{
		/*x=compartmentPos[i].x()*width*2-width; y=compartmentPos[i].y()*height*2-height;
		w=compartmentPos[i].width()*width*2;   h=compartmentPos[i].height()*height*2;
		QRect rect = QRect(x+dCenter.x(), y+dCenter.y(), w, h);*/
		paintCompartment( painter, _newCompartmentPos, scene->CompartmentName[m_pathwayID], i, dCenter, _scale);
		//painter->drawRoundedRect( rect,6,6);	//rect
		//painter->drawText(rect.x() + 8, rect.y() + rect.height() - 10 , CompartmentName[i]);	
	}
	
    /*for(int i=0; i<compartmentPos.size(); i++)//_newCompartmentPos.size()
	{	
		painter->drawRect(_newCompartmentPos[i].x()*width*2-width,_newCompartmentPos[i].y()*height*2-height,_newCompartmentPos[i].width()*width*2, _newCompartmentPos[i].height()*height*2);	

	}*/

	//outside
	//boundary
	//inside
	
	//2 complex

	//3 protein
		
	QRectF s,m,e;
	int type, id;

	//4 draw edge
	//reaction first
	for(int i=0; i<edge.size(); i++)
	{
         //painter->drawRect(reactionPos[i].x(),reactionPos[i].y(),2,2);	
		
		if(edge[i][2]=='R'&&edge[i][4]=='R')
		{
			
		}
		else
		{

		 QColor color=getEdgeColorByPathway(edge[i][6]);
		 
		 //painter->setPen(color);
		 painter->setPen(QColor(80,80,80,200));
		 bool t1=false,t2=false;
		 type=edge[i][0], id=edge[i][1];		 
		 switch(type)
	     {
		     case 'C': s=complexPos[id]; start=complexPos[id].center();         break;
			 case 'D': s=DnaPos[id]; start=DnaPos[id].center();             break;
			 case 'E': s=physicalEntityPos[id]; start=physicalEntityPos[id].center();          break; 
			 case 'P': s=proteinPos[id];  start=proteinPos[id].center();         break;
			 case 'S': s=smallMoleculePos[id]; start=smallMoleculePos[id].center();   break;			
		 }
		 type=edge[i][4];
		 id=edge[i][5];
		 if(type=='R') 
		 {
			 middle=reactionPos[id].center();
		     m=reactionPos[id];			
		 }
		 //else middle degeneration???
		 		 
		 type=edge[i][2], id=edge[i][3];	
		 QRectF rect;
		 switch(type)
	     {
		     case 'C': end=complexPos[id].center();       rect= complexPos[id]; break;
			 case 'D': end=DnaPos[id].center();           rect= DnaPos[id];    break;
			 case 'E': end=physicalEntityPos[id].center();        rect= physicalEntityPos[id];    break; 
			 case 'P': end=proteinPos[id].center();       rect= proteinPos[id];    break;
			 case 'S': end=smallMoleculePos[id].center(); rect= smallMoleculePos[id];  break;			
			 case 'R': end=reactionPos[id].center();      rect= reactionPos[id];  break;  //catalysis for bio reaction	
		 }	
		 e=rect;
		
		 
		 QLineF line1,line2,line3,line4;
		 
		 line1.setP1(QPointF(rect.x(),rect.y()));                             line1.setP2(QPointF(rect.x()+rect.width(),rect.y())); 
		 line2.setP1(QPointF(rect.x()+rect.width(),rect.y()));                line2.setP2(QPointF(rect.x()+rect.width(),rect.y()+rect.height()));
		 line3.setP1(QPointF(rect.x()+rect.width(),rect.y()+rect.height()));  line3.setP2(QPointF(rect.x(),rect.y()+rect.height()));
		 line4.setP1(QPointF(rect.x(),rect.y()+rect.height()));               line4.setP2(QPointF(rect.x(),rect.y()));

		 QLineF line0(middle,end);
		 QPointF itsect;

	 	 if(line0.intersect(line1,&itsect)!=QLineF::BoundedIntersection)
		 {
		   if(line0.intersect(line2,&itsect)!=QLineF::BoundedIntersection)
		   {
			   if(line0.intersect(line3,&itsect)!=QLineF::BoundedIntersection)
			   {
				   if(line0.intersect(line4,&itsect)==QLineF::BoundedIntersection)
				       end=itsect;	
			   }
			   else end=itsect;	
		   }
		   else 
			  end=itsect;	
		 }
		 else 
		   end=itsect;	
		
		 start=QPointF(start.x()*width*2-width, start.y()*height*2-height);
		 middle=QPointF(middle.x()*width*2-width, middle.y()*height*2-height);
		 end=QPointF(end.x()*width*2-width, end.y()*height*2-height);
 
		 	if(start.x()>=-width*1.3&&middle.x()>=-width*1.3&&end.x()>=-width*1.3)
			{
				//if(edge[i][2]=='R'&&edge[i][4]=='R')
				if(m.x()>-0.99 && e.x() >-0.99 && s.x()>-0.99)
				{

					temp[0]=edge[i][4];	temp[1]=edge[i][5]; reactionRecord.insert(temp);
					temp[0]=edge[i][2];	temp[1]=edge[i][3]; reactionRecord.insert(temp);
					temp[0]=edge[i][0];	temp[1]=edge[i][1]; reactionRecord.insert(temp);
		 			painter->drawLine(start+dCenter,middle+dCenter);	

					painter->drawLine(middle+dCenter,end+dCenter);		
					painter->setBrush(QColor(255,255,255,255));                                                                                                                                                                                                                                                                                                                                                                                                        
					if(type=='R')
					{
						int R=3;
						float dx,dy,ds;
						ds=sqrt(pow(start.x()-middle.x(),2)+pow(start.y()-middle.y(),2));
						dx=(start.x()-middle.x())/ds, dy=(start.y()-middle.y())/ds;
						end.setX(middle.x()+dx*R*2), end.setY(middle.y()+dy*R*2);
						painter->drawEllipse(end+dCenter,R,R);
					}
					else
					{
					   drawArrow(painter, middle+dCenter, end+dCenter, QColor(0, 0, 0, 255) );		 
					}
				}
		   }
		   else
		   {			
			end=end;
		   }
			}
	}


	for(int i=0; i<edge.size(); i++)
	{
		if( (edge[i][2]=='P'&& edge[i][3]==39) || (edge[i][0]=='P'&& edge[i][1]==39) || (edge[i][4]=='P'&& edge[i][5]==39))
			i=i;

         //painter->drawRect(reactionPos[i].x(),reactionPos[i].y(),2,2);	
		if(!(edge[i][2]=='R'&&edge[i][4]=='R'))
		{
			
		}
		else
		{

		 QColor color=getEdgeColorByPathway(edge[i][6]);
		 
		 //painter->setPen(color);
		 painter->setPen(QColor(80,80,80,200));
		 bool t1=false,t2=false;
		 type=edge[i][0], id=edge[i][1];		 
		 switch(type)
	     {
		     case 'C': s=complexPos[id]; start=complexPos[id].center();         break;
			 case 'D': s=DnaPos[id]; start=DnaPos[id].center();             break;
			 case 'E': s=physicalEntityPos[id]; start=physicalEntityPos[id].center();          break; 
			 case 'P': s=proteinPos[id];  start=proteinPos[id].center();         break;
			 case 'S': s=smallMoleculePos[id]; start=smallMoleculePos[id].center();   break;			
		 }
		 type=edge[i][4];
		 id=edge[i][5];
		 if(type=='R') 
		 {
			 middle=reactionPos[id].center();
		     m=reactionPos[id];			
		 }
		 //else middle degeneration???
		 		 
		 type=edge[i][2], id=edge[i][3];	
		 QRectF rect;
		 switch(type)
	     {
		     case 'C': end=complexPos[id].center();       rect= complexPos[id]; break;
			 case 'D': end=DnaPos[id].center();           rect= DnaPos[id];    break;
			 case 'E': end=physicalEntityPos[id].center();        rect= physicalEntityPos[id];    break; 
			 case 'P': end=proteinPos[id].center();       rect= proteinPos[id];    break;
			 case 'S': end=smallMoleculePos[id].center(); rect= smallMoleculePos[id];  break;			
			 case 'R': end=reactionPos[id].center();      rect= reactionPos[id];  break;  //catalysis for bio reaction	
		 }	
		 e=rect;
		
		 
		 QLineF line1,line2,line3,line4;
		 
		 line1.setP1(QPointF(rect.x(),rect.y()));                             line1.setP2(QPointF(rect.x()+rect.width(),rect.y())); 
		 line2.setP1(QPointF(rect.x()+rect.width(),rect.y()));                line2.setP2(QPointF(rect.x()+rect.width(),rect.y()+rect.height()));
		 line3.setP1(QPointF(rect.x()+rect.width(),rect.y()+rect.height()));  line3.setP2(QPointF(rect.x(),rect.y()+rect.height()));
		 line4.setP1(QPointF(rect.x(),rect.y()+rect.height()));               line4.setP2(QPointF(rect.x(),rect.y()));

		 QLineF line0(middle,end);
		 QPointF itsect;

	 	 if(line0.intersect(line1,&itsect)!=QLineF::BoundedIntersection)
		 {
		   if(line0.intersect(line2,&itsect)!=QLineF::BoundedIntersection)
		   {
			   if(line0.intersect(line3,&itsect)!=QLineF::BoundedIntersection)
			   {
				   if(line0.intersect(line4,&itsect)==QLineF::BoundedIntersection)
				       end=itsect;	
			   }
			   else end=itsect;	
		   }
		   else 
			  end=itsect;	
		 }
		 else 
		   end=itsect;	
		
		 start=QPointF(start.x()*width*2-width, start.y()*height*2-height);
		 middle=QPointF(middle.x()*width*2-width, middle.y()*height*2-height);
		 end=QPointF(end.x()*width*2-width, end.y()*height*2-height);
 
		 	if(start.x()>=-width*1.3&&middle.x()>=-width*1.3&&end.x()>=-width*1.3)
			{
				temp[0]=edge[i][2];	temp[1]=edge[i][3]; 
				if(m.x()>-0.99 && e.x() >-0.99 && s.x()>-0.99 && reactionRecord.find(temp)!=reactionRecord.end())
				{

					temp[0]=edge[i][4];	temp[1]=edge[i][5]; reactionRecord.insert(temp);
					temp[0]=edge[i][2];	temp[1]=edge[i][3]; reactionRecord.insert(temp);
					temp[0]=edge[i][0];	temp[1]=edge[i][1]; reactionRecord.insert(temp);
		 			painter->drawLine(start+dCenter,middle+dCenter);	

					painter->drawLine(middle+dCenter,end+dCenter);		
					painter->setBrush(QColor(255,255,255,255));                                                                                                                                                                                                                                                                                                                                                                                                        
					if(type=='R')
					{
						int R=3;
						float dx,dy,ds;
						ds=sqrt(pow(start.x()-middle.x(),2)+pow(start.y()-middle.y(),2));
						dx=(start.x()-middle.x())/ds, dy=(start.y()-middle.y())/ds;
						end.setX(middle.x()+dx*R*2), end.setY(middle.y()+dy*R*2);
						painter->drawEllipse(end+dCenter,R,R);
					}
					else
					{
					   drawArrow(painter, middle+dCenter, end+dCenter, QColor(0, 0, 0, 255) );		 
					}
				}
		   }
		   else
		   {			
			end=end;
		   }
			}
	}
	
	
	
	for(int i=0; i<reactionPos.size(); i++)
	{
		temp[0]='R';	temp[1]=i; 
		if(reactionPos[i].x()>-0.99&& reactionRecord.find(temp)!=reactionRecord.end())
		{
			paintReaction(painter,reactionPos, _scene->_4biochemicalReaction[m_pathwayID], i, dCenter, _scale, _scene->_4reactionName[m_pathwayID][i][0]);
		}
	}
	
	for(int i=0; i<smallMoleculePos.size(); i++)
	{
		temp[0]='S';	temp[1]=i; 
		if(smallMoleculePos[i].x()>-0.99&& reactionRecord.find(temp)!=reactionRecord.end())		
        {
			paintSmallMolecule(painter, smallMoleculePos, _scene->_9smallMoleculeName[m_pathwayID], i, dCenter, _scale);
	        //painter->drawRect(smallMoleculePos[i].x()*width*2-width,smallMoleculePos[i].y()*height*2-height,2,2);
		}
	}
	
	for(int i=0; i<DnaPos.size(); i++)
	{
		temp[0]='D';	temp[1]=i; 
		if(DnaPos[i].x()>-0.99 && reactionRecord.find(temp)!=reactionRecord.end())				
		{	
			paintDna(painter, DnaPos,  _scene->_9DnaName[m_pathwayID], i, dCenter, _scale);
            //painter->drawDna(DnaPos[i].x()*width*2-width,DnaPos[i].y()*height*2-height,2,2);
		}
	    
	}

	for(int i=0; i<physicalEntityPos.size(); i++)
	{temp[0]='E';	temp[1]=i; 
	  if(physicalEntityPos[i].x()>-0.99&&reactionRecord.find(temp)!=reactionRecord.end())	
	  {
		  paintEntity( painter,  physicalEntityPos, _scene->_8physicalEntityName[m_pathwayID], i, dCenter, _scale);
	      //painter->drawRect(proteinPos[i].x()*width*2-width,proteinPos[i].y()*height*2-height,2,2);
	  }
	}
	
	
	for(int i=0; i<complexPos.size(); i++)
	{
	temp[0]='C';	temp[1]=i; 
		if(complexPos[i].x()>-0.99&&reactionRecord.find(temp)!=reactionRecord.end())	
		{  
			
			paintComplex( painter, complexPos, _scene->_6complexName[m_pathwayID], i, dCenter, _scale);
	        //painter->drawRect(complexPos[i].x()*width*2-width,complexPos[i].y()*height*2-height,2,2);
		}
	}
	for(int i=0; i<proteinPos.size(); i++)
	{
		temp[0]='P';	temp[1]=i; 
	  if(proteinPos[i].x()>-0.99&&reactionRecord.find(temp)!=reactionRecord.end())	
	  {
		 QString s=_scene->_7proteinName[m_pathwayID][i][0];
		 paintProtein( painter, proteinPos, _scene->_7proteinName[m_pathwayID], i, dCenter, _scale);
	     
	  }
	}
			 
	painter->setClipping(false);
	
	//setFlag(QGraphicsItem::ItemClipsToShape, false);	//if we set this flag true, then it clip all the item to the bounding rect no matter what
	//use painter->setclip can perform selectibe clipping		
	if(_penisOn)
	{
		drawCurve(painter);
	}
	else if(groupSelected)
	{
	    drawfills(painter);	
	}

	drawConnections(painter);//pathwaySelected=-1;
	
	if(!itemSelected.empty()&&draggingItem)
	{
	    paintItemDragged(painter);
	}

	
}


int PathBubble1::getVerticeID(int type, int id)
{
	     //Graph g(complexNum + proteinNum + smallMoleculeNum + DnaNum + physicalEntityNum + reactionNum );
	     int ID;
	     switch(type)
	     {
		     case 'C': ID=id;  break;			 
			 case 'P': ID=complexNum + 1 + id;  break;
			 case 'S': ID=complexNum + 1 + proteinNum + 1 + id ;  break;
			 case 'D': ID=complexNum + 1 + proteinNum + 1 + smallMoleculeNum + 1 + id;  break;			
			 case 'E': ID=complexNum + 1 + proteinNum + 1 + smallMoleculeNum + 1 + DnaNum + 1 + id;  break; 
		     case 'R': ID=complexNum + 1 + proteinNum + 1 + smallMoleculeNum + 1 + DnaNum + 1 + physicalEntityNum + 1 + id ;  break;
		 }
		 return ID;
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
		 case 'R': ID = complexNum + proteinNum + smallMoleculeNum + physicalEntityNum + DnaNum + id ;  break;		 
	  }
	  return ID-1;
}

void PathBubble1::getItemID(int ID, int &type, int &id)
{
	     //Graph g(complexNum + proteinNum + smallMoleculeNum + DnaNum + physicalEntityNum + reactionNum );
	     if(ID<=complexNum)
	     {
		     type = 'C', id=ID;  
		 }
		 else if(ID <= complexNum + 1 + proteinNum)	
		 {	 type = 'P', id = ID - (complexNum + 1);
		 }
		 else if(ID <=complexNum + 1 + proteinNum + 1 + smallMoleculeNum)
         {   
			 type = 'S',   id = ID - (complexNum + 1 + proteinNum + 1 );  
		 }
		 else if(ID<=	complexNum + 1 + proteinNum + 1 + smallMoleculeNum + 1 + DnaNum)
		 {	 
			 type= 'D', id= ID - (complexNum + 1 + proteinNum + 1 + smallMoleculeNum + 1);			
		 }
		 else if(ID<=complexNum + 1 + proteinNum + 1 + smallMoleculeNum + 1 + DnaNum + 1 + physicalEntityNum)
         { 
			 
			 type='E', id= ID - (complexNum + 1 + proteinNum + 1 + smallMoleculeNum + 1 + DnaNum + 1);		     
		 }
		 else
		 {	
			 type= 'R', id = ID - (complexNum + 1 + proteinNum + 1 + smallMoleculeNum + 1 + DnaNum + 1 + physicalEntityNum + 1) ;
		 }		 
}

void PathBubble1::hoverMoveEvent( QGraphicsSceneHoverEvent *event )
{   //static bool grabon=false;
	ItemBase::hoverMoveEvent(event);
	if(!groupSelected)
	{
		highlighted.clear();
		vector<int> item=whichItem(event->pos(),this->Width()*_scale, this->Height()*_scale);
		if(!item.empty())
		{
			highlighted.insert(item);
		}
	    if(!highlighted.empty())
		{
			itemSelected.clear();
			edgeSelected.clear();
		}
		if(_grabisOn)
		{
            Qt::CursorShape shape = Qt::OpenHandCursor;
	        setCursor(shape);  
			
	    }	
		if(_queryisOn)
		{
			//QBitmap bitmap("icon/pencil.png");
			//QCursor cursor (bitmap,bitmap);
			Qt::CursorShape shape = Qt::WhatsThisCursor;
	        setCursor(shape);    		
			//setCursor(cursor);   
	    }	
		if(_penisOn)
		{
		   QBitmap bitmap("icon/Pencil.png");
		   QCursor cursor (bitmap,bitmap);
		   setCursor(cursor); 
		}
		if(_upisOn)
		{	QBitmap bitmap("icon/GoUp.png");
			QCursor cursor (bitmap,bitmap);
		   setCursor(cursor); 
		}
		if(_downisOn)
		{
			QBitmap bitmap("icon/GoDown.png");
			QCursor cursor (bitmap,bitmap);
			setCursor(cursor);   
		}
		if(_evenisOn)
		{
			QBitmap bitmap("icon/GoEven.png");
			QCursor cursor (bitmap,bitmap);
			setCursor(cursor); 
		}
	}
}

void PathBubble1::updateItemGrid(vector<vector<set<vector<int>>>> &itemGrid, int type, int id, QPointF newPos)
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

vector<vector<set<vector<int>>>> PathBubble1::recordItemGrid( )
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

void PathBubble1::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	ItemBase::mousePressEvent(event);
	
	
	//void PathBubble1::setAItemDragged(int type, int id)updateBubblePath(event->pos()-event->lastPos());

	//highlighted.clear();
	
	if (event->button() == Qt::LeftButton )
	{
		_mousePressed=1;
	    //MenuisOn=1;
		if(!groupSelected)
		{   
			if(_grabisOn)
			{
				itemSelected = whichItem(event->pos(),this->Width()*_scale, this->Height()*_scale);
   		        if(!itemSelected.empty()) 
		        {   
					setAItemDragged(itemSelected[0], itemSelected[1]);		
				    
				}
			}
			else if(_queryisOn)
			{
				itemSelected = whichItem(event->pos(),this->Width()*_scale, this->Height()*_scale);
   		        if(!itemSelected.empty()) 
		        {
					 setAItemDragged(itemSelected[0], itemSelected[1]);		
					 
				}
			}
			else if(_upisOn)
			{
			    vector<int> ColorSelected = whichItem(event->pos(),this->Width()*_scale, this->Height()*_scale);	
				if(!ColorSelected.empty()) 
		        {
					 setAItemColored(ColorSelected[0], ColorSelected[1],_UP);							 
				}
			}
			else if(_downisOn)
			{
			    vector<int> ColorSelected = whichItem(event->pos(),this->Width()*_scale, this->Height()*_scale);	
				if(!ColorSelected.empty()) 
		        {
					 setAItemColored(ColorSelected[0], ColorSelected[1],_DOWN);							 
				}
			}
			else if(_evenisOn)
			{
			    vector<int> ColorSelected = whichItem(event->pos(),this->Width()*_scale, this->Height()*_scale);	
				if(!ColorSelected.empty()) 
		        {
					 setAItemColored(ColorSelected[0], ColorSelected[1],_EVEN);							 
				}
			}
			else
			{
			    itemSelected = whichItem(event->pos(),this->Width()*_scale, this->Height()*_scale);   	      
			  
			}
		}
	    else 
		{
			if(_contain.containsPoint(event->pos(), Qt::OddEvenFill))
			{
				setItemDragged(itemSelected);
				setEdgeAffected(itemSelected, _edgeSelected, _edgeAffected);
			}
	    }
   	    if(_grabisOn)
		{
		   Qt::CursorShape shape = Qt::ClosedHandCursor;
	       setCursor(shape);  
		}
		else if(_queryisOn)
		{
		   Qt::CursorShape shape = Qt::ClosedHandCursor;
	       setCursor(shape);  
		}
		/*else if(_upisOn)
		{
		   Qt::CursorShape shape = Qt::ArrowCursor;
	       setCursor(shape);  
		}*/
		if(_penisOn)
	        initPos = event->pos();
	}	
	else 
	{
	   draggingItem=0;
	}
}

void PathBubble1::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	ItemBase::mouseReleaseEvent( event );
	_mousePressed=0;
	
	/*if( itemSelected == whichItem(event->pos(),this->Width(), this->Height()))
	{		
		if(!itemSelected.empty())
		{
			QString qstr;
			string s1;
			std::ostringstream oss;
			int type=itemSelected[0], id=itemSelected[1];
			switch(type)
			{ 
			    case 'C': qstr=_scene->_6complexName[id].c_str(); break;
		        case 'E': qstr=_scene->_8physicalEntityName[id][0].c_str(); break;
		        case 'P': qstr=_scene->_7proteinName[id][0].c_str(); break;
		        case 'S': s1 = "smallMolecule";  	oss << s1 << id; s1=oss.str(); qstr=s1.c_str(); break;		
		        case 'D': s1 = "Dna"; 	oss << s1 << id;	s1=oss.str();  qstr=s1.c_str(); break;	 				     	
			}
			showIterm(event->pos(), event->scenePos(), qstr); 	
		}
		itemDragged.clear();
		//itemSelected[0]=-1;
		itemSelected.clear();
		edgeSelected.clear();
	}*/
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
			    case 'C': qstr=_scene->_6complexName[m_pathwayID][id]; iPos=complexPos[id].center(); break;
		        case 'E': qstr=_scene->_8physicalEntityName[m_pathwayID][id][0]; iPos=physicalEntityPos[id].center(); break;
		        case 'P': qstr=_scene->_7proteinName[m_pathwayID][id][0]; iPos=proteinPos[id].center(); break;
		        case 'S': s1 = "smallMolecule";  	oss << s1 << id; s1=oss.str(); qstr=s1.c_str(); iPos=smallMoleculePos[id].center(); break;		
		        case 'D': s1 = "Dna"; 	oss << s1 << id;	s1=oss.str();  qstr=s1.c_str(); iPos=DnaPos[id].center(); break;	 				     	
				case 'N': qstr=_scene->_4EmptyName[m_pathwayID][id][0]; iPos=EmptyPos[id].center(); break;	
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
			    case 'C': qstr=_scene->_6complexName[m_pathwayID][id]; iPos=complexPos[id].center(); break;
		        case 'E': qstr=_scene->_8physicalEntityName[m_pathwayID][id][0]; iPos=physicalEntityPos[id].center(); break;
		        case 'P': qstr=_scene->_7proteinName[m_pathwayID][id][0]; iPos=proteinPos[id].center(); break;
		        case 'S': s1 = "smallMolecule";  	oss << s1 << id; s1=oss.str(); qstr=s1.c_str(); iPos=smallMoleculePos[id].center(); break;		
		        case 'D': s1 = "Dna"; 	oss << s1 << id;	s1=oss.str();  qstr=s1.c_str(); iPos=DnaPos[id].center(); break;
				case 'N': qstr=_scene->_4EmptyName[m_pathwayID][id][0]; iPos=EmptyPos[id].center(); break;	
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
			/*switch(type)
			{ 
			    case 'C': qstr=_scene->_6complexName[id].c_str(); iPos=complexPos[id].center(); break;
		        case 'E': qstr=_scene->_8physicalEntityName[id][0].c_str(); iPos=physicalEntityPos[id].center(); break;
		        case 'P': qstr=_scene->_7proteinName[id][0].c_str(); iPos=proteinPos[id].center(); break;
		        case 'S': s1 = "smallMolecule";  	oss << s1 << id; s1=oss.str(); qstr=s1.c_str(); iPos=smallMoleculePos[id].center(); break;		
		        case 'D': s1 = "Dna"; 	oss << s1 << id;	s1=oss.str();  qstr=s1.c_str(); iPos=DnaPos[id].center(); break;	 				     	
			}*/
			QRectF rect=_contain.boundingRect();
			iPos=rect.center();
			
			if(!this->realRect().contains(event->pos().x(),event->pos().y()) && varifyItemSelected(itemSelected))
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
							case 'C': complexPos[id].setX(-1); break;
							case 'E': physicalEntityPos[id].setX(-1); break;
		                    case 'P': proteinPos[id].setX(-1); break;
		                    case 'S': smallMoleculePos[id].setX(-1); break;
		                    case 'D': DnaPos[id].setX(-1); break;
							case 'R': reactionPos[id].setX(-1); break;
					    }			 					 
		           }
														
					//change selected bubble to same location
					/*
					iPos.setX((iPos.x()+this->Width()/2.0)/this->Width());
					iPos.setY((iPos.y()+this->Height()/2.0)/this->Height());
					for(set<vector<int>>::iterator it=highlighted.begin(); it!=highlighted.end(); it++) 
					{
						vector<int> node=*it;
						int type=node[0], id=node[1];
						switch(type)
						{
							case 'C': complexPos[id].moveCenter(iPos); break;
							case 'E': physicalEntityPos[id].moveCenter(iPos); break;
		                    case 'P': proteinPos[id].moveCenter(iPos); break;
		                    case 'S': smallMoleculePos[id].moveCenter(iPos); break;
		                    case 'D': DnaPos[id].moveCenter(iPos); break;
							case 'R': reactionPos[id].moveCenter(iPos); break;
					    }			 					 
		           }*/
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
				groupSelected=groupSeletion(_lines);
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
}

bool PathBubble1::varifyItemSelected(vector<int> itemSelected)
{
	bool flag=false;
	for(int i=0; i<itemSelected.size(); i+=2)
	{
	    if(itemSelected[i]!='R')
		   return true;
	}
	return flag;
}

void PathBubble1::checkGroupConnections(vector<int> &itemSelected, vector<vector<int>> &edgeSelected)
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

bool PathBubble1::groupSeletion(QVector<QLineF> _lines)
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

  //check overlap
  
  /*for (i=0; i<fills.size(); i+=4) 
  {
	  for(j=fills[i].x1(); j<fills[1].x2(); j+=4)
	  {
		  vector<int> temp= whichItem(QPointF(j,fills[i].y1()),this->Width(), this->Height());
		  if(!temp.empty())
		     itemSelected.push_back(temp[0]), itemSelected.push_back(temp[1]);
	  }
  }*/

    bool flag=0;
    float width=this->Width(), height=this->Height();
    float X,Y,W,H;
	for(int id=0; id<complexPos.size(); id++)
	{
		X=complexPos[id].x(), Y=complexPos[id].y(), W=complexPos[id].width(), H=complexPos[id].height();  
		X=X*width-width/2+dCenter.x(), Y=Y*height-height/2+dCenter.y(), W=W*width, H=H*height;
		
		QPolygonF p;
		int x1,y1;
		for(int i=0;i<2;i++)
		for(int j=0;j<2;j++)
		{
			x1=X+W*i;  y1=Y+H*j;
		    p << QPointF(x1,y1); 
		}
			
		QPolygonF s=_contain.intersected(p);
		if(s.size()!=0)
		{
			flag=1;
			itemSelected.push_back('C'), itemSelected.push_back(id);
			vector<int> item; item.push_back('C'); item.push_back(id);
			highlighted.insert(item);
		}
	}

	for(int id=0; id<reactionPos.size(); id++)
	{
		X=reactionPos[id].x(), Y=reactionPos[id].y(), W=reactionPos[id].width(), H=reactionPos[id].height();  
		X=X*width-width/2+dCenter.x(), Y=Y*height-height/2+dCenter.y(), W=W*width, H=H*height;
		
		QPolygonF p;
		int x1,y1;
		for(int i=0;i<2;i++)
		for(int j=0;j<2;j++)
		{
			x1=X+W*i;  y1=Y+H*j;
		    p << QPointF(x1,y1); 
		}
			
		QPolygonF s=_contain.intersected(p);
		if(s.size()!=0)
		{
			flag=1;
			itemSelected.push_back('R'), itemSelected.push_back(id);
			vector<int> item; item.push_back('R'); item.push_back(id);
			highlighted.insert(item);
		}
	}
	  
	for(int id=0; id<proteinPos.size(); id++)
	{
		X=proteinPos[id].x(), Y=proteinPos[id].y(), W=proteinPos[id].width(), H=proteinPos[id].height();  
		X=X*width-width/2+dCenter.x(), Y=Y*height-height/2+dCenter.y(), W=W*width, H=H*height;
		
		QPolygonF p;
		int x1,y1;
		for(int i=0;i<2;i++)
		for(int j=0;j<2;j++)
		{
			x1=X+W*i;  y1=Y+H*j;
		    p << QPointF(x1,y1); 
		}
			
		QPolygonF s=_contain.intersected(p);
		if(s.size()!=0)
		{
			flag=1;
			itemSelected.push_back('P'), itemSelected.push_back(id);
			vector<int> item; item.push_back('P'); item.push_back(id);
			highlighted.insert(item);
		}
	}
	
	for(int id=0; id<physicalEntityPos.size(); id++)
	{
		X=physicalEntityPos[id].x(), Y=physicalEntityPos[id].y(), W=physicalEntityPos[id].width(), H=physicalEntityPos[id].height();  
		X=X*width-width/2+dCenter.x(), Y=Y*height-height/2+dCenter.y(), W=W*width, H=H*height;
		
		QPolygonF p;
		int x1,y1;
		for(int i=0;i<2;i++)
		for(int j=0;j<2;j++)
		{
			x1=X+W*i;  y1=Y+H*j;
		    p << QPointF(x1,y1); 
		}
			
		QPolygonF s=_contain.intersected(p);
		if(s.size()!=0)
		{
			flag=1;
			itemSelected.push_back('E'), itemSelected.push_back(id);
			vector<int> item; item.push_back('E'); item.push_back(id);
			highlighted.insert(item);
		}
	}

  checkGroupConnections(itemSelected, edgeSelected);
	 
  return flag;
}

void PathBubble1::setItemDragged(vector<int> itemSelected)
{
	itemDragged.clear();
	for(int i=0; i<itemSelected.size(); i+=2)
	{
	   setAItemDragged(itemSelected[i], itemSelected[i+1]);
	}	
}

void PathBubble1::setEdgeAffected(vector<int> itemSelected, set<vector<vector<int>>> &edgeSelected, set<vector<vector<int>>> &edgeAffected)
{//edgeSelected in the pen selection, edgeAffected: the edge spaning the selected items with the rest
	edgeSelected.clear(); edgeAffected.clear();
		
	set < vector <int>> itemset;
	int j=0;	
	for(int i=0; i<itemSelected.size(); i+=2)
	{
		vector<int> node(2,0);		
		node[0]=itemSelected[i], node[1]=itemSelected[i+1];
		itemset.insert(node);
	}

	for(set < vector <int>> :: iterator it=itemset.begin(); it != itemset.end(); it ++)
	{
		vector<vector<int>> nodes;
		vector<int> node= *it;
		QRectF rect;
		vector<int> temp;
		vector <vector <int>>  edge (2, node);		
		switch(node[0])
		{
		     case 'C': nodes = _complex[node[1]];  break;
			 case 'D': nodes = _Dna[node[1]];  break;
			 case 'E': nodes = _entity[node[1]]; break;
			 case 'P': nodes = _protein[node[1]]; break;
			 case 'S': nodes = _smallMolecule[node[1]]; break;
			 case 'R': nodes = _reaction[node[1]]; break;
		}		
	
		for(int k=0; k<nodes.size(); k++)
		{	
			 node[0]=nodes[k][0];
			 node[1]=nodes[k][1];
			 edge[1]=node; 
			 
			 switch(node[0])
		     {
		     case 'C': rect= complexPos[node[1]]; break;
			 case 'D': rect=DnaPos[node[1]];  break;
			 case 'E': rect=physicalEntityPos[node[1]]; break;
			 case 'P': rect=proteinPos[node[1]]; break;
			 case 'S': rect=smallMoleculePos[node[1]]; break;
			 case 'R': rect=reactionPos[node[1]]; break;
		     }

			 if(rect.x()>-0.99)
			 {
			  if(nodes[k][3]==-1)
			  {
			    temp=edge[1]; edge[1]=edge[0]; edge[0]=temp;
			  }

			  if(itemset.find(node)==itemset.end())
			    edgeAffected.insert(edge);
			  else edgeSelected.insert(edge);
			 }
		}		    
		j++;
	}	
	
}

void PathBubble1::setAItemColored(int type, int id, int up_or_down)
{
	vector<int> temp(3,0);
	std::set< vector<int> >::iterator it;
	temp[0]=type; temp[1]=id; temp[2]=_UP;
	it=_itemColored.find(temp);
	if(it!=_itemColored.end()) _itemColored.erase(it);

	temp[2]=_DOWN;
	it=_itemColored.find(temp);
	if(it!=_itemColored.end()) _itemColored.erase(it);

	temp[2]=_EVEN;
	it=_itemColored.find(temp);	
	if(it!=_itemColored.end()) _itemColored.erase(it);

	temp[2]=up_or_down;
    _itemColored.insert(temp);

}

void PathBubble1::setAItemDragged(int type, int id)
{
	
	int width = this->realRect().width();
    int height = this->realRect().height();
    int h,w,cx,cy; //Actually wx,wy;
	
    switch(type)
	{
	    case 'C': h=complexPos[id].height()*height, w=complexPos[id].width()*width; 
			      h=h/2, w=w/2;
			      cx=complexPos[id].x()*width-width/2+w, cy=complexPos[id].y()*height-height/2+h;
				  itemDragged.push_back(QRect(cx,cy,w,h));	     break;
	    case 'D': 
			      h=DnaPos[id].height()*height, w=DnaPos[id].width()*width; 
			      h=h/2, w=w/2;
			      cx = DnaPos[id].x()*width-width/2+w, cy=DnaPos[id].y()*height-height/2+h;
				  itemDragged.push_back(QRect(cx,cy,w,h));	     break;
		case 'E': 
			      h=physicalEntityPos[id].height()*height, w=physicalEntityPos[id].width()*width; 
			      h=h/2, w=w/2;
			      cx=physicalEntityPos[id].x()*width-width/2+w, cy=physicalEntityPos[id].y()*height-height/2+h;
				  itemDragged.push_back(QRect(cx,cy,w,h));	     break;
		case 'P': 
			      h=proteinPos[id].height()*height, w=proteinPos[id].width()*width; 
			      h=h/2, w=w/2;
			      cx=proteinPos[id].x()*width-width/2+w, cy=proteinPos[id].y()*height-height/2+h;
				  itemDragged.push_back(QRect(cx,cy,w,h));	     break;
		case 'S': 
			      h=smallMoleculePos[id].height()*height, w=smallMoleculePos[id].width()*width; 
			      h=h/2, w=w/2;
			      cx=smallMoleculePos[id].x()*width-width/2+w, cy=smallMoleculePos[id].y()*height-height/2+h;
				  itemDragged.push_back(QRect(cx,cy,w,h));       break;
		case 'R': 
			      h=reactionPos[id].height()*height, w=reactionPos[id].width()*width; 
			      h=h/2, w=w/2;
			      cx=reactionPos[id].x()*width-width/2+w, cy=reactionPos[id].y()*height-height/2+h;
				  itemDragged.push_back(QRect(cx,cy,w,h));	     break;
	}	
	
	//QRectF firstRect=this->sceneBoundingRect();	
	//rect.moveCenter(QPointF(rect.x()-firstRect.x(), rect.y()-firstRect.y()));	
	//groupSeletion(_lines);				
}

void PathBubble1::paintItem(QPainter *painter, int type, int i, bool flag, QPointF dCenter, float scale, QColor c)
{
	switch(type)
	{ 
		case 'C': paintComplex( painter, complexPos, _scene->_6complexName[m_pathwayID], i, dCenter, scale, flag, c); break;
		case 'E': paintEntity( painter,  physicalEntityPos, _scene->_8physicalEntityName[m_pathwayID], i, dCenter, scale, flag, c); break;
		case 'P': paintProtein( painter, proteinPos, _scene->_7proteinName[m_pathwayID], i, dCenter, scale, flag, c); break;
		case 'S': paintSmallMolecule(painter, smallMoleculePos, _scene->_9smallMoleculeName[m_pathwayID], i, dCenter, scale, flag, c); break;	
		case 'D': paintDna(painter, DnaPos, _scene->_9DnaName[m_pathwayID], i, dCenter,scale, flag, c); break;	 	
		case 'R': paintReaction(painter,reactionPos, _scene->_4biochemicalReaction[m_pathwayID], i, dCenter, scale, _scene->_4reactionName[m_pathwayID][i][0], flag, c);  break;	 	
	}	

}


void PathBubble1::showIterm( QPointF iPos, QPointF pos, QPointF scenePos, QString name)
{
	float x=pos.x(), y=pos.y();	
	
	QString fileName("iterms/");
	fileName.append(name);
	fileName.append(".txt");
	QByteArray ba = fileName.toLocal8Bit();
    const char *name_str = ba.data();
	
	FILE *fp = fopen(name_str,"r"); 
	if(fp!=NULL)
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
	}
}

void PathBubble1::drawConnections(QPainter *painter)
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

	for (int i = 0; i < _codeBubbles.size(); i ++)
	{
		color = this->_codeBubbles[i]->getColor();
		pen.setColor( QColor(color.a, color.b, color.c, 200) );
		painter->setPen( pen );

		constart = this->_conPoint[i];
		constart.setX( this->Width()/2 );
		constart.setY( constart.y()*height - height/2);
		constart=constart+dCenter;
		
		conend = _codeBubbles[i]->pos();
		conend.setX( conend.x() - _codeBubbles[i]->Width()/2);
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
				painter->drawArc(  tmpmid.x()-20.0,  tmpmid.y()-20.0, 20.0, 20.0, 270*16, 90*16 );
				painter->drawLine( tmpmid-QPointF( 0, 10.0), QPointF(tmpmid.x(), conend.y()+10.0));
				tmpmid.setY( conend.y() );
				painter->drawArc(  tmpmid.x(), tmpmid.y(), 20.0, 20.0, 90*16, 90*16 );
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
		
		if(i<_conhighlight.size())
		{
			set<vector<int>> hi=_conhighlight[i];
			for(set<vector<int>>::iterator it=hi.begin(); it!=hi.end(); it++) 
			{
			vector<int> temp=*it;
            paintItem(painter, temp[0], temp[1], true, dCenter, _scale, QColor(color.a, color.b, color.c, 200) );			
			}	
		}
	}


	for (int i = 0; i < _reactionBubbles.size(); i ++)
	{
		color = this->_reactionBubbles[i]->getColor();
		pen.setColor( QColor(color.a, color.b, color.c, 200) );
		painter->setPen( pen );

		constart = this->_reactionPoint[i];
		constart.setX( this->Width()/2 );
		constart.setY( constart.y()*height - height/2);
		constart=constart+dCenter;

		conend = _reactionBubbles[i]->pos();
		conend.setX( conend.x() - _reactionBubbles[i]->Width()/2);
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
            paintItem(painter, temp[0], temp[1], true, dCenter, _scale, QColor(color.a, color.b, color.c, 200));					
		    }	
		}
	}

	//width = this->realRect().width();
    //height = this->realRect().height();

	for (int i = 0; i < _groupBubbles.size(); i ++)
	{
		color = this->_groupBubbles[i]->getColor();
		pen.setColor( QColor(color.a, color.b, color.c, 200) );
		painter->setPen( pen );

		constart = this->_groupPoint[i];
		constart.setX( this->Width()/2 );
		constart.setY( constart.y()*height - height/2);
		constart=constart+dCenter;

		conend = _groupBubbles[i]->pos();
		conend.setX( conend.x() - _groupBubbles[i]->Width()/2);
		conend -= this->pos();
	
		int wid = conend.x() - constart.x();

		tmpmid.setX( constart.x() + wid/2 );
		tmpmid.setY( constart.y() );

		/*
		if ( fabs(conend.y()-constart.y()) > 20)
		{
			painter->drawLine( dCenter + constart, dCenter +  tmpmid - QPointF( 10.0, 0) );

			if ( conend.y() > constart.y() )
			{
				painter->drawArc( dCenter.x() + tmpmid.x()-20.0, dCenter.y() + tmpmid.y(), 20.0, 20.0, 0, 90*16 );
				painter->drawLine( dCenter + tmpmid+QPointF( 0, 10.0), dCenter + QPointF(tmpmid.x(), conend.y()-10.0));
				tmpmid.setY( conend.y() );
				painter->drawArc( dCenter.x() + tmpmid.x(), dCenter.y() + tmpmid.y()-20.0, 20.0, 20.0, 180*16, 90*16 );
			}
			else
			{
				painter->drawArc( dCenter.x() + tmpmid.x()-20.0, dCenter.y() + tmpmid.y()-20.0, 20.0, 20.0, 270*16, 90*16 );
				painter->drawLine( dCenter + tmpmid-QPointF( 0, 10.0), dCenter + QPointF(tmpmid.x(), conend.y()+10.0));
				tmpmid.setY( conend.y() );
				painter->drawArc( dCenter.x() + tmpmid.x(), dCenter.y() + tmpmid.y(), 20.0, 20.0, 90*16, 90*16 );
			}
			painter->drawLine( dCenter + tmpmid + QPointF( 10.0, 0), dCenter + conend );
		}
		else
		{
			painter->drawLine( dCenter + constart, dCenter + tmpmid);
			painter->drawLine( dCenter + tmpmid, dCenter + QPointF(tmpmid.x(), conend.y()));
			tmpmid.setY( conend.y() );
			painter->drawLine( dCenter + tmpmid, dCenter + conend );
		}
	

		//arrow
		if (conend.x() > constart.x())
		{
			painter->drawLine( conend, dCenter + QPointF(conend.x()-10, conend.y()-4) );
			painter->drawLine( conend, dCenter + QPointF(conend.x()-10, conend.y()+4) );
		}
		else
		{
 			painter->drawLine( conend, dCenter + QPointF(conend.x()+10, conend.y()-4) );
 			painter->drawLine( conend, dCenter + QPointF(conend.x()+10, conend.y()+4) );
		}
		
		*/

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
				//paintItem(painter, temp[0], temp[1], true, QColor(color.a, color.b, color.c, 200));		
								
				QPointF center,center1;
				QRectF rect;
		        switch(temp[0])
		        {
					case 'C': nodes = _complex[temp[1]]; break;
					case 'D': nodes = _Dna[temp[1]]; break;
					case 'E': nodes = _entity[temp[1]]; break;
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
						case 'P': rect=proteinPos[it]; break;
						case 'S': rect=smallMoleculePos[it]; break;
						case 'R': rect=reactionPos[it]; break;
					}					
					if(rect.x()>-0.99)
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

vector<int> PathBubble1::whichItem( QPointF pos, float width, float height)
{	   
	vector<int> item(2,-1);
	int xi, yi;
	float x,y;
	int w=20,h=20;
	float XL,XR,YB,YT;
	x=pos.x()+width/2-dCenter.x(), y=pos.y()+height/2-dCenter.y();
	xi=x/width*w, yi=y/height*h;
	if(xi<0) xi=0; else if(xi>=w) xi=w;
	if(yi<0) yi=0; else if(yi>=h) yi=h;

	//for(int i=0; i<itemGrid[xi][yi].size(); i=i+2)
	for(set<vector<int>>::iterator it=itemGrid[xi][yi].begin(); it!=itemGrid[xi][yi].end(); it++) 
	{
		 vector<int> node=*it;
		 int type=node[0], id=node[1];
		 switch(type)
	     {
		     case 'C': XL=complexPos[id].x(), YB=complexPos[id].y(), XR=XL+complexPos[id].width(), YT=YB+complexPos[id].height();  break;
			 case 'D': XL=DnaPos[id].x(), YB=DnaPos[id].y(), XR=XL+DnaPos[id].width(), YT=YB+DnaPos[id].height();  break;
			 case 'E': XL=physicalEntityPos[id].x(), YB=physicalEntityPos[id].y(), XR=XL+physicalEntityPos[id].width(), YT=YB+physicalEntityPos[id].height();  break;
			 case 'P': XL=proteinPos[id].x(), YB=proteinPos[id].y(), XR=XL+proteinPos[id].width(), YT=YB+proteinPos[id].height();  break;
			 case 'S': XL=smallMoleculePos[id].x(), YB=smallMoleculePos[id].y(), XR=XL+smallMoleculePos[id].width(), YT=YB+smallMoleculePos[id].height();  break;
			 case 'R': XL=reactionPos[id].x(), YB=reactionPos[id].y(), XR=XL+reactionPos[id].width(), YT=YB+reactionPos[id].height();  break;
		 }
		
		 XL=XL*width, XR=XR*width, YB=YB*height, YT=YT*height;	

		if(insideRect(x, y, XL, YB, XR, YT))
		{
			item[0]=type, item[1]=id;
			return item;
		}
	}
	item.clear();
	return item;
}

QSizeF PathBubble1::getItemSize( int type, int id)
{
	float width=600, height=600;
	float h,w;
	QString qstr;
	string s1;
	std::ostringstream oss;
	QSizeF size;

	switch(type)
	{
	     case 'C': w=0.022; qstr=_scene->_6complexName[m_pathwayID][id]; break;
		 case 'E': w=0.03; qstr=_scene->_8physicalEntityName[m_pathwayID][id][0]; 
			     if(qstr.size()==0) 
				  { 
					 s1 = "physicalEntity"; 
					 oss << s1 << id; s1=oss.str(); 
					 qstr=s1.c_str(); 
				  } 
			      break;
		 case 'P': w=0.019; qstr=_scene->_7proteinName[m_pathwayID][id][0]; break;
		 case 'S': w=0.011; qstr=_scene->_9smallMoleculeName[m_pathwayID][id][0];   break;		
		 case 'D': w=0.011; qstr=_scene->_9DnaName[m_pathwayID][id][0];  break;	 		
		 case 'R': size.setWidth(6/width); size.setHeight(6/height); break;	 
	}

	if(type!='R')
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
	painter->save();
	//*************************************************************************
	painter->setPen( QPen( QBrush( QColor( 0, 0, 255, 255) ), 2 ) );
	painter->drawLines( _lines );
	//*************************************************************************
	painter->restore();
	return;
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
    if(!onBoarder( event->pos() ))
	{
		float rate = exp(-event->delta() / 600.0);
	    _scale *= rate;
		x0y0 = (x0y0-event->pos())*rate+event->pos();		
		x1y1 = (x1y1-event->pos())*rate+event->pos();

		reScale(x0y0, x1y1, _scale, dCenter);
		//logResizeTo();
		//itemGrid=recordItemGrid();
	}	
}


bool PathBubble1::onBoarder( QPointF pos )
{
	int x = pos.x();
	int y = pos.y();
	int half = this->Width()/2;
	int half2 = this->Height()/2;
	if( (x > half - (BOARDERSIZE+half*0.1+2)) || (x < -half + (BOARDERSIZE+half*0.1+2)) )
	{
		return true;
	}
	if( (y > half2 - (BOARDERSIZE+half2*0.1+2)) || (y < -half2 + (BOARDERSIZE+half2*0.1+2)) )
	{
		return true;
	}
	return false;
}


void PathBubble1::mouseMoveEvent( QGraphicsSceneMouseEvent *event )
{

	if(_penisOn)
	{
	    addLine( initPos, event->pos() );
		initPos = event->pos();
	}
	else if(!itemSelected.empty())	
	{	
		if(groupSelected||_grabisOn||_queryisOn||_upisOn)
	    {
			dragItem(event->pos()-event->lastPos());	
		    if(_contain.empty())
			   highlighted.clear();
	 	     draggingItem=1;
		}
		else
		{
			if(!onBoarder( event->pos() ))
		        updateAItemPos(itemSelected[0], itemSelected[1], event->pos()-event->lastPos());		
		}
	}	
	else if( onBoarder( event->pos() ) )
	{
		ItemBase::mouseMoveEvent( event );
		if (m_isResizing)
		   reScale(x0y0, x1y1, _scale, dCenter);
	}
	else
	{
	    //pan
		if(_mousePressed)
	    {
			//dCenter = dCenter + event->pos() - event->lastPos();	
			x0y0 = x0y0 + event->pos() - event->lastPos();			    
			x1y1 = x1y1 + event->pos() - event->lastPos();	

			//comput scale and dcenter
			reScale(x0y0, x1y1, _scale, dCenter);
		}
	}	
	return;
}



void PathBubble1::dragItem(QPointF Pos)
{
	for(int i=0;i<itemDragged.size();i++)
	{
		itemDragged[i].moveCenter(Pos+itemDragged[i].center());			
	}
}

void PathBubble1::updateAItemPos(int type, int id, QPointF move)
{
	itemDragged.clear();
	int width = this->realRect().width();
    int height = this->realRect().height();
    move.setX(move.x()/width);
	move.setY(move.y()/height);
    switch(type)
	{
	    case 'C': updateItemGrid(itemGrid, type, id, move+complexPos[id].center()); complexPos[id].moveCenter(move+complexPos[id].center());	      break;
		case 'P': updateItemGrid(itemGrid, type, id, move+proteinPos[id].center()); proteinPos[id].moveCenter(move+proteinPos[id].center());	      break;
	    case 'D': updateItemGrid(itemGrid, type, id, move+DnaPos[id].center());  DnaPos[id].moveCenter(move+DnaPos[id].center());		         break;
		case 'E': updateItemGrid(itemGrid, type, id, move+physicalEntityPos[id].center()); physicalEntityPos[id].moveCenter(move+physicalEntityPos[id].center());   break;
		case 'S': updateItemGrid(itemGrid, type, id, move+smallMoleculePos[id].center());  smallMoleculePos[id].moveCenter(move+smallMoleculePos[id].center());   break;
		case 'R': updateItemGrid(itemGrid, type, id, move+reactionPos[id].center());  reactionPos[id].moveCenter(move+reactionPos[id].center());   break;			      
	}			
				
}

void PathBubble1::paintItemDragged(QPainter *painter)
{   
	int width = this->realRect().width();
    int height = this->realRect().height();
	painter->setBrush(Qt::white);
	painter->setPen(Qt::NoPen);	
	for(int i=0;i<itemDragged.size();i++)
	{		
		itemDragged[i].moveCenter(itemDragged[i].center()+dCenter);
		painter->drawRect(itemDragged[i]);
		
	}
	
    painter->setPen(QColor(200,200,200,255));

	
	int j=0;

	for(int i=0; i<itemSelected.size(); i+=2)
	{
		vector<int> node;  vector<vector<int>> nodes;
		node.push_back(itemSelected[i]); node.push_back(itemSelected[i+1]);
		int type=node[0], id=node[1];
		vector<vector<int>> ed1(2,node),ed2(2,node);
		/*
		QPointF center;
		switch(type)
		{
		     case 'C': center=complexPos[id].center(); break;
			 case 'D': center=DnaPos[id].center(); break;
			 case 'E': center=physicalEntityPos[id].center(); break;
			 case 'P': center=proteinPos[id].center(); break;
			 case 'S': center=smallMoleculePos[id].center(); break;
			 case 'R': center=reactionPos[id].center(); break;
		}
		center.setX(center.x()*width-width/2);
		center.setY(center.y()*height-height/2);
		painter->drawLine(itemDragged[j].center(),center);*/

		

		QPointF center,center1;
		switch(type)
		{
		     case 'C': nodes = _complex[id]; break;
			 case 'D': nodes = _Dna[id]; break;
			 case 'E': nodes = _entity[id]; break;
			 case 'P': nodes = _protein[id]; break;
			 case 'S': nodes = _smallMolecule[id]; break;
			 case 'R': nodes = _reaction[id]; break;
		}
		center1=itemDragged[j].center();

		for(int k=0; k<nodes.size(); k++)
		{	
			ed1[0][0]=nodes[k][0]; ed1[0][1]=nodes[k][1]; ed1[1]=node;
			ed2[0]=node; ed2[1][0]=nodes[k][0];  ed2[1][1]=nodes[k][1];
			if(_edgeSelected.find(ed1)==_edgeSelected.end()&&_edgeSelected.find(ed2)==_edgeSelected.end())
			{
			   int it=nodes[k][1];
		       switch(nodes[k][0])
		       {
				 case 'C': center=complexPos[it].center(); break;
				 case 'D': center=DnaPos[it].center(); break;
				 case 'E': center=physicalEntityPos[it].center(); break;
				 case 'P': center=proteinPos[it].center(); break;
				 case 'S': center=smallMoleculePos[it].center(); break;
				 case 'R': center=reactionPos[it].center(); break;
		      }								
			  center.setX(center.x()*width-width/2);
		      center.setY(center.y()*height-height/2);

			  if(center.x()>=-width*1.3&&center1.x()>=-width*1.3)			
		           painter->drawLine(center1,center+dCenter);
			}
		}
		    
		j++;
	}	
}



void PathBubble1::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
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


	
	
}

void PathBubble1::savePathWay(QString filename, vector<QRectF> complexPos, vector<QRectF> proteinPos, vector<QRectF> smallMoleculePos, vector<QRectF> DnaPos, vector<QRectF> reactionPos, vector<QRectF> physicalEntityPos, vector<vector<int>> edge)
{
	/*QString inforstr = "Save the current scene as the xml file ";
	inforstr.append( filename );
	inforstr.append( "\n" );
	logOperation( inforstr );*/

	//QString filename;
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



	//complexPos
	/*QDomElement navNode = doc.createElement( tr("complexBlock") );
	_navi->saveAsXmlNode( doc, navNode );
	root.appendChild( navNode );*/

	//group
	//QList<Group*> gList = this->_manager->groupList();
	
	QDomElement iGroup, iBubble;
	QDomAttr gId, bId;

	QString str,tmpStr;
	QDomText text;
		

	//complex
		iGroup = doc.createElement( tr("complexBlock") );
		gId = doc.createAttribute( tr("Num") );

		str.setNum( complexNum );
		gId.setValue( str );
		iGroup.setAttributeNode( gId );

		for (int j = 1; j < complexNum+1; j ++)
		{
			iBubble = doc.createElement( tr("complex") );
			bId = doc.createAttribute( tr("j") );

			str.setNum( j );
			bId.setValue( str );
			iBubble.setAttributeNode( bId );

			//////
			QDomElement parePos = doc.createElement( tr("Name") );
		    QString cstr(_scene->_6complexName[m_pathwayID][j]); 
		
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

		for (int j = 1; j < proteinNum+1; j ++)
		{
			iBubble = doc.createElement( tr("protein") );
			bId = doc.createAttribute( tr("j") );

			str.setNum( j );
			bId.setValue( str );
			iBubble.setAttributeNode( bId );

			//////
			QDomElement parePos = doc.createElement( tr("Name") );
		    QString cstr(_scene->_7proteinName[m_pathwayID][j][0]); 
		
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

		for (int j = 1; j < physicalEntityNum+1; j ++)
		{
			iBubble = doc.createElement( tr("physicalEntity") );
			bId = doc.createAttribute( tr("j") );

			str.setNum( j );
			bId.setValue( str );
			iBubble.setAttributeNode( bId );

			//////
			QDomElement parePos = doc.createElement( tr("Name") );
		    QString cstr(_scene-> _8physicalEntityName[m_pathwayID][j][0]); 
		
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

		for (int j = 1; j < smallMoleculeNum+1; j ++)
		{
			iBubble = doc.createElement( tr("smallMolecule") );
			bId = doc.createAttribute( tr("j") );

			str.setNum( j );
			bId.setValue( str );
			iBubble.setAttributeNode( bId );

			//////
			QDomElement parePos = doc.createElement( tr("Name") );
		    QString cstr(_scene-> _9smallMoleculeName[m_pathwayID][j][0]); 
		
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

		for (int j = 1; j < DnaNum+1; j ++)
		{
			iBubble = doc.createElement( tr("Dna") );
			bId = doc.createAttribute( tr("j") );

			str.setNum( j );
			bId.setValue( str );
			iBubble.setAttributeNode( bId );

			//////
			QDomElement parePos = doc.createElement( tr("Name") );
		    QString cstr(_scene-> _9DnaName[m_pathwayID][j][0]); 
		
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

		for (int j = 1; j < reactionNum+1; j ++)
		{
			iBubble = doc.createElement( tr("reaction") );
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

	//proteinPos = _proteinPos, proteinPos = _proteinPos, smallMoleculePos=_smallMoleculePos, DnaPos=_DnaPos, reactionPos=_reactionPos, physicalEntityPos=_physicalEntityPos, edge=_edge;
	//XML
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



void PathBubble1::savePathWayinJSFile(QString filename, vector<QRectF> complexPos, vector<QRectF> proteinPos, vector<QRectF> smallMoleculePos, vector<QRectF> DnaPos, vector<QRectF> reactionPos, vector<QRectF> physicalEntityPos, vector<vector<int>> edge)
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
		for (int j = 1; j < complexNum+1; j ++)
		{
			fcString += createFeatureGeoJSON ('C', j, _scene->_6complexName[m_pathwayID][j], complexPos[j] );
		}			
		//protein	
		for (int j = 1; j < proteinNum+1; j ++)
		{
		   fcString += createFeatureGeoJSON ('P', j, _scene->_7proteinName[m_pathwayID][j][0], proteinPos[j] );
		}	
        //physical Entity		

		for (int j = 1; j < physicalEntityNum+1; j ++)
		{
			fcString += createFeatureGeoJSON ('E', j, _scene-> _8physicalEntityName[m_pathwayID][j][0], physicalEntityPos[j] );
        }		
		
	    //smallMolecule					
		for (int j = 1; j < smallMoleculeNum+1; j ++)
		{
			fcString += createFeatureGeoJSON ('S', j, _scene-> _9smallMoleculeName[m_pathwayID][j][0], smallMoleculePos[j] );
        }		
				
		//Dna
		for (int j = 1; j < DnaNum+1; j ++)
		{
			fcString += createFeatureGeoJSON ('D', j, _scene-> _9DnaName[m_pathwayID][j][0], DnaPos[j] );			
		}		
		////////////////
		//reaction	
		
		for (int j = 1; j < reactionNum+1; j ++)
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
		 QRectF rect;
		 switch(type)
	     {
		     case 'C': end=complexPos[id].center();       rect= complexPos[id];  break;
			 case 'D': end=DnaPos[id].center();           rect= DnaPos[id];    break;
			 case 'E': end=physicalEntityPos[id].center();        rect= physicalEntityPos[id];    break; 
			 case 'P': end=proteinPos[id].center();       rect= proteinPos[id];    break;
			 case 'S': end=smallMoleculePos[id].center(); rect= smallMoleculePos[id];  break;			
			 case 'R': end=reactionPos[id].center();      rect= reactionPos[id];  break;  //catalysis for bio reaction	
		 }	
		 if(end.x()!=-1&& middle.x()!=-1)
		 fcString += createFeatureGeoJSON (i, edge[i][4], edge[i][5], middle, edge[i][2], edge[i][3], end);		

		}		
		fcString.chop(2);
		fcString +="]});";

        file.write((const char *)fcString.toAscii().data());
	    QTextStream out(&file);
	    
	    file.close();
	    //proteinPos = _proteinPos, proteinPos = _proteinPos, smallMoleculePos=_smallMoleculePos, DnaPos=_DnaPos, reactionPos=_reactionPos, physicalEntityPos=_physicalEntityPos, edge=_edge;
	    //XML
}


void PathBubble1::readPathWay(QString filename, vector<QRectF> &complexPos, vector<QRectF> &proteinPos, vector<QRectF> &smallMoleculePos, vector<QRectF> &DnaPos, vector<QRectF> &reactionPos, vector<QRectF> &physicalEntityPos, vector<vector<int>> &edge)
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
	
	_scene-> _6complexName.clear();
	_scene->  _8physicalEntityName.clear();
	_scene->  _7proteinName.clear();
	_scene->  _9smallMoleculeName.clear();
	_scene-> _9DnaName.clear();
	

	QRectF rect(-1,-1,0,0);

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
	

	
	_scene-> _6complexName[m_pathwayID].push_back("");
	_scene->  _8physicalEntityName[m_pathwayID].push_back(unit);
	_scene->  _7proteinName[m_pathwayID].push_back(unit);	
	_scene->  _9smallMoleculeName[m_pathwayID].push_back(unit);
	_scene-> _9DnaName[m_pathwayID].push_back(unit);

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
					    _scene-> _6complexName[m_pathwayID].push_back(itemname);
						complexPos.push_back(nrect);
					}
					else if(tagname=="physicalEntityBlock")
					{
						unit[0]=itemname;
					    _scene-> _8physicalEntityName[m_pathwayID].push_back(unit);	
						physicalEntityPos.push_back(nrect);
					}
					else if(tagname=="proteinBlock")
					{
						unit[0]=itemname;
					    _scene->  _7proteinName[m_pathwayID].push_back(unit);
						proteinPos.push_back(nrect);	
					}
					else if(tagname=="smallMoleculeBlock")
					{
						unit[0]=itemname;
					    _scene->  _9smallMoleculeName[m_pathwayID].push_back(unit);
						smallMoleculePos.push_back(nrect);
    				}
					else if(tagname=="DnaBlock")
					{
						unit[0]=itemname;
					    _scene-> _9DnaName[m_pathwayID].push_back(unit);
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

