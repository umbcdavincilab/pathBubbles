#ifndef VARIABLECHECK_H
#define VARIABLECHECK_H

#include "ControlBubble.h"
#include "MatlabEngThread.h"
#include <QLineEdit>

class VariableCheck : public ControlBubble
{
	Q_OBJECT
public:
	VariableCheck( int x, int y );
	void updateThread( MatlabEngThread* _thread );
	void acceptResult( MatResult );
	void initUI();
	QString makeTitle( int m, int n );
	void displayVariables( MatResult result );
protected:
	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
public slots:
	void needQuery();
signals:
	void query( QString name );
private:
	QTextEdit* _text;
	QPushButton* _go;
	QLineEdit* _name;
};

#endif