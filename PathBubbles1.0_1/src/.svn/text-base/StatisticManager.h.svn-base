#ifndef STATISTICMANAGER_H
#define STATISTICMANAGER_H

#include <string>
#include <QFile>
#include <QString>
#include <QList>
#include <QTextStream>
#include <QStringList>

class StatisticManager : public QObject
{
	Q_OBJECT
public:
	StatisticManager( QString path );
	~StatisticManager();

	void load();
	QStringList get1DNameList();
	QStringList get1DTNameList();
	QStringList get1DFromMatNameList();
	QStringList get1DTFromMatNameList();
	int checkIndexByName( std::string name );
	QString getNameByIndex( int index );
	float getValueAt( int x, int y );
	float getValueAt( QString file, QString para );
	void indexAcceptedUpdate( int index );
	void addParaFromMat( QString para, QList<double> dataResult );
	QList<double> getParaValueFromMat( QString para );
	void removePara( int typeIndex, int nameIndex );
	void updateMatData( QString pName, QList<double> dataResult );

signals:
	void indexAccepted( int index );
private:
	QString _path;
	QList<QStringList> _table;
	QStringList _names;
	
	QStringList _1DparaName;
	QStringList _1DTparaName;
	QStringList _1DparaFromMatName;
	QStringList _1DTparaFromMatName;
	QList<QList<double>> _1DResultFromMat;
	QList<QList<double>> _1DTResultFromMat;
};
#endif