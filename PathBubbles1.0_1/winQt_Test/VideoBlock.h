#ifndef VIDEOBUBBLE_H
#define VIDEOBUBBLE_H

#include "ControlBubble.h"
#include "VideoPlayer.h"
//#include "BatDataView.h"

//class BatDataView;

class VideoBlock : public ControlBubble
{
	Q_OBJECT
public:
	VideoBlock( int x, int y, OpenGLScene *scene, QString path = tr(""), float begin = 0.0, float end = 0.0 );
	myVideoPlayer* getPlayerWidget();
	static QString checkPath( QString path );
	void initUI();
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	void resizeItem(int size1, int size2 );
	QString getPath()
	{
		return _path;
	}
	void saveAsXmlNode( QDomDocument &doc, QDomElement &node );
	void recoverFromXmlNode( QDomElement node );
	
	void logOpen();
	
protected:
	void hoverMoveEvent(QGraphicsSceneHoverEvent *event);

signals:
	void updateLabelState( ItemBase *item );

public slots:
	void addBatWithVideo();
	void logPause();
	void logPlay();

private:
	void updateLabel();
	myVideoPlayer *_player;
	QString _path;
	float _begin, _end;

	bool _LABEL;

	//BatDataView* rawDataView;
	bool _RAWDATAVIEW;
	QRectF nameRect;
};
#endif
