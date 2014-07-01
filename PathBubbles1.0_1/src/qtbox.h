#ifndef QTBOX_H
#define QTBOX_H

#include <QtGui>
#include "3DHelper.h"
#include <QtGui/qvector3d.h>
#include <exception>
#include <QList>
#include <QtXml>
#include <QTextStream>
#include "glbuffers.h"
#include "TextNote.h"
#include "ItemLabel.h"

const int MAX_ITEM_SIZE = 800;
const int MIN_ITEM_SIZE = 100;
const int MATSIZE = 260;
const int BOARDERSIZE = 6;
const int HALOWIDTH = 16;

//			type of object
#define BAT		1
#define PC		2
#define COMPLOT	3
#define MATLAB	4
#define WEB		5
#define COLOR	6
#define WAKE	7
#define VTK		8
#define VIDEO	9
#define NOTE	10
#define PEN		11
#define VOLUM	12
#define CON		13
#define IMAGE	14
#define DATAPLOT 15
#define BARCHART 17
#define LINECHART 18
#define SCATTER 19


class ItemBase :  public QObject, public QGraphicsItem
{
	Q_OBJECT
		Q_INTERFACES( QGraphicsItem )

public:
	enum { Type = UserType + 1 };
	ItemBase(int size, int x, int y);
	void finishInit();
	bool isDelegated()
	{
		return this->DELEGATE;
	}
	void setDelegate( bool bit )
	{
		this->DELEGATE = bit;
	}
	void hightLight( myColor3 hiColor )
	{
		this->_highLight = true;
		this->_hiColor = hiColor;
	}
	void unHighLight()
	{
		this->_highLight = false;
	}
	int Width()
	{
		return m_size_width;
	}
	int Height()
	{
		return m_size_height;
	}
	QPointF getInitPos()
	{
		return _initPos;
	}
	void properRemove()
	{
		emit itemRemoved( this );
	}
	myColor3 getColor()
	{
		return this->_colorBoarder;
	}
	void setColor( myColor3 input )
	{
		this->_colorBoarder = input;
		if (this->_label)
		{
			this->_label->setLabelColor( input );
		}
	}
	int getInitSize()
	{
		return _initSize;
	}
	bool needRemoveControl()
	{
		return this->_REMOVECONTROL;
	}
	bool canBeShift()
	{
		return this->_CANBESHIFTED;
	}
	void addToGroup( ItemBase* target );
	virtual ~ItemBase();
	virtual void calculateForces();
	virtual void resizeItem( int size1, int size2 );
	virtual bool collision( ItemBase* node );
	virtual void tighten(QList<ItemBase*> groupMember);
	virtual void disperse(QList<ItemBase*> groupMember);
	virtual bool needtoTighten( ItemBase* node);
	virtual bool advance();
	virtual QRectF boundingRect() const;
	QRect realRect();
	virtual int half( int input )
	{
		return (int)((float)input / 2.0 + 0.5);
	}
	virtual bool between( int tar, int left, int right)
	{
		if( tar > left && tar < right)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	virtual bool outRange(int tar, int left, int right)
	{
		if (tar < left && tar > right)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	virtual bool inBox(QPointF cur, int width1, int height1, QPointF tar, int widht2, int height2 );
	virtual bool inBox(QPointF cur, int cur_size, QPointF tar, int tar_size);
	virtual bool outBox(QPointF cur, int width1, int height1, QPointF tar, int widht2, int height2 );
	virtual bool outBox(QPointF cur, int cur_size, QPointF tar, int tar_size);
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	virtual int getType()
	{
		return _TYPE;
	}
	virtual QGraphicsProxyWidget* getControl()
	{
		return _myControl;
	}
	void removeControl()
	{
		delete _myControl;
	}
	void mySetPos( QPointF pos );
	bool onHalo( QPointF pos );
	QPointF LeftTop()
	{
		QPointF tmp;
		tmp = this->pos() + this->boundingRect().topLeft();

		return tmp;
	}
	ItemLabel* getLabel()
	{
		return this->_label;
	}
	QPointF getTargetPos()
	{
		return this->_targetPos;
	}
	void setTargetPos( QPointF pos )
	{
		_targetPos = pos;
	}
	void adjustPosLineUp();
	void setBoarderLeftFlag( bool flag )
	{
		boaderLeft = flag;
	}
	void setBoarderRightFlag( bool flag )
	{
		boarderRight = flag;
	}
	void setBoarderUpFlag( bool flag )
	{
		boarderUp = flag;
	}
	void setBoarderDownFlag( bool flag )
	{
		boarderDown = flag;
	}
	QString posString()
	{
		QString str, tmp;
		str = "(";
		tmp.setNum( this->pos().x() );
		str.append( tmp );
		str.append( ", ");
		tmp.setNum( this->pos().y() );
		str.append( tmp );
		str.append( ")" );
		return str;
	}
	QString sizeString()
	{
		QString str, tmp;
		str = "(";
		tmp.setNum( this->Width() );
		str.append( tmp );
		str.append( ", ");
		tmp.setNum( this->Height() );
		str.append( tmp );
		str.append( ")" );
		return str;
	}

	virtual void saveAsXmlNode( QDomDocument &doc, QDomElement &node );
	virtual void recoverFromXmlNode( QDomElement node );
	QRegion AllRegion();
	QRegion roundRect( QRect& rect, int radius );
	QString BubbleIdentifier();

	virtual void logOpen();
	virtual void logRemove();
	virtual void logMoveTo();
	virtual void logResizeTo();
	virtual void logUnGroup();
	QString headofLogMsg();
	QString tailofLogMsg();

signals:
	void itemPosUpdated( ItemBase* init );
	void unGroup( ItemBase* init );
	void itemPosMoved( ItemBase* init, QPointF offset );
	void itemRemoved( ItemBase* init);
	void logInfor( QString str );

protected:
	bool onUngroup( QPointF pos );
	void ControlAutoHide();
	void drawHalo( QPainter* painter );
	virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	virtual void keyPressEvent(QKeyEvent *event);
	virtual void wheelEvent(QGraphicsSceneWheelEvent *event);
	virtual bool onBoarder( QPointF pos );
	bool isInResizeArea(const QPointF &pos);
	bool isInLabelArea(const QPointF &pos);
	void drawControlHint( QPainter *painter );

	static void deleteSelectedItems(QGraphicsScene *scene);
	static void growSelectedItems(QGraphicsScene *scene);
	static void shrinkSelectedItems(QGraphicsScene *scene);
	//
	bool onMove( QPointF pos );
	bool onControl( QPointF pos );
	bool _Base_ONMOVE;
	bool _Base_ONCONTROL;
	bool _REMOVECONTROL;
	bool _CANBESHIFTED;
	void initControlsAnimation( QPointF pos );
	QTime m_startTime;
	bool m_isResizing;
	int m_size_width;
	int m_size_height;
	int _TYPE;
	QPointF newPos;
	QGraphicsProxyWidget* _myControl;
	myColor3 _colorBoarder;
	TextNote* _note;
	int _initSize;
	int _initSize2;
	QPointF _initPos;
	bool _highLight;
	bool LASTOPENSTATE;
	bool CONTROLLOCK;
	myColor3 _hiColor;
	//
	bool UNGROUP;
	bool DELEGATE;

	ItemLabel* _label; 
	QPointF _targetPos;
	bool boaderLeft, boarderRight, boarderUp, boarderDown;

	int bIndex;
};

class QtBox : public ItemBase
{
public:
	QtBox(int size, int x, int y);
	virtual ~QtBox();
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
protected:
private:
	QVector3D m_vertices[8];
	QVector3D m_texCoords[4];
	QVector3D m_normals[6];
	GLTexture *m_texture;
};

class CircleItem : public ItemBase
{
public:
	CircleItem(int size, int x, int y);
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
protected:
	QColor m_color;
};

class SquareItem : public ItemBase
{
public:
	SquareItem(int size, int x, int y);
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
protected:
	QPixmap m_image;
};

#endif
