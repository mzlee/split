#include "WebWindow.h"
#include "MainWindow.h"
#include <QEvent>
#include <QGraphicsWidget>
#include <QUrl>
#include <QWebView>
#include <iostream>

WebWindow::WebWindow( QWidget *parent ) : QDialog( parent ){
	
        //setup state machine
        setupState();

	//show the window
	ui.setupUi( this );

	//this window does not have focus
	focusCounter = false; 
	
	//if a WebWindow comes into focus, the MainWindow must know
	connect( this, SIGNAL(wwFocus(WebWindow*)), parent, SLOT(setCurrentWindow(WebWindow*)) );
}

void WebWindow::resizeEvent(QResizeEvent * e) {
        ui.WebView->setFixedSize(e->size());
        ui.ClickArea->setFixedSize(e->size());
}

//void WebWindow::recordMouseDown() {
//
//}
//
//void WebWindow::recordMouseUp() {
//    ui.WebView->setGeometry(start_x, start_y, end_x, end_y);
//}

void WebWindow::startClipping() {
        ui.ClickArea->setEnabled(true);
        ui.ClickArea->show();
        ui.ClickArea->setText("Clipping");
}

void WebWindow::exitClipping() {
        ui.ClickArea->setDisabled(true);
        ui.ClickArea->hide();
        ui.ClickArea->setText("Not Clipping");
}

//void WebWindow::saveWindowSize() {
//        window_geometry = this->saveGeometry();
//}
//
//void WebWindow::restoreWindowSize() {
//        this->restoreGeometry(window_geometry);
//}

void WebWindow::setupState(){
        // Sets up the state machine
        QState *normalWindow = new QState();
        QState *clippedWindow = new QState();
        //how do we get the signal
        machine.addState(normalWindow);
        machine.addState(clippedWindow);
        machine.setInitialState(normalWindow);
        machine.start();
}

//implementing a virtual function to make use of changeEvent
void WebWindow::changeEvent( QEvent *event ){

	//99 is the type returned when a window loses or gains focus
	if(event->type() == 99){
		//if it didn't previously have focus, it alerts the main window
		//that it is getting focus
		if(!focusCounter){
			emit wwFocus(this);
			focusCounter = true;
		}else{
			//if it did have focus, this means it's losing focus
			focusCounter = false;
		}
		
	}
}

void WebWindow::navigate( QString url ){
	
	//navigate to url
	//code injections possible?
	ui.WebView->setUrl(QUrl(url));

}
