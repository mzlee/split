#include "MouseCaptureWidget.h"

MouseCaptureWidget::MouseCaptureWidget(QWidget* parent) : QWidget(parent) {
        start_point = QPoint();
        penWidth = 1;
}

QRegion MouseCaptureWidget::getCapturedRegion() {
        return captured_region;
}

void MouseCaptureWidget::paintEvent(QPaintEvent * ) {
    int xFlair = start_point.x() < end_point.x() ? 10 : -10;
    int yFlair = start_point.y() < end_point.y() ? 10 : -10;
    QPainter painter(this);
    setupPainter(painter);
    painter.drawLine(start_point.x() - xFlair, start_point.y(), end_point.x() +  xFlair, start_point.y());
    painter.drawLine(start_point.x(), start_point.y() - yFlair, start_point.x(), end_point.y() + yFlair);
    painter.drawLine(start_point.x() - xFlair, end_point.y(), end_point.x() + xFlair, end_point.y());
    painter.drawLine(end_point.x(), start_point.y() - yFlair, end_point.x(), end_point.y() + yFlair);
}

void MouseCaptureWidget::mousePressEvent(QMouseEvent * event ) {
    if (event->button() == Qt::LeftButton) {
        start_point = event->pos();
        emit(mcStartCapture(this));
    }
}

void MouseCaptureWidget::mouseMoveEvent(QMouseEvent * event ) {
    end_point = event->pos();
    update();
}

void MouseCaptureWidget::mouseReleaseEvent(QMouseEvent * event ) {
    end_point = event->pos();
    captured_region = QRegion(QRect(start_point, end_point), QRegion::Rectangle);
    emit(mcEndCapture(this));
}

void MouseCaptureWidget::setupPainter(QPainter & painter) {
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::black, penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
 }
