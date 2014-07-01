#include "BubbleNode.h"
#include <QFontMetricsF>

BubbleNode::BubbleNode( Infor type, BubbleNode* parent )
: _HIGHLIGHT( false )
, _DRAG( false )
, _DRAGREADY( false )
, _font( "times", 20 - type.Name.size() )
, _metric( this->_font )
{
	this->_infor = type;
	_parent = parent;
}

void BubbleNode::printNode( std::ofstream* file, int space )
{
	for( int i = 0; i < space; i++ )
	{
		(*file)<<"    ";
	}
	(*file)<< this->_infor.Name.toStdString()<<std::endl;
	for( int i = 0; i < this->_child.size(); i++ )
	{
		this->_child[i]->printNode( file, space + 1 );
	}
	return;
}

bool BubbleNode::formalizeRect( QRectF &bounding, QRectF &max )
{
	if( bounding.contains( max ) )
	{
		return false;
	}
	QRectF inter = bounding.intersect( max );
	if( inter.width() == 0 || inter.height() == 0 )
	{
		return false;
	}
	if( bounding.left() < max.left() )
	{
		bounding.setLeft( max.left() );
	}
	if( bounding.right() > max.right() )
	{
		bounding.setRight( max.right() );
	}
	if( bounding.top() < max.top() )
	{
		bounding.setTop( max.top() );
	}
	if( bounding.bottom() > max.bottom() )
	{
		bounding.setBottom( max.bottom() );
	}
	return true;
}

bool BubbleNode::textCanBeDrawn(QRectF boundingRect)
{
	QSizeF size = _metric.size( Qt::TextSingleLine, this->_infor.Name );
	if( size.width() < boundingRect.width() )
	{
		return true;
	}
	else
	{
		return false;
	}
}

void BubbleNode::drawCurrentRect( QPainter* painter, QRectF boundingRect, QRectF max, bool font )
{
	if( !formalizeRect( boundingRect, max ) )
	{
		return;
	}
	if( this->_HIGHLIGHT )
	{
		painter->setBrush( QBrush( QColor( 255, 0, 0, 200 ) ) );
		painter->setPen( QColor( 255, 0, 0, 200 ) );
		painter->drawRect( boundingRect );
		if( _DRAG )
		{
			painter->setPen( QColor( 0, 0, 100, 100 ) );
			painter->setBrush( QBrush( QColor( 0, 0, 100, 100 ) ) );
			painter->drawRect( _transRect );
		}
		//
	}
	else
	{
		painter->setBrush( QBrush( TDHelper::getColorByType( this->_infor.Type ) ) );
		painter->setPen( TDHelper::getColorByType( this->_infor.Type ) );
		painter->drawRect( boundingRect );
	}
	//***************************************************************************
	//QFontMetricsF _metric( this->_font );
	if( this->textCanBeDrawn( boundingRect ) && ! font )
	{
		painter->setPen( QColor( 255, 255, 255, 255 ) );
		painter->setFont( this->_font );
		painter->drawText( boundingRect.bottomLeft(), this->_infor.Name );
	}
	//float maxArea = max.width()*max.height();
	//float curArea = boundingRect.width()*boundingRect.height();
	//if( curArea / maxArea > 0.15 && curArea / maxArea < 0.5 )
	//{
	//}
	return;
}

void BubbleNode::drawChildrenRect( QPainter* painter, QRectF boundingRect, QRectF max )
{
	initBoundingRect( boundingRect );
	for( int i = 0; i < this->_child.size(); i++ )
	{
		this->_child[i]->paint( painter, this->_bounding[i], max );
	}
	return;
}

void BubbleNode::getHighLight( QString &name)
{
	//painter->setFont( QFont( "times", 8 ) );
	if( this->_infor.Control == 3)
	{
		//		root node, should not show any vis infor
		for( int i = 0; i < this->_child.size(); i++ )
		{
			this->_child[i]->getHighLight( name );
		}
	}
	else if( this->_infor.Control == 1 )
	{
		if( this->_HIGHLIGHT )
		{
			name = this->_infor.Name;
		}
	}
	else
	{
		if( this->_HIGHLIGHT )
		{
			name = this->_infor.Name;
		}
		else
		{
			for( int i = 0; i < this->_child.size(); i++ )
			{
				this->_child[i]->getHighLight( name );
			}
		}
	}
	return;
}

