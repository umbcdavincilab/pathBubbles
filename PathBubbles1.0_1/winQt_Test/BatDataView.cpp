#include "BatDataView.h"

BatDataView::BatDataView( StatisticManager* manager )
{
	setPos( QPointF(0,0) );
	readParaName();

	m_width = 50;
	m_height = 100;

	m_dataManager = manager;
}

BatDataView::~BatDataView(void)
{
}

void BatDataView::setPos(QPointF pos)
{
	m_pos = pos;
}

void BatDataView::readParaName()
{
	QFile file2( "data/1D_parameterData.txt" );
	if (!file2.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		return;
	}
	QTextStream in2(&file2);
	while (!in2.atEnd()) 
	{
		QString line = in2.readLine();
		m_paraName.append( line );
	}
}

void BatDataView::setBatName( QString str )
{
	batName = str;
}

void BatDataView::paintDataView( QPainter *painter )
{
	updateItems();

	painter->save();
	
	painter->setFont( QFont( "times", 10 ) );
	QRect rect = painter->fontMetrics().boundingRect( m_items[m_len] );
	m_width = rect.width()+10;
	m_height = rect.height()*m_items.size()+5;

	painter->setBrush( QBrush( QColor(161, 160, 203, 200)) );
	painter->drawRect( m_pos.x(), m_pos.y(), m_width, m_height );

	drawItems( painter );
	
	painter->restore();
}

void BatDataView::drawItems( QPainter *painter )
{
	painter->save();
	
	painter->setFont( QFont( "times", 10 ) );
	painter->setPen( QColor(Qt::black) );
	QRect rect = painter->fontMetrics().boundingRect( m_items[0] );

	for ( int i = 0; i < m_items.size(); i ++ )
	{
		painter->drawText( m_pos.x()+2, m_pos.y()+(i+1)*rect.height(), m_items[i] );
	}

	painter->restore();
}

void BatDataView::updateItems()
{
	m_items.clear();
	
	float val;
	QString str, strItem;
	int len = -1;
	
	for ( int i = 0; i < m_paraName.size(); i ++ )
	{
		val = m_dataManager->getValueAt( batName, m_paraName[i] );
		strItem = m_paraName[i];
		strItem += " = ";
		str.setNum( val );
		strItem += str;

		m_items.append( strItem );
	
		if ( len == -1 || strItem.size() > m_items[len].size() )
		{
			len = i;
		}
	}

	m_len = len;
}