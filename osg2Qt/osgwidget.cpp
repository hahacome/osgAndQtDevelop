#include "osgwidget.h"
#include <osg/TexEnv>
#include <osgDB/ReadFile>
#include <osgGA/MultiTouchTrackballManipulator>

osgWidget::osgWidget(QWidget *parent)
	: QWidget(parent)
{

	//����CompositeViewer
	setThreadingModel(SingleThreaded); 
	setKeyEventSetsDone(0);
	_viewer = new osgViewer::Viewer;
	addView(_viewer);//��CompositeViewer���viewer

	//����GraphicsWindowQt���豸��������
	_gw = createGraphicsWindow(0, 0, 100, 100);
	osg::ref_ptr<osg::Camera> camera = _viewer->getCamera();//��ȡ�Ӿ��������
	camera->setGraphicsContext(_gw);//��������������ʾ�豸��������������������������
	_gw->setTouchEventsEnabled(true);

	//�������
	const osg::GraphicsContext::Traits* traits = _gw->getTraits();
	camera->setClearColor(osg::Vec4(0.2, 0.2, 0.6, 1.0));
	camera->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));
	camera->setProjectionMatrixAsPerspective(30.0f, static_cast<double>(traits->width) / static_cast<double>(traits->height), 1.0f, 10000.0f);
	_viewer->setCameraManipulator(new osgGA::MultiTouchTrackballManipulator); //�������������

	
	//�򳡾������ģ��
	_nodeRoot = new osg::Group();
	_viewer->setSceneData(_nodeRoot);
	readModel("D:\\OpenSceneGraph340\\OpenSceneGraph-Data-3.0.0\\cow.osg");
	rePositionCamera(); //���ģ��֮����������Զ���Ӧģ�ʹ�Сʹģ�;�����ʾ


	//��GraphicsWindowQ��QWidgetȡ���������õ�this��QWidget��
	_widget = _gw->getGLWidget();
	_grid = new QGridLayout;
	setLayout(_grid);
	_grid->setMargin(1);
	_grid->addWidget(_widget, 0, 0);


	//����QWidget��ʱ����
	connect(&_timer, SIGNAL(timeout()), this, SLOT(update()));
	_timer.start(10);
}

void osgWidget::paintEvent(QPaintEvent* event)//��дQWidget�Ļ��ƺ���
{
	frame(); //����CompositeViewer��frame������ˢ�´���
}

void osgWidget::rePositionCamera()
{//--------------�����������Ӧ
	_viewer->setCameraManipulator(NULL);//���ﲻʹ������������Ҫ�Լ��ֶ��رգ���ΪOSGĬ���˻����TrackballManipulator
	//*�����������*//  
	double radius = _nodeRoot->getBound().radius();
	double viewDistance = radius * 3;
	osg::ref_ptr<osg::Camera> camera = _viewer->getCamera();
	osg::Vec3d objCenter = _nodeRoot->getBound().center();//��ȡģ�͵İ�Χ������  
	osg::Vec3 updirection(0.0, 0.0, 1.0);//�������  
	osg::Vec3 center = objCenter;//�ο������꣬ͨ������������  
	osg::Vec3d moveEve(0.0, -1.0, 0);
	osg::Vec3 eyepoint = center + moveEve * viewDistance;//���ӵ㣬�����λ��������λ�ü�center���롣  
	_viewer->getCamera()->setViewMatrixAsLookAt(eyepoint, center, updirection);
	_viewer->setCameraManipulator(new osgGA::MultiTouchTrackballManipulator); //�������������
}

void osgWidget::readModel(QString modelPath)
{
	//------------------ʹ����·����Ч
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
{//����ͼ�ν���
	osg::DisplaySettings* ds = osg::DisplaySettings::instance().get();

	//����ͼ�λ��Ľ������
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
