#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>
#include <QResizeEvent>
#include <QGLWidget>

class GraphicsView : public QGraphicsView
{
public:
    GraphicsView()
    {
        setWindowTitle(tr("Viewer"));
        setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    }
	QSize minimumSizeHint() const
	{
		return QSize( 600, 600);
	}
	QSize sizeHint() const
	{
		return QSize( 600,800);
	}
	void updateViewport() 
    {
        QGLWidget* glWidget = dynamic_cast<QGLWidget*>( this->viewport() ); 
		if (glWidget)
		{
            glWidget->updateGL();
		}
    }

protected:
    void resizeEvent(QResizeEvent *event)
    {
		if (scene())
		{
			scene()->setSceneRect(QRect(QPoint(0, 0), event->size()));
		}
 
        QGraphicsView::resizeEvent(event);
    }
};

#endif // GRAPHICSVIEW_H
