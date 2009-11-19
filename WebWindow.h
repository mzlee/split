//////////////////////////////////////////////////
//                                              //
// WebWindow.h                                  //
//   -> part of Split                           //
//                                              //
// Copyright (c) 2009 Jim Brusstar, Michael Lee //
//                 Ben Montgomery, Robert Steen //
//                                              //
//////////////////////////////////////////////////

#ifndef WEBWINDOW_H
#define WEBWINDOW_H

#include "ui_WebWindow.h"
#include <QMainWindow>
#include <QStateMachine>
#include <QVector>

class QWidget;
class QNetworkCookieJar;
class QString;

class WebWindow : public QMainWindow
{
    Q_OBJECT

public:
    WebWindow(QWidget *parent = 0, QNetworkCookieJar *jar = 0, QString defaultUrl = "about:blank");

signals:
    void wwFocus(WebWindow *ww);            // emitted when a window gains focus
    void wwRestoreWindow(WebWindow *ww);    // emitted when a window is restored

protected:
    //void changeEvent(QEvent *event);    // a virtual event function that must be
                                            //      implemented (to keep track of window focus)
    void resizeEvent(QResizeEvent *);       // virtual event function to update the size of
                                            //      the webview and the mouse tracking

private slots:
    void selectAddrBar();                   // used by hotkeys
    void createMask(QRegion region);        // creates the basic mask
    void removeMask();                      // removes the basic mask
    void storeMask(QRegion region);         // stores the mask for later use
    void updateStatus(const QString &q);    // updates the status bar
    void go();                              // navigates the window to the url in the address bar
    void newWindow(const QString &url = "about:blank");    //opens a new top level window which contains a browser
    void newWindow(const QUrl &url);
    void startClippingMode();               // enters clipping mode
    void exitClippingMode();                // exits clipping mode
    void setClip(QRegion region);           // sets a clip
    void forward();                         // moves the browser forward
    void back();                            // moves the browser back
    void restoreClip();                     // restores a clip to its original size
    void setWebKitState();                  // queries WebKit and sets values accordingly

private:
    void setupShortcuts();
    void resizeAddressBox();
    void navigate(QUrl url);                // navigate the WebWindow's page to url
    void navigate(QString url);
    void setupConnections();                // sets up the connection
    void setupState();                      // sets up the state machine on the WebWindow
    void destroySharedClips();              // let other clips know that this window is no longer a clip

    Ui::WebWindow ui;                       // the ui form
    QStateMachine machine;
    QRect mainGeometry;                     // saved geometry
    QRect windowGeometry;
    QVector<QRegion> storedMasks;           // masks
    QVector<WebWindow*> sharedClips;
    bool geometrySet;                       // state flags
    bool clipped;
    QWidget *wwparent;                      //state flags
    QNetworkCookieJar *cookieJar;
};

#endif //WEBWINDOW_H
