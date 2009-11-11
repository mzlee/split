#ifndef WEBWINDOW_H
#define WEBWINDOW_H
#include "ui_WebWindow.h"
#include <QString>
#include <QWidget>
#include <QFocusEvent>
#include <QStateMachine>
#include <QByteArray>
#include <QMainWindow>
#include <vector>

class WebWindow : public QMainWindow {

	Q_OBJECT //this needs to be here for signals/slots

public:

	WebWindow( QWidget *parent = 0 );

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

        //this creates the basic mask
        void createMask(QRegion region);

        //stores the mask to later be used
        void storeMask(QRegion region);

        //this removes the basic mask
        void removeMask();
	
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

        void setClip(QRegion region);

	//moves the browser forward
	void forward();

	//moves the browser backwards
	void back();

	//restores a clip to its main size
	void restoreClip();

private:

        void reload();

        //this sets up the connection
        void setupConnections();

        //this sets up the state machine on the web window
        void setupState();

        //navigate the WebWindow's page to url
        void navigate( QString url );

        //let other clips know that this window is no longer a clip
        void removeThisSharedClipFromOtherClips();

        //the ui
        Ui::WebWindow ui;

        //qt State Machine
        QStateMachine machine;

        //saved geometry
        QRect mainGeometry;
        QRect windowGeometry;

        //masks
        std::vector<QRegion> storedMasks;
        std::vector<WebWindow*> sharedClips;

        //state flags
        //TODO: Put this into the finite state machine
        bool geometrySet;
        bool clipped;

        QWidget *wwparent;
};

#endif //WEBWINDOW_H