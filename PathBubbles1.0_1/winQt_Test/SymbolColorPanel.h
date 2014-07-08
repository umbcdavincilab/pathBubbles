#ifndef SYMBOLCOLORPANEL_H
#define SYMBOLCOLORPANEL_H

//#include "qtbox.h"
#include "BarBase.h"
#include "OpenManager.h"
#include "InstanceManager.h"
#include "BubbleInstance.h"
#include "BubbleNode.h"
#include "InstanceInitializer.h"
#include "PenItem.h"
#include "FileItem.h"
#include <QImage>
#include "3DHelper.h"
//#include "TreeRing.h"


//class TreeRing;
class OpenGLScene;
class OpenManager;
//class ItemBase;

class SymbolColorPanel : public BarBase
{
	Q_OBJECT
public:
	SymbolColorPanel(  QSizeF size, int px, int py, OpenGLScene* scene, OpenManager* open );
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

	QString tailofLogMsg();

	QRegion roundRect(QRect &rect, int radius);
	void drawArrow_4(QPainter *painter, QPointF start, QPointF end, float width, float height, QColor color);
	void drawAFan(QPainter *painter, QPoint center, float radius1, float radius2, float angle1, float angle2, QColor color, int fanShape=0);
	void drawAFan(QPainter *painter, QPoint center, float radius1, float radius2, float angle1, float angle2, vector<QColor> color, int fanShape=0);

	void drawDissociation(QPainter *painter, QRectF rect);

	
	void drawOrthologyPanel(QPainter *painter, int sx , int sy);		
    void drawPathwayNodeTypes(QPainter *painter, int sx , int sy);
	void drawPathwayNodeAttributes(QPainter *painter, int sx , int sy);
	void drawPathwayProteinAttributes(QPainter *painter, int sx , int sy);
	void drawPathwayReactionTypes(QPainter *painter, int sx , int sy);
	void drawPathwayReactionAttrubutes(QPainter *painter, int sx , int sy);
	void drawExpressionGlyphs(QPainter *painter, int sx , int sy);
    void drawTreeRingNodePanel(QPainter *painter, int sx , int sy);

	void drawGlyphTemplat(QPainter *painter, int sx , int sy); //used as a template

	bool isSymbolPanelHided() {return hideIt; }

	/*void logMoveto();
	void logHide();
	void logOrderByWeight();
	void logOrderBySpeed();
	void logShowColorBar();
	void logHideColorBar();
	void logDragFileToCompose( QString filename );
	void logDragVisToCompose( int index, int vis );
	void logDragFileToScene( QString filename, QPointF pos );
	void logDragVisToScene( int vis, QPointF pos );
	void logDragItemtoScene( int index, QPointF pos );*/
	//ItemBase *pItem;
signals:
	void orderByWeight();
	void orderBySpeed();
protected:
	QRectF hideButton;
	bool hideIt;	
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	bool isInHideButton(int x, int y);
    void drawHideButton(QPainter *painter);
/*protected:
	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	void keyPressEvent(QKeyEvent *event);
	void keyReleaseEvent(QKeyEvent *event);
	void wheelEvent(QGraphicsSceneWheelEvent *event);
	*/
private:
	QString getVisNameByIndex( int vis );
	void drawColorBar( QPainter* painter );
	void openViewDirectly( QPointF pos );
	void drawPreOpenViewDirectly( QPointF pos );
	void initIcons();

protected:
	void initViewRect();
private:

	void drawViews( QPainter* painter );
	//
	void drawControl( QPainter* painter );
	void handleDragInDrop( QPointF pos );
	void renderItem( QPainter* painter );
	QList<FileItem*> _lines;
	void reOrder();
	//
	bool inFileView( QPointF pos );
	bool handleHoverFileView( QPointF pos );
	bool handlePressFileView( QPointF pos );
	bool handleMoveFileView( QPointF pos );
	bool handleReleaseFileView( QPointF pos );
	//
	bool inDropView( QPointF pos );
	bool handleHoverDropView( QPointF pos );
	int handlePressDropView( QPointF pos );
	//
	bool inVisView( QPointF pos );
	bool handleHoverVisView( QPointF pos );
	bool handlePressVisView( QPointF pos );
	bool handleMoveVisView( QPointF pos );
	bool handleReleaseVisView( QPointF pos );
	void initManager();
	int _VISINDEX;
	QRectF _visDragRect;
	//
	QRectF getDorpArea();
	QRectF getPaintArea();
	QRectF getVisArea();
	QRectF scaleArea();
	//
	void drawDropArea( QPainter* painter );
	void drawFileArea( QPainter* painter );
	void drawVisArea( QPainter* painter );

	OpenGLScene* _scene;
	OpenManager* _open;
	InstanceManager* _manager;
	InstanceInitializer* _init;
	float _heightPercent;
	QPointF _initDragPos;
	//
	bool _DRAGFILE;
	//
	void addNewToDrag();
	void drawPreOpenFileDirectly( QPointF pos );
	void OpenFileDirectly( QPointF pos );
	void changeView( QPointF pos, int visindex );
	//
	int CLICKINDEX;
	//
	QRectF _Control;
	int _ONCONTROLTYPE;
	QRectF _EllipseMap;
	QRectF _EllipseMove;
	QRectF _EllipseHide;
	QRectF _EllipseColor;
	bool COLORBAR;
	QString controlName[4];
	bool _ONMOVE;
	bool _HIGHLIGHTCONTROL;
	//**********************************************************
	QList<QString> _view;
	QList<QRectF> _iconRects;
	QList<QRectF> _shadow;
	QList<QRectF> _iconArea;
	QRectF _preOpenViewDirectly;
	QVector<QRectF> _rectsView;
	QList<QColor> _colors;
	bool _DIRECTPREOPEN;
	//
	bool SHIFTFILEVIEW;
	QPointF _FileTrans;
	QImage _icons[15];
	QString _CurrentFileName;

	bool _CTRL;
};

#endif
