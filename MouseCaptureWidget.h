#ifndef MOUSECAPTUREWIDGET_H
#define MOUSECAPTUREWIDGET_H
#include <QWidget>
#include <QRegion>
#include <QPainter>
#include <QMouseEvent>

class MouseCaptureWidget : public QWidget {

        Q_OBJECT //this needs to be here for signals/slots

public:

        MouseCaptureWidget(QWidget* parent = 0);

        QRegion getCapturedRegion();

signals:

        void mcStartCapture(MouseCaptureWidget *mc);
        void mcEndCapture(QRegion);

protected:

        void paintEvent ( QPaintEvent * );

        void mouseMoveEvent ( QMouseEvent * event );

        void mousePressEvent ( QMouseEvent * event );

        void mouseReleaseEvent ( QMouseEvent * event );

private:

        void setupPainter(QPainter& painter);
        int penWidth;

        QPoint startPoint;
        QPoint endPoint;

};

#endif // MOUSECAPTUREWIDGET_H
