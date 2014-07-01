#include "subPathBubble1.h"
#include <QTextDocument>
#include <QMap>


subPathBubble1::subPathBubble1(QString orthName, int pathwayID, int size, int sizey, int x, int y, vector<int> ItemSelected, vector<vector<int>> EdgeSelected, vector<set<vector<int>>> ANodeContains, OpenGLScene* Scene, StatisticManager* manager, OpenManager* open, QString name, QString pathName, QString lastStoredGraph)
:  PathBubble1 (pathwayID, size, x, y, Scene, manager, open, name, orthName, pathName,lastStoredGraph)
{
	
	//_pieMenuState=0;
	if(pathwayID<0)
    {		 
		if(reactionNum<=0)
			return;
		float newWidth = fixedWidth, newHeight = fixedHeight;
	    getGraphSize(newWidth, newHeight);
		
		newHeight = newHeight+8;// + 16; //space for legend
		if(newHeight<170)
			newHeight = 170;
		newWidth = newWidth+8;// +150; //space for legend
		
		if(newHeight>700)	newHeight=700;
		if(newWidth>700)	newWidth=700;
	    resizeItem( newWidth, newHeight);
		initWidth = newWidth / _scale;

		/*float S=-10000;
		Pan(x0y0, x1y1, QPointF(-58,0), QPointF(-58,0), S, dCenter);	
		moveGraphToBePaint(QPointF(-58,0));	*///for legend

		if(!initialANodeHandled)
		{
			for(int i=0; i<initANodeContains.size(); i++)
			{
				set<vector<int>> aANode=initANodeContains[i];
				//convert it to new id		
				if(!ANodeContains.empty())//there is a same pathway opened
				     aggregation(aANode);
				else
					aggregation(aANode, i);
			}		
			initialANodeHandled=true;
		}

		//QString firstfileName = getFirstPathWayGraphName();	
		//savePathWay(firstfileName, complexPos, proteinPos, smallMoleculePos, DnaPos, reactionPos, physicalEntityPos, ANodePos);			
		
		QString fileName = getPathwayGraphName(pathwayGraphStored);
		if(fileName.size()>0)
		{
			savePathWay(fileName, complexPos, proteinPos, smallMoleculePos, DnaPos, reactionPos, physicalEntityPos, ANodePos);	
			if(pathwayGraphStored.empty())
			{
				QString empty;	
				pathwayGraphStored.push_back(empty);	
			}		
			pathwayGraphStored.push_back(fileName);
		}
		return;
	}
	
	initANodeContains=ANodeContains;	 
    this->edgeSelected=EdgeSelected;
	this->m_pathwayID = pathwayID;	
	this->_scene=Scene;
	edge=EdgeSelected;	
	_itemSelected=ItemSelected;

    setParentPathWayID(pathwayID);
	_pathBubbleParent=_scene->_pathBubbles[pathwayID];
	_scale=_pathBubbleParent->_scale;

	setCurrentFile(_name);	
	
    //initItemPos(ItemSelected);	
	subPathWay(ItemSelected, EdgeSelected);
 

	itemGrid=recordItemGrid(visibleItemNum, visibleCompartmentNum);	

	
    complexNum=complexPos.size(); 	
	reactionNum = reactionPos.size();		
	proteinNum = proteinPos.size();
	smallMoleculeNum = smallMoleculePos.size();
	DnaNum = DnaPos.size();
	physicalEntityNum = physicalEntityPos.size();	   
	ANodeNum = ANodePos.size();	
	
		
	updateBubbleSize_3();
		
	highlighted.clear();
	resetOriRect();
	_whole=false;
		
	if(drawOrthology)
	{
		//rb->setOrthFileName(getOrthFileName());
		pwdParser->readOrthologySymbol(getOrthFileName(), orthologySymbol);
		orthology = pwdParser->matchSymbol(orthologySymbol,  _scene->_proteinNameD[dataID]);					
		//rb->drawOrthology = drawOrthology;	
	}

	crossTalk = _scene->pwdParser->matchSymbol(_scene->crossTalkSymbol, _scene->_proteinNameD[dataID]);
	rateLimit = _scene->pwdParser->matchSymbol(_scene->rateLimitSymbol,  _scene->_proteinNameD[dataID]);

	_pathName = _labelName;
	getGraphToBePaint();
	
	float newWidth = fixedWidth, newHeight = fixedHeight;
	getGraphSize(newWidth, newHeight);
	initWidth = newWidth / _scale;
	newHeight = newHeight+20; // + 16; //space for legend	
	newWidth = newWidth+20; // +150; //space for legend

	if(newHeight>700)	newHeight=700;
	if(newWidth>700)	newWidth=700;

	resizeItem( newWidth, newHeight);

	/*float S=-10000;
	Pan(x0y0, x1y1, QPointF(-58,0), QPointF(-58,0), S, dCenter);	
	moveGraphToBePaint(QPointF(-58,0));*/

	if( initANodeContains.empty())
	{
		initialANodeHandled=true;
		StoreInitSubPathWay();
	}
}

void subPathBubble1::StoreInitSubPathWay()
{

	        QString firstfileName = getFirstPathWayGraphName();	
			savePathWay(firstfileName, complexPos, proteinPos, smallMoleculePos, DnaPos, reactionPos, physicalEntityPos, ANodePos);			
			_scene->backupItemPos(m_pathwayID, complexPos, proteinPos, smallMoleculePos, DnaPos, EmptyPos, reactionPos, physicalEntityPos, compartmentPos, ANodePos, _scale, dCenter, drawSmallmolecule);				

			QString fileName = getPathwayGraphName(pathwayGraphStored);
			if(fileName.size()>0)
			{
				savePathWay(fileName, complexPos, proteinPos, smallMoleculePos, DnaPos, reactionPos, physicalEntityPos, ANodePos);	
				if(pathwayGraphStored.empty())
				{
					QString empty;	
					pathwayGraphStored.push_back(empty);	
				}		
				pathwayGraphStored.push_back(fileName);
			}
			
			_scene->backupEdge(m_pathwayID, edge);
			_scene->backupItemPos(m_pathwayID, scene->_complexPos[m_pathwayID], scene->_proteinPos[m_pathwayID], scene->_smallMoleculePos[m_pathwayID], scene->_DnaPos[m_pathwayID], scene->_EmptyPos[m_pathwayID], scene->_reactionPos[m_pathwayID], scene->_physicalEntityPos[m_pathwayID], scene->_compartmentPos[m_pathwayID], scene->_ANodePos[m_pathwayID], _scale, dCenter, drawSmallmolecule);
			//compartmentPos.resize(compartmentPos.size()+1); why?
			//_newCompartmentPos.resize(compartmentPos.size());
			
					 
			this->_TYPE = SUBPATHBUBBLE1;
			this->_colorBoarder = TDHelper::getBoarderColorByType(_TYPE, m_pathwayID);	
			this->_colorInner = TDHelper::getInnerColorByType(_TYPE, m_pathwayID);		
	
			if(drawSmallmolecule)
			{
				//pbubble->_scale = pbubble->_scale * (0.85+0.15 * (1-sRatio));
				//pbubble->_scale = pbubble->_scale * (cScale/pbubble->_ScaleBackup);				
				//pbubble->_ScaleBackup_S = pbubble->_scale;
				_ScaleBackup_S = _scale;
				_ScaleBackup = _scale * (_pathBubbleParent->_ScaleBackup/_pathBubbleParent->_ScaleBackup_S);
			}
			else 				
			{
				//pbubble->_scale = pbubble->_scale * (cScale/pbubble->_ScaleBackup_S);
				_ScaleBackup = _scale;
				_ScaleBackup_S = _scale * (_pathBubbleParent->_ScaleBackup_S/_pathBubbleParent->_ScaleBackup);
			}
}


