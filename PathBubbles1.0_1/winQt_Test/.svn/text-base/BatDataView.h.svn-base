#ifndef BATDATAVIEW_H
#define BATDATAVIEW_H

#include <QPainter>
#include "StatisticManager.h"

#define EACHHEIGHT 10

class BatDataView
{
public:
	BatDataView( StatisticManager* manager );
	~BatDataView();
	void paintDataView( QPainter *painter );
	void drawItems( QPainter *painter );
	void setPos( QPointF pos );
	void readParaName();
	void setBatName( QString str );
	void updateItems();

private:
	QList<QString> m_paraName;
	QPointF m_pos;
	QString batName;
	QList<QString> m_items;
	int m_width, m_height, m_len;
	StatisticManager* m_dataManager;
};
#endif

