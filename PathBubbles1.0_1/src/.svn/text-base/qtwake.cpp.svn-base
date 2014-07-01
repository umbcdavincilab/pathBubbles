#include "qtwake.h"
#include "openglscene.h"

QtWake::QtWake( int size, int x, int y, OpenGLScene* Scene, Bat* bat, QtBat* pare, int begin, int end ) : ItemBase(size, x, y)
, _ONRESIZE( false )
, _ONMOVE( false )
, _ONCONTROL( false )
, _margin( 0.0 )
{
	_scene = Scene;
	_pare = pare;
	_begin = begin;
	_end = end;
	_bat = new Bat( (*bat) );
	_bat->setWakeRender( _begin, _end );
	initFloatingControl();
	_myControl->hide();
	_myControl->setZValue(99);
	//_myControl->topLevelItem();
	_TYPE = WAKE;
	bIndex = _scene->getBubbleIndex();
	controlLastPos = this->pos();
	lastSize = this->Width();
	initInfor();
	return;
}

void QtWake::logOpen()
{
	ItemBase::logOpen();
	emit logInfor("\n");
}

void QtWake::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	//if( ani->isAnimated())
 //   {
 //       this->setPos( ani->animateP());
 //       this->m_size = ani->animateS();
 //   }

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

	//painter->setBrush( QColor( 0, 0, 50, 10));
	//painter->drawRect( this->boundingRect());

    painter->beginNativePainting();

	glEnable( GL_DEPTH_TEST );
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadMatrixf( moveToRectMatrix );

	//glFrustum(-1.0, 1.0, -1.0, 1.0, -100, 100);
	glOrtho( -1.0, 1.0, -1.0, 1.0, -100, 100);
	glTranslatef( 0.0, 0.0, -5.0 );

    glMatrixMode(GL_MODELVIEW);

	GLdouble modelview1[16];
	glGetDoublev( GL_MODELVIEW_MATRIX, modelview1 );

    glPushMatrix();
    glLoadIdentity();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);

    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    float lightColour[] = {1.0f, 1.0f, 1.0f, 1.0f};
    float lightDir[] = {0.0f, 0.0f, 1.0f, 0.0f};

    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColour);
    glLightfv(GL_LIGHT0, GL_POSITION, lightDir);
    glEnable(GL_LIGHT0);

//     int dt = m_startTime.msecsTo(QTime::currentTime());
// 	if (dt < 500)
// 	{
//         glScalef(dt / 500.0f, dt / 500.0f, dt / 500.0f);
// 	}
	glScalef(0.4f, 0.4f, 0.4f);

	// bat only draw wake with margin
	this->_bat->renderWakeFromTo();
	//

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

	ItemBase::paint(painter, option, widget);

	displayName( painter );
	displayLinksToQtBat( painter );
	updateControlPos();
}

//ItemBase* QtWake::createNew( int size, int x, int y)
//{
//	QtWake* tempWake = new QtWake( 200, 200, 200, this->_scene, this->_bat, _pare, _begin, _end );
//	return tempWake;
//}

void QtWake::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if( this->onMove( event->pos() ) ) //&& ! )
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
		}
		else
		{
			_myControl->setPos( this->pos().x() + this->Width() / 2, this->pos().y() - this->Height() / 2 );
			_myControl->show();
			_ONCONTROL = true;
		}
	}
	ItemBase::mousePressEvent( event );
}

void QtWake::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if( _ONMOVE || _ONRESIZE )
	{
		ItemBase::mouseMoveEvent( event );
	}
	else
	{
		QPointF offset = event->pos() - event->lastPos();
		_bat->rotateXY( offset.x(), offset.y() );
		this->update( this->boundingRect() );
		event->accept();
	}
}

void QtWake::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
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

