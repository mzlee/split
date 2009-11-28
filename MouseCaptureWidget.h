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
    void _paintEvent(QPaintEvent *);
    void _mouseMoveEvent(QMouseEvent *event);
    void _mousePressEvent(QMouseEvent *event);
    void _mouseReleaseEvent(QMouseEvent *event);

private:
    void _setupPainter(QPainter& painter);

    int _penWidth;
    QPoint _startPoint, _endPoint;
    QPolygon _polygon;
};

#endif // MOUSECAPTUREWIDGET_H
