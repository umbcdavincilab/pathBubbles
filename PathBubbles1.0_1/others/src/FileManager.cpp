#include "FileManager.h"
#include "openglscene.h"

//static const QPointF ControlBound[4] = {
//     QPointF(200, 0),
//     QPointF(400, 0),
//     QPointF(380, 18),
//     QPointF(220, 18)
// };

FileManager::FileManager( QSizeF size, OpenGLScene* scene, OpenManager* open )
: BarBase( size, 10, 110 )
, _FileTrans( 0, 0 )
, _heightPercent( 0 )
, _DRAGFILE( false )
, _Control( size.width()/3, 0, size.width()/3, 18 )
, _ONMOVE( false )
, _HIGHLIGHTCONTROL( false )
, _VISINDEX( -1 )
, _EllipseMove( size.width()/3*2 - 36, 0 , 18, 18)
, _EllipseHide( size.width()/3*2 - 18, 0 , 18, 18)
, _EllipseMap( size.width()/3*2 - 54, 0 , 18, 18)
, _EllipseColor( size.width()/3*2 - 72, 0 , 18, 18)
, SHIFTFILEVIEW( false )
, _DIRECTPREOPEN( false )
, _ONCONTROLTYPE( -1 )
, COLORBAR( false )
, _CurrentFileName( "" )
, _CTRL( false )
{
	setFlag( QGraphicsItem::ItemIsMovable, true);
	setFlag( QGraphicsItem::ItemIsSelectable, true);
	setFlag( QGraphicsItem::ItemIsFocusable, true);
	initViewRect();
	initIcons();
	this->_scene = scene;
	this->_open = open;
	controlName[0] = QString( tr("Sort bats by weight") );
	controlName[1] = QString( tr("Map bats to speed") );
	controlName[2] = QString( tr("Hide") );
	controlName[3] = QString( tr("Color scale") );
	//
	initManager();
	return;
}

QString FileManager::tailofLogMsg()
{
	QString logtext;

	logtext.append( " (Model-Composition-View Bubble)\n" );

	return logtext;
}

