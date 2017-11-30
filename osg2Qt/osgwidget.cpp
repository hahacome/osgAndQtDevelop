#include "osgwidget.h"
#include <osg/TexEnv>
#include <osgDB/ReadFile>
#include <osgGA/MultiTouchTrackballManipulator>

osgWidget::osgWidget(QWidget *parent)
	: QWidget(parent)
{

	//设置CompositeViewer
	setThreadingModel(SingleThreaded); 
	setKeyEventSetsDone(0);
	_viewer = new osgViewer::Viewer;
	addView(_viewer);//向CompositeViewer添加viewer

	//设置GraphicsWindowQt（设备描述符）
	_gw = createGraphicsWindow(0, 0, 100, 100);
	osg::ref_ptr<osg::Camera> camera = _viewer->getCamera();//获取视景器的相机
	camera->setGraphicsContext(_gw);//设置相机画面的显示设备！！！！！！！！！！！！！
	_gw->setTouchEventsEnabled(true);

	//相机设置
	const osg::GraphicsContext::Traits* traits = _gw->getTraits();
	camera->setClearColor(osg::Vec4(0.2, 0.2, 0.6, 1.0));
	camera->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));
	camera->setProjectionMatrixAsPerspective(30.0f, static_cast<double>(traits->width) / static_cast<double>(traits->height), 1.0f, 10000.0f);
	_viewer->setCameraManipulator(new osgGA::MultiTouchTrackballManipulator); //启用相机漫游器

	
	//向场景中添加模型
	_nodeRoot = new osg::Group();
	_viewer->setSceneData(_nodeRoot);
	readModel("D:\\OpenSceneGraph340\\OpenSceneGraph-Data-3.0.0\\cow.osg");
	rePositionCamera(); //添加模型之后设置相机自动适应模型大小使模型居中显示


	//把GraphicsWindowQ的QWidget取出来，放置到this的QWidget中
	_widget = _gw->getGLWidget();
	_grid = new QGridLayout;
	setLayout(_grid);
	_grid->setMargin(1);
	_grid->addWidget(_widget, 0, 0);


	//设置QWidget定时更新
	connect(&_timer, SIGNAL(timeout()), this, SLOT(update()));
	_timer.start(10);
}

void osgWidget::paintEvent(QPaintEvent* event)//重写QWidget的绘制函数
{
	frame(); //调用CompositeViewer的frame函数，刷新窗口
}

void osgWidget::rePositionCamera()
{//--------------设置相机自适应
	_viewer->setCameraManipulator(NULL);//这里不使用漫游器，需要自己手动关闭，因为OSG默认了会调用TrackballManipulator
	//*设置相机参数*//  
	double radius = _nodeRoot->getBound().radius();
	double viewDistance = radius * 3;
	osg::ref_ptr<osg::Camera> camera = _viewer->getCamera();
	osg::Vec3d objCenter = _nodeRoot->getBound().center();//获取模型的包围盒中心  
	osg::Vec3 updirection(0.0, 0.0, 1.0);//相机向上  
	osg::Vec3 center = objCenter;//参考点坐标，通常是物体中心  
	osg::Vec3d moveEve(0.0, -1.0, 0);
	osg::Vec3 eyepoint = center + moveEve * viewDistance;//将视点，即相机位置与物体位置即center分离。  
	_viewer->getCamera()->setViewMatrixAsLookAt(eyepoint, center, updirection);
	_viewer->setCameraManipulator(new osgGA::MultiTouchTrackballManipulator); //重启相机漫游器
}

void osgWidget::readModel(QString modelPath)
{
	//------------------使中文路径有效
	QByteArray ba = modelPath.toLocal8Bit();
	const char *c_str = ba.data();
	//==================
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile(c_str);
	//node->setName(c_str);
	_nodeRoot->addChild(node);
	//_nodeRoot->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	//osg::ref_ptr<osg::TexEnv> texenv = new osg::TexEnv;
	//_nodeRoot->getOrCreateStateSet()->setAttributeAndModes(texenv, osg::StateAttribute::ON);
}

osgQt::GraphicsWindowQt* osgWidget::createGraphicsWindow(int x, int y, int w, int h, const std::string& name, bool windowDecoration)
{//创建图形界面
	osg::DisplaySettings* ds = osg::DisplaySettings::instance().get();

	//设置图形化的界面参数
	osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
	traits->windowName = name;
	traits->windowDecoration = windowDecoration;
	traits->x = x;
	traits->y = y;
	traits->width = w;
	traits->height = h;
	traits->doubleBuffer = true;
	traits->alpha = ds->getMinimumNumAlphaBits();
	traits->stencil = ds->getMinimumNumStencilBits();
	traits->sampleBuffers = ds->getMultiSamples();
	traits->samples = ds->getNumMultiSamples();

	return new osgQt::GraphicsWindowQt(traits.get());
}



osgWidget::~osgWidget()
{
	delete _grid;
}
