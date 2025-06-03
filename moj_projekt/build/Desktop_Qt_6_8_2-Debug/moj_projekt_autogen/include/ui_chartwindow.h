/********************************************************************************
** Form generated from reading UI file 'chartwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHARTWINDOW_H
#define UI_CHARTWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ChartWindow
{
public:

    void setupUi(QWidget *ChartWindow)
    {
        if (ChartWindow->objectName().isEmpty())
            ChartWindow->setObjectName("ChartWindow");
        ChartWindow->resize(1183, 744);

        retranslateUi(ChartWindow);

        QMetaObject::connectSlotsByName(ChartWindow);
    } // setupUi

    void retranslateUi(QWidget *ChartWindow)
    {
        ChartWindow->setWindowTitle(QCoreApplication::translate("ChartWindow", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ChartWindow: public Ui_ChartWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHARTWINDOW_H
