#ifndef SCRIPTENGIN_H
#define SCRIPTENGIN_H

#include <QObject>
#include <QList>
#include <QString>
#include <QStringList>
#include "Discreptor.h"
#include "PenItem.h"
#include "qtbox.h"

class OpenManager;

class ScriptEngin : public QObject
{
	Q_OBJECT 
public:
	ScriptEngin( OpenManager* manager );
	~ScriptEngin();
	QString runScript( QList<PenItem*> script );
	void setDiscreptor( DESCRIPTOR dis );
	bool runCommand( QStringList words );
	QString errorAt( QString name );
	OpenManager* getOpenManager();
signals:
	void needSlider( QList<QList<ItemBase*>> bats, QList<QList<float>> rates );
private:
	int resolveMarker( QString name );
	int resolveParameter( QString name );
	bool handelOpen( QList<PenItem*> script );
	bool handelFind( QList<PenItem*> script, QList<QList<ItemBase*>> &bats, QList<QList<float>> &rates );
	bool handelSet( QList<PenItem*> script );
	OpenManager* _openManager;
	DESCRIPTOR _discreptor;
	bool DIS;
	int _PARA[5];
};

#endif