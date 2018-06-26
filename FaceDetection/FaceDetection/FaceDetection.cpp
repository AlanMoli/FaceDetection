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
	//����
	setWindowTitle("FaceDetection");
	//Icon
	setWindowIcon(QIcon());
	
	//���ع�������״̬��
	ui.mainToolBar->setHidden(true);
	ui.statusBar->setHidden(true);

	//�����С��λ��
	resize(QApplication::desktop()->width()*0.8, QApplication::desktop()->screenGeometry().height()*0.8);
	move((QApplication::desktop()->width() - width())/2 , (QApplication::desktop()->height() - height())/3);

	//ɾ�����봰��
	QWidget *p = takeCentralWidget();
	if (p)
	{
		delete p;
	}
	
	//�ļ��˵�
	Menu_File();
	//��ʾ�˵�
	Menu_Show();
	//�����˵�
	Menu_Help();
	//��ʼ��ͼ��
	InitImage();
	SetStyle::setStyle("dracula.qss");
}

//�ļ��˵�
void FaceDetection::Menu_File()
{
	QMenu *file = menuBar()->addMenu(tr("�ļ�"));

	//��̬���˵�
	QAction *Static_detection = new QAction(QIcon(), tr("��̬���"), this);
	
	//����Ӳ˵�
	QMenu *static_detection = new QMenu();
	Static_detection->setMenu(static_detection);

	//��ͼƬ
	QAction *Act_file_open = new QAction(QIcon(), tr("��ͼƬ"), this);
	//���ÿ�ݼ�
	Act_file_open->setShortcuts(QKeySequence::Open);
	//�����ź����
	connect(Act_file_open, SIGNAL(triggered()), this, SLOT(File_open()));
	
	//�������
	QAction *Act_frontalface_open = new QAction(QIcon(), tr("�������"), this);
	connect(Act_frontalface_open, SIGNAL(triggered()), this, SLOT(FrontFace_open()));

	//�������
	QAction *Act_profileface_open = new QAction(QIcon(), tr("�������"), this);
	connect(Act_profileface_open, SIGNAL(triggered()), this, SLOT(ProfileFace_open()));

	static_detection->addAction(Act_file_open);
	static_detection->addAction(Act_frontalface_open);
	static_detection->addAction(Act_profileface_open);

	//��̬׷�ٲ˵�
	QAction *Dynamic_tracking = new QAction(QIcon(), tr("��̬׷��"), this);

	//����Ӳ˵�
	QMenu *dynamic_tracking = new QMenu();
	Dynamic_tracking->setMenu(dynamic_tracking);

	//������ͷ
	QAction *Act_camera_open = new QAction(QIcon(), tr("������ͷ"), this);
	Act_camera_open->setShortcuts(QKeySequence::Copy);
	connect(Act_camera_open, SIGNAL(triggered()), this, SLOT(Camera_open()));

	//һ������
	QAction *Act_camera_beauty = new QAction(QIcon(), tr("һ������"), this);
	connect(Act_camera_beauty, SIGNAL(triggered()), this, SLOT(Camera_beauty()));

	//��������ͷ
	QAction *Act_camera_save = new QAction(QIcon(), tr("����ͼƬ"), this);
	Act_camera_save->setShortcuts(QKeySequence::Save);
	connect(Act_camera_save, SIGNAL(triggered()), this, SLOT(Camera_save()));

	//�ر�����ͷ
	QAction *Act_camera_close = new QAction(QIcon(), tr("�ر�����ͷ"), this);
	Act_camera_close->setShortcuts(QKeySequence::Undo);
	connect(Act_camera_close, SIGNAL(triggered()), this, SLOT(Camera_close()));

	dynamic_tracking->addAction(Act_camera_open);
	dynamic_tracking->addAction(Act_camera_beauty);
	dynamic_tracking->addAction(Act_camera_save);
	dynamic_tracking->addAction(Act_camera_close);

	//�˳�
	QAction *Act_file_close = new QAction(QIcon(), tr("�ر�"), this);
	Act_file_close->setShortcuts(QKeySequence::Close);
	connect(Act_file_close, SIGNAL(triggered()), this, SLOT(File_close()));
	
	file->addAction(Static_detection);
	file->addAction(Dynamic_tracking);
	//��ӷָ���
	file->addSeparator();
	file->addAction(Act_file_close);
}

