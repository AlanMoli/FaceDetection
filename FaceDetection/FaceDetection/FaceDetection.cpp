#include "FaceDetection.h"
#include "CustomWindow.h"
#include "SetStyle.h"
#include <QtGui>
#include <QtWidgets>
#include <QCloseEvent>
#include <QTimer>

#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <vector>
#include <iostream>

#pragma execution_character_set("utf-8")

using namespace cv;
using namespace std;

FaceDetection::FaceDetection(QWidget *parent)
	: QMainWindow(parent)
{ 
	ui.setupUi(this);
	//标题
	setWindowTitle("FaceDetection");
	//Icon
	setWindowIcon(QIcon());
	
	//隐藏工具栏、状态栏
	ui.mainToolBar->setHidden(true);
	ui.statusBar->setHidden(true);

	//界面大小与位置
	resize(QApplication::desktop()->width()*0.8, QApplication::desktop()->screenGeometry().height()*0.8);
	move((QApplication::desktop()->width() - width())/2 , (QApplication::desktop()->height() - height())/3);

	//删除中央窗体
	QWidget *p = takeCentralWidget();
	if (p)
	{
		delete p;
	}
	
	//文件菜单
	Menu_File();
	//显示菜单
	Menu_Show();
	//帮助菜单
	Menu_Help();
	//初始化图像
	InitImage();
	SetStyle::setStyle("dracula.qss");
}

//文件菜单
void FaceDetection::Menu_File()
{
	QMenu *file = menuBar()->addMenu(tr("文件"));

	//静态检测菜单
	QAction *Static_detection = new QAction(QIcon(), tr("静态检测"), this);
	
	//添加子菜单
	QMenu *static_detection = new QMenu();
	Static_detection->setMenu(static_detection);

	//打开图片
	QAction *Act_file_open = new QAction(QIcon(), tr("打开图片"), this);
	//设置快捷键
	Act_file_open->setShortcuts(QKeySequence::Open);
	//连接信号与槽
	connect(Act_file_open, SIGNAL(triggered()), this, SLOT(File_open()));
	
	//正脸检测
	QAction *Act_frontalface_open = new QAction(QIcon(), tr("正脸检测"), this);
	connect(Act_frontalface_open, SIGNAL(triggered()), this, SLOT(FrontFace_open()));

	//侧脸检测
	QAction *Act_profileface_open = new QAction(QIcon(), tr("侧脸检测"), this);
	connect(Act_profileface_open, SIGNAL(triggered()), this, SLOT(ProfileFace_open()));

	static_detection->addAction(Act_file_open);
	static_detection->addAction(Act_frontalface_open);
	static_detection->addAction(Act_profileface_open);

	//动态追踪菜单
	QAction *Dynamic_tracking = new QAction(QIcon(), tr("动态追踪"), this);

	//添加子菜单
	QMenu *dynamic_tracking = new QMenu();
	Dynamic_tracking->setMenu(dynamic_tracking);

	//打开摄像头
	QAction *Act_camera_open = new QAction(QIcon(), tr("打开摄像头"), this);
	Act_camera_open->setShortcuts(QKeySequence::Copy);
	connect(Act_camera_open, SIGNAL(triggered()), this, SLOT(Camera_open()));

	//一键美颜
	QAction *Act_camera_beauty = new QAction(QIcon(), tr("一键美颜"), this);
	connect(Act_camera_beauty, SIGNAL(triggered()), this, SLOT(Camera_beauty()));

	//保存摄像头
	QAction *Act_camera_save = new QAction(QIcon(), tr("保存图片"), this);
	Act_camera_save->setShortcuts(QKeySequence::Save);
	connect(Act_camera_save, SIGNAL(triggered()), this, SLOT(Camera_save()));

	//关闭摄像头
	QAction *Act_camera_close = new QAction(QIcon(), tr("关闭摄像头"), this);
	Act_camera_close->setShortcuts(QKeySequence::Undo);
	connect(Act_camera_close, SIGNAL(triggered()), this, SLOT(Camera_close()));

	dynamic_tracking->addAction(Act_camera_open);
	dynamic_tracking->addAction(Act_camera_beauty);
	dynamic_tracking->addAction(Act_camera_save);
	dynamic_tracking->addAction(Act_camera_close);

	//退出
	QAction *Act_file_close = new QAction(QIcon(), tr("关闭"), this);
	Act_file_close->setShortcuts(QKeySequence::Close);
	connect(Act_file_close, SIGNAL(triggered()), this, SLOT(File_close()));
	
	file->addAction(Static_detection);
	file->addAction(Dynamic_tracking);
	//添加分割线
	file->addSeparator();
	file->addAction(Act_file_close);
}

