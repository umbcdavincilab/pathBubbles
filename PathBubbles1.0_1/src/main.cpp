
#include "winqt_test.h"
#include <QtGui/QApplication>
#include <gl/freeglut.h>

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	QApplication a(argc, argv);

	winQt_Test w;
	w.show();
	return a.exec();
}