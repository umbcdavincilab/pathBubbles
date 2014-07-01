#include <vector>
#include <QString>
#include <QRectF>
#include <QDir>
#include <QProgressDialog>
#include <QTextStream>
#include <QTextDocument>
#include <QtXml>
//#include <QtObject>

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <set>
#include <iostream>
#ifndef __PARSER
#define __PARSER
using namespace std;

class PWDParser
{
  public:	
	set<int> matchSymbol(set<QString> symbol, vector<vector<QString>> proteinName);
	bool readRateLimitSymbol(QString fileName, set<QString> &rateLimitSymbol);
    bool readCrossTalkSymbol(QString fileName, set<QString> &crossTalkSymbol);
	bool readOrthologySymbol(QString fileName, set<QString> &orthologySymbol); //"data/gallus_gallus_ortholog.txt"
	void readPathLayer( vector<QString> lines, vector<vector<QString>> &tree_B, vector<vector<QString>> &tree_ID_B, vector<bool> &extraLayer, vector<QString> &node, vector<QString> &nodeID, int &count, int layer, int &pathcount, int layerNum);
	
	//bool PWDParser::readOrthologySymbol(QString fileName, vector<QString> &orthologySymbol)

	vector<QString> readRateLimitSymbol();

	QString getPathID(QString pathName);
	void buildTreeRingData_by_E(QString eName, vector<vector<QString>> geneInfo, vector<vector<float>> quantity);
	void buildTreeRingData(QString filename);
	bool buildTreeRingData(QString filename, vector<QString> crossTalkSymbol, vector<set<QString>> crossTalkFile, int layerNum);
	void findMissingEmptyFills(vector<vector<QString>> tree_B, set<vector<QString>> pathIDtable);
	void getPathGraphData(vector<int> pathIDs, vector<QString> pathNames, QDir currentDir);
	void findFillsinFolder();
	QStringList findFiles(QDir currentDir, const QStringList &files, const QString &text);
	void readNewPathwayNameFiles(vector<int> &pathIDs, vector<QString> &pathNames);   
	void buildTreeRingData(vector<int> pathIDs, vector<QString> pathNames, QDir currentDir, vector<QString> crossTalkSymbol, vector<set<QString>> crossTalkFile);
	vector<set<QString>> readCrossTalkProtein(QDir currentDir, vector<int> pathIDs, vector<QString> pathNames, vector<QString> crossTalkSymbol);
	void buildTreeRingData( QDir currentDir);
    
	
	//vector<QString> readOrthologySymbol();
	QString convertToStandard(vector<vector<QString>> subNameConvert, QString name);
	QString convertToNonStandard(vector<vector<QString>> subNameConvert, QString name0);
    int convertToTreeRingNodeID( vector<vector<QString>> tree_B, QString name);
	bool compareSymbols(QString str, QString name, int mark);

	void showFiles(QDir currentDir, const QStringList &files);
	set<QString> crossTalkSymbolToPathway(QString symbol);
	void writeProteinToPathwayFile(QDir currentDir, vector<int> pathIDs, vector<QString> pathNames);//write pathways for all the protein
	vector<QString> findPathwayFileforASymbol(QString symbol); //return all the pathway containing the protein symbol

	QString savePathGraphPreparation(QString _pathName, bool drawSmallmolecue=false);
	QString loadPathGraphPreparation(QString _pathName, bool drawSmallmolecue=false);
	QString getPathGraphHie(QString pathName);
	QString getPathExpressionFile(QString expression);
	int findFillsinFolder(QString path, QString pathwayName);		
    QString savePathNotePreparation(QString pathName, QString userName);
	

	
    vector<vector<float>> readMunshell();
	void MunShellConversion(vector<vector<float>> AVCRGBs, float angle, int value, int chroma, int &R, int &G, int &B); //input angle, value, chroma, output 
	PWDParser(QString name);
	virtual ~PWDParser();

	vector<vector<int>> _Edge; //(start,end),(start,end),...
	vector<vector<int>> _Node; //(type,id),(type,id),...

	vector<QString> getNodeInfor(char type, int id); //non-reaction node (name, compartment, reactionID, description/uniProt id) //reaction node (type, description/uniProt))
	vector<QString> getNodeInfor(int id); //(name, description/id)
	
	QString getCompartmentName(int id); 
	
	set<int> getComplexContent(int id); //get entities (complex, protein, physical entity, etc) that a complex contain
	set<int> getCompartmentContent(int id); //get entities (complex, protein, physical entity, etc) that a complartment contain
	void newPathwayNameFiles(QDir currentDir, const QStringList &files);
	
	int getCompartmentNum();
	int getNodeNum();	
	int getEdgeNum();
	int getNodeID(int id);
    char getNodeType(int id);
	
	//type 
	//'C': complex
	//'E': physicalEntity
	//'P': protein
    //'S': smallMolecule
	//'D': nodePos
	//'R': _reaction

