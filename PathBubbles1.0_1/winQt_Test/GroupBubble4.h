#ifndef GROUPBUBBLE4_H
#define GROUPBUBBLE4_H

#include <QTextEdit>
#include <QSyntaxHighlighter>
#include <QPushButton>
#include <QGraphicsProxyWidget>
#include <QString>
#include <QStringList>
#include <QList>
#include "qtbox.h"
#include "3DHelper.h"
#include "highLighter.h"
//#include "MatlabEngThread.h"
//#include "variableCheck.h"
//#include "matDataVisBlock.h"
#include "OpenManager.h"
#include <set>

class OpenGLScene;
class PathBubble1;

class GroupBubble4 : public ItemBase
{
	Q_OBJECT

public:
	void showIterm( QPointF pos, QPointF scenePos, QString name);
	PathBubble1 * _pathBubbleParent;

	vector<vector<int>> edgeSelected;
	vector<QRectF> itemPos;
	vector<vector<int>> itemInfo;

	void deleteSelectedItems(QGraphicsScene *scene);
	GroupBubble4(int pathwayID, int size, int x, int y, vector<int> ItemSelected, vector<vector<int>> EdgeSelected, OpenGLScene* Scene, OpenManager* open, QString name);
	~GroupBubble4();
	void initUI();
	void paint( QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *widget );
	void removeCodeCon( GroupBubble4 *codeBubble );
	void removeAllCodeSon();
	void removeDataVis();
	void copyAttribute( GroupBubble4 *another );
	void deleteSelectedItemsbyParent(QGraphicsScene *scene);	
	GroupBubble4* getCodeParent()
	{
		return _codeParent;
	}
	QString getCurrentFilename()
	{
		return _curFileName;
	}
	void saveAsXmlNode( QDomDocument &doc, QDomElement &node );
	void recoverFromXmlNode( QDomElement node );
	void rebuildBubbleConnect( QDomElement node );

	void logOpen();
	void logFindDeclaration( QString str );
	void logAddtoParaBar( QString str );
	void logCopy();
	void logRunCode();
	void logShowCheck();
	void logHideCheck();
	void logNewFile();
	void logOpenFile( QString str );
	void logSaveFile( QString str );

	int m_pathwayID;	
	QTextLine currentTextLine(const QTextCursor &cursor);

public slots:
	//void finishMatlab();
	//void checkByName( QString name );
	void updatePath();
	void updatePos();

signals:
	void updateLabelState( ItemBase *item );
	//void setMatFileBubble( GroupBubble4* currentMat );
	void addParameterFromMat( QString pname, QString run_num, QList<double> dataResult );
	void updateDataFromMat( QString pname, QList<double> dataResult );

protected:
	void mousePressEvent( QGraphicsSceneMouseEvent *event );
	void mouseReleaseEvent( QGraphicsSceneMouseEvent *event );
	void mouseMoveEvent( QGraphicsSceneMouseEvent *event );
	void hoverMoveEvent( QGraphicsSceneHoverEvent *event );
	void contextMenuEvent( QGraphicsSceneContextMenuEvent *event );
	void contentsMouseMoveEvent(QMouseEvent* event);

private:
	
	void matItemPosMoved( QPointF offset );
	bool hasOpenMatOrNot( GroupBubble4 *para, QString name );
	void saveFile( const QString &fileName );
	bool maybeSave();
	void setCurrentFile(const QString &fileName);
	void updateLabel();
	void drawConnections( QPainter *painter );
	void findFunction();
	//void setPlotPara();
	GroupBubble4* findCodeBubblebyName( QString fileName );
	void drawBubbleWithoutHalo( QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *widget );
	void drawBoarder( QPainter* painter );
	bool onBoarder( QPointF pos );
	int codeIndexofParameter( QString str );
	void paintItem(QPainter *painter);

	/*void runMatlab();
	void setVisable();
	void openMatlabFile();
	void saveMatlabFile();
	void newMatlabFile();
	void saveasMatlabFile();*/

	//QTextEdit* _text;
	Highlighter* _hightlight;
	//OpenGLScene* _scene;
	OpenManager* _open;
	//MatlabEngThread* _thread;
	//VariableCheck* _check;
	QList< GroupBubble4* > _groupbubbles;
	GroupBubble4* _codeParent;
	QList< QPointF > _conPoint;
	//QList< QList<MatResult> > _results;

	QString _curFileName;
	QString _curFilePath;
	//bool _LABEL;
	bool _ONMOVE;

	//MatDataVisBlock* _dataVis;
	QStringList functionName;
	QList< QStringList > paraName;
	QStringList codes;

	bool rightClicked;
	void initItemPos(vector<int> ItemSelected);

	float L,B,W,H;
};

#endif
