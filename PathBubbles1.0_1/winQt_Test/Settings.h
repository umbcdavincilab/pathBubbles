#ifndef Settings
#define Settings

#include "webBlock.h"
#include <QWebView>
#include <QDialog>
#include "moveButton.h"
#include <QPushButton>
#include <QGridLayout>
#include <QLineEdit>
#include <QString>
#include <QGroupBox>
#include <QRadioButton>
#include <QCheckBox>
//#include "PathBubbleBase.h"

class OpenGLScene;
class WebBlock;
class Settings : public QDialog
{
	Q_OBJECT
public:
	Settings( OpenGLScene *scene, QString target = tr(""));
	Settings::~Settings( );
	QString getWebSite()
	{
		return this->_path->text();
	}
	QLineEdit* _path;
	QString _name;
	//void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
	//void resetOriRect();
	//void scaleUpBasedonOriRect();
	//void scaleDownBasedonOriRect();
	WebQuery * _web;
public slots:	
	void updatePreview();
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
	void setWebItem(class WebBlock *web)
	{
	   _webItem=web;
	}

signals:
	void needSetToHide();
	void moveButtonState( bool );
private:
	//QRectF _oriRect; 
	//float _curScale; 

	QRadioButton *createRadioButton(const QString &text);
	QCheckBox *createCheckBox(const QString &text);	

	QGroupBox *typeGroupBox;
	QRadioButton *windowRadioButton;
    QRadioButton *dialogRadioButton;
    
	QGroupBox *hintsGroupBox;
	QCheckBox *msWindowsFixedSizeDialogCheckBox;
    QCheckBox *x11BypassWindowManagerCheckBox;
    QCheckBox *framelessWindowCheckBox;
    QCheckBox *windowNoShadowCheckBox;
    QCheckBox *windowTitleCheckBox;

	QGridLayout* _layout;
	moveButton* _move;
	QPushButton* _go;
	QPushButton* _hide;
	
	QWebView *_view;
	OpenGLScene *_scene;
	WebBlock *_webItem;
};

#endif