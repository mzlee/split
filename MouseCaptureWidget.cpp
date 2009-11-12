#include "MouseCaptureWidget.h"

MouseCaptureWidget::MouseCaptureWidget(QWidget* parent) : QWidget(parent) {
        startPoint = QPoint();
        endPoint = QPoint();
        penWidth = 1;
}

QRegion MouseCaptureWidget::getCapturedRegion() {
    return QRegion(QRect(startPoint, endPoint), QRegion::Rectangle);
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
}

//setup the painter brush
void MouseCaptureWidget::setupPainter(QPainter & painter) {
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::black, penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
 }