QRect subPathBubble1::updateBubbleSize_2()
{
    //int width=_pathBubbleParent->realRect().width(), height=_pathBubbleParent->realRect().height();	
	
	int width=realRect().width(), height=realRect().height();	


	int Px=this->pos().x(),Py=this->pos().y();
	QRect result(Px, Py, width, height);
	return result;
}

QRect subPathBubble1::updateBubbleSize_3()
{
    int width=_pathBubbleParent->realRect().width(), height=_pathBubbleParent->realRect().height();	
	
	
	int sizew = width*W*1.1, sizeh = height*H*1.1;
	if(sizew>graphReferenceSize && sizew > sizeh)
	{
		//if(fixedSize)
		//	_scale=sizew/graphReferenceSize; 
		sizew=graphReferenceSize;
	}
	if(sizeh>graphReferenceSize && sizeh > sizew)
	{
		//if(fixedSize)
	    //    _scale=sizeh/graphReferenceSize; 
		sizeh=graphReferenceSize;
	}

	if(sizew<80 && sizew < sizeh)
		sizew=80;

	if(sizeh<80 && sizeh < sizew)
		sizeh=80;

	if(sizew < sizeh*1.0/3.0) sizew = sizeh*1.0/3.0;
	else if(sizeh < sizew*1.0/3.0) sizeh = sizew*1.0/3.0;

	float sizex, sizey;

	if(!fixedSize)
	{
		
	}
	else
	{
	    //sizex=maxx*50, sizey=sumy*25;	
		if(!toplevel)
		{
			sizew=fixedWidth;
			sizeh=fixedHeight;	
		}
	}
	this->resizeItem( sizew, sizeh );
	
	//find corner 1	
	//find corner 2

	//QPointF center=(X1Y1+X0Y0)/2.0;		
    //dCenter.setX(-center.x()/sizew);
	//dCenter.setY(-center.y()/sizeh);
	dCenter = QPointF(0,0);//_pathBubbleParent->dCenter;
	

	//x1y1=center+QPointF(sizew/2.0,sizew/2.0);
	//x0y0=center+QPointF(-sizew/2.0,-sizew/2.0);

	x0y0=_pathBubbleParent->getx0y0();
	x1y1=_pathBubbleParent->getx1y1();

	
	QPointF center=QPointF(0,0);//(x1y1+x0y0)/2.0;		
    //dCenter.setX(-center.x()/sizew);
	//dCenter.setY(-center.y()/sizeh);
	float W = fabs(x1y1.x()-x0y0.x());
	float H = fabs(x1y1.y()-x0y0.y());

	x1y1=center+QPointF(W/2.0,H/2.0);
	x0y0=center+QPointF(-W/2.0,-H/2.0);
	
	int Px=this->pos().x(),Py=this->pos().y();
	QRect result(Px, Py, sizew, sizeh);
	return result;
}


subPathBubble1::~subPathBubble1()
{
	//_pieMenuState=0;
	//m_pathwayID=-1;
}


void subPathBubble1::selfDelection(PathBubble1* p0, PathBubble1* pb)
{
	PathBubble1::selfDelection(p0, pb);
}