void BubbleNode::paint(QPainter *painter, QRectF boundingRect, QRectF max )
{
	_currentRect = boundingRect;
	//painter->setFont( QFont( "times", 8 ) );
	if( this->_infor.Control == 3)
	{
		//		root node, should not show any vis infor
		drawChildrenRect( painter, boundingRect, max );
	}
	else if( this->_infor.Control == 1 )
	{
		drawCurrentRect( painter, boundingRect, max, false );
	}
	else
	{
		this->initBoundingRect( boundingRect );
		drawCurrentRect( painter, boundingRect, max, this->_child.first()->textCanBeDrawn( this->_bounding.first() ) );
		drawChildrenRect( painter, boundingRect, max );
	}
}

void BubbleNode::initBoundingRect( QRectF boundingRect )
{
	this->_bounding.clear();
	int heightN = this->findMinSqur( this->_child.size() );
	int widthN = heightN - ( heightN*heightN - this->_child.size() ) / heightN;
	int width, height;
	if( (boundingRect.width() - MARGIN*2) < 0 )
	{
		width = ( boundingRect.width() ) / widthN;
	}
	else
	{
		width = ( boundingRect.width() - MARGIN*2 ) / widthN;
	}
	if( (boundingRect.height() - MARGIN*2) < 0)
	{
		height = ( boundingRect.height() ) / heightN;
	}
	else
	{
		height = ( boundingRect.height() - MARGIN*2 ) / heightN;
	}
	for( int i = 0; i < this->_child.size(); i++ )
	{
		int x = i % widthN;
		int y = i / widthN;
		this->_bounding.append( QRectF( MARGIN + x * width + boundingRect.topLeft().x(), MARGIN + y * height  + boundingRect.topLeft().y(), width, height ) );
	}
	return;
}

int BubbleNode::findMinSqur(int size)
{
	int result = 1;
	while( pow( result, 2.0 ) < size )
	{
		result += 1;
	}
	return result;
}

bool BubbleNode::satisfyNode(BubbleInstance *instance)
{
	if( this->_infor.Control != 3 )
	{
		bool current = false;
		if( this->_infor.Name == instance->getDiscreption( this->_infor.Type ) )
		{
			current = true;
		}
		bool previous = this->_parent->satisfyNode( instance );
		if( current && previous )
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return true;
	}
}

void BubbleNode::highLight( POS &even )
{
	this->_HIGHLIGHT = false;
	for( int i = 0; i < this->_child.size(); i++ )
	{
		this->_child[i]->highLight( even );
		//if( even.ACCEPT == true )
		//{
		//	ACCEPT = true;
		//}
	}
	if( even.ACCEPT == true )
	{
		return;
	}
	QPointF pos = even.pos;
	if( even.ACCEPT == false && this->_currentRect.contains( pos ) )
	{
		even.ACCEPT = true;
		this->_HIGHLIGHT = true;
	}
	return;
}

void BubbleNode::drawInitViewPos( QPointF pos )
{
	for( int i = 0; i < this->_child.size(); i++ )
	{
		this->_child[i]->drawInitViewPos( pos );
	}
	if( this->_HIGHLIGHT )
	{
		 _transRect = this->_currentRect;
		 _transRect.translate( pos );
		 //_DRAGREADY = true;
		return;
	}
	return;
}

void BubbleNode::enableDrag()
{
	this->_DRAG = true;
	for( int i = 0; i < this->_child.size(); i++ )
	{
		this->_child[i]->enableDrag();
	}
	return;
}

void BubbleNode::disableDrag()
{
	this->_DRAG = false;
	for( int i = 0; i < this->_child.size(); i++ )
	{
		this->_child[i]->disableDrag();
	}
	_transRect = _currentRect;
	return;
}