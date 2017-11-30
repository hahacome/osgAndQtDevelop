#include "osg2qt.h"
#include "osglib.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	osg2Qt w;
	w.show();
	return a.exec();
}
