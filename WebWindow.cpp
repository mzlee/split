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

WebWindow::WebWindow( QWidget *parent, QNetworkCookieJar *jar, QString defaultUrl ) : QMainWindow( parent ){

    wwparent = parent;

    //show the window
    ui.setupUi( this );
    cookieJar = jar;
    ui.WebView->page()->networkAccessManager()->setCookieJar(jar);

    ui.ClickArea->lower();
    ui.ClickArea->setHidden(true);

    setupConnections();
	setupShortcuts();

    //setup state machine
    setupState();

    clipped = false;
    geometrySet = false;
    navigate(defaultUrl);
}

void WebWindow::setupShortcuts(){
	/* 
		new window				ctrl + n
		go						ctrl + g
		press clip button		ctrl + m
		select address bar		ctrl + l
	*/

	//new window
	QShortcut* qsc = new QShortcut(QKeySequence("Ctrl+n"),this);
	connect( qsc, SIGNAL(activated()), this, SLOT(newWindow()) );

	//go
	qsc = new QShortcut(QKeySequence("Ctrl+g"),this);
	connect( qsc, SIGNAL(activated()), this, SLOT(go()) );
	
	//clipping mode && restore
	qsc = new QShortcut(QKeySequence("Ctrl+m"),this);
	connect( qsc, SIGNAL(activated()), ui.clippingModeButton, SLOT(click()) );
	
	//select address bar
	qsc = new QShortcut(QKeySequence("Ctrl+l"),this);
	connect( qsc, SIGNAL(activated()), this, SLOT(selectAddrBar()) );

	
}

void WebWindow::selectAddrBar(){
	//get the keyboard focus and select all the text
	ui.addressBar->setFocus(Qt::OtherFocusReason);
	ui.addressBar->selectAll();
}

void WebWindow::setupConnections(){
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
}

void WebWindow::setupState(){
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
}

void WebWindow::updateStatus(const QString &message) {
    ui.statusBar->showMessage(message);
}

//if the window starts stretching infinitely... look here
void WebWindow::resizeEvent(QResizeEvent * e) {
    if (!clipped) {
        ui.WebView->setFixedSize(e->size().width(), e->size().height() - 54);
        ui.ClickArea->setGeometry(ui.WebView->x(),ui.WebView->y(),ui.WebView->width(),ui.WebView->height());
        ui.NavigationBox->setGeometry(0, 0, e->size().width(), 34);
        resizeAddressBox();
        geometrySet = false;
    }
}

void WebWindow::resizeAddressBox() {
    ui.clippingModeButton->setGeometry(ui.NavigationBox->size().width() - 33, 0, 32, 32);
    ui.stopButton->setGeometry(ui.NavigationBox->size().width() - 64, 0, 32, 32);
    ui.reloadButton->setGeometry(ui.NavigationBox->size().width() - 95, 0 , 32, 32);
    ui.addressBar->setGeometry(68, 3, ui.NavigationBox->size().width() - 168, 28);
}

void WebWindow::createMask(QRegion region) {
   setMask(region);
   ui.WebView->blockSignals(true);
}

void WebWindow::storeMask(QRegion region) {
    storedMasks.push_back(region);
}


void WebWindow::removeMask() {
   clearMask();
   ui.WebView->blockSignals(false);
}

void WebWindow::go() {
    navigate(ui.addressBar->text());
}

void WebWindow::newWindow(const QUrl & url) {
    newWindow(url.toString());
}

void WebWindow::newWindow(const QString & url) {
    WebWindow *w = new WebWindow(wwparent, cookieJar, url);
    w->show();
    w->setFocus();
}

void WebWindow::startClippingMode() {
    //enter clipping mode
    if (geometrySet) {
        restoreClip();
    }
    ui.ClickArea->update();
    ui.ClickArea->setEnabled(true);
    ui.ClickArea->raise();
    ui.ClickArea->show();

    storedMasks.clear();
    sharedClips.clear();
}


void WebWindow::exitClippingMode() {
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
}

void WebWindow::setClip(QRegion region) {
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
}

// Button Functionality
void WebWindow::forward() {
    //go forward in currentWindow
    if (ui.WebView->history()->canGoForward()) {
        ui.WebView->history()->forward();
    }
}

void WebWindow::back() {
    //go backwards in currentWindow
    if (ui.WebView->history()->canGoBack()) {
        ui.WebView->history()->back();
    }
}

void WebWindow::navigate( QUrl url ) {
    //navigate to url
    //code injections possible?
    //if (!url.startsWith("http", Qt::CaseInsensitive)) {
    //    url = "http://" + url;
    ui.addressBar->setText(url.toString());
    //}
    ui.WebView->setUrl(url);
}

void WebWindow::navigate( QString url ) {
    if (!url.contains("://", Qt::CaseInsensitive)) {
        url = "http://" + url;
    }
    navigate(QUrl(url));
}

void WebWindow::restoreClip() {
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
}

void WebWindow::destroySharedClips() {
    for(int i=0; i<sharedClips.size(); i++) {
        for(int j=0; j<sharedClips[i]->sharedClips.size(); j++) {
            if(sharedClips[i]->sharedClips[j] == this) {
                sharedClips[i]->sharedClips.erase(sharedClips[i]->sharedClips.begin() + j);
                break;
            }
        }
    }
}

void WebWindow::setWebKitState() {
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
}
