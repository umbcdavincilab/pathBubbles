//#include "wiiThread.h"
//#include <windows.h> 
//#include <stdio.h> 
//#include <tchar.h>
//#include <strsafe.h>
////#include "RenderWindow.h"
//
////#define BUFSIZE sizeof( wiiData )
//
//wiiThread::wiiThread( )
//:QThread()
//{
//	console = new wiiConsole();
//	initConnection();
//	console->show();
//	TBSETTED = 1;
//	rotationBegin[0] = -9999;
//	return;
//}
//void wiiThread::run()
//{
//	//int time = 0;
//	//while( true )
//	//{
//	//	emit updateInfor( );
//	//	sleep( 2 );
//	//	time += 1;
//	//}
//	waitPipe();
//	//std::string hello = "hello, wii!";
//	wiiData* data = new wiiData;
//	data->angleX = 0;
//	data->angleY = 0;
//	//char* cha = new char[hello.length()];
//	//strcpy( cha, hello.c_str());
//	while( true )
//	{
//		emit updateInfor( data );
//		data->angleX += 1;
//		data->angleY += 1;
//		sleep( 2 );
//	}
//}
//
//void wiiThread::initConnection()
//{
//	int result;
//	result = connect( this, SIGNAL( updateInfor(wiiData*)), console, SLOT( receive(wiiData*)));
//	//result = connect( console, SIGNAL( close() ), this, SLOT( destroyed()) );
//}
//
//void wiiThread::destroyed()
//{
//	this->terminate();
//}
//
//void wiiThread::waitPipe()
//{
//	BOOL   fConnected = FALSE; 
//	DWORD  dwThreadId = 0; 
//	hPipe = INVALID_HANDLE_VALUE; 
//	LPTSTR lpszPipename = TEXT("\\\\.\\pipe\\mynamedpipe"); 
//
//	for (;;) 
//	{ 
//		//_tprintf( TEXT("\nPipe Server: Main thread awaiting client connection on %s\n"), lpszPipename);
//		hPipe = CreateNamedPipe( 
//			lpszPipename,             // pipe name 
//			PIPE_ACCESS_DUPLEX,       // read/write access 
//			PIPE_TYPE_MESSAGE |       // message type pipe 
//			PIPE_READMODE_MESSAGE |   // message-read mode 
//			PIPE_WAIT,                // blocking mode 
//			PIPE_UNLIMITED_INSTANCES, // max. instances  
//			BUFSIZE,                  // output buffer size 
//			BUFSIZE,                  // input buffer size 
//			0,                        // client time-out 
//			NULL);                    // default security attribute 
//
//		if (hPipe == INVALID_HANDLE_VALUE) 
//		{
//			//_tprintf(TEXT("CreateNamedPipe failed, GLE=%d.\n"), GetLastError()); 
//			return;
//		}
//		fConnected = ConnectNamedPipe(hPipe, NULL) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);
//		if (fConnected) 
//		{ 
//			//printf("Client connected, creating a processing thread.\n");
//			//(LPVOID) hPipe,    // thread parameter 
//			handlePipe();
//			return;
//		} 
//		else			// The client could not connect, so close the pipe. 
//		{
//			CloseHandle(hPipe); 
//		}
//	} 
//	return;
//}
//
////void wiiThread::setTB( TrackBall* TB)
////{
////	tb = TB;
////	TBSETTED = 1;
////	return;
////}
//
//void wiiThread::handlePipe()
//{
//	wiiData* wiiData;
//	//HANDLE hHeap      = GetProcessHeap();
//	//TCHAR* pchRequest = (TCHAR*)HeapAlloc(hHeap, 0, BUFSIZE*sizeof(TCHAR));
//	TCHAR* pchRequest = new TCHAR[ BUFSIZE ];
//	//TCHAR* pchReply   = (TCHAR*)HeapAlloc(hHeap, 0, BUFSIZE*sizeof(TCHAR));
//	//TCHAR* pchRequest = new TCHAR[BUFSIZE*sizeof(TCHAR)];
//	DWORD cbBytesRead = 0, cbReplyBytes = 0, cbWritten = 0; 
//	BOOL fSuccess = FALSE;
//	// Do some extra error checking since the app will keep running even if this
//	// thread fails.
//	if (hPipe == NULL)
//	{
//		printf( "\nERROR - Pipe Server Failure:\n");
//		printf( "   InstanceThread got an unexpected NULL value in lpvParam.\n");
//		printf( "   InstanceThread exitting.\n");
//	}
//
//	if (pchRequest == NULL)
//	{
//       printf( "\nERROR - Pipe Server Failure:\n");
//       printf( "   InstanceThread got an unexpected NULL heap allocation.\n");
//       printf( "   InstanceThread exitting.\n");
//   }
//   // Print verbose messages. In production code, this should be for debugging only.
//	// The thread's parameter is a handle to a pipe object instance.
//	// Loop until done reading
//	while (1)
//	{ 
//		// Read client requests from the pipe. This simplistic code only allows messages
//		// up to BUFSIZE characters in length.
//		fSuccess = ReadFile( 
//			hPipe,        // handle to pipe 
//			pchRequest,    // buffer to receive data 
//			BUFSIZE, // size of buffer 
//			&cbBytesRead, // number of bytes read 
//			NULL);        // not overlapped I/O 
//		if (!fSuccess || cbBytesRead == 0)
//		{   
//			if (GetLastError() == ERROR_BROKEN_PIPE)
//			{
//				//_tprintf(TEXT("InstanceThread: client disconnected.\n"), GetLastError()); 
//			}
//			else
//			{
//				//_tprintf(TEXT("InstanceThread ReadFile failed, GLE=%d.\n"), GetLastError()); 
//			}
//			//break;
//		}
//		wiiData = costWiipToCharp( pchRequest );
//		//if( TBSETTED == 1)
//		//{
//		//	tb->setXY( wiiData->angleX, wiiData->angleY );
//		//}
//		if( wiiData->button == (int)pow(2.0, 10) )
//		{
//			if( rotationBegin[0] == -9999)
//			{
//				rotationBegin[0] = wiiData->angleX;
//				rotationBegin[1] = wiiData->angleY;
//			}
//			else
//			{
//				//tb->setXY( wiiData->angleX - rotationBegin[0], wiiData->angleY - rotationBegin[1]);
//				rotationBegin[0] = wiiData->angleX;
//				rotationBegin[1] = wiiData->angleY;
//			}
//		}
//		else
//		{
//			if( rotationBegin[0] != -9999 )
//			{
//				rotationBegin[0] = -9999;
//			}
//		}
//		updateInfor( costWiipToCharp( pchRequest ));
//		//rd->wiiInfor( costWiipToCharp( pchRequest ) );
//		//return;
//	}
//
//// Flush the pipe to allow the client to read the pipe's contents 
//// before disconnecting. Then disconnect the pipe, and close the 
//// handle to this pipe instance. 
// 
//   //FlushFileBuffers(hPipe); 
//   //DisconnectNamedPipe(hPipe); 
//   //CloseHandle(hPipe); 
//}