#ifndef BUBBLEINSTANCE_H
#define BUBBLEINSTANCE_H

#include <QString>
#include <QList>
#include <QPainter>

//enum View
//{
//	BAT, PC1, PC2, PC3, VIDEOX, VIDEOY, VIDEOZ, CURVE
//};

class BubbleInstance
{
public:
	BubbleInstance( QString ind, QString run, QString view );
	QString getDiscreption( int index );
private:
	QString _ind;
	QString _run;
	QString _spe;
};
#endif