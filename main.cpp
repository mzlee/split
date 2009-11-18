#include <QApplication>
#include <QObject>
#include <QNetworkCookieJar>
#include "WebWindow.h"

int main( int argc, char **argv ){
	
    //initialize the resources
    Q_INIT_RESOURCE(Resources);
    //make an application
    QApplication app( argc, argv );
    //app.setQuitOnLastWindowClosed(false);
    QWidget *parent = new QWidget();
    QNetworkCookieJar* cookieJar = new QNetworkCookieJar(parent);
    //make and show the main window
    WebWindow ww(parent, cookieJar, "http://google.com");
    ww.show();
    //qt requires this
    return app.exec();

}
