#ifndef MOVEBUTTON_H
#define MOVEBUTTON_H

#include <QPushButton>
#include <string>

class moveButton : public QPushButton
{
	Q_OBJECT
public:
	moveButton( QWidget* parent );
	~moveButton();
	QSize sizeHint () const;
public slots:
	void _released();
	void _pushed();
private:
	QIcon *push_icon;
	QIcon *release_icon;
};

#endif