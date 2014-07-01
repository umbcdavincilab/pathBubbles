#include "ImageWidget.h"

#include "videowidgetsurface.h"

#include <QtMultimedia>

ImageWidget::ImageWidget(QWidget *parent)
    : QWidget(parent)
    , surface(0)
{
    setAutoFillBackground(false);
    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_PaintOnScreen, true);

    QPalette palette = this->palette();
    palette.setColor(QPalette::Background, Qt::black);
    setPalette(palette);

    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    surface = new VideoWidgetSurface(this);
}

ImageWidget::~ImageWidget()
{
    delete surface;
}

QSize ImageWidget::sizeHint() const
{
    //return surface->surfaceFormat().sizeHint();
	return this->size();
}

void ImageWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    if (surface->isActive()) {
        const QRect videoRect = surface->videoRect();

        if (!videoRect.contains(event->rect())) {
            QRegion region = event->region();
            region.subtract(videoRect);

            QBrush brush = palette().background();

            foreach (const QRect &rect, region.rects())
                painter.fillRect(rect, brush);
        }

        surface->paint(&painter);
    } else {
        painter.fillRect(event->rect(), palette().background());
    }
}

void ImageWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    surface->updateVideoRect();
}