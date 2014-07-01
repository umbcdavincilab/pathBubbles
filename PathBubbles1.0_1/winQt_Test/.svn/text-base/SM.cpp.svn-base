#include "SM.h"
#include <cstring>
#include <sstream>

#ifndef FONT
#define FONT
void* glutFonts[3] = {
        GLUT_BITMAP_HELVETICA_10,
        GLUT_BITMAP_HELVETICA_12,
        GLUT_BITMAP_HELVETICA_18
};
#endif

SM::SM( std::string input, int size ) : num_Camber( 0 )
, num_Speed( 0 )
//, SPACE( 60 )
//, HEIGHT( 160 )
//, WIDTH( 5 )
, font_index( -1 )
, _size_width( size )
, _size_height( size )
, barOffsetX( 0 )
, barOffsetY( 0 )
, _index( -1 )
, _HIGHLIGHT( -1 )
, _INIT( true )
, COLORSTATE( 0 )
{
	pathName = input;
	//
	int start = pathName.find_last_of( "/" );
	int end = pathName.find_last_of( "." );
	int length = end - start - 1;
	_name = pathName.substr( start + 1, length );
	//
	initParameters();
	loadData();
}

void SM::initParameters()
{
	SPACE = _size_width / 3;
	HEIGHT = _size_height * 0.7;
	WIDTH = _size_width / 40;
	OFFSET = -_size_width * 0.35;
	return;
}

QString SM::checkPath( QString name )
{
	QString result;
	//*************************************************
	QString camberPath = name + ".camber";
	if( !QFile::exists( camberPath ) )
	{
		result = "Camber file cannot be opened.";
		return result;
	}
	QString speedPath = name + ".speed";
	if( !QFile::exists( speedPath ) )
	{
		result = "Speed file cannot be opened.";
		return result;
	}
	//*************************************************
	result = "Success";
	return result;
}

void SM::loadData()
{
	std::string camberPath = pathName + ".camber";
	size_t found = pathName.find( "dhl" );
	if( found != std::string::npos)
	{
		pathName.erase( found, 3);
	}
	std::string speedPath = pathName + ".speed";

	_data = new SMObject*[3];
	_data[0] = new SMCamberObject( camberPath.c_str() );
	_data[1] = new SMFrameObject( _data[0]->total_Number() );
	_data[2] = new SMSpeedObject( speedPath.c_str() );

	order[0] = 0;
	order[1] = 1;
	order[2] = 2;
	return;
}

void SM::renderScene()
{
	glTranslatef( OFFSET, -15.0, 0.0 );
	renderBar();
	//renderFonts();
	//		enable blend
// 	glEnable(GL_BLEND);
// 	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    for( int i = 0; i < 2; i++ )
    {
        renderLinks( i );
    }
//	glDisable(GL_BLEND);
	glTranslatef( OFFSET, -15.0, 0.0 );
	//painter->setPen( QColor( 255, 255, 255 ) );
	//painter->drawText( QPointF( -_size/2, -_size/2 ), QObject::tr( "This is a test! " ) );
}

void SM::drawRect( int x, int y, int width, int height )
{
	glBegin(GL_POLYGON);
		glVertex2f( x, y);
		glVertex2f( x, y - height );
		glVertex2f( x + width, y);
	glEnd();

	glBegin(GL_POLYGON);
		glVertex2f( x, y - height );
		glVertex2f( x + width, y - height );
		glVertex2f( x + width, y );
	glEnd();
	return;
}

