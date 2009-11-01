#include "WebWindow.h"
#include <QEvent>
#include <QGraphicsWidget>
#include <QUrl>
#include <QWebView>
#include <iostream>

WebWindow::WebWindow( QWidget *parent ) : QMainWindow( parent ){
	
    
	wwparent = parent;
	//show the window
	ui.setupUi( this );

	//this window does not have focus yet
	focus = false; 
	
	setupConnections();
	
        //set the label apperance
        overlay_palette.setColor(QPalette::Base, QColor(128, 128, 128, 128));
        overlay_palette.setColor(QPalette::Window, QColor(128, 128, 128, 128));
        overlay_palette.setColor(QPalette::Background, QColor(128, 128, 128, 128));
        overlay_palette.setColor(QPalette::Button, QColor(128, 128, 128, 128));
        ui.ClickArea->setPalette(overlay_palette);
        ui.ClickArea->setHidden(true);
		
		//setup state machine
		setupState();
		ui.statusBar->showMessage("MESSAGE");
	
}

void WebWindow::setupConnections(){

		connect( ui.ClickArea, SIGNAL(mcEndCapture(QRegion)), this, SLOT(createMask(QRegion)) );

        //connect the click() action of each button to a function
        connect( ui.forwardButton, SIGNAL(clicked()), this, SLOT(forward()) );
        connect( ui.backButton, SIGNAL(clicked()), this, SLOT(back()) );
        //connect( ui.restoreClipButton, SIGNAL(clicked()), this, SLOT(restoreClip()) );
        connect( ui.goButton, SIGNAL(clicked()), this, SLOT(go()) );
		connect( ui.reloadButton, SIGNAL(clicked()), ui.WebView, SLOT(reload()) );
		
		connect( ui.WebView, SIGNAL(statusBarMessage(const QString &)), this, SLOT(updateStatus(const QString &)));
		
		connect( ui.actionNewWindow, SIGNAL(triggered()), this, SLOT(newWindow()) );
		connect( ui.actionRestoreWindow, SIGNAL(triggered()), this, SLOT(restoreClip()) );

}

void WebWindow::updateStatus(const QString &q){
	//ui.statusBar->showMessage(q);
}

//if the window starts stretching infinitely... look here
void WebWindow::resizeEvent(QResizeEvent * e) {
		ui.ClickArea->setGeometry(ui.WebView->x(),ui.WebView->y(),ui.WebView->width(),ui.WebView->height());
		//ui.horizontalLayout->setFixedSize(this->width()-20,ui.horizontalLayout->height);
        //ui.WebView->setFixedSize(e->size());
        //ui.ClickArea->setFixedSize(e->size());
}

void WebWindow::startClipping() {
    ui.ClickArea->update();
    ui.ClickArea->setEnabled(true);
	ui.ClickArea->raise();
    ui.ClickArea->show();
}

void WebWindow::exitClipping() {
    ui.ClickArea->setDisabled(true);
    ui.ClickArea->hide();
    ui.ClickArea->update();
}

void WebWindow::createMask(QRegion region) {
   setMask(region);
   ui.WebView->blockSignals(true);
}

void WebWindow::removeMask() {
   clearMask();
   ui.WebView->blockSignals(false);
}

void WebWindow::setupState(){
        // Sets up the state machine
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

void WebWindow::go(){

	navigate(ui.addressBar->text());

}

void WebWindow::newWindow(){
	WebWindow *w = new WebWindow(wwparent);
	w->show();
}

void WebWindow::startClippingMode(){
	//enter clipping mode
    startClipping();
}

void WebWindow::exitClippingMode() {
	//exit clipping mode
	exitClipping();
}

void WebWindow::forward(){
	//go forward in currentWindow
	ui.WebView->forward();
}

void WebWindow::back(){
	//go backwards in currentWindow
	ui.WebView->back();

}

void WebWindow::reload(){
	ui.WebView->reload();
}

void WebWindow::restoreClip(){
    //if a clip is selected, restore it to full size
    removeMask();
}
void WebWindow::navigate( QString url ){
	
	//navigate to url
	//code injections possible?
	ui.WebView->setUrl(QUrl(url));

}
