#ifndef GRAPHBLOCK_H
#define GRAPHBLOCK_H

#include "qtbox.h"
#include <queue>
#include "tree.h"
#include "newick_file.h"
using namespace BiRC::treelib;
using namespace std;

class GraphBlock: public ItemBase
{
public:
	GraphBlock( int size, int x, int y );

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
	void drawTheTree_random( QPainter *painter );
	void drawTheTree_bMethod( QPainter *painter );
	void dfsSetTreeNodePos( int tRoot, vector< vector<int> > allEdge , QPointF *nodePos );

	std::auto_ptr<Tree> tree;
};

#endif