//generate subpathway
void subPathBubble1::subPathWay(vector<int> ItemSelected, vector<vector<int>> EdgeSelected)
{ 
	//m_pathwayID parent
	    fixedWidth=250, fixedHeight=fixedWidth/1.618;
	    QString tname;
        const char * ename;
		string sname;
		float x,y,w,h;

		int pathwayID=_scene->_compartmentName.size()-1;
		  
		vector<QRectF> tempPos;	
		QPointF center;
		vector<vector<int>> _Rank0;
		vector<vector<vector<int>>> _Ranks;
		
		_scene->_proteinNameD[dataID].clear();
		_scene->_complexNameD[dataID].clear();
		_scene->_smallMoleculeNameD[dataID].clear();
		_scene->_DnaNameD[dataID].clear();
		_scene->_physicalEntityNameD[dataID].clear();
		_scene->_reactionNameD[dataID].clear();
		_scene->_ANodeName[pathwayID].clear();


	    _scene->_complexPos.push_back(tempPos), _scene->_proteinPos.push_back(tempPos), _scene->_smallMoleculePos.push_back(tempPos);
		_scene->_DnaPos.push_back(tempPos),  _scene->_EmptyPos.push_back(tempPos), _scene->_reactionPos.push_back(tempPos), _scene->_physicalEntityPos.push_back(tempPos);
		_scene->_ANodePos.push_back(tempPos);
		_scene->_compartmentPos.push_back(tempPos);
		  
		_scene->_Scale.push_back(0);
		_scene->_dCenter.push_back(center);
		//_scene->Rank0.push_back(_Rank0); 
		//_scene->Ranks.push_back(_Ranks);
		//_scene->newPos.push_back(_newPos);
		//_scene->newCompartmentPos.push_back(tempPos);
		_scene->drawSmallmolecule.push_back(false);
		
        vector<vector<QString>> newname;
		if(_scene->_compartmentName.size()<=pathwayID)
		   _scene->_compartmentName.push_back(newname);

		//initiate compartment
		int tpid=m_pathwayID;
		m_pathwayID=pathwayID;

		manuLocateCompartment(_parentPathBubbleID, m_pathwayID, compartmentPos);			
		
		m_pathwayID=tpid;
		int _pid=_scene->_complexPos.size()-1;	
		
		
		set<int> initCompartment;
	
		CompartmentContain.resize(_scene->CompartmentContain[pathwayID].size());
		CompartmentContain_0.resize(CompartmentContain.size());
		CompartmentContain_1.resize(CompartmentContain.size());

		scene->_compartmentPos[pathwayID].resize(_scene->CompartmentContain[pathwayID].size());
		compartmentPos_1.resize(_scene->CompartmentContain[pathwayID].size());
		compartmentPos_0.resize(_scene->CompartmentContain[pathwayID].size());

		//vector<QString> aname;		
		//aname.push_back("whole");
		//_scene->_compartmentName[pathwayID].push_back(aname);
		if(_scene->_ANodeContain.size() <= pathwayID)
		{
			_scene->_ANodeContain.resize(pathwayID+1);
		}
		//_scene->_ANodeContain[pathwayID].resize(1);

		set<vector<int>> newCompart;	
		set<int> mid;
		int countC,countE,countP, countS, countD, countR,countA;
		countC = countE = countP = countS = countD = countA =countR=0;
		int Did=_scene->_pathBubbles[m_pathwayID]->dataID;
		for(int i=0; i<ItemSelected.size(); i+=2)
	    {
			    int type=ItemSelected[i], id=ItemSelected[i+1];						
				CalloutNote* nodeNote=_scene->findNote(_parentPathBubbleID, type, id);	  
				bool v;
				if(nodeNote!=NULL)
					v= nodeNote->isVisible();
				vector<int> temp;
				temp.push_back(type);
				if(type!='M')
				{
					float xl,yb,xr,yt;		
					switch(type)
					{ 
						case 'C': if(nodeNote!=NULL)  nodeNote->_pid=_pid, nodeNote->_type=type, nodeNote->_id=_scene->_complexPos[pathwayID].size();
							      //_scene->_complexPos[pathwayID].push_back(_scene->_complexPos[m_pathwayID][id]);  
							      _scene->_complexPos[pathwayID].push_back( _scene->_complexPos[m_pathwayID][id] ); 
								  complexPos.push_back(                     _scene->_complexPos[m_pathwayID][id] );  
								  if(_pathBubbleParent->complexPos[id].x()<-990)		
								       complexPos[complexPos.size()-1].setX(-2000), complexPos[complexPos.size()-1].setY(-2000);

							      complexPos_0.push_back(_pathBubbleParent->complexPos_0[id]);								  
								  complexPos_1.push_back(_pathBubbleParent->complexPos_1[id]);
								  _scene->_complexNameD[dataID].push_back(_scene->_complexNameD[Did][id]);		
								  //_scene->_complexContain[pathwayID].push_back(_scene->_complexContain[m_pathwayID][id]);	//keqin come back later to add complexcontain to file
								  
								  complexID.push_back(id); 
								  temp.push_back(countC);
								  countC++;
								  break; //xl=_scene->_complexPos[m_pathwayID][id].left();        yb=_scene->_complexPos[m_pathwayID][id].bottom();        xr=_scene->_complexPos[m_pathwayID][id].right();        yt=_scene->_complexPos[m_pathwayID][id].top();       break;
						case 'E': if(nodeNote!=NULL)  nodeNote->_pid=_pid, nodeNote->_type=type, nodeNote->_id=_scene->_physicalEntityPos[pathwayID].size();
							       //_scene->_physicalEntityPos[pathwayID].push_back(_scene->_physicalEntityPos[m_pathwayID][id]);  
								   scene->_physicalEntityPos[pathwayID].push_back(  _scene->_physicalEntityPos[m_pathwayID][id]); 
								   physicalEntityPos.push_back(                     _scene->_physicalEntityPos[m_pathwayID][id] );  
								   if(_pathBubbleParent->physicalEntityPos[id].x()<-990)		
								       physicalEntityPos[physicalEntityPos.size()-1].setX(-2000), physicalEntityPos[physicalEntityPos.size()-1].setY(-2000);

								  physicalEntityPos_0.push_back(_pathBubbleParent->physicalEntityPos_0[id]);
								  physicalEntityPos_1.push_back(_pathBubbleParent->physicalEntityPos_1[id]);
							       _scene->_physicalEntityNameD[dataID].push_back(_scene->_physicalEntityNameD[Did][id]);	
								   physicalEntityID.push_back(id); 
								   temp.push_back(countE);
								   countE++;
								   break; //.left(); yb=_scene->_physicalEntityPos[m_pathwayID][id].bottom(); xr=_scene->_physicalEntityPos[m_pathwayID][id].right(); yt=_scene->_physicalEntityPos[m_pathwayID][id].top();break;
						case 'P': if(nodeNote!=NULL)  nodeNote->_pid=_pid, nodeNote->_type=type, nodeNote->_id=_scene->_proteinPos[pathwayID].size();
							       //_scene->_proteinPos[pathwayID].push_back(_scene->_proteinPos[m_pathwayID][id]);  								    
							       _scene->_proteinPos[pathwayID].push_back( _scene->_proteinPos[m_pathwayID][id]); 
								    proteinPos.push_back(                    _scene->_proteinPos[m_pathwayID][id] );  
								   if(_pathBubbleParent->proteinPos[id].x()<-990)		
								       proteinPos[proteinPos.size()-1].setX(-2000), proteinPos[proteinPos.size()-1].setY(-2000);

								   proteinPos_0.push_back(_pathBubbleParent->proteinPos_0[id]);
								   proteinPos_1.push_back(_pathBubbleParent->proteinPos_1[id]);
									_scene->_proteinNameD[dataID].push_back(_scene->_proteinNameD[Did][id]);
								   proteinID.push_back(id);  
								   temp.push_back(countP);
								   countP++;
								   break; 
						case 'S': if(nodeNote!=NULL)  nodeNote->_pid=_pid, nodeNote->_type=type, nodeNote->_id=_scene->_smallMoleculePos[pathwayID].size();
							      //_scene->_smallMoleculePos[pathwayID].push_back(_scene->_smallMoleculePos[m_pathwayID][id]);  								  
							      _scene->_smallMoleculePos[pathwayID].push_back( _scene->_smallMoleculePos[m_pathwayID][id]); 
								  smallMoleculePos.push_back(                     _scene->_smallMoleculePos[m_pathwayID][id] );  
								  if(_pathBubbleParent->smallMoleculePos[id].x()<-990)		
								       smallMoleculePos[smallMoleculePos.size()-1].setX(-2000), smallMoleculePos[smallMoleculePos.size()-1].setY(-2000);

								  smallMoleculePos_0.push_back(_pathBubbleParent->smallMoleculePos_0[id]);
								  smallMoleculePos_1.push_back(_pathBubbleParent->smallMoleculePos_1[id]);
								  _scene->_smallMoleculeNameD[dataID].push_back(_scene->_smallMoleculeNameD[Did][id]);	
								  smallMoleculeID.push_back(id);  
								  temp.push_back(countS);
								  countS++;
								  break; 
						case 'D': if(nodeNote!=NULL)  nodeNote->_pid=_pid, nodeNote->_type=type, nodeNote->_id=_scene->_DnaPos[pathwayID].size();
							      //_scene->_DnaPos[pathwayID].push_back(_scene->_DnaPos[m_pathwayID][id]);  								  
							      _scene->_DnaPos[pathwayID].push_back(  _scene->_DnaPos[m_pathwayID][id]);  	
								   DnaPos.push_back(                     _scene->_DnaPos[m_pathwayID][id] );  
								  if(_pathBubbleParent->DnaPos[id].x()<-990)		
								       DnaPos[DnaPos.size()-1].setX(-2000), DnaPos[DnaPos.size()-1].setY(-2000);

								  DnaPos_0.push_back(_pathBubbleParent->DnaPos_0[id]);
								  DnaPos_1.push_back(_pathBubbleParent->DnaPos_1[id]);
								  _scene->_DnaNameD[dataID].push_back(_scene->_DnaNameD[Did][id]);	
								  DnaID.push_back(id); 
								  temp.push_back(countD);
								  countD++;
								  break; 								  
						case 'R': if(nodeNote!=NULL)  nodeNote->_pid=_pid, nodeNote->_type=type, nodeNote->_id=_scene->_reactionPos[pathwayID].size();
							      //_scene->_reactionPos[pathwayID].push_back(_scene->_reactionPos[m_pathwayID][id]);  								  
							      _scene->_reactionPos[pathwayID].push_back(_scene->_reactionPos[m_pathwayID][id]);  	
								   reactionPos.push_back(                     _scene->_reactionPos[m_pathwayID][id] );  
								  if(_pathBubbleParent->reactionPos[id].x()<-990)		
								       reactionPos[reactionPos.size()-1].setX(-2000), reactionPos[reactionPos.size()-1].setY(-2000);

								  reactionPos_0.push_back(_pathBubbleParent->reactionPos_0[id]);
								  reactionPos_1.push_back(_pathBubbleParent->reactionPos_1[id]);
								  _scene->_reactionNameD[dataID].push_back(_scene->_reactionNameD[Did][id]);	
								  reactionID.push_back(id);  								 
					              temp.push_back(countR);
								  countR++;
								  break; 
						case 'L':  if(nodeNote!=NULL)  nodeNote->_pid=_pid, nodeNote->_type=type, nodeNote->_id=_scene->_ANodePos[pathwayID].size();
							      //_scene->_ANodePos[pathwayID].push_back(_scene->_ANodePos[m_pathwayID][id]);  								  
							      _scene->_ANodePos[pathwayID].push_back(_scene->_ANodePos[m_pathwayID][id]); 
								   ANodePos.push_back(                     _scene->_ANodePos[m_pathwayID][id] );  
								  if(_pathBubbleParent->ANodePos[id].x()<-990)		
								       ANodePos[ANodePos.size()-1].setX(-2000), ANodePos[ANodePos.size()-1].setY(-2000);

								  ANodePos_0.push_back(_pathBubbleParent->ANodePos_0[id]);
								  ANodePos_1.push_back(_pathBubbleParent->ANodePos_1[id]);
								  _scene->_ANodeName[pathwayID].push_back(_scene->_ANodeName[m_pathwayID][id]);	
								  ANodeID.push_back(id);  
								  _scene->_ANodeContain[pathwayID].push_back(scene->_ANodeContain[m_pathwayID][id]);
					              temp.push_back(countA);
								  countA++;
								  break; 
					}
					int cid=whichCompartment(m_pathwayID, type, id);
					if(cid>=0)
					{   
						if( pathwayID < _scene->CompartmentContain.size() && cid < _scene->CompartmentContain[pathwayID].size() ) //keqin may be a bug here
						     _scene->CompartmentContain[pathwayID][cid].insert(temp);

						if( cid < CompartmentContain.size() ) //keqin may be a bug here
  					         CompartmentContain[cid].insert(temp);
					}
					newCompart.insert(temp);
			}
			else
			{
				mid.insert(id);
				if(nodeNote!=NULL)  nodeNote->_pid=_pid, nodeNote->_type=type, nodeNote->_id=id;
				initCompartment.insert(id);      
			}
		}
		_scene->CompartmentContain[pathwayID].push_back(newCompart);

		CompartmentContain.push_back(newCompart);
		//if(drawSmallmolecule)		
		CompartmentContain_1 = CompartmentContain; //push_back(newCompart);
		//else 
		CompartmentContain_0 = CompartmentContain;

		bool mflag=true;
		for(int i=0; i<CompartmentContain.size()-1; i++)
	    {
			if(!CompartmentContain[i].empty())
			{
				_scene->_compartmentPos[pathwayID][i]=_scene->_compartmentPos[m_pathwayID][i];			
				compartmentPos[i]=_scene->_compartmentPos[m_pathwayID][i];										
				compartmentPos_0[i]=_pathBubbleParent->compartmentPos_0[i];
				compartmentPos_1[i]=_pathBubbleParent->compartmentPos_1[i];
				if(mid.find(i)==mid.end())
				{    
					ItemSelected.push_back('M'); ItemSelected.push_back(i);
				}
				if(initCompartment.find(i)==initCompartment.end())
				{
					virtualCompartment.insert(i);
					compartmentPos[i].setX(-1000);
				    _scene->_compartmentPos[pathwayID][i].setX(-1000);	
					mflag=false;				
				}
			}
			else 
			{
				compartmentPos[i].setX(-1000);
				_scene->_compartmentPos[pathwayID][i].setX(-1000);	
			}
	   }
		//replace node id in ANodeContain
		for(int m=0; m<_scene->_ANodeContain[pathwayID].size(); m++)
		{

			for(int n=0; n<_scene->_ANodeContain[pathwayID][m].size(); n=n+2)
			{
			     int type=_scene->_ANodeContain[pathwayID][m][n], id=_scene->_ANodeContain[pathwayID][m][n+1];
				 int Kid;
				 switch(type)
			     {
					   case 'C': for(int k=0; k<complexID.size();k++) 
							     {  if(id==complexID[k])  
					                {
								       Kid=k;   break;
								    }
							     }     
					             break;
					   case 'D': for(int k=0; k<DnaID.size();k++) 
								 { if(id==DnaID[k]) { Kid=k; break;} }      break;
			           case 'E': for(int k=0; k<physicalEntityID.size();k++) 
								 { if(id==physicalEntityID[k])  { Kid=k; break; } }      break;
			           case 'P': for(int k=0; k<proteinID.size();k++)
								 { if(id==proteinID[k]) { Kid=k; break; } }     break;
			           case 'S': for(int k=0; k<smallMoleculeID.size();k++)
								 { if(id==smallMoleculeID[k]) { Kid=k; break; } }     break;		
			           case 'L': for(int k=0; k<ANodeID.size();k++)
								 { if(id==ANodeID[k]) { Kid=k; break; } }     break;			
		               case 'R': for(int k=0; k<reactionID.size();k++)
								 { if(id==reactionID[k]) { Kid=k; break; } }    break;
				       
				 }
				 _scene->_ANodeContain[pathwayID][m][n+1]=Kid;  
			}
		}			              


	QString Name;
	int count1=0;
	for(int m=0; m<_scene->_ANodeContain[pathwayID].size(); m++)
	{
		for(int n=0; n<_scene->_ANodeContain[pathwayID][m].size(); n=n+2)
		{
			int type=_scene->_ANodeContain[pathwayID][m][n], id=_scene->_ANodeContain[pathwayID][m][n+1];
			QString end;
			switch(type)
			{
				case 'C': Name = Name + _scene->_complexNameD[dataID][id][0];  break;
				case 'E': Name = Name + _scene->_physicalEntityNameD[dataID][id][0];  break;
				case 'P': Name = Name + _scene->_proteinNameD[dataID][id][0];  break;
				case 'S': Name = Name + _scene->_smallMoleculeNameD[dataID][id][0];  break;
				case 'D': Name = Name + _scene->_DnaNameD[dataID][id][0];  break;
				case 'R': Name = Name + _scene->_reactionNameD[dataID][id][1];  
						  break;
				case 'M': Name = Name + _scene->_compartmentName[m_pathwayID][id][0];  break;
				case 'L': Name = Name + _scene->_ANodeName[m_pathwayID][id][0];  break;
			}
			if(++count1!=highlighted.size())
				Name = Name + ',';
		}
		_scene->_ANodeName[pathwayID][m][1]=Name;  
	}

	Dis = initItemSize(ItemSelected, mflag, pathwayID, m_pathwayID);//get new x0y0 x1y1

	itemPos.clear();	
    
	int Cnum,Enum,Pnum,Snum,Dnum,Rnum, Anum;
	Cnum=Enum=Pnum=Snum=Dnum=Rnum=Anum=0;
	for(int i=0; i<itemInfo.size(); i++)
	{
	    int type=itemInfo[i][0], id=itemInfo[i][1];
		switch(type)
		{
		    case 'C': itemInfo[i][2]=Cnum; itemPos.push_back(_scene->_complexPos[pathwayID][Cnum++]);  break;
		    case 'E': itemInfo[i][2]=Enum; itemPos.push_back(_scene->_physicalEntityPos[pathwayID][Enum++]);  break;
		    case 'P': itemInfo[i][2]=Pnum;  itemPos.push_back(_scene->_proteinPos[pathwayID][Pnum++]);  break;				
		    case 'S': itemInfo[i][2]=Snum; itemPos.push_back(_scene->_smallMoleculePos[pathwayID][Snum++]);  break;
		    case 'D': itemInfo[i][2]=Dnum;  itemPos.push_back(_scene->_DnaPos[pathwayID][Dnum++]);  break;
		    case 'R': itemInfo[i][2]=Rnum;  itemPos.push_back(_scene->_reactionPos[pathwayID][Rnum++]);  break;
			case 'L': itemInfo[i][2]=Anum;  itemPos.push_back(_scene->_ANodePos[pathwayID][Anum++]);  break;
			case 'M': itemInfo[i][2]=id;    itemPos.push_back(_scene->_compartmentPos[pathwayID][id]);  break;
		}		
	}	
	
    for(int i=0; i<edgeSelected.size(); i++)
	{
         //if( itemSet.find(item) != itemSet.end() && itemSet.find(item2) != itemSet.end() )
		 for(int j=0; j< 6; j=j+2)
		 {
			int type=edgeSelected[i][j], id=edgeSelected[i][j+1];
			bool flag=false;
		
			switch(type)
			{
			 case 'C': for(int k=0; k<complexID.size();k++) 
					   {  
						   if(id==complexID[k])  
					           edgeSelected[i][j+1]=k,flag=true;  				           
					   }     
					   if(!flag)	 
							   edgeSelected[i][j+1]=-1; 
					   break;
			 case 'D': for(int k=0; k<DnaID.size();k++) { if(id==DnaID[k])  edgeSelected[i][j+1]=k,flag=true;   }        if(!flag)	 edgeSelected[i][j+1]=-1;    break;
			 case 'E': for(int k=0; k<physicalEntityID.size();k++) { if(id==physicalEntityID[k])  edgeSelected[i][j+1]=k,flag=true;    }     if(!flag)	 edgeSelected[i][j+1]=-1;    break; 
			 case 'P': for(int k=0; k<proteinID.size();k++) { if(id==proteinID[k])  edgeSelected[i][j+1]=k,flag=true;    }    if(!flag)	 edgeSelected[i][j+1]=-1;   break;
			 case 'S': for(int k=0; k<smallMoleculeID.size();k++) { if(id==smallMoleculeID[k])  edgeSelected[i][j+1]=k, flag=true;   }   if(!flag)	 edgeSelected[i][j+1]=-1; break;			
			 case 'L': for(int k=0; k<ANodeID.size();k++) { if(id==ANodeID[k])  edgeSelected[i][j+1]=k, flag=true;   }   if(!flag)	 edgeSelected[i][j+1]=-1; break;			
		     case 'R': 
				 if(j==4)
				 {
					 for(int k=0; k<reactionID.size();k++) 
				     { 
					    if(id==reactionID[k])  
						  edgeSelected[i][j+1]=k, flag=true;  
				     }   
				     if(!flag)
					 edgeSelected[i][j+1]=-1;
				 }
				 else if(j==2)
				 {
					 int ID=edgeSelected[i][5];
				     for(int k=0; k<reactionID.size();k++) 
				     { 
					    if(ID==reactionID[k])  
						  edgeSelected[i][j+1]=k, flag=true;  
				     }   
				     if(!flag)
					 edgeSelected[i][j+1]=-1;				 
				 }
				 break;							 
			}			
		}
	}	
	edge=edgeSelected;

	

	vector<vector<int>> empty;
	vector<vector<vector<int>>> temp11(proteinID.size()+1, empty);
	_protein=temp11;

	vector<vector<vector<int>>> temp12(complexID.size()+1, empty);
	_complex=temp12;
	
	vector<vector<vector<int>>> temp13(physicalEntityID.size()+1, empty);
	_entity=temp13;

	vector<vector<vector<int>>> temp14(smallMoleculeID.size()+1, empty);
	_smallMolecule=temp14;

	vector<vector<vector<int>>> temp15(DnaID.size()+1, empty);
	_Dna=temp15;

	vector<vector<vector<int>>> temp16(reactionID.size()+1, empty);
	_reaction=temp16;

	vector<vector<vector<int>>> temp17(ANodeID.size()+1, empty);
	_ANode=temp17;


	//vector<vector<vector<int>>> temp17(scene->_compartmentNameD[dataID].size(), empty);
	//_compartment=temp17;

	//vector<vector<vector<int>>> temp20(EmptyNum+1, empty);
	//_Empty=temp20;
	
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
			     _reaction[middle[1]].push_back(ed);  
			
			
			 ed=end; ed[2]='A'; ed[3]=1; 
			 if(middle[1]>=0)
			     _reaction[middle[1]].push_back(ed);       
		    
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
				case 'L': _ANode[end[1]].push_back(ed);    break;
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
			 if(end[1]>=0 && end[1]<_reaction.size())
			     _reaction[end[1]].push_back(ed);      			 
		 }
	}   


	for(int i=0; i<itemPos.size(); i++)
	{
		int width=this->realRect().width();
	    int height=this->realRect().height();
	        
		if(fixedSize)
		{
		    width = fixedWidth;
			height = fixedHeight;
		}

		x=itemPos[i].x();
		y=itemPos[i].y();
		w=itemPos[i].width();
		h=itemPos[i].height();
        x=(x-0.5)*width; w=w*width;
		y=(y-0.5)*height; h=h*height;		
		itemPos[i]=QRectF(x,y,w,h);
	}		
	this->m_pathwayID = _scene->_complexPos.size()-1;	
}

