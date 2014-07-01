#include "smitem.h"

const qreal ROTATE_SPEED_X = 30.0 / 1000.0;
const qreal ROTATE_SPEED_Y = 20.0 / 1000.0;
const qreal ROTATE_SPEED_Z = 40.0 / 1000.0;

smItem::smItem(int size, int x, int y) : m_size(size)
{
    m_image = QPixmap("/home/Hanyu/Pictures/1.gif");
    this->setFlag( QGraphicsItem::ItemIsMovable, true);
    this->setFlag( QGraphicsItem::ItemIsSelectable, true);
    this->setFlag( QGraphicsItem::ItemIsFocusable, true);
    this->setAcceptHoverEvents( true );
    this->setPos( x, y);
    m_startTime = QTime::currentTime();
}

smItem::~smItem()
{
    return;
}

QRectF smItem::boundingRect() const
{
    return QRectF( -m_size/2, -m_size/2, m_size, m_size);
}

void smItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rect = boundingRect().translated(pos());
    float width = float(painter->device()->width());
    float height = float(painter->device()->height());

    float left = 2.0f * float(rect.left()) / width - 1.0f;
    float right = 2.0f * float(rect.right()) / width - 1.0f;
    float top = 1.0f - 2.0f * float(rect.top()) / height;
    float bottom = 1.0f - 2.0f * float(rect.bottom()) / height;
    float moveToRectMatrix[] = {
        0.5f * (right - left), 0.0f, 0.0f, 0.0f,
        0.0f, 0.5f * (bottom - top), 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.5f * (right + left), 0.5f * (bottom + top), 0.0f, 1.0f
    };

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadMatrixf(moveToRectMatrix);
    gluPerspective(60.0, 1.0, 0.01, 10.0);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    //glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);

    glEnable(GL_TEXTURE_2D);

    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    float lightColour[] = {1.0f, 1.0f, 1.0f, 1.0f};
    float lightDir[] = {0.0f, 0.0f, 1.0f, 0.0f};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColour);
    glLightfv(GL_LIGHT0, GL_POSITION, lightDir);
    glEnable(GL_LIGHT0);

    glTranslatef(0.0f, 0.0f, -1.5f);
    glRotatef(ROTATE_SPEED_X * m_startTime.msecsTo(QTime::currentTime()), 1.0f, 0.0f, 0.0f);
    glRotatef(ROTATE_SPEED_Y * m_startTime.msecsTo(QTime::currentTime()), 0.0f, 1.0f, 0.0f);
    glRotatef(ROTATE_SPEED_Z * m_startTime.msecsTo(QTime::currentTime()), 0.0f, 0.0f, 1.0f);

    glColor4f(1.0f, 0.0f, 1.0f, 1.0);
    float mcolor[] = { 1.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mcolor);
    glutSolidTeapot(2.5);

    //glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glDisable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHT0);
    glDisable(GL_NORMALIZE);

    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    //          all about drawing pixel map
    /*QTransform oldTransform = painter->worldTransform();
    int dtMod = 2000;
    qreal amp = 0.002 * (dtMod < 1000 ? dtMod : 2000 - dtMod) - 1.0;
    qreal scale = 0.6 + 0.2 * amp * amp;
    painter->setWorldTransform(QTransform().rotate(15.0 * amp).scale(scale, scale), true);
    painter->drawPixmap(-m_size / 2, -m_size / 2, m_size, m_size, m_image);
    painter->setWorldTransform(oldTransform, false);*/

    boarder(painter, option, widget);
}

void smItem::boarder(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    //if (option->state & QStyle::State_Selected)
    //{
        painter->setRenderHint(QPainter::Antialiasing, true);
        if (option->state & QStyle::State_HasFocus)
            painter->setPen(Qt::yellow);
        else
            painter->setPen(Qt::white);
        painter->drawRect(boundingRect());

        painter->drawLine(m_size / 2 - 9, m_size / 2, m_size / 2, m_size / 2 - 9);
        painter->drawLine(m_size / 2 - 6, m_size / 2, m_size / 2, m_size / 2 - 6);
        painter->drawLine(m_size / 2 - 3, m_size / 2, m_size / 2, m_size / 2 - 3);

        painter->setRenderHint(QPainter::Antialiasing, false);
    //}
}
