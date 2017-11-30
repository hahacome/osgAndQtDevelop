#ifndef OSGWIDGET_H
#define OSGWIDGET_H

#include <QWidget>
#include "osgViewer/CompositeViewer"
#include <QGridLayout>
#include <QTimer>
#include <osgQt/GraphicsWindowQt>
#include <osgViewer/Viewer>

class osgWidget : public QWidget, public osgViewer::CompositeViewer
{
	Q_OBJECT

public:
	osgWidget(QWidget *parent = 0);
	void readModel(QString modelPath);

	~osgWidget();
	virtual void paintEvent(QPaintEvent* event);// 重写QWidget的绘制事件

private:
	osgQt::GraphicsWindowQt* createGraphicsWindow(int x, int y, int w, int h, const std::string& name="", bool windowDecoration=false);
	void rePositionCamera();

	osg::ref_ptr<osgQt::GraphicsWindowQt> _gw;
	osg::ref_ptr<osgViewer::Viewer> _viewer;
	osg::ref_ptr<osg::Group> _nodeRoot;
	QGridLayout* _grid;
	QWidget* _widget;
	QTimer _timer; //用于定时更新widget
};

#endif // OSGWIDGET_H
