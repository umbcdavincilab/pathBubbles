#ifndef GROUP_H
#define GROUP_H

#include <QList>
#include <QPointF>
#include "qtbox.h"
#include "qtsm.h"
#include "qtbat.h"
#include "VideoBlock.h"
#include "ArrowGen.h"
#include "myColor3.h"

#define LINEUP 1
#define RANDOM 0

class Group
{
public:
	Group();
	~Group();
	bool inThisGroup( ItemBase* item );
	void highLight( myColor3 hiColor )
	{
		for( int i = 0; i < this->_members.size(); i++ )
		{
			this->_members[i]->hightLight( hiColor );
		}
	}
	void unHightLight()
	{
		for( int i = 0; i < this->_members.size(); i++ )
		{
			this->_members[i]->unHighLight();
		}
	}
	void removeItem( ItemBase* item );
	void addToThisGroup( ItemBase* item );
	void moveOtherMember( ItemBase* init, QPointF offset );
	int NumOfMembers();
	ItemBase* getMemberAt( int index );
	void groupCombine( Group* );
	void tightenItems();
	void disperseItems();
	void updateGroupLabelState();
	void updateGroupRotation( QPointF offset );
	void updateGroupPCColorState( int input );

	void setBatGroupFrameState( float rate, int number );
	void setBatGroupSurfaceState( bool state );
	void setBatGroupWireframeState( bool state );
	void setBatGroupCamberState( int camberindex, bool state );
	void setBatGroupSymbolState( bool state );
	void updateBatGroupWingShapePara( ArrowGen* agen );

	void setPCHighlightState( bool state );

	void updateLayoutType();
	void updateLineUpListAdd( int ibegin, int iend );
	void updateLineUpListRemove( int index );
	void insertIntoLineUpList( int index );
	void updateBoarderState();
	
	void swapPCBars( int source, int target );
	void pressHightlight( QRect rect );
	void moveHightlight( QRect rect );
	void releaseHighlight();
	void drawGroupBoarder( QPainter* painter );

	QtBat* getBatLinkWithPC( QtSM* item );
	QtBat* getBatLinkWithVideo( VideoBlock* item );

private:
	QList<ItemBase*> _members;
	int layoutType;
	QList< QList<int> > lineUpList;

};
#endif