#ifndef OPENMANAGER_H
#define OPENMANAGER_H

#include <QtGlobal>
#include <fstream>
#include <QList>
#include <set>
#include <QObject>
//#include "matBlock.h"
#include <QDomElement>
#include <QTextEdit>
#include "LabCut.h"
#include "LabObject.h"
//#include "AllBats.h"
#include "Discreptor.h"
#include "SpaceManager.h"


//class QtBat;
class ItemBase;
class OpenGLScene;
class GroupManager;
class treeRingBubble;
//class Camber;
class myVideoPlayer;

struct CurveMatch
{
	QString name;
	int index;
};

class OpenManager : public QObject
{
	Q_OBJECT
public:
	OpenManager( OpenGLScene* scene, LabObject* lab, LabCut* cut, GroupManager* manager);
	bool openBatByPath( ItemBase* &bat, int size, int x, int y, QString name, int index = 0 );
	ItemBase* openNote_2(int pid, int type, int id, ItemBase * item, int size, int x, int y );

	QString checkBatByPath( QString name );
	bool openBatByPathWithoutRegister( ItemBase *&result, int size, int x, int y, QString name, int index = 0 );
	bool openExpressionByPath( ItemBase *&result, int size, int x, int y, QString name);
	bool openPCByPath( ItemBase *&result, int size, int x, int y, QString name );
	bool openPathwayByPath( ItemBase *&result, QString orthName, int size, int x, int y, QString name, QString pathName= tr(""), QString lastStoredGraph= tr("") );
	QString checkPCByPath( QString name );
	QString checkPathwayByPath(QString name);
	
	ItemBase* openWebView( ItemBase * item, int size, int x, int y, QString name = tr("") );
	ItemBase* openSearchView(ItemBase * item, int size, int x, int y, QString name = tr("") );
	ItemBase* openSettingsView( ItemBase * item, int size, int x, int y, QString name = tr("") );
	ItemBase* openSearchSettingsView( ItemBase * item, int size, int x, int y, QString name = tr("") );

	bool openVideoByPath( ItemBase* &result, int size, int x, int y, QString name );
	QString checkVideoByPath( QString name );

	ItemBase* openNote( ItemBase * item, int size, int x, int y );
	//ItemBase* openPen( int size, int x, int y );
	//ItemBase* openMatlab( int size, int x, int y );
	//ItemBase* openMatlabCopy( int size, int x, int y, MatBlock* matCur );
	//ItemBase* openMatlabByPath( int size, int x, int y, QString name );
	
	
	ItemBase* openParallelCoordinate( int size, int x, int y, QString name = tr("") );
	ItemBase* openBarChart( int size, int x, int y, QString name = tr("") );
	ItemBase* openLineChart( int size, int x, int y, QString name = tr("") );
	ItemBase* openScatterPlot( int size, int x, int y, QString name = tr("") );
	ItemBase* openPathBubble0( int size, int x, int y, QString name = tr("") );	
    ItemBase* openExpressionBubble(int size, int x, int y, QString name  = tr(""));	ItemBase* openTreeRingBubble(int sizex, int x, int y);
	
    ItemBase* openSubTreeRing( treeRingBubble *parent, int size, int x, int y, set< vector <int> > ItemSelected);
	ItemBase* openPathBubble1( int size, int x, int y, QString name = tr(""));
	//ItemBase* openPathBubble2( int size, int x, int y, QString name = tr(""));
	ItemBase* openTextBubble2(int parentPathID, int parentCodeID, int size, int x, int y, QString name = tr(""));	
	ItemBase* openReactionBubble3(int m_pathwayID, int size, int x, int y, vector<int> itemSelected, QString name = tr(""));
	ItemBase* openGroupBubble4(int m_pathwayID, int size, int x, int y, vector<int> itemSelected, vector<vector<int>> edgeSelected, QString name = tr(""));
	ItemBase* openSubPathBubble1(QString orthName, int pathwayid, int sizex, int sizey, int x, int y, vector<int> ItemSelected, vector<vector<int>> EdgeSelected,  vector<set<vector<int>>> AnodeContains,  QString name, QString pathName = tr(""), QString lastStoredGraph= tr("") );
	void recoverItemFromXmlNode( QDomNodeList paraNode, set<vector<int>> &item);

