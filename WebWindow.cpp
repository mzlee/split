#include "WebWindow.h"
#include <QEvent>
#include <QGraphicsWidget>
#include <QUrl>
#include <QWebView>
#include <QWebHistory>
#include <iostream>

WebWindow::WebWindow( QWidget *parent ) : QMainWindow( parent ){

    wwparent = parent;

    //show the window
    ui.setupUi( this );

    ui.ClickArea->lower();
    ui.ClickArea->setHidden(true);

    setupConnections();

    //setup state machine
    setupState();

    ui.statusBar->showMessage("MESSAGE");
    clipped = false;
    geometrySet = false;

    this->navigate("www.google.com");
}

void WebWindow::setupConnections(){
    //connect the click() action of each button to a function
    connect( ui.forwardButton, SIGNAL(clicked()), this, SLOT(forward()) );
    connect( ui.backButton, SIGNAL(clicked()), this, SLOT(back()) );
    connect( ui.goButton, SIGNAL(clicked()), this, SLOT(go()) );
    connect( ui.stopButton, SIGNAL(clicked()), ui.WebView, SLOT(stop()) );
    connect( ui.reloadButton, SIGNAL(clicked()), ui.WebView, SLOT(reload()) );

    connect( ui.WebView, SIGNAL(statusBarMessage(const QString &)), this, SLOT(updateStatus(const QString &)));

    connect( ui.actionNewWindow, SIGNAL(triggered()), this, SLOT(newWindow()) );
    connect( ui.actionRestoreWindow, SIGNAL(triggered()), this, SLOT(restoreClip()) );
    connect( ui.actionGo, SIGNAL(triggered()), this, SLOT(go()) );
    connect( ui.actionBack, SIGNAL(triggered()), this, SLOT(back()) );
    connect( ui.actionForward, SIGNAL(triggered()), this, SLOT(forward()) );
    connect( ui.actionReload, SIGNAL(triggered()), ui.WebView, SLOT(reload()) );
    connect( ui.actionBack, SIGNAL(triggered()), ui.WebView, SLOT(stop()) );

    connect( ui.ClickArea, SIGNAL(mcEndCapture(QRegion)), this, SLOT(storeMask(QRegion)) );
}

void WebWindow::setupState(){
    // Sets up the state machine
    QState *normalMode = new QState();
    QState *clippingMode = new QState();
    normalMode->addTransition(ui.clippingModeButton, SIGNAL(clicked()), clippingMode);
    clippingMode->addTransition(ui.clippingModeButton, SIGNAL(clicked()), normalMode);
    normalMode->assignProperty(ui.clippingModeButton, "text", "Clipping Mode");
    clippingMode->assignProperty(ui.clippingModeButton, "text", "Exit Clipping Mode");
    machine.addState(normalMode);
    machine.addState(clippingMode);
    machine.setInitialState(normalMode);
    connect( clippingMode, SIGNAL(entered()), this, SLOT(startClippingMode()) );
    connect( clippingMode, SIGNAL(exited()), this, SLOT(exitClippingMode()) );
    machine.start();
}

void WebWindow::updateStatus(const QString &message) {
    ui.statusBar->showMessage(message);
}

//if the window starts stretching infinitely... look here
void WebWindow::resizeEvent(QResizeEvent * e) {
    if (!clipped) {
        ui.WebView->setFixedSize(e->size().width(), e->size().height() - 120);
        ui.ClickArea->setGeometry(ui.WebView->x(),ui.WebView->y(),ui.WebView->width(),ui.WebView->height());
        ui.controlBox->setGeometry(0, 0, e->size().width(), 100);
        geometrySet = false;
    }
}

void WebWindow::createMask(QRegion region) {
   setMask(region);
   ui.WebView->blockSignals(true);
}

void WebWindow::storeMask(QRegion region) {
    storedMasks.push_back(region);
}

void WebWindow::removeMask() {
   clearMask();
   ui.WebView->blockSignals(false);
}

void WebWindow::go(){
    navigate(ui.addressBar->text());
}

void WebWindow::newWindow(){
	WebWindow *w = new WebWindow(wwparent);
	w->show();
}

void WebWindow::startClippingMode(){
    //enter clipping mode
    if (geometrySet) {
        restoreClip();
    }
    ui.ClickArea->update();
    ui.ClickArea->setEnabled(true);
    ui.ClickArea->raise();
    ui.ClickArea->show();

    storedMasks.clear();
    sharedClips.clear();
}

