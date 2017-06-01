#include "CardWidget.h"

#include <QMouseEvent>
#include <QPainter>

QColor getColor(Card::Data card) {
    if (card.isColorless()) {
        return Qt::darkGray;
    }
    Qt::GlobalColor colors[] = {Qt::red, Qt::green, Qt::blue, Qt::yellow};
    return QColor(colors[card.color]).lighter();
}

QString getText(Card::Data card) {
    if (card.isColorless()) {
        switch (card.asColorless.type){
            case Card::COLOR_CHOOSE:
                return "C";
            case Card::PLUS_4:
                return "+4";
        }
    }
    else if (card.isSpecial()) {
        switch (card.asSpecial.type){
            case Card::PLUS_2:
                return "+2";
            case Card::REVERSE:
                return u8"↑↓";
            case Card::SKIP:
                return "X";
        }
    }
    else {
        return QString("%1").arg(card.asNumber.number);
    }
}

CardWidget::CardWidget(QWidget* parent, Card::Data card)
        : QFrame(parent), card(card) {
    setupUi(this);
    numberNameLabel->setText(getText(card));
}

void CardWidget::paintEvent(QPaintEvent* event) {
    QPainter p(this);
    QPointF size(width(), height());
    QRadialGradient g(size/2, qMin(size.x(), size.y())/2);
    g.setColorAt(0.5, Qt::white);
    QColor color = getColor(card);
    g.setColorAt(1, color);
    QBrush b(g);
    p.fillRect(rect(), color);
    p.scale(0.5, 1);
    p.fillRect(rect(), b);
}

void CardWidget::on_send_clicked() {
    emit clickPlaceCard();
}
