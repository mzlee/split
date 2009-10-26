#ifndef WEBWINDOW_H
#define WEBWINDOW_H

#include "ui_WebWindow.h"
#include <QString>
#include <QWidget>
#include <QFocusEvent>


class WebWindow : public QDialog{

	Q_OBJECT //this needs to be here for signals/slots

public:
	
	WebWindow( QWidget *parent = 0 );
	
	//navigate the WebWindow's page to url
	void navigate( QString url );

signals:
	
	//this is emitted when a window gains focus
	void wwFocus(WebWindow *ww);

protected:
	
	//this is a virtual event function that must be implemented
	//(used to keep track of window focus)
	void changeEvent( QEvent *event );

private:

	Ui::WebWindow ui;
	bool focusCounter;

};

#endif //WEBWINDOW_H