//��ʾ�˵�
void FaceDetection::Menu_Show()
{
	QMenu *show = menuBar()->addMenu("��ʾ");

	QAction *Act_full_screen = new QAction(QIcon(), tr("ȫ����ʾ"), this);
	connect(Act_full_screen, SIGNAL(triggered()), this, SLOT(Full_screen()));
	show->addAction(Act_full_screen);

	QAction *Act_exit_screen = new QAction(QIcon(), tr("�˳�ȫ��"), this);
	connect(Act_exit_screen, SIGNAL(triggered()), this, SLOT(Exit_screen()));
	show->addAction(Act_exit_screen);

	QAction *Skin = new QAction(QIcon(), tr("�л�Ƥ��"), this);
	connect(Skin, SIGNAL(triggered()), this, SLOT(Select_skin()));
	show->addAction(Skin);
}

//�����˵�
void FaceDetection::Menu_Help()
{
	QMenu *help = menuBar()->addMenu(tr("����"));

	//����
	QAction *Act_showWin = new QAction(QIcon(), tr("����"), this);
	connect(Act_showWin, SIGNAL(triggered()), this, SLOT(showWin()));
	help->addAction(Act_showWin);
}

//ͼ���ʼ��
void FaceDetection::InitImage()
{
	//Դͼ�����
	dock_srcImage = new QDockWidget(tr("Դͼ��"), this);
	//ͣ����ര��
	addDockWidget(Qt::LeftDockWidgetArea, dock_srcImage);

	imgLabelLeft = new QLabel(dock_srcImage);
	//QLabelLeft�Զ���Ӧͼ���С
	imgLabelLeft->setScaledContents(true);

	//��ʼͼ��
	QImage image = QImage(500, 500, QImage::Format_RGB32);
	
	//��ɫͼ��
	image.fill(qRgb(255, 255, 255));
	//��ʾͼ��
	imgLabelLeft->setPixmap(QPixmap::fromImage(image));
	imgLabelLeft->resize(image.width(), image.height());

	//���ӹ�����
	scrollAreaL = new QScrollArea(this);
	scrollAreaL->setBackgroundRole(QPalette::Dark);
	scrollAreaL->setAlignment(Qt::AlignCenter);
	scrollAreaL->setWidget(imgLabelLeft);
	dock_srcImage->setWidget(scrollAreaL);

	//���ͼ�����
	dock_detImage = new QDockWidget(tr("���ͼ��"), this);
	//ͣ���Ҳര��
	addDockWidget(Qt::RightDockWidgetArea, dock_detImage);

	imgLabelRight = new QLabel(dock_detImage);
	imgLabelRight->setScaledContents(true);

	QImage image2 = QImage(500, 500, QImage::Format_RGB32);
	image2.fill(qRgb(255, 255, 255));
	//��ʾͼ��
	imgLabelRight->setPixmap(QPixmap::fromImage(image2));
	imgLabelRight->resize(image2.width(), image2.height());

	//���ӹ�����
	scrollAreaR = new QScrollArea(this);
	scrollAreaR->setBackgroundRole(QPalette::Dark);
	scrollAreaR->setAlignment(Qt::AlignCenter);
	scrollAreaR->setWidget(imgLabelRight);
	dock_detImage->setWidget(scrollAreaR);

	//�ָ��
	splitDockWidget(dock_srcImage, dock_detImage, Qt::Horizontal);
}

