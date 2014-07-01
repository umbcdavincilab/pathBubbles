#include "PenBlock.h"
#include "openglscene.h"
#include "OpenManager.h"
#include "PenText.h"
#include "PenSlider.h"

PenBlock::PenBlock( int x, int y, OpenGLScene* scene, OpenManager* openmanager )
: ItemBase( PENBLOCK, x, y )
, VOLUMED( false )
, _ONEXISTED( false )
, _ONCREATING( false )
, currentIndex( -1, -1 )
, _ONACTION( false )
, _ONADDPOINT( false )
, _PreviousSize( PENBLOCK )
, _run( QString( QObject::tr("icon/run.png") ) )
, _rectRun( -PENBLOCK/2, PENBLOCK/2 - 30, 30, 30 )
, _ONRUN( false )
, _NEEDNEW( false )
, _LABEL( false )
, GLOBAL_INDEX( -1 )
{
	this->_scene = scene;
	this->_TYPE = PEN;
	this->bIndex = scene->getBubbleIndex();
	this->_openManager = openmanager;
	initEngin();

	PenText* text = new PenText();
	text->setText("      ");
	_curLine.append( text );

	this->_lines.append( _curLine );

	_label = NULL;

	return;
}

PenBlock::~PenBlock()
{
	for( int i = 0; i < this->_lines.size(); i++ )
	{
		for( int j = 0; j < this->_lines[i].size(); j++ )
		{
			delete this->_lines[i][j];
		}
	}
	this->_lines.clear();
	this->_curLine.clear();
	delete _engin;
	return;
}

void PenBlock::logOpen()
{
	ItemBase::logOpen();

	emit logInfor("\n");
}

void PenBlock::logSearch()
{
	QString logtext;

	logtext.append( headofLogMsg() );
	logtext.append( "Run script for searching" );
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void PenBlock::initEngin()
{
	this->_engin = new ScriptEngin( this->_openManager );
	connect( _engin, SIGNAL(needSlider( QList<QList<ItemBase*>>, QList<QList<float>> )), this, SLOT(needSlider( QList<QList<ItemBase*>>, QList<QList<float>>)) );
	return;
}

void PenBlock::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	//updateLabel();
	if( _PreviousSize != this->Width() )
	{
		_rectRun = QRectF( -this->Width()/2, this->Height()/2 - 30, 30, 30 );
		reOrderItems();
		_PreviousSize = this->Width();
	}
	drawRun( painter );
	paintDrawing( painter );
	ItemBase::paint( painter, option, widget );
	return;
}

 void PenBlock::updateLabel()
 {
 	if (_LABEL == false)
 	{
 		_label = new ItemLabel( this, _scene, m_size_width, m_size_height );
 		_label->setLabelColor(_colorBoarder);
 		this->_scene->addItem(_label);
 		_LABEL = true;

		emit updateLabelState( this );

 		return;
 	}
 	_label->updateLabel( m_size_width, m_size_height );
 }

void PenBlock::paintDrawing( QPainter *painter )
{
	for( int i = 0; i < this->_lines.size(); i++ )
	{
		for( int j = 0; j < _lines[i].size(); j++ )
		{
			this->_lines[i][j]->paint( painter );
		}
	}
}

void PenBlock::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
	initPos = event->pos();
	if( onBoarder( event->pos() ) )
	{
		_ONACTION = true;
	}
	else if( highLight( event->pos() ) )
	{
		clickItem();
		return;
	}
	else
	{
		_ONCREATING = true;
	}
	ItemBase::mousePressEvent( event );
	return;
}

void PenBlock::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	if( _ONADDPOINT )
	{
		_NEEDNEW = false;
		this->_currentDrawing->finalize();
		reOrderItems();
	}
	else if( initPos == event->pos() && this->_rectRun.contains( event->pos() ) )
	{
		runScript();
		logSearch();
	}
	else if( highLight( event->pos() ) )
	{
		releaseItem();
	}
	_ONADDPOINT = false;
	_ONEXISTED = false;
	_ONCREATING = false;
	_ONACTION = false;
	ItemBase::mouseReleaseEvent( event );
	//
	return;
}

