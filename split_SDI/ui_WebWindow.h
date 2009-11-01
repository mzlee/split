/********************************************************************************
** Form generated from reading UI file 'WebWindow.ui'
**
** Created: Sat Oct 31 21:54:08 2009
**      by: Qt User Interface Compiler version 4.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WEBWINDOW_H
#define UI_WEBWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>
#include <QtWebKit/QWebView>
#include "MouseCaptureWidget.h"

QT_BEGIN_NAMESPACE

class Ui_WebWindow
{
public:
    QAction *actionNewWindow;
    QWidget *centralwidget;
    MouseCaptureWidget *ClickArea;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *backButton;
    QPushButton *forwardButton;
    QLineEdit *addressBar;
    QPushButton *goButton;
    QPushButton *clippingModeButton;
    QWebView *WebView;
    QMenuBar *menubar;
    QMenu *menuFile;

    void setupUi(QMainWindow *WebWindow)
    {
        if (WebWindow->objectName().isEmpty())
            WebWindow->setObjectName(QString::fromUtf8("WebWindow"));
        WebWindow->resize(611, 364);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(WebWindow->sizePolicy().hasHeightForWidth());
        WebWindow->setSizePolicy(sizePolicy);
        actionNewWindow = new QAction(WebWindow);
        actionNewWindow->setObjectName(QString::fromUtf8("actionNewWindow"));
        centralwidget = new QWidget(WebWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        ClickArea = new MouseCaptureWidget(centralwidget);
        ClickArea->setObjectName(QString::fromUtf8("ClickArea"));
        ClickArea->setGeometry(QRect(200, 130, 251, 161));
        sizePolicy.setHeightForWidth(ClickArea->sizePolicy().hasHeightForWidth());
        ClickArea->setSizePolicy(sizePolicy);
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetNoConstraint);
        backButton = new QPushButton(centralwidget);
        backButton->setObjectName(QString::fromUtf8("backButton"));

        horizontalLayout->addWidget(backButton);

        forwardButton = new QPushButton(centralwidget);
        forwardButton->setObjectName(QString::fromUtf8("forwardButton"));

        horizontalLayout->addWidget(forwardButton);

        addressBar = new QLineEdit(centralwidget);
        addressBar->setObjectName(QString::fromUtf8("addressBar"));

        horizontalLayout->addWidget(addressBar);

        goButton = new QPushButton(centralwidget);
        goButton->setObjectName(QString::fromUtf8("goButton"));

        horizontalLayout->addWidget(goButton);

        clippingModeButton = new QPushButton(centralwidget);
        clippingModeButton->setObjectName(QString::fromUtf8("clippingModeButton"));

        horizontalLayout->addWidget(clippingModeButton);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        WebView = new QWebView(centralwidget);
        WebView->setObjectName(QString::fromUtf8("WebView"));
        sizePolicy.setHeightForWidth(WebView->sizePolicy().hasHeightForWidth());
        WebView->setSizePolicy(sizePolicy);
        WebView->setUrl(QUrl("about:blank"));

        gridLayout->addWidget(WebView, 1, 0, 1, 1);

        WebWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(WebWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 611, 19));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        WebWindow->setMenuBar(menubar);

        menubar->addAction(menuFile->menuAction());
        menuFile->addAction(actionNewWindow);

        retranslateUi(WebWindow);

        QMetaObject::connectSlotsByName(WebWindow);
    } // setupUi

    void retranslateUi(QMainWindow *WebWindow)
    {
        WebWindow->setWindowTitle(QApplication::translate("WebWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        actionNewWindow->setText(QApplication::translate("WebWindow", "New Window", 0, QApplication::UnicodeUTF8));
        backButton->setText(QApplication::translate("WebWindow", "Back", 0, QApplication::UnicodeUTF8));
        forwardButton->setText(QApplication::translate("WebWindow", "Forward", 0, QApplication::UnicodeUTF8));
        addressBar->setText(QApplication::translate("WebWindow", "http://", 0, QApplication::UnicodeUTF8));
        goButton->setText(QApplication::translate("WebWindow", "Go", 0, QApplication::UnicodeUTF8));
        clippingModeButton->setText(QApplication::translate("WebWindow", "Clipping Mode", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("WebWindow", "File", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class WebWindow: public Ui_WebWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WEBWINDOW_H
