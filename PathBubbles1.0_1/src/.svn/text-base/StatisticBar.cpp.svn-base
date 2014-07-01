#include "StatisticBar.h"

StatisticBar::StatisticBar( QSizeF size, QPoint pos, StatisticManager *manager)
: BarBase( size, pos.x(), pos.y() )
, _curSelectedParameter( 0 )
, _displayDrag( false )
, ONMOVE( false )
{
	setFlag( QGraphicsItem::ItemIsMovable, true);
	setFlag( QGraphicsItem::ItemIsSelectable, true);
	setFlag( QGraphicsItem::ItemIsFocusable, true);

	_control = QRectF(0, 0, size.width(), 20);
	this->_manager = manager;
	connect( manager, SIGNAL(indexAccepted(int)), this, SLOT(indexAccepted(int)) );

	initNameItem();
	return;
}

QString StatisticBar::tailofLogMsg()
{
	QString logtext;

	logtext.append( " (ParameterBar Bubble)\n" );

	return logtext;
}

void StatisticBar::logHide()
{
	QString logtext;

	logtext.append( headofLogMsg() );
	logtext.append( "Hide the Parameter Bar");
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void StatisticBar::logDeleteItem( QString str )
{
	QString logtext;

	logtext.append( headofLogMsg() );
	logtext.append( "Delete the item ");
	logtext.append( str );
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void StatisticBar::initNameItem()
{
	QStringList list = this->_manager->get1DNameList();
	for( int i = 0; i < list.size(); i++ )
	{
		PenText* text = new PenText();
		text->appendText( list[i] );
		_1Ditems.append( text );
	}

	list = this->_manager->get1DTNameList();
	for ( int i = 0; i < list.size(); i++ )
	{
		PenText* text = new PenText();
		text->appendText( list[i] );
		_1DTitems.append( text );
	}

	reOrderItems();
}

void StatisticBar::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	painter->save();
	//*********************************************************
	this->drawWholeRect( painter );
	this->drawControlRect( painter );
	this->drawItems( painter );
	this->drawDrag( painter );
	//*********************************************************
	painter->restore();

	return;
}

void StatisticBar::drawDrag( QPainter* painter )
{
	if( !this->_displayDrag )
	{
		return;
	}
	painter->save();
	//*************************************************
	painter->setPen( Qt::NoPen );
	painter->setBrush( QBrush( QColor( 0, 0, 200, 150 ) ) );
	//*************************************************
	painter->drawRect( _curSelectedRect );
	//*************************************************
	painter->restore();
	return;
}

void StatisticBar::drawWholeRect( QPainter* painter )
{
	painter->save();

	painter->setPen( Qt::NoPen );
	painter->setBrush( QBrush( QColor(141, 160, 203, 200)) );
	painter->drawRect( this->boundingRect() );

	painter->restore();
}

void StatisticBar::drawControlRect( QPainter* painter )
{
	painter->save();

	painter->setPen( Qt::NoPen );
	painter->setBrush( QBrush( QColor(110, 130, 160, 200)) );
	painter->drawRect( this->_control );

	painter->setFont( QFont( "times", 10 ) );
	painter->setPen( QColor(10, 10, 10, 100) );
	painter->drawText(80, 15, "Parameters");

	painter->restore();
}

void StatisticBar::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
	this->mouseMoveOnItems( event->pos() );
	BarBase::hoverMoveEvent( event );
	return;
}

void StatisticBar::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if (_control.contains(event->pos()))
	{
		ONMOVE = true;
	}
	else if( this->_curSelectedParameter != -1 )
	{
		_selectedParameter = _curSelectedParameter;
		if ( this->_1Ditems.size() > 0 
			&& _curSelectedParameter < this->_1Ditems.size() )
		{
			_curSelectedRect = this->_1Ditems[_curSelectedParameter]->boundingRect();
		}
		else if (_1DTitems.size() > 0 
			&& _curSelectedParameter-_1Ditems.size() < this->_1DTitems.size())
		{
			_curSelectedRect = this->_1DTitems[_curSelectedParameter-_1Ditems.size()]->boundingRect();
		}
		else if (_1DitemsFromMat.size() > 0
			&& _curSelectedParameter-_1Ditems.size()-_1DTitems.size() < this->_1DitemsFromMat.size())
		{
			_curSelectedRect = this->_1DitemsFromMat[_curSelectedParameter-_1Ditems.size()-_1DTitems.size()]->boundingRect();
		}
		else if (_1DTitemsFromMat.size() > 0
			&&  _curSelectedParameter-_1Ditems.size()-_1DTitems.size() - _1DitemsFromMat.size() < this->_1DTitemsFromMat.size())
		{
			_curSelectedRect = this->_1DTitemsFromMat[_curSelectedParameter-_1Ditems.size()-_1DTitems.size() - _1DitemsFromMat.size()]->boundingRect();
		}

		_displayDrag = true;
	}
	BarBase::mousePressEvent( event );
	return;
}

