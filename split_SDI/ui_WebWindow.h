/********************************************************************************
** Form generated from reading UI file 'WebWindow.ui'
**
** Created: Sat Oct 31 19:36:50 2009
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
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
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
    QWebView *WebView;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QPushButton *backButton;
    QPushButton *forwardButton;
    QLineEdit *addressBar;
    QPushButton *goButton;
    QPushButton *clippingModeButton;
    QMenuBar *menubar;
    QMenu *menuFile;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *WebWindow)
    {
        if (WebWindow->objectName().isEmpty())
            WebWindow->setObjectName(QString::fromUtf8("WebWindow"));
        WebWindow->resize(681, 546);
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
        ClickArea->setGeometry(QRect(9, 39, 671, 465));
        sizePolicy.setHeightForWidth(ClickArea->sizePolicy().hasHeightForWidth());
        ClickArea->setSizePolicy(sizePolicy);
        WebView = new QWebView(centralwidget);
        WebView->setObjectName(QString::fromUtf8("WebView"));
        WebView->setGeometry(QRect(9, 43, 664, 461));
        sizePolicy.setHeightForWidth(WebView->sizePolicy().hasHeightForWidth());
        WebView->setSizePolicy(sizePolicy);
        WebView->setUrl(QUrl("about:blank"));
        widget = new QWidget(centralwidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(9, 9, 661, 26));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetNoConstraint);
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        backButton = new QPushButton(widget);
        backButton->setObjectName(QString::fromUtf8("backButton"));

        horizontalLayout->addWidget(backButton);

        forwardButton = new QPushButton(widget);
        forwardButton->setObjectName(QString::fromUtf8("forwardButton"));

        horizontalLayout->addWidget(forwardButton);

        addressBar = new QLineEdit(widget);
        addressBar->setObjectName(QString::fromUtf8("addressBar"));

        horizontalLayout->addWidget(addressBar);

        goButton = new QPushButton(widget);
        goButton->setObjectName(QString::fromUtf8("goButton"));

        horizontalLayout->addWidget(goButton);

        clippingModeButton = new QPushButton(widget);
        clippingModeButton->setObjectName(QString::fromUtf8("clippingModeButton"));

        horizontalLayout->addWidget(clippingModeButton);

        WebWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(WebWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 681, 19));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        WebWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(WebWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        WebWindow->setStatusBar(statusbar);

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
