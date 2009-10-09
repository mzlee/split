/********************************************************************************
** Form generated from reading ui file 'splitwindow.ui'
**
** Created: Fri Oct 9 16:31:21 2009
**      by: Qt User Interface Compiler version 4.5.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_SPLITWINDOW_H
#define UI_SPLITWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>
#include <QtWebKit/QWebView>

QT_BEGIN_NAMESPACE

class Ui_SplitWindow
{
public:
    QWidget *centralWidget;
    QLineEdit *txtUrl;
    QTabWidget *tabPage;
    QWidget *tab;
    QWebView *webView;
    QWidget *tab_2;
    QWebView *webView_2;
    QPushButton *btnGo;
    QMenuBar *menuBar;
    QMenu *menuSplit;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *SplitWindow)
    {
        if (SplitWindow->objectName().isEmpty())
            SplitWindow->setObjectName(QString::fromUtf8("SplitWindow"));
        SplitWindow->resize(413, 325);
        centralWidget = new QWidget(SplitWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        txtUrl = new QLineEdit(centralWidget);
        txtUrl->setObjectName(QString::fromUtf8("txtUrl"));
        txtUrl->setGeometry(QRect(10, 10, 361, 22));
        tabPage = new QTabWidget(centralWidget);
        tabPage->setObjectName(QString::fromUtf8("tabPage"));
        tabPage->setGeometry(QRect(10, 40, 391, 241));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        webView = new QWebView(tab);
        webView->setObjectName(QString::fromUtf8("webView"));
        webView->setGeometry(QRect(0, 0, 391, 201));
        webView->setUrl(QUrl("http://www.google.com/"));
        tabPage->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        webView_2 = new QWebView(tab_2);
        webView_2->setObjectName(QString::fromUtf8("webView_2"));
        webView_2->setGeometry(QRect(0, 0, 391, 201));
        webView_2->setUrl(QUrl("about:blank"));
        tabPage->addTab(tab_2, QString());
        btnGo = new QPushButton(centralWidget);
        btnGo->setObjectName(QString::fromUtf8("btnGo"));
        btnGo->setGeometry(QRect(380, 10, 21, 21));
        SplitWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(SplitWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 413, 22));
        menuSplit = new QMenu(menuBar);
        menuSplit->setObjectName(QString::fromUtf8("menuSplit"));
        SplitWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(SplitWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        SplitWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(SplitWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        SplitWindow->setStatusBar(statusBar);

        menuBar->addAction(menuSplit->menuAction());

        retranslateUi(SplitWindow);

        tabPage->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(SplitWindow);
    } // setupUi

    void retranslateUi(QMainWindow *SplitWindow)
    {
        SplitWindow->setWindowTitle(QApplication::translate("SplitWindow", "SplitWindow", 0, QApplication::UnicodeUTF8));
        tabPage->setTabText(tabPage->indexOf(tab), QApplication::translate("SplitWindow", "Tab 1", 0, QApplication::UnicodeUTF8));
        tabPage->setTabText(tabPage->indexOf(tab_2), QApplication::translate("SplitWindow", "Tab 2", 0, QApplication::UnicodeUTF8));
        btnGo->setText(QApplication::translate("SplitWindow", "Go", 0, QApplication::UnicodeUTF8));
        menuSplit->setTitle(QApplication::translate("SplitWindow", "File", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SplitWindow: public Ui_SplitWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SPLITWINDOW_H
