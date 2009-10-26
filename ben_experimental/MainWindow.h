#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_MainWindow.h"
#include "WebWindow.h"

class MainWindow : public QDialog{

	Q_OBJECT

public:

	MainWindow();
	
private slots:
	
	//navigate the most recently selected window to the url in the address bar
	//or create a new window and navigate 
	void go();

	//opens a new top level window which contains a browser
	void newWindow();

	//enters the clipping mode
	void clippingMode();

	//moves the browser forward
	void forward();

	//moves the browser backwards
	void back();

	//restores a clip to its main size
	void restoreClip();

	//modifier for *currentWindow
	void setCurrentWindow( WebWindow *cw );

private:
	//qt needs this to set up the ui
	Ui::MainWindow ui;

	//currentWindow points to the WebWindow that navigation commands will go to
	WebWindow *currentWindow;
};



#endif //MAINWINDOW_H