	//vector<QString> getNodeInfor(int id);
	//vector < string > GetNames() {return _names;}
	//vector < vector <float > > GetValues() {return _values; }
	//vector < float> GetValues(int index) {return _values[index]; }
    bool readexpression(const char *name, vector<QString> &quantityName, vector<vector<QString>> &geneInfo, vector<vector<float>> &quantity, vector<vector<float>> &minmax);
	int read1pathway(const char *name, vector<set<int>> &_1pathway);
	int read2pathwayStepOrder(const char *name, int stepNum, vector<vector<int>> &_pathwayStep);
	bool read3pathwayStep_reactionAndcontrol(const char *name, int stepNum, vector<vector<int>> &_3pathwayStepCatalysis, vector<int> &_3pathwayStepReaction);
	vector<int> read4biochemicalReaction(const char *name, vector<vector<vector<int>>> &_biochemicalReaction, vector<vector<QString>> &_reactionName, vector<vector<vector<int>>> &_degradation, vector<vector<QString>> &_degradationName, vector<vector<QString>> &_EmptyName, vector<int> &smallMolecule, vector<int> &Dna, int &reactionNum, int &physicalEntityNum, int &smallMoleculeNum, int &DnaNum, int &degradationNum);
	bool read5catalysisANDcontrol(const char *name, vector<vector<int>> &_Catalysis, vector<vector<int>> &_5Control);
	int read6complex(const char *name, vector<vector<QString>> &_complexName, vector<vector<int>> &_complexContain, vector<set<vector<int>>> &CompartmentContain, vector<vector<QString>> _compartmentName);
	
	int read7protein(const char *name, vector<vector<QString>> &_proteinName, vector<set<vector<int>>> &CompartmentContain, vector<vector<QString>> &_compartmentName, vector<QRectF> &compartmentPos);
	bool read8convertedEntity(const char *name, int &proteinNum, int &complexNum, int &physicalEntityNum, vector<vector<int>> &_8convertedProtein, vector<vector<int>> &_8convertedComplex, vector<vector<int>> &_8convertedEntity, vector<set<vector<int>>> &CompartmentContain, vector<vector<QString>> _compartmentName);
	bool read9smallEntity(const char *name, vector<vector<QString>> & _smallMoleculeName, vector<vector<QString>> &_DnaName, vector<set<vector<int>>> &CompartmentContain, vector<vector<QString>> _compartmentName, int &smallMoleculeNum, int &DnaNum);
	void getNameforConverted(vector<vector<int>> _8convertedProtein, vector<vector<int>> _8convertedComplex, vector<vector<int>> _8convertedEntity, vector<vector<QString>> &_complexName, vector<vector<QString>> &_proteinName, vector<vector<QString>> &_physicalEntityName, int physicalEntityNum);
	void TraceSteps( vector<vector<int>> _pathwayStep, vector<vector<int>> &edge, vector<QRectF> &reactionPos, vector<set<int>> _1pathway, vector<vector<int>> _5Control, vector<vector<int>> _Catalysis, vector<vector<int>> _3pathwayStepCatalysis, vector<int> _3pathwayStepReaction, vector<vector<vector<int>>> _biochemicalReaction, bool drawSmallmolecule);
	void TraceSteps_2( vector<vector<int>> _pathwayStep, vector<vector<int>> &edge, vector<QRectF> &reactionPos, vector<set<int>> _1pathway, vector<vector<int>> _5Control, vector<vector<int>> _Catalysis, vector<vector<int>> _3pathwayStepCatalysis, vector<int> _3pathwayStepReaction, vector<vector<vector<int>>> _biochemicalReaction, bool drawSmallmolecule);
	void processComplexContains(vector<vector<int>> &_complexContain);
	//vector<vector<vector<int>>> recordEdge(vector<vector<vector<int>>> _biochemicalReaction);
	//output edge to ...	
	
  protected:
	//float setVariances(int startIndex, int endIndex);
  private:
	//for track the reactions involved in steps
	vector<vector<QString>> _complexName,_physicalEntityName,_proteinName, _reactionName,_degradationName,_smallMoleculeName, _EmptyName, _DnaName; //vector item start from 1	
	vector<set<vector<int>>> CompartmentContain; //start from 0 set((type,id),(type,id),..
	vector<vector<int>> _complexContain;	
	vector<vector<QString>> _compartmentName; //start from 0

	QPointF getLocation(int pathwayid, int stepid);
	QRectF LayoutBiochemicalReaction(vector<vector<int>> &edge, vector<set<int>> _1pathway, vector<vector<vector<int>>> _biochemicalReaction, int bid, int sid, bool drawSmallmolecule, vector<QRectF> &reactionPos);
	void LayoutCatalysis(vector<vector<int>> &edge, vector<vector<int>> _Catalysis,  QRectF rPos, int cid, int sid,  vector<set<int>> _1pathway, vector<QRectF> &reactionPos);
	void LayoutControl(vector<vector<int>> &edge, vector<set<int>> _1pathway, vector<vector<int>> _Catalysis,  QRectF rPos, int cid, int sid, vector<QRectF> &reactionPos);
	int whichPathway(int id, vector<set<int>> _1pathway);
	void trackstep(vector<vector<int>> &edge, vector<QRectF> &reactionPos, set<int> &stepTracked, int id, vector<set<int>> _1pathway, vector<vector<int>> _5Control, vector<vector<int>> _Catalysis, vector<vector<int>> _pathwayStep, vector<vector<int>> _3pathwayStepCatalysis, vector<int> _3pathwayStepReaction, vector<vector<vector<int>>> _biochemicalReaction, bool drawSmallmolecule);
	//void trackstep_2(vector<vector<int>> &edge, vector<QRectF> &reactionPos, set<int> &stepTracked, int id, vector<set<int>> _1pathway, vector<vector<int>> _5Control, vector<vector<int>> _Catalysis, vector<vector<int>> _pathwayStep, vector<vector<int>> _3pathwayStepCatalysis, vector<int> _3pathwayStepReaction, vector<vector<vector<int>>> _biochemicalReaction, bool drawSmallmolecule);
	void writeCompartmentName(vector<vector<QString>> &_compartmentName, vector<set<vector<int>>> &CompartmentContain);	
	
	int findNodeID(vector<vector<int>> _Node, int type, int id);
	
};

#endif //_PARSER

