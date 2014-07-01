#include <iostream>
#include <fstream>
#include "pathwayDataParser.h"

PWDParser::PWDParser(QString name)
{

}

PWDParser::~PWDParser()
{

	
}


bool PWDParser::readexpression(const char *name, vector<QString> &quantityName, vector<vector<QString>> &geneInfo, vector<vector<float>> &quantity, vector<vector<float>> &minmax)
{//return maximum step
	char ch[100];
	FILE *fp = fopen(name,"r"); 
	if(!fp)
		return false;
	char e;		
	int pid, sid, pid2=-1, sid2=-1;
	int count=0;
	vector<float> min,max;
	
	for(int i=0; i< 2; i++)
	{
	    min.push_back(100000), max.push_back(-100000);
	}

    // read head
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
		
		 quantityName.push_back(buffer[2].c_str());
		 quantityName.push_back(buffer[3].c_str());
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


          vector<float> temp;
		  temp.push_back(atof(buffer[2].c_str()));
		  temp.push_back(atof(buffer[3].c_str()));
		  quantity.push_back(temp);

		  for(int i=0; i< 2; i++)
		  {
			  if(max[i]<temp[i]) max[i]=temp[i];
		      if(min[i]>temp[i]) min[i]=temp[i];
		  }
	  

		  vector<QString> stemp;
		  stemp.push_back(buffer[0].c_str());
		  stemp.push_back(buffer[1].c_str());
		  geneInfo.push_back(stemp);
		
	 }while(true);

	 for(int i=0; i< 2; i++)
	 {
		vector<float> temp;
        temp.push_back(min[i]); temp.push_back(max[i]);
	    minmax.push_back(temp);
	 }	 
	 fclose(fp);	 
	 return true;
}

/*
int PWDParser::read1pathway(const char *name, vector<set<int>> &_1pathway)
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
*/

int PWDParser::read1pathway(const char *name, vector<set<int>> &_1pathway)
{//return maximum step
	char ch[100];
	FILE *fp = fopen(name,"r"); 	
	if(!fp)
		return 0;

	char e;		
	int pid, sid, pid2=-1, sid2=-1;
	int count=0;
	int max=-100000;
	set<int> empty;
	_1pathway.push_back(empty);	
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
	 if(max<0)
		 max=0;
	 return max;
}


int PWDParser::read2pathwayStepOrder(const char *name, int stepNum, vector<vector<int>> &_pathwayStep)
{
	char ch[100];
	FILE *fp = fopen(name,"r"); 	
	if(!fp)
		return 0;
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
	 if( stepNum<0)
		  stepNum=0;
	 return stepNum;
}

bool PWDParser::read3pathwayStep_reactionAndcontrol(const char *name, int stepNum, vector<vector<int>> &_3pathwayStepCatalysis, vector<int> &_3pathwayStepReaction)
{//return maximum step
	char ch[100];
	char c,o,c2,o2;
	FILE *fp = fopen(name,"r");
	if(!fp)
		return false;
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
	 
	 return true;
}


vector<int> PWDParser::read4biochemicalReaction(const char *name, vector<vector<vector<int>>> &_biochemicalReaction, vector<vector<QString>> &_reactionName, vector<vector<vector<int>>> &_degradation, vector<vector<QString>> &_degradationName, vector<vector<QString>> &_EmptyName, vector<int> &smallMolecule, vector<int> &Dna, int &reactionNum, int &physicalEntityNum, int &smallMoleculeNum, int &DnaNum, int &degradationNum)
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
			  else if(o1=='r')
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
			  else if(o2=='r')
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

	smallMolecule.clear();	Dna.clear();

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
	
	set<vector<int>> lSet, rSet;
	vector<int> itemr(2,0),iteml(2,0);
	int Bid=-1;
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
		  if(buffer[0][0]!='B')
			  continue;

		  

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
		  if(Bid!=bid)
		  {
		      lSet.clear();
			  rSet.clear();		  
			  Bid=bid;
		  }
		  itemr[0]=c1; itemr[1]=rid; 
		  iteml[0]=c2; iteml[1]=lid; 

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
			   // if(lSet.find(iteml)==lSet.end())
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
			 // if(lSet.find(iteml)==lSet.end())
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
			  //if(rSet.find(itemr)==rSet.end())
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
			   //if(rSet.find(itemr)==rSet.end())
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
		   
		  lSet.insert(iteml);
		  rSet.insert(itemr);		

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
               _reactionName[i][0]='B';  //disscociation
		    else if(rcount==1)
			  _reactionName[i][0]='D';  //bind
			else  _reactionName[i][0]='T';//transition/process
		}
	 }
	 reactionNum = empty[0], physicalEntityNum=empty[1], smallMoleculeNum=empty[2], DnaNum=empty[3]; degradationNum=empty[3];
	 //for(smallMoleculeSet) //for a smallmolecule or Dna serving as a critical link to other reaction, only record once in Dna and smallMolecule
	 /*for(int i=0; i<smallMolecule.size(); i++)
	 {
          if(sSet.find(smallMolecule[i])!= sSet.end())
		  {
		  
		  }	 
	 }*/
	 return empty;
}


bool PWDParser::read5catalysisANDcontrol(const char *name, vector<vector<int>> &_Catalysis, vector<vector<int>> &_5Control)
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
		return false;
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
			  else
				  _Catalysis[cid][4]=0;
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
			  else if(c1=='S')
			  {
				  //fscanf(fp,"omplex%d", &mid);
				  _5Control[cid][0]='S';
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
			  else
				  _5Control[cid][4]=0;			  
		  }
		  //fscanf(fp,"\n");
		  if(bid2==bid&&cid2==cid&&mid2==mid&&C2==C)
		 	  break;

		  cid2=cid, mid2=mid, bid2=bid;	 C2=C;
	 }while(true);	
	 fclose(fp);	 

	 return true;
}

int PWDParser::read6complex(const char *name, vector<vector<QString>> &_complexName, vector<vector<int>> &_complexContain, vector<set<vector<int>>> &CompartmentContain, vector<vector<QString>> _compartmentName)
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

	if(max<0)
		return 0;

	QString a;
	vector<QString> empty3(4,a);
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
		  _complexName[cid][1]= buffer[2].c_str();
		  _complexName[cid][2]= buffer[3].c_str();
          
			cname=buffer[2].c_str();
			//vector<vector<vector<QString>>> _compartmentName; 
			//for(int i=0;i<_scene->_compartmentName[m_pathwayID].size();i++)
			for(int i=0;i<_compartmentName.size();i++)
			{
			  if(cid==51)
				  cid=cid;
			   if(_compartmentName[i][0]==cname.c_str())
			   {
				   vector<int> item;
				   item.push_back('C'); item.push_back(cid);
			       CompartmentContain[i].insert(item);		
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
	  if( max<0)
		 max=0;
	 return max;
}


void PWDParser::processComplexContains(vector<vector<int>> &_complexContain)
{//replace the complex with proteins contained according to _complexContain
	//there are cases that you can not find proteins contained by complex
	int size;
	for(int i=1; i<_complexContain.size(); i++)
	{
		vector<int> set=_complexContain[i];		
		for(int k=0; k<set.size();)
	    {
		             int type=set[k], id=set[k+1];
					 vector<QString> name;	 	                     
			         switch(type)
					 {
						 case 'C':  size=_complexContain[id].size(); 
							        if(size==0)
									{
									   k=k+2;
									}
									else
									{									
									   set[k]  = _complexContain[id][0];
									   set[k+1]= _complexContain[id][1];	
									   for(int l=2; l<size;l=l+2)
									   {
									       set.push_back(_complexContain[id][l]);									   
										   set.push_back(_complexContain[id][l+1]);									   
									   }
									}
							        break;													
						 case 'E': k=k+2;  break; 
						 case 'P': k=k+2;   break;						 
					 }						 
		}
		_complexContain[i]=set;
	}
		 
}

int PWDParser::read7protein(const char *name, vector<vector<QString>> &_proteinName, vector<set<vector<int>>> &CompartmentContain, vector<vector<QString>> &_compartmentName, vector<QRectF> &compartmentPos)
{
	char ch[255], ch2[255];	
	string n1,n2,n12,n22,n3;
	char e;	
	FILE *fp = fopen(name,"r"); 
	if(!fp)
		return 0;

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

	 if(max<0)
		 return 0;
	 QString a;
	vector<QString> empty(5,a);
	vector<vector<QString>> temps(max+1,empty);
	_proteinName = temps;
		
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
			bool flag=false;
			for(int i=0;i<_compartmentName.size();i++)
			{
			   if(_compartmentName[i][0]==n3.c_str())
			   {
				   vector<int> item;
				   item.push_back('P'); item.push_back(pid);
			       CompartmentContain[i].insert(item);		
				   flag=true;
				   break;
			   }				  
			}
			if(!flag && !_compartmentName.empty())
			{
				vector<int> item;
				set<vector<int>> empty;
				vector<QString> temp(1,"");
				//temp.push_back("whole");
				temp[0]=n3.c_str();
				compartmentPos.push_back(QRectF(530/2587.0, 241/1333.0, (2515-530)/2587.0, (1278-241)/1333.0));  //inside right				
				_compartmentName[_compartmentName.size()-1]=temp;
				temp[0]="whole";
				_compartmentName.push_back(temp);

				CompartmentContain.push_back(empty);	
				item.push_back('P'); item.push_back(pid);
			    CompartmentContain[CompartmentContain.size()-1].insert(item);		
			}
					
			_proteinName[pid][0]=buffer[1].c_str(); //name	
			_proteinName[pid][1]=buffer[4].c_str(); //location
			_proteinName[pid][2]=buffer[3].c_str(); //reactome id
			
			QString uniProt=buffer[2].c_str(); //uniProt id
			QStringList Slist=uniProt.split(' ');

			_proteinName[pid][3]=Slist[0]; //uniProt id
			if(Slist.size()>1)
			   _proteinName[pid][4]=Slist[1]; //symbol //109581, protein22 do not have symbol after uniprot id
			
			if(pid>max)
			    max=pid;
	 }while(true);
	 fclose(fp);		 
	 if( max<0)
		 max=0;
	 return max;
}


