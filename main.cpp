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

int main(int argc, char **argv)
{
    // Initialize our resources
    Q_INIT_RESOURCE(Resources);

    // Initialize the application
    QApplication SplitApp(argc, argv);
    QNetworkCookieJar *cookieJar = new QNetworkCookieJar();
    WebWindow ww(cookieJar, "http://google.com");
    ww.show();

    // Run message loop
    return SplitApp.exec();
}