//��ͼƬ
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

	QString path = QFileDialog::getOpenFileName(this, tr("ѡ��ͼ��"), ".", tr("Image(*.jpg)"));
	
	if (!path.isEmpty())
	{
		QImage *img = new QImage();
		if (!(img->load(path)))
		{
			QMessageBox::information(this, tr("����"), tr("��ͼ��ʧ�ܣ�"));
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

//�������
void FaceDetection::FrontFace_open()
{
	String face_cascade_name = "haarcascade_frontalface_alt.xml";
	CascadeClassifier face_cascade;

	Mat src_img = imread(currentPath.toLatin1().data());
	Mat gray_img;

	//��ȡ����
	if (!face_cascade.load(face_cascade_name))
	{
		return;
	}

	//תΪ�Ҷ�ͼ
	cvtColor(src_img, gray_img, COLOR_BGR2GRAY);
	//����ֱ��ͼ���⻯
	equalizeHist(gray_img, gray_img);

	//�������
	vector<Rect> faces;
	//���ط�����
	face_cascade.detectMultiScale(gray_img, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

	size_t i = 0;
	for (i; i < faces.size(); i++)
	{
		Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);
		//������Բ����Ǽ�⵽����	
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

//�������
void FaceDetection::ProfileFace_open()
{
	String face_cascade_name = "haarcascade_profileface.xml";
	CascadeClassifier face_cascade;

	Mat src_img = imread(currentPath.toLatin1().data());
	Mat gray_img;

	//��ȡ����
	if (!face_cascade.load(face_cascade_name))
	{
		return;
	}

	//תΪ�Ҷ�ͼ
	cvtColor(src_img, gray_img, COLOR_BGR2GRAY);
	//����ֱ��ͼ���⻯
	equalizeHist(gray_img, gray_img);

	//�������
	vector<Rect> faces;
	//���ط�����
	face_cascade.detectMultiScale(gray_img, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

	size_t i = 0;
	for (i; i < faces.size(); i++)
	{
		Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);
		//������Բ����Ǽ�⵽����	
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

//������ͷ
void FaceDetection::Camera_open()
{
	timer = new QTimer(this);
	timer->start(33);
	capture.open(0);
	connect(timer, SIGNAL(timeout()), this, SLOT(showImage()));
}	

//��������ͷ
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

//�ر�����ͷ
void FaceDetection::Camera_close()
{
	timer->stop();
	capture.release();
}

void FaceDetection::File_close()																			
{	
	this->close();
}

//�˳���ʾ
void FaceDetection::closeEvent(QCloseEvent *event)
{
	switch (QMessageBox::information(this, tr("�˳�"), tr("�Ƿ�ȷ���˳���"), tr("��"), tr("��"), 0, 1))
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

//ȫ����ʾ
void FaceDetection::Full_screen()
{
	setWindowState(Qt::WindowFullScreen);
}

//�˳�ȫ��
void FaceDetection::Exit_screen()
{
	setWindowState(Qt::WindowNoState);
}

//�л�Ƥ��
void FaceDetection::Select_skin()
{
	QDialog *skin = new QDialog();
	skin->resize(600, 400);

	QLabel *Left = new QLabel(skin);
	Left->setScaledContents(true);

	//��ʼͼ��
	QImage image = QImage(250, 250, QImage::Format_RGB32);

	//��ɫͼ��
	image.fill(qRgb(255, 255, 255));

	Left->setPixmap(QPixmap::fromImage(image));
	Left->resize(image.width(), image.height());

	QImage *img1 = new QImage();
	img1->load("../images/dark.png");
	*img1 = autoSize(*img1, Left);

	//��ʾͼ��
	Left->setPixmap(QPixmap::fromImage(*img1));
	Left->resize(img1->width(), img1->height());

	Left->setStyleSheet("QLabel{border:1px solid rgb(187, 212, 238);}");

	QLabel *Right = new QLabel(skin);
	Right->setScaledContents(true);

	QImage image2 = QImage(250, 250, QImage::Format_RGB32);
	image2.fill(qRgb(255, 255, 255));
	//��ʾͼ��
	Right->setPixmap(QPixmap::fromImage(image2));
	Right->resize(image2.width(), image2.height());

	QImage *img2 = new QImage();
	img2->load("../images/light.png");
	*img2 = autoSize(*img2, Right);

	//��ʾͼ��
	Right->setPixmap(QPixmap::fromImage(*img2));
	Right->resize(img2->width(), img2->height());

	Right->setStyleSheet("QLabel{border:1px rgb(187, 212, 238);}");

	QRadioButton *radio1 = new QRadioButton(tr("��ɫ�ſ�"), skin);
	QRadioButton *radio2 = new QRadioButton(tr("��ɫ����"), skin);

	// �����趨��ʼֵ
	radio1->setChecked(true);       
	
	connect(radio1, SIGNAL(clicked()), this, SLOT(skin1()));
	connect(radio2, SIGNAL(clicked()), this, SLOT(skin2()));

	//��ѡ��ť��
	QButtonGroup *radioGroup = new QButtonGroup();
	//���ڰ�ť����
	radioGroup->setExclusive(true);                 
	radioGroup->addButton(radio1);
	radioGroup->addButton(radio2);

	Left->move(30, 100);
	Right->move(320, 100);
	radio1->move(100, 250);
	radio2->move(400, 250);

	//ģ̬�Ի��򣬹رմ˴���ǰ���ܶ����ര�ڽ������
	skin->exec();
}

//����
void FaceDetection::showWin()
{
	//�������ʵ����
	CustomWindow *helpWin = new CustomWindow();
	helpWin->resize(600, 400);

	QLabel *label_about = new QLabel(helpWin);
	label_about->setText(tr("�������ϵͳ 1.0 ��"));
	QLabel *label_right = new QLabel(helpWin);
	label_right->setText(tr("Copyright (C) 2018 ��ͨ "));
	QLabel *label_author = new QLabel(helpWin);
	label_author->setText(tr("���ߣ����"));

	QPushButton *button_ok = new QPushButton(helpWin);
	button_ok->setText(tr("ȷ��"));
	connect(button_ok, SIGNAL(clicked()), helpWin, SLOT(close()));

	label_about->move(100, 100);
	label_right->move(100, 180);
	label_author->move(100, 260);
	button_ok->move(400, 172);

	//ģ̬�Ի��򣬹رմ˴���ǰ���ܶ����ര�ڽ������
	helpWin->exec();
}

//ͼƬ����Ӧ��С
QImage FaceDetection::autoSize(QImage img, QLabel *label)
{
	QImage Img;
	//ͼ���߱�
	double ImgRatio = 1.0 * img.width() / img.height();   
	//���ڿ�߱�
	double WinRatio = 1.0 * (label->width() - 2) / (label->height() - 2); 
	//ͼ���߱ȴ���ͼ���߱�
	if (ImgRatio > WinRatio)        
	{
		Img = img.scaled((label->width() - 2), (label->width() - 2) / ImgRatio, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	}
	//ͼ���߱�С�ڵ���ͼ���߱�
	else                            
	{
		Img = img.scaled((label->height() - 2) * ImgRatio, (label->height() - 2), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	}
	return Img;
}

//Mat��ת��ΪQImage��
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

//QImage��ת��ΪMat��
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

//��̬׷��
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

	//��ȡ����
	if (!face_cascade.load(face_cascade_name))
	{
		return;
	}

	//תΪ�Ҷ�ͼ
	cvtColor(frame, gray_img, COLOR_BGR2GRAY);
	//����ֱ��ͼ���⻯
	equalizeHist(gray_img, gray_img);

	//�������
	vector<Rect> faces;
	//���ط�����
	face_cascade.detectMultiScale(gray_img, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

	size_t i = 0;
	size_t j = 0;
	for (i; i < faces.size(); i++)
		//for(j; j < faces2.size(); j++)
	{
		Point center1(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);
		//������Բ����Ǽ�⵽����	
		ellipse(gray_img, center1, Size(faces[i].width / 2, faces[i].height / 2), 0, 0, 360,
			Scalar(255, 0, 255), 4, 8, 0);
	}

	//Mat��תΪQImage��
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