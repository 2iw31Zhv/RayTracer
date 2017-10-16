/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindowClass
{
public:
    QAction *actionBy_Rotation;
    QAction *actionNew_Scene;
    QAction *actionRay_tracing;
    QAction *actionImport_model;
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuModel;
    QMenu *menuRender;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindowClass)
    {
        if (MainWindowClass->objectName().isEmpty())
            MainWindowClass->setObjectName(QStringLiteral("MainWindowClass"));
        MainWindowClass->resize(1200, 800);
        actionBy_Rotation = new QAction(MainWindowClass);
        actionBy_Rotation->setObjectName(QStringLiteral("actionBy_Rotation"));
        actionNew_Scene = new QAction(MainWindowClass);
        actionNew_Scene->setObjectName(QStringLiteral("actionNew_Scene"));
        actionRay_tracing = new QAction(MainWindowClass);
        actionRay_tracing->setObjectName(QStringLiteral("actionRay_tracing"));
        actionImport_model = new QAction(MainWindowClass);
        actionImport_model->setObjectName(QStringLiteral("actionImport_model"));
        centralWidget = new QWidget(MainWindowClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        MainWindowClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindowClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1200, 30));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuModel = new QMenu(menuBar);
        menuModel->setObjectName(QStringLiteral("menuModel"));
        menuRender = new QMenu(menuBar);
        menuRender->setObjectName(QStringLiteral("menuRender"));
        MainWindowClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindowClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindowClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindowClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindowClass->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuModel->menuAction());
        menuBar->addAction(menuRender->menuAction());
        menuFile->addAction(actionImport_model);
        menuModel->addAction(actionBy_Rotation);
        menuRender->addAction(actionNew_Scene);
        menuRender->addAction(actionRay_tracing);

        retranslateUi(MainWindowClass);

        QMetaObject::connectSlotsByName(MainWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindowClass)
    {
        MainWindowClass->setWindowTitle(QApplication::translate("MainWindowClass", "MainWindow", 0));
        actionBy_Rotation->setText(QApplication::translate("MainWindowClass", "RotationModel", 0));
        actionNew_Scene->setText(QApplication::translate("MainWindowClass", "New Scene", 0));
        actionRay_tracing->setText(QApplication::translate("MainWindowClass", "Ray Tracing", 0));
        actionImport_model->setText(QApplication::translate("MainWindowClass", "Import Model", 0));
        menuFile->setTitle(QApplication::translate("MainWindowClass", "File", 0));
        menuModel->setTitle(QApplication::translate("MainWindowClass", "Model", 0));
        menuRender->setTitle(QApplication::translate("MainWindowClass", "Render", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindowClass: public Ui_MainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
