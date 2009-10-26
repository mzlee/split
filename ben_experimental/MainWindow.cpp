#include "MainWindow.h"
#include "WebWindow.h"
#include <iostream>

MainWindow::MainWindow() : QDialog(){

	//initialize cw to null
	currentWindow = 0;

	//necessary call to setupUi
	ui.setupUi( this );
	
	//connect the click() action of each button to a function
	connect( ui.forwardButton, SIGNAL(clicked()), this, SLOT(forward()) );
	connect( ui.backButton, SIGNAL(clicked()), this, SLOT(back()) );
	connect( ui.newWindowButton, SIGNAL(clicked()), this, SLOT(newWindow()) );
	connect( ui.clippingModeButton, SIGNAL(clicked()), this, SLOT(clippingMode()) );
	connect( ui.restoreClipButton, SIGNAL(clicked()), this, SLOT(restoreClip()) );
	connect( ui.goButton, SIGNAL(clicked()), this, SLOT(go()) );

}

void MainWindow::go(){

	if(currentWindow){
		//current window exists, navigate to the text in the address bar
		currentWindow->navigate(ui.addressBar->text());
	}else{
		//there is no window selected
		//open a new one and navigate to the url
		newWindow();
		currentWindow->navigate(ui.addressBar->text());
	}

}

void MainWindow::newWindow(){

	//make a new WebWindow with MainWindow as parent
	WebWindow *newWindow = new WebWindow( this );

	//display modeless dialog
	newWindow->show();

	//set current window
	currentWindow = newWindow; 

}

void MainWindow::clippingMode(){
	//enter clipping mode

}

void MainWindow::forward(){
	//go forward in currentWindow

}

void MainWindow::back(){
	//go backwards in currentWindow

}

void MainWindow::restoreClip(){
	//if a clip is selected, restore it to full size

}

void MainWindow::setCurrentWindow( WebWindow *cw ){
	//set the current window

	currentWindow = cw;

}

