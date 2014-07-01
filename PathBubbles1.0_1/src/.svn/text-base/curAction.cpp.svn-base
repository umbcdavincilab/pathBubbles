#include "curAction.h"

curAction::curAction(const QString & text, QObject * parent, int input)
:QAction( text, parent)
{
	flag = input;
	connect( this, SIGNAL(  changed() ), this, SLOT( cur_clicked_slot() ));
	return;
}

void curAction::cur_clicked_slot()
{
	//connect( this, SIGNAL( changed() ), this, SLOT( clickSM() ));
	emit cur_clicked_signal( flag );
}