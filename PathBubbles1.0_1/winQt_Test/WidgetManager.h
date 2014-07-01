#ifndef MYWIDGETMANAGER_H
#define MYWIDGETMANAGER_H
#include <QGroupBox>

class myWidgetManager
{
public:
	myWidgetManager(int Length);
	~myWidgetManager();
	void addWidget( QGroupBox* );
	QGroupBox* queryWidget( int index);
	void removeWidget( QGroupBox* widget);
	int Lenght(){ return Length; }
private:
	int Length;
	QGroupBox** array;
};

#endif