void SM::renderLinks(int index)
{
    float HeightL[50], HeightR[50];
    //          LEFT LINKS
	if( _data[order[index]]->getName() == "Camber" )
	{
		if( COLORSTATE != 0 )
		{
			for( int i = 0; i < 50; i++)
			{
				HeightL[i] = ( _data[order[index]]->getRealValueAt( i ) - this->getGlobalMin() ) / ( this->getGlobalMax() - this->getGlobalMin() );
				HeightL[i] = HEIGHT * HeightL[i] - HEIGHT/2;
				//HeightL[i] *= _scale;
			}
		}
		else
		{
			for( int i = 0; i < 50; i++)
			{
				HeightL[i] = _data[order[index]]->percentage_At(i);
				HeightL[i] = HEIGHT * HeightL[i] - HEIGHT/2;
				//HeightL[i] *= _scale;
			}
		}
	}
	else
	{
		for( int i = 0; i < 50; i++)
		{
			HeightL[i] = _data[order[index]]->percentage_At(i);
			HeightL[i] = HEIGHT * HeightL[i] - HEIGHT/2;
			//HeightL[i] *= _scale;
		}
	}
    //
	if( _data[order[index+1]]->getName() == "Camber" )
	{
		if( COLORSTATE != 0 )
		{
			for( int i = 0; i < 50; i++)
			{
				HeightR[i] = ( _data[order[index+1]]->getRealValueAt( i ) - this->getGlobalMin() ) / ( this->getGlobalMax() - this->getGlobalMin() );
				HeightR[i] = HEIGHT * HeightR[i] - HEIGHT/2;
				//HeightL[i] *= _scale;
			}
		}
		else
		{
			for( int i = 0; i < 50; i++)
			{
				HeightR[i] = _data[order[index+1]]->percentage_At(i);
				HeightR[i] = HEIGHT * HeightR[i] - HEIGHT/2;
				//HeightL[i] *= _scale;
			}
		}
	}
	else
	{
		for( int i = 0; i < 50; i++)
		{
			HeightR[i] = _data[order[index+1]]->percentage_At(i);
			HeightR[i] = HEIGHT * HeightR[i] - HEIGHT/2;
			//HeightR[i] *= _scale;
		}
	}
    //      DRAW THE LINES
    myColor3 color;
    for( int i = 0; i < 50; i++)
    {
		//			percentage, needs camber to color all the links
        //float PERCENTAGE = i/50.0;
		float PERCENTAGE;
		if( COLORSTATE == 0 )
		{
			PERCENTAGE = _data[0]->percentage_At(i);
		}
		else
		{
			PERCENTAGE = ( _data[0]->getRealValueAt( i ) - this->getGlobalMin() ) / ( this->getGlobalMax() - this->getGlobalMin() );
		}
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		//			high risk, need to be updated
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		if( PERCENTAGE < 0 )
		{
			PERCENTAGE = 0;
		}
		if( PERCENTAGE > 1 )
		{
			PERCENTAGE = 1;
		}
        Lab3D templab = cut->get_Position( PERCENTAGE );
        labobject->get_Color(templab, color);
		
		if ( needHighlighted(i) )
		{
			glColor4f(color.a/255.0, color.b/255.0, color.c/255.0, 0.3 );
		}
		else
		{
			glColor4f(color.a/255.0, color.b/255.0, color.c/255.0, 1.0 );
		}

		if( order[index] == order[_index] )
		{
			drawLine(  index * this->SPACE + barOffsetX, HeightL[i] - barOffsetY, (index+1) * this->SPACE, HeightR[i]);
		}
		else
		{
			if( order[index+1] == order[_index] )
			{
				drawLine(  index * this->SPACE, HeightL[i], (index+1) * this->SPACE + barOffsetX, HeightR[i] - barOffsetY);
			}
			else
			{
				drawLine(  index * this->SPACE, HeightL[i], (index+1) * this->SPACE, HeightR[i]);
			}
		}
		
    }
}

void SM::renderBar()
{
	//glColor4f(0.2, 0.6, 0.2, 1.0);
	for(int i = 0; i < 3; i++)
	{
		glColor4f(0.2, 0.6, 0.2, 1.0);
		if( _HIGHLIGHT == i )
		{
			glColor4f(0.9, 0.9, 0.1, 1.0);
		}
		if( _index != i)
		{
			drawRect( i*SPACE, HEIGHT/2, WIDTH, HEIGHT ); 
		}
		else
		{
			drawRect( i*SPACE + barOffsetX, HEIGHT/2 - barOffsetY, WIDTH, HEIGHT ); 
		}
	}
}

void SM::swap(int source, int target)
{
    int temp = order[source];
    order[source] = order[target];
    order[target] = temp;
}

void SM::drawLine(int x1, int y1, int x2, int y2)
{
	glBegin(GL_LINES);
		glVertex2i(x1, y1);
		glVertex2i(x2, y2);
	glEnd();
}

void SM::renderFonts()
{
	int tempHeight = _size_height / 2 - ( this->_size_height - this->HEIGHT ) / 2 + 15 ;
	this->displayFonts( 0, tempHeight, GLUT_BITMAP_HELVETICA_12, _data[order[0]]->getName().c_str(), 0.9, 0.9, 0.9, 0.9 );
	this->displayFonts( SPACE, tempHeight, GLUT_BITMAP_HELVETICA_12, _data[order[1]]->getName().c_str(), 0.9, 0.9, 0.9, 0.9 );
	this->displayFonts( SPACE*2, tempHeight, GLUT_BITMAP_HELVETICA_12, _data[order[2]]->getName().c_str(), 0.9, 0.9, 0.9, 0.9 );
	return;
}

