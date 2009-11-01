/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created: Fri Oct 30 22:51:24 2009
**      by: Qt User Interface Compiler version 4.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLineEdit *addressBar;
    QPushButton *goButton;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *forwardButton;
    QPushButton *backButton;
    QPushButton *newWindowButton;
    QPushButton *clippingModeButton;
    QPushButton *restoreClipButton;
    QLabel *debugText;

    void setupUi(QDialog *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(568, 127);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(MainWindow);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        addressBar = new QLineEdit(MainWindow);
        addressBar->setObjectName(QString::fromUtf8("addressBar"));

        horizontalLayout->addWidget(addressBar);

        goButton = new QPushButton(MainWindow);
        goButton->setObjectName(QString::fromUtf8("goButton"));

        horizontalLayout->addWidget(goButton);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        forwardButton = new QPushButton(MainWindow);
        forwardButton->setObjectName(QString::fromUtf8("forwardButton"));

        horizontalLayout_2->addWidget(forwardButton);

        backButton = new QPushButton(MainWindow);
        backButton->setObjectName(QString::fromUtf8("backButton"));

        horizontalLayout_2->addWidget(backButton);

        newWindowButton = new QPushButton(MainWindow);
        newWindowButton->setObjectName(QString::fromUtf8("newWindowButton"));

        horizontalLayout_2->addWidget(newWindowButton);

        clippingModeButton = new QPushButton(MainWindow);
        clippingModeButton->setObjectName(QString::fromUtf8("clippingModeButton"));

        horizontalLayout_2->addWidget(clippingModeButton);

        restoreClipButton = new QPushButton(MainWindow);
        restoreClipButton->setObjectName(QString::fromUtf8("restoreClipButton"));

        horizontalLayout_2->addWidget(restoreClipButton);


        verticalLayout->addLayout(horizontalLayout_2);

        debugText = new QLabel(MainWindow);
        debugText->setObjectName(QString::fromUtf8("debugText"));

        verticalLayout->addWidget(debugText);


        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QDialog *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Split - Main Window", 0, QApplication::UnicodeUTF8));
        addressBar->setText(QApplication::translate("MainWindow", "http://", 0, QApplication::UnicodeUTF8));
        goButton->setText(QApplication::translate("MainWindow", "Go", 0, QApplication::UnicodeUTF8));
        forwardButton->setText(QApplication::translate("MainWindow", "Forward", 0, QApplication::UnicodeUTF8));
        backButton->setText(QApplication::translate("MainWindow", "Back", 0, QApplication::UnicodeUTF8));
        newWindowButton->setText(QApplication::translate("MainWindow", "New Window", 0, QApplication::UnicodeUTF8));
        clippingModeButton->setText(QApplication::translate("MainWindow", "Clipping Mode", 0, QApplication::UnicodeUTF8));
        restoreClipButton->setText(QApplication::translate("MainWindow", "Restore Clip", 0, QApplication::UnicodeUTF8));
        debugText->setText(QApplication::translate("MainWindow", "TextLabel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
