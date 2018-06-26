/********************************************************************************
** Form generated from reading UI file 'FaceDetection.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FACEDETECTION_H
#define UI_FACEDETECTION_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMdiArea>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FaceDetectionClass
{
public:
    QWidget *centralWidget;
    QMdiArea *mdiArea;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *FaceDetectionClass)
    {
        if (FaceDetectionClass->objectName().isEmpty())
            FaceDetectionClass->setObjectName(QStringLiteral("FaceDetectionClass"));
        FaceDetectionClass->resize(600, 400);
        centralWidget = new QWidget(FaceDetectionClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        mdiArea = new QMdiArea(centralWidget);
        mdiArea->setObjectName(QStringLiteral("mdiArea"));
        mdiArea->setGeometry(QRect(150, 60, 200, 160));
        FaceDetectionClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(FaceDetectionClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 18));
        FaceDetectionClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(FaceDetectionClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        FaceDetectionClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(FaceDetectionClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        FaceDetectionClass->setStatusBar(statusBar);

        retranslateUi(FaceDetectionClass);

        QMetaObject::connectSlotsByName(FaceDetectionClass);
    } // setupUi

    void retranslateUi(QMainWindow *FaceDetectionClass)
    {
        FaceDetectionClass->setWindowTitle(QApplication::translate("FaceDetectionClass", "FaceDetection", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FaceDetectionClass: public Ui_FaceDetectionClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FACEDETECTION_H
