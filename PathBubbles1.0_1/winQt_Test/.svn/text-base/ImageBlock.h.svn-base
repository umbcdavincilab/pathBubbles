#ifndef IMAGEBLOCK_H
#define IMAGEBLOCK_H

#include "ControlBubble.h"
#include "ImagePlayer.h"

class ImageBlock : public ControlBubble
{
public:
	ImageBlock( int x, int y, OpenGLScene* scene, QString path, float rate );
	void initUI();
	void setFrameByPercentate( float rate );
	void logOpen();

private:
	ImagePlayer* imagePlayer;
	QString _path;
};

#endif