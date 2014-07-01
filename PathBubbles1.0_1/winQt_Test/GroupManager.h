#ifndef GROUPMANAGER_H
#define GROUPMANAGER_H

#include <QObject>
#include <QList>
#include <QPointF>
#include "Group.h"
//#include "subPathBubble1.h"

class ItemBase;
class OpenGLScene;
class expressionBubble;
class treeRingBubble;
class PathBubble1;
class WebBlock;

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
	
	QPointF getPosition(ItemBase *parent, ItemBase *newitem, int x, int y, int width, int height); //Keqin // reassign a position if the newly created item overlap other position/ do not add to group since it is just created
    QPointF getPosition(ItemBase *newitem, int x, int y, int width, int height); //Keqin // reassign a position if the newly created item overlap other position/ do not add to group since it is just created
	bool outBox(float x1, float y1, float width1, float height1, float x2, float y2, float width2, float height2, float margin);
	void acceptSelectedArea( QRectF rect );
	bool oneOverAnother( ItemBase *left, ItemBase *right);
	void newItemAdded( ItemBase* item );
	void removeItem( ItemBase* item );
	void removeGroup( Group* group);
	void updateWingShapeParainGroup(ItemBase* bat, ArrowGen* agen);
	void addToGroup( QList<ItemBase*> list, ItemBase* init, bool updatePos=true, bool setPos=false);
	void addToGroup( ItemBase* add, ItemBase* init, bool updatePos=true );
	QList<ItemBase*> allMembers();
	QList<Group*> allGroups();
	QList<Group*> groupList();
	void updateItemsPos();
	void bubbleDeleted( ItemBase* init, QRectF parentRect=QRectF(-10000,-10000,-10000,-10000));
	vector<set<int> > GroupToFamily(QList<ItemBase *> list);
	vector<set<int> > GroupToFamily(QList<ItemBase *> list, ItemBase *init);
	vector<int> getParentList(QList<ItemBase *> list, vector< set<int> > familySets, ItemBase *parent=NULL);	
    bool sameFamilySet(int id1, int id2, vector< set<int> > familySets);

	bool lineUpDisplayGroups(QList<ItemBase *> items, vector<QRectF>  rectList, bool setPos=false);
	vector<QPointF> getSynchronizedSpeed(QList<ItemBase*> list, vector<QRectF> preRect, vector<QRectF> newRect);
	QPointF getSynchronizedSpeed(ItemBase* list, QRectF preRect, QRectF newRect);
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
	
    void drawAllLabel(QPainter *painter);
	int findGroupIndex( ItemBase* item );

	bool RectRect(float cx1,float cy1,float cx2, float cy2, float w1, float h1, float w2, float h2);
    void encode(float x,float y,int &code,float XL,float XR,float YB,float YT);
	float OverlapArea(float cx1, float cy1, float cx2, float cy2, float w1, float h1, float w2, float h2);
	bool Enclosed(ItemBase *left, ItemBase *right);
	bool Enclosed(ItemBase *left, QList<ItemBase *> right);
	//QtBat* getBatLinkWithPC( QtSM* item );
	//QtBat* getBatLinkWithVideo( VideoBlock* item );

	void logAddToGroup( QList<ItemBase*> newMembers, int index );
	void logRemoveItem( ItemBase* item, int index );

	void newBubbleCreated( ItemBase* init );
	bool moveOverlaped( ItemBase* init ) ;//but do not add to group

	//void bubbleResized( ItemBase* init );

   bool adjustOverlaid( int eventType, ItemBase* init, QRectF oriRect, int cornerIndex, bool notLayoutGroup=false, bool setPos=false);
   bool tightenGroupMembers(int eventType, ItemBase* init, QRectF oriRect, int cornerIndex, bool deleted, bool setPos=false, QRectF parentRect=QRectF(-10000,-10000,-10000,-10000), bool notLayoutGroup=false);    
   bool shiftNeighborforResizing(int eventType, ItemBase* init, QRectF oriRect, int cornerIndex, bool notLayoutGroup, bool setPos);
   bool bubbleGrouped(Group *aGroup, ItemBase *init, set<int> Init, bool notLayoutGroup=false, bool setPos=false);
   QPointF gravityCenter(vector<QRectF> rectList, int init);
	//int isTightend;
   void itemPosUpdatedforSpring( ItemBase* init );
signals:	
	void findEnclosed(PathBubble1* b0, PathBubble1* tmp);
	void findEnclosedTreeRing(treeRingBubble* b1, treeRingBubble* b2);
public slots:
	void bubbleResized( ItemBase* init, QRectF oriRect, int cornerIndex);
	void updateLabelState( ItemBase* init);
	void updatePCColorState( ItemBase* init, int input );
	void itemPosUpdated( ItemBase* init );
	void itemPosMoved( ItemBase* init, QPointF offset );
	void itemRemoved( ItemBase* init );
	void unGroup( ItemBase* init );
	void updateSelectBats( ItemBase* bat, QPointF offset );
	void swapSelectPCs( SM* sm, int source, int target );
	//void addBatsToScene( QList<QList<ItemBase*>> bats );

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
	void pressHightlightinGroup( ItemBase *item, QRectF rect );
	void moveHightlightinGroup( ItemBase *item, QRectF rect );
	void releaseHightlightinGroup( ItemBase *item );
	QList<ItemBase*> getGroupMembers(ItemBase* init);
	Group* getGroup(ItemBase* init);
	void tightenItemsinSameGroup();
	

private:
	bool sameGroup( ItemBase* left, ItemBase* right );
	void unHightLightAll();
	void findAndHighlight( ItemBase* &init );
	int findOverlap( ItemBase* init );
	void highLightGroup( QList<ItemBase*> list, ItemBase* &init );
	QList<ItemBase*> getCurrentItems( ItemBase* init );
	bool Overlaid( ItemBase* left, ItemBase* right );
	
	bool needCompare( ItemBase* left, ItemBase* right );
	void setCompareWithOutPlot( ItemBase* target, ItemBase* init );
	bool isInThisList( QList<ItemBase*> list, ItemBase* init );
	
	void disperseItemsinSameGroup();
	void DisperseAndTighteninSameGroup();

	OpenGLScene* _scene;
	myColor3 _highLightColor;
	QList<Group*> _list;
	QList<ItemBase*> _selected;

	bool _ONMOVING;
};
#endif