bool PWDParser::read8convertedEntity(const char *name, int &proteinNum, int &complexNum, int &physicalEntityNum, vector<vector<int>> &_8convertedProtein, vector<vector<int>> &_8convertedComplex, vector<vector<int>> &_8convertedEntity, vector<set<vector<int>>> &CompartmentContain, vector<vector<QString>> _compartmentName)
{
	char ch[100];
	char c,d,o,c2,d2;
	char e;
	string cname,n1;
	FILE *fp; 
	int lid, rid, lid2, rid2;
	int pmax=proteinNum, cmax=complexNum, emax=physicalEntityNum;

	fp = fopen(name,"r"); 
	if(!fp)
		return false;

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
			  else if(i==4) rid=atoi(result.c_str()); 
		  }     
		  c=buffer[0][0];
			d=buffer[4][0];
			if(c=='C')
		    {
				 
				n1=buffer[2];
				for(int i=0;i<_compartmentName.size();i++)
					{
						if(_compartmentName[i][0]==n1.c_str())
						{
							vector<int> item;
							item.push_back('C'); item.push_back(lid);
							CompartmentContain[i].insert(item);		
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
					
					n1=buffer[2];
					for(int i=0;i<_compartmentName.size();i++)
					{
						if(_compartmentName[i][0]==n1.c_str())
						{
							vector<int> item;
							item.push_back('E'); item.push_back(lid);
							CompartmentContain[i].insert(item);		
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
					
					n1=buffer[2];
					for(int i=0;i<_compartmentName.size();i++)
					{
						if(_compartmentName[i][0]==n1.c_str())
						{
							vector<int> item;
							item.push_back('P'); item.push_back(lid);
							CompartmentContain[i].insert(item);		
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
			  else if(i==4) rid=atoi(result.c_str()); 
		  }     
		    c=buffer[0][0];
			d=buffer[4][0];
		    
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

	 return true;
}


bool PWDParser::read9smallEntity(const char *name, vector<vector<QString>> & _smallMoleculeName, vector<vector<QString>> &_DnaName, vector<set<vector<int>>> &CompartmentContain, vector<vector<QString>> _compartmentName, int &smallMoleculeNum, int &DnaNum)
{
	//path 192905 has rna in smallentity files
	char ch[255], ch2[255];	
	char c;
	char e;	
	string n1,n2,n12,n22,n3;
	FILE *fp = fopen(name,"r"); 
	if(!fp)
		return false;
	
	int pid, pid2;
	char c2;
	int Dmax=0, Smax=0;

	pid2=-1; c2='x';
	n12='x',n22='x';	
	do
	{
		// if(pid==8)
			//  pid=pid;
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
	vector<QString> empty(4,a);
	vector<vector<QString>> temps(Smax+1,empty);
	vector<vector<QString>> tempd(Dmax+1,empty);
	
	_DnaName = tempd;
	 _smallMoleculeName  = temps;
	

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
			for(int i=0;i<_compartmentName.size();i++)
			{
			   if(_compartmentName[i][0]==n3.c_str())
			   {
				   vector<int> item;
				   if(c=='S' || c=='D')    ////keqin: path 192905 has rna in smallentity files
				   {
					   item.push_back(c); item.push_back(pid);
			       CompartmentContain[i].insert(item);		
				   
				   }
				   break;
			   }			
			}
		
			n1=buffer[1];
			n2=buffer[2];			
			if(c=='D')
		    {
				_DnaName[pid][0]=n1.c_str();	//name			
				int tindex0 = _DnaName[pid][0].lastIndexOf("gene");		
				//pathwayName.remove(tindex1+1,pathwayName.size());	
				_DnaName[pid][0].remove(tindex0,4);

				_DnaName[pid][1]=buffer[4].c_str();	 //location			
				_DnaName[pid][2]=buffer[3].c_str();  //reactom id
				_DnaName[pid][3]=buffer[2].c_str();  //description
		    }
		    else if(c=='S')
			{
				_smallMoleculeName[pid][0]=n1.c_str();
				_smallMoleculeName[pid][1]=buffer[4].c_str();	 //location			
				_smallMoleculeName[pid][2]=buffer[3].c_str();  //reactom id
				_smallMoleculeName[pid][3]=buffer[2].c_str();  //description
			}	


	 }while(true);
	 fclose(fp);	 	

	 return true;
}

int PWDParser::findFillsinFolder(QString path, QString pathwayName)
{
     //filesTable->setRowCount(0);
     QString fileName = "";//fileComboBox->currentText();//"_1pathway" only work on emapty "" case
     QString text = "";//textComboBox->currentText();
     //QString path = "data/Reactome_Pathway_Data/pathwayTable"; //directoryComboBox->currentText();
     //The find() slot is called whenever the user requests a new search by pressing the Find button.

     //First we eliminate any previous search results by setting the table widgets row count to zero. Then we retrieve the specified file name, text and directory path from the respective comboboxes.
	 int count=0;
     QDir currentDir = QDir(path);
     QStringList files;
     if (fileName.isEmpty())
         fileName = "*";
    files = currentDir.entryList(QStringList(fileName),
                                  QDir::Files | QDir::NoSymLinks);

    if (!text.isEmpty())
         files = findFiles(currentDir, files, text);
	
	QString pureName;
	
	 
	QString num;
	int lid;
	set<int> idrecord;
    for (int i = 0; i < files.size(); ++i) 
	{  
        //if(files[i].contains(pathwayName))
		{
			if(files[i].contains("1222352"))
			    i=i;

			lid=files[i].lastIndexOf('.');
			pureName = files[i].mid(0, lid);

			count=-1;
			for(int j=pureName.size()-1; j>=0; j--)
			{
		        QChar c=pureName.at(j);
			    if((c>'9'||c<'0')&&c!='_')
				{
					count=j;
					break;
				}	 	   
			}			
			count=count+1;
			num=pureName.mid(count+1, pureName.size()-count);
			pureName = files[i].mid(0, count);
			if(pureName==pathwayName) //remove this line if use folder
			{
		        count = num.toInt(); //convert QString to ini/ convert QString to num
				idrecord.insert(count);
			}
		}  
    }
	if(idrecord.empty())
		return -1;
	else
	{
	    count = *idrecord.rbegin();
		for (int i = 0; i < count+1; i++) 
		{
		    if(idrecord.find(i)==idrecord.end())
			{
			   return i;
			} 
		}
	}
	return count;
}



void PWDParser::findFillsinFolder()
 {
     //filesTable->setRowCount(0);
     QString fileName = "";//fileComboBox->currentText();//"_1pathway" only work on emapty "" case
     QString text = "_1pathway";//textComboBox->currentText();
     QString path = "data/Reactome_Pathway_Data/pathwayTable"; //directoryComboBox->currentText();
     //The find() slot is called whenever the user requests a new search by pressing the Find button.

     //First we eliminate any previous search results by setting the table widgets row count to zero. Then we retrieve the specified file name, text and directory path from the respective comboboxes.

     QDir currentDir = QDir(path);
     QStringList files;
     if (fileName.isEmpty())
         fileName = "*";
     files = currentDir.entryList(QStringList(fileName),
                                  QDir::Files | QDir::NoSymLinks);

     if (!text.isEmpty())
         files = findFiles(currentDir, files, text);
	 	 
    //newPathwayNameFiles(currentDir, files);     	 
 }

set<int> PWDParser::matchSymbol(set<QString> symbol, vector<vector<QString>> proteinName)
{
	set<int> output;
	for (int j = 0; j < proteinName.size(); j ++)
	{
	    if(symbol.find(proteinName[j][0])!=symbol.end())
			output.insert(j);
		if(proteinName[j].size()>4)
		if(symbol.find(proteinName[j][4])!=symbol.end())
			output.insert(j);
	}
	return output;
}


void  PWDParser::MunShellConversion(vector<vector<float>> AVCRGBs, float angle, int value, int chroma, int &R, int &G, int &B)
{
	int i=40*9*(value-1)+40*chroma+angle/2.5;
	R=AVCRGBs[i][3];
	G=AVCRGBs[i][4];
	B=AVCRGBs[i][5];
}

vector<vector<float>> PWDParser::readMunshell()
{
	QString fileName="MunshellColor.txt";
	int size=40*17*9;
	vector<float> rgb(6,0);
	vector<vector<float>> AVCRGBs(size, rgb);	
	QFile inputFile(fileName);
	int count=0;		
	int pid;
	QString pName;	
	if (inputFile.open(QIODevice::ReadOnly))
	{
		   QTextStream in(&inputFile);
		   while ( !in.atEnd() )
		   {			   
				QString line = in.readLine();    
				if(count!=0)
				{
					QStringList sname=line.split('\t');
					/*for(int i=0; i<sname.size(); i++)
					{
						QString phrase=sname[i];
						phrase=phrase;
					}*/
					//convert QString to number, convert QString to integer
					rgb[3]=sname[10].toInt(), rgb[4]=sname[11].toInt(), rgb[5]=sname[12].toInt();
					rgb[0]=sname[0].toFloat(), rgb[1]=sname[2].toInt(), rgb[2]=sname[3].toInt();
					int i=40*9*(rgb[1]-1)+40*rgb[2]/2+rgb[0]/2.5;
					//AVCRGBs.push_back(rgb);
					AVCRGBs[i]=rgb;
				}
				count++;
		   }
		  //rateLimitSymbol=rateLimitSymbol1;
	}    
	else return AVCRGBs;
	inputFile.close();
	return AVCRGBs;
}


vector<QString> PWDParser::readRateLimitSymbol()
{
    //search all files
   //read cross talk symbol file
    QFile rateLimitFile("data/Reactome_Pathway_Data/ratelimitsymbol.txt");
	vector<QString> RateLimitSymbol;
	
	if (rateLimitFile.open(QIODevice::ReadOnly))
	{
		   QTextStream in(&rateLimitFile);
		   while ( !in.atEnd() )
		   {
				QString line = in.readLine();    
				line=line.remove("\n");				  			  
				RateLimitSymbol.push_back(line);											  
		   }		   
	}
	rateLimitFile.close();
	return RateLimitSymbol;
}	


bool PWDParser::readOrthologySymbol(QString fileName, set<QString> &orthologySymbol)
{
    //search all files
   //read cross talk symbol file
    QFile inFile(fileName);
	//vector<QString> orthologySymbol;
	
	if (inFile.open(QIODevice::ReadOnly))
	{
		QTextStream in(&inFile);
		while ( !in.atEnd() )
		{
			QString line = in.readLine();    
			//line=line.remove("\n");				  			  				
		    QStringList sname=line.split('\t');	
		    orthologySymbol.insert(sname[0]);
		}		   
	}
	else return false;

	 inFile.close();
	 return true;
}	



bool PWDParser::readRateLimitSymbol(QString fileName, set<QString> &rateLimitSymbol)
{
	QFile inputFile(fileName);
	int count=0;		
	int pid;
	QString pName;
	//vector<QString> rateLimitSymbol1;
	if (inputFile.open(QIODevice::ReadOnly))
	{
		   QTextStream in(&inputFile);
		   while ( !in.atEnd() )
		   {
				QString line = in.readLine();    
				line=line.remove("\n");				  
				rateLimitSymbol.insert(line);							  
		   }
		  //rateLimitSymbol=rateLimitSymbol1;
	}    
	else return false;

	 inputFile.close();
	 return true;
}


bool PWDParser::readCrossTalkSymbol(QString fileName,set<QString> &crossTalkSymbol)
{
	QFile inputFile(fileName);
	int count=0;		
	int pid;
	QString pName;
	if (inputFile.open(QIODevice::ReadOnly))
	{
		   QTextStream in(&inputFile);
		   while ( !in.atEnd() )
		   {
				QString line = in.readLine();    
				line=line.remove("\n");				  			  
				crossTalkSymbol.insert(line);											  
		   }		   
	}
	else return false;
    
	inputFile.close();
	 return true;
}

QStringList PWDParser::findFiles(QDir currentDir, const QStringList &files, const QString &text)
 {
     //QProgressDialog progressDialog(this);
     //progressDialog.setCancelButtonText(tr("&Cancel"));
     //progressDialog.setRange(0, files.size());
     //progressDialog.setWindowTitle(tr("Find Files"));
     QStringList foundFiles;
     for (int i = 0; i < files.size(); ++i) {
        // progressDialog.setValue(i);
         //progressDialog.setLabelText(tr("Searching file number %1 of %2...").arg(i).arg(files.size()));
         //qApp->processEvents();
		 QString path = currentDir.absoluteFilePath(files[i]);//for debug
		 if(path.contains("1222352"))
			 path=path;

		 if(path.contains(text))
			 foundFiles << files[i];

         /*QFile file(currentDir.absoluteFilePath(files[i]));
		 if (file.open(QIODevice::ReadOnly)) {
             QString line;
             QTextStream in(&file);
             while (!in.atEnd()) {
                 //if (progressDialog.wasCanceled())
                 //    break;

                 line = in.readLine();
                 if (line.contains(text) || line.contains(text)) {
                     foundFiles << files[i];
                     break;
                 }
             }
         }*/
     }
     return foundFiles;
 }

void PWDParser::readNewPathwayNameFiles(vector<int> &pathIDs, vector<QString> &pathNames)
{
	QString name="data/Reactome_Pathway_Data/pathwayID.txt";//pathwayID//pathwayTablenewPathwayID
	QFile inputFile(name);
	int count=0;
		
	int pid;
	QString pName;
	if (inputFile.open(QIODevice::ReadOnly))
	{
		   QTextStream in(&inputFile);
		   while ( !in.atEnd() )
		   {
			  QString line = in.readLine();    
			  //if(count>0)
			  {
				  QStringList sname=line.split('\t');
				  for(int i=0; i<sname.size(); i++)
				  {
					  QString phrase=sname[i];
					  if(i==1)
					     pathIDs.push_back(atoi(phrase.toLocal8Bit().data()));
					  if(i==2)
				         pathNames.push_back(phrase);
				  }
				  //pathLines.push_back(line);
			  }
			 // count++;			  
		   }
	}
    inputFile.close();
}
	 

void PWDParser::newPathwayNameFiles(QDir currentDir, const QStringList &files)
{
	
	 QString newPathway_ID; //only store the files 
	 vector<int> pathIDs;
	 vector<QString> pathNames, pathLines;
	 set<QString> nameSet;
	 newPathway_ID=currentDir.absolutePath()+"PathwayID.txt";
	 QFile outFile(newPathway_ID);
	 outFile.open(QIODevice::WriteOnly | QIODevice::Text);
     QTextStream out(&outFile);

     // optional, as QFile destructor will already do it:    
     //qint64 size = QFileInfo(file).size();
	 
	for (int i = 0; i < files.size(); ++i) 
	{
		//QString name=currentDir.absoluteFilePath(files[i]);		 
		//QFile inputFile(name);
		if(files[i].contains("_1pathway"))
		{
			QString name=files[i];
			QStringList nlist=name.split('_');
			int ID= atoi(nlist[0].toLocal8Bit().data());
			if(ID == 1222352)
				ID=ID;

			for(int j=0;j<pathIDs.size();j++)
			{
					if(pathIDs[j]==ID)
					{
						nameSet.insert(pathNames[j]);
					}		
			}  
		}
    }

	//make *.path names for pathway
    for(int j=0;j<pathIDs.size();j++)
	{		  
		    if(pathIDs[j]==1222352)
			    j=j;
		    QString newPathFileName = pathNames[j]; 					
			{
				newPathFileName = newPathFileName.remove(':');			
				newPathFileName = newPathFileName.remove(',');		
				newPathFileName = newPathFileName.remove('/');
				newPathFileName = currentDir.absolutePath()+ '/' + newPathFileName + ".path";			
				QFile outFile1(newPathFileName);

				outFile1.open(QIODevice::WriteOnly | QIODevice::Text);
				QTextStream out1(&outFile1);
				QString test = QString::number(pathIDs[j]);
				//convert number to QString
				out1 << pathIDs[j] << "\n";
				outFile1.close(); 
		  	}
	}	


     for(set<QString>::iterator it=nameSet.begin(); it!=nameSet.end(); it++)
	 {
	     QString name = *it;
        for(int j=0;j<pathIDs.size();j++)
		{
		   if(name==pathNames[j])
		   {
		       //outFile.writewriteLine(pathLines[j]);
			   out << pathLines[j] << "\n";			   
		   }		
		}	 
	 }
	 outFile.close(); 
	 
	int count=0;

	int pid;
	QString pName;
	QString name="data/Reactome_Pathway_Data/pathwayID.txt";
	QFile inputFile(name);
	if (inputFile.open(QIODevice::ReadOnly))
	{
		   QTextStream in(&inputFile);
		   while ( !in.atEnd() )
		   {
			  QString line = in.readLine();    
			  if(count>0)
			  {
				  QStringList sname=line.split('\t');
				  for(int i=0; i<sname.size(); i++)
				  {
					  QString phrase=sname[i];
					  if(i==1)
					     pathIDs.push_back(atoi(phrase.toLocal8Bit().data()));
					  if(i==2)
				         pathNames.push_back(phrase);
				  }
				  pathLines.push_back(line);
			  }
			  count++;			  
		   }
	}
    inputFile.close();

	writeProteinToPathwayFile(currentDir, pathIDs, pathNames);//, crossTalkSymbol);
	

	vector<QString> crossTalkSymbol;
	set <QString> crossTalkSymbol1;

	readCrossTalkSymbol("data/Reactome_Pathway_Data/crosstalksymbol.txt", crossTalkSymbol1);

	for(set<QString>::iterator it=crossTalkSymbol1.begin(); it!=crossTalkSymbol1.end();  it++ )
    {
		crossTalkSymbol.push_back(*it);
	}
	vector<set<QString>> crossTalkFile=readCrossTalkProtein(currentDir, pathIDs, pathNames,crossTalkSymbol);
   
	//write pathways for each protein in reactome for text searching function 
	
	//return;

	getPathGraphData(pathIDs, pathNames, currentDir);
	buildTreeRingData("data/Reactome_Pathway_Data/homo+sapiens.xml", crossTalkSymbol, crossTalkFile, 4); //layer num layernum
    
	//go through all the pathwayfiles to get its subpath
    //buildTreeRingData(pathIDs, pathNames, currentDir, crossTalkSymbol, crossTalkFile);
   
    //write file for a orthology input	
}

void PWDParser::buildTreeRingData_by_E(QString eName, vector<vector<QString>> geneInfo, vector<vector<float>> quantity)
{
	//write a file which store the matched and expressed protein infor for a input expression file
	set<int> expresseGeneLinkedtoTreeRing, matchedGene_TreeRing_Expression;
	QString tname;
	const char * ename;
	//string sname;

	int idx1,idx2;
	idx1=eName.lastIndexOf("/");
	idx2=eName.lastIndexOf(".");
	QString onthName_t;
	onthName_t=eName.mid(idx1+1,idx2-idx1-1);

	QString toName="data/treeRingData/path_"+ onthName_t + "_expression_b.txt";	
	QString nfName="data/treeRingData/path_tree_b_11.out";	
	
	QFile NFile(nfName);
	//QFile OnthFile(eName);
	QFile TOFile(toName);
	//QFile MSFile(msName);

	if(!NFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{

	     //QMessageBox msgBox; msgBox.setText(text); msgBox.exec();
	}
	//OnthFile.open(QIODevice::ReadOnly | QIODevice::Text);
	TOFile.open(QIODevice::WriteOnly | QIODevice::Text);
	//MSFile.open(QIODevice::WriteOnly| QIODevice::Text);
	
	set<QString> pathSet;
	/*set<QString> nameSet;
	QTextStream in2(& OnthFile);			 
	while ( !in2.atEnd() )
	{			   
		QString line = in2.readLine();    
		QStringList sname=line.split('\t');	
		nameSet.insert(sname[0]);
		//out6 << sname[1] << " ";	
	}*/
	
	//QTextStream outMS(&MSFile);
	//outMS << "displayName" << "\t" << "dbId"; 

	QTextStream out6(&TOFile);
    QTextStream in1(& NFile);	
	int id=0;
	int count =0;
	while ( !in1.atEnd() )
	{			   
		QString line = in1.readLine();   
		QStringList sname=line.split(' ');			
		//out6 << id << " " << sname[1] << " ";			
		count++;		
		QString names=sname[1];
		QStringList nameList=names.split('.');
		{
			for(int i=0; i<nameList.size(); i++)
			{
				
				float expressionLevel=0;
				names=nameList[i];			
				if(names=="Signaling_by_NOTCH1_in_Cancer")
					names=names;

				if(pathSet.find(names)==pathSet.end())
				{
					pathSet.insert(names);	
					QString num = getPathID(names);
					if(num.size()==0)
					{
						continue;
					}	
					names = names.replace(" ","_", Qt::CaseSensitive);				
					vector<vector<QString>> _proteinName;
					vector<set<vector<int>>> CompartmentContain;
					vector<vector<QString>> _compartmentName;	
					vector<QRectF> _compartmentPos;	

					QString fname = "data/Reactome_Pathway_Data/pathwayTable/" + num + "_";			
					tname = fname + "7protein.txt"; 
					string sName= tname.toStdString();  
					const char * ename=sName.c_str();	

					PWDParser *pwdParser;
					int proteinNum = pwdParser->read7protein(ename, _proteinName, CompartmentContain, _compartmentName, _compartmentPos);
					int flag=0;
					for(int j=0; j< geneInfo.size(); j++)
					{
						for(int k=1; k<=proteinNum; k++)
						{
							if(_proteinName[k][0].contains(geneInfo[j][1])||_proteinName[k][4].contains(geneInfo[j][1]))
							{
								matchedGene_TreeRing_Expression.insert(j);		
								flag=1;									
							    break;
							}
						}							
					}	
					//struct expressedPathwayInfo infor;	
					if(flag!=0)
					{
						int pNum = proteinNum ;
						int count=0;
						for(int k=1; k<= pNum; k++)
						{
							for(int j=0; j< geneInfo.size(); j++)
							{
								if(_proteinName[k][0].contains(geneInfo[j][1])||_proteinName[k][4].contains(geneInfo[j][1]))
								{

									if(quantity[j][0]!=0)
									{
										  if(quantity[j][1]>quantity[j][0] && quantity[j][1]/quantity[j][0]>=2.0)
										  {
										  	    count++;	
												expresseGeneLinkedtoTreeRing.insert(j);	
										  }
										  else if(quantity[j][1]<quantity[j][0] && (quantity[j][0]-quantity[j][1])/quantity[j][0]>=0.5)
										  {
												count++;	
												expresseGeneLinkedtoTreeRing.insert(j);		
										  }
									 }	
									 else if(quantity[j][1]>0)
									 {
										 count++;	
										 expresseGeneLinkedtoTreeRing.insert(j);	
									 }						
								}						
							}					
						}						
						//infor.pathIDs=num;
						//infor.expressionLevel=proteinNum==0?0:float(count)/proteinNum;
						//expressedPathways.push_back(infor);
						//expressionLevel[layer][i]=infor.expressionLevel;		
						expressionLevel = proteinNum==0?0:float(count)/proteinNum;
					}
					out6 << id << " " << num << " " << names << " ";	
					out6 << flag << " " << expressionLevel << "\n";
					id++;
				}
			}
		}		
    }

	out6 << "expresseGeneLinkedtoTreeRing:";		
	for(set<int>::iterator it=expresseGeneLinkedtoTreeRing.begin(); it!=expresseGeneLinkedtoTreeRing.end(); it++)
	{
		out6 << " " << *it;
	}		
	out6 << "\n";
	out6 << "matchedGene_TreeRing_Expression:";		
	for(set<int>::iterator it= matchedGene_TreeRing_Expression.begin(); it != matchedGene_TreeRing_Expression.end(); it++)
	{	
		out6 << " " << *it;
	}	
	out6 << "\n";
	
	NFile.close(); 
	//OnthFile.close(); 	
	TOFile.close(); 
	//MSFile.close();	

}

QString PWDParser::getPathID(QString pathName)
{
	   if(pathName.contains("Signaling_by_NOTCH"))
		{
			pathName = pathName;
		}
	    pathName = pathName.replace("/","");
		pathName = pathName.replace("_"," ");
		QString pathName1;
			
		QString tname;
		const char * ename;
		string sname;
		//QString lastStoredfileName;
		//struct expressedPathwayInfo infor;

		//pathName = pathName.replace(",","");
		QString fileName = "data/Reactome_Pathway_Data/pathwayTable/"+ pathName + ".path";
		QString num;
		QFile inputFile(fileName);
		if(inputFile.open(QIODevice::ReadOnly))
		{
			QTextStream in(&inputFile);
			QString line = in.readLine();  
			line = line.split('\n')[0];
			num=line;			  
		}					
		// e. g. "428359") //Binding of RNA by Insulin-like Growth Factor-2 mRNA Binding Proteins (IGF2BPs/IMPs/VICKZs)
		if(num =="")
		{
			pathName1 = pathName;
			pathName1 = pathName1.replace(",","");
			fileName = "data/Reactome_Pathway_Data/pathwayTable/"+ pathName1 + ".path";
			QFile inputFile(fileName);
			if(inputFile.open(QIODevice::ReadOnly))
			{
				QTextStream in(&inputFile);
				QString line = in.readLine();  
				line = line.split('\n')[0];
				num=line;			  
			}	
			inputFile.close();
		}
		if(num =="")
		{
			
			//pathName1 = pathName;
			pathName1 = pathName1.replace(":","");
			fileName = "data/Reactome_Pathway_Data/pathwayTable/"+ pathName1 + ".path";
			QFile inputFile(fileName);
			if(inputFile.open(QIODevice::ReadOnly))
			{
				QTextStream in(&inputFile);
				QString line = in.readLine();  
				line = line.split('\n')[0];
				num=line;			  
			}
			inputFile.close();
		}
		if(num=="")
		{
			fileName = "data/treeRingData/MissingPathFile.txt";
			QFile inputFile(fileName);
			if(inputFile.open(QIODevice::ReadOnly))
			{
			    QTextStream in(&inputFile);
				QString line = in.readLine();  			    
			    while ( !in.atEnd() )
			    {			   
					line = in.readLine();  
					QStringList sname=line.split('\t');	
  			        QString qstr=sname[1];			
					if(qstr==pathName || pathName1==qstr)
					{
					    num=sname[0]; //keqin, a name error for 380284//Loss of proteins required for interphase microtubule organizationu00C2u00A0from the centrosome
						break;
					}
				}
			}
			inputFile.close();
			//search missing file
			//num=num;
		}
		if(num=="")
			num=num;
		inputFile.close();
		return num;
}


void PWDParser::buildTreeRingData(QString onthName)
{
    //write a file which store the onthology number for a input onthology file
	QString tname;
	const char * ename;
	string sname;

	int idx1,idx2;
	idx1=onthName.lastIndexOf("/");
	idx2=onthName.lastIndexOf(".");
	QString onthName_t;
	onthName_t=onthName.mid(idx1+1,idx2-idx1-1);

	QString toName="data/treeRingData/path_"+ onthName_t + "_b.txt";	
	QString nfName="data/treeRingData/path_tree_b_11.out";	
	
	QFile NFile(nfName);
	QFile OnthFile(onthName);
	QFile TOFile(toName);
	//QFile MSFile(msName);

	NFile.open(QIODevice::ReadOnly | QIODevice::Text);
	OnthFile.open(QIODevice::ReadOnly | QIODevice::Text);
	TOFile.open(QIODevice::WriteOnly | QIODevice::Text);
	//MSFile.open(QIODevice::WriteOnly| QIODevice::Text);
	
	set<QString> pathSet;
	set<QString> nameSet;
	QTextStream in2(& OnthFile);			 
	while ( !in2.atEnd() )
	{			   
		QString line = in2.readLine();    
		QStringList sname=line.split('\t');	
		nameSet.insert(sname[0]);
		//out6 << sname[1] << " ";	
	}
	
	//QTextStream outMS(&MSFile);
	//outMS << "displayName" << "\t" << "dbId"; 

	QTextStream out6(&TOFile);
    QTextStream in1(& NFile);	
	int id=0;
	int count =0;
	while ( !in1.atEnd() )
	{			   
		QString line = in1.readLine();    
		QStringList sname=line.split(' ');			
		//out6 << id << " " << sname[1] << " ";			
		count++;		
		QString names=sname[1];
		QStringList nameList=names.split('.');
		for(int i=0; i<nameList.size(); i++)
		{
			names=nameList[i];			
			if(pathSet.find(names)==pathSet.end())
			{
			    pathSet.insert(names);	

				/*QString pathName = names.replace("_"," ", Qt::CaseSensitive);				
				QString fileName = "data/Reactome_Pathway_Data/pathwayTable/" + pathName + ".path";
				QString num;
				QFile inputFile(fileName);
				if(inputFile.open(QIODevice::ReadOnly))
				{
					QTextStream in(&inputFile);
					QString line = in.readLine();  
					line = line.split('\n')[0];
					num=line;			  
				}
				int st = fileName.lastIndexOf("/");
				fileName = fileName.remove(0, st+1);
				fileName.chop(5);
								
				if(num.size()==0)
				{
					fileName = "data/treeRingData/MissingPathFile.txt";
					QFile inputFile(fileName);
					if(inputFile.open(QIODevice::ReadOnly))
					{
						QTextStream in(&inputFile);
						QString line = in.readLine();  			    
						while ( !in.atEnd() )
						{			   
							line = in.readLine();  
							QStringList sname=line.split('\t');	
  							QString qstr=sname[1];			
							if(qstr==pathName )
							{
								num=sname[0];
								break;
							}
						}
					}
					inputFile.close();
					//search missing file
					//num=num;
				}*/
				QString num = getPathID(names);
				if(num.size()==0)
				{
					continue;
				}				

				names = names.replace(" ","_", Qt::CaseSensitive);
				out6 << id << " " << names << " ";				
						
				QString fname = "data/Reactome_Pathway_Data/pathwayTable/" + num + "_";			
				QString tname = fname + "7protein.txt"; 
				string sName= tname.toStdString();  
				const char * ename=sName.c_str();					
				
				vector<vector<QString>> _proteinName;
				vector<set<vector<int>>> CompartmentContain;
				vector<vector<QString>> _compartmentName;	
				vector<QRectF> _compartmentPos;	

		
				int proteinNum = read7protein(ename, _proteinName, CompartmentContain, _compartmentName, _compartmentPos);
				int pNum=proteinNum;
				bool flag=false;
				int count=0;
				for(int k=1; k<= pNum; k++)
				{
					if(nameSet.find(_proteinName[k][0]) !=nameSet.end()||nameSet.find(_proteinName[k][4]) !=nameSet.end()) //maybe only need match one proteinName[k][0] or proteinName[k][1]
					{
						count++;				
					}
					else if(_proteinName[k][0]=="" || _proteinName[k][4]=="")
					{
						proteinNum=proteinNum-1;
					}
					else
						k=k;
				}							
				float rate = float(count)/proteinNum;
				out6 << rate << "\n";	
				id++;
			}
		}		
	}
	NFile.close(); 
	OnthFile.close(); 	
	TOFile.close(); 
	//MSFile.close();
}




void PWDParser::buildTreeRingData(vector<int> pathIDs, vector<QString> pathNames, QDir currentDir, vector<QString> crossTalkSymbol, vector<set<QString>> crossTalkFile)
{
    vector<set<QString>> subPath;
	vector<QString> subName;
	vector<vector<QString>> subNameConvert;
	vector<QString> twoName(2,"");
	set<QString> subNameSet;
	subPath.resize(pathIDs.size());
	subName.resize(pathIDs.size());
	for(int j=0;j<pathIDs.size();j++)
	{
		//open file
		QString ID=QString::number(pathIDs[j]);
		QString fname ="data/Reactome_Pathway_Data/pathwayTable/" + ID + "_" + "1pathway.txt";
		QFile inputFile(fname);
		QString symbol;
		
		if (inputFile.open(QIODevice::ReadOnly))
		{
			QTextStream in(&inputFile);		
			while ( !in.atEnd() )
			{
				QString line = in.readLine();    
				QStringList SList=line.split("\t");				  			  
				//QStringList uniProt=SList[2].split(" ");
				QString pathNum=SList[0];
				QString pID_str=pathNum.mid(7,pathNum.size()-7);
				int pID=pID_str.toInt();				
				symbol=SList[2];							
				if(pID==1)
				{
					if(symbol=="")
						symbol=pathNames[j];
					subName[j]=symbol;
					subNameSet.insert(symbol);
					if(subNameSet.size()!=j+1)
						j=j;
				}
				else if(symbol!="") 
					subPath[j].insert(symbol);
			}
			if(subName[j].size()==0)
			{
			   symbol=pathNames[j];
			   subName[j]=symbol;
			   subNameSet.insert(symbol);
			}
		}
		twoName[0]=pathNames[j];  twoName[1]=subName[j];		
		subNameConvert.push_back(twoName);
	}
   //do the converting here
   for(int j=0; j<subPath.size(); j++)
   {
		set<QString> subs=subPath[j];		
		set<QString> newSubs;
		for(set<QString>::iterator it=subs.begin(); it!=subs.end(); it++)
		{
			QString sName=*it;
			//if(subNameSet.find(sName)!=subNameSet.end()) //if there is subpathway not in the 1391 file, ignore it		
			if(sName=="Signal Transduction" ||sName=="Signaling Pathways")
			      j=j;
			QString str=convertToStandard(subNameConvert, sName);			
			newSubs.insert(str);
		}
		if(newSubs.size()!=subs.size())
		    j=j;   
		subPath[j]=newSubs;
	}
    for(int j=0; j<subName.size(); j++)
	{
	    if(subName[j]=="Signal Transduction" ||subName[j]=="Signaling Pathways")
		{
			j=j;
		}
		subName[j]=convertToStandard(subNameConvert, subName[j]);
	}

	//go through all the sub to get its hierachy
	set<QString> nonLeafSet;
	set<QString> nonTopSet;
	vector<vector<QString>> tree_B;
	for(int j=0; j<subPath.size(); j++)
	{
		vector<QString> tree_str;
		set<QString> subs=subPath[j];
		tree_str.push_back(subName[j]);
		bool flag=false;

		for(set<QString>::iterator it=subs.begin(); it!=subs.end(); it++)
		{
		    QString sName=*it;
			if(subNameSet.find(sName)!=subNameSet.end()) //if there is subpathway not in the 1391 file, ignore it
			{				  
				vector<QString> tree_str1=tree_str;
				tree_str1.push_back(sName);
				bool flag1=false;
				for(int i=0; i<subName.size(); i++)
				{
					 if(sName==subName[i])			  
					 {
						 set<QString> subs1=subPath[i];					 
						 for(set<QString>::iterator it1=subs1.begin(); it1!=subs1.end(); it1++)
						 {
							  QString sName1=*it1;	
							  if(subNameSet.find(sName1)!=subNameSet.end()) 	//if there is subpathway not in the 1391 file, ignore it
							  {
								  vector<QString> tree_str2=tree_str1;
								  tree_str2.push_back(sName1);							 
								  tree_B.push_back(tree_str2);
								  flag1=true;						  
							  }
						 }
						 break;
					 }
				}
				if(!flag1)
				{
					tree_B.push_back(tree_str1);
				}
				nonTopSet.insert(sName);
				flag=true;
			}
		}
		if(!flag)
		{
		    tree_B.push_back(tree_str);
		}
		else
			nonLeafSet.insert(subName[j]);
	}
	
	set<QString> leafSet;
	set<QString> topSet;
	
	for(int j=0; j<subPath.size(); j++)
	{
	     if(nonLeafSet.find(subName[j])==nonLeafSet.end())
		 {
		     leafSet.insert(subName[j]);
		 }
		 if(nonTopSet.find(subName[j])==nonTopSet.end())
		 {
		     topSet.insert(subName[j]);
		 }
	}

	//clean tree_B, only leave those containing all the part top- leaf	
	vector<vector<QString>> tree_B1;
	for(int i=0; i<tree_B.size(); i++)
	{
		QString start=tree_B[i][0], end=tree_B[i][tree_B[i].size()-1];
		if( leafSet.find(end)!=leafSet.end() && topSet.find(start)!=topSet.end())
		{
		    tree_B1.push_back(tree_B[i]);
		}	
	}
	set<vector<QString>> tree_Bset;
	for(int i=0; i<tree_B1.size(); i++)
	{
		tree_Bset.insert(tree_B1[i]);
	}
	tree_B.clear();
	for(set<vector<QString>>::iterator it=tree_Bset.begin(); it!= tree_Bset.end(); it++)
	{
		tree_B.push_back(*it);
	}
	
	//write it into a file
	QString treefile=currentDir.absolutePath()+"/path_tree_b.out";
	QFile TreeFile(treefile);
	TreeFile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out3(&TreeFile);
	for(int i=0; i<tree_B.size(); i++)
	{
		vector<QString> item=tree_B[i];
		out3 << i << " ";	
		if(item.size()<3 && item.size()>=1)
		{
		    vector<QString> item1(3,item[item.size()-1]);
		    for(int j=0; j<item.size(); j++)
				item1[j]=item[j];
			
			item=item1;
		}
		for(int j=0; j<item.size(); j++)
		{
			QString str=item[j];
			
			//str=convertToStandard(subNameConvert, item[j]);
		    str= str.replace(" ","_", Qt::CaseSensitive);
			int idx=str.lastIndexOf('.');
			if(idx==str.size()-1) // remove last '.'
			{
				str=str.mid(0,idx);
			}
			out3 << str;
			if(j!=item.size()-1)
				out3 << ".";			
		}
		out3 << "\n";
	}
	TreeFile.close();

	QString normalfile=currentDir.absolutePath()+"/path_normal_b.out";
	QFile NormalFile(normalfile);
	NormalFile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out2(&NormalFile);
	for(int i=0; i<tree_B.size(); i++)
	{
		vector<QString> item=tree_B[i];
		out2 << i << " ";	
		int j=item.size()-1;				
		
		QString str=item[j];////convertToStandard(subNameConvert, item[j]);
		str= str.replace(" ","_", Qt::CaseSensitive);
		
		int idx=str.lastIndexOf('.');
		if(idx==str.size()-1) // remove last '.'
		{
		    str=str.mid(0,idx);
		}

		out2 << str << " ";	
			//if(j!=item.size()-1)
			//	out2 << ".";	
		for(int i=0; i<18; i++)
		{
		   float rValue=rand()%1000/100.0;
		   out2 << rValue << " ";
		}
		out2 << "\n";
	}
	NormalFile.close(); 

	QString graphfile=currentDir.absolutePath()+"/path_graph_b.out";
	QFile GraphFile(graphfile);
	GraphFile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out4(&GraphFile);
	for(int k=0; k<crossTalkSymbol.size(); k++)
	{
		vector<QString> nodes;
		for(set<QString>::iterator it=crossTalkFile[k].begin(); it!=crossTalkFile[k].end(); it++)
		{
			nodes.push_back(*it);			          				  			          
		}
		for(int i=0; i<nodes.size(); i++)
		{
		   for(int j=i+1; j<nodes.size(); j++) //j=i+1 exclude self-talk
		   {
			   int di,dj;			   
			   di = convertToTreeRingNodeID(tree_B, nodes[i]); //convertToNonStandard(subNameConvert, nodes[i]));
			   dj = convertToTreeRingNodeID(tree_B, nodes[j]); //convertToNonStandard(subNameConvert, nodes[j]));	
			   if(di>=0 && dj>=0)
			       out4 << di  << " " << dj << "\n";	
		   }
		}
	}
	GraphFile.close(); 
	
	QString topPathfile=currentDir.absolutePath()+"/toplevel_pathway_names.out";	
	QFile TopFile(topPathfile);
	TopFile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out5(&TopFile);
	set<QString> topNameSet;
	int tcount=0;
	for(int i=0; i<tree_B.size(); i++)
	{
		vector<QString> item=tree_B[i];
		for(int j=0; j<1; j++)
		{
			QString ID = QString::number(tcount);
			QString str=item[j];			
			if(topNameSet.find(str)==topNameSet.end())
			{   
				topNameSet.insert(str);
				//str=convertToStandard(subNameConvert, item[j]);
				//str= str.replace(" ","_", Qt::CaseSensitive);
				int idx=str.lastIndexOf('.');
				if(idx==str.size()-1) // remove last '.'
				{
					str=str.mid(0,idx);
				}
				out5 << ID;
				out5 << " ";
				out5 << str;
				out5 << "\n";
				tcount ++ ;
			}		
		}		
	}
	TopFile.close();		
}

void PWDParser::getPathGraphData(vector<int> pathIDs, vector<QString> pathNames, QDir currentDir)
{
	int maxN=-100000, maxE=-100000;
	QString maxNID, maxEID;
    vector<set<QString>> subPath;
	vector<QString> subName;
	vector<vector<QString>> subNameConvert;
	vector<QString> twoName(2,"");
	vector<int> edgeNum1,edgeNum2,nodeNum1,nodeNum2; //1: not include small moleculars, 2: consider small moleculars
	set<vector<int>> EdgeNum1,EdgeNum2,NodeNum1,NodeNum2;
	vector<int> e1(2,0),e2(2,0),n1(2,0),n2(2,0);
	bool pflag, eflag, sflag, dflag, cflag, Aflag, Iflag;
	subPath.resize(pathIDs.size());
	subName.resize(pathIDs.size());
	for(int j=0;j<pathIDs.size();j++)
	{
		pflag=false, eflag=false, sflag=false, dflag=false, cflag=false, Aflag=false, Iflag=false;
		//open file
		set<QString> nodeNameSet1, nodeNameSet2;	
		
		set< vector <QString> > edgeSet1, edgeSet2;
	    int edgenum1,edgenum2,nodenum1,nodenum2;

		QString ID=QString::number(pathIDs[j]);
		QString fname ="data/Reactome_Pathway_Data/pathwayTable/" + ID + "_" + "4biochemicalReaction.txt";
		QFile inputFile(fname);
		//QString symbol;		
		if (inputFile.open(QIODevice::ReadOnly))
		{
			QTextStream in(&inputFile);		
			while ( !in.atEnd() )
			{
				vector <QString> edgea1,edgeb1,edgea2,edgeb2;
				QString line = in.readLine();    
				QStringList SList=line.split("\t");				  			  
				//QStringList uniProt=SList[2].split(" ");
				//QString reactNum=SList[0];
				nodeNameSet1.insert(SList[0]);
				nodeNameSet1.insert(SList[2]);
				nodeNameSet1.insert(SList[3]);

				edgea1.push_back(SList[2]); edgea1.push_back(SList[0]);
				edgeb1.push_back(SList[0]); edgeb1.push_back(SList[3]);

				edgeSet1.insert(edgea1);
				edgeSet1.insert(edgeb1);

				if(!SList[2].contains("SmallMolecule") && !SList[3].contains("SmallMolecule"))
				{
					nodeNameSet2.insert(SList[0]);
					nodeNameSet2.insert(SList[2]);
					nodeNameSet2.insert(SList[3]);

					edgea2.push_back(SList[2]); edgea2.push_back(SList[0]);
				    edgeb2.push_back(SList[0]); edgeb2.push_back(SList[3]);

					edgeSet2.insert(edgea2);
					edgeSet2.insert(edgeb2);
				}	
				if(SList[2].contains("Dna") || SList[3].contains("Dna"))
				{
					dflag=true;
				}
				if(SList[2].contains("Complex") || SList[3].contains("Complex"))
				{
					cflag=true;
				}
				if(SList[2].contains("Protein") || SList[3].contains("Protein"))
				{
					pflag=true;
				}
				if(SList[2].contains("PhysicalEntity") || SList[3].contains("PhysicalEntity"))
				{
					eflag=true;
				}
				if(SList[2].contains("SmallMolecule") || SList[3].contains("SmallMolecule"))
				{
					sflag=true;
				}
				
			}			
		}
		inputFile.close(); 

		
		QString fnameA ="data/Reactome_Pathway_Data/pathwayTable/" + ID + "_" + "5catalysisControl.txt";
		QFile inputFileA(fnameA);
		//QString symbol;		
		if (inputFileA.open(QIODevice::ReadOnly))
		{
			QTextStream inA(&inputFileA);		
			while ( !inA.atEnd() )
			{
				vector <QString> edgea1,edgeb1,edgea2,edgeb2;
				QString line = inA.readLine();    
				QStringList SList=line.split("\t");				  			  
				
				nodeNameSet1.insert(SList[1]);
				nodeNameSet1.insert(SList[2]);
				
                edgea1.push_back(SList[1]); edgea1.push_back(SList[2]);				
				edgeSet1.insert(edgea1);
				
				if(!SList[1].contains("SmallMolecule") && !SList[2].contains("SmallMolecule"))
				{
					nodeNameSet2.insert(SList[1]);
					nodeNameSet2.insert(SList[2]);					

					edgea2.push_back(SList[1]); edgea2.push_back(SList[2]);				    
                    edgeSet2.insert(edgea2);					
				}	
				if(SList[1].contains("Dna") || SList[2].contains("Dna"))
				{
					dflag=true;
				}
				if(SList[1].contains("Complex") || SList[2].contains("Complex"))
				{
					cflag=true;
				}
				if(SList[1].contains("Protein") || SList[2].contains("Protein"))
				{
					pflag=true;
				}
				if(SList[1].contains("PhysicalEntity") || SList[2].contains("PhysicalEntity"))
				{
					eflag=true;
				}
				if(SList[1].contains("SmallMolecule") || SList[2].contains("SmallMolecule"))
				{
					sflag=true;
				}
				if(SList[3].contains("ACTIVATION")) 
				{
					Aflag=true;
				}
				if(SList[3].contains("INHIBITION")) 
				{
					Iflag=true;
				}
			}			
		}
		inputFileA.close(); 
		edgenum1=edgeSet1.size();     edgenum2=edgeSet2.size();      nodenum1=nodeNameSet1.size(),  nodenum2=nodeNameSet2.size();
		edgeNum1.push_back(edgenum1); edgeNum2.push_back(edgenum2);  nodeNum1.push_back(nodenum1);  nodeNum2.push_back(nodenum2); 

		//if(eflag && sflag && cflag && pflag && dflag)// && Aflag && Iflag)
		{
			int pid=ID.toInt();
			e1[0]=edgenum1,   e2[0] = edgenum2, n1[0]=nodenum1, n2[0]=nodenum2;
			e1[1]=pid,        e2[1] = pid,      n1[1]=pid,      n2[1]=pid;
			EdgeNum1.insert(e1), EdgeNum2.insert(e2), NodeNum1.insert(n1), NodeNum2.insert(n2);
		}

		if(nodenum1>4000)
			nodenum1=nodenum1;
		
		if(maxN < nodenum1) maxN = nodenum1, maxNID=ID;
		if(maxE < edgenum1) maxE = edgenum1, maxEID=ID;

	    int maxNID=-1, maxEID=-1;  //1430728 //metabolism
	}
  	
	QString normalfile=currentDir.absolutePath()+"/node_edge1.txt";
	QFile NormalFile(normalfile);
	NormalFile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out2(&NormalFile);
	for(int i=0; i<edgeNum1.size(); i++)
	{
		QString data = QString::number(edgeNum1[i] + nodeNum1[i]);
		out2 << data << " ";					
	}
	NormalFile.close(); 

	QString normalfile1=currentDir.absolutePath()+"/node_edge2.txt";
	QFile NormalFile1(normalfile1);
	NormalFile1.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out3(&NormalFile1);
	for(int i=0; i<edgeNum1.size(); i++)
	{
		QString data = QString::number(edgeNum2[i] + nodeNum2[i]);
		out3 << data << " ";					
	}
	NormalFile1.close(); 

	QString normalfile2=currentDir.absolutePath()+"/node1.txt";
	QFile NormalFile2(normalfile2);
	NormalFile2.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out4(&NormalFile2);
	for(int i=0; i<edgeNum1.size(); i++)
	{
		QString data = QString::number(nodeNum1[i]);
		out4 << data << " ";					
	}
	NormalFile2.close(); 

	QString normalfile3=currentDir.absolutePath()+"/node2.txt";
	QFile NormalFile3(normalfile3);
	NormalFile3.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out5(&NormalFile3);
	for(int i=0; i<edgeNum1.size(); i++)
	{
		QString data = QString::number(nodeNum2[i]);
		out5 << data << " ";					
	}
	NormalFile3.close(); 

	QString normalfile4=currentDir.absolutePath()+"/edge1.txt";
	QFile NormalFile4(normalfile4);
	NormalFile4.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out6(&NormalFile4);
	for(int i=0; i<edgeNum2.size(); i++)
	{
		QString data = QString::number(edgeNum1[i]);
		out6 << data << " ";					
	}
	NormalFile4.close(); 
}

bool PWDParser::buildTreeRingData(QString filename, vector<QString> crossTalkSymbol, vector<set<QString>> crossTalkFile, int layerNum)
{//read in xml for pathway hierarchy
	
	QFile file( filename );
	if ( !file.open(QIODevice::ReadOnly) )
	{
		return false;
	}
	QTextStream in( &file );
	//int count=0;
	QString species;

	vector<QString> lines;
	if(!in.atEnd())
	{
	    QString line = in.readLine();   //first line
		int id1 = line.indexOf("\"");
		int id2 = line.lastIndexOf("\"");
		species = line.mid(id1+1, id2-id1-1);
	}
	while(!in.atEnd())
	{
		QString line = in.readLine();   //first line
		lines.push_back(line);
	}
	//recur
	int layer=0;
	int pathcount=0;
	vector<vector<QString>> tree_B;
	vector<vector<QString>> tree_ID_B;
	vector<vector<QString>> tree_B_1;
	vector<bool> extraLayer;
	vector<QString> node;
	vector<QString> nodeID;
	for(int i=0; i<lines.size(); i++)
	{
		readPathLayer(lines, tree_B, tree_ID_B, extraLayer, node,  nodeID, i, layer, pathcount, layerNum);
	}
	//get the conversion between name and pathID
	set<vector<QString>> pathIDtable;
	for(int i=0; i<tree_B.size(); i++)
	{
		
		for(int j=0; j<tree_B[i].size(); j++)
		{
			vector<QString> pair;
			pair.push_back(tree_B[i][j]);
			pair.push_back(tree_ID_B[i][j]);
			pathIDtable.insert(pair);
		}		
	}

	findMissingEmptyFills(tree_B, pathIDtable);

	set<vector<QString>> tree_B_set;	
	for(int i=0; i<tree_B.size(); i++)
	{
		vector<QString> item=tree_B[i];
		if(item.size()!=0)
		{
			if(tree_B_set.find(item)==tree_B_set.end())
			{
				tree_B_1.push_back(item);
			    tree_B_set.insert(item);
			}
		}
	}

	
	
	//tree_B=tree_B_1;
	tree_B.clear();
	for(set<vector<QString>>::iterator it=tree_B_set.begin(); it!=tree_B_set.end(); it++)// i=0; i<tree_B.size(); i++)
	{
		tree_B.push_back(*it);
	}	

	pathcount=0;
	for(int i=0; i<lines.size(); i++)
	{
		if(lines[i].contains("Pathway") && lines[i].contains("displayName") && lines[i].contains("dbId"))
			pathcount++;
	}
	//tree_B.resize(tree_B.size()-1);
	file.close();

	//output
	//QString nfName="data/treeRingData/path_normal_b.out";	
	//QString onthName="data/treeRingData/path_tree_b.out";	
	//QString toName="data/treeRingData/path_graph_b.out";

	//write it into a file
	QString treefile="data/treeRingData/path_tree_b_" + QString::number(layerNum) + ".out";
	QFile TreeFile(treefile);
	TreeFile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out3(&TreeFile);
	for(int i=0; i<tree_B.size(); i++)
	{
		vector<QString> item=tree_B[i];
		out3 << i << " ";	
		if(item.size()<layerNum && item.size()>=1)
		{
		    vector<QString> item1(layerNum,item[item.size()-1]);
		    for(int j=0; j<item.size(); j++)
				item1[j]=item[j];			
			item=item1;
		}
		for(int j=0; j<layerNum; j++)//item.size()
		{
			QString str=item[j];			
			//str=convertToStandard(subNameConvert, item[j]);
		    str= str.replace(" ","_", Qt::CaseSensitive);
			int idx=str.lastIndexOf('.');
			if(idx==str.size()-1) // remove last '.'
			{
				str=str.mid(0,idx);
			}
			out3 << str;
			if(j!=layerNum-1)///item.size()-1
				out3 << ".";			
		}
		out3 << "\n";
	}
	TreeFile.close();

	

	//write it into a file
	QString treeNLfile="data/treeRingData/path_tree_extraLayer_b_" + QString::number(layerNum) + ".out";
	QFile TreeNLFile(treeNLfile);
	TreeNLFile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out5(&TreeNLFile);
	for(int i=0; i<tree_B.size(); i++)
	{
		vector<QString> item=tree_B[i];
		out5 << i << " ";	
				
		if(extraLayer[i])
			out5 << 1;
		else 
			out5 << 0;			
		
		out5 << "\n";
	}
	TreeNLFile.close();


	QString normalfile="data/treeRingData/path_normal_b_" + QString::number(layerNum) + ".out";
	QFile NormalFile(normalfile);
	NormalFile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out2(&NormalFile);
	for(int i=0; i<tree_B.size(); i++)
	{
		vector<QString> item=tree_B[i];
		out2 << i << " ";	
		int j=item.size()-1;				
		
		QString str=item[j];////convertToStandard(subNameConvert, item[j]);
		str= str.replace(" ","_", Qt::CaseSensitive);
		
		int idx=str.lastIndexOf('.');
		if(idx==str.size()-1) // remove last '.'
		{
		    str=str.mid(0,idx);
		}
		out2 << str << " ";	
		//if(j!=item.size()-1)
		//	out2 << ".";	
		for(int i=0; i<18; i++)
		{
		   float rValue=rand()%1000/100.0;
		   out2 << rValue << " ";
		}
		out2 << "\n";
	}
	NormalFile.close(); 

	QString graphfile="data/treeRingData/path_graph_b_"  + QString::number(layerNum) + ".out";
	QFile GraphFile(graphfile);
	GraphFile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out4(&GraphFile);
	for(int k=0; k<crossTalkSymbol.size(); k++)
	{
		vector<QString> nodes;
		for(set<QString>::iterator it=crossTalkFile[k].begin(); it!=crossTalkFile[k].end(); it++)
		{
			nodes.push_back(*it);			          				  			          
		}
		for(int i=0; i<nodes.size(); i++)
		{
		   for(int j=i+1; j<nodes.size(); j++) //j=i+1 exclude self-talk
		   {
			   int di,dj;			   
			   di = convertToTreeRingNodeID(tree_B, nodes[i]); //convertToNonStandard(subNameConvert, nodes[i]));
			   dj = convertToTreeRingNodeID(tree_B, nodes[j]); //convertToNonStandard(subNameConvert, nodes[j]));	
			   if(di>=0 && dj>=0)
			       out4 << di  << " " << dj << "\n";	
		   }
		}
	}
	GraphFile.close(); 

	return true;
}

void PWDParser::findMissingEmptyFills(vector<vector<QString>> tree_B, set<vector<QString>> pathIDtable)
{// and empty file (reaction file is empty)
	QString msName="data/treeRingData/MissingPathFile.txt";
	QFile MSFile(msName);
	MSFile.open(QIODevice::WriteOnly| QIODevice::Text);
	QTextStream outMS(&MSFile);
	outMS << "displayName" << "\t" << "dbId" << "\n"; 

	QString emName="data/treeRingData/EmptyPathFile.txt";//reaction file is empty
	QFile EMFile(emName);
	EMFile.open(QIODevice::WriteOnly| QIODevice::Text);
	QTextStream outEM(&EMFile);
	outEM << "dbId" << "\t" << "displayName" << "\n"; 

	set < vector <QString> > missingList, emptyList;
	for(int i=0; i<tree_B.size(); i++)
	{
		for(int j=0; j<tree_B[i].size(); j++)
		{
			QString pathName1, pathName = tree_B[i][j];						
			//pathName = pathName.replace("_"," ");			
			QString tname;
			const char * ename;
			vector<QString> pair;
						
			//pathName = pathName.replace(",","");
			pathName=pathName.remove("/");
			QString fileName = "data/Reactome_Pathway_Data/pathwayTable/"+ pathName + ".path";
			QString num;
			QFile inputFile(fileName);
			if(inputFile.open(QIODevice::ReadOnly))
			{
				QTextStream in(&inputFile);
				QString line = in.readLine();  
				line = line.split('\n')[0];
				num=line;			  
			}	
			if(num =="")
			{
				pathName1 = pathName;
				pathName1 = pathName1.replace(",","");
				fileName = "data/Reactome_Pathway_Data/pathwayTable/"+ pathName1 + ".path";
				QFile inputFile(fileName);
				if(inputFile.open(QIODevice::ReadOnly))
				{
					QTextStream in(&inputFile);
					QString line = in.readLine();  
					line = line.split('\n')[0];
					num=line;			  
				}	
			}
			if(num =="")
			{
				//outMS << num << "\t" << pathName << "\n";	
				for(set < vector <QString> >::iterator it=pathIDtable.begin(); it!=pathIDtable.end(); it++)
				{   
					vector <QString>  item = *it;
					if(item[0] == pathName)
					{
						num = item[1];					
						break;
					}
				}
				if(num =="")
					num = num;
				pair.push_back(num);  
				pair.push_back(pathName);
				missingList.insert(pair);
			}
			else
			{
			    //find empty file
				QString fname ="data/Reactome_Pathway_Data/pathwayTable/" + num + "_" + "3pathwayStepReactionControl.txt";
				//tname = name + "3pathwayStepReactionControl.txt"; sname= tname.toStdString();  ename=sname.c_str();
		        //read3pathwayStep_reactionAndcontrol(ename, stepNum, _3pathwayStepCatalysis, _3pathwayStepReaction);
				int size=0;
				QFile myFile(fname);
				if (myFile.open(QIODevice::ReadOnly)){
					size = myFile.size();  //when file does open.
					myFile.close();
				} 
				if(size==0)
                {
					pair.push_back(num);  pair.push_back(pathName);
					//outEM << num << "\t" << pathName << "\n";					
					emptyList.insert(pair);
				}
			}
		}
	}
	for(set < vector <QString> >::iterator it=missingList.begin(); it!=missingList.end(); it++)
	{
		vector<QString> pair=*it;
		outMS << pair[0] << "\t" << pair[1] << "\n";		
	}
	for(set < vector <QString> >::iterator it=emptyList.begin(); it!=emptyList.end(); it++)
	{
	    vector<QString> pair=*it;
		outEM << pair[0] << "\t" << pair[1] << "\n";		
	}
	MSFile.close();
	EMFile.close();	
}

//recur
void PWDParser::readPathLayer(vector<QString> lines, vector<vector<QString>> &tree_B, vector<vector<QString>> &tree_ID_B, vector<bool> &extraLayer, vector<QString> &node, vector<QString> &nodeID, int &count, int layer, int &pathcount, int layerNum)
{
	if(layer>=11)
		layer=layer;
	if(layer>=10)
		layer=layer;
	int Layer=layer;
	int I=count-1;	
	for(int i=count; i<lines.size(); i++)
	{	       		
		bool flag=false;		
		QString line = lines[i];	
		QString temp=line;
		QString attributeName = temp.remove("<Pathway");
		QString pathName,Num;
		if(attributeName.size()!=line.size())
		{
		   //recur  
			int id=attributeName.indexOf("dbId=\"");
			attributeName=attributeName.mid(id+6,attributeName.size()-id-6);
			id=attributeName.indexOf("\"");
			Num=attributeName.mid(0,id);			
			nodeID.push_back(Num); 

			id=attributeName.indexOf("displayName=\"");
			attributeName=attributeName.mid(id+13,attributeName.size()-id-13);
			id=attributeName.indexOf("\"");
			pathName=attributeName.mid(0,id);
			node.push_back(pathName); 
			pathcount++;
			
			if(!line.contains("/>"))
			{
				layer=Layer+1; i++;
				readPathLayer(lines, tree_B, tree_ID_B, extraLayer, node, nodeID, i, layer, pathcount, layerNum);	
			}
			else
			{			
				if(node.size()>0)
				{
					vector<QString> tnode=node;
					vector<QString> tnodeID=nodeID;
					bool eflag=false;
					if(tree_B.size()>=1)
					{		
						
						if(tnode.size()>layerNum)
						{
							tnode.resize(layerNum);   tnodeID.resize(layerNum);							
							eflag=true;
						}
						if(tree_B[tree_B.size()-1]!=tnode)
						{							
							tree_B.push_back(tnode);	tree_ID_B.push_back(tnodeID);
							extraLayer.push_back(eflag);	
						}						
					}
					else 
					{

						if(tnode.size()>layerNum)
						{
							tnode.resize(layerNum);	tnodeID.resize(layerNum);							
							eflag=true;
						}
						extraLayer.push_back(eflag);	
						tree_B.push_back(tnode);			
						tree_ID_B.push_back(tnodeID);
					}
				}				
				count=i;
				if(layer==0)
				{
					node.clear(); nodeID.clear();
				}
				else if(node.size()>=1)
				{
					node.resize(node.size()-1);
					nodeID.resize(nodeID.size()-1);
				}
				break;	
			}
		}		
		else if(attributeName.contains("BlackBoxEvent"))
		{		    
			bool eflag=false;
			if(node.size()>0)
			{
				vector<QString> tnode=node;
				vector<QString> tnodeID=nodeID;
				if(tree_B.size()>=1)
				{				
					if(tnode.size()>layerNum)
					{
						tnode.resize(layerNum);	tnodeID.resize(layerNum);						
						eflag=true;
					}
					if(tree_B[tree_B.size()-1]!=tnode)
					{						
						tree_B.push_back(tnode);	tree_ID_B.push_back(tnodeID);
						extraLayer.push_back(eflag);	
					}				
				}
				else 
				{
					if(tnode.size()>layerNum)
					{
						tnode.resize(layerNum);	tnodeID.resize(layerNum);	
						eflag=true;
					}
					tree_B.push_back(tnode);	tree_ID_B.push_back(tnodeID);			
					extraLayer.push_back(eflag);	
				}
				
			}
			for(; i<lines.size(); i++)
		    {
				line = lines[i];  	
		        if(line.contains("</Pathway>"))
			    {
					break;
				}
				else if(line.contains("<Pathway"))
			    {
					flag=true; // a rare case 
					break;
				}
			}			
		}
		else if(!line.contains("</Pathway>"))
		{	
			bool eflag=false;
			if(node.size()>0)
			{
				vector<QString> tnode=node;
				vector<QString> tnodeID=nodeID;
				if(tree_B.size()>=1)
				{				
					if(tnode.size()>layerNum)
					{
						tnode.resize(layerNum);		tnodeID.resize(layerNum);	
						eflag=true;
					}

					if(tree_B[tree_B.size()-1]!=tnode)
					{
						tree_B.push_back(tnode);	tree_ID_B.push_back(tnodeID);
						extraLayer.push_back(eflag);	
					}					
				}
				else 
				{
					if(tnode.size()>layerNum)
					{
						tnode.resize(layerNum);	tnodeID.resize(layerNum);	
						eflag=true;
					}
		
					tree_B.push_back(tnode);	 tree_ID_B.push_back(tnodeID);
					extraLayer.push_back(eflag);	
				}
				
			}
			for(; i<lines.size(); i++)
		    {
				line = lines[i];  	
		        if(line.contains("</Pathway>"))
			    {
					break;
				}
				else if(line.contains("<Pathway"))
			    {
					flag=true;// a rare case 
					break;
				}			
			}
		}
		if(line.contains("</Pathway>"))
		{			
			count=i;
			if(layer==0)
			{
				node.clear(); nodeID.clear();
			}
			else if(node.size()>=1)
			{
				node.resize(node.size()-1);
				nodeID.resize(nodeID.size()-1);
			}
			break;	
		}
		else if(flag)
		{
			i=i-1;		
		}
	}	
}

int PWDParser::convertToTreeRingNodeID( vector<vector<QString>> tree_B, QString name)
{
	for(int i=0; i<tree_B.size(); i++)
	{
		vector<QString> item=tree_B[i];
		int j=item.size()-1;
		QString str=item[j];
		if(str==name)
			return i;
	}
	return -1;
}

QString PWDParser::convertToNonStandard(vector<vector<QString>> subNameConvert, QString name0)
{
	for(int i=0; i<subNameConvert.size(); i++)
	{
		if(name0==subNameConvert[i][0])
		    return subNameConvert[i][1];
	}
	return "";
}


QString PWDParser::convertToStandard(vector<vector<QString>> subNameConvert, QString name1)
{

	for(int i=0; i<subNameConvert.size(); i++)
	{
		if(name1==subNameConvert[i][1])
		    return subNameConvert[i][0];
	}
	return name1;
}


vector<set<QString>> PWDParser::readCrossTalkProtein(QDir currentDir, vector<int> pathIDs, vector<QString> pathNames, vector<QString> crossTalkSymbol)
{
	//make cross talk protein to pathway file
	//go through all the pathwayfiles
	vector<set<QString>> crossTalkfile;
	crossTalkfile.resize(crossTalkSymbol.size());
	for(int j=0;j<pathIDs.size();j++)
	{
		//open file
		QString ID=QString::number(pathIDs[j]);
		QString fname ="data/Reactome_Pathway_Data/pathwayTable/" + ID + "_" + "7protein.txt";
		QFile inputFile(fname);
		if (inputFile.open(QIODevice::ReadOnly))
		{
			QTextStream in(&inputFile);
			while ( !in.atEnd() )
			{
				QString line = in.readLine();    
				QStringList SList=line.split("\t");				  			  
				QStringList uniProt=SList[2].split(" ");
				QString symbol;
				if(uniProt.size()>1)
					symbol=uniProt[1];
				for(int i=0; i<crossTalkSymbol.size(); i++)
				{
					if(crossTalkSymbol[i]==symbol)
					{
						crossTalkfile[i].insert(pathNames[j]);
					}	
				}
			}
		}
	}
	//write it into a file
	QString newfile=currentDir.absolutePath()+"/crossTalkSymbol_Pathway.txt";
	QFile PathwayFile(newfile);
	PathwayFile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out1(&PathwayFile);
	for(int i=0; i<crossTalkSymbol.size(); i++)
	{
		for(set<QString>::iterator it=crossTalkfile[i].begin(); it!=crossTalkfile[i].end(); it++)
		{
			out1 << crossTalkSymbol[i] << "\t" << *it << "\n";			
		}
	}
	PathwayFile.close(); 	
	return crossTalkfile;
}

//
vector<QString> PWDParser::findPathwayFileforASymbol(QString symbol)
{
	QString fname = "data/Reactome_Pathway_Data/pathwayTable/proteinSymbol_Pathway.txt";
	QFile inputFile(fname);
	vector<QString> pathID;
	int caseID=-1;

	if(symbol.lastIndexOf("*") == symbol.size()-1 && symbol.indexOf("*") == 0)
	{
		symbol = symbol.mid(1, symbol.size());		
		symbol = symbol.mid(0, symbol.size()-1);
		caseID = 1;
	}
	else if(symbol.lastIndexOf("*") == symbol.size()-1)
	{
		symbol = symbol.mid(0, symbol.size()-1);
		caseID = 2;
	}
	else if(symbol.indexOf("*") == 0)
	{
		symbol = symbol.mid(1, symbol.size()-1);	
		caseID = 3;
	}
	else
		caseID = 0;		

    if (inputFile.open(QIODevice::ReadOnly))
	{
			QTextStream in(&inputFile);
			while ( !in.atEnd() )
			{
				QString line = in.readLine();    
				QStringList SList=line.split("\t");	
				if (compareSymbols(symbol, SList[0],caseID))
				{
					for(int i=1; i<SList.size(); i++)
					{
						pathID.push_back(SList[i]);			
					}
					if(caseID==0)
						break;
				}
				/*QStringList uniProt=SList[2].split(" ");
				QString symbol;
				if(uniProt.size()>1)
				symbol=uniProt[1];*/
			}
	}
	inputFile.close();
	return pathID;
}
bool PWDParser::compareSymbols(QString str, QString name, int matchType)
{
	bool flag=false, flag1,flag2;
	QString p0,s0;
	switch(matchType)
	{
		case 0: flag = name.contains(str,Qt::CaseInsensitive) && name.size()==str.size(); break;
		case 1: flag = name.contains(str,Qt::CaseInsensitive); break;
		case 2: //idx = name.indexOf(str, 0, Qt::CaseInsensitive);
				flag1 = name.indexOf(str, 0, Qt::CaseInsensitive) ==0; 
				p0 = name.mid(0,1), s0=str.mid(0,1);
				flag2 = QString::compare(p0, s0, Qt::CaseInsensitive)==0;
				flag = (flag1 && flag2);
				break;
		case 3: //flag =  && name[name.size()-1]==str[str.size()-1];  
				flag1 = name.lastIndexOf(str, -1, Qt::CaseInsensitive)>=0; 
				p0 = name.mid(name.size()-1,1), s0=str.mid(str.size()-1,1);
				flag2 = QString::compare(p0, s0, Qt::CaseInsensitive)==0;
				flag = (flag1 && flag2);
				break;								
	}
	return flag;

}

void PWDParser::writeProteinToPathwayFile(QDir currentDir, vector<int> pathIDs, vector<QString> pathNames)
{
	//make cross talk protein to pathway file
	//go through all the pathwayfiles
	vector<set<QString>> SymbolFile;
	vector<QString> SymbolList;
	set<QString> SymbolSet;
	//crossTalkfile.resize(SymbolList.size());
	for(int j=0;j<pathIDs.size();j++)
	{
		//open file
		QString ID=QString::number(pathIDs[j]);
		QString fname ="data/Reactome_Pathway_Data/pathwayTable/" + ID + "_" + "7protein.txt";
		QFile inputFile(fname);
		if (inputFile.open(QIODevice::ReadOnly))
		{
			QTextStream in(&inputFile);
			while ( !in.atEnd() )
			{
				QString line = in.readLine();    
				QStringList SList=line.split("\t");				  			  
				QStringList uniProt=SList[2].split(" ");
				QString symbol;
				if(uniProt.size()>1)
					symbol=uniProt[1];
				
				if(symbol!="")
				   SymbolSet.insert(symbol);
				/*for(int i=0; i<SymbolList.size(); i++)
				{	
					if(SymbolList[i]==symbol)
					{
						crossTalkfile[i].insert(pathNames[j]);
					}	
				}*/
			}
		}
	}	
	for(set<QString>::iterator it = SymbolSet.begin(); it!= SymbolSet.end(); it++)
	{
		SymbolList.push_back(*it);
	}

	SymbolFile.resize(SymbolList.size());
	for(int j=0;j<pathIDs.size();j++)
	{
		//open file
		QString ID=QString::number(pathIDs[j]);
		QString fname ="data/Reactome_Pathway_Data/pathwayTable/" + ID + "_" + "7protein.txt";
		QFile inputFile(fname);
		if (inputFile.open(QIODevice::ReadOnly))
		{
			QTextStream in(&inputFile);
			while ( !in.atEnd() )
			{
				QString line = in.readLine();    
				QStringList SList=line.split("\t");				  			  
				QStringList uniProt=SList[2].split(" ");
				QString symbol;
				if(uniProt.size()>1)
					symbol=uniProt[1];
				
				//SymbolList.push_back(symbol);
				for(int i=0; i<SymbolList.size(); i++)
				{	
					if(SymbolList[i]==symbol)
					{
						SymbolFile[i].insert(ID);
						break;
					}	
				}
			}
		}
	}
	//write it into a file
	QString newfile=currentDir.absolutePath()+"/proteinSymbol_Pathway.txt";
	QFile PathwayFile(newfile);
	PathwayFile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out1(&PathwayFile);
	for(int i=0; i<SymbolFile.size(); i++)
	{
		out1 << SymbolList[i]; 
		for(set<QString>::iterator it=SymbolFile[i].begin(); it!=SymbolFile[i].end(); it++)
		{
			out1 << "\t" << *it;			
		}
		out1 << "\n";			
	}
	PathwayFile.close(); 
	SymbolFile.clear();
	SymbolList.clear();
	SymbolSet.clear();
}

set<QString> PWDParser::crossTalkSymbolToPathway(QString symbol)
{
	//write it into a file
	QString newfile="data/Reactome_Pathway_Data/pathwayTable/crossTalkSymbol_Pathway.txt";
	QFile inputFile(newfile);
	inputFile.open(QIODevice::ReadOnly);
    QTextStream in(&inputFile);
	set<QString> fileSet;
	while ( !in.atEnd() )
	{		
		QString line = in.readLine();    
		QStringList SList=line.split("\t");		
		if(SList[0]==symbol)
		{
			if(SList[1]!="Metabolism") //keqin
			    fileSet.insert(SList[1]);
		}		
	}
	inputFile.close(); 
	return fileSet;
}


void PWDParser::showFiles(QDir currentDir, const QStringList &files)
 {
     for (int i = 0; i < files.size(); ++i) {
         QFile file(currentDir.absoluteFilePath(files[i]));
         qint64 size = QFileInfo(file).size();
		 QString name=currentDir.absoluteFilePath(files[i]);
	
		QFile inputFile(name);
		if (inputFile.open(QIODevice::ReadOnly))
		{
		   QTextStream in(&inputFile);
		   while ( !in.atEnd() )
		   {
			  QString line = in.readLine();    
			  line=line;
		   }
		}

        inputFile.close();

        /* QTableWidgetItem *fileNameItem = new QTableWidgetItem(files[i]);
         fileNameItem->setFlags(fileNameItem->flags() ^ Qt::ItemIsEditable);
         QTableWidgetItem *sizeItem = new QTableWidgetItem(tr("%1 KB")
                                              .arg(int((size + 1023) / 1024)));
         sizeItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
         sizeItem->setFlags(sizeItem->flags() ^ Qt::ItemIsEditable);

         int row = filesTable->rowCount();
         filesTable->insertRow(row);
         filesTable->setItem(row, 0, fileNameItem);
         filesTable->setItem(row, 1, sizeItem);*/
     }
     //filesFoundLabel->setText(tr("%1 file(s) found").arg(files.size()) + (" (Double click on a file to open it)"));
 }


void PWDParser::getNameforConverted(vector<vector<int>> _8convertedProtein, vector<vector<int>> _8convertedComplex, vector<vector<int>> _8convertedEntity, vector<vector<QString>> &_complexName, vector<vector<QString>> &_proteinName, vector<vector<QString>> &_physicalEntityName, int physicalEntityNum)
{   //only keep the last one
	//void &_complexName, vector<vector<QString>> &_proteinName, vector<vector<QString>> &_physicalEntityName
	//_proteinName
	QString at('/');

	for(int i=0;i<_8convertedProtein.size() && i<_proteinName.size();i++)
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
		
	for(int i=0;i<_8convertedComplex.size() && i<_complexName.size();i++)
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
	vector<vector<QString>> temps(physicalEntityNum+1,empty); //temp
	_physicalEntityName = temps;

	for(int i=0;i<_8convertedEntity.size() && i<_physicalEntityName.size();i++)
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

void PWDParser::TraceSteps( vector<vector<int>> _pathwayStep, vector<vector<int>> &edge, vector<QRectF> &reactionPos, vector<set<int>> _1pathway, vector<vector<int>> _5Control, vector<vector<int>> _Catalysis, vector<vector<int>> _3pathwayStepCatalysis, vector<int> _3pathwayStepReaction, vector<vector<vector<int>>> _biochemicalReaction, bool drawSmallmolecule)
{
	edge.clear();
	QRectF rect(-1000,-1000,0,0);
	vector<QRectF> reactionRect(reactionPos.size(),rect);
	reactionPos=reactionRect;
	set<int> stepTracked; 
    for(int i=1; i<_pathwayStep.size();i++)
	{
		if(stepTracked.find(i)==stepTracked.end())	        
			trackstep(edge, reactionPos, stepTracked, i, _1pathway, _5Control, _Catalysis, _pathwayStep, _3pathwayStepCatalysis, _3pathwayStepReaction, _biochemicalReaction, drawSmallmolecule);
	}	
}  

void PWDParser::TraceSteps_2( vector<vector<int>> _pathwayStep, vector<vector<int>> &edge, vector<QRectF> &reactionPos, vector<set<int>> _1pathway, vector<vector<int>> _5Control, vector<vector<int>> _Catalysis, vector<vector<int>> _3pathwayStepCatalysis, vector<int> _3pathwayStepReaction, vector<vector<vector<int>>> _biochemicalReaction, bool drawSmallmolecule)
{ //get edges not using this the _2pathwayStepOrder file
	edge.clear();
	QRectF rect(-1000,-1000,0,0);
	vector<QRectF> reactionRect(reactionPos.size(), rect);
	reactionPos=reactionRect;
	set<int> stepTracked; 
	QRectF rPos;
    
	for(int i=0; i<_biochemicalReaction.size() && i<500; i++)//keqin for test here
	{
		LayoutBiochemicalReaction(edge, _1pathway, _biochemicalReaction, i, -1, true, reactionPos);//drawSmallmolecule
	}

	for(int i=0; i<_3pathwayStepCatalysis.size(); i++)
	{
		LayoutCatalysis(edge, _Catalysis, rPos, i, -1, _1pathway, reactionPos);
	}

	for(int i=0; i<_3pathwayStepCatalysis.size(); i++)
	{
		LayoutControl(edge, _1pathway, _5Control, rPos, i, -1, reactionPos);
	}
	
	/*LayoutCatalysis(edge, _Catalysis, rPos, _3pathwayStepCatalysis[id][0], id, _1pathway);	
	//void LayoutCatalysis(vector<vector<int>> &edge, vector<vector<int>> _Catalysis,  QRectF rPos, int cid, int sid,  vector<set<int>> _1pathway);

	//if(_3pathwayStepCatalysis[id][1]!=-1)
	LayoutControl(edge, _1pathway, _5Control, rPos, _3pathwayStepCatalysis[id][1], id);	
	*/
}  







void PWDParser::trackstep(vector<vector<int>> &edge, vector<QRectF> &reactionPos, set<int> &stepTracked, int id, vector<set<int>> _1pathway, vector<vector<int>> _5Control, vector<vector<int>> _Catalysis, vector<vector<int>> _pathwayStep, vector<vector<int>> _3pathwayStepCatalysis, vector<int> _3pathwayStepReaction, vector<vector<vector<int>>> _biochemicalReaction, bool drawSmallmolecule)// vector<vector<vector<int>>> _degradation)
{
	
	int nid;//next step
	QRectF rPos;

	if(stepTracked.find(id)==stepTracked.end())
	    stepTracked.insert(id);
	else 
		return;
 
	if(_3pathwayStepReaction[id]!=-1)
	   rPos=LayoutBiochemicalReaction(edge, _1pathway, _biochemicalReaction, _3pathwayStepReaction[id], id, drawSmallmolecule, reactionPos);
	
	if(_3pathwayStepCatalysis[id][0]!=-1)
	   LayoutCatalysis(edge, _Catalysis, rPos, _3pathwayStepCatalysis[id][0], id, _1pathway, reactionPos);
	//void LayoutCatalysis(vector<vector<int>> &edge, vector<vector<int>> _Catalysis,  QRectF rPos, int cid, int sid,  vector<set<int>> _1pathway);

	if(_3pathwayStepCatalysis[id][1]!=-1)
	   LayoutControl(edge, _1pathway, _5Control, rPos, _3pathwayStepCatalysis[id][1], id, reactionPos);

	if(_pathwayStep[id].empty())
		return;
		
	for(int i=0; i<_pathwayStep[id].size(); i++)
	{
		nid = _pathwayStep[id][i];	
		trackstep(edge, reactionPos, stepTracked, nid, _1pathway, _5Control, _Catalysis, _pathwayStep, _3pathwayStepCatalysis, _3pathwayStepReaction, _biochemicalReaction,  drawSmallmolecule);
	}		
}



int PWDParser::whichPathway(int id, vector<set<int>> _1pathway)
{	
	for(int i=0; i<_1pathway.size(); i++)
	{
		if(_1pathway[i].find(id)!=_1pathway[i].end())
			return i;
	}
	return -1;
}

/*void PWDParser::LayoutCatalysis(vector<vector<int>> &edge, vector<vector<int>> _Catalysis, QRectF rPos, int cid, int sid, vector<set<int>> _1pathway)
{
	int type,id,type2,id2;
	float DY=0.05;
	bool flag=1;
	
	

	
	if(flag && cid<_Catalysis.size()) //some catalysis did not appear in //remember to add new type "Modulation" see pathway 2514859
	{
		vector<int> aedge(7,-1);
	     type=_Catalysis[cid][0], id=_Catalysis[cid][1];
		type2=_Catalysis[cid][2], id2=_Catalysis[cid][3];	
		if(type2=='R')
	    {
			aedge[0]=type, aedge[1]=id;
			aedge[2]=type2, aedge[3]=_Catalysis[cid][4];
			aedge[4]=type2;
			aedge[5]=id2;
			aedge[6]=whichPathway(sid, _1pathway);
			edge.push_back(aedge);		 

			if(aedge[0]<0)
			    aedge=aedge;
	    }
	    else if(type2=='D')
	    {
			//?????
			//Degradation	
	    }	    
	}
	
}
*/

QPointF PWDParser::getLocation(int pathwayid, int stepid)
{
	float DX=0.02,DY=0.04;
	return( QPointF (stepid*DX+0.01, pathwayid*DY+0.1) );	
}


void PWDParser::LayoutControl(vector<vector<int>> &edge, vector<set<int>> _1pathway, vector<vector<int>> _5Control,  QRectF rPos, int cid, int sid,  vector<QRectF> &reactionPos)
{
	int type,id,type2,id2;
	float DY=0.05;
	bool flag=1;
	
	//QSizeF size=getItemSize(type, id);	
	//QRectF point(rPos.x()  - size.width()/2 , rPos.y() - DY  - size.height()/2, size.width(), size.height());
    if(flag && cid<_5Control.size())
	{
		vector<int> aedge(7,-1); 
	    type=_5Control[cid][0], id=_5Control[cid][1];
		type2=_5Control[cid][2], id2=_5Control[cid][3];	
		if(type2=='R' && id2<500) //keqin for test
	    {
			aedge[0]=type, aedge[1]=id;
			aedge[2]=type2, aedge[3]=_5Control[cid][4];
			aedge[4]=type2;
			aedge[5]=id2;
			aedge[6]=whichPathway(sid, _1pathway);
			edge.push_back(aedge);	
			
			reactionPos[id2].setX(1);	      
	        reactionPos[id2].setWidth(0.01); 
	         reactionPos[id2].setHeight(0.01);
	    }
	    else if(type2=='D')
	    {
		
	    }	   
	}	
}


void PWDParser::LayoutCatalysis(vector<vector<int>> &edge, vector<vector<int>> _Catalysis, QRectF rPos, int cid, int sid, vector<set<int>> _1pathway,  vector<QRectF> &reactionPos)
{
	int type,id,type2,id2;
	float DY=0.05;
	bool flag=1;
	
	if(flag && cid<_Catalysis.size()) //some catalysis did not appear in //remember to add new type "Modulation" see pathway 2514859
	{
		vector<int> aedge(7,-1);
	     type=_Catalysis[cid][0], id=_Catalysis[cid][1];
		type2=_Catalysis[cid][2], id2=_Catalysis[cid][3];	
		if(type2=='R' && id2<500 )//keqin for test
	    {
			aedge[0]=type, aedge[1]=id;
			aedge[2]=type2, aedge[3]=_Catalysis[cid][4];//id2;
			aedge[4]=type2;
			aedge[5]=id2;
			aedge[6]=whichPathway(sid, _1pathway);
			edge.push_back(aedge);		 

			reactionPos[id2].setX(1);	      
	        reactionPos[id2].setWidth(0.01); 
	         reactionPos[id2].setHeight(0.01);

	    }
	    else if(type2=='D')
	    {
			//?????
			//Degradation	
	    }	    
	}	
	/*reactionPos[bid].setX(1);	      
	reactionPos[bid].setWidth(0.01); 
	reactionPos[bid].setHeight(0.01);*/	  		 
}


QRectF PWDParser::LayoutBiochemicalReaction(vector<vector<int>> &edge, vector<set<int>> _1pathway, vector<vector<vector<int>>> _biochemicalReaction, int bid, int sid, bool drawSmallmolecule, vector<QRectF> &reactionPos)
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

	  if(right.size()!=left.size())
		  right=right;
	 
	  //locate first left and first right
	  float DY=0.05;
	  float DX=0.1;
	  float x=0,y=0;
	  	
	  for(int i=0; i<left.size(); i=i+2)
	  {
		 int type=left[i], id=left[i+1];		 
		
		 
		 switch(type)
	     {
		     case 'C': 
					   {   
						 
						   count++;
						   
					   }					   
					  break;
			 case 'D': //DnaPos[id]=point;  x=x+point.x() +  size.width()/2 ; y=y+point.y() + size.height()/2; 
				      break;    				 
			 case 'E': 
					   {   
						   
						   count++;
					   }					   
					  break;	
			 case 'P': 
				       {   
						   
						   count++;
					   }					   
					   break;				      
			 case 'S':  if(drawSmallmolecule)
							count++;				      
					  break;	
		 }
	  }
	 for(int i=0; i<right.size();  i=i+2)
	  {
	     int type=right[i], id=right[i+1];	
		 switch(type)
	     {
		     case 'C': {   
						   count++;
					   }					  
					   break;			
			 case 'D': //DnaPos[id]=point;    x=x+point.x() +  size.width()/2 ; y=y+point.y() + size.height()/2;  
				       break;
			 case 'E': {   
						   
						   count++;
					   }					   
					  break;	
			 case 'P':  
					   {   
						  
						   count++;
					   }					   
					  break;	
			 case 'S':  if(drawSmallmolecule)
							count++;	
				 //	smallMoleculePos[id]=point;  x=x+point.x() +  size.width()/2 ; y=y+point.y() + size.height()/2; 
				 break;				
		 }
	  }	  
	  if(flag&&count>0)	  
	  {
		  reactionPos[bid].setX(1);	      
	      reactionPos[bid].setWidth(0.01); 
	      reactionPos[bid].setHeight(0.01);
	  		 
		  if(left.size()>right.size())
		  {
		     vector<int> temp;
			 temp=left; left=right; right=temp;
		  }
	      for(int i=0; i<left.size(); i=i+2)
	      {
			 int type=left[i], id=left[i+1];
		     aedge[0]=type, aedge[1]=id;

		     type=right[i], id=right[i+1];
		     aedge[2]=type, aedge[3]=id;
			 		
		     aedge[4]='R';
		     aedge[5]=bid;
		     aedge[6]=whichPathway(sid, _1pathway);

			 edge.push_back(aedge);		
			 if(aedge[0]<0)
			    aedge=aedge;
		  }		
	  }	  	 
	  if(bid<reactionPos.size())		  
	      return reactionPos[bid];
	  else return QRectF(0,0,0,0);
      
}

void PWDParser::writeCompartmentName(vector<vector<QString>> &_compartmentName, vector<set<vector<int>>> &CompartmentContain)	
{
	set<vector<int>> empty;
	vector<QString> temp;
	temp.push_back("");

	temp[0]="cytosol";
	_compartmentName.push_back(temp);
	CompartmentContain.push_back(empty);
	
	temp[0]="Golgi lumen";
	_compartmentName.push_back(temp);
	CompartmentContain.push_back(empty);

	temp[0]="Golgi membrane";
	_compartmentName.push_back(temp);
	CompartmentContain.push_back(empty);

	temp[0]="early endosome";
	_compartmentName.push_back(temp);
	CompartmentContain.push_back(empty);

	temp[0]="";
	_compartmentName.push_back(temp);
	CompartmentContain.push_back(empty);

	temp[0]="early endosome membrane";
	_compartmentName.push_back(temp);
	CompartmentContain.push_back(empty);

	temp[0]="mitochondrial inner membrane";
	
	_compartmentName.push_back(temp);
	CompartmentContain.push_back(empty);

	temp[0]="mitochondrial outer membrane";
	
	_compartmentName.push_back(temp);
	CompartmentContain.push_back(empty);

	temp[0]="photoreceptor disc membrane";	
	_compartmentName.push_back(temp);
	CompartmentContain.push_back(empty);

	temp[0]="lysosomal membrane";	
	_compartmentName.push_back(temp);
	CompartmentContain.push_back(empty);

	temp[0]="endoplasmic reticulum membrane";
	_compartmentName.push_back(temp);
	CompartmentContain.push_back(empty);

	temp[0]="plasma membrane";
	_compartmentName.push_back(temp);
	CompartmentContain.push_back(empty);
	
	temp[0]="endosome membrane";
	_compartmentName.push_back(temp);
	CompartmentContain.push_back(empty);

	temp[0]="Golgi-associated vesicle membrane";
	_compartmentName.push_back(temp);
	CompartmentContain.push_back(empty);
	
	temp[0]="cytoplasmic vesicle membrane";
	_compartmentName.push_back(temp);
	CompartmentContain.push_back(empty);

	temp[0]="nuclear envelope";
	_compartmentName.push_back(temp);
	CompartmentContain.push_back(empty);

	temp[0]="chromosome, centromeric region";
	_compartmentName.push_back(temp);
	CompartmentContain.push_back(empty);

	temp[0]="chromosome";
	_compartmentName.push_back(temp);
	CompartmentContain.push_back(empty);

	temp[0]="endosome";
	_compartmentName.push_back(temp);
	CompartmentContain.push_back(empty);

	temp[0]="nucleoplasm";
	_compartmentName.push_back(temp);
	CompartmentContain.push_back(empty);

	
	temp[0]="extracellular region";
	_compartmentName.push_back(temp);
	CompartmentContain.push_back(empty);

	temp[0]="cell junction";
	_compartmentName.push_back(temp);
	CompartmentContain.push_back(empty);

	temp[0]="peroxisomal matrix";
	_compartmentName.push_back(temp);
	CompartmentContain.push_back(empty);

	temp[0]="mitochondrial matrix";
	_compartmentName.push_back(temp);
	CompartmentContain.push_back(empty);

	temp[0]="endosome lumen";
	_compartmentName.push_back(temp);
	CompartmentContain.push_back(empty);

	temp[0]="endocytic vesicle membrane";
	_compartmentName.push_back(temp);
	CompartmentContain.push_back(empty);

	temp[0]="phagocytic vesicle membrane";
	_compartmentName.push_back(temp);
	CompartmentContain.push_back(empty);

	temp[0]="phagocytic vesicle lumen";
	_compartmentName.push_back(temp);
	CompartmentContain.push_back(empty);

	temp[0]="lysosomal lumen";
	_compartmentName.push_back(temp);
	CompartmentContain.push_back(empty);
		
    temp[0]="photoreceptor outer segment membrane";
	_compartmentName.push_back(temp);
	CompartmentContain.push_back(empty);

	temp[0]="photoreceptor inner segment membrane";
	_compartmentName.push_back(temp);
	CompartmentContain.push_back(empty);

	temp[0]="mitochondrial intermembrane space";
	_compartmentName.push_back(temp);
	CompartmentContain.push_back(empty);

	temp[0]="secretory granule membrane";
	_compartmentName.push_back(temp);
	CompartmentContain.push_back(empty);

	temp[0]="secretory granule lumen";
	_compartmentName.push_back(temp);
	CompartmentContain.push_back(empty);

	temp[0]="endoplasmic reticulum lumen";
	_compartmentName.push_back(temp);
	CompartmentContain.push_back(empty);

	temp[0]="endoplasmic reticulum quality control compartment";
	_compartmentName.push_back(temp);
	CompartmentContain.push_back(empty);

	temp[0]="ER to Golgi transport vesicle membrane";
	_compartmentName.push_back(temp);
	CompartmentContain.push_back(empty);

	temp[0]="clathrin-coated endocytic vesicle";
	_compartmentName.push_back(temp);
	CompartmentContain.push_back(empty);

	temp[0]="platelet dense tubular network membrane";
	_compartmentName.push_back(temp);
	CompartmentContain.push_back(empty);

	temp[0]="whole";
	_compartmentName.push_back(temp);
	CompartmentContain.push_back(empty);
}



int PWDParser::findNodeID(vector<vector<int>> _Node, int type, int id)
{
	for(int i=0; i<_Node.size(); i++)
	{
	  if(_Node[i][0]==type && _Node[i][1]==id)
		  return i;
	}
	return -1;
}




QString PWDParser::loadPathGraphPreparation(QString pathName, bool drawSmallmolecule)
{//return suggested names	
	QDir dir;
	int count=0;
	QString pathDir="data//Reactome_Pathway_Data/pathwayTable";//+pathName;
	if(!QDir(pathDir).exists())
	{
		if(QDir().mkdir(pathDir))
		{
		    
		}	
		else 
			return pathDir+'/';
	}	  
	
	if(drawSmallmolecule)
		pathName = pathName + "_S";
	
	count = findFillsinFolder(pathDir, pathName);
	if(count<0)
		pathName = pathDir;
	else 
	{
		pathName = pathDir + "/" + pathName + "_" + QString::number(count);
	}
	return pathName;	
}

QString PWDParser::savePathGraphPreparation(QString pathName, bool drawSmallmolecule)
{//return suggested names	
	QDir dir;
	int count=0;
	QString pathDir="data//Reactome_Pathway_Data/pathwayTable";//+pathName;
	if(!QDir(pathDir).exists())
	{
		if(QDir().mkdir(pathDir))
		{
		    
		}	
		else 
			return pathDir + '/';
	}	  
	if(drawSmallmolecule)
		pathName = pathName + "_S";
	count = findFillsinFolder(pathDir, pathName) + 1;

	
	pathName = pathDir + "/" + pathName + "_" + QString::number(count);
	return pathName;	
}

QString PWDParser::getPathGraphHie(QString pathName)
{//return suggested names	
	QDir dir;
	int count=0;
	QString pathDir="data//Reactome_Pathway_Data/pathwayTable";//+pathName;
	if(!QDir(pathDir).exists())
	{
		if(QDir().mkdir(pathDir))
		{
		    
		}	
		else 
			return pathDir + '/';
	}	  
	//count = findFillsinFolder(pathDir, pathName) + 1;
	pathName = pathDir + "/" + pathName;
	return pathName;	
}

//this->_curFileName
QString PWDParser::getPathExpressionFile(QString expression)
{//return suggested names	
	QDir dir;
	int count=0;
	QString pathDir="data//Reactome_Pathway_Data/pathwayTable";//+pathName;
	if(!QDir(pathDir).exists())
	{
		if(QDir().mkdir(pathDir))
		{
		    
		}	
		else 
			return pathDir + '/';
	}	  
	//count = findFillsinFolder(pathDir, pathName) + 1;
	QString fileName = pathDir + "/path_" + expression;
	return fileName;	
}


QString PWDParser::savePathNotePreparation(QString pathName, QString userName)
{//return suggested names	
	QDir dir;
	int count=0;
	QString pathDir="data//Reactome_Pathway_Data/pathwayTable";//+pathName;
	if(!QDir(pathDir).exists())
	{
		if(QDir().mkdir(pathDir))
		{
		    
		}	
		else 
			return pathDir + '/';
	}	  
	//count = findFillsinFolder(pathDir, pathName) + 1;
	pathName = pathDir + "/" + pathName;// + '_' + userName;
	return pathName;	
}


vector<QString> PWDParser::getNodeInfor(char type, int id)
{
	vector<vector<int>> _Node;
	vector<int> temp(2,0);
	vector<QString> nodeInfor;
	switch(type)
	{
		case 'C': nodeInfor=_complexName[id]; break;
		case 'E': nodeInfor=_physicalEntityName[id]; break;
		case 'P': nodeInfor=_proteinName[id];  break;
		case 'S': nodeInfor=_smallMoleculeName[id]; break;		
		case 'D': nodeInfor=_DnaName[id]; break;	 		
		case 'R': nodeInfor=_reactionName[id]; break;	 
	}	
	return nodeInfor;
}

vector<QString> PWDParser::getNodeInfor(int id)
{	
//(name, description/id)
	if(id<_Node.size())
	{
		int type=_Node[id][0], nid=_Node[id][1];	
		return(getNodeInfor(type,nid));		
	}
	vector<QString> empty;
	return empty;
}

QString PWDParser::getCompartmentName(int id)
{
	if(id<_compartmentName.size())
	{
		return(_compartmentName[id][0]);
	}
	return "";
}

set<int> PWDParser::getComplexContent(int id)
{	//get entities (complex, protein, physical entity, etc) that a complex contain
	set<int> content;
	int nid=getNodeID(id);
	if(nid<_complexContain.size())
	{
		for(int i=0; i<_complexContain[nid].size(); i=i+2)
		{
			content.insert(findNodeID(_Node, _complexContain[nid][i], _complexContain[nid][i+1]));
		}
	}
	return content;
}

set<int> PWDParser::getCompartmentContent(int id)
{	//get entities (complex, protein, physical entity, etc) that a complartment contain
	set<int> content;
	if(id<CompartmentContain.size())
	{
		for(set<vector<int>>::iterator it=CompartmentContain[id].begin(); it!=CompartmentContain[id].end(); it++)
		{
			vector<int> node=*it;			
			content.insert(findNodeID(_Node, node[0], node[1]));
		}
	}
	return content;

}
	
int PWDParser::getNodeNum()
{
	return _Node.size();	
}

int PWDParser::getEdgeNum()
{
	return _Edge.size();	
}

int PWDParser::getCompartmentNum()
{
	return _compartmentName.size();	
}

char PWDParser::getNodeType(int id)
{
	if(id<_Node.size())
	   return(_Node[id][0]);

	else return ' ';
}

int PWDParser::getNodeID(int id)
{ //get a node's id in the node of its type	
	if(id<_Node.size())
	   return(_Node[id][1]);
	else return -1;
}