//显示菜单
void FaceDetection::Menu_Show()
{
	QMenu *show = menuBar()->addMenu("显示");

	QAction *Act_full_screen = new QAction(QIcon(), tr("全屏显示"), this);
	connect(Act_full_screen, SIGNAL(triggered()), this, SLOT(Full_screen()));
	show->addAction(Act_full_screen);

	QAction *Act_exit_screen = new QAction(QIcon(), tr("退出全屏"), this);
	connect(Act_exit_screen, SIGNAL(triggered()), this, SLOT(Exit_screen()));
	show->addAction(Act_exit_screen);

	QAction *Skin = new QAction(QIcon(), tr("切换皮肤"), this);
	connect(Skin, SIGNAL(triggered()), this, SLOT(Select_skin()));
	show->addAction(Skin);
}

//帮助菜单
void FaceDetection::Menu_Help()
{
	QMenu *help = menuBar()->addMenu(tr("帮助"));

	//关于
	QAction *Act_showWin = new QAction(QIcon(), tr("关于"), this);
	connect(Act_showWin, SIGNAL(triggered()), this, SLOT(showWin()));
	help->addAction(Act_showWin);
}

//图像初始化
void FaceDetection::InitImage()
{
	//源图像组件
	dock_srcImage = new QDockWidget(tr("源图像"), this);
	//停靠左侧窗口
	addDockWidget(Qt::LeftDockWidgetArea, dock_srcImage);

	imgLabelLeft = new QLabel(dock_srcImage);
	//QLabelLeft自动适应图像大小
	imgLabelLeft->setScaledContents(true);

	//初始图像
	QImage image = QImage(500, 500, QImage::Format_RGB32);
	
	//白色图像
	image.fill(qRgb(255, 255, 255));
	//显示图像
	imgLabelLeft->setPixmap(QPixmap::fromImage(image));
	imgLabelLeft->resize(image.width(), image.height());

	//增加滚动条
	scrollAreaL = new QScrollArea(this);
	scrollAreaL->setBackgroundRole(QPalette::Dark);
	scrollAreaL->setAlignment(Qt::AlignCenter);
	scrollAreaL->setWidget(imgLabelLeft);
	dock_srcImage->setWidget(scrollAreaL);

	//检测图像组件
	dock_detImage = new QDockWidget(tr("检测图像"), this);
	//停靠右侧窗口
	addDockWidget(Qt::RightDockWidgetArea, dock_detImage);

	imgLabelRight = new QLabel(dock_detImage);
	imgLabelRight->setScaledContents(true);

	QImage image2 = QImage(500, 500, QImage::Format_RGB32);
	image2.fill(qRgb(255, 255, 255));
	//显示图像
	imgLabelRight->setPixmap(QPixmap::fromImage(image2));
	imgLabelRight->resize(image2.width(), image2.height());

	//增加滚动条
	scrollAreaR = new QScrollArea(this);
	scrollAreaR->setBackgroundRole(QPalette::Dark);
	scrollAreaR->setAlignment(Qt::AlignCenter);
	scrollAreaR->setWidget(imgLabelRight);
	dock_detImage->setWidget(scrollAreaR);

	//分割窗口
	splitDockWidget(dock_srcImage, dock_detImage, Qt::Horizontal);
}