	//ItemBase* openVTK( int size, int x, int y );
	ItemBase* openImage( int size, int x, int y, QString path, float rate );
	//QList<ItemBase*> openBatByToken( QString name );
	//bool findBatByToken( QString name, DESCRIPTOR dis, int number, int index, QList<ItemBase*> &bats, QList<float> &rates );
	//
	//bool getCAMBER();
	//Camber* getCurrentCamber();
	int queryPossibleOpen( QString name );
	QVector<QRectF> queryPreOpen( QPointF pos, int num );
	QVector<QRectF> queryPreOpen( QPointF pos, int row, int col );
	void openDataInViewAt( QString name, QList<int> view, QPointF pos);
	void changeView( QPointF pos, int view);
	bool changeFile( QPointF pos, QString name);
	void setNewFlag( bool flag )
	{
		_NEW = flag;
	}
	QList<QString> matchFileName( QString name );

	//ItemBase* openBatFromXmlNode( QDomElement node );
	ItemBase* openPCFromXmlNode( QDomElement node );
	ItemBase* openVideoFromXmlNode( QDomElement node );
	ItemBase* openWebFromXmlNode( QDomElement node );
	ItemBase* openBarChartFromXmlNode( QDomElement node );
	ItemBase* openLineChartFromXmlNode( QDomElement node );
	ItemBase* openScatterFromXmlNode( QDomElement node );
	//ItemBase* openMatlabFromXmlNode( QDomElement node );
	ItemBase* openNoteFromXmlNode( QDomElement node );

	ItemBase* openTextBubble2FromXmlNode( QDomElement node );
	ItemBase* openReactionBubble3FromXmlNode( QDomElement node );
	ItemBase* openGroupBubble4FromXmlNode( QDomElement node );	
	ItemBase* openPathBubble0FromXmlNode( QDomElement node);
	ItemBase* openPathBubble1FromXmlNode( QDomElement node);
	ItemBase* openSubPathBubble1FromXmlNode( QDomElement node, int pathwayID, int toplevel);
	ItemBase* openTreeRingFromXmlNode( QDomElement node, int treeRingID);

	ItemBase* openExpressionBubbleFromXmlNode( QDomElement node, int pathwayID);
	void recoverEdgeFromXmlNode( QDomNodeList paraNode, vector<vector<int>> &edge);
	void recoverItemFromXmlNode( QDomNodeList paraNode, vector<int> &item);

	QList<QString> getPathwayPathByToken( QString token, int type );	
	QList<QString> getExpresssionByToken( QString token, int type );

	GroupManager* _manager;

private:
	void getIndexsByMaxRate( QList<float> &rates, QList<int> indexs, int &which, int &index );
	void getIndexsByMinRate(  QList<float> &rates, QList<int> indexs, int &which, int &index );
	void loadName();
	QString extractFullName( QString path );
	QString getFullName( QString abber );
	int getIndexByToken( QString name );
	QList<QString> getVideoPathByToken( QString name, int type  );
	QList<QString> getBatPathByToken( QString token );
	QList<QString> getPCPathByToken( QString token, int type );	
	

	//QList<CurveMatch> getMatchBatPath( QList<QString> names, DESCRIPTOR dis, int number, int index, QList<float> &rates );
	//QList<QString> getCamberPath( QList<QString> names, int number );
	QList<QString> _fileName;
	QList<QString> _fullName;
	QList<int> _startFrame;
	QList<int> _endFrame;
	ItemBase* openSpecifiedViewByName( QString name, int view, QPointF pos );
	QString getNameFromPath(QString name)
	{
		int tindex;

		tindex = name.lastIndexOf("/");
		name.remove(0, tindex+1);
		tindex = name.lastIndexOf("_");
		name.remove(tindex, name.length()-tindex);

		return name;
	}

	//
	OpenGLScene* _scene;
	LabObject* _lab; 
	LabCut* _cut;
	
	//AllBats* _allBats;
	//
	QString _batPrefix;
	QString _pcPrefix;
	QString _videoPrefix;
	//
	//bool _CAMBER;
	//Camber* _Camber;
	//
	SpaceManager* _space;
	//

	bool _NEW;

	public slots:
		void needCapture( myVideoPlayer* video );
};

#endif