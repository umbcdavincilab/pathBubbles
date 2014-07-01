#ifndef CONTROLMANAGER_H
#define CONTROLMANAGER_H

#include <QGraphicsItem>
#include <QGraphicsProxyWidget>

class ControlManager
{
public:
	ControlManager();
	void addControl( QGraphicsItem* item, QGraphicsProxyWidget* floatingControlProxy );
	void showControl( QGraphicsItem* item );
	void hideControl( QGraphicsItem* item );
private:
	int _index;
	QGraphicsItem* _item[100];
	QGraphicsProxyWidget* _floatingControlProxy[100];
};

#endif