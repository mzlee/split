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
        connect( ui.ClickArea, SIGNAL(mcEndCapture(QRegion)), this, SLOT(createMask(QRegion)) );

        //set the label apperance
        overlay_palette.setColor(QPalette::Base, QColor(128, 128, 128, 128));
        overlay_palette.setColor(QPalette::Window, QColor(128, 128, 128, 128));
        overlay_palette.setColor(QPalette::Background, QColor(128, 128, 128, 128));
        overlay_palette.setColor(QPalette::Button, QColor(128, 128, 128, 128));
        ui.ClickArea->setPalette(overlay_palette);
        ui.ClickArea->setHidden(true);
}

void WebWindow::resizeEvent(QResizeEvent * e) {
        ui.WebView->setFixedSize(e->size());
        ui.ClickArea->setFixedSize(e->size());
}

void WebWindow::startClipping() {
    ui.ClickArea->update();
    ui.ClickArea->setEnabled(true);
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
        QState *normalWindow = new QState();
        QState *clippingWindow = new QState();
        QState *clippedWindow = new QState();
        //normalWindow->addTransition(ui.ClickArea, SIGNAL(mcStartCapture(MouseCaptureWidget*)), clippingWindow);
        //clippingWindow->addTransition(ui.ClickArea, SIGNAL(mcEndCapture(MouseCaptureWidget*)), clippedWindow);
        //clippedWindow->addTransition(this, SIGNAL(wwRestoreWindow(WebWindow*)), normalWindow);
        machine.addState(normalWindow);
        machine.addState(clippingWindow);
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
                        emit wwRestoreWindow(this);
		}
		
	}
}

void WebWindow::navigate( QString url ){
	
	//navigate to url
	//code injections possible?
	ui.WebView->setUrl(QUrl(url));

}