void subPathBubble1::manuLocateCompartment(int parentID, int pID, vector<QRectF> &CompartmentPos)	
{
	set<vector<int>> empty;
	vector<QString> temp;
	temp.push_back("");

	for(int i=0; i<_scene->_compartmentName[parentID].size(); i++)
	{
		_scene->_compartmentName[pID].push_back(_scene->_compartmentName[parentID][i]);

		_scene->CompartmentContain[pID].push_back(empty);
		if(_scene->_compartmentName[parentID][i][0]=="whole")
		    CompartmentPos.push_back(QRectF(0, 0, 1, 1));//left
		else
			CompartmentPos.push_back(QRectF(23/2587.0, 102/1333.0, (439-23)/2587.0, (1311-102)/1333.0));//left
	}
}


QPointF subPathBubble1::initItemSize(vector<int> ItemSelected, bool mflag, int pathwayID, int m_pathwayID)
{
	//vector<int> item(2,0);
	int x0,y0,x1,y1;

	x1=y1=-100000;
	x0=y0=100000;

	L=100000, B=100000;
	float R=-100000, T=-100000;
	vector<int> node(6,0);	
	
	for(int i=0; i<ItemSelected.size(); i+=2)
	{
	    int type=ItemSelected[i], id=ItemSelected[i+1];		
		QRectF nRect,nPos;
		float XL,YB,XR,YT;	
		float xl,yb,xr,yt;	
		int Did=_scene->_pathBubbles[m_pathwayID]->dataID;
		if( !(type=='M' && virtualCompartment.find(id)!=virtualCompartment.end()) ) 		
		{
			switch(type)
			{ 
				case 'C': nPos=_scene->_complexPos[m_pathwayID][id];						  		                  
						  nRect=getNodeRect( nPos, _scene->_complexNameD[Did][id][0], type, fixedSize,  dCenter, _scale);
						  break;
				case 'E': nPos=_scene->_physicalEntityPos[m_pathwayID][id];					      				  		                  
						  nRect=getNodeRect( nPos, _scene->_physicalEntityNameD[Did][id][0], type, fixedSize,  dCenter, _scale);					      
					      break;
				case 'P': nPos=_scene->_proteinPos[m_pathwayID][id];					      				  		                  
						  nRect=getNodeRect( nPos, _scene->_proteinNameD[Did][id][0], type, fixedSize,  dCenter, _scale);
					      break; 
				case 'S': nPos=_scene->_smallMoleculePos[m_pathwayID][id];					      				  		                  
						  nRect=getNodeRect( nPos, _scene->_smallMoleculeNameD[Did][id][0], type, fixedSize,  dCenter, _scale);						  
					      break; 
				case 'D': nPos=_scene->_DnaPos[m_pathwayID][id];					      				  		                  
						  nRect=getNodeRect( nPos, _scene->_DnaNameD[Did][id][0], type, fixedSize,  dCenter, _scale);						  
					      break; 
				case 'R': nPos=_scene->_reactionPos[m_pathwayID][id];					      				  		                  
						  nRect=getNodeRect( nPos, _scene->_reactionNameD[Did][id][0], type, fixedSize,  dCenter, _scale);						  
					      break;
				case 'M': nPos=_scene->_compartmentPos[m_pathwayID][id];					      				  		                  
						  nRect=getNodeRect( nPos, _scene->_compartmentName[m_pathwayID][id][0], type, fixedSize,  dCenter, _scale);						 
					      break;
			}
			XL=nPos.left();        YB=nPos.bottom();       XR=nPos.right();        YT=nPos.top();   
		    xl=nRect.left();       yb=nRect.bottom();      xr=nRect.right();       yt=nRect.top();    

			if(L>XL&&XL>-990.99)  L=XL;  if(R<XR) R=XR;  
			if(B>YT&&YT>-990.99)  B=YT;  if(T<YB) T=YB;	

			if(x0>xl&&XL>-990.99) x0=xl;  if(x1<xr) x1=xr;  
			if(y0>yt&&YT>-990.99) y0=yt;  if(y1<yb) y1=yb;	
		
			node[0]=type, node[1]=id, node[2]=-1, node[3]=-1, node[4]=-1, node[5]=m_pathwayID;
			//if(type!='M') //keqin
			itemInfo.push_back(node);
		}
	}	
	//QPointF x0y0=_pathBubbleParent->getx0y0(),	x1y1=_pathBubbleParent->getx1y1();
	X0Y0=QPointF(x0,y0); X1Y1=QPointF(x1,y1);
	QPointF dcenter((L+R)/2,(T+B)/2);// center=(x1y1+x0y0)/2.0;

	//dcenter.setX(center.x()/this->Width());
	//dcenter.setY(center.y()/this->Height());

	QPointF dis= dcenter-QPointF(0.5,0.5);//-_pathBubbleParent->dCenter;

	W = R-L;  H = T-B;
	int countC,countE,countP, countS, countD, countR,countL;
	countC = countE = countP = countS = countD = countL =countR=0;
	set<int> cid;
	for(int i=0; i<ItemSelected.size(); i+=2)
	{
			    int type=ItemSelected[i], id=ItemSelected[i+1];		
				if(type!='M')
				{
					float xl,yb,xr,yt;		
					switch(type)
					{ 
						case 'C': if(_scene->_complexPos[pathwayID][countC].x()>-990)
									  _scene->_complexPos[pathwayID][countC].moveCenter(_scene->_complexPos[pathwayID][countC].center()- dis); 

							      if(complexPos[countC].x()>-990)
								       complexPos[countC].moveCenter(complexPos[countC].center()- dis); 
								  
								  if(complexPos_0[countC].x()>-990)
								      complexPos_0[countC].moveCenter(complexPos_0[countC].center()- dis); 

								  if(complexPos_1[countC].x()>-990)
								      complexPos_1[countC].moveCenter(complexPos_1[countC].center()- dis); 

								  countC++;
								  break; //xl=_scene->_complexPos[m_pathwayID][id].left();        yb=_scene->_complexPos[m_pathwayID][id].bottom();        xr=_scene->_complexPos[m_pathwayID][id].right();        yt=_scene->_complexPos[m_pathwayID][id].top();       break;
						case 'E': 
							      if(_scene->_physicalEntityPos[pathwayID][countE].x()>-990)
									  _scene->_physicalEntityPos[pathwayID][countE].moveCenter(_scene->_physicalEntityPos[pathwayID][countE].center()- dis); 
							      if(physicalEntityPos[countE].x()>-990)
								       physicalEntityPos[countE].moveCenter(physicalEntityPos[countE].center()- dis); 
								  
								  if(physicalEntityPos_0[countE].x()>-990)
								      physicalEntityPos_0[countE].moveCenter(physicalEntityPos_0[countE].center()- dis); 

								  if(physicalEntityPos_1[countE].x()>-990)
								      physicalEntityPos_1[countE].moveCenter(physicalEntityPos_1[countE].center()- dis); 

							       /*scene->_physicalEntityPos[pathwayID].push_back(  _scene->_physicalEntityPos[m_pathwayID][id]); 
								   physicalEntityPos.push_back(                     _scene->_physicalEntityPos[m_pathwayID][id] );  
								   if(_pathBubbleParent->physicalEntityPos[id].x()<-990)		
								       physicalEntityPos[physicalEntityPos.size()-1].setX(-2000), physicalEntityPos[physicalEntityPos.size()-1].setY(-2000);

								  physicalEntityPos_0.push_back(_pathBubbleParent->physicalEntityPos_0[id]);
								  physicalEntityPos_1.push_back(_pathBubbleParent->physicalEntityPos_1[id]);*/
								  countE++;
								  break; //.left(); yb=_scene->_physicalEntityPos[m_pathwayID][id].bottom(); xr=_scene->_physicalEntityPos[m_pathwayID][id].right(); yt=_scene->_physicalEntityPos[m_pathwayID][id].top();break;
						case 'P':  /* _scene->_proteinPos[pathwayID].push_back( _scene->_proteinPos[m_pathwayID][id]); 
								    proteinPos.push_back(                    _scene->_proteinPos[m_pathwayID][id] );  
								   if(_pathBubbleParent->proteinPos[id].x()<-990)		
								       proteinPos[proteinPos.size()-1].setX(-2000), proteinPos[proteinPos.size()-1].setY(-2000);

								   proteinPos_0.push_back(_pathBubbleParent->proteinPos_0[id]);
								   proteinPos_1.push_back(_pathBubbleParent->proteinPos_1[id]);*/
							      if(_scene->_proteinPos[pathwayID][countP].x()>-990)
									  _scene->_proteinPos[pathwayID][countP].moveCenter(_scene->_proteinPos[pathwayID][countP].center()- dis); 
							      if(proteinPos[countP].x()>-990)
								       proteinPos[countP].moveCenter(proteinPos[countP].center()- dis); 
								  
								  if(proteinPos_0[countP].x()>-990)
								      proteinPos_0[countP].moveCenter(proteinPos_0[countP].center()- dis); 

								  if(proteinPos_1[countP].x()>-990)
								      proteinPos_1[countP].moveCenter(proteinPos_1[countP].center()- dis); 
								   countP++;
								   break; 
						case 'S': 
							      if(_scene->_smallMoleculePos[pathwayID][countS].x()>-990)
									  _scene->_smallMoleculePos[pathwayID][countS].moveCenter(_scene->_smallMoleculePos[pathwayID][countS].center()- dis); 
							      if(smallMoleculePos[countS].x()>-990)
								       smallMoleculePos[countS].moveCenter(smallMoleculePos[countS].center()- dis); 
								  
								  if(smallMoleculePos_0[countS].x()>-990)
								      smallMoleculePos_0[countS].moveCenter(smallMoleculePos_0[countS].center()- dis); 

								  if(smallMoleculePos_1[countS].x()>-990)
								      smallMoleculePos_1[countS].moveCenter(smallMoleculePos_1[countS].center()- dis); 
								  /* _scene->_smallMoleculePos[pathwayID].push_back( _scene->_smallMoleculePos[m_pathwayID][id]); 
								  smallMoleculePos.push_back(                     _scene->_smallMoleculePos[m_pathwayID][id] );  
								  if(_pathBubbleParent->smallMoleculePos[id].x()<-990)		
								       smallMoleculePos[smallMoleculePos.size()-1].setX(-2000), smallMoleculePos[smallMoleculePos.size()-1].setY(-2000);

								  smallMoleculePos_0.push_back(_pathBubbleParent->smallMoleculePos_0[id]);
								  smallMoleculePos_1.push_back(_pathBubbleParent->smallMoleculePos_1[id]);*/
								  countS++;
								  break; 
						case 'D': if(_scene->_DnaPos[pathwayID][countD].x()>-990)
									  _scene->_DnaPos[pathwayID][countD].moveCenter(_scene->_DnaPos[pathwayID][countD].center()- dis); 
							      if(DnaPos[countD].x()>-990)
								       DnaPos[countD].moveCenter(DnaPos[countD].center()- dis); 
								  
								  if(DnaPos_0[countD].x()>-990)
								      DnaPos_0[countD].moveCenter(DnaPos_0[countD].center()- dis); 

								  if(DnaPos_1[countD].x()>-990)
								      DnaPos_1[countD].moveCenter(DnaPos_1[countD].center()- dis); 
							      /*_scene->_DnaPos[pathwayID].push_back(  _scene->_DnaPos[m_pathwayID][id]);  	
								   DnaPos.push_back(                     _scene->_DnaPos[m_pathwayID][id] );  
								  if(_pathBubbleParent->DnaPos[id].x()<-990)		
								       DnaPos[DnaPos.size()-1].setX(-2000), DnaPos[DnaPos.size()-1].setY(-2000);

								  DnaPos_0.push_back(_pathBubbleParent->DnaPos_0[id]);
								  DnaPos_1.push_back(_pathBubbleParent->DnaPos_1[id]);*/
								  countD++;
								  break; 								  
						case 'R': 
							      if(_scene->_reactionPos[pathwayID][countR].x()>-990)
									  _scene->_reactionPos[pathwayID][countR].moveCenter(_scene->_reactionPos[pathwayID][countR].center()- dis); 
							      if(reactionPos[countR].x()>-990)
								       reactionPos[countR].moveCenter(reactionPos[countR].center()- dis); 
								  
								  if(reactionPos_0[countR].x()>-990)
								      reactionPos_0[countR].moveCenter(reactionPos_0[countR].center()- dis); 

								  if(reactionPos_1[countR].x()>-990)
								      reactionPos_1[countR].moveCenter(reactionPos_1[countR].center()- dis); 
								  /*_scene->_reactionPos[pathwayID].push_back(_scene->_reactionPos[m_pathwayID][id]);  	
								   reactionPos.push_back(                     _scene->_reactionPos[m_pathwayID][id] );  
								  if(_pathBubbleParent->reactionPos[id].x()<-990)		
								       reactionPos[reactionPos.size()-1].setX(-2000), reactionPos[reactionPos.size()-1].setY(-2000);

								  reactionPos_0.push_back(_pathBubbleParent->reactionPos_0[id]);
								  reactionPos_1.push_back(_pathBubbleParent->reactionPos_1[id]);*/
								  countR++;
								  break; 
						case 'L': 
							     if(_scene->_ANodePos[pathwayID][countL].x()>-990)
									  _scene->_ANodePos[pathwayID][countL].moveCenter(_scene->_ANodePos[pathwayID][countL].center()- dis); 
							      if(ANodePos[countL].x()>-990)
								       ANodePos[countL].moveCenter(ANodePos[countL].center()- dis); 
								  
								  if(ANodePos_0[countL].x()>-990)
								      ANodePos_0[countL].moveCenter(ANodePos_0[countL].center()- dis); 

								  if(ANodePos_1[countL].x()>-990)
								      ANodePos_1[countL].moveCenter(ANodePos_1[countL].center()- dis); 
								  /*_scene->_ANodePos[pathwayID].push_back(_scene->_ANodePos[m_pathwayID][id]); 
								   ANodePos.push_back(                     _scene->_ANodePos[m_pathwayID][id] );  
								  if(_pathBubbleParent->ANodePos[id].x()<-990)		
								       ANodePos[ANodePos.size()-1].setX(-2000), ANodePos[ANodePos.size()-1].setY(-2000);

								  ANodePos_0.push_back(_pathBubbleParent->ANodePos_0[id]);
								  ANodePos_1.push_back(_pathBubbleParent->ANodePos_1[id]);
								  _scene->_ANodeName[pathwayID].push_back(_scene->_ANodeName[m_pathwayID][id]);*/
								  countL++;
								  break; 
					}
					
			}
			else
			{
				if(cid.find(id)==cid.end())
				{
					cid.insert(id);
					 if(_scene->_compartmentPos[pathwayID][id].x()>-990)
						_scene->_compartmentPos[pathwayID][id].moveCenter(_scene->_compartmentPos[pathwayID][id].center()- dis); 
					if(compartmentPos[id].x()>-990)
						compartmentPos[id].moveCenter(compartmentPos[id].center()- dis); 
								  
					if(compartmentPos_0[id].x()>-990)
						compartmentPos_0[id].moveCenter(compartmentPos_0[id].center()- dis); 

					if(compartmentPos_1[id].x()>-990)
						compartmentPos_1[id].moveCenter(compartmentPos_1[id].center()- dis); 
				}

				/*_scene->_compartmentPos[pathwayID][i]=_scene->_compartmentPos[m_pathwayID][i];			
				compartmentPos[i]=_scene->_compartmentPos[m_pathwayID][i];										
				compartmentPos_0[i]=_pathBubbleParent->compartmentPos_0[i];
				compartmentPos_1[i]=_pathBubbleParent->compartmentPos_1[i];		*/
			}
		}
		return dis;

}


