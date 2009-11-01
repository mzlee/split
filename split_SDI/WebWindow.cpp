#include "WebWindow.h"
#include <QEvent>
#include <QGraphicsWidget>
#include <QUrl>
#include <QWebView>
#include <QWebHistory>
#include <iostream>

WebWindow::WebWindow( QWidget *parent ) : QMainWindow( parent ){
	
    //setup the window
    ui.setupUi( this );

    //setup state machine
    setupState();

    //setup connections
    setupConnections();

    //setup mouse capture widget
    ui.ClickArea->lower();
    ui.ClickArea->setHidden(true);

}

void WebWindow::setupConnections(){

    //connect the click() action of each button to a function
    connect( ui.forwardButton, SIGNAL(clicked()), this, SLOT(forward()) );
    connect( ui.backButton, SIGNAL(clicked()), this, SLOT(back()) );
    connect( ui.goButton, SIGNAL(clicked()), this, SLOT(go()) );
    connect( ui.actionNewWindow, SIGNAL(triggered()), this, SLOT(newWindow()) );
    connect( ui.actionRestoreWindow, SIGNAL(triggered()), this, SLOT(restoreClip()) );
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

//if the window starts stretching infinitely... look here
void WebWindow::resizeEvent(QResizeEvent * e) {
    ui.WebView->setFixedSize(e->size().width(), e->size().height() - 20);
    ui.ClickArea->setFixedSize(e->size().width(), e->size().height() - 20);
    //ui.horizontalLayout->setFixedSize(this->width()-20,ui.horizontalLayout->height);
    //ui.WebView->setFixedSize(e->size());
    //ui.ClickArea->setFixedSize(e->size());
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
    //create a new window
}

void WebWindow::startClippingMode(){
    //enter clipping mode
    ui.ClickArea->update();
    ui.ClickArea->setEnabled(true);
    ui.ClickArea->raise();
    ui.ClickArea->show();
}

void WebWindow::exitClippingMode() {
    //exit clipping mode
    setMask(ui.ClickArea->getCapturedRegion());
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
    update();
}

/*
//implementing a virtual function to make use of changeEvent
void WebWindow::changeEvent( QEvent *event ){

    //99 is the type returned when a window loses or gains focus
    if(event->type() == 99){
        //if it didn't previously have focus, it alerts the main window
        //that it is getting focus
        if(!focus){
            focus = true;
        }else{
            //if it did have focus, this means it's losing focus
            focus = false;
        }
    }
}
*/
