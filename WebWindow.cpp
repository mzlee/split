//////////////////////////////////////////////////
//                                              //
// WebWindow.cpp                                //
//   -> part of Split                           //
//                                              //
// Copyright (c) 2009 Jim Brusstar, Michael Lee //
//                 Ben Montgomery, Robert Steen //
//                                              //
//////////////////////////////////////////////////

#include "WebWindow.h"
#include <QEvent>
#include <QGraphicsWidget>
#include <QUrl>
#include <QWebView>
#include <QWebPage>
#include <QWebFrame>
#include <QWebHistory>
#include <QIcon>
#include <QKeySequence>
#include <QShortcut>
#include <QStateMachine>

WebWindow::WebWindow(QNetworkCookieJar *argCookieJar, QString defaultUrl)
{
    // Setup the window
    _ui.setupUi(this);              // Build form UI
    _ui.ClickArea->setHidden(true); // Hide the clipping field
    _setupConnections();            // Connect signals/slots
    _setupShortcuts();              // Init keyboard shortcuts
    _setupState();                  // Init the state machine
    _clipped = false;
    _geometrySet = false;
    _navigate(defaultUrl);

    // Set the cookie jar
    _cookieJar = argCookieJar;
    _ui.WebView->page()->networkAccessManager()->setCookieJar(_cookieJar);
}

void WebWindow::_setupShortcuts()
{
    // Init keyboard shortcuts
    //      new window			ctrl + n
    //      go					ctrl + g
    //      press clip button	ctrl + m
    //      select address bar	ctrl + l
    QShortcut* qsc = new QShortcut(QKeySequence("Ctrl+n"),this);    // new window
    connect( qsc, SIGNAL(activated()), this, SLOT(_newWindow()) );
    qsc = new QShortcut(QKeySequence("Ctrl+g"),this);               // go
    connect( qsc, SIGNAL(activated()), this, SLOT(_go()) );
    qsc = new QShortcut(QKeySequence("Ctrl+m"),this);               // clip/no-clip
    connect( qsc, SIGNAL(activated()), _ui.clippingModeButton, SLOT(_click()) );
    qsc = new QShortcut(QKeySequence("Ctrl+l"),this);               // set url bar
    connect( qsc, SIGNAL(activated()), this, SLOT(_selectAddrBar()) );
    return;
}

void WebWindow::_selectAddrBar()
{
    // Highlight the text of the address bar
    //  and gain keyboard focus
    _ui.addressBar->setFocus(Qt::OtherFocusReason);
    _ui.addressBar->selectAll();
    return;
}

void WebWindow::_setupConnections()
{
    // XXX THIS FUNCTION REQURES BETTER DOCUMENTATION

    //connect the click() action of each button to a function
    connect( _ui.backButton, SIGNAL(clicked()), this, SLOT(_back()) );
    connect( _ui.forwardButton, SIGNAL(clicked()), this, SLOT(_forward()) );
    connect( _ui.reloadButton, SIGNAL(clicked()), this, SLOT(_go()) );
    connect( _ui.stopButton, SIGNAL(clicked()), _ui.WebView, SLOT(_stop()) );

    connect( _ui.addressBar, SIGNAL(returnPressed()), this, SLOT(_go()) );
    connect( _ui.WebView, SIGNAL(loadFinished(bool)), this, SLOT(_setWebKitState()) );
    connect( _ui.WebView, SIGNAL(linkClicked(QUrl)), this, SLOT(_newWindow(const QUrl &)) );
    connect( _ui.WebView, SIGNAL(statusBarMessage(const QString &)), this, SLOT(_updateStatus(const QString &)));

    connect( _ui.actionNewWindow, SIGNAL(triggered()), this, SLOT(_newWindow()) );
    connect( _ui.actionRestoreWindow, SIGNAL(triggered()), this, SLOT(_restoreClip()) );
    connect( _ui.actionReload, SIGNAL(triggered()), this, SLOT(_go()) );
    connect( _ui.actionBack, SIGNAL(triggered()), this, SLOT(_back()) );
    connect( _ui.actionForward, SIGNAL(triggered()), this, SLOT(_forward()) );
    connect( _ui.actionBack, SIGNAL(triggered()), _ui.WebView, SLOT(_stop()) );

    connect( _ui.ClickArea, SIGNAL(mcEndCapture(QRegion)), this, SLOT(_storeMask(QRegion)) );

    return;
}

