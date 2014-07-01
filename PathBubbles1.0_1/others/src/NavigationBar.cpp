#include "NavigationBar.h"
#include "openglscene.h"
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>

NavigationBar::NavigationBar( QWidget *parent, OpenGLScene* scene ) : QGLWidget( QGLFormat( QGL::DoubleBuffer ), parent  )
, _rect( 0, 0, 0, 0 )
, _HIT( false )
, _ITEMRECT( -1 )
, rectOffset( 1.0 )
, increamentalX( 0 )
, increamentalY( 0 )
, _HIGHLIGHTINDEX( -1 )
, _RESIZE( false )
{
	_scale = 20;
	_scene = scene;
}

QSize NavigationBar::sizeHint() const
{
	return QSize( 250, 50);
}

void NavigationBar::paintEvent(QPaintEvent *event)
{
	QGLWidget::paintEvent( event );
	return;
}

void NavigationBar::onHighLight()
{
	QPoint pos = this->cursor().pos();
	pos = this->mapFromGlobal( pos );
	if( this->_rect.contains( pos ) )
	{
		this->_HIGHLIGHTINDEX = -1;
		return;
	}
	this->LOCKER.lock();
	addItemRect();
	for( int i = 0; i < renderArray.count(); i++ )
	{
		if( renderArray[i].contains( pos ) )
		{
			SELECTEDBOARD = renderArray[i];
			renderArray.clear();
			holderArray.clear();
			this->LOCKER.unlock();
			this->_HIGHLIGHTINDEX = i;
			return;
		}
	}
	renderArray.clear();
	holderArray.clear();
	this->LOCKER.unlock();
	return;
}

void NavigationBar::mousePressEvent( QMouseEvent *event )
{
	_ITEMRECT = hitItemRect( event->pos() );
	if( _ITEMRECT > -1 )
	{
		lastPos = event->pos();
		return;
	}
	if( this->contains( event->pos() ) )
	{
		if( onResize( event->pos() ) )
		{
			_RESIZE = true;
		}
		else
		{
			_HIT = true;
		}
		lastPos = event->pos();
	}
	return;
}

void NavigationBar::mouseReleaseEvent(QMouseEvent * event )
{
	_ITEMRECT = -1;
	_HIT = false;
	_RESIZE = false;
	return;
}

void NavigationBar::mouseMoveEvent(QMouseEvent * event)
{
	if( _ITEMRECT > -1 )
	{
		QPoint offset = event->pos() - lastPos;
		int offX = offset.x();
		int offY = offset.y();
		if( SELECTEDBOARD.x() + offX < 0 )
		{
			offX = 0;
		}
		else if( SELECTEDBOARD.x() + SELECTEDBOARD.width() + offX > this->width() )
		{
			offX = 0;
		}
		if( SELECTEDBOARD.y() + offY < 0 )
		{
			offY = 0;
		}
		else if( SELECTEDBOARD.y() + SELECTEDBOARD.height() + offY > this->height() )
		{
			offY = 0;
		}
		SELECTED->setPos( SELECTED->pos().x() + offX * this->_scale, SELECTED->pos().y() + offY * this->_scale );
		lastPos = event->pos();
	}
	if( _HIT )
	{
		QPoint offset = event->pos() - lastPos;
		shiftViewportByOffset( offset );
		lastPos = event->pos();
	}
	if( _RESIZE )
	{
		QPoint offset = event->pos() - lastPos;
		if( offset.x() > 0 )
		{
			rectOffset += 0.02;
			if( touchBoundary() )
			{
				rectOffset -= 0.02;
			}
			else
			{
				emit updateCoverage( rectOffset );
			}
		}
		else
		{
			if( ( rectOffset - 0.02 ) >= MINRATE )
			{
				rectOffset = rectOffset - 0.02;
				emit updateCoverage( rectOffset );
			}
		}
		this->update();
	}
	return;
}

