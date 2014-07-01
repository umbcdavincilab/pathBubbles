#ifndef QTBAT_H
#define QTBAT_H

#include <qtbox.h>
#include <QFile>
#include <GL/freeglut.h>
#include <string>
#include <QGraphicsProxyWidget>
#include <QFont>
#include "animateit.h"
#include "Bat.h"
#include "LabObject.h"
#include "LabCut.h"
#include "Lab3D.h"
#include "TextNote.h"
#include "PenDrawing.h"
#include "PenText.h"
#include "PenCamber.h"
#include "CamberMenu.h"
#include "ArrowGen.h"
#include "BatDataView.h"
//#include "qtwake.h"

class OpenGLScene;
class QtWake;

#define BATSCALE 10

struct WingNote
{
	PenText *text;
	int Frame;
	int Marker;
};

class QtBat : public ItemBase
{
	Q_OBJECT

public:
	QtBat( int size, int x, int y, OpenGLScene* Scene,  std::string name, LabObject *Lab, LabCut *Cut);
	static QString checkPath( QString path );
	void disableContextMenu();
	~QtBat();
	QString getBatName();
	void addBat( QtBat* input );
	void noteExistOrNot( int frame, int marker );
	void sendDrawingToPenBlock( QPointF pos );
	void displayCamberItem( QPainter* painter );
	void acceptNewBat( QtBat* target );
	void updateWingShapePara(ArrowGen* arrowGened);
	void updateTextNote();
	void updateLabel();
	float getSpeed();
	void unCompare();
	void unCompare( QString name );
	Bat* getBat();
	void setAnimation( QPoint Distination, int size );
	void setFrame( float index, int number );
	void setMassAndSpeed( float mass, float speed );
	void setChoosen( bool input )
	{
		_CHOOSEN = input;
		return;
	}
	BatContext getBatContext( )
	{
		return bat[0]->getBatContext();
	}
	void setBatContext(BatContext bcontext);
	void setSurface( bool input )
	{
		bat[0]->setSurface( input );
		surface->setChecked( input );
	}
	void setWireframe( bool input )
	{
		bat[0]->setWireframe( input );
		wireframe->setChecked( input );
	}
	void setSymbol( bool state )
	{
		bat[0]->setSymbols( state );
		symbols->setChecked( state );
	}
	void setHighlight( QList<int> list )
	{
		bat[0]->setHighlight( list );
	}
	void setCamberState(int index, bool state)
	{
		bat[0]->getCamber()->setCamberDisplay( index, state );
		if (index == 0)
		{
			camber1->setChecked( state );
		}
		else if (index == 1)
		{
			camber2->setChecked( state );
		}
		else if (index == 2)
		{
			camber3->setChecked( state );
		}
	}
	LabObject* getLab()
	{
		return _lab;
	}
	LabCut* getCut()
	{
		return _cut;
	}
	int getFrame()
	{
		return bat[0]->getCurrentNumberOfFrame();
	}
	int getTotalNumberOfFrame()
	{
		return bat[0]->getTotalNumberOfFrame();
	}
	bool between( int tar, int left, int right)
	{
		if( tar > left && tar < right)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	void saveAsXmlNode( QDomDocument &doc, QDomElement &node );
	void recoverFromXmlNode( QDomElement node );
	
	void logOpen();
	void logSelectTrailing();
	void logHideControl();
	void logShowControl();
	void logWanttoCom();
	void logClearOutCamber();
	void logCopyAtt();
	void logPasteAtt();
	void logOutputCamber();
	void logCamberSwitchProjection();
	void logCamberSwitchMarker();
	void logCamberAttachFrame();
	void logCamberDelete();

public slots:
	void acceptRotation( QPointF offset );
	void sliderChanged( int );
	void surfaceChecked( int );
	void wireframeChecked( int );
	void symbolChecked( int );
	void camber1Checked( int );
	void camber2Checked( int );
	void camber3Checked( int );

signals:
	void updateLabelState( ItemBase *item );
	
	void needCompare( QtBat *object );
	void removeCompare();
	void updateRotation( ItemBase *item, QPointF offset );
	void setCurrentBatAttribute( QtBat *object );
	void getNewBatAttribute( QtBat *object );

	void sliderChangedinGroup( ItemBase *item, int );
	void surfaceCheckedinGroup( ItemBase *item, int );
	void wireframeCheckedinGroup( ItemBase *item, int );
	void symbolCheckedinGroup( ItemBase *item, int );
	void camber1CheckedinGroup( ItemBase *item, int );
	void camber2CheckedinGroup( ItemBase *item, int );
	void camber3CheckedinGroup( ItemBase *item, int );

protected:
	void keyPressEvent( QKeyEvent * event );
	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
	void mousePressEvent( QGraphicsSceneMouseEvent *event );
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent( QGraphicsSceneMouseEvent *event );
	void hoverMoveEvent(QGraphicsSceneHoverEvent *event);

private:
	void addFloatingCamber( PenCamber* tempCamber );
	void displayCompareLink( QPainter* painter );
	void mouseOverCamberMenu( QPointF pos );
	void displayMenus( QPainter* painter );
	void displayOutCamber( QPainter* painter );
	void determineShift();
	void clearOutCamber();
	void selectWake();
	void registerWakePos();
	void clickMarkerOrNot();
	void paintNotes( QPainter* painter );
	void updateNotes();
	QList< QtWake* > _wakeOut;
	bool _paired;
	QList< QtBat* > _pair;
	void displayName( QPainter *painter );
	int half( int input )
	{
		return (int)((float)input / 2.0 + 0.5);
	}
	void initFloatingUI();
	void updateFloatingUI(BatContext bcontext);
	void updateControlPos();
	bool _ONLABEL;
	bool _CHOOSEN;
	bool _ONMOVE;
	bool _ONRESIZE;
	bool _ONCONTROL;
	bool _SELECTWAKE;
	bool _BEGINSELECTED;
	QRect _wakeSelectedRect;
	QPointF controlLastPos;
	int lastSize;
	QPointF lastPos;
	QLCDNumber* lcdNumber;
	QSlider* slider;
	QCheckBox* surface;
	QCheckBox* wireframe;
	QCheckBox* symbols;
	QCheckBox* camber1;
	QCheckBox* camber2;
	QCheckBox* camber3;
	animateIt* ani;
	OpenGLScene* scene;
	//Bat *bat;
	QList< Bat* > bat;
	std::string _name;
	LabObject *_lab;
	LabCut *_cut;
	//temp
	GLdouble posX, posY, posZ;
	GLfloat winX, winY, winZ;
	QPointF clickLastPos;
	bool _DETERMINESHIFT;
	QPointF mouseMovePos;
	float _mass;
	QString _strMass;
	float _speed;
	QString _strSpeed;
	QString _batMassAndSpeedInfor;
	int lengthForButtom;
	int heightForButtom;
	bool _NOTED;
	bool _LABEL;
	bool _RIGHTPOSNEEDED;
	QPointF _RIGHTPOS;
	bool OUTPUTCAMBER;
	bool INSERTNOTE;
	PenDrawing *_currentDrawing;
	int _CURRENTCAMBERMARKER;
	QList<PenCamber*> _outCamber;
	float _currentCamber;
	QList<WingNote> _notes;
	PenText *cur_note;
	bool _CONTEXTMENU;
	QList<CamberMenu*> _camberControl;
	bool addNewCamberControl( PenCamber* tempCamber );
	void updatePosofCamberControl();
	int CURCAMBERINDEX;

	BatDataView* rawDataView;
	bool _RAWDATAVIEW;

	QRectF nameRect;
}; 

#endif
