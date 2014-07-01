#include "VolumBlock.h"
#include "openglscene.h"

VolumBlock::VolumBlock(int size, int x, int y, OpenGLScene *scene, Bat *dat)
: ItemBase( size, x, y )
, _ONCONTROL( false )
{
	this->_scene = scene;
	this->_TYPE = VOLUM;
	this->bIndex = _scene->getBubbleIndex();
	this->_bat = new Bat( (*dat) );
	this->_bat->getCamber()->initVolum();
	return;
}

void VolumBlock::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
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

	glOrtho( -1.0, 1.0, -1.0, 1.0, -100, 100);
	//gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
	//gluPerspective( 60.0, 1.0, 0.1, 10.0 );
	
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
    GLfloat lightColour[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat light_ambient[]= { 0.05f, 0.05f, 0.05f, 0.3f };
    GLfloat lightDir[] = { 0.0f, 0.0f, 1.0f, 0.0f };
	GLfloat specular[] = { 0.4f, 0.4f, 0.4f, 1.0f };
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColour);
	glLightfv (GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT0, GL_POSITION, lightDir);
    glEnable(GL_LIGHT0);

    int dt = m_startTime.msecsTo(QTime::currentTime());
	if (dt < 500)
	{
        glScalef(dt / 500.0f, dt / 500.0f, dt / 500.0f);
	}
	//**************************************************
	//			highlight
	//**************************************************
	this->_bat->renderVolum();
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

	ItemBase::paint( painter, option, widget );
	return;
}

void VolumBlock::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
	return;
}

void VolumBlock::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if( onBoarder( event->pos() ) )
	{
		_ONCONTROL = true;
	}
	ItemBase::mousePressEvent( event );
	return;
}

void VolumBlock::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if( _ONCONTROL )
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
	return;
}

void VolumBlock::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	_ONCONTROL = false;
	ItemBase::mouseReleaseEvent( event );
	return;
}

void VolumBlock::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
	ItemBase::hoverMoveEvent( event );
	return;
}