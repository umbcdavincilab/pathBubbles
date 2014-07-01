
#include "openglscene.h"
#include "model.h"
#include "smitem.h"
#include "qtbox.h"

#include <QtGui>
#include <QtOpenGL>
#include <QFileDialog>

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

//const int MAX_ITEM_SIZE = 512;
//const int MIN_ITEM_SIZE = 16;

QDialog* OpenGLScene::createDialog(const QString &windowTitle) const
{
    QDialog *dialog = new QDialog(0, Qt::CustomizeWindowHint | Qt::WindowTitleHint);

    dialog->setWindowOpacity(0.8);
    dialog->setWindowTitle(windowTitle);
    dialog->setLayout(new QVBoxLayout);

    return dialog;
}

OpenGLScene::OpenGLScene()
    : m_wireframeEnabled(false)
    , m_normalsEnabled(false)
    , m_modelColor(153, 255, 0)
    , m_backgroundColor(20, 20, 20)
    , m_model(0)
    , m_lastTime(0)
    , m_distance(1.4f)
    , m_angularMomentum(0, 40, 0)
{


    lab = new LabObject();
    cut = new LabCut(45, 35, 75, 100);
    ani = new animateIt( 500);

    initializeUI();

    //smItem* item;
    //item = new smItem(100, 40, 40);
    //addItem(item);
    //QtBox* box;
    //box = new QtBox( 100, 100, 100);
    //addItem( box );
    //Controls->move( 0, this->height()-Controls->height());
    m_time.start();
}

void OpenGLScene::initializeUI()
{
    STATE = 1;

    QWidget *controls = createDialog(tr("Controls"));
    Controls = controls;
    QGridLayout* layout = new QGridLayout;

    m_modelButton = new QPushButton(tr("Add view"));
    connect(m_modelButton, SIGNAL(clicked()), this, SLOT(addView()));
    //controls->layout()->addWidget(m_modelButton);
    layout->addWidget(m_modelButton, 0, 0);

    QPushButton *backgroundButton = new QPushButton(tr("Background color"));
    connect(backgroundButton, SIGNAL(clicked()), this, SLOT(setBackgroundColor()));
    layout->addWidget(backgroundButton, 1, 0);

    QPushButton *orderButton = new QPushButton(tr("Order views"));
    connect(orderButton, SIGNAL(clicked()), this, SLOT(orderViews()));
    layout->addWidget(orderButton, 2, 0);

    QPushButton *controlButton = new QPushButton(tr("Hide"));
    connect(controlButton, SIGNAL(clicked()), this, SLOT(switchUI()));
    layout->addWidget(controlButton, 1, 1);

    layout->update();
    delete controls->layout();
    controls->setLayout( layout );

    addWidget(controls);
    // move UI
    QPointF pos(0, this->height() - controls->height());
    foreach (QGraphicsItem *item, items())
    {
        item->setFlag(QGraphicsItem::ItemIsMovable);
        item->setCacheMode(QGraphicsItem::DeviceCoordinateCache);

        const QRectF rect = item->boundingRect();
        item->setPos(pos.x() - rect.x(), pos.y() - rect.y());
        pos += QPointF(0, 10 + rect.height());
    }
    SHIFT = controlButton->pos().x();
    return;
}

