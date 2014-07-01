#ifndef GROUP_H
#define GROUP_H

#include <QList>
#include <QPointF>
#include "qtbox.h"
#include "qtsm.h"
//#include "PathBubble1.h"// not allowed
#include "VideoBlock.h"
#include "CalloutNote.h"
#include "ArrowGen.h"
#include "myColor3.h"
//#include "treeRingBubble.h"

#define LINEUP 1
#define RANDOM 0


class Group
{
public:
	Group();
	~Group();
	bool isUnGroup()
	{
		for( int i = 0; i < this->_members.size(); i++ )
		{
			if(this->_members[i]->isUnGroup())
				return true;
		}
		return false;
	}


	bool inThisGroup( ItemBase* item );
	void highLight( myColor3 hiColor )
	{
		for( int i = 0; i < this->_members.size(); i++ )
		{
			this->_members[i]->hightLight( hiColor );
		}
	}
	int getLayoutType()
	{
	  return(layoutType);
	}
	int encode(float x,float y, float XL,float XR,float YB,float YT);
	int connectionIndex(int code);
	int getConnectionType(QPointF tar, float width1, float height1, QPointF cur, float width2, float height2, float margin=0.001);
	
	void unHightLight()
	{
		for( int i = 0; i < this->_members.size(); i++ )
		{
			this->_members[i]->unHighLight();
		}
	}
	void updateSharedSet(ItemBase *init);
	vector< set <int> > getGroupSegment(vector<QRectF> rectList);
	vector< set <ItemBase *> > getGroupSegment(ItemBase* init);

    int getUnionID(vector<set<ItemBase *> > unions, int id);
	int getUnionID(vector<set<int> > unions, int id);

	bool isChangeConnection(int i, vector<vector<int> > neighbor, vector<set<int> > familySets, vector<QRectF>  rectList);
    bool isbridgeBroken(ItemBase* init);	
    bool isbridgeBroken(vector<QRectF> rectList);
	bool isOverlaid( QList<ItemBase*>  members);
	void getNeighbor(set<int> initF, vector<set<int> > familySets, vector<QRectF>  RectList, int cornerIndex, vector<vector<int> > &referIndex, vector<vector<int>> &headID, vector<vector<QPointF> > &relativeLocation,	vector<vector<int> > &neighbor, bool isResize);

	bool outBox(QPointF cur, int width1, int height1, QPointF tar, int width2, int height2, int margin=3);
	

    void removeItem( ItemBase* item );
	void addToThisGroup( ItemBase* item, bool updatePos=true);
	void moveOtherMember( ItemBase* init, QPointF offset );
	void moveAssociateNodeNotes(ItemBase *init, QList<CalloutNote*> nodeNotes, QPointF offset);
	int NumOfMembers();
	ItemBase* getMemberAt( int index );
	int getMemberID( ItemBase*);
	QList<ItemBase*> getMembers(  );
	

	
	void groupCombine( Group* target, bool updatePos=true);
	set<int> groupCombine( QList<Group* > target, bool updatePos=true);
    //vector<set<int> > GroupToFamily(QList<ItemBase *> list);
	int getFamilyID(int id, vector<set<int> > familySets);
	//vector<QRectF> getFamiltyRects(int fid, set<int> familySet, vector<QRectF> rectList);	
	vector<QRectF> getFamiltyRects(int fid, set<int> familySet, vector<QRectF> rectList);
	bool isSameFamily(int id1, int id2, vector<set<int> > familySets);
	
	
	myColor3 getGroupColor(Group* target);
	myColor3 getGroupHighlightColor(ItemBase *topNode1 );
	ItemBase *compareTopNodeType(ItemBase *item1, ItemBase *item2);	
	int getBubbleLevel(ItemBase *item);

	void tightenItems();
	void tightenItems(vector<bool>  &tflag);
	void disperseItems(vector<bool>  &tflag);
	void disperseItems();
	void updateGroupLabelState();
	void updateGroupRotation( QPointF offset );
	void updateGroupPCColorState( int input );