void SM::renderFontWithPainter( QPainter* painter )
{
	painter->setPen( QColor( 0, 0, 0 ) );
	painter->setFont( QFont("Times", 12) );
	for( int i = 0; i < 3; i++ )
	{
		QRect rect = painter->fontMetrics().boundingRect( QObject::tr( _data[order[i]]->getName().c_str() ) );
		//int tempX = SPACE * i + OFFSET -_size/2;
		//int tempY = this->HEIGHT / 2 -_size/2;
		int tempX = SPACE * i + OFFSET - rect.width()/2;
		int tempY = -this->HEIGHT / 2 - rect.height()/2 + 7;
		if( _index == i)
		{
			tempX += barOffsetX;
			tempY += barOffsetY;
		}
		painter->drawText( QPointF( tempX, tempY ), QObject::tr( _data[order[i]]->getName().c_str() ) );
	}
	painter->setFont( QFont("Times", 8) );
	for( int i = 0; i < 3; i++ )
	{
		QRect rectMax = painter->fontMetrics().boundingRect( QString().setNum( _data[order[i]]->getMAX() )  );
		QRect rectMin = painter->fontMetrics().boundingRect( QString().setNum( _data[order[i]]->getMIN() )  );
		//int tempX = SPACE * i + OFFSET -_size/2;
		//int tempY = this->HEIGHT / 2 -_size/2;
		int tempXMax = SPACE * i + OFFSET - rectMax.width()/2;
		int tempYMax = -this->HEIGHT / 2 - rectMax.height()/2 + 15.0;
		int tempXMin = SPACE * i + OFFSET - rectMin.width()/2;
		int tempYMin = this->HEIGHT / 2 + rectMin.height() + 15.0;
		if( _index == i)
		{
			tempXMax += barOffsetX;
			tempYMax += barOffsetY;
			tempXMin += barOffsetX;
			tempYMin += barOffsetY;
		}
		if(_data[order[i]]->getName() == "Camber")
		{
			if( COLORSTATE == 0 )
			{
				painter->drawText( QPointF( tempXMax, tempYMax ), QString().setNum( _data[order[i]]->getMAX() ) );
				painter->drawText( QPointF( tempXMin, tempYMin ), QString().setNum( _data[order[i]]->getMIN() ) );
			}
			else
			{
				painter->drawText( QPointF( tempXMax, tempYMax ), QString().setNum( this->getGlobalMax() ) );
				painter->drawText( QPointF( tempXMin, tempYMin ), QString().setNum( this->getGlobalMin() ) );
			}
		}
		else
		{
			painter->drawText( QPointF( tempXMax, tempYMax ), QString().setNum( _data[order[i]]->getMAX() ) );
			painter->drawText( QPointF( tempXMin, tempYMin ), QString().setNum( _data[order[i]]->getMIN() ) );
		}
	}
	painter->setFont( QFont("Times", 12) );
	if( _HIGHLIGHT != -1 )
	{
		float percentage = 1.0 - ( this->_heightPercentage + HEIGHT/2 ) / HEIGHT;
		if( percentage < 1.0 && percentage >= 0.0 )
		{
			float data;
			if( this->_data[order[_HIGHLIGHT]]->getName() == "Camber" )
			{
				if( COLORSTATE != 0 )
				{
					data = ( this->getGlobalMax() - this->getGlobalMin() ) * percentage + this->getGlobalMin();
				}
				else
				{
					data = ( _data[order[_HIGHLIGHT]]->getMAX() - _data[order[_HIGHLIGHT]]->getMIN() ) * percentage + _data[order[_HIGHLIGHT]]->getMIN();
				}
			}
			else
			{
				data = ( _data[order[_HIGHLIGHT]]->getMAX() - _data[order[_HIGHLIGHT]]->getMIN() ) * percentage + _data[order[_HIGHLIGHT]]->getMIN();
			}
			QString dataString;
			dataString.setNum( data );
			if( dataString.size() > 5 )
			{
				dataString.resize( 5 );
			}
			painter->drawText( _HIGHLIGHT * SPACE + OFFSET , this->_heightPercentage, "<" + dataString );
		}
	}
	//_nameRect = 
	return;
}

void SM::size_hint( float size )
{
    if( size > 0.66)
    {
        font_index = 2;
    }
    else if(size < 0.33)
    {
        font_index = 0;
    }
    else
    {
        font_index = 1;
    }
    return;
}
void SM::displayFonts(float x, float y, void *font, std::string curname, float r, float g, float b, float a)
{
	char text[100];
	strcpy(text, curname.c_str());
	if(!text || !strlen(text)) 
		return; 
	bool blending = false; 
	if(glIsEnabled(GL_BLEND))
		blending = true; 
	glEnable(GL_BLEND); 
	glColor4f(r,g,b,a); 
	glRasterPos2f(x,y); 
	for(int i = 0; i < (int)curname.length(); i++)
	{ 
		glutBitmapCharacter(font, text[i]); 
		//text++; 
	} 
	if(!blending) 
		glDisable(GL_BLEND); 
}