//打开图片
void FaceDetection::File_open()
{	
	QImage image = QImage(500, 500, QImage::Format_RGB32);
	image.fill(qRgb(255, 255, 255));
	imgLabelLeft->setPixmap(QPixmap::fromImage(image));
	imgLabelLeft->resize(image.width(), image.height());

	QImage image2 = QImage(500, 500, QImage::Format_RGB32);
	image.fill(qRgb(255, 255, 255));
	imgLabelRight->setPixmap(QPixmap::fromImage(image2));
	imgLabelRight->resize(image2.width(), image2.height());

	currentPath = "";

	QString path = QFileDialog::getOpenFileName(this, tr("选择图像"), ".", tr("Image(*.jpg)"));
	
	if (!path.isEmpty())
	{
		QImage *img = new QImage();
		if (!(img->load(path)))
		{
			QMessageBox::information(this, tr("错误"), tr("打开图像失败！"));
			delete img;
			return;
		}
		
		*img = autoSize(*img, imgLabelLeft);
		
		imgLabelLeft->setPixmap(QPixmap::fromImage(*img));
		imgLabelLeft->resize(img->width(), img->height());
		
		currentPath = path;
	}
}

QImage cvMat2QImage(const cv::Mat& mat);

//正脸检测
void FaceDetection::FrontFace_open()
{
	String face_cascade_name = "haarcascade_frontalface_alt.xml";
	CascadeClassifier face_cascade;

	Mat src_img = imread(currentPath.toLatin1().data());
	Mat gray_img;

	//读取级联
	if (!face_cascade.load(face_cascade_name))
	{
		return;
	}

	//转为灰度图
	cvtColor(src_img, gray_img, COLOR_BGR2GRAY);
	//进行直方图均衡化
	equalizeHist(gray_img, gray_img);

	//脸部检测
	vector<Rect> faces;
	//加载分类器
	face_cascade.detectMultiScale(gray_img, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

	size_t i = 0;
	for (i; i < faces.size(); i++)
	{
		Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);
		//绘制椭圆，标记检测到的脸	
		ellipse(gray_img, center, Size(faces[i].width / 2, faces[i].height / 2), 0, 0, 360,
			Scalar(255, 0, 255), 4, 8, 0);
	}

	QVector<QRgb> grayTable;
	for (int j = 0; j < 256; j++)
	{
		grayTable.push_back(qRgb(j, j, j));
	}

	QImage image = cvMat2QImage(gray_img);

	image.setColorTable(grayTable);

	image = autoSize(image, imgLabelRight);
	imgLabelRight->setPixmap(QPixmap::fromImage(image));
	imgLabelRight->resize(image.width(), image.height());
}

//侧脸检测
void FaceDetection::ProfileFace_open()
{
	String face_cascade_name = "haarcascade_profileface.xml";
	CascadeClassifier face_cascade;

	Mat src_img = imread(currentPath.toLatin1().data());
	Mat gray_img;

	//读取级联
	if (!face_cascade.load(face_cascade_name))
	{
		return;
	}

	//转为灰度图
	cvtColor(src_img, gray_img, COLOR_BGR2GRAY);
	//进行直方图均衡化
	equalizeHist(gray_img, gray_img);

	//脸部检测
	vector<Rect> faces;
	//加载分类器
	face_cascade.detectMultiScale(gray_img, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

	size_t i = 0;
	for (i; i < faces.size(); i++)
	{
		Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);
		//绘制椭圆，标记检测到的脸	
		ellipse(gray_img, center, Size(faces[i].width / 2, faces[i].height / 2), 0, 0, 360,
			Scalar(255, 0, 255), 4, 8, 0);
	}

	QVector<QRgb> grayTable;
	for (int j = 0; j < 256; j++)
	{
		grayTable.push_back(qRgb(j, j, j));
	}

	QImage image = cvMat2QImage(gray_img);

	image.setColorTable(grayTable);

	image = autoSize(image, imgLabelRight);
	imgLabelRight->setPixmap(QPixmap::fromImage(image));
	imgLabelRight->resize(image.width(), image.height());
}

//打开摄像头
void FaceDetection::Camera_open()
{
	timer = new QTimer(this);
	timer->start(33);
	capture.open(0);
	connect(timer, SIGNAL(timeout()), this, SLOT(showImage()));
}	

//保存摄像头
void FaceDetection::Camera_save()
{
	Mat frame;

	if (!capture.isOpened())
	{
		capture.open(0);
	}
	capture >> frame;

	namedWindow("Photo", WINDOW_NORMAL);
	imshow("Photo", frame);
	imwrite("F:\\Qt\\FaceDetection\\images\\test.jpg", frame);
}

