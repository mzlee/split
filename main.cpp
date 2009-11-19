//////////////////////////////////////////////////
//                                              //
// main.cpp                                     //
//   -> part of Split                           //
//                                              //
// Copyright (c) 2009 Jim Brusstar, Michael Lee //
//                 Ben Montgomery, Robert Steen //
//                                              //
//////////////////////////////////////////////////

#include <QApplication>
#include <QObject>
#include <QNetworkCookieJar>
#include "WebWindow.h"

int main( int argc, char **argv )
{
    // Initialize our resources
    Q_INIT_RESOURCE(Resources);

    // Initialize the application
    QApplication SplitApp(argc, argv);
    QWidget *parent = 0;    // XXX THIS THING SUCKS. IF YOU DON'T SET *parent TO NULL,
                            // THE APP WILL CLOSE IF JUST ONE WINDOW CLOSES.
                            // SOMEONE == FAIL. -jb
    QNetworkCookieJar *cookieJar = new QNetworkCookieJar(parent);
    WebWindow ww(parent, cookieJar, "http://google.com");
    ww.show();

    // Run message loop
    return SplitApp.exec();
}