void WebWindow::_setupState()
{
    // XXX THIS FUNCTION REQUIRES BETTER DOCUMENTATION

    // Initialize the browser's internal state machine
    //  to track the various modes (i.e. clipping mode)
    QState *normalMode = 0, *clippingMode = 0;  // Allocate new QStates
    normalMode = new QState();
    clippingMode = new QState();
    normalMode->addTransition(_ui.clippingModeButton, SIGNAL(clicked()), clippingMode);
    clippingMode->addTransition(_ui.clippingModeButton, SIGNAL(clicked()), normalMode);
    normalMode->assignProperty(_ui.clippingModeButton, "checked", false);
    normalMode->assignProperty(_ui.clippingModeButton, "icon", ":icons/clip");
    clippingMode->assignProperty(_ui.clippingModeButton, "checked", true);
    clippingMode->assignProperty(_ui.clippingModeButton, "icon", ":icons/clip");
    _machine.addState(normalMode);
    _machine.addState(clippingMode);
    _machine.setInitialState(normalMode);
    connect( clippingMode, SIGNAL(entered()), this, SLOT(_startClippingMode()) );
    connect( clippingMode, SIGNAL(exited()), this, SLOT(_exitClippingMode()) );
    _machine.start();
    return;
}

void WebWindow::_updateStatus(const QString &message)
{
    // Updates the string in the status bar
    _ui.statusBar->showMessage(message);
    return;
}

void WebWindow::_resizeEvent(QResizeEvent *e)
{
    // Responds to resizing events, sets window geometry
    //  to reflect changes in the new frame. (This is
    //  like Win32's 'move-size' loop, for us Microsofties.

    // XXX THIS FUNCTION IS FULL OF MAGIC NUMBERS, THIS NEEDS TO BE REMEDIED.

    if (!_clipped)   //if the window starts stretching infinitely... look here
    {
        _ui.WebView->setFixedSize(e->size().width(), e->size().height() - 54);
        _ui.ClickArea->setGeometry(_ui.WebView->x(), _ui.WebView->y(), _ui.WebView->width(), _ui.WebView->height());
        _ui.NavigationBox->setGeometry(0, 0, e->size().width(), 34);
        _resizeAddressBox();
        _geometrySet = false;
    }
    return;
}

void WebWindow::_resizeAddressBox()
{
    // Function to resize the address box, called in
    //  case of resizing events

    // XXX WHY IS THIS PROCEDURE NOT A PART OF ::resizeEvent? And the magic numbers?!

    _ui.clippingModeButton->setGeometry(_ui.NavigationBox->size().width() - 33, 0, 32, 32);
    _ui.stopButton->setGeometry(_ui.NavigationBox->size().width() - 64, 0, 32, 32);
    _ui.reloadButton->setGeometry(_ui.NavigationBox->size().width() - 95, 0 , 32, 32);
    _ui.addressBar->setGeometry(68, 3, _ui.NavigationBox->size().width() - 168, 28);
    return;
}

void WebWindow::_createMask(QRegion region)
{
    // Creates a clipping mask for the web view.
    setMask(region);
    _ui.WebView->blockSignals(true); // XXX WHAT IS THIS FOR? IT LOOKS LIKE A HACK.
    return;
}

void WebWindow::_storeMask(QRegion region)
{
    // Saves the geometry of a clipping mask.
    _storedMasks.push_back(region);
    return;
}

