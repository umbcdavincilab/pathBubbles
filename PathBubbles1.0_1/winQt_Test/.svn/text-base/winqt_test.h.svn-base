#ifndef WINQT_TEST_H
#define WINQT_TEST_H

#include <QWidget>
#include "GraphicsView.h"
#include "openglscene.h"

class winQt_Test : public QWidget
{
	Q_OBJECT
public:
	winQt_Test(QWidget *parent = 0, Qt::WFlags flags = 0);
	QGridLayout* CreateUI();

signals:
	void hasClosed();
protected:
	void closeEvent( QCloseEvent *event );
	void resizeEvent( QResizeEvent *event );

private:
	bool maybeSave();
	LabObject *lab;
	LabCut *cut;
	//Bat *bat;
};

#endif // WINQT_TEST_H