void subPathBubble1::paintItem(QPainter *painter)
{

}


void subPathBubble1::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	PathBubble1::mouseReleaseEvent( event );
}


void subPathBubble1::hoverMoveEvent( QGraphicsSceneHoverEvent *event )
{   //static bool grabon=false;
	
	PathBubble1::hoverMoveEvent(event);
	
}


void subPathBubble1::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	PathBubble1::mousePressEvent(event);
	
}

void subPathBubble1::wheelEvent(QGraphicsSceneWheelEvent *event)
{
	PathBubble1::wheelEvent(event);  
}

void subPathBubble1::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	int size=_pathBubbles.size();
	PathBubble1::paint(painter, option, widget);	
	if(!toplevel && !initialANodeHandled)
	{
		StoreInitSubPathWay();
		for(int i=0; i<initANodeContains.size(); i++)
		{
			set<vector<int>> aANode=initANodeContains[i];
			//convert it to new id
			set<vector<int>> ANodes;
			for(int i=0; i<itemInfo.size(); i++)
			{
				int type=itemInfo[i][0], id0=itemInfo[i][1], id1=itemInfo[i][2];
				vector<int> item0, item1; 
				item0.push_back(type); item0.push_back(id0);
				item1.push_back(type); item1.push_back(id1);

				if(aANode.find(item0)!=aANode.end())
				{
				   ANodes.insert(item1);			
				}		
			}
			aggregation(ANodes);
		}
		
		initialANodeHandled=true;
	}
}


