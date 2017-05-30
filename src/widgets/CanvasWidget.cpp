#include <QtCore/QElapsedTimer>
#include "widgets/CanvasWidget.h"

void CanvasWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() & Qt::LeftButton) {
        emit beginDrag();
    }
}

void CanvasWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() & Qt::LeftButton) {
        emit endDrag();
    }
}

void CanvasWidget::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        emit drag(event->pos());
    }
}

CanvasWidget::CanvasWidget(QWidget *parent) : QWidget(parent) {}
