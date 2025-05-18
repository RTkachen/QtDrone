/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QWidget *widget;
    QFrame *frame;
    QPushButton *pushButtonCharts;
    QLCDNumber *lcdNumberX;
    QLCDNumber *lcdNumberY;
    QLCDNumber *lcdNumberZ;
    QPushButton *pushButtonLanguage;
    QPushButton *pushButtonConnect;
    QPushButton *pushButtonStart;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1130, 682);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName("gridLayout");
        widget = new QWidget(centralwidget);
        widget->setObjectName("widget");
        frame = new QFrame(widget);
        frame->setObjectName("frame");
        frame->setGeometry(QRect(0, 10, 1111, 61));
        frame->setFrameShape(QFrame::Shape::StyledPanel);
        frame->setFrameShadow(QFrame::Shadow::Raised);
        pushButtonCharts = new QPushButton(frame);
        pushButtonCharts->setObjectName("pushButtonCharts");
        pushButtonCharts->setGeometry(QRect(10, 10, 151, 41));
        lcdNumberX = new QLCDNumber(frame);
        lcdNumberX->setObjectName("lcdNumberX");
        lcdNumberX->setGeometry(QRect(170, 10, 81, 41));
        lcdNumberY = new QLCDNumber(frame);
        lcdNumberY->setObjectName("lcdNumberY");
        lcdNumberY->setGeometry(QRect(260, 10, 91, 41));
        lcdNumberZ = new QLCDNumber(frame);
        lcdNumberZ->setObjectName("lcdNumberZ");
        lcdNumberZ->setGeometry(QRect(360, 10, 81, 41));
        pushButtonLanguage = new QPushButton(frame);
        pushButtonLanguage->setObjectName("pushButtonLanguage");
        pushButtonLanguage->setGeometry(QRect(990, 10, 111, 41));
        pushButtonConnect = new QPushButton(frame);
        pushButtonConnect->setObjectName("pushButtonConnect");
        pushButtonConnect->setGeometry(QRect(860, 10, 121, 41));
        pushButtonStart = new QPushButton(frame);
        pushButtonStart->setObjectName("pushButtonStart");
        pushButtonStart->setGeometry(QRect(720, 10, 131, 41));

        gridLayout->addWidget(widget, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        pushButtonCharts->setText(QCoreApplication::translate("MainWindow", "Charts", nullptr));
        pushButtonLanguage->setText(QCoreApplication::translate("MainWindow", "PushButton", nullptr));
        pushButtonConnect->setText(QCoreApplication::translate("MainWindow", "Connect", nullptr));
        pushButtonStart->setText(QCoreApplication::translate("MainWindow", "Start", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
