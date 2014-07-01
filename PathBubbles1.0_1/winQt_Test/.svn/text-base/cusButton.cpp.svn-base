#include "cusButton.h"

cusButton::cusButton()
{
	for_Out = new QIcon( tr( "icon/for_out.med.png" ));
	for_Hid = new QIcon( tr( "icon/for_hid.med.png" ));
	return;
}

cusButton::~cusButton()
{
	delete for_Hid;
	delete for_Out;
	return;
}

void cusButton::for_Hid_But()
{
	this->setIcon( (*for_Hid) );
}

void cusButton::for_Out_But()
{
	this->setIcon( (*for_Out) );
}

QSize cusButton::sizeHint() const
{
	return QSize( 40, 40 );
}