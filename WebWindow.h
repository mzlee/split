#ifndef WEBWINDOW_H
#define WEBWINDOW_H
#include "ui_WebWindow.h"
#include <QString>
#include <QWidget>
#include <QFocusEvent>
#include <QStateMachine>
#include <QMainWindow>
#include <QVector>

class WebWindow : public QMainWindow {

	Q_OBJECT //this needs to be here for signals/slots

public:

        WebWindow( QWidget *parent = 0, QNetworkCookieJar *jar = 0, QString defaultUrl = "about:blank" );

signals:
	
	//this is emitted when a window gains focus
	void wwFocus(WebWindow *ww);

	//this is emitted when a window is restored
	void wwRestoreWindow(WebWindow *ww);

protected:
	
	//this is a virtual event function that must be implemented
	//(used to keep track of window focus)
        //void changeEvent( QEvent *event );

        //this is a virtual event function that updates the size of the webview and the mouse tracking
        void resizeEvent(QResizeEvent *);

private slots:

		//used by hotkeys
		void selectAddrBar();

        //this creates the basic mask
        void createMask(QRegion region);

        //this removes the basic mask
        void removeMask();

        //this stores the mask for later use
        void storeMask(QRegion region);

        //this updates the status bar
	void updateStatus(const QString &q);
	
        //navigate the window to the url in the address bar
	void go();

	//opens a new top level window which contains a browser
        void newWindow();

	//enters the clipping mode
        void startClippingMode();

	//exits the clipping mode
	void exitClippingMode();

        //sets a clip
        void setClip(QRegion region);

	//moves the browser forward
	void forward();

	//moves the browser backwards
	void back();

	//restores a clip to its main size
	void restoreClip();

        //checks the webkit and sets values accordingly
        void setWebKitState();

private:

		void setupShortcuts();

        void resizeAddressBox();

        //navigate the WebWindow's page to url
        void navigate( QString url );

        //this sets up the connection
        void setupConnections();

        //this sets up the state machine on the web window
        void setupState();

        //let other clips know that this window is no longer a clip
        void destroySharedClips();

        //the ui
        Ui::WebWindow ui;

        //qt State Machine
        QStateMachine machine;

        //saved geometry
        QRect mainGeometry;
        QRect windowGeometry;

        //masks
        QVector<QRegion> storedMasks;
        QVector<WebWindow*> sharedClips;

        //state flags
        bool geometrySet;
        bool clipped;

        QWidget *wwparent;
        QNetworkCookieJar *cookieJar;
};

#endif //WEBWINDOW_H