void StatisticBar::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if( _displayDrag )
	{
		QPointF offset = event->pos() - event->lastPos();
		this->_curSelectedRect.translate( offset );
		this->update();
	}

	if ( ONMOVE )
	{
		if ( event->scenePos().y() > 50 )
		{
			BarBase::mouseMoveEvent( event );
		}
	}
	return;
}

void StatisticBar::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	if( _displayDrag )
	{
		emit needStatisticalData( event->scenePos(), this->_manager, _selectedParameter );
		_displayDrag = false;

		//logDragItem();
	}
	ONMOVE = false;
	BarBase::mouseReleaseEvent( event );
	return;
}

void StatisticBar::keyPressEvent(QKeyEvent *event)
{
	BarBase::keyPressEvent( event );
	return;
}

void StatisticBar::wheelEvent(QGraphicsSceneWheelEvent *event)
{
	BarBase::wheelEvent( event );
	return;
}

void StatisticBar::mouseMoveOnItems(QPointF pos)
{
	int num;
	for( int i = 0; i < this->_1Ditems.size(); i++ )
	{
		if( this->_1Ditems[i]->mouseOver( pos ) )
		{
			this->_curSelectedParameter = i;
		}
	}
	
	num = this->_1Ditems.size();
	for ( int i = 0; i < this->_1DTitems.size(); i ++ )
	{
		if (this->_1DTitems[i]->mouseOver( pos ) )
		{
			this->_curSelectedParameter = i+num;
		}
	}

	num += this->_1DTitems.size();
	for (int i = 0; i < this->_1DitemsFromMat.size(); i ++)
	{
		if (this->_1DitemsFromMat[i]->mouseOver( pos ) )
		{
			this->_curSelectedParameter = i + num;
		}
	}

	num += this->_1DitemsFromMat.size();
	for (int i = 0; i < this->_1DTitemsFromMat.size(); i ++)
	{
		if (this->_1DTitemsFromMat[i]->mouseOver( pos ) )
		{
			this->_curSelectedParameter = i + num;
		}
	}

	if( this->_curSelectedParameter != -1 )
	{
		return;
	}
	else
	{
		this->_curSelectedParameter = -1;
		return;
	}
}

void StatisticBar::reOrderItems()
{
	int curHeight = 20;

	for( int i = 0; i < this->_1Ditems.size(); i++ )
	{
		QRectF curBounding = this->_1Ditems[i]->boundingRect();
		this->_1Ditems[i]->setPos( QPoint(0, curHeight ) );
		curHeight += curBounding.height()+5;
	}

	curHeight += 11;

	for (int i = 0; i < this->_1DTitems.size(); i++ )
	{
		QRectF curBounding = this->_1DTitems[i]->boundingRect();
		this->_1DTitems[i]->setPos( QPoint(0, curHeight ) );
		curHeight += curBounding.height()+5;
	}

	curHeight += 21;

	for( int i = 0; i < this->_1DitemsFromMat.size(); i++ )
	{
		QRectF curBounding = this->_1DitemsFromMat[i]->boundingRect();
		this->_1DitemsFromMat[i]->setPos( QPoint(0, curHeight ) );
		curHeight += curBounding.height()+5;
	}

	curHeight += 11;

	for (int i = 0; i < this->_1DTitemsFromMat.size(); i++ )
	{
		QRectF curBounding = this->_1DTitemsFromMat[i]->boundingRect();
		this->_1DTitemsFromMat[i]->setPos( QPoint(0, curHeight ) );
		curHeight += curBounding.height()+5;
	}

	if (curHeight == 20)
	{
		curHeight = 50;
	}
	
	resizeItem(QSizeF(this->getSize().width(), curHeight));

	return;
}

