#ifndef SPACEMANAGER_H
#define SPACEMANAGER_H

#include <QList>
#include <QPoint>
#include <QRectF>
#include <QVector>

class OpenGLScene;
class ItemBase;

#define STEP 200

class SpaceManager
{
public:
	SpaceManager( OpenGLScene* _scene );
	void needToOpen( int number );
	QVector<QRectF> queryPreOpen( QPointF pos, int num );
	QVector<QRectF> queryPreOpen( QPointF pos, int row, int col );
	QPoint getPosition();
private:
	OpenGLScene* _scene;
	QVector<QPoint> _posList;
};

#endif