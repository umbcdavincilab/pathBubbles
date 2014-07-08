/********************************************************************************
** Form generated from reading UI file 'winqt_test.ui'
**
** Created: Mon Jul 7 18:35:40 2014
**      by: Qt User Interface Compiler version 4.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WINQT_TEST_H
#define UI_WINQT_TEST_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_winQt_TestClass
{
public:
    QWidget *centralWidget;
    QFrame *frame;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *winQt_TestClass)
    {
        if (winQt_TestClass->objectName().isEmpty())
            winQt_TestClass->setObjectName(QString::fromUtf8("winQt_TestClass"));
        winQt_TestClass->resize(600, 400);
        centralWidget = new QWidget(winQt_TestClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        frame = new QFrame(centralWidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(10, 0, 571, 341));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        winQt_TestClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(winQt_TestClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 21));
        winQt_TestClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(winQt_TestClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        winQt_TestClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(winQt_TestClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        winQt_TestClass->setStatusBar(statusBar);

        retranslateUi(winQt_TestClass);

        QMetaObject::connectSlotsByName(winQt_TestClass);
    } // setupUi

    void retranslateUi(QMainWindow *winQt_TestClass)
    {
        winQt_TestClass->setWindowTitle(QApplication::translate("winQt_TestClass", "winQt_Test", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class winQt_TestClass: public Ui_winQt_TestClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WINQT_TEST_H
