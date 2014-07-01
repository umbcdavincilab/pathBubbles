#ifndef GROUPMANAGER_H
#define GROUPMANAGER_H

#include <QObject>
#include <QList>
#include <QPointF>
#include "Group.h"

class ItemBase;
class OpenGLScene;

class GroupManager : public QObject
{
	Q_OBJECT
public:
	GroupManager( OpenGLScene* scene );
	virtual ~GroupManager()//J.C.
	{
		delete _scene;
		_list.clear();
	}
	void acceptSelectedArea( QRectF rect );
	bool oneOverAnother( ItemBase *left, ItemBase *right );
	void newItemAdded( ItemBase* item );
	void removeItem( ItemBase* item );
	void updateWingShapeParainGroup(ItemBase* bat, ArrowGen* agen);
	void addToGroup( QList<ItemBase*> list, ItemBase* init );
	void addToGroup( ItemBase* add, ItemBase* init );
	QList<ItemBase*> allMembers();
	QList<Group*> groupList();
	void updateItemsPos();
	void clearAll()
	{
		this->_list.clear();
	}
	bool isOnMoving()
	{
		return this->_ONMOVING;
	}
	void addNewGroupFromXmlNode( QDomElement node );
	void drawAllGroupBoarder( QPainter* painter );
	int findGroupIndex( ItemBase* item );
	QtBat* getBatLinkWithPC( QtSM* item );
	QtBat* getBatLinkWithVideo( VideoBlock* item );

	void logAddToGroup( QList<ItemBase*> newMembers, int index );
	void logRemoveItem( ItemBase* item, int index );

public slots:
	void updateLabelState( ItemBase* init);
	void updatePCColorState( ItemBase* init, int input );
	void itemPosUpdated( ItemBase* init );
	void itemPosMoved( ItemBase* init, QPointF offset );
	void itemRemoved( ItemBase* init );
	void unGroup( ItemBase* init );
	void updateSelectBats( ItemBase* bat, QPointF offset );
	void swapSelectPCs( SM* sm, int source, int target );
	void addBatsToScene( QList<QList<ItemBase*>> bats );

	//for bat
	void sliderChanged(ItemBase* bat, int );
	void surfaceChecked(ItemBase* bat, int );
	void wireframeChecked(ItemBase* bat, int );
	void symbolChecked(ItemBase* bat, int );
	void camber1Checked(ItemBase* bat, int );
	void camber2Checked(ItemBase* bat, int );
	void camber3Checked(ItemBase* bat, int );

	//for PC
	void HighlightChangeinGroup( ItemBase* item, bool );
	void pressHightlightinGroup( ItemBase *item, QRect rect );
	void moveHightlightinGroup( ItemBase *item, QRect rect );
	void releaseHightlightinGroup( ItemBase *item );

private:
	bool sameGroup( ItemBase* left, ItemBase* right );
	void unHightLightAll();
	void findAndHighlight( ItemBase* init );
	void findOverlap( ItemBase* init );
	void highLightGroup( QList<ItemBase*> list, ItemBase* init );
	QList<ItemBase*> getCurrentItems( ItemBase* init );
	bool Overlaied( ItemBase* left, ItemBase* right );
	bool needCompare( ItemBase* left, ItemBase* right );
	void setCompareWithOutPlot( ItemBase* target, ItemBase* init );
	bool isInThisList( QList<ItemBase*> list, ItemBase* init );
	void tightenItemsinSameGroup();
	void disperseItemsinSameGroup();

	OpenGLScene* _scene;
	myColor3 _highLightColor;
	QList<Group*> _list;
	QList<ItemBase*> _selected;

	bool _ONMOVING;
};
#endif