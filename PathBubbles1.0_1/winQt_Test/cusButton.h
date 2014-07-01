#ifndef CUSBUTTON_H
#define CUSBUTTON_H

#include <QPushButton>
#include <QIcon>
#include <QSize>

class cusButton : public QPushButton
{
public:
	cusButton();
	~cusButton();
	void for_Hid_But();
	void for_Out_But();
	QSize sizeHint () const;
private:
	QIcon *for_Out;
	QIcon *for_Hid;
};

#endif
