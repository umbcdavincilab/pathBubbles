#include "BubbleInstance.h"

BubbleInstance::BubbleInstance( QString spe, QString ind, QString run )
{
	this->_ind = ind;
	this->_run = run;
	this->_spe = spe;	
}

QString BubbleInstance::getDiscreption(int index)
{
	switch( index )
	{
	case 1:
		return this->_run;
	case 2:
		return this->_ind;
	case 3:
		return this->_spe;
	case 4:
		return this->_speLabel;
	}
	return "";
}

void BubbleInstance::setSpeLabel( QString speLabel)
{
	this->_speLabel = speLabel;
		
}
