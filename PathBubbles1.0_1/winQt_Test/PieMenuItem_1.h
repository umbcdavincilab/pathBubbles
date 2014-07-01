#ifndef PieMenuITEM_H
#define PieMenuITEM_H

#include <QObject>
#include <QRectF>
#include <QPainter>
#include <QString>
#include <vector>

using namespace std;

class OpenGLScene;

class PieMenuItem : public QObject
{
	Q_OBJECT
public:
	PieMenuItem(OpenGLScene* scene);
	virtual ~PieMenuItem();

	virtual QPointF getPos();
	virtual void paint( QPainter* painter, QPointF bubblePos);
	virtual bool mouseOver( QPointF pos );
	virtual int getType();
	virtual QString toQString();
	virtual void click();
	virtual void release();
	virtual QVector<QString> getText();	

	void setSectorID(QPointF curPos);  
	
	//virtual void setPos( QPointF pos );
	//virtual void setRadius( float R);
	//virtual void setText( QVector<QString> text );

	virtual void setPieMenu(QPointF pos, float R, QVector<QString> text, QVector<QVector<QString>> text2, QVector<QColor> color, QVector<QVector<QColor>> color2, vector<bool> disables, vector<vector<bool>> disables2);
	virtual bool inPie(QPointF curPos);
	virtual bool inPie2(QPointF curPos);	
	virtual int getSectorID(QPointF curPos);	
	virtual vector<int> getSectorID2(QPointF curPos);

	virtual int getSectorNum();
	virtual float getStartAngle();	
	virtual float getIntervalAngle();	
	virtual vector<bool> getDisables();	
	virtual vector<vector<bool>> getDisables2();	
	virtual void setSectorID2(QPointF curPos);

	void testSignal();

//signals:
	void test();	

protected:
	QPointF _pos; //center
	float _R;
	int _currentSector; //to highlight
	int _currentSector2;
	float _startAngle;
	float _intervalAngle;

	QVector<QString> _text;
	QVector<QColor> _color;
	QVector<QVector<QString>> _text2;
	QVector<QVector<QColor>> _color2;

	bool _curState;
	int _TYPE;
	int _sectorNum;
	vector<bool> _disables;
	vector<vector<bool>> _disables2;	
};
#endif