int SM::mousePressEvent( int x, int y)
{
	x = x - OFFSET;
	int remainder = x%SPACE;
	if( remainder >= 0 && remainder < WIDTH )
	{
		_index = x - remainder;
		_index = _index/SPACE;
		initPos.setX( x + OFFSET);
		initPos.setY( y );
		return 1;
	}
	return -1;
}

void SM::mouseOn( int x, int y )
{
	x = x - OFFSET;
	int remainder = x%SPACE;
	if( remainder >= 0 && remainder < WIDTH )
	{
		_HIGHLIGHT = x - remainder;
		_HIGHLIGHT = _HIGHLIGHT/SPACE;
		//*****************************************
		_heightPercentage = y - OFFSET - HEIGHT/2;
	}
	else
	{
		_HIGHLIGHT = -1;
	}
}

int SM::mouseMoveEvent( int x, int y )
{
	barOffsetX = x - initPos.x();
	barOffsetY = y - initPos.y();
	return -1;
}

int SM::mouseReleaseEvent( int x, int y)
{
	x = x - OFFSET;
    int remainder = x%SPACE;
    if( remainder < WIDTH )
    {
        int tempIndex = x - remainder;
        if( tempIndex/SPACE != _index )
        {
             target = tempIndex/SPACE;
			 //swap( _index, target );
			 emit swapBars( this, _index, target );
			 _index = -1;
			 barOffsetX = 0;
			 barOffsetY = 0;
			 return 1;
        }
    }
	barOffsetX = 0;
	barOffsetY = 0;
	_index = -1;
	return -1;
}

SMDetail SM::getDetailAt( int index )
{
	SMDetail result;
	result.name = _data[index]->getName();
	result.max = _data[index]->getMAX();
	result.min = _data[index]->getMIN();
	return result;
}

int SM::mapFrame( int input )
{
	int result;
	int height = this->HEIGHT/2 - input;
	result = _data[1]->getMAX() * height / this->HEIGHT;
	return result;
}

QList<int> SM::findHighlight( QRect rect )
{
	QList<int> frameIndex;
	_INIT = false;
	highLightedList.clear();
	int selected = -1;
	int begin = TDHelper::Min( rect.topLeft().x(), rect.topRight().x() );
	int end = begin + abs( rect.width() );
	for(int i = 0; i < 3; i++)
	{
		int currentX = i*SPACE + OFFSET;
		if( currentX >= begin && currentX <= end )
		{
			selected = i;
		}
	}
	if( selected == -1 )
	{
		return frameIndex;
	}
	//			begin find
	if( this->_data[selected]->getType() == 2 )
	{
		int endFrame = this->mapFrame( rect.y() );
		int beginFrame = this->mapFrame( rect.y() + rect.height() );
		for( int i = beginFrame; i <= endFrame; i++ )
		{
			highLightedList.append( i );
			frameIndex.append( i );
		}
	}
	else
	if( this->_data[selected]->getType() == 1 || this->_data[selected]->getType() == 3 )
	{
		//		find max
		int maxHeight, minHeight;
		maxHeight = TDHelper::Max( rect.y(), rect.y() + rect.height() );
		minHeight = TDHelper::Min( rect.y(), rect.y() + rect.height() );
		//************************************************
		float curDataMAM = this->_data[selected]->getMIN() - (( this->HEIGHT/2.0f - minHeight ) * ( this->_data[selected]->getMIN() - this->_data[selected]->getMAX() ) / this->HEIGHT );
		float curDataMIN = this->_data[selected]->getMIN() - (( this->HEIGHT/2.0f - maxHeight ) * ( this->_data[selected]->getMIN() - this->_data[selected]->getMAX() ) / this->HEIGHT );
		//************************************************
		//maxHeight = 
		//max = this->HEIGHT/2 - rect.y();
		//max = _data[1]->getMAX() * max / this->HEIGHT;
		//min = this->HEIGHT/2 - rect.y() - rect.height();
		//min = _data[selected]->getMIN * min / this->HEIGHT; 
		//
		for( int i = 0; i < 50; i++)
		{
			float percent = _data[selected]->percentage_At(i);
			float currentValue = percent * ( _data[selected]->getMAX() - _data[selected]->getMIN() ) + _data[selected]->getMIN() ;
			if( currentValue >= curDataMIN && currentValue <= curDataMAM )
			{
				highLightedList.append( i );
				frameIndex.append( int( _data[1]->percentage_At(i) * ( _data[1]->getMAX() - _data[1]->getMIN() ) + _data[1]->getMIN() ));
			}
		}
	}
	return frameIndex;
}

bool SM::needHighlighted(int index)
{
	if( highLightedList.count() == 0 )
	{
		return false;
	}
	else
	{
		for( int i = 0; i < highLightedList.count(); i++ )
		{
			if( index == highLightedList[i] )
			{
				return true;
			}
		}
		return false;
	}
}