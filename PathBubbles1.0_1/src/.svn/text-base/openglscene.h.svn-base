#ifndef OPENGLSCENE_H
#define OPENGLSCENE_H

#include "point3d.h"
#include <QGraphicsScene>
#include <QLabel>
#include <QTime>
#include <QGraphicsProxyWidget>
#include <QtGui>
#include <QtOpenGL>
#include <QFileDialog>
#include "SM.h"
#include "LabCut.h"
#include "LabObject.h"
#include "qtsm.h"
#include "OrderManager.h"
#include "qtbat.h"
#include "ControlManager.h"
#include "AllBats.h"
#include "qtLCom.h"
#include "GroupManager.h"
#include "webQuery.h"
#include "matBlock.h"
#include "NoteBlock.h"
#include "webBlock.h"
#include "VideoBlock.h"
#include "PenBlock.h"
#include "VolumBlock.h"
#include "ControlBlock.h"
#include "NavigationBlock.h"
#include "OpenManager.h"
#include "FileManager.h"
#include "StatisticManager.h"
#include "StatisticBar.h"
#include "ArrowGen.h"
#include "BatContext.h"

#ifndef QT_NO_CONCURRENT
#include <QFutureWatcher>
#endif

#ifndef OUT
#define OUT 1
#define HIDDEN 2
#endif

class Model;

class OpenGLScene : public QGraphicsScene
{
	Q_OBJECT

public:
	OpenGLScene( LabObject* input2, LabCut* input3 );
	void drawBackground(QPainter *painter, const QRectF &rect); //QGraphicsScene
	void drawForeground(QPainter *painter, const QRectF &rect);
	void itemMoved();
	QWidget* getDialog( )
	{
		return this->createDialog( tr("Bat Controls") );
	}
	void myAddItem(  ItemBase * item )
	{
		this->addItem( item );
		this->_manager->newItemAdded( item );
	}
	LabObject* getLab() 
	{ 
		return lab; 
	}
	LabCut* getCut()
	{
		return cut; 
	}
	GroupManager* getGroupManager()
	{
		return this->_manager;
	}
	OpenManager* getOpenManager()
	{
		return _openManager;
	}
	StatisticBar* getStatisticBar()
	{
		return _staBar;
	}
	StatisticManager* getStaManager()
	{
		return _statistic;
	}
	bool changePlotAttribute( QPointF pos, QString name );
	void addLComObjectFromXmlNode( QDomElement node );

	QTime getCurTime()
	{
		return m_time.currentTime( );
	}
	void addLogText( QString str)
	{
		logStr.append( str );
	}

	void logOperation( QString str );
	void logPasteMatBubble( QPointF pos );

public slots:
	void logInfor( QString str );
	void setWingShapePara(ArrowGen* aGen, QPointF pos);
	void needStatisticalData( QPointF pos, StatisticManager* manager, int index );
	void moveButtonState( bool );
	void updateCoverage( float );
	void hideWebView();
	void setBackgroundColor();
	void orderViews();
	void floatingButtonPushed();
	void index_recieved( int index );
	void shiftViewport( QPoint offset );
	void addCompare( QtBat* object );
	void openAllPCs();
	void mapViews();
	void openCBRY()
	{
		_BATSPECIES = 1;
	}
	void openCBOR()
	{ 
		_BATSPECIES = 2;
	}
	void openCBDG()
	{ 
		_BATSPECIES = 3;
	}
	void openCBMA()
	{ 
		_BATSPECIES = 4;
	}
	void openCBYR()
	{ 
		_BATSPECIES = 5;
	}
	void openPCM1()
	{ 
		_PCINDEX = 1;
	}
	void openPCM2()
	{ 
		_PCINDEX = 2;
	}
	void openPCM3()
	{ 
		_PCINDEX = 3;
	}
	void setCurrentBatAttribute( QtBat *object );
	void getNewBatAttribute( QtBat *object );
	void setMatFileBubble( MatBlock* currentMat );
	int getBubbleIndex();

signals:
		void item_clicked( int flag );
		
public slots:
		void appHasClosed();

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void wheelEvent(QGraphicsSceneWheelEvent * wheelEvent);
	void contextMenuEvent( QGraphicsSceneContextMenuEvent * contextMenuEvent );
	void resizeEvent (QResizeEvent *event );
	void springEvent();

private:
	void buildMatBubbleConnect( QDomElement node );
	void initFloatingControl();
	void findSelected();
	void addLComObject( QtBat* left, QtBat* right );
	void findCompareTarget( QPointF pos );
	void removeAllItems();
	QDialog *createDialog(const QString &windowTitle) const;
	void setModel(Model *model);
	void recordSceneAsXmlFile( QString filename );
	void recoverSceneFromXmlFile( QString fileName );
	void writeLogFile();

	bool m_wireframeEnabled;
	bool m_normalsEnabled;
	bool _MOVEFLOATING;

	QColor m_modelColor;
	QColor m_backgroundColor;

	QTime m_time;
	QTimer timer;
	int m_lastTime;
	int m_mouseEventTime;

	float m_distance;
	Point3d m_rotation;
	Point3d m_angularMomentum;
	Point3d m_accumulatedMomentum;

	QCheckBox* surface;
	QCheckBox* wireframe;
	QLabel *m_labels[4];
	QWidget *floatControls;
	QSizeF _iniFloatingControlsSize;
	LabObject *lab;
	LabCut *cut;
	int STATE;
	int SHIFT;
	QWidget* Controls;
	animateIt* ani;
	QLCDNumber* lcdNumber;
	QSlider* slider;
	OrderManager* order;
	int timerId;

	//		reserved items in scene
	QtBat* _batAttached;
	bool _BATATTACHED;

	//
	bool _wantsCompare;
	QtBat* batWantsCompare;

	AllBats* allbats;
	std::string _perfix;
	//			specified open state
	int _BATSPECIES;
	int _PCINDEX;

	//			group manager
	GroupManager* _manager;

	//
	QGraphicsProxyWidget* _webView;
	bool _WEB;
	bool _MOVEBUTTON;
	//
	QPointF _TopLeft;
	//
	//bool _ONDRAG;
	//
	OpenManager* _openManager;
	//
	NavigationBlock* _navi;
	//
	FileManager* _file;
	//
	ArrowGen* _arrow;
	//
	void expendRect( QPointF pos );
	void drawSelecting( QPainter* painter );
	bool _MultiSelect;
	QPointF _initSelectPoint;
	QRectF _selectRect;
	//
	StatisticManager* _statistic;
	StatisticBar* _staBar;

	BatContext batAttribute;
	bool hasBatAttribute;

	MatBlock* matCurrent;
	bool hasMatBubble;

	bool recordingScene;

	QFile logFile;
	QTextStream log_output;
	QString logStr;

	int bubbleIndex;

#ifndef QT_NO_CONCURRENT
	QFutureWatcher<Model *> m_modelLoader;

#endif
};


#endif // OPENGLSCENE_H
