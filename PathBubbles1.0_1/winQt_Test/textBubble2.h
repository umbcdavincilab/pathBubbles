#ifndef TEXTBUBBLE2_H
#define TEXTBUBBLE2_H

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

class TextBubble2 : public ItemBase
{
	Q_OBJECT

public:
	//ItemBase* _ItemBaseParent;
	void getAllSubTextBubble(TextBubble2* bubble, QList<ItemBase *> &list);
	void getAllPSTextBubble(TextBubble2* bubble, QList<ItemBase *> &list); //bubble in the whole tree hierarchy
	void getHigherTextBubble(TextBubble2* bubble, QList<ItemBase *> &list); //bubble in higher level of the whole tree hierarchy	
	int getTextBubbleLevel(TextBubble2* bubble);
	TextBubble2* getTopTextBubble(TextBubble2* bubble);
	void updateLabel();
    PathBubble1* getTopPathBubble(TextBubble2* bubble);

	int getTotalLevel(int pid, int type);
	int m_parentTextBubbleID;
	int m_parentPathBubbleID;

	PathBubble1 * _pathBubbleParent;
	void showIterm( QPointF pos, QPointF scenePos, QString name);
	void deleteSelectedItemsbyParent(QGraphicsScene *scene);
	
	void deleteSelectedItems(QGraphicsScene *scene);
	TextBubble2(int parentPathID, int parentCodeID,  int size, int x, int y, OpenGLScene* Scene, OpenManager* open, QString name);
	~TextBubble2();
	void initUI();
	void paint( QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *widget );
	void loadFile( const QString &fileName );
	QTextEdit* _text;
	//void removeCodeCon( TextBubble2 *codeBubble );
	//void removeAllCodeSon();
	//void removeDataVis();
	void copyAttribute( TextBubble2 *another );
	void resizeItem(int size1, int size2 );
	TextBubble2* getCodeParent()
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

	QTextLine currentTextLine(const QTextCursor &cursor);
public slots:
	//void finishMatlab();
	//void checkByName( QString name );
	void updatePath();
	void updatePos();
	QString on_textEdit_clicked(); //QPointF pos, QPointF scenePos
	

signals:
	void updateLabelState( ItemBase *item );
	//void setMatFileBubble( TextBubble2* currentMat );
	void addParameterFromMat( QString pname, QString run_num, QList<double> dataResult );
	void updateDataFromMat( QString pname, QList<double> dataResult );

protected:
	bool onBoarder( QPointF pos );
	void mousePressEvent( QGraphicsSceneMouseEvent *event );
	void mouseReleaseEvent( QGraphicsSceneMouseEvent *event );
	void mouseMoveEvent( QGraphicsSceneMouseEvent *event );
	void hoverMoveEvent( QGraphicsSceneHoverEvent *event );
	void contextMenuEvent( QGraphicsSceneContextMenuEvent *event );
	void contentsMouseMoveEvent(QMouseEvent* event);
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

private:
	QPointF _queryPos, _queryScenePos;
	void matItemPosMoved( QPointF offset );
	bool hasOpenMatOrNot( TextBubble2 *para, QString name );
	void saveFile( const QString &fileName );
	bool maybeSave();
	void setCurrentFile(const QString &fileName);
	

	
    void deleteSelectedItemKids(QGraphicsScene *scene);
	
	void drawConnections( QPainter *painter );
	void findFunction();
	void setPlotPara();
	TextBubble2* findCodeBubblebyName( QString fileName );
	void drawBubbleWithoutHalo( QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *widget );
	void drawBoarder( QPainter* painter );
	//bool onBoarder( QPointF pos );
	int codeIndexofParameter( QString str );
	void highlightLineClicked(QPainter *painter, int lineClicked, QColor c);
	int whichLine(QPointF pos);


	

	/*void runMatlab();
	void setVisable();
	void openMatlabFile();
	void saveMatlabFile();
	void newMatlabFile();
	void saveasMatlabFile();*/

	int _lineClicked;
	
	Highlighter* _hightlight;
	//OpenGLScene* _scene;
	OpenManager* _open;
	//MatlabEngThread* _thread;
	//VariableCheck* _check;
	

	TextBubble2* _codeParent;
	QList< TextBubble2* > _codeBubbles;
	QList< QPointF > _conPoint;
	QList< int > _bubbleLine;
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
};

#endif
