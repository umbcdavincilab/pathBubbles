

#include "SolParser.h"

#define BUFSIZE 256

SolParser::SolParser(char *fn)
{
  ifstream inf(fn, ios::in);

  char lineBuf[BUFSIZE];


  while(inf.getline(lineBuf, BUFSIZE))
  {
    char token[BUFSIZE], parName[BUFSIZE];
    vector <float>  pv; // current parameter values

    inf.getline(lineBuf, BUFSIZE);
	sscanf(lineBuf, "%s", token);
	if(strcmp(token, "<name>") == 0)
	{
	  // save the name 
	  sscanf(lineBuf, "%s %s", token, parName);
	  _names.push_back(parName);

	  // keep reading until the token is not <val>
	  while(inf.getline(lineBuf, BUFSIZE))
	  {
	    sscanf(lineBuf, "%s", token);
		if(strcmp(token, "<val>")==0)
		{
		  float v;
	      sscanf(lineBuf, "%s %f", token, &v);
		  pv.push_back(v);
		}; 
	  } // 
	}

	_values.push_back(pv);
	pv.clear();
  };

  inf.close();
}

SolParser::~SolParser()
{

}

// void SolParser::Print()
// {
//   int ps = _names.size(); // number of parameters
//   
//   cerr << "NUM_OF_PAR=" << ps << endl;
//   vector<string>::iterator iter_ii, iter_jj;
//   for(iter_ii=_names.begin(); iter_ii!=_names.end(); iter_ii++)
//   {
//      // cerr << _lev[li]; 
// 	 /* li++;*/
//       for(iter_jj=(*iter_ii).begin(); iter_jj!=(*iter_ii).end(); iter_jj++)
//       {
// 		 cerr << *iter_jj << "," ;
//       }
// 	  cerr << endl;
//   }; // end for(iter_ii)
// 
// }



if(toplevel&&name.size()!=0)
	{
		QString tname;
        const char * ename;
		string sname;

		tname = name + "0expression.txt";  sname= tname.toStdString();  ename=sname.c_str();
		_scene->pwdParser->readexpression("pathways/TGF0expression.txt", _scene->_quantityName[pathwayID], _scene->_geneInfo[pathwayID], _scene->_quantity[pathwayID], _scene->_minmax[pathwayID]);

		tname = name + "1pathway.txt"; sname= tname.toStdString();  ename=sname.c_str();
		stepNum=_scene->pwdParser->read1pathway(ename, _1pathway);

		tname = name + "2pathwayStepOrder.txt"; sname= tname.toStdString();  ename=sname.c_str();
        stepNum=_scene->pwdParser->read2pathwayStepOrder(ename, stepNum, _pathwayStep);

		tname = name + "3pathwayStepReactionControl.txt"; sname= tname.toStdString();  ename=sname.c_str();
		_scene->pwdParser->read3pathwayStep_reactionAndcontrol(ename, stepNum, _3pathwayStepCatalysis, _3pathwayStepReaction);    	

		tname = name + "4biochemicalReaction.txt"; sname= tname.toStdString();  ename=sname.c_str();
		vector<int> temp = _scene->pwdParser->read4biochemicalReaction(ename, _scene->_biochemicalReaction[pathwayID], _scene->_reactionName[pathwayID], _scene->_degradation[pathwayID], _scene->_degradationName[pathwayID], _scene->_EmptyName[pathwayID], smallMolecule, Dna, reactionNum, physicalEntityNum, smallMoleculeNum, DnaNum, degradationNum);
		
		//reactionNum = temp[0], physicalEntityNum=temp[1], smallMoleculeNum=temp[2], DnaNum=temp[3]; degradationNum=temp[3];

		manuLocateCompartment(compartmentPos);	

		tname = name + "5catalysisControl.txt"; sname= tname.toStdString();  ename=sname.c_str();
		_scene->pwdParser->read5catalysisANDcontrol(ename, _Catalysis, _5Control);

		tname = name + "6complex.txt"; sname= tname.toStdString();  ename=sname.c_str();
		complexNum = _scene->pwdParser->read6complex(ename, _scene->_complexName[pathwayID],_scene->_complexContain[pathwayID], _scene->CompartmentContain[m_pathwayID], _scene->_compartmentName[m_pathwayID]);
		processComplexContains(_scene->_complexContain[pathwayID]);

		tname = name + "7protein.txt"; sname= tname.toStdString();  ename=sname.c_str();
		proteinNum = _scene->pwdParser->read7protein(ename, _scene->_proteinName[pathwayID], _scene->CompartmentContain[m_pathwayID], _scene->_compartmentName[m_pathwayID]);

		tname = name + "8convertedEntity.txt"; sname= tname.toStdString();  ename=sname.c_str();
		_scene->pwdParser->read8convertedEntity(ename, proteinNum, complexNum, physicalEntityNum, _8convertedProtein, _8convertedComplex, _8convertedEntity, _scene->CompartmentContain[m_pathwayID], _scene->_compartmentName[m_pathwayID]);

		tname = name + "9smallEntity.txt"; sname= tname.toStdString();  ename=sname.c_str();
		_scene->pwdParser->read9smallEntity(ename,  _scene->_smallMoleculeName[pathwayID], _scene->_DnaName[pathwayID], _scene->CompartmentContain[m_pathwayID], _scene->_compartmentName[m_pathwayID], smallMoleculeNum, DnaNum);		
        

	multiplyDna();
	multiplySmallMolecule();

	smallMoleculeNum = _scene->_smallMoleculeName[pathwayID].size()==0?0:_scene->_smallMoleculeName[pathwayID].size()-1; 
	DnaNum = _scene->_DnaName[pathwayID].size()==0?0:_scene->_DnaName[pathwayID].size()-1;
	getNameforConverted(_8convertedProtein, _8convertedComplex, _8convertedEntity, _scene->_complexName[pathwayID], _scene->_proteinName[pathwayID], _scene->_physicalEntityName[pathwayID]);
    EmptyNum=_scene->_EmptyName[pathwayID].size();
	
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

	vector<QRectF> temp77(degradationNum+1, rect);
	degradationPos=temp77;

	vector<QRectF> temp66(physicalEntityNum+1, rect);
	physicalEntityPos=temp66;

	
	preLocateNodes(complexPos,proteinPos,smallMoleculePos);
    for(int i=1; i<_pathwayStep.size();i++)
	{
		if(stepTracked.find(i)==stepTracked.end())
	      trackstep(i, _pathwayStep, _3pathwayStepCatalysis, _3pathwayStepReaction, _scene->_biochemicalReaction[pathwayID]);
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

	vector<vector<vector<int>>> temp17(_scene->_compartmentName[m_pathwayID].size(), empty);
	compartmentNum = _scene->_compartmentName[m_pathwayID].size();
	_compartment=temp17;

	vector<vector<vector<int>>> temp20(EmptyNum+1, empty);
	_Empty=temp20;

	_whole=false;
		
	updateVisibleNodes(); 	
 	HierarchicalLayout(_whole);	
	RemoveDisabledNodes(_whole, _VertexInfo, _Vertex, Rank0, Ranks);	
	HierarchicalTreeNodeLocation(_whole, _VertexInfo, Rank0, Ranks);
	updateBubbleSize_2();
	resetOriRect();	
    }
	