//exit clipping mode
void WebWindow::exitClippingMode() {
    //Combine masks that intersect
    for(int i=0; i<storedMasks.size(); i++)
    {
        for(int j=0; j<storedMasks.size(); j++)
        {
            if(i ==j)
                ; //Do nothing
            else if(!storedMasks[i].intersect(storedMasks[j]).isEmpty())
            {
                storedMasks[i] = storedMasks[i].united(storedMasks[j]);
                storedMasks.erase(storedMasks.begin() + j);
            }
        }
    }
    
    //Create each mask
    for(int i=0; i<storedMasks.size(); i++)
    {
        if(i==0)
        {
            setClip(storedMasks[0]);
        }
        else
        {
            WebWindow *w = new WebWindow(wwparent);
            w->show();
            w->navigate(ui.WebView->url().toString());
            w->storeMask(storedMasks[i]);
            w->setClip(storedMasks[i]);

            //Keep track of clips from same page
            sharedClips.push_back(w);
        }
    }

    //Make sure every new clip has a pointer to all the
    // other clips that were created from the same page.
    for(int i=0; i<sharedClips.size(); i++)
    {
        sharedClips[i]->sharedClips.push_back(this);

        for(int j=0; j<sharedClips.size(); j++)
        {
            if(i != j)
            {
                sharedClips[i]->sharedClips.push_back(sharedClips[j]);
            }
        }
    }
}

void WebWindow::setClip(QRegion region)
{
    clipped = true;
    ui.controlBox->setHidden(true);

    windowGeometry = ui.WebView->geometry();
    mainGeometry = this->geometry();
    geometrySet = true;


    ui.WebView->setGeometry(-region.boundingRect().left(), -region.boundingRect().top(), ui.WebView->width(), ui.WebView->height());
    this->setGeometry(mainGeometry.left(), mainGeometry.top(), region.boundingRect().width(), region.boundingRect().height() + 40);
    QRegion newRegion;
    //region.boundingRect().setTopLeft(QPoint(0, 0));
    QPoint p = region.boundingRect().topLeft();
    for(int i=0; i<region.rects().size(); i++)
    {
        QRect rect = region.rects()[i];
        rect.moveTopLeft(rect.topLeft() - p);
        rect.setHeight(rect.height() + 20);
        newRegion += QRegion(rect, QRegion::Rectangle);
    }
    setMask(newRegion);

    /*ui.WebView->setGeometry(0, 0, ui.WebView->width(), ui.WebView->height());
    //ui.menuBar->setGeometry(region.boundingRect().left(), region.boundingRect().top()-20, ui.menuBar->width(), ui.menuBar->height());
    int i = mainGeometry.left();
    int j = mainGeometry.top();
    int k = region.boundingRect().width();
    int l = region.boundingRect().height();
    int m = mainGeometry.width();
    int n = mainGeometry.height();
    this->setGeometry(mainGeometry.left(), mainGeometry.top(), mainGeometry.width(), mainGeometry.height()+20);
    //region.united(QRect(0, 0, region.boundingRect().width(), 20));
    setMask(region);
*/

    //region.united(QRect(0, 0, region.boundingRect().width(), 20));
    //ui.WebView->setGeometry(0, -region.boundingRect().top(), ui.WebView->width(), ui.WebView->height());
    //setMask(region);

    //ui.WebView->setGeometry(-region.boundingRect().left(), -region.boundingRect().top(), ui.WebView->width(), ui.WebView->height());
    //this->setGeometry(mainGeometry.left(), mainGeometry.top(), region.boundingRect().width(), region.boundingRect().height() + 20);
    //setMask(QRegion(0, 0, region.boundingRect().width(), region.boundingRect().height(), QRegion::Rectangle));

    ui.ClickArea->setDisabled(true);
    ui.ClickArea->lower();
    ui.ClickArea->hide();
    ui.ClickArea->update();
}


// Button Functionality
void WebWindow::forward(){
    //go forward in currentWindow
    if (ui.WebView->history()->canGoForward()) {
        ui.WebView->history()->forward();
    }
}

void WebWindow::back(){
    //go backwards in currentWindow
    if (ui.WebView->history()->canGoBack()) {
        ui.WebView->history()->back();
    }
}

void WebWindow::reload(){
        ui.WebView->reload();
}


void WebWindow::navigate( QString url ){
    //navigate to url
    //code injections possible?
    if (!url.startsWith("http://", Qt::CaseInsensitive)) {
        url = "http://" + url;
        ui.addressBar->setText(url);
    }
    ui.WebView->setUrl(QUrl(url));
}

void WebWindow::restoreClip(){
    removeMask();
    ui.WebView->setGeometry(windowGeometry);
    this->setGeometry(mainGeometry);
    ui.controlBox->setVisible(true);
    clipped = false;
    update();

    //Tell other clips that this one has been restored
    removeThisSharedClipFromOtherClips();
}


void WebWindow::removeThisSharedClipFromOtherClips()
{
    for(int i=0; i<sharedClips.size(); i++)
    {
        for(int j=0; j<sharedClips[i]->sharedClips.size(); j++)
        {
            if(sharedClips[i]->sharedClips[j] == this)
            {
                sharedClips[i]->sharedClips.erase(sharedClips[i]->sharedClips.begin() + j);
                break;
            }
        }
    }
}

/*
//implementing a virtual function to make use of changeEvent
void WebWindow::changeEvent( QEvent *event ){

    //99 is the type returned when a window loses or gains focus
    if(event->type() == 99){
        //if it didn't previously have focus, it alerts the main window
        //that it is getting focus
        if(!focus){
            focus = true;
        }else{
            //if it did have focus, this means it's losing focus
            focus = false;
        }
    }
}
*/
