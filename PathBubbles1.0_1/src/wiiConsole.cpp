//#include "wiiConsole.h"
//#include <QString>
//#include <sstream>
//
//wiiConsole::wiiConsole()
//: QMainWindow()
//{
//	ui = new Ui::wiiConsole();
//	ui->setupUi(this);
//}
//
//void wiiConsole::receive( wiiData* stu )
//{
//	std::stringstream input;
//	//std::string str(cha);
//	input<<stu->angleX<<" >-<"<<stu->angleY<<" BUT: "<<stu->button;
//	//ui->textEdit->append( QString( tr( str.c_str() ) ) );
//	ui->textEdit->clear();
//	ui->textEdit->append( QString( input.str().c_str() ) );
//}