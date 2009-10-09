#include "splitwindow.h"
#include "ui_splitwindow.h"

SplitWindow::SplitWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::SplitWindow)
{
    ui->setupUi(this);
}

SplitWindow::~SplitWindow()
{
    delete ui;
}
