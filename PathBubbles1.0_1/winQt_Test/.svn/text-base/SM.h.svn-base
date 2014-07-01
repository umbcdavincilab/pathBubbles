#ifndef SM_H
#define SM_H

#include <QObject>
#include <QPainter>
#include <QFile>
#include <math.h>
#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <gl\freeglut.h>
#include <QPoint>
#include "3DHelper.h"
#include "SMOBject.h"
#include "LabCut.h"
#include "LabObject.h"

#define BIG 2
#define MID 1
#define SMALL 0

struct SMDetail
{
	float max;
	float min;
	std::string name;
};

class SM : public QObject
{
	Q_OBJECT

public:
	SM(std::string pathName, int size );
	static QString checkPath( QString name );
	static float getGlobalMax()
	{
		return 0.263157;
	}
	static float getGlobalMin()
	{
		return 0.008116;
	}
	void loadData();
	int mapFrame( int input );
	std::string getName()
	{
		return _name;
	}
	void updateSize( int size_width, int size_height )
	{
		_size_width = size_width;
		_size_height = size_height;
		initParameters();
	}
	float getCamberBoundary( int type){ return boundaryCamber[type]; }
	float getSpeedBoundary( int type ){ return boundarySpeed[type]; }
	float getCamberValue( int index){ return Fcamber[index]; }
	float getSpeedValue( int index ){ return Fspeed[index]; }
	int mousePressEvent( int x, int y );
	void mouseOn( int x, int y );
	int mouseMoveEvent( int x, int y );
	int mouseReleaseEvent( int x, int y );
	void setColor(LabCut* input1, LabObject* input2)
	{
		cut = input1;
		labobject = input2;
	}
	void renderFontWithPainter( QPainter* painter );
	void renderScene();
	void renderFonts();
	void renderBar();
	void renderLinks();
	void renderLinks( int index);
	void displayFonts( float x, float y, void *font, std::string text, float r, float g, float b, float a );
	void swap( int source, int target);
	void size_hint( float percentage );
	void drawRect( int x, int y, int width, int height );
	SMDetail getDetailAt( int index );
	QList<int> findHighlight( QRect rect );
	bool needHighlighted( int index );
	void setColorState( int input )
	{
		COLORSTATE = input;
	}
signals:
	void swapBars( SM* sm, int source, int target );
private:
	void initParameters();
	QRect _nameRect;
	std::string _name;
	SMObject **_data;
	int font_index;
	int barOffsetX, barOffsetY;
	int OFFSET;
	int _size_width, _size_height;
	std::string pathName;
	std::string perfix;
	float camber[60];
	float Fcamber[50];
	float boundaryCamber[2];
	int num_Camber;
	float speed[60];
	float Fspeed[50];
	float boundarySpeed[2];
	int num_Speed;
	int SPACE, WIDTH, HEIGHT;
	bool _INIT;
	LabCut* cut;
	LabObject* labobject;
	int target, _index;
	int order[3];
	QPoint initPos;
	int _scale;
	int _HIGHLIGHT;
	QList< int > highLightedList;
	int COLORSTATE;	// 0 for in
	void drawLine(int x1, int y1, int x2, int y2);
	float _heightPercentage;
};

#endif
