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
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "D3widget.hpp"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QSplitter *splitter;
    QFrame *frame;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *pushButtonCharts;
    QDoubleSpinBox *NumberX;
    QDoubleSpinBox *NumberY;
    QDoubleSpinBox *NumberZ;
    QPushButton *pushButtonStart;
    QPushButton *pushButtonConnect;
    QComboBox *comboBoxDevices;
    QPushButton *pushButtonSearch;
    QPushButton *pushButtonLanguage;
    QFrame *frame_2;
    QVBoxLayout *verticalLayout_2;
    D3widget *openGLWidget;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1145, 682);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        splitter = new QSplitter(centralwidget);
        splitter->setObjectName("splitter");
        splitter->setOrientation(Qt::Orientation::Vertical);
        frame = new QFrame(splitter);
        frame->setObjectName("frame");
        frame->setFrameShape(QFrame::Shape::StyledPanel);
        frame->setFrameShadow(QFrame::Shadow::Raised);
        horizontalLayout_3 = new QHBoxLayout(frame);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        pushButtonCharts = new QPushButton(frame);
        pushButtonCharts->setObjectName("pushButtonCharts");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pushButtonCharts->sizePolicy().hasHeightForWidth());
        pushButtonCharts->setSizePolicy(sizePolicy);
        QFont font;
        font.setPointSize(12);
        pushButtonCharts->setFont(font);

        horizontalLayout_3->addWidget(pushButtonCharts);

        NumberX = new QDoubleSpinBox(frame);
        NumberX->setObjectName("NumberX");
        NumberX->setMinimumSize(QSize(0, 30));
        NumberX->setMaximumSize(QSize(100, 30));
        NumberX->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        NumberX->setStyleSheet(QString::fromUtf8("font-size: 18px;"));
        NumberX->setWrapping(false);
        NumberX->setAlignment(Qt::AlignmentFlag::AlignCenter);
        NumberX->setReadOnly(true);
        NumberX->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);
        NumberX->setMinimum(-2.500000000000000);
        NumberX->setMaximum(2.500000000000000);

        horizontalLayout_3->addWidget(NumberX);

        NumberY = new QDoubleSpinBox(frame);
        NumberY->setObjectName("NumberY");
        NumberY->setMinimumSize(QSize(0, 30));
        NumberY->setMaximumSize(QSize(100, 30));
        NumberY->setStyleSheet(QString::fromUtf8("font-size: 18px;"));
        NumberY->setAlignment(Qt::AlignmentFlag::AlignCenter);
        NumberY->setReadOnly(true);
        NumberY->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);
        NumberY->setMinimum(-2.500000000000000);
        NumberY->setMaximum(2.500000000000000);

        horizontalLayout_3->addWidget(NumberY);

        NumberZ = new QDoubleSpinBox(frame);
        NumberZ->setObjectName("NumberZ");
        NumberZ->setMinimumSize(QSize(0, 30));
        NumberZ->setMaximumSize(QSize(100, 30));
        NumberZ->setStyleSheet(QString::fromUtf8("font-size: 18px;"));
        NumberZ->setAlignment(Qt::AlignmentFlag::AlignCenter);
        NumberZ->setReadOnly(true);
        NumberZ->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);
        NumberZ->setMinimum(-2.500000000000000);
        NumberZ->setMaximum(2.500000000000000);

        horizontalLayout_3->addWidget(NumberZ);

        pushButtonStart = new QPushButton(frame);
        pushButtonStart->setObjectName("pushButtonStart");
        sizePolicy.setHeightForWidth(pushButtonStart->sizePolicy().hasHeightForWidth());
        pushButtonStart->setSizePolicy(sizePolicy);
        pushButtonStart->setFont(font);

        horizontalLayout_3->addWidget(pushButtonStart);

        pushButtonConnect = new QPushButton(frame);
        pushButtonConnect->setObjectName("pushButtonConnect");
        sizePolicy.setHeightForWidth(pushButtonConnect->sizePolicy().hasHeightForWidth());
        pushButtonConnect->setSizePolicy(sizePolicy);
        pushButtonConnect->setFont(font);

        horizontalLayout_3->addWidget(pushButtonConnect);

        comboBoxDevices = new QComboBox(frame);
        comboBoxDevices->setObjectName("comboBoxDevices");
        sizePolicy.setHeightForWidth(comboBoxDevices->sizePolicy().hasHeightForWidth());
        comboBoxDevices->setSizePolicy(sizePolicy);
        comboBoxDevices->setFont(font);
        comboBoxDevices->setContextMenuPolicy(Qt::ContextMenuPolicy::PreventContextMenu);

        horizontalLayout_3->addWidget(comboBoxDevices);

        pushButtonSearch = new QPushButton(frame);
        pushButtonSearch->setObjectName("pushButtonSearch");
        sizePolicy.setHeightForWidth(pushButtonSearch->sizePolicy().hasHeightForWidth());
        pushButtonSearch->setSizePolicy(sizePolicy);
        pushButtonSearch->setFont(font);

        horizontalLayout_3->addWidget(pushButtonSearch);

        pushButtonLanguage = new QPushButton(frame);
        pushButtonLanguage->setObjectName("pushButtonLanguage");
        sizePolicy.setHeightForWidth(pushButtonLanguage->sizePolicy().hasHeightForWidth());
        pushButtonLanguage->setSizePolicy(sizePolicy);
        pushButtonLanguage->setFont(font);

        horizontalLayout_3->addWidget(pushButtonLanguage);

        splitter->addWidget(frame);
        frame_2 = new QFrame(splitter);
        frame_2->setObjectName("frame_2");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy1.setHorizontalStretch(1);
        sizePolicy1.setVerticalStretch(1);
        sizePolicy1.setHeightForWidth(frame_2->sizePolicy().hasHeightForWidth());
        frame_2->setSizePolicy(sizePolicy1);
        frame_2->setMinimumSize(QSize(0, 0));
        frame_2->setFrameShape(QFrame::Shape::StyledPanel);
        frame_2->setFrameShadow(QFrame::Shadow::Raised);
        verticalLayout_2 = new QVBoxLayout(frame_2);
        verticalLayout_2->setObjectName("verticalLayout_2");
        openGLWidget = new D3widget(frame_2);
        openGLWidget->setObjectName("openGLWidget");

        verticalLayout_2->addWidget(openGLWidget);

        splitter->addWidget(frame_2);

        verticalLayout->addWidget(splitter);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        pushButtonCharts->setText(QCoreApplication::translate("MainWindow", "Charts", nullptr));
        NumberX->setPrefix(QCoreApplication::translate("MainWindow", "X: ", nullptr));
        NumberX->setSuffix(QCoreApplication::translate("MainWindow", " g", nullptr));
        NumberY->setPrefix(QCoreApplication::translate("MainWindow", "Y: ", nullptr));
        NumberY->setSuffix(QCoreApplication::translate("MainWindow", " g", nullptr));
        NumberZ->setPrefix(QCoreApplication::translate("MainWindow", "Z: ", nullptr));
        NumberZ->setSuffix(QCoreApplication::translate("MainWindow", " g", nullptr));
        pushButtonStart->setText(QCoreApplication::translate("MainWindow", "Start", nullptr));
        pushButtonConnect->setText(QCoreApplication::translate("MainWindow", "Connect", nullptr));
        pushButtonSearch->setText(QCoreApplication::translate("MainWindow", "Search", nullptr));
        pushButtonLanguage->setText(QCoreApplication::translate("MainWindow", "Language", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