void subPathBubble1::saveAsXmlNode( QDomDocument &doc, QDomElement &node )
{
	QDomText text;
	QString str, tmpStr;
	
	PathBubble1::saveAsXmlNode( doc, node );
	if(!toplevel && isVisible())
	{
		QDomElement initEdges = doc.createElement( tr("initEdges") );
		
		for (int i = 0; i < _scene->_edge[m_pathwayID].size(); i ++)
		{	
			//QDomElement Edge = doc.createElement( tr("start") );
        	QDomElement descentPos = doc.createElement( tr("edge") );
			str = "";
			for(int j=0; j< 6; j=j+2)
			{
				int type=_scene->_edge[m_pathwayID][i][j], id=_scene->_edge[m_pathwayID][i][j+1];		
				tmpStr.setNum( type );
				str += tmpStr;
				str += ",";
				tmpStr.setNum( id );
				str += tmpStr;			
				str += ",";
			}
			tmpStr.setNum( _scene->_edge[m_pathwayID][i][6] );
			str += tmpStr;
			str += ",";				
			str += "";
			text = doc.createTextNode( str );
			descentPos.appendChild( text );
			//Edge.appendChild( descentPos );
			initEdges.appendChild( descentPos );
		}				
		node.appendChild( initEdges );

		QDomElement initItems = doc.createElement( tr("initItems") );		
		for (int i = 0; i < _itemSelected.size(); i=i+2)
		{	
			//QDomElement Edge = doc.createElement( tr("start") );
        	QDomElement descentPos = doc.createElement( tr("item") );
			str = "";
			int type=_itemSelected[i], id=_itemSelected[i+1];		
			tmpStr.setNum( type );
			str += tmpStr;
			str += ",";
			tmpStr.setNum( id );
			str += tmpStr;			
			str += ",";			
			str += "";
			text = doc.createTextNode( str );
			descentPos.appendChild( text );			
			initItems.appendChild( descentPos );
		}				
		node.appendChild( initItems );
	}
}


