#include "WebWindow.h"
#include "MainWindow.h"
#include <QEvent>
#include <QGraphicsWidget>
#include <QUrl>
#include <QWebView>
#include <iostream>

WebWindow::WebWindow( QWidget *parent ) : QDialog( parent ){
	
	//show the window
	ui.setupUi( this );
	
	//this window does not have focus
	focusCounter = false; 
	
	//if a WebWindow comes into focus, the MainWindow must know
	connect( this, SIGNAL(wwFocus(WebWindow*)), parent, SLOT(setCurrentWindow(WebWindow*)) );
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
