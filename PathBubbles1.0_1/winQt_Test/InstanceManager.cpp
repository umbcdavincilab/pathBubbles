#include "InstanceManager.h"

InstanceManager::InstanceManager()
: _level( 3 )
, _ROOT( false )
, _instance()
{
	//		default order
	
	//for( int i = 1; i < 4; i++ )
	//{
	//	order[i-1] = i;
	//}
	int order[3];
	order[0] = 1;
	order[1] = 2;
	order[2] = 3;
	this->setOrder( order );
	_instance.clear();
	return;
}

void InstanceManager::setOrder(int *order)
{
	for( int i = 0; i <= _level; i++ )
	{
		this->_order[i] = order[i];
	}
	return;
}

void InstanceManager::addInstance( BubbleInstance *instance )
{
	this->_instance.append( instance );
	return;
}

void InstanceManager::initTree()
{
	this->_ROOT = false;
	Infor infor;
	infor.Control = 3;
	infor.Name = QObject::tr( "root" );
	this->_root = new BubbleNode( infor );
	//
	this->recursiveInitNode( 2, this->_root );
	//	print out the tree structure 
	//
	//printOutTree();
	this->_ROOT = true;
}

void InstanceManager::printOutTree()
{
	std::ofstream tree("Tree_log");
	this->_root->printNode( &tree, 0 );
	return;
}

void InstanceManager::recursiveInitNode( int level, BubbleNode *parent )
{
	if( level < 0 )
	{
		return;
	}
	QList<BubbleInstance*> stack = instanceOnThisLevel( level, parent );
	for( int i = 0; i < stack.size(); i++ )
	{
		BubbleInstance* tempInstance = stack[i];
		//
		Infor infor;
		infor.Type = _order[level];
		infor.Name = tempInstance->getDiscreption( _order[level] );
		infor.Label = tempInstance->getDiscreption(4); //keqin
		if( level > 0 )
		{
			infor.Control = 2;
		}
		else
		{
			infor.Control = 1;
		}
		//
		BubbleNode* child = new BubbleNode( infor, parent );
		
		this->recursiveInitNode( level-1, child );
		//
		parent->addChild( child );
	}
	return;
}

QList<BubbleInstance*> InstanceManager::instanceOnThisLevel( int level, BubbleNode* parent )
{
	QList<BubbleInstance*> stack;
	QList<QString> type;
	for( int i = 0; i < this->_instance.size(); i++ )
	{
		if( parent->satisfyNode(this->_instance[i]) )
		{
			if( !type.contains( this->_instance[i]->getDiscreption( this->_order[level]) ) )
			{
				stack.append( this->_instance[i] );
				type.append( this->_instance[i]->getDiscreption( this->_order[level]) );
			}
		}
	}
	return stack;
}

void InstanceManager::highLightInstance( QPointF pos )
{
	POS even;
	even.pos = pos;
	even.ACCEPT = false;
	this->_root->highLight( even );
	return;
}

void InstanceManager::getHighLighted( QString &name)
{
	this->_root->getHighLight( name );
	return;
}