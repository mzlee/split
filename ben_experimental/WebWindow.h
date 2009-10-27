#ifndef WEBWINDOW_H
#define WEBWINDOW_H

#include "ui_WebWindow.h"
#include <QString>
#include <QWidget>
#include <QFocusEvent>
#include <QStateMachine>
#include <QByteArray>

class WebWindow : public QDialog{

	Q_OBJECT //this needs to be here for signals/slots

public:

        Ui::WebWindow ui;

	WebWindow( QWidget *parent = 0 );
	
	//navigate the WebWindow's page to url
	void navigate( QString url );

//        void saveWindowSize();
//
//        void restoreWindowSize();

        void startClipping();

        void exitClipping();

signals:
	
	//this is emitted when a window gains focus
	void wwFocus(WebWindow *ww);

protected:
	
	//this is a virtual event function that must be implemented
	//(used to keep track of window focus)
	void changeEvent( QEvent *event );

//        void mousePressEvent(QMouseEvent *);
//
//        void mouseReleaseEvent(QMouseEvent *);

        void resizeEvent(QResizeEvent *);
private slots:

        //this sets up the state machine on the web window
        void setupState();

//        void recordMouseDown();
//
//        void recordMouseUp();

private:

        int start_x, start_y;
        int end_x, end_y;

        bool focusCounter;

        // qt State Machine
        QStateMachine machine;

        // window geometry
        QByteArray window_geometry;
};

#endif //WEBWINDOW_H
