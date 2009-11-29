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

#define _OVERRIDE_

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

private:
    _OVERRIDE_ void paintEvent(QPaintEvent *);
    _OVERRIDE_ void mouseMoveEvent(QMouseEvent *event);
    _OVERRIDE_ void mousePressEvent(QMouseEvent *event);
    _OVERRIDE_ void mouseReleaseEvent(QMouseEvent *event);
    void _setupPainter(QPainter& painter);

    int _penWidth;
    QPoint _startPoint, _endPoint;
    QPolygon _polygon;
};

#endif // MOUSECAPTUREWIDGET_H