void WebWindow::_removeMask()
{
    // Clears the saved clipping masks.
    clearMask();
    _ui.WebView->blockSignals(false);   // XXX THE OTHER HALF OF THE ::_createMask HACK...
   return;
}

void WebWindow::_go()
{
    // Directs the browser to load a page.
    // XXX THIS IS SIMPLE ENOUGH, WHY IS IT NOT JUST PART OF ::_navigate?
    _navigate(_ui.addressBar->text());
    return;
}

void WebWindow::_newWindow(const QUrl &url)
{
    // Creates a new browser window and directs it to a QUrl.
    _newWindow(url.toString());
    return;
}

void WebWindow::_newWindow(const QString &url)
{
    // Creates a new browser window and directs it to a URL string.
    WebWindow *w = new WebWindow(_cookieJar, url);
    w->show();
    w->setFocus();
    return;
}

void WebWindow::_startClippingMode()
{
    // Enter clipping mode.
    // XXX ALL THIS CRAP IS NOT WELL DOCUMENTED ENOUGH.
    if (_geometrySet)
    {
        _restoreClip();
    }
    _ui.ClickArea->update();
    _ui.ClickArea->setEnabled(true);
    _ui.ClickArea->raise();
    _ui.ClickArea->show();
    _storedMasks.clear();
    _sharedClips.clear();
    return;
}


void WebWindow::_exitClippingMode() // XXX THIS FUNCTION HAS A SHITTY NAME. IT SHOULD BE CALLED
                                    // "_stopClippingMode", SINCE _STOP_ IS THE OPPOSITE OF
                                    // _START_, NOT "_exit..."
{
    // Exits clipping mode, returns to
    //  normal browsing mode.

    // XXX THIS FUNCTION HAS ISSUES.

    for (int i = 0; i < _storedMasks.size(); ++i)    // If multiple masks, intersect them...
    {
        for (int j = 0; j < _storedMasks.size(); ++j)
        {
            if (i == j)
            { }     // XXX DEAD CODE INTENTIONALLY?! WHO IS PROGRAMMING ON CRYSTAL METH?!
            else if (!_storedMasks[i].intersect(_storedMasks[j]).isEmpty())
            {
                _storedMasks[i] = _storedMasks[i].united(_storedMasks[j]);
                _storedMasks.erase(_storedMasks.begin() + j);
            }
        }
    }

    for (int i = 0; i < _storedMasks.size(); ++i) // Create each mask // XXX WHAT DOES THAT MEAN?
    {
        if (i == 0)
        {
            _setClip(_storedMasks[0]);
        }
        else
        {
            WebWindow *w = new WebWindow(_cookieJar, _ui.WebView->url().toString());
            w->show();
            w->_storeMask(_storedMasks[i]);
            w->_setClip(_storedMasks[i]);
            _sharedClips.push_back(w);  // Keep track of clips from same page // XXX EXPLAIN, PLEASE.
        }
    }

    for (int i = 0; i < _sharedClips.size(); ++i)   // Make sure every new clip has a pointer
                                                    // to all the other clips that were created
                                                    // from the same page.
                                                    // XXX WHAT, WHY? SAY THE _EFFECTS_ OF THE CODE.
    {
        _sharedClips[i]->_sharedClips.push_back(this);
        for (int j = 0; j < _sharedClips.size(); j++)
        {
            if (i != j)
            {
                _sharedClips[i]->_sharedClips.push_back(_sharedClips[j]);
            }
        }
    }
    return;
}

