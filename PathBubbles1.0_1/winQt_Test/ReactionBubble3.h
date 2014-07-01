#ifndef REACTIONBUBBLE3_H
#define REACTIONBUBBLE3_H

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

class PathBubble1;
class OpenGLScene;

class ReactionBubble3 : public ItemBase
{
	Q_OBJECT

public:
	void showIterm( QPointF pos, QPointF scenePos, QString name);
	PathBubble1 * _pathBubbleParent;
	

	void deleteSelectedItemsbyParent(QGraphicsScene *scene);

	void deleteSelectedItems(QGraphicsScene *scene);
	ReactionBubble3(int m_pathwayID, int size, int x, int y, vector<int> ItemSelected, OpenGLScene* Scene, OpenManager* open, QString name);
	~ReactionBubble3();
	void initUI();
	void initItemPos(vector<int> ItemSelected);
	void paint( QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *widget );	
	void removeCodeCon( ReactionBubble3 *codeBubble );
	void removeAllCodeSon();
	void removeDataVis();
	void copyAttribute( ReactionBubble3 *another );
	void resizeItem(int size1, int size2 );
	ReactionBubble3* getCodeParent()
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
	void loadFile(const QString &fileName);
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


	QTextLine currentTextLine(const QTextCursor &cursor);
	int m_pathwayID;

public slots:
	//void finishMatlab();
	//void checkByName( QString name );
	void updatePath();
	void updatePos();

signals:
	void updateLabelState( ItemBase *item );
	//void setMatFileBubble( ReactionBubble3* currentMat );
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
	bool hasOpenMatOrNot( ReactionBubble3 *para, QString name );
	void saveFile( const QString &fileName );
	bool maybeSave();
	void setCurrentFile(const QString &fileName);
	void updateLabel();
	void drawConnections( QPainter *painter );
	void findFunction();
	//void setPlotPara();
	ReactionBubble3* findCodeBubblebyName( QString fileName );
	void drawBubbleWithoutHalo( QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *widget );
	void drawBoarder( QPainter* painter );
	bool onBoarder( QPointF pos );
	int codeIndexofParameter( QString str );
	void paintItem(QPainter *painter);
	vector<QRectF> itemPos;

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
	QList< ReactionBubble3* > _reactionBubbles;
	ReactionBubble3* _codeParent;
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

	vector<vector<int>> itemInfo;
	bool rightClicked;
	float L,B,W,H;
};

#endif
