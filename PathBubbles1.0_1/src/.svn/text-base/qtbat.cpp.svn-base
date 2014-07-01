#include "qtbat.h"
#include "openglscene.h"
#include <QtOpenGL>
#include "qtwake.h"
#include "PenBlock.h"

QtBat::QtBat( int size, int x, int y, OpenGLScene* Scene, std::string name, LabObject *Lab, LabCut *Cut ) : ItemBase(size, x, y)
, _ONLABEL( false )
, _ONRESIZE( false )
, _ONMOVE( false )
, _ONCONTROL( false )
, _paired( false )
, _SELECTWAKE( false )
, _BEGINSELECTED( false )
, _DETERMINESHIFT( false )
, _NOTED( false )
, _LABEL( false )
, _RIGHTPOSNEEDED( false )
, OUTPUTCAMBER( false )
, INSERTNOTE( false )
, _CONTEXTMENU( true )
{
	cur_note = NULL;
	_lab = Lab;
	_cut = Cut;
	_name = name;
	Bat *tempBat = new Bat( Lab, Cut, BATSCALE, name );
	bat.append( tempBat );
	scene = Scene;
	initFloatingUI();
	_myControl->hide();
	_myControl->setZValue(99);
	m_startTime = QTime::currentTime();
	_TYPE = BAT;
	bIndex = scene->getBubbleIndex();
	ani = new animateIt( 2000 );
	controlLastPos = this->pos();
	lastSize = this->Width();
	_REMOVECONTROL = true;
	
	rawDataView = new BatDataView( scene->getStaManager() );
	_RAWDATAVIEW = false;

	return;
}

void QtBat::logOpen()
{
	ItemBase::logOpen();
	QString logtext;
	logtext.append( " with bat name " );
	logtext.append( this->getBatName() );
	logtext.append( "\n" );

	emit logInfor( logtext );
}

