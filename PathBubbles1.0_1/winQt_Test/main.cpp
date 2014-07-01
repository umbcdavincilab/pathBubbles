
#include "winqt_test.h"
#include <QtGui/QApplication>
#include <GL/glut.h>

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);	
	QApplication a(argc, argv);

	winQt_Test w;
	//w.setWindowState((windowState() & ~(Qt::WindowMinimized | Qt::WindowFullScreen))
    //               | Qt::WindowMaximized);
	w.show();
	QDesktopWidget desktop;

	int desktopHeight=desktop.geometry().height();
	int desktopWidth=desktop.geometry().width();
	w.resize(desktopWidth,desktopHeight);//60
	//w.maximumSize();

	w.setWindowTitle("PathBubbles");
	w.setWindowIcon(QIcon("image.jpg"));

	//w.resize(1900,1200);	
	w.move(0,0);
	return a.exec();
}