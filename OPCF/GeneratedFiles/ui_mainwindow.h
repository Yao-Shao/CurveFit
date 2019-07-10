/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_OPCFClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *OPCFClass)
    {
        if (OPCFClass->objectName().isEmpty())
            OPCFClass->setObjectName(QString::fromUtf8("OPCFClass"));
        OPCFClass->resize(600, 400);
        menuBar = new QMenuBar(OPCFClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        OPCFClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(OPCFClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        OPCFClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(OPCFClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        OPCFClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(OPCFClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        OPCFClass->setStatusBar(statusBar);

        retranslateUi(OPCFClass);

        QMetaObject::connectSlotsByName(OPCFClass);
    } // setupUi

    void retranslateUi(QMainWindow *OPCFClass)
    {
        OPCFClass->setWindowTitle(QCoreApplication::translate("OPCFClass", "OPCF", nullptr));
    } // retranslateUi

};

namespace Ui {
    class OPCFClass: public Ui_OPCFClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
