#ifndef SEARCH_FIND
#define SEARCH_FIND

#include <QWebView>
#include <QDialog>
#include "moveButton.h"
#include <QPushButton>
#include <QGridLayout>
#include <QLineEdit>

class OpenGLScene;

class Search_Find : public QDialog
{
	Q_OBJECT
public:
	Search_Find( OpenGLScene *scene, QString target = tr(""));
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