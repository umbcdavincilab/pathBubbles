#include "VideoWidget.h"
#include <QtMultimedia>

 VideoWidget::VideoWidget(QWidget *widget, QObject *parent)
     : QAbstractVideoSurface(parent)
     , widget(widget)
     , imageFormat(QImage::Format_Invalid)
 {
 }

 QList<QVideoFrame::PixelFormat> VideoWidget::supportedPixelFormats(
         QAbstractVideoBuffer::HandleType handleType) const
 {
     if (handleType == QAbstractVideoBuffer::NoHandle)
	 {
         return QList<QVideoFrame::PixelFormat>()
                 << QVideoFrame::Format_RGB32
                 << QVideoFrame::Format_ARGB32
                 << QVideoFrame::Format_ARGB32_Premultiplied
                 << QVideoFrame::Format_RGB565
                 << QVideoFrame::Format_RGB555;
     } 
	 else {
         return QList<QVideoFrame::PixelFormat>();
     }
 }

 bool VideoWidget::isFormatSupported(
         const QVideoSurfaceFormat &format, QVideoSurfaceFormat *similar) const
 {
     Q_UNUSED(similar);

     const QImage::Format imageFormat = QVideoFrame::imageFormatFromPixelFormat(format.pixelFormat());
     const QSize size = format.frameSize();

     return imageFormat != QImage::Format_Invalid
             && !size.isEmpty()
             && format.handleType() == QAbstractVideoBuffer::NoHandle;
 }

 bool VideoWidget::start(const QVideoSurfaceFormat &format)
 {
     const QImage::Format imageFormat = QVideoFrame::imageFormatFromPixelFormat(format.pixelFormat());
     const QSize size = format.frameSize();

     if (imageFormat != QImage::Format_Invalid && !size.isEmpty()) 
	 {
         this->imageFormat = imageFormat;
         imageSize = size;
         sourceRect = format.viewport();

         QAbstractVideoSurface::start(format);

         widget->updateGeometry();
         updateVideoRect();

         return true;
     }
	 else
	 {
         return false;
     }
 }

 void VideoWidget::stop()
 {
     currentFrame = QVideoFrame();
     targetRect = QRect();

     QAbstractVideoSurface::stop();

     widget->update();
 }

 bool VideoWidget::present(const QVideoFrame &frame)
 {
     if (surfaceFormat().pixelFormat() != frame.pixelFormat()
             || surfaceFormat().frameSize() != frame.size()) {
         setError(IncorrectFormatError);
         stop();

         return false;
     } 
	 else 
	 {
         currentFrame = frame;

         widget->repaint(targetRect);

         return true;
     }
 }

 void VideoWidget::updateVideoRect()
 {
     QSize size = surfaceFormat().sizeHint();
     //size.scale( widget->size().boundedTo(size), Qt::KeepAspectRatio );
	 size.scale( widget->size(), Qt::KeepAspectRatio );

     targetRect = QRect(QPoint(0, 0), size);
     targetRect.moveCenter(widget->rect().center());
 }

 void VideoWidget::paint(QPainter *painter)
 {
     if (currentFrame.map(QAbstractVideoBuffer::ReadOnly)) {
         const QTransform oldTransform = painter->transform();

         if (surfaceFormat().scanLineDirection() == QVideoSurfaceFormat::BottomToTop) {
            painter->scale(1, -1);
            painter->translate(0, -widget->height());
         }

         QImage image(
                 currentFrame.bits(),
                 currentFrame.width(),
                 currentFrame.height(),
                 currentFrame.bytesPerLine(),
                 imageFormat);

         painter->drawImage(targetRect, image, sourceRect);

         painter->setTransform(oldTransform);

         currentFrame.unmap();
     }
 }