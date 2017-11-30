#include "osg2qt.h"

osg2Qt::osg2Qt(QWidget *parent)
	: QMainWindow(parent)
{
	//----------------------------布局
	_panel = new QWidget(this);
	_layout = new QHBoxLayout(_panel);
	_layout->setMargin(0);
	_layout1 = new QVBoxLayout(_panel);
	_layout1->setContentsMargins(0, 1, 2, 1);
	_layout2 = new QVBoxLayout(_panel);

	_osgWidget = new osgWidget(this);//新建一个osg视图控件
	_layout2->addWidget(_osgWidget);
	_layout2->setMargin(0);

	_layout->setSpacing(0);
	_layout->addLayout(_layout1);//在layout1中可以放置其他的控件，比如列表、按钮等
	_layout->addLayout(_layout2);
	_layout->setStretchFactor(_layout1, 1);//设置比例值为1
	_layout->setStretchFactor(_layout2, 4);//设置比例值为4
	_panel->setLayout(_layout);
	this->setCentralWidget(_panel);
	setGeometry(400, 200, 700, 500);
	//============================

}



osg2Qt::~osg2Qt()
{

}
