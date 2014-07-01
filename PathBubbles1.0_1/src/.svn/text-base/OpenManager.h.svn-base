#ifndef OPENMANAGER_H
#define OPENMANAGER_H

#include <QtGlobal>
#include <fstream>
#include <QList>
#include <QObject>
#include "matBlock.h"
#include "LabCut.h"
#include "LabObject.h"
#include "AllBats.h"
#include "Discreptor.h"
#include "SpaceManager.h"

class QtBat;
class ItemBase;
class OpenGLScene;
class GroupManager;
class Camber;
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
	OpenManager( OpenGLScene* scene, LabObject* lab, LabCut* cut, GroupManager* manager, AllBats* bats );
	bool openBatByPath( ItemBase* &bat, int size, int x, int y, QString name, int index = 0 );
	QString checkBatByPath( QString name );
	bool openBatByPathWithoutRegister( ItemBase *&result, int size, int x, int y, QString name, int index = 0 );
	bool openPCByPath( ItemBase *&result, int size, int x, int y, QString name );
	QString checkPCByPath( QString name );
	ItemBase* openWebView( int size, int x, int y, QString name = tr("") );
	bool openVideoByPath( ItemBase* &result, int size, int x, int y, QString name );
	QString checkVideoByPath( QString name );

	ItemBase* openNote( int size, int x, int y );
	ItemBase* openPen( int size, int x, int y );
	ItemBase* openMatlab( int size, int x, int y );
	ItemBase* openMatlabCopy( int size, int x, int y, MatBlock* matCur );
	ItemBase* openMatlabByPath( int size, int x, int y, QString name );

	ItemBase* openParallelCoordinate( int size, int x, int y, QString name = tr("") );
	ItemBase* openBarChart( int size, int x, int y, QString name = tr("") );
	ItemBase* openLineChart( int size, int x, int y, QString name = tr("") );
	ItemBase* openScatterPlot( int size, int x, int y, QString name = tr("") );

	//ItemBase* openVTK( int size, int x, int y );
	ItemBase* openImage( int size, int x, int y, QString path, float rate );
	QList<ItemBase*> openBatByToken( QString name );
	bool findBatByToken( QString name, DESCRIPTOR dis, int number, int index, QList<ItemBase*> &bats, QList<float> &rates );
	//
	bool getCAMBER();
	Camber* getCurrentCamber();
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

	ItemBase* openBatFromXmlNode( QDomElement node );
	ItemBase* openPCFromXmlNode( QDomElement node );
	ItemBase* openVideoFromXmlNode( QDomElement node );
	ItemBase* openWebFromXmlNode( QDomElement node );
	ItemBase* openBarChartFromXmlNode( QDomElement node );
	ItemBase* openLineChartFromXmlNode( QDomElement node );
	ItemBase* openScatterFromXmlNode( QDomElement node );
	ItemBase* openMatlabFromXmlNode( QDomElement node );
	ItemBase* openNoteFromXmlNode( QDomElement node );

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
	QList<CurveMatch> getMatchBatPath( QList<QString> names, DESCRIPTOR dis, int number, int index, QList<float> &rates );
	QList<QString> getCamberPath( QList<QString> names, int number );
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
	GroupManager* _manager;
	AllBats* _allBats;
	//
	QString _batPrefix;
	QString _pcPrefix;
	QString _videoPrefix;
	//
	bool _CAMBER;
	Camber* _Camber;
	//
	SpaceManager* _space;
	//

	bool _NEW;

	public slots:
		void needCapture( myVideoPlayer* video );
};

#endif