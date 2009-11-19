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

MouseCaptureWidget::MouseCaptureWidget(QWidget* parent) : QWidget(parent) {
        startPoint = QPoint();
        endPoint = QPoint(size().width(), size().height());
        penWidth = 1;
        update();
}

QRegion MouseCaptureWidget::getCapturedRegion() {
    int l = startPoint.x() < endPoint.x() ? startPoint.x() : endPoint.x();
    int t = startPoint.y() < endPoint.y() ? startPoint.y() : endPoint.y();
    int w = startPoint.x() > endPoint.x() ? startPoint.x() - l : endPoint.x() - l;
    int h = startPoint.y() > endPoint.y() ? startPoint.y() - t : endPoint.y() - t;
    return QRegion(QRect(l, t, w, h), QRegion::Rectangle);
}

void MouseCaptureWidget::paintEvent(QPaintEvent * ) {
    QPainter painter(this);
    setupPainter(painter);

    int xFlair = startPoint.x() < endPoint.x() ? 10 : -10;
    int yFlair = startPoint.y() < endPoint.y() ? 10 : -10;

    painter.drawLine(startPoint.x() - xFlair, startPoint.y(), endPoint.x() +  xFlair, startPoint.y());
    painter.drawLine(startPoint.x(), startPoint.y() - yFlair, startPoint.x(), endPoint.y() + yFlair);
    painter.drawLine(startPoint.x() - xFlair, endPoint.y(), endPoint.x() + xFlair, endPoint.y());
    painter.drawLine(endPoint.x(), startPoint.y() - yFlair, endPoint.x(), endPoint.y() + yFlair);
}

void MouseCaptureWidget::mousePressEvent(QMouseEvent * event ) {
    if (event->button() == Qt::LeftButton) {
        startPoint = event->pos();
        emit(mcStartCapture(this));
    }
}

void MouseCaptureWidget::mouseMoveEvent(QMouseEvent * event ) {
    endPoint = event->pos();
    update();
}

void MouseCaptureWidget::mouseReleaseEvent(QMouseEvent * event ) {
    endPoint = event->pos();
    if((startPoint - endPoint).manhattanLength() > 5)
    {
        emit(mcEndCapture(QRegion(QRect(startPoint, endPoint), QRegion::Rectangle)));
        return;
    }

    for(int i=0; i<polygon.size(); i++)
    {
        if((startPoint - polygon[i]).manhattanLength() < 20)
        {
            emit(mcEndCapture(QRegion(polygon)));
            return;
        }
    }
    polygon << startPoint;
}

//setup the painter brush
void MouseCaptureWidget::setupPainter(QPainter & painter) {
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::black, penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
 }
