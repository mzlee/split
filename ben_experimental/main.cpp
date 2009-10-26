#include <QApplication>
#include "MainWindow.h"
#include "WebWindow.h"



int main( int argc, char **argv ){
	
	//make an application
	QApplication app( argc, argv );

	//make and show the main window
	MainWindow mw;

	mw.show();

	//qt requires this
	return app.exec();

}
