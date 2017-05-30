#pragma once

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>

class CanvasWidget : public QWidget {
Q_OBJECT
public:
    explicit CanvasWidget(QWidget *parent);

signals:
    void debugInfo(int linesCount, int paintTime);

    void beginDrag();
    void endDrag();
    void drag(const QPoint& pos);

protected:
    void mousePressEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;
};