void WebWindow::_setClip(QRegion region)
{
    // XXX THIS FUNCTION NEEDS DOCUMENTATION! WHAT WERE YOU THINKING?!

    _clipped = true;
    _ui.NavigationBox->setHidden(true);

    _windowGeometry = _ui.WebView->geometry();
    _mainGeometry = this->geometry();
    _geometrySet = true;

    _ui.WebView->setGeometry(-region.boundingRect().left(),
                             -region.boundingRect().top(),
                             _ui.WebView->width(),
                             _ui.WebView->height());
    this->setGeometry(_mainGeometry.left(),
                      _mainGeometry.top(),
                      region.boundingRect().width(),
                      region.boundingRect().height() + 40);
    QRegion newRegion;
    QPoint p = region.boundingRect().topLeft();
    for (int i = 0; i < region.rects().size(); ++i)
    {
        QRect rect = region.rects()[i];
        rect.moveTopLeft(rect.topLeft() - p);
        rect.setHeight(rect.height() + 20);
        newRegion += QRegion(rect, QRegion::Rectangle);
    }
    setMask(newRegion);

    _ui.ClickArea->setDisabled(true);
    _ui.ClickArea->lower();
    _ui.ClickArea->update();

    _ui.WebView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    return;
}

void WebWindow::_forward()
{
    // Load 'forward' page.
    if (_ui.WebView->history()->canGoForward())
    {
        _ui.WebView->history()->forward();
    }
    return;
}

void WebWindow::_back()
{
    // Load 'back' page.
    if (_ui.WebView->history()->canGoBack())
    {
        _ui.WebView->history()->back();
    }
    return;
}

void WebWindow::_navigate(QUrl url)
{
    // Naviage browser to QUrl
    _ui.addressBar->setText(url.toString()); // Warning: Are code injections possible?
    _ui.WebView->setUrl(url);
    return;
}

void WebWindow::_navigate(QString url)
{
    // Navigate browserl to URL string.
    //  This is a wrapper for QUrl-based _navigate
    if (!url.contains("://", Qt::CaseInsensitive))
    {
        url = "http://" + url;
    }
    _navigate(QUrl(url));
    return;
}

void WebWindow::_restoreClip()
{
    // XXX THIS FUNCTION NEEDS DOCUMENTATION! WHAT IS THIS, THE OPPPOSITE OF setClip?!
    //  I SHOULD KNOW THAT!
    _removeMask();
    _ui.WebView->setGeometry(_windowGeometry);
    this->setGeometry(_mainGeometry);
    _ui.NavigationBox->setVisible(true);
    _clipped = false;
    update();
    _destroySharedClips();  // Tell other clips that this one has been restored // XXX WHY?
    _ui.WebView->page()->setLinkDelegationPolicy(QWebPage::DontDelegateLinks);
        // Tell the page to start capturing link clicks again
    return;
}

void WebWindow::_destroySharedClips()
{
    // XXX THIS FUNCTION NEEDS DOCUMENTATION
    for (int i = 0; i < _sharedClips.size(); ++i)
    {
        for (int j = 0; j < _sharedClips[i]->_sharedClips.size(); ++j)
        {
            if (_sharedClips[i]->_sharedClips[j] == this)
            {
                _sharedClips[i]->_sharedClips.erase(_sharedClips[i]->_sharedClips.begin() + j);
                break;
            }
        }
    }
    return;
}

void WebWindow::_setWebKitState()
{
    // XXX THIS FUNCTION NEEDS DOCUMENTATION
    if (_ui.WebView->history()->canGoBack())
    {
        _ui.backButton->setEnabled(true);
        _ui.backButton->setIcon(QIcon(":/icons/back"));
    }
    else
    {
        _ui.backButton->setDisabled(true);
        _ui.backButton->setIcon(QIcon(":/icons/back.disabled"));
    }

    if (_ui.WebView->history()->canGoForward())
    {
        _ui.forwardButton->setEnabled(true);
        _ui.forwardButton->setIcon(QIcon(":/icons/forward"));
    }
    else
    {
        _ui.forwardButton->setDisabled(true);
        _ui.forwardButton->setIcon(QIcon(":/icons/forward.disabled"));
    }

    _ui.addressBar->setText(_ui.WebView->url().toString());
    this->setWindowTitle("Split -- " + _ui.WebView->title());
    return;
}