void PenBlock::mouseMoveEvent( QGraphicsSceneMouseEvent *event )
{
	if( _ONCREATING )
	{
		_currentDrawing = new PenDrawing( this->_engin );
		this->_lines.last().append( _currentDrawing );
		this->_currentDrawing->addLine( initPos, event->pos() );
		initPos = event->pos();
		_ONCREATING = false;
		_ONADDPOINT = true;
	}
	else if( _ONADDPOINT )
	{
		this->_currentDrawing->addLine( initPos, event->pos() );
		initPos = event->pos();
	}
	else if( _ONACTION )
	{
		ItemBase::mouseMoveEvent( event );
	}
	else if( highLight( event->pos() ) )
	{
		event->setAccepted( true );
		return;
	}
	//ItemBase::mouseMoveEvent( event );
	return;
}

void PenBlock::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
	highLight( event->pos() );
	_ONRUN = this->_rectRun.contains( event->pos() );
	ItemBase::hoverMoveEvent( event);
	return;
}

bool PenBlock::onExisted(QPointF pos)
{
	//for( int i = 0; i < this->_drawing.size(); i++ )
	//{
	//	if( this->_drawing[i]->contains( pos ) )
	//	{
	//		_EXISTINDEX = i;
	//		return true;
	//	}
	//}
	return false;
}

void PenBlock::clickItem()
{
	this->_lines[currentIndex.x()][currentIndex.y()]->click();
}

void PenBlock::releaseItem()
{
	this->_lines[currentIndex.x()][currentIndex.y()]->release();
}

bool PenBlock::highLight( QPointF pos )
{
	bool state = false;
	currentIndex.setX( -1 );
	currentIndex.setY( -1 );
	for( int i = 0; i < this->_lines.size(); i++ )
	{
		for( int j = 0; j < _lines[i].size(); j++ )
		{
			if( this->_lines[i][j]->mouseOver( pos ) )
			{
				currentIndex.setX( i );
				currentIndex.setY( j );
				state = true;
			}
		}
	}
	return state;
}

void PenBlock::keyPressEvent( QKeyEvent* event )
{
	//if( this->currentIndex != -1 )
	//{
	//	this->_drawing[currentIndex]->addCode( event->key(), event->text() );
	//}
	int key = event->key();
	switch( key )
	{
	case 16777220: // Enter
		handleEnter();
		break;
	case 16777219:	// backspace
		handleBackspace();
		break;
	case 32:
		handleSpace();
		break;
	default:
		handleKeyBoard( event );
	}
	return;
}

void PenBlock::handleSpace()
{
	_NEEDNEW = true;
	return;
}

void PenBlock::handleKeyBoard( QKeyEvent* event )
{
	if( _NEEDNEW )
	{
		PenText* text = new PenText();
		_lines.last().append( text );
		_NEEDNEW = false;
	}
	if( _lines.last().size() == 0 )
	{
		PenText* text = new PenText();
		_lines.last().append( text );
		text->appendText( event->text() );
	}
	else if( _lines.last().last()->getType() == 2 )
	{
		((PenText*)_lines.last().last())->appendText( event->text() );
	}
	else if( _lines.last().last()->getType() == 1 )
	{
		PenText* text = new PenText();
		_lines.last().append( text );
		text->appendText( event->text() );
	}
	//*************************************************************
	reOrderItems();
	return;
}

void PenBlock::handleEnter()
{
	QList<PenItem*> newLine;
	PenText* text = new PenText();
	text->setText("      ");
	newLine.append( text );
	_curLine = newLine;
	this->_lines.append( _curLine );
	return;
}

