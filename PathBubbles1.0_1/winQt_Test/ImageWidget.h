#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include "videowidgetsurface.h"

#include <QtGui/QWidget>

class QAbstractVideoSurface;

class VideoWidgetSurface;

class ImageWidget : public QWidget
{
    Q_OBJECT
public:
    ImageWidget(QWidget *parent = 0);
    ~ImageWidget();

    QAbstractVideoSurface *videoSurface() const { return surface; }

    QSize sizeHint() const;

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

private:
    VideoWidgetSurface *surface;
};

#endif