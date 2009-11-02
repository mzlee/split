#include <QApplication>
#include <QObject>
#include "WebWindow.h"

int main( int argc, char **argv ){
	
	//make an application
	QApplication app( argc, argv );
	//app.setQuitOnLastWindowClosed(false);
	QWidget *parent = new QWidget();

	//make and show the main window
	WebWindow ww(parent);
	
	ww.show();
	
	//qt requires this
	return app.exec();

}
