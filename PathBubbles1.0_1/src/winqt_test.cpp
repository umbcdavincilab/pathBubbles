#include "winqt_test.h"
#include <QGLFormat>
#include <QLCDNumber>
#include <QScrollArea>
#include <QTreeWidgetItem>
#include <QFileDialog>
#include "GLViewport.h"

winQt_Test::winQt_Test(QWidget *parent, Qt::WFlags flags) : QWidget( parent, flags )
{
	//ui.setupUi(this);
	//			initialize the data
	lab = new LabObject();
	cut = new LabCut(50, 35, 70, 100);
	//bat = new Bat(lab, cut, 20);
	QGridLayout *buttonLayout = CreateUI();
	QGridLayout* mainLayout = new QGridLayout;
	mainLayout->addLayout(buttonLayout, 0, 0, 1, 2);

	setLayout(mainLayout);
	setWindowTitle(tr("New window"));
}

QGridLayout* winQt_Test::CreateUI()
{
	QGridLayout* layout = new QGridLayout;
	//		initialize the context
	GraphicsView *view;
	view = new GraphicsView();
	view->setViewport( new GLViewport() );
	//view->setViewport(new QGLWidget( QGLFormat(QGL::DoubleBuffer )));
	view->setViewportUpdateMode( QGraphicsView::FullViewportUpdate );
	view->updateViewport();

	OpenGLScene* scene = new OpenGLScene(lab, cut );
	view->setScene(scene );
	view->show();

	connect( this, SIGNAL( hasClosed() ), scene, SLOT( appHasClosed() ) );

	layout->addWidget( view );
	return layout;
}

void winQt_Test::closeEvent( QCloseEvent *event )
{
	if ( maybeSave() )
	{
		event->accept();
	}
	else
	{
		event->ignore();
	}
}

bool winQt_Test::maybeSave()
{
	emit hasClosed();

	return true;
}

void winQt_Test::resizeEvent( QResizeEvent *event )
{

}