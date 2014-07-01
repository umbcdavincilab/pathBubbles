#include "Group.h"

Group::Group()
{
	layoutType = LINEUP;
	return;
}

Group::~Group()
{
	_members.clear();
	lineUpList.clear();
}

void Group::addToThisGroup(ItemBase *item)
{
	this->_members.append( item );
	if (this->_members.size() == 1)
	{
		QList<int> tmp;
		tmp.append(0);
		this->lineUpList.append( tmp );
		item->mySetPos( item->pos() );
		updateBoarderState();
	}
}

bool Group::inThisGroup(ItemBase *item)
{
	for( int i = 0; i < this->_members.size(); i++ )
	{
		if( this->_members[i] == item )
		{
			return true;
		}
	}
	return false;
}

void Group::moveOtherMember(ItemBase *init, QPointF offset)
{
	for( int i = 0; i < this->_members.size(); i++ )
	{
		if( this->_members[i] != init )
		{
			this->getMemberAt(i)->mySetPos( QPointF(_members[i]->pos().x() + offset.x(), _members[i]->pos().y() + offset.y()) );
		}
	}
	return;
}

int Group::NumOfMembers()
{
	return this->_members.size();
}

ItemBase* Group::getMemberAt( int index )
{
	return this->_members[index];
}

void Group::groupCombine( Group* target )
{
	int indexbegin, indexend;

	indexbegin = this->NumOfMembers();
	
	for( int i = 0; i < target->NumOfMembers(); i++ )
	{
		if( !this->inThisGroup( target->getMemberAt(i) ) )
		{
			this->addToThisGroup( target->getMemberAt(i) );
			target->getMemberAt(i)->setColor( this->_members[0]->getColor() );
		}
	}

	indexend = this->NumOfMembers();

 	for (int i = 0; i < this->NumOfMembers(); i ++)
 	{
 		this->getMemberAt(i)->mySetPos( this->getMemberAt(i)->pos() );
 	}

	updateLayoutType();

	updateLineUpListAdd( indexbegin, indexend );

	updateGroupLabelState();

	return;
}

void Group::removeItem(ItemBase *item)
{
	int index = this->_members.indexOf( item );
	
	updateLayoutType();
	updateLineUpListRemove( index );
	
	this->_members.removeAt( index );

	updateBoarderState();
	updateGroupLabelState();

	return;
}

void Group::tightenItems()
{
	ItemBase *item;
	QList<ItemBase*> list;

	updateLayoutType();

	list = this->_members;

	for (int i = 0; i < this->NumOfMembers(); i ++)
	{
		item = this->getMemberAt(i);
		if (this->layoutType == LINEUP)
		{
			//item->adjustPosLineUp();
		}
		else if (this->layoutType == RANDOM)
		{
			item->tighten(list);
		}
	}
	return;
}

void Group::disperseItems()
{
	ItemBase *item;
	QList<ItemBase*> list;

	updateLayoutType();

	list = this->_members;

	for (int i = 0; i < this->NumOfMembers(); i ++)
	{
		item = this->getMemberAt(i);
		if (this->layoutType == LINEUP)
		{
			item->adjustPosLineUp();
		}
		else if (this->layoutType == RANDOM)
		{
			item->disperse(list);
		}	
	}
	return;
}

void Group::updateGroupLabelState()
{
	if (this->NumOfMembers() == 0)
	{
		return;
	}
	ItemBase *item,*hasLabel;
	ItemLabel *label;
	QList<ItemBase*> list;

	list = this->_members;

	hasLabel = this->getMemberAt( 0 );

	for (int i = 1; i < this->NumOfMembers(); i ++)
	{
		item = this->getMemberAt( i );
	
		if ((item->pos().x() < hasLabel->pos().x()) || 
			(item->pos().x() == hasLabel->pos().x() && item->pos().y() < hasLabel->pos().y()))
		{
			label = hasLabel->getLabel();
			if (label && label->isVisible())
			{
				label->hide();
			}
			hasLabel = item;
		}
		else
		{
			label = item->getLabel();
			if (label && label->isVisible())
			{
				label->hide();
			}
		}
	}

	label = hasLabel->getLabel();
	if (label && (!label->isVisible()))
	{
		label->show();
	}
}

