#ifndef SEARCHFIND_H
#define SEARCHFIND_H

#include "webBlock.h"
#include <QWebView>
#include <QDialog>
#include "moveButton.h"
#include <QPushButton>
#include <QGridLayout>
#include <QLineEdit>
#include <QGroupBox>
#include <QRadioButton>
#include <QCheckBox>
#include "eQuery.h"

//#include "PathBubbleBase.h"

class OpenGLScene;
class WebBlock;
class SearchFind : public QDialog
{
	Q_OBJECT
public:
	eQuery m_eQuery; 
	QRadioButton *dragResizeMode1, *dragResizeMode2, *dragResizeMode3; //dragResizeMode
	QCheckBox *framelessWindowCheckBox; //draw/search small molecular
	QCheckBox *msWindowsFixedSizeDialogCheckBox; //draw/search protein
	QCheckBox *x11BypassWindowManagerCheckBox;  //draw/search complex/physical entity
	QCheckBox *windowNoShadowCheckBox; //draw/search all entity

	QCheckBox *fixedSizeCheckBox;
	
	QRadioButton *proteinRadioButton, *compartmentRadioButton, *pathwayRadioButton;
	QRadioButton *setAllRadioButton, *setNewRadioButton;

	QCheckBox *sharedCheckBox, *differedCheckBox, *linkedCheckBox, *expressedCheckBox;

	void setInvisible();
	void setVisible();
	
	SearchFind( OpenGLScene *scene, QString target = tr(""), int type = 1);
	void deleteItems( );
	QString getWebSite()
	{
		return this->_path->text();
	}
	void setDrawSmallMolecule(bool flag);
	
	QLineEdit* _path;
	QString _name;	
	WebQuery * _web;
	set<int> SearchList;
public slots:	
	void updatePreview_1(); //checkbox
	void updatePreview_2(); //radiobutton
	void needHide();
	void openURL();
	void search();
	void onQueryFinished(bool bOK, QString ip, QString area);
	void onQueryButton(); 
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

	QGroupBox *typeGroupBox, *initBubbleSize;
	QRadioButton *windowRadioButton;
    QRadioButton *dialogRadioButton;
    
	QGroupBox *hintsGroupBox; //display : small molecule or not
	QGroupBox *WindowDragResize; //rescale setting : small molecule or not    
    QCheckBox *windowTitleCheckBox;
	
	QGridLayout* _layout;
	moveButton* _move;
	QPushButton* _go;
	QPushButton* _hide;

	QPushButton* ApplyButton;
	QLineEdit *topSizeInput, *sonSizeInput;

	QWebView *_view;
	OpenGLScene *_scene;
	WebBlock *_webItem;
};

#endif