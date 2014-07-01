#ifndef MATBLOCK_H
#define MATBLOCK_H

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
#include "MatlabEngThread.h"
#include "variableCheck.h"
#include "matDataVisBlock.h"

class OpenGLScene;

class MatBlock : public ItemBase
{
	Q_OBJECT

public:
	MatBlock( int x, int y, OpenGLScene* Scene );
	~MatBlock();
	void initUI();
	void paint( QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *widget );
	void loadFile( const QString &fileName );
	void removeCodeCon( MatBlock *codeBubble );
	void removeAllCodeSon();
	void removeDataVis();
	void copyAttribute( MatBlock *another );
	void resizeItem(int size1, int size2 );
	MatBlock* getCodeParent()
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

public slots:
	void finishMatlab();
	void checkByName( QString name );
	void updatePath();
	void updatePos();

signals:
	void updateLabelState( ItemBase *item );
	void setMatFileBubble( MatBlock* currentMat );
	void addParameterFromMat( QString pname, QString run_num, QList<double> dataResult );
	void updateDataFromMat( QString pname, QList<double> dataResult );

protected:
	void mousePressEvent( QGraphicsSceneMouseEvent *event );
	void mouseReleaseEvent( QGraphicsSceneMouseEvent *event );
	void mouseMoveEvent( QGraphicsSceneMouseEvent *event );
	void hoverMoveEvent( QGraphicsSceneHoverEvent *event );
	void contextMenuEvent( QGraphicsSceneContextMenuEvent *event );

private:
	void matItemPosMoved( QPointF offset );
	bool hasOpenMatOrNot( MatBlock *para, QString name );
	void saveFile( const QString &fileName );
	bool maybeSave();
	void setCurrentFile(const QString &fileName);
	void updateLabel();
	void drawConnections( QPainter *painter );
	void findFunction();
	void setPlotPara();
	MatBlock* findCodeBubblebyName( QString fileName );
	void drawBubbleWithoutHalo( QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *widget );
	void drawBoarder( QPainter* painter );
	bool onBoarder( QPointF pos );
	int codeIndexofParameter( QString str );

	void runMatlab();
	void setVisable();
	void openMatlabFile();
	void saveMatlabFile();
	void newMatlabFile();
	void saveasMatlabFile();

	QTextEdit* _text;
	Highlighter* _hightlight;
	OpenGLScene* _scene;
	MatlabEngThread* _thread;
	VariableCheck* _check;
	QList< MatBlock* > _codeBubbles;
	MatBlock* _codeParent;
	QList< QPointF > _conPoint;
	QList< QList<MatResult> > _results;

	QString _curFileName;
	QString _curFilePath;
	bool _LABEL;
	bool _ONMOVE;

	MatDataVisBlock* _dataVis;
	QStringList functionName;
	QList< QStringList > paraName;
	QStringList codes;

	bool rightClicked;
};

#endif
