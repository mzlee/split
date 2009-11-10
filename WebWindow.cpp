#include "WebWindow.h"
#include <QEvent>
#include <QGraphicsWidget>
#include <QUrl>
#include <QWebView>
#include <QWebHistory>
#include <QIcon>

WebWindow::WebWindow( QWidget *parent ) : QMainWindow( parent ){

    wwparent = parent;

    //show the window
    ui.setupUi( this );

    ui.ClickArea->lower();
    ui.ClickArea->setHidden(true);

    setupConnections();

    //setup state machine
    setupState();
    checkHistoryState();

    clipped = false;
    geometrySet = false;
}

void WebWindow::setupConnections(){
    //connect the click() action of each button to a function
    connect( ui.backButton, SIGNAL(clicked()), this, SLOT(back()) );
    connect( ui.forwardButton, SIGNAL(clicked()), this, SLOT(forward()) );
    connect( ui.reloadButton, SIGNAL(clicked()), this, SLOT(go()) );
    connect( ui.stopButton, SIGNAL(clicked()), ui.WebView, SLOT(stop()) );

    connect( ui.WebView, SIGNAL(statusBarMessage(const QString &)), this, SLOT(updateStatus(const QString &)));

    connect( ui.actionNewWindow, SIGNAL(triggered()), this, SLOT(newWindow()) );
    connect( ui.actionRestoreWindow, SIGNAL(triggered()), this, SLOT(restoreClip()) );
    connect( ui.actionReload, SIGNAL(triggered()), this, SLOT(go()) );
    connect( ui.actionBack, SIGNAL(triggered()), this, SLOT(back()) );
    connect( ui.actionForward, SIGNAL(triggered()), this, SLOT(forward()) );
    connect( ui.actionBack, SIGNAL(triggered()), ui.WebView, SLOT(stop()) );
}

void WebWindow::setupState(){
    // Sets up the state machine
    QState *normalMode = new QState();
    QState *clippingMode = new QState();
    normalMode->addTransition(ui.clippingModeButton, SIGNAL(clicked()), clippingMode);
    clippingMode->addTransition(ui.clippingModeButton, SIGNAL(clicked()), normalMode);
    normalMode->assignProperty(ui.clippingModeButton, "text", "Clipping Mode");
    clippingMode->assignProperty(ui.clippingModeButton, "text", "Exit Clipping Mode");
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
        ui.WebView->setFixedSize(e->size().width(), e->size().height() - 120);
        ui.ClickArea->setGeometry(ui.WebView->x(),ui.WebView->y(),ui.WebView->width(),ui.WebView->height());
        ui.NavigationBox->setGeometry(0, 0, e->size().width(), 100);
        geometrySet = false;
    }
}

void WebWindow::createMask(QRegion region) {
   setMask(region);
   ui.WebView->blockSignals(true);
}

void WebWindow::removeMask() {
   clearMask();
   ui.WebView->blockSignals(false);
}

void WebWindow::go(){
    navigate(ui.addressBar->text());
}

void WebWindow::newWindow(){
	WebWindow *w = new WebWindow(wwparent);
	w->show();
}

void WebWindow::startClippingMode(){
    //enter clipping mode
    if (geometrySet) {
        restoreClip();
    }
    ui.ClickArea->update();
    ui.ClickArea->setEnabled(true);
    ui.ClickArea->raise();
    ui.ClickArea->show();
}

void WebWindow::exitClippingMode() {
    //exit clipping mode
    clipped = true;
    QRegion capturedRegion = ui.ClickArea->getCapturedRegion();
    ui.NavigationBox->setHidden(true);

    windowGeometry = ui.WebView->geometry();
    mainGeometry = this->geometry();
    geometrySet = true;

    ui.WebView->setGeometry(-capturedRegion.boundingRect().left(), -capturedRegion.boundingRect().top(), ui.WebView->width(), ui.WebView->height());
    this->setGeometry(mainGeometry.left(), mainGeometry.top(), capturedRegion.boundingRect().width(), capturedRegion.boundingRect().height() + 20);
    setMask(QRegion(0, 0, capturedRegion.boundingRect().width(), capturedRegion.boundingRect().height(), QRegion::Rectangle));

    ui.ClickArea->setDisabled(true);
    ui.ClickArea->lower();
    ui.ClickArea->hide();
    ui.ClickArea->update();
}

// Button Functionality
void WebWindow::forward(){
    //go forward in currentWindow
    if (ui.WebView->history()->canGoForward()) {
        ui.WebView->history()->forward();
    }
    checkHistoryState();
}

void WebWindow::back(){
    //go backwards in currentWindow
    if (ui.WebView->history()->canGoBack()) {
        ui.WebView->history()->back();
    }
    checkHistoryState();
}

void WebWindow::reload(){
        ui.WebView->reload();
}


void WebWindow::navigate( QString url ){
    //navigate to url
    //code injections possible?
    if (!url.startsWith("http://", Qt::CaseInsensitive)) {
        url = "http://" + url;
        ui.addressBar->setText(url);
    }
    ui.WebView->setUrl(QUrl(url));
}

void WebWindow::restoreClip(){
    removeMask();
    ui.WebView->setGeometry(windowGeometry);
    this->setGeometry(mainGeometry);
    ui.NavigationBox->setVisible(true);
    clipped = false;
    update();
}

void WebWindow::checkHistoryState() {
    if (!setGoBack && ui.WebView->history()->canGoBack()) {
        setGoBack = true;
        ui.backButton->setEnabled(true);
        ui.backButton->setIcon(QIcon(":/icons/BackButton.png"));
    } else {
        setGoBack = false;
        ui.backButton->setDisabled(true);
        ui.backButton->setIcon(QIcon(":/icons/BackButton.Disabled.png"));
    }
    if (!setGoForward) {
        setGoForward = true;
        ui.forwardButton->setEnabled(true);
        ui.forwardButton->setIcon(QIcon(":/icons/ForwardButton.png"));
    } else {
        setGoForward = false;
        ui.forwardButton->setEnabled(true);
        ui.forwardButton->setIcon(QIcon(":/icons/ForwardButton.Disabled.png"));
    }
}
