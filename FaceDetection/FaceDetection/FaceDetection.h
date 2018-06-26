#pragma once

#include <QtWidgets/QMainWindow>
#include <QtGui>
#include <QtWidgets>
#include <QTimer>

#include <opencv2/highgui.hpp>
#include "ui_FaceDetection.h"

using namespace cv;

class FaceDetection : public QMainWindow
{
	Q_OBJECT

public:
	FaceDetection(QWidget *parent = Q_NULLPTR);

private:
	Ui::FaceDetectionClass ui;

	QDockWidget *dock_srcImage;
	QDockWidget *dock_detImage;
	QString currentPath;
	QLabel *imgLabelLeft;
	QLabel *imgLabelRight;
	QScrollArea *scrollAreaL;
	QScrollArea *scrollAreaR;
	QImage autoSize(QImage img, QLabel *label);
	QTimer *timer;

	VideoCapture capture;
	Mat frame;

	void Menu_File();
	void Menu_Show();
	void Menu_Help();
	void InitImage();

private slots:
	void File_open();
	void FrontFace_open();
	void ProfileFace_open();
	void Camera_open();
	//void Camera_beauty();
	void Camera_save();
	void Camera_close();
	void File_close();
	void Full_screen();
	void Exit_screen();
	void Select_skin();
	void showWin();
	//void Beauty();
	void showImage();
	void skin1();
	void skin2();
	void closeEvent(QCloseEvent *event);
};