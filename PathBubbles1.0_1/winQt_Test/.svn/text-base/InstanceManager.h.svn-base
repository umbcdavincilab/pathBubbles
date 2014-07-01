#ifndef INSTANCEMANAGER_H
#define INSTANCEMANAGER_H

#include "BubbleInstance.h"
#include <fstream>
#include <QList>
#include "BubbleNode.h"

class InstanceManager
{
public:
	InstanceManager();
	void addInstance( BubbleInstance* instance );
	void setOrder( int* order );
	void initTree();
	void renderRoot( QPainter* painter, QRectF bounding, QRectF max )
	{
		bool font = true;
 		if( this->_ROOT )
		{
			this->_root->paint( painter, bounding, max );
		}
	}
	BubbleNode* getRoot()
	{
		return this->_root;
	}
	void highLightInstance( QPointF pos );
	void getHighLighted( QString &name );
private:
	QList<BubbleInstance*> _instance;
	void printOutTree();
	void recursiveInitNode( int level, BubbleNode* parent );
	QList<BubbleInstance*> instanceOnThisLevel( int level, BubbleNode* parent );
	//BubbleInstance* getInstanceByKind( int level );
	int _level;
	//		1 individual 2 run 3 view
	int _order[3];
	BubbleNode* _root;
	bool _ROOT;
};

#endif