void QtBat::logSelectTrailing()
{
	QString logtext;
	
	logtext.append( headofLogMsg() );
	logtext.append( "Select trailing edges" );
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void QtBat::logHideControl()
{
	QString logtext;
	
	logtext.append( headofLogMsg() );
	logtext.append( "Hide Control panel" );
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void QtBat::logShowControl()
{
	QString logtext;
	
	logtext.append( headofLogMsg() );
	logtext.append( "Open Control panel" );
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void QtBat::logWanttoCom()
{
	QString logtext;
	
	logtext.append( headofLogMsg() );
	logtext.append( "Want to Compare with another bubble" );
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void QtBat::logClearOutCamber()
{
	QString logtext;
	
	logtext.append( headofLogMsg() );
	logtext.append( "Clear out all the cambers" );
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void QtBat::logCopyAtt()
{
	QString logtext;
	
	logtext.append( headofLogMsg() );
	logtext.append( "Copy the attribute" );
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void QtBat::logPasteAtt()
{
	QString logtext;

	logtext.append( headofLogMsg() );
	logtext.append( "Paste the attribute" );
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void QtBat::logOutputCamber()
{
	QString logtext, tmp;
	
	logtext.append( headofLogMsg() );
	logtext.append( "Out put the camber " );
	tmp.setNum( _camberControl.size() );
	logtext.append( tmp );
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void QtBat::logCamberSwitchProjection()
{
	QString logtext, tmp;
	
	logtext.append( headofLogMsg() );
	logtext.append( "Switch the projection of the camber " );
	tmp.setNum( _camberControl.size() );
	logtext.append( tmp );
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void QtBat::logCamberSwitchMarker()
{
	QString logtext, tmp;
	
	logtext.append( headofLogMsg() );
	logtext.append( "Switch the marker of the camber " );
	tmp.setNum( _camberControl.size() );
	logtext.append( tmp );
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void QtBat::logCamberAttachFrame()
{
	QString logtext, tmp;
	
	logtext.append( headofLogMsg() );
	logtext.append( "Attach the camber " );
	tmp.setNum( _camberControl.size() );
	logtext.append( tmp );
	logtext.append( " with the frame" );
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void QtBat::logCamberDelete()
{
	QString logtext, tmp;
	
	logtext.append( headofLogMsg() );
	logtext.append( "Delete the camber " );
	tmp.setNum( _camberControl.size() );
	logtext.append( tmp );
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

QString QtBat::checkPath( QString path )
{
	return Bat::checkPath( path );
}

QtBat::~QtBat()
{
	unCompare();
}

void QtBat::addBat( QtBat* input )
{
	Bat *tempBat = new Bat( (*input->getBat()) );
	tempBat->setCompareBatColor( TDHelper::getBoarderColor( 255 ) );
	bat.append( tempBat );
	tempBat->acceptContext( bat[0]->getBatContext() );
	_paired = true;
	//_pair = input;
	_pair.append( input );
	//************************************
	bat.first()->resetCurrentNumberOfFrame();
	slider->setValue( 0 );
	return;
}

void QtBat::unCompare()
{
	if( !_paired )
	{
		return;
	}
	_paired = false;
	if( this->bat.count() > 1 )
	{
		for( int i = 1; i < this->bat.count(); i++ )
		{
			this->bat.removeAt( i );
		}
	}
	emit removeCompare();
	return;
}

void QtBat::unCompare( QString name )
{
	for( int i = 0 ; i < this->bat.count(); i++ )
	{
		if( this->bat[i]->getQName() == name )
		{
			delete bat[i];
			bat.removeAt( i );
			for( int j = 0; j < this->_pair.size(); j++ )
			{
				if( _pair[j]->getBatName() == name )
				{
					_pair.removeAt( j );
					break;
				}
			}
		}
	}
	if( this->bat.count() == 1 )
	{
		_paired = false;
	}
	return;
}

float QtBat::getSpeed()
{
	return _speed;
}

QString QtBat::getBatName()
{
	return this->bat.first()->getQName();
}
void QtBat::setAnimation( QPoint Distination, int size )
{
	QPointF pt = this->pos();
	ani->start( QPoint( pt.x(), pt.y() ), Distination, m_size_width, size);
}

void QtBat::setMassAndSpeed( float mass, float speed )
{
	QFontMetricsF metric( QFont( "times", 9 ) );
	this->_mass = mass;
	this->_speed = speed;
	_strMass.setNum( mass );
	_strSpeed.setNum( speed );

	QRectF bounding1 = metric.boundingRect( tr( "Speed:" ) );
	QRectF bounding2 = metric.boundingRect( tr( "Mass:" ) );
	QRectF bounding3 = metric.boundingRect( _strMass );
	QRectF bounding4 = metric.boundingRect( _strSpeed );
	lengthForButtom = bounding1.width() + bounding2.width() + bounding3.width() + bounding4.width() + 15;
	heightForButtom = bounding1.height();
	_batMassAndSpeedInfor.append( tr("Speed: ") );
	_batMassAndSpeedInfor.append( _strSpeed );
	_batMassAndSpeedInfor.append( tr(" Mass: ") );
	_batMassAndSpeedInfor.append( _strMass );

}

Bat* QtBat::getBat()
{
	return this->bat[0];
}

void QtBat::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	clickLastPos = event->pos();
	if( _SELECTWAKE )
	{
		_BEGINSELECTED = true;
		_wakeSelectedRect.setX( event->pos().x() );
		_wakeSelectedRect.setY( event->pos().y() );
		_wakeSelectedRect.setWidth( 1 );
		_wakeSelectedRect.setHeight( 1 );

		return;
	}
	if( event->button() == Qt::RightButton )
	{
		_RIGHTPOS = event->pos();
		this->_RIGHTPOSNEEDED = true;
		event->accept();
	}
	if( this->onMove( clickLastPos ) ) 
	{
		_ONMOVE = true;
	}
	if( this->isInResizeArea( event->pos() ) )
	{
		_ONRESIZE = true;
	}
	if( this->onControl( event->pos() ) )
	{
		if( _ONCONTROL )
		{
			_ONCONTROL = false;
			_myControl->hide();

			logHideControl();
		}
		else
		{
			_myControl->setPos( this->pos().x() + this->Width() / 2, this->pos().y() - this->Height() / 2 );
			_myControl->show();
			_ONCONTROL = true;

			logShowControl();
		}

	}
	ItemBase::mousePressEvent( event );
}

void QtBat::hoverMoveEvent( QGraphicsSceneHoverEvent *event)
{
	mouseMovePos = event->pos();
	if ( nameRect.contains( mouseMovePos) )
	{
		_RAWDATAVIEW = true;
		rawDataView->setBatName( bat[0]->getQName() );
		rawDataView->setPos( mouseMovePos );
	}
	else
	{
		_RAWDATAVIEW = false;
		mouseOverCamberMenu( mouseMovePos );
	}
	ItemBase::hoverMoveEvent( event );
}

void QtBat::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if( _SELECTWAKE )
	{
		_wakeSelectedRect.setWidth( -_wakeSelectedRect.x() + event->pos().x() );
		_wakeSelectedRect.setHeight( -_wakeSelectedRect.y() + event->pos().y() );
		return;
	}
	if( OUTPUTCAMBER && !_RIGHTPOSNEEDED )
	{
		this->_currentDrawing->setPos( QPoint( event->pos().x(), event->pos().y() ) );
		return;
	}
	if( _ONMOVE || _ONRESIZE || UNGROUP )
	{
		ItemBase::mouseMoveEvent( event );
	}
	else
	{
		QPointF offset = event->pos() - event->lastPos();

		emit updateRotation( this, offset );
		this->update( this->boundingRect() );
		event->accept();
	}
}

void QtBat::acceptRotation( QPointF offset )
{
	for( int i = 0; i < bat.count(); i++ )
	{
		bat[i]->rotateXY( offset.x(), offset.y() );
	}
}

void QtBat::mouseOverCamberMenu( QPointF pos )
{
	CURCAMBERINDEX = -1;
	for( int i = 0; i < this->_camberControl.size(); i++ )
	{
		if( _camberControl[i]->mouseOver( QPoint( pos.x(), pos.y() ) ) )
		{
			CURCAMBERINDEX = i;
		}
	}
	return;
}

void QtBat::sendDrawingToPenBlock( QPointF pos )
{
	foreach (QGraphicsItem *item, this->scene->items() )
	{
		ItemBase *itembase = dynamic_cast< ItemBase* >(item);
		if( itembase )
		{
			QPointF relative_Pos = pos - itembase->pos();
			if( itembase->boundingRect().contains( relative_Pos ) && itembase->getType() == PEN )
			{
				PenBlock *pen = dynamic_cast<PenBlock*>(item);
				if( pen )
				{
					pen->AcceptDrawing( this->_currentDrawing, this->_currentCamber );
					return;
				}
			}
		}
	}
	//*****************************************************
	//		create camber item
	//*****************************************************
	PenCamber* tempCamber = new PenCamber( bat[0]->getCamber(), _CURRENTCAMBERMARKER, this->bat[0]->getCurrentNumberOfFrame(), QPoint( pos.x()-this->pos().x(), pos.y()-this->pos().y() ) );
	if( addNewCamberControl( tempCamber ) )
	{
		//this->_outCamber.append( tempCamber );
		//tempCamber->setPos(  );
	}
	else
	{
		delete tempCamber;
	}
	return;
}

bool QtBat::addNewCamberControl( PenCamber* tempCamber )
{
	if( _camberControl.size() == 0 )
	{
		CamberMenu* menu = new CamberMenu( 1, tempCamber );
		menu->setPos( QPoint( -this->Width()/2, -this->Height()/2 + 20 ) );
		_camberControl.append( menu );
		return true;
	}
	if( _camberControl.last()->boundingRect().bottom() < this->Height()/2 - 30 )
	{
		CamberMenu* menu = new CamberMenu( _camberControl.last()->getIndex() + 1, tempCamber );
		QPointF pos = _camberControl.last()->boundingRect().bottomLeft();
		menu->setPos( QPoint( pos.x(), pos.y() ) );
		_camberControl.append( menu );
		return true;
	}
	else
	{
		return false;
	}
}

void QtBat::updatePosofCamberControl()
{
	if (_camberControl.isEmpty())
		return;

	QPointF pos;

	_camberControl[0]->setPos( QPoint( -this->Width()/2, -this->Height()/2 + 20 ) );
	for (int i = 1; i < _camberControl.size(); i ++)
	{
		pos = _camberControl[i-1]->boundingRect().bottomLeft();
		_camberControl[i]->setPos( QPoint( pos.x(), pos.y() ) );
	}
}

void QtBat::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	for (int i = 0; i < bat.count(); i ++)
	{
		if (bat[i]->getSymbols())
		{
			bat[i]->switchMarkerState();
		}
	}
	if( clickLastPos == event->pos() )
	{
		_DETERMINESHIFT = true;
	}
	if( OUTPUTCAMBER )
	{
		OUTPUTCAMBER = false;
		// send to the PenBlock
		sendDrawingToPenBlock( event->scenePos() );	
		event->accept();

		logOutputCamber();
	}
	if( _SELECTWAKE )
	{
		int outputBegin = -1, outputEnd = -1;
		_BEGINSELECTED = false;
		_SELECTWAKE = false;
		bat[0]->endSelectWake( outputBegin, outputEnd );

		logSelectTrailing();

		if ( outputBegin > -1 && outputEnd > -1 )
		{
			//	init new object below
			QtWake* tempWake = new QtWake( 200, this->pos().x(), this->pos().y() + this->Height(), this->scene, this->bat[0], this, outputBegin, outputEnd );
			this->scene->addItem( tempWake );
			this->_wakeOut.append( tempWake );
			connect( tempWake, SIGNAL( logInfor( QString ) ), this->scene, SLOT( logInfor( QString ) ) );
			tempWake->logOpen();
		}
		return;
	}
	if( _ONMOVE )
	{
		_ONMOVE = false;
	}
	if( _ONRESIZE )
	{
		_ONRESIZE = false;
	}

	ItemBase::mouseReleaseEvent( event );
}

void QtBat::clickMarkerOrNot()
{
	if( !_RIGHTPOSNEEDED )
	{
		return;
	}
	_RIGHTPOSNEEDED = false;
	int tempPos[2];
	float cur_Length = 999;
	int cur_index = -1;
	QPoint pos;
	for( int i = 0; i < bat[0]->getNumOfMarkers(); i++ )
	{
		bat[0]->getMarkerXToSceneAtFrame( bat[0]->getCurrentNumberOfFrame(), i, tempPos );
		tempPos[1] = this->scene->height() - tempPos[1];
		tempPos[0] = tempPos[0] - this->pos().x();
		tempPos[1] = tempPos[1] - this->pos().y();
		float length = TDHelper::distance2( _RIGHTPOS, QPointF(tempPos[0], tempPos[1]) );
		if( length < cur_Length && length < 15 )
		{
			cur_index = i;
			cur_Length = length;
			pos.setX( tempPos[0] );
			pos.setY( tempPos[1] );
		}
	}
	if( cur_index == -1 )
	{
		INSERTNOTE = false;
		return;
	}
	//camber
	if( cur_index == 1 || cur_index == 2 || cur_index == 3 )
	{
		_CURRENTCAMBERMARKER = cur_index - 1;
		//		init the camber shape
		_currentDrawing = new PenDrawing();
		int size = this->bat[0]->getCamber()->getNumOfPointAtFrame(cur_index-1, bat[0]->getCurrentNumberOfFrame());
		double *point_data;
		point_data = new double[size*3];
		this->bat[0]->getCamber()->getPointsAtFrame( bat[0]->getCurrentNumberOfFrame(), cur_index - 1, point_data );
		for( int i = size - 1; i > 0; i-- )
		{
			this->_currentDrawing->addLine( QPointF( -point_data[i*3+0]*200, -point_data[i*3+1]*200), QPointF( -point_data[(i-1)*3+0]*200, -point_data[(i-1)*3+1]*200) );
		}
		this->_currentCamber = this->bat[0]->getCamber()->getCurrentCamberAt( cur_index - 1, this->bat[0]->getCurrentNumberOfFrame() );
		//this->_currentDrawing->finalize();
		OUTPUTCAMBER = true;
		delete point_data;
	}
	else
	{
		//			note
		noteExistOrNot( bat[0]->getCurrentNumberOfFrame(), cur_index );
		INSERTNOTE = true;
	}
}

void QtBat::updateNotes()
{
	int tempPos[2];
	for( int i = 0; i < this->_notes.size(); i++ )
	{
		int frame = this->_notes[i].Frame;
		int marker = this->_notes[i].Marker;
		bat[0]->getMarkerXToSceneAtFrame( frame, marker, tempPos );
		tempPos[1] = this->scene->height() - tempPos[1];
		tempPos[0] = tempPos[0] - this->pos().x();
		tempPos[1] = tempPos[1] - this->pos().y();
		this->_notes[i].text->setPos( QPoint( tempPos[0], tempPos[1] - this->_notes[i].text->boundingRect().height() ) );
	}
	return;
}

void QtBat::noteExistOrNot( int frame, int marker )
{
	for( int i = 0; i < this->_notes.size(); i++ )
	{
		if( _notes[i].Frame == frame && _notes[i].Marker == marker )
		{
			this->cur_note = _notes[i].text;
			return;
		}
	}
	WingNote new_Note;
	new_Note.Frame = frame;
	new_Note.Marker = marker;
	new_Note.text = new PenText( true );
	this->_notes.append( new_Note );
	this->cur_note = _notes.last().text;
	return;
}

void QtBat::displayCamberItem( QPainter* painter )
{
	if( !OUTPUTCAMBER )
	{
		return;
	}
	painter->save();
	//*****************************************
	this->_currentDrawing->paint( painter );
	//*****************************************
	painter->restore();
	return;
}

void QtBat::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	QPointF tempPOS;
	if( ani->isAnimated())
	{
		this->setPos( ani->animateP());
		this->m_size_width = ani->animateS();
	}
//	updateLabel();
	updateTextNote();
	displayCamberItem( painter );
	displayOutCamber( painter );
	displayMenus( painter );		//cambermenu
	QRectF rect = boundingRect().translated( pos() );
	float width = float(painter->device()->width());
	float height = float(painter->device()->height());

	float left = 2.0f * float(rect.left()) / width - 1.0f;
	float right = 2.0f * float(rect.right()) / width - 1.0f;
	float top = 1.0f - 2.0f * float(rect.top()) / height;
	float bottom = 1.0f - 2.0f * float(rect.bottom()) / height;
	float moveToRectMatrix[] = {
		0.5f * (right - left), 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f * (bottom - top), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f * (right + left), 0.5f * (bottom + top), 0.0f, 1.0f
	};

	//painter->setBrush( QColor( 0, 0, 50, 30));
	//painter->drawRect( this->boundingRect());

	painter->beginNativePainting();

	glEnable( GL_DEPTH_TEST );
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadMatrixf( moveToRectMatrix );

	//glOrtho( -1.0, 1.0, -1.0, 1.0, -100, 100);
	gluPerspective( 60.0, 1.0, 0.1, 50.0 ); 

	glMatrixMode(GL_MODELVIEW);
	GLdouble modelview1[16];
	glGetDoublev( GL_MODELVIEW_MATRIX, modelview1 );

	glPushMatrix();
	glLoadIdentity();
	//********************************************
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_NORMALIZE);
	glShadeModel( GL_SMOOTH );

	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	GLfloat lightColour[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat light_ambient[]= { 0.05f, 0.05f, 0.05f, 0.3f };
	GLfloat lightDir[] = { 0.0f, 0.0f, 1.0f, 0.0f };
	GLfloat specular[] = { 0.4f, 0.4f, 0.4f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColour);
	glLightfv (GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, lightDir);
	glEnable(GL_LIGHT0);

	//*********************************************************************************
	for( int i = 0; i < bat.count(); i++ )
	{
		bat[i]->highlightNearMarkers( mouseMovePos, this->pos(), this->scene->height() );
		bat[i]->renderScene();
	}
	//*********************************************************************************

	clickMarkerOrNot();
	updateNotes();
	//
	if( _BEGINSELECTED )
	{
		bat[0]->highlightSelectedWake( QRect( this->pos().x() + _wakeSelectedRect.x(), this->pos().y() + _wakeSelectedRect.y(), _wakeSelectedRect.width(), _wakeSelectedRect.height()) );
	}
	if( _DETERMINESHIFT )
	{
		determineShift();
	}
	if( _SELECTWAKE )
	{
		selectWake();
	}
	registerWakePos();
	//**********************************************************************

	//**********************************************************************
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_LIGHT0);
	glDisable(GL_NORMALIZE);

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	painter->endNativePainting();

	if( _paired == true )
	{
		displayCompareLink( painter );
	}

	this->displayName( painter );
	if ( _RAWDATAVIEW )
	{
		rawDataView->paintDataView( painter );
	}
	paintNotes( painter );
	if( _BEGINSELECTED )
	{
		painter->setPen( QPen( QBrush( QColor( 150, 0, 0, 100)), 2 ));
		painter->drawRect( _wakeSelectedRect );
	}
	ItemBase::paint(painter, option, widget);
	updateControlPos();
}

void QtBat::displayCompareLink( QPainter* painter )
{
	painter->setPen( QPen( QBrush( QColor(100, 100, 0, 100), Qt::Dense4Pattern ), 4, Qt::DotLine ));
	for( int i = 0; i < this->_pair.count(); i++ )
	{
		painter->drawLine( QPointF( 0, 0 ), _pair[i]->pos() - this->pos() );
	}
	return;
}

void QtBat::displayOutCamber( QPainter* painter )
{
	//for( int i = 0; i < this->_outCamber.size(); i++ )
	//{
	//	_outCamber[i]->paint( painter );
	//}
	return;
}

void QtBat::displayMenus( QPainter* painter )
{
	painter->save();
	//***********************************************
	for( int i = 0; i < _camberControl.size(); i++ )
	{
		_camberControl[i]->paint( painter );
	}
	//***********************************************
	painter->restore();
	return;
}

void QtBat::registerWakePos()
{
	int tempPos[2];
	for( int i = 0; i < this->_wakeOut.count(); i++ )
	{
		int begin = _wakeOut[i]->getBegin(); 
		int end = _wakeOut[i]->getEnd();
		if( begin <= bat[0]->getCurrentNumberOfFrame() )
		{
			bat[0]->getMarker5ToSceneAtFrame( begin, tempPos );
			this->_wakeOut[i]->setBeginPosToLink( QPointF( tempPos[0], this->scene->height() - tempPos[1] ) );
		}
		else
		{
			this->_wakeOut[i]->setBeginPosToLink( QPointF( -9999, -9999 ) );
		}
		if( end <= bat[0]->getCurrentNumberOfFrame() )
		{
			bat[0]->getMarker5ToSceneAtFrame( end, tempPos );
			this->_wakeOut[i]->setEndPosToLink( QPointF( tempPos[0], this->scene->height() - tempPos[1] ) );
		}
		else
		{
			this->_wakeOut[i]->setEndPosToLink( QPointF( -9999, -9999 ) );
		}
	}
}

void QtBat::selectWake()
{
	int tempPos[2];
	char num[10];
	for( int i =0; i <= bat[0]->getCurrentNumberOfFrame(); i++)
	{
		if( i % 10 != 0)
		{
			continue;
		}
		bat[0]->getMarker5ToSceneAtFrame( i, tempPos );
		QString disNum;
		disNum.setNum( i );

		memcpy( num, disNum.toStdString().c_str(), disNum.count() );
		TDHelper::drawHUD( tempPos[0], this->scene->height() - tempPos[1], num, 1.0, 1.0, 1.0 );
	}
}

void QtBat::determineShift()
{
	int tempPos[2];
	_DETERMINESHIFT = false;
	bat[0]->getMarkerXToSceneAtFrame( bat[0]->getCurrentNumberOfFrame(), 9, tempPos );
	tempPos[1] = this->scene->height() - tempPos[1];
	tempPos[0] = tempPos[0] - this->pos().x();
	tempPos[1] = tempPos[1] - this->pos().y();
	float length1 = TDHelper::distance2( clickLastPos, QPointF(tempPos[0], tempPos[1]) );
	bat[0]->getMarkerXToSceneAtFrame( bat[0]->getCurrentNumberOfFrame(), 6, tempPos );
	tempPos[1] = this->scene->height() - tempPos[1];
	tempPos[0] = tempPos[0] - this->pos().x();
	tempPos[1] = tempPos[1] - this->pos().y();
	float length2 = TDHelper::distance2( clickLastPos, QPointF(tempPos[0], tempPos[1]) );
	if( length2 <= 15 && length1 <= 15 )
	{
		if( length1 < length2 )
		{
			bat[0]->switchLevel1Shift();
		}
		else
		{
			bat[0]->switchLevel2Shift();
		}
	}
	if( length2 <= 15 )
	{
		bat[0]->switchLevel2Shift();
	}
	if( length1 <= 15 )
	{
		bat[0]->switchLevel1Shift();
	}
	return;
}

void QtBat::paintNotes( QPainter* painter )
{
	painter->save();
	for( int i = 0; i < this->_notes.size(); i++ )
	{
		this->_notes[i].text->paint( painter );
	}
	painter->restore();
	return;
}

void QtBat::updateControlPos()
{
	if( _myControl && ( controlLastPos != this->pos() || lastSize != this->Width() ) )
	{
		_myControl->setPos( this->pos().x() + this->Width() / 2, this->pos().y() - this->Height() / 2 );
	}
	return;
}

void QtBat::displayName( QPainter *painter )
{
	painter->setFont( QFont( "times", 9 ) );
	//painter->setPen( QColor( 0, 0, 0 ) );
	//			display the mass and speed
	painter->drawText( -this->Width()/2 + (this->Width() - lengthForButtom)/2, -this->Height()/2+2*heightForButtom, _batMassAndSpeedInfor );
	int lastHeight = 0;
	//painter->drawText( -this->Size()/2 + bounding.width()/2, -this->Size()/2 + bounding.height(), tr( bat->getname().c_str() ) );
	for( int i = 0; i < this->bat.count(); i++ )
	{
		painter->setPen( bat[i]->getCompareBatColor() );
		
		QString name = bat[i]->getQName();
		int index = name.indexOf("_");
		name.remove(0, index+1);

		QRect bounding = painter->fontMetrics().boundingRect( name );
	
		painter->drawText( -this->Width()/2 + (this->Width() - bounding.width())/2, -this->Height()/2 + lastHeight + heightForButtom, name );
		nameRect = QRectF( -this->Width()/2 + (this->Width() - bounding.width())/2, -this->Height()/2 + lastHeight + heightForButtom-bounding.height(), bounding.width(), bounding.height() );

		lastHeight += bounding.height();
	}
	//painter->drawText( -this->Size()/2 + (this->Size() - bounding.width())/2, this->Size()/2, tr( bat[0]->getname().c_str() ) );
	return;
}

void QtBat::setFrame( float input, int number )
{
	slider->setValue(int( input*100 ) );
	lcdNumber->display( number + 1 );

	for( int i = 0; i < bat.count(); i++ )
	{
		bat[i]->setCurrentNumberOfFrame( input );
	}
	for( int i = 0; i < _camberControl.size(); i++ )
	{
		_camberControl[i]->setFrame( input * bat[0]->getTotalNumberOfFrame() );
	}
}

void QtBat::initFloatingUI()
{
	QWidget* widget = scene->getDialog();
	delete widget->layout();

	QGroupBox* controlsForBat = new QGroupBox();
	controlsForBat->setFlat( true );

	QGridLayout* result = new QGridLayout();
	//		second row
	QGroupBox *groupBox = new QGroupBox( tr("Frame Number(%)"), controlsForBat );
	QGridLayout* frameNumberLayout = new QGridLayout;
	groupBox->setLayout( frameNumberLayout );

	lcdNumber = new QLCDNumber( controlsForBat );
	frameNumberLayout->addWidget( lcdNumber, 0, 0, 1, 2 );
	lcdNumber->display( 1 );

	slider = new QSlider( Qt::Horizontal, controlsForBat );
	slider->setMaximum( 99 );
	slider->setMinimum( 0 );
	slider->setSingleStep( 1 );
	this->connect( slider, SIGNAL( valueChanged( int ) ), this, SLOT( sliderChanged( int ) ) );
	frameNumberLayout->addWidget( slider, 1, 0, 1, 3 );

	result->addWidget( groupBox, 0, 0);

	//		third row
	QGroupBox *groupBox1 = new QGroupBox( tr("Wake Structure"), controlsForBat );
	QGridLayout* wakeStructureLayout = new QGridLayout;
	groupBox1->setLayout( wakeStructureLayout );

	surface = new QCheckBox( tr( "Surface" ), controlsForBat );
	surface->setChecked( true );
	this->connect( surface, SIGNAL( stateChanged ( int ) ), this, SLOT( surfaceChecked( int ) ) );
	wireframe = new QCheckBox( tr( "Wireframe" ), controlsForBat );
	this->connect( wireframe, SIGNAL( stateChanged ( int ) ), this, SLOT( wireframeChecked( int ) ) );
	wireframe->setChecked( true );
	symbols = new QCheckBox( tr( "Symbols" ), controlsForBat );
	this->connect( symbols, SIGNAL( stateChanged (int )), this, SLOT(symbolChecked( int ) ) );
	symbols->setChecked( false );

	wakeStructureLayout->addWidget( surface, 0, 0 );
	wakeStructureLayout->addWidget( wireframe, 1, 0 );
	wakeStructureLayout->addWidget( symbols, 2, 0);

	result->addWidget( groupBox1, 1, 0);
	//		fourth row
	QGroupBox *groupBox2 = new QGroupBox( tr("Camber"), controlsForBat );
	QGridLayout* camberLayout = new QGridLayout;
	groupBox2->setLayout( camberLayout );

	camber1 = new QCheckBox( tr( "M1" ), controlsForBat );
	camber1->setChecked( true );
	this->connect( camber1, SIGNAL( stateChanged ( int ) ), this, SLOT( camber1Checked( int ) ) );
	camber2 = new QCheckBox( tr( "M2" ), controlsForBat );
	camber2->setChecked( true );
	this->connect( camber2, SIGNAL( stateChanged ( int ) ), this, SLOT( camber2Checked( int ) ) );
	camber3 = new QCheckBox( tr( "M3" ), controlsForBat );
	camber3->setChecked( true );
	this->connect( camber3, SIGNAL( stateChanged ( int ) ), this, SLOT( camber3Checked( int ) ) );
	camberLayout->addWidget( camber1, 0, 0 );
	camberLayout->addWidget( camber2, 0, 1 );
	camberLayout->addWidget( camber3, 1, 0, 1, 2 );

	result->addWidget( groupBox2, 2, 0);

	widget->setLayout( result );

	_myControl = scene->addWidget( widget );

	this->connect( this, SIGNAL( sliderChangedinGroup(ItemBase*, int)), this->scene->getGroupManager(), SLOT( sliderChanged(ItemBase*, int)) );
	this->connect( this, SIGNAL( surfaceCheckedinGroup(ItemBase*, int)), this->scene->getGroupManager(), SLOT( surfaceChecked(ItemBase*, int)) );
	this->connect( this, SIGNAL( wireframeCheckedinGroup(ItemBase*, int)), this->scene->getGroupManager(), SLOT( wireframeChecked(ItemBase*, int)) );
	this->connect( this, SIGNAL( symbolCheckedinGroup(ItemBase*, int)), this->scene->getGroupManager(), SLOT( symbolChecked(ItemBase*, int)) );
	this->connect( this, SIGNAL( camber1CheckedinGroup(ItemBase*, int)), this->scene->getGroupManager(), SLOT( camber1Checked(ItemBase*, int)) );
	this->connect( this, SIGNAL( camber2CheckedinGroup(ItemBase*, int)), this->scene->getGroupManager(), SLOT( camber2Checked(ItemBase*, int)) );
	this->connect( this, SIGNAL( camber3CheckedinGroup(ItemBase*, int)), this->scene->getGroupManager(), SLOT( camber3Checked(ItemBase*, int)) );

}

void QtBat::updateFloatingUI(BatContext bcontext)
{
	float rate = 1.0*bcontext.cur_frame_num / bcontext.frame_nums;
	setFrame( rate, (int)(rate*100)+1 );

	surface->setChecked(bcontext.surface);
	wireframe->setChecked(bcontext.wireframe);
	symbols->setChecked(bcontext.symbols);
	camber1->setChecked(bcontext.camber[0]);
	camber2->setChecked(bcontext.camber[1]);
	camber3->setChecked(bcontext.camber[2]);
}

void QtBat::contextMenuEvent( QGraphicsSceneContextMenuEvent *event )
{
	if (!isSelected()) 
	{
		scene->clearSelection();
		setSelected(true);
	}
	if( OUTPUTCAMBER || INSERTNOTE )
	{
		return;
	}
	if( !_CONTEXTMENU )
	{
		return;
	}
	if( CURCAMBERINDEX != -1 )
	{
		QMenu menu;
		QAction *proAction = menu.addAction("Switch projection");
		QAction *marAction = menu.addAction("Switch marker");
		QAction *fraAction = menu.addAction("Attach frame");
		QAction *delAction = menu.addAction("Delete");
		QAction *selectedAction = menu.exec(event->screenPos());
		if (selectedAction == proAction)
		{
			_camberControl[CURCAMBERINDEX]->switchProjection();

			logCamberSwitchProjection();
		}
		else if(selectedAction == marAction)
		{
			_camberControl[CURCAMBERINDEX]->switchMarker();
			
			logCamberSwitchMarker();
		}
		else if(selectedAction == fraAction)
		{
			_camberControl[CURCAMBERINDEX]->switchFrame();
			
			logCamberAttachFrame();
		}
		else if(selectedAction == delAction)
		{
			logCamberDelete();

			delete _camberControl[CURCAMBERINDEX];
			_camberControl.removeAt( CURCAMBERINDEX );
		}
		return;
	}
	//*****************************************************
	QMenu menu;
	QAction *delAction = menu.addAction("Delete");
	QAction *ungroupAction = menu.addAction("Ungroup");
	QAction *comAction = menu.addAction("Compare");
	QAction *wakeAction = menu.addAction("Select trailing edge");
	QAction *camAction = menu.addAction("Clear Camber Projection");
	QAction *copyAttribute = menu.addAction("Copy Attribute");
	QAction *pasteAttribute = menu.addAction("Paste Attribute");

	QAction *selectedAction = menu.exec(event->screenPos());

	if (selectedAction == delAction)
	{
		if( this->_paired )
		{
			QMessageBox msgBox;
			msgBox.setText("Please remove compare bubble first!");
			msgBox.exec();
		}
		else
		{
			logRemove();
			//find which video block belongs to 
			deleteSelectedItems( scene );
		}
	}
	else if (selectedAction == comAction)
	{
		// NEEDS COMPARE WITH OTHER OBJECT
		logWanttoCom();

		emit this->needCompare( this );
	}
	else if( selectedAction == ungroupAction )
	{
		logUnGroup();

		emit unGroup( this );
	}
	else if (selectedAction == wakeAction)
	{
		_SELECTWAKE = true;
		bat[0]->beginSelectWake( this->scene->height() );
	}
	else if( camAction == selectedAction )
	{
		logClearOutCamber();

		clearOutCamber();
	}
	else if ( selectedAction == copyAttribute )
	{
		logCopyAtt();

		emit setCurrentBatAttribute( this ); 
	}
	else if ( selectedAction == pasteAttribute )
	{
		logPasteAtt();

		emit getNewBatAttribute( this );
	}
	return;
}

void QtBat::clearOutCamber()
{
	for( int i = 0; i < _camberControl.size(); i++ )
	{
		delete this->_camberControl[i];
	}
	this->_camberControl.clear();
}

void QtBat::updateTextNote()
{
	if( this->_NOTED == false )
	{
		return;
	}
	_note->setPos( this->m_size_width/2+1, -this->m_size_height/2 );
	return;
}

void QtBat::acceptNewBat(QtBat *target)
{
	this->bat.append( target->getBat() );
	//this->bat.last()->setCompareBatColor( TDHelper::getBoarderColor( 0.6 ) );
}

void QtBat::keyPressEvent(QKeyEvent *event)
{
	//ItemBase::keyPressEvent( event );
	if( !INSERTNOTE || !this->isSelected() )
	{
		return;
	}
	if( cur_note == NULL )
	{
		ItemBase::keyPressEvent( event );
	}
	int key = event->key();
	switch( key )
	{
		//case 16777220: // Enter
		//	handleEnter();
		//	break;
	case 16777219:	// backspace
		this->cur_note->removeText();
		break;
	default:
		this->cur_note->appendText( event->text() );
	}
	return;
}

void QtBat::updateLabel()
{
	if (_LABEL == false)
	{
		_label = new ItemLabel(this, scene, m_size_width, m_size_height );
		_label->setLabelColor(_colorBoarder);
		this->scene->addItem(_label);
		_LABEL = true;

		emit updateLabelState( this );

		return;
	}
	this->_label->updateLabel( m_size_width, m_size_height );
}

void QtBat::updateWingShapePara( ArrowGen* arrowGened )
{
	float tmpCylinderSize, tmpSphereSize;

	if (arrowGened->getCylinderInfor() == SPEED)
	{
		tmpCylinderSize = _speed/100.0;
	}
	else if (arrowGened->getCylinderInfor() == MASS)
	{
		tmpCylinderSize = _mass;
	}
	else if (arrowGened->getCylinderInfor() == NONE)
	{
		tmpCylinderSize = DefaultCylinderSize;
	}

	if (arrowGened->getSphereInfor() == SPEED)
	{
		tmpSphereSize = _speed/100.0;
	}
	else if (arrowGened->getSphereInfor() == MASS)
	{
		tmpSphereSize = _mass;
	}
	else if (arrowGened->getSphereInfor() == NONE)
	{
		tmpSphereSize = DefaultSphereSize;
	}

	for (int i = 0; i < bat.count(); i ++)
	{
		if (arrowGened->getCurrentData() == SPHERE)
		{
			bat[i]->setArrowType( 1 );
			bat[i]->setSpherePara( tmpSphereSize );
		}
		else if (arrowGened->getCurrentData() == CYLINDER)
		{
			bat[i]->setArrowType( 2 ); 
			bat[i]->setCylinderPara( tmpCylinderSize );
		}
		else if (arrowGened->getCurrentData() == ARROW)
		{
			bat[i]->setArrowType( 0 );
			bat[i]->setSpherePara( tmpSphereSize );
			bat[i]->setCylinderPara( tmpCylinderSize );
		}
	}
}

void QtBat::setBatContext(BatContext bcontext)
{
	for (int i = 0; i < bat.size(); i++)
	{
		bat[i]->acceptContext(bcontext);
	}
	updateFloatingUI( bcontext );
}

void QtBat::sliderChanged( int number )
{
	emit sliderChangedinGroup( this, number );
}

void QtBat::wireframeChecked( int state )
{
	emit wireframeCheckedinGroup( this, state );
}

void QtBat::symbolChecked( int state )
{
	emit symbolCheckedinGroup( this, state );
}

void QtBat::surfaceChecked( int state )
{
	emit surfaceCheckedinGroup( this, state );
}

void QtBat::camber1Checked( int state )
{
	emit camber1CheckedinGroup( this, state );
}

void QtBat::camber2Checked( int state )
{
	emit camber2CheckedinGroup( this, state );
}

void QtBat::camber3Checked( int state )
{
	emit camber3CheckedinGroup( this, state );
}

void QtBat::saveAsXmlNode( QDomDocument &doc, QDomElement &node )
{
	QDomElement type = doc.createElement( tr("Type") );
	QDomElement filename = doc.createElement( tr("FileName") );	
	QDomText text;

	text = doc.createTextNode( tr("Bat") );
	type.appendChild( text );
	node.appendChild( type );

	text = doc.createTextNode( this->getBatName() );
	filename.appendChild( text );
	node.appendChild( filename );
	
	ItemBase::saveAsXmlNode( doc, node );
}

void QtBat::recoverFromXmlNode( QDomElement node )
{
	ItemBase::recoverFromXmlNode( node );
}