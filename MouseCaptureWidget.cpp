//////////////////////////////////////////////////
//                                              //
// MouseCaptureWidget.cpp                       //
//   -> part of Split                           //
//                                              //
// Copyright (c) 2009 Jim Brusstar, Michael Lee //
//                 Ben Montgomery, Robert Steen //
//                                              //
//////////////////////////////////////////////////

#include "MouseCaptureWidget.h"
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>

MouseCaptureWidget::MouseCaptureWidget(QWidget *parent)
    : QWidget(parent)
{
    _startPoint = QPoint();
    _endPoint = QPoint(size().width(), size().height());
    _penWidth = 1;
    update();
}

QRegion MouseCaptureWidget::getCapturedRegion()
{
    // Returns the mouse-selected region rect
    // XXX THIS FUNCTION NEEDS DOCUMENTATION.
    int l = _startPoint.x() < _endPoint.x() ? _startPoint.x() : _endPoint.x();
    int t = _startPoint.y() < _endPoint.y() ? _startPoint.y() : _endPoint.y();
    int w = _startPoint.x() > _endPoint.x() ? _startPoint.x() - l : _endPoint.x() - l;
    int h = _startPoint.y() > _endPoint.y() ? _startPoint.y() - t : _endPoint.y() - t;
    return QRegion(QRect(l, t, w, h), QRegion::Rectangle);
}

_OVERRIDE_ void MouseCaptureWidget::paintEvent(QPaintEvent *)
{
    // Draws the bounding box (?)
    // XXX THIS FUNCTION NEEDS DOCUMENTATION
    QPainter painter(this);
    _setupPainter(painter);
    int xFlair = _startPoint.x() < _endPoint.x() ? 10 : -10;
    int yFlair = _startPoint.y() < _endPoint.y() ? 10 : -10;

    painter.drawLine(_startPoint.x() - xFlair, _startPoint.y(), _endPoint.x() +  xFlair, _startPoint.y());
    painter.drawLine(_startPoint.x(), _startPoint.y() - yFlair, _startPoint.x(), _endPoint.y() + yFlair);
    painter.drawLine(_startPoint.x() - xFlair, _endPoint.y(), _endPoint.x() + xFlair, _endPoint.y());
    painter.drawLine(_endPoint.x(), _startPoint.y() - yFlair, _endPoint.x(), _endPoint.y() + yFlair);
    return;
}

_OVERRIDE_ void MouseCaptureWidget::mousePressEvent(QMouseEvent *event)
{
    // Handler for mouse clicks, signals capture starts
    if (event->button() == Qt::LeftButton)
    {
        _startPoint = event->pos();
        emit(mcStartCapture(this));
    }
    return;
}

_OVERRIDE_ void MouseCaptureWidget::mouseMoveEvent(QMouseEvent *event)
{
    // Handler for mouse moves, adjusts bounding box
    _endPoint = event->pos();
    update();
    return;
}

_OVERRIDE_ void MouseCaptureWidget::mouseReleaseEvent(QMouseEvent *event)
{
    // Handler for mouse button-release, signals capture stop
    _endPoint = event->pos();
    if ((_startPoint - _endPoint).manhattanLength() > 5)
    {
        emit(mcEndCapture(QRegion(QRect(_startPoint, _endPoint), QRegion::Rectangle)));
        return; // XXX TRY NOT TO EARLY RETURN, IT'S BAD FOR BUSINESS.
    }
    for (int i = 0; i < _polygon.size(); ++i)
    {
        if ((_startPoint - _polygon[i]).manhattanLength() < 20)
        {
            emit(mcEndCapture(QRegion(_polygon)));
            return; // XXX WHAT I SAID BEFORE ABOUT EARLY RETURNS STILL STANDS.
        }
    }
    _polygon << _startPoint;
    return;
}

void MouseCaptureWidget::_setupPainter(QPainter &painter)
{
    // Initialize the brush for the painter
    // XXX WHY ISN'T THIS PROCEDURE PART OF THE PAINTER INIT OR CONSTRUCTOR?
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::black, _penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    return;
 }
