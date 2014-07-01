#include "PathBubbleBase.h"
/*
typedef boost::rectangle_topology<> topology_type;
typedef topology_type::point_type point_type;

typedef adjacency_list<listS, vecS, undirectedS, 
                       property<vertex_name_t, std::string> > Graph;

typedef graph_traits<Graph>::vertex_descriptor Vertex;

typedef std::map<std::string, Vertex> NameToVertex;

*/

PathBubbleBase::PathBubbleBase( int size, int x, int y, OpenGLScene* Scene, StatisticManager* manager, OpenManager* open, QString name)
: PlotBase(size, x, y, Scene, manager)
{
	//_scene= Scene;	
	this->_name = name;
	loadFile(name);
	//this->_open = open;	
}


PathBubbleBase::~PathBubbleBase()
{
	
}


void PathBubbleBase::loadFile(const QString &fileName)
{
	//if (maybeSave())
	{
		/*QFile file(fileName);
		if (!file.open(QFile::ReadOnly | QFile::Text)) 
		{
			return;
		}
		QTextStream in(&file);*/
		//_text->setText(in.readAll());

		setCurrentFile(fileName);
	}
}


void PathBubbleBase::setCurrentFile(const QString &fileName)
{
	_curFileName = fileName;
	_curFilePath = _curFileName;
	int tindex = _curFilePath.lastIndexOf("\\");
	_curFilePath.remove(tindex+1, _curFilePath.length());
}



void PathBubbleBase::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
	PlotBase::paint( painter, option, widget );
	
}


void PathBubbleBase::drawCurve( QPainter* painter )
{
	painter->save();
	//*************************************************************************
	painter->setPen( QPen( QBrush( QColor( 0, 0, 255, 255) ), 2 ) );
	painter->drawLines( _lines );
	//*************************************************************************
	painter->restore();
	return;
}

void PathBubbleBase::drawfills( QPainter* painter )
{
	/*painter->save();
	painter->setPen( QPen( QBrush( QColor( 0, 0, 255, 10) ), 2 ) );
	painter->drawLines( fills );
	painter->restore();*/
	painter->setBrush( QColor( 0, 0, 255, 100) );	
	painter->drawPolygon(_contain);
}

void PathBubbleBase::addLine( QPointF pos1, QPointF pos2 )
{
	//updateBoundingRect( pos1 );
	//updateBoundingRect( pos2 );
	_lines.append( QLineF( pos1, pos2 ) );
	return;
}

