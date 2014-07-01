#ifndef SPACEMANAGER_H
#define SPACEMANAGER_H

#include <QList>
#include <QPoint>
#include <QRectF>
#include <QVector>
//#include "subPathBubble1.h"

class OpenGLScene;
class ItemBase;

#define STEP 400

class SpaceManager
{
public:
	SpaceManager( OpenGLScene* _scene );
	void needToOpen( int number );
	QVector<QRectF> queryPreOpen( QPointF pos, int num );
	QVector<QRectF> queryPreOpen( QPointF pos, int row, int col );
	QPoint getPosition(); //old
	//QPoint getPosition(ItemBase *newitem, int x, int y, int width, int height); //Keqin // reassign a position if the newly created item overlap other position/ do not add to group since it is just created
	

private:
	OpenGLScene* _scene;
	QVector<QPoint> _posList;
};

#endif