//关闭摄像头
void FaceDetection::Camera_close()
{
	timer->stop();
	capture.release();
}

void FaceDetection::File_close()																			
{	
	this->close();
}

//退出提示
void FaceDetection::closeEvent(QCloseEvent *event)
{
	switch (QMessageBox::information(this, tr("退出"), tr("是否确认退出？"), tr("是"), tr("否"), 0, 1))
	{
		case 0:
			event->accept();
			break;
		case 1:
		default:
			event->ignore();
			break;
	}
}

//全屏显示
void FaceDetection::Full_screen()
{
	setWindowState(Qt::WindowFullScreen);
}

//退出全屏
void FaceDetection::Exit_screen()
{
	setWindowState(Qt::WindowNoState);
}

//切换皮肤
void FaceDetection::Select_skin()
{
	QDialog *skin = new QDialog();
	skin->resize(600, 400);

	QLabel *Left = new QLabel(skin);
	Left->setScaledContents(true);

	//初始图像
	QImage image = QImage(250, 250, QImage::Format_RGB32);

	//白色图像
	image.fill(qRgb(255, 255, 255));

	Left->setPixmap(QPixmap::fromImage(image));
	Left->resize(image.width(), image.height());

	QImage *img1 = new QImage();
	img1->load("../images/dark.png");
	*img1 = autoSize(*img1, Left);

	//显示图像
	Left->setPixmap(QPixmap::fromImage(*img1));
	Left->resize(img1->width(), img1->height());

	Left->setStyleSheet("QLabel{border:1px solid rgb(187, 212, 238);}");

	QLabel *Right = new QLabel(skin);
	Right->setScaledContents(true);

	QImage image2 = QImage(250, 250, QImage::Format_RGB32);
	image2.fill(qRgb(255, 255, 255));
	//显示图像
	Right->setPixmap(QPixmap::fromImage(image2));
	Right->resize(image2.width(), image2.height());

	QImage *img2 = new QImage();
	img2->load("../images/light.png");
	*img2 = autoSize(*img2, Right);

	//显示图像
	Right->setPixmap(QPixmap::fromImage(*img2));
	Right->resize(img2->width(), img2->height());

	Right->setStyleSheet("QLabel{border:1px rgb(187, 212, 238);}");

	QRadioButton *radio1 = new QRadioButton(tr("黑色炫酷"), skin);
	QRadioButton *radio2 = new QRadioButton(tr("白色靓丽"), skin);

	// 用于设定初始值
	radio1->setChecked(true);       
	
	connect(radio1, SIGNAL(clicked()), this, SLOT(skin1()));
	connect(radio2, SIGNAL(clicked()), this, SLOT(skin2()));

	//单选按钮组
	QButtonGroup *radioGroup = new QButtonGroup();
	//组内按钮互斥
	radioGroup->setExclusive(true);                 
	radioGroup->addButton(radio1);
	radioGroup->addButton(radio2);

	Left->move(30, 100);
	Right->move(320, 100);
	radio1->move(100, 250);
	radio2->move(400, 250);

	//模态对话框，关闭此窗口前不能对其余窗口进项操作
	skin->exec();
}

//关于
void FaceDetection::showWin()
{
	//对类进行实例化
	CustomWindow *helpWin = new CustomWindow();
	helpWin->resize(600, 400);

	QLabel *label_about = new QLabel(helpWin);
	label_about->setText(tr("人脸检测系统 1.0 版"));
	QLabel *label_right = new QLabel(helpWin);
	label_right->setText(tr("Copyright (C) 2018 南通 "));
	QLabel *label_author = new QLabel(helpWin);
	label_author->setText(tr("作者：昊天"));

	QPushButton *button_ok = new QPushButton(helpWin);
	button_ok->setText(tr("确定"));
	connect(button_ok, SIGNAL(clicked()), helpWin, SLOT(close()));

	label_about->move(100, 100);
	label_right->move(100, 180);
	label_author->move(100, 260);
	button_ok->move(400, 172);

	//模态对话框，关闭此窗口前不能对其余窗口进项操作
	helpWin->exec();
}

