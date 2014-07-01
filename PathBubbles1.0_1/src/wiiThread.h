//#ifndef WIITHREAD_H
//#define WIITHREAD_H
//
//#include <windows.h>
//#include <QThread>
//#include "wiiConsole.h"
////#include "wiimote.h"
//#include "math.h"
//#include <mmsystem.h>
//#include "wiiStruct.h"
//#include "trackball.h"
//
////class RenderWindow;
//
//#define USE_BEEPS_AND_DELAYS
//#define LOOK_FOR_ADDITIONAL_WIIMOTES
//
//class wiiThread : public QThread
//{
//	Q_OBJECT
//public:
//	wiiThread( );
//	void run();
//	//void setTB( TrackBall* TB );
//signals:
//	void updateInfor( wiiData* );
//public slots:
//	void destroyed( );
//private:
//	void initConnection();
//	wiiConsole* console;
//	HANDLE hPipe;
//	void waitPipe();
//	void handlePipe();
//	TrackBall* tb;
//	int TBSETTED;
//	int rotationBegin[2];
//};
//
//#endif