void OpenGLScene::drawBackground(QPainter *painter, const QRectF &)
{
    //if (painter->paintEngine()->type() != QPaintEngine::OpenGL)
    //{
        //qWarning("OpenGLScene: drawBackground needs a QGLWidget to be set as viewport on the graphics view");
        //return;
    //}

    glClearColor(m_backgroundColor.redF(), m_backgroundColor.greenF(), m_backgroundColor.blueF(), 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    this->updateScene();
    QTimer::singleShot(20, this, SLOT(update()));
}

void OpenGLScene::updateScene()
{
    if( ani->isAnimated() )
    {
        Controls->move( ani->animateP());
        return;
        //updateScene();
        //update(this)
    }
    else
    {
        if( STATE == 1)
        {
            Controls->move( 0, this->height()-Controls->height());
        }
        else
        {
            Controls->move( -SHIFT, this->height()-Controls->height());
        }
    }
}

void OpenGLScene::switchUI()
{
    if( STATE == 0)
    {
        ani->start( QPoint(-SHIFT, this->height()-Controls->height()), QPoint(0, this->height()-Controls->height()), 0, 0);
        STATE = 1;
    }
    else
    {
        ani->start( QPoint(0, this->height()-Controls->height()), QPoint(-SHIFT, this->height()-Controls->height()), 0, 0);
        STATE = 0;
    }
}

void OpenGLScene::addView()
{
    QString dir = QFileDialog::getOpenFileName( NULL , tr("Open File"), "./", tr("Camber (*.camber)"));
    if( dir.length() < 1)
    {
        return;
    }
    dir.remove( dir.length() - 7, 7);
    SM* sm = new SM( dir.toStdString() );
    sm->size_hint( (float)100/(float)MAX_ITEM_SIZE );
    sm->setColor(cut, lab);

    QtSM* qtsm;
    qtsm = new QtSM( 100, 100, 100);
    qtsm->setSM( sm );
    addItem( qtsm );
}

void OpenGLScene::orderViews()
{
    QList<QGraphicsItem *> list = items();
    //int length = list.length();
    int length = 0;

    float width = 0, height = 0;
    foreach (QGraphicsItem *item, items())
    {
        ItemBase *itemBase = qgraphicsitem_cast<ItemBase *>(item);
        if (itemBase)
        {
            width += itemBase->Size();
            length += 1;
        }
        //height = item->boundingRect().height();
    }
    int Width = (int)(width/(float)length + 0.5);
    int parent_Width = this->width();
    int num_Wid = (float)this->width() / Width;
    int index = 0;
    foreach (QGraphicsItem *item, items())
    {
        QtSM *itemBase = qgraphicsitem_cast<QtSM *>(item);
        if (itemBase)
        {
            //itemBase->prepareGeometryChange();
            //itemBase->m_size = Width;
            itemBase->update(itemBase->boundingRect());
            itemBase->setAnimation( QPoint((index % num_Wid) * Width + Width/2 ,(index - (index % num_Wid))/num_Wid*Width + Width/2), Width);
            //itemBase->setPos( (index % num_Wid) * Width + Width/2 ,(index - (index % num_Wid))/num_Wid*Width + Width/2);
            //itemBase->setPos( (index - (index % num_Wid))/num_Wid*Width, (index % num_Wid) * Width);
            index += 1;
        }
    }
}

void OpenGLScene::setBackgroundColor()
{
    const QColor color = QColorDialog::getColor(m_backgroundColor);
    if (color.isValid()) {
        m_backgroundColor = color;
        update();
    }
}

void OpenGLScene::resizeEvent(QResizeEvent *event)
{
}

void OpenGLScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseMoveEvent(event);
    if (event->isAccepted())
        return;
    if (event->buttons() & Qt::LeftButton) {
        const QPointF delta = event->scenePos() - event->lastScenePos();
        const Point3d angularImpulse = Point3d(delta.y(), delta.x(), 0) * 0.1;

        m_rotation += angularImpulse;
        m_accumulatedMomentum += angularImpulse;

        event->accept();
        update();
    }
}

void OpenGLScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mousePressEvent(event);
    if (event->isAccepted())
        return;

    m_mouseEventTime = m_time.elapsed();
    m_angularMomentum = m_accumulatedMomentum = Point3d();
    event->accept();
}

void OpenGLScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseReleaseEvent(event);
    if (event->isAccepted())
        return;

    const int delta = m_time.elapsed() - m_mouseEventTime;
    m_angularMomentum = m_accumulatedMomentum * (1000.0 / qMax(1, delta));
    event->accept();
    update();
}

void OpenGLScene::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    QGraphicsScene::wheelEvent(event);
    if (event->isAccepted())
        return;

    m_distance *= qPow(1.2, -event->delta() / 120);
    event->accept();
    update();
}