void NavigationBar::shiftViewportByOffset( QPoint offset, int TYPE )
{
	int offX = offset.x();
	int offY = offset.y();
	if( TYPE == 0 )
	{
		if( _rect.x() + offX < 0 )
		{
			offX = 0;
		}
		else if( _rect.x() + _rect.width() + offX > this->width() )
		{
			offX = 0;
		}
		if( _rect.y() + offY < 0 )
		{
			offY = 0;
		}
		else if( _rect.y() + _rect.height() + offY > this->height() )
		{
			offY = 0;
		}
		_rect.translate( offX, offY );
		emit shiftViewport( QPoint( offX * this->_scale, offY * this->_scale ) );
	}
	else
	{
		increamentalX += offX;
		increamentalY += offY;
		if( abs(increamentalX) > this->_scale )
		{
			int incX = increamentalX/this->_scale;
			if( _rect.x() + incX < 0 )
			{
				incX = 0;
			}
			else if( _rect.x() + _rect.width() + incX > this->width() )
			{
				incX = 0;
			}
			_rect.translate( incX, 0 );
			emit shiftViewport( QPoint( incX * this->_scale, 0 ) );
			if( increamentalX > 0 )
			{
				increamentalX -= this->_scale*incX;
			}
			else
			{
				increamentalX += this->_scale*incX;
			}
		}
		if( abs(increamentalY) > this->_scale )
		{
			int incY = increamentalY/this->_scale;
			if( _rect.y() + incY < 0 )
			{
				incY = 0;
			}
			else if( _rect.y() + _rect.height() + incY > this->height() )
			{
				incY = 0;
			}
			_rect.translate( 0, incY );
			emit shiftViewport( QPoint( 0, incY * this->_scale ) );
			if( increamentalY > 0 )
			{
				increamentalY -= this->_scale*incY;
			}
			else
			{
				increamentalY += this->_scale*incY;
			}
		}
	}
	this->update();
	return;
}

bool NavigationBar::contains(QPoint pos)
{
	return _rect.contains( pos );
}

bool NavigationBar::onResize( QPoint pos )
{
	if( pos.x() >= ( this->_rect.left() + this->_rect.width() - 10 ) && pos.x() <= ( this->_rect.left() + this->_rect.width() ) )
	{
		if( pos.y() >= ( this->_rect.top() + this->_rect.height() - 10 ) && pos.y() <= ( this->_rect.top() + this->_rect.height() ) )
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
		return false;
	}
}

void NavigationBar::initializeGL()
{
	glOrtho( 0, this->width(), this->height(), 0, -1, 1);
	return;
}

