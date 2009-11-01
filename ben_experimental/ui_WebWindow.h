/********************************************************************************
** Form generated from reading UI file 'WebWindow.ui'
**
** Created: Fri Oct 30 22:51:24 2009
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
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtWebKit/QWebView>
#include "MouseCaptureWidget.h"

QT_BEGIN_NAMESPACE

class Ui_WebWindow
{
public:
    QWebView *WebView;
    MouseCaptureWidget *ClickArea;

    void setupUi(QDialog *WebWindow)
    {
        if (WebWindow->objectName().isEmpty())
            WebWindow->setObjectName(QString::fromUtf8("WebWindow"));
        WebWindow->resize(400, 400);
        WebView = new QWebView(WebWindow);
        WebView->setObjectName(QString::fromUtf8("WebView"));
        WebView->setGeometry(QRect(0, 0, 400, 400));
        WebView->setUrl(QUrl("about:blank"));
        ClickArea = new MouseCaptureWidget(WebWindow);
        ClickArea->setObjectName(QString::fromUtf8("ClickArea"));
        ClickArea->setGeometry(QRect(0, 0, 400, 400));

        retranslateUi(WebWindow);

        QMetaObject::connectSlotsByName(WebWindow);
    } // setupUi

    void retranslateUi(QDialog *WebWindow)
    {
        WebWindow->setWindowTitle(QApplication::translate("WebWindow", "Web Window", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class WebWindow: public Ui_WebWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WEBWINDOW_H