void FileManager::logHide()
{
	QString logtext;

	logtext.append( headofLogMsg() );
	logtext.append( "Hide the Open Manager" );
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void FileManager::logOrderByWeight()
{
	QString logtext;

	logtext.append( headofLogMsg() );
	logtext.append( "Order the bubbles by weight" );
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void FileManager::logOrderBySpeed()
{
	QString logtext;

	logtext.append( headofLogMsg() );
	logtext.append( "Order the bubbles by speed" );
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void FileManager::logShowColorBar()
{
	QString logtext;

	logtext.append( headofLogMsg() );
	logtext.append( "Show the Color Bar" );
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void FileManager::logHideColorBar()
{
	QString logtext;

	logtext.append( headofLogMsg() );
	logtext.append( "Hide the Color Bar" );
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void FileManager::logDragFileToCompose( QString filename )
{
	QString logtext;

	logtext.append( headofLogMsg() );
	logtext.append( "Drag the file " );
	logtext.append( filename );
	logtext.append( " to Composition bubble" );
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void FileManager::logDragVisToCompose( int index, int vis )
{
	QString logtext;
	QString tmp;

	tmp = getVisNameByIndex( vis );

	logtext.append( headofLogMsg() );
	logtext.append( "Drag the vis method " );
	logtext.append( tmp );
	logtext.append( " to the ");
	tmp.setNum( index );
	logtext.append( tmp );
	logtext.append( "th item in the Composition bubble" );
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void FileManager::logDragItemtoScene( int index, QPointF pos )
{
	QString logtext;
	QString str, tmp;

	logtext.append( headofLogMsg() );
	logtext.append( "Drag the " );
	str.setNum( index );
	logtext.append( str );
	logtext.append( "th item to " );
	str = "(";
	tmp.setNum( pos.x() );
	str.append( tmp );
	str.append( ", ");
	tmp.setNum( pos.y() );
	str.append( tmp );
	str.append( ")" );
	logtext.append( str );
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void FileManager::logDragFileToScene( QString filename, QPointF pos )
{
	QString logtext;
	QString str, tmp;

	logtext.append( headofLogMsg() );
	logtext.append( "Drag the file " );
	logtext.append( filename );
	logtext.append( "to " );
	str = "(";
	tmp.setNum( pos.x() );
	str.append( tmp );
	str.append( ", ");
	tmp.setNum( pos.y() );
	str.append( tmp );
	str.append( ")" );
	logtext.append( str );
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void FileManager::logDragVisToScene( int vis, QPointF pos )
{
	QString logtext;
	QString str, tmp;

	tmp = getVisNameByIndex( vis );

	logtext.append( headofLogMsg() );
	logtext.append( "Drag the vis method " );
	logtext.append( tmp );
	logtext.append( "to " );
	str = "(";
	tmp.setNum( pos.x() );
	str.append( tmp );
	str.append( ", ");
	tmp.setNum( pos.y() );
	str.append( tmp );
	str.append( ")" );
	logtext.append( str );
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

QString FileManager::getVisNameByIndex( int vis )
{
	switch ( vis )
	{
	case 0:
		return "Geometry";
	case 1:
		return "Video-Batc";
	case 2:
		return "Video-Batg";
	case 3:
		return "Video-Bath";
	case 4:
		return "PC-elbow";
	case 5:
		return "PC-wrist";
	case 6:
		return "PC-MCP3";
	case 7:
		return "BarChart";
	case 8:
		return "LineChart";
	case 9:
		return "Scatter";
	case 10:
		return "Web";
	case 11:
		return "Matlab";
	case 12:
		return "Pen";
	case 13:
		return "Note";
	}
}

void FileManager::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	BarBase::paint( painter, option, widget );
	painter->save();
	//*********************************************************
	drawDropArea( painter );
	drawFileArea( painter );
	drawVisArea( painter );
	renderItem( painter );
	drawControl( painter );
	drawViews( painter );
	drawColorBar( painter );
	//*********************************************************
	painter->restore();
	//*********************************************************
	return;
}

void FileManager::drawDropArea( QPainter *painter )
{
	painter->save();
	//*********************************************************
	painter->setPen( Qt::NoPen );
	painter->setBrush( QBrush( QColor( 0, 0, 60, 60) ) );
	painter->drawRect( this->getDorpArea() );
	//*********************************************************
	painter->restore();
	return;
}

void FileManager::drawFileArea( QPainter *painter )
{
	painter->save();
	//*********************************************************
	this->_manager->renderRoot( painter, scaleArea(), getPaintArea()); 
	//*********************************************************
	painter->restore();
	return;
}

void FileManager::drawVisArea( QPainter *painter )
{
	painter->save();
	//*********************************************************
	//*********************************************************
	painter->restore();
	return;
}

void FileManager::initManager()
{
	this->_manager = new InstanceManager();
	//
	this->_init = new InstanceInitializer();
	this->_init->initInstance( this->_manager );
	//
	this->_manager->initTree();
	return;
}

QRectF FileManager::getPaintArea()
{
	QRectF result( QPointF( 0, 0 ), QSize( this->boundingRect().width()/3, this->boundingRect().height() ) );
	return result;
}

QRectF FileManager::getDorpArea()
{
	return QRectF(QPointF( this->boundingRect().width()/3, 0 ), QSize( this->boundingRect().width()/3, this->boundingRect().height() ) );
}

QRectF FileManager::getVisArea()
{
	return QRectF(QPointF( 2 * this->boundingRect().width()/3, 0 ), QSize( this->boundingRect().width()/3, this->boundingRect().height() ) );
}

QRectF FileManager::scaleArea()
{
	int ScaleFactor = this->getSize().height() * _heightPercent * 2;
	QRectF bounding = this->getPaintArea();
	bounding.setLeft( bounding.left() - ScaleFactor );
	bounding.setRight( bounding.right() + ScaleFactor );
	bounding.setTop( bounding.top() - ScaleFactor );
	bounding.setBottom( bounding.bottom() + ScaleFactor );
	bounding.translate( this->_FileTrans.x(), this->_FileTrans.y() );
	return bounding;
}

bool FileManager::inFileView( QPointF pos )
{
	return this->getPaintArea().contains( pos );
}

bool FileManager::inDropView(QPointF pos)
{
	return this->getDorpArea().contains( pos );
}

bool FileManager::inVisView( QPointF pos )
{
	return this->getVisArea().contains( pos );
}

void FileManager::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
	if (inFileView(event->pos()) || inVisView(event->pos()))
	{
		return;
	}

	QMenu menu;
	QAction *hideAction = menu.addAction("Hide");

	QAction *selectedAction = menu.exec( event->screenPos() );

	if (selectedAction == hideAction)
	{
		this->hide();
		logHide();
	}
}

void FileManager::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if( this->_Control.contains( event->pos() ) )
	{
		if( this->_EllipseHide.contains( event->pos() ) )
		{
			this->hide();
			logHide();
		}
		else if( _EllipseMove.contains( event->pos() ) )
		{
			emit orderByWeight();
			logOrderByWeight();
		}
		else if( _EllipseMap.contains( event->pos() ) )
		{
			emit orderBySpeed();
			logOrderBySpeed();
		}
		else if( this->_EllipseColor.contains( event->pos() ) )
		{
			if( !COLORBAR )
			{
				COLORBAR = true;
				logShowColorBar();
			}
			else
			{
				COLORBAR = false;
				logHideColorBar();
			}
		}
		else
		{
			_ONMOVE = true;
		}
	}
	if( inFileView( event->pos() ) )
	{
		if( event->button() == Qt::RightButton )
		{
			SHIFTFILEVIEW = true;
		}
		else if( !handlePressFileView( event->pos() ) )
		{
			BarBase::mousePressEvent( event );
		}
	}
	else if( inDropView( event->pos() ) )
	{
		if( !(handlePressDropView( event->pos() ) < 0 ) )
		{
			BarBase::mousePressEvent( event );
		}
	}
	else if( inVisView( event->pos() ) )
	{
		if( !handlePressVisView( event->pos() ) )
		{
			BarBase::mousePressEvent( event );
		}
	}
	BarBase::mousePressEvent( event );

	if (_CTRL)	
		this->_open->setNewFlag(false);
	else	
		this->_open->setNewFlag(true);
}

void FileManager::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if( _DIRECTPREOPEN == true && this->boundingRect().contains( event->pos() ) )
	{
		_DIRECTPREOPEN = false;
	}

	if( _ONMOVE )
	{
		if( event->scenePos().y() > 50 )
		{
			BarBase::mouseMoveEvent( event );
		}
	}

	if( SHIFTFILEVIEW )
	{
		this->_FileTrans += event->pos() - event->lastPos();
	}
	else if( CLICKINDEX >= 0 )
	{
		handleDragInDrop( event->pos() );
	}
	else if( _VISINDEX == -1 )
	{
		handleMoveFileView( event->pos() );
	}
	else if( inDropView( event->pos() ) || inVisView( event->pos() ) )
	{
		handleMoveVisView( event->pos() ) ;
	}
	else if( _VISINDEX > 6 )
	{
		drawPreOpenViewDirectly( event->pos() );
		_DIRECTPREOPEN = true;
	}
	else
	{
		handleMoveVisView( event->pos() );
		_DIRECTPREOPEN = true;
	}

	if (_CTRL == false)	this->_open->setNewFlag(true);
}

void FileManager::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	if( CLICKINDEX >= 0 )
	{
		QPointF pos = event->scenePos();
		this->_lines[CLICKINDEX]->disablePreOpen( pos, this->boundingRect().contains( event->pos() ) );
		
		logDragItemtoScene( CLICKINDEX+1, pos );

		CLICKINDEX = -1;
	}
	if( _DIRECTPREOPEN )
	{
		if (_CurrentFileName != NULL)
		{
			this->OpenFileDirectly( event->scenePos() );

			logDragFileToScene( _CurrentFileName, event->scenePos() );
		}
		else if (_VISINDEX > -1)
		{
			if (_VISINDEX > 6)
			{
				this->openViewDirectly( event->scenePos() );
			}
			else
			{
				this->changeView( event->scenePos(), _VISINDEX );
			}

			logDragVisToScene( _VISINDEX, event->scenePos() );
		}

		_DIRECTPREOPEN = false;
	}
	if( inFileView( event->pos() ) || inDropView( event->pos() ) )
	{
		if( !handleReleaseFileView( event->pos() ) )
		{
		}
	}
	if( ( inDropView( event->pos() )|| inVisView( event->pos() ) ) && _VISINDEX > -1 )
	{
		if( !handleReleaseVisView( event->pos() ) )
		{
		}
	}
	_ONMOVE = false;
	_VISINDEX = -1;
	_CurrentFileName = "";
	BarBase::mouseReleaseEvent( event );
	SHIFTFILEVIEW = false;
}

void FileManager::keyPressEvent(QKeyEvent *event)
{
	BarBase::keyPressEvent( event );

	_CTRL = event->modifiers().testFlag(Qt::ControlModifier);
}

void FileManager::keyReleaseEvent(QKeyEvent *event)
{
	BarBase::keyReleaseEvent( event );

	_CTRL = event->modifiers().testFlag(Qt::ControlModifier);
}

void FileManager::wheelEvent(QGraphicsSceneWheelEvent *event)
{
	if( event->delta() > 0 )
	{
		_heightPercent += 0.05;
	}
	else
	{
		_heightPercent -= 0.05;
	}
	BarBase::wheelEvent( event );
}

void FileManager::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
	if( inFileView( event->pos() ) )
	{
		if( !handleHoverFileView( event->pos() ) )
		{
			BarBase::hoverMoveEvent( event );
		}
	}
	else if( inDropView( event->pos() ) )
	{
		if( !handleHoverDropView( event->pos() ) )
		{
			BarBase::hoverMoveEvent( event );
		}
	}
	else if( inVisView( event->pos() ) )
	{
		if( !handleHoverVisView( event->pos() ) )
		{
			BarBase::hoverMoveEvent( event );
		}
	}
	else
	{
		BarBase::hoverMoveEvent( event );
	}
}

bool FileManager::handleHoverDropView(QPointF pos)
{
	//		highlight item
	for( int i = 0; i < this->_lines.size(); i++ )
	{
		this->_lines[i]->mouseOver( pos );
	}
	if( _Control.contains( pos ) )
	{
		if( _EllipseMap.contains( pos ) )
		{
			_ONCONTROLTYPE = 0;
		}
		else if( _EllipseMove.contains( pos ) )
		{
			_ONCONTROLTYPE = 1;
		}
		else if( _EllipseHide.contains( pos ) )
		{
			_ONCONTROLTYPE = 2;
		}
		else if( _EllipseColor.contains( pos ) )
		{
			_ONCONTROLTYPE = 3;
		}
		else
		{
			_ONCONTROLTYPE = -1;
		}
		_HIGHLIGHTCONTROL = true;
	}
	else
	{
		_HIGHLIGHTCONTROL = false;
	}
	return false;
}

bool FileManager::handleHoverFileView(QPointF pos)
{
	this->_manager->highLightInstance( pos );
	return false;
}

bool FileManager::handleHoverVisView(QPointF pos)
{
	return false;
}

bool FileManager::handlePressFileView(QPointF pos )
{
	_DRAGFILE = true;
	_initDragPos = pos;
	this->_manager->getRoot()->enableDrag();

	_CurrentFileName = "";
	this->_manager->getHighLighted(_CurrentFileName);

	return true;
}

bool FileManager::handleMoveFileView( QPointF pos )
{
	if( inVisView( pos ) )
	{
		return false;
	}

	this->_manager->getRoot()->drawInitViewPos( pos - _initDragPos );

	if (_DIRECTPREOPEN== false && !this->boundingRect().contains( pos ))
	{
		_DIRECTPREOPEN = true;
	}

	return true;
}

bool FileManager::handleReleaseFileView(QPointF pos)
{
	if( _DRAGFILE )
	{
		if( inDropView( pos ) )
		{
			addNewToDrag();
		}
	}
	_DRAGFILE = false;
	this->_manager->getRoot()->disableDrag();
	return true;
}

void FileManager::renderItem( QPainter* painter )
{
	for( int i = 0; i < this->_lines.size(); i++ )
	{
		this->_lines[i]->paint( painter );
	}
}

void FileManager::addNewToDrag()
{
	//			add to the drop area
	if( this->_lines.size() > 7 )
	{
		delete this->_lines.first();
		this->_lines.removeAt( 0 );
	}
	FileItem* item = new FileItem( _CurrentFileName, 1, _open );
	this->_lines.append( item );
	//

	logDragFileToCompose( _CurrentFileName );

	reOrder();
	return;
}

void FileManager::reOrder()
{
	int Height = 20;
	int Width = this->boundingRect().width()/3+10;
	for( int i = 0; i < this->_lines.size(); i++ )
	{
		QRectF curBounding = this->_lines[i]->boundingRect();
		if( Width + curBounding.width() > this->boundingRect().width()/3*2 )
		{
			Width = this->boundingRect().width()/3+10;
			Height += curBounding.height() + 5;
		}
		this->_lines[i]->setPos( QPoint( Width, Height ) );
		Width += curBounding.width() + 3;
	}
}

int FileManager::handlePressDropView( QPointF pos )
{
	CLICKINDEX = -1;
	for( int i = 0; i < this->_lines.size(); i++ )
	{
		if (this->_lines[i]->contains( pos ) )
		{
			CLICKINDEX = i;
			this->_lines[i]->enablePreOpen();
			break;
		}
	}
	return CLICKINDEX;
}

void FileManager::handleDragInDrop(QPointF pos)
{
	if( this->boundingRect().contains( pos ) )
	{
		this->_lines[CLICKINDEX]->drawPreOpen( pos, false );
	}
	else
	{
		QPointF ScenePos = this->boundingRect().topLeft() + pos;
		this->_lines[CLICKINDEX]->drawPreOpen( ScenePos, true );
	}
	return;
}

void FileManager::drawControl( QPainter* painter )
{
	painter->save();
	//************************************
	painter->setPen( Qt::NoPen );
	if( !_HIGHLIGHTCONTROL )
	{
		painter->setBrush( QBrush( QColor( 10, 10, 60, 100 ) ) );
	}
	else
	{
		painter->setBrush( QBrush( QColor( 10, 10, 60, 200 ) ) );
	}
	painter->drawRect( _Control );
	painter->setPen( Qt::RoundJoin );
	painter->setBrush( QBrush( QColor( 150, 150, 150, 100 ) ) );
	painter->setPen( QColor( 150, 150, 150, 100 ) );
	painter->drawRect( _EllipseMap );
	painter->drawRect( _EllipseColor );
	painter->drawRect( _EllipseMove );
	painter->setPen( QColor( 250, 250, 250, 200 ) );
	painter->drawText( this->_EllipseMap.bottomLeft() + QPointF( 5, -5 ), "W" );
	painter->drawText( this->_EllipseMove.bottomLeft() + QPointF( 5, -5 ), "S" );
	painter->drawText( this->_EllipseColor.bottomLeft() + QPointF( 5, -5 ), "C" );
	painter->drawText( this->_EllipseHide.bottomLeft() + QPointF( 5, -5 ), "H" );
	painter->setBrush( QBrush( QColor( 200, 10, 10, 100 ) ) );
	painter->setPen( QColor( 200, 10, 10, 100 ) );
	painter->drawRect( _EllipseHide );
	//************************************
	if( _ONCONTROLTYPE > -1 )
	{
		painter->setPen( QColor( 255, 255, 255, 255 ) );
		painter->drawText( this->_Control.bottomLeft()+QPointF(5, -3), controlName[_ONCONTROLTYPE] );
	}
	painter->restore();
	return;
}

void FileManager::initViewRect()
{
	//************************************************
	this->_view.append( tr("3D") );
	this->_colors.append( QColor( 10,10,150,250) );
	this->_view.append( tr("Video1") );
	this->_colors.append( QColor( 120,20,80,250) );
	this->_view.append( tr("Video2") );
	this->_colors.append( QColor( 120,20,80,250) );
	this->_view.append( tr("Video3") );
	this->_colors.append( QColor( 120,20,80,250) );
	this->_view.append( tr("PC1") );
	this->_colors.append( QColor( 20,160,20,250) );
	this->_view.append( tr("PC2") );
	this->_colors.append( QColor( 20,160,20,250) );
	this->_view.append( tr("PC3") );
	this->_colors.append( QColor( 20,160,20,250) );
	this->_view.append( tr("barChart") );
	this->_colors.append( QColor( 80,160,160,250) );
	this->_view.append( tr("lineChart") );
	this->_colors.append( QColor( 80,160,160,250) );
	this->_view.append( tr("scatterPlot"));
	this->_colors.append( QColor( 80,160,160,250) );
	this->_view.append( tr("Web"));
	this->_colors.append( QColor( 40,20,80,250) );
	this->_view.append( tr("Matlab"));
	this->_colors.append( QColor( 40,20,80,250) );
	this->_view.append( tr("Pen"));
	this->_colors.append( QColor( 40,20,80,250) );
	this->_view.append( tr("Note"));
	this->_colors.append( QColor( 40,20,80,250) );
	
	//************************************************
	int x_step = 40;
	int y_step = 60;
	int width_step = 35;
	int height_step = 50;
	int Num_Item = 5;
	//************************************************
	int initX = this->getSize().width() / 3 * 2;
	for( int i = 0; i < this->_view.size(); i++ )
	{
		this->_rectsView.append( QRectF( i%Num_Item*x_step + initX + 5, i/Num_Item*y_step + 5, width_step, height_step ) );
		this->_shadow.append( QRectF( i%Num_Item*x_step + initX + 7, i/Num_Item*y_step + 7, width_step, height_step ) );
		this->_iconArea.append( QRectF( i%Num_Item*x_step + initX + 5, i/Num_Item*y_step + 10, width_step, height_step-10 ) );
	}
	return;
}

void FileManager::drawViews(QPainter *painter)
{
	painter->save();
	//**************************************
	painter->setPen( Qt::NoPen );
	for( int i = 0; i < this->_rectsView.size(); i++ )
	{
		painter->setBrush( QColor( 20, 20, 20, 200) );
		painter->drawRect( this->_shadow[i] );
		painter->setBrush( this->_colors[i] );
		painter->drawRect( this->_rectsView[i] );
		painter->drawImage( _iconArea[i], this->_icons[i], _iconRects[i] );
	}
	//**************************************
	if( _VISINDEX >= 0 )
	{
		if( _DIRECTPREOPEN && _VISINDEX > 6)
		{
			painter->setBrush( this->_colors[_VISINDEX] );
			painter->drawRect( _preOpenViewDirectly );
		}
		else
		{
			painter->setBrush( this->_colors[_VISINDEX] );
			painter->drawRect( _visDragRect );
		}
	}
	//**************************************
	painter->restore();
	return;
}

bool FileManager::handlePressVisView(QPointF pos)
{
	for( int i = 0; i < this->_rectsView.size(); i++ )
	{
		if( this->_rectsView[i].contains( pos ) )
		{
			this->_VISINDEX = i;
			_visDragRect = this->_rectsView[i];
			return true;
		}
	}
	return false;
}

bool FileManager::handleMoveVisView( QPointF pos )
{
	if (inFileView(pos))
	{
		return false;
	}

	if( _VISINDEX >= 0 )
	{
		this->_visDragRect.moveTo( pos );
		return true;
	}
	return false;
}

bool FileManager::handleReleaseVisView(QPointF pos)
{
	for( int i = 0; i < this->_lines.size(); i++ )
	{
		if( this->_lines[i]->mouseOver( pos ) )
		{
			this->_lines[i]->addView( _VISINDEX, _colors[_VISINDEX] );

			logDragVisToCompose( i, _VISINDEX );
			_VISINDEX = -1;
		}
	}
	_VISINDEX = -1;
	return false;
}

void FileManager::initIcons()
{
	this->_icons[0] = QImage( tr("icon/3d.png") );
	this->_icons[1] = QImage( tr("icon/batc.png") );
	this->_icons[2] = QImage( tr("icon/batg.png") );
	this->_icons[3] = QImage( tr("icon/bath.png") );
	this->_icons[4] = QImage( tr("icon/PC1.png") );
	this->_icons[5] = QImage( tr("icon/PC2.png") );
	this->_icons[6] = QImage( tr("icon/PC3.png") );
	this->_icons[7] = QImage( tr("icon/bar chart.png") );
	this->_icons[8] = QImage( tr("icon/line chart.png") );
	this->_icons[9] = QImage( tr("icon/scatter plot.png") );
	this->_icons[10] = QImage( tr("icon/web.png") );
	this->_icons[11] = QImage( tr("icon/mat.png") );
	this->_icons[12] = QImage( tr("icon/pen.png") );
	this->_icons[13] = QImage( tr("icon/note.png") );
	
	for( int i = 0; i < _view.size(); i++ )
	{
		this->_iconRects.append( QRectF( QPointF( 0, 0 ), this->_icons[i].size() ) );
	}
	return;
}

void FileManager::openViewDirectly( QPointF pos )
{
	int size = _preOpenViewDirectly.height();
	
	if( _VISINDEX == 7 )
	{
		this->_open->openBarChart( size, pos.x() + size/2, pos.y() + size/2 );
	}
	else if( _VISINDEX == 8 )
	{
		this->_open->openLineChart( size, pos.x() + size/2, pos.y() + size/2 );
	}
	else if ( _VISINDEX == 9 )
	{
		this->_open->openScatterPlot( size, pos.x() + size/2, pos.y() + size/2 );
	}
	else if( _VISINDEX == 10 )
	{
		this->_open->openWebView( size, pos.x() + size/2, pos.y() + size/2 );
	}
	else if( _VISINDEX == 11 )
	{
		this->_open->openMatlab( size, pos.x() + size/2, pos.y() + size/2 );
	}
	else if( _VISINDEX == 12 )
	{
		this->_open->openPen( size, pos.x() + size/2, pos.y() + size/2 );
	}
	else if ( _VISINDEX == 13 )
	{
		this->_open->openNote( size, pos.x() + size/2, pos.y() + size/2 );
	}
	return;
}

void FileManager::drawPreOpenViewDirectly(QPointF pos)
{
	if( this->boundingRect().contains( pos ))
	{
		return ;
	}
	else
	{
		_preOpenViewDirectly = this->_open->queryPreOpen( pos, 1 )[0];
	}
}

void FileManager::drawColorBar( QPainter* painter )
{
	if( !COLORBAR )
	{
		return;
	}
	painter->save();
	//*********************************************
	myColor3 color;
	for( int i = 0; i<100; i++)
	{
		Lab3D point = this->_scene->getCut()->get_Position( float( i /100.0) );
		this->_scene->getLab()->get_Color( point, color );
		painter->setBrush( QColor( color.a, color.b, color.c, 255) );
		painter->setPen( QColor( color.a, color.b, color.c, 255) );
		painter->drawRect( this->getSize().width()/3 + i*2, this->getSize().height(), 2, 20 );
	}
	//*********************************************
	painter->setFont( QFont( "times", 12) );
	painter->setPen( QColor( 255, 255, 255, 255) );
	painter->drawText( this->getSize().width()/3 + 165, this->getSize().height(), tr("100%") );
	painter->drawText( this->getSize().width()/3, this->getSize().height(), tr("0%") );
	painter->restore();
	return;
}

void FileManager::OpenFileDirectly(QPointF pos)
{
	_DRAGFILE = false;
	this->_manager->getRoot()->disableDrag();

	if (this->_open->changeFile(pos, _CurrentFileName))
	{
		return ;
	}
	else if ( this->_scene->changePlotAttribute(pos, _CurrentFileName) )
	{
		return ;
	}
	else
	{
		QList<int> views;
		views.append(0);	//default 3D view

		addNewToDrag();
		int lindex = this->_lines.size()-1;
		this->_lines[lindex]->addView(0, _colors[0]);

		this->_open->openDataInViewAt(_CurrentFileName, views, pos);
	}
}

void FileManager::changeView( QPointF pos, int visindex )
{
	this->_open->changeView(pos, visindex);
}