void NavigationBar::paintGL()
{
	glClearColor( 200, 200, 200, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawRect( _rect, 0 );
	onHighLight();
	addItemRect();
	drawItemRects();
	return;
}

void NavigationBar::drawRect(QRect input, int type, bool highlight )
{
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glColor4f( 0.0, 0.0, 1.0, 0.6 );
	if( type == BAT )
	{
		glColor4f( 0.0, 0.0, 3.0, 0.6 );
	}
	else if( type == PC )
	{
		glColor4f( 8.0, 0.1, 0.1, 0.6 );
	}
	else if( type == COMPLOT )
	{
		glColor4f( 0.1, 8.0, 0.1, 0.6 );
	}
	else if( type == MATLAB )
	{
		glColor4f( 0.1, 0.1, 8.0, 0.6 );
	}
	else if( type == WEB )
	{
		glColor4f( 0.0, 0.3, 0.0, 0.6 );
	}
	else if( type == COMPARE )
	{
		glColor4f( 0.3, 0.0, 0.0, 0.6 );
	}
	else if( type == 0 )
	{
		glColor4f( 0.1, 0.1, 0.6, 0.6 );
	}
	else
	{
		glColor4f( 0.1, 0.1, 0.1, 0.6 );
	}
	glBegin(GL_POLYGON);
		glVertex2i( input.x(), input.y() );
		glVertex2i( input.x() + input.width() , input.y() );
		glVertex2i( input.x() + input.width(), input.y() + input.height() );
        glVertex2i( input.x(), input.y() + input.height() );
    glEnd();
	if( type == 0 )
	{
		glColor4f( 0.0, 0.0, 1.0, 1.0 );
		glBegin( GL_LINES );
			glVertex2i( input.x() + input.width() , input.y()+ input.height() - 10 );
			glVertex2i( input.x() + input.width(), input.y() + input.height() );
			glVertex2i( input.x() + input.width(), input.y() + input.height() );
			glVertex2i( input.x() + input.width() - 10, input.y() + input.height() );
		glEnd();
	}
	if( highlight )
	{
		glColor4f( 1.0, 0.1, 0.1, 0.6 );
		glBegin( GL_LINES );
			glVertex2i( input.x(), input.y() );
			glVertex2i( input.x() + input.width() , input.y() );
			glVertex2i( input.x() + input.width() , input.y() );
			glVertex2i( input.x() + input.width(), input.y() + input.height() );
			glVertex2i( input.x() + input.width(), input.y() + input.height() );
			glVertex2i( input.x(), input.y() + input.height() );
			glVertex2i( input.x(), input.y() + input.height() );
			glVertex2i( input.x(), input.y() );
		glEnd();
	}
	glDisable(GL_BLEND);
} 

void NavigationBar::addItemRect()
{
	QPoint offset;
	foreach( QGraphicsItem* base, _scene->items() )
	{
		ItemBase *item = dynamic_cast< ItemBase* >( base );
		if( !item )
		{
			continue;
		}
		if( item->getType() == COLOR )
		{
			continue;
		}
		int itemSize = item->Width() / this->_scale;
		int itemSize2 = item->Height() / this->_scale;
		int X = item->pos().x() / this->_scale + _rect.x();
		int Y = item->pos().y() / this->_scale + _rect.y();
		QtBat *bat = dynamic_cast<QtBat *>(item);
		if( bat )
		{
			//glColor4f( 0.0, 8.0, 8.0, 0.6 );
			//drawRect( QRect(  X - itemSize/2, Y - itemSize/2, itemSize, itemSize ) );
			renderArray.append( QRect(  X - itemSize/2, Y - itemSize2/2, itemSize, itemSize2 ) );
			holderArray.append( item );
			continue;
		}
		//QtSM *sm = dynamic_cast<QtSM *>(item);
		//if( sm )
		//{
		//	//glColor4f( 8.0, 8.0, 0.0, 0.6 );
		//	//drawRect( QRect(  X - itemSize/2, Y - itemSize/2, itemSize, itemSize ));
		//	continue;
		//}
	}
}

void NavigationBar::drawItemRects()
{
	this->LOCKER.lock();
	for( int i = 0; i < renderArray.count(); i++ )
	{
		int type = holderArray[i]->getType();
		drawRect( renderArray[i], type );
		if( i == this->_HIGHLIGHTINDEX )
		{
			drawRect( renderArray[i], type, true );
		}
	}
	renderArray.clear();
	holderArray.clear();
	this->LOCKER.unlock();
}

int NavigationBar::hitItemRect( QPoint pos )
{
	this->LOCKER.lock();
	addItemRect();
	for( int i = 0; i < renderArray.count(); i++ )
	{
		if( renderArray[i].contains( pos ) )
		{
			SELECTED = holderArray[i];
			SELECTEDBOARD = renderArray[i];
			renderArray.clear();
			holderArray.clear();
			this->LOCKER.unlock();
			return i;
		}
	}
	renderArray.clear();
	holderArray.clear();
	this->LOCKER.unlock();
	return -1;
}

void NavigationBar::wheelEvent( QWheelEvent *event )
{
	return;
}

bool NavigationBar::touchBoundary()
{
	int left = this->_rect.x();
	int right = left + this->_curSize.width() / _scale * rectOffset + 1;
	int up = this->_rect.y();
	int down = up + this->_curSize.height() / _scale * rectOffset + 1;
	int width = this->width();
	int height = this->height();
	if( left < 0 )
	{
		return true;
	}
	if( right > width )
	{
		return true;
	}
	if( up < 0 )
	{
		return true;
	}
	if( down > height )
	{
		return true;
	}
	return false;
}