void StatisticBar::drawItems(QPainter *painter)
{
	for ( int i = 0; i < this->_1Ditems.size(); i++ )
	{
		this->_1Ditems[i]->paint( painter );
	}

	if (this->_1DTitems.size() > 0)
	{
		double y = this->_1DTitems[0]->getPos().y() - 8;

		for ( int i = 0; i < this->getSize().width()-50; i += 7)
		{
			painter->drawLine( QPointF( i, y ), QPointF( i + 5, y ) );
		}

		for ( int i = 0; i < this->_1DTitems.size(); i++ )
		{
			this->_1DTitems[i]->paint( painter );
		}
	}
	
	if (this->_1DitemsFromMat.size() > 0)
	{
		double y = this->_1DitemsFromMat[0]->getPos().y() - 13;

		for ( int i = 0; i < this->getSize().width(); i += 7)
		{
			painter->drawLine( QPointF( i, y ), QPointF( i + 3, y ) );
		}

		for ( int i = 0; i < this->_1DitemsFromMat.size(); i++ )
		{
			this->_1DTitemsFromMat[i]->paint( painter );
		}
	}
	
	if (this->_1DTitemsFromMat.size() > 0)
	{
		double y = this->_1DTitemsFromMat[0]->getPos().y() - 8;

		for ( int i = 0; i < this->getSize().width()-50; i += 7)
		{
			painter->drawLine( QPointF( i, y ), QPointF( i + 5, y ) );
		}

		for ( int i = 0; i < this->_1DTitemsFromMat.size(); i++ )
		{
			this->_1DTitemsFromMat[i]->paint( painter );
		}
	}

	return;
}

void StatisticBar::updateSize( QSizeF size )
{	
	BarBase::updateSize( size );

	this->reOrderItems();
}

void StatisticBar::indexAccepted( int index )
{
	return;
}

void StatisticBar::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
	QMenu menu;
	QAction *hideAction = menu.addAction("Hide");
	QAction *deleteAction = menu.addAction("Delete");

	QAction *selectedAction = menu.exec( event->screenPos() );

	if ( selectedAction == hideAction )
	{
		this->hide();
		logHide();
	}
	else if ( selectedAction == deleteAction )
	{
		if (_selectedParameter > -1)
		{
			int selected = _selectedParameter;
			
			QString str = this->_manager->getNameByIndex( selected );
			logDeleteItem( str );

			if ( this->_1Ditems.size() > 0 
				&& selected < this->_1Ditems.size() )
			{
				this->_1Ditems.removeAt( selected );
				_manager->removePara( 1, selected );
			}
			else if (_1DTitems.size() > 0 
				&& selected-_1Ditems.size() < this->_1DTitems.size())
			{
				this->_1DTitems.removeAt(selected-_1Ditems.size());
				_manager->removePara( 2, selected-_1Ditems.size() );
			}
			else if (_1DitemsFromMat.size() > 0
				&& selected-_1Ditems.size()-_1DTitems.size() < this->_1DitemsFromMat.size())
			{
				this->_1DitemsFromMat.removeAt( selected-_1Ditems.size()-_1DTitems.size() );
				_manager->removePara( 3,  selected-_1Ditems.size()-_1DTitems.size() );
			}
			else if (_1DTitemsFromMat.size() > 0
				&&  selected-_1Ditems.size()-_1DTitems.size() - _1DitemsFromMat.size() < this->_1DTitemsFromMat.size())
			{
				this->_1DTitemsFromMat.removeAt( selected-_1Ditems.size()-_1DTitems.size() - _1DitemsFromMat.size());
				_manager->removePara( 4, selected-_1Ditems.size()-_1DTitems.size() - _1DitemsFromMat.size() );
			}
		}

		reOrderItems();
	}
}

void StatisticBar::addParameterFromMat( QString pName, QString run_num, QList<double> dataResult )
{
	QString str;

	str = pName;
	str += "(run_num=";
	str += run_num;
	str += ")";

	PenText* text = new PenText();
	text->appendText( str );

	if (dataResult.size() > 1)
	{
		if ( _1DTitemsFromMat.indexOf( text ) == -1 )
		{
			_1DTitemsFromMat.append( text );
			this->_manager->addParaFromMat( str, dataResult );
		}
	}
	else
	{
		if ( _1DitemsFromMat.indexOf( text ) == -1 )
		{
			_1DitemsFromMat.append( text );
		}
	}

	reOrderItems();
}

void StatisticBar::updateDataFromMat( QString pName, QList<double> dataResult )
{
	_manager->updateMatData( pName, dataResult );
}