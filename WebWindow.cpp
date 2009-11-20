//////////////////////////////////////////////////
//                                              //
// WebWindow.cpp                                //
//   -> part of Split                           //
//                                              //
// Copyright (c) 2009 Jim Brusstar, Michael Lee //
//                 Ben Montgomery, Robert Steen //
//                                              //
//////////////////////////////////////////////////

#include "WebWindow.h"
#include <QEvent>
#include <QGraphicsWidget>
#include <QUrl>
#include <QWebView>
#include <QWebPage>
#include <QWebFrame>
#include <QWebHistory>
#include <QIcon>
#include <QKeySequence>
#include <QShortcut>
#include <QStateMachine>

WebWindow::WebWindow(QNetworkCookieJar *argCookieJar, QString defaultUrl)
{
    // Set the cookie jar
    _cookieJar = argCookieJar;
    _ui.WebView->page()->networkAccessManager()->setCookieJar(_cookieJar);

    // Setup the window
    _ui.setupUi(this);              // Build form UI
    _ui.ClickArea->setHidden(true); // Hide the clipping field
    _setupConnections();            // Connect signals/slots
    _setupShortcuts();              // Init keyboard shortcuts
    _setupState();                  // Init the state machine
    _clipped = false;
    _geometrySet = false;
    _navigate(defaultUrl);
}

void WebWindow::_setupShortcuts()
{
    // Init keyboard shortcuts
    //      new window			ctrl + n
    //      go					ctrl + g
    //      press clip button	ctrl + m
    //      select address bar	ctrl + l
    QShortcut* qsc = new QShortcut(QKeySequence("Ctrl+n"),this);    // new window
	connect( qsc, SIGNAL(activated()), this, SLOT(newWindow()) );
    qsc = new QShortcut(QKeySequence("Ctrl+g"),this);               // go
	connect( qsc, SIGNAL(activated()), this, SLOT(go()) );
    qsc = new QShortcut(QKeySequence("Ctrl+m"),this);               // clip/no-clip
	connect( qsc, SIGNAL(activated()), ui.clippingModeButton, SLOT(click()) );
    qsc = new QShortcut(QKeySequence("Ctrl+l"),this);               // set url bar
	connect( qsc, SIGNAL(activated()), this, SLOT(selectAddrBar()) );
    return;
}

void WebWindow::_selectAddrBar()
{
	//get the keyboard focus and select all the text
	ui.addressBar->setFocus(Qt::OtherFocusReason);
	ui.addressBar->selectAll();
    return;
}

void WebWindow::_setupConnections()
{
    //connect the click() action of each button to a function
    connect( ui.backButton, SIGNAL(clicked()), this, SLOT(back()) );
    connect( ui.forwardButton, SIGNAL(clicked()), this, SLOT(forward()) );
    connect( ui.reloadButton, SIGNAL(clicked()), this, SLOT(go()) );
    connect( ui.stopButton, SIGNAL(clicked()), ui.WebView, SLOT(stop()) );

	connect( ui.addressBar, SIGNAL(returnPressed()), this, SLOT(go()) );
    connect( ui.WebView, SIGNAL(loadFinished(bool)), this, SLOT(setWebKitState()) );
    connect( ui.WebView, SIGNAL(linkClicked(QUrl)), this, SLOT(newWindow(const QUrl &)) );
    connect( ui.WebView, SIGNAL(statusBarMessage(const QString &)), this, SLOT(updateStatus(const QString &)));

    connect( ui.actionNewWindow, SIGNAL(triggered()), this, SLOT(newWindow()) );
    connect( ui.actionRestoreWindow, SIGNAL(triggered()), this, SLOT(restoreClip()) );
    connect( ui.actionReload, SIGNAL(triggered()), this, SLOT(go()) );
    connect( ui.actionBack, SIGNAL(triggered()), this, SLOT(back()) );
    connect( ui.actionForward, SIGNAL(triggered()), this, SLOT(forward()) );
    connect( ui.actionBack, SIGNAL(triggered()), ui.WebView, SLOT(stop()) );

    connect( ui.ClickArea, SIGNAL(mcEndCapture(QRegion)), this, SLOT(storeMask(QRegion)) );

    return;
}