	//void setBatGroupFrameState( float rate, int number );
	//void setBatGroupSurfaceState( bool state );
	//void setBatGroupWireframeState( bool state );
	//void setBatGroupCamberState( int camberindex, bool state );
	//void setBatGroupSymbolState( bool state );
	//void updateBatGroupWingShapePara( ArrowGen* agen );
	void setPCHighlightState( bool state );	
	
	QPointF moveOneorTwoSteps(int eventType, QRectF justMovedRect, QRectF toBeMovedRect, int direction, int &lineuped, bool disperse=false, float parentX=-10000000);
	bool isTwoRectConnected(QRectF justMovedRect, QRectF toBeMovedRect);
	QPointF getSmallestDistance(QRectF justMovedRect, QRectF toBeMovedRect);
	void shiftNeighbor(int parentID, QPointF moveCenterPos, QRectF oriRect, QRectF curRect, int justMoved, set<int> &movedSet, set<int> &notMovedSet, set<int> &shifted, vector<vector<int> > neighbor, int cornerIndex, vector<vector<int> > referIndex, vector<vector<int> > headID, vector<vector<QPointF> > relativeLocation, vector<QRectF> &rectList, vector<int> parentList, vector<int> directionMark, vector<set<int> > familySets, int screenWidth, int screenHeight, int screenPosx, int screenPosy);
	QPointF setNeighborPos(QRectF Rect, QPointF toBeMoved, QPointF relativePos, int index);

	bool adjustOverlaid(int initF, vector<QRectF>  &rectList, vector<int> parentList, vector<set<int> > familySets,  int screenWidth, int screenHeight, QRectF oriRect, int cornerIndex, int screenPosx, int screenPosy);// not shift neighbor
	vector<QRectF> lineUpDisplay(int eventType, QList<ItemBase *> members, vector<int> parentList, set<int> init, int size, vector<set<int> > familySets, vector<QRectF>  rectList, int screenWidth=100, int screenHeight=100, QRectF oriRect=QRectF(-100000,-100000,-100000,-100000), int cornerIndex=-1, int sx=-10000, int sy=-1000, bool bigRect=false);
	bool isOneWayConnection(int testID, int referIndex, vector<QRectF> rectList);
	void dSpacer(int justMoved, set<int> &movedSet, set<int> &notMovedSet, vector<QRectF> &rectList, vector<int> parentList, vector<int> &directionMark, vector<set<int> > familySets, int screenWidth, int screenHeight, int screenPosx, int screenPosy); //not shift neighbors
	void dSpacer(int eventType, int justMoved, QRectF oriRect, QRectF curRect, set<int> &movedSet, set<int> &notMovedSet, set<int> &shifted, vector<vector<int> > neighbor, int cornerIndex, vector<vector<int> > referIndex, vector<vector<int> > headID, vector<vector<QPointF> > relativeLocation, vector<QRectF> &rectList, vector<int> parentList, vector<int> &directionMark, vector<set<int> > familySets, int screenWidth, int screenHeight, int screenPosx, int screenPosy);
	void tSpacer(int eventType, int justMoved, QRectF oriRect, QRectF curRect, set<int> &movedSet, set<int> &notMovedSet, set<int> &shifted, vector<vector<int> > neighbor, int cornerIndex, vector<vector<int> > referIndex, vector<vector<int> > headID, vector<vector<QPointF> > relativeLocation, vector<QRectF> &rectList, vector<int> parentList, vector<int> &directionMark, vector<set<int> > familySets, int screenWidth, int screenHeight, int screenPosx, int screenPosy);
	vector<QRectF> assignRectList(QList<ItemBase *> members, bool bigRect=false, int start=0);
	
