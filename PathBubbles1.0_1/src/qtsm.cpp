#include "qtsm.h"
#include "openglscene.h"

const qreal ROTATE_SPEED_X = 30.0 / 1000.0;
const qreal ROTATE_SPEED_Y = 20.0 / 1000.0;
const qreal ROTATE_SPEED_Z = 40.0 / 1000.0;

QtSM::QtSM(int size, int x, int y, OpenGLScene* Scene) : ItemBase(size, x, y)
, ONDRAG( false )
, lastSize_width( size )
, lastSize_height( size )
, _ONCONTROL( false )
, LINKED( false )
, _NEEDHI( false )
, _DISRECT( false )
, _LABEL( false )
{
	this->_REMOVECONTROL = true;
    m_startTime = QTime::currentTime();
	scene = Scene;
	_TYPE = PC;
	bIndex = scene->getBubbleIndex();
	controlLastPos = this->pos();

    //sm->size_hint( (float)size / (float)MAX_ITEM_SIZE );
	rawDataView = new BatDataView( scene->getStaManager() );
	_RAWDATAVIEW = false;
}

QtSM::~QtSM()
{
}

void QtSM::setSM( SM* input)
{
	sm = input; 
	initFloatingUI();
	_myControl->hide();
}

void QtSM::logOpen()
{
	ItemBase::logOpen();
	QString logtext;
	logtext.append( " with File name " );
	logtext.append( this->getName() );
	logtext.append( "\n" );
	emit logInfor( logtext );
}

void QtSM::logSwapBars( int st, int ed )
{
	QString logtext, tmp;
	
	logtext.append( headofLogMsg() );
	logtext.append( "Swap the two bars ");

	tmp.setNum( st );
	logtext.append( tmp );
	tmp.setNum( ed );
	logtext.append(" and ");
	logtext.append( tmp );

	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

void QtSM::logHighlight( QList<int> selected )
{
	QString logtext, tmp;

	logtext.append( headofLogMsg() );
	logtext.append( "Highlight the frames [");

	for ( int i = 0; i < selected.size(); i ++ )
	{
		tmp.setNum( selected[i] );
		if (i < selected.size()-1)
		{
			tmp.append(", ");
		}
		logtext.append( tmp );
	}
	logtext.append("]");
	logtext.append( tailofLogMsg() );

	emit logInfor( logtext );
}

//ItemBase *QtSM::createNew(int size, int x, int y)
//{
//    return new QtSM(size, x, y, scene);
//}

QString QtSM::checkPath( QString name )
{
	return "";
}

void QtSM::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rect = boundingRect().translated(pos());
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

	//painter->setBrush( QColor( 0, 0, 50, 10));
	//painter->drawRect( this->boundingRect());
//	updateLabel();

    painter->beginNativePainting();

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadMatrixf(moveToRectMatrix);
    //gluPerspective(60.0, 1.0, -50, 50.0);
	glOrtho( -rect.width()/2 , rect.width() - rect.width() / 2, rect.height() - rect.height() / 2, - rect.height() / 2 , -10, 10 );
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    //glEnable(GL_LIGHTING);
    //glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);

    //glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    //float lightColour[] = {1.0f, 1.0f, 1.0f, 1.0f};
    //float lightDir[] = {0.0f, 0.0f, 1.0f, 0.0f};
    //glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColour);
    //glLightfv(GL_LIGHT0, GL_POSITION, lightDir);
    //glEnable(GL_LIGHT0);

    glTranslatef(0.0f, 0.0f, -1.5f);
    int dt = m_startTime.msecsTo(QTime::currentTime());
    if (dt < 500)
        glScalef(dt / 500.0f, dt / 500.0f, dt / 500.0f);

    sm->renderScene();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    //glDisable(GL_LIGHTING);
    //glDisable(GL_COLOR_MATERIAL);
    //glDisable(GL_LIGHT0);
    glDisable(GL_NORMALIZE);

    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    painter->endNativePainting();

	sm->renderFontWithPainter( painter );

	painter->setFont( QFont( "times", 9 ) );
	QString name = tr( sm->getName().c_str() );
	int index = name.indexOf( "_" );
	name.remove( 0, index+1 );
	QRect bounding = painter->fontMetrics().boundingRect( name );
	painter->drawText( QPointF(-this->Width()/2 + (this->Width() - bounding.width())/2, -this->Height()/2+bounding.height() ), name );
    nameRect = QRectF( -this->Width()/2 + (this->Width() - bounding.width())/2, -this->Height()/2, bounding.width(), bounding.height() );

	if ( _RAWDATAVIEW )
	{
		rawDataView->paintDataView( painter );
	}

	ItemBase::paint(painter, option, widget);

	if( _DISRECT )
	{
		painter->setPen( QPen( QBrush( QColor( 0, 0, 100, 80), Qt::Dense4Pattern ), 2 ));
		painter->setBrush( QBrush( QColor( 0, 0, 100, 80) ) );
		painter->drawRect( _highLighted );
	}

	updateControlPos();
}

