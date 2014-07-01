#ifndef WEBQUERY_H
#define WEBQUERY_H

#include <QWebView>
#include <QDialog>
#include "moveButton.h"
#include <QPushButton>
#include <QGridLayout>
#include <QLineEdit>


class OpenGLScene;

class WebQuery : public QDialog
{
	Q_OBJECT
public:
	WebQuery( OpenGLScene *scene, QString target = tr(""));
	void deleteItems();
	QString getWebSite()
	{
		return this->_path->text();
	}
public slots:
	void needHide();
	void openURL();
	void search();
	void moveButtonPress()
	{
		emit moveButtonState( true );
	}
	void moveButtonRelease()
	{
		emit moveButtonState( false );
	}
signals:
	void needSetToHide();
	void moveButtonState( bool );
private:
	QGridLayout* _layout;
	moveButton* _move;
	QPushButton* _go;
	QPushButton* _hide;
	QLineEdit* _path;
	QWebView *_view;
	OpenGLScene *_scene;
};

#endif