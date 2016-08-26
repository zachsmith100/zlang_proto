/********************************************************************************
** Form generated from reading UI file 'emptywindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EMPTYWINDOW_H
#define UI_EMPTYWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_EmptyWindow
{
public:

    void setupUi(QDialog *EmptyWindow)
    {
        if (EmptyWindow->objectName().isEmpty())
            EmptyWindow->setObjectName(QStringLiteral("EmptyWindow"));
        EmptyWindow->resize(400, 300);

        retranslateUi(EmptyWindow);

        QMetaObject::connectSlotsByName(EmptyWindow);
    } // setupUi

    void retranslateUi(QDialog *EmptyWindow)
    {
        EmptyWindow->setWindowTitle(QString());
    } // retranslateUi

};

namespace Ui {
    class EmptyWindow: public Ui_EmptyWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EMPTYWINDOW_H