void subPathBubble1::deleteSelectedItemsbyParent(QGraphicsScene *scene)
{
	if (!scene)
		return;

	//if(_pathBubbles==NULL)
	for (int i = 0; i < _pathBubbles.size(); i ++)
	{		
		_pathBubbles[i]->deleteSelectedItemsbyParent(scene);
	}
	if(_pathBubbleParent)
	{
		for (int i = 0; i < _pathBubbleParent->_pathBubbles.size(); i ++)
		{
			if(_pathBubbleParent->_pathBubbles[i]==this)
            {	
				int id=_pathBubbleParent->_pathBubbles[i]->m_pathwayID;
				_pathBubbleParent->_pathBubbles.removeOne(this);
		        _pathBubbleParent->_groupPoint.removeOne(_pathBubbleParent->_groupPoint[i]);
				break;
			}
		}
	}
	//m_pathwayID=-1;
	_pathBubbles.clear();
	_conPoint.clear();

	ItemBase *itemBase = dynamic_cast<ItemBase *>(this);
	itemBase->setVisible(false);
	itemBase->hide();
	itemBase->properRemove();	
	//delete itemBase;
	deleteSelectedItems(_scene);	
	_pathBubbleParent=NULL;
}



void subPathBubble1::rebuildBubbleConnect( QDomElement node )
{
	//_codeParent = NULL;
	_pathBubbles.clear();

	QList<ItemBase*> itemList;
	ItemBase* item;
	subPathBubble1* tmp;
	QString name;
	int i;

	QDomNodeList paraNode;
	QDomElement tmpNode;

	itemList = this->_scene->getGroupManager()->allMembers();

	paraNode = node.elementsByTagName( tr("ParentBubble") );
	
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
			if ( item->getType() == PATHBUBBLE1 )
			{
				PathBubble1* tmp = dynamic_cast<PathBubble1*>(item);
				if (//ptmp->getCurrentFilename() == name && 
					fabs(tmp->pos().x() - x) < 3
					&& fabs(tmp->pos().y() - y) < 3 )
				{
					this->_pathBubbleParent = tmp;
					break;
				}
			}
		}
	}
	
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
				if ( item->getType() == SUBPATHBUBBLE1)
				{
					tmp = dynamic_cast<subPathBubble1*>(item);
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

	