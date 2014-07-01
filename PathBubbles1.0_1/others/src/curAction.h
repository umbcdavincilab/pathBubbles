#ifndef CURACTION_H
#define CURACTION_H

#include <QAction>

class curAction : public QAction
{
	Q_OBJECT
public:
	curAction(const QString & text, QObject * parent, int flag);
signals:
	void cur_clicked_signal( int flag );
public slots:
	void cur_clicked_slot();
private:
	int flag;
};
#endif