void Group::setBatGroupFrameState(float rate, int number )
{
	QList<ItemBase*> list;
	ItemBase *item;
	QtBat *bat;

	list = this->_members;
	for (int i = 0; i < this->NumOfMembers(); i ++)
	{
		item = this->getMemberAt( i );
		if (item && item->getType() == BAT)
		{
			bat = dynamic_cast<QtBat*>(item);
			bat->setFrame( rate, number );
		}
	}
}

void Group::setBatGroupSurfaceState(bool state)
{
	QList<ItemBase*> list;
	ItemBase *item;
	QtBat *bat;

	list = this->_members;
	for (int i = 0; i < this->NumOfMembers(); i ++)
	{
		item = this->getMemberAt( i );
		if (item && item->getType() == BAT)
		{
			bat = dynamic_cast<QtBat*>(item);
			bat->setSurface( state );
		}
	}
}

void Group::setBatGroupWireframeState( bool state )
{
	QList<ItemBase*> list;
	ItemBase *item;
	QtBat *bat;

	list = this->_members;
	for (int i = 0; i < this->NumOfMembers(); i ++)
	{
		item = this->getMemberAt( i );
		if (item && item->getType() == BAT)
		{
			bat = dynamic_cast<QtBat*>(item);
			bat->setWireframe( state );
		}
	}
}

void Group::setBatGroupCamberState(int camberindex, bool state)
{
	QList<ItemBase*> list;
	ItemBase *item;
	QtBat *bat;

	list = this->_members;
	for (int i = 0; i < this->NumOfMembers(); i ++)
	{
		item = this->getMemberAt( i );
		if (item && item->getType() == BAT)
		{
			bat = dynamic_cast<QtBat*>(item);
			bat->setCamberState( camberindex, state );
		}
	}
}

void Group::setBatGroupSymbolState( bool state )
{
	QList<ItemBase*> list;
	ItemBase *item;
	QtBat *bat;

	list = this->_members;
	for (int i = 0; i < this->NumOfMembers(); i ++)
	{
		item = this->getMemberAt( i );
		if (item && item->getType() == BAT)
		{
			bat = dynamic_cast<QtBat*>(item);
			bat->setSymbol( state );
		}
	}
}

void Group::updateBatGroupWingShapePara( ArrowGen* agen )
{
	QList<ItemBase*> list;
	ItemBase *item;
	QtBat *bat;

	list = this->_members;
	for (int i = 0; i < this->NumOfMembers(); i ++)
	{
		item = this->getMemberAt( i );
		if (item && item->getType() == BAT)
		{
			bat = dynamic_cast<QtBat*>(item);
			bat->updateWingShapePara( agen );
		}
	}
}

void Group::updateGroupRotation( QPointF offset )
{
	QList<ItemBase*> list;
	ItemBase *item;
	QtBat *bat;

	list = this->_members;
	for (int i = 0; i < this->NumOfMembers(); i ++)
	{
		item = this->getMemberAt( i );
		if ( item && item->getType() == BAT )
		{
			bat = dynamic_cast<QtBat*>(item);
			bat->acceptRotation( offset );
		}
	}
}

void Group::updateGroupPCColorState( int input )
{
	QList<ItemBase*> list;
	ItemBase *item;
	QtSM *pcoor;

	list = this->_members;
	for (int i = 0; i < this->NumOfMembers(); i ++)
	{
		item = this->getMemberAt( i );
		if ( item && item->getType() == PC )
		{
			pcoor = dynamic_cast<QtSM*>(item);
			pcoor->getSM()->setColorState( input );
		}
	}
}

void Group::swapPCBars( int source, int target )
{
	QList<ItemBase*> list;
	ItemBase *item;
	QtSM *pcoor;

	list = this->_members;
	for (int i = 0; i < this->NumOfMembers(); i ++)
	{
		item = this->getMemberAt( i );
		if ( item && item->getType() == PC )
		{
			pcoor = dynamic_cast<QtSM*>(item);
			pcoor->needSwapBars( source, target );
		}
	}
}