void WebWindow::_setupState()
{
    // Sets up the state machine
    QState *normalMode = new QState();
    QState *clippingMode = new QState();
    normalMode->addTransition(ui.clippingModeButton, SIGNAL(clicked()), clippingMode);
    clippingMode->addTransition(ui.clippingModeButton, SIGNAL(clicked()), normalMode);
    normalMode->assignProperty(ui.clippingModeButton, "checked", false);
    normalMode->assignProperty(ui.clippingModeButton, "icon", ":icons/clip");
    clippingMode->assignProperty(ui.clippingModeButton, "checked", true);
    clippingMode->assignProperty(ui.clippingModeButton, "icon", ":icons/clip");
    machine.addState(normalMode);
    machine.addState(clippingMode);
    machine.setInitialState(normalMode);
    connect( clippingMode, SIGNAL(entered()), this, SLOT(startClippingMode()) );
    connect( clippingMode, SIGNAL(exited()), this, SLOT(exitClippingMode()) );
    machine.start();
    return;
}

void WebWindow::_updateStatus(const QString &message)
{
    ui.statusBar->showMessage(message);
    return;
}

void WebWindow::resizeEvent(QResizeEvent *e)
{
    //if the window starts stretching infinitely... look here
    if (!clipped) {
        ui.WebView->setFixedSize(e->size().width(), e->size().height() - 54);
        ui.ClickArea->setGeometry(ui.WebView->x(),ui.WebView->y(),ui.WebView->width(),ui.WebView->height());
        ui.NavigationBox->setGeometry(0, 0, e->size().width(), 34);
        resizeAddressBox();
        geometrySet = false;
    }
    return;
}

void WebWindow::_resizeAddressBox()
{
    ui.clippingModeButton->setGeometry(ui.NavigationBox->size().width() - 33, 0, 32, 32);
    ui.stopButton->setGeometry(ui.NavigationBox->size().width() - 64, 0, 32, 32);
    ui.reloadButton->setGeometry(ui.NavigationBox->size().width() - 95, 0 , 32, 32);
    ui.addressBar->setGeometry(68, 3, ui.NavigationBox->size().width() - 168, 28);
    return;
}

void WebWindow::_createMask(QRegion region)
{
   setMask(region);
   ui.WebView->blockSignals(true);
   return;
}

void WebWindow::_storeMask(QRegion region)
{
    storedMasks.push_back(region);
    return;
}

void WebWindow::_removeMask()
{
   clearMask();
   ui.WebView->blockSignals(false);
   return;
}

void WebWindow::_go()
{
    navigate(ui.addressBar->text());
    return;
}

void WebWindow::_newWindow(const QUrl &url)
{
    newWindow(url.toString());
    return;
}

void WebWindow::_newWindow(const QString &url)
{
    WebWindow *w = new WebWindow(_cookieJar, url);
    w->show();
    w->setFocus();
    return;
}

void WebWindow::_startClippingMode()
{
    // Enter clipping mode
    if (_geometrySet)
    {
        _restoreClip();
    }
    _ui.ClickArea->update();
    _ui.ClickArea->setEnabled(true);
    _ui.ClickArea->raise();
    _ui.ClickArea->show();
    _storedMasks.clear();
    _sharedClips.clear();
    return;
}


void WebWindow::_exitClippingMode()
{
    //Combine masks that intersect
    for (int i=0; i<storedMasks.size(); i++) {
        for(int j=0; j<storedMasks.size(); j++) {
            if(i == j) {
                //Do nothing
            }
            else if(!storedMasks[i].intersect(storedMasks[j]).isEmpty()) {
                storedMasks[i] = storedMasks[i].united(storedMasks[j]);
                storedMasks.erase(storedMasks.begin() + j);
            }
        }
    }

    //Create each mask
    for(int i=0; i<storedMasks.size(); i++) {
        if(i==0) {
            setClip(storedMasks[0]);
        }
        else {
            WebWindow *w = new WebWindow(wwparent, cookieJar, ui.WebView->url().toString());
            w->show();
            w->storeMask(storedMasks[i]);
            w->setClip(storedMasks[i]);

            //Keep track of clips from same page
            sharedClips.push_back(w);
        }
    }

    //Make sure every new clip has a pointer to all the
    // other clips that were created from the same page.
    for(int i=0; i<sharedClips.size(); i++) {
        sharedClips[i]->sharedClips.push_back(this);
        for(int j=0; j<sharedClips.size(); j++) {
            if(i != j) {
                sharedClips[i]->sharedClips.push_back(sharedClips[j]);
            }
        }
    }
    return;
}

