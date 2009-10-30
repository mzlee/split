#ifndef WEBWINDOW_H
#define WEBWINDOW_H
#include "ui_WebWindow.h"
#include <QObject>
#include <QString>
#include <QWidget>
#include <QFocusEvent>
#include <QStateMachine>
#include <QByteArray>

class WebWindow : public QDialog {

	Q_OBJECT //this needs to be here for signals/slots

public:

	WebWindow( QWidget *parent = 0 );
	
	//navigate the WebWindow's page to url
	void navigate( QString url );

        void startClipping();

        void exitClipping();

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

private:

        Ui::WebWindow ui;

        QPalette overlay_palette;

        bool focusCounter;

        // qt State Machine
        QStateMachine machine;

        // window geometry
        QByteArray window_geometry;
};

#endif //WEBWINDOW_H