void PenBlock::handleBackspace()
{
	if( this->_lines.size() == 1 && this->_lines.first().size() == 0 )
	{
		return;
	}
	if( this->_lines.last().size() == 0 )
	{
		this->_lines.removeLast();
	}
	if( this->_lines.last().last()->getType() == 2 )
	{
		bool result = ((PenText*)this->_lines.last().last())->removeText();
		if( !result )
		{
			delete this->_lines.last().last();
			_lines.last().removeLast();
			handleBackspace();
		}
	}
	else if( this->_lines.last().last()->getType() == 0 )
	{
		delete this->_lines.last().last();
		this->_lines.last().removeLast();
	}
	else if( this->_lines.last().last()->getType() == 1 )
	{
		delete this->_lines.last().last();
		_lines.last().removeLast();
	}
	return;
}

void PenBlock::reOrderItems()
{
	//		begin setup the order
	int Height = -this->Height()/2 + 10;
	for( int i = 0; i < this->_lines.size(); i++ )
	{
		int Width = -this->Width()/2;
		int MaxHeight = 0;
		for( int j = 0; j < this->_lines[i].size(); j++ )
		{
			QRectF curBounding = this->_lines[i][j]->boundingRect();
			if( Width + curBounding.width() > this->Width()/2)
			{
				Width = -this->Width()/2;
				Height += MaxHeight;
				MaxHeight = 0;
			}
			this->_lines[i][j]->setPos( QPoint( Width, Height ) );
			if( MaxHeight < curBounding.height() )
			{
				MaxHeight = curBounding.height();
			}
			Width += curBounding.width();
		}
		Height += MaxHeight;
	}
	return;
}

void PenBlock::AcceptDrawing( PenDrawing* drawing, float camber )
{
	if( this->_lines.size() == 0 )
	{
		return;
	}
	drawing->setEngine( this->_engin );
	this->_lines.last().append( drawing );
	drawing->finalize( camber );
	reOrderItems();
	return;
}

void PenBlock::drawRun( QPainter* painter )
{
	if( this->_lines.size() > 0 )
	{
		painter->save();
		//*************************************************************************
		painter->drawImage( _rectRun, this->_run, QRectF( QPointF( 0, 0 ), this->_run.size() ) );
		if( _ONRUN )
		{
			painter->setPen( Qt::NoPen );
			painter->setBrush( QBrush( QColor( 200, 200, 0, 80 ) ) );
			painter->drawEllipse( this->_rectRun );
		}
		//if( this->_MOUSEONRUN )
		//{
		//	painter->setPen( QPen( QBrush( QColor( 200, 200, 0, 100)) , 2 ) );
		//	painter->setBrush( QColor( 200, 200, 0, 100) );
		//	painter->drawRect( _rectRun );
		//}
		//*************************************************************************
		painter->restore();
	}
	return;
}

void PenBlock::runScript()
{
	GLOBAL_INDEX = 0;
	for( int i = 0; i < this->_lines.size(); i++ )
	{
		QString result = this->_engin->runScript( this->_lines[i] );
		GLOBAL_INDEX += 1;
		if( result.length() > 1 )
		{
			QMessageBox msgBox;
			msgBox.setText( result.toStdString().c_str() );
			msgBox.exec();
			break;
		}
	}
}

void PenBlock::needSlider( QList<QList<ItemBase*> > bats, QList<QList<float> > rates )
{
	QPointF off = this->pos() - bats[0][0]->pos() + QPointF( 2*this->Width(),0);
	for( int i = 0; i < bats.size(); i++ )
	{
		for( int j = 0; j < bats[i].size(); j++ )
		{
			bats[i][j]->setPos( bats[i][j]->pos() + off );
		}
	}
	emit addBatsToScene(bats);

	PenSlider* slider = new PenSlider( this->pos() + QPointF( this->Width()/2 + 100, 0 ), bats, rates );
	connect( this, SIGNAL( itemPosUpdated( ItemBase* )), slider, SLOT(posUpdate( ItemBase* )) );
	QList<PenItem*> list;
	list.append( slider );
	_lines.insert( GLOBAL_INDEX+1, list );
	reOrderItems();
	if( GLOBAL_INDEX + 1 == _lines.size() - 1)
	{
		QList<PenItem*> tail;
		this->_lines.append( tail );
	}
	_NEEDNEW = true;
	return;
}