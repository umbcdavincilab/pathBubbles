
#include "GraphBlock.h"
#include "openglscene.h"

GraphBlock::GraphBlock( int size, int x, int y ): ItemBase( GRAPH,size, x, y )
{
	this->_TYPE = GRAPH;
	this->bIndex = ItemBase::getBubbleIndex();

//	tree = parse_newick_file( "D:\\usm\\group.proml.fullname" );
	tree = parse_newick_file( "H:\\visualizationproject\\graph\\group.human.txt" );

	connect( this, SIGNAL(unGroup(ItemBase*)), getGroupManager(), SLOT(unGroup( ItemBase*)) );
	connect( this, SIGNAL(itemRemoved( ItemBase*)), getGroupManager(), SLOT(itemRemoved( ItemBase* )) );
	connect( this, SIGNAL(itemPosMoved( ItemBase*, QPointF)), getGroupManager(), SLOT(itemPosMoved( ItemBase*, QPointF)) );
	connect( this, SIGNAL( itemPosUpdated(ItemBase*) ), getGroupManager(), SLOT( itemPosUpdated(ItemBase*)) );
	connect( this, SIGNAL( logInfor( QString ) ), getOpenGLScene(), SLOT( logInfor( QString ) ) );
	connect( this, SIGNAL( itemCopyTo( ItemBase*, QPointF ) ), getOpenGLScene(), SLOT( itemCopyTo( ItemBase*, QPointF ) ) );

	logOpen();
}

void GraphBlock::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
	drawTheTree_bMethod( painter );

	ItemBase::paint( painter, option, widget );
}

void GraphBlock::drawTheTree_random( QPainter *painter )
{
	int nodeNum = tree.get()->size();
	queue<int> Sq;
	double eachWidth, px, py;
	int tmpNum;
	vector<int> sonNode;
	QPointF *nodePos = new QPointF[nodeNum];

	Sq.push( tree.get()->root() );
	py = -this->Height()/2.0;

	while ( !Sq.empty() )
	{
		tmpNum = Sq.size();	
		
		eachWidth = this->Width()*1.0/( tmpNum + 1 );
		py += 30;
		px = -this->Width()/2.0;

		while ( tmpNum > 0 )
		{
			int a = Sq.front();
			Sq.pop();
			tmpNum --;
			px += eachWidth;
			
			nodePos[a] = QPointF(px, py);

			sonNode = tree.get()->get_child( a );
			for ( int i = 0; i < sonNode.size(); i ++ )
			{
				Sq.push( sonNode[i] );
			}
		}
	}

	painter->save();

	for ( int i = 0; i < nodeNum; i ++ )
	{
		sonNode = tree.get()->get_child( i );
		painter->setPen( Qt::green );
		painter->drawEllipse( nodePos[i], 2, 2 );
		QString str = tr( tree.get()->label( i ).c_str() );
		painter->setPen( Qt::darkBlue );
		painter->drawText( nodePos[i].x(), nodePos[i].y()-5, str );

		painter->setPen( Qt::yellow );
		for ( int j = 0; j < sonNode.size(); j ++ )
		{
			painter->drawLine( nodePos[i], nodePos[sonNode[j]] );
		}
	}

	painter->restore();
}

void GraphBlock::drawTheTree_bMethod( QPainter *painter )
{
	int nodeNum = tree.get()->size();
	QPointF *nodePos = new QPointF[nodeNum];

	QString maxlenName = tr(tree.get()->getMaxlenLabel().c_str());
	QRectF bounding = painter->fontMetrics().boundingRect( maxlenName );

	double rightwidth = this->Width()/2 - 20 - bounding.width();
	double eachHeight = 1.0*this->Height() / ( tree.get()->leafNum() +1);
	double curheight = -this->Height()/2.0 + eachHeight;

	for ( int i = 0; i < nodeNum; i ++ )
	{
		if ( tree.get()->is_leaf( i ) )
		{
			nodePos[i] = QPointF( rightwidth, curheight );
			curheight += eachHeight;
		}
	}

	int treeRoot = tree.get()->root();
	vector< vector<int> > allEdge = tree.get()->get_edge();

	dfsSetTreeNodePos( treeRoot, allEdge, nodePos );
	
	double miny, maxy, tmpy;

	painter->save();
	for ( int i = 0; i < nodeNum; i ++ )
	{
		painter->setPen( Qt::green );
		painter->drawEllipse( nodePos[i], 2, 2 );
		QString str = tr( tree.get()->label( i ).c_str() );
		painter->setPen( Qt::darkBlue );
		painter->drawText( nodePos[i].x(), nodePos[i].y()-5, str );

		if ( tree.get()->is_leaf( i) )
		{
			continue;
		}

		miny = this->Height()/2.0;
		maxy = -this->Height()/2.0;

		painter->setPen( Qt::yellow );
		for ( int j = 0; j < allEdge[i].size(); j ++ )
		{
			tmpy = nodePos[allEdge[i][j]].y();
			painter->drawLine( QPointF(nodePos[i].x(), tmpy), QPointF(nodePos[allEdge[i][j]].x(), tmpy) );

			if ( tmpy < miny )
			{
				miny = tmpy;
			}
			if ( tmpy > maxy )
			{
				maxy = tmpy;
			}
		}
		painter->drawLine( QPointF(nodePos[i].x(), miny), QPointF(nodePos[i].x(), maxy) );
	}
	painter->restore();

}

void GraphBlock::dfsSetTreeNodePos( int node, vector< vector<int> > allEdge , QPointF *nodePos )
{
	if ( allEdge[node].size() == 0 )
	{
		return;
	}

	for ( int i = 0; i < allEdge[node].size(); i ++ )
	{
		dfsSetTreeNodePos( allEdge[node][i], allEdge, nodePos );
	}
	
	double minWidth = this->Width()/2.0;
	double miny = this->Height()/2.0;
	double maxy = - this->Height()/2.0;

	for ( int i = 0; i < allEdge[node].size(); i ++ )
	{
		if ( nodePos[allEdge[node][i]].x() < minWidth )
		{
			minWidth = nodePos[allEdge[node][i]].x();
		}
		if ( nodePos[allEdge[node][i]].y() < miny )
		{
			miny = nodePos[allEdge[node][i]].y();
		}
		if ( nodePos[allEdge[node][i]].y() > maxy )
		{
			maxy = nodePos[allEdge[node][i]].y();
		}
	}
	nodePos[node] = QPointF( minWidth-20, (miny+maxy)/2.0 );
}