void Group::setPCHighlightState( bool state )
{
	QList<ItemBase*> list;
	ItemBase *item;
	QtSM *pcoor;

	list = this->_members;
	for (int i = 0; i < this->NumOfMembers(); i ++)
	{
		item = this->getMemberAt( i );
		if ( item && item->getType() == PC )
		{
			pcoor = dynamic_cast<QtSM*>(item);
			pcoor->setHightlightState( state );
		}
	}
}

void Group::pressHightlight( QRect rect )
{
	QList<ItemBase*> list;
	ItemBase *item;
	QtSM *pcoor;

	list = this->_members;
	for (int i = 0; i < this->NumOfMembers(); i ++)
	{
		item = this->getMemberAt( i );
		if ( item && item->getType() == PC )
		{
			pcoor = dynamic_cast<QtSM*>(item);
			pcoor->pressHightLight( rect );
		}
	}
}

void Group::moveHightlight( QRect rect )
{
	QList<ItemBase*> list;
	ItemBase *item;
	QtSM *pcoor;

	list = this->_members;
	for (int i = 0; i < this->NumOfMembers(); i ++)
	{
		item = this->getMemberAt( i );
		if ( item && item->getType() == PC )
		{
			pcoor = dynamic_cast<QtSM*>(item);
			pcoor->moveHightLight( rect );
		}
	}
}

void Group::releaseHighlight()
{
	QList<ItemBase*> list;
	ItemBase *item;
	QtSM *pcoor;

	list = this->_members;
	for (int i = 0; i < this->NumOfMembers(); i ++)
	{
		item = this->getMemberAt( i );
		if ( item && item->getType() == PC )
		{
			pcoor = dynamic_cast<QtSM*>(item);
			pcoor->releaseHighLight();
		}
	}
}

void Group::insertIntoLineUpList( int index )
{
	int ti, tj, esize, esize2, inLi, inLj, i, j;
	QPointF firstPos, nowPos, tmpPos;

	esize = this->_members[0]->Width();
	esize2 = this->_members[0]->Height();

	firstPos = this->_members[lineUpList[0][0]]->pos();
	nowPos = this->_members[index]->pos();

	tj = (int)((nowPos.x() - firstPos.x()) / (esize+10));
	ti = (int)((nowPos.y() - firstPos.y()) / (esize2+10));

	tmpPos.setX( firstPos.x() + esize2*tj );
	tmpPos.setY( firstPos.y() + esize*ti );

	if ( nowPos.y() < tmpPos.y() - esize2/2 )
	{
		inLi = -1;
	}
	else if ( nowPos.y() > tmpPos.y() + esize2/2 )
	{
		inLi = 1;
	}
	else
	{
		inLi = 0;
	}

	if ( nowPos.x() < tmpPos.x() - esize/2 )
	{
		inLj = -1;
	}
	else if ( nowPos.x() > tmpPos.x() + esize/2 )
	{
		inLj = 1;
	}
	else
	{
		inLj = 0;
	}

	if ( ti + inLi == -1 )
	{
		QList<int> tmp;
		tmp.append( index );
		lineUpList.insert( 0, tmp );
		this->getMemberAt(index)->setTargetPos( QPointF(firstPos.x(), tmpPos.y()-esize2 ) );
	}
	else if ( ti+inLi == lineUpList.size() )
	{
		QList<int> tmp;
		tmp.append( index );
		lineUpList.append( tmp );
		this->getMemberAt(index)->setTargetPos( QPointF(firstPos.x(), tmpPos.y()+esize2 ) );
	}
	else
	{
		if ( inLj + tj < 0 )
		{
			QPointF ppos = this->getMemberAt( lineUpList[inLi+ti][0] )->pos();
			this->lineUpList[inLi+ti].insert( 0, index );
			this->getMemberAt(index)->setTargetPos( QPointF( ppos.x()-esize, tmpPos.y()+inLi*esize2 ) );
		}
		else if ( inLj + tj >= this->lineUpList[inLi + ti].size())
		{
			QPointF ppos = this->getMemberAt( lineUpList[inLi+ti][this->lineUpList[inLi + ti].size()-1] )->pos();
			this->lineUpList[inLi+ti].append( index );
			this->getMemberAt(index)->setTargetPos( QPointF( ppos.x()+esize, tmpPos.y()+inLi*esize2 ) );
		}
		else
		{
			if (inLj < 0)
			{
				lineUpList[inLi+ti].insert( tj, index );

				for (i = 0; i < tj; i ++)
				{
					int tindex = lineUpList[inLi+ti][i];
					QPointF ppos = this->getMemberAt(tindex)->pos();
					this->getMemberAt(tindex)->setTargetPos( QPointF( ppos.x()-esize, ppos.y() ) );
				}
				this->getMemberAt(index)->setTargetPos( QPointF(tmpPos.x()+inLj*esize, tmpPos.y()+inLi*esize2 ) );
			}
			else
			{
				this->lineUpList[inLi + ti].insert( inLj+tj, index );
				for (i = inLj+tj+1; i < lineUpList[inLi+ti].size(); i ++)
				{
					int tindex = lineUpList[inLi+ti][i];
					QPointF ppos = this->getMemberAt(tindex)->pos();
					this->getMemberAt(tindex)->setTargetPos( QPointF( ppos.x()+esize, ppos.y() ) );
				}
				this->getMemberAt(index)->setTargetPos( QPointF(tmpPos.x()+inLj*esize, tmpPos.y()+inLi*esize2 ) );
			}
		}
	}
}

