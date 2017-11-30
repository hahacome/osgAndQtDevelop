#ifndef OSG2QT_H
#define OSG2QT_H

#include <QtWidgets/QMainWindow>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include "osgwidget.h"

class osg2Qt : public QMainWindow
{
	Q_OBJECT

public:
	osg2Qt(QWidget *parent = 0);
	~osg2Qt();

private:
	QHBoxLayout* _layout;
	QVBoxLayout* _layout1;
	QVBoxLayout * _layout2;
	QWidget* _panel;
	osg::ref_ptr<osgWidget> _osgWidget;
};

#endif // OSG2QT_H