	vector<int> findNeighbor(int i, QList<ItemBase *> members, set<int> initF, vector<set<int> > familySets);
	vector<int> findConnectionF(int i, vector<set<int> > familySets, vector<QRectF>  rectList, int dis);
	vector<int> findConnectionF(int i, vector<set<int> > familySets, vector<QRectF>  rectList, vector<int> &Index, vector<int> &headNeighbor, int dis);
	void moveToClosest(QRectF justMovedRect, QRectF &toBeMovedRect, int direction, bool disperse);
	bool isChangeConnection(int initID, QRectF oriRect, vector<QRectF> rectList);
	bool isConnectionF(int i, int justMoved, vector<set<int> > familySets, vector<QRectF>  rectList);
    set<int> findConnectionFSet(int i, vector<set<int> > familySets, vector<QRectF>  rectList, int dis);
	vector<int> findConnectionFSet(int i, vector<set<int> > familySets, vector<QRectF>  rectList, vector<int> &Index, vector<int> &headNeighbor, int dis);
	vector<QRectF> assignSceneRectList(QList<ItemBase *> members,  bool bigRect=false, int start=0);
	QPointF getShiftedPos(int resizedID, int testID, int cornerID, int referID, QRectF headRect, QRectF oriRect, QRectF curRect, vector<vector<int> > neighbor, vector<vector<int> > referIndex);
	bool ifShareKeyNeighbor(int resizedID, int testID, int cornerID, int testReferIndex, vector<vector<int> > neighbor, vector<vector<int> > referIndex);
	
	bool overlaid(QRectF left, QRectF right);
	bool overlaid(ItemBase *left, ItemBase *right);
	bool overlaid(int l, int r, vector<QRectF> rectList, vector<set<int> > familySets);
	bool overlaid(int l, set<int> movedSet, vector<QRectF> rectList, vector<set<int> > familySets);
	bool overlaid(int l, int r, vector<QRectF> rectList);
	bool isOverlaid( vector<QRectF> rectlist);
	int findOverlaid( int id, vector<QRectF> rectlist);
	float getMostLowPos(set<int> family, vector<QRectF> rectList);
    float getMostLeftPos(set<int> family, vector<QRectF> rectList);
    float getMostHighPos(set<int> family, vector<QRectF> rectList);
	float getMostRightPos(set<int> family, vector<QRectF> rectList);
	QPointF moveFamilyOneDirection(int eventType, int justMovedID, int toBeMovedID, set<int> movedSet, vector<QRectF> rectList, vector<int> parentList, vector<set<int> > familySets, int direction, int &lineuped, bool disperse=false, int screenWidth=2000, int screenHeight=2000, int screenPosx=0, int screenPosy=0);
	QPointF moveMoreSteps(int eventType, int justMovedID, int toBeMovedID, set<int> justMoved, vector<QRectF> rectList, int direction,int &lineuped, bool disperse, float parentX);
	QPointF moveOneDirection(int justMovedID, int toBeMovedID, set<int> movedSet, vector<QRectF> rectList, int direction);//move one direction through all the justmoved bubble
	QRectF moveOverlaid(int justMovedID, int toBeMovedID, vector<QRectF> &rectList, vector<int> &directionMark); //consider two single bubble
	QRectF moveOverlaid(int justMoved, int id, set<int> &movedSet, set<int> &notMovedSet, vector<QRectF> &rectList, vector<int> &directionMark, int screenWidth, int screenHeight);	 //consider a bubble interected with a group bubble in justmoved
	QPointF moveOverlaid(int eventType, int justMoved, int id, set<int> &movedSet, set<int> &notMovedSet, vector<QRectF> &rectList, vector<int> parentList, vector<int> &directionMark, vector<set<int> > familySets, int screenWidth, int screenHeight, int screenPosx, int screenPosy, bool disperse=false);	  //consider a bubble family interected with a group bubble in justmoved 

	void updateLayoutType();
	void updateLineUpListAdd( int ibegin, int iend );
	void updateLineUpListRemove( int index );
	void insertIntoLineUpList( int index );
	void updateBoarderState();
	
	void swapPCBars( int source, int target );
	void pressHightlight( QRectF rect );
	void moveHightlight( QRectF rect );
	void releaseHighlight();
	void drawGroupBoarder( QPainter* painter );
	
	QList<ItemBase*> getGroupMembers()
	{
	   return this->_members;
	}

	//QtBat* getBatLinkWithPC( QtSM* item );
	//QtBat* getBatLinkWithVideo( VideoBlock* item );
	
	vector<set<vector<int>>> sharedSets; //record the search found shared sets (to color them differently)
	vector<set<vector<int>>> expressedSets;

private:
	QList<ItemBase*> _members;
	int layoutType;
	QList< QList<int> > lineUpList;

};
#endif