void QtSM::tryToLinkWithBat()
{
	LINKED = false;
	
	QtBat* node = this->scene->getGroupManager()->getBatLinkWithPC( this );
	if ( node != NULL )
	{
		_linked = node;
		LINKED = true;
	}
}

void QtSM::pressHightLight( QRect rect )
{
	_DISRECT = true;
	tryToLinkWithBat();
	_highLighted = rect;
}

void QtSM::moveHightLight( QRect rect )
{
	_highLighted = rect;
	QList<int> tmpList = enableHighlight();
	if( this->LINKED )
	{
		_linked->setHighlight( tmpList );
	}
}

void QtSM::releaseHighLight()
{
	_NEEDHI = false;
	_DISRECT = false;
	//		set highlight
	QList<int> tmpList = enableHighlight();
	if( this->LINKED )
	{
		_linked->setHighlight( tmpList );
	}

	logHighlight( tmpList );
}

void QtSM::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if( _NEEDHI == true )
	{
		QRect rect = QRect( event->pos().x(), event->pos().y(), 1, 1 );
		emit pressHightlightInGroup( this, rect );
	}
	if( this->onControl( event->pos() ) )
	{
		if( _ONCONTROL )
		{
			_ONCONTROL = false;
			_myControl->hide();
		}
		else
		{
			_myControl->setPos( this->pos().x() + this->Width() / 2, this->pos().y() - this->Height() / 2 );
			_myControl->show();
			_ONCONTROL = true;
		}
		return;
	}
	if( sm->mousePressEvent( event->pos().x(), event->pos().y() ) > 0 )
	{
		ONDRAG = true;
	}

	ItemBase::mousePressEvent( event );
	return;
}

void QtSM::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if( _NEEDHI == true )
	{
		_highLighted.setWidth( event->pos().x() - _highLighted.x() );
		_highLighted.setHeight( event->pos().y() - _highLighted.y() );
		
		emit moveHightlightInGroup( this, _highLighted );

		return;
	}
	if( ONDRAG )
	{
		sm->mouseMoveEvent( event->pos().x(), event->pos().y() );
		return;
	}
	ItemBase::mouseMoveEvent( event );
	if( lastSize_width != this->Width() || lastSize_height != this->Height() )
	{

		sm->updateSize( this->Width(), this->Height() );
		lastSize_width = this->Width();
		lastSize_height = this->Height();
	}
	return;
}

void QtSM::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	if( _NEEDHI == true )
	{
		emit releaseHightlightInGroup( this );
		return;
	}
	if( this->ONDRAG )
	{
		sm->mouseReleaseEvent( event->pos().x(), event->pos().y() );
		ONDRAG = false;
	}
	ItemBase::mouseReleaseEvent( event );
	return;
}

QList<int> QtSM::enableHighlight()
{
	return sm->findHighlight( _highLighted );
	//int endFrame = sm->mapFrame( _highLighted.y() );
	//int beginFrame = sm->mapFrame( _highLighted.y() + _highLighted.height() );
	//if( LINKED )
	//{
	//	_linked->setHighlight( beginFrame, endFrame );
	//}
}

void QtSM::disableHighlight()
{
	if( LINKED )
	{
		QList<int> temp;
		_linked->setHighlight( temp );
	}
	return;
}

void QtSM::updateControlPos()
{
	if( controlLastPos != this->pos() || lastSize_width != this->Width() || lastSize_height != this->Height() )
	{
		_myControl->setPos( this->pos().x() + this->Width() / 2, this->pos().y() - this->Height() / 2 );
	}
	return;
}

