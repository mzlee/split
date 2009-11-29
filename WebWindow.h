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
    WebWindow(QNetworkCookieJar *argCookieJar = 0, QString defaultUrl = "about:blank");

signals:
    void wwFocus(WebWindow *ww);            // emitted when a window gains focus
    void wwRestoreWindow(WebWindow *ww);    // emitted when a window is restored

protected:
    //void _changeEvent(QEvent *event);      // a virtual event function that must be
                                            //      implemented (to keep track of window focus)
    void _resizeEvent(QResizeEvent *);       // virtual event function to update the size of
                                            //      the webview and the mouse tracking

private slots:
    void _selectAddrBar();                  // used by hotkeys
    //void _createMask(QRegion region);       // creates the basic mask
    void _removeMask();                     // removes the basic mask
    void _storeMask(QRegion region);        // stores the mask for later use
    void _updateStatus(const QString &q);   // updates the status bar
    void _go();                             // navigates the window to the url in the address bar
    void _newWindow(const QString &url = "about:blank");    //opens a new top level window which contains a browser
    void _newWindow(const QUrl &url);
    void _startClippingMode();              // enters clipping mode
    void _exitClippingMode();               // exits clipping mode
    void _setClip(QRegion region);          // sets a clip
    void _forward();                        // moves the browser forward
    void _back();                           // moves the browser back
    void _restoreClip();                    // restores a clip to its original size
    void _setWebKitState();                 // queries WebKit and sets values accordingly

private:
    void _setupShortcuts();
    void _resizeAddressBox();
    void _navigate(QUrl url);               // navigate the WebWindow's page to url
    void _navigate(QString url);
    void _setupConnections();               // sets up the connection
    void _setupState();                     // sets up the state machine on the WebWindow
    void _destroySharedClips();             // let other clips know that this window is no longer a clip

    Ui::WebWindow _ui;                      // UI Designer Form
    QStateMachine _machine;
    QRect _mainGeometry;                    // saved geometry
    QRect _windowGeometry;
    QVector<QRegion> _storedMasks;          // masks
    QVector<WebWindow*> _sharedClips;
    bool _geometrySet;                      // state flags
        // _geometrySet is set TRUE when the window is first clipped
        //      and it saves the window rect into _windowGeometry.
        //      _mainGeometry is for the WebView(?) aka the client area.
    bool _clipped;
    QNetworkCookieJar *_cookieJar;          // Browser's data store for web cookies
};

#endif //WEBWINDOW_H
