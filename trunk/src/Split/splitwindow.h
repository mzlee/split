#ifndef SPLITWINDOW_H
#define SPLITWINDOW_H

#include <QtGui/QMainWindow>

namespace Ui
{
    class SplitWindow;
}

class SplitWindow : public QMainWindow
{
    Q_OBJECT

public:
    SplitWindow(QWidget *parent = 0);
    ~SplitWindow();

private:
    Ui::SplitWindow *ui;
};

#endif // SPLITWINDOW_H
