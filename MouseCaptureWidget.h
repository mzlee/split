//////////////////////////////////////////////////
//                                              //
// MouseCaptureWidget.h                         //
//   -> part of Split                           //
//                                              //
// Copyright (c) 2009 Jim Brusstar, Michael Lee //
//                 Ben Montgomery, Robert Steen //
//                                              //
//////////////////////////////////////////////////

#ifndef MOUSECAPTUREWIDGET_H
#define MOUSECAPTUREWIDGET_H

#include <QWidget>

class MouseCaptureWidget : public QWidget
{
    Q_OBJECT

public:
    MouseCaptureWidget(QWidget* parent = 0);
    QRegion getCapturedRegion();

signals:
    void mcStartCapture(MouseCaptureWidget *mc);
    void mcEndCapture(QRegion);

protected:
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    void setupPainter(QPainter& painter);

    int penWidth;
    QPoint startPoint;
    QPoint endPoint;
    QPolygon polygon;
};

#endif // MOUSECAPTUREWIDGET_H