void Group::updateLineUpListAdd( int ibegin, int iend )
{
	if (this->layoutType != LINEUP)
	{
		return;
	}
	int st, i;

	if (this->lineUpList.size() == 0)
	{
		QList<int> tmp;
		tmp.append( ibegin );
		this->lineUpList.append( tmp );
		st = ibegin+1;
	}
	else
	{
		st = ibegin;
	}

	for ( i = st; i < iend; i ++ )
	{
		insertIntoLineUpList( i );
	}

	updateBoarderState();
}

void Group::updateLineUpListRemove( int index )
{
	if (this->layoutType != LINEUP || NumOfMembers() == 0 )
	{
		return;
	}
	int i, j, k, tindex, esize, esize2;
	QPointF tpos;

	k = -1;
	esize = this->getMemberAt(0)->Width();
	esize2 = this->getMemberAt(0)->Height();

	for (i = 0; i < lineUpList.size(); i ++)
	{
		for (j = 0; j < lineUpList[i].size(); j ++)
		{
			if ( lineUpList[i][j] == index )
			{
				lineUpList[i].removeAt( j );
				k = j;
				break;
			}
		}
		if (k > -1)
		{
			for (j = k; j < lineUpList[i].size(); j ++)
			{
				tindex = lineUpList[i][j];
				tpos = this->getMemberAt(tindex)->pos();

				this->getMemberAt(tindex)->setTargetPos( QPointF( tpos.x() - esize, tpos.y() ) );
			}
			break;
		}
	}

	for (i = 0; i < lineUpList.size(); i ++)
	{
		for (j = 0; j < lineUpList[i].size(); j ++)
		{
			if (lineUpList[i][j] > index)
			{
				lineUpList[i][j] --;
			}
		}
	}
	updateBoarderState();
}

void Group::updateLayoutType()
{
	int num = this->NumOfMembers();
	
	if (num == 0)
	{
		return;
	}

	int size = this->_members[0]->Width();
	int size2 = this->_members[0]->Height();
	this->layoutType = LINEUP;

	for (int i = 1; i < this->NumOfMembers(); i ++)
	{
		if (this->_members[i]->Width() != size && this->_members[i]->Height()!= size2)
		{
			this->layoutType = RANDOM;
			break;
		}
	}

	if (this->layoutType == RANDOM)
	{
		this->lineUpList.clear();
		updateBoarderState();
	}
	else if (this->layoutType == LINEUP)
	{
		if (this->lineUpList.size() == 0)
		{
			updateLineUpListAdd(0, this->NumOfMembers());
		}
	}
}

