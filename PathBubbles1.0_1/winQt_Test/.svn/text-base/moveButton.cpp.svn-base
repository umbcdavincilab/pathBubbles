#include "moveButton.h"

moveButton::moveButton( QWidget* parent ) : QPushButton( parent )
{
	this->push_icon = new QIcon( tr( "icon/release.png" ) );
	this->release_icon = new QIcon( tr( "icon/push.png" ) );
	_released();
	this->connect( this, SIGNAL( pressed() ), this, SLOT( _pushed() ) );
	this->connect( this, SIGNAL( released() ), this, SLOT( _released() ) );
}

moveButton::~moveButton()
{
	delete push_icon;
	delete release_icon;
	return;
}
void moveButton::_released()
{
	this->setIcon( (*release_icon) );
	return;
}

void moveButton::_pushed()
{
	this->setIcon( (*push_icon) );
	return;
}

QSize moveButton::sizeHint() const
{
	return QSize( 20, 20 );
}