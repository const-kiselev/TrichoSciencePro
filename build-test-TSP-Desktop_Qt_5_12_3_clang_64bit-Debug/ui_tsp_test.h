/********************************************************************************
** Form generated from reading UI file 'tsp_test.ui'
**
** Created by: Qt User Interface Compiler version 5.12.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TSP_TEST_H
#define UI_TSP_TEST_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TSP_test
{
public:
    QWidget *centralwidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *TSP_test)
    {
        if (TSP_test->objectName().isEmpty())
            TSP_test->setObjectName(QString::fromUtf8("TSP_test"));
        TSP_test->resize(800, 600);
        centralwidget = new QWidget(TSP_test);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        TSP_test->setCentralWidget(centralwidget);
        menubar = new QMenuBar(TSP_test);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        TSP_test->setMenuBar(menubar);
        statusbar = new QStatusBar(TSP_test);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        TSP_test->setStatusBar(statusbar);

        retranslateUi(TSP_test);

        QMetaObject::connectSlotsByName(TSP_test);
    } // setupUi

    void retranslateUi(QMainWindow *TSP_test)
    {
        TSP_test->setWindowTitle(QApplication::translate("TSP_test", "TSP_test", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TSP_test: public Ui_TSP_test {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TSP_TEST_H
