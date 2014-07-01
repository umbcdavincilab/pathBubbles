//#include "vtkBlock.h"
//#include "openglscene.h"
//
//#define VTK_CREATE(type, name) \
//  vtkSmartPointer<type> name = vtkSmartPointer<type>::New()
//
//VTKBlock::VTKBlock(int x, int y, OpenGLScene *Scene) : ItemBase( MATSIZE, x, y)
//{
//	this->_scene = Scene;
//	this->_TYPE = MATLAB;
//	initUI();
//	this->_myControl->setPos( x - MATSIZE/2, y - MATSIZE/2 );
//	return;
//}
//
//void VTKBlock::initUI()
//{
//	//	init UI
//	//QWidget* widget = TDHelper::createDialog( tr("") );
//	//	init VTK
//	_vtk = new QVTKWidget( 0, Qt::CustomizeWindowHint | Qt::WindowTitleHint);
//	this->_vtk->show();
//	//vtkTextSource *text = vtkTextSource::New();
// //   text->SetText("Hello World!");
// //   text->BackingOff();
// //   vtkVectorText *vectorText = vtkVectorText::New();
// //   vectorText->SetText("QVTKWidget");
// //   vtkPolyDataMapper *textMapper = vtkPolyDataMapper::New();
// //   textMapper->SetInput(text->GetOutput());
// //   vtkPolyDataMapper *vectorTextMapper = vtkPolyDataMapper::New();
// //   vectorTextMapper->SetInput(vectorText->GetOutput());
// //   vtkActor *textActor = vtkActor::New();
// //   textActor->SetMapper(textMapper);
// //   vtkActor *vectorTextActor = vtkActor::New();
// //   vectorTextActor->SetMapper(vectorTextMapper);
// //   vtkRenderer *renderer = vtkRenderer::New();
// //   renderer->SetBackground(0.0,0.0,0.8);
// //   renderer->AddActor(textActor);
// //   renderer->AddActor(vectorTextActor);
// //   vtkRenderWindow *renderWindow = vtkRenderWindow::New();
// //   renderWindow->AddRenderer(renderer);
// //   renderWindow->SetStereoTypeToDresden();
//	//this->_vtk->SetRenderWindow(renderWindow);
//	//
//	//widget->layout()->addWidget( this->_vtk );
//	//	add VTK
//	this->_myControl = this->_scene->addWidget( _vtk );
//	this->_myControl->resize( VTKSIZE, VTKSIZE);
//	return;
//}
//
//void VTKBlock::mousePressEvent( QGraphicsSceneMouseEvent *event )
//{
//	if( onBoarder( event->pos() ) )
//	{
//		ItemBase::mousePressEvent( event );
//	}
//	else
//	{
//		//vtk->mousePressEvent( event );
//		event->setAccepted( false );
//	}
//	return;
//}
//
//void VTKBlock::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
//{
//	if( onBoarder( event->pos() ) )
//	{
//		ItemBase::mouseReleaseEvent( event );
//	}
//	else
//	{
//		//vtk->mouseReleaseEvent( event );
//		event->setAccepted( false );
//	}
//	//
//	return;
//}
//
//void VTKBlock::mouseMoveEvent( QGraphicsSceneMouseEvent *event )
//{
//	if( onBoarder( event->pos() ) )
//	{
//		ItemBase::mouseMoveEvent( event );
//	}
//	else
//	{
//		//vtk->mouseMoveEvent( event );
//		event->setAccepted( false );
//	}
//	return;
//}
//
//void VTKBlock::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
//{
//	this->_myControl->resize( this->Size(), this->Size() );
//	this->_myControl->setPos( this->pos().x() - this->Size()/2, this->pos().y() - this->Size()/2 );
//	ItemBase::paint( painter, option, widget );
//}
//
//void VTKBlock::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
//{
//	ItemBase::hoverMoveEvent( event);
//	return;
//}