void QtWake::initFloatingControl()
{
	QWidget* widget = this->_scene->getDialog();
	delete widget->layout();

	QGroupBox* controlsForWake = new QGroupBox();
	controlsForWake->setFlat( true );

	QGridLayout* result = new QGridLayout();

	//***************************************************
	QGroupBox *groupBox = new QGroupBox( tr("Frame") );
	QGridLayout* frameNumberLayout = new QGridLayout;
	groupBox->setLayout( frameNumberLayout );

	QLabel *label11 = new QLabel( tr("Begin:") );
	QString begin;
	begin.setNum( _begin );
	QLabel *label12 = new QLabel( begin );
	//********************************************
	QLabel *label21 = new QLabel( tr("End:") );
	QString end;
	end.setNum( _end );
	QLabel *label22 = new QLabel( end );
	//********************************************
	frameNumberLayout->addWidget( label11, 0, 0 );
	frameNumberLayout->addWidget( label12, 0, 1 );
	frameNumberLayout->addWidget( label21, 1, 0 );
	frameNumberLayout->addWidget( label22, 1, 1 );
	//***************************************************
	//QGroupBox *groupBox1 = new QGroupBox( tr("Margin:") );
	//QGridLayout* marginLayout = new QGridLayout;
	//groupBox1->setLayout( marginLayout );

	QLabel *label31 = new QLabel( tr("Multiple:") );
	frameNumberLayout->addWidget( label31, 2, 0 );

	lcdNumber = new QLCDNumber( controlsForWake );
	frameNumberLayout->addWidget( lcdNumber, 2, 1 );

	slider = new QSlider( Qt::Horizontal, controlsForWake );
	slider->setMaximum( 99 );
	slider->setMinimum( 0 );
	slider->setSingleStep( 1 );
	this->connect( slider, SIGNAL( sliderMoved( int ) ), this, SLOT( sliderChanged( int ) ) );
	frameNumberLayout->addWidget( slider, 3, 0, 1, 2 );
	//***************************************************

	result->addWidget( groupBox, 0, 0);
	//result->addWidget( groupBox1, 1, 0);
	widget->setLayout( result );
	_myControl = this->_scene->addWidget( widget );
}

void QtWake::updateControlPos()
{
	if( controlLastPos != this->pos() || lastSize != this->Height() )
	{
		_myControl->setPos( this->pos().x() + this->Width() / 2, this->pos().y() - this->Height() / 2 );
	}
	return;
}

void QtWake::sliderChanged(int input )
{
	this->lcdNumber->display( input );
	_bat->setWakeMargin( input );
	return;
}

void QtWake::displayLinksToQtBat( QPainter* painter )
{
	//			source points
	QPointF currentLeft( -this->Width()/2, -this->Height()/2 );
	QPointF currentRight( this->Width()/2, -this->Height()/2 );
	//			draw lines
	painter->setPen( QPen( QBrush( QColor( 100, 0, 100, 80 ) ), 3) );
	if( beginPosToLink.x() != -9999 )
	{
		painter->drawLine( beginPosToLink - this->pos(), currentLeft );
	}
	if( endPosToLink.x() != -9999 )
	{
		painter->drawLine( endPosToLink - this->pos(), currentRight );
	}
	return;
}

void QtWake::displayName( QPainter* painter )
{
	painter->setFont( QFont( "times", 9 ) );
	QRect bounding1 = painter->fontMetrics().boundingRect( tr( "Frame:" ) );
	QRect bounding2 = painter->fontMetrics().boundingRect( n5 );
	int length = bounding2.width()*6;
	int curLength = 0;
	painter->setPen( QColor( 255, 255, 255 ) );
	painter->drawText( -this->Width()/2 + (this->Width() - bounding1.width() )/2, -this->Height()/2 + bounding1.height(), tr( "Frame:" ) );
	painter->drawText( -this->Width()/2 + (this->Width() - length)/2 + curLength, -this->Height()/2 + bounding1.height() + bounding2.height(), n1 );
	curLength += bounding2.width()+4;
	painter->drawText( -this->Width()/2 + (this->Width() - length)/2 + curLength, -this->Height()/2 + bounding1.height() + bounding2.height(), n2 );
	curLength += bounding2.width()+4;
	painter->drawText( -this->Width()/2 + (this->Width() - length)/2 + curLength, -this->Height()/2 + bounding1.height() + bounding2.height(), n3 );
	curLength += bounding2.width()+4;
	painter->drawText( -this->Width()/2 + (this->Width() - length)/2 + curLength, -this->Height()/2 + bounding1.height() + bounding2.height(), n4 );
	curLength += bounding2.width()+4;
	painter->drawText( -this->Width()/2 + (this->Width() - length)/2 + curLength, -this->Height()/2 + bounding1.height() + bounding2.height(), n5 );
	return;
}

void QtWake::initInfor()
{
	float step = ( _end - _begin ) / 4.0;
	n1.setNum( int( _begin ));
	n2.setNum( int( _begin + 1*step +0.5 ));
	n3.setNum( int( _begin + 2*step +0.5));
	n4.setNum( int( _begin + 3*step +0.5 ));
	n5.setNum( int( _begin + 4*step +0.5 ));
	return;
}