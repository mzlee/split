#include "WebWindow.h"
#include <QEvent>
#include <QGraphicsWidget>
#include <QUrl>
#include <QWebView>
#include <QWebPage>
#include <QWebFrame>
#include <QWebHistory>
#include <QIcon>

WebWindow::WebWindow( QWidget *parent, QString defaultUrl ) : QMainWindow( parent ){

    wwparent = parent;

    //show the window
    ui.setupUi( this );

    ui.ClickArea->lower();
    ui.ClickArea->setHidden(true);

    setupConnections();

    //setup state machine
    setupState();

    clipped = false;
    geometrySet = false;
    navigate(defaultUrl);
}

void WebWindow::setupConnections(){
    //connect the click() action of each button to a function
    connect( ui.backButton, SIGNAL(clicked()), this, SLOT(back()) );
    connect( ui.forwardButton, SIGNAL(clicked()), this, SLOT(forward()) );
    connect( ui.reloadButton, SIGNAL(clicked()), this, SLOT(go()) );
    connect( ui.stopButton, SIGNAL(clicked()), ui.WebView, SLOT(stop()) );

    connect( ui.WebView, SIGNAL(loadFinished(bool)), this, SLOT(setWebKitState()) );
    //connect( ui.WebView, SIGNAL(linkClicked(QUrl)), this, SLOT(newWindow(const QUrl &)) );
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
}

void WebWindow::back(){
    //go backwards in currentWindow
    if (ui.WebView->history()->canGoBack()) {
        ui.WebView->history()->back();
    }
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
