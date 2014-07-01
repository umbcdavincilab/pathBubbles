
#include "newick_file.h"
#include "newick.h"
#include "tree.h"
using namespace BiRC::treelib;

#include <iostream>
#include <string>
#include <QFile>
#include <QString>
#include <QTextStream>
using namespace std;

// FIXME: There is no error handling here!
std::auto_ptr<Tree> BiRC::treelib::parse_newick_file(const char *fname)
{
	QFile curFile( fname );
	if ( curFile.open( QFile::ReadOnly | QFile::Text ))
	{
		QTextStream in( &curFile );
		string str = in.readAll().toStdString();
		curFile.close();

		std::auto_ptr<Tree> tree( parse_newick(str) );
		return tree;
	}
	else	return std::auto_ptr<Tree>(0);
}

std::auto_ptr<Tree> BiRC::treelib::parse_newick_file(const std::string &fname)
{
    return parse_newick_file(fname.c_str());
}