//图片自适应大小
QImage FaceDetection::autoSize(QImage img, QLabel *label)
{
	QImage Img;
	//图像宽高比
	double ImgRatio = 1.0 * img.width() / img.height();   
	//窗口宽高比
	double WinRatio = 1.0 * (label->width() - 2) / (label->height() - 2); 
	//图像宽高比大于图像宽高比
	if (ImgRatio > WinRatio)        
	{
		Img = img.scaled((label->width() - 2), (label->width() - 2) / ImgRatio, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	}
	//图像宽高比小于等于图像宽高比
	else                            
	{
		Img = img.scaled((label->height() - 2) * ImgRatio, (label->height() - 2), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	}
	return Img;
}

//Mat类转换为QImage类
QImage cvMat2QImage(const cv::Mat& mat)
{
	// 8-bits unsigned, NO. OF CHANNELS = 1  
	if (mat.type() == CV_8UC1)
	{
		QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
		//Set the color table (used to translate colour indexes to qRgb values)  
		image.setColorCount(256);
		for (int i = 0; i < 256; i++)
		{
			image.setColor(i, qRgb(i, i, i));
		}
		//Copy input Mat  
		uchar *pSrc = mat.data;
		for (int row = 0; row < mat.rows; row++)
		{
			uchar *pDest = image.scanLine(row);
			memcpy(pDest, pSrc, mat.cols);
			pSrc += mat.step;
		}
		return image;
	}
	// 8-bits unsigned, NO. OF CHANNELS = 3  
	else if (mat.type() == CV_8UC3)
	{
		// Copy input Mat  
		const uchar *pSrc = (const uchar*)mat.data;
		// Create QImage with same dimensions as input Mat  
		QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
		return image.rgbSwapped();
	}
	else if (mat.type() == CV_8UC4)
	{
		qDebug() << "CV_8UC4";
		// Copy input Mat  
		const uchar *pSrc = (const uchar*)mat.data;
		// Create QImage with same dimensions as input Mat  
		QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
		return image.copy();
	}
	else
	{
		qDebug() << "ERROR: Mat could not be converted to QImage.";
		return QImage();
	}
}

//QImage类转换为Mat类
cv::Mat QImage2cvMat(QImage image)
{
	cv::Mat mat;
	qDebug() << image.format();
	switch (image.format())
	{
	case QImage::Format_ARGB32:
	case QImage::Format_RGB32:
	case QImage::Format_ARGB32_Premultiplied:
		mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
		break;
	case QImage::Format_RGB888:
		mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
		cv::cvtColor(mat, mat, CV_BGR2RGB);
		break;
	case QImage::Format_Indexed8:
		mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
		break;
	}
	return mat;
}

//动态追踪
void FaceDetection::showImage()
{
	Mat gray_img;
	capture >> frame;
	waitKey(1);
	
	QImage image = cvMat2QImage(frame);
	imgLabelLeft->setPixmap(QPixmap::fromImage(image));
	imgLabelLeft->resize(image.width(), image.height());

	String face_cascade_name = "haarcascade_frontalface_alt.xml";
	CascadeClassifier face_cascade;

	//读取级联
	if (!face_cascade.load(face_cascade_name))
	{
		return;
	}

	//转为灰度图
	cvtColor(frame, gray_img, COLOR_BGR2GRAY);
	//进行直方图均衡化
	equalizeHist(gray_img, gray_img);

	//脸部检测
	vector<Rect> faces;
	//加载分类器
	face_cascade.detectMultiScale(gray_img, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

	size_t i = 0;
	size_t j = 0;
	for (i; i < faces.size(); i++)
		//for(j; j < faces2.size(); j++)
	{
		Point center1(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);
		//绘制椭圆，标记检测到的脸	
		ellipse(gray_img, center1, Size(faces[i].width / 2, faces[i].height / 2), 0, 0, 360,
			Scalar(255, 0, 255), 4, 8, 0);
	}

	//Mat类转为QImage类
	QImage image2 = cvMat2QImage(gray_img);
	imgLabelRight->setPixmap(QPixmap::fromImage(image2));
	imgLabelRight->resize(image2.width(), image2.height());
}

void FaceDetection::skin1()
{
	SetStyle::setStyle("dracula.qss");
}

void FaceDetection::skin2()
{
	SetStyle::setStyle("light.qss");
}