void QtSM::initFloatingUI()
{
	QWidget* widget = scene->getDialog();
	delete widget->layout();
	QGridLayout* result = new QGridLayout();
	//			The first one
	//SMDetail detail1 = sm->getDetailAt( 0 );
	//QGroupBox *groupBoxSpeed = new QGroupBox( tr( detail1.name.c_str() ), controlForSM );
	//QGridLayout* detailSpeed = new QGridLayout;
	//groupBoxSpeed->setLayout( detailSpeed );
	//QLabel *label1 = new QLabel( tr(" Max: "), groupBoxSpeed );
	//QLabel *label2 = new QLabel( QString().setNum( detail1.max ), groupBoxSpeed );
	//detailSpeed->addWidget( label1, 0, 0 );
	//detailSpeed->addWidget( label2, 0, 1 );
	//QLabel *label3 = new QLabel( tr(" Min: "), groupBoxSpeed );
	//QLabel *label4 = new QLabel( QString().setNum( detail1.min ), groupBoxSpeed );
	//detailSpeed->addWidget( label3, 1, 0 );
	//detailSpeed->addWidget( label4, 1, 1 );
	//result->addWidget( groupBoxSpeed, 0, 0 );
	////			The second one 
	//SMDetail detail2 = sm->getDetailAt( 1 );
	//QGroupBox *groupBoxFrame = new QGroupBox( tr( detail2.name.c_str() ), controlForSM );
	//QGridLayout* detailFrame = new QGridLayout;
	//groupBoxFrame->setLayout( detailFrame );
	//QLabel *label5 = new QLabel( tr(" Max: "), groupBoxFrame );
	//QLabel *label6 = new QLabel( QString().setNum( detail2.max ), groupBoxFrame );
	//detailFrame->addWidget( label5, 0, 0 );
	//detailFrame->addWidget( label6, 0, 1 );
	//QLabel *label7 = new QLabel( tr(" Min: "), groupBoxFrame );
	//QLabel *label8 = new QLabel( QString().setNum( detail2.min ), groupBoxFrame );
	//detailFrame->addWidget( label7, 1, 0 );
	//detailFrame->addWidget( label8, 1, 1 );
	//result->addWidget( groupBoxFrame, 1, 0 );
	////			The third one
	//SMDetail detail3 = sm->getDetailAt( 2 );
	//QGroupBox *groupBoxCamber = new QGroupBox( tr( detail3.name.c_str() ), controlForSM );
	//QGridLayout* detailCamber = new QGridLayout;
	//groupBoxCamber->setLayout( detailCamber );
	//QLabel *label9 = new QLabel( tr(" Max: "), groupBoxCamber );
	//QLabel *label10 = new QLabel( QString().setNum( detail3.max ), groupBoxCamber );
	//detailCamber->addWidget( label9, 0, 0 );
	//detailCamber->addWidget( label10, 0, 1 );
	//QLabel *label11 = new QLabel( tr(" Min: "), groupBoxCamber );
	//QLabel *label12 = new QLabel( QString().setNum( detail3.min ), groupBoxCamber );
	//detailCamber->addWidget( label11, 1, 0 );
	//detailCamber->addWidget( label12, 1, 1 );
	//result->addWidget( groupBoxCamber, 2, 0 );
	//*************************************************************************
	radioButForInd = new QRadioButton("Color by Individual", widget );
	radioButForInd->setChecked( true );
	connect( radioButForInd, SIGNAL( clicked() ), this, SLOT( switchColorMethod( ) ) );
	radioButForGlo = new QRadioButton("Color by Species", widget);
	connect( radioButForGlo, SIGNAL( clicked() ), this, SLOT( switchColorMethod( ) ) );
	result->addWidget( radioButForInd, 0, 0 );
	result->addWidget( radioButForGlo, 1, 0 );
	//*************************************************************************
	widget->setLayout( result );
	_myControl = scene->addWidget( widget );
	return;
}

void QtSM::hoverMoveEvent( QGraphicsSceneHoverEvent *event )
{
	if ( nameRect.contains( event->pos() ) )
	{
		_RAWDATAVIEW = true;
		rawDataView->setBatName( tr(sm->getName().c_str()) );
		rawDataView->setPos( event->pos() );
	}
	else
	{
		_RAWDATAVIEW = false;
		this->sm->mouseOn( event->pos().x(), event->pos().y() );
	}
	ItemBase::hoverMoveEvent( event );
}

void QtSM::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    if ( !isSelected() && scene )
	{
        scene->clearSelection();
        setSelected(true);
    }

    QMenu menu;
	QAction *hiAction = menu.addAction("Highlight");
	QAction *ungroupAction = menu.addAction("Ungroup");
    QAction *delAction = menu.addAction("Delete");

    QAction *selectedAction = menu.exec(event->screenPos());

    if (selectedAction == delAction)
	{
		logRemove();
        deleteSelectedItems( scene );
	}
	else if (selectedAction == hiAction)
	{
		if( !_NEEDHI )
		{
			_NEEDHI = true;
		}
		else
		{
			_NEEDHI = false;
		}
		emit HighlightChangeInGroup( this, _NEEDHI);
	}
	else if( selectedAction == ungroupAction )
		emit unGroup( this );
}

void QtSM::switchColorMethod()
{
	if( radioButForInd->isChecked() )
	{
		emit updatePCColorState( this, 0 );
	}
	else
	{
		emit updatePCColorState( this, 1 );
	}
}

void QtSM::needSwapBars(int source, int target)
{
	this->sm->swap( source, target );
	logSwapBars( source, target );
	return;
}

void QtSM::updateLabel()
{
	if (_LABEL == false)
	{
		_label = new ItemLabel( this, scene, m_size_width, m_size_height );
		_label->setLabelColor(_colorBoarder);
		this->scene->addItem(_label);
		_LABEL = true;

		emit updateLabelState( this );

		return;
	}
	_label->updateLabel( m_size_width, m_size_height );
}

void QtSM::saveAsXmlNode( QDomDocument &doc, QDomElement &node )
{
	QDomText text;

	QDomElement type = doc.createElement( tr("Type") );
	text = doc.createTextNode( tr("Parallel Coordinate") );
	type.appendChild( text );
	node.appendChild( type );

	ItemBase::saveAsXmlNode( doc, node );

	QDomElement fileName = doc.createElement( tr("FileName") );
	text = doc.createTextNode( this->getName() );
	fileName.appendChild( text );
	node.appendChild( fileName );
}

void QtSM::recoverFromXmlNode( QDomElement node )
{
	ItemBase::recoverFromXmlNode( node );
}