void WebWindow::_setClip(QRegion region)
{
    clipped = true;
    ui.NavigationBox->setHidden(true);

    windowGeometry = ui.WebView->geometry();
    mainGeometry = this->geometry();
    geometrySet = true;

    ui.WebView->setGeometry(-region.boundingRect().left(), -region.boundingRect().top(), ui.WebView->width(), ui.WebView->height());
    this->setGeometry(mainGeometry.left(), mainGeometry.top(), region.boundingRect().width(), region.boundingRect().height() + 40);
    QRegion newRegion;
    //region.boundingRect().setTopLeft(QPoint(0, 0));
    QPoint p = region.boundingRect().topLeft();
    for(int i=0; i<region.rects().size(); i++) {
        QRect rect = region.rects()[i];
        rect.moveTopLeft(rect.topLeft() - p);
        rect.setHeight(rect.height() + 20);
        newRegion += QRegion(rect, QRegion::Rectangle);
    }
    setMask(newRegion);

    ui.ClickArea->setDisabled(true);
    ui.ClickArea->lower();
    ui.ClickArea->update();

    ui.WebView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    return;
}

void WebWindow::_forward()
{
    // Button Functionality
    if (ui.WebView->history()->canGoForward())
    {
        //go forward in currentWindow
        ui.WebView->history()->forward();
    }
    return;
}

void WebWindow::_back()
{
    //go backwards in currentWindow
    if (ui.WebView->history()->canGoBack())
    {
        ui.WebView->history()->back();
    }
    return;
}

void WebWindow::_navigate(QUrl url)
{
    //navigate to url
    //code injections possible?
    //if (!url.startsWith("http", Qt::CaseInsensitive)) {
    //    url = "http://" + url;
    ui.addressBar->setText(url.toString());
    //}
    ui.WebView->setUrl(url);
    return;
}

void WebWindow::_navigate(QString url)
{
    if (!url.contains("://", Qt::CaseInsensitive)) {
        url = "http://" + url;
    }
    navigate(QUrl(url));
    return;
}

void WebWindow::_restoreClip()
{
    removeMask();
    ui.WebView->setGeometry(windowGeometry);
    this->setGeometry(mainGeometry);
    ui.NavigationBox->setVisible(true);
    clipped = false;
    update();
    //Tell other clips that this one has been restored
    destroySharedClips();
    //Tell the page to start capturing link clicks again
    ui.WebView->page()->setLinkDelegationPolicy(QWebPage::DontDelegateLinks);
    return;
}

void WebWindow::_destroySharedClips()
{
    for(int i=0; i<sharedClips.size(); i++) {
        for(int j=0; j<sharedClips[i]->sharedClips.size(); j++) {
            if(sharedClips[i]->sharedClips[j] == this) {
                sharedClips[i]->sharedClips.erase(sharedClips[i]->sharedClips.begin() + j);
                break;
            }
        }
    }
    return;
}

void WebWindow::_setWebKitState()
{
    if (ui.WebView->history()->canGoBack()) {
        ui.backButton->setEnabled(true);
        ui.backButton->setIcon(QIcon(":/icons/back"));
    } else {
        ui.backButton->setDisabled(true);
        ui.backButton->setIcon(QIcon(":/icons/back.disabled"));
    }
    if (ui.WebView->history()->canGoForward()) {
        ui.forwardButton->setEnabled(true);
        ui.forwardButton->setIcon(QIcon(":/icons/forward"));
    } else {
        ui.forwardButton->setDisabled(true);
        ui.forwardButton->setIcon(QIcon(":/icons/forward.disabled"));
    }
    ui.addressBar->setText(ui.WebView->url().toString());
    this->setWindowTitle("Split -- " + ui.WebView->title());
    return;
}
