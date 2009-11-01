#ifndef WEBWINDOW_H
#define WEBWINDOW_H
#include "ui_WebWindow.h"
#include <QString>
#include <QWidget>
#include <QFocusEvent>
#include <QStateMachine>
#include <QByteArray>
#include <QMainWindow>

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
	void changeEvent( QEvent *event );

        //this is a virtual event function that updates the size of the webview and the mouse tracking
        void resizeEvent(QResizeEvent *);

        //this sets up the state machine on the web window
        void setupState();

private slots:
	
	void updateStatus(const QString &q);
	
    //this creates the basic mask
    void createMask(QRegion region);
	
	//navigate the most recently selected window to the url in the address bar
	//or create a new window and navigate 
	void go();

	//opens a new top level window which contains a browser
	void newWindow();

	//enters the clipping mode
    void startClippingMode();

	//exits the clipping mode
	void exitClippingMode();

	//moves the browser forward
	void forward();

	//moves the browser backwards
	void back();

	//restores a clip to its main size
	void restoreClip();
private:
		
		void reload();
		
		//navigate the WebWindow's page to url
		void navigate( QString url );

        void startClipping();

        void exitClipping();

        //this removes the basic mask
        void removeMask();
		void setupConnections();

        Ui::WebWindow ui;

        QPalette overlay_palette;

        bool focus;

        // qt State Machine
        QStateMachine machine;

        // window geometry
        QByteArray window_geometry;
		
		QWidget *wwparent;
};

#endif //WEBWINDOW_H