void Group::updateBoarderState()
{
	int tindex;
	ItemBase* item;

	if ( this->layoutType == LINEUP )
	{
		for (int i = 0; i < lineUpList.size(); i ++)
		{
			for (int j = 0; j < lineUpList[i].size(); j ++)
			{
				tindex = lineUpList[i][j];
				item = this->getMemberAt(tindex);
				if ( i == 0 )
				{
					item->setBoarderUpFlag( true );
				}
				else
				{
					if ( j >= lineUpList[i-1].size() )
					{
						item->setBoarderUpFlag( true );
					}
					else
					{
						item->setBoarderUpFlag( false );
					}
				}

				if ( i== lineUpList.size()-1 )
				{
					item->setBoarderDownFlag( true );
				}
				else
				{
					if ( j >= lineUpList[i+1].size() )
					{
						item->setBoarderDownFlag( true );
					}
					else
					{
						item->setBoarderDownFlag( false );
					}
				}

				if ( j == 0 )
				{
					item->setBoarderLeftFlag( true );
				}
				else
				{
					item->setBoarderLeftFlag( false );
				}

				if ( j == lineUpList[i].size()-1 )
				{
					item->setBoarderRightFlag( true );
				}
				else
				{
					item->setBoarderRightFlag( false );
				}
			}
		}
	}
	else if (this->layoutType == RANDOM)
	{
		for (int i = 0; i < NumOfMembers(); i ++)
		{
			item = this->getMemberAt( i );

			item->setBoarderDownFlag( true );
			item->setBoarderLeftFlag( true );
			item->setBoarderRightFlag( true );
			item->setBoarderUpFlag( true );
		}
	}
}

void Group::drawGroupBoarder(QPainter *painter)
{
	if( NumOfMembers() == 0)
		return;

	ItemBase* item;
	QRegion reg, tmpreg;
	int dx, dy;

	for ( int i = 0; i < NumOfMembers(); i ++ )
	{
		item = this->getMemberAt(i);
		tmpreg = item->AllRegion();
		dx = item->pos().x();
		dy = item->pos().y();
		reg += tmpreg.translated( dx, dy );
	}

	for ( int i = 0; i < NumOfMembers(); i ++ )
	{
		item = this->getMemberAt(i);
		tmpreg = item->roundRect(item->realRect(), 20);
		dx = item->pos().x();
		dy = item->pos().y();
		reg -= tmpreg.translated( dx, dy );
	}

 	QPainterPath path;
 	path.addRegion( reg );
 	item = this->getMemberAt(0);
 	painter->fillPath( path, QBrush( QColor( item->getColor().a, item->getColor().b, item->getColor().c, 100 ) ) );
}

QtBat* Group::getBatLinkWithPC( QtSM* item )
{
	ItemBase* node;
	QtBat* bat;

	int pos;

	for (int i = 0; i < NumOfMembers(); i ++)
	{
		node = this->getMemberAt( i );

		if ( !node )
		{
			continue;
		}

		if( node->getType() == BAT )
		{
			bat = dynamic_cast<QtBat*>( node );
			int pos = item->getSM()->getName().find( bat->getBat()->getname() );
			if (pos >= 0)
			{
				return bat;
			}
		}
	}

	return NULL;
}

QtBat* Group::getBatLinkWithVideo( VideoBlock* item )
{
	ItemBase* node;
	QtBat* bat;

	int pos;

	for (int i = 0; i < NumOfMembers(); i ++)
	{
		node = this->getMemberAt( i );

		if ( !node )
		{
			continue;
		}

		if( node->getType() == BAT )
		{
			bat = dynamic_cast<QtBat*>( node );
			int pos = item->getPlayerWidget()->getName().toStdString().find( bat->getBat()->getname() );
			if (pos >= 0)
			{
				return bat;
			}